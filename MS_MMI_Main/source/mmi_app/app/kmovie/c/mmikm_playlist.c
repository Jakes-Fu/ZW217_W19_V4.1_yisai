/***************************************************************************
** File Name:      mmikm_playlist.c                                        *
** Author:                                                                 *
** Date:           07/14/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen          Create                              *
**																		   *
****************************************************************************/
/**-------------------------------------------------------------------------*/
/**                         Include Files                                   */
/**-------------------------------------------------------------------------*/

#include "mmi_app_kmovie_trc.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmipub.h"
#include "guistring.h"

#include "mmifmm_export.h"
#include "mmisd_export.h"
#include "mmk_type.h"
#include "mmifilearray_export.h"
#include "mmi_appmsg.h"
#include "mmikm_internal.h"
#ifndef KM_DEBUG_SWITCH_ENABLE
#include "video_security.h"
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND
#include "video_security_kmv.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
#include "video_security_hmv.h"
#endif
#endif
#include "mmikm_menutable.h"
#include "mmikm_id.h"
#include "mmikm_text.h"
#include "mmikm_export.h"
#include "mmikm_playlist.h"
#include "mmikm_text.h"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                  */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMIFILE_DEVICE_E       dev_type;
} MMIKM_RESERVE_LIST_T;

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
//存放路径文件夹"KM_res" 
#define MMIKM_RESERVED_DIR                (s_mmikm_reserved_dir)//KM_res
#define MMIKM_RESERVED_DIR_LEN            6
//播放列表名称 "km_playlist.dat"
#define MMIKM_PLAY_LIST_NAME              (s_mmikm_playlist_wstr)//km_playlist.dat
#define MMIKM_PLAY_LIST_NAME_LEN          15
//播放列表详细信息名称 "km_playlist_infor.dat"
#define MMIKM_PLAY_LIST_INFOR_NAME        (s_mmikm_playlist_infor_wstr)//km_playlist_infor.dat
#define MMIKM_PLAY_LIST_INFOR_NAME_LEN    21
//最近播放文件"km_play_infor.dat"
#define MMIKM_PLAY_INFOR_NAME             (s_mmikm_play_infor_wstr)//km_play_infor.dat
#define MMIKM_PLAY_INFOR_NAME_LEN         17

#ifdef  KM_DEBUG_SWITCH_ENABLE 
//data.dat
#define MMIKM_TEST_FILE_NAME              (s_test_data_file)//D:\data.dat
#define MMIKM_TEST_FILE_NAME_LEN          11
#endif

#define MMIKM_STORE_FILE_DIR_LEN         (MMIMULTIM_DIR_KINGMOVIE_LEN + 1)
#define MMIKM_STORE_FILE_DIR_STR        g_mmimultim_kingmovie_dir_wstr//L"kingmovie" 

#define KM_POS_BIAS '\\'   //"\x00\x2f" // "\" 反斜线
#define KM_POS_DIOT '.'   //'.' 文件后缀名

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
LOCAL MMIKM_RESERVE_LIST_T s_play_list_dev = {0};

LOCAL const wchar s_mmikm_reserved_dir[]        = { 'K', 'M', '_', 'r', 'e', 's' ,0 }; 
LOCAL const wchar s_mmikm_playlist_wstr[]       = { 'k', 'm', '_', 'p', 'l', 'a', 'y', 'l', 'i', 's', 't', '.', 'd', 'a', 't' ,0 };
LOCAL const wchar s_mmikm_playlist_infor_wstr[] = { 'k', 'm', '_', 'p', 'l', 'a', 'y', 'l', 'i', 's', 't', '_', 'i', 'n', 'f', 'o', 'r', '.', 'd', 'a', 't' ,0 };
LOCAL const wchar s_mmikm_play_infor_wstr[]     = { 'k', 'm', '_', 'p', 'l', 'a', 'y', '_', 'i', 'n', 'f', 'o', 'r', '.', 'd', 'a', 't' ,0 };

#ifdef  KM_DEBUG_SWITCH_ENABLE
LOCAL const wchar s_test_data_file[] = { 'D',':','\\','d','a','t','a','.','d', 'a', 't' ,0 };
#endif

LOCAL uint32      s_playlist_num = 0;

LOCAL const  KM_SUFFIX_T s_km_all_suffix[MMIKM_VIDEO_TYPE_MAX] =     //视频的支持情况
{
    {"kmv",MMIKM_VIDEO_TYPE_KMV,TRUE},
    {"smv",MMIKM_VIDEO_TYPE_SMV,TRUE},  //smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    {"hmv",MMIKM_VIDEO_TYPE_HMV,TRUE}  //hmv_support
#endif
};

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
LOCAL FILEARRAY  s_km_play_list_array_ptr[KM_PLAYLIST_MAX]   = {PNULL};//for Udisk
LOCAL FILEARRAY  s_km_play_list_array_sd1_ptr[KM_PLAYLIST_MAX]   = {PNULL};
#else
LOCAL FILEARRAY  s_km_play_list_array_ptr[KM_PLAYLIST_MAX]   = {PNULL};  
#endif
LOCAL FILEARRAY  s_km_play_list_array_sd2_ptr[KM_PLAYLIST_MAX]   = {PNULL};  
LOCAL MMIKM_PLAY_LIST_INFO_T  *s_km_play_list_info_ptr = PNULL;
LOCAL MMIKM_PLAY_INFO_T       s_km_play_info = {0}; 

#ifndef KM_DEBUG_SWITCH_ENABLE
LOCAL  VPLAYER_SECURITY_HANDLE s_vplayer_sec_handle = PNULL;
LOCAL  void   *s_vdec_sec_argv[2] = {NULL, NULL};
LOCAL   BOOLEAN  s_sprd_kmv = FALSE;//andy.he_kingmv
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
LOCAL   BOOLEAN  s_sprd_hmv = FALSE;//hmv_support
#endif
#endif

/*****************************************************************************/
//  Description : Combine the current play list name.
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CombinePlayListFileName(
                MMIFILE_DEVICE_E    dev_type,           //[IN]
                MMIKM_LIST_FILE_TYPE_E file_type,       //[IN]
                wchar           *full_path_name_ptr,    //[OUT]
                uint16          *full_path_len_ptr      //[OUT]
                );

/*****************************************************************************/
//  Description : create empty file 
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateEmptyFile (
                   wchar    *file_name,         //[IN]
                   uint16   file_name_len,      //[IN]
                   BOOLEAN  *is_space_enough    //[OUT]
                   );

/*****************************************************************************/
//  Description : Set play list device information
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlayListDevice(
                        MMIFILE_DEVICE_E    dev
                        );

/*****************************************************************************/
//  Description : 获取筛选视频文件的筛选参数
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void GetFilerInfo(MMIFMM_FILTER_T * filter);

/*****************************************************************************/
//  Description : 获取默认存放视频文件的文件夹路径
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDevFolderPath(
                                   MMIFILE_DEVICE_E dev,
                                   wchar          *full_path_name_ptr,
                                   uint16          *full_path_len_ptr
                                   );


/*****************************************************************************/
//  Description : 保存本地播放列表信息到默认文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlaylistToFile(KM_PLAYLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 保存播放列表详细信息到指定文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlaylistInforToFile(KM_PLAYLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 保存播放信息到指定文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlayInforToFile(KM_PLAYLIST_TYPE_E  list_type);

/*****************************************************************************/
//  Description : 获取存放播放列表文件的路径。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileNameForReadWrite(
                                   MMIFILE_DEVICE_E    dev_type, 
                                   MMIKM_LIST_FILE_TYPE_E file_type,
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr ,
                                   KM_PLAYLIST_TYPE_E  locallist_type
                                   );

/*****************************************************************************/
//  Description : 获取本地播放列表视频文件名信息。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistAllItem(KM_PLAYLIST_TYPE_E  list_type);




/*****************************************************************************/
//  Description : 获取本地播放列表中某一项的信息。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistItemInfo(
                                    uint32               list_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr,
                                    KM_PLAYLIST_TYPE_E  list_type
                                    );

/*****************************************************************************/
//  Description : 设置播放列表总的数目
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaylistCount(int32 list_num);

/*****************************************************************************/
//  Description : 根据指定文件名获取该文件的详细信息，比如:该影片类型、播放时间、
//                主演、上演时间、剧情简介、海报图
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistInforItemInfo(
                                    uint32          list_index,
                                    wchar          *full_path_name_ptr,
                                    uint16          full_path_len
                                    );

/*****************************************************************************/
//  Description : 根据指定文件名获取该文件的详细信息，信息从驱动接口处获得
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileItemInfor(MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr);

/*****************************************************************************/
//  Description : 统计影片文件类型计数
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaylistTypeNum(MMIKM_TYPE_E type_enum) ;

/*****************************************************************************/
//  Description : read infor from file
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E ReadFromFile(MMIKM_LIST_FILE_TYPE_E file_enum);

/*****************************************************************************/
//  Description : 拷贝MMIKM_FILE_ITEM_INFO_T信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyFileItmInfo(
                                  MMIKM_FILE_ITEM_INFO_T *dest_item_info_ptr, 
                                  MMIKM_FILE_ITEM_INFO_T *src_item_info_ptr
                                  );

/*****************************************************************************/
//  Description : OpenVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenVideoSecurty(void * pvoid);

/*****************************************************************************/
//  Description : CloseVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CloseVideoSecurty(void);

/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailInfo
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetVideoSecurtyThumbnailInfo(void);

/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailData
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoSecurtyThumbnailData(uint8 *data_buffer_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : 解析videosecurty数据内容
//  Global resource dependence : 
//  Author: haili
//  Note: 
/*****************************************************************************/
LOCAL BOOL ParseKmvFile(KMV_INFORMATION_T  *ret_info,  uint8 *bufptr, uint32 len);

/*****************************************************************************/
//  Description : Set play list device information
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlayListDevice(
                        MMIFILE_DEVICE_E    dev
                        )
{
    s_play_list_dev.dev_type = dev;
}

/*****************************************************************************/
//  Description : get play list device information
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIKM_GetPlayListDevice(void)                     
{
    return s_play_list_dev.dev_type;
}

/*****************************************************************************/
//  Description : create empty file 
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateEmptyFile (
                   wchar    *file_name,         //[IN]
                   uint16    file_name_len,      //[IN]
                   BOOLEAN  *is_space_enough    //[OUT]
                   )
{
    MMIFILE_HANDLE file_handle = 0;
    //if exist,delete it
    if(MMIAPIFMM_IsFileExist(file_name, file_name_len))
    {
        MMIAPIFMM_DeleteFile(file_name, PNULL);
    }
    //create file     
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : create play list file 
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_CreateInforFile(void)
{
    BOOLEAN result = FALSE;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    BOOLEAN is_space_enough = TRUE;

    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = FMM_SEARCH_FILENAME_MAXLEN+1;

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
     //check 顺序sdcard-->sdcard1-->udisk
     dev = MMIAPIFMM_GetFirstValidDevice();

#else
    //check sdcard or sdcard1 status
    dev = MMIAPIFMM_GetFirstValidSD();
#endif
     if(dev >= MMI_DEVICE_NUM)
     {
        return result;
     }     
    full_path_ptr = SCI_ALLOCA((FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    if(NULL == full_path_ptr)
    {
        return result;
    }

    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    
    //create playlist file
    CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST, full_path_ptr, &full_path_len);
    if(CreateEmptyFile(full_path_ptr, full_path_len, &is_space_enough))
    {
        SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
        //create playlist infor file
        CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, full_path_ptr, &full_path_len);
        if(CreateEmptyFile(full_path_ptr, full_path_len, &is_space_enough))
        {
            SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
            //create play infor file
            CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_INFOR, full_path_ptr, &full_path_len);
            if(CreateEmptyFile(full_path_ptr, full_path_len, &is_space_enough))
            {
                SetPlayListDevice(dev);
                result = TRUE;
            } 
        }   
    }

    SCI_FREE(full_path_ptr);
  
    return result;
}

/*****************************************************************************/
//  Description : Combine the current play list file name.
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CombinePlayListFileName(
                MMIFILE_DEVICE_E   dev_type,            //[IN]
                MMIKM_LIST_FILE_TYPE_E file_type,       //[IN]
                wchar           *full_path_name_ptr,    //[OUT]
                uint16          *full_path_len_ptr      //[OUT]
                )
{
    *full_path_len_ptr = FMM_SEARCH_FILENAME_MAXLEN+1;
    //play list file 
    if(MMIKM_FILE_TYPE_PLAY_LIST == file_type)
    {
        MMIAPIFMM_CombineSysPath(
        dev_type,
        MMIKM_RESERVED_DIR, 
        MMIKM_RESERVED_DIR_LEN,
        MMIKM_PLAY_LIST_NAME, 
        MMIKM_PLAY_LIST_NAME_LEN,
        full_path_name_ptr,
        full_path_len_ptr
        );
    }
    //play list infor file 
    else if(MMIKM_FILE_TYPE_PLAY_LIST_INFOR == file_type)
    {
        MMIAPIFMM_CombineSysPath(
        dev_type,
        MMIKM_RESERVED_DIR, 
        MMIKM_RESERVED_DIR_LEN,
        MMIKM_PLAY_LIST_INFOR_NAME, 
        MMIKM_PLAY_LIST_INFOR_NAME_LEN,
        full_path_name_ptr,
        full_path_len_ptr
        );
    }
    //play infor file
    else     
    {
        MMIAPIFMM_CombineSysPath(
        dev_type,
        MMIKM_RESERVED_DIR, 
        MMIKM_RESERVED_DIR_LEN,
        MMIKM_PLAY_INFOR_NAME, 
        MMIKM_PLAY_INFOR_NAME_LEN,
        full_path_name_ptr,
        full_path_len_ptr
        );
    }
    
}

/*****************************************************************************/
//  Description : delete km play infor files 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_DeleteFile(void)
{
    BOOLEAN recode = FALSE;
    MMIFILE_ERROR_E  read_result = SFS_NO_ERROR;
    MMIFILE_DEVICE_E    dev = s_play_list_dev.dev_type;
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = FMM_SEARCH_FILENAME_MAXLEN+1;
    uint16 indx = 0;
    uint16 delt_count = 0;
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
#endif

        wchar  thumbnail_file_full_name[FMM_SEARCH_FILENAME_MAXLEN+1]={0};  //cr141075
        uint16  thumbnail_file_full_name_len=0;



    full_path_ptr = SCI_ALLOCA((FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    if(NULL == full_path_ptr)
    {
        return recode;
    }  

    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST, full_path_ptr, &full_path_len);
    //check playlist file 
    if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
    { 
        MMIAPIFMM_DeleteFile(full_path_ptr, NULL);
    }

    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_INFOR, full_path_ptr, &full_path_len);
    //check play infor file
    if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
    {
         MMIAPIFMM_DeleteFile(full_path_ptr, NULL);         
    }
    
    if(MMIKM_AlloclMemoForPlaylistInfo())
    {
        read_result = ReadFromFile(MMIKM_FILE_TYPE_PLAY_LIST_INFOR);
        if(SFS_NO_ERROR == read_result)
        {
            //read playlist infor file success, then delete bmp files
            for(indx = 0; indx < KM_FILE_INFO_NUM_MAX; indx ++)
            {
                if(TRUE == s_km_play_list_info_ptr->km_file_item_info[indx].is_valid)
                {

                    GetThumbnailImageByFileName(s_km_play_list_info_ptr->km_file_item_info[indx].file_full_path_name,s_km_play_list_info_ptr->km_file_item_info[indx].file_info_name,thumbnail_file_full_name,&thumbnail_file_full_name_len);
                    if(MMIAPIFMM_IsFileExist(thumbnail_file_full_name, 
                                                            thumbnail_file_full_name_len))
                    {
#ifdef KM_DEBUG_SWITCH_ENABLE
                        MMIAPICOM_WstrToStr(thumbnail_file_full_name, char_filename);
#endif
                        //delete bmp file
                        MMIAPIFMM_DeleteFile(thumbnail_file_full_name, NULL); 
                        delt_count ++;
                    }
                }   

                if(delt_count >= s_km_play_info.total_num)
                {
                    break;
                }
            }        
        }
    }

    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, full_path_ptr, &full_path_len);
    //check play list infor file
    if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
    { 
         MMIAPIFMM_DeleteFile(full_path_ptr, NULL);       
    }

    SCI_FREE(full_path_ptr);

    s_play_list_dev.dev_type = MMI_DEVICE_SYSTEM;
    recode = TRUE;
    
    return recode;

}
//  Description : check km files 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_CheckFile(void)
{
    BOOLEAN recode = FALSE;
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_SDCARD;
    wchar   *full_path_ptr = PNULL;
   
    uint16  full_path_len = FMM_SEARCH_FILENAME_MAXLEN + 1;

    s_play_list_dev.dev_type = MMI_DEVICE_SYSTEM;
    
    full_path_ptr = SCI_ALLOCA((FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    if(NULL == full_path_ptr)
    {
        return FALSE;
    }  

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
    for(dev = MMI_DEVICE_NUM -1; dev >= MMI_DEVICE_UDISK; dev--)
#else
    for(dev = MMI_DEVICE_NUM -1; dev >= MMI_DEVICE_SDCARD; dev--)
#endif
    {
        SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
        CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST, full_path_ptr, &full_path_len);
        //check playlist file 
        if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
        { 
            SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
            CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, full_path_ptr, &full_path_len);
            //check playlist infor file
            if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
            {
                //check play infor file
                SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
                CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_INFOR, full_path_ptr, &full_path_len);
                if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
                {
                    s_play_list_dev.dev_type = dev;
                    recode = TRUE;
                    break;
                }    
            }
        }
    }

    SCI_FREE(full_path_ptr);
    return recode;

}

/*****************************************************************************/
//  Description : read infor from file
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E ReadFromFile(MMIKM_LIST_FILE_TYPE_E file_enum)
{
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    wchar               abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]  =   {0};
    uint16              abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    MMIFILE_HANDLE      file_handle =   0;    
    MMIFILE_DEVICE_E    dev_type = s_play_list_dev.dev_type;
    KM_PLAYLIST_TYPE_E  list_type = KM_PLAYLIST_DEFAULT;
    
    //SCI_TRACE_LOW:"[MMIKM] ReadFromFile entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_637_112_2_18_2_31_17_7,(uint8*)"" );
    //获取播放列表文件全路径
    if(GetFileNameForReadWrite(dev_type, file_enum, abs_default_path, &abs_default_path_length, list_type))
    {
        //打开文件写入
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {        
                file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {   
                    uint32 read_len = 0;
                    if(file_enum == MMIKM_FILE_TYPE_PLAY_INFOR)
                    {
                        SCI_MEMSET(&s_km_play_info, 0x00, sizeof(MMIKM_PLAY_INFO_T));
                    	save_result = MMIAPIFMM_ReadFile(file_handle, (void*)&s_km_play_info, sizeof(MMIKM_PLAY_INFO_T), &read_len, NULL);
                    }
                    else if(file_enum == MMIKM_FILE_TYPE_PLAY_LIST_INFOR)
                    {
                        SCI_MEMSET(s_km_play_list_info_ptr->km_file_item_info, 0x00, sizeof(MMIKM_PLAY_LIST_INFO_T));
                    save_result = MMIAPIFMM_ReadFile(file_handle, (void*)s_km_play_list_info_ptr->km_file_item_info, sizeof(MMIKM_PLAY_LIST_INFO_T), &read_len, NULL);
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
            }
            else
            {
            	save_result = SFS_ERROR_NOT_EXIST;
            }
        
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }

    if (SFS_NO_ERROR == save_result)
    {
        SCI_TRACE_LOW("[MMIKM]: ReadFromFile: read success, will check data is valid");

        if(file_enum == MMIKM_FILE_TYPE_PLAY_INFOR)
        {
            if (s_km_play_info.cur_file_full_path_len > ARR_SIZE(s_km_play_info.cur_file_full_path_name))
            {
                save_result = SFS_ERROR_ACCESS; //校验失败
            }
            else if (s_km_play_info.total_num > KM_FILE_INFO_NUM_MAX)
            {
                save_result = SFS_ERROR_ACCESS; //校验失败
            }

            SCI_TRACE_LOW("[MMIKM]: MMIKM_ReadFile: s_km_play_info check end, ret = %d", (BOOLEAN)(SFS_NO_ERROR == save_result));
        }
        else if(file_enum == MMIKM_FILE_TYPE_PLAY_LIST_INFOR)
        {
            if (PNULL != s_km_play_list_info_ptr)
            {
                MMIKM_FILE_ITEM_INFO_T tmp_info = {0};
                
                SCI_MEMCPY(&tmp_info, &s_km_play_list_info_ptr->km_file_item_info[0], sizeof(MMIKM_FILE_ITEM_INFO_T));
                
                if (tmp_info.cur_indx > KM_FILE_INFO_NUM_MAX)
                {
                    save_result = SFS_ERROR_ACCESS; //校验失败
                }
                else if (tmp_info.file_contx_type >= MMIKM_TYPE_MAX)
                {
                    save_result = SFS_ERROR_ACCESS; //校验失败
                }
                else if (tmp_info.file_full_path_len > ARR_SIZE(tmp_info.file_full_path_name))
                {
                    save_result = SFS_ERROR_ACCESS; //校验失败
                }
                
                SCI_TRACE_LOW("[MMIKM]: MMIKM_ReadFile: s_km_play_list_info_ptr->km_file_item_info[0] check end, ret = %d", (BOOLEAN)(SFS_NO_ERROR == save_result));
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIKM] ReadFromFile save_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_672_112_2_18_2_31_17_8,(uint8*)"d",save_result);
    return save_result;              
    
}

/*****************************************************************************/
//  Description : read km file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_ReadFile(void)
{
    MMIFILE_ERROR_E  read_result = SFS_NO_ERROR;
    BOOLEAN recode = FALSE;

    read_result = ReadFromFile(MMIKM_FILE_TYPE_PLAY_INFOR);
    if(SFS_NO_ERROR == read_result)
    {
        read_result = ReadFromFile(MMIKM_FILE_TYPE_PLAY_LIST_INFOR);
    }
    
    if(SFS_NO_ERROR == read_result)
    {
        recode = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIKM]: MMIKM_ReadFile: recode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_697_112_2_18_2_31_17_9,(uint8*)"d",recode);
    return recode;
}
#if 0
/*****************************************************************************/
//  Description : write km playlist file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_WritePlayListFile(MMIKM_PLAY_LIST_INFO_T *play_list_info_ptr)
{
    BOOLEAN recode = FALSE;
    MMIFILE_HANDLE file_handle = 0;
    wchar   *full_path_ptr = PNULL;
    uint16   full_path_len = FMM_SEARCH_FILENAME_MAXLEN+1;
     
    if(NULL == play_list_info_ptr)
    {
        return recode;
    }
    
    full_path_ptr = SCI_ALLOCA((FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    if(PNULL == full_path_ptr)
    {
        return recode;
    }
    
    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    CombinePlayListFileName(s_play_list_dev.dev_type, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, full_path_ptr, &full_path_len);

    //create file 
    file_handle = MMIAPIFMM_CreateFile(full_path_ptr, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {   
        //uint32 data_len = sizeof(play_list_info_ptr);//sizeof(play_list_info_ptr)有问题，应该是错误代码 converity
        MMIAPIFMM_WriteFile(file_handle, (const void*)play_list_info_ptr, data_len, &data_len, NULL);
        MMIAPIFMM_CloseFile(file_handle);
        recode = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIKM]: MMIKM_WritePlayListFile: recode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_736_112_2_18_2_31_17_10,(uint8*)"d",recode);

    SCI_FREE(full_path_ptr);
    return recode;
}
#endif
/*****************************************************************************/
//  Description : 获取筛选视频文件的筛选参数
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void GetFilerInfo(MMIFMM_FILTER_T * filter)
{
    int32           i           = 0;
    uint16          str_len     = 0; 
    
    if (PNULL == filter)
    {
        //SCI_TRACE_LOW:"[MMIKM] GetFilerInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_753_112_2_18_2_31_17_11,(uint8*)"");
        return;
    }
    //将所有支持的视频格式生成".kmv"串
    for (i = 0; i < MMIKM_VIDEO_TYPE_MAX; i++)
    {
        uint16 suffix_len = (uint16)strlen((char*)s_km_all_suffix[i].suffix);
        if (s_km_all_suffix[i].is_support)
        {
            str_len = (uint16)strlen((char*)filter->filter_str);
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                "*.", 2, 2);
            str_len += 2;
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                 s_km_all_suffix[i].suffix, 
                 suffix_len, 
                 suffix_len); 
            str_len = (uint16)strlen((char*)filter->filter_str);//smv_support
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                " ", 1, 1);
        }
    }
}

/*****************************************************************************/
//  Description : 获取默认存放视频文件的文件夹路径
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDevFolderPath(
                                   MMIFILE_DEVICE_E dev,
                                   wchar           *full_path_name_ptr,
                                   uint16          *full_path_len_ptr
                                   )
{
    BOOLEAN     result  =   FALSE;
    uint16 sd_device_len = 0;
    wchar   store_dir[MMIKM_STORE_FILE_DIR_LEN + 1] = {0};    
    
    MMIAPICOM_Wstrcpy(store_dir, MMIKM_STORE_FILE_DIR_STR);
    
    dev = MMIAPIFMM_GetValidateDevice(dev);
    
#ifndef MMI_KING_MOVIE_IN_UDISK_SUPPORT
     if(MMI_DEVICE_UDISK == dev)
     {
         //not support udisk
         dev = MMI_DEVICE_NUM;
     }
#endif
    sd_device_len = MMIAPIFMM_GetDevicePathLen(dev); 
    if(MMI_DEVICE_NUM > dev && MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), sd_device_len))
    {
        result = MMIAPIFMM_CombineFullPath(
            MMIAPIFMM_GetDevicePath(dev),
            sd_device_len,
            store_dir, 
            (uint16)MMIAPICOM_Wstrlen(store_dir),
            PNULL, 
            PNULL,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }  
    
    return result; 
}

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT   
/*****************************************************************************/
//  Description : 合并Udisk和T卡文件数组
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_UdiskTcardFileArrayCombine(void)
{
    static KM_PLAYLIST_TYPE_E   locallist_type = KM_PLAYLIST_DEFAULT;
    
    MMIFILEARRAY_Combine(s_km_play_list_array_ptr[locallist_type], s_km_play_list_array_sd1_ptr[locallist_type]);
   
    if(PNULL != s_km_play_list_array_sd1_ptr[locallist_type])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd1_ptr[locallist_type]);
        s_km_play_list_array_sd1_ptr[locallist_type] = PNULL;
    }    
}
#endif
//  Description : 合并双t卡文件数组
//  Global resource dependence :  none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKM_DoubleCardFileArrayCombine(void)
{
    static KM_PLAYLIST_TYPE_E   locallist_type = KM_PLAYLIST_DEFAULT;
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
    MMIAPIFILEARRAY_Combine(s_km_play_list_array_sd1_ptr[locallist_type], s_km_play_list_array_sd2_ptr[locallist_type]);
#else    
    MMIAPIFILEARRAY_Combine(s_km_play_list_array_ptr[locallist_type], s_km_play_list_array_sd2_ptr[locallist_type]);
#endif
    if(PNULL != s_km_play_list_array_sd2_ptr[locallist_type])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd2_ptr[locallist_type]);
        s_km_play_list_array_sd2_ptr[locallist_type] = PNULL;
    }    
}

