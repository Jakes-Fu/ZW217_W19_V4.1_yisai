/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_file.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _COMM_FILE_H_
#define _COMM_FILE_H_


#include "mmifmm_export.h"


#define COMM_MEMORY_REMAIN_SIZE 1*1024*1024

//�ļ����
#define MAIL_FileHandle MMIFILE_HANDLE

//�ļ�����
#define MAIL_FileError MMIFILE_ERROR_E

#define MAIL_FILE_NOERROR  SFS_NO_ERROR

#define MAIL_INVALID_HANDLE SFS_INVALID_HANDLE

//���ļ�
#define MAIL_FileOpen MMIAPIFMM_CreateFile

//���ļ�
#define MAIL_FileRead MMIAPIFMM_ReadFile

//д�ļ�
#define MAIL_FileWrite MMIAPIFMM_WriteFile

//�ļ��Ƿ����
#define MAIL_FileIsExist MMIAPIFMM_IsFileExist

//�ļ���С
#define MAIL_FileGetSize MMIAPIFMM_GetFileSize

//�ر��ļ�
#define MAIL_FileClose MMIAPIFMM_CloseFile

/*==========================================================
 * Function 	: comm_GetMusicRoot
 * Author		: sunguochen      
 * Description : ����������������ظ�Ŀ¼
                      ����п�������sd���ϣ�������ڱ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_boolean comm_DownloadRootDir(char *path);


/*==========================================================
 * Function 	: comm_FreeSpace
 * Author		: sunguochen      
 * Description : ���ʣ��洢�ռ�
                      ������������ȡ��ʣ��ռ�
                      �����ȡ����ʣ��ռ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_uint32 comm_DownloadFreeSpace(void);



#endif
