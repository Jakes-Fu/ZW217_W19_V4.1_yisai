
#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef CAMERA_SUPPORT
#include "os_api.h"
#include "mmi_applet_table.h"
#include "mmifmm_export.h"
//#include "dal_jpeg.h"
#include "mmidc_camera_text.h"
#include "mmidc_display.h"
#include "dal_dcamera.h"
#include "dal_time.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h"
//#include "mmimultim_text.h"
#include "mmimms_export.h"
#include "mmisd_export.h"
#include "mmidc_option.h"
#ifdef DC_WATCH_UI_SUPPORT
#include "watch_gallery_export.h"
#else
#include "mmipicview_export.h"
#endif
#include "sfs.h"
#include "mmiudisk_export.h"
#include "mmitheme_update.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif

#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#include "mmidc_export.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#endif
#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_export.h"
#endif


/*
#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif
*/

#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif
#define DV_VIDEOEXT_3GP					            "3gp"
#define DV_VIDEOEXT_MP4					            "mp4"
#define DV_VIDEOEXT_AVI					            "avi"

#ifdef ZDT_PICLIST_SAVE_ONLY_NUM
#define DC_SAVE_MAX_NUM 5
#endif

#ifdef SNS_SUPPORT
LOCAL IMMISns *pMe  = PNULL;
#endif
 
LOCAL BOOLEAN s_rename_file_status = FALSE;
static MMIDC_SAVE_DATA*          s_save_data_ptr = PNULL;
#define MMIDC_SAVE_DATA_PTR      (PNULL != s_save_data_ptr ? s_save_data_ptr : (MMIDC_SAVE_DATA*)MMIDC_AssertMacro())

LOCAL const wchar* s_temp_file[]=
{
    L"_@#dctemp#@_0.dtemp",
    L"_@#dctemp#@_1.dtemp",
    L"_@#dctemp#@_2.dtemp",
    L"_@#dctemp#@_3.dtemp",
    L"_@#dctemp#@_4.dtemp",
    L"_@#dctemp#@_5.dtemp"
};

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_NewSaveData(void)
{
    if (PNULL == s_save_data_ptr)
    {
        s_save_data_ptr = (MMIDC_SAVE_DATA*)SCI_ALLOCA(sizeof(MMIDC_SAVE_DATA));
        if (PNULL == s_save_data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_NewSaveData s_save_data_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_82_112_2_18_2_9_40_528,(uint8*)"");
            return ;
        }
    }
    SCI_MEMSET(s_save_data_ptr, 0, sizeof(MMIDC_SAVE_DATA));
}

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteSaveData(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_DeleteSaveData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_95_112_2_18_2_9_40_529,(uint8*)"");
    
    if(PNULL != s_save_data_ptr)
    {
        SCI_Free(s_save_data_ptr);
        s_save_data_ptr = PNULL;
    }
}
/******************************************************************************/
//  Description : 获得随机数
//  Global resource dependence : none
//  Author:  jinju.ma
//  Note:  
/*****************************************************************************/
LOCAL uint32 GetRand(uint32 min,uint32 max);

/*****************************************************************************/
// 	Description : send by mms
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByMMS(void)
{
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;

    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {    
        photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
        if (PNULL != photo_info_ptr)
        {
            full_path_ptr = photo_info_ptr->file_info.file_name;
            full_path_len = photo_info_ptr->file_info.file_name_len;
            MMIAPIFMM_GetFileInfoFormFullPath(full_path_ptr,full_path_len,&file_info);
        }        
        
        file_type = MMIFMM_FILE_TYPE_PICTURE;
    }
    else
    {
        full_path_len = MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr);
        MMIAPIFMM_GetFileInfoFormFullPath(MMIDC_SAVE_DATA_PTR->video_full_path_arr,full_path_len,&file_info);
        file_type = MMIFMM_FILE_TYPE_MOVIE;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SendByMMS file_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_136_112_2_18_2_9_40_530,(uint8*)"d",file_type);
    
    //若文件为空，在接口内有错误提示
    MMIAPIMMS_EditMMSFromFile(&file_info,file_type);
}

#ifdef MMIEMAIL_SUPPORT
/*****************************************************************************/
// 	Description : send by Email
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByEmail(void)
{
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {    
        photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
        if (PNULL != photo_info_ptr)
        {
            full_path_ptr = photo_info_ptr->file_info.file_name;
            full_path_len = photo_info_ptr->file_info.file_name_len;
            //MMIAPIFMM_GetFileInfoFormFullPath(full_path_ptr,full_path_len,&file_info);
        }
    }
    else
    {
        full_path_ptr = MMIDC_SAVE_DATA_PTR->video_full_path_arr;
        full_path_len = MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr);
        //MMIAPIFMM_GetFileInfoFormFullPath(MMIDC_SAVE_DATA_PTR->video_full_path_arr,full_path_len,&file_info);
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_SendByEmail file_name_len=%d", file_info.file_name_len);
    
    //MMIAPIMAIL_EditMailFromFile(full_path_ptr, full_path_len);
}
#endif //PUSH_EMAIL_SUPPORT


#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC void  HandleSNSRelease(void)
{
    if(pMe)
    {   
        IMMISNS_Release(pMe);
        pMe         = PNULL;
    }
}

/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
MMI_RESULT_E  HandleDcWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    widget_id = pubwin_info_ptr->user_data;
    
    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                IMMISNS_AppInstall(pMe, widget_id);
                
                IMMISNS_Release(pMe);
                pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : share to sns
//	Global resource dependence : none
//  Author: weiren
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SharetoSNS(uint32 msg_id)
{
    uint16 full_path_len = 0;
    uint32 file_size = 0;
    wchar full_path_arr_photo[MMIDC_FULL_PATH_LENGTH+1] = {0};
    wchar* full_path_arr = full_path_arr_photo;
    SFS_DATE_T create_date = {0};
    SFS_TIME_T create_time = {0};
    //MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
    MMI_STRING_T dc_file_info = {0};
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;    
    int sns_id = 0;
    
    if(ID_CAMERA_SHARE_TO_SINA == msg_id)
    {
        sns_id = MMI_SNS_SINA;
    }
    else if(ID_CAMERA_SHARE_TO_FACEBOOK == msg_id)
    {
        sns_id = MMI_SNS_FACEBOOK;
    }
    else
    {
        sns_id = MMI_SNS_TIWTTER;
    }

	
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {        
        if (PNULL == MMIDC_GetPhotoSavePath())
        {
            //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SendByBt Path = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_239_112_2_18_2_9_40_531,(uint8*)"");
            return ;
        }
        
        photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
        if (PNULL != photo_info_ptr)
        {
            full_path_arr = photo_info_ptr->file_info.file_name;
            full_path_len = photo_info_ptr->file_info.file_name_len;
        }        
    }
    else
    {
        full_path_arr = MMIDC_SAVE_DATA_PTR->video_full_path_arr;
        full_path_len = MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr);
    }
    MMIFILE_GetFileInfo(full_path_arr, full_path_len, &file_size, &create_date, &create_time);      

   
    //MMIAPICOM_Wstrncpy((wchar*)send_file_info.filepath_name,full_path_arr,full_path_len); 	

    IMMISNS_CreateObject(&pMe);
    
    if(pMe)
    {
        if(IMMISNS_IsFixAppInstalled(pMe, sns_id) < 0)
        {
            IMMISNS_Release(pMe);
            pMe = PNULL;
            
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_WHETHER_INSTALL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleDcWhetherInstallWinMsg, sns_id);
        }
        else
        {
            uint32        time_num    = 500;
            dc_file_info.wstr_ptr = (wchar*)SCI_ALLOC_APP(sizeof(wchar) *(full_path_len+1));

            if(PNULL == dc_file_info.wstr_ptr)
            {
                return ;
            }

            SCI_MEMSET(dc_file_info.wstr_ptr, 0, sizeof(wchar) *(full_path_len+1));

            MMIAPICOM_Wstrncpy((wchar*)dc_file_info.wstr_ptr, full_path_arr, full_path_len); 	
            dc_file_info.wstr_len = full_path_len;
            
            IMMISNS_ShareToSns(pMe, sns_id, SNS_MODULE_CAMERA, (uint32)&dc_file_info);

            SCI_FREE(dc_file_info.wstr_ptr);
            dc_file_info.wstr_ptr = PNULL;
            MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        }
    }
}
#endif
/*****************************************************************************/
// 	Description : send by blue tooth
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SendByBt(void)
{
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar   *full_path_ptr = PNULL;
    uint16 full_path_len = 0;
    uint32 file_size = 0;

    SFS_DATE_T create_date = {0};
    SFS_TIME_T create_time = {0};
#ifdef BLUETOOTH_SUPPORT	
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
#endif	
	
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {        
        photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
        if (PNULL != photo_info_ptr)
        {
            full_path_ptr = photo_info_ptr->file_info.file_name;
            full_path_len = photo_info_ptr->file_info.file_name_len;
        }        
    }
    else
    {
        full_path_ptr = MMIDC_SAVE_DATA_PTR->video_full_path_arr;
        full_path_len = MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr);
    }
    MMIAPIFMM_GetFileInfo(full_path_ptr, full_path_len, &file_size, &create_date, &create_time);  

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SendByBt file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_330_112_2_18_2_9_40_532,(uint8*)"d",file_size);
				  
#ifdef BLUETOOTH_SUPPORT   
    MMIAPICOM_Wstrncpy((wchar*)send_file_info.filepath_name,full_path_ptr,full_path_len);
    send_file_info.filepath_len = full_path_len;
    send_file_info.file_size = file_size;
    send_file_info.is_temp_file = FALSE;   	
	
   MMIAPIBT_SendMultiFile(&send_file_info,1);
#endif
}


/*****************************************************************************/
// 	Description : set current photo as wallpaper
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCurrentPhotoAsWallpaper(void)
{
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar   *full_path_ptr = PNULL;
    uint16 full_path_len = 0;

    BOOLEAN is_success = FALSE;

    photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
    if (PNULL != photo_info_ptr)
    {
        full_path_ptr = photo_info_ptr->file_info.file_name;
        full_path_len = photo_info_ptr->file_info.file_name_len;
 

        if (full_path_len > 0)
        {
#if defined MMI_WALLPAPER_SUPPORT
            is_success = MMIAPIFMM_SetPictureToWallpaperEx(FALSE, full_path_ptr, full_path_len);
#endif
        }
    }

    if (!is_success)
    {
        MMIDC_ShowMsg(MMIDC_ERR_FAIL);
    }
    
    //SCI_TRACE_LOW:"MMIDC_SetCurrentPhotoAsWallpaper is_success =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_375_112_2_18_2_9_40_533,(uint8*)"d",is_success);
}

/*****************************************************************************/
//  Description : check room is enough
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  dev    file  device by FILE_DEV_E_T struct
//        Input:  size    space is or no smaller  the size
//       Return:  return value as MMIFILE_ERROR_E  
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E  CheckDevRoomSufficient(MMIFILE_DEVICE_E file_dev, uint32 need_size)
{
    MMIFILE_ERROR_E ret = SFS_ERROR_NONE;
    wchar  save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    BOOLEAN is_folder_exist = FALSE;

    //先检查文件夹是否存在,不存在则创建
    //若是创建文件夹出错，返回错误，以便使用其他磁盘
    //目前总是使用默认文件夹
    MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,file_dev,MMIDC_GetPhotoDefaultPathName());

    
    is_folder_exist = MMIAPIFMM_IsFolderExist((const wchar*)save_path,\
                                            MMIAPICOM_Wstrlen((const wchar*)save_path)
                                            );
    if(!is_folder_exist)
    {
        ret = SFS_CreateDirectory((const uint16 *)save_path);
    }

    //SCI_TRACE_LOW:"[MMIDC] CheckDevRoomSufficient ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_406_112_2_18_2_9_40_534,(uint8*)"d",ret);

   return ret;
}

/*****************************************************************************/
//  Description : create capture photo name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E RealCreateOnePicName(int32 photo_index,
                                                   MMIFILE_DEVICE_E file_dev,
                                                   uint32 photo_size
                                                  )
{
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    MMIFILE_ERROR_E ret = SFS_ERROR_NONE;
    wchar *temp_file_name_ptr;
    MMIDC_FILE_INFO_T file_info = {0};
    SFS_HANDLE sfs_handle = 0;

    //SCI_TRACE_LOW:"[MMIDC]: RealCreateOnePicName photo_index=%d, file_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_426_112_2_18_2_9_40_535,(uint8*)"dd",photo_index, file_dev);

    MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,file_dev,MMIDC_GetPhotoDefaultPathName());

    do
    {
        //get image file name
        temp_file_name_ptr = MMIDC_GetImageFileName(photo_index);
        if (PNULL == temp_file_name_ptr)
        {
            ret = SFS_ERROR_SYSTEM;
            break;
        }
        file_info.file_dev = file_dev;
        file_info.file_size = photo_size;
        file_info.file_name_len = MMIAPIDC_CombinePathName(file_info.file_name, MMIFILE_FULL_PATH_MAX_LEN, save_path, temp_file_name_ptr);


        
        //若SFS_OVERLAPPED_T *overlapped_ptr 为PNULL，就是同步创建文件
        ret = SFS_CreateFileAsyn(save_path, temp_file_name_ptr, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL, &sfs_handle, PNULL);/*lint !e655*/
        //ret = SFS_CreateFileAsyn(save_path, name_info_ptr->ucs2_name, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL, &(MMIDC_SAVE_DATA_PTR->sfs_handle), PNULL);/*lint !e655*/
        //SCI_TRACE_LOW:"[MMIDC] RealCreateOnePicName create file ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_447_112_2_18_2_9_40_536,(uint8*)"d",ret);
        //MMIDC_SAVE_DATA_PTR->sfs_handle = MMIAPIFMM_CreateFile(full_path_arr, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL);
        if (SFS_ERROR_NONE == ret)
        {
            if(0 != sfs_handle)
            {
                file_info.file_handle = sfs_handle;
                MMIDC_SetImageFileInfo(photo_index, &file_info);
                break;
            }
            else
            {
                ret = SFS_ERROR_SYSTEM;
            }
        }       
    }while(0);
    
    return ret;    
}

/*****************************************************************************/
// 	Description : check the udisk and sd card before create capture photo name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//  Param
//          Input: photo_index   photo order index
//         Return: MMIFILE_ERROR_E
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateOnePicName(int32 photo_index, uint32 photo_size)
{	
    MMIFILE_DEVICE_E cur_file_dev = MMIDC_GetPrefPhotoStorageDevice();			
    MMIFILE_ERROR_E  ret = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E suitable_dev = 0;
    wchar *save_path = MMIDC_GetPhotoSavePath();
    int8 count_dev_num = 0;
    MMIFILE_DEVICE_E old_file_dev = 0;
    BOOLEAN is_folder_full[MMI_DEVICE_NUM] = {0};
    MMIFILE_DEVICE_E i = 0;

    //SCI_TRACE_LOW:"[MMIDC] CreateOnePicName %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_486_112_2_18_2_9_41_537,(uint8*)"d", photo_index);

    if (PNULL == save_path)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] CreateOnePicName save_path = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_495_112_2_18_2_9_41_539,(uint8*)"");
        ret = SFS_ERROR_SYSTEM;
        return ret;
    }
    
    old_file_dev = cur_file_dev;
    //find next existed device
    do
    {   
        //if user_sel_dev is not enough space
        //then change to another device by max space
#ifndef CMCC_UI_STYLE
        ret = MMIAPIFMM_GetSuitableFileDev(cur_file_dev,
                                               photo_size, 
                                               &suitable_dev
                                               );
        //SCI_TRACE_LOW:"[MMIDC] CreateOnePicName ret=%d, user_sel_dev=%d,suitable_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_514_112_2_18_2_9_41_540,(uint8*)"ddd",ret, cur_file_dev, suitable_dev);
#else
        //CMCC入库测试，指定存储器空间不足则提示，不再自动查找设备
        if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, cur_file_dev, photo_size, PNULL))
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(cur_file_dev))
            {
                ret = SFS_ERROR_NO_SPACE;
            }
            else
            {
                ret = SFS_ERROR_NOT_EXIST;
            }
        }

        if(MMI_DEVICE_UDISK == cur_file_dev)
        {
            //NEWMS00220449
            if(5 >= MMIAPIFMM_GetFreeSpaceInPercent(MMIAPIFMM_GetDevicePath(cur_file_dev)))
            {
                ret = SFS_ERROR_NO_SPACE;
            }
        }
#endif
        if (SFS_ERROR_NONE != ret)
        {
            break;
        }
        
#ifndef CMCC_UI_STYLE
        cur_file_dev = suitable_dev;
#endif
        
        /** set new file dev **/
        MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,cur_file_dev,MMIDC_GetPhotoDefaultPathName());

        
        MMIDC_SetPhotoStorageDevice(cur_file_dev);
        
        //delete the previous code as check dev in the following fun ever.
        ret = CheckDevRoomSufficient(cur_file_dev, photo_size);    
        if (SFS_ERROR_FOLD_FULL != ret )
        {
            break;
        }
        is_folder_full[cur_file_dev] = TRUE;
        
        cur_file_dev++;

        if (MMI_DEVICE_NUM <= cur_file_dev)
        {
            cur_file_dev = MMI_DEVICE_UDISK;
        }

        count_dev_num++;
        
    }while(count_dev_num < (MMI_DEVICE_NUM - 1));

    
    if (SFS_ERROR_NONE == ret)
    {
        /** create the photo name **/	
        ret = RealCreateOnePicName(photo_index, cur_file_dev, photo_size);
        if (SFS_ERROR_NONE == ret)
        {
            //SCI_TRACE_LOW:"[MMIDC]: CreateOnePicName success"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_554_112_2_18_2_9_41_541,(uint8*)"");
            
            /** set new file dev **/
            MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,cur_file_dev,MMIDC_GetPhotoDefaultPathName());
            
            MMIDC_SetPhotoStorageDevice(cur_file_dev);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDC]: CreateOnePicName fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_563_112_2_18_2_9_41_542,(uint8*)"");
        }
    }
    else
    {
        if ((SFS_ERROR_FOLD_FULL == ret)
            && ((MMI_DEVICE_NUM - 1) == count_dev_num)
            )
        {
            for (i = (MMI_DEVICE_NUM - 1)  ; i >= MMI_DEVICE_UDISK ; i--)
            {
                if (!is_folder_full[i])
                {
                    break;
                }
            }
            
            if (MMI_DEVICE_NUM == i)
            {
                MMIDC_SetPhotoStorageDevice(old_file_dev);

                MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,old_file_dev,MMIDC_GetPhotoDefaultPathName());
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC]: CreateOnePicName i=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_590_112_2_18_2_9_41_543,(uint8*)"d",i);
    
    //SCI_TRACE_LOW:"[MMIDC]: CreateOnePicName count_dev_num=%d, old_file_dev=%d,cur_file_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_593_112_2_18_2_9_41_544,(uint8*)"ddd",count_dev_num, old_file_dev, cur_file_dev);
    
    //SCI_TRACE_LOW:"[MMIDC]: CreateOnePicName ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_595_112_2_18_2_9_41_545,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : write photo buffer to the file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E WriteToFile(uint8 * const photo_ptr, uint32 photo_size)
{
    uint32 write_size = 0;
    MMIFILE_ERROR_E  file_ret = SFS_ERROR_WRITE_CONFLICT;
    MMIFILE_ERROR_E  write_ret = 0;
    uint32 max_file_size = 0;

    //modified, @robert.wang, 09-9-22, cr152395
    //增加判断是否写入成功
    write_ret = MMIAPIFMM_WriteFile(MMIDC_SAVE_DATA_PTR->sfs_handle, photo_ptr, photo_size, &write_size, PNULL);
    
    if( (SFS_NO_ERROR == write_ret) && (photo_size == write_size) )
    {
        //SCI_TRACE_LOW:"[MMIDC]: WriteToFile success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_616_112_2_18_2_9_41_546,(uint8*)"");
        file_ret = SFS_NO_ERROR;

        max_file_size = MMIDC_GetMaxImageFileSize();
        max_file_size = MAX(max_file_size, photo_size);
        MMIDC_SetMaxImageFileSize(max_file_size);
    }
	else
    {
	if(write_ret != SFS_NO_ERROR)
	{
	  file_ret = write_ret;
	}
    }

    //SCI_TRACE_LOW:"[MMIDC]: WriteToFile file_ret=%d, write_ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_625_112_2_18_2_9_41_547,(uint8*)"dd",file_ret, write_ret);
    
    return file_ret;
}

/*****************************************************************************/
// 	Description : save captured photos
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_SavePhotos(int32 photo_id)
{
    MMIFILE_ERROR_E ret = SFS_ERROR_NONE;
    
    ret = MMIDC_RenameImageFileToPhotoFile(photo_id);

    //SCI_TRACE_LOW:"[MMIDC]MMIDC_SavePhotos ret=%d, photo_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_641_112_2_18_2_9_41_548,(uint8*)"dd",ret, photo_id);
    
    return ret;
}

/*****************************************************************************/
// 	Description : get record video subfix
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL char* GetVideoSubfix(void)
{	
    char* ret = PNULL;
    
    switch(MMIDC_GetRecordFileType())
    {
    case VIDEO_FILE_TYPE_MP4:
        //SCI_TRACE_LOW:"[MMIDC]: GetVideoSubfix DV_VIDEOEXT_MP4 -"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_657_112_2_18_2_9_41_549,(uint8*)"");
        ret = (char*)DV_VIDEOEXT_MP4;
        break;

    case VIDEO_FILE_TYPE_3GP:
        //SCI_TRACE_LOW:"[MMIDC]: GetVideoSubfix DV_VIDEOEXT_3GP -"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_662_112_2_18_2_9_41_550,(uint8*)"");
        ret = (char*)DV_VIDEOEXT_3GP;
        break;
        
    case VIDEO_FILE_TYPE_AVI:
        //SCI_TRACE_LOW:"[MMIDC]: GetVideoSubfix VIDEO_FILE_TYPE_MJPEG -"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_667_112_2_18_2_9_41_551,(uint8*)"");
        ret = (char*)DV_VIDEOEXT_AVI;
        break;
        
    default:
        ret = (char*)DV_VIDEOEXT_AVI;
        //MMIDC_ASSERT_WARNING:"[MMIDC] GetVideoSubfix TYPE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_673_112_2_18_2_9_41_552,(uint8*)"");
        break;
    }	
    return ret;
}

/*****************************************************************************/
//Description : check record video save path
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E  CheckVideoSavePath(MMIFILE_DEVICE_E file_dev)
{
    MMIFILE_ERROR_E ret = SFS_ERROR_NONE;
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    BOOLEAN is_folder_exist = FALSE;
    
    //解决CR MS00149927
    //如果指定的路径不存在，就将录像的默认地址设置到

    MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,file_dev,MMIMULTIM_DIR_MOVIE);

    is_folder_exist = MMIAPIFMM_IsFolderExist((const wchar*)save_path,\
                                           MMIAPICOM_Wstrlen((const wchar*)save_path)
                                           );
    if (!is_folder_exist)
    {
        ret = SFS_CreateDirectory((const uint16 *)save_path);
    }

    //SCI_TRACE_LOW:"[MMIDC] CheckVideoSavePath is_folder_exist=%d, ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_703_112_2_18_2_9_41_553,(uint8*)"dd",is_folder_exist, ret);
    
    return ret;
}
/*****************************************************************************/
// 	Description : create record video name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateVideoName(wchar* ucs2_name_ptr, uint16 ucs2_size)
{	
    uint16 full_path_len = 0;
    register uint32 step = 1;
    uint32 circle = 1;
    register uint32 name_id = 0;
    register uint32 id_max = DC_FILE_NAME_ID_MAX;
    char temp_video_name[MMIDC_NAME_MAX_LEN+1] = {0};
    char* subfix = GetVideoSubfix();
    wchar *save_path = MMIDC_GetVideoSavePath();

    MMIFILE_DEVICE_E user_sel_dev = 0;
    MMIFILE_DEVICE_E suitable_dev = 0;
    uint32 min_free_space = MMI_DC_MIN_SPACE_SIZE;
    MMIFILE_ERROR_E error_ret = SFS_ERROR_NO_SPACE;

    int8 count_dev_num = 0;
    MMIFILE_DEVICE_E old_sel_dev = 0;
    BOOLEAN is_folder_full[MMI_DEVICE_NUM] = {0};
    MMIFILE_DEVICE_E i = 0;
    uint32 rand_num = 0;
    
    if (PNULL == subfix)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] CreateVideoName subfix = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_735_112_2_18_2_9_41_554,(uint8*)"");
        return error_ret;
    }
    if (PNULL == save_path)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] CreateVideoName path = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_740_112_2_18_2_9_41_555,(uint8*)"");
        return error_ret;
    }

    //SCI_TRACE_LOW:"[MMIDC]: CreateVideoName"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_744_112_2_18_2_9_41_556,(uint8*)"");
    if (PNULL == ucs2_name_ptr)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] CreateVideoName ucs2_name = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_747_112_2_18_2_9_41_557,(uint8*)"");
        return error_ret;
    }


    //解决CR MS00149927 end
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
    if (!MMISD_IsSDExistEx())
    {
        //SCI_TRACE_LOW:"[MMIDC] UDISK MEM NO DISK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_756_112_2_18_2_9_41_558,(uint8*)"");
        return error_ret;
    }
#endif

    user_sel_dev = MMIDC_GetPrefVideoStorageDevice();
    old_sel_dev = user_sel_dev;
    do
    {
        //if user_sel_dev is not enough space
        //then change to another device by max space
#ifndef CMCC_UI_STYLE
        error_ret = MMIAPIFMM_GetSuitableFileDev(user_sel_dev,
                                               min_free_space, 
                                               &suitable_dev
                                               );
        //SCI_TRACE_LOW:"[MMIDC] CreateVideoName DV error_ret=%d, user_sel_dev=%d,suitable_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_772_112_2_18_2_9_41_559,(uint8*)"ddd",error_ret, user_sel_dev, suitable_dev);
#else
        //CMCC入库测试，指定存储器空间不足则提示，不再自动查找设备
        if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, user_sel_dev, min_free_space, PNULL))
        {
            error_ret = SFS_ERROR_NONE;
        }
        else
        {
            error_ret = SFS_ERROR_NO_SPACE;
        }
        
        if(MMI_DEVICE_UDISK == user_sel_dev)
        {
            //NEWMS00220449
            if(5 >= MMIAPIFMM_GetFreeSpaceInPercent(MMIAPIFMM_GetDevicePath(user_sel_dev)))
            {
                error_ret = SFS_ERROR_NO_SPACE;
            }
        }
#endif
        
        if (SFS_ERROR_NONE != error_ret)
        {
            break;
        }

        #ifndef CMCC_UI_STYLE
        user_sel_dev = suitable_dev;
        #endif

        #if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
            if (MMI_DEVICE_UDISK == user_sel_dev)
            {
                user_sel_dev++;
                count_dev_num++;
                continue;
            }
        #endif
        
        MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,user_sel_dev,MMIMULTIM_DIR_MOVIE);
        MMIDC_SetVideoStorageDevice(user_sel_dev);

        error_ret = CheckVideoSavePath(user_sel_dev);
        if (SFS_ERROR_FOLD_FULL != error_ret)
        {
            break;
        }
        
        is_folder_full[user_sel_dev] = TRUE;
        user_sel_dev++;        
        if (MMI_DEVICE_NUM <= user_sel_dev)
        {
            user_sel_dev = MMI_DEVICE_UDISK;
        }

        count_dev_num++;
        
    }while(count_dev_num < (MMI_DEVICE_NUM - 1));


    if (SFS_ERROR_NONE == error_ret)
    {
        MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,user_sel_dev,MMIMULTIM_DIR_MOVIE);
        MMIDC_SetVideoStorageDevice(user_sel_dev);
        
        name_id = MMIDC_GetVideoNameID();    
        while(1) /*lint !e716*/
        {	
			if(circle > MMIDC_NAME_CIRCLE_TIMES_LIMIT)
			{
				rand_num = GetRand(0,10);
				name_id = name_id * rand_num;
				MMIDC_SetVideoNameID(name_id);
				MMIDC_SetRenameFileCircleFlag(TRUE);
			
				MMIDC_PostRenameFileHandleMsg();
				return SFS_ERROR_ACCESS;  //暂未创建成功,出去喂狗后 继续创建
			}
            name_id += step;
            step = step * 2;
            if(name_id >= id_max)
            {
                circle ++;
//                if(circle >= id_max)
//               {
//                    circle = 1;
//                    id_max = id_max * 10;
//                }
				if(!MMIDC_GetRenameFileCircleFlag())
				{
					name_id = circle;
				}
				else
				{
					name_id = MMIDC_GetVideoNameID();
					name_id = name_id + circle;
				}
                step = 1;
            }
            MMIAPIDC_GetFileIDString(temp_video_name, "MOV", name_id, id_max, subfix);
            MMIAPICOM_StrToWstr(temp_video_name, ucs2_name_ptr);/*lint !e64*/
            
            full_path_len = MMIAPIDC_CombinePathName(MMIDC_SAVE_DATA_PTR->video_full_path_arr, MMIDC_FULL_PATH_LENGTH, save_path, ucs2_name_ptr);
            
            // check the exist of the file name.
            if(!MMIAPIFMM_IsFileExist(MMIDC_SAVE_DATA_PTR->video_full_path_arr, full_path_len))
            {
                break;
            }
        }
        MMIDC_SetTempFileNameID(name_id);

    }
    else
    {
        if ((SFS_ERROR_FOLD_FULL == error_ret)
            && ((MMI_DEVICE_NUM - 1) == count_dev_num)
           )
        {
            i = MMI_DEVICE_UDISK;
            #if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
                i++;
            #endif
            
            while(i < MMI_DEVICE_NUM)
            {
                if (!is_folder_full[i])
                {
                    break;
                }
                i++;
            }
            
            if (MMI_DEVICE_NUM == i)
            {
                MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,old_sel_dev,MMIMULTIM_DIR_MOVIE);
                MMIDC_SetVideoStorageDevice(old_sel_dev);
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateVideoName user_sel_dev=%d, old_sel_dev=%d, i =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_873_112_2_18_2_9_41_560,(uint8*)"ddd",user_sel_dev, old_sel_dev, i);

    //SCI_TRACE_LOW:"[MMIDC] CreateVideoName error_ret=%d, count_dev_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_876_112_2_18_2_9_41_561,(uint8*)"dd",error_ret, count_dev_num);
    
    return error_ret;
}

/*****************************************************************************/
// 	Description : review video
//	Global resource dependence :
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReviewVideo(void)
{    
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMIAPIVP_MiniFunction_PlayVideo(MMIDC_SAVE_DATA_PTR->video_full_path_arr,MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr));
#else
    MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,MMIDC_SAVE_DATA_PTR->video_full_path_arr,MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr),PNULL);
#endif
#endif
}

/*****************************************************************************/
// 	Description : create recording video name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateVideoName(DRECORDER_START_RECORD_T* recorder_start_t)
{
    MMIDC_SAVE_DATA *save_data_ptr = MMIDC_SAVE_DATA_PTR;
    MMIFILE_ERROR_E ret = 0;
    MMIFILE_DEVICE_E  file_dev = 0;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_CreateVideoName +"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_902_112_2_18_2_9_41_562,(uint8*)"");

    SCI_MEMSET(save_data_ptr->record_file_name_arr, 0x00, sizeof(save_data_ptr->record_file_name_arr));
    ret = CreateVideoName(MMIDC_SAVE_DATA_PTR->record_file_name_arr, MMIDC_NAME_MAX_LEN);
    if (SFS_ERROR_NONE != ret)
    {
        //SCI_TRACE_LOW:"[MMIDC]: MMIDC_CreateVideoName - create dv name fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_908_112_2_18_2_9_41_563,(uint8*)"");
		if(!MMIDC_GetRenameFileCircleFlag())
		{
#ifndef CMCC_UI_STYLE
        	file_dev = MMIDC_GetVideoStorageDevice();
#else
		file_dev = MMIDC_GetPrefVideoStorageDevice();
#endif
        	MMIDC_ShowErrorMsg(ret, file_dev);
        	return FALSE;
		}
		else
		{
        	return FALSE;  //由于创建文件名时间太长导致的返回失败,暂不提示,还需要去继续命名
		}
    }    
    recorder_start_t->file_name_ptr		 = (uint8*)MMIDC_SAVE_DATA_PTR->video_full_path_arr;
    recorder_start_t->file_name_byte_len = MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr) * 2;
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_CreateVideoName -"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_916_112_2_18_2_9_41_564,(uint8*)"");
    return TRUE;
}

/*****************************************************************************/
// 	Description : delete the recorded file
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteNoSavedFile(void)
{	
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_DeleteNoSavedFile +-"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_926_112_2_18_2_9_42_565,(uint8*)"");

/*
#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        MMIDC_SetWorkError(MMIDC_WORK_ERROR);
    }
#endif
*/

    if (MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr) > 0)//yuanl add 2009.9.4 for cr150786 做一下保护
    {
        return MMIAPIFMM_DeleteFileSyn(MMIDC_SAVE_DATA_PTR->video_full_path_arr, (uint16)MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->video_full_path_arr));
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : delete current photo
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeletePhoto(void)
{
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar   *full_path_ptr = PNULL;
    uint16 full_path_len = 0;
    
    MMIDC_ERROR_TYPE_E   err_ret = MMIDC_ERR_DELETE_FAIL;

    photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
    if (PNULL != photo_info_ptr)
    {
        full_path_ptr = photo_info_ptr->file_info.file_name;
        full_path_len = photo_info_ptr->file_info.file_name_len;

/*
#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        MMIDC_SetWorkError(MMIDC_WORK_ERROR);
    }
#endif    
*/   
    
        if(full_path_len > 0)
        {
            if(MMIAPIFMM_DeleteFileSyn(full_path_ptr, full_path_len))
            {
                /** check whether this is wall paper **/
#if defined MMI_WALLPAPER_SUPPORT
                if(MMIAPIFMM_IsWallPaperFile(full_path_ptr, full_path_len)) //全路径名
                {
                    MMIAPIFMM_InitWallPaper();
                }
#endif
                err_ret = MMIDC_ERR_DELETE_SUCCESS;
            }
         }
     }
    if(err_ret != MMIDC_ERR_DELETE_SUCCESS)
    {
        MMIDC_ShowMsg(err_ret);
    }
}

/*****************************************************************************/
// 	Description : get current photo name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetPhotoName(uint32 id)
{
    return MMIDC_SAVE_DATA_PTR->photo_info[id].ucs2_name;
}

/*****************************************************************************/
// 	Description : get current video name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetVideoName(void)
{
    return MMIDC_SAVE_DATA_PTR->record_file_name_arr;
}

/*****************************************************************************/
//  Description : combine path according to root, dir, name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_CombinePath(wchar* path_ptr, uint16 path_len, MMIFILE_DEVICE_E file_dev, const wchar* dir_ptr)
{
	MMIAPIFMM_CombineFullPathEx(path_ptr, path_len,file_dev,dir_ptr, PNULL);
    return (uint16)MMIAPICOM_Wstrlen(path_ptr);
}

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPictureWin(void)
{
    if (PNULL == MMIDC_GetPhotoSavePath())
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenPictureWin path = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1024_112_2_18_2_9_42_566,(uint8*)"");
        return ;
    }
    
    MMIAPIDC_SetChangeFolderState(FALSE);

    //for black screen  flush
    MMITHEME_InitMainLcdStortUpdateRect();
#ifdef DC_WATCH_UI_SUPPORT
#ifdef PIC_VIEWER_SUPPORT
	WatchGallery_MainWin_Enter();
#endif
#else
#ifdef PIC_VIEWER_SUPPORT
	MMIAPIPICVIEW_OpenMyPhotos(); 
#else
    MMIAPIFMM_OpenExplorerExt(MMIDC_GetPhotoSavePath(), 
        (uint16)MMIAPICOM_Wstrlen(MMIDC_GetPhotoSavePath()), MMIDC_SAVE_DATA_PTR->photo_info[MMIDC_GetCurrentPhotoID()].ucs2_name, 
        (uint16)MMIAPICOM_Wstrlen(MMIDC_SAVE_DATA_PTR->photo_info[MMIDC_GetCurrentPhotoID()].ucs2_name), FALSE, MMIFMM_PICTURE_JPG);
#endif
#endif
}

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenMovieWin(void)
{
    if (PNULL == MMIDC_GetVideoSavePath())
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenMovieWin path = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1048_112_2_18_2_9_42_567,(uint8*)"");
        return ;
    }
    if (PNULL == MMIDC_GetVideoName())
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenMovieWin name = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1053_112_2_18_2_9_42_568,(uint8*)"");
        return ;
    }

    MMITHEME_InitMainLcdStortUpdateRect();//added, @robert.wang, 09-10-5, cr154947
                                                     //for black screen  flush

    MMIAPIFMM_OpenExplorerExt(MMIDC_GetVideoSavePath(), 
        (uint16)MMIAPICOM_Wstrlen(MMIDC_GetVideoSavePath()), MMIDC_GetVideoName(),
        (uint16)MMIAPICOM_Wstrlen(MMIDC_GetVideoName()), FALSE, MMIFMM_FILE_TYPE(MMIFMM_MOVIE_ALL) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_MP4) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_AVI) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_3GP));
}

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIDC_GetPicName(void)
{
    return MMIDC_SAVE_DATA_PTR->photo_info[MMIDC_GetCurrentPhotoID()].ucs2_name;
}

/*****************************************************************************/
// 	Description : get current save photo index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDC_GetSavePhotoIndex(
                                      uint32        pic_num,    //in
                                      FILEARRAY     pic_array   //in
                                      )
{
    uint16              file_name_len = 0;
    uint32              photo_index = 0;
    const wchar*        pic_name = MMIDC_GetPicName();
    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    FILEARRAY_DATA_T    file_info = {0};

    if (PNULL == pic_array)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIAPIDC_GetSavePhotoIndex array = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1091_112_2_18_2_9_42_569,(uint8*)"");
        return 0;
    }
        
    for (photo_index = 0; photo_index < pic_num; photo_index ++)
    {
        if(!MMIAPIFILEARRAY_Read(pic_array,photo_index, &file_info))
		{
			break;
		}
        SCI_MEMSET(file_name, 0, ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar)));
        MMIAPIFMM_SplitFullPath(file_info.filename,
            file_info.name_len,
            PNULL,
            PNULL,
            PNULL,
            PNULL,
            file_name,
            &file_name_len);
        if(0 == MMIAPICOM_CompareTwoWstr(file_name, file_name_len, pic_name, file_name_len))
        {
            break;
        }
    }

    if(photo_index == pic_num)
    {
        photo_index = 0;
    }

    return (photo_index);
}
/*****************************************************************************/
// 	Description : get current video file name
//	Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-27
//	Note:
/*****************************************************************************/
PUBLIC wchar * MMIDC_GetVideoFullFileName(void)
{
    return MMIDC_SAVE_DATA_PTR->video_full_path_arr;
}

/*****************************************************************************/
// 	Description : get  file size
//	Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-27
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetFileSize(wchar *filename)
{
        uint32 file_size = 0;
        uint32 opt_mode;
        MMIFILE_HANDLE file_handle;
  
	opt_mode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ; /*lint !e655*/
	file_handle = MMIAPIFMM_CreateFile((wchar *)filename, 
							opt_mode, 
							PNULL, 
							PNULL
							);
      if(PNULL != file_handle)
      {
           file_size = MMIAPIFMM_GetFileSize(file_handle);
           MMIAPIFMM_CloseFile(file_handle);
           file_handle = PNULL;
    }

    return file_size;
}

/*****************************************************************************/
// 	Description : get current photo information
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_STOREG_INFO_T * MMIDC_GetCurrentPhotoInfo(void)
{
    uint32   photo_index = MMIDC_GetCurrentPhotoID();
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    BOOLEAN is_enable = FALSE;

    photo_info_ptr = &(MMIDC_SAVE_DATA_PTR->photo_info[photo_index]);
    is_enable = photo_info_ptr->file_info.is_enable;
    if (!is_enable)
    {
        photo_info_ptr = PNULL;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetCurrentPhotoInfo is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1178_112_2_18_2_9_42_570,(uint8*)"d",is_enable);
    
    return photo_info_ptr;
}

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param 
/*****************************************************************************/
PUBLIC void  MMIDC_ShowMsg(MMIDC_ERROR_TYPE_E error)
{
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_UDISK;
//     BOOLEAN         ret = TRUE;
//     wchar           dev_char = 0;
    MMIFILE_ERROR_E fs_error = SFS_ERROR_SYSTEM;
    int16           msg_type = 1; // fs :0, alert:1 ,query:2, NO Msg: 3
    MMI_TEXT_ID_T   text1_id = TXT_NULL;

    //convert error type
    switch(error)
    {
    case MMIDC_SUCCESS:
        msg_type = 1;
        text1_id = TXT_SUCCESS;
        break;
		/*
    case MMIDC_ERR_DELETE_SUCCESS:
        msg_type = 1;
        text1_id = TXT_DELETED;
        image_id = IMAGE_PUBWIN_SUCCESS;				
        break;
		*/
    case MMIDC_ERR_DELETE_FAIL:
        msg_type = 1;
        text1_id = TXT_COMMON_DEL_FAIL;
        break;
    case MMIDC_ERR_FAIL:
        msg_type = 1;
        text1_id = TXT_ERROR;
        break;
    default:
        msg_type = 1;
        text1_id = TXT_USSD_REQUEST_UNCOMPLETED;
        break;
    }


    switch(msg_type)
    {
    case 1:
        MMIDC_ErrorTip(text1_id);
        break;

    default:
        if (CAMERA_MODE_DC == MMIDC_GetCameraMode())
        {
            file_dev = MMIDC_GetPhotoStorageDevice();
        }
        else
        {
            file_dev = MMIDC_GetVideoStorageDevice();
        }
        MMIDC_ShowErrorMsg(fs_error, file_dev);
        break;
    }
}

/*****************************************************************************/
//Description : set image file enable
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFileHandle(int32 index,
                                       SFS_HANDLE handle
                                       )
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        dc_info_ptr->file_list[index].file_handle = handle;
    }
}

/*****************************************************************************/
//  Description : close all file handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAllFileHandle(void)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    int32 max_photo_number = MMIDC_GetMaxBurstPhotoNumber();
    int32 i = 0;
    SFS_HANDLE handle = 0;

    if (PNULL != dc_info_ptr)
    {
        for (i = 0; i < max_photo_number; i++)
        {
            handle = dc_info_ptr->file_list[i].file_handle;
            if (0 != handle)
            {
                SFS_CloseFile(handle);
                dc_info_ptr->file_list[i].file_handle = 0;
            }
        }
    }
    
    SCI_Sleep(20);//sleep for file system
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_CloseAllFileHandle max_photo_number=%d",
          max_photo_number);
}

