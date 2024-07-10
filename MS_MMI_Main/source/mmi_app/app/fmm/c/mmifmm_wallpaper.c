/*****************************************************************************
** File Name:      mmifmm_wallpaper.c                                        *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMM_WALLPAPER_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_wallpaper.h"
#include "mmiset_export.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmimultim_text.h"
#include "mmifmm_nv.h"
#ifdef MMI_PIC_CROP_SUPPORT
#include "mmipic_crop_export.h"
#include "mmiset_id.h"
#include "mmiwidget_export.h"
#endif
#include "mmipub.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_wallpaper_export.h"
#include "mmilivewallpaper_export.h"
#endif
#include "mmi_appmsg.h"
#include "mmi_image.h"
#include "mmifmm_nv.h"
#include "mmi_modu_main.h"
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
#include "mmi_livewallpaper_slideshow.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIFMM_AS_WALLPAPER_INFO      s_file_as_wallpaper_info = {0}; //墙纸的信息
LOCAL MMISET_WALLPAPER_SET_T        s_pre_wallpaper = {0};          //前墙纸的信息
LOCAL BOOLEAN                       s_is_reset_pre_wallpaper = FALSE;//是否会赋之前的墙纸设置
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef MMI_PIC_CROP_SUPPORT
/*****************************************************************************/
//  Description : pic crop callback
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL void SetPictureToWallpaperCallBack (MMIPICCROP_CB_T* callback);
/*****************************************************************************/
//  Description : FmmSetPicToWallpaperCBK
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL void FmmSetPicToWallpaperCBK (MMIPICCROP_CB_T* callback);
#endif
/*****************************************************************************/
//  Description : 设置图片为墙纸,处理请稍候消息
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_HandleWallpaperWaitMsg (void)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};

    wallpaper_info.wallpaper_id   = MMISET_WALLPAPER_DEFAULT_ID;
    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_DOWNLOAD;

	MMIAPIFMM_SetMoreWallpaperNVByStyle(&s_file_as_wallpaper_info);
    MMIAPISET_SetWallpaperSetInfo (wallpaper_info);
}