/*****************************************************************************/
//  Description : 开始异步加载本地播放列表。出厂首次打开播放器以及本地列表为
//                空，自动搜索指定目录中的视频文件；
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayList_BeginLoad(
                                      MMI_WIN_ID_T        win_id,  
                                      MMI_MESSAGE_ID_E    msg_id,
                                      KM_PLAYLIST_TYPE_E  locallist_type,
                                      MMIFILE_DEVICE_E dev
                                      )
{                   
    wchar                   abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]={0};
    uint16                  abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    BOOLEAN                 load_result             = FALSE;
    MMIFMM_FILTER_T         filter                  = {0};
    MMIFILE_DEVICE_E file_dev = MMIKM_GetPlayListDevice();
    FILEARRAY               file_array_ptr           = PNULL;
    
    //为本地播放列表分配空间
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_902_112_2_18_2_31_18_12,(uint8*)"d",locallist_type);
    if (KM_PLAYLIST_MAX <= locallist_type)
    {
        return load_result;
    }

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT    
    if(MMI_DEVICE_UDISK == dev)
    {
        s_km_play_list_array_ptr[locallist_type] = MMIFILEARRAY_Create();
        file_array_ptr = s_km_play_list_array_ptr[locallist_type];
    }
    else if(MMI_DEVICE_SDCARD == dev)
    {
        s_km_play_list_array_sd1_ptr[locallist_type] = MMIFILEARRAY_Create();
        file_array_ptr = s_km_play_list_array_sd1_ptr[locallist_type];
    }
    else if(MMI_DEVICE_SDCARD_1 == dev)
    {
        s_km_play_list_array_sd2_ptr[locallist_type] = MMIFILEARRAY_Create();
        file_array_ptr = s_km_play_list_array_sd2_ptr[locallist_type];
    }
    else
    {
         return load_result;
    }
#else
    if(file_dev == dev)
    {
        s_km_play_list_array_ptr[locallist_type] = MMIAPIFILEARRAY_Create();
        file_array_ptr = s_km_play_list_array_ptr[locallist_type];
    }
    else
    {
        s_km_play_list_array_sd2_ptr[locallist_type] = MMIAPIFILEARRAY_Create();
        file_array_ptr = s_km_play_list_array_sd2_ptr[locallist_type];
    }
#endif    
    //file_array_ptr = MMIFILEARRAY_Create();      
    if(PNULL == file_array_ptr)
    {
        return load_result;
    }

    if (KM_PLAYLIST_DEFAULT == locallist_type)
    {
        //获取视频文件过滤信息
        GetFilerInfo(&filter);

        //获取默认文件夹
        GetDevFolderPath(dev,abs_default_path,&abs_default_path_length);

        //默认文件夹中搜索支持的视频文件
        MMIAPIFMM_SearchFileInPath(
            abs_default_path, 
            abs_default_path_length,
            &filter,
            TRUE,
            FUNC_FIND_FILE,
            file_array_ptr,
            win_id,
            msg_id
            ); 
            load_result = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad leave "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_968_112_2_18_2_31_18_13,(uint8*)"");
    
    return load_result;    
}

/*****************************************************************************/
//  Description : 开始同步加载本地播放列表。出厂首次打开播放器以及本地列表为
//                空，自动搜索指定目录中的视频文件；
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIKM_PlayList_BeginLoad(
                                      MMI_WIN_ID_T        win_id,  
                                      MMI_MESSAGE_ID_E    msg_id,
                                      KM_PLAYLIST_TYPE_E  locallist_type,
                                      MMIFILE_DEVICE_E dev
                                      )
{                   
    wchar                   abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]={0};
    uint16                  abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    BOOLEAN                 load_result             = FALSE;
    MMIFMM_FILTER_T         filter                  = {0};
    FILEARRAY               sd2_file_array           = PNULL;
    MMIFILE_DEVICE_E   dev2 = MMI_DEVICE_SDCARD_1;
    
    //为本地播放列表分配空间
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_993_112_2_18_2_31_18_14,(uint8*)"d",locallist_type);
    if (KM_PLAYLIST_MAX <= locallist_type)
    {
        return load_result;
    }
   
    s_km_play_list_array_ptr[locallist_type] = MMIAPIFILEARRAY_Create();      
    if(PNULL == s_km_play_list_array_ptr[locallist_type])
    {
        return load_result;
    }

    if (KM_PLAYLIST_DEFAULT == locallist_type)
    {
        //获取视频文件过滤信息
        GetFilerInfo(&filter);

        //获取默认文件夹
        GetDevFolderPath(dev,abs_default_path,&abs_default_path_length);

        //默认文件夹中搜索支持的视频文件
        MMIAPIFMM_SearchFileInPath(
            abs_default_path, 
            abs_default_path_length,
            &filter,
            TRUE,
            FUNC_FIND_FILE,
            s_km_play_list_array_ptr[locallist_type],
            0,
            0
            ); 

        //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 1 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1025_112_2_18_2_31_18_15,(uint8*)"d",MMIAPIFILEARRAY_GetArraySize(s_km_play_list_array_ptr[locallist_type]));
        
        if(dev == MMI_DEVICE_SDCARD)
        {
            dev2 = MMI_DEVICE_SDCARD_1;
        }
        else if(dev == MMI_DEVICE_SDCARD_1)
        {
            dev2 = MMI_DEVICE_SDCARD;
        }
        else
        {
            return load_result;
        }

        if(MMIAPIFMM_GetDeviceTypeStatus(dev2))
        {
            sd2_file_array = MMIAPIFILEARRAY_Create();      
            if(PNULL == sd2_file_array)
            {
                return load_result;
            }

            //获取默认文件夹
            SCI_MEMSET(abs_default_path, 0X00, sizeof(abs_default_path));
            GetDevFolderPath(dev2,abs_default_path,&abs_default_path_length);

            //默认文件夹中搜索支持的视频文件
            MMIAPIFMM_SearchFileInPath(
                abs_default_path, 
                abs_default_path_length,
                &filter,
                TRUE,
                FUNC_FIND_FILE,
                sd2_file_array,
                0,
                0
                ); 
            //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad sd2_file_array = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1063_112_2_18_2_31_18_16,(uint8*)"d",MMIAPIFILEARRAY_GetArraySize(sd2_file_array));
            MMIAPIFILEARRAY_Combine(s_km_play_list_array_ptr[locallist_type], sd2_file_array);
            //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 2 = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1065_112_2_18_2_31_18_17,(uint8*)"d",MMIAPIFILEARRAY_GetArraySize(s_km_play_list_array_ptr[locallist_type]));
            MMIAPIFILEARRAY_Destroy(&sd2_file_array);
            sd2_file_array= PNULL;
        }            
        load_result = TRUE;  
    }
    
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_BeginLoad leave "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1072_112_2_18_2_31_18_18,(uint8*)"");
    MMK_PostMsg(win_id,msg_id,PNULL,PNULL);  
    return load_result;    
}*/

/*****************************************************************************/
//  Description : 异步加载本地播放列表完成，保存本地播放列表。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIKM_PlayList_EndLoad(KM_PLAYLIST_TYPE_E  locallist_type)
{
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayList_EndLoad locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1083_112_2_18_2_31_18_19,(uint8*)"d",locallist_type);
    return SavePlaylistToFile(locallist_type);
}

/*****************************************************************************/
//  Description : 保存本地播放列表信息到默认文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlaylistToFile(KM_PLAYLIST_TYPE_E  locallist_type)
{
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    wchar               abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]  =   {0};
    uint16              abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    MMIFILE_HANDLE      file_handle =   0;    
    MMIFILE_DEVICE_E  dev_type = s_play_list_dev.dev_type;
    
    //SCI_TRACE_LOW:"[MMIKM] SavePlaylistToFile locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1099_112_2_18_2_31_18_20,(uint8*)"d",locallist_type);
    //获取默认本地播放列表文件全路径
    if(GetFileNameForReadWrite(dev_type, MMIKM_FILE_TYPE_PLAY_LIST, abs_default_path, &abs_default_path_length, locallist_type))
    {
        //删除默认文件
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {        
            MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        }
        if (KM_PLAYLIST_MAX > locallist_type && PNULL != s_km_play_list_array_ptr[locallist_type])
        {        
            file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
            MMIAPIFMM_CloseFile(file_handle);
            //保存本地播放列表到默认文件中
            save_result = MMIAPIFILEARRAY_SaveArrayToFileEx( 
                s_km_play_list_array_ptr[locallist_type],
                abs_default_path,
                abs_default_path_length );
        }
        else
        {
            save_result = SFS_ERROR_NO_ENOUGH_RESOURCE;
        }
        
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }
    
    //SCI_TRACE_LOW:"[MMIKM] SavePlaylistToFilesave_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1129_112_2_18_2_31_18_21,(uint8*)"d",save_result);
    if (KM_PLAYLIST_MAX > locallist_type && SFS_ERROR_NONE != save_result)
    {
        MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_ptr[locallist_type]);
        s_km_play_list_array_ptr[locallist_type] = PNULL;
    }
    
    return save_result;              
}

/*****************************************************************************/
//  Description : 获取每个playlist item文件名信息
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayList_AllItem_Load(void)
{  
     KM_PLAYLIST_TYPE_E  list_type = KM_PLAYLIST_DEFAULT;
     return GetPlaylistAllItem(list_type);
}

/*****************************************************************************/
//  Description : 获取本地播放列表的列表项的数目。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKM_GetPlaylistNum( KM_PLAYLIST_TYPE_E  list_type )
{
    uint32  playlist_num = 0;
    
    if(KM_PLAYLIST_MAX > list_type &&  PNULL != s_km_play_list_array_ptr[list_type])
    {
        playlist_num = MMIAPIFILEARRAY_GetArraySize(s_km_play_list_array_ptr[list_type]);
    }
    //SCI_TRACE_LOW:"[MMIKM] GetPlaylistNum list_type = %d, locallist_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1162_112_2_18_2_31_18_22,(uint8*)"dd",list_type,playlist_num);
    
    return playlist_num;
}

/*****************************************************************************/
//  Description : 获取本地播放列表中某一项的信息。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistItemInfo(
                                    uint32               list_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr,
                                    KM_PLAYLIST_TYPE_E  list_type
                                    )
{
    BOOLEAN    result = FALSE;

    //SCI_TRACE_LOW:"[MMIKM] GetPlaylistItemInfo list_type = %d, list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1179_112_2_18_2_31_18_23,(uint8*)"dd",list_type, list_index);
    if (PNULL != f_array_data_ptr && PNULL != s_km_play_list_array_ptr[list_type] && KM_PLAYLIST_MAX > list_type)   
    {
        result = MMIAPIFILEARRAY_Read(s_km_play_list_array_ptr[list_type], list_index, f_array_data_ptr);
    }
    //SCI_TRACE_LOW:"[[MMIKM] GetPlaylistItemInfo list_type = %d, result = %d, list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1184_112_2_18_2_31_18_24,(uint8*)"ddd",list_type, result,list_index);
    return result;
}


/*****************************************************************************/
//  Description : 删除本地播放列表中某一项的信息。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeletePlaylistItemInfo(
                                    uint32               list_index,
                                    KM_PLAYLIST_TYPE_E  list_type
                                    )//andy.he_cr122288
{
    BOOLEAN    result = FALSE;

    //SCI_TRACE_LOW:"[MMIKM] DeletePlaylistItemInfo list_type = %d, list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1200_112_2_18_2_31_18_25,(uint8*)"dd",list_type, list_index);
    if ( PNULL != s_km_play_list_array_ptr[list_type] && KM_PLAYLIST_MAX > list_type)   
    {
        result = MMIAPIFILEARRAY_Delete(s_km_play_list_array_ptr[list_type], list_index);
    }
    //SCI_TRACE_LOW:"[[MMIKM] DeletePlaylistItemInfo list_type = %d, result = %d, list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1205_112_2_18_2_31_18_26,(uint8*)"ddd",list_type, result,list_index);
    return result;
}



/*****************************************************************************/
//  Description : 设置播放列表总的数目
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaylistCount(int32 list_num)
{
    if(list_num >= KM_FILE_INFO_NUM_MAX)
    {
        s_km_play_info.total_num = KM_FILE_INFO_NUM_MAX; 
    }
    else
    {
        s_km_play_info.total_num = list_num; 
    }
    
    //SCI_TRACE_LOW:"SetPlaylistCount : total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1226_112_2_18_2_31_18_27,(uint8*)"d",  s_km_play_info.total_num);
}

