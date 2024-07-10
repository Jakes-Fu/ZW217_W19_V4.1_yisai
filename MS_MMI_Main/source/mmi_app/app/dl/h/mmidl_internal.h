/***************************************************************************
** File Name:      mmidl_internal.h                                                                               *
** Author:                                                                                                                    *
** Date:           28/09/2009                                                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                             *
** Description:    This file is used to download internal define                                      *
****************************************************************************
**                         Important Edit History                                                                       *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                  *
** 07/2009        wancan.you    Create
****************************************************************************/
#ifndef  _MMIDL_INTERNAL_H_    
#define  _MMIDL_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "http_api.h"
#include "mmi_module.h"
#include "mmidl_export.h"
#include "mmifmm_export.h"
#include "mn_type.h"
#include "mime_type.h"
#include "mmiconnection_export.h"
#include "mmifilearray_export.h"
#include "odm_api.h"
#include "guirichtext.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIDL_INVALID_GATEWAY              "0.0.0.0"
#define MMIDL_DOWNLOAD_PDP_SUPPORT
//#define MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
//#define MMIDL_MMI_BUFFER_SUPPORT

#define MMIDL_CAFEAPPLET(download_applet) ((download_applet)->caf_applet)
#define MMIDL_HANDLE(download_applet) ((MMIDL_CAFEAPPLET(download_applet)).app_handle)

#define MMIDL_APPLETINSTANCE MMIDL_GetAppletInstance
#define MMIDL_INSTANCE MMIDL_GetDownloadInstance

#define MMIDL_MODULE_ID MMI_MODULE_DL

#define MMIDL_MAX_MULTI_DL_TASK_NUM   3
#define MMIDL_QUEUE_DL_TASK_NUM   1

#ifdef MMIDL_MMI_BUFFER_SUPPORT
#define MMIDL_MAX_DL_BUF_SIZE   (30 * 1024)
#endif

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
#define MMIDL_NETCONNECT() MMIDL_NetConnect(MMIDL_INSTANCE())
#define MMIDL_NETDISCONNECT() MMIDL_NetDisconnect(MMIDL_INSTANCE())
#endif

#define MMIDL_TMPFILE_EXT   ".tmp"

#define MMIDL_VIBRA_TIMEROUT   (3000)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIDL_PDP_NONE,
    MMIDL_PDP_ACTIVING,
    MMIDL_PDP_ACTIVE_OK,        
    MMIDL_PDP_ACTIVE_FAILED,
    MMIDL_PDP_DEACTIVING,
    MMIDL_PDP_DEACTIVE_FAILED,
    MMIDL_PDP_DEACTIVE_OK = MMIDL_PDP_NONE,
}MMIDL_PDP_STATE_E;

typedef enum
{
    MMIDL_DOWNLOAD_METHOD_APPOSITE,
    MMIDL_DOWNLOAD_METHOD_QUEUE,
    MMIDL_DOWNLOAD_METHOD_TYPE_MAX
}MMIDL_DOWNLOAD_METHOD_TYPE;

typedef enum
{
    MMIDL_DOWNLOAD_PATH_DEFAULT,
    MMIDL_DOWNLOAD_PATH_CUSTOM,
    MMIDL_DOWNLOAD_PATH_TYPE_MAX
}MMIDL_DOWNLOAD_PATH_TYPE;

typedef enum
{
    MMIDL_DOWNLOAD_NONE,
    MMIDL_DOWNLOAD_DOWNLOADING,
    MMIDL_DOWNLOAD_DOWNLOADED,
    MMIDL_DOWNLOAD_FAILED,
    MMIDL_DOWNLOAD_WAITING,
    MMIDL_DOWNLOAD_WAITING_HTTP,
    MMIDL_DOWNLOAD_PAUSED,
    MMIDL_DOWNLOAD_STATUS_MAX
}MMIDL_DOWNLOAD_STATUS_E;

typedef struct _MMIDL_PDP_INFO_T_
{
    MMICONNECTION_LINKSETTING_DETAIL_T connect_setting;
    MMIDL_PDP_STATE_E pdp_state;
    MMIPDP_INTERFACE_E bearer_type;
    MN_DUAL_SYS_E dual_sys;
    uint32 net_id;
}MMIDL_PDP_INFO_T;

