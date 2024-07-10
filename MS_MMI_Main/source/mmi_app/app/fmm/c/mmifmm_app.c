/*****************************************************************************
** File Name:      mmifmm_app.c                                              *
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

#define _MMIFMM_APP_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_app.h"
#include "mmifmm_videowin.h"
#include "mmifmm_picwin.h"
#include "mmifmm_detailwin.h"
#include "mmifmm_nv.h"
#include "mmifmm_id.h"
#include "mmifmm_interface.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "guitab.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmifmm_image.h"
#include "mmifmm_mainwin.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmifmm_findfilewin.h"
#include "mmifmm_createwin.h"
#include "mmifmm_renamewin.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiacc_id.h"
#include "mmifmm_movewin.h"
#include "mmifmm_deletewin.h"
#include "mmifmm_copywin.h"

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmimms_export.h"
#include "mmifmm_sort.h"
#include "mmifmm_comfunc.h"
#include "mmimp3_export.h"
#include "mmibt_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmifmm_internal.h"
#include "mmifmm_sd.h"
#include "mmifmm_mem.h"
#include "mmienvset_export.h"
#include "mmifmm_menutable.h"
#include "mmialarm_export.h"
#include "mmifmm_pic.h"
#ifdef CALENDAR_SUPPORT
#include "mmischedule_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#if defined (BLUETOOTH_SUPPORT)
#include "mmifmm_text.h"
#endif
//#include "Ctrllistbox.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_VCARD_MAX_8BIT_SPLIT_LEN (128*MMISMS_SPLIT_MAX_NUM)




//fixed by liyan.zhu for bugzilla 2760
#ifdef AAC_SUPPORT
#define MMIFMM_IS_AAC_SUPPORT            TRUE
#else
#define MMIFMM_IS_AAC_SUPPORT            FALSE
#endif

#ifdef WMA_SUPPORT
#define MMIFMM_IS_WMA_SUPPORT            TRUE
#else
#define MMIFMM_IS_WMA_SUPPORT            FALSE
#endif

#ifdef AMR_SUPPORT
#define MMIFMM_IS_AMR_SUPPORT            TRUE
#else
#define MMIFMM_IS_AMR_SUPPORT            FALSE
#endif

#if defined MIDI_SUPPORT && !defined MMI_SOUNDBANK_MODE_COMPACT
#define MMIFMM_IS_MID_SUPPORT            TRUE
#else
#define MMIFMM_IS_MID_SUPPORT            FALSE
#endif

#ifdef MP3_SUPPORT
#define MMIFMM_IS_MP3_SUPPORT            TRUE
#else
#define MMIFMM_IS_MP3_SUPPORT            FALSE
#endif

#ifdef WAV_SUPPORT
#define MMIFMM_IS_WAV_SUPPORT            TRUE
#else
#define MMIFMM_IS_WAV_SUPPORT            FALSE
#endif
//macro replace
//#ifdef MMIMPEG4_AVI_ENABLE
#ifdef AVI_DEC_SUPPORT
#define MMIFMM_IS_AVI_SUPPORT            TRUE
#else
#define MMIFMM_IS_AVI_SUPPORT            FALSE
#endif

#ifdef MMI_RMVB_SUPPORT
#define MMIFMM_IS_RMVB_SUPPORT            TRUE
#else
#define MMIFMM_IS_RMVB_SUPPORT            FALSE
#endif

#ifdef FLV_DEC_SUPPORT
#define MMIFMM_IS_FLV_SUPPORT            TRUE
#else
#define MMIFMM_IS_FLV_SUPPORT            FALSE
#endif

#ifdef CMMB_SUPPORT
#define MMIFMM_IS_MTV_SUPPORT            TRUE
#else
#define MMIFMM_IS_MTV_SUPPORT            FALSE
#endif

#ifdef JAVA_SUPPORT
#define MMIFMM_IS_JAVA_SUPPORT            TRUE
#else
#define MMIFMM_IS_JAVA_SUPPORT            FALSE
#endif

#ifdef QBTHEME_SUPPORT
#define MMIFMM_IS_QBTHEME_SUPPORT			TRUE
#else
#define MMIFMM_IS_QBTHEME_SUPPORT			FALSE
#endif

#ifdef BROWSER_SUPPORT_NETFRONT
#define MMIFMM_IS_HTM_SUPPORT               TRUE
#define MMIFMM_IS_HTML_SUPPORT              TRUE
#define MMIFMM_IS_XML_SUPPORT               TRUE

#else
#define MMIFMM_IS_HTM_SUPPORT               FALSE
#define MMIFMM_IS_HTML_SUPPORT              FALSE
#define MMIFMM_IS_XML_SUPPORT               FALSE
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint8   suffix[MMIFMM_FILE_SUFFIX_MAX_LEN];
    uint32  type;
    BOOLEAN is_support;
} MMIFMM_SUFFIX_T;

LOCAL const MMIFMM_SUFFIX_T s_fmm_all_suffix[] =
{   {"jpg",     MMIFMM_PICTURE_JPG, TRUE},
    {"gif",     MMIFMM_PICTURE_GIF, TRUE},
    {"bmp",     MMIFMM_PICTURE_BMP, TRUE},
    {"wbmp",    MMIFMM_PICTURE_WBMP, TRUE},
#ifdef PNG_DEC_SUPPORT //原因mini工程不支持png'
    {"png",     MMIFMM_PICTURE_PNG, TRUE},
#endif
    {"jpeg",    MMIFMM_PICTURE_JPG, TRUE},
#ifdef DRM_SUPPORT
    {"dm",      MMIFMM_DM_FILE,      TRUE},
    {"dcf",     MMIFMM_DCF_FILE,     TRUE},
#endif
    {"mp3",     MMIFMM_MUSIC_MP3,   MMIFMM_IS_MP3_SUPPORT},
    //@CR241798 2011.05.30
    {"mp2",     MMIFMM_MUSIC_MP3,   MMIFMM_IS_MP3_SUPPORT},
    {"mp1",     MMIFMM_MUSIC_MP3,   MMIFMM_IS_MP3_SUPPORT},
    //@CR241798 2011.05.30
    {"wma",     MMIFMM_MUSIC_WMA,   MMIFMM_IS_WMA_SUPPORT},
    {"mid",     MMIFMM_MUSIC_MID,   MMIFMM_IS_MID_SUPPORT},
    {"amr",     MMIFMM_MUSIC_AMR,   MMIFMM_IS_AMR_SUPPORT},
    {"aac",     MMIFMM_MUSIC_ACC,   MMIFMM_IS_AAC_SUPPORT},
    {"m4a",     MMIFMM_MUSIC_M4A,   MMIFMM_IS_AAC_SUPPORT},
    {"wav",     MMIFMM_MUSIC_WAV,   MMIFMM_IS_WAV_SUPPORT},
    {"midi",    MMIFMM_MUSIC_MIDI,  MMIFMM_IS_MID_SUPPORT},
    {"rm",      MMIFMM_MOVIE_RMVB,  MMIFMM_IS_RMVB_SUPPORT},
    {"rmvb",    MMIFMM_MOVIE_RMVB,  MMIFMM_IS_RMVB_SUPPORT},
    {"rv",      MMIFMM_MOVIE_RMVB,  MMIFMM_IS_RMVB_SUPPORT},
    {"flv",     MMIFMM_MOVIE_FLV,   MMIFMM_IS_FLV_SUPPORT},
    {"mp4",     MMIFMM_MOVIE_MP4,   TRUE},
    {"3gp",     MMIFMM_MOVIE_3GP,   TRUE},
#ifdef MMI_KING_MOVIE_SUPPORT
    {"kmv",     MMIFMM_MOVIE_KMV,   TRUE},
    {"smv",     MMIFMM_MOVIE_SMV,   TRUE},//smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    {"hmv",     MMIFMM_MOVIE_HMV,   TRUE},//hmv_support
#endif
#endif
    {"txt",     MMIFMM_MOVIE_TXT,   TRUE},
    {"lrc",     MMIFMM_MOVIE_LRC,   TRUE},
    {"szip",    MMIFMM_MOVIE_SZIP,  TRUE},
#if defined MMI_VCARD_SUPPORT
    {"vcf",     MMIFMM_MOVIE_VCF,   TRUE},
#endif
    {"mtv",     MMIFMM_MOVIE_MTV,   MMIFMM_IS_MTV_SUPPORT},
    {"avi",     MMIFMM_MOVIE_AVI,   MMIFMM_IS_AVI_SUPPORT},
    {"jad",     MMIFMM_JAVA_JAD,    MMIFMM_IS_JAVA_SUPPORT},
    {"jar",     MMIFMM_JAVA_JAR,    MMIFMM_IS_JAVA_SUPPORT},
#ifdef QBTHEME_SUPPORT
    {"qbt",     MMIFMM_THEME_QB,    MMIFMM_IS_QBTHEME_SUPPORT},
#endif
    {"htm",     MMIFMM_BROWSER_HTM,    MMIFMM_IS_HTM_SUPPORT},
    {"html",     MMIFMM_BROWSER_HTML,    MMIFMM_IS_HTML_SUPPORT},
    {"xml",     MMIFMM_BROWSER_XML,    MMIFMM_IS_XML_SUPPORT},
    {"lib",     MMIFMM_FONT,        TRUE},
#ifdef MMI_VCALENDAR_SUPPORT
    {"vcs",     MMIFMM_MOVIE_VCS,   TRUE},
#endif
#ifdef MRAPP_SUPPORT
    {"mrp",     MMIFMM_MRAPP_MRP,   TRUE},
#endif
#ifdef HERO_ENGINE_SUPPORT 
    {"too",     MMIFMM_HERO_TOO,   TRUE},
#endif
#ifdef BROWSER_SUPPORT
    {"url",     MMIFMM_BROWSER_URL,    TRUE},
#endif    
    {0,}, 		/*lint !e651*/
};


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetIconIdByFileName (
    const wchar        *file_name,
    uint16     file_name_len
);
/*****************************************************************************/
//  Description : init multim folder
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitMultimFolder (
    const wchar   *device_ptr,
    uint16  device_len,
    const wchar   *dir_ptr,
    uint16  dir_len
);
#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : open url file by browser
//  Global resource dependence :
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void OpenUrlFileByBrowserEx(
                            wchar *full_file_name_ptr
                            );
#endif

/*****************************************************************************/
//  Description : initialize FMM module
//  Global resource dependence : none
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitModule (void)
{
    //register menu
    MMIFMM_RegMenuGroup();
    MMIFMM_RegFMMNv();
    MMIFMM_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : init multim folder
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitMultimFolder (
    const wchar   *device_ptr,
    uint16  device_len,
    const wchar   *dir_ptr,
    uint16  dir_len
)
{
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;

    if ( (PNULL == device_ptr) || (0 == device_len) || (PNULL == dir_ptr) || (0 == dir_len))
    {
        //SCI_TRACE_LOW:"InitMultimFolder param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_4471_112_2_18_2_20_14_244, (uint8*) "");
        return;
    }

    if (MMIAPIFMM_CombineFullPath (device_ptr, device_len,
                                   dir_ptr, dir_len,
                                   NULL, 0,
                                   full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir (full_path_name, full_path_len);
        }
    }
}

/*****************************************************************************/
//  Description : init system folder
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitSystemFolder (
    const wchar   *device_ptr,
    uint16  device_len,
    const wchar   *dir_ptr,
    uint16  dir_len
)
{
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;

    if (PNULL == device_ptr || 0 == device_len || PNULL == dir_ptr || 0 == dir_len)
    {
        return;
    }

    if (MMIAPIFMM_CombineFullPath (device_ptr, device_len,
                                   dir_ptr, dir_len,
                                   NULL, 0,
                                   full_path_name, &full_path_len))
    {
        MMIAPIFMM_CreateDir (full_path_name, full_path_len);
        MMIAPIFMM_SetAttr (full_path_name, full_path_len, TRUE, TRUE, TRUE, FALSE);
    }

}

/*****************************************************************************/
//  Description : initialize Multim
//  Global resource dependence : none
//  Author: Liqing.Peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitFolders (void)
{
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;

    //create default dir
    // D:/Photos, D:/Audio, D:/Video
    do
    {
        if (MMIAPIFMM_GetDeviceTypeStatus (fmm_dev))
        {

            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE));

#ifdef BAIDU_DRIVE_SUPPORT

            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL));

            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD));

            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD));
#endif

        #ifdef CMCC_UI_STYLE
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_PHOTOS, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS));
        #endif
        
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MUSIC));

            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE));

#ifdef MMI_KING_MOVIE_SUPPORT

            //u盘下不创建kingmovie文件夹
            if (fmm_dev != MMI_DEVICE_UDISK)
            {
                InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                                  MMIMULTIM_DIR_KINGMOVIE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_KINGMOVIE));
            }

#endif
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_EBOOK, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_EBOOK));
#if defined MMI_VCARD_SUPPORT
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_VCARD, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_VCARD));
#endif
#ifdef MMI_VCALENDAR_SUPPORT
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_VCALENDAR, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_VCALENDAR));
#endif
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_OTHER_DEFAULT_DIR, MMIAPICOM_Wstrlen (MMIMULTIM_OTHER_DEFAULT_DIR));

            MMIAPIFMM_InitSystemFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                                        MMIMULTIM_DIR_SYSTEM, MMIMULTIM_DIR_SYSTEM_LEN);
            InitMultimFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                              MMIMULTIM_DIR_ALARM, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_ALARM));
#ifdef MMI_SSL_CERT_SUPPORT                              
            //browser certification folder
            InitMultimFolder(MMIAPIFMM_GetDevicePath(fmm_dev), MMIAPIFMM_GetDevicePathLen(fmm_dev),
                            MMIMULTIM_DIR_SSL_CERT, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_SSL_CERT));
#endif
        }
    }
    while (++fmm_dev < MMI_DEVICE_NUM);
#if defined MMI_WALLPAPER_SUPPORT
    MMIAPIFMM_InitWallPaper();
#endif
#ifdef DRM_SUPPORT
    MMIDRM_CreateDrmRoDir();
#endif
}

/*****************************************************************************/
//  Description : get file devie from data info
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E FmmGetFileDevice (const wchar *device_ptr,  uint16 device_len);

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence :
// Author:  murphy.xie
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewVideo (wchar *full_path_name_ptr)
{
    MMIFMM_EnterVideoPreviewWin (full_path_name_ptr);
}

/*****************************************************************************/
// Description : 文件详情
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_DetailFileData (
    MMIFMM_DETAIL_DATA_T* detail_data  // 数据
)
{
    MMIFMM_EnterFileDetailWin (detail_data);
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchOpenPicturePreviewWin (
    uint32 index,
    FILEARRAY   file_arra
)
{
    BOOLEAN     result = FALSE;
    FILEARRAY_DATA_T file_info = {0};

    result = MMIAPIFILEARRAY_Read (file_arra, index, &file_info);

    if (0 == file_info.name_len)
    {
        result = FALSE;
    }

    if (result)
    {
        MMIFMM_PIC_PERVIEW_WIN_DATA_T pic_preview_data = {0};
        MMIAPICOM_Wstrncpy (pic_preview_data.filename, file_info.filename, MIN (MMIFILE_FULL_PATH_MAX_LEN, file_info.name_len));
        pic_preview_data.name_len = file_info.name_len;
        MMIFMM_EnterSearchPreviewWin (&pic_preview_data);
    }
    else
    {
        SCI_TRACE_LOW ("FMM:MMIFMM_SearchOpenPicturePreviewWin fail!");
    }
}

/*****************************************************************************/
//  Description : get file sort type
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_SORT_TYPE_E MMIFMM_GeFileSortType (void)
{
    MN_RETURN_RESULT_E  nv_value = MN_RETURN_FAILURE;
    MMIFMM_SORT_TYPE_E sort_type = MMIFMM_SORT_TYPE_NAME;

    MMINV_READ (FMM_SORT_TYPE, &sort_type, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        sort_type = MMIFMM_SORT_TYPE_NAME;
        MMINV_WRITE (FMM_SORT_TYPE, &sort_type);
    }

    return sort_type;
}

/*****************************************************************************/
//  Description : set file sort type
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetFileSortType (MMIFMM_SORT_TYPE_E sort_type)
{
    //if (sort_type<=MMIFMM_SORT_TYPE_TYPE&&sort_type>=MMIFMM_SORT_TYPE_NAME)
    {
        MMINV_WRITE (FMM_SORT_TYPE, &sort_type);
    }
}

/*****************************************************************************/
//  Description : get select tartget path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_GetSelectTargetPath (MMIFMM_OpenFileCallBack callback)
{

    MMIFMM_OpenTartgetWin (callback);
}

/*****************************************************************************/
// Description : memory detail
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmMemoryDetail (uint32 device)
{
    MMIAPIUDISK_OpenMemDetailWin (device);
}



/*****************************************************************************/
//  Description : MMIAPIFMM_ShowTxtContent
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIAPIFMM_ShowTxtContent (wchar *full_path_name_ptr)
{
    MMIFMM_ShowTxtContent (full_path_name_ptr);
}

/*****************************************************************************/
//  Description : show txt content
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIFMM_ShowTxtContent (wchar *full_path_name_ptr)
{
#ifdef EBOOK_SUPPORT
    uint32                      file_size = 0;                                                      // 文件大小
    uint16                      filename_len = 0;                                               // 文件名长度，字符数
    uint16                      fullfilename_len = 0;                                           // 文件名长度，字符数
    MMIFILE_DEVICE_E            device = MMI_DEVICE_UDISK;
    uint16                      file_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    BOOLEAN                     is_empty = FALSE;

    if (PNULL == full_path_name_ptr)
    {
        is_empty = TRUE;
        file_size = 0;
    }
    else
    {
        fullfilename_len = (uint16) MMIAPICOM_Wstrlen (full_path_name_ptr);//(uint16)MMIFMM_CombineFullFileName(list_data_ptr,PNULL,index,s_full_file_name_ptr,MMIFMM_FULL_FILENAME_LEN);
        MMIAPIFMM_GetFileInfo (full_path_name_ptr,
                               fullfilename_len,
                               &file_size,
                               PNULL,
                               PNULL);
    }


    if (0 == file_size || is_empty)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return ;
    }

    device = FmmGetFileDevice (full_path_name_ptr, 1);
    MMIAPICOM_GetFileName (full_path_name_ptr, fullfilename_len, file_name, &filename_len);
    MMIAPIEBOOK_ManagerWinMsg_4Fmm ( (uint8) device, file_name, filename_len, file_size, full_path_name_ptr, fullfilename_len);
#else
	MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
#endif
}

/*****************************************************************************/
//  Description : get file devie from data info
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E FmmGetFileDevice (const wchar *device_ptr,  uint16 device_len)
{
    return MMIAPIFMM_GetDeviceTypeByPath (device_ptr, device_len);
}


/*****************************************************************************/
// Description : open preview picture win
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewPicture (wchar *full_path_name_ptr)
{
    MMIFMM_EnterPreviewPicture (full_path_name_ptr);
}

/*****************************************************************************/
//  Description : append tab ctrl
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_AppendTabCtrl (
    MMI_CTRL_ID_T ctrl_id,
    uint32 base_index,
    MMIFILE_DEVICE_E fmm_dev)

{
    MMI_STRING_T   str_text = {0};
#ifdef MMI_PDA_SUPPORT
    MMI_TEXT_ID_T g_s_tabstr[] =
    {
        TXT_COMMON_UDISK,
#ifndef DUAL_TCARD_SUPPORT
        TXT_COMMON_SD_CARD,
#else
		TXT_COMMON_SD_CARD0,
#endif
        TXT_COMMON_SD_CARD1,
    };
#if defined MMI_ISTYLE_SUPPORT
    MMI_IMAGE_ID_T g_s_focustab_istyle_icon[] =
    {
        ISTYLE_IMAGE_FMM_UDISK_FOCUSED,
        ISTYLE_IMAGE_FMM_SDCARD_FOCUSED,
        ISTYLE_IMAGE_FMM_SDCARD1_FOCUSED,
    };
    MMI_IMAGE_ID_T g_s_releasetab_istyle_icon[] =
    {
        ISTYLE_IMAGE_FMM_UDISK_RELEASED,
        ISTYLE_IMAGE_FMM_SDCARD_RELEASED,
        ISTYLE_IMAGE_FMM_SDCARD1_RELEASED,
    };
#endif	
    MMI_IMAGE_ID_T g_s_focustab_icon[] =
    {
        IMAGE_FMM_UDISK_FOCUSED,
        IMAGE_FMM_SDCARD_FOCUSED,
        IMAGE_FMM_SDCARD1_FOCUSED,
    };

    MMI_IMAGE_ID_T g_s_releasetab_icon[] =
    {
        IMAGE_FMM_UDISK_RELEASED,
        IMAGE_FMM_SDCARD_RELEASED,
        IMAGE_FMM_SDCARD1_RELEASED,
    };

    MMI_GetLabelTextByLang (g_s_tabstr[fmm_dev - base_index], &str_text);


    {
        MMI_IMAGE_ID_T *focustab_icon_ptr = PNULL;
        MMI_IMAGE_ID_T *releasetab_icon_ptr = PNULL;
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            focustab_icon_ptr = g_s_focustab_istyle_icon;
            releasetab_icon_ptr = g_s_releasetab_istyle_icon;
        }
        else
#endif			
        {
            focustab_icon_ptr = g_s_focustab_icon;
            releasetab_icon_ptr = g_s_releasetab_icon;
        }

        GUITAB_AppendSimpleItem (
            ctrl_id,
            &str_text,
            focustab_icon_ptr[fmm_dev - base_index],
            releasetab_icon_ptr[fmm_dev - base_index]
        );

    }
#else
    GUITAB_AppendSimpleItem (
        ctrl_id,
        &str_text,
        IMAGE_FMM_UDISK_FOCUSED + fmm_dev - base_index,
        IMAGE_FMM_UDISK_RELEASED + fmm_dev - base_index
    );
#endif

}


/*****************************************************************************/
//  Description : local open fmm  main win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenFmmMainExplorer (
    const wchar * path_ptr,
    uint16   path_len,
    const wchar * filename,
    uint16   file_len,
    BOOLEAN is_need_display,
    BOOLEAN is_protect_checked,
    uint32  find_suffix_type
)
{
    MMIFMM_OPENWIN_INFO_T  openwin_info = {0};

    openwin_info.path_ptr = path_ptr;
    openwin_info.path_len = path_len;
    openwin_info.filename = filename;
    openwin_info.file_len = file_len;
    openwin_info.is_need_display = is_need_display;
    openwin_info.privacy_protect = is_protect_checked;
    openwin_info.find_suffix_type = find_suffix_type;
    openwin_info.sk_text_id.leftsoft_id = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
	openwin_info.sk_text_id.middlesoft_id = IMAGE_NULL;
#else
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
#endif
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;
    //openwin_info.callback = 0;

    MMIFMM_OpenExplorerWin (&openwin_info);
}


/*****************************************************************************/
//  Description : open any fmm win with callback function
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenExplorerWin (MMIFMM_OPENWIN_INFO_T * openwin_info)
{
    return MMIFMM_OpenExplorerWin (openwin_info);
}

/*****************************************************************************/
//  Description : open any fmm win no para
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorer (void)
{
    MMIFMM_OPENWIN_INFO_T  openwin_info = {0};

    openwin_info.path_ptr = PNULL;
    openwin_info.path_len = 0;
    openwin_info.filename = PNULL;
    openwin_info.file_len = 0;
    openwin_info.is_need_display = FALSE;
    openwin_info.privacy_protect = FALSE;
    openwin_info.find_suffix_type = MMIFMM_FILE_ALL;
    openwin_info.sk_text_id.leftsoft_id = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    openwin_info.sk_text_id.middlesoft_id = IMAGE_NULL;
#else
	openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
#endif
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;
    //openwin_info.callback = PNULL;

    MMIFMM_OpenExplorerWin (&openwin_info);
}

/*****************************************************************************/
//  Description : open any fmm win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorerExt (
    const wchar * path_ptr,
    uint16   path_len,
    const wchar * filename,
    uint16   file_len,
    BOOLEAN is_need_display,
    uint32  find_suffix_type
)
{
    MMIFMM_OPENWIN_INFO_T  openwin_info = {0};

    openwin_info.path_ptr = path_ptr;
    openwin_info.path_len = path_len;
    openwin_info.filename = filename;
    openwin_info.file_len = file_len;
    openwin_info.is_need_display = is_need_display;
    openwin_info.privacy_protect = FALSE;
    openwin_info.find_suffix_type = find_suffix_type;
    openwin_info.sk_text_id.leftsoft_id = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    openwin_info.sk_text_id.middlesoft_id = IMAGE_NULL;
#else
	openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
#endif
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;
    //openwin_info.callback = PNULL;

    MMIFMM_OpenExplorerWin (&openwin_info);
}

/*****************************************************************************/
//  Description : open any fmm win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorerWithoutProtect (
    const wchar * path_ptr,
    uint16   path_len,
    const wchar * filename,
    uint16   file_len,
    BOOLEAN is_need_display,
    uint32  find_suffix_type
)
{
    MMIFMM_OPENWIN_INFO_T  openwin_info = {0};

    openwin_info.path_ptr = path_ptr;
    openwin_info.path_len = path_len;
    openwin_info.filename = filename;
    openwin_info.file_len = file_len;
    openwin_info.is_need_display = is_need_display;
    openwin_info.privacy_protect = TRUE;
    openwin_info.find_suffix_type = find_suffix_type;
    openwin_info.sk_text_id.leftsoft_id = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
	openwin_info.sk_text_id.middlesoft_id = IMAGE_NULL;
#else
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
#endif
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;
    //openwin_info.callback = PNULL;

    MMIFMM_OpenExplorerWin (&openwin_info);
}


/*****************************************************************************/
// 	Description : indication to other module that file has updated
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIFMM_UpdateEventInd (void)
{
#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_AppUpdateInd (MMISEARCH_TYPE_FILE);
#endif
}


/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlayMusicFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16                               cur_index,
    uint16				        total_num
)
{

    uint16                            index = 0;
    uint16                            filename_len = 0;
    MMIFILE_FILE_INFO_T               file_info = {0};
	wchar               full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
#ifndef  MUSIC_PLAYER_SUPPORT
    uint16                            *dir_path_ptr = PNULL;
    uint16                            dir_path_len = 0;
#endif
    SCI_TRACE_LOW ("MMIFMM: FmmPlayMusicFile, index = %d", index);
    //SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5528_112_2_18_2_20_46_434, (uint8*) "d", ctrl_id);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmPlayMusicFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5533_112_2_18_2_20_46_435, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmPlayMusicFile] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5539_112_2_18_2_20_46_436, (uint8*) "");
        return;
    }

    index = cur_index;

    if (0 == list_data_ptr->data[index]->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }

    // 建立全路径文件名
    filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);
    MMIAPIFMM_GetFileInfoFormFullPath (full_file_name, filename_len, &file_info);
#ifdef DRM_SUPPORT
	if((!MMIAPIMP3_IsMp3PlayerLoadType(full_file_name,filename_len)))
	{
		MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
		return;
	}
#endif
#ifdef  MUSIC_PLAYER_SUPPORT
    MMIAPIMP3_PlayMyDocumentMusic (&file_info);
#elif defined(MMI_AUDIO_PLAYER_SUPPORT)

    if (!list_data_ptr->path_is_valid)
    {
        dir_path_ptr = current_path_ptr->pathname;
        dir_path_len = current_path_ptr->pathname_len;
    }

    MMIAPIMP3_PlayFmmMusic (
        list_data_ptr,
        total_num,
        index,
        dir_path_ptr,
        dir_path_len
    );
//fixed by liyan.zhu for CR NEWMS00207324
#else
    MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
#endif
}


/*****************************************************************************/
//  Description : play movie file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlayMovieFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16              index
)
{
#ifdef VIDEO_PLAYER_SUPPORT
    uint16              suffix_len = 0;
    uint16              filename_len = 0;
    MMICOM_VIDEO_TYPE_E     file_type   = MMICOM_VIDEO_TYPE_MAX;
    //FILE_DEV_E_T        device       = FS_HS;
    wchar               list_suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
    wchar               file_name [MMIFMM_FILENAME_LEN+1] = {0};
    wchar               full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmPlayMovieFile, index = %d", index);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmPlayMovieFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5399_112_2_18_2_20_45_426, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmPlayMovieFile] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5405_112_2_18_2_20_45_427, (uint8*) "");
        return;
    }

    if (0 == list_data_ptr->data[index]->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }

    filename_len = (uint16) MMIFMM_FmmGetFileName (list_data_ptr, index, file_name, MMIFMM_FULL_FILENAME_LEN);

    suffix_len = MMIFMM_FILENAME_LEN;

    if (MMIAPIFMM_SplitFileName (file_name, filename_len, PNULL, PNULL, list_suffix_name, &suffix_len))
    {

        // 解析文件类型
        file_type = MMIAPIFMM_ConvertMovieFileType (list_suffix_name, suffix_len);

        // 建立全路径文件名
        filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

        {
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_PlayVideo (full_file_name, filename_len);
#else
            MMIAPIVP_PlayVideoFromVideoPlayer (VP_ENTRY_FROM_FILE, full_file_name, filename_len, PNULL);
#endif
        }
    }

#endif
}



#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : open szip file
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenFont (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
)
{
    //uint16      index = 0;
    wchar       full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmOpenFont, index = %d", index);
    //SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2471_112_2_18_2_20_40_349, (uint8*) "d", ctrl_id);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenFont]:NULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2476_112_2_18_2_20_40_350, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenFont]:NULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2481_112_2_18_2_20_40_351, (uint8*) "");
        return;
    }


    // 建立全路径文件名
    MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    MMIAPISET_SetFont (full_file_name, FALSE, TRUE);
}
#endif


#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : open szip file
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenSzipFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
)
{
    //uint16      index = 0;
    wchar       full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmOpenSzipFile, index = %d", index);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:" PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2877_112_2_18_2_20_40_361, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:" PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2882_112_2_18_2_20_40_362, (uint8*) "");
        return;
    }

    // 建立全路径文件名
    MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    MMIAPIDYNA_InstallApp (full_file_name);
}
#endif




#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : open vcard file
//  Global resource dependence :
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC VCARD_ERROR_E MMIFMM_GetVcardFileData (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index)
{
    //uint16           index = 0;
    uint16           filename_len = 0;
    VCARD_ERROR_E    ret = VCARD_NO_ERROR;
    wchar            full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: FmmPlayMusicFile, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetVcardFileData] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8708_112_2_18_2_20_53_480, (uint8*) "");
        return VCARD_PARAM_ERROR;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetVcardFileData] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8714_112_2_18_2_20_53_481, (uint8*) "");
        return VCARD_PARAM_ERROR;
    }

    // 建立全路径文件名
    filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    ret = MMIAPIVCARD_GetDataByFile (full_file_name);
    //SCI_TRACE_LOW:"MMIFMM_GetVcardFileData list_data_ptr->data[index].file_size %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8722_112_2_18_2_20_53_482, (uint8*) "d", list_data_ptr->data[index]->file_size);

    if ( (ret == VCARD_NO_ERROR) && (MMISMS_VCARD_MAX_8BIT_SPLIT_LEN < list_data_ptr->data[index]->file_size))
    {
        //vcard too large
        ret = VCARD_TOBIG_FILE;
    }

    return ret;
}
#endif


#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIFMM_GetVcalendarFileData (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index)
{
    uint32           filename_len = 0;
    MMIVCAL_ERROR_TYPE_E    ret = MMIVCAL_NO_ERROR;
    wchar            full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_GetVcalendarFileData, index = %d", index);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8749_112_2_18_2_20_53_484, (uint8*) "");
        return MMIVCAL_PARAM_ERROR;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8755_112_2_18_2_20_53_485, (uint8*) "");
        return MMIVCAL_PARAM_ERROR;
    }

    // 建立全路径文件名
    filename_len = MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    ret = MMIVCAL_GetDataByFile (full_file_name);
    //SCI_TRACE_LOW:"MMIFMM_GetVcalendarFileData list_data_ptr->data[index].file_size %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8763_112_2_18_2_20_53_486, (uint8*) "d", list_data_ptr->data[index]->file_size);

    if ( (ret == MMIVCAL_NO_ERROR) && (MMISMS_VCARD_MAX_8BIT_SPLIT_LEN < list_data_ptr->data[index]->file_size))
    {
        //vcard too large
        ret = MMIVCAL_IS_TOOBIG;
    }

    return ret;
}
#endif

#ifdef MMI_FMM_SEARCH_SUPPORT
/*****************************************************************************/
// Description : 在给定路径下查找文件，需要输入文件名字
// Global resource dependence :
// Author:    Ming.Song 2007-4-12
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenFindFileWin (
    wchar           *find_path_ptr,
    MMIFMM_DATA_INFO_T    *result_data_ptr,
    uint16                max_result_file_num,
    uint32                win_id,
    uint32                msg_id
)
{
    MMIFMM_FINDFILE_WIN_PARAM_T param_to_findfile_win = {0};

    param_to_findfile_win.find_path_ptr = find_path_ptr;
    param_to_findfile_win.result_data_ptr = result_data_ptr;
    param_to_findfile_win.max_result_file_num = max_result_file_num;
    param_to_findfile_win.win_id = win_id;
    param_to_findfile_win.msg_id = msg_id;
    MMIFMM_EnterFindFileWin (&param_to_findfile_win);
}

#endif
/*****************************************************************************/
// Description : 在给定路径下创建目录，需要输入目录名字
// Global resource dependence :
// Author:    Ming.Song 2007-4-17
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmCreateFolder (
    wchar  *full_path_ptr,      // 全路径
    wchar  *new_folder_ptr,     // 新文件夹
    uint16  *new_folder_len_ptr, // 新文件夹的长度
    uint32  win_id,              // 创建文件夹后通知的窗口
    uint32  msg_id               // 通知的消息
)
{
    uint32                              current_path_len = 0;
    //uint16                              i = 0;
    uint16                              path_depth = 0;
    uint16                              new_folder_len = 0;

    current_path_len = MMIAPICOM_Wstrlen (full_path_ptr);

    if (MMIFMM_FULL_FILENAME_LEN <= (current_path_len + 1))
    {
        MMIPUB_OpenAlertWarningWin (TXT_COM_FILE_NAME_TOO_LONG);
        return;
    }

    if (MMIFMM_PATH_DEPTH <= MMIFMM_GetPathDepth (full_path_ptr, (uint16) current_path_len))
    {
        //fixed by liyan.zhu for CR NEWMS00207771
        //MMIPUB_OpenAlertFailWin(TXT_MAX_DEPTH);
        MMIPUB_OpenAlertWarningWin (TXT_MAX_DEPTH);
        return;
    }

//    if(s_fmm_list_data_ptr->file_num + s_fmm_list_data_ptr->folder_num >= MMIFMM_FILE_NUM)
//    {
//        MMIPUB_OpenAlertWarningWin(TXT_FMM_FILES_TOO_MANY);
//        return;
//     }
    new_folder_len = (uint16) (MMIFMM_FULL_FILENAME_LEN - current_path_len - 1);

    path_depth = (uint16) MMIFMM_GetFullPathDepth (full_path_ptr, current_path_len); /*lint !e516*/

    if (MMIFMM_PATH_DEPTH > path_depth)
    {
        MMIFMM_CREATEFOLDER_WIN_PARAM_T create_folder_win_info = {0};
        create_folder_win_info.full_path_ptr = full_path_ptr;
        create_folder_win_info.new_folder_max_len = new_folder_len;
        create_folder_win_info.new_folder_len_ptr = new_folder_len_ptr;
        create_folder_win_info.new_folder_ptr = new_folder_ptr;
        create_folder_win_info.win_id = win_id;
        create_folder_win_info.msg_id = msg_id;
        MMIFMM_CreateFolderWin (&create_folder_win_info);
    }
    else
    {
        //fixed by liyan.zhu for CR NEWMS00207771
        //MMIPUB_OpenAlertFailWin(TXT_MAX_DEPTH);
        MMIPUB_OpenAlertWarningWin (TXT_MAX_DEPTH);
    }
}


/*****************************************************************************/
//  Description : sort
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmFileDataSort (
    uint32  win_id,
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_SORT_TYPE_E     sort
)
{
    //SCI_TRACE_LOW:"MMIFMM: FmmFileDataSort, sort =%d."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5240_112_2_18_2_20_45_420, (uint8*) "d", sort);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmFileDataSort] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5243_112_2_18_2_20_45_421, (uint8*) "");
        return;
    }

    list_data_ptr->sort = sort;

    MMIFMM_FilePreSort (list_data_ptr);

    MMIAPIFMM_FileDataSort (list_data_ptr, sort);

    MMIFMM_SetFileSortType (sort);

    MMK_PostMsg (win_id, MSG_FMM_SORT_END_CNF, PNULL, 0);
}

#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : open vcard file
//  Global resource dependence :
//  Author: ming.song
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenVcardFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
)
{
    wchar            full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: FmmPlayMusicFile, index = %d", index);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5461_112_2_18_2_20_46_429, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5467_112_2_18_2_20_46_430, (uint8*) "");
        return;
    }

    // 建立全路径文件名
    MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    MMIAPIPB_OpenVcardFileContactWin (full_file_name);
}
#endif


#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
//  Description : open vcalendar file
//  Global resource dependence :
//  Author: ming.song
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenVcalendarFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
)
{
    wchar            full_file_name[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmOpenVcalendarFile, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5493_112_2_18_2_20_46_432, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5499_112_2_18_2_20_46_433, (uint8*) "");
        return;
    }

    // 建立全路径文件名
    MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);
#ifdef CALENDAR_SUPPORT
    MMIAPISCH_ReadVcalendarFile (full_file_name);
#endif
}
#endif

#ifdef BROWSER_SUPPORT_NETFRONT
/*****************************************************************************/
//  Description : open www file
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenWWWFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
)
{
    uint8 *url_ptr = PNULL;
    uint16 str_len = 0;
    uint16 header_len = 0;
    //uint16 index  = 0;
    const char * file_header = "file:///";
    wchar full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmOpenWWWFile, index = %d", index);


    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return;
    }

    MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, full_file_name, MMIFMM_FILENAME_LEN);

    header_len = strlen (file_header);
    str_len = (uint16) (MMIAPICOM_Wstrlen (full_file_name) * 3 + header_len + 1);
    url_ptr = SCI_ALLOCA (str_len * sizeof (uint8));

    if (PNULL == url_ptr)
    {
        return;
    }

    SCI_MEMSET (url_ptr, 0x00, (str_len * sizeof (uint8)));
    SCI_MEMCPY (url_ptr, file_header, header_len);
    str_len -= header_len;
    GUI_WstrToUTF8 (url_ptr + header_len, str_len, full_file_name, MMIAPICOM_Wstrlen (full_file_name));
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = (char *) url_ptr;
    entry_param.user_agent_ptr = PNULL;
    MMIAPIBROWSER_Entry (&entry_param);
    SCI_FREE (url_ptr);

    return;
}
#endif


#ifdef KURO_SUPPORT
/*****************************************************************************/
//  Description : play kur music file
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlaykurFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16     					 index
)
{

    uint16                            filename_len = 0;
    MMIFILE_FILE_INFO_T               file_info = {0};
    wchar                             full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmPlaykurFile, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetFileIndex]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10505_112_2_18_2_20_57_513, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetFileIndex]PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10511_112_2_18_2_20_57_514, (uint8*) "");
        return;
    }


    // 建立全路径文件名
    filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    MMIAPIFMM_GetFileInfoFormFullPath (full_file_name, filename_len, &file_info);

    MMIAPIKUR_PlayMyDocumentMusic (&file_info);
}
#endif



/*****************************************************************************/
//  Description :是否在文件复制/删除操作
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFileOption (void)
{
    if (MMK_IsOpenWin (MMIFMM_FILE_WAITING_WIN_ID)
            || MMK_IsOpenWin (MMISDCARD_FORMATTING_WIN_ID) //格式化
            || (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID) && (0 != MMIFMM_GetDelState()))
       )
    {
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_IsFileOption return false!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10316_112_2_18_2_20_57_508, (uint8*) "");
        return FALSE;
    }
}

#if defined (BLUETOOTH_SUPPORT)
/*****************************************************************************/
//  Description : BT send files
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmBtSendFile (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    MMIFMM_PATH_INFO_T     *current_path_ptr,
    uint16     					 index
)
{
    MMIFILE_FILE_INFO_T         file_info = {0};
    uint16                      name_len = 0;
    uint16                      send_file_index = 0;
    uint16                      send_file_num = 0;
    wchar                       full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    MMIFMM_BT_SENDFILE_INFO_T   *s_fmm_sendfile_info = PNULL;
    //SCI_TRACE_LOW:"MMIFMM: FmmBtSendFile."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5124_112_2_18_2_20_45_414, (uint8*) "");

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmBtSendFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5127_112_2_18_2_20_45_415, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmBtSendFile] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5133_112_2_18_2_20_45_416, (uint8*) "");
        return;
    }

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
        return;
    }

    if (MMIFMM_FILE_STATE_UNCHECKED == list_data_ptr->checked)
    {
        send_file_num = 1;

        // 建立全路径文件名
        name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

#ifdef DRM_SUPPORT
		if (MMIAPIFMM_IsDrmFileBySuffix(full_file_name, name_len))
		{	
			//只要不能发送，则立马提示返回
			if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, full_file_name))
			{
				MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
				return;
			}
		} 
#endif


        MMIAPIFMM_GetFileInfo (full_file_name, name_len, &file_info.file_size, NULL, NULL);
        s_fmm_sendfile_info = (MMIFMM_BT_SENDFILE_INFO_T *) SCI_ALLOCA (sizeof (MMIFMM_BT_SENDFILE_INFO_T));

        if (PNULL == s_fmm_sendfile_info)
        {
            return;
        }

        SCI_MEMSET (s_fmm_sendfile_info, 0, sizeof (MMIFMM_BT_SENDFILE_INFO_T));
        MMIAPICOM_Wstrncpy (s_fmm_sendfile_info->filepath_name, full_file_name, name_len); /*lint !e605*/
        s_fmm_sendfile_info->filepath_len = name_len;
        s_fmm_sendfile_info->file_size  = file_info.file_size;
        s_fmm_sendfile_info->is_temp_file = FALSE;
        //SCI_TRACE_LOW:"[&&&]FmmBtSendFile  send_file_num = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5169_112_2_18_2_20_45_417, (uint8*) "d", send_file_num);
    }
    else
    {
        send_file_num = list_data_ptr->mark_num;
        //SCI_TRACE_LOW:"[&&&]FmmBtSendFile  send_file_num=%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5174_112_2_18_2_20_45_418, (uint8*) "d", send_file_num);

		if(send_file_num > MMIBT_SENDFILE_MAX_NUM)//CR178620
		{
			MMIPUB_OpenAlertWarningWin(TXT_BT_SENDFILE_MAX_NUMBER);
			return;
		}

        
        for (index = 0; index < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); index++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[index]->state)
            {
                // 建立全路径文件名
                name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

				//check if contails any folder
				if(index<list_data_ptr->folder_num)
				{
					MMIPUB_OpenAlertWarningWin(TXT_FMM_SEND_RESELECT);
					return;
				} 
				//check if contails any empty file
				if (0 == list_data_ptr->data[index]->file_size)
				{
					MMIPUB_OpenAlertWarningWin(TXT_FMM_SEND_RESELECT);
					return;
				}
				//检查发送的文件中是否含有DRM未授权文件
#ifdef DRM_SUPPORT
				if (MMIAPIFMM_IsDrmFileBySuffix(full_file_name, name_len))
				{	
					//只要不能发送，则立马提示返回
					if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, full_file_name))
					{
						MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
						return;
					}
				}  
