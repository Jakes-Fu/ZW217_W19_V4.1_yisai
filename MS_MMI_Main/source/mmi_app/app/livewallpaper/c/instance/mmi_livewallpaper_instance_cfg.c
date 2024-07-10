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

#ifndef _MMI_LIVEWALLPAPER_INSTANCE_CFG_C_
#define _MMI_LIVEWALLPAPER_INSTANCE_CFG_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 #ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_liner_Instance;
 #endif
 
#ifdef LIVE_WALLPAPER_SUPPORT_POP
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_pop_Instance;
#endif
 
 #ifdef LIVE_WALLPAPER_SUPPORT_BEE
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_bee_Instance;
 #endif
 
#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_galaxy_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_sakura_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_seaworld_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_lightgrid_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_ripple_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_collision_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_littlecat_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_sweettree_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_collision_balloon_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_collision_menu_Instance;
#endif 
 #ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_windmill_Instance;
 #endif
 #ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_rainbowcity_Instance;
 #endif
 #ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_papertown_Instance;
 #endif
 #ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_aquarium_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
 extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_girlswing_Instance;
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_SPRING
extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_spring_Instance;
#endif
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_slideshow_Instance;
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_NEST
extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_nest_Instance;
#endif

/*
    记录所有实例的数组，请其他应用不要引用，
    为了把客户要修改的文件集中出来
*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T *g_livewallpaper_item_table[MMI_LIVE_WALLPAPER_TYPE_MAX] = 
{
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
    &g_liner_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
    &g_pop_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_BEE
    &g_bee_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
    &g_galaxy_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
    &g_sakura_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
    &g_seaworld_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
    &g_lightgrid_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
    &g_ripple_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
    &g_collision_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
    &g_littlecat_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
    &g_sweettree_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
    &g_collision_balloon_Instance, 
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
    &g_collision_menu_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
    &g_windmill_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
   &g_rainbowcity_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
   &g_papertown_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
    &g_aquarium_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
     &g_girlswing_Instance,
#endif 
#ifdef LIVE_WALLPAPER_SUPPORT_SPRING
    &g_spring_Instance,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_NEST
    &g_nest_Instance,
#endif
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
    &g_slideshow_Instance,
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

#endif /*_MMI_LIVEWALLPAPER_INSTANCE_CFG_C_*/
