/****************************************************************************
** File Name:      mail_file.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MAIL_FILE_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mail_cfg.h"
#include "mail_mem.h"
#include "mail_ret.h"
#include "comm_file.h"
#include "mail_pop3.h"
#include "mail_network.h"
#include "mail_conttypes.h"
#include "mail_sig.h"
#include "mmifmm_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef E_FREE
#undef E_FREE
#endif
#define E_FREE(ptr) \
        do\
        {\
            if (PNULL != (ptr)) \
            {\
                SCI_FREE((ptr));\
                (ptr) = NULL;\
            }\
        }while(0)
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * Function     : createFilePath
 * Author	    : sunguochen      
 * Description : 生成完整目录
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
LOCAL void createFilePath( char *filePath, const char *fileName);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*==========================================================
 * Function     : generateFileName
 * Author	    : sunguochen      
 * Description : 生成指定类型的文件名
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
void generateFileName(char *str,  const char *extension)
{
    if (str == NULL)
    {
        return;
    }
    else if (NULL != extension && strlen(extension) <= 3)
    {   
        comm_uint32 currentTime = 0;

        currentTime = comm_CurrentTime();
        
        sprintf(str, "%lx.%s", (long)currentTime, extension);
    }
    else
    {   
        *str = '\0';
    }
}  

/*==========================================================
 * Function     : createFilePath
 * Author	    : sunguochen      
 * Description : 生成完整目录
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
LOCAL void createFilePath( char *filePath, const char *fileName)
{
    if(NULL == filePath || NULL == fileName)
    {
        return;
    }

    strcpy(filePath, MAIL_FM_FOLDER); 

    strcat(filePath, fileName);
}  

/*==========================================================
 * Function     : mailCreateTempFile
 * Author	    : sunguochen      
 * Description : 在指定目录生成临时文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailCreateTempFile(char *filePath, long size)
{
    const char *extension = MAIL_TEMP_FILE_EXTENSION;
    char name[8 + 3 + 1 + 1] = {0};
    comm_int32 res = 0;
    char index = 1;
    comm_boolean unique = FALSE;
    comm_int8 fileNameLen;
    int full_file_len = 0;
    wchar* full_filename = NULL;   

    //生成临时文件
    generateFileName(name, extension);
    createFilePath(filePath, name);

    full_file_len = strlen(filePath);
    full_filename = E_ALLOC(2*(full_file_len + 1));

    comm_str2wstr((comm_uint16 *)full_filename, (char *) filePath);
	while (!unique)
    {        
    	res = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_WRITE | SFS_MODE_CREATE_NEW, NULL, NULL);
    	if (res > 0)
        {
            if(size)
            {
                //设置文件大小
                //MMIAPIFMM_SetFileSize(res, size);
            }
        	MMIAPIFMM_CloseFile(res);
        	unique = TRUE;
        }
    	else
        {
        	fileNameLen = (comm_int8) (strlen(filePath) - 5);
        	if (fileNameLen < 0)
            {                
                generateFileName(filePath, extension);
                createFilePath(filePath, name);
            } 
        	else
            {                
            	filePath[fileNameLen] = (char)(index + '0');
            	index++;
            }
            memset(full_filename, 0, 2*(full_file_len + 1));
            comm_str2wstr((comm_uint16 *)full_filename, (char *) filePath);

        	if ( index >= 10)
            {
/*
                if(NULL != full_filename)
                {
                    E_FREE(full_filename);
                    full_filename = NULL;
                }
*/
                return  FALSE;
                //超过10次，不再生成临时文件
            }    
        } 
    }
    if(full_filename)
    {
        E_FREE(full_filename);
        full_filename = NULL;
    }

	return TRUE;
}

#ifdef MAIL_SUPPORT_CMPOP