#endif

            }
        }

        s_fmm_sendfile_info = (MMIFMM_BT_SENDFILE_INFO_T *) SCI_ALLOCA (send_file_num * sizeof (MMIFMM_BT_SENDFILE_INFO_T));

        if (PNULL == s_fmm_sendfile_info)
        {
            return;
        }

        SCI_MEMSET (s_fmm_sendfile_info, 0, send_file_num * sizeof (MMIFMM_BT_SENDFILE_INFO_T));

        //找出mark的文件
        for (index = 0; index < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); index++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[index]->state)
            {
                SCI_MEMSET (full_file_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));
                // 建立全路径文件名
                name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

                MMIAPIFMM_GetFileInfo (full_file_name, name_len, &file_info.file_size, NULL, NULL);

                MMIAPICOM_Wstrncpy (s_fmm_sendfile_info[send_file_index].filepath_name, full_file_name, name_len); /*lint !e605*/
                s_fmm_sendfile_info[send_file_index].filepath_len = name_len;
                s_fmm_sendfile_info[send_file_index].file_size  = file_info.file_size;
                s_fmm_sendfile_info[send_file_index].is_temp_file = FALSE;

                send_file_index ++;
            }
        }

        //SCI_TRACE_LOW:"[&&&]FmmBtSendFile  send_file_index=%d,send_file_num = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5220_112_2_18_2_20_45_419, (uint8*) "dd", send_file_index, send_file_num);
    }

    if (0 == s_fmm_sendfile_info->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_FMM_SEND_RESELECT);
        SCI_FREE (s_fmm_sendfile_info);
        return;
    }

    MMIAPIBT_SendMultiFile (s_fmm_sendfile_info, send_file_num);

    //if (PNULL != s_fmm_sendfile_info)
    {
        SCI_FREE (s_fmm_sendfile_info);
    }

}
#endif
/*****************************************************************************/
//  Description : 详情
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmFileDetail (
    uint16     					 index,
    MMIFMM_DATA_INFO_T     *list_data_ptr
)
{
    MMIFMM_DETAIL_DATA_T    detail_data = {0};

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmFileDetail, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmFileDetail] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4059_112_2_18_2_20_43_384, (uint8*) "");
        return;
    }

    SCI_MEMSET (&detail_data , 0, sizeof (MMIFMM_DETAIL_DATA_T));
    detail_data.time = list_data_ptr->data[index]->time;
    detail_data.file_size = list_data_ptr->data[index]->file_size;

    detail_data.filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, detail_data.filename, MMIFMM_FILENAME_LEN);

    detail_data.type = list_data_ptr->data[index]->type;
    MMIFMM_EnterFileDetailWin (&detail_data);
}

/*****************************************************************************/
//  Description : 重命名
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmRenameFile (
    uint16     					 index,
    MMI_WIN_ID_T           win_id,
    MMI_MESSAGE_ID_E       msg_id,
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    wchar*                 full_file_name_ptr
)
{

    MMIFMM_RENAME_WIN_PARAM_T  rename_param = {0};

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmRenameFile, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmRenameFile] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4090_112_2_18_2_20_43_386, (uint8*) "");
        return;
    }


    MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, full_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);

    //FmmRename(s_full_file_name_ptr, list_data_ptr->data[index]->type,0, win_id, msg_id);
    rename_param.full_path_ptr = full_file_name_ptr;
    rename_param.max_file_lan = 0;
    rename_param.msg_id = msg_id;
    rename_param.win_id = win_id;
    rename_param.type = list_data_ptr->data[index]->type;
    MMIFMM_EnterReNameWin (&rename_param);

}

#if defined (MMS_SUPPORT)
/*****************************************************************************/
//  Description : send MMS
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSendMMS (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    uint16     					 index
)
{
    MMIFILE_FILE_INFO_T     file_info = {0};
    wchar                    full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    //uint16                  dir_name_len = 0;
    uint16                  name_len = 0;
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;

    //SCI_TRACE_LOW:"MMIFMM: FmmSendMMS."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5033_112_2_18_2_20_45_411, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSendMMS] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5038_112_2_18_2_20_45_412, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSendMMS] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_5044_112_2_18_2_20_45_413, (uint8*) "");
        return;
    }

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
        return;
    }

    // 建立全路径文件名
    name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    MMIAPIFMM_GetFileInfoFormFullPath (full_file_name, name_len, &file_info);


    file_type = list_data_ptr->data[index]->type;

    if (MMIFMM_FILE_TYPE_FOLDER != file_type)
    {
        if (MMIFMM_FILE_TYPE_PICTURE == file_type
                || MMIFMM_FILE_TYPE_MUSIC == file_type
                || MMIFMM_FILE_TYPE_MOVIE == file_type)
        {
            MMIAPIMMS_EditMMSFromFile (&file_info, (MMIMULTIM_FILE_TYPE_E) file_type);/*lint !e64*/
        }

#ifdef DRM_SUPPORT
        else if (MMIFMM_FILE_TYPE_DRM == file_type)
        {
            //只要不能发送，则立马提示返回
            if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, full_file_name))
            {
                MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                return;
            }

            MMIAPIMMS_EditMMSFromFile (&file_info, file_type);
        }

#endif
        else
        {
            file_type = MMIFMM_FILE_TYPE_NORMAL;
            MMIAPIMMS_EditMMSFromFile (&file_info, (MMIMULTIM_FILE_TYPE_E) file_type);/*lint !e64*/
        }
    }
    else
    {
        // 提示格式不支持
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
    }
}
#endif




/*****************************************************************************/
//  Description : handle menu option function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_HandleMenuOpt (MMIFMM_OPTION_TYPE_E type)
{
    MMIFMM_HandleMenuOpt (type);
}

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : set call ring
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetCallRing (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    uint16     					 index,
    MN_DUAL_SYS_E         dual_sys
#ifndef MMI_PDA_SUPPORT
	,
	MMIFMM_SET_RING_TYPE_E   ring_type
#endif
)
{
    wchar                   full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    uint16                  name_len = 0;
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;

    //SCI_TRACE_LOW:"MMIFMM: FmmSetCallRing."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4353_112_2_18_2_20_43_396, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetCallRing] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4358_112_2_18_2_20_43_397, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetCallRing] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4364_112_2_18_2_20_43_398, (uint8*) "");
        return;
    }

    // 建立全路径文件名
    name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

    file_type = list_data_ptr->data[index]->type;

#ifdef DRM_SUPPORT

    if (!MMIAPIDRM_CanDrmFileSetToWSPC (full_file_name, name_len, DRMFILE_SETTING_CALLRING))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_RINGTONE);
        return;
    }

    if (MMIFMM_FILE_TYPE_DRM == file_type)
    {
        file_type = MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, full_file_name);
    }

#endif


    if (MMIFMM_FILE_TYPE_MUSIC == file_type)
    {
#ifndef MMI_PDA_SUPPORT 
        MMIAPIFMM_SetMusicToRing(ring_type, (DPARAM)&dual_sys, full_file_name, name_len);
#else
        MMIAPIFMM_SetMusicToRing (MMIMULTIM_SET_CALL_RING, (DPARAM) &dual_sys, full_file_name, name_len);
#endif
	}
    else
    {
        // 提示格式不支持
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
    }
}
#endif



#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : Set as screen saver
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsScreenSaver (
    uint16     					 index,
    MMIFMM_DATA_INFO_T     *list_data_ptr
)
{
    MMISET_SCREENSAVER_INFO_T ssaver_info = {0};
#ifdef VIDEO_PLAYER_SUPPORT	
    VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;


    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmSetAsScreenSaver, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: [FmmSetAsScreenSaver] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9891_112_2_18_2_20_56_497, (uint8*) "");
        return;
    }

    if (0 == list_data_ptr->data[index]->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }

    ssaver_info.more_ani_saver_info.name_wstr_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, ssaver_info.more_ani_saver_info.name_wstr, MMISET_SCREENSAVER_NAME_MAX_LEN);
#ifdef DRM_SUPPORT

    if (!MMIAPIDRM_CanDrmFileSetToWSPC (ssaver_info.more_ani_saver_info.name_wstr,
                                        ssaver_info.more_ani_saver_info.name_wstr_len, DRMFILE_SETTING_SCREENSAVER))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_SCREENSAVER);
        return;
    }

#endif
    supported_type =  MMIAPIVP_GetSupportedStatus (ssaver_info.more_ani_saver_info.name_wstr,
                      ssaver_info.more_ani_saver_info.name_wstr_len);

    if (VP_SUPPORTED_BOTH       == supported_type ||
            VP_SUPPORTED_VIDEO_ONLY == supported_type)

    {
        ssaver_info.is_open                     = TRUE;
        ssaver_info.ssaver_type                 = MMISET_SCREENSAVER_MORE_ANIMATION;
        MMIAPISET_SetScreenSaverInfo (&ssaver_info);
        //MMIPUB_OpenAlertSuccessWin (TXT_COMPLETE);
    }
	else
#endif
	{
		uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
   		uint16 suffix_len = MMIFMM_FILENAME_LEN;
		MMI_WSTRNCPY(ssaver_info.more_pic_saver_info.name_wstr, MMIFMM_FILENAME_LEN,ssaver_info.more_ani_saver_info.name_wstr,ssaver_info.more_ani_saver_info.name_wstr_len,ssaver_info.more_ani_saver_info.name_wstr_len);
		ssaver_info.more_pic_saver_info.name_wstr_len = ssaver_info.more_ani_saver_info.name_wstr_len;
		MMIAPIFMM_SplitFileName(ssaver_info.more_ani_saver_info.name_wstr, 
			ssaver_info.more_ani_saver_info.name_wstr_len, 
    PNULL, PNULL, suffix_wstr, &suffix_len);

		if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
        {
            ssaver_info.is_open                 = TRUE;
            ssaver_info.ssaver_type             = MMISET_SCREENSAVER_MORE_PICTURE;
			if(GUIANIM_IsSupport(MMIFMM_PIC_PREVIEW_CTRL_ID)
				||MMIAPIFMM_IsPreviewPicSucc(ssaver_info.more_pic_saver_info.name_wstr,
				ssaver_info.more_pic_saver_info.name_wstr_len))
			{
				MMIAPISET_SetScreenSaverInfo(&ssaver_info);
			}
			else
			{//提示该图片设置失败
				SCI_TRACE_LOW("[mmifmm_app.c] MMIFMM_FmmSetAsScreenSaver pic is error");   
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
            
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
       }
	   else
	   {
		   MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
	   }
    }
}
#endif



#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set as screen saver
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsVideoWallpaper (
    uint16     					 index,
    MMIFMM_DATA_INFO_T     *list_data_ptr
)
{
    VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_name_len = 0;
    uint32 file_size = 0;

    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmSetAsVideoWallpaper, index = %d", index);

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: [FmmSetAsVideoWallpaper] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9953_112_2_18_2_20_56_499, (uint8*) "");
        return;
    }

    file_size = list_data_ptr->data[index]->file_size;

    if (0 == file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }

    full_name_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, full_name, MMIFILE_FULL_PATH_MAX_LEN);
#ifdef DRM_SUPPORT

    if (!MMIAPIDRM_CanDrmFileSetToWSPC (full_name, full_name_len, DRMFILE_SETTING_SCREENSAVER))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_VIDEOWALLPAPER);
        return;
    }

#endif
    supported_type =  MMIAPIVP_GetSupportedStatus (full_name, full_name_len);

    if (VP_SUPPORTED_BOTH       == supported_type ||
            VP_SUPPORTED_VIDEO_ONLY == supported_type)

    {
        MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        MMISET_POWER_SETTING_ITEM_T power_saving_setting =  MMIAPISET_GetPowerSavingSetting();//设置视频墙纸直接生效
#endif
        MMIAPISETVWP_AddToVideoWallpaperList (full_name, full_name_len, file_size);
#ifdef MMI_POWER_SAVING_MODE_SUPPORT

        if (! (power_saving_setting.is_fb_anim_wallpaper && power_saving_setting.is_active))
#endif
        {
            SCI_MEMSET (&wallpaper_info, 0, sizeof (MMISET_WALLPAPER_SET_T));
            wallpaper_info.wallpaper_type = MMISET_WALLPAPER_VIDEO;
            wallpaper_info.wallpaper_id = (uint16) (MMISET_WALLPAPER_LIST_VIDEO + 1);
            MMIAPISET_SetWallpaperSetInfo (wallpaper_info);
        }
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        else
        {
            MMIPUB_OpenAlertWarningWin (TXT_VP_VIDEO_WALLPAPER_POWERSAVING_ON);
        }

#endif
    }
    else

    {
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
    }
}
#endif

/*****************************************************************************/
//  Description : FmmSetAsPowerOnOffAnim
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsPowerOnOffAnim (
    uint16     					 index,
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    BOOLEAN     is_power_on_off     //TRUE: power on, FALSE: power off
)
{
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
#ifdef VIDEO_PLAYER_SUPPORT
    VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;
#endif
    uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
    uint16 suffix_len = MMIFMM_FILENAME_LEN;


    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmSetAsPowerOnOffAnim, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: [FmmSetAsPowerOnOffAnim] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10025_112_2_18_2_20_56_501, (uint8*) "");
        return;
    }

    if (0 == list_data_ptr->data[index]->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }

    power_on_off_anim_info.more_anim_info.name_wstr_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, PNULL, index, power_on_off_anim_info.more_anim_info.name_wstr, MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);

    MMIAPIFMM_SplitFileName (power_on_off_anim_info.more_anim_info.name_wstr,
                             power_on_off_anim_info.more_anim_info.name_wstr_len,
                             PNULL, PNULL, suffix_wstr, &suffix_len);


#ifdef DRM_SUPPORT

    if (!MMIAPIDRM_CanDrmFileSetToWSPC (power_on_off_anim_info.more_anim_info.name_wstr,
                                        power_on_off_anim_info.more_anim_info.name_wstr_len, DRMFILE_SETTING_POWERONOFFANIM))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_POWERANIM);
        return;
    }

#endif

#ifdef VIDEO_PLAYER_SUPPORT
    supported_type =  MMIAPIVP_GetSupportedStatus (power_on_off_anim_info.more_anim_info.name_wstr,
                      power_on_off_anim_info.more_anim_info.name_wstr_len);

    if (VP_SUPPORTED_BOTH       == supported_type ||
            VP_SUPPORTED_VIDEO_ONLY == supported_type)

    {
        if (MMIFMM_FILE_TYPE_MOVIE == MMIAPIFMM_ConvertFileType (suffix_wstr, suffix_len)
#ifdef DRM_SUPPORT
                || (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, power_on_off_anim_info.more_anim_info.name_wstr)
                    && MMIFMM_FILE_TYPE_MOVIE == MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, power_on_off_anim_info.more_anim_info.name_wstr))
