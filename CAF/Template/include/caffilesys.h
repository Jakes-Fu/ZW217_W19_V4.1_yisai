/****************************************************************************
 ** File Name:      caffilesys.h                                                 *
 ** Author:         Xiaoqing.Lu                                             *
 ** Date:           07/28/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the application modual.   *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 07/2008        Xiaoqing.Lu       Create
 ** 
 ****************************************************************************/

#ifndef  _CAFFILESYS_H_    
#define  _CAFFILESYS_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

typedef uint32  IFILE_HANDLE;

typedef enum ifile_error_tag
{

    IFILE_NO_ERROR                   = 0,	//success
    IFILE_ERROR_NONE                 = 0,

    IFILE_ERROR_DEVICE,				//device error

    IFILE_ERROR_SYSTEM, 				//os error
    /*
        param you give is invalid
    */
    IFILE_ERROR_INVALID_PARAM,
    /*
        memory or else is limited
    */
    IFILE_ERROR_NO_ENOUGH_RESOURCE,

    /*
        when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING 
        immediately.Real error message and return value will be given in callback funtion.
    */
    IFILE_ERROR_IO_PENDING,
    /*
        can not access the file ,maybe it is being used by other handler
    */
    IFILE_ERROR_ACCESS,

    /*
        file or device not exist
    */
    IFILE_ERROR_NOT_EXIST,
    /*
        file or device has exist
    */
    IFILE_ERROR_HAS_EXIST,
    
/**------------------------------------------------------------------*
 **     device-lay error code                                        *
 **------------------------------------------------------------------*/
    IFILE_ERROR_HARDWARE_FAILED,
    IFILE_ERROR_WRITE_CONFLICT,

/**------------------------------------------------------------------*
 **     item-lay error code                                          *
 **------------------------------------------------------------------*/
    IFILE_ERROR_NO_SPACE

}IFILE_ERROR_E;

/*device format*/
typedef enum ifile_device_format_tag
{
	IFILE_FAT12_FORMAT = 0,
	IFILE_FAT16_FORMAT = 1,
	IFILE_FAT32_FORMAT = 2,
	IFILE_AUTO_FORMAT = 6,
	IFILE_OTHER_FORMAT = 7,
	IFILE_UNKNOWN_FORMAT
}IFILE_DEVICE_FORMAT_E;

/*
    Seek Macro define.
*/
#define IFILE_SEEK_BEGIN	0  /*Beginning of file */
#define IFILE_SEEK_CUR		1  /*Current position of file */
#define IFILE_SEEK_END		2  /*End of file   */

#define  IFILE_MAX_PATH     255 //max unicode length
//added by andrew.zhang at 2009/07/21
#define  IFILE_MAX_SHORT_FILENAME_LEN     13 //max short filename length


/**---------------------------------------------------------------------------*
 **    Overlapped struct                                                      *
 
    completed-route :
    void CALLBACK  FileIOCompletionRoutine(
        IFILE_ERROR_E  error,		// error code of the relative opration.
        IFILE_PARAM   irp_param,	// the param set by user.
        IFILE_PARAM	param1,		// result of the relative operation.
        IFILE_PARAM	param1)		// result of the relative operation.
**---------------------------------------------------------------------------*/

typedef uint32  IFILE_PARAM;
typedef void (*IFILE_COMPLETETION_ROUTINE)(IFILE_ERROR_E error, uint32 irp_param, uint32 param1,uint32 param2);
/*
    if create file, the key_ok: whether the OK key of query window be pressed by user.
                        param1: if the key_ok is TRUE, it will be the handle of file.
                        param2: the .
    if delete file, the key_ok: whether the OK key of query window be pressed by user.
                        param1: if the key_ok is TRUE, it will be the result of delete operation, type is IFILE_ERROR_E.
                        param2: reserve.
    if rename file, the key_ok: whether the OK key of query window be pressed by user.
                        param1: if the key_ok is TRUE, it will be the result of rename operation, type is IFILE_ERROR_E.
                        param2: reserve.
 */
typedef void (*IFILE_RESULTCALLBACK)(BOOLEAN key_ok, uint32 param1,uint32 param2);