/*==========================================================
 * Function     : mailCreateRecvTempFile
 * Author	    : sunguochen      
 * Description : 在指定目录生成临时文件
                      在手机上用uidl做名字-不会出现重名
                      否则肯定有异常
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailCreateRecvTempFile(char *filePath, long size, char* uidl)
{
	const char *extension = MAIL_TEMP_FILE_EXTENSION;
       char name[MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1] = {0};//changed by liangshun for MS00217735 20110105
	comm_int32 res = 0;
	comm_boolean unique = FALSE;
    int full_file_len = 0;
    wchar* full_filename = NULL;
    if(NULL == uidl || 0 == *uidl)
    {
      return mailCreateTempFile(filePath, size);
    }

    //生成临时文件
	sprintf(name, "%s.%s", uidl, extension);
    createFilePath(filePath, name);
    full_file_len = strlen(filePath);
        if(full_file_len > MAIL_MAX_COMPL_FILE_NAME_LENGTH)
        {
            return FALSE;
        }
    full_filename = E_ALLOC(2*(full_file_len + 1));
    comm_str2wstr((comm_uint16 *)full_filename, (char *) filePath);

	while (!unique)
    {        
    	res = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_WRITE | SFS_MODE_CREATE_NEW, NULL, NULL);
    	if (res > 0)
        {
        	MMIAPIFMM_CloseFile(res);
        	unique = TRUE;
        }
    	else
        {
        	MMIAPIFMM_DeleteFile(full_filename, NULL);
        } 
    }
    if(full_filename)
    {
        E_FREE(full_filename);
        full_filename = NULL;
    }

	return TRUE;
}

#endif

/*==========================================================
 * Function     : mail_FileOperationStartBuf
 * Author	    : sunguochen      
 * Description : 打开文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_uint32 mail_FileOperationStartBuf(char* file_name)
{
    int full_file_len = 0;
    wchar* full_filename = NULL;   
    comm_int32 res = 0;

    if(NULL == file_name)
    {
        return 0;
    }

    full_file_len = strlen(file_name);
    full_filename = E_ALLOC(2*(full_file_len + 1));

    comm_str2wstr((comm_uint16 *)full_filename, (char *) file_name);
    res = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS, NULL, NULL);
    E_FREE(full_filename);

    return res;
}

/*==========================================================
 * Function     : 
 * Author	    :       
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: 
 ==========================================================*/
comm_uint32 mail_FileOperationOpen(char* file_name)
{
    int         full_file_len = 0;
    wchar*   full_filename = NULL;   
    comm_int32 res = 0;

    if(NULL == file_name)
    {
        return 0;
    }

    full_file_len = strlen(file_name);

    full_filename = E_ALLOC(2*(full_file_len + 1));

    comm_str2wstr((comm_uint16 *)full_filename, (char *) file_name);

    res = MMIAPIFMM_CreateFile(full_filename, SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING, NULL, NULL);

    E_FREE(full_filename);

    return res;
}

/*==========================================================
 * Function     : mail_FileOperationWriteBuf
 * Author	    : sunguochen      
 * Description : 写文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_FileOperationWriteBuf(void* session, comm_uint32 file_handle, void *buffer, 
                                                            comm_uint32 bufSize, comm_boolean moreData,
                                                            EStateMachine dst, int signum)
{
    comm_uint32 buf_write = 0;
    int result;

    MMIFILE_ERROR_E error;

    if(!file_handle || !buffer)
    {
        return;
    }

    //COMM_TRACE:"Mail: mail_FileOperationWriteBuf: buffer %s \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_340_112_2_18_2_47_41_34,(uint8*)"s", buffer);

    error = MMIAPIFMM_WriteFile(file_handle, buffer, bufSize, &buf_write, NULL);

    if((SFS_ERROR_NONE != error) || (bufSize != buf_write))
    {
        //just for test .write error
        result = MAIL_RESULT_FILE_ERROR;
        //COMM_ASSERT(0);
    }
    else
    {
        //write success
        result = MAIL_OK;

    }
    //COMM_TRACE:"Mail: mail_FileOperationWriteBuf: written %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_356_112_2_18_2_47_41_35,(uint8*)"d", buf_write);

    E_SIGNAL_SENDTO_IP(dst, signum, (int)session,(void *)result);

    return;
}

/*==========================================================
 * Function     : mail_FileOperationReadBuf
 * Author	    : sunguochen      
 * Description : 读文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/26/2010 
 ==========================================================*/
void mail_FileOperationReadBuf(void* session, comm_uint32 file_handle, void *buffer, 
                                                           comm_uint32 start_pos, comm_uint32 bufSize,
                                                           comm_uint32* read_Size)
{
    MMIFILE_ERROR_E error = SFS_NO_ERROR;

    if(!file_handle || !buffer)
    {
        return ;
    }

    MMIAPIFMM_SetFilePointer(file_handle, start_pos, SFS_SEEK_BEGIN);

    error = MMIAPIFMM_ReadFile(file_handle, buffer, bufSize, read_Size, NULL);

    if((SFS_ERROR_NONE != error) || (bufSize != *read_Size))
    {
        //just for test .write error
        //COMM_ASSERT(0);
    }
    else
    {
        //write success
        
    }
}

/*==========================================================
 * Function     : mail_FileOperationStopBuf
 * Author	    : sunguochen      
 * Description : 关闭文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_FileOperationStopBuf(comm_uint32* file_handle)
{

    if(!file_handle || !*file_handle)
    {
        return ;
    }

    MMIAPIFMM_CloseFile(*file_handle);

    *file_handle = 0;
}

/*==========================================================
 * Function     : mail_fileGetSize
 * Author	    : sunguochen      
 * Description : 获得文件大小
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_boolean mail_fileGetSize(char* file_name, comm_uint32* file_size )
{
    MMIFILE_HANDLE file_handle;

    file_handle = mail_FileOperationStartBuf(file_name);

    if(!file_handle)
    {
        return FALSE;
    }

    *file_size = MMIAPIFMM_GetFileSize(file_handle);

    mail_FileOperationStopBuf(&file_handle);

    return TRUE;
}



/*==========================================================
 * Function     : mail_FileFreeSpace
 * Author	    : sunguochen      
 * Description : 获得剩余存储空间                     
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/11/2010 
 ==========================================================*/
comm_uint32 mail_FileFreeSpace(void)
{
    uint32 free_space_high = 0;
    uint32 free_space_low = 0;    

    //get mobile free memory
    MMIAPIFMM_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), &free_space_high, &free_space_low);

    //leave 2M free space 
    if(free_space_low > COMM_MEMORY_REMAIN_SIZE)
    {
        free_space_low -= COMM_MEMORY_REMAIN_SIZE;
    }
    else
    {
        free_space_low = 0;
    }

    return free_space_low;
}


/*==========================================================
 * Function     : mail_FileDelete
 * Author	    : sunguochen      
 * Description : 删除文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/24/2010 
 ==========================================================*/
void mail_FileDelete(char* file_name)
{
    int         full_file_len = 0;
    wchar*   full_filename = NULL;   
    comm_int32 res = 0;
    
    if((file_name == NULL) || (*file_name == 0))
    {
        return;
    }

    full_file_len = strlen(file_name);

    full_filename = E_ALLOC(2*(full_file_len + 1));

    comm_str2wstr((comm_uint16 *)full_filename, (char *) file_name);

    res = MMIAPIFMM_DeleteFile(full_filename, NULL);

    E_FREE(full_filename);
}

/*
void mail_CleanDirectory(const char* directory)
{
    int type;
    int index;
    int returnCode;
	int dirHandle;
    long size;
    char fileName[MAIL_MAX_FILE_NAME_LENGTH + 1]; 
    char *filePath = NULL;

    filePath = E_ALLOC(strlen(directory) + 1 + MAIL_MAX_FILE_NAME_LENGTH);
	dirHandle = MSF_FILE_OPEN_DIR(MSF_MODID_MAIL, directory);

	if (dirHandle <= 0)
    {
    	MSF_LOG_MSG(( MSF_LOG_DETAIL_MEDIUM, MSF_MODID_MAIL,
            "%s(%d): Directory open failed\n",
        	__FILE__, __LINE__));
    	return;
    }
    index = 0;
    do
    {
        returnCode = MSF_FILE_READ_DIR_EX(dirHandle, index, fileName, 
        	MAIL_MAX_FILE_NAME_LENGTH + 1, &type, &size);

        if (returnCode == MSF_FILE_OK)
        {
            if (type == MSF_FILE_FILETYPE)
            {
                sprintf(filePath,"%s%s", directory, fileName);
                (void)MSF_FILE_REMOVE(filePath);
            }
        }
        else if (returnCode != MSF_FILE_ERROR_INVALID) {
            
        }
    	index++;
    } while (returnCode == MSF_FILE_OK);

    (void)MSF_FILE_CLOSE_DIR(dirHandle);
    E_FREE(filePath);
}  

MailResult mail_cleanAllMailDirectories( void )
{    
    
    MSF_FILE_CLOSE_ALL(MSF_MODID_MAIL);

    
	if(strcmp(MAIL_TMP_FOLDER, MAIL_FM_FOLDER) != 0)
    {
    	mail_CleanDirectory(MAIL_TMP_FOLDER);
    }
    
    mail_CleanDirectory(MAIL_FM_FOLDER);
    
    mail_CleanDirectory(MAIL_FOLDER);
     
    mail_CleanDirectory(MAIL_BLOCKED_FOLDER); 
    
    return MAIL_RES_OK;
}  

mail_retval mail_createFolder(const char* folder)
{
    int msfResult;

            msfResult = MSF_FILE_MKDIR(folder);

	if (msfResult == MSF_FILE_OK || msfResult == MSF_FILE_ERROR_EXIST)
    {
    	return MAIL_OK;
    }
	else
    {
                MSF_LOG_MSG(( MSF_LOG_DETAIL_MEDIUM, MSF_MODID_MAIL,
                              "%s(%d): Unable to create MAIL folder, errorcode %d\n",
                              __FILE__, __LINE__, msfResult));
        return MAIL_FM_FILE_ERROR;
    }
}  
*/

