/*****************************************************************************
** File Name:      xxx_mmi_qbtheme_map.c                                          *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#include "mmk_type.h"
#include "mmi_qbtheme_map.h"
#include "mmi_image.h"
#include "mmiqbtheme_internal.h"
//#include "mmiqbtheme_export.h"
#include "mmiqbtheme_nv.h"
#include "mmi_resource.h"
#include "mmiwallpaper_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define QB_INVALID_INDEX   ((uint32)(-1))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define MACRO_MDU_TYPE  MACRO_MDU_QBTHEME
#include "macro_mdu_def.h"

QBTHEME_MAP_ITEM g_qbtheme_map[] = 
{
    #include "common_mdu_def.h"
    #include "wallpaper_mdu_def.h"
};

#undef MACRO_MDU_TYPE

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/

/*******************************************************************/
//  Interface:		IsWallpaperPath
//  Description : 	IsWallpaperPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IsWallpaperPath(
							  wchar* img_path_ptr
							  )
{
	if (0 == MMIAPICOM_Wstrcmp(img_path_ptr, QBTHEME_WALLPAPER_PATH))
	{
		return TRUE;
	}
	
	return FALSE;
}

/*******************************************************************/
//  Interface:		IsMenuIconPath
//  Description : 	IsMenuIconPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IsMenuIconPath(
							 wchar* img_path_ptr
							 )
{
	/*
	static uint32 s_npos =  0;
	if (0 == s_npos)
	{
		s_npos = MMIAPICOM_Wstrlen(QBTHEME_ICON_PATH);
	}	
	*/

	if (0 == MMIAPICOM_Wstrncmp(img_path_ptr, QBTHEME_ICON_PATH, QBTHEME_ICON_PATH_LEN/*s_npos*/))
	{
		return TRUE;
	}
	
	return FALSE;		
}

/*******************************************************************/
//  Interface:		IsMenuIconBgPath
//  Description : 	IsMenuIconBgPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IsMenuIconBgPath(
							   wchar* img_path_ptr
							   )
{
	if (0 == MMIAPICOM_Wstrcmp(img_path_ptr, QBTHEME_ICON_BG_PATH))
	{
		return TRUE;
	}
	
	return FALSE;
}

