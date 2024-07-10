/*****************************************************************************
** File Name:      browser_manager_image.h                                           *
** Author:         fen.xie                                                    *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser image resource  *
*****************************************************************************/
#ifndef BROWSER_MANAGER_IMAGE_H_
#define BROWSER_MANAGER_IMAGE_H_

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
extern   "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "browser_manager_mdu_def.h"
#endif

typedef enum BM_MMI_IMAGE_ID_E
{
	BM_IMAGE_NULL = ( MMI_MODULE_MMIBROWSER_MANAGER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "browser_manager_mdu_def.h"
#endif
	BM_IMAGE_MAX_ID
} BM_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //HELLO_MMI_IMAGE_H_


