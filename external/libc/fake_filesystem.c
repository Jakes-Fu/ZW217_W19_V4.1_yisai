#include "fake.h"
#include "os_api.h"
#include "sfs.h"
#include "dirent.h"

#define FILE_MAX_PATH		256

typedef struct sci_dir_t {
    uint16  wPath[FILE_MAX_PATH + 2];
    struct dirent sci_dirent;
    SFS_HANDLE  read_handle;
}sci_dir;

// file
/*******************************************************************/
//  Interface:		FAKE_StrToWstr
//  Description : 	FAKE_StrToWstr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static uint16* SPLIBC_StrToWstr(
							  const char* src,
							  uint16* dst
							  )
{
	uint16 *return_wstr = dst;
	
	if( src == PNULL )
	{
		return return_wstr;
	}
	
	while( '\0' != ( *dst++ = *src++ ) )    /* copy string */
	{
		NULL;/*lint !e522*/
	}
	
	return return_wstr;
}


// file
/*******************************************************************/
//  Interface:		SPLIBC_WstrToStr
//  Description : 	SPLIBC_WstrToStr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static uint8* SPLIBC_WstrToStr(
							  const uint16* src ,
							  char* dst
							  )
{
	uint8 *return_wstr = dst;
	
	if( src == PNULL )
	{
		return return_wstr;
	}
	
	while( '\0' != ( *dst++ = *src++ ) )    /* copy string */
	{
		NULL;/*lint !e522*/
	}
	
	return return_wstr;
}


/*******************************************************************/
//  Interface:		FAKE_sflags
//  Description : 	FAKE_sflags
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static uint32 SPLIBC_sflags(const char *mode)
{
	int ret, m, o;
	
	switch (*mode++) 
	{
	case 'r':	/* open for reading */
		m = SFS_MODE_READ;
		o = SFS_MODE_OPEN_EXISTING;
		break;
		
	case 'w':	/* open for writing */
		m = SFS_MODE_WRITE;
		o = SFS_MODE_CREATE_ALWAYS;
		break;
		
	case 'a':	/* open for appending */
		m = SFS_MODE_WRITE;
		o = SFS_MODE_APPEND;
		break;
		
	default:	/* illegal mode */
		return (0);
	}
	
	/* [rwa]\+ or [rwa]b\+ means read and write */
	if (*mode == '+' || (*mode == 'b' && mode[1] == '+')) 
	{
		m = SFS_MODE_READ | SFS_MODE_WRITE;
	}
	ret = m | o;
	return (ret);
}

void Ali_change_path(char * in, char* out)
{
    uint32 in_len =  strlen(in) + 1;
    uint32 i =0,j=0;
    for(i = 0; i< in_len ;i ++)
    {
        if(in[i] == '/')
        {
            out[j] = '\\' ;
            j+= 1;
        }
        else
        {
            out[j] = in[i] ;
            j+=1;
        }
    }
}
/*******************************************************************/
//  Interface:		SPRD_fopen
//  Description : 	SPRD_fopen
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
FILE * SPLIBC_fopen(const char * path,const char * mode)
{
	uint16 wPath[FILE_MAX_PATH+2] = {0};
    uint8 use_path[FILE_MAX_PATH] = {0};
	FILE * pfile = 0;
	
	if(PNULL == path || PNULL == mode)
	{
		return 0;
	}
    Ali_change_path((char *)path,use_path);
	
	SPLIBC_StrToWstr(use_path, wPath);
	pfile = (FILE *)SFS_CreateFile(wPath, SPLIBC_sflags(mode), 0, 0);
	SPLIBC_FAKE_LOG("SPLIBC_fopen. handle = %d\n", (SFS_HANDLE)pfile);/*lint !e522*/
	SCI_DEBUG("[curl jsong_debug] ----pfile:0x%x",  pfile);
	return pfile;
}

/*******************************************************************/
//  Interface:		SPRD_fclose
//  Description : 	SPRD_fclose
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fclose(FILE * stream)
{
	if(NULL == stream)
	{
		return (-1);
	}
	
	return SFS_CloseFile((SFS_HANDLE)stream);
}

