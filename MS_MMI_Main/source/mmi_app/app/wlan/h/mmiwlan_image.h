/*****************************************************************************
** File Name:      mmiwlan_image.h                                           *
** Author:                                                                   *
** Date:           2009/11/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe wifi                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIWLAN_IMAGE_H_
#define MMIWLAN_IMAGE_H_

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
#include "wifi_mdu_def.h"
#endif

typedef enum MMIWIFI_IMAGE_ID_E
{
	MMIWIFI_IMAGE_NULL = ( MMI_MODULE_WIFI << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "wifi_mdu_def.h"
#endif
	MMIWIFI_IMAGE_MAX_ID
} MMIWIFI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //MMIWLAN_IMAGE_H_