/*==========================================================
 * Function     : mail_translateFileError
 * Author	    : sunguochen      
 * Description : 转化文件操作错误
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/1/2010 
 ==========================================================*/
mail_retval mail_translateFileError(long fileError)
{
    mail_retval ret = fileError;    
/*    
    switch (fileError)
    {
    case MSF_FILE_OK :
        ret = MAIL_OK;
        break;
    case MSF_FILE_ERROR_DELAYED :
        ret = MAIL_ASYNC_DELAYED;
        break;
    case MSF_FILE_ERROR_PATH :
        ret = MAIL_RESULT_FILE_NOT_FOUND;
        break;
    case MSF_FILE_ERROR_SIZE :    
        ret = MAIL_FILE_ERROR_SIZE;
        break;
    case MSF_FILE_ERROR_FULL :    
        ret = MAIL_FILE_ERROR_FULL;
        break;
    case MSF_FILE_ERROR_EOF :
        ret = MAIL_ASYNC_EOF;
        break;
    default :

        break;
    } 
*/
    return ret;
}

#ifdef MAIL_FLDR_MANAGER
/*==========================================================
 * Function     : mail_FileOperationStop
 * Author	    : lixu      
 * Description : 关闭文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_FileOperationStop(comm_uint32* file_handle)
{
    mail_retval ret = MAIL_OK;
    
    if(!file_handle || !*file_handle)
    {
        return MAIL_FM_FILE_ERROR;
    }

    if(!MMIAPIFMM_CloseFile(*file_handle))
    {
        ret = MAIL_FM_FILE_ERROR;
    }
    *file_handle = 0;

    return ret;
}

/*==========================================================
 * Function     : mail_FileOperationRead
 * Author	    : lixu      
 * Description : 读文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  
 ==========================================================*/
mail_retval mail_createFolder(const char* folder)
{
    mail_retval  ret = MAIL_OK; 
    int path_len =0;
    int full_path_len = 0;
    wchar* folder_path = NULL;

    if(NULL == folder)
    {
        ret = MAIL_FM_FOLDER_NOT_FOUND;
        return ret;
    }

    path_len = strlen(folder);

    folder_path = E_ALLOC(2*(path_len + 1));
    memset(folder_path,0,sizeof(2*(path_len + 1)));

    comm_str2wstr((comm_uint16 *)folder_path, (char *) folder);
    //MMIAPICOM_StrToWstr(folder, folder_path);

    full_path_len = MMIAPICOM_Wstrlen(folder_path);

    if (full_path_len <= 0)
    {
        //SCI_TRACE_LOW:"MMIMail mail_createFolder full_path_len <= 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_690_112_2_18_2_47_42_36,(uint8*)"");
        return MAIL_ERROR;
    }
    
    if(!MMIAPIFMM_IsFolderExist(folder_path, full_path_len))
    {
        if(!MMIAPIFMM_CreateDir(folder_path, full_path_len))
        {  
            ret = MAIL_FM_FOLDER_NOT_FOUND;                
        }
    }
    E_FREE(folder_path);

    return ret;
}

/*==========================================================
 * Function     : mail_FileOperationRead
 * Author	    : lixu      
 * Description : 读文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  
 ==========================================================*/
mail_retval mail_FileOperationRead(comm_uint32 file_handle, void *buffer, 
                                                           int32 start_pos, comm_uint32 bufSize)
{
    mail_retval res = MAIL_OK;
    MMIFILE_ERROR_E error = SFS_ERROR_NONE;
    comm_uint32 read_Size = 0;
    comm_uint32 read_len = 0;
    comm_uint32 totLen = 0;
    comm_uint32 read_pos = 0;
    
    if(!file_handle || !buffer)
    {
        return MAIL_FM_FILE_ERROR;
    }

    read_pos = start_pos;
    totLen = bufSize;
    
    while(0 != totLen)
    {
        if(MMIAPIFMM_SetFilePointer(file_handle,(comm_int32)read_pos,SFS_SEEK_BEGIN))
        {
            //COMM_TRACE:"mail_fldr mail_FileOperationRead set pointer fail\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_736_112_2_18_2_47_42_37,(uint8*)"");
            return MAIL_FM_FILE_ERROR;

        }
        error = MMIAPIFMM_ReadFile(file_handle, buffer, bufSize, &read_Size, NULL);
        if((SFS_ERROR_NONE != error)||(0 == read_Size))
        {
            //COMM_TRACE:"mail_fldr mail_FileOperationRead error=%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_743_112_2_18_2_47_42_38,(uint8*)"d",error);
            //COMM_TRACE:"mail_fldr mail_FileOperationRead read_Size=%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_744_112_2_18_2_47_42_39,(uint8*)"d",read_Size);
            //COMM_TRACE:"mail_fldr mail_FileOperationRead bufSize=%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_745_112_2_18_2_47_42_40,(uint8*)"d",bufSize);
            return MAIL_FM_FILE_ERROR;
        }
        read_len = read_len + read_Size;
        read_pos = read_pos + read_len;
        totLen = bufSize - read_len;
    }

    //COMM_TRACE:"mail_fldr mail_FileOperationRead  bufsize=%d,read_size=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_753_112_2_18_2_47_42_41,(uint8*)"dd",bufSize,read_Size);
    
    if (bufSize != read_len)
    {
        //COMM_TRACE:"mail_fldr mail_FileOperationRead error=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_757_112_2_18_2_47_42_42,(uint8*)"d",error);
        //COMM_TRACE:"mail_fldr mail_FileOperationRead read_Size=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_758_112_2_18_2_47_42_43,(uint8*)"d",read_Size);
        //COMM_TRACE:"mail_fldr mail_FileOperationRead bufSize=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_759_112_2_18_2_47_42_44,(uint8*)"d",bufSize);
        return MAIL_FM_FILE_ERROR;
    }

    return res;
}

