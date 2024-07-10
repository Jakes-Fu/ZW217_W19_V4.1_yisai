/*****************************************************************************
** File Name:      mmi_baidu_api.c                                               *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMISRV_BAIDUDRV_API_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "priority_app.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "in_message.h"
#include "mmibaidu_export.h"
#include "mmi_filemgr.h"
#include "mn_type.h" 
#include "mmiphone_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmi_nv.h"
#include "mmisrvfmm_export.h"
#ifdef MMI_WIFI_SUPPORT  
#include "mmiwifi_export.h"
#endif
#include "mmibaidu_id.h"
#include "mmipicview_export.h"

 /**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#define BAIDUDRIVE_METADATA_STRING_MAX_LEN 1024 

#define MAX_HTTP_CONTEXT_NUM 1
#define MAX_THUMBNAIL_DOWNLOAD_NUM 12

#define HTTP_RESPONSE_CODE_CREATED                              201
#define HTTP_RESPONSE_CODE_OK                                   200
#define HTTP_RESPONSE_CODE_UNAUTHORIZED                         401
#define HTTP_RESPONSE_CODE_NOT_FOUND                            404
#define HTTP_RESPONSE_CODE_FOUND                            302
#if 1 //def  X_SLICING_SUPPORT
#define  S_UPLOAD_SLICING_MAX                 (4*1024*1024)
#define  S_UPLOAD_X_SLICING_MAX                 (256*1024)
DWORD  g_x_bs_file_size = 0;
uint8       s_pre_contest_md5[MMIFILE_FILE_MD5_LEN + 1];
#else
#define  S_UPLOAD_SLICING_MAX                 (512*1024)
#endif
#define S_CONFIG_FILE_LEN_MAX                1024

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL  BOOLEAN s_baidu_task_init_done = FALSE;

LOCAL SCI_MUTEX_PTR         s_baidu_drv_mutex_ptr    = SCI_NULL;

static uint8 s_content_buff[BAIDU_DRV_MAX_STRING_LEN] = {0};

static BAIDU_DRV_UPLOAD_T s_upload_photo_ary = {0} ; 

LOCAL uint8  s_baidu_account_index[MN_DUAL_SYS_MAX]={0};    

LOCAL uint8  s_baidu_pdp_timer_id      = 0;
LOCAL uint8  s_baidu_pdp_retry_timer_id = 0;
LOCAL uint8 s_baidu_pdp_retry_count =0;

LOCAL uint8 s_baidu_drv_access_token_str[BAIDU_TOKEN_MAX_SIZE + 1]={0};
LOCAL uint8 s_baidu_drv_refresh_token_str[BAIDU_TOKEN_MAX_SIZE + 1]={0};
LOCAL uint8 s_baidu_drv_code_str[BAIDU_TOKEN_MAX_SIZE + 1]={0};
LOCAL uint8 s_baidu_drv_session_key_str[BAIDU_TOKEN_MAX_SIZE + 1]={0};
LOCAL uint8 s_baidu_drv_session_secret_str[BAIDU_TOKEN_MAX_SIZE + 1]={0};
LOCAL uint8 s_baidu_drv_upload_id_str[BAIDU_UPLOAD_ID_MAX_SIZE + 1]={0};

#ifdef BAIDU_STORE_U
LOCAL wchar *s_meta_info_file_path = L"D:\\baidu_meta_info.txt";
LOCAL wchar *s_new_meta_info_file_path = L"D:\\baidu_new_meta_info.txt";
LOCAL wchar *s_meta_info_file_name = L"baidu_meta_info.txt";
LOCAL wchar *s_baidu_drv_upload_success_file_path = L"D:\\baidu_drv_upload_success.txt";
LOCAL wchar *s_baidu_drv_snapshot_upload_list_file_path = L"D:\\baidu_drv_upload_list.txt";
LOCAL wchar *s_baidu_drv_manual_upload_list_file_path = L"D:\\baidu_drv_manual_upload_list.txt";
LOCAL wchar *s_baidu_drv_configure_file_path = L"D:\\config.ini";

LOCAL wchar *s_baidu_drv_user_info_path = L"D:\\userinfo_data.txt";
#else
LOCAL wchar *s_meta_info_file_path = L"E:\\baidu_meta_info.txt";
LOCAL wchar *s_new_meta_info_file_path = L"E:\\baidu_new_meta_info.txt";
LOCAL wchar *s_meta_info_file_name = L"baidu_meta_info.txt";
LOCAL wchar *s_baidu_drv_upload_success_file_path = L"E:\\baidu_drv_upload_success.txt";
LOCAL wchar *s_baidu_drv_snapshot_upload_list_file_path = L"E:\\baidu_drv_upload_list.txt";
LOCAL wchar *s_baidu_drv_manual_upload_list_file_path = L"E:\\baidu_drv_manual_upload_list.txt";
LOCAL wchar *s_baidu_drv_configure_file_path = L"E:\\config.ini";

LOCAL wchar *s_baidu_drv_user_info_path = L"E:\\userinfo_data.txt";

#endif
LOCAL FILEARRAY s_baidu_drv_manual_upload_file_array = PNULL;
LOCAL FILEARRAY s_baidu_drv_upload_success_file_array = PNULL;
PUBLIC FILEARRAY s_baidu_drv_snapshot_upload_file_array = PNULL;
LOCAL FILEARRAY s_baidu_drv_snapshot_list_file_array = PNULL;
LOCAL FILEARRAY s_baidu_drv_deleted_list_file_array = PNULL;
LOCAL FILEARRAY s_current_upload_file_array = PNULL;
LOCAL FILEARRAY s_baidu_drv_exist_thumb_file_array = PNULL;
FILEARRAY s_baidu_drv_all_thumb_file_array = PNULL;
LOCAL wchar * s_current_upload_file_path = PNULL;

LOCAL BOOLEAN s_is_process_download_thumbnail = FALSE;
LOCAL BOOLEAN s_is_need_to_refresh_thumbnail = FALSE;
LOCAL BOOLEAN s_is_process_get_delta_link = FALSE;
LOCAL BOOLEAN s_is_in_gallery = FALSE;

LOCAL BAIDU_DRV_OPERATION_E s_baidu_resume_operation = BAIDU_DRV_OPERAION_MAX;
LOCAL BLOCK_ID g_delete_baidu_drv_file_task_id = SCI_INVALID_BLOCK_ID;

static MMISRV_BAIDUDRV_REQUEST_ARRAY_T http_request_operation_array[MAX_HTTP_CONTEXT_NUM] = {0};
LOCAL uint8 s_current_download_thumbnail_count = 0;
LOCAL BOOLEAN  s_is_process_pdp_activate=FALSE;

LOCAL const char* const s_baidu_drv_json_tag_arr[] = 
{
    "",
    "access_token",
    "refresh_token",
    "expires_in",
    "session_key",    
    "session_secret",        
    "avatar_url",		
    "baidu_name",
    "total",
    "used",
    "netdisk_name",
    "vip_type",
    "qrcode_url",
    "device_code",
    "uploadid",
    "return_type",
    "errno",
    "error_code",
    "list",
    "fs_id",
    "cursor",
    "has_more",
    "server_filename"
};

static MMISRV_BAIDUDRV_METADATA * s_baidu_drv_metadata_ptr = PNULL;
static MMISRV_BAIDUDRV_UPLOAD_INFO_T * s_baidu_drv_upload_photo_ptr = PNULL;
//static MMISRV_BAIDUDRV_DOWNLOAD_INFO_T *s_current_download_ptr = PNULL;
static MMISRV_BAIDUDRV_DOWNLOAD_INFO_T *s_current_download_photo_ptr = PNULL;
static MMISRV_BAIDUDRV_JSON_PARSE_DATA s_baidu_drv_photo_json_parse_info = {0};
static uint8 s_baidu_drv_photo_info_src_name[BAIDU_DRV_MAX_STRING_LEN+1] = {0};
PUBLIC uint8 s_baidu_drv_qr_code_url[BAIDU_DRV_MAX_STRING_LEN+1] = {0};
static uint8 s_download_thumbnail_total_count =0;
static uint8 s_download_thumbnail_fail_count =0;
static uint8 s_download_thumbnail_success_count =0;
LOCAL uint8 s_download_thumbnail_timer = 0;
LOCAL uint32 s_download_thumbnail_system_time = 0;
LOCAL uint8 s_baidu_drv_check_user_info_timer = 0;
LOCAL uint8 s_baidu_drv_refresh_token_timer = 0;

uint32 s_tocken_expires_in = 0;
MMI_APPLICATION_T g_baidu_drv = {0};
extern uint32 g_current_module_id;
LOCAL uint32 s_baidu_net_id = 0;
LOCAL MN_DUAL_SYS_E s_baidu_cur_sys_num   = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_b_is_cancel_upload_photo = FALSE;
LOCAL BOOLEAN s_b_is_cancel_download_photo = FALSE;
LOCAL BAIDU_DRIVE_AUTO_UPLOAD_STATE_E s_auto_upload_status =BAIDU_DRIVE_AUTO_UPLOAD_NONE;
LOCAL uint32 g_baidu_upload_errno = 0;
LOCAL uint32 g_baidu_upload_error_code = 0;

//#define DEBUG_REFRESH 1

#ifdef DEBUG_REFRESH
LOCAL BOOLEAN s_set_refresh_flag = TRUE;
#endif

extern BAIDU_DRV_TYPE_E s_baidu_drv_init_from;
LOCAL uint8 s_query_qr_timer_id = 0;

PUBLIC MMIAPIBAIDU_USER_INFO_T s_baidu_user_info = {0};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//Get Drive
//#define BAIDUDRIVE_DEVICE_CODE_URL "https://openapi.baidu.com/oauth/2.0/device/code?response_type=device_code&client_id=rMMjVCPNMPqOAq6v4UZluqS34Q0QcpUV&scope=basic,netdisk"
//#define BAIDUDRIVE_AUTH_CODE_URL "https://openapi.baidu.com/oauth/2.0/authorize?response_type=code&client_id=rMMjVCPNMPqOAq6v4UZluqS34Q0QcpUV&redirect_uri=http://tankband.orgfree.com/redirect_code_1.php&scope=basic,netdisk&display=mobile&force_login=1"
#define BAIDUDRIVE_DEVICE_CODE_URL "https://openapi.baidu.com/oauth/2.0/device/code?response_type=device_code&client_id=GEGoM7V7YrP9bYEZLwuPUGQ6MOyTpsIv&scope=basic,netdisk"
#define BAIDUDRIVE_AUTH_CODE_URL "https://openapi.baidu.com/oauth/2.0/authorize?response_type=code&client_id=GEGoM7V7YrP9bYEZLwuPUGQ6MOyTpsIv&redirect_uri=http://tankband.orgfree.com/redirect_code_1.php&scope=basic,netdisk&display=mobile&force_login=1"
#define BAIDUDRIVE_GET_TOKEN_URL_PART1 "https://openapi.baidu.com/oauth/2.0/token?grant_type=device_token&code="
//#define BAIDUDRIVE_GET_TOKEN_URL_PART2 "&client_id=rMMjVCPNMPqOAq6v4UZluqS34Q0QcpUV&client_secret=c1L8lwsqVnP4ZxIRUkNsBjrxkfBDQgqW"
#define BAIDUDRIVE_GET_TOKEN_URL_PART2 "&client_id=GEGoM7V7YrP9bYEZLwuPUGQ6MOyTpsIv&client_secret=PAxea80XZU2IgqeEYBoID8Uy9tOyX6ZE"
#define BAIDUDRIVE_GET_USER_INFO_URL "https://pan.baidu.com/rest/2.0/xpan/nas?method=uinfo&access_token="
#define BAIDUDRIVE_GET_QUOTA_INFO_URL "https://pan.baidu.com/api/quota?checkfree=1&checkexpire=1&access_token="

/*refresh*/
#define BAIDUDRIVE_REFRESH_TOKEN_URL_PART1 "https://openapi.baidu.com/oauth/2.0/token?grant_type=refresh_token&refresh_token="

/*upload start*/
#define BAIDUDRIVE_UPLOAD_START_URL_PART1 "http://pan.baidu.com/rest/2.0/xpan/file?method=precreate&access_token="
#define BAIDUDRIVE_UPLOAD_START_URL_PART2  "&path=/apps/baipan/"
#define BAIDUDRIVE_UPLOAD_START_URL_PART3 "&isdir=0&autoinit=1&rtype=3&block_list=[\""
uint32  G_UPLOAD_START_return_type = 0;
/*upload file */
#define  BAIDUDRIVE_UPLOAD_FILE_URL_PART1 "https://d.pcs.baidu.com/rest/2.0/pcs/superfile2?access_token="
#define  BAIDUDRIVE_UPLOAD_FILE_URL_PART2   "&method=upload&type=tmpfile&path=/apps/baipan/"
#define  BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE  "multipart/form-data; boundary=--------------------------054345279095770762450212"
#define  BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY   "--------------------------054345279095770762450212"
#define  BAIDUDRIVE_UPLOAD_MULTIPART_HEAD1 "\r\nContent-Disposition: form-data; name=\"file\"; filename=\""
#define  BAIDUDRIVE_UPLOAD_MULTIPART_HEAD2 "\"\r\nContent-Type: char[]\r\n\r\n"
/*create file*/
#define BAIDUDRIVE_UPLOAD_END_URL_PART1  "http://pan.baidu.com/rest/2.0/xpan/file?method=create&access_token="
#define BAIDUDRIVE_UPLOAD_END_URL_PRE_PATH  "/apps/baipan/"
#define BAIDUDRIVE_UPLOAD_END_BODY_H1 "\r\nContent-Disposition: form-data; name=\""
#define BAIDUDRIVE_UPLOAD_END_BODY_H2 "\"\r\nContent-Type: string\r\n\r\n"

/*thumb file*/
#define BAIDUDRIVE_UPLOAD_THUMB_URL_PART1 "http://pan.baidu.com/rest/2.0/xpan/file?method=imagelist&access_token="
#define BAIDUDRIVE_UPLOAD_THUMB_URL_PART2 "&parent_path=/apps/baipan/&web=1"

/*getfile info*/
#define BAIDUDRIVE_GETFILEINFO_URL_PART1 "http://pan.baidu.com/rest/2.0/xpan/multimedia?method=filemetas&access_token="
#define BAIDUDRIVE_GETFILEINFO_URL_PART2 "&thumb=0&dlink=1&extra=0&fsids="
//uint8 FS_ID[MMIFILE_FS_ID_LEN_MAX+1] = {0};
uint8 *s_dlink_ptr= NULL;

/*get categorylist info*/
#define BAIDUDRIVE_GETCATEGORYLIST_URL_PART1 "http://pan.baidu.com/rest/2.0/xpan/multimedia?method=categorylist&access_token="
#define BAIDUDRIVE_GETCATEGORYLIST_URL_PART2 "&category=1,3&parent_path=/apps/baipan/&order=time&desc=1&limit=100&start="
LOCAL BOOLEAN s_is_process_get_categorylist = FALSE;
uint16  s_get_category_list_cusor = 0;
uint8    s_get_category_list_has_more = 0;
uint16  s_total_file_num = 0;
uint16  s_invalid_name_cnt = 0;
MMIFILE_HANDLE s_meta_data_file_handle = 0;
MMIFILE_HANDLE s_new_meta_data_file_handle = 0;

/*delete photo*/
#define BAIDUDRIVE_DELETE_URL_PART  "http://pan.baidu.com/rest/2.0/xpan/files?method=filemanager&opera=delete&access_token="
uint8 *g_delete_usr_body = NULL;
#define  DELETE_PHOTO_NAME_LIST_MAX_LEN  1024*5
uint8 *g_delete_photo_name_list = NULL;
uint16 g_delete_photo_num = 0;
//LOCAL BOOLEAN s_is_process_delete_photo = FALSE;
//LOCAL BOOLEAN s_continue_delete_photo = FALSE;
//#define ONCE_DELETE_PHONE_NUN_MAX  12

/*****************************************************************************/
//  Description : GetSignalStruct
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void HandleHttpErrorInd(MMISRV_BAIDUDRV_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_thumbnail_info_ind_func(uint8 app_param , void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_download_thumbnail_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_create_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_upload_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_close_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_create_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_close_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_download_photo_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8    baidu_drv_get_user_info_req(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_user_info_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : baidu_drv_get_user_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_user_info_Json_callback(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8    baidu_drv_get_disc_size_info_req(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_disc_size_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : baidu_drv_get_baidu_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_disc_size_Json_callback(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_req(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_cnf_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_req(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_cnf_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : init json config
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void baidu_drv_InitjsonConfig(JSON_config* config_ptr,
                                    JSON_parser_callback callback,
                                    MMISRV_BAIDUDRV_JSON_PARSE_DATA *src_data_ptr);

/*****************************************************************************/
//  Description : parse json char one by one
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_ParseJsonChar(JSON_parser jp_ptr , uint8* data_ptr , uint32 data_len);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 baidu_drv_get_avaliable_req_index( uint8* oper_idx);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void baidu_drv_download_thumbnail_req(uint8 , uint8);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_thumbnail_info_cnf_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_download_photo_cnf_func(uint8 app_param, void * param);


/*****************************************************************************/
//  Description : baidu_GetCodeCallback
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void baidu_GetCodeCallback(void);

/*****************************************************************************/
//  Description : baidu_drv_device_code_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 baidu_drv_device_code_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_token_ind_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_token_cnf_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : baidu_drv_get_baidu_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_device_code_Json_callback(void* ctx, int type, const JSON_value* value);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_Http_all_request(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_meta_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_upload_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_download_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Destroy_Http_all_request(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_all_data(void);

LOCAL void Clean_Categorylist_data_info(void);

/*****************************************************************************/
//  Description :HandlePdpRetryTimer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpRetryTimer(   uint8 timer_id,  uint32 param   );

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Handle_JSON_Is_Tag_Equal(uint8 index , const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint8 Generate_Download_thumbnail_node_by_JSON(const uint8* data ,uint32 data_len, uint16 num );

/*****************************************************************************/
// 	Description : Handle_UserInfo_timers
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Handle_UserInfo_timer(uint8  timer_id, uint32 param);

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleDownloadThumbnail_timer(uint8  timer_id, uint32 param);

/*****************************************************************************/
//  Description : Free Header
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader, uint32 header_num);
PUBLIC uint16 MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind( uint8* data_ptr,uint32 data_len );
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack(void* ctx, int type, const JSON_value* value);
LOCAL uint8    baidu_drv_auto_upload_create_data_ind_func(uint8 app_param, void * param);
/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Remove_File_From_UploadFileAry(uint16 * file_path , uint16 file_path_len);
LOCAL uint8    baidu_drv_auto_upload_close_data_ind_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_get_categorylist_data_ind_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_download_thumbnail_info_cnf_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_get_categorylist_cnf_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_get_filemetas_data_ind_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_get_filemetas_cnf_func(uint8 app_param, void * param);
LOCAL uint8    baidu_drv_delete_file_cnf_func(uint8 app_param, void * param);
PUBLIC void  MMISRV_BAIDUDRV_Delete_Photo(uint8  *filename_list,  uint16  filenum);
PUBLIC int MMIAPIBAIDU_refresh_access_token(void);
LOCAL uint8    baidu_drv_refresh_token_ind_func(uint8 app_param, void * param);
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Close_Session_CallBack(void* ctx, int type, const JSON_value* value);
LOCAL uint8    baidu_drv_auto_upload_session_data_ind_func(uint8 app_param, void * param);
BOOLEAN MMISRV_BAIDUDRV_Check_Device_Space(uint32 writelen);
BAIDU_DRV_HTTP_REQ_FUNC http_data_get_ind_function[] = 
{
	PNULL,											//BAIDU_DRV_OPER_NOT_INIT                                              
	PNULL ,											//BAIDU_DRV_OPER_NO_OPERATION
	baidu_drv_device_code_ind_func,					//BAIDU_DRV_DEVICE_CODE, 
	baidu_drv_get_qrcode_img_ind_func,					//BAIDU_DRV_OPER_GET_QR_IMG
	baidu_drv_get_token_ind_func,					//BAIDU_DRV_OPER_GET_TOKEN
	baidu_drv_get_thumbnail_info_ind_func,		//BAIDU_DRV_OPER_GET_THUMBNAIL_INFO
	PNULL,			//BAIDU_DRV_OPER_GET_THUMBNAIL_URL
	baidu_drv_download_thumbnail_req_func,		//BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL
	PNULL,											//BAIDU_DRV_OPER_CREATE_SESSION
	PNULL,											//BAIDU_DRV_OPER_UPLOAD_SESSION
	PNULL,											//BAIDU_DRV_OPER_CLOSE_SESSION
	PNULL ,     //BAIDU_DRV_OPER_DOWNLOAD_PHOTO_LOCATION
	baidu_drv_download_photo_req_func,   //    BAIDU_DRV_OPER_DOWNLOAD_PHOTO
	baidu_drv_auto_upload_create_data_ind_func ,	//BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION
	baidu_drv_auto_upload_session_data_ind_func,	//BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION
	baidu_drv_auto_upload_close_data_ind_func,	  //BAIDU_DRV_OPER_AUTO_CLOSE_SESSION  
	baidu_drv_get_user_info_ind_func ,		//BAIDU_DRV_OPER_GET_USER_INFO
	baidu_drv_get_disc_size_ind_func ,		//BAIDU_DRV_OPER_GET_DISK_QUOTA
	baidu_drv_get_avatar_img_ind_func,		//BAIDU_DRV_OPER_GET_AVATAR_IMG
	baidu_drv_get_categorylist_data_ind_func,         //BAIDU_DRV_OPER_GET_CATEGORYLIST
	baidu_drv_get_filemetas_data_ind_func,                //BAIDU_DRV_OPER_GET_FILE_METAS
	PNULL,                                                                          //BAIDU_DRV_OPER_DELETE_PHOTO
	baidu_drv_refresh_token_ind_func,                               //BAIDU_DRV_OPER_REFRESH_TOKEN
	PNULL,											//BAIDU_DRV_OPERAION_MAX
};  

BAIDU_DRV_HTTP_REQ_FUNC http_get_cnf_function[] = 
{
	PNULL,											//BAIDU_DRV_OPER_NOT_INIT
	PNULL ,											//BAIDU_DRV_OPER_NO_OPERATION
	PNULL,											//BAIDU_DRV_DEVICE_CODE, 	
	baidu_drv_get_qrcode_img_cnf_func,					//BAIDU_DRV_OPER_GET_QR_IMG	
	baidu_drv_get_token_cnf_func,					//BAIDU_DRV_OPER_GET_TOKEN	
	baidu_drv_get_thumbnail_info_cnf_func ,		//BAIDU_DRV_OPER_GET_THUMBNAIL_INFO
	PNULL,			//BAIDU_DRV_OPER_GET_THUMBNAIL_URL
	baidu_drv_download_thumbnail_info_cnf_func,		//BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL
	baidu_drv_create_session_req_func,			//BAIDU_DRV_OPER_CREATE_SESSION
	baidu_drv_upload_session_req_func,			//BAIDU_DRV_OPER_UPLOAD_SESSION
	baidu_drv_close_session_req_func,				//BAIDU_DRV_OPER_CLOSE_SESSION
	PNULL ,     //BAIDU_DRV_OPER_DOWNLOAD_PHOTO_LOCATION
	baidu_drv_download_photo_cnf_func, //    BAIDU_DRV_OPER_DOWNLOAD_PHOTO
	baidu_drv_auto_upload_create_session_req_func,	//BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION  13
	baidu_drv_auto_upload_session_req_func,			//BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION
	baidu_drv_auto_upload_close_session_req_func,	//BAIDU_DRV_OPER_AUTO_CLOSE_SESSION    
       PNULL , 										//BAIDU_DRV_OPER_GET_USER_INFO
	PNULL , 									//BAIDU_DRV_OPER_GET_DISK_QUOTA        
       baidu_drv_get_avatar_img_cnf_func , 										//BAIDU_DRV_OPER_GET_AVATAR_IMG
	baidu_drv_get_categorylist_cnf_func,                 //BAIDU_DRV_OPER_GET_CATEGORYLIST
	baidu_drv_get_filemetas_cnf_func,             //BAIDU_DRV_OPER_GET_FILE_METAS
	baidu_drv_delete_file_cnf_func,                    //BAIDU_DRV_OPER_DELETE_PHOTO
	PNULL,                                                                  //BAIDU_DRV_OPER_REFRESH_TOKEN
	PNULL,											//BAIDU_DRV_OPERAION_MAX
};  


/*****************************************************************************/
//  FUNCTION:      MMISRV_CAMERARoll_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_InitModule(void)
{
	g_baidu_drv.ProcessMsg = MMISRV_BAIDUDRV_HandleHTTPMsg;
	s_baidu_drv_mutex_ptr = SCI_CreateMutex("Mutex for camera roll callback protection", SCI_INHERIT);
}

/*****************************************************************************/
//  FUNCTION:      MMISRV_BAIDUDRV_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_InitHttpRequest(uint32 module_id)
{
       uint8 i=0;
	BOOLEAN bHas_not_init = FALSE;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T sig_data={0};	
	
	//init HTTP !!!
	SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_InitHttpRequest-----");
	
	init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
	SCI_ASSERT(PNULL != init_param_ptr);
	init_param_ptr->is_cookie_enable = FALSE;
	//SCI_TRACE_LOW("[cookie] MMISRV_BAIDUDRV_InitHttpRequest init_param_ptr->is_cookie_enable = FALSE;");
	init_param_ptr->is_cache_enable = FALSE;
	s_baidu_task_init_done = FALSE;

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_InitHttpRequest----- i = %d" , i);

		if( 0 == http_request_operation_array[i].context_id)
		{
			HTTP_InitRequest(module_id, i, init_param_ptr);
			bHas_not_init = TRUE;
		}
	}
		
	if(FALSE == bHas_not_init) //means all http context are requested before
	{
		SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_InitHttpRequest all http context are requested before");

		if(BAIDU_DRV_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO != s_baidu_drv_init_from)
		{
			sig_data.init_type = s_baidu_drv_init_from;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_INIT_CNF , &sig_data);		
			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF SEND MSG_BAIDU_DRV_INIT_CNF !!!");			
		}
		else
		{
			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF MMISRV_BAIDUDRV_exec_auto_upload_photo !!!");					  
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}		
		s_baidu_task_init_done = TRUE;
	}

	SCI_FREE(init_param_ptr);
}

/*****************************************************************************/
//  Description: Get Mutex
//  Global resource dependence:
//  Author: 
/*****************************************************************************/
LOCAL void BAIDU_DRV_GetMutex(SCI_MUTEX_PTR mutex)
{
    SCI_TRACE_LOW("[baidu]BAIDU_DRV_GetMutex");

    if( NULL == mutex )
    {
	SCI_TRACE_LOW("[baidu]BAIDU_DRV_GetMutex : Mutex is NULL!");

        return; 
    }
    
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
}
/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author: 
/*****************************************************************************/
LOCAL void BAIDU_DRV_PutMutex(SCI_MUTEX_PTR mutex)
{
    SCI_TRACE_LOW("[baidu]BAIDU_DRV_PutMutex");
    if( NULL == mutex )
    {
	 SCI_TRACE_LOW("[baidu]BAIDU_DRV_PutMutex : Mutex is NULL!");
        return; 
    }
    SCI_PutMutex( mutex );  
}

PUBLIC void MMISRV_BAIDUDRV_get_categorylist_info(void)
{
    uint8 avaliable_request_idx,context_id ;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;
    uint16 len = 0;
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    uint8 temp_uri[1024] = {0};

#ifdef WIN32
	baidu_drv_get_categorylist_cnf_func(0,PNULL);
	return;
#endif


    SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_get_categorylist_info");
    if(TRUE == s_is_process_get_categorylist)
    {
        SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_get_categorylist_info previous get categorylist still ongoing  SKIP !!!");                
        return ;
    }
	/*if uploading, pause upload process*/
	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		SCI_TRACE_LOW("[baidu]: baidu_drv_download_thumbnail_info_cnf_func need download thumbnail !! Pause auto upload !!!");
		Clean_upload_data_info();			
	}

    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
    {
       // SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_get_categorylist_info start get categorylist");
        get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

        if(get_param_ptr == NULL)
        {
        	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_get_categorylist_info error code MSG_BAIDU_DRV_ERROR_NOTIFY get_param_ptr == NULL" );
        	baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
        	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        	return;
        }	

        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_GET_CATEGORYLIST;
           
        memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T)); 

        get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
        get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        get_param_ptr->need_net_prog_ind = FALSE;
        get_param_ptr->is_use_relative_url = TRUE;
 
        sprintf(temp_uri, "%s%s%s%d", 
                   BAIDUDRIVE_GETCATEGORYLIST_URL_PART1, 
                   s_baidu_drv_access_token_str, 
                   BAIDUDRIVE_GETCATEGORYLIST_URL_PART2,
                   s_get_category_list_cusor);

        len=strlen(temp_uri);
        get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
        memset(get_param_ptr->uri.uri_ptr, 0, len+1);
        strcpy(get_param_ptr->uri.uri_ptr, temp_uri);
        
        //SCI_TRACE_LOW("uri:%s",  get_param_ptr->uri.uri_ptr);
        if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
        {
        	SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
              //s_is_process_get_categorylist = TRUE;
        }

        SCI_FREE(get_param_ptr->uri.uri_ptr);
        SCI_FREE(get_param_ptr);
    }
	
    return;	
}

PUBLIC int MMISRV_BAIDUDRV_JsonParse_Get_Categorylist_CallBack(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;
	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

       //SCI_TRACE_LOW("type:%d, tag:%d, value:%s ", type,baidu_drv_data_ptr->cur_tag, value);
	switch(type) 
	{
	case JSON_T_INTEGER:         // 5
	{
		if (JSON_TAG_CURSOR == baidu_drv_data_ptr->cur_tag)
		{
                  s_get_category_list_cusor = value->vu.integer_value;
		    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Categorylist_CallBack cusor: %d " , s_get_category_list_cusor);			
		}
              else if(JSON_TAG_HAS_MORE == baidu_drv_data_ptr->cur_tag)
              {
                    s_get_category_list_has_more = value->vu.integer_value;
                    //SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Categorylist_CallBack has_more: %d " , s_get_category_list_has_more);
              }
	}
	
	break;
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}

PUBLIC uint16 MMISRV_BAIDUDRV_Json_Parse( uint8* data_ptr,uint32 data_len , JSON_parser_callback callback)
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	   
	if(PNULL == data_ptr || 0 == data_len)
				return FALSE;

	SCI_TRACE_LOW("[baidu] Enter MMISRV_BAIDUDRV_Json_Parse");

	baidu_drv_InitjsonConfig(&jc,callback,&input_data);

	jp_ptr = new_JSON_parser(&jc); 

	if(PNULL == jp_ptr)
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Json_Parse PNULL == jp_ptr");
		return FALSE;
	}
	if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Json_Parse parse error!");
	}
	delete_JSON_parser(jp_ptr); 

	return TRUE;
}
LOCAL uint8    baidu_drv_get_categorylist_data_ind_func(uint8 app_param, void * param)
{
       MMIFILE_DEVICE_E    dev = MMI_DEVICE_SDCARD;
	wchar            default_dir[MMIFILE_FILE_NAME_MAX_LEN] = {0};
	uint16           device_len = MMIFILE_FILE_NAME_MAX_LEN;
	
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
    	MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;
	int32 temp_pointer = 0;

#ifdef DEBUG_REFRESH

	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_categorylist_data_ind_func refresh token !!!");
		Reset_Http_all_request();
		s_baidu_resume_operation = BAIDU_DRV_OPER_GET_CATEGORYLIST; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif
    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_categorylist_data_ind_func");   

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_categorylist_data_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

 	MMISRV_BAIDUDRV_Json_Parse(data_ptr, data_len, MMISRV_BAIDUDRV_JsonParse_Get_Categorylist_CallBack);

	if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
	{
		Clean_meta_data_info();

		if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
		{
			MMIAPIFMM_DeleteFile(s_meta_info_file_path , PNULL);
		}
		
		s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			return FALSE;		
		}
	}
	else
	{
		MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_CUR);
	}

       temp_pointer  =MMIAPIFMM_GetFilePointer(s_meta_data_file_handle, SFS_SEEK_BEGIN);
	{
		file_ret = MMIAPIFMM_WriteFile(
			s_meta_data_file_handle, 
			cnf->data_ptr, 
			cnf->data_len, 
			&write_size, 
			NULL
			);

		if(SFS_ERROR_NONE == file_ret)
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_get_categorylist_data_ind_func meta update success !!");
		}
		if(SFS_ERROR_NO_SPACE == file_ret)
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_get_categorylist_data_ind_func meta update fail !!, NO SPACE, delete thumbnail and download.");

			/*delete all file in thumb, download file*/
			dev = MMIAPIFMM_GetValidateDevice(dev);
			device_len = MMIAPIFMM_GetDevicePathLen(dev);
			 MMIAPICOM_Wstrncpy(default_dir, MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MIN(device_len, MMIFILE_FILE_NAME_MAX_LEN));
			 if(device_len < MMIFILE_FILE_NAME_MAX_LEN)
			 {
			        default_dir[device_len] = ':';
			  }
			 
			//MMIAPIFMM_DeleteTreeEx(default_dir, MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL, FALSE);

                     MMIAPIFMM_DeleteTreeEx(default_dir, MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD, FALSE);
			
                     MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, temp_pointer, SFS_SEEK_BEGIN);
                     file_ret = MMIAPIFMM_WriteFile(
				s_meta_data_file_handle, 
				cnf->data_ptr, 
				cnf->data_len, 
				&write_size, 
				NULL
			);
					 
			if(SFS_ERROR_NONE == file_ret)
			{
				SCI_TRACE_LOW("[baidu]baidu_drv_get_categorylist_data_ind_func meta update success TOO !!");
			}
			else if(SFS_ERROR_NO_SPACE == file_ret)
			{
				baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
				MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
				SCI_TRACE_LOW("[baidu]baidu_drv_get_categorylist_data_ind_func meta update fail !!, NO SPACE ENOUGH");
				return FALSE;
			}
		}
	}

    //SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_categorylist_data_ind_func");   
    
    return TRUE;
}

PUBLIC uint16 MMISRV_BAIDUDRV_Get_Total_file_num()
{
      return s_total_file_num;
}

