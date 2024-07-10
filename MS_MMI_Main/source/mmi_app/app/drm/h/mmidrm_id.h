/*****************************************************************************
** File Name:      mmidrm_id.h                                               *
** Author:                                                                   *
** Date:           02/23/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define the win-control id of drm     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/03/2011     haiwu.chen       Create                                    *
******************************************************************************/

#ifndef _MMIDRM_ID_H_
#define _MMIDRM_ID_H_ 

#if defined(DRM_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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
 
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MMI_DRM_WIN_ID_START = (MMI_MODULE_DRM << 16),

#include "mmidrm_id.def"

    MMIDRM_MAX_WIN_ID
}MMIDRM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIDRM_CTRL_ID_START = MMIDRM_MAX_WIN_ID,//(MMI_MODULE_DRM << 16),

    MMIDRM_RENAME_EDITBOX_CTRL_ID,
#ifdef MMI_PDA_SUPPORT
    MMIDRM_RENAME_FORM_CTRL_ID,
    MMIDRM_RENAME_SOFTKEY_CTRL_ID,
#endif    
    MMIDRM_MAX_CTRL_ID
}DRM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //DRM_SUPPORT

#endif //_MMIDRM_ID_H_