/*==========================================================
 * Function     : mail_FileOperationWrite
 * Author	    : lixu      
 * Description : 写文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  
 ==========================================================*/
comm_int32 mail_FileOperationWrite(comm_uint32 file_handle, void *buffer, 
                                        comm_uint32 bufSize,int32 start_pos)
{
    comm_uint32 buf_write = 0;
    comm_uint32 totLen = 0;
    comm_uint32 write_len = 0;
    comm_uint32  write_pos = 0;

    MMIFILE_ERROR_E error = SFS_ERROR_NONE;
    
    if(!file_handle || !buffer)
    {
        return -1;
    }

    write_pos = start_pos;
    totLen = bufSize;
    while(0 != totLen)
    {
        if(MMIAPIFMM_SetFilePointer(file_handle, (comm_int32)write_pos,SFS_SEEK_BEGIN))
        {
            return -1;
        }
        error = MMIAPIFMM_WriteFile(file_handle, buffer, bufSize, &buf_write, NULL);
        if((SFS_ERROR_NONE != error)||(0 == buf_write))
        {
            return -1;
        }
        write_len = write_len + buf_write;
        write_pos = write_pos + write_len;
        totLen = bufSize - write_len;
    }

    //COMM_TRACE:"mail_fldr mail_FileOperationWrite bufsize=%d,buf_write=%d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_807_112_2_18_2_47_42_45,(uint8*)"dd",bufSize,buf_write);
    if(bufSize < write_len)
    {
        return -1;
    }
    else
    {
        //write success
        return 1;
    }
}

