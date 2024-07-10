/******************************************************************************
** File Name:      ad_output_adp.h                                              
** Author:         shujing.dong                                              
** DATE:           11/16/2010                                                
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces of codec *
 **                 to finish arm record and output.                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 11/16/2010     Shujing.Dong    Create.                                   *                              *
******************************************************************************/
#ifndef _AD_OUTPUT_ADP_H
#define _AD_OUTPUT_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
/**---------------------------------------------------------------------------**
**                        Debugging Flag                                     **
**---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
#define AD_OUTPUT_OFFER_ADDEVICE_PARA_COUNT 2

/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef struct
{
    HAUDIO  hAudioHandle;   	
    HAUDIODEV hDeviceHandle;//handle of device which offer pcm data.
}AD_OUTPUT_CODEC_EXT_OPE_OFFER_ADDEVICE_T;

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    This Function is used to reg the codec to the codec list. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC AD_OUTPUT_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );


//notes:
// pusCodecName = ANSI2UINT16("AD_OUTPUT_CODEC");
// bVerifyAudioFormat = SCI_FALSE;
// uiTrackOnPoint = 160;

//extend operations:
// 1.
// pusExtOpeSymbol:ANSI2UINT16("OFFER_AD_DEVICE")
// argc:   AD_OUTPUT_OFFER_ADDEVICE_PARA_COUNT
// argv:   AD_OUTPUT_CODEC_EXT_OPE_OFFER_ADDEVICE_T
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _VOI//end of file

