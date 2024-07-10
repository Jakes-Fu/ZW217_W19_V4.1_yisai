/******************************************************************************
** File Name:      mmidl_func.c                                              *
** Author:                                                                    *
** Date:           2009/07/28                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe download func             *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 07/2009        wancan.you           Create                                     *
******************************************************************************/
#define _MMIDL_FUC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmi_text.h"
#include "mmidl_internal.h"
#include "mmidl_nv.h"
#include "mmidl_image.h"
#include "mmidl_text.h"
#include "mmidl_id.h"
#include "mmi_resource.h"
#include "guirichtext.h"
#include "gui_ucs2b_converter.h"
#include "mmifmm_export.h"
#include "sfs.h"
#include "mmipub.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#endif
#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#ifdef DL_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIDL_ONE_KBYTE 1024

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get Download Display Filename
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDownloadDisplayFilename(wchar *full_path, wchar *file_name, uint16 name_len);

/*****************************************************************************/
//  Description : add item to listbox for download mdu 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cr137619liyan modified
/*****************************************************************************/
LOCAL void AppendDLListItemData(MMI_HANDLE_T ctrl_handle, MMIDL_DOWNLOAD_MANAGER_INFO_T *task_info_ptr,uint32 *j_ptr, uint32 i );

/*****************************************************************************/
//  Description : Get download file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL wchar *GetDownloadFileName(const wchar *file_name);

/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DEFINITION                       *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Init Download task status
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_InitDownloadTaskStatus(void)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    uint32 i = 0;

    dl_manager = SCI_ALLOCA(sizeof(MMIDL_DOWNLOAD_MANAGER_T));

    if (PNULL == dl_manager)
    {
        return;
    }
    SCI_MEMSET(dl_manager, 0x00, sizeof(MMIDL_DOWNLOAD_MANAGER_T));
    MMIDL_ReadDownloadManager(dl_manager);

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
#ifdef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
        if (MMIDL_DOWNLOAD_DOWNLOADING == dl_manager->task_info[i].status
            || MMIDL_DOWNLOAD_WAITING == dl_manager->task_info[i].status
            || MMIDL_DOWNLOAD_WAITING_HTTP == dl_manager->task_info[i].status)
        {
            MMIDL_ChangeDownloadTaskStatus(dl_manager, i, MMIDL_DOWNLOAD_PAUSED);
        }
#else
        if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[i].status
            && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[i].status)
        {
            MMIAPIFMM_DeleteFile(dl_manager->task_info[i].file, PNULL);
            MMIDL_DeleteDownloadManagerTask(dl_manager, i, FALSE);
        }
#endif
    }

    SCI_FREE(dl_manager);
}

/*****************************************************************************/
//  Description : Get download Status
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDownloadStatus(void)
{
    BOOLEAN is_download = FALSE;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    uint32 i = 0;

    dl_manager = MMIDL_GetDownloadManager();
    if (PNULL != dl_manager)
    {
        for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
        {
            if (MMIDL_DOWNLOAD_DOWNLOADING == dl_manager->task_info[i].status
                || MMIDL_DOWNLOAD_WAITING == dl_manager->task_info[i].status
                || MMIDL_DOWNLOAD_WAITING_HTTP == dl_manager->task_info[i].status)
            {
                is_download = TRUE;
                break;
            }
        }
    }

    return is_download;
}

/*****************************************************************************/
//  Description : Check Next Download Task
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_CheckNextDownloadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager)
{
    uint32 i = 0;

    if (0 == dl_manager->total_task_num)
    {
        return;
    }

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (MMIDL_DOWNLOAD_WAITING == dl_manager->task_info[i].status)
        {
            MMIDL_ResumeDownload(dl_manager, i);
            break;
        }
    }

}

/*****************************************************************************/
//  Description : Do task need to download  
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_IsNeedDownLoadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager)
{
    uint32 i = 0;

    if (0 == dl_manager->total_task_num)
    {
        return FALSE;
    }

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (MMIDL_DOWNLOAD_DOWNLOADING == dl_manager->task_info[i].status
            || MMIDL_DOWNLOAD_WAITING == dl_manager->task_info[i].status
            || MMIDL_DOWNLOAD_WAITING_HTTP == dl_manager->task_info[i].status)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : save download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDL_SaveDownloadTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager ,uint8 *url_ptr, wchar *file_ptr, uint32 mime_type)
{
    uint32 i = 0;
    uint16 url_str_len = 0;
    
    if (PNULL == url_ptr || PNULL == file_ptr)
    {
        return -1;
    }

    if (MMIDL_MAX_DOWNLOAD_TASK_NUM <= dl_manager->total_task_num)
    {
        return -1;
    }

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (MMIDL_DOWNLOAD_NONE == dl_manager->task_info[i].status)
        {
            break;
        }
    }

    if (MMIDL_MAX_DOWNLOAD_TASK_NUM == i)
    {
        return -1;
    }
    url_str_len = SCI_STRLEN((char *)url_ptr);
    SCI_MEMSET(dl_manager->task_info[i].url, 0, sizeof(dl_manager->task_info[i].url));
    SCI_MEMCPY(dl_manager->task_info[i].url, url_ptr, MIN(url_str_len, MMIDL_URL_MAX));

    url_str_len = MMIAPICOM_Wstrlen(file_ptr);
    SCI_MEMSET(dl_manager->task_info[i].file, 0, sizeof(dl_manager->task_info[i].file));
    MMI_WSTRNCPY(dl_manager->task_info[i].file,
                            MMIFILE_FULL_PATH_MAX_LEN,
                            file_ptr,
                            MMIAPICOM_Wstrlen(file_ptr),
                            MIN(url_str_len, MMIFILE_FULL_PATH_MAX_LEN)
                            );

    dl_manager->task_info[i].cur_len = 0;
    dl_manager->task_info[i].status = MMIDL_DOWNLOAD_WAITING;
    dl_manager->total_task_num++;
    dl_manager->task_info[i].mime_type = mime_type;
    MMIDL_WriteDownloadManager(dl_manager);

    return i;
}

