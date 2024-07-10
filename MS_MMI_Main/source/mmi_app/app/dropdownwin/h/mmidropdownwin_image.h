/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           07/2011                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe dropdown win                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011        Paul.Huang         Create                                  *
******************************************************************************/


#ifndef DROPDOWNWIN_MMI_IMAGE_H_
#define DROPDOWNWIN_MMI_IMAGE_H_

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
#include "dropdownwin_mdu_def.h"
#endif

typedef enum DROPDOWNWIN_MMI_IMAGE_ID_E
{
	DROPDOWNWIN_IMAGE_NULL = MMI_MODULE_DROPDOWNWIN << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "dropdownwin_mdu_def.h"
#endif
	DROPDOWNWIN_IMAGE_MAX_ID
} DROPDOWNWIN_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //DROPDOWNWIN_MMI_IMAGE_H_

