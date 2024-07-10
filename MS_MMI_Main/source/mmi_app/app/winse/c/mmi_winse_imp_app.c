#ifdef UI_P3D_SUPPORT   
/*****************************************************************************
** File Name:      mmi_winse_imp_app.c                                    *
** Author:          arvin.wu                                                *
** Date:           12/10/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  10/2011      Arvin.wu            Create                                  *
******************************************************************************/
#ifndef _MMI_WINSE_IMP_C_
#define _MMI_WINSE_IMP_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 
#include "mmk_app.h" 
#include "mmi_winse.h"
#include "graphics_3d_rotate.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define FADEANIM_ALPHA_STEP (255>>3)
#define FADEANIM_TOTAL_FRAME_COUNT (8)
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                                                  */
/*---------------------------------------------------------------------------*/
                
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  Fade Anim Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeFadeAnim(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr);
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_anim = 
{
    WinSeFadeAnim,
    PNULL,    //支持播放函数
    MMI_WIN_ANIM_SE_TYPE_FADE,      //特效的type
    MMI_WIN_ANIM_SE_SETTING_FADE,  //为了匹配原有的特效，这个与MMI_WIN_ANIM_SE_TYPE_FADE对应
    8,                  //总的帧数
    FALSE,          //是否存在前景层跟背景层
    0,
    0,     //这个参数是个flag，是跟播放声音函数配合使用
    TXT_COMM_FADE_INOUT,
    0
};

/*****************************************************************************/
//  Description:  Fade Anim Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeFadeAnim(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr)
{
    uint16 *tmp_new_ptr = PNULL;
    uint16 *tmp_old_ptr = PNULL;
    uint16 *alpha_buf_ptr = PNULL;
    uint8 alpha = 0;
    uint8  n = 0;
    uint32 x = 0;
    uint32 y = 0;
    uint32 j = 0;
    uint32 result = 0;
    uint32 src_size = 0;
    
    if(PNULL == in_param_ptr || PNULL == out_param_ptr)
    {
        return;
    }
    alpha = in_param_ptr->frame_index*(uint8)FADEANIM_ALPHA_STEP;
    alpha_buf_ptr = in_param_ptr->single_layer.dest_buffer_ptr;
    tmp_new_ptr = in_param_ptr->single_layer.next_buffer_ptr;
    tmp_old_ptr = in_param_ptr->single_layer.pre_buffer_ptr;
    n = (alpha + 1) >> 3;
    src_size = in_param_ptr->buf_height*in_param_ptr->buf_width*sizeof(uint16);
    for(j = 0; j < src_size >> 1; j++)
    {
        x = *tmp_new_ptr++;
        y = *tmp_old_ptr++;
        
        x = ( x | (x << 16)) & 0x7E0F81F;
        y = ( y | (y << 16)) & 0x7E0F81F;
        result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
        *alpha_buf_ptr++ = (uint16)((result & 0xFFFF) | (result >> 16));
    }
}
#endif /*_MMI_WINSE_IMP_C_*/
#endif
