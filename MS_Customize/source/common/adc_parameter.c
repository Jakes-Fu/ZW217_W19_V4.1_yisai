/******************************************************************************
 ** File Name:      adc_parameter.c                                           *
 ** Author:         Jim.zhang                                                 *
 ** DATE:           03/24/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function to get the           *
 **                 the ADC result : Voltage Current Temperature and          *
 **                 Volume for Project.                                       * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/24/2004     Jim.zhang        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "adc_parameter.h"
#include "tb_hal.h"
#include "sc_reg.h"
#include "prod_param.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


LOCAL uint32 current_k1 = 1;	// This shoud be modified.
LOCAL uint32 current_k2 = 1;
LOCAL uint32 temper_k1  = 1;		 
LOCAL uint32 temper_k2  = 1;
LOCAL float envtemper_k1 = 2.353;//2353;
LOCAL uint32 temper_adcenv = 340;

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the charge current from ADC result.                               *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the charge current of battery.  The units is mA.                      *
 **---------------------------------------------------------------------------*/
uint32 ADC_GetChargeCurrent( void )
{
	uint32 adc_result = ADC_GetResultDirectly(ADIN_1, ADC_SCALE_3V233);
	
	return ( adc_result * current_k1 / current_k2  );	
}

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the VBAT temperature from ADC result.                                  *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 0.01 C.                                *
 **---------------------------------------------------------------------------*/
int32 ADC_GetVbatTemperature( void )
{
    int32 temper_result = 0, adc_result = 0;
    ADC_E adc_ch = PROD_ADCGetVbatTemperCh();

	if( adc_ch == ADC_MAX)
    {
		return 0;
    }	
	
    adc_result = ADC_GetResultDirectly(adc_ch, ADC_SCALE_3V233);

    temper_result = (int32)(adc_result * temper_k1 / temper_k2);//lint !e737
	return temper_result;
}

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the environment temperature from ADC result.                                  *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 1 C.                                *
 **---------------------------------------------------------------------------*/
int32 ADC_GetEnvTemperature( void )
{ 
    int32 temper_result = 0;

    temper_result = 26 + (int32)((envtemper_k1 *(temper_adcenv - 345.0))/10);
    //SCI_TRACE_LOW:"ENVIRONMENT temper_adcenv %d, Temper %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PARAMETER_81_112_2_18_0_26_13_0,(uint8*)"dd", temper_adcenv, temper_result);
    return temper_result;
}

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the environment adc value from ADC.                               *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the adc value.                                                        *
 **---------------------------------------------------------------------------*/
uint32 ADC_GetTransceiverADCValue(void)
{
    uint32 adc_result = 0;
    uint32 adc_ch = PROD_ADCGetEnvTemperCh();
	
    if( adc_ch == ADC_MAX)
    {
		return 0;
    }		
	
	adc_result = ADC_GetResultDirectly(adc_ch, ADC_SCALE_3V233);	

    temper_adcenv = adc_result;
	return temper_adcenv;
}

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the temperature from ADC result.                                  *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 0.01 C.                                *
 **---------------------------------------------------------------------------*/
int32 ADC_GetTemperature( void )
{
    int32 temper_result;
	uint32 adc_result = ADC_GetResultDirectly(ADIN_0, ADC_SCALE_3V233);
    temper_result = (int32)( adc_result * temper_k1 / temper_k2);
	return temper_result;
}
/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the value of one ADC channel.                                     *
 ** Input Parameter                                                           *
 **     channel: ADC channel(0--4)                  					      *                                *
 ** Retrun                                                                    *
 **     the value of the speci ADC channel.(0--1023)                          *
 **     0xFFFFFFFF: error
 **---------------------------------------------------------------------------*/
uint32 ADC_GetValue( uint16 channel )
{
	if ( channel < ADC_MAX )
	{
		return ADC_GetResultDirectly((ADC_E)channel, ADC_SCALE_5V000);	
	}
	else
		return 0xFFFFFFFF;
}

int32 ADC_GetPaTemperature(void)/*lint -esym(765, ADC_GetPaTemperature)*/
{
    return 0;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif



