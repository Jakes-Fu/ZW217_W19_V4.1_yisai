/******************************************************************************
 ** File Name:      power.c                                                   *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           06/03/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the function to handle power off.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2002     Xueliang.Wang    Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                             *
 ** 21/31/2003     Zhemin.Lin       Modify CR6659                             *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "os_api.h"
#include "os_apiext.h"
#include "sci_log.h"
#include "power.h"
#include "tasks_id.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "cmddef.h"
#include "ref_import.h"
#include "nvitem.h"

#include "ps_outport.h"
#include "layer1_engineering.h"

#include "chip.h"
#include "run_mode.h"
#include "driver_export.h"
#include "boot_drvapi.h"
#include "mn_api.h"

#include "dal_lcd.h"

#include "atc_port.h"

#include "prod_param.h"
#include "chg_drv.h"
#include "prod_param.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define POWER_DEBUG

#ifdef POWER_DEBUG
    #define POWER_PRINT     SCI_TRACE_LOW
#else
    #define POWER_PRINT
#endif  // POWER_DEBUG

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#ifdef WIN32
    extern void FLASH_Close(void);
#endif

extern uint32 SCI_InitApplication(void);


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
// MS status
LOCAL MCU_MODE_E                s_reset_mode        = NORMAL_MODE;
LOCAL POWER_RESTART_CONDITION_E s_restart_condition = RESTART_BY_NONE;
LOCAL POWER_MS_STATUS_E         s_power_ms_status   = POWER_MS_STATUS_POWER_OFF;
LOCAL NET_EXIT_FPTR             s_net_exit          = SCI_NULL;
LOCAL uint16                    calibration_post_mode_frequency = 0;

LOCAL uint32                    s_auto_test_flag = 0;
#define		POWER_AUTOTESTMODE		(AUTO_TEST_MODE)

LOCAL PCALCULATE_STANDBY_TIME CalculateStandbyTimePtr =SCI_NULL;


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void Power_CalculateStandbyTime(void);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
// Description :    Whether in auto test mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
 BOOLEAN POWER_IsAutoTestMode(void)
{
    if(s_auto_test_flag == POWER_AUTOTESTMODE)
    {
	    return SCI_TRUE;
	}
    return SCI_FALSE;
}
/*****************************************************************************/
// Description :    Set to auto test mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
BOOLEAN POWER_SetAutoTestMode()
{
	s_auto_test_flag = POWER_AUTOTESTMODE;
	return SCI_TRUE;
}
/*****************************************************************************/
// Description :    clear auto test mode.
// Global resource dependence :
// Author :        lianlian.huang
// Note :
/*****************************************************************************/
BOOLEAN POWER_ClearAutoTestMode()
{
	s_auto_test_flag = 0;
	return SCI_TRUE;
}

/*****************************************************************************/
// Description :    Handle power on operate.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
extern void DAPS_Start( void );
PUBLIC void  POWER_PowerOn(void)
{
	uint16	reset_mode = POWER_GetResetMode();

	//POWER_PRINT:"POWER_PowerOn !!!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,POWER_118_112_2_18_1_5_27_48,(uint8*)"");

	if (POWER_GetMsStatus() == POWER_MS_STATUS_POWER_ON)
	{
		return;
	}

	POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_ON);
	#ifdef WIN32
	SCI_InitL4();
	SCI_InitPs();
	DAPS_Start();
	#else	// not define WIN32

	#if defined(PLATFORM_UWS6121E)
	       // init RPC and l4
              CFW_RpcInit();
	#endif

	// always need l4
	SCI_InitL4();

	#if !defined(PLATFORM_UWS6121E)
	       // Initialize PS. and create all PS task.
              SCI_InitPs();
	#else
	       // init ipc bearer for tcpip/ppp
	       TCPIP_ipcBear_init();
	#endif

	  // Initialize Data Application Protocol Stack.
               DAPS_Start();
    #ifdef PPP_USB
            PPP_TaskStart();
	#endif
	#ifdef PRJ_TDPS_UEIT_MMI
	       SCI_RegLogGprsMsg(REF_PktGprsMsg);
	#endif

	#if !defined(PLATFORM_UWS6121E)
	        // @Ivan.Yin added , register for GPRS log
	       SCI_RegLogGprsMsg(REF_PktGprsMsg);
	       // Initialize Layer1.
	       SCI_InitLayer1();
	#endif
	#endif
	/* @jim.zhang CR:MS10763 2004-07-22 */
	if (!RM_GetPinTestFlag())
	{
		if( (reset_mode != CALIBRATION_MODE) && (reset_mode != CALIBRATION_POST_NO_LCM_MODE) )
		{
			// Initialize Application tasks and resources.
			SCI_InitApplication();
		}
		if(reset_mode == CALIBRATION_POST_NO_LCM_MODE)
		{
			SCI_InitAtc();

			//Activate PS.
			MNPHONE_StartupPs(3);

		}
	}
	else
	{
		//Activate PS.
		MNPHONE_StartupPs(3);
	}

    POWER_SetMsStatus(POWER_MS_STATUS_POWER_ON);
}

