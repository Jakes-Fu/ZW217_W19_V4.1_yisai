/******************************************************************************
 ** File Name:      sfs.h                                                     *
 ** Author:                                                           *
 ** DATE:           8/16/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SFS.               *
 **                 (FAT File System)                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 8/16/2006               Create.                                   *
 ******************************************************************************/

#ifndef _SFS_H
#define _SFS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*============================================================================
Define basic data types:

==============================================================================*/

typedef uint32               SFS_HANDLE;

typedef enum SFS_ERROR_E_TAG
{

    SFS_NO_ERROR                   = 0,	//success
    SFS_ERROR_NONE                 = 0,

    SFS_ERROR_DEVICE,				//device error

    SFS_ERROR_SYSTEM, 				//os error
    SFS_ERROR_INVALID_PARAM,		//param you give is invalid
    SFS_ERROR_NO_ENOUGH_RESOURCE, 	//memory or else is limited

    SFS_ERROR_IO_PENDING,			//when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING immediately.Real error message and return value will be given in callback funtion.
    SFS_ERROR_ACCESS,				//can not access the file ,maybe it is being used by other handler

    SFS_ERROR_NOT_EXIST,			// file or device not exist
    SFS_ERROR_HAS_EXIST,			// file or device has exist
    
/**------------------------------------------------------------------*
 **     device-lay error code                                        *
 **------------------------------------------------------------------*/
    SFS_ERROR_HARDWARE_FAILED,
    SFS_ERROR_WRITE_CONFLICT,

/**------------------------------------------------------------------*
 **     item-lay error code                                          *
 **------------------------------------------------------------------*/
    SFS_ERROR_NO_SPACE

}SFS_ERROR_E;


typedef enum SFS_DEVICE_FORMAT_E_TAG
{
//	SFS_FAT12_FORMAT = 0,//reserved,jason.wu
	SFS_FAT16_FORMAT = 1,
	SFS_FAT32_FORMAT = 2,
	SFS_OTHER_FORMAT = 7,
	SFS_UNKNOWN_FORMAT
}SFS_DEVICE_FORMAT_E;

/*
    Seek Macro define.
*/
#define SFS_SEEK_BEGIN		0  /*Beginning of file */
#define SFS_SEEK_CUR		1  /*Current position of file */
#define SFS_SEEK_END		2  /*End of file   */

#define  SFS_MAX_PATH 256

/**---------------------------------------------------------------------------*
 **    Overlapped struct                                                      *
 
    completed-route :
    void CALLBACK  FileIOCompletionRoutine(
        SFS_ERROR_E  error,		// error code of the relative opration.
        SFS_PARAM   irp_param,	// the param set by user.
        SFS_PARAM	param1,		// result of the relative operation.
        SFS_PARAM	param1)		// result of the relative operation.
**---------------------------------------------------------------------------*/

typedef uint32  SFS_PARAM;
typedef void (*SFS_COMPLETETION_ROUTINE)(SFS_ERROR_E error, uint32 irp_param, uint32 param1,uint32 param2);

typedef struct SFS_OVERLAPPED_tag
{
   SFS_COMPLETETION_ROUTINE   complete_route; //the point to callback function.
   SFS_PARAM                  param;//the param set by user,it canbe anything,it will be passed to callbackfun without modify 
}SFS_OVERLAPPED_T;


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
typedef enum SFS_MODE_TAG
{   
    /* 0      1       2             3            4 5 6 7      7......31
    +---------------------------------------------------------------------+
    | read | write | share_read | share_write | create/open | bit 7..31   |
    +---------------------------------------------------------------------+   */

	// Bit 0-1
	SFS_MODE_READ			= 0x0001,  
	SFS_MODE_WRITE			= 0x0002,
         
	// Bit 2-3
	SFS_MODE_SHARE_READ		= 0x0004,//reserved jason.wu
	SFS_MODE_SHARE_WRITE	= 0x0008,//reserved

	// Bit 4-7
	SFS_MODE_CREATE_NEW		= 0x0010,
	SFS_MODE_CREATE_ALWAYS	= 0x0020,
	SFS_MODE_OPEN_EXISTING	= 0x0030,
	SFS_MODE_OPEN_ALWAYS	= 0x0040,
	SFS_MODE_APPEND			= 0x0050
}SFS_MODE_E;


/*
    Find Data Define.
    The receives information about the found file.
*/
typedef struct 
{
	uint8   mday;       // day of the month - [1,31] 
	uint8   mon;        // months  - [1,12] 
	uint16  year;       // years [1980,2107] 
} SFS_DATE_T;

typedef struct 
{
	uint8   sec;        // secondsafter the minute - [0,59] 
	uint8   min;        // minutesafter the hour - [0,59] 
	uint8   hour;       // hours since midnight - [0,23] 
}SFS_TIME_T;

typedef struct SFS_FIND_DATA_TAG
{
	SFS_DATE_T	create_Date;
	SFS_TIME_T	create_time;
	SFS_DATE_T	modify_Date	;
	SFS_TIME_T	modify_time;
	SFS_DATE_T	access_date;
	uint16          attr;                // see FFS_ATTR_E.
	uint32          length;
	uint16          name[SFS_MAX_PATH];
	uint8           short_name[13];
}SFS_FIND_DATA_T;

