/*****************************************************************************
** File Name:      mmidrm.h                                                  *
** Author:                                                                   *
** Date:           02/23/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to deal with DRM-related operations     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/03/2011     haiwu.chen       Create                                    *
******************************************************************************/


#ifndef _MMIDRM_H_
#define _MMIDRM_H_


#if defined(DRM_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "drm_api.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmidrm_export.h"
#include "mmi_drmfilemgr.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmi_appmsg.h"

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

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    char* rights_buffer_ptr;            //DRM rights download buffer pointer
    uint32 rights_buffer_len;           //DRM rights download buffer len
    BOOLEAN is_wbxml_rights;            //DRM rights encode type

} MMIDRM_RO_SAVE_WIN_DATA_T; //win data

typedef struct
{
    BOOLEAN is_include_sd_timer_out;    //暂不使用:是否包含超时时间(SD类型文件时)
    uint32 timer_out;                   //超时时间
    MIME_TYPE_E drm_mo_type;            //the media type in the DRM
} MMIDRM_DL_DATA_T;                     //the DRM file download from browser

typedef struct
{
    uint16 *download_file_name_ptr;     //DRM download file path name
    uint16 *new_file_name_ptr;          //DRM install new file name
    MMIDRM_DL_DATA_T dl_data;           //the DRM file info from browser download
    MIME_TYPE_E mime_type;              //the type of the DRM file
    char *url_ptr;                      //from browser(for Java)
    uint16  is_hava_saved;
    //DRM_FILE_TYPE_E install_file_type; //DRM install file type
} MMIDRM_INSTALL_DATA_T; //the DRM file download from browser

typedef struct
{
    wchar       *full_path_ptr;         //全路径,改名之前的名字，改名之后再保存到这里
    uint32      win_id;                 //通知窗口
    uint32      msg_id;                 //通知消息
} MMIDRM_RENAME_WIN_PARAM_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reg browser info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_RegBrowserInfo(void);

/*****************************************************************************/
//  Description : install sd type ro file from push message
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_InstallRoFile(
    char* rights_buffer_ptr,            //DRM rights download buffer pointer
    uint32 rights_buffer_len,           //DRM rights download buffer len
    BOOLEAN is_wbxml_rights,            //DRM rights encode type
    BOOLEAN is_trust_rights             //whether trust rights
);

/*****************************************************************************/
//  Description : MMIDRM_HandleDrmControlMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理drm反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_HandleDrmControlMsg(MMI_MESSAGE_ID_E msg_id, void *param);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : 处理通过浏览下载到的DRM文件
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDRM_HandleDLDrmCB(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr);
#endif

/*****************************************************************************/
//  Description : start to install the original DRM file,then open a waiting win or a fail alert win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_StartToInstallDrm(
    wchar *file_name_ptr,               //[IN]
    MIME_TYPE_E mime_type,              //[IN]
    char *url_ptr,                      //[IN]
    MMIDRM_DL_DATA_T *dl_data_ptr,       //[IN]
    uint16  is_hava_saved
);

/*****************************************************************************/
//  Description : handle download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIDRM_HandleDrmDLRoQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/


#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_SUPPORT)

#endif  //_MMIDRM_H_
