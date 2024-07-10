/*****************************************************************************
** File Name:      mmi_wallpaer.c                                            *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       Jassmine.Meng       Create                                  *
******************************************************************************/

#define _MMI_WALLPAPER_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wallpaper_trc.h"
#include "mmi_wallpaper_export.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
#include "guianim.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_nv.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
//#include "mmiset_display.h"
//#include "mmiset_wintab.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)	
#include"Mmiwidget_id.h"
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#endif
#include "guictrl_api.h"
#include "guiownerdraw.h"
#include "mmk_gsensor.h"
#include "special_effect.h"
#include "mmi_appmsg.h"
#include "guistatusbar.h"
#include "guiwin.h"
#include "mmiidle_export.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_utility.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIDLE_WALLPAPER_NAME              (g_wallpaper_name)//"\x00\x57\x00\x61\x00\x6c\x00\x6c\x00\x70\x00\x61\x00\x70\x00\x65\x00\x72\x00\x2e\x00\x64\x00\x61\x00\x74"  //wallpaper.dat
#define MMIIDLE_WALLPAPER_NAME_LEN          13 //墙纸名字的长度

#define MMIIDLE_WALLPAPER_SIZE_TIME_LEN     11  //wallpaper文件size+creat time max len


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const wchar g_wallpaper_name[]   = { 'w','a','l','l','p','a','p','e','r','.','d','a','t', 0 };
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : NextLiveWallpaperIdFromBegining;
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL  int32  NextLiveWallpaperIdFromBegining();

/*****************************************************************************/
//  Description : NextLiveWallpaperIndexFromCurrent;
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL  int32  NextLiveWallpaperIndexFromCurrent();
#endif

/*****************************************************************************/
//  Description : 重新获取墙纸信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  void  Get_Change_WallpaperInfo(MMISET_WALLPAPER_SET_T *wallpaper_info);

/*****************************************************************************/
//  Description : the wallpaper.dat is corresponding to the src wallpaper file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsCorrespondingSrcWallpaper(
                                          const wchar   *full_path_ptr, //in
                                          uint16        full_path_len   //in
                                          );

/*****************************************************************************/
//  Description : get wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWallpaperBitmap(
                                 GUIIMG_BITMAP_T    *bitmap_ptr //in/out
                                 );

/*****************************************************************************/
// 	Description : get wallpaper data file dev
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWallpaperDev(
                              uint32            min_free_space, //in:
                              MMIFILE_DEVICE_E  *file_dev_ptr   //in/out:
                              );

///*****************************************************************************/
//// 	Description : get devie path
////	Global resource dependence : 
////  Author: Jassmine
////	Note: 
///*****************************************************************************/
//LOCAL wchar* GetDevicePath(
//                           MMIFILE_DEVICE_E     fs_dev,         //in:
//                           uint16           *path_len_ptr   //in/out:
//                           );
// 
/*****************************************************************************/
//  Description : is need save wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSaveWallpaperBitmap(void);

/*****************************************************************************/
//  Description : generate wallpaper file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GenerateWallpaperFile(
                                    const wchar                     *full_path_ptr, //in
                                    uint16                          full_path_len,  //in
                                    uint8                           *write_buf_ptr, //in
                                    uint32                          to_write_size,  //in
                                    MMIMULTIM_AS_WALLPAPER_INFO     *src_file_ptr   //in:src file name
                                    );

/*****************************************************************************/
//  Description : get file size and creat time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileSizeAndCreatTime(
                                      wchar             *full_path_ptr,     //in
                                      uint16            full_path_len,      //in
                                      SFS_FIND_DATA_T   *find_data_ptr      //in/out
                                      );

/*****************************************************************************/
//  Description : get wallpaper bitmap width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetWallpaperWidthHeight(
                                   MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T     *width_height_ptr
                                   );

/*****************************************************************************/
//  Description : get wallpaper bitmap width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaperWidthHeight(
                                   MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T     *width_height_ptr
                                   );

/*****************************************************************************/
//  Description : creat wallpaper anim control 
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreatWallpaperCtrl(
                                 MMI_HANDLE_T win_id, 
                                 MMI_CTRL_ID_T ctrl_id, 
                                 BOOLEAN is_owner_draw
                                 );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

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
                                        )
{
    return CreatWallpaperCtrl(win_id, ctrl_id, is_owner_draw);
}

/*****************************************************************************/
//  Description : creat wallpaper anim control 
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreatWallpaperCtrl(
                                 MMI_HANDLE_T win_id, 
                                 MMI_CTRL_ID_T ctrl_id, 
                                 BOOLEAN is_owner_draw
                                 )
{
    IGUICTRL_T                    *anim_ctrl_ptr  = PNULL;
    GUI_BOTH_RECT_T         anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    MMI_CONTROL_CREATE_T    create          = {0};
    //GUIOWNDRAW_INIT_DATA_T  owndraw_init = {0};
    GUIANIM_INIT_DATA_T     init_data       = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    //MMISET_WALLPAPER_SET_T wallpaper_info = {0};

    create.ctrl_id           = ctrl_id;
    create.parent_win_handle = win_id;

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    
    if(is_owner_draw)
    {
        return TRUE;
    }
    else
#endif        
    {
        init_data.both_rect = anim_ctrl_rect;
        create.init_data_ptr     = &init_data;
        create.guid = SPRD_GUI_ANIM_ID;
    }

    anim_ctrl_ptr = MMK_CreateControl( &create );
    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    if( PNULL != anim_ctrl_ptr )
    {
        GUIAPICTRL_SetState(ctrl_handle,
            GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, 
            TRUE
            );

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreatWallpaperCtrl(void)
{    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_CreatWallpaperCtrl");
    MMIAPIIDLE_SetInitWallpaper(TRUE, MAIN_IDLE_WIN_ID, MMIIDLE_WALLPAPER_ANIM_CTRL_ID );
    return (TRUE);
}

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
                                        )
{
    BOOLEAN                         result              = FALSE;
    BOOLEAN                         is_gif              = FALSE;
    GUIANIM_RESULT_E                anim_result         = GUIANIM_RESULT_FAIL;
    GUIANIM_CTRL_INFO_T             control_info        = {0};
    GUIANIM_DATA_INFO_T             data_info           = {0};
    GUIANIM_FILE_INFO_T             file_info           = {0};
    GUIANIM_DISPLAY_INFO_T          display_info        = {0};
    MMISET_WALLPAPER_SET_T          wallpaper_info      = {0};
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr  = {0};
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMI_HANDLE_T ctrl_handle = 0;
#endif
    IGUICTRL_T *ctrl_ptr = 0;
    //CAF_GUID_T guid = 0;
    
    //set parameter
    control_info.is_wallpaper_ctrl = TRUE;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;

	display_info.is_syn_decode = (BOOLEAN)(!is_save_bitmap);
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_save_wallpaper = is_save_bitmap;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_BLACK_COLOR;

    //get wallpaper info
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_SetInitWallpaper GetWallpaperSetInfo  type = %d, id = %d", 
	    wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);
	
    if (MMIAPIUDISK_UdiskIsRun() && 
        (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type 
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        || MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type
#endif
#ifdef  LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        || MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type
#endif
    ))
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    if(MMISET_WALLPAPER_LIVE== wallpaper_info.wallpaper_type && MMIKL_CLOCK_DISP_WIN_ID == win_id)
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
    else
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    if(MMISET_WALLPAPER_VIDEO== wallpaper_info.wallpaper_type && MMIKL_CLOCK_DISP_WIN_ID == win_id)
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
    else
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    if(MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
    {
        ctrl_ptr = MMK_GetCtrlPtrByWin(win_id, ctrl_id);
        if(ctrl_ptr != PNULL)
        {
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
            MMK_DestroyControl(ctrl_handle);
        }
    }
    else
#endif
    {
        ctrl_ptr = MMK_GetCtrlPtrByWin(win_id, ctrl_id);
        /* if wallpaper ctrl did not exist, we should create it */
        if(PNULL == ctrl_ptr)
        {
            CreatWallpaperCtrl(win_id, ctrl_id, FALSE);
        }
    }
    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_SetInitWallpaper SetWallpaperSetInfo type = %d, id = %d", 
	    wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);
    switch(wallpaper_info.wallpaper_type)
    {
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    case MMISET_WALLPAPER_VIDEO:
       data_info.img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());
       GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
       break;
#endif
    
    case MMISET_WALLPAPER_FIXED_PIC:
        //get image data buffer
        if (wallpaper_info.wallpaper_id >= MMISET_WALLPAPER_DEFAULT_ID) /*lint !e685 !e568*/
        {
            data_info.img_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);
            anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            if ((GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
            {
                result = TRUE;
            }
        }
        break;

    case MMISET_WALLPAPER_FIXED_ANIM:
        break;
    case MMISET_WALLPAPER_ALBUM:
    case MMISET_WALLPAPER_DOWNLOAD:
        if (MMIAPIIDLE_IsExistWallpaperFile(&is_gif))
        {
            if ((!is_gif) &&
                (GetWallpaperBitmap(&data_info.bitmap_data)))
            {
                //anim will free wallpaper
                data_info.is_bitmap = TRUE;
                data_info.is_free_bitmap = TRUE;

                if (MMITHEME_IsSupportGsensor())
                {
                    //get file name info
                    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
                    file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                    file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                }
                anim_result = GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
            else
            {
                //free bitmap
                SCI_FREE(data_info.bitmap_data.bit_ptr);

                //get file name info
                MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

                file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                anim_result = GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
        }
        else
        {
            //reset default wallpaper
            MMIAPISET_ResetDefaultWallpaper();
        }
        break;
    default:
        //SCI_PASSERT(FALSE,("MMIAPIIDLE_SetInitWallpaper:wallpaper_type %d is error!",wallpaper_info.wallpaper_type));
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : wallpaper file is exist
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsExistWallpaperFile(
                                            BOOLEAN     *is_gif_ptr //in/out:may PNULL
                                            )
{
    BOOLEAN                         result              = FALSE;
    wchar                           suffix[10]          = {0};
    const wchar                     gif_wstr[]          = { 'g', 'i', 'f', 0 };
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr  = {0};

    //get file name info
    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

    if (MMIAPIFMM_IsFileExist(wallpaper_file_ptr.full_path_name,wallpaper_file_ptr.full_path_len))
    {
        if (PNULL != is_gif_ptr)
        {
            MMIAPICOM_SplitSuffix(PNULL,0,suffix,9,wallpaper_file_ptr.full_path_name);
            MMIAPICOM_Wstrlower(suffix);

            if( 0 == MMIAPICOM_Wstrncmp(suffix,gif_wstr,3))
            {
                *is_gif_ptr = TRUE;
            }
            else
            {
                *is_gif_ptr = FALSE;
            }
        }

        result = TRUE;
    }
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_IsExistWallpaperFile result = %d", result);
    return (result);
}

/*****************************************************************************/
//  Description : set wallpaper parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetWallpaper(
                                 BOOLEAN    is_change
                                 )
{
    uint16                              full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16                              full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    uint16                              dev_path_len = 0;
    wchar                               *dev_path_ptr = PNULL;
    MMI_WIN_ID_T                        win_id = MAIN_IDLE_WIN_ID;
    MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T	width_height = {0};
    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_SetWallpaper is_change = %d", is_change);

    if (MMK_IsOpenWin(win_id))
    {
        //get wallpaper bitmap width and height
        GetWallpaperWidthHeight(&width_height);

        //get devie path
        dev_path_ptr = MMIAPIFMM_GetDevicePath(width_height.fs_dev);
		dev_path_len = MMIAPIFMM_GetDevicePathLen(width_height.fs_dev);
			//GetDevicePath(width_height.fs_dev,&dev_path_len);
        
        //combine full path
        if(MMIAPIFMM_CombineFullPath((uint16 *)dev_path_ptr,
            dev_path_len,
            PNULL,
            0,
            (uint16 *)MMIIDLE_WALLPAPER_NAME,
            MMIIDLE_WALLPAPER_NAME_LEN,
            full_path_name,
            &full_path_len))
        {
            if (is_change)
            {

                //delete wallpaper file
                MMIAPIFMM_DeleteFileSyn(full_path_name,full_path_len);				
            }
        }

        MMIAPIIDLE_CreatWallpaperCtrl();
    }
}

/*****************************************************************************/
//  Description : the wallpaper.dat is corresponding to the src wallpaper file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsCorrespondingSrcWallpaper(
                                          const wchar   *full_path_ptr, //in
                                          uint16        full_path_len   //in
                                          )
{
    BOOLEAN                         result                                          = FALSE;
    uint8                           size_time[MMIIDLE_WALLPAPER_SIZE_TIME_LEN+1]    = {0};
    uint32                          i                                               = 0;
    uint32                          size_time_len                                   = 0;
    uint32                          file_size                                       = 0;
    uint32                          file_offset                                     = 0;
    uint32                          src_file_size                                   = 0;
    SFS_HANDLE                      file_handle                                     = 0;
    SFS_DATE_T                      src_creat_date                                  = {0};
    SFS_TIME_T                      src_creat_time                                  = {0};
    SFS_FIND_DATA_T                 find_data                                       = {0};
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr                              = {0};

    //get file size and creat time from wallpaper.dat
    file_handle = SFS_CreateFile(full_path_ptr,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,PNULL,PNULL);
    if (0 != file_handle)
    {
        if (SFS_NO_ERROR == SFS_GetFileSize(file_handle,&file_size))
        {
            //read wallpaper.dat last MMIIDLE_WALLPAPER_SIZE_TIME_LEN byte
            file_offset = file_size - MMIIDLE_WALLPAPER_SIZE_TIME_LEN;
            if(SFS_NO_ERROR == SFS_SetFilePointer(file_handle,(int64)file_offset,SFS_SEEK_BEGIN))
            {
                if(SFS_ERROR_NONE == SFS_ReadFile(file_handle,size_time,(MMIIDLE_WALLPAPER_SIZE_TIME_LEN),&size_time_len,PNULL))
                {
                    if ((MMIIDLE_WALLPAPER_SIZE_TIME_LEN) == size_time_len)
                    {
                        //get src file size and creat time
                        src_file_size = (size_time[i++] << 24) & 0xFF000000;
                        src_file_size += ((size_time[i++] << 16) & 0x00FF0000);
                        src_file_size += ((size_time[i++] << 8) & 0x0000FF00);
                        src_file_size += (size_time[i++] & 0x000000FF);

                        //get src file creat time year
                        src_creat_date.year = (size_time[i++] << 8) & 0xFF00;
                        src_creat_date.year += (size_time[i++] & 0x00FF);

                        //get src file creat time month
                        src_creat_date.mon = size_time[i++];

                        //get src file creat time day
                        src_creat_date.mday = size_time[i++];

                        //get src file creat time hour
                        src_creat_time.hour = size_time[i++];

                        //get src file creat time minute
                        src_creat_time.min = size_time[i++];

                        //get src file creat time second
                        src_creat_time.sec = size_time[i++];
                        SCI_ASSERT(i == MMIIDLE_WALLPAPER_SIZE_TIME_LEN);/*assert to do*/

                        result = TRUE;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"IsCorrespondingSrcWallpaper: get wallpaper file size_time_len %d is error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_591_112_2_18_3_6_28_0,(uint8*)"d",size_time_len);
                    }
                }
            }
        }
    }

    //close miniature file
    SFS_CloseFile(file_handle);

    if (result)
    {
        //get wallpaper file name info
        MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

        //get src file size and creat time
        if (GetFileSizeAndCreatTime(wallpaper_file_ptr.full_path_name,
                wallpaper_file_ptr.full_path_len,
                &find_data))
        {
            //compare two file is corresponding
            if ((src_file_size == find_data.length) &&
                (src_creat_date.year == find_data.create_Date.year) && 
                (src_creat_date.mon == find_data.create_Date.mon) && 
                (src_creat_date.mday == find_data.create_Date.mday) && 
                (src_creat_time.hour == find_data.create_time.hour) && 
                (src_creat_time.min == find_data.create_time.min) && 
                (src_creat_time.sec == find_data.create_time.sec))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_GetWallpaperBitmap(
                                             GUIIMG_BITMAP_T    *bitmap_ptr //in/out
                                             )
{
    return GetWallpaperBitmap(bitmap_ptr);
}

/*****************************************************************************/
//  Description : get wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWallpaperBitmap(
                                 GUIIMG_BITMAP_T    *bitmap_ptr //in/out
                                 )
{
    BOOLEAN                             result = FALSE;
    uint16                              full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16                              full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    uint16                              dev_path_len = 0;
    wchar                               *dev_path_ptr = PNULL;
    uint32                              bitmap_data_size = 0;
    MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T    width_height = {0};

    //SCI_ASSERT(PNULL != bitmap_ptr);
    if(PNULL == bitmap_ptr)
    {
        //SCI_TRACE_LOW:"GetWallpaperBitmap error:PNULL == bitmap_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_651_112_2_18_3_6_29_1,(uint8*)"");
        return FALSE;
    }

    //get wallpaper bitmap width and height
    GetWallpaperWidthHeight(&width_height);

    //get devie path
	dev_path_ptr = MMIAPIFMM_GetDevicePath(width_height.fs_dev);
    dev_path_len = MMIAPIFMM_GetDevicePathLen(width_height.fs_dev);

    //combine full path
    if(MMIAPIFMM_CombineFullPath((uint16 *)dev_path_ptr,
        dev_path_len,
        PNULL,
        0,
        (uint16 *)MMIIDLE_WALLPAPER_NAME,
        MMIIDLE_WALLPAPER_NAME_LEN,
        full_path_name,
        &full_path_len))

    {
        if ((IsSaveWallpaperBitmap()) && 
            (MMIAPIFMM_IsFileExist(full_path_name,full_path_len)))
        {
            if (IsCorrespondingSrcWallpaper(full_path_name,full_path_len))
            {
                //get wallpaper bitmap width and height
                if (GUIIMG_DATE_TYPE_ARGB888 == width_height.data_type)
                {
                    bitmap_data_size = width_height.width * width_height.height * 4;
                }
                else
                {
                    bitmap_data_size = width_height.width * width_height.height * 2;
                }
                bitmap_ptr->img_height = width_height.height;
                bitmap_ptr->img_width  = width_height.width;
                
                if (0 < bitmap_data_size)
                {
                    uint32          read_size   = 0;
                    SFS_HANDLE      file_handle = SFS_ERROR_NONE;

                    //get wallpaper buffer
                    bitmap_ptr->bit_ptr = SCI_ALLOCA(bitmap_data_size);
                    if(PNULL == bitmap_ptr->bit_ptr)
                    {
                        //SCI_TRACE_LOW:"GetWallpaperBitmap PNULL == bitmap_ptr->bit_ptr"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_699_112_2_18_3_6_29_2,(uint8*)"");
                        return FALSE;
                    }                   

                    SCI_MEMSET(bitmap_ptr->bit_ptr,0,bitmap_data_size);

                    file_handle = SFS_CreateFile(full_path_name,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,PNULL,PNULL);

                    if(SFS_INVALID_HANDLE != file_handle)
                    {
                        if(SFS_ERROR_NONE == SFS_ReadFile(file_handle,bitmap_ptr->bit_ptr,bitmap_data_size,&read_size,PNULL))
                        {
                            if (read_size == bitmap_data_size)
                            {
                                result = TRUE;
                            }
                            else
                            {
                                //可能文件没有写完被终止,比如断电
                                //SCI_TRACE_LOW:"GetWallpaperBitmap:bitmap_data_size = %d,read_size= %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_718_112_2_18_3_6_29_3,(uint8*)"dd",bitmap_data_size,read_size);
                            }
                        }
                    }

                    SFS_CloseFile(file_handle);
                }
            }
            else
            {
                //delete wallaper.dat
                MMIAPIFMM_DeleteFileSyn(full_path_name,full_path_len);
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : save wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_SaveWallpaperBitmap(
                                           GUIIMG_BITMAP_T  *bitmap_ptr//in
                                           )
{
    BOOLEAN                         result = FALSE;
    uint16                          full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16                          full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    uint16                          dev_path_len = 0;
    wchar                           *dev_path_ptr = PNULL;
    uint32                          wallpaper_data_size = 0;
    MMIFILE_DEVICE_E                fs_dev = MMI_DEVICE_SYSTEM;
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr = {0};

    //SCI_ASSERT(PNULL != bitmap_ptr);
    //SCI_ASSERT(PNULL != bitmap_ptr->bit_ptr);
    if (PNULL == bitmap_ptr)  
    {
        //SCI_TRACE_LOW:"MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_758_112_2_18_3_6_29_4,(uint8*)"");
        return FALSE;
    }
    if (PNULL == bitmap_ptr->bit_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIIDLE_SaveWallpaperBitmap ERROR! PNULL == bitmap_ptr->bit_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_763_112_2_18_3_6_29_5,(uint8*)"");
        return FALSE;
    }
    
    //get wallpaper data size
    if (GUIIMG_DATE_TYPE_ARGB888 == bitmap_ptr->date_type)
    {
        wallpaper_data_size = bitmap_ptr->img_width * bitmap_ptr->img_height * 4;
    }
    else
    {
	    wallpaper_data_size = bitmap_ptr->img_width * bitmap_ptr->img_height * 2;
    }

    //get wallpaper save device
    if (GetWallpaperDev((wallpaper_data_size + MMIIDLE_WALLPAPER_SIZE_TIME_LEN),&fs_dev))
    {
        //get devie path
        dev_path_ptr = MMIAPIFMM_GetDevicePath(fs_dev);
		dev_path_len = MMIAPIFMM_GetDevicePathLen(fs_dev);
			//GetDevicePath(fs_dev,&dev_path_len);
        
        //combine full path
        if(MMIAPIFMM_CombineFullPath((uint16 *)dev_path_ptr,
            dev_path_len,
            PNULL,
            0,
            (uint16 *)MMIIDLE_WALLPAPER_NAME,
            MMIIDLE_WALLPAPER_NAME_LEN,
            full_path_name,
            &full_path_len))

        {
            if (IsSaveWallpaperBitmap())
            {
                //get file name info
                MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

                //write file
                result = GenerateWallpaperFile(full_path_name,full_path_len,bitmap_ptr->bit_ptr,wallpaper_data_size,&wallpaper_file_ptr);

                if (result)
                {
                    MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T    width_height = {0};

                    //save width and height in nv
                    width_height.width     = bitmap_ptr->img_width;
                    width_height.height    = bitmap_ptr->img_height;
                    width_height.fs_dev    = fs_dev;
                    width_height.data_type = bitmap_ptr->date_type;

                    SetWallpaperWidthHeight(&width_height);

                    result = TRUE;
                }
            }
        }
    }

	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_SaveWallpaperBitmap result = %d", result);

    return (result);
}

/*****************************************************************************/
// 	Description : get wallpaper data file dev
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWallpaperDev(
                              uint32        min_free_space, //in:
                              MMIFILE_DEVICE_E  *file_dev_ptr   //in/out:
                              )
{
    BOOLEAN     result = FALSE;
    MMIFILE_DEVICE_E      i = 0;

    for (i = MMI_DEVICE_NUM -1; i >= MMI_DEVICE_UDISK; i--)
    {
        if (MMIAPIFMM_IsDevEnoughSpace(min_free_space, i))
        {
            result = TRUE;
            if (PNULL != file_dev_ptr)
            {
                *file_dev_ptr = i;
            }
            break;
        }
    }

    if(!result)
    {
        if(MMIAPIFMM_IsDevEnoughSpace(min_free_space, MMI_DEVICE_SYSTEM))
        {
            if (PNULL != file_dev_ptr)
            {
                *file_dev_ptr = MMI_DEVICE_SYSTEM;
            }       
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is need save wallpaper bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSaveWallpaperBitmap(void)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         cur_is_landscape = FALSE;
    BOOLEAN         init_is_landscape = FALSE;
    LCD_ANGLE_E     init_angle = LCD_ANGLE_0;

    //get current is landscape
    cur_is_landscape = MMITHEME_IsMainScreenLandscape();

    //get init
    init_angle = GUILCD_GetInitAngle(GUI_MAIN_LCD_ID);
    if ((LCD_ANGLE_90 == init_angle) ||
        (LCD_ANGLE_270 == init_angle))
    {
        init_is_landscape = TRUE;
    }

    if (init_is_landscape == cur_is_landscape)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : generate wallpaper file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GenerateWallpaperFile(
                                    const wchar                     *full_path_ptr, //in
                                    uint16                          full_path_len,  //in
                                    uint8                           *write_buf_ptr, //in
                                    uint32                          to_write_size,  //in
                                    MMIMULTIM_AS_WALLPAPER_INFO     *src_file_ptr   //in:src file name
                                    )
{
    BOOLEAN             result                                          = FALSE;
    uint8               size_time[MMIIDLE_WALLPAPER_SIZE_TIME_LEN+1]    = {0};
    uint16              size_time_len                                   = 0;
    uint32              i                                               = 0;
    uint32              writen_size                                     = 0;
    SFS_HANDLE          file_handle                                     = 0;
    SFS_FIND_DATA_T     find_data                                       = {0};

    if ((PNULL == full_path_ptr)
        || (PNULL == write_buf_ptr)
        || (0 == to_write_size)
        || (PNULL == src_file_ptr)
        )
    {
        return result;
    }

    if (0 == src_file_ptr->full_path_len)
    {
        return result;
    }

    //write wallpaper file
    file_handle = SFS_CreateFile(full_path_ptr,SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS,PNULL,PNULL);
    if(0 != file_handle)
    {
        if(SFS_ERROR_NONE == SFS_WriteFile(file_handle,(const void *)write_buf_ptr,to_write_size,&writen_size,PNULL))
        {
            //将原文件的size和creat time写入缩略图文件末尾
            if (GetFileSizeAndCreatTime(src_file_ptr->full_path_name,
                    src_file_ptr->full_path_len,
                    &find_data))
            {
                //set size
                size_time[i++] = (find_data.length >> 24) & 0x000000FF;
                size_time[i++] = (find_data.length >> 16) & 0x000000FF;
                size_time[i++] = (find_data.length >> 8) &  0x000000FF;
                size_time[i++] = find_data.length &  0x000000FF;

                //set creat time year
                size_time[i++] = (find_data.create_Date.year >> 8) & 0x00FF;
                size_time[i++] = find_data.create_Date.year & 0x00FF;

                //set creat time month
                size_time[i++] = find_data.create_Date.mon;

                //set creat time day
                size_time[i++] = find_data.create_Date.mday;

                //set creat time hour
                size_time[i++] = find_data.create_time.hour;

                //set creat time minute
                size_time[i++] = find_data.create_time.min;

                //set creat time second
                size_time[i++] = find_data.create_time.sec;

                SCI_ASSERT(i == MMIIDLE_WALLPAPER_SIZE_TIME_LEN);/*assert to do*/
                size_time_len = i;

                if(SFS_ERROR_NONE == SFS_WriteFile(file_handle,(const void *)size_time,size_time_len,&writen_size,PNULL))
                {
                    result = TRUE;
                }
                else
                {
                    //SCI_TRACE_LOW:"GenerateMiniatureFile: write src file size and creat time error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_962_112_2_18_3_6_29_6,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"GenerateWallpaperFile: get file size and creat timer error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_967_112_2_18_3_6_29_7,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"GenerateWallpaperFile: write wallpaper file error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_972_112_2_18_3_6_29_8,(uint8*)"");
        }
    }

    SFS_CloseFile(file_handle);

    return (result);
}

/*****************************************************************************/
//  Description : get file size and creat time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileSizeAndCreatTime(
                                      wchar             *full_path_ptr,     //in
                                      uint16            full_path_len,      //in
                                      SFS_FIND_DATA_T   *find_data_ptr      //in/out
                                      )
{
    BOOLEAN             result          = FALSE;
    SFS_HANDLE          file_handle     = {0};

    //SCI_ASSERT(PNULL != full_path_ptr);
    //SCI_ASSERT(0 < full_path_len);
    //SCI_ASSERT(PNULL != find_data_ptr);
    if((PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == find_data_ptr))
    {
        //SCI_TRACE_LOW:"GetFileSizeAndCreatTime param ERROR! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_999_112_2_18_3_6_29_9,(uint8*)"");
        return FALSE;
    }
    //get src file size and creat time
    file_handle = SFS_FindFirstFile(full_path_ptr,find_data_ptr);
    if (0 != file_handle)
    {
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_1010_112_2_18_3_6_29_10,(uint8*)"d",file_handle);
    }

    //close file
    SFS_FindClose(file_handle);

    return (result);
}

/*****************************************************************************/
//  Description : get wallpaper bitmap width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetWallpaperWidthHeight(
                                   MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T     *width_height_ptr
                                   )
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(PNULL != width_height_ptr);
    if(PNULL == width_height_ptr)
    {
        //SCI_TRACE_LOW:"GetWallpaperWidthHeight ERROR! PNULL == width_height_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_1032_112_2_18_3_6_29_11,(uint8*)"");
        return;
    }

    //get main menu style from nv
    MMINV_READ(MMINV_WALLPAPER_BITMAP_INFO,width_height_ptr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(width_height_ptr,0,sizeof(MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T));
        MMINV_WRITE(MMINV_WALLPAPER_BITMAP_INFO,width_height_ptr);
    }
}

/*****************************************************************************/
//  Description : get wallpaper bitmap width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaperWidthHeight(
                                   MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T     *width_height_ptr
                                   )
{
    //SCI_ASSERT(PNULL != width_height_ptr);
    if(PNULL == width_height_ptr)
    {
        //SCI_TRACE_LOW:"SetWallpaperWidthHeight ERROR! PNULL == width_height_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_1056_112_2_18_3_6_29_12,(uint8*)"");
        return;
    }

    //set main menu style
    MMINV_WRITE(MMINV_WALLPAPER_BITMAP_INFO,width_height_ptr);
}

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : NextLiveWallpaperIdFromBegining;
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL  int32  NextLiveWallpaperIdFromBegining()
{
    uint32 i = 0;
    int32 index_result = -1;
    LIVE_WALLPAPER_INFO lw_info = {0};

    SCI_TRACE_LOW("WALLPAPER: NextLiveWallpaperIdFromBegining();");
    
    //get next livewallpaper index.
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        MMI_LivewallpaperResListInit();
#endif

    for(;i<MMI_LIVE_WALLPAPER_TYPE_MAX;i++)
    {
	SCI_MEMSET(&lw_info, 0, sizeof(LIVE_WALLPAPER_INFO));
        if(MMIAPILIVEWALLPAPER_GetAllWallpapersInfo(&lw_info,i))
        {         
            if(lw_info.type < MMI_LIVE_WALLPAPER_TYPE_MAX)
            {
	        index_result = (int32)lw_info.type;
                break;   
            }
        }
    }
    
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
            MMI_DestroyLivewallpaperResList();
#endif

    return index_result;
}

/*****************************************************************************/
//  Description : NextLiveWallpaperIndexFromCurrent;
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL  int32  NextLiveWallpaperIndexFromCurrent()
{
    uint32 i = 0;
    int32 index_result = -1;
    LIVE_WALLPAPER_INFO lw_info = {0};
    MMISET_WALLPAPER_SET_T wallpaper_info      = {0};

    SCI_TRACE_LOW("WALLPAPER: NextLiveWallpaperIndexFromCurrent();");
    
    //Calculate current index of livewallpaper;
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if(MMISET_WALLPAPER_LIVE != wallpaper_info.wallpaper_type)
    {
        return index_result;
    }
    
    i= wallpaper_info.wallpaper_id+ 1;

        //get next livewallpaper index.
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        MMI_LivewallpaperResListInit();
#endif

    for(;i<MMI_LIVE_WALLPAPER_TYPE_MAX;i++)
    {
	SCI_MEMSET(&lw_info, 0, sizeof(LIVE_WALLPAPER_INFO));
        if(MMIAPILIVEWALLPAPER_GetAllWallpapersInfo(&lw_info,i))
        {         
            if(lw_info.type < MMI_LIVE_WALLPAPER_TYPE_MAX)
            {
	        index_result = (int32)lw_info.type;
                break;   
            }
        }
    }
    
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
            MMI_DestroyLivewallpaperResList();
#endif

    return index_result;
}
#endif

/*****************************************************************************/
//  Description : 重新获取墙纸信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  void  Get_Change_WallpaperInfo(MMISET_WALLPAPER_SET_T *wallpaper_info)
{
    //current is fixed wallpaper
	if(PNULL == wallpaper_info)
	{
		//SCI_TRACE_LOW:"[IDLE] Get_Change_WallpaperInfo wallpaper_info 0x%x !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_1074_112_2_18_3_6_29_13,(uint8*)"d", wallpaper_info);
		return;
	}
	SCI_TRACE_LOW("Get_Change_WallpaperInfo type = %d, id = %d",wallpaper_info->wallpaper_type,wallpaper_info->wallpaper_id);
	if(MMISET_WALLPAPER_DOWNLOAD == wallpaper_info->wallpaper_type)//from download wallpaper type;
	{
		SCI_TRACE_LOW("Get_Change_WallpaperInfo; from download type. type = %d, id = %d",wallpaper_info->wallpaper_type,wallpaper_info->wallpaper_id);
        
		wallpaper_info->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT 
     	if(-1 != NextLiveWallpaperIdFromBegining())
     	{
			//switch to first livewallpaper;
			wallpaper_info->wallpaper_type = MMISET_WALLPAPER_LIVE;
			wallpaper_info->wallpaper_id = NextLiveWallpaperIdFromBegining();    
		}
        else 
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
		if (MMIAPISETVWP_IsExistVWP())		
     	{
			wallpaper_info->wallpaper_type = MMISET_WALLPAPER_VIDEO; 
			wallpaper_info->wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);
		}
        else
#endif		
		{
			wallpaper_info->wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
		}
	}
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT  
    else if(MMISET_WALLPAPER_LIVE == wallpaper_info->wallpaper_type)//from livewallpaper type;
    {
        SCI_TRACE_LOW("Get_Change_WallpaperInfo; from livewallpaper type. type = %d, id = %d",wallpaper_info->wallpaper_type,wallpaper_info->wallpaper_id);
    
        if(-1 != NextLiveWallpaperIndexFromCurrent())
        {
            //switch to first livewallpaper;
            wallpaper_info->wallpaper_type = MMISET_WALLPAPER_LIVE;
            wallpaper_info->wallpaper_id = NextLiveWallpaperIndexFromCurrent();    
        }
#ifdef MMI_VIDEOWALLPAPER_SUPPORT   
        else if (MMIAPISETVWP_IsExistVWP())		
        {
	    	wallpaper_info->wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);
            wallpaper_info->wallpaper_type = MMISET_WALLPAPER_VIDEO; 
        }
#endif
        else
        {
        	//user defined wallpaper change to fixed wallpaper
        	wallpaper_info->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        	wallpaper_info->wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;   
        }
     }
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
	else if(MMISET_WALLPAPER_VIDEO == wallpaper_info->wallpaper_type)//from videowallpaper type;
	{
		SCI_TRACE_LOW("Get_Change_WallpaperInfo; from videowallpaper type. type = %d, id = %d",wallpaper_info->wallpaper_type,wallpaper_info->wallpaper_id);
    
		if (MMIAPISETVWP_IsExistNextVWP())
		{
			
		}
		else
		{
			MMIAPISETVWP_StopVideoWallpaper();
			wallpaper_info->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
			wallpaper_info->wallpaper_type = MMISET_WALLPAPER_FIXED_PIC; 
		}
	}
#endif
    else//from fixed wallpaper type;
    {
        //fixed wallpaper change to next fixed wallpaper
        SCI_TRACE_LOW("Get_Change_WallpaperInfo; from fixedwallpaper type. type = %d, id = %d",wallpaper_info->wallpaper_type,wallpaper_info->wallpaper_id);
        
        wallpaper_info->wallpaper_id++;
        if(wallpaper_info->wallpaper_id >= MMISET_WALLPAPER_DEFAULT_ID + MMISET_WALLPAPER_FIXED_PIC_MAX_NUM)
        {
        	//current wallpaper id is last id, change to first id or user defined wallpaper
        	wallpaper_info->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        	if(MMIAPIIDLE_IsExistWallpaperFile(PNULL))
        	{
        		wallpaper_info->wallpaper_type = MMISET_WALLPAPER_DOWNLOAD;
        	}   	
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT 
       		else if(-1 != NextLiveWallpaperIdFromBegining())
          	{
         		//switch to first livewallpaper;
            	wallpaper_info->wallpaper_type = MMISET_WALLPAPER_LIVE;
         		wallpaper_info->wallpaper_id = NextLiveWallpaperIdFromBegining();    
			}
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
           	else if (MMIAPISETVWP_IsExistVWP())		
           	{
				wallpaper_info->wallpaper_type = MMISET_WALLPAPER_VIDEO; 
				wallpaper_info->wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);
		 	}
#endif
		  	else
		  	{
				wallpaper_info->wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
		  	}
			SCI_TRACE_LOW("Get_Change_WallpaperInfo type = %d",wallpaper_info->wallpaper_type);
        }        
    }  
        //SCI_TRACE_LOW:"[IDLE] Get_Change_WallpaperInfo id 0x%x type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_WALLPAPER_1126_112_2_18_3_6_29_14,(uint8*)"dd", wallpaper_info->wallpaper_id, wallpaper_info->wallpaper_type);
}
/*****************************************************************************/
//  Description :却换墙纸
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_Change_Wallpaper(void)
{
    MMISET_WALLPAPER_SET_T          wallpaper_info      = {0};

    //获取当前墙纸的信息
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_Change_Wallpaper enter time = %d, ", 
	    SCI_GetTickCount(), wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);
	
	//墙纸转换
	Get_Change_WallpaperInfo(&wallpaper_info);
	//根据获取的墙纸信息重新设置墙纸
    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_Change_Wallpaper leave time = %d, ", 
	    SCI_GetTickCount(), wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);
}

/*****************************************************************************/
//  Description :check wall paper valid
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CheckWallPaperValid(void)
{
    SFS_DATE_T create_date = {0};
    SFS_TIME_T create_time = {0};
    BOOLEAN     result = FALSE;
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr = {0};

    if (MMIAPIIDLE_IsExistWallpaperFile(PNULL))
    {
        MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
        MMIAPIFMM_GetFileInfo(wallpaper_file_ptr.full_path_name, wallpaper_file_ptr.full_path_len, PNULL, &create_date, &create_time);
        if (create_date.mday == wallpaper_file_ptr.wallpaper_modify_date.mday 
            &&create_date.mon == wallpaper_file_ptr.wallpaper_modify_date.mon
            &&create_date.year == wallpaper_file_ptr.wallpaper_modify_date.year
            &&create_time.hour == wallpaper_file_ptr.wallpaper_modify_time.hour
            &&create_time.min ==  wallpaper_file_ptr.wallpaper_modify_time.min
            &&create_time.sec ==  wallpaper_file_ptr.wallpaper_modify_time.sec)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    
	SCI_TRACE_LOW("MMI_WALLPAPER MMIAPIIDLE_CheckWallPaperValid result = %d", result);
    return result;
}

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : 当前是否为动态墙纸
//  Global resource dependence : 
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsLiveWallpaper(void)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);

    return (MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type);
}
#endif
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
PUBLIC BOOLEAN MMIAPIIDLE_SetInitWallpaper_KL(
                                        BOOLEAN         is_save_bitmap, //only for wallpaper
                                        MMI_WIN_ID_T    win_id,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN disp_one_frame
                                        )
{
    BOOLEAN                         result              = FALSE;
    BOOLEAN                         is_gif              = FALSE;
    GUIANIM_RESULT_E                anim_result         = GUIANIM_RESULT_FAIL;
    GUIANIM_CTRL_INFO_T             control_info        = {0};
    GUIANIM_DATA_INFO_T             data_info           = {0};
    GUIANIM_FILE_INFO_T             file_info           = {0};
    GUIANIM_DISPLAY_INFO_T          display_info        = {0};
    MMISET_WALLPAPER_SET_T          wallpaper_info      = {0};
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr  = {0};
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMI_HANDLE_T ctrl_handle = 0;
#endif
    IGUICTRL_T *ctrl_ptr = 0;
    //CAF_GUID_T guid = 0;
    
    //set parameter
    control_info.is_wallpaper_ctrl = TRUE;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;

    if(disp_one_frame)
    {
        display_info.is_disp_one_frame = TRUE;
    }
    display_info.is_syn_decode = (BOOLEAN)(!is_save_bitmap);

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_save_wallpaper = is_save_bitmap;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_BLACK_COLOR;

    //get wallpaper info
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    
    if (MMIAPIUDISK_UdiskIsRun() && 
        (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type 
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        || MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type
#endif
        ))
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    if(MMISET_WALLPAPER_LIVE== wallpaper_info.wallpaper_type && MMIKL_CLOCK_DISP_WIN_ID == win_id)
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
    else
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    if(MMISET_WALLPAPER_VIDEO== wallpaper_info.wallpaper_type && MMIKL_CLOCK_DISP_WIN_ID == win_id)
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    if(MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
    {
        ctrl_ptr = MMK_GetCtrlPtrByWin(win_id, ctrl_id);
        if(ctrl_ptr != PNULL)
        {
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
            MMK_DestroyControl(ctrl_handle);
        }
    }
    else
#endif
    {
        ctrl_ptr = MMK_GetCtrlPtrByWin(win_id, ctrl_id);
        /* if wallpaper ctrl did not exist, we should create it */
        if(PNULL == ctrl_ptr)
        {
            CreatWallpaperCtrl(win_id, ctrl_id, FALSE);
        }
    }
    
    switch(wallpaper_info.wallpaper_type)
    {
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    case MMISET_WALLPAPER_VIDEO:
       data_info.img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());
       GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
       break;
#endif
    
    case MMISET_WALLPAPER_FIXED_PIC:
        //get image data buffer
        if (wallpaper_info.wallpaper_id >= MMISET_WALLPAPER_DEFAULT_ID)/*lint !e685 !e568*/
        {
            data_info.img_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);
            anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            if ((GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
            {
                result = TRUE;
            }
        }
        break;

    case MMISET_WALLPAPER_FIXED_ANIM:
        break;
    case MMISET_WALLPAPER_ALBUM:
    case MMISET_WALLPAPER_DOWNLOAD:
        if (MMIAPIIDLE_IsExistWallpaperFile(&is_gif))
        {
            if ((!is_gif) &&
                (GetWallpaperBitmap(&data_info.bitmap_data)))
            {
                //anim will free wallpaper
                data_info.is_bitmap = TRUE;
                data_info.is_free_bitmap = TRUE;
#ifdef MOTION_SENSOR_TYPE
                if (MMITHEME_IsSupportGsensor())
#endif
                {
                    //get file name info
                    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
                    file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                    file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                }
                anim_result = GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
            else
            {
                //free bitmap
                SCI_FREE(data_info.bitmap_data.bit_ptr);

                //get file name info
                MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

                file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                anim_result = GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
        }
        else
        {
            //reset default wallpaper
            MMIAPISET_ResetDefaultWallpaper();
        }
        break;

    default:
        //SCI_PASSERT(FALSE,("MMIAPIIDLE_SetInitWallpaper:wallpaper_type %d is error!",wallpaper_info.wallpaper_type));
        break;
    }

    return (result);
}

#endif
