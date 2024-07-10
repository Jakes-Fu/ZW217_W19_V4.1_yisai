/*****************************************************************************
** File Name:      mmiqbtheme_app.c                                          *
** Author:         cheng.luo                                                 *
** Date:           2011/09                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/09/17     cheng.luo       Create.                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                      *
**--------------------------------------------------------------------------*/
#include "mmi_app_qbtheme_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "gui_ucs2b_converter.h"
#include "mmipub.h"
#include "sfs.h"
#include "mmifmm_export.h"
#include "mmi_link.h"
#include "mmi_resource.h"
#include "guilcd.h"

#include "mmiqbtheme_export.h"
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_id.h"
#include "mmiqbtheme_nv.h"

#include "mmi_modu_main.h"
//#include "sig_code.h"
#include "theme_gzip_export.h"

#include "mmi_utility.h"

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmi_nv.h"
#ifdef QBTHEME_SUPPORT
#include "Mmi_appmsg.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/ 
#define	QB_TOTAL_RATE	100

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL wchar* s_qbtheme_image_path[] = 
{
	L"\\Preview\\Overview.jpg",	// QB_IMG_OVERVIEW
	L"\\Preview\\Preview1.jpg",	// QB_IMG_PREVIEW1
	L"\\Preview\\Preview2.jpg",	// QB_IMG_PREVIEW2
	L"\\Preview\\Preview3.jpg",	// QB_IMG_PREVIEW3
	L"\\Icon\\PREVIEW.png",		// QB_IMG_ICON_PRIVIEW
	L"\\MainMenu\\IconBg_view.png",	// QB_IMG_ICON_BG
	L"\\Font\\Sample.jpg",		// QB_IMG_FONT_SAMPLE
	L"\\Font\\Preview.png"		// QB_IMG_FONT_PREVIEW
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct  
{
	uint32		tag;
	uint32		version;
	uint32		info_size;
	uint32		pkg_size;
	uint32		pkg_version;
	wchar		name_en[QBTHEME_NAME_EN_MAX_LEN + 1];
	wchar		name_zh[QBTHEME_NAME_MAX_LEN + 1];
	wchar		author[QBTHEME_AUTHOR_MAX_LEN + 1];
	uint16		reserved;
	uint16		crc;
}QBTHEME_INFO_DATA_T;

typedef struct  
{
	wchar name[QBTHEME_NAME_EN_MAX_LEN + 1];
	wchar postfix[QBTHEME_POSIX_MAX_LEN + 1];
	uint32 file_size;
}QBTHEME_WALLPAPER_DATA_T;

typedef struct
{
	uint32 tag;
	uint32 version;
	uint32 hdr_size;
	uint16 lcd_width;
	uint16 lcd_height;
	uint8  zip;
	uint8  is_bigendian;
	uint8  reserved[14];
	wchar  name[QBTHEME_NAME_EN_MAX_LEN + 1];
}QBTHEME_PACKAGE_HEADER_T;

typedef struct  
{
	uint32 tag;
	uint32 version;
	uint32 count;
	uint8  is_bigendian;
	uint8  reserved[3];
}QBTHEME_RES_HEADER_T;

typedef struct  
{
	uint32 hash;
	uint32 offset;
}QBTHEME_RES_HASH_TAB;

/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
LOCAL MMI_LINK_NODE_T* s_qbtheme_info_list_ptr = PNULL;
LOCAL MMI_LINK_NODE_T* s_qbtheme_wallpaper_list_ptr = PNULL;

LOCAL SFS_HANDLE			s_qbtheme_res_handle = 0;
LOCAL QBTHEME_RES_HASH_TAB*	s_qbtheme_res_tab_ptr = 0;
LOCAL uint32				s_qbtheme_hash_tab_count = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void GetInfoFilePathByFolder(
								   wchar *folder_ptr, 
								   wchar *info_file_ptr
								   );

LOCAL BOOLEAN ReadThemePackageInfo(
								   wchar* info_file_ptr, 
								   QBTHEME_PACKAGE_INFO_T *info_node_ptr
								   );

LOCAL BOOLEAN SearchQBThemeList(
							   MMI_LINK_NODE_T const* node_ptr,
							   uint32 condition1, 
							   uint32 condition2
							   );

LOCAL void DestroyQBThemeNode(
							  MMI_LINK_NODE_T const * node_ptr
							  );

LOCAL BOOLEAN AddQBThemeToList(
							   QBTHEME_PACKAGE_INFO_T *info_node_ptr
							   );

LOCAL BOOLEAN RemoveQBThemeFromList(
									const wchar *name_ptr
									);

LOCAL QBTHEME_PACKAGE_INFO_T* GetInfoFormList(
											  const wchar *name_ptr
											  );

/**--------------------------------------------------------------------------*
**                         EXTERNAL FUNCTION DEFINITION                     *
**--------------------------------------------------------------------------*/
/*******************************************************************/
//  Interface:		MMIQBTHEME_Wstrrchr
//  Description : 	MMIQBTHEME_Wstrrchr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMIQBTHEME_Wstrrchr(
								  wchar *src_ptr, 
								  wchar ch
								  )
{
	wchar* des_ptr = PNULL;

	if (PNULL == src_ptr)
	{
		return 0; 
	}
	
	des_ptr = src_ptr + MMIAPICOM_Wstrlen(src_ptr) - 1;
	while (des_ptr != src_ptr)
	{
		if (ch == *des_ptr)
		{
			break;
		}
		des_ptr--;
	}

	return (des_ptr - src_ptr);
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetLocalRootFolder
//  Description : 	MMIQBTHEME_GetLocalRootFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_GetLocalRootFolder(
											 wchar *root_ptr
											 )
{
	BOOLEAN bret = FALSE;
	
	if (PNULL != root_ptr)
	{
		wchar local_folder[QBTHEME_FOLDER_MAX_LEN + 1] = {0};
		uint8  c_folder[QBTHEME_FOLDER_MAX_LEN + 1] = {0};
		uint8  endian = 0;
		uint16	lcd_width = 0;
		uint16	lcd_height = 0;
		
		endian = (MMI_IsBigEndian() ? 'B' : 'L');
		MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
		
		_snprintf((char*)c_folder, QBTHEME_FOLDER_MAX_LEN, "%d_%d_%c", lcd_width, lcd_height, endian);
		MMIAPICOM_StrToWstr(c_folder, local_folder);
		
		MMIAPICOM_Wstrncpy(root_ptr, QBTHEME_ROOT_PATH, MMIAPICOM_Wstrlen(QBTHEME_ROOT_PATH));
		MMIAPICOM_Wstrncat(root_ptr, QBTHEME_SLASH, 1);
		MMIAPICOM_Wstrncat(root_ptr, local_folder, MMIAPICOM_Wstrlen(local_folder));
		
		bret = TRUE;	
	}	
	
	return bret;
}

//////////////////////////////////////////////////////////////////////////
// QBTheme list