/*****************************************************************************/
// Description :    Handle power off operate.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_PowerOff(void)
{
#ifndef WIN32
    CHIP_Disable7sRstMode();  //硬件关机过程中关闭7秒重启
#endif
#ifndef WIN32
    PROD_Capacity_Save_NV();
#endif
    //POWER_PRINT:"POWER_PowerOff !!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,POWER_182_112_2_18_1_5_27_49,(uint8*)"");
    {
        // Delay a while to trace.
        uint32 i;
        i = 0xFFFF;
        while (i--){};
    }
    // Handle TCP/IP.
    if (SCI_NULL != s_net_exit)
    {
        (*s_net_exit)(0);
    }

#if 0//defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    GPS_PowerOff();
#endif

    //rex.yang add to record standby time start
#ifndef WIN32
    {
        CHGMNG_DISCHARGE_PARAM_T dischg = CHGMNG_GetDischgParam();
        CHGMNG_STATE_INFO_T* p_state = CHGMNG_GetModuleState();
        if(p_state->bat_cur_vol < dischg.shutdown_vol)
        {
            if(!CHGMNG_IsChargeConnect())
            {
                Power_CalculateStandbyTime();
            }
        }
    }
    // rex.yang add to record standby time end

#endif
#ifdef WIN32
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    SCI_DisableIRQ();
    FLASH_Close();
    SCI_RestoreIRQ();

    POWER_SetMsStatus(POWER_MS_STATUS_POWER_OFF);

    // Reset RTOS.
    SCI_ResetRtos();

#else
    LCD_Close();

    {// @lin.liu. CR12921.
     // To flush all nvitem before power. Only be called when the caller is from a task.
     // For the NVITEM_FlushAll not support the non-thread call.
        BLOCK_ID  thread;

        thread = SCI_IdentifyThread();
        if( SCI_INVALID_BLOCK_ID != thread )
        {
            NVITEM_Uninit();
        }
    }
    //@lin.liu. CR11795
    // let platform function to close some device( eg. the lcd/keypad backlight )
    {
        void SCI_PrePowerOff( void );

        SCI_PrePowerOff( );
    }

    s_auto_test_flag = 0;
    HAL_TurnOffPower();
#endif

}

/*****************************************************************************/
// Description :    Handle reset operate.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_Reset(void)
{
    //POWER_PRINT:"POWER_Reset !!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,POWER_260_112_2_18_1_5_27_50,(uint8*)"");
    
#ifndef WIN32
    CHIP_Disable7sRstMode();  //硬件重启过程中关闭7秒重启
#endif
    
    // Handle TCP/IP.
    if (SCI_NULL != s_net_exit)
    {
        (*s_net_exit)(0);
    }

#ifdef WIN32
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    SCI_DisableIRQ();
    FLASH_Close();
    SCI_RestoreIRQ();

    POWER_SetMsStatus(POWER_MS_STATUS_POWER_OFF);

    // Reset RTOS.
    SCI_ResetRtos();

#else

    /* CR:555820 2016-06-01 */
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    //turn off backlight before LCD Sleep
    GPIO_SetLcdBackLight (SCI_FALSE);/*lint !e718 !e18 */

    LCD_Close();

    // Change MCU mode.
    BOOT_SetResetMode (RESET_MODE);

    HAL_MCUReset();
#endif

}

PUBLIC void  POWER_UpdateReset(void)
{
    // Handle TCP/IP.
    if (SCI_NULL != s_net_exit)
    {
        (*s_net_exit)(0);
    }

#ifndef WIN32
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    GPIO_SetLcdBackLight (SCI_FALSE);/*lint !e718 !e18 */

    LCD_Close();

    BOOT_SetResetMode (UPDATE_RESET_MODE);

    HAL_MCUReset();
#endif

}