typedef struct ifile_overlapped_tag
{
    /*
        the point to callback function.
    */
    IFILE_COMPLETETION_ROUTINE   complete_route; 
    /*
        the param set by user,it can be anything,it will be passed to callbackfun without modify 
    */
    IFILE_PARAM                  param;          
}IFILE_OVERLAPPED_T;


/**---------------------------------------------------------------------------*
 **    Open/Create  flag                                                      *
 **                                                                           *
 **      Mode                            Description                          *
 **---------------------------------------------------------------------------*
         SFS_MODE_READ                   Specifies read access to the object. 
                                         Data can be read from the file and
                                         the file pointer can be moved.
   ----------------------------------------------------------------------------                                                
         SFS_MODE_WRITE                  Specifies write access to the object.
                                         Data can be written to the file and 
                                         the file pointer can be moved
   ----------------------------------------------------------------------------
         SFS_MODE_SHARE_READ             Subsequent open operations on the
         (reserved)                      object will succeed only if read 
                                         access is requested.
   ----------------------------------------------------------------------------
         SFS_MODE_SHARE_WRITE            Subsequent open operations on the
         (reserved)                      object will succeed only if write
                                         access is requested
   ----------------------------------------------------------------------------
         SFS_MODE_CREATE_NEW             Creates a new file. 
                                         The function fails if the specified 
                                         file already exists.
   ----------------------------------------------------------------------------      
         SFS_MODE_CREATE_ALWAYS          Creates a new file.
                                         If the file exists, the function 
                                         overwrites the file.
   ----------------------------------------------------------------------------
         SFS_MODE_OPEN_EXISTING          Opens the file.
                                         The function fails if the file 
                                         does not exist.
   ----------------------------------------------------------------------------
         SFS_MODE_OPEN_ALWAYS            Opens the file, if it exists.
                                         If the file does not exist, the function
                                         creates a new file.
   ----------------------------------------------------------------------------
         SFS_MODE_APPEND                 Open/create the file, all write operations 
                                         occur at the end of the file even if set/seek
                                         the file position.
   
 **---------------------------------------------------------------------------*/
typedef enum ifile_mode_tag
{   
    /* 0      1       2             3            4 5 6 7      7......31
    +---------------------------------------------------------------------+
    | read | write | share_read | share_write | create/open | bit 7..31   |
    +---------------------------------------------------------------------+   */

	// Bit 0-1
	IFILE_MODE_READ		    	= 0x0001,  
	IFILE_MODE_WRITE			= 0x0002,
         
	// Bit 2-3
	IFILE_MODE_SHARE_READ		= 0x0004,//reserved jason.wu
	IFILE_MODE_SHARE_WRITE  	= 0x0008,//reserved

	// Bit 4-7
	IFILE_MODE_CREATE_NEW		= 0x0010,
	IFILE_MODE_CREATE_ALWAYS	= 0x0020,
	IFILE_MODE_OPEN_EXISTING	= 0x0030,
	IFILE_MODE_OPEN_ALWAYS  	= 0x0040,
	IFILE_MODE_APPEND			= 0x0050
}IFILE_MODE_E;

typedef struct ifile_find_data_tag
{
	CAF_DATE_T	create_Date;
	CAF_TIME_T	create_time;
	CAF_DATE_T	modify_Date	;
	CAF_TIME_T	modify_time;
	CAF_DATE_T	access_date;
	uint16          attr;                // see FFS_ATTR_E.
	uint32          length;
	wchar          name[IFILE_MAX_PATH+1];
	uint8           short_name[IFILE_MAX_SHORT_FILENAME_LEN];
}IFILE_FIND_DATA_T;

//fat attribute bits
//the value of SFS_FIND_DATA_T->attr,you can use it to identify the file
#define IFILE_ATTR_NONE    0x0 /* no attribute bits */
#define IFILE_ATTR_RO      0x1  /* read-only */
#define IFILE_ATTR_HIDDEN  0x2  /* hidden */
#define IFILE_ATTR_SYS     0x4  /* system */
#define IFILE_ATTR_VOLUME  0x8  /* volume label */	//reserved : you can not use it
#define IFILE_ATTR_DIR     0x10 /* directory */			//can not used in SFS_SetAttr()
#define IFILE_ATTR_ARCH    0x20 /* archived */


typedef struct IFILESYS_T IFILESYS_T;

// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IFILESYS_T)
{
    INHERIT_IUNKNOWN(IFILESYS_T);

    /**---------------------------------------------------------------------------*
    //Device operation
    //note:	if the space of dev_name and overlapped_ptr is allocated by caller,it must 
    //		be free by caller itself after api return.
    //
    **---------------------------------------------------------------------------*/

    IFILE_ERROR_E (*RegisterDevice)(IFILESYS_T *me_ptr, const wchar * dev_name_ptr, IFILE_DEVICE_FORMAT_E * format_ptr);
    IFILE_ERROR_E (*UnRegisterDevice)(IFILESYS_T *me_ptr, const wchar * dev_name_ptr);
    IFILE_ERROR_E (*Format)(IFILESYS_T *me_ptr, const wchar *dev_name_ptr, IFILE_DEVICE_FORMAT_E format_type,IFILE_OVERLAPPED_T *overlapped_ptr);
    IFILE_ERROR_E (*GetDeviceStatus)(IFILESYS_T *me_ptr, const wchar* dev_name_ptr);
    //IFILE_ERROR_E (*GetDeviceFreeSpace)(IFILESYS_T *me_ptr, const wchar *device_name_ptr,uint32 *high_word_ptr,uint32 *low_word_ptr);
    //IFILE_ERROR_E (*GetDeviceUsedSpace)(IFILESYS_T *me_ptr, const wchar *device_name_ptr,uint32 *high_word_ptr,uint32 *low_word_ptr);
    IFILE_ERROR_E (*GetDiskFreeSpace)(IFILESYS_T *me_ptr, const wchar *device_name_ptr,uint32 *high_word_ptr,uint32 *low_word_ptr);
    IFILE_ERROR_E (*GetDiskUsedSpace)(IFILESYS_T *me_ptr, const wchar *device_name_ptr,uint32 *high_word_ptr,uint32 *low_word_ptr);

    /*--------------------------------------------------------*/
    //file and dir operation
    
    //note:	1	if the space of name and overlapped_ptr is allocated by caller,it must 
    //			be free by caller itself after api return.
    //		2	the pathname or filename must like:
    //          "devname:\\foldername1\\foldername2\\filename " in unicode form and nothing else,
    //		3	length of pathname must less than SFS_MAX_PATH
    //			
    /*--------------------------------------------------------*/
    IFILE_HANDLE (*CreateFile)(IFILESYS_T *me_ptr, const wchar *file_name_ptr, uint32 access_mode, uint32 share_mode, uint32 file_attri);
    IFILE_ERROR_E (*DeleteFile)(IFILESYS_T *me_ptr, const wchar *name_ptr, IFILE_OVERLAPPED_T *overlapped_ptr);
    IFILE_ERROR_E (*RenameFile)(IFILESYS_T *me_ptr, const wchar *sour_ptr,const wchar *dest_ptr,IFILE_OVERLAPPED_T *overlapped_ptr);
    IFILE_ERROR_E (*CreateDirectory)(IFILESYS_T *me_ptr, const wchar *path_ptr);
    //IFILE_ERROR_E (*DeleteDirectory)(IFILESYS_T *me_ptr, const wchar *path_ptr);
    IFILE_ERROR_E (*RemoveDirectory)(IFILESYS_T *me_ptr, const wchar *path_ptr);
    IFILE_ERROR_E (*SetAttr)(IFILESYS_T *me_ptr, const wchar *name_ptr, uint16 attr);
    IFILE_ERROR_E (*GetAttr)(IFILESYS_T *me_ptr, const wchar *name_ptr, uint16 *attr_ptr);
    IFILE_HANDLE  (*FindFirstFile)(IFILESYS_T *me_ptr, const wchar *find_ptr, IFILE_FIND_DATA_T *fd_ptr);
    IFILE_ERROR_E (*FindNextFile)(IFILESYS_T *me_ptr, IFILE_HANDLE sfs_handle, IFILE_FIND_DATA_T *fd_ptr);
    IFILE_ERROR_E (*FindClose)(IFILESYS_T *me_ptr, IFILE_HANDLE sfs_handle);

    /*--------------------------------------------------------*/
    //file internal operation
    /*--------------------------------------------------------*/
    IFILE_ERROR_E (*GetFileSize)(IFILESYS_T *me_ptr, IFILE_HANDLE sfs_handle, uint32 *size_ptr);
    IFILE_ERROR_E (*SetFileSize)(IFILESYS_T *me_ptr, IFILE_HANDLE sfs_handle, uint32 size_ptr);
    IFILE_ERROR_E (*ReadFile)(IFILESYS_T *me_ptr, IFILE_HANDLE handle, void *buffer_ptr, uint32 bytes_to_read, uint32 *bytes_read_ptr, IFILE_OVERLAPPED_T *overlapped_ptr);
    IFILE_ERROR_E (*WriteFile)(IFILESYS_T *me_ptr, IFILE_HANDLE handle, void *buffer_ptr, uint32 bytes_to_write, uint32 *bytes_written_ptr, IFILE_OVERLAPPED_T *overlapped_ptr);

    IFILE_ERROR_E (*SetFilePointer)(IFILESYS_T *me_ptr, IFILE_HANDLE handle, int32 offset, uint32 origin);
    IFILE_ERROR_E (*GetFilePointer)(IFILESYS_T *me_ptr, IFILE_HANDLE handle, int32 origin, int32* currentpos_ptr);
    IFILE_ERROR_E (*CloseFile)(IFILESYS_T *me_ptr, IFILE_HANDLE handle);
};


