/*****************************************************************************
** File Name:      ctrlcheckbox.c                                            *
** Author:         hua.fang                                                  *
** Date:           08/01/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "guicommon.h"
#include "guistring.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmk_tp.h"

#include "ctrlbase.h"
#include "ctrlbutton.h"
#include "ctrlcheckbox.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init checkbox class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckboxInitVtbl(
                                CTRLCHECKBOX_VTBL_T     *checkbox_vtbl_ptr
                                );

/*****************************************************************************/
//  Description : checkbox construct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckboxCtrlConstruct(
                                 CTRLBASE_OBJ_T             *base_ctrl_ptr,
                                 CTRLCHECKBOX_INIT_PARAM_T  *checkbox_param_ptr
                                 );

/*****************************************************************************/
//  Description : checkbox destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckBoxCtrlDestruct(
                                CTRLCHECKBOX_OBJ_T      *checkbox_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CheckBoxCtrlHandleMsg(
                                      CTRLBASE_OBJ_T           *base_ctrl_handle,  //control pointer
                                      MMI_MESSAGE_ID_E         msg_id,     //message
                                      DPARAM                   param       //add data
                                      );

/*****************************************************************************/
//  Description : get checkbox bg or fg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlGetBgOrFg(
                              CTRLBUTTON_OBJ_T  *button_ctrl_ptr,   //in:
                              GUI_BG_T          *bg_ptr,            //in/out:
                              BOOLEAN           is_bg               //in:
                              );

/*****************************************************************************/
//  Description : get checkbox bg height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 CheckboxCtrlGetBgHeight(
                                  CTRLBUTTON_OBJ_T      *button_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : tp down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlTpDown(
                           CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : tp move out
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlTpMoveOut(
                              CTRLBUTTON_OBJ_T  *button_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : get button pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLCHECKBOX_OBJ_T* GetCheckBoxPtr(
                                           MMI_HANDLE_T   ctrl_handle
                                           );

/*****************************************************************************/
//  Description : check box type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckBoxTypeOf(
                             CTRLBASE_OBJ_T   *ctrl_ptr
                             );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckboxCtrlPackInitParam(
                                    GUIBUTTON_INIT_DATA_T       *button_init_ptr,   //in
                                    CTRLCHECKBOX_INIT_PARAM_T   *checkbox_param_ptr //out
                                    );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get checkbox type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T CHECKBOX_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BUTTON_TYPE,
            "checkbox",
            sizeof(CTRLCHECKBOX_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)CheckboxCtrlConstruct,
            (OBJECT_DESTRUCTOR_FUNC)CheckBoxCtrlDestruct,
            sizeof(CTRLCHECKBOX_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)CheckboxCtrlPackInitParam,
            sizeof(CTRLCHECKBOX_VTBL_T),
            (VTBL_INIT_FUNC)CheckboxInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init checkbox class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckboxInitVtbl(
                                CTRLCHECKBOX_VTBL_T     *checkbox_vtbl_ptr
                                )
{
    CTRLBASE_VTBL_T    *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)checkbox_vtbl_ptr;
    CTRLBUTTON_VTBL_T  *button_ctrl_vtbl_ptr =  (CTRLBUTTON_VTBL_T*)checkbox_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = CheckBoxCtrlHandleMsg;

    button_ctrl_vtbl_ptr->GetButtonBgOrFg = CheckBoxCtrlGetBgOrFg;
    button_ctrl_vtbl_ptr->GetButtonBgHeight = CheckboxCtrlGetBgHeight;
    button_ctrl_vtbl_ptr->TpDown = CheckBoxCtrlTpDown;
    button_ctrl_vtbl_ptr->TpMoveOut = CheckBoxCtrlTpMoveOut;

    return TRUE;
}

/*****************************************************************************/
//  Description : checkbox construct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckboxCtrlConstruct(
                                 CTRLBASE_OBJ_T             *base_ctrl_ptr,
                                 CTRLCHECKBOX_INIT_PARAM_T  *checkbox_param_ptr
                                 )
{
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr    = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;
    CTRLCHECKBOX_OBJ_T      *checkbox_ctrl_ptr  = (CTRLCHECKBOX_OBJ_T*)base_ctrl_ptr;

    // then, init its member
    button_ctrl_ptr->theme.anim_type  = GUIBUTTON_NONE_ANIM;

    MMITHEME_GetCheckboxTheme(&checkbox_ctrl_ptr->theme);

    return TRUE;
}

/*****************************************************************************/
//  Description : checkbox destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckBoxCtrlDestruct(
                                CTRLCHECKBOX_OBJ_T    *checkbox_ctrl_ptr
                                )
{
    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckboxCtrlPackInitParam(
                                    GUIBUTTON_INIT_DATA_T       *button_init_ptr,   //in
                                    CTRLCHECKBOX_INIT_PARAM_T   *checkbox_param_ptr //out
                                    )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)checkbox_param_ptr;

    base_ctrl_param_ptr->both_rect = button_init_ptr->both_rect;

    checkbox_param_ptr->button_param.bg = button_init_ptr->bg;
}

