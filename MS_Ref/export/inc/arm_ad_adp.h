/******************************************************************************
** File Name:      arm_ad_adp.h                                              
** Author:         shujing.dong                                              
** DATE:           11/16/2010                                                
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of arm record codec.                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 11/16/2010     Shujing.Dong    Create.                                   *                              *
******************************************************************************/
#ifndef _ARM_AD_ADP_H
#define _ARM_AD_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "adm.h"
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
#define ARM_AD_CODEC_EXT_OPE_READ_DATA_PARA_COUNT 2
#define ARM_AD_CODEC_EXT_OPE_OFFER_CALLBACK_PARA_COUNT 2

/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef struct
{
    HAUDIO  hAudioHandle;   	
    uint16 *pusAddr;
    uint32 uiLen;
    uint32 *puiRealLen;
}ARM_AD_CODEC_EXT_OPE_READ_DATA_T;

typedef struct
{
    HAUDIO  hAudioHandle;   	
    ADM_RECCALLBACK pAdCallback;
}ARM_AD_CODEC_EXT_OPE_OFFER_CALLBACK_T;
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
PUBLIC HAUDIOCODEC ARM_AD_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );


//notes:
// pusCodecName = ANSI2UINT16("ARM_AD_CODEC");
// bVerifyAudioFormat = SCI_FALSE;
// uiTrackOnPoint = 160;


//extend operations:
// 1.
// pusExtOpeSymbol:ANSI2UINT16("READ_DATA")
// argc:   ARM_AD_CODEC_EXT_OPE_READ_DATA_PARA_COUNT
// argv:   ARM_AD_CODEC_EXT_OPE_READ_DATA_T
// 2.
// pusExtOpeSymbol:ANSI2UINT16("OFFER_CALLBACK")
// argc:   ARM_AD_CODEC_EXT_OPE_OFFER_CALLBACK_PARA_COUNT
// argv:   ARM_AD_CODEC_EXT_OPE_OFFER_CALLBACK_T
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _ARM_AD_ADP_H//end of file

