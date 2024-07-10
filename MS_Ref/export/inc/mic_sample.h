/**************************************************************************
 ** File Name:      mic_sample.h                                                 *
 ** Author:         Hai.Li                                          *
 ** Date:           12/21/2010                                           *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.     *
 ** Description:    This file inludes prototypes of mic_sample functions*
***************************************************************************
***************************************************************************
 **                        Edit History                                   *
 ** ----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                           *
 ** 12/21/2010     Hai.Li           Create.                               *
***************************************************************************/
#ifndef _MIC_SAMPLE_H_
#define _MIC_SAMPLE_H_

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "audio_api.h"
#include "asm.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Enum definitions                                 *
 **--------------------------------------------------------------------------*/

//mic sample type definition
typedef enum
{
    MIC_SAMPLE_128,               //get the value of db from 128 sample date
    MIC_SAMPLE_256,               //get the value of db from 256 sample date
    MIC_SAMPLE_AVERAGE_128,       //get the average value of db from 128 sample date (two times) 
    MIC_SAMPLE_MAX                // Reserved, user can't use it.
} MIC_SAMPLE_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    This Function is used to register mic sample codec plugger. 
//  Author:         Hai.Li
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC MicSample_RegCodecPlugger(HCOLLISION hCollision, AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo);


/*****************************************************************************/
//  Description:    This Function is used to get mic sample dB according as 
//                  sampling_type:
//  Param :         0=< *sampling_type <3. 
//                  MIC_SAMPLE_128=0, MIC_SAMPLE_256=1 ,MIC_SAMPLE_AVERAGE_128=2. 
//  Author:         Hai.Li
//  Note:           
/*****************************************************************************/
PUBLIC int16 AUD_GetMic_dB(uint8 sampling_type);



#ifdef __cplusplus
    }
#endif

#endif /* _MIC_SAMPLE_H_ */




