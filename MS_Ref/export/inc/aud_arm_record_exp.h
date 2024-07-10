/******************************************************************************
 ** File Name:      aud_arm_record_exp.h                                                *
 ** Author:         Cherry.Liu                                             *
 ** DATE:           04/14/2010                                               *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/14/2010       Cherry.Liu       Create.                                  *
 ******************************************************************************/

#ifndef _AUD_ARM_RECORD_EXP_H
#define _AUD_ARM_RECORD_EXP_H

#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else 
	#ifdef DSP_AUDIO_II
	#include "admm_II.h"
	#else
	#include "admm.h"
	#endif
#endif
#include "gpio_prod_api.h"

#include "aud_enha_exp.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef struct
{
    AUDIO_DEVICE_MODE_TYPE_E arm_audio_mode;
    AUD_ENHA_EQPARA_SET_E audio_eq_mode;
    char * audio_mode_name_ptr;
}AUDIO_EQ_REC_STRUCT_T;

typedef enum//  3  para  types
{   
    AUD_ARM_REC_PARA_DEV_MODE = 0,
    AUD_ARM_REC_PARA_EQ_MAX,
}AUD_ARM_REC_PARA_TYPE_E;

union Aud_arm_rec_exp_union
{
    AUDIO_DEVICE_MODE_TYPE_E devMode;
};

typedef struct 
{
    AUD_ENHA_PARA_TYPE_E eParaType;			  
	union Aud_arm_rec_exp_union unAudProcPara; 
}AUD_ARM_REC_EXP_PARA_T;

#define AUD_ARM_REC_EXP_PARA_COUNT 2	

/**---------------------------------------------------------------------------*
 **                      public  Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUD_rec_RegExpPlugger(
//!    void
//!    ) 
//! \param void no para
//! \return return express handle 
//! \brief  Description:  This function is to register an DG express plugger.
//! \author Author:  Benjamin.Wang 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP AUD_rec_RegExpPlugger( 
    void
    );

PUBLIC AUDIO_OBJECT_T* AUD_rec_GetCurAudioObj(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif
