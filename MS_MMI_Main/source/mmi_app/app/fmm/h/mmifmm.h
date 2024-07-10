/*****************************************************************************
** File Name:      mmifmmsrv_api.h                                           *
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

#ifndef _MMIFMM_H_
#define _MMIFMM_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_srv.h"
#include "mmifmm_export.h"
#include "mmifmm_drm.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ticked file name in open file window
//  Global resource dependence : 
//  Author: Jassmine 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_GetOpenFileWinTickedFile(
                                               wchar   *file_name_ptr, //in:
                                               uint16  *name_len_ptr   //in/out:in传入file name max len
                                               );

/*****************************************************************************/
// Description :是否正在预览铃声
// Global resource dependence : 
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsPreviewMusic(void);

/*****************************************************************************/
// Description : 给选中的文件打勾
// Global resource dependence : 
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_DrawTickedOnFile(uint16 tick_file_index);


#endif