/*******************************************************************/
//  Interface:		SPRD_feof
//  Description : 	SPRD_feof
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_feof(FILE * stream)
{
	uint32 length =0;
	int32 pos = 0;
	
	if(NULL == stream)
	{
		return 0;
	}
	
	if(0 == SFS_GetFileSize((SFS_HANDLE)stream, &length))
	{
		if(0 == SFS_GetFilePointer((SFS_HANDLE)stream, SFS_SEEK_BEGIN, &pos))
		{
			SPLIBC_FAKE_LOG("SPLIBC_feof length: %d,pos:%d \n", length,pos);/*lint !e522*/
			if(length == (uint32)pos)
			{
				return 1;
			}
		}
	}
	
	return 0;	
}

/*******************************************************************/
//  Interface:		SPRD_fread
//  Description : 	SPRD_fread
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
size_t SPLIBC_fread(void * ptr,size_t size,size_t nmemb,FILE * stream)
{
	uint32 bytes_read = 0;
	
	if(NULL == ptr || NULL == stream)
	{
		return 0;
	}
	
	if(0 == SFS_ReadFile((SFS_HANDLE)stream, ptr,(size * nmemb), &bytes_read, NULL))
	{
		return (bytes_read / size);
	}
	
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fwrite
//  Description : 	SPRD_fwrite
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
size_t SPLIBC_fwrite(const void * ptr,size_t size,size_t nmemb,FILE * stream)
{
	uint32 bytes_write = 0;
	
	if(NULL == ptr || NULL == stream)
	{
		return 0;
	}
	
	if(0 == SFS_WriteFile((SFS_HANDLE)stream, ptr,(size * nmemb), &bytes_write, NULL))
	{
		return (bytes_write / size );
	}
	
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fseek
//  Description : 	SPRD_fseek
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fseek(FILE * stream,long offset,int whence)
{
	if(NULL == stream)
	{
		return (-1);
	}
	SPLIBC_FAKE_LOG("SPLIBC_fseek  handle = %d\n", (SFS_HANDLE)stream);/*lint !e522*/
	return SFS_SetFilePointer((SFS_HANDLE)stream, (__int64)offset, whence);
}

/*******************************************************************/
//  Interface:		SPRD_ftell
//  Description : 	SPRD_ftell
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
long SPLIBC_ftell(FILE * stream)
{
	int32 pos = 0;
	
	if(NULL == stream)
	{
		return (-1);	
	}
	SPLIBC_FAKE_LOG("SPLIBC_ftell handle = %d\n", (SFS_HANDLE)stream);/*lint !e522*/
	if(0 == SFS_GetFilePointer((SFS_HANDLE)stream, SFS_SEEK_BEGIN, &pos))
	{
		return pos;
	}
	
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_rewind
//  Description : 	SPRD_rewind
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void SPLIBC_rewind(FILE * stream)
{
	SFS_SetFilePointer((SFS_HANDLE)stream, (__int64)0, SFS_SEEK_BEGIN);
}

/*******************************************************************/
//  Interface:		SPRD_fileno
//  Description : 	SPRD_fileno
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fileno(FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_mktemp
//  Description : 	SPRD_mktemp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
char * SPLIBC_mktemp(char * temp)
{
	//SCI_ASSERT(FALSE);
	return 0;
}
/*******************************************************************/
//  Interface:		SPRD_setbuf
//  Description : 	SPRD_setbuf
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void SPLIBC_setbuf(FILE * stream,char * buf)
{
	//SCI_ASSERT(FALSE);
}

/*******************************************************************/
//  Interface:		SPRD_setbuffer
//  Description : 	SPRD_setbuffer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void SPLIBC_setbuffer(FILE * stream,char * buf,size_t size)
{
	//SCI_ASSERT(FALSE);
}

/*******************************************************************/
//  Interface:		SPRD_fflush
//  Description : 	SPRD_fflush
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fflush(FILE* stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_freopen
//  Description : 	SPRD_freopen
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
FILE * SPLIBC_freopen(const char * path, const char * mode,FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fdopen
//  Description : 	SPRD_fdopen
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
FILE * SPLIBC_fdopen(int fildes,const char * mode)
{
	//SCI_ASSERT(FALSE);
	return 0;
}


DIR* SPLIC_opendir(const char*  dirpath)
{
	SFS_FIND_DATA_T ppath = {0};
	sci_dir*    p_scidir = NULL;
	uint8 use_path[FILE_MAX_PATH] = {0};

	if(PNULL == dirpath) {
		return 0;
	}

    p_scidir = malloc(sizeof(sci_dir));
        
	if(PNULL == p_scidir) {
		return 0;
	}
    SCI_MEMSET(p_scidir, 0, sizeof(sci_dir));


    Ali_change_path((char *)dirpath,use_path);
	SPLIBC_StrToWstr(use_path, p_scidir->wPath);
        
        SCI_TraceLow("TODO opendir null");
	return p_scidir;
}

int SPLIC_closedir(DIR*  dirp)
{
        if (dirp == NULL)
        {
            SCI_TraceLow("[%s:%d] error input param ", __FUNCTION__, __LINE__);

            return -1;
        }

        free(dirp);
        dirp = NULL;
	SCI_TraceLow("error !! TODO closedir null");
	return 0;
}


uint32 SPLIC_mkdir(const char*  dirpath)
{
	uint16 wPath[FILE_MAX_PATH+2] = {0};
	uint8 use_path[FILE_MAX_PATH] = {0};

	if(PNULL == dirpath) {
		return -1;
	}
    Ali_change_path((char *)dirpath,use_path);
	SPLIBC_StrToWstr(use_path, wPath);
	
	return SFS_CreateDirectory(dirpath);
}

uint32 SPLIC_rmdir(const char*  dirpath)
{
	uint16 wPath[FILE_MAX_PATH+2] = {0};	
	uint8 use_path[FILE_MAX_PATH] = {0};

	if(PNULL == dirpath) {
		return -1;
	}
    Ali_change_path((char *)dirpath,use_path);
	SPLIBC_StrToWstr(use_path, wPath);
	
	return SFS_DeleteDirectory(dirpath);
}

uint32 SPLIC_dir_is_exist(const char*  dirpath)
{
	uint16 wPath[FILE_MAX_PATH+2] = {0};
	FILE * pfile = 0;
	SFS_ERROR_E state = SFS_NO_ERROR;

	uint8 use_path[FILE_MAX_PATH] = {0};

	if(PNULL == dirpath) {
		return -1;
	}
    Ali_change_path(dirpath,use_path);
	SPLIBC_StrToWstr(use_path, wPath);

	state = SFS_CreateDirectory(dirpath);
	if(state == SFS_NO_ERROR) {
		SFS_DeleteDirectory(dirpath);
		return 0;
	} else if(state == SFS_ERROR_HAS_EXIST) {
		return 1;
	} else {
		return -1;
	}
}

struct dirent* SPLIC_readdir(DIR*  dirp)
{
	sci_dir*    p_scidir = NULL;
        SFS_FIND_DATA_T fifle_info = {0};
            
        if (p_scidir == NULL)
        {
            return NULL;
        }
        
        if (p_scidir->read_handle == 0)
        {
	    SCI_TraceLow("[%s:%d] first readdir ", __FUNCTION__, __LINE__);
            p_scidir->read_handle = SFS_FindFirstFile(p_scidir->wPath, &fifle_info);
            SPLIBC_WstrToStr(fifle_info.name, p_scidir->sci_dirent.d_name);
            
        }
        else
        {
	    SCI_TraceLow("[%s:%d] next readdir ", __FUNCTION__, __LINE__);

            if (SFS_NO_ERROR == SFS_FindNextFile(p_scidir->read_handle, &fifle_info))
            {
                SCI_TraceLow("[%s:%d] readdir end", __FUNCTION__, __LINE__);
            
                return NULL;
            }
            SPLIBC_WstrToStr(fifle_info.name, p_scidir->sci_dirent.d_name);                
        }
        

        SCI_TraceLow("[%s:%d] readdir end ", __FUNCTION__, __LINE__);
	return &p_scidir->sci_dirent;
}



int  unlink(const char *pathname)
{
    uint16 wPath[FILE_MAX_PATH+2] = {0};
    int ret= 0;
    
    if (pathname == NULL)
    {
        SCI_TraceLow("[%s:%d] error input param ", __FUNCTION__, __LINE__);
        return -1;
    }
    
    SPLIBC_StrToWstr(pathname, wPath);

    ret = SFS_DeleteFile(wPath, NULL);
    
    SCI_TraceLow("[%s:%d] ret:%d ", __FUNCTION__, __LINE__, ret);

    return ret;
}

