#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      guifilesys.c                                            *
** Author:         Xiaoqing.Lu                                             *
** Date:           07/29/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "caf.h" 
#include "cafmodule.h"
#include "cafmodule_gen.h"
#include "caffilesys.h"
#include "sfs.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "guifont.h"
#include "mmi_common.h"
#include "string.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CAFSFS_STR_LENGHT  IFILE_MAX_PATH

//Added by andrew.zhang at 2009/02/27
/*文件系统中加入了对用户可以操作的目录的限制，对D盘只能操作“D:\cafapp\”下的
** 文件，对E盘可以操作整个磁盘，用户访问应用程序中的目录或文件时，只能有两种
** 文件名或目录参数的传递的方式：一种是只传文件名，如test.txt，此时系统使用的
** 是相对路径，相对于"D:\cafapp\"路径；一种是以"fs:\"开始的目录，此时相对的是
** 系统内部指定的某一目录，对D盘,"fs:\"对应D盘的目录为"D:\cafapp\",对SD卡，
** "fs:\e:\"对应"E:\",例如："fs:\test\test.txt"对应实际目录为
** "D:\cafapp\test\test.txt"；fs:\e:\test\test.txt"对应实际目录为
** "E:\test\test.txt"
*/
//用户传文件路径时对应D盘的开头部分，目前只能传小写
#define CAF_DISK_D_PREFIX	"fs:\\"
//用户传文件路径时对应E盘的开头部分，目前只能传小写
#define CAF_DISK_E_PREFIX	"fs:\\e:\\"
//用户操作权限在D盘上对应的目录
#define D_DIRECTORY		"D:\\cafapp\\"
//用户操作权限在E盘上对应的目录
#define E_DIRECTORY		"E:\\"

//D盘前缀的长度，即CAF_DISK_D_PREFIX对应的字符串长度
#define CAF_DISK_D_PREFIX_LEN	4
//E盘前缀的长度，即CAF_DISK_E_PREFIX对应的字符串长度
#define CAF_DISK_E_PREFIX_LEN	7
//D盘目录长度，即D_DIRECTORY对应的字符串长度
#define D_DIRECTORY_LEN		10
//D盘目录长度，即E_DIRECTORY对应的字符串长度
#define E_DIRECTORY_LEN		3

//D盘上的安装目录（特殊处理目录）
#define SPECIAL_DIRECTORY_D_INSTALL		    "fs:\\d:\\Com\\"
#define SPECIAL_DIRECTORY_D_INSTALL_LEN		11


//外部用户传入的路径模式
//1为相对路径(如：test.txt)
//2为D盘绝对路径(如：fs:\test.txt)
//3为E盘绝对路径(如：fs:\e:\test.txt)
#define PATH_MODE_NONE_PREFIX		1    //相对路径模式
#define PATH_MODE_D_PREFIX    		2    //D盘模式
#define PATH_MODE_E_PREFIX   		3    //E盘模式

//add end

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    CAF_VTBL_T(IFILESYS_T)  *vtbl_ptr; // 函数列表
    
    CAF_GUID_T       guid;
    uint32           ref_num;    
}CAFSFS_CTRL_T;

#define UNUSED_PARAM(param)   param

//closed by andrew.zhang at 2009/02/27
#if 0
typedef struct
{
    IFILESYS_T *isfs_ptr;
    wchar file_name[CAFSFS_STR_LENGHT + 1];
    uint32 access_mode; 
    uint32 share_mode; 
    uint32 file_attri;
}CAFSFS_CREATE_PARAM_T;

typedef struct
{
    IFILESYS_T *isfs_ptr;
    wchar sour[CAFSFS_STR_LENGHT + 1];
    wchar dest[CAFSFS_STR_LENGHT + 1];
    SFS_OVERLAPPED_T sfs_overlapped;
}CAFSFS_RENAME_PARAM_T;

typedef struct
{
    IFILESYS_T *isfs_ptr;
    wchar name[CAFSFS_STR_LENGHT + 1];
    SFS_OVERLAPPED_T sfs_overlapped;
}CAFSFS_DELETE_PARAM_T;

typedef union
{
    CAFSFS_CREATE_PARAM_T create_param;
    CAFSFS_RENAME_PARAM_T rename_param;
    CAFSFS_DELETE_PARAM_T delete_param;    
}CAFSFS_OPERATE_PARAM_T;

typedef enum operate_type_t
{
    IFILE_OPERATE_NONE = 0,
	IFILE_CREATE,
	IFILE_FORMAT,
	IFILE_DELETE,
	IFILE_RENAME,	
	IFILE_OPERATE_MAX
}IFILE_OPERATE_E;

typedef struct
{
    BOOLEAN is_operating;
    IFILE_OPERATE_E operate_type;
    IFILE_RESULTCALLBACK result_callback;
    CAFSFS_OPERATE_PARAM_T operate_param;
}CAFSFS_RESULT_RECORD_T;


LOCAL CAFSFS_RESULT_RECORD_T s_result_record = {0};
#endif

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Get caf file system error code from sfs
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL IFILE_ERROR_E GetCafSfsErrorCode(
                                       SFS_ERROR_E error_code
                                       );

/*****************************************************************************/
// 	Description : file system query interface
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSFS_QueryInterface( 
                                         IFILESYS_T *isfs_ptr, 
                                         CAF_GUID_T guid, 
                                         void** ojbect_pptr 
                                         );

/*****************************************************************************/
// 	Description : file system add ref
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSFS_AddRef(
                           IFILESYS_T * isfs_ptr 
                           );

/*****************************************************************************/
// 	Description : file system release
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSFS_Release(
                            IFILESYS_T * isfs_ptr
                            );


/*****************************************************************************/
//  Description:   Mount a disk to file system
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_ptr:if Api success,this will indicate the type of  file system
//  Return:
//       SFS_ERROR_NONE: Success
//       other: Fail
//  Note:           
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_RegisterDevice(
                                          IFILESYS_T *isfs_ptr, 
                                          const uint16 * dev_name, 
                                          IFILE_DEVICE_FORMAT_E *format_ptr
                                          );

/*****************************************************************************/
//  Description:   Unmount a disk to file system
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note:           
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_UnRegisterDevice(
                                            IFILESYS_T *isfs_ptr, 
                                            const uint16 * dev_name
                                            );

/*****************************************************************************/
//  Description:   Format disk
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_type:the type you want to format.But note that some disk can
//       only be format one type
//       overlapped_ptr:asynchronism param,if you give this parm,the result 
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result 
//                will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       If success the disk will be mounted automatically 
//       If fail the disk will be unmounted automatically
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_Format(
                                  IFILESYS_T *isfs_ptr, 
                                  const uint16 *dev_name,
                                  IFILE_DEVICE_FORMAT_E format_type, 
                                  IFILE_OVERLAPPED_T *overlapped_ptr
                                  );

/*****************************************************************************/
//  Description:   Get disk status
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: The disk has be mounted
//       Other: The disk has not be mounted
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceStatus(
                                           IFILESYS_T *isfs_ptr, 
                                           const uint16* dev_name
                                           );

/*****************************************************************************/
//  Description:   Get disk free space
//  Author: Xiaoqing.Lu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of free space
//       low_word:the low 32 bit value of free space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceFreeSpace(
                                              IFILESYS_T *isfs_ptr, 
                                              const uint16 *device_name,
                                              uint32 *high_word,
                                              uint32 *low_word
                                              );

/*****************************************************************************/
//  Description:   Get disk used space
//  Author: Xiaoqing.Lu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of used space
//       low_word:the low 32 bit value of used space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceUsedSpace(
                                              IFILESYS_T *isfs_ptr, 
                                              const uint16 *device_name,
                                              uint32 *high_word,
                                              uint32 *low_word
                                              );

//modified by andrew.zhang at 2009/02/27
#if 0
/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Xiaoqing.Lu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved,Must be NULL,File System have not realize it
//       file_attri:reserved,Must be NULL,File System have not realize it 
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_OpenFile(
                                    IFILESYS_T *isfs_ptr, 
                                    const uint16 *file_name, 
                                    uint32 access_mode, 
                                    uint32 share_mode, 
                                    uint32 file_attri,
                                    IFILE_RESULTCALLBACK result_callback_func
                                    );

/*****************************************************************************/
//  Description:   Delete a file
//  Author: Xiaoqing.Lu
//  Param
//       name :The name of path and file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result 
//                will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_DeleteFile(
                                      IFILESYS_T *isfs_ptr, 
                                      const uint16 *name, 
                                      IFILE_OVERLAPPED_T *overlapped_ptr,
                                      IFILE_RESULTCALLBACK result_callback_func
                                      );

