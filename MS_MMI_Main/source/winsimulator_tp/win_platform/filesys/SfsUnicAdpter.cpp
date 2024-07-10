/******************************************************************************
 ** File Name:      sfsunicadpter.c                                                *
 ** Author:         Apple Gao                                                          *
 ** DATE:           12/40/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                             
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/30/2009   applegao          Create.                                 *
 ******************************************************************************/
#include "stdafx.h"
#include "sfsunicadpter.h"  
#ifndef UNICODE
#include "atlconv.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Function definition                               *
 **---------------------------------------------------------------------------*/

SFS_ERROR_E SFS_API_RegisterDevice_W(
                               const TCHAR         *dev_name,
                               SFS_DEVICE_FORMAT_E  *format_ptr
                               )
{
#ifdef UNICODE
	return SFS_API_RegisterDevice( dev_name, format_ptr);
#else
	USES_CONVERSION;
	return SFS_API_RegisterDevice( A2W(dev_name), format_ptr);
#endif
}


SFS_ERROR_E SFS_API_UnRegisterDevice_W(
                                 const TCHAR       *dev_name
                                 )
{
#ifdef UNICODE
	return SFS_API_UnRegisterDevice( dev_name );
#else	
	USES_CONVERSION;
	return SFS_API_UnRegisterDevice( A2W(dev_name) );
#endif
}


SFS_ERROR_E SFS_API_Format_W(
                       const TCHAR         *dev_name,
                       SFS_DEVICE_FORMAT_E  format_type,
                       SFS_OVERLAPPED_T     *overlapped_ptr
                       )
{
#ifdef UNICODE
	return SFS_API_Format( dev_name,format_type,overlapped_ptr);
#else	
	USES_CONVERSION;
	return SFS_API_Format( A2W( dev_name),format_type,overlapped_ptr);
#endif
}


SFS_ERROR_E SFS_API_GetDeviceStatus_W(
                               const TCHAR     * dev_name
                               )
{
#ifdef UNICODE
	return  SFS_API_GetDeviceStatus( dev_name );
#else	
	USES_CONVERSION;
	return  SFS_API_GetDeviceStatus( A2W(dev_name) );
#endif
}


SFS_ERROR_E SFS_API_GetDeviceFreeSpace_W(
                                   const TCHAR     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
#ifdef UNICODE
	return SFS_API_GetDeviceFreeSpace( device_name,high_word,low_word);
#else	
	USES_CONVERSION;
	return SFS_API_GetDeviceFreeSpace( A2W(device_name),high_word,low_word);
#endif
}


SFS_ERROR_E SFS_API_GetDeviceUsedSpace_W(
                                   const TCHAR     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
#ifdef UNICODE
	return SFS_API_GetDeviceUsedSpace( device_name,high_word,low_word);
#else	
	USES_CONVERSION;
	return SFS_API_GetDeviceUsedSpace( A2W(device_name),high_word,low_word);
#endif
}



/**************/
//file and dir operation
/**************/
SFS_HANDLE SFS_API_CreateFile_W(
                        const TCHAR    *file_name, 
                        uint32          access_mode,
                        uint32          share_mode,	//Must be NULL,File System have not realize it 
                        uint32          file_attri		//Must be NULL,File System have not realize it 
                        )
{
#ifdef UNICODE
	return  SFS_API_CreateFile( file_name,access_mode,share_mode,file_attri);
#else	
	USES_CONVERSION;
	return  SFS_API_CreateFile( A2W(file_name),access_mode,share_mode,file_attri);
#endif
}


SFS_ERROR_E SFS_API_DeleteFile_W(
                           const TCHAR         *name,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
#ifdef UNICODE
	return SFS_API_DeleteFile( name,overlapped_ptr);
#else	
	USES_CONVERSION;
	return SFS_API_DeleteFile( A2W(name),overlapped_ptr);
#endif	
}


SFS_ERROR_E SFS_API_RenameFile_W(
                           const TCHAR         *sour,
                           const TCHAR         *dest,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
#ifdef UNICODE
	return SFS_API_RenameFile( sour,dest,overlapped_ptr);
#else	
	USES_CONVERSION;
	return SFS_API_RenameFile( A2W(sour),A2W(dest),overlapped_ptr);
#endif
}


SFS_ERROR_E SFS_API_CreateDirectory_W(
                                const TCHAR    *path
                                )
{
#ifdef UNICODE
	return  SFS_API_CreateDirectory( path );
#else	
	USES_CONVERSION;
	return  SFS_API_CreateDirectory( A2W(path));
#endif
}


SFS_ERROR_E SFS_API_DeleteDirectory_W(
                                const TCHAR    *path
                                )
{
#ifdef UNICODE
	return 	SFS_API_DeleteDirectory(path);
#else	
	USES_CONVERSION;
	return 	SFS_API_DeleteDirectory(A2W(path));
#endif
}


SFS_HANDLE SFS_API_FindFirstFile_W(
                             const TCHAR       *find, 
                             SFS_FIND_DATA_T    *fd
                             )
{
	SFS_HANDLE handle;
#ifdef UNICODE
	handle= SFS_API_FindFirstFile(find,fd);
#else	
	USES_CONVERSION;
	handle= SFS_API_FindFirstFile(A2W(find),fd);
#endif
	return handle;
}
