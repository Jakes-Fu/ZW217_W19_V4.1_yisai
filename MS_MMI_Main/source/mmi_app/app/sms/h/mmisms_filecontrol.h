/****************************************************************************
** File Name:      mmisms_filecontrol.h                                     *
** Author:                                                                  *
** Date:           08/15/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file ctrl.             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
**   DATE              NAME             DESCRIPTION                        *
** 08/15/2012        jixin.xu          Create for sms reconstruction
** 
****************************************************************************/
#if defined(MMI_SMS_EXPORT_SUPPORT)

#ifndef _MMISMS_FILECONTROL_H_
#define _MMISMS_FILECONTROL_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmifilearray_export.h"
#include "mmisms_app.h"
//#include "mmisms_filewin.h"

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
//  Description : get device name
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC wchar *MMISMS_ExportFile_GetDeviceName(uint16 *device_name_len);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_ExportItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num);

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
//  Description : free export file ptr
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FreeExportFilePtr(void);

/*****************************************************************************/
//  Description : to create SMS file
//  Global resource dependence : g_mmisms_global
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CreateExportMsgFile(wchar *s_dsp_path_str);

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDefaultExportPath(
                                     wchar      *full_path_ptr,     //out
                                     uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                     );

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
//  Description : to export the SMS to file
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ExportMsgToFile(void);
 
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
//  Description : get export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMISMS_GetExportFileHandle(void);

/*****************************************************************************/
//  Description : set export sms from which boxS
//                  TRUE:        do export all SMS operation  
//                  FALSE:    without do export all SMS operation
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportBoxType(MMISMS_BOX_TYPE_E export_box_type);

/*****************************************************************************/
//  Description : get export sms from which box
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetExportBoxType(void);

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

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif

#endif //_MMISMS_FILECONTROL_H_