//fat attribute bits
//the value of SFS_FIND_DATA_T->attr,you can use it to identify the file
#define SFS_ATTR_NONE    0 /* no attribute bits */
#define SFS_ATTR_RO      1  /* read-only */
#define SFS_ATTR_HIDDEN  2  /* hidden */
#define SFS_ATTR_SYS     4  /* system */
#define SFS_ATTR_VOLUME  8  /* volume label */
#define SFS_ATTR_DIR     16 /* directory */
#define SFS_ATTR_ARCH    32 /* archived */


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/**************/
//file system initialize,call it in os start once and just once.after that you must not call it in anytime.
/**************/
//void SFS_Init(void);
/**************/
//device operation
//note:	if the space of dev_name and overlapped_ptr is allocated by caller,it must 
//		be free by caller itself after api return.
/**************/
SFS_ERROR_E SFS_RegisterDevice(/*in*/const uint16 * dev_name,/*out*/SFS_DEVICE_FORMAT_E * format_ptr);
SFS_ERROR_E SFS_UnRegisterDevice(const uint16 *dev_name);
SFS_ERROR_E SFS_Format(const uint16 *dev_name,SFS_DEVICE_FORMAT_E format_type,SFS_OVERLAPPED_T *overlapped_ptr);
SFS_ERROR_E SFS_GetDeviceStatus(const uint16* dev_name);
SFS_ERROR_E SFS_GetDeviceFreeSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);
SFS_ERROR_E SFS_GetDeviceUsedSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);

/*--------------------------------------------------------*/
//file and dir operation

//note:	1	if the space of name and overlapped_ptr is allocated by caller,it must 
//			be free by caller itself after api return.
//		2	the pathname must like 	"devname:\\foldername1\\foldername2\\filename " in unicode form and nothing else,
//			or like	"devname:/foldername1/foldername2/filename " in unicode form and nothing else.
//			here "/" = "\\"
//		3	length of pathname must less than SFS_MAX_PATH
//			
/*--------------------------------------------------------*/
SFS_HANDLE SFS_CreateFile(
const uint16 *file_name, 
uint32 access_mode,
uint32 share_mode,	//Must be NULL,File System have not realize it 
uint32 file_attri		//Must be NULL,File System have not realize it 
);
SFS_ERROR_E SFS_DeleteFile(const uint16 *name,SFS_OVERLAPPED_T    *overlapped_ptr);
SFS_ERROR_E SFS_RenameFile(const uint16 *sour,const uint16 *dest,SFS_OVERLAPPED_T    *overlapped_ptr);

SFS_ERROR_E SFS_CreateDirectory(const uint16 *path);
SFS_ERROR_E SFS_DeleteDirectory(const uint16 *path);

SFS_HANDLE SFS_FindFirstFile(const uint16 *find, SFS_FIND_DATA_T *fd);
SFS_ERROR_E SFS_FindNextFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T *fd);
SFS_ERROR_E SFS_FindClose(SFS_HANDLE sfs_handle);

/*--------------------------------------------------------*/
//file internal operation
/*--------------------------------------------------------*/
SFS_ERROR_E SFS_GetFileSize(SFS_HANDLE sfs_handle, uint32*  size);
SFS_ERROR_E SFS_SetFileSize(SFS_HANDLE sfs_handle, uint32   size);

SFS_ERROR_E SFS_ReadFile(
	SFS_HANDLE handle,
	void *buffer,
	uint32 bytes_to_read,
	uint32 *bytes_read,
	SFS_OVERLAPPED_T *overlapped
);
SFS_ERROR_E SFS_WriteFile(
	SFS_HANDLE handle,
	const void *buffer,
	uint32 bytes_to_write,
	uint32 *bytes_written,
	SFS_OVERLAPPED_T *overlapped
);
SFS_ERROR_E SFS_SetFilePointer(
	SFS_HANDLE handle,
	int32 offset,
	uint32 origin
);
SFS_ERROR_E SFS_GetFilePointer(
	SFS_HANDLE handle,
	uint32 origin,
	int32* currentpos 
);
SFS_ERROR_E SFS_CloseFile(SFS_HANDLE handle);

/*--------------------------------------------------------*/
//
//TOOLs Interface
//
//note:
//	when you port third party lib into your system ,next funtion maybe useful for you . 
//	you'd better not use it in other place. 
//注意：
//	在集成一些第三方软件时，下面的接口可能会简化移植过程。
//	但在一般的情况下，最好不要使用下面接口。
//
/*--------------------------------------------------------*/

SFS_ERROR_E SFS_GetDevnameFromPath(/*in*/uint16* pathname,/*out*/uint16* dev_name);	//you can get device name from a path ,this two name must be managed by callerself:eg: if this two name is allocated by caller they must be free by caller after filesystem api return. 
SFS_ERROR_E SFS_ConvertStrmodeToBinmode(const uint8 *mode,uint32 *bin_mode);	//change 'w','r','a','w+','r+','a+' to binary,this binary can be used in SFS_CreateFile as access_mode. 



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif         /*End FFS.h*/

