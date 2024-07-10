/*****************************************************************************
** File Name:      mmifmm_vthumbnail.h                                       *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIFMM_VTHUMBNAIL_H_
#define _MMIFMM_VTHUMBNAIL_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guilistbox.h"
#include "mmifmm_export.h"

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能的状态初始化，列表的显示风格
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnail_Init (MMIFMM_PATH_INFO_T* current_path_ptr);

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 刷新未显示的ICONLIST
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnailUpdateIcon (
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
);

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能实时刷新 ICONLIST列表
//  Global resource dependence :
//  Author:liyan.zhu
//  Note:ForCR139456 重构产生的后遗症
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailUpdateIcon();

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中关闭缩略图获取，并重置状态
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailClose (void);

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中添加视频至视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailAdd (wchar  *video_fullpath_name);

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 通过视频全路径获取对应BMP 全路径
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIFMM_FmmVideoThumbnailGetPathname (wchar* thumbnail, uint16 *length, wchar* videopath);
/*****************************************************************************/
//  Description : change state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumnailChangeState (
    GUILIST_SLIDE_STATE_E state,
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
);
/*****************************************************************************/
//  Description : whether file is in default picture folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDefaultVideoFolder (
											const wchar      *file_name,
											uint16      name_len
											);
/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 删除相关的文件
//  Global resource dependence : 
//  Author: 
//  Note: cr130686
/*****************************************************************************/
PUBLIC void  MMIFMM_VideoThumbnailDeleteRelatedFile(wchar *file_name_ptr);

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 加载当前页面未解析的视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIFMM_FmmVideoThumbnailLoadingMovieList (MMIFMM_DATA_INFO_T* list_data_ptr,
        MMIFMM_PATH_INFO_T* current_path_ptr
                                                       );

#endif