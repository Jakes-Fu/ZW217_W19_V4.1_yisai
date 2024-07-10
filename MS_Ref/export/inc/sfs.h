/******************************************************************************
 ** File Name:      sfs.h                                                     *
 ** Author:                                                           *
 ** DATE:           8/16/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SFS.               *
 **                 (FAT File System)                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 8/16/2006               Create.                   Jason.wu                *
 ******************************************************************************/

#ifndef _SFS_H_
#define _SFS_H_

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

#define SFS_IN
#define SFS_OUT
#define SFS_INOUT

/*============================================================================
Define basic data types:

==============================================================================*/

typedef uint32               SFS_HANDLE;

typedef enum SFS_ERROR_E_TAG
{
/**------------------------------------------------------------------*
 **     SFS error code(0-999)                                        *
 **------------------------------------------------------------------*/
	// 0-1000 is reserved by sfs file system
    SFS_NO_ERROR					= 0,		//success
    SFS_ERROR_NONE					= 0,

    SFS_ERROR_DEVICE				= 1,		//device error

    SFS_ERROR_SYSTEM				= 2,		//os error, memory overwrite, memory error.or other unlogic error.
    SFS_ERROR_INVALID_PARAM		= 3,		//param you give is invalid
    SFS_ERROR_NO_ENOUGH_RESOURCE	= 4, 	//no use

    SFS_ERROR_IO_PENDING			= 5,		//when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING immediately.Real error message and return value will be given in callback funtion.
    SFS_ERROR_ACCESS				= 6,		//can not access the file ,maybe it is being used by other handler. or read data that not exist

    SFS_ERROR_NOT_EXIST			= 7,		// file or device not exist
    SFS_ERROR_HAS_EXIST			= 8,		// file or device has exist
    
    SFS_ERROR_HARDWARE_FAILED	= 9,		// no use
    SFS_ERROR_WRITE_CONFLICT		= 10,	// no use

    SFS_ERROR_NO_SPACE			= 11,

	SFS_ERROR_NOT_EMPTY			= 20,	// 	folder not empty.
	SFS_ERROR_LONG_DEPTH			= 21,	//	copy tree or move tree , has exceed too many folder.
	SFS_ERROR_FOLD_FULL			= 22,	// fold entry is not enough to  add new file or fold in it
	SFS_ERROR_ABORT				= 23,	//	copy or move process is cancelled.

	SFS_ERROR_NORES_HANDLE		= 24,	// no handle resource for open new file

	SFS_ERROR_BUSY					= 25,	// current sfs task is busy(formating or mountring)

	SFS_ERROR_INVALID_FORMAT		= 26,

	SFS_ERROR_TOOMANYFILE			= 27,

/**------------------------------------------------------------------*
 **     DRM error code(1000-1199)                                    *
 **------------------------------------------------------------------*/
	SFS_ERROR_DRM_ACCESS_DENIED	= 1000,	// @681, operation not allowed
	SFS_ERROR_DRM_NORMAL_FILE	= 1001,	// file is normal    
	SFS_ERROR_DRM_NO_RIGHTS		= 1002,	// no rights
	SFS_ERROR_DRM_RIGHTS_EXPIRED	= 1003,	// rights expired
	SFS_ERROR_DRM_INVALID_RIGHTS	= 1004,	// rights file is invalid    
	SFS_ERROR_DRM_INVALID_FORMAT	= 1005,	// DRM file format is invalid
	SFS_ERROR_INVALID_HANDLE		= 1006

/**------------------------------------------------------------------*
 **     reserved error code(1200-0xFFFFFFFF)                         *
 **------------------------------------------------------------------*/
// if you want to add error code here please inform it to midware group. Jason.wu




}SFS_ERROR_E;


typedef enum SFS_DEVICE_FORMAT_E_TAG
{
	SFS_FAT12_FORMAT = 0,
	SFS_FAT16_FORMAT = 1,
	SFS_FAT32_FORMAT = 2,
	SFS_AUTO_FORMAT = 6,
	SFS_OTHER_FORMAT = 7,
	SFS_UNKNOWN_FORMAT
}SFS_DEVICE_FORMAT_E;

/*
    Seek Macro define.
*/
#define SFS_SEEK_BEGIN		0  /*Beginning of file */
#define SFS_SEEK_CUR		1  /*Current position of file */
#define SFS_SEEK_END		2  /*End of file   */

#define  SFS_MAX_PATH 255

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


/**------------------------------------------------------------------*
 **     SFS operation Mode(bit0-bit4)                                *
 **     only have 6 group can be used:                               *
 **     1  SFS_MODE_READ                                             *
 **     2  SFS_MODE_WRITE                                            *
 **     3  SFS_MODE_READ|SFS_MODE_WRITE                              *
 **     4  SFS_MODE_SHARE_READ                                       *
 **     5  SFS_MODE_SHARE_WRITE                                      *
 **     6  SFS_MODE_SHARE_READ|SFS_MODE_SHARE_WRITE                  *
 **                                                                  *
 **------------------------------------------------------------------*/
#define		SFS_MODE_READ				((uint32)(0x1<<0))
#define		SFS_MODE_WRITE				((uint32)(0x1<<1))
#define		SFS_MODE_SHARE_READ		((uint32)(0x1<<2))
#define		SFS_MODE_SHARE_WRITE		((uint32)(0x1<<3))
/**------------------------------------------------------------------*
 **     SFS access Mode(bit4-bit7)                                   *
 **     only have following choice:                                  *
 **------------------------------------------------------------------*/
#define		SFS_MODE_CREATE_NEW		((uint32)(0x1<<4))
#define		SFS_MODE_CREATE_ALWAYS	((uint32)(0x2<<4))
#define		SFS_MODE_OPEN_EXISTING	((uint32)(0x3<<4))
#define		SFS_MODE_OPEN_ALWAYS		((uint32)(0x4<<4))
#define		SFS_MODE_APPEND			((uint32)(0x5<<4))
/**------------------------------------------------------------------*
 **     SFS DRM access Mode(bit8-bit12)                              *
 **     only have following choice:                                  *
 **------------------------------------------------------------------*/
#define		SFS_MODE_DRM_READ			((uint32)(0x1<<8))
#define	    SFS_MODE_DRM_PLAY			((uint32)(0x2<<8))
#define	    SFS_MODE_DRM_DISPLAY		((uint32)(0x3<<8))
#define	    SFS_MODE_DRM_EXECUTE		((uint32)(0x4<<8))
#define	    SFS_MODE_DRM_PRINT		((uint32)(0x5<<8))

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
	SFS_DATE_T	modify_Date;
	SFS_TIME_T	modify_time;
	SFS_DATE_T	access_date;
	uint16          attr;                // see FFS_ATTR_E.
	uint32          length;
	uint16          name[SFS_MAX_PATH+1];
	uint8           short_name[13];
}SFS_FIND_DATA_T;

//fat attribute bits
//the value of SFS_FIND_DATA_T->attr,you can use it to identify the file
#define SFS_ATTR_NONE    0x0 /* no attribute bits */
#define SFS_ATTR_RO      0x1  /* read-only */
#define SFS_ATTR_HIDDEN  0x2  /* hidden */
#define SFS_ATTR_SYS     0x4  /* system */
#define SFS_ATTR_VOLUME  0x8  /* volume label */	//reserved : you can not use it
#define SFS_ATTR_DIR     0x10 /* directory */			//can not used in SFS_SetAttr()
#define SFS_ATTR_ARCH    0x20 /* archived */


/**---------------------------------------------------------------------------*
//                         Function Prototypes                              
**---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
//Device operation
//note:	if the space of dev_name and overlapped_ptr is allocated by caller,it must 
//		be free by caller itself after api return.
//
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:   Initialize SFS module
//  Author: Jason.wu
//  Param
//       param: reserved
//  Return:
//       SFS_ERROR_NONE: Success
//       other: Fail
//  Note:           
/*****************************************************************************/
SFS_ERROR_E SFS_Init(void* param);

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
SFS_ERROR_E SFS_RegisterDevice(/*in*/const uint16 * dev_name,/*out*/SFS_DEVICE_FORMAT_E * format_ptr);
// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_RegisterDeviceAsyn(
	SFS_IN const uint16 * devName,
	SFS_OUT SFS_DEVICE_FORMAT_E *formatType,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

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
SFS_ERROR_E SFS_UnRegisterDevice(const uint16 *dev_name);

/*****************************************************************************/
//  Description:   Format disk
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_type:the type you want to format.But note that some disk can only be format one type
//       overlapped_ptr:asynchronism param,if you give this parm,the result will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       If success the disk will be mounted automatically 
//       If fail the disk will be unmounted automatically
/*****************************************************************************/
SFS_ERROR_E SFS_Format(const uint16 *dev_name,SFS_DEVICE_FORMAT_E format_type,SFS_OVERLAPPED_T *overlapped_ptr);

