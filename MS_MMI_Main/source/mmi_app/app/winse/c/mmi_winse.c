#include "mmi_app_winse_trc.h"
#ifdef UI_P3D_SUPPORT   
/*****************************************************************************
** File Name:      mmi_winse.c                                    *
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

#ifndef _MMI_WINSE_C_
#define _MMI_WINSE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h" 
#include "mmi_winse.h"
#include "mmidisplay_data.h"
#include "mmi_default.h"
#include "guilcd.h"
#include "block_mem.h"
#include "mmiset_display.h"
#include "mmi_theme.h"
#include "mmi3d_export.h"
#include "graphics_draw.h"
#include "mmk_tp.h"
#include "mmi_appmsg.h"
#include "mmi_utility.h"
#include "graphics_3d_rotate.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 #ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif

#define WINSE_FREE_IF(p)\
            do{\
                    if(p)\
                    {\
                        SCI_FREE(p);\
                        p = PNULL;\
                    }\
                }while(0)
                
#define WINSE_DEFAULT_FRAME_COUNT (10)
#define WINSE_DELAY_TIME_MAX   50
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
                
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
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
//  Description:  ��ò���ǰ�����ź��source buffer
//  Global resource dependence:
//  Author: Arvin.wu
//   Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSEGetBuffer(uint16 **pre_buffer_pptr,uint16**next_buffer_pptr);
/*****************************************************************************/
//  Description:   ��ʼ����׼����ǰ��buffer�����ݣ��Լ����Ŀ���ַ
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSESEInitialise(LCD_ID_E lcd_id,uint16 **dst_buffer_pptr,uint16*pre_buffer_ptr,uint16* next_buffer_ptr);

/*****************************************************************************/
//  Description:   �����Ч��֡��
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 WINSEGetFrameCount(void);
/*****************************************************************************/
//  Description:   ���㣬�˴���ʼ���ü��㺯,
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSECalculate(MMI_WIN_SE_PARAM_IN *param_in_ptr,MMI_WIN_SE_PARAM_OUT *param_out_ptr);
/*****************************************************************************/
//  Description:   ˢ������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEUpdate(LCD_ID_E lcd_id,GUI_RECT_T *rc_update_ptr,BOOLEAN is_multi_layer);
/*****************************************************************************/
//  Description:   Delay
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEDelay(void);
/*****************************************************************************/
//  Description:   �ж��Ƿ���Ҫ���ŵĺ���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSEIsNeedPlayAudio(uint16 frame_index);
/*****************************************************************************/
//  Description:   �����������ź���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEPlayAudio(uint16 frame_index);
/*****************************************************************************/
//  Description:   �����������ź���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEPlay(MMI_WIN_ANIM_PARAM_T *param_ptr);
/*****************************************************************************/
//  Description:  �ָ�����Ŀ������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEResume(uint16 *dst_buffer_ptr,uint16*next_buffer_ptr);