// Define pointer to poit the virtual table of the control base class
struct IFILESYS_T
{
    CAF_VTBL_T(IFILESYS_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

static __inline CAF_RESULT_E IFILE_QueryInterface( IFILESYS_T *me_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T )->QueryInterface( me_ptr, guid, object_pptr );
}

static __inline uint32 IFILE_AddRef( IFILESYS_T *me_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T )->AddRef( me_ptr );
}

static __inline uint32 IFILE_Release( IFILESYS_T *me_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T )->Release( me_ptr );
}

/*****************************************************************************/
//  Description:   Mount a disk to file system
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_ptr:if Api success,this will indicate the type of  file system
//  Return:
//       SFS_ERROR_NONE: Success
//       other: Fail
//  Note:           
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_RegisterDevice(
                                                     IFILESYS_T *me_ptr, 
                                                     const wchar * dev_name_ptr, 
                                                     IFILE_DEVICE_FORMAT_E * format_ptr
                                                     )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->RegisterDevice( me_ptr, dev_name_ptr, format_ptr );
}

/*****************************************************************************/
//  Description:   Unmount a disk to file system
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note:           
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_UnRegisterDevice(
                                                       IFILESYS_T *me_ptr, 
                                                       const wchar * dev_name_ptr
                                                       )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->UnRegisterDevice( me_ptr, dev_name_ptr);
}

/*****************************************************************************/
//  Description:   Format disk
//  Author: Jason.wu
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
static __inline IFILE_ERROR_E IFILE_Format(
                                             IFILESYS_T *me_ptr, 
                                             const wchar *dev_name_ptr,
                                             IFILE_DEVICE_FORMAT_E format_type, 
                                             IFILE_OVERLAPPED_T *overlapped_ptr
                                             )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->Format( me_ptr, dev_name_ptr, format_type, overlapped_ptr );
}

/*****************************************************************************/
//  Description:   Get disk status
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: The disk has be mounted
//       Other: The disk has not be mounted
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_GetDeviceStatus(
                                                      IFILESYS_T *me_ptr, 
                                                      const wchar* dev_name_ptr
                                                      )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->GetDeviceStatus( me_ptr, dev_name_ptr);
}

/*****************************************************************************/
//  Description:   Get disk free space
//  Author: Jason.wu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of free space
//       low_word:the low 32 bit value of free space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/03/04
//static __inline IFILE_ERROR_E IFILE_GetDeviceFreeSpace(
static __inline IFILE_ERROR_E IFILE_GetDiskFreeSpace(
                                                         IFILESYS_T *me_ptr, 
                                                         const wchar *device_name_ptr,
                                                         uint32 *high_word_ptr,
                                                         uint32 *low_word_ptr
                                                         )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->GetDiskFreeSpace(me_ptr, device_name_ptr, high_word_ptr, low_word_ptr);
}