/*****************************************************************************/
//  Description:   Format disk
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//       format_type:the type you want to format.But note that some disk can only be format one type
//       option:SFS_FORMAT or SFS_FORMAT_BY_FORCE
//       overlapped_ptr:asynchronism param,if you give this parm,the result will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       If success the disk will be mounted automatically 
//       If fail the disk will be unmounted automatically
/*****************************************************************************/
#define SFS_FORMAT_PEACEFUL	0x1		// if have unclosed file in current disk ,format will not be done and API return SFS_ERROR_ACCESS
#define	SFS_FORMAT_BY_FORCE	0x2		// this option will close all the unclosed file in current disk and format disk
SFS_ERROR_E SFS_FormatByOption(const uint16 *dev_name,SFS_DEVICE_FORMAT_E format_type, uint32 option, SFS_OVERLAPPED_T *overlapped_ptr);

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
SFS_ERROR_E SFS_GetDeviceStatus(const uint16* dev_name);

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
SFS_ERROR_E SFS_GetDeviceFreeSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);

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
SFS_ERROR_E SFS_GetDeviceUsedSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);

/*****************************************************************************/
//  Description:   Get and Set disk label name
//  Author: Jason.wu
//  Param
//       device_name :The name of device ,must be unicode string
//       label:	volume name will stored in this buf
//  Return:
//       SFS_ERROR_NONE: Success the value of label is valid
//       Other: Fail ,the value of label is invalid
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetDeviceLabel(
	SFS_IN const uint16 *dev_name,
	SFS_OUT uint8* label,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);
SFS_ERROR_E SFS_SetDeviceLabel(
	SFS_IN const uint16 *dev_name,
	SFS_IN uint8* label,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);


/*--------------------------------------------------------*/
//file and dir operation

//note:	1	if the space of name and overlapped_ptr is allocated by caller,it must 
//			be free by caller itself after api return.
//		2	the pathname or filename must like 	"devname:\\foldername1\\foldername2\\filename " in unicode form and nothing else,
//		3	length of pathname must less than SFS_MAX_PATH
//			
/*--------------------------------------------------------*/

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Jason.wu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved
//       file_attri:reserved
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note: 
/*****************************************************************************/
SFS_HANDLE SFS_CreateFile(
const uint16 *file_name, 
uint32 access_mode,
uint32 share_mode,	//Must be NULL,File System have not realize it 
uint32 file_attri		//Must be NULL,File System have not realize it 
);

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Jason.wu
//  Param
//       pathName :The path that will create file,must be unicode string
//       fileName: The name of file that will be created.must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved
//       file_attri:reserved
//       handle: when file is opened or created ,this handle will used to access data of file
//       overlapped_ptr::asynchronism param,if you give this parm,the result will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_CreateFileAsyn(
	SFS_IN const uint16* pathName,
	SFS_IN const uint16* fileName,
	SFS_IN uint32 access_mode,
	SFS_IN uint32 share_mode,	//Must be NULL,File System have not realize it 
	SFS_IN uint32 file_attri,	//Must be NULL,File System have not realize it 
	SFS_OUT SFS_HANDLE* handle,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

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
SFS_ERROR_E SFS_CreateDirectory(const uint16 *path);

// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_CreateDirectoryAsyn(
	SFS_IN const uint16* pathName,
	SFS_IN const uint16* fileName,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

/*****************************************************************************/
//  Description:   Delete a file
//  Author: Jason.wu
//  Param
//       name :The name of path and file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_DeleteFile(const uint16 *name,SFS_OVERLAPPED_T    *overlapped_ptr);

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
SFS_ERROR_E SFS_DeleteDirectory(const uint16 *path);

// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_DeleteDirectoryAsyn(
	SFS_IN const uint16* pathName,
	SFS_IN const uint16* fileName,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

#if 0
/*****************************************************************************/
//  Description:   Delete a directory and his sub content
//  Author: Jason.wu
//  Param
//       pathName	:The path that will create file,must be unicode string
//       fileName	: The name of file that will be created.must be unicode string
//       ext		: extent param that maybe used in following vertion
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_DeleteTree(
	SFS_IN const uint16 *pathName,
	SFS_IN const uint16 *fileName,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr,
	SFS_INOUT void* ext			// reserved , must be zero
);

