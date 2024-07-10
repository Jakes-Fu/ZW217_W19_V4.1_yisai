#ifdef WIN32    
#ifdef __cplusplus
    extern   "C"
    {
#endif
		
#include <windows.h>
#include <windef.h>
#include <crtdbg.h>
#include "msk_api_gui.h"

HANDLE g_hFile = INVALID_HANDLE_VALUE;
HANDLE g_hMap  = NULL;

__declspec(dllexport) char g_file_buff[255];


void GetMMIResEntry( LPBYTE * pEntry)
{
	g_hFile = CreateFile(g_file_buff, GENERIC_READ, 
                                 0,  NULL, OPEN_EXISTING, 
                                 FILE_ATTRIBUTE_NORMAL, NULL );

    _ASSERTE( g_hFile != INVALID_HANDLE_VALUE );
        
    g_hMap = CreateFileMapping( g_hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    _ASSERTE( g_hMap != NULL );

    *pEntry = (LPBYTE)MapViewOfFile( g_hMap, FILE_MAP_READ, 0, 0, 0 );
    _ASSERTE( *pEntry != NULL );
}

void ReleaseMMIRes( LPBYTE pEntry )
{
    if( pEntry != NULL )
        UnmapViewOfFile(pEntry);
        
    CloseHandle(g_hMap);
    CloseHandle(g_hFile);
}

#ifdef MMI_RES_DYN_LOAD_SUPPORT

#include "bzp_api.h"

typedef struct SCI_BZP_FILE_HEAD_Tag
{
    uint32  magic;
	  uint32  version;
	  uint32  block_head_pos;
    uint32  block_num;
}SCI_BZP_FILE_HEAD_T;

typedef struct SCI_BZP_BLOCK_HEAD_Tag
{
	uint32  magic;
	uint32  type; 
  uint32  start_pos;
	uint32  block_len;
	uint32  des_addr;
}SCI_BZP_BLOCK_HEAD_T;

typedef struct BZP_PARAM_Tag
{
	 uint8*  zip_buf_ptr;
	 uint32  zip_buf_len;
	 uint8*  unzip_buf_ptr;
	 uint32  unzip_buf_len; 
}BZP_PARAM_T;

#define GZIP_FILE_FLAG     0x1F8B0800
#define LZMA_FILE_FLAG     0x5D000004

#define BZP_BLOCK_MAGIC   0x424C4F43      //BLOC
#define BZP_HEAD_MAGIC    0x53505252      //

#define BZP_UNZIP_CODE    0x636F6465UL
#define BZP_UNZIP_RES     0x7253736FUL
#define BZP_UNZIP_LANG      0x6C616E67UL
#define PSRAM_LIMIT_SIZE    0x400000

#define  BZP_DEOCDER_CACHE_SIZE  (300*1024)

LOCAL  uint32 _GetBlockType(BZP_TYPE  type)
{
    uint32 block_type = 0;
	 
    switch(type)	
    {
        case BZP_TYPE_CODE:
        {
        	block_type = BZP_UNZIP_CODE;
        	break;	
        }	
        case BZP_TYPE_RES:
        {
        	block_type = BZP_UNZIP_RES;
        	break;	
        }
        case BZP_TYPE_LANG:
        {
            block_type = BZP_UNZIP_LANG;
            break;
        }    
        default:
        {
        	block_type = 0;
        	break;	
        }	 	 	
    }	 
	return block_type;
}

/*****************************************************************************/
//  Description:    The function BZP_api is used to get the Res, Code or LANG 
//                  BLOCK address for BZIP format data.
//	Global resource dependence: 
//  Author:         
//	Note:       
/*****************************************************************************/
PUBLIC BZP_STATUS BZP_api(
    const  BZP_IN_PARAM_T* in_ptr,  //in: type-Res/Code/Lang, idx-Lang index, param-extend parametar
    BZP_OUT_PARAM_T* out_ptr        //out:unzip_buf_ptr-Address for upper layer, unzip_size-data size
    )
{
    uint8* BZP_file_ptr = (uint8*)in_ptr->param;
    SCI_BZP_FILE_HEAD_T*   BZP_file_head_ptr = NULL;
    SCI_BZP_BLOCK_HEAD_T*  block_head_ptr = NULL;
    uint8*  head_buf_ptr = NULL;
    uint32 cnt = 0;
    BZP_PARAM_T   bzp_param = {0};
    BZP_STATUS ret = BZP_BLOCK_ERROR;
    uint8* gzip_buf_ptr = NULL;
    uint8* block_data_ptr = NULL;
    uint32 unzip_real_size = 0;
    uint32 zip_file_type = 0;

    BZP_file_head_ptr = (SCI_BZP_FILE_HEAD_T*)BZP_file_ptr;   
    if(BZP_HEAD_MAGIC != BZP_file_head_ptr->magic)
        return  BZP_FILE_ERROR;

    block_head_ptr = (SCI_BZP_BLOCK_HEAD_T*)(BZP_file_ptr + BZP_file_head_ptr->block_head_pos);
    for(cnt = 0; cnt < BZP_file_head_ptr->block_num; cnt++)
    {
        if(BZP_BLOCK_MAGIC != block_head_ptr->magic)
        {
        	return  BZP_BLOCK_ERROR;
        }

        if(BZP_TYPE_LANG == in_ptr->type)
        {
            if((_GetBlockType(BZP_TYPE_LANG) == block_head_ptr->type) 
                && in_ptr->idx == block_head_ptr->des_addr)
            {
                ret = BZP_SUCCESS;
                break;
            }

        }
        else if(BZP_TYPE_RES == in_ptr->type || (BZP_TYPE_CODE == in_ptr->type))
        {   
            if(_GetBlockType(in_ptr->type) == block_head_ptr->type) 
            {
                ret = BZP_SUCCESS;
                break;
            }
        }   

        block_head_ptr++;
    }

    if(BZP_SUCCESS != ret)
    {
        return BZP_BLOCK_ERROR;
    }
    
    out_ptr->unzip_buf_ptr = block_head_ptr->start_pos + (uint8*)in_ptr->param;
    out_ptr->unzip_size = block_head_ptr->block_len;

    return ret;
}

#if 0
HANDLE g_hFile1 = INVALID_HANDLE_VALUE;
HANDLE g_hMap1  = NULL;
char   *s_language_file_ptr = "D:\\cheng.luo_09a.w16.06.view\\MS_Code\\build\\sc6600l_6620_32X32\\img\\mmi_res_220x176_lang_English_win.bin";
void GetLanguageResEntry( LPBYTE * pEntry)
{
	if (NULL != *pEntry)
	{
		ReleaseMMIRes(*pEntry);
		*pEntry = NULL;
	}
	
	g_hFile1 = CreateFile(s_language_file_ptr, GENERIC_READ, 
		0,  NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL );
	
    _ASSERTE( g_hFile1 != INVALID_HANDLE_VALUE );
	
    g_hMap1 = CreateFileMapping( g_hFile1, NULL, PAGE_READONLY, 0, 0, NULL );
    _ASSERTE( g_hMap1 != NULL );
	
    *pEntry = (LPBYTE)MapViewOfFile( g_hMap1, FILE_MAP_READ, 0, 0, 0 );
    _ASSERTE( *pEntry != NULL );
}
#endif
#endif

#ifdef __cplusplus
    }
#endif
#endif
