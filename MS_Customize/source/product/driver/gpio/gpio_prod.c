/******************************************************************************
 ** File Name:      pinmap_sp7320.c                                          *
 ** Author:         Richard.Yang                                              *
 ** DATE:           03/08/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the pin usage and initiate value of     *
 **                 SP7100B                                                   *
 **                GPIO used information( SP7100B ), please see following     *
 **                GPIO define                                                *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2004     Richard.Yang     Create.                                   *
 ** 03/18/2004     Lin.liu          Modify for SP7100B                        *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "gpio_drv.h"
#include "cmddef.h"
#include "tb_dal.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "gpio_ext_drv.h"
#include "scm_api.h"
#include "lcd_backlight.h"
#include "deep_sleep.h"
#include "ref_outport.h"
#include "pinmap.h"
#include "task_monitor.h"
//#include "pwm_drvapi.h"
#include "boot_drvapi.h"/*lint -esym(766, chip_drv\export\inc\boot_drvapi.h)*/
#include "analog_drv.h"
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "eic_hal.h"
#endif

#ifdef DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
#endif //DUAL_BATTERY_SUPPORT

#if defined(PLATFORM_UWS6121E)
GPIO_INT_TYPE g_gpio_inttype[32];
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


static irq_handler_t GPIO_VBUS_handler = NULL;
/**---------------------------------------------------------------------------*
 **                         local Variables                                  *
 **---------------------------------------------------------------------------*/
/*
typedef struct HEADSET_Tag
{
    uint32 status;
    uint32 button_status;
    uint32 time;
}HEADSET_T;

#define  HEADSET_DISCONNECT  0
#define  HEADSET_CONNECT     1
LOCAL  HEADSET_T  s_headset_ctl;

#define  MAX_HEADSET_BUTTON_READY_TIME  1000

//LOCAL BOOLEAN is_headset_button_masked = SCI_FALSE;
*/

LOCAL GPIO_CFG_INFO_T s_gpio_prod_cfg_full_table[GPIO_PROD_ID_MAX] = {0};

//Invoked in demod interrupt handler.
#if defined(DEMOD_HW_SIANO) || defined(DEMOD_HW_INNOFIDEI)
extern void DemodSpiIsr(void* context);
#endif
//extern PUBLIC void AUDDEV_HPDetectRoutine (BOOLEAN status);



/*****************************************************************************/
//  Description:    This function is used to set product gpio value
//  Author:         Liangwen.Zhen
//  Note:           First call callback funtion, second set gpio value
/*****************************************************************************/
BOOLEAN _GPIO_PROD_SetVal (GPIO_PROD_ID_E id, BOOLEAN value)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (id);

    if (PNULL != cfg_ptr)
    {
        if (!cfg_ptr->valid_level)
        {
            value = !value;
        }

        // have Call back,execute call back function
        if (PNULL != cfg_ptr->gpio_cb_fun)
        {
            cfg_ptr->gpio_cb_fun (value);
            //return SCI_TRUE;
        }

        if (GPIO_PROD_NUM_INVALID != cfg_ptr->gpio_num)
        {
            switch (cfg_ptr->gpio_type)
            {
                case GPIO_PROD_TYPE_BB0:

                    HAL_SetGPIOVal (cfg_ptr->gpio_num, value);
                    break;

                case GPIO_PROD_TYPE_EXT:
                    GPIO_EXT_SetValue (cfg_ptr->gpio_num, value);
                    break;

                default :
                    //SCI_PASSERT(0, ("_GPIO_PROD_RegGpio : type %d", cfg_ptr->gpio_type));
                    break;
            }
        }

        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }



}
/*****************************************************************************/
//  Description:    This function is used to get product gpio value
//  Author:         Liangwen.Zhen
//  Note:           First to check if there is the valid hardware GPIO,
//                  or call call-back function
/*****************************************************************************/
LOCAL BOOLEAN _GPIO_PROD_GetVal (GPIO_PROD_ID_E  id, BOOLEAN *value_ptr)
{

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (id);

    if (PNULL != cfg_ptr)
    {
        if (GPIO_PROD_NUM_INVALID != cfg_ptr->gpio_num)
        {
            switch (cfg_ptr->gpio_type)
            {
                case GPIO_PROD_TYPE_BB0:
                    *value_ptr = HAL_GetGPIOVal (cfg_ptr->gpio_num);
                    break;

                case GPIO_PROD_TYPE_EXT:
                    *value_ptr = GPIO_EXT_GetValue (cfg_ptr->gpio_num);
                    break;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
                case GPIO_PROD_TYPE_EIC_DBNC:
                    *value_ptr = EIC_HAL_GetValue (cfg_ptr->gpio_num);
                    break;
#endif
                default :
                    //SCI_PASSERT(0, ("_GPIO_PROD_RegGpio : type %d", cfg_ptr->gpio_type));
                    break;
            }
        }

        else if ( (PNULL != cfg_ptr->gpio_cb_fun)
                  && (GPIO_PROD_TYPE_MAX == cfg_ptr->gpio_type))
        {
            // Call back
            *value_ptr = cfg_ptr->gpio_cb_fun (0);
        }
        else
        {
            return SCI_FALSE;
        }

        if (!cfg_ptr->valid_level)
        {
            *value_ptr = (! (*value_ptr));
        }

        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    This function is used to set gpio interrupt sense
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL void _GPIO_PROD_SetInterruptSense (GPIO_PROD_ID_E  id, GPIO_PROD_INT_TYPE type)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (id);

    if (PNULL != cfg_ptr)
    {
        if (GPIO_PROD_TYPE_BB0 ==  cfg_ptr->gpio_type)
        {
            HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, type);
        }
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        else if(GPIO_PROD_TYPE_EIC_DBNC ==  cfg_ptr->gpio_type)
        {
            EIC_HAL_SetIntSense(cfg_ptr->gpio_num, type);
        }
#endif
        else
        {

        }
    }


}


/*****************************************************************************/
//  Description:    This function is used to set gpio direction
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void _GPIO_PROD_SetDirection (GPIO_PROD_ID_E  id, BOOLEAN is_output)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (id);

    if (PNULL != cfg_ptr)
    {
        if (GPIO_PROD_TYPE_BB0 ==  cfg_ptr->gpio_type)
        {
            HAL_SetGPIODirection (cfg_ptr->gpio_num, is_output);
        }
    }


}

/*****************************************************************************/
//  Description:    This function check if speaker pa and headset pa share one PA or not.
//   return  val:    SCI_TRUE  :   share
//                       SCI_FALSE :   no share
//  Author:          hyy
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  _GPIO_Check_Amplifier_Share (void)
{
    GPIO_CFG_INFO_T_PTR speaker_pa_cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_SPEAKER_PA_EN_ID);
    GPIO_CFG_INFO_T_PTR headset_pa_cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_HEADSET_PA_EN_ID);

    if ( (PNULL == speaker_pa_cfg_ptr) || (PNULL == headset_pa_cfg_ptr))
    {
        return SCI_FALSE;
    }

    if (speaker_pa_cfg_ptr->gpio_num == headset_pa_cfg_ptr->gpio_num)
    {
        return SCI_TRUE;
    }

    return SCI_FALSE;

}

/*****************************************************************************/
//  Description:    This function works when speaker pa and headset pa share one PA .
//  pa_type:        SCI_FALSE: speaker pa         SCI_TRUE:headset pa
//  b_enable:      enable selected pa
//  Author:          hyy
//  Note:
/*****************************************************************************/
LOCAL void  _GPIO_Control_Amplifier_Shared (BOOLEAN pa_type, BOOLEAN b_enable)
{
    if (!_GPIO_Check_Amplifier_Share())
    {
        return;  // no share
    }

    //when share
#define _SHARED_PA_STATE_X_X_   0   //speaker dis,  headset dis
#define _SHARED_PA_STATE_S_X_   1   //SPEAKER EN,   headset dis
#define _SHARED_PA_STATE_X_H_   2   //speaker dis,  HEADSER EN,
#define _SHARED_PA_STATE_S_H_   3   //SPEAKER EN,   HEADSER EN,

    {
        static uint8 shared_pa_state = _SHARED_PA_STATE_X_X_;

        switch (shared_pa_state)
        {
            case _SHARED_PA_STATE_X_X_:

                if (b_enable)
                {
                    if (pa_type)   //headset
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_HEADSET_PA_EN_ID, SCI_TRUE);
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_TRUE);  // SCI_TRUE MEANS :SHARED PA OUT = HEADSET
                        shared_pa_state = _SHARED_PA_STATE_X_H_;
                    }
                    else            //speaker
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_SPEAKER_PA_EN_ID, SCI_TRUE);
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_FALSE);  // SCI_FALSE MEANS :SHARED PA OUT = ALL
                        shared_pa_state = _SHARED_PA_STATE_S_X_;
                    }
                }

                break;

            case _SHARED_PA_STATE_S_X_:

                if (!pa_type)     //speaker
                {
                    if (!b_enable)
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_SPEAKER_PA_EN_ID, SCI_FALSE);
                        shared_pa_state = _SHARED_PA_STATE_X_X_;
                    }
                }
                else                //headset
                {
                    if (b_enable)
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_HEADSET_PA_EN_ID, SCI_TRUE);
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_FALSE);  // SCI_TRUE MEANS :SHARED PA OUT = ALL
                        shared_pa_state = _SHARED_PA_STATE_S_H_;
                    }
                }

                break;

            case _SHARED_PA_STATE_X_H_:

                if (pa_type)    //headset
                {
                    if (!b_enable)
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_HEADSET_PA_EN_ID, SCI_FALSE);
                        shared_pa_state = _SHARED_PA_STATE_X_X_;
                    }
                }
                else                 //speaker
                {
                    if (b_enable)
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_SPEAKER_PA_EN_ID, SCI_TRUE);
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_FALSE);  // SCI_TRUE MEANS :SHARED PA OUT = ALL
                        shared_pa_state = _SHARED_PA_STATE_S_H_;
                    }
                }

                break;

            case _SHARED_PA_STATE_S_H_:

                if (!b_enable)
                {
                    if (pa_type)   //headset
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_FALSE);  // SCI_TRUE MEANS :SHARED PA OUT = ALL
                        shared_pa_state = _SHARED_PA_STATE_S_X_;
                    }
                    else            //speaker
                    {
                        _GPIO_PROD_SetVal (GPIO_PROD_SHARED_PA_SW_ID,SCI_TRUE);  // SCI_FALSE MEANS :SHARED PA OUT = HEADSET
                        shared_pa_state = _SHARED_PA_STATE_X_H_;
                    }
                }

                break;
            default:
                break;
        }
    }
}