/*****************************************************************************/
//  Description : set wallpaper,close waiting window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseWaitSetWallpaper (
    GUIANIM_RESULT_E     result
)
{
    LOCAL GUIANIM_RESULT_E  s_wallpaper_result = GUIANIM_RESULT_SUCC;
    MMI_WIN_ID_T            win_id = MMIMULTIM_SET_WALLPAPER_WAIT_WIN_ID;
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    if (MMK_IsOpenWin (win_id))
    {
        //prompt
        if (GUIANIM_RESULT_SUCC == result)
        {
            if (s_is_reset_pre_wallpaper)
            {
//                 switch (s_wallpaper_result)
//                 {
//                 case GUIANIM_RESULT_BIG:
//                     //prompt too big
//                     MMIPUB_OpenAlertWarningWin (TXT_MULTIM_PICTOO_LARGE);
//                     break;
// 
//                 case GUIANIM_RESULT_BIG_RESOLUTION:
//                     //prompt fail
//                     MMIPUB_OpenAlertWarningWin (TXT_RESOLUTION_TOO_BIG);
//                     break;
// 
//                 default:
                    //prompt fail
                    MMIPUB_OpenAlertFailWin (TXT_ERROR);
//                     break;
//                 }

                MMIAPISET_SetWallPaperName();//如果设置失败，则需要重新设置一下label的内容
            }
            else
            {
                //to write nv only when setting wallpaper success
                //MMINV_WRITE (MMINV_MULTIM_AS_WALLPAPER_INFO, &s_file_as_wallpaper_info);
                MMIAPIFMM_SetMoreWallpaperNVByStyle (&s_file_as_wallpaper_info);
                //prompt success
                //MMIPUB_OpenAlertSuccessWin (TXT_SUCCESS);
            }

            MMK_CloseWin (win_id);
        }
        else
        {
            if (s_is_reset_pre_wallpaper)
            {
                //错误墙纸反复恢复,会导致死循环.如果固定墙纸显示错误,或者设置不支持的图片为墙纸过程中断电
                //SCI_TRACE_LOW:"MMIAPIFMM_CloseWaitSetWallpaper: reset previous wallpaper error!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4641_112_2_18_2_20_15_245, (uint8*) "");
                //prompt fail
                MMIPUB_OpenAlertFailWin (TXT_ERROR);

                MMK_CloseWin (win_id);
            }
            else
            {
                s_wallpaper_result = result;
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT

                if (MMISET_WALLPAPER_LIVE == s_pre_wallpaper.wallpaper_type)
                {

                }
                else
#endif
                {

                    //恢复之前的墙纸
                    switch (s_pre_wallpaper.wallpaper_type)
                    {
                    case MMISET_WALLPAPER_FIXED_PIC:
                        s_is_reset_pre_wallpaper = TRUE;
                        MMIAPISET_SetWallpaperSetInfo (s_pre_wallpaper);
                        break;

                    case MMISET_WALLPAPER_FIXED_ANIM:
                        break;

                    case MMISET_WALLPAPER_ALBUM:
                    case MMISET_WALLPAPER_DOWNLOAD:
                        //to reset s_file_as_wallpaper_info read from nv while set wallpaper failed
                        /*MMINV_READ (MMINV_MULTIM_AS_WALLPAPER_INFO, &s_file_as_wallpaper_info, return_value);

                        if (MN_RETURN_SUCCESS != return_value) //force to initwallpaper if read nv failed
                        {
                            MMIAPIFMM_InitWallPaper();
                        }*/
                        MMIAPIFMM_GetMoreWallpaperNVByStyle(&s_file_as_wallpaper_info);

                        MMIAPIFMM_SetPictureToWallpaper (TRUE, s_file_as_wallpaper_info.full_path_name, s_file_as_wallpaper_info.full_path_len);
                        break;
                    default:
                        //SCI_PASSERT(FALSE,("MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!",s_pre_wallpaper.wallpaper_type));
                        //SCI_TRACE_LOW:"MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4676_112_2_18_2_20_15_246, (uint8*) "d", s_pre_wallpaper.wallpaper_type);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if ( (GUIANIM_RESULT_SUCC != result) &&
                (!s_is_reset_pre_wallpaper))
        {
            //恢复之前的墙纸
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
            MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
            MMIAPISET_GetWallpaperSetInfo (&wallpaper_info);

            if (!MMILiveWallpaper_NeedResetWallpaper (wallpaper_info.wallpaper_id))
            {
            }
            else
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
                if (MMISET_WALLPAPER_LIVE == s_pre_wallpaper.wallpaper_type)
                {

                }
                else
#endif
                {
                    switch (s_pre_wallpaper.wallpaper_type)
                    {
                    case MMISET_WALLPAPER_FIXED_PIC:
                        s_is_reset_pre_wallpaper = TRUE;
                        MMIAPISET_SetWallpaperSetInfo (s_pre_wallpaper);
                        break;

                    case MMISET_WALLPAPER_FIXED_ANIM:
                        break;

                    case MMISET_WALLPAPER_ALBUM:
                    case MMISET_WALLPAPER_DOWNLOAD:
                        //to reset s_file_as_wallpaper_info read from nv while set wallpaper failed
                        /*MMINV_READ (MMINV_MULTIM_AS_WALLPAPER_INFO, &s_file_as_wallpaper_info, return_value);

                        if (MN_RETURN_SUCCESS != return_value) //force to initwallpaper if read nv failed
                        {
                            MMIAPIFMM_InitWallPaper();
                        }*/
                        MMIAPIFMM_GetMoreWallpaperNVByStyle(&s_file_as_wallpaper_info);

                        MMIAPIFMM_SetPictureToWallpaper (TRUE, s_file_as_wallpaper_info.full_path_name, s_file_as_wallpaper_info.full_path_len);
                        MMK_CloseWin (win_id);
                        break;

                    default:
                        //SCI_PASSERT(FALSE,("MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!",s_pre_wallpaper.wallpaper_type));
                        //SCI_TRACE_LOW:"MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4715_112_2_18_2_20_15_247, (uint8*) "d", s_pre_wallpaper.wallpaper_type);
                        break;
                    }
                }
        }
    }
}



/*****************************************************************************/
//  Description : initialize NV about multim wallpaper
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitWallPaper (void)
{
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};


    MMIAPIFMM_GetMoreWallpaperNVByStyle(&s_file_as_wallpaper_info);

    SCI_MEMSET (&wallpaper_info, 0, sizeof (MMISET_WALLPAPER_SET_T));
    MMIAPISET_GetWallpaperSetInfo (&wallpaper_info);

    switch (wallpaper_info.wallpaper_type)
    {
    case MMISET_WALLPAPER_FIXED_PIC:
        MMIAPIFMM_ResetWallpaperSetting();
        break;

    case MMISET_WALLPAPER_DOWNLOAD:
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : change wallpaper file name
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ChangeWallPaperFileName (
    const wchar *full_path_ptr,
    uint16    full_path_len
)
{
    SFS_DATE_T			wallpaper_modify_date = {0};
    SFS_TIME_T			wallpaper_modify_time = {0};
    uint32  file_size = 0;

    //SCI_ASSERT(NULL != full_path_ptr);
    //SCI_ASSERT(0 < full_path_len);
    if ( (PNULL == full_path_ptr) || (0 == full_path_len))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_ChangeWallPaperFileName param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4772_112_2_18_2_20_15_248, (uint8*) "");
        return;
    }

    s_file_as_wallpaper_info.full_path_len = full_path_len;
    SCI_MEMSET (s_file_as_wallpaper_info.full_path_name, 0, sizeof (s_file_as_wallpaper_info.full_path_name));
    MMI_WSTRNCPY (s_file_as_wallpaper_info.full_path_name, MMIFILE_FULL_PATH_MAX_LEN,
                  full_path_ptr, full_path_len, full_path_len);
    MMIAPIFMM_GetFileInfo (s_file_as_wallpaper_info.full_path_name, s_file_as_wallpaper_info.full_path_len, &file_size, &wallpaper_modify_date, &wallpaper_modify_time);
    s_file_as_wallpaper_info.wallpaper_modify_date.mday = wallpaper_modify_date.mday;
    s_file_as_wallpaper_info.wallpaper_modify_date.mon = wallpaper_modify_date.mon;
    s_file_as_wallpaper_info.wallpaper_modify_date.year = wallpaper_modify_date.year;
    s_file_as_wallpaper_info.wallpaper_modify_time.hour = wallpaper_modify_time.hour;
    s_file_as_wallpaper_info.wallpaper_modify_time.min = wallpaper_modify_time.min;
    s_file_as_wallpaper_info.wallpaper_modify_time.sec = wallpaper_modify_time.sec;

    //MMINV_WRITE (MMINV_MULTIM_AS_WALLPAPER_INFO, &s_file_as_wallpaper_info);
    MMIAPIFMM_SetMoreWallpaperNVByStyle(&s_file_as_wallpaper_info);
}

/*****************************************************************************/
//  Description : write wallpaper file info NV
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_WriteWallPaperNV (
    MMIMULTIM_AS_WALLPAPER_INFO  *wallpaper_file_ptr
)
{
    //MMINV_WRITE (MMINV_MULTIM_AS_WALLPAPER_INFO, wallpaper_file_ptr);
    MMIAPIFMM_SetMoreWallpaperNVByStyle(wallpaper_file_ptr);
}

/*****************************************************************************/
//  Description : is file wallpaper file
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsWallPaperFile (
    const wchar *full_path_ptr,
    uint16    full_path_len
)
{
    //SCI_ASSERT(NULL != full_path_ptr);
    //SCI_ASSERT(0 < full_path_len);
    if ( (PNULL == full_path_ptr) || (0 == full_path_len))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_IsWallPaperFile param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4815_112_2_18_2_20_15_249, (uint8*) "");
        return FALSE;
    }

    if (0 == MMIAPICOM_CompareTwoWstr (full_path_ptr, full_path_len,
                                       s_file_as_wallpaper_info.full_path_name,
                                       s_file_as_wallpaper_info.full_path_len))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 设置图片为墙纸
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaper (
    BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
    const wchar  *full_path_ptr,
    uint16       full_path_len
)
{
    BOOLEAN     result = FALSE;
    SFS_DATE_T			wallpaper_modify_date = {0};
    SFS_TIME_T			wallpaper_modify_time = {0};
    uint32  file_size = 0;
    MMIFMM_AS_WALLPAPER_INFO   pre_wallpaper_info = {0};

    //SCI_ASSERT(NULL != full_path_ptr);
    //SCI_ASSERT(0 < full_path_len);
    if ( (PNULL == full_path_ptr) || (0 == full_path_len))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaper param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4849_112_2_18_2_20_15_250, (uint8*) "");
        return FALSE;
    }

    s_is_reset_pre_wallpaper = is_pre_wallpaper;

    if (!is_pre_wallpaper)
    {
        //save previous wallpaper file name
        SCI_MEMSET (&pre_wallpaper_info, 0, sizeof (MMIMULTIM_AS_WALLPAPER_INFO));
        MMI_MEMCPY (&pre_wallpaper_info, sizeof (MMIMULTIM_AS_WALLPAPER_INFO),
                    &s_file_as_wallpaper_info, sizeof (MMIMULTIM_AS_WALLPAPER_INFO),
                    sizeof (MMIMULTIM_AS_WALLPAPER_INFO));
    }

    //set previous wallpaper info
    MMIAPISET_GetWallpaperSetInfo (&s_pre_wallpaper);

    //set global variable
    if (!is_pre_wallpaper) //here must not reset s_file_as_wallpaper_info for its value read from nv already
    {
        SCI_MEMSET (s_file_as_wallpaper_info.full_path_name, 0, sizeof (s_file_as_wallpaper_info.full_path_name));
        s_file_as_wallpaper_info.full_path_len = full_path_len;
        MMI_WSTRNCPY (s_file_as_wallpaper_info.full_path_name,  MMIFILE_FULL_PATH_MAX_LEN,
                      full_path_ptr, full_path_len, full_path_len);
    }

    if (MMIAPIFMM_GetFileInfo (s_file_as_wallpaper_info.full_path_name,
                               s_file_as_wallpaper_info.full_path_len,
                               &file_size,
                               &wallpaper_modify_date,
                               &wallpaper_modify_time))
    {
        s_file_as_wallpaper_info.wallpaper_modify_date.mday = wallpaper_modify_date.mday;
        s_file_as_wallpaper_info.wallpaper_modify_date.mon = wallpaper_modify_date.mon;
        s_file_as_wallpaper_info.wallpaper_modify_date.year = wallpaper_modify_date.year;
        s_file_as_wallpaper_info.wallpaper_modify_time.hour = wallpaper_modify_time.hour;
        s_file_as_wallpaper_info.wallpaper_modify_time.min = wallpaper_modify_time.min;
        s_file_as_wallpaper_info.wallpaper_modify_time.sec = wallpaper_modify_time.sec;
        //MMINV_WRITE(MMINV_MULTIM_AS_WALLPAPER_INFO,&s_file_as_wallpaper_info);

        //prompt waiting
        MMIAPIFMM_OpenWaitSetWallpaper();

        MMK_PostMsg (VIRTUAL_WIN_ID, MSG_SET_WALLPAPER_WAIT, PNULL, 0);

        result = TRUE;
    }
    else
    {
        if (!is_pre_wallpaper)
        {
            //save previous wallpaper file name
            SCI_MEMSET (&s_file_as_wallpaper_info, 0, sizeof (MMIMULTIM_AS_WALLPAPER_INFO));
            MMI_MEMCPY (&s_file_as_wallpaper_info, sizeof (MMIMULTIM_AS_WALLPAPER_INFO),
                        &pre_wallpaper_info, sizeof (MMIMULTIM_AS_WALLPAPER_INFO),
                        sizeof (MMIMULTIM_AS_WALLPAPER_INFO));
        }

        //prompt fail
        MMIPUB_OpenAlertFailWin (TXT_ERROR);

        //SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaper:get file info fail!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4906_112_2_18_2_20_15_251, (uint8*) "");
        result = FALSE;
    }

    return (result);
}