/*****************************************************************************/
//  Description:   Copy file to another place
//  Author: Jason.wu
//  Param
//       srcPath	: path of source file
//       srcName	: name of source file
//       destPath	: path of dest
//       destName	: new name of file that will be copied
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_CopyFile(
	SFS_IN const uint16 *srcPath,
	SFS_IN const uint16 *srcName,
	SFS_IN const uint16 *destPath,
	SFS_IN const uint16 *destName,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr
);

/*****************************************************************************/
//  Description:   Copy folder and its sub content to another place
//  Author: Jason.wu
//  Param
//       srcPath	: path of source folder
//       srcName	: name of source folder
//       destPath	: path of dest
//       destName	: new name of folder that will be copied
//       ext		: extent param that maybe used in following vertion
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_CopyTree(
	SFS_IN const uint16 *srcPath,
	SFS_IN const uint16 *srcName,
	SFS_IN const uint16 *destPath,
	SFS_IN const uint16 *destName,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr,
	SFS_INOUT void* ext			// reserved , must be zero
);

/*****************************************************************************/
//  Description:   Move file to another place
//  Author: Jason.wu
//  Param
//       srcPath	: path of source file
//       srcName	: name of source file
//       destPath	: path of dest
//       destName	: new name of file that will be moved
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_MoveFile(
	SFS_IN const uint16 *srcPath,
	SFS_IN const uint16 *srcName,
	SFS_IN const uint16 *destPath,
	SFS_IN const uint16 *destName,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr
);

/*****************************************************************************/
//  Description:   Move folder and its sub content to another place
//  Author: Jason.wu
//  Param
//       srcPath	: path of source folder
//       srcName	: name of source folder
//       destPath	: path of dest
//       destName	: new name of folder that will be moved
//       ext		: extent param that maybe used in following vertion
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_MoveTree(
	SFS_IN const uint16 *srcPath,
	SFS_IN const uint16 *srcName,
	SFS_IN const uint16 *destPath,
	SFS_IN const uint16 *destName,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr,
	SFS_INOUT void* ext			// reserved , must be zero
);

/*****************************************************************************/
//  Description:   Copy and Move function maybe occupy much time .This function can stop Copy or Move process
//  Author: Jason.wu
//  Param

//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//	Added in SFS2.0 Jason.wu
/*****************************************************************************/
SFS_ERROR_E SFS_Abort(SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr);
#endif
/*****************************************************************************/
//  Description:   Rename a file
//  Author: Jason.wu
//  Param
//       sour:The name of path and source file ,must be unicode string
//       dest:The name of path and dest file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
//       the path of sourcename and destname must be same
/*****************************************************************************/
SFS_ERROR_E SFS_RenameFile(const uint16 *sour,const uint16 *dest,SFS_OVERLAPPED_T    *overlapped_ptr);

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
SFS_ERROR_E SFS_SetAttr(const uint16 *name, uint16 attr);
// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_SetAttrAsyn(
	SFS_IN const uint16* pName,
	SFS_IN const uint16* fName,
	SFS_IN uint16* ctlStr,			// control string, just use /SA command
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

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
SFS_ERROR_E SFS_GetAttr(const uint16 *name, uint16* attr);
// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_GetAttrAsyn(
	SFS_IN const uint16* pName,
	SFS_IN const uint16* fName,
	SFS_OUT SFS_FIND_DATA_T *fd,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

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
SFS_HANDLE SFS_FindFirstFile(const uint16 *find, SFS_FIND_DATA_T *fd);
// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_FindFirstFileCtrl(
	SFS_IN const uint16* pathName,
	SFS_IN const uint16* fileName,	// such as 123.mp3|*.mp3|*.mp4|*.mp?|1?3.mp3
	SFS_IN uint16* ctlStr,			// control string
	SFS_OUT SFS_HANDLE* handle,
	SFS_OUT SFS_FIND_DATA_T *fd,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

/*****************************************************************************/
//  Description:  Find a next or previous file that match the param 'find' in SFS_FindFirstFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       SFS_ERROR_NONE: the next or previous file is founded
//       Other: not found
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_FindPreFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T *fd);
SFS_ERROR_E SFS_FindNextFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T *fd);

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
SFS_ERROR_E SFS_FindClose(SFS_HANDLE sfs_handle);

