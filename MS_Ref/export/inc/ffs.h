/******************************************************************************
 ** File Name:      ffs.h                                                     *
 ** Author:                                                           *
 ** DATE:           12/16/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of FFS.               *
 **                 (both FATand EFS File System)                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/16/2004               Create.                                   *
 ******************************************************************************/

#ifndef _FFS_H
#define _FFS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "ffs_error.h"
#include "efs.h"
#include "sfs.h"

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

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef wchar
typedef unsigned short		wchar;
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif




typedef uint32               HFS;

typedef uint32               SCI_Dir;

#define FFS_INVALID_HANDLE   ((HFS)0)


#define WORD_SWAP(x)	((((x)<<8)&0xff00)|(((x)>>8)&0x00ff))
#define DWORD_SWAP(x)	((((x)<<24)&0xff000000L)|(((x)<<8)&0x00ff0000L)|(((x)>>8)&0x0000ff00L)|(((x)>>24)&0x000000ffL))


/**---------------------------------------------------------------------------*
 **    Overlapped struct                                                      *
 
    completed-route :
    void CALLBACK  FileIOCompletionRoutine(
        FFS_ERROR_E  error,    // error code of the relative opration.
        FFS_RESULT   result,   // result of the relative opration.
        FFS_PARAM    param);   // the param set by user.
**---------------------------------------------------------------------------*/

typedef uint32  FFS_PARAM;
typedef uint32  FFS_RESULT;

typedef void (*FFS_COMPLETETION_ROUTINE)(FFS_ERROR_E error, FFS_RESULT result, FFS_PARAM param);

typedef struct FFS_OVERLAPPED_tag
{
   uint32                     offset;  /* Specifies a file position at which to 
                                          start the transfer. The file position
                                          is offset from the start of the file  */

   FFS_COMPLETETION_ROUTINE   complete_route; // Complete routine.
                                              // This will be called after request completed.
   FFS_PARAM                  param;          // user will set this param(it will be as the 
                                              // input param of completetion routine.
}FFS_OVERLAPPED_T;



//文件设备类型定义：
//typedef enum
//{
// FS_HS,
// FS_UDISK,
// FS_MMC,
// FS_MMC1,
// FS_INVALID 
//} FILE_DEV_E_T;

//文件设备信息定义：
typedef struct FILE_DEV_INFO_TAG
{
   DWORD  cap;     //容量；
   DWORD  use;    //已用;
   DWORD  free;    //空闲；
   BOOLEAN unknowFormat; //未知格式；
   DWORD bad;    //损坏；
} FILE_DEV_INFO_T;


#define FFS_MAX_HANDLE  16
#define SD_MAX_HANDLE   16

#define  LARGE_AMOUNT    8*1024//over LARGE_AMOUNT data trans, should use asyn mode 
#if 0
/**---------------------------------------------------------------------------*
 **    Open/Create  flag                                                      *
 **                                                                           *
 **      Mode                            Description                          *
 **---------------------------------------------------------------------------*
         FFS_MODE_READ                   Specifies read access to the object. 
                                         Data can be read from the file and
                                         the file pointer can be moved.
   ----------------------------------------------------------------------------                                                
         FFS_MODE_WRITE                  Specifies write access to the object.
                                         Data can be written to the file and 
                                         the file pointer can be moved
   ----------------------------------------------------------------------------
         FFS_MODE_SHARE_READ             Subsequent open operations on the
                                         object will succeed only if read 
                                         access is requested.
   ----------------------------------------------------------------------------
         FFS_MODE_SHARE_WRITE            Subsequent open operations on the
                                         object will succeed only if write
                                         access is requested
   ----------------------------------------------------------------------------
         FFS_MODE_CREATE_NEW             Creates a new file. 
                                         The function fails if the specified 
                                         file already exists.
   ----------------------------------------------------------------------------      
         FFS_MODE_CREATE_ALWAYS          Creates a new file.
                                         If the file exists, the function 
                                         overwrites the file.
   ----------------------------------------------------------------------------
         FFS_MODE_OPEN_EXISTING          Opens the file.
                                         The function fails if the file 
                                         does not exist.
   ----------------------------------------------------------------------------
         FFS_MODE_OPEN_ALWAYS            Opens the file, if it exists.
                                         If the file does not exist, the function
                                         creates a new file.
   ----------------------------------------------------------------------------
         FFS_MODE_APPEND                 Open/create the file, all write operations 
                                         occur at the end of the file even if set/seek
                                         the file position.
   
 **---------------------------------------------------------------------------*/
