/******************************************************************************
 ** File Name:      agc_exp.h                                                *
 ** Author:         Cherry.Liu                                             *
 ** DATE:           10/24/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/24/2009     Cherry.Liu & binggo      Create.                                  *
 ******************************************************************************/

#ifndef _AGC_EXP_H
#define _AGC_EXP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "apm_codec.h"
#include "asm.h"
#include "audio_nv_arm.h"
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
#define AGC_EXP_PARA_COUNT 1    

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef enum
{
    AUDIO_LCF_OFF,
    AUDIO_LCF_NORMAL,
    AUDIO_LCF_LOUD,
    AUDIO_LCF_MAX_TYPE  //reserve
}AUDIO_LCF_CONTROL_TYPE_E;

typedef struct 
{
    int16  iAgcControlWord[2];
	BOOLEAN bAgcEnable;       //AGC_SW
	BOOLEAN bpad;
    
	AUDIO_LCF_CONTROL_TYPE_E bLcfEnable;        
    
    int16   iDigitalGain;
    int16   iAgcInputGain;  //dg_gain_1 digital gain of agc input

    uint32 Samplerate;
}AGC_EXP_PARA_T;



typedef struct aud_AGCExp_param_t
{
	BOOLEAN enable;
	AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode;
	AUDIO_AGC_INPUG_GAIN_TYPE_E agc_type;
	uint32 Samplerate;

}AUD_AGC_EXP_RARAM_T;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    agc process
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/  
PUBLIC HAUDIOEXP AGC_EXP_RegExpPlugger( 
    void
    );
/*****************************************************************************/
//  Description:    agc process
//  Author:         peter.huang
//  Note:
//****************************************************************************/  

PUBLIC void  AGC_ReInit_Param(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode);


/*****************************************************************************/
//  Description:    agc process
//  Author:         peter.huang
//  Note:
//****************************************************************************/  
PUBLIC void AGC_Set_DigitalGain(uint32 digital_gain);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    


/*****************************************************************************/
//  Description:     Set AGC Switch To Locked Status
//  Author:            binggo
/*****************************************************************************/
PUBLIC void AUD_AGC_LockSwitch(BOOLEAN bLocked);
/*****************************************************************************/
//  Description:     Set AGC Module include enable and music type
//  Author:            binggo
/*****************************************************************************/
PUBLIC void AUD_AGC_Switch(BOOLEAN bEnable,AUDIO_AGC_INPUG_GAIN_TYPE_E eType);

/*****************************************************************************/
//  Description:     Get the the realization status of  the AGC function 
//  Author:            peter.huang
/*****************************************************************************/
PUBLIC BOOLEAN AUD_IsAGC_Support(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode , AUDIO_AGC_INPUG_GAIN_TYPE_E agc_type);

#ifdef __cplusplus
}
#endif

#endif  // _AGC_EXP_H

// End of aud_agc.h



