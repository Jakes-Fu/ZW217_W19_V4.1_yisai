/*****************************************************************************
** File Name:      xxx_mmi_image.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool					              *
******************************************************************************/

#ifndef JAVA_MMI_IMAGE_H_
#define JAVA_MMI_IMAGE_H_
#include "std_header.h"
#ifdef JAVA_SUPPORT_SUN
#include "sci_types.h"
#include "mmi_anim.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Í¼Æ¬×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "java_mdu_def.h"
#endif

typedef enum JAVA_MMI_IMAGE_ID_E
{
	JAVA_IMAGE_NULL = ( MMI_MODULE_JAVA << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "java_mdu_def.h"
#endif
	JAVA_IMAGE_MAX_ID
} JAVA_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //JAVA_MMI_IMAGE_H_
