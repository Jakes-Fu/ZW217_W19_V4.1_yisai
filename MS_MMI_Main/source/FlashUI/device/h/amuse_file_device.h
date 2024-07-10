/*
*  amuse_file.h
*  Copyright (C) 2011 Microrapid, Inc.
*  Authors: Tim <tim@Microrapid.com>
*  This file is a part of Amuse(TM).
*/
#ifndef _AMUSE_FILE_H_
#define _AMUSE_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C	__declspec(dllimport)
#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
#endif

/* File macro definitions */
#define FILE_HANDLE unsigned int
#define FILE_NAME const char*
#define OPEN_MODE const char*
#define OPEN_MODE_READ_BIN "rb"
#define OPEN_MODE_WRITE_BIN "wb"
#define OPEN_MODE_READ_AND_WRITE_BIN "rb+"
#define FILE_OK 0
#define FILE_ERROR -1
#define MIN_FREE_SPACE 0

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#if IPHONE_SDK
#include <sys/types.h>	
#include <dirent.h>
#include <sys/dirent.h>
#define DIR_HANDLE DIR*
#define DIR_ENTRY struct dirent*
#endif


#define FILE_PATH_MAX_LEN 256

typedef struct 
{
	char str_u8[(FILE_PATH_MAX_LEN+1)*3];
	short str_u16[FILE_PATH_MAX_LEN+1];
}Amuse_Mtk_String;

extern Amuse_Mtk_String gAmuseMtkStr;
/* 
* Open a file.
* fileName: file name
* mode: mode to open file
* return value: file handler to the open file
*/
EXPORT_C FILE_HANDLE amuse_fopen_device(FILE_NAME fileName, OPEN_MODE mode);

/*
* Open a bitmap file.
* fileName: file name
* mode: mode to open file
* return value: file handler to the open file
*/
EXPORT_C FILE_HANDLE amuse_fopen_bitmap(FILE_NAME fileName, OPEN_MODE mode);

/* 
* Close a file.
* f: file handle
* return value: if successful return FILE_OK, otherwise return FILE_ERROR to indicate an error
*/
EXPORT_C int amuse_fclose_device(FILE_HANDLE f);

/*
 * Get the position of the file handle.
 * f: file handle
 * return value: the current file position
 */
EXPORT_C int amuse_ftell(FILE_HANDLE f);

/* 
* Moves the file pointer to a specified location.
* f: file handle
* offset: number of bytes from origin
* origin: initial position.SEE_SET=0,SEEK_CUR=1,SEEK_END=2
* return value: if successful return FILE_OK
*/
EXPORT_C int amuse_fseek(FILE_HANDLE f, int offset, int origin);

/*
* Reads data from a file.
* f: file handle
* pData: data buffer to read
* size: item size in byte
* count: maximum number of items to be read
* return value: if successful return count of bytes actually read, otherwise return FILE_ERROR
*/
EXPORT_C int amuse_fread(FILE_HANDLE f, void* pData, int size, int count);

/*
* Writes data to a file.
* f: file handle
* pData: data buffer to write
* size: item size in byte
* count: maximum number of items to be written
* return value: if successful return count of bytes actually written, otherwise return FILE_ERROR
*/
EXPORT_C int amuse_fwrite(FILE_HANDLE f, void* pData, int size, int count);

/*
* Reads a character from a file.
* f: file handle
* return value: returns the character read as an int or returns FILE_ERROR to indicate an error or end of file
*/
EXPORT_C int amuse_fgetc(FILE_HANDLE f);

/*
* Writes a character to a file.
* f: file handle
* c: character to be written
* return value: return the character written or return FILE_ERROR indicates an error
*/
EXPORT_C int amuse_fputc(FILE_HANDLE f, int c);

/*
 * Print formatted data to a file.
 * f: file handle
 * format: format specification
 * return value: returns the number of bytes written if success, or -1 if error
 */
EXPORT_C int amuse_fprintf(FILE_HANDLE f, const char* format, ...);


/*
 * Flushes a stream.
 * f: file handle
 * return value: returns 0 if the buffer was successfully flushed, return FILE_ERROR if error
 */