/*==========================================================
 * Function     : mail_FileOperationSeek
 * Author	    : lixu      
 * Description : 写文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
 comm_int32 mail_FileOperationSeek(comm_uint32 file_handle, int32 startPos,
                                                                    uint32 origin)
{
    mail_retval ret = 0;

    if((NULL == file_handle) || (startPos < 0))
    {
        return -1;
    }

    if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle,startPos, origin))
    {
        ret = 1;
    }

    return ret;
}
/*==========================================================
 * Function     : mail_GetFileSizeByPath
 * Author	    : lixu      
 * Description : file_name为文件全路径
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByPath(const char *path_name)
{
    MMIFILE_HANDLE file_handle;
    comm_int32 file_size = 0;

    file_handle = mail_FileOperationOpen((char *)path_name);

    if(!file_handle)
    {
        //SCI_ASSERT(0 != file_handle);
        return -1;
    }

    file_size = MMIAPIFMM_GetFileSize(file_handle);

    if(MMIAPIFMM_CloseFile(file_handle))
    {
        return -1;
    }

    return file_size;
}
/*==========================================================
 * Function     : mail_GetFileSizeByFileName
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByFileName(const char *file_name)
{
    char pathBuf[MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1] = {0};//changed by liangshun for MS00217735 20110105
    comm_int32 size = 0;

    strcpy(pathBuf, MAIL_FM_FOLDER);
    //strcat(pathBuf + strlen(MAIL_FM_FOLDER),"\\");
    strcat(pathBuf + strlen(MAIL_FM_FOLDER), file_name);

    size = mail_GetFileSizeByPath(pathBuf);

    return size;
}
/*==========================================================
 * Function     : mail_SetFileSize
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_SetFileSize(comm_uint32 file_handle,comm_uint32 size)
{
     mail_retval ret = MAIL_OK;

    if(!file_handle)
    {
        ret = MAIL_FM_FILE_ERROR;
        return ret;
    }

    if(SFS_SetFileSize(file_handle,size))
    {
        ret = MAIL_FM_FILE_ERROR;
    }

    return ret;
}
/*==========================================================
 * Function     : mail_RenameFileByWchar
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
 mail_retval mail_RenameFileByWchar(const wchar *srcName, wchar *dstName)
{
    mail_retval ret = MAIL_OK;
    comm_boolean res = FALSE;
    int src_filename_len = 0;
    int dst_filename_len = 0;

    if(PNULL == srcName ||PNULL ==dstName)
    {
        //SCI_TRACE_LOW:"mail_RenameFileByWchar srcName=%d,dstName =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_937_112_2_18_2_47_42_46,(uint8*)"dd", srcName, dstName);
        return MAIL_FM_FILE_ERROR;
    }
    src_filename_len = MMIAPICOM_Wstrlen(srcName);
    dst_filename_len = MMIAPICOM_Wstrlen(dstName);

    if(0 != MMIAPICOM_Wstrcmp(srcName, dstName))
    {
        if(MMIAPIFMM_IsFileExist(srcName, src_filename_len))
        {
            if(MMIAPIFMM_IsFileExist(dstName, dst_filename_len))
            {
                MMIAPIFMM_DeleteFile(dstName, PNULL);
            }
            if(SFS_ERROR_NONE == SFS_RenameFile(srcName, dstName, NULL))
            {
                res = TRUE;
            }
            (void)SFS_DeleteFile(srcName, NULL);
        }
    }
    else
    {
        //
    }

    if(!res)
    {
        ret = MAIL_FM_FILE_ERROR;
    }
    else
    {
        ret = MAIL_OK;
    }

    return  ret;
}
/*==========================================================
 * Function     : mail_RenameFile
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
mail_retval mail_RenameFile(const char *oldName, const char *newName)
{
    mail_retval ret = MAIL_OK;
    comm_boolean res = FALSE;
    comm_uint16 new_full_file_name_len = 0;
    comm_uint16 old_full_file_name_len = 0;
    int old_filename_len = 0;
    int new_filename_len = 0;
    wchar* old_file_name = NULL;   
    wchar* new_file_name = NULL;   

    if(PNULL == oldName ||PNULL ==newName)
    {
        //SCI_TRACE_LOW:"mail_RenameFileByWchar oldName=%d,newName =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_995_112_2_18_2_47_42_47,(uint8*)"dd", oldName, newName);
        return MAIL_FM_FILE_ERROR;
    }

    old_filename_len = strlen(oldName);
    new_filename_len = strlen(newName);

    old_file_name = E_ALLOC(2*(old_filename_len + 1));
    comm_str2wstr((comm_uint16 *)old_file_name, (char *) oldName);

    new_file_name = E_ALLOC(2*(new_filename_len + 1));
    comm_str2wstr((comm_uint16 *)new_file_name, (char *) newName);

    new_full_file_name_len = MMIAPICOM_Wstrlen(new_file_name);
    old_full_file_name_len = MMIAPICOM_Wstrlen(old_file_name);

    if(0 != MMIAPICOM_Wstrcmp(new_file_name, old_file_name))
    {
        if(MMIAPIFMM_IsFileExist(old_file_name, old_filename_len))
        {
            if(MMIAPIFMM_IsFileExist(new_file_name, new_filename_len))
            {
                if(SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(new_file_name, PNULL))
                {
                    E_FREE(new_file_name);
                    E_FREE(old_file_name);
                    return MAIL_FM_FILE_ERROR;
                }                                

                //(void)MMIAPIFMM_DeleteFile(new_file_name, PNULL);
            }

            //res = MMIAPIFMM_RenameFile(old_file_name, old_filename_len, new_file_name, new_filename_len); 
            if(SFS_ERROR_NONE == SFS_RenameFile(old_file_name, new_file_name, NULL))
            {
                res = TRUE;
            }
            if(MMIAPIFMM_IsFileExist(old_file_name, old_filename_len))
            {
                if(SFS_ERROR_NONE != SFS_DeleteFile(old_file_name, NULL))
                {
                    //COMM_TRACE:"mail_fldr mail_RenameFile del old fail!\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1036_112_2_18_2_47_43_48,(uint8*)"");
                }
            }
        }
    else
    {
           res = TRUE;
        }
    }

    E_FREE(new_file_name);
    E_FREE(old_file_name);

    //COMM_TRACE:"mail_fldr mail_RenameFile res=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1049_112_2_18_2_47_43_49,(uint8*)"d",res);
    if(!res)
    {
        ret = MAIL_FM_FILE_ERROR;
    }
    else
    {
        ret = MAIL_OK;
    }

    return  ret;
}
/*==========================================================
 * Function     : mail_FileNameCmp
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_boolean mail_FileNameCmp(const char *FileName1,const char *FileName2)
{
    comm_boolean ret = FALSE;
    //comm_uint16 new_full_file_name_len = 0;
    //comm_uint16 old_full_file_name_len = 0;

    int file_name1_len = 0;
    int file_name2_len = 0;
    wchar* file_name1 = NULL;   
    wchar* file_name2 = NULL;   

    if(PNULL == FileName1 ||PNULL ==FileName2)
    {
        //SCI_TRACE_LOW:"mail_FileNameCmp FileName1=%d, FileName2 =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1082_112_2_18_2_47_43_50,(uint8*)"dd", FileName1, FileName2);
        return MAIL_FM_FILE_ERROR;
    }

    file_name1_len = strlen(FileName1);
    file_name2_len = strlen(FileName2);

    file_name1 = E_ALLOC(2*(file_name1_len + 1));
    comm_str2wstr((comm_uint16 *)file_name1, (char *) FileName1);

    file_name2 = E_ALLOC(2*(file_name2_len + 1));
    comm_str2wstr((comm_uint16 *)file_name2, (char *) FileName2);

    //new_full_file_name_len = MMIAPICOM_Wstrlen(full_source_filename);
    //old_full_file_name_len = MMIAPICOM_Wstrlen(full_target_filename);

    if(0 == MMIAPICOM_Wstrcmp(file_name1, file_name2))
    {
        ret = TRUE;
    }

    E_FREE(file_name1);
    E_FREE(file_name2);

    return ret;
}
/*==========================================================
 * Function     : mail_GetFileSizeByHandle
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_int32 mail_GetFileSizeByHandle(MMIFILE_HANDLE file_handle)
{
    //MMIFILE_HANDLE file_handle;
    comm_int32 size = 0;

    if(NULL == file_handle)
    {
        return -1;
    }

    size = MMIAPIFMM_GetFileSize(file_handle);
    
    return size;
    
}

/*==========================================================
 * Function     : mail_GetFileNumOfDir
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
 comm_int32 mail_GetFileNumOfDir(const char* pathName)
{
    //wchar filter[]=L"*";
    wchar *full_path = NULL;
    int file_name_len = 0;
    MMIFILE_FIND_DATA_T find_result   =  {0};
    MMIFILE_HANDLE handle_result =   0;    
    comm_int32 num_of_file = 0;
    comm_int32 full_path_len = 0;

    //SCI_TRACE_LOW:"mail_GetFileNumOfDir"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1150_112_2_18_2_47_43_51,(uint8*)"");
    if(NULL == pathName)
    {
        return -1;
    }

    file_name_len = strlen(pathName);
    full_path = E_ALLOC(2*(file_name_len + 1));
    comm_str2wstr((comm_uint16 *)full_path, (char *) pathName);
    full_path_len = MMIAPICOM_Wstrlen(full_path);

    //result = SFS_FindFirstFileCtrl(full_path,NULL,NULL,&file_handle,&find_data,PNULL);
    handle_result = MMIAPIFMM_FindFirstFile(full_path, full_path_len,&find_result);
    if (SFS_INVALID_HANDLE != handle_result)
    {
        do
        {
            num_of_file++;
        }
        while (SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(handle_result, &find_result)); 
    }
    SFS_FindClose(handle_result);

    #if 0
    if (NULL != file_handle)
    {
        SFS_FindClose(file_handle);
    }
    #endif

    E_FREE(full_path);
    return num_of_file;
}
/*==========================================================
 * Function     : mail_RemoveFile
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
comm_uint32 mail_RemoveFile(const char* FileName)
{
    wchar *file_name = NULL;
    int file_name_len = 0;

    if(PNULL == FileName)
    {
        //SCI_TRACE_LOW:"mail_RemoveFile FileName=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1198_112_2_18_2_47_43_52,(uint8*)"d", FileName);
        return -1;
    }

    file_name_len = strlen(FileName);
    file_name = E_ALLOC(2*(file_name_len + 1));
    comm_str2wstr((comm_uint16 *)file_name, (char *) FileName);

    if(!MMIAPIFMM_DeleteFile(file_name, PNULL))
    {            
        E_FREE(file_name);
        return 1;
    }
    else
    {
        E_FREE(file_name);
        return (comm_uint32)-1;
    }
}

/*==========================================================
 * Function     : mail_sentMailWriteFile
 * Author	    : lixu      
 * Description :  发送邮件时将编码后的字符写到文件中
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  
 ==========================================================*/
 comm_boolean mail_sentMailWriteFile(const char* file_name,void* write_buf,comm_uint32 buffer_size)
{
    MMIFILE_HANDLE file_handle = NULL;
    comm_uint32 buf_write = 0;
    comm_uint32 write_len = 0;
    wchar wfile_name[2*(MAIL_MAX_UIDL_LENGTH + 1)] = {0};
    comm_int32 offset = 0;

    write_len = buffer_size;
    //COMM_TRACE:"mail_fldr mail_sentMailWriteFile start write file(%s) buffer_size=%d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1235_112_2_18_2_47_43_53,(uint8*)"sd",file_name,buffer_size);

    comm_str2wstr(wfile_name, (char *)file_name);
    //MMIAPICOM_StrToWstr(file_name, wfile_name);

    file_handle = MMIAPIFMM_CreateFile(wfile_name, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    if(NULL == file_handle)
    {
        //COMM_TRACE:"mail_fldr mail_sentMailWriteFile create file(%s)fail!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1243_112_2_18_2_47_43_54,(uint8*)"s",file_name);
        return FALSE;
    }
    do
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, write_buf, write_len, &buf_write, NULL))
        {
          return FALSE; 
        }
        //当buf_write==0 时会出现死循环
        if(0 == buf_write)
        {
          return FALSE;
        }

        if(write_len > buf_write)
        {
           offset += (comm_int32)buf_write;
           MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN);
        }
        write_len -= buf_write;

    } while (0 < write_len);

    MMIAPIFMM_CloseFile(file_handle);

    return TRUE;
}

