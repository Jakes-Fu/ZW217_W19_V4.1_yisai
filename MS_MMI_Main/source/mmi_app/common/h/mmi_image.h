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

#ifndef COMMON_MMI_IMAGE_H_
#define COMMON_MMI_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

//image_id
//#define RES_ADD_IMG(IMAGE_ID, param1, param2, param3)   IMAGE_ID,
//#define RES_ADD_ANIM(ANIM_ID, param1, param2, param3, param4)   //none
//#define RES_ADD_STRING(TXT_ID, default_string)  //none

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif

typedef enum COMMON_MMI_IMAGE_ID_E
{
	COMMON_IMAGE_NULL = (MMI_MODULE_COMMON << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
    #include "common_mdu_def.h"	
#endif
	COMMON_IMAGE_MAX_ID
} COMMON_MMI_IMAGE_ID_E;

//#undef RES_ADD_IMG
//#undef RES_ADD_ANIM
//#undef RES_ADD_STRING
#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //COMMON_MMI_IMAGE_H_