/*--------------------------------------------------------*/
//file internal operation
/*--------------------------------------------------------*/

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
SFS_ERROR_E SFS_GetFileSize(SFS_HANDLE sfs_handle, uint32*  size);

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
SFS_ERROR_E SFS_SetFileSize(SFS_HANDLE sfs_handle, uint32   size);

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
SFS_ERROR_E SFS_ReadFile(
	SFS_HANDLE handle,
	void *buffer,
	uint32 bytes_to_read,
	uint32 *bytes_read,
	SFS_OVERLAPPED_T *overlapped
);

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
SFS_ERROR_E SFS_WriteFile(
	SFS_HANDLE handle,
	const void *buffer,
	uint32 bytes_to_write,
	uint32 *bytes_written,
	SFS_OVERLAPPED_T *overlapped
);

/*****************************************************************************/
//  Description:  Flush file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_FlushFile(
	SFS_HANDLE handle,
	SFS_OVERLAPPED_T *overlapped
);

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
SFS_ERROR_E SFS_SetFilePointer(
	SFS_HANDLE handle,
	__int64 offset,
	uint32 origin
);

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
SFS_ERROR_E SFS_GetFilePointer(
	SFS_HANDLE handle,
	uint32 origin,
	int32* currentpos 
);

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
SFS_ERROR_E SFS_CloseFile(SFS_HANDLE handle);

/*****************************************************************************/
//  Description:  X function use this call back function to return the progress of function, and APP
//			also can use the return value of this function to abort the X function's progress
//  Author: Jason.wu
//  Param
//       inParam: APP can set this param by routingParam of X function
//       status: the progress of X function, It's meaning is different of X function
//       outParam1: 
//       outParam2: 
//  Return:
//       None zero: the file founded ,this find handle can used to find next file match the param 'find'
//       zero: not found
//  Note: 
/*****************************************************************************/
#define SFS_X_CONTINUE 0
#define SFS_X_STOP		1
typedef int (*SFS_XFUNC_ROUTINE)(uint32 inParam, uint32 status,uint32 outParam1, uint32 outParam2);

/*****************************************************************************/
//  Description:  Find all file and folder that match the param 'tName' and ctlStr
//                And sort it as sortType.
//  Author: Jason.wu
//  Param
//       pName:path name ,that search all the file and folder in it.
//       tName:The file name you want to find,such as "*.mp3","*.3gp",or "123.mp3"...
//  Return:
//       None zero: the file founded ,this find handle can used to find next file match the param 'find'
//       zero: not found
//  Note: 
/*****************************************************************************/

//xfind status
#define SFS_XFIND_PREPARE					1
#define SFS_XFIND_PREPARE_END				2		// outParam1: foldCnt, outParam2: fileCnt
//---
#define SFS_XFIND_FOLD_PREPARE			3		// outParam1: foldCnt, outParam2: 0
#define SFS_XFIND_FOLD_SORTING_START		4		// outParam1: foldCnt, outParam2: 0
#define SFS_XFIND_FOLD_SORTING			5		// outParam1: foldCnt, outParam2: sorting index(sorting progress)
#define SFS_XFIND_FOLD_SORTING_END		6		// outParam1: foldCnt, outParam2: 0
//---
#define SFS_XFIND_FILE_PREPARE				7		// outParam1: fileCnt, outParam2: 0
#define SFS_XFIND_FILE_SORTING_START		8		// outParam1: fileCnt, outParam2: 0
#define SFS_XFIND_FILE_SORTING				9		// outParam1: fileCnt, outParam2: sorting index(sorting progress)
#define SFS_XFIND_FILE_SORTING_END		10		// outParam1: fileCnt, outParam2: 0
//---
#define SFS_XFIND_SORTING_END				11		// outParam1: foldCnt, outParam2: fileCnt
//---
//	This call back will get a key word from fileinfo, during the sorting , 
//	xfind first compare the key word, if key word is equity ,then xfind will use compare funciton to compare the fd1 and fd2
typedef uint32 (*SFS_XFIND_GETKEY)(SFS_FIND_DATA_T* fd);
//---
//	This is a compare function ,if key word is same ,xFind will call this function to compare the fd1 and fd2.
//	If (fd1 > fd2) return 1
//	If (fd1 == fd2) return 0
//	If (fd1 < fd2) return -1
typedef int32 (*SFS_XFIND_COMPARE)(SFS_FIND_DATA_T* fd1, SFS_FIND_DATA_T* fd2);