#ifndef FOTA_SUPPORT_NONE
PUBLIC void  POWER_FOTA_Update_Reset(void)
{
    // Handle TCP/IP.
    if (SCI_NULL != s_net_exit)
    {
        (*s_net_exit)(0);
    }

#ifndef WIN32
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    GPIO_SetLcdBackLight (SCI_FALSE);/*lint !e718 !e18 */

    LCD_Close();

    BOOT_SetResetMode (FOTA_RESET_MODE);

    HAL_MCUReset();
#endif

}
#endif

/*****************************************************************************/
// Description :    Handle reset operate, reset from boot mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_BootReset(void)
{
    // Handle TCP/IP.
    if (SCI_NULL != s_net_exit)
    {
        (*s_net_exit)(0);
    }

#ifdef WIN32
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    SCI_DisableIRQ();
    FLASH_Close();
    SCI_RestoreIRQ();

    POWER_SetMsStatus(POWER_MS_STATUS_POWER_OFF);

    // Reset RTOS.
    SCI_ResetRtos();

#else
    LCD_Close();

    // Change MCU mode.
    BOOT_SetResetMode (RESET_MODE);

    SCI_DisableIRQ();
    HAL_MCUBootReset();
    SCI_RestoreIRQ();
#endif
}

/*****************************************************************************/
// Description :    TCP/IP regist power off function.
//                  Function prototype: void func_name(int)
// Global resource dependence :
// Author : Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_RegNetExit(NET_EXIT_FPTR net_exit)
{
    s_net_exit = net_exit;
}

/*****************************************************************************/
// Description :    Get the status of MS.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC POWER_MS_STATUS_E POWER_GetMsStatus(void)
{
    return s_power_ms_status;
}

/*****************************************************************************/
// Description :    Set the status of MS.
// Global resource dependence :
// Author : Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_SetMsStatus(POWER_MS_STATUS_E status)
{
    s_power_ms_status = status;
}

/*****************************************************************************/
// Description :    This function gets restart condition.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
POWER_RESTART_CONDITION_E POWER_GetRestartCondition(void)
{
#ifndef FPGA_VERIFICATION
    //POWER_PRINT:"POWER_GetRestartCondition: Geted value is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,POWER_366_112_2_18_1_5_28_51,(uint8*)"d", s_restart_condition);

	return s_restart_condition;
#else
	return RESTART_BY_SOFTWARE;
#endif
}

/*****************************************************************************/
// Description :    This function sets restart condition.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_SetRestartCondition(POWER_RESTART_CONDITION_E condition)
{
    //POWER_PRINT:"POWER_SetRestartCondition: Set to %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,POWER_380_112_2_18_1_5_28_52,(uint8*)"d", condition);

	s_restart_condition = condition;
}

/*****************************************************************************/
//  Description:    The function used to set mcu reset mode.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void POWER_SetResetMode(MCU_MODE_E mode)
{
    s_reset_mode = mode;

#ifndef WIN32
    if(RM_GetDloaderMode())
    {
        /*
        *  Dloader Mode is 1 only when usb mcft is working
        */
        BOOT_SetUSBConnectStatus (1);
    }
    // Change MCU mode.
    BOOT_SetResetMode (mode);

    HAL_MCUReset();

#endif

}

/*****************************************************************************/
//  Description:    The function used to get mcu reset mode.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/