/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                             *
 **---------------------------------------------------------------------------*/
#ifdef MOTION_SENSOR_TYPE_OPTICAL_SCM013
PUBLIC void GPIO_ProximitySensorTXEn (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal(GPIO_PROD_PROXSENSOR_TX_ID,b_on);
}

PUBLIC void GPIO_ProximitySensorRXEn (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal(GPIO_PROD_PROXSENSOR_RX_ID,b_on);
}
#endif
/*****************************************************************************/
//  Description:    This function is used to set udc driver reg isr handler
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_VBUS_RegIntHandler(irq_handler_t irq_handler)
{
    if(irq_handler != NULL)
    {
        GPIO_VBUS_handler = irq_handler;
    }
}

/*****************************************************************************/
//  Description:    This function is used to call udc Callback function
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_VBUS_Callback(uint32 gpio_state)
{
    if(GPIO_VBUS_handler != NULL)
    {
        GPIO_VBUS_handler(gpio_state);
    }
}

/*****************************************************************************/
//  Description:    This function is used to handle charge int
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
extern void Debug_Uart4_puts(char *Str);
char buffsize[32];
PUBLIC void GPIO_ChargeIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    SCI_TRACE_LOW("GPIO_ChargeIntHandler");
    Debug_Uart4_puts("Enter GPIO_ChargeIntHandler\n");
    sprintf(buffsize, "ANA_CHGR_STATUS_RTC = 0x%x\n", ANA_REG_GET(ANA_CHGR_STATUS_RTC));
    Debug_Uart4_puts(buffsize);
    sprintf(buffsize, "ANA_CHGR_STATUS_RTC = 0x%x\n", ANA_REG_GET(ANA_CHGR_STATUS));
    Debug_Uart4_puts(buffsize);
    CHGMNG_ChargerPlugInHandler (gpio_id, gpio_state);
    GPIO_VBUS_Callback(gpio_state);
    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_CHARGE_PLUG_DETECT_ID, GPIO_INT_LEVEL_LOW); //plug out det
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_CHARGE_PLUG_DETECT_ID, GPIO_INT_LEVEL_HIGH); //plug in det
    }
}

#ifdef BATTERY_DETECT_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to handle charge int
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BatDetIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_BATTERY_DETECT_ID, GPIO_INT_LEVEL_LOW); //battery off det
    }
    else
    {
    CHGMNG_SendMsgToChgTask(CHGMNG_BATTERY_OFF_MSG, 0);
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_BATTERY_DETECT_ID, GPIO_INT_LEVEL_HIGH); //battery on det
    }
}
#endif

#ifdef MOTION_SENSOR_TYPE
/*****************************************************************************/
//  Description:    This function is used to handler g-sensor interrupt
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_GSensorIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    //Because Reading registers via IIC is very slow, so it CANNOT be done
    //in interrupt context.
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);

    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_GSENSOR_INT_ID, GPIO_INT_LEVEL_LOW);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_GSENSOR_INT_ID, GPIO_INT_LEVEL_HIGH);
    }

    if (cfg_ptr && (cfg_ptr->valid_level == gpio_state) && (cfg_ptr->gpio_cb_fun))
    {
        cfg_ptr->gpio_cb_fun (GPIO_PROD_GSENSOR_INT_ID);
    }

}
#endif

#ifdef DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description:    Aux Battery Detect Int Handler.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DBAT_AuxBatDetectIntHandler(uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_DBAT_ABAT_INT_ID );

    if(PNULL != cfg_ptr)
    {
        if (!gpio_state)
        {
            //SCI_TRACE_LOW:"[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug OUT !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_461_112_2_18_0_33_1_1521,(uint8*)"");
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_DBAT_ABAT_INT_ID, GPIO_INT_LEVEL_HIGH);
            DBAT_SendMsgToDBatTask (DBAT_A_BAT_PLUGOUT_MSG);
        }
        else
        {
            //SCI_TRACE_LOW:"[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug IN !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_467_112_2_18_0_33_1_1522,(uint8*)"");
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_DBAT_ABAT_INT_ID, GPIO_INT_LEVEL_LOW);
            DBAT_SendMsgToDBatTask (DBAT_A_BAT_PLUGIN_MSG);
        }
    }
}

/*****************************************************************************/
//  Description:    Main Battery Detect Int Handler.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DBAT_MainBatDetectIntHandler(uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_DBAT_MBAT_INT_ID );

    if(PNULL != cfg_ptr)
    {
        if (!gpio_state)
        {
            //SCI_TRACE_LOW:"[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug OUT !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_485_112_2_18_0_33_1_1523,(uint8*)"");
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_DBAT_MBAT_INT_ID, GPIO_INT_LEVEL_HIGH);
            DBAT_SendMsgToDBatTask (DBAT_M_BAT_PLUGOUT_MSG);
        }
        else
        {
            //SCI_TRACE_LOW:"[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug IN !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_491_112_2_18_0_33_1_1524,(uint8*)"");
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_DBAT_MBAT_INT_ID, GPIO_INT_LEVEL_LOW);
            DBAT_SendMsgToDBatTask (DBAT_M_BAT_PLUGIN_MSG);
        }
    }
}

#endif  //DUAL_BATTERY_SUPPORT

/*****************************************************************************/
//  Description:    GPIO extend intrrupt handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_TEST_IRQ(uint32 gpio_id, uint32 gpio_state)
{
	int gpio_value;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_TEST_ID);

	gpio_value=GPIO_GetValue(gpio_id);

	if(gpio_value==1)
	{
		GPIO_SetValue(30, 1);
	}

}
#if 0
PUBLIC void GPIO_xxxIntHandler(uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TEST_ID );

    if(PNULL != cfg_ptr)
    {
        if (gpio_state == 1)
        {
            GPIO_SetValue(30, 1);
        }
        else
        {
            GPIO_SetValue(30, 0);
        }
    }
}
#endif
PUBLIC void GPIO_GpioExtendINTHandler (uint32 gpio_id, uint32 gpio_state)
{
    BOOLEAN gpio_value;
    HAL_SetGPIOInterruptSense (gpio_id, GPIO_INT_LEVEL_LOW);

    gpio_value = GPIO_GetValue (gpio_id);

    if (SCI_FALSE == gpio_value)
    {
        GPIO_EXT_ISR (gpio_id);
    }
}

/*
LOCAL uint8 ucHeadSetButton = 0;
LOCAL uint8 ucHeadsetdet = 0;

PUBLIC uint8 GPIO_GetHeadsetButtonFlag(void)
{
    if(AUD_HEADSET_TYPE_INSERT4P == (AUD_HEADSET_TYPE_T)ucHeadsetdet)
    {
        return ucHeadSetButton;
    }
    return AUD_HEADSET_TYPE_PLUGOUT;
}

PUBLIC uint8 GPIO_GetHeadsetDetectFlag(void)
{
    return (uint8)(AUD_HEADSET_TYPE_INSERT4P == (AUD_HEADSET_TYPE_T)ucHeadsetdet);
}

PUBLIC void GPIO_SetHeadsetButtonFlag(uint8 ucFlag)
{
    ucHeadSetButton = ucFlag;
}

PUBLIC void GPIO_SetHeadsetDetectFlag(uint8 ucFlag)
{
    ucHeadsetdet = ucFlag;
}

/***************************************************************************** /
//  Description:    Headset button detection handler function.
//  Author:         BenjaminWang
//  Note:
/***************************************************************************** /
PUBLIC void GPIO_HeadsetButtonIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg = {0};
    uint32 tick = SCI_GetTickCount();
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_HEADSET_BUTTON_ID);

    GPIO_SetHeadsetButtonFlag(1);
    SCI_TRACE_LOW("{xx}GPIO_HeadsetButtonIntHandler gpio_state = %d",gpio_state);
   //SCI_ASSERT(0);
    if (PNULL != cfg_ptr)
    {
       /*   The available condition of Headset Button:
            1.     Headset had connected
            2.  The time of headset button interrupt raised should be lated more than 1000ms
                after headset detected interrupt raised
            3.  The pin status of headset button is available
       * /

    SCI_TRACE_LOW("{xx}GPIO_HeadsetButtonIntHandler (PNULL != cfg_ptr)");

        if (gpio_state)
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_HEADSET_BUTTON_ID, GPIO_INT_LEVEL_LOW);
        }
        else
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_HEADSET_BUTTON_ID, GPIO_INT_LEVEL_HIGH);
        }


        if ((HEADSET_CONNECT == s_headset_ctl.status) &&
            (tick  > (s_headset_ctl.time + MAX_HEADSET_BUTTON_READY_TIME)))

        {

            if (gpio_state)
            {
                if (cfg_ptr->valid_level)
                {
                    gpio_msg.message = TB_KPD_PRESSED;
                }
                else
                {
                    gpio_msg.message = TB_KPD_RELEASED;
                }

                gpio_msg.wparam  = SCI_VK_HEADSET_BUTTOM;
                gpio_msg.lparam  = TB_NULL;
            }
            else
            {
                if (!cfg_ptr->valid_level)
                {
                    gpio_msg.message = TB_KPD_PRESSED;
                }
                else
                {
                    gpio_msg.message = TB_KPD_RELEASED;
                }

                gpio_msg.wparam  = SCI_VK_HEADSET_BUTTOM;
                gpio_msg.lparam  = TB_NULL;
            }
            s_headset_ctl.button_status = gpio_msg.message;
            DRV_Callback (TB_GPIO_INT,&gpio_msg);
        }
    }
    SCI_TRACE_LOW("GPIO_HeadsetButton: headset = %d, after timer = %d, button_status = %d, gpio_state = %d",
                      s_headset_ctl.status, (tick - s_headset_ctl.time), gpio_msg.message, gpio_state );
}

/***************************************************************************** /
//  Description:    Headset detection handler function.
//  Author:         BenjaminWang
//  Note:
/***************************************************************************** /
PUBLIC void GPIO_HeadsetDetectIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg = {0};

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_HEADSET_DETECT_ID);

    SCI_TRACE_LOW("{xx}GPIO_HeadsetDetectIntHandler gpio_state = %d",gpio_state);
    if (PNULL != cfg_ptr)
    {
     SCI_TRACE_LOW("{xx}GPIO_HeadsetDetectIntHandler (PNULL != cfg_ptr)");
       if (gpio_state)
        {
            if (cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
                s_headset_ctl.status = HEADSET_CONNECT;
                GPIO_SetHeadsetDetectFlag(1);
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
                s_headset_ctl.status = HEADSET_DISCONNECT;
                GPIO_SetHeadsetDetectFlag(0);
            }

            gpio_msg.wparam  = SCI_VK_HEADSET_DETECT;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_HEADSET_DETECT_ID, GPIO_INT_LEVEL_LOW);
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_HEADSET_BUTTON_ID, GPIO_INT_LEVEL_HIGH);
        }
        else
        {
            if (!cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
                s_headset_ctl.status = HEADSET_CONNECT;
                GPIO_SetHeadsetDetectFlag(1);
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
                s_headset_ctl.status = HEADSET_DISCONNECT;
                GPIO_SetHeadsetDetectFlag(0);
            }

            gpio_msg.wparam  = SCI_VK_HEADSET_DETECT;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_HEADSET_DETECT_ID, GPIO_INT_LEVEL_HIGH);
        }

        // notify codec headphone connection status
        CODEC_SetHeadphoneConnectStatus(s_headset_ctl.status);

        if(HEADSET_CONNECT == s_headset_ctl.status)
        {
            s_headset_ctl.time = SCI_GetTickCount();
        }
        else
        {
            s_headset_ctl.time = 0;
        }

        if (TB_KPD_PRESSED == gpio_msg.message)
        {
            // If HeadSet in detected, do related routines with VB
            AUDDEV_HPDetectRoutine (SCI_TRUE);
        }
        else if (TB_KPD_RELEASED == gpio_msg.message)
        {
            // If HeadSet out detected, do related routines with VB
            AUDDEV_HPDetectRoutine (SCI_FALSE);
            // If HeadSet out detected, button need sent "paired-message" (TB_KPD_RELEASED) to UI, to avoid LCD Display always ON
            if(s_headset_ctl.button_status == TB_KPD_PRESSED)
            {
                 TB_MSG  btn_gpio_msg = {0};
                 btn_gpio_msg.message = TB_KPD_RELEASED;
                 btn_gpio_msg.wparam  = SCI_VK_HEADSET_BUTTOM;
                 btn_gpio_msg.lparam  = TB_NULL;
                 DRV_Callback (TB_GPIO_INT,&btn_gpio_msg);
                 s_headset_ctl.button_status = TB_KPD_RELEASED; // restore idle value
                 SCI_TRACE_LOW("GPIO_HeadsetDet:  sent paired- HeadsetButton (TB_KPD_RELEASED)");
            }
        }

        DRV_Callback (TB_GPIO_INT,&gpio_msg);

    }
    SCI_TRACE_LOW("GPIO_HeadsetDet: headset = %d, time = %d, gpio_state = %d",
                  s_headset_ctl.status, s_headset_ctl.time, gpio_state );

}
*/
/*****************************************************************************/
//  Description:    FlipOn detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_FlipOnIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg;

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_FLIP_ON_ID);

    if (PNULL != cfg_ptr)
    {

        if (gpio_state)
        {
            if (cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
            }

            gpio_msg.wparam  = SCI_VK_FLIP;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_FLIP_ON_ID, GPIO_INT_LEVEL_LOW);
        }
        else
        {
            if (!cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
            }

            gpio_msg.wparam  = SCI_VK_FLIP;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_FLIP_ON_ID, GPIO_INT_LEVEL_HIGH);
        }

        DRV_Callback (TB_GPIO_INT,&gpio_msg);
    }
}