/*****************************************************************************/
//  Description:  ע������callback����
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void  WINSELayerCallBack(uint8* buffer, 
                                       UILAYER_INFO_T *layer_info_ptr,uint32 param);
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  �����Ļbuffer�Ĵ�С
//  Global resource dependence:
//  Author: Arvin.wu
//   Note:
/*****************************************************************************/
LOCAL uint32 WINSEGetScreenBufSize(void)
{
    uint16 width = 0;
    uint16 height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);

    return (width*height*sizeof(uint16));
}
/*****************************************************************************/
//  Description:  ��ò���ǰ�����ź��source buffer
//  Global resource dependence:
//  Author: Arvin.wu
//   Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSEGetBuffer(uint16 **pre_buffer_pptr,uint16**next_buffer_pptr)
{

    if(PNULL == pre_buffer_pptr ||PNULL == next_buffer_pptr )
    {
        return FALSE;
    }

    MMI_GetTrans3DBuf(pre_buffer_pptr,next_buffer_pptr);

    if(PNULL == *pre_buffer_pptr ||PNULL == *next_buffer_pptr )
    {
        return FALSE;
    }


    return TRUE;
}
/*****************************************************************************/
//  Description:   ��ʼ����׼����ǰ��buffer�����ݣ��Լ����Ŀ���ַ
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSESEInitialise(LCD_ID_E lcd_id,uint16 **dst_buffer_pptr,uint16*pre_buffer_ptr,uint16* next_buffer_ptr)
{
    GUI_RECT_T rc_screen = MMITHEME_GetFullScreenRect();
    uint32  buf_size = WINSEGetScreenBufSize();
    
    if(PNULL == pre_buffer_ptr ||PNULL == next_buffer_ptr || PNULL == dst_buffer_pptr)
    {
        //SCI_TRACE_LOW:"WINSESEInitialise param check failed:0x%x,0x%x,0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WINSE_170_112_2_18_3_9_25_0,(uint8*)"ddd",pre_buffer_ptr,next_buffer_ptr,dst_buffer_pptr);
        return FALSE;
    }

    //ִ��3d��Ч֮ǰ����lcd buf����׼����
    UILAYER_InvalidateRect( lcd_id, &rc_screen );
    
    *dst_buffer_pptr = GUILCD_GetMainLcdBufPtr();

    if(PNULL == *dst_buffer_pptr)
    {
        //SCI_TRACE_LOW:"WINSESEInitialise :dst buffer error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WINSE_181_112_2_18_3_9_25_1,(uint8*)"");
        return FALSE;
    }

    SCI_MEMCPY(next_buffer_ptr, *dst_buffer_pptr, buf_size); //GUILCD_GetLogicWidthHeight�����
    SCI_MEMCPY(*dst_buffer_pptr, pre_buffer_ptr, buf_size);
    return TRUE;
}
/*****************************************************************************/
//  Description:   ��ñ��ʣ����٣�����������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 WINSEGetFrameRate(void)
{
    return 1;//Ԥ���Ժ�������������
}
/*****************************************************************************/
//  Description:   ��ʼ����׼����ǰ��buffer�����ݣ��Լ����Ŀ���ַ
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 WINSEGetFrameCount(void)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;
    uint32 dest_fame_cout = 0;
    
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(se_imp_ptr)
    {
        /*���û����֡�ʣ���Ĭ��*/
        if(0 == se_imp_ptr->frame_count_total)
        {
            dest_fame_cout = WINSE_DEFAULT_FRAME_COUNT;
        }
        else
        {
            dest_fame_cout = se_imp_ptr->frame_count_total;
        }
        
        /*��������ѡ�����*/
        dest_fame_cout*= WINSEGetFrameRate();

        /*�ݴ�����ֹ��֡�ʳ˰ٷֱȵó�0*/
        if( 0 == dest_fame_cout)
        {
            dest_fame_cout = 1;
        }
    }

    return dest_fame_cout;
}

/*****************************************************************************/
//  Description:   ��ö������ݵĴ�С
//  Global resource dependence:
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 WINSEGetExSize(void)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;
    uint32 ex_size = 0;
    
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(se_imp_ptr)
    {
        ex_size = se_imp_ptr->ext_size;
    }

    return ex_size;
}

/*****************************************************************************/
//  Description:   ���㣬�˴���ʼ���ü��㺯,
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSECalculate(MMI_WIN_SE_PARAM_IN *param_in_ptr,MMI_WIN_SE_PARAM_OUT *param_out_ptr)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;

    /*�������*/
    if(PNULL == param_in_ptr || PNULL == param_out_ptr)
    {
        return;
    }
    
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(se_imp_ptr)
    {
        se_imp_ptr->SeCal(param_in_ptr,param_out_ptr);
    }
}
/*****************************************************************************/
//  Description:   ˢ������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEUpdate(LCD_ID_E lcd_id,GUI_RECT_T *rc_update_ptr,BOOLEAN is_multi_layer)
{
    GUI_RECT_T rc_default = {0};
    
    rc_default = MMITHEME_GetFullScreenRect();
    
    /*Ĭ��Ϊȫ��ˢ������*/
    if(PNULL != rc_update_ptr)
    {
        rc_default = *rc_update_ptr;
    }
    if(is_multi_layer)
    {
        GUILCD_InvalidateRect( lcd_id, rc_default, 0 );
    }else
    {
        GUILCD_InvalidateRectOnly( lcd_id, rc_default, 0 );
    }
}
/*****************************************************************************/
//  Description:   Delay
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEDelay(void)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(PNULL == se_imp_ptr)return;
#ifndef WIN32  
    if(0 == se_imp_ptr->delay_time_per_second || WINSE_DELAY_TIME_MAX < se_imp_ptr->delay_time_per_second)
    {
        //SCI_TRACE_LOW:"WINSEDelay error delay time:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WINSE_289_112_2_18_3_9_25_2,(uint8*)"d",se_imp_ptr->delay_time_per_second);
        return;
    }
    SCI_SLEEP(se_imp_ptr->delay_time_per_second);
