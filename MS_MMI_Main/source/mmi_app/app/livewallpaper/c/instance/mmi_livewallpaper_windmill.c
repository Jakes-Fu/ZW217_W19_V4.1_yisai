/*****************************************************************************
** File Name:      mmi_livewallpaper_WINDMILL.c                                    *
** Author:         Arvin.wu                                                       *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_WINDMILL_C_
#define _MMI_LIVEWALLPAPER_WINDMILL_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
#include "mic_sample.h"
#include "audio_config.h"
#include "graphics_3d_rotate.h"
#include "mmimp3_export.h"
//#include "mmu_drvapi.h"
#include "Mmisrvaud_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*
#define DMA_DONE_EVENT_TIMEOUT   0x32 //50ms
#define DMA_DONE_EVENT    (1<<0)



#define WINDMILL_SUN_NUM  2
#define WINDMILL_WING_NORMAL_NUM  5
#define WINDMILL_WING_ROTATION_NUM  2
#define WINDMILL_WING_NUM  (WINDMILL_WING_NORMAL_NUM + WINDMILL_WING_ROTATION_NUM)
#define WINDMILL_CENTER_NUM  4
*/
#define DMA_DONE_EVENT_TIMEOUT   0x32 //50ms
#define DMA_DONE_EVENT    (1<<0)
#define WINDMILL_OFFSET_X  2
#define WINDMILL_OFFSET_Y  2
#define WINDMILL_SUN_NUM  2
#if defined(MAINLCD_SIZE_240X400)
#define WINDMILL_VERTICAL_OFFSET_BIG  25
#define WINDMILL_VERTICAL_OFFSET_SMALL  0
#elif defined(MAINLCD_SIZE_240X320)
#define WINDMILL_VERTICAL_OFFSET_BIG  25
#define WINDMILL_VERTICAL_OFFSET_SMALL  0
#elif defined(MAINLCD_SIZE_320X480)
#define WINDMILL_VERTICAL_OFFSET_BIG  70
#define WINDMILL_VERTICAL_OFFSET_SMALL  45
#else
#define WINDMILL_VERTICAL_OFFSET_BIG  0
#define WINDMILL_VERTICAL_OFFSET_SMALL  0
#endif

#define WINDMILL_WING_NORMAL_NUM  5
#define WINDMILL_WING_ROTATION_NUM  2
#define WINDMILL_WING_NUM  (WINDMILL_WING_NORMAL_NUM + WINDMILL_WING_ROTATION_NUM)
#define WINDMILL_CENTER_NUM  4
#define EMPTY_PIXEL  4
#define WIN_GRASS_ANGLE (400)
#define WINDMILL_MOVE_Z_PARAM 40
#define WING_SPEED_MAX (120)
#define WING_SPEED_NEAR_MAX (100)
#define CUT_START (100)
#define CUT_END (700)
#define WINDMILL_ZERO_SIZE (2)
#define BLOCK_MEM_POOL_SIZE_WINDMILL (660 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct
{
	MMI_IMAGE_ID_T theme_image_id;
	IMGREF_POINT_T theme_offset_relative;
	IMGREF_POINT_T theme_offset_actual;	
	IMGREF_SIZE_T  theme_size;
	IMGREF_POINT_T theme_mid;
	void *theme_image_buf_ptr;
}WINDMILL_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL WINDMILL_INFO_T s_windmill_sun_info[WINDMILL_SUN_NUM] = {
{IMAGE_WINDMILL_SUN_01,{165*256/320, 107*256/480}, {233  , 95}, {132 ,  18 }, {132 ,  9}, PNULL},
{IMAGE_WINDMILL_SUN_02,{165*256/320, 107*256/480}, {233  , 95}, {148 ,  150}, {75  , 83}, PNULL}
};

LOCAL WINDMILL_INFO_T s_windmill_wing_info[WINDMILL_WING_NUM] = {
/*size.x size.y mid.x mid.y offset.x offset.y  img_id*/
{IMAGE_WINDMILL_TRI_WING_01, {122*256/320, 301*256/480}, {122  , 301}, {32 , 34 }, {16 ,17 }, PNULL },
{IMAGE_WINDMILL_TRI_WING_02, {94 *256/320, 298*256/480}, {94   , 298}, {38 , 42 }, {19 ,21 }, PNULL },
{IMAGE_WINDMILL_TRI_WING_03, {54 *256/320, 293*256/480}, {54   , 293}, {44 , 50 }, {22 ,25 }, PNULL},
{IMAGE_WINDMILL_TRI_WING_04, {293*256/320, 293*256/480}, {293  , 293}, {44 , 50 }, {22 ,25 }, PNULL },
{IMAGE_WINDMILL_TRI_WING_05, {261*256/320, 277*256/480}, {261  , 277}, {70 , 80 }, {35 ,40 }, PNULL },
{IMAGE_WINDMILL_WING_06    , {214*256/320, 250*256/480}, {214  , 250}, {17 , 57 }, {9  ,57 }, PNULL },
{IMAGE_WINDMILL_WING_07    , {149*256/320, 204*256/480}, {149  , 204}, {20 , 80 }, {10 ,80 }, PNULL }
};

LOCAL WINDMILL_INFO_T s_windmill_center_info[WINDMILL_CENTER_NUM] = {
{IMAGE_WINDMILL_CENTER_01, {292*256/320 , 291*256/480}, {292  , 291}, {2 , 3 }, {1 ,1 }, PNULL},
{IMAGE_WINDMILL_CENTER_02, {259*256/320 , 274*256/480}, {259  , 274}, {3 , 4 }, {1 ,2 }, PNULL},
{IMAGE_WINDMILL_CENTER_03, {211*256/320 , 247*256/480}, {211  , 247}, {6 , 6 }, {3 ,3 }, PNULL},
{IMAGE_WINDMILL_CENTER_04, {146*256/320 , 200*256/480}, {146  , 200}, {7 , 7 }, {4 ,4 }, PNULL}
};                                                                                     
LOCAL WINDMILL_INFO_T s_windmill_bar_info[WINDMILL_WING_NUM] = {

{IMAGE_WINDMILL_BAR_01, {(103)*256/240, (135+WINDMILL_VERTICAL_OFFSET_BIG)*256/320}, {131, 135}, {11 , 98 }, {6 ,49 }, PNULL },
{IMAGE_WINDMILL_BAR_02, {(155) *256/240, (148+WINDMILL_VERTICAL_OFFSET_BIG)*256/320}, {147, 164}, {8 , 63 }, {4 ,32 }, PNULL },
{IMAGE_WINDMILL_BAR_03, {(185) *256/240, (157+WINDMILL_VERTICAL_OFFSET_BIG)*256/320}, {157, 183}, {6 , 41 }, {3 ,21 }, PNULL},
{IMAGE_WINDMILL_BAR_04, {(205)*256/240, (161+WINDMILL_VERTICAL_OFFSET_BIG)*256/320}, {162,193}, {4 , 27 }, {2 ,14 }, PNULL },
{IMAGE_WINDMILL_BAR_05, {(28)*256/240, (249+WINDMILL_VERTICAL_OFFSET_SMALL)*256/400}, {38,193}, {4 , 23 }, {2 ,12 }, PNULL },
{IMAGE_WINDMILL_BAR_06, {61*256/240, (247+WINDMILL_VERTICAL_OFFSET_SMALL)*256/400}, {69,196}, {3 , 25 }, {2  ,13 }, PNULL },
{IMAGE_WINDMILL_BAR_07, {89*256/240, (246+WINDMILL_VERTICAL_OFFSET_SMALL)*256/400}, {90,200}, {3 , 20 }, {2 ,10 }, PNULL }
};
LOCAL WINDMILL_INFO_T s_windmill_bottom_info[WINDMILL_WING_NUM] = {

{IMAGE_WINDMILL_BOTTOM_01, {(114)*256/240, 131*256/320}, {131, 135}, {11 , 98 }, {6 ,49 }, PNULL },
{IMAGE_WINDMILL_BOTTOM_02, {(154) *256/240, 164*256/320}, {147, 164}, {8 , 63 }, {4 ,32 }, PNULL },
{IMAGE_WINDMILL_BOTTOM_03, {(190) *256/240, 183*256/320}, {157, 183}, {6 , 41 }, {3 ,21 }, PNULL},
{IMAGE_WINDMILL_BOTTOM_04, {(218)*256/240, 193*256/320}, {162,193}, {4 , 27 }, {2 ,14 }, PNULL },
{IMAGE_WINDMILL_BOTTOM_05, {34*256/240, 173*256/320}, {38,193}, {4 , 23 }, {2 ,12 }, PNULL },
{IMAGE_WINDMILL_BOTTOM_06, {64*256/240, 174*256/320}, {69,196}, {3 , 25 }, {2  ,13 }, PNULL },
{IMAGE_WINDMILL_BOTTOM_07, {89*256/240, 175*256/320}, {90,200}, {3 , 20 }, {2 ,10 }, PNULL }
};

LOCAL COLOR16 * s_clean_grass_ptr=PNULL;
//LOCAL WINDMILL_INFO_T s_windmill_grass_info={IMAGE_WINDMILL_GRASS, {320*256/320 , 161*256/480}, {320  , 161}, {7 , 7 }, {4 ,4 }, PNULL};
LOCAL WINDMILL_INFO_T s_windmill_grass_info_jpg={IMAGE_WINDMILL_GRASS_JPG, {320*256/320 , 161*256/480}, {320  , 161}, {7 , 7 }, {4 ,4 }, PNULL};
//LOCAL WINDMILL_INFO_T s_windmill_sky_info={IMAGE_WINDMILL_SKY, {320*256/320 , 330*256/480}, {720  , 320}, {720 , 320 }, {4 ,4 }, PNULL};
LOCAL GUI_LCD_DEV_INFO s_windmill_bar_layer_handle[WINDMILL_WING_NUM] = {0} ;
LOCAL GUI_LCD_DEV_INFO s_windmill_bottom_layer_handle[WINDMILL_WING_NUM] = {0} ;

LOCAL GUI_LCD_DEV_INFO s_windmill_grass_layer_handle_jpg = {0};
LOCAL int s_new_offset[WINDMILL_WING_NUM]={0};
LOCAL int s_offset_y[WINDMILL_WING_NUM]={0};
LOCAL int s_eye_speed=2;
LOCAL int s_total_frame=0;

LOCAL IMGREF_3D_POINT_T s_fix_eye={0};
LOCAL int s_step_num=120;                                                                                       
LOCAL GUI_LCD_DEV_INFO s_windmill_sun_layer_handle[WINDMILL_SUN_NUM]= {0} ;
LOCAL GUI_LCD_DEV_INFO s_windmill_wing_layer_handle[WINDMILL_WING_NUM] = {0} ;
LOCAL GUI_LCD_DEV_INFO s_windmill_center_layer_handle[WINDMILL_CENTER_NUM] = {0} ;

LOCAL int s_wing_speed = 10;
LOCAL int s_wing_t = 0;
LOCAL SCI_EVENT_GROUP_PTR  s_dma_done_event = PNULL;
LOCAL BOOLEAN  s_is_mic_sample_enb = FALSE;
LOCAL FIX16_POINT_T * s_g_point_ptr=PNULL;
LOCAL int32 s_moved_z_axis = {0};

LOCAL int16 g_windmill_last_x = 0;

MMISRV_HANDLE_T     audio_srv_handle = 0;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void WINDMILLDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL BOOLEAN Create_Shadow_To_Grass(WINDMILL_INFO_T *grass_info,
									WINDMILL_INFO_T *origin_image,
									WINDMILL_INFO_T *wing_info,
									WINDMILL_INFO_T *center_info,
									IMGREF_POINT_T *start_point,
									int angle,
									int wing_num,
									FIX16_POINT_T * Point_Add_ptr
									);
LOCAL void Make_Rotate_Grass(COLOR16 * tempColor, int grass_offset,int y_offset,int z_offset,IMGREF_3D_POINT_T new_eye,IMGREF_SIZE_T win_size, WINDMILL_INFO_T grass_img);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 WINDMILLGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *WINDMILLGetOptionList(void);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_windmill_Instance =
{
    WINDMILLInit,
    WINDMILLConstruct,
    WINDMILLDrawBg,
    WINDMILLCalculate,
    WINDMILLDestruct,
    WINDMILLHandleEvent,
    PNULL,
    WINDMILLGetType,
    WINDMILLGetThumbNailInfo,
    WINDMILLGetWidthAndHeight,
    PNULL,
    PNULL,
    WINDMILLGetOptionList
};


LOCAL BOOLEAN Windmill_CreateImageLayer(WINDMILL_INFO_T *image_info, uint32 rotation_mode, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_HANDLE_T win_id)
{
    UILAYER_INFO_T   layer_info = {0};
    UILAYER_CREATE_T create_info = {0}; 
    GUI_POINT_T      display_point       = {0,0};
	uint8 *layer_buf_ptr = PNULL;
    uint32 i = 0;

    /*先创建一个层*/
    layer_handle_ptr->lcd_id = 0;
    layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = (MMI_HANDLE_T)win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, image_info->theme_image_id, win_id);
	if(image_info->theme_image_id == IMAGE_WINDMILL_GRASS_JPG)
	{
		create_info.is_bg_layer = TRUE;
	}
	else
	{
		create_info.is_bg_layer = FALSE;
	}
    create_info.is_static_layer = TRUE; 
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(
    					&create_info,
    					layer_handle_ptr
    					))
    {
        // 设置color key
        UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(layer_handle_ptr);

        //把图片画到层上
        GUIRES_DisplayImg(&display_point,
        					PNULL,
        					PNULL,
        					win_id,
        					image_info->theme_image_id,
        					(const GUI_LCD_DEV_INFO*)layer_handle_ptr);
        //获取图片的信息
        UILAYER_GetLayerInfo(layer_handle_ptr, &layer_info);
		layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_handle_ptr);
	    if(PNULL != layer_buf_ptr)
	    {
			if(image_info->theme_image_id == IMAGE_WINDMILL_GRASS_JPG)
			{
				
			}
			else
			{
		        //加速
	            for (i=0; i<layer_info.layer_width*layer_info.layer_height; i++)
	            {
	                if(*((uint32*)(layer_buf_ptr)+i) < 0x08000000)
	                {
	                    *((uint32*)(layer_buf_ptr)+i) = 0x00000000;
	                }
	            }
			}

            image_info->theme_image_buf_ptr = layer_buf_ptr;
            image_info->theme_size.w = layer_info.layer_width;
            image_info->theme_size.h = layer_info.layer_height;
            if (2 == rotation_mode)
            {
                image_info->theme_mid.x = image_info->theme_size.w;
                image_info->theme_mid.y = image_info->theme_size.h/2+image_info->theme_size.w%2;
            }
            else if (1 == rotation_mode)
            {
                image_info->theme_mid.x = image_info->theme_size.w/2+image_info->theme_size.w%2;
                image_info->theme_mid.y = image_info->theme_size.h;
            }
            else
            {
                image_info->theme_mid.x = image_info->theme_size.w/2+image_info->theme_size.w%2;
                image_info->theme_mid.y = image_info->theme_size.h/2+image_info->theme_size.w%2;
            }
            image_info->theme_offset_actual.x = (int16)(((int32)(image_info->theme_offset_relative.x)*MMI_MAINSCREEN_WIDTH)>>8);
            image_info->theme_offset_actual.y = (int16)(((int32)(image_info->theme_offset_relative.y)*MMI_MAINSCREEN_HEIGHT)>>8);
            return TRUE;
        }
    }

    return FALSE;
}