LOCAL uint8    baidu_drv_get_categorylist_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    	MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	MMISRV_BAIDUDRV_METADATA *tmp_ptr = PNULL;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;
	uint32 file_size = 0 , read_size = 0 ;
	uint8 * buff = PNULL ;
	BOOLEAN retVal = FALSE;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_categorylist_cnf_func");  
		
       s_is_process_get_categorylist = FALSE;
       /*1. check has_more*/
       if(1 == s_get_category_list_has_more)
       {
            s_get_category_list_has_more = 0;
            MMISRV_BAIDUDRV_get_categorylist_info();
            return TRUE;
       } 
       
       /*2. save the thumb list*/
       if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
       {
              
        	if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        	{
        		s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
        		if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
        		{
        			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
        			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        			return FALSE;		
        		}
        	}
            
       }
       else
        {
            MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_BEGIN);
        }
	//retVal = MMIAPIFMM_RenameFile((const wchar *)s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path), s_meta_info_file_name , MMIAPICOM_Wstrlen(s_meta_info_file_name));

	//if(FALSE == retVal)
	///{
	//	SCI_TRACE_LOW("[baidu] baidu_drv_get_categorylist_cnf_func MMIAPIFMM_RenameFile FAIL !!!!");
	//}

	//s_is_process_get_delta_link = FALSE;

       /*3. download the before n thumb.*/
       file_size = MMIAPIFMM_GetFileSize(s_meta_data_file_handle);
	buff = SCI_ALLOC_APPZ(file_size);
    
	//SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_categorylist_cnf_func, FILE_SIZE:%d,", file_size);   
	if(PNULL == buff)
	{
		SCI_TRACE_LOW("[baidu]:baidu_drv_get_categorylist_cnf_func --- (PNULL == buff)"); 
		return 0;
	}

	//MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_BEGIN);

	file_ret =  MMIAPIFMM_ReadFile(
	    s_meta_data_file_handle, 
	    buff, 
	    file_size, 
	    &read_size, 
	    NULL
	    );
//SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_categorylist_cnf_func, FILE_SIZE:%d,%d", file_size,read_size);   
	if(SFS_ERROR_NONE == file_ret)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_categorylist_cnf_func read meta success !!");
	}
	
	MMIAPIFMM_CloseFile(s_meta_data_file_handle);
	s_meta_data_file_handle = SFS_INVALID_HANDLE ;
	if(0 < s_get_category_list_cusor)
	{
		Generate_Download_thumbnail_node_by_JSON(buff , file_size, 12);
	}
	else
	{
	        Clean_meta_data_info();
               s_current_download_thumbnail_count = 0;
	       if(s_baidu_drv_exist_thumb_file_array)
		{
			MMIAPIFILEARRAY_Destroy(&s_baidu_drv_exist_thumb_file_array);
			s_baidu_drv_exist_thumb_file_array = PNULL;
		}
		if(s_baidu_drv_all_thumb_file_array)
		{
			MMIAPIFILEARRAY_Destroy(&s_baidu_drv_all_thumb_file_array);
			s_baidu_drv_all_thumb_file_array = PNULL;
		}
		s_download_thumbnail_total_count =  s_total_file_num = (s_get_category_list_cusor - s_invalid_name_cnt);
             	baidu_drv_msg_data.total_num = s_download_thumbnail_total_count ;
	       baidu_drv_msg_data.curr_download_num = s_current_download_thumbnail_count;
	
	       MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY , &baidu_drv_msg_data);
	}

#ifdef WIN32
	s_download_thumbnail_total_count =  s_total_file_num = (s_get_category_list_cusor - s_invalid_name_cnt);
	baidu_drv_msg_data.total_num = s_download_thumbnail_total_count ;
	baidu_drv_msg_data.curr_download_num = s_current_download_thumbnail_count;

	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY , &baidu_drv_msg_data);
#endif

       SCI_FREE(buff);
							
	if(PNULL != s_baidu_drv_metadata_ptr)
	{     				
		s_current_download_thumbnail_count == MMISRV_BAIDUDRV_start_download_thumbnails();
    
		if(0 == s_current_download_thumbnail_count)
		{
			if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
			{
				if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
				MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
				{
					MMISRV_BAIDUDRV_exec_auto_upload_photo();
				}
			}
		}
	}
	else if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
	{
		if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
		   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
		{
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}
	}

	s_download_thumbnail_total_count =  s_total_file_num = (s_get_category_list_cusor - s_invalid_name_cnt);
	baidu_drv_msg_data.total_num = s_download_thumbnail_total_count ;
	baidu_drv_msg_data.curr_download_num = s_current_download_thumbnail_count;

	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY , &baidu_drv_msg_data);

	s_download_thumbnail_fail_count =0;
	s_download_thumbnail_success_count =0;
	s_is_process_download_thumbnail = TRUE;


	s_download_thumbnail_system_time = MMIAPICOM_GetCurTime();
	s_is_need_to_refresh_thumbnail = FALSE;
       s_get_category_list_cusor = 0;

	return TRUE;
}
/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_start_get_thumbnail_info(void)
{
    return;	
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_start_upload_file(void)
{
	//MMI_RESULT_E        result = MMI_RESULT_FALSE;
	//HTTP_INIT_CNF_SIG_T signal = {0};
	//HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id ;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);
	
    if(0xFF != context_id)
    {
		SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_start_upload_file !!!!");
		post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));
		
		if(post_param_ptr == NULL) 
		{
			SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_start_upload_file error code MSG_BAIDU_DRV_ERROR_NOTIFY post_param_ptr == NULL" );
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			return;
		}	
		
		if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
		   BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION;
		}
		else
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_CREATE_SESSION;
		}
		
		memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
		
		post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		post_param_ptr->need_net_prog_ind = FALSE;
              post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		post_param_ptr->need_net_prog_ind = FALSE;
		post_param_ptr->is_use_relative_url = TRUE;

		//post_param_ptr->recv_timeout = 5*60*1000; //10 min 
              len = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
		post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(post_param_ptr->uri.uri_ptr, 0, len+1);
		strcpy(post_param_ptr->uri.uri_ptr,s_baidu_drv_upload_photo_ptr->upload_url);
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id,post_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_PostRequest success !!!!");
		}
#endif

		SCI_FREE(post_param_ptr->uri.uri_ptr);
		//FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
		//SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
		//post_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(post_param_ptr);
	}
	return;	

}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_upload_session(uint8 context_id , uint8 http_req_ary_idx , MMISRV_BAIDUDRV_UPLOAD_INFO_T * src_ptr)
{
    HTTP_POST_PARAM_T* post_param_ptr= NULL;
    uint16 len = 0;
    uint32 read_bytes = 0;
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    uint32 multipart_offset = 0;
	
    SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_upload_session !!!!");
    post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

    if(post_param_ptr == NULL)
    {
        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_upload_session error code post_MSG_BAIDU_DRV_ERROR_NOTIFY param_ptr == NULL" );
        baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
        MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        return;
    }	

    memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity

    post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    post_param_ptr->need_net_prog_ind = FALSE;

   //SCI_TRACE_LOW("upload test: file:%s", src_ptr->file_path);
    //handle range value 
#if 1 //def  X_SLICING_SUPPORT
    read_bytes = MIN(S_UPLOAD_X_SLICING_MAX, src_ptr->remain_file_size);
#else
    read_bytes = MIN(S_UPLOAD_SLICING_MAX, src_ptr->remain_file_size);
#endif
    SCI_TRACE_LOW("read_bytes:%d", read_bytes);
    if(PNULL ==s_upload_photo_ary.buf_ptr)
    {
        s_upload_photo_ary.buf_ptr = SCI_ALLOC_APPZ(read_bytes+1024);
        if(PNULL !=s_upload_photo_ary.buf_ptr)
        {
            s_upload_photo_ary.buf_size = read_bytes+1024;
            SCI_TRACE_LOW("[baidu] ENTER baidu_drv_send_session_http_req s_upload_photo_ary.buf_size %d" , s_upload_photo_ary.buf_size);
        }
    }
    else
    {
        //compose post request 
        SCI_MEMSET(s_upload_photo_ary.buf_ptr , 0x00 , s_upload_photo_ary.buf_size);
    }
SCI_TRACE_LOW("sended_file_size:%d", src_ptr->sended_file_size);
    MMIAPIFMM_SetFilePointer(src_ptr->file_handle ,src_ptr->sended_file_size ,MMIFILE_SEEK_BEGIN);
    sprintf(s_upload_photo_ary.buf_ptr ,"--%s" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY)+2);
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,BAIDUDRIVE_UPLOAD_MULTIPART_HEAD1);
    multipart_offset += strlen(BAIDUDRIVE_UPLOAD_MULTIPART_HEAD1);
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,s_baidu_drv_upload_photo_ptr->file_name);
    multipart_offset += s_baidu_drv_upload_photo_ptr->file_name_len;
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,BAIDUDRIVE_UPLOAD_MULTIPART_HEAD2);
    multipart_offset += strlen(BAIDUDRIVE_UPLOAD_MULTIPART_HEAD2);
    //sprintf(s_upload_photo_ary.buf_ptr+multipart_offset,"%s", "123456789" );
    //multipart_offset += sizeof("123456789");
    if( SFS_ERROR_NONE == MMIAPIFMM_ReadFile(src_ptr->file_handle, s_upload_photo_ary.buf_ptr+multipart_offset , read_bytes,&read_bytes, PNULL))
    {
    	SCI_TRACE_LOW("[baidu] read file ok.  read_bytes:%d", read_bytes);
    }
    
    multipart_offset += read_bytes;
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s--\r\n" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY) + 8);
    
    post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
    post_param_ptr->need_net_prog_ind = FALSE;
    post_param_ptr->is_use_relative_url = TRUE;

    post_param_ptr->post_body.is_use_file = FALSE;
    len = strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);
    post_param_ptr->post_body.body_type_ptr = SCI_ALLOC(len + 1);
    memset(post_param_ptr->post_body.body_type_ptr,0,len + 1);
    strcpy(post_param_ptr->post_body.body_type_ptr,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);
	
SCI_TRACE_LOW("multipart_offset:%d,read_bytes:%d", multipart_offset, read_bytes);

	post_param_ptr->post_body.u.post_buffer.buffer_ptr = s_upload_photo_ary.buf_ptr;
	post_param_ptr->post_body.u.post_buffer.buffer_len = multipart_offset+10;

	post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;

    // handle upload url 
    len=strlen(src_ptr->upload_url);
    post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(post_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(post_param_ptr->uri.uri_ptr,src_ptr->upload_url);

#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id,post_param_ptr, http_req_ary_idx))
	{
		SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_PostRequest success !!!!");

		src_ptr->remain_file_size -= read_bytes;
		src_ptr->sended_file_size += read_bytes;
		http_request_operation_array[context_id].operation_ary[http_req_ary_idx].upload_buf_size = read_bytes;
	}
#endif

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	SCI_FREE(post_param_ptr);

	return;	
	
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_close_session(uint8 context_id , uint8 http_index, MMISRV_BAIDUDRV_UPLOAD_INFO_T * src_ptr)
{
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
       uint32 multipart_offset = 0, body_h1_len=0, body_h2_len=0,  boundary_len=0;
       uint8  temp_path[128]={0};
       uint8  temp_size_str[64] = {0};
       uint8  *md5_list = NULL;
       int     i = 0;
	
	SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_close_session !!!!");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_close_session error code MSG_BAIDU_DRV_ERROR_NOTIFY post_param_ptr == NULL" );
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		return;
	}	

	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;
    post_param_ptr->is_use_relative_url = TRUE;
    
    post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
    /*set contect type*/
    post_param_ptr->post_body.is_use_file = FALSE;
    len = strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);
    post_param_ptr->post_body.body_type_ptr = SCI_ALLOC(len + 1);
    memset(post_param_ptr->post_body.body_type_ptr,0,len + 1);
    strcpy(post_param_ptr->post_body.body_type_ptr,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);

    /*set path*/
    sprintf(temp_path,"%s%s" , BAIDUDRIVE_UPLOAD_END_URL_PRE_PATH, s_baidu_drv_upload_photo_ptr->file_name);
    /*size str*/
    sprintf(temp_size_str,"%d" , s_baidu_drv_upload_photo_ptr->total_file_size);

    body_h1_len = strlen(BAIDUDRIVE_UPLOAD_END_BODY_H1);
    body_h2_len = strlen(BAIDUDRIVE_UPLOAD_END_BODY_H2);
    boundary_len = strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    
    /*set body*/
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr ,"--%s" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len+2);
    /*path*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%spath%s" ,BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 4/*strlen("path")*/ + body_h2_len);
    
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,temp_path);
    multipart_offset += strlen(temp_path);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
    
    /*size*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%ssize%s" , BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 4/*strlen("size")*/+body_h2_len);
    
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,temp_size_str);
    multipart_offset += strlen(temp_size_str);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s" , BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
    
    /*isdir*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%sisdir%s0", BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 5/*strlen("isdir")*/+body_h2_len+1);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
    
    /*rtype*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%srtype%s3", BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 5/*strlen("rtype")*/+body_h2_len+1);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
    
    /*uploadid*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%suploadid%s", BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 8/*strlen("uploadid")*/+body_h2_len);
    
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%s" ,s_baidu_drv_upload_id_str);
    multipart_offset +=strlen(s_baidu_drv_upload_id_str);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);

#if 1 //def X_SLICING_SUPPORT
    
    /*x-enable-resume*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%sx-enable-resume%s2", BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 15/*strlen("x-enable-resume")*/+body_h2_len+1);
    
    /*boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
	
#endif
	
    /*block list*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"%sblock_list%s", BAIDUDRIVE_UPLOAD_END_BODY_H1, BAIDUDRIVE_UPLOAD_END_BODY_H2);
    multipart_offset += (body_h1_len + 10/*strlen("block_list")*/+body_h2_len);
#if 0 //ndef  X_SLICING_SUPPORT
    if(1 == s_baidu_drv_upload_photo_ptr->slicing_num)
    {
        sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"[\"%s\"]" , s_baidu_drv_upload_photo_ptr->file_md5);
        multipart_offset += (strlen(s_baidu_drv_upload_photo_ptr->file_md5)+4);
    }
    else if(1 < s_baidu_drv_upload_photo_ptr->slicing_num)
#endif
    {
        sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"[\"");
        multipart_offset += 2;
        md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
        for(i=0; i<s_baidu_drv_upload_photo_ptr->slicing_num; i++)
        {
            if(i < (s_baidu_drv_upload_photo_ptr->slicing_num -1))
            {
                sprintf(s_upload_photo_ary.buf_ptr+multipart_offset, "%s%s",
                           md5_list+i*MMIFILE_FILE_MD5_OFFSET,
                           "\",\"");
                multipart_offset += MMIFILE_FILE_MD5_LEN + 3;
            }
            else
            {
                sprintf(s_upload_photo_ary.buf_ptr+multipart_offset, "%s",
                           md5_list+i*MMIFILE_FILE_MD5_OFFSET);
                multipart_offset += MMIFILE_FILE_MD5_LEN;
            }
        }
    
        sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\"]");
        multipart_offset += 2;
    }

    /*end boundary*/
    sprintf(s_upload_photo_ary.buf_ptr+multipart_offset ,"\r\n--%s--\r\n", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 8/*strlen(\r\n--)+strlen(--\r\n)*/);

    	post_param_ptr->post_body.u.post_buffer.buffer_ptr = s_upload_photo_ary.buf_ptr;
	post_param_ptr->post_body.u.post_buffer.buffer_len = multipart_offset+10;

	post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;	

    len=strlen(s_baidu_drv_upload_photo_ptr->upload_url);
    post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(post_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(post_param_ptr->uri.uri_ptr,s_baidu_drv_upload_photo_ptr->upload_url);
	
#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id ,post_param_ptr, http_index))
	{
		SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_PostRequest success !!!!");
	}
#endif

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	SCI_FREE(post_param_ptr);

	return;	
}


/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr + sizeof(xSignalHeaderRec)), param, (struct_size - sizeof(xSignalHeaderRec)));
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandleHttpErrorInd(MMISRV_BAIDUDRV_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    HTTP_CONTEXT_ID_T       context_id  = 0;
    HTTP_ERROR_E            http_result = HTTP_SUCCESS;         //http error
    
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    context_id  = signal.context_id;
    http_result = signal.result;
    SCI_TRACE_LOW("[baidu]:HandleHttpErrorInd error = %d",http_result);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1620_112_2_18_3_6_35_25,(uint8*)"d",http_result);
        
    //HTTP_CloseRequest(context_id); 
    return ;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 baidu_drv_get_avaliable_req_index(uint8 * oper_idx)
{
    uint8 context_id=0 , j = 0;
	

    for(context_id=0 ; context_id<MAX_HTTP_CONTEXT_NUM ; context_id++)
    {
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{    
			if(BAIDU_DRV_OPER_NO_OPERATION == http_request_operation_array[context_id].operation_ary[j].operation)		
			{
				*oper_idx = j;
				return context_id;
			}
		}
    }
	
    SCI_TRACE_LOW("[baidu] baidu_drv_get_avaliable_req_index NO req index is avaliable !!!!");
    return 0xFF;
}
/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_device_code_ind_func(uint8 app_param, void * param)
{

	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	   
    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_device_code_ind_func");   
		
	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_device_code_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	if(PNULL == data_ptr || 0 == data_len)
				return 0;

	baidu_drv_InitjsonConfig(&jc,baidu_drv_get_device_code_Json_callback,&input_data);

	jp_ptr = new_JSON_parser(&jc); 

	if(PNULL == jp_ptr)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_device_code_ind_func PNULL == jp_ptr");
		return FALSE;
	}
	if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_device_code_ind_func parse error!");
	}
	delete_JSON_parser(jp_ptr); 

	
	if(0 != strlen (s_baidu_drv_qr_code_url))
	{
		if(MMIAPIFMM_IsFileExist(BAIDU_DRV_QRCODE_IMG_FILE_PATH , MMIAPICOM_Wstrlen(BAIDU_DRV_QRCODE_IMG_FILE_PATH)))
		{
			SCI_TRACE_LOW("[baidu] Enter baidu_drv_device_code_ind_func , qrcode img exist , delete it first ");   
			MMIAPIFMM_DeleteFile(BAIDU_DRV_QRCODE_IMG_FILE_PATH,NULL);
		}

		baidu_drv_get_qrcode_img_req();
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : baidu_GetCodeCallback
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void baidu_GetCodeCallback(void)
{
	MMK_StopTimer(s_query_qr_timer_id);
	MMIAPIBAIDU_get_access_token();
}

/*****************************************************************************/
//  Description : baidu_drv_get_baidu_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_device_code_Json_callback(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

	switch(type) 
	{
	case JSON_T_STRING:         // 10
	{
		if (JSON_TAG_QR_URL == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_drv_qr_code_url , 0x00 , BAIDU_DRV_MAX_STRING_LEN);
			SCI_ASSERT(BAIDU_DRV_MAX_STRING_LEN >= (int)(((*value).vu).str).length);
			
			strncpy(s_baidu_drv_qr_code_url , (int)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] baidu_drv_get_device_code_Json_callback get qr_code_url %s " ,s_baidu_drv_qr_code_url);
		}		
		else if (JSON_TAG_DEVICE_CODE == baidu_drv_data_ptr->cur_tag)
		{			
			SCI_MEMSET(s_baidu_drv_code_str , 0x00 , BAIDU_TOKEN_MAX_SIZE);
			SCI_ASSERT(BAIDU_TOKEN_MAX_SIZE >= (int)(((*value).vu).str).length);
			
			strncpy(s_baidu_drv_code_str , (int)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] baidu_drv_get_device_code_Json_callback get qr_code_url %s " ,s_baidu_drv_code_str);
		}			
	}
	
	break;
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				SCI_TRACE_LOW("[baidu] baidu_drv_get_device_code_Json_callback key is found %d " , i);				
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}



/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_token_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
       MMIFILE_HANDLE  file_handle = 0;
       uint32  cur_timer_s = 0;
       uint32  expires_timer_s = 0;
       uint8 *tmp_data[1024] = {0};
       MMIFILE_ERROR_E  file_ret = SFS_NO_ERROR;
       uint32  write_len = 0;
       uint32  timer_count = 0;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_token_ind_func"); 

	if(HTTP_RESPONSE_CODE_BAD_REQUEST == cnf->response_code)
	{
		if (0 != s_query_qr_timer_id)
		{
			MMK_StopTimer(s_query_qr_timer_id);
			//s_query_qr_timer_id = 0;
		}

		s_query_qr_timer_id = MMK_CreateTimerCallback(MMI_1SECONDS * 5, baidu_GetCodeCallback, PNULL, TRUE);
		return FALSE;
	}

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_token_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	MMISRV_BAIDUDRV_Get_Token_Data(data_ptr , data_len);

       if(MMIAPIFMM_IsFileExist(s_baidu_drv_configure_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_configure_file_path)))
        {
        	MMIAPIFMM_DeleteFile(s_baidu_drv_configure_file_path, PNULL);
        }
	   /*save tocken to file.*/
        file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_configure_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
        if(SFS_INVALID_HANDLE == file_handle)
        {
            baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
            MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
            return FALSE;		
        }
         cur_timer_s = MMIAPICOM_GetCurTime();
         expires_timer_s = cur_timer_s + s_tocken_expires_in;
         SCI_TRACE_LOW("get tocken :cur_timer:%d, expires_timer:%d, expires_in: %d", cur_timer_s, expires_timer_s, s_tocken_expires_in);

         sprintf(tmp_data, "expires_timer:%d,access_token:%s,refresh_token:%s",expires_timer_s,s_baidu_drv_access_token_str, s_baidu_drv_refresh_token_str);

	/*check file mem if enough*/
          if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(cnf->data_len))
          {
              MMIAPIFMM_CloseFile(file_handle);
          	return FALSE;
          }

         file_ret =  MMIAPIFMM_WriteFile(file_handle, tmp_data, strlen(tmp_data), &write_len, NULL);
        if(SFS_ERROR_NONE == file_ret)
        {
        	SCI_TRACE_LOW("[baidu] write access token  success !!");
        }
	 MMIAPIFMM_CloseFile(file_handle);
        /*start token timer.*/
        if((24*60*60+1000) < s_tocken_expires_in)
        {
            timer_count = s_tocken_expires_in - 24*60*60;
        }
        else
        {
             timer_count = (s_tocken_expires_in>60*60)?(s_tocken_expires_in-30*60):s_tocken_expires_in;
         }

        // timer_count = 30;    /*test  30s refresh*/
        if(!s_baidu_drv_refresh_token_timer)
        {
            s_baidu_drv_refresh_token_timer =  MMK_CreateTimerCallback(timer_count, MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }
        else
        {
            if(MMK_IsTimerActive(s_baidu_drv_refresh_token_timer))
            {
                MMK_StopTimer(s_baidu_drv_refresh_token_timer);
            }

            write_len = 1000;
            while(0 < write_len)
            {
                 write_len--;
            }
            MMK_StartTimerCallback(s_baidu_drv_refresh_token_timer, timer_count, MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }

	baidu_drv_get_user_info_req();
	baidu_drv_get_disc_size_info_req();

	baidu_drv_msg_data.init_type= BAIDU_DRV_TYPE_LOGIN;
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_GET_TOKEN_CNF , &baidu_drv_msg_data);
 
    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_token_cnf_func(uint8 app_param, void * param)
{
	HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)param;	
    	MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};


    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_token_cnf_func , response_code = %d" , cnf->rsp_header_info.response_code);   
	
	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_thumbnail_info_ind_func(uint8 app_param, void * param)
{
#if 0
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    	MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	//uint8 total_download_count = 1;
	uint16 baidu_drv_total_photo_size = 1;
	MMISRV_BAIDUDRV_METADATA *tmp_ptr = PNULL;
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;

#ifdef DEBUG_REFRESH

	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_thumbnail_info_ind_func refresh token !!!");
		Reset_Http_all_request();
		//MMIAPIMSA_refresh_token(MSA_APP_TYPE_BAIDU_DRV ,MMISRV_BAIDUDRV_Get_Token);	
		s_baidu_resume_operation = BAIDU_DRV_OPER_GET_THUMBNAIL_INFO ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif
    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_thumbnail_info_ind_func");   

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_thumbnail_info_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

        //Generate_Download_thumbnail_node_by_JSON(data_ptr , data_len);
	//delete meta data first 

	if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
	{
		Clean_meta_data_info();

		if(MMIAPIFMM_IsFileExist(s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path)))
		{
			MMIAPIFMM_DeleteFile(s_new_meta_info_file_path , PNULL);
		}
		
		s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_new_meta_info_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			return FALSE;		
		}
	}
	else
	{
		MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_CUR);
	}
	
	//if(SFS_INVALID_HANDLE != s_meta_data_file_handle)
	{
		file_ret = MMIAPIFMM_WriteFile(
			s_meta_data_file_handle, 
			cnf->data_ptr, 
			cnf->data_len, 
			&write_size, 
			NULL
			);

		if(SFS_ERROR_NONE == file_ret)
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_get_thumbnail_info_ind_func meta update success !!");
		}
		//MMIAPIFMM_CloseFile(file_handle);
	}

    SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_thumbnail_info_ind_func");   
    #endif
    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_thumbnail_info_cnf_func(uint8 app_param, void * param)
{
#if 0
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    	MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	MMISRV_BAIDUDRV_METADATA *tmp_ptr = PNULL;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;
	uint32 file_size = 0 , read_size = 0 ;
	uint8 * buff = PNULL ;
	BOOLEAN retVal = FALSE;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_thumbnail_info_cnf_func");   
    
	file_size = MMIAPIFMM_GetFileSize(s_meta_data_file_handle);
	buff = SCI_ALLOC(file_size);
	
	if(PNULL == buff)
	{
		SCI_TRACE_LOW("[baidu]:baidu_drv_get_thumbnail_info_cnf_func --- (PNULL == buff)"); 
		return 0;
	}

	MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_BEGIN);

	file_ret =  MMIAPIFMM_ReadFile(
	    s_meta_data_file_handle, 
	    buff, 
	    file_size, 
	    &read_size, 
	    NULL
	    );

	if(SFS_ERROR_NONE == file_ret)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_thumbnail_info_cnf_func read meta success !!");
	}
	
	MMIAPIFMM_CloseFile(s_meta_data_file_handle);
	s_meta_data_file_handle = SFS_INVALID_HANDLE ;

	if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_thumbnail_info_cnf_func delete meta file !!");
		MMIAPIFMM_DeleteFile(s_meta_info_file_path , PNULL);
	}

	retVal = MMIAPIFMM_RenameFile((const wchar *)s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path), s_meta_info_file_name , MMIAPICOM_Wstrlen(s_meta_info_file_name));

	if(FALSE == retVal)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_get_thumbnail_info_cnf_func MMIAPIFMM_RenameFile FAIL !!!!");
	}

	s_is_process_get_delta_link = FALSE;

	Generate_Download_thumbnail_node_by_JSON(buff , file_size, 0);

	if(PNULL != s_baidu_drv_metadata_ptr)
	{     
		if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
	           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
		{
			SCI_TRACE_LOW("[baidu]: baidu_drv_get_thumbnail_info_cnf_func need download thumbnail !! Pause auto upload !!!");
			Reset_Http_all_request();			
		}				

		s_download_thumbnail_fail_count =0;
		s_download_thumbnail_success_count =0;
		s_is_process_download_thumbnail = TRUE;				
		
		MMISRV_BAIDUDRV_start_download_thumbnails();			
	}
	else if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
	{
		if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
		   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
		{
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}
	}

	s_download_thumbnail_total_count =  //MMISRV_BAIDUDRV_Get_OneDrive_Photo_Count(buff , file_size);

	SCI_FREE(buff);
	
	baidu_drv_msg_data.total_num = s_download_thumbnail_total_count ;
	baidu_drv_msg_data.curr_download_num = s_current_download_thumbnail_count;
	
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY , &baidu_drv_msg_data);

	s_download_thumbnail_system_time = MMIAPICOM_GetCurTime();
	s_is_need_to_refresh_thumbnail = FALSE;
#endif
	return TRUE;
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 baidu_drv_download_thumbnail_req_func(uint8 app_param, void * param)
{
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 write_size = 0;
	uint8 ret=0 , i;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	MMISRV_BAIDUDRV_MSG_T sig_data={0};
	BOOLEAN bIsFound = FALSE;
       MMISRV_BAIDUDRV_METADATA *remove_ptr = PNULL;
	uint8  avaliable_request_idx = cnf->app_instance;
	uint32 free_high = 0, free_low = 0;
	
#ifdef DEBUG_REFRESH

	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_download_thumbnail_req_func refresh token !!!");
		Reset_Http_all_request();
		//MMIAPIMSA_refresh_token(MSA_APP_TYPE_BAIDU_DRV ,MMISRV_BAIDUDRV_Get_Token);	
		s_baidu_resume_operation = BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif


	if (0 != s_download_thumbnail_timer)
	{
	       MMK_StopTimer(s_download_thumbnail_timer);
		s_download_thumbnail_timer = 0;
    	}

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code && 
	    HTTP_RESPONSE_CODE_FOUND !=cnf->response_code &&
	    HTTP_RESPONSE_CODE_NOT_FOUND !=cnf->response_code &&
	    HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code
	    )
		{
			Reset_all_data();
			sig_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_download_thumbnail_req_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , cnf->response_code);
        	
		return FALSE;
	}

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_thumbnail_req_func");     
		
	if(302 ==cnf->response_code)
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_download_thumbnail_req_func  302 , start get files ");
		return TRUE;
	}
		
	if(HTTP_RESPONSE_CODE_NOT_FOUND !=cnf->response_code)
	{
		file_dev = MMI_DEVICE_SDCARD;
		file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
		if (file_dev < MMI_DEVICE_NUM)
		{
			default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL ;//MMIDC_GetPhotoDefaultPathName();        
		}	

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(FALSE == bIsFound)
		{
			SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_thumbnail_req_func context not found !!!");     
			return FALSE;
		}

		/*check file mem if enough*/
              if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(cnf->data_len))
              {
              	return FALSE;
              }
		
		MMIAPICOM_StrToWstr(http_request_operation_array[i].operation_ary[avaliable_request_idx].info_string , img_file_name);

		MMIAPIFMM_CombineFullPathEx(save_path, MMIFILE_FULL_PATH_MAX_LEN,file_dev,default_path, img_file_name);  

		file_handle = MMIAPIFMM_CreateFile(save_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_ret = MMIAPIFMM_WriteFile(
				file_handle, 
				cnf->data_ptr, 
				cnf->data_len, 
				&write_size, 
				NULL
				);

			if(SFS_ERROR_NONE == file_ret)
			{
				ret = TRUE;
			}
			MMIAPIFMM_CloseFile(file_handle);

			s_download_thumbnail_success_count++;
			sig_data.success_num = s_download_thumbnail_success_count;
			sig_data.total_num = s_download_thumbnail_total_count;
			sig_data.curr_download_num = s_current_download_thumbnail_count;
			sig_data.fail_num = s_download_thumbnail_fail_count;
			sig_data.bDone = TRUE;
#if 0
			if((s_download_thumbnail_success_count + s_download_thumbnail_fail_count) == s_download_thumbnail_total_count)
			{
				sig_data.bDone = TRUE;
			}
			else
			{
				sig_data.bDone = FALSE;
			}
#endif
				
			SCI_MEMCPY((void*)sig_data.filename , save_path , MMIAPICOM_Wstrlen(save_path) * sizeof(wchar));
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF , &sig_data);
		}
	}
	else
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_download_thumbnail_req_func  404 get !!!");
		s_download_thumbnail_fail_count++;
		sig_data.success_num = s_download_thumbnail_success_count;
		sig_data.total_num = s_download_thumbnail_total_count;
		sig_data.curr_download_num = s_current_download_thumbnail_count;
		sig_data.fail_num = s_download_thumbnail_fail_count;
		sig_data.bDone = FALSE;
		SCI_MEMCPY((void*)sig_data.filename , save_path , MMIAPICOM_Wstrlen(save_path) * sizeof(wchar));
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF , &sig_data);
	}

	if(s_current_download_thumbnail_count == (s_download_thumbnail_success_count + s_download_thumbnail_fail_count))
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_download_thumbnail_req_func  download thumbnail done !!!");
		s_current_download_thumbnail_count =0;
		s_is_process_download_thumbnail = FALSE;

		
		if((BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) && 
		    (MMISRV_BAIDUDRV_Need_manual_auto_upload_photo() ||
		     MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo()))
		{
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}
	}
	else
	{
        	s_download_thumbnail_timer =  MMK_CreateTimerCallback(1000 * 15, HandleDownloadThumbnail_timer, PNULL, FALSE);
	}

	return TRUE;
}

LOCAL uint8    baidu_drv_download_thumbnail_info_cnf_func(uint8 app_param, void * param)
{
	SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_thumbnail_info_cnf_func");   

	if(s_download_thumbnail_total_count == (s_download_thumbnail_success_count+s_download_thumbnail_fail_count))
	{
		if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
		{
			if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
			   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
			{
				MMISRV_BAIDUDRV_exec_auto_upload_photo();
			}
		}
	}

	return TRUE;
}
/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_send_session_http_req(void)
{
    uint8 i=0 , avaliable_request_idx =0 , http_idx=0 , context_id;
    BOOLEAN bIs_found = FALSE;
    //uint32 buff_size_ary = S_UPLOAD_SLICING_MAX+512;
    DWORD   x_part_size = 0;
    uint16 templen = 0;

    SCI_TRACE_LOW("[baidu] ENTER baidu_drv_send_session_http_req");

    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);		   
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);
       
    if(0xFF == context_id)
    {
        SCI_TRACE_LOW("[baidu] ENTER baidu_drv_send_session_http_req (0xFF == avaliable_request_idx)");
        return 0;
    }

    if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
       BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
    {
        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION;
    }
    else
    {
        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_UPLOAD_SESSION;
    }

    #if   0  
    bIs_found = FALSE;
    if(PNULL ==s_upload_photo_ary.buf_ptr)
    {
        s_upload_photo_ary.buf_ptr = SCI_ALLOC_APPZ(buff_size_ary);
        if(PNULL !=s_upload_photo_ary.buf_ptr)
        {
            bIs_found  = TRUE;
            s_upload_photo_ary.buf_size= buff_size_ary;
            SCI_TRACE_LOW("[baidu] ENTER baidu_drv_send_session_http_req s_upload_photo_ary.buf_size %d" , s_upload_photo_ary.buf_size);
        }
    }
    else
    {
        //compose post request 
        SCI_MEMSET(s_upload_photo_ary.buf_ptr , 0x00 , s_upload_photo_ary.buf_size);
        bIs_found  = TRUE;
    }
    #endif
    /*set upload url*/
#if 1 //def X_SLICING_SUPPORT
	if((1 == s_baidu_drv_upload_photo_ptr->slicing_num)&&(1 == s_baidu_drv_upload_photo_ptr->x_last_slicing_num))
     	{
    sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%s%s%d",
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&uploadid=",s_baidu_drv_upload_id_str,
                                    "&partseq=",
                                    s_baidu_drv_upload_photo_ptr->current_slicing_num);
	}
	else
	{ 
	       if(1 == s_baidu_drv_upload_photo_ptr->slicing_num)
	       {
	       	x_part_size = s_baidu_drv_upload_photo_ptr->x_part_size;
	       }
		else
		{
			if((s_baidu_drv_upload_photo_ptr->current_slicing_num+1) == s_baidu_drv_upload_photo_ptr->slicing_num)
			{
				x_part_size = s_baidu_drv_upload_photo_ptr->x_part_size;
			}
			else
			{
				x_part_size = S_UPLOAD_SLICING_MAX;
			}
		}
		sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%s%s%d%s%d%s%d",
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&uploadid=",s_baidu_drv_upload_id_str,
                                    "&partseq=",
                                    s_baidu_drv_upload_photo_ptr->current_slicing_num,
                                    "&x-enable-resume=2&x-file-size=",
                                    s_baidu_drv_upload_photo_ptr->total_file_size,
                                    "&x-part-size=",
                                    x_part_size);
		if(0 < s_baidu_drv_upload_photo_ptr->x_slicing_current_count)
		{
		       templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
			sprintf((char*)&s_baidu_drv_upload_photo_ptr->upload_url[templen], "%s%d%s%s",
				           "&x-part-offset=",
				           g_x_bs_file_size,
				           "&x-part-etag=",
				           s_pre_contest_md5);
		}
	}
#else
    sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%s%s%d",
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_FILE_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&uploadid=",s_baidu_drv_upload_id_str,
                                    "&partseq=",
                                    s_baidu_drv_upload_photo_ptr->current_slicing_num);
#endif
	SCI_TRACE_LOW("[baidu]: Enter baidu_drv_send_session_http_req !! upload_url: %s" , s_baidu_drv_upload_photo_ptr->upload_url);	             

    //if(bIs_found)
    {
        MMISRV_BAIDUDRV_upload_session(context_id , avaliable_request_idx , s_baidu_drv_upload_photo_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_close_session_http_req(void)
{
    uint8 i, avaliable_request_idx =0 , context_id;
    uint64 buff_size_ary = 2048;
    BOOLEAN bIs_found = FALSE;
    
    //send close session !!!!!
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF == context_id)
        return FALSE;

    if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
       BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
    {
        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_AUTO_CLOSE_SESSION;
    }
    else
    {
        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_CLOSE_SESSION;
    }

    if(PNULL ==s_upload_photo_ary.buf_ptr)
    {

            s_upload_photo_ary.buf_ptr = SCI_ALLOC_APPZ(buff_size_ary);
            if(PNULL !=s_upload_photo_ary.buf_ptr)
            {
                bIs_found  = TRUE;
                s_upload_photo_ary.buf_size= buff_size_ary;
                SCI_TRACE_LOW("[baidu] ENTER baidu_drv_close_session_http_req s_upload_photo_ary.buf_size %d" , s_upload_photo_ary.buf_size);
            }
    }
    else
    {
        //compose post request 
        SCI_MEMSET(s_upload_photo_ary.buf_ptr , 0x00 , s_upload_photo_ary.buf_size);
        bIs_found  = TRUE;
    }

    /*set upload url*/
    sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s",
                                    BAIDUDRIVE_UPLOAD_END_URL_PART1 ,
                                    s_baidu_drv_access_token_str);
    SCI_TRACE_LOW("[baidu]: Enter baidu_drv_close_session_http_req !! upload_url: %s" , s_baidu_drv_upload_photo_ptr->upload_url);
    
    if(bIs_found)
    {
        MMISRV_BAIDUDRV_close_session(context_id , avaliable_request_idx , s_baidu_drv_upload_photo_ptr);
    }
    SCI_TRACE_LOW("[baidu] baidu_drv_close_session_http_req send close session end !!!!");

    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_create_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint8 i=0 , avaliable_request_idx =0 , http_idx=0;
	BOOLEAN bIs_found = FALSE;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

#ifdef DEBUG_REFRESH
	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_create_session_req_func refresh token !!!");
		Reset_Http_all_request();
		//MMIAPIMSA_refresh_token(MSA_APP_TYPE_BAIDU_DRV ,MMISRV_BAIDUDRV_Get_Token);	
		s_baidu_resume_operation = BAIDU_DRV_OPER_CREATE_SESSION ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_create_session_req_func"); 

	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{		
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
	              SCI_TRACE_LOW("[baidu]baidu_drv_create_session_req_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
			Reset_all_data();	  
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		}
		return FALSE;
	}

	data_len = cnf->rsp_header_info.content_length;
	data_ptr = cnf->rsp_header_info.content_type_ptr;

	MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind(data_ptr , data_len);

	if(s_b_is_cancel_upload_photo)
	{
              SCI_TRACE_LOW("[baidu]baidu_drv_create_session_req_func upload has been canceled !!");
		s_b_is_cancel_upload_photo = FALSE;
		return FALSE;
	}

	// get session id
	if(PNULL != cnf->rsp_header_info.session_id_ptr)
	{
		strncpy(s_baidu_drv_upload_photo_ptr->session_id , cnf->rsp_header_info.session_id_ptr , strlen(cnf->rsp_header_info.session_id_ptr));
		s_baidu_drv_upload_photo_ptr->session_id[strlen(cnf->rsp_header_info.session_id_ptr)] = 0;
	}
	//prepare file info 
	
	//s_baidu_drv_upload_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_photo_ptr->file_path , SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/
	if (s_baidu_drv_upload_photo_ptr->file_handle != 0)
	{
		//get file size
		s_baidu_drv_upload_photo_ptr->total_file_size =  s_baidu_drv_upload_photo_ptr->remain_file_size =MMIAPIFMM_GetFileSize(s_baidu_drv_upload_photo_ptr->file_handle);
		s_baidu_drv_upload_photo_ptr->uploaded_file_size = s_baidu_drv_upload_photo_ptr->sended_file_size= 0;

		//start upload processing ...
		baidu_drv_send_session_http_req();
	}
	else
	{
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		SCI_TRACE_LOW("[baidu]baidu_drv_create_session_req_func ERROR s_baidu_drv_upload_photo_ptr->file_handle == 0 !!!!");
		Remove_File_From_UploadFileAry(s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len);
	}	

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_upload_session_req_func(uint8 app_param, void * param)
{
    HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
    uint8 i=0 , avaliable_request_idx =0;
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    BOOLEAN bIsFound = FALSE;

    SCI_TRACE_LOW("[baidu] Enter baidu_drv_upload_session_req_func"); 

    SCI_MEMSET((void*)s_upload_photo_ary.buf_ptr, 0x00 , s_upload_photo_ary.buf_size);

    if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
    {
        if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
        {
            baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
            SCI_TRACE_LOW("[baidu] baidu_drv_upload_session_req_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
            MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
            Reset_all_data();
            MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);		
        }
        return FALSE;
    }

    if(s_b_is_cancel_upload_photo)
    {
        SCI_TRACE_LOW("[baidu]baidu_drv_upload_session_req_func upload has been canceled !!");
        s_b_is_cancel_upload_photo = FALSE;
        return FALSE;
    }

    for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
    {
        if(cnf->context_id == http_request_operation_array[i].context_id)
        {
            bIsFound = TRUE;
            break;
        }			
    }

    if(FALSE == bIsFound)
    {
        SCI_TRACE_LOW("[baidu] Enter baidu_drv_upload_session_req_func context not found !!!");     
        return FALSE;
    }

	s_baidu_drv_upload_photo_ptr->uploaded_file_size += http_request_operation_array[i].operation_ary[cnf->app_instance].upload_buf_size;
	if(0 < s_baidu_drv_upload_photo_ptr->remain_file_size)
	{
		baidu_drv_send_session_http_req();
	}
	else
	{
		baidu_drv_close_session_http_req();
	}

	baidu_drv_msg_data.total_file_size = s_baidu_drv_upload_photo_ptr->total_file_size; 
	baidu_drv_msg_data.uploaded_file_size = s_baidu_drv_upload_photo_ptr->uploaded_file_size ;

	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY , &baidu_drv_msg_data);


	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_close_session_req_func(uint8 app_param, void * param)
{
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	MMISRV_BAIDUDRV_UPLOAD_INFO_T * tmp_ptr = PNULL;
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	FILEARRAY_DATA_T file_data = {0};
	MMIFILE_HANDLE      file_handle =   0;    

	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			SCI_TRACE_LOW("[baidu]baidu_drv_close_session_req_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
			Reset_all_data();
		}
		return FALSE;
	}	
	
	SCI_TRACE_LOW("[baidu] Enter baidu_drv_close_session_req_func"); 
	
	MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);	

	SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
	file_data.type = FILEARRAY_TYPE_FILE;
	MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
	file_data.name_len = s_baidu_drv_upload_photo_ptr->file_path_len;

	//upload success file array
	if(PNULL == s_baidu_drv_upload_success_file_array)
	{
		s_baidu_drv_upload_success_file_array = MMIAPIFILEARRAY_Create();
		MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_upload_success_file_array, 10);

		//upload success file array
		if(MMIAPIFMM_IsFileExist(s_baidu_drv_upload_success_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path)))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));
		}
		else
			{
			file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_success_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
			MMIAPIFMM_CloseFile(file_handle);
			}
	}
	
	MMIAPIFILEARRAY_Add (s_baidu_drv_upload_success_file_array, &file_data);
	MMIAPIFILEARRAY_SaveArrayToFileEx(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));
       MMIAPIFILEARRAY_Destroy(&s_baidu_drv_upload_success_file_array);
	s_baidu_drv_upload_success_file_array = PNULL; 

	baidu_drv_msg_data.bDone = TRUE;
	MMI_WSTRNCPY (baidu_drv_msg_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF , &baidu_drv_msg_data);

	//if(s_is_in_gallery)
	{
		if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
		   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
		{
			SCI_TRACE_LOW("[baidu] baidu_drv_auto_upload_close_session_req_func start auto upload !!!"); 
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}
		else
		{
			s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE ;
			Clean_upload_data_info();
		}
	}

	s_is_need_to_refresh_thumbnail = TRUE;

	SCI_FREE(s_baidu_drv_upload_photo_ptr);
	s_baidu_drv_upload_photo_ptr = PNULL;

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_create_data_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_create_data_ind_func");   
       if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
              SCI_TRACE_LOW("[baidu] UPLOAD pause......");
       	return FALSE;
       }

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_token_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind(data_ptr , data_len);
	//SCI_TRACE_LOW("[baidu]baidu_drv_auto_upload_create_data_ind_func Upload_ID = %s" , s_baidu_drv_upload_id_str);	
        //SCI_ASSERT(0);
    return TRUE;
}

LOCAL uint8    baidu_drv_auto_upload_session_data_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
       uint8* tmp_ptr = NULL, *tmp_ptr1 = NULL;
       uint32 templen = 0;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_session_data_ind_func");   
		
      if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
       	SCI_TRACE_LOW("[baidu] UPLOAD pause......");  
       	return FALSE;
       }

	if((HTTP_RESPONSE_CODE_OK != cnf->response_code)&&(HTTP_RESPONSE_CODE_BAD_REQUEST != cnf->response_code))
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_token_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind(data_ptr , data_len);
	SCI_TRACE_LOW("%s", data_ptr);
#if 0
       tmp_ptr = strstr(data_ptr, "fs_id");
       if(tmp_ptr)
       {
            SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr != 0x3A) // ':'
            {
                tmp_ptr++;
            }
            tmp_ptr1 = ++tmp_ptr;
            while(*tmp_ptr1 != 0x2C) // ','
            {
                templen++;
                tmp_ptr1++;
            }
            {
                 SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                 if(templen > (MMIFILE_FS_ID_LEN_MAX +1))
                 {
                    SCI_ASSERT(0);
                 }
                 SCI_MEMSET(s_baidu_drv_upload_photo_ptr->file_fs_id, 0x00 , MMIFILE_FS_ID_LEN_MAX+ 1);	
                 strncpy(s_baidu_drv_upload_photo_ptr->file_fs_id , tmp_ptr , templen);
                 SCI_TRACE_LOW("[baidu] FS_ID: %s :%d" , s_baidu_drv_upload_photo_ptr->file_fs_id, templen);
            }
            
       }
       else
        {
            SCI_ASSERT(0);
        }
#endif
    
    return TRUE;
}
/*****************************************************************************/
//  Description : baidu_drv_auto_upload_close_data_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/

LOCAL uint8    baidu_drv_auto_upload_close_data_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
       uint8* tmp_ptr = NULL, *tmp_ptr1 = NULL;
       uint32 templen = 0;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_close_data_ind_func");   
		
      if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
       	SCI_TRACE_LOW("[baidu] UPLOAD pause......");  
       	return FALSE;
       }

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_token_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind(data_ptr , data_len);
#if 0
       tmp_ptr = strstr(data_ptr, "fs_id");
       if(tmp_ptr)
       {
            SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr != 0x3A) // ':'
            {
                tmp_ptr++;
            }
            tmp_ptr1 = ++tmp_ptr;
            while(*tmp_ptr1 != 0x2C) // ','
            {
                templen++;
                tmp_ptr1++;
            }
            {
                 SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                 if(templen > (MMIFILE_FS_ID_LEN_MAX +1))
                 {
                    SCI_ASSERT(0);
                 }
                 SCI_MEMSET(s_baidu_drv_upload_photo_ptr->file_fs_id, 0x00 , MMIFILE_FS_ID_LEN_MAX+ 1);	
                 strncpy(s_baidu_drv_upload_photo_ptr->file_fs_id , tmp_ptr , templen);
                 SCI_TRACE_LOW("[baidu] FS_ID: %s :%d" , s_baidu_drv_upload_photo_ptr->file_fs_id, templen);
            }
            
       }
       else
        {
            SCI_ASSERT(0);
        }
#endif
    
    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_create_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
       MMIFILE_HANDLE file_handle = 0;
       MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint8 i=0 , avaliable_request_idx =0 , http_idx=0;
	BOOLEAN bIs_found = FALSE;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_create_session_req_func"); 

      if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
       	SCI_TRACE_LOW("[baidu] UPLOAD pause......");  
       	return FALSE;
       }

	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{

		SCI_TRACE_LOW("[baidu]baidu_drv_auto_upload_create_session_req_func ERROR code %d" , cnf->rsp_header_info.response_code);

		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{		
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			Reset_all_data();	  
		}
		return FALSE;
	}
    
	//s_baidu_drv_upload_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_photo_ptr->file_path , SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/
	if (s_baidu_drv_upload_photo_ptr->file_handle != 0)
	{
            //get file size
            s_baidu_drv_upload_photo_ptr->total_file_size =  s_baidu_drv_upload_photo_ptr->remain_file_size =MMIAPIFMM_GetFileSize(s_baidu_drv_upload_photo_ptr->file_handle);
            s_baidu_drv_upload_photo_ptr->uploaded_file_size = s_baidu_drv_upload_photo_ptr->sended_file_size= 0;
            //for(i=0; i< s_baidu_drv_upload_photo_ptr->slicing_num; i++)
            {
                s_baidu_drv_upload_photo_ptr->current_slicing_num = 0;
                //start upload processing ...
                baidu_drv_send_session_http_req();
            }
	}
	else
	{
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		SCI_TRACE_LOW("[baidu]baidu_drv_auto_upload_create_session_req_func ERROR s_baidu_drv_upload_photo_ptr->file_handle == 0 !!!!");
		Remove_File_From_UploadFileAry(s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len);
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	BOOLEAN bIsFound = FALSE;
	FILEARRAY_DATA_T file_data = {0};

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_session_req_func"); 

	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
       	SCI_TRACE_LOW("[baidu] UPLOAD pause......");  
       	return FALSE;
       }

       /*if 31299 error, delete from upload list.*/
       if(31299 == g_baidu_upload_error_code)
       {
              g_baidu_upload_error_code = 0;
       	MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);	
		s_baidu_drv_upload_photo_ptr->file_handle = 0;
		
		SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
		file_data.type = FILEARRAY_TYPE_FILE;
		MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
		  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
		file_data.name_len = s_baidu_drv_upload_photo_ptr->file_path_len;

		//MMIAPIFILEARRAY_Add (s_baidu_drv_upload_success_file_array, &file_data);
		//MMIAPIFILEARRAY_SaveArrayToFileEx(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));

		MMIAPIFILEARRAY_Delete(s_current_upload_file_array , 0);
		MMIAPIFILEARRAY_SaveArrayToFileEx(s_current_upload_file_array , s_current_upload_file_path ,MMIAPICOM_Wstrlen(s_current_upload_file_path));
		
		baidu_drv_msg_data.bDone = TRUE;
		MMI_WSTRNCPY (baidu_drv_msg_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
		  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF , &baidu_drv_msg_data);
		
		Clean_upload_data_info();
		
		if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo() || 
			MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
	        {
	            MMISRV_BAIDUDRV_exec_auto_upload_photo();
	        }
	        else
	        {
			SCI_TRACE_LOW("[baidu] baidu_drv_auto_upload_close_session_req_func No photo need to be uploaded !!!"); 
			s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
			if(s_is_in_gallery)
			{
				Clean_upload_data_info();
			}
			else
			{
				MMISRV_DeleteBaiduDrvTaskThread();
			}
	        }
		//baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_JSON_ERROR;
		//MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			
		 return FALSE;
       }
	SCI_MEMSET((void*)s_upload_photo_ary.buf_ptr, 0x00 , s_upload_photo_ary.buf_size);


	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);		
       	 	SCI_TRACE_LOW("[baidu]baidu_drv_auto_upload_session_req_func error code %d" , cnf->rsp_header_info.response_code);
			MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
			Reset_all_data();
		}
		return FALSE;
	}

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		if(cnf->context_id == http_request_operation_array[i].context_id)
		{
			bIsFound = TRUE;
			break;
		}			
	}

	if(FALSE == bIsFound)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_upload_session_req_func context not found !!!");     
		return FALSE;
	}
    
       if(s_upload_photo_ary.buf_ptr)
       {
            SCI_FREE(s_upload_photo_ary.buf_ptr);
            s_upload_photo_ary.buf_ptr = NULL;
       }
       
	s_baidu_drv_upload_photo_ptr->uploaded_file_size += http_request_operation_array[i].operation_ary[cnf->app_instance].upload_buf_size;
	#if 1 //def X_SLICING_SUPPORT
	SCI_TRACE_LOW("[baidu] uploaded_file_size:%d, current_slicing_num:%d, x_slicing_current_count:%d",
		  s_baidu_drv_upload_photo_ptr->uploaded_file_size,
		  s_baidu_drv_upload_photo_ptr->current_slicing_num,
		  s_baidu_drv_upload_photo_ptr->x_slicing_current_count);
	#endif
	if(0 < s_baidu_drv_upload_photo_ptr->remain_file_size)
	{
#if 1 //def X_SLICING_SUPPORT
              /*have send one slice, reset x_slicing*/
              if(0 == (s_baidu_drv_upload_photo_ptr->uploaded_file_size%S_UPLOAD_SLICING_MAX))
              {
              	s_baidu_drv_upload_photo_ptr->current_slicing_num++;
			s_baidu_drv_upload_photo_ptr->x_slicing_current_count = 0;
              }
	       else
	       {
	       	s_baidu_drv_upload_photo_ptr->x_slicing_current_count++;;
	       }
	       
              if(s_baidu_drv_upload_photo_ptr->current_slicing_num < s_baidu_drv_upload_photo_ptr->slicing_num)
              {
		    baidu_drv_send_session_http_req();
              }
              else
              {
                    SCI_TRACE_LOW("[baidu] baidu_drv_upload_session_req_func. pararmeter error.....");
                    //SCI_ASSERT(0);
              }
#else
	       s_baidu_drv_upload_photo_ptr->current_slicing_num++;
              if(s_baidu_drv_upload_photo_ptr->current_slicing_num < s_baidu_drv_upload_photo_ptr->slicing_num)
              {
		    baidu_drv_send_session_http_req();
              }
              else
              {
                    SCI_TRACE_LOW("[baidu] baidu_drv_upload_session_req_func. pararmeter error.....");
                    SCI_ASSERT(0);
              }
#endif
	}
	else
	{
		baidu_drv_close_session_http_req();
	}

	baidu_drv_msg_data.total_file_size = s_baidu_drv_upload_photo_ptr->total_file_size; 
	baidu_drv_msg_data.uploaded_file_size = s_baidu_drv_upload_photo_ptr->uploaded_file_size ;

	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY , &baidu_drv_msg_data);
	

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_auto_upload_close_session_req_func(uint8 app_param, void * param)
{
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	MMISRV_BAIDUDRV_UPLOAD_INFO_T * tmp_ptr = PNULL;
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	//FILEARRAY upload_success_file_array = PNULL;
	FILEARRAY_DATA_T file_data = {0};
	MMIFILE_HANDLE      file_handle =   0;    
	uint32 upload_file_size = 0;
	
      SCI_TRACE_LOW("[baidu] Enter baidu_drv_auto_upload_close_session_req_func"); 
      if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status&&
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING != s_auto_upload_status)
       {
       	SCI_TRACE_LOW("[baidu] UPLOAD pause......");  
       	return FALSE;
       }
	
	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);		
			SCI_TRACE_LOW("baidu_drv_close_session_req_func error code %d" , cnf->rsp_header_info.response_code);
			MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
			s_baidu_drv_upload_photo_ptr->file_handle = 0;
			Reset_all_data();
		}
		return FALSE;
	}
	if((0 != g_baidu_upload_errno)&&(31190 != g_baidu_upload_errno))
	{
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_JSON_ERROR;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);		
		SCI_TRACE_LOW("baidu_drv_close_session_req_func. g_baidu_upload_errno:%d" , g_baidu_upload_errno);
		MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
		s_baidu_drv_upload_photo_ptr->file_handle = 0;
		Reset_all_data();
		return FALSE;
	}

	MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);	
	s_baidu_drv_upload_photo_ptr->file_handle = 0;

       if(0 == g_baidu_upload_errno)
	{
		SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
		file_data.type = FILEARRAY_TYPE_FILE;
		MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
		  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
		file_data.name_len = s_baidu_drv_upload_photo_ptr->file_path_len;

		//upload success file array
		if(PNULL == s_baidu_drv_upload_success_file_array)
		{
			s_baidu_drv_upload_success_file_array = MMIAPIFILEARRAY_Create();
			MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_upload_success_file_array, 10);

			//upload success file array
			if(MMIAPIFMM_IsFileExist(s_baidu_drv_upload_success_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path)))
			{
				MMIAPIFILEARRAY_LoadArrayFromFile(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));
			}
			else
				{
				file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_success_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
				MMIAPIFMM_CloseFile(file_handle);
				}
		}
		MMIAPIFILEARRAY_Add (s_baidu_drv_upload_success_file_array, &file_data);
		MMIAPIFILEARRAY_SaveArrayToFileEx(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));
              MMIAPIFILEARRAY_Destroy(&s_baidu_drv_upload_success_file_array);
	       s_baidu_drv_upload_success_file_array = PNULL;


		MMIAPIFILEARRAY_Delete(s_current_upload_file_array , 0);
		MMIAPIFILEARRAY_SaveArrayToFileEx(s_current_upload_file_array , s_current_upload_file_path ,MMIAPICOM_Wstrlen(s_current_upload_file_path));
		
		baidu_drv_msg_data.bDone = TRUE;
		MMI_WSTRNCPY (baidu_drv_msg_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
		  				s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, s_baidu_drv_upload_photo_ptr->file_path_len);/*lint !e670 */
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF , &baidu_drv_msg_data);

	            /*delete upload photo.*/
	        if(MMIAPIFMM_IsFileExist(s_baidu_drv_upload_photo_ptr->file_path, MMIAPICOM_Wstrlen(s_baidu_drv_upload_photo_ptr->file_path)))
	        {
	            SCI_TRACE_LOW("[baidu] baidu_drv_auto_upload_close_session_req_func delete sucess upload !!!"); 
	            MMIAPIFMM_DeleteFile(s_baidu_drv_upload_photo_ptr->file_path , PNULL);
	        }
       }
        Clean_upload_data_info();
	
	if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo() || 
		MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
        {
            MMISRV_BAIDUDRV_exec_auto_upload_photo();
        }
        else
        {
		SCI_TRACE_LOW("[baidu] baidu_drv_auto_upload_close_session_req_func No photo need to be uploaded !!!"); 
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
		if(s_is_in_gallery)
		{
			Clean_upload_data_info();
		}
		else
		{
			MMISRV_DeleteBaiduDrvTaskThread();
		}
        }


       /*recieve thumbnail*/
	s_is_need_to_refresh_thumbnail = TRUE;
        return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8    baidu_drv_get_user_info_req(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
		

	SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_user_info_req !!");	
	
	BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
	context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
	BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  baidu_drv_get_user_info_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_GET_USER_INFO;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(BAIDUDRIVE_GET_USER_INFO_URL) + strlen(s_baidu_drv_access_token_str);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s" , BAIDUDRIVE_GET_USER_INFO_URL ,s_baidu_drv_access_token_str);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  Description : baidu_drv_get_user_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_user_info_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
       MMIFILE_ERROR_E  file_ret = SFS_NO_ERROR;
       uint32  write_len = 0;
       MMIFILE_HANDLE  file_handle = 0;
       uint8 *tmp_data[1024] = {0};
	   
    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_user_info_ind_func");   
		
	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		wchar * str_default_name = L"---------";
		MMI_WSTRNCPY (s_baidu_user_info.baidu_name, MAX_BAIDU_USER_INFO_STR_SIZE,
		  				str_default_name, MMIAPICOM_Wstrlen(str_default_name) , MMIAPICOM_Wstrlen(str_default_name));
				
#if 0	
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_user_info_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
#endif		
	}
	else
	{
		data_len = cnf->data_len;
		data_ptr = cnf->data_ptr;

              /*check file mem if enough*/
              if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(data_len))
              {
              	return FALSE;
              }
			  
		if(PNULL == data_ptr || 0 == data_len)
					return 0;

		if(MMIAPIFMM_IsFileExist(s_baidu_drv_user_info_path, MMIAPICOM_Wstrlen(s_baidu_drv_user_info_path)))
	        {
	        	MMIAPIFMM_DeleteFile(s_baidu_drv_user_info_path, PNULL);
	        }
		   /*save tocken to file.*/
	        file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_user_info_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/

	         file_ret =  MMIAPIFMM_WriteFile(file_handle, data_ptr, data_len, &write_len, NULL);
	        if(SFS_ERROR_NONE == file_ret)
	        {
	        	SCI_TRACE_LOW("[baidu] write user info data  success !!");
	        }
		 MMIAPIFMM_CloseFile(file_handle);	   

		baidu_drv_InitjsonConfig(&jc,baidu_drv_get_user_info_Json_callback,&input_data);

		jp_ptr = new_JSON_parser(&jc); 

		if(PNULL == jp_ptr)
		{
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Token_Data PNULL == jp_ptr");
			return FALSE;
		}
		if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
		{
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Token_Data parse error!");
		}
		delete_JSON_parser(jp_ptr); 


		if(strlen(s_baidu_user_info.avatar_url))
		{
			baidu_drv_get_avatar_img_req();
		}
	}
	
	s_baidu_drv_check_user_info_timer =  MMK_CreateTimerCallback(1000 * 5, Handle_UserInfo_timer, PNULL, FALSE);
      	
    return TRUE;
}

/*****************************************************************************/
//  Description : baidu_drv_get_baidu_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_user_info_Json_callback(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;
	char tmp_utf8_str[MAX_BAIDU_USER_INFO_STR_SIZE+1] = {0};

	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

	switch(type) 
	{
	case JSON_T_STRING:         // 10
	{
		if (JSON_TAG_AVATAR == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_user_info.avatar_url , 0x00 , MAX_BAIDU_USER_INFO_STR_SIZE);
			SCI_ASSERT(MAX_BAIDU_USER_INFO_STR_SIZE >= (int)(((*value).vu).str).length);
			
			strncpy(s_baidu_user_info.avatar_url , (int)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] baidu_drv_get_user_info_Json_callback get avatar_url %s " , s_baidu_user_info.avatar_url);				
		}
		else if (JSON_TAG_NAME == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(tmp_utf8_str , 0x00 , MAX_BAIDU_USER_INFO_STR_SIZE);
			SCI_ASSERT(MAX_BAIDU_USER_INFO_STR_SIZE >= (int)(((*value).vu).str).length);
			
			strncpy(tmp_utf8_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			GUI_UTF8ToWstr(s_baidu_user_info.baidu_name, MAX_BAIDU_USER_INFO_STR_SIZE ,tmp_utf8_str, strlen(tmp_utf8_str) );					
		}		
		else if (JSON_TAG_NETDISK_NAME == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(tmp_utf8_str , 0x00 , MAX_BAIDU_USER_INFO_STR_SIZE);
			SCI_ASSERT(MAX_BAIDU_USER_INFO_STR_SIZE >= (int)(((*value).vu).str).length);
			
			strncpy(tmp_utf8_str , (uint8*)(((*value).vu).str).value , (int)(((*value).vu).str).length);
			GUI_UTF8ToWstr(s_baidu_user_info.netdisk_name, MAX_BAIDU_USER_INFO_STR_SIZE ,tmp_utf8_str, strlen(tmp_utf8_str) );			
		}					
	}
	
	break;
       case JSON_T_INTEGER:       //5
       {
            if(JSON_TAG_VIP_TYPE == baidu_drv_data_ptr->cur_tag)
            {
                s_baidu_user_info.vip_type = value->vu.integer_value;
            }
       }
       break;
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8    baidu_drv_get_disc_size_info_req(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
		

	SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_disc_size_info_req !!");	
	
	BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
	context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
	BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  baidu_drv_get_disc_size_info_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_GET_DISK_QUOTA;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(BAIDUDRIVE_GET_QUOTA_INFO_URL) + strlen(s_baidu_drv_access_token_str);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s" , BAIDUDRIVE_GET_QUOTA_INFO_URL ,s_baidu_drv_access_token_str);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx );
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_disc_size_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	   
    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_disc_size_ind_func");   
		
	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{

		SCI_TRACE_LOW("[baidu]baidu_drv_get_disc_size_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		s_baidu_user_info.size_total = 9999;
#if 0	
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_disc_size_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
#endif		
	}
	else
	{
		data_len = cnf->data_len;
		data_ptr = cnf->data_ptr;
		   
		if(PNULL == data_ptr || 0 == data_len)
					return 0;

		baidu_drv_InitjsonConfig(&jc,baidu_drv_get_disc_size_Json_callback,&input_data);

		jp_ptr = new_JSON_parser(&jc); 

		if(PNULL == jp_ptr)
		{
			SCI_TRACE_LOW("[baidu] baidu_drv_get_disc_size_ind_func PNULL == jp_ptr");
			return FALSE;
		}
		if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
		{
			SCI_TRACE_LOW("[baidu] baidu_drv_get_disc_size_ind_func parse error!");
		}
		delete_JSON_parser(jp_ptr); 
	}

    return TRUE;
}


uint64 byte_atoi( const char *inString ) {
	char suffix = '\0';
	/*int saw_digit, octets;*/
	uint64 tmp = 0;
	uint64 *tp;
	static const char digits[] = "0123456789";
	uint64 newch;
	/*saw_digit = 0;
	octets = 0;*/
	tp = (uint64 *)&tmp;

	while ( (suffix = *inString++) != '\0' ) 
	{
		const char *pch;

		if ( (pch = strchr(digits, suffix)) != NULL ) 
		{
			newch = (uint64)(*tp * 10 + (pch - digits));

			*tp = newch;
		} 
		else
		{
			break;
		}
	}

	return (uint64) tmp;
} /* end byte_atof */

/*****************************************************************************/
//  Description : baidu_drv_get_baidu_info_ind_func
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_get_disc_size_Json_callback(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;
	char tmp_utf8_str[MAX_BAIDU_USER_INFO_STR_SIZE+1] = {0};

	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

	switch(type) 
	{
	case JSON_T_INTEGER:         // 10
	{
		if (JSON_TAG_SIZE_TOTAL == baidu_drv_data_ptr->cur_tag)
		{
			//s_baidu_user_info.size_total = (*value).vu.integer_value;
			s_baidu_user_info.size_total = byte_atoi((*value).parse_buffer.value);
			SCI_TRACE_LOW("[baidu] baidu_drv_get_disc_size_Json_callback get size_total %d " , s_baidu_user_info.size_total);				
		}
		else if (JSON_TAG_SIZE_USED == baidu_drv_data_ptr->cur_tag)
		{
			s_baidu_user_info.size_used = byte_atoi((*value).parse_buffer.value);
			SCI_TRACE_LOW("[baidu] baidu_drv_get_disc_size_Json_callback get size_used %d " , s_baidu_user_info.size_used);
		}					
	}
	
	break;
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				SCI_TRACE_LOW("[baidu] baidu_drv_get_disc_size_Json_callback key is found %d " , i);				
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_req(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

	SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_qrcode_img_req !!");	

	BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
	context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
	BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

	if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  baidu_drv_get_qrcode_img_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		if(PNULL == s_current_download_photo_ptr)
		{
			s_current_download_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
			SCI_ASSERT(PNULL != s_current_download_photo_ptr);
		}
		else
		{
			SCI_MEMSET(s_current_download_photo_ptr , 0x00 , sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
		}

		//store photo info
		SCI_MEMCPY((void*)s_current_download_photo_ptr->file_path , BAIDU_DRV_QRCODE_IMG_FILE_PATH , MMIAPICOM_Wstrlen(BAIDU_DRV_QRCODE_IMG_FILE_PATH) * sizeof(uint16));
		s_current_download_photo_ptr->file_path_len = MMIAPICOM_Wstrlen(BAIDU_DRV_QRCODE_IMG_FILE_PATH);
		s_current_download_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_current_download_photo_ptr->file_path, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);

		if(SFS_INVALID_HANDLE == s_current_download_photo_ptr->file_handle)
		{
			SCI_TRACE_LOW("[baidu]: baidu_drv_get_qrcode_img_req create file handle fail  !!!!");
			return FALSE;
		}

		MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, 0, SFS_SEEK_BEGIN);

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_GET_QR_IMG;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(s_baidu_drv_qr_code_url);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s" , s_baidu_drv_qr_code_url);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:baidu_drv_get_qrcode_img_req HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	
	MMIFILE_ERROR_E file_ret = SFS_ERROR_NONE;


	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_qrcode_img_ind_func"); 


	data_ind_sig = (HTTP_DATA_IND_SIG_T*)param;
	if(HTTP_RESPONSE_CODE_OK != data_ind_sig->response_code && 302 !=data_ind_sig->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != data_ind_sig->response_code)
		{
			SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_qrcode_img_ind_func  MSG_BAIDU_DRV_ERROR_NOTIFY ");
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		}
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_current_download_photo_ptr = PNULL;
		SCI_TRACE_LOW("[baidu]baidu_drv_get_qrcode_img_ind_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , data_ind_sig->response_code);
		return FALSE;
	}

	if(302 ==data_ind_sig->response_code)
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_get_qrcode_img_ind_func  302 , start get files ");
		return TRUE;
	}

      /*check file mem if enough*/
      if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(data_ind_sig->data_len))
      {
      		return FALSE;
      }
	file_ret = MMIAPIFMM_WriteFile(s_current_download_photo_ptr->file_handle,data_ind_sig->data_ptr, data_ind_sig->data_len,&write_size,NULL);
		
	if(SFS_ERROR_NONE == file_ret)
	{
		s_current_download_photo_ptr->saved_file_size +=  data_ind_sig->data_len;
		MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, s_current_download_photo_ptr->saved_file_size, SFS_SEEK_BEGIN);
		SCI_TRACE_LOW("[baidu]baidu_drv_get_qrcode_img_ind_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile s_current_download_photo_ptr->saved_file_size = %d " , s_current_download_photo_ptr->saved_file_size);
	}
	else
	{
		SCI_TRACE_LOW("[baidu]baidu_drv_get_qrcode_img_ind_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile FAIL , ret = %d " , file_ret);		
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_qrcode_img_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_qrcode_img_cnf_func"); 

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_qrcode_img_cnf_func has been canceled !!");
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_b_is_cancel_download_photo = FALSE;
			return FALSE;
	}

	MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
	SCI_FREE(s_current_download_photo_ptr);
	s_current_download_photo_ptr = PNULL;

	baidu_drv_msg_data.init_type = BAIDU_DRV_TYPE_LOGIN;
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_SHOW_QR , &baidu_drv_msg_data);		
	if (0 != s_query_qr_timer_id)
	{
		MMK_StopTimer(s_query_qr_timer_id);
		//s_query_qr_timer_id = 0;
	}

	s_query_qr_timer_id = MMK_CreateTimerCallback(MMI_1SECONDS * 5, baidu_GetCodeCallback, PNULL, TRUE);


	return TRUE;
}



