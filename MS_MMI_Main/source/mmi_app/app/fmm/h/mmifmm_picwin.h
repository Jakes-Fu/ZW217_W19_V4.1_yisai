/*****************************************************************************
** File Name:      mmifmm_picwin.h                                           *
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

#ifndef _MMIFMM_PICWIN_H_
#define _MMIFMM_PICWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmifmm_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    wchar       filename[MMIFILE_FULL_PATH_MAX_LEN+1]; //文件名或文件夹名
    uint32      name_len;
} MMIFMM_PIC_PERVIEW_WIN_DATA_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open pic preview win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterSearchPreviewWin (MMIFMM_PIC_PERVIEW_WIN_DATA_T *file_info_ptr);
/*****************************************************************************/
// Description : show file error type
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_EnterPreviewPicture (wchar *full_path_name_ptr);

#endif