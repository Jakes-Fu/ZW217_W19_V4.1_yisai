/*****************************************************************************
** File Name:      mmi_wallpaper_export.h                                           *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007       Jassmine              Creat
******************************************************************************/

#ifndef _MMI_WALLPAPER_EXPORT_H_
#define _MMI_WALLPAPER_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "sci_types.h"
#include "mmi_filemgr.h"
#include "guiimg.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//wallpaper width and height
typedef struct 
{
    uint32              width;
    uint32              height;
    MMIFILE_DEVICE_E    fs_dev; //save device
    GUIIMG_DATE_TYPE_E  data_type;
} MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T;  

typedef enum
{
    LIVE_WALLPAPER_BASE = 0,     
    #ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
    LIVE_WALLPAPER_LINEAR,
    #endif
    #ifdef LIVE_WALLPAPER_SUPPORT_POP
    LIVE_WALLPAPER_POP, 
    #endif
    #ifdef LIVE_WALLPAPER_SUPPORT_BEE
    LIVE_WALLPAPER_BEE, 
    #endif
    LIVE_WALLPAPER_MAX             
}MMIIDLE_LIVE_WALLPAPER_TYPE_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreatWallpaperCtrl(void);

/*****************************************************************************/
//  Description : wallpaper file is exist
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsExistWallpaperFile(
                                            BOOLEAN     *is_gif_ptr //in/out:may PNULL
                                            );

/*****************************************************************************/
//  Description : set wallpaper parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetWallpaper(
                                 BOOLEAN    is_change
                                 );

/*****************************************************************************/
//  Description : save wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_SaveWallpaperBitmap(
                                           GUIIMG_BITMAP_T  *bitmap_ptr//in
                                           );

/*****************************************************************************/
//  Description : set init wallpaper parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_SetInitWallpaper(
                                        BOOLEAN         is_save_bitmap, //only for wallpaper
                                        MMI_WIN_ID_T    win_id,
                                        MMI_CTRL_ID_T   ctrl_id
                                        );

/*****************************************************************************/
//  Description :却换墙纸
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_Change_Wallpaper(void);

/*****************************************************************************/
//  Description :check wall paper valid
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CheckWallPaperValid(void);

/*****************************************************************************/
//  Description : creat wallpaper anim control 
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreatAnimCtrl(
                                        MMI_HANDLE_T win_id, 
                                        MMI_CTRL_ID_T ctrl_id, 
                                        BOOLEAN is_owner_draw
                                        );

/*****************************************************************************/
//  Description : get wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_GetWallpaperBitmap(
                                             GUIIMG_BITMAP_T    *bitmap_ptr //in/out
                                             );

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    

/*****************************************************************************/
//  Description : 当前是否为动态墙纸
//  Global resource dependence : 
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsLiveWallpaper(void);

#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
/************************************/
//for key lock win setting wallpaper
/*************************/
PUBLIC BOOLEAN MMIAPIIDLE_SetInitWallpaper_KL(
                                        BOOLEAN         is_save_bitmap, //only for wallpaper
                                        MMI_WIN_ID_T    win_id,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN disp_one_frame
                                        );



#endif
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIIDLE_CreatWallpaperCtrl          MMIAPIIDLE_CreatWallpaperCtrl
#define MMIIDLE_IsExistWallpaperFile        MMIAPIIDLE_IsExistWallpaperFile
#define MMIIDLE_SetWallpaper                MMIAPIIDLE_SetWallpaper
#define MMIIDLE_SaveWallpaperBitmap         MMIAPIIDLE_SaveWallpaperBitmap
#define MMIIDLE_SetInitWallpaper            MMIAPIIDLE_SetInitWallpaper
#define MMIIDLE_Change_Wallpaper            MMIAPIIDLE_Change_Wallpaper
#define MMIIDLE_CheckWallPaperValid         MMIAPIIDLE_CheckWallPaperValid
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    
#define MMIIDLE_IsLiveWallpaper             MMIAPIIDLE_IsLiveWallpaper
#endif
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
/************************************/
//for key lock win setting wallpaper
/*************************/
#define MMIIDLE_SetInitWallpaper_KL         MMIAPIIDLE_SetInitWallpaper_KL
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