#endif
           )
        {
            power_on_off_anim_info.anim_type = MMISET_POWER_ON_OFF_ANIM_MORE;
        }

        MMIAPISET_SetPowerOnOffAnimInfo (&power_on_off_anim_info, is_power_on_off);
        //MMIPUB_OpenAlertSuccessWin (TXT_COMPLETE);
    }
    else
#endif
    {
        if (MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType (suffix_wstr, suffix_len))
        {
            power_on_off_anim_info.is_open                 = TRUE;
            power_on_off_anim_info.anim_type = MMISET_POWER_ON_OFF_PIC_MORE;
            if(GUIANIM_IsSupport(MMIFMM_PIC_PREVIEW_CTRL_ID)
				||MMIAPIFMM_IsPreviewPicSucc(power_on_off_anim_info.more_anim_info.name_wstr,
				power_on_off_anim_info.more_anim_info.name_wstr_len))
			{
                MMIAPISET_SetPowerOnOffAnimInfo (&power_on_off_anim_info, is_power_on_off);
            //MMIPUB_OpenAlertSuccessWin (TXT_COMPLETE);
			}
			else
			{
				//提示该图片设置失败
			    SCI_TRACE_LOW("[mmifmm_app.c] MMIFMM_FmmSetAsPowerOnOffAnim pic is error");   
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
        }
        else
        {
            MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
        }
    }
}

/*****************************************************************************/
//  Description : FmmSetAsPowerOnOffAnim
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetAsPowerOnOffAnim (
    BOOLEAN     is_power_on_off,
    const wchar  *full_path_ptr,
    uint16       full_path_len
)
{
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
#ifdef VIDEO_PLAYER_SUPPORT
    VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;
#endif
    uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
    uint16 suffix_len = MMIFMM_FILENAME_LEN;

    if (PNULL == full_path_ptr)
    {
        SCI_TRACE_LOW("MMIAPIFMM_SetAsPowerOnOffAnim full_path_ptr is PNULL !!!");
        return;
    }

    if (0 == full_path_len)
    {
        SCI_TRACE_LOW("MMIAPIFMM_SetAsPowerOnOffAnim full_path_len is 0 !!!");
        return;
    }

    power_on_off_anim_info.more_anim_info.name_wstr_len = full_path_len;
	SCI_MEMCPY(power_on_off_anim_info.more_anim_info.name_wstr, full_path_ptr, full_path_len * sizeof(wchar));

    MMIAPIFMM_SplitFileName (full_path_ptr, full_path_len,
                             PNULL, PNULL, suffix_wstr, &suffix_len);

#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC (full_path_ptr, full_path_len, DRMFILE_SETTING_POWERONOFFANIM))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_POWERANIM);
        return;
    }
#endif

#ifdef VIDEO_PLAYER_SUPPORT
    if (MMIFMM_FILE_TYPE_MOVIE == MMIAPIFMM_ConvertFileType (suffix_wstr, suffix_len)
#ifdef DRM_SUPPORT
            || (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, full_path_ptr)
                && MMIFMM_FILE_TYPE_MOVIE == MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, full_path_ptr))
#endif
       )
    {
        supported_type =  MMIAPIVP_GetSupportedStatus (full_path_ptr, full_path_len);

        power_on_off_anim_info.anim_type = MMISET_POWER_ON_OFF_ANIM_MORE;

        if (VP_SUPPORTED_BOTH       == supported_type ||
                VP_SUPPORTED_VIDEO_ONLY == supported_type)
        {
            MMIAPISET_SetPowerOnOffAnimInfo (&power_on_off_anim_info, is_power_on_off);
            MMIPUB_OpenAlertSuccessWin (TXT_COMPLETE);
        }
    }
    else
#endif
    {
        if (MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType (suffix_wstr, suffix_len))
        {
            power_on_off_anim_info.is_open                 = TRUE;
            power_on_off_anim_info.anim_type = MMISET_POWER_ON_OFF_PIC_MORE;
            MMIAPISET_SetPowerOnOffAnimInfo (&power_on_off_anim_info, is_power_on_off);
            MMIPUB_OpenAlertSuccessWin (TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
        }
    }
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set as pb contact icon
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetPB (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index
)
{
    uint16                  filename_len = 0;
    MMIPB_CUSTOM_PHOTO_T    data_info = {0};
    wchar full_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmSetPB, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetPB] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4244_112_2_18_2_20_43_394, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetPB] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4250_112_2_18_2_20_43_395, (uint8*) "");
        return;
    }

    // 建立全路径文件名
    filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_file_name, MMIFMM_FULL_FILENAME_LEN);

#ifdef DRM_SUPPORT

    //pb与WALLPAPER条件一致
    if (!MMIAPIDRM_CanDrmFileSetToWSPC (full_file_name, filename_len, DRMFILE_SETTING_WALLPAPER))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_PB);
        return;
    }

#endif
    SCI_MEMSET (&data_info, 0, sizeof (MMIPB_CUSTOM_PHOTO_T));
    MMI_WSTRNCPY (data_info.file_name, MMIFILE_FULL_PATH_MAX_LEN, full_file_name, MMIFILE_FULL_PATH_MAX_LEN, filename_len);
    data_info.file_name_len = filename_len;
    data_info.file_size     = list_data_ptr->data[index]->file_size;
    data_info.is_valid      = TRUE;
    data_info.photo_type    = list_data_ptr->data[index]->type;

	if(GUIANIM_IsSupport(MMIFMM_PIC_PREVIEW_CTRL_ID)
		||MMIAPIFMM_IsPreviewPicSucc(data_info.file_name,
				data_info.file_name_len))
	{
        MMIAPIPB_AddToExistContact (&data_info,  MMIPB_ADD_TYPE_PIC, PNULL);
	}
	else
	{
		//提示该图片设置失败
		SCI_TRACE_LOW("[mmifmm_app.c] MMIFMM_FmmSetPB pic is error");   
		MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
}
#endif

/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetWallpaper (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index
)
{

    uint16                  filename_len = 0;
    wchar                   full_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    SCI_TRACE_LOW ("MMIFMM: MMIFMM_FmmSetWallpaper, index = %d", index);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetWallpaper] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4193_112_2_18_2_20_43_391, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmSetWallpaper] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4199_112_2_18_2_20_43_392, (uint8*) "");
        return;
    }

    if(0 == list_data_ptr->data[index]->file_size)
    {
        MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        return;
    }
    
    // 建立全路径文件名
    filename_len = (uint16) MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, index, full_name, MMIFMM_FULL_FILENAME_LEN);

#ifdef DRM_SUPPORT

    if (!MMIAPIDRM_CanDrmFileSetToWSPC (full_name, filename_len, DRMFILE_SETTING_WALLPAPER))
    {
        //不可以设置此DRM文件为壁纸
        MMIPUB_OpenAlertWarningWin (TXT_DRM_NOT_SET_WALLPAPER);
        return;
    }

#endif
#if defined MMI_WALLPAPER_SUPPORT
    if (!MMIAPIFMM_SetPictureToWallpaperEx (FALSE, full_name, filename_len))
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin (TXT_ERROR);
    }
#endif
}


/*****************************************************************************/
//  Description : copy file menu function
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_CopySelectFileToFolder (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    uint16     					 index,
    MMIFMM_PATH_INFO_T*   current_path_ptr
)
{
    uint16   current_index = index;
    wchar    user_scr_path [MMIFMM_PATHNAME_LEN +1] = {0};
    wchar    cur_pathname [MMIFMM_PATHNAME_LEN +1] = {0};
    uint32   i = 0;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
#endif
    //SCI_TRACE_LOW:"MMIFMM: CopySelectFileToFolder"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7672_112_2_18_2_20_50_465, (uint8*) "");

    //记录当前需要复制的文件所在路径
    if (list_data_ptr->path_is_valid > 0)
    {
        MMIAPICOM_Wstrncpy (user_scr_path, list_data_ptr->data[current_index]->filename_ptr,
                            list_data_ptr->data[current_index]->filename_len);

        //去掉后面的文件名，只保留路径
        if (current_index < MMIFMM_FILE_NUM)
        {
            for (i = list_data_ptr->data[current_index]->filename_len; (i < MMIFMM_PATHNAME_LEN) && user_scr_path[i-1] != MMIFILE_SLASH && i > 0; i--)
            {
                user_scr_path[i-1] = 0;
            }
        }
    }
    else
    {
        MMIAPICOM_Wstrncpy (user_scr_path, current_path_ptr->pathname, current_path_ptr->pathname_len);
        user_scr_path[current_path_ptr->pathname_len] = MMIFILE_SLASH;
    }

    if (0 == list_data_ptr->mark_num)          //如果当前无标准，选择高亮文件
    {
#ifdef MMI_PDA_SUPPORT
        GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            MMIPUB_OpenAlertWarningWin (TXT_PLS_SELECT_ONE_ITEM);
        }
        else
        {
#endif

            if (current_index < MMIFMM_FILE_NUM)
            {
                MMIFMM_CombineFullFileName (list_data_ptr, PNULL, current_index, cur_pathname, MMIFMM_PATHNAME_LEN);
#ifdef DRM_SUPPORT

                if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, cur_pathname))
                {
                    uint32 drm_file_type = 0;
                    uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1] = {0};
                    uint16 suffix_len                         = MMIFMM_FILENAME_LEN;
					
                    MMIAPIFMM_SplitFileName(cur_pathname, MMIAPICOM_Wstrlen(cur_pathname), PNULL, PNULL, suffix_wstr, &suffix_len);
                    drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);
					
                    //FL CD类型的DRM文件不允许复制,SD类型的仅允许复制媒体文件，共享版权信息
                    if(drm_file_type != MMIFMM_DCF_FILE)
                    {
                        //如果只有当个文件且是DRM文件，则直接提示并返回，不让copy
						MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                        return;
                    }
                }

#endif
            }

            //MMK_CreateWin((uint32*)MMIFMM_COPY_SELECTFILE_TOFOLDER_WIN_TAB, PNULL);
            MMIFMM_EnterCopyWin (list_data_ptr, current_path_ptr, user_scr_path, cur_pathname, current_index);
#ifdef MMI_PDA_SUPPORT
        }

#endif
    }
    else     //如果有标准
    {
#ifdef DRM_SUPPORT
        {
            int32 marked_drm_file_num = 0;

            //检查标志状态, get drm files num marked
            for (i = 0; i < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
            {
                if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[i]->state)
                {
                    if (MMIFMM_FILE_TYPE_DRM == list_data_ptr->data[i]->type)
                    {
                        uint32 drm_file_type = 0;
                        uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1] = {0};
                        uint16 suffix_len                         = MMIFMM_FILENAME_LEN;

                        MMIAPIFMM_SplitFileName(list_data_ptr->data[i]->filename_ptr, list_data_ptr->data[i]->filename_len, PNULL, PNULL, suffix_wstr, &suffix_len);
                        drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);

                        //FL CD类型的DRM文件不允许复制,SD类型的仅允许复制媒体文件，共享版权信息
                        if(drm_file_type != MMIFMM_DCF_FILE)
                        {
                            marked_drm_file_num++;
                        }
                    }
                }
            }

            if (marked_drm_file_num > 0)
            {
                //means marked files are all drm files, no copy operation are executed
                if (list_data_ptr->mark_num == marked_drm_file_num)
                {
                    MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                    //send message
                    MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_COPY_END_CNF, PNULL, 0);
                    return;
                }
            }
        }
#endif
        //MMK_CreateWin((uint32*)MMIFMM_COPY_SELECTFILE_TOFOLDER_WIN_TAB, PNULL);
        MMIFMM_EnterCopyWin (list_data_ptr, current_path_ptr, user_scr_path, cur_pathname, current_index);
    }
}


/*****************************************************************************/
//  Description : 移动文件或目录
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_MoveSlectFileToFolder (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    uint16     					 index,
    MMIFMM_PATH_INFO_T    *current_path_ptr
)
{
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
#endif
    uint16 current_index = 0;
    wchar  full_path_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};

#ifdef MMI_PDA_SUPPORT
    GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
#endif

    if (0 == list_data_ptr->mark_num)
    {
        current_index = index;

        MMIFMM_CombineFullFileName (list_data_ptr, PNULL, current_index, full_path_name, MMIFMM_PATHNAME_LEN);
#ifdef DRM_SUPPORT

        if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, full_path_name))
        {
            //如果只有当个文件且是DRM文件，则直接提示并返回，不让copy
            MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
            return;
        }

#endif
#ifdef MMI_PDA_SUPPORT

        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            MMIPUB_OpenAlertWarningWin (TXT_PLS_SELECT_ONE_ITEM);
            return;
        }

#endif
    }
    else
    {
#ifdef DRM_SUPPORT
        {
            int32 marked_drm_file_num = 0;
            uint16 i = 0;

            //检查标志状态, get drm files num marked
            for (i = 0; i < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
            {
                if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[i]->state)
                {
                    if (MMIFMM_FILE_TYPE_DRM == list_data_ptr->data[i]->type)
                    {
                        marked_drm_file_num++;
                    }
                }
            }

            if (marked_drm_file_num > 0)
            {
                //means marked files are all drm files, no copy operation are executed
                if (list_data_ptr->mark_num == marked_drm_file_num)
                {
                    MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                    //send message
                    MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
                    return;
                }
            }
        }
#endif
    }

    MMIFMM_EnterMoveWin (list_data_ptr, current_path_ptr);
}