/*****************************************************************************/
//  Description:    PowerKey detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PowerKeyIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_POWER_KEY_ID);
    Debug_Uart4_puts("Enter GPIO_PowerKeyIntHandler\n");
    SCI_TRACE_LOW("GPIO_PowerKeyIntHandler");

    if(NULL==cfg_ptr){
        return ;
    }
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
if (gpio_state == cfg_ptr->valid_level)
    {
        gpio_msg.message = TB_KPD_RELEASED;
        gpio_msg.wparam  = SCI_VK_POWER;
        gpio_msg.lparam  = TB_NULL;
    }
    else
    {
        gpio_msg.message = TB_KPD_PRESSED;
        gpio_msg.wparam  = SCI_VK_POWER;
        gpio_msg.lparam  = TB_NULL;
    }
#else

    if (gpio_state == cfg_ptr->valid_level)
    {
        gpio_msg.message = TB_KPD_PRESSED;
        gpio_msg.wparam  = SCI_VK_POWER;
        gpio_msg.lparam  = TB_NULL;
    }
    else
    {
        gpio_msg.message = TB_KPD_RELEASED;
        gpio_msg.wparam  = SCI_VK_POWER;
        gpio_msg.lparam  = TB_NULL;
    }
#endif
    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_POWER_KEY_ID, GPIO_INT_LEVEL_LOW);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_POWER_KEY_ID, GPIO_INT_LEVEL_HIGH);
    }

    DRV_Callback (TB_GPIO_INT,&gpio_msg);
}

/*****************************************************************************/
//  Description:    Touch panel detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_TPDetectIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_TPXL_DETECT_ID);

    if (PNULL != cfg_ptr)
    {
        if (!cfg_ptr->valid_level)
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_TPXL_DETECT_ID, GPIO_INT_LEVEL_LOW);
        }
        else
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_TPXL_DETECT_ID, GPIO_INT_LEVEL_HIGH);
        }

        if (PNULL != cfg_ptr->gpio_cb_fun)
        {
            if (!cfg_ptr->valid_level)
            {
                gpio_state = !gpio_state;
            }

            cfg_ptr->gpio_cb_fun (gpio_state);
        }

    }

}


/*****************************************************************************/
//  Description:    PowerKey detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BackendICIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    //Interrupt_Processing(gpio_id, gpio_state);
}

#ifndef MODEM_PLATFORM
#ifdef SDCARD_SUPPORT
/*****************************************************************************/
//  Description:    SDCard detection handler function.
//  Author:         juan.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SdcardDetectIntHandler (uint32 gpio_id, uint32 gpio_state)
{
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
#define    SCM_SLOT_DETECT     SCM_SLOT_0
#else
#define    SCM_SLOT_DETECT     SCM_SLOT_1
#endif

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_SDCARD_DETECT_ID);

    if (PNULL != cfg_ptr)
    {
        if (gpio_state)
        {
            if (cfg_ptr->valid_level)
            {
                //Send message to SCM
                SCM_PlugIn (SCM_SLOT_DETECT);
            }
            else
            {
                //Send message to SCM
                SCM_PlugOut (SCM_SLOT_DETECT);
            }

            _GPIO_PROD_SetInterruptSense (GPIO_PROD_SDCARD_DETECT_ID, GPIO_INT_LEVEL_LOW);
        }
        else
        {
            if (!cfg_ptr->valid_level)
            {
                //Send message to SCM
                SCM_PlugIn (SCM_SLOT_DETECT);
            }
            else
            {
                //Send message to SCM
                SCM_PlugOut (SCM_SLOT_DETECT);
            }

            _GPIO_PROD_SetInterruptSense (GPIO_PROD_SDCARD_DETECT_ID, GPIO_INT_LEVEL_HIGH);
        }
    }

}


/*****************************************************************************/
//  Description:    Customer 1 detection handler function.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Cus1DetectIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T cfg_info = {0};
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_CUS_1_DETECT_ID);

    SCI_ASSERT (PNULL != cfg_ptr);/*assert verified*/

    cfg_info.gpio_id    = GPIO_PROD_SDCARD_DETECT_ID;
    cfg_info.gpio_type  = GPIO_PROD_TYPE_EXT;
    cfg_info.gpio_num   = cfg_ptr->gpio_num; /*lint !e613*/ // when cfg_ptr is null pointer, aseert
    cfg_info.valid_level= GPIO_PROD_HIGH_LEVEL;

    GPIO_PROD_SetCfgInfo (&cfg_info);

    GPIO_SelectSPI2EXTCS (0);

    GPIO_SdcardDetectIntHandler (gpio_id, gpio_state);
}

/*****************************************************************************/
//  Description:    Customer 2 detection handler function.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Cus2DetectIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T cfg_info = {0};
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_CUS_2_DETECT_ID);

    SCI_ASSERT (PNULL != cfg_ptr);/*assert verified*/

    cfg_info.gpio_id    = GPIO_PROD_SDCARD_DETECT_ID;
    cfg_info.gpio_type  = GPIO_PROD_TYPE_EXT;
    cfg_info.gpio_num   = cfg_ptr->gpio_num; /*lint !e613*/ // when cfg_ptr is null pointer, aseert
    cfg_info.valid_level= GPIO_PROD_LOW_LEVEL;

    GPIO_PROD_SetCfgInfo (&cfg_info);

    GPIO_SelectSPI2EXTCS (5);

    GPIO_SdcardDetectIntHandler (gpio_id, gpio_state);
}
#endif
#endif


/*****************************************************************************/
//  Description:    Camera cover detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_CameraCoverDetectHandler (uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_FLIP_ON_ID);

    if (PNULL != cfg_ptr)
    {
        if (gpio_state)
        {
            if (cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
            }

            //gpio_msg.wparam  = SCI_VK_CAMERA_COVER;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_FLIP_ON_ID, GPIO_INT_LEVEL_LOW);

        }
        else
        {
            if (!cfg_ptr->valid_level)
            {
                gpio_msg.message = TB_KPD_PRESSED;
            }
            else
            {
                gpio_msg.message = TB_KPD_RELEASED;
            }

            // gpio_msg.wparam  = SCI_VK_CAMERA_COVER;
            gpio_msg.lparam  = TB_NULL;
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_FLIP_ON_ID, GPIO_INT_LEVEL_HIGH);

        }

        DRV_Callback (TB_GPIO_INT,&gpio_msg);
    }

}

/*****************************************************************************/
//  Description:    LCD Frame mark detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_LCDFrameMarkDetectHandler (uint32 gpio_id, uint32 gpio_state)
{


    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_LCD_FMARK_DETECT_ID);

    //SCI_TRACE_LOW:"GPIO_WiFiIrqHander()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_935_112_2_18_0_33_2_1525,(uint8*)"");

    if (PNULL != cfg_ptr)
    {
        if (cfg_ptr->gpio_cb_fun && (cfg_ptr->valid_level == gpio_state))
        {
            cfg_ptr->gpio_cb_fun (0);
        }

    }
}

