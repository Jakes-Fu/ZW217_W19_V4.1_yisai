/*****************************************************************************
** File Name:      ctrlform_parse.c                                          *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "ctrlform_export.h"
#include "ctrlform.h"
#include "guitext.h"
#include "guiedit.h"
#include "guianim.h"
#include "guilabel.h"
#include "guibutton.h"
#include "guidropdownlist.h"
#include "guisetlist.h"
#include "guiownerdraw.h"
#include "mmi_theme.h"
#include "mmitheme_dropdownlist.h"
#include "guisoftkey.h"
#include "mmk_handle_internal.h"
#include "ctrlslider_export.h"
#include "ctrlprogress_export.h"
#include "ctrlsoftkey.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create the text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildTextCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );

/*****************************************************************************/
//  Description : create child edit box text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTextCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      );

/*****************************************************************************/
//  Description : create the edit box phone number control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPhonenumCtrl(
                                          MMI_HANDLE_T  win_handle,
                                          uint32        **value_pptr
                                          );

/*****************************************************************************/
//  Description : create the edit box digital control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditDigitalCtrl(
                                         MMI_HANDLE_T   win_handle,
                                         uint32         **value_pptr
                                         );

/*****************************************************************************/
//  Description : create the edit box password control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPasswordCtrl(
                                          MMI_HANDLE_T  win_handle,
                                          uint32        **value_pptr
                                          );

/*****************************************************************************/
//  Description : create the edit box list control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditListCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      );

/*****************************************************************************/
//  Description : create the edit box date control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditDateCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      );

/*****************************************************************************/
//  Description : create the edit box time control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTimeCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      );

/*****************************************************************************/
//  Description : create the edit box IP control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditIPCtrl(
                                    MMI_HANDLE_T    win_handle,
                                    uint32          **value_pptr
                                    );

/*****************************************************************************/
//  Description : create the animation control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildAnimCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );

/*****************************************************************************/
//  Description : create the label control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildLabelCtrl(
                                   MMI_HANDLE_T win_handle,
                                   uint32       **value_pptr
                                   );

/*****************************************************************************/
//  Description : create the button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildButtonCtrl(
                                    MMI_HANDLE_T    win_handle,
                                    uint32          **value_pptr
                                    );

/*****************************************************************************/
//  Description : create the button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildCheckboxCtrl(
                                    MMI_HANDLE_T    win_handle,
                                    uint32          **value_pptr
                                    );

/*****************************************************************************/
//  Description : create the drop-down list control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildDropdownlistCtrl(
                                          MMI_HANDLE_T   win_handle,
                                          uint32         **value_pptr
                                          );

/*****************************************************************************/
//  Description : create the set list control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSetListCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     );

/*****************************************************************************/
//  Description : create the owner draw control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildOwnDrawCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     );

/*****************************************************************************/
//  Description : create the form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildFormCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );

/*****************************************************************************/
//  Description : create the form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildListCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );

/*****************************************************************************/
//  Description : create the softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSoftkeyCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     );

/*****************************************************************************/
//  Description : create the edit touch date control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTouchDate(
                                       MMI_HANDLE_T  win_handle,
                                       uint32        **value_pptr
                                       );

/*****************************************************************************/
//  Description : create the edit touch time control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTouchTime(
                                       MMI_HANDLE_T  win_handle,
                                       uint32        **value_pptr
                                       );

/*****************************************************************************/
//  Description : create the edit picker control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPicker(
                                    MMI_HANDLE_T  win_handle,
                                    uint32        **value_pptr
                                    );

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSlider(
                                MMI_HANDLE_T  win_handle,
                                uint32        **value_pptr
                                );

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildProgress(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );
                                  
/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildCombox(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildPrgbox(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  );     
                                                               
/*****************************************************************************/
//  Description : form control add child control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FormAddChildCtrl(
                            CTRLFORM_CHILD_INIT_T    *init_ptr   //in:
                            );

/*****************************************************************************/
//  Description : add child to list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddChildToList(
                          CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                          CTRLFORM_CHILD_T  *child_ptr      //in:
                          );

