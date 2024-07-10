/*****************************************************************************
** File Name:      mmirecord_srv.c                                           *
** Author:                                                                   *
** Date:           2012-06                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv flow control            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2012        apple.zhang           Create                                   *
*****************************************************************************/
#define _MMIRECORD_SRV_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_record_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mmisrvrecord_export.h"
#include "mmisrv.h"
#include "mmi_filemgr.h"
#include "dal_time.h"
#include "mmisrvfmm_export.h"
#include "adpcmrecord_adp.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define RECORD_FILE_NAME_ID_MAX				    10000000

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MMIRECORD_SRV_STATE_START,          // start state
    MMIRECORD_SRV_STATE_RECORDING,      // recording  
    MMIRECORD_SRV_STATE_PLAYING,        // is playing
    MMIRECORD_SRV_STATE_PLAY_PAUSED,    // play be paused   
    MMIRECORD_SRV_STATE_RECORD_PAUSED,
    MMIRECORD_SRV_STATE_MAX             // record is closed
}MMIRECORD_SRV_STATE_E;  //record state

typedef struct  
{
	MMIRECORD_SRV_STATE_E   state;                //录制服务状态
	MMISRV_HANDLE_T     play_handle;              //播放录音handle
	MMISRV_HANDLE_T     record_handle;            //录制handle
	MMIFILE_DEVICE_E	record_dev;
	wchar*              record_file_name_ptr;     //录制文件名
	wchar*              record_file_path_ptr;     //录制文件路径
	RECORD_SRV_NOTIRY_CALLBACK     notifycallback;           //通知调用者的回调函数
	MMIRECORD_SRV_SUPPORT_TYPE_E support_type;
}RECORD_SRV_INFO_T;


//用于存放所有的录制服务信息，该录制服务支持冲入
typedef struct RECORD_SRV_LIST_tag
{
    RECORD_SRV_INFO_T             *record_srv_info_ptr;          
    struct RECORD_SRV_LIST_tag   *next_record_srv_info_ptr;     //下一个录制服务信息
}RECORD_SRV_LIST_T;

typedef struct  
{
	BOOLEAN  is_create_filename;
	char* prefix_ptr;
	uint32 * record_file_id_ptr;
	MMI_STRING_T *record_name_ptr;
	MMISRVAUD_RECORD_FMT_E audio_type;
	MMIFILE_DEVICE_E *record_dev_ptr;	
}RECORD_FILE_NAME_INFO_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/    
LOCAL SCI_MUTEX_PTR s_record_srv_list_mutex = 0;
LOCAL RECORD_SRV_LIST_T  s_record_srv_list = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : assign a default name with date
//  Global resource dependence :                                
//  Author: koki.gao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordDateFileName(RECORD_SRV_INFO_T *record_srv_info_ptr, 
									   MMI_STRING_T  record_name_str,
									   MMISRVAUD_RECORD_FMT_E formattype);

/*****************************************************************************/
//  Description : assign a default name with id
//  Global resource dependence :                                
//  Author: koki.gao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordIDFileName(RECORD_SRV_INFO_T *record_srv_info_ptr, 
									 uint32 record_file_id, 
									 char* prefix_ptr,
									 MMISRVAUD_RECORD_FMT_E record_format);

/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateFileName(RECORD_SRV_INFO_T *record_srv_info_ptr,
							 RECORD_FILE_NAME_INFO_T file_name_info);


/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_rec(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : notify handle function for play
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_ply(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
// 	Description : create name
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL uint16 GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type);
LOCAL uint16 GetFileIDStringEx(char* file_name, const char* prefix, uint32 id,const char* file_type);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CheckFilenameSuffix(RECORD_SRV_HANDLE record_srv_handle,
							   MMISRVAUD_RECORD_FMT_E audio_type);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E GetPreferDev(MMIFILE_DEVICE_E     *file_dev_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL RECORD_SRV_HANDLE GetRecordHandle(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: xingdong.li
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEnd(RECORD_SRV_HANDLE record_srv_handle, 
						   BOOLEAN is_succeed);

/*****************************************************************************/
//  Description : 生成录制文件名字
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL  MMIFILE_ERROR_E CreateRecordName(RECORD_SRV_HANDLE record_srv_handle,
								RECORD_FILE_NAME_INFO_T file_name_info
								);

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
LOCAL BOOLEAN RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        );

/*****************************************************************************/
//  Description : clear current record file name
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ClearCurrentRecordFileName(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : destroy mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyRecordListMutex(void);

/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordListMutex(void);

/*****************************************************************************/
//  Description : 开始录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartIQRecord(RECORD_SRV_HANDLE record_srv_handle,
										   RECORD_PARAM_T *record_param_ptr,
										   MMISRVAUD_VOLUME_T volume);

/*****************************************************************************/
// 	Description : set record file path
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetRecordFilePath(RECORD_SRV_HANDLE record_srv_handle,
											 wchar  *record_file_full_path_ptr,
											 uint16  record_file_full_path_len);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get record file name
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORDSRV_GetRecordFileName(RECORD_SRV_HANDLE record_srv_handle)
{
	RECORD_SRV_INFO_T *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	wchar *record_file_name_ptr = PNULL;

    if (PNULL != record_srv_info_ptr
		&& PNULL != record_srv_info_ptr->record_file_name_ptr
		&& 0 != MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr))
    {
		record_file_name_ptr = record_srv_info_ptr->record_file_name_ptr;
    }
	return record_file_name_ptr;
}

/*****************************************************************************/
// 	Description : get record file path
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORDSRV_GetRecordFilePath(RECORD_SRV_HANDLE record_srv_handle)
{
    RECORD_SRV_INFO_T *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	wchar *record_file_path_ptr = PNULL;

    if (PNULL != record_srv_info_ptr)
    {
		record_file_path_ptr = record_srv_info_ptr->record_file_path_ptr;
    }
	return record_file_path_ptr;
}

/*****************************************************************************/
// 	Description : set record file path
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetRecordFilePath(RECORD_SRV_HANDLE record_srv_handle,
											 wchar  *record_file_full_path_ptr,
											 uint16  record_file_full_path_len)
{
    RECORD_SRV_INFO_T *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	BOOLEAN            result  = FALSE;
	wchar      record_file_name[MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1] = {0};
	uint16     record_file_name_len = MMIRECORD_SRV_MAX_FILE_NAME_LEN;
	wchar      record_file_path[MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1] = {0};
	uint16     record_file_path_len = MMIRECORD_SRV_MAX_FILE_NAME_LEN;

    if (PNULL != record_srv_info_ptr
		&& PNULL != record_file_full_path_ptr
		&& PNULL != record_srv_info_ptr->record_file_path_ptr
		&& PNULL != record_srv_info_ptr->record_file_name_ptr)
    {
		SCI_MEMSET(record_file_name,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN+1)*sizeof(wchar)));
		record_file_path_len = MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN,record_file_full_path_len);
		SCI_MEMSET(record_file_path,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN+1)*sizeof(wchar)));
		MMIAPICOM_Wstrncpy(record_file_path,record_file_full_path_ptr,record_file_path_len);
		result = MMIAPIFMM_SplitLastName(record_file_path,&record_file_path_len,record_file_name,&record_file_name_len);
		if (result)
		{
			MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_path_ptr,record_file_full_path_ptr,MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN,record_file_full_path_len));
			MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_name_ptr,record_file_name,MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN,record_file_name_len));	
		}
    }
	return result;
	
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: xingdong.li
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEnd(RECORD_SRV_HANDLE record_srv_handle, 
						   BOOLEAN is_succeed)
{	
	
	RECORD_SRV_INFO_T   *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMIRECORD_SRV_RESULT_E  result = MMIRECORD_SRV_RESULT_SUCCESS;
	
	if (PNULL == record_srv_info_ptr)
	{
		SCI_TRACE_LOW("[RECORDSRV] HandleRecordEnd record srv handle is null");
		return;
	}    
	

	if (0 != record_srv_info_ptr->record_handle)
	{
		is_succeed = MMISRVAUD_Stop(record_srv_info_ptr->record_handle);
        MMISRVMGR_Free(record_srv_info_ptr->record_handle);
		record_srv_info_ptr->record_handle = 0;
	}

	SCI_TRACE_LOW("[RECORDSRV] HandleRecordEnd record srv handle is_succeed = %d", is_succeed);
	if(!is_succeed)
    {
        MMIRECORDSRV_DeleteRecordFile(record_srv_handle,TRUE);    
		result = MMIRECORD_SRV_RESULT_ERROR;
    }    

	
	record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;

	if (PNULL != record_srv_info_ptr->notifycallback)
	{
		record_srv_info_ptr->notifycallback(record_srv_handle, MMIRECORD_SRV_SUPPORT_RECORD, result);
	}
	
}