/*****************************************************************************/
//  Description:    Double key first action detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DKeyFirstActionHandler (uint32 gpio_id, uint32 gpio_state)
{
    if (!gpio_state)
    {
        //    HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_HIGH);
        //    DKey_ChangeStarte(SPECIAL_KEY_DK_FIRST_PRESS);
    }
    else
    {
        //    HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_LOW);
        //    DKey_ChangeStarte(SPECIAL_KEY_DK_RELEASE);
    }
}

/*****************************************************************************/
//  Description:    Double key secord action detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DKeySecordActionHandler (uint32 gpio_id, uint32 gpio_state)
{
    if (!gpio_state)
    {
        //    HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_HIGH);
        //    DKey_ChangeStarte(SPECIAL_KEY_DK_SECORD_PRESS);
    }
    else
    {
        //No need send message again.
        //    HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_LOW);
    }
}


/*****************************************************************************/
//  Description:    Bluetooth interrupte detection handler function.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BTIntHandler (
    uint32 gpio_id,
    uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_BT_REQ_CLK_ID);
    BOOLEAN             is_open = SCI_FALSE;

    if (PNULL != cfg_ptr)
    {
        if (gpio_state)
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_BT_REQ_CLK_ID,GPIO_INT_LEVEL_LOW);
        }
        else
        {
            _GPIO_PROD_SetInterruptSense (GPIO_PROD_BT_REQ_CLK_ID,GPIO_INT_LEVEL_HIGH);
        }

        if (gpio_state == cfg_ptr->valid_level)
        {
            is_open = SCI_TRUE;
        }

        DPSLP_XTLOpenInSleep (is_open);

        HAL_EnableGPIOIntCtl (gpio_id);

    }
}

/*****************************************************************************/
//  Description:    MicorUSB interrupte detection handler function.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_MicroUSBIntHandler (
        uint32 gpio_id,
        uint32 gpio_state)
{
      GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_MICRO_USB_INT_ID );

       if(PNULL != cfg_ptr)
    {
           if (gpio_state)
           {
              _GPIO_PROD_SetInterruptSense(GPIO_PROD_MICRO_USB_INT_ID,GPIO_INT_LEVEL_LOW);
           }
           else
           {
              _GPIO_PROD_SetInterruptSense(GPIO_PROD_MICRO_USB_INT_ID,GPIO_INT_LEVEL_HIGH);
           }

        if(cfg_ptr->gpio_cb_fun && (cfg_ptr->valid_level == gpio_state))
        {
            cfg_ptr->gpio_cb_fun(0);
        }

    }
}

/*****************************************************************************/
//  Description:    This function is used to Register product gpio
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
BOOLEAN GPIO_PROD_RegGpio (
    GPIO_PROD_ID_E          id,
    BOOLEAN                 direction,        // SCI_FALSE: Input dir; SCI_TRUE: output dir
    BOOLEAN                 default_value,    //
    BOOLEAN                 shaking_en,
    uint32                  shaking_interval,
    GPIO_PROD_CALLBACK      gpio_callback_fun
)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (id);
    PM_IS_E int_sense;

    SCI_TRACE_LOW("{xx}GPIO_PROD_RegGpio");
    if (cfg_ptr == PNULL)
    {
        return SCI_FALSE;
    }

    SCI_TRACE_LOW("{xx}GPIO_PROD_RegGpio:gpio_id=%d, gpio_type=%d,valid_level=%d,gpio_num=%d,",
        cfg_ptr->gpio_id,cfg_ptr->gpio_type,cfg_ptr->valid_level,cfg_ptr->gpio_num);
    int_sense = PM_GetGPIOIntType (cfg_ptr->gpio_num);


    switch (cfg_ptr->gpio_type)
    {
        case GPIO_PROD_TYPE_BB0:

            switch (int_sense)
            {
                case PM_LEVEL:
#if defined(CHIP_VER_UWS6121E)
                    if ( !HAL_GetGPIOVal (cfg_ptr->gpio_num) || (cfg_ptr->gpio_num == 6))//RTC INT use hight leve triger
#else
                    if ( !HAL_GetGPIOVal (cfg_ptr->gpio_num) || (cfg_ptr->gpio_num == 7))//RTC INT use hight leve triger
#endif
                    {
                        HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, GPIO_INT_LEVEL_HIGH);
                        g_gpio_inttype[cfg_ptr->gpio_num] = GPIO_INT_LEVEL_HIGH;
                    }
                    else
                    {
                        HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, GPIO_INT_LEVEL_LOW);
                        g_gpio_inttype[cfg_ptr->gpio_num] = GPIO_INT_LEVEL_LOW;
                    }
                    break;

                case PM_RISING_EDGE:
                    HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, GPIO_INT_EDGES_RISING);
                    g_gpio_inttype[cfg_ptr->gpio_num] = GPIO_INT_EDGES_RISING;
                    break;

                case PM_FALLING_EDGE:
                    HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, GPIO_INT_EDGES_FALLING);
                    g_gpio_inttype[cfg_ptr->gpio_num] = GPIO_INT_EDGES_FALLING;
                    break;

                case PM_BOTH_EDGE:
                    HAL_SetGPIOInterruptSense (cfg_ptr->gpio_num, GPIO_INT_EDGES_BOTH);
                    g_gpio_inttype[cfg_ptr->gpio_num] = GPIO_INT_EDGES_BOTH;
                    break;

                default:
                    //SCI_TRACE_LOW:"GPIO_PROD_RegGpio - INT type error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1122_112_2_18_0_33_3_1526,(uint8*)"");
                    return SCI_FALSE;
            }

            HAL_AddGPIOToCallbackTable (cfg_ptr->gpio_num, shaking_en, shaking_interval,
                                        (GPIO_CALLBACK) gpio_callback_fun);
            break;

        case GPIO_PROD_TYPE_EXT:
            GPIO_EXT_RegGpio (cfg_ptr->gpio_num, direction, default_value,gpio_callback_fun);
            break;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        case GPIO_PROD_TYPE_EIC_DBNC:
            SCI_TRACE_LOW("cfg_ptr->gpio_num=%d, EIC_HAL_GetValue (cfg_ptr->gpio_num) = %d",cfg_ptr->gpio_num,EIC_HAL_GetValue (cfg_ptr->gpio_num));
            if (EIC_HAL_GetValue (cfg_ptr->gpio_num))
            {
                EIC_DBNC_RegCallback(cfg_ptr->gpio_num,SCI_FALSE,shaking_en,shaking_interval,gpio_callback_fun);
            }
            else
            {
                EIC_DBNC_RegCallback(cfg_ptr->gpio_num,SCI_TRUE,shaking_en,shaking_interval,gpio_callback_fun);
#ifdef BATTERY_DETECT_SUPPORT
                if (GPIO_PROD_BATTERY_DETECT_ID == id) {
                    // battery is off, power off right now
                    if (!POWER_IsCalibrationResetMode()) {
                        CHG_PHY_ShutDown();
                                             SCI_SLEEP(100);
                        INT_HAL_DisableINT();
                        SCI_DisableIRQ();
                         while (1)
                        {
                            ; // loop forever, don't send the response.
                        }
                    }
                }
#endif
            }
            break;
#endif
        default :
            // Do not use assert here, because the LCD is still not ready to
            // display assert information at the start of system init.
            //SCI_TRACE_LOW:"_GPIO_PROD_RegGpio : type %d - invalid"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1173_112_2_18_0_33_3_1527,(uint8*)"d", cfg_ptr->gpio_type);
    }

    return SCI_TRUE;

}

/*****************************************************************************/
//  Description:    This function is used to get gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PROD_InitCfgTable (void)
{
    uint32 i = 0;
    GPIO_PROD_ID_E      gpio_id        = GPIO_PROD_ID_MAX;
    GPIO_CFG_INFO_T_PTR cus_table_ptr  = GPIO_CFG_GetCusTable();
    GPIO_CFG_INFO_T_PTR full_table_ptr = s_gpio_prod_cfg_full_table;

    //SCI_TRACE_LOW:"_GPIO_PROD_InitCfgTable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1190_112_2_18_0_33_3_1528,(uint8*)"");

    for (i = 0; i < GPIO_PROD_ID_MAX; i++)
    {
        full_table_ptr[i].gpio_id  = GPIO_PROD_ID_MAX;
        full_table_ptr[i].gpio_num = GPIO_PROD_NUM_INVALID;
    }

    for (i = 0; i < GPIO_PROD_ID_MAX; i++)
    {
        gpio_id = cus_table_ptr[i].gpio_id;

        if (GPIO_PROD_ID_MAX != gpio_id)
        {
            // unused, can fill the cfg information to full table, or else, assert
            if (GPIO_PROD_ID_MAX == full_table_ptr[gpio_id].gpio_id)
            {
                SCI_MEMCPY (&full_table_ptr[gpio_id], &cus_table_ptr[i], sizeof (GPIO_CFG_INFO_T));
            }
            else
            {
                SCI_PASSERT (0, ("GPIO full table %d line has been used !!", gpio_id));   /*assert verified*/
            }

            if (i == (GPIO_PROD_ID_MAX - 1))
            {
                SCI_PASSERT (0, ("GPIO cus cfg table has not end flag !!"));   /*assert verified*/
            }

        }
        else
        {
            break;
        }

    }

    // About Extend GPIO
    GPIO_EXT_InitCfgTab();
}

/*****************************************************************************/
//  Description:    This function is used to get gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPIO_CFG_INFO_T_PTR GPIO_PROD_GetCfgInfo (GPIO_PROD_ID_E id)
{
    GPIO_CFG_INFO_T_PTR full_table_ptr = s_gpio_prod_cfg_full_table;

    SCI_ASSERT (id < GPIO_PROD_ID_MAX);/*assert verified*/

    if (full_table_ptr[id].gpio_id == id) /*lint !e661*/
    {
        // Check if there is the valid hardware GPIO
        if ( (GPIO_PROD_NUM_INVALID != full_table_ptr[id].gpio_num) /*lint !e661*/
                && (GPIO_PROD_TYPE_MAX  != full_table_ptr[id].gpio_type)) /*lint !e661*/
        {
            return (GPIO_CFG_INFO_T_PTR) &full_table_ptr[id];
        }
        // Check if there is callback function
        else if (PNULL != full_table_ptr[id].gpio_cb_fun) /*lint !e661*/
        {
            return (GPIO_CFG_INFO_T_PTR) &full_table_ptr[id];
        }
        else
        {
            return PNULL;
        }
    }
    else
    {
        //SCI_PASSERT(id != 22, ("GET: Gpio prod id err %d", id));
        //SCI_TRACE_LOW("GPIO_PROD_GetCfgInfo: Gpio prod id err %d", id);
    }

    return PNULL;
}

