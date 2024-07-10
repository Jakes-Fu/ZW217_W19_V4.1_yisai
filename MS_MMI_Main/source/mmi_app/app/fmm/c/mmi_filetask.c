/***************************************************************************
** File Name:      mmi_filetask.c                                          *
** Author:                                                                 *
** Date:           03/29/2007                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the app file task         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2007       Liqing Peng       Create                                  *
****************************************************************************
**                                                                         *
** Using App file Task:                                                    *
** 1. other task send APP_COPY_START signal to Copy Task, start a copy     *
** 2. when copy finished, file task send APP_COPY_END_IND signal           *
**    to other task                                                        *
** 3. before copy finished, other task can send APP_COPY_STOP to stop copy,*
**    then file task also send APP_COPY_END_IND signal back                *
** 4. APP_COPY_END_IND signal with a result value                          *
**    APP_COPY_RESULT_SUCCESS -- APP_COPY_RESULT_WRITEFILE_ERROR           *
** 5. Before copy, call APPFILE_CreateTask first to create file task       *
** 6. After copy finished, call APPFILE_ExitTask to exit file task         *
****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_fmm_trc.h"
#include "os_api.h"
#include "mmi_osbridge.h"
#include "mmi_signal.h"
#include "mmifmm_comfunc.h"
#include "mmi_filetask.h"
#include "mmi_signal.h"
//#include "mmi_signal_ext.h"
#include "tasks_id.h"
#include "mmifmm_export.h"
#include "priority_app.h"
#include "mmifmm_interface.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "guianim.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// @Liqing.peng, define the app file task
//file task id
#define   P_APP_COPY            g_app_file_task_id
#define   P_APP_DEL             g_app_file_task_id
#define   P_APP_MOVE            g_app_file_task_id
// stack and queue define
#define   P_APP_FILE_STACK_SIZE              (1024*12)    //12K   bug1351566
#define   P_APP_FILE_QUEUE_NUM               8
#define   P_APP_FILE_TASK_PRIORITY           78   //PRI_APP_FILE_TASK //  (30)bug1349967

#define   APP_COPY_BUFFER_SIZE      MMI_FILE_COPY_BUFFER_MAX_SIZE

#define   APP_COPY_LARG_BUFFER_SIZE   (APP_COPY_BUFFER_SIZE*10)
LOCAL SCI_MUTEX_PTR                  s_filetask_mutex_p = PNULL;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL BOOLEAN s_is_findingfile = FALSE; //是否正在搜索文件
LOCAL volatile BOOLEAN s_is_stopfindfile = FALSE; //是否马上停止搜索文件
LOCAL void* s_current_search_handle = PNULL; //记录当前运行的搜索
LOCAL volatile BOOLEAN s_is_stopcopy = FALSE; //是否马上停止文件复制

#ifdef MMI_FMM_MOVE_SUPPORT
LOCAL BOOLEAN s_move_has_exsit = FALSE;
#endif

//semphore
LOCAL SCI_SEMAPHORE_PTR              s_copy_semphore_p = PNULL;        //复制标志信号量
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
BLOCK_ID g_app_file_task_id = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_Init (void);

/*****************************************************************************/
//  Description : combine current full path name and dest full path name by find file name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CombineCurrentPathAndDestPath (APP_COPY_HANDLE_T *handle_ptr, wchar* find_name);

/*****************************************************************************/
//  Description : start copy a file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E FileCopy (APP_COPY_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description : init copy
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E CopyInit (APP_COPY_HANDLE_T *handle_ptr, uint16 src_name_start);

/*****************************************************************************/
//  Description : find only files
//  Global resource dependence :
//  Author:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E FindOnlyFiles (APP_COPY_HANDLE_T *app_handle_ptr);

/*****************************************************************************/
//  Description : open find only folders
//  Global resource dependence :
//  Author:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E FindOnlyFolders (APP_COPY_HANDLE_T *app_handle_ptr);


/*****************************************************************************/
//  Description : handle app file task signal
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleCopySig (
    APP_DELETE_SIG_T    *psig
);

/*****************************************************************************/
//  Description : the function of the app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_Task (
    uint32 argc,
    void * argv
);

/*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FileDeleteCallback (
    SFS_ERROR_E   error,
    uint32        irp_param,
    uint32        param1,
    uint32        param2
);

/*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetParentFolder (wchar *src_path_ptr, uint16* src_path_len_ptr, wchar *base_path_ptr, uint16 base_path_len);

/*****************************************************************************/
//  Description : handle app file task signal
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleDeleteSig (
    APP_DELETE_SIG_T  *psig
);

#ifdef MMI_FMM_MOVE_SUPPORT
/*****************************************************************************/
//  Description : handle move sig
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleMoveSig (
    APP_DELETE_SIG_T  *psig
);
/*****************************************************************************/
//  Description : 判断在目标路径中是否有与源文件重名的文件
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_IsSrcFileExistInDstPath (const wchar * src_filename_ptr,
        const wchar * dst_full_path_ptr,
        uint16 src_filename_len,
        uint16 dest_full_path_len);
#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION IMPLEMENT                               */
/*---------------------------------------------------------------------------*/
PUBLIC void MMIFMM_SetCopyEnableFlag(
                                  BOOLEAN is_enable//IN:TRUE:enable file copy; FALSE:disable file copy.
                                  )
{
    SCI_GetSemaphore(s_copy_semphore_p, SCI_WAIT_FOREVER);
    s_is_stopcopy = is_enable;
    SCI_TRACE_LOW("[MMIFMM]MMIFMM_SetCopyEnableFlag  is_enable %d", is_enable);
    SCI_PutSemaphore(s_copy_semphore_p);
}


PUBLIC BOOLEAN MMIFMM_GetCopyEnableFlag(void)
{
    BOOLEAN is_enable = FALSE;
    SCI_GetSemaphore(s_copy_semphore_p, SCI_WAIT_FOREVER);
    is_enable = s_is_stopcopy;
    SCI_PutSemaphore(s_copy_semphore_p);
    return is_enable;
}

/*****************************************************************************/
//  Description : create app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_CreateTask (void)
{
    if (0 != g_app_file_task_id)
    {
        //之前创建的task还没有关闭!
        //SCI_TRACE_LOW:"APPFILE_CreateTask, file task hasn't been closed!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_179_112_2_18_2_19_58_153, (uint8*) "");
        return;
    }

    g_app_file_task_id = SCI_CreateAppThread (
                             "T_P_APP_FILE_TASK",
                             "Q_P_APP_FILE_TASK",
                             APPFILE_Task,
                             0,
                             0,
                             P_APP_FILE_STACK_SIZE,
                             P_APP_FILE_QUEUE_NUM,
                             P_APP_FILE_TASK_PRIORITY,
                             SCI_PREEMPT,
                             SCI_AUTO_START);

    if(s_copy_semphore_p == PNULL)
    {
        s_copy_semphore_p = SCI_CreateSemaphore("s_copy_semphore_p", 1);
    }
    //SCI_TRACE_LOW:"APPFILE_CreateTask, g_app_file_task_id = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_195_112_2_18_2_19_58_154, (uint8*) "d", g_app_file_task_id);
}

/*****************************************************************************/
//  Description : improve file task priority
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_ImpoveFileTaskPriority (void)
{
    uint32 state;
    uint32 priorityCur, priorityCurFFS;
    //modified by liyan.zhu for c++ test
    //uint32 old_priority_ptr;
    uint32 preempt;

    if (SCI_SUCCESS != SCI_GetThreadInfo (
                SCI_IdentifyThread() ,
                PNULL,
                PNULL,
                &state,
                &priorityCur,
                &preempt
            )
       )
    {
        SCI_TRACE_LOW ("[MMIFILE] SCI_GetThreadInfo fail");
        return;
    }

    if (SCI_SUCCESS != SCI_GetThreadInfo (
                P_APP_COPY,
                PNULL,
                PNULL,
                &state,
                &priorityCurFFS,
                &preempt
            )
       )
    {
        SCI_TRACE_LOW ("[MMIFILE] SCI_GetThreadInfo filetask fail");
        return;
    }

    if (priorityCur < priorityCurFFS)
    {
        uint32 old_priority_ptr;
        SCI_ChangeThreadPriority (P_APP_COPY, priorityCur, &old_priority_ptr);
    }
}

/*****************************************************************************/
//  Description : Restore file task priority
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_RestoreFileTaskPriority (void)
{
    uint32 ret = 0;
    uint32 old_priority = 0;

    if (0 != g_app_file_task_id)
    {
        ret = SCI_ChangeThreadPriority (P_APP_COPY, P_APP_FILE_TASK_PRIORITY, &old_priority);
    }

    SCI_TRACE_LOW ("[MMIFILE]MMIFILE_RestoreFileTaskPriority old_priority 0x%x ret %d", old_priority, ret);
}
/*****************************************************************************/
//  Description : handle asy search file cnf
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void APPFILE_HandleSearchFileCnf (MMIFMM_SRV_HANDLE handle, BOOLEAN search_result)
{
    s_is_findingfile = FALSE;
    //SCI_TRACE_LOW:"APPFILE_HandleSearchFileCnf search_result=%d,s_is_stopfindfile=%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_204_112_2_18_2_19_58_155, (uint8*) "dd", search_result, s_is_stopfindfile);

    SCI_TRACE_LOW("APPFILE_HandleSearchFileCnf search_handle = 0x%x",handle);
    if (search_result && !s_is_stopfindfile)
    {
        MMI_FindSendSignal_2APP (APP_FIND_FILES_END_IND, handle);
        MMIAPICOM_TriggerMMITask();
    }
}
/*****************************************************************************/
//  Description : init app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_Init (void)
{
    //s_is_findingfile = FALSE;
    //s_is_stopfindfile = FALSE;
    if (PNULL != s_filetask_mutex_p)
    {
        SCI_DeleteMutex (s_filetask_mutex_p);
        s_filetask_mutex_p = PNULL;
    }

    s_filetask_mutex_p = SCI_CreateMutex ("mmi_filetask_mutex", SCI_INHERIT);
    //SCI_ASSERT(PNULL != s_filetask_mutex_p);
}


/*****************************************************************************/
//  Description : the function of the app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_Task (
    uint32 argc,
    void * argv
)
{
    //APP_COPY_SIG_T      *psig = NULL;
    xSignalHeaderRec    *psig = NULL;
    APPFILE_Init();

    while (1) /*lint !e716*/
    {
        MMI_RECEIVE_SIGNAL (psig, SCI_IdentifyThread());/*lint !e63*/
        //psig = s_cur_sig;
        SCI_TRACE_LOW ("[MMIFILE]APPFILE_Task psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/

        switch (psig->SignalCode) /*lint !e613*/
        {
        case APP_COPY_START:
        case APP_COPY_STOP:
        case APP_COPY_CLOSE:
        case APP_COPY_FIND_FILE:
        case APP_COPY_FIND_FOLDER:
        case APP_COPY_FILE:
        case APP_COPY_OVERWRITE:
            //case APP_DEL_FILES:
            APPFILE_HandleCopySig ( (APP_DELETE_SIG_T *) psig);
            break;
        case APP_DEL_FILES:
        case APP_DEL_ONE_FILE_IND:
        case APP_DEL_ONE_FOLDER_IND:
        case APP_DEL_END_IND:
        case APP_DEL_STOP:
            APPFILE_HandleDeleteSig ( (APP_DELETE_SIG_T *) psig);/*lint !e613*/
            break;
        case APP_FIND_FILES:
            {
                BOOLEAN search_result = FALSE;
                APP_DELETE_SIG_T * sig = (APP_DELETE_SIG_T *) psig;
                if (PNULL != sig)
                {
                    //file search
                    //s_is_findingfile = TRUE;
                    s_current_search_handle = (MMIFMM_SRV_HANDLE) sig->data_ptr;
                    if (!s_is_stopfindfile)
                    {
                        search_result = MMIFMM_SearchFileInTask ( (MMIFMM_SRV_HANDLE) sig->data_ptr, &s_is_stopfindfile);/*lint !e605*/
                    }
                    s_current_search_handle = PNULL;
                    APPFILE_HandleSearchFileCnf ( (MMIFMM_SRV_HANDLE) sig->data_ptr, search_result);
                }
            }
            break;

#ifdef MMI_FMM_MOVE_SUPPORT
        case APP_MOVE_FILES:
        case APP_MOVE_CLOSE:
        case APP_MOVE_START:
        case APP_MOVE_STOP:
            APPFILE_HandleMoveSig ( (APP_DELETE_SIG_T *) psig);
            break;
#endif

        default:
            //SCI_TRACE_LOW:"APP_COPY_Task: SigCode unknown"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_288_112_2_18_2_19_58_156, (uint8*) "");
            break;
        }

        if (PNULL != s_filetask_mutex_p)
        {
            SCI_GetMutex (s_filetask_mutex_p, SCI_WAIT_FOREVER);
        }

        MMI_FREE_SIGNAL (psig);

        if (PNULL != s_filetask_mutex_p)
        {
            SCI_PutMutex (s_filetask_mutex_p);
        }
    }

//   if(PNULL != s_filetask_mutex_p)
//   {
//       SCI_DeleteMutex(s_filetask_mutex_p);
//       s_filetask_mutex_p = PNULL;
//   }
//   SCI_ThreadExit();/*lint !e527 */
}

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_AbortCopy (APP_COPY_HANDLE_T *handle_ptr)
{
    BOOLEAN result = FALSE;

    //SCI_TRACE_LOW:"[MMIFILE] APPFILE_AbortCopy "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_317_112_2_18_2_19_58_157, (uint8*) "");
    MMIFMM_SetCopyEnableFlag(FALSE);
    MMI_SendSignal_2APPCOPY ( (uint16) APP_COPY_STOP, handle_ptr);
    return result;
}

/*****************************************************************************/
//  Description : close app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_CopyClose (APP_COPY_HANDLE_T *handle_ptr)
{
    BOOLEAN result = FALSE;

    //SCI_TRACE_LOW:"[MMIFILE] APPFILE_CopyClose "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_330_112_2_18_2_19_58_158, (uint8*) "");
    MMIFMM_SetCopyEnableFlag(FALSE);
    MMI_SendSignal_2APPCOPY (APP_COPY_CLOSE, handle_ptr);
    return result;
}
/*****************************************************************************/
//  Description : start delete file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
APP_DELETE_HANDLE_T *MMIFILE_DeleteOpen (wchar *del_file_name, uint16 del_name_len)
{
    APP_DELETE_HANDLE_T *del_handle_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteOpen enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_342_112_2_18_2_19_58_159, (uint8*) "");

    if (PNULL == del_file_name || 0 == del_name_len || del_name_len > MMIFILE_FULL_PATH_MAX_LEN)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_DeleteOpen del_file_name 0x&x del_name_len %d !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_345_112_2_18_2_19_58_160, (uint8*) "d", del_file_name, del_name_len);
        return PNULL;
    }

    del_handle_ptr = (APP_DELETE_HANDLE_T *) SCI_ALLOC_APP (sizeof (APP_DELETE_HANDLE_T));

    if (PNULL == del_handle_ptr)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_DeleteOpen del_handle_ptr 0x%x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_353_112_2_18_2_19_58_161, (uint8*) "d", del_handle_ptr);
        return PNULL;
    }

    SCI_MEMSET (del_handle_ptr, 0x00, (sizeof (APP_DELETE_HANDLE_T)));

    SCI_MEMCPY (del_handle_ptr->src_full_path, del_file_name, sizeof (wchar) *del_name_len);
    del_handle_ptr->src_path_len = del_name_len;
    MMI_SendSignal_2APPDel (APP_DEL_FILES, del_handle_ptr);

    return del_handle_ptr;
}


/*****************************************************************************/
//  Description : close delete handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteClose (APP_DELETE_HANDLE_T *del_handle)
{

    MMIFILE_ERROR_E fs_ret = SFS_NO_ERROR;

    if (PNULL == del_handle)
    {
        fs_ret = SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        fs_ret = MMIAPIFMM_FindClose (del_handle->find_handle);
        SCI_FREE (del_handle);
    }

    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteClose fs_ret %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_384_112_2_18_2_19_58_162, (uint8*) "d", fs_ret);
    return fs_ret;

}
/*****************************************************************************/
//  Description : start delete file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN MMIFILE_AbortDelete (APP_DELETE_HANDLE_T * del_handle_ptr)
{
    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_AbortDelete enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_394_112_2_18_2_19_58_163, (uint8*) "");

    if (PNULL == del_handle_ptr)
    {
        //SCI_TRACE_LOW:"MMIFILE_AbortDelete del_handle_ptr 0x&x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_398_112_2_18_2_19_59_164, (uint8*) "", del_handle_ptr);
        return FALSE;
    }

    MMI_SendSignal_2APPDel (APP_DEL_STOP, del_handle_ptr);
    return TRUE;
}

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_StopFindFile (void)
{
    s_is_stopfindfile = TRUE;
}

/*****************************************************************************/
//  Description : set s_is_stopfindfile = FALSE
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_SetStartFindFileFlag (void)
{
    s_is_findingfile = TRUE;
    s_is_stopfindfile = FALSE;
}

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_IsFindingFile (void)
{
    return (s_is_findingfile);
}

/*****************************************************************************/
//  Description : handle app file task signal
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleDeleteSig (
    APP_DELETE_SIG_T  *psig
)
{
    APP_DELETE_HANDLE_T *app_handle_ptr = PNULL;
    //modified by liyan.zhu for c++ test
    //SFS_FIND_DATA_T     *finddate_ptr = PNULL;
    MMIFILE_ERROR_E      sfs_ret = SFS_ERROR_NONE;

    //uint16              result = APP_COPY_RESULT_NO_ERROR;
    //LOCAL BOOLEAN        s_is_user_stop = FALSE;
    switch (psig->SignalCode) /*lint !e613*/
    {
    case APP_DEL_FILES:
        //start delete
        //SCI_TRACE_LOW:"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_FILES enter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_451_112_2_18_2_19_59_165, (uint8*) "");
        app_handle_ptr = (APP_DELETE_HANDLE_T *) psig->data_ptr; /*lint !e613*/
        app_handle_ptr->user_stopped = FALSE;

        if (app_handle_ptr->src_path_len > APP_COPY_FULLPATH_LEN)
        {
            sfs_ret = SFS_ERROR_INVALID_PARAM;
        }
        else
        {
            MMIAPICOM_Wstrncpy (app_handle_ptr->delete_full_path, app_handle_ptr->src_full_path, app_handle_ptr->src_path_len);
            app_handle_ptr->delete_path_len = app_handle_ptr->src_path_len;
            sfs_ret = MMIFILE_DeleteDirTree (app_handle_ptr);
        }

        if (SFS_ERROR_NONE != sfs_ret)
        {
            //delete error
            app_handle_ptr->file_ret = sfs_ret;
            MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
        }

        break;
    case APP_DEL_ONE_FILE_IND:
        //delete one file call back

        app_handle_ptr = (APP_DELETE_HANDLE_T *) psig->data_ptr; /*lint !e613*/
        
        if (PNULL == app_handle_ptr)
        {
            //parameter error
            sfs_ret = SFS_ERROR_INVALID_PARAM;
        }
        else
        {
			//coverity 10346
			//SCI_TRACE_LOW:"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FILE_IND user stop %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_477_112_2_18_2_19_59_166, (uint8*) "d", app_handle_ptr->user_stopped);
            if (app_handle_ptr->user_stopped)
            {
                //user stop
                MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
                break;
            }

            //delete next
            if (0 != app_handle_ptr->find_handle)
            {
                //delete next
                if (SFS_ERROR_NONE != app_handle_ptr->file_ret)
                {
                    //delete callback error
                    //SCI_TRACE_LOW("[baokun]3");
                    MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
                }
                else
                {
                    SFS_FIND_DATA_T     *finddate_ptr = PNULL;
                    //delete callback delete next
                    finddate_ptr = (SFS_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (SFS_FIND_DATA_T));

                    if (PNULL != finddate_ptr)
                    {
                        sfs_ret = SFS_FindNextFile (app_handle_ptr->find_handle, finddate_ptr);

                        if (SFS_ERROR_NOT_EXIST == sfs_ret)
                        {
                            SFS_FindClose (app_handle_ptr->find_handle);
                            app_handle_ptr->find_handle = 0;
                            //find its sub folder
                            sfs_ret = MMIFILE_DeleteSubFolder (app_handle_ptr);//MMIFILE_DeleteDirTree(app_handle_ptr);
                        }
                        else
                        {
                            //delet its next file
                            sfs_ret = MMIFILE_DeleteSubFileAsyn (app_handle_ptr, (wchar *) finddate_ptr->name, MMIAPICOM_Wstrlen (finddate_ptr->name));

                        }

                        SCI_FREE (finddate_ptr);
                    }
                    else
                    {
                        //error, stop delete
                        SFS_FindClose (app_handle_ptr->find_handle);
                        app_handle_ptr->find_handle = 0;
                        sfs_ret = SFS_ERROR_NO_SPACE;
                    }

                    if (SFS_ERROR_NONE != sfs_ret)
                    {
                        app_handle_ptr->file_ret = sfs_ret;
                        //SCI_TRACE_LOW("[baokun]4");
                        MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
                    }
                }//delete callback delete next
            }//delete next
            else
            {
                //delete end
                //SCI_TRACE_LOW("[baokun]5");
                MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
            }
        }//(PNULL != app_handle_ptr)

        break;
    case APP_DEL_ONE_FOLDER_IND:
        //delete next folder
        app_handle_ptr = (APP_DELETE_HANDLE_T *) psig->data_ptr;/*lint !e613*/
        
        if (PNULL != app_handle_ptr)
        {
			//coverity 10346
			//SCI_TRACE_LOW:"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FOLDER_IND user stop %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_549_112_2_18_2_19_59_167, (uint8*) "d", app_handle_ptr->user_stopped);
            if (app_handle_ptr->user_stopped)
            {
                //user stop
                MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
                break;
            }

            if (SFS_ERROR_NONE != app_handle_ptr->file_ret)
            {
                //delete callback error
                //SCI_TRACE_LOW("[baokun]6");
                MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
                break;
            }

            sfs_ret = MMIFILE_DeleteDirTree (app_handle_ptr);

            if (SFS_ERROR_NONE != sfs_ret)
            {
                //delete dir error
                app_handle_ptr->file_ret = sfs_ret;
                //SCI_TRACE_LOW("[baokun]7");
                MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
            }
        }
        else
        {
            //parameter error
            //SCI_TRACE_LOW("[baokun]8");
            MMI_SendSignal_2APPDel (APP_DEL_END_IND, app_handle_ptr);
        }

        break;
    case APP_DEL_STOP:
        //user stop delete
        app_handle_ptr = (APP_DELETE_HANDLE_T *) psig->data_ptr;/*lint !e613*/

        if (PNULL != app_handle_ptr)
        {//coverity 10346
			//SCI_TRACE_LOW:"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_STOP user stop %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_585_112_2_18_2_19_59_168, (uint8*) "d", app_handle_ptr->user_stopped);
            app_handle_ptr->user_stopped = TRUE;
            app_handle_ptr->file_ret = SFS_ERROR_ABORT;
        }

        //SCI_TRACE_LOW("[baokun]9");
        //MMI_SendSignal_2APPDel(APP_DEL_END_IND, app_handle_ptr);
        break;
    case APP_DEL_END_IND:
        //delete end
        app_handle_ptr = (APP_DELETE_HANDLE_T *) psig->data_ptr; /*lint !e613*/

        if (PNULL != app_handle_ptr) /*lint !e613*/
        {
            sfs_ret = app_handle_ptr->file_ret;
			//coverity 12492
			SCI_TRACE_LOW ("[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_END_IND user stop %d", app_handle_ptr->user_stopped);/*lint !e613*/
        }
        else
        {
            sfs_ret = SFS_ERROR_INVALID_PARAM;
        }

        //MMIFILE_DeleteClose(app_handle_ptr);
        MMI_SendSignal_2APP (APP_DEL_DIR_END_IND, (uint16) sfs_ret);
        break;
    default:
        SCI_TRACE_LOW ("APP APPFILE_HandleDeleteSig code 0x%x", psig->SignalCode);/*lint !e613*/
        break;
    }
}
/*****************************************************************************/
//  Description : handle app file task signal
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleCopySig (
    APP_DELETE_SIG_T    *psig
)
{
//     uint16               result = APP_COPY_RESULT_ERROR;
    APP_COPY_HANDLE_T    *copy_handle_ptr = PNULL;
//  MMIFILE_HANDLE       file_handle = PNULL;
    SFS_FIND_DATA_T      find_data = {0};
    SFS_ERROR_E          file_ret = SFS_ERROR_NONE;
//  uint16               find_name_len = 0;
    uint16               slash_index = 0;
    uint16               path_depth = 0;

    //modified by liyan.zhu for c++ test
    //BOOLEAN              is_exist = TRUE;
    //SCI_ASSERT(NULL != psig);
    if (PNULL == psig)
    {
        //SCI_TRACE_LOW:"APPFILE_HandleCopySig param error:PNULL == psig"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_633_112_2_18_2_19_59_169, (uint8*) "");
        return;
    }

    copy_handle_ptr = (APP_COPY_HANDLE_T *) psig->data_ptr;

    switch (psig->SignalCode)
    {
    case APP_COPY_START:

        //收到copy文件消息
        //copy_handle_ptr = (APP_COPY_HANDLE_T *)psig->data_ptr;
        if (PNULL == copy_handle_ptr)
        {
            //send fail to app
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_INVALID_PARAM);
        }
        else if (copy_handle_ptr->user_stopped)
        {
            //copy be stopped
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_ABORT);
        }
        else
        {
            //perpare to copy
            copy_handle_ptr->is_first_find_folder = TRUE;
            SCI_MEMSET (copy_handle_ptr->findfile_offset, 0x00, sizeof (copy_handle_ptr->findfile_offset));
            copy_handle_ptr->root_src_path_len = copy_handle_ptr->src_full_path_len;
            //copy_handle_ptr->root_dest_path_len = copy_handle_ptr->dest_full_path_len;
            SCI_MEMSET (copy_handle_ptr->current_full_path, 0x00, sizeof (copy_handle_ptr->current_full_path));
            copy_handle_ptr->current_full_path_len = copy_handle_ptr->src_full_path_len;
            MMIAPICOM_Wstrncpy (copy_handle_ptr->current_full_path, copy_handle_ptr->src_full_path, copy_handle_ptr->src_full_path_len);
            slash_index = MMIFMM_GetFinallySlashIndex (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len); /*lint !e718 !e746*/
            slash_index ++;
            path_depth = MMIFMM_GetFullPathDepth (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);/*lint !e718 !e746*/
            copy_handle_ptr->findfile_offset[path_depth] = 0;

            if (slash_index >= MMIFILE_FULL_PATH_MAX_LEN || slash_index >= copy_handle_ptr->current_full_path_len)
            {
                //src path fail
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_LONG_DEPTH);
                break;
            }

            if (copy_handle_ptr->dest_full_path_len + copy_handle_ptr->current_full_path_len - slash_index >= MMIFILE_FULL_PATH_MAX_LEN)
            {
                //dest path too long
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_LONG_DEPTH);
                break;
            }

            //combine dest file or folder name
            file_ret = CopyInit (copy_handle_ptr, slash_index);

            if (SFS_ERROR_NONE != file_ret)
            {
                //fail
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) file_ret);
            }
        }

        break;
    case APP_COPY_FIND_FILE:

        //find file
        if (PNULL == copy_handle_ptr || copy_handle_ptr->user_stopped)
        {
            //copy be stopped
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_ABORT);
            break;
        }

        copy_handle_ptr->is_first_find_folder = TRUE;

        if (0 == copy_handle_ptr->find_handle)
        {
            //find file not started, start find file
            file_ret = FindOnlyFiles (copy_handle_ptr);

            if (SFS_ERROR_NONE == file_ret)
            {
#ifdef DRM_SUPPORT

                //omit drm file
                if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, copy_handle_ptr->current_full_path))

                {
					uint32 drm_file_type = 0;
					uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1] = {0};
					uint16 suffix_len                         = MMIFMM_FILENAME_LEN;
					
					MMIAPIFMM_SplitFileName(copy_handle_ptr->current_full_path, MMIAPICOM_Wstrlen(copy_handle_ptr->current_full_path), PNULL, PNULL, suffix_wstr, &suffix_len);
					drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);
					
					//FL CD类型的DRM文件不允许复制,SD类型的仅允许复制媒体文件，共享版权信息
					if(drm_file_type != MMIFMM_DCF_FILE)
					{
						//如果只有当个文件且是DRM文件，则直接提示并返回，不让copy
						MMI_SendSignal_2APPCOPY(APP_COPY_FIND_FILE, copy_handle_ptr);
						break;     
					}
                }

#endif
                //send APP_COPY_FIND_FILE
                copy_handle_ptr->copy_offset = 0;
                MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
                //MMI_SendSignal_2APPCOPY(APP_COPY_FIND_FILE, copy_handle_ptr);
            }
            else if (SFS_ERROR_INVALID_PARAM == file_ret || SFS_ERROR_NOT_EXIST == file_ret)
            {
                //no file in it, find folder in it
                MMI_SendSignal_2APPCOPY (APP_COPY_FIND_FOLDER, copy_handle_ptr);
            }
            else
            {
                //copy fail or dest name too long,end it
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) file_ret);
            }
        }
        else
        {
            //find files in src path
            if (SFS_ERROR_NONE == SFS_FindNextFile (copy_handle_ptr->find_handle, &find_data))
            {

                //find a file copy it to dest folder
                slash_index = MMIFMM_GetFinallySlashIndex (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);
                //slash_index ++;
                copy_handle_ptr->current_full_path[slash_index] = 0;
                copy_handle_ptr->current_full_path_len = MMIAPICOM_Wstrlen (copy_handle_ptr->current_full_path);

                if (CombineCurrentPathAndDestPath (copy_handle_ptr, find_data.name))
                {
#ifdef DRM_SUPPORT

                    //omit drm file
                    if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, copy_handle_ptr->current_full_path))
                    {
                        uint32 drm_file_type = 0;
                        uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1] = {0};
                        uint16 suffix_len                         = MMIFMM_FILENAME_LEN;
						
                        MMIAPIFMM_SplitFileName(copy_handle_ptr->current_full_path, MMIAPICOM_Wstrlen(copy_handle_ptr->current_full_path), PNULL, PNULL, suffix_wstr, &suffix_len);
                        drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);
						
                        //FL CD类型的DRM文件不允许复制,SD类型的仅允许复制媒体文件，共享版权信息
                        if(drm_file_type != MMIFMM_DCF_FILE)
                        {
                            //如果只有当个文件且是DRM文件，则直接提示并返回，不让copy
                            MMI_SendSignal_2APPCOPY(APP_COPY_FIND_FILE, copy_handle_ptr);
                            break;     
                        }
                    }

#endif
                    //copy file
                    copy_handle_ptr->copy_offset = 0;
                    MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
                }
                else
                {
                    //copy fail
                    MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_LONG_DEPTH);
                    SFS_FindClose (copy_handle_ptr->find_handle);
                    copy_handle_ptr->find_handle = 0;
                }
            }
            else
            {
                //if no find, close this folder send find folder in path
                MMIAPIFMM_FindClose (copy_handle_ptr->find_handle);
                copy_handle_ptr->find_handle = 0;
                //remove file name and combine current folder
                slash_index = MMIFMM_GetFinallySlashIndex (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);
                copy_handle_ptr->current_full_path[slash_index] = 0;
                copy_handle_ptr->current_full_path_len = MMIAPICOM_Wstrlen (copy_handle_ptr->current_full_path);
                MMI_SendSignal_2APPCOPY (APP_COPY_FIND_FOLDER, copy_handle_ptr);
            }

            //copy this file dest path
        }

        break;
    case APP_COPY_FILE:

        //copy_handle_ptr = (APP_COPY_HANDLE_T *)psig->data_ptr;
        if (PNULL == copy_handle_ptr || copy_handle_ptr->user_stopped)
        {
            //copy be stopped
            if (PNULL != copy_handle_ptr)
            {
                MMIAPIFMM_DeleteFile (copy_handle_ptr->dest_full_path, PNULL);
            }

            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_ABORT);
            break;
        }

        copy_handle_ptr->sfs_error = FileCopy (copy_handle_ptr);

        if (SFS_ERROR_IO_PENDING == copy_handle_ptr->sfs_error)
        {
            //copy not finished
            MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
        }
        else if (SFS_ERROR_NONE == copy_handle_ptr->sfs_error || SFS_ERROR_HAS_EXIST == copy_handle_ptr->sfs_error)
        {
            //find next file to copy
            copy_handle_ptr->copy_offset = 0;
            MMI_SendSignal_2APPCOPY (APP_COPY_FIND_FILE, copy_handle_ptr);
        }
        else
        {
            //copy fail
            MMIAPIFMM_FindClose (copy_handle_ptr->find_handle);
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) copy_handle_ptr->sfs_error);

        }

        break;

    case APP_COPY_OVERWRITE:

        //start copy
        if (PNULL == copy_handle_ptr)
        {
            //invalid parament
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_INVALID_PARAM);
        }
        else if (copy_handle_ptr->user_stopped)
        {
            //copy be stopped
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_ABORT);
        }
        else
        {
            //copy file
            copy_handle_ptr->copy_offset = 0;
            SCI_MEMSET (copy_handle_ptr->findfile_offset, 0x00, sizeof (copy_handle_ptr->findfile_offset));
            MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
        }

        break;

    case APP_COPY_FIND_FOLDER:

        //find folder
        //copy_handle_ptr = (APP_COPY_HANDLE_T *)psig->data_ptr;
        if (PNULL == copy_handle_ptr || copy_handle_ptr->user_stopped)
        {
            //copy be stopped
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_ABORT);
            break;
        }

        SCI_MEMSET (&find_data, 0x00, sizeof (find_data));

        if (0 == copy_handle_ptr->find_handle)
        {
            //find folder not started, start find folder
            file_ret = FindOnlyFolders (copy_handle_ptr);
            copy_handle_ptr->current_folder_index = 0;

            if (SFS_ERROR_NONE == file_ret)
            {
                //exist sub folder in it, start sub folder copy
                path_depth = MMIFMM_GetFullPathDepth (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);

                if (!copy_handle_ptr->is_first_find_folder)
                {
                    //not first enter this folder
                    if (copy_handle_ptr->current_folder_index == copy_handle_ptr->findfile_offset[path_depth])
                    {
                        //this folder not be create, create folder
                        SFS_FindClose (copy_handle_ptr->find_handle);
                        copy_handle_ptr->find_handle = 0;
                        copy_handle_ptr->copy_offset = 0;
                        copy_handle_ptr->findfile_offset[path_depth]++;
                        MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
                    }
                    else
                    {
                        //this sub folder already copied, find next folder
                        copy_handle_ptr->current_folder_index++ ;
                        MMI_SendSignal_2APPCOPY (APP_COPY_FIND_FOLDER, copy_handle_ptr);
                    }

                }
                else
                {
                    //first enter this folder, create its sub folder directly
                    copy_handle_ptr->findfile_offset[path_depth]++;
                    copy_handle_ptr->current_folder_index ++;
                    SFS_FindClose (copy_handle_ptr->find_handle);
                    copy_handle_ptr->find_handle = 0;
                    copy_handle_ptr->copy_offset = 0;
                    MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
                }
            }
            else if (SFS_ERROR_INVALID_PARAM == file_ret || SFS_ERROR_NOT_EXIST == file_ret)
            {
                //copy end
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_NONE);
            }
            else
            {
                //fail
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) file_ret);
            }
        }
        else
        {
            BOOLEAN              is_exist = TRUE;
            //find this folder's sub folder
            //is_exist = TRUE;
            file_ret = SFS_ERROR_NONE;
            slash_index = MMIFMM_GetFinallySlashIndex (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);

            while (SFS_ERROR_NONE == SFS_FindNextFile (copy_handle_ptr->find_handle, &find_data))
            {
                copy_handle_ptr->current_full_path[slash_index] = 0;
                copy_handle_ptr->current_full_path_len = MMIAPICOM_Wstrlen (copy_handle_ptr->current_full_path);

                if (CombineCurrentPathAndDestPath (copy_handle_ptr, find_data.name))
                {
                    path_depth = MMIFMM_GetFullPathDepth (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);

                    if (!copy_handle_ptr->is_first_find_folder)
                    {
                        //update to parent
                        if (copy_handle_ptr->current_folder_index == copy_handle_ptr->findfile_offset[path_depth])
                        {
                            //this folder not be create, create folder
                            copy_handle_ptr->findfile_offset[path_depth]++;
                            is_exist = FALSE;
                            break;
                        }
                        else
                        {
                            copy_handle_ptr->current_folder_index++ ;
                        }

                    }
                    else
                    {
                        copy_handle_ptr->findfile_offset[path_depth]++;
                        copy_handle_ptr->current_folder_index++ ;
                        break;
                    }

                }
                else
                {
                    //dest name too long
                    file_ret = SFS_ERROR_LONG_DEPTH;
                    break;
                }
            }

            SFS_FindClose (copy_handle_ptr->find_handle);
            copy_handle_ptr->find_handle = 0;

            if (!is_exist)
            {
                //create folder
                copy_handle_ptr->copy_offset = 0;
                MMI_SendSignal_2APPCOPY (APP_COPY_FILE, copy_handle_ptr);
            }
            else if (SFS_ERROR_NONE == file_ret)
            {
                //get it's parent folder to find file
                //MMI_SendSignal_2APPCOPY(APP_COPY_FIND_FOLDER, copy_handle_ptr);
                slash_index = MMIFMM_GetFinallySlashIndex (copy_handle_ptr->current_full_path, copy_handle_ptr->current_full_path_len);
                //slash_index ++;
                copy_handle_ptr->current_full_path[slash_index] = 0;
                copy_handle_ptr->current_full_path_len = MMIAPICOM_Wstrlen (copy_handle_ptr->current_full_path);

                if (copy_handle_ptr->current_full_path_len == copy_handle_ptr->src_full_path_len
                        || (!GetParentFolder (copy_handle_ptr->current_full_path,
                                              &copy_handle_ptr->current_full_path_len,
                                              copy_handle_ptr->src_full_path,
                                              copy_handle_ptr->src_full_path_len))

                   )
                {
                    //root src path no folder to copy
                    //copy end
                    MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_NONE);
                }
                else
                {
                    //find its parent folder
                    copy_handle_ptr->is_first_find_folder = FALSE;
                    MMI_SendSignal_2APPCOPY (APP_COPY_FIND_FOLDER, copy_handle_ptr);
                }
            }
            else
            {
                //copy fail or dest name too long
                MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) file_ret);
            }
        }

        break;
    case APP_COPY_STOP:

        //收到中止copy消息
        //copy_handle_ptr = (APP_COPY_HANDLE_T *)psig->data_ptr;
        if (PNULL != copy_handle_ptr)
        {
            copy_handle_ptr->user_stopped = TRUE;
        }
        else
        {
            MMI_SendSignal_2APP (APP_COPY_END_IND, (uint16) SFS_ERROR_INVALID_PARAM);
        }

        break;

    case APP_COPY_CLOSE://copy finished, free resource
        //copy_handle_ptr = (APP_COPY_HANDLE_T *)psig->data_ptr;
        if (PNULL != copy_handle_ptr)
        {
            SCI_FREE (copy_handle_ptr);
        }

        break;
    default:
        //SCI_TRACE_LOW:"APP_COPY_Task: SigCode unknown"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1000_112_2_18_2_20_0_170, (uint8*) "");
        break;
    }
}

/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPDel (uint16      sig_id, APP_DELETE_HANDLE_T *handle_ptr)
{
    APP_DELETE_SIG_T *sig_ptr = PNULL;

    MMI_CREATE_SIGNAL (sig_ptr,
                       sig_id,
                       sizeof (APP_DELETE_SIG_T),
                       P_APP
                      );
    sig_ptr->data_ptr = handle_ptr;
    MMI_SEND_SIGNAL (sig_ptr, P_APP_DEL);
}

/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPCOPY (
    uint16              sig_id,
    APP_COPY_HANDLE_T *handle_ptr
)
{
    APP_DELETE_SIG_T*sig_ptr = PNULL;

    MMI_CREATE_SIGNAL (sig_ptr,
                       sig_id,
                       sizeof (APP_DELETE_SIG_T),
                       P_APP
                      );
    sig_ptr->data_ptr = handle_ptr;

    MMI_SEND_SIGNAL (sig_ptr, P_APP_COPY)
}

/*****************************************************************************/
//  Description : 发送消息给app task
//  Global resource dependence :
//  Author:
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APP (uint16   sig_id, uint16 data)
{
    MmiAppCopyMsgS *sig_ptr = PNULL;

    MmiCreateSignal (sig_id, sizeof (MmiAppCopyMsgS), (MmiSignalS**)&sig_ptr);

    sig_ptr->SignalSize = sizeof (MmiAppCopyMsgS);
    sig_ptr->Sender = P_APP;
    sig_ptr->data = data;

    MMI_SEND_SIGNAL (sig_ptr, P_APP)
}
/*****************************************************************************/
//  Description : 发送消息给app task
//  Global resource dependence :
//  Author:
//  Note:
//  Return:
/*****************************************************************************/
void MMI_FindSendSignal_2APP (uint16   sig_id, void* data)
{
    MmiOtherTaskMsgS *sig_ptr = PNULL;

    MmiCreateSignal (sig_id, sizeof (MmiOtherTaskMsgS), (MmiSignalS**)&sig_ptr);

    sig_ptr->SignalSize = sizeof (MmiOtherTaskMsgS);
    sig_ptr->Sender = P_APP;
    sig_ptr->handle = data; /*lint !e64*/

    MMI_SEND_SIGNAL (sig_ptr, P_APP)
}

/*****************************************************************************/
//  Description :get current find handle
//  Global resource dependence :
//  Author:
//  Return:
/*****************************************************************************/
PUBLIC void* APPFILE_GetCurrentFindHandle(void)
{
    return s_current_search_handle;
}

/*****************************************************************************/
//  Description : delete dir tree
//  Global resource dependence :
//  Author:
//  Note: if dir_path_ptr is file, delete it dircty,
//        if dir_path_ptr is directory, first delete its all sub folders, then delete itself.
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteDirTree (APP_DELETE_HANDLE_T *app_del_handle_ptr)
{
    MMIFILE_FIND_DATA_T *find_data_ptr = PNULL;
    //modified by liyan.zhu for c++ test
    //uint16              find_name_len = 0;
    SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
    //uint16              ctlStr[] = L"/A:+NF-NF";//find file not include folder
    //uint16              filter[] = L"*";//all files
//  SFS_OVERLAPPED_T    overLapped = {0};
    // 查找第一个数据
    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteDirTree enter "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1077_112_2_18_2_20_0_171, (uint8*) "");

    if (PNULL == app_del_handle_ptr)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    find_data_ptr = (MMIFILE_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (MMIFILE_FIND_DATA_T));

    if (PNULL == find_data_ptr)
    {
        sfs_ret = SFS_ERROR_NO_SPACE;
    }
    else
    {
        uint16              ctlStr[] = L"/A:+NF-NF";//find file not include folder
        uint16              filter[] = L"*";//all files
        sfs_ret = SFS_FindFirstFileCtrl (app_del_handle_ptr->delete_full_path,
                                         filter,
                                         ctlStr,
                                         &app_del_handle_ptr->find_handle,
                                         find_data_ptr,
                                         PNULL
                                        );

        if (0 == app_del_handle_ptr->find_handle || SFS_ERROR_NONE != sfs_ret)
        {
            //dir_path_ptr is a file or not file in this folder
            if (0 != app_del_handle_ptr->find_handle)
            {
                SFS_FindClose (app_del_handle_ptr->find_handle);
                app_del_handle_ptr->find_handle = 0;
            }

            if (SFS_ERROR_NOT_EXIST != sfs_ret)
            {
                //delete this file
                sfs_ret = MMIFILE_DeleteSubFileAsyn (app_del_handle_ptr, PNULL, 0);

            }
            else
            {
                //find its sub folders
                sfs_ret = MMIFILE_DeleteSubFolder (app_del_handle_ptr);
            }
        }
        else
        {
            uint16              find_name_len = 0;
            //exist file in it, delete find file
            find_name_len = MMIAPICOM_Wstrlen (find_data_ptr->name);

            if (find_name_len + app_del_handle_ptr->delete_path_len < MMIFILE_FULL_PATH_MAX_LEN)
            {
                sfs_ret = MMIFILE_DeleteSubFileAsyn (app_del_handle_ptr, find_data_ptr->name, find_name_len);
            }
            else
            {
                //file name too long, stop delete operation
                SFS_FindClose (app_del_handle_ptr->find_handle);
                app_del_handle_ptr->find_handle = 0;
                sfs_ret = SFS_ERROR_LONG_DEPTH;
            }
        }

        SCI_FREE (find_data_ptr);
    }

    //delete its sub folde or file


    return sfs_ret;
}


/*****************************************************************************/
//  Description : delete sub file
//  Global resource dependence :
//  Author:
//  Note: this file or folder must no sub foler or file
//
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteSubFileAsyn (APP_DELETE_HANDLE_T *del_handle_ptr,
        wchar* file_name,
        uint16 file_name_len)
{
    MMIFILE_ERROR_E  file_ret = SFS_ERROR_NONE;
    //modified by liyan.zhu for c++ test
    //SFS_OVERLAPPED_T overLapped = {0};
    //wchar            *full_path_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteSubFileAsyn enter "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1158_112_2_18_2_20_0_172, (uint8*) "");

    if (PNULL == del_handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x file_name 0x%x file_name_len %d !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1162_112_2_18_2_20_0_173, (uint8*) "ddd", del_handle_ptr, file_name, file_name_len);
        return SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        SFS_OVERLAPPED_T overLapped = {0};
        wchar            *full_path_ptr = PNULL;
        //delete file
        overLapped.param = (uint32) del_handle_ptr;
        overLapped.complete_route = FileDeleteCallback;
        full_path_ptr = (wchar *) SCI_ALLOC_APP (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));

        if (PNULL != full_path_ptr)
        {
            SCI_MEMSET (full_path_ptr,  0x00, (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1)));

            //delete file path
            if (del_handle_ptr->delete_path_len <=  MMIFILE_FULL_PATH_MAX_LEN)
            {
                MMIAPICOM_Wstrncpy (full_path_ptr,
                                    del_handle_ptr->delete_full_path,
                                    del_handle_ptr->delete_path_len);

                
                //delete file full path
                if (file_name_len > 0 && (PNULL != file_name))
                {
                    if (del_handle_ptr->delete_path_len + 1 + file_name_len <= MMIFILE_FULL_PATH_MAX_LEN)
                    {
                        full_path_ptr[del_handle_ptr->delete_path_len] = MMIFILE_SLASH;
                        MMIAPICOM_Wstrncpy (&full_path_ptr[del_handle_ptr->delete_path_len + 1],
                                            file_name,
                                            file_name_len);
                    }
                    else
                    {
                        //filename too long
                        file_ret = SFS_ERROR_LONG_DEPTH;
                    }
                }
                //delete the miniature file
                if (MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_GetFileTypeByFileName (full_path_ptr, MMIAPICOM_Wstrlen(full_path_ptr)))
                {
                    GUIANIM_DeleteMiniatureFile (full_path_ptr, MMIAPICOM_Wstrlen(full_path_ptr));
                }
                if (file_ret == SFS_ERROR_NONE)
                {
#if defined(DRM_SUPPORT) && defined(MMI_FMM_MOVE_SUPPORT)

                    if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, full_path_ptr)
                            && MMIFMM_IsCopyFromMove()) //drm文件不能移动
                    {
                        del_handle_ptr->file_ret = SFS_ERROR_NONE;
                        del_handle_ptr->user_stopped = FALSE;
                        MMI_SendSignal_2APPDel (APP_DEL_ONE_FILE_IND, del_handle_ptr);
                        return SFS_ERROR_NONE;
                    }

#endif
                    file_ret = SFS_DeleteFile (full_path_ptr, &overLapped);

                    if (SFS_ERROR_IO_PENDING == file_ret)
                    {
                        //SCI_TRACE_LOW:"MMIFILE_DeleteSubFileAsyn: delete asyn success"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1212_112_2_18_2_20_0_174, (uint8*) "");
                        file_ret = SFS_ERROR_NONE;
                    }
                    else
                    {
                        file_ret = SFS_ERROR_ACCESS;
                    }
                }
            }
            else
            {
                //file name too long
                //SCI_TRACE_LOW:"MMIFILE_DeleteSubFileAsyn file name too long !"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1224_112_2_18_2_20_0_175, (uint8*) "");
                file_ret = SFS_ERROR_LONG_DEPTH;
            }

            SCI_FREE (full_path_ptr);
        }
        else
        {
            file_ret = SFS_ERROR_ACCESS;
        }

    }

//    if(SFS_ERROR_NONE != file_ret)
//  {
//      //delete error, stop delete
//      SCI_FREE(del_handle_ptr);
//      SendSignal_2APP(APP_DEL_END_IND, result);
//  }
    return file_ret;

}

/*****************************************************************************/
//  Description : find first sub folder,and delet
//  Global resource dependence :
//  Author:
//  Note: find first sub folder, if its first folder is empty delet its first folder
//        if no sub folder to find, delete this folder itself,
//        if find a sub folder and this sub folder not empty, close this folder and
//           enter this sub folder to find its sub folder's sub folder
//
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteSubFolder (APP_DELETE_HANDLE_T *del_handle_ptr)
{
    MMIFILE_ERROR_E  file_ret = SFS_ERROR_NONE;
    //uint16           result = APP_COPY_RESULT_SUCCESS;
    //wchar            *full_path_ptr = PNULL;
    //modified by liyan.zhu for c++ test
    //MMIFILE_FIND_DATA_T *find_data_ptr = PNULL;
    //uint16              find_name_len = 0;
    //uint16           ctlStr[] = L"/A:+NF-F";//find folder
    //uint16           filter[] = L"*";//all files
    //SFS_OVERLAPPED_T overlapped = {0};
    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteSubFolder enter "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1263_112_2_18_2_20_0_176, (uint8*) "");

    if (PNULL == del_handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1266_112_2_18_2_20_0_177, (uint8*) "d", del_handle_ptr);
        return SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        MMIFILE_FIND_DATA_T *find_data_ptr = PNULL;
        uint16              find_name_len = 0;
        uint16           ctlStr[] = L"/A:+NF-F";//find folder
        uint16           filter[] = L"*";//all files
        //delete file
        find_data_ptr = (MMIFILE_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (MMIFILE_FIND_DATA_T));

        //SCI_ASSERT(PNULL != find_data_ptr);
        if (PNULL == find_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIFILE_DeleteSubFolder alloc find_data_ptr error:PNULL == find_data_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1276_112_2_18_2_20_0_178, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        file_ret = SFS_FindFirstFileCtrl (del_handle_ptr->delete_full_path,
                                          filter,
                                          ctlStr,
                                          &del_handle_ptr->find_handle,
                                          find_data_ptr,
                                          PNULL);
        SFS_FindClose (del_handle_ptr->find_handle);
        del_handle_ptr->find_handle = 0;

        if (SFS_ERROR_NOT_EXIST == file_ret)
        {
            //this folder is empty, delete it
            if (2 == del_handle_ptr->delete_path_len) //root folder
            {
                file_ret = SFS_ERROR_NONE;
            }
            else
            {
                file_ret = MMIAPIFMM_DeleteDirectory (del_handle_ptr->delete_full_path);
            }

            if (SFS_ERROR_NONE == file_ret)
            {
                //delete ok
                if (GetParentFolder (del_handle_ptr->delete_full_path,
                                     &del_handle_ptr->delete_path_len,
                                     del_handle_ptr->src_full_path,
                                     del_handle_ptr->src_path_len))
                {
                    //delete its upper folder
                    MMI_SendSignal_2APPDel (APP_DEL_ONE_FOLDER_IND, del_handle_ptr);
                }
                else
                {
                    //delete end, source path and its all sub file and folder be deleted
                    //SCI_TRACE_LOW("[baokun]1");
                    MMI_SendSignal_2APPDel (APP_DEL_END_IND, del_handle_ptr);
                }
            }
        }
        else
        {
            //combine it sub folder
            find_name_len = (uint16) MMIAPICOM_Wstrlen (find_data_ptr->name);

            if (del_handle_ptr->delete_path_len + find_name_len < MMIFILE_FULL_PATH_MAX_LEN)
            {
                //combine sub folder full path name
                del_handle_ptr->delete_full_path[del_handle_ptr->delete_path_len] = MMIFILE_SLASH;
                del_handle_ptr->delete_path_len ++;
                MMIAPICOM_Wstrncpy (&del_handle_ptr->delete_full_path[del_handle_ptr->delete_path_len],
                                    find_data_ptr->name,
                                    find_name_len);
                del_handle_ptr->delete_path_len += find_name_len;
                file_ret = SFS_ERROR_NONE;
                //send delete next folder msg
                MMI_SendSignal_2APPDel (APP_DEL_ONE_FOLDER_IND, del_handle_ptr);
            }
            else
            {
                //sub folder name too long, stop delete
                file_ret = SFS_ERROR_LONG_DEPTH;
            }
        }

        SCI_FREE (find_data_ptr);
    }

    return file_ret;

}

/*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FileDeleteCallback (
    SFS_ERROR_E   error,
    uint32        irp_param,
    uint32        param1,
    uint32        param2
)
{
    APP_DELETE_HANDLE_T *app_handle_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIFILE]FileDeleteCallback error %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1353_112_2_18_2_20_0_179, (uint8*) "d", error);
    app_handle_ptr = (APP_DELETE_HANDLE_T *) irp_param;

    if (PNULL != app_handle_ptr)
    {
        app_handle_ptr->file_ret = error;

    }

    MMI_SendSignal_2APPDel (APP_DEL_ONE_FILE_IND, app_handle_ptr);
    MMIAPICOM_TriggerMMITask();
}

/*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetParentFolder (wchar *src_path_ptr, uint16* src_path_len_ptr, wchar *base_path_ptr, uint16 base_path_len)
{
    BOOLEAN  result = FALSE;

    //modified by liyan.zhu for c++ test
    //uint16   i  = 0;
    //uint16   *path_ptr = PNULL;
    //uint16   src_path_len = 0;
    if (PNULL == src_path_ptr || PNULL == base_path_ptr || PNULL == src_path_len_ptr)
    {
        result = FALSE;
    }
    else
    {
        uint16   i  = 0;
        uint16   *path_ptr = PNULL;
        uint16   src_path_len = 0;
        src_path_len = MMIAPICOM_Wstrlen (src_path_ptr);
        path_ptr = src_path_ptr;

        for (i = src_path_len; i > 0 ; i--)
        {
            if (path_ptr[i] == MMIFILE_SLASH)
            {
                path_ptr[i] = 0x00;
                break;
            }

            path_ptr[i] = 0x00;
        }

        if (i != 0)
        {
            *src_path_len_ptr = (uint16) i;
        }

        if (!MMIAPICOM_Wstrncmp (base_path_ptr, src_path_ptr, base_path_len))
        {
            result = TRUE;
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : find only files
//  Global resource dependence :
//  Author:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E FindOnlyFiles (APP_COPY_HANDLE_T *app_handle_ptr)
{
    MMIFILE_FIND_DATA_T *find_data_ptr = PNULL;
//  uint16              find_name_len = 0;
    SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
//modified by liyan.zhu for c++ test
    //uint16              ctlStr[] = L"/A:+NF-NF";//find file not include folder
    //uint16              filter[] = L"*";//all files
//  SFS_OVERLAPPED_T    overLapped = {0};
    // 查找第一个数据
    //SCI_TRACE_LOW:"[MMIFILE] FindOnlyFiles enter "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1416_112_2_18_2_20_1_180, (uint8*) "");

    if (PNULL == app_handle_ptr)
    {
        return SFS_ERROR_DEVICE;
    }

    find_data_ptr = (MMIFILE_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (MMIFILE_FIND_DATA_T));

    if (PNULL == find_data_ptr)
    {
        sfs_ret = SFS_ERROR_NO_SPACE;
    }
    else
    {
        uint16              ctlStr[] = L"/A:+NF-NF";//find file not include folder
        uint16              filter[] = L"*";//all files
        sfs_ret = SFS_FindFirstFileCtrl (app_handle_ptr->current_full_path,
                                         filter,
                                         ctlStr,
                                         &app_handle_ptr->find_handle,
                                         find_data_ptr,
                                         PNULL
                                        );

        if (0 == app_handle_ptr->find_handle || SFS_ERROR_NONE != sfs_ret)
        {
            //not exist
            SFS_FindClose (app_handle_ptr->find_handle);
            app_handle_ptr->find_handle = 0;
        }
        else
        {
            if (!CombineCurrentPathAndDestPath (app_handle_ptr, find_data_ptr->name))
            {
                sfs_ret = SFS_ERROR_LONG_DEPTH;
            }
        }

        SCI_FREE (find_data_ptr);
    }

    return sfs_ret;
}



/*****************************************************************************/
//  Description : open find only folders
//  Global resource dependence :
//  Author:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E FindOnlyFolders (APP_COPY_HANDLE_T *app_handle_ptr)
{
    MMIFILE_FIND_DATA_T *find_data_ptr = PNULL;
//  uint16              find_name_len = 0;
    SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
//modified by liyan.zhu for c++ test
    //uint16              ctlStr[] = L"/A:+NF-F";//find folder not include file
    //uint16              filter[] = L"*";//all files
    //uint16              path_depth = 0;
    BOOLEAN             is_down = TRUE;
    //SFS_OVERLAPPED_T    overLapped = {0};
    // 查找第一个数据
    //SCI_TRACE_LOW:"[MMIFILE] FindOnlyFolders enter "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1473_112_2_18_2_20_1_181, (uint8*) "");

    if (PNULL == app_handle_ptr)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    find_data_ptr = (MMIFILE_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (MMIFILE_FIND_DATA_T));

    if (!app_handle_ptr->is_first_find_folder)
    {
        is_down = FALSE;
    }

    if (PNULL == find_data_ptr)
    {
        sfs_ret = SFS_ERROR_NO_SPACE;
    }
    else
    {
        uint16              ctlStr[] = L"/A:+NF-F";//find folder not include file
        uint16              filter[] = L"*";//all files

        do
        {
            SCI_MEMSET (find_data_ptr, 0x00, sizeof (MMIFILE_FIND_DATA_T));
            sfs_ret = SFS_FindFirstFileCtrl (app_handle_ptr->current_full_path,
                                             filter,
                                             ctlStr,
                                             &app_handle_ptr->find_handle,
                                             find_data_ptr,
                                             PNULL
                                            );

            if (0 == app_handle_ptr->find_handle || SFS_ERROR_NONE != sfs_ret)
            {
                //not exist
                is_down = FALSE;

                if (!GetParentFolder (app_handle_ptr->current_full_path,
                                      &app_handle_ptr->current_full_path_len,
                                      app_handle_ptr->src_full_path,
                                      app_handle_ptr->src_full_path_len))
                {

                    //copy end
                    //sfs_ret = SFS_ERROR_NONE;
                    break;
                }

            }
        }
        while (0 == app_handle_ptr->find_handle || SFS_ERROR_NONE != sfs_ret);

        if (SFS_ERROR_NONE == sfs_ret)
        {
            if (!CombineCurrentPathAndDestPath (app_handle_ptr, find_data_ptr->name))
            {
                SFS_FindClose (app_handle_ptr->find_handle);
                app_handle_ptr->find_handle = 0;
                sfs_ret = SFS_ERROR_LONG_DEPTH;
            }
            else if (is_down)
            {
                //first enter this folder, offset = 0;
                uint16 path_depth = MMIFMM_GetFullPathDepth (app_handle_ptr->current_full_path, app_handle_ptr->current_full_path_len);
                app_handle_ptr->findfile_offset[path_depth] = 0;
            }
        }

        SCI_FREE (find_data_ptr);
    }

    if (app_handle_ptr->is_first_find_folder)
    {
        app_handle_ptr->is_first_find_folder = is_down;
    }

    return sfs_ret;
}


/*****************************************************************************/
//  Description : start copy
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
APP_COPY_HANDLE_T *MMIFILE_CopyOpen (wchar *src_file_name,
                                     uint16 src_name_len,
                                     wchar *dest_file_name,
                                     uint16 dest_file_name_len
                                    )
{
    APP_COPY_HANDLE_T *handle_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIFILE] MMIFILE_CopyOpen enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1555_112_2_18_2_20_1_182, (uint8*) "");

    if (PNULL == src_file_name || 0 == src_name_len || src_name_len > MMIFILE_FULL_PATH_MAX_LEN)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_CopyOpen src_file_name 0x&x src_name_len %d !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1558_112_2_18_2_20_1_183, (uint8*) "d", src_file_name, src_name_len);
        return PNULL;
    }

    if (PNULL == dest_file_name || 0 == dest_file_name_len || dest_file_name_len > MMIFILE_FULL_PATH_MAX_LEN)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_CopyOpen dest_file_name 0x&x dest_file_name_len %d !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1564_112_2_18_2_20_1_184, (uint8*) "d", dest_file_name, dest_file_name_len);
        return PNULL;
    }

    handle_ptr = (APP_COPY_HANDLE_T *) SCI_ALLOC_APP (sizeof (APP_COPY_HANDLE_T));

    if (PNULL == handle_ptr)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_CopyOpen handle_ptr 0x%x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1571_112_2_18_2_20_1_185, (uint8*) "d", handle_ptr);
        return PNULL;
    }

    SCI_MEMSET (handle_ptr, 0x00, (sizeof (APP_COPY_HANDLE_T)));

    SCI_MEMCPY (handle_ptr->src_full_path, src_file_name, sizeof (wchar) *src_name_len);
    handle_ptr->src_full_path_len = src_name_len;

    SCI_MEMCPY (handle_ptr->dest_full_path, dest_file_name, sizeof (wchar) *dest_file_name_len);
    handle_ptr->dest_full_path_len = dest_file_name_len;
    handle_ptr->user_stopped = FALSE;
    MMIFMM_SetCopyEnableFlag(TRUE);
    MMI_SendSignal_2APPCOPY (APP_COPY_START, handle_ptr);

    return handle_ptr;
}

/*****************************************************************************/
//  Description :  copy overwrite
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_CopyOverwrite (APP_COPY_HANDLE_T *handle_ptr)
{
    //APP_COPY_HANDLE_T *handle_ptr = PNULL;
    if (handle_ptr != PNULL)
    {
        handle_ptr->user_stopped = FALSE;
    }

    MMI_SendSignal_2APPCOPY (APP_COPY_OVERWRITE, handle_ptr);
}
/*****************************************************************************/
//  Description : combine current full path name and dest full path name by find file name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CombineCurrentPathAndDestPath (APP_COPY_HANDLE_T *handle_ptr, wchar* find_name)
{
    uint16 find_name_len = 0;
    BOOLEAN result = FALSE;

    if (PNULL == handle_ptr || PNULL == find_name)
    {
        return FALSE;
    }

    find_name_len = MMIAPICOM_Wstrlen (find_name);

    if (handle_ptr->current_full_path_len +  find_name_len < MMIFILE_FULL_PATH_MAX_LEN)
    {
        handle_ptr->current_full_path[handle_ptr->current_full_path_len] = MMIFILE_SLASH;
        handle_ptr->current_full_path_len ++;

        MMIAPICOM_Wstrncpy (&handle_ptr->current_full_path[handle_ptr->current_full_path_len],
                            find_name,
                            find_name_len);
        handle_ptr->current_full_path_len += find_name_len;
        handle_ptr->current_full_path[handle_ptr->current_full_path_len] = 0;

        if (handle_ptr->root_dest_path_len + handle_ptr->current_full_path_len - handle_ptr->root_src_path_len  < MMIFILE_FULL_PATH_MAX_LEN)
        {
            //combine dest folder not large MMIFILE_FULL_PATH_MAX_LEN
            handle_ptr->dest_full_path_len = handle_ptr->root_dest_path_len;
            MMIAPICOM_Wstrncpy (&handle_ptr->dest_full_path[handle_ptr->dest_full_path_len],
                                &handle_ptr->current_full_path[handle_ptr->root_src_path_len],
                                handle_ptr->current_full_path_len - handle_ptr->root_src_path_len);
            handle_ptr->dest_full_path_len += handle_ptr->current_full_path_len - handle_ptr->root_src_path_len;//MMIAPICOM_Wstrlen(handle_ptr->dest_full_path);
            handle_ptr->dest_full_path[handle_ptr->dest_full_path_len] = 0;
            result = TRUE;
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : init copy
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E CopyInit (APP_COPY_HANDLE_T *handle_ptr, uint16 src_name_start)
{
    SFS_ERROR_E file_ret = SFS_ERROR_NONE;
    //modified by liyan.zhu for c++ test
    //MMIFILE_HANDLE file_handle = PNULL;
    //SFS_FIND_DATA_T *find_data_ptr =  PNULL;
    //SFS_FIND_DATA_T find_data = {0};

    //find_data_ptr = (SFS_FIND_DATA_T *)SCI_ALLOC_APP(sizeof(SFS_FIND_DATA_T));

    if (PNULL == handle_ptr) //|| PNULL == find_data_ptr)
    {
        file_ret = SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        handle_ptr->dest_full_path[handle_ptr->dest_full_path_len] = MMIFILE_SLASH;
        handle_ptr->dest_full_path_len ++;
        MMIAPICOM_Wstrncpy (&handle_ptr->dest_full_path[handle_ptr->dest_full_path_len],
                            &handle_ptr->current_full_path[src_name_start],
                            handle_ptr->current_full_path_len - src_name_start
                           );
        handle_ptr->dest_full_path_len += handle_ptr->current_full_path_len - src_name_start;
        handle_ptr->root_dest_path_len = handle_ptr->dest_full_path_len;
#ifdef MMI_FMM_MOVE_SUPPORT

        if (MMIFMM_IsCopyFromMove() && s_move_has_exsit) //从move过渡到copy，且是覆盖的操作，什么也不用做，直接返回，在app中有给继续copy的消息
        {
            return file_ret;
        }

#endif

        if (MMIAPICOM_Wstrcmp (handle_ptr->dest_full_path, handle_ptr->src_full_path))
        {
            MMIFILE_HANDLE file_handle = PNULL;
            SFS_FIND_DATA_T find_data = {0};
            //dest and src not in sama folder
            file_handle = MMIAPIFMM_FindFirstFile (handle_ptr->dest_full_path, handle_ptr->dest_full_path_len, &find_data);

            if (SFS_INVALID_HANDLE == file_handle)
            {
                //not exist in dest folder
                handle_ptr->copy_offset = 0;
                MMI_SendSignal_2APPCOPY (APP_COPY_FILE, handle_ptr);
            }
            else
            {
                //dest file or folder exist, inform FMM
                //MMI_SendSignal_2APP(APP_COPY_EXIST_IND, (uint16)SFS_ERROR_NONE);
                //NEWMS00202628
#ifdef MMI_FMM_MOVE_SUPPORT

                if (MMIFMM_IsCopyFromMove()) //剪切，不能进行重命名
                {
                    MMI_SendSignal_2APP (APP_COPY_EXIST_IND, (uint16) SFS_ERROR_NONE);
                    return file_ret;
                }
                else
#endif
                {
                    if (MMIAPICOM_AutoRenameFileName (handle_ptr->dest_full_path, handle_ptr->dest_full_path_len))
                    {
                        handle_ptr->dest_full_path_len = MMIAPICOM_Wstrlen (handle_ptr->dest_full_path);
                        handle_ptr->root_dest_path_len = handle_ptr->dest_full_path_len;
                        handle_ptr->copy_offset = 0;
                        MMI_SendSignal_2APPCOPY (APP_COPY_FILE, handle_ptr);
                    }
                    else
                    {
                        //auto renamed name too long
                        file_ret = SFS_ERROR_LONG_DEPTH;
                    }
                }
            }

            MMIAPIFMM_FindClose (file_handle);
        }
        else
        {
#ifdef MMI_FMM_MOVE_SUPPORT

            if (MMIFMM_IsCopyFromMove()) //剪切，不能进行重命名
            {
                MMI_SendSignal_2APP (APP_COPY_EXIST_IND, (uint16) SFS_ERROR_NONE);
                return file_ret;
            }

#endif

            //if(dest full path ==  src full path) auto rename
            if (MMIAPICOM_AutoRenameFileName (handle_ptr->dest_full_path, handle_ptr->dest_full_path_len))
            {
                handle_ptr->dest_full_path_len = MMIAPICOM_Wstrlen (handle_ptr->dest_full_path);
                handle_ptr->root_dest_path_len = handle_ptr->dest_full_path_len;
                handle_ptr->copy_offset = 0;
                MMI_SendSignal_2APPCOPY (APP_COPY_FILE, handle_ptr);
            }
            else
            {
                //auto renamed name too long
                file_ret = SFS_ERROR_LONG_DEPTH;
                //MMI_SendSignal_2APP(APP_COPY_EXIST_IND, (uint16)SFS_ERROR_LONG_DEPTH);
            }
        }
    }

    //if(find_data_ptr != PNULL)
    //{
    //	SCI_FREE(find_data_ptr);
    //}
    return file_ret;
}
/*****************************************************************************/
//  Description : start copy a file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E FileCopy (APP_COPY_HANDLE_T *handle_ptr)
{
    MMIFILE_ERROR_E copy_ret = SFS_NO_ERROR;
    //modified by liyan.zhu for c++ test
    //MMIFILE_ERROR_E creat_dir = SFS_NO_ERROR;
    MMIFILE_HANDLE  sour_file_handle = PNULL;
    //MMIFILE_HANDLE  dest_file_handle = PNULL;
    //MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    uint8           *data_ptr = PNULL;
    uint32          copy_size = 0;
    uint32          max_buffer_size = APP_COPY_LARG_BUFFER_SIZE;
    uint32          file_size = 0;
    //uint32          file_mode = 0;
    wchar           ctrl_wstr1[] = L"/SA:+H";
    //SFS_FIND_DATA_T dir_find_data = {0} ;
    //wchar*          dir_name_ptr	= PNULL ;
    //uint16          dir_name_length = 0;
    //wchar*          dir_path_ptr	= PNULL ;
    //uint16          dir_path_length    =  0;
    //uint32          read_size = 0;

    if (PNULL == handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFILE] FileCopy invalid file name !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1736_112_2_18_2_20_1_186, (uint8*) "");
        return SFS_ERROR_INVALID_PARAM;
    }

    if (handle_ptr->copy_offset == 0 && MMIAPIFMM_IsFolderExist (handle_ptr->current_full_path, handle_ptr->current_full_path_len))
    {
        MMIFILE_ERROR_E creat_dir = SFS_NO_ERROR;
        SFS_FIND_DATA_T dir_find_data = {0} ;
        wchar*          dir_name_ptr	= PNULL ;
        uint16          dir_name_length = 0;
        wchar*          dir_path_ptr	= PNULL ;
        uint16          dir_path_length    =  0;
        //folder, create folder directly
        creat_dir = MMIAPIFMM_CreateDirectory (handle_ptr->dest_full_path);

        if (SFS_ERROR_NONE == creat_dir)
        {
            dir_name_ptr = (wchar*) SCI_ALLOCA ( (MMIFMM_FILENAME_LEN + 1) * sizeof (wchar));
            dir_path_ptr = (wchar*) SCI_ALLOCA ( (MMIFMM_FILENAME_LEN + 1) * sizeof (wchar));

            if (PNULL == dir_name_ptr || PNULL == dir_path_ptr)
            {
                SCI_FREE (dir_name_ptr);
                SCI_FREE (dir_path_ptr);
                return creat_dir;
            }

            SCI_MEMSET (dir_name_ptr, 0 , (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar));
            SCI_MEMSET (dir_path_ptr, 0 , (MMIFMM_PATHNAME_LEN + 1) *sizeof (wchar));
            dir_path_length = handle_ptr->current_full_path_len;
            MMIAPICOM_Wstrncpy (dir_path_ptr, handle_ptr->current_full_path, dir_path_length);
            MMIAPIFMM_SplitLastName (dir_path_ptr, &dir_path_length, dir_name_ptr, &dir_name_length);
            SFS_GetAttrAsyn (dir_path_ptr ,  dir_name_ptr,  &dir_find_data,   PNULL);

            SCI_MEMSET (dir_name_ptr, 0 , (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar));
            SCI_MEMSET (dir_path_ptr, 0 , (MMIFMM_PATHNAME_LEN + 1) *sizeof (wchar));
            dir_path_length = handle_ptr->dest_full_path_len;
            MMIAPICOM_Wstrncpy (dir_path_ptr, handle_ptr->dest_full_path, dir_path_length);
            MMIAPIFMM_SplitLastName (dir_path_ptr, &dir_path_length, dir_name_ptr, &dir_name_length);

            if (dir_find_data.attr & SFS_ATTR_HIDDEN)
            {
                SFS_SetAttrAsyn (dir_path_ptr, dir_name_ptr, ctrl_wstr1, PNULL);
            }

            SCI_FREE (dir_name_ptr);
            SCI_FREE (dir_path_ptr);
        }

        return creat_dir;
    }

    //first malloc larg size
    data_ptr = (void *) SCI_ALLOC_APP (APP_COPY_LARG_BUFFER_SIZE);

    if (PNULL == data_ptr)
    {
        //no memory for larg size, malloc small size
        data_ptr = (void *) SCI_ALLOCA (APP_COPY_BUFFER_SIZE);
        max_buffer_size = APP_COPY_BUFFER_SIZE;

        if (PNULL == data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFILE] FileCopy no memory!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1785_112_2_18_2_20_1_187, (uint8*) "");
            return SFS_ERROR_NO_SPACE;
        }
    }

    copy_size = max_buffer_size;
    copy_ret = MMIAPIFMM_CreateFileAsyn (handle_ptr->current_full_path,
                                         SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, /*lint !e655*/
                                         0,
                                         0,
                                         0,
                                         0,
                                         &sour_file_handle
                                        );

    if (SFS_INVALID_HANDLE != sour_file_handle)
    {
        MMIFILE_HANDLE  dest_file_handle = PNULL;
        uint32          file_mode = 0;
        uint32          read_size = 0;

#ifdef DRM_SUPPORT
        if (MMIAPIDRM_IsDRMFile(sour_file_handle, NULL) && DRM_LEVEL_SD == MMIAPIDRM_GetDRMFileType(sour_file_handle, NULL))
        {
            //如果copy SD类型的DRM文件，需要拷贝整个文件，不需要解析
			MMIAPIDRM_SDSetEncrypt( sour_file_handle, TRUE);
        }
#endif
        file_size = MMIAPIFMM_GetFileSize (sour_file_handle);

        if (0 == handle_ptr->copy_offset)
        {
            MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
            //first write, create a new file to overwrite
            file_mode = SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS; /*lint !e655*/
            device_type = MMIAPIFMM_GetDeviceTypeByPath (handle_ptr->dest_full_path, MMIFILE_DEVICE_LEN);

            //fs_dev = MMIFILE_GetFileDeviceType(device_type);
            if (!MMIAPIFMM_IsEnoughSpace (MMIFMM_STORE_TYPE_FIXED, device_type, file_size, PNULL))
            {
                copy_ret = SFS_ERROR_NO_SPACE;
            }
        }
        else
        {
            //not first write, cannot create new
            file_mode = SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS;/*lint !e655*/
        }

        if (SFS_ERROR_NONE == copy_ret)
        {
            copy_ret = MMIAPIFMM_CreateFileAsyn (handle_ptr->dest_full_path,
                                                 file_mode,
                                                 0,
                                                 0,
                                                 0,
                                                 0,
                                                 &dest_file_handle
                                                );
        }

        if (SFS_ERROR_NONE != copy_ret)
        {
            //SCI_TRACE_LOW:"[MMIFILE] FileCopy open file error !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1831_112_2_18_2_20_1_188, (uint8*) "");
            MMIAPIFMM_CloseFile (sour_file_handle);
            MMIAPIFMM_CloseFile (dest_file_handle);
            SCI_FREE (data_ptr);
            return copy_ret;
        }

        //start copy
        handle_ptr->copy_offset = 0;
        MMIAPIFMM_SetFilePointer (sour_file_handle, handle_ptr->copy_offset, SFS_SEEK_BEGIN);
        MMIAPIFMM_SetFilePointer (dest_file_handle, handle_ptr->copy_offset, SFS_SEEK_BEGIN);

        do
        {
            copy_ret = MMIAPIFMM_ReadFile (sour_file_handle, data_ptr, max_buffer_size, &read_size, PNULL);

            if (SFS_ERROR_NONE == copy_ret && 0 < read_size)
            {
                //read success, write to dest file
                copy_ret = MMIAPIFMM_WriteFile (dest_file_handle, data_ptr, read_size, &copy_size, PNULL);

                if (SFS_ERROR_NONE == copy_ret && read_size == copy_size)
                {
                    if (read_size < max_buffer_size)
                    {
                        SCI_TRACE_LOW ("[FileTask]: FileCopy Write success!");
                        break;
                    }
                }
                else
                {
                    SCI_TRACE_LOW ("[FileTask]: FileCopy Write copy_ret=%d,read_size=%d,copy_size=%d", copy_ret, read_size, copy_size);
                    break;
                }
            }
            else
            {
                SCI_TRACE_LOW ("[FileTask]: FileCopy Read copy_ret=%d,read_size=%d,copy_size=%d", copy_ret, read_size, copy_size);
                break;
            }
        }
        while (MMIFMM_GetCopyEnableFlag() && 0 != read_size);

        MMIAPIFMM_CloseFile (sour_file_handle);
        MMIAPIFMM_CloseFile (dest_file_handle);

        if ((SFS_ERROR_IO_PENDING != copy_ret && SFS_ERROR_NONE != copy_ret)
            ||(read_size == max_buffer_size)//user cancel
            )
        {
            //copy error, delete dest file
            MMIAPIFMM_DeleteFile (handle_ptr->dest_full_path, PNULL);
        }
        else
        {
            if (handle_ptr->copy_offset >=  file_size)
            {
                SFS_FIND_DATA_T find_data = {0} ;
                wchar path[MMIFMM_PATHNAME_LEN +1]	= {0} ;
                wchar name[MMIFMM_FILENAME_LEN +1]	= {0} ;
                wchar ctrl_wstr[MMIFMM_FILENAME_LEN + 1] = {0} ;
                char ctrl_str[MMIFMM_FILENAME_LEN + 1] = {0} ;
                uint16 path_length = handle_ptr->current_full_path_len;
                uint16 name_length = 0;


                //get source file name and path
                SCI_MEMSET (name, 0 , (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar));
                SCI_MEMSET (path, 0 , (MMIFMM_PATHNAME_LEN + 1) *sizeof (wchar));

                MMIAPICOM_Wstrncpy (path, handle_ptr->current_full_path, path_length);
                MMIAPIFMM_SplitLastName (path, &path_length, name, &name_length);

                //get time arributes
                SFS_GetAttrAsyn (path ,  name,  &find_data,   PNULL);

                //get destination file name and path
                SCI_MEMSET (name, 0 , (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar));
                SCI_MEMSET (path, 0 , (MMIFMM_PATHNAME_LEN + 1) *sizeof (wchar));

                path_length = handle_ptr->dest_full_path_len;
                MMIAPICOM_Wstrncpy (path, handle_ptr->dest_full_path, path_length);
                MMIAPIFMM_SplitLastName (path, &path_length, name, &name_length);

                //set time arributes
                sprintf (ctrl_str, "/STM:%d-%d-%d-%d-%d-%d" ,
                         find_data.modify_Date.year, find_data.modify_Date.mon, find_data.modify_Date.mday,
                         find_data.modify_time.hour, find_data.modify_time.min, find_data.modify_time.sec);

                //	sprintf(ctrl_str, "/STM:2010-1-20-12-34-56");

                MMI_STRNTOWSTR (ctrl_wstr, MMIFMM_FILENAME_LEN, (uint8*) ctrl_str, MMIFMM_FILENAME_LEN,
                                SCI_STRLEN (ctrl_str));
                SFS_SetAttrAsyn (path, name, ctrl_wstr, PNULL);

                if (find_data.attr & SFS_ATTR_HIDDEN)
                {
                    SFS_SetAttrAsyn (path, name, ctrl_wstr1, PNULL);
                }
            }
        }
    }

    SCI_FREE (data_ptr);
    //SCI_TRACE_LOW:"[MMIFILE] FileCopy copy_ret %d handle_ptr->copy_offset %d filesize %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1925_112_2_18_2_20_2_189, (uint8*) "ddd", copy_ret, handle_ptr->copy_offset, file_size);
    return copy_ret;
}

#ifdef MMI_FMM_MOVE_SUPPORT
/*****************************************************************************/
//  Description : start move
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
APP_MOVE_HANDLE_T *MMIFILE_MoveOpen (wchar *src_path,
                                     wchar * src_filename,
                                     wchar *dest_full_path,
                                     wchar * dest_filename,
                                     MMIFMM_FILE_TYPE_E type
                                    )
{
    APP_MOVE_HANDLE_T *move_handle_ptr = PNULL;
    uint16  src_path_len = 0;
    uint16  src_name_len = 0;
    uint16  dst_path_len = 0;
    uint16  dst_name_len = 0;

    //SCI_TRACE_LOW:"[[MMIFMM]: MMIFILE_MoveOpen enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1946_112_2_18_2_20_2_190, (uint8*) "");

    if (PNULL == src_path || PNULL == src_filename)
    {
        //SCI_TRACE_LOW:"MMIFILE MMIFILE_MoveOpen src_file_name 0x&x src_filename 0x&x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1949_112_2_18_2_20_2_191, (uint8*) "", src_path, src_filename);
        return PNULL;
    }

    if (PNULL == dest_full_path || PNULL == dest_filename)
    {
        //SCI_TRACE_LOW:"[MMIFMM]: MMIFILE_MoveOpen dest_file_name 0x&x dest_filename 0x&x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1955_112_2_18_2_20_2_192, (uint8*) "", dest_full_path, dest_filename);
        return PNULL;
    }

    move_handle_ptr = (APP_MOVE_HANDLE_T *) SCI_ALLOC_APP (sizeof (APP_MOVE_HANDLE_T));

    if (PNULL == move_handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFMM]: MMIFILE_MoveOpen handle_ptr 0x%x !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1962_112_2_18_2_20_2_193, (uint8*) "d", move_handle_ptr);
        return PNULL;
    }

    SCI_MEMSET (move_handle_ptr, 0x00, (sizeof (APP_MOVE_HANDLE_T)));

    src_path_len = MMIAPICOM_Wstrlen (src_path);
    src_name_len = MMIAPICOM_Wstrlen (src_filename);
    dst_path_len = MMIAPICOM_Wstrlen (dest_full_path);
    dst_name_len = MMIAPICOM_Wstrlen (dest_filename);

    if (src_path_len > APP_COPY_FULLPATH_LEN || src_name_len > APP_COPY_FULLPATH_LEN
            || dst_path_len > APP_COPY_FULLPATH_LEN || dst_name_len > APP_COPY_FULLPATH_LEN)
    {
        //SCI_TRACE_LOW:"[MMIFMM]: MMIFILE_MoveOpen param error"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_1974_112_2_18_2_20_2_194, (uint8*) "");
        SCI_FREE (move_handle_ptr);
        return PNULL;
    }

    MMIAPICOM_Wstrncpy (move_handle_ptr->src_path, src_path, src_path_len);
    MMIAPICOM_Wstrncpy (move_handle_ptr->src_filename, src_filename, src_name_len);
    MMIAPICOM_Wstrncpy (move_handle_ptr->dest_full_path, dest_full_path, dst_path_len);
    MMIAPICOM_Wstrncpy (move_handle_ptr->dest_filename, dest_filename, dst_name_len);
    move_handle_ptr->type = type;
    move_handle_ptr->user_stopped = FALSE;
    MMIFMM_SetCopyEnableFlag(TRUE);
    MMI_SendSignal_2APPMOVE (APP_MOVE_START, move_handle_ptr);

    return move_handle_ptr;
}
/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence :
//  Author: juan.wu
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPMOVE (uint16      sig_id, APP_MOVE_HANDLE_T *handle_ptr)
{
    APP_DELETE_SIG_T *sig_ptr = PNULL;

    MMI_CREATE_SIGNAL (sig_ptr,
                       sig_id,
                       sizeof (APP_MOVE_HANDLE_T),
                       P_APP
                      );
    sig_ptr->data_ptr = handle_ptr;
    MMI_SEND_SIGNAL (sig_ptr, P_APP_MOVE);
}
#if defined(MMIFMM_XMOVE_SUPPORT)
/*****************************************************************************/
//  Description : 移动file或folder
//  Global resource dependence :
//  Author: juan.wu
//  Note:
//  Return:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_MoveFileOrTree (const wchar * src_path_ptr,
        const wchar *src_filename_ptr,
        const wchar *dst_full_path_ptr,
        const wchar *dst_filename_ptr,
        MMIFMM_FILE_TYPE_E file_type)
{
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;

    if (MMIFMM_FILE_TYPE_FOLDER == file_type)
    {
        sfs_error = SFS_XMoveTree (src_path_ptr,
                                   src_filename_ptr,
                                   dst_full_path_ptr,
                                   dst_filename_ptr,
                                   0,
                                   0,
                                   PNULL);
    }
    else
    {

        sfs_error = SFS_XMoveFile (src_path_ptr,
                                   src_filename_ptr,
                                   dst_full_path_ptr,
                                   dst_filename_ptr,
                                   0,
                                   0,
                                   PNULL);
    }

    return sfs_error;

}
#endif
/*****************************************************************************/
//  Description : handle move sig
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void APPFILE_HandleMoveSig (
    APP_DELETE_SIG_T  *psig
)
{
    APP_MOVE_HANDLE_T * move_handle_ptr = PNULL;

    if (PNULL == psig)
    {
        //SCI_TRACE_LOW:"[MMIFMM]:APPFILE_HandleMoveSig para error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2055_112_2_18_2_20_2_195, (uint8*) "");
        return;
    }

    move_handle_ptr = (APP_MOVE_HANDLE_T *) psig->data_ptr;

    if (PNULL == move_handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFMM]:APPFILE_HandleMoveSig PNULL == move_handle_ptr!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2062_112_2_18_2_20_2_196, (uint8*) "");
        return;
    }

    switch (psig->SignalCode)
    {
    case APP_MOVE_START:
        {
            uint16 dst_full_path_len = MMIAPICOM_Wstrlen (move_handle_ptr->dest_full_path);
            uint16 src_name_len = MMIAPICOM_Wstrlen (move_handle_ptr->src_filename);

            if (dst_full_path_len + src_name_len + 1 >= MMIFILE_FULL_PATH_MAX_LEN)
            {
                //dest path too long
                MMI_SendSignal_2APP (APP_MOVE_END_IND, (uint16) SFS_ERROR_LONG_DEPTH);
                break;
            }

            if (move_handle_ptr->user_stopped)
            {
                MMI_SendSignal_2APP (APP_MOVE_ONE_FILE_IND, (uint16) SFS_ERROR_ABORT);
                break;
            }

            MMI_SendSignal_2APPMOVE (APP_MOVE_FILES, move_handle_ptr);
        }
        break;

    case APP_MOVE_FILES:
        {
            wchar src_full_name[MMIFMM_FULL_FILENAME_LEN +1] = {0};
            uint16 src_path_len = MMIAPICOM_Wstrlen (move_handle_ptr->src_path);
            uint16 src_file_len = MMIAPICOM_Wstrlen (move_handle_ptr->src_filename);

            if ( (src_path_len + src_file_len + 1) > MMIFMM_FULL_FILENAME_LEN)
            {
                //SCI_TRACE_LOW:"[MMIFMM]:filename too long src_path_len + src_file_len + 1 = %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2096_112_2_18_2_20_2_197, (uint8*) "d", src_path_len + src_file_len + 1);
                break;
            }

            MMIAPICOM_Wstrncpy (src_full_name, move_handle_ptr->src_path, src_path_len);
            MMIAPICOM_Wstrncat (src_full_name, L"\\", 1);
            MMIAPICOM_Wstrncat (src_full_name, move_handle_ptr->src_filename, src_file_len);

            if (move_handle_ptr->user_stopped)
            {
                MMI_SendSignal_2APP (APP_MOVE_ONE_FILE_IND, (uint16) SFS_ERROR_ABORT);
            }
            else if (MMIFMM_IsSrcFileExistInDstPath (move_handle_ptr->src_filename,
                     move_handle_ptr->dest_full_path,
                     MMIAPICOM_Wstrlen (move_handle_ptr->src_filename),
                     MMIAPICOM_Wstrlen (move_handle_ptr->dest_full_path)))
            {
                s_move_has_exsit = TRUE;
                MMI_SendSignal_2APP (APP_MOVE_FILE_EXIST, (uint16) SFS_ERROR_NONE);
            }
            else
            {
#if defined(MMIFMM_XMOVE_SUPPORT)
                SFS_ERROR_E       file_ret = SFS_ERROR_NONE;
                file_ret = MMIFMM_MoveFileOrTree (move_handle_ptr->src_path,
                                                  move_handle_ptr->src_filename,
                                                  move_handle_ptr->dest_full_path,
                                                  move_handle_ptr->dest_filename,
                                                  move_handle_ptr->type);
                MMI_SendSignal_2APP (APP_MOVE_ONE_FILE_IND, (uint16) file_ret);
#else
                SCI_TRACE_LOW ("movefiles error,not support xmove but in xmove flow!");
                MMI_SendSignal_2APP (APP_MOVE_ONE_FILE_IND, (uint16) SFS_ERROR_ABORT);
#endif
            }
        }
        break;

    case APP_MOVE_STOP:
        move_handle_ptr->user_stopped = TRUE;
        break;

    case APP_MOVE_CLOSE:
        s_move_has_exsit = FALSE;
        SCI_FREE (move_handle_ptr);
        break;

    default:
        break;
    }

}
/*****************************************************************************/
//  Description : close move file task
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_MoveClose (APP_MOVE_HANDLE_T *handle_ptr)
{
    BOOLEAN result = FALSE;

    //SCI_TRACE_LOW:"[MMIFMM]: APPFILE_CopyClose "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2147_112_2_18_2_20_2_198, (uint8*) "");
    MMIFMM_SetCopyEnableFlag(FALSE);
    MMI_SendSignal_2APPMOVE (APP_MOVE_CLOSE, handle_ptr);
    return result;
}
/*****************************************************************************/
//  Description : 判断在目标路径中是否有与源文件重名的文件
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_IsSrcFileExistInDstPath (const wchar * src_filename_ptr,
        const wchar * dst_full_path_ptr,
        uint16 src_filename_len,
        uint16 dest_full_path_len)
{
    MMIFILE_HANDLE file_handle = PNULL;
    BOOLEAN   result = FALSE;
    MMIFILE_FIND_DATA_T find_data = {0};
    wchar     combine_full_path[MMIFMM_FULL_FILENAME_LEN +1] = {0};
    uint16    combine_full_path_len = 0;

    if (src_filename_len + dest_full_path_len + 1 > MMIFMM_FILE_FILE_NAME_MAX_LEN)
    {
        //SCI_TRACE_LOW:"[MMIFMM]:src_filename_len + dest_full_path_len = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2168_112_2_18_2_20_2_199, (uint8*) "d", src_filename_len + dest_full_path_len);
        return result;
    }

    //将源文件名和目标路径组合起来 dst_full_path_ptr\src_filename_ptr
    MMIAPICOM_Wstrncpy (combine_full_path, dst_full_path_ptr, dest_full_path_len);
    MMIAPICOM_Wstrncat (combine_full_path, L"\\", 1);
    MMIAPICOM_Wstrncat (combine_full_path, src_filename_ptr, src_filename_len);
    combine_full_path_len = MMIAPICOM_Wstrlen (combine_full_path);

    file_handle = MMIFILE_FindFirstFile (combine_full_path, combine_full_path_len, &find_data);

    if (SFS_INVALID_HANDLE == file_handle) //没找到
    {
        result = FALSE;
    }
    else//找到了
    {
        result = TRUE;
    }

    MMIFILE_FindClose (file_handle);
    return result;
}
/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_AbortMove (APP_MOVE_HANDLE_T *handle_ptr)
{
    //SCI_TRACE_LOW:"[MMIFMM]: APPFILE_AbortMove "
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2196_112_2_18_2_20_2_200, (uint8*) "");

    if (PNULL == handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFMM]: APPFILE_AbortMove PNULL == handle_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILETASK_2199_112_2_18_2_20_2_201, (uint8*) "");
        return FALSE;
    }
    MMIFMM_SetCopyEnableFlag(FALSE);
    MMI_SendSignal_2APPMOVE ( (uint16) APP_MOVE_STOP, handle_ptr);
    return TRUE;
}
#endif
