/******************************************************************************
 ** File Name:      prod_cfg.c                                             *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           08/23/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the config information of PROD          *
 **                 project.                                                  *     
 **                                                                           *
 ******************************************************************************

 **---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "os_api.h"
#include "chip.h"
#include "dal_lcd.h"
#include "sensor_drv.h"
#include "bus_monitor.h"
#include "adc_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/*****************************************************************************/
//  Description:    This function to  initialize customer config setting
//  Author:         liangwen.zhen
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_CusCfgInit(void)
{
}

/*****************************************************************************/
//  Description:    get the ADC channel for controlling battery temperature
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC ADC_E PROD_ADCGetVbatTemperCh(void)
{
    ADC_E ADC_Channel = ADC_MAX;
    return ADC_Channel;
}

/*****************************************************************************/
//  Description:    get the ADC channel for controlling environment temperature
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC ADC_E PROD_ADCGetEnvTemperCh(void)
{
    ADC_E ADC_Channel = ADC_MAX;
    return ADC_Channel;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
