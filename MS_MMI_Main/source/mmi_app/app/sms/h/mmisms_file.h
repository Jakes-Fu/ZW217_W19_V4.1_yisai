/****************************************************************************
** File Name:      mmisms_file.h                                            *
** Author:                                                                  *
** Date:           06/12/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file.                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME             DESCRIPTION                             *
** 06/12/2007     fengming.huang           Create
** 
****************************************************************************/
#if defined(MMI_SMS_EXPORT_SUPPORT)

#ifndef _MMISMS_FILE_H_
#define _MMISMS_FILE_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmifilearray_export.h"
#include "mmisms_app.h"

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
#define MMISMS_FILE_EXT   ".txt"

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*MMISMS_OpenFileCallBack)(BOOLEAN is_success, FILEARRAY file_array);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : to open the window to set the export position
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSetExportPosWin(void);

/*****************************************************************************/
//     Description : to open the window to set the export path
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSetExportPathWin(void);

/*****************************************************************************/
//  Description : get export path assigned by user
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_GetCustomizedExportPath(
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            );

/*****************************************************************************/
//  Description : Get export text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetDisplayPathText(
                                    MMI_STRING_T *path_value_str_ptr,
                                    wchar *path_name_ptr,
                                    uint16 *path_name_len_ptr);

/*****************************************************************************/
//  Description : Get Real path text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetRealPathText( MMI_STRING_T *path_value_str_ptr,
                                    wchar *path_name_ptr,
                                    uint16 *path_name_len_ptr);

/*****************************************************************************/
//  Description : Appoint export save path text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_AppointExportSavePath(wchar *path_ptr, uint32 path_len, MMISMS_OpenFileCallBack callback);

/*****************************************************************************/
//  Description : to export the SMS box called by window
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppExportMsgBox(
                                                MMISMS_BOX_TYPE_E    type,    //IN:
                                                uint16* msg_type,  //out
                                                MMISMS_FOLDER_TYPE_E folder_type
                                                );

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CombineExportFile(
                                 MMI_STRING_T *path_ptr,
                                 MMI_STRING_T *name_ptr,
                                 wchar      *full_path_ptr,     //out
                                 uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                 BOOLEAN is_combine_tmp_file
                                 );

/*****************************************************************************/
//  Description : set export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportTotalNum(uint32 total_num);

/*****************************************************************************/
//  Description : get export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportTotalNum(void);

/*****************************************************************************/
//  Description : set export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurrentNum(uint32 current_num);

/*****************************************************************************/
//  Description : get export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportCurrentNum(void);

/*****************************************************************************/
//  Description : set export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportFileHandle(MMIFILE_HANDLE file_handle);

/*****************************************************************************/
//  Description : get export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMISMS_GetExportFileHandle(void);

/*****************************************************************************/
//  Description : set export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportSecurityBoxFlag(BOOLEAN is_export);

/*****************************************************************************/
//  Description : get export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetExportSecurityBoxFlag(void);

/*****************************************************************************/
//  Description : set export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//  Description : get export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetExportCurBoxType(void);

/*****************************************************************************/
//  Description : free export file ptr
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FreeExportFilePtr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_ID_H_
#endif //#if defined(MMI_SMS_EXPORT_SUPPORT)