/*******************************************************************/
//  Interface:		GetThemeImageResData
//  Description : 	GetThemeImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetThemeImageResData(
								   uint32 map_index, 
								   uint8** data_buff_pptr, 
								   uint32* len_ptr
								   )
{
	BOOLEAN bret = FALSE;
	
	if (0 == g_qbtheme_map[map_index].offset)
	{
		// get offset
		g_qbtheme_map[map_index].offset = MMIQBTHEME_GetResImgDataByPath(\
			g_qbtheme_map[map_index].img_path, data_buff_pptr, len_ptr);
	}
	else if (0 < g_qbtheme_map[map_index].offset)
	{
		MMIQBTHEME_GetResImgDatByOffset(g_qbtheme_map[map_index].offset, \
			data_buff_pptr, len_ptr);
	}
	
	if (-1 != g_qbtheme_map[map_index].offset)
	{
		bret = TRUE;
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		GetWallpaperResData
//  Description : 	GetWallpaperResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetWallpaperResData(
								  uint8** data_buff_pptr, 
								  uint32* len_ptr
								  )
{
	BOOLEAN bret = FALSE;
	wchar*  wallpaper_ptr = PNULL;
	QBTHEME_PACKAGE_INFO_T *cur_info_ptr = PNULL;
	SFS_HANDLE  img_handle = 0;
	wchar       img_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	
	cur_info_ptr = MMIQBTHEME_GetCurThemeInfo();
	wallpaper_ptr = MMIQBTHEME_GetCurWallpaper();
	
	if (PNULL != cur_info_ptr
		&& PNULL != wallpaper_ptr 
		&& 0 != MMIAPICOM_Wstrlen(wallpaper_ptr))
	{
		if (PNULL != data_buff_pptr && PNULL != len_ptr)
		{
			MMIQBTHEME_GetLocalRootFolder(img_path);
			if ((MMIAPICOM_Wstrlen(img_path) + 1+ MMIAPICOM_Wstrlen(cur_info_ptr->folder) + \
				MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_PATH) + 1 + MMIAPICOM_Wstrlen(wallpaper_ptr) +  \
				MMIAPICOM_Wstrlen(QBTHEME_IMG_POSTFIX)) <= MMIFILE_FULL_PATH_MAX_LEN)
			{
				MMIAPICOM_Wstrncat(img_path, QBTHEME_SLASH, 1);
				MMIAPICOM_Wstrncat(img_path, cur_info_ptr->folder, MMIAPICOM_Wstrlen(cur_info_ptr->folder));							
				MMIAPICOM_Wstrncat(img_path, QBTHEME_WALLPAPER_PATH, MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_PATH));
				MMIAPICOM_Wstrncat(img_path, QBTHEME_SLASH, 1);
				MMIAPICOM_Wstrncat(img_path, wallpaper_ptr, MMIAPICOM_Wstrlen(wallpaper_ptr));
				MMIAPICOM_Wstrncat(img_path, QBTHEME_IMG_POSTFIX, MMIAPICOM_Wstrlen(QBTHEME_IMG_POSTFIX));		
			}
			
			img_handle = SFS_CreateFile(img_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
			if (0 != img_handle)
			{
				SFS_GetFileSize(img_handle, len_ptr);
				if (0 < *len_ptr)
				{
					MMIRES_ReadImageDataFromFile(img_handle, data_buff_pptr, *len_ptr);
				}
				
				SFS_CloseFile(img_handle);
			}
		}
		
		bret = TRUE;
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetImageResData
//  Description : 	MMIQBTHEME_GetImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_GetImageResData(
										  uint32 image_id, 
										  uint8** data_pptr, 
										  uint32* len_ptr
										  )
{
	BOOLEAN bret = FALSE;
	static uint32 s_qb_img_cache[2] = {QB_INVALID_INDEX, QB_INVALID_INDEX};
	uint32 map_index = QB_INVALID_INDEX;
	int32 l = 0;
	int32 m = 0;
	int32 r = ARR_SIZE(g_qbtheme_map) - 1;
	
	if (image_id >= g_qbtheme_map[l].img_id 
		&& image_id < g_qbtheme_map[r].img_id + g_qbtheme_map[r].range)
	{
		if (image_id == s_qb_img_cache[0])
		{
			map_index = s_qb_img_cache[1];
		}
		else
		{
			while (l <= r)
			{
				m = l + (r - l) / 2;
				
				if (image_id >= g_qbtheme_map[m].img_id
					&& image_id < g_qbtheme_map[m].img_id + g_qbtheme_map[m].range)
				{
					map_index = m;
					break;
				}
				else if (image_id < g_qbtheme_map[m].img_id)
				{
					r = m - 1;
				}
				else
				{
					l = m + 1;
				}
			}
			
			s_qb_img_cache[0] = image_id;
			s_qb_img_cache[1] = map_index;
		}
		
		if (QB_INVALID_INDEX != map_index)
		{
			if (IsMenuIconPath(g_qbtheme_map[map_index].img_path))
			{
				if (THEME_DEFAULT == MMIQBTHEME_GetCurIconType()
					|| MMIQBTHEME_GetForceTransform())
				{
					bret = GetThemeImageResData(map_index, data_pptr, len_ptr);
				}
			}
			else if (IsMenuIconBgPath(g_qbtheme_map[map_index].img_path))
			{
				if (THEME_DEFAULT == MMIQBTHEME_GetCurIconBgType()
					|| MMIQBTHEME_GetForceTransform())
				{
					bret = GetThemeImageResData(map_index, data_pptr, len_ptr);
				}
			}
			else if (IsWallpaperPath(g_qbtheme_map[map_index].img_path))
			{
				bret = GetWallpaperResData(data_pptr, len_ptr);
			}
			else
			{
				// other
				bret = GetThemeImageResData(map_index, data_pptr, len_ptr);
			}
		}
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_RestMapOffset
//  Description : 	MMIQBTHEME_RestMapOffset
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_RestMapOffset(void)
{
	uint32 i = 0;
	for (; i < sizeof(g_qbtheme_map)/sizeof(g_qbtheme_map[0]); i ++)
	{
		if (IMAGE_WALLPAPER_1 == g_qbtheme_map[i].img_id
            || IMAGE_THEME_BG == g_qbtheme_map[i].img_id)
		{
			g_qbtheme_map[i].offset = -1;
		}
		else
		{
			g_qbtheme_map[i].offset = 0;
		}
	}
}