/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_req(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

	SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_avatar_img_req !!");	

	BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
	context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
	BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

	if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  baidu_drv_get_avatar_img_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		if(PNULL == s_current_download_photo_ptr)
		{
			s_current_download_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
			SCI_ASSERT(PNULL != s_current_download_photo_ptr);
		}
		else
		{
			SCI_MEMSET(s_current_download_photo_ptr , 0x00 , sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
		}

		//store photo info
		SCI_MEMCPY((void*)s_current_download_photo_ptr->file_path , BAIDU_DRV_AVATAR_IMG_FILE_PATH , MMIAPICOM_Wstrlen(BAIDU_DRV_AVATAR_IMG_FILE_PATH) * sizeof(uint16));
		s_current_download_photo_ptr->file_path_len = MMIAPICOM_Wstrlen(BAIDU_DRV_AVATAR_IMG_FILE_PATH);
		s_current_download_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_current_download_photo_ptr->file_path, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);

		if(SFS_INVALID_HANDLE == s_current_download_photo_ptr->file_handle)
		{
			SCI_TRACE_LOW("[baidu]: baidu_drv_get_avatar_img_req create file handle fail  !!!!");
			return FALSE;
		}

		MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, 0, SFS_SEEK_BEGIN);

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_GET_AVATAR_IMG;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(s_baidu_user_info.avatar_url);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s" , s_baidu_user_info.avatar_url);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	
	MMIFILE_ERROR_E file_ret = SFS_ERROR_NONE;


	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_avatar_img_ind_func"); 


	data_ind_sig = (HTTP_DATA_IND_SIG_T*)param;
	if(HTTP_RESPONSE_CODE_OK != data_ind_sig->response_code && 302 !=data_ind_sig->response_code)
	{
#if 0	
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != data_ind_sig->response_code)
		{
			SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_avatar_img_ind_func  MSG_BAIDU_DRV_ERROR_NOTIFY ");
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		}
#endif		
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_current_download_photo_ptr = PNULL;
		SCI_TRACE_LOW("[baidu]baidu_drv_get_avatar_img_ind_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , data_ind_sig->response_code);
		return FALSE;
	}
	else
	{
		if(302 ==data_ind_sig->response_code)
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_get_avatar_img_ind_func  302 , start get files ");
			return TRUE;
		}

	      /*check file mem if enough*/
	      if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(data_ind_sig->data_len))
	      {
	      		return FALSE;
	      }
			  
		file_ret = MMIAPIFMM_WriteFile(s_current_download_photo_ptr->file_handle,data_ind_sig->data_ptr, data_ind_sig->data_len,&write_size,NULL);
			
		if(SFS_ERROR_NONE == file_ret)
		{
			s_current_download_photo_ptr->saved_file_size +=  data_ind_sig->data_len;
			MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, s_current_download_photo_ptr->saved_file_size, SFS_SEEK_BEGIN);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_avatar_img_ind_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile s_current_download_photo_ptr->saved_file_size = %d " , s_current_download_photo_ptr->saved_file_size);
		}
		else
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_get_avatar_img_ind_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile FAIL , ret = %d " , file_ret);		
		}
	}
	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_get_avatar_img_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_avatar_img_cnf_func"); 

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_get_avatar_img_cnf_func has been canceled !!");
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_b_is_cancel_download_photo = FALSE;
			return FALSE;
	}

	MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
	SCI_FREE(s_current_download_photo_ptr);
	s_current_download_photo_ptr = PNULL;

	return TRUE;
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_download_photo_req_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
       wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;
       wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
       MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
       MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_ERROR_NONE;

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_photo_req_func"); 

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_download_photo_req_func has been canceled !!");	
		Clean_download_data_info();
			return FALSE;
	}

#ifdef DEBUG_REFRESH
	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[baidu] baidu_drv_download_photo_req_func refresh token !!!");
		Reset_Http_all_request();
		//MMIAPIMSA_refresh_token(MSA_APP_TYPE_BAIDU_DRV ,MMISRV_BAIDUDRV_Get_Token);	
		s_baidu_resume_operation = BAIDU_DRV_OPER_DOWNLOAD_PHOTO ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif

	{
		data_ind_sig = (HTTP_DATA_IND_SIG_T*)param;
		if(HTTP_RESPONSE_CODE_OK != data_ind_sig->response_code && 302 !=data_ind_sig->response_code)
		{
			if(HTTP_RESPONSE_CODE_UNAUTHORIZED != data_ind_sig->response_code)
			{
				SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_photo_req_func  MSG_BAIDU_DRV_ERROR_NOTIFY ");
				Reset_all_data();
				baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
				MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			}
			MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
			MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
			SCI_FREE(s_current_download_photo_ptr);
			s_current_download_photo_ptr = PNULL;
			SCI_TRACE_LOW("[baidu]baidu_drv_download_photo_req_func error code MSG_BAIDU_DRV_ERROR_NOTIFY %d" , data_ind_sig->response_code);
			return FALSE;
		}

		if(302 ==data_ind_sig->response_code)
		{
			SCI_TRACE_LOW("[baidu]baidu_drv_download_photo_req_func  302 , start get files ");
			return TRUE;
		}

	      /*check file mem if enough*/
	      if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(data_ind_sig->data_len))
	      {
	      		return FALSE;
	      }
			  
		file_ret = MMIAPIFMM_WriteFile(s_current_download_photo_ptr->file_handle,data_ind_sig->data_ptr, data_ind_sig->data_len,&write_size,NULL);

		if(SFS_ERROR_NONE == file_ret)
		       {
			s_current_download_photo_ptr->saved_file_size +=  data_ind_sig->data_len;
			MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, s_current_download_photo_ptr->saved_file_size, SFS_SEEK_BEGIN);
			SCI_TRACE_LOW("[baidu]baidu_drv_download_photo_req_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile s_current_download_photo_ptr->saved_file_size = %d " , s_current_download_photo_ptr->saved_file_size);
		}
              else
              {
			SCI_TRACE_LOW("[baidu]baidu_drv_download_photo_req_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile FAIL , ret = %d " , file_ret);		
                     s_current_download_photo_ptr->saved_file_size = data_ind_sig->data_len;
              }

		baidu_drv_msg_data.total_file_size = s_current_download_photo_ptr->total_file_size;
		baidu_drv_msg_data.downloaded_file_size = s_current_download_photo_ptr->saved_file_size;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY , &baidu_drv_msg_data);
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    baidu_drv_download_photo_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar w_img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};	

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_download_photo_cnf_func"); 
	
	if(PNULL != s_current_download_photo_ptr)
	{
		baidu_drv_msg_data.file_len = MMIAPICOM_Wstrnlen(s_current_download_photo_ptr->file_path, MMIFILE_FULL_PATH_MAX_LEN);
		SCI_MEMCPY((void*)baidu_drv_msg_data.filename , s_current_download_photo_ptr->file_path , sizeof(wchar) * baidu_drv_msg_data.file_len);

		baidu_drv_msg_data.total_file_size = s_current_download_photo_ptr->total_file_size;
		baidu_drv_msg_data.downloaded_file_size = s_current_download_photo_ptr->saved_file_size;
		baidu_drv_msg_data.bDone = TRUE;

		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_download_photo_cnf_func file_len = %d",baidu_drv_msg_data.file_len);
		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_download_photo_cnf_func total_file_size = %d", s_current_download_photo_ptr->total_file_size);		

		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		s_current_download_photo_ptr->file_handle = 0;
		SCI_FREE(s_current_download_photo_ptr);
		s_current_download_photo_ptr = PNULL;

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[baidu]: Enter baidu_drv_download_photo_cnf_func has been canceled !!");
		   MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		s_b_is_cancel_download_photo = FALSE;
			return FALSE;
		}

	}

		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF , &baidu_drv_msg_data);

	if((BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) &&
	    (MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
	     MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo()))
	{
		MMISRV_BAIDUDRV_exec_auto_upload_photo();
	}	
	
	return TRUE;
}

LOCAL void baidu_drv_fill_default_info(BAIDU_DRV_OPERATION_E operation)
{

	SCI_TRACE_LOW("[baidu]baidu_drv_fill_default_info  operation = %d" , operation);
	switch(operation)
	{
		case BAIDU_DRV_OPER_GET_DISK_QUOTA:
			s_baidu_user_info.size_total = 9999;
			break;
		case BAIDU_DRV_OPER_GET_USER_INFO:
			{
			wchar * str_default_name = L"---------";
			MMI_WSTRNCPY (s_baidu_user_info.baidu_name, MAX_BAIDU_USER_INFO_STR_SIZE,
			  				str_default_name, MMIAPICOM_Wstrlen(str_default_name) , MMIAPICOM_Wstrlen(str_default_name));			
			}
			break;
		default:
			break;

	}
}


void MMISRV_BAIDUDRV_Parse_Http_Read_Upload_Session(char*  http_head_data_ptr)
{
	uint8  *tmp_ptr =NULL, *tmp_ptr1 = NULL;
	uint16  tmp_count = 0;
	char  tmp_value_ptr[64] = {0};

	SCI_TRACE_LOW("MMISRV_BAIDUDRV_Parse_Http_Read_Upload_Session.");
	/*check Content-Md5*/
	tmp_ptr = strstr(http_head_data_ptr, "Content-Md5");
       if(tmp_ptr)
       {
            SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr) /*find :"*/
            {
                if(0x3a == *tmp_ptr)   /*: = 0x3a*/
                {
                    tmp_count = 0;
			tmp_ptr++;		
			break;
                }
		  tmp_ptr++;
            }
		/*escape  space.*/
		while(0x20 == *tmp_ptr)
		{
			tmp_ptr++;
		}
		
		 tmp_ptr1 = tmp_ptr;
		while(*tmp_ptr)
		{
			if((0xd == *tmp_ptr)&&(0xa == *(tmp_ptr+1))) /*find end*/
			{
				break;
			}
			tmp_count++;
			tmp_ptr++;
		}
		
		SCI_TRACE_LOW("temp_count:%d", tmp_count);
             if(0 < tmp_count)
            {
	          SCI_MEMSET(s_pre_contest_md5, 0, (MMIFILE_FILE_MD5_LEN+1));
                 strncpy(s_pre_contest_md5, tmp_ptr1, tmp_count);
                 SCI_TRACE_LOW("[baidu] s_pre_contest_md5: %s :%s" , s_pre_contest_md5, tmp_ptr1);
            }
		else
		{
			SCI_TRACE_LOW("tmp_count = 0: ");
		}
        }
	  else
	  {
	  	SCI_TRACE_LOW("NOT FIND Content-Md5");
	  }

	/*check X-Bs-File-Size*/
	tmp_ptr = strstr(http_head_data_ptr, "X-Bs-File-Size");
       if(tmp_ptr)
       {
            SCI_TRACE_LOW("[baidu] tmp_ptr1: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find :"*/
            {
                if(0x3a == *tmp_ptr)   /*: = 0x3a*/
                {
                    tmp_count = 0;
			tmp_ptr++;		
			break;
                }
            }
		/*escape  space.*/
		while(0x20 == *tmp_ptr)
		{
			tmp_ptr++;
		}
		
		 tmp_ptr1 = tmp_ptr;
		while(*tmp_ptr)
		{
			if((0xd == *tmp_ptr)&&(0xa == *(tmp_ptr+1))) /*find end*/
			{
				break;
			}
			tmp_count++;
			tmp_ptr++;
		}
		
		SCI_TRACE_LOW("temp_count1:%d", tmp_count);
		if(0 < tmp_count)
		{
			strncpy(tmp_value_ptr, tmp_ptr1, tmp_count);
			g_x_bs_file_size = byte_atoi(tmp_value_ptr);
			SCI_TRACE_LOW("[baidu] s_pre_contest_md5: %s :%s:%d" , tmp_value_ptr, tmp_ptr1,g_x_bs_file_size);
		}
		else
		{
			SCI_TRACE_LOW("tmp_count1 = 0");
		}
        }
	  else
	  {
	  	SCI_TRACE_LOW("NOT FIND X-Bs-File-Size");
	  }

     SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_Parse_Http_Read_Upload_Session end.");
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRV_BAIDUDRV_HandleHTTPMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    //void    *original_param = (void*)((uint32)param-sizeof(xSignalHeaderRec));
	void    *original_param = (void*)((uint32)param);	
	uint8 i=0;
	BOOLEAN bIsFound = FALSE;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T sig_data={0};	
	uint8 operation = 0;

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
		{
			HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
			if(cnf->module_id != g_current_module_id || cnf->app_param >= MAX_HTTP_CONTEXT_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			if( 0 != http_request_operation_array[cnf->app_param].context_id)
			{
				SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_InitHttpRequest request too much context , idx %d , context_id %d , close it !", cnf->app_param , http_request_operation_array[cnf->app_param].context_id);
				HTTP_CloseRequest(http_request_operation_array[cnf->app_param].context_id);
			}

			SCI_MEMSET((void*)&http_request_operation_array[cnf->app_param] , 0x00 , sizeof(MMISRV_BAIDUDRV_REQUEST_ARRAY_T));

			for(i=0;i<MAX_REQUEST_NUM;i++)
			{
				http_request_operation_array[cnf->app_param].operation_ary[i].operation= BAIDU_DRV_OPER_NO_OPERATION;
			}			
			
			http_request_operation_array[cnf->app_param].context_id   = cnf->context_id;
			http_request_operation_array[cnf->app_param].app_instance = 0;
			http_request_operation_array[cnf->app_param].module_id    = cnf->module_id;

			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF index %d , context_id %d",cnf->app_param , cnf->context_id);

			if(!s_baidu_task_init_done)
			{
				s_baidu_task_init_done = TRUE;
				SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF s_auto_upload_status %d ",s_auto_upload_status);			

				if(BAIDU_DRV_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO != s_baidu_drv_init_from)
				{
					sig_data.init_type = s_baidu_drv_init_from;
					MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_INIT_CNF , &sig_data);		
					SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF SEND MSG_BAIDU_DRV_INIT_CNF !!!");			
				}
				else
				{
					SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_INIT_CNF MMISRV_BAIDUDRV_exec_auto_upload_photo !!!");					  
					MMISRV_BAIDUDRV_exec_auto_upload_photo();
				}
			}
			result = MMI_RESULT_TRUE;
		}
        break;

    case HTTP_SIG_NEED_AUTH_IND:
		{
			MMI_RESULT_E result = MMI_RESULT_TRUE;
			HTTP_AUTH_IND_SIG_T *cnf = (HTTP_AUTH_IND_SIG_T *)original_param;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND");        

			s_baidu_resume_operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;

			// Cancel first 
			Reset_Http_all_request();
			s_is_process_download_thumbnail = FALSE;	   
			s_is_process_get_delta_link = FALSE;			
			
			//token issue , modify later
			//MMIAPIMSA_refresh_token(MSA_APP_TYPE_BAIDU_DRV ,MMISRV_BAIDUDRV_Get_Token);	

			result = MMI_RESULT_TRUE;
		}
        break;

    case HTTP_SIG_REQUEST_ID_IND:
	{
	    MMI_RESULT_E result = MMI_RESULT_TRUE;
	    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T *)original_param;

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
		return MMI_RESULT_FALSE;
		}

		http_request_operation_array[i].operation_ary[cnf->app_instance].request_id = cnf->request_id;		
	}
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND");
		result = MMI_RESULT_TRUE;
        break;

    case HTTP_SIG_GET_CNF:
		{
			HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)original_param;
			
			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
            {
                return MMI_RESULT_FALSE;
            }

	              SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_GET_CNF");

			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;
				
			if(PNULL != http_get_cnf_function[operation])
			{
				http_get_cnf_function[operation](i , cnf);
			}
			
			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_GET_CNF index %d , error_code = %d , response code = %d" , i , result , cnf->rsp_header_info.response_code);        
			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_GET_CNF  operation:%d" , operation); 
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			http_request_operation_array[i].operation_ary[cnf->app_instance].operation = BAIDU_DRV_OPER_NO_OPERATION;
		}
        break;

    case HTTP_SIG_HEAD_CNF:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_HEAD_CNF");        
        break;

    case HTTP_SIG_DATA_IND:
		{
			HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;	
			uint32 data_len = 0;
			uint8 *data_ptr = PNULL;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;

			if(HTTP_RESPONSE_CODE_UNAUTHORIZED == cnf->response_code)
			{
				SCI_TRACE_LOW("[baidu]: token expired !!! , need to refresh token"); 
				return MMI_RESULT_TRUE; // need refresh token
			}

			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_DATA_IND index %d, operation:%d" , i, operation); 

			if(PNULL !=http_data_get_ind_function[operation])
			http_data_get_ind_function[operation](i , cnf);						       
		}
        break;

    case HTTP_SIG_HEADER_IND:
	{
		HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)original_param;
		
		SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_HEADER_IND:%d", cnf->context_id);

              for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;


			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_HEADER_IND index %d, operation:%d" , i, operation);

			
	             if(BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION == operation)
	             	{
	             	       MMISRV_BAIDUDRV_Parse_Http_Read_Upload_Session(cnf->header_ptr);
	             	}
    	   }
        break;

    case HTTP_SIG_ERROR_IND:
	{
		HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;
		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
			return MMI_RESULT_FALSE;
		}		

		//HTTP_CloseRequest(cnf->context_id);
		operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;
		SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_ERROR_IND , MSG_BAIDU_DRV_ERROR_NOTIFY result = %d" , cnf->result);
		SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_ERROR_IND index %d, operation:%d" , i, operation); 

		if((PNULL != s_current_download_photo_ptr) && (SFS_INVALID_HANDLE != s_current_download_photo_ptr->file_handle))
		{
			MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
			MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
			SCI_FREE(s_current_download_photo_ptr);
			s_current_download_photo_ptr = PNULL;
		}


		if(	BAIDU_DRV_OPER_GET_DISK_QUOTA == operation ||
			BAIDU_DRV_OPER_GET_AVATAR_IMG == operation ||
			BAIDU_DRV_OPER_GET_USER_INFO == operation )
		{
			baidu_drv_fill_default_info(operation);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			http_request_operation_array[i].operation_ary[cnf->app_instance].operation = BAIDU_DRV_OPER_NO_OPERATION;						
		}
		else
		{
			Reset_all_data();
		}
		
		sig_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;	
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
	}
        break;

    case HTTP_SIG_CLOSE_CNF:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_CLOSE_CNF");
        break;

    case HTTP_SIG_CANCEL_CNF:
	{
	        HTTP_CANCEL_CNF_SIG_T *cnf = (HTTP_CANCEL_CNF_SIG_T*)original_param;		
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_CANCEL_CNF");

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}
		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
			return MMI_RESULT_FALSE;
		}	

		SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_CANCEL_CNF index %d " , i );        
		SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
		SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
                http_request_operation_array[i].operation_ary[cnf->app_instance].operation = BAIDU_DRV_OPER_NO_OPERATION;
    	}
        break;

    case HTTP_SIG_NET_PROG_IND:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_NET_PROG_IND");
        break;

    case HTTP_SIG_REDIRECT_IND:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_REDIRECT_IND");        
        break;

    case HTTP_SIG_POST_CNF:
		{
			HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)original_param;
			uint8 operation = 0;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}
			
			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_POST_CNF");   
			
			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;
					
			if(PNULL !=http_get_cnf_function[operation])
				http_get_cnf_function[operation](i , cnf);

			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
                        http_request_operation_array[i].operation_ary[cnf->app_instance].operation = BAIDU_DRV_OPER_NO_OPERATION;
		}
        break;

#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND");
        break;
#endif

#if defined (MSA_SUPPORT)
    case MSG_MS_ACCOUNT_SIGNOFF_CNF:
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_SIGNOFF_CNF , &sig_data);
	 break;
#endif

    default:
        SCI_TRACE_LOW("[baidu]HandleHttpMsg Not handle msg_id = %x");
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_Token_Data( uint8* data_ptr,uint32 data_len )
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	   
	if(PNULL == data_ptr || 0 == data_len)
				return 0;

	SCI_TRACE_LOW("[baidu] Enter MMISRV_BAIDUDRV_Get_Token_Data");

	SCI_TraceBuf("[baidu] MMISRV_BAIDUDRV_Get_Token_Data data: ", data_ptr, data_len);


	baidu_drv_InitjsonConfig(&jc,MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack,&input_data);

	jp_ptr = new_JSON_parser(&jc); 

	if(PNULL == jp_ptr)
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Token_Data PNULL == jp_ptr");
		//	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		return FALSE;
	}
	if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
	{
		//weather_data.is_bad_data = TRUE;
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Token_Data parse error!");
	}
	delete_JSON_parser(jp_ptr); 

	return input_data.size;
}

/**************************************************************************************/
// Description : MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind( uint8* data_ptr,uint32 data_len )
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
	   
	if(PNULL == data_ptr || 0 == data_len)
				return 0;

	SCI_TRACE_LOW("[baidu] Enter MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind");

	baidu_drv_InitjsonConfig(&jc,MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack,&input_data);

	jp_ptr = new_JSON_parser(&jc); 

	if(PNULL == jp_ptr)
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind PNULL == jp_ptr");
		return FALSE;
	}
	if(0 == baidu_drv_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Prase_Upload_Create_Data_Ind parse error!");
	}
	delete_JSON_parser(jp_ptr); 

	return input_data.size;
}

/*****************************************************************************/
//  Description : parse json char one by one
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int baidu_drv_ParseJsonChar(JSON_parser jp_ptr , uint8* data_ptr , uint32 data_len)
{
    uint32 offset = 0;
    int  c = -1;
    int parse_res = 0;

    SCI_TRACE_LOW("[baidu]baidu_drv_ParseJsonChar ENTER");
    
    for(offset = 0; offset < data_len; offset++)
    {
        c = (*(data_ptr+offset))& 0x000000ff;
        parse_res = JSON_parser_char(jp_ptr, c);
        if(0 == parse_res)
        {            
            SCI_TRACE_LOW("[baidu]baidu_drv_ParseJsonChar break !! jp_ptr->error = %d " , JSON_parser_get_last_error(jp_ptr));
	     break;
        }
    }
    return parse_res;
}

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;

	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

	switch(type) 
	{
	case JSON_T_STRING:         // 10
	{
		if (JSON_TAG_ACCESSTOKEN == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_drv_access_token_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);	
			strncpy(s_baidu_drv_access_token_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack get access token %s " , s_baidu_drv_access_token_str);			
		}
		else if (JSON_TAG_REFRESHTOKEN == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_drv_refresh_token_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);	
			strncpy(s_baidu_drv_refresh_token_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack get refresh token %s " , s_baidu_drv_refresh_token_str);			
		}		
		else if (JSON_TAG_SESSION_KEY == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_drv_session_key_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);	
			strncpy(s_baidu_drv_session_key_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack get session key %s " , s_baidu_drv_session_key_str);			
		}			
		else if (JSON_TAG_SESSION_SECRET == baidu_drv_data_ptr->cur_tag)
		{
			SCI_MEMSET(s_baidu_drv_session_secret_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);	
			strncpy(s_baidu_drv_session_secret_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
			SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack get session secret %s " , s_baidu_drv_session_secret_str);			
		}				
	}
	
	break;
       case JSON_T_INTEGER:  //5
       {
            if(JSON_TAG_EXPIRES_IN == baidu_drv_data_ptr->cur_tag)
            {
                s_tocken_expires_in = value->vu.integer_value;
                SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Get_Token_Data_CallBack expires_in: %d" , s_tocken_expires_in);
            }
       }
       break;
       
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}
/*****************************************************************************/
//  Description : JsonParse_Get_Upload_ID_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;

	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

       SCI_TRACE_LOW("type:%d, tag:%d, value:%s ", type,baidu_drv_data_ptr->cur_tag, value);
	switch(type) 
	{
            case JSON_T_STRING:         // 10
            {
                if (JSON_TAG_UPLOAD_ID == baidu_drv_data_ptr->cur_tag)
                {
                    SCI_MEMSET(s_baidu_drv_upload_id_str , 0x00 , BAIDU_UPLOAD_ID_MAX_SIZE + 1);	
                    strncpy(s_baidu_drv_upload_id_str , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
                    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack get upload id: %s " , s_baidu_drv_upload_id_str);			
                }
            }
	
            break;
            case JSON_T_INTEGER:  //5
            {
                if(JSON_TAG_RETURN_TYPE == baidu_drv_data_ptr->cur_tag)
                {
                    G_UPLOAD_START_return_type = value->vu.integer_value;
                    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack return_type: %d" , G_UPLOAD_START_return_type);			
                }
		  else if(JSON_TAG_ERRNO == baidu_drv_data_ptr->cur_tag)
                {
                    g_baidu_upload_errno = value->vu.integer_value;
                    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack g_baidu_upload_errno: %d" , g_baidu_upload_errno);			
                }
		  else if(JSON_TAG_ERROR_CODE == baidu_drv_data_ptr->cur_tag)
		  {
		  	g_baidu_upload_error_code = value->vu.integer_value;
                    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_JsonParse_Upload_Create_Data_Ind_CallBack g_baidu_upload_error_code: %d" , g_baidu_upload_error_code);	
		  }
		  
            }
            break;
            case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
		break;
	default:
	    res = -1;
	    break;
	} 
	return res;
}
/*****************************************************************************/
//  Description : JsonParse_Get_Upload_ID_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Close_Session_CallBack(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;
#if 0
	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	

       SCI_TRACE_LOW("type:%d, tag:%d, value:%s ", type,baidu_drv_data_ptr->cur_tag, value);
	switch(type) 
	{
	case JSON_T_STRING:         // 10
	{
		;
	}
	
	break;
	case JSON_T_KEY:            // 11
		{
			uint8 i=0;
			BOOLEAN b_is_found = FALSE;
			for(i=0 ; i <JSON_TAG_MAX;i++)
			{
			    if(Handle_JSON_Is_Tag_Equal(i , value))
			    {
				baidu_drv_data_ptr->cur_tag = i;
				b_is_found = TRUE;
				break;
			    }   
			}
			if(FALSE ==b_is_found)
			{
				baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
			}
                     //if (JSON_TAG_FSID == baidu_drv_data_ptr->cur_tag)
                    //{
                     //  SCI_MEMSET(s_baidu_drv_upload_photo_ptr->file_fs_id, 0x00 , MMIFILE_FS_ID_LEN_MAX+ 1);	
                      //  strncpy(s_baidu_drv_upload_photo_ptr->file_fs_id , (uint8*)(((*value).vu).str).value , (uint8*)(((*value).vu).str).length);
                     //   SCI_TRACE_LOW("[baidu] FS_ID: %s " , s_baidu_drv_upload_photo_ptr->file_fs_id);			
                   // }
		}
		break;
	default:
	    res = -1;
	    break;
	} 
    #endif
	return res;
}
/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Handle_JSON_Is_Tag_Equal(uint8 index , const JSON_value* value)
{
	if(0 == strncmp((char*)s_baidu_drv_json_tag_arr[index], (char*)(((*value).vu).str).value, (((*value).vu).str).length))
        {
		return TRUE;
	}

	return FALSE;
}
//#define UPDATE_VALID
#if 1 //def  UPDATE_VALID
#define FILE_NAME_LEN  19
/*check the file name whether complies with the naming rules*/
PUBLIC  BOOLEAN MMIAPISRV_BAIDUSRV_Check_Name(uint8* name)
{
	/*check the len*/
	SCI_TRACE_LOW("[baidu]:name:%s, len:%d", name, strlen(name));
	if(FILE_NAME_LEN != strlen(name))
	{
		return FALSE;
	}
	/*check the naming rules*/
	if(NULL == strstr(name, "-"))
	{
		return FALSE;
	}
	return TRUE;
}

/*return need update num.*/
PUBLIC uint16  MMIAPISRV_BAIDUSRV_Update_Thumb()
{
        uint32  file_size = 0, read_size;
        uint8 *buff =NULL;
	 MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	 uint16  update_thumb_num = 0, down_thumb_num = 0;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	

         SCI_TRACE_LOW("[baidu] ENTER MMIAPISRV_BAIDUSRV_Update_Thumb, s_get_category_list_cusor:%d,", s_get_category_list_cusor);  
	/*read meta list.*/
	 /*1. get the thumb list*/
       if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
       {
              
        	if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        	{
        		s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
        		if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
        		{
        		       SCI_TRACE_LOW("[baidu] open meta info file error. ");
        			return 0;		
        		}
        	}
		else
		{ 
		       SCI_TRACE_LOW("[baidu] meta info file not exist. ");
			   //SCI_ASSERT(0);
			return 0;
		}
            
       }
       else
        {
            MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_BEGIN);
        }

       /*3. download the before n thumb.*/
       file_size = MMIAPIFMM_GetFileSize(s_meta_data_file_handle);
	if(0 == file_size)
       {
              SCI_TRACE_LOW("[baidu] meta info file size is 0. ");
       	return 0;
       }
	buff = SCI_ALLOC_APPZ(file_size);
    
	SCI_TRACE_LOW("[baidu] MMIAPISRV_BAIDUSRV_Update_Thumb, FILE_SIZE:%d,", file_size);   
	if(PNULL == buff)
	{
		SCI_TRACE_LOW("[baidu]:MMIAPISRV_BAIDUSRV_Update_Thumb --- (PNULL == buff)"); 
		return 0;
	}

	file_ret =  MMIAPIFMM_ReadFile(
	    s_meta_data_file_handle, 
	    buff, 
	    file_size, 
	    &read_size, 
	    NULL
	    );
	
	if(SFS_ERROR_NONE == file_ret)
	{
		SCI_TRACE_LOW("[baidu] MMIAPISRV_BAIDUSRV_Update_Thumb read meta success !!");
	}
	
	MMIAPIFMM_CloseFile(s_meta_data_file_handle);
	s_meta_data_file_handle = SFS_INVALID_HANDLE ;
	
	update_thumb_num = Generate_Download_thumbnail_node_by_JSON(buff , file_size, 12);

       SCI_FREE(buff);

	if(PNULL != s_baidu_drv_metadata_ptr)
	{     							   
	       s_current_download_thumbnail_count = MMISRV_BAIDUDRV_start_download_thumbnails();
		s_download_thumbnail_total_count = s_total_file_num;
		baidu_drv_msg_data.total_num = s_download_thumbnail_total_count ;
	       baidu_drv_msg_data.curr_download_num = s_current_download_thumbnail_count;

	   
		   MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY , &baidu_drv_msg_data);
	
		   s_download_thumbnail_fail_count =0;
		   s_download_thumbnail_success_count =0;
		   s_is_process_download_thumbnail = TRUE;			   
	}
	   
	SCI_TRACE_LOW("[baidu] END MMIAPISRV_BAIDUSRV_Update_Thumb. %d, %d", update_thumb_num, s_current_download_thumbnail_count);
	return update_thumb_num;
}

PUBLIC FILEARRAY MMIAPISRV_BAIDUSRV_GET_All_Thumb_File_array()
{
	return s_baidu_drv_all_thumb_file_array;
}

PUBLIC FILEARRAY MMIAPISRV_BAIDUSRV_GET_Exist_Thumb_File_array()
{
	return s_baidu_drv_exist_thumb_file_array;
}

LOCAL BOOLEAN MMIAPISRV_BAIDUSRV_Check_Thumbnail_Exist(const uint8 *filename)
{
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	FILEARRAY_DATA_T file_data = {0};

	/*if not complies with the naming rules, return TRUE, not handle this file*/
	if(FALSE == MMIAPISRV_BAIDUSRV_Check_Name(filename))
	{
	       s_invalid_name_cnt++;
		SCI_TRACE_LOW("[baidu]:this file not compllies with the naming, error cnt:%d.,  file:%s", s_invalid_name_cnt, filename);
		
		return TRUE;
	}

	file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL ;     
	}
	
	/*if have exit, not download again*/
	MMIAPICOM_StrToWstr(filename , img_file_name);

	MMIAPIFMM_CombineFullPathEx(save_path, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path, img_file_name); 
#if 1
	SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
		
	file_data.type = FILEARRAY_TYPE_FILE;
	MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
					save_path, MMIFILE_FULL_PATH_MAX_LEN,  MMIAPICOM_Wstrlen(save_path));/*lint !e670 */
	file_data.name_len = MMIAPICOM_Wstrlen(save_path);

	MMIAPIFMM_GetFileInfo(file_data.filename, file_data.name_len, 
	&file_data.size, PNULL, PNULL);

	if(PNULL == s_baidu_drv_all_thumb_file_array)
	{
		s_baidu_drv_all_thumb_file_array = MMIAPIFILEARRAY_Create();
		MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_all_thumb_file_array, 20);
	}
	MMIAPIFILEARRAY_Add (s_baidu_drv_all_thumb_file_array, &file_data);
#endif
	if(MMIAPIFMM_IsFileExist(save_path, MMIAPICOM_Wstrlen(save_path)))
	{
		if(PNULL == s_baidu_drv_exist_thumb_file_array)
		{
			s_baidu_drv_exist_thumb_file_array = MMIAPIFILEARRAY_Create();
			MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_exist_thumb_file_array, 20);
		}
#if 0	
		SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
			
		file_data.type = FILEARRAY_TYPE_FILE;
		MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
						save_path, MMIFILE_FULL_PATH_MAX_LEN,  MMIAPICOM_Wstrlen(save_path));/*lint !e670 */
		file_data.name_len = MMIAPICOM_Wstrlen(save_path);

		MMIAPIFMM_GetFileInfo(file_data.filename, file_data.name_len, 
		&file_data.size, PNULL, PNULL);
#endif

		MMIAPIFILEARRAY_Add (s_baidu_drv_exist_thumb_file_array, &file_data);
       	return TRUE;
	}
	else
	{
		return  FALSE;
	}
}

LOCAL uint8 Generate_Download_thumbnail_node_by_JSON( const uint8* data_ptr,uint32 data_len , uint16 num)
{
       uint8 *tmp_ptr = NULL, *tmp_ptr1=NULL, *tmp_data_ptr = NULL;
       uint8 tmp_count = 0;
       uint32 templen = 0;
       MMISRV_BAIDUDRV_METADATA * tmp_metadata_ptr = PNULL,  * tmp1_metadata_ptr = PNULL;
       uint8 * current_tag_ptr = PNULL;
       uint32   str_len = 0;
       uint8 *jsonptr = NULL;
	//uint8  exist_flag = 0;
       
        SCI_TRACE_LOW("[baidu] Generate_Download_thumbnail_node_by_JSON");
	//delete meta data first 
      Clean_meta_data_info();
      s_current_download_thumbnail_count = 0;
	s_invalid_name_cnt = 0;
	if(s_baidu_drv_exist_thumb_file_array)
	{
		MMIAPIFILEARRAY_Destroy(&s_baidu_drv_exist_thumb_file_array);
		s_baidu_drv_exist_thumb_file_array = PNULL;
	}
	if(s_baidu_drv_all_thumb_file_array)
	{
		MMIAPIFILEARRAY_Destroy(&s_baidu_drv_all_thumb_file_array);
		s_baidu_drv_all_thumb_file_array = PNULL;
	}
	
    tmp_ptr = strstr(data_ptr, "list");
    tmp_data_ptr = tmp_ptr;
    while(data_len > 0)
    {
         SCI_TRACE_LOW("[baidu] len:%d, 122:%s", data_len, tmp_data_ptr);
         if((0 < num)&&(s_current_download_thumbnail_count >=num ))
         {
            SCI_TRACE_LOW("[baidu] Generate_Download_thumbnail_node_by_JSON. have reach to the max num:%d exit." , num);
            break;
         }
         /*find list*/
         tmp_ptr = tmp_data_ptr;
         templen = data_len;
         while(0 < templen)
         {
             if(0x7B == *tmp_ptr) /*{*/
             {
                tmp_data_ptr = tmp_ptr+1;
                data_len = templen -1;
                 break;
             }
             tmp_ptr++;
             templen--;
         };
         if(0 == templen)
         {
             break;
         }
        #if 0
        if(0 == exist_flag)
        {
	        if(PNULL == s_baidu_drv_metadata_ptr)
	        {
	            s_baidu_drv_metadata_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_METADATA));
	            SCI_ASSERT(PNULL != s_baidu_drv_metadata_ptr);
	            tmp_metadata_ptr = s_baidu_drv_metadata_ptr;
	         }
	         else
	         {
	             tmp_metadata_ptr = s_baidu_drv_metadata_ptr;
	             while(PNULL != tmp_metadata_ptr->next_ptr)
	             tmp_metadata_ptr = tmp_metadata_ptr->next_ptr;

	             tmp_metadata_ptr->next_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_METADATA));
	             SCI_ASSERT(PNULL != tmp_metadata_ptr->next_ptr);
	             tmp_metadata_ptr = tmp_metadata_ptr->next_ptr;
	          }
	          tmp_metadata_ptr->next_ptr =NULL;
	          s_current_download_thumbnail_count ++ ;
         }
	  exist_flag = 0;
	  #endif
	  if(!tmp_metadata_ptr)
	  {
	  	tmp_metadata_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_METADATA));
		SCI_ASSERT(PNULL != tmp_metadata_ptr);
	  }
	  
       /*filename: filename": "ftest.jpg"*/
        tmp_count =0;
        templen =0;
        tmp_ptr = strstr(tmp_data_ptr, "filename");
        if(tmp_ptr)
        {
            //SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find second "*/
            {
                if(*tmp_ptr == 0x22)
                {
                    tmp_count++;
                }
                if(2 == tmp_count)
                {
                    tmp_ptr++;
                    break;
                }
            }
            tmp_ptr1 = tmp_ptr;
            while(*tmp_ptr1 != 0x22) /*find the end*/
            {
                templen++;
                tmp_ptr1++;
            }
            {
                 //SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                     
                /*record thumb uri.*/
                str_len = MIN(BAIDUDRIVE_METADATA_STRING_MAX_LEN, templen);
                current_tag_ptr = SCI_ALLOC_APPZ(str_len + 1);
		  SCI_ASSERT(PNULL != current_tag_ptr);
                strncpy(current_tag_ptr ,  tmp_ptr, str_len);
		  if(MMIAPISRV_BAIDUSRV_Check_Thumbnail_Exist(current_tag_ptr))
		  {
		  	SCI_FREE(current_tag_ptr);
                     current_tag_ptr = NULL;
					 
			/*find next list*/
			tmp_ptr = tmp_data_ptr;
			templen = data_len;
			while(0 < templen)
			{
				if(0x7D == *tmp_ptr) /*}*/
				{
					tmp_data_ptr = tmp_ptr+1;
					data_len = templen-1;
					break;
				}
				tmp_ptr++;
				templen--;
			}
			continue;
		  }
                
                tmp_metadata_ptr->name = current_tag_ptr;
            }
        }
        else
        {
            //SCI_ASSERT(0);
            break;
        }
       
         /*icon: "icon": "https:*/
         tmp_count =0;
         templen =0;
         //tmp_ptr = strstr(tmp_data_ptr, "icon");
         tmp_ptr = strstr(tmp_data_ptr, "url1");
         if(tmp_ptr)
         {
            //SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find second "*/
            {
                if(*tmp_ptr == 0x22)
                {
                    tmp_count++;
                }
                if(2 == tmp_count)
                {
                    tmp_ptr++;
                    break;
                }
            }
            tmp_ptr1 = tmp_ptr;
            jsonptr = tmp_ptr;
            while(*tmp_ptr1 != 0x22) /*find the end*/
            {
                if((0x5C == *tmp_ptr1)&&(0x75 == *(tmp_ptr1+1))&&
                    (0x30 == *(tmp_ptr1+2))&& (0x30 == *(tmp_ptr1+3))&&
                    (0x32 == *(tmp_ptr1+4))&& (0x36 == *(tmp_ptr1+5)))
                {
                    *jsonptr = 0x26;
                    tmp_ptr1++;  //0x75
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x32
                    tmp_ptr1++;  //0x36
                }
                else
                {
                    *jsonptr = *tmp_ptr1;
                }
                templen++;
                tmp_ptr1++;
                jsonptr++;
            }
            {
                 //SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                     
                /*record thumb uri.*/
                str_len = MIN(BAIDUDRIVE_METADATA_STRING_MAX_LEN, templen);
                current_tag_ptr = SCI_ALLOC_APPZ(str_len + 1);
		  SCI_ASSERT(PNULL != current_tag_ptr);
                strncpy(current_tag_ptr ,  tmp_ptr, str_len);
                
                tmp_metadata_ptr->thumbnail_url = current_tag_ptr;
            }
        }
       else
       {
           //SCI_ASSERT(0);
           break;
       }


    if(PNULL == s_baidu_drv_metadata_ptr)
    {
        s_baidu_drv_metadata_ptr = tmp_metadata_ptr;
        SCI_ASSERT(PNULL != s_baidu_drv_metadata_ptr);
        tmp1_metadata_ptr = s_baidu_drv_metadata_ptr;
     }
     else
     {
         tmp1_metadata_ptr = s_baidu_drv_metadata_ptr;
         while(PNULL != tmp1_metadata_ptr->next_ptr)
         tmp1_metadata_ptr = tmp1_metadata_ptr->next_ptr;

         tmp1_metadata_ptr->next_ptr = tmp_metadata_ptr;
         SCI_ASSERT(PNULL != tmp1_metadata_ptr->next_ptr);
         tmp1_metadata_ptr = tmp1_metadata_ptr->next_ptr;
      }
      tmp1_metadata_ptr->next_ptr =NULL;
      tmp_metadata_ptr = NULL;
      s_current_download_thumbnail_count ++ ;

        /*find next list*/
       tmp_ptr = tmp_data_ptr;
       templen = data_len;
       while(0 < templen)
       {
            if(0x7D == *tmp_ptr) /*}*/
            {
                tmp_data_ptr = tmp_ptr+1;
                data_len = templen-1;
                break;
            }
            tmp_ptr++;
            templen--;
       }
      }
	if(tmp_metadata_ptr)
	{
		SCI_Free(tmp_metadata_ptr);
	}
     SCI_TRACE_LOW("[baidu]  Generate_Download_thumbnail_node_by_JSON  s_current_download_thumbnail_count = %d" , s_current_download_thumbnail_count);

    return s_current_download_thumbnail_count;
}

#else


/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint8 Generate_Download_thumbnail_node_by_JSON( const uint8* data_ptr,uint32 data_len , uint16 num)
{
       uint8 *tmp_ptr = NULL, *tmp_ptr1=NULL, *tmp_data_ptr = NULL;
       uint8 tmp_count = 0;
       uint32 templen = 0;
       MMISRV_BAIDUDRV_METADATA * tmp_metadata_ptr = PNULL;
       uint8 * current_tag_ptr = PNULL;
       uint32   str_len = 0;
       uint8 *jsonptr = NULL;
       
        SCI_TRACE_LOW("[baidu] Generate_Download_thumbnail_node_by_JSON");
	//delete meta data first 
    Clean_meta_data_info();
    s_current_download_thumbnail_count = 0;
    tmp_ptr = strstr(data_ptr, "list");
    tmp_data_ptr = tmp_ptr;
    while(data_len > 0)
    {
         SCI_TRACE_LOW("[baidu] len:%d, 122:%s", data_len, tmp_data_ptr);
         if((0 < num)&&(s_current_download_thumbnail_count >num ))
         {
            SCI_TRACE_LOW("[baidu] Generate_Download_thumbnail_node_by_JSON. have reach to the max num:%d exit." , num);
            break;
         }
         /*find list*/
         tmp_ptr = tmp_data_ptr;
         templen = data_len;
         while(0 < templen)
         {
             if(0x7B == *tmp_ptr) /*{*/
             {
                tmp_data_ptr = tmp_ptr+1;
                data_len = templen -1;
                 break;
             }
             tmp_ptr++;
             templen--;
         };
         if(0 == templen)
         {
             break;
         }
        
        if(PNULL == s_baidu_drv_metadata_ptr)
        {
            s_baidu_drv_metadata_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_METADATA));
            SCI_ASSERT(PNULL != s_baidu_drv_metadata_ptr);
            tmp_metadata_ptr = s_baidu_drv_metadata_ptr;
         }
         else
         {
             tmp_metadata_ptr = s_baidu_drv_metadata_ptr;
             while(PNULL != tmp_metadata_ptr->next_ptr)
             tmp_metadata_ptr = tmp_metadata_ptr->next_ptr;

             tmp_metadata_ptr->next_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_METADATA));
             SCI_ASSERT(PNULL != tmp_metadata_ptr->next_ptr);
             tmp_metadata_ptr = tmp_metadata_ptr->next_ptr;
          }
          tmp_metadata_ptr->next_ptr =NULL;
          s_current_download_thumbnail_count ++ ;
       /*filename: filename": "ftest.jpg"*/
        tmp_count =0;
        templen =0;
        tmp_ptr = strstr(tmp_data_ptr, "filename");
        if(tmp_ptr)
        {
            //SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find second "*/
            {
                if(*tmp_ptr == 0x22)
                {
                    tmp_count++;
                }
                if(2 == tmp_count)
                {
                    tmp_ptr++;
                    break;
                }
            }
            tmp_ptr1 = tmp_ptr;
            while(*tmp_ptr1 != 0x22) /*find the end*/
            {
                templen++;
                tmp_ptr1++;
            }
            {
                 //SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                     
                /*record thumb uri.*/
                str_len = MIN(BAIDUDRIVE_METADATA_STRING_MAX_LEN, templen);
                current_tag_ptr = SCI_ALLOC_APPZ(str_len + 1);
                strncpy(current_tag_ptr ,  tmp_ptr, str_len);
                
                tmp_metadata_ptr->name = current_tag_ptr;
            }
        }
        else
        {
            //SCI_ASSERT(0);
            break;
        }
       
         /*icon: "icon": "https:*/
         tmp_count =0;
         templen =0;
         tmp_ptr = strstr(tmp_data_ptr, "icon");
         if(tmp_ptr)
         {
            //SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find second "*/
            {
                if(*tmp_ptr == 0x22)
                {
                    tmp_count++;
                }
                if(2 == tmp_count)
                {
                    tmp_ptr++;
                    break;
                }
            }
            tmp_ptr1 = tmp_ptr;
            jsonptr = tmp_ptr;
            while(*tmp_ptr1 != 0x22) /*find the end*/
            {
                if((0x5C == *tmp_ptr1)&&(0x75 == *(tmp_ptr1+1))&&
                    (0x30 == *(tmp_ptr1+2))&& (0x30 == *(tmp_ptr1+3))&&
                    (0x32 == *(tmp_ptr1+4))&& (0x36 == *(tmp_ptr1+5)))
                {
                    *jsonptr = 0x26;
                    tmp_ptr1++;  //0x75
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x32
                    tmp_ptr1++;  //0x36
                }
                else
                {
                    *jsonptr = *tmp_ptr1;
                }
                templen++;
                tmp_ptr1++;
                jsonptr++;
            }
            {
                 //SCI_TRACE_LOW("[baidu] tmp_ptr: %d, 0x%x " , templen, tmp_ptr);
                     
                /*record thumb uri.*/
                str_len = MIN(BAIDUDRIVE_METADATA_STRING_MAX_LEN, templen);
                current_tag_ptr = SCI_ALLOC_APPZ(str_len + 1);
                strncpy(current_tag_ptr ,  tmp_ptr, str_len);
                
                tmp_metadata_ptr->thumbnail_url = current_tag_ptr;
            }
        }
       else
       {
           //SCI_ASSERT(0);
           break;
       }

        /*find next list*/
        tmp_ptr = tmp_data_ptr;
       templen = data_len;
       while(0 < templen)
       {
            if(0x7D == *tmp_ptr) /*}*/
            {
                tmp_data_ptr = tmp_ptr+1;
                data_len = templen-1;
                break;
            }
            tmp_ptr++;
            templen--;
       }
        }
     SCI_TRACE_LOW("[baidu]  Generate_Download_thumbnail_node_by_JSON  s_current_download_thumbnail_count = %d" , s_current_download_thumbnail_count);

    return s_current_download_thumbnail_count;
}
#endif
#if 0
/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_BAIDUDRV_JsonParse_Fetch_PhotoInfo_CallBack(void* ctx, int type, const JSON_value* value)
{
    int res = 1;
    uint16 str_len = 0;

	MMISRV_BAIDUDRV_JSON_PARSE_DATA * baidu_drv_data_ptr = (MMISRV_BAIDUDRV_JSON_PARSE_DATA *)ctx;	
        
    switch(type) 
    {
    case JSON_T_INTEGER:        // 5
       if(baidu_drv_data_ptr->bDone)
       {
        baidu_drv_data_ptr->size = ((*value).vu.integer_value);
		res = 0; // break JSON !!!!
       }
		break;
    case JSON_T_STRING:         // 10
	 if(JSON_TAG_NAME ==baidu_drv_data_ptr->cur_tag)
		{
		str_len = MIN(BAIDU_DRV_MAX_STRING_LEN,(((*value).vu).str).length);

		SCI_MEMSET(baidu_drv_data_ptr->name , 0x00 ,BAIDU_DRV_MAX_STRING_LEN + 1);
		strncpy(baidu_drv_data_ptr->name , (((*value).vu).str).value , str_len);
	 }
	 
		break;
    case JSON_T_KEY:            // 11
        if (Handle_JSON_Is_Tag_Equal(JSON_TAG_NAME , value))
        {
        	baidu_drv_data_ptr->cur_tag = JSON_TAG_NAME;
        }
	 else
	 {
        	baidu_drv_data_ptr->cur_tag = JSON_TAG_NONE;
	 }
        break;
    
    default:
        res = -1;
        break;
    } 
    return res;
} 
#endif
/*****************************************************************************/
//  Description : init json config
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void baidu_drv_InitjsonConfig(JSON_config* config_ptr,
                                    JSON_parser_callback callback,
                                    MMISRV_BAIDUDRV_JSON_PARSE_DATA *record_data_ptr)
{
    if(PNULL == config_ptr)
    {
        SCI_TRACE_LOW("[baidu]baidu_drv_InitjsonConfig PNULL == config_ptr");
        return;
    }
    init_JSON_config(config_ptr);

    config_ptr->callback = callback;
    config_ptr->allow_comments = TRUE;
    config_ptr->handle_floats_manually = TRUE;
    config_ptr->callback_ctx = record_data_ptr;
}
/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void baidu_drv_download_thumbnail_req(uint8 context_id , uint8  avaliable_request_idx)
{
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	SCI_TRACE_LOW("[baidu]:baidu_drv_download_thumbnail_req");	   
	get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
       SCI_ASSERT(PNULL != get_param_ptr);

	memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  //modify for coverity
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;

	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	get_param_ptr->recv_timeout = 60*1000*3 ; 	
    
       if(!s_baidu_drv_metadata_ptr)
       {
            SCI_ASSERT(0);
       }
	len=strlen(http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_url);
    
	get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(get_param_ptr->uri.uri_ptr,0,len+1);

	sprintf(get_param_ptr->uri.uri_ptr , "%s" , http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_url);
	
       //SCI_TRACE_LOW("uri_ptr:%s", get_param_ptr->uri.uri_ptr );
	if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
	{
		SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
	}

	SCI_FREE(get_param_ptr->uri.uri_ptr);
	SCI_FREE(get_param_ptr);

    return;	
}



/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRV_BAIDUDRV_start_download_thumbnails(void)
{
	uint i=0;
	BOOLEAN bIs_Found = FALSE;
    uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_METADATA *remove_ptr = PNULL;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;	   
	uint8 ret_val = 0;
       MMISRV_BAIDUDRV_MSG_T sig_data={0};
    
       SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_start_download_thumbnails");
	if(PNULL == s_baidu_drv_metadata_ptr)
		return 0 ;

 	/*if uploading, pause upload process*/
	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		SCI_TRACE_LOW("[baidu]: baidu_drv_download_thumbnail_info_cnf_func need download thumbnail !! Pause auto upload !!!");
		Clean_upload_data_info();			
	}      
	
	file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL ;     
	}	

	while(s_baidu_drv_metadata_ptr)
	{
		if(PNULL == s_baidu_drv_metadata_ptr->name || 0 == strlen(s_baidu_drv_metadata_ptr->name))
		{
			remove_ptr = s_baidu_drv_metadata_ptr;
			s_baidu_drv_metadata_ptr = s_baidu_drv_metadata_ptr->next_ptr;
			
			if(remove_ptr->id) SCI_FREE(remove_ptr->id);
			if(remove_ptr->name)SCI_FREE(remove_ptr->name);
			if(remove_ptr->thumbnail_url)SCI_FREE(remove_ptr->thumbnail_url);
			SCI_FREE(remove_ptr);	
			continue;
		}

 #ifdef WIN32 //ndef UPDATE_VALID
            /*if have exit, not download again*/

        SCI_MEMSET((void*)&save_path[0] , 0x00 , sizeof(wchar)*MMIFILE_FULL_PATH_MAX_LEN);    
		SCI_MEMSET((void*)&img_file_name[0] , 0x00 , sizeof(wchar)*MMIFILE_FULL_PATH_MAX_LEN);    
            MMIAPICOM_StrToWstr(s_baidu_drv_metadata_ptr->name , img_file_name);

            MMIAPIFMM_CombineFullPathEx(save_path, MMIFILE_FULL_PATH_MAX_LEN,file_dev,default_path, img_file_name);  
            if(MMIAPIFMM_IsFileExist(save_path, MMIAPICOM_Wstrlen(save_path)))
            {
		 SCI_MEMSET((void*)&sig_data , 0x00 , sizeof(MMISRV_BAIDUDRV_MSG_T));

                s_download_thumbnail_success_count++;
                sig_data.success_num = s_download_thumbnail_success_count;
                sig_data.total_num = s_download_thumbnail_total_count;
				sig_data.curr_download_num = s_current_download_thumbnail_count;
                sig_data.fail_num = s_download_thumbnail_fail_count;
				sig_data.bDone = TRUE;

                SCI_MEMCPY((void*)sig_data.filename , save_path , MMIAPICOM_Wstrlen(save_path) * sizeof(wchar));
                MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF , &sig_data);

                remove_ptr = s_baidu_drv_metadata_ptr;
                s_baidu_drv_metadata_ptr = s_baidu_drv_metadata_ptr->next_ptr;

                if(remove_ptr->id) SCI_FREE(remove_ptr->id);
                if(remove_ptr->name)SCI_FREE(remove_ptr->name);
                if(remove_ptr->thumbnail_url)SCI_FREE(remove_ptr->thumbnail_url);
                SCI_FREE(remove_ptr);	
                continue;
            }
 #endif
			
		BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
		context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
		BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

		if(0xFF != context_id)
		{
			//copy photo name
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL;
			strncpy((char*)&http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_string[0] , s_baidu_drv_metadata_ptr->name , strlen(s_baidu_drv_metadata_ptr->name));
			strncpy((char*)&http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_url[0] , s_baidu_drv_metadata_ptr->thumbnail_url , strlen(s_baidu_drv_metadata_ptr->thumbnail_url));

			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_string[strlen(s_baidu_drv_metadata_ptr->name)]  = 0;
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_url[strlen(s_baidu_drv_metadata_ptr->thumbnail_url)] = 0;

                     baidu_drv_download_thumbnail_req(context_id , avaliable_request_idx);
			ret_val ++ ; 

			remove_ptr = s_baidu_drv_metadata_ptr;
			s_baidu_drv_metadata_ptr = s_baidu_drv_metadata_ptr->next_ptr;
			
			if(remove_ptr->id) SCI_FREE(remove_ptr->id);
			if(remove_ptr->name)SCI_FREE(remove_ptr->name);
			if(remove_ptr->thumbnail_url)SCI_FREE(remove_ptr->thumbnail_url);
			SCI_FREE(remove_ptr);			
		}
		else
		{
			Clean_meta_data_info();
			SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_start_download_thumbnails -- too much photos , need 2nd round to finish it ");
			break;
		}
	    }

	return ret_val;
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetApnStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    uint8   index   = 0;

    index = s_baidu_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }

    return apn_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetUserNameStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   user_name_str = PNULL;
    uint8   index   = 0;

    index = s_baidu_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->username_len)
    {
        user_name_str = (char*)linksetting->username;
    }
    
    return user_name_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetPasswordStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   password_str = PNULL;
    uint8   index   = 0;

    index = s_baidu_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    
    return password_str;
}

/*****************************************************************************/
//  Description : pdp retry if it can't active
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaiduDrvPDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    uint8                setting_index = 0;

    setting_index = MMIAPICONNECTION_GetAccountDefaultIndex(sim_sys, MMICONNECTION_ACCOUNT_BROWSER);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, setting_index);
    if(setting_item_ptr)
    {
        active_info.app_handler = g_current_module_id;
        active_info.apn_ptr = (char*)setting_item_ptr->apn;
        active_info.user_name_ptr = (char*)setting_item_ptr->username;
        active_info.psw_ptr = (char*)setting_item_ptr->password;
        active_info.dual_sys = sim_sys;
        active_info.priority = setting_index;
        active_info.ps_service_rat = MN_UNSPECIFIED;
        active_info.handle_msg_callback = HandlePdpMsg;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = MMICONNECTION_IP_V4V6;//setting_item_ptr->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
            s_is_process_pdp_activate = TRUE;
        }
        SCI_TRACE_LOW("[baidu]:BaiduDrvPDPTry return_val = %d",return_val);	
    }	
    return return_val;
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void) 
{
   SCI_TRACE_LOW("[baidu]:StopPdpActiveTimer");

    if(0 != s_baidu_pdp_timer_id)
    {
        MMK_StopTimer(s_baidu_pdp_timer_id);
        s_baidu_pdp_timer_id = 0;
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    MMISRV_BAIDUDRV_MSG_T sig_data={0};
    
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- msg_id=%d",msg_ptr->msg_id);
    SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- result=%d" , msg_ptr->result);

    s_is_process_pdp_activate = FALSE;

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        {
            SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- MMIPDP_ACTIVE_CNF");
            StopPdpActiveTimer();
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                s_baidu_net_id = msg_ptr->nsapi;
                SCI_TRACE_LOW("[baidu]:HandlePdpMsg  MMIPDP_RESULT_SUCC s_baidu_net_id %d " , s_baidu_net_id);
		  if( BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK == s_auto_upload_status)
		  {
		 	s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
		  }
                MMISRV_BAIDUDRV_InitHttpRequest(g_current_module_id);
            }
            else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
            {
                MMIAPIPDP_Deactive(g_current_module_id);
                if(0 != s_baidu_pdp_retry_timer_id)
                {
                        MMK_StopTimer(s_baidu_pdp_retry_timer_id);
                        s_baidu_pdp_retry_timer_id = 0;
                }
                s_baidu_pdp_retry_timer_id = MMK_CreateTimerCallback(
                        2000, 
                        HandlePdpRetryTimer, 
                        PNULL, 
                        FALSE
                        );
            }
            else 
            {
                SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- FAIL to ACTIVATE PDP , RESET BAIDU TASK !!");
                s_baidu_net_id = 0;
                MMIAPIPDP_Deactive(g_current_module_id);
                MMISRV_BAIDUDRV_Check_Network_status(FALSE);            
            }
        }        
        break;
        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- MMIPDP_DEACTIVE_CNF");        
            s_baidu_net_id = 0;
        break;
        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        {
            SCI_TRACE_LOW("[baidu]:HandlePdpMsg--- MMIPDP_DEACTIVE_IND");
            s_baidu_net_id = 0;
            MMIAPIPDP_Deactive(g_current_module_id);
            MMISRV_BAIDUDRV_Check_Network_status(FALSE);
        }
        break;

        default:
        break;
    }
    
}

/*****************************************************************************/
//  Description :HandlePdpRetryTimer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMISRV_BAIDUDRV_MSG_T sig_data={0};
	
    SCI_TRACE_LOW("[baidu]:HandlePdpRetryTimer begin");

	if( 0 < s_baidu_pdp_retry_count)	
		s_baidu_pdp_retry_count --;
	else
	{
	       SCI_TRACE_LOW("[baidu]:HandlePdpRetryTimer - PDP activate FAIL !!");
		MMIAPIPDP_Deactive(g_current_module_id);
		MMISRV_BAIDUDRV_Check_Network_status(FALSE);
		return ; 
	}	
	
	if(timer_id == s_baidu_pdp_retry_timer_id && 0 != s_baidu_pdp_retry_timer_id)
	{
	    MMK_StopTimer(s_baidu_pdp_retry_timer_id);
	    s_baidu_pdp_retry_timer_id = 0;
	}
	
	if(!BaiduDrvPDPTry(s_baidu_cur_sys_num ))
	{
		s_baidu_pdp_retry_timer_id = MMK_CreateTimerCallback(
	                                            1500, 
	                            		        HandlePdpRetryTimer, 
	                            		        PNULL, 
	                                		    FALSE
	                                		    );
	}
}


/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    SCI_TRACE_LOW("[baidu]:HandlePdpActiveTimer---Time out error!---");
    MMIAPIPDP_Deactive(g_current_module_id);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void)
{
    if(0 != s_baidu_pdp_timer_id)
    {
        MMK_StopTimer(s_baidu_pdp_timer_id);
        s_baidu_pdp_timer_id = 0;
    }
    
    s_baidu_pdp_timer_id = MMK_CreateTimerCallback(20000, 
                            HandlePdpActiveTimer, 
                            PNULL, 
                            FALSE);
}

/*****************************************************************************/
// 	Description : Handle_UserInfo_timers
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Handle_UserInfo_timer(uint8  timer_id, uint32 param)
{
	int i=0 , j=0;
	MMISRV_BAIDUDRV_MSG_T sig_data={0};	

	SCI_TRACE_LOW("[baidu]:Enter Handle_UserInfo_timer");


	if (0 != s_baidu_drv_check_user_info_timer)
	{
		MMK_StopTimer(s_baidu_drv_check_user_info_timer);
		s_baidu_drv_check_user_info_timer = 0;

		if((0 < strlen(s_baidu_user_info.baidu_name)) && 
		    (0 < s_baidu_user_info.size_total)  
		    /* && (MMIAPIFMM_IsFileExist(s_baidu_drv_avatar_img_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_avatar_img_file_path)))*/
		    )
		{
                     SCI_TRACE_LOW("[baidu]:Handle_UserInfo_timer user info completed !!");

			sig_data.init_type= BAIDU_DRV_TYPE_LOGIN;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_SIGNIN_DONE , &sig_data);		
		}
		else 
		{
                     SCI_TRACE_LOW("[baidu]:Handle_UserInfo_timer user info not ready , restart check user info timer !!");
		      	s_baidu_drv_check_user_info_timer =  MMK_CreateTimerCallback(1000 * 5, Handle_UserInfo_timer, PNULL, FALSE);					 
		}
	}	
}

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleDownloadThumbnail_timer(uint8  timer_id, uint32 param)
{
	int i=0 , j=0;
	MMISRV_BAIDUDRV_MSG_T sig_data={0};	

	if (0 != s_download_thumbnail_timer)
	{
		MMK_StopTimer(s_download_thumbnail_timer);
		s_download_thumbnail_timer = 0;

		if(0 != s_current_download_thumbnail_count)
		{
			//some download thumbnail is expired , so reset all settings
                     SCI_TRACE_LOW("[baidu]:Enter HandleDownloadThumbnail_timer to reset download request ");
			//Reset_all_data();
			Clean_meta_data_info();

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				for(j=0;j<MAX_REQUEST_NUM;j++)
				{
					if((BAIDU_DRV_OPER_GET_THUMBNAIL_INFO  == http_request_operation_array[i].operation_ary[j].operation) || 
					    (BAIDU_DRV_OPER_GET_THUMBNAIL_URL  == http_request_operation_array[i].operation_ary[j].operation)  ||
					    (BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL  == http_request_operation_array[i].operation_ary[j].operation)
				   	)
					{
						HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
						SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
						SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);	
					}
				}			
			}

			s_current_download_thumbnail_count = 0;
			s_is_process_download_thumbnail = FALSE;
			s_is_process_get_delta_link = FALSE;

			sig_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
			
			if((BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) && 
			    (MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
		            MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo()))
			{
				MMISRV_BAIDUDRV_exec_auto_upload_photo();
			}
		}
	}	
}

/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMISRV_BAIDUDRV_MSG_T sig_data={0};    
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	
    SCI_TRACE_LOW("[baidu]:Enter MMISRV_BAIDUDRV_PdpActive");

#ifdef WIN32
	return;
#endif

    if(0 != s_baidu_net_id)
    {
        SCI_TRACE_LOW("[baidu]:Enter MMISRV_BAIDUDRV_PdpActive s_baidu_net_id is exist %d" , s_baidu_net_id);
        MMISRV_BAIDUDRV_InitHttpRequest(g_current_module_id);
	 if( BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK == s_auto_upload_status)
	 {
	 	s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
	 }
        return TRUE;
    }
    else if (s_is_process_pdp_activate) // already process pdp activate before !!!!
    {
        SCI_TRACE_LOW("[baidu]:Enter MMISRV_BAIDUDRV_PdpActive s_is_process_pdp_activate is TRUE , wait for callback function !");
        return TRUE;
    }  
   
#ifdef MMI_WIFI_SUPPORT    
    //优先wifi
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = g_current_module_id;
        active_info.handle_msg_callback = HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            //StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    else
