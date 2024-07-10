/*****************************************************************************
** File Name:      mmi_resource.h                                                   *
** Author:                                                                   *
** Date:           05/30/2008                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit Histovry                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2008        gang.tong       Create
******************************************************************************/

#ifndef _MMI_RESOURCE_H_
#define _MMI_RESOURCE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "guimenu.h"
#include "mmi_ring.h"
#include "mmi_font.h"
#include "mn_api.h"
#include "mmi_resource_def.h"
#include "guifont.h"
#include "sfs.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_MAX_MODU_NUM                      64  

#ifdef BROWSER_SUPPORT
typedef struct
{
    wchar *name;
    uint16 name_len;
    const char *url;
    uint16 url_len;
}MMIRES_BOOKMARK_ITEM_T;//书签结构

typedef struct
{
    MMIRES_BOOKMARK_ITEM_T *res_item_ptr;
    uint16 res_num;
    BOOLEAN is_res_data;
}MMIRES_BOOKMARK_INFO_T;
#endif

#ifdef MMI_RES_OUTSIDE_SUPPORT

typedef enum
{
    RES_OUT_INVALID = 0,
    RES_OUT_MENU,

    RES_OUT_MAX
}RES_OUT_TYPE_E;

#define MENU_SWEETS         L"SWEETS"

#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Init resource module and info
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_ResourceInit(void);

/*****************************************************************************/
//  Description : load com resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadComResource(uint32 com_id);

/*****************************************************************************/
//  Description : check the text whether is NULL
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_IsTextNull(MMI_TEXT_ID_T text_id);
/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMIRES_GetText(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   );    //[out] the data of text 

/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: 
//  Note: Application should Free str_ptr->wstr_ptr
/*****************************************************************************/
PUBLIC const wchar* MMIRES_GetTextEx(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   );    //[out] the data of text

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetRingInfo(
                                  MMI_RING_ID_T ring_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIAUD_RING_DATA_INFO_T * pRingInfo
                                  );

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetDataInfo(
                                  MMI_DATA_ID_T data_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIRES_DATA_INFO_T * pdataInfo                                  
                                  );

/*****************************************************************************/
//  Description : to get the data by label
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetData(
                                  MMI_DATA_ID_T data_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIRES_DATA_INFO_T * pdataInfo                                  
                                  );

/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
PUBLIC uint8* MMIRES_GetImage(
                              MMI_IMAGE_ID_T image_id,
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr
                              );

/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIRES_GetAnim(
                             MMI_ANIM_ID_T anim_id,
                             CAF_HANDLE_T handle_id, 
                             uint32 *size_ptr
                             );
/*****************************************************************************/
//  Description : get color value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetColor(
                                   MMI_COLOR_ID_T      color_id,     
                                   CAF_HANDLE_T       handle_id,      
                                   GUI_COLOR_T          *color_ptr
                                   );
/*****************************************************************************/
//  Description : get rect value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetRect(
                                   MMI_RECT_ID_T      rect_id,     
                                   CAF_HANDLE_T       handle_id,
                                   GUI_RECT_T       * rect_ptr
                                   );

/*****************************************************************************/
//  Description : Free dynamic resource node of indicated handle from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_FreeNodeResByHandle(CAF_HANDLE_T handle_id);
/*****************************************************************************/
//  Description : Free all dynamic resource node from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_FreeAllNodeRes(void);
/*****************************************************************************/
//  Description : Free dynamic resource node of indicated com id from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_EmptyComRes(uint32 com_id);
/*****************************************************************************/
//  Description : 判断字库是否存在
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_CheckFontLibIsExist(
                            GUI_FONT_T font,
                            BOOLEAN     is_ucs2);
/*****************************************************************************/
//  Description : load new text resource to ram when changing language
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_LoadNewTextRes(void);

/*****************************************************************************/
//  Description : load all static  com resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_LoadAllStaticComResource(void);

/*****************************************************************************/
//  Description : get current theme number
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIRES_GetStaticThemeTotalNum(void);
/*****************************************************************************/
//  Description : load resource and get every module offset for other theme
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: static_theme_id from 1, 0: default theme.
/*****************************************************************************/
PUBLIC void MMIRES_UpdateStaticThemeRes(uint8 static_theme_id);

/*****************************************************************************/
//  Description : load resource and get every module offset for other theme from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC void MMIRES_UpdateFileThemeRes(const wchar* file_name);

/*****************************************************************************/
//  Description : switch static theme
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 横竖屏切换时调用
/*****************************************************************************/
PUBLIC void MMIRES_SwitchStaticThemeRes(void) ;

/*****************************************************************************/
//  Description : get english font lib head info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
//PUBLIC void RES_GetEnglishFontLibHeadInfo(GUI_FONT_HEAD_INFO_T *head_ptr,GUI_FONT_T font);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:分配内存
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadFileFont( 
                                   const uint16 *file_name
                                   );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:释放内存
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_UnloadFileFont( void );

/*******************************************************************/
//  Interface:		MMIRES_ReadImageFromFile
//  Description : 	MMIRES_ReadImageFromFile
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRES_ReadImageDataFromFile(
											SFS_HANDLE handle, 
											uint8** buffer_pptr,
											uint32 read_bytes
											);

/*******************************************************************/
//  Interface:		MMIRES_NorFileResInit
//  Description : 	MMIRES_NorFileResInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRES_NorFileResInit(void);

/*******************************************************************/
//  Interface:		MMIRES_NorFileResUnInit
//  Description : 	MMIRES_NorFileResUnInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRES_NorFileResUnInit(void);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : Load Bookmark Info From Resource
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadBookmarkFromRes(MMIRES_BOOKMARK_INFO_T *res_info_ptr);

/*****************************************************************************/
//  Description : Unload Bookmark Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_UnloadBookmarkInfo(MMIRES_BOOKMARK_INFO_T *res_info_ptr);
#endif

#ifdef MMI_RES_OUTSIDE_SUPPORT
/*******************************************************************/
//  Interface:		MMIRESOUT_FolderInit
//  Description : 	MMIRESOUT_FolderInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_FolderInit(void);
/*******************************************************************/
//  Interface:		MMIRESOUT_IsEnable
//  Description : 	MMIRESOUT_IsEnable
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_IsEnable(void);
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
                              );
/*******************************************************************/
//  Interface:		MMIRESOUT_Uninit
//  Description : 	MMIRESOUT_Uninit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_UnInit(void);
/*******************************************************************/
//  Interface:		MMIRESOUT_Pause
//  Description : 	MMIRESOUT_Pause
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRESOUT_Pause(void);
/*******************************************************************/
//  Interface:		MMIRESOUT_Resume
//  Description : 	MMIRESOUT_Resume
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRESOUT_Resume(void);
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
                                         );
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