/*==========================================================
 * Function     : mail_GetFilePathByFileName
 * Author	    : lixu      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_GetFilePathByFileName(const char *file_name,char* path)
{
    char pathBuf[MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1] = {0};//changed by liangshun for MS00217735 20110105
    int str_len = 0;

    strcpy(pathBuf, MAIL_FM_FOLDER);
    strcat(pathBuf + strlen(MAIL_FM_FOLDER), file_name);
    str_len = strlen(pathBuf);
    strncpy(path,pathBuf,str_len);
}

#endif

/*==========================================================
 * Function     : mail_CleanDir
 * Author	    : sunguochen      
 * Description : 清空目录
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/21/2010 
 ==========================================================*/
void mail_CleanDir(comm_uint16* directory)
{
    SFS_HANDLE      file_handle = NULL;
    SFS_FIND_DATA_T find_data = {0};
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    int full_path_len = 0;

    MMIAPICOM_Wstrcpy(filename, directory);
    full_path_len = MMIAPICOM_Wstrlen(filename);
    MMIAPICOM_StrToWstr((uint8 *)"\\*", filename + full_path_len);
    full_path_len = MMIAPICOM_Wstrlen(filename);

    //解决多次嵌套调用mail_CleanDir()后可能导致栈溢出的问题
    while (SFS_INVALID_HANDLE != (file_handle = MMIAPIFMM_FindFirstFile(filename, full_path_len, &find_data)))
    {
        //COMM_TRACE:"Mail:delet folder exit\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1316_112_2_18_2_47_43_55,(uint8*)"");
        //循环调用MMIAPICOM_DelDirAll(),删除filename目录中所有的文件和子目录
        MMIAPICOM_DelDirAll(directory, MMIFILE_FULL_PATH_MAX_LEN+1);
        
        //if (NULL != file_handle)
        {
            SFS_FindClose(file_handle);
        }
    }
    //多次调用MMIAPICOM_DelDirAll()后只剩下空的filename目录
}

