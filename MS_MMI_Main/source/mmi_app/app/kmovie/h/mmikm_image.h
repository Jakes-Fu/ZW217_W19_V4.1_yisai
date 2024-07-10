/*****************************************************************************
** File Name:      mmikm_image.h                                             *
** Author:                                                                   *
** Date:           14/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** ------------------------------------------------------------------------  *
** DATE             NAME              DESCRIPTION                            *
** 07/2011         tonny.chen          Create                                *
**																		     *
*****************************************************************************/

#ifndef _MMIKM_IMAGE_H_
#define _MMIKM_IMAGE_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif
///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"
/*
typedef enum MMI_TEST_IMAGE_ID_E
{
	TEST_IMAGE_NULL = MMI_MODULE_MMITEST << 16 | MMI_SYS_RES_FLAG,
#include "mmitest_mdu_def.h"
	TEST_IMAGE_MAX
} MMI_TEST_IMAGE_ID_E;
*/

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMIKM_IMAGE_H_