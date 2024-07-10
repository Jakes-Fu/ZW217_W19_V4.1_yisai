/*************************************************************************
 ** File Name:      mmijava_nv.h                                            *
 ** Author:         arthur.peng                                       *
 ** Date:           2006/09/15                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/15     arthur.peng      Create.                              *
*************************************************************************/

#ifdef JAVA_SUPPORT_SUN
#ifndef _MMIJAVA_NV_H_
#define _MMIJAVA_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_JAVA_MEDIA_VOLUME = MMI_MODULE_JAVA << 16,	
    MMINV_JAVA_CALLBACK_PARAM,
    
    MMINV_JAVA_MAX_ITEM_NUM
}JAVA_NV_ITEM_E;



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register java module nv len and max item
//	Global resource dependence : none
//  Author: arthur.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegJavaNv(void);

/*****************************************************************************/
// 	Description : Read Download Manager
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_ReadOtaCallbackFile(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr);

/*****************************************************************************/
// 	Description :  Write Download Manager
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_WriteOtaCallbackFile(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif