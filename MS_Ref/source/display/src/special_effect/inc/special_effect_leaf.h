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

#ifndef _SPECIAL_EFFECT_LEAF_H_
#define _SPECIAL_EFFECT_LEAF_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "special_effect.h"
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
typedef struct SE_tag_Leaf_tag {
    int32 enable;
    IMGREF_DATA_FORMAT_E  format;   
    int32 type;                 //  may be lot of leaf
    int32 start_x, start_y;
    int32 vx, vy;
    int32 alpha, d_alpha; 

    int32 touched;

    int32 egg;
}SE_LEAF_T;

    

typedef struct effect_leaf_tag
{
    int32   handle_tag;           //  const number : " SE "
    SE_EFFECT_FUNC_T *func;    
   
    SE_EFFECT_SURFACE_T  background;
    SE_EFFECT_SURFACE_T  canvas;

    int32   leaf_kind_num;  
    SE_LEAF_TEXTURE_T *texture;  
    int32   leaves_num;  // leaf

    SE_LEAF_T *leaves;
    
    uint32   frame;

    SE_HANDLE ripple;  // ripple
} SE_LEAF_HANDLE_T;

/**---------------------------------------------------------------------------*
 **                         GLobal  Var.
 **---------------------------------------------------------------------------*/
extern SE_EFFECT_FUNC_T se_effect_leaf_func;


#ifdef __cplusplus
    }
#endif

#endif