/*****************************************************************************/
//  Description : Delete download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DeleteDownloadManagerTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index, BOOLEAN is_all)
{
    uint32 i = 0;
    MMIFILE_ERROR_E result = SFS_NO_ERROR;
    uint32 filename_len = 0;

    //SCI_TRACE_LOW:"MMIDL_DeleteDownloadManagerTask index=%d, is_all=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_266_112_2_18_2_13_32_135,(uint8*)"dd", index, is_all);
    if (PNULL == dl_manager)
    {
        return;
    }

    if (is_all)
    {
        for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
        {
            SCI_TRACE_LOW("[mmidl_func.c][MMIDL_DeleteDownloadManagerTask]1 dl_manager->task_info[%d].status = %d",i,dl_manager->task_info[i].status);
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
            if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[i].status
                && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[i].status)
#else
            if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[i].status
                && MMIDL_DOWNLOAD_PAUSED != dl_manager->task_info[i].status
                && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[i].status)
#endif
            {
                filename_len = MMIAPICOM_Wstrlen(dl_manager->task_info[i].file);
                if(MMIAPIFMM_IsFileExist((const wchar *)dl_manager->task_info[i].file, filename_len))
                {
                    result = MMIAPIFMM_DeleteFile((const wchar *)dl_manager->task_info[i].file, PNULL);
                }
            }
//#endif

            if(SFS_NO_ERROR == result)
            {
                SCI_MEMSET(dl_manager->task_info[i].url, 0x00, sizeof(dl_manager->task_info[i].url));
                SCI_MEMSET(dl_manager->task_info[i].file, 0x00, sizeof(dl_manager->task_info[i].file));
                dl_manager->task_info[i].cur_len = 0;
                dl_manager->task_info[i].total_len = 0;
                dl_manager->task_info[i].status = MMIDL_DOWNLOAD_NONE;

                if (dl_manager->total_task_num > 0)
                {
                    dl_manager->total_task_num--;
                }
            }

            result = SFS_NO_ERROR;
        }

        //dl_manager->total_task_num = 0;
    }
    else
    {
        if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
        {
            return;
        }
        SCI_TRACE_LOW("[mmidl_func.c][MMIDL_DeleteDownloadManagerTask]2 dl_manager->task_info[%d].status = %d",i,dl_manager->task_info[i].status);
        if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[index].status)
        {
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
            if (MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[index].status)
#else
            if (MMIDL_DOWNLOAD_PAUSED != dl_manager->task_info[index].status
                && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[index].status)
#endif
            {
                filename_len = MMIAPICOM_Wstrlen(dl_manager->task_info[index].file);
                if(MMIAPIFMM_IsFileExist((const wchar *)dl_manager->task_info[index].file, filename_len))
                {
                    result = MMIAPIFMM_DeleteFile(dl_manager->task_info[index].file, PNULL);
                }
            }
//#endif

            if(SFS_NO_ERROR == result)
            {
                SCI_MEMSET(dl_manager->task_info[index].url, 0x00, sizeof(dl_manager->task_info[index].url));
                SCI_MEMSET(dl_manager->task_info[index].file, 0x00, sizeof(dl_manager->task_info[index].file));
                dl_manager->task_info[index].cur_len = 0;
                dl_manager->task_info[index].total_len = 0;
                dl_manager->task_info[index].status = MMIDL_DOWNLOAD_NONE;
                
                if (dl_manager->total_task_num > 0)
                {
                    dl_manager->total_task_num--;
                }
            }
        }
    }

    MMIDL_WriteDownloadManager(dl_manager);

}

/*****************************************************************************/
//  Description : Stop download task 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownloadManagerTask(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, BOOLEAN is_all)
{
    uint32 i = 0;

    if (is_all)
    {
        for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
        {
            if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[i].status
                && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[i].status)
            {
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
                MMIAPIFMM_DeleteFile((const wchar *)dl_manager->task_info[i].file, PNULL);

                SCI_MEMSET(dl_manager->task_info[i].url, 0x00, sizeof(dl_manager->task_info[i].url));
                SCI_MEMSET(dl_manager->task_info[i].file, 0x00, sizeof(dl_manager->task_info[i].file));
                dl_manager->task_info[i].cur_len = 0;
                dl_manager->task_info[i].total_len = 0;
                dl_manager->task_info[i].status = MMIDL_DOWNLOAD_NONE;

                if (dl_manager->total_task_num > 0)
                {
                    dl_manager->total_task_num--;
                }
#else
                dl_manager->task_info[i].status = MMIDL_DOWNLOAD_PAUSED;
#endif
            }
        }
    }
    else
    {
        if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
        {
            return;
        }

        if (MMIDL_DOWNLOAD_NONE != dl_manager->task_info[index].status
            && MMIDL_DOWNLOAD_DOWNLOADED != dl_manager->task_info[index].status)
        {
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
            MMIAPIFMM_DeleteFile(dl_manager->task_info[index].file, PNULL);

            SCI_MEMSET(dl_manager->task_info[index].url, 0x00, sizeof(dl_manager->task_info[index].url));
            SCI_MEMSET(dl_manager->task_info[index].file, 0x00, sizeof(dl_manager->task_info[index].file));
            dl_manager->task_info[index].cur_len = 0;
            dl_manager->task_info[index].total_len = 0;
            dl_manager->task_info[index].status = MMIDL_DOWNLOAD_NONE;
            
            if (dl_manager->total_task_num > 0)
            {
                dl_manager->total_task_num--;
            }
#else
            dl_manager->task_info[index].status = MMIDL_DOWNLOAD_PAUSED;
#endif
        }
    }

    MMIDL_WriteDownloadManager(dl_manager);
}


/*****************************************************************************/
//  Description : change download task status
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskStatus(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, MMIDL_DOWNLOAD_STATUS_E status)
{
    MMIDL_DOWNLOAD_STATUS_E dl_status[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"MMIDL_ChangeDownloadTaskStatus index = %d, status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_436_112_2_18_2_13_32_136,(uint8*)"dd", index, status);
    dl_manager->task_info[index].status = status;

    MMIDL_ReadDownloadManagerInfoStatus(dl_status);
    dl_status[index] = status;
    MMIDL_WriteDownloadManagerInfoStatus(dl_status);
}

/*****************************************************************************/
//  Description : change download task file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskFileName(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index)
{
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (MMIDL_DOWNLOAD_DOWNLOADED == dl_manager->task_info[index].status)
    {

        MMIDL_GetFileNameWithoutTemp(dl_manager, index, file_name, MMIFILE_FULL_PATH_MAX_LEN);

        SFS_RenameFile((uint16 *)dl_manager->task_info[index].file, (uint16 *)file_name, PNULL);
        SCI_MEMCPY(dl_manager->task_info[index].file, file_name, sizeof(dl_manager->task_info[index].file))
        MMIDL_WriteDownloadManager(dl_manager); //写回NV
    }
}

/*****************************************************************************/
//  Description : change OMA download task file name
//  Global resource dependence : none
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OMAChangeDownloadTaskFileName(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index)
{
    wchar           file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (MMIDL_DOWNLOAD_DOWNLOADED == dl_manager->task_info[index].status)
    {
        int32       i = 0;
        int32       j= 0;
        uint16      name_len = 0;
        uint16      ext_len = 0;
        const char  *ext_name = PNULL;

        MMIDL_GetFileNameWithoutTemp(dl_manager, index, file_name, MMIFILE_FULL_PATH_MAX_LEN);
        name_len = MMIAPICOM_Wstrlen(file_name);

        for (i = name_len; i >= 0; i--)
        {
            SCI_TRACE_LOW("MMIDL_ChangeDownloadTaskStatus mime_type = %d", dl_manager->task_info[index].mime_type);

            if (file_name[i] == '.')
            {
                ext_name = mime_to_ext(dl_manager->task_info[index].mime_type);
                ext_len = strlen(ext_name);
                j = i + 1;
                if (j + ext_len <= MMIFILE_FULL_PATH_MAX_LEN)
                {
                    for (i = 0; i < ext_len; i++)
                    {
                        file_name[j++] = ext_name[i];
                    }
                    SCI_MEMSET(&file_name[j], 0, (MMIFILE_FULL_PATH_MAX_LEN - j) * sizeof(wchar));
                }
                break;
            }
        }

        SFS_RenameFile((uint16 *)dl_manager->task_info[index].file, (uint16 *)file_name, PNULL);
        SCI_MEMCPY(dl_manager->task_info[index].file, file_name, sizeof(dl_manager->task_info[index].file))
        MMIDL_WriteDownloadManager(dl_manager); //写回NV
    }
}

/*****************************************************************************/
//  Description : Get file name without .temp
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetFileNameWithoutTemp(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index, wchar *file_name, uint32 name_len)
{
    uint16 file_len = MMIAPICOM_Wstrlen(dl_manager->task_info[index].file);
    uint16 tmp_ext_len = strlen(MMIDL_TMPFILE_EXT);
    wchar *temp_ext_name_ptr = PNULL;

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (file_len <= tmp_ext_len)
    {
        MMI_WSTRNCPY(file_name,
                                MMIFILE_FULL_PATH_MAX_LEN,
                                dl_manager->task_info[index].file,
                                file_len,
                                MIN(name_len, file_len));
        return;
    }

    temp_ext_name_ptr = (wchar *)(dl_manager->task_info[index].file + file_len - tmp_ext_len);

    MMI_STRNTOWSTR(file_name,
                                name_len,
                                (uint8 *)MMIDL_TMPFILE_EXT,
                                tmp_ext_len,
                                tmp_ext_len);

    if (0 == MMIAPICOM_Wstrcmp(temp_ext_name_ptr, (wchar *)file_name))
    {
        SCI_MEMSET(file_name, 0x00, name_len);
        MMI_WSTRNCPY(file_name,
                                name_len,
                                dl_manager->task_info[index].file,
                                file_len,
                                MIN(name_len, file_len));

        SCI_MEMSET(file_name + file_len - tmp_ext_len, 0x00, (tmp_ext_len * sizeof(wchar)));
    }
    else
    {
        MMI_WSTRNCPY(file_name,
                                name_len,
                                dl_manager->task_info[index].file,
                                file_len,
                                MIN(name_len, file_len));
    }
}

/*****************************************************************************/
//  Description : change download task file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeFileNameToTmpFile(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index)
{
    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (MMIDL_DOWNLOAD_DOWNLOADED == dl_manager->task_info[index].status)
    {
        wchar temp_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
        uint16 file_len = MMIAPICOM_Wstrlen(dl_manager->task_info[index].file);
        uint16 tmp_ext_len = strlen(MMIDL_TMPFILE_EXT);
        wchar *temp_ext_name_ptr = PNULL;

        if (file_len <= tmp_ext_len)
        {
            return;
        }

        temp_ext_name_ptr = (wchar *)(dl_manager->task_info[index].file + file_len - tmp_ext_len);

        MMI_STRNTOWSTR(temp_file,
                                    MMIFILE_FULL_PATH_MAX_LEN,
                                    (uint8 *)MMIDL_TMPFILE_EXT,
                                    tmp_ext_len,
                                    tmp_ext_len);

        if (0 != MMIAPICOM_Wstrcmp(temp_ext_name_ptr, (wchar *)temp_file)//后缀名不是.tmp
            && (file_len + tmp_ext_len <= MMIFILE_FULL_PATH_MAX_LEN))
        {
            SCI_MEMSET(temp_file, 0x00, sizeof(temp_file));
            MMI_STRNTOWSTR(dl_manager->task_info[index].file + file_len,
                                        MMIFILE_FULL_PATH_MAX_LEN - file_len,
                                        (uint8 *)MMIDL_TMPFILE_EXT,
                                        tmp_ext_len,
                                        tmp_ext_len);

            MMIDL_WriteDownloadManager(dl_manager); //写回NV
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDL_ChangeDownloadTaskFileName index = %d, error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_567_112_2_18_2_13_32_137,(uint8*)"d", index);
        }
    }

}

/*****************************************************************************/
//  Description : change download task total len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskTotalLen(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 total_len)
{
    uint32 dl_total_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIDL_ChangeDownloadTaskTotalLen index = %d, total_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_586_112_2_18_2_13_32_138,(uint8*)"dd", index, total_len);

    dl_manager->task_info[index].total_len = total_len;

    MMIDL_ReadDownloadManagerInfoTotalLen(dl_total_len);
    dl_total_len[index] = total_len;
    MMIDL_WriteDownloadManagerInfoTotalLen(dl_total_len);
}

/*****************************************************************************/
//  Description : change download task current len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskCurLen(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 cur_len)
{
    uint32 dl_current_len[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIDL_ChangeDownloadTaskCurLen index = %d, cur_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_608_112_2_18_2_13_32_139,(uint8*)"dd", index, cur_len);

    dl_manager->task_info[index].cur_len = cur_len;

    MMIDL_ReadDownloadManagerInfoCurrentLen(dl_current_len);
    dl_current_len[index] = cur_len;
    MMIDL_WriteDownloadManagerInfoCurrentLen(dl_current_len);
}

/*****************************************************************************/
//  Description : get total task num
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetDownloadManagerTotalTaskNum(MMIDL_DOWNLOAD_MANAGER_T *dl_manager)
{
    uint32 total_task_num = 0;

    total_task_num = dl_manager->total_task_num;

    //SCI_TRACE_LOW:"MMIDL_GetDownloadManagerTotalTaskNum total_task_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_627_112_2_18_2_13_32_140,(uint8*)"d", total_task_num);

    return total_task_num;
}

/*****************************************************************************/
//  Description : change download task total len
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ChangeDownloadTaskMimeType(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, uint32 mime_type)
{
    uint32 mime[MMIDL_MAX_DOWNLOAD_TASK_NUM] = {0};

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    SCI_TRACE_LOW("MMIDL_ChangeDownloadTaskMimeType mime_type = %d", mime_type);

    dl_manager->task_info[index].mime_type = mime_type;
    MMIDL_ReadDownloadManagerInfoMimeType(mime);
    mime[index] = mime_type;
    MMIDL_WriteDownloadManagerInfoMimeType(mime);
}

/*****************************************************************************/
//  Description : get unused Manager task id
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetUnUnusedManagerTaskId(MMIDL_DOWNLOAD_MANAGER_T *dl_manager)
{
    uint32 unused_id = 0;
    uint32 i = 0;

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (MMIDL_DOWNLOAD_NONE == dl_manager->task_info[i].status)
        {
            break;
        }
    }

    //SCI_TRACE_LOW:"MMIDL_GetUnUnusedManagerTaskId unused_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_649_112_2_18_2_13_32_141,(uint8*)"d", unused_id);

    return i;
}

/*****************************************************************************/
//  Description : get unused Manager task Info by Index
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetManagerTaskInfo(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 index, MMIDL_DOWNLOAD_MANAGER_INFO_T *info_ptr)
{
    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM || PNULL == info_ptr)
    {
        return;
    }

    SCI_MEMCPY(info_ptr, &dl_manager->task_info[index], sizeof(MMIDL_DOWNLOAD_MANAGER_INFO_T));
}

/*****************************************************************************/
//  Description : Judg whether is same url in download manager
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_IsSameDownloadDestUrl(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, char *url, uint32 *same_task_index)
{
    uint32 i = 0;
    BOOLEAN is_same_url = FALSE;

    if (PNULL == url)
    {
        return is_same_url;
    }

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (0 == strcmp(url, (char *)dl_manager->task_info[i].url))
        {
            *same_task_index = i;

            is_same_url = TRUE;
            break;
        }
    }

    //SCI_TRACE_LOW:"MMIDL_IsSameDownloadDestUrl is_same_url=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_693_112_2_18_2_13_32_142,(uint8*)"d", is_same_url);

    return is_same_url;
}

/*****************************************************************************/
//  Description : append DL Task Item 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppendDLTaskListItem(MMI_WIN_ID_T win_id, MMI_HANDLE_T ctrl_handle)
{
    uint32 i = 0;
    uint32 j = 0;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 cur_index = 0;
    uint16 total_num = 0;

    dl_manager = MMIDL_GetDownloadManager();

    if (0 < dl_manager->total_task_num)
    {
        GUILIST_POS_INFO_T pos_info = {0};

        GUILIST_GetCurPosInfo(ctrl_handle, &pos_info);

        cur_index = GUILIST_GetCurItemIndex(ctrl_handle);

        if (GUILIST_GetTotalItemNum(ctrl_handle) > 0)
        {
            GUILIST_RemoveAllItems(ctrl_handle);
        }
        GUILIST_SetMaxItem(ctrl_handle, (uint16)dl_manager->total_task_num, FALSE);

        for( i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++ )
        {
            SCI_TRACE_LOW("liyanzhu i = %d, j = %d",i,j);
            if (MMIDL_DOWNLOAD_NONE < dl_manager->task_info[i].status && dl_manager->task_info[i].status < MMIDL_DOWNLOAD_STATUS_MAX)
            {//cr137619liyan modified
                //MMIDL_SetDownloadManagerTaskIndex(j++, i);
                AppendDLListItemData(ctrl_handle, &dl_manager->task_info[i], &j, i);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDL_AppendDLTaskListItem invalid status = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_735_112_2_18_2_13_33_143,(uint8*)"d", dl_manager->task_info[i].status);
            }
        }
		//cr137619liyan modified
		SCI_TRACE_LOW("liyanzhu137619 dl_manager->total_task_num = %d, MMIDL_GetDownloadManagerTaskIndex(j -1) = %d",dl_manager-> total_task_num, MMIDL_GetDownloadManagerTaskIndex(j -1));
		if( (j > 0) && (dl_manager-> total_task_num <= MMIDL_GetDownloadManagerTaskIndex(j - 1)))
		{
			MMIDL_WriteDownloadManagerTotalTaskNum(j);
		}

        total_num = GUILIST_GetTotalItemNum(ctrl_handle);
        if (total_num >= 1 && cur_index >= total_num)
        {
            cur_index = total_num - 1;
            GUILIST_SetCurItemIndex(ctrl_handle, cur_index);
        }
        else
        {
            GUILIST_SetCurPosInfo(ctrl_handle, &pos_info);
        }
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_handle, 1, FALSE);

        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_DL_EMPTY_LIST;

        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_AppendItem(ctrl_handle, &item_t);  
    }

}

/*****************************************************************************/
//  Description : add item to listbox for download mdu 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cr137619liyan modified
/*****************************************************************************/
LOCAL void AppendDLListItemData(MMI_HANDLE_T ctrl_handle, MMIDL_DOWNLOAD_MANAGER_INFO_T *task_info_ptr,uint32 *index_ptr, uint32 task_index )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    uint16 file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 total_len = 0;
    uint8 prog_str[10] = {0};
    wchar temp_wstr_1[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr_2[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 prog_str_len = 0;
    uint64 cur_len = 0;

    if (PNULL == task_info_ptr || PNULL == index_ptr)
    {
        return;
    }

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    switch (task_info_ptr->status)
    {
    case MMIDL_DOWNLOAD_DOWNLOADING:
        image_id = IMAGE_DL_DOWNLOADING;
        cur_len = task_info_ptr->cur_len;
        total_len = task_info_ptr->total_len;

        if (0 != total_len)
        {
            if (total_len < cur_len)
            {
                cur_len = total_len;
            }

            cur_len *= 100;
            sprintf((char *)prog_str, "%d%%", (uint32)(cur_len/total_len));
        }
        else
        {
            sprintf((char *)prog_str, "-%%");
        }
        break;

    case MMIDL_DOWNLOAD_DOWNLOADED:
        image_id = IMAGE_DL_DOWNLOADED;
        sprintf((char *)prog_str, "100%%");
        break;

    case MMIDL_DOWNLOAD_FAILED:
        image_id = IMAGE_DL_FAILED;
        sprintf((char *)prog_str, " ");
        break;

    case MMIDL_DOWNLOAD_WAITING:
    case MMIDL_DOWNLOAD_WAITING_HTTP:
        image_id = IMAGE_DL_WAITING;

        cur_len = task_info_ptr->cur_len;
        total_len = task_info_ptr->total_len;
        if (0 != total_len)
        {
            if (total_len < cur_len)
            {
                cur_len = total_len;
            }
            cur_len *= 100;
            sprintf((char *)prog_str, "%d%%", (uint32)(cur_len/total_len));
        }
        else
        {
            sprintf((char *)prog_str, "-%%");
        }
        break;

    case MMIDL_DOWNLOAD_PAUSED:
        image_id = IMAGE_DL_PAUSED;

        cur_len = task_info_ptr->cur_len;
        total_len = task_info_ptr->total_len;
        if (0 != total_len)
        {
            if (total_len < cur_len)
            {
                cur_len = total_len;
            }
            cur_len *= 100;
            sprintf((char *)prog_str, "%d%%", (uint32)(cur_len/total_len));
        }
        else
        {
            sprintf((char *)prog_str, "-%%");
        }
        break;

    default:
        return;
    }
    item_data.item_content[0].item_data.image_id = image_id;

    file_name_len = GetDownloadDisplayFilename(task_info_ptr->file, file_name, file_name_len);
    if(file_name_len > 0)//cr137619liyan modified
   	{
		MMIDL_SetDownloadManagerTaskIndex((*index_ptr)++, task_index);
		
    item_data.item_content[1].item_data.text_buffer.wstr_len = (uint8)MIN(file_name_len, GUILIST_STRING_MAX_NUM);

    MMI_WSTRNCPY(temp_wstr_1,
                                item_data.item_content[1].item_data.text_buffer.wstr_len,
                                file_name,
                                item_data.item_content[1].item_data.text_buffer.wstr_len,
                                item_data.item_content[1].item_data.text_buffer.wstr_len);

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)temp_wstr_1;

    prog_str_len =SCI_STRLEN((char *)prog_str);
    
    item_data.item_content[2].item_data.text_buffer.wstr_len = (uint8)MIN(prog_str_len, GUILIST_STRING_MAX_NUM);

    MMI_STRNTOWSTR(temp_wstr_2,
                                item_data.item_content[2].item_data.text_buffer.wstr_len,
                                (uint8 *)prog_str,
                                strlen((char *)prog_str),
                                strlen((char *)prog_str));

    item_data.item_content[2].item_data.text_buffer.wstr_ptr = (wchar *)temp_wstr_2;

    GUILIST_AppendItem(ctrl_handle, &item_t);
   	}
   
} 

/*****************************************************************************/
//  Description : Get Download Display Filename
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDownloadDisplayFilename(wchar *full_path, wchar *file_name, uint16 max_name_len)
{

    int32 i = 0;
    uint16 full_path_len = (uint16)MMIAPICOM_Wstrlen(full_path);
    uint16 name_len = 0;

    if (PNULL == full_path || PNULL == file_name)
    {
        return 0;
    }

    //SCI_TRACE_LOW:"GetDownloadDisplayFilename full_path_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_921_112_2_18_2_13_33_144,(uint8*)"d", full_path_len);
    if (full_path_len <= 1)
    {
        return 0;
    }

    for (i = (int32)(full_path_len - 1); i >= 0; i--)
    {
        if ('\\' == full_path[i] || '/' == full_path[i])
        {
            break;
        }
    }
    
    SCI_TRACE_LOW("GetDownloadDisplayFilename i = %d", i);

    if (i < 0)
    {
        i = 0;
    }

    name_len = (uint16)(full_path_len - 1 - i);

    if (name_len > max_name_len)
    {
        name_len = max_name_len;
    }

    MMI_WSTRNCPY(file_name,
                            max_name_len,
                            full_path + i + 1,
                            name_len,
                            name_len
                            );

    return name_len;
}

/*****************************************************************************/
//  Description : add Task Detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppendDLTaskDetail(MMI_WIN_ID_T win_id, MMI_HANDLE_T ctrl_handle, uint32 task_index)
{
    uint16 index = 0;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T string = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    uint16 file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    char file_size[10] = {0};
    uint16 file_size_len = 0;
    uint16 url_str_len =0;
    
    if (task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    MMIDL_GetManagerTaskInfo(MMIDL_GetDownloadManager(), task_index, &task_info);

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF; 

    MMI_GetLabelTextByLang(TXT_INPUT_FILE_NAME, &string);

    //file name:
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index);

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

    file_name_len = GetDownloadDisplayFilename(task_info.file, file_name, file_name_len);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.str_ptr = (wchar *)file_name;
    item_data.text_data.buf.len = file_name_len;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index);

    //URL:
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
            
    MMI_GetLabelTextByLang(TXT_COMMON_URL, &string);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

    SCI_MEMSET(item_text, 0, sizeof(item_text));

    url_str_len = SCI_STRLEN((char *)task_info.url);
    
    item_data.text_data.buf.len = GUI_UTF8ToWstr(
                                                                                item_text,
                                                                                GUIRICHTEXT_TEXT_MAX_LEN,
                                                                                task_info.url,
                                                                                (uint16)MIN(url_str_len, GUIRICHTEXT_TEXT_MAX_LEN)
                                                                            );
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 

    //file size:
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
            
    MMI_GetLabelTextByLang(TXT_FILE_DETAIL_SIZE, &string);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

    if (task_info.cur_len < MMIDL_ONE_KBYTE)
    {
        sprintf(file_size, "%dB", task_info.cur_len);
    }
    else
    {
        sprintf(file_size, "%dKB", task_info.cur_len/MMIDL_ONE_KBYTE);
    }

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    file_size_len = (uint16)strlen((char *)file_size);
    item_data.text_data.buf.len = file_size_len;
    MMI_STRNTOWSTR(item_text,
                                GUIRICHTEXT_TEXT_MAX_LEN,
                                (uint8 *)file_size,
                                file_size_len,
                                file_size_len
                                );

    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 
#if 0
    //file path:
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
            
    MMI_GetLabelTextByLang(TXT_DL_DETAIL_PATH, &string);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

    SCI_MEMSET(item_text, 0, sizeof(item_text));

    if ((uint16)MMIAPICOM_Wstrlen(task_info.file) > file_name_len + 1)
    {
        MMI_STRING_T path_string = {0};
        wchar path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
        uint16 path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
        uint16 file_len = 0;

        file_len = (uint16)MMIAPICOM_Wstrlen(task_info.file);
        item_data.text_data.buf.len = (uint16)MIN(file_len - file_name_len - 1, GUIRICHTEXT_TEXT_MAX_LEN);

        path_string.wstr_ptr = (wchar *)task_info.file;
        path_string.wstr_len = item_data.text_data.buf.len;

        MMIDL_GetDisplayPathText(&path_string,
                                                    path_name,
                                                    &path_name_len
                                                    );

        item_data.text_data.buf.len = (uint16)MIN(path_name_len, GUIRICHTEXT_TEXT_MAX_LEN);

        MMI_WSTRNCPY(item_text,
                                GUIRICHTEXT_TEXT_MAX_LEN,
                                path_name,
                                item_data.text_data.buf.len,
                                item_data.text_data.buf.len
                                );
    }

    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_handle, &item_data, &index); 
#endif
}

/*****************************************************************************/
//  Description : Open File Explorer
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenFileExplorer(uint32 task_index)
{
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    wchar path_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};  
    uint16 path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    uint16 file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 total_task_num = 0;
    uint16 file_len = 0;
    
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    
    dl_manager = MMIDL_GetDownloadManager();

    total_task_num = MMIDL_GetDownloadManagerTotalTaskNum(dl_manager);

    SCI_TRACE_LOW("[mmidl_func.c][MMIDL_OpenFileExplorer]  task_index = %d,total_task_num = %d",task_index,total_task_num);

    if (0 == total_task_num)
    {
        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_TYPE_MAX, path_name, &path_name_len);

        MMIAPIFMM_OpenExplorerExt(path_name,
                                                    path_name_len,
                                                    PNULL,
                                                    0,
                                                    FALSE,
                                                    MMIFMM_FILE_ALL
                                                    );
    }
    else
    {
        MMIDL_GetManagerTaskInfo(dl_manager, task_index, &task_info);
	    SCI_TRACE_LOW("[mmidl_func.c][MMIDL_OpenFileExplorer]  task_info.mime_type = %d",task_info.mime_type);
        if ((task_info.mime_type >= MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE) && (task_info.mime_type <= MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML))
        {

          wchar   device_name[MMIFILE_DEVICE_NAME_MAX_LEN + 2] = {0};
          uint16  device_name_len = 0;
          wchar*              full_path_ptr = PNULL;
          uint16              full_path_len = MMIFMM_FULL_FILENAME_LEN;
          full_path_ptr = SCI_ALLOC_APP((MMIFMM_FULL_FILENAME_LEN + 1) * sizeof(wchar));
          if(PNULL == full_path_ptr)
          {
            return;
          }
          SCI_MEMSET(full_path_ptr, 0, ((MMIFMM_FULL_FILENAME_LEN + 1) * sizeof(wchar)));
          //get the new full file name by the type
          MMIAPIFMM_SplitFullPath(task_info.file, 
            MMIAPICOM_Wstrlen(task_info.file), 
            device_name, &device_name_len, 
            PNULL, PNULL, 
            PNULL, PNULL
            );  
          MMIAPIFMM_CombineFullPath(&device_name, device_name_len, PNULL, 0,
          PNULL, 0, full_path_ptr, &full_path_len);    

          MMIAPIFMM_OpenExplorerExt(full_path_ptr, full_path_len,PNULL,0,FALSE, MMIFMM_FILE_ALL);  
          SCI_FREE(full_path_ptr);
        }
        else
        {
                    
          file_name_len = GetDownloadDisplayFilename(task_info.file, file_name, file_name_len);
          
          file_len = (uint16)MMIAPICOM_Wstrlen(task_info.file);

                  if (file_len > file_name_len + 1)
        {
            path_name_len = (uint16)MIN(file_len- file_name_len - 1, MMIFILE_FULL_PATH_MAX_LEN);

            MMI_WSTRNCPY(path_name,
                                    MMIFILE_FULL_PATH_MAX_LEN,
                                    task_info.file,
                                    path_name_len,
                                    path_name_len
                                    );

            MMIAPIFMM_OpenExplorerExt(path_name,
                                                        path_name_len,
                                                        file_name,
                                                        file_name_len,
                                                        FALSE,
                                                        MMIFMM_FILE_ALL
                                                        );
        }
          
        }
        
    }
}

/*****************************************************************************/
//  Description : Get download path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetDisplayPathText(MMI_STRING_T *path_value_str_ptr,
                                                                                wchar *path_name_ptr,
                                                                                uint16 *path_name_len_ptr)
{
    MMI_STRING_T     path_string = {0};
    MMIFILE_DEVICE_E dev         = MMI_DEVICE_NUM;
    
    if (PNULL == path_value_str_ptr || PNULL == path_name_ptr || PNULL == path_name_len_ptr)
    {
        return;
    }

    dev = MMIAPIFMM_GetDeviceTypeByPath(path_value_str_ptr->wstr_ptr , MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM == dev)
    {
        //SCI_TRACE_LOW:"invalid device"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1197_112_2_18_2_13_33_145,(uint8*)"");
        return;
    } 
     
    MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &path_string);
  
    *path_name_len_ptr = (uint16)MIN(path_string.wstr_len, MMIFILE_FULL_PATH_MAX_LEN);

    MMI_WSTRNCPY(path_name_ptr,
                            MMIFILE_FULL_PATH_MAX_LEN,
                            path_string.wstr_ptr,
                            path_string.wstr_len,
                            *path_name_len_ptr);

    MMI_WSTRNCPY(path_name_ptr + *path_name_len_ptr,
                            MMIFILE_FULL_PATH_MAX_LEN,
                            path_value_str_ptr->wstr_ptr + 1,
                            path_value_str_ptr->wstr_len - 1,
                            MIN(path_value_str_ptr->wstr_len - 1, MMIFILE_FULL_PATH_MAX_LEN - *path_name_len_ptr));

    *path_name_len_ptr += MIN(path_value_str_ptr->wstr_len - 1, MMIFILE_FULL_PATH_MAX_LEN - *path_name_len_ptr);
}

/*****************************************************************************/
//  Description : Get Real path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_GetRealPathText(MMI_STRING_T *path_value_str_ptr,
                                                                                wchar *path_name_ptr,
                                                                                uint16 *path_name_len_ptr)
{
    MMI_STRING_T     path_str = {0};
    MMIFILE_DEVICE_E dev      = MMI_DEVICE_UDISK;
    //uint16           path_len = 0;
    if (PNULL == path_value_str_ptr || PNULL == path_name_ptr || PNULL == path_name_len_ptr)
    {
        return;
    }

    for(; dev<MMI_DEVICE_NUM; dev++)
    {
        MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &path_str);
        if(0 == MMIAPICOM_Wstrncmp(path_value_str_ptr->wstr_ptr, path_str.wstr_ptr, path_str.wstr_len))
        {
            MMIAPICOM_Wstrncpy(path_name_ptr, MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev));
            break;
        }
    }
    if(MMI_DEVICE_NUM == dev)
    {
        return;
    }
 
    *path_name_len_ptr = 1;

    MMI_WSTRNCPY(path_name_ptr + 1,
                            MMIFILE_FULL_PATH_MAX_LEN - 1,
                            path_value_str_ptr->wstr_ptr + path_str.wstr_len,
                            path_value_str_ptr->wstr_len - path_str.wstr_len,
                            MIN(path_value_str_ptr->wstr_len - path_str.wstr_len, MMIFILE_FULL_PATH_MAX_LEN - 1));

    *path_name_len_ptr += MIN(path_value_str_ptr->wstr_len - path_str.wstr_len, MMIFILE_FULL_PATH_MAX_LEN - 1);
}
/*****************************************************************************/
//  Description : Appoint download save path text
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_AppointDownloadSavePath(wchar *path_ptr, uint32 path_len, MMIDL_OpenFileCallBack callback)
{
    FILEARRAY ret_array = PNULL;
    MMIFMM_FILTER_T filter = {0};

    ret_array = MMIAPIFILEARRAY_Create();

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    if (FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(path_ptr,
                                                                    (uint16)path_len,
                                                                    &filter,
                                                                    FUNC_FIND_FOLDER,
                                                                    FALSE,
                                                                    NULL,
                                                                    ret_array,
                                                                    callback,
                                                                    FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetPDPInfo(MMIDL_PDP_INFO_T *pdp_info)
{
    if (PNULL != pdp_info)
    {
        MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

        if (PNULL == download_instance)
        {
            //SCI_TRACE_LOW:"MMIDL_SetPDPInfo download_instance Null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1299_112_2_18_2_13_34_146,(uint8*)"");
            return;
        }

        SCI_MEMCPY(&download_instance->pdp_info, pdp_info, sizeof(MMIDL_PDP_INFO_T));
    }
}

/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetPDPInfo(MMIDL_PDP_INFO_T *pdp_info)
{
    if (PNULL != pdp_info)
    {
        MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

        if (PNULL == download_instance)
        {
            //SCI_TRACE_LOW:"MMIDL_GetPDPInfo download_instance Null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1319_112_2_18_2_13_34_147,(uint8*)"");
            return FALSE;
        }

        SCI_MEMCPY(pdp_info, &download_instance->pdp_info, sizeof(MMIDL_PDP_INFO_T));
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Get PDP info of settings
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetSettingsPDPInfo(MMIDL_PDP_INFO_T *pdp_info, MN_DUAL_SYS_E e_dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    uint8 index = 0;

    if (PNULL == pdp_info)
    {
        return FALSE;
    }

#ifdef BROWSER_SUPPORT
    index = MMIAPIBROWSER_GetNetSettingIndex(e_dual_sys);
#endif

    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(e_dual_sys, index);

    SCI_MEMSET(&pdp_info->connect_setting, 0x0, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
    SCI_MEMCPY(&pdp_info->connect_setting, link_setting_ptr, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

    return TRUE;
}

/*****************************************************************************/
//  Description : Get download file name
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL wchar *GetDownloadFileName(const wchar *file_name)
{
    uint32 i = 0;
    uint32 file_name_len = 0;

    if (PNULL == file_name)
    {
        return PNULL;
    }

    file_name_len = MMIAPICOM_Wstrlen(file_name);

    if (0 == file_name_len)
    {
        return PNULL;
    }

    for (i = file_name_len - 1; i > 0; i--)
    {
        if ('\\' == file_name[i] || '/' == file_name[i])
        {
            break;
        }
    }

    if (i == file_name_len - 1)
    {
        return PNULL;
    }

    return ((wchar *) (file_name + i + 1));
}

/*****************************************************************************/
//  Description : Notify download complete
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
PUBLIC void MMIDL_NotifyDownloadComplete(uint32 index, BOOLEAN is_success)
{
    BOOLEAN sound_notify = FALSE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMISET_ALL_RING_TYPE_E ring_type = MMISET_RING_TYPE_NONE;
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    MMI_STRING_T string_dl_status = {0};
    MMI_STRING_T string_dl_file = {0};
    
    MMI_WIN_ID_T win_id = MMIDL_DOWNLOAD_ALERT_WIN_ID;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    MMIDL_MIME_CALLBACK_PARAM_T callback_param = {0};
    
    dl_manager = MMIDL_GetDownloadManager();

    sound_notify = (0 == MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, DOWNLOAD_NOTIFY));

    //SCI_TRACE_LOW:"MMIDL_NotifyDownloadComplete index=%d,is_success=%d,sound_notify= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1417_112_2_18_2_13_34_148,(uint8*)"ddd", index, is_success, sound_notify);
    MMIDL_GetManagerTaskInfo(dl_manager, index, &task_info);
    MMI_WSTRNCPY(callback_param.file_name, MMIFILE_FULL_PATH_MAX_LEN , task_info.file, MMIFILE_FULL_PATH_MAX_LEN, MMIAPICOM_Wstrlen(task_info.file));
    callback_param.mime_type = task_info.mime_type;//download_task_ptr->mime_type;
    callback_param.url_ptr = (uint8 *)task_info.url;
    //callback_param.user_data = (void *)download_task_ptr->manager_index;
    callback_param.download_result = TRUE;
    callback_param.result_code = task_info.result;//added for cr240847

    if (!is_success
#ifdef OMADL_SUPPORT
        || MIME_TYPE_APPLICATION_VND_OMA_DD != task_info.mime_type
#endif
        )
    {
        if (is_success)
        {
            callback_param.download_result = TRUE;
            text_id = TXT_DL_DOWNLOAD_COMPLETE;
            img_id = IMAGE_PUBWIN_SUCCESS;
            ring_type = MMISET_RING_TYPE_DL;
        }
        else
        {
            callback_param.download_result = FALSE;    
            text_id = TXT_DOWNLOAD_FAILED;
            img_id = IMAGE_PUBWIN_WARNING;
            ring_type = MMISET_RING_TYPE_DL;
        }

        MMIDL_GetManagerTaskInfo(dl_manager, index, &task_info);

        string_dl_file.wstr_ptr = (wchar *)GetDownloadFileName((const wchar *)task_info.file);
        string_dl_file.wstr_len = MMIAPICOM_Wstrlen((const wchar *)string_dl_file.wstr_ptr);

        MMI_GetLabelTextByLang(text_id, &string_dl_status);

        MMIPUB_OpenAlertWinByTextPtr(PNULL, &string_dl_file, &string_dl_status, img_id, &win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIDEFAULT_TurnOnBackLight();

        if (sound_notify)
        {
            if (MMIAPIUDISK_UdiskIsRun() 
                ||MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef JAVA_SUPPORT
                || MMIAPIJAVA_IsFocusJAVARunWin() 
#endif
#ifdef WRE_SUPPORT
                ||WRE_IsFocusRunWin()
#endif
                || MMIAPIUdisk_IsOpenPcCamera()
#ifdef STREAMING_HS_SUPPORT
                || MMIAPISMP_IsStreamPlaying()
#endif /* STREAMING_HS_SUPPORT */ 
            )
            {
                MMIAPISET_StopAllVibrator();
                MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, FALSE, MMIDL_VIBRA_TIMEROUT);        
            }
            else
            {
                MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE, 0, 1, ring_type, PNULL);
            }
        }
    }

    //SCI_TRACE_LOW:"MMIDL_NotifyDownloadComplete mime_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1476_112_2_18_2_13_34_149,(uint8*)"d",callback_param.mime_type);
    MMIDL_MimeCallBack(&callback_param);
}

/*****************************************************************************/
//  Description : Set Multi Task num
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetMultiTaskNum(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    MMIDL_DOWNLOAD_METHOD_TYPE download_method = MMIDL_DOWNLOAD_METHOD_APPOSITE;

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_SetMultiTaskNum download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1491_112_2_18_2_13_34_150,(uint8*)"");
        return;
    }

    MMIDL_ReadDownloadMethodType(&download_method);

    if (MMIDL_DOWNLOAD_METHOD_APPOSITE == download_method)
    {
        download_instance->multi_task_num = MMIDL_MAX_MULTI_DL_TASK_NUM;
    }
    else
    {
        download_instance->multi_task_num = MMIDL_QUEUE_DL_TASK_NUM;
    }
}

/*****************************************************************************/
//  Description : Get Multi Task num
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetMultiTaskNum(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_SetMultiTaskNum download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1517_112_2_18_2_13_34_151,(uint8*)"");
        return 0;
    }

    return download_instance->multi_task_num;
}

/*****************************************************************************/
//  Description : Get Download Manager
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_MANAGER_T *MMIDL_GetDownloadManager(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_GetDownloadManager download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1534_112_2_18_2_13_34_152,(uint8*)"");
        return PNULL;
    }

    return download_instance->downoad_manager;
}

/*****************************************************************************/
//  Description : Set Download Manager
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadManager(MMIDL_DOWNLOAD_MANAGER_T *downoad_manager )
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (PNULL == download_instance ||PNULL == downoad_manager)
    {
        //SCI_TRACE_LOW:"MMIDL_SetDownloadManager download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1551_112_2_18_2_13_34_153,(uint8*)"");
        return;
    }
    download_instance->downoad_manager = downoad_manager;
}

/*****************************************************************************/
//  Description : reset download setting to default
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_ResetDefaultSettings(void)
{
    MMIDL_WriteDownloadMethodType(MMIDL_DOWNLOAD_METHOD_QUEUE);

    MMIDL_WriteDownloadPathType(MMIDL_DOWNLOAD_PATH_DEFAULT);
}

/*****************************************************************************/
//  Description : Start OMA Download
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_StartOMADownload(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, uint32 task_id)
{
#ifdef OMADL_SUPPORT
    ODM_START_DOWNLOAD_PARAM_T oma_dl_req={0};
    MMIFILE_HANDLE file_handle = PNULL;
    uint8 *buffer = PNULL;
    uint32 file_size = 0;
    uint32 read_size = 0;
    MMIDL_PDP_INFO_T pdp_info = {0};
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    int32 oma_ret = ODM_SUCCESS + 1;
    //MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager;
    int32 mgr_index = 0;

     //SCI_TRACE_LOW:"MMIDL_StartOMADownload enter"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1587_112_2_18_2_13_34_154,(uint8*)"");

    //download_task_ptr = &download_instance->download_task[task_id];//cr242594
    dl_manager = download_instance->downoad_manager;
    mgr_index = task_id;//modified for cr242594

    do
    {
        //SCI_TRACE_LOW:"MMIDL_StartOMADownload dl_manager->task_info[%d].url=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1595_112_2_18_2_13_34_155,(uint8*)"ds", mgr_index, dl_manager->task_info[mgr_index].file);
        file_handle = MMIAPIFMM_CreateFile(dl_manager->task_info[mgr_index].file, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);

        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIDL_StartOMADownload File_handle is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1600_112_2_18_2_13_34_156,(uint8*)"");
            break;
        }

        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if (file_size == 0)
        {
            //SCI_TRACE_LOW:"MMIDL_StartOMADownload File size Incorrect"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1607_112_2_18_2_13_34_157,(uint8*)"");
            break;
        }

        buffer = SCI_ALLOCA(file_size);
        if (PNULL == buffer)
        {
            //SCI_TRACE_LOW:"MMIDL_StartOMADownload No mem"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1614_112_2_18_2_13_34_158,(uint8*)"");
            break;
        }

        file_ret = MMIAPIFMM_ReadFile(file_handle, buffer, file_size, &read_size, PNULL);
        if (SFS_NO_ERROR != file_ret)
        {
            //SCI_TRACE_LOW:"MMIDL_StartOMADownload Read File error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1621_112_2_18_2_13_34_159,(uint8*)"");
            break;
        }

        MMIAPIFMM_CloseFile(file_handle);

        MMIAPIFMM_DeleteFileSyn(dl_manager->task_info[mgr_index].file, MMIAPICOM_Wstrlen(dl_manager->task_info[mgr_index].file));

        file_handle = PNULL;

        oma_dl_req.is_dd_url = FALSE;

        //TODO get charset
        oma_dl_req.content_encoding = ODM_ENCODING_UTF8;

        //TODO get protocol
        oma_dl_req.connection_type = ODM_CONNECTION_TYPE_HTTP;

        oma_dl_req.length = file_size;
        oma_dl_req.content = buffer;

        oma_dl_req.magic = task_id;

        MMIDL_GetPDPInfo(&pdp_info);
#ifndef WIN32
        if (0 != pdp_info.connect_setting.gateway_len
            && 0 != strcmp((char *)pdp_info.connect_setting.gateway, (char *)MMIDL_INVALID_GATEWAY))
        {
            oma_dl_req.proxy_info.is_use_proxy = TRUE;
            oma_dl_req.proxy_info.proxy_port = pdp_info.connect_setting.port;
            oma_dl_req.proxy_info.proxy_host_ptr = (char *)pdp_info.connect_setting.gateway;
        }
        else
#endif
        {
            oma_dl_req.proxy_info.is_use_proxy = FALSE;
        }

        oma_dl_req.net_id = download_instance->pdp_info.net_id;
        oma_ret = ODM_StartDownload(&oma_dl_req);
        if(ODM_SUCCESS != oma_ret)
        {
            //SCI_TRACE_LOW:"MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FUNC_1660_112_2_18_2_13_34_160,(uint8*)"d", oma_ret);
            break;
        }
    }
    while (0);

    if (PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
        file_handle = PNULL;
    }

    if (PNULL != buffer)
    {
        SCI_FREE(buffer);
    }

    return (BOOLEAN)(ODM_SUCCESS == oma_ret);

#else
    return TRUE;
#endif
}

/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/

PUBLIC void MMIDL_RichText_AddItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T * item_string, GUIRICHTEXT_FRAME_TYPE_E frame_type, GUIRICHTEXT_TAG_TYPE_E tag_type)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;

    if (PNULL == item_string)
    {
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.len = item_string->wstr_len;
    item_data.text_data.buf.str_ptr = item_string->wstr_ptr;
    item_data.frame_type = frame_type;
    item_data.tag_type = tag_type;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}
/*added for cr240847 begin*/
/*****************************************************************************/
//  Description : set download task's result
//  Global resource dependence : 
//  Author: rui.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadResult(uint32 index, MMIDL_DOWNLOAD_RESULT_E result)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = MMIDL_GetDownloadManager();
    if(PNULL == dl_manager)
    {
        return;
    }
    
    dl_manager->task_info[index].result = result;
}

/*****************************************************************************/
//  Description : get download task's result by index
//  Global resource dependence : 
//  Author: rui.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_RESULT_E MMIDL_GetDownloadResult(uint32 index)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = MMIDL_GetDownloadManager();
    if(PNULL == dl_manager)
    {
        return MMIDL_DOWNLOAD_RESULT_MAX;
    }
    
    return dl_manager->task_info[index].result;
}
/*added for cr240847 end*/
#endif //#ifdef DL_SUPPORT