#else
    SCI_SLEEP(15);
#endif
    
}
/*****************************************************************************/
//  Description:   �����������ź���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSEIsNeedPlayAudio(uint16 frame_index) 
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;
    
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(PNULL == se_imp_ptr)return FALSE;
    
    /*��������һ֡�������ٲ���audio*/
    if(frame_index == (se_imp_ptr->frame_count_total ) && WIN_SE_FRAME_END == se_imp_ptr->audio_play_flag)
    {
        return TRUE;
    }
    /*�ж���һ֡��Ҫ����audio*/
    if((1<<(frame_index)) & se_imp_ptr->audio_play_flag)
    {
        return TRUE;
    }

    return FALSE;
}
/*****************************************************************************/
//  Description:   �����������ź���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEPlayAudio(uint16 frame_index)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = PNULL;

    if(!WINSEIsNeedPlayAudio(frame_index))
    {
        return;
    }
    
    se_imp_ptr = MMIWINSEIMP_GetSeImplementor();

    if(se_imp_ptr && se_imp_ptr->PlayAudio)
    {
        se_imp_ptr->PlayAudio(frame_index);
    }
}

/*****************************************************************************/
//  Description:   ����ǰ��ʹ�õĲ㲢���ӽ���������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32* WINSECreateFgLayer(GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    UILAYER_CREATE_T uilayer_create_info = {0};
    uint32 *fg_buffer = PNULL;
    UILAYER_APPEND_BLT_T  append_blt_info = {0};
    uint16 width = MMI_MAINSCREEN_WIDTH;
    uint16 height = MMI_MAINSCREEN_HEIGHT;
    
    if(lcd_dev_info_ptr)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &width, &height);
        
        uilayer_create_info.owner_handle = MMK_GetFocusWinId();
        uilayer_create_info.is_bg_layer = FALSE;
        uilayer_create_info.format = UILAYER_MEM_FPS_ACCELERATE;
        uilayer_create_info.width =  width;
        uilayer_create_info.height = height;
        uilayer_create_info.offset_x = 0;
        uilayer_create_info.offset_y = 0;
        uilayer_create_info.lcd_id = GUI_MAIN_LCD_ID;
        uilayer_create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer((const UILAYER_CREATE_T *)&uilayer_create_info, lcd_dev_info_ptr);

        if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
        {
            fg_buffer = (uint32*)UILAYER_GetLayerBufferPtr(lcd_dev_info_ptr);
            append_blt_info.layer_level = UILAYER_LEVEL_NORMAL;
            append_blt_info.lcd_dev_info = *lcd_dev_info_ptr;
            UILAYER_AppendBltLayer(&append_blt_info);//��setbltlayer�滻
        }
        
    }

    return fg_buffer;
}

