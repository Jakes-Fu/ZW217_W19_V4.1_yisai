/****************************************************************************
** File Name:      mmigame_quintet_id.h                                               *
** Author:                                                                 *
** Date:           03/29/2006                                             *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006        Annie.an        Create
** 
****************************************************************************/
#ifndef  _MMIGAME_QUINTET_ID_H_    
#define  _MMIGAME_QUINTET_ID_H_  

#define __UM_DUAL_SIM__
//#define APPLE_ROTATE_DYNAMIC 
//#define APPLE_ROTATE_STATIC
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

#include "mmiapple.h"
//#include "apple_menutable.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

// #define WIN_ID_DEF(win_id)          win_id

typedef enum
{
	APPLE_WIN_ID_START = (MMI_MODULE_SXH_APPLE << 16),    //模块区分
	APPLE_MAIN_WIN_ID ,
	MMIAPPLE_EDITBOX_WIN_ID,
#ifdef __UM_DUAL_SIM__	
	APPLE_SELECT_SIM_WIN_ID,
#endif
	APPLETV_WIN_ID,
	APPLE_MSGPOP_WIN_ID,
	APPLE_WAIT_WIN_ID,
	APPLE_SMD_WIN_ID,
#ifdef SXMOBI_UMMUSIC
    APPLE_UMMUSIC_WIN_ID,
#endif
	APPLE_MAX_WIN_ID
}SXH_APPLE_WINDOW_ID_E;


// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
       APPLE_CTRL_ID_ID_START = APPLE_MAX_WIN_ID, 
       MMIAPPLE_EDITBOX_CTRL_ID,
       MMIAPPLE_SOFTKEY_CTRL_ID,
       MMIAPPLE_TITLE_CTRL_ID,
 #ifdef __UM_DUAL_SIM__      
	MMIAPPLE_SELECT_SIM_CTRL_ID,
#endif	
	APPLE_MAX_CTRL_ID
}APPLE_CTRL_ID_T;

//	文字ID
#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum APPLE_MMI_TEXT_ID_E
{
	APPLE_TXT_NULL = ( MMI_MODULE_SXH_APPLE << 16 ) | MMI_SYS_RES_FLAG,
#include "apple_mdu_def.h"
		APPLE_TXT_MAX
} APPLE_MMI_TEXT_ID_E;
#undef MACRO_MDU_TYPE

//	图片ID
#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"
typedef enum APPLE_MMI_IMAGE_ID_E
{
	APPLE_IMAGE_NULL = ( MMI_MODULE_SXH_APPLE << 16 ) | MMI_SYS_RES_FLAG,
#include "apple_mdu_def.h"
		APPLE_IMAGE_MAX_ID
} APPLE_MMI_IMAGE_ID_E;
#undef MACRO_MDU_TYPE

#define APPLE_BASE_MSG_ID (MSG_USER_START+1)
#define SMD_BASE_MSG_ID (MSG_USER_START+3)
#define APPLE_BASE2_MSG_ID (MSG_USER_START+2)
#define APPLE_VIDEOPLAYER  ("APPLE VIDEOPLAYER")



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
