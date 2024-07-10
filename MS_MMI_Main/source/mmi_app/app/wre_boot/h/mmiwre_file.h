#ifdef WRE_SUPPORT
#ifndef _MMIWRE_FILE_H_
#define _MMIWRE_FILE_H_


typedef enum ADAPT_SFS_ERROR_E_TAG
{
/**------------------------------------------------------------------*
 **     SFS error code(0-999)                                        *
 **------------------------------------------------------------------*/
	// 0-1000 is reserved by sfs file system
    ADAPT_SFS_NO_ERROR					= 0,		//success
    ADAPT_SFS_ERROR_NONE					= 0,

    ADAPT_SFS_ERROR_DEVICE				= 1,		//device error

    ADAPT_SFS_ERROR_SYSTEM				= 2,		//os error, memory overwrite, memory error.or other unlogic error.
    ADAPT_SFS_ERROR_INVALID_PARAM		= 3,		//param you give is invalid
    ADAPT_SFS_ERROR_NO_ENOUGH_RESOURCE	= 4, 	//no use

    ADAPT_SFS_ERROR_IO_PENDING			= 5,		//when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING immediately.Real error message and return value will be given in callback funtion.
    ADAPT_SFS_ERROR_ACCESS				= 6,		//can not access the file ,maybe it is being used by other handler. or read data that not exist

    ADAPT_SFS_ERROR_NOT_EXIST			= 7,		// file or device not exist
    ADAPT_SFS_ERROR_HAS_EXIST			= 8,		// file or device has exist
    
    ADAPT_SFS_ERROR_HARDWARE_FAILED	= 9,		// no use
    ADAPT_SFS_ERROR_WRITE_CONFLICT		= 10,	// no use

    ADAPT_SFS_ERROR_NO_SPACE			= 11,

	ADAPT_SFS_ERROR_NOT_EMPTY			= 20,	// 	folder not empty.
	ADAPT_SFS_ERROR_LONG_DEPTH			= 21,	//	copy tree or move tree , has exceed too many folder.
	ADAPT_SFS_ERROR_FOLD_FULL			= 22,	// fold entry is not enough to  add new file or fold in it
	ADAPT_SFS_ERROR_ABORT				= 23,	//	copy or move process is cancelled.

	ADAPT_SFS_ERROR_NORES_HANDLE		= 24,		//no handle resource for open new file


/**------------------------------------------------------------------*
 **     DRM error code(1000-1199)                                    *
 **------------------------------------------------------------------*/
	ADAPT_SFS_ERROR_DRM_ACCESS_DENIED	= 1000,	// @681, operation not allowed
	ADAPT_SFS_ERROR_DRM_NORMAL_FILE	= 1001,	// file is normal    
	ADAPT_SFS_ERROR_DRM_NO_RIGHTS		= 1002,	// no rights
	ADAPT_SFS_ERROR_DRM_RIGHTS_EXPIRED	= 1003,	// rights expired
	ADAPT_SFS_ERROR_DRM_INVALID_RIGHTS	= 1004,	// rights file is invalid    
	ADAPT_SFS_ERROR_DRM_INVALID_FORMAT	= 1005,	// DRM file format is invalid
	ADAPT_SFS_ERROR_INVALID_HANDLE		= 1006

/**------------------------------------------------------------------*
 **     reserved error code(1200-0xFFFFFFFF)                         *
 **------------------------------------------------------------------*/
// if you want to add error code here please inform it to midware group. Jason.wu




}ADAPT_SFS_ERROR_E;

typedef uint32 ADAPT_FILE_HANDLE;
typedef uint32 ADAPT_SFS_HANDLE;

//保持跟MOCOR一致
#define WRE_FILE_SEEK_BEGIN      0
#define WRE_FILE_SEEK_CUR        1
#define WRE_FILE_SEEK_END        2

#define ADAPT_SFS_SEEK_BEGIN     0
#define ADAPT_SFS_SEEK_CUR       1
#define ADAPT_SFS_SEEK_END       2

