/*****************************************************************************
** File Name:      mmi_livewallpaper_nv.h                                    *
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

#ifndef _MMI_LIVEWALLPAPER_NV_C_
#define _MMI_LIVEWALLPAPER_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset_nv.h"
#include "mmilivewallpaper_export.h"
#include "mmi_modu_main.h"
#include "mmi_livewallpaper_sakura.h"
#include "mmi_livewallpaper_seaworld.h"

#ifdef SLIDESHOW_WALLPAPER_SUPPORT
#include "mmi_livewallpaper_slideshow.h"
#endif

/*the length of set nv*/
LOCAL const uint16 mmilivewallpaper_nv_len[] =
{
    sizeof(uint32),//MMILIVEWALLPAPER_TYPE_SAVER
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
    sizeof(WALLPAPER_SAKURA_NV_T),                  //MMINV_LIVE_WALLPAPER_SAKURA,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
    sizeof(WALLPAPER_SEAWORLD_NV_T),   ///MMINV_LIVE_WALLPAPER_SEAWORLD
#endif
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
    sizeof(WALLPAPER_SLIDESHOW_NV_T),   ///MMINV_LIVE_WALLPAPER_SLIDESHOW
    #endif

};

/*****************************************************************************/
// 	Description : register live wallpaper module nv len and max item
//	Global resource dependence : none
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMLIVEWALLPAPER_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_LIVEWALLPAPER, mmilivewallpaper_nv_len, sizeof(mmilivewallpaper_nv_len) / sizeof(uint16));
}
#endif