/*****************************************************************************/
//  Description : set anim bg child control init rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAnimBgRect(
                         CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                         CTRLFORM_CHILD_T  *child_ptr      //in:
                         );

/*****************************************************************************/
//  Description : set softkey param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetSoftkeyParam(
                           CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                           CTRLFORM_CHILD_T  *child_ptr      //in:
                           );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
//parse child control
const MMK_KEYWORD_INFO_T FormChildCtrlParseInfo[] =
{
    { WIN_KEYWORD(CREATE_CHILD_TEXT, CreateChildTextCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_TEXT, CreateChildEditTextCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_PHONENUM, CreateChildEditPhonenumCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_DIGITAL, CreateChildEditDigitalCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_PASSWORD, CreateChildEditPasswordCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_LIST, CreateChildEditListCtrl, 5) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_DATE, CreateChildEditDateCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_TIME, CreateChildEditTimeCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_IP, CreateChildEditIPCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_ANIM, CreateChildAnimCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_LABEL, CreateChildLabelCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_BUTTON, CreateChildButtonCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_CHECKBOX, CreateChildCheckboxCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_DROPDOWNLIST, CreateChildDropdownlistCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_SETLIST, CreateChildSetListCtrl, 3) },
    { WIN_KEYWORD(CREATE_CHILD_OWNDRAW, CreateChildOwnDrawCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_FORM, CreateChildFormCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_LIST, CreateChildListCtrl, 4) },
    { WIN_KEYWORD(CREATE_CHILD_SOFTKEY, CreateChildSoftkeyCtrl, 5) },
    { WIN_KEYWORD(CREATE_CHILD_TAB, PNULL, 0) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_TOUCH_DATE, CreateChildEditTouchDate, 3) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_TOUCH_TIME, CreateChildEditTouchTime, 3) },
    { WIN_KEYWORD(CREATE_CHILD_EDIT_PICKER, CreateChildEditPicker, 3) },
    { WIN_KEYWORD(CREATE_CHILD_SLIDER, CreateChildSlider, 3) },
    { WIN_KEYWORD(CREATE_CHILD_PROGRESS, CreateChildProgress, 3) },
    { WIN_KEYWORD(CREATE_CHILD_COMBOX, CreateChildCombox, 3) },
    { WIN_KEYWORD(CREATE_CHILD_PRGBOX, CreateChildPrgbox, 4) },
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create the text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ParseChild(
                                   uint32        index,
                                   MMI_HANDLE_T  win_handle,
                                   uint32        **value_pptr
                                   )
{
    BOOLEAN     result = FALSE;
    
    result = FormChildCtrlParseInfo[index].parse_func(win_handle,value_pptr);
    
    return (result);
}

