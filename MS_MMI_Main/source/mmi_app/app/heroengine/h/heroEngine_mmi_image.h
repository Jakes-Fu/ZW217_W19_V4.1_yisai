/*****************************************************************************
** File Name:      mrapp_mmi_image.h                                         *
** Author:                                                                   *
** Date:           11/2009                                                   *
** Copyright:      sky-mobi.                                                 *
** Description:    This file is used to describe mrapp image resource        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MRAPP_MMI_IMAGE_H_
#define _MRAPP_MMI_IMAGE_H_

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

typedef enum MRAPP_MMI_IMAGE_ID_E
{
	MRAPP_IMAGE_NULL = (MMI_MODULE_HEROENGINE << 16) | MMI_SYS_RES_FLAG,
#include "heroengine_mdu_def.h"
	MRAPP_IMAGE_MAX_ID
} MRAPP_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //_MRAPP_MMI_IMAGE_H_
