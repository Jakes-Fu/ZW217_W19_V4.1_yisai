/*****************************************************************************
** File Name:      mmibaidu_id.h                                                *
** Author:                                                                   *
** Date:           2012/03/17                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe ad win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/03/17     chunjie.liu        Create                                    *
******************************************************************************/

#ifndef _MMIBAIDU_DRV_ID_H_
#define _MMIBAIDU_DRV_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 #define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "baidu_drive_mdu_def.h"
#endif
 
// window ID
typedef enum
{
    MMIBAIDU_DRV_WIN_ID_START = (MMI_MODULE_BAIDU_DRV << 16),
    MMIBAIDU_DRV_WIN_ID,
    MMIBAIDU_DRV_USER_INFO_WIN_ID,
    MMIBAIDU_DRV_LAYER_WIN_ID,
    MMIBAIDU_DRV_GET_THUMBNAIL_WIN_ID,
    MMIBAIDU_DRV_WAITING_WIN_ID,
    MMIBAIDU_DRV_ALERT_WIN_ID,    
    MMIBAIDU_DRV_VIP_WIN_ID,    
    MMIBAIDU_DRV_MAX_WIN_ID
}MMIBAIDU_DRV_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIBAIDU_DRV_CTRL_ID_START = MMIBAIDU_DRV_MAX_WIN_ID,//(MMI_MODULE_BAIDU_DRV << 16),
//ad
    MMIBAIDU_DRV_LISTBOX_CTRL_ID,	// the listbox control of Autodemo Window
    MMIBAIDU_DRV_OPT_LISTBOX_CTRL_ID,
    MMIBAIDU_LOGIN_ANIM_ID,
    MMIBAIDU_LOGIN_LABEL1_CTRL_ID,
    MMIBAIDU_LOGIN_LOGOUT_BTN_CTRL_ID,    
    MMIBAIDU_VIP_ANIM_ID,
    MMIBAIDU_VIP_LABEL1_CTRL_ID,    
    MMIBAIDU_USERINFO_AVATAR_CTRL_ID,
    MMIBAIDU_USERINFOLABLE1_CTRL_ID,
    MMIBAIDU_USERINFOLABLE2_CTRL_ID,
    MMIBAIDU_DRV_MAX_CTRL_ID
}BAIDU_DRV_CONTROL_ID_E;

typedef enum _BAIDU_DRV_MMI_TEXT_ID_E
{
	BAIDU_DRV_TXT_NULL = (MMI_MODULE_BAIDU_DRV << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "baidu_drive_mdu_def.h"
#endif
	BAIDU_DRV_TXT_MAX
} BAIDU_DRV_MMI_TEXT_ID_E; 

#undef MACRO_MDU_TYPE

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"
typedef enum BAIDU_DRV_IMAGE_ID_E
{
	BAIDU_DRV_IMAGE_NULL = ( MMI_MODULE_BAIDU_DRV << 16 ) | MMI_SYS_RES_FLAG,
#include "baidu_drive_mdu_def.h"
		BAIDU_DRV_IMAGE_MAX_ID
} BAIDU_DRV_MMI_IMAGE_ID_E;
#undef MACRO_MDU_TYPE


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIBAIDU_DRV_ID_H_