/*****************************************************************************/
//  Description : create all file handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateAllFileHandle(void)
{
    uint32 number = MMIDC_GetMulitShotNumber();
    uint32 i = 0;
    MMIDC_PHOTO_SIZE_E photo_size = MMIDC_GetPhotoSize();
    uint32 photo_bytes = 0;


    photo_bytes = MMIDC_GetJPEGSizeByPhotoSize(photo_size);

    MMIDC_DeleteAllSavedImageFile();
        
    MMIDC_PushFileHandleMutex();
    for (i = 0; i < number; i++)
    {
        CreateOnePicName(i, photo_bytes);
    }
    MMIDC_PopFileHandleMutex();

    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateAllFileHandle number=%d",
      number);
}

/*****************************************************************************/
//  Description : get file handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileHandle(int32 photo_id, //in
                                  SFS_HANDLE *handle_ptr,//out
                                  MMIFILE_ERROR_E *err_ret_ptr //out
                                  )
{
    BOOLEAN ret = FALSE;
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    SFS_HANDLE handle = 0;
    MMIFILE_ERROR_E file_ret = 0;
    uint32 photo_bytes = 0;


    if (PNULL != dc_info_ptr)
    {
        handle = dc_info_ptr->file_list[photo_id].file_handle;
        photo_bytes = dc_info_ptr->image_len;

        SCI_TRACE_LOW("[MMIDC] GetFileHandle handle=%d,photo_bytes=%d",\
                     handle, photo_bytes);

        if (0 == handle)
        {
            file_ret = CreateOnePicName(photo_id, photo_bytes);
            if (SFS_ERROR_NONE == file_ret)
            {
                handle = dc_info_ptr->file_list[photo_id].file_handle;
                ret = TRUE;
            }
        }
        else
        {
            ret = TRUE;
        }
        if(ret)
        {
            if (PNULL != handle_ptr)
            {
                *handle_ptr = handle;
                dc_info_ptr->file_list[photo_id].file_size = photo_bytes;
            }
        }
            if (PNULL != err_ret_ptr)
            {
                *err_ret_ptr = file_ret;
            }

    }


    SCI_TRACE_LOW("[MMIDC] GetFileHandle ret=%d, file_ret=%d",\
               ret, file_ret);

    return ret;
}

/*****************************************************************************/
//  Description : save captured image data to image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_SaveImageFile(int32 photo_id)
{
    MMIFILE_ERROR_E ret = SFS_ERROR_NONE;
    BOOLEAN create_ret = FALSE;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SaveImageFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1280_112_2_18_2_9_42_571,(uint8*)"");
    
    if(photo_id < 0)
    {
        return SFS_ERROR_SYSTEM;
    }

    MMIDC_PushFileHandleMutex();
    
    create_ret = GetFileHandle(photo_id,                                 
                                &MMIDC_SAVE_DATA_PTR->sfs_handle, 
                                &ret);//CreateOnePicName(photo_id);
    if (create_ret)
    {
        if (PNULL != MMIDC_GetDCInfo())
        {
            ret = WriteToFile(MMIDC_GetDCInfo()->image_addr, MMIDC_GetDCInfo()->image_len);
            if (SFS_ERROR_NONE == ret)
            {
                MMIDC_SetImageFileEnable(photo_id, TRUE);
            }
        }
        else
        {
                ret = SFS_ERROR_SYSTEM;
        }
    }
    else
    {
        if(ret == SFS_ERROR_NONE)
       {
           ret = SFS_ERROR_SYSTEM;
        }
    }
    
    if(0 != MMIDC_SAVE_DATA_PTR->sfs_handle)
    { 

        SFS_CloseFile(MMIDC_SAVE_DATA_PTR->sfs_handle);

        MMIDC_SAVE_DATA_PTR->sfs_handle = 0;

        MMIDC_SetFileHandle(photo_id, 0);
        
    }
    
    MMIDC_PopFileHandleMutex();
    
    if (create_ret && SFS_ERROR_NONE == ret)
    {
        MMIDC_SetImageFileHidden(photo_id, TRUE);
    }
    else
    {
        //若是没有成功，总是删除临时文件
        MMIDC_DeleteSavedImageFile(photo_id);
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SaveImageFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1320_112_2_18_2_9_42_572,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : set max photo file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMaxPhotoFileSize(uint32 file_size)
{
    if (PNULL != MMIDC_SAVE_DATA_PTR)
    {
        MMIDC_SAVE_DATA_PTR->max_file_size = file_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMaxPhotoFileSize file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1337_112_2_18_2_9_42_573,(uint8*)"d",file_size);
}

/*****************************************************************************/
//  Description : get max photo file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxPhotoFileSize(void)
{
    uint32 file_size = 0;
    
    if (PNULL != MMIDC_SAVE_DATA_PTR)
    {
        file_size = MMIDC_SAVE_DATA_PTR->max_file_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxPhotoFileSize file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1354_112_2_18_2_9_42_574,(uint8*)"d",file_size);

    return file_size;
}
/******************************************************************************/
//  Description : 获得随机数
//  Global resource dependence : none
//  Author:  jinju.ma
//  Note:  
/*****************************************************************************/
LOCAL uint32 GetRand(uint32 min,uint32 max)
{
    int32  temp = 0;
	uint32 tempnum = 0;

    if(min == max)
	{
        return min;
	}
    if(min > max)
    {
        tempnum = min;
        min = max;
        max = tempnum;
    } 
    temp = rand();
    if(temp < 0)
	{
        temp = temp * (-1);
	}

    temp = min + (temp%(max - min + 1));
    return temp;
}
/*****************************************************************************/
//  Description : generate capture photo name
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GeneratePhotoFileName(MMIFILE_DEVICE_E file_dev,
                                            PHOTO_STOREG_INFO_T* file_info_ptr, //out
                                            wchar *save_path_ptr,//in
                                            uint16 save_path_len //in
                                            )
{
    BOOLEAN ret = FALSE;
    wchar *full_path_ptr = PNULL;
    char temp_photo_name[MMIDC_NAME_MAX_LEN+1] = {0};
    uint16 full_path_len = 0;
    uint16 name_len = 0;
    register uint32 step = 1;
    uint32 circle = 1;
    register uint32 name_id = 0;
    register uint32 id_max = DC_FILE_NAME_ID_MAX;
    uint32 max_count = 1000;
    uint32 count = 0;
	uint32 temp = 0;

    //SCI_TRACE_LOW:"[MMIDC]: GeneratePhotoFileName %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1382_112_2_18_2_9_42_575,(uint8*)"d", file_dev);
    

    if (PNULL == file_info_ptr || PNULL == save_path_ptr)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] GeneratePhotoFileName name info = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1387_112_2_18_2_9_42_576,(uint8*)"");
        ret = SFS_ERROR_SYSTEM;
        return ret;
    }

    MMIDC_CombinePath(save_path_ptr, save_path_len, file_dev, MMIDC_GetPhotoDefaultPathName());

    name_id = MMIDC_GetPhotoNameID();
    
    while(count < max_count) /*lint !e716*/
    {
		if(circle > MMIDC_NAME_CIRCLE_TIMES_LIMIT)
		{
			temp = GetRand(0,10);
			name_id = name_id * temp;
			MMIDC_SetPhotoNameID(name_id);
			MMIDC_SetRenameFileCircleFlag(TRUE);
			
			MMIDC_PostRenameFileHandleMsg();
			return ret;
		}

        name_id += step;
#ifdef ZDT_PICLIST_SAVE_ONLY_NUM
                if(name_id > DC_SAVE_MAX_NUM)
                {
                    name_id = 1;
                }
#endif
        step = step * 2;
        if(name_id >= id_max)
        {
            circle ++;

//             if(circle >= id_max)
//             {
//                 circle = 1;
//                 id_max = id_max * 10;
//             }
			if(!MMIDC_GetRenameFileCircleFlag())
			{
				name_id = circle;
			}
			else
			{
				name_id = MMIDC_GetPhotoNameID();
				name_id = name_id + circle;
			}

            step = 1;
        }
        
        //get photo file name
        name_len = MMIAPIDC_GetFileIDString(temp_photo_name, "DSC", name_id, id_max, "jpg");
        MMI_STRNTOWSTR(file_info_ptr->ucs2_name, MMIDC_NAME_MAX_LEN,\
                        (const uint8*)temp_photo_name, MMIDC_NAME_MAX_LEN,\
                        name_len);

        full_path_ptr = file_info_ptr->file_info.file_name;
        full_path_len = MMIAPIDC_CombinePathName(full_path_ptr, MMIDC_FULL_PATH_LENGTH,\
                                           save_path_ptr, file_info_ptr->ucs2_name);
        file_info_ptr->file_info.file_name_len = full_path_len;

        
#ifdef ZDT_PICLIST_SAVE_ONLY_NUM
                if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
                {
                    MMIAPIFMM_DeleteFileSyn(full_path_ptr,full_path_len);
                }
                ret = TRUE;
                break;
#else
                if(!MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
                {
                    ret = TRUE;
                    break;
                }
#endif

        
        count++;
    }
    
    MMIDC_SetTempFileNameID(name_id);
    
    return ret;    
}

