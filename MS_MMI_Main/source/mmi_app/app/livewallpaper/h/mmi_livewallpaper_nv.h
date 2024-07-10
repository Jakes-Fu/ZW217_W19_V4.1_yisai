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

#ifndef _MMI_LIVEWALLPAPER_NV_H_
#define _MMI_LIVEWALLPAPER_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
        MMILIVEWALLPAPER_TYPE_SAVER = MMI_MODULE_LIVEWALLPAPER<< 16,
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
        MMINV_LIVE_WALLPAPER_SAKURA,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
        MMINV_LIVE_WALLPAPER_SEAWORLD,
#endif  
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
        MMINV_LIVE_WALLPAPER_SLIDESHOW,
    #endif    
        MMILIVEWALLPAPER_MAX_NV_ITEM_NUM
}MMI_LIVEWALLPAPER_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register live wallpaper module nv len and max item
//	Global resource dependence : none
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMLIVEWALLPAPER_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIWIDGET_NV_H_


