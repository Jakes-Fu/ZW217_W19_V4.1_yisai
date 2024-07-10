/***************************************************************************
** File Name:      mmi_filetask.h                                          *
** Author:                                                                 *
** Date:           03/29/2007                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the app copy task         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2007       Liqing Peng       Create                                  *
****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
//#include "mmi_filemgr.h"
#include "sfs.h"
#include "mmifmm_export.h"
#include "mmi_custom_define.h"
#include "caf.h"


#ifndef MMI_FILETASK_H
#define MMI_FILETASK_H


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//return result
#define APP_COPY_RESULT_SUCCESS             0x00    //finish copy a file with success
#define APP_COPY_RESULT_USER_END            0x01    //finish copy a file with user end

#define APP_COPY_RESULT_FAIL                0x02    //finish copy a file with fail
#define APP_COPY_RESULT_FILENAME_ERROR      0x03
#define APP_COPY_RESULT_SFS_ERROR           0x04
#define APP_COPY_RESULT_CREATEFILE_ERROR    0x05
#define APP_COPY_RESULT_READFILE_ERROR      0x06
#define APP_COPY_RESULT_WRITEFILE_ERROR     0x07

#define APP_COPY_RESULT_NO_ERROR            0x0a    //这个只用于file task内部
#define APP_COPY_RESULT_ERROR               0x0b    //这个只用于file task内部

#define APP_COPY_FULLPATH_LEN           MMIFMM_FILE_FILE_NAME_MAX_LEN
#define MMI_COPY_OPT_MAX_LEN            MMIFMM_FILE_FULL_PATH_MAX_LEN

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//发送给app file task的消息
typedef enum
{
    APP_COPY_SIG_NONE,

    APP_COPY_START,         //start copy a file
    APP_COPY_STOP,          //stop copy current file
	APP_COPY_CLOSE,         //copy finished, free resource
	APP_COPY_FIND_FILE,
	APP_COPY_FILE,
	APP_COPY_OVERWRITE,
	APP_COPY_FIND_FOLDER,
    APP_DEL_FILES,
	APP_DEL_ONE_FILE_IND,
	APP_DEL_ONE_FOLDER_IND,
	APP_DEL_END_IND,
	APP_DEL_STOP,
    //for find file
    APP_FIND_FILES,
#ifdef MMI_FMM_MOVE_SUPPORT
    APP_MOVE_START,
    APP_MOVE_FILES,
    APP_MOVE_CLOSE,
    APP_MOVE_STOP,
#endif
    APP_COPY_SIG_MAX
}APP_FILE_SIG_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    wchar       src_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16      src_full_path_len;
    wchar       dest_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16      dest_full_path_len;
    SFS_ERROR_E sfs_error;
}APP_COPY_SIG_T;


typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}APP_DELETE_SIG_T;

typedef struct
{
    wchar                   src_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16                  src_path_len;
    wchar                   delete_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16                  delete_path_len;
    MMIFILE_HANDLE          find_handle;         //原文件的句柄
	MMIFILE_ERROR_E         file_ret;
	BOOLEAN                 user_stopped;
}APP_DELETE_HANDLE_T;


typedef struct
{
    wchar       src_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16      src_full_path_len;
    wchar       dest_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16      dest_full_path_len;
    wchar       current_full_path[APP_COPY_FULLPATH_LEN+1];
    uint16      current_full_path_len;
	uint16      root_src_path_len;
	uint16      root_dest_path_len;
	uint16      findfile_offset[APP_COPY_FULLPATH_LEN + 1];
	uint16      current_folder_index;
	BOOLEAN     is_first_find_folder;
	uint32      copy_offset;
    MMIFILE_HANDLE find_handle;        
    SFS_ERROR_E sfs_error;
	BOOLEAN     user_stopped;
}APP_COPY_HANDLE_T;

typedef struct
{
    wchar       src_path[APP_COPY_FULLPATH_LEN+1];//源路径，不含文件名
    wchar       src_filename[APP_COPY_FULLPATH_LEN+1];//源文件名
    wchar       dest_full_path[APP_COPY_FULLPATH_LEN+1];//目标 全路径
    wchar       dest_filename[APP_COPY_FULLPATH_LEN+1];//目标文件名,和源文件名要一样
    MMIFMM_FILE_TYPE_E type;
    BOOLEAN     user_stopped;
}APP_MOVE_HANDLE_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create the app file task
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_CreateTask(void);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION IMPLEMENT                               */
/*---------------------------------------------------------------------------*/
PUBLIC void MMIFMM_SetCopyEnableFlag(
                                  BOOLEAN is_enable//IN:TRUE:enable file copy; FALSE:disable file copy.
                                  );


PUBLIC BOOLEAN MMIFMM_GetCopyEnableFlag(void);

/*****************************************************************************/
//  Description : improve file task priority
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_ImpoveFileTaskPriority(void);

/*****************************************************************************/
//  Description : Restore file task priority
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_RestoreFileTaskPriority(void);

/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence : 
//  Author:
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPCOPY(
                            uint16              sig_id,
                            APP_COPY_HANDLE_T *handle_ptr
                            );

/*****************************************************************************/
//  Description : 发送消息给app task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APP(uint16   sig_id, uint16 data);


/*****************************************************************************/
//  Description : 发送消息给app task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
void MMI_FindSendSignal_2APP(uint16   sig_id, void* data);

/*****************************************************************************/
//  Description :get current find handle
//  Global resource dependence :
//  Author:
//  Return:
/*****************************************************************************/
PUBLIC void* APPFILE_GetCurrentFindHandle(void);

/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPDel( uint16      sig_id, 
							APP_DELETE_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_AbortCopy(APP_COPY_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description :  copy overwrite
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFILE_CopyOverwrite(APP_COPY_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_StopFindFile(void);

/*****************************************************************************/
//  Description : set s_is_stopfindfile = FALSE
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void APPFILE_SetStartFindFileFlag(void);

/*****************************************************************************/
//  Description : get find be stop
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_IsStopingFindFile(void);

/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_IsFindingFile(void);

/*****************************************************************************/
//  Description : delete dir tree
//  Global resource dependence : 
//  Author:
//  Note: if dir_path_ptr is file, delete it dircty, 
//        if dir_path_ptr is directory, first delete its all sub folders, then delete itself.
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteDirTree(APP_DELETE_HANDLE_T *app_del_handle_ptr);

/*****************************************************************************/
//  Description : delete sub file
//  Global resource dependence : 
//  Author:
//  Note: this file or folder must no sub foler or file 
//      
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteSubFileAsyn(APP_DELETE_HANDLE_T *del_handle_ptr, 
												 wchar* file_name, 
												 uint16 file_name_len);

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
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteSubFolder(APP_DELETE_HANDLE_T *del_handle_ptr);

/*****************************************************************************/
//  Description : create delete handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
APP_DELETE_HANDLE_T *MMIFILE_DeleteOpen(wchar *del_file_name, uint16 del_name_len);

/*****************************************************************************/
//  Description : close delete handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFILE_DeleteClose(APP_DELETE_HANDLE_T *del_handle);

/*****************************************************************************/
//  Description : start delete file
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIFILE_AbortDelete(APP_DELETE_HANDLE_T * del_handle_ptr);

/*****************************************************************************/
//  Description : start copy 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
APP_COPY_HANDLE_T *MMIFILE_CopyOpen(wchar *src_file_name, 
									 uint16 src_name_len,
									 wchar *dest_file_name,
									 uint16 dest_file_name_len
									 );

/*****************************************************************************/
//  Description : close app file task
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_CopyClose(APP_COPY_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description : handle asy search file cnf
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void APPFILE_HandleSearchFileCnf(MMIFMM_SRV_HANDLE handle,BOOLEAN search_result);
/*****************************************************************************/
//  Description : start move 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
APP_MOVE_HANDLE_T *MMIFILE_MoveOpen(wchar *src_path, 
									 wchar * src_filename,
									 wchar *dest_path,
									 wchar * dest_filename,
									 MMIFMM_FILE_TYPE_E type
									 );
/*****************************************************************************/
//  Description : 发送消息给app file task
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
//  Return:
/*****************************************************************************/
void MMI_SendSignal_2APPMOVE( uint16      sig_id, APP_MOVE_HANDLE_T *handle_ptr);

/*****************************************************************************/
//  Description : close move file task
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_MoveClose(APP_MOVE_HANDLE_T *handle_ptr);
/*****************************************************************************/
//  Description : stop app file task
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
BOOLEAN APPFILE_AbortMove(APP_MOVE_HANDLE_T *handle_ptr);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //APP_FILE_TASK_H