/*****************************************************************************/
//  Description:    This function is used to set gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PROD_SetCfgInfo (GPIO_CFG_INFO_T_PTR cfg_info_ptr)
{
    GPIO_CFG_INFO_T_PTR full_table_ptr = s_gpio_prod_cfg_full_table;

    SCI_ASSERT (PNULL != cfg_info_ptr);/*assert verified*/
    SCI_ASSERT (cfg_info_ptr->gpio_id < GPIO_PROD_ID_MAX);/*assert verified*/

    if (full_table_ptr[cfg_info_ptr->gpio_id].gpio_id == cfg_info_ptr->gpio_id)/*lint !e661 !e613*/
    {
        //SCI_TRACE_LOW:"GPIO_PROD_SetCfgInfo: Gpio prod id %d be Modified"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1279_112_2_18_0_33_3_1529,(uint8*)"d", cfg_info_ptr->gpio_id);
    }
    else
    {
        //SCI_TRACE_LOW:"GPIO_PROD_SetCfgInfo: Gpio prod id %d Create"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1283_112_2_18_0_33_3_1530,(uint8*)"d", cfg_info_ptr->gpio_id);
    }

    SCI_MEMCPY (&full_table_ptr[cfg_info_ptr->gpio_id], cfg_info_ptr, sizeof (GPIO_CFG_INFO_T));/*lint !e661 !e613*/

    return SCI_TRUE;
}


/*****************************************************************************/
//  Description:    This function turn on/off the KeyPad backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           KeyPad backlight is controlled by GPIO13 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void GPIO_SetKeyPadBackLight (BOOLEAN b_on)
{
    //SCI_TRACE_LOW:"GPIO_SetKeyPadBackLight: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1299_112_2_18_0_33_3_1531,(uint8*)"d", b_on);

    _GPIO_PROD_SetVal (GPIO_PROD_KEYPAD_BL_ID, b_on);

}

/*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void GPIO_SetLcdBackLight (BOOLEAN b_on)
{
#ifndef MODEM_PLATFORM
    //SCI_TRACE_LOW:"GPIO_SetLcdBackLight: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1313_112_2_18_0_33_3_1532,(uint8*)"d", b_on);

    if (b_on)
    {
        OS_TickDelay (20);
        LCD_SetBackLightBrightness (1);
        ANA_DecisionCodecTpFunc(ANA_DEV_ID_TP, TRUE);
        BOOT_SetBLStatus(1);  //set bk flag to be 1
         _GPIO_PROD_SetVal (GPIO_PROD_LCD_BL_EN_ID, b_on);
    }
    else
    {
    	 _GPIO_PROD_SetVal (GPIO_PROD_LCD_BL_EN_ID, b_on);
        LCD_SetBackLightBrightness (0);
	 ANA_DecisionCodecTpFunc(ANA_DEV_ID_TP, FALSE);
        BOOT_SetBLStatus(0);  //clear bk flag
    }
    if (MMIAPIPHONE_IsPoweringOff())
    {
        SCI_MMI_EnableDeepSleep(0);
        SCI_LCDBacklightBrightless_EnableDeepSleep (0);
    }
    else
    {
        SCI_MMI_EnableDeepSleep( (uint32) !b_on);
        SCI_LCDBacklightBrightless_EnableDeepSleep ( (uint32) !b_on);
    }
#endif
}

// add for fixing when keypad wakeup Lcd but not light Backlight
PUBLIC void GPIO_SetLcdBackLightSleepEnable (void)
{
#ifndef MODEM_PLATFORM
    //SCI_LCDBacklightBrightless_EnableDeepSleep (1);
    SCI_MMI_EnableDeepSleep(1);
#endif
}

/*****************************************************************************/
//  Description:    This function control lcd backlight
//                  is_high = SCI_TRUE, gpio in high level
//                  is_high = SCI_FALSE, gpio in low level
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetLcdBackLightness (BOOLEAN is_high)
{
    _GPIO_PROD_SetVal (GPIO_PROD_LCD_BL_BRIGHTNESS_ID, is_high);
}
/*****************************************************************************/
//  Description:    This function set BackLight.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetBackLight (BOOLEAN b_light)
{
    //SCI_TRACE_LOW:"GPIO_SetBackLight: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1350_112_2_18_0_33_3_1533,(uint8*)"d", b_light);
    GPIO_SetLcdBackLight (b_light);
    GPIO_SetKeyPadBackLight (b_light);
}

LOCAL BOOLEAN s_torch_is_on = FALSE;

PUBLIC void GPIO_SetTorch (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_FLASH_EN_ID,  b_on);
    s_torch_is_on = b_on;
}

PUBLIC BOOLEAN GPIO_GetTorchStatus( void )
{
    return s_torch_is_on;
}

PUBLIC void GPIO_SetTorchStatus( BOOLEAN b_on )
{
    s_torch_is_on = b_on;
}

/*****************************************************************************/
//  Description:    This function set the state of vibrator.
//
//  Input      :
//      b_on   : If true, start vibrate. If false, stop it.
//  Return     : None
//  Author     : Lin.liu
//  Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetVibrator (BOOLEAN b_on)
{
    //SCI_TRACE_LOW:"GPIO_SetVibrator: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1364_112_2_18_0_33_3_1534,(uint8*)"d", b_on);

    _GPIO_PROD_SetVal (GPIO_PROD_VIBRATIOR_EN_ID,  b_on);
}

LOCAL SCI_TIMER_PTR  S_SetVibirator_Timer = PNULL;
LOCAL void GPIO_Vibrator_Timer_Callback(uint32 para)
{


    if(S_SetVibirator_Timer)
    {
       SCI_DeactiveTimer(S_SetVibirator_Timer);
    }

    ANA_SetDevValule(ANA_DEV_ID_VIBRATOR, ANA_DEV_VAL_MIN);
}
/*****************************************************************************/
//  Description:    This function set the state of vibrator.
//
//  Input      :
//      b_on   : If true, start vibrate. If false, stop it.
//      on_period   : unit: ms, only for TURN ON, on_period == 0, it means VIB always ON without timer
//                    should be Turn off later
//      on_stress   : reserved for future use
//      reserved    : reserved for future use
//  Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetVibrator_Ext(BOOLEAN b_on, uint32 on_period, uint32 on_stress, uint32 *reserved)
{
    //SCI_TRACE_LOW:"GPIO_SetVibrator_Ext: on = %d, period = %d, stress = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1390_112_2_18_0_33_3_1535,(uint8*)"ddd", b_on, on_period, on_stress);
   // set Timer at b_on and on_period !=0, if on_period == 0, it means Vib=1 always without timer
   if(b_on && on_period)
   {
        if(PNULL == S_SetVibirator_Timer)
        {
            S_SetVibirator_Timer = SCI_CreateTimer("SetVibrator_Timer",
            GPIO_Vibrator_Timer_Callback, NULL, on_period, SCI_AUTO_ACTIVATE);
        }
        else
        {
            SCI_ChangeTimer(S_SetVibirator_Timer, GPIO_Vibrator_Timer_Callback, on_period);
            SCI_ActiveTimer(S_SetVibirator_Timer);
        }
    }
    else
    {
        if(S_SetVibirator_Timer)
        {
            SCI_DeactiveTimer(S_SetVibirator_Timer);
        }
    }

    // set VIBRATOR
    GPIO_SetVibrator(b_on);

    return;
}
/*****************************************************************************/
//  Description:    This function query the state of Flip
//                  return value = SCI_TRUE, flip is open
//                  return value = SCI_FALSE, flip is close
//  Author:         Zhemin.Lin
//  Note:
//                  Lin.liu.   don't use this file. (only for compatible with prev version)
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetFlipState (void)
{
    BOOLEAN  status = SCI_TRUE;

    _GPIO_PROD_GetVal (GPIO_PROD_FLIP_ON_ID, &status);

    //SCI_TRACE_LOW:"GPIO_GetFlipState: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1435_112_2_18_0_33_3_1536,(uint8*)"d", status);

    return status;
}

/*****************************************************************************/
//  Description:    This function check the headset detect pin status.
//
//  Input      : None
//  Return     :
//      True   : the headset is in.
//      False  : the headset is out.
//  Author     : Lin.liu
//  Note       : When call this function, make sure the gpio has been config correctly.
/*****************************************************************************/
#if 0
PUBLIC BOOLEAN GPIO_CheckHeadsetStatus (void)
{
    BOOLEAN  status = 0;

    _GPIO_PROD_GetVal (GPIO_PROD_HEADSET_DETECT_ID, &status);

    //SCI_TRACE_LOW:"GPIO_CheckHeadsetStatus: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1455_112_2_18_0_33_3_1537,(uint8*)"d", status);
#ifdef FPGA_VERIFICATION
    // HACK: No GPIO pin for headset insert detection at FPGA verification stage
    return SCI_TRUE;
#else
    return (status);
#endif
}
#else
PUBLIC BOOLEAN GPIO_CheckHeadsetStatus (void)
{
    return SCI_FALSE;

}

#endif

/*****************************************************************************/
//  Description:    This function check the SDCard detect pin status.
//
//  Input      : None
//  Return     :
//      True   : the SDCard is in.
//      False  : the SDCard is out.
//  Author     : yuehz
//  Note       : 20060410;when call this function, make sure the gpio has been config correctly.
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_CheckSDCardStatus (void)
{
    BOOLEAN  status = SCI_TRUE;

    _GPIO_PROD_GetVal (GPIO_PROD_SDCARD_DETECT_ID, &status);
    //SCI_TRACE_LOW:"GPIO_CheckSDCardStatus: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1473_112_2_18_0_33_3_1538,(uint8*)"d", status);

    return status;
}

/*****************************************************************************/
//  Description:    This function set enable/disable amplifier.
//                  b_enable = SCI_TRUE   Enable amplifier
//                  b_enable = SCI_FALSE  Disable amplifier
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableAmplifier (BOOLEAN b_enable)
{
#ifndef PLATFORM_UWS6121E
    if (_GPIO_Check_Amplifier_Share())
    {
        _GPIO_Control_Amplifier_Shared (SCI_FALSE,b_enable);
    }
    else
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPEAKER_PA_EN_ID, b_enable);
    }
