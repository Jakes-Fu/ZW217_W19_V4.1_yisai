/*****************************************************************************
** File Name:      xxx_mmi_ring.h                                            *
** Author:                                                                   *
** Date:           03/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_RING_H__
#define _MMI_RING_H__

#include "sci_types.h"
#include "mmisrvaud_api.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

typedef struct _AUD_RING_DATA_INFO_T
{
	MMISRVAUD_RING_FMT_E    type;
	uint32            data_len;
	uint8            *data_ptr;
} MMIAUD_RING_DATA_INFO_T, * PMMIAUD_RING_DATA_INFO_T;

typedef MMIAUD_RING_DATA_INFO_T MMIRES_DATA_INFO_T;
//ring_id
/*
#define RES_ADD_LANGUAGE(language_string)       //none
#define RES_SET_VERSION(version_string)     //none
#define RES_SET_MAX_MODULE_NUM(num)         //none
#define RES_SET_IMG_DEFAULT_COMPRESS_TYPE(type, percent)    //none
#define RES_SET_ANIM_DEFAULT_COMPRESS_TYPE(type, percent)   //none
#define RES_SET_IMG_GLOBAL_COMPRESS_TYPE(type, percent)     //none
#define RES_SET_ANIM_GLOBAL_COMPRESS_TYPE(type, percent)    //none
#define RES_SET_ANIM_GLOBAL_KEY_COLOR(color)        //none
#define RES_SET_GLOBAL_JPEG_COMPRESS_RATE(percent)  //none
#define RES_ADD_SKIN(skin_name, version_string, index)  //none
#define RES_ADD_MODULE(module_id, module_path)          //none
//#define RES_ADD_RING(RING_ID, param1,ring_path)         RING_ID,
#define RES_ADD_RING(RING_ID,ring_path)         RING_ID,
#define RES_ADD_FONT(FONT_ID, font_path, describe_string)   //none
*/
#define MACRO_RES_PRJ_TYPE      MACRO_RES_PRJ_RING

#include "macro_res_prj_def.h"

typedef enum _MMI_RING_ID_E
{
	COMMON_RING_NULL = (MMI_MODULE_COMMON << 16) | MMI_SYS_RES_FLAG,

//#include "mmi_240x320_res_mmi_res_prj_def.c"
    #include "mmi_res_prj_def.h"
	RING_MAX_ID
} MMI_RING_ID_E;

#undef MACRO_RES_PRJ_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMI_RING_H__
