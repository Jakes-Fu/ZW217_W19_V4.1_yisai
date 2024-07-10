/*****************************************************************************
** File Name:      mmiwidget_wallpaper.h                                     *
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

#ifndef _MMIWIDGET_WALLPAPER_H_
#define _MMIWIDGET_WALLPAPER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 
#ifdef __cplusplus
         extern   "C"
         {
#endif

/*****************************************************************************/
//  Description : reset wallpaper layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ResetWallpaperLayer(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               );

/*****************************************************************************/
//  Description : destroy wallpaper layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DestroyWallpaperLayer(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 );

/*****************************************************************************/
//  Description : create wallpaper layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CreateWallpaperLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void SetWallpaperPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int16 offsetX, int16 offsetY, BOOLEAN is_update);

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void SetWallpaperDeltaPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int16 offsetX, BOOLEAN is_update);

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CopyWallpaper2MainLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
PUBLIC BOOLEAN IsStaticWallpaper(void);


#ifdef   __cplusplus
    }
#endif
#endif
