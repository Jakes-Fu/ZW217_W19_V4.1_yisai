#ifdef UI_P3D_SUPPORT   
/*****************************************************************************
** File Name:      mmi_winse_imp_vertical_inout.c                                    *
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
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                                                  */
/*---------------------------------------------------------------------------*/
                
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  Vertical Inout Function
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeVerticalInOut(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr);

const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_vert_InOut = 
{
    WinSeVerticalInOut,
    PNULL,    //支持播放函数
    MMI_WIN_ANIM_SE_TYPE_VERT_INOUT,      //特效的type
    MMI_WIN_ANIM_SE_SETTING_VERT_INOUT,  //为了匹配原有的特效，这个与MMI_WIN_ANIM_SE_TYPE_FADE对应
    15,                  //总的帧数
    TRUE,          //是否存在前景层跟背景层
    0,
    0,     //这个参数是个flag，是跟播放声音函数配合使用
    TXT_SET_3D_STYLE_SCREEN_VERT_INOUT,
    0
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
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  Vertical_In_Out_32
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN Vertical_In_Out_32(uint32 * dst_buff,uint32 * next_color_buff, IMGREF_SIZE_T window_size,int cur_frame,int total_frame,BOOLEAN Is_Go_Back)
{	
    int SinData=0,copy_size=0;

    uint32 * memcpy_location=PNULL;

    if(0==total_frame)
    {
        return SCI_FALSE;
    }
    if(Is_Go_Back)
    {	
        SinData=s2d_sin_new(900*(cur_frame)/total_frame);
        memcpy_location=dst_buff+window_size.w*(window_size.h-window_size.h*SinData/65536);
        copy_size=window_size.w*window_size.h*4-window_size.w*(window_size.h-window_size.h*SinData/65536)*4;
        SCI_MEMCPY(memcpy_location, next_color_buff,copy_size );
    }
    else
    {	
        SinData=s2d_sin_new(900+(-900)*(cur_frame)/total_frame);
        if(0!=SinData)
        {
            memcpy_location=dst_buff+window_size.w*(window_size.h-window_size.h*SinData/65536);
            copy_size=window_size.w*window_size.h*4-window_size.w*(window_size.h-window_size.h*SinData/65536)*4;
            SCI_MEMCPY(memcpy_location, next_color_buff,copy_size );
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description:  WinSeVerticalInOut
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WinSeVerticalInOut(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr)
{
    IMGREF_SIZE_T ref_size = {0};
    
    if(PNULL == in_param_ptr || PNULL == out_param_ptr|| PNULL == in_param_ptr->multi_layer.fg_buffer_ptr || PNULL == in_param_ptr->multi_layer.src_buffer_ptr)
    {
        return;
    }
    ref_size.w = in_param_ptr->buf_width;
    ref_size.h = in_param_ptr->buf_height;
    Vertical_In_Out_32(in_param_ptr->multi_layer.fg_buffer_ptr,
                            in_param_ptr->multi_layer.src_buffer_ptr,
                            ref_size,
                            in_param_ptr->frame_index,
                            in_param_ptr->frame_count,

                            in_param_ptr->is_back
                            );
}
#endif /*_MMI_WINSE_IMP_C_*/
#endif
