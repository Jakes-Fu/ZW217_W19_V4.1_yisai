/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_file.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-30  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-30      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_FILE_H_
#define _MAIL_FILE_H_

#include "mail_ret.h"
#include "mail_sig.h"


#define MAIL_REMAIN_SPACE         1*1024*1024


#ifdef MAIL_SUPPORT_CMPOP

/*==========================================================
 * Function 	: mailCreateRecvTempFile
 * Author		: sunguochen      
 * Description : ��ָ��Ŀ¼������ʱ�ļ�
                      ���ֻ�����uidl������-�����������
                      ����϶����쳣
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailCreateRecvTempFile(char *filePath, long size, char* uidl);

#endif

/*==========================================================
 * Function 	: generateFileName
 * Author		: sunguochen      
 * Description : ����ָ�����͵��ļ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void generateFileName(char *str,  const char *extension);
/*==========================================================
 * Function 	: mailCreateTempFile
 * Author		: sunguochen      
 * Description : ��ָ��Ŀ¼������ʱ�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailCreateTempFile(char *filePath, long size);



/*==========================================================
 * Function 	: mail_FileOperationStartBuf
 * Author		: sunguochen      
 * Description : ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_uint32 mail_FileOperationStartBuf(char* file_name);


/*==========================================================
 * Function 	: 
 * Author		:       
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: 
 ==========================================================*/
comm_uint32 mail_FileOperationOpen(char* file_name);


/*==========================================================
 * Function 	: mail_FileOperationWriteBuf
 * Author		: sunguochen      
 * Description : д�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_FileOperationWriteBuf(void* session, comm_uint32 file_handle, void *buffer, 
                                                            comm_uint32 bufSize, comm_boolean moreData,
                                                            EStateMachine dst, int signum);



/*==========================================================
 * Function 	: mail_FileOperationReadBuf
 * Author		: sunguochen      
 * Description : ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/26/2010 
 ==========================================================*/
void mail_FileOperationReadBuf(void* session, comm_uint32 file_handle, void *buffer, 
                                                           comm_uint32 start_pos, comm_uint32 bufSize,
                                                           comm_uint32* read_Size);



/*==========================================================
 * Function 	: mail_FileOperationStopBuf
 * Author		: sunguochen      
 * Description : �ر��ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_FileOperationStopBuf(comm_uint32* file_handle);



/*==========================================================
 * Function 	: mail_fileGetSize
 * Author		: sunguochen      
 * Description : ����ļ���С
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_boolean mail_fileGetSize(char* file_name, comm_uint32* file_size );



/*==========================================================
 * Function 	: mail_FileFreeSpace
 * Author		: sunguochen      
 * Description : ���ʣ��洢�ռ�                     
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/11/2010 
 ==========================================================*/
comm_uint32 mail_FileFreeSpace(void);


/*==========================================================
 * Function 	: mail_FileDelete
 * Author		: sunguochen      
 * Description : ɾ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/27/2010 
 ==========================================================*/
void mail_FileDelete(char* file_name);



/*==========================================================
 * Function 	: mail_translateFileError
 * Author		: sunguochen      
 * Description : ת���ļ���������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
mail_retval mail_translateFileError(long fileError);

#ifdef MAIL_FLDR_MANAGER
/*==========================================================
 * Function 	: mail_FileOperationRead
 * Author		: sunguochen      
 * Description : ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/26/2010 
 ==========================================================*/
mail_retval mail_createFolder(const char* folder);
/*==========================================================
 * Function 	: mail_FileOperationWrite
 * Author		: lixu      
 * Description : д�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_FileOperationWrite(comm_uint32 file_handle, void *buffer, 
                                        comm_uint32 bufSize,int32 start_pos);

/*==========================================================
 * Function 	: mail_FileOperationSeek
 * Author		: lixu      
 * Description : д�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
 comm_int32 mail_FileOperationSeek(comm_uint32 file_handle, int32 startPos,
                                                                    uint32 origin);

/*==========================================================
 * Function 	: mail_FileOperationRead
 * Author		: sunguochen      
 * Description : ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/26/2010 
 ==========================================================*/
mail_retval mail_FileOperationRead(comm_uint32 file_handle, void *buffer, 
                                                           int32 start_pos, comm_uint32 bufSize);
/*==========================================================
 * Function 	: mail_GetFileSizeByPath
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByPath(const char *path_name);
/*==========================================================
 * Function 	: mail_GetFileSizeByFileName
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByFileName(const char *file_name);
/*==========================================================
 * Function 	: mail_FileOperationRead
 * Author		: sunguochen      
 * Description : ���ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/26/2010 
 ==========================================================*/
mail_retval mail_createFolder(const char* folder);
/*==========================================================
 * Function 	: mail_FileOperationStop
 * Author		: sunguochen      
 * Description : �ر��ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_FileOperationStop(comm_uint32* file_handle);
/*==========================================================
 * Function 	: mail_SetFileSize
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_SetFileSize(comm_uint32 file_handle,comm_uint32 size);
/*==========================================================
 * Function 	: mail_RenameFileByWchar
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
 mail_retval mail_RenameFileByWchar(const wchar *srcName, wchar *dstName);
/*==========================================================
 * Function 	: mail_RenameFile
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_RenameFile(const char *oldName, const char *newName);

 /*==========================================================
 * Function 	: mail_FileNameCmp
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_boolean mail_FileNameCmp(const char *FileName1,const char *FileName2);
 /*==========================================================
 * Function 	: mail_GetFileSizeByHandle
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByHandle(MMIFILE_HANDLE file_handle);
 /*==========================================================
 * Function 	: mail_GetFileNumOfDir
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
 comm_int32 mail_GetFileNumOfDir(const char *pathName);
 /*==========================================================
 * Function 	: mail_RemoveFile
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
 comm_uint32 mail_RemoveFile(const char *FileName);

 /*==========================================================
 * Function 	: mail_sentMailWriteFile
 * Author		: lixu      
 * Description :  �����ʼ�ʱ���������ַ�д���ļ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  
 ==========================================================*/
 comm_boolean mail_sentMailWriteFile(const char* file_name,void* write_buf,comm_uint32 buffer_size);
/*==========================================================
 * Function 	: mail_GetFilePathByFileName
 * Author		: lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_GetFilePathByFileName(const char *file_name,char* path);
#endif


/*==========================================================
 * Function 	: mail_CleanDir
 * Author		: sunguochen      
 * Description : ���Ŀ¼
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
void mail_CleanDir(comm_uint16* directory);



/*==========================================================
 * Function 	: mail_GetDirFileNum
 * Author		: sunguochen      
 * Description : ���Ŀ¼
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
int mail_GetDirFileNum(comm_uint16* directory);




#endif 
