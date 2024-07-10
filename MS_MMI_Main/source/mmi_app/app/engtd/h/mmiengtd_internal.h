/*****************************************************************************
** File Name:      mmiengtd_internal.h                                                *
** Author:                                                                   *
** Date:           2007/10/26                                                *
** Copyright:      2007 Spreadrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe engtd module export function    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/10/26     yongwei.he      Create                                   *
******************************************************************************/

#ifndef _MMI_ENGTD_INTERNAL_H
#define _MMI_ENGTD_INTERNAL_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmiengtd_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/********************************************************************************
 NAME:			MMIENGTD_RegWinIdNameArr
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIENGTD_RegWinIdNameArr(void);

/********************************************************************************
 NAME:			MMIENGTD_GetVTVideoType
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
PUBLIC void MMIENGTD_GetVTVideoType(MMIENG_VT_T * video_type);

/********************************************************************************
 NAME:			MMIENGTD_SetVTVideoType
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
PUBLIC void MMIENGTD_SetVTVideoType(MMIENG_VT_T * video_type);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif/* _MMI_ENGTD_INTERNAL_H */
