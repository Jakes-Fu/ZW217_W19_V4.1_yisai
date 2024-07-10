/*************************************************************************
 ** File Name:      mmisrv_cameraroll_export.h                                 *
 ** Author:                                       *
 ** Date:                                                    *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                                *
*************************************************************************/

#ifndef MMISRV_BAIDUDRV_EXPORT_H_
#define MMISRV_BAIDUDRV_EXPORT_H_

#include "mmk_type.h"
#include "mmk_app.h"
#include "http_api.h"
#include "json_parser.h"
#include "mmi_filemgr.h"
#include "mmifilearray_export.h"
#include "mn_type.h"
#include "mmipicview_internal.h" 

#define BAIDU_DRV_MAX_STRING_LEN  1024
#define BAIDU_DRV_MAX_ID_LEN  128
#define CONTENT_BUFF_SIZE 50*1024
#define MAX_UPLOAD_ARRAY_SIZE 5
#define MAX_REQUEST_NUM 20
#define BAIDU_TOKEN_MAX_SIZE  (1600)
#define BAIDU_UPLOAD_ID_MAX_SIZE  (1600)
#define MAX_BAIDU_USER_INFO_STR_SIZE 256
#define BAIDU_MEMORY_NEEDED_SIZE 50*1024

#define BAIDU_CANCEL  2
#define BAIDU_OK  1
#define BAIDU_FAILED 0
#define BAIDU_TOKEN -1
#ifdef BAIDU_STORE_U
#define BAIDU_DRV_AVATAR_IMG_FILE_PATH  L"D:\\baidu_drv_avatar.jpg"
#define BAIDU_DRV_QRCODE_IMG_FILE_PATH  L"D:\\baidu_drv_qrcode.jpg"
#else
#define BAIDU_DRV_AVATAR_IMG_FILE_PATH  L"E:\\baidu_drv_avatar.jpg"
#define BAIDU_DRV_QRCODE_IMG_FILE_PATH  L"E:\\baidu_drv_qrcode.jpg"
#endif

typedef enum
{
	BAIDU_DRV_TYPE_NONE,
	BAIDU_DRV_TYPE_LOGIN,		
	BAIDU_DRV_TYPE_DOWNLOAD_THUMBNAIL,
	BAIDU_DRV_TYPE_DOWNLOAD_PHOTO,
	BAIDU_DRV_TYPE_MANUAL_UPLOAD_SINGLE_PHOTO,
	BAIDU_DRV_TYPE_MANUAL_UPLOAD_MULTI_PHOTO,
	BAIDU_DRV_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO,
	BAIDU_DRV_TYPE_MAX
}BAIDU_DRV_TYPE_E;

typedef enum
{
    BAIDU_DRV_SIG_NONE,
    BAIDU_DRV_DOWNLOAD_THUMBNAIL,
    BAIDU_DRV_UPLOAD_PHOTO,
    BAIDU_DRV_CANCEL,
    BAIDU_DRV_DOWNLOAD_PHOTO,
    BAIDU_DRV_DELETE_PHOTO,
    //BAIDU_DRV_DELETE_CLOAD_PHOTO,
    BAIDU_DRV_SIG_MAX
}BAIDU_DRV_SIG_TYPE_E;

typedef enum
{
    BAIDU_DRV_OPER_NOT_INIT,
    BAIDU_DRV_OPER_NO_OPERATION,
    BAIDU_DRV_DEVICE_CODE,          // 查询扫码授权结果   
    BAIDU_DRV_OPER_GET_QR_IMG,
    BAIDU_DRV_OPER_GET_TOKEN,
    BAIDU_DRV_OPER_GET_THUMBNAIL_INFO,
    BAIDU_DRV_OPER_GET_THUMBNAIL_URL,
    BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL,
    BAIDU_DRV_OPER_CREATE_SESSION,
    BAIDU_DRV_OPER_UPLOAD_SESSION,
    BAIDU_DRV_OPER_CLOSE_SESSION,
    BAIDU_DRV_OPER_DOWNLOAD_PHOTO_LOCATION,
    BAIDU_DRV_OPER_DOWNLOAD_PHOTO,
    BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION,
    BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION,
    BAIDU_DRV_OPER_AUTO_CLOSE_SESSION,    
    BAIDU_DRV_OPER_GET_USER_INFO,    
    BAIDU_DRV_OPER_GET_DISK_QUOTA,    
    BAIDU_DRV_OPER_GET_AVATAR_IMG,
    BAIDU_DRV_OPER_GET_CATEGORYLIST,
    BAIDU_DRV_OPER_GET_FILE_METAS,
    BAIDU_DRV_OPER_DELETE_PHOTO,
    BAIDU_DRV_OPER_REFRESH_TOKEN,
    BAIDU_DRV_OPERAION_MAX
}BAIDU_DRV_OPERATION_E;

typedef enum 
{
    JSON_TAG_NONE = 0,
    JSON_TAG_ACCESSTOKEN,
    JSON_TAG_REFRESHTOKEN,
    JSON_TAG_EXPIRES_IN,
    JSON_TAG_SESSION_KEY,
    JSON_TAG_SESSION_SECRET,
    JSON_TAG_AVATAR,		
    JSON_TAG_NAME,
    JSON_TAG_SIZE_TOTAL,
    JSON_TAG_SIZE_USED,
    JSON_TAG_NETDISK_NAME,
    JSON_TAG_VIP_TYPE,
    JSON_TAG_QR_URL,    //10
    JSON_TAG_DEVICE_CODE,  
    JSON_TAG_UPLOAD_ID,
    JSON_TAG_RETURN_TYPE,
    JSON_TAG_ERRNO,
    JSON_TAG_ERROR_CODE,
    JSON_TAG_LIST,
    JSON_TAG_FSID,
    JSON_TAG_CURSOR,
    JSON_TAG_HAS_MORE,
    JSON_TAG_SEVICE_NAME,
    JSON_TAG_MAX
}BAIDU_DRV_JSON_TAG_E;


typedef enum
{
    BAIDU_DRV_AUTO_SYNC,
    BAIDU_DRV_MANUAL_SYNC,
    BAIDU_DRV_SYNC_TYPE_MAX
}BAIDU_DRIVE_SYNC_STATE;

typedef enum
{
    BAIDU_DRIVE_AUTO_UPLOAD_NONE,
    BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK,		
    BAIDU_DRIVE_AUTO_UPLOAD_IDLE,
    BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING,    
    BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING,        
    BAIDU_DRIVE_AUTO_UPLOAD_MAX
}BAIDU_DRIVE_AUTO_UPLOAD_STATE_E;

typedef enum
{
	BAIDU_DRIVE_ERROR_NONE,
	BAIDU_DRIVE_ERROR_NO_MEMORY,
	BAIDU_DRIVE_ERROR_NO_NETWORK,
	BAIDU_DRIVE_ERROR_HTTP_ERROR,
	BAIDU_DRIVE_ERROR_JSON_ERROR,
	BAIDU_DRIVE_ERROR_UNKNOWN
}BAIDU_DRIVE_ERROR_E;

typedef struct _MMISRV_BAIDUDRV_ONEDRIVE_METADATA
{
	uint8 * id;
	uint8 * name;
	uint64 size;
	uint8 * thumbnail_url;	
	struct _MMISRV_BAIDUDRV_METADATA * next_ptr;
}MMISRV_BAIDUDRV_METADATA;


typedef enum
{
    BAIDU_DRV_VIP_NONE,
    BAIDU_DRV_VIP_DENY,
    BAIDU_DRV_VIP_DONE
}BAIDU_DRIVE_VIP_STATUS;