typedef enum FFS_MODE_TAG
{   
    /* 0      1       2             3            4 5 6 7      7......31
    +---------------------------------------------------------------------+
    | read | write | share_read | share_write | create/open | bit 7..31   |
    +---------------------------------------------------------------------+   */

    // Bit 0-1
    FFS_MODE_READ            = 0x0001,  
    FFS_MODE_WRITE           = 0x0002,
         
    // Bit 2-3
    FFS_MODE_SHARE_READ      = 0x0004,
    FFS_MODE_SHARE_WRITE     = 0x0008,

    // Bit 4-7
    FFS_MODE_CREATE_NEW      = 0x0010,
    FFS_MODE_CREATE_ALWAYS   = 0x0020,
    FFS_MODE_OPEN_EXISTING   = 0x0030,
    FFS_MODE_OPEN_ALWAYS     = 0x0040,
    FFS_MODE_APPEND          = 0x0050
}FFS_MODE_E;

#define FFS_MODE_ACCESS_MASK     0x000F
#define FFS_MODE_CREATE_MASK     0x00F0


#define FFS_DIR_ENTRY_MASK 0x010

//fat attribute bits
#define ATTR_NONE    0 /* no attribute bits */
#define ATTR_RO      1  /* read-only */
#define ATTR_HIDDEN  2  /* hidden */
#define ATTR_SYS     4  /* system */
#define ATTR_VOLUME  8  /* volume label */
#define ATTR_DIR     16 /* directory */
#define ATTR_ARCH    32 /* archived */

/*
    File Attribute Define.    
*/
typedef enum
{
    
    FFS_ATTR_READONLY = 0x1,
    FFS_ATTR_READWRITE = 0x2
    
}FFS_ATTR_E;




/*
    Find Data Define.
    The receives information about the found file.
*/


#define  FFS_MAX_PATH 256
typedef struct FFS_FIND_DATA_TAG
{
    uint32          create_time;
    uint16          attr;                // see FFS_ATTR_E.
    wchar          name[FFS_MAX_PATH];
    uint32          length;
    char            short_name[13];
}FFS_FIND_DATA_T;


/*
    Seek Macro define.
*/
#define FFS_SEEK_BEGIN        0  /*Current position of file pointer */
#define FFS_SEEK_CUR        1  /*End of file   */
#define FFS_SEEK_END      2  /*Beginning of file */



/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
 
/* Init the FFS Module*/
FFS_ERROR_E FFS_Init(FILE_DEV_E_T file_dev_type);   

/*start ffs */
int FFS_Start(
    FILE_DEV_E_T file_dev_type,
    FFS_OVERLAPPED_T    *overlapped_ptr);
    
/*shutdown ffs    */
int FFS_Shutdown(
    FILE_DEV_E_T file_dev_type,
    FFS_OVERLAPPED_T *overlapped_ptr);

int FFS_Status(FILE_DEV_E_T file_dev_type);    
 
/*
    close file handle
*/
void FFS_Close(
    FILE_DEV_E_T file_dev_type,
    HFS  handle);

/*
    Get the length of a file.
*/
uint32 FFS_GetLength(
    FILE_DEV_E_T file_dev_type,
    HFS handle);

/*
    Create or open a file.    
*/
HFS FFS_Create(
    FILE_DEV_E_T file_dev_type,
    const wchar *name,
    uint16  mode); // see FFS_MODE_E.

/*
    Move file pointer.
*/
FFS_ERROR_E FFS_Seek(
    FILE_DEV_E_T file_dev_type,
    HFS    handle,
    __int64     offset,
    uint8   origin);

/*
    Read data from the file.
    If overlapped_ptr is NULL, this function wouldn't return until read all data.
    If *NOT* NULL, you must set complete_route, and the callback will be called
    after the operation completed.
    the result field of the callback indicate the count that has read.
    and you MUST not release the buf before the callback be called.
*/
FFS_ERROR_E FFS_Read(
    FILE_DEV_E_T file_dev_type,
    HFS                handle, // the handle return by FFS_Create.
    uint8               *buf,
    uint32              len,
    uint32              *transmitted,
    FFS_OVERLAPPED_T    *overlapped_ptr);

/*
    Write data to the file.
    If overlapped_ptr is NULL, this function wouldn't return until read all data.
    If *NOT* NULL, you must set complete_route, and the callback will be called
    after the operation completed.
    the result field of the callback indicate the count that has been written.
    and you MUST not release the buf before the callback be called.
*/
FFS_ERROR_E FFS_Write(
    FILE_DEV_E_T file_dev_type,
    HFS                handle,
    uint8               *buf,    // the handle return by FFS_Create.
    uint32              len,
    uint32              *transmitted,
    FFS_OVERLAPPED_T    *overlapped_ptr);


/*
    Delete a file.
    If the file being opened, delete operation will fail.
*/
FFS_ERROR_E FFS_Delete(
    FILE_DEV_E_T file_dev_type,
    const wchar *name);
FFS_ERROR_E FFS_AYS_Delete(
    FILE_DEV_E_T file_dev_type,
    const wchar *name,
    FFS_OVERLAPPED_T    *overlapped_ptr);

/*
    Rename a file.
*/
FFS_ERROR_E FFS_Rename(
    FILE_DEV_E_T file_dev_type,
    const wchar *sour,
    const wchar *dest);

