/******************************************************************************
 ** File Name:      mmi_app_wallpaper_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:237
#ifndef _MMI_APP_WALLPAPER_TRC_H_
#define _MMI_APP_WALLPAPER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_WALLPAPER_591_112_2_18_3_6_28_0 "IsCorrespondingSrcWallpaper: get wallpaper file size_time_len %d is error!"
#define MMI_WALLPAPER_651_112_2_18_3_6_29_1 "GetWallpaperBitmap error:PNULL == bitmap_ptr"
#define MMI_WALLPAPER_699_112_2_18_3_6_29_2 "GetWallpaperBitmap PNULL == bitmap_ptr->bit_ptr"
#define MMI_WALLPAPER_718_112_2_18_3_6_29_3 "GetWallpaperBitmap:bitmap_data_size = %d,read_size= %d"
#define MMI_WALLPAPER_758_112_2_18_3_6_29_4 "MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr"
#define MMI_WALLPAPER_763_112_2_18_3_6_29_5 "MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr->bit_ptr"
#define MMI_WALLPAPER_962_112_2_18_3_6_29_6 "GenerateMiniatureFile: write src file size and creat time error!"
#define MMI_WALLPAPER_967_112_2_18_3_6_29_7 "GenerateWallpaperFile: get file size and creat timer error!"
#define MMI_WALLPAPER_972_112_2_18_3_6_29_8 "GenerateWallpaperFile: write wallpaper file error!"
#define MMI_WALLPAPER_999_112_2_18_3_6_29_9 "GetFileSizeAndCreatTime param ERROR! "
#define MMI_WALLPAPER_1010_112_2_18_3_6_29_10 "GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!"
#define MMI_WALLPAPER_1032_112_2_18_3_6_29_11 "GetWallpaperWidthHeight ERROR! PNULL == width_height_ptr"
#define MMI_WALLPAPER_1056_112_2_18_3_6_29_12 "SetWallpaperWidthHeight ERROR! PNULL == width_height_ptr"
#define MMI_WALLPAPER_1074_112_2_18_3_6_29_13 "[IDLE] Get_Change_WallpaperInfo wallpaper_info 0x%x !"
#define MMI_WALLPAPER_1126_112_2_18_3_6_29_14 "[IDLE] Get_Change_WallpaperInfo id 0x%x type %d"
#define MMI_WALLPAPER_1136_112_2_18_3_6_29_15 "[idle] MMIAPIIDLE_Change_Wallpaper enter time %d"
#define MMI_WALLPAPER_1143_112_2_18_3_6_29_16 "[idle] MMIAPIIDLE_Change_Wallpaper exit time %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WALLPAPER_TRC)
TRACE_MSG(MMI_WALLPAPER_591_112_2_18_3_6_28_0,"IsCorrespondingSrcWallpaper: get wallpaper file size_time_len %d is error!")
TRACE_MSG(MMI_WALLPAPER_651_112_2_18_3_6_29_1,"GetWallpaperBitmap error:PNULL == bitmap_ptr")
TRACE_MSG(MMI_WALLPAPER_699_112_2_18_3_6_29_2,"GetWallpaperBitmap PNULL == bitmap_ptr->bit_ptr")
TRACE_MSG(MMI_WALLPAPER_718_112_2_18_3_6_29_3,"GetWallpaperBitmap:bitmap_data_size = %d,read_size= %d")
TRACE_MSG(MMI_WALLPAPER_758_112_2_18_3_6_29_4,"MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr")
TRACE_MSG(MMI_WALLPAPER_763_112_2_18_3_6_29_5,"MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr->bit_ptr")
TRACE_MSG(MMI_WALLPAPER_962_112_2_18_3_6_29_6,"GenerateMiniatureFile: write src file size and creat time error!")
TRACE_MSG(MMI_WALLPAPER_967_112_2_18_3_6_29_7,"GenerateWallpaperFile: get file size and creat timer error!")
TRACE_MSG(MMI_WALLPAPER_972_112_2_18_3_6_29_8,"GenerateWallpaperFile: write wallpaper file error!")
TRACE_MSG(MMI_WALLPAPER_999_112_2_18_3_6_29_9,"GetFileSizeAndCreatTime param ERROR! ")
TRACE_MSG(MMI_WALLPAPER_1010_112_2_18_3_6_29_10,"GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!")
TRACE_MSG(MMI_WALLPAPER_1032_112_2_18_3_6_29_11,"GetWallpaperWidthHeight ERROR! PNULL == width_height_ptr")
TRACE_MSG(MMI_WALLPAPER_1056_112_2_18_3_6_29_12,"SetWallpaperWidthHeight ERROR! PNULL == width_height_ptr")
TRACE_MSG(MMI_WALLPAPER_1074_112_2_18_3_6_29_13,"[IDLE] Get_Change_WallpaperInfo wallpaper_info 0x%x !")
TRACE_MSG(MMI_WALLPAPER_1126_112_2_18_3_6_29_14,"[IDLE] Get_Change_WallpaperInfo id 0x%x type %d")
TRACE_MSG(MMI_WALLPAPER_1136_112_2_18_3_6_29_15,"[idle] MMIAPIIDLE_Change_Wallpaper enter time %d")
TRACE_MSG(MMI_WALLPAPER_1143_112_2_18_3_6_29_16,"[idle] MMIAPIIDLE_Change_Wallpaper exit time %d")
END_TRACE_MAP(MMI_APP_WALLPAPER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WALLPAPER_TRC_H_

