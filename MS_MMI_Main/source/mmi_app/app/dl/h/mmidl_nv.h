/*************************************************************************
 ** File Name:      mmidl_nv.h                                        *
 ** Author:          wancan.you                                         *
 ** Date:             2009/07/28                                          *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/28     wancan.you      Create.                              *
*************************************************************************/
#ifndef _MMIDL_NV_H_
#define _MMIDL_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmidl_internal.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_DL_MANAGER = MMI_MODULE_DL << 16,
    MMINV_DL_MANAGER_TOTAL_TASK_NUM,
    MMINV_DL_MANAGER_INFO_STATUS,
    MMINV_DL_MANAGER_INFO_CURRWNT_LEN,
    MMINV_DL_MANAGER_INFO_TOTAL_LEN,
    MMINV_DL_MANAGER_INFO_MIME_TYPE,
    MMINV_DL_METHOD_TYPE,
    MMINV_DL_PATH_TYPE,
    MMINV_DL_PICTURE_PATH,
    MMINV_DL_MUSIC_PATH,
    MMINV_DL_VIDEO_PATH,
    MMINV_DL_OTHERS_PATH,

    MMINV_DL_MAX_ITEM_NUM
}DL_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register mmidl module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_RegNv(void);

/*****************************************************************************/
// 	Description : Read task num of DownloadManager
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerTotalTaskNum(uint32 *task_num);

/*****************************************************************************/
// 	Description : Write task num of DownloadManager
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerTotalTaskNum(uint32 task_num);

/*****************************************************************************/
// 	Description : Read Status of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoStatus(MMIDL_DOWNLOAD_STATUS_E *status);

/*****************************************************************************/
// 	Description : Write Status of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoStatus(MMIDL_DOWNLOAD_STATUS_E *status);

/*****************************************************************************/
// 	Description : Read current length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoCurrentLen(uint32 *current_len);

/*****************************************************************************/
// 	Description : Write current length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoCurrentLen(uint32 *current_len);

/*****************************************************************************/
// 	Description : Read Total length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoTotalLen(uint32 *total_len);

/*****************************************************************************/
// 	Description : Write Total length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoTotalLen(uint32 *total_len);

/*****************************************************************************/
// 	Description : Read Mime type of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoMimeType(uint32 *mime_type);

/*****************************************************************************/
// 	Description : Write Mime type of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoMimeType(uint32 *mime_type);

/*****************************************************************************/
// 	Description : Read Download Manager
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *manager_ptr);

/*****************************************************************************/
// 	Description :  Write Download Manager
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *manager_ptr);

/*****************************************************************************/
// 	Description : Read Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadMethodType(MMIDL_DOWNLOAD_METHOD_TYPE *method_type);

/*****************************************************************************/
// 	Description : Write Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadMethodType(MMIDL_DOWNLOAD_METHOD_TYPE method_type);

/*****************************************************************************/
// 	Description : Read Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadPathType(MMIDL_DOWNLOAD_PATH_TYPE *method_type);

/*****************************************************************************/
// 	Description : Write Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadPathType(MMIDL_DOWNLOAD_PATH_TYPE method_type);

/*****************************************************************************/
// 	Description : Read Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadPicturePath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Write Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadPicturePath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Read Download Music path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadMusicPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Write Download music path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadMusicPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Read Download Video path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadVideoPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Write Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadVideoPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Read Download Other path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadOthersPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Write Download Other path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadOthersPath(wchar *path_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDL_NV_H_