/*****************************************************************************/
//  Description : assign a default name with date
//  Global resource dependence :                                
//  Author: koki.gao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordDateFileName(RECORD_SRV_INFO_T *record_srv_info_ptr, 
									   MMI_STRING_T  record_name_str,
									   MMISRVAUD_RECORD_FMT_E formattype)
{  
     // 日期和时间
    wchar  date_time[MMIRECORD_SRV_STRING_MAX_NUM +1]    = {0};
	uint16 date_len  = 0;
    wchar  time_wstr[MMIRECORD_SRV_STRING_MAX_NUM +1]    = {0};
    uint8  time_str[MMIRECORD_SRV_STRING_MAX_NUM+1]      = {0};
    uint8  date_str[MMIRECORD_SRV_STRING_MAX_NUM+1]      = {0};    
    SCI_DATE_T    sys_date = {0};
    SCI_TIME_T    sys_time = {0};

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr)
	{
		SCI_TRACE_LOW("[RECORDSRV] CreateRecordDateFileName input param error");
		return FALSE;
	}

	SCI_TRACE_LOW("[RECORDSRV] CreateRecordDateFileName formattype = %d",formattype);
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);   
    
      // 日期
	SCI_MEMSET(date_str,0,(MMIRECORD_SRV_STRING_MAX_NUM+1));
	SCI_MEMSET(date_time, 0, ((MMIRECORD_SRV_STRING_MAX_NUM+1)*sizeof(wchar)));
    sprintf((char*) date_str, "%04d%02d%02d", sys_date.year, sys_date.mon, sys_date.mday); 
    MMIAPICOM_StrToWstr(date_str, date_time);
    
    // 时间
	SCI_MEMSET(time_str,0,(MMIRECORD_SRV_STRING_MAX_NUM+1));
	SCI_MEMSET(time_wstr, 0, ((MMIRECORD_SRV_STRING_MAX_NUM+1)*sizeof(wchar)));
    sprintf((char*)time_str,"%02d%02d%02d",sys_time.hour,sys_time.min,sys_time.sec); 
    MMIAPICOM_StrToWstr(time_str, time_wstr);
    MMIAPICOM_Wstrcat(date_time, time_wstr);
    date_len = MMIAPICOM_Wstrlen(date_time);
    
    /* Combine file name something */
	SCI_MEMSET(record_srv_info_ptr->record_file_name_ptr,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    MMIAPICOM_Wstrncat(record_srv_info_ptr->record_file_name_ptr, record_name_str.wstr_ptr,MIN(record_name_str.wstr_len,(MMIRECORD_SRV_MAX_FILE_NAME_LEN-5-date_len)));
    MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, L"_");
    MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, date_time);
#ifdef MMI_RECORD_INCALL_FORMAT_ALL   
    
    if(MMISRVAUD_RECORD_FMT_MP3==formattype)
     {
         MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, L".mp3");
     }
    else if(MMISRVAUD_RECORD_FMT_AMR==formattype)
     {
         MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, L".amr");
     }
    else
     {
         MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, L".wav");  
     } 
#else
	MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr, L".wav"); 
#endif
	return TRUE;
}

/*****************************************************************************/
//  Description : assign a default name with id
//  Global resource dependence :                                
//  Author: koki.gao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordIDFileName(RECORD_SRV_INFO_T *record_srv_info_ptr, 
									 uint32 record_file_id, 
									 char* prefix_ptr,
									 MMISRVAUD_RECORD_FMT_E formattype)
{
	char            temp_photo_name[MMIRECORD_SRV_MAX_FILE_NAME_LEN] = {0};

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr)
	{
		SCI_TRACE_LOW("[RECORDSRV] CreateRecordIDFileName record info null");
		return FALSE;
	}
    
	SCI_TRACE_LOW("[RECORDSRV] CreateRecordIDFileName formattype = %d",formattype);

	if (PNULL == prefix_ptr)
	{
		prefix_ptr = "REC";
	}
	if(MMISRVAUD_RECORD_FMT_MP3 == formattype)
    {
        GetFileIDStringEx(temp_photo_name, prefix_ptr, record_file_id,  "mp3");
    }
    else if(MMISRVAUD_RECORD_FMT_AMR == formattype) 
    {
        GetFileIDStringEx(temp_photo_name, prefix_ptr, record_file_id,  "amr");
    }
     else 
    {
        GetFileIDStringEx(temp_photo_name, prefix_ptr, record_file_id,  "wav");
    }
	SCI_MEMSET(record_srv_info_ptr->record_file_name_ptr,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    MMIAPICOM_StrToWstr((uint8 *)temp_photo_name, record_srv_info_ptr->record_file_name_ptr);
	return TRUE;
}

/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateFileName(RECORD_SRV_INFO_T *record_srv_info_ptr,
							 RECORD_FILE_NAME_INFO_T file_name_info)
{
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    wchar           full_path[MMIRECORD_SRV_MAX_FILE_NAME_LEN +1] = {0};
    uint16          full_path_len = MMIRECORD_SRV_MAX_FILE_NAME_LEN;  
    MMIFILE_ERROR_E  result = SFS_ERROR_NONE;
	BOOLEAN         create_file_result = FALSE;

 

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == file_name_info.record_dev_ptr
		)
	{
		SCI_TRACE_LOW("[RECORDSRV] CreateFileName record info is null");
		return SFS_ERROR_INVALID_PARAM;
	}

    result = GetPreferDev(file_name_info.record_dev_ptr);
    if(SFS_ERROR_NONE != result)
	{
		return result;
	}


	SCI_TRACE_LOW("[RECORDSRV] CreateFileName record info file_dev = %d, formattype = %d, record_file_id = %d",file_name_info.record_dev_ptr?*file_name_info.record_dev_ptr:-1,
	                                 file_name_info.audio_type,
	                                 file_name_info.record_file_id_ptr?*file_name_info.record_file_id_ptr:-1);


   if (PNULL != file_name_info.record_name_ptr)
    {  	  
        create_file_result = CreateRecordDateFileName(record_srv_info_ptr,*file_name_info.record_name_ptr,file_name_info.audio_type);
    }
    else if (PNULL != file_name_info.record_file_id_ptr)    
	{
		create_file_result = CreateRecordIDFileName(record_srv_info_ptr,*file_name_info.record_file_id_ptr, file_name_info.prefix_ptr, file_name_info.audio_type);	
        
    }

	if (create_file_result)
	{			
		*file_name_info.record_dev_ptr = (*file_name_info.record_dev_ptr < MMI_DEVICE_NUM ? *file_name_info.record_dev_ptr : MMI_DEVICE_UDISK);
		file_dev_ptr = MMIAPIFMM_GetDevicePath(*file_name_info.record_dev_ptr);
		file_dev_len = MMIAPIFMM_GetDevicePathLen(*file_name_info.record_dev_ptr);     
		SCI_MEMSET(full_path,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
		MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
					record_srv_info_ptr->record_file_name_ptr, (uint16)MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr), full_path, &full_path_len);
		if(full_path_len <= MMIRECORD_SRV_MAX_FILE_NAME_LEN)                
		{
			SCI_MEMSET(record_srv_info_ptr->record_file_path_ptr,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
			MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_path_ptr, full_path,full_path_len);
			record_srv_info_ptr->record_dev = *file_name_info.record_dev_ptr;			
			//快速点击录音按钮，如果1s内点击多次，则每次创建的file name 会相同，从而导致录音失败NEWMS00191498
			if (MMIAPIFMM_IsFileExist(record_srv_info_ptr->record_file_path_ptr, MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr)))
			{
				return SFS_ERROR_HAS_EXIST;
			}  
			return SFS_ERROR_NONE;
		
		}
	
	}
    return SFS_ERROR_INVALID_PARAM; 
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_DeleteRecordFile(RECORD_SRV_HANDLE record_srv_handle,
														  BOOLEAN  is_clear_info)
{
    MMIRECORD_SRV_RESULT_E result = MMIRECORD_SRV_RESULT_SUCCESS;
	RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
    uint16 cur_file_len = 0;
	
	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| 0 == MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr))
	{
		SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_DeleteRecordFile record handle is null");
		return MMIRECORD_SRV_RESULT_ERROR;
	}
	
	cur_file_len = MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr);

    if(0 != cur_file_len)
	{
		if (MMIAPIFMM_IsFileExist(record_srv_info_ptr->record_file_path_ptr, cur_file_len))
		{
			if (!MMIAPIFMM_DeleteFileSyn(record_srv_info_ptr->record_file_path_ptr, cur_file_len))
			{
				result = MMIRECORD_SRV_RESULT_ERROR;
			}
			else if(is_clear_info)
			{
				ClearCurrentRecordFileName(record_srv_handle);
			}
		}
		else
		{

			result = MMIRECORD_SRV_RESULT_NO_EXIST;
		}
	}
	else
	{
		result = MMIRECORD_SRV_RESULT_PARAM_ERROR;
	}
		
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_DeleteRecordFile result = %d",result);
    return result;
}


/*****************************************************************************/
//  Description : 生成录制文件名字
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL  MMIFILE_ERROR_E CreateRecordName(RECORD_SRV_HANDLE record_srv_handle,
								RECORD_FILE_NAME_INFO_T file_name_info	
								)
{
	register uint32       step = 1;
	RECORD_SRV_INFO_T* record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMIFILE_ERROR_E  result = SFS_ERROR_NONE;


	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == file_name_info.record_dev_ptr)
	{
		return FALSE;
	}
    SCI_TRACE_LOW("[RECORDSRV] CreateRecordName is_create_filename = %d",file_name_info.is_create_filename);
    //default filename
    if (file_name_info.is_create_filename)
    {      				
		if (PNULL!= file_name_info.record_name_ptr)
		{
			result = CreateFileName(record_srv_info_ptr,file_name_info);			  
			if (SFS_ERROR_HAS_EXIST == result)
			{
				MMIAPIFMM_DeleteFile(record_srv_info_ptr->record_file_path_ptr, PNULL);
				result = SFS_ERROR_NONE;
			}
			   
			return result;
		}
		else if(PNULL != file_name_info.record_file_id_ptr)
		{

			while(1)  /*lint !e716*/
			{           
				*file_name_info.record_file_id_ptr += 1;
				if(*file_name_info.record_file_id_ptr >= RECORD_FILE_NAME_ID_MAX)
				{
				       return SFS_ERROR_INVALID_PARAM;
				}		
				SCI_TRACE_LOW("[RECORDSRV] CreateRecordName record_file_id = %d",*file_name_info.record_file_id_ptr);
				result = CreateFileName(record_srv_info_ptr, file_name_info);
				if (SFS_ERROR_HAS_EXIST != result)	
				{
					return result;
				}
			}
		}
		else
		{
			return SFS_ERROR_INVALID_PARAM;
		}
    }
    else 
    {
		if (PNULL != file_name_info.record_name_ptr
			&& PNULL != file_name_info.record_name_ptr->wstr_ptr)   
		{
			if(!SetRecordFilePath(record_srv_handle, file_name_info.record_name_ptr->wstr_ptr, file_name_info.record_name_ptr->wstr_len))
			{
				return SFS_ERROR_INVALID_PARAM;
			}
		}
		MMIRECORDSRV_DeleteRecordFile(record_srv_handle,FALSE);
        CheckFilenameSuffix(record_srv_handle, file_name_info.audio_type);
    }

	return SFS_ERROR_NONE;
}

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
LOCAL  MMIFILE_ERROR_E CreateRecordNameFull(RECORD_SRV_HANDLE record_srv_handle,
								RECORD_FILE_NAME_INFO_T file_name_info	
								)
{
	register uint32       step = 1;
	uint32                circle = 1;
	RECORD_SRV_INFO_T* record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMIFILE_ERROR_E  result = SFS_ERROR_NONE;


	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == file_name_info.record_dev_ptr)
	{
		return FALSE;
	}
	if (PNULL!= file_name_info.record_name_ptr)
	{
		SCI_MEMSET(record_srv_info_ptr->record_file_path_ptr,0,((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
		MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_path_ptr, file_name_info.record_name_ptr->wstr_ptr,file_name_info.record_name_ptr->wstr_len);
		record_srv_info_ptr->record_dev = *file_name_info.record_dev_ptr;			
		//快速点击录音按钮，如果1s内点击多次，则每次创建的file name 会相同，从而导致录音失败NEWMS00191498
		if (MMIAPIFMM_IsFileExist(record_srv_info_ptr->record_file_path_ptr, MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr)))
		{
			MMIAPIFMM_DeleteFile(record_srv_info_ptr->record_file_path_ptr, PNULL);
		}  
		result = SFS_ERROR_NONE;			   
		return result;
	}
	else
	{
		return SFS_ERROR_INVALID_PARAM;
	}

    return SFS_ERROR_NONE;
}
#endif

/*****************************************************************************/
//  Description : rename record name
//  Global resource dependence : none
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL  MMIFILE_ERROR_E RenameRecordName(RECORD_SRV_HANDLE record_srv_handle,
								MMI_STRING_T *record_name_ptr)
{
	register uint32       step = 1;
	uint32                circle = 1;
	RECORD_SRV_INFO_T* record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMIFILE_ERROR_E  result = SFS_ERROR_NONE;


	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr)
	{
		return FALSE;
	}
    //default filename
    if(MMIAPIFMM_RenameFile(
        record_srv_info_ptr->record_file_path_ptr,
        MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr),
        record_name_ptr->wstr_ptr,record_name_ptr->wstr_len)
        )
    {
        return SFS_ERROR_NONE;
    }
    else
    {
        return SFS_ERROR_INVALID_FORMAT;
    }
}

PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_RenameRecord(RECORD_SRV_HANDLE record_srv_handle,
								MMI_STRING_T *record_name_ptr)
{   
    MMISRVAUD_TYPE_U     audio_data  = {0};
	RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMISRV_HANDLE_T audio_handle = 0;
	MMIFILE_ERROR_E   file_result = SFS_ERROR_NONE;
	RECORD_FILE_NAME_INFO_T file_name_info = {0};

    RenameRecordName(record_srv_handle,record_name_ptr);

    return MMIRECORD_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
//  Description : 开始录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartIQRecord(RECORD_SRV_HANDLE record_srv_handle,
										   RECORD_PARAM_T *record_param_ptr,
										   MMISRVAUD_VOLUME_T volume)
{	
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMISRVAUD_TYPE_U audio_data = {0};
	MMISRV_HANDLE_T audio_handle = 0;
        
	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_param_ptr
		|| PNULL != record_srv_info_ptr->record_handle)
	{
		return MMIRECORD_SRV_RESULT_PARAM_ERROR;
	}
	

	if(record_srv_info_ptr->record_handle > 0)//如果原来handle不为0，则初始化。
    {
        MMISRVAUD_Stop(record_srv_info_ptr->record_handle);
        MMISRVMGR_Free(record_srv_info_ptr->record_handle);
        record_srv_info_ptr->record_handle = 0;
    } 
	
    MMIRECORDSRV_DeleteRecordFile(record_srv_handle,FALSE);

    		
    audio_data.iq_file.type = MMISRVAUD_TYPE_IQ;
    audio_data.iq_file.name_ptr = record_srv_info_ptr->record_file_path_ptr;
    audio_data.iq_file.param    = record_param_ptr; /*lint !e733*/
    
    if(RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec, volume))
    {
		record_srv_info_ptr->record_handle = audio_handle;
		if(MMISRVAUD_RET_OK == MMISRVAUD_Play(audio_handle, 0))
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_RECORDING;
			return MMIRECORD_SRV_RESULT_SUCCESS;
		}
		else
		{
			MMISRVMGR_Free(audio_handle);
			record_srv_info_ptr->record_handle = 0;				
		}
		ClearCurrentRecordFileName(record_srv_handle);
	}
    
	return MMIRECORD_SRV_RESULT_ERROR;
}

/*****************************************************************************/
//  Description : 开始录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 如果需要创建新的文件名，则server层提供两种文件名的录制：
//        一种是通过传入的record id来命名， 另一种是通过日期来命名；
//         通过record id命名规则是：prefix+id+后缀；
//        通过日期命名规则是：record_name+日期时间+后缀；
//        如果record_name_ptr为空，则使用record id来命名，否则以record_name来命名。
//       如果不需要另外创建新的文件名，则如果record_name_ptr不为空，采用该名字录制，
//       否则采用之前server层保存的名字录制，也就是重录
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_StartRecord(RECORD_SRV_HANDLE record_srv_handle,
													   MMIRECORD_SRV_RECORD_PARAM_T record_param)
{   
    MMISRVAUD_TYPE_U     audio_data  = {0};
	RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMISRV_HANDLE_T audio_handle = 0;
	MMIFILE_ERROR_E   file_result = SFS_ERROR_NONE;
	RECORD_FILE_NAME_INFO_T file_name_info = {0};


    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord");

    if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL != record_srv_info_ptr->record_handle
		)
    {        
        SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord return MMIRECORD_SRV_RESULT_PARAM_ERROR");
        return MMIRECORD_SRV_RESULT_PARAM_ERROR;
    }

	file_name_info.audio_type = record_param.fmt;
	file_name_info.is_create_filename = record_param.is_create_file_name;
	file_name_info.prefix_ptr = record_param.prefix_ptr;
	file_name_info.record_dev_ptr = record_param.record_dev_ptr;
	file_name_info.record_file_id_ptr = record_param.record_file_id_ptr;
	file_name_info.record_name_ptr = record_param.record_name_ptr;

	if (PNULL != record_param.record_param_ptr)
	{
		file_result = CreateRecordName(record_srv_handle, file_name_info);
		if(SFS_ERROR_NONE == file_result)
		{
			return StartIQRecord(record_srv_handle,record_param.record_param_ptr,record_param.volume);
		}
		else if (SFS_ERROR_NO_SPACE == file_result)
		{
			return MMIRECORD_SRV_RESULT_NO_SPACE;
		}
		SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord return MMIRECORD_SRV_RESULT_ERROR");
		return MMIRECORD_SRV_RESULT_ERROR;
	}
	
    
    if(record_srv_info_ptr->record_handle > 0)//如果原来handle不为0，则初始化。
    {
        MMISRVAUD_Stop(record_srv_info_ptr->record_handle);
        MMISRVMGR_Free(record_srv_info_ptr->record_handle);
        record_srv_info_ptr->record_handle = 0;
    }   
     

	
	file_result = CreateRecordName(record_srv_handle, file_name_info);
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord file_result:%d",file_result);
	if(SFS_ERROR_NONE == file_result)
    {
		audio_data.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
		audio_data.record_file.name = record_srv_info_ptr->record_file_path_ptr;
		audio_data.record_file.name_len  = MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr);
		audio_data.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;    
		audio_data.record_file.source   = record_param.source;
		audio_data.record_file.fmt = record_param.fmt;   
    
    
		if (RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec,record_param.volume))
		{
			record_srv_info_ptr->record_handle = audio_handle;       
			if(MMISRVAUD_RET_OK == MMISRVAUD_Play(audio_handle, 0))
			{
				record_srv_info_ptr->state = MMIRECORD_SRV_STATE_RECORDING;
				SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord return MMIRECORD_SRV_RESULT_SUCCESS");
				return MMIRECORD_SRV_RESULT_SUCCESS;
			}
			else
			{
				MMISRVMGR_Free(audio_handle);
				record_srv_info_ptr->record_handle = 0;			
			}
		}
		else
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;        
		}
		ClearCurrentRecordFileName(record_srv_handle);
	}
	else if (SFS_ERROR_NO_SPACE == file_result)
	{
		return MMIRECORD_SRV_RESULT_NO_SPACE;
	}
    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StartRecord return MMIRECORD_SRV_RESULT_ERROR");
    return MMIRECORD_SRV_RESULT_ERROR;
}

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_StartRecordFullFile(RECORD_SRV_HANDLE record_srv_handle,
													   MMIRECORD_SRV_RECORD_PARAM_T record_param)
{   
    MMISRVAUD_TYPE_U     audio_data  = {0};
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
    MMISRV_HANDLE_T audio_handle = 0;
    MMIFILE_ERROR_E   file_result = SFS_ERROR_NONE;
    RECORD_FILE_NAME_INFO_T file_name_info = {0};


    if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL != record_srv_info_ptr->record_handle
		)
    {        
       
        return MMIRECORD_SRV_RESULT_PARAM_ERROR;
    }

	file_name_info.audio_type = record_param.fmt;
	file_name_info.is_create_filename = record_param.is_create_file_name;
	file_name_info.prefix_ptr = record_param.prefix_ptr;
	file_name_info.record_dev_ptr = record_param.record_dev_ptr;
	file_name_info.record_file_id_ptr = record_param.record_file_id_ptr;
	file_name_info.record_name_ptr = record_param.record_name_ptr;
	
    
    if(record_srv_info_ptr->record_handle > 0)//如果原来handle不为0，则初始化。
    {
        MMISRVAUD_Stop(record_srv_info_ptr->record_handle);
        MMISRVMGR_Free(record_srv_info_ptr->record_handle);
        record_srv_info_ptr->record_handle = 0;
    }   
     
	file_result = CreateRecordNameFull(record_srv_handle, file_name_info);
	if(SFS_ERROR_NONE == file_result)
    {
		audio_data.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
		audio_data.record_file.name = record_srv_info_ptr->record_file_path_ptr;
		audio_data.record_file.name_len  = MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr);
		audio_data.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;    
		audio_data.record_file.source   = record_param.source;
		audio_data.record_file.fmt = record_param.fmt;   
    
    
		if (RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec,record_param.volume))
		{
			record_srv_info_ptr->record_handle = audio_handle;       
			if(MMISRVAUD_RET_OK == MMISRVAUD_Play(audio_handle, 0))
			{
				record_srv_info_ptr->state = MMIRECORD_SRV_STATE_RECORDING;
				return MMIRECORD_SRV_RESULT_SUCCESS;
			}
			else
			{
				MMISRVMGR_Free(audio_handle);
				record_srv_info_ptr->record_handle = 0;			
			}
		}
		else
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;        
		}
		ClearCurrentRecordFileName(record_srv_handle);
	}
	else if (SFS_ERROR_NO_SPACE == file_result)
	{
		return MMIRECORD_SRV_RESULT_NO_SPACE;
	}
    return MMIRECORD_SRV_RESULT_ERROR;
}
#endif

