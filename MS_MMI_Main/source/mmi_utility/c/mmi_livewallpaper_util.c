/****************************************************************************
** File Name:      mmi_livewallpaper_util.c                                  *
** Author:         Cheng.luo                                             *
** Date:           02/20/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2012        Cheng.luo       Create
** 
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_utility_trc.h"
#include "mmi_utility.h"
#include "guicommon.h"
#include "mmi_link.h"
#include "mmi_cache.h"
#include "sfs.h"
#include "mmk_app.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_LIVEWALLPAPER_RES_CACHE_SIZE	    (1600)
#define LW_FILE_FULL_PATH_MAX_LEN               255
#define LW_RES_ID_STR_MAX_LEN					63
#define LW_RES_FILE_MAX_LEN                     31
#define LW_SLASH				                L"\\"
#define LW_ROOT_PATH                            L"E:"
#define LW_RES_FOLD                             L"livewallpaper_res"

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct _LW_RES_HEAD_T
{
    uint8  tag[4]; 	//'L','W','P','R'	
    uint32 version;	
    uint16 lcd_width;
    uint16 lcd_height;
    uint8  is_bigendian;
    uint8  reserved[15];
    uint32 count; 	//hash table count
}LW_RES_HEAD_T;

typedef struct _LW_RES_INFO_T 
{
    wchar   file[LW_RES_FILE_MAX_LEN + 1];
    char    lw_name[LW_RES_ID_STR_MAX_LEN + 1];
    uint32  img_count;
}LW_RES_INFO_T;

typedef struct  
{
    uint32 hash;
    uint32 offset;
}LW_RES_HASH_TAB;

LOCAL MMI_CACHE_INFO_T*		s_lw_res_cache_ptr = PNULL;
LOCAL SFS_HANDLE			s_lw_res_handle = 0;
LOCAL LW_RES_HASH_TAB*		s_lw_res_tab_ptr = PNULL;
LOCAL uint32				s_lw_hash_tab_count = 0;

LOCAL MMI_LINK_NODE_T*      s_lw_res_list_ptr = PNULL;
LOCAL const wchar*          s_lw_res_file_name_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIRES_ReadImageDataFromFile(
											SFS_HANDLE handle, 
											uint8** buffer_pptr,
											uint32 read_bytes
											);


//////////////////////////////////////////////////////////////////////////
// livewallpaper res list
/*******************************************************************/
//  Interface:		GetLwLocalResRootFolder
//  Description : 	GetLwLocalResRootFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetLwLocalResRootFolder(
                                      wchar *root_ptr
                                      )
{
    BOOLEAN bret = FALSE;
    
    if (PNULL != root_ptr)
    {
        wchar local_folder[LW_RES_FILE_MAX_LEN + 1] = {0};
        uint8  c_folder[LW_RES_FILE_MAX_LEN + 1] = {0};
        uint32  endian = 0;
        uint16	lcd_width = 0;
        uint16	lcd_height = 0;
        
        endian = (MMI_IsBigEndian() ? 'B' : 'L');
        MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
        
        _snprintf((char*)c_folder, LW_RES_FILE_MAX_LEN, "LW_%d_%d_%c", lcd_width, lcd_height, endian);
        MMIAPICOM_StrToWstr(c_folder, local_folder);
        
        MMIAPICOM_Wstrncpy(root_ptr, LW_ROOT_PATH, MMIAPICOM_Wstrlen(LW_ROOT_PATH));
        MMIAPICOM_Wstrncat(root_ptr, LW_SLASH, 1);
        MMIAPICOM_Wstrncat(root_ptr, LW_RES_FOLD, MMIAPICOM_Wstrlen(LW_RES_FOLD));
        MMIAPICOM_Wstrncat(root_ptr, LW_SLASH, 1);
        MMIAPICOM_Wstrncat(root_ptr, local_folder, \
            LW_FILE_FULL_PATH_MAX_LEN - MMIAPICOM_Wstrlen(root_ptr));
        
        bret = TRUE;	
    }	
    
    return bret;
}

/*******************************************************************/
//  Interface:		GetLwResPathByFile
//  Description : 	GetLwResPathByFile
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetLwResPathByFile(
                              const wchar*  file_ptr,
                              wchar*        path_ptr,
                              uint32        max_len
                              )
{
    if (PNULL != file_ptr 
        && PNULL != path_ptr
        && 0 < max_len)
    {
        GetLwLocalResRootFolder(path_ptr);
        if (MMIAPICOM_Wstrlen(path_ptr) + 1 + MMIAPICOM_Wstrlen(file_ptr) <= max_len)
        {
            MMIAPICOM_Wstrncat(path_ptr, LW_SLASH, 1);
            MMIAPICOM_Wstrncat(path_ptr, file_ptr, MMIAPICOM_Wstrlen(file_ptr));
        }
    }
}

/*******************************************************************/
//  Interface:		LwpIsValidate
//  Description : 	LwpIsValidate
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN LwpIsValidate(
                            LW_RES_HEAD_T* head_ptr, 
                            uint32* img_cnt
                            )
{
    BOOLEAN bRet = FALSE;
    uint16	lcd_width = 0;
    uint16	lcd_height = 0;
    
    MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
    
    if (PNULL != head_ptr
        && MMI_IsBigEndian() == head_ptr->is_bigendian
        && lcd_width == B2L_U16(&head_ptr->lcd_width)
        && lcd_height == B2L_U16(&head_ptr->lcd_height))
    {
        if (PNULL !=img_cnt)
        {
            *img_cnt = B2L_U32(&head_ptr->count);
        }
        bRet = TRUE;
    }
    
    return bRet;
}

/*******************************************************************/
//  Interface:		DestroyLwResNode
//  Description : 	DestroyLwResNode
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyLwResNode(
                            MMI_LINK_NODE_T const * node_ptr
                            )
{
    LW_RES_INFO_T *info_node_ptr = NULL;
    
    if (PNULL != node_ptr)
    {
        info_node_ptr = (LW_RES_INFO_T*)node_ptr->data;
        SCI_FREE(info_node_ptr);
    }
}

/*******************************************************************/
//  Interface:		SearchLwResList
//  Description : 	SearchLwResList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN SearchLwResList(
                              MMI_LINK_NODE_T const* node_ptr,
                              uint32            condition1, 
                              uint32            condition2
                              )
{
    if(0 == strcmp((char *)condition1,((LW_RES_INFO_T*)node_ptr->data)->lw_name))
    {
        return TRUE;
    }
    
    return FALSE;
}

/*******************************************************************/
//  Interface:		AddLwResToList
//  Description : 	AddLwResToList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN AddLwResToList(
                             LW_RES_INFO_T *wallpaper_data_ptr
                             )
{
    MMI_LINK_NODE_T	*link_node_ptr = PNULL;
    
    if (PNULL == wallpaper_data_ptr)
    {
        return FALSE;
    }
    
    if(PNULL != s_lw_res_list_ptr)
    {
        link_node_ptr = MMILINK_CreateNode((uint32)wallpaper_data_ptr);
        if (PNULL == link_node_ptr)
        {
            return FALSE;
        }
        MMILINK_AddNodeBeforeBaseNode(s_lw_res_list_ptr, link_node_ptr);		
    }
    else
    {
        link_node_ptr = MMILINK_CreateHead((uint32)wallpaper_data_ptr);
        if (PNULL == link_node_ptr)
        {
            return FALSE;
        }
        s_lw_res_list_ptr = link_node_ptr;
    } 
    
    return (BOOLEAN)(PNULL != link_node_ptr);
}

/*******************************************************************/
//  Interface:		GetInfoFormList
//  Description : 	GetInfoFormList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL LW_RES_INFO_T* GetInfoFormList(
                                     const char *name_ptr
                                     )
{
    MMI_LINK_NODE_T *res_node_ptr = PNULL;
    
    if (PNULL == name_ptr || 0 == strlen(name_ptr))
    {
        return PNULL;
    }
    
    if (PNULL != s_lw_res_list_ptr)
    {
        res_node_ptr = MMILINK_SearchNode(s_lw_res_list_ptr, TRUE, SearchLwResList, \
            (uint32)name_ptr, 0); 
    }
    
    if(PNULL != res_node_ptr)
    {  		
        return (LW_RES_INFO_T*)res_node_ptr->data;
    }
    
    return PNULL;	
}

/*******************************************************************/
//  Interface:		ReadLwResHeadAndAddList
//  Description : 	ReadLwResHeadAndAddList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN ReadLwResHeadAndAddList(
                                      const wchar* folder_ptr,
                                      const wchar* file_ptr
                                      )
{
    SFS_HANDLE  info_handle = 0;
    SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
    wchar lw_path[LW_FILE_FULL_PATH_MAX_LEN + 1] = {0};    
    
    if (PNULL == folder_ptr || PNULL == file_ptr)
    {
        return FALSE;
    }
    
    MMIAPICOM_Wstrncpy(lw_path, folder_ptr, LW_FILE_FULL_PATH_MAX_LEN);
    MMIAPICOM_Wstrncat(lw_path, LW_SLASH, 1);
    MMIAPICOM_Wstrncat(lw_path, file_ptr, LW_FILE_FULL_PATH_MAX_LEN - MMIAPICOM_Wstrlen(lw_path));
    
    info_handle = SFS_CreateFile(lw_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    if (0 != info_handle)
    {
        LW_RES_HEAD_T   info_head = {0};
        uint32			read_cnt = 0;
        uint32          img_cnt = 0;
        
        sfs_ret = SFS_ReadFile(info_handle, &info_head, sizeof(LW_RES_HEAD_T), &read_cnt, PNULL);
        if (SFS_ERROR_NONE == sfs_ret)
        {
            // validate
            if (LwpIsValidate(&info_head, &img_cnt) && 0 < img_cnt)
            {
                uint32     name_cnt = 0;
                
                // name count
                sfs_ret = SFS_SetFilePointer(info_handle, (__int64)img_cnt * sizeof(LW_RES_HASH_TAB), SFS_SEEK_CUR);
                if (SFS_ERROR_NONE == sfs_ret)
                {
                    sfs_ret = SFS_ReadFile(info_handle, &name_cnt, sizeof(uint32), &read_cnt, 0);
                }
                if (SFS_ERROR_NONE == sfs_ret)
                {
                    uint32 i = 0;
                    int32* offset_ptr = PNULL;
                    
                    // offset
                    name_cnt = B2L_U32(&name_cnt);
                    offset_ptr = (int32*)SCI_ALLOCA(sizeof(int32) * name_cnt);
                    if (PNULL == offset_ptr)
                    {
                        SFS_CloseFile(info_handle);	
                        return FALSE;
                    }
                    SCI_MEMSET(offset_ptr, 0, sizeof(int32) * name_cnt);
                    
                    sfs_ret = SFS_ReadFile(info_handle, offset_ptr, sizeof(int32) * name_cnt, &read_cnt, PNULL);
                    if (SFS_ERROR_NONE == sfs_ret)
                    {
                        for (i = 0; i < name_cnt; i++)
                        {
                            LW_RES_INFO_T*   lw_info_ptr = PNULL;
                            int32 cur_pos = 0;
                            int32 name_end_pos = B2L_U32(offset_ptr + i);
                            int32 name_len = 0;
                            
                            SFS_GetFilePointer(info_handle, SFS_SEEK_BEGIN, &cur_pos);
                            name_len = name_end_pos - cur_pos;
                            if (name_len > LW_RES_ID_STR_MAX_LEN)
                                continue;
                            
                            lw_info_ptr = (LW_RES_INFO_T*)SCI_ALLOCA(sizeof(LW_RES_INFO_T));
                            if (PNULL != lw_info_ptr)
                            {
                                SCI_MEMSET(lw_info_ptr, 0, sizeof(LW_RES_INFO_T));
                                MMIAPICOM_Wstrncpy(lw_info_ptr->file, file_ptr, LW_RES_FILE_MAX_LEN);
                                lw_info_ptr->img_count = img_cnt;
                                
                                sfs_ret = SFS_ReadFile(info_handle, lw_info_ptr->lw_name, name_len, &read_cnt, PNULL);
                                if (SFS_ERROR_NONE != sfs_ret)
                                {
                                    SCI_FREE(lw_info_ptr);
                                }
                                
                                if (PNULL != lw_info_ptr)
                                {
                                    AddLwResToList(lw_info_ptr);
                                }
                            }
                        }
                    }
                    
                    SCI_FREE(offset_ptr)
                }     
            }
        }
        else
        {
            sfs_ret = SFS_ERROR_SYSTEM;
        }
    }
    
    SFS_CloseFile(info_handle);	
    
    return (BOOLEAN)(SFS_ERROR_NONE == sfs_ret);
}

/*******************************************************************/
//  Interface:		MMI_LivewallpaperResListInit
//  Description : 	MMI_LivewallpaperResListInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_LivewallpaperResListInit(void)
{
    SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
    SFS_HANDLE			find_handle = 0;
    wchar				ctlStr[] = L"/A:+NF-NF";//find file not include fold
    wchar				filter[] = L"*.lwp";//all files
    SFS_FIND_DATA_T		find_data = {0};
    wchar				file_path[LW_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint32				file_len = 0;
    
    if (PNULL != s_lw_res_list_ptr)
    {
        return FALSE;
    }
    
    GetLwLocalResRootFolder(file_path);
    sfs_ret = SFS_FindFirstFileCtrl(file_path, filter, ctlStr, &find_handle, &find_data, PNULL);
    
    if(0 != find_handle && SFS_ERROR_NONE == sfs_ret)
    {
        do 
        {
           	file_len = MMIAPICOM_Wstrlen(find_data.name);
            if (file_len > 0 && file_len <= LW_RES_FILE_MAX_LEN)
            {                 
                ReadLwResHeadAndAddList(file_path, find_data.name);
            }
            
        } while(SFS_NO_ERROR == SFS_FindNextFile(find_handle, &find_data));
    }
    
    if (NULL != find_handle)
    {
        SFS_FindClose(find_handle);
    }
    
    return TRUE;
}

/*******************************************************************/
//  Interface:		MMI_DestroyLivewallpaperResList
//  Description : 	MMI_DestroyLivewallpaperResList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_DestroyLivewallpaperResList(void)
{
    if (PNULL != s_lw_res_list_ptr)
    {
        MMILINK_DestroyLink(s_lw_res_list_ptr, DestroyLwResNode);
        s_lw_res_list_ptr = PNULL;
    }
}

/*******************************************************************/
//  Interface:		MMI_IsLivewallpaperResExist
//  Description : 	MMI_IsLivewallpaperResExist
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_IsLivewallpaperResExist(
                                           const char* name_ptr
                                           )
{
    if (PNULL == name_ptr)
    {
        return FALSE;
    }
    
    return (BOOLEAN)(PNULL != GetInfoFormList(name_ptr));
}

//////////////////////////////////////////////////////////////////////////
// offset cache
/*******************************************************************/
//  Interface:		GetLwResCache
//  Description : 	GetLwResCache
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_CACHE_INFO_T* GetLwResCache(void)
{   
    if (PNULL == s_lw_res_cache_ptr)
    {
        s_lw_res_cache_ptr = MMICACHE_Init("livewallpaper_res",
            MMI_LIVEWALLPAPER_RES_CACHE_SIZE,
            CKT_INT);
    }
    
    return s_lw_res_cache_ptr;
}

/*******************************************************************/
//  Interface:		MMI_LivewallpaperCleanUp
//  Description : 	MMI_LivewallpaperCleanUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_LivewallpaperCleanUp(void)
{
	if (PNULL != s_lw_res_cache_ptr)
	{
		MMICACHE_Destroy(s_lw_res_cache_ptr);
		s_lw_res_cache_ptr = PNULL;
	}
		
	if (PNULL != s_lw_res_tab_ptr)
	{
		SCI_FREE(s_lw_res_tab_ptr);
	}
	
	if (0 != s_lw_res_handle)
	{
		SFS_CloseFile(s_lw_res_handle);
		s_lw_res_handle = 0;
	}

    s_lw_hash_tab_count = 0;
    s_lw_res_file_name_ptr = PNULL;
}

/*******************************************************************/
//  Interface:		MMI_LivewallpaperInitHandle
//  Description : 	MMI_LivewallpaperInitHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_LivewallpaperInitHandle(
                                        const char* lw_name_ptr
                                        )
{
    LW_RES_INFO_T* info_ptr = PNULL;
    
    info_ptr = GetInfoFormList(lw_name_ptr);
    if (PNULL == info_ptr)
    {
        return;
    }
    
    if (0 != MMIAPICOM_Wstrcmp(info_ptr->file, s_lw_res_file_name_ptr))
    {
        wchar lw_path[LW_FILE_FULL_PATH_MAX_LEN + 1] = {0};    
        
        MMI_LivewallpaperCleanUp();
        
        GetLwResPathByFile(info_ptr->file, lw_path, LW_FILE_FULL_PATH_MAX_LEN);
        s_lw_res_handle = SFS_CreateFile(lw_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);    
        if (0 != s_lw_res_handle)
        {
            SFS_ERROR_E  sfs_err = 0; 
            uint32 read_cnt = 0;
            
            sfs_err = SFS_SetFilePointer(s_lw_res_handle, (__int64)sizeof(LW_RES_HEAD_T), SFS_SEEK_BEGIN);
            
            s_lw_hash_tab_count = info_ptr->img_count;
            s_lw_res_tab_ptr = (LW_RES_HASH_TAB*)SCI_ALLOCA(s_lw_hash_tab_count * sizeof(LW_RES_HASH_TAB));
            sfs_err = SFS_ReadFile(s_lw_res_handle, s_lw_res_tab_ptr, \
                s_lw_hash_tab_count * sizeof(LW_RES_HASH_TAB), &read_cnt, 0);
            
            if (SFS_ERROR_NONE == sfs_err)
            {
                s_lw_res_cache_ptr = GetLwResCache();
                s_lw_res_file_name_ptr = info_ptr->file;
            }
            else
            {
                SFS_CloseFile(s_lw_res_handle);
            }
        }
    }
}

/*******************************************************************/
//  Interface:		SearchLwResDataPositon
//  Description : 	SearchLwResDataPositon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL int32 SearchLwResDataPositon(
                                   const char*	 name_ptr
                                   )
{
    int32 nPos = -1;
    
    if (PNULL != name_ptr
        && PNULL != s_lw_res_tab_ptr 
        && 0 < s_lw_hash_tab_count
        && 0 != s_lw_res_handle)
    {
        int l = 0;
        int m = 0;
        int r = s_lw_hash_tab_count - 1;
        uint32 hash = 0;
        SFS_ERROR_E  sfs_err = 0;
        uint32 path_len = 0;
        uint32 read_cnt = 0;
        char res_id_str[LW_RES_ID_STR_MAX_LEN + 1] = {0};
        
        // calc hash
        hash = MMI_DJBHash(name_ptr);
        
        // binary search
        while (l <= r)
        {
            m = l + (r - l) / 2;
            if (hash == s_lw_res_tab_ptr[m].hash)
            {
                nPos = s_lw_res_tab_ptr[m].offset;
                break;
            }
            else if (hash < s_lw_res_tab_ptr[m].hash)
            {
                r = m - 1;
            }
            else
            {
                l = m + 1;
            }
        }
        
        while((-1 != nPos) && (hash == s_lw_res_tab_ptr[--m].hash)); //lint !e722
        
        if (-1 != nPos)
        {
            while (hash == s_lw_res_tab_ptr[++m].hash)
            {
                nPos = s_lw_res_tab_ptr[m].offset;		
                sfs_err = SFS_SetFilePointer(s_lw_res_handle, (__int64)nPos, SFS_SEEK_BEGIN);
                if (SFS_ERROR_NONE == sfs_err)
                {
                    sfs_err = SFS_ReadFile(s_lw_res_handle, &path_len, sizeof(uint32), &read_cnt, 0);
                }
                if (SFS_ERROR_NONE == sfs_err)
                {
                    path_len = B2L_U32(&path_len);
                    sfs_err = SFS_ReadFile(s_lw_res_handle, res_id_str, path_len, &read_cnt, 0);
                    if (0 == strcmp(name_ptr, res_id_str))
                    {
                        break;
                    }
                }
                nPos = -1;
            }
        }
        
        if (-1 != nPos)
        {
            int32 file_pos = 0;
            SFS_GetFilePointer(s_lw_res_handle, SFS_SEEK_BEGIN, &file_pos); 
            nPos = (((file_pos + 3) >> 2) << 2);
        }		
    }
    
    return nPos;
}

/*******************************************************************/
//  Interface:		GetLwResDataPositon
//  Description : 	GetLwResDataPositon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetLwResDataPositon(
                                  uint32 image_id,
                                  const char* name_ptr,
                                  int32* pos_ptr
                                  )
{
    BOOLEAN		bRet = FALSE;
    MMI_CACHE_INT_KEY_ITEM_T item_t = {0};
    int32 nPos = -1;
    
    if (0 == image_id || PNULL == name_ptr)
    {
        return FALSE;
    }
    
    bRet = MMICACHE_GetItemByInt(GetLwResCache(), image_id, &item_t);
    if (!bRet) 
    {
        nPos = SearchLwResDataPositon(name_ptr);
        
        // put item
        SCI_MEMSET(&item_t, 0, sizeof(MMI_CACHE_INT_KEY_ITEM_T));
        item_t.key_id = image_id;
        item_t.reserve = (uint32)nPos;
        
        bRet = MMICACHE_PutItemByInt(GetLwResCache(), &item_t, TRUE);
    }
    
    if (bRet && (PNULL != pos_ptr))
    {
        *pos_ptr = (int32)item_t.reserve;
    }
    
    return bRet;
}

/*******************************************************************/
//  Interface:		GetLwResImgDataByOffset
//  Description : 	GetLwResImgDataByOffset
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetLwResImgDataByOffset(
									int32 offset, 
									uint8** data_buff_pptr, 
									uint32* len_ptr
									)
{
    SFS_ERROR_E	sfs_err = 0;
    uint32		read_cnt = 0;	
    
    if (PNULL != data_buff_pptr && PNULL != len_ptr && -1 < offset)
    {
        sfs_err = SFS_SetFilePointer(s_lw_res_handle, (__int64)offset, SFS_SEEK_BEGIN);
        if (SFS_ERROR_NONE == sfs_err)
        {
            sfs_err = SFS_ReadFile(s_lw_res_handle, len_ptr, sizeof(uint32), &read_cnt, 0);
        }
        //SCI_TRACE_LOW:"@@GetLwResImgDataByOffset:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_UTIL_723_112_2_18_3_25_11_1,(uint8*)"ddd", offset, s_lw_res_handle, sfs_err);
        if (SFS_ERROR_NONE == sfs_err)
        {
            *len_ptr = B2L_U32(len_ptr);
            MMIRES_ReadImageDataFromFile(s_lw_res_handle, data_buff_pptr, *len_ptr);
        }
    }	
}