PUBLIC SFS_ERROR_E SFS_XFindOpen(
	SFS_IN uint16* pName,
	SFS_IN uint16* tName,
	SFS_IN uint16* ctlStr,
	SFS_IN SFS_XFIND_GETKEY getKey,			// if ctlStr is L"/SB:CUS" , this param will be used
	SFS_IN SFS_XFIND_COMPARE compareFunc,	// if ctlStr is L"/SB:CUS" , this param will be used
	SFS_IN SFS_XFUNC_ROUTINE routing,
	SFS_IN uint32	routingParam,
	SFS_OUT SFS_HANDLE* handle,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
);

/*****************************************************************************/
//  Description:  Get the file info that is located by param 'index'.
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_XFindOpen
//       index:the index of file or folder
//       fd: If file is founded ,the infomation of file is stored in this structure 
//  Return:
//       SFS_ERROR_NONE: the next or previous file is founded
//       Other: not found
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E SFS_XFindGet(
	SFS_IN SFS_HANDLE sfs_handle,
	SFS_IN uint32 index,
	SFS_OUT SFS_FIND_DATA_T *fd
);

/*****************************************************************************/
//  Description:  Close the find handle returned by SFS_XFindOpen
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_XFindOpen
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E SFS_XFindClose(SFS_HANDLE sfs_handle);


/*****************************************************************************/
//  Description:  move file in the same partition
//  Author: Cheng.Fang
//  Param
//       srcPath:source path
//       srcName:source file name
//       destPath: destination path
//       destName: destination file name
//       routing: fill 0,now reserved
//       routingParam: fill 0,now reserved
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: the next or previous file is founded
//       Other: not found
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E SFS_XMoveFile(
	SFS_IN  const uint16 *srcPath,
	SFS_IN  const uint16 *srcName,
	SFS_IN  const uint16 *destPath,
	SFS_IN  const uint16 *destName,
	SFS_IN SFS_XFUNC_ROUTINE routing,
	SFS_IN uint32	routingParam,
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr
);

/*****************************************************************************/
//  Description:  move folder in the same partition
//  Author: Cheng.Fang
//  Param
//       srcPath:source path
//       srcName:source folder name
//       destPath: destination path
//       destName: destination folder name
//       routing: fill 0,now reserved
//       routingParam: fill 0,now reserved
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: the next or previous file is founded
//       Other: not found
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E SFS_XMoveTree(
	SFS_IN  const uint16 *srcPath,
	SFS_IN  const uint16 *srcName,
	SFS_IN  const uint16 *destPath,
	SFS_IN  const uint16 *destName,
	SFS_IN SFS_XFUNC_ROUTINE routing,		// reserved.
	SFS_IN uint32	routingParam,				// reserved.
	SFS_IN SFS_OVERLAPPED_T    *overlapped_ptr
);

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

/*--------------------------------------------------------*/
//
//	CONFIG: SFS buffer config
//
/*--------------------------------------------------------*/
typedef struct
{
	//	name of disk
	char *devName;
	//	priority of this task.
	uint32	priority;
	//	1:	a group buf per file
	//	2:	a group buf per disk
	uint32	option;

	//	if(1 == option): buffer count and buffer size of a file that has been opened
	//	if(2 == option): buffer count and buffer size of a disk, that has been mounted
	uint32	fileBufCnt;
	uint32	fileBufSize;

	//	if(1 == option): buffer count and buffer size of a folder that has been opened,used in search operation.
	//	if(2 == option): ignored
	uint32	foldBufCnt;
	uint32	foldBufSize;

	//	if(1 == option): app buffer size is multiple of foldBufSize.
	//	if(2 == option): app buffer size is multiple of fileBufSize.
	uint32	tmpFoldAppBufSize;	//	
	uint32	foldAppBufSize;		//	findfirst will open a fold as a handle , this is the fold entry buffer size of this handle
}SFS_DEV_CONFIG_T;

typedef struct
{
	uint32	priority;
	uint32	sortNum;
	uint32	folderContextNum;
}SFS_XFIND_CONFIG_T;

typedef struct
{
	uint32	priority;
	uint32	version;
}SFS_XMOV_CONFIG_T;

typedef struct
{
	uint32		fileNum;
	SFS_XFIND_CONFIG_T*	appXFindCfg;
	SFS_XMOV_CONFIG_T*	appXMovCfg;
	uint32		diskNum;
	SFS_DEV_CONFIG_T* diskCfg;
}
SFS_CONFIG_T;

SFS_CONFIG_T* SFS_GetConfig(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif         /*End FFS.h*/

