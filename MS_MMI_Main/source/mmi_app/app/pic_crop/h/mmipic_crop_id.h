/*************************************************************************
 ** File Name:      mmipic_crop_id.h                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about id             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef MMI_PIC_CROP_SUPPORT
#ifndef _MMIPIC_CROP_ID_H_
#define _MMIPIC_CROP_ID_H_ 

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
    PIC_CROP_WIN_ID_START = (MMI_MODULE_PIC_CROP << 16),

#include "mmipic_crop_id.def"

    PIC_CROP_MAX_WIN_ID
}PIC_CROP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    PIC_CROP_CTRL_ID_ID_START = PIC_CROP_MAX_WIN_ID,
    PIC_CROP_OK_BUTTON_CTRL_ID,
    PIC_CROP_BACK_BUTTON_CTRL_ID,
    //PIC_CROP_TITLE_LABEL_CTRL_ID,
    
    
    PIC_CROP_MAX_CTRL_ID
}PIC_CROP_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
