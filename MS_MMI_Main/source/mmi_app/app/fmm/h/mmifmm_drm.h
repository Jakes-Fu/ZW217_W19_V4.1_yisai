/*****************************************************************************
** File Name:      mmifmm_drm.h                                              *
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
#ifndef _MMIFMM_DRM_H_
#define _MMIFMM_DRM_H_

#include "mmk_type.h"
#include "mmifmm_export.h"
#include "mmisrvfmm_export.h"
#include "os_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef DRM_SUPPORT

#define MMIFMM_DRM_CONS_LIMIT_TYPE      (0x0001)
#define MMIFMM_DRM_CONS_LIMIT_COUNT     (MMIFMM_DRM_CONS_LIMIT_TYPE<<0)
#define MMIFMM_DRM_CONS_LIMIT_INTERVAL  (MMIFMM_DRM_CONS_LIMIT_TYPE<<1)
#define MMIFMM_DRM_CONS_LIMIT_DATETIME  (MMIFMM_DRM_CONS_LIMIT_TYPE<<2)

#define MMIFMM_DRM_METHOD_LIMIT_TYPE    (0x0001)
#define MMIFMM_DRM_METHOD_LIMIT_FL      (MMIFMM_DRM_METHOD_LIMIT_TYPE<<0)
#define MMIFMM_DRM_METHOD_LIMIT_CD      (MMIFMM_DRM_METHOD_LIMIT_TYPE<<1)
#define MMIFMM_DRM_METHOD_LIMIT_SD      (MMIFMM_DRM_METHOD_LIMIT_TYPE<<2)


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set the select drm file limit value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_IsSelDrmConstraintFile(
//    uint16 *filename, 
//    uint16 name_len, 
//    MMIFMM_DRM_LIMIT_TYPE_T limit_value
// );

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 是否可以预览，如果不能预览，则需要提示下载
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateListIconData(
    MMI_CTRL_ID_T ctrl_id, 
    uint16 index,
    const wchar *file_dir_ptr,              //文件所在文件夹名称
    const wchar *file_path_ptr              //文件全路径
);
#endif

#endif