/*****************************************************************************
** File Name:      mmicom_extract.h                                                   *
** Author:                                                                   *
** Date:           2012/01/04                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe extract number/ur               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 
******************************************************************************/

#ifndef _MMICOM_EXTRACT_H_
#define _MMICOM_EXTRACT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmk_type.h"


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
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :Extract Number Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractNumberWin(MMI_WIN_ID_T win_id, wchar * num_ptr, uint16 num_len, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :Extract Url Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractUrlWin(MMI_WIN_ID_T win_id, wchar * url_ptr, uint16 url_len, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :Extract Email Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractEmailWin(MMI_WIN_ID_T win_id, wchar * email_ptr, uint16 email_len, MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

