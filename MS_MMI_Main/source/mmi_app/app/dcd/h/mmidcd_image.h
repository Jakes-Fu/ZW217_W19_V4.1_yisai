/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           03/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef DCD_MMI_IMAGE_H_
#define DCD_MMI_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "dcd_mdu_def.h"
#endif

typedef enum DCD_MMI_IMAGE_ID_E
{
	DCD_IMAGE_NULL = MMI_MODULE_DCD << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "dcd_mdu_def.h"
#endif
	DCD_IMAGE_MAX_ID
} DCD_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //PB_MMI_IMAGE_H_
