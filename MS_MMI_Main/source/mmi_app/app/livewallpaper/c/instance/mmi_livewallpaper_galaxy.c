

/*****************************************************************************
** File Name:      mmi_livewallpaper_galaxy.c                                    *
** Author:         Gaily.Wang                                                *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011       Gaily.Wang          Create                                  *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_GALAXY_C_
#define _MMI_LIVEWALLPAPER_GALAXY_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY

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
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GALAXY_STAR_NUM (15000)
#define BLOCK_MEM_POOL_SIZE_GALAXY (350 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct _GALAXY_MOVE_STAR_POINT_T_
{
    int32 x;
    int32 y;
    int32 r;
    int32 a;
    uint32 color;
}GALAXY_MOVE_STAR_POINT_T;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL GALAXY_MOVE_STAR_POINT_T *sg_galaxy_star = PNULL;

LOCAL BOOLEAN g_galaxy_is_init = FALSE;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 extern   int s2d_sin(int angle);
extern  int s2d_cos(int angle);
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 星系初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyInit(void);
/*****************************************************************************/
//  Description : 星系初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 星系准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 星系算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GalaxyCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 星系释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void GalaxyDestruct(void);
/*****************************************************************************/
//  Description : 星系交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 GalaxyGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GalaxyGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_galaxy_Instance =
{
    GalaxyInit,
    GalaxyConstruct,
    GalaxyDrawBg,
    GalaxyCalculate,
    GalaxyDestruct,
    GalaxyHandleEvent,
    PNULL,
    GalaxyGetType,
    GalaxyGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : GALAXY 刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GalaxyUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height)
{
    register int x = 0;
    register int y = 0;
    int i = 0;

    //SCI_TRACE_LOW:"[Star] GalaxyUpdate-----1-----time = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_GALAXY_128_112_2_18_2_32_2_23,(uint8*)"d", SCI_GetTickCount());
    if(PNULL != sg_galaxy_star)
    {
        for(i = 0; i < GALAXY_STAR_NUM; i++)
        {
            int r = sg_galaxy_star[i].r;

            x = ((r*s2d_sin(sg_galaxy_star[i].a))>>16) + sg_galaxy_star[i].x;
            y = ((r*s2d_cos(sg_galaxy_star[i].a))>>16) + sg_galaxy_star[i].y;

            sg_galaxy_star[i].a -= 2;

            if(sg_galaxy_star[i].a < 0)
                sg_galaxy_star[i].a += 3600;

            if(y<scr_height && y>=0 && x<scr_width && x>=0)
                target_buf_ptr[(y)*scr_width+(x)] = sg_galaxy_star[i].color;
        }
    }
    //SCI_TRACE_LOW:"[Star] GalaxyUpdate-----2-----time = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_GALAXY_147_112_2_18_2_32_2_24,(uint8*)"d", SCI_GetTickCount());
}
/*****************************************************************************/
//  Description : 星系初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyInit(void)
{
    BOOLEAN b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 星系初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    int32 i = 0;
    
    if(FALSE == g_galaxy_is_init)
    {
        sg_galaxy_star = (GALAXY_MOVE_STAR_POINT_T *)MMILIVEWALLPAPER_ALLOC(GALAXY_STAR_NUM*sizeof(GALAXY_MOVE_STAR_POINT_T));//SCI_ALLOCA //MMILIVEWALLPAPER_ALLOC
        
        if(PNULL != sg_galaxy_star)
        {
            for(i = 0; i < GALAXY_STAR_NUM; i++)
            {
                int a = 0;
                int b = 0;
                int k = 0;
                int t[4] = {0};

                while(a < 3000)
                {
                    a = rand()%100;
                    b = (rand()%100)+0;
                    a *= b;
                }
                sg_galaxy_star[i].r = a/(28*1);
                k = rand()%2;
                sg_galaxy_star[i].a = (a+k*1800)%3600;
                a = (rand()%2);
                if(a==0)
                    a = -1;
                b = (int)GDI_sqrt(sg_galaxy_star[i].r*2/6+1);
                t[0] = rand()%b;
                t[1] = rand()%b;
                t[2] = rand()%b;
                t[3] = rand()%b;

                sg_galaxy_star[i].x = a*(t[0]*t[1])+MMI_MAINSCREEN_WIDTH/2;
                a = (rand()%2);
                if(a==0)
                    a = -1;
                sg_galaxy_star[i].y = a*(t[2]*t[3])+MMI_MAINSCREEN_HEIGHT/2;
                sg_galaxy_star[i].r -= 130;
                a = sg_galaxy_star[i].r/50;
                a = a * 4 + 1;
                sg_galaxy_star[i].color = ((rand()%(a))+31-a*2)*0x0841;

            }

            g_galaxy_is_init = TRUE;
            
    	    b_result = TRUE;
        }
        else
        {
    	    b_result = FALSE;
        }
    }
    return b_result;
}

/*****************************************************************************/
//  Description :星系准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};
    
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                                        PNULL, 
                                        PNULL, 
                                        win_id, 
                                        IMAGE_WALLPAPER_GALAXY_BG, 
                                        dev_info_ptr); 
    }
    return b_result;
}
#define LW_NEW_TEST
/*****************************************************************************/
//  Description : 星系算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GalaxyCalculate(LW_CAL_PARAM *lw_cal_param)
{

    if((PNULL == lw_cal_param) || (PNULL == lw_cal_param->dest_buf_info.buf_ptr) || (PNULL == lw_cal_param->src_buf_info.buf_ptr))
    {
        return;
    }
    SCI_MEMCPY((GUI_COLOR_T*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
    
    GalaxyUpdate(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->src_buf_info.w,lw_cal_param->src_buf_info.h);

}
/*****************************************************************************/
//  Description : 星系释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void GalaxyDestruct(void)
{
    if(TRUE == g_galaxy_is_init)
    {
        if(PNULL != sg_galaxy_star)
        {
            MMILIVEWALLPAPER_FREE(sg_galaxy_star); //SCI_FREE
            sg_galaxy_star = PNULL;
        }

        g_galaxy_is_init = FALSE;
    }
}

/*****************************************************************************/
//  Description : 星系交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalaxyHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    return TRUE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 GalaxyGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_GALAXY;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GalaxyGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_GALAXY_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_GALAXY;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_GALAXY;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LINEAR


#endif //_MMI_LIVEWALLPAPER_GALAXY_C_
