/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef STREAMING_MMI_IMAGE_H_
#define STREAMING_MMI_IMAGE_H_

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
#include "videoplayer_mdu_def.h"
#endif

typedef enum VP_MMI_IMAGE_ID_E
{
    VP_IMAGE_NULL = (MMI_MODULE_VP << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "videoplayer_mdu_def.h"
#endif
    VP_IMAGE_MAX_ID
}VP_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //STREAMING_MMI_IMAGE_H_
