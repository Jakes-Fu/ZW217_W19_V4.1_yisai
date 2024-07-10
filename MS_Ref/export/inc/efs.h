/******************************************************************************
 ** File Name:      efs.h                                                     *
 ** Author:         Lin.liu                                                   *
 ** DATE:           11/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of EFS.               *
 **                 (Embedded File System)                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/

#ifndef _EFS_H
#define _EFS_H

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

/*
    the max length of file name, include '.' if have,
    eg:  abc.cpp , length = 7 + 1(\0)  = 8
*/
#define EFS_MAX_PATH          256 // 32 //@lin.liu. 32==>256 for J2ME required.

#define EFS_DIR_SEPARATE      ('\\')

/*
    the max handle that can be opened.
*/
#define EFS_MAX_HANDLE       16


// Following Config the EFS size
//
// The max file we support is EFS_MAX_FILE_ITEM
// The max file size(all file) == EFS_MAX_NORMAL_ITEM * EFS_BLOCK_SIZE
// You can increase these value, but need more SRAM.
// The SRAM used : (EFS_MAX_DATA_BLOCK_NUM + EFS_MAX_FILE_NUM) * sizeof(uint32)
// If you increase EFS_BLOCK_SIZE or define EFS_CACHE, need more SRAM(stack and pool)

// Total File Data Block Number
extern const uint32 g_file_system_data_block_num;
#define EFS_MAX_DATA_BLOCK_NUM       g_file_system_data_block_num  //2048

// Total File Header Block Number
extern const uint32 g_file_system_file_num;
#define EFS_MAX_FILE_NUM             g_file_system_file_num //128  // file header item number

// The size of File Data Block Number
#define EFS_BLOCK_SIZE               1024 // must be an even

// if defined it, EFS use SRAM to cache the read/write.
// This will improve capability of EFS, but need more resource.
#define EFS_CACHE           1


typedef enum EFS_ERROR_TAG
{

    EFS_NO_ERROR                   = 0,
    EFS_ERROR_NONE                 = 0,

    EFS_ERROR_DEVICE,

    EFS_ERROR_SYSTEM, 
    EFS_ERROR_INVALID_PARAM,
    EFS_ERROR_NO_ENOUGH_RESOURCE, 

    EFS_ERROR_IO_PENDING,   // IO operation is pending.
    EFS_ERROR_ACCESS,       // access denied

    EFS_ERROR_NOT_EXIST,    // file not exist.
    EFS_ERROR_HAS_EXIST,    // file has exist
    
/**------------------------------------------------------------------*
 **     device-lay error code                                        *
 **------------------------------------------------------------------*/
    EFS_ERROR_HARDWARE_FAILED,
    EFS_ERROR_WRITE_CONFLICT,

/**------------------------------------------------------------------*
 **     item-lay error code                                          *
 **------------------------------------------------------------------*/
    EFS_ERROR_NO_SPACE


/**------------------------------------------------------------------*
 **     IO-Task lay error code                                       *
 **------------------------------------------------------------------*/

/**------------------------------------------------------------------*
 **     interface-lay error code                                     *
 **------------------------------------------------------------------*/
}EFS_ERROR_E;


typedef uint32               HEFS;
#define EFS_INVALID_HANDLE   ((HEFS)0)


/**---------------------------------------------------------------------------*
 **    Overlapped struct                                                      *
 
    completed-route :
    void CALLBACK  FileIOCompletionRoutine(
        EFS_ERROR_E  error,    // error code of the relative opration.
        EFS_RESULT   result,   // result of the relative opration.
        EFS_PARAM    param);   // the param set by user.
**---------------------------------------------------------------------------*/

typedef uint32  EFS_PARAM;
typedef uint32  EFS_RESULT;

typedef void (*EFS_COMPLETETION_ROUTINE)(EFS_ERROR_E, EFS_RESULT result, EFS_PARAM param);

typedef struct EFS_OVERLAPPED_tag
{
   uint32                     offset;  /* Specifies a file position at which to 
                                          start the transfer. The file position
                                          is offset from the start of the file  */

   EFS_COMPLETETION_ROUTINE   complete_route; // Complete routine.
                                              // This will be called after request completed.
   EFS_PARAM                  param;          // user will set this param(it will be as the 
                                              // input param of completetion routine.
}EFS_OVERLAPPED_T;


