/*****************************************************************************
** File Name:      mmipic_crop_export.h                                                  *
** Author:           chunyou                                                     *
** Date:             12/25/2011                                                *
** Copyright:      2011 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_crop.c  mmipic_crop_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       chunyou           Create
******************************************************************************/
#ifndef _MMIPIC_CROP_EXPORT_H_
#define _MMIPIC_CROP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "sysTypes.h"
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
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIPIC_CROP_WALLPAPER = 0,      //墙纸裁剪
    MMIPIC_CROP_DEFAULT_SAVE,        //保存在默认路径
    MMIPIC_CROP_CUSTOMIZE_SAVE,        //保存在自定义路径
    MMIPIC_CROP_TYPE_MAX
}MMIPICCROP_TYPE_E;

typedef enum
{
    MMI_PICCROP_SUCCESS = 0,          //成功
    MMI_PICCROP_SUCCESS_NO_CROP,          //成功,没有裁剪，图片尺寸太小
    MMI_PICCROP_ERR_NO_SPACE,       // 空间不足
    MMI_PICCROP_ERR_NOT_SUPPORT,       // 格式不支持
    MMI_PICCROP_ERR_NO_MEMORY,       // 内存不足
    MMI_PICCROP_ERR_CANCEL,       // 取消裁剪
    MMI_PICCROP_ERR_EXIT,            // 其他需要退出图片裁剪的错误
}MMI_PICCROP_ERROR_TYPE_E;

typedef struct 
{
    MMIPICCROP_TYPE_E type;     //裁剪类型，与输入的值一致
    MMI_PICCROP_ERROR_TYPE_E err_type;  //错误类型
    wchar* full_path_ptr;       //文件路径  
    uint16 full_path_len;       //文件路径长度
    BOOLEAN      is_pre_wallpaper;      //是否是恢复前一张墙纸，与输入的值一致
} MMIPICCROP_CB_T;

typedef void (*MMIPICCROP_CALLBACK)(MMIPICCROP_CB_T* callback);
typedef struct 
{
    uint16      min_width;                /*!< 裁剪后图片的最小宽高，为0表示不限制 */
    uint16      min_height;               /*!< 裁剪后图片的最小宽高，为0表示不限制 */
    uint8       *src_buf_ptr;       /*!< 图像buffer地址 @不为pnull时有效 */
    uint32      src_data_size;      /*!< 图像buffer大小 @src_buf_ptr不为pnull src_data_size不为0时有效 */
    wchar      *full_path_ptr;     /*!< 图像文件路径 @不为pnull时有效 */
    uint16      full_path_len;      /*!< 图像文件路径长度 @full_path_ptr不为pnull full_path_len不为0时有效 */
    MMIPICCROP_TYPE_E crop_type;
    MMIPICCROP_CALLBACK callback;     /*裁剪图片结果，full_path_len=0表示失败*/    
    BOOLEAN      is_pre_wallpaper;      //是否是恢复前一张墙纸
} MMIPICCROP_SRC_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_ExitFromPicCrop(void);

/*****************************************************************************/
//  Description : init pic crop module
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_InitModule(void);

/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICCROP_OpenPicCrop(
                                              MMIPICCROP_SRC_T* crop_src    //in:
                                              );
#endif

