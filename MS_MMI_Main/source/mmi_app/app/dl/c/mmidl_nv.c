/*************************************************************************
 ** File Name:       mmidl_nv.c                                           *
 ** Author:           wancan.you                                          *
 ** Date:              07/28/2009                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 07/28/2009     wancan.you      Create.                              *
*************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmidl_nv.h"
#include "mmi_nv.h"

/*the length of mmidl nv*/
const uint16 dl_nv_len[] =
{
    sizeof(MMIDL_DOWNLOAD_MANAGER_T),
    sizeof(uint32),
    sizeof(MMIDL_DOWNLOAD_STATUS_E) * MMIDL_MAX_DOWNLOAD_TASK_NUM,
    sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM,
    sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM,
    sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM,
    sizeof(MMIDL_DOWNLOAD_METHOD_TYPE),
    sizeof(MMIDL_DOWNLOAD_PATH_TYPE),
    (MMIFILE_LONG_FULL_PATH_MAX_LEN + 1) * sizeof(wchar),
    (MMIFILE_LONG_FULL_PATH_MAX_LEN + 1) * sizeof(wchar),
    (MMIFILE_LONG_FULL_PATH_MAX_LEN + 1) * sizeof(wchar),
    (MMIFILE_LONG_FULL_PATH_MAX_LEN + 1) * sizeof(wchar),
};

#ifdef DL_SUPPORT
/*****************************************************************************/
// 	Description : register mmidl module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_DL, dl_nv_len, sizeof(dl_nv_len)/sizeof(uint16));
}

/*****************************************************************************/
// 	Description : Read task num of DownloadManager
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerTotalTaskNum(uint32 *task_num)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == task_num)
    {
        return;
    }

    MMINV_READ(MMINV_DL_MANAGER_TOTAL_TASK_NUM, task_num, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        *task_num = 0;
        MMIDL_WriteDownloadManagerTotalTaskNum(0);
    }
}

/*****************************************************************************/
// 	Description : Write task num of DownloadManager
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerTotalTaskNum(uint32 task_num)
{
    MMINV_WRITE(MMINV_DL_MANAGER_TOTAL_TASK_NUM, &task_num);
}

/*****************************************************************************/
// 	Description : Read Status of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoStatus(MMIDL_DOWNLOAD_STATUS_E *status)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if(PNULL != status)
    {
        MMINV_READ(MMINV_DL_MANAGER_INFO_STATUS, status, nv_value);
    
        if (MN_RETURN_SUCCESS != nv_value)
        {
            SCI_MEMSET(status, 0x00, (sizeof(MMIDL_DOWNLOAD_STATUS_E) * MMIDL_MAX_DOWNLOAD_TASK_NUM));
            MMIDL_WriteDownloadManagerInfoStatus(status);
        }
    }
}

/*****************************************************************************/
// 	Description : Write Status of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoStatus(MMIDL_DOWNLOAD_STATUS_E *status)
{
    if (PNULL != status)
    {
        MMINV_WRITE(MMINV_DL_MANAGER_INFO_STATUS, status);
    }
}

/*****************************************************************************/
// 	Description : Read current length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoCurrentLen(uint32 *current_len)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if(PNULL != current_len)
    {
        MMINV_READ(MMINV_DL_MANAGER_INFO_CURRWNT_LEN, current_len, nv_value);
    
        if (MN_RETURN_SUCCESS != nv_value)
        {
            SCI_MEMSET(current_len, 0x00, sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM);
            MMIDL_WriteDownloadManagerInfoCurrentLen(current_len);
        }
    }
}

/*****************************************************************************/
// 	Description : Write current length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoCurrentLen(uint32 *current_len)
{
    if (PNULL != current_len)
    {
        MMINV_WRITE(MMINV_DL_MANAGER_INFO_CURRWNT_LEN, current_len);
    }
}

/*****************************************************************************/
// 	Description : Read Total length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoTotalLen(uint32 *total_len)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if(PNULL != total_len)
    {
        MMINV_READ(MMINV_DL_MANAGER_INFO_TOTAL_LEN, total_len, nv_value);
    
        if (MN_RETURN_SUCCESS != nv_value)
        {
            SCI_MEMSET(total_len, 0x00, sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM);
            MMIDL_WriteDownloadManagerInfoTotalLen(total_len);
        }
    }
}

/*****************************************************************************/
// 	Description : Write Total length of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoTotalLen(uint32 *total_len)
{
    if (PNULL != total_len)
    {
        MMINV_WRITE(MMINV_DL_MANAGER_INFO_TOTAL_LEN, total_len);
    }
}

/*****************************************************************************/
// 	Description : Read Mime type of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManagerInfoMimeType(uint32 *mime_type)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if(PNULL != mime_type)
    {
        MMINV_READ(MMINV_DL_MANAGER_INFO_MIME_TYPE, mime_type, nv_value);
    
        if (MN_RETURN_SUCCESS != nv_value)
        {
            SCI_MEMSET(mime_type, 0x00, sizeof(uint32) * MMIDL_MAX_DOWNLOAD_TASK_NUM);
            MMIDL_WriteDownloadManagerInfoMimeType(mime_type);
        }
    }
}

/*****************************************************************************/
// 	Description : Write Mime type of DownloadManager Info
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManagerInfoMimeType(uint32 *mime_type)
{
    if (PNULL != mime_type)
    {
        MMINV_WRITE(MMINV_DL_MANAGER_INFO_MIME_TYPE, mime_type);
    }
}

/*****************************************************************************/
// 	Description : Read Download Manager
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *manager_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    uint32 total_num = 0;
    uint32 current_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    uint32 total_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    uint32 mime_type[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    MMIDL_DOWNLOAD_STATUS_E dl_status[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    int32 i= 0;

    if (PNULL == manager_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_DL_MANAGER, manager_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;

        dl_manager = SCI_ALLOCA(sizeof(MMIDL_DOWNLOAD_MANAGER_T));
        if (PNULL == dl_manager)
        {
            return;
        }
        SCI_MEMSET(dl_manager, 0x00, sizeof(MMIDL_DOWNLOAD_MANAGER_T));

        MMIDL_WriteDownloadManager(dl_manager);

        SCI_FREE(dl_manager);
    }

    /* Real value */
    MMIDL_ReadDownloadManagerTotalTaskNum(&total_num);
    MMIDL_ReadDownloadManagerInfoStatus(dl_status);
    MMIDL_ReadDownloadManagerInfoCurrentLen(current_len);
    MMIDL_ReadDownloadManagerInfoTotalLen(total_len);
    MMIDL_ReadDownloadManagerInfoMimeType(mime_type);

    manager_ptr->total_task_num = total_num;
    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        manager_ptr->task_info[i].cur_len = current_len[i];
        manager_ptr->task_info[i].total_len = total_len[i];
        manager_ptr->task_info[i].status = dl_status[i];
        manager_ptr->task_info[i].mime_type = mime_type[i];
    }
}

/*****************************************************************************/
// 	Description :  Write Download Manager
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *manager_ptr)
{
    uint32 current_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    uint32 total_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    uint32 mime_type[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    MMIDL_DOWNLOAD_STATUS_E dl_status[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};
    int32 i= 0;

    if (PNULL == manager_ptr)
    {
        return;
    }

    MMINV_WRITE(MMINV_DL_MANAGER, manager_ptr);
    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        SCI_TRACE_LOW("[mmidl_nv.c][MMIDL_WriteDownloadManager] %d.status = %d",i,manager_ptr->task_info[i].status);
        current_len[i] = manager_ptr->task_info[i].cur_len;
        total_len[i] = manager_ptr->task_info[i].total_len;
        dl_status[i] = manager_ptr->task_info[i].status;
        mime_type[i] = manager_ptr->task_info[i].mime_type;
    }
    MMIDL_WriteDownloadManagerTotalTaskNum(manager_ptr->total_task_num);
    MMIDL_WriteDownloadManagerInfoStatus(dl_status);
    MMIDL_WriteDownloadManagerInfoCurrentLen(current_len);
    MMIDL_WriteDownloadManagerInfoTotalLen(total_len);
    MMIDL_WriteDownloadManagerInfoMimeType(mime_type);
}

/*****************************************************************************/
// 	Description : Read Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadMethodType(MMIDL_DOWNLOAD_METHOD_TYPE *method_type)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == method_type)
    {
        return;
    }

    MMINV_READ(MMINV_DL_METHOD_TYPE, method_type, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        *method_type = MMIDL_DOWNLOAD_METHOD_QUEUE;
        MMIDL_WriteDownloadMethodType(MMIDL_DOWNLOAD_METHOD_QUEUE);
    }
}

/*****************************************************************************/
// 	Description : Write Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadMethodType(MMIDL_DOWNLOAD_METHOD_TYPE method_type)
{
    MMINV_WRITE(MMINV_DL_METHOD_TYPE, &method_type);
}

/*****************************************************************************/
// 	Description : Read Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadPathType(MMIDL_DOWNLOAD_PATH_TYPE *path_type)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_type)
    {
        return;
    }

    MMINV_READ(MMINV_DL_PATH_TYPE, path_type, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        *path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;
        MMIDL_WriteDownloadPathType(MMIDL_DOWNLOAD_PATH_DEFAULT);
    }
}

/*****************************************************************************/
// 	Description : Write Download Method type
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadPathType(MMIDL_DOWNLOAD_PATH_TYPE method_type)
{
    MMINV_WRITE(MMINV_DL_PATH_TYPE, &method_type);
}

/*****************************************************************************/
// 	Description : Read Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadPicturePath(wchar *path_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_DL_PICTURE_PATH, path_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;

        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, path_ptr, &path_len);
        MMIDL_WriteDownloadPicturePath(path_ptr);
    }
}

/*****************************************************************************/
// 	Description : Write Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadPicturePath(wchar *path_ptr)
{
    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_WRITE(MMINV_DL_PICTURE_PATH, path_ptr);
}

/*****************************************************************************/
// 	Description : Read Download Music path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadMusicPath(wchar *path_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_DL_MUSIC_PATH, path_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;

        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_AUDIO_TYPE, path_ptr, &path_len);
        MMIDL_WriteDownloadMusicPath(path_ptr);
    }
}

/*****************************************************************************/
// 	Description : Write Download music path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadMusicPath(wchar *path_ptr)
{
    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_WRITE(MMINV_DL_MUSIC_PATH, path_ptr);
}

/*****************************************************************************/
// 	Description : Read Download Video path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadVideoPath(wchar *path_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_DL_VIDEO_PATH, path_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;

        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_VIDEO_TYPE, path_ptr, &path_len);
        MMIDL_WriteDownloadVideoPath(path_ptr);
    }
}

/*****************************************************************************/
// 	Description : Write Download Picture path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadVideoPath(wchar *path_ptr)
{
    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_WRITE(MMINV_DL_VIDEO_PATH, path_ptr);
}

/*****************************************************************************/
// 	Description : Read Download Other path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReadDownloadOthersPath(wchar *path_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_DL_OTHERS_PATH, path_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;

        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_OTHER_TYPE, path_ptr, &path_len);
        MMIDL_WriteDownloadOthersPath(path_ptr);
    }
}

/*****************************************************************************/
// 	Description : Write Download Other path
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_WriteDownloadOthersPath(wchar *path_ptr)
{
    if (PNULL == path_ptr)
    {
        return;
    }

    MMINV_WRITE(MMINV_DL_OTHERS_PATH, path_ptr);
}

#endif //#ifdef DL_SUPPORT
