/*****************************************************************************
** File Name:      mmidrm_image.h                                            *
** Author:                                                                   *
** Date:           04/23/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define the image id of drm file      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/03/2011     haiwu.chen       Create                                    *
******************************************************************************/

#ifndef DRM_MMI_IMAGE_H_
#define DRM_MMI_IMAGE_H_

#if defined(DRM_SUPPORT)

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
#include "drm_mdu_def.h"
#endif

typedef enum DRM_MMI_IMAGE_ID_E
{
    DRM_IMAGE_NULL = ( MMI_MODULE_DRM << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "drm_mdu_def.h"
#endif
    DRM_IMAGE_MAX_ID
} DRM_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //#if defined(DRM_SUPPORT)

#endif //DRM_MMI_IMAGE_H_


