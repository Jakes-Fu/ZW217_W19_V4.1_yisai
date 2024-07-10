#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_filemgr.h"
#include "mmifmm_export.h"

SFS_HANDLE jt_Fopen(const uint16 * file_name, const char * access_mode)
{
	SFS_HANDLE Rtn;
	Rtn = MMIFILE_CreateFile(file_name,SFS_MODE_OPEN_EXISTING | SFS_MODE_READ,NULL,NULL);	

    SCI_TRACE_LOW("tts jt_Fopen, Rtn = %d",Rtn);	
        
	return Rtn;
}

void jt_Fclose(SFS_HANDLE FileHandle)
{
    SFS_ERROR_E Rtn;
	Rtn = MMIFILE_CloseFile(FileHandle);	

    if(Rtn)
    {
        SCI_TRACE_LOW("tts jt_Fclose failed! Rtn = %d",Rtn);	
    }
    else
    {
       SCI_TRACE_LOW("tts jt_Fclose successfully! Rtn = %d",Rtn);	
    }
}


int32 jt_Fseek(SFS_HANDLE FileHandle,int32 offset,uint32 origin)
{	
	int32 seek_result = 0;

	seek_result = MMIAPIFMM_SetFilePointer(FileHandle,offset,origin);
	SCI_TRACE_LOW("tts jt_Fseek seek_result = %d",seek_result);	
	if (SFS_ERROR_NONE != seek_result)
	{
		seek_result = -1;
	}
	return seek_result;
}

uint32 jt_Fread(void * buffer,uint32 size,uint32 count,SFS_HANDLE FileHandle)
{
	uint32 bytes_read = 0;
	if(SFS_ERROR_NONE == MMIFILE_ReadFile(FileHandle,buffer,size * count,&bytes_read,NULL)
		&& bytes_read == size * count)
	{
		bytes_read = count;
	}
	else
	{
		bytes_read = 0;
	}
	SCI_TRACE_LOW("tts jt_Fread bytes_read = %d, size = %d, count = %d",bytes_read,size,count);
	return bytes_read;
}

