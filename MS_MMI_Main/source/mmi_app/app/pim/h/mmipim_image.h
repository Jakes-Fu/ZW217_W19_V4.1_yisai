/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : mmipim_image.h
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-2 ,wangxiaolin,create originally.
*
*******************************************************************************/

#ifndef PIM_MMI_IMAGE_H_
#define PIM_MMI_IMAGE_H_

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
#include "pim_mdu_def.h"
#endif

typedef enum PIM_MMI_IMAGE_ID_E
{
	PIM_IMAGE_NULL = MMI_MODULE_PIM << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "pim_mdu_def.h"
#endif
	PIM_IMAGE_MAX_ID
} PIM_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //PIM_MMI_IMAGE_H_