#endif //WIFI_SUPPORT
    {
        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        BOOLEAN bIsFound = FALSE;

            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
            if(sim_ok_num ==0)
            {
                bIsFound = FALSE;
            }
            else if (sim_ok_num == 1)
            {
                bIsFound = TRUE;              
            }

        //bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sim_ok);

        if(FALSE == bIsFound)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_NETWORK;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);			
            return FALSE;
        }
		
        s_baidu_pdp_retry_count = 2;
        s_baidu_cur_sys_num = sim_ok;
		
        return_val = BaiduDrvPDPTry(s_baidu_cur_sys_num);//第一张有效卡
        if(FALSE == return_val) 
        {
		if(0 != s_baidu_pdp_retry_timer_id)
		{
		    MMK_StopTimer(s_baidu_pdp_retry_timer_id);
		    s_baidu_pdp_retry_timer_id = 0;
		}
		
		s_baidu_pdp_retry_timer_id = MMK_CreateTimerCallback(
			3000, 
			HandlePdpRetryTimer, 
			PNULL, 
			FALSE
		);
        }
    }
    
    return return_val;
}

#ifdef  SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Upload_single_photo(uint16 * file_path , uint16 file_path_len)
{
	MMIAPIBAIDU_USER_INFO_T *user_info = PNULL;
	FILEARRAY single_upload_ary = PNULL;
       FILEARRAY_DATA_T array_item = {0};
	SFS_DATE_T  modify_date = {0};
	SFS_TIME_T  modify_time = {0};
	
	if(PNULL == file_path || 0 == file_path_len)
		return FALSE;

	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Upload_single_photo");

	single_upload_ary = MMIAPIFILEARRAY_Create();
	
	if(PNULL== single_upload_ary)
		return FALSE;
	   
	//check auto upload status , if YES , pause first 
	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		SCI_TRACE_LOW("[baidu]: Pause auto upload !!!");
		Reset_all_data();
	}
	SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Upload_single_photo add file to ary");
	MMIAPICOM_Wstrncpy(array_item.filename, file_path, file_path_len);
	array_item.name_len = file_path_len;
	MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, 
		&array_item.size, &modify_date, &modify_time);
       
	array_item.modify_date.mday = modify_date.mday;
	array_item.modify_date.mon = modify_date.mon;
	array_item.modify_date.year = modify_date.year;

	array_item.modify_time.sec = modify_time.sec;
	array_item.modify_time.min = modify_time.min;
	array_item.modify_time.hour = modify_time.hour;

	array_item.type = FILEARRAY_TYPE_FILE;
	MMIAPIFILEARRAY_Add(single_upload_ary, &array_item);

	MMIAPIFILEARRAY_Combine(single_upload_ary , s_baidu_drv_manual_upload_file_array);	

	MMIAPIFILEARRAY_Destroy(&s_baidu_drv_manual_upload_file_array);
	s_baidu_drv_manual_upload_file_array = single_upload_ary;
	   
	MMIAPIFILEARRAY_SaveArrayToFile(s_baidu_drv_manual_upload_file_array, s_baidu_drv_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));

	SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Upload_single_photo current manual upload array size = %d " , MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array));

	s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
	MMISRV_BAIDUDRV_exec_auto_upload_photo();

	return TRUE;
}
#endif

LOCAL void MMISRV_BAIDUDRV_calc_md5_by_buf(uint8 *buf, uint32 buflen, uint8 * MD5ptr)
{
    MD5_CONTEXT  mdContext;
    unsigned char digest[16] = {0};
    unsigned char *tempmd5 = MD5ptr;
    int i = 0;
#ifndef WIN32
    /*init para*/
    Baidu_MD5Init(&mdContext);
    Baidu_MD5Update(&mdContext, (uint8*)buf, (unsigned)buflen);
    Baidu_MD5Final(digest ,&mdContext);
#endif

    for(i = 0; i < 16; i++)
    {
        sprintf(tempmd5, "%02x", digest[i]);
        SCI_TRACE_LOW("MD5:%c%c: %02x", *tempmd5, *(tempmd5+1), digest[i]);
        tempmd5 += 2;
    }

}

MMIFILE_ERROR_E  MMISRV_BAIDUDRV_SlicingFile (wchar *base_file_name, uint32 slicing_len, uint16 slicing_num)
{
    MMIFILE_HANDLE  base_file_handle = PNULL;
    MMIFILE_HANDLE  slicing_file_handle = PNULL;
    MMIFILE_ERROR_E file_ret = SFS_ERROR_SYSTEM;
    uint32          bytes_read = 0;
    uint32          write_size = 0;
    wchar          Wfilename[MMIFILE_FILE_NAME_MAX_LEN+1]={0};
    uint8            filename[MMIFILE_FILE_NAME_MAX_LEN+1]={0};
    uint16          Wfilename_len = 0;
    uint16          Wpath_len = 0;
    wchar          Wtemp_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    uint8            temp_filetype[16] = {0};
    uint32          base_file_len = 0;
    uint8           *readbuf = NULL;
    uint8            tempmd5[MMIFILE_FILE_MD5_LEN+1] = {0};
    uint8            *md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
    int               i = 0;

    if (PNULL == base_file_name)
    {
        SCI_TRACE_LOW("[MMIFILE] MMIAPIFMM_SlicingFile file name invalid !");
        return file_ret;
    }
    if(slicing_num < 2)
    {
        SCI_TRACE_LOW("[MMIFILE] MMIAPIFMM_SlicingFile file name  slicing_num < 2 ");
        return file_ret ;
    }
    
    //Wpath_len = MMIAPICOM_Wstrlen(base_file_name);
    //MMIAPIFMM_SplitLastName(base_file_name , &Wpath_len , Wfilename, &Wfilename_len);
    //MMIAPICOM_WstrToStr(Wfilename, filename);
    //MMIAPICOM_Wstrncpy(Wtemp_path, base_file_name, MMIAPICOM_Wstrlen(base_file_name));
    //strcpy(temp_filetype, &filename[strlen(filename)-4]);
    //SCI_TRACE_LOW("basefile:%s, filename:%s",base_file_name ,Wfilename);
    
    //open source file 
    base_file_handle = SFS_CreateFile (base_file_name, SFS_MODE_READ| SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

    if (SFS_INVALID_HANDLE != base_file_handle)
    {
        file_ret = SFS_GetFileSize(base_file_handle, &base_file_len);
        /*malloc mem for read.*/
        readbuf = SCI_ALLOC_APP (slicing_len+1);
        if(NULL == readbuf)
        {
            SCI_TRACE_LOW("NOT FREE MEM. ");
            SCI_ASSERT(0);
        }
        for(i = 0; i < slicing_num; i++)
        {
            /*filename */
            //sprintf(&filename[strlen(filename)-4], "%s%d%s","_", i, temp_filetype);
            //MMIAPICOM_StrToWstr(filename, Wfilename);
            //MMIAPICOM_Wstrncat(Wtemp_path, Wfilename, MMIAPICOM_Wstrlen(Wfilename));
            //SCI_TRACE_LOW("NEW:Wtemp_path:%s, WFILENAME:%s", Wtemp_path, Wfilename);
            if(slicing_len*i > s_baidu_drv_upload_photo_ptr->total_file_size)
            {
                SCI_ASSERT(0);
            }
            MMIAPIFMM_SetFilePointer(base_file_handle , slicing_len*i,MMIFILE_SEEK_BEGIN);            

            SFS_ReadFile(base_file_handle, readbuf, slicing_len, &bytes_read, PNULL);
            if(slicing_len != bytes_read)
            {
                SCI_TRACE_LOW("have end read.  last size:%d", bytes_read);
            }

            MMISRV_BAIDUDRV_calc_md5_by_buf(readbuf, bytes_read, tempmd5);
            strcpy((uint8 *)(md5_list+i*MMIFILE_FILE_MD5_OFFSET), tempmd5);
            SCI_TRACE_LOW("list:%s, md5:%s", md5_list+i*MMIFILE_FILE_MD5_OFFSET, tempmd5);
                
           #if 0   
            //open file to read
            slicing_file_handle = SFS_CreateFile (Wtemp_path, SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_CREATE_NEW, NULL, NULL);/*lint !e655*/
            if (SFS_INVALID_HANDLE != slicing_file_handle)
            {

                 file_ret = SFS_WriteFile (slicing_file_handle, readbuf, bytes_read, &write_size, PNULL);

                if (SFS_ERROR_NONE != file_ret)
                {
                    SCI_TRACE_LOW("create file fail.!!!!!!!");
                    SCI_ASSERT(0);
                }
                MMIAPIFMM_CloseFile(slicing_file_handle);
                
                MMISRV_BAIDUDRV_calc_md5_by_buf(readbuf, bytes_read, tempmd5);
                strcpy((uint8 *)(md5_list+i*MMIFILE_FILE_MD5_OFFSET), tempmd5);
                SCI_TRACE_LOW("list:%s, md5:%s", md5_list+i*MMIFILE_FILE_MD5_OFFSET, tempmd5);
            }
            #endif
        }
        
         SCI_FREE(readbuf);
    }
    MMIAPIFMM_CloseFile (base_file_handle);
    return file_ret;
}

PUBLIC BOOLEAN  MMISRV_BAIDUDRV_Judge_Slicing_File( DWORD filelen)
{
    uint16   slicing_num = 0;

#if 1 //def X_SLICING_SUPPORT
    uint16  x_slicing_num = 0;
    DWORD  left_len;
	
    slicing_num = filelen/S_UPLOAD_SLICING_MAX;
    left_len = filelen%S_UPLOAD_SLICING_MAX;
    if(0 < left_len)
    {
        x_slicing_num = left_len/S_UPLOAD_X_SLICING_MAX;
	 if(0 < left_len%S_UPLOAD_X_SLICING_MAX)
	 {
	     x_slicing_num++;
	 }
        slicing_num++;
    }

    /*md5 malloc*/
    if(0 == slicing_num)
    {
        SCI_ASSERT(0);
    }
    if(s_baidu_drv_upload_photo_ptr->file_md5_list)
    {
        SCI_FREE(s_baidu_drv_upload_photo_ptr->file_md5_list);
    }
    s_baidu_drv_upload_photo_ptr->file_md5_list = SCI_ALLOC_APPZ(slicing_num*(MMIFILE_FILE_MD5_OFFSET) + 1);
    SCI_ASSERT(s_baidu_drv_upload_photo_ptr->file_md5_list != NULL);
    s_baidu_drv_upload_photo_ptr->slicing_num = slicing_num;
    s_baidu_drv_upload_photo_ptr->x_last_slicing_num = x_slicing_num;
    s_baidu_drv_upload_photo_ptr->x_part_size = left_len;

    if(1 == slicing_num)
    {
        return FALSE;
    }
    else
    {
        /*slicing file*/
        //MMISRV_BAIDUDRV_SlicingFile(s_baidu_drv_upload_photo_ptr->file_path, S_UPLOAD_SLICING_MAX, slicing_num);
        return TRUE;
    }
#else
    slicing_num = filelen/S_UPLOAD_SLICING_MAX;
    if(0 < filelen%S_UPLOAD_SLICING_MAX)
    {
        slicing_num++;
    }

    /*md5 malloc*/
    if(0 == slicing_num)
    {
        SCI_ASSERT(0);
    }
    if(s_baidu_drv_upload_photo_ptr->file_md5_list)
    {
        SCI_FREE(s_baidu_drv_upload_photo_ptr->file_md5_list);
    }
    s_baidu_drv_upload_photo_ptr->file_md5_list = SCI_ALLOC_APPZ(slicing_num*(MMIFILE_FILE_MD5_OFFSET) + 1);
    SCI_ASSERT(s_baidu_drv_upload_photo_ptr->file_md5_list != NULL);
    s_baidu_drv_upload_photo_ptr->slicing_num = slicing_num;

    if(1 == slicing_num)
    {
        return FALSE;
    }
    else
    {
        /*slicing file*/
        MMISRV_BAIDUDRV_SlicingFile(s_baidu_drv_upload_photo_ptr->file_path, S_UPLOAD_SLICING_MAX, slicing_num);
        return TRUE;
    }
#endif
}
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Check_auto_upload_photo(void)
{
	uint8 i = 0 , j=0;

	if(((BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK == s_auto_upload_status)
#ifdef MMI_WIFI_SUPPORT
		&&(MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
#endif
		)||!(MMIAPIBAIDU_IsAccountExist() && 
	      MMISRV_BAIDUDRV_Get_Connect_State()))
		return;

	if(TRUE == s_is_in_gallery)
	{
		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			for(j=0;j<MAX_REQUEST_NUM;j++)
			{
				if(BAIDU_DRV_OPER_NOT_INIT != http_request_operation_array[i].operation_ary[j].operation&&
				    BAIDU_DRV_OPER_NO_OPERATION != http_request_operation_array[i].operation_ary[j].operation)
				{
				       SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Check_auto_upload_photo some operation is processing in gallery , don't trigger auto upload !!");
					return;
				}
			}
		}		
	}

	//Can start auto upload !!!!!
	if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo())
        {
            if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING !=s_auto_upload_status ||
            (BAIDU_DRV_OPER_NO_OPERATION == MMISRV_BAIDUDRV_Check_Current_auto_upload_request())
            )
            {
                SCI_TRACE_LOW("[baidu] ========= MMISRV_BAIDUDRV_Check_auto_upload_photo NEED auto upload photos  =================");
                s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
                MMISRV_BAIDUDRV_CreateTask(MMI_MODULE_PIC_VIEWER , BAIDU_DRV_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO);
            }
	}else if(MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
	{
            if(BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING !=s_auto_upload_status ||
            (BAIDU_DRV_OPER_NO_OPERATION == MMISRV_BAIDUDRV_Check_Current_auto_upload_request())
            )
            {
                SCI_TRACE_LOW("[baidu] ========= MMISRV_BAIDUDRV_Check_auto_upload_photo NEED auto upload photos  =================");
                s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING;
                MMISRV_BAIDUDRV_CreateTask(MMI_MODULE_PIC_VIEWER , BAIDU_DRV_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO);
            }
	}
}
#if 1 //def X_SLICING_SUPPORT
LOCAL void MMISRV_BADIDUDRV_calc_file_md5(MMISRV_BAIDUDRV_UPLOAD_INFO_T *upload_info_ptr)
{
	MMIFILE_HANDLE  file_handle = PNULL;
	uint16         slicing_num = 0;
	MD5_CONTEXT  mdContext;
	uint32          data_size = 0;
	uint8            md5[MMIFILE_FILE_MD5_LEN+1] = {0};
	uint8            *tempmd5 = md5;
	uint8            *md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
	int               i = 0, j = 0;
	unsigned char digest[16] = {0};

	file_handle = upload_info_ptr->file_handle;
	slicing_num = upload_info_ptr->slicing_num;

    if (SFS_INVALID_HANDLE != file_handle)
    {
        for(i = 0; i < slicing_num; i++)
	 {
		if((i+1) == slicing_num)
		{
			data_size = (upload_info_ptr->total_file_size%S_UPLOAD_SLICING_MAX);
		}
		else
		{
			data_size = S_UPLOAD_SLICING_MAX;
		}
		
              tempmd5 = md5;	  	
		Baidu_MD5Init(&mdContext);
		FileMD5Update(&mdContext, file_handle, i*S_UPLOAD_SLICING_MAX, data_size);
		Baidu_MD5Final(digest ,&mdContext);
		for(j = 0; j < 16; j++)
		{
			sprintf(tempmd5, "%02x", digest[j]);
			SCI_TRACE_LOW("MD5:%c %c: %02x", *tempmd5, *(tempmd5+1), digest[j]);
			tempmd5 += 2;
		}
            	
            	strcpy((uint8 *)(md5_list+i*MMIFILE_FILE_MD5_OFFSET), md5);
            	SCI_TRACE_LOW("list:%s, md5:%s", md5_list+i*MMIFILE_FILE_MD5_OFFSET, md5);
                
           }
        
    }
    else
    {
    	SCI_TRACE_LOW("CALA md5 file handle is NULL.");
    }

}
#else

/**************************************************************************************/
// Description : MMISRV_BADIDUDRV_calc_file_md5, get the md5 of upload file
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void MMISRV_BADIDUDRV_calc_file_md5(MMISRV_BAIDUDRV_UPLOAD_INFO_T *upload_info_ptr)
{
    uint32  read_bytes = 0;
    uint32  remain_size = 0;
    uint32  readed_size = 0;
    MMIFILE_HANDLE  filehandle = upload_info_ptr->file_handle;
    uint8 *tmpbuf = NULL;
    uint32 buff_size = S_UPLOAD_SLICING_MAX;  
    MD5_CONTEXT  mdContext;
    unsigned char digest[16] = {0};
    unsigned char *tempmd5 = upload_info_ptr->file_md5;
    int i = 0;

    if(buff_size < upload_info_ptr->total_file_size)
    {
        SCI_TRACE_LOW("[baidu]: the file too large, filesize:%d", upload_info_ptr->total_file_size);
        SCI_ASSERT(0);
    }
    /*init para*/
    remain_size = upload_info_ptr->total_file_size;

#ifndef WIN32   
    Baidu_MD5Init(&mdContext);
#endif

    while(remain_size > 0)
    {
        read_bytes = MIN(buff_size, remain_size);
        tmpbuf = SCI_ALLOC_APPZ(read_bytes+10);
        MMIAPIFMM_SetFilePointer(filehandle ,readed_size ,MMIFILE_SEEK_BEGIN);
        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(filehandle, tmpbuf , read_bytes,&read_bytes, PNULL))
        {
        	SCI_TRACE_LOW("[baidu]: MMISRV_BADIDUDRV_calc_file_md5 read file OK, readlen:%d  !!!", read_bytes);
        }

#ifndef WIN32   
       Baidu_MD5Update(&mdContext, (uint8*)tmpbuf, (unsigned)read_bytes);
#endif

        if(remain_size < read_bytes)
        {
            SCI_TRACE_LOW("[baidu]: MMISRV_BADIDUDRV_calc_file_md5 read file len error  !!!");
            break;
        }
        else
        {
            remain_size -= read_bytes;
        }
    }
#ifndef WIN32   
    Baidu_MD5Final(digest ,&mdContext);
#endif

	for(i = 0; i < 16; i++)
    {
        sprintf(tempmd5, "%02x", digest[i]);
        SCI_TRACE_LOW("MD5:%c%c: %02x", *tempmd5, *(tempmd5+1), digest[i]);
        tempmd5 += 2;
    }
    SCI_FREE(tmpbuf);
}
#endif
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_exec_auto_upload_photo(void)
{
    	SCI_TIME_T time = {0};
	SCI_DATE_T date = {0};
	uint32 sec = 0;
	wchar *file_path = PNULL;
       wchar suffix_wstr[4] = {0};
       uint8  suffix_str[4] = {0};
       uint16 suffix_len = 3;
	uint16 snapshot_upload_file_size = 0 , manual_upload_file_size =0 ,i=0, j = 0 , current_upload_size=0; //upload_file_success_size =  0 ,
	FILEARRAY_DATA_T upload_file_data = {0};
	FILEARRAY_DATA_T manual_upload_file_data = {0};
	FILEARRAY_DATA_T upload_success_file_data = {0};
	BOOLEAN b_already_uploaded = FALSE;
	FILEARRAY modified_auto_upload_ary = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};	
	MMIAPIBAIDU_USER_INFO_T * user_info = MMISRV_BAIDUDRV_get_user_info();
       uint8*  md5_list = NULL;
       char temp_size_str[100] = {0};
       uint16  templen = 0;
       BOOLEAN is_need_update_file = FALSE;
	uint16 file_index = 0;
    
	if( 0 == strlen(s_baidu_drv_access_token_str))
	{
		
            SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo baidu baidu_name doesn't exist  !!!");
            //MMISRV_BAIDUDRV_Get_user_info_req();
            baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;	
            MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
            return ;
	}

	snapshot_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array);
	//upload_file_success_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_upload_success_file_array);
	#ifdef SINGLE_UPLOAD_SUPPORT 
	manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);
       #endif
	   
	if(0 == manual_upload_file_size && 0 == snapshot_upload_file_size)
	{
		SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo baidu no photo needs to be upload !!");
		return;
	}

	#ifdef SINGLE_UPLOAD_SUPPORT
	if(manual_upload_file_size > 0) 
	{
		s_current_upload_file_array = s_baidu_drv_manual_upload_file_array;
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING ; 
		s_current_upload_file_path = s_baidu_drv_manual_upload_list_file_path ; 
		current_upload_size = manual_upload_file_size;
	}
	else
	#endif
	{
		s_current_upload_file_array = s_baidu_drv_snapshot_upload_file_array;
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING ; 
		s_current_upload_file_path = s_baidu_drv_snapshot_upload_list_file_path ; 
		current_upload_size = snapshot_upload_file_size ;
	}
#ifdef SINGLE_UPLOAD_SUPPORT
	if(manual_upload_file_size > 0 && snapshot_upload_file_size > 0)
	{
		modified_auto_upload_ary = MMIAPIFILEARRAY_Create();
		
		for(i=0; i < snapshot_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_baidu_drv_snapshot_upload_file_array , i ,&upload_file_data);

			for(j=0; j < manual_upload_file_size ; j++)
			{
				MMIAPIFILEARRAY_Read(s_baidu_drv_manual_upload_file_array , j ,&manual_upload_file_data);
				
				if(!((manual_upload_file_data.name_len == upload_file_data.name_len) && 
				      ( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
				   )
				{
					MMIAPIFILEARRAY_Add(modified_auto_upload_ary , &upload_file_data);
				}
			}
		}
		
		MMIAPIFILEARRAY_SaveArrayToFileEx(modified_auto_upload_ary , s_baidu_drv_snapshot_upload_list_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_snapshot_upload_list_file_path));	
		MMIAPIFILEARRAY_Destroy(&modified_auto_upload_ary);		
	}
#endif

              /*del the invalid file.*/
              snapshot_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_current_upload_file_array);
		file_index = 0;
	       for(i = 0; i < snapshot_upload_file_size; i++)
	       {
	       	if(!MMIAPIFILEARRAY_Read(s_current_upload_file_array , file_index ,&upload_file_data))
			{
				SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo MMIAPIFILEARRAY_Read s_baidu_drv_snapshot_upload_file_array fail !!!");
				s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE; 
				return;
			}
			/*if not exist the file. delete*/
	              if(!MMIAPIFMM_IsFileExist(upload_file_data.filename, MMIAPICOM_Wstrlen(upload_file_data.filename)))
	              {
	                  SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo %s is not exist!!!", upload_file_data.filename );
                          is_need_update_file =TRUE;
	                   MMIAPIFILEARRAY_Delete(s_current_upload_file_array , file_index);
			     continue;
	              }
		       file_index++;
	       }
		if(is_need_update_file)
		{
			MMIAPIFILEARRAY_SaveArrayToFileEx(s_current_upload_file_array , s_current_upload_file_path ,MMIAPICOM_Wstrlen(s_current_upload_file_path));
		}
              /*not have valid file return*/
		if(0 ==  MMIAPIFILEARRAY_GetArraySize(s_current_upload_file_array))
		{
			SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo not have valid file!!!");
			s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE; 
			return;
		}
        
		if(!MMIAPIFILEARRAY_Read(s_current_upload_file_array , 0 ,&upload_file_data))
		{
			SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo MMIAPIFILEARRAY_Read s_baidu_drv_snapshot_upload_file_array fail !!!");
			s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE; 
			return;
		}

	SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_exec_auto_upload_photo start upload photo !!");	
		   
		if(PNULL == s_baidu_drv_upload_photo_ptr)
		{
			s_baidu_drv_upload_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_UPLOAD_INFO_T));
			SCI_ASSERT(PNULL != s_baidu_drv_upload_photo_ptr);
			//tmp_ptr = s_baidu_drv_upload_photo_ptr;
		}
		else
		{
			SCI_MEMSET((void*)s_baidu_drv_upload_photo_ptr , 0x00 ,sizeof(MMISRV_BAIDUDRV_UPLOAD_INFO_T) );
		}

	       // save file path 
	       SCI_MEMCPY(s_baidu_drv_upload_photo_ptr->file_path , upload_file_data.filename , sizeof(uint16)*upload_file_data.name_len);
		s_baidu_drv_upload_photo_ptr->file_path_len = upload_file_data.name_len;
           
		//create name 
	       TM_GetSysTime(&time);
		TM_GetSysDate(&date);

		time.hour = (0 == time.hour) ? 12 : time.hour;
		time.min = (0 == time.min) ? 60 : time.min;
		time.sec = (0 == time.sec) ? 60 : time.sec;

              MMIAPIFMM_SplitFileName(s_baidu_drv_upload_photo_ptr->file_path, s_baidu_drv_upload_photo_ptr->file_path_len, PNULL, PNULL, suffix_wstr, &suffix_len);
              MMIAPICOM_WstrToStr(suffix_wstr, suffix_str);
              
		sprintf((char*)s_baidu_drv_upload_photo_ptr->file_name,"%04d%02d%02d%02d%02d-%02d.%s",date.year, date.mon,date.mday,time.hour,time.min,time.sec, suffix_str);
		s_baidu_drv_upload_photo_ptr->file_name_len = strlen(s_baidu_drv_upload_photo_ptr->file_name);
        
              SCI_TRACE_LOW("######upload test: tiime:%d,%d,%d, %s",time.sec, time.min, time.hour, s_baidu_drv_upload_photo_ptr->file_name);
              /*cal md5*/
              if (s_baidu_drv_upload_photo_ptr->file_handle != 0)
        	{
                    MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
                    s_baidu_drv_upload_photo_ptr->file_handle = 0;
        	}
              //SCI_TRACE_LOW("######upload test: file_patch:%s",(char*)&s_baidu_drv_upload_photo_ptr->file_path[0] );
              //SCI_TRACE_LOW("######upload test: file_name:%s",(char*)&s_baidu_drv_upload_photo_ptr->file_name[0] );
              s_baidu_drv_upload_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_photo_ptr->file_path , SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/
              //SCI_TRACE_LOW("######upload test: file_handle:%d",s_baidu_drv_upload_photo_ptr->file_handle);
              s_baidu_drv_upload_photo_ptr->total_file_size = MMIAPIFMM_GetFileSize(s_baidu_drv_upload_photo_ptr->file_handle);
              if(0 == s_baidu_drv_upload_photo_ptr->total_file_size )
              {
                  SCI_ASSERT(0);
              }
              
              MMISRV_BAIDUDRV_Judge_Slicing_File( s_baidu_drv_upload_photo_ptr->total_file_size);
              //SCI_TRACE_LOW("######upload test: total_file_size:%d",s_baidu_drv_upload_photo_ptr->total_file_size);
              
              sprintf(temp_size_str, "%d", s_baidu_drv_upload_photo_ptr->total_file_size);
#if 1 //def X_SLICING_SUPPORT
              SCI_TRACE_LOW("######upload test: total_file_size:%d, slicing:%d, x_last_slicing:%d\n",s_baidu_drv_upload_photo_ptr->total_file_size, s_baidu_drv_upload_photo_ptr->slicing_num,s_baidu_drv_upload_photo_ptr->x_last_slicing_num);

              MMISRV_BADIDUDRV_calc_file_md5(s_baidu_drv_upload_photo_ptr);
		if((1 == s_baidu_drv_upload_photo_ptr->slicing_num)&&(1 == s_baidu_drv_upload_photo_ptr->x_last_slicing_num))
              {
            		sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%d%s",
                                    BAIDUDRIVE_UPLOAD_START_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_START_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&size=",s_baidu_drv_upload_photo_ptr->total_file_size,
                                    BAIDUDRIVE_UPLOAD_START_URL_PART3);
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
                    for(i=0; i<s_baidu_drv_upload_photo_ptr->slicing_num; i++)
                    {
                        
                        if(i < (s_baidu_drv_upload_photo_ptr->slicing_num -1))
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET,
                                       "\",\"");
                        }
                        else
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET);
                        }
                        templen  = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    }
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    if(BAIDU_DRV_MAX_STRING_LEN < templen)
                    {
                        SCI_ASSERT(0);
                    }
                    sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       "\"]");
               }
	       else
              {
                    /*set md5 list*/
                    sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%d%s%s",
                                    BAIDUDRIVE_UPLOAD_START_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_START_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&size=",s_baidu_drv_upload_photo_ptr->total_file_size,
                                    "&x-enable-resume=2",
                                    BAIDUDRIVE_UPLOAD_START_URL_PART3);
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
                    for(i=0; i<s_baidu_drv_upload_photo_ptr->slicing_num; i++)
                    {
                        
                        if(i < (s_baidu_drv_upload_photo_ptr->slicing_num -1))
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET,
                                       "\",\"");
                        }
                        else
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET);
                        }
                        templen  = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    }
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    if(BAIDU_DRV_MAX_STRING_LEN < templen)
                    {
                        SCI_ASSERT(0);
                    }
                    sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       "\"]");
                }
#else
              if(1 == s_baidu_drv_upload_photo_ptr->slicing_num)
              {
                    MMISRV_BADIDUDRV_calc_file_md5(s_baidu_drv_upload_photo_ptr);
            		sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%d%s%s%s",
                            BAIDUDRIVE_UPLOAD_START_URL_PART1 ,
                            s_baidu_drv_access_token_str ,  
                            BAIDUDRIVE_UPLOAD_START_URL_PART2, 
                            s_baidu_drv_upload_photo_ptr->file_name,
                            "&size=",s_baidu_drv_upload_photo_ptr->total_file_size,
                            BAIDUDRIVE_UPLOAD_START_URL_PART3,
                            s_baidu_drv_upload_photo_ptr->file_md5,
                            "\"]");
               }
              else
              {
                    /*set md5 list*/
                    sprintf((char*)s_baidu_drv_upload_photo_ptr->upload_url,"%s%s%s%s%s%d%s",
                                    BAIDUDRIVE_UPLOAD_START_URL_PART1 ,
                                    s_baidu_drv_access_token_str ,  
                                    BAIDUDRIVE_UPLOAD_START_URL_PART2, 
                                    s_baidu_drv_upload_photo_ptr->file_name,
                                    "&size=",s_baidu_drv_upload_photo_ptr->total_file_size,
                                    BAIDUDRIVE_UPLOAD_START_URL_PART3);
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    md5_list = s_baidu_drv_upload_photo_ptr->file_md5_list;
                    for(i=0; i<s_baidu_drv_upload_photo_ptr->slicing_num; i++)
                    {
                        
                        if(i < (s_baidu_drv_upload_photo_ptr->slicing_num -1))
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET,
                                       "\",\"");
                        }
                        else
                        {
                            sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       md5_list+i*MMIFILE_FILE_MD5_OFFSET);
                        }
                        templen  = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    }
                    templen = strlen(s_baidu_drv_upload_photo_ptr->upload_url);
                    if(BAIDU_DRV_MAX_STRING_LEN < templen)
                    {
                        SCI_ASSERT(0);
                    }
                    sprintf((char*)(&s_baidu_drv_upload_photo_ptr->upload_url[templen]), "%s",
                                       "\"]");
                }
#endif
	       //SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Upload_single_photo !! tmp_ptr->file_name %s" , s_baidu_drv_upload_photo_ptr->upload_url);	             
       
              MMISRV_BAIDUDRV_start_upload_file();
	}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Enter_Gallery(void)
{
       SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Enter_Gallery !!");	
	s_is_in_gallery = TRUE;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_In_Gallery(void)
{
	return s_is_in_gallery ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Leave_Gallery(void)
{
        SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Leave_Gallery !!");
	s_is_in_gallery = FALSE;

	if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE == s_auto_upload_status && 
	    !(s_is_process_download_thumbnail ||s_is_process_get_delta_link))
	{
	       SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Leave_Gallery MMISRV_DeleteBaiduDrvTaskThread  !!");
		MMISRV_DeleteBaiduDrvTaskThread();
	}
}

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Upload_multiple_photos(void)
{
       SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Upload_multiple_photos !!");	


	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status)
	{
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
		MMISRV_BAIDUDRV_exec_auto_upload_photo();
	}	

	return TRUE;
}
#ifdef SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Set_Upload_multiple_photos(FILEARRAY file_array)
{
       SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Set_Upload_multiple_photos !!");	
	MMIAPIFILEARRAY_Combine(s_baidu_drv_manual_upload_file_array,file_array);			
	MMIAPIFILEARRAY_SaveArrayToFile(s_baidu_drv_manual_upload_file_array, s_baidu_drv_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));
	return TRUE;
}
#endif
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Get_Totalsize_By_Filename(uint8 *buff,  uint32 file_size , uint8 *filename, DWORD  *totalsize)
{
	MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    uint32  read_size=0;
    uint8 totalsize_str[32] = {0};
    uint8 *tmp_ptr = NULL, *tmp_ptr1 = NULL;
    uint16 templen = 0;
	
    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Totalsize_By_Filename. filename:%s", filename);
	
	/*find the filename post. */
	tmp_ptr = strstr(buff, filename);
	templen = (uint16)(tmp_ptr -buff);
	if(templen > file_size)
	{
		SCI_ASSERT(0);
	}
	
	/*find the array start.*/
	while(0 < templen)
	{
		if(0x7B == *tmp_ptr) /*{*/
		{
			break;
		}
		tmp_ptr--;
		templen--;
	}
	tmp_ptr = strstr(tmp_ptr, "\"size\":");
	if(tmp_ptr)
	{
		SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
		while(0x3a != *tmp_ptr) /*find :*/
		{
			tmp_ptr++;
		}
		tmp_ptr++;
		while(0x20 ==*tmp_ptr)
		{
			tmp_ptr++;
		}
		
		templen = 0;
		tmp_ptr1 = tmp_ptr;
		while(0x2c != *tmp_ptr)/*find ,*/
		{
			templen++;
			tmp_ptr++;  
		}
		/*read toal size*/
		memcpy(totalsize_str, tmp_ptr1, templen);
	}
	else
	{
		SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Get_Totalsize_By_Filename.ERROR, not find \size\ string ");
	}

	*totalsize = (uint32)byte_atoi(totalsize_str);
	SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_Get_Totalsize_By_Filename  totalsize = %s , len:%d" , totalsize_str, *totalsize); 
	
	return TRUE;
	
}

PUBLIC BOOLEAN MMISRV_BAIDUDRV_GetFsid_By_Filename(uint8 *buff,  uint32 file_size, uint8 *file_name, uint8 *fs_id)
{
    JSON_config jc = {0};
    JSON_parser jp_ptr = PNULL;
    MMISRV_BAIDUDRV_JSON_PARSE_DATA input_data = {0};
    uint8 *tmp_ptr = NULL, *tmp_ptr1=NULL;
    uint8 tmp_count = 0;
    uint16 templen = 0;
    MMISRV_BAIDUDRV_METADATA * tmp_metadata_ptr = PNULL;
    uint8 * current_tag_ptr = PNULL;
    uint16   str_len = 0;
    uint8 *jsonptr = NULL, *checkptr = NULL;

    SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_GetFsid_By_Filename. filename:%s", file_name);

    /*find the filename post. */
     tmp_ptr = strstr(buff, file_name);
     templen = (uint16)(tmp_ptr -buff);
     if(templen > file_size)
     {
         SCI_ASSERT(0);
     }

    /*find the array start.*/
    while(0 < templen)
    {
        if(0x7B == *tmp_ptr) /*{*/
        {
            break;
        }
        tmp_ptr--;
        templen--;
    }
     tmp_ptr = strstr(tmp_ptr, "fs_id");
    if(tmp_ptr)
    {
        SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
        while(0x3a != *tmp_ptr) /*find :*/
        {
            tmp_ptr++;
        }
        tmp_ptr++;
        while(0x20 ==*tmp_ptr)
        {
            tmp_ptr++;
        }
        
        templen = 0;
        tmp_ptr1 = tmp_ptr;
        while(0x2c != *tmp_ptr)/*find ,*/
        {
            templen++;
            tmp_ptr++;   
        }
        /*read fs_id*/
        memcpy(fs_id, tmp_ptr1, templen);
    }
    else
    {
    	SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_GetFsid_By_Filename  not find fs_id string. ERROE.");
    }
		
 
     SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_GetFsid_By_Filename  fs_id:%s, len:%d" , fs_id, templen);

    return TRUE;
}

LOCAL uint8    baidu_drv_delete_file_cnf_func(uint8 app_param, void * param)
{
	SCI_TRACE_LOW("[baidu] Enter baidu_drv_delete_file_cnf_func"); 

       //s_is_process_delete_photo = FALSE;
       //if(s_continue_delete_photo)
       // {
     //      s_continue_delete_photo = FALSE;
      //      MMISRV_BAIDUDRV_Delete_Photo(g_delete_photo_name_list,  g_delete_photo_num);
      //  }
	  if(g_delete_usr_body)
	 {
	 	SCI_FREE(g_delete_usr_body);
		g_delete_usr_body = NULL;
	 }

	//MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY , &baidu_drv_msg_data);
       /*check upload*/
	SCI_TRACE_LOW("[baidu] s_auto_upload_status :%d", s_auto_upload_status); 
	if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
	{
		if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
		   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
		{
			MMISRV_BAIDUDRV_exec_auto_upload_photo();
		}
	}
	return TRUE;
}
/*filename_list:   filename1,filename2, .........*/
PUBLIC void MMISRV_BAIDUDRV_Delete_original_Photo(uint8  *filename_list,  uint16  filenum)
{
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar  file_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	wchar* default_path1 = PNULL;
	wchar  file_path1[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	wchar  wfilename[64] = {0};
	uint8  temp_str[64] = {0};
	uint16 tempfilenum = 0;
	uint8   *temp_filenameptr = NULL;
	uint16  temp_file_len = 0;

	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_original_Photo, :%s", filename_list);

	file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD ;
		default_path1 = MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL;
	}
	
	temp_filenameptr = filename_list;
	temp_file_len = 0;
	tempfilenum = filenum;

	while(0 < tempfilenum)
	{
		if(1 == tempfilenum)
		{
			/*delete photo*/
			MMIAPICOM_StrToWstr(filename_list , wfilename);
			MMIAPIFMM_CombineFullPathEx(file_path, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path, wfilename);
			SCI_TRACE_LOW("[baidu]file_path1, :%s", file_path);
			MMIAPIFMM_DeleteFile( file_path, PNULL);
			MMIAPIFMM_CombineFullPathEx(file_path1, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path1, wfilename);
			SCI_TRACE_LOW("[baidu]file_path11, :%s", file_path1);
			MMIAPIFMM_DeleteFile( file_path1, PNULL);
			break;
		}
		else
		{
			if(0x2c == *(temp_filenameptr))  /*,*/
			{
				SCI_MEMSET(temp_str, 0, 64);
				SCI_MEMCPY(temp_str,  filename_list,  temp_file_len);

				/*delete photo*/
				MMIAPICOM_StrToWstr(temp_str , wfilename);
				MMIAPIFMM_CombineFullPathEx(file_path, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path, wfilename);
				SCI_TRACE_LOW("[baidu]file_path2, :%s", file_path);
				MMIAPIFMM_DeleteFile(file_path, PNULL);
				MMIAPIFMM_CombineFullPathEx(file_path1, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path1, wfilename);
				SCI_TRACE_LOW("[baidu]file_path21, :%s", file_path1);
				MMIAPIFMM_DeleteFile(file_path1, PNULL);

				tempfilenum--;
				filename_list = temp_filenameptr+1;
				temp_filenameptr++;
				temp_file_len = 0;
				continue;
			}
		}
		temp_filenameptr++;
		temp_file_len++;
	}

	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_original_Photo END");
}
	
PUBLIC void  MMISRV_BAIDUDRV_Record_Delete_Photo(uint8  *filename)
{
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	
        if(!g_delete_photo_name_list)
         {
             g_delete_photo_name_list = SCI_ALLOC_APPZ(DELETE_PHOTO_NAME_LIST_MAX_LEN);
             if(g_delete_photo_name_list == NULL)
        	{
        		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Record_Delete_Photo error code MMISRV_BAIDUDRV_Delete_Photo g_delete_photo_name_list == NULL" );
        		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
        		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        		return;
        	}
         }
         /*check repeat name */
         if(strstr(g_delete_photo_name_list, filename))
         {
              SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Record_Delete_Photo error %s have exit.",  filename);
         	return;
         }
		
        if((strlen(g_delete_photo_name_list) + strlen(filename)) > DELETE_PHOTO_NAME_LIST_MAX_LEN)
        {
             SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Record_Delete_Photo error code delete list over 5*1024. L" );
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		return;
        }
        if(0 == g_delete_photo_num)
        {
            sprintf(g_delete_photo_name_list, "%s", filename);
        }
        else
        {
            sprintf(g_delete_photo_name_list, "%s%s%s", g_delete_photo_name_list, ",", filename);
        }
        g_delete_photo_num ++;

}

PUBLIC void MMISRV_BAIDUDRV_Delete_CloadPhoto()
{
    MMISRV_BAIDUDRV_Delete_Photo(g_delete_photo_name_list,  g_delete_photo_num);
    Clean_Categorylist_data_info();
}

/*filename_list:   filename1,filename2, .........*/
PUBLIC void  MMISRV_BAIDUDRV_Delete_Photo(uint8  *filename_list,  uint16  filenum)
{
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
       uint32 multipart_offset = 0, body_h1_len=0,  boundary_len=0;
       uint8  filelist_str[DELETE_PHOTO_NAME_LIST_MAX_LEN]={0};
       uint16 filelist_len = 0;
       uint8  temp_str[64] = {0};
        uint8 avaliable_request_idx,context_id ;
        uint16 tempfilenum = 0;
        uint8   *temp_filenameptr = NULL;
        uint16  temp_file_len = 0;
	
	SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_Delete_Photo !!!!, num:%d", filenum);

        if((NULL == filename_list)||(0 == filenum))
        {
              /*check upload*/
		if(BAIDU_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
		{
			if(MMISRV_BAIDUDRV_Need_manual_auto_upload_photo()||
			   MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo())
			{
				MMISRV_BAIDUDRV_exec_auto_upload_photo();
			}
		}
            return;
        }

    //if(s_is_process_delete_photo)
    //{
    #if 0
        if(!g_delete_photo_name_list)
         {
             g_delete_photo_name_list = SCI_ALLOC_APPZ(DELETE_PHOTO_NAME_LIST_MAX_LEN);
             if(g_delete_photo_name_list == NULL)
        	{
        		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_Photo error code MMISRV_BAIDUDRV_Delete_Photo g_delete_photo_name_list == NULL" );
        		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
        		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        		return;
        	}
         }
        if((strlen(g_delete_photo_name_list) + strlen(filename_list)) > DELETE_PHOTO_NAME_LIST_MAX_LEN)
        {
             SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_Photo error code delete list over 1024. L" );
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		return;
        }
        if(0 == g_delete_photo_num)
        {
            sprintf(g_delete_photo_name_list, "%s", filename_list);
        }
        else
        {
            sprintf(g_delete_photo_name_list, "%s%s%s", g_delete_photo_name_list, ",", filename_list);
        }
        g_delete_photo_num += filenum;
 #endif	
        //s_continue_delete_photo = TRUE;
     //   return;
   // }

    SCI_TRACE_LOW("[baidu]g_delete_photo_name_list:%s", g_delete_photo_name_list);
        
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
    {
        
       http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_DELETE_PHOTO;
       
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_Photo error code MSG_BAIDU_DRV_ERROR_NOTIFY post_param_ptr == NULL" );
		baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
		return;
	}	

	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;
       post_param_ptr->is_use_relative_url = TRUE;
    
    post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
    /*set contect type*/
    post_param_ptr->post_body.is_use_file = FALSE;
    len = strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);
    post_param_ptr->post_body.body_type_ptr = SCI_ALLOC(len + 1);
    memset(post_param_ptr->post_body.body_type_ptr,0,len + 1);
    strcpy(post_param_ptr->post_body.body_type_ptr,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_TYPE);

    /*set filelist*/
    temp_filenameptr = filename_list;
    temp_file_len = 0;
    tempfilenum = filenum;

    sprintf(filelist_str,"%s" , "[");
    filelist_len++;
    while(0 < tempfilenum)
    {
        if(DELETE_PHOTO_NAME_LIST_MAX_LEN < filelist_len + 64) //64 byte is protect
        {
              SCI_TRACE_LOW("[baidu]: error. oversize max delete msg len. len:%d.", filelist_len);
        	break;
        }
        if(1 == tempfilenum)
        {
            sprintf(filelist_str+filelist_len, "{\"path\":\"\/apps\/baipan\/%s\"}", filename_list);
            filelist_len  += (strlen("{\"path\":\"\/apps\/baipan\/") + 2 + strlen(filename_list));
            break;
        }
        else
        {
            if(0x2c == *(temp_filenameptr))  /*,*/
            {
                SCI_MEMSET(temp_str, 0, 64);
                SCI_MEMCPY(temp_str,  filename_list,  temp_file_len);
                sprintf(filelist_str+filelist_len, "{\"path\":\"\/apps\/baipan\/%s\"}", temp_str);
                filelist_len  += (strlen("{\"path\":\"\/apps\/baipan\/") + 2 + strlen(temp_str));
                
                tempfilenum--;
                {
                    sprintf(filelist_str+ filelist_len, "%s",",\r");
                    filelist_len += 2;
                    filename_list = temp_filenameptr+1;
                    temp_filenameptr++;
                    temp_file_len = 0;
                    continue;
                }
            }
        }
        temp_filenameptr++;
        temp_file_len++;
    }
     
    sprintf(filelist_str+ filelist_len,"%s" , "]");
    filelist_len++;
     
    SCI_TRACE_LOW("filelist:end:%s", &filelist_str[filelist_len-6]);
   
    body_h1_len = strlen(BAIDUDRIVE_UPLOAD_END_BODY_H1);
    boundary_len = strlen(BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);

    g_delete_usr_body = SCI_ALLOC_APPZ(512+filenum*40);
    if(g_delete_usr_body == NULL)
    {
	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Delete_Photo error, alloc failg_delete_usr_body== NULL" );
	return;
   }
    
    /*set body*/
    /*boundary*/
    sprintf(g_delete_usr_body ,"--%s" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len+2);
    /*async*/
    sprintf(g_delete_usr_body+multipart_offset ,"%sasync\"\r\n\r\n1" ,BAIDUDRIVE_UPLOAD_END_BODY_H1);
    multipart_offset += (body_h1_len + 11/*strlen("async\"\r\n\r\n2")*/);
    
    /*boundary*/
    sprintf(g_delete_usr_body+multipart_offset ,"\r\n--%s" ,BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 4/*strlen(\r\n--)*/);
    
    /*filelist*/
    sprintf(g_delete_usr_body+multipart_offset ,"%sfilelist\"\r\n\r\n%s" , BAIDUDRIVE_UPLOAD_END_BODY_H1, filelist_str);
    multipart_offset += (body_h1_len + 13/*strlen("filelist\"\r\n\r\n")*/+strlen(filelist_str));
    
    /*end boundary*/
    sprintf(g_delete_usr_body+multipart_offset ,"\r\n--%s--\r\n", BAIDUDRIVE_UPLOAD_HTTP_CONTENT_BOUNDARY);
    multipart_offset += (boundary_len + 8/*strlen(\r\n--)+strlen(--\r\n)*/);

    	post_param_ptr->post_body.u.post_buffer.buffer_ptr = g_delete_usr_body;
	post_param_ptr->post_body.u.post_buffer.buffer_len = multipart_offset+10;

	post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;	

    len=strlen(BAIDUDRIVE_DELETE_URL_PART)+strlen(s_baidu_drv_access_token_str);
    post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(post_param_ptr->uri.uri_ptr,0,len+1);
    sprintf(post_param_ptr->uri.uri_ptr,"%s%s", BAIDUDRIVE_DELETE_URL_PART, s_baidu_drv_access_token_str);
	
#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id ,post_param_ptr, avaliable_request_idx))
	{
		SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_PostRequest success !!!!");
              //s_is_process_delete_photo = TRUE; 
              if(g_delete_photo_name_list)
              {
                  SCI_FREE(g_delete_photo_name_list);
                  g_delete_photo_name_list = NULL;
              }
              g_delete_photo_num = 0;
              // s_continue_delete_photo = FALSE;
	}
#endif

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	SCI_FREE(post_param_ptr);
        }
	return;	
}

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Download_photo(void)
{
    HTTP_GET_PARAM_T* get_param_ptr= NULL;
    uint16 len = 0;
    uint8 avaliable_request_idx , context_id;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	MMIFILE_HANDLE file_handle = 0;

    if(s_b_is_cancel_download_photo)
    {
    	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Download_photo has been canceled !!");	
    	Clean_download_data_info();
    	return FALSE;
    }

    SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Download_photo !!");
    
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);
    
    if(0xFF != context_id)
    {
        SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_Download_photo !!!!");
        get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

        if(get_param_ptr == NULL)
        {
        	SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Download_photo MSG_BAIDU_DRV_ERROR_NOTIFY !!!!get_param_ptr == NULL");
        	return FALSE;
        }	

 /*create file*/
       file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD ;
	}

          MMIAPICOM_StrToWstr(s_current_download_photo_ptr->file_name , img_file_name);
          
	MMIAPIFMM_CombineFullPathEx(s_current_download_photo_ptr->file_path, MMIFILE_FULL_PATH_MAX_LEN,file_dev,default_path, img_file_name);
    
    	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Download_photo  file name:%s" , s_current_download_photo_ptr->file_name);						

	s_current_download_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_current_download_photo_ptr->file_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
	s_current_download_photo_ptr->file_path_len = MMIAPICOM_Wstrlen(s_current_download_photo_ptr->file_path);

	if(SFS_INVALID_HANDLE == s_current_download_photo_ptr->file_handle)
	{
		SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Download_photo create file handle fail  !!!!");
		return FALSE;
	}

	MMIAPIFMM_SetFilePointer(s_current_download_photo_ptr->file_handle, 0, SFS_SEEK_BEGIN);

        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_DOWNLOAD_PHOTO;
        	
        memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  

        get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
        get_param_ptr->need_net_prog_ind = FALSE;
        get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        get_param_ptr->is_use_relative_url = TRUE;

        len=strlen(s_dlink_ptr)+strlen("&access_token=")+strlen(s_baidu_drv_access_token_str);
        get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
        memset(get_param_ptr->uri.uri_ptr,0,len+1);
        sprintf(get_param_ptr->uri.uri_ptr , "%s%s%s" , s_dlink_ptr,"&access_token=",s_baidu_drv_access_token_str);

        SCI_TRACE_LOW("[baidu] uri:%s", get_param_ptr->uri.uri_ptr); 
#ifdef HTTP_SUPPORT
        if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
        {
        	SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
        }
#endif

        SCI_FREE(get_param_ptr->uri.uri_ptr);
        SCI_FREE(get_param_ptr);
	 SCI_FREE(s_dlink_ptr);
	 s_dlink_ptr = NULL;
    }

	return TRUE;	
}

PUBLIC BOOLEAN MMISRV_BAIDUDRV_Get_filemetas(uint8 *fs_id)
{
    uint8 avaliable_request_idx,context_id ;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;
    uint16 len = 0;
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    uint8 temp_uri[1024] = {0};

    SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Get_filemetas");
   
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
    {
        //SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_start_get_thumbnail_info start get meta info from baidu_drv");
        get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

        if(get_param_ptr == NULL)
        {
        	SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Get_filemetas error code MSG_BAIDU_DRV_ERROR_NOTIFY get_param_ptr == NULL" );
        	baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
        	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
        	return FALSE;
        }	

        http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = BAIDU_DRV_OPER_GET_FILE_METAS;
           
        memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  //modify for coverity

        get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
        get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        get_param_ptr->need_net_prog_ind = FALSE;
        get_param_ptr->is_use_relative_url = TRUE;

        sprintf(temp_uri, "%s%s%s%s%s%s", 
                   BAIDUDRIVE_GETFILEINFO_URL_PART1, 
                   s_baidu_drv_access_token_str, 
                   BAIDUDRIVE_GETFILEINFO_URL_PART2,
                   "[",fs_id,"]");

        len=strlen(temp_uri);
        get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
        memset(get_param_ptr->uri.uri_ptr, 0, len+1);
        strcpy(get_param_ptr->uri.uri_ptr, temp_uri);
        //SCI_TRACE_LOW("uri:%s",  get_param_ptr->uri.uri_ptr);
        if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
        {
        	SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
        }

        SCI_FREE(get_param_ptr->uri.uri_ptr);
        SCI_FREE(get_param_ptr);
    }
	
    return TRUE;	
}

LOCAL void  MMISRV_BAIDUDRV_Parse_Filemetas_Data_Json( const uint8* data_ptr, uint32  data_len)
{
       uint8  *tmp_ptr = NULL, *tmp_ptr1=NULL;
       uint8  tmp_count = 0;
       uint16 templen = 0;
       uint8 *jsonptr = NULL;
       
        SCI_TRACE_LOW("[baidu] MMISRV_BAIDUDRV_Parse_Filemetas_Data_Json");

       /*dlink:  dlink": "https:*/
       tmp_ptr = strstr(data_ptr, "dlink");
       if(tmp_ptr)
       {
            //SCI_TRACE_LOW("[baidu] tmp_ptr: %s " , tmp_ptr);
            while(*tmp_ptr++) /*find second "*/
            {
                if(*tmp_ptr == 0x22)
                {
                    tmp_count++;
                }
                if(2 == tmp_count)
                {
                    tmp_ptr++;
                    break;
                }
            }
            tmp_ptr1 = tmp_ptr;
            jsonptr = tmp_ptr;
            while(*tmp_ptr1 != 0x22) /*find the end*/
            {
                if((0x5C == *tmp_ptr1)&&(0x75 == *(tmp_ptr1+1))&&
                    (0x30 == *(tmp_ptr1+2))&& (0x30 == *(tmp_ptr1+3))&&
                    (0x32 == *(tmp_ptr1+4))&& (0x36 == *(tmp_ptr1+5)))
                {
                    *jsonptr = 0x26;
                    tmp_ptr1++;  //0x75
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x30
                    tmp_ptr1++;  //0x32
                    tmp_ptr1++;  //0x36
                }
                else
                {
                    *jsonptr = *tmp_ptr1;
                }
                templen++;
                tmp_ptr1++;
                jsonptr++;
            }
            {
                 //SCI_TRACE_LOW("[baidu] tmp_ptr: %d, %s " , templen, tmp_ptr);
                 if(NULL != s_dlink_ptr)
                 {
                 	SCI_FREE(s_dlink_ptr);
			s_dlink_ptr = NULL;	
                 }
                 s_dlink_ptr = SCI_ALLOC_APPZ(templen+1);
		   SCI_ASSERT(PNULL != s_dlink_ptr);
                 SCI_MEMSET(s_dlink_ptr, 0x00 , templen+ 1);	
                 strncpy(s_dlink_ptr, tmp_ptr , templen);
                 //SCI_TRACE_LOW("[baidu] dlink: %s :%d" , s_dlink_ptr, templen);
            }
        }
	 else
	 {
	 	SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_Parse_Filemetas_Data_Json not find dlink string. ERROR..");
	 }

     SCI_TRACE_LOW("[baidu]  MMISRV_BAIDUDRV_Parse_Filemetas_Data_Json end.");

}
LOCAL uint8    baidu_drv_get_filemetas_data_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_filemetas_data_ind_func");   

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_get_filemetas_data_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

       MMISRV_BAIDUDRV_Parse_Filemetas_Data_Json(data_ptr , data_len);

    //SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_thumbnail_info_ind_func");   
    return TRUE;
}
LOCAL uint8    baidu_drv_get_filemetas_cnf_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	BOOLEAN bIsFound = FALSE;

	SCI_TRACE_LOW("[baidu] Enter baidu_drv_get_filemetas_cnf_func"); 

	
	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);		
       	 	SCI_TRACE_LOW("[baidu]baidu_drv_get_filemetas_cnf_func error code %d" , cnf->rsp_header_info.response_code);
		}
		return FALSE;
	}
       MMISRV_BAIDUDRV_Download_photo();
	
	//MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY , &baidu_drv_msg_data);
	
	return TRUE;
}

PUBLIC BOOLEAN MMISRV_BAIDUDRV_Download_Original_photo(uint8 * file_name , uint16 file_name_len)
{
    uint8 fs_id[100] = {0};
    uint32 read_size = 0 , file_size =0;	
    uint8 * buff = PNULL;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
	
     if(PNULL==file_name || 0==file_name_len  ||s_b_is_cancel_download_photo)
    {
	    SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Download_Original_photo --- (PNULL==file_name || 0==file_name_len ||s_b_is_cancel_download_photo)"); 
	    return FALSE;
    }

    SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Download_Original_photo !! filename %s", file_name);	
	//check auto upload status , if YES , pause first 
	#if 0
    if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status || 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status
	)
	{
		Reset_all_data();
	}
	#endif
	//SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Download_Original_photo get fsid form meta info , filename %s" , *file_name);

        if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        {
            file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_size = MMIAPIFMM_GetFileSize(file_handle);
                buff = SCI_ALLOC(file_size);
                if(PNULL == buff)
                {
                	SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Download_Original_photo --- (PNULL == buff)"); 
                    return FALSE;
                }

                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    buff, 
                    file_size, 
                    &read_size, 
                    NULL
                    );
				
                if(SFS_ERROR_NONE == file_ret)
                {
                    SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Download_Original_photo meta update success !!");
                }
				
                MMIAPIFMM_CloseFile(file_handle);
		  //find fs_id
                if(PNULL == s_current_download_photo_ptr)
                {
                    s_current_download_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
                    SCI_ASSERT(PNULL != s_current_download_photo_ptr);
                }
                else
                {
                    SCI_MEMSET(s_current_download_photo_ptr , 0x00 , sizeof(MMISRV_BAIDUDRV_DOWNLOAD_INFO_T));
                }

		MMISRV_BAIDUDRV_Get_Totalsize_By_Filename(buff,  file_size, file_name , &s_current_download_photo_ptr->total_file_size);
		s_current_download_photo_ptr->saved_file_size = 0 ;
		
		  MMISRV_BAIDUDRV_GetFsid_By_Filename(buff,  file_size, file_name, fs_id);

                s_current_download_photo_ptr->file_name_len = SCI_STRLEN(file_name);
                SCI_STRCPY(s_current_download_photo_ptr->file_name,  file_name);
                MMISRV_BAIDUDRV_Get_filemetas(fs_id);
                SCI_FREE(buff);
                
        }
	}
	else
		return FALSE;

	return TRUE;
}

/**************************************************************************************/
// Description : send signal to MMI Task  
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC  void MMISRV_BAIDUDRV_SendSignalToMMI(uint32 sig_id,DPARAM param)
{
	BAIDU_DRV_SIG_T *signal = PNULL;
	
	if(PNULL == param)
	{
	    return;
	}
    
	MmiCreateSignal(sig_id, sizeof(BAIDU_DRV_SIG_T), (MmiSignalS**)&signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_MEMCPY(&(signal->baidu_drv_msg_data), param, sizeof(MMISRV_BAIDUDRV_MSG_T));
	
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void  MMISRV_BAIDUDRV_Get_Token(int result, char*  buf, uint16 buflen)

{
	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Get_Token !!");	
	
	if(result == BAIDU_OK)
	{
		SCI_MEMSET((void*)s_baidu_drv_access_token_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);
//		sprintf(s_baidu_drv_access_token_str , "Bearer %s" , buf);	
		sprintf(s_baidu_drv_access_token_str , "WLID1.1 t=%s" , buf);			

		//check whether this request is from refresh 
		if(s_baidu_resume_operation != BAIDU_DRV_OPERAION_MAX)
		{
			switch(s_baidu_resume_operation)
			{
				case     BAIDU_DRV_OPER_GET_THUMBNAIL_INFO:
					MMISRV_BAIDUDRV_start_get_thumbnail_info();
					break;
				case     BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL:
					MMISRV_BAIDUDRV_start_download_thumbnails();
					break;
				case     BAIDU_DRV_OPER_CREATE_SESSION:
				case BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION:
					MMISRV_BAIDUDRV_start_upload_file();
					break;
				case     BAIDU_DRV_OPER_UPLOAD_SESSION:
				case BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION:
					baidu_drv_send_session_http_req();
					break;
				case     BAIDU_DRV_OPER_CLOSE_SESSION:
				case BAIDU_DRV_OPER_AUTO_CLOSE_SESSION:
				{
					//need to restart upload process !!!!
					Clean_upload_data_info();
					MMISRV_BAIDUDRV_exec_auto_upload_photo();					
				}
					break;
				case BAIDU_DRV_OPER_DOWNLOAD_PHOTO:
					MMISRV_BAIDUDRV_Download_photo();
					break;
				case BAIDU_DRV_OPER_GET_USER_INFO:
//					MMISRV_BAIDUDRV_Get_user_info_req();
					break;			
				default:
					break;
			}

			s_baidu_resume_operation = BAIDU_DRV_OPERAION_MAX ; 
		}
	}
	else
	{
		SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Get_Token   result != MSA_OK !!!!");
	}
}



/**************************************************************************************/
// Description : Set_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_SYNC_STATE MMISRV_BAIDUDRV_Set_Sync_State(uint8 state)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	BAIDU_DRIVE_SYNC_STATE state_data = state;
#if 0	
	MMINV_WRITE(MMINV_BAIDUDRV_SYNC_STATE, &state_data);
#endif
	return state_data;
}

/**************************************************************************************/
// Description : Get_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_SYNC_STATE MMISRV_BAIDUDRV_Get_Sync_State(void)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	
//#ifdef MMI_SHOW_MSA
	BAIDU_DRIVE_SYNC_STATE state_data = BAIDU_DRV_AUTO_SYNC;
//#else
//	BAIDU_DRIVE_SYNC_STATE state_data = BAIDU_DRV_MANUAL_SYNC;
//#endif	

#if 0
	MMINV_READ(MMINV_BAIDUDRV_SYNC_STATE, &state_data, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_BAIDUDRV_SYNC_STATE, &state_data);
	}
#endif

	return state_data;
}

PUBLIC void MMISRV_BAIDUDRV_Enable_Download(void)
{
	s_b_is_cancel_download_photo = FALSE;
}
/**************************************************************************************/
// Description : Cancel Download
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Cancel_Download(void)
{
	s_b_is_cancel_download_photo = TRUE;
}
#ifdef SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Cancel_Upload(void)
{
	s_b_is_cancel_upload_photo = TRUE;

	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status)
	{
		MMIAPIFILEARRAY_Destroy(&s_baidu_drv_manual_upload_file_array);
		s_baidu_drv_manual_upload_file_array = MMIAPIFILEARRAY_Create();
		MMIAPIFILEARRAY_SaveArrayToFile(s_baidu_drv_manual_upload_file_array, s_baidu_drv_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));
		s_auto_upload_status =BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
	}

	Reset_all_data();
}
#endif

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY  MMISRV_BAIDUDRV_Create_upload_file_ary(void)
{
	s_baidu_drv_snapshot_list_file_array = MMIAPIFILEARRAY_Create();
	return s_baidu_drv_snapshot_list_file_array;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_append_upload_file(wchar * file_path , uint8 file_len)
{
	FILEARRAY_DATA_T array_item = {0};
	SFS_DATE_T  modify_date = {0};
	SFS_TIME_T  modify_time = {0};
       BOOLEAN ret = 0;

	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_append_upload_file");
	
	MMIAPICOM_Wstrncpy(array_item.filename, file_path, file_len);
	array_item.name_len = file_len;
	MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, 
		&array_item.size, &modify_date, &modify_time);
    
	array_item.modify_date.mday = modify_date.mday;
	array_item.modify_date.mon = modify_date.mon;
	array_item.modify_date.year = modify_date.year;
	
	array_item.modify_time.sec = modify_time.sec;
	array_item.modify_time.min = modify_time.min;
	array_item.modify_time.hour = modify_time.hour;

	array_item.type = FILEARRAY_TYPE_FILE;
	ret = MMIAPIFILEARRAY_Add(s_baidu_drv_snapshot_list_file_array, &array_item);
	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_append_upload_file end ret:%d,size:%d", ret, MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_list_file_array));
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Close_upload_file_ary(void)
{
       BOOLEAN  ret = 0;
	   
	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Close_upload_file_ary,size:%d, %d",MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_list_file_array), MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array));
	
	ret = MMIAPIFILEARRAY_Combine(s_baidu_drv_snapshot_upload_file_array,s_baidu_drv_snapshot_list_file_array);
	SCI_TRACE_LOW("[baidu]: ret1:%d ,size:%d, %d",MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_list_file_array), MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array));
	ret = MMIAPIFILEARRAY_SaveArrayToFile(s_baidu_drv_snapshot_upload_file_array, s_baidu_drv_snapshot_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_snapshot_upload_list_file_path));
       SCI_TRACE_LOW("[baidu]: ret2:%d ,size:%d, %d",MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_list_file_array), MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array));

	MMIAPIFILEARRAY_Destroy(&s_baidu_drv_snapshot_list_file_array);
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRIVE_AUTO_UPLOAD_STATE_E MMISRV_BAIDUDRV_Get_Auto_upload_status(void)
{
    SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Get_Auto_upload_status status = %d" , s_auto_upload_status);
    SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Get_Auto_upload_status net_id =%d" , s_baidu_net_id);	

    if( 0 == s_baidu_net_id)
    {
	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Get_Auto_upload_status PDP deactive !!!");	
	return BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK;
    }
	
    return s_auto_upload_status;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo(void)
{
	uint16 upload_file_size = 0 ;
	
	MMISRV_BAIDUDRV_Load_Upload_File_array();

	upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array);

	SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo upload_file_size = %d" , upload_file_size);

	return (upload_file_size > 0) ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_manual_auto_upload_photo(void)
{
	uint16 upload_file_size = 0 ;
#ifdef SINGLE_UPLOAD_SUPPORT
	MMISRV_BAIDUDRV_Load_Upload_File_array();

	upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);

	SCI_TRACE_LOW("[baidu]: MMISRV_BAIDUDRV_Need_manual_auto_upload_photo upload_file_size = %d" , upload_file_size);
#endif
	return (upload_file_size > 0) ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Load_Upload_File_array(void)
{
	MMIFILE_HANDLE file_handle = 0;
	
	//upload file array
	if(PNULL == s_baidu_drv_snapshot_upload_file_array)
	{
		s_baidu_drv_snapshot_upload_file_array = MMIAPIFILEARRAY_Create();
		MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_snapshot_upload_file_array, 20);

		if(MMIAPIFMM_IsFileExist(s_baidu_drv_snapshot_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_snapshot_upload_list_file_path) ))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_baidu_drv_snapshot_upload_file_array , s_baidu_drv_snapshot_upload_list_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_snapshot_upload_list_file_path));
	}	
	else
	{
			file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_snapshot_upload_list_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		MMIAPIFMM_CloseFile(file_handle);
		}
	}
#if 0
	//upload success file array
	if(PNULL == s_baidu_drv_upload_success_file_array)
	{
		s_baidu_drv_upload_success_file_array = MMIAPIFILEARRAY_Create();
		MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_upload_success_file_array, 10);

		//upload success file array
		if(MMIAPIFMM_IsFileExist(s_baidu_drv_upload_success_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path)))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_baidu_drv_upload_success_file_array , s_baidu_drv_upload_success_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path));
		}
		else
			{
			file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_upload_success_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
			MMIAPIFMM_CloseFile(file_handle);
			}
	}
	#endif
#ifdef SINGLE_UPLOAD_SUPPORT
	// manual upload file array
	if(PNULL == s_baidu_drv_manual_upload_file_array)
	{
		s_baidu_drv_manual_upload_file_array = MMIAPIFILEARRAY_Create();
		MMIVIRTUALARRAY_SetBufferSize(s_baidu_drv_manual_upload_file_array, 2);

		//upload success file array
		if(MMIAPIFMM_IsFileExist(s_baidu_drv_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path)))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_baidu_drv_manual_upload_file_array , s_baidu_drv_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));
		}
		else
		{
			file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_manual_upload_list_file_path , SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
			MMIAPIFMM_CloseFile(file_handle);
}
	}
#endif

}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Destroy(void)
{

    SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Destroy g_current_module_id = %d" , g_current_module_id);

    Reset_all_data();
    Destroy_Http_all_request();
}

#ifdef SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_Manual_Upload_Photo_Count(void)
{
	return MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);
}
#endif
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Check_Network_status(BOOLEAN network_on)
{

        MN_PHONE_PLMN_STATUS_E net_status = PLMN_NO_SERVICE;
        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        MMISRV_BAIDUDRV_MSG_T sig_data={0};	
        MMI_GMMREG_RAT_E current_rat;
        BOOLEAN bIsFound = FALSE;
		
        if (FALSE == network_on)
        {
        	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Check_Network_status and NO NETWORK !!!!");

		MMISRV_DeleteBaiduDrvTaskThread();

              s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK;	
		if(s_is_in_gallery)
		{
			sig_data.error_type = BAIDU_DRIVE_ERROR_NO_NETWORK;	
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
		}
        }
	 else
	 {        	

#if 0

        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        BOOLEAN bIsFound = FALSE;

            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
            if(sim_ok_num ==0)
            {
                bIsFound = FALSE;
            }
            else if (sim_ok_num == 1)
            {
                bIsFound = TRUE;              
            }

//            bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sim_ok);

            if(FALSE == bIsFound)
            {
                return FALSE;
            }

            current_rat = MMIAPIPHONE_GetTDOrGsm(sim_ok);

            if (MMI_GMMREG_RAT_GPRS != current_rat
            &&	MMI_GMMREG_RAT_3G != current_rat
            && MMI_GMMREG_RAT_GPRS_AND_3G != current_rat)
            {
                SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Check_Network_status No Network !! %d" , current_rat);
                MMISRV_DeleteBaiduDrvTaskThread();
                s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK;	
                if(s_is_in_gallery)
                {
                    sig_data.error_type = BAIDU_DRIVE_ERROR_NO_NETWORK;	
                    MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
                }				
                return TRUE;
            }
#endif

		switch(s_auto_upload_status)
		{
			case     BAIDU_DRIVE_AUTO_UPLOAD_NONE:
			case     BAIDU_DRIVE_AUTO_UPLOAD_NO_NETWORK:
	              	s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
				break;
			case     BAIDU_DRIVE_AUTO_UPLOAD_IDLE:
			case     BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING:
			case     BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING:
				break;
		}

	       SCI_TRACE_LOW("[baidu]:MMISRV_BAIDUDRV_Check_Network_status s_auto_upload_status = %d " , s_auto_upload_status);
	 }

	 return 1;
}
#ifdef SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Delete_Manual_Upload_File_array(wchar* filename , uint16 file_len)
{
	uint32 manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);
	FILEARRAY_DATA_T file_data = {0};
	uint32 i=0 ,  count=0;

			count = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);

			for(i=0;i<count;i++)
			{
				MMIAPIFILEARRAY_Read(s_baidu_drv_manual_upload_file_array , i ,&file_data);
				
		if(((file_data.name_len == file_len) && 
		      ( 0 == MMIAPICOM_Wstrncmp(file_data.filename , filename , file_len)))
				   )
				{
					MMIAPIFILEARRAY_Delete(s_baidu_drv_manual_upload_file_array , i);
					break;
				}
			}

		MMIAPIFILEARRAY_SaveArrayToFileEx(s_baidu_drv_manual_upload_file_array , s_baidu_drv_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));		
	}

#endif
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Get_Connect_State(void)
{
#if 0
#ifdef MSA_SUPPORT
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	BOOLEAN state_data = FALSE;
	
	MMINV_READ(MMINV_BAIDUDRV_IS_CONNECT_BAIDUDRIVE, &state_data, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_BAIDUDRV_IS_CONNECT_BAIDUDRIVE, &state_data);
	}
	return state_data;
#else
    return FALSE;
#endif
#endif

	return TRUE;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Set_Connect_State(BOOLEAN bConnect)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	BOOLEAN state_data = bConnect;

#if 0	
	MMINV_WRITE(MMINV_BAIDUDRV_IS_CONNECT_BAIDUDRIVE, &state_data);
#endif

	if(FALSE == bConnect)
	{
		MMISRV_DeleteBaiduDrvTaskThread();
	}

	return;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_Network_init_Done(void)
{
	return s_baidu_task_init_done;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BAIDU_DRV_OPERATION_E MMISRV_BAIDUDRV_Check_Current_auto_upload_request(void)
{
	uint8 i=0,j=0;
	BAIDU_DRV_OPERATION_E retval = BAIDU_DRV_OPER_NO_OPERATION;
	
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			switch(http_request_operation_array[i].operation_ary[j].operation)
		{
			case BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION:
			case BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION:
			case BAIDU_DRV_OPER_AUTO_CLOSE_SESSION:
					retval =  http_request_operation_array[i].operation_ary[j].operation;
			break;
		}	
	}
	}

	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Check_Current_auto_upload_request %d " , retval);

	return retval;
}

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_Is_Downloading_Thumbanils
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Is_Downloading_Thumbanils(void)
{
	return s_is_process_download_thumbnail;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY MMISRV_BAIDUDRV_Get_Delete_photo_FileAry(void)
{
	return	s_baidu_drv_deleted_list_file_array;
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Get_OneDrive_Info(MMISRV_BAIDUDRV_DRIVEINFO_T * info)
{
#if 0
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	MMISRV_BAIDUDRV_DRIVEINFO_T onedrive_info = {0};

	
	MMINV_READ(MMINV_BAIDUDRV_ONDRIVE_INFO, &onedrive_info, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_BAIDUDRV_ONDRIVE_INFO, &onedrive_info);
	}


	SCI_MEMCPY((void*)info , (void*)&onedrive_info , sizeof(MMISRV_BAIDUDRV_DRIVEINFO_T));
#endif
	return ;	
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_Set_OneDrive_Info(MMISRV_BAIDUDRV_DRIVEINFO_T * info)
{
#if 0
	MMINV_WRITE(MMINV_BAIDUDRV_ONDRIVE_INFO, info);
#endif
	return;
}

#ifdef SINGLE_UPLOAD_SUPPORT
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Clean_manual_upload_list(void)
{

	MMIAPIFILEARRAY_Destroy(&s_baidu_drv_manual_upload_file_array);
	s_baidu_drv_manual_upload_file_array = MMIAPIFILEARRAY_Create();
	MMIAPIFILEARRAY_SaveArrayToFile(s_baidu_drv_manual_upload_file_array, s_baidu_drv_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path));

	return TRUE;
}
#endif
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Need_Get_Token()
{
    MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE;
    uint8 *buf = NULL;
    uint32  file_len = 0;
    uint32  file_read_len =0;
    MMIFILE_ERROR_E   file_ret = SFS_ERROR_NONE;
    uint32  token_expire_timer = 0;
    uint8    token_expires_timer_ptr[64] = {0};
    uint8  *start_ptr = NULL, *end_ptr = NULL;
    uint32  tmp_ptr_len = 0;
    uint32  cur_timer_s = 0;

    if((s_baidu_drv_refresh_token_timer)&&(MMK_IsTimerActive(s_baidu_drv_refresh_token_timer)))
    {
        return FALSE;
    }
    
    /*read the config.ini file.*/
    if(MMIAPIFMM_IsFileExist(s_baidu_drv_configure_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_configure_file_path)))
    {
        file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_configure_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
        if(SFS_INVALID_HANDLE == file_handle)
        {
            baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
            MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
            return FALSE;		
        }
        file_len = MMIAPIFMM_GetFileSize(file_handle);
	 if(0 < file_len)
	 {
	     buf = SCI_ALLOC_APPZ(file_len);
	 }
	 else
        {
            SCI_TRACE_LOW("CONFIG.INI FILE LEN 0!!!!!!");
            return TRUE;
        }
        

	if(buf == NULL)
	{
		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Need_Get_Token error code ALLOC buf fail.!!!!" );
              MMIAPIFMM_CloseFile(file_handle);
		return TRUE;
	}
        /*read file*/

        file_ret = MMIAPIFMM_ReadFile(file_handle, (void*)buf, file_len, &file_read_len, NULL);
        if(SFS_ERROR_NONE == file_ret)
        {
        	SCI_TRACE_LOW("[baidu] read config.ini file  success !!");
        }
	MMIAPIFMM_CloseFile(file_handle);
        /*get the data*/
        end_ptr = strstr(buf, "expires_timer");
        if(!end_ptr)
        {
              SCI_TRACE_LOW("NOT FIND expires_timer");
             return TRUE;
        }
        tmp_ptr_len = 0;
        while(end_ptr < (buf+file_read_len))
        {
            if(0x3A == *end_ptr) /*:*/
            {
                SCI_TRACE_LOW("end_ptr1:%s", end_ptr);
                start_ptr = end_ptr+1;
                end_ptr++;
                tmp_ptr_len = 0;
                continue;
            }
            if(0x2C == *end_ptr) /*,*/
            {
                SCI_TRACE_LOW("start_ptr2:%s", start_ptr);
                SCI_MEMCPY(token_expires_timer_ptr, start_ptr, tmp_ptr_len);
                break;
            }
            end_ptr++;
            tmp_ptr_len++;
        }
        SCI_TRACE_LOW("token_expires_timer:%s", token_expires_timer_ptr);
        token_expire_timer = (uint32)byte_atoi(token_expires_timer_ptr);
        SCI_TRACE_LOW("token_expires_timer1:%d", token_expire_timer);

        end_ptr = strstr(buf, "access_token");
	 if(!end_ptr)
        {
             SCI_TRACE_LOW("NOT FIND access_token");
             return TRUE;
        }
        tmp_ptr_len = 0;
        while(end_ptr < (buf+file_read_len))
        {
            if(0x3A == *end_ptr) /*:*/
            {
                SCI_TRACE_LOW("end_ptr1:%s", end_ptr);
                start_ptr = end_ptr+1;
                end_ptr++;
                tmp_ptr_len = 0;
                continue;
            }
            if(0x2C == *end_ptr) /*,*/
            {
                SCI_TRACE_LOW("start_ptr2:%s", start_ptr);
                SCI_MEMCPY(s_baidu_drv_access_token_str, start_ptr, tmp_ptr_len);
                break;
            }
            end_ptr++;
            tmp_ptr_len++;
        }
        SCI_TRACE_LOW("s_baidu_drv_access_token_str:%s", s_baidu_drv_access_token_str);

        end_ptr = strstr(buf, "refresh_token");
	 if(!end_ptr)
        {
             memset(s_baidu_drv_access_token_str, 0, BAIDU_TOKEN_MAX_SIZE + 1);
             SCI_TRACE_LOW("NOT FIND refresh_token");
             return TRUE;
        }
        tmp_ptr_len = 0;
        while(end_ptr < (buf+file_read_len))
        {
            if(0x3A == *end_ptr) /*:*/
            {
                SCI_TRACE_LOW("end_ptr1:%s", end_ptr);
                start_ptr = end_ptr+1;
                SCI_STRCPY(s_baidu_drv_refresh_token_str, start_ptr);
                break;
            }
            end_ptr++;
        }
        SCI_TRACE_LOW("s_baidu_drv_refresh_token_str:%s", s_baidu_drv_refresh_token_str);

        cur_timer_s = MMIAPICOM_GetCurTime();
        /*if left not more than 30 min, get token again. */
        if(60*30  > token_expire_timer)
        {
            memset(s_baidu_drv_access_token_str, 0, BAIDU_TOKEN_MAX_SIZE + 1);
	     memset(s_baidu_drv_refresh_token_str, 0, BAIDU_TOKEN_MAX_SIZE + 1);
            return TRUE;
        }
        /*if left not more than 30 min, get token again. */
        if(cur_timer_s > (token_expire_timer-60*30))
         {
             memset(s_baidu_drv_access_token_str, 0, BAIDU_TOKEN_MAX_SIZE + 1);
	     memset(s_baidu_drv_refresh_token_str, 0, BAIDU_TOKEN_MAX_SIZE + 1);
             return TRUE;
         }
        /*if left timer not more than 12 +12 hours,  refresh*/
        if(24*60*60+1000  > (token_expire_timer - cur_timer_s))
        {
            MMIAPIBAIDU_refresh_access_token();
            return FALSE;
        }
        
        /*start token timer.*/
        if(!s_baidu_drv_refresh_token_timer)
        {
            s_baidu_drv_refresh_token_timer =  MMK_CreateTimerCallback((token_expire_timer - 24*60*60), MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }
        else
        {
            if(MMK_IsTimerActive(s_baidu_drv_refresh_token_timer))
            {
                MMK_StopTimer(s_baidu_drv_refresh_token_timer);
            }

            tmp_ptr_len = 1000;
            while(0 < tmp_ptr_len)
            {
                 tmp_ptr_len--;
            }
            MMK_StartTimerCallback(s_baidu_drv_refresh_token_timer, (token_expire_timer - 24*60*60), MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }

        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/*****************************************************************************/
// 	Description : Is Account Exist
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_IsAccountExist(void)
{
	return (0 < strlen(s_baidu_drv_access_token_str));
}

LOCAL uint8    baidu_drv_refresh_token_ind_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
       MMIFILE_HANDLE  file_handle = 0;
       uint32  cur_timer_s = 0;
       uint32  expires_timer_s = 0;
       uint8 *tmp_data[1024] = {0};
       MMIFILE_ERROR_E  file_ret = SFS_NO_ERROR;
       uint32  write_len = 0;
       uint32  timer_count = 0;

    	SCI_TRACE_LOW("[baidu] Enter baidu_drv_refresh_token_ind_func");   

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
			SCI_TRACE_LOW("[baidu]baidu_drv_refresh_token_ind_func MSG_BAIDU_DRV_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	MMISRV_BAIDUDRV_Get_Token_Data(data_ptr , data_len);

         cur_timer_s = MMIAPICOM_GetCurTime();
         expires_timer_s = cur_timer_s + s_tocken_expires_in;
         SCI_TRACE_LOW("get tocken :cur_timer:%d, expires_timer:%d, expires_in: %d", cur_timer_s, expires_timer_s, s_tocken_expires_in);

         sprintf(tmp_data, "expires_timer:%d,access_token:%s,refresh_token:%s",expires_timer_s,s_baidu_drv_access_token_str, s_baidu_drv_refresh_token_str);

       /*check file mem if enough*/
      if(FALSE == MMISRV_BAIDUDRV_Check_Device_Space(strlen(tmp_data)))
      {
      		return FALSE;
      }

       /*save tocken to file.*/
        file_handle = MMIAPIFMM_CreateFile(s_baidu_drv_configure_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
        if(SFS_INVALID_HANDLE == file_handle)
        {
            baidu_drv_msg_data.error_type = BAIDU_DRIVE_ERROR_UNKNOWN;
            MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &baidu_drv_msg_data);
            return FALSE;		
        }
         
         file_ret =  MMIAPIFMM_WriteFile(file_handle, tmp_data, strlen(tmp_data), &write_len, NULL);
        if(SFS_ERROR_NONE == file_ret)
        {
        	SCI_TRACE_LOW("[baidu] write access token  success !!");
        }
	 MMIAPIFMM_CloseFile(file_handle);
        /*start token timer.*/
        if((24*60*60+1000) < s_tocken_expires_in)
        {
            timer_count = s_tocken_expires_in - 24*60*60;
        }
        else
        {
             timer_count = (s_tocken_expires_in>60*60)?(s_tocken_expires_in-30*60):s_tocken_expires_in;
         }

        // timer_count = 30;    /*test  30s refresh*/
        if(!s_baidu_drv_refresh_token_timer)
        {
            s_baidu_drv_refresh_token_timer =  MMK_CreateTimerCallback(timer_count, MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }
        else
        {
            if(MMK_IsTimerActive(s_baidu_drv_refresh_token_timer))
            {
                MMK_StopTimer(s_baidu_drv_refresh_token_timer);
            }

            write_len = 1000;
            while(0 < write_len)
            {
                 write_len--;
            }
            MMK_StartTimerCallback(s_baidu_drv_refresh_token_timer, timer_count, MMIAPIBAIDU_refresh_access_token, PNULL, FALSE);
        }

	baidu_drv_get_user_info_req();
	baidu_drv_get_disc_size_info_req();
 
    return TRUE;
}

PUBLIC int MMIAPIBAIDU_refresh_access_token(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
		

	SCI_TRACE_LOW("[baidu]: Enter MMIAPIBAIDU_refresh_access_token !!");	

    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:1  MMIAPIBAIDU_refresh_access_token !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
                     SCI_TRACE_LOW("[baidu]: SCI_ALLOC_APPZ return null error!!!!");
			return FALSE;
		}

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_REFRESH_TOKEN;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(BAIDUDRIVE_REFRESH_TOKEN_URL_PART1) + strlen(s_baidu_drv_refresh_token_str)+strlen(BAIDUDRIVE_GET_TOKEN_URL_PART2);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC_APPZ(len+1);
		if(get_param_ptr->uri.uri_ptr == NULL)
		{
                     SCI_TRACE_LOW("[baidu]: SCI_ALLOC_APPZ return null error!!!!");
			return FALSE;
		}
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s%s" , BAIDUDRIVE_REFRESH_TOKEN_URL_PART1 , s_baidu_drv_refresh_token_str , BAIDUDRIVE_GET_TOKEN_URL_PART2);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		SCI_FREE(get_param_ptr);
	}
	return TRUE;	
}

/*****************************************************************************/
//  FUNCTION:      MMIAPIBAIDU_get_access_token
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIBAIDU_get_access_token(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
		

	SCI_TRACE_LOW("[baidu]: Enter MMIAPIBAIDU_get_access_token !!");	
	
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  MMIAPIBAIDU_get_access_token !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_OPER_GET_TOKEN;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(BAIDUDRIVE_GET_TOKEN_URL_PART1) + strlen(s_baidu_drv_code_str)+strlen(BAIDUDRIVE_GET_TOKEN_URL_PART2);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s%s" , BAIDUDRIVE_GET_TOKEN_URL_PART1 , s_baidu_drv_code_str , BAIDUDRIVE_GET_TOKEN_URL_PART2);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  FUNCTION:      MMISRV_BAIDUDRV_get_user_info
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIAPIBAIDU_USER_INFO_T * MMISRV_BAIDUDRV_get_user_info(void)
{
	return &s_baidu_user_info;
}

/**************************************************************************************/
// Description : get qr image
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_BAIDUDRV_Get_QR_Image(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_BAIDUDRV_MSG_T baidu_drv_msg_data={0};
		

	SCI_TRACE_LOW("[baidu]: Enter MMISRV_BAIDUDRV_Get_QR_Image !!");	
	
    BAIDU_DRV_GetMutex(s_baidu_drv_mutex_ptr);
    context_id = baidu_drv_get_avaliable_req_index(&avaliable_request_idx);
    BAIDU_DRV_PutMutex(s_baidu_drv_mutex_ptr);

    if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[baidu]:START  MMISRV_BAIDUDRV_Get_QR_Image !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return FALSE;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= BAIDU_DRV_DEVICE_CODE ; //BAIDU_DRV_OPER_GET_QR_IMG;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(BAIDUDRIVE_DEVICE_CODE_URL);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s" , BAIDUDRIVE_DEVICE_CODE_URL);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[baidu]:HandleHttpMsg HTTP_GetRequest OK context_id = %d , app_instance = %d " , context_id , avaliable_request_idx);
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_is_in_BaiduFolder
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_is_in_BaiduFolder(
PIC_SOURCE_TYPE type,
const wchar      *path_name_ptr,     //in
uint16           path_name_len      //in, 双字节为单位
)
{
	const wchar* dev_path = PNULL;
	int dev_len = MMIAPIFMM_GetDevicePathLen(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));	
	MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};	

    dev_path = MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));
	if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
	{
		MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);     
	}

	return MMIFILE_FileIsInThisFolder (default_path.path , default_path.path_len , path_name_ptr , path_name_len);
}

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_Logout
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_BAIDUDRV_Logout(void)
{

	MMIFILE_ERROR_E sfs_err = SFS_NO_ERROR;
	wchar            default_dir[MMIFILE_FILE_NAME_MAX_LEN] = {0};
	uint16           device_len = MMIFILE_FILE_NAME_MAX_LEN;
	wchar            full_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	uint16           path_len = MMIFILE_FULL_PATH_MAX_LEN;

	Reset_all_data();
	SCI_MEMSET((void*)s_baidu_drv_access_token_str , 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);
	SCI_MEMSET((void*)s_baidu_drv_refresh_token_str, 0x00 , BAIDU_TOKEN_MAX_SIZE + 1);
	SCI_MEMSET((void*)&s_baidu_user_info , 0x00 , sizeof(MMIAPIBAIDU_USER_INFO_T));

	MMIAPIFMM_CombineFullPathEx(full_path, MMIFILE_FULL_PATH_MAX_LEN,MMI_DEVICE_UDISK,MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL, PNULL);

	path_len = MMIAPICOM_Wstrlen(full_path);
	device_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
	MMIAPICOM_Wstrncpy(default_dir, MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MIN(device_len, MMIFILE_FILE_NAME_MAX_LEN));
	if(device_len < MMIFILE_FILE_NAME_MAX_LEN)
	{
	    default_dir[device_len] = ':';
	}

	if(MMIAPIFMM_IsFolderExist(full_path, path_len))
	{
		sfs_err =  MMIAPIFMM_DeleteTreeEx(default_dir,MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL,FALSE); 
		if (!((SFS_NO_ERROR == sfs_err) || (SFS_ERROR_NOT_EXIST == sfs_err)))
		{		
			SCI_TRACE_LOW("[baidu]delete thumbnail dir , result:%d",sfs_err);
			return FALSE;
		}
	}

	SCI_MEMSET(full_path , 0x00 , MMIFILE_FULL_PATH_MAX_LEN * sizeof(wchar));
	MMIAPIFMM_CombineFullPathEx(full_path, MMIFILE_FULL_PATH_MAX_LEN,MMI_DEVICE_UDISK,MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD, PNULL);

	path_len = MMIAPICOM_Wstrlen(full_path);
	if(MMIAPIFMM_IsFolderExist(full_path, path_len))
	{
		sfs_err =  MMIAPIFMM_DeleteTreeEx(default_dir,MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD,FALSE); 
		if (!((SFS_NO_ERROR == sfs_err) || (SFS_ERROR_NOT_EXIST == sfs_err)))
		{		
			SCI_TRACE_LOW("[baidu]delete download dir , result:%d",sfs_err);
			return FALSE;
		}
	}	
	
	SCI_MEMSET(full_path , 0x00 , MMIFILE_FULL_PATH_MAX_LEN * sizeof(wchar));
	MMIAPIFMM_CombineFullPathEx(full_path, MMIFILE_FULL_PATH_MAX_LEN,MMI_DEVICE_UDISK,MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD, PNULL);

	path_len = MMIAPICOM_Wstrlen(full_path);
	if(MMIAPIFMM_IsFolderExist(full_path, path_len))
	{
		sfs_err =  MMIAPIFMM_DeleteTreeEx(default_dir,MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD,FALSE); 
		if (!((SFS_NO_ERROR == sfs_err) || (SFS_ERROR_NOT_EXIST == sfs_err)))
		{		
			SCI_TRACE_LOW("[baidu]delete upload dir , result:%d",sfs_err);
			return FALSE;
		}
	}

	if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
	{
		sfs_err = MMIAPIFMM_DeleteFile(s_meta_info_file_path, NULL);

		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d" , sfs_err);
		        return FALSE;
		}
	}
	
	if(MMIAPIFMM_IsFileExist(s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path)))	
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_new_meta_info_file_path, NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}	
	}
	
	if(MMIAPIFMM_IsFileExist(s_baidu_drv_upload_success_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_upload_success_file_path)))
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_baidu_drv_upload_success_file_path, NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}
	}
	
	if(MMIAPIFMM_IsFileExist(s_baidu_drv_snapshot_upload_list_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_snapshot_upload_list_file_path)))
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_baidu_drv_snapshot_upload_list_file_path , NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}
	}
	
	if(MMIAPIFMM_IsFileExist(s_baidu_drv_manual_upload_list_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_manual_upload_list_file_path)))
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_baidu_drv_manual_upload_list_file_path , NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}
	}

	if(MMIAPIFMM_IsFileExist(s_baidu_drv_user_info_path, MMIAPICOM_Wstrlen(s_baidu_drv_user_info_path)))
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_baidu_drv_user_info_path , NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}
	}
	
	if(MMIAPIFMM_IsFileExist(s_baidu_drv_configure_file_path, MMIAPICOM_Wstrlen(s_baidu_drv_configure_file_path)))
	{
		sfs_err =MMIAPIFMM_DeleteFile(s_baidu_drv_configure_file_path , NULL);
		if (SFS_NO_ERROR != sfs_err)
		{
		        SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Logout delete file error !! sfs_err=%d", sfs_err);
		        return FALSE;
		}
	}
	
	return TRUE;
	
}


/*****************************************************************************/
//  Description : Free Header
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader, uint32 header_num)
{
    uint32 i = 0;
    if (pHttpExternHeader == NULL)
        return ;
	
    for (; i < header_num; i++)
    {
        if (pHttpExternHeader->other_header_ptr[i].header_name_ptr != NULL)
        {
			SCI_FREE(pHttpExternHeader->other_header_ptr[i].header_name_ptr);
			pHttpExternHeader->other_header_ptr[i].header_name_ptr = NULL;
        }
        if (pHttpExternHeader->other_header_ptr[i].header_value_ptr != NULL)
        {
			SCI_FREE(pHttpExternHeader->other_header_ptr[i].header_value_ptr);
			pHttpExternHeader->other_header_ptr[i].header_value_ptr = NULL;
        }
    }
}

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Remove_File_From_UploadFileAry(uint16 * file_path , uint16 file_path_len)
{
	uint16 snapshot_upload_file_size = 0 ,  manual_upload_file_size =0 ,i=0;
	FILEARRAY_DATA_T upload_file_data = {0};
	FILEARRAY_DATA_T manual_upload_file_data = {0};

	SCI_TRACE_LOW("[baidu]:ENTER Remove_File_From_UploadFileAry !!!"); 

	snapshot_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array);

	#ifdef SINGLE_UPLOAD_SUPPORT
	manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_manual_upload_file_array);

	if(manual_upload_file_size > 0) 
	{
		for(i=0; i < manual_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_baidu_drv_manual_upload_file_array , i , &manual_upload_file_data);

			if(((manual_upload_file_data.name_len == file_path_len) && 
			( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
			)
			{
				MMIAPIFILEARRAY_Delete(s_baidu_drv_manual_upload_file_array , i);
				SCI_TRACE_LOW("[baidu]:ENTER Remove_File_From_UploadFileAry Delete array data !!!"); 
				break;
			}
		}
	}
	#endif
	if(snapshot_upload_file_size > 0)
	{
		for(i=0; i < snapshot_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_baidu_drv_snapshot_upload_file_array , i , &manual_upload_file_data);

			if(((manual_upload_file_data.name_len == file_path_len) && 
			( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
			)
			{
				MMIAPIFILEARRAY_Delete(s_baidu_drv_snapshot_upload_file_array , i);
				SCI_TRACE_LOW("[baidu]:ENTER Remove_File_From_UploadFileAry Delete array data !!!"); 
				break;
			}
		}
	}	
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_Http_all_request(void)
{
	uint8 i=0,j=0;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	
	SCI_TRACE_LOW("[baidu]: Enter Reset_Http_all_request");

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if((BAIDU_DRV_OPER_NO_OPERATION  != http_request_operation_array[i].operation_ary[j].operation) && 
			    (BAIDU_DRV_OPER_NOT_INIT  != http_request_operation_array[i].operation_ary[j].operation))
			{
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			}
		}			
	}

       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;	
	s_b_is_cancel_upload_photo = FALSE;
	s_b_is_cancel_download_photo = FALSE;

	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
	}	
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_meta_data_info(void)
{
	MMISRV_BAIDUDRV_METADATA * tmp_ptr = PNULL;

	SCI_TRACE_LOW("[baidu]: Enter Clean_meta_data_info");
		
	while(s_baidu_drv_metadata_ptr)
	{
		tmp_ptr =  s_baidu_drv_metadata_ptr;
		s_baidu_drv_metadata_ptr = s_baidu_drv_metadata_ptr->next_ptr;
		if(tmp_ptr->id) SCI_FREE(tmp_ptr->id);
		if(tmp_ptr->name) SCI_FREE(tmp_ptr->name);
		if(tmp_ptr->thumbnail_url) SCI_FREE(tmp_ptr->thumbnail_url);	
		SCI_FREE(tmp_ptr);
	}
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_upload_data_info(void)
{
       int i, j;
	
	SCI_TRACE_LOW("[baidu]: Enter Clean_upload_data_info");
#if 1
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if((BAIDU_DRV_OPER_CREATE_SESSION  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_UPLOAD_SESSION  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_CLOSE_SESSION  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_AUTO_UPLOAD_CREATE_SESSION  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_AUTO_UPLOAD_SESSION  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_AUTO_CLOSE_SESSION  == http_request_operation_array[i].operation_ary[j].operation) 
			    )
			{
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			}
		}			
	}
#endif		
	if(s_baidu_drv_upload_photo_ptr)
	{
		if(0 !=s_baidu_drv_upload_photo_ptr->file_handle)
		{
			MMIAPIFMM_CloseFile(s_baidu_drv_upload_photo_ptr->file_handle);
		}
		if(s_baidu_drv_upload_photo_ptr->file_md5_list)
		{
			SCI_FREE(s_baidu_drv_upload_photo_ptr->file_md5_list);
			s_baidu_drv_upload_photo_ptr->file_md5_list = PNULL;
		}
		
	    	SCI_FREE(s_baidu_drv_upload_photo_ptr);
		s_baidu_drv_upload_photo_ptr = NULL;	
	}

	if(s_upload_photo_ary.buf_ptr)
	{
		SCI_FREE(s_upload_photo_ary.buf_ptr);
              s_upload_photo_ary.buf_ptr = NULL;
		s_upload_photo_ary.buf_size =0;
	}

	if(BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		s_auto_upload_status = BAIDU_DRIVE_AUTO_UPLOAD_IDLE;
	}
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_download_data_info(void)
{
	uint8 i ,j ;
	if((PNULL != s_current_download_photo_ptr) && (SFS_INVALID_HANDLE != s_current_download_photo_ptr->file_handle))
	{
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_current_download_photo_ptr = PNULL;
	}

       if((PNULL != s_current_download_photo_ptr) && (SFS_INVALID_HANDLE != s_current_download_photo_ptr->file_handle))
	{
		MMIAPIFMM_CloseFile(s_current_download_photo_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_photo_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_photo_ptr);
		s_current_download_photo_ptr = PNULL;
	}
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if(BAIDU_DRV_OPER_DOWNLOAD_PHOTO== http_request_operation_array[i].operation_ary[j].operation)
			{
				SCI_TRACE_LOW("[baidu]: Clean_download_data_info cancel download request !! context id %d , request %d " , i , j );	
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			}
		}			
	}
	s_b_is_cancel_download_photo = FALSE;
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Destroy_Http_all_request(void)
{
	uint8 i=0;

	SCI_TRACE_LOW("[baidu]: Enter Clean_Http_all_request");
	
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		HTTP_CloseRequest(http_request_operation_array[i].context_id);
		SCI_MEMSET((void*)&http_request_operation_array[i] , 0x00 , sizeof(MMISRV_BAIDUDRV_REQUEST_ARRAY_T));
	}

       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;	
	s_b_is_cancel_upload_photo = FALSE;
	s_b_is_cancel_download_photo = FALSE;	   
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_all_data(void)
{
	SCI_TRACE_LOW("[baidu]: Enter Reset_all_data");
	Reset_Http_all_request();
	Clean_meta_data_info();
	Clean_upload_data_info();
	Clean_download_data_info();

	if(BAIDU_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status ||
	    BAIDU_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status)
		s_auto_upload_status =BAIDU_DRIVE_AUTO_UPLOAD_IDLE;

	if(SFS_INVALID_HANDLE !=s_meta_data_file_handle)
	{
		MMIAPIFMM_CloseFile(s_meta_data_file_handle);
		s_meta_data_file_handle = SFS_INVALID_HANDLE ;
	}	
	
	s_download_thumbnail_total_count =0;
       s_download_thumbnail_fail_count =0;
       s_download_thumbnail_success_count =0;
	s_b_is_cancel_upload_photo = FALSE;
       s_b_is_cancel_download_photo = FALSE;	   
       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_Categorylist_data_info(void)
{
       int i, j;
	
	SCI_TRACE_LOW("[baidu]: Enter Clean_Categorylist_data_info");

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if((BAIDU_DRV_OPER_DOWNLOAD_THUMBNAIL  == http_request_operation_array[i].operation_ary[j].operation) ||
			    (BAIDU_DRV_OPER_GET_CATEGORYLIST  == http_request_operation_array[i].operation_ary[j].operation) )
			{
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_url , 0x00 , sizeof(char) * BAIDU_DRV_MAX_STRING_LEN);
			}
		}			
	}
		
	if(s_baidu_drv_all_thumb_file_array)
	{
		MMIAPIFILEARRAY_Destroy(&s_baidu_drv_all_thumb_file_array);
		s_baidu_drv_all_thumb_file_array = PNULL;	
	}

	if(s_baidu_drv_exist_thumb_file_array)
	{
		MMIAPIFILEARRAY_Destroy(&s_baidu_drv_exist_thumb_file_array);
		s_baidu_drv_exist_thumb_file_array = PNULL;
	}
       if(SFS_INVALID_HANDLE != s_meta_data_file_handle)
       {
	       MMIAPIFMM_CloseFile(s_meta_data_file_handle);
		s_meta_data_file_handle = SFS_INVALID_HANDLE ;
       }
	   
	if (0 != s_download_thumbnail_timer)
	{
	       MMK_StopTimer(s_download_thumbnail_timer);
		s_download_thumbnail_timer = 0;
    	}
       Clean_meta_data_info();
}

BOOLEAN MMISRV_BAIDUDRV_Check_Device_Space(uint32 writelen)
{
	uint32 free_high = 0, free_low = 0;
	MMISRV_BAIDUDRV_MSG_T sig_data={0};
       MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;

	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	
	MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(file_dev), MMIAPIFMM_GetDevicePathLen(file_dev) , &free_high, &free_low);
	if((0 == free_high)&&(writelen > free_low))
	{
		sig_data.error_type = BAIDU_DRIVE_ERROR_NO_MEMORY;
		MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_ERROR_NOTIFY , &sig_data);
		SCI_TRACE_LOW("[baidu]MMISRV_BAIDUDRV_Check_Space error no space" );	
		return FALSE;
	}
	return TRUE;
	//SCI_TRACE_LOW("[baidu] high:0x%x, low:0x%x !!!", free_high, free_low);   
	//MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(file_dev), MMIAPIFMM_GetDevicePathLen(file_dev) , &free_high, &free_low);
	//SCI_TRACE_LOW("[baidu] 2 high:0x%x, low:0x%x !!!", free_high, free_low);  
}

