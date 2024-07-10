/*****************************************************************************
** File Name:      mmi_res_outside.c                                          *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifdef MMI_RES_OUTSIDE_SUPPORT

#include "mmk_type.h"
#include "mmi_image.h"
#include "mmi_utility.h"
#include "mmi_resource.h"
#include "mmi_event_api.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define RES_INVALID_INDEX            ((uint32)(-1))
#define RES_OUT_NAME_MAX_LEN         15
#define RES_FOLDER_MAX_LEN           31
#define RES_FILE_MAX_LEN             31
#define RES_FILE_FULL_PATH_MAX_LEN   255

#define RES_ROOT_PATH       L"E:"
#define RES_SLASH		    L"\\"
#define RES_OUT_POSTFIX     L"*.ort"
#define U32_TAG(a, b, c, d) ((uint32)(((a)<<24)|((b)<<16)|((c)<<8)|(d)))

#define MENU_RES_FOLDER     L"menu_res"
#define MENU_RES_TAG        (U32_TAG('M','E','N','U'))       

typedef struct
{
    uint32	tag;   // "MENU"
    uint32 	version;
    uint16	lcd_width;
    uint16	lcd_height;
    uint32  count;
    wchar	res_name[RES_OUT_NAME_MAX_LEN + 1];
    uint8 	is_bigendian;	
    uint8 	Reserved[15];
}RES_OUT_HAED_T;

typedef struct
{
    uint32	img_id;
    wchar*  img_path;
    int32	offset;
}RES_OUT_MAP_ITEM;

typedef struct  
{
    uint32 hash;
    uint32 offset;
}RES_OUT_HASH_TAB;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define MACRO_MDU_TYPE  MACRO_MDU_OUTSIDE
#include "macro_mdu_def.h"

RES_OUT_MAP_ITEM g_res_outside_map[] = 
{
    #include "common_mdu_def.h"
};
#undef MACRO_MDU_TYPE

LOCAL SFS_HANDLE			s_res_out_handle = 0;
LOCAL RES_OUT_HASH_TAB*	    s_res_out_tab_ptr = PNULL;
LOCAL uint32				s_res_out_hash_tab_count = 0;
LOCAL BOOLEAN               s_res_out_enable = FALSE;

LOCAL RES_OUT_TYPE_E    s_type = RES_OUT_INVALID;
LOCAL wchar*            s_name_ptr = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
/*******************************************************************/
//  Interface:		GetOutResImgDatByOffset
//  Description : 	GetOutResImgDatByOffset
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetOutResImgDatByOffset(
                                   int32 offset, 
                                   uint8** data_buff_pptr, 
                                   uint32* len_ptr
                                   )
{
    SFS_ERROR_E	sfs_err = 0;
    uint32		read_cnt = 0;	
    
    if (PNULL != data_buff_pptr && PNULL != len_ptr && -1 < offset)
    {
        sfs_err = SFS_SetFilePointer(s_res_out_handle, (__int64)offset, SFS_SEEK_BEGIN);
        if (SFS_ERROR_NONE == sfs_err)
        {
            sfs_err = SFS_ReadFile(s_res_out_handle, len_ptr, sizeof(uint32), &read_cnt, 0);
        }
        
        SCI_TRACE_LOW("@@GetOutResImgDatByOffset:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n", offset, s_res_out_handle, sfs_err);
        if (SFS_ERROR_NONE == sfs_err)
        {
            *len_ptr = B2L_U32(len_ptr);
            MMIRES_ReadImageDataFromFile(s_res_out_handle, data_buff_pptr, *len_ptr);
        }
    }	
}

/*******************************************************************/
//  Interface:		GetOutResImgDataByPath
//  Description : 	GetOutResImgDataByPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL int32 GetOutResImgDataByPath(
                                   wchar* path_ptr, 
                                   uint8** data_buff_pptr, 
                                   uint32* len_ptr
                                   )
{
    int32 nPos = -1;
    int l = 0;
    int m = 0;
    int r = s_res_out_hash_tab_count - 1;
    uint32 hash = 0;
    SFS_ERROR_E  sfs_err = 0;
    uint32 path_len = 0;
    uint32 read_cnt = 0;
    wchar hash_path[RES_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    if (0 == s_res_out_hash_tab_count
        || PNULL == s_res_out_tab_ptr
        || PNULL == path_ptr)
    {
        return (-1);
    }
    hash = MMI_WDJBHash(path_ptr);
    
    while (l <= r)
    {
        m = l + (r - l) / 2;
        if (hash == s_res_out_tab_ptr[m].hash)
        {
            nPos = s_res_out_tab_ptr[m].offset;
            break;
        }
        else if (hash < s_res_out_tab_ptr[m].hash)
        {
            r = m - 1;
        }
        else
        {
            l = m + 1;
        }
    }
    
    while((-1 != nPos) && (hash == s_res_out_tab_ptr[--m].hash)); //lint !e722
    
    if (-1 != nPos)
    {
        while (hash == s_res_out_tab_ptr[++m].hash)
        {
            nPos = s_res_out_tab_ptr[m].offset;		
            sfs_err = SFS_SetFilePointer(s_res_out_handle, (__int64)nPos, SFS_SEEK_BEGIN);
            if (SFS_ERROR_NONE == sfs_err)
            {
                sfs_err = SFS_ReadFile(s_res_out_handle, &path_len, sizeof(uint32), &read_cnt, 0);
            }
            if (SFS_ERROR_NONE == sfs_err)
            {
                path_len = B2L_U32(&path_len);
                sfs_err = SFS_ReadFile(s_res_out_handle, hash_path, path_len, &read_cnt, 0);
                MMI_B2LWstr(hash_path);
                if (0 == MMIAPICOM_Wstrcmp(path_ptr, hash_path))
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
        SFS_GetFilePointer(s_res_out_handle, SFS_SEEK_BEGIN, &file_pos); 
        nPos = (((file_pos + 3) >> 2) << 2);
    }
    
    if (-1 != nPos)
    {
        GetOutResImgDatByOffset(nPos, data_buff_pptr, len_ptr);
    }
    
    return nPos;
}

/*******************************************************************/
//  Interface:		GetOutResImageResData
//  Description : 	GetOutResImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetOutResImageResData(
                                    uint32 map_index, 
                                    uint8** data_buff_pptr, 
                                    uint32* len_ptr
                                    )
{
    BOOLEAN bret = FALSE;
    
    if (0 == g_res_outside_map[map_index].offset)
    {
        // get offset
        g_res_outside_map[map_index].offset = GetOutResImgDataByPath(\
            g_res_outside_map[map_index].img_path, data_buff_pptr, len_ptr);
    }
    else if (0 < g_res_outside_map[map_index].offset)
    {
        GetOutResImgDatByOffset(g_res_outside_map[map_index].offset, \
            data_buff_pptr, len_ptr);
    }
    
    if (-1 != g_res_outside_map[map_index].offset)
    {
        bret = TRUE;
    }
    
    return bret;
}

/*******************************************************************/
//  Interface:		MMIRESOUT_GetImageResData
//  Description : 	MMIRESOUT_GetImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_GetImageResData(
                                         uint32 image_id, 
                                         uint8** data_pptr, 
                                         uint32* len_ptr
                                         )
{
    BOOLEAN bret = FALSE;
    static uint32 s_res_img_cache[2] = {RES_INVALID_INDEX, RES_INVALID_INDEX};
    uint32 map_index = RES_INVALID_INDEX;
    int32 l = 0;
    int32 m = 0;
    int32 r = ARR_SIZE(g_res_outside_map) - 1;
    
    if (image_id >= g_res_outside_map[l].img_id 
        && image_id <= g_res_outside_map[r].img_id)
    {
        if (image_id == s_res_img_cache[0])
        {
            map_index = s_res_img_cache[1];
        }
        else
        {
            while (l <= r)
            {
                m = l + (r - l) / 2;
                
                if (image_id == g_res_outside_map[m].img_id)
                {
                    map_index = m;
                    break;
                }
                else if (image_id < g_res_outside_map[m].img_id)
                {
                    r = m - 1;
                }
                else
                {
                    l = m + 1;
                }
            }
            
            s_res_img_cache[0] = image_id;
            s_res_img_cache[1] = map_index;
        }
        
        if (RES_INVALID_INDEX != map_index)
        {
            bret = GetOutResImageResData(map_index, data_pptr, len_ptr);
        }
    }
    
    return bret;
}

//////////////////////////////////////////////////////////////////////////
//

/*******************************************************************/
//  Interface:		GetResOutLocalFolder
//  Description : 	GetResOutLocalFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetResOutLocalFolder(
                                   RES_OUT_TYPE_E type, 
                                   wchar* path_ptr, 
                                   uint32 path_len
                                   )
{
    BOOLEAN bRet = FALSE;
    
    if (PNULL == path_ptr || 0 == path_len)
    {
        return FALSE;
    }
    
    switch(type)
    {
    case RES_OUT_MENU:
        {
            if (MMIAPICOM_Wstrlen(RES_ROOT_PATH) + 1 + MMIAPICOM_Wstrlen(MENU_RES_FOLDER) + 1
                + RES_FOLDER_MAX_LEN <= path_len)
            {
                MMIAPICOM_Wstrncpy(path_ptr, RES_ROOT_PATH, MMIAPICOM_Wstrlen(RES_ROOT_PATH));
                MMIAPICOM_Wstrncat(path_ptr, RES_SLASH, 1);
                MMIAPICOM_Wstrncat(path_ptr, MENU_RES_FOLDER, MMIAPICOM_Wstrlen(MENU_RES_FOLDER));
                MMIAPICOM_Wstrncat(path_ptr, RES_SLASH, 1);     
                
                bRet = TRUE;
            }
        }
        break;
    default:
        break;
    }
    
    if (bRet)
    {
        wchar local_folder[RES_FOLDER_MAX_LEN + 1] = {0};
        uint8  c_folder[RES_FOLDER_MAX_LEN + 1] = {0};
        uint8  endian = 0;
        uint16	lcd_width = 0;
        uint16	lcd_height = 0;
        
        endian = (uint8)(MMI_IsBigEndian() ? 'B' : 'L');
        MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
        
        _snprintf((char*)c_folder, RES_FOLDER_MAX_LEN, "Menu_%d_%d_%c", lcd_width, lcd_height, endian);
        MMIAPICOM_StrToWstr(c_folder, local_folder);
        
        MMIAPICOM_Wstrncat(path_ptr, local_folder, RES_FOLDER_MAX_LEN);
    }
    
    return bRet;
}

/*******************************************************************/
//  Interface:		GetOutResImgTag
//  Description : 	GetOutResImgTag
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL uint32 GetOutResImgTag(
                             RES_OUT_TYPE_E type
                             )
{
    uint32 tag = 0xffffffff;
    
    switch(type)
    {
    case RES_OUT_MENU:
        tag = MENU_RES_TAG;
        break;
    default:
        break;
    }
    
    return tag;
}

/*******************************************************************/
//  Interface:		ResetResOutMapOffset
//  Description : 	ResetResOutMapOffset
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetResOutMapOffset(void)
{
    uint32 i = 0;
    for (; i < ARR_SIZE(g_res_outside_map); i ++)
    {
        g_res_outside_map[i].offset = 0;
    }
}

/*******************************************************************/
//  Interface:		ResetResOutHandle
//  Description : 	ResetResOutHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetResOutHandle(void)
{
    s_res_out_hash_tab_count = 0;
    ResetResOutMapOffset();
    
    if (0 != s_res_out_tab_ptr)
    {
        SCI_FREE(s_res_out_tab_ptr)
    }
    if (0 != s_res_out_handle)
    {
        SFS_CloseFile(s_res_out_handle);
        s_res_out_handle = 0;
    }	
}

/*******************************************************************/
//  Interface:		ResOutIsValidate
//  Description : 	ResOutIsValidate
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN ResOutIsValidate(
                               RES_OUT_HAED_T* head_ptr,
                               RES_OUT_TYPE_E type,
                               const wchar* name_ptr,
                               uint32* img_cnt
                               )
{
    BOOLEAN bRet = FALSE;
    uint16	lcd_width = 0;
    uint16	lcd_height = 0;
    
    MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
    
    if (PNULL != head_ptr
        && GetOutResImgTag(type) == B2L_U32_F(&head_ptr->tag)
        && MMI_IsBigEndian() == head_ptr->is_bigendian
        && lcd_width == B2L_U16(&head_ptr->lcd_width)
        && lcd_height == B2L_U16(&head_ptr->lcd_height))
    {
        wchar res_name[RES_OUT_NAME_MAX_LEN + 1] = {0};

        MMIAPICOM_Wstrncpy(res_name, head_ptr->res_name, RES_OUT_NAME_MAX_LEN);
        MMI_B2LWstr(res_name);
        
        if (0 == MMIAPICOM_Wstrcmp(res_name, name_ptr))
        {
            if (PNULL !=img_cnt)
            {
                *img_cnt = B2L_U32(&head_ptr->count);
            }
            bRet = TRUE;
        }
    }
    
    return bRet;
}

/*******************************************************************/
//  Interface:		LoadResOutHead
//  Description : 	LoadResOutHead
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN LoadResOutHead(
                             const wchar* folder_ptr, 
                             const wchar* file_ptr,
                             RES_OUT_TYPE_E type,
                             const wchar* name_ptr
                             )
{
    SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
    wchar res_path[RES_FILE_FULL_PATH_MAX_LEN + 1] = {0}; 
    BOOLEAN bRet = FALSE;
    
    if (PNULL == folder_ptr || PNULL == file_ptr || PNULL == name_ptr)
    {
        return FALSE;
    }
    
    MMIAPICOM_Wstrncpy(res_path, folder_ptr, RES_FILE_FULL_PATH_MAX_LEN);
    MMIAPICOM_Wstrncat(res_path,RES_SLASH, 1);
    MMIAPICOM_Wstrncat(res_path, file_ptr, RES_FILE_FULL_PATH_MAX_LEN - MMIAPICOM_Wstrlen(res_path));
    
    s_res_out_handle = SFS_CreateFile(res_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    if (0 != s_res_out_handle)
    {
        RES_OUT_HAED_T  res_head = {0};
        uint32			read_cnt = 0;
        uint32          img_cnt = 0;
        
        sfs_ret = SFS_ReadFile(s_res_out_handle, &res_head, sizeof(RES_OUT_HAED_T), &read_cnt, PNULL);
        if (SFS_ERROR_NONE == sfs_ret)
        {
            // validate
            if (ResOutIsValidate(&res_head, type, name_ptr, &img_cnt) && 0 < img_cnt)
            {
                s_res_out_hash_tab_count = img_cnt;
                s_res_out_tab_ptr = (RES_OUT_HASH_TAB*)SCI_ALLOCA(s_res_out_hash_tab_count * sizeof(RES_OUT_HASH_TAB));
                if (PNULL != s_res_out_tab_ptr)
                {
                    sfs_ret = SFS_ReadFile(s_res_out_handle, s_res_out_tab_ptr, \
                        s_res_out_hash_tab_count * sizeof(RES_OUT_HASH_TAB),&read_cnt, PNULL);
                    if (SFS_ERROR_NONE == sfs_ret)
                    {
                        uint32 j = 0;
                        for (; j < s_res_out_hash_tab_count; j++)
                        {
                            s_res_out_tab_ptr[j].hash = B2L_U32(&s_res_out_tab_ptr[j].hash);
                            s_res_out_tab_ptr[j].offset = B2L_U32(&s_res_out_tab_ptr[j].offset);
                        }
                        
                        bRet = TRUE;
                    }
                }
            }
        }
    }
    
    if (!bRet)
    {
        SFS_CloseFile(s_res_out_handle);
        s_res_out_handle = 0;
        SCI_FREE(s_res_out_tab_ptr);
        s_res_out_hash_tab_count = 0;
    }
    
    return bRet;
}

/*******************************************************************/
//  Interface:		InitResOutHandle
//  Description : 	InitResOutHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN InitResOutHandle(
                               RES_OUT_TYPE_E type, 
                               const wchar* name_ptr
                               )
{
    SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
    SFS_HANDLE			find_handle = 0;
    wchar				ctlStr[] = L"/A:+NF-NF";//find file not include fold
    wchar				filter[] = RES_OUT_POSTFIX;
    SFS_FIND_DATA_T		find_data = {0};
    wchar				file_path[RES_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint32				file_len = 0;
    BOOLEAN             bRet = FALSE;
    
    if (!GetResOutLocalFolder(type, file_path, RES_FILE_FULL_PATH_MAX_LEN)
        || PNULL == name_ptr)
    {
        return FALSE;
    } 
    
    ResetResOutHandle();
    sfs_ret = SFS_FindFirstFileCtrl(file_path, filter, ctlStr, &find_handle, &find_data, PNULL);
    
    if(0 != find_handle && SFS_ERROR_NONE == sfs_ret)
    {
        do 
        {
           	file_len = MMIAPICOM_Wstrlen(find_data.name);
            if (file_len > 0 && file_len <= RES_FILE_MAX_LEN)
            {                 
                bRet = LoadResOutHead(file_path, find_data.name, type, name_ptr);
                break;
            }
            
        } while(SFS_NO_ERROR == SFS_FindNextFile(find_handle, &find_data));
    }
    
    if (NULL != find_handle)
    {
        SFS_FindClose(find_handle);
    }
    
    return TRUE;
}

//U Disk
/*******************************************************************/
//  Interface:		ResOutUdiskMountCallback
//  Description : 	ResOutUdiskMountCallback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResOutUdiskMountCallback(
                                     MMIEVENT_UDISK_DEV_E device,
                                     BOOLEAN mount
                                     )
{
    if (DEV_UDISK == device)
    {
        SCI_TRACE_LOW("@@ResOut UdiskMount:%d\n", mount);
        
        if (mount)
        {
            MMIRESOUT_Pause();
        }
        else
        {
            MMIRESOUT_Resume();
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//init
/*******************************************************************/
//  Interface:		MMIRESOUT_IsEnable
//  Description : 	MMIRESOUT_IsEnable
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_IsEnable()
{
    return s_res_out_enable;
}

/*******************************************************************/
//  Interface:		MMIRESOUT_UnInit
//  Description : 	MMIRESOUT_UnInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_UnInit()
{
    SCI_TRACE_LOW("@MMIRESOUT_UnInit.\n");
    
    MMIFW_SetUdiskMountListener(ResOutUdiskMountCallback, FALSE);
    
    s_res_out_enable = FALSE;
    ResetResOutHandle();
    MMIRES_NorFileResUnInit();
    
    s_type = RES_OUT_INVALID; 
    s_name_ptr = PNULL;
}

/*******************************************************************/
//  Interface:		MMIRESOUT_Init
//  Description : 	MMIRESOUT_Init
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_Init(
                              RES_OUT_TYPE_E type, 
                              const wchar* name_ptr
                              )
{
    BOOLEAN bRet = FALSE;
    
    if (s_res_out_enable)
    {
        MMIRESOUT_UnInit();
    }
    
    bRet = MMIRES_NorFileResInit();
    if (bRet)
    {
        bRet = InitResOutHandle(type, name_ptr);
    }
    
    if (bRet)
    {
        s_res_out_enable = TRUE;
        s_type = type;
        s_name_ptr = (wchar*)name_ptr;
        MMIFW_SetUdiskMountListener(ResOutUdiskMountCallback, TRUE);
    }
    else
    {
        s_res_out_enable = FALSE;
        ResetResOutHandle();
        MMIRES_NorFileResUnInit();       
    }
    
    SCI_TRACE_LOW("@MMIRESOUT_Init, type= %d, name_ptr= 0x%x, bRet= %d \n", type, name_ptr, bRet);
    return bRet;
}

/*******************************************************************/
//  Interface:		MMIRESOUT_Pause
//  Description : 	MMIRESOUT_Pause
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_Pause(void)
{
    SCI_TRACE_LOW("@MMIRESOUT_Pause \n");
    
    s_res_out_enable = FALSE;
    ResetResOutHandle();
    MMIRES_NorFileResUnInit();
}

/*******************************************************************/
//  Interface:		MMIRESOUT_Resume
//  Description : 	MMIRESOUT_Resume
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_Resume(void)
{
    BOOLEAN bRet = FALSE;
    
    bRet = MMIRES_NorFileResInit();
    if (bRet)
    {
        bRet = InitResOutHandle(s_type, s_name_ptr);
    }
    
    if (bRet)
    {
        s_res_out_enable = TRUE;
    }
    else
    {
        MMIRES_NorFileResUnInit();
    }
    
    SCI_TRACE_LOW("@MMMIRESOUT_Resume, type= %d, name_ptr= 0x%x, bRet= %d \n", s_type, s_name_ptr, bRet);
    return bRet;
}

/*******************************************************************/
//  Interface:		MMIRESOUT_FolderInit
//  Description : 	MMIRESOUT_FolderInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_FolderInit(void)
{   
    SFS_ERROR_E sfs_ret = SFS_ERROR_NONE;
    wchar       res_out_path[RES_FILE_FULL_PATH_MAX_LEN] = {0};
    
    // menu
    MMIAPICOM_Wstrncpy(res_out_path, RES_ROOT_PATH, MMIAPICOM_Wstrlen(RES_ROOT_PATH));
    MMIAPICOM_Wstrncat(res_out_path, RES_SLASH, 1);
    MMIAPICOM_Wstrncat(res_out_path, MENU_RES_FOLDER, MMIAPICOM_Wstrlen(MENU_RES_FOLDER));
    
    sfs_ret = SFS_CreateDirectory(res_out_path);
    if (SFS_ERROR_HAS_EXIST == sfs_ret || SFS_ERROR_NONE == sfs_ret)
    {
        sfs_ret = SFS_SetAttrAsyn(RES_ROOT_PATH, MENU_RES_FOLDER, L"/SA:+H",0);
    }
}

#endif // MMI_RES_OUTSIDE_SUPPORT