/*****************************************************************************/
//  Description : get button is selected for radio or check button
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCHECKBOX_GetSelect(
                                     MMI_CTRL_ID_T    ctrl_id
                                     )
{
    BOOLEAN                 result = FALSE;
    CTRLCHECKBOX_OBJ_T      *checkbox_ctrl_ptr = PNULL;

    //get button pointer by control id
    checkbox_ctrl_ptr = GetCheckBoxPtr(ctrl_id);
    if (PNULL != checkbox_ctrl_ptr)
    {
        result = checkbox_ctrl_ptr->is_checked;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button is selected for radio or check button
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLCHECKBOX_SetSelect(
                                  MMI_CTRL_ID_T   ctrl_id,
                                  BOOLEAN         is_checked
                                  )
{
    CTRLCHECKBOX_OBJ_T   *checkbox_ctrl_ptr = PNULL;

    //get button pointer by control id
    checkbox_ctrl_ptr = GetCheckBoxPtr(ctrl_id);
    if (PNULL != checkbox_ctrl_ptr)
    {
        checkbox_ctrl_ptr->is_checked = is_checked;
    }
}

/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CheckBoxCtrlHandleMsg(
                                      CTRLBASE_OBJ_T        *base_ctrl_ptr,  //control pointer
                                      MMI_MESSAGE_ID_E      msg_id,     //message
                                      DPARAM                param       //add data
                                      )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl (CTRL_CHECKBOX_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);

    return (result);
}

/*****************************************************************************/
//  Description : get checkbox bg or fg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlGetBgOrFg(
                              CTRLBUTTON_OBJ_T  *button_ctrl_ptr,    //in:
                              GUI_BG_T          *bg_ptr,            //in/out:
                              BOOLEAN           is_bg               //in:
                              )
{
    CTRLCHECKBOX_OBJ_T   *checkbox_ctrl_ptr = (CTRLCHECKBOX_OBJ_T*)button_ctrl_ptr;

    if (is_bg)
    {
        bg_ptr->bg_type = GUI_BG_IMG;
        if (button_ctrl_ptr->is_gray)
        {
            bg_ptr->img_id = checkbox_ctrl_ptr->theme.check.gray;
        }
        else
        {
            if (checkbox_ctrl_ptr->is_checked)
            {
                bg_ptr->img_id = checkbox_ctrl_ptr->theme.check.select;
            }
            else
            {
                bg_ptr->img_id = checkbox_ctrl_ptr->theme.check.unselect;
            }
        }
    }
    else
    {
        bg_ptr->bg_type = GUI_BG_NONE;
    }
}

/*****************************************************************************/
//  Description : get checkbox bg height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 CheckboxCtrlGetBgHeight(
                                  CTRLBUTTON_OBJ_T  *button_ctrl_ptr
                                  )
{
    uint16          bg_height = 0;
    GUI_BG_T        button_bg = {0};

    CheckBoxCtrlGetBgOrFg(button_ctrl_ptr,&button_bg,TRUE);

    //get button bg height
    GUIRES_GetImgWidthHeight(
            PNULL, &bg_height,
            button_bg.img_id,
            button_ctrl_ptr->win_handle);

    return bg_height;
}

/*****************************************************************************/
//  Description : tp down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlTpDown(
                           CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                           )
{
    CTRLCHECKBOX_OBJ_T   *checkbox_ctrl_ptr = (CTRLCHECKBOX_OBJ_T*)button_ctrl_ptr;

    checkbox_ctrl_ptr->is_checked = (BOOLEAN)(!checkbox_ctrl_ptr->is_checked);
    checkbox_ctrl_ptr->is_tp_checked_state = checkbox_ctrl_ptr->is_checked;
}

/*****************************************************************************/
//  Description : tp move out
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckBoxCtrlTpMoveOut(
                              CTRLBUTTON_OBJ_T  *button_ctrl_ptr
                              )
{
    CTRLCHECKBOX_OBJ_T   *checkbox_ctrl_ptr = (CTRLCHECKBOX_OBJ_T*)button_ctrl_ptr;

    checkbox_ctrl_ptr->is_checked  = (BOOLEAN)(!checkbox_ctrl_ptr->is_tp_checked_state);
}

/*****************************************************************************/
//  Description : get button pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLCHECKBOX_OBJ_T* GetCheckBoxPtr(
                                           MMI_HANDLE_T   ctrl_handle
                                           )
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(CheckBoxTypeOf(ctrl_ptr));/*assert verified*/
    }
    
    return (CTRLCHECKBOX_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : check box type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckBoxTypeOf(
                             CTRLBASE_OBJ_T   *ctrl_ptr
                             )
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_CHECKBOX_TYPE);
}