/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_StopRecord(RECORD_SRV_HANDLE record_srv_handle)
{
    MMIRECORD_SRV_RESULT_E error_code = MMIRECORD_SRV_RESULT_SUCCESS;
   

	RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
    
    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StopRecord enter");

    if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_handle)
    {
        SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StopRecord param error");
        error_code = MMIRECORD_SRV_RESULT_PARAM_ERROR;
        return error_code;
    }

           
    MMISRVAUD_Stop(record_srv_info_ptr->record_handle);
    MMISRVMGR_Free(record_srv_info_ptr->record_handle);
    record_srv_info_ptr->record_handle = 0;
    HandleRecordEnd(record_srv_handle,TRUE);         
    
    return error_code;
}


/*****************************************************************************/
//  Description : 暂停录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_PauseRecord(RECORD_SRV_HANDLE record_srv_handle)
{
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	BOOLEAN              result = FALSE;

    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_PauseRecord enter ");
    if(PNULL != record_srv_info_ptr
		&& record_srv_info_ptr->record_handle > 0
		&& MMIRECORD_SRV_STATE_RECORDING == record_srv_info_ptr->state)
    {
        result = MMISRVAUD_Pause(record_srv_info_ptr->record_handle);
		if (result)
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_RECORD_PAUSED;
		}		
    }
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_PauseRecord  result = %d",result); 
	return result;
}

/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_ResumeRecord(RECORD_SRV_HANDLE record_srv_handle)
{
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	BOOLEAN                result = FALSE;

    SCI_TRACE_LOW("[RECORD] MMIRECORDSRV_ResumeRecord enter ");
    if(PNULL != record_srv_info_ptr
		&& record_srv_info_ptr->record_handle > 0
		&& MMIRECORD_SRV_STATE_RECORD_PAUSED == record_srv_info_ptr->state)
    {
        result = MMISRVAUD_Resume(record_srv_info_ptr->record_handle);
		if (result)
		{		
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_RECORDING;
		}
    }
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_ResumeRecord  result = %d",result); 
	return result;
}



/*****************************************************************************/
//  Description : 暂停播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_PausePlay(RECORD_SRV_HANDLE record_srv_handle)
{
   RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
   BOOLEAN                result = FALSE;

    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_PausePlay enter ");
    
    if(PNULL != record_srv_info_ptr
		&& record_srv_info_ptr->play_handle > 0
		&& MMIRECORD_SRV_STATE_PLAYING == record_srv_info_ptr->state)
    {
        result = MMISRVAUD_Pause(record_srv_info_ptr->play_handle);
		if (result)
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_PLAY_PAUSED;
		}		
    }
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_PausePlay  result = %d",result); 
	return result;
}

/*****************************************************************************/
//  Description : 继续播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_ResumePlay(RECORD_SRV_HANDLE record_srv_handle)
{
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	BOOLEAN               result  = FALSE;
    
   SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_ResumePlay enter ");
    if(PNULL != record_srv_info_ptr
		&& record_srv_info_ptr->play_handle > 0
	    && MMIRECORD_SRV_STATE_PLAY_PAUSED == record_srv_info_ptr->state)
    {
		
        result = MMISRVAUD_Resume(record_srv_info_ptr->play_handle);
		if (result)
		{
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_PLAYING;
		}		
    }
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_ResumePlay  result = %d",result); 
	return result;
}

/*****************************************************************************/
//  Description : 停止播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_StopPlay(RECORD_SRV_HANDLE record_srv_handle)
{
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	BOOLEAN               result = FALSE;

    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StopPlay enter ");    

    if(PNULL != record_srv_info_ptr
		&& 0 != record_srv_info_ptr->play_handle)
    {
        result = MMISRVAUD_Stop(record_srv_info_ptr->play_handle);
		if (result)
		{
			MMISRVMGR_Free(record_srv_info_ptr->play_handle);
			record_srv_info_ptr->play_handle = 0;		
			record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;
		}        
    }
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_StopPlay  result = %d",result); 
	return result;
	
}

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_Play(RECORD_SRV_HANDLE record_srv_handle,
												MMISRVAUD_VOLUME_T volume)
{
    MMIRECORD_SRV_RESULT_E  error_code = MMIRECORD_SRV_RESULT_SUCCESS;      
    MMISRVAUD_TYPE_U    audio_data  = {0};
    RECORD_SRV_INFO_T     *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	MMISRV_HANDLE_T audio_handle = PNULL;

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| 0 == MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr)
		|| MMIRECORD_SRV_STATE_START != record_srv_info_ptr->state)
	{
		return MMIRECORD_SRV_RESULT_ERROR;
	}

    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_Play enter ");    

  
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = record_srv_info_ptr->record_file_path_ptr;
    audio_data.ring_file.name_len = MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr);
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    if(RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_ply,volume))
    {
        record_srv_info_ptr->play_handle = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {			          
            MMISRVMGR_Free(record_srv_info_ptr->play_handle);
			record_srv_info_ptr->play_handle = 0;		
            error_code = MMIRECORD_SRV_RESULT_ERROR;
        }
		else
		{
			record_srv_info_ptr->state =  MMIRECORD_SRV_STATE_PLAYING;   
		}
    }
    else
    {        
        error_code = MMIRECORD_SRV_RESULT_ERROR;
    }
    SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_Play  error_code = %d",error_code); 
    return error_code;
}

/*****************************************************************************/
//  Description : MMIRECORD_Seek
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_Seek(RECORD_SRV_HANDLE record_srv_handle,
								  uint32  seek_offset)
{
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_Seek enter seek_offset = %d",seek_offset);    

	if (PNULL == record_srv_info_ptr
		|| MMIRECORD_SRV_STATE_PLAYING != record_srv_info_ptr->state
		|| PNULL == record_srv_info_ptr->play_handle
		)
	{
		return FALSE;
	}
	return MMISRVAUD_Seek(record_srv_info_ptr->play_handle, seek_offset);
}
/*****************************************************************************/
//  Description : get playing info
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetPlayingInfo(RECORD_SRV_HANDLE record_srv_handle,  
										MMISRVAUD_PLAY_INFO_T *playing_info_ptr)
{
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_GetPlayingInfo enter "); 
	
	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->play_handle
		|| PNULL == playing_info_ptr
		)
	{
		return FALSE;
	}
	return MMISRVAUD_GetPlayingInfo(record_srv_info_ptr->play_handle, playing_info_ptr);
}

/*****************************************************************************/
//  Description : get record info
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetRecordInfo(RECORD_SRV_HANDLE record_srv_handle,
									   MMISRVAUD_RECORD_INFO_T *record_info_ptr)
{
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_GetRecordInfo enter "); 

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_handle
		|| PNULL == record_info_ptr
		)
	{
		return FALSE;
	}
	return MMISRVAUD_GetRecordInfo(record_srv_info_ptr->record_handle, record_info_ptr);
}

/*****************************************************************************/
//  Description : get record size
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetRecordSize(RECORD_SRV_HANDLE record_srv_handle,
									   MMISRVAUD_RECORD_SIZE_T *record_info_ptr)
{
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_GetRecordSize enter "); 

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_handle
		|| PNULL == record_info_ptr
		)
	{
		return FALSE;
	}
	return MMISRVAUD_GetRecordSize(record_srv_info_ptr->record_handle, record_info_ptr);
}

/*****************************************************************************/
//  Description : to whether there is recoding  
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_IsExistSRVRecord(void)
{    
    BOOLEAN result  = FALSE;
	RECORD_SRV_LIST_T *cur_record_node_ptr = PNULL;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_IsExistSRVRecord enter "); 

	
	if(!CreateRecordListMutex())
	{
		return FALSE;
	}
	SCI_GetMutex(s_record_srv_list_mutex, SCI_WAIT_FOREVER);    	

	cur_record_node_ptr = &s_record_srv_list;
	while (PNULL != cur_record_node_ptr)
	{
		if (PNULL != cur_record_node_ptr->record_srv_info_ptr
			&& 0 != cur_record_node_ptr->record_srv_info_ptr->record_handle)
		{
			result = TRUE;
			break;
		}
		else
		{
			cur_record_node_ptr = cur_record_node_ptr->next_record_srv_info_ptr;
		}
	}

	SCI_PutMutex(s_record_srv_list_mutex);
	DestroyRecordListMutex();
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_IsExistSRVRecord result = %d ",result); 
	return result;
}


