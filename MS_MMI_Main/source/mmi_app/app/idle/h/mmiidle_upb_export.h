/*****************************************************************************
** File Name:      mmiidle_upb_export.h                                            *
** Author:                                                                   *
** Date:           05/14/2012                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2012        kylejin             Create                                 *   
******************************************************************************/
#ifdef UPB_IDLE_SUPPORT

#ifndef _MMIIDLE_UPB_EXPORT_H_
#define _MMIIDLE_UPB_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

#include "mmk_type.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/ 
PUBLIC void MMIIDLE_UPB_Start(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
PUBLIC void UPB_DestoryAnim(void);
#endif /*_MMIIDLE_UPB_EXPORT_H_*/
#endif