/*****************************************************************************/
//  Description:   Rename a file
//  Author: Xiaoqing.Lu
//  Param
//       sour:The name of path and source file ,must be unicode string
//       dest:The name of path and dest file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result
//                   will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       the path of sourcename and destname must be same
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_RenameFile(
                                      IFILESYS_T *isfs_ptr,
                                      const uint16 *sour,
                                      const uint16 *dest,
                                      IFILE_OVERLAPPED_T *overlapped_ptr,
                                      IFILE_RESULTCALLBACK result_callback_func
                                      );
#else
/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Xiaoqing.Lu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved,Must be NULL,File System have not realize it
//       file_attri:reserved,Must be NULL,File System have not realize it 
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note: 
/*****************************************************************************/
LOCAL SFS_HANDLE CAFSFS_OpenFile(
                                    IFILESYS_T *isfs_ptr, 
                                    const uint16 *file_name, 
                                    uint32 access_mode, 
                                    uint32 share_mode, 
                                    uint32 file_attri
                                    );

/*****************************************************************************/
//  Description:   Delete a file
//  Author: Xiaoqing.Lu
//  Param
//       name :The name of path and file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result 
//                will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_DeleteFile(
                                      IFILESYS_T *isfs_ptr, 
                                      const uint16 *name, 
                                      IFILE_OVERLAPPED_T *overlapped_ptr
                                      );

/*****************************************************************************/
//  Description:   Rename a file
//  Author: Xiaoqing.Lu
//  Param
//       sour:The name of path and source file ,must be unicode string
//       dest:The name of path and dest file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result
//                   will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       the path of sourcename and destname must be same
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_RenameFile(
                                      IFILESYS_T *isfs_ptr,
                                      const uint16 *sour,
                                      const uint16 *dest,
                                      IFILE_OVERLAPPED_T *overlapped_ptr
                                      );

#endif

/*****************************************************************************/
//  Description:   Create a directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_CreateDirectory(
                                           IFILESYS_T *isfs_ptr,
                                           const uint16 *path
                                           );

/*****************************************************************************/
//  Description:   Delete a directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_DeleteDirectory(
                                           IFILESYS_T *isfs_ptr, 
                                           const uint16 *path
                                           );

/*****************************************************************************/
//  Description:   Change the attrib of file/directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetAttr(
                                   IFILESYS_T *isfs_ptr,
                                   const uint16 *name,
                                   uint16 attr
                                   );

/*****************************************************************************/
//  Description:   Get the attrib of file/directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you get
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetAttr(
                                   IFILESYS_T *isfs_ptr, 
                                   const uint16 *name, 
                                   uint16 *attr
                                   );

/*****************************************************************************/
//  Description:  Find a file that match the param 'find'
//  Author: Xiaoqing.Lu
//  Param
//       find:The file name you want to find,such as "*.mp3","*.3gp",or "123.mp3"...
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       None zero: the file founded ,this find handle can used to find next file match the param 'find'
//       zero: not found
//  Note: 
/*****************************************************************************/
LOCAL IFILE_HANDLE CAFSFS_FindFirstFile(
                                        IFILESYS_T *isfs_ptr, 
                                        const uint16 *find, 
                                        IFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description:  Findnext a file that match the param 'find' in SFS_FindFirstFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       SFS_ERROR_NONE: the next file founded
//       Other: not found
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_FindNextFile(
                                        IFILESYS_T *isfs_ptr, 
                                        IFILE_HANDLE sfs_handle, 
                                        IFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description:  Close the find handle returned by SFS_FindFirstFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_FindClose(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE sfs_handle
                                     );

/*****************************************************************************/
//  Description:  Get file length
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:If success ,the file length is stored in this param
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetFileSize(
                                       IFILESYS_T *isfs_ptr, 
                                       IFILE_HANDLE sfs_handle, 
                                       uint32 *size
                                       );

/*****************************************************************************/
//  Description:  Set file length
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:The length you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
//       If param size is less than file length ,some data in the tail of file will be lost
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetFileSize(
                                       IFILESYS_T *isfs_ptr, 
                                       IFILE_HANDLE sfs_handle, 
                                       uint32 size
                                       );

/*****************************************************************************/
//  Description:  Read file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data has be read will stored in this buffer 
//       bytes_to_read:the number you want to read ,unit is byte
//       bytes_read:the number you has be read infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_ReadFile(
                                    IFILESYS_T *isfs_ptr, 
                                    IFILE_HANDLE handle, 
                                    void *buffer, 
                                    uint32 bytes_to_read, 
                                    uint32 *bytes_read, 
                                    IFILE_OVERLAPPED_T *overlapped_ptr
                                    );

/*****************************************************************************/
//  Description:  Write file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data will be writen is stored in this buffer 
//       bytes_to_write:the number you want to write ,unit is byte
//       bytes_written:the number you has be write infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_WriteFile(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE handle, 
                                     void *buffer, 
                                     uint32 bytes_to_write, 
                                     uint32 *bytes_written, 
                                     IFILE_OVERLAPPED_T *overlapped_ptr
                                     );

/*****************************************************************************/
//  Description:  Set current positon in file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       offset:the relative posion of origin.
//       origin:the absolutely you will be seek.It can be SFS_SEEK_BEGIN ,SFS_SEEK_CUR,SFS_SEEK_END
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetFilePointer(
                                          IFILESYS_T *isfs_ptr, 
                                          IFILE_HANDLE handle, 
                                          int32 offset, 
                                          uint32 origin
                                          );

/*****************************************************************************/
//  Description:  Get current positon in file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       origin:the absolutely you will be find.It can be SFS_SEEK_BEGIN ,SFS_SEEK_END
//       *currentpos:If api success ,this will stored the relative posion of origin.
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetFilePointer(
                                          IFILESYS_T *isfs_ptr, 
                                          IFILE_HANDLE handle, 
                                          int32 origin, 
                                          int32* currentpos
                                          );

/*****************************************************************************/
//  Description:  Close the handler returned by SFS_CreateFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_CloseFile(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE handle
                                     );

/*****************************************************************************/
// 	Description : Get sfs device format type
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: translate from IFILE_FIND_DATA_T to SFS_FIND_DATA_T
/*****************************************************************************/
LOCAL SFS_OVERLAPPED_T ChangeOverLapped(
                                        IFILE_OVERLAPPED_T *overlapped_ptr
                                        );


//added by andrew.zhang at 2009/02/27
/*****************************************************************************/
//  Description:  change fd from outside to internal
//  Author: Andrew.Zhang
//  Param
//       fd_dest_ptr:internal fd struct
//		 fd_sour_ptr:outside fd struct
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CovertParam_FD_OUT_TO_IN(SFS_FIND_DATA_T* fd_dest_ptr, IFILE_FIND_DATA_T * fd_sour_ptr);

/*****************************************************************************/
//  Description:  change fd from internal to outside
//  Author: Andrew.Zhang
//  Param
//       fd_dest_ptr:internal fd struct
//		 fd_sour_ptr:outside fd struct
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CovertParam_FD_IN_TO_OUT(IFILE_FIND_DATA_T * fd_dest_ptr, SFS_FIND_DATA_T * fd_sour_ptr);

/*****************************************************************************/
//  Description:  change file pointer position from outside to internal
//  Author: Andrew.Zhang
//  Param
//       origin:outside file pointer position
//  Return:
//       internal file pointer position
//  Note: 
/*****************************************************************************/
LOCAL uint32 CovertParam_FPP_OUT_TO_IN(uint32 origin);