/*****************************************************************************/
// Description : open file
// Global resource dependence :
// Author: xiaoming.ren
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenFile (wchar *full_path_name_ptr)
{
    uint16              full_path_name_len                        = 0;
    uint16              suffix_len                                = MMIFMM_FILENAME_LEN;
    wchar              *suffix_wstr_ptr                           = PNULL;
    MMIFMM_FILE_TYPE_E  file_type                                 = MMIFMM_FILE_TYPE_NORMAL;
    MMIFILE_FILE_INFO_T file_info                                 = {0};

    full_path_name_len = MMIAPICOM_Wstrlen (full_path_name_ptr);

    //SCI_TRACE_LOW:"MMIAPIFMM_OpenFile Enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13453_112_2_18_2_21_3_559, (uint8*) "");

    if (0 == full_path_name_len)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_OpenFile, file name is null"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13457_112_2_18_2_21_3_560, (uint8*) "");
        return;
    }

    MMIAPICOM_WstrTraceOut (full_path_name_ptr, full_path_name_len * sizeof (wchar));

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
        return;
    }

    if (MMIAPIFMM_GetFileInfoFormFullPath (full_path_name_ptr, full_path_name_len, &file_info))
    {
        suffix_wstr_ptr = SCI_ALLOCA ( (MMIFMM_FILENAME_LEN + 1) * sizeof (wchar));

        if (PNULL == suffix_wstr_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_OpenFile Fail, no memory"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13474_112_2_18_2_21_3_561, (uint8*) "");
            return;
        }

        SCI_MEMSET (suffix_wstr_ptr, 0x00, (MMIFMM_FILENAME_LEN + 1) * sizeof (wchar));

        MMIAPIFMM_SplitFileName (file_info.file_name, file_info.file_name_len, PNULL, PNULL, suffix_wstr_ptr, &suffix_len);

        file_type = MMIAPIFMM_ConvertFileType (suffix_wstr_ptr, suffix_len);

#if defined(DRM_SUPPORT)
        //  如果是DRM文件，进一步分析是何种媒体文件
        {
            DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;
            DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status = DRMFILE_PRE_CHECK_NORMAL;

            if (MMIFMM_FILE_TYPE_DRM == file_type)
            {
                file_type = MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, full_path_name_ptr);

                switch (file_type)
                {
                case MMIFMM_FILE_TYPE_PICTURE:
                case  MMIFMM_FILE_TYPE_EBOOK:
                    drm_permission = DRM_PERMISSION_DISPLAY;
                    break;

                case MMIFMM_FILE_TYPE_MUSIC:
                case MMIFMM_FILE_TYPE_MOVIE:
                    drm_permission = DRM_PERMISSION_PLAY;
                    break;

                case MMIFMM_FILE_TYPE_JAVA:
                    drm_permission = DRM_PERMISSION_EXECUTE;
                    break;

                default:
                    break;
                }

                if (DRM_PERMISSION_NONE == drm_permission)
                {
                    MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
                    return;
                }

                pre_check_drmfile_status = MMIAPIDRM_PreCheckFileStatus (full_path_name_ptr, drm_permission);

                if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                {
                    if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                    {
                        //如果是无效的，则需要提示guilist去刷新本行
//                          MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
//                          MMIAPIFMM_UpdateListIconData(ctrl_id, index, list_data_ptr->pathname, s_full_file_name_ptr);
                    }

                    return;
                }
            }
        }
#endif

        switch (file_type)
        {
        case MMIFMM_FILE_TYPE_PICTURE:
            {
                MMIAPIFMM_PreviewPicture (full_path_name_ptr);
            }
            break;
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        case MMIFMM_FILE_TYPE_MUSIC:
            {
                MMIAPIMP3_PlayFile (full_path_name_ptr, (const uint32) full_path_name_len);
            }
            break;
#endif

#ifdef VIDEO_PLAYER_SUPPORT
        case MMIFMM_FILE_TYPE_MOVIE:
            {
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                MMIAPIVP_MiniFunction_PlayVideo (full_path_name_ptr, full_path_name_len);
#else
                MMIAPIFMM_PreviewVideo (full_path_name_ptr);
#endif
            }
            break;
#endif

#ifdef EBOOK_SUPPORT
        case MMIFMM_FILE_TYPE_EBOOK:
            {
                MMIFMM_ShowTxtContent (full_path_name_ptr);
            }
            break;
#endif
#if defined MMI_VCARD_SUPPORT
        case MMIFMM_FILE_TYPE_VCARD:
            {
                //MMIPB_ReadVCardFile(full_path_name_ptr);
                MMIFMM_ShowTxtContent (full_path_name_ptr);
            }
            break;
#endif
        case MMIFMM_FILE_TYPE_JAVA:
            {
#ifdef JAVA_SUPPORT_IA
                MMIAPIJAVA_InstallFromFilesystem (full_path_name_ptr, full_path_name_len);
#elif defined (JAVA_SUPPORT_MYRIAD)
                MMIAPIJAVA_Install (full_path_name_ptr, full_path_name_len);
#endif
            }
            break;
            
#ifdef BROWSER_SUPPORT
        case  MMIFMM_FILE_TYPE_URL:
            OpenUrlFileByBrowserEx(full_path_name_ptr);
            break;
#endif

        default:
            {
                MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
            }
            break;
        }

        SCI_FREE (suffix_wstr_ptr);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin (TXT_COM_FILE_NO_EXIST);
    }
}

/*****************************************************************************/
// Description : update file list
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateFmmList (uint32 device, BOOLEAN is_enter_root)
{
    MMIFMM_UpdateFmmList (device, is_enter_root);
}

/*****************************************************************************/
//  Description : get icon id by fmm data info
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetIconIdByFileType (MMIFMM_FILE_TYPE_E  file_type, MMIFILE_DEVICE_E device)
{
    MMI_IMAGE_ID_T icon_id   = IMAGE_NULL;
    //  BOOLEAN        is_sdcard =( MMI_DEVICE_SDCARD == device ) ? TRUE : FALSE;

    switch (file_type)
    {
    case MMIFMM_FILE_TYPE_PICTURE:
        icon_id = IMAGE_COMMON_PIC_UDISK_ICON;
        break;

    case MMIFMM_FILE_TYPE_MUSIC:
        icon_id = IMAGE_COMMON_MUSIC_UDISK_ICON;
        break;

    case MMIFMM_FILE_TYPE_MOVIE:
        icon_id =  IMAGE_COMMON_MOVIE_UDISK_ICON;
        break;

#ifdef EBOOK_SUPPORT
    case MMIFMM_FILE_TYPE_EBOOK:
        icon_id =  IMAGE_COMMON_TXT_ICON;       //IMAGE_COMMON_TXT_ICON;
        break;
#endif

    case MMIFMM_FILE_TYPE_FOLDER:
        icon_id = IMAGE_FMM_FOLDER_ICON;
        break;
#if defined MMI_VCARD_SUPPORT
    case MMIFMM_FILE_TYPE_VCARD:
        icon_id = IMAGE_COMMON_VCF_ICON;
        break;
#endif
#ifdef JAVA_SUPPORT
    case MMIFMM_FILE_TYPE_JAVA:
        icon_id = IMAGE_COMMON_JAVA_ICON;
        break;
#endif

#ifdef MMI_VCALENDAR_SUPPORT
    case MMIFMM_FILE_TYPE_VCALENDAR:
        icon_id = IMAGE_COMMON_VCS_ICON;
        break;
#endif
 
#ifdef BROWSER_SUPPORT
    case MMIFMM_FILE_TYPE_URL:
        icon_id =  IMAGE_COMMON_URL_ICON;
        break;
#endif

   default:
        icon_id =  IMAGE_FMM_FILE_UDISK_ICON ;
        break;
    }

    return icon_id;
}
/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIFMM_GetIconIdByFileName (
    const wchar        *file_name,
    uint16     file_name_len
)

{
    if (PNULL == file_name || 0 == file_name_len)
    {
        //SCI_TRACE_LOW:"MMIFMM_INTERFACE.C MMIFMM_GetIconIdByFileName filename == NULL"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4123_112_2_18_2_20_27_328, (uint8*) "");
        return IMAGE_FMM_FILE_UDISK_ICON;	//default value
    }

    return GetIconIdByFileName (file_name, file_name_len);
}
/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetIconIdByFileName (
    const wchar        *file_name,
    uint16     file_name_len
)
{

    wchar      suffix_name[FMM_FILE_SUFFIX_MAXLEN] = {0};
    uint16      suffix_len = 0;
    MMI_IMAGE_ID_T image_id =  IMAGE_FMM_FILE_UDISK_ICON ;
    MMIFMM_FILE_TYPE_E   file_type = 0; //SCI_ASSERT(NULL != file_name);

    //获取后缀名
    suffix_len = FMM_FILE_SUFFIX_MAXLEN - 1;

    if (MMIAPIFMM_SplitFileName (file_name, file_name_len,
                                 PNULL, PNULL,
                                 suffix_name, &suffix_len))
    {
        file_type = MMIAPIFMM_ConvertFileType (suffix_name, suffix_len); /*lint !e605*/
#ifdef DRM_SUPPORT

        if (MMIFMM_FILE_TYPE_DRM == file_type)
        {
            image_id = MMIAPIFMM_GetDrmIconId ( (uint16*) file_name);
        }
        else
#endif
            image_id = MMIAPIFMM_GetIconIdByFileType (file_type, MMI_DEVICE_UDISK);
    }

    return image_id;
}

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence :
//  Author:robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_StopOperaSDPlug (void)
{
    MMIFMM_StopOperaSDPlug();
}

/*****************************************************************************/
//  Description :if is FmmMainWinOpen
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFmmMainWinOpen (void)
{
    return MMIFMM_IsFmmMainWinOpen();
}


/*****************************************************************************/
//  Description : 停止复制/删除等文件操作
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ExitFileOption (void)
{
    if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIFMM_COPY_SELECT_FILE_WIN_ID enter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10258_112_2_18_2_20_57_506, (uint8*) "");

        if (MMK_IsOpenWin (MMIFMM_FILE_WAITING_WIN_ID))
        {
            MMK_PostMsg (MMIFMM_FILE_WAITING_WIN_ID, MSG_APP_CANCEL, PNULL, 0);
        }
        else
        {
            MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_APP_CANCEL, PNULL, 0);
        }
    }

    if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIFMM_DEL_SELECT_FILE_WIN_ID enter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10272_112_2_18_2_20_57_507, (uint8*) "");

        if (MMK_IsOpenWin (MMIFMM_FILE_WAITING_WIN_ID))
        {
            MMK_PostMsg (MMIFMM_FILE_WAITING_WIN_ID, MSG_APP_CANCEL, PNULL, 0);
        }
        else
        {
            MMK_PostMsg (MMIFMM_DEL_SELECT_FILE_WIN_ID, MSG_APP_CANCEL, PNULL, 0);
        }
    }

    MMIAPIFMM_SearchFileStop();

}


/*****************************************************************************/
//  Description : 读当前目录中的文件数据，
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetCurrentPathFile (// 所读文件和目录的总数
    const wchar          *full_path_ptr,     // 全路径,查找条件
    MMIFMM_DATA_INFO_T   *data_ptr          // 输出数据
)
{
    uint16                  i = 0;
    uint32                  full_path_len = 0;
    uint32                  find_len = 0;
    uint16                  file_num = 0, folder_num = 0;
    //modified by liyan.zhu for c++ test
    //uint16                  index = 0;
    //uint16                  filename_len = 0;
    wchar                   *find_ptr = PNULL;
    MMIFILE_HANDLE          file_handle = 0;
    SFS_ERROR_E             result = SFS_ERROR_NONE;
    MMIFILE_FIND_DATA_T     find_data = {0};
    //uint16                  suffix_len = 0;
    //wchar                   suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
    MMIFMM_SORT_TYPE_E      sort  = MMIFMM_SORT_TYPE_NAME;
    uint16                  path_depth = 0;
    uint16                  ctlStr[] = L"/A:+NF-H-S";
    uint16                  filter[] = L"*";
    uint16                  *full_file_path_ptr = PNULL;
    //uint16                  full_file_path_len = 0;

    if ( (PNULL == full_path_ptr) || (PNULL == data_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_GetCurrentPathFile param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_983_112_2_18_2_20_7_216, (uint8*) "");
        return 0;
    }

    sort = data_ptr->sort;

    // 计算路径的长度
    full_path_len = MMIAPICOM_Wstrlen (full_path_ptr);

    // 建立查找条件
    find_len  = (full_path_len + 3) * sizeof (wchar);  //需要增加\*,
    find_ptr  = (wchar *) SCI_ALLOC_APP (find_len);

    if (PNULL == find_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: MMIFMM_GetCurrentPathFile, allocate find_ptr failed!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_997_112_2_18_2_20_7_217, (uint8*) "");
        return 0;
    }

    SCI_MEMSET (find_ptr, 0, find_len);
    MMI_WSTRNCPY (find_ptr, find_len, full_path_ptr, full_path_len, full_path_len);

    full_file_path_ptr = (wchar *) SCI_ALLOC_APP (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1));

    if (PNULL == full_file_path_ptr)
    {
        SCI_FREE (find_ptr);
        //SCI_TRACE_LOW:"MMIFMM: MMIFMM_GetCurrentPathFile, allocate full_file_path_ptr failed!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_1008_112_2_18_2_20_7_218, (uint8*) "");
        return 0;
    }

    //get path depth
    path_depth = MMIFMM_GetPathDepth (full_path_ptr, (uint16) full_path_len);

    // 查找第一个数据
    result = SFS_FindFirstFileCtrl (find_ptr, filter, ctlStr, &file_handle, &find_data, PNULL);

    // 查找下一个数据
    if (SFS_INVALID_HANDLE != file_handle && (SFS_ERROR_NONE == result))
    {
        uint16                  index = 0;
        uint16                  filename_len = 0;

        do
        {
            filename_len = (uint16) MMIAPICOM_Wstrlen (find_data.name);

            // 全路径文件名不能超过MMIFMM_FULL_FILENAME_LEN
            if ( (full_path_len + filename_len + 1) <= MMIFMM_FULL_FILENAME_LEN && (filename_len > 0))
            {
                if (PNULL == data_ptr->data[index])
                {
                    data_ptr->data[index] = (MMIFMM_FILE_INFO_T*) MMIFMM_GetFileInfoBuffer();

                    if (PNULL == data_ptr->data[index])
                    {
                        break;
                    }
                }

                SCI_MEMSET (data_ptr->data[index], 0x00, sizeof (MMIFMM_FILE_INFO_T));

                if (data_ptr->path_is_valid > 0)
                {
                    data_ptr->data[index]->filename_len = (uint16) (full_path_len + filename_len + 1);
                }
                else
                {
                    data_ptr->data[index]->filename_len = filename_len;
                }

                if (PNULL != data_ptr->data[index]->filename_ptr)
                {
                    SCI_FREE (data_ptr->data[index]->filename_ptr);
                }

                data_ptr->data[index]->filename_ptr = SCI_ALLOCA ( (sizeof (wchar) * data_ptr->data[index]->filename_len));

                if (PNULL == data_ptr->data[index]->filename_ptr)
                {
                    SCI_FREE (data_ptr->data[index]);
                    data_ptr->data[index] = PNULL;
                    break;
                }

                SCI_MEMSET (data_ptr->data[index]->filename_ptr, 0x00, (sizeof (wchar) *data_ptr->data[index]->filename_len));

                if ( (find_data.attr & SFS_ATTR_DIR) > 0)
                {
                    // 目录数据
                    if (MMIFMM_PATH_DEPTH <= path_depth)
                    {
                        //已经达到最大目录深度
                        SCI_FREE (data_ptr->data[index]->filename_ptr);
                        data_ptr->data[index]->filename_ptr = PNULL;
                        continue;
                    }

                    data_ptr->data[index]->type = MMIFMM_FILE_TYPE_FOLDER;
                    data_ptr->data[index]->file_size = 0;
                    folder_num++;
                }
                else
                {
                    uint16                  full_file_path_len = 0;
                    SCI_MEMSET (full_file_path_ptr, 0, sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1));
                    full_file_path_len = MMIFMM_PATHNAME_LEN;

                    MMI_WSTRNCPY (full_file_path_ptr, full_file_path_len,
                                  full_path_ptr, full_path_len, full_path_len);
                    * (full_file_path_ptr + full_path_len) = MMIFILE_SLASH;
                    MMI_WSTRNCPY (full_file_path_ptr + full_path_len + 1, full_file_path_len - full_path_len - 1,
                                  find_data.name, filename_len, filename_len);
                    full_file_path_len = (uint16) MMIAPICOM_Wstrlen (full_file_path_ptr);

                    // 文件数据
                    if (!MMIFMM_IsFileNeedDisplay (MMIFMM_GetDisplayFileType(), full_file_path_ptr, full_file_path_len))
                    {
                        SCI_FREE (data_ptr->data[index]->filename_ptr);
                        data_ptr->data[index]->filename_ptr = PNULL;
                        continue;
                    }

                    data_ptr->data[index]->type = MMIFMM_FILE_TYPE_NORMAL;
                    data_ptr->data[index]->file_size = find_data.length;

                    file_num++;
                }

                data_ptr->data[index]->time = MMIAPICOM_Tm2Second (find_data.modify_time.sec, find_data.modify_time.min, find_data.modify_time.hour,
                                              find_data.modify_Date.mday, find_data.modify_Date.mon, find_data.modify_Date.year);

                if (data_ptr->path_is_valid > 0)
                {
                    // 包含路径
                    MMI_WSTRNCPY (data_ptr->data[index]->filename_ptr,
                                  data_ptr->data[index]->filename_len,
                                  full_path_ptr, full_path_len, full_path_len);
                    data_ptr->data[index]->filename_ptr[full_path_len] = MMIFILE_SLASH;
                    MMI_WSTRNCPY (& (data_ptr->data[index]->filename_ptr[full_path_len+1]), data_ptr->data[index]->filename_len - full_path_len - 1,
                                  find_data.name, filename_len, filename_len);
                }
                else
                {
                    // 不包含路径
                    MMI_WSTRNCPY (data_ptr->data[index]->filename_ptr, data_ptr->data[index]->filename_len,
                                  find_data.name, filename_len, filename_len);

                }

                // 普通文件需要进一步解析文件类型
                if (MMIFMM_FILE_TYPE_NORMAL == data_ptr->data[index]->type)
                {
                    uint16                  suffix_len = 0;
                    wchar                   suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
                    SCI_MEMSET (suffix_name , 0, ( (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar)));
                    suffix_len = MMIFMM_FILENAME_LEN;

                    if (MMIAPIFMM_SplitFileName (find_data.name, filename_len,
                                                 PNULL, PNULL,
                                                 suffix_name, &suffix_len))
                    {
                        data_ptr->data[index]->type = MMIAPIFMM_ConvertFileType (suffix_name, suffix_len);
                    }
                }
            }

            if (MMIFMM_FILE_NUM == (file_num + folder_num))
            {
                break;
            }

            index = (uint16) (file_num + folder_num);
        }
        while ( (SFS_NO_ERROR == MMIAPIFMM_FindNextFile (file_handle, &find_data)));

        MMIAPIFMM_FindClose (file_handle);
        data_ptr->file_num = file_num;
        data_ptr->folder_num = folder_num;
    }

    SCI_FREE (find_ptr);
    SCI_FREE (full_file_path_ptr);

    i = (uint16) (folder_num + file_num);

    if (i > 0)
    {
        if (MMIFMM_FilePreSort (data_ptr))
        {
            MMIAPIFMM_FileDataSort (data_ptr, sort);
        }
    }

    return (i);
}

