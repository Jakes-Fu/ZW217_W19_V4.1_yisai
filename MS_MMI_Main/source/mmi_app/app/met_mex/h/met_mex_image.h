/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef __MET_MEX_IMAGE_H__
#define __MET_MEX_IMAGE_H__

#include "sci_types.h"
#ifdef MET_MEX_SUPPORT
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif // __cplusplus

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum MET_MEX_MMI_IMAGE_ID_E
{
	MET_MEX_IMAGE_NULL = ( MMI_MODULE_MET_MEX << 16 ) | MMI_SYS_RES_FLAG,
#include "met_mex_mdu_def.h"
	MET_MEX_IMAGE_MAX_ID
} MET_MEX_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MET_MEX_SUPPORT
#endif // __MET_MEX_IMAGE_H__