/*******************************************************************/
//  Interface:		QbThemeListInit
//  Description : 	QbThemeListInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN QbThemeListInit(void)
{
	SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
	SFS_HANDLE			find_handle = 0;
	wchar				ctlStr[] = L"/A:+NF-F";//find folder not include file
	wchar				filter[] = L"*";//all files
	SFS_FIND_DATA_T		find_data = {0};
	QBTHEME_PACKAGE_INFO_T *theme_info_ptr = PNULL;
	wchar				file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32				file_len = 0;
	
	MMIQBTHEME_GetLocalRootFolder(file_path);
	sfs_ret = SFS_FindFirstFileCtrl(file_path, filter, ctlStr, &find_handle, &find_data, PNULL);
	
	if(0 != find_handle && SFS_ERROR_NONE == sfs_ret)
	{
		do 
		{
			file_len = MMIAPICOM_Wstrlen(find_data.name);
			if (file_len > 0 && file_len <= QBTHEME_FOLDER_MAX_LEN)
			{
				BOOLEAN bret = FALSE;
				
				theme_info_ptr = (QBTHEME_PACKAGE_INFO_T *)SCI_ALLOCA(sizeof(QBTHEME_PACKAGE_INFO_T));
				if (PNULL == theme_info_ptr)
				{
					break;
				}
				SCI_MEMSET(theme_info_ptr, 0, sizeof(QBTHEME_PACKAGE_INFO_T));
				MMIAPICOM_Wstrncpy(theme_info_ptr->folder, find_data.name, QBTHEME_FOLDER_MAX_LEN);
				
				SCI_MEMSET(file_path, 0, sizeof(file_path));
				GetInfoFilePathByFolder(find_data.name, file_path);
				
				bret = ReadThemePackageInfo(file_path, theme_info_ptr);
				if (bret)
				{
					bret = AddQBThemeToList(theme_info_ptr);
				}
				if (!bret)
				{
					SCI_FREE(theme_info_ptr);
				}
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
//  Interface:		GetInfoFilePathByFolder
//  Description : 	GetInfoFilePathByFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetInfoFilePathByFolder(
								   wchar *folder_ptr, 
								   wchar *info_file_ptr
								   )
{
	if(PNULL != folder_ptr && PNULL != info_file_ptr)
	{
		wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetLocalRootFolder(root_path);
		
		if ((MMIAPICOM_Wstrlen(root_path) + 1 + MMIAPICOM_Wstrlen(folder_ptr) + \
			1 + MMIAPICOM_Wstrlen(QBTHEME_PACKAGE_INFO_FILE)) <= MMIFILE_FULL_PATH_MAX_LEN)
		{
			MMIAPICOM_Wstrncpy(info_file_ptr, root_path, MMIAPICOM_Wstrlen(root_path));
			MMIAPICOM_Wstrncat(info_file_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(info_file_ptr, folder_ptr, MMIAPICOM_Wstrlen(folder_ptr));
			MMIAPICOM_Wstrncat(info_file_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(info_file_ptr, QBTHEME_PACKAGE_INFO_FILE, MMIAPICOM_Wstrlen(QBTHEME_PACKAGE_INFO_FILE));
		}
	}
}

/*******************************************************************/
//  Interface:		GetResFilePathByFolder
//  Description : 	GetResFilePathByFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetResFilePathByFolder(
								  wchar *folder_ptr, 
								  wchar *res_file_ptr
								  )
{
	if(PNULL != folder_ptr && PNULL != res_file_ptr)
	{
		wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetLocalRootFolder(root_path);
		
		if ((MMIAPICOM_Wstrlen(root_path) + 1 + MMIAPICOM_Wstrlen(folder_ptr) + \
			1 + MMIAPICOM_Wstrlen(QBTHEME_RES_FILE)) <= MMIFILE_FULL_PATH_MAX_LEN)
		{
			MMIAPICOM_Wstrncpy(res_file_ptr, root_path, MMIAPICOM_Wstrlen(root_path));
			MMIAPICOM_Wstrncat(res_file_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(res_file_ptr, folder_ptr, MMIAPICOM_Wstrlen(folder_ptr));
			MMIAPICOM_Wstrncat(res_file_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(res_file_ptr, QBTHEME_RES_FILE, MMIAPICOM_Wstrlen(QBTHEME_RES_FILE));
		}
	}
}

/*******************************************************************/
//  Interface:		GetFontFilePathByFolder
//  Description : 	GetFontFilePathByFolder
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetFontFilePathByFolder(
								   wchar *folder_ptr, 
								   wchar *font_file_ptr
								   )
{
	BOOLEAN bret = FALSE;
	
	if(PNULL != folder_ptr && PNULL != font_file_ptr)
	{
		wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetLocalRootFolder(root_path);
		
		if ((MMIAPICOM_Wstrlen(root_path) + 1 + MMIAPICOM_Wstrlen(folder_ptr) + \
			1 + QBTHEME_FONT_PATH_LEN + QBTHEME_FONT_MAX_LEN) <= MMIFILE_FULL_PATH_MAX_LEN)
		{
			SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
			SFS_HANDLE			find_handle = 0;
			wchar				ctlStr[] = L"/A:+NF-NF";//find file not include fold
			wchar				filter[] = L"*.lib";//all files
			SFS_FIND_DATA_T		find_data = {0};
			uint32				file_len = 0;
			
			MMIAPICOM_Wstrncpy(font_file_ptr, root_path, MMIAPICOM_Wstrlen(root_path));
			MMIAPICOM_Wstrncat(font_file_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(font_file_ptr, folder_ptr, MMIAPICOM_Wstrlen(folder_ptr));
			MMIAPICOM_Wstrncat(font_file_ptr, QBTHEME_FONT_PATH, QBTHEME_FONT_PATH_LEN);
			
			// find font *.lib
			sfs_ret = SFS_FindFirstFileCtrl(font_file_ptr, filter, ctlStr, &find_handle, &find_data, PNULL);
			if(0 != find_handle && SFS_ERROR_NONE == sfs_ret)
			{
				file_len = MMIAPICOM_Wstrlen(find_data.name);
				if (file_len > 0 && file_len <= QBTHEME_FONT_MAX_LEN)
				{
					uint16 attr = 0;
					
					MMIAPICOM_Wstrncat(font_file_ptr, QBTHEME_SLASH, 1);
					MMIAPICOM_Wstrncat(font_file_ptr, find_data.name, file_len);
					
					sfs_ret = SFS_GetAttr(font_file_ptr, &attr);
					if (SFS_ERROR_NONE == sfs_ret)
					{
						bret = TRUE;
					}
				}
			}
			
			if (NULL != find_handle)
			{
				SFS_FindClose(find_handle);
			}
		}
	}
	
	return bret;
}

PUBLIC BOOLEAN IsVersionMatch(uint32 ver)
{
    BOOLEAN bRet = FALSE;
    
    bRet = (QBTHEME_MAJOR_VER == ((ver & 0xff000000) >> 24));
    bRet = bRet && (QBTHEME_MINOR_VER == ((ver & 0x00ff0000) >> 16));
    
    return bRet;
}

LOCAL BOOLEAN ReadThemePackageInfo(
								   wchar* info_file_ptr, 
								   QBTHEME_PACKAGE_INFO_T *info_node_ptr
								   )
{
	SFS_HANDLE  info_handle = 0;
	SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
	
	if (PNULL == info_file_ptr || PNULL == info_node_ptr)
	{
		return FALSE;
	}
	
	info_handle = SFS_CreateFile(info_file_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
	if (0 != info_handle)
	{
		QBTHEME_INFO_DATA_T*  info_data_ptr = PNULL;
		uint32				  read_cnt = 0;
		
		info_data_ptr = (QBTHEME_INFO_DATA_T*)SCI_ALLOCA(sizeof(QBTHEME_INFO_DATA_T));
		if (PNULL == info_data_ptr)
		{
			return FALSE;
		}
		
		SCI_MEMSET(info_data_ptr, 0, sizeof(QBTHEME_INFO_DATA_T));
		sfs_ret = SFS_ReadFile(info_handle, info_data_ptr, sizeof(QBTHEME_INFO_DATA_T), &read_cnt, PNULL);
		
		if (SFS_ERROR_NONE == sfs_ret)
		{			
			if (QBTHEME_INFO_TAG == B2L_U32_F(&info_data_ptr->tag)
				&& sizeof(QBTHEME_INFO_DATA_T) == read_cnt
                && IsVersionMatch(B2L_U32_F(&info_data_ptr->pkg_version)))
			{
				// size
				info_node_ptr->pkg_size = B2L_U32(&info_data_ptr->pkg_size);
				// name
				MMIAPICOM_Wstrncpy(info_node_ptr->name_zh, info_data_ptr->name_zh, QBTHEME_NAME_MAX_LEN);
				MMI_B2LWstr(info_node_ptr->name_zh);
				// name_en
				MMIAPICOM_Wstrncpy(info_node_ptr->name_en, info_data_ptr->name_en, QBTHEME_NAME_EN_MAX_LEN);
				MMI_B2LWstr(info_node_ptr->name_en);
				// author
				MMIAPICOM_Wstrncpy(info_node_ptr->author, info_data_ptr->author, QBTHEME_AUTHOR_MAX_LEN);
				MMI_B2LWstr(info_node_ptr->author);
				// font_size
				info_node_ptr->font_size = (uint32)-1;
			}
			else
			{
				sfs_ret = SFS_ERROR_SYSTEM;
			}
		}
		
		SCI_FREE(info_data_ptr);
	}
	
	SFS_CloseFile(info_handle);	
	
	return (BOOLEAN)(SFS_ERROR_NONE == sfs_ret);
}

/*******************************************************************/
//  Interface:		SearchQBThemeList
//  Description : 	SearchQBThemeList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN SearchQBThemeList(
							   MMI_LINK_NODE_T const* node_ptr,
							   uint32 condition1, 
							   uint32 condition2
							   )
{
	if(0 == MMIAPICOM_Wstrcmp((wchar *)condition1,((QBTHEME_PACKAGE_INFO_T*)node_ptr->data)->name_en))
	{
		return TRUE;
	}
	
	return FALSE;
}

/*******************************************************************/
//  Interface:		DestroyQBThemeNode
//  Description : 	DestroyQBThemeNode
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyQBThemeNode(
							  MMI_LINK_NODE_T const * node_ptr
							  )
{
	QBTHEME_PACKAGE_INFO_T *info_node_ptr = NULL;
	
	if (PNULL != node_ptr)
	{
		info_node_ptr = (QBTHEME_PACKAGE_INFO_T*)node_ptr->data;
		SCI_FREE(info_node_ptr);
	}
}

/*******************************************************************/
//  Interface:		AddQBThemeToList
//  Description : 	AddQBThemeToList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN AddQBThemeToList(
							   QBTHEME_PACKAGE_INFO_T *info_node_ptr
							   )
{
	MMI_LINK_NODE_T *theme_node_ptr = PNULL;
	MMI_LINK_NODE_T	*link_node_ptr = PNULL;
	
	if (PNULL == info_node_ptr)
	{
		return FALSE;
	}
	
	if (PNULL != s_qbtheme_info_list_ptr)
	{
		theme_node_ptr = MMILINK_SearchNode(s_qbtheme_info_list_ptr, TRUE, SearchQBThemeList, \
			(uint32)info_node_ptr->name_en, 0); 
	}
	
	if (PNULL != theme_node_ptr)
	{
		// Already Exist
		return FALSE;
	}
	else
	{
		if(PNULL != s_qbtheme_info_list_ptr)
		{
			link_node_ptr = MMILINK_CreateNode((uint32)info_node_ptr);
			if (PNULL == link_node_ptr)
			{
				return FALSE;
			}
			MMILINK_AddNodeBeforeBaseNode(s_qbtheme_info_list_ptr, link_node_ptr);		
		}
		else
		{
			link_node_ptr = MMILINK_CreateHead((uint32)info_node_ptr);
			if (PNULL == link_node_ptr)
			{
				return FALSE;
			}
			s_qbtheme_info_list_ptr = link_node_ptr;
		} 
	}
	
	return (BOOLEAN)(PNULL != link_node_ptr);
}


/*******************************************************************/
//  Interface:		RemoveQBThemeFromList
//  Description : 	RemoveQBThemeFromList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN RemoveQBThemeFromList(
									const wchar *name_ptr
									)
{
	MMI_LINK_NODE_T *theme_node_ptr = PNULL;
	MMI_LINK_NODE_T	*link_node_ptr = PNULL;
	
	if (PNULL == name_ptr)
	{
		return FALSE;
	}
	
	if (PNULL != s_qbtheme_info_list_ptr)
	{
		theme_node_ptr = MMILINK_SearchNode(s_qbtheme_info_list_ptr, TRUE, SearchQBThemeList, \
			(uint32)name_ptr, 0); 
	}
	
	if (PNULL != theme_node_ptr)
	{
		link_node_ptr = MMILINK_DestroyNode(theme_node_ptr, s_qbtheme_info_list_ptr,\
			DestroyQBThemeNode);
		s_qbtheme_info_list_ptr = link_node_ptr;
	}
	
	return TRUE;
}

/*******************************************************************/
//  Interface:		GetInfoFormList
//  Description : 	GetInfoFormList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL QBTHEME_PACKAGE_INFO_T* GetInfoFormList(
											  const wchar *name_ptr
											  )
{
	MMI_LINK_NODE_T *theme_node_ptr = PNULL;
	
	if (PNULL == name_ptr
		|| 0 == MMIAPICOM_Wstrlen(name_ptr))
	{
		return PNULL;
	}
	
	if (PNULL != s_qbtheme_info_list_ptr)
	{
		theme_node_ptr = MMILINK_SearchNode(s_qbtheme_info_list_ptr, TRUE, SearchQBThemeList, \
			(uint32)name_ptr, 0); 
	}
	
	if(PNULL != theme_node_ptr)
	{  		
		return (QBTHEME_PACKAGE_INFO_T*)theme_node_ptr->data;
	}
	
	return PNULL;	
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetDownloadCount
//  Description : 	MMIQBTHEME_GetDownloadCount
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMIQBTHEME_GetDownloadCount(void)
{
	return MMILINK_GetListNodesNum(s_qbtheme_info_list_ptr);	
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_ManageInit
//  Description : 	MMIQBTHEME_ManageInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_ManageInit(void)
{	
	return QbThemeListInit();
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_ManageUninit
//  Description : 	MMIQBTHEME_ManageUninit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_ManageUninit(void)
{
	if (PNULL != s_qbtheme_info_list_ptr)
	{
		MMILINK_DestroyLink(s_qbtheme_info_list_ptr, DestroyQBThemeNode);
		s_qbtheme_info_list_ptr = PNULL;
	}
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetCurThemeInfo
//  Description : 	MMIQBTHEME_GetCurThemeInfo
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC QBTHEME_PACKAGE_INFO_T *MMIQBTHEME_GetCurThemeInfo(void)
{
	return GetInfoFormList(MMIQBTHEME_GetCurrentTheme());
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetInfoByIndex
//  Description : 	MMIQBTHEME_GetInfoByIndex
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC QBTHEME_PACKAGE_INFO_T *MMIQBTHEME_GetInfoByIndex(
														 uint16 index  //base 0
														 )
{
	uint16  cur_index = index;
	MMI_LINK_NODE_T *cur_node_ptr = s_qbtheme_info_list_ptr;
	
	if (PNULL == s_qbtheme_info_list_ptr)
	{
		return PNULL;
	}
	
	while(0 != cur_index--) 
	{
		cur_node_ptr = cur_node_ptr->next_ptr;
		if (cur_node_ptr == s_qbtheme_info_list_ptr)
		{
			cur_node_ptr = PNULL;
			break;
		}
	}
	
	if(PNULL != cur_node_ptr)
	{  		
		return (QBTHEME_PACKAGE_INFO_T*)cur_node_ptr->data;
	}
	
	return PNULL;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetIndexByName
//  Description : 	MMIQBTHEME_GetIndexByName
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint16 MMIQBTHEME_GetIndexByName(
                                        wchar* name_ptr
                                        )
{
    uint16 cur_index = 0;
    MMI_LINK_NODE_T *cur_node_ptr = s_qbtheme_info_list_ptr;
    QBTHEME_PACKAGE_INFO_T* cur_info_ptr = PNULL;
    BOOLEAN is_find = FALSE;
    
    if (PNULL == s_qbtheme_info_list_ptr
        || PNULL == name_ptr
        || 0 == MMIAPICOM_Wstrlen(name_ptr))
    {
        return (uint16)(-1);
    }
    
    do 
    {
        cur_info_ptr = (QBTHEME_PACKAGE_INFO_T*)cur_node_ptr->data;
        if (0 == MMIAPICOM_Wstrcmp(name_ptr, cur_info_ptr->name_en))
        {
            is_find = TRUE;
            break;
        }
        ++cur_index;
        cur_node_ptr = cur_node_ptr->next_ptr;
    }while(cur_node_ptr != s_qbtheme_info_list_ptr);
    
    if (is_find)
    {
        return cur_index;
    }
    
    return (uint16)(-1);
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetImagePath
//  Description : 	MMIQBTHEME_GetImagePath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_GetImagePath(
									   QBTHEME_PACKAGE_INFO_T *pkg_info_ptr,
									   QBTHEME_IMAGE_ID_E img_id,
									   wchar* path_ptr,
									   uint32 max_len
									   )
{
	wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

	if (PNULL == pkg_info_ptr || PNULL == path_ptr)
	{
		return FALSE;
	}

	MMIQBTHEME_GetLocalRootFolder(root_path);
	if ((MMIAPICOM_Wstrlen(root_path) + 1 + MMIAPICOM_Wstrlen(pkg_info_ptr->folder) + \
		MMIAPICOM_Wstrlen(s_qbtheme_image_path[img_id])) <= max_len)
	{
		MMIAPICOM_Wstrncpy(path_ptr, root_path, MMIAPICOM_Wstrlen(root_path));
		MMIAPICOM_Wstrncat(path_ptr, QBTHEME_SLASH, 1);
		MMIAPICOM_Wstrncat(path_ptr, pkg_info_ptr->folder, MMIAPICOM_Wstrlen(pkg_info_ptr->folder));
		MMIAPICOM_Wstrncat(path_ptr, s_qbtheme_image_path[img_id], MMIAPICOM_Wstrlen(s_qbtheme_image_path[img_id]));
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// wallpaper list

/*******************************************************************/
//  Interface:		DestroyWallpaperNode
//  Description : 	DestroyWallpaperNode
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyWallpaperNode(
								MMI_LINK_NODE_T const * node_ptr
								)
{
	QBTHEME_WALLPAPER_DATA_T *info_node_ptr = NULL;
	
	if (PNULL != node_ptr)
	{
		info_node_ptr = (QBTHEME_WALLPAPER_DATA_T*)node_ptr->data;
		SCI_FREE(info_node_ptr);
	}
}

/*******************************************************************/
//  Interface:		AddWallpaperToList
//  Description : 	AddWallpaperToList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN AddWallpaperToList(
								 QBTHEME_WALLPAPER_DATA_T *wallpaper_data_ptr
								 )
{
	MMI_LINK_NODE_T	*link_node_ptr = PNULL;
	
	if (PNULL == wallpaper_data_ptr)
	{
		return FALSE;
	}
	
	if(PNULL != s_qbtheme_wallpaper_list_ptr)
	{
		link_node_ptr = MMILINK_CreateNode((uint32)wallpaper_data_ptr);
		if (PNULL == link_node_ptr)
		{
			return FALSE;
		}
		MMILINK_AddNodeBeforeBaseNode(s_qbtheme_wallpaper_list_ptr, link_node_ptr);		
	}
	else
	{
		link_node_ptr = MMILINK_CreateHead((uint32)wallpaper_data_ptr);
		if (PNULL == link_node_ptr)
		{
			return FALSE;
		}
		s_qbtheme_wallpaper_list_ptr = link_node_ptr;
	} 
	
	return (BOOLEAN)(PNULL != link_node_ptr);
}

/*******************************************************************/
//  Interface:		GetWallpaperByIndex
//  Description : 	GetWallpaperByIndex
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL QBTHEME_WALLPAPER_DATA_T* GetWallpaperByIndex(
													uint16 index  //base 0
													)
{
	uint16  cur_index = index;
	MMI_LINK_NODE_T *cur_node_ptr = s_qbtheme_wallpaper_list_ptr;
	
	if (PNULL == s_qbtheme_wallpaper_list_ptr)
	{
		return PNULL;
	}
	
	while(0 != cur_index--) 
	{
		cur_node_ptr = cur_node_ptr->next_ptr;
		if (cur_node_ptr == s_qbtheme_wallpaper_list_ptr)
		{
			cur_node_ptr = PNULL;
			break;
		}
	}
	
	if(PNULL != cur_node_ptr)
	{  		
		return (QBTHEME_WALLPAPER_DATA_T*)cur_node_ptr->data;
	}
	
	return PNULL;
}	

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetWallpaperName
//  Description : 	MMIQBTHEME_GetWallpaperName
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC wchar* MMIQBTHEME_GetWallpaperName(
										  uint16 index
										  )
{
	QBTHEME_WALLPAPER_DATA_T* wallpaper_data_ptr = GetWallpaperByIndex(index);
	if (PNULL != wallpaper_data_ptr)
	{
		return wallpaper_data_ptr->name;
	}
	
	return PNULL;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetWallpaperSize
//  Description : 	MMIQBTHEME_GetWallpaperSize
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMIQBTHEME_GetWallpaperSize(
										  uint16 index
										  )
{
	QBTHEME_WALLPAPER_DATA_T* wallpaper_data_ptr = GetWallpaperByIndex(index);
	if (PNULL != wallpaper_data_ptr)
	{
		return wallpaper_data_ptr->file_size;
	}
	
	return 0;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetWallpaerPreviewPath
//  Description : 	MMIQBTHEME_GetWallpaerPreviewPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_GetWallpaerPreviewPath(
											  QBTHEME_PACKAGE_INFO_T *pkg_info_ptr, 
											  uint16 index, 
											  wchar* path_ptr
											  )
{
	QBTHEME_WALLPAPER_DATA_T* wallpaper_data_ptr = PNULL;
	
	if (PNULL == pkg_info_ptr || PNULL == path_ptr)
	{
		return;
	}
	
	wallpaper_data_ptr = GetWallpaperByIndex(index);
	if (PNULL != wallpaper_data_ptr)
	{
		wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetLocalRootFolder(root_path);

		if ((MMIAPICOM_Wstrlen(root_path) + 1 + MMIAPICOM_Wstrlen(pkg_info_ptr->folder) + \
			MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_ORIG_PATH) + 1 + MMIAPICOM_Wstrlen(wallpaper_data_ptr->name) + \
			MMIAPICOM_Wstrlen(wallpaper_data_ptr->postfix)) <= MMIFILE_FULL_PATH_MAX_LEN)
		{
			MMIAPICOM_Wstrncpy(path_ptr, root_path, MMIAPICOM_Wstrlen(root_path));
			MMIAPICOM_Wstrncat(path_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(path_ptr, pkg_info_ptr->folder, MMIAPICOM_Wstrlen(pkg_info_ptr->folder));
			MMIAPICOM_Wstrncat(path_ptr, QBTHEME_WALLPAPER_ORIG_PATH, MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_ORIG_PATH));
			MMIAPICOM_Wstrncat(path_ptr, QBTHEME_SLASH, 1);
			MMIAPICOM_Wstrncat(path_ptr, wallpaper_data_ptr->name, MMIAPICOM_Wstrlen(wallpaper_data_ptr->name));
			MMIAPICOM_Wstrncat(path_ptr, wallpaper_data_ptr->postfix, MMIAPICOM_Wstrlen(wallpaper_data_ptr->postfix));
		}
	}
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_WallpaperListInit
//  Description : 	MMIQBTHEME_WallpaperListInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_WallpaperListInit(
										 QBTHEME_PACKAGE_INFO_T *pkg_info_ptr
										 )
{
	SFS_ERROR_E         sfs_ret = SFS_ERROR_NONE;
	SFS_HANDLE			find_handle = 0;
	wchar				ctlStr[] = L"/A:+NF-NF";//find file not include fold
	wchar				filter[] = L"*";//all files
	SFS_FIND_DATA_T		find_data = {0};
	wchar				file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32				file_len = 0;
	QBTHEME_WALLPAPER_DATA_T *wallpaper_data_ptr = 0;
	
	if (PNULL == pkg_info_ptr)
	{
		return;
	}
	
	MMIQBTHEME_GetLocalRootFolder(file_path);
	if ((MMIAPICOM_Wstrlen(file_path) + 1 + MMIAPICOM_Wstrlen(pkg_info_ptr->folder) + \
		MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_ORIG_PATH)) <= MMIFILE_FULL_PATH_MAX_LEN)
	{
		MMIAPICOM_Wstrncat(file_path, QBTHEME_SLASH, 1);
		MMIAPICOM_Wstrncat(file_path, pkg_info_ptr->folder, MMIAPICOM_Wstrlen(pkg_info_ptr->folder));
		MMIAPICOM_Wstrncat(file_path, QBTHEME_WALLPAPER_ORIG_PATH, MMIAPICOM_Wstrlen(QBTHEME_WALLPAPER_ORIG_PATH));
	}
	
	sfs_ret = SFS_FindFirstFileCtrl(file_path, filter, ctlStr, &find_handle, &find_data, PNULL);
	if(0 != find_handle && SFS_ERROR_NONE == sfs_ret)
	{
		do 
		{
			file_len = MMIAPICOM_Wstrlen(find_data.name);
			if (file_len > 0 && file_len <= QBTHEME_FOLDER_MAX_LEN)
			{
				BOOLEAN bret = FALSE;
				uint32  npos = 0;
				
				wallpaper_data_ptr = (QBTHEME_WALLPAPER_DATA_T *)SCI_ALLOCA(sizeof(QBTHEME_WALLPAPER_DATA_T));
				if (PNULL == wallpaper_data_ptr)
				{
					goto INIT1_ERROR;
				}
				SCI_MEMSET(wallpaper_data_ptr, 0, sizeof(QBTHEME_WALLPAPER_DATA_T));
				npos = MMIQBTHEME_Wstrrchr(find_data.name, QBTHEME_DOT);
				
				if (file_len - npos <=  QBTHEME_POSIX_MAX_LEN)
				{
					MMIAPICOM_Wstrncpy(wallpaper_data_ptr->name, find_data.name, npos);
					MMIAPICOM_Wstrncpy(wallpaper_data_ptr->postfix, find_data.name + npos, file_len - npos);
					wallpaper_data_ptr->file_size = find_data.length;
#ifdef LOW_MEMORY_SUPPORT
					if (0 == MMIAPICOM_Wstrcmp(wallpaper_data_ptr->name, L"default"))
#endif
						bret = AddWallpaperToList(wallpaper_data_ptr);
				}
				
				if (!bret)
				{
					SCI_FREE(wallpaper_data_ptr);
				}
			}
		} while(SFS_NO_ERROR == SFS_FindNextFile(find_handle, &find_data));
	}
	
INIT1_ERROR:	
	if (NULL != find_handle)
    {
        SFS_FindClose(find_handle);
    }
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetThemeWallpaperCount
//  Description : 	MMIQBTHEME_GetThemeWallpaperCount
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMIQBTHEME_GetThemeWallpaperCount(void)
{
	return MMILINK_GetListNodesNum(s_qbtheme_wallpaper_list_ptr);	
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_DestroyWallpaperList
//  Description : 	MMIQBTHEME_DestroyWallpaperList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_DestroyWallpaperList(void)
{
	if (PNULL != s_qbtheme_wallpaper_list_ptr)
	{
		MMILINK_DestroyLink(s_qbtheme_wallpaper_list_ptr, DestroyWallpaperNode);
		s_qbtheme_wallpaper_list_ptr = PNULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// install package

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_Install
//  Description : 	MMIAPIQBTHEME_Install
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPIQBTHEME_Install(
								  const wchar* pkg_path_ptr
								  )
{
	BOOLEAN bret = FALSE;
	SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
	MMI_TEXT_ID_T err_text = TXT_QB_INSTALL_FAIL;
	SFS_HANDLE pkg_handle = PNULL;
	QBTHEME_PACKAGE_HEADER_T* pkg_hdr_ptr = PNULL;
	
	if (PNULL == pkg_path_ptr)
	{
		goto INSTALL_ERROR;
	}
	
	pkg_handle = SFS_CreateFile(pkg_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
	if (0 != pkg_handle)
	{
		uint32	read_cnt = 0;
		
		pkg_hdr_ptr = (QBTHEME_PACKAGE_HEADER_T*)SCI_ALLOCA(sizeof(QBTHEME_PACKAGE_HEADER_T));
		if (PNULL == pkg_hdr_ptr)
		{
			sfs_ret = SFS_ERROR_SYSTEM;
			goto INSTALL_ERROR;
		}
		
		SCI_MEMSET(pkg_hdr_ptr, 0, sizeof(QBTHEME_PACKAGE_HEADER_T));
		sfs_ret = SFS_ReadFile(pkg_handle, pkg_hdr_ptr, sizeof(QBTHEME_PACKAGE_HEADER_T), &read_cnt, PNULL);
		
		if (SFS_ERROR_NONE == sfs_ret)
		{			
			if (QBTHEME_PACKAGE_TAG == B2L_U32_F(&pkg_hdr_ptr->tag)
				&& sizeof(QBTHEME_PACKAGE_HEADER_T) == read_cnt
				&& MMI_IsBigEndian() == pkg_hdr_ptr->is_bigendian
                && IsVersionMatch(B2L_U32_F(&pkg_hdr_ptr->version)))
			{
				uint16 lcd_width = 0;
				uint16 lcd_height = 0;
				wchar file_name[QBTHEME_NAME_EN_MAX_LEN + 1] = {0};
				wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
				
				// resolution
				MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);				
				if (lcd_width != B2L_U16(&pkg_hdr_ptr->lcd_width)
					|| lcd_height != B2L_U16(&pkg_hdr_ptr->lcd_height))
				{
					err_text = TXT_QB_RESOLUTION_ERROR;
					sfs_ret = SFS_ERROR_INVALID_FORMAT;
					goto INSTALL_ERROR;
				}
				
				// name
				MMIAPICOM_Wstrncpy(file_name, pkg_hdr_ptr->name, QBTHEME_NAME_EN_MAX_LEN);
				MMI_B2LWstr(file_name);
				
				if (PNULL != GetInfoFormList(file_name))
				{
					err_text = TXT_QB_ALREADY_INSTALL;
					sfs_ret = SFS_ERROR_HAS_EXIST;
					goto INSTALL_ERROR;	
				}
				
				{
					MMI_WIN_ID_T    progress_win = MMIQBTHEME_PROGRESS_WIN_ID;
					MMIPUB_OpenProgressWinByTextId( TXT_QB_INSTALLING, &progress_win, PNULL, MMIPUB_SOFTKEY_NONE, PNULL );
					MMIPUB_SetProgressTotalSize(&progress_win, QB_TOTAL_RATE);
				}
				
				MMIQBTHEME_GetLocalRootFolder(root_path);				
				if (0 == theme_unzip(pkg_path_ptr, root_path, file_name))
				{
					err_text = TXT_QB_INSTALL_FAIL;
					sfs_ret = SFS_ERROR_SYSTEM;
					goto INSTALL_ERROR;	
				}
			}
			else
			{
				err_text = TXT_QB_PACKAGE_ERROR;
				sfs_ret = SFS_ERROR_INVALID_FORMAT;
				goto INSTALL_ERROR;
			}
		}
		
		SCI_FREE(pkg_hdr_ptr);
	}
	SFS_CloseFile(pkg_handle);	
	
INSTALL_ERROR:
	bret = (BOOLEAN)(SFS_ERROR_NONE == sfs_ret);
	if (!bret)
	{
		SCI_FREE(pkg_hdr_ptr);
		SFS_CloseFile(pkg_handle);
		
		MMIPUB_OpenAlertFailWin(err_text);		
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_HandleRefInstallMsg
//  Description : 	MMIQBTHEME_HandleRefInstallMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_RESULT_E MMIQBTHEME_HandleRefInstallMsg(
												   PWND			app_ptr, 
												   uint16		msg_id, 
												   DPARAM		param
												   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_WIN_ID_T progress_win = MMIQBTHEME_PROGRESS_WIN_ID;
	THEME_UNZIP_SIG_T unzip_content = *((THEME_UNZIP_SIG_T*)param);
	BOOLEAN is_need_dispatch = 0;
    switch (msg_id)
	{
	case THEME_UNZIP_SCODE_PROCESS:
		{		
	            if(!MMK_IsOpenWin(MMIQBTHEME_PROGRESS_WIN_ID))
	            {
	                is_need_dispatch = 1;
	            }
	            else
	            {
	                MMIPUB_UpdateProgressBar(&progress_win, unzip_content.sig_content.process);
	            }
			if (QB_TOTAL_RATE == unzip_content.sig_content.process)
			{
				BOOLEAN bret = FALSE;
				QBTHEME_PACKAGE_INFO_T *theme_info_ptr = PNULL;
				wchar				file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
				
				//add node
				if (PNULL != unzip_content.sig_content.p_param
					&& PNULL != unzip_content.sig_content.p_param->p_base
					&& PNULL != unzip_content.sig_content.p_param->p_dir)
				{					
					theme_info_ptr = (QBTHEME_PACKAGE_INFO_T *)SCI_ALLOCA(sizeof(QBTHEME_PACKAGE_INFO_T));
					if (PNULL != theme_info_ptr)
					{
						SCI_MEMSET(theme_info_ptr, 0, sizeof(QBTHEME_PACKAGE_INFO_T));
						MMIAPICOM_Wstrncpy(theme_info_ptr->folder, unzip_content.sig_content.p_param->p_dir, QBTHEME_FOLDER_MAX_LEN);						
						
						GetInfoFilePathByFolder(unzip_content.sig_content.p_param->p_dir, file_path);
						
						bret = ReadThemePackageInfo(file_path, theme_info_ptr);
						if (bret)
						{
							bret = AddQBThemeToList(theme_info_ptr);
						}
						
						if (bret && PNULL != unzip_content.sig_content.p_param->iname)
						{
							// test by cheng.luo
							//SFS_DeleteFile(unzip_content.p_param->iname, PNULL);
						}
					}					
				}
				// win
				MMITHEME_UpdateRect();
				MMIPUB_CloseProgressWin(&progress_win);
				theme_clean(&unzip_content.sig_content);
		             if (is_need_dispatch)
		             {
		                    MMK_PostMsg(MMK_GetFocusWinHandle(), MSG_QBTHEME_UNZIP, (DPARAM)&bret, sizeof(BOOLEAN));
		              }
				else
				{
					if (bret)
					{
			                    if (STYLE_IS_SUPPORT_QBTHEME)
			                    {
			                    	 	MMIAPIQBTHEME_QueryApplyWin(theme_info_ptr);
			                    }
			                    else
			                    {
							MMIPUB_OpenAlertSuccessWin( TXT_COMM_INSTALL_COMPLETE);
			                    }
					}
					else
					{
						MMIPUB_OpenAlertFailWin(TXT_QB_INSTALL_FAIL);
					}				
				}
			}
		}
		break;
	case THEME_UNZIP_SCODE_FAIL:
		{
			wchar file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	            if(!MMK_IsOpenWin(MMIQBTHEME_PROGRESS_WIN_ID))
	            {
	                is_need_dispatch = 1;
	            }
	            else
	            {
				MMIPUB_CloseProgressWin(&progress_win);
	            }
			
			// delete info file
			if (PNULL != unzip_content.sig_content.p_param
				&& PNULL != unzip_content.sig_content.p_param->p_dir)
			{
				GetInfoFilePathByFolder(unzip_content.sig_content.p_param->p_dir, file_path);
				SFS_DeleteFile(file_path, PNULL);
			}
			theme_clean(&unzip_content.sig_content);
	            if (is_need_dispatch)
	            {
	            		BOOLEAN bret = FALSE;
		              MMK_PostMsg(MMK_GetFocusWinHandle(), MSG_QBTHEME_UNZIP, (DPARAM)&bret, sizeof(BOOLEAN));
	            }
	            else
	            {
				MMIPUB_OpenAlertFailWin(TXT_QB_INSTALL_FAIL);
	            }			
		}
		break;
	default:
		result = MMI_RESULT_FALSE;
        break;
	}
	
	return result;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_Uninstall
//  Description : 	MMIQBTHEME_Uninstall
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_Uninstall(
									const wchar* name_ptr
									)
{
	BOOLEAN bret = FALSE;
	QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
	wchar theme_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	
	info_ptr = GetInfoFormList(name_ptr);
	if (PNULL == info_ptr)
	{
		return FALSE;
	}
	
	// remove fold(delete info file)
	GetInfoFilePathByFolder(info_ptr->folder, theme_path);
	
	if (SFS_ERROR_NONE == SFS_DeleteFile(theme_path, PNULL))
	{
		SCI_MEMSET(theme_path, 0, sizeof(theme_path));
		GetResFilePathByFolder(info_ptr->folder, theme_path);
		SFS_DeleteFile(theme_path, PNULL);
		
		SCI_MEMSET(theme_path,0, sizeof(theme_path));
		GetFontFilePathByFolder(info_ptr->folder, theme_path);
		SFS_DeleteFile(theme_path, PNULL);
		
		// remove node
		bret = RemoveQBThemeFromList(name_ptr);
	}
	
	return bret;
}

//////////////////////////////////////////////////////////////////////////
// download
#ifdef BROWSER_SUPPORT   
/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_OpenDownLoadUrl
//  Description : 	MMIAPIQBTHEME_OpenDownLoadUrl
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_OpenDownLoadUrl()
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0}; /*lint !e64*/
    char u8_url[QBTHEME_DOWNLOAD_URL_MAX_LEN + 1] = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint8 endian = 0;
    
    endian = MMI_IsBigEndian();
    MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
    
    _snprintf((char*)u8_url, QBTHEME_DOWNLOAD_URL_MAX_LEN, \
        QBTHEME_DOWNLOAD_URL"resolution=%d*%d&big=%d&version=%c.%c",\
        lcd_width, lcd_height, endian, QBTHEME_MAJOR_VER,QBTHEME_MINOR_VER);
    
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = (char *)u8_url;
    entry_param.user_agent_ptr = PNULL;
    
    MMIAPIBROWSER_Entry(&entry_param);
}

LOCAL void HandleQbThemeDownloadCB(
								   MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr
								   )
{
    if (PNULL != param_ptr
		&& MMIBROWSER_MIME_TYPE_QBT == param_ptr->mime_type)
    {
        MMIAPIQBTHEME_Install(param_ptr->file_name);;
    }
}
#endif

/*******************************************************************/
//  Interface:		MMIQBTHEME_RegBrowserCB
//  Description : 	MMIQBTHEME_RegBrowserCB
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_RegBrowserCB(void)
{
#ifdef BROWSER_SUPPORT    
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T qbt_callback = {0};

	qbt_callback.call_back = HandleQbThemeDownloadCB;	
	qbt_callback.mime_type = MMIBROWSER_MIME_TYPE_QBT;

	MMIAPIBROWSER_RegMimeFunc(&qbt_callback);
#endif	
}

//////////////////////////////////////////////////////////////////////////
// paser theme package

/*******************************************************************/
//  Interface:		ResetQbThemeRes
//  Description : 	ResetQbThemeRes
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetQbThemeRes(void)
{
	s_qbtheme_hash_tab_count = 0;
	MMIQBTHEME_RestMapOffset();
	if (0 != s_qbtheme_res_tab_ptr)
	{
		SCI_FREE(s_qbtheme_res_tab_ptr)
	}
	if (0 != s_qbtheme_res_handle)
	{
		SFS_CloseFile(s_qbtheme_res_handle);
		s_qbtheme_res_handle = 0;
	}	
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_InitResHandle
//  Description : 	MMIQBTHEME_InitResHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_InitResHandle(
										QBTHEME_PACKAGE_INFO_T *pkg_info_ptr
										)
{
	wchar file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
	
	ResetQbThemeRes();
	if (PNULL == pkg_info_ptr)
	{
		return TRUE;
	}
	
	GetResFilePathByFolder(pkg_info_ptr->folder, file_path);
	
	s_qbtheme_res_handle = SFS_CreateFile(file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
	if (0 != s_qbtheme_res_handle)
	{
		QBTHEME_RES_HEADER_T  res_head = {0};
		uint32				  read_cnt = 0;
		
		sfs_ret = SFS_ReadFile(s_qbtheme_res_handle, &res_head, sizeof(QBTHEME_RES_HEADER_T), &read_cnt, PNULL);
		if (SFS_ERROR_NONE == sfs_ret
			&& QBTHEME_RES_TAG == B2L_U32_F(&res_head.tag)
			&& sizeof(QBTHEME_RES_HEADER_T) == read_cnt
			&& MMI_IsBigEndian() == res_head.is_bigendian
            && IsVersionMatch(B2L_U32_F(&res_head.version)))
		{
			s_qbtheme_hash_tab_count = B2L_U32(&res_head.count);
			s_qbtheme_res_tab_ptr = (QBTHEME_RES_HASH_TAB*)SCI_ALLOCA(s_qbtheme_hash_tab_count * sizeof(QBTHEME_RES_HASH_TAB));
			if (PNULL != s_qbtheme_res_tab_ptr)
			{
				sfs_ret = SFS_ReadFile(s_qbtheme_res_handle, s_qbtheme_res_tab_ptr, \
					s_qbtheme_hash_tab_count * sizeof(QBTHEME_RES_HASH_TAB),&read_cnt, PNULL);
				if (SFS_ERROR_NONE == sfs_ret)
				{
					uint32 j = 0;
					for (; j < s_qbtheme_hash_tab_count; j++)
					{
						s_qbtheme_res_tab_ptr[j].hash = B2L_U32(&s_qbtheme_res_tab_ptr[j].hash);
						s_qbtheme_res_tab_ptr[j].offset = B2L_U32(&s_qbtheme_res_tab_ptr[j].offset);
					}
					
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_StyleStart
//  Description : 	MMIAPIQBTHEME_StyleStart
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_StyleStart(void)
{
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;

	MMIRES_NorFileResInit();
	MMIQBTHEME_ManageInit();
	
	info_ptr = MMIQBTHEME_GetCurThemeInfo();
	MMIQBTHEME_InitResHandle(info_ptr);
	MMIQBTHEME_LoadThemeFont(info_ptr);

#if 0 /*defined LOW_MEMORY_SUPPORT && defined MMI_VIDEOWALLPAPER_SUPPORT*/
	{
		MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
		
		MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
		if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
		{
			wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
			wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
			
			MMIAPISET_SetWallpaperSetInfo(wallpaper_info);			
		}		
	}
#endif
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_StyleStop
//  Description : 	MMIAPIQBTHEME_StyleStop
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_StyleStop(void)
{
	//MMIRES_UnloadFileFont();
#ifdef QBTHEME_FONT_SUPPORT
	MMIAPISET_SetFont(PNULL, TRUE, TRUE);
#endif
	ResetQbThemeRes();
	MMIQBTHEME_ManageUninit();
	MMIRES_NorFileResUnInit();
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetResImgDatByOffset
//  Description : 	MMIQBTHEME_GetResImgDatByOffset
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_GetResImgDatByOffset(
											int32 offset, 
											uint8** data_buff_pptr, 
											uint32* len_ptr
											)
{
	SFS_ERROR_E	sfs_err = 0;
	uint32		read_cnt = 0;	
	
	if (PNULL != data_buff_pptr && PNULL != len_ptr && -1 < offset)
	{
		sfs_err = SFS_SetFilePointer(s_qbtheme_res_handle, (__int64)offset, SFS_SEEK_BEGIN);
		if (SFS_ERROR_NONE == sfs_err)
		{
			sfs_err = SFS_ReadFile(s_qbtheme_res_handle, len_ptr, sizeof(uint32), &read_cnt, 0);
		}
		//SCI_TRACE_LOW:"@@QBTHEME_GetResImgDat:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_APP_1498_112_2_18_2_48_51_0,(uint8*)"ddd", offset, s_qbtheme_res_handle, sfs_err);
		if (SFS_ERROR_NONE == sfs_err)
		{
			*len_ptr = B2L_U32(len_ptr);
			MMIRES_ReadImageDataFromFile(s_qbtheme_res_handle, data_buff_pptr, *len_ptr);
		}
	}	
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetResImgDataByPath
//  Description : 	MMIQBTHEME_GetResImgDataByPath
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC int32 MMIQBTHEME_GetResImgDataByPath(
											wchar* path_ptr, 
											uint8** data_buff_pptr, 
											uint32* len_ptr
											)
{
	int32 nPos = -1;
	int l = 0;
	int m = 0;
	int r = s_qbtheme_hash_tab_count - 1;
	uint32 hash = 0;
	SFS_ERROR_E  sfs_err = 0;
	uint32 path_len = 0;
	uint32 read_cnt = 0;
	wchar hash_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	
	if (0 == s_qbtheme_hash_tab_count
		|| PNULL == s_qbtheme_res_tab_ptr
        || PNULL == path_ptr)
	{
		return (-1);
	}
	hash = MMI_WDJBHash(path_ptr);
	
	while (l <= r)
	{
		m = l + (r - l) / 2;
		if (hash == s_qbtheme_res_tab_ptr[m].hash)
		{
			nPos = s_qbtheme_res_tab_ptr[m].offset;
			break;
		}
		else if (hash < s_qbtheme_res_tab_ptr[m].hash)
		{
			r = m - 1;
		}
		else
		{
			l = m + 1;
		}
	}
	
	while((-1 != nPos) && (hash == s_qbtheme_res_tab_ptr[--m].hash)); //lint !e722
	
	if (-1 != nPos)
	{
		while (hash == s_qbtheme_res_tab_ptr[++m].hash)
		{
			nPos = s_qbtheme_res_tab_ptr[m].offset;		
			sfs_err = SFS_SetFilePointer(s_qbtheme_res_handle, (__int64)nPos, SFS_SEEK_BEGIN);
			if (SFS_ERROR_NONE == sfs_err)
			{
				sfs_err = SFS_ReadFile(s_qbtheme_res_handle, &path_len, sizeof(uint32), &read_cnt, 0);
			}
			if (SFS_ERROR_NONE == sfs_err)
			{
				path_len = B2L_U32(&path_len);
				sfs_err = SFS_ReadFile(s_qbtheme_res_handle, hash_path, path_len, &read_cnt, 0);
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
		SFS_GetFilePointer(s_qbtheme_res_handle, SFS_SEEK_BEGIN, &file_pos); 
		nPos = (((file_pos + 3) >> 2) << 2);
	}
	
	if (-1 != nPos)
	{
		MMIQBTHEME_GetResImgDatByOffset(nPos, data_buff_pptr, len_ptr);
	}
	
	return nPos;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_UdiskMountCallback
//  Description : 	MMIQBTHEME_UdiskMountCallback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_UdiskMountCallback(
										  MMIEVENT_UDISK_DEV_E device,
										  BOOLEAN mount
										  )
{
	if (DEV_UDISK == device
		&& STYLE_IS_SUPPORT_QBTHEME)
	{
		//SCI_TRACE_LOW:"@@QBTHEME UdiskMount:%d \n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_APP_1607_112_2_18_2_48_51_1,(uint8*)"d", mount);
		
		if (mount)
		{
			MMIAPIQBTHEME_StyleStop();
		}
		else
		{
			MMIAPIQBTHEME_StyleStart();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// font
/*******************************************************************/
//  Interface:		MMIQBTHEME_GetPkgFontSize
//  Description : 	MMIQBTHEME_GetPkgFontSize
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint32 MMIQBTHEME_GetPkgFontSize(
                                        QBTHEME_PACKAGE_INFO_T* info_ptr
                                        )
{
    uint32 size = 0;
    
#ifdef QBTHEME_FONT_SUPPORT    
    if (PNULL != info_ptr)
    {
        if ((uint32)-1 == info_ptr->font_size)
        {
            wchar font_file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
            if (GetFontFilePathByFolder(info_ptr->folder, font_file_path))
            {
                SFS_HANDLE font_handle = 0;
                
                font_handle = SFS_CreateFile(font_file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
                if (0 != font_handle)
                {
                    SFS_GetFileSize(font_handle, &size);
                }
                SFS_CloseFile(font_handle);
            }
            info_ptr->font_size = size;
        }
        else
        {
            size = info_ptr->font_size;
        }
    }
#endif	
    
    return size;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_LoadThemeFont
//  Description : 	MMIQBTHEME_LoadThemeFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_LoadThemeFont(
                                        QBTHEME_PACKAGE_INFO_T* info_ptr
                                        )
{
    BOOLEAN bret = TRUE;
    
#ifdef QBTHEME_FONT_SUPPORT     
    if (PNULL != info_ptr
        && THEME_DEFAULT == MMIQBTHEME_GetCurFontType())
    {
        wchar font_file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
        
        bret = GetFontFilePathByFolder(info_ptr->folder, font_file_path);
        bret = MMIRES_LoadFileFont(font_file_path);
        if (!bret)
        {
            uint8 font_type = SYSTEM_DEFAULT;
            MMIQBTHEME_SetCurFontType(font_type);
            MMINV_WRITE(QBTHEME_NV_FONT, &font_type);
        }
    }
    else
    {
        bret = MMIRES_UnloadFileFont();
    }
#endif
    
    return bret;
}