typedef struct _MMISRV_BAIDUDRV_UPLOAD_INFO
{
       uint8 upload_url[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8  session_id[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8  file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
       uint8  file_md5[MMIFILE_FILE_MD5_LEN+1];  /*md5*/
       uint8  file_fs_id[MMIFILE_FS_ID_LEN_MAX+1];
	uint16  file_name_len;
	uint16  file_path[MMIFILE_FULL_PATH_MAX_LEN+1];
	uint16  file_path_len;	
       uint16  slicing_num;
       uint16  current_slicing_num;
#if 1 //def X_SLICING_SUPPORT
       uint16  x_last_slicing_num;
       uint16  x_slicing_current_count;
	DWORD x_part_size;
#endif
	DWORD total_file_size ;  /*byte*/
	DWORD remain_file_size ;
	DWORD sended_file_size ; 
	DWORD uploaded_file_size ;
       uint8* file_md5_list;     /*md51 md52 md53*/
	MMIFILE_HANDLE file_handle ; 
	struct _MMISRV_BAIDUDRV_UPLOAD_INFO * next_ptr;
}MMISRV_BAIDUDRV_UPLOAD_INFO_T;

typedef struct _MMISRV_BAIDUDRV_DOWNLOAD_INFO
{
	BOOLEAN bReceive_cnf;
	uint8  file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
	uint16  file_name_len;
	uint16  file_path[MMIFILE_FULL_PATH_MAX_LEN+1];
	uint16  file_path_len;	
	uint8 id[BAIDU_DRV_MAX_STRING_LEN+1];
	DWORD total_file_size ; 
	DWORD saved_file_size ;
	MMIFILE_HANDLE file_handle ; 
}MMISRV_BAIDUDRV_DOWNLOAD_INFO_T;

typedef struct _MMISRV_BAIDUDRV_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //¨??¨?¨2???¤?2?¨a?|ì?HTTP???¨???ê¨a?¨o?à??¨?D¨°???HTTP???¨?¨o?à?¨|¨°??a¨¨?¨°a?|ì??ê
}MMISRV_BAIDUDRV_HTTP_CONTEXT_T;

typedef struct _MMISRV_BAIDUDRV_JSON_PARSE_DATA
{
    BAIDU_DRV_JSON_TAG_E           cur_tag;            // ?????y?¨2?a??|ì? xml tag
    int     Object_depth;
	uint8 id[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8 name[BAIDU_DRV_MAX_STRING_LEN+1];
	uint64 size;	
	BOOLEAN bFileExist;
	BOOLEAN bDone;
}MMISRV_BAIDUDRV_JSON_PARSE_DATA;

typedef struct _MMISRV_BAIDUDRV_JSON_PARSE_DELTA_INFO
{
    	BAIDU_DRV_JSON_TAG_E           cur_tag;            // ?????y?¨2?a??|ì? xml tag
	uint8 deltaLink[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8 name[BAIDU_DRV_MAX_STRING_LEN+1];
	BOOLEAN bNeed_update_meta;
	FILEARRAY delete_photo_ary;
}MMISRV_BAIDUDRV_JSON_PARSE_DELTA_INFO;
	
typedef struct _MMISRV_BAIDUDRV_JSON_PARSE_FOLDER_ID_INFO
{
	BAIDU_DRV_JSON_TAG_E           cur_tag;            // ?????y?¨2?a??|ì? xml tag
	BOOLEAN id_from_user;
	BOOLEAN id_from_application;
	uint8 user_id[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8 folder_id[BAIDU_DRV_MAX_STRING_LEN+1];
}MMISRV_BAIDUDRV_JSON_PARSE_FOLDER_ID_INFO;

typedef struct _MMISRV_BAIDUDRV_OPERATE_INFO_T_
{
    BAIDU_DRV_OPERATION_E operation;
    char info_string[BAIDU_DRV_MAX_STRING_LEN];
    uint16 info_strlen;
    char info_url[BAIDU_DRV_MAX_STRING_LEN];
    uint16 id_strlen;
    uint32 upload_buf_size;
    HTTP_REQUEST_ID_T    request_id;
}MMISRV_BAIDUDRV_OPERATE_INFO_T;



typedef struct _MMISRV_BAIDUDRV_REQUEST_ARRAY_T_
{
//    BAIDU_DRV_OPERATION_E operation;
    MMISRV_BAIDUDRV_OPERATE_INFO_T operation_ary[MAX_REQUEST_NUM];
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //¨??¨?¨2???¤?2?¨a?|ì?HTTP???¨???ê¨a?¨o?à??¨?D¨°???HTTP???¨?¨o?à?¨|¨°??a¨¨?¨°a?|ì?
}MMISRV_BAIDUDRV_REQUEST_ARRAY_T;

typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}BAIDU_DRV_DOWNLOAD_THUMBNAIL_SIG_T;

typedef struct
{
    SIGNAL_VARS
    void * filename_ptr;
    uint16   filename_len;
}BAIDU_DRV_DOWNLOAD_PHOTO_SIG_T;

typedef struct
{
    SIGNAL_VARS
    void * filename_ptr;
    uint16   filename_len;
}BAIDU_DRV_DELETE_PHOTO_SIG_T;

typedef struct
{
    SIGNAL_VARS
    char * filename; 
    uint8 file_len;
}BAIDU_DRV_THUMBNAIL_INFO_SIG_T;

typedef struct
{
    SIGNAL_VARS
    uint8 success_num ; 
    uint8 fail_num;
}BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF_SIG_T;

typedef struct
{
    SIGNAL_VARS
    uint8 success_num ; 
    uint8 fail_num;
}BAIDU_DRV_UPLOAD_PHOTO_CNF_SIG_T;

typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}BAIDU_DRV_UPLOAD_PHOTO_SIG_T;

typedef struct _MMISRV_BAIDUDRV_MSG_T_
{
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]; 
    uint8 url[BAIDU_DRV_MAX_STRING_LEN+1]; 	
    uint8 file_len;
    uint16 url_len;
    uint8 success_num ; 
    uint8 fail_num;
    uint8 total_num;	
    uint8 curr_download_num;
    uint64 total_file_size;
    uint64 uploaded_file_size;	
    uint64 downloaded_file_size;	
    BOOLEAN bDone;
	BAIDU_DRIVE_ERROR_E error_type;
    BAIDU_DRV_TYPE_E init_type;
}MMISRV_BAIDUDRV_MSG_T;

typedef struct _MMISRV_BAIDUDRV_DRIVEINFO_T_
{
	uint8 delta_link[BAIDU_DRV_MAX_STRING_LEN+1]; 	
	uint8 folder_id[BAIDU_DRV_MAX_STRING_LEN+1];
	uint8 user_id[BAIDU_DRV_MAX_STRING_LEN+1];
}MMISRV_BAIDUDRV_DRIVEINFO_T;

typedef struct
{
    SIGNAL_VARS
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data;
}BAIDU_DRV_SIG_T;


typedef struct _BAIDU_DRV_ONEDRIVE_UPLOAD
{
	uint8 *buf_ptr;
	uint32  buf_size;
}BAIDU_DRV_UPLOAD_T;


typedef struct MMIAPIBAIDU_USER_INFO
{
   char  id[MAX_BAIDU_USER_INFO_STR_SIZE+1];				
   char avatar_url[MAX_BAIDU_USER_INFO_STR_SIZE+1];
   wchar baidu_name[MAX_BAIDU_USER_INFO_STR_SIZE+1];				
   wchar netdisk_name[MAX_BAIDU_USER_INFO_STR_SIZE+1];
   uint32 vip_type;    //0--comm user, 1- comm vip, 2- supper vip
   uint64 size_total;
   uint64 size_used;
}MMIAPIBAIDU_USER_INFO_T;

typedef enum
{
	BAIDU_DRV_ERROR_NONE,
	BAIDU_DRV_ERROR_NO_MEMORY,
	BAIDU_DRV_ERROR_NO_NETWORK,
	BAIDU_DRV_ERROR_HTTP_ERROR,
	BAIDU_DRV_ERROR_JSON_ERROR,
	BAIDU_DRV_ERROR_UNKNOWN
}BAIDU_DRV_ERROR_E;

typedef uint8 (*BAIDU_DRV_HTTP_REQ_FUNC)(uint8 app_param , void *);


/*****************************************************************************/
//  FUNCTION:      MMISRV_BAIDUDRV_CreateTask
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_CreateTask(uint32 module_id , BAIDU_DRV_TYPE_E type);

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_InitHttpRequest(uint32 module_id);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/

PUBLIC void MMISRV_BAIDUDRV_start_get_thumbnail_info(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_start_upload_file(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRV_BAIDUDRV_start_download_thumbnails(void);

/*****************************************************************************/
//  Description : get json string 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_GetJsonString(void * user_data,const JSON_value* j_value);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_BAIDUDRV_Download_Thumbnail(void);
BOOLEAN MMISRV_BAIDUDRV_Download_Photo(uint8 *filename, uint16 filename_len);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRV_BAIDUDRV_HandleHTTPMsg(PWND app_ptr, uint16 msg_id, DPARAM param);


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_Token_Data( uint8* data_ptr,uint32 data_len );

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Metadata_FileAry_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack(void* ctx, int type, const JSON_value* value);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Upload_single_photo(uint16 * file_patch , uint16 file_path_len);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Download_Original_photo(uint8 * file_name , uint16 file_name_len);


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Upload_multiple_photos(void);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Set_Upload_multiple_photos(FILEARRAY file_array);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_upload_session(uint8 context_id , uint8 http_req_ary_idx , MMISRV_BAIDUDRV_UPLOAD_INFO_T * src_ptr);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_close_session(uint8 context_id , uint8 http_idx , MMISRV_BAIDUDRV_UPLOAD_INFO_T * src_ptr);

/**************************************************************************************/
// Description : send signal to MMI Task  
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC  void MMISRV_BAIDUDRV_SendSignalToMMI(uint32 sig_id,DPARAM param);

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void  MMISRV_BAIDUDRV_Get_Token(int result, char*  buf, uint16 buflen);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Download_photo(void);

/**************************************************************************************/
// Description : Set_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_SYNC_STATE MMISRV_BAIDUDRV_Set_Sync_State(uint8 state);


/**************************************************************************************/
// Description : Get_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_SYNC_STATE MMISRV_BAIDUDRV_Get_Sync_State(void);

/**************************************************************************************/
// Description : Cancel Download
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Cancel_Download(void);  
PUBLIC void MMISRV_BAIDUDRV_Enable_Download(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Cancel_Upload(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY  MMISRV_BAIDUDRV_Create_upload_file_ary(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_append_upload_file(wchar *  , uint8 );  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Close_upload_file_ary(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Check_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_exec_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Enter_Gallery(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_In_Gallery(void);  


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Leave_Gallery(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_AUTO_UPLOAD_STATE_E MMISRV_BAIDUDRV_Get_Auto_upload_status(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_manual_auto_upload_photo(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Destroy(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Load_Upload_File_array(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_Manual_Upload_Photo_Count(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Check_Network_status(BOOLEAN);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Delete_Manual_Upload_File_array(wchar* filename , uint16 file_len);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Get_Connect_State(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Set_Connect_State(BOOLEAN bConnect);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_Network_init_Done(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRV_OPERATION_E MMISRV_BAIDUDRV_Check_Current_auto_upload_request(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY MMISRV_BAIDUDRV_Get_Delete_photo_FileAry(void);


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Get_OneDrive_Info(MMISRV_BAIDUDRV_DRIVEINFO_T *);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Set_OneDrive_Info(MMISRV_BAIDUDRV_DRIVEINFO_T *);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Clean_manual_upload_list(void);

/*****************************************************************************/
//  Description : to open autodemo main menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenQRImgWindow(void);

/*****************************************************************************/
//  Description : to open vip window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenVIPWindow(void);

/*****************************************************************************/
//  Description : to open DiskOpt menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenDiskOptWindow(void);

/*****************************************************************************/
//  Description : is read  the file over
//  Global resource dependence : s_is_readfile_over
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/     
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_IsFileOver(void);

/*****************************************************************************/
//  Description : get file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC MMIFILE_HANDLE MMIAPIBAIDU_DRV_GetFileHandle(void);

/*****************************************************************************/
//  Description : set file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC void MMIAPIBAIDU_DRV_SetFileHandle(MMIFILE_HANDLE file_handle);

/*****************************************************************************/
// 	Description : Is Account Exist
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_IsAccountExist(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Get_baidu_info_req(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIBAIDU_get_access_token
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIBAIDU_get_access_token(void);

/**************************************************************************************/
// Description : get qr image
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_QR_Image(void);
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_Get_Token();
PUBLIC uint8    baidu_drv_get_user_info_req(void);
PUBLIC uint8    baidu_drv_get_disc_size_info_req(void);

/*****************************************************************************/
//  FUNCTION:      MMISRV_BAIDUDRV_get_user_info
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIAPIBAIDU_USER_INFO_T * MMISRV_BAIDUDRV_get_user_info(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIBAIDU_DRV_HandleSignal
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/

PUBLIC MMI_RESULT_E MMIAPIBAIDU_DRV_HandleSignal(MMI_MESSAGE_ID_E  msg_id,   DPARAM  param);

/*****************************************************************************/
//  Description : Create Baidu Dir
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMISRV_BAIDUDRV_CreateDir(void);

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_is_in_BaiduFolder
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_is_in_BaiduFolder(PIC_SOURCE_TYPE type,const wchar *path_name_ptr,  uint16 path_name_len);


/*****************************************************************************/
//  Description : MMIAPIBaidu_CheckVIPAccount
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BAIDU_DRIVE_VIP_STATUS MMISRV_BAIDUDRV_CheckVIPAccount(void);

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_OpenVIPQueryWin
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_OpenVIPQueryWin(void);

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_OpenBaiduLoginQueryWin
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_OpenBaiduLoginQueryWin(void);

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_OpenBaiduLogOutQueryWin
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_OpenBaiduLogoutQueryWin(void);

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_Logout
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Logout(void);


/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_Is_Downloading_Thumbanils
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_Downloading_Thumbanils(void);

PUBLIC uint16 MMISRV_BAIDUDRV_Get_Total_file_num(void);
#endif //MMISRV_BAIDUDRV_EXPORT_H_