/*****************************************************************************/
//  Description:   Get disk used space
//  Author: Jason.wu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of used space
//       low_word:the low 32 bit value of used space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
//static __inline IFILE_ERROR_E IFILE_GetDeviceUsedSpace(
//modified by andrew.zhang at 2009/03/04
static __inline IFILE_ERROR_E IFILE_GetDiskUsedSpace(
                                                         IFILESYS_T *me_ptr, 
                                                         const wchar *device_name_ptr,
                                                         uint32 *high_word_ptr,
                                                         uint32 *low_word_ptr
                                                         )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->GetDiskUsedSpace(me_ptr, device_name_ptr, high_word_ptr, low_word_ptr);
}

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Jason.wu
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
static __inline IFILE_HANDLE IFILE_CreateFile(
                                             IFILESYS_T *me_ptr, 
                                             const wchar *file_name_ptr, 
                                             uint32 access_mode, 
                                             uint32 share_mode, 
                                             uint32 file_attri
                                             )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->CreateFile(me_ptr, file_name_ptr, access_mode, share_mode, file_attri);
}

/*****************************************************************************/
//  Description:   Delete a file
//  Author: Jason.wu
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
static __inline IFILE_ERROR_E IFILE_DeleteFile(
                                                 IFILESYS_T *me_ptr, 
                                                 const wchar *name_ptr, 
                                                 IFILE_OVERLAPPED_T *overlapped_ptr
                                                 )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->DeleteFile(me_ptr, name_ptr, overlapped_ptr);
}

/*****************************************************************************/
//  Description:   Rename a file
//  Author: Jason.wu
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
static __inline IFILE_ERROR_E IFILE_RenameFile(
                                                 IFILESYS_T *me_ptr,
                                                 const wchar *sour_ptr,
                                                 const wchar *dest_ptr,
                                                 IFILE_OVERLAPPED_T *overlapped_ptr
                                                 )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->RenameFile(me_ptr, sour_ptr, dest_ptr, overlapped_ptr);
}

/*****************************************************************************/
//  Description:   Create a directory
//  Author: Jason.wu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_CreateDirectory(
                                                      IFILESYS_T *me_ptr,
                                                      const wchar *path_ptr
                                                      )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->CreateDirectory(me_ptr, path_ptr);
}

/*****************************************************************************/
//  Description:   Delete a directory
//  Author: Jason.wu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
//modified by andrew.zhang at 2009/03/04
//static __inline IFILE_ERROR_E IFILE_DeleteDirectory(
static __inline IFILE_ERROR_E IFILE_RemoveDirectory(
                                                      IFILESYS_T *me_ptr, 
                                                      const wchar *path_ptr
                                                      )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->RemoveDirectory(me_ptr, path_ptr);
}

/*****************************************************************************/
//  Description:   Change the attrib of file/directory
//  Author: Jason.wu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_SetAttr(
                                              IFILESYS_T *me_ptr,
                                              const wchar *name_ptr,
                                              uint16 attr
                                              )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->SetAttr(me_ptr, name_ptr, attr);
}

/*****************************************************************************/
//  Description:   Get the attrib of file/directory
//  Author: Jason.wu
//  Param
//       path:The name of file/directory you want to change ,must be unicode string
//       attr: The attrib you get
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_GetAttr(
                                            IFILESYS_T *me_ptr, 
                                            const wchar *name_ptr, 
                                            uint16* attr_ptr
                                            )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->GetAttr(me_ptr, name_ptr, attr_ptr);
}

/*****************************************************************************/
//  Description:  Find a file that match the param 'find'
//  Author: Jason.wu
//  Param
//       find:The file name you want to find,such as "*.mp3","*.3gp",or "123.mp3"...
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       None zero: the file founded ,this find handle can used to find next file match the param 'find'
//       zero: not found
//  Note: 
/*****************************************************************************/
static __inline IFILE_HANDLE IFILE_FindFirstFile(
                                                   IFILESYS_T *me_ptr, 
                                                   const wchar *find_ptr, 
                                                   IFILE_FIND_DATA_T *fd_ptr
                                                   )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->FindFirstFile(me_ptr, find_ptr, fd_ptr);
}