/*****************************************************************************/
//  Description : 异步获取本地播放列表视频文件名信息。
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayListGetItem(
                                    KM_PLAYLIST_TYPE_E  list_type, 
                                    MMIKM_LOAD_SAVE_INFO_T* save_info_ptr,
                                    uint32* file_size_ptr)
{
    uint32               cur_list_index = 0;
    FILEARRAY_DATA_T    array_data      = {0};
    BOOLEAN             result          = FALSE;
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    uint16  full_path_len = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    wchar   full_path_name[KM_FILE_INFO_FULL_NAME_LEN_MAX +1] = {0};
    uint16  PlaylistNum=0;
    
    if(PNULL == save_info_ptr)
    {
        return result;
    }
    cur_list_index = save_info_ptr->cur_list_index;
    if(GetPlaylistItemInfo(cur_list_index,&array_data,list_type))
    {    
#ifdef KM_DEBUG_SWITCH_ENABLE
        uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
        MMIAPICOM_WstrToStr(array_data.filename, char_filename);
#endif
        full_path_len = KM_FILE_INFO_FULL_NAME_LEN_MAX;
        SCI_MEMSET(full_path_name, 0X00, KM_FILE_INFO_FULL_NAME_LEN_MAX +1);
        MMI_WSTRNCPY(
            full_path_name,
            full_path_len,
            array_data.filename,
            array_data.name_len,
            array_data.name_len);
        
        full_path_len = array_data.name_len;
#ifdef KM_DEBUG_SWITCH_ENABLE
        memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1); 
        MMIAPICOM_WstrToStr(full_path_name, char_filename);
#endif
        MMIAPIFMM_GetFileInfo(full_path_name,full_path_len,file_size_ptr, PNULL, PNULL);//andy.he_cr122288
        if(0==*file_size_ptr)
        {
            DeletePlaylistItemInfo(cur_list_index,list_type);
        }
        else
        {
            if(GetPlaylistInforItemInfo(cur_list_index, full_path_name, full_path_len))
            {
                  cur_list_index++;
            }
            else
            {
                 DeletePlaylistItemInfo(cur_list_index,list_type);
            }
          
        }

    }

    if(save_info_ptr->cur_list_index == (save_info_ptr->list_num - 1)
             ||cur_list_index==KM_FILE_INFO_NUM_MAX
		|| (save_info_ptr->cur_list_index==0)&&(save_info_ptr->list_num==0))
    {
        PlaylistNum=(MMIKM_GetPlaylistNum(KM_PLAYLIST_DEFAULT)>=KM_FILE_INFO_NUM_MAX)?KM_FILE_INFO_NUM_MAX:MMIKM_GetPlaylistNum(KM_PLAYLIST_DEFAULT);
        //设置播放列表总的数目
        SetPlaylistCount(PlaylistNum);
        //save it to playlist infor file
        save_result = SavePlaylistInforToFile(list_type);
        if(SFS_NO_ERROR == save_result)
        {
            //save it to play infor file
            save_result = SavePlayInforToFile(list_type);
            if(SFS_NO_ERROR == save_result)
            {
                result = TRUE;
            }
        }

        //SCI_TRACE_LOW:"[MMIKM] MMIKM_PlayListGetItem result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1303_112_2_18_2_31_18_28,(uint8*)"d", result);
    }
    return result;
}

/*****************************************************************************/
//  Description : 获取本地播放列表视频文件名信息。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistAllItem(KM_PLAYLIST_TYPE_E  list_type)
{    
    uint32                     list_num = 0;
    BOOLEAN             result          = TRUE;
    MMIKM_LOAD_SAVE_INFO_T save_info = {0};
    
    //SCI_TRACE_LOW:"[MMIKM] GetPlaylistAllItem entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1318_112_2_18_2_31_18_29,(uint8*)"");
    
    //获取总的文件数目
    list_num = MMIKM_GetPlaylistNum(list_type);
    //SCI_TRACE_LOW:"[MMIKM] GetPlaylistAllItem list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1322_112_2_18_2_31_19_30,(uint8*)"d", list_num);

    //最多保存列表信息为KM_FILE_INFO_NUM_MAX个
    //if(list_num > KM_FILE_INFO_NUM_MAX)  //andy.he_cr122288  过滤0 bit文件
    //{
  //      list_num = KM_FILE_INFO_NUM_MAX;
 //   }
    save_info.cur_list_index = 0;
    save_info.list_num = list_num;
    save_info.list_type = list_type;
    MMIAPICOM_OtherTaskToMMI(MMIKM_SEACH_PLAYLIST_WAIT_WIN_ID,MSG_KM_LOADING_SAVE, &save_info, sizeof(MMIKM_LOAD_SAVE_INFO_T));
    
    return result;
}

/*****************************************************************************/
//  Description : 根据指定文件名获取该文件的详细信息，比如:该影片类型、播放时间、
//                主演、上演时间、剧情简介、海报图
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetPlaylistInforItemInfo(
                                    uint32          list_index,
                                    wchar          *full_path_name_ptr,
                                    uint16          full_path_len
                                    )
{
    BOOLEAN    result = FALSE;
   
   
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
    MMIAPICOM_WstrToStr(full_path_name_ptr, char_filename);
#endif            
    SCI_MEMSET(&s_km_play_list_info_ptr->km_file_item_info[list_index], 0x00, sizeof(MMIKM_FILE_ITEM_INFO_T));
    s_km_play_list_info_ptr->km_file_item_info[list_index].file_full_path_len = full_path_len;

    s_km_play_list_info_ptr->km_file_item_info[list_index].cur_indx = list_index;

    MMIAPICOM_Wstrncpy( s_km_play_list_info_ptr->km_file_item_info[list_index].file_full_path_name, 
                        full_path_name_ptr, full_path_len); 
   
#ifdef KM_DEBUG_SWITCH_ENABLE
    memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1);
    MMIAPICOM_WstrToStr(s_km_play_list_info_ptr->km_file_item_info[list_index].file_full_path_name, char_filename);
#endif            

    result = GetFileItemInfor(&s_km_play_list_info_ptr->km_file_item_info[list_index]);
    if(result)
    {
        s_km_play_list_info_ptr->km_file_item_info[list_index].is_valid = TRUE;  
    }

    //SCI_TRACE_LOW:"[MMIKM] GetPlaylistInforItemInfo result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1374_112_2_18_2_31_19_31,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 保存播放列表详细信息到指定文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlaylistInforToFile(KM_PLAYLIST_TYPE_E  locallist_type)
{
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    wchar               abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]  =   {0};
    uint16              abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    MMIFILE_HANDLE      file_handle =   0;    
    MMIFILE_DEVICE_E  dev_type = s_play_list_dev.dev_type;
    
    //SCI_TRACE_LOW:"[MMIKM] SavePlaylistInforToFile locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1390_112_2_18_2_31_19_32,(uint8*)"d",locallist_type);
    //获取播放列表文件全路径
    if(GetFileNameForReadWrite(dev_type, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, abs_default_path, &abs_default_path_length, locallist_type))
    {
        //打开文件写入
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {        
            if (KM_PLAYLIST_MAX > locallist_type)
            {        
                file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {   
                    uint32 data_len = sizeof(MMIKM_PLAY_LIST_INFO_T);
                    save_result = MMIAPIFMM_WriteFile(file_handle, (const void*)s_km_play_list_info_ptr, data_len, &data_len, NULL);

                    MMIAPIFMM_CloseFile(file_handle);
                }
            }
            else
            {
                save_result = SFS_ERROR_NO_ENOUGH_RESOURCE;
            }
        }
        
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }

    //SCI_TRACE_LOW:"[MMIKM] SavePlaylistInforToFile save_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1420_112_2_18_2_31_19_33,(uint8*)"d",save_result);
    return save_result;              
}

/*****************************************************************************/
//  Description : 保存播放信息到指定文件中
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SavePlayInforToFile(KM_PLAYLIST_TYPE_E  list_type)
{
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    wchar               abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]  =   {0};
    uint16              abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    MMIFILE_HANDLE      file_handle =   0;    
    MMIFILE_DEVICE_E  dev_type = s_play_list_dev.dev_type;
    
    //SCI_TRACE_LOW:"[MMIKM] SavePlayInforToFile list_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1436_112_2_18_2_31_19_34,(uint8*)"d", list_type);
    //获取播放列表文件全路径
    if(GetFileNameForReadWrite(dev_type, MMIKM_FILE_TYPE_PLAY_INFOR, abs_default_path, &abs_default_path_length, list_type))
    {
        //打开文件写入
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {        
            if (KM_PLAYLIST_MAX > list_type )
            {        
                file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {   
                    uint32 data_len = sizeof(MMIKM_PLAY_INFO_T);
                    save_result = MMIAPIFMM_WriteFile(file_handle, (const void*)&s_km_play_info, data_len, &data_len, NULL);

                    MMIAPIFMM_CloseFile(file_handle);
                }
            }
            else
            {
                save_result = SFS_ERROR_NO_ENOUGH_RESOURCE;
            }
        }
        
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }

    //SCI_TRACE_LOW:"[MMIKM] SavePlayInforToFile save_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1466_112_2_18_2_31_19_35,(uint8*)"d",save_result);
    return save_result;              
    
}


/*****************************************************************************/
//  Description : 统计影片文件类型计数
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaylistTypeNum(MMIKM_TYPE_E type_enum)                                 
{
   if(MMIKM_TYPE_MAX <= type_enum)
   {
        return;
   }
   
   s_km_play_info.type_num[type_enum] ++;
   //SCI_TRACE_LOW:"[MMIKM] SetPlaylistTypeNum type_num[%d] = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1485_112_2_18_2_31_19_36,(uint8*)"dd",type_enum, s_km_play_info.type_num[type_enum]);
}

/*****************************************************************************/
//  Description : 从全路径中获取不带后缀的文件名
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: file_name_ptr[d:\com\test\test.exe],输出参数file_path_ptr[test]
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetFileNameNoSuffix(      
                        const uint16 *src_name_ptr, // [IN]
                        uint16 src_name_len,  // [IN]
                        uint16 *des_path_ptr, // [OUT]
                        uint16 *des_path_len  // [OUT]
                         )
{
    BOOLEAN result = FALSE;
    int16 i = 0;
    int16 file_name_len = 0;
    wchar file_name[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
    
    if (des_path_ptr == PNULL || des_path_len == PNULL || src_name_ptr == PNULL || src_name_len == 0)
    {
        return result;
    }
    
    for (i = (uint16)(src_name_len - 1); i >= 0; i--)
    {
        if ( src_name_ptr[i] == KM_POS_BIAS )    
        {
            MMIAPICOM_Wstrncpy( file_name, &src_name_ptr[i+1], (src_name_len - i - 1) );
            break;
        }
    }

    file_name_len = MMIAPICOM_Wstrlen((const wchar*) file_name);
    for(i = (uint16)(file_name_len -1); i >=0; i-- )
    {
        if ( file_name[i] == KM_POS_DIOT)    
        {
            MMIAPICOM_Wstrncpy( des_path_ptr,(const wchar*)&file_name, i);
            *des_path_len = (uint16)i;
            break;
        }      
    }
    
    result =  TRUE;
    return result;
}

/*****************************************************************************/
//  Description : 从全路径中获取除后缀名外的字符
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: //  Note: file_name_ptr[d:\com\test\test.exe],
//            输出参数file_path_ptr[d:\com\test\test]
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetFileFullPathNoSuffix(      
                        const uint16 *src_name_ptr, // [IN]
                        uint16 src_name_len,  // [IN]
                        uint16 *des_path_ptr, // [OUT]
                        uint16 *des_path_len  // [OUT]
                         )
{
    BOOLEAN result = FALSE;
    int16 i = 0;
    
    if (des_path_ptr == PNULL || des_path_len == PNULL || src_name_ptr == PNULL || src_name_len == 0)
    {
        return result;
    }
    
    for(i = 0; i < src_name_len; i++)
    {
        if ( src_name_ptr[i] == KM_POS_DIOT )    
        {
            MMIAPICOM_Wstrncpy(des_path_ptr, src_name_ptr, (i - 1) );
            *des_path_len = (uint16)i;
            break;
        }
    }
    result =  TRUE;
    
    return result;
}

/*****************************************************************************/
//  Description : 获取文件名信息
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileItemNameInfor(MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr)                                 
{
    BOOLEAN    result = FALSE;
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
#endif 

    if(NULL == file_item_info_ptr)
    {
        return result;
    } 
#ifdef KM_DEBUG_SWITCH_ENABLE
    MMIAPICOM_WstrToStr(file_item_info_ptr->file_full_path_name, char_filename);
#endif 

    result = MMIKM_GetFileNameNoSuffix(file_item_info_ptr->file_full_path_name, file_item_info_ptr->file_full_path_len, 
                file_item_info_ptr->file_info_name, &file_item_info_ptr->file_info_name_len);

#ifdef KM_DEBUG_SWITCH_ENABLE   
    memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1);
    MMIAPICOM_WstrToStr(file_item_info_ptr->file_info_name, char_filename);
#endif 

    return result;
}

/*****************************************************************************/
//  Description : 获取影片类型信息
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIKM_TYPE_E GetFileItemTypeInfor(uint8* item_buffer_ptr, uint16	item_size)                                 
{
    MMIKM_TYPE_E file_type_e = MMIKM_TYPE_MAX;
    if(PNULL == item_buffer_ptr || 0 == item_size)
    {
        return file_type_e;
    }
    //类型为'0','1','2'等
    file_type_e = (MMIKM_TYPE_E)atoi((char*)item_buffer_ptr);
    if(file_type_e >= MMIKM_TYPE_MAX)
    {
        file_type_e = MMIKM_TYPE_OTHER;
    }
    //SCI_TRACE_LOW:"[KM] GetFileItemTypeInfor:file_type_e = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1618_112_2_18_2_31_19_37,(uint8*)"d", file_type_e);
    return file_type_e;
}

