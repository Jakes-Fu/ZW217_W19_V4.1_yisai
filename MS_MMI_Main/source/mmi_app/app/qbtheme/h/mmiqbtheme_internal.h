/*****************************************************************************
** File Name:      mmiqbtheme_internal.h                                            *
** Author:         cheng.luo                                                         *
** Date:           2011/09/16                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2011/09/16			cheng.luo			  created						 		                      *
******************************************************************************/

#ifndef _MMI_QBTHEME_INTERNAL_H_
#define _MMI_QBTHEME_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmi_event_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define QBTHEME_ROOT_PATH			L"E:\\theme"
#define QBTHEME_SLASH				L"\\"
#define QBTHEME_DOT					L'.'
#define QBTHEME_SLASH_CH			L'\\'
#define QBTHEME_PACKAGE_INFO_FILE	L"info"
#define QBTHEME_WALLPAPER_PATH		L"\\Desktop\\Wallpaper"
#define QBTHEME_WALLPAPER_ORIG_PATH L"\\Desktop\\Wallpaper\\Original"
#define QBTHEME_ICON_PATH			L"\\Icon"
#define QBTHEME_ICON_PATH_LEN		5
#define QBTHEME_ICON_BG_PATH		L"\\MainMenu\\IconBg.img"
#define QBTHEME_FONT_PATH			L"\\Font"
#define QBTHEME_FONT_PATH_LEN		5
#define QBTHEME_IMG_POSTFIX			L".img"
#define QBTHEME_RES_FILE			L"Qbtheme.tab"

#define QBTHEME_AUTHOR_MAX_LEN	31
#define QBTHEME_NAME_MAX_LEN	31
#define QBTHEME_NAME_EN_MAX_LEN	31		
#define QBTHEME_FOLDER_MAX_LEN	31
#define QBTHEME_FONT_MAX_LEN	31
#define QBTHEME_POSIX_MAX_LEN	7

#define U32_TAG(a, b, c, d) ((uint32)(((a)<<24)|((b)<<16)|((c)<<8)|(d)))
#define QBTHEME_INFO_TAG		(U32_TAG('Q','I','N','F')) // (U32_TAG('F','N','I','Q'))
#define QBTHEME_PACKAGE_TAG		(U32_TAG('Q','B','T','H')) // (U32_TAG('H','T','B','Q'))
#define QBTHEME_RES_TAG			(U32_TAG('I','B','T','H')) // (U32_TAG('H','T','B','I'))

#define SYSTEM_DEFAULT	0x00	//系统自带
#define THEME_DEFAULT	0x01	//主题自带
#define DEFAULT_NONE	0xff	//无

//和theme pkg的version前2位匹配
#define QBTHEME_MAJOR_VER       '1'
#ifdef MAIN_MENU_60X60
#define QBTHEME_MINOR_VER       '2'
#else
#define QBTHEME_MINOR_VER       '1'
#endif

#define QBTHEME_DOWNLOAD_URL            "http://51wre.com/theme/list?"
#define QBTHEME_DOWNLOAD_URL_MAX_LEN    127

#if (defined FONT_TYPE_SUPPORT_VECTOR) \
     && (defined VECTOR_FONT_SUPPORT_DEFAULT) \
     && (defined FILE_FONT_SUPPORT)
    #define QBTHEME_FONT_SUPPORT
#endif

 /**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 typedef struct
 {
	 uint32		pkg_size;
	 uint32     font_size;
	 wchar		name_zh[QBTHEME_NAME_MAX_LEN + 1];
     wchar		name_en[QBTHEME_NAME_EN_MAX_LEN + 1];
	 wchar		author[QBTHEME_AUTHOR_MAX_LEN + 1];
	 wchar		folder[QBTHEME_FOLDER_MAX_LEN + 1];
 }QBTHEME_PACKAGE_INFO_T;

 typedef enum
 {
	QB_IMG_OVERVIEW,
	QB_IMG_PREVIEW1,
	QB_IMG_PREVIEW2,
	QB_IMG_PREVIEW3,
	QB_IMG_ICON_PRIVIEW,
	QB_IMG_ICON_BG,
	QB_IMG_FONT_SAMPLE,
	QB_IMG_FONT_PREVIEW,
	
	QB_IMG_MAX
 }QBTHEME_IMAGE_ID_E;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIQBTHEME_RegWinIdNameArr(void);

PUBLIC BOOLEAN MMIQBTHEME_GetLocalRootFolder(wchar *root_ptr);
PUBLIC QBTHEME_PACKAGE_INFO_T *MMIQBTHEME_GetCurThemeInfo(void);
PUBLIC QBTHEME_PACKAGE_INFO_T *MMIQBTHEME_GetInfoByIndex(uint16 index);
PUBLIC uint16 MMIQBTHEME_GetIndexByName(wchar* name_ptr);
PUBLIC BOOLEAN MMIQBTHEME_ManageInit(void);
PUBLIC void MMIQBTHEME_ManageUninit(void);
PUBLIC BOOLEAN MMIQBTHEME_GetImagePath(
									   QBTHEME_PACKAGE_INFO_T *pkg_info_ptr,
									   QBTHEME_IMAGE_ID_E img_id,
									   wchar* path_ptr,
									   uint32 max_len
									   );
PUBLIC uint32 MMIQBTHEME_GetDownloadCount(void);

PUBLIC wchar* MMIQBTHEME_GetWallpaperName(uint16 index);
PUBLIC uint32 MMIQBTHEME_GetWallpaperSize(uint16 index);
PUBLIC uint32 MMIQBTHEME_GetThemeWallpaperCount(void);
PUBLIC void MMIQBTHEME_DestroyWallpaperList(void);
PUBLIC void MMIQBTHEME_WallpaperListInit(QBTHEME_PACKAGE_INFO_T *pkg_info_ptr);
PUBLIC void MMIQBTHEME_GetWallpaerPreviewPath(QBTHEME_PACKAGE_INFO_T *pkg_info_ptr, uint16 index, wchar* path_ptr);
PUBLIC uint32 MMIQBTHEME_GetPkgFontSize(QBTHEME_PACKAGE_INFO_T* info_ptr);
PUBLIC BOOLEAN MMIQBTHEME_LoadThemeFont(QBTHEME_PACKAGE_INFO_T* info_ptr);

PUBLIC uint32 MMIQBTHEME_Wstrrchr(wchar *src_ptr, wchar ch);
PUBLIC void MMIQBTHEME_RestMapOffset(void);

PUBLIC MMI_RESULT_E MMIQBTHEME_HandleRefInstallMsg(
                               PWND                app_ptr, 
                               uint16              msg_id, 
                               DPARAM              param
                               );
PUBLIC void MMIQBTHEME_RegBrowserCB(void);
PUBLIC void MMIQBTHEME_UdiskMountCallback(
										  MMIEVENT_UDISK_DEV_E device,
										  BOOLEAN mount
										  );
PUBLIC void MMIAPIQBTHEME_QueryApplyWin(QBTHEME_PACKAGE_INFO_T* info_ptr);

PUBLIC BOOLEAN MMIQBTHEME_InitResHandle(QBTHEME_PACKAGE_INFO_T *pkg_info_ptr);
PUBLIC void MMIQBTHEME_GetResImgDatByOffset(int32 offset, uint8** data_buff_pptr, uint32* len_ptr);
PUBLIC int32 MMIQBTHEME_GetResImgDataByPath(wchar* path_ptr, uint8** data_buff_pptr, uint32* len_ptr);


PUBLIC uint8 MMIQBTHEME_GetCurIconType(void);
PUBLIC uint8 MMIQBTHEME_GetCurIconBgType(void);
PUBLIC uint8 MMIQBTHEME_GetCurFontType(void);
PUBLIC void MMIQBTHEME_SetCurIconType(uint8 type);
PUBLIC void MMIQBTHEME_SetCurIconBgType(uint8 type);
PUBLIC void MMIQBTHEME_SetCurFontType(uint8 type);
PUBLIC wchar* MMIQBTHEME_GetCurWallpaper(void);
PUBLIC wchar* MMIQBTHEME_GetCurrentTheme(void);
PUBLIC void MMIQBTHEME_SetNeedTransform(BOOLEAN is_transform);
PUBLIC void MMIQBTHEME_SetForceTransform(BOOLEAN is_force);
PUBLIC BOOLEAN MMIQBTHEME_GetForceTransform(void);
PUBLIC BOOLEAN MMIQBTHEME_Uninstall(const wchar* name_ptr);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMI_QBTHEME_INTERNAL_H_