EXPORT_C int amuse_fflush(FILE_HANDLE f);

/*
 * This function tests for an error on a stream.
 * f: file handle
 * return value: returns 0 if no error has occurred, otherwise returns a nonzero value
 */
EXPORT_C int amuse_ferror(FILE_HANDLE f);

/*
 * Tests for an error on a stream.
 * f: file handle
 * return value: returns a nonzero value if a read operation has attempted to read past the end of the file, 
 * otherwise returns 0.
 */
EXPORT_C int amuse_feof(FILE_HANDLE f);

/*
 * Delete a file.
 * f: file name
 * return value: returns 0 if the file is successfully deleted, otherwise it returns -1.
 */
EXPORT_C int amuse_remove(FILE_NAME f);

/*
 * Rename a file.
 * oldfn: old file name
 * newfn: new file name
 * return value: returns 0 if it is successful, otherwise returns a nonzero value.
 */
EXPORT_C int amuse_rename(FILE_NAME oldfn, FILE_NAME newfn); 

/*
 * Get file size by file name or file handle, use 
 * one of the two parameters with the other one set 0.
 *
 * fileName: file name
 * fileHandler: file handle
 * return value: if successful return count of bytes of the specified file in local disk,
 *				 otherwise return FILE_ERROR.
 */
EXPORT_C unsigned int amuse_getFileSize(FILE_NAME fileName, FILE_HANDLE fileHandle);

/*
* Create a new directory.
* 
* dirname: path for a new directory.
* return value: returns 0 if success, otherwise returns -1.
*/
EXPORT_C int amuse_mkdir(const char* dirname);

/*
 * Create a new file.
 * fileName: name of the new file.
 * return value: returns 0 if success, otherwise returns -1.
 */
EXPORT_C int amuse_create_file(FILE_NAME fileName);

/*
 * Check free space on disk.
 * disk: disk label
 * min_size: minimal free space of disk
 * return value: returns 0 if free space size is more than min_size, otherwise returns -1.
 */
EXPORT_C int amuse_check_free_space(char disk, int min_size);

/*
 * This function gets directory path separator, which is platform dependent.
 *
 * Parameters: None
 *
 * Returns: the separator char
 */
EXPORT_C char* amuse_get_path_separator();

/*
 * This function detects whether a path string is full path or relative path
 *
 * Parameters: 
 * path: the path string
 *
 * Returns: 1, full path; 0, relative path
 */
EXPORT_C int amuse_is_full_path(char* path);

/*
* This function detects whether a path string is local disk path or not
*
* Parameters: 
* path: the path string
*
* Returns: 1, local disk path; 0, not local disk path
*/
EXPORT_C int amuse_is_local_disk_path(char* path);
	
/*
* This function gets the file cached path
*
* Parameters: 
* path: the url string
*
* Returns: the file cached path
*/
EXPORT_C char* getLocalFilePath(char* url);

/*
* This function get the cached file data length
*
* Parameters: 
* path: the cached file name
*
* Returns: 1, file cached data length
*/
EXPORT_C int fileCached(char* path);

/*
 * This function print formatted string to standard i/o device
 *
 * Parameters: 
 * format :Format control.
 * argument (the ...):  Optional arguments.
 *
 * Returns: Returns the number of characters printed, 
 * or a negative value if an error occurs. 
 */
#if USE_AIRPLAY
#include "s3eDebug.h"
#define amuse_printf  s3eDebugTracePrintf 
#else
EXPORT_C void amuse_printf(const char* format, ...);
#endif
/*
*  This function checks if need check if word based.
*
*  Parameters:   
*
*  Returns: true or false
*/
EXPORT_C int need_check_if_word_based();
EXPORT_C void set_working_directory(char* filePath);
EXPORT_C void get_working_directory(int size, char* p);
EXPORT_C void get_save_path(int size,char *path);
EXPORT_C void amuse_getWorkingDirectory(int size, char* p);
EXPORT_C void amuse_print_init(void);
EXPORT_C void amuse_print_destroy(void);

#ifdef __cplusplus
}
#endif

#endif