PUBLIC MCU_MODE_E POWER_GetResetMode(void)
{
#ifndef WIN32
  // linker stub, use normal
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
       MCU_MODE_E	mode;
	BOOLEAN usb_is_connected;

    // Get MCU mode.
    mode = (MCU_MODE_E)BOOT_GetUSBConnectStatus ();

    if(mode & MCU_MODE_USB)
    {
        usb_is_connected = SCI_TRUE;
    }
    else
    {
        usb_is_connected = SCI_FALSE;
    }

    mode = (MCU_MODE_E)BOOT_GetResetMode ();

    if(s_auto_test_flag ==0)
    {
   		s_auto_test_flag = (mode&0xFF);
    }

    if(mode >= MODE_MAX_TYPE)
    {
        return s_reset_mode;
    }

    // First get mode, return value in Register to user.
    // Else return the value saved in s_reset_mode to user.
    /* CR:555820 2016-06-01 */
    if (NORMAL_MODE != mode && POWER_GetMsStatus() != POWER_MS_STATUS_HANDLE_POWER_OFF)
    {
        // First get mode.
        s_reset_mode =(MCU_MODE_E) mode;

        // Set MCU mode to default mode NORMAL_MODE.
        BOOT_SetResetMode (NORMAL_MODE);
    }

	return ((usb_is_connected)?(s_reset_mode|MCU_MODE_USB):s_reset_mode);
#else
    return NORMAL_MODE;
#endif
#else
    return NORMAL_MODE;
#endif
#if 0
#ifndef WIN32
	MCU_MODE_E	mode;
	BOOLEAN usb_is_connected;

    // Get MCU mode.
    mode = (MCU_MODE_E)BOOT_GetUSBConnectStatus ();

    if(mode & MCU_MODE_USB)
    {
        usb_is_connected = SCI_TRUE;
    }
    else
    {
        usb_is_connected = SCI_FALSE;
    }

    mode = (MCU_MODE_E)BOOT_GetResetMode ();

    if(s_auto_test_flag ==0)
    {
   		s_auto_test_flag = (mode&0xFF);
    }

    if(mode >= MODE_MAX_TYPE)
    {
        return s_reset_mode;
    }

    // First get mode, return value in Register to user.
    // Else return the value saved in s_reset_mode to user.
    /* CR:555820 2016-06-01 */
    if (NORMAL_MODE != mode && POWER_GetMsStatus() != POWER_MS_STATUS_HANDLE_POWER_OFF)
    {
        // First get mode.
        s_reset_mode =(MCU_MODE_E) mode;

        // Set MCU mode to default mode NORMAL_MODE.
        BOOT_SetResetMode (NORMAL_MODE);
    }

	return ((usb_is_connected)?(s_reset_mode|MCU_MODE_USB):s_reset_mode);
#else
	return s_reset_mode;
#endif
#endif

}

/*****************************************************************************/
//  Description:    The function used to check if in calibration mcu reset mode.
//	Global resource dependence:
//  Author:         Liangwen.Zhen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN POWER_IsCalibrationResetMode(void)
{
	MCU_MODE_E reset_mode = POWER_GetResetMode();

	if((CALIBRATION_POST_MODE == reset_mode)|| \
       (CALIBRATION_MODE == reset_mode) || \
       (CALIBRATION_POST_NO_LCM_MODE == reset_mode) || \
       (CALIBRATION_NV_ACCESS_MODE == reset_mode) ||\
       (TD_CALIBRATION_MODE == reset_mode) ||\
       (TD_CALIBRATION_POST_MODE == reset_mode) || \
       (TD_CALIBRATION_POST_NO_LCM_MODE == reset_mode)
     )
	{
		return SCI_TRUE;
	}
	else
	{
		return SCI_FALSE;
	}

}

/*****************************************************************************/
//  Description:    The function used to Get the frequency parameter set in calibration post mode.
//	Global resource dependence:
//  Author:        king.li
//	Note:    2008.02.18
/*****************************************************************************/
PUBLIC uint16 POWER_GetCalibrationPostModeFreq(void)
{
     return calibration_post_mode_frequency;
}

/*****************************************************************************/
//  Description:    The function used to set the frequency parameter in calibration post mode.
//	Global resource dependence:
//  Author:        king.li
//	Note:    2008.02.18
/*****************************************************************************/
PUBLIC void POWER_SetCalibrationPostModeFreq(uint16 freq)
{
    calibration_post_mode_frequency=freq;
}

/*****************************************************************************/
//  Description:  The function used to register the calculate standby time function from Mmidisk.c
//	Global resource dependence:
//  Author: rex yang
//	Note:
/*****************************************************************************/
PUBLIC void Power_RegFunCalculateStandby(PCALCULATE_STANDBY_TIME pcalculatestandby)
{
    if(SCI_NULL!=pcalculatestandby)
    {
        CalculateStandbyTimePtr=pcalculatestandby;
    }
}

/*****************************************************************************/
//  Description:  The function used to the calculate standby time function register by Mmidisk.c
//	Global resource dependence:
//  Author: rex yang
//	Note:
/*****************************************************************************/
LOCAL void Power_CalculateStandbyTime(void)
{
    if(SCI_NULL!=CalculateStandbyTimePtr)
    {
        (*CalculateStandbyTimePtr)();
    }
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif // End of power.c
