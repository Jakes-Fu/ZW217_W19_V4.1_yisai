/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_file.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _COMM_FILE_H_
#define _COMM_FILE_H_


#include "mmifmm_export.h"


#define COMM_MEMORY_REMAIN_SIZE 1*1024*1024

//文件句柄
#define MAIL_FileHandle MMIFILE_HANDLE

//文件错误
#define MAIL_FileError MMIFILE_ERROR_E

#define MAIL_FILE_NOERROR  SFS_NO_ERROR

#define MAIL_INVALID_HANDLE SFS_INVALID_HANDLE

//打开文件
#define MAIL_FileOpen MMIAPIFMM_CreateFile

//读文件
#define MAIL_FileRead MMIAPIFMM_ReadFile

//写文件
#define MAIL_FileWrite MMIAPIFMM_WriteFile

//文件是否存在
#define MAIL_FileIsExist MMIAPIFMM_IsFileExist

//文件大小
#define MAIL_FileGetSize MMIAPIFMM_GetFileSize

//关闭文件
#define MAIL_FileClose MMIAPIFMM_CloseFile

/*==========================================================
 * Function 	: comm_GetMusicRoot
 * Author		: sunguochen      
 * Description : 获得音乐随身听下载根目录
                      如果有卡，放在sd卡上，否则放在本机
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_boolean comm_DownloadRootDir(char *path);


/*==========================================================
 * Function 	: comm_FreeSpace
 * Author		: sunguochen      
 * Description : 获得剩余存储空间
                      如果卡存在则获取卡剩余空间
                      否则获取本机剩余空间
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_uint32 comm_DownloadFreeSpace(void);



#endif
