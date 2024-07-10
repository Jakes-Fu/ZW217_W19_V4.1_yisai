#if defined(HERO_ENGINE_HMGS_SUPPORT)&&(!defined(WIN32))

#include <stdio.h>
#include <stdarg.h>

#include "HcFileAdapter.h"
//#include "hero_util.h"
//#include "valfsiapi.h"
#include "Mmi_filemgr.h"
#include "os_api.h"
#include "mmifmm_export.h"

#ifdef TOO_MODE
#include "gmem.h"
#include "util.h"
#else
#endif

#include "hero_income_data_res.h"

#ifdef TOO_MODE

#define HMGS_FILE_RDONLY         MR_FILE_RDONLY//以只读的方式打开文件。
#define HMGS_FILE_WRONLY        MR_FILE_WRONLY//以只写的方式打开文件。
#define HMGS_FILE_RDWR             MR_FILE_RDWR//以读写的方式打开文件。
#define HMGS_FILE_CREATE          MR_FILE_CREATE//如果文件不存在，创建该文件。
#define HMGS_IS_FILE     MR_IS_FILE      //文件
#define HMGS_IS_DIR      MR_IS_DIR      //目录
#define HMGS_IS_INVALID  MR_IS_INVALID  //无效(非文件、非目录)

void *  hmgs_malloc(int s)
{
	return mrc_malloc(s);
}
 
void hmgs_free(void *address)
{
	mrc_free(address);
}

int32 hmgs_getLen(const char* filename)
{
	return mrc_getLen(filename);
}



int32 hmgs_open(const char* filename,  uint32 mode)
{
	return mrc_open(filename,mode);
}

int32 hmgs_read(int32 f,void *p,uint32 l)
{
	return mrc_read(f,p,l);
}

int32 hmgs_close(int32 f)
{
	return mrc_close(f);
}

int32 hmgs_fileState(const char* filename)
{
	return mrc_fileState(filename);
}
#else
#define HMGS_FILE_RDONLY         (0)//FS_READ_ONLY//以只读的方式打开文件。
#define HMGS_IS_FILE     1      //文件
#define HMGS_IS_DIR      2      //目录
#define HMGS_IS_INVALID  8  //无效(非文件、非目录)

#define HERO_CALL_FLASH_TCRAD               (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD)
#define HERO_CALL_FLASH_TCRAD_LEN           MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)

#define HERO_CALL_ROOT_PATH					L"heroEngineSpr\\"
#define HERO_CALL_ROOT_PATH_LEN				hr_str_wstrlen(HERO_CALL_ROOT_PATH)

static uint16 shapp_Callfile[4] = {0};
static uint8 gHeroCallFullPath[128] = {0};