/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
LOCAL BOOLEAN RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  volume;

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISRVAUD_VOLUME_LEVEL_MAX)
        {
            audio_srv.volume = MMISRVAUD_VOLUME_LEVEL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

	SCI_TRACE_LOW("[RECORDSRV] RequestHandle*audio_handle = %x",*audio_handle);
    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_rec(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
	RECORD_SRV_HANDLE       record_srv_handle = 0;
	RECORD_SRV_INFO_T *record_srv_info_ptr = PNULL;

    SCI_TRACE_LOW("[RECORDSRV] HandleNotify_rec handle = %x, param = %x",handle,param); 
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;        
        
        if(report_ptr != PNULL)
        {            
            SCI_TRACE_LOW("[RECORDSRV] HandleNotify_rec report_ptr->report = %d",report_ptr->report);         
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:                
				record_srv_handle = GetRecordHandle(handle);
				record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
                if (0 != record_srv_handle)
                {
					MMISRVAUD_Stop(handle);
					MMISRVMGR_Free(handle);
					record_srv_info_ptr->record_handle = 0;
                }                
				SCI_TRACE_LOW("[RECORDSRV] HandleNotify_rec report_ptr->data1 = %d",report_ptr->data1); 
                if(MMISRVAUD_REPORT_RESULT_SUCESS == report_ptr->data1)
				{
					HandleRecordEnd(record_srv_handle, TRUE);
				}
				else
				{
					HandleRecordEnd(record_srv_handle, FALSE);
				}
                break;

            default:
                break;
            }
        
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

/*****************************************************************************/
//  Description : notify handle function for play
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_ply(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
	RECORD_SRV_HANDLE       record_srv_handle = 0;
	RECORD_SRV_INFO_T  *record_srv_info_ptr = PNULL;
	MMIRECORD_SRV_RESULT_E            result = MMIRECORD_SRV_RESULT_SUCCESS;

    SCI_TRACE_LOW("[RECORDSRV] HandleNotify_ply handle = %x, param = %x",handle,param); 
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;        
        
        if(report_ptr != PNULL)
        {            
            SCI_TRACE_LOW("[RECORDSRV] HandleNotify_ply report_ptr->report = %d",report_ptr->report); 
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:                
                record_srv_handle = GetRecordHandle(handle);
				record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
				if (0 != record_srv_handle)
				{
					MMISRVAUD_Stop(handle);
					MMISRVMGR_Free(handle);
					record_srv_info_ptr->play_handle = 0;
					record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;
					SCI_TRACE_LOW("[RECORDSRV] HandleNotify_ply report_ptr->data1=%d, record_srv_info_ptr->notifycallback = %x",report_ptr->data1,record_srv_info_ptr->notifycallback);
					if (PNULL != record_srv_info_ptr->notifycallback
						&& MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
					{
						if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
						{
							result = MMIRECORD_SRV_RESULT_ERROR;
						}
						SCI_TRACE_LOW("[RECORDSRV] HandleNotify_ply result=%d",result);
						record_srv_info_ptr->notifycallback(record_srv_handle,MMIRECORD_SRV_SUPPORT_PLAY,result);
					}
				}
                break;

            default:
                break;
            }
        
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : clear current record file name
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ClearCurrentRecordFileName(RECORD_SRV_HANDLE record_srv_handle)
{    
    wchar dev[] = {'D', ':', '\\', 0};
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	uint16  dev_len = 0;
 
	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr)
	{
		SCI_TRACE_LOW("[RECORDSRV] ClearCurrentRecordFileName record handle null");
		return;
	}
  
    SCI_MEMSET(record_srv_info_ptr->record_file_path_ptr, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
    if(MMIAPIFMM_GetDevicePath(record_srv_info_ptr->record_dev) != PNULL)
	{
		MMIAPICOM_Wstrncpy(dev, MMIAPIFMM_GetDevicePath(record_srv_info_ptr->record_dev),  MMIAPIFMM_GetDevicePathLen(record_srv_info_ptr->record_dev));
	}

	dev_len = MMIAPICOM_Wstrlen(dev);
    MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_path_ptr, dev, dev_len);
    MMIAPICOM_Wstrncpy(record_srv_info_ptr->record_file_path_ptr + dev_len, 
		MMIMULTIM_DIR_MUSIC,
		MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN-dev_len,MMIMULTIM_DIR_MUSIC_LEN));
    
    SCI_MEMSET(record_srv_info_ptr->record_file_name_ptr, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
}


/*****************************************************************************/
// 	Description : create name
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL uint16 GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
 
	SCI_TRACE_LOW("[RECORDSRV] GetFileIDString id = %d, id_max = %d",id,id_max); 
    if(id >= id_max)
    {        
        id = id_max - 100;
    }

    id_len = sprintf(file_name, "%d", id);
    id_max_len = sprintf(string_max, "%d", id_max);
    for(i = 0; i < id_len; i++)
    {
        string_max[id_max_len - id_len + i] = file_name[i];
    }
    string_max[0] = '_';
    string_max[id_max_len] = '.';
    id_len = sprintf(file_name, "%s%s%s", prefix, string_max, file_type);

	SCI_TRACE_LOW("[RECORDSRV] GetFileIDString id_len = %d",id_len); 
    return id_len;
}

LOCAL uint16 GetFileIDStringEx(char* file_name, const char* prefix, uint32 id,  const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
    char max[20] = {0};
    uint8 len = 0;
    uint32 id_max = 0;
 

    if(id <= 999)
    {
        id_max = 1000;
    }
    else
    {
        len = strlen(itoa(id,max,10));
        id_max = 1;
        for(i=0;i<len;i++)
        {
            id_max *= 10;
        }
        id = id -(id_max / 10) + 1;
    }
    id_len = sprintf(file_name, "%d", id);
    id_max_len = sprintf(string_max, "%d", id_max);
    SCI_TRACE_LOW("[RECORDSRV] GetFileIDString id = %d, id_max = %d",id,id_max); 
    for(i = 0; i < id_len; i++)
    {
        string_max[id_max_len - id_len + i] = file_name[i];
    }
    string_max[0] = '.';
    string_max[id_max_len] = '.';
    id_len = sprintf(file_name, "%s%s%s", prefix, string_max, file_type);

    SCI_TRACE_LOW("[RECORDSRV] GetFileIDString id_len = %d",id_len); 
    return id_len;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CheckFilenameSuffix(RECORD_SRV_HANDLE record_srv_handle,
							   MMISRVAUD_RECORD_FMT_E audio_type)
{    
    uint8 filename_len=0;
    wchar current_suffix[MMIRECORD_SRV_DEFAULT_SUFFIX_LEN+1]={0};
    uint16 suffix_len=MMIRECORD_SRV_DEFAULT_SUFFIX_LEN;
    wchar  audiotype_suffix[MMIRECORD_SRV_DEFAULT_SUFFIX_LEN+1]={0};
    wchar  wchar_value=0;
    uint8 recordname_len=0;
    uint8 recordpath_len=0;
    int8 i=0;
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	if (PNULL == record_srv_info_ptr
		|| PNULL == record_srv_info_ptr->record_file_name_ptr
		|| PNULL == record_srv_info_ptr->record_file_path_ptr)
	{
		SCI_TRACE_LOW("[RECORDSRV] CheckFilenameSuffix param erro"); 
		return;
	}
    
	filename_len=MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr);    
    recordname_len=MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_name_ptr);
    recordpath_len=MMIAPICOM_Wstrlen(record_srv_info_ptr->record_file_path_ptr);

    MMIAPIFMM_SplitFileName(record_srv_info_ptr->record_file_name_ptr,
			filename_len,
			PNULL,PNULL,
			current_suffix,&suffix_len);
        
	SCI_TRACE_LOW("[RECORDSRV] CheckFilenameSuffix audio_type = %d",audio_type); 

    switch(audio_type)
    {
        case MMISRVAUD_RECORD_FMT_MP3:
            MMIAPICOM_StrToWstr((const uint8*)"mp3", audiotype_suffix);
			break;
        case MMISRVAUD_RECORD_FMT_AMR:
            MMIAPICOM_StrToWstr((const uint8*)"amr", audiotype_suffix);
			break;
        default:
            MMIAPICOM_StrToWstr((const uint8*)"wav", audiotype_suffix);
			break;
    }

	if(MMIAPICOM_Wstrcmp(current_suffix, audiotype_suffix) !=0)
	{
			//find DOT in file_name_ptr
		for(i=recordname_len-1; i >= 0; i--)
		{
			wchar_value = *(record_srv_info_ptr->record_file_name_ptr+i);
			
			if(MMIFILE_DOT == wchar_value)
			{
				suffix_len = recordname_len - i -1;
				break;
			}
		}
		*(record_srv_info_ptr->record_file_name_ptr+recordname_len-suffix_len)=0X00;
		MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_name_ptr+recordname_len-suffix_len, audiotype_suffix);

				//find DOT in file_name_ptr
		for(i=recordpath_len-1; i >= 0; i--)
		{
			wchar_value = *(record_srv_info_ptr->record_file_path_ptr+i);
			
			if(MMIFILE_DOT == wchar_value)
			{
				suffix_len = recordpath_len - i -1;
				break;
			}
		}
		*(record_srv_info_ptr->record_file_path_ptr+recordpath_len-suffix_len)=0X00;
		MMIAPICOM_Wstrcat(record_srv_info_ptr->record_file_path_ptr+recordpath_len-suffix_len, audiotype_suffix);
    
	}

}


