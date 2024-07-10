/****************************************************************************
** File Name:      mmisearch_id.h                                            *
** Author:         plum.peng                                                   *
** Date:           2010-03                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe window and control id.     *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 03/2010        plum.peng           Create
****************************************************************************/
#ifdef SEARCH_SUPPORT
#ifndef _MMISEARCH_ID_H_
#define _MMISEARCH_ID_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmisearch_text.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    SEARCH_WIN_ID_START = (MMI_MODULE_SEARCH << 16),
#include "mmisearch_id.def"

    SEARCH_MAX_WIN_ID
}SEARCH_WINDOW_ID_E;

#undef WIN_ID_DEF

// control id
typedef enum
{
    SEARCH_CTRL_ID_START = SEARCH_MAX_WIN_ID,

#if 1 // hermann
    MMISEARCH_TYPE_OPT_FORM_CTRL_ID,
    MMISEARCH_KEYWORD_EDITBOX_FORM_CTRL_ID,
    MMISEARCH_KEYWORD_EDITBOX_CTRL_ID,
    MMISEARCH_WAITING_ANIM_CTRL_ID,
    MMISEARCH_RESULT_LISTBOX_CTRL_ID,
    MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID,
    MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID,
    MMISEARCH_KEYWORD_FORM_SEARCH_BUTTON_CTRL_ID,   
    MMISEARCH_TYPE_BUTTON_ALL_CTRL_ID,    
    MMISEARCH_TYPE_BUTTON_FILE_CTRL_ID,
    MMISEARCH_TYPE_BUTTON_PB_CTRL_ID,
    MMISEARCH_TYPE_BUTTON_SMS_CTRL_ID,
    MMISEARCH_TYPE_BUTTON_APP_CTRL_ID,
    MMISEARCH_TYPE_BUTTON_CL_CTRL_ID,
#if  !defined MMI_PDA_SUPPORT // hermann liu
    MMISEARCH_TYPE_OPT_MENU_CTRL_ID,
#endif
#endif
    SEARCH_MAX_CTRL_ID
} SEARCH_CONTROL_ID_E;
        


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMISEARCH_RegWinIdNameArr(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMTV_ID_H_ */
#endif

