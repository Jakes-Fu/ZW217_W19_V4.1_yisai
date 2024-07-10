/******************************************************************************
 ** File Name:      msk_api_sfs.h                                                     *
 ** Author:                                                           *
 ** DATE:           8/16/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SFS.               *
 **                 (FAT File System)                                    *
 **                                                                           * 
 ******************************************************************************/

#ifndef _MSK_API_SFS_H
#define _MSK_API_SFS_H

#include "sci_types.h"
#include "sfs.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

SFS_ERROR_E SFS_API_RegisterDevice(/*in*/const uint16 * dev_name,/*out*/SFS_DEVICE_FORMAT_E * format_ptr);

SFS_ERROR_E SFS_API_UnRegisterDevice(const uint16 *dev_name);

SFS_ERROR_E SFS_API_Format(const uint16 *dev_name,SFS_DEVICE_FORMAT_E format_type,SFS_OVERLAPPED_T *overlapped_ptr);

SFS_ERROR_E SFS_API_GetDeviceStatus(const uint16* dev_name);

SFS_ERROR_E SFS_API_GetDeviceFreeSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);

SFS_ERROR_E SFS_API_GetDeviceUsedSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word);

SFS_HANDLE SFS_API_CreateFile(
								const uint16 *file_name, 
								uint32 access_mode,
								uint32 share_mode,	//Must be NULL,File System have not realize it 
								uint32 file_attri		//Must be NULL,File System have not realize it 
								);


SFS_ERROR_E SFS_API_DeleteFile(const uint16 *name,SFS_OVERLAPPED_T    *overlapped_ptr);

SFS_ERROR_E SFS_API_RenameFile(const uint16 *sour,const uint16 *dest,SFS_OVERLAPPED_T    *overlapped_ptr);

SFS_ERROR_E SFS_API_CreateDirectory(const uint16 *path);

SFS_ERROR_E SFS_API_DeleteDirectory(const uint16 *path);

SFS_HANDLE  SFS_API_FindFirstFile(const uint16 *find, SFS_FIND_DATA_T *fd);

SFS_ERROR_E SFS_API_FindNextFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T *fd);

SFS_ERROR_E SFS_API_FindClose(SFS_HANDLE sfs_handle);

SFS_ERROR_E SFS_API_GetFileSize(SFS_HANDLE sfs_handle, uint32*  size);

SFS_ERROR_E SFS_API_SetFileSize(SFS_HANDLE sfs_handle, uint32   size);

SFS_ERROR_E SFS_API_ReadFile(
							SFS_HANDLE handle,
							void *buffer,
							uint32 bytes_to_read,
							uint32 *bytes_read,
							SFS_OVERLAPPED_T *overlapped
							);


SFS_ERROR_E SFS_API_WriteFile(
							SFS_HANDLE handle,
							const void *buffer,
							uint32 bytes_to_write,
							uint32 *bytes_written,
							SFS_OVERLAPPED_T *overlapped
							);


SFS_ERROR_E SFS_API_SetFilePointer(
							SFS_HANDLE handle,
							int32 offset,
							uint32 origin
							);


SFS_ERROR_E SFS_API_GetFilePointer(
							SFS_HANDLE handle,
							uint32 origin,
							int32* currentpos 
							);
		

SFS_ERROR_E SFS_API_CloseFile(SFS_HANDLE handle);



#ifdef __cplusplus
    }
#endif

#endif // _FILESYS_H__