/*****************************************************************************/ 
//  Description : 根据指定文件名获取该文件的详细信息，信息从驱动接口处获得
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileItemInfor(MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr)                                  
{
    //uint8 *data_buf_ptr = NULL;
    BOOLEAN    result = FALSE;
    KMV_INFORMATION_T kmv_infor_t = {0};
    char temp_char[KM_FILE_INFO_SYNOPSIS_LEN_MAX + 1] = {0};
    MMIFILE_HANDLE file_handle = 0;
    uint32 bytes_write = 0;
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
#endif
    BOOLEAN   IslongestFilename=FALSE;//cr141075
    MMIFILE_DEVICE_E dev = s_play_list_dev.dev_type;
        
    if(NULL == file_item_info_ptr)
    {
        return result;
    } 
    SCI_TRACE_LOW("[KM] GetFileItemInfor ENTYR");
    MMIAPIKM_CkeckSupportfile((wchar*)file_item_info_ptr->file_full_path_name);

    
    
    //get kmv file content    //smv_support
    if(MMIAPISM_IsSupportfile())
    {
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor THIS IS SMV FILE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1648_112_2_18_2_31_19_38,(uint8*)"");
        if(FALSE == MMIKM_GetSmvFileContent(file_item_info_ptr->file_full_path_name, &kmv_infor_t))                                 
        {
            if(PNULL != kmv_infor_t.data_buf_ptr)
            {
                SCI_FREE(kmv_infor_t.data_buf_ptr);
                kmv_infor_t.data_buf_ptr = PNULL;
            }
            return result;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor THIS IS KingMV FILE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1656_112_2_18_2_31_19_39,(uint8*)"");
        if(FALSE == MMIKM_GetKmvFileContent(file_item_info_ptr->file_full_path_name, &kmv_infor_t))                                 
        {
            if(PNULL != kmv_infor_t.data_buf_ptr)
            {
                SCI_FREE(kmv_infor_t.data_buf_ptr);
                kmv_infor_t.data_buf_ptr = PNULL;
            }
            return result;
        }
    }
    
    //get file name
    result = GetFileItemNameInfor(file_item_info_ptr);
    //SCI_TRACE_LOW:"[KM] GetFileItemInfor:result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1677_112_2_18_2_31_19_40,(uint8*)"d", result);

#ifdef KM_DEBUG_SWITCH_ENABLE
    MMIAPICOM_WstrToStr(file_item_info_ptr->file_info_name, char_filename); 
    //SCI_TRACE_LOW:"[KM] GetFileItemInfor:file name = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1681_112_2_18_2_31_19_41,(uint8*)"s", char_filename);
#endif

    //get thumbnail
    if(kmv_infor_t.item_thumbnail.item_is_true)
    {
        
        wchar  thumbnail_file_full_name[FMM_SEARCH_FILENAME_MAXLEN+1]={0};   //cr141075
        uint16  thumbnail_file_full_name_len=0;

        IslongestFilename = GetThumbnailImageByFileName(file_item_info_ptr->file_full_path_name,
			file_item_info_ptr->file_info_name,
			thumbnail_file_full_name,
			&thumbnail_file_full_name_len);
        
        if(!MMIAPIFMM_IsFileExist(thumbnail_file_full_name, thumbnail_file_full_name_len))
        {
            //create bmp file     
            file_handle = MMIAPIFMM_CreateFile(thumbnail_file_full_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);

            if(SFS_INVALID_HANDLE != file_handle)
            {
                MMIAPIFMM_WriteFile(file_handle, kmv_infor_t.item_thumbnail.item_buffer_ptr,
                                  kmv_infor_t.item_thumbnail.item_size, &bytes_write, NULL);
                MMIAPIFMM_CloseFile(file_handle);
            }
            
            if(IslongestFilename)
            {
                 MMIAPIFMM_SetAttr(thumbnail_file_full_name, MMIAPICOM_Wstrlen(thumbnail_file_full_name), TRUE, FALSE, FALSE, FALSE);
			}
            
		}
    }

    //get file type
    if(kmv_infor_t.item_type.item_is_true)
    {
         file_item_info_ptr->file_contx_type = GetFileItemTypeInfor(kmv_infor_t.item_type.item_buffer_ptr,
                                                        kmv_infor_t.item_type.item_size);
    }
    //SCI_TRACE_LOW:"[KM] GetFileItemInfor: type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1748_112_2_18_2_31_20_42,(uint8*)"d",file_item_info_ptr->file_contx_type);
    //set file type number
    SetPlaylistTypeNum(file_item_info_ptr->file_contx_type);

    //get file time
    if(kmv_infor_t.item_time.item_is_true)
    {
        memset(temp_char, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + 1);
        if(kmv_infor_t.item_time.item_size > KM_FILE_INFO_TIME_LEN_MAX)
        {
            kmv_infor_t.item_time.item_size = KM_FILE_INFO_TIME_LEN_MAX;
        }
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor:time len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1760_112_2_18_2_31_20_43,(uint8*)"d", kmv_infor_t.item_time.item_size);
        strncpy(temp_char, (char *)kmv_infor_t.item_time.item_buffer_ptr, kmv_infor_t.item_time.item_size);
        MMIAPICOM_StrToWstr((const uint8*)temp_char, file_item_info_ptr->file_info_time);
    }
    //get file dispaly year
    if(kmv_infor_t.item_year.item_is_true)
    {
        memset(temp_char, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + 1);
        if(kmv_infor_t.item_year.item_size > KM_FILE_INFO_YEAR_LEN_MAX)
        {
            kmv_infor_t.item_year.item_size = KM_FILE_INFO_YEAR_LEN_MAX;
        }
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor:year len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1772_112_2_18_2_31_20_44,(uint8*)"d", kmv_infor_t.item_year.item_size);
        strncpy(temp_char, (char *)kmv_infor_t.item_year.item_buffer_ptr, kmv_infor_t.item_year.item_size);
        MMIAPICOM_StrToWstr((const uint8*)temp_char, file_item_info_ptr->file_info_year);
    }

    if(PNULL != kmv_infor_t.data_buf_ptr)
    {
        SCI_FREE(kmv_infor_t.data_buf_ptr);
        kmv_infor_t.data_buf_ptr = PNULL;
    }

    result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description : 获取存放本地播放列表文件的路径。
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileNameForReadWrite(
                                   MMIFILE_DEVICE_E    dev_type, 
                                   MMIKM_LIST_FILE_TYPE_E file_type,
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr ,
                                   KM_PLAYLIST_TYPE_E  locallist_type
                                   )
{
    BOOLEAN         result      =   FALSE;
    
    //SCI_TRACE_LOW:"[MMIKM] GetFileNameForReadWrite locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1795_112_2_18_2_31_20_45,(uint8*)"d",locallist_type);

    if (PNULL != full_path_name_ptr && PNULL != full_path_len_ptr)
    { 
    
        if (KM_PLAYLIST_DEFAULT == locallist_type)
        {      
            if(file_type == MMIKM_FILE_TYPE_PLAY_LIST)
            {
                CombinePlayListFileName(dev_type, MMIKM_FILE_TYPE_PLAY_LIST, full_path_name_ptr, full_path_len_ptr);
            }
            else if(file_type == MMIKM_FILE_TYPE_PLAY_LIST_INFOR)
            {
                CombinePlayListFileName(dev_type, MMIKM_FILE_TYPE_PLAY_LIST_INFOR, full_path_name_ptr, full_path_len_ptr);
            }  
            else
            {
                CombinePlayListFileName(dev_type, MMIKM_FILE_TYPE_PLAY_INFOR, full_path_name_ptr, full_path_len_ptr);
            }
        }
        result = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIKM] GetFileNameForReadWrite result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1818_112_2_18_2_31_20_46,(uint8*)"d",result);
    return result; 
    
}

/*****************************************************************************/
//  Description :   通过文件fullpath获取 图片 fullpath 信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN GetThumbnailImageByFileName(wchar * file_info_fullpath,wchar *file_info_name,wchar *thumbnail_file_full_name,uint16 *thumbnail_file_full_name_len)
{
        uint16 thumbnail_file_name_len = 0;
        wchar thumbnail_file_name[FILEARRAY_FILENAME_MAX_LEN+1] = {0};    
        MMIFILE_DEVICE_E dev = s_play_list_dev.dev_type;
		MMICOM_VIDEO_TYPE_E file_type = MMICOM_VIDEO_TYPE_KMV;
		BOOLEAN IslongestFilename = FALSE;

        if(PNULL==file_info_name || PNULL==thumbnail_file_full_name || PNULL==thumbnail_file_full_name_len ||PNULL== file_info_fullpath)
        {
            return IslongestFilename;
        }
		
		file_type = MMIAPICOM_GetMovieType(file_info_fullpath, MMIAPICOM_Wstrlen(file_info_fullpath));
    
        MMIAPICOM_Wstrcpy(thumbnail_file_name, file_info_name);

		if(MMICOM_VIDEO_TYPE_SMV == file_type)
		{
			MMIAPICOM_Wstrncat(thumbnail_file_name, KM_FILE_SUFFIX_STR, MMIAPICOM_Wstrlen(KM_FILE_SUFFIX_STR));
		}
		else if(MMICOM_VIDEO_TYPE_KMV == file_type)
		{
			MMIAPICOM_Wstrncat(thumbnail_file_name, SMV_FILE_SUFFIX_STR, MMIAPICOM_Wstrlen(SMV_FILE_SUFFIX_STR));
		}
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
		else
		{
			MMIAPICOM_Wstrncat(thumbnail_file_name, HMV_FILE_SUFFIX_STR, MMIAPICOM_Wstrlen(HMV_FILE_SUFFIX_STR));
		}
#endif
		
        MMIAPICOM_Wstrncat(thumbnail_file_name, KM_THUMBNAIL_SUFFIX_STR, MMIAPICOM_Wstrlen(KM_THUMBNAIL_SUFFIX_STR));

        thumbnail_file_name_len = MMIAPICOM_Wstrlen(thumbnail_file_name);
        
        *thumbnail_file_full_name_len = FILEARRAY_FILENAME_MAX_LEN;

    if(MMIAPICOM_Wstrlen(file_info_fullpath) + MMIAPICOM_Wstrlen(KM_FILE_SUFFIX_STR) <= FMM_SEARCH_FILENAME_MAXLEN-4)//E:\System\KM_res\  -  E:\kingmovie\  =4
    {
        MMIAPIFMM_CombineSysPath( dev,
                                MMIKM_RESERVED_DIR, 
                                MMIKM_RESERVED_DIR_LEN,
                                thumbnail_file_name, 
                                thumbnail_file_name_len,
                                thumbnail_file_full_name,
                                thumbnail_file_full_name_len
                                );
    }
     else
    {
		IslongestFilename = TRUE;
        MMIAPICOM_Wstrcpy(thumbnail_file_full_name,file_info_fullpath);
        MMIAPICOM_Wstrncpy(thumbnail_file_full_name+MMIAPICOM_Wstrlen(file_info_fullpath)-MMIAPICOM_Wstrlen(KM_THUMBNAIL_SUFFIX_STR), KM_THUMBNAIL_SUFFIX_STR,MMIAPICOM_Wstrlen(KM_THUMBNAIL_SUFFIX_STR));
    }
  
        *thumbnail_file_full_name_len=MMIAPICOM_Wstrlen(thumbnail_file_full_name);
        return IslongestFilename;
}



/*****************************************************************************/
//  Description : 拷贝MMIKM_FILE_ITEM_INFO_T信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyFileItmInfo(
                                  MMIKM_FILE_ITEM_INFO_T *dest_item_info_ptr, 
                                  MMIKM_FILE_ITEM_INFO_T *src_item_info_ptr
                                  )
{
    if(PNULL == dest_item_info_ptr || PNULL == src_item_info_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(dest_item_info_ptr, 0x00, sizeof(MMIKM_FILE_ITEM_INFO_T));
    
    dest_item_info_ptr->is_valid = src_item_info_ptr->is_valid;
    dest_item_info_ptr->cur_indx = src_item_info_ptr->cur_indx;
    dest_item_info_ptr->file_contx_type = src_item_info_ptr->file_contx_type;
    dest_item_info_ptr->file_full_path_len = src_item_info_ptr->file_full_path_len;
    dest_item_info_ptr->file_info_name_len = src_item_info_ptr->file_info_name_len;

     
    SCI_MEMCPY(dest_item_info_ptr->file_full_path_name, src_item_info_ptr->file_full_path_name,
                 FMM_SEARCH_FILENAME_MAXLEN*sizeof(wchar));
    SCI_MEMCPY(dest_item_info_ptr->file_info_name, src_item_info_ptr->file_info_name,
                   KM_FILE_INFO_NAME_LEN_MAX*sizeof(wchar));
    SCI_MEMCPY(dest_item_info_ptr->file_info_time, src_item_info_ptr->file_info_time,
                   KM_FILE_INFO_TIME_LEN_MAX*sizeof(wchar));
    SCI_MEMCPY(dest_item_info_ptr->file_info_year, src_item_info_ptr->file_info_year,
                   KM_FILE_INFO_YEAR_LEN_MAX*sizeof(wchar));
    return TRUE;
}
                                                                  
/*****************************************************************************/
//  Description : 是否有上次播放信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_IsLastPlayInfor(void)
{
    if(0 < s_km_play_info.cur_file_full_path_len )
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : 获取上次播放文件信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetLastPlayInfor(MMIKM_PLAY_INFO_T *km_play_info_ptr)
{
    BOOLEAN recode = FALSE;
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
    MMIAPICOM_WstrToStr(s_km_play_info.cur_file_full_path_name, char_filename);
#endif 
    if(PNULL == km_play_info_ptr)
    {
        return recode;
    }
    
    if(s_km_play_info.cur_file_full_path_len >0)
    {
        SCI_MEMSET(km_play_info_ptr, 0X00, sizeof(MMIKM_PLAY_INFO_T));
        km_play_info_ptr->cur_file_full_path_len = s_km_play_info.cur_file_full_path_len;
        km_play_info_ptr->played_time = s_km_play_info.played_time;
        MMIAPICOM_Wstrncpy(km_play_info_ptr->cur_file_full_path_name, 
                           s_km_play_info.cur_file_full_path_name,
                           s_km_play_info.cur_file_full_path_len);
        
        recode = TRUE;
    }
#ifdef KM_DEBUG_SWITCH_ENABLE
    memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1);
    MMIAPICOM_WstrToStr((const wchar*)km_play_info_ptr->cur_file_full_path_name, char_filename);
#endif 
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_GetLastPlayInfor recode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1945_112_2_18_2_31_20_47,(uint8*)"d",recode);
    return recode;
}

