/*****************************************************************************
** File Name:      mmi_winse.h                                                                                                  *
** Author:          arvin.wu                                                                                                       *
** Date:           12/10/2011                                                                                                    *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                                             *
** Description:    This file is used to describe runninglight wallpaer                                                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  10/2011      Arvin.wu            Create                                  *
******************************************************************************/
#ifdef UI_P3D_SUPPORT   
#ifndef _MMI_WINSE_H_
#define _MMI_WINSE_H_
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmitheme_special.h"
#include "mmiset_display.h"
#include "mmiset_text.h"
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _MMI_WIN_SE_SINGLE_LAYER_INFO_
{
    uint16 *pre_buffer_ptr; //传入的buffer
    uint16 *next_buffer_ptr;//传入的buffer
    uint16 *dest_buffer_ptr;//目标buffer
}MMI_WIN_SE_SINGLE_LAYER_INFO;
typedef struct _MMI_WIN_SE_MULTI_LAYER_INFO_
{
    uint32 *fg_buffer_ptr; //前景目标buf
    uint32 *src_buffer_ptr;//前景的源buf
    uint16 *bg_buffer_ptr;//目标buffer,已经含有背景内容
}MMI_WIN_SE_MULTI_LAYER_INFO;
/*pass in param*/
typedef struct _MMI_WIN_SE_PARAM_IN_
{
    MMI_WIN_SE_SINGLE_LAYER_INFO single_layer;//仅用单层时，使用的buffer
    MMI_WIN_SE_MULTI_LAYER_INFO   multi_layer;//两层时，使用的buffer
    
    uint16 buf_width;    //传入buffer的宽度
    uint16 buf_height;   //传入buffer的高度
    uint16 frame_count;//总帧数
    uint16 frame_index;//帧索引
    BOOLEAN is_back;//有些动画有进入，退出表现形式不一样

    void* ext_ptr;//算法需要的格外数据
}MMI_WIN_SE_PARAM_IN;

/*pass out param*/
typedef struct _MMI_WIN_SE_PARAM_OUT_
{
    GUI_RECT_T invalide_rc;//需要更新的区域，默认全屏更新
    BOOLEAN is_set_invalide_rc;//默认全屏更新
    uint32 mask_color;//如果使用两层，通过LAYER 的callback函数在LCD任务中用DMA填充MASK
}MMI_WIN_SE_PARAM_OUT;

/*枚举所有帧，用位来标志哪一帧是否需要播放audio*/
typedef enum
{
    WIN_SE_FRAME_1 = 0x01,//第一帧之前
    WIN_SE_FRAME_2 = (WIN_SE_FRAME_1<<1),
    WIN_SE_FRAME_3 = (WIN_SE_FRAME_1<<2),
    WIN_SE_FRAME_4 = (WIN_SE_FRAME_1<<3),
    WIN_SE_FRAME_5 = (WIN_SE_FRAME_1<<4),
    WIN_SE_FRAME_6 = (WIN_SE_FRAME_1<<5),
    WIN_SE_FRAME_7 = (WIN_SE_FRAME_1<<6),
    WIN_SE_FRAME_8 = (WIN_SE_FRAME_1<<7),
    WIN_SE_FRAME_9 = (WIN_SE_FRAME_1<<8),
    WIN_SE_FRAME_10 = (WIN_SE_FRAME_1<<9),
    WIN_SE_FRAME_11 = (WIN_SE_FRAME_1<<10),
    WIN_SE_FRAME_12 = (WIN_SE_FRAME_1<<11),
    WIN_SE_FRAME_13 = (WIN_SE_FRAME_1<<12),
    WIN_SE_FRAME_14 = (WIN_SE_FRAME_1<<13),
    WIN_SE_FRAME_15 = (WIN_SE_FRAME_1<<14),
    WIN_SE_FRAME_16 = (WIN_SE_FRAME_1<<15),
    WIN_SE_FRAME_END = 0
};


typedef struct _MMI_WIN_SE_IMPLEMENTOR_
{
    /*implementor function*/
    void (*SeCal)(MMI_WIN_SE_PARAM_IN *in_param_ptr,MMI_WIN_SE_PARAM_OUT *out_param_ptr);
    void (*PlayAudio)(uint16 frame_index);
    
    /*private element*/
    uint32 win_se_type;                   //the type of se
    uint32 win_se_set_style;            //设置指定的类型，为了匹配原有的特效。
    uint8 frame_count_total;            //the total frame count of a se
    BOOLEAN is_need_fg;    //是否需要前景层
    uint16 delay_time_per_second;  //delay time per second
    uint16 audio_play_flag;     //
    uint32 winse_name_id;//
    uint32 ext_size;       //算法需要的格外数据大小
}MMI_WIN_SE_IMPLEMENTOR;
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                                                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   获得当前特效执行者
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_SE_IMPLEMENTOR*MMIWINSEIMP_GetSeImplementor(void);

/*****************************************************************************/
//  Description:  特效入口
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWINSE_Play(MMI_WIN_ANIM_PARAM_T *param_ptr);

/*****************************************************************************/
//  Description:   预设当前类型的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWINSEIMP_SetSeImplementorType(uint32 win_se_type);

/*****************************************************************************/
//  Description:   根据当前设置的特效风格，取得相应的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_TYPE_E MMIWINSE_GetAnimTypeBySetingStyle(uint32 seting_style);
/*****************************************************************************/
//  Description:   根据当前设置的特效风格，取得相应的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWINSE_GetNameIdBySetingStyle(uint32 seting_style);
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                                                              */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /*_MMI_LIVE_WALLPAPER_C_*/
#endif


