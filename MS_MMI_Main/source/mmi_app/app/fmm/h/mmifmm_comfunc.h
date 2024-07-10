/*****************************************************************************
** File Name:      mmifmm_comfunc.c                                          *
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
#ifndef _MMIFMM_COMFUNC_H_
#define _MMIFMM_COMFUNC_H_

#include "mmk_type.h"
#include "mmifmm_export.h"
#include "mime_type.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : combine  filename with path
//  Global resource dependence : 
//  Author: robert.lu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFMM_CombineToFileName(
                                const wchar      *pathname_ptr, //in
                                uint16      pahname_len,           //in
                                const wchar      *filename_ptr,     //in
                                uint16      filename_len,       //in
                                wchar      *full_path_filename_ptr,//out
                                uint16      full_path_filename_len_ptr//out
                                );
/*****************************************************************************/
//  Description : combine full file name
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFMM_CombineFullFileName(
                                    const MMIFMM_DATA_INFO_T      *list_data_ptr, //in
                                    const MMIFMM_PATH_INFO_T      *current_path_ptr,      //in
                                    const uint32                   index,                 //in
                                    wchar                   *file_name_ptr,         //out
                                    const uint32                   file_name_max_len      //in
                                    );
/*****************************************************************************/
//  Description : printf error code
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_RaiseError(uint32 error_index);


/*****************************************************************************/
//  Description : crrect folder name, such as remove "."," ",from tail
//  Global resource dependence : none
//  Parameter: folder_name_ptr [IN/OUT]: orignal folder, after change, 
//                                       it will return corrected folder name
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIFMM_CorrectFolderName(wchar *folder_name_ptr);

/*****************************************************************************/
// Description : 新建目录名字是否有效
// Global resource dependence : 
// Author: Ming.Song 2007-4-18
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIFMM_NewFolderNameStringIsValid(
                                         const MMI_STRING_T     *file_name_ptr, //in:
                                         uint32                     ucs2_max_len    //in
                                         );
/*****************************************************************************/
//  Description : get finally slash index
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetFinallySlashIndex(wchar *file_name_ptr, int32 file_len);

/*****************************************************************************/
//  Description : is file need display
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsFileNeedDisplay(uint32 suffix_type,wchar *name_ptr,uint16 name_len);

/*****************************************************************************/
//  Description : get path depth
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetFullPathDepth(wchar *path_ptr,uint32 path_len);

/*****************************************************************************
 	Description : Convert the character from upper to lower 
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_ConvertToLower(uint16 *ch);

/*****************************************************************************
 	Description : Convert the characer from SBC to DBC.
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_ConvertSBC2DBC(
                                uint16 *ch    //IN/OUT:全角符号转换成半角
                            );

/*****************************************************************************/
// Description : 搜索串是否有效
// Global resource dependence : 
// Author: Ming.Song
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIFMM_FindStringIsValid(
                                   MMI_STRING_T   *file_name_ptr  //in:
                                   );
/*****************************************************************************/
// Description : show file error type  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIFMM_PromptFileErrorType(MMI_WIN_ID_T alert_win_id, SFS_ERROR_E fs_error, MMIFILE_DEVICE_E filedev, BOOLEAN is_file);

/*****************************************************************************/
//  Description : The file is dir
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FileIsFolder(const uint16 file_attr);

/*****************************************************************************/
//  Description : convert suffix type
//  Global resource dependence : 
//  Author: haiwu.chen`
/*****************************************************************************/
PUBLIC uint32 MMIFMM_ConvertMimeTypeToSuffix(
    MIME_TYPE_E mime_type
);

/*****************************************************************************/
//  Description : check copy file path before copying
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsPath1ContainPath2(
                                    uint16   *pathname_1,
                                    uint16   pathname_1_len,
                                    uint16   *pathname_2,
                                    uint16   pathname_2_len
                                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetPathDepth(
                           const wchar *path_ptr,
                           uint16   path_len
                           );

/*****************************************************************************/
//  Description : whether file is in default picture folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDefaultPicFolder(
                                    const wchar      *file_name,
                                    uint16      name_len
                                    );

/*****************************************************************************/
//  Description : get file info form fullpath
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfoFormFullPath(
                                      const wchar *full_file_name,       //in
                                      const uint32 filename_len,          //in
                                      MMIFILE_FILE_INFO_T *file_info//out
                                      );

/*****************************************************************************/
//  Description : get file name from data info
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFMM_FmmGetFileName(MMIFMM_DATA_INFO_T      *list_data_ptr,         //in
                            uint32                   index,                 //in
                            wchar                   *file_name_ptr,         //out
                            uint32                   file_name_max_len      //in
                            );

#endif