/*****************************************************************************/
//  Description : 保存上次播放文件信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC void MMIKM_SetLastPlayInfor(wchar    *file_full_path_name_ptr,    
                                          uint16          file_full_path_len ,
                                          uint32          played_time)
                                                                          
{
    KM_PLAYLIST_TYPE_E list_type = KM_PLAYLIST_DEFAULT;
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
    MMIAPICOM_WstrToStr(file_full_path_name_ptr, char_filename);
#endif 
    if(PNULL == file_full_path_name_ptr)
    {
        return;
    }
    s_km_play_info.cur_file_full_path_len = file_full_path_len;
    s_km_play_info.played_time = played_time;
    
    SCI_MEMSET(s_km_play_info.cur_file_full_path_name, 0X00, FMM_SEARCH_FILENAME_MAXLEN+1);
    MMIAPICOM_Wstrncpy(s_km_play_info.cur_file_full_path_name, file_full_path_name_ptr, file_full_path_len);
#ifdef KM_DEBUG_SWITCH_ENABLE 
    memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1);
    MMIAPICOM_WstrToStr((const wchar*)s_km_play_info.cur_file_full_path_name, char_filename);
#endif 

    SavePlayInforToFile(list_type);
}
/*****************************************************************************/
//  Description : 获取每种影片类型的数目
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIKM_GetMovieTypeNum(MMIKM_TYPE_E type_enum)
{   
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_GetMovieTypeNum num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_1985_112_2_18_2_31_20_48,(uint8*)"d",s_km_play_info.type_num[type_enum]);
    return s_km_play_info.type_num[type_enum];
}

/*****************************************************************************/
//  Description : 获取指定影片类型指定index的影片信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have get movie type infor; FALSE: no movie type infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetMovieTpyeInfor(
                                        uint16    index,  //in:type num索引
                                        MMIKM_TYPE_E type_enum,
                                        MMIKM_FILE_ITEM_INFO_T *km_file_item_info_ptr
                                        )
{
    uint16 i = 0;
    uint16 type_num_index = 0;
    uint16 km_type_num = 0;
    BOOLEAN recode = FALSE;
    
    if(PNULL == km_file_item_info_ptr)
    {
        return recode;
    }
    
    km_type_num = MMIKM_GetMovieTypeNum(type_enum);

    if(0 == km_type_num)
    {
        return recode;
    }

    for(i = 0; i < KM_FILE_INFO_NUM_MAX; i ++)
    {
        if(s_km_play_list_info_ptr->km_file_item_info[i].is_valid && 
             type_enum == s_km_play_list_info_ptr->km_file_item_info[i].file_contx_type) 
        {
            if(index == type_num_index)
            {
#ifdef KM_DEBUG_SWITCH_ENABLE
                uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
                MMIAPICOM_WstrToStr(s_km_play_list_info_ptr->km_file_item_info[i].file_info_name, char_filename);
#endif
                CopyFileItmInfo(km_file_item_info_ptr, &s_km_play_list_info_ptr->km_file_item_info[i]);

#ifdef KM_DEBUG_SWITCH_ENABLE
                memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1); 
                MMIAPICOM_WstrToStr(km_file_item_info_ptr->file_info_name, char_filename);
#endif
                recode = TRUE;
                break;
            }
            type_num_index ++; 
        }
    }
    
    return recode;
    
}

/*****************************************************************************/
//  Description : 删除指定影片类型指定index的影片信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: delete movie type infor success; FALSE: delete fail 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_DeltMovieTpyeInfor(
                                        uint16*       index_ptr,  //in:type num索引
                                        uint16        delete_num, //in
                                        MMIKM_TYPE_E type_enum //in 
                                       )
                                        