#define HERO_CALL_ROOT_DIR_LEN  (6) 
void hmgs_init_file_drv()
{
	uint16 *pDeviceRoot = NULL;
	if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_CALL_FLASH_TCRAD, HERO_CALL_FLASH_TCRAD_LEN))
	{
		pDeviceRoot =  (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else
	{
		pDeviceRoot =  (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
	}
	shapp_Callfile[0] = pDeviceRoot[0];
	shapp_Callfile[1] = ':';
	shapp_Callfile[2] = '\\';
	shapp_Callfile[3] = 0;
}


char* hmgs_get_filename(char *outputbuf,const char *filename)
{
	int len = 0;
	hmgs_init_file_drv();
	memset(gHeroCallFullPath, 0, sizeof(gHeroCallFullPath));
	memcpy(gHeroCallFullPath,(char*)shapp_Callfile, HERO_CALL_ROOT_DIR_LEN);
	len += HERO_CALL_ROOT_DIR_LEN;

	MMIAPICOM_Wstrncpy(gHeroCallFullPath+len, HERO_CALL_ROOT_PATH, HERO_CALL_ROOT_PATH_LEN);
	len += HERO_CALL_ROOT_PATH_LEN;
	
	len = hr_str_wstrlen(gHeroCallFullPath);
	GUI_GB2UCS(gHeroCallFullPath+len, filename, strlen(filename));
	return gHeroCallFullPath;
}

void *  hmgs_malloc(int sz)
{
	void* buffer = NULL;
	return SCI_ALLOCA(sz);
}
 
void hmgs_free(void *address)
{
	if(address)
	{
		SCI_FREE(address);
		address=NULL;
	}
}

int32 hmgs_getLen(const char* filename)
{
	int filehandle = -1;
	//U16 fullpathname[128] = {0};
	unsigned int size = -1;
	int ferrno;
	uint32 file_size_ptr;

	if(hmgs_get_filename(NULL, filename) == NULL)
	{
		return HR_FAILED;
	}
	
	//filehandle = FS_Open((WCHAR *)fullpathname, FS_READ_ONLY);
	filehandle = SFS_CreateFile(gHeroCallFullPath, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);
	if(filehandle != SFS_INVALID_HANDLE)
	{
		ferrno = SFS_GetFileSize(filehandle, &file_size_ptr);
        if(SFS_ERROR_NONE == ferrno)
        {
            size = file_size_ptr;
        }
        MMIFILE_CloseFile(filehandle);
	}
	return size;

#if 0
	if (filehandle < FS_NO_ERROR)
		return HR_FAILED;

	ret = FS_GetFileSize( filehandle, &size);
	if (ret == FS_NO_ERROR)
	{
		FS_Close(filehandle);
		return size;
	}
	else
	{
		FS_Close(filehandle);
		return HR_FAILED;
	}
#endif
}

int32 hmgs_open(const char* filename,  uint32 mode)
{
#if 1
	int filehandle = -1;
	if(hmgs_get_filename(NULL, filename) == NULL)
	{
		return 0;
	}
	filehandle = SFS_CreateFile(gHeroCallFullPath, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);
	if(filehandle != SFS_INVALID_HANDLE)
	{
		return filehandle;
	}
	else
	{
		return 0;
	}
#else
	U16 m_path_w[40];
	FS_HANDLE _file_handle =0;
	
	memset(m_path_w, 0, sizeof(m_path_w));
	if(hmgs_get_filename(NULL, filename) == NULL)
	{
		return 0;
	}
	_file_handle = FS_Open((const WCHAR *)m_path_w, mode);
	if(_file_handle < FS_NO_ERROR)
	{
		return 0;
	}
	return _file_handle;
#endif
}

int32 hmgs_read(int32 f,void *p,uint32 l)
{
	uint32 transmit = 0;
	int32 ferrno;
	//FS_Seek(f, 0, FS_FILE_BEGIN);
	//FS_Read(f, p, l, &read_size);
	ferrno = MMIFILE_SetFilePointer(f, 0, SFS_SEEK_BEGIN);
	if(ferrno != SFS_ERROR_NONE)
	{
		MMIFILE_CloseFile(f);
		return -1;
	}
	ferrno = MMIFILE_ReadFile(f, p, l, &transmit, NULL);
	if(ferrno != SFS_ERROR_NONE)
	{
		MMIFILE_CloseFile(f);
		return -1;
	}
	return transmit;
}

int32 hmgs_close(int32 f)
{
	int ret;
	if(f == 0)
	{
		return HR_FAILED;
	}
	ret = MMIFILE_CloseFile(f);
	if (ret == SFS_ERROR_NONE)
	{
		return HR_SUCCESS;
	}
	else
	{
		return HR_FAILED;
	}
}

int32 hmgs_fileState(const char* filename)
{
	//U16 fullpathname[HR_MAX_FILE_LEN] = {0};
	int ret = HMGS_IS_INVALID;
	if(hmgs_get_filename(NULL, filename) == NULL)
	{
		return -1;
	}
	//ret = FS_GetAttributes((WCHAR *)fullpathname);
	if(MMIFILE_IsFileExist(gHeroCallFullPath, hr_str_wstrlen(gHeroCallFullPath)))
	{
		ret = HMGS_IS_FILE;
	}
	else if(MMIFILE_IsFolderExist(gHeroCallFullPath, hr_str_wstrlen(gHeroCallFullPath)))
	{
		ret = HMGS_IS_DIR;
	}
	return ret;
	//if (ret < 0) 
	//	return HMGS_IS_INVALID;
	
	//if (ret & FS_ATTR_DIR) 
	//	return HMGS_IS_DIR;
	//else
	//	return HMGS_IS_FILE;
}
#endif


#define TIHO_MAX_CITY_LEN 14
#define TIHO_MAX_CITY_NUM 512
#define TIHO_MOBILENUMBER_LEAST_LENGTH             7	
#define TIHO_MOBILENUMBER_MOST_LENGTH           11						
#define TIHO_MOBILENUMBER_HEAD_LEN			3
#define TIHO_MOBILENUMBER_AREA_LEN			4
#define MMIFINDMP_CITYNAME_LENGTH	30
#define TIHO_MAX_PROVINCE_LEN 5
#define TIHO_MAX_FILESSPLIT_SIZE 10
/* start modify by matthew 20180120 */
#define TIHO_MOBILENUMBER_COM_SIZE			21  //19
/* end modify by matthew 20180120 */
#define TIHO_MOBILENUMBER_COM_NAME_LEN		16
#define TIHO_MOBILENUMBER_COM_NUM_LEN		5
typedef enum 
{
    TIHO_NUMLOC_SEARCH_NUMBER=0,
    TIHO_NUMLOC_SEARCH_AREA=1,
    TIHO_NUMLOC_SEARCH_MAX
}TIHO_NUMLOC_RESULT_E;

typedef struct  
{
	uint32 tiho_area_code;
	uint16 tiho_city_index;
		
}TIHO_LOC_T;

typedef struct  
{
	unsigned short tiho_city_name[TIHO_MAX_CITY_LEN];
}TIHO_CITY_T;
typedef struct  
{
	uint16 tiho_pnum_area;
	uint16 tiho_pnum_city_index;
		
}TIHO_PHONENUM_LOC_T;


typedef struct  
{
	uint16 tiho_pnum_city_index;
		
}TIHO_PHONENUM_LOC_DISP_T;


typedef struct
{
	BOOL tiho_num_has86;
	TIHO_LOC_T *loc_array;
	uint32 loc_array_size;
	TIHO_CITY_T*city_array;
	uint32 city_array_size;
	TIHO_PHONENUM_LOC_T* num_buff;
	uint32 num_buff_size;
	TIHO_PHONENUM_LOC_DISP_T* tiho_disp_phonenum;
	TIHO_NUMLOC_RESULT_E tiho_search_result;

}NUMLOC_DATA_T;

typedef struct
{
	int code;
	char name[TIHO_MOBILENUMBER_COM_NAME_LEN];
}NUMCOM_DATA_T;


const char tiho_chinacode_str[][2+2+1]=
{
	{'0','0','8','6','\0'},
	{'+','8','6','\0'}
};

const char tiho_prefix_str[][5+1]=
{
	{'1','7','9','5','1','\0'},
	{'1','7','9','0','9','\0'},
	{'1','2','5','9','3','\0'}
};

const NUMCOM_DATA_T tiho_com_num[TIHO_MOBILENUMBER_COM_SIZE]=
{
	{110,"\x6c\x51\x89\x5b\xa5\x62\x66\x8b"},//公安报警
	{120,"\x25\x60\x51\x65"},//急救
	/* start modify by matthew 20180120 */
	{122,"\xA4\x4E\x1A\x90\x8B\x4E\x45\x65"},//交通事故
	{911,"\x27\x7d\x25\x60\xf7\x53\x01\x78"},//紧急号码
	/* end modify by matthew 20180120 */
	{119,"\x6b\x70\x66\x8b"},//火警
	{114,"\x35\x75\xdd\x8b\xe5\x67\xe2\x8b"},//电话查询
	{10086,"\x2d\x4e\xfd\x56\xfb\x79\xa8\x52\xa2\x5b\x0d\x67"},//中国移动客服
	{10010,"\x2d\x4e\xfd\x56\x54\x80\x1a\x90\xa2\x5b\x0d\x67"},//中国联通客服
	{10000,"\x2d\x4e\xfd\x56\x35\x75\xe1\x4f\xa2\x5b\x0d\x67"},//中国电信客服
	{12110,"\x6c\x51\x89\x5b\xed\x77\xe1\x4f\xa5\x62\x66\x8b"},//公安短信报警
	{12121,"\x29\x59\x14\x6c\x84\x98\xa5\x62"},//天气预报
	{12117,"\xa5\x62\xf6\x65\x0d\x67\xa1\x52"},//报时服务
	{12315,"\x88\x6d\x4f\x53\xed\x70\xbf\x7e"},//消协热线
	{12333,"\xb3\x52\xa8\x52\xdd\x4f\x9c\x96\xed\x70\xbf\x7e"},//劳动保障热线
	{12338,"\x87\x59\x73\x59\xf4\x7e\x43\x67\xed\x70\xbf\x7e"},//妇女维权热线
	{95588,"\xe5\x5d\x46\x55\xf6\x94\x4c\x88"},//工商银行
	{95533,"\xfa\x5e\xbe\x8b\xf6\x94\x4c\x88"},//建设银行
	{95599,"\x9c\x51\x1a\x4e\xf6\x94\x4c\x88"},//农业银行
	{95559,"\xa4\x4e\x1a\x90\xf6\x94\x4c\x88"},//交通银行
	{95566,"\x2d\x4e\xfd\x56\xf6\x94\x4c\x88"},//中国银行
	{95555,"\xdb\x62\x46\x55\xf6\x94\x4c\x88"},//招商银行
};


TIHO_PHONENUM_LOC_DISP_T tiho_disp_phonenum;
NUMLOC_DATA_T numlocData;

char * hmgs_strdup (const char *s)
{     
    int len = strlen (s) + 1;
    void *new = hmgs_malloc (len);
    if (new == NULL)
        return NULL;
    return (char *) memcpy (new, s, len);
}
static int32 hmgs_wstrlen(const uint8* str)
{
	int i = 0;
	while(str[i] || str[i+1]) 
		i+=2;
	return i;
}



static uint8* hmgs_wstrcpy(uint8* dst, const uint8* src)
{
	int i = 0;
	while(src[i] || src[i+1]) 
	{
		dst[i] = src[i];
		dst[i+1] = src[i+1];
		i+=2;
	}

	dst[i] = dst[i+1] = 0;
	return dst;
}

static uint8* hmgs_wstrcat(uint8* dst, const uint8* src)
{
	int len = hmgs_wstrlen(dst);
	return hmgs_wstrcpy(dst + len, src);
}


static NUMLOC_DATA_T*getNumLocData(void)
{
	return &numlocData;
}

 static uint16  TIHO_remove_prefix_str(uint8 *number, uint16 numberlen)
{
	uint16 len = numberlen;
	uint16 i=0;
	uint16 lg=0;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	lg=sizeof(tiho_prefix_str)/sizeof(tiho_prefix_str[0]);
	pNumLocData->tiho_num_has86=FALSE;
	//移除17951 17909 12593
	while(i<lg)
	{
		if(0 == strncmp((char*)number,tiho_prefix_str[i],strlen(tiho_prefix_str[i])))
		{
			len = len - strlen(tiho_prefix_str[i]);
			memcpy(number,number+strlen(tiho_prefix_str[i]),len);
			number[len] = 0;	
			break;
		}
		i++;
	}
    
	i=0;
	lg=sizeof(tiho_chinacode_str)/sizeof(tiho_chinacode_str[0]);
    
	//移除0086,+86
	while(i<lg)
	{
		if(0 == strncmp((char*)number,tiho_chinacode_str[i],strlen(tiho_chinacode_str[i]))) 
		{
			len = len-strlen(tiho_chinacode_str[i]);
			memcpy(number,number+strlen(tiho_chinacode_str[i]),len);
			number[len] = 0;
			pNumLocData->tiho_num_has86=TRUE;
			break;
		}
		i++;
	}   
	return len;
}
 

 BOOL TIHO_SearchNumLoc_LocalFixNumber( uint8* number, uint16 length,uint32 size )
{
	uint16 area_code_len=0;	
	uint8 * number_str= NULL;
	uint8 * char_text = NULL;
	uint16 numarea=0;
	uint16 number_str_len=length;
	uint16 locarray_num=0;
	uint16 i=0;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=0xffff;

	
	number_str = (uint8 *)hmgs_malloc(number_str_len+2);
	memset(number_str,0,(number_str_len+2));
	memcpy(number_str,number,number_str_len);
	
	char_text = (uint8 *)hmgs_malloc(number_str_len+1);
	memset(char_text,0,(number_str_len+1));
	
	if(!strncmp((char*)number_str, "00886", 5)
            ||!strncmp((char*)number_str, "00853", 5)
            ||!strncmp((char*)number_str, "00852", 5)
            )
	{
		number_str[0]='3';
		area_code_len=5;
	}
	else if(!strncmp((char*)number_str, "+886", 4)
            ||!strncmp((char*)number_str, "+853", 4)
            ||!strncmp((char*)number_str, "+852", 4)
            )
	{
		memcpy(number_str+2,number_str+1,number_str_len);
		number_str[0]='3';
		number_str[1]='0';		
		number_str_len=number_str_len+1;
		number_str[number_str_len] = 0;	
		area_code_len=5;		
	}
	else if((number_str[1]=='2'||number_str[1]=='1')&&(number_str[0]=='0'))
	{
		//区号长度为3.  
		//如:010,020,021,022,023,024,025,027,028,029等等
		area_code_len=3;
	}
	else
	{
		area_code_len=4;
	}
	memcpy(char_text,number_str,area_code_len);
	numarea=atoi((char*)char_text);	
	locarray_num=size/sizeof(TIHO_LOC_T);//TIHO_GetLocArraySize();	
	for(i=0;i<locarray_num;i++)
	{
		if(numarea==pNumLocData->loc_array[i].tiho_area_code)
		{
			pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=pNumLocData->loc_array[i].tiho_city_index;
			break;
		}
	}
	hmgs_free(number_str);
	number_str = NULL; 	
	hmgs_free(char_text);
	char_text = NULL; 	
	if(i>=locarray_num)
		return FALSE;
	else
		return TRUE;
	
}
   
 static BOOL TIHO_SearchNumLoc_MobileNumber_array(TIHO_PHONENUM_LOC_T * p_array,uint16 numarea,uint16 array_num)
{
	uint16 low_index=0;
	uint16 mid_index=0;
	uint16 high_index=array_num-1;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
      if(array_num==0)
      {
          return FALSE;
      }
	while(low_index<=high_index)
	{
		mid_index=(low_index+high_index)/2;
		if(p_array[mid_index].tiho_pnum_area==numarea)
		{
			pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=p_array[mid_index].tiho_pnum_city_index;
			return TRUE;
		}
		else if(p_array[mid_index].tiho_pnum_area>numarea)
		{
			high_index=mid_index-1;
			if(high_index>=0)
			{
				if(p_array[high_index].tiho_pnum_area<numarea)
				{
					pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=p_array[high_index].tiho_pnum_city_index;
					return TRUE;
				}
			}
		}
		else
		{
			low_index=mid_index+1;
			if(low_index<array_num)
			{
				if(p_array[low_index].tiho_pnum_area>numarea)
				{
					pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=p_array[mid_index].tiho_pnum_city_index;
					return TRUE;				
				}
			}
		}
	}
	return FALSE;
}
 static BOOL TIHO_SearchNumLoc_MobileNumber( uint8* number, uint16 length ,uint32 size)
{
	BOOL res = FALSE;
	uint8 * head_char_text = NULL;
	uint8 * char_text = NULL;
	uint16 numhead=0;
	uint16 numarea=0;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	
	pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index=0xffff;
	
	head_char_text = (uint8 *)hmgs_malloc(TIHO_MOBILENUMBER_AREA_LEN+1);
	memset(head_char_text,0,(TIHO_MOBILENUMBER_AREA_LEN+1));	
	char_text = (uint8 *)hmgs_malloc(length+1);
	memset(char_text,0,(length+1));
	memcpy(head_char_text,number,TIHO_MOBILENUMBER_HEAD_LEN);
	numhead=atoi((char*)head_char_text);	
	memcpy(char_text,number+TIHO_MOBILENUMBER_HEAD_LEN,TIHO_MOBILENUMBER_AREA_LEN);
	numarea=atoi((char*)char_text);


       res=TIHO_SearchNumLoc_MobileNumber_array((TIHO_PHONENUM_LOC_T*)pNumLocData->num_buff,numarea,size/sizeof(TIHO_PHONENUM_LOC_T));
	   if(pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index==0xffff)
	   {
	  	      res=FALSE;
	   }	
	

	hmgs_free(char_text);
	char_text = NULL; 	
	hmgs_free(head_char_text);
	head_char_text = NULL; 	
	return res;
}
static BOOL Tiho_SearchNumLoc(uint8* text, uint16 length,uint32 size1,uint32 size2,uint32 size3,BOOL isMobileNum)
{
	uint8   index = 0;
	uint8   offset = 0;
	BOOL is_digits = TRUE;
	BOOL recode = FALSE;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	uint8* char_text =(uint8*) hmgs_strdup((char*)text);

	if(text[0]=='+')
	{
		for( index = 1; index < length; index++ )
		{
			if( ( text[index] < '0') || ( text[index] > '9') )
			{
				is_digits = FALSE;
				break;
			}
		}
	}
	else
	{
		for( index = 0; index < length; index++ )
		{
			if( ( text[index] < '0') || ( text[index] > '9') )
			{
				is_digits = FALSE;
				break;
			}
		}
	}

	if(is_digits)
	{
		//MMI_WSTRNTOSTR(char_text, length, text, length, length);
		length=TIHO_remove_prefix_str(char_text,length);
		if(isMobileNum)
		{
			//手机号码查询范围7到11位
			if((length>=TIHO_MOBILENUMBER_LEAST_LENGTH)&&(length<=TIHO_MOBILENUMBER_MOST_LENGTH))
			{
				recode=TIHO_SearchNumLoc_MobileNumber(char_text, length,size1);
			}
		}
		else
		{
			//固定电话
			if(pNumLocData->tiho_num_has86)
			{
				memcpy(char_text+1,char_text,length);
				char_text[0]='0';
				length=length+1;
				char_text[length]=0;
			}
			//考虑到台湾的情况国际号5位，3位区号，8位号码所以定长度为16
			if(length<=16)
			{
				recode=TIHO_SearchNumLoc_LocalFixNumber(char_text, length,size2);
			}	
		}

		if(recode)
			pNumLocData->tiho_search_result=TIHO_NUMLOC_SEARCH_NUMBER;
	}
	else
	{
		//非数字情况
//		recode=TIHO_SearchNumLoc_CityName(text,length,size3,size2);
//		if(recode)
//			pNumLocData->tiho_search_result=TIHO_NUMLOC_SEARCH_AREA;
	}
//	tiho_hmgs_big_free(loc_array);
//	tiho_hmgs_big_free(city_array);
//	tiho_hmgs_big_free(num_buff);
	hmgs_free(char_text);
    return (recode);
}

static BOOL Tiho_SearchComNum(uint8* text,uint8* result_str)
{
	int i;
	int code =atoi((char*)text);
	int isFind =FALSE;
	for(i=0;i<TIHO_MOBILENUMBER_COM_SIZE;i++)
	{
        if(strncmp(text, "0911", 4) == 0)
        {
			break;
		}
		else if(code==tiho_com_num[i].code)
		{
			hmgs_wstrcat(result_str,(const uint8*)tiho_com_num[i].name);
			#ifdef TOO_MODE
			str_convert_endian(result_str);
			#endif
			isFind =TRUE;
			break;
		}
	} 
	return isFind;
}


void hmgs_read_Loc(char * filename,void* array,unsigned long int* size,int32 stSize,int32 stRealSize)
{
	int32 fileSize =hmgs_getLen((const char *) filename);
	if(fileSize > 0){
		int32 i = 0;
		int32 n = fileSize /stRealSize;
		uint8 * arrayCur =array;
		int32 f = hmgs_open((const char *)filename,HMGS_FILE_RDONLY);
		if(f!=0){
			uint8* buffer = hmgs_malloc(fileSize);
			if(buffer!=NULL){
				uint8 * bufferCur = buffer;
				hmgs_read(f,buffer,fileSize);
				for(i=0;i<n;i++){
					memcpy(arrayCur,bufferCur,stRealSize);
					arrayCur+=stSize;
					bufferCur+=stRealSize;
				}
				*size = n*stSize;
				hmgs_free(buffer);
			}
			hmgs_close(f);
		}
	}
}

void hmgs_read_FirmArrayLoc(void* firmArray,int32 firmArrayLen,void* array,unsigned long int* size,int32 stSize,int32 stRealSize)
{
	if(firmArrayLen > 0){
		int32 i = 0;
		int32 n = firmArrayLen /stRealSize;
		uint8 * arrayCur =array;
		uint8* buffer = hmgs_malloc(firmArrayLen);
		if(buffer!=NULL){
			uint8 * bufferCur = buffer;
			memcpy(buffer,firmArray,firmArrayLen);
			for(i=0;i<n;i++){
				memcpy(arrayCur,bufferCur,stRealSize);
				arrayCur+=stSize;
				bufferCur+=stRealSize;
			}
			*size = n*stSize;
			hmgs_free(buffer);
		}	
	}
}




void hmgs_read_cityData(void* array,unsigned long int* size,int32 stSize,uint8* fileBuffer,int32 fileSize)
{
	uint8 * bufferCur = fileBuffer;
	int32 remainSize = fileSize;
	int32 n = 0;
	int32 i = 0;
	uint8 * arrayCur =array;
	for(n=0; n<remainSize && (((i + 1) * stSize) < TIHO_MAX_CITY_NUM*sizeof(TIHO_CITY_T)); )
	{
		if(*(bufferCur+n)=='\x0A'
			&&*(bufferCur+n+1)=='\x00')
		{
			memcpy(arrayCur,bufferCur,MIN(n, stSize));
			bufferCur = bufferCur+(n+2);
			arrayCur = arrayCur+stSize;
			remainSize = remainSize-n-2;
			n = 0;
			i++;
		}else{
			n+=2;
		}
	}
	*size=i*stSize;
}

void hmgs_read_FirmArrayCity(void* firmArray,int32 firmArrayLen,void* array,unsigned long int* size,int32 stSize)
{
	if(firmArrayLen > 0){
		uint8* fileBuffer = hmgs_malloc(firmArrayLen);
		if(fileBuffer!=NULL)
		{
			memcpy(fileBuffer,firmArray,firmArrayLen);
			hmgs_read_cityData(array,size,stSize,fileBuffer,firmArrayLen);
			hmgs_free(fileBuffer);
		}
	}	
}
void hmgs_read_city(char * filename,void* array,unsigned long int* size,int32 stSize)
{
	int32 fileSize =hmgs_getLen((const char *) filename);
	if(fileSize > 0){
		int32 f = hmgs_open((const char *)filename,HMGS_FILE_RDONLY);
		if(f!=0){
			uint8* fileBuffer = hmgs_malloc(fileSize);
			if(fileBuffer!=NULL){
				hmgs_read(f,fileBuffer,fileSize);
				hmgs_read_cityData(array,size,stSize,fileBuffer,fileSize);
				hmgs_free(fileBuffer);
			}
			hmgs_close(f);
		}
	}
}


void transform24byte(TIHO_PHONENUM_LOC_T* tmpArray,uint8 * bufferCur)
{
	tmpArray[0].tiho_pnum_area = (bufferCur[0]>>4)&0xf;
	tmpArray[1].tiho_pnum_area = bufferCur[0]&0xf;
	tmpArray[2].tiho_pnum_area = (bufferCur[1]>>4)&0xf;
	tmpArray[3].tiho_pnum_area = bufferCur[1]&0xf;
	tmpArray[4].tiho_pnum_area = (bufferCur[2]>>4)&0xf;
	tmpArray[5].tiho_pnum_area = bufferCur[2]&0xf;
	tmpArray[6].tiho_pnum_area = (bufferCur[3]>>4)&0xf;
	tmpArray[7].tiho_pnum_area = bufferCur[3]&0xf;
	tmpArray[0].tiho_pnum_city_index = (bufferCur[4]<<1|bufferCur[5]>>7)&0x1ff;
	tmpArray[1].tiho_pnum_city_index = (bufferCur[5]<<2|bufferCur[6]>>6)&0x1ff;
	tmpArray[2].tiho_pnum_city_index = (bufferCur[6]<<3|bufferCur[7]>>5)&0x1ff;
	tmpArray[3].tiho_pnum_city_index = (bufferCur[7]<<4|bufferCur[8]>>4)&0x1ff;
	tmpArray[4].tiho_pnum_city_index = (bufferCur[8]<<5|bufferCur[9]>>3)&0x1ff;
	tmpArray[5].tiho_pnum_city_index = (bufferCur[9]<<6|bufferCur[10]>>2)&0x1ff;
	tmpArray[6].tiho_pnum_city_index = (bufferCur[10]<<7|bufferCur[11]>>1)&0x1ff;
	tmpArray[7].tiho_pnum_city_index = (bufferCur[11]<<8|bufferCur[12])&0x1ff;
}


void hmgs_read_numData(TIHO_PHONENUM_LOC_T* array,unsigned long int* size,uint8* fileBuffer,int32 fileSize)
{
	uint8 * bufferCur = fileBuffer;
	int32 i = 0;
	int32 num = 0;
	uint16 area = 0;
	int n;
	TIHO_PHONENUM_LOC_T tmpArray[8] = {0};
	for(n = 0;n<fileSize;)
	{
		transform24byte(tmpArray,bufferCur);
		for(i=0;i<8;i++){
			area+=tmpArray[i].tiho_pnum_area;
			array[num+i].tiho_pnum_area = area;
			array[num+i].tiho_pnum_city_index = tmpArray[i].tiho_pnum_city_index;
		}
		num+=8;
		bufferCur+=13;
		n+=13;
	}
	*size = num*sizeof(TIHO_PHONENUM_LOC_T);
}


void hmgs_read_FirmArrayNum(void* firmArray,int32 firmArrayLen,TIHO_PHONENUM_LOC_T* array,unsigned long int* size)
{
	if(firmArrayLen > 0){

		uint8* fileBuffer = hmgs_malloc(firmArrayLen);
		if(fileBuffer!=NULL)
		{
			memcpy(fileBuffer,firmArray,firmArrayLen);
			hmgs_read_numData(array,size,fileBuffer,firmArrayLen);
			hmgs_free(fileBuffer);
		}
	}
}

void hmgs_read_Num(char * filename,TIHO_PHONENUM_LOC_T* array,unsigned long int* size)
{
	int32 fileSize =hmgs_getLen((const char *) filename);
	if(fileSize > 0){
		int32 f = hmgs_open((const char *)filename,HMGS_FILE_RDONLY);
		
		if(f!=0){
			uint8* fileBuffer = hmgs_malloc(fileSize);
			if(fileBuffer!=NULL)
			{
				hmgs_read(f,fileBuffer,fileSize);
				hmgs_read_numData(array,size,fileBuffer,fileSize);
				hmgs_free(fileBuffer);
			}
			hmgs_close(f);
		}
	}
}

#if defined(FIRM_HMGS_MIN_ROM)||defined(FIRM_HMGS_FULL_ROM)
typedef struct  
{
	char header[4];
	uint8* buf;	
	int32 len;
}Numhead_T;

void getNumFirmArray(char* numheadstr,uint8** firmArry,int32* firmArrylen)
{
	if(numheadstr){
		uint16 header =atoi(numheadstr);
		switch(header)
		{
			case 130:
				*firmArry = (uint8 *)too_130Data;
				*firmArrylen = sizeof(too_130Data);
				break;
			case 131:
				*firmArry = (uint8 *)too_131Data;
				*firmArrylen = sizeof(too_131Data);
				break;
			case 132:
				*firmArry = (uint8 *)too_132Data;
				*firmArrylen = sizeof(too_132Data);
				break;
			case 133:
				*firmArry = (uint8 *)too_133Data;
				*firmArrylen = sizeof(too_133Data);
				break;
			case 134:
				*firmArry = (uint8 *)too_134Data;
				*firmArrylen = sizeof(too_134Data);
				break;
			case 135:
				*firmArry = (uint8 *)too_135Data;
				*firmArrylen = sizeof(too_135Data);
				break;
			case 136:
				*firmArry = (uint8 *)too_136Data;
				*firmArrylen = sizeof(too_136Data);
				break;
			case 137:
				*firmArry = (uint8 *)too_137Data;
				*firmArrylen = sizeof(too_137Data);
				break;
			case 138:
				*firmArry = (uint8 *)too_138Data;
				*firmArrylen = sizeof(too_138Data);
				break;
			case 139:
				*firmArry = (uint8 *)too_139Data;
				*firmArrylen = sizeof(too_139Data);
				break;
#ifdef FIRM_HMGS_FULL_ROM				
			case 141:
				*firmArry = (uint8 *)too_141Data;
				*firmArrylen = sizeof(too_141Data);
				break;
			case 145:
				*firmArry = (uint8 *)too_145Data;
				*firmArrylen = sizeof(too_145Data);
				break;
			case 146:
				*firmArry = (uint8 *)too_146Data;
				*firmArrylen = sizeof(too_146Data);
			break;
			case 147:
				*firmArry = (uint8 *)too_147Data;
				*firmArrylen = sizeof(too_147Data);
				break;
			case 148:
				*firmArry = (uint8 *)too_148Data;
				*firmArrylen = sizeof(too_148Data);
				break;
			case 149:
				*firmArry = (uint8 *)too_149Data;
				*firmArrylen = sizeof(too_149Data);
				break;
			case 154:
				*firmArry = (uint8 *)too_154Data;
				*firmArrylen = sizeof(too_154Data);
				break;
			case 160:
				*firmArry = (uint8 *)too_160Data;
				*firmArrylen = sizeof(too_160Data);
				break;
			case 162:
				*firmArry = (uint8 *)too_162Data;
				*firmArrylen = sizeof(too_162Data);
				break;
			case 165:
				*firmArry = (uint8 *)too_165Data;
				*firmArrylen = sizeof(too_165Data);
				break;
			case 166:
				*firmArry = (uint8 *)too_166Data;
				*firmArrylen = sizeof(too_166Data);
				break;
			case 167:
				*firmArry = (uint8 *)too_167Data;
				*firmArrylen = sizeof(too_167Data);
				break;
			case 171:
				*firmArry = (uint8 *)too_171Data;
				*firmArrylen = sizeof(too_171Data);
				break;
			case 172:
				*firmArry = (uint8 *)too_172Data;
				*firmArrylen = sizeof(too_172Data);
				break;
			case 173:
				*firmArry = (uint8 *)too_173Data;
				*firmArrylen = sizeof(too_173Data);
				break;
			case 174:
				*firmArry = (uint8 *)too_174Data;
				*firmArrylen = sizeof(too_174Data);
				break;
			case 190:
				*firmArry = (uint8 *)too_190Data;
				*firmArrylen = sizeof(too_190Data);
				break;
			case 191:
				*firmArry = (uint8 *)too_191Data;
				*firmArrylen = sizeof(too_191Data);
				break;
			case 192:
				*firmArry = (uint8 *)too_192Data;
				*firmArrylen = sizeof(too_192Data);
				break;
			case 193:
				*firmArry = (uint8 *)too_193Data;
				*firmArrylen = sizeof(too_193Data);
				break;
			case 194:
				*firmArry = (uint8 *)too_194Data;
				*firmArrylen = sizeof(too_194Data);
				break;
			case 195:
				*firmArry = (uint8 *)too_195Data;
				*firmArrylen = sizeof(too_195Data);
				break;
			case 196:
				*firmArry = (uint8 *)too_196Data;
				*firmArrylen = sizeof(too_196Data);
				break;
#endif			
			case 150:
				*firmArry = (uint8 *)too_150Data;
				*firmArrylen = sizeof(too_150Data);
				break;
			case 151:
				*firmArry = (uint8 *)too_151Data;
				*firmArrylen = sizeof(too_151Data);
				break;
			case 152:
				*firmArry = (uint8 *)too_152Data;
				*firmArrylen = sizeof(too_152Data);
				break;
			case 153:
				*firmArry = (uint8 *)too_153Data;
				*firmArrylen = sizeof(too_153Data);
				break;
			case 155:
				*firmArry = (uint8 *)too_155Data;
				*firmArrylen = sizeof(too_155Data);
				break;
			case 156:
				*firmArry = (uint8 *)too_156Data;
				*firmArrylen = sizeof(too_156Data);
				break;
			case 157:
				*firmArry = (uint8 *)too_157Data;
				*firmArrylen = sizeof(too_157Data);
				break;
			case 158:
				*firmArry = (uint8 *)too_158Data;
				*firmArrylen = sizeof(too_158Data);
				break;
			case 159:
				*firmArry = (uint8 *)too_159Data;
				*firmArrylen = sizeof(too_159Data);
				break;
#ifdef FIRM_HMGS_FULL_ROM			
			case 170:
				*firmArry = (uint8 *)too_170Data;
				*firmArrylen = sizeof(too_170Data);
				break;
			case 175:
				*firmArry = (uint8 *)too_175Data;
				*firmArrylen = sizeof(too_175Data);
				break;
			case 176:
				*firmArry = (uint8 *)too_176Data;
				*firmArrylen = sizeof(too_176Data);
				break;
			case 177:
				*firmArry = (uint8 *)too_177Data;
				*firmArrylen = sizeof(too_177Data);
				break;
			case 178:
				*firmArry = (uint8 *)too_178Data;
				*firmArrylen = sizeof(too_178Data);
				break;
#endif
			case 180:
				*firmArry = (uint8 *)too_180Data;
				*firmArrylen = sizeof(too_180Data);
				break;
			case 181:
				*firmArry = (uint8 *)too_181Data;
				*firmArrylen = sizeof(too_181Data);
				break;
			case 182:
				*firmArry = (uint8 *)too_182Data;
				*firmArrylen = sizeof(too_182Data);
				break;
			case 183:
				*firmArry = (uint8 *)too_183Data;
				*firmArrylen = sizeof(too_183Data);
				break;		
			case 184:
				*firmArry = (uint8 *)too_184Data;
				*firmArrylen = sizeof(too_184Data);
				break;		
			case 185:
				*firmArry = (uint8 *)too_185Data;
				*firmArrylen = sizeof(too_185Data);
				break;
			case 186:
				*firmArry = (uint8 *)too_186Data;
				*firmArrylen = sizeof(too_186Data);
				break;
			case 187:
				*firmArry = (uint8 *)too_187Data;
				*firmArrylen = sizeof(too_187Data);
				break;
			case 188:
				*firmArry = (uint8 *)too_188Data;
				*firmArrylen = sizeof(too_188Data);
				break;
			case 189:
				*firmArry = (uint8 *)too_189Data;
				*firmArrylen = sizeof(too_189Data);
				break;
			case 198:
				*firmArry = (uint8 *)too_198Data;
				*firmArrylen = sizeof(too_198Data);
				break;
			case 199:
				*firmArry = (uint8 *)too_199Data;
				*firmArrylen = sizeof(too_199Data);
				break;
		}

	}
}
#endif

int hmgs_init(void)
{
	int isSuccess = FALSE;
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	hmgs_deinit();
	do{
		//mrc_printf("hmgs_init loc=%d city=%d num=%d",sizeof(TIHO_LOC_T),sizeof(TIHO_CITY_T),sizeof(TIHO_PHONENUM_LOC_T));
		pNumLocData->loc_array = hmgs_malloc(327*sizeof(TIHO_LOC_T));
		if(pNumLocData->loc_array==NULL)
			break;
		memset(pNumLocData->loc_array,0,327*sizeof(TIHO_LOC_T));
		
		pNumLocData->city_array = hmgs_malloc(TIHO_MAX_CITY_NUM*sizeof(TIHO_CITY_T));
		if(pNumLocData->city_array==NULL)
			break;
		memset(pNumLocData->city_array,0,TIHO_MAX_CITY_NUM*sizeof(TIHO_CITY_T));

		pNumLocData->num_buff =hmgs_malloc(6000*sizeof(TIHO_PHONENUM_LOC_T));
		if(pNumLocData->num_buff==NULL)
			break;
		memset(pNumLocData->num_buff,0,6000*sizeof(TIHO_PHONENUM_LOC_T));
		
		pNumLocData->tiho_disp_phonenum = &tiho_disp_phonenum;
		isSuccess = TRUE;
	}while(0);
	return isSuccess;
}

void get_hmgs_buff(char* number,uint8* strbuf)
{
	uint16 numhead=0;
	char path[48]="";
	uint32 size1=0;
	uint32 size2=0;
	uint32 size3=0;
	char*numtmp = hmgs_strdup(number);
	char numheadstr[TIHO_MOBILENUMBER_HEAD_LEN+1] = {0};
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	BOOL isMobileNum =FALSE;
	TIHO_remove_prefix_str((uint8 *)numtmp,strlen(numtmp));

	if(strlen(numtmp)>0&&strlen(numtmp)<=TIHO_MOBILENUMBER_COM_NUM_LEN)
	{
		if(Tiho_SearchComNum((uint8*)numtmp,strbuf))
		{
			hmgs_free(numtmp);
			return;
		}		
	}

	if(strlen(numtmp) > 0 && ((strncmp(numtmp, "178377", 6)==0) || (strncmp(numtmp, "86178377", 8)==0))){
		hmgs_wstrcat(strbuf,(const uint8*)"\xb3\x6c\x57\x53\x57\x53\x33\x96\x00\x00");
		hmgs_free(numtmp);
		return;
	}
	
	if(strlen(numtmp) > 0 && ((strncmp(numtmp, "198835", 6)==0) || (strncmp(numtmp, "86198835", 8)==0)))
	{
		hmgs_wstrcat(strbuf,(const uint8*)"\x59\x6d\x5f\x6c\xcd\x7e\x74\x51\x00\x00");
		hmgs_free(numtmp);
		return;
	}
	if(strlen(numtmp)<TIHO_MOBILENUMBER_HEAD_LEN){
		hmgs_free(numtmp);
		return;
	}
	memcpy(numheadstr,numtmp,TIHO_MOBILENUMBER_HEAD_LEN);


	memset(path,0,20);
	
	strcpy(path,TIHO_MOBILENUMBER_PATH);
	strcat(path,"too_loc");
	#if defined(FIRM_HMGS_MIN_ROM) || defined(FIRM_HMGS_FULL_ROM)
		if(hmgs_fileState(path)==HMGS_IS_FILE)
		{
			hmgs_read_Loc(path,(void*)pNumLocData->loc_array,&pNumLocData->loc_array_size,sizeof(TIHO_LOC_T),6);		
		}else
		{
			hmgs_read_FirmArrayLoc((void*)too_locData,sizeof(too_locData),(void*)pNumLocData->loc_array,&pNumLocData->loc_array_size,sizeof(TIHO_LOC_T),6);
		}
	#else
		hmgs_read_Loc(path,(void*)pNumLocData->loc_array,&pNumLocData->loc_array_size,sizeof(TIHO_LOC_T),6);	
	#endif
	  

	memset(path,0,20);
	strcpy(path,TIHO_MOBILENUMBER_PATH);
	strcat(path,"too_city");
	#if defined(FIRM_HMGS_MIN_ROM) || defined(FIRM_HMGS_FULL_ROM)
		if(hmgs_fileState(path)==HMGS_IS_FILE)
		{
			hmgs_read_city(path,(void*)pNumLocData->city_array,&pNumLocData->city_array_size,sizeof(TIHO_CITY_T));
		}else
		{
			hmgs_read_FirmArrayCity((void*)too_cityData,sizeof(too_cityData),(void*)pNumLocData->city_array,&pNumLocData->city_array_size,sizeof(TIHO_CITY_T));
		}
	#else
		hmgs_read_city(path,(void*)pNumLocData->city_array,&pNumLocData->city_array_size,sizeof(TIHO_CITY_T));
	#endif
	
	
	memset(path,0,20);
	sprintf(path,"%s%s%s",TIHO_MOBILENUMBER_PATH,"too_",numheadstr);
	memset(pNumLocData->num_buff,0,sizeof(pNumLocData->num_buff));
	pNumLocData->num_buff_size = 0;

	#if defined(FIRM_HMGS_MIN_ROM) || defined(FIRM_HMGS_FULL_ROM)
		if(hmgs_fileState(path)==HMGS_IS_FILE)
		{
			hmgs_read_Num(path,pNumLocData->num_buff,&pNumLocData->num_buff_size);
		}else
		{	
			uint8* firmArry =NULL;
			int32 firmArrylen =0;
			getNumFirmArray(numheadstr,&firmArry,&firmArrylen);
			hmgs_read_FirmArrayNum(firmArry,firmArrylen,pNumLocData->num_buff,&pNumLocData->num_buff_size);
		}
	#else
		hmgs_read_Num(path,pNumLocData->num_buff,&pNumLocData->num_buff_size);
	#endif
	if(pNumLocData->num_buff_size>0){
		isMobileNum =TRUE;
	}
	
	if(pNumLocData->num_buff_size<0||pNumLocData->loc_array_size<0||pNumLocData->city_array_size<0){
		hmgs_free(numtmp);
		return;
	}
	if(Tiho_SearchNumLoc((uint8*)number,strlen(number),pNumLocData->num_buff_size,pNumLocData->loc_array_size,pNumLocData->city_array_size,isMobileNum))
	{

	//	uint8 result_str[32]={0};
		uint8* result_str;
		result_str = strbuf;
		if(pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index != 0xffff)
		{
			
			hmgs_wstrcat(result_str,(const uint8*)pNumLocData->city_array[pNumLocData->tiho_disp_phonenum->tiho_pnum_city_index].tiho_city_name);
		}
#ifdef TOO_MODE
		 str_convert_endian(result_str);
#endif
		hmgs_free(numtmp);
		return;
	}

//	tiho_disp_phonenum.tiho_pnum_pro_index = 0xffff;
	hmgs_free(numtmp);
	
}

void hmgs_deinit(void)
{
	NUMLOC_DATA_T* pNumLocData = getNumLocData();
	if(pNumLocData->loc_array)
	{
		hmgs_free(pNumLocData->loc_array);
		pNumLocData->loc_array = NULL;
	}
	if(pNumLocData->city_array)
	{
		hmgs_free(pNumLocData->city_array);
		pNumLocData->city_array =NULL;
	}
	if(pNumLocData->num_buff)
	{
		hmgs_free(pNumLocData->num_buff);
		pNumLocData->num_buff =NULL;
	}
}

void LocationData_Init_ex(void)
{
	hmgs_init();
}

void LocationData_deInit_ex(void)
{
	hmgs_deinit();
}

int GetLocation_ex( char *area, int size, char *tel )
{
	get_hmgs_buff(tel,area);
	hccall_printf("[inkleak.zhao] GetLocation_ex  tel = %s, area = %s", tel, area);
	return 1;
}

#ifdef HERO_INCOMECALL_APP_LOGOPEN
static char hccall_pintfBuf[256];
void hccall_printf(const char *format,...)
{
    char *ptr;
    va_list params;
    int32 fHandle = 0;
    int32 ret = 0;
    int32 writen = 0;

    {
    	memset(hccall_pintfBuf, 0, sizeof(hccall_pintfBuf));
    	strcpy(hccall_pintfBuf, "\n* HR log: ");
    	ptr = hccall_pintfBuf + sizeof("\n* HR log: ") - 1;
    	va_start(params, format);
#ifndef WIN32
    	vsnprintf(ptr, sizeof(hccall_pintfBuf) - sizeof("\n* HR log: ") -1, format, params); 
#else
    	//vsprintf(ptr, format, params); 
    	_vsnprintf(ptr, sizeof(hccall_pintfBuf) - sizeof("\n* HR log: ") -1, format, params); 
#endif
    	fHandle = SFS_CreateFile(L"E:\\heroEngineSpr\\spr_call.txt", SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, PNULL, PNULL);
    	if(fHandle != 0)
        {
            SFS_SetFilePointer(fHandle, 0, SFS_SEEK_END);
            SFS_WriteFile(fHandle, (void*)hccall_pintfBuf, sizeof(hccall_pintfBuf), &writen, PNULL);
            SFS_CloseFile(fHandle);
        }   
    	va_end(params);	
    }
}
#else
void hccall_printf(const char *format,...)
{
	//nothing
}
#endif

#endif //FEATURE_SUPPORT_HERO_HCCALL
