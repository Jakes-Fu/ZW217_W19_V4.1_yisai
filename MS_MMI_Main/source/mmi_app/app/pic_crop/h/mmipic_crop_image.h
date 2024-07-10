/*************************************************************************
 ** File Name:      mmipic_crop_image.h                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about image             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/

#ifndef PIC_CROP_MMI_IMAGE_H_
#define PIC_CROP_MMI_IMAGE_H_

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
#include "pic_crop_mdu_def.h"
#endif

typedef enum PIC_CROP_MMI_IMAGE_ID_E
{
	PIC_CROP_IMAGE_NULL = ( MMI_MODULE_PIC_CROP << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "pic_crop_mdu_def.h"
#endif
    PIC_CROP_IMAGE_MAX_ID
} PIC_CROP_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //PIC_MMI_IMAGE_H_