#ifdef MMI_PIC_CROP_SUPPORT
/*****************************************************************************/
//  Description : pic crop callback
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL void SetPictureToWallpaperCallBack (MMIPICCROP_CB_T* callback)
{
    if (PNULL != callback)
    {
        switch (callback->err_type)
        {
        case MMI_PICCROP_SUCCESS:
        case MMI_PICCROP_SUCCESS_NO_CROP:
        case MMI_PICCROP_ERR_NOT_SUPPORT://格式不支持时直接设为墙纸，路径为传入的路径
            MMIAPIFMM_SetPictureToWallpaper (callback->is_pre_wallpaper, callback->full_path_ptr, callback->full_path_len);
            break;

        default:
            break;
        }
    }
}
/*****************************************************************************/
//  Description : FmmSetPicToWallpaperCBK
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL void FmmSetPicToWallpaperCBK (MMIPICCROP_CB_T* callback)
{
    MMI_STRING_T wallpaper_label_name = {0};

    if (PNULL != callback)
    {
        switch (callback->err_type)
        {
        case MMI_PICCROP_SUCCESS:
        case MMI_PICCROP_SUCCESS_NO_CROP:
        case MMI_PICCROP_ERR_NOT_SUPPORT://格式不支持时直接设为墙纸，路径为传入的路径
        case MMI_PICCROP_ERR_NO_MEMORY:
        case MMI_PICCROP_ERR_NO_SPACE:

            if (MMIAPIFMM_SetPictureToWallpaper (callback->is_pre_wallpaper, callback->full_path_ptr, callback->full_path_len))
            {
                wallpaper_label_name.wstr_ptr = callback->full_path_ptr;
                wallpaper_label_name.wstr_len = callback->full_path_len;
                GUILABEL_SetText (MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }

                if (MMK_IsOpenWin (MMISET_SET_WALLPAPER_WIN_ID))
                {
                    MMK_CloseWin (MMISET_SET_WALLPAPER_WIN_ID);
                }
            }
            else
            {
                //prompt fail
                MMIPUB_OpenAlertFailWin (TXT_ERROR);
            }

            break;

        default:
            break;
        }

    }
}