/*****************************************************************************/
//  Description : 分配录音Handle
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC RECORD_SRV_HANDLE MMIRECORDSRV_RequestRecordHandle(RECORD_SRV_NOTIRY_CALLBACK notifycallback)
{
	RECORD_SRV_INFO_T *record_srv_info_ptr = PNULL;
	RECORD_SRV_LIST_T *cur_record_node_ptr = PNULL;
	BOOLEAN         result = FALSE;

	if(!CreateRecordListMutex())
	{
		return (RECORD_SRV_HANDLE)record_srv_info_ptr;
	}
	record_srv_info_ptr = (RECORD_SRV_INFO_T *)SCI_ALLOCA(sizeof(RECORD_SRV_INFO_T));
	if (PNULL != record_srv_info_ptr)
	{
		SCI_MEMSET(record_srv_info_ptr,0,sizeof(RECORD_SRV_INFO_T));
		record_srv_info_ptr->record_file_name_ptr = SCI_ALLOCA((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
		if (PNULL != record_srv_info_ptr->record_file_name_ptr)
		{			
			SCI_MEMSET(record_srv_info_ptr->record_file_name_ptr, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
			record_srv_info_ptr->record_file_path_ptr = SCI_ALLOCA((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
			if (PNULL != record_srv_info_ptr->record_file_path_ptr)
			{
				SCI_MEMSET(record_srv_info_ptr->record_file_path_ptr, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
				record_srv_info_ptr->notifycallback = notifycallback;
				record_srv_info_ptr->state = MMIRECORD_SRV_STATE_START;
				result = TRUE;
			}
		}
	}

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_RequestRecordHandle alloca record info result = %d",result);
	if (result)
	{		
 
		SCI_GetMutex(s_record_srv_list_mutex, SCI_WAIT_FOREVER);    
		if(PNULL == s_record_srv_list.record_srv_info_ptr)
		{
			s_record_srv_list.record_srv_info_ptr = record_srv_info_ptr;
			s_record_srv_list.next_record_srv_info_ptr = PNULL;		
		}
		else
		{
			cur_record_node_ptr = &s_record_srv_list;
			while (PNULL != cur_record_node_ptr->next_record_srv_info_ptr)
			{
				cur_record_node_ptr = cur_record_node_ptr->next_record_srv_info_ptr;
			}
        
			cur_record_node_ptr->next_record_srv_info_ptr = (RECORD_SRV_LIST_T*)SCI_ALLOCA(sizeof(RECORD_SRV_LIST_T));
			if (PNULL != cur_record_node_ptr->next_record_srv_info_ptr)
			{
				cur_record_node_ptr->next_record_srv_info_ptr->next_record_srv_info_ptr = PNULL;
				cur_record_node_ptr->next_record_srv_info_ptr->record_srv_info_ptr = record_srv_info_ptr;
			}
			else
			{
				result = FALSE;
			}
		}
		SCI_PutMutex(s_record_srv_list_mutex);
	}

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_RequestRecordHandle add record info list result = %d",result);
	if (!result
		&& PNULL != record_srv_info_ptr)
	{
		if (PNULL != record_srv_info_ptr->record_file_name_ptr)
		{
			SCI_FREE(record_srv_info_ptr->record_file_name_ptr);
		}
		if (PNULL != record_srv_info_ptr->record_file_path_ptr)
		{
			SCI_FREE(record_srv_info_ptr->record_file_path_ptr);
		}
		
		SCI_FREE(record_srv_info_ptr);
		record_srv_info_ptr = PNULL;
		
	}
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_FreeRecordHandle record_srv_info_ptr = %x", record_srv_info_ptr); 
	return (RECORD_SRV_HANDLE)record_srv_info_ptr;
}
 

/*****************************************************************************/
//  Description : 释放录音Handle
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORDSRV_FreeRecordHandle(RECORD_SRV_HANDLE record_srv_handle)
{
	RECORD_SRV_INFO_T *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;
	RECORD_SRV_LIST_T *cur_record_node_ptr = PNULL;
	RECORD_SRV_LIST_T *next_record_node_ptr = PNULL;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_FreeRecordHandle record_srv_handle = %x", record_srv_handle); 

	if (PNULL == record_srv_info_ptr)
	{
		return;
	}
	
	SCI_GetMutex(s_record_srv_list_mutex, SCI_WAIT_FOREVER);   

	if (s_record_srv_list.record_srv_info_ptr == record_srv_info_ptr)
	{
		next_record_node_ptr = s_record_srv_list.next_record_srv_info_ptr;
		if (PNULL != next_record_node_ptr)
		{		
			s_record_srv_list.record_srv_info_ptr = next_record_node_ptr->record_srv_info_ptr;
			s_record_srv_list.next_record_srv_info_ptr = next_record_node_ptr->next_record_srv_info_ptr;
		}
		else
		{
			s_record_srv_list.record_srv_info_ptr = PNULL;
		}
	}
	else
	{
		cur_record_node_ptr = &s_record_srv_list;
		next_record_node_ptr = s_record_srv_list.next_record_srv_info_ptr;
		while (PNULL != next_record_node_ptr
			&& next_record_node_ptr->record_srv_info_ptr != record_srv_info_ptr)
		{
			cur_record_node_ptr = next_record_node_ptr;
			next_record_node_ptr = next_record_node_ptr->next_record_srv_info_ptr;			
		}
		if (PNULL != next_record_node_ptr)
		{
			cur_record_node_ptr->next_record_srv_info_ptr = next_record_node_ptr->next_record_srv_info_ptr;
			SCI_FREE(next_record_node_ptr);
		}
	}

	SCI_PutMutex(s_record_srv_list_mutex);

	if (PNULL != record_srv_info_ptr->record_file_name_ptr)
	{
		SCI_FREE(record_srv_info_ptr->record_file_name_ptr);
	}
	if (PNULL != record_srv_info_ptr->record_file_path_ptr)
	{
		SCI_FREE(record_srv_info_ptr->record_file_path_ptr);
	}
	
	SCI_FREE(record_srv_info_ptr);
	record_srv_info_ptr = PNULL;
	DestroyRecordListMutex();
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E  GetPreferDev(MMIFILE_DEVICE_E     *file_dev_ptr)
{  
    MMIFILE_DEVICE_E     file_max_dev = MMI_DEVICE_NUM;
	MMIFILE_ERROR_E      file_result = SFS_ERROR_NONE;
    
	if (PNULL == file_dev_ptr)
	{
		return SFS_ERROR_INVALID_PARAM;
	}
	SCI_TRACE_LOW("[RECORDSRV] GetPreferDev file_dev = %d", *file_dev_ptr); 
#ifndef CMCC_UI_STYLE 
    file_result = MMIAPIFMM_GetSuitableFileDev(*file_dev_ptr, MMIRECORD_SRV_MIN_NEED_SPACE, &file_max_dev);
	SCI_TRACE_LOW("[RECORDSRV] GetPreferDev file_max_dev = %d, file_result = %d", file_max_dev,*file_dev_ptr); 

	if(SFS_ERROR_NONE == file_result)
	{
		*file_dev_ptr = file_max_dev;
	}	

#else
	if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, *file_dev_ptr, MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
    {
        file_result = SFS_ERROR_NONE;   
    }
	else
	{
		file_result = SFS_ERROR_NO_SPACE;
	}
#endif	

    return file_result;   
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL RECORD_SRV_HANDLE GetRecordHandle(MMISRV_HANDLE_T audio_handle)
{
	RECORD_SRV_LIST_T *cur_record_node_ptr = PNULL;

	SCI_TRACE_LOW("[RECORDSRV] GetRecordHandle audio_handle = %d", audio_handle); 

	if (0 == audio_handle)
	{
		return 0;
	}
	cur_record_node_ptr = &s_record_srv_list;
	while (PNULL != cur_record_node_ptr)
	{
		if (PNULL != cur_record_node_ptr->record_srv_info_ptr
			&& (cur_record_node_ptr->record_srv_info_ptr->play_handle == audio_handle
			|| cur_record_node_ptr->record_srv_info_ptr->record_handle == audio_handle))
		{
			return (RECORD_SRV_HANDLE)cur_record_node_ptr->record_srv_info_ptr;
		}
		cur_record_node_ptr = cur_record_node_ptr->next_record_srv_info_ptr;
	}
	return 0;

}


/*****************************************************************************/
//  Description : MMIRECORD_SetVolume
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_SetVolume(RECORD_SRV_HANDLE record_srv_handle, uint32 cur_vol_value)
{
	RECORD_SRV_INFO_T  *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_SetVolume cur_vol_value = %d",cur_vol_value);

	if (PNULL == record_srv_info_ptr
		|| 0 == record_srv_info_ptr->play_handle)
	{
		return FALSE;
	}
	return MMISRVAUD_SetVolume(record_srv_info_ptr->play_handle, cur_vol_value);
}

/*****************************************************************************/
//  Description : MMIRECORD_GetContentInfo
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetContentInfo(RECORD_SRV_HANDLE record_srv_handle,
										MMISRVAUD_CONTENT_INFO_T *info_ptr)
{
	wchar *path_ptr = MMIRECORDSRV_GetRecordFilePath(record_srv_handle);   
	uint16 file_path_len = 0;
       
	SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_GetContentInfo enter");

    if(PNULL != path_ptr
		&& PNULL != info_ptr)
    {
        file_path_len = MMIAPICOM_Wstrlen(path_ptr);
        if(file_path_len > 0) 
        {
            return MMISRVAUD_GetFileContentInfo(path_ptr, MMIAPICOM_Wstrlen(path_ptr), info_ptr);
        }
    }
	return FALSE;
}

/*****************************************************************************/
//  Description : MMIRECORDSRV_GetRecordDB
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORDSRV_GetRecordDB(RECORD_SRV_HANDLE record_srv_handle)
{
	RECORD_SRV_INFO_T *record_srv_info_ptr = (RECORD_SRV_INFO_T *)record_srv_handle;

	if (PNULL == record_srv_info_ptr
		|| 0 == record_srv_info_ptr->record_handle)
	{
		SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_GetRecordDB param error");           
		return 0;
	}
	return MMISRVAUD_GetRecordDB(record_srv_info_ptr->record_handle);
	
}

/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateRecordListMutex(void)
{
    if(s_record_srv_list_mutex == 0)
    {
        s_record_srv_list_mutex = SCI_CreateMutex(
            "RECORD_SRV_LIST_MUTEX",
            SCI_INHERIT
            );
    }

	SCI_TRACE_LOW("[RECORD SRV] CreateRecordListMutex s_record_srv_list_mutex = %x",s_record_srv_list_mutex);
	if (0 == s_record_srv_list_mutex)
	{
		return FALSE;
	}
	return TRUE;

}


/*****************************************************************************/
//  Description : destroy mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyRecordListMutex(void)
{
	BOOLEAN is_exist_record_srv = FALSE;

	if(s_record_srv_list_mutex != 0)
	{
		SCI_GetMutex(s_record_srv_list_mutex, SCI_WAIT_FOREVER);   
		if(PNULL != s_record_srv_list.record_srv_info_ptr)
		{
			is_exist_record_srv = TRUE;
		}
		SCI_PutMutex(s_record_srv_list_mutex);

		if (!is_exist_record_srv)
		{	
			SCI_DeleteMutex(s_record_srv_list_mutex);
			s_record_srv_list_mutex = PNULL;	
		}
	}	
}


/*****************************************************************************/
//  Description : record file is exist
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_IsRecordFileExist(RECORD_SRV_HANDLE record_srv_handle)
{
	uint16           file_path_len = 0;
	wchar            *file_path_ptr = MMIRECORDSRV_GetRecordFilePath(record_srv_handle);
	wchar		*record_file_name_ptr = MMIRECORDSRV_GetRecordFileName(record_srv_handle);

	if (PNULL == file_path_ptr
		|| PNULL == record_file_name_ptr)
	{
		return FALSE;
	}
	file_path_len = MMIAPICOM_Wstrlen(file_path_ptr);
    if(0 < file_path_len)
    {
        if(!MMIAPIFMM_IsFileExist(file_path_ptr, file_path_len))
        {
            ClearCurrentRecordFileName(record_srv_handle);
			SCI_TRACE_LOW("[RECORDSRV] MMIRECORDSRV_IsRecordFileExist not exist");
            return FALSE;
        }
    }
	return TRUE;
}

/*****************************************************************************/
//  Description : MMIRECORDSRV_QueryEncoderCapability
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_QueryEncoderCapability(MMISRVAUD_RECORD_FMT_E fmt)
{
	//AUD_REC_FORMAT_E format = AUD_REC_MP3;
	BOOLEAN is_support = FALSE;

	switch(fmt)
    {
    case MMISRVAUD_RECORD_FMT_AMR:
        if(Record_QueryEncoderCapability(AUD_REC_AMR_SW)
			|| Record_QueryEncoderCapability(AUD_REC_AMR))
        {             
            is_support = TRUE;
        }
        break;

    case MMISRVAUD_RECORD_FMT_PCM:
        if(Record_QueryEncoderCapability(AUD_REC_PCM))
        {
            is_support = TRUE;
        }
        break;

    case MMISRVAUD_RECORD_FMT_ADPCM:
        if(Record_QueryEncoderCapability(AUD_REC_ADPCM))
        {
            is_support = TRUE;
        }        
        break;

	case MMISRVAUD_RECORD_FMT_MP3:
		if(Record_QueryEncoderCapability(AUD_REC_MP3))
        {
            is_support = TRUE;
        } 
		break;
		
    default:
        is_support = FALSE;
        break;
    }
	return is_support;
}
