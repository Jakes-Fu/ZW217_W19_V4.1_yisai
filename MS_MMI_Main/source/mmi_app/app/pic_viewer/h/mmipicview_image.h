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
#ifdef PIC_VIEWER_SUPPORT
#ifndef PIC_VIEWER_MMI_IMAGE_H_
#define PIC_VIEWER_MMI_IMAGE_H_

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
#include "pic_viewer_mdu_def.h"
#endif

typedef enum PIC_VIEWER_MMI_IMAGE_ID_E
{
	PIC_VIEWER_IMAGE_NULL = (MMI_MODULE_PIC_VIEWER << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "pic_viewer_mdu_def.h"
#endif
	PIC_VIEWER_IMAGE_MAX_ID
} PIC_VIEWER_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifndef PIC_PREVIEW_U200_SUPPORT
#define PICVIEW_DEFAULT_BG_IMG IMAGE_THEME_BG
#else
#define PICVIEW_DEFAULT_BG_IMG IMAGE_PICVIEW_FOLDER_LIST_BG
#endif

#define PICVIEW_DEFAULT_ICON_IMG IMAGE_COMMON_PIC_ICON
#ifdef __cplusplus
}
#endif

#endif //PIC_VIEWER_MMI_IMAGE_H_
#endif //#ifdef PIC_VIEWER_SUPPORT