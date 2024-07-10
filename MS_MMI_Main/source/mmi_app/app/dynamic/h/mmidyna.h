#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************
** File Name:      mmidyna.h                                                   *
** Author:                                                                   *
** Date:           20/08/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe csk2 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2008       peng                Create
******************************************************************************/

#ifndef _MMIDYNA_H_
#define _MMIDYNA_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmidyna_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
 **                         INTERFACE TO OTHER MMI MODULE                                      *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register dyna menu group
//	Global resource dependence : none
//  Author: peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIDYNA_RegMenuGroup(void);

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDYNA_RegWinIdNameArr(void);


#ifdef   __cplusplus
    }
#endif

#endif
#endif

