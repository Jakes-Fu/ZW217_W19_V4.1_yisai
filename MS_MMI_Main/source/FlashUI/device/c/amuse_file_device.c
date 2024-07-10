/*
* amuse_file.c
* Copyright (C) 2011 Microrapid, Inc.
* Authors: Tim <tim@Microrapid.com>
*
* This file implements file interface on Win32.
*/

#include "mmi_flashui_trc.h"
#include "amuse.h"
#include "amuse_file_device.h"
#include "stdarg.h"
#ifndef FLASHUI_WIN32_SIM_DLL
#include "mmiidle_export.h"
#endif

#define AMUSE_ENABLE_DEBUG	1
#define TRACE_TO_FILE 	0

extern void amuse_error(int errcode);

static FILE_HANDLE print_fh=0;

LOCAL uint32 GetFileOptMode(const char *mode)
{
	uint32 opt = 0;

	SCI_ASSERT(PNULL != mode);/*assert verified*/

	if(0 == strcmp(mode, "r") || 0 == strcmp(mode, "rb"))
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
	}
	else if (0 == strcmp(mode, "r+"))
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "w") || 0 == strcmp(mode, "wb"))
	{
		opt = SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "w+"))
	{
		opt = SFS_MODE_CREATE_ALWAYS | SFS_MODE_READ | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "a"))
	{
		opt = SFS_MODE_APPEND | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "a+"))
	{
		opt = SFS_MODE_APPEND | SFS_MODE_WRITE | SFS_MODE_READ ;
	} 
	else if(0 == strcmp(mode, "rb+"))
	{
		opt = SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE | SFS_MODE_READ ;
	}
	else
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
	}

	return opt;
}

FILE_HANDLE amuse_fopen_device(FILE_NAME fileName, OPEN_MODE mode)
{
	SFS_HANDLE f = 0;
	uint32 uflag = 0;
	uint16  full_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	uflag = 67;//GetFileOptMode(mode);
	GUI_GB2UCS((wchar *)full_path, (const uint8 *)fileName, (uint16)SCI_STRLEN(fileName));

	f = MMIAPIFMM_CreateFile((uint16 *)full_path,uflag, 0, 0);

	return (FILE_HANDLE)f;
}

int amuse_fclose_device(FILE_HANDLE f)
{
	if (PNULL != f)
	{
		 MMIAPIFMM_CloseFile(f); 
	}
	else
	{
		//SCI_TRACE_LOW:"MMIEBOOK_FFS_fclose file_handle = PNULL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMUSE_FILE_DEVICE_85_112_2_18_3_27_24_0,(uint8*)"");
	}
	
    return 0;
}

int amuse_ftell(FILE_HANDLE f)
{
	int32 pos = 0;
	if(SFS_GetFilePointer(f,0,&pos) == SFS_ERROR_NONE)
	{
		return pos;
	}
	
	return FILE_ERROR;
}

int amuse_fseek(FILE_HANDLE f, int offset, int origin)
{
	int result = 0;

	//SCI_ASSERT(NULL != f); 
    if(NULL == f)
	{
	   return result;
	}

	result = MMIAPIFMM_SetFilePointer(f, offset, origin);

	return result; 
}

int amuse_fread(FILE_HANDLE f, void* pData, int size, int count)
{
	SIZE_T len = 0;
	if(NULL == f || NULL == pData)
	{
		amuse_printf("amuse_fread,read file error.\n");
		return 0;
	}

	MMIAPIFMM_ReadFile(
			f, 
			pData, 
			size * count, 
			&len, 
			NULL
			);
	return len;
}

int amuse_fwrite(FILE_HANDLE f, void* pData, int size, int count)
{
	SIZE_T len = 0;
	if( NULL == f || NULL == pData )
	{
		//SCI_TRACE_LOW:"amuse_fwrite invalid parameter"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMUSE_FILE_DEVICE_141_112_2_18_3_27_24_1,(uint8*)"");
		return 0;
	}
	//SCI_ASSERT(NULL != f);
	//SCI_ASSERT(NULL != pData);


	MMIAPIFMM_WriteFile(f, pData, size * count, &len, NULL);
	return len;
}

int amuse_fgetc(FILE_HANDLE f)
{
	char c;
	if (amuse_fread(f, &c, 1, 1) != 1)
	{
		return FILE_ERROR;
	}
	return (int)c;

}

int amuse_fputc(FILE_HANDLE f, int c)
{
	char buf = (char)c;
	if (amuse_fwrite(f, &buf, 1, 1) != 1)
	{
		return FILE_ERROR;
	}
	return FILE_OK;
}

int amuse_fprintf(FILE_HANDLE f, const char* format, ...)
{
	return 0;
}

int amuse_fflush(FILE_HANDLE f)
{
	return 0;
}

int amuse_ferror(FILE_HANDLE f)
{
	return 0;
}

int amuse_feof(FILE_HANDLE f)
{
	return MMIAPIFMM_IsEndOfFile(f);
}

