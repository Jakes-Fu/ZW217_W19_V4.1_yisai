/******************************************************************************
 ** File Name:      sfsunicadpter.h                                                     *
 ** Author:          Apple Gao                                               *
 ** DATE:           12/30/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                       *
 **                                                                           * 
 ******************************************************************************/

#ifndef _SFS_UNIC_ADPTER_H
#define _SFS_UNIC_ADPTER_H

#include "msk_api_sfs.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


SFS_ERROR_E SFS_API_RegisterDevice_W(/*in*/const TCHAR * dev_name,/*out*/SFS_DEVICE_FORMAT_E * format_ptr);

SFS_ERROR_E SFS_API_UnRegisterDevice_W(const TCHAR *dev_name);

SFS_ERROR_E SFS_API_Format_W(const TCHAR *dev_name,SFS_DEVICE_FORMAT_E format_type,SFS_OVERLAPPED_T *overlapped_ptr);

SFS_ERROR_E SFS_API_GetDeviceStatus_W(const TCHAR* dev_name);

SFS_ERROR_E SFS_API_GetDeviceFreeSpace_W(const TCHAR *device_name,uint32 *high_word,uint32 *low_word);

SFS_ERROR_E SFS_API_GetDeviceUsedSpace_W(const TCHAR *device_name,uint32 *high_word,uint32 *low_word);

SFS_HANDLE SFS_API_CreateFile_W(
								const TCHAR *file_name, 
								uint32 access_mode,
								uint32 share_mode,	//Must be NULL,File System have not realize it 
								uint32 file_attri		//Must be NULL,File System have not realize it 
								);


SFS_ERROR_E SFS_API_DeleteFile_W(const TCHAR *name,SFS_OVERLAPPED_T    *overlapped_ptr);

SFS_ERROR_E SFS_API_RenameFile_W(const TCHAR *sour,const TCHAR *dest,SFS_OVERLAPPED_T    *overlapped_ptr);

SFS_ERROR_E SFS_API_CreateDirectory_W(const TCHAR *path);

SFS_ERROR_E SFS_API_DeleteDirectory_W(const TCHAR *path);

SFS_HANDLE  SFS_API_FindFirstFile_W(const TCHAR *find, SFS_FIND_DATA_T *fd);




#endif // _SFS_UNIC_ADPTER_H