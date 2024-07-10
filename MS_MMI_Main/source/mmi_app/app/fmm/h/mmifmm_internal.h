/*****************************************************************************
** File Name:      mmifmm_internal.h                                                  *
** Author:                                                                   *
** Date:           06/04/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:     file manager modue                                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Robert           Create
******************************************************************************/

#ifndef _MMIFMM_INTERNAL_H_
#define _MMIFMM_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mmi_filetask.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mime_type.h"
#if defined(DRM_SUPPORT)
#include "drm_api.h"
#endif
#include "mmifmm_pic.h"
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

/**--------------------------------------------------------------------------*
**                         GLOBAL VALUE DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 //fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : delete file syn
//  Global resource dependence : 
//  Author: liqing.peng
//  Modify: xinrui.wang
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFMM_DeleteFileSyn(
                             const wchar *full_path_ptr, //in
                             uint16      full_path_len   //in
                              );

/*****************************************************************************/
//  Description : get file information
//  Global resource dependence : 
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFMM_GetFileInfo(
                           const wchar        *full_path_ptr,     //in
                           uint16       full_path_len,      //in
                           uint32       *create_time_ptr,   //out
                           uint32       *file_size_ptr      //out
                           );
#endif

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_AlertNoSpaceInDisk(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note: 
/*****************************************************************************/
uint16 MMIFMM_GetPathDepth(
                           const wchar  *path_ptr,
                           uint16   path_len
                           );
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : get all device info and check if have enough space 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsEnoughSpace(uint32 size);
#endif
/*****************************************************************************/
//  Description : register fmm module nv len and max item
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RegFMMNv(void);

/*****************************************************************************/
//  Description : set display file type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_SetDisplayFileType(uint32 file_type);

/*****************************************************************************/
//  Description : get display file type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIFMM_GetDisplayFileType(void);

/*****************************************************************************/
// Description : 是否选中的是同一种文件类型文件
// Global resource dependence : 
// Author: renyi.hu
// Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_IsSelectedSameFileType(MMIFMM_DATA_INFO_T    *list_data_ptr,
//                                                MMIFMM_FILE_TYPE_E     type
//                                            );


                                
/*****************************************************************************/
//  Description : set tab title
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_SetWinTitle(BOOLEAN is_normal_type,
//                                MMI_WIN_ID_T win_id,                                 
//                                MMI_CTRL_ID_T tab_ctrl_id, 
//                                wchar * current_path,
//                                 uint16  current_path_len);

/*****************************************************************************/
//  Description : 清除列表数据
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_RemoveListData( MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : MMIFMM_IsListMultiSel
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsListMultiSel(void);


/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFMM_IsFilesInFolder(
                            const wchar *full_path_ptr,     //in
                            uint16       full_path_len ,      //in, 双字节为单位
							const wchar *filter//IN:查询字符串
                            );

/*****************************************************************************/
//  Description : check wether file is this folder's sub file 
//  Global resource dependence : 
//  Author:
//  Note: 
//  Return: TRUE:file_name_ptr is in path_name_ptr; FALSE:file_name_ptr isnot in path_name_ptr; 
/*****************************************************************************/
BOOLEAN MMIFILE_FileIsInThisFolder(    
                              const wchar      *path_name_ptr,     //in
                              uint16           path_name_len,      //in, 双字节为单位
                              const wchar      *file_name_ptr,     //IN
                              uint16           file_name_len  //in, 双字节为单位
                              );

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : find files in directory by key word
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
uint16 MMIFMM_FindAndGetFiles(         //返回找到的文件数
                                const wchar             *full_path_ptr,     //in, 查找的文件名(包括通配符*), eg: D:\Music\*.mp3
                                uint16                  full_path_len,      //in, unicode个数
                                MMIFILE_FILE_INFO_T     file_list[],        //out,用于存储找到的文件信息
                                uint16                  file_list_size      //in, file_list 数组的大小
                                );

/*****************************************************************************/
//  Description : read file data syn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_ReadFilesDataSynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size        //in
);

/*****************************************************************************/
//  Description : read file data syn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFMM_ReadFileDataByOffsetSynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    uint32                  offset              //in
);

/*****************************************************************************/
//  Description : write file data by offset syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_WriteFileDataByOffsetSyn(
                                        const wchar            *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode个数
                                        const uint8             *buf_ptr,           //in
                                        uint32                  to_write_size,      //in
                                        uint32                  offset              //in
                                        );

/*****************************************************************************/
//  Description : read file data by offset asyn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_ReadFileDataByOffsetAsyn(
                                        const wchar             *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode个数
                                        uint8                   *buf_ptr,           //out
                                        uint32                  to_read_size,       //in
                                        uint32                  offset,             //in
                                        MMI_WIN_ID_T            win_id,             //in
                                        MMI_MESSAGE_ID_E        msg_id              //in
                                        );
#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : combine drm suffix
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 通过非DRM的后缀，返回组合上drm的后缀
/*****************************************************************************/
//PUBLIC uint32 MMIFMM_CombineDrmSuffixInfo(uint32 suffix_type);


/*****************************************************************************/
//  Description : drm support type
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_IsDrmSupportType(uint32 suffix_type);

/*****************************************************************************/
//  Description : convert suffix type
//  Global resource dependence : 
//  Author: haiwu.chen`
/*****************************************************************************/
PUBLIC uint32 MMIFMM_ConvertMimeTypeToSuffix(
    MIME_TYPE_E mime_type
);

/*****************************************************************************/
//  Description : get drm icon id by full path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetDrmIconId(uint16 *file_path_ptr);
#endif

///*****************************************************************************/
////  Description : 读当前目录中的文件数据，
////  Global resource dependence :
////  Author: robert.lu
////  Note: 文件保存，没有实现
///*****************************************************************************/
//PUBLIC uint16 MMIFMM_GetCurrentPathFile(// 所读文件和目录的总数
//                                           const wchar          *full_path_ptr,     // 全路径,查找条件
//                                           //MMIFMM_READ_TYPE_E   type,               // 类型
//                                           MMIFMM_DATA_INFO_T   *data_ptr         // 输出数据
//                                           //uint16               *output_filename_ptr// 输出数据的文件名
//                                            );
/*****************************************************************************/
//  Description : 预排序，将文件夹排前，文件排后
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_FilePreSort(MMIFMM_DATA_INFO_T* data_ptr);


/*****************************************************************************/
//  Description :MMIFMM_GetPicIndexData
//  Global resource dependence : 
//  Author: juan.wu
//  Note:   
/*****************************************************************************/
PUBLIC MMIFMM_PICTURE_INDEX_DATA_T  ** MMIFMM_GetPicIndexData(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
