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
    uint16 *pre_buffer_ptr; //�����buffer
    uint16 *next_buffer_ptr;//�����buffer
    uint16 *dest_buffer_ptr;//Ŀ��buffer
}MMI_WIN_SE_SINGLE_LAYER_INFO;
typedef struct _MMI_WIN_SE_MULTI_LAYER_INFO_
{
    uint32 *fg_buffer_ptr; //ǰ��Ŀ��buf
    uint32 *src_buffer_ptr;//ǰ����Դbuf
    uint16 *bg_buffer_ptr;//Ŀ��buffer,�Ѿ����б�������
}MMI_WIN_SE_MULTI_LAYER_INFO;
/*pass in param*/
typedef struct _MMI_WIN_SE_PARAM_IN_
{
    MMI_WIN_SE_SINGLE_LAYER_INFO single_layer;//���õ���ʱ��ʹ�õ�buffer
    MMI_WIN_SE_MULTI_LAYER_INFO   multi_layer;//����ʱ��ʹ�õ�buffer
    
    uint16 buf_width;    //����buffer�Ŀ��
    uint16 buf_height;   //����buffer�ĸ߶�
    uint16 frame_count;//��֡��
    uint16 frame_index;//֡����
    BOOLEAN is_back;//��Щ�����н��룬�˳�������ʽ��һ��

    void* ext_ptr;//�㷨��Ҫ�ĸ�������
}MMI_WIN_SE_PARAM_IN;

/*pass out param*/
typedef struct _MMI_WIN_SE_PARAM_OUT_
{
    GUI_RECT_T invalide_rc;//��Ҫ���µ�����Ĭ��ȫ������
    BOOLEAN is_set_invalide_rc;//Ĭ��ȫ������
    uint32 mask_color;//���ʹ�����㣬ͨ��LAYER ��callback������LCD��������DMA���MASK
}MMI_WIN_SE_PARAM_OUT;

/*ö������֡����λ����־��һ֡�Ƿ���Ҫ����audio*/
typedef enum
{
    WIN_SE_FRAME_1 = 0x01,//��һ֮֡ǰ
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
    uint32 win_se_set_style;            //����ָ�������ͣ�Ϊ��ƥ��ԭ�е���Ч��
    uint8 frame_count_total;            //the total frame count of a se
    BOOLEAN is_need_fg;    //�Ƿ���Ҫǰ����
    uint16 delay_time_per_second;  //delay time per second
    uint16 audio_play_flag;     //
    uint32 winse_name_id;//
    uint32 ext_size;       //�㷨��Ҫ�ĸ������ݴ�С
}MMI_WIN_SE_IMPLEMENTOR;
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                                                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   ��õ�ǰ��Чִ����
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_SE_IMPLEMENTOR*MMIWINSEIMP_GetSeImplementor(void);

/*****************************************************************************/
//  Description:  ��Ч���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWINSE_Play(MMI_WIN_ANIM_PARAM_T *param_ptr);

/*****************************************************************************/
//  Description:   Ԥ�赱ǰ���͵���Ч
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWINSEIMP_SetSeImplementorType(uint32 win_se_type);

/*****************************************************************************/
//  Description:   ���ݵ�ǰ���õ���Ч���ȡ����Ӧ����Ч
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_TYPE_E MMIWINSE_GetAnimTypeBySetingStyle(uint32 seting_style);
/*****************************************************************************/
//  Description:   ���ݵ�ǰ���õ���Ч���ȡ����Ӧ����Ч
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


