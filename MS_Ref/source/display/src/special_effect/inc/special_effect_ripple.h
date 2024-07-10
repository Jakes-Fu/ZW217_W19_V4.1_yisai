/******************************************************************************
 ** File Name:    special_effect_ripple.h                                             *
 ** Author:       Allen.Mao                                                   *
 ** DATE:         22/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 22/11/2010    Allen.Mao       Create.                                     *
 ******************************************************************************/

#ifndef _SPECIAL_EFFECT_RIPPLE_H_
#define _SPECIAL_EFFECT_RIPPLE_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "special_effect_util.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/
 
typedef struct effect_ripple_tag
{
    int32   handle_tag;           //  const number : " SE "
    SE_EFFECT_FUNC_T *func;    
    SE_RIPPLE_INIT_PARAM_T init; 
    int16  *pixel;                //  pointer ,size :width*height*2*2
    int16  *pFront,*pBack;        //  only pointer

} SE_RIPPLE_HANDLE_T;

typedef struct {                 //  for leaf
    SE_RIPPLE_RENDER_INPUT_PARAM_T normal;
    void * target_pixels;
    void * bg_pixels;
}SE_LEAF_RIPPLE_RENDER_INPUT_PARAM_T;

/**---------------------------------------------------------------------------*
 **                         GLobal  Var.
 **---------------------------------------------------------------------------*/
extern SE_EFFECT_FUNC_T se_effect_ripple_func;
extern SE_EFFECT_FUNC_T se_effect_ripple_func_ex;


#ifdef __cplusplus
    }
#endif

#endif