/*****************************************************************************/
//  Description:  Findnext a file that match the param 'find' in SFS_FindFirstFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       SFS_ERROR_NONE: the next file founded
//       Other: not found
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_FindNextFile(
                                                   IFILESYS_T *me_ptr, 
                                                   IFILE_HANDLE sfs_handle, 
                                                   IFILE_FIND_DATA_T *fd_ptr
                                                   )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->FindNextFile(me_ptr, sfs_handle, fd_ptr);
}

/*****************************************************************************/
//  Description:  Close the find handle returned by SFS_FindFirstFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_FindClose(
                                                IFILESYS_T *me_ptr, 
                                                IFILE_HANDLE sfs_handle
                                                )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->FindClose(me_ptr, sfs_handle);
}

/*****************************************************************************/
//  Description:  Get file length
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:If success ,the file length is stored in this param
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_GetFileSize(
                                                  IFILESYS_T *me_ptr, 
                                                  IFILE_HANDLE sfs_handle, 
                                                  uint32 *size_ptr
                                                  )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->GetFileSize(me_ptr, sfs_handle, size_ptr);
}

/*****************************************************************************/
//  Description:  Set file length
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:The length you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
//       If param size is less than file length ,some data in the tail of file will be lost
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_SetFileSize(
                                                  IFILESYS_T *me_ptr, 
                                                  IFILE_HANDLE sfs_handle, 
                                                  uint32 truncate_pos
                                                  )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IFILESYS_T)->SetFileSize(me_ptr, sfs_handle, truncate_pos);
}

/*****************************************************************************/
//  Description:  Read file
//  Author: Jason.wu
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
static __inline IFILE_ERROR_E IFILE_ReadFile(
                                               IFILESYS_T *me_ptr, 
                                               IFILE_HANDLE handle, 
                                               void *buffer_ptr, 
                                               uint32 bytes_to_read, 
                                               uint32 *bytes_read_ptr, 
                                               IFILE_OVERLAPPED_T *overlapped_ptr
                                               )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IFILESYS_T)->ReadFile(me_ptr, handle, buffer_ptr, bytes_to_read, bytes_read_ptr, overlapped_ptr);
}

/*****************************************************************************/
//  Description:  Write file
//  Author: Jason.wu
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
static __inline IFILE_ERROR_E IFILE_WriteFile(
                                                IFILESYS_T *me_ptr, 
                                                IFILE_HANDLE handle, 
                                                void *buffer_ptr, 
                                                uint32 bytes_to_write, 
                                                uint32 *bytes_written_ptr, 
                                                IFILE_OVERLAPPED_T *overlapped_ptr
                                                )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IFILESYS_T)->WriteFile(me_ptr, handle, buffer_ptr, bytes_to_write, bytes_written_ptr, overlapped_ptr);
}

/*****************************************************************************/
//  Description:  Set current positon in file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       offset:the relative posion of origin.
//       origin:the absolutely you will be seek.It can be SFS_SEEK_BEGIN ,SFS_SEEK_CUR,SFS_SEEK_END
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_SetFilePointer(
                                                     IFILESYS_T *me_ptr, 
                                                     IFILE_HANDLE handle, 
                                                     int32 offset, 
                                                     uint32 origin
                                                     )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IFILESYS_T)->SetFilePointer(me_ptr, handle, offset, origin);
}

/*****************************************************************************/
//  Description:  Get current positon in file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       origin:the absolutely you will be find.It can be SFS_SEEK_BEGIN ,SFS_SEEK_END
//       *currentpos:If api success ,this will stored the relative posion of origin.
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_GetFilePointer(
                                                     IFILESYS_T *me_ptr, 
                                                     IFILE_HANDLE handle, 
                                                     int32 origin, 
                                                     int32* currentpos_ptr
                                                     )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IFILESYS_T)->GetFilePointer(me_ptr, handle, origin, currentpos_ptr);
}

/*****************************************************************************/
//  Description:  Close the handler returned by SFS_CreateFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline IFILE_ERROR_E IFILE_CloseFile(
                                                IFILESYS_T *me_ptr, 
                                                IFILE_HANDLE handle
                                                )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IFILESYS_T)->CloseFile(me_ptr, handle);
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
BOOLEAN TransferFileNameFromCAFToSys(uint16 *dest_ptr, const uint16 *sour_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFFILESYS_H_