/*******************************************************************/
//  Interface:		MMI_GetLivewallpaerImageResData
//  Description : 	MMI_GetLivewallpaerImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_GetLivewallpaerImageResData(
                                               uint32        image_id,
                                               const char*   image_id_str,
                                               uint8**       data_pptr, 
                                               uint32*       len_ptr
                                               )
{
    BOOLEAN bRet = FALSE;
    int32 nPos = -1;
    
    if (0 == image_id || PNULL == image_id_str)
    {
        return FALSE;
    }
    
    bRet = GetLwResDataPositon(image_id, image_id_str, &nPos);
    if (bRet && -1 < nPos)
    {
        GetLwResImgDataByOffset(nPos, data_pptr, len_ptr);
    }
    
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

#endif //LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT

// fold attr
PUBLIC void MMI_LivewallpaperResFoldInit(void)
{
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
    
    SFS_ERROR_E sfs_ret = SFS_ERROR_NONE;
    wchar lw_res_path[LW_FILE_FULL_PATH_MAX_LEN] = {0};
    
    MMIAPICOM_Wstrncpy(lw_res_path, LW_ROOT_PATH, MMIAPICOM_Wstrlen(LW_ROOT_PATH));
    MMIAPICOM_Wstrncat(lw_res_path, LW_SLASH, 1);
    MMIAPICOM_Wstrncat(lw_res_path, LW_RES_FOLD, MMIAPICOM_Wstrlen(LW_RES_FOLD));
    
    sfs_ret = SFS_CreateDirectory(lw_res_path);
    if (SFS_ERROR_HAS_EXIST == sfs_ret || SFS_ERROR_NONE == sfs_ret)
    {
        sfs_ret = SFS_SetAttrAsyn(LW_ROOT_PATH, LW_RES_FOLD, L"/SA:+H",0);
    }
#endif
}

/*******************************************************************/
//  Interface:		GetMainLCDRealSpecs
//  Description : 	GetMainLCDRealSpecs
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_GetMainLCDRealSpecs(
                                    uint16* lcd_w_ptr, 
                                    uint16* lcd_h_ptr
                                    )
{
    if (PNULL != lcd_w_ptr && PNULL != lcd_h_ptr)
    {
        uint16	lcd_width = 0;
        uint16	lcd_height = 0;
        
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        if (GUILCD_IsLandscape(GUI_MAIN_LCD_ID))
        {
            uint16 temp = 0;
            temp = lcd_width;
            lcd_width = lcd_height;
            lcd_height = temp;
        }
        
        *lcd_w_ptr = lcd_width;
        *lcd_h_ptr = lcd_height;
    }
}

/*******************************************************************/
//  Interface:		DJBHash
//  Description : 	DJBHash
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMI_DJBHash(const char *c_str)
{
	uint32 hash = 5381;
	
	while ('\0' != *c_str)
	{
		hash += (hash << 5) + (*c_str++); /*lint !e737*/
	}
	
	return (hash & 0x7FFFFFFF);
}

/*******************************************************************/
//  Interface:		MMI_WDJBHash
//  Description : 	MMI_WDJBHash
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMI_WDJBHash(const wchar *str)
{
    uint32 hash = 5381;
    uint8  c_str = PNULL;
    
    while ('\0' != (c_str = *str++))
    {
        hash += (hash << 5) + c_str;
    }
    
    return (hash & 0x7FFFFFFF);
}

/*******************************************************************/
//  Interface:		MMI_B2LWstr
//  Description : 	MMI_B2LWstr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_B2LWstr(wchar *str_ptr)
{	
	if( PNULL != str_ptr )
    {		
		do
		{
			*str_ptr = B2L_U16(str_ptr);
		}while (*str_ptr++);
    }
}
