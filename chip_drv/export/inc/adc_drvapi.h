/******************************************************************************
 ** File Name:      adc_drvapi.h                                                  *
 ** Author:         hao.liu                                             *
 ** DATE:           06/12/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2010     hao.liu    Create.                                   *
 ******************************************************************************/

#ifndef _ADC_DRVAPI_H_
#define _ADC_DRVAPI_H_

#ifdef __cplusplus
extern   "C"
{
#endif

//#define ADC_SCALE_3V        SCI_FALSE
//#define ADC_SCALE_1V2       SCI_TRUE

typedef enum
{
    ADIN_0 = 0,
    ADIN_1,
    ADIN_2,
    ADIN_3,
    ADIN_4,
    ADIN_5,
    ADIN_6,
    ADIN_7,
    ADIN_8,
    ADIN_9,
    ADIN_10,
    ADIN_11,
    ADIN_12,
    ADIN_13,
    ADIN_14,
    ADIN_15,
    ADIN_16,
    ADIN_17,
    ADIN_18,
    ADIN_19,
    ADIN_20,
    ADC_MAX
} ADC_E;


typedef enum
{
    ADC_SCALE_1V250 = 0,
    ADC_SCALE_2V444 = 1,
    ADC_SCALE_3V233 = 2,
    ADC_SCALE_5V000 = 3,
    ADC_SCALE_MAX
}ADC_SCALE_TYPE_E;


uint32 ADC_Init (void);
uint32 ADC_GetResultDirectly (ADC_E adcSource, ADC_SCALE_TYPE_E scale);

#ifdef __cplusplus
}
#endif

#endif  // _ADC_DRVAPI_H_