/*****************************************************************************/
//  Description : FmmSetPicToPowerAndSaverCBK
//  Global resource dependence : none
//  Author: liyan.zhu
//  Note:copy FmmSetPicToWallpaperCBK
/*****************************************************************************/
LOCAL void FmmSetPicToPowerAndSaverCBK(MMIPICCROP_CB_T* callback)
{
	MMI_WIN_ID_T set_win_id  = 0;
	MMIFMM_SELECT_RETURN_T          selected_info = {0};
	
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
	if (MMIAPISET_IsPoweronCall())
	{
		SCI_TRACE_LOW("[mmifmm.c] FmmSetPicToPowerAndSaverCBK MMIAPISET_IsPoweronCall");
		set_win_id = MMISET_SET_POWER_ON_ANIM_WIN_ID;
	}
	else if(MMIAPISET_IsPoweroffCall())	
	{
		SCI_TRACE_LOW("[mmifmm.c] FmmSetPicToPowerAndSaverCBKMMIAPISET_IsPoweroffCall");
		set_win_id = MMISET_SET_POWER_OFF_ANIM_WIN_ID;
	}
#endif
#ifdef SCREENSAVER_SUPPORT	
	if (MMIAPISET_IsScreensaverCall())			
	{
		SCI_TRACE_LOW("[mmifmm.c] FmmSetPicToPowerAndSaverCBK MMIAPISET_IsScreensaverCall");
		set_win_id = MMISET_SET_SCREENSAVER_WIN_ID;
	}
#endif
	if (0!=set_win_id)
	{
		if(PNULL != callback)
		{
			SCI_TRACE_LOW("[mmifmm.c] FmmSetPicToPowerAndSaverCBK  callback->err_type = %d",callback->err_type);
			switch(callback->err_type)
			{
			case MMI_PICCROP_SUCCESS:
			case MMI_PICCROP_SUCCESS_NO_CROP:
			case MMI_PICCROP_ERR_NOT_SUPPORT://格式不支持时直接设为墙纸，路径为传入的路径
			case MMI_PICCROP_ERR_NO_MEMORY:
			case MMI_PICCROP_ERR_NO_SPACE:
				MMIAPICOM_Wstrncpy(selected_info.file_name_ptr,callback->full_path_ptr,callback->full_path_len);
				//selected_info.file_name_ptr = callback->full_path_ptr;
				selected_info.file_name_len = callback->full_path_len;
				selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
				MMK_SendMsg(set_win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
				
				if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
				{
					MMK_CloseParentWin(MMIFMM_OPEN_FILE_WIN_ID); 
				}
				if(MMK_IsOpenWin(MMISET_SET_WALLPAPER_WIN_ID))
				{
					MMK_CloseWin(MMISET_SET_WALLPAPER_WIN_ID);
				}
				break;
				
			default:
				break;
			}
			
		}
	}
    
}

/*****************************************************************************/
//  Description : 设置图片为开关机动画/屏保?如果有图片裁剪，并格式支持，则先进入图片裁剪
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note: copy MMIAPIFMM_SetPictureToWallpaperEx
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToPowerAndSaverEx(
													 //BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
													 const wchar  *full_path_ptr,
													 uint16       full_path_len
													 )
{   
	uint16  lcd_width = 0;
	uint16  lcd_height = 0;
    MMIPICCROP_SRC_T crop_src = {0};
	
    if(PNULL == full_path_ptr)
	{
		//SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaperEx full_path_ptr is PNULL !!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_4950_112_2_18_2_20_15_252,(uint8*)"");
		return FALSE;
	}
    if(0 == full_path_len)
	{
		//SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaperEx full_path_len is PNULL !!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_4955_112_2_18_2_20_15_253,(uint8*)"");
		return FALSE;
	}
	
	
	
    crop_src.crop_type = MMIPIC_CROP_WALLPAPER;
	
    crop_src.callback = FmmSetPicToPowerAndSaverCBK;
    crop_src.full_path_ptr = (wchar*)full_path_ptr;
    crop_src.full_path_len = full_path_len;
	
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
#ifdef GRID_SLIDE_WALLPAPER
    crop_src.min_width = lcd_width * 2;
#else        
    crop_src.min_width = lcd_width;
#endif
    crop_src.min_height = lcd_height;
    if (MMIAPIPICCROP_OpenPicCrop(&crop_src))
	{
		return TRUE;
	}
    else
	{
		return FALSE;
	}
}
#endif

/*****************************************************************************/
//  Description : 设置图片为墙纸,如果有图片裁剪，并格式支持，则先进入图片裁剪
//  Global resource dependence :
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaperEx (
    BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
    const wchar  *full_path_ptr,
    uint16       full_path_len
)
{
    BOOLEAN result = TRUE;

    if (PNULL == full_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaperEx full_path_ptr is PNULL !!!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4950_112_2_18_2_20_15_252, (uint8*) "");
        return FALSE;
    }

    if (0 == full_path_len)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_SetPictureToWallpaperEx full_path_len is PNULL !!!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4955_112_2_18_2_20_15_253, (uint8*) "");
        return FALSE;
    }