typedef enum ADAPT_SFS_MODE_TAG
{   

/**------------------------------------------------------------------*
 **     SFS Mode(bit0-bit15)                                         *
 **------------------------------------------------------------------*/
   /**---------------------------------------------------------------------------*
    **    Open/Create  flag                                                      *
    **                                                                           *
    **      Mode                            Description                          *
    **---------------------------------------------------------------------------*
      // operation mode:
      ----------------------------------------------------------------------------
            SFS_MODE_READ                   Specifies read access to the object. 
                                            Data can be read from the file and
                                            the file pointer can be moved.
      ----------------------------------------------------------------------------                                                
            SFS_MODE_WRITE                  Specifies write access to the object.
                                            Data can be written to the file and 
                                            the file pointer can be moved
      ----------------------------------------------------------------------------
            SFS_MODE_SHARE_READ             Subsequent open operations on the
                                            object will succeed only if read 
                                            access is requested.
      ----------------------------------------------------------------------------
            SFS_MODE_SHARE_WRITE            Subsequent open operations on the
                                            object will succeed only if write
                                            access is requested
      ----------------------------------------------------------------------------
      // access mode:only one access mode can be used every time
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
    /* 0      1       2             3            4 5 6 7      7......31
    +---------------------------------------------------------------------+
    | read | write | share_read | share_write | create/open | bit 7..31   |
    +---------------------------------------------------------------------+   */
	// Bit 0-1
	ADAPT_SFS_MODE_READ				= 0x0001,  
	ADAPT_SFS_MODE_WRITE				= 0x0002,
         
	// Bit 2-3
	ADAPT_SFS_MODE_SHARE_READ		= 0x0004,
	ADAPT_SFS_MODE_SHARE_WRITE		= 0x0008,

	// Bit 4-7
	ADAPT_SFS_MODE_CREATE_NEW		= 0x0010,
	ADAPT_SFS_MODE_CREATE_ALWAYS	= 0x0020,
	ADAPT_SFS_MODE_OPEN_EXISTING	= 0x0030,
	ADAPT_SFS_MODE_OPEN_ALWAYS		= 0x0040,
	ADAPT_SFS_MODE_APPEND			= 0x0050,

/**------------------------------------------------------------------*
 **     DRM Mode(bit16-bit19)                                        *
 **------------------------------------------------------------------*/
	ADAPT_SFS_MODE_DRM_READ			= 0x0100,   // @681
    ADAPT_SFS_MODE_DRM_PLAY			= 0x0200,
    ADAPT_SFS_MODE_DRM_DISPLAY		= 0x0300,
    ADAPT_SFS_MODE_DRM_EXECUTE		= 0x0400,
    ADAPT_SFS_MODE_DRM_PRINT		= 0x0500

/**------------------------------------------------------------------*
 **     reserved Mode(bit20-bit31)                                   *
 **------------------------------------------------------------------*/
// if you want to add mode type here please inform it to midware group. Jason.wu



}ADAPT_SFS_MODE_E;

#define  ADAPT_SFS_MAX_PATH 255


//fat attribute bits
//the value of SFS_FIND_DATA_T->attr,you can use it to identify the file
#define ADAPT_SFS_ATTR_NONE    0x0 /* no attribute bits */
#define ADAPT_SFS_ATTR_RO      0x1  /* read-only */
#define ADAPT_SFS_ATTR_HIDDEN  0x2  /* hidden */
#define ADAPT_SFS_ATTR_SYS     0x4  /* system */
#define ADAPT_SFS_ATTR_VOLUME  0x8  /* volume label */	//reserved : you can not use it
#define ADAPT_SFS_ATTR_DIR     0x10 /* directory */			//can not used in SFS_SetAttr()
#define ADAPT_SFS_ATTR_ARCH    0x20 /* archived */

typedef uint32  ADAPT_SFS_PARAM;
typedef void (*ADAPT_SFS_COMPLETETION_ROUTINE)(ADAPT_SFS_ERROR_E error, uint32 irp_param, uint32 param1,uint32 param2);

typedef struct ADAPT_SFS_OVERLAPPED_tag
{
   ADAPT_SFS_COMPLETETION_ROUTINE   complete_route; //the point to callback function.
   ADAPT_SFS_PARAM                  param;//the param set by user,it canbe anything,it will be passed to callbackfun without modify 
}ADAPT_SFS_OVERLAPPED_T;

typedef struct 
{
	uint8   mday;       // day of the month - [1,31] 
	uint8   mon;        // months  - [1,12] 
	uint16  year;       // years [1980,2107] 
} ADAPT_SFS_DATE_T;

typedef struct 
{
	uint8   sec;        // secondsafter the minute - [0,59] 
	uint8   min;        // minutesafter the hour - [0,59] 
	uint8   hour;       // hours since midnight - [0,23] 
}ADAPT_SFS_TIME_T;

