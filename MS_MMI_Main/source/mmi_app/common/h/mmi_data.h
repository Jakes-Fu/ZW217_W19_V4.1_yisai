/*****************************************************************************
** File Name:      mmi_data.h                                                *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef COMMON_MMI_DATA_H_
#define COMMON_MMI_DATA_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// DATA Resource ID
///////////////////////////////////////////////////////////////////////////////


#define MACRO_MDU_TYPE  MACRO_MDU_DATA
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif

typedef enum COMMON_MMI_DATA_ID_E
{
	COMMON_DATA_NULL = (MMI_MODULE_COMMON << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif
	COMMON_DATA_MAX_ID
} COMMON_MMI_DATA_ID_E;


#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //COMMON_MMI_DATA_H_