/*****************************************************************************/
//  Description:   ����ǰ����
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEDestroyFgLayer(GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{  
    if(lcd_dev_info_ptr)
    {
        UILAYER_ReleaseLayer((GUI_LCD_DEV_INFO const *)lcd_dev_info_ptr);
        lcd_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
        lcd_dev_info_ptr->lcd_id = 0;
    }
}
/*****************************************************************************/
//  Description:   �ж��Ƿ���Ҫʹ��ǰ������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINSEIsNeedFgBuffer(void)
{
    MMI_WIN_SE_IMPLEMENTOR *se_imp_ptr = MMIWINSEIMP_GetSeImplementor();


    if(se_imp_ptr)
    {
        return se_imp_ptr->is_need_fg;
    }

    return FALSE;
}
/*****************************************************************************/
//     Description : ��565��buf����888�Ĳ���
//    Global resource dependence : 
//  Author:   Arvin.wu
//    Note:
/*****************************************************************************/
LOCAL void WINSE565bufto888buf(uint16* dst_buf_ptr,
										uint32 *src_buf_ptr,
										uint32 width,
										uint32 height)
{
    if ((PNULL == dst_buf_ptr) || (PNULL == src_buf_ptr) || (0 == width) || (0 == height))
    {
        return;
    }
    
    S2d_Blend_RGB565ToARGB888((uint16*)dst_buf_ptr,(uint32*)src_buf_ptr,width,height,0xFF);
}
/*****************************************************************************/
//  Description:   �����������ź����������ȫ������WINSE��˽�к���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEPlay(MMI_WIN_ANIM_PARAM_T *param_ptr)
{
    uint16 *pre_buffer_ptr = PNULL;
    uint16 *next_buffer_ptr = PNULL;
    uint16 *dst_buffer_ptr = PNULL;
    uint32 i = 0;
    uint32 frame_max = 0;
    MMI_WIN_SE_PARAM_IN win_se_cal_param_in = {0};
    MMI_WIN_SE_PARAM_OUT win_se_cal_param_out = {0};
    uint32 tick_keep = 0;
    GUI_RECT_T *invalide_rc_ptr = PNULL;
    GUI_LCD_DEV_INFO   fg_lcd_dev_info = {0};
    UILAYER_APPEND_BLT_T append_array[UILAYER_TOTAL_BLT_COUNT] = {0};
    UILAYER_APPEND_BLT_T append_array_temp[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32 layer_count = 0;
    uint16 *temp_buf = PNULL;
    uint16 *src_fg_buf = PNULL;
    BOOLEAN is_need_multi_layer = FALSE;
    uint32  buf_size = WINSEGetScreenBufSize();
    uint32 array_size = 0;
    uint32 ex_size = 0;

    /*��ȡǰ����֡buffer*/
    if(!WINSEGetBuffer(&pre_buffer_ptr,&next_buffer_ptr))
    {
        return;
    }

    /*��ʼ������:����׼������֡���ݣ��Լ�׼��old֡����*/
    if(!WINSESEInitialise(param_ptr->lcd_id,&dst_buffer_ptr,pre_buffer_ptr,next_buffer_ptr))
    {
        return;
    }
    
    /*
        ��õ�ǰ��Ч��Ҫ��֡��������ÿ����Ч֡�����Բ�һ��
        Ԥ��һ������֡�ʵĽӿڣ�ÿ����Ч��֡�ʿ����ã�����
        Ĭ��֡�İٷֱ�������
    */
    frame_max = WINSEGetFrameCount();

    /*׼�����*/
    win_se_cal_param_in.frame_count = frame_max;
    win_se_cal_param_in.is_back = param_ptr->is_back;
    GUILCD_GetLogicWidthHeight( param_ptr->lcd_id, &win_se_cal_param_in.buf_width,&win_se_cal_param_in.buf_height);

    is_need_multi_layer = WINSEIsNeedFgBuffer();
    /*�ж��Ƿ���Ҫǰ�󱳾���*/
    if(is_need_multi_layer)
    {
        UILAYER_INFO_T layer_info = {0};
        
        //����Ƕ��ת�����򱸷�֮ǰ�ĵ�������
        layer_count = UILAYER_GetBltLayer(append_array,UILAYER_TOTAL_BLT_COUNT);

        //׼��ǰ��buffer
        win_se_cal_param_in.multi_layer.fg_buffer_ptr = WINSECreateFgLayer(&fg_lcd_dev_info);
        win_se_cal_param_in.multi_layer.bg_buffer_ptr = (uint16*)UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev());//ʹ��������Ϊ������
        
        UILAYER_GetLayerInfo(MMITHEME_GetDefaultLcdDev(),&layer_info);
        temp_buf = SCI_ALLOCA((layer_info.layer_width*layer_info.layer_height)*sizeof(uint16));//����buffer�洢�������ݣ��Ա�ָ�150K
        src_fg_buf = SCI_ALLOCA((layer_info.layer_width*layer_info.layer_height)*sizeof(uint32));//����buffer��ΪԴ����300K
        if(PNULL == temp_buf || PNULL == src_fg_buf)
        {
            /*���û����ʱbuffer�Ͳ���ת������*/
            WINSEDestroyFgLayer(&fg_lcd_dev_info);
            WINSE_FREE_IF(temp_buf);
            WINSE_FREE_IF(src_fg_buf);
            return;
        }
        append_array_temp[array_size].lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
        append_array_temp[array_size].layer_level= UILAYER_LEVEL_NORMAL;
        array_size++;
        append_array_temp[array_size].lcd_dev_info = fg_lcd_dev_info;
        append_array_temp[array_size].layer_level= UILAYER_LEVEL_NORMAL;        
        UILAYER_SetBltLayer(append_array_temp,array_size+1);
        
        //��ֵԴ���ݴ���ARGB888
        win_se_cal_param_in.multi_layer.src_buffer_ptr= (uint32*)src_fg_buf;
        if(param_ptr->is_back)
        {
            //���浱ǰ����block����������
            SCI_MEMCPY(temp_buf, win_se_cal_param_in.multi_layer.bg_buffer_ptr,buf_size);
            
            //����һ֡buf������
            SCI_MEMCPY(win_se_cal_param_in.multi_layer.bg_buffer_ptr, pre_buffer_ptr,buf_size);
            WINSE565bufto888buf(  (uint16*)next_buffer_ptr,
                                                (uint32*)win_se_cal_param_in.multi_layer.src_buffer_ptr,
                                                win_se_cal_param_in.buf_width,
                                                win_se_cal_param_in.buf_height
                                                );
            
        }
        else
        {
            //���浱ǰ����block����������
            SCI_MEMCPY(temp_buf, win_se_cal_param_in.multi_layer.bg_buffer_ptr,buf_size);
            
            //��ǰһ֡buf������
            SCI_MEMCPY(win_se_cal_param_in.multi_layer.bg_buffer_ptr, next_buffer_ptr,buf_size);
            /*ת��ǰ��bufer */
            WINSE565bufto888buf( (uint16*)pre_buffer_ptr,
                                                (uint32*)win_se_cal_param_in.multi_layer.src_buffer_ptr,
                                                win_se_cal_param_in.buf_width,
                                                win_se_cal_param_in.buf_height
                                                );
        }
    }
    else
    {
        /*׼�����*/
        win_se_cal_param_in.single_layer.pre_buffer_ptr = pre_buffer_ptr;
        win_se_cal_param_in.single_layer.next_buffer_ptr = next_buffer_ptr;
        win_se_cal_param_in.single_layer.dest_buffer_ptr = dst_buffer_ptr;
    }
    tick_keep = SCI_GetTickCount();

    ex_size = WINSEGetExSize();
    if ( ex_size > 0 )
    {
        win_se_cal_param_in.ext_ptr = SCI_ALLOCA( ex_size );

         if ( PNULL != win_se_cal_param_in.ext_ptr )
         {
             SCI_MEMSET( win_se_cal_param_in.ext_ptr, 0, ex_size );
         }
    }

    /*���������Ӵ˿�ʼ*/
    for(i = 0; i<frame_max; i++)
    {
        /*��ʼ��������*/
        WINSEPlayAudio((uint16)(i+1));

        /*����֡����*/
        win_se_cal_param_in.frame_index = i;
        win_se_cal_param_out.is_set_invalide_rc = FALSE;

        /*����buffer*/
        UILAYER_SwitchDoubleMem(&fg_lcd_dev_info,TRUE);
        /*����µ�BUFFER*/
        win_se_cal_param_in.multi_layer.fg_buffer_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&fg_lcd_dev_info);
        
        /*��Ч���㣬���Ǹ�ǰ��֡��֡�������ó�Ŀ��֡*/
        WINSECalculate(&win_se_cal_param_in,&win_se_cal_param_out);
        
        /*ˢ��*/
        if(win_se_cal_param_out.is_set_invalide_rc)
        {
            invalide_rc_ptr = &win_se_cal_param_out.invalide_rc;
        }
        
        if(is_need_multi_layer)
        {
            /*���������HOOK��������Ҫ�ڴ�ע��һ��*/
            UILAYER_SetBlendCallback((GUI_LCD_DEV_INFO const *) &fg_lcd_dev_info, (UILAYER_BLEND_CALLBACK) WINSELayerCallBack,win_se_cal_param_out.mask_color);
            UILAYER_SetDirectDraw(TRUE);
        }
        WINSEUpdate(GUI_MAIN_LCD_ID,invalide_rc_ptr,is_need_multi_layer);

        /*��ճ��Σ��Ա��´�ʹ��*/
        SCI_MEMSET(&win_se_cal_param_out,0,sizeof(win_se_cal_param_out));
        invalide_rc_ptr = PNULL;
        
        /*�ӳ�*/
        WINSEDelay();
    }

    if ( PNULL != win_se_cal_param_in.ext_ptr )
    {
        SCI_FREE( win_se_cal_param_in.ext_ptr );
    }

    //SCI_TRACE_LOW:"PlayWinSe cost Time:%d ms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WINSE_588_112_2_18_3_9_26_3,(uint8*)"d",(SCI_GetTickCount() - tick_keep));
    tick_keep = SCI_GetTickCount();
        
    /*����ǰ��������*/
    if(is_need_multi_layer && temp_buf)
    {
        SCI_MEMCPY(win_se_cal_param_in.multi_layer.bg_buffer_ptr, temp_buf,buf_size);
        WINSE_FREE_IF(temp_buf);
        WINSE_FREE_IF(src_fg_buf);
        //���ת����ɣ��ָ���������
        UILAYER_SetBltLayer(append_array,layer_count);
        WINSEDestroyFgLayer(&fg_lcd_dev_info);
    }
    /*��������*/
    WINSEPlayAudio((uint16)frame_max);
    /*�����ˣ��ָ���������*/
    WINSEResume(dst_buffer_ptr,next_buffer_ptr);
    /*ˢ��*/
    WINSEUpdate(GUI_MAIN_LCD_ID,PNULL,FALSE);
}
/*****************************************************************************/
//  Description:  �ָ�����Ŀ������
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINSEResume(uint16 *dst_buffer_ptr,uint16*next_buffer_ptr)
{
    GUI_LCD_DEV_INFO main_lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T update_rect = {0};
    uint32  buf_size = WINSEGetScreenBufSize();
    
    if(PNULL != dst_buffer_ptr && PNULL != next_buffer_ptr)
    {
        SCI_MEMCPY(dst_buffer_ptr, next_buffer_ptr, buf_size);
        
        update_rect = MMITHEME_GetFullScreenRect();
        MMITHEME_StoreUpdateRect(&main_lcd_dev_info, update_rect);
    }
}      
/*****************************************************************************/
//  Description:  ע������callback����
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void  WINSELayerCallBack(uint8* buffer, 
                                       UILAYER_INFO_T *layer_info_ptr,uint32 param)
{
    uint32 count = 0;
    
    if(PNULL == buffer || PNULL == layer_info_ptr)
    {
        return;
    }
    count = layer_info_ptr->layer_width*layer_info_ptr->layer_height;
    
    switch(layer_info_ptr->data_type)
    {
        case DATA_TYPE_ARGB888:
            {
                MMI_MEM32SET((uint32*)buffer,(uint32)param,count*sizeof(uint32));
            }
            break;
        case DATA_TYPE_RGB565:
            SCI_MEM16SET((uint16*)buffer,(uint16)param,count*sizeof(uint16));
            break;
        default:
            break;
    }
}
/*****************************************************************************/
//  Description:  ��Ч���
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWINSE_Play(MMI_WIN_ANIM_PARAM_T *param_ptr)
{
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    if(!MMIWINSEIMP_SetSeImplementorType(param_ptr->type))
    {
        return;
    }
    WINSEPlay(param_ptr);
}
#endif /*_MMI_WINSE_C_*/
#endif