LOCAL void  Windmill_PointProc(int16 cur_db, int16 pre_db)
{
    if (s_wing_speed < WING_SPEED_NEAR_MAX)
    {
        s_wing_speed += (cur_db*cur_db/s_wing_speed)>>3;
    }
    if(s_wing_speed > WING_SPEED_MAX)
    {
        s_wing_speed = WING_SPEED_MAX;
    }
	s_wing_t = 0;
}


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLInit(void)
{	
    BOOLEAN b_result = TRUE;
	s_fix_eye.x=MMI_MAINSCREEN_WIDTH/2;
	s_fix_eye.y=MMI_MAINSCREEN_HEIGHT/2;
	s_fix_eye.z=-MMI_MAINSCREEN_HEIGHT;
    return b_result;
}

//*********************************************************************************
//  Description:    This function get Mic Sample dB
//  Author:         hai.li
//  Note:
//********************************************************************************/
LOCAL int16 WINDMILLGetMic_dB(uint8 sampling_type)
{
    int16 mic_db = 0;
    //SCI_TRACE_LOW:"WINDMILLGetMic_dB, start sampling_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_365_112_2_18_2_32_35_75,(uint8*)"d",sampling_type);
#ifndef WIN32
#ifdef MIC_SAMPLE_SUPPORT
    mic_db = AUD_GetMic_dB(sampling_type);
#endif
#endif
    //SCI_TRACE_LOW:"WINDMILLGetMic_dB, s_is_mic_sample_enb = %d,mic_db = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_371_112_2_18_2_32_35_76,(uint8*)"dd", s_is_mic_sample_enb, mic_db);
    return mic_db;
}

//*********************************************************************************
//  Description:    This function control MicSample.
//  Author:         hai.li
//  Note:           Default mic samplerate is 16k, you can set samplerate
//********************************************************************************/
LOCAL void WINDMILLEnableMicSample( BOOLEAN is_enable )                                
{   
#ifndef WIN32
#ifdef MIC_SAMPLE_SUPPORT
    if(AUDIO_NO_ERROR == AUD_EnableMicSample(is_enable))
    {
        s_is_mic_sample_enb = is_enable;
    }
#endif
#endif
    //SCI_TRACE_LOW:"WINDMILLEnableMicSample, s_is_mic_sample_enb = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_388_112_2_18_2_32_35_77,(uint8*)"d", s_is_mic_sample_enb);
}
/*****************************************************************************/
//  Description : mic sample srv play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLMicCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)   
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    
    if(handle == audio_srv_handle 
        && PNULL != param)
    {
        //SCI_TRACE_LOW:"[MMIWINDMILL]:WINDMILLMicCallBack event=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_401_112_2_18_2_32_35_78,(uint8*)"d",param->event);
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            WINDMILLEnableMicSample(FALSE);
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            WINDMILLEnableMicSample(TRUE);
            break;
        case MMISRVMGR_NOTIFY_EXT:
            break;
        default:
            break;
        }
    }
    return TRUE;

}


/*****************************************************************************/
//  Description : Get audio srv handle
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLRequestVirtualHandle(void)
{
    BOOLEAN result = FALSE;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    //SCI_TRACE_LOW:"[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle  entry audio_srv_handle =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_430_112_2_18_2_32_35_79,(uint8*)"d", audio_srv_handle);
    
    if(0 == audio_srv_handle)
    {
        req.pri = MMISRVAUD_PRI_NORMAL_N10;
        req.notify = WINDMILLMicCallBack;
        audio_srv.volume =  2;
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;        
        audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
        audio_srv_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        //SCI_TRACE_LOW:"[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_440_112_2_18_2_32_36_80,(uint8*)"d",audio_srv_handle);
    }
    
    if(audio_srv_handle > 0)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIWINDMILL]:WINDMILLRequestVirtualHandle result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_452_112_2_18_2_32_36_81,(uint8*)"d",result);
    return result;
    
}

/*****************************************************************************/
//  Description : Release audio srv handle
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void WINDMILLReleaseVirtualHandle(void)
{
    if(0 != audio_srv_handle)
    {
        MMISRVMGR_Free(audio_srv_handle);
        audio_srv_handle = 0;
    }
}