/*****************************************************************************/
//  Description:  change access mode from outside to internal
//  Author: Andrew.Zhang
//  Param
//       origin:outside mode
//  Return:
//       internal mode
//  Note: 
/*****************************************************************************/
LOCAL uint32 CovertParam_AM_OUT_TO_IN(uint32 mode);


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// functions table
LOCAL const CAF_VTBL_T(IFILESYS_T) s_filesys_vtbl =
{
    CAFSFS_QueryInterface,
        CAFSFS_AddRef,
        CAFSFS_Release,
        CAFSFS_RegisterDevice,
        CAFSFS_UnRegisterDevice,
        CAFSFS_Format,
        CAFSFS_GetDeviceStatus,
        CAFSFS_GetDeviceFreeSpace,
        CAFSFS_GetDeviceUsedSpace,
        CAFSFS_OpenFile,
        CAFSFS_DeleteFile,
        CAFSFS_RenameFile,
        CAFSFS_CreateDirectory,
        CAFSFS_DeleteDirectory, 
        CAFSFS_SetAttr,
        CAFSFS_GetAttr,
        CAFSFS_FindFirstFile,
        CAFSFS_FindNextFile,
        CAFSFS_FindClose,
        CAFSFS_GetFileSize,
        CAFSFS_SetFileSize, 
        CAFSFS_ReadFile,
        CAFSFS_WriteFile,
        CAFSFS_SetFilePointer,
        CAFSFS_GetFilePointer,
        CAFSFS_CloseFile
};

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          functions definition                             */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : query handle message
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
//closed by andrew.zhang at 2009/02/27
#if 0
LOCAL uint32 HandleFile( void )
{
    uint32 operate_result = 0;

    switch(s_result_record.operate_type)
    {
    case IFILE_CREATE:
        operate_result = SFS_CreateFile(s_result_record.operate_param.create_param.file_name, 
            s_result_record.operate_param.create_param.access_mode, 
            s_result_record.operate_param.create_param.share_mode, 
            s_result_record.operate_param.create_param.file_attri);
        //s_result_record.result_callback(TRUE, operate_result, 0);
        break;
    case IFILE_DELETE:
        operate_result = SFS_DeleteFile(s_result_record.operate_param.delete_param.name, &(s_result_record.operate_param.delete_param.sfs_overlapped));
        //s_result_record.result_callback(TRUE, (uint32)GetCafSfsErrorCode(operate_result), (uint32)&(s_result_record.operate_param.delete_param.sfs_overlapped));            
        break;
    case IFILE_RENAME:            
        operate_result = SFS_RenameFile(s_result_record.operate_param.rename_param.sour, s_result_record.operate_param.rename_param.dest, &(s_result_record.operate_param.rename_param.sfs_overlapped));
        //s_result_record.result_callback(TRUE, (uint32)GetCafSfsErrorCode(operate_result), (uint32)&(s_result_record.operate_param.delete_param.sfs_overlapped));
        break;
    default:
        SCI_ASSERT(0);
        break;
    }
    s_result_record.is_operating = FALSE;
    s_result_record.operate_type = IFILE_OPERATE_NONE;
    s_result_record.result_callback = PNULL;

    return operate_result;
}

/*****************************************************************************/
// 	Description : query handle message
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E QueryHandleMsg(CAF_HANDLE_T win_handle, CAF_MESSAGE_ID_E msg_id, void* param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 operate_result = 0;
    switch(msg_id)
    {
    case MSG_APP_OK:		
        switch(s_result_record.operate_type)
        {
        case IFILE_CREATE:
            operate_result = SFS_CreateFile(s_result_record.operate_param.create_param.file_name, 
                s_result_record.operate_param.create_param.access_mode, 
                s_result_record.operate_param.create_param.share_mode, 
                s_result_record.operate_param.create_param.file_attri);
            s_result_record.result_callback(TRUE, operate_result, 0);
            break;
        case IFILE_DELETE:
            operate_result = SFS_DeleteFile(s_result_record.operate_param.delete_param.name, &(s_result_record.operate_param.delete_param.sfs_overlapped));
            s_result_record.result_callback(TRUE, (uint32)GetCafSfsErrorCode(operate_result), (uint32)&(s_result_record.operate_param.delete_param.sfs_overlapped));            
            break;
        case IFILE_RENAME:            
            operate_result = SFS_RenameFile(s_result_record.operate_param.rename_param.sour, s_result_record.operate_param.rename_param.dest, &(s_result_record.operate_param.rename_param.sfs_overlapped));
            s_result_record.result_callback(TRUE, (uint32)GetCafSfsErrorCode(operate_result), (uint32)&(s_result_record.operate_param.delete_param.sfs_overlapped));
            break;
        default:
            SCI_ASSERT(0);
            break;
        }
        s_result_record.is_operating = FALSE;
        s_result_record.operate_type = IFILE_OPERATE_NONE;
        s_result_record.result_callback = PNULL;
		MMK_CloseWin(win_handle);
		break;
	case MSG_APP_RED:
	case MSG_APP_CANCEL:
        switch(s_result_record.operate_type)
        {
        case IFILE_CREATE:                        
        case IFILE_DELETE:            
        case IFILE_RENAME:                        
            s_result_record.result_callback(FALSE, 0, 0);
            break;
        default:
            SCI_ASSERT(0);  
            break;
        }
        s_result_record.is_operating = FALSE;
        s_result_record.operate_type = IFILE_OPERATE_NONE;
        s_result_record.result_callback = PNULL;
		MMK_CloseWin(win_handle);
		break;
    default:
        result = MMIPUB_HandleQueryWinMsg(win_handle,msg_id,param);
        break;
    }
    return (result);
}

#endif


/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSFS_ClassNew( 
                                   CAF_GUID_T guid, 
                                   void**     object_pptr 
                                   );

LOCAL const CAF_STATIC_CLASS_T s_sfs_class_list[] = 
{
    { SPRD_GUID_SFS, CAFSFS_ClassNew, 0 },
};

const CAF_STATIC_CLASS_INFO_T g_sfs_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_sfs_class_list, 
        sizeof(s_sfs_class_list) / sizeof(s_sfs_class_list[0])
};

/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSFS_ClassNew( 
                                   CAF_GUID_T guid, 
                                   void**     object_pptr 
                                   )
{
    CAF_RESULT_E    result  = CAF_RESULT_NOT_SUPPORT;
    CAFSFS_CTRL_T   *sfs_ptr = PNULL;
    
    if( SPRD_GUID_SFS == guid )
    {       
        sfs_ptr = SCI_ALLOC_APP(sizeof(CAFSFS_CTRL_T));
        
        SCI_MEMSET(sfs_ptr, 0, sizeof(CAFSFS_CTRL_T));
        sfs_ptr->vtbl_ptr = (CAF_VTBL_T(IFILESYS_T)*)&s_filesys_vtbl;
        
        sfs_ptr->guid       = guid;
        sfs_ptr->ref_num    = 1;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    *object_pptr = sfs_ptr;
    
    return result;
}

/*****************************************************************************/
// 	Description : Get caf file system error code from sfs
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: translate from SFS_ERROR_E to IFILE_ERROR_E
/*****************************************************************************/
LOCAL IFILE_ERROR_E GetCafSfsErrorCode(
                                       SFS_ERROR_E error_code
                                       )
{
    IFILE_ERROR_E result = IFILE_NO_ERROR;
    
    switch(error_code) 
    {
    case SFS_NO_ERROR:
        result = IFILE_NO_ERROR;
        break;
    case SFS_ERROR_DEVICE:
        result = IFILE_ERROR_DEVICE;
        break;
    case SFS_ERROR_SYSTEM:
        result = IFILE_ERROR_SYSTEM;
        break;
    case SFS_ERROR_INVALID_PARAM:
        result = IFILE_ERROR_INVALID_PARAM;
        break;
    case SFS_ERROR_NO_ENOUGH_RESOURCE:
        result = IFILE_ERROR_NO_ENOUGH_RESOURCE;
        break;
    case SFS_ERROR_IO_PENDING:
        result = IFILE_ERROR_IO_PENDING;
        break;
    case SFS_ERROR_ACCESS:
        result = IFILE_ERROR_ACCESS;
        break;
    case SFS_ERROR_NOT_EXIST:
        result = IFILE_ERROR_NOT_EXIST;
        break;
    case SFS_ERROR_HAS_EXIST:
        result = IFILE_ERROR_HAS_EXIST;
        break;
    case SFS_ERROR_HARDWARE_FAILED:
        result = IFILE_ERROR_HARDWARE_FAILED;
        break;
    case SFS_ERROR_WRITE_CONFLICT:
        result = IFILE_ERROR_WRITE_CONFLICT;
        break;
    case SFS_ERROR_NO_SPACE:
        result = IFILE_ERROR_NO_SPACE;
        break;
        
    default:
        result = IFILE_NO_ERROR;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Get sfs device format type
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: translate from IFILE_FIND_DATA_T to SFS_FIND_DATA_T
/*****************************************************************************/
LOCAL SFS_OVERLAPPED_T ChangeOverLapped(
                                        IFILE_OVERLAPPED_T *overlapped_ptr
                                        )
{
    SFS_OVERLAPPED_T sfs_overlapped = {0};
    if (overlapped_ptr != PNULL)
    {
        sfs_overlapped.complete_route = (SFS_COMPLETETION_ROUTINE)overlapped_ptr->complete_route;
        sfs_overlapped.param = (SFS_PARAM)overlapped_ptr->param;
    }
    
    return sfs_overlapped;
}

/*****************************************************************************/
// 	Description : Get sfs device format type
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: translate from IFILE_DEVICE_FORMAT_E to SFS_DEVICE_FORMAT_E
/*****************************************************************************/
LOCAL SFS_DEVICE_FORMAT_E GetSfsDevFormatType(
                                              IFILE_DEVICE_FORMAT_E dev_format_type
                                              )
{
    SFS_DEVICE_FORMAT_E type = SFS_UNKNOWN_FORMAT;
    
    switch(dev_format_type) 
    {
    case IFILE_FAT12_FORMAT:
        type = SFS_FAT12_FORMAT;
        break;
        
    case IFILE_FAT16_FORMAT:
        type = SFS_FAT16_FORMAT;
        break;
        
    case IFILE_FAT32_FORMAT:
        type = SFS_FAT32_FORMAT;
        break;
        
    case IFILE_AUTO_FORMAT:
        type = SFS_AUTO_FORMAT;
        break;
        
    case IFILE_OTHER_FORMAT:
        type = SFS_OTHER_FORMAT;
        break;
        
    default:
        type = SFS_UNKNOWN_FORMAT;
        break;
    }
    
    return type;
}
/*****************************************************************************/
// 	Description : Get caf file system device format type
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: translate from SFS_DEVICE_FORMAT_E to IFILE_DEVICE_FORMAT_E
/*****************************************************************************/
LOCAL IFILE_DEVICE_FORMAT_E GetCafSfsDevFormatType(
                                                   SFS_DEVICE_FORMAT_E dev_format_type
                                                   )
{
    IFILE_DEVICE_FORMAT_E type = IFILE_UNKNOWN_FORMAT;
    
    switch(dev_format_type) 
    {
    case SFS_FAT12_FORMAT:
        type = IFILE_FAT12_FORMAT;
        break;
        
    case SFS_FAT16_FORMAT:
        type = IFILE_FAT16_FORMAT;
        break;
        
    case SFS_FAT32_FORMAT:
        type = IFILE_FAT32_FORMAT;
        break;
        
    case SFS_AUTO_FORMAT:
        type = IFILE_AUTO_FORMAT;
        break;
        
    case SFS_OTHER_FORMAT:
        type = IFILE_OTHER_FORMAT;
        break;
        
    default:
        type = IFILE_UNKNOWN_FORMAT;
        break;
    }
    return type;
}

/*****************************************************************************/
// 	Description : file system query interface
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSFS_QueryInterface( 
                                         IFILESYS_T *isfs_ptr, 
                                         CAF_GUID_T guid, 
                                         void** ojbect_pptr 
                                         )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    
    CAFSFS_CTRL_T* sfs_ptr = (CAFSFS_CTRL_T*)isfs_ptr;
    
    if ( IUNKNOWN_ID == guid ||  sfs_ptr->guid == guid )
    {
        *ojbect_pptr = isfs_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    if( result == CAF_RESULT_SUCCEEDED )
    {
        IUNKNOWN_AddRef( *ojbect_pptr );
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : file system add ref
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSFS_AddRef(
                           IFILESYS_T * isfs_ptr 
                           )
{
    return ++(((CAFSFS_CTRL_T *)isfs_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : file system release
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSFS_Release(
                            IFILESYS_T * isfs_ptr
                            )
{   
    CAFSFS_CTRL_T * sfs_ptr = (CAFSFS_CTRL_T *)isfs_ptr;
    
    if ( --sfs_ptr->ref_num )
    {
        return( sfs_ptr->ref_num );
    }
    else
    {
        
        SCI_FREE( sfs_ptr );   
        
        return 0;
    }
}

/*****************************************************************************/
//  Description:   Mount a disk to file system
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_ptr:if Api success,this will indicate the type of  file system
//  Return:
//       SFS_ERROR_NONE: Success
//       other: Fail
//  Note:           
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_RegisterDevice(
                                          IFILESYS_T *isfs_ptr, 
                                          const uint16 * dev_name, 
                                          IFILE_DEVICE_FORMAT_E *format_ptr
                                          )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    SFS_DEVICE_FORMAT_E format_type = SFS_UNKNOWN_FORMAT;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_RegisterDevice(dev_name, &format_type);
    
    *format_ptr = GetCafSfsDevFormatType(format_type);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Unmount a disk to file system
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note:           
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_UnRegisterDevice(
                                            IFILESYS_T *isfs_ptr, 
                                            const uint16 * dev_name
                                            )
{
    SFS_ERROR_E result = SFS_ERROR_NONE;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_UnRegisterDevice(dev_name);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Format disk
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_type:the type you want to format.But note that some disk can
//       only be format one type
//       overlapped_ptr:asynchronism param,if you give this parm,the result 
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result 
//                will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       If success the disk will be mounted automatically 
//       If fail the disk will be unmounted automatically
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_Format(
                                  IFILESYS_T *isfs_ptr, 
                                  const uint16 *dev_name,
                                  IFILE_DEVICE_FORMAT_E format_type, 
                                  IFILE_OVERLAPPED_T *overlapped_ptr
                                  )
{
    SFS_ERROR_E result = SFS_ERROR_NONE;
    SFS_DEVICE_FORMAT_E type = GetSfsDevFormatType(format_type);
    SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);
    UNUSED_PARAM(isfs_ptr);

    if(overlapped_ptr == PNULL)
        result = SFS_Format(dev_name, type, PNULL);
    else
        result = SFS_Format(dev_name, type, &sfs_overlapped);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Get disk status
//  Author: Xiaoqing.Lu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: The disk has be mounted
//       Other: The disk has not be mounted
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceStatus(
                                           IFILESYS_T *isfs_ptr, 
                                           const uint16* dev_name
                                           )
{
    SFS_ERROR_E result = SFS_ERROR_NONE;
    UNUSED_PARAM(isfs_ptr);
    result = SFS_GetDeviceStatus(dev_name);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Get disk free space
//  Author: Xiaoqing.Lu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of free space
//       low_word:the low 32 bit value of free space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceFreeSpace(
                                              IFILESYS_T *isfs_ptr, 
                                              const uint16 *device_name,
                                              uint32 *high_word,
                                              uint32 *low_word
                                              )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_GetDeviceFreeSpace(device_name, high_word, low_word);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Get disk used space
//  Author: Xiaoqing.Lu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of used space
//       low_word:the low 32 bit value of used space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetDeviceUsedSpace(
                                              IFILESYS_T *isfs_ptr, 
                                              const uint16 *device_name,
                                              uint32 *high_word,
                                              uint32 *low_word
                                              )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_GetDeviceUsedSpace(device_name, high_word, low_word);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Xiaoqing.Lu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved,Must be NULL,File System have not realize it
//       file_attri:reserved,Must be NULL,File System have not realize it 
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_ERROR_E CAFSFS_OpenFile(
                                    IFILESYS_T *isfs_ptr, 
                                    const uint16 *file_name, 
                                    uint32 access_mode, 
                                    uint32 share_mode, 
                                    uint32 file_attri,
                                    IFILE_RESULTCALLBACK result_callback_func
                                    )
{
    IFILE_ERROR_E result = IFILE_ERROR_NONE;
    //SCI_ASSERT(PNULL != result_callback_func);
    SCI_ASSERT(FALSE == s_result_record.is_operating);
    s_result_record.is_operating = TRUE;
    s_result_record.operate_type = IFILE_CREATE;
    s_result_record.result_callback = result_callback_func;
    s_result_record.operate_param.create_param.isfs_ptr = isfs_ptr;
    SCI_ASSERT(CAFSFS_STR_LENGHT >= MMIAPICOM_Wstrlen(file_name));
    MMIAPICOM_Wstrncpy( s_result_record.operate_param.create_param.file_name, file_name, MMIAPICOM_Wstrlen(file_name));
    s_result_record.operate_param.create_param.access_mode = access_mode;
    s_result_record.operate_param.create_param.share_mode = share_mode;
    s_result_record.operate_param.create_param.file_attri = file_attri;
    
    result = HandleFile();
    //MMIPUB_OpenQueryWinByTextId(TXT_ADD, IMAGE_PUBWIN_QUERY, PNULL, (MMIPUB_HANDLE_FUNC)QueryHandleMsg);
    
    return (IFILE_ERROR_E)result;
}
#else
LOCAL IFILE_HANDLE CAFSFS_OpenFile(
                                    IFILESYS_T *isfs_ptr, 
                                    const uint16 *file_name, 
                                    uint32 access_mode, 
                                    uint32 share_mode, 
                                    uint32 file_attri
                                    )
{
    //SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
    IFILE_HANDLE handle = NULL;
 
    UNUSED_PARAM(isfs_ptr);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);
    if(TransferFileNameFromCAFToSys(sysfilename, file_name) == FALSE) {
        SCI_FREE(sysfilename);
        return NULL;
    }

    //handle = SFS_CreateFile(sysfilename, access_mode, share_mode, file_attri);
    handle = SFS_CreateFile(sysfilename, CovertParam_AM_OUT_TO_IN(access_mode), share_mode, file_attri);
		
    SCI_FREE(sysfilename);
	
    return handle;
}
#endif


/*****************************************************************************/
//  Description:   Delete a file
//  Author: Xiaoqing.Lu
//  Param
//       name :The name of path and file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result 
//                will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_ERROR_E CAFSFS_DeleteFile(
                                      IFILESYS_T *isfs_ptr, 
                                      const uint16 *name, 
                                       IFILE_OVERLAPPED_T *overlapped_ptr,
                                       IFILE_RESULTCALLBACK result_callback_func
                                       )
{
    SFS_ERROR_E result = IFILE_NO_ERROR;
    SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);
    
    //SCI_ASSERT(PNULL != result_callback_func);
    SCI_ASSERT(FALSE == s_result_record.is_operating);
    s_result_record.is_operating = TRUE;
    s_result_record.operate_type = IFILE_DELETE;
    s_result_record.result_callback = result_callback_func;
    s_result_record.operate_param.delete_param.isfs_ptr = isfs_ptr;
    SCI_ASSERT(CAFSFS_STR_LENGHT >= MMIAPICOM_Wstrlen(name));
    MMIAPICOM_Wstrncpy( s_result_record.operate_param.delete_param.name, name, MMIAPICOM_Wstrlen(name));
    SCI_MEMCPY(&(s_result_record.operate_param.delete_param.sfs_overlapped), &sfs_overlapped, sizeof(sfs_overlapped));    
    
    result = HandleFile();
    //MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, PNULL, (MMIPUB_HANDLE_FUNC)QueryHandleMsg);
    
    return result;
}
#else
LOCAL IFILE_ERROR_E CAFSFS_DeleteFile(
                                      IFILESYS_T *isfs_ptr, 
                                      const uint16 *name, 
                                       IFILE_OVERLAPPED_T *overlapped_ptr
                                       )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
    SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);

    UNUSED_PARAM(isfs_ptr);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);
    if(TransferFileNameFromCAFToSys(sysfilename, name) == FALSE) {
        SCI_FREE(sysfilename);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
        return IFILE_ERROR_NO_ENOUGH_RESOURCE;
    }

    if(overlapped_ptr == PNULL)
        result = SFS_DeleteFile(sysfilename, PNULL);
    else
        result = SFS_DeleteFile(sysfilename, &sfs_overlapped);
	
    SCI_FREE(sysfilename);
	
    return GetCafSfsErrorCode(result);
}
#endif

/*****************************************************************************/
//  Description:   Rename a file
//  Author: Xiaoqing.Lu
//  Param
//       sour:The name of path and source file ,must be unicode string
//       dest:The name of path and dest file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value
//       will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result
//                   will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       the path of sourcename and destname must be same
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_ERROR_E CAFSFS_RenameFile(
                                      IFILESYS_T *isfs_ptr,
                                      const uint16 *sour,
                                      const uint16 *dest,
                                       IFILE_OVERLAPPED_T *overlapped_ptr,
                                       IFILE_RESULTCALLBACK result_callback_func
                                       )
{
    SFS_ERROR_E result = IFILE_NO_ERROR;
    SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);

    //SCI_ASSERT(PNULL != result_callback_func);
    SCI_ASSERT(FALSE == s_result_record.is_operating);
    s_result_record.is_operating = TRUE;
    s_result_record.operate_type = IFILE_RENAME;
    s_result_record.result_callback = result_callback_func;
    s_result_record.operate_param.rename_param.isfs_ptr = isfs_ptr;
    SCI_ASSERT(CAFSFS_STR_LENGHT >= MMIAPICOM_Wstrlen(sour));
    MMIAPICOM_Wstrncpy( s_result_record.operate_param.rename_param.sour, sour, MMIAPICOM_Wstrlen(sour));
    SCI_ASSERT(CAFSFS_STR_LENGHT >= MMIAPICOM_Wstrlen(dest));
    MMIAPICOM_Wstrncpy( s_result_record.operate_param.rename_param.dest, dest, MMIAPICOM_Wstrlen(dest));    
    SCI_MEMCPY(&(s_result_record.operate_param.delete_param.sfs_overlapped), &sfs_overlapped, sizeof(sfs_overlapped));        

    result = HandleFile();
    //MMIPUB_OpenQueryWinByTextId(TXT_DC_INPUTNAME, IMAGE_PUBWIN_QUERY, PNULL, (MMIPUB_HANDLE_FUNC)QueryHandleMsg);
    
    return result;
}
#else
LOCAL IFILE_ERROR_E CAFSFS_RenameFile(
                                      IFILESYS_T *isfs_ptr,
                                      const uint16 *sour,
                                      const uint16 *dest,
                                       IFILE_OVERLAPPED_T *overlapped_ptr
                                       )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename_sour = NULL;
    uint16 *sysfilename_dest = NULL;
    SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);
	
    UNUSED_PARAM(isfs_ptr);

    sysfilename_sour = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    sysfilename_dest = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);

    SCI_ASSERT(sysfilename_sour != NULL);
    SCI_ASSERT(sysfilename_dest != NULL);
	
    if(TransferFileNameFromCAFToSys(sysfilename_sour, sour) == FALSE) {
        SCI_FREE(sysfilename_sour);
        SCI_FREE(sysfilename_dest);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
        return IFILE_ERROR_NO_ENOUGH_RESOURCE;
    }

    if(TransferFileNameFromCAFToSys(sysfilename_dest, dest) == FALSE) {
        SCI_FREE(sysfilename_sour);
        SCI_FREE(sysfilename_dest);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
        return IFILE_ERROR_NO_ENOUGH_RESOURCE;
    }

    if(overlapped_ptr == PNULL)
        result = SFS_RenameFile(sysfilename_sour, sysfilename_dest, PNULL);
    else
        result = SFS_RenameFile(sysfilename_sour, sysfilename_dest, &sfs_overlapped);
	
    SCI_FREE(sysfilename_sour);
    SCI_FREE(sysfilename_dest);
	
    return GetCafSfsErrorCode(result);
}
#endif

/*****************************************************************************/
//  Description:   Create a directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_ERROR_E CAFSFS_CreateDirectory(
                                           IFILESYS_T *isfs_ptr,
                                           const uint16 *path
                                           )
{
    SFS_ERROR_E result = IFILE_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_CreateDirectory(path);
    
    return GetCafSfsErrorCode(result);
}
#else
LOCAL IFILE_ERROR_E CAFSFS_CreateDirectory(
                                           IFILESYS_T *isfs_ptr,
                                           const uint16 *path
                                           )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
	
    UNUSED_PARAM(isfs_ptr);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);
    if(TransferFileNameFromCAFToSys(sysfilename, path) == FALSE) {
        SCI_FREE(sysfilename);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
        return IFILE_ERROR_NO_ENOUGH_RESOURCE;
    }
	
    result = SFS_CreateDirectory(sysfilename);
    SCI_FREE(sysfilename);
	
    return GetCafSfsErrorCode(result);
}
#endif

/*****************************************************************************/
//  Description:   Delete a directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_ERROR_E CAFSFS_DeleteDirectory(
                                           IFILESYS_T *isfs_ptr, 
                                           const uint16 *path
                                           )
{
    SFS_ERROR_E result = IFILE_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_DeleteDirectory(path);
    
    return GetCafSfsErrorCode(result);
}
#else
LOCAL IFILE_ERROR_E CAFSFS_DeleteDirectory(
                                           IFILESYS_T *isfs_ptr, 
                                           const uint16 *path
                                           )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
	
    UNUSED_PARAM(isfs_ptr);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);
    if(TransferFileNameFromCAFToSys(sysfilename, path) == FALSE) {
        SCI_FREE(sysfilename);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
        return IFILE_ERROR_NO_ENOUGH_RESOURCE;
    }
	
    result = SFS_DeleteDirectory(sysfilename);
    SCI_FREE(sysfilename);
	
    return GetCafSfsErrorCode(result);
}
#endif

/*****************************************************************************/
//  Description:   Change the attrib of file/directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetAttr(
                                   IFILESYS_T *isfs_ptr,
                                   const uint16 *name,
                                   uint16 attr
                                   )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    UNUSED_PARAM(name);
    UNUSED_PARAM(attr);
    /* 这里暂时要注销掉,因为这个版本的SFS.lib没有包含这个接口，所以编译的时候无法link*/
    // result = SFS_SetAttr(name, attr);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:   Get the attrib of file/directory
//  Author: Xiaoqing.Lu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you get
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetAttr(
                                   IFILESYS_T *isfs_ptr, 
                                   const uint16 *name, 
                                   uint16 *attr
                                   )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    UNUSED_PARAM(name);
    UNUSED_PARAM(attr);
    /* 这里暂时要注销掉,因为这个版本的SFS.lib没有包含这个接口，所以编译的时候无法link*/
    // result = SFS_GetAttr(name, attr);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Find a file that match the param 'find'
//  Author: Xiaoqing.Lu
//  Param
//       find:The file name you want to find,such as "*.mp3","*.3gp",or "123.mp3"...
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       None zero: the file founded ,this find handle can used to find next file match the param 'find'
//       zero: not found
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/02/27
#if 0
LOCAL IFILE_HANDLE CAFSFS_FindFirstFile(
                                        IFILESYS_T *isfs_ptr, 
                                        const uint16 *find, 
                                        IFILE_FIND_DATA_T *fd
                                        )
{
    SFS_HANDLE result = 0;
    UNUSED_PARAM(isfs_ptr);
    result = SFS_FindFirstFile(find, (SFS_FIND_DATA_T*)fd);
    
    return (SFS_HANDLE)(result);
}
#else
LOCAL IFILE_HANDLE CAFSFS_FindFirstFile(
                                        IFILESYS_T *isfs_ptr, 
                                        const uint16 *find, 
                                        IFILE_FIND_DATA_T *fd
                                        )
{
    //SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
    IFILE_HANDLE handle = NULL;
	SFS_FIND_DATA_T sfs_find_data = {0};
	
    UNUSED_PARAM(isfs_ptr);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);
    if(TransferFileNameFromCAFToSys(sysfilename, find) == FALSE) {
        SCI_FREE(sysfilename);
        //return (GetCafSfsErrorCode(IFILE_ERROR_NO_SPACE));
		return NULL;
	}

    //handle = SFS_FindFirstFile(sysfilename, (SFS_FIND_DATA_T*)fd);
    handle = SFS_FindFirstFile(sysfilename, (SFS_FIND_DATA_T*)&sfs_find_data);

	CovertParam_FD_IN_TO_OUT(fd, (SFS_FIND_DATA_T*)&sfs_find_data);

    //由于返回的是相对路径，所以不需要转换
    //if(handle != NULL) {
    //    TransferFileNameFromSysToCAF(sysfilename, (const uint16 *)fd->name);
    //    wcscpy((uint16 *)fd->name, (const uint16 *)sysfilename);
    //}

    SCI_FREE(sysfilename);
	
    return handle;
}
#endif

/*****************************************************************************/
//  Description:  Findnext a file that match the param 'find' in SFS_FindFirstFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       SFS_ERROR_NONE: the next file founded
//       Other: not found
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_FindNextFile(
                                        IFILESYS_T *isfs_ptr, 
                                        IFILE_HANDLE sfs_handle, 
                                        IFILE_FIND_DATA_T *fd
                                        )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint16 *sysfilename = NULL;
	SFS_FIND_DATA_T sfs_find_data = {0};
		
    UNUSED_PARAM(isfs_ptr);

    SCI_ASSERT(NULL != fd);

    sysfilename = SCI_ALLOC(CAFSFS_STR_LENGHT*2 + 4);
    SCI_ASSERT(sysfilename != NULL);

	CovertParam_FD_OUT_TO_IN((SFS_FIND_DATA_T *)(&sfs_find_data), fd);
	
    result = SFS_FindNextFile((SFS_HANDLE)sfs_handle, (SFS_FIND_DATA_T*)&sfs_find_data);

	CovertParam_FD_IN_TO_OUT(fd, (SFS_FIND_DATA_T*)&sfs_find_data);

    //由于返回的是相对路径，所以不需要转换
    //if(GetCafSfsErrorCode(result) == IFILE_NO_ERROR) {
    //    TransferFileNameFromSysToCAF(sysfilename, (const uint16 *)fd->name);
    //    wcscpy((uint16 *)fd->name, (const uint16 *)sysfilename);
    //}
	
    SCI_FREE(sysfilename);
	
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Close the find handle returned by SFS_FindFirstFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_FindClose(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE sfs_handle
                                     )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_FindClose((SFS_HANDLE)sfs_handle);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Get file length
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:If success ,the file length is stored in this param
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetFileSize(
                                       IFILESYS_T *isfs_ptr, 
                                       IFILE_HANDLE sfs_handle, 
                                       uint32 *size
                                       )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_GetFileSize((SFS_HANDLE)sfs_handle, size);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Set file length
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:The length you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
//       If param size is less than file length ,some data in the tail of file will be lost
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetFileSize(
                                       IFILESYS_T *isfs_ptr, 
                                       IFILE_HANDLE sfs_handle, 
                                       uint32 size
                                       )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_SetFileSize((SFS_HANDLE)sfs_handle, size);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Read file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data has be read will stored in this buffer 
//       bytes_to_read:the number you want to read ,unit is byte
//       bytes_read:the number you has be read infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_ReadFile(
                                    IFILESYS_T *isfs_ptr, 
                                    IFILE_HANDLE handle, 
                                    void *buffer, 
                                    uint32 bytes_to_read, 
                                    uint32 *bytes_read, 
                                    IFILE_OVERLAPPED_T *overlapped_ptr
                                    )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    //SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);
    
    UNUSED_PARAM(overlapped_ptr);
    UNUSED_PARAM(isfs_ptr);
    result = SFS_ReadFile(handle, buffer, bytes_to_read, bytes_read, /*&sfs_overlapped*/ PNULL );
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Write file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data will be writen is stored in this buffer 
//       bytes_to_write:the number you want to write ,unit is byte
//       bytes_written:the number you has be write infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_WriteFile(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE handle, 
                                     void *buffer, 
                                     uint32 bytes_to_write, 
                                     uint32 *bytes_written, 
                                     IFILE_OVERLAPPED_T *overlapped_ptr
                                     )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    //SFS_OVERLAPPED_T sfs_overlapped = ChangeOverLapped(overlapped_ptr);

    UNUSED_PARAM(overlapped_ptr);    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_WriteFile(handle, buffer, bytes_to_write, bytes_written, /*&sfs_overlapped*/PNULL);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Set current positon in file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       offset:the relative posion of origin.
//       origin:the absolutely you will be seek.It can be SFS_SEEK_BEGIN ,SFS_SEEK_CUR,SFS_SEEK_END
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_SetFilePointer(
                                          IFILESYS_T *isfs_ptr, 
                                          IFILE_HANDLE handle, 
                                          int32 offset, 
                                          uint32 origin
                                          )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    //result = SFS_SetFilePointer(handle, offset, origin);
    result = SFS_SetFilePointer(handle, (__int64)offset, CovertParam_FPP_OUT_TO_IN(origin));
		
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Get current positon in file
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       origin:the absolutely you will be find.It can be SFS_SEEK_BEGIN ,SFS_SEEK_END
//       *currentpos:If api success ,this will stored the relative posion of origin.
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_GetFilePointer(
                                          IFILESYS_T *isfs_ptr, 
                                          IFILE_HANDLE handle, 
                                          int32 origin, 
                                          int32* currentpos
                                          )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    //result = SFS_GetFilePointer(handle,	origin,	currentpos);
    result = SFS_GetFilePointer(handle, CovertParam_FPP_OUT_TO_IN(origin), currentpos);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Close the handler returned by SFS_CreateFile
//  Author: Xiaoqing.Lu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL IFILE_ERROR_E CAFSFS_CloseFile(
                                     IFILESYS_T *isfs_ptr, 
                                     IFILE_HANDLE handle
                                     )
{
    SFS_ERROR_E result = SFS_NO_ERROR;
    
    UNUSED_PARAM(isfs_ptr);
    result = SFS_CloseFile((SFS_HANDLE)handle);
    
    return GetCafSfsErrorCode(result);
}

/*****************************************************************************/
//  Description:  Transfer CAF file name to system file name
//  Author: Andrew.Zhang
//  Param
//       sour_ptr: CAF file name 
//		 dest_ptr: system file name
//  Return:
//       TRUE:  Success
//       FALSE: Fail
//  Note: 
/*****************************************************************************/
BOOLEAN TransferFileNameFromCAFToSys(uint16 *dest_ptr, const uint16 *sour_ptr)
{
    uint16 *p1_ptr = NULL;
    uint16 *p2_ptr = NULL;
    char *p3_ptr = NULL;
    int32 pathmode = -1; 	//传入的路径模式，1为相对路径(如：test.txt)；2为D盘绝对路径(如：fs:\test.txt)；3为E盘绝对路径(如：fs:\e:\test.txt)
    uint32 len = 0;
    wchar D_disk[] = {'D', 0};
    
    if((sour_ptr == NULL) || (dest_ptr == NULL))
    {
        return FALSE;
    }
    
    len = MMIAPICOM_Wstrlen((wchar *)sour_ptr);
    //SCI_ASSERT(len > 0);
    if(0 == len)
    {
        return FALSE;
    }
    
    //处理特殊目录情况
    if(len >= SPECIAL_DIRECTORY_D_INSTALL_LEN)
    {
        if((uint16)GUI_WstrToGB((uint8 *)dest_ptr, sour_ptr, SPECIAL_DIRECTORY_D_INSTALL_LEN) != SPECIAL_DIRECTORY_D_INSTALL_LEN)
        {
            return FALSE;
        }
        p3_ptr = (char *)dest_ptr;
        *(p3_ptr + SPECIAL_DIRECTORY_D_INSTALL_LEN) = 0;
        
        if(0 == strcmp(p3_ptr, (char *)SPECIAL_DIRECTORY_D_INSTALL))
        {
            p1_ptr = dest_ptr;
            p2_ptr = (uint16 *)sour_ptr + CAF_DISK_D_PREFIX_LEN;
            while(0 != *p2_ptr)
            {
                *p1_ptr++ = *p2_ptr++;
            }
            *p1_ptr = 0;

            //将盘符转换为大写
            *dest_ptr = D_disk[0];
            
            return TRUE;
        }
    }
    
    if(len < CAF_DISK_D_PREFIX_LEN)
    {
        pathmode = PATH_MODE_NONE_PREFIX;
    }
    else if((len >= CAF_DISK_D_PREFIX_LEN) && (len < CAF_DISK_E_PREFIX_LEN))
    {
        if((uint16)GUI_WstrToGB((uint8 *)dest_ptr, sour_ptr, CAF_DISK_D_PREFIX_LEN) != CAF_DISK_D_PREFIX_LEN)
        {
            return FALSE;
        }
        p3_ptr = (char *)dest_ptr;
        *(p3_ptr + CAF_DISK_D_PREFIX_LEN) = 0;
        
        if(0 == strcmp(p3_ptr, (char *)CAF_DISK_D_PREFIX)) //判断是否是D盘路径
        {
            pathmode = PATH_MODE_D_PREFIX;
        }
        else
        {
            pathmode = PATH_MODE_NONE_PREFIX; //相对路径
        }
    }
    else
    {
        if((uint16)GUI_WstrToGB((uint8 *)dest_ptr, sour_ptr, CAF_DISK_E_PREFIX_LEN) != CAF_DISK_E_PREFIX_LEN)
        {
            return FALSE;
        }
        p3_ptr = (char *)dest_ptr;
        *(p3_ptr + CAF_DISK_E_PREFIX_LEN) = 0;
        
        if(0 == strcmp(p3_ptr, (char *)CAF_DISK_E_PREFIX)) //判断是否是E盘路径
        {
            pathmode = PATH_MODE_E_PREFIX;
        }
        else
        {
            *(p3_ptr + CAF_DISK_D_PREFIX_LEN) = 0;
            if(0 == strcmp(p3_ptr, (char *)CAF_DISK_D_PREFIX)) //判断是否是D盘路径
            {
                pathmode = PATH_MODE_D_PREFIX;
            }
            else
            {
                pathmode = PATH_MODE_NONE_PREFIX; //相对路径
            }
        }
    }
    
    if(pathmode == PATH_MODE_NONE_PREFIX) {
        if((uint16)GUI_GBToWstr(dest_ptr, (const uint8 *)D_DIRECTORY, D_DIRECTORY_LEN) != D_DIRECTORY_LEN)
        {
            return FALSE;
        }
        p1_ptr = dest_ptr + D_DIRECTORY_LEN;
        p2_ptr = (uint16 *)sour_ptr;
        while(0 != *p2_ptr)
        {
            *p1_ptr++ = *p2_ptr++;
        }
        *p1_ptr = 0;
    }
    else if(pathmode == PATH_MODE_D_PREFIX)
    {
        if((uint16)GUI_GBToWstr(dest_ptr, (const uint8 *)D_DIRECTORY, D_DIRECTORY_LEN) != D_DIRECTORY_LEN)
        {
            return FALSE;
        }
        p1_ptr = dest_ptr + D_DIRECTORY_LEN;
        p2_ptr = (uint16 *)sour_ptr + CAF_DISK_D_PREFIX_LEN;
        while(0 != *p2_ptr)
        {
            *p1_ptr++ = *p2_ptr++;
        }
        *p1_ptr = 0;
    }
    else if(pathmode == PATH_MODE_E_PREFIX)
    {
        if((uint16)GUI_GBToWstr(dest_ptr, (const uint8 *)E_DIRECTORY, E_DIRECTORY_LEN) != E_DIRECTORY_LEN)
        {
            return FALSE;
        }
        p1_ptr = dest_ptr + E_DIRECTORY_LEN;
        p2_ptr = (uint16 *)sour_ptr + CAF_DISK_E_PREFIX_LEN;
        while(0 != *p2_ptr)
        {
            *p1_ptr++ = *p2_ptr++;
        }
        *p1_ptr = 0;
    }
    else
    {
        SCI_ASSERT(0);
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description:  Transfer system file name to CAF file name
//  Author: Andrew.Zhang
//  Param
//       sour_ptr: system file name 
//		 dest_ptr: CAF file name
//  Return:
//       TRUE:  Success
//       FALSE: Fail
//  Note: 
/*****************************************************************************/
//此函数暂没使用
#if 0
extern BOOLEAN TransferFileNameFromSysToCAF(uint16 *dest_ptr, const uint16 *sour_ptr)
{
	uint16 *p1 = NULL;
	uint16 *p2 = NULL;
	char *sysfilename = NULL;

	if((sour_ptr == NULL) || (dest_ptr == NULL))
		return FALSE;

	sysfilename = SCI_ALLOC(D_DIRECTORY_LEN*2 + 4);
	SCI_ASSERT(sysfilename != NULL);

	if((uint16)GUI_WstrToGB(sysfilename, sour_ptr, D_DIRECTORY_LEN) != D_DIRECTORY_LEN) {
		SCI_FREE(sysfilename);
		return FALSE;
	}

	sysfilename[D_DIRECTORY_LEN] = 0;
	//判断是否是D盘路径
	if(strcmp(sysfilename, (char *)D_DIRECTORY) == 0) {
		if((uint16)GUI_GBToWstr(dest_ptr, (uint8 *)CAF_DISK_D_PREFIX, CAF_DISK_D_PREFIX_LEN) != CAF_DISK_D_PREFIX_LEN) {
			SCI_FREE(sysfilename);
			return FALSE;
		}
		p1 = dest_ptr + CAF_DISK_D_PREFIX_LEN;
		p2 = (uint16 *)sour_ptr + D_DIRECTORY_LEN;
		while(*p2 != 0)
			*p1++ = *p2++;
		*p1 = 0;
	}
	else
	{
		sysfilename[E_DIRECTORY_LEN] = 0;

		if(strcmp(sysfilename, (char *)E_DIRECTORY) == 0) {
			if((uint16)GUI_GBToWstr(dest_ptr, (uint8 *)CAF_DISK_E_PREFIX, CAF_DISK_E_PREFIX_LEN) != CAF_DISK_E_PREFIX_LEN) {
				SCI_FREE(sysfilename);
				return FALSE;
			}
			p1 = dest_ptr + CAF_DISK_E_PREFIX_LEN;
			p2 = (uint16 *)sour_ptr + E_DIRECTORY_LEN;
			while(*p2 != 0)
				*p1++ = *p2++;
			*p1 = 0;
		}
		else
		{
			SCI_ASSERT(0);
		}
	}

	SCI_FREE(sysfilename);

	return TRUE;
}
#endif


/*****************************************************************************/
//  Description:  change fd from outside to internal
//  Author: Andrew.Zhang
//  Param
//       fd_dest_ptr:internal fd struct
//		 fd_sour_ptr:outside fd struct
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CovertParam_FD_OUT_TO_IN(SFS_FIND_DATA_T* fd_dest_ptr, IFILE_FIND_DATA_T * fd_sour_ptr)
{
    SCI_ASSERT(NULL != fd_dest_ptr);
    SCI_ASSERT(NULL != fd_sour_ptr);

	//参数转换
	fd_dest_ptr->create_Date.mday = fd_sour_ptr->create_Date.mday;
	fd_dest_ptr->create_Date.mon = fd_sour_ptr->create_Date.mon;
	fd_dest_ptr->create_Date.year = fd_sour_ptr->create_Date.year;
	
	fd_dest_ptr->create_time.hour = fd_sour_ptr->create_time.hour;
	fd_dest_ptr->create_time.min= fd_sour_ptr->create_time.min;
	fd_dest_ptr->create_time.sec= fd_sour_ptr->create_time.sec;

	fd_dest_ptr->modify_Date.mday = fd_sour_ptr->modify_Date.mday;
	fd_dest_ptr->modify_Date.mon = fd_sour_ptr->modify_Date.mon;
	fd_dest_ptr->modify_Date.year = fd_sour_ptr->modify_Date.year;
	
	fd_dest_ptr->modify_time.hour = fd_sour_ptr->modify_time.hour;
	fd_dest_ptr->modify_time.min= fd_sour_ptr->modify_time.min;
	fd_dest_ptr->modify_time.sec= fd_sour_ptr->modify_time.sec;

	fd_dest_ptr->access_date.mday = fd_sour_ptr->access_date.mday;
	fd_dest_ptr->access_date.mon = fd_sour_ptr->access_date.mon;
	fd_dest_ptr->access_date.year = fd_sour_ptr->access_date.year;
	
	fd_dest_ptr->attr = fd_sour_ptr->attr;
	fd_dest_ptr->length = fd_sour_ptr->length;

	//MMIAPICOM_Wstrcpy(fd_dest_ptr->name, fd_sour_ptr->name);
	MMIAPICOM_Wstrncpy(fd_dest_ptr->name, fd_sour_ptr->name, IFILE_MAX_PATH);
	//strcpy((char *)fd_dest_ptr->short_name, (char *)fd_sour_ptr->short_name);
	strncpy((char *)fd_dest_ptr->short_name, (char *)fd_sour_ptr->short_name, IFILE_MAX_SHORT_FILENAME_LEN);
	
    return TRUE;
}

/*****************************************************************************/
//  Description:  change fd from internal to outside
//  Author: Andrew.Zhang
//  Param
//       fd_dest_ptr:internal fd struct
//		 fd_sour_ptr:outside fd struct
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CovertParam_FD_IN_TO_OUT(IFILE_FIND_DATA_T * fd_dest_ptr, SFS_FIND_DATA_T * fd_sour_ptr)
{
    SCI_ASSERT(NULL != fd_dest_ptr);
    SCI_ASSERT(NULL != fd_sour_ptr);

	//参数转换
	fd_dest_ptr->create_Date.mday = fd_sour_ptr->create_Date.mday;
	fd_dest_ptr->create_Date.mon = fd_sour_ptr->create_Date.mon;
	fd_dest_ptr->create_Date.year = fd_sour_ptr->create_Date.year;
	
	fd_dest_ptr->create_time.hour = fd_sour_ptr->create_time.hour;
	fd_dest_ptr->create_time.min= fd_sour_ptr->create_time.min;
	fd_dest_ptr->create_time.sec= fd_sour_ptr->create_time.sec;

	fd_dest_ptr->modify_Date.mday = fd_sour_ptr->modify_Date.mday;
	fd_dest_ptr->modify_Date.mon = fd_sour_ptr->modify_Date.mon;
	fd_dest_ptr->modify_Date.year = fd_sour_ptr->modify_Date.year;
	
	fd_dest_ptr->modify_time.hour = fd_sour_ptr->modify_time.hour;
	fd_dest_ptr->modify_time.min= fd_sour_ptr->modify_time.min;
	fd_dest_ptr->modify_time.sec= fd_sour_ptr->modify_time.sec;

	fd_dest_ptr->access_date.mday = fd_sour_ptr->access_date.mday;
	fd_dest_ptr->access_date.mon = fd_sour_ptr->access_date.mon;
	fd_dest_ptr->access_date.year = fd_sour_ptr->access_date.year;
	
	fd_dest_ptr->attr = fd_sour_ptr->attr;
	fd_dest_ptr->length = fd_sour_ptr->length;

	//MMIAPICOM_Wstrcpy(fd_dest_ptr->name, fd_sour_ptr->name);
	MMIAPICOM_Wstrncpy(fd_dest_ptr->name, fd_sour_ptr->name, IFILE_MAX_PATH);
	//strcpy((char *)fd_dest_ptr->short_name, (char *)fd_sour_ptr->short_name);
	strncpy((char *)fd_dest_ptr->short_name, (char *)fd_sour_ptr->short_name, IFILE_MAX_SHORT_FILENAME_LEN);

    return TRUE;
}


/*****************************************************************************/
//  Description:  change file pointer position from outside to internal
//  Author: Andrew.Zhang
//  Param
//       origin:outside file pointer position
//  Return:
//       internal file pointer position
//  Note: 
/*****************************************************************************/
LOCAL uint32 CovertParam_FPP_OUT_TO_IN(uint32 origin)
{
	uint32 internal_origin = origin;

	switch(origin)
	{
	case IFILE_SEEK_BEGIN:
		internal_origin = SFS_SEEK_BEGIN;
		break;
	case IFILE_SEEK_CUR:
		internal_origin = SFS_SEEK_CUR;
		break;
	case IFILE_SEEK_END:
		internal_origin = SFS_SEEK_END;
		break;
	default:
		internal_origin = origin;
		break;
	}	
	
    return internal_origin;
}


/*****************************************************************************/
//  Description:  change access mode from outside to internal
//  Author: Andrew.Zhang
//  Param
//       origin:outside mode
//  Return:
//       internal mode
//  Note: 
/*****************************************************************************/
LOCAL uint32 CovertParam_AM_OUT_TO_IN(uint32 mode)
{
	uint32 internal_mode = 0;

	if((mode & IFILE_MODE_READ) == IFILE_MODE_READ)
	{
		internal_mode |= SFS_MODE_READ;
	}

	if((mode & IFILE_MODE_WRITE) == IFILE_MODE_WRITE)
	{
		internal_mode |= SFS_MODE_WRITE;
	}

	if((mode & IFILE_MODE_SHARE_READ) == IFILE_MODE_SHARE_READ)
	{
		internal_mode |= SFS_MODE_SHARE_READ;
	}
	
	if((mode & IFILE_MODE_SHARE_WRITE) == IFILE_MODE_SHARE_WRITE)
	{
		internal_mode |= SFS_MODE_SHARE_WRITE;
	}
         
	if((mode & IFILE_MODE_CREATE_NEW) == IFILE_MODE_CREATE_NEW)
	{
		internal_mode |= SFS_MODE_CREATE_NEW;
	}
	
	if((mode & IFILE_MODE_CREATE_ALWAYS) == IFILE_MODE_CREATE_ALWAYS)
	{
		internal_mode |= SFS_MODE_CREATE_ALWAYS;
	}
	
	if((mode & IFILE_MODE_OPEN_EXISTING) == IFILE_MODE_OPEN_EXISTING)
	{
		internal_mode |= SFS_MODE_OPEN_EXISTING;
	}
	
	if((mode & IFILE_MODE_OPEN_ALWAYS) == IFILE_MODE_OPEN_ALWAYS)
	{
		internal_mode |= SFS_MODE_OPEN_ALWAYS;
	}
	
	if((mode & IFILE_MODE_APPEND) == IFILE_MODE_APPEND)
	{
		internal_mode |= SFS_MODE_APPEND;
	}
	
    return internal_mode;
}

#endif


