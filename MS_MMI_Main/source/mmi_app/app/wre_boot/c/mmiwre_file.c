#ifdef WRE_SUPPORT
#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "sfs.h"
#include "mmiwre_file.h"


#ifdef _DEBUG
void adapt_filetest()
{
	SCI_ASSERT(ADAPT_SFS_NO_ERROR == SFS_NO_ERROR);                 /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_ERROR_HAS_EXIST == SFS_ERROR_HAS_EXIST);   /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_ERROR_NORES_HANDLE == SFS_ERROR_NORES_HANDLE); /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_MODE_READ == SFS_MODE_READ);               /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_MODE_APPEND == SFS_MODE_APPEND);           /*assert verified*/

	SCI_ASSERT(ADAPT_SFS_ATTR_NONE == SFS_ATTR_NONE);               /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_ATTR_HIDDEN == SFS_ATTR_HIDDEN);           /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_ATTR_ARCH == SFS_ATTR_ARCH);               /*assert verified*/
	SCI_ASSERT(sizeof(ADAPT_SFS_FIND_DATA_T) ==sizeof(SFS_FIND_DATA_T));    /*assert verified*/
	SCI_ASSERT(ADAPT_SFS_MAX_PATH == SFS_MAX_PATH);                 /*assert verified*/
}
#endif

/*****************************************************************************/
//  Description : ADAPT_SFS_CreateFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_HANDLE ADAPT_SFS_CreateFile(
						  const uint16 *file_name, 
						  uint32 access_mode,
						  uint32 share_mode,	//Must be NULL,File System have not realize it 
						  uint32 file_attri		//Must be NULL,File System have not realize it 
						  )
{
	return (ADAPT_SFS_HANDLE)SFS_CreateFile(file_name,access_mode,share_mode,file_attri);	
}

/*****************************************************************************/
//  Description : ADAPT_SFS_CloseFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_CloseFile(ADAPT_SFS_HANDLE handle)
{
	return (ADAPT_SFS_ERROR_E)SFS_CloseFile((SFS_HANDLE)handle);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_ReadFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_ReadFile(
	ADAPT_SFS_HANDLE handle,
	void *buffer,
	uint32 bytes_to_read,
	uint32 *bytes_read,
	void *overlapped )
{
	return (ADAPT_SFS_ERROR_E)SFS_ReadFile((SFS_HANDLE)handle,buffer,bytes_to_read,bytes_read,(SFS_OVERLAPPED_T*)overlapped);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_WriteFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_WriteFile(
	ADAPT_SFS_HANDLE handle,
	const void *buffer,
	uint32 bytes_to_write,
	uint32 *bytes_written,
	void *overlapped )
{
	return (ADAPT_SFS_ERROR_E)SFS_WriteFile((SFS_HANDLE)handle,buffer,bytes_to_write,bytes_written,(SFS_OVERLAPPED_T*)overlapped);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_CreateDirectory
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_CreateDirectory(const uint16 *path)
{
	return (ADAPT_SFS_ERROR_E)SFS_CreateDirectory(path);
}


/*****************************************************************************/
//  Description : ADAPT_SFS_DeleteFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_DeleteFile(const uint16 *name,void *overlapped_ptr)
{
	return (ADAPT_SFS_ERROR_E)SFS_DeleteFile(name,(SFS_OVERLAPPED_T *)overlapped_ptr);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_FindClose
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_FindClose(ADAPT_SFS_HANDLE sfs_handle)
{
	return (ADAPT_SFS_ERROR_E)SFS_FindClose((SFS_HANDLE)sfs_handle);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_FindFirstFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_HANDLE ADAPT_SFS_FindFirstFile(const uint16 *find, ADAPT_SFS_FIND_DATA_T *fd)
{
	return SFS_FindFirstFile(find,(SFS_FIND_DATA_T*)fd);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_FindNextFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_FindNextFile(ADAPT_SFS_HANDLE sfs_handle, ADAPT_SFS_FIND_DATA_T *fd)
{
	return (ADAPT_SFS_ERROR_E)SFS_FindNextFile((SFS_HANDLE)sfs_handle,(SFS_FIND_DATA_T*)fd);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_FlushFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_FlushFile(
	ADAPT_SFS_HANDLE handle,
	void *overlapped
)
{
	return (ADAPT_SFS_ERROR_E)SFS_FlushFile((SFS_HANDLE)handle,(SFS_OVERLAPPED_T*)overlapped);
}


/*****************************************************************************/
//  Description : ADAPT_SFS_GetDeviceFreeSpace
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceFreeSpace(
	const uint16 *device_name,
	uint32 *high_word,
	uint32 *low_word
)
{
	return (ADAPT_SFS_ERROR_E)SFS_GetDeviceFreeSpace(device_name,high_word,low_word);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_GetDeviceUsedSpace
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceUsedSpace(
	const uint16 *pathName,
	uint32 *high_word,
	uint32 *low_word
)
{
	return (ADAPT_SFS_ERROR_E)SFS_GetDeviceUsedSpace(pathName,high_word,low_word);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_GetDeviceStatus
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetDeviceStatus(const uint16* dev_name)
{
	return (ADAPT_SFS_ERROR_E)SFS_GetDeviceStatus(dev_name);
}



/*****************************************************************************/
//  Description : ADAPT_SFS_GetAttr
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetAttr(const uint16 *name, uint16* attr)
{
	return (ADAPT_SFS_ERROR_E)SFS_GetAttr(name,attr);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_GetFileSize
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetFileSize(ADAPT_SFS_HANDLE sfs_handle, uint32*  size)
{
	return (ADAPT_SFS_ERROR_E)SFS_GetFileSize((SFS_HANDLE)sfs_handle,size);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_DeleteDirectory
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_DeleteDirectory(const uint16 *path)
{
	return (ADAPT_SFS_ERROR_E)SFS_DeleteDirectory(path);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_GetFilePointer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_GetFilePointer(
	ADAPT_SFS_HANDLE handle,
	uint32 origin,
	int* currentpos )
{
	return (ADAPT_SFS_ERROR_E)SFS_GetFilePointer((SFS_HANDLE)handle,origin,(int32 *)currentpos);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_SetFileSize
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_SetFileSize(ADAPT_SFS_HANDLE sfs_handle, uint32   size)
{
	return (ADAPT_SFS_ERROR_E)SFS_SetFileSize((SFS_HANDLE)sfs_handle,size);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_SetAttr
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_SetAttr(const uint16 *name, uint16 attr)
{
	return (ADAPT_SFS_ERROR_E)SFS_SetAttr(name,attr);
}

/*****************************************************************************/
//  Description : ADAPT_SFS_SetFilePointer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_SetFilePointer(
	ADAPT_SFS_HANDLE handle,
	int offset,
	uint32 origin )
{
	return (ADAPT_SFS_ERROR_E)SFS_SetFilePointer((SFS_HANDLE)handle,(__int64)offset,origin);
}


/*****************************************************************************/
//  Description : ADAPT_SFS_RenameFile
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
ADAPT_SFS_ERROR_E ADAPT_SFS_RenameFile(const uint16 *sour,const uint16 *dest,void    *overlapped_ptr)
{
	return (ADAPT_SFS_ERROR_E)SFS_RenameFile(sour,dest,(SFS_OVERLAPPED_T    *)overlapped_ptr);
}


#endif