/*****************************************************************************/
//  Description : create directory with RO or HIDDEN attr
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAttr(
                               const wchar       *full_path_ptr,     //in
                               uint16            full_path_len,       //in, 双字节为单位
                               BOOLEAN       is_set_hidden,
                               BOOLEAN           is_hidden,         //in, 是否为隐藏属性 
                               BOOLEAN           is_ro,             //in, 是否为只读属性 
                               BOOLEAN           is_system,         //in, 是否为系统属性
                               BOOLEAN           is_arch           //in, 是否为存档属性
                               )
{
    wchar           *file_name_ptr = PNULL;
    wchar           *path_name_ptr = PNULL;
    uint16          file_name_len = full_path_len;
    wchar           attr[MMIFILE_FILE_ATTR_LEN] = {0};

    if(NULL == full_path_ptr || 0 == full_path_len)
    {
        return FALSE;
    }

    if(is_set_hidden || is_hidden || is_ro || is_system || is_arch)
    {
        file_name_ptr = (wchar *)SCI_ALLOCA((file_name_len+1) *sizeof(wchar));
        path_name_ptr = (wchar *)SCI_ALLOCA((full_path_len + 1)* sizeof(wchar));
        if(file_name_ptr == PNULL || path_name_ptr == PNULL)
        {
            //SCI_TRACE_LOW:"[MMIFILE] SetAttr NO memory !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1468_112_2_18_2_9_43_577,(uint8*)"");
            return FALSE;
        }
        SCI_MEMSET(file_name_ptr, 0x00, ((file_name_len + 1)*sizeof(wchar)));
        SCI_MEMSET(path_name_ptr, 0x00, ((full_path_len + 1)*sizeof(wchar)));
        MMIAPICOM_Wstrncpy(path_name_ptr, full_path_ptr, full_path_len);
        MMIAPIFMM_SplitLastName(path_name_ptr,&full_path_len,file_name_ptr,&file_name_len);
        MMIAPICOM_Wstrcpy(attr,L"/SA:");
        if(is_hidden)
        {
            MMIAPICOM_Wstrcat(attr,L"+H");
        }
        else
        {
            MMIAPICOM_Wstrcat(attr,L"-H");
        }
        if(is_ro)
        {
            MMIAPICOM_Wstrcat(attr,L"+R");
        }
        if(is_system)
        {
            MMIAPICOM_Wstrcat(attr,L"+S");
        }
        if(is_arch)
        {
            MMIAPICOM_Wstrcat(attr,L"+A");
        }
        SFS_SetAttrAsyn(path_name_ptr,file_name_ptr,attr,0);
        SCI_FREE(file_name_ptr);
        SCI_FREE(path_name_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
//Description : set photo file attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoFileHidden(int32 index,
                                    BOOLEAN is_hidden
                                    )
{
    PHOTO_STOREG_INFO_T* photo_file_info_ptr = &(MMIDC_SAVE_DATA_PTR->photo_info[index]);
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = 0;
    BOOLEAN is_enable = FALSE;
    BOOLEAN file_ret = FALSE;

    if (PNULL != photo_file_info_ptr) /*lint !e774*/
    {
        is_enable = photo_file_info_ptr->file_info.is_enable;
        if (is_enable)
        {
            full_path_ptr = photo_file_info_ptr->file_info.file_name;
            full_path_len = photo_file_info_ptr->file_info.file_name_len;
            file_ret = SetAttr(full_path_ptr, full_path_len, \
                                TRUE,is_hidden, FALSE, FALSE, FALSE);
        }
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetPhotoFileHidden file_ret=%d,is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1531_112_2_18_2_9_43_578,(uint8*)"dd",file_ret, is_enable);
}
/*****************************************************************************/
//  Description : get rename file status
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetRenameFileCircleFlag(void)
{
    return s_rename_file_status;
}
/*****************************************************************************/
//  Description : set rename file status
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRenameFileCircleFlag(BOOLEAN rename_status)
{
	s_rename_file_status = rename_status;
}
/*****************************************************************************/
//  Description : rename image file name to photo file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIDC_RenameImageFileToPhotoFile(int32 photo_index)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    MMIFILE_ERROR_E ret = SFS_ERROR_SYSTEM;
    BOOLEAN result = FALSE;

    PHOTO_STOREG_INFO_T* photo_file_info_ptr = PNULL;
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    MMIFILE_DEVICE_E file_dev = 0;

    BOOLEAN is_enable = FALSE;
    uint32 file_size = 0;
    wchar *full_path_ptr = PNULL;
    uint16 full_path_len = 0;
    wchar *new_file_name_ptr = PNULL;
    uint16 new_file_name_len = 0;

    uint32 max_file_size = 0;

    if (PNULL != dc_info_ptr)
    {
        is_enable = dc_info_ptr->file_list[photo_index].is_enable;
        if (is_enable)
        {
            file_dev = dc_info_ptr->file_list[photo_index].file_dev;
            full_path_ptr = dc_info_ptr->file_list[photo_index].file_name_ptr;
            full_path_len = dc_info_ptr->file_list[photo_index].file_name_len;

            photo_file_info_ptr = &(MMIDC_SAVE_DATA_PTR->photo_info[photo_index]);
            result = GeneratePhotoFileName(file_dev, photo_file_info_ptr,\
                                         save_path, MMIDC_FULL_PATH_LENGTH);
                            
			if(PNULL !=	full_path_ptr)
			{
				if (result)
				{
					new_file_name_ptr = photo_file_info_ptr->ucs2_name;
					new_file_name_len = MMIAPICOM_Wstrlen(new_file_name_ptr);

					result = MMIAPIFMM_RenameFile((const wchar *)full_path_ptr, full_path_len, \
						  new_file_name_ptr, new_file_name_len);
                
					if (result)
					{
						ret = SFS_ERROR_NONE;
                    
						photo_file_info_ptr->file_info.is_enable = TRUE;  
                    
						file_size = dc_info_ptr->file_list[photo_index].file_size;
						photo_file_info_ptr->file_info.file_size = file_size;
						photo_file_info_ptr->file_info.file_dev = file_dev;

						MMIDC_SetPhotoFileHidden(photo_index, FALSE);

						max_file_size = MMIDC_GetMaxPhotoFileSize();
						max_file_size = MAX(max_file_size, file_size);
						MMIDC_SetMaxPhotoFileSize(max_file_size);

						MMIDC_SetPhotoNameID(MMIDC_GetTempFileNameID());

						//SCI_TRACE_LOW:"[MMIDC]: MMIDC_RenamePhotoFile photo_index=%d"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1593_112_2_18_2_9_43_579,(uint8*)"d",photo_index);

                    
						#ifdef WRE_SUPPORT      
						{
						extern void wre_StoreLastPhotePath(wchar* path,wchar* name);
									wre_StoreLastPhotePath(save_path,photo_file_info_ptr->ucs2_name);
						}
						#endif
#ifdef BAIDU_DRIVE_SUPPORT
						if(BAIDU_DRV_AUTO_SYNC == MMISRV_BAIDUDRV_Get_Sync_State()
                                                    &&MMISRV_BAIDUDRV_Get_Connect_State()
                                                    &&MMIAPIBAIDU_IsAccountExist())
						{
							MMISRV_BAIDUDRV_append_upload_file(photo_file_info_ptr->file_info.file_name , photo_file_info_ptr->file_info.file_name_len);
						}
#endif
					}
				}
            }        
        }
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_RenamePhotoFile ret=%d,result=%d,is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1608_112_2_18_2_9_43_580,(uint8*)"ddd",ret, result, is_enable);
    
    return ret;
}

/*****************************************************************************/
//Description : get image file name
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetImageFileName(int32 index)
{
    if ((index < 0)
       || (index >= MAX_DC_BURST_PHOTO_NUMBER)
       )
    {
        return PNULL;
    }
    
    return (wchar*)s_temp_file[index];
}

/*****************************************************************************/
//Description : set image file info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileInfo(int32 index, //in
                                        MMIDC_FILE_INFO_T *file_info_ptr //in
                                        )
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
/*    
        dc_info_ptr->file_list[index].file_name_len = file_name_len;
        MMI_WSTRNCPY(dc_info_ptr->file_list[index].file_name, MMIFILE_FULL_PATH_MAX_LEN,\
                 full_file_name_ptr, file_name_len, file_name_len);
        dc_info_ptr->file_list[index].file_size = dc_info_ptr->image_len;
*/       
        SCI_TRACE_LOW("[MMIDC] MMIDC_SetImageFileInfo file_dev=%d", file_info_ptr->file_dev);
        SCI_TRACE_LOW("[MMIDC] MMIDC_SetImageFileInfo file_size=%d", file_info_ptr->file_size);
        SCI_TRACE_LOW("[MMIDC] MMIDC_SetImageFileInfo handle=%d", file_info_ptr->file_handle);
        
    //     SCI_MEMCPY(&dc_info_ptr->file_list[index], file_info_ptr, sizeof(*file_info_ptr));

		dc_info_ptr->file_list[index].is_enable = file_info_ptr->is_enable;
		dc_info_ptr->file_list[index].file_dev = file_info_ptr->file_dev;

		MMI_MEMCPY(dc_info_ptr->file_list[index].file_name_ptr, sizeof(wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1), file_info_ptr->file_name, sizeof(file_info_ptr->file_name), sizeof(file_info_ptr->file_name));

		dc_info_ptr->file_list[index].file_name_len = file_info_ptr->file_name_len;
		dc_info_ptr->file_list[index].file_size = file_info_ptr->file_size;
		dc_info_ptr->file_list[index].file_handle = file_info_ptr->file_handle;
      
    }
}