#else
    _GPIO_PROD_SetVal (GPIO_PROD_SPEAKER_PA_EN_ID, b_enable);
#endif
}

/*****************************************************************************/
//  Description:    This function set enable/disable AudioCodec.
//                  b_enable = SCI_TRUE   Enable AudioCodec
//                  b_enable = SCI_FALSE  Disable AudioCodec
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableCodec (BOOLEAN b_enable)
{
    //_GPIO_PROD_SetVal (GPIO_PROD_CODEC_EN_ID, b_enable);
    //GPIO_SetValue (11, b_enable);
    GPIO_SetValue (11, 1);
    //GPIO_SetValue (35, 0); /// TP releated
    SCI_TRACE_LOW("%s %d",__FUNCTION__,b_enable);
}

/*****************************************************************************/
//  Description:    This function set enable/disable Micbias.
//                  b_enable = SCI_TRUE   Enable Micbias
//                  b_enable = SCI_FALSE  Disable Micbias
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableMicBias (BOOLEAN b_enable)
{
#ifndef AUDIO_CODEC_TYPE_ES8374
    GPIO_SetValue (9, b_enable);
    SCI_TRACE_LOW("%s %d",__FUNCTION__,b_enable);
#endif
}


/*****************************************************************************/
//  Description:    This function set enable/disable amplifier.
//                  b_enable = SCI_TRUE   Enable amplifier
//                  b_enable = SCI_FALSE  Disable amplifier
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableHeadsetAmplifier (BOOLEAN b_enable)
{
#ifndef PLATFORM_UWS6121E
    if (_GPIO_Check_Amplifier_Share())
    {
        _GPIO_Control_Amplifier_Shared (SCI_TRUE,b_enable);
    }
    else
    {
        _GPIO_PROD_SetVal (GPIO_PROD_HEADSET_PA_EN_ID, b_enable);
    }
#endif
}

/*****************************************************************************/
//  Description:    This function turn on/off the Flash .
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetFlash (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_FLIP_ON_ID, b_on);
}

/*****************************************************************************/
//  Description:    This function set VCM enable.
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:        Hansen.sun
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetVCMEnable (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_AF_VCM_CTL_ID, b_on);

}

/*****************************************************************************/
//  Description:    This function turn on/off the Flash .
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetDCFlash (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_DC_FLASH_ID, b_on);
}

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
)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_ID, (BOOLEAN) !pulse_level);
    SCI_Sleep (10);
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_ID, pulse_level);
    SCI_Sleep (pulse_width);
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_ID, (BOOLEAN) !pulse_level);
    SCI_Sleep (10);

}
/*****************************************************************************/
//  Description:    This function is used to control the sensor power if the
//                  the sensor is enabled by a gpio.
//                  b_on = SCI_TRUE, open sensor power
//                  b_on = SCI_FALSE, close sensor power
//  Author:         junyi.lv
//  Note:
/*****************************************************************************/

PUBLIC void GPIO_SetSensorPower (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_POWER_ID, b_on);
}

/*****************************************************************************/
//  Description:    This function set sensor pwdn .
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSensorPwdn (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_PWDN_ID, b_on);
}

/*****************************************************************************/
//  Description:    This function set front sensor pwdn .
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetFrontSensorPwdn (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_PWDN_FRONT_ID, b_on);

}

/*****************************************************************************/
//  Description:    This function do sensor reset.
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetMainSensorReset (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_ID, b_on);
}

PUBLIC void GPIO_SetFrontSensorReset (BOOLEAN b_on)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_FRONT_ID, b_on);
}

/*****************************************************************************/
//  Description:    This function set sensor reset signal .
//                  pulse_level = SCI_TRUE, High
//                  pulse_level = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSensorResetLevel (BOOLEAN pulse_level)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SENSOR_RESET_ID, pulse_level);
}
/*****************************************************************************/
//  Description:    This function set the state of flash light.
//
//  Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetFlashLight(BOOLEAN b_on)
{
// GPIO_PROD_FLASH_EN_ID
    SCI_TRACE_LOW("GPIO_SetFlashLight: on = %d", b_on);
    _GPIO_PROD_SetVal (GPIO_PROD_FLASH_EN_ID, b_on);

    return;
}
/*****************************************************************************/
//  Description:    This function set analog tv reset signal .
//                  pulse_level = SCI_TRUE, High
//                  pulse_level = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetAnalogTVResetLevel (BOOLEAN pulse_level)
{
    _GPIO_PROD_SetVal (GPIO_PROD_ATV_RESET_ID, pulse_level);
}

/*****************************************************************************/
//  Description:    This function get charge interrupt gpio number
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_GetChargeIntGpio (void)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_CHARGE_PLUG_DETECT_ID);

    if (PNULL != cfg_ptr)
    {
        return cfg_ptr->gpio_num;
    }
    else
    {
        return 0xffffffff;
    }
}

/*****************************************************************************/
//  Description:    This function enable SPI CS select SD
//
//  Author:        yuehz
//  Note:           20060512 b_level :0 enable;1:disable
/*****************************************************************************/
PUBLIC void GPIO_SetSDCS (BOOLEAN b_enable)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SDCARD_CS_ID,b_enable);
}

/*****************************************************************************/
//  Description:    This function selects headset mic or tv out in DVB
//                       gpio_state=Low:headset mic;High: TV Out
//  Author:        yuehz
//  Note:           20060628
/*****************************************************************************/
PUBLIC void GPIO_Sel_MIC_TVOut (BOOLEAN is_to_tv)
{
    _GPIO_PROD_SetVal (GPIO_PROD_TV_MIC_SW_ID, is_to_tv);
}

/*****************************************************************************/
//  Description:    This function Open PA or not
//
//  Author:        yuehz
//  Note:           20060627
/*****************************************************************************/
PUBLIC void GPIO_OpenFMPA (BOOLEAN is_open)
{
    ;
}

/*****************************************************************************/
//  Description:    This function set GPIO which controls the motor
//
//  Author:        Jianping.wang
//  Note:
/*****************************************************************************/
void GPIO_MOTOR_SetPortValue (
    uint16 ic_num,      // define in gpio_dvb_ext.h
    uint16 gpio_mask,   // when the related bit is '1', use value of related bit in gpio_value to set gpio output value
    uint16 gpio_value   // represent 16 gpio value
)
{
    //GPIO_EXT_SetPortValue(ic_num, gpio_mask, gpio_value) ;
    ;
}


/*****************************************************************************/
//  Description:    This function control SD CARD Power ;
//                  is_open = SCI_TRUE,     Low
//                  is_open = SCI_FALSE,    High
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_OpenSDPower (BOOLEAN is_open)
{
    _GPIO_PROD_SetVal (GPIO_PROD_SDCARD_PWR_ID, is_open);
}

/*****************************************************************************/
//  Description:    This function detect usb or adapter ,
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio input in pinmap_**.c.
//  Author:         junyi.lv
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_DetectUsbOrAdapter (void)
{
    BOOLEAN  status = SCI_TRUE;

    _GPIO_PROD_GetVal (GPIO_PROD_USB_DETECT_ID, &status);

    return status;
}

/*****************************************************************************/
//  Description:    This function select spi2 extend cs

//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SelectSPI2EXTCS (uint32 cs_num)
{
    //SCI_TRACE_LOW:"GPIO_SelectSPI2CS: cs_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1718_112_2_18_0_33_4_1539,(uint8*)"d", cs_num);

    SCI_ASSERT (cs_num < 8);/*assert verified*/

    if (cs_num&0x1)
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL0_ID,SCI_TRUE);
    }
    else
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL0_ID,SCI_FALSE);
    }

    if (cs_num&0x2)
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL1_ID,SCI_TRUE);
    }
    else
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL1_ID,SCI_FALSE);
    }

    if (cs_num&0x4)
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL2_ID,SCI_TRUE);
    }
    else
    {
        _GPIO_PROD_SetVal (GPIO_PROD_SPI2_EXT_CS_SEL2_ID,SCI_FALSE);
    }
}

/*****************************************************************************/
//  Description:    This function set RF transceiver reset GPIO output high,
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:         Younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_ConfigRfReset (void)
{
    // OUTPUT
    _GPIO_PROD_SetDirection (GPIO_PROD_RF_RESET_ID, SCI_TRUE);

    if (_GPIO_PROD_SetVal (GPIO_PROD_RF_RESET_ID, SCI_FALSE))
    {
        OS_TickDelay (1);
    }
    else
    {
        // Because  the RF-reset GPIO is un-register, so don't need to continue
        return;
    }

    _GPIO_PROD_SetVal (GPIO_PROD_RF_RESET_ID, SCI_TRUE);

    // INPUT
    _GPIO_PROD_SetDirection (GPIO_PROD_RF_RESET_ID, SCI_FALSE);
}

/*****************************************************************************/
//  Description:    This function set BT set pin status
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetBtReset (BOOLEAN is_high)
{
    _GPIO_PROD_SetVal (GPIO_PROD_BT_RESET_ID, is_high);
}

/******************************************************************************/
// Description:   PULL UP/DOWN RESET PIN TO FORCE WIFI TO ENTER POWERDOWN MODE
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC void GPIO_SetWifiPowerDown (BOOLEAN is_powerdown)
{
    _GPIO_PROD_SetVal(GPIO_PROD_WIFI_PWD_ID, is_powerdown);
}

/*****************************************************************************/
//  Description:    wifi interrupt handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_WiFiIntHander (uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_WIFI_INT_ID);

    SCI_TRACE_LOW("GPIO_WiFiIntHander:gpio_id=%d, gpio_state=%d", gpio_id, gpio_state);

    if (PNULL != cfg_ptr)
    {
        if (gpio_state)
        {
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_WIFI_INT_ID, GPIO_INT_LEVEL_LOW);
        }
        else
        {
            _GPIO_PROD_SetInterruptSense(GPIO_PROD_WIFI_INT_ID, GPIO_INT_LEVEL_HIGH);
        }

        if (cfg_ptr->gpio_cb_fun && (cfg_ptr->valid_level == gpio_state))
        {
            cfg_ptr->gpio_cb_fun(0);
        }

    }
}
/*****************************************************************************/
//  Description:    This function dot lcd reset
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:
//  Note:           pulse_level: pin level,
//                  delay_ms:  reset pulse time
/*****************************************************************************/
PUBLIC void GPIO_ResetLcd (BOOLEAN pulse_level, uint32  delay_ms)
{

    _GPIO_PROD_SetVal (GPIO_PROD_LCD_RST_ID, (BOOLEAN) !pulse_level);
    OS_TickDelay (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_LCD_RST_ID, pulse_level);
    OS_TickDelay (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_LCD_RST_ID, (BOOLEAN) !pulse_level);
    OS_TickDelay (delay_ms);

}