/*****************************************************************************/
//  Description : 风车草地初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLConstruct(MMI_WIN_ID_T win_id)
{

    uint32 i = 0,j=0;
	uint16 swap_temp=0;
	uint16 *dst_ptr=PNULL;
	//uint16 *horinzen_buf_ptr=PNULL;
	uint16 *end_ptr=PNULL;
	uint16 *fill_ptr=PNULL;
	BOOLEAN result = FALSE;
	int32 count = 0;
	
	//SCI_TRACE_LOW:"[LIVEWALLPAPER] WINDMILLConstruct 1"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_486_112_2_18_2_32_36_82,(uint8*)"");
	result = Windmill_CreateImageLayer(&s_windmill_sun_info[0], 2, &s_windmill_sun_layer_handle[0], win_id);
	if(!result)
	{
		goto err;
	}
	else
	{
		count++;
	}
	
	result = Windmill_CreateImageLayer(&s_windmill_sun_info[1], 0, &s_windmill_sun_layer_handle[1], win_id);
	if(!result)
	{
		goto err;
	}
	else
	{
		count++;
	}
	result = Windmill_CreateImageLayer(&s_windmill_grass_info_jpg, 0, &s_windmill_grass_layer_handle_jpg, win_id);
	if(!result)
	{
		goto err;
	}
	else
	{
		count++;
	}
#if 0
	horinzen_buf_ptr=MMILIVEWALLPAPER_ALLOC(s_windmill_grass_info_jpg.theme_size.w*s_windmill_grass_info_jpg.theme_size.h*sizeof(uint16));//MMILIVEWALLPAPER_ALLOC

	if(PNULL==horinzen_buf_ptr)
	{
		goto err;
	}
	else
		{

			for(i=0;i<s_windmill_grass_info_jpg.theme_size.w;i++)
				{
					for(j=0;j<s_windmill_grass_info_jpg.theme_size.h;j++)
						{
							horinzen_buf_ptr[i*s_windmill_grass_info_jpg.theme_size.h+j]=((uint16 *)s_windmill_grass_info_jpg.theme_image_buf_ptr)[j*s_windmill_grass_info_jpg.theme_size.w+s_windmill_grass_info_jpg.theme_size.w-1-i];
						}
				}
            	        if(PNULL != s_windmill_grass_info_jpg.theme_image_buf_ptr)
            	        {
			SCI_MEMCPY(s_windmill_grass_info_jpg.theme_image_buf_ptr,horinzen_buf_ptr,s_windmill_grass_info_jpg.theme_size.w*s_windmill_grass_info_jpg.theme_size.h*sizeof(uint16));
            	        }
			MMILIVEWALLPAPER_FREE(horinzen_buf_ptr);//MMILIVEWALLPAPER_FREE  //SCI_FREE
			swap_temp=s_windmill_grass_info_jpg.theme_size.w;
			s_windmill_grass_info_jpg.theme_size.w=s_windmill_grass_info_jpg.theme_size.h;
			s_windmill_grass_info_jpg.theme_size.h=swap_temp;
		}
#endif
	for(i=0; i<WINDMILL_WING_NORMAL_NUM; i++)
    {
        result = Windmill_CreateImageLayer(&s_windmill_wing_info[i], 0, &s_windmill_wing_layer_handle[i], win_id);
		if(!result)
		{
			goto err;
		}
		else
		{
			count++;
		}
    }
	 for(i=0; i<WINDMILL_WING_NUM; i++)
    {
        result = Windmill_CreateImageLayer(&s_windmill_bar_info[i], 0, &s_windmill_bar_layer_handle[i], win_id);
		if(!result)
		{
			goto err;
		}
		else
		{
			count++;
		}
    }
	 for(i=0; i<WINDMILL_WING_NUM; i++)
    {
        result = Windmill_CreateImageLayer(&s_windmill_bottom_info[i], 0, &s_windmill_bottom_layer_handle[i], win_id);
		if(!result)
		{
			goto err;
		}
		else
		{
			count++;
		}
    }

    for(i=WINDMILL_WING_NORMAL_NUM; i<WINDMILL_WING_NUM; i++)
    {
        result = Windmill_CreateImageLayer(&s_windmill_wing_info[i], 1, &s_windmill_wing_layer_handle[i], win_id);
		if(!result)
		{
			goto err;
		}
		else
		{
			count++;
		}
    }

    for(i=0; i<WINDMILL_CENTER_NUM; i++)
    {
        result = Windmill_CreateImageLayer(&s_windmill_center_info[i], 0, &s_windmill_center_layer_handle[i], win_id);
		if(!result)
		{
			goto err;
		}
		else
		{
			count++;
		}
    }
	s_clean_grass_ptr=MMILIVEWALLPAPER_ALLOC((CUT_END-CUT_START)*s_windmill_grass_info_jpg.theme_size.h*sizeof(uint16));

	if(PNULL==s_clean_grass_ptr)
	{
		goto err;
	}
	else
	{
		count++;
	}
	#if 1
	for(i=0;i<s_windmill_grass_info_jpg.theme_size.h;i++)
		{	dst_ptr=&s_clean_grass_ptr[i*(CUT_END-CUT_START)];
			fill_ptr=&((uint16 *)s_windmill_grass_info_jpg.theme_image_buf_ptr)[i*s_windmill_grass_info_jpg.theme_size.w+CUT_START];
			end_ptr=&((uint16 *)s_windmill_grass_info_jpg.theme_image_buf_ptr)[i*s_windmill_grass_info_jpg.theme_size.w+CUT_END];
			//SCI_TRACE_LOW:"[LIVEWALLPAPER]  WHILE-----------IN"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_614_112_2_18_2_32_36_83,(uint8*)"");
			while(fill_ptr<end_ptr)
				{
					* dst_ptr++ = * fill_ptr++;
				}
			//SCI_TRACE_LOW:"[LIVEWALLPAPER]  WHILE-----------OUT"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_619_112_2_18_2_32_36_84,(uint8*)"");
		}
	#endif
	//SCI_MEMCPY(clean_grass_ptr,s_windmill_grass_info_jpg.theme_image_buf_ptr,s_windmill_grass_info_jpg.theme_size.w*s_windmill_grass_info_jpg.theme_size.h*sizeof(COLOR16));

	s_g_point_ptr=MMILIVEWALLPAPER_ALLOC(sizeof(FIX16_POINT_T)*2048);//算法需要SCI_ALLOCA
	if(PNULL==s_g_point_ptr)
	{
		goto err;
	}
	else
	{
		count++;
	}

    if (WINDMILLRequestVirtualHandle())
    {
        WINDMILLEnableMicSample(TRUE);
    }
	return TRUE;

err:
	WINDMILLDestruct();
	//SCI_TRACE_LOW:"[LIVEWALLPAPER] windmill layer construct failed   count = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_642_112_2_18_2_32_36_85,(uint8*)"d",count);
	return FALSE;

}

