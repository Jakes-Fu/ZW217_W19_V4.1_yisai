/******************************************************************************
 ** File Name:      special_effect_curtain.h                                                                                            *
 ** Author:                                                                                                                                          *
 ** DATE:           2012-01-16                                                                                                             *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.                                             *
 ** Description:                                                             *
 ** Note:                                                                                                                                            *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                                                                                               *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                                                                                     *    
 ** 2012-01-16                         Create                                                                                              *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                                                                                       *
 **---------------------------------------------------------------------------*/
#ifndef _SPECIAL_EFFECT_CURTAIN_H_
#define _SPECIAL_EFFECT_CURTAIN_H_

#include "sci_types.h"
#include "img_ref_types.h"
#include "special_effect.h"
#include "special_effect_util.h"

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                                                                                        *
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                                                                                      *
**----------------------------------------------------------------------------*/

typedef enum
{
    IMGREF_GRAPH_CIRCLE = 0x00,
    IMGREF_GRAPH_MAX
    
}IMGREF_GRAPH_TYPE_E;

typedef struct imgref_circle_tag
{
    IMGREF_POINT_T cent_pt;
    uint16 radius;
    
}IMGREF_CIRCLE_T, *IMGREF_CIRCLE_T_PTR;


typedef struct imgref_update_param_tag
{
    IMGREF_RECT_T dis_rect;
    union
    {
        IMGREF_CIRCLE_T circle;
        
    }update;    
    IMGREF_GRAPH_TYPE_E graph_type;    
    
}IMGREF_UPDATE_PARM_T, *IMGREF_UPDATE_PARM_T_PTR;



typedef struct SE_Curtain_handle_tag
{
	int32 handle_tag;           	//  const number : " SE "
	SE_EFFECT_FUNC_T  *func;
	void*	context_ptr;

} SE_CURTAIN_HANLDE_T;

/**---------------------------------------------------------------------------*
**                               Data Prototype                                                                                                 *
**----------------------------------------------------------------------------*/

PUBLIC BOOLEAN IMGPROC_Curtain(IMGREF_CHN_T *data_chn_ptr,IMGREF_SIZE_T* img_size_ptr, 
                       IMGREF_CHN_T *dst_chn_ptr, IMGREF_DATA_FORMAT_E type, 
                       IMGREF_UPDATE_PARM_T* update_param_ptr);


/**---------------------------------------------------------------------------*
 **                         GLobal  Var.                                     **
 **---------------------------------------------------------------------------*/
extern SE_EFFECT_FUNC_T se_effect_curtain_func;

/**/
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                                                                                            *
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif

