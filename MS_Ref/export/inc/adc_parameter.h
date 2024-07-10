/******************************************************************************
 ** File Name:      adc_parameter.h                                           *
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

#ifndef _ADC_PARAMETER_H
#define	_ADC_PARAMETER_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the charge current from ADC result.                               *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the charge current of battery.  The units is mA.                      *
 **---------------------------------------------------------------------------*/
uint32 ADC_GetChargeCurrent( void );


/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the Vbat temperature from ADC result.                             *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 0.01 C.                                *
 **---------------------------------------------------------------------------*/
int32 ADC_GetVbatTemperature(void);
/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the temperature from ADC result.                                  *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 0.01 C.                                *
 **---------------------------------------------------------------------------*/
int32 ADC_GetTemperature( void );

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the value of one ADC channel.                                     *
 ** Input Parameter                                                           *
 **     channel: ADC channel(0--4)                  					      * 
 ** Retrun                                                                    *
 **     the value of the speci ADC channel.(0--1023)                          *
 **---------------------------------------------------------------------------*/
uint32 ADC_GetValue( uint16 channel );

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the environment temperature from ADC result.                      *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the temperature.  The units is 1 C.                                   *
 **---------------------------------------------------------------------------*/
int32 ADC_GetEnvTemperature( void );

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the value of environment ADC channel.                             *
 ** Retrun                                                                    *
 **     the value of the speci ADC channel.(0--1023)                          *
 **---------------------------------------------------------------------------*/
uint32 ADC_GetTransceiverADCValue(void); 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _ADC_PARAMETER_H */

