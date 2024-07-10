/****************************************************************************
** File Name:      mmisms_filewin.h                                         *
** Author:                                                                  *
** Date:           08/15/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file win.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
**   DATE              NAME             DESCRIPTION                        *
** 08/15/2012        jixin.xu          Create for sms reconstruction
** 
****************************************************************************/
#if defined(MMI_SMS_EXPORT_SUPPORT)

#ifndef _MMISMS_FILEWIN_H_
#define _MMISMS_FILEWIN_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmifilearray_export.h"
#include "mmisms_app.h"
#include "mmisms_filecontrol.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : to open the window to set the export position
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterExportMainWin(void);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to set the export path
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSetExportPathWin(void);
#else
/*****************************************************************************/
//     Description : to open the window to set the export path
//    Global resource dependence : 
//  Author: jixin.xu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSetExportPathWin(void);
#endif

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
//  Description : to export all mared message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ExportAllMarkedMsg( 
                      MMISMS_BOX_TYPE_E    box_type
                      );

/*****************************************************************************/
//  Description : to handle the message of export waiting window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleExportWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_FILEWIN_H_
#endif //#if defined(MMI_SMS_EXPORT_SUPPORT)
