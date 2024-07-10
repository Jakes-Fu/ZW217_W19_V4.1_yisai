/******************************************************************************
 ** File Name:      sfs_simu.c                                                *
 ** Author:                                                                   *
 ** DATE:           08/07/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of FFS.               *
 **                 (both FATand EFS File System)                             *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/07/2006   liqing.peng          Create.                                 *
 ******************************************************************************/



/**---------------------------------------------------------------------------*
 **                         include files                                     *
 **---------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "sfs.h"

#if 0

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**---------------------------------------------------------------------------*
 **                         Function definition                               *
 **---------------------------------------------------------------------------*/
void SFS_Init(void)
{
    return;
}


/**************/
//device operation
/**************/
SFS_ERROR_E SFS_RegisterDevice(
                               const uint16         *dev_name,
                               SFS_DEVICE_FORMAT_E  *format_ptr
                               )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_UnRegisterDevice(
                                 const uint16       *dev_name
                                 )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_Format(
                       const uint16         *dev_name,
                       SFS_DEVICE_FORMAT_E  format_type,
                       SFS_OVERLAPPED_T     *overlapped_ptr
                       )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_GetDeviceStatus(
                               const uint16     * dev_name
                               )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_GetDeviceFreeSpace(
                                   const uint16     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
    *high_word = 0;
    *low_word = 345;

    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_GetDeviceUsedSpace(
                                   const uint16     *device_name,
                                   uint32           *high_word,
                                   uint32           *low_word
                                   )
{
    *high_word = 0;
    *low_word = 2048;

    return SFS_ERROR_NONE;
}



/**************/
//file and dir operation
/**************/
SFS_HANDLE SFS_CreateFile(
                        const uint16    *file_name, 
                        uint32          access_mode,
                        uint32          share_mode,	//Must be NULL,File System have not realize it 
                        uint32          file_attri		//Must be NULL,File System have not realize it 
                        )
{
    return 1;
}


SFS_ERROR_E SFS_DeleteFile(
                           const uint16         *name,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_RenameFile(
                           const uint16         *sour,
                           const uint16         *dest,
                           SFS_OVERLAPPED_T     *overlapped_ptr
                           )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_CreateDirectory(
                                const uint16    *path
                                )
{
    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_DeleteDirectory(
                                const uint16    *path
                                )
{
    return SFS_ERROR_NONE;
}


SFS_HANDLE SFS_FindFirstFile(
                             const uint16       *find, 
                             SFS_FIND_DATA_T    *fd
                             )
{
    fd->attr = SFS_ATTR_ARCH;
    fd->length = 1024*2;

    fd->name[0] = 0x3100;
    fd->name[1] = 0x3200;
    fd->name[2] = 0x3300;
    fd->name[3] = 0x3100;
    fd->name[4] = 0x3200;
    fd->name[5] = 0x3300;
    fd->name[6] = 0x3100;
    fd->name[7] = 0x3200;
    fd->name[8] = 0x3300;
    fd->name[9] = 0x3100;
    fd->name[10] = 0x0000; 

    return 1;
}


SFS_ERROR_E SFS_FindNextFile(
                             SFS_HANDLE         sfs_handle, 
                             SFS_FIND_DATA_T    *fd
                             )
{
    static uint32  i = 0;

    fd->attr = SFS_ATTR_ARCH;
    fd->length = 1024*2;

    switch(i%3)
    {
    case 0:
        fd->name[0] = 0x3100;
        fd->name[1] = 0x3200;
        fd->name[2] = 0x3300;
        fd->name[3] = 0x3400;
        fd->name[4] = 0x0000;
        break;

    case 1:
        fd->name[0] = 0x3500;
        fd->name[1] = 0x3600;
        fd->name[2] = 0x3700;
        fd->name[3] = 0x3800;
        fd->name[4] = 0x0000;
        break;

    case 2:
        fd->name[0] = 0x3900;
        fd->name[1] = 0x4000;
        fd->name[2] = 0x4100;
        fd->name[3] = 0x4200;
        fd->name[4] = 0x0000;
        break;

    default:
        break;
    }

    i++;

    return 1;
}


SFS_ERROR_E SFS_FindClose(
                          SFS_HANDLE    sfs_handle
                          )
{
    return SFS_ERROR_NONE;
}




/**************/
//file internal operation
/**************/
SFS_ERROR_E SFS_GetFileSize(
                            SFS_HANDLE  sfs_handle, 
                            uint32      *file_size
                            )
{
    *file_size = 1024*100;

    return SFS_ERROR_NONE;
}


SFS_ERROR_E SFS_SetFileSize(
                            SFS_HANDLE      sfs_handle, 
                            uint32          size
                            )
{
    return SFS_ERROR_NONE;
}



SFS_ERROR_E SFS_ReadFile(
	                    SFS_HANDLE          handle,
	                    void                *buffer,
	                    uint32              bytes_to_read,
	                    uint32              *bytes_read,
	                    SFS_OVERLAPPED_T    *overlapped
                        )
{
    return SFS_ERROR_NONE;
}



SFS_ERROR_E SFS_WriteFile(
	                    SFS_HANDLE          handle,
	                    const void          *buffer,
	                    uint32              bytes_to_write,
	                    uint32              bytes_written,
	                    SFS_OVERLAPPED_T    *overlapped
                        )
{
    return SFS_ERROR_NONE;
}


uint32 SFS_SetFilePointer(
	                    SFS_HANDLE      handle,
	                    int32           offset,
	                    uint32          origin
                        )
{
    return 1;
}


int32 SFS_GetFilePointer(
	                    SFS_HANDLE      handle,
	                    uint32          origin
                        )
{
    return 1;
}


SFS_ERROR_E SFS_CloseFile(
                          SFS_HANDLE    handle
                          )
{
    return SFS_ERROR_NONE;
}

SFS_ERROR_E SFS_GetDevnameFromPath(/*in*/uint16* pathname,/*out*/uint16* dev_name)
{	
	return SFS_ERROR_NONE;
}

PUBLIC SFS_ERROR_E SFS_ConvertStrmodeToBinmode(const uint8 *mode,uint32 *bin_mode)
{
    if( NULL == mode )
        return SFS_ERROR_INVALID_PARAM;
       
        
    *bin_mode = 0;    
    // Only parse the first mode
    switch( *mode++ )
    {
    case 'R':    
    case 'r':
        if( '+' == *mode ) // "r+"
        {
            // Opens for both reading and writing. (The file must exist.) 
            *bin_mode = SFS_MODE_READ | SFS_MODE_WRITE |SFS_MODE_OPEN_EXISTING;
        }
        else if(NULL == *mode)// "r"
        {
            // Opens the exist file for reading. If the file does not exist
            // or cannot be found, the fopen call fails. 
            *bin_mode = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
        }
        else
        {
        	return SFS_ERROR_INVALID_PARAM;
        }
    break;
    case 'W':
    case 'w':
        if( '+' == *mode ) // "w+"
        {
            // Opens an empty file for both reading and writing. 
            // If the given file exists, its contents are destroyed. 
            *bin_mode = SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS;
        }
        else if(NULL == *mode)// "w"
        {
            // Opens an empty file for writing. 
            // If the given file exists, its contents are destroyed.
            *bin_mode = SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS;
        }    
        else
        {
        	return SFS_ERROR_INVALID_PARAM;
        }
    break;
    case 'A':
    case 'a':
        if( '+' == *mode ) // "a+"
        {
            // Opens for reading and appending; the appending operation 
            // includes the removal of the EOF marker before new data is written
            // to the file and the EOF marker is restored after writing is complete; 
            // creates the file first if it doesn¡¯t exist. 
            *bin_mode |= SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_APPEND;
        }
        else if(NULL == *mode)// "a"
        {
            // Opens for writing at the end of 
            // the file (appending) without removing
            // the EOF marker before writing new data to the file; 
            // creates the file first if it doesn¡¯t exist
            *bin_mode = SFS_MODE_WRITE | SFS_MODE_APPEND;
        }
        else
        {
        	return SFS_ERROR_INVALID_PARAM;
        }
    break;
    default:
    	return SFS_ERROR_INVALID_PARAM;
    break;
    }
    return SFS_ERROR_NONE;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif