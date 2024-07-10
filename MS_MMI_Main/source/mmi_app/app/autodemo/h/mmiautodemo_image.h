/*****************************************************************************
** File Name:      mmiautodemo_image.h                                           *
** Author:         chunjie.liu                                                      *
** Date:           17/03/2012                                                   *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe AUTO DEMO                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIAUTODEMO_IMAGE_H_
#define MMIAUTODEMO_IMAGE_H_

#include "sci_types.h"
#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"


typedef enum AD_MMI_IMAGE_ID_E
{
	AD_IMAGE_NULL = ( MMI_MODULE_AD << 16 ) | MMI_SYS_RES_FLAG,

    #include "autodemo_mdu_def.h"

	AD_IMAGE_MAX_ID
} AD_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //MMIAUTODEMO_IMAGE_H_