/*==========================================================
 * Function     : mail_GetDirFileNum
 * Author	    : sunguochen      
 * Description : 清空目录
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/25/2010 
 ==========================================================*/
int mail_GetDirFileNum(comm_uint16* directory)
{
    //SFS_ERROR_E     result = SFS_NO_ERROR;
    SFS_HANDLE      file_handle = NULL;
    SFS_FIND_DATA_T find_data = {0};
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    int full_path_len = 0;
    int file_num = 0;

    MMIAPICOM_Wstrcpy(filename, directory);
    full_path_len = MMIAPICOM_Wstrlen(filename);
    MMIAPICOM_StrToWstr((uint8 *)"\\*.*", filename + full_path_len);
    full_path_len = MMIAPICOM_Wstrlen(filename);
    file_handle = MMIAPIFMM_FindFirstFile(filename, full_path_len,&find_data);

    if (SFS_INVALID_HANDLE != file_handle)
    {        
        do
        {            

            if(!(find_data.attr & SFS_ATTR_DIR))
            {
                //get file num
                file_num ++;
            }

        }while (SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(file_handle, &find_data)); 

        SFS_FindClose(file_handle);
    }

    //COMM_TRACE:"Mail:mail_GetDirFileNum %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_FILE_1367_112_2_18_2_47_43_56,(uint8*)"d", file_num);

    return file_num;
}




