/******************************************************************************
 ** File Name:      msk_api_sfs.c                                                *
 ** Author:                                                                   *
 ** DATE:           08/07/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SFS.               
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/07/2006   andy.bian          Create.                                 *
 ******************************************************************************/

#include "sfs.h"  
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#define MSKERNEL_SFS_API __declspec(dllexport)
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Function definition                               *
 **---------------------------------------------------------------------------*/
//james remove for unicode
#if 0
//andy add 2006-1-8 
void uniswap(uint16 *dest)
{
	uint16 length,i;
	uint16 value=0;
	
	//get unicode string length
	length = 0;  
	while (0 != *(dest+length))	
		length++;
	
	for(i = 0; i < length; i++)
	{
		value =*dest;
		*dest=(((value) >> 8) & 0xff) | (((value) << 8) & 0xff00);
		dest++;
	}
}
		
//andy add 2006-1-8
uint16* uniconvert(uint16 *dest,const uint16* sour )
{
	uint16 length=0;
	uint16 value =0;
	uint16 i =0;
	

	//get unicode string length
	length = 0;  
	while (0 != *(sour+length))	
		length++;

	for(i = 0; i <= length; i++)
	{
		value =*(sour+i);
		*(dest+i)=(((value) >> 8) & 0xff) | (((value) << 8) & 0xff00);
	}

	return dest;
}
#endif



MSKERNEL_SFS_API void SFS_API_Init(void)
{
    return;
}

MSKERNEL_SFS_API SFS_ERROR_E SFS_API_RegisterDevice(
                               const uint16         *dev_name,
                               SFS_DEVICE_FORMAT_E  *format_ptr
                               )
{
    //james remove for unicode
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert(pstr,dev_name);	  
	return SFS_RegisterDevice( dev_name, format_ptr);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_UnRegisterDevice(
                                 const uint16       *dev_name
                                 )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert(pstr,dev_name);
	return SFS_UnRegisterDevice( dev_name );
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_Format(
                       const uint16         *dev_name,
                       SFS_DEVICE_FORMAT_E  format_type,
                       SFS_OVERLAPPED_T     *overlapped_ptr
                       )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,dev_name);
	return SFS_Format( dev_name,format_type,overlapped_ptr);

}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_GetDeviceStatus(
                               const uint16     * dev_name
                               )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,dev_name);
	return  SFS_GetDeviceStatus( dev_name );
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_GetDeviceFreeSpace(
                                   const uint16     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,device_name);
	return SFS_GetDeviceFreeSpace( device_name,high_word,low_word);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_GetDeviceUsedSpace(
                                   const uint16     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,device_name);
	return SFS_GetDeviceUsedSpace( device_name,high_word,low_word);
}



/**************/
//file and dir operation
/**************/
MSKERNEL_SFS_API SFS_HANDLE SFS_API_CreateFile(
                        const uint16    *file_name, 
                        uint32          access_mode,
                        uint32          share_mode,	//Must be NULL,File System have not realize it 
                        uint32          file_attri		//Must be NULL,File System have not realize it 
                        )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,file_name);
	return  SFS_CreateFile(file_name,access_mode,share_mode,file_attri);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_DeleteFile(
                           const uint16         *name,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,name);
	return SFS_DeleteFile( name,overlapped_ptr);

}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_RenameFile(
                           const uint16         *sour,
                           const uint16         *dest,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
	//uint16 pstr1[SFS_MAX_PATH+1]={0};
	//uint16 pstr2[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr1,sour);
	//uniconvert((uint16*)pstr2,dest);
	return SFS_RenameFile(sour,dest,overlapped_ptr);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_CreateDirectory(
                                const uint16    *path
                                )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,path);
	return  SFS_CreateDirectory(path);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_DeleteDirectory(
                                const uint16    *path
                                )
{
	//uint16 pstr[SFS_MAX_PATH+1]={0};
	//uniconvert((uint16*)pstr,path);
	return 	SFS_DeleteDirectory(path);
}


MSKERNEL_SFS_API SFS_HANDLE SFS_API_FindFirstFile(
                             const uint16       *find, 
                             SFS_FIND_DATA_T    *fd
                             )
{
	SFS_HANDLE handle;
	//uint16 pstr[SFS_MAX_PATH+1]={0};

	//uniconvert((uint16*)pstr,find);
    handle= SFS_FindFirstFile(find,fd);
	//uniswap(fd->name);

	return handle;
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_FindNextFile(
                             SFS_HANDLE         sfs_handle, 
                             SFS_FIND_DATA_T    *fd
                             )
{
    SFS_ERROR_E err= SFS_FindNextFile(sfs_handle,fd);
	//uniswap(fd->name);
	return err;
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_FindClose(
                          SFS_HANDLE    sfs_handle
                          )
{
    return SFS_FindClose(sfs_handle);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_GetFileSize(
                            SFS_HANDLE  sfs_handle, 
                            uint32      *file_size
                            )
{
    return SFS_GetFileSize(sfs_handle,file_size);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_SetFileSize(
                            SFS_HANDLE      sfs_handle, 
                            uint32          size
                            )
{
    return SFS_SetFileSize(sfs_handle,size);
}



MSKERNEL_SFS_API SFS_ERROR_E SFS_API_ReadFile(
	                    SFS_HANDLE          handle,
	                    void                *buffer,
	                    uint32              bytes_to_read,
	                    uint32              *bytes_read,
	                    SFS_OVERLAPPED_T    *overlapped
                        )
{
    return SFS_ReadFile(handle,buffer,bytes_to_read,bytes_read,overlapped);
}



MSKERNEL_SFS_API SFS_ERROR_E SFS_API_WriteFile(
	                    SFS_HANDLE          handle,
	                    const void          *buffer,
	                    uint32              bytes_to_write,
	                    uint32              bytes_written,
	                    SFS_OVERLAPPED_T    *overlapped
                        )
{
    return SFS_WriteFile(handle,buffer,bytes_to_write,bytes_written,overlapped);
}


MSKERNEL_SFS_API uint32 SFS_API_SetFilePointer(
	                    SFS_HANDLE      handle,
	                    int32           offset,
	                    uint32          origin
                        )
{
    return SFS_SetFilePointer(handle,offset,origin);
}


MSKERNEL_SFS_API int32 SFS_API_GetFilePointer(
	                    SFS_HANDLE      handle,
	                    uint32          origin
                        )
{
    return SFS_GetFilePointer(handle,origin,NULL);
}


MSKERNEL_SFS_API SFS_ERROR_E SFS_API_CloseFile(
                          SFS_HANDLE    handle
                          )
{
    return SFS_CloseFile(handle);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

	