typedef struct 
{
    uint8   mday;       // day of the month - [1,31] 
    uint8   mon;        // months  - [1,12] 
    uint16  year;       // years [2000,2049] 
    uint8   wday;       // days since Sunday - [0,6], (Sunday : 0, Monday : 1)
}ADAPT_DATE_T;

typedef struct 
{
    uint8   sec;        // secondsafter the minute - [0,59] 
    uint8   min;        // minutesafter the hour - [0,59] 
    uint8   hour;       // hours since midnight - [0,23] 
}ADAPT_TIME_T;

typedef struct ADAPT_SFS_FIND_DATA_TAG
{
	ADAPT_SFS_DATE_T	create_Date;
	ADAPT_SFS_TIME_T	create_time;
	ADAPT_SFS_DATE_T	modify_Date	;
	ADAPT_SFS_TIME_T	modify_time;
	ADAPT_SFS_DATE_T	access_date;
	uint16          attr;                // see FFS_ATTR_E.
	uint32          length;
	uint16          name[ADAPT_SFS_MAX_PATH+1];
	uint8           short_name[13];
}ADAPT_SFS_FIND_DATA_T;

//================================ function declaration ===========
ADAPT_SFS_HANDLE ADAPT_SFS_CreateFile(
						  const uint16 *file_name, 
						  uint32 access_mode,
						  uint32 share_mode,	//Must be NULL,File System have not realize it 
						  uint32 file_attri		//Must be NULL,File System have not realize it 
						  );

ADAPT_SFS_ERROR_E ADAPT_SFS_CloseFile(ADAPT_SFS_HANDLE handle);

ADAPT_SFS_ERROR_E ADAPT_SFS_ReadFile(
	ADAPT_SFS_HANDLE handle,
	void *buffer,
	uint32 bytes_to_read,
	uint32 *bytes_read,
	void *overlapped );

ADAPT_SFS_ERROR_E ADAPT_SFS_WriteFile(
	ADAPT_SFS_HANDLE handle,
	const void *buffer,
	uint32 bytes_to_write,
	uint32 *bytes_written,
	void *overlapped );

ADAPT_SFS_ERROR_E ADAPT_SFS_CreateDirectory(const uint16 *path);

ADAPT_SFS_ERROR_E ADAPT_SFS_DeleteFile(const uint16 *name,void *overlapped_ptr);

ADAPT_SFS_ERROR_E ADAPT_SFS_FindClose(ADAPT_SFS_HANDLE sfs_handle);

ADAPT_SFS_HANDLE ADAPT_SFS_FindFirstFile(const uint16 *find, ADAPT_SFS_FIND_DATA_T *fd);

ADAPT_SFS_ERROR_E ADAPT_SFS_FindNextFile(ADAPT_SFS_HANDLE sfs_handle, ADAPT_SFS_FIND_DATA_T *fd);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceFreeSpace(
	const uint16 *device_name,
	uint32 *high_word,
	uint32 *low_word
);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceUsedSpace(
	const uint16 *pathName,
	uint32 *high_word,
	uint32 *low_word
);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetAttr(const uint16 *name, uint16* attr);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetFileSize(ADAPT_SFS_HANDLE sfs_handle, uint32*  size);

ADAPT_SFS_ERROR_E ADAPT_SFS_DeleteDirectory(const uint16 *path);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetFilePointer(
	ADAPT_SFS_HANDLE handle,
	uint32 origin,
	int* currentpos );

ADAPT_SFS_ERROR_E ADAPT_SFS_SetFileSize(ADAPT_SFS_HANDLE sfs_handle, uint32   size);

ADAPT_SFS_ERROR_E ADAPT_SFS_SetAttr(const uint16 *name, uint16 attr);

ADAPT_SFS_ERROR_E ADAPT_SFS_SetFilePointer(
	ADAPT_SFS_HANDLE handle,
	int offset,
	uint32 origin );

ADAPT_SFS_ERROR_E ADAPT_SFS_RenameFile(const uint16 *sour,const uint16 *dest,void    *overlapped_ptr);

ADAPT_SFS_ERROR_E ADAPT_SFS_FlushFile(ADAPT_SFS_HANDLE handle,void *overlapped);

ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceStatus(const uint16* dev_name);

//获得系统磁盘
uint16 WRE_GetSysDisk(void);

//获得运行盘
uint16 WRE_GetSDDisk(void);

#endif

#endif