/*****************************************************************************/
//  Description : create the text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildTextCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUITEXT_INIT_DATA_T     init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //creat
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_TEXTBOX_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create child edit box text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTextCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      )
{
    uint16                  str_max_len = 0;
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    str_max_len              = (uint16)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_TEXT;
    init_data.str_max_len      = str_max_len;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box phone number control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPhonenumCtrl(
                                          MMI_HANDLE_T  win_handle,
                                          uint32        **value_pptr
                                          )
{
    uint16                  str_max_len = 0;
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    str_max_len              = (uint16)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_PHONENUM;
    init_data.str_max_len      = str_max_len;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box digital control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditDigitalCtrl(
                                         MMI_HANDLE_T   win_handle,
                                         uint32         **value_pptr
                                         )
{
    uint16                  str_max_len = 0;
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    str_max_len              = (uint16)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_DIGITAL;
    init_data.str_max_len      = str_max_len;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box password control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPasswordCtrl(
                                          MMI_HANDLE_T  win_handle,
                                          uint32        **value_pptr
                                          )
{
    uint16                  str_max_len = 0;
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    str_max_len              = (uint16)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_PASSWORD;
    init_data.str_max_len      = str_max_len;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box list control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditListCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      )
{
    uint16                  item_num = 0;
    uint16                  item_max_len = 0;
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    item_num                 = (uint16)(*value_ptr++);
    item_max_len             = (uint16)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type                       = GUIEDIT_TYPE_LIST;
    init_data.add_data.list.item_num     = item_num;
    init_data.add_data.list.item_max_len = item_max_len;
    init_data.is_rect_adaptive           = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box date control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditDateCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_DATE;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box time control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTimeCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                      )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_TIME;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit box IP control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditIPCtrl(
                                    MMI_HANDLE_T    win_handle,
                                    uint32          **value_pptr
                                    )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_IP;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the animation control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildAnimCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIANIM_INIT_DATA_T     init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_bg         = (BOOLEAN)(*value_ptr++);
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //init data
    init_data.is_form_bg     = child_init.is_bg;
    
    //creat
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_ANIM_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    IGUICTRL_SetState((IGUICTRL_T *)child_init.ctrl_ptr,GUICTRL_STATE_DISABLE_TP,FALSE);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the label control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildLabelCtrl(
                                   MMI_HANDLE_T win_handle,
                                   uint32       **value_pptr
                                   )
{
    uint32                  *value_ptr = PNULL;
    GUILABEL_ALIGN_E        align = GUILABEL_ALIGN_LEFT;
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T    init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    align                    = (GUILABEL_ALIGN_E)(*value_ptr++);
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.align = align;
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_LABEL_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildButtonCtrl(
                                    MMI_HANDLE_T    win_handle,
                                    uint32          **value_pptr
                                    )
{
    uint32                  *value_ptr = PNULL;
    MMI_IMAGE_ID_T          bg_img_id = 0;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    bg_img_id                = (MMI_IMAGE_ID_T)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.bg.bg_type = GUI_BG_IMG;
    init_data.bg.img_id  = bg_img_id;
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_BUTTON_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildCheckboxCtrl(
                                      MMI_HANDLE_T  win_handle,
                                      uint32        **value_pptr
                                    )
{
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUICHECKBOX_INIT_DATA_T init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_CHECKBOX_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the Dropdownlist control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildDropdownlistCtrl(
                                          MMI_HANDLE_T   win_handle,
                                          uint32         **value_pptr
                                          )
{
    uint32                      *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T        create = {0};
    GUIDROPDOWNLIST_INIT_DATA_T init_data = {0};
    CTRLFORM_CHILD_INIT_T       child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.page_item_num = 0;
    init_data.show_type     = GUIDNLIST_SHOW_AUTOSET;
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_DROPDOWNLIST_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the set list control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSetListCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     )
{
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUISETLIST_INIT_DATA_T  init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_SETLIST_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the owner draw control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildOwnDrawCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     )
{
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIOWNDRAW_INIT_DATA_T  init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.OwnDrawFunc = (GUIOWNDRAW_CALL_BACK_FUNC)(*value_ptr++);
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_OWNDRAW_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildFormCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    GUIFORM_LAYOUT_E        layout_type = GUIFORM_LAYOUT_ORDER;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    layout_type              = (GUIFORM_LAYOUT_E)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.layout_type = layout_type;
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_FORM_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the list control
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildListCtrl(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    GUILIST_TYPE_E          list_type = GUILIST_TYPE_NONE;
    MMI_CONTROL_CREATE_T    create = {0};
    GUILIST_INIT_DATA_T     init_data = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    list_type                = (GUILIST_TYPE_E)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type            = list_type;
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_LIST_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = &init_data;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSoftkeyCtrl(
                                     MMI_HANDLE_T   win_handle,
                                     uint32         **value_pptr
                                     )
{
    BOOLEAN                 is_visible = TRUE;
    uint32                  *value_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    GUISOFTKEY_INIT_DATA_T  init_data = {0};
    
    value_ptr = *value_pptr;
    
    //set init data
    init_data.leftsoft_id  = (MMI_TEXT_ID_T)(*value_ptr++);
    init_data.midsoft_id   = (MMI_TEXT_ID_T)(*value_ptr++);
    init_data.rightsoft_id = (MMI_TEXT_ID_T)(*value_ptr++);
    
#ifdef MMI_PDA_SUPPORT
    if ((MMITHEME_IsIstyle()) && 
        (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_TITLE)) && 
        (!MMK_GetWinDisplayStyleState(win_handle, WS_DISABLE_SOFTKEY_TO_TITLE)))
    {
        GUISOFTKEY_BUTTONSTYLE_E softkey_style = GUISOFTKEY_BUTTONSTYLE_NONE;
        
        softkey_style = GUIBUTTONSOFTKEY_GetStyleFromText(init_data.leftsoft_id, init_data.midsoft_id, init_data.rightsoft_id);
        
        switch (softkey_style)
        {
        case GUISOFTKEY_BUTTONSTYLE_1BUTTON:
            if ((MMITHEME_GetCancelTextID() == init_data.midsoft_id) ||
                (MMITHEME_GetReturnTextID() == init_data.midsoft_id))
            {
                GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, init_data.midsoft_id, FALSE);
                GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_LEFT, MSG_CTL_PENOK, MMITHEME_GetSoftkeyMidCtrlId());
            }
            else
            {
                GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, init_data.midsoft_id, FALSE);
                GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_RIGHT, MSG_CTL_PENOK, MMITHEME_GetSoftkeyMidCtrlId());
                GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            
            is_visible = FALSE;
            break;
            
        case GUISOFTKEY_BUTTONSTYLE_2BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, init_data.leftsoft_id, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_PENOK, MMITHEME_GetSoftkeyLeftCtrlId());
            GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, init_data.rightsoft_id, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_LEFT, MSG_NOTIFY_PENOK, MMITHEME_GetSoftkeyRightCtrlId());
            
            is_visible = FALSE;
            break;
            
        case GUISOFTKEY_BUTTONSTYLE_3BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, init_data.leftsoft_id, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_PENOK, MMITHEME_GetSoftkeyLeftCtrlId());
            GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, init_data.rightsoft_id, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_handle, GUITITLE_BUTTON_LEFT, MSG_NOTIFY_PENOK, MMITHEME_GetSoftkeyRightCtrlId());
            
            init_data.leftsoft_id  = MMITHEME_GetInvalidTextID();
            init_data.rightsoft_id = MMITHEME_GetInvalidTextID();
            break;
            
        default:
            break;
        }
    }
#endif
    
    //get param
    child_init.is_softkey    = TRUE;
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    create.ctrl_id            = child_init.ctrl_id;
    create.guid               = SPRD_GUI_SOFTKEY_ID;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;
    create.init_data_ptr      = &init_data;
    
    if ((MMITHEME_IsIstyle()) && 
        (!is_visible))/*lint !e774*/
    {
        //do nothing
    }
    else
    {
        child_init.ctrl_ptr = MMK_CreateControl(&create);
        
        //form add child
        FormAddChildCtrl(&child_init);
    }
    
    *value_pptr = value_ptr;
    
    return TRUE;
                                     }
                                     