{
    uint16 i = 0;
    uint16 type_num_index = 0;
    uint16 km_type_num = 0;
    uint16 delete_array_index = 0;
    BOOLEAN recode = FALSE;
    KM_PLAYLIST_TYPE_E  list_type = KM_PLAYLIST_DEFAULT;
    uint16  thumbnail_file_full_name_len=0;
    wchar   thumbnail_file_full_name[FMM_SEARCH_FILENAME_MAXLEN+1]={0};



    km_type_num = MMIKM_GetMovieTypeNum(type_enum);

    if(0 == km_type_num)
    {
        return recode;
    }

    for(i = 0; (i < KM_FILE_INFO_NUM_MAX) && (delete_array_index < delete_num); i ++)
    {
        if(s_km_play_list_info_ptr->km_file_item_info[i].is_valid && 
             type_enum == s_km_play_list_info_ptr->km_file_item_info[i].file_contx_type) 
        {
            if(index_ptr[delete_array_index] == type_num_index)
            {
#ifdef KM_DEBUG_SWITCH_ENABLE
                uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
                MMIAPICOM_WstrToStr(s_km_play_list_info_ptr->km_file_item_info[i].file_info_name, char_filename);
#endif
                //物理删除文件系统中的指定文件
                //删除相应的bmp图片 

                GetThumbnailImageByFileName(s_km_play_list_info_ptr->km_file_item_info[i].file_full_path_name,s_km_play_list_info_ptr->km_file_item_info[i].file_info_name,thumbnail_file_full_name,&thumbnail_file_full_name_len);
                if(MMIAPIFMM_IsFileExist(thumbnail_file_full_name,thumbnail_file_full_name_len))
                {
                    MMIAPIFMM_DeleteFile(thumbnail_file_full_name, NULL); 
                }
                SCI_MEMSET(&s_km_play_list_info_ptr->km_file_item_info[i], 0X00, sizeof(MMIKM_FILE_ITEM_INFO_T));    
                //将总数和某种类型数减少一个
                s_km_play_info.total_num --;
                s_km_play_info.type_num[type_enum]--;
                delete_array_index++;
            }
            type_num_index ++; 
        }
    }
    
    if(delete_array_index == delete_num)
    {
        //重新写入文件系统
        SavePlayInforToFile(list_type);
        SavePlaylistInforToFile(list_type);
        recode = TRUE;
    }
    
    return recode;    
}
/*****************************************************************************/
//  Description : 重新设置所有的配置信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_ResetAllInfo(void)
{
    BOOLEAN recode = FALSE;
   
    MMIKM_ReleaseMemoForPlaylistInfo();

    SCI_MEMSET(&s_km_play_info, 0X00, sizeof(MMIKM_PLAY_INFO_T)); 

    if(PNULL != s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
    if(PNULL != s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
    if(PNULL != s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
#endif
    
    recode = TRUE; 
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_ResetAllInfo :recode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2143_112_2_18_2_31_20_49,(uint8*)"d", recode);
    
    return recode;
}
/*****************************************************************************/
//  Description : 重新设置playlist infor申请的资源信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_MemSetForPlaylistInfo(void)
{
    BOOLEAN recode = FALSE;
   
    if(s_km_play_list_info_ptr)
    {
        uint16 index = 0;
        SCI_MEMSET(s_km_play_list_info_ptr, 0X00, sizeof(MMIKM_PLAY_LIST_INFO_T)); 
        s_km_play_info.total_num = 0;

        for(index = 0; index < MMIKM_TYPE_MAX; index ++)
        {
            s_km_play_info.type_num[index] = 0;
        }
        recode = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIKM] MMIKM_MemSetForPlaylistInfo :recode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2168_112_2_18_2_31_20_50,(uint8*)"d", recode);
    
    return recode;
}

/*****************************************************************************/
//  Description : 释放playlist infor申请的资源信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_ReleaseMemoForPlaylistInfo(void)
{
    if(s_km_play_list_info_ptr)
    {
        SCI_FREE(s_km_play_list_info_ptr);
        s_km_play_list_info_ptr = NULL;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : playlist infor申请的资源信息
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_AlloclMemoForPlaylistInfo(void)
{
    if(s_km_play_list_info_ptr)
    {
        SCI_FREE(s_km_play_list_info_ptr);
        s_km_play_list_info_ptr = NULL;
    }

    //allco memory for s_km_play_list_info_ptr
    s_km_play_list_info_ptr = SCI_ALLOCA(sizeof(MMIKM_PLAY_LIST_INFO_T));
    if(PNULL == s_km_play_list_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2204_112_2_18_2_31_20_51,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(s_km_play_list_info_ptr, 0X00, sizeof(MMIKM_PLAY_LIST_INFO_T)); 

    //SCI_TRACE_LOW:"[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :ok "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2210_112_2_18_2_31_21_52,(uint8*)"");
    return TRUE;
    
}

#ifdef KM_DEBUG_SWITCH_ENABLE
/*****************************************************************************/
//  Description : OpenVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenVideoSecurty(void * pvoid)
{
    return TRUE;   
}
/*****************************************************************************/
//  Description : CloseVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CloseVideoSecurty(void)
{

}

/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailInfo
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetVideoSecurtyThumbnailInfo(void)
{
    uint32 ret_val = 18000;
    return ret_val;
    //return MMIKM_STORE_FILE_DIR_LEN;
}
/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailData
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoSecurtyThumbnailData(uint8 *data_buffer_ptr, uint32 data_len)
{
    return TRUE;
}
#else

extern  USER_SECKEY_PROCESS_FUN userkey_process_callback(uint8 * user_secbuff, uint8 * org_seckey,uint16 keylen);
/*****************************************************************************/
//  Description : OpenVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenVideoSecurty(void * pvoid)
{
    BOOLEAN recode = FALSE;
    uint32 vdec_sec_argc = 1;

    if(PNULL == pvoid)
    {
        return recode;
    }
#if defined MMI_KING_MOVIE_SUPPORT && defined VIDEO_PLAYER_SUPPORT
	 SPRDMV_UserSecFun_RegCallback(userkey_process_callback);
#endif

    MMIAPIKM_CkeckSupportfile((wchar*)pvoid);
    
    s_vdec_sec_argv[0] = pvoid;
    if(MMIAPIKM_IsSupportfile())
    {
        s_vdec_sec_argv[0] = pvoid;
        s_sprd_kmv=TRUE;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
        s_sprd_hmv=FALSE;
#endif
        s_vplayer_sec_handle = VPlayerSecurity_Open(vdec_sec_argc,(void *)s_vdec_sec_argv);
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND     
        if(PNULL==s_vplayer_sec_handle)
        {
            s_vplayer_sec_handle=VPlayerSecurity_Open_Kmv(vdec_sec_argc,(void *)s_vdec_sec_argv);
            if(PNULL != s_vplayer_sec_handle)
            {
                s_sprd_kmv=FALSE;
            }
        }
#endif        
        SCI_TRACE_LOW("[KM] OpenVideoSecurty1 handle=0x%x",s_vplayer_sec_handle);

        if(PNULL != s_vplayer_sec_handle)
        {
            recode = TRUE;
    }
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV    
    else if(MMIAPIKM_IsHmvfile())
    {
        s_vdec_sec_argv[0] = pvoid;
        s_sprd_hmv=TRUE;
        s_sprd_kmv=FALSE;
        s_vplayer_sec_handle = VPlayerSecurity_Open_Hmv(vdec_sec_argc,(void *)s_vdec_sec_argv);        
        SCI_TRACE_LOW("[KM] OpenVideoSecurty2 handle=0x%x",s_vplayer_sec_handle);
        if(PNULL != s_vplayer_sec_handle)
        {
            recode = TRUE;
        }
    }
#endif
    //SCI_TRACE_LOW:"[KM] OpenVideoSecurty recode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2305_112_2_18_2_31_21_55,(uint8*)"d",recode);
    return recode;
}
/*****************************************************************************/
//  Description : CloseVideoSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CloseVideoSecurty(void)
{
      SPRDMV_UserSecFun_UnRegCallback( );

    if(MMIAPIKM_IsSupportfile())
    {
        if(s_vplayer_sec_handle)
        {    
            //SCI_TRACE_LOW:"[KM] CloseVideoSecurty handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2318_112_2_18_2_31_21_56,(uint8*)"d",s_vplayer_sec_handle);
            if(s_sprd_kmv)
            {
                VPlayerSecurity_Close(s_vplayer_sec_handle);
            }
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND     
            else
            {
                VPlayerSecurity_Close_Kmv(s_vplayer_sec_handle);
            }
#endif
            s_vplayer_sec_handle = PNULL;
        }
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    else if(MMIAPIKM_IsHmvfile())
    {
        if(s_vplayer_sec_handle)
        {    
            //SCI_TRACE_LOW:"[KM] CloseVideoSecurty handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2337_112_2_18_2_31_21_57,(uint8*)"d",s_vplayer_sec_handle);
            VPlayerSecurity_Close_Hmv(s_vplayer_sec_handle);
            
            s_vplayer_sec_handle = PNULL;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailInfo
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetVideoSecurtyThumbnailInfo(void)
{
    int result = MMI_ERROR;
    VPLAYER_SEC_THUMBNAIL_INFO_T vp_thumbnail_info = {0};
    if(s_vplayer_sec_handle)
    {
        if(s_sprd_kmv)
        {
            result = VPlayerSecurity_GetThumbnailInfo(s_vplayer_sec_handle,&vp_thumbnail_info); 
        }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
        else if(s_sprd_hmv)
        {
            result = VPlayerSecurity_GetThumbnailInfo_Hmv(s_vplayer_sec_handle,&vp_thumbnail_info);
        }
#endif
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND         
        else
        {
            result = VPlayerSecurity_GetThumbnailInfo_Kmv(s_vplayer_sec_handle,&vp_thumbnail_info);
        }
#endif                                                                        
    }
    if(MMI_OK != result)
    {
        vp_thumbnail_info.data_len = 0;
    }
    return vp_thumbnail_info.data_len;   
}

/*****************************************************************************/
//  Description : GetVideoSecurtyThumbnailData
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoSecurtyThumbnailData(uint8 *data_buffer_ptr, uint32 data_len)
{
    int result = MMI_ERROR;
    BOOLEAN recode = FALSE;
    if(PNULL == data_buffer_ptr || 0 == data_len)
    {
        return recode;
    }

    if(s_vplayer_sec_handle)
    {
        if(s_sprd_kmv)
        {
            result = VPlayerSecurity_GetThumbnailData(s_vplayer_sec_handle, data_buffer_ptr, data_len);                              
        }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
        else if(s_sprd_hmv)
        {
            result = VPlayerSecurity_GetThumbnailData_Hmv(s_vplayer_sec_handle, data_buffer_ptr, data_len);                              
        }
#endif
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND         
        else
        {
            result = VPlayerSecurity_GetThumbnailData_Kmv(s_vplayer_sec_handle, data_buffer_ptr, data_len);                              
        }
#endif     
        //SCI_TRACE_LOW:"[KM] GetVideoSecurtyThumbnailData result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2411_112_2_18_2_31_21_58,(uint8*)"d",result);
    }
    if(MMI_OK == result)
    {
        recode = TRUE;
    }  

    //SCI_TRACE_LOW:"[KM] GetVideoSecurtyThumbnailData recode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2418_112_2_18_2_31_21_59,(uint8*)"d",recode);
    return recode;
}

#endif


#define BE_4B2U32(a,b,c,d) ( ((uint8)(a)<<24) | ((uint8)(b)<<16) | ((uint8)(c)<<8) | ((uint8)(d)) )
#define SEC_BOX_HEADER_LEN   8 


typedef struct
{
    uint32  file_pos;
    uint32  box_size;

    uint8   name[16];
    uint8   head[16];
    
}MP4BOX_T;

typedef struct
{
    SFS_HANDLE file_handle;
    uint32 file_size;
    uint32 cur_pos;

    uint32 udta_is_ok;  // 0: none or error. 1: is ok.
    MP4BOX_T udta_box;

}FILE_INFO_T;

typedef struct
{
    uint8 *p_data;
    uint32 len;
}MP4_SMV_DATA_T;

/*****************************************************************************/
//  Description : Mp4FileOpen
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_HANDLE Mp4FileOpen(void * pvoid)
{                
    return MMIFILE_CreateFile((const wchar *)pvoid, SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
}

/*****************************************************************************/
//  Description : Mp4FileClose
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL void Mp4FileClose(MMIFILE_HANDLE file_handle)
{
    MMIFILE_CloseFile(file_handle);
}

LOCAL uint32 mp4file_read(FILE_INFO_T *p_file,uint8 *p_buff,uint32 size2read)
{
    uint32 size_read_out = 0;
    
    // TODO, read the file.
    if(PNULL != p_file && PNULL != p_buff)
    {
        MMIFILE_ReadFile(p_file->file_handle, (void*)p_buff, size2read, &size_read_out, NULL);
    }

    return size_read_out;
}

LOCAL uint32 mp4file_seek(FILE_INFO_T *p_file,uint32 target_pos)
{
    uint32 ret = SCI_ERROR;
    
    // TODO, seek the file.
    if(PNULL != p_file)
    {
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(p_file->file_handle, target_pos, SFS_SEEK_BEGIN))
        {
            p_file->cur_pos = target_pos;
            ret = SCI_SUCCESS;
        }
    }
    
    return ret;
}



LOCAL int mp4file_readmp4box(FILE_INFO_T *p_file,MP4BOX_T *p_box)
{
    int ret = SCI_ERROR;
    uint32 cur_pos = 0;

    cur_pos = p_file->cur_pos;

    if( 16 == mp4file_read(p_file,p_box->head,16) )
    {
        p_box->file_pos = cur_pos;
        p_box->box_size = BE_4B2U32(p_box->head[0],p_box->head[1],p_box->head[2],p_box->head[3]);
        //
        SCI_MEMCPY(p_box->name,&(p_box->head[4]),4);

        ret = SCI_SUCCESS;
    }

    return ret;
}


LOCAL int mp4file_get_udta_len(FILE_INFO_T *p_file)
{
    int ret = SCI_ERROR;
    //
    uint32 sec_box_type;
    MP4BOX_T box_tmp = {0};
    uint8 *p_head;
    //
    uint32 box_end_pos;
    uint32 next_box_pos;
    
    // get the start_pos/information of udta box.
    mp4file_seek(p_file,0);
    ret = mp4file_readmp4box(p_file,&box_tmp);
    while( SCI_SUCCESS == ret )
    {
        if(0 == box_tmp.box_size)
        {
            return 0;
        }
        // udta box is in moov box.
        p_head = &(box_tmp.head[0]);
        sec_box_type = BE_4B2U32(p_head[4],p_head[5],p_head[6],p_head[7]);
        if( sec_box_type == BE_4B2U32('m','o','o','v') )
        {
            // search the udta box.
            box_end_pos = box_tmp.file_pos + box_tmp.box_size;
            next_box_pos = box_tmp.file_pos + SEC_BOX_HEADER_LEN;
            //
            mp4file_seek(p_file,next_box_pos);
            ret = mp4file_readmp4box(p_file,&box_tmp);
            while( (SCI_SUCCESS == ret) && (box_end_pos > next_box_pos) )
            {
                p_head = &(box_tmp.head[0]);
                sec_box_type = BE_4B2U32(p_head[4],p_head[5],p_head[6],p_head[7]);
                if( sec_box_type == BE_4B2U32('u','d','t','a') )
                {
                    p_file->udta_is_ok = 1;
                    p_file->udta_box.file_pos = box_tmp.file_pos;
                    p_file->udta_box.box_size = box_tmp.box_size;
 
                    break;
                }
                else
                {
                    // point to next box.
                    next_box_pos = box_tmp.file_pos + box_tmp.box_size;
                    ret = mp4file_seek(p_file,next_box_pos);
                    if(SCI_SUCCESS == ret)
                    {
                        ret = mp4file_readmp4box(p_file,&box_tmp);
                    }
                }
            }
        }
        else
        {
            // point to next box.
            next_box_pos = box_tmp.file_pos + box_tmp.box_size;
            ret = mp4file_seek(p_file,next_box_pos);
            if(SCI_SUCCESS == ret)
            {
                ret = mp4file_readmp4box(p_file,&box_tmp);
            }
        }
    }

    if((p_file->udta_is_ok) && (p_file->udta_box.box_size > SEC_BOX_HEADER_LEN) )
    {
        return (p_file->udta_box.box_size - SEC_BOX_HEADER_LEN);
    }
    else
    {
        return 0;
    }
}


LOCAL int mp4file_get_udta_data(FILE_INFO_T *p_file, MP4_SMV_DATA_T *p_smv_data)
{
    int ret = SCI_ERROR;
    //VDEC_SEC_FILE_INFO_T *p_file_info = PNULL;
    MP4BOX_T *p_udta_box = PNULL;
    uint32 size2read = 0;
    uint32 size_read_out = 0;

    if( p_file->udta_is_ok > 0 )
    {
        p_udta_box = &(p_file->udta_box);
        if(SCI_SUCCESS == mp4file_seek(p_file,p_file->udta_box.file_pos + SEC_BOX_HEADER_LEN) )
        {
            size2read = (p_udta_box->box_size > SEC_BOX_HEADER_LEN) ? (p_udta_box->box_size-SEC_BOX_HEADER_LEN) : 0;
            size2read = (size2read > p_smv_data->len) ? p_smv_data->len : size2read;
            //
            size_read_out = mp4file_read(p_file,p_smv_data->p_data,size2read);
            if(size_read_out == size2read)
            {
                ret = SCI_SUCCESS;
            }
        }
    }

    return ret;
}




/*****************************************************************************/
//  Description : get kmv file content 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 调用完该函数一定要释放kmv_infor_ptr->data_buf_ptr
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetSmvFileContent(wchar *file_full_path_ptr,
										KMV_INFORMATION_T *kmv_infor_ptr
                                                )
{
    //uint8 *data_buf_ptr = NULL;
    //uint32 data_len = 0;
    BOOLEAN recode = FALSE;
    //MMIFILE_HANDLE  file_handle = 0;
    FILE_INFO_T mp4_file_info = {0};
    MP4_SMV_DATA_T smv_data = {0};
    
    if(NULL == file_full_path_ptr || NULL == kmv_infor_ptr)
    {
        //SCI_TRACE_LOW:"[KM] MMIKM_GetSmvFileContent:file_full_path_ptr = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2652_112_2_18_2_31_21_60,(uint8*)"");
        return recode;
    }
    
    //open mp4 file
    mp4_file_info.file_handle = Mp4FileOpen(file_full_path_ptr);
    if(SFS_INVALID_HANDLE == mp4_file_info.file_handle) 
    {
         //SCI_TRACE_LOW:"[KM] MMIKM_GetSmvFileContent:Mp4FileOpen fail"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2660_112_2_18_2_31_21_61,(uint8*)"");
         return recode;
    }
    
    //get mp4 file information
    smv_data.len = mp4file_get_udta_len(&mp4_file_info);
    //SCI_TRACE_LOW:"[KM] MMIKM_GetSmvFileContent data_len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2666_112_2_18_2_31_21_62,(uint8*)"d", smv_data.len);
    
    if(smv_data.len > 0)
    {
        smv_data.p_data = SCI_ALLOCA(sizeof(uint8)*smv_data.len);
        if(NULL != smv_data.p_data)
        {
            memset(smv_data.p_data, 0x00, smv_data.len);
            //get video securty data
            if(SCI_SUCCESS == mp4file_get_udta_data(&mp4_file_info, &smv_data))
            {
#ifdef  KM_DEBUG_SWITCH_ENABLE    
                //for debug
                MMIFILE_HANDLE file_handle;
                uint32 bytes_to_write = 0;
                //file_handle = MMIFILE_CreateFile(MMIKM_TEST_FILE_NAME, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                file_handle = MMIFILE_CreateFile(MMIKM_TEST_FILE_NAME, SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {   
                    //MMIFILE_WriteFile(file_handle, (const void*)data_buf_ptr, data_len, &bytes_to_write, NULL);
                    MMIFILE_ReadFile(file_handle, (void*)smv_data.p_data, smv_data.len, &bytes_to_write, NULL);
                    MMIFILE_CloseFile(file_handle);
                }
#endif    
                ParseKmvFile(kmv_infor_ptr, smv_data.p_data, smv_data.len);
                recode = TRUE;
            }   
        } 
    }
    if(PNULL != kmv_infor_ptr->data_buf_ptr)
    {
        SCI_FREE(kmv_infor_ptr->data_buf_ptr);
        kmv_infor_ptr->data_buf_ptr = PNULL;
    }
    kmv_infor_ptr->data_buf_ptr = smv_data.p_data;
    //close video securty
    Mp4FileClose(mp4_file_info.file_handle);

    return recode;
}

/*****************************************************************************/
//  Description : get kmv file content 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 调用完该函数一定要释放kmv_infor_ptr->data_buf_ptr
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetKmvFileContent(wchar *file_full_path_ptr,
										KMV_INFORMATION_T *kmv_infor_ptr
                                                )
{
    uint8 *data_buf_ptr = NULL;
    uint32 data_len = 0;
    BOOLEAN recode = FALSE;
    if(NULL == file_full_path_ptr || NULL == kmv_infor_ptr)
    {
        //SCI_TRACE_LOW:"[KM] MMIKM_GetKmvFileContent:file_full_path_ptr = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2719_112_2_18_2_31_22_63,(uint8*)"");
        return recode;
    }
    
    //open video security
    recode = OpenVideoSecurty(file_full_path_ptr);
    if(FALSE == recode)
    {
         //SCI_TRACE_LOW:"[KM] MMIKM_GetKmvFileContent:OpenVideoSecurty fail"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2727_112_2_18_2_31_22_64,(uint8*)"");
         return recode;
    }
    
    //get video securty information
    data_len = GetVideoSecurtyThumbnailInfo();
    //SCI_TRACE_LOW:"[KM] GetKmvFileContent data_len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2733_112_2_18_2_31_22_65,(uint8*)"d", data_len);
    
    if(data_len > 0)
    {
        data_buf_ptr = SCI_ALLOCA(sizeof(uint8)*data_len);
        if(NULL != data_buf_ptr)
        {
            memset(data_buf_ptr, 0x00, data_len);
            //get video securty data
            if(GetVideoSecurtyThumbnailData(data_buf_ptr, data_len))
            {
#ifdef  KM_DEBUG_SWITCH_ENABLE    
                //for debug
                MMIFILE_HANDLE file_handle;
                uint32 bytes_to_write = 0;
                //file_handle = MMIAPIFMM_CreateFile(MMIKM_TEST_FILE_NAME, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                file_handle = MMIAPIFMM_CreateFile(MMIKM_TEST_FILE_NAME, SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {   
                    //MMIAPIFMM_WriteFile(file_handle, (const void*)data_buf_ptr, data_len, &bytes_to_write, NULL);
                    MMIAPIFMM_ReadFile(file_handle, (void*)data_buf_ptr, data_len, &bytes_to_write, NULL);
                    MMIAPIFMM_CloseFile(file_handle);
                }
#endif    
                ParseKmvFile(kmv_infor_ptr, data_buf_ptr, data_len);
                recode = TRUE;
            }   
        } 
    }
    if(PNULL != kmv_infor_ptr->data_buf_ptr)
    {
        SCI_FREE(kmv_infor_ptr->data_buf_ptr);
        kmv_infor_ptr->data_buf_ptr = PNULL;
    }
    kmv_infor_ptr->data_buf_ptr = data_buf_ptr;
    //close video securty
    CloseVideoSecurty();

    return recode;
}


/*****************************************************************************/
//  Description : 解析videosecurty数据内容
//  Global resource dependence : 
//  Author: haili
//  Note: 
/*****************************************************************************/
LOCAL BOOL ParseKmvFile(KMV_INFORMATION_T  *ret_info,  uint8 *bufptr, uint32 len)
{
    uint8 *buf_ptr;
    uint8  ret = FALSE;
    uint32 item_size=0; 
    uint32 item_size_offset=0; 
 
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: ParseKmvFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2785_112_2_18_2_31_22_66,(uint8*)"");
    
    //SCI_TRACE_LOW("[Mmikm_playlist.c]: buf_ptr %0x,%x0,%0x",buf_ptr[0], bufptr[1], bufptr[2]);
    buf_ptr = bufptr + MEMORY_OFFSET;

    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2790_112_2_18_2_31_22_67,(uint8*)"dddd",buf_ptr[0], buf_ptr[1], buf_ptr[2], buf_ptr[3]);
    if(buf_ptr[0] == 0x42 && buf_ptr[1] == 0x4D )//BM
    {	
        ret_info->item_thumbnail.item_buffer_ptr = buf_ptr;
        ret_info->item_thumbnail.item_size = (uint16)(buf_ptr[3]<<8) +(uint16)(buf_ptr[2]);
        ret_info->item_thumbnail.item_is_true = TRUE;
    }
    else
    {
    	return ret;  
    }
    
    // get year	must have
    item_size_offset = ret_info->item_thumbnail.item_size;
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2804_112_2_18_2_31_22_68,(uint8*)"d",item_size_offset);

    if(item_size_offset > len )
    {
    	return ret;  
    } 
    buf_ptr += ret_info->item_thumbnail.item_size;

    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get year buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2812_112_2_18_2_31_22_69,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);
    if(buf_ptr[4] == 0xA9 && buf_ptr[5] == 0x64 && buf_ptr[6] == 0x61 && buf_ptr[7] == 0x79)
    {
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2815_112_2_18_2_31_22_70,(uint8*)"dddd",buf_ptr[0], buf_ptr[1], buf_ptr[2],buf_ptr[3]);
    	item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) + (uint32)(buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get year item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2817_112_2_18_2_31_22_71,(uint8*)"dd",item_size,item_size);

        if(!(item_size - ITEM_OFFSET>0))
        {
            return ret;
        }

    	ret_info->item_year.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
    	ret_info->item_year.item_size= item_size - ITEM_OFFSET;	
    	ret_info->item_year.item_is_true = TRUE;
    }
    else
    {
        return ret;
    }
    
    // get time must have
    item_size_offset += item_size; 
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2835_112_2_18_2_31_22_72,(uint8*)"d",item_size_offset);
    if(item_size_offset > len )
    {
    	return ret;  
    } 
    buf_ptr += item_size;
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get time buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2841_112_2_18_2_31_22_73,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);
    if(buf_ptr[4]==0xA9 && buf_ptr[5]==0x63 && buf_ptr[6]==0x6D && buf_ptr[7]==0x74)
    {
    	item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) +(uint32)( buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get time item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2845_112_2_18_2_31_22_74,(uint8*)"dd",item_size,item_size);

        if(!(item_size - ITEM_OFFSET>0))
        {
            return ret;
        }

    	ret_info->item_time.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
    	ret_info->item_time.item_size = item_size - ITEM_OFFSET;	
    	ret_info->item_time.item_is_true = TRUE;
    }
    else
    {
        return ret;
    }

    // get name
    item_size_offset += item_size;
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2863_112_2_18_2_31_22_75,(uint8*)"d",item_size_offset);
    if(item_size_offset > len )
    {
    	return ret;  
    } 
    buf_ptr += item_size;

    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get name buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2870_112_2_18_2_31_22_76,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);
    if(buf_ptr[4] == 0xA9 && buf_ptr[5] == 0x6E && buf_ptr[6] == 0x61 && buf_ptr[7] == 0x6D)
    {
    	item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) + (uint32)(buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get name item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2874_112_2_18_2_31_22_77,(uint8*)"dd",item_size,item_size);
    	
        if(!(item_size - ITEM_OFFSET>0))
        {
            return ret;
        }

    	ret_info->item_name.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
    	ret_info->item_name.item_size = item_size - ITEM_OFFSET;	
    	ret_info->item_name.item_is_true = TRUE;
    }
    else
    {
        ret_info->item_name.item_is_true = FALSE;
    }	

    // get actor
    if(TRUE == ret_info->item_name.item_is_true)
    {
        buf_ptr += item_size;
        item_size_offset += item_size;
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2895_112_2_18_2_31_22_78,(uint8*)"d",item_size_offset);
        if(item_size_offset > len )
        {
        	return ret;  
        } 
    }

    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get actor buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2902_112_2_18_2_31_22_79,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);

    if(buf_ptr[4] == 0xA9 && buf_ptr[5] == 0x41 && buf_ptr[6] == 0x52 && buf_ptr[7] == 0x54)  
    {
        item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) + (uint32)(buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get actor item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2907_112_2_18_2_31_22_80,(uint8*)"dd",item_size,item_size);
        
        if(!(item_size - ITEM_OFFSET>0))
        {
            return ret;
        }

        ret_info->item_actor.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
        ret_info->item_actor.item_size = item_size - ITEM_OFFSET;	
        ret_info->item_actor.item_is_true = TRUE;
    } 
    else
    {
    	ret_info->item_actor.item_is_true = FALSE;
    }
    	
    // get synop
    if(TRUE == ret_info->item_actor.item_is_true )
    {
        buf_ptr += item_size;
        item_size_offset += item_size;
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2928_112_2_18_2_31_22_81,(uint8*)"d",item_size_offset);
        if(item_size_offset > len )
        {
        	return ret;  
        } 
    }  

    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get synop buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2935_112_2_18_2_31_22_82,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);

    if(buf_ptr[4] == 0xA9 && buf_ptr[5] == 0x6C && buf_ptr[6] == 0x79 && buf_ptr[7] == 0x72)  
    {
    	item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) + (uint32)(buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get synop item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2940_112_2_18_2_31_22_83,(uint8*)"dd",item_size,item_size);
    	
        if(!(item_size - ITEM_OFFSET>0))
        {
            return ret;
        }

    	ret_info->item_synop.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
    	ret_info->item_synop.item_size = item_size - ITEM_OFFSET;	
    	ret_info->item_synop.item_is_true = TRUE;
    }
    else
    {
    	ret_info->item_synop.item_is_true = FALSE;
    }

    // get type  must have
    if(ret_info->item_synop.item_is_true == TRUE)
    {
        buf_ptr += item_size;
        item_size_offset += item_size; 
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2961_112_2_18_2_31_22_84,(uint8*)"d",item_size_offset);
        if(item_size_offset > len )
        {
        	return ret;  
        } 
    }
    
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get type buf_ptr %0x,%0x,%0x,%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2968_112_2_18_2_31_22_85,(uint8*)"dddd",buf_ptr[4], buf_ptr[5], buf_ptr[6],buf_ptr[7]);

    if(buf_ptr[4] == 0xA9 && buf_ptr[5] == 0x67 && buf_ptr[6] == 0x72 && buf_ptr[7] == 0x70)
    {
    	item_size = (uint32)(buf_ptr[0]<<24) + (uint32)(buf_ptr[1]<<16) + (uint32)(buf_ptr[2]<<8) + (uint32)(buf_ptr[3]);
        //SCI_TRACE_LOW:"[Mmikm_playlist.c]: get type item_size %0x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2973_112_2_18_2_31_22_86,(uint8*)"dd",item_size,item_size);

        if(!(item_size - ITEM_OFFSET > 0))
        {
            return ret;
        }

    	ret_info->item_type.item_buffer_ptr = buf_ptr + ITEM_OFFSET;
    	ret_info->item_type.item_size = item_size - ITEM_OFFSET;
    	ret_info->item_type.item_is_true = TRUE;
    }
    else
    {
        return ret;
    }	

    item_size_offset += item_size;
    //SCI_TRACE_LOW:"[Mmikm_playlist.c]: item_size_offset= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_2990_112_2_18_2_31_22_87,(uint8*)"d",item_size_offset);
    if(item_size_offset > len )
    {
    	return ret;  
    } 
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : 是否播放列表为空
//  Global resource dependence :  none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_IsPlayListEmpty(void)
{
    MMIKM_TYPE_E type = MMIKM_TYPE_ACTION;
    BOOLEAN result = TRUE;

    for(type=MMIKM_TYPE_ACTION; type<MMIKM_TYPE_MAX; type++)
    {
        if(MMIKM_GetMovieTypeNum(type) > 0 && MMIKM_GetMovieTypeNum(type) <= KM_FILE_INFO_NUM_MAX)
        {
            result = FALSE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 保存上次播放文件的进度 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_SaveLastPorcesstime(uint32 process_time)
{
    KM_PLAYLIST_TYPE_E  list_type=KM_PLAYLIST_DEFAULT;
    s_km_play_info.played_time=process_time;
    SavePlayInforToFile(list_type);
}
/*****************************************************************************/
//  Description : 获取上次播放文件的进度 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32  MMIAPIKM_GetLastPorcesstime(void)
{
    return s_km_play_info.played_time;
}

/*****************************************************************************/
//  Description : 删除FileArray
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_FileArrayDestroy(void)
{
    if(PNULL != s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
    if(PNULL != s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_sd2_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
    
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
    if(PNULL != s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT])
    {
        MMIAPIFILEARRAY_Destroy(&s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT]);
        s_km_play_list_array_sd1_ptr[KM_PLAYLIST_DEFAULT] = PNULL;
    }
#endif
    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_FileArrayDestroy :leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_PLAYLIST_3065_112_2_18_2_31_22_88,(uint8*)"");
}
#ifndef MMI_KM_NEED_SEARCH_FIRST_TIME
/*****************************************************************************/
//  Description : check play_info file 
//  Global resource dependence :  none
//  Author: Leiming.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_CheckPlayInfoFile(void)
{
    BOOLEAN recode = FALSE;
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_SDCARD;
    wchar   *full_path_ptr = PNULL;
    
    uint16  full_path_len = FMM_SEARCH_FILENAME_MAXLEN + 1;
    
    s_play_list_dev.dev_type = MMI_DEVICE_SYSTEM;
    
    full_path_ptr = SCI_ALLOCA((FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
    if(NULL == full_path_ptr)
    {
        return FALSE;
    }  
    
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
    for(dev = MMI_DEVICE_NUM -1; dev >= MMI_DEVICE_UDISK; dev--)
#else
    for(dev = MMI_DEVICE_NUM -1; dev >= MMI_DEVICE_SDCARD; dev--)
#endif
    {
            //check play infor file
            SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
            CombinePlayListFileName(dev, MMIKM_FILE_TYPE_PLAY_INFOR, full_path_ptr, &full_path_len);
            if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
            {
                s_play_list_dev.dev_type = dev;
                recode = TRUE;
                break;
            } 
    }

    SCI_FREE(full_path_ptr);
    return recode;

}

/*****************************************************************************/
//  Description : save play_info file 
//  Global resource dependence :  none
//  Author: Leiming.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_LoadPlayInfoFromFile(MMIKM_PLAY_INFO_T *km_play_info_ptr)
{
    wchar               abs_default_path[KM_FILE_INFO_FULL_NAME_LEN_MAX+1]  =   {0};
    uint16              abs_default_path_length = KM_FILE_INFO_FULL_NAME_LEN_MAX;
    MMIFILE_DEVICE_E    dev_type = s_play_list_dev.dev_type;
    MMIFILE_ERROR_E     save_result = SFS_NO_ERROR;
    MMIFILE_HANDLE      file_handle =   0; 


    if(GetFileNameForReadWrite(dev_type,
                                MMIKM_FILE_TYPE_PLAY_INFOR, 
                                abs_default_path, 
                                &abs_default_path_length, 
                                KM_PLAYLIST_DEFAULT))
    {
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {
            file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
            if(SFS_INVALID_HANDLE != file_handle)
            {
                uint32 read_len = 0;
                SCI_MEMSET(km_play_info_ptr, 0x00, sizeof(MMIKM_PLAY_INFO_T));
                save_result = MMIAPIFMM_ReadFile(file_handle, (void*)km_play_info_ptr, sizeof(MMIKM_PLAY_INFO_T), &read_len, NULL);
                MMIAPIFMM_CloseFile(file_handle);
            }

        }
        else
        {
            save_result = SFS_ERROR_NOT_EXIST;
        }
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }

    if (SFS_NO_ERROR == save_result)
    {
        if (km_play_info_ptr->cur_file_full_path_len > ARR_SIZE(km_play_info_ptr->cur_file_full_path_name))
        {
            save_result = SFS_ERROR_ACCESS; //校验失败
        }
    }

    return save_result;
}
#endif // MMI_KM_NEED_SEARCH_FIRST_TIME