/*****************************************************************************/
//  Description : 风车草地准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T     display_point   = {0,0};

    if(PNULL != dev_info_ptr && UILAYER_IsLayerActive(dev_info_ptr))
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                        PNULL, 
                        PNULL, 
                        win_id, 
                        IMAGE_WINDMILL_BG, 
                        dev_info_ptr);
    } 
    return b_result;
}
/*****************************************************************************/
//  Description : 风车草地算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLCalculate(LW_CAL_PARAM *lw_cal_param)
{
    static uint32 s_cost_time = 0;
	static uint32 s_Whole_time_1 = 0;
	static uint32 s_Whole_time_2 = 0;
	static uint32 s_print_time = 0;
    uint32 i=0;
    uint32 j = 0;
    static int s_windmill_wing_angle  = 0;
    static int s_windmill_sun_1_angle = 3600;
    static int s_windmill_sun_2_angle = 3600;
    IMGREF_SIZE_T  bg_window_size = {0};
    FIX16_POINT_T  up_offset = {0};
    IMGREF_3D_POINT_T new_eye={0};
    uint32 *windmill_buf_ptr = PNULL;
    uint32 windmill_angle = 0;
    int bottom_height=0,d_times=0;
    uint8 sampling_type = MIC_SAMPLE_128;
    int eye_height=0;
    int moved_yaxis=0,moved_z_axis=0;
    int grass_height=0,half_total_wid=0;
    int grass_width=0,grass_bottom=0,current_width=0;
    int z_para=0,z_bar=0,positive_z=0;
	int32 Sin_Data=s2d_sin_new(WIN_GRASS_ANGLE);
	int32 Cos_Data=s2d_cos_new(WIN_GRASS_ANGLE);
    IMGREF_POINT_T draw_points[WINDMILL_WING_NUM]={0};
    IMGREF_POINT_T offset_value[WINDMILL_WING_NUM]={0};
    uint16 *target_buf_ptr = PNULL;
    uint16 *source_buf_ptr = PNULL;
	uint16 *dst_ptr = PNULL;
	uint16 *end_ptr = PNULL;
	uint16 *fill_ptr = PNULL;
    uint16 scr_width = PNULL;
    uint16 scr_height = PNULL;
    //int x_move=0,y_move=0;
    static int16 get_db = 0;
    static int16 db_pre = 0;
    // 2d to 3d
    IMGREF_POINT_T new_offset={0};
   
    int moved_eye=0;
    uint32 tick_time = 0;

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr)
    {
        return;
    }
    //看资源是否创建成功，找一个作为判断依据
    if(s_windmill_grass_layer_handle_jpg.block_id == UILAYER_NULL_HANDLE)
    {
        return;
    }
    
	tick_time = SCI_GetTickCount();
	s_Whole_time_1= SCI_GetTickCount();
	
	s_print_time=s_Whole_time_1-s_Whole_time_2;
	//SCI_TRACE_LOW:"[WHOLE_TIME], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_726_112_2_18_2_32_36_86,(uint8*)"d", s_print_time);
	s_Whole_time_2= SCI_GetTickCount();
    target_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;
    source_buf_ptr = lw_cal_param->src_buf_info.buf_ptr;
    scr_width = lw_cal_param->src_buf_info.w;
    scr_height = lw_cal_param->src_buf_info.h;
    eye_height = scr_height>>2;
	moved_z_axis = s_moved_z_axis;

    half_total_wid=((lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1-MMI_MAINSCREEN_WIDTH)/2);

	if(half_total_wid != 0)
	{
		moved_eye=-(lw_cal_param->fg_pos_info.x-half_total_wid)*MMI_MAINSCREEN_WIDTH/half_total_wid+45;
	}

	positive_z=-moved_z_axis;
	bg_window_size.w = scr_width;
	bg_window_size.h = scr_height;
	new_eye=s_fix_eye;
	
	s_cost_time = SCI_GetTickCount();
	
	grass_height=(((s_windmill_grass_info_jpg.theme_size.h)*Cos_Data)>>16);
	if(0==(bg_window_size.h-new_eye.y-grass_height))
		{
			return;
		}
	else
		{
	d_times=(((-new_eye.z)<<16)+Sin_Data*s_windmill_grass_info_jpg.theme_size.h)/(bg_window_size.h-new_eye.y-grass_height);		
	grass_height=grass_height+Sin_Data*s_windmill_grass_info_jpg.theme_size.h/d_times;
		}
	grass_width=s_windmill_grass_info_jpg.theme_size.w*(-new_eye.z)/(-new_eye.z+((Sin_Data*s_windmill_grass_info_jpg.theme_size.h)>>16));
	if(0==grass_height)
		{
			return;
		}
	for(i=0;i<WINDMILL_WING_NUM;i++)
		{	
			bottom_height=grass_height-(bg_window_size.h-s_windmill_bar_info[i].theme_offset_actual.y-s_windmill_bar_info[i].theme_size.h+2*EMPTY_PIXEL);
			s_new_offset[i]=(moved_eye)*bottom_height/grass_height;
			draw_points[i].y=s_windmill_grass_info_jpg.theme_size.h*bottom_height/grass_height-EMPTY_PIXEL;
			draw_points[i].x=(s_windmill_grass_info_jpg.theme_size.w-grass_width)*bottom_height/grass_height+grass_width;
			draw_points[i].x=s_windmill_grass_info_jpg.theme_size.w*(s_windmill_bar_info[i].theme_offset_actual.x-bg_window_size.w/2+draw_points[i].x/2)/(draw_points[i].x);
		}	
	
	for(i=0;i<WINDMILL_WING_NUM;i++)
		{	
			z_bar=(((s_windmill_grass_info_jpg.theme_size.h-draw_points[i].y)*Sin_Data)>>8);
			z_para=4096*(((-new_eye.z)<<8)+z_bar)/(((-new_eye.z)<<8)+z_bar-((-moved_z_axis)<<8));
			offset_value[i].x=s_windmill_bar_info[i].theme_offset_actual.x+s_windmill_bar_info[i].theme_size.w/2;
			offset_value[i].y=s_windmill_bar_info[i].theme_offset_actual.y+s_windmill_bar_info[i].theme_size.h-2*EMPTY_PIXEL;
			//offset_value[i].y=-EMPTY_PIXEL;
				if(0!=moved_z_axis)
				{
					grass_height=(((s_windmill_grass_info_jpg.theme_size.h)*Cos_Data)>>16);
					d_times=(((-new_eye.z)<<16)+(Sin_Data*s_windmill_grass_info_jpg.theme_size.h-(positive_z<<16)))/(bg_window_size.h-new_eye.y-grass_height);		
					grass_height=grass_height+(Sin_Data*s_windmill_grass_info_jpg.theme_size.h-(positive_z<<16))/d_times;
					grass_height=grass_height+(bg_window_size.h-new_eye.y)*positive_z/(-new_eye.z-positive_z);
					offset_value[i].y=((-new_eye.z)<<16)/d_times+new_eye.y+grass_height*(draw_points[i].y+EMPTY_PIXEL)/s_windmill_grass_info_jpg.theme_size.h+2;
					//offset_value[i].y=offset_value[i].y-s_windmill_bar_info[i].theme_offset_actual.y-(s_windmill_bar_info[i].theme_size.h-2*EMPTY_PIXEL)*z_para/4096;
					//求出X轴向的偏移量。
					grass_width=s_windmill_grass_info_jpg.theme_size.w*(-new_eye.z)*256/(-new_eye.z+((Sin_Data*(s_windmill_grass_info_jpg.theme_size.h))>>16)-positive_z);
					grass_bottom=s_windmill_grass_info_jpg.theme_size.w*new_eye.z*256/(new_eye.z-moved_z_axis);
					/*
					current_width=(grass_width+(grass_bottom-grass_width)*(draw_points[i].y+EMPTY_PIXEL)/s_windmill_grass_info_jpg.theme_size.h);
					offset_value[i].x=bg_window_size.w/2+(current_width*(draw_points[i].x+s_windmill_bar_info[i].theme_size.w/2)/s_windmill_grass_info_jpg.theme_size.w-current_width/2)/256;
					*/
					
					current_width=(grass_width+(grass_bottom-grass_width)*(draw_points[i].y+EMPTY_PIXEL)/s_windmill_grass_info_jpg.theme_size.h);
					offset_value[i].x=bg_window_size.w/2+(current_width*(draw_points[i].x+s_windmill_bar_info[i].theme_size.w/2)/s_windmill_grass_info_jpg.theme_size.w-current_width/2)/256+3;
				

				}
				
			
		}  
	s_cost_time = SCI_GetTickCount()-s_cost_time;
	//SCI_TRACE_LOW:"[YOON_CALC], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_805_112_2_18_2_32_36_87,(uint8*)"d", s_cost_time);
	// 2d to 3d

 

    s_cost_time = SCI_GetTickCount();
    if(PNULL == target_buf_ptr || PNULL == source_buf_ptr)
    {
        return;
    }

  // SCI_MEMSET(target_buf_ptr,0,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*(sizeof(uint16)));
    SCI_MEMCPY(target_buf_ptr,source_buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*(sizeof(uint16)));
    //SCI_TRACE_LOW:"WINDMILLCalculate, s_is_mic_sample_enb = %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_818_112_2_18_2_32_36_88,(uint8*)"d", s_is_mic_sample_enb);
    if (s_is_mic_sample_enb)
    {
        s_cost_time = SCI_GetTickCount();
        
        get_db = WINDMILLGetMic_dB(sampling_type);
       
        SCI_TRACE_LOW("harry, get_db = %d db", get_db); //test
        if ((get_db > db_pre + 20) && (db_pre > 0))
        {
            //flow_flog = 1;
            Windmill_PointProc(get_db,db_pre);
        }

        if (db_pre > 0)
        {
            db_pre = (db_pre + get_db)>>1;
        }
        else
        {
            db_pre = get_db;
        }
        //SCI_TRACE_LOW:"harry, db_pre = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_839_112_2_18_2_32_36_89,(uint8*)"d", db_pre);
    }

    if (s_wing_speed > 10)
    {
        s_wing_t ++;
        if (get_db < 50)
        {
            s_wing_speed -= s_wing_t>>5;
        }
        if (s_wing_speed < 10)
        {
            s_wing_t = 0;
            s_wing_speed = 10;
        }
    }
    else
    {
        s_wing_speed = 10;
    }
    //SCI_TRACE_LOW:"harry, s_wing_speed = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_859_112_2_18_2_32_36_90,(uint8*)"d", s_wing_speed);

    s_windmill_wing_angle += s_wing_speed;
    //windmill_wing_angle += 290;
    s_windmill_sun_1_angle -= 5;
    s_windmill_sun_2_angle -= 5;

    if(s_windmill_wing_angle >=3600)
    {
        s_windmill_wing_angle-=3600;
    }
    if(s_windmill_sun_1_angle <= 0)
    {
        s_windmill_sun_1_angle+=3600;
    }
    if(s_windmill_sun_2_angle <= 0)
    {
        s_windmill_sun_2_angle+=3600;
    }

    //SCI_TRACE_LOW:"harry, windmill_wing_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_879_112_2_18_2_32_36_91,(uint8*)"d", s_windmill_wing_angle);
    //SCI_TRACE_LOW:"harry, windmill_sun_1_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_880_112_2_18_2_32_36_92,(uint8*)"d", s_windmill_sun_1_angle);
    //SCI_TRACE_LOW:"harry, windmill_sun_2_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_881_112_2_18_2_32_36_93,(uint8*)"d", s_windmill_sun_2_angle);
	
		 s_cost_time = SCI_GetTickCount() - s_cost_time;
        SCI_TRACE_LOW("YOON_MEM&VOICE, s_cost_time windmill WINDMILLGetMic_dB cost time = %d ms",s_cost_time); //about 20ms

	
	s_cost_time = SCI_GetTickCount();
