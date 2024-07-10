#ifdef UI_P3D_SUPPORT   
/*****************************************************************************
** File Name:      mmi_winse_imp_fade_inout.c                                    *
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
#define POINT_ADD_COUNT (1024)
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  Fade InOut Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeFadeInOut(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr);
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//LOCAL FIX16_POINT_T s_point_add_ptr[POINT_ADD_COUNT] = {0};

const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_inout = 
{
    WinSeFadeInOut,
    PNULL,    //支持播放函数
    MMI_WIN_ANIM_SE_TYPE_FADE_INOUT,      //特效的type
    MMI_WIN_ANIM_SE_SETTING_FADE_INOUT,  //为了匹配原有的特效，这个与MMI_WIN_ANIM_SE_TYPE_FADE对应
    15,                  //总的帧数
    TRUE,          //是否存在前景层跟背景层
    0,
    0,     //这个参数是个flag，是跟播放声音函数配合使用
    TXT_SET_3D_STYLE_SCREEN_FADE_INOUT,
    POINT_ADD_COUNT*sizeof(MMI_WIN_SE_IMPLEMENTOR)
};
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  Fade Anim Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Fade_In_Out(uint32 * bg_buff,uint32 * next_color_buff, IMGREF_SIZE_T window_size,int cur_frame,int total_frame, FIX16_POINT_T * Point_Add_ptr,BOOLEAN Is_Back)
{	
    int next_z_para=0,next_alpha=0,SinData=0;
    int min_z_para=4096*3/4;
    IMGREF_POINT_T window_mid={0},img_mid={0};

    window_mid.x=window_size.w/2;
    window_mid.y=window_size.h/2;
    img_mid.x=window_size.w/2;
    img_mid.y=window_size.h/2;

    if(0==total_frame)
    {
        return SCI_FALSE;
    }
    
    if(Is_Back)
    {	
        SinData=s2d_sin_new(900*(cur_frame)/total_frame);
        next_alpha=80+(255-80)*SinData/(65536);
        next_z_para=min_z_para+(4096-min_z_para)*SinData/(65536);
        S2d_4_in_one_fast_Out32(window_size,   window_size, window_mid,  img_mid,  0, next_color_buff,   bg_buff,  next_z_para,  next_alpha,  Point_Add_ptr);
    }
    else
    {	
        SinData=s2d_sin_new(900+(-900)*(cur_frame)/total_frame);
        if(0!=SinData)
        {
            next_alpha=255*SinData/65536;
            next_z_para=min_z_para+(4096-min_z_para)*SinData/65536;
            S2d_4_in_one_fast_Out32(window_size,   window_size, window_mid,  img_mid,  0, next_color_buff,   bg_buff,  next_z_para,  next_alpha,  Point_Add_ptr);
        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description:  Fade Inout Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeFadeInOut(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr)
{
    IMGREF_SIZE_T ref_size = {0};
    
    if(PNULL == in_param_ptr || PNULL == out_param_ptr|| PNULL == in_param_ptr->multi_layer.fg_buffer_ptr || PNULL == in_param_ptr->multi_layer.src_buffer_ptr
        || PNULL == in_param_ptr->ext_ptr )
    {
        return;
    }
    
    ref_size.w = in_param_ptr->buf_width;
    ref_size.h = in_param_ptr->buf_height;
    Fade_In_Out(in_param_ptr->multi_layer.fg_buffer_ptr,
                            in_param_ptr->multi_layer.src_buffer_ptr,ref_size,
                            in_param_ptr->frame_index,
                            in_param_ptr->frame_count,
                            (FIX16_POINT_T*)in_param_ptr->ext_ptr,//s_point_add_ptr,
                            in_param_ptr->is_back
                            );
}
#endif /*_MMI_WINSE_IMP_C_*/
#endif