/*****************************************************************************/
//  Description : 更新当前的文件列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateCurrentFileLIst (void)
{
    //update file list
    MMK_CloseWin (MMIFMM_PICTURE_PREVIEW_WIN_ID);
    MMK_CloseWin (MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID);
    MMK_SendMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_UPDATE_LIST_DATA, (ADD_DATA) TRUE);


    if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_FMM_OPENFILEWIN_RELOAD, NULL, 0);
    }
}
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : update current list item
//  Global resource dependence :
//  Author: liyan.zhu
//  Note:copy from mmifmm.c MMIAPIFMM_UpdateCurrentFileLIst
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateCurrentLIstItem(void)
{                        
    //update file list
    MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);
	MMK_CloseWin(MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID);
	MMK_SendMsg(MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_LIST_REPLACE_ITEM_DATA, PNULL);
	
	
    if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_BYLAYER_WIN_ID))
    {
        MMK_PostMsg(MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_FMM_OPENFILEWIN_RELOAD, NULL, 0);
    }
}
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : set music file to ring
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetMusicToRing (
    MMIFMM_SET_RING_TYPE_E   ring_type,
    DPARAM                      param,
    uint16                      *full_name_ptr,
    uint16                      full_name_len
)
{
    uint32          file_size = 0;
    BOOLEAN         is_file_exist = FALSE;
#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif
    MN_DUAL_SYS_E                s_set_ring_dual_sys = MN_DUAL_SYS_1;
    uint16                       s_set_ring_alarm_id = 0;
    MMIMULTIM_SET_RING_TYPE_E    s_set_ring_type = MMIMULTIM_SET_RING_NULL;

    //SCI_ASSERT(NULL != full_name_ptr);
    if (PNULL == full_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_SetMusicToRing param error: PNULL == full_name_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5015_112_2_18_2_20_16_254, (uint8*) "");
        return;
    }

    s_set_ring_type = ring_type;

    switch (ring_type)
    {
    case MMIMULTIM_SET_CALL_RING:
        s_set_ring_dual_sys = * (MN_DUAL_SYS_E *) param;
        break;
    case MMIMULTIM_SET_MSG_RING:
        break;
    case MMIMULTIM_SET_ALARM_RING:
        s_set_ring_alarm_id = * (uint16 *) param;
        break;
    default:
        break;
    }

    is_file_exist = MMIAPIFMM_IsFileExist (full_name_ptr, full_name_len);

    if (is_file_exist)
    {
        MMIAPIFMM_GetFileInfo (full_name_ptr, full_name_len, &file_size, NULL, NULL);
    }

    if (is_file_exist && 0 < file_size)
    {
        MMISRVAUD_CONTENT_INFO_T        content_info = {0};
        MMISET_CALL_MORE_RING_T      s_set_ring_info = {0};      //设置铃声文件
        wchar           file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
        uint16          file_name_len = 0;

        MMIAPIFMM_SplitFullPath (
            full_name_ptr, full_name_len,
            NULL, NULL, NULL, NULL,
            file_name, &file_name_len);

#ifdef DRM_SUPPORT

        if (MMIAPIMP3_IsDrmFile (
                    full_name_ptr, full_name_len,
                    &drm_audio_type, &is_audio_drm_file, &is_drm_valid
                ))
        {
            if (!is_drm_valid)
            {
                MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);
                return;
            }
            else if (!is_audio_drm_file)
            {
                MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
                return;
            }
        }

#endif

        SCI_MEMSET (&s_set_ring_info, 0, sizeof (s_set_ring_info));
        MMI_WSTRNCPY (s_set_ring_info.name_wstr, MMISET_CALL_RING_NAME_MAX_LEN,
                      full_name_ptr, full_name_len, full_name_len);
        s_set_ring_info.name_wstr_len = (uint16) MIN (MMISET_CALL_RING_NAME_MAX_LEN, full_name_len);

        MMIAPISET_StopAppRing();

        if (MMISRVAUD_GetFileContentInfo (
                    s_set_ring_info.name_wstr,
                    (uint32) s_set_ring_info.name_wstr_len,
                    &content_info))
        {
            BOOLEAN                     is_success = TRUE;

            switch (s_set_ring_type)
            {
            case MMIMULTIM_SET_CALL_RING:
                //s_set_ring_info.name_wstr_len = s_set_ring_info.name_wstr_len;//coverity 12597

                if (!MMIAPIENVSET_SetMoreCallRingInfo (s_set_ring_dual_sys, s_set_ring_info, MMIAPIENVSET_GetActiveModeId()))
                {
                    is_success = FALSE;
                }

                break;
            case MMIMULTIM_SET_MSG_RING:
                break;
            case MMIMULTIM_SET_ALARM_RING:

                if (!MMIAPIALARM_SetAlarmRing (s_set_ring_alarm_id, s_set_ring_info.name_wstr, s_set_ring_info.name_wstr_len))
                {
                    is_success = FALSE;
                }

                break;
            default:
                break;
            }

            if (is_success)
            {
                MMIPUB_OpenAlertSuccessWin (TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin (TXT_NO_SPACE);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SetMusicToRing: create audio file handle error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5119_112_2_18_2_20_16_255, (uint8*) "");
//#ifdef MMI_AUDIO_PLAYER_SUPPORT
            MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
//#endif
        }
    }
    else
    {
        if (!is_file_exist)
        {
            MMIPUB_OpenAlertWarningWin (TXT_COM_FILE_NO_EXIST);
        }
        else if (0 == file_size)
        {
            MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
        }
    }
}
#endif
/*****************************************************************************/
//  Description : set win title
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetFileNameWinTitle (
    MMI_WIN_ID_T    win_id,
    const wchar     *file_name_ptr,
    uint16          file_name_len,
    BOOLEAN         is_refresh
)
{
    wchar                       name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      name_len = 0;
    uint16                      title_max_len = 0;

    //SCI_ASSERT(NULL != file_name_ptr);
    //SCI_ASSERT(0 < file_name_len);
    if ( (PNULL == file_name_ptr) || (0 == file_name_len))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_SetFileNameWinTitle param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5157_112_2_18_2_20_16_256, (uint8*) "");
        return;
    }

    title_max_len = MMITHEME_GetWinTitleMaxLen();

    MMI_WSTRNCPY (name, MMIFILE_FILE_NAME_MAX_LEN, file_name_ptr, file_name_len, file_name_len);
    name_len = file_name_len;

    if (title_max_len < name_len && title_max_len> MMIFMM_ELLIPSIS_LEN)
    {
        name_len = title_max_len;

        MMI_WSTRNCPY (&name[title_max_len-MMIFMM_ELLIPSIS_LEN], MMIFMM_ELLIPSIS_LEN,
                      MMIFMM_ELLIPSIS, MMIFMM_ELLIPSIS_LEN, MMIFMM_ELLIPSIS_LEN);
    }

    GUIWIN_SetTitleText (win_id, name, name_len, is_refresh);
    GUIWIN_SetTitleTextAlign (win_id, ALIGN_HMIDDLE);
}