PUBLIC void GPIO_SetFmLNA(BOOLEAN is_high)
{
#if defined(PRODUCT_CONFIG_uis8910c_demophone ) || defined(PRODUCT_CONFIG_uis8910c_mpphone) || defined(PRODUCT_CONFIG_uis8910ff_mpphone)  \
	|| defined(PRODUCT_CONFIG_uis8910ff_zxf)
	_GPIO_PROD_SetVal (GPIO_PROD_FMLNA_ID, is_high);
#endif
}


#if 1 //TOUCHPANEL_TYPE_MULTITP
PUBLIC void GPIO_ResetTPInt(BOOLEAN is_on,uint32 delay_ms)
{
    uint16 gpio_num = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        gpio_num =  cfg_ptr->gpio_num;
    // GPIO Output
        GPIO_SetDataMask (gpio_num,SCI_TRUE);
        GPIO_SetDirection (gpio_num,SCI_TRUE);
        GPIO_DisableIntCtl (gpio_num);
        
        GPIO_SetValue (gpio_num,is_on);
        OS_TickDelay (delay_ms);
        GPIO_SetValue (gpio_num,!is_on);
        OS_TickDelay (delay_ms);
        GPIO_SetValue (gpio_num,is_on);
        OS_TickDelay (delay_ms);
        
    // GPIO Input Int
        GPIO_SetDataMask (gpio_num,SCI_TRUE);
        GPIO_SetDirection (gpio_num,SCI_FALSE);
    }
    return;
}

PUBLIC void GPIO_ResetTPIntExt(BOOLEAN is_on,uint32 delay_ms)
{
    uint16 gpio_num = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        gpio_num =  cfg_ptr->gpio_num;
    // GPIO Output
        GPIO_SetDataMask (gpio_num,SCI_TRUE);
        GPIO_SetDirection (gpio_num,SCI_TRUE);
        GPIO_DisableIntCtl (gpio_num);
        
        GPIO_SetValue (gpio_num,is_on);
        OS_TickDelay (delay_ms);
        GPIO_SetValue (gpio_num,!is_on);
        OS_TickDelay (delay_ms);
        GPIO_SetValue (gpio_num,is_on);
        OS_TickDelay (delay_ms);
    }
    return;
}

PUBLIC void GPIO_SetTPIntDir(BOOLEAN is_out,uint8 val)
{
    uint16 gpio_num = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        gpio_num =  cfg_ptr->gpio_num;
    // GPIO Output
        if(is_out)
        {
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            GPIO_SetDirection (gpio_num,SCI_TRUE);
            GPIO_DisableIntCtl (gpio_num);
            if(val)
            {
                GPIO_SetValue (gpio_num,1);
            }
            else
            {
                GPIO_SetValue (gpio_num,0);
            }
        }
        else
        {
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            GPIO_SetDirection (gpio_num,SCI_FALSE);
        }
    }
    return;
}

/*****************************************************************************/
//  Name:           GPIO_ResetTP
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_ResetTP(BOOLEAN is_on,uint32 delay_ms)
{
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  is_on);
    OS_TickDelay (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  !is_on);
    OS_TickDelay (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  is_on);
    OS_TickDelay (200);
}

/*****************************************************************************/
//  Name:           GPIO_ResetTP
//  Description:    This function set the rest pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_ResetTP_Sleep(BOOLEAN is_on,uint32 delay_ms)
{
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  is_on);
    SCI_Sleep (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  !is_on);
    SCI_Sleep (delay_ms);
    _GPIO_PROD_SetVal (GPIO_PROD_TP_WAKE_ID,  is_on);
    SCI_Sleep (delay_ms);
}

/*****************************************************************************/
//  Name:           GPIO_TPIRQCtrl
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_TPIRQCtrl(BOOLEAN is_active)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        if(is_active)
        {
            GPIO_EnableIntCtl(cfg_ptr->gpio_num);
        }
        else
        {
            GPIO_DisableIntCtl(cfg_ptr->gpio_num);
        }
    }

}
/*****************************************************************************/
//  Name:           GPIO_TPInterruptSense
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_TPInterruptSense(uint32 status)
{
    if(status)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_TP_INT_ID, GPIO_INT_LEVEL_HIGH);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_TP_INT_ID, GPIO_INT_LEVEL_LOW);
    }
}

/*****************************************************************************/
//  Name:           GPIO_TPInterruptPin
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC uint16 GPIO_TPInterruptPin(void)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        return cfg_ptr->gpio_num;
    }
    return 0;
}

/*****************************************************************************/
//  Name:           GPIO_TPGetInterruptStatus
//  Description:    This function get valid interrupt level.
/*****************************************************************************/
PUBLIC uint16 GPIO_TPGetInterruptStatus(void)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_TP_INT_ID );
    if(PNULL != cfg_ptr )
    {
        return cfg_ptr->valid_level;
    }
    return 0;
}

#endif


PUBLIC void GPIO_SPIIRQCtrl(BOOLEAN is_active)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo( GPIO_PROD_WIFI_INT_ID );

    if(PNULL != cfg_ptr )
    {
         if(is_active)
        {
            GPIO_EnableIntCtl(cfg_ptr->gpio_num);
        }
        else
        {
            GPIO_DisableIntCtl(cfg_ptr->gpio_num);
        }
    }
}

/*****************************************************************************/
//  Description:    This function handle demod power
//  Author:
//  Note:           pulse_level
/*****************************************************************************/
PUBLIC void GPIO_DemodPower( BOOLEAN is_poweron )
{
#ifndef CMMB_WIFI_SHARE_SPI_SUPPORT
    _GPIO_PROD_SetVal(GPIO_PROD_DEMOD_POWER, is_poweron);
#endif
}
PUBLIC void CMMB_gpio_power_on(void)
{
#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
    _GPIO_PROD_SetVal(GPIO_PROD_DEMOD_POWER, 1);
#endif
}
PUBLIC void CMMB_gpio_power_off(void)
{
#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
    _GPIO_PROD_SetVal(GPIO_PROD_DEMOD_POWER, 0);
#endif
}

/*****************************************************************************/
//  Description:    This function handle demod reset
//  Author:
//  Note:           pulse_level
/*****************************************************************************/
PUBLIC void GPIO_DemodReset( BOOLEAN pulse_level )
{
    _GPIO_PROD_SetVal(GPIO_PROD_DEMOD_RESET, pulse_level);
}

/*****************************************************************************/
//  Description:    demod int handler
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DemodIntHandler( uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_DEMOD_INT);

    if (PNULL != cfg_ptr)
    {
#if defined(DEMOD_HW_SIANO) || defined(DEMOD_HW_INNOFIDEI)
           DemodSpiIsr(PNULL);
#endif
    }
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//  Description:    SIM card hot swap feature.
//  Input      :
//  Return     :    None
//  Author     :    wuding.yang
//    Note       :
/*****************************************************************************/
PUBLIC void GPIO_SIMIntHandler(uint32 gpio_id, uint32 gpio_state)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_PROD_SIM_PLUG_IN_ID);
    BOOLEAN is_on = 0;
    //SCI_TRACE_LOW:"GPIO_SIMIntHandler %d %d. %d %d %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_1997_112_2_18_0_33_5_1541,(uint8*)"dddddd",gpio_id,gpio_state,cfg_ptr->gpio_id,cfg_ptr->gpio_type,cfg_ptr->valid_level,cfg_ptr->gpio_num);

    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_SIM_PLUG_IN_ID, GPIO_INT_LEVEL_LOW);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_SIM_PLUG_IN_ID, GPIO_INT_LEVEL_HIGH);
    }
    // plug in --low_level/ plug out -- high_level
    if(gpio_state ==0)
    {
        is_on = 1;
    }
    else
    {
        is_on = 0;
    }

    if (PNULL != cfg_ptr)
    {
        //SCI_TRACE_LOW("GPIO_SIMIntHandler %d %d. %d %d %d %d",gpio_id,gpio_state,cfg_ptr->gpio_id,cfg_ptr->gpio_type,cfg_ptr->valid_level,cfg_ptr->gpio_num);
        if (NULL != cfg_ptr->gpio_cb_fun)
        {
            cfg_ptr->gpio_cb_fun((BOOLEAN)is_on);
        }
    }
}
#endif

/*****************************************************************************/
//  Description:    Get the GPIO Id which is connected with the hot swap sim slot.
//  Input      :
//  Return     :    None
//  Author     :    wuding.yang
//    Note       :
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_SIM_Hotswap_GetGPIOId(uint32 *gpio_id)
{
#ifdef SIM_PLUG_IN_SUPPORT
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_PROD_SIM_PLUG_IN_ID);
    if(cfg_ptr)
    {
        *gpio_id = cfg_ptr->gpio_num;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;//do not support sim hot swap
#endif
}

#ifdef GPIO_SIMULATE_SPI_SUPPORT
/*****************************************************************************/
//  Description:    Simulate a SPI by 3 GPIO, 2 SPI by 6 GPIO.
//  Author:        yun.wang
//  Note:           2010-11-23
/*****************************************************************************/
PUBLIC uint32 GPIO_SPI_id_get(GPIO_SPI_PIN_ID_E gpio_spi_pin)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = NULL ;
    switch(gpio_spi_pin)
    {
        case VIR_SPI0_CLK_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI0_CLK_ID);
            break;
        }
        case VIR_SPI0_DIN_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI0_DIN_ID);
            break;
        }
        case VIR_SPI0_DOUT_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI0_DOUT_ID);
            break;
        }
        case VIR_SPI0_CS_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI0_CS_ID);
            break;
        }
        case VIR_SPI1_CLK_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI1_CLK_ID);
            break;
        }
        case VIR_SPI1_DIN_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI1_DIN_ID);
            break;
        }
        case VIR_SPI1_DOUT_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI1_DOUT_ID);
            break;
        }
        case VIR_SPI1_CS_PIN_ID:
        {
            cfg_ptr= GPIO_PROD_GetCfgInfo(GPIO_PROD_VIR_SPI1_CS_ID);
            break;
        }
        default :
        //SCI_PASSERT(0, ("GPIO_SPI_id_get : gpio_spi_pin:%d", gpio_spi_pin));
        return GPIO_PROD_NUM_INVALID;
    }
    if(NULL==cfg_ptr)
    {
        //SCI_PASSERT(0, ("GPIO_SPI_id_get : gpio_spi_pin:%d", gpio_spi_pin));
        return GPIO_PROD_NUM_INVALID;
    }
    else
    {
        return cfg_ptr->gpio_num;
    }
}

