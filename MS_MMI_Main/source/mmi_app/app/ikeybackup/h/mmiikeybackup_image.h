/****************************************************************************
** File Name:      mmiikeybackup_image.h                                    *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/

#ifdef IKEYBACKUP_SUPPORT

#ifndef _MMIIKEYBACKUP_IMAGE_H_
#define _MMIIKEYBACKUP_IMAGE_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

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

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "ikeybackup_mdu_def.h"
#endif

typedef enum IKEYBACKUP_MMI_IMAGE_ID_E
{
    IKEYBACKUP_IMAGE_NULL = (MMI_MODULE_IKEYBACKUP << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "ikeybackup_mdu_def.h"
#endif
    IKEYBACKUP_IMAGE_MAX_ID
} IKEYBACKUP_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //IKEYBACKUP_SUPPORT

#endif //_MMIIKEYBACKUP_IMAGE_H_

