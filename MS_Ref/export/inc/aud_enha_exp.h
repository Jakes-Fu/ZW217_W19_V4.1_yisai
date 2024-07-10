/******************************************************************************
** File Name:      aud_enha_exp.h                                            
** Author:         cherry.liu                                              
** DATE:           12/16/2010                                                
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of audio enhanced express plugger.                   
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 12/16/2010      cherry.liu      Create.                                   
******************************************************************************/

#ifndef _AUD_ENHA_EXP_H_
#define _AUD_ENHA_EXP_H_
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
#include "audio_api.h"
#include "vb_drvapi.h"
#include "eq_exp.h"
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

#define NAME_LEN_MAX (16)
#define EQ_BAND_MAX   8
#define EQ_MODE_MAX   6
#define AUD_ENHA_EXP_PARA_COUNT 2	
#define TOTAL_S_GAIN_NUM  7 //s0-s6
#define HPF_S_GAIN_NUM    6 //s0-s5
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{   
    AUD_ENHA_EQPARA_NULL = 0,
    AUD_ENHA_EQPARA_HEADSET ,// 1
    AUD_ENHA_EQPARA_HEADFREE,   // 2
    AUD_ENHA_EQPARA_HANDSET,   // 3
    AUD_ENHA_EQPARA_HANDSFREE,// 4
   #if defined(CUS_ADD_SHAKE)
    AUD_ENHA_EQPARA_HANDSFREE_S, // 5    for SHAKE WHILE MUSIC
   #else
    AUD_ENHA_EQPARA_BTHS,          // 5    for mp3
   #endif 
   #ifdef FM_VBC_EQ
    AUD_ENHA_EQPARA_HEADSET_ONE,   // 6    for FM
    AUD_ENHA_EQPARA_HANDSFREE_ONE, // 7    for FM
   #endif
    AUD_ENHA_EQPARA_MAX// 8
}AUD_ENHA_EQPARA_SET_E;

typedef enum
{   
    AUD_ENHA_TUNABLE_EQPARA_NULL=0,//0
    AUD_ENHA_TUNABLE_EQPARA_COMMON,// 1
    AUD_ENHA_TUNABLE_EQPARA_MAX//  2
}AUD_ENHA_TUNABLE_EQPARA_SET_E;

typedef enum
{   
    AUD_ENHA_EQMODE_SEL_OFF = 0,     //0
    AUD_ENHA_EQMODE_SEL_REGULAR ,   // 1
    AUD_ENHA_EQMODE_SEL_CLASSIC,   // 2
    AUD_ENHA_EQMODE_SEL_ODEUM,     // 3
    AUD_ENHA_EQMODE_SEL_JAZZ,      // 4
    AUD_ENHA_EQMODE_SEL_ROCK,      // 5
    AUD_ENHA_EQMODE_SEL_SOFTROCK,// 6
    AUD_ENHA_EQMODE_SEL_MMISET =15,// 15
    AUD_ENHA_EQMODE_SEL_MAX
}AUD_ENHA_EQMODE_SEL_E;

//------------definition for getting params for this plugger ----------------//
typedef struct 
{	  
    BOOLEAN isEqTunableEn;
    BOOLEAN pad[3];
    uint32 eqLevelN;// level renge :-eqLevelN,-eqLevelN+1,...,eqLevelN-1,eqLevelN
    uint32 eqLevelStep;
    uint32 eqBandNum;
    uint32 eqbandFreq[EQ_BAND_MAX];   
    int16  boostdB_default[EQ_EXP_MODE_MAX][EQ_BAND_MAX];//mapped the enum definition of EQ_EXP_MODE_E
    uint32 eqMode;
}AUD_ENHA_EXP_EQ_PARA_INFO_T;



//------------definition for setting params for this plugger ----------------//
typedef enum//  3  para  types
{   
    AUD_ENHA_PARA_EQ_MODE = 0,
    AUD_ENHA_PARA_DIGI_GAIN ,
    AUD_ENHA_PARA_EQ_SET ,
    AUD_ENHA_PARA_DEV_MODE,
    AUD_ENHA_PARA_BAND_BOOST,  
    AUD_ENHA_PARA_SET_DEFAULT,   
    AUD_ENHA_PARA_EQ_ALLBANDS_BOOST,
    AUD_ENHA_PARA_TUNABLE_EQ_SET,
    AUD_ENHA_PARA_MAX
}AUD_ENHA_PARA_TYPE_E;


typedef struct 
{
    uint32 eqMode;
    int16  bandIndex;			  
	int16  bandBoost; 
}AUD_ENHA_EXP_BAND_INFO_T; 

typedef struct 
{
    uint32 eqMode; 
    int16  bandNum;
	int16  bandBoost[EQ_BAND_MAX]; 
}AUD_ENHA_EXP_ALLBANDS_INFO_T;

typedef struct 
{
    uint32  eqSetIndex;		  
	uint32  eqMode; 
}AUD_ENHA_EXP_EQ_MODE_SET_T;

union Aud_enha_para_union
{
    uint32 eqMode;
    uint32 digitalGain;                
    AUD_ENHA_EXP_EQ_MODE_SET_T eqSetInfo;
    char *devModeName;
    AUD_ENHA_EXP_BAND_INFO_T bandInfo;
    AUD_ENHA_EXP_ALLBANDS_INFO_T modeInfo;
};

typedef struct 
{
    AUD_ENHA_PARA_TYPE_E eParaType;			  
	union Aud_enha_para_union unAudProcPara; 
}AUD_ENHA_EXP_PARA_T;

//---------------------------control params in nv ----------------------//
//untunable eq
typedef struct 
{
    int16   fo ;  /*f0*/
    int16   q;    /*q*/   
    int16   boostdB;   /*boost */
    int16   gaindB ;      /*gain*/
}EQ_BAND_INPUT_PARA_T;

typedef struct 
{
    int16   agc_in_gain;  /*agc in gain set*/
    int16   band_control; /*bit15-bit8 :filter_sw_1~8 ; bit 1: high shelve;bit0:low shelve */
    EQ_BAND_INPUT_PARA_T  eq_band[EQ_BAND_MAX];   
}EQ_MODE_PARA_T;  

typedef struct //PACKED  272 words
{
    uint8   para_name[NAME_LEN_MAX];/*struct name*/ 
    uint16  eq_control;//bit15:8-bands-sw
    EQ_MODE_PARA_T eq_modes[EQ_MODE_MAX];     /*eq mode para*/
    int16 externdArray[59]; /*reserved for future*/
}AUDIO_ENHA_EQ_STRUCT_T;

//tunable eq
typedef struct 
{
    int16   agc_in_gain;  /*agc in gain set*/
    int16   band_control;
    int16   boostdB_default[EQ_BAND_MAX];  /*default boost dB for each band*/
    int16   boostdB_current[EQ_BAND_MAX];  /*current boost dB for each band;set by mmi*/
}TUNABLE_EQ_MODE_PARA_T;

typedef struct //PACKED 188 words
{
    uint8   para_name[NAME_LEN_MAX];/*struct name*/ 
    int16   eq_control;       /* bit15:8-bands-sw;bit9-bit0:level_step;*/
    int16   fo_array[EQ_BAND_MAX];
    int16   q_array[EQ_BAND_MAX];
    int16   level_n;
    TUNABLE_EQ_MODE_PARA_T eq_modes[EQ_MODE_MAX];     /*eq mode para*/
    int16 externdArray[54]; /*reserved for future*/
}AUDIO_ENHA_TUNABLE_EQ_STRUCT_T;

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                        Function Declare                               **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    register audio enhanced plugger
//  Author:         Cherry.Liu
//  Note:           
//****************************************************************************/  
PUBLIC HAUDIOEXP AUDENHA_EXP_RegExpPlugger( 
    void
    );

/*****************************************************************************/
//  Description:    read all eq para sets from flash nv
//  Author:         Cherry.Liu
//  Note:           should be called when the mobile phone is powered.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E  AUDENHA_EXP_GetEqParaFromNv(
    void
    );