int amuse_remove(FILE_NAME f)
{
	uint16  full_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	if(f == NULL)
	{
		return FILE_ERROR;
	}
	GUI_GB2UCS((wchar *)full_path,(const uint8 *)f, SCI_STRLEN(f));
	if(SFS_DeleteFile((const uint16*)full_path,NULL) == SFS_ERROR_NONE)
	{
		amuse_printf("remove file: %s ok",f);
		return FILE_OK;
	}
	amuse_printf("remove file: %s Fail",f);
	return FILE_ERROR;
}

int amuse_rename(FILE_NAME oldfn, FILE_NAME newfn)
{
	return -1;
}

unsigned int amuse_getFileSize(FILE_NAME fileName, FILE_HANDLE fileHandle)
{
	uint32 filesize = 0;
	
	if(fileName)	
	{
		unsigned int len;
		FILE_HANDLE fp;
		fp = amuse_fopen_device(fileName, OPEN_MODE_READ_BIN);
		if (!fp)
		{
			return 0;
		}
		amuse_fseek(fp, 0L, SEEK_END);
		len = amuse_ftell(fp);
		amuse_fclose_device(fp);
		return len;	

	}
	else if(fileHandle)
	{
		if(SFS_GetFileSize(fileHandle, &filesize) == SFS_ERROR_NONE)
		{
			return filesize;
		}
	}
	return 0;
}

int amuse_mkdir(const char* dirname)
{
	return FILE_ERROR;
}

int amuse_create_file(FILE_NAME fileName)
{
	return 0;
}

int amuse_check_free_space(char disk, int min_size)
{
	uint32 free_space = MMI_GetFreeSpace(MMIAPIFMM_GetDefaultDisk());

	if(free_space  > (uint32)((1 << 21) + min_size) )
	{
		return 0;
	}
	else
	{
#ifndef WIN32
		amuse_error(-3);
#endif
		return -1;
	}
}

void amuse_print_init()
{
#if TRACE_TO_FILE
	static char szLogPath[255]={"d:\\APP\\flashlog.txt"};
	if(!print_fh)
	{
		/*if( szLogPath[0] == 0)
		{
			sprintf(szLogPath, "d:\\APP\\%d.txt",amuse_getTickCount());
		}*/
		
		print_fh = amuse_fopen_device(szLogPath, OPEN_MODE_READ_AND_WRITE_BIN);
	}
#endif
}

void amuse_print_destroy()
{
#if TRACE_TO_FILE
	if(print_fh)
	{
		amuse_fclose_device(print_fh);
		print_fh = 0;
	}
#endif
}

void amuse_printf(const char *format, ...)
{
#if AMUSE_ENABLE_DEBUG
	
	static char buffer[512] ={0};// (char*)SCI_Malloc(512,__FILE__,__LINE__);
	va_list l;
	va_start(l,format);	
	SCI_MEMSET(buffer, 0, sizeof(buffer));
	vsprintf(buffer,format,l);
	
	//SCI_TRACE_MID:"flash_printf:***** %s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMUSE_FILE_DEVICE_308_112_2_18_3_27_25_2,(uint8*)"s",buffer);
	#if TRACE_TO_FILE
		amuse_print_init();
		if(print_fh)	
		{
			amuse_fseek(print_fh, 0, SEEK_END);
			amuse_fwrite(print_fh, buffer, 1, strlen(buffer));	
			amuse_print_destroy();//for flush data to file
		}
	#endif
	//SCI_Free(buffer);
#endif
}


void amuse_getWorkingDirectory(int size, char* p)
{
	return;	
}

char* amuse_get_path_separator()
{
	// In most platforms, it should be '/' or '\\'
	return "\\";
}

int amuse_is_full_path(char* path)
{
	return strstr(path, ":") != 0;
}

int need_check_if_word_based()
{
	return 1;
}

#define MAX_PATH 260
static char workingDirectory[MAX_PATH];

void
set_working_directory(char* filePath)
{
	// first try to get working directory from file path
	// if fail, then get the platform default working directory.

	int i = strlen(filePath) - 1;
	while(i > 0 && strncmp(&filePath[i], amuse_get_path_separator(), 1) != 0){
		-- i;
	}
	if(i > 0){
		memcpy(workingDirectory, filePath, i);
		workingDirectory[i] = 0;
	}else{
		amuse_getWorkingDirectory(MAX_PATH, workingDirectory);
	}
}

void
get_working_directory(int size, char* p)
{
	//SCI_ASSERT((int)strlen(workingDirectory) < size);
	if((int)strlen(workingDirectory) >= size)
	{
	  return;
	}
	strcpy(p, workingDirectory);
}

int 
amuse_is_local_disk_path(char* path)
{	
	return ((path[0] >= 'A' && path[0] <= 'Z' || path[0] >= 'a' && path[0] <= 'z') && path[1] == ':');
}

void
get_save_path(int size, char *path)
{
	if(path)
	{
		if(0==workingDirectory[0])
		{
			strcpy(workingDirectory, "D:\\APP");
		}
		//SCI_ASSERT( (size > (int)strlen(workingDirectory)) );
		if((int)strlen(workingDirectory) >= size)
		{
		  return;
		}
		strcpy(path, workingDirectory);
	}
}