#ifdef MMI_PIC_CROP_SUPPORT
    //if(GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(full_path_ptr,full_path_len))
    {
        uint16  lcd_width = 0;
        uint16  lcd_height = 0;
        MMIPICCROP_SRC_T crop_src = {0};
        uint16  wallpaper_layer_size = 1;

        crop_src.crop_type = MMIPIC_CROP_WALLPAPER;

        if (MMIAPISET_IsWallpaperCall())
        {
            crop_src.callback = FmmSetPicToWallpaperCBK;
        }
        else
        {
            crop_src.callback = SetPictureToWallpaperCallBack;
        }

        crop_src.full_path_ptr = (wchar*) full_path_ptr;
        crop_src.full_path_len = full_path_len;

        GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
//#ifdef GRID_SLIDE_WALLPAPER
        crop_src.min_width = lcd_width * wallpaper_layer_size;
//#else
//        crop_src.min_width = lcd_width;
//#endif
        crop_src.min_height = lcd_height;
        MMIAPIPICCROP_OpenPicCrop (&crop_src);
    }
    //else
#else
    {
        result = MMIAPIFMM_SetPictureToWallpaper (is_pre_wallpaper, full_path_ptr, full_path_len);
    }

#endif
    return result;
}


/*****************************************************************************/
//  Description : reset pic wallpaper setting
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ResetWallpaperSetting (void)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};

    SCI_MEMSET (&wallpaper_info, 0, sizeof (MMISET_WALLPAPER_SET_T));

    //reset wallpaper name
    SCI_MEMSET (&s_file_as_wallpaper_info, 0, sizeof (MMIMULTIM_AS_WALLPAPER_INFO));
    MMIAPIFMM_SetMoreWallpaperNVByStyle (&s_file_as_wallpaper_info);

    MMIAPISET_GetWallpaperSetInfo (&wallpaper_info);

    if (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type)
    {
        wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
        MMIAPISET_SetWallpaperSetInfo (wallpaper_info);
    }
}

