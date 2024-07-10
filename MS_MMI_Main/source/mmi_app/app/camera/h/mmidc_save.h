#ifndef _MMIDC_SAVE_H
#define _MMIDC_SAVE_H

#include "dal_recorder.h"
#include "mmi_filemgr.h"
#include "mmifilearray_export.h"
#include "mmidc_setting.h"

#define	MMIDC_NAME_MAX_LEN			52			// 照片名的最大长度，不包括扩展名,单位为byte
#define MMIDC_NAME_CIRCLE_TIMES_LIMIT      10   //图片命名查找循环次数限制

typedef enum
{
    DC_SAVE_NO_ERROR = 0,
        DC_SAVE_SUCCESS = 0,
        DC_SAVE_ERROR,
        DC_SAVE_NOROOM,
        DC_SAVE_MAX
}DC_SAVE_RESULT_E;

typedef struct 
{
    MMIDC_FILE_INFO_T file_info;
    wchar   ucs2_name[MMIDC_NAME_MAX_LEN + 1];
}PHOTO_STOREG_INFO_T;

typedef struct 
{
    SFS_HANDLE              sfs_handle;
    wchar                   record_file_name_arr[MMIDC_NAME_MAX_LEN + 1];
    wchar                   video_full_path_arr[MMIDC_FULL_PATH_LENGTH + 1];
    PHOTO_STOREG_INFO_T     photo_info[MAX_DC_BURST_PHOTO_NUMBER];
    uint32 max_file_size; //to alloc memory
    uint32 temp_name_id; //to save temp file id
}MMIDC_SAVE_DATA;

typedef enum
{
    MMIDC_SUCCESS = 0,          //成功
    MMIDC_ERR_DELETE_SUCCESS,      //删除成功
    MMIDC_ERR_DELETE_FAIL,         //删除失败
    MMIDC_ERR_FAIL,               //一般的错误提示
/*    
    MMIDC_ERR_NO_SPACE ,       // 空间不足
    MMIDC_ERR_EXIT,            // 其他需要退出的错误
    MMIDC_ERR_INVALID_NAME,    //无效文件名
    MMIDC_ERR_LONG_FILENAME,   //文件名超过长度
    MMIDC_ERR_QUERY_OVERWRITE  //询问是否覆盖文件
*/    
    MMIDC_ERR_MAX
}MMIDC_ERROR_TYPE_E;

/*****************************************************************************/
// 	Description : save captured photos
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_SavePhotos(int32 photo_id);

/*****************************************************************************/
//  Description : get rename file status
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetRenameFileCircleFlag(void);

/*****************************************************************************/
//  Description : set rename file status
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRenameFileCircleFlag(BOOLEAN rename_status);

/*****************************************************************************/
// 	Description : delete current photo
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeletePhoto(void);

/*****************************************************************************/
// 	Description : create recording video name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateVideoName(DRECORDER_START_RECORD_T* recorder_start_t);

/*****************************************************************************/
// 	Description : delete the recorded file
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteNoSavedFile(void);

/*****************************************************************************/
// 	Description : review video
//	Global resource dependence :
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReviewVideo(void);

/*****************************************************************************/
// 	Description : set current photo as wallpaper
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCurrentPhotoAsWallpaper(void);

/*****************************************************************************/
// 	Description : send by blue tooth
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByBt(void);

#ifdef SNS_SUPPORT
PUBLIC void MMIDC_SharetoSNS(uint32 msg_id);
PUBLIC void  HandleSNSRelease(void);
#endif
/*****************************************************************************/
//  Description : get current photo name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetPhotoName(uint32 id);

/*****************************************************************************/
// 	Description : get current video name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetVideoName(void);

/*****************************************************************************/
// 	Description : send by mms
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByMMS(void);

#ifdef MMIEMAIL_SUPPORT
/*****************************************************************************/
// 	Description : send by Email
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByEmail(void);
#endif

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPictureWin(void);

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenMovieWin(void);

/*****************************************************************************/
//  Description : combine path according to root, dir, name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_CombinePath(wchar* path_ptr, uint16 path_len, MMIFILE_DEVICE_E file_dev, const wchar* dir_ptr);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_NewSaveData(void);

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteSaveData(void);

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIDC_GetPicName(void);

/*****************************************************************************/
// 	Description : get current video file name
//	Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-27
//	Note:
/*****************************************************************************/
PUBLIC wchar * MMIDC_GetVideoFullFileName(void);

/*****************************************************************************/
// 	Description : get  file size
//	Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-27
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetFileSize(wchar *filename);

/*****************************************************************************/
// 	Description : get current photo information
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_STOREG_INFO_T * MMIDC_GetCurrentPhotoInfo(void);

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param 
/*****************************************************************************/
PUBLIC void  MMIDC_ShowMsg(MMIDC_ERROR_TYPE_E error);

/*****************************************************************************/
//  Description : save captured image data to image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_SaveImageFile(int32 photo_id);

/*****************************************************************************/
//  Description : rename image file name to photo file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_RenameImageFileToPhotoFile(int32 photo_id);

/*****************************************************************************/
//Description : get image file name
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetImageFileName(int32 index);

/*****************************************************************************/
//Description : set image file info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileInfo(int32 index, //in
                                        MMIDC_FILE_INFO_T *file_info_ptr //in
                                        );
                                                
/*****************************************************************************/
//Description : set image file enable
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileEnable(int32 index,
                                              BOOLEAN is_enable
                                              );
                                                        
/*****************************************************************************/
//Description : set image file attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileHidden(int32 index,
                                             BOOLEAN is_hidden
                                             );
                                             
/*****************************************************************************/
//Description : set temp file name id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetTempFileNameID(uint32 name_id);

/*****************************************************************************/
//  Description : get temp file name id
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetTempFileNameID(void);

/*****************************************************************************/
//  Description : get photo default path
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC wchar *MMIDC_GetPhotoDefaultPathName(void);

/*****************************************************************************/
//  Description : delete all disk image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteAllDiskImageFile(void);

/*****************************************************************************/
//  Description : delete saved image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteSavedImageFile(int32 index);

/*****************************************************************************/
//  Description : delete all saved image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteAllSavedImageFile(void);

/*****************************************************************************/
//  Description : init photo store info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitPhotoStoreInfo(void);

/*****************************************************************************/
//  Description : get max photo file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxPhotoFileSize(void);

/*****************************************************************************/
//  Description : set max image file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMaxImageFileSize(uint32 file_size);

/*****************************************************************************/
//  Description : get max image file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxImageFileSize(void);

/*****************************************************************************/
//  Description : close all file handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAllFileHandle(void);

/*****************************************************************************/
//  Description : create all file handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateAllFileHandle(void);

#endif

