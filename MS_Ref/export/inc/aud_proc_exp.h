/******************************************************************************
** File Name:      aud_proc_exp.h                                            
** Author:         cherry.liu                                              
** DATE:           04/15/2010                                                
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of aud proc express plugger including lcf,agc and eq.                   
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 04/15/2010     cherry.liu       Create.                                   
******************************************************************************/
 
  
#ifndef _AUD_PROC_EXP_H_
#define _AUD_PROC_EXP_H_
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
#include "eq_exp.h"
#include "audio_api.h"
#include "audio_nv_arm.h"
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
/*! \def AUD_PROC_EXP_PARA_COUNT
 *  define the count of aud proc express para
 */
#define AUD_PROC_EXP_PARA_COUNT 2	


#define  TOTAL_EQ_MODE_NUM  6
#define  TOTAL_EQ_DEV_NUM   4
#define  AUD_PROC_NAME_MAX_LEN (16)



 /**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum//  3  para  types
{   
    AUD_PROC_PARA_NULL = 0,
    AUD_PROC_PARA_EQ_SET ,
    AUD_PROC_PARA_AGC,    
    AUD_PROC_PARA_EQ_MODE,
    AUD_PROC_PARA_MAX
}AUD_PROC_PARA_TYPE_E;	

typedef enum//  
{   
    AUD_PROC_EQPARA_NULL = 0,
    AUD_PROC_EQPARA_HEADSET ,// 1
    AUD_PROC_EQPARA_HEADFREE,   // 2 
    AUD_PROC_EQPARA_HANDSET,// 3
    AUD_PROC_EQPARA_HANDSFREE,// 4
    AUD_PROC_EQPARA_MAX
}AUD_PROC_EQPARA_SET_E;

typedef struct 
{
    int16 f1_g0;  
    int16 f1_g1;  
    int16 f1_fp;  
  
    int16 f2_g0;  
    int16 f2_g1;    
    int16 f2_fp;  
}LCF_FILTER_NV_PARAM_T; 

typedef struct 
{
    int16 bass_fo;          
    int16 bass_df;          
    int16 bass_boostdB;            
    int16 bass_gaindB;   
}EQ_BASS_NV_PARAM_T;  

typedef struct 
{ 
    int16 treble_fo;          
    int16 treble_df;          
    int16 treble_boostdB;            
    int16 treble_gaindB; 
}EQ_TREBLE_NV_PARAM_T;  
 
 typedef struct 
{
    LCF_FILTER_NV_PARAM_T lcf_filter;	
    int16   agc_in_gain;
    EQ_BASS_NV_PARAM_T eq_bass;
    EQ_TREBLE_NV_PARAM_T eq_treble;
 
} EQ_MODE_NV_PARAM_T;  

typedef struct //PACKED 
{
    uint8 ucParaSetName[AUD_PROC_NAME_MAX_LEN];	// EQ PARA_SET name
    uint16 eq_mode_control;
    EQ_MODE_NV_PARAM_T eq_modes[TOTAL_EQ_MODE_NUM];

}AUDIO_EQ_STRUCT_T;//totally 99 words


typedef enum//10 eq modes
{    
    AUD_PROC_EQ_MODE_BASS    = 0,
    AUD_PROC_EQ_MODE_TREBLE      ,   // 1
    AUD_PROC_EQ_MODE_VOCAL       ,   // 2
    AUD_PROC_EQ_MODE_POWER       ,   // 3
    AUD_PROC_EQ_MODE_POP         ,   // 4
    AUD_PROC_EQ_MODE_ROCK        ,   // 5
    AUD_PROC_EQ_MODE_CLASSIC     ,   // 6
    AUD_PROC_EQ_MODE_SPEAKER_LCF ,   // 7
    AUD_PROC_EQ_MODE_PASS        ,   // 8
    AUD_PROC_EQ_MODE_MAX         ,   // 9
    AUD_PROC_EQ_MODE_INVALID = 0xFF 
}AUD_PROC_EXP_EQ_MODE_E;	// just names differ. same as EQ_EXP_MODE_E


union Aud_proc_para_union
{

    int32 digitalGain;                //agc
    AUD_PROC_EXP_EQ_MODE_E eqMode;
};

 typedef struct 
{
    AUD_PROC_PARA_TYPE_E eParaType;			  
	union Aud_proc_para_union unAudProcPara; 

}AUD_PROC_EXP_PARA_T;


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
//  Description:    register audio process plugger
//  Author:         Cherry.Liu
//  Note:           
//****************************************************************************/  
PUBLIC HAUDIOEXP AUDPROC_EXP_RegExpPlugger( 
    void
    );

/*****************************************************************************/
//  Description:    read all eq para sets from flash nv
//  Author:         Cherry.Liu
//  Note:           should be called when the mobile phone is powered.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E  AUDPROC_EXP_GetEqParaFromNv(
    void
    );

/*****************************************************************************/
//  Description:    read  one  set of eq para from ram nv
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_EQ_STRUCT_T*  AUDPROC_EXP_GetEqSetFromRam(
    AUD_PROC_EQPARA_SET_E eq_para_set_index // 1,2,3,4
);

/*****************************************************************************/
//  Description:    1. set eq para to ram nv .
//                  2. set updated para to audio plugger.
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDPROC_EXP_SetEqSetToRam(
    AUD_PROC_EQPARA_SET_E eq_para_set_index, // 1,2,3,4
    AUDIO_EQ_STRUCT_T*   eq_para_set_ptr
);

/*****************************************************************************/
//  Description:    read  one  set of eq para from flash nv
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_EQ_STRUCT_T*  AUDPROC_EXP_GetEqSetFromFlash(
    AUD_PROC_EQPARA_SET_E eq_para_set_index // 1,2,3,4
);

/*****************************************************************************/
//  Description:    set current eq para to flash nv .
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDPROC_EXP_SetEqSetToFlash(
    AUD_PROC_EQPARA_SET_E eq_para_set_index, // 1,2,3,4
    AUDIO_EQ_STRUCT_T*   eq_para_set_ptr
);

/*****************************************************************************/
//  Description:    get the total number of eq para sets.
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC uint32 AUDPROC_EXP_GetEqParaSetNum(void);
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _TEST_H

//end of file