typedef struct _MMIDL_DOWNLOAD_TASK_T_
{
    HTTP_CONTEXT_ID_T context_id;/* session ID In case of OMA DL */
    HTTP_REQUEST_ID_T request_id;
    uint32 app_instance;
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_APP_PARAM_T app_param;
    MMIDL_DOWNLOAD_INFO_T download_info;
    MIME_TYPE_E mime_type;
    uint32 total_len;
    uint32 cur_len;
#ifdef MMIDL_MMI_BUFFER_SUPPORT
    uint8* data_buf;
    uint32 buf_len;
    uint32 max_buf_len;
#endif
    int32 manager_index;
    BOOLEAN is_old_url;
    BOOLEAN is_downloading;
    BOOLEAN is_used;
    BOOLEAN is_oma_dl;
    BOOLEAN is_need_reload;
    uint32 session_id;
    MN_DUAL_SYS_E dual_sys;
    
}MMIDL_DOWNLOAD_TASK_T;

typedef struct _MMIDL_DOWNLOAD_MANAGER_INFO_T_
{
    uint8 url[MMIDL_URL_MAX + 1];
    wchar file[MMIFILE_FULL_PATH_MAX_LEN + 1];
    MMIDL_DOWNLOAD_STATUS_E status;
    uint32 cur_len;
    uint32 total_len;
    uint32 mime_type;
    MMIDL_DOWNLOAD_RESULT_E result;
}MMIDL_DOWNLOAD_MANAGER_INFO_T;

typedef struct _MMIDL_DOWNLOAD_MANAGER_T_
{
    uint32 total_task_num;
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info[MMIDL_MAX_DOWNLOAD_TASK_NUM];
}MMIDL_DOWNLOAD_MANAGER_T;

typedef struct _MMIDL_DOWNLOAD_INSTANCE_T_
{
    uint32 multi_task_num;
    uint32 download_task_num;
    uint32 download_manager_task_index[MMIDL_MAX_DOWNLOAD_TASK_NUM];
    MMIDL_DOWNLOAD_TASK_T download_task[MMIDL_MAX_DOWNLOAD_TASK_NUM];
    uint32 http_waiting_task_id_num;
    int32 http_waiting_task_id[MMIDL_MAX_DOWNLOAD_TASK_NUM];
    MMIDL_PDP_INFO_T pdp_info;
    wchar save_path[MMIFILE_FULL_PATH_MAX_LEN];
    uint32 save_path_len;
    MMIDL_DOWNLOAD_MANAGER_T * downoad_manager;
    BOOLEAN is_freq_change;
}MMIDL_DOWNLOAD_INSTANCE_T;

typedef struct DOWNLOAD_APPLET_T_
{
    CAF_APPLET_T caf_applet;
    MMIDL_DOWNLOAD_INSTANCE_T download_instance;
}DOWNLOAD_APPLET_T;

typedef struct MMIDL_OMA_DD_INFO_T_
{
    _SIGNAL_VARS
    uint32 task_id;
    ODM_DOWNLOAD_DESCRIPTION_T dd_content;
}MMIDL_OMA_DD_INFO_T;

typedef enum
{
    MMIDL_CHARSET_NONE,
    MMIDL_CHARSET_UTF8,
    MMIDL_CHARSET_GB2312,
}MMIDL_CHARSET_T;

typedef void (*MMIDL_OpenFileCallBack)(BOOLEAN is_success, FILEARRAY file_array);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get Download Applet Instance
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC DOWNLOAD_APPLET_T *MMIDL_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Get Download Instance
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_INSTANCE_T *MMIDL_GetDownloadInstance(void);

/*****************************************************************************/
//  Description : Delete Download Task
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DeleteDownloadTask(uint32 task_index);

/*****************************************************************************/
//  Description : Create New Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_StartHttpRequest(uint32 task_id);

/*****************************************************************************/
//  Description : Auth Response for Task id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E MMIDL_AuthResponse(uint32 task_id, MMI_STRING_T *account_str_ptr, MMI_STRING_T *password_str_ptr);

/*****************************************************************************/
//  Description : split filename
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_SplitFileName(
                                                                                     wchar      *full_file_name,
                                                                                     wchar      *file_name,
                                                                                     uint16     file_name_maxlen,
                                                                                     wchar      *file_ext,
                                                                                     uint16     file_ext_maxlen
                                                                                     );

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_CombineDownloadFile(
                                                                                     MMI_STRING_T *path_ptr,
                                                                                     MMI_STRING_T *name_ptr,
                                                                                     wchar      *full_path_ptr,     //out
                                                                                     uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                                                                     BOOLEAN is_combine_tmp_file
                                                                                     );

/*****************************************************************************/
//  Description : Get file type by content
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MIME_TYPE_E MMIDL_GetDownloadFileType(const char * content_type_ptr);

/*****************************************************************************/
//  Description : Open Download Authorize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadAuthWin(uint32 task_id);

/*****************************************************************************/
//  Description : Open Download Authorize Input
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadAuthInputWin(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : Open Download Save Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSaveWin(uint32 task_id);

/*****************************************************************************/
//  Description : Open Download Save As Win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSaveAsWin(MMIDL_DOWNLOAD_INFO_T *download_info_ptr);

/*****************************************************************************/
//  Description : Open Download Input filename
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenInputFileNameWin(MMI_HANDLE_T label_handle);

/*****************************************************************************/
//  Description : Open Download Input filename
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenInputFileNameWinEx(MMI_HANDLE_T applet_handle, MMI_HANDLE_T label_handle);

/*****************************************************************************/
//  Description : Open Download Manager
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadManagerWin(void);

/*****************************************************************************/
//  Description : Open Download Manager Option
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadManagerOptionWin(uint32 task_index);

/*****************************************************************************/
//  Description : Open Download Task Detail
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadTaskDetailWin(uint32 task_index);

/*****************************************************************************/
//  Description : Open Download Query
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadQueryWin(uint32 task_index);

/*****************************************************************************/
//  Description : Open Download Settings
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSettingsWin(void);

/*****************************************************************************/
//  Description : Init Download task status
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_InitDownloadTaskStatus(void);

/*****************************************************************************/
//  Description : Init default download folder
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_InitDownloadFolder(void);

/*****************************************************************************/
//  Description : save download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDL_SaveDownloadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint8 *url_ptr, wchar *file_ptr, uint32 mime_type);

/*****************************************************************************/
//  Description : Destory Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DestoryDownloadTask(uint32 task_id, MMIDL_DOWNLOAD_INSTANCE_T *download_instance);

/*****************************************************************************/
//  Description : Delete download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DeleteDownloadManagerTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, BOOLEAN is_all);

/*****************************************************************************/
//  Description : change download task status
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskStatus(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, MMIDL_DOWNLOAD_STATUS_E status);

/*****************************************************************************/
//  Description : change download task file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskFileName(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index);

/*****************************************************************************/
//  Description : Get file name without .temp
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetFileNameWithoutTemp(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index, wchar *file_name, uint32 name_len);

/*****************************************************************************/
//  Description : change download task file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeFileNameToTmpFile(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index);

/*****************************************************************************/
//  Description : change download task total len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskTotalLen(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 total_len);

/*****************************************************************************/
//  Description : change download task current len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskCurLen(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 cur_len);

/*****************************************************************************/
//  Description : get total task num
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetDownloadManagerTotalTaskNum(MMIDL_DOWNLOAD_MANAGER_T *dl_manager);

/*****************************************************************************/
//  Description : Judge whether is same url in download manager
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_IsSameDownloadDestUrl(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, char *url, uint32 *same_task_index);

/*****************************************************************************/
//  Description : get unused Manager task id
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetUnUnusedManagerTaskId(MMIDL_DOWNLOAD_MANAGER_T *dl_manager);

/*****************************************************************************/
//  Description : get unused Manager task Info by Index
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetManagerTaskInfo(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, MMIDL_DOWNLOAD_MANAGER_INFO_T *info_ptr);

/*****************************************************************************/
//  Description : append DL Task Item 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppendDLTaskListItem(MMI_WIN_ID_T win_id, MMI_HANDLE_T ctrl_handle);

/*****************************************************************************/
//  Description : Set DL Task Focus Item 
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DLTaskListSetFocusItem(uint16 focus_item);

/*****************************************************************************/
//  Description : add Task Detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppendDLTaskDetail(MMI_WIN_ID_T win_id, MMI_HANDLE_T ctrl_handle, uint32 task_index);

/*****************************************************************************/
// Description : update download manager Win
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIDL_UpdateDownloadManagerWin(void);

/*****************************************************************************/
// Description : Get manager win handle
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIDL_GetDownloadManagerWinHandle(void);

/*****************************************************************************/
//  Description : Set download manager task index
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadManagerTaskIndex(uint32 index, uint32 task_index);

/*****************************************************************************/
//  Description : get download manager task index
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetDownloadManagerTaskIndex(uint32 index);

/*****************************************************************************/
//  Description : Pause Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_PauseDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index);

/*****************************************************************************/
//  Description : Resume Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ResumeDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index);

/*****************************************************************************/
//  Description : Re Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index);

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownload(uint32 task_id, BOOLEAN is_all);

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownloadApplet(void);

/*****************************************************************************/
//  Description : Check Next Download Task
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_CheckNextDownloadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager);

/*****************************************************************************/
//  Description : Do task need to download  
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_IsNeedDownLoadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager);

/*****************************************************************************/
//  Description : Open File Explorer
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenFileExplorer(uint32 task_index);

/*****************************************************************************/
// Description : Get Default File Name
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDefaultFileName(wchar *file_name, uint32 name_len, const uint8 * url_ptr, const uint8 * http_header_ptr, uint32 mime_type);

/*****************************************************************************/
//  Description : Get Download type
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_TYPE MMIDL_GetDownloadType(uint32 mime_type, BOOLEAN is_nf_type);

/*****************************************************************************/
//  Description : Get display path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetDisplayPathText(MMI_STRING_T *path_value_str_ptr,
                                                                                wchar *path_name_ptr,
                                                                                uint16 *path_name_len_ptr);

/*****************************************************************************/
//  Description : Get Real path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetRealPathText(MMI_STRING_T *path_value_str_ptr,
                                                                                wchar *path_name_ptr,
                                                                                uint16 *path_name_len_ptr);

/*****************************************************************************/
//  Description : Appoint download save path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppointDownloadSavePath(wchar *path_ptr, uint32 path_len, MMIDL_OpenFileCallBack callback);

/*****************************************************************************/
//  Description : Notify download complete
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_NotifyDownloadComplete(uint32 index, BOOLEAN is_success);

/*****************************************************************************/
//  Description : Create Download File
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIDL_CreateDownloadFile(MMIDL_DOWNLOAD_TASK_T *download_task_ptr, BOOLEAN *is_file_exist);

/*****************************************************************************/
//  Description : Get download Status
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDownloadStatus(void);

/*****************************************************************************/
//  Description : Set Multi Task num
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetMultiTaskNum(void);

/*****************************************************************************/
//  Description : Get Multi Task num
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetMultiTaskNum(void);

/*****************************************************************************/
//  Description : Get Download Manager
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_MANAGER_T * MMIDL_GetDownloadManager(void);

/*****************************************************************************/
//  Description : Set Download Manager
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *downoad_manager );

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetPDPInfo(MMIDL_PDP_INFO_T *pdp_info);

/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetPDPInfo(MMIDL_PDP_INFO_T *pdp_info);

/*****************************************************************************/
//  Description : Get PDP info of settings
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetSettingsPDPInfo(MMIDL_PDP_INFO_T *pdp_info, MN_DUAL_SYS_E e_dual_sys);

/*****************************************************************************/
//  Description : set PDP state
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetPDPState(MMIDL_DOWNLOAD_INSTANCE_T *self, MMIDL_PDP_STATE_E in_state);

/*****************************************************************************/
//  Description : Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int MMIDL_NetConnect(MMIDL_DOWNLOAD_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_NetDisconnect(MMIDL_DOWNLOAD_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Check Is Network Ready
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_NetworkIsReady(MMIDL_DOWNLOAD_INSTANCE_T *self);
#endif

/*****************************************************************************/
//  Description : reset download setting to default
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_ResetDefaultSettings(void);

/*****************************************************************************/
//  Description : get download path
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDownloadPath(
                                            MMIDL_DOWNLOAD_TYPE type,
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            );

/*****************************************************************************/
//  Description : get download path assigned by user
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetCustomizedDownloadPath(
                                            MMIDL_DOWNLOAD_TYPE type,
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            );

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDefaultDownloadPath(
                                            MMIDL_DOWNLOAD_TYPE type,
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            );

/*****************************************************************************/
// Description : GetMimeTypeByhttpHeader
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetMimeTypeByhttpHeader(const uint8 * http_header_ptr);

/*****************************************************************************/
//  Description : change download task total len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskMimeType(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 mime_type);

/*****************************************************************************/
//  Description : Stop download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownloadManagerTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, BOOLEAN is_all);

/*****************************************************************************/
//  Description : Tidy all windows of DL
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_TidyExistedWin(void);

/*****************************************************************************/
//  Description : Start OMA Download
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_StartOMADownload(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, uint32 task_id);

/*****************************************************************************/
//  Description : Start OMA Download by DD
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StartOMADownloadByDD(uint8 * dd_buffer);

/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_RichText_AddItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T * item_string, GUIRICHTEXT_FRAME_TYPE_E frame_type, GUIRICHTEXT_TAG_TYPE_E tag_type);


/*****************************************************************************/
//  Description : handle download callback
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_MimeCallBack(MMIDL_MIME_CALLBACK_PARAM_T* param_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenPDPChangeQueryWin(void);


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenPDPDeactiveQueryWin(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_HandlePDP_Change(void);
/*added for cr240847 begin*/
/*****************************************************************************/
//  Description : set download task's result
//  Global resource dependence : 
//  Author: rui.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadResult(uint32 index, MMIDL_DOWNLOAD_RESULT_E result);

/*****************************************************************************/
//  Description : get download task's result by index
//  Global resource dependence : 
//  Author: rui.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_RESULT_E MMIDL_GetDownloadResult(uint32 index);

/*****************************************************************************/
//  Description : Create page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDDPropertyWin(MMIDL_OMA_DD_INFO_T *dd_info_ptr);

/*added for cr240847 end*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


