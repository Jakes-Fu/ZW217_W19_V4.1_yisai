#ifndef _IMG_DEC_FILE_H
#define _IMG_DEC_FILE_H

#include "img_dec_interface.h"
#include "os_api.h"
#include "sfs.h"

/*origin SFS_SEEK_BEGIN ,SFS_SEEK_CUR,SFS_SEEK_END*/

#define  IMG_DEC_SEEK_BEGIN SFS_SEEK_BEGIN
#define  IMG_DEC_SEEK_CUR SFS_SEEK_CUR
#define  IMG_DEC_SEEK_END  SFS_SEEK_END

typedef SFS_HANDLE IMG_DEC_FILE_HANDLE;


//filename
IMG_DEC_FILE_HANDLE IMG_DEC_File_Open(uint16 * filename);

//handle: file handle
IMG_DEC_RET_E IMG_DEC_Read_Data(IMG_DEC_FILE_HANDLE handle, uint8 *data_ptr, uint32 read_count, uint32 *read_count_ptr);

//handle: file handle
uint32 IMG_DEC_File_Get_Size(IMG_DEC_FILE_HANDLE  handle);

//handle: file handle
IMG_DEC_RET_E IMG_DEC_File_Close(IMG_DEC_FILE_HANDLE  handle);


//origin: IMG_DEC_SEEK_BEGIN, IMG_DEC_SEEK_CUR,  IMG_DEC_SEEK_END
//offset: offset from origin
//handle: file handle
IMG_DEC_RET_E IMG_DEC_Seek(IMG_DEC_FILE_HANDLE handle, __int64 offset ,uint32 origin);




#endif