/*****************************************************************************/
//  Description : create the edit touch date control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTouchDate(
                                       MMI_HANDLE_T  win_handle,
                                       uint32        **value_pptr
                                       )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_TOUCH_DATE;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit touch time control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditTouchTime(
                                       MMI_HANDLE_T  win_handle,
                                       uint32        **value_pptr
                                       )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_TOUCH_TIME;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the edit picker control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildEditPicker(
                                    MMI_HANDLE_T  win_handle,
                                    uint32        **value_pptr
                                    )
{
    uint32                  *value_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set init data
    init_data.type             = GUIEDIT_TYPE_PICKER;
    init_data.is_rect_adaptive = TRUE;
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildSlider(
                                MMI_HANDLE_T  win_handle,
                                uint32        **value_pptr
                                )
{
    uint32                  *value_ptr = PNULL;
    GUISLIDER_INIT_DATA_T   init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_SLIDER_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildProgress(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    GUIPROGRESS_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_PROGRESS_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildCombox(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    GUIPROGRESS_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);
    
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_COMBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}
/*****************************************************************************/
//  Description : create the slider control
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildPrgbox(
                                  MMI_HANDLE_T  win_handle,
                                  uint32        **value_pptr
                                  )
{
    uint32                  *value_ptr = PNULL;
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLFORM_CHILD_INIT_T   child_init = {0};
	GUIPRGBOX_STYLE_E       prgbox_style = GUIPRGBOX_STYLE_VOLUME_BAR;
    
    value_ptr = *value_pptr;
    
    //get param
    child_init.is_get_active = (BOOLEAN)(*value_ptr++);
	prgbox_style             = (GUIPRGBOX_STYLE_E)(*value_ptr++);
    child_init.ctrl_id       = (MMI_CTRL_ID_T)(*value_ptr++);
    child_init.parent_handle = MMK_GetCtrlHandleByWin(win_handle,*value_ptr++);

	//set init data
    init_data.style = prgbox_style;
	
    //set control param
    ctrl_create.ctrl_id            = child_init.ctrl_id;
    ctrl_create.guid               = SPRD_GUI_PRGBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = child_init.parent_handle;
    ctrl_create.parent_win_handle  = win_handle;
    
    //creat control
    child_init.ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    //form add child
    FormAddChildCtrl(&child_init);
    
    *value_pptr = value_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : form control add child control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FormAddChildCtrl(
                            CTRLFORM_CHILD_INIT_T    *init_ptr   //in:
                            )
{
    GUI_BG_T            ctrl_bg = {0};
    CTRLFORM_CHILD_T    *new_child_ptr = PNULL;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    if ((PNULL != init_ptr) && 
        (PNULL != init_ptr->ctrl_ptr))
    {
        //get form pointer by control id
        form_ctrl_ptr = CTRLFORM_GetPtr(init_ptr->parent_handle);
        SCI_ASSERT(PNULL != form_ctrl_ptr); /*assert verified*/
        
        //alloc child memory
        new_child_ptr = SCI_ALLOC_APP(sizeof(CTRLFORM_CHILD_T));
        SCI_MEMSET(new_child_ptr,0,sizeof(CTRLFORM_CHILD_T));
        
        //set space etc.
        new_child_ptr->is_bg         = init_ptr->is_bg;
        new_child_ptr->is_get_active = init_ptr->is_get_active;
        new_child_ptr->ver_space     = form_ctrl_ptr->theme.ver_space;
        new_child_ptr->hor_space     = form_ctrl_ptr->theme.hor_space;

        //set size range
        new_child_ptr->size_range.min_size = 0;
        new_child_ptr->size_range.max_size = (uint16)(-1);
        
        //set display type
        new_child_ptr->display_type   = GUIFORM_CHILD_DISP_NORMAL;
        new_child_ptr->child_ctrl_ptr = init_ptr->ctrl_ptr;
        new_child_ptr->ctrl_handle    = GUICTRL_GetCtrlHandle(init_ptr->ctrl_ptr);
        
        if (init_ptr->is_bg)
        {
            if (PNULL == form_ctrl_ptr->bg_child_ptr)
            {
                //set bg child
                form_ctrl_ptr->bg_child_ptr  = new_child_ptr;
                new_child_ptr->is_get_active = FALSE;
            }
            else
            {
                SCI_PASSERT(FALSE,("FormAddChildCtrl: has one bg!")); /*assert verified*/
            }
        }
        else if (init_ptr->is_softkey)
        {
            if (PNULL == form_ctrl_ptr->softkey_child_ptr)
            {
                //set bg child
                form_ctrl_ptr->softkey_child_ptr = new_child_ptr;
                new_child_ptr->is_get_active     = FALSE;
            }
            else
            {
                SCI_PASSERT(FALSE,("FormAddChildCtrl: has one softkey!")); /*assert verified*/
            }
        }
        else
        {
            //add to child list
            AddChildToList(form_ctrl_ptr,new_child_ptr);
        }
        
        //set anim bg child init rect
        SetAnimBgRect(form_ctrl_ptr,new_child_ptr);
        
        //set no bg
        ctrl_bg.bg_type = GUI_BG_NONE;
        ctrl_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        IGUICTRL_SetBg(new_child_ptr->child_ctrl_ptr,&ctrl_bg);
        
        //set may get active
        IGUICTRL_SetState(new_child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,FALSE);
        
        //set not circular handle up/down key
        IGUICTRL_SetCircularHandleUpDown(new_child_ptr->child_ctrl_ptr,FALSE);
        
        //set softkey
        SetSoftkeyParam(form_ctrl_ptr,new_child_ptr);

        //set child layer
        if (SPRD_GUI_LIST_ID == IGUICTRL_GetCtrlGuid(new_child_ptr->child_ctrl_ptr))
        {
            form_ctrl_ptr->is_child_layer = TRUE;
        }
        
        //child form set not display progress
        if (CTRLFORM_IsFormChild(new_child_ptr))
        {
            //no progress
            IGUICTRL_SetProgress(new_child_ptr->child_ctrl_ptr,FALSE);
            
            //disable tp
            IGUICTRL_SetState(new_child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_TP,TRUE);
            
            //set type
            CTRLFORM_SetType(GUICTRL_GetCtrlHandle(new_child_ptr->child_ctrl_ptr),form_ctrl_ptr->type);
            
            //not support slide
            CTRLFORM_IsSlide(GUICTRL_GetCtrlHandle(new_child_ptr->child_ctrl_ptr),FALSE);
            
            //not display dividing line
            CTRLFORM_SetDividingLine(GUICTRL_GetCtrlHandle(new_child_ptr->child_ctrl_ptr),0,form_ctrl_ptr->theme.line_color);
        }
    }
}

/*****************************************************************************/
//  Description : add child to list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddChildToList(
                          CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                          CTRLFORM_CHILD_T  *child_ptr      //in:
                          )
{
    //add to child list,将末尾指针存放在头指针的prev
    if (PNULL == form_ctrl_ptr->first_child_ptr)
    {
        //set new child previous and next
        child_ptr->prev_ptr = child_ptr;
        child_ptr->next_ptr = PNULL;

        //set first child previous
        form_ctrl_ptr->first_child_ptr = child_ptr;
    }
    else
    {
        //set new child previous and next
        child_ptr->prev_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
        child_ptr->next_ptr = PNULL;

        //set previous child next
        form_ctrl_ptr->first_child_ptr->prev_ptr->next_ptr = child_ptr;

        //set first child previous
        form_ctrl_ptr->first_child_ptr->prev_ptr = child_ptr;
    }
    
    //set current child
    form_ctrl_ptr->cur_child_ptr = child_ptr;

    child_ptr->index = form_ctrl_ptr->child_num;

    //is layout
    if ((form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num) &&
        (0 < form_ctrl_ptr->layout_index))
    {
        //need layout
        if (0 < form_ctrl_ptr->child_num)
        {
            form_ctrl_ptr->layout_index = (uint16)(form_ctrl_ptr->child_num - 1);
        }
        else
        {
            form_ctrl_ptr->layout_index = 0;
        }
    }
        
    //set form child number
    form_ctrl_ptr->child_num++;
    form_ctrl_ptr->all_child_num++;

    //set form get active child number
    if (child_ptr->is_get_active)
    {
        form_ctrl_ptr->active_child_num++;
    }
}

/*****************************************************************************/
//  Description : set anim bg child control init rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAnimBgRect(
                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                         CTRLFORM_CHILD_T   *child_ptr      //in:
                         )
{
    GUI_RECT_T      child_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //only set anim init rect
    if ((CTRLFORM_IsAnimChild(child_ptr)) &&
        (child_ptr->is_bg))
    {
        child_rect = CTRLFORM_GetRect(form_ctrl_ptr);
        
        //set child bg
        IGUICTRL_SetBg(child_ptr->child_ctrl_ptr,&base_ctrl_ptr->bg);
        
        //set child rect
        IGUICTRL_SetRect(child_ptr->child_ctrl_ptr,&child_rect);
    }
}

/*****************************************************************************/
//  Description : set softkey param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetSoftkeyParam(
                           CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                           CTRLFORM_CHILD_T *child_ptr      //in:
                           )
{
    //is softkey
    if ((child_ptr == form_ctrl_ptr->softkey_child_ptr) &&
        (MMK_IsHandleValid(child_ptr->ctrl_handle)))
    {
        //set not get active
        IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
        
        //set top most
        IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_TOPMOST,TRUE);
        
        //get softkey height
        IGUICTRL_GetHeightByWidth(child_ptr->child_ctrl_ptr,0,&child_ptr->height);
        
        //modify form relative display rect
        CTRLFORM_InitDisplayRect(form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : creat dynamic form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaCtrl(
                                      MMI_HANDLE_T       win_handle, //in:
                                      MMI_HANDLE_T       form_handle,//in:
                                      GUIFORM_LAYOUT_E   layout_type //in:
                                      )
{
    BOOLEAN                 result = FALSE;
    GUIFORM_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    
    //set init data
    init_data.both_rect   = MMITHEME_GetWinClientBothRect(win_handle);
    init_data.layout_type = layout_type;
    
    //set control param
    ctrl_create.ctrl_id           = form_handle;
    ctrl_create.guid              = SPRD_GUI_FORM_ID;
    ctrl_create.init_data_ptr     = &init_data;
    ctrl_create.parent_win_handle = win_handle;
    
    //creat control
    if (PNULL != MMK_CreateControl(&ctrl_create))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : creat form child control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaChildCtrl(
                                           MMI_HANDLE_T          win_handle,     //in:
                                           MMI_HANDLE_T          form_handle,    //in:
                                           GUIFORM_DYNA_CHILD_T  *child_data_ptr //in:
                                           )
{
    BOOLEAN                 result = FALSE;
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    
    //get param
    child_init.is_bg         = child_data_ptr->is_bg;
    child_init.is_softkey    = child_data_ptr->is_softkey;
    child_init.is_get_active = child_data_ptr->is_get_active;
    child_init.ctrl_id       = child_data_ptr->child_handle;
    child_init.parent_handle = MMK_ConvertIdToHandle(form_handle);
    
    //creat
    create.ctrl_id            = child_data_ptr->child_handle;
    create.guid               = child_data_ptr->guid;
    create.parent_ctrl_handle = child_init.parent_handle;
    create.parent_win_handle  = win_handle;    
    create.init_data_ptr      = child_data_ptr->init_data_ptr;
    
    child_init.ctrl_ptr = MMK_CreateControl(&create);
    if (PNULL != child_init.ctrl_ptr)
    {
        //form add child
        FormAddChildCtrl(&child_init);
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : destroy form child control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_DestroyDynaChildCtrl(
                                             MMI_HANDLE_T    form_handle,    //in:
                                             MMI_HANDLE_T    child_handle    //in:
                                             )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)))
    {
        //init display rect
        CTRLFORM_InitDisplayRect(form_ctrl_ptr);

        //remove child form list
        CTRLFORM_RemoveChildFromList(form_ctrl_ptr,child_ptr);

        //destroy child control
        MMK_DestroyControl(child_handle);

        //free child node
        SCI_FREE(child_ptr);

        result = TRUE;
    }
    
    return (result);
}
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT

/*****************************************************************************/
//  Description : add child to from list
//  Global resource dependence : 
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN CTRLFORM_AddChildCtrl(
                                           MMI_HANDLE_T          win_handle,     //in:
                                           MMI_HANDLE_T          form_handle,    //in:
                                           MMI_HANDLE_T  child_ctrl_handle //in:
                                           )
{
    BOOLEAN                 result = FALSE;
    CTRLFORM_CHILD_INIT_T   child_init = {0};
    MMI_CTRL_ID_T child_ctrl_id = 0;

    //get param
    child_init.is_bg         = FALSE;
    child_init.is_softkey    = FALSE;
    child_init.is_get_active = TRUE;
    child_init.ctrl_id       = child_ctrl_handle;
    child_init.parent_handle = MMK_ConvertIdToHandle(form_handle);

    child_ctrl_id = MMK_GetCtrlId(MMK_ConvertIdToHandle(child_ctrl_handle));

    child_init.ctrl_ptr = MMK_GetCtrlPtrByWin(win_handle, child_ctrl_id);
    if (PNULL != child_init.ctrl_ptr)
    {
		CTRLFORM_OBJ_T* form_ctrl_ptr = PNULL;

        //form add child
        FormAddChildCtrl(&child_init);
		//Set first child is active ctrl default
		//get form pointer by control id
		form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
		if (PNULL != form_ctrl_ptr->first_child_ptr)
		{
			//get child pointer by first_valid_index
			CTRLFORM_SetActiveChild(form_handle, form_ctrl_ptr->first_child_ptr->ctrl_handle);
		}
        result = TRUE;
    }
    
    return (result);
}

#endif
/*****************************************************************************/

//  Description : remove child from list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_RemoveChildFromList(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         )
{
    CTRLFORM_CHILD_T    *next_child_ptr = PNULL;

    //modify child index
    next_child_ptr = child_ptr->next_ptr;
    while (PNULL != next_child_ptr)
    {
        next_child_ptr->index--;

        next_child_ptr = next_child_ptr->next_ptr;
    }

    //modify child list
    if (child_ptr == form_ctrl_ptr->first_child_ptr)
    {
        //set next child previous
        if (PNULL != child_ptr->next_ptr)
        {
            child_ptr->next_ptr->prev_ptr = child_ptr->prev_ptr;
        }

        //set first child
        form_ctrl_ptr->first_child_ptr = child_ptr->next_ptr;
    }
    else
    {
        //set previous child next
        if (PNULL != child_ptr->prev_ptr)
        {
            child_ptr->prev_ptr->next_ptr = child_ptr->next_ptr;
        }

        //set next child previous
        if (PNULL != child_ptr->next_ptr)
        {
            child_ptr->next_ptr->prev_ptr = child_ptr->prev_ptr;
        }
    }

    //modify first child previous
    if ((PNULL != form_ctrl_ptr->first_child_ptr) &&
        (form_ctrl_ptr->first_child_ptr->prev_ptr == child_ptr))
    {
        form_ctrl_ptr->first_child_ptr->prev_ptr = child_ptr->prev_ptr;
    }

    //is active child
    if (child_ptr == form_ctrl_ptr->active_child_ptr)
    {
        form_ctrl_ptr->active_child_ptr = PNULL;
    }

    //is previous active
    if (child_ptr == form_ctrl_ptr->prev_active_child_ptr)
    {
        form_ctrl_ptr->prev_active_child_ptr = PNULL;
    }

    //is bg child
    if (child_ptr == form_ctrl_ptr->bg_child_ptr)
    {
        form_ctrl_ptr->bg_child_ptr = PNULL;
    }

    //modify current child
    if (child_ptr == form_ctrl_ptr->cur_child_ptr)
    {
        if (PNULL != child_ptr->next_ptr)
        {
            form_ctrl_ptr->cur_child_ptr = child_ptr->next_ptr;
        }
        else if (0 < child_ptr->index)
        {
            form_ctrl_ptr->cur_child_ptr = child_ptr->prev_ptr;
        }
        else
        {
            form_ctrl_ptr->cur_child_ptr = PNULL;
        }
    }

    //is layout
    if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
    {
        //need layout
        if (0 < child_ptr->index)
        {
            form_ctrl_ptr->layout_index = (uint16)(child_ptr->index - 1);
        }
        else
        {
            form_ctrl_ptr->layout_index = 0;
        }
    }

    //set form child number
    form_ctrl_ptr->child_num--;
    form_ctrl_ptr->all_child_num--;

    //set form get active child number
    if (child_ptr->is_get_active)
    {
        form_ctrl_ptr->active_child_num--;
    }

    //set child
    child_ptr->prev_ptr = child_ptr;
    child_ptr->next_ptr = PNULL;
}