/*****************************************************************************/
//Description : set image file enable
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileEnable(int32 index,
                                              BOOLEAN is_enable
                                             )
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        dc_info_ptr->file_list[index].is_enable = is_enable;
    }
}

/*****************************************************************************/
//Description : set image file attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageFileHidden(int32 index,
                                             BOOLEAN is_hidden
                                             )
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = 0;

    if (PNULL != dc_info_ptr)
    {
        full_path_ptr = dc_info_ptr->file_list[index].file_name_ptr;
        full_path_len = dc_info_ptr->file_list[index].file_name_len;
        if(PNULL != full_path_ptr)
		{
			MMIAPIFMM_SetAttr(full_path_ptr, full_path_len, \
								is_hidden, FALSE, FALSE, FALSE);
		}
    }
}

/*****************************************************************************/
//  Description : set max image file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMaxImageFileSize(uint32 file_size)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        dc_info_ptr->max_file_size = file_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMaxImageFileSize file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1702_112_2_18_2_9_43_581,(uint8*)"d",file_size);
}

/*****************************************************************************/
//  Description : get max image file size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxImageFileSize(void)
{
    uint32 file_size = 0;    
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        file_size = dc_info_ptr->max_file_size;
    }


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxImageFileSize file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1721_112_2_18_2_9_43_582,(uint8*)"d",file_size);

    return file_size;
}
/*****************************************************************************/
//Description : set temp file name id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetTempFileNameID(uint32 name_id)
{
    if (PNULL != MMIDC_SAVE_DATA_PTR)
    {
        MMIDC_SAVE_DATA_PTR->temp_name_id = name_id;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetTempFileNameID name_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1737_112_2_18_2_9_43_583,(uint8*)"d",name_id);
}

/*****************************************************************************/
//  Description : get temp file name id
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetTempFileNameID(void)
{
    uint32 name_id = 0;
    
    if (PNULL != MMIDC_SAVE_DATA_PTR)
    {
        name_id = MMIDC_SAVE_DATA_PTR->temp_name_id;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetTempFileNameID name_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1754_112_2_18_2_9_43_584,(uint8*)"d",name_id);

    return name_id;
}

/*****************************************************************************/
//  Description : get photo default path name
//  Global resource dependence : none
//  Author: robert.wang
//  Note: not include file device
/*****************************************************************************/
PUBLIC wchar *MMIDC_GetPhotoDefaultPathName(void)
{
    wchar *path_ptr = PNULL;
    
#ifdef BAIDU_DRIVE_SUPPORT
	MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM -1;
	MMIFILE_DEVICE_E        ret_dev  = MMI_DEVICE_NUM -1;

	if(1)//!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, ret_dev, photo_size, PNULL))
	{
		path_ptr = MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD;
	}
	else
	{
		path_ptr = MMIMULTIM_DIR_PICTURE;
	}		
#else	
    #ifdef CMCC_UI_STYLE
    path_ptr = MMIMULTIM_DIR_PHOTOS;
    #else
    path_ptr = MMIMULTIM_DIR_PICTURE;
    #endif
#endif   

    return path_ptr;
}

/*****************************************************************************/
//  Description : delete image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteImageFile(int32 index, //in
                                          MMIFILE_DEVICE_E file_dev,//in
                                          wchar *file_path //in
                                          )
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    int32 full_path_len = 0;
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 file_name_len = 0;
    wchar *short_file_name_ptr = PNULL;
    BOOLEAN ret = FALSE;

    if (PNULL != file_path)
    {
        short_file_name_ptr = MMIDC_GetImageFileName(index);

        if (PNULL != short_file_name_ptr)
        {
            full_path_len = MMIDC_CombinePath(full_path, MMIFILE_FULL_PATH_MAX_LEN, file_dev, file_path);            
            file_name_len = MMIAPIDC_CombinePathName(file_name, MMIFILE_FULL_PATH_MAX_LEN,\
                                                 full_path, short_file_name_ptr);
            ret = MMIAPIFMM_DeleteFileSyn(file_name,file_name_len);
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DeleteImageFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1805_112_2_18_2_9_43_585,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : delete disk image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteAllImageFile(MMIFILE_DEVICE_E file_dev)
{
    wchar *default_path_ptr = MMIDC_GetPhotoDefaultPathName();
    int32 i = 0;
    int32 max_number = MMIDC_GetMaxBurstPhotoNumber();

    for (i = 0; i < max_number; i++)
    {
        MMIDC_DeleteImageFile(i, file_dev, default_path_ptr);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DeleteAllImageFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1825_112_2_18_2_9_43_586,(uint8*)"");
}

/*****************************************************************************/
//  Description : delete all disk image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteAllDiskImageFile(void)
{
    MMIFILE_DEVICE_E sd_type = 0;

    for(sd_type = MMI_DEVICE_UDISK; sd_type < MMI_DEVICE_NUM; sd_type++)
    {
        MMIDC_DeleteAllImageFile(sd_type);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DeleteAllDiskImageFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1841_112_2_18_2_9_43_587,(uint8*)"");
}

/*****************************************************************************/
//  Description : delete saved image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteSavedImageFile(int32 index)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    wchar* file_name_ptr = PNULL;
    uint16 file_name_len = 0;
    BOOLEAN ret = FALSE;
    
    if (PNULL != dc_info_ptr)
    {
        file_name_ptr = dc_info_ptr->file_list[index].file_name_ptr;
        file_name_len = dc_info_ptr->file_list[index].file_name_len;

		if(PNULL != file_name_ptr)
		{
			if (file_name_len > 0)
			{
				ret = MMIAPIFMM_DeleteFileSyn(file_name_ptr,file_name_len);
			}
			else
			{
				ret = TRUE;
			}
		}
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1872_112_2_18_2_9_44_588,(uint8*)"dd",ret, index);

    return ret;
}

/*****************************************************************************/
//  Description : delete all saved image file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DeleteAllSavedImageFile(void)
{
    BOOLEAN ret = FALSE;
    int32 i = 0;
    int32 max_number = MMIDC_GetMaxBurstPhotoNumber();

    MMIDC_CloseAllFileHandle();
    for (i = 0; i < max_number; i++)
    {
        ret = MMIDC_DeleteSavedImageFile(i);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SAVE_1892_112_2_18_2_9_44_589,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : init photo store info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitPhotoStoreInfo(void)
{
    if (PNULL != MMIDC_SAVE_DATA_PTR)
    {
        SCI_MEMSET(MMIDC_SAVE_DATA_PTR->photo_info, 0x00, sizeof(MMIDC_SAVE_DATA_PTR->photo_info)); /*lint !e666*/
        MMIDC_SAVE_DATA_PTR->max_file_size = 0;
    }
}

/*****************************************************************************/
//   Description : get video full path ,to provide for autotest_device
//   Global resource dependence : none
//   Author: chunjuanliang.rda
//   Note:this API is used,first record video,and enter autotest_device mode to test
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetVedioFullPath(wchar* videofullpath)
{
    uint16 lenFilename=0;

    if(PNULL == s_save_data_ptr)
    {
        return 0;
    }
    else
    {
        lenFilename=MMIAPICOM_Wstrlen(s_save_data_ptr->video_full_path_arr);

        if(videofullpath!=PNULL)
        {
            MMIAPICOM_Wstrncpy(videofullpath,(s_save_data_ptr->video_full_path_arr),lenFilename);
            return lenFilename;
        }
        return 0;
    }
}

#endif  //#ifdef  CAMERA_SUPPORT  

