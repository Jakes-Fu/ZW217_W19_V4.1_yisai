/*****************************************************************************
** File Name:      mmifmm_autotest.h                                         *
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

#ifndef _MMIFMM_AUTOTEST_H_
#define _MMIFMM_AUTOTEST_H_


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


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : create folders in same path
//  Global resource dependence : s_fmm_current_path_ptr
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderHori(uint16 hori_num);

/*****************************************************************************/
//  Description : create folders across path
//  Global resource dependence : s_fmm_current_path_ptr
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderVerti(uint16 Veri_num);


#endif
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Parameter: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMTrace(
                               const MMIFMM_DATA_INFO_T *list_data_ptr,  
                               const MMIFMM_PATH_INFO_T *current_path_ptr,  
                               const uint32 index
                               );
#endif