#if 1
        if(PNULL == s_clean_grass_ptr)
        {
            return;
        }
        
	for(i=0;i<s_windmill_grass_info_jpg.theme_size.h;i++)
		{	fill_ptr=&s_clean_grass_ptr[i*(CUT_END-CUT_START)];
			dst_ptr=&((uint16 *)s_windmill_grass_info_jpg.theme_image_buf_ptr)[i*s_windmill_grass_info_jpg.theme_size.w+CUT_START];
			end_ptr=&s_clean_grass_ptr[(i+1)*(CUT_END-CUT_START)];
			while(fill_ptr<end_ptr)
				{
					* dst_ptr++=* fill_ptr++;
				}
		}
	#endif
	
	s_cost_time = SCI_GetTickCount()-s_cost_time;
	//SCI_TRACE_LOW:"[YOON_MEMCPY], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_900_112_2_18_2_32_36_94,(uint8*)"d", s_cost_time);
	s_cost_time = SCI_GetTickCount();
	for(i=0;i<WINDMILL_WING_NUM;i++)
		{	
			
			Create_Shadow_To_Grass(&s_windmill_grass_info_jpg,
									s_windmill_bar_info, 
									s_windmill_wing_info,
									s_windmill_center_info,
									&draw_points[i],
									s_windmill_wing_angle,
									i,
									s_g_point_ptr);
		}


	
	s_cost_time = SCI_GetTickCount();
	Make_Rotate_Grass(target_buf_ptr,  moved_eye,moved_yaxis,moved_z_axis,new_eye,  bg_window_size,s_windmill_grass_info_jpg );
	s_cost_time = SCI_GetTickCount()-s_cost_time;
	
    //SCI_TRACE_LOW:"[YOON_ROTATE], CALC cost time = %d ms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_921_112_2_18_2_32_36_95,(uint8*)"d", s_cost_time);
	
	s_cost_time = SCI_GetTickCount();
		for (i =0; i<WINDMILL_WING_NUM; i++)
    {
        windmill_buf_ptr = s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_image_buf_ptr;//6改为宏，james
        windmill_angle   = s_windmill_wing_angle;
	new_offset.x= s_new_offset[WINDMILL_WING_NUM-1-i]+offset_value[WINDMILL_WING_NUM-1-i].x;
	new_offset.y=offset_value[WINDMILL_WING_NUM-1-i].y;
	s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_mid.y=s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_size.h-EMPTY_PIXEL-1;


	S2d_rotation_slim_with_z_size(bg_window_size, 
						s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_size, 
						new_offset,
						s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_mid, 
						0, 
						(uint32*)s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_image_buf_ptr, 
						target_buf_ptr, 
						z_para,
						WINDMILL_ZERO_SIZE,
						s_g_point_ptr);
	if(i<WINDMILL_WING_NORMAL_NUM)
		{
	windmill_buf_ptr = s_windmill_wing_info[i].theme_image_buf_ptr;
	new_offset.x= s_new_offset[WINDMILL_WING_NUM-1-i]+offset_value[WINDMILL_WING_NUM-1-i].x;
	new_offset.y=offset_value[WINDMILL_WING_NUM-1-i].y-(s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_size.h-2*EMPTY_PIXEL)*z_para/4096;

        S2d_rotation_slim_with_z_size(  bg_window_size,
                            s_windmill_wing_info[i].theme_size,
                            new_offset,
                            s_windmill_wing_info[i].theme_mid,
                            windmill_angle,
                            windmill_buf_ptr,
                            target_buf_ptr,
                            z_para,
                            WINDMILL_ZERO_SIZE,
                            s_g_point_ptr);


		}
	else
		{
			new_offset.x= s_new_offset[WINDMILL_WING_NUM-1-i]+offset_value[WINDMILL_WING_NUM-1-i].x-WINDMILL_OFFSET_X*z_para/4096;
			new_offset.y=offset_value[WINDMILL_WING_NUM-1-i].y-(s_windmill_bar_info[WINDMILL_WING_NUM-1-i].theme_size.h-2*EMPTY_PIXEL)*z_para/4096+WINDMILL_OFFSET_Y*z_para/4096;
			windmill_buf_ptr = s_windmill_wing_info[i].theme_image_buf_ptr;
			for (j = 0; j< WINDMILL_WING_ROTATION_NUM+1; j++)
			{
			windmill_angle = 1200 + windmill_angle;
			if (windmill_angle >= 3600)
			{
			    windmill_angle -= 3600;
			}
			S2d_rotation_slim_with_z_size(	bg_window_size,
			                            s_windmill_wing_info[i].theme_size,
			                            new_offset,
			                            s_windmill_wing_info[i].theme_mid,
			                            windmill_angle,
			                            windmill_buf_ptr,
			                            target_buf_ptr,
			                            z_para,
			                            WINDMILL_ZERO_SIZE,
			                            s_g_point_ptr);
			}
			
		}
	if(i<2)
		{
			 windmill_buf_ptr = s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_image_buf_ptr;
        	new_offset.x =s_new_offset[i]+offset_value[i].x-(int)(2-i);
       		 new_offset.y =offset_value[i].y-(s_windmill_bar_info[i].theme_size.h-2*EMPTY_PIXEL)*z_para/4096+WINDMILL_OFFSET_Y*z_para/4096;
       
			S2d_rotation_slim_with_z_size(	bg_window_size,
	                                        s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_size,
	                                        new_offset,
	                                        s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_mid,
	                                        0,
	                                       windmill_buf_ptr,
	                                        target_buf_ptr,
	                                        z_para,
	                                        WINDMILL_ZERO_SIZE,
	                                        s_g_point_ptr);
		}
	else if((i>=2)&&(i<4))
		{	
			windmill_buf_ptr = s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_image_buf_ptr;
      		new_offset.x =s_new_offset[i]+offset_value[i].x-WINDMILL_OFFSET_X*z_para/4096;
       		new_offset.y =offset_value[i].y-(s_windmill_bar_info[i].theme_size.h-2*EMPTY_PIXEL)*z_para/4096+WINDMILL_OFFSET_Y*z_para/4096;
//james 改立即数
			S2d_rotation_slim_with_z_size(	bg_window_size,
			                                s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_size,
			                                new_offset,
			                                s_windmill_center_info[WINDMILL_CENTER_NUM-1-i].theme_mid,
			                                0,
			                               windmill_buf_ptr,
			                                target_buf_ptr,
			                                z_para,
			                                WINDMILL_ZERO_SIZE,
			                                s_g_point_ptr);
		
		}
		
	windmill_buf_ptr = s_windmill_bottom_info[i].theme_image_buf_ptr;
	new_offset.x= s_new_offset[i]+offset_value[i].x;
	new_offset.y= s_offset_y[i]+offset_value[i].y;
	S2d_rotation_slim_with_z_size(bg_window_size, 
					s_windmill_bottom_info[i].theme_size, 
					new_offset,
					s_windmill_bottom_info[i].theme_mid, 
					0, 
					(uint32*)s_windmill_bottom_info[i].theme_image_buf_ptr, 
					target_buf_ptr, 
					z_para,
					WINDMILL_ZERO_SIZE,
					s_g_point_ptr);	
		
				
	
     
    }
	
  #if 1
    //画圆太阳
   
    windmill_buf_ptr = s_windmill_sun_info[1].theme_image_buf_ptr;
	up_offset.x = s_windmill_sun_info[1].theme_offset_actual.x - s_windmill_sun_info[1].theme_mid.x;
	up_offset.y = s_windmill_sun_info[1].theme_offset_actual.y - s_windmill_sun_info[1].theme_mid.y;
    S2D_Blending(bg_window_size ,target_buf_ptr, s_windmill_sun_info[1].theme_size, up_offset ,(uint32*)windmill_buf_ptr, target_buf_ptr);

   #endif

    //画太阳光束
    s_cost_time = SCI_GetTickCount();
    windmill_buf_ptr = s_windmill_sun_info[0].theme_image_buf_ptr;
    windmill_angle   = s_windmill_sun_1_angle;
	new_offset.x= s_windmill_sun_info[0].theme_offset_actual.x;
	new_offset.y= s_windmill_sun_info[0].theme_offset_actual.y;
    for (j = 0; j< 4; j++)
    {
        windmill_angle = 900 + windmill_angle;
        if (windmill_angle >= 3600)
        {
            windmill_angle -= 3600;
        }
		
        S2d_rotation_slim(	bg_window_size,
                                    s_windmill_sun_info[0].theme_size,
                                    new_offset,
                                    s_windmill_sun_info[0].theme_mid,
                                    windmill_angle,
                                    windmill_buf_ptr,
                                    target_buf_ptr,
                                    4096,
                                    s_g_point_ptr);
    }
    s_cost_time = SCI_GetTickCount()-s_cost_time;
	
    //SCI_TRACE_LOW:"[YOON_DRAW], CALC cost time = %d ms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_1078_112_2_18_2_32_37_96,(uint8*)"d", s_cost_time);
  
    tick_time = SCI_GetTickCount()-tick_time;
	
    //SCI_TRACE_LOW:"[YOON_TOTAL], CALC cost time = %d ms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_1082_112_2_18_2_32_37_97,(uint8*)"d", tick_time);

}
/*****************************************************************************/
//  Description : 风车草地释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLDestruct(void)
{
    uint32 i = 0;

    for(i=0; i<WINDMILL_SUN_NUM; i++)
    {
        UILAYER_ReleaseLayer(&s_windmill_sun_layer_handle[i]);
        s_windmill_sun_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<WINDMILL_WING_NUM; i++)
    {
        UILAYER_ReleaseLayer(&s_windmill_wing_layer_handle[i]);
        s_windmill_wing_info[i].theme_image_buf_ptr = PNULL;
		 UILAYER_ReleaseLayer(&s_windmill_bar_layer_handle[i]);
        s_windmill_bar_info[i].theme_image_buf_ptr = PNULL;
		UILAYER_ReleaseLayer(&s_windmill_bottom_layer_handle[i]);
        s_windmill_bottom_info[i].theme_image_buf_ptr = PNULL;
    }
	
    for(i=0; i<WINDMILL_CENTER_NUM; i++)
    {
        UILAYER_ReleaseLayer(&s_windmill_center_layer_handle[i]);
        s_windmill_center_info[i].theme_image_buf_ptr = PNULL;
    }

	UILAYER_ReleaseLayer(&s_windmill_grass_layer_handle_jpg);
	s_windmill_grass_info_jpg.theme_image_buf_ptr = PNULL;
	
	MMILIVEWALLPAPER_FREE(s_clean_grass_ptr);
	MMILIVEWALLPAPER_FREE(s_g_point_ptr);
	
    WINDMILLEnableMicSample(FALSE);
    WINDMILLReleaseVirtualHandle();   //being short of a parenthesis to cause a big assert;

    //MMIDEFAULT_EnableKeyRing(TRUE);
    //MMIDEFAULT_EnableTpRing(TRUE);
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description : 风车手势消息处理
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Windmill_HandleTpGesture(DPARAM param)
{	
	if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))//收缩
	{
		s_moved_z_axis += WINDMILL_MOVE_Z_PARAM;
	}
	else if(MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))//伸展
	{
		s_moved_z_axis -= WINDMILL_MOVE_Z_PARAM;
	}
	else
	{

	}
	
	s_moved_z_axis = MIN(s_moved_z_axis,150);

	s_moved_z_axis = MAX(s_moved_z_axis,-120);
}

/*****************************************************************************/
//  Description : 风车TP UP消息处理
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Windmill_HandleTpUp(DPARAM param)
{
	Windmill_PointProc(30, 0);
}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

/*****************************************************************************/
//  Description : 风车草地释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WINDMILLHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
		case MSG_TP_GESTURE://手势处理 拉近  放缩
			Windmill_HandleTpGesture(param);
			break;

		case MSG_TP_PRESS_UP:
			Windmill_HandleTpUp(param);
			break;

	    default:
	        return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    return TRUE;
}
/*****************************************************************************/
//  Description : creat shadow to grass
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Create_Shadow_To_Grass(WINDMILL_INFO_T *grass_info,
	WINDMILL_INFO_T *origin_image,
	WINDMILL_INFO_T *wing_info,
	WINDMILL_INFO_T *center_info,
	IMGREF_POINT_T *start_point,
	int angle,
	int wing_num,
	FIX16_POINT_T * Point_Add_ptr
	)
{	uint32 s_cost_time=0;
	int draw_wid=0,draw_height=0,i=0,j=0;
	IMGREF_POINT_T point_on_grass={0},wing_centre={0};
	COLOR16* grass_buff=PNULL;
	COLOR32 * image_buff=PNULL;
	register uint8 alpha=0;
	register COLOR16 Color_R_G=0,Color_B=0,tempcolor=0;
	
	point_on_grass.x=start_point->x;
	point_on_grass.y=start_point->y;
	draw_wid=origin_image[wing_num].theme_size.w;
	draw_height=MIN((grass_info->theme_size.h-start_point->y),origin_image[wing_num].theme_size.h);
	wing_centre.x=point_on_grass.x+(origin_image[wing_num].theme_size.w)/2;
	wing_centre.y=point_on_grass.y+origin_image[wing_num].theme_size.h-EMPTY_PIXEL;
	s_cost_time = SCI_GetTickCount();
	/*
	if(4==wing_num)
			{
			for(i=0;i<grass_info->theme_size.h;i++)
				{
					((COLOR16 *)grass_info->theme_image_buf_ptr)[i*grass_info->theme_size.w+point_on_grass.x-11]=0xf000;
					((COLOR16 *)grass_info->theme_image_buf_ptr)[i*grass_info->theme_size.w+point_on_grass.x-10]=0xf000;
				}
			}
	if(3==wing_num)
			{
			for(i=0;i<grass_info->theme_size.h;i++)
				{
					((COLOR16 *)grass_info->theme_image_buf_ptr)[i*grass_info->theme_size.w+point_on_grass.x+21]=0x000f;
					((COLOR16 *)grass_info->theme_image_buf_ptr)[i*grass_info->theme_size.w+point_on_grass.x+20]=0x000f;
				}
			}
		*/
	
	for(i=0;i<draw_height;i++)
		{	
			grass_buff=&((COLOR16 *)grass_info->theme_image_buf_ptr)[(i+point_on_grass.y)*grass_info->theme_size.w+point_on_grass.x];
			image_buff=&((COLOR32 *)origin_image[wing_num].theme_image_buf_ptr)[(draw_height-i-1)*origin_image[wing_num].theme_size.w];
			for(j=0;j<draw_wid;j++)
				{	alpha=((*image_buff)>>24);
							if(alpha)
								{
							tempcolor =* grass_buff;
							Color_R_G=(((tempcolor)&0xf81f)>>1)&0xf81f;
							Color_B=(((tempcolor)&0x07e0)>>1)&0x07e0;
							* grass_buff =Color_R_G|Color_B;
							
								}
							grass_buff++;
							image_buff++;
						
				}
		}
	s_cost_time = SCI_GetTickCount()-s_cost_time;
	//SCI_TRACE_LOW:"[YOON_SHADOW_BAR], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_1245_112_2_18_2_32_37_98,(uint8*)"d", s_cost_time);
	s_cost_time = SCI_GetTickCount();
	if(wing_num<WINDMILL_WING_ROTATION_NUM)
		{
				 for (j = 0; j< 3; j++)
			{
			    angle = 1200 + angle ;
			    if (angle  >= 3600)
			    {
			        angle  -= 3600;
			    }
				S2d_Shadow_slim(grass_info->theme_size, 
		  					wing_info[6-wing_num].theme_size,
		  					wing_centre, 
		  					wing_info[6-wing_num].theme_mid,
		  					-angle+1800, 
		  					(COLOR32 *)wing_info[6-wing_num].theme_image_buf_ptr, 
		  					(COLOR16 *)grass_info->theme_image_buf_ptr, 
		  					4096, 
		  					Point_Add_ptr);
			    
			}
		
		}
	//6，7用宏代替,james
	else if((wing_num>=WINDMILL_WING_ROTATION_NUM)&&(wing_num<WINDMILL_WING_NUM))
		{
		  S2d_Shadow_slim(grass_info->theme_size, 
		  					wing_info[WINDMILL_WING_NUM-1-wing_num].theme_size,
		  					wing_centre, wing_info[WINDMILL_WING_NUM-1-wing_num].theme_mid,
		  					-angle+1800, (COLOR32 *)wing_info[WINDMILL_WING_NUM-1-wing_num].theme_image_buf_ptr, 
		  					(COLOR16 *)grass_info->theme_image_buf_ptr, 
		  					4096, 
		  					Point_Add_ptr);
		}
	s_cost_time = SCI_GetTickCount()-s_cost_time;
	//SCI_TRACE_LOW:"[YOON_SHADOW_WING], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_1281_112_2_18_2_32_37_99,(uint8*)"d", s_cost_time);
	s_cost_time = SCI_GetTickCount();
//		if(wing_num<WINDMILL_CENTER_NUM)
//			{	
//				draw_height=center_info[WINDMILL_CENTER_NUM-1-wing_num].theme_size.h;
//				draw_wid=center_info[WINDMILL_CENTER_NUM-1-wing_num].theme_size.w;
//						for(i=0;i<draw_height;i++)
//				{	
//					grass_buff=&((COLOR16 *)grass_info->theme_image_buf_ptr)[(i+wing_centre.y-2*EMPTY_PIXEL)*grass_info->theme_size.w+wing_centre.x-draw_wid/2];
//					image_buff=&((COLOR32 *)center_info[WINDMILL_CENTER_NUM-1-wing_num].theme_image_buf_ptr)[i*center_info[WINDMILL_CENTER_NUM-1-wing_num].theme_size.w];
//					for(j=0;j<draw_wid;j++)
//						{	alpha=((*image_buff)>>24);
//									if(alpha)
//										{
//									tempcolor =* grass_buff;
//									Color_R_G=(((tempcolor)&0xf81f)>>1)&0xf81f;
//									Color_B=(((tempcolor)&0x07e0)>>1)&0x07e0;
//									* grass_buff =Color_R_G|Color_B;
//									
//										}
//									grass_buff++;
//									image_buff++;
//								
//						}
//				}
//			}
		
		s_cost_time = SCI_GetTickCount()-s_cost_time;
	//SCI_TRACE_LOW:"[YOON_SHADOW_CENTRE], CALC cost time = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_WINDMILL_1309_112_2_18_2_32_37_100,(uint8*)"d", s_cost_time);
	return SCI_TRUE;
}
/*****************************************************************************/
//  Description : grass rotate
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Make_Rotate_Grass(COLOR16 * tempColor, int grass_offset,int y_offset,int z_offset,IMGREF_3D_POINT_T new_eye,IMGREF_SIZE_T win_size, WINDMILL_INFO_T grass_img)
{   
 

	
	
	S3D_ROTATION_GEOMETRY_T  s3d_geometry={0};
	S3D_ROTATION_IMG_T   s3d_src_img={0};
	S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img={0};
		
	s3d_geometry.img_size.w=grass_img.theme_size.w;
	s3d_geometry.img_size.h=grass_img.theme_size.h;
	//s3d_geometry.img_size.y=0;
	s3d_geometry.img_size.y = grass_img.theme_size.h/2-y_offset;
	s3d_geometry.img_size.x=500;
	s3d_geometry.angle=-WIN_GRASS_ANGLE+1800;
	s3d_geometry.eye.x=new_eye.x;
	s3d_geometry.eye.y=new_eye.y;
	s3d_geometry.eye.z=new_eye.z;

	s3d_geometry.axis.x=win_size.w/2;
	s3d_geometry.axis.y=win_size.h;
	s3d_geometry.axis.z=z_offset;
	s3d_geometry.axis.axis=0;
	s3d_geometry.is_axis_use_default = SCI_FALSE;
	s3d_geometry.is_eye_use_default = SCI_FALSE;

	s3d_src_img.src_ptr=(COLOR16 *)grass_img.theme_image_buf_ptr;
	s3d_src_img.src_type=S3D_DATA_RGB565;
	
	s3d_img.color_ptr=tempColor;
	s3d_img.size.h=win_size.h;
	s3d_img.size.w=win_size.w;
	/*
	S3D_ImageMidAxisRotation_Ex ( &(s3d_geometry),&(s3d_src_img),&(s3d_img),grass_offset);
	S2D_Blending(win_size, tempColor, win_size, offset, temp32_buff, tempColor);
	SCI_FREE(temp32_buff);
	*/
	
	S3D_ImageMidAxis565Rotation_Windmill( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,grass_offset);
	
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 WINDMILLGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_WINDMILL;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_WINDMILL_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_WINDMILL;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_WINDMILL;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void WINDMILLGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,width_ptr,height_ptr);
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *WINDMILLGetOptionList(void)
{
    return (OPTIONLIST*)PNULL;
}  
#endif// LIVE_WALLPAPER_SUPPORT_WINDMILL


#endif //_MMI_LIVEWALLPAPER_LINEAR_C_


/*Edit by script, ignore 2 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
