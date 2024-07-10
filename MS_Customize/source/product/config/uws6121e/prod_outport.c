/******************************************************************************
 ** File Name:      dummy_func.c                                              *
 ** Author:         Lin.liu                                                   *
 ** DATE:           09/04/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Lin.liu          Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "cmddef.h"
#include "production_test.h"
#include "ldo_drv.h"
#include "gpio_prod_api.h"
#include "prod_outport.h"
/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
//
LOCAL BOOLEAN 	g_is_close_sd = FALSE;

void SCI_AfterChangeClk( uint32  clk )
{
}

//
// Change the power button pin function 
// For some issue, if the audion is in the board, can't sleep.
// But if remove the audion, we should change the 
void PowerButton_SetMode( BOOLEAN  bGPIO_Mode )
{
}

void SCI_BeforeEnterDeepSleep( void )
{
    //GPIO_SetFMPAEnable(FALSE);
//    LDO_TurnOffLDO(LDO_LDO_AVB);
//    LDO_TurnOffLDO(LDO_LDO_AUX2);
}

void SCI_BeforeEnterHalfDeepSleep(void)
{
}

void SCI_AfterExitDeepSleep( void )
{  
}

uint32 SCI_ProductTest_Extend( uint8 *src_ptr, uint32 src_len, uint8 *rsp_ptr, uint32 max_rsp_len )
{
    return 0;
}


void Register_MMI_Test_CmdRoutine( void )
{    
    //SCI_TRACE_LOW:"Production test dummy function: Register_MMI_Test_CmdRoutine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PROD_OUTPORT_70_112_2_18_0_26_31_36,(uint8*)"" );
    
    return;
}

void SCI_OpenCameraPower(void)
{
	 LDO_TurnOnLDO(LDO_LDO_AUX2);
}
/*
// Register 
void Register_Current_Test_CmdRoutine( void )
{    
    //SCI_TRACE_LOW:"Production test dummy function: Register_Current_Test_CmdRoutine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PROD_OUTPORT_82_112_2_18_0_26_31_37,(uint8*)"" );
    
    return;
}
*/
/*****************************************************************************/
//  Description:    The function got virtual code for up layer.
//	Global resource dependence: 
//  Author:         Jimmy.Jia
//	Note:           just return scan_code if have no external keypad
/*****************************************************************************/
int16 SCI_PreHandleKeypadInterrupt( uint16  scan_code )
{	
	// Have no external keypad setting, so we just return the code.
	return scan_code;	
}

/*****************************************************************************/
//  Description:    The function shutdown peripheral when system assert and reset
//	Global resource dependence: 
//  Author:         Jimmy.Jia
//	Note:           return NULL but the implementaion should not be blocked!!!
/*****************************************************************************/
void SCI_BeforeAssertReset(POWER_RESTART_CONDITION_E restart_condition)
{
	//SC6600L
	//DC_CloseinAssert();
		
	return;
}

BOOLEAN SCI_IsCloseSDCard(void)
{
   return (g_is_close_sd);
}

void SCI_SetCloseSDCard(BOOLEAN 	is_close )
{
	g_is_close_sd = is_close;
}

/*****************************************************************************/
//  Description:    Dummy function for ATC library.
//  Author:         Benjamin.Wang(Ask to Shijun.Cui)
//	Note:           
/*****************************************************************************/
void ATC_ExInit(void)
{
}

/*****************************************************************************/
//  Description:   	to configure SD spi bus for customer
//
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
//@Liangwen 061130
PUBLIC BOOLEAN SD_OpenSPIBusCustomer(BOOLEAN b_open)
{
	// if customer has set the SD SPI bus here, don't use defalt setting in SD_OpenSPIBus() fun, 
	// and return SCI_TRUE, or return SCI_FALSE
	return SCI_FALSE;	
}

/*****************************************************************************/
//  Description:   	This function is a temporary dummy function and will be 
//                  removed later. It is used in EFS.  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
uint32 REF_GetDspParaSize(void)
{
    return 0;
}

/*****************************************************************************/
//  Description:   	This function is a temporary dummy function and will be 
//                  removed later. It is used in EFS.  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
uint32 REF_GetCalibrationParaSize(void)
{
    return 0;
}

/*****************************************************************************/
//  Description:   	This function is a temporary dummy function and 
//  Author:         
//  Note:           
/*****************************************************************************/
uint32 REF_GetSizeOfClassmark()
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function will be called before PowerOff.
//  Input      :
//  Return     :    None
//  Author     :    Lin.liu
//	Note       :
/*****************************************************************************/
void SCI_PrePowerOff( void )
{
    CHGMNG_Close();
    GPIO_SetVibrator( 0 );
    //GPIO_SetLcdBackLight( 0 );
    GPIO_SetKeyPadBackLight( 0 );
    SCI_MMI_EnableDeepSleep(0);
    
#ifdef WIFI_SUPPORT
    GPIO_SetWifiPowerDown(SCI_TRUE);
#endif

    if( CHR_BATTERY_CHARGING_S == CHR_CheckBatteryStaus() )
    {
        // Still in charge, restart 
        POWER_SetResetMode( NORMAL_MODE );
    }	
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/* diag_routine_sp7100.c  */
