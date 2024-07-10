/*****************************************************************************
** File Name:      mmivt_image.h                                           *
** Author:                                                                   *
** Date:           02/2010                                                   *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef VT_MMI_IMAGE_H_
#define VT_MMI_IMAGE_H_

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
#include "vt_mdu_def.h"
#endif

typedef enum VT_MMI_IMAGE_ID_E
{
	VT_IMAGE_NULL = (MMI_MODULE_VT << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "vt_mdu_def.h"
#endif
	VT_IMAGE_MAX_ID
} VT_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //VT_MMI_IMAGE_H_