#endif

#ifdef MODEM_PLATFORM
PUBLIC  void  GPIO_APWakeUpBBHandler(uint32 gpio_id, uint32 status)
{
    SCI_TRACE_LOW("GPIO_APWakeUpBBHandler: AP wake up modem, gpio=%d, status=0x%X", gpio_id, status);

    if(status)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_AP_WAKEUP_BB_ID, GPIO_INT_LEVEL_LOW);
        GPIO_WakeUpAP(SCI_FALSE);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_AP_WAKEUP_BB_ID, GPIO_INT_LEVEL_HIGH);
        GPIO_WakeUpAP(SCI_TRUE);
    }

    SCI_DisableIRQ();
    TM_SendTestPointRequest(1234, (uint32)(!GPIO_GetAPSleepStatus()));
    if(GPIO_GetAPSleepStatus())
    {/*lint -esym(718, SCI_MODEM_EnableDeepSleep) */
    /*lint -esym(746, SCI_MODEM_EnableDeepSleep) */
        SCI_TRACE_LOW("GPIO_APWakeUpBBHandler: Disable system sleep");
        SCI_MODEM_EnableDeepSleep(DISABLE_DEEP_SLEEP);
    }
    else
    {
        SCI_TRACE_LOW("GPIO_APWakeUpBBHandler: Disable system sleep");
        SCI_MODEM_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    }

    SCI_RestoreIRQ();
}

PUBLIC void GPIO_WakeUpAP(BOOLEAN is_wake_up)
{
    _GPIO_PROD_SetVal(GPIO_PROD_BB_WAKEUP_AP_ID, is_wake_up);
}

/**************************************************
*ret SCI_TRUE,   High, AP is not sleep
*    SCI_FALSE,  Low,  AP is sleep
*
**************************************************/
PUBLIC BOOLEAN GPIO_GetAPSleepStatus(void)
{
    BOOLEAN status = SCI_FALSE;
    _GPIO_PROD_GetVal(GPIO_PROD_AP_WAKEUP_BB_ID, &status);
    return status;
}


/*****************************************************************************/
//  Description:    This function is to check if allowed to report RSSI to AP ;
//                  return value = SCI_TRUE,     is allowed
//                  return value = SCI_FALSE,     is not allowed
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GPIO_IsAllowedReportRssiToAP(void)
{
    BOOLEAN  status = SCI_FALSE;

#ifdef MODEM_TYPE_V0
    _GPIO_PROD_GetVal(GPIO_PROD_AP_RSSI_RPT_ID, &status);
#endif

    SCI_TRACE_LOW("GPIO_IsAllowedReportRssiToAP: Status=0x%X", status);

    return  status;
}
#endif

#ifdef DPHONE_SUPPORT    // zhiguo.li_cr225830
//add by brezen
PUBLIC void GPIO_HookIntHandler(uint32 gpio_id, uint32 gpio_state)
{
    TB_MSG  gpio_msg;


    //SCI_TRACE_LOW:"GPIO_HookKeyIntHandler gpio_state(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2113_112_2_18_0_33_5_1542,(uint8*)"d",gpio_state);
    if (gpio_state)
    {
        gpio_msg.message = TB_KPD_PRESSED;
        gpio_msg.wparam  = SCI_VK_HOOK;
        gpio_msg.lparam  = TB_NULL;
        HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_LOW);
    }
    else
    {
        gpio_msg.message = TB_KPD_RELEASED;
        gpio_msg.wparam  = SCI_VK_HOOK;
        gpio_msg.lparam  = TB_NULL;
        HAL_SetGPIOInterruptSense(gpio_id, GPIO_INT_LEVEL_HIGH);

    }
    DRV_Callback(TB_GPIO_INT,&gpio_msg);

}

PUBLIC BOOLEAN GPIO_GetHookState(void)
{
    return (!GPIO_GetGPIOState(37));
}

PUBLIC void GPIO_EnableHookInt(BOOLEAN b_enable)
{
    if (b_enable)
        GPIO_EnableIntCtl(37);
    else
        GPIO_DisableIntCtl(37);

    //SCI_TRACE_LOW:"GPIO_EnableHookInt.........%d...."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2145_112_2_18_0_33_5_1543,(uint8*)"d", b_enable);
}

PUBLIC void GPIO_EnableHandFreeILed(BOOLEAN b_enable)
{
    if (b_enable)
       _GPIO_PROD_SetVal (GPIO_PROD_HANDFREE_ID, 0);
    else
       _GPIO_PROD_SetVal (GPIO_PROD_HANDFREE_ID, 1);

    //SCI_TRACE_LOW:"GPIO_EnableHandFreeILed.........%d...."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2155_112_2_18_0_33_5_1544,(uint8*)"d", b_enable);
}
#endif
PUBLIC void GPIO_USBResumeHandler (uint32 gpio_id, uint32 gpio_state)
{
      GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_USB_RESUME_ID);


      if (PNULL != cfg_ptr)
      {
            if (NULL != cfg_ptr->gpio_cb_fun)
            {
                  cfg_ptr->gpio_cb_fun((BOOLEAN)gpio_state);
            }
      }
    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_USB_RESUME_ID, GPIO_INT_LEVEL_LOW); //plug out det
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_USB_RESUME_ID, GPIO_INT_LEVEL_HIGH); //plug in det
    }
}
#ifdef MOTION_SENSOR_TYPE_OFN
/*****************************************************************************/
//  Description:    This function is used to handler ofn-sensor interrupt
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_OFNIntHandler (uint32 gpio_id, uint32 gpio_state)
{
    //Because Reading registers via IIC is very slow, so it CANNOT be done
    //in interrupt context.
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_OFN_INT_ID);

    if (gpio_state)
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_OFN_INT_ID, GPIO_INT_LEVEL_LOW);
    }
    else
    {
        _GPIO_PROD_SetInterruptSense (GPIO_PROD_OFN_INT_ID, GPIO_INT_LEVEL_HIGH);
    }

    if (cfg_ptr && (cfg_ptr->valid_level == gpio_state) && (cfg_ptr->gpio_cb_fun))
    {
        cfg_ptr->gpio_cb_fun (GPIO_PROD_OFN_INT_ID);
    }

}
#endif
#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
/*****************************************************************************/
//  Description:    Switch SPI Bus by Logic Id
//  Input      :
//  Return     :
//  Author     :    Chi.Chen
//    Note       :
/*****************************************************************************/
PUBLIC void GPIO_SPISwitch(uint32 * logic_id)
{
    uint32 wifi_spi_bus_logic_id, cmmb_spi_bus_logic_id;

    wifi_spi_bus_logic_id = WIFI_GetSPIBusLogicID();
    cmmb_spi_bus_logic_id = CMMB_GetSPIBusLogicID();
    SCI_ASSERT( (wifi_spi_bus_logic_id >= 0 && wifi_spi_bus_logic_id < 2) &&/*assert verified*/
                (cmmb_spi_bus_logic_id >= 0 && cmmb_spi_bus_logic_id < 2) );
    if (wifi_spi_bus_logic_id == *logic_id)
    {
        GPIO_SetSPISwitch(GPIO_SPI_SW_WIFI);
    }
    else if (cmmb_spi_bus_logic_id == *logic_id)
    {
        GPIO_SetSPISwitch(GPIO_SPI_SW_CMMB);
    }
    //SCI_TRACE_LOW("GPIO_SPISwitch: spi bus select cs%d", *logic_id);
}

/*****************************************************************************/
//  Description:    Set GPIO to Switch SPI Bus to WiFi or CMMB
//  Input      :
//  Return     :
//  Author     :    Chi.Chen
//    Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetSPISwitch(GPIO_SPI_SW_E sw_type)
{
    BOOLEAN gpio_switch_default_value, gpio_switch_wifi_value, gpio_switch_cmmb_value;
    static GPIO_SPI_SW_E sw_state = GPIO_SPI_SW_MAX;

    if (sw_state == sw_type)
    {
        return;
    }
    switch (sw_type)
    {
    case GPIO_SPI_SW_WIFI:
        sw_state = GPIO_SPI_SW_WIFI;
        gpio_switch_wifi_value = GPIO_GetSwitchWiFiValue();
        _GPIO_PROD_SetVal (GPIO_PROD_SPISWICTH_ID, gpio_switch_wifi_value);
        //SCI_TRACE_LOW:"GPIO_SetSPISwitch: WIFI ON"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2250_112_2_18_0_33_5_1545,(uint8*)"");
        break;

    case GPIO_SPI_SW_CMMB:
        sw_state = GPIO_SPI_SW_CMMB;
        gpio_switch_cmmb_value = GPIO_GetSwitchCMMBValue();
        _GPIO_PROD_SetVal (GPIO_PROD_SPISWICTH_ID, gpio_switch_cmmb_value);
        //SCI_TRACE_LOW:"GPIO_SetSPISwitch: CMMB ON"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2257_112_2_18_0_33_5_1546,(uint8*)"");
        break;

    case GPIO_SPI_SW_MAX:
        sw_state = GPIO_SPI_SW_MAX;
        gpio_switch_default_value = GPIO_GetSwitchDefaultValue();
        _GPIO_PROD_SetVal (GPIO_PROD_SPISWICTH_ID, gpio_switch_default_value);
        //SCI_TRACE_LOW:"GPIO_SetSPISwitch: SWITCH DEFAULT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2264_112_2_18_0_33_5_1547,(uint8*)"");
        break;

    default:
        sw_state = GPIO_SPI_SW_MAX;
        gpio_switch_default_value = GPIO_GetSwitchDefaultValue();
        _GPIO_PROD_SetVal (GPIO_PROD_SPISWICTH_ID, gpio_switch_default_value);
        //SCI_TRACE_LOW:"GPIO_SetSPISwitch: PARAMETERS ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PROD_2271_112_2_18_0_33_5_1548,(uint8*)"");
        break;
    }
}
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

// End of tb_comm.c