/*
    Read/Write physical device directly.
    This function only be called by system task, I will move it to other header file later.
*/
FFS_ERROR_E FFS_Direct(
    FILE_DEV_E_T file_dev_type,
    BOOLEAN    transmit_direction,   /* if true, read. otherwize write  */
    uint8      *buf,
    uint32     addr,                 /* Physical device.                */
    uint32     len);

/*
    Continue search the file that match the given string.
    the string should be same as FFS_FindFirst.
*/
FFS_ERROR_E FFS_FindNext(
    FILE_DEV_E_T file_dev_type,
    HFS find_handle,      // the handle that return by FFS_FindFirst.
    const wchar *find,      // same as FFS_FindFirst.
    FFS_FIND_DATA_T * fd);  // search result

/*
    Create a dir
*/
FFS_ERROR_E FFS_CreateDir(
    FILE_DEV_E_T file_dev_type,
    const wchar *path);

/*
    Delete a dir
*/
FFS_ERROR_E FFS_DeleteDir(
    FILE_DEV_E_T file_dev_type,
    const wchar *path);
/*change current dir*/
FFS_ERROR_E  FFS_CD(
    FILE_DEV_E_T file_dev_type,
    const wchar *path);
/*list one item met the find condition under current dir*/    
FFS_ERROR_E FFS_Dir(
    FILE_DEV_E_T file_dev_type,
    SCI_Dir *HFSD,
    const wchar *find,
    FFS_FIND_DATA_T *fd);


    
 
/*
    Get Free Flash Space
*/
uint32 FFS_GetFreeSpace(
    FILE_DEV_E_T file_dev_type);

/*
    Get Used Flash Space.
*/
uint32 FFS_GetUsedSpace(
    FILE_DEV_E_T file_dev_type);

/*
    Is the file exist ?
*/
BOOLEAN FFS_CheckExist(
    FILE_DEV_E_T file_dev_type,
    const wchar *file);
 
/*
    Try find the first file/directory that math the given string.
    and you can get the file infomation from fd.
*/
HFS FFS_FindFirst(
    FILE_DEV_E_T file_dev_type,
    const wchar *find,
    FFS_FIND_DATA_T *fd);  // Search result



/*
    Change the file size.
    Input:
        stream : the file handler that returned by FFS_Create(); 
        size   : the file size want to be changed.
    Return:
        the file size after changed, maybe less than the size if no enough free sapce.
*/
uint32 FFS_ChangeSize(
    FILE_DEV_E_T file_dev_type,
    HFS stream, uint32  size );

/*
    Get the files/dir number in the file system.
    Input:
        none
    Return:
        the numbers of files/dir in the file system.
*/
//uint32  FFS_GetFileCount(
//    FILE_DEV_E_T file_dev_type);




/**---------------------------------------------------------------------------*
 **  ANSI File Interface
 **
 **---------------------------------------------------------------------------*/

SCI_FILE *FFS_fopen(
    FILE_DEV_E_T file_dev_type,
    const wchar *filename,
    const char *mode );

int FFS_fclose(
    FILE_DEV_E_T file_dev_type,
    SCI_FILE *stream );

SIZE_T FFS_fread(
    FILE_DEV_E_T file_dev_type,
    void *buffer, 
    SIZE_T      size, 
    SIZE_T      count, 
    SCI_FILE *  stream );

SIZE_T FFS_fwrite(
    FILE_DEV_E_T file_dev_type,
    const void *buffer, 
    SIZE_T      size, 
    SIZE_T      count, 
    SCI_FILE *  stream );

int FFS_fputc(
    FILE_DEV_E_T file_dev_type,
    int c, 
    SCI_FILE *stream );

int FFS_fgetc(
    FILE_DEV_E_T file_dev_type,
    SCI_FILE *stream);

int FFS_fseek(
    FILE_DEV_E_T file_dev_type, 
    SCI_FILE *stream, 
    __int64     offset, 
    int     origin );

//Gets the current position of a file pointer.
uint32  FFS_ftell(
    FILE_DEV_E_T file_dev_type,
    SCI_FILE *stream );

/*
    Tests for end-of-file on a file.
*/
int FFS_feof(
    FILE_DEV_E_T file_dev_type, 
    SCI_FILE *stream );

/*
    Change file size.
*/
int FFS_chsize(
    FILE_DEV_E_T file_dev_type,
    SCI_FILE *stream, uint32  size);

int FFS_mk_dir(
    FILE_DEV_E_T file_dev_type,
    const wchar *path);
int FFS_rm_dir(
    FILE_DEV_E_T file_dev_type,
    const wchar *path);

FFS_ERROR_E FFS_SetAttr(
   FILE_DEV_E_T file_dev_type, 
   HFS handle,FFS_ATTR_E file_attr
       );

FFS_ERROR_E FFS_Format(
   FILE_DEV_E_T file_dev_type,
   FFS_OVERLAPPED_T *overlapped_ptr);

FFS_ERROR_E FFS_CloseAllFiles(FILE_DEV_E_T file_dev_type);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif         /*End FFS.h*/
