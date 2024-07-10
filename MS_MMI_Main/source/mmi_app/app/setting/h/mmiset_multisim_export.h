/*****************************************************************************
** File Name:      mmiset_multisim_export.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    bruce.chi         Create
** 2012/02/12    xiyuan.ma         Edit
******************************************************************************/

#ifndef _MMISET_DUALSIM_EXPORT_H_
#define _MMISET_DUALSIM_EXPORT_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set sim name
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSimName(
                                 MN_DUAL_SYS_E dual_sys, 
                                 MMISET_SIM_NAME_T * sim_name_ptr
                                 );

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
// 	Description : set default sim Color
//	Global resource dependence : 
//  Author: Sam.hua
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultSimColor(void);
#endif

/*****************************************************************************/
// 	Description : reset dual sim factory setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultDualSysSetting(void);

/*****************************************************************************/
// 	Description : set default sim name
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultSimName(BOOLEAN is_lang_change);

/*****************************************************************************/
// 	Description : is sim name context init
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSimNameContextInit(void);

#endif

