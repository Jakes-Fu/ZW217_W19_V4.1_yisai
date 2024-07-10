/*****************************************************************************
** File Name:      mmiautodemo_export.h                                       *
** Author:         chunjie.liu                                                *
** Date:           2006/09/25                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pb menu id                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/03/25     chunjie.liu       Create                                    *
******************************************************************************/

#ifndef _MMIAUTODEMO_EXPORT_H_
#define _MMIAUTODEMO_EXPORT_H_
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to open autodemo main menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUTODEMO_OpenAutodemoWindow(void);

/*****************************************************************************/
//  Description : to open DiskOpt menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUTODEMO_OpenDiskOptWindow(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif // _MMIAUTODEMO_EXPORT_H_