/*****************************************************************************/
//  Description : set wallpaper,open waiting window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenWaitSetWallpaper (void)
{
    uint32          time_out = 0;
    MMI_STRING_T    wait_text = {0};
    MMI_WIN_ID_T    win_id = MMIMULTIM_SET_WALLPAPER_WAIT_WIN_ID;

    //进入等待界面,必须是单张图片,否则低优先级别的图片decode无法得到时间运行
    MMI_GetLabelTextByLang (TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenAlertWinByTextId (&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT,
                                 &win_id, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
}

/*****************************************************************************/
//  Description : get wallpaper information
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetWallPaperInfo (MMIMULTIM_AS_WALLPAPER_INFO *wallpaper_file_ptr)
{
    //return &s_file_as_wallpaper_info;
    MMIAPIFMM_GetMoreWallpaperNVByStyle(wallpaper_file_ptr);
}

/*****************************************************************************/
//Description : Set default system's more_wallpaperNV informatioin;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetDefaultMoreWallpaperNV(void)
{
    uint32 i = 0;
    MMIFMM_AS_WALLPAPER_INFO wallpaper_info = {0};

    for(i = 0; i < MMIFMM_WALLPAPER_STYLE; i++)
    {  
        SCI_MEMSET(&wallpaper_info, 0x00, sizeof(MMIFMM_AS_WALLPAPER_INFO));
        MMINV_WRITE(MMINV_MULTIM_AS_WALLPAPER_INFO + i, &wallpaper_info);
    }
}

/*****************************************************************************/
//Description : Get more_wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetMoreWallpaperNVByStyle(MMIFMM_AS_WALLPAPER_INFO* wallpaper_info_ptr)
{
    uint32 return_value = MN_RETURN_FAILURE;
    MMISET_IDLE_STYLE_E  idle_style = MMIAPISET_GetIdleStyle();
    MMISET_IDLE_SUBSTYLE_E idle_substyle = MMIAPISET_GetIdleSubStyle();
    MMIFMM_AS_WALLPAPER_INFO wallpaper_info = {0};
    uint8  wallpaper_style = 0;

    SCI_TRACE_LOW("MMIAPIFMM_GetMoreWallpaperNVByStyle: idle_style = %d,idle_substyle = %d",idle_style,idle_substyle);
    
#ifdef MMI_DIFF_WALLPAPER_STYLE
    if(idle_style >= MMIFMM_WALLPAPER_STYLE)
    {
        SCI_TRACE_LOW("MMIAPIFMM_GetMoreWallpaperNVByStyle error idle_style = %d",idle_style);
        return FALSE;
    }
    
    if ((idle_style < MMISET_IDLE_STYLE_MAX) 
#ifdef MMI_SAM_THEME    
        && (MMISET_IDLE_SUBSTYLE_SAM != idle_substyle)
#endif
        )
    {
        wallpaper_style = idle_style;
    }
#ifdef MMI_SAM_THEME
    else if ((MMISET_IDLE_STYLE_GRID == idle_style) && (MMISET_IDLE_SUBSTYLE_SAM == idle_substyle))
    {  
        wallpaper_style = MMISET_IDLE_STYLE_MAX;
    }
#endif
#endif//MMI_DIFF_WALLPAPER_STYLE

    MMINV_READ(MMINV_MULTIM_AS_WALLPAPER_INFO + wallpaper_style, &wallpaper_info, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIFMM_SetDefaultMoreWallpaperNV();

        MMINV_READ(MMINV_MULTIM_AS_WALLPAPER_INFO + wallpaper_style, &wallpaper_info, return_value);
    }
    
    wallpaper_info_ptr->full_path_len = wallpaper_info.full_path_len;
    MMI_WSTRNCPY (wallpaper_info_ptr->full_path_name, MMIFILE_FULL_PATH_MAX_LEN,
                  wallpaper_info.full_path_name, wallpaper_info.full_path_len, 
                  wallpaper_info.full_path_len);
    wallpaper_info_ptr->wallpaper_modify_date = wallpaper_info.wallpaper_modify_date;
    wallpaper_info_ptr->wallpaper_modify_time = wallpaper_info.wallpaper_modify_time;

    return TRUE;
}

/*****************************************************************************/
//Description : Set multi_wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetMoreWallpaperNVByStyle(MMIFMM_AS_WALLPAPER_INFO* wallpaper_info_ptr)
{
    uint32 return_value = MN_RETURN_FAILURE;
    MMISET_IDLE_STYLE_E  idle_style = MMIAPISET_GetIdleStyle();
    MMISET_IDLE_SUBSTYLE_E idle_substyle = MMIAPISET_GetIdleSubStyle();
    MMIFMM_AS_WALLPAPER_INFO wallpaper_info = {0};
    uint8  wallpaper_style = 0;

    SCI_TRACE_LOW("MMIAPIFMM_SetMoreWallpaperNVByStyle: idle_style = %d,idle_substyle = %d",idle_style,idle_substyle);

#ifdef MMI_DIFF_WALLPAPER_STYLE
    if(idle_style >= (MMISET_IDLE_STYLE_MAX+1))
    {
        SCI_TRACE_LOW("MMIAPIFMM_SetMoreWallpaperNVByStyle error idle_style = %d",idle_style);
        return FALSE;
    }

    if ((idle_style < MMISET_IDLE_STYLE_MAX) 
#ifdef MMI_SAM_THEME    
        && (MMISET_IDLE_SUBSTYLE_SAM != idle_substyle)
#endif
        )
    {
        wallpaper_style = idle_style;
    }
#ifdef MMI_SAM_THEME
    else if ((MMISET_IDLE_STYLE_GRID == idle_style) && (MMISET_IDLE_SUBSTYLE_SAM == idle_substyle))
    {  
        wallpaper_style = MMISET_IDLE_STYLE_MAX;
    }
#endif
#endif//MMI_DIFF_WALLPAPER_STYLE

    MMINV_READ(MMINV_MULTIM_AS_WALLPAPER_INFO + wallpaper_style, &wallpaper_info, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIFMM_SetDefaultMoreWallpaperNV();

        MMINV_READ(MMINV_MULTIM_AS_WALLPAPER_INFO + wallpaper_style, &wallpaper_info, return_value);
    }

    SCI_MEMSET(&wallpaper_info, 0x00, sizeof(MMIFMM_AS_WALLPAPER_INFO));
    wallpaper_info.full_path_len = wallpaper_info_ptr->full_path_len;
    MMI_WSTRNCPY (wallpaper_info.full_path_name, MMIFILE_FULL_PATH_MAX_LEN,
                  wallpaper_info_ptr->full_path_name, wallpaper_info_ptr->full_path_len, wallpaper_info_ptr->full_path_len);
    wallpaper_info.wallpaper_modify_date = wallpaper_info_ptr->wallpaper_modify_date;
    wallpaper_info.wallpaper_modify_time = wallpaper_info_ptr->wallpaper_modify_time;

    MMINV_WRITE(MMINV_MULTIM_AS_WALLPAPER_INFO + wallpaper_style, &wallpaper_info);
    return TRUE; 
}

