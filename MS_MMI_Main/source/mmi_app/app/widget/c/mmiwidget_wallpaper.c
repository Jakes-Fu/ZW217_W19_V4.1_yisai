/*****************************************************************************
** File Name:      mmiwidget_wallpaper.c                                     *
** Author:                                                                   *
** Date:           08/10/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe wallpaper under widget ilde *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/10/2012     ying.xu              Creat
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiwidget.h"
#include "guictrl_api.h"
#include "guicommon.h"
#include "mmitheme_widget.h"
#include "mmiwidget_id.h"
#include "Mmi_wallpaper_export.h"
#include "mmiwidget_image.h"
#include "mmiwidget_position.h"
#include "mmiwidget_text.h"
#include "mmiwidget_page.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_wallpaper.h"

/*****************************************************************************/
//  Description : create wallpaper layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CreateWallpaperLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    
    uint32  wallpaper_layer_size = MMIWIDGET_GetWallpaperLayerSize(idle_grid_ptr);

    if(!IsStaticWallpaper())
    {
        return;
    }
    
    if(1 == wallpaper_layer_size)
    {        
        idle_grid_ptr->wallpaper_layer = *MMITHEME_GetDefaultLcdDev();
        widget_layer_handle = idle_grid_ptr->wallpaper_layer;
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &widget_layer_handle);
        return;
    }      

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->wallpaper_layer))    
     {
        width  = main_rect.right  - main_rect.left + 1;
        height = main_rect.bottom - main_rect.top  + 1;
        
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = wallpaper_layer_size * width;
        create_info.height          = height;
        create_info.is_bg_layer     = TRUE;
        create_info.is_static_layer = FALSE;   
        create_info.format          = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);        
        idle_grid_ptr->wallpaper_layer = widget_layer_handle;
    }
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &widget_layer_handle);
        UILAYER_RemoveMainLayer();
    }
}

/*****************************************************************************/
//  Description : destroy wallpaper layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DestroyWallpaperLayer(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 )
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    
    if(!IsStaticWallpaper())
    {
        return;
    }
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->wallpaper_layer))    
    {
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, MMITHEME_GetDefaultLcdDev()); /*lint !e605*/

        GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &rect);
        
        UILAYER_RELEASELAYER(&(idle_grid_ptr->wallpaper_layer));   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : check whether the current wallpaper is set to static picture
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IsStaticWallpaper(void)
{
    MMISET_WALLPAPER_SET_T        wallpaper_info      = {0};
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if( MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type
        || MMISET_WALLPAPER_ALBUM == wallpaper_info.wallpaper_type
        || MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type
        )
    {            
        return TRUE;
    }    
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void SetWallpaperPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int16 offsetX, int16 offsetY, BOOLEAN is_update)
{
    GUI_RECT_T rect   = {0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    int16 offset_x = 0;
    uint32  wallpaper_layer_size = MMIWIDGET_GetWallpaperLayerSize(idle_grid_ptr);
    
    if(1 == wallpaper_layer_size)
    {
        return;
    }

    if(!IsStaticWallpaper())
    {
        return;
    }
    
    if(idle_grid_ptr->page_num <= 1)
    {
        return;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);    
    
    offset_x = offsetX/((int16)idle_grid_ptr->page_num - 1);
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->wallpaper_layer, offset_x, 0);
    
    rect.left   = offset_x; 
    rect.top    = 0;
    rect.right  = rect.left + (int16)(wallpaper_layer_size * lcd_width);
    rect.bottom = rect.top  + lcd_height;
    
    GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &rect);    

}

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void SetWallpaperDeltaPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int16 offsetX, BOOLEAN is_update)
{
    GUI_RECT_T rect   = {0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    int16  offset_x   = 0;
    int16  offset_y   = 0;
    uint32  wallpaper_layer_size = MMIWIDGET_GetWallpaperLayerSize(idle_grid_ptr);
    
    if(1 == wallpaper_layer_size)
    {
        return;
    }

    if(!IsStaticWallpaper())
    {
        return;
    }    
    if(idle_grid_ptr->page_num <= 1)
    {
        return;
    }
    
    UILAYER_GetLayerPosition(&idle_grid_ptr->wallpaper_layer, &offset_x, &offset_y);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 
    
    
    offset_x += (offsetX/((int16)idle_grid_ptr->page_num - 1));
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->wallpaper_layer, offset_x, offset_y);
    
    rect.left   = offset_x;  
    rect.top    = offset_y;
    rect.right  = rect.left + (int16)(wallpaper_layer_size * lcd_width);
    rect.bottom = rect.top  + lcd_height;
    
    GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &rect);    
}

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CopyWallpaper2MainLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
#if 0
    uint16            lcd_width      = 0;
    uint16            lcd_height     = 0;
    GUI_RECT_T        disp_rect      = {0};
    const GUI_LCD_DEV_INFO *defalt_dev_ptr = PNULL;
    
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        return;
    }
#endif
    defalt_dev_ptr = MMITHEME_GetDefaultLcdDev();
    
    if(!UILAYER_IsLayerActive(&idle_grid_ptr->wallpaper_layer))
    {
        return;
    }
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    disp_rect.left   = 0;
    disp_rect.top    = 0;
    disp_rect.right  = disp_rect.left + lcd_width - 1;
    disp_rect.bottom = disp_rect.top  + lcd_height - 1;
    
    UILAYER_BltLayerToLayer(defalt_dev_ptr, &idle_grid_ptr->wallpaper_layer, &disp_rect, FALSE);    /*lint !e605*/  
#endif
    int16   offset_x = 0;
    int16   offset_y = 0;
    offset_x = -idle_grid_ptr->cur_page_index * MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);
    SetWallpaperPosition(idle_grid_ptr, offset_x, offset_y, FALSE);
}

/*****************************************************************************/
//  Description : reset wallpaper layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ResetWallpaperLayer(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               )
{    
    GUI_RECT_T rect = MMITHEME_GetFullScreenRectEx(idle_grid_ptr->win_id);//MMITHEME_GetFullScreenRect();            	
    uint32  wallpaper_layer_size = MMIWIDGET_GetWallpaperLayerSize(idle_grid_ptr);

    if(!IsStaticWallpaper())
    {
        return;
    }
        
    if (UILAYER_IsLayerActive(&idle_grid_ptr->wallpaper_layer))
    {
        //MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &idle_grid_ptr->wallpaper_layer);
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(idle_grid_ptr->win_id, MMIIDLE_WALLPAPER_ANIM_CTRL_ID), 
            &idle_grid_ptr->wallpaper_layer);             

    }
    else
    {        
        UILAYER_RestoreMainLayer();
        //Ö÷²ãÄÚÈÝ£ºshortcut£¬×´Ì¬À¸£¬softkey, Ç½Ö½£¬widgetÓ¦ÓÃ
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(idle_grid_ptr->win_id, MMIIDLE_WALLPAPER_ANIM_CTRL_ID), 
            MMITHEME_GetDefaultLcdDev()); /*lint !e605 */
    }
            
    rect.right = (rect.right - rect.left + 1) * (int16)wallpaper_layer_size;     
    GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &rect);
}