/*****************************************************************************/
//  Description:    read  one set of eq para from ram nv
//  Author:         Cherry.Liu
//  Note:           for untunable eq para set
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E  AUDENHA_EXP_GetEqSetFromRam(
    AUD_ENHA_EQPARA_SET_E   eq_para_set_index, // 1,2,3,4
    AUDIO_ENHA_EQ_STRUCT_T* eq_para_mem_ptr //out ptr
);

/*****************************************************************************/
//  Description:    1. set eq para to ram nv .
//                  2. set updated para to audio plugger.
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDENHA_EXP_SetEqSetToRam(
    AUD_ENHA_EQPARA_SET_E     eq_para_set_index, // 1,2,3,4
    AUDIO_ENHA_EQ_STRUCT_T*   eq_para_set_ptr,
    AUD_ENHA_EQMODE_SEL_E     eq_mode_sel //  1,2,3,4,5,6
    
);

/*****************************************************************************/
//  Description:    read  one  set of eq para from flash nv
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E  AUDENHA_EXP_GetEqSetFromFlash(
    AUD_ENHA_EQPARA_SET_E     eq_para_set_index, // 1,2,3,4
    AUDIO_ENHA_EQ_STRUCT_T*   eq_para_mem_ptr
);

/*****************************************************************************/
//  Description:    set current eq para to flash nv .
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDENHA_EXP_SetEqSetToFlash(
    AUD_ENHA_EQPARA_SET_E     eq_para_set_index, // 1,2,3,4
    AUDIO_ENHA_EQ_STRUCT_T*   eq_para_mem_ptr
);


/*****************************************************************************/
//  Description:    read  one set of tunable eq para from ram nv
//  Author:         Cherry.Liu
//  Note:           for tunable eq para set
//****************************************************************************/ 
PUBLIC AUDIO_RESULT_E   AUDENHA_EXP_GetTunableEqSetFromRam(
    AUD_ENHA_TUNABLE_EQPARA_SET_E   tunable_eq_para_set_index, // 1
    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T* tunable_eq_para_mem_ptr //out ptr
);

/*****************************************************************************/
//  Description:    1. set eq para to ram nv .
//                  2. set updated para to audio plugger.
//  Author:         Cherry.Liu
//  Note:           
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDENHA_EXP_SetTunableEqSetToRam(
    AUD_ENHA_TUNABLE_EQPARA_SET_E tunable_eq_para_set_index, // 1
    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T*   tunable_eq_para_set_ptr,
    AUD_ENHA_EQMODE_SEL_E     eq_mode_sel //  1,2,3,4,5,6
);

/*****************************************************************************/
//  Description:    read  one  set of tunable eq para from flash nv
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E  AUDENHA_EXP_GetTunableEqSetFromFlash(
    AUD_ENHA_TUNABLE_EQPARA_SET_E     tunable_eq_para_set_index, // 1
    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T*   tunable_eq_para_mem_ptr
);

/*****************************************************************************/
//  Description:     set current eq para to flash nv .
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC  AUDIO_RESULT_E AUDENHA_EXP_SetTunableEqSetToFlash(
    AUD_ENHA_TUNABLE_EQPARA_SET_E     tunable_eq_para_set_index, // 1
    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T*   tunable_eq_para_mem_ptr
);

/*****************************************************************************/
//  Description:    get the total number of eq para sets (EQ_NULL not included).
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC uint32 AUDENHA_EXP_GetEqParaSetNum(BOOLEAN is_eq_tunable);

#ifdef CUS_ADD_SHAKE
/*****************************************************************************/
//  Description:    set shake enable while music playing().
//  Author:         Cherry.Liu
//  Note:           this interface is designed for AT command layer.
//****************************************************************************/ 
PUBLIC void AUDENHA_EXP_SetShakeEnable(BOOLEAN is_shake_enable);
#endif
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 



#endif //end of _AUD_ENHA_EXP_H_

//end of file



