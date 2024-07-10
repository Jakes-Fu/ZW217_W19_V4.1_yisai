/*****************************************************************************
** File Name:      guiscrollkey.c                                                   *
** Author:                                                                   *
** Date:           05/08/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/08/2004       jibin       Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef GUIF_SCROLLKEY

#include "guiscrollkey.h"
//#include "guilcd.h"
//#include "mmi_theme.h"
//#include "guifont.h"
//#include "mmi_textfun.h"
//#include "mmitheme_pubwin.h"
//#include "mmk_app.h"
//#include "mmk_tp.h"
//#include "guires.h"
//#include "guistring.h"
//#include "mmk_window_internal.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/************************************************************************/
/* functions  table                                                     */
/************************************************************************/

/*****************************************************************************/
//  Description : 动态创建状态栏控件
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUISCROLLKEY_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id, 
                                        MMI_TEXT_ID_T left_text_id,
                                        MMI_TEXT_ID_T right_text_id
                                        )
{
    BOOLEAN result = FALSE;
    MMI_CONTROL_CREATE_T   scrollkey_ctrl = {0};
    GUISCROLLKEY_INIT_DATA_T init_data = {0};
    
    //init scrollkey
    init_data.left_scroll_key_id  = left_text_id;
    init_data.right_scroll_key_id = right_text_id;
    
    //create scrollkey control
    scrollkey_ctrl.ctrl_id           = ctrl_id;// MMITHEME_GetScrollkeyCtrlId();
    scrollkey_ctrl.guid              = SPRD_GUI_SCROLLKEY_ID;
    scrollkey_ctrl.init_data_ptr     = &init_data;
    scrollkey_ctrl.parent_win_handle = win_handle;
    if (PNULL != MMK_CreateControl(&scrollkey_ctrl))
    {
        result = TRUE;
        MMK_SetWinScrollkeyCtrlId(win_handle, ctrl_id);
    }
    
    return result;
}


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#endif // GUIF_SCROLLKEY
// end file


/*Edit by script, ignore 7 case. Thu Apr 26 19:01:15 2012*/ //IGNORE9527
