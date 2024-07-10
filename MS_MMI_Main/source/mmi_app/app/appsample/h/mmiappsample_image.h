/*****************************************************************************
 ** File Name:   mmiappsample_image.h
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe image resource 
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/


#ifndef APPSAMPLE_MMI_IMAGE_H_
#define APPSAMPLE_MMI_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"
#include "mmiappsample_define.h"
#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "APPSAMPLE_mdu_def.h"
#endif

typedef enum APPSAMPLE_MMI_IMAGE_ID_E
{
	APPSAMPLEIMAGE_NULL = ( MMI_MODULE_APPSAMPLE << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "APPSAMPLE_mdu_def.h"
#endif
	APPSAMPLE_IMAGE_MAX_ID
} APPSAMPLE_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //APPSAMPLE_MMI_IMAGE_H_