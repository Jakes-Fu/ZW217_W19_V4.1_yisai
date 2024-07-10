/*****************************************************************************
** File Name:      mmi_livewallpaper_win_cfg.c                                    *
** Author:          arvin.wu                                                *
** Date:           01/08/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  08/2011      Arvin.wu            Create                                  *
******************************************************************************/

#ifndef _MMI_LIVEWALLPAPER_WIN_CFG_C_
#define _MMI_LIVEWALLPAPER_WIN_CFG_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"
#include "mmi_livewallpaper_preview.h"
#include "mmiidle_export.h"
#include "mmikl_effectmain.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 #ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : MMIWIDGET_GetWallpaperLayer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GetMainLayer(GUI_LCD_DEV_INFO *layer_ptr);

/*
    该文件为配置文件，需要增加一个窗体的支持，在这个
    表里增加两项，一个是Win id,一个是显示的层，这里的层
    都是565的层
*/
LOCAL MMI_LIVEWALLPAPER_WIN_CFG s_livewallpaper_win_cfg_table[] = 
{
    {(uint32)MAIN_IDLE_WIN_ID,MMIAPIIDLE_GetMainAppFgInfo},
    {(uint32)MMILIVEWALLPAPER_PREVIEW_WIN_ID,MMILIVEWALLPAPER_GetPreviewFgInfo}, 
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
    {(uint32)MMIKL_CLOCK_DISP_WIN_ID,MMILIVEWALLPAPER_GetKLFgInfo},
    #endif
};

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
//  Description : 判断是否支持的窗体
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_IsSupportWin(MMI_WIN_ID_T win_id)
{
    int32 i = 0;
    int32 cfg_win_count = ARRAY_SIZE(s_livewallpaper_win_cfg_table);

    for(i = 0;i < cfg_win_count;i++)
    {
        if(s_livewallpaper_win_cfg_table[i].win_id == win_id)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : get the display info ,like the fg's rect,and current x,y, and display layer
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_GetDisplayInfo(MMI_WIN_ID_T win_id, LW_FG_INFO *lw_fg_info_ptr)
{
    int32 i = 0;
    int32 cfg_win_count = ARRAY_SIZE(s_livewallpaper_win_cfg_table);
    LW_FG_INFO lw_fg_info = {0};
        
    if(PNULL != lw_fg_info_ptr)
    {
        for(i = 0;i < cfg_win_count;i++)
        {
            if(s_livewallpaper_win_cfg_table[i].win_id == win_id)
            {
                if(s_livewallpaper_win_cfg_table[i].GetFgInfo)
                {
                    s_livewallpaper_win_cfg_table[i].GetFgInfo(lw_fg_info_ptr);
                    break;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : MMIWIDGET_GetWallpaperLayer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GetMainLayer(GUI_LCD_DEV_INFO *layer_ptr)
{
    GUI_LCD_DEV_INFO main_layer = {0, 0};
    *layer_ptr = main_layer;
}

#endif /*_MMI_LIVEWALLPAPER_WIN_CFG_C_*/