/**---------------------------------------------------------------------------*
 **    Open/Create  flag                                                      *
 **                                                                           *
 **      Mode                            Description                          *
 **---------------------------------------------------------------------------*
         EFS_MODE_READ                   Specifies read access to the object. 
                                         Data can be read from the file and
                                         the file pointer can be moved.
   ----------------------------------------------------------------------------                                                
         EFS_MODE_WRITE                  Specifies write access to the object.
                                         Data can be written to the file and 
                                         the file pointer can be moved
   ----------------------------------------------------------------------------
         EFS_MODE_SHARE_READ             Subsequent open operations on the
                                         object will succeed only if read 
                                         access is requested.
   ----------------------------------------------------------------------------
         EFS_MODE_SHARE_WRITE            Subsequent open operations on the
                                         object will succeed only if write
                                         access is requested
   ----------------------------------------------------------------------------
         EFS_MODE_CREATE_NEW             Creates a new file. 
                                         The function fails if the specified 
                                         file already exists.
   ----------------------------------------------------------------------------      
         EFS_MODE_CREATE_ALWAYS          Creates a new file.
                                         If the file exists, the function 
                                         overwrites the file.
   ----------------------------------------------------------------------------
         EFS_MODE_OPEN_EXISTING          Opens the file.
                                         The function fails if the file 
                                         does not exist.
   ----------------------------------------------------------------------------
         EFS_MODE_OPEN_ALWAYS            Opens the file, if it exists.
                                         If the file does not exist, the function
                                         creates a new file.
   ----------------------------------------------------------------------------
         EFS_MODE_APPEND                 Open/create the file, all write operations 
                                         occur at the end of the file even if set/seek
                                         the file position.
   
 **---------------------------------------------------------------------------*/
typedef enum EFS_MODE_TAG
{   
    /* 0      1       2             3            4 5 6 7      7......31
    +---------------------------------------------------------------------+
    | read | write | share_read | share_write | create/open | bit 7..31   |
    +---------------------------------------------------------------------+   */

    // Bit 0-1
    EFS_MODE_READ            = 0x0001,  
    EFS_MODE_WRITE           = 0x0002,
         
    // Bit 2-3
    EFS_MODE_SHARE_READ      = 0x0004,
    EFS_MODE_SHARE_WRITE     = 0x0008,

    // Bit 4-7
    EFS_MODE_CREATE_NEW      = 0x0010,
    EFS_MODE_CREATE_ALWAYS   = 0x0020,
    EFS_MODE_OPEN_EXISTING   = 0x0030,
    EFS_MODE_OPEN_ALWAYS     = 0x0040,
    EFS_MODE_APPEND          = 0x0050
}EFS_MODE_E;

#define EFS_MODE_ACCESS_MASK     0x000F
#define EFS_MODE_CREATE_MASK     0x00F0

/*
    File Attribute Define.    
*/
typedef enum
{
    // Bit 0
    EFS_ATTR_READONLY = 0x1,

    // Bit 7
    EFS_ATTR_DIR  = 0x80    
}EFS_ATTR_E;

/*
    Find Data Define.
    The receives information about the found file.
*/
typedef struct EFS_FIND_DATA_TAG
{
    uint32          create_time;         // seconds counts that escaped from 2000/01/01/ 00:00:00.
    uint16          attr;                // see EFS_ATTR_E.
    char            name[EFS_MAX_PATH];  // The file/dir name we found, If find failed,
                                         // the data of this feild is not init-ed.
}EFS_FIND_DATA_T;


/*
    Seek Macro define.
*/
#define EFS_SEEK_CUR        0  /*Current position of file pointer */
#define EFS_SEEK_END        1  /*End of file   */
#define EFS_SEEK_BEGIN      2  /*Beginning of file */



/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
 
// Init the EFS Module
BOOLEAN EFS_Init(void);   
 
/*
    close file handle
*/
void EFS_Close(HEFS  handle);

/*
    Get the length of a file.
*/
uint32 EFS_GetLength(HEFS handle);

/*
    Create or open a file.    
*/
HEFS EFS_Create(
    const char *name,
    uint16  mode); // see EFS_MODE_E.

/*
    Move file pointer.
*/
EFS_ERROR_E EFS_Seek(
    HEFS    handle,
    long    offset,
    uint8   origin);

/*
    Read data from the file.
    If overlapped_ptr is NULL, this function wouldn't return until read all data.
    If *NOT* NULL, you must set complete_route, and the callback will be called
    after the operation completed.
    the result field of the callback indicate the count that has read.
    and you MUST not release the buf before the callback be called.
*/
EFS_ERROR_E EFS_Read(
    HEFS                handle, // the handle return by EFS_Create.
    uint8               *buf,
    uint32              len,
    uint32              *transmitted,
    EFS_OVERLAPPED_T    *overlapped_ptr);

/*
    Write data to the file.
    If overlapped_ptr is NULL, this function wouldn't return until read all data.
    If *NOT* NULL, you must set complete_route, and the callback will be called
    after the operation completed.
    the result field of the callback indicate the count that has been written.
    and you MUST not release the buf before the callback be called.
*/
EFS_ERROR_E EFS_Write(
    HEFS                handle,
    uint8               *buf,    // the handle return by EFS_Create.
    uint32              len,
    uint32              *transmitted,
    EFS_OVERLAPPED_T    *overlapped_ptr);


/*
    Delete a file.
    If the file being opened, delete operation will fail.
*/
EFS_ERROR_E EFS_Delete(
    const char *name);

/*
    Rename a file.
*/
EFS_ERROR_E EFS_Rename(
    const char *sour,
    const char *dest);

/*
    Read/Write physical device directly.
    This function only be called by system task, I will move it to other header file later.
*/
EFS_ERROR_E EFS_Direct(
    BOOLEAN    transmit_direction,   /* if true, read. otherwize write  */
    uint8      *buf,
    uint32     addr,                 /* Physical device.                */
    uint32     len);
 
/*
    Get Free Flash Space
*/
uint32 EFS_GetFreeSpace(void);

/*
    Get Used Flash Space.
*/
uint32 EFS_GetUsedSpace(void);

/*
    Is the file exist ?
*/
BOOLEAN EFS_CheckExist(const char *file);
 
/*
    Try find the first file/directory that math the given string.
    and you can get the file infomation from fd.
*/
HEFS EFS_FindFirst(
    const char *find,
    EFS_FIND_DATA_T *fd);  // Search result

/*
    Continue search the file that match the given string.
    the string should be same as EFS_FindFirst.
*/
EFS_ERROR_E EFS_FindNext(
    HEFS find_handle,      // the handle that return by EFS_FindFirst.
    const char *find,      // same as EFS_FindFirst.
    EFS_FIND_DATA_T *fd);  // search result

/*
    Create a dir
*/
EFS_ERROR_E  EFS_CreateDir(const char *path);

/*
    Delete a dir
*/
EFS_ERROR_E  EFS_DeleteDir(const char *path);


/*
    Change the file size.
    Input:
        stream : the file handler that returned by EFS_Create(); 
        size   : the file size want to be changed.
    Return:
        the file size after changed, maybe less than the size if no enough free sapce.
*/
uint32 EFS_ChangeSize( HEFS stream, uint32  size );

/*
    Get the files/dir number in the file system.
    Input:
        none
    Return:
        the numbers of files/dir in the file system.
*/
uint32  EFS_GetFileCount( void );




/**---------------------------------------------------------------------------*
 **  ANSI File Interface
 **
 **---------------------------------------------------------------------------*/
typedef int         SCI_FILE;
typedef uint32      SIZE_T;

SCI_FILE *EFS_fopen( const char *filename, const char *mode );

int EFS_fclose( SCI_FILE *stream );

SIZE_T EFS_fread( void *buffer, 
    SIZE_T      size, 
    SIZE_T      count, 
    SCI_FILE *  stream );

SIZE_T EFS_fwrite( const void *buffer, 
    SIZE_T      size, 
    SIZE_T      count, 
    SCI_FILE *  stream );

int EFS_fputc( int c, SCI_FILE *stream );

int EFS_fgetc(SCI_FILE *stream);

int EFS_fseek( SCI_FILE *stream, 
    long    offset, 
    int     origin );

//Gets the current position of a file pointer.
long EFS_ftell( SCI_FILE *stream );

/*
    Tests for end-of-file on a file.
*/
int EFS_feof( SCI_FILE *stream );

/*
    Change file size.
*/
int EFS_chsize(SCI_FILE *stream, long);

int EFS_mk_dir(const char *path);
int EFS_rm_dir(const char *path);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif         /*End efs.h*/

