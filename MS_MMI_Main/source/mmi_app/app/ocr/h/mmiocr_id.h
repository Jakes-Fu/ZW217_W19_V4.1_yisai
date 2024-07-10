/*****************************************************************************
** File Name:      ocr_id.h                                                   *
** Author:                                                                   *
** Date:           11/17/2005                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       Annie.an        Create
******************************************************************************/
#ifdef MMI_OCR_ENABLE
#ifndef _MMIOCR_ID_H_
#define _MMIOCR_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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

#define WIN_ID_DEF(win_id)          win_id
// window ID
// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
   MMIOCR_WIN_ID_START = (MMI_MODULE_OCR<< 16),

#include "mmiocr_id.def"

   MMIOCR_MAX_WIN_ID

   
}OCR_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
// 命名规则: 控件所属的窗口名 + 控件名 + CTRL_ID, eg: IDLEWIN_TEXTBOX_CTRL_ID (IDLE窗口下text控件ID)
typedef enum
{
    MMIOCR_CTRL_ID_ID_START = MMIOCR_MAX_WIN_ID, //(MMI_MODULE_OCR << 16),
    MMIOCR_SHOOTING_LABEL_CTRL_ID,
	MMIOCR_RECOG_LABEL_CTRL_ID

}OCR_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

#endif