/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: aoke.hu
//  Note: add is_consume para
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_PlayMusicFileEx (
    MMISET_ALL_RING_TYPE_E         ring_type,
    BOOLEAN                        is_a2dp,
    BOOLEAN                        is_consume,
    const wchar                    *full_path_ptr,
    uint16                         full_path_len,
    uint32                         play_times,
    uint32                         play_offset,
    uint32                         vol,
    MMISET_RING_CALLBACK_PFUNC    call_back
)
{
#ifdef MMISRV_AUDIO_SUPPORT
    return MMIAPISET_PlayMusicFile (ring_type, is_a2dp, is_consume, full_path_ptr, full_path_len, play_times, play_offset, vol, call_back);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_PlayMusicFile (
    MMISET_ALL_RING_TYPE_E         ring_type,
    BOOLEAN                        is_a2dp,
    const wchar                    *full_path_ptr,
    uint16                         full_path_len,
    uint32                         play_times,
    uint32                         play_offset,
    uint32                         vol,
    MMISET_RING_CALLBACK_PFUNC    call_back
)
{
    return MMIAPIFMM_PlayMusicFileEx (ring_type, is_a2dp, TRUE, full_path_ptr, full_path_len, play_times, play_offset, vol, call_back);
}


/*****************************************************************************/
//  Description : get file type
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFMM_GetFileType (
    const wchar      *suffix_name_ptr,
    uint16      suffix_name_len
)
{
    uint32 i = 0;
    uint8 gb_suffix_name[MMIFMM_FILENAME_LEN + 1] = {0};
    uint16 gb_suffix_name_len = 0;

    //gb_suffix_name_len = GUI_UCS2GB((uint8 *) gb_suffix_name, (uint8 *) suffix_name_ptr, suffix_name_len);
    gb_suffix_name_len = suffix_name_len;
    MMI_WSTRNTOSTR (gb_suffix_name, MMIFMM_FILENAME_LEN, suffix_name_ptr, suffix_name_len, suffix_name_len);

    //忽略大小写
    for (i = 0; i < gb_suffix_name_len; i++)
    {
        if ('A' <= gb_suffix_name[i] && 'Z' >= gb_suffix_name[i])
        {
            gb_suffix_name[i] = (uint8) (gb_suffix_name[i] - 'A' + 'a');
        }
    }

    for (i = 0; i < ARR_SIZE (s_fmm_all_suffix); i++)
    {
        if (suffix_name_len == strlen ( (char*) s_fmm_all_suffix[i].suffix))
        {
            if (0 == MMIAPICOM_Stricmp ( (uint8 *) gb_suffix_name, (uint8 *) s_fmm_all_suffix[i].suffix))
            {
                if (s_fmm_all_suffix[i].is_support)
                {
                    return s_fmm_all_suffix[i].type;
                }
                else
                {
                    return MMIFMM_UNKNOW_TYPE;
                }
            }
        }
    }

    return  MMIFMM_UNKNOW_TYPE;
}

/*****************************************************************************/
//  Description : get file type
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_ConvertFileType (
    const wchar      *suffix_name_ptr,
    uint16      suffix_name_len
)
{
    MMIFMM_FILE_TYPE_E type = MMIFMM_FILE_TYPE_NORMAL;

    switch (MMIAPIFMM_GetFileType (suffix_name_ptr, suffix_name_len))
    {
    case MMIFMM_PICTURE_JPG:
    case MMIFMM_PICTURE_GIF:
    case MMIFMM_PICTURE_BMP:
    case MMIFMM_PICTURE_WBMP:
    case MMIFMM_PICTURE_PNG:
        type = MMIFMM_FILE_TYPE_PICTURE;
        break;
    case MMIFMM_MUSIC_MP3:
    case MMIFMM_MUSIC_WMA:
    case MMIFMM_MUSIC_MID:
    case MMIFMM_MUSIC_AMR:
    case MMIFMM_MUSIC_ACC:
    case MMIFMM_MUSIC_M4A:
    case MMIFMM_MUSIC_WAV:
        //case MMIFMM_MUSIC_MIDI: ==MMIFMM_MUSIC_MID
//        case MMIFMM_MOVIE_KUR:
        type = MMIFMM_FILE_TYPE_MUSIC;
        break;
#ifdef VIDEO_PLAYER_SUPPORT
    case MMIFMM_MOVIE_RMVB:
    case MMIFMM_MOVIE_MP4:
    case MMIFMM_MOVIE_3GP:
    case MMIFMM_MOVIE_AVI:
    case MMIFMM_MOVIE_FLV:
#ifdef MMI_KING_MOVIE_SUPPORT
    case MMIFMM_MOVIE_KMV:
    case MMIFMM_MOVIE_SMV: //smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    case MMIFMM_MOVIE_HMV:
#endif
#endif
        type = MMIFMM_FILE_TYPE_MOVIE;
        break;
#endif
#ifdef EBOOK_SUPPORT
    case MMIFMM_MOVIE_TXT:
    case MMIFMM_MOVIE_LRC:
        type = MMIFMM_FILE_TYPE_EBOOK;
        break;
#endif
#if defined MMI_VCARD_SUPPORT
    case MMIFMM_MOVIE_VCF:
        type = MMIFMM_FILE_TYPE_VCARD;
        break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    case MMIFMM_MOVIE_VCS:
        type = MMIFMM_FILE_TYPE_VCALENDAR;
        break;
#endif
    case MMIFMM_MOVIE_SZIP:
        type = MMIFMM_FILE_TYPE_SZIP;
        break;
    case MMIFMM_FONT:
        type = MMIFMM_FILE_TYPE_FONT;
        break;
#ifdef  JAVA_SUPPORT
    case MMIFMM_JAVA_JAD:
    case MMIFMM_JAVA_JAR:
        type = MMIFMM_FILE_TYPE_JAVA;
        break;
#endif
#ifdef	QBTHEME_SUPPORT
    case MMIFMM_THEME_QB:
        type = MMIFMM_FILE_TYPE_THEME;
        break;
#endif
    case MMIFMM_BROWSER_HTM:
    case MMIFMM_BROWSER_HTML:
    case MMIFMM_BROWSER_XML :
        type = MMIFMM_FILE_TYPE_BROWSER;
        break;

#ifdef DRM_SUPPORT
    case MMIFMM_DM_FILE:
    case MMIFMM_DCF_FILE:
        type = MMIFMM_FILE_TYPE_DRM;
        break;
#endif

    case MMIFMM_BROWSER_URL:
        type = MMIFMM_FILE_TYPE_URL;	
        break;
            
    case MMIFMM_UNKNOW_TYPE:
    default:
        type = MMIFMM_FILE_TYPE_NORMAL;
        break;
    }

    return type;
}
/*****************************************************************************/
//  Description : get movie type
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMICOM_VIDEO_TYPE_E MMIAPIFMM_ConvertMovieFileType (
    const wchar    *suffix_name_ptr,
    uint16    suffix_name_len
)
{

    MMICOM_VIDEO_TYPE_E  type = MMICOM_VIDEO_TYPE_MAX;

    switch (MMIAPIFMM_GetFileType (suffix_name_ptr, suffix_name_len))
    {
    case MMIFMM_MOVIE_MP4:
        type = MMICOM_VIDEO_TYPE_MP4;
        break;
    case MMIFMM_MOVIE_3GP:
        type = MMICOM_VIDEO_TYPE_3GP;
        break;
#ifdef MMI_KING_MOVIE_SUPPORT
    case MMIFMM_MOVIE_KMV:
        type = MMICOM_VIDEO_TYPE_KMV;
        break;
    case MMIFMM_MOVIE_SMV://smv_support
        type = MMICOM_VIDEO_TYPE_SMV;
        break;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    case MMIFMM_MOVIE_HMV://hmv_support
        type = MMICOM_VIDEO_TYPE_HMV;
        break;
#endif
#endif

#ifdef AVI_DEC_SUPPORT
    case MMIFMM_MOVIE_AVI:
        type = MMICOM_VIDEO_TYPE_AVI;
        break;
#endif
    case MMIFMM_MOVIE_FLV:
        type = MMICOM_VIDEO_TYPE_FLV;
        break;

    case MMIFMM_MOVIE_RMVB:
        type = MMICOM_VIDEO_TYPE_RMVB;
        break;
    default:
        type = MMICOM_VIDEO_TYPE_MAX;
        break;
    }

    return (type);
}

/*****************************************************************************/
//  Description : get music type
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIFMM_ConvertMusicFileType (
    const wchar    *suffix_name_ptr,
    uint16    suffix_name_len
)
{
    MMISRVAUD_RING_FMT_E  type = MMISRVAUD_RING_FMT_MAX;
    uint32 			file_type = 0;
    file_type = MMIAPIFMM_GetFileType (suffix_name_ptr, suffix_name_len);

    switch (file_type)
    {
    case MMIFMM_MUSIC_MP3:
        type = MMISRVAUD_RING_FMT_MP3;
        break;
    case MMIFMM_MUSIC_WMA:
        type = MMISRVAUD_RING_FMT_WMA;
        break;
        //case MMIFMM_MUSIC_MIDI: == MMIFMM_MUSIC_MID
    case MMIFMM_MUSIC_MID:
        type = MMISRVAUD_RING_FMT_MIDI;
        break;
    case MMIFMM_MUSIC_AMR:
        type = MMISRVAUD_RING_FMT_AMR;
        break;
    case MMIFMM_MUSIC_ACC:
        type = MMISRVAUD_RING_FMT_AAC;
        break;
    case MMIFMM_MUSIC_M4A:
        type = MMISRVAUD_RING_FMT_M4A;
        break;
    case MMIFMM_MUSIC_WAV:
        type = MMISRVAUD_RING_FMT_WAVE;
        break;
//        case MMIFMM_MOVIE_KUR:
//            type = MMISRVAUD_RING_FMT_MAX;
//            break;
    default:
        type = MMISRVAUD_RING_FMT_MAX;
        break;
    }

    return  type;
}

/*****************************************************************************/
//  Description : get the type of folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FOLDER_TYPE_E MMIAPIFMM_GetFolderType (
    const wchar      *path_ptr,
    uint16      path_len,
    BOOLEAN     is_include_path
)
{
    uint16     offset = 0;

    //SCI_TRACE_LOW:"MMIFMM: MMIAPIFMM_GetFolderType, path len = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_1637_112_2_18_2_20_9_219, (uint8*) "d", path_len);

    //SCI_ASSERT(PNULL != path_ptr);
    //SCI_PASSERT(path_len > 0,(("path_len = %d"),path_len));
    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_GetFolderType PNULL == path_ptr is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_1642_112_2_18_2_20_9_220, (uint8*) "");
        return MMIFMM_FOLDER_TYPE_NUM;
    }

    if (0 == path_len)
    {
        //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_GetFolderType path_len = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_1648_112_2_18_2_20_9_221, (uint8*) "d", path_len);
        return MMIFMM_FOLDER_TYPE_NUM;
    }

    // 是否包含路径
    if (is_include_path)
    {
        offset = MMIFMM_PATH_NAME_OFFSET;
    }

#ifdef CMCC_UI_STYLE

    // 判断是否是Photos
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_PHOTOS, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_PICTURE;
        }
    }

#endif

    // 判断是否是Photos
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_PICTURE;
        }
    }

    // 判断是否是Audio
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MUSIC) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MUSIC), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_MUSIC;
        }
    }


    // 判断是否是Video
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_MOVIE;
        }
    }

#ifdef MMI_KING_MOVIE_SUPPORT

    // 判断是否是KingMovie
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_KINGMOVIE) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_KINGMOVIE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_KINGMOVIE), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_KINGMOVIE;
        }
    }

#endif
#ifdef EBOOK_SUPPORT

    // 判断是否是EBOOK
    if (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_EBOOK) == (path_len - offset)) // path 去掉设备缅（1)， ":\"(2)长度
    {
        if (0 == MMIAPICOM_CompareTwoWstrExt (&path_ptr[offset], (uint16) (path_len - offset),
                                              MMIMULTIM_DIR_EBOOK, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_EBOOK), FALSE))
        {
            return MMIFMM_FOLDER_TYPE_EBOOK;
        }
    }

#endif

    return (MMIFMM_FOLDER_TYPE_NORMAL);
}

/*****************************************************************************/
//  Description : 把搜索类型转化成字符串形式
//  Global resource dependence :
//  Author:liqingpeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetFilerInfo (MMIFMM_FILTER_T * filter, uint32 suffix_type)
{
    int32 i = 0;
    int32 total_count = 0;
    uint32       str_len = 0;

    if (MMIFMM_FILE_ALL == suffix_type)
    {
        MMI_MEMCPY (filter->filter_str , FMM_FILTER_STRING_MAXLEN ,
                    "*.*", 3, 3);
        return;
    }

    total_count = ARR_SIZE (s_fmm_all_suffix);

    for (i = 0; i < total_count; i++)
    {

        if ( ( (MMIFMM_DRM_ALL & MMIFMM_MEDIA_TYPE (s_fmm_all_suffix[i].type)) //drm type
                || (MMIFMM_FILE_TYPE (suffix_type) == MMIFMM_FILE_TYPE (s_fmm_all_suffix[i].type)) //大类别相同
             ) //drm type or same file type
                && (MMIFMM_MEDIA_TYPE (suffix_type) &MMIFMM_MEDIA_TYPE (s_fmm_all_suffix[i].type)) //小类别包含
                && (s_fmm_all_suffix[i].is_support)
           )
        {
            str_len = strlen ( (char*) filter->filter_str);
            MMI_MEMCPY (filter->filter_str + str_len, (FMM_FILTER_STRING_MAXLEN - str_len),
                        "*.", 2, 2);

            str_len += 2;
            MMI_MEMCPY (filter->filter_str + str_len, (FMM_FILTER_STRING_MAXLEN - str_len),
                        s_fmm_all_suffix[i].suffix, strlen ( (char*) s_fmm_all_suffix[i].suffix), strlen ( (char*) s_fmm_all_suffix[i].suffix));/*lint !e666*/

            str_len = strlen ( (char*) filter->filter_str);
            MMI_MEMCPY (filter->filter_str + str_len, (FMM_FILTER_STRING_MAXLEN - str_len),
                        " ", 1, 1);
        }
    }
}

/*****************************************************************************/
//  Description : 通过给定的字符，查出当前支持的后缀类型
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_GetSuffixTypeInfo (char *filter_str, uint32 *suffix_type_ptr)
{
    uint16 i = 0;
    uint32 suffix_type = 0;

    if (PNULL == filter_str
            || PNULL == suffix_type_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIFMM_GetSuffixTypeInfo: invalid param!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5896_112_2_18_2_20_17_262, (uint8*) "");
        return;
    }

    if (0 == strcmp (filter_str, "*.*")
            || 0 == strcmp (filter_str, "*"))
    {
        //如果是*.*，则所有的都支持
        suffix_type = MMIFMM_FILE_ALL;
        *suffix_type_ptr = suffix_type;
        //SCI_TRACE_LOW:"[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set all file type is %x!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5906_112_2_18_2_20_17_263, (uint8*) "d", suffix_type);
        return;
    }

    for (i = 0; i < ARR_SIZE (s_fmm_all_suffix); i++)
    {
        char *find_ptr = PNULL;

        if (s_fmm_all_suffix[i].is_support)
        {
            find_ptr = strstr (filter_str, (char*) s_fmm_all_suffix[i].suffix);

            if (PNULL != find_ptr)
            {
                suffix_type |= s_fmm_all_suffix[i].type;
            }
        }
    }

    *suffix_type_ptr = suffix_type;

    //SCI_TRACE_LOW:"[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set file type is %x!"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5926_112_2_18_2_20_17_264, (uint8*) "d", suffix_type);
    return;
}

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : open url file by browser
//  Global resource dependence :
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenUrlFile(
                MMIFMM_DATA_INFO_T  *list_data_ptr,
                MMIFMM_PATH_INFO_T  * current_path_ptr,
                uint16  index
                )
{
    wchar full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return;
    }

    MMIFMM_CombineFullFileName(list_data_ptr,PNULL,index,full_file_name,MMIFMM_FILENAME_LEN);
    OpenUrlFileByBrowserEx(full_file_name);
    return;
}

/*****************************************************************************/
//  Description : open url file by browser
//  Global resource dependence :
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void OpenUrlFileByBrowserEx(
                            wchar *full_file_name_ptr
                            )
{
    uint8   *buf_ptr = PNULL;
    uint32 url_len = 0;
    uint8   *url_temp_buf_ptr = PNULL;
    uint8   *url_buf_ptr = PNULL;
    uint32 full_file_name_len = 0;
    uint32 file_size = 0;
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    if (PNULL == full_file_name_ptr)
    {
        return;
    }

    full_file_name_len = MMIAPICOM_Wstrlen(full_file_name_ptr);
    if (MMIAPIFMM_GetFileInfo(full_file_name_ptr, full_file_name_len, &file_size, PNULL, PNULL))
    {
        if (file_size > 10*1024)/*size需要限制大小,避免出错时申请过大内存*/
        {
            SCI_TRACE_LOW("[BRW]W OpenUrlFileByBrowser:file_size =%d", file_size);
            return;
        }
    
        buf_ptr = SCI_ALLOCAZ(file_size + 1);
        if (PNULL == buf_ptr)
        {
            SCI_TRACE_LOW("[BRW]W OpenUrlFileByBrowser:buf_ptr ALLOC FAIL");
            return;
        }
        
        if(MMIAPIFMM_ReadFilesDataSyn(full_file_name_ptr, full_file_name_len, buf_ptr, file_size))
        {
            url_temp_buf_ptr = MMIAPIBROWSER_GetUrlByFile(buf_ptr, file_size, &url_len);
            if ((0 != url_len) && (PNULL != url_temp_buf_ptr))
            {
                url_buf_ptr = SCI_ALLOCAZ(url_len + 1);
                if (url_buf_ptr != PNULL)
                {
                    SCI_MEMCPY(url_buf_ptr, url_temp_buf_ptr, url_len);
                    
                    entry_param.type = MMIBROWSER_ACCESS_URL;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = (char *)url_buf_ptr;
                    MMIAPIBROWSER_Entry(&entry_param);
                    
                    SCI_FREE(url_buf_ptr);
                }
                else
                {
                    SCI_TRACE_LOW("[BRW]W OpenUrlFileByBrowser:url_buf_ptr ALLOC FAIL");
                }
            }            
        }
    
        SCI_FREE(buf_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : get total and used space of t/sd card
//	Global resource dependence :
//  Author:zhewei.tang
//	Note:05_31
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetTotalAndUsedInExternalMem(
            uint32     *used_space_low ,
            uint32    *used_space_high ,
            uint32    *total_space_high ,
            uint32    *total_space_low
    )
{
    return FALSE;   // dummy, because this interface just for samsung
}
#endif

