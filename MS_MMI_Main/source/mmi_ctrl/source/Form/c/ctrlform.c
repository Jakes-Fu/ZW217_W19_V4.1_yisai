/*****************************************************************************
** File Name:      ctrlform.c                                                *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "cafcontrol.h"
#include "mmk_type.h"
#include "ctrlform_export.h"
#include "ctrlform.h"
#include "guictrl.h"
#include "guires.h"
#include "guiwin.h"
#include "mmi_theme.h"
#include "guicommon.h"
#include "ctrldropdownlist_export.h"
#include "ctrlcombox_export.h"

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
extern const uint8* MMI_GetLabelImage (MMI_IMAGE_ID_T label,MMI_WIN_ID_T win_id,uint32 *size);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init form class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormInitVtbl(
                           CTRLFORM_VTBL_T  *form_vtbl_ptr
                           );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FormCtrlPackInitParam(
                                 GUIFORM_INIT_DATA_T    *form_init_ptr,   //in
                                 CTRLFORM_INIT_PARAM_T  *form_param_ptr   //out
                                 );

/*****************************************************************************/
//  Description : form construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormConstruct(
                            CTRLBASE_OBJ_T          *obj_ptr,
                            CTRLFORM_INIT_PARAM_T   *form_param_ptr
                            );

/*****************************************************************************/
//  Description : form destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormDestruct(
                           CTRLBASE_OBJ_T           *obj_ptr,
                           CTRLFORM_INIT_PARAM_T    *form_param_ptr
                           );

/*****************************************************************************/
//  Description : handle form msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FormHandleMsg(
                                 CTRLBASE_OBJ_T     *base_ctrl_ptr, //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,         //message
                                 DPARAM             param           //add data
                                 );

/*****************************************************************************/
//  Description : display form background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayFormBg(
                         BOOLEAN            is_paint,
                         GUI_RECT_T         bg_rect,
                         CTRLFORM_OBJ_T     *form_ctrl_ptr
                         );

/*****************************************************************************/
//  Description : display form save bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayFormSaveBg(
                                GUI_RECT_T      cross_rect,
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : display form dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDividingLine(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN         is_update,
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle child form get active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormGetActive(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               );

/*****************************************************************************/
//  Description : handle form child get active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormNotifyGetActive(
                                     MMI_NOTIFY_T       *notify_ptr,    //in:
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                     );

/*****************************************************************************/
//  Description : handle form child lose active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormNotifyLoseActive(
                                      MMI_NOTIFY_T      *notify_ptr,    //in:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                      );

/*****************************************************************************/
//  Description : handle edit resize msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormNotifyResize(
                                          GUI_RECT_T        resize_rect,    //in:
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                          );

/*****************************************************************************/
//  Description : resize form display rect and adjust display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResizeFormDisplayRect(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : resize form display rect and adjust display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResizeDisplayRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T   *child_ptr      //in:
                             );

/*****************************************************************************/
//  Description : handle line change msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLineChangeMsg(
                                       CTRLFORM_OBJ_T           *form_ctrl_ptr, //in:
                                       GUIFORM_NOTIFY_LINE_T    *notify_ptr     //in:
                                       );

/*****************************************************************************/
//  Description : adjust active child edit/text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustEditDisplayRect(
                                 GUI_BIG_RECT_T *prev_rect_ptr, //in:child previous relative rect
                                 CTRLFORM_OBJ_T *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : handle anim display indication message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleAnimDisplayIndMsg(
                                   CTRLFORM_OBJ_T           *form_ctrl_ptr, //in:
                                   GUIANIM_DISPLAY_IND_T    *display_ind_ptr//in:
                                   );

/*****************************************************************************/
//  Description : display form highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayFormHighlight(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : is display form highlight,yes to display select highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 是否由TP模式转换为KEY模式,如果是,显示焦点
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayHighlight(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : handle form direction key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormDirectionKey(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                          MMI_MESSAGE_ID_E  msg_id
                                          );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle form tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleFormTpPrg(
                              CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                              MMI_MESSAGE_ID_E  msg_id,         //in:
                              DPARAM            param           //in:
                              );
#endif

/*****************************************************************************/
//  Description : set child is need rect adaptive
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildRectAdaptive(
                                   CTRLFORM_CHILD_T     *child_ptr
                                   );

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildDisplay(
                              BOOLEAN                   is_update,
                              MMI_HANDLE_T              form_handle,
                              MMI_HANDLE_T              child_handle,
                              GUIFORM_CHILD_DISPLAY_E   display_type
                              );

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAllChildDisplay(
                              CTRLFORM_OBJ_T            *form_ctrl_ptr,
                              GUIFORM_CHILD_DISPLAY_E   display_type
                              );

/*****************************************************************************/
//  Description : get parent form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 获得最外层的Form handle
/*****************************************************************************/
LOCAL BOOLEAN GetParentFormCtrl(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr,     //in:
                                CTRLFORM_OBJ_T  **parent_form_pptr  //in/out:
                                );

/*****************************************************************************/
//  Description : get parent control has active child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsParentHasActiveChild(
                                     MMI_HANDLE_T   form_handle
                                     );

/*****************************************************************************/
//  Description : get next active child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextActiveChildPtr(
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                                    CTRLFORM_CHILD_T    *cur_child_ptr, //in:
                                    CTRLFORM_CHILD_T    **out_child_pptr//in/out:
                                    );

/*****************************************************************************/
//  Description : set form need layout
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormLayout(
                         CTRLFORM_OBJ_T     *form_ctrl_ptr
                         );

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildDisplayOnly(
                                  BOOLEAN                   is_nested,
                                  MMI_HANDLE_T              form_handle,
                                  MMI_HANDLE_T              child_handle,
                                  GUIFORM_CHILD_DISPLAY_E   display_type
                                  );

/*****************************************************************************/
//  Description : only set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAllChildDisplayOnly(
                                  CTRLFORM_OBJ_T            *form_ctrl_ptr,
                                  GUIFORM_CHILD_DISPLAY_E   display_type
                                  );

/*****************************************************************************/
//  Description : set form rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const GUI_RECT_T  *rect_ptr
                              );

/*****************************************************************************/
//  Description : get form rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN FormCtrlGetRect(
                               CTRLBASE_OBJ_T   *base_ctrl_ptr,
                               GUI_RECT_T       *rect_ptr
                               );

/*****************************************************************************/
//  Description : set form lcd device
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetLcdDevInfo(
                                    CTRLBASE_OBJ_T          *base_ctrl_ptr,
                                    const GUI_LCD_DEV_INFO  *lcd_dev_ptr
                                    );

/*****************************************************************************/
//  Description : set form padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetPadding(
                                 CTRLBASE_OBJ_T *base_ctrl_ptr,
                                 GUI_PADDING_T  *padding_ptr
                                 );

/*****************************************************************************/
//  Description : set form bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetBg(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            );

/*****************************************************************************/
//  Description : set form is display progress
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetProgress(
                                  CTRLBASE_OBJ_T    *base_ctrl_ptr,
                                  BOOLEAN           is_display
                                  );

/*****************************************************************************/
//  Description : get form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetDisplayRect(
                                     CTRLBASE_OBJ_T     *base_ctrl_ptr, //in:
                                     GUI_RECT_T         *rect_ptr       //in/out:
                                     );

/*****************************************************************************/
//  Description : set form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetDisplayRect(
                                     CTRLBASE_OBJ_T     *base_ctrl_ptr, //in:
                                     const GUI_RECT_T   *rect_ptr,      //in:
                                     BOOLEAN            is_update       //in:
                                     );

/*****************************************************************************/
//  Description : get form active child select rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetSelectRect(
                                    CTRLBASE_OBJ_T  *base_ctrl_ptr, //in:
                                    GUI_RECT_T      *rect_ptr       //in/out:
                                    );

/*****************************************************************************/
//  Description : get form acitve child index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetSelectIndex(
                                     CTRLBASE_OBJ_T *base_ctrl_ptr, //in:
                                     BOOLEAN        *is_first_ptr,  //in/out:
                                     BOOLEAN        *is_last_ptr    //in/out:
                                     );

#ifdef GUI_DUMP_CTRL_TREE
#define CTRLFORM_DUMP_MAX_INDENT    10      // 最大缩进个数
#define CTRLFORM_DUMP_INDENT_STEP   2       // 缩进步长
/*****************************************************************************/
//  Description : dump child basic info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpFormMainInfo(
                            CTRLFORM_OBJ_T      *form_ctrl_ptr,
                            uint16              deep_level
                            );

/*****************************************************************************/
//  Description : dump form info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpFormInfo(
                        CTRLFORM_OBJ_T      *form_ctrl_ptr,
                        uint16              deep_level
                        );

/*****************************************************************************/
//  Description : dump child info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpChildInfo(
                         CTRLFORM_CHILD_T   *child_ptr,
                         uint16             deep_level
                         );
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get form type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T FORM_GetType(void)
{
    static TYPE_TAG_T   obj_type = 0;
    
    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "form",
            sizeof(CTRLFORM_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)FormConstruct,
            (OBJECT_DESTRUCTOR_FUNC)FormDestruct,
            sizeof(CTRLFORM_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)FormCtrlPackInitParam,
            sizeof(CTRLFORM_VTBL_T),
            (VTBL_INIT_FUNC)FormInitVtbl);
    }
    
    return obj_type;
}

/*****************************************************************************/
//  Description : init form class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormInitVtbl(
                           CTRLFORM_VTBL_T  *form_vtbl_ptr
                           )
{
    BOOLEAN             result = TRUE;
    CTRLBASE_VTBL_T     *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)form_vtbl_ptr;
    
    base_ctrl_vtbl_ptr->HandleEvent         = FormHandleMsg;
    base_ctrl_vtbl_ptr->SetRect             = FormCtrlSetRect;
    base_ctrl_vtbl_ptr->GetRect             = FormCtrlGetRect;
    base_ctrl_vtbl_ptr->SetLcdDevInfo       = FormCtrlSetLcdDevInfo;
    base_ctrl_vtbl_ptr->SetPadding          = FormCtrlSetPadding;
    base_ctrl_vtbl_ptr->SetBg               = FormCtrlSetBg;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = FormCtrlSetProgress;
    base_ctrl_vtbl_ptr->GetHeightByWidth    = CTRLFORM_GetHeightByWidth;
    base_ctrl_vtbl_ptr->GetDisplayRect      = FormCtrlGetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect      = FormCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->GetSelectRect       = FormCtrlGetSelectRect;
    base_ctrl_vtbl_ptr->GetSelectIndex      = FormCtrlGetSelectIndex;
    
    return (result);
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FormCtrlPackInitParam(
                                 GUIFORM_INIT_DATA_T    *form_init_ptr,   //in
                                 CTRLFORM_INIT_PARAM_T  *form_param_ptr   //out
                                 )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)form_param_ptr;

    base_ctrl_param_ptr->both_rect = form_init_ptr->both_rect;

    form_param_ptr->layout_type = form_init_ptr->layout_type;
}

/*****************************************************************************/
//  Description : form construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormConstruct(
                            CTRLBASE_OBJ_T          *obj_ptr,
                            CTRLFORM_INIT_PARAM_T   *form_param_ptr
                            )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = (CTRLFORM_OBJ_T*)obj_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)obj_ptr;
    
    //set display progress
    form_ctrl_ptr->is_display_prg = TRUE;
    
    //child permit set bg and font
    form_ctrl_ptr->is_permit_bg     = TRUE;
    form_ctrl_ptr->is_permit_border = TRUE;
    form_ctrl_ptr->is_permit_font   = TRUE;
    form_ctrl_ptr->is_permit_font_color = FALSE;

    //set size range
    form_ctrl_ptr->size_range.min_size = 0;
    form_ctrl_ptr->size_range.max_size = (uint16)(-1);
    
    //set form rect
    form_ctrl_ptr->original_rect = base_ctrl_ptr->both_rect.v_rect;
    
    //set form layout type
    form_ctrl_ptr->layout_type = form_param_ptr->layout_type;
    
    //set display direction
    form_ctrl_ptr->display_dir = MMITHEME_GetDisDirection();
    
    //set win handle
    form_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
    
    //get form theme
    MMITHEME_GetFormTheme(&form_ctrl_ptr->theme);
    base_ctrl_ptr->lcd_dev_info = form_ctrl_ptr->theme.lcd_dev;
    base_ctrl_ptr->bg           = form_ctrl_ptr->theme.bg;
    base_ctrl_ptr->border       = form_ctrl_ptr->theme.border;
    
    //init display rect
    CTRLFORM_InitDisplayRect(form_ctrl_ptr);
    
    //set state
    if (0 == MMK_GetParentCtrlHandle(base_ctrl_ptr->handle))
    {
        //child form padding default is 0,not is 3
        base_ctrl_ptr->padding = form_ctrl_ptr->theme.padding;
        VTLBASE_SetCanKbdActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,TRUE);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : form destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormDestruct(
                           CTRLBASE_OBJ_T           *obj_ptr,
                           CTRLFORM_INIT_PARAM_T    *form_param_ptr
                           )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = (CTRLFORM_OBJ_T*)obj_ptr;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_next_ptr = PNULL;
    
    if (PNULL != form_ctrl_ptr)
    {
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
#endif
        
        //free backup bg
        if (PNULL != form_ctrl_ptr->bg_backup.buf_ptr)
        {
            SCI_FREE(form_ctrl_ptr->bg_backup.buf_ptr);
        }
        
        //free bg child
        if (PNULL != form_ctrl_ptr->bg_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->bg_child_ptr);
        }
        
        //free softkey child
        if (PNULL != form_ctrl_ptr->softkey_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->softkey_child_ptr);
        }
        
#ifdef GUIF_FORM_DOCKABLE
        //free top child
        if (PNULL != form_ctrl_ptr->top_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->top_child_ptr);
        }

        //free bottom child
        if (PNULL != form_ctrl_ptr->bottom_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->bottom_child_ptr);
        }

        //free left child
        if (PNULL != form_ctrl_ptr->left_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->left_child_ptr);
        }

        //free right child
        if (PNULL != form_ctrl_ptr->right_child_ptr)
        {
            SCI_FREE(form_ctrl_ptr->right_child_ptr);
        }
#endif
        
        //free child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            child_next_ptr = child_ptr->next_ptr;
            
            //free
            SCI_FREE(child_ptr);
            
            child_ptr = child_next_ptr;
        }
        form_ctrl_ptr->first_child_ptr  = PNULL;
        form_ctrl_ptr->active_child_ptr = PNULL;
        form_ctrl_ptr->cur_child_ptr    = PNULL;
        
        //free prgbox control memory
        if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
        {
            form_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }
    }
    
    GUI_INVALID_PARAM(form_param_ptr);  /*lint !e522*/
    
    return (result);
}

/*****************************************************************************/
//  Description : handle form msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FormHandleMsg(
                                 CTRLBASE_OBJ_T     *base_ctrl_ptr, //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,         //message
                                 DPARAM             param           //add data
                                 )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = (CTRLFORM_OBJ_T*)base_ctrl_ptr;
    
    SCI_ASSERT(PNULL != form_ctrl_ptr); /*assert verified*/
    
    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        //current language display direction is different layout
        if (form_ctrl_ptr->display_dir != MMITHEME_GetDisDirection())
        {
            //need layout
            form_ctrl_ptr->layout_index = 0;
        }
        
        //layout
        CTRLFORM_Layout(form_ctrl_ptr,TRUE,FALSE);
        
        //display
        CTRLFORM_Display(TRUE,TRUE,form_ctrl_ptr);
        break;
        
    case MSG_CTL_GET_ACTIVE:
        HandleFormGetActive(form_ctrl_ptr);
        break;
        
    case MSG_NOTIFY_GET_ACTIVE:
        HandleFormNotifyGetActive((MMI_NOTIFY_T *)param,form_ctrl_ptr);
        break;
        
    case MSG_NOTIFY_LOSE_ACTIVE:
        HandleFormNotifyLoseActive((MMI_NOTIFY_T *)param,form_ctrl_ptr);
        break;
        
    case MSG_NOTIFY_RESIZE:
        result = HandleFormNotifyResize(*(GUI_RECT_T *)param,form_ctrl_ptr);
        break;
        
    case MSG_NOTIFY_UPDATE:
        if (!form_ctrl_ptr->slide_info.is_on_slide_paint)
        {
            //update bg and border
            CTRLFORM_DisplayRect((GUI_RECT_T *)param,form_ctrl_ptr);
        }
        break;
        
    case MSG_NOTIFY_LINE_CHANGE:
        result = HandleLineChangeMsg(form_ctrl_ptr,(GUIFORM_NOTIFY_LINE_T *)param);
        break;
        
    case MSG_NOTIFY_ANIM_DISPLAY_IND:
    case MSG_CTL_ANIM_GIF_UPDATE:
        HandleAnimDisplayIndMsg(form_ctrl_ptr,(GUIANIM_DISPLAY_IND_T *)param);
        break;

    case MSG_NOTIFY_HIGHLIGHT:
        DisplayFormHighlight(form_ctrl_ptr);
        break;
        
    case MSG_CTL_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
        
        //reset form slide
        CTRLFORM_ResetSlide(FALSE,form_ctrl_ptr);
        
        //reset slide state
        form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
#endif
        break;
        
    case MSG_TIMER:
#ifdef TOUCH_PANEL_SUPPORT
        if (form_ctrl_ptr->slide_info.slide_timer_id == *(uint8*)param)
        {
            CTRLFORM_HandleSlideTimer(form_ctrl_ptr);
        }
        else
#endif        
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_APP_OK:
    case MSG_NOTIFY_OK:
        if (MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state)
        {
            //notify parent control
            result = GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_OK);
        }
        break;
        
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
        if (MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state)
        {
            //not display highlight
            if (!IsDisplayHighlight(form_ctrl_ptr))
            {
                //notify parent control
                result = GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_MIDSK);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
        if (MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state)
        {
            //notify parent control
            result = GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_CANCEL);
        }
        break;
        
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleFormDirectionKey(form_ctrl_ptr,MSG_APP_UP);
        break;
        
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleFormDirectionKey(form_ctrl_ptr,MSG_APP_DOWN);
        break;
        
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleFormDirectionKey(form_ctrl_ptr,MSG_APP_LEFT);
        break;
        
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleFormDirectionKey(form_ctrl_ptr,MSG_APP_RIGHT);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_SHORT:
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_UP:
        result = CTRLFORM_DispatchTpMsg(form_ctrl_ptr,msg_id,param);
        break;
        
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
    case MSG_CTL_PRGBOX_MOVE:
        if (MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state)
        {
            HandleFormTpPrg(form_ctrl_ptr,msg_id,param);
        }
        break;
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : display form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_Display(
                             BOOLEAN         is_paint,
                             BOOLEAN         is_update_prgbox,
                             CTRLFORM_OBJ_T  *form_ctrl_ptr
                             )
{
    GUI_RECT_T      border_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if (PNULL != form_ctrl_ptr)
    {
        //display bg
        DisplayFormBg(is_paint,base_ctrl_ptr->rect,form_ctrl_ptr);
        
        //set border rect
        border_rect        = base_ctrl_ptr->rect;
        border_rect.left   = (int16)(border_rect.left + base_ctrl_ptr->padding.left);
        border_rect.top    = (int16)(border_rect.top + base_ctrl_ptr->padding.top);
        border_rect.right  = (int16)(border_rect.right - base_ctrl_ptr->padding.right);
        border_rect.bottom = (int16)(border_rect.bottom - base_ctrl_ptr->padding.bottom);

        //display border
        GUI_DisplayBorder(border_rect,
            base_ctrl_ptr->display_rect,
            &base_ctrl_ptr->border,
            &base_ctrl_ptr->lcd_dev_info);
        
        if (is_paint)
        {
            //display form dividing line
            DisplayDividingLine(form_ctrl_ptr);
        }
        
        //display scroll bar
        DisplayScrollBar(is_update_prgbox,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : display form control by rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DisplayRect(
                                 GUI_RECT_T      *rect_ptr,      //in:
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                 )
{
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      border_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != rect_ptr))
    {
        //display border
        if (GUI_IntersectRect(&display_rect,*rect_ptr,base_ctrl_ptr->display_rect))
        {
            //display bg
            DisplayFormBg(FALSE,display_rect,form_ctrl_ptr);

            //set border rect
            border_rect        = base_ctrl_ptr->rect;
            border_rect.left   = (int16)(border_rect.left + base_ctrl_ptr->padding.left);
            border_rect.top    = (int16)(border_rect.top + base_ctrl_ptr->padding.top);
            border_rect.right  = (int16)(border_rect.right - base_ctrl_ptr->padding.right);
            border_rect.bottom = (int16)(border_rect.bottom - base_ctrl_ptr->padding.bottom);

            //display border
            GUI_DisplayBorder(border_rect,
                display_rect,
                &base_ctrl_ptr->border,
                &base_ctrl_ptr->lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : display form background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayFormBg(
                         BOOLEAN            is_paint,
                         GUI_RECT_T         bg_rect,
                         CTRLFORM_OBJ_T     *form_ctrl_ptr
                         )
{
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          form_rect = {0};
    GUI_RECT_T          img_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    GUI_BG_DISPLAY_T    bg_display = {0};
    
    //get crossed rect
    if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,bg_rect))
    {
        //display form save bg
        if (!DisplayFormSaveBg(cross_rect,form_ctrl_ptr))
        {
            //get form rect
            form_rect = CTRLFORM_GetRect(form_ctrl_ptr);
            
            //set image rect
            img_rect.left   = (int16)(cross_rect.left - form_rect.left);
            img_rect.top    = (int16)(cross_rect.top - form_rect.top);
            img_rect.right  = (int16)(img_rect.left + cross_rect.right - cross_rect.left);
            img_rect.bottom = (int16)(img_rect.top + cross_rect.bottom - cross_rect.top);
            
            //set bg display info
            bg_display.rect         = base_ctrl_ptr->rect;
            bg_display.display_rect = cross_rect;
            bg_display.img_rect     = img_rect;
            bg_display.ctrl_handle  = base_ctrl_ptr->handle;
            bg_display.win_handle   = form_ctrl_ptr->win_handle;
            
            //display bg
            GUI_DisplayBg(&base_ctrl_ptr->bg,
                &bg_display,
                &base_ctrl_ptr->lcd_dev_info);
        }
        
        //update bg control
        if ((!is_paint) &&
            (PNULL != form_ctrl_ptr->bg_child_ptr))
        {
            GUIANIM_UpdateRectByPtr(cross_rect,form_ctrl_ptr->bg_child_ptr->child_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : display form save bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayFormSaveBg(
                                GUI_RECT_T      cross_rect,
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                )
{
    BOOLEAN         result = FALSE;
    uint8           *img_ptr = PNULL;
    uint16          dest_width = 0;
    uint16          dest_height = 0;
    uint32          img_size = 0;
    uint32          dest_buf_size = 0;
    GUI_POINT_T     display_point = {0};
    GUI_RECT_T      form_rect = {0};
    GUIIMG_BITMAP_T img_bitmap = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if ((form_ctrl_ptr->bg_backup.is_save) && 
        (GUI_BG_IMG == base_ctrl_ptr->bg.bg_type))
    {
        //get form rect
        form_rect = CTRLFORM_GetRect(form_ctrl_ptr);
        
        if (PNULL == form_ctrl_ptr->bg_backup.buf_ptr)
        {
            if (GUIRES_GetImgWidthHeight(&dest_width,&dest_height,base_ctrl_ptr->bg.img_id,form_ctrl_ptr->win_handle))
            {
                //set dest
                dest_width  = (uint16)MIN(dest_width,(uint16)(form_rect.right - form_rect.left + 1));
                dest_height = (uint16)MIN(dest_height,(uint16)(form_rect.bottom - form_rect.top + 1));
                
                //alloc memory
                dest_buf_size = dest_width * dest_height * 2;
                
                form_ctrl_ptr->bg_backup.buf_ptr = SCI_ALLOCA(dest_buf_size);
                if (PNULL != form_ctrl_ptr->bg_backup.buf_ptr)
                {
                    //get src image data
                    img_ptr = (uint8 *)MMI_GetLabelImage(base_ctrl_ptr->bg.img_id,form_ctrl_ptr->win_handle,&img_size);
                    
                    result = GUIRES_CopyImgBuf(img_ptr,
                        dest_width,
                        dest_height,
                        dest_buf_size,
                        img_size,
                        &display_point,
                        PNULL,
                        (GUI_COLOR_T *)form_ctrl_ptr->bg_backup.buf_ptr);
                    
                    form_ctrl_ptr->bg_backup.img_width  = dest_width;
                    form_ctrl_ptr->bg_backup.img_height = dest_height;
                }
            }
        }
        else
        {
            result = TRUE;
        }
        
        if (result)
        {
            display_point.x = form_rect.left;
            display_point.y = form_rect.top;
            
            //set bitmap
            img_bitmap.bit_ptr    = form_ctrl_ptr->bg_backup.buf_ptr;
            img_bitmap.img_width  = form_ctrl_ptr->bg_backup.img_width;
            img_bitmap.img_height = form_ctrl_ptr->bg_backup.img_height;
            
            //display
            GUIIMG_DisplayBmp(FALSE,
                &cross_rect,
                &display_point,
                &img_bitmap,
                &base_ctrl_ptr->lcd_dev_info);
        }
    }
    
    
    return (result);
}

/*****************************************************************************/
//  Description : display form dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDividingLine(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr
                               )
{
    GUI_RECT_T          child_rect = {0};
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    if (0 < form_ctrl_ptr->theme.line_width)
    {
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            //child is valid
            if (CTRLFORM_IsValidChild(child_ptr))
            {
                //get child rect
                IGUICTRL_GetRect(child_ptr->child_ctrl_ptr,&child_rect);
                
                //display child dividing line
                CTRLFORM_DisplayChildLine(form_ctrl_ptr,child_ptr,child_rect);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : display form dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DisplayChildLine(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                                      CTRLFORM_CHILD_T  *child_ptr,     //in:
                                      GUI_RECT_T        child_rect      //in:
                                      )
{
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_BIG_RECT_T  big_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if (0 < form_ctrl_ptr->theme.line_width)
    {
        big_rect = form_ctrl_ptr->relative_disp_rect;
        CTRLFORM_ClientToScreen(form_ctrl_ptr,
            &big_rect);
        
        //get form display rect except softkey
        display_rect.left   = (int16)big_rect.left;
        display_rect.top    = (int16)big_rect.top;
        display_rect.right  = (int16)big_rect.right;
        display_rect.bottom = (int16)big_rect.bottom;
        
        //draw top line
        if (child_ptr->index == form_ctrl_ptr->first_valid_index)
        {
            //get line rect
            line_rect        = child_rect;
            line_rect.bottom = (int16)(line_rect.top - 1);
            line_rect.top    = (int16)(line_rect.bottom - form_ctrl_ptr->theme.line_width + 1);
            
            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,display_rect,line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    cross_rect,
                    form_ctrl_ptr->theme.line_color);
            }
        }
        
        //get line rect
        line_rect        = child_rect;
        line_rect.top    = (int16)(line_rect.bottom + 1);
        line_rect.bottom = (int16)(line_rect.top + form_ctrl_ptr->theme.line_width - 1);
        
        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,display_rect,line_rect))
        {
            //draw line
            GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                cross_rect,
                form_ctrl_ptr->theme.line_color);
        }
    }
}

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN         is_update,
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            )
{
    int32           display_min_top = 0;
    int32           display_max_top = 0;
    uint32          item_index = 0; 
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      display_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;

    if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
    {
        prg_rect = CTRLFORM_GetScrollBarRect(form_ctrl_ptr);

        //get display rect
        if (!GUI_IntersectRect(&display_rect,prg_rect,base_ctrl_ptr->display_rect))
        {
            display_rect.left   = 0;
            display_rect.top    = 0;
            display_rect.right  = 0;
            display_rect.bottom = 0;
        }

        //set display rect
        IGUICTRL_SetDisplayRect(form_ctrl_ptr->prgbox_ctrl_ptr,&display_rect,FALSE);

        //set progress param
        GUIPRGBOX_SetParamByPtr(form_ctrl_ptr->prgbox_ctrl_ptr,
            CTRLFORM_GetRelativeHeight(form_ctrl_ptr),
            (prg_rect.bottom - prg_rect.top + 1));

        //get display min and max top
        display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);

        //get current item index
        if (display_min_top > form_ctrl_ptr->relative_disp_rect.top)
        {
            item_index = 0;
        }
        else if (form_ctrl_ptr->relative_disp_rect.top > display_max_top)
        {
            item_index = display_max_top;
        }
        else
        {
            item_index = form_ctrl_ptr->relative_disp_rect.top - display_min_top;
        }
        
        //set current position
        GUIPRGBOX_SetPosByPtr(is_update,
            item_index,
            item_index,
            form_ctrl_ptr->prgbox_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : handle child form get active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormGetActive(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *active_child_ptr = PNULL;
    
    //get active child
    if (PNULL != form_ctrl_ptr->prev_active_child_ptr)
    {
        active_child_ptr = form_ctrl_ptr->prev_active_child_ptr;
        
        result = TRUE;
    }
    else if (CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->first_valid_index,form_ctrl_ptr,&active_child_ptr))
    {
        //get the first active child
        while (PNULL != active_child_ptr)
        {
            if (CTRLFORM_IsGetAtvChild(active_child_ptr))
            {
                result = TRUE;
                break;
            }
            
            //next child
            active_child_ptr = active_child_ptr->next_ptr;
        }
    }
    
    //get the first get active child
    if ((result) &&
        (PNULL != active_child_ptr))
    {
        //set active control
        MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,active_child_ptr->ctrl_handle);
    }
    else
    {
        form_ctrl_ptr->cur_child_ptr = form_ctrl_ptr->first_child_ptr;
    }
}

/*****************************************************************************/
//  Description : handle form child get active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormNotifyGetActive(
                                     MMI_NOTIFY_T       *notify_ptr,    //in:
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                     )
{
    MMI_CTRL_ID_T       ctrl_id = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    CTRLFORM_OBJ_T      *child_form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *active_child_ptr = PNULL;
    
    //get active child pointer by handle
    if ((CTRLFORM_GetChildPtrByHandle(notify_ptr->src_handle,form_ctrl_ptr,&active_child_ptr)) &&
        (active_child_ptr != form_ctrl_ptr->active_child_ptr))
    {
        //switch active control
        form_ctrl_ptr->active_child_ptr = active_child_ptr;
        
        //set current child
        form_ctrl_ptr->cur_child_ptr = active_child_ptr;
        
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {
            if (CTRLFORM_IsFormChild(active_child_ptr))
            {
                //unit form not handle tp first
                if (CTRLFORM_IsUnitFormChild(active_child_ptr))
                {
                    VTLBASE_SetTpPriority((CTRLBASE_OBJ_T *)form_ctrl_ptr,CTRL_TP_PRIORITY_NORMAL);
                    VTLBASE_SetCanActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,FALSE);
                    
                    //get form pointer
                    child_form_ctrl_ptr = CTRLFORM_GetPtr(active_child_ptr->ctrl_handle);
                    
                    //set form param
                    CTRLFORM_SetParam(child_form_ctrl_ptr);
                    
                    //update unit form and all child
                    CTRLFORM_Display(FALSE,TRUE,child_form_ctrl_ptr);
                    
                    //set child rect
                    CTRLFORM_SetChildRect(TRUE,child_form_ctrl_ptr);
                }
            }
            else
            {
                //set child param
                CTRLFORM_SetChildParam(form_ctrl_ptr,active_child_ptr);
                
                //is unit form
                if (CTRLFORM_SetParam(form_ctrl_ptr))
                {
                    //update unit form and all child
                    CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
                    
                    //set child rect
                    CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
                }
                else
                {
                    if (PNULL != active_child_ptr)
                    {
                        IGUICTRL_HandleEvent(active_child_ptr->child_ctrl_ptr,MSG_CTL_PAINT,PNULL);
                    }
                }
                
                if (PNULL != active_child_ptr)
                {
                     //post switch active msg
                    ctrl_id = MMK_GetCtrlId(active_child_ptr->ctrl_handle);
                    MMK_PostMsg(form_ctrl_ptr->win_handle,MSG_NOTIFY_FORM_SWITCH_ACTIVE,&ctrl_id,sizeof(MMI_CTRL_ID_T));
                }
            }
        }
        
        //notify parent control
        GUICTRL_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
    }
}

/*****************************************************************************/
//  Description : handle form child lose active msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormNotifyLoseActive(
                                      MMI_NOTIFY_T      *notify_ptr,    //in:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                      )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    CTRLFORM_OBJ_T      *child_form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get lose active child pointer by handle
    if ((CTRLFORM_GetChildPtrByHandle(notify_ptr->src_handle,form_ctrl_ptr,&child_ptr)) &&
        (child_ptr == form_ctrl_ptr->active_child_ptr))
    {
        //switch active control
        form_ctrl_ptr->prev_active_child_ptr = form_ctrl_ptr->active_child_ptr;
        form_ctrl_ptr->active_child_ptr = PNULL;
        
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            //unit form not handle tp first
            if (CTRLFORM_IsUnitFormChild(child_ptr))
            {
                VTLBASE_SetTpPriority((CTRLBASE_OBJ_T *)form_ctrl_ptr,CTRL_TP_PRIORITY_NORMAL);
                VTLBASE_SetCanActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,FALSE);
                
                //get form pointer
                child_form_ctrl_ptr = CTRLFORM_GetPtr(child_ptr->ctrl_handle);
                
                //set form param
                CTRLFORM_SetParam(child_form_ctrl_ptr);
                
                //update unit form and all child
                CTRLFORM_Display(FALSE,TRUE,child_form_ctrl_ptr);
                
                //set child rect
                CTRLFORM_SetChildRect(TRUE,child_form_ctrl_ptr);
            }
        }
        else
        {
            //set child param
            CTRLFORM_SetChildParam(form_ctrl_ptr,child_ptr);
            
            //is unit form
            if (CTRLFORM_SetParam(form_ctrl_ptr))
            {
                //update unit form and all child
                CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
                
                //set child rect
                CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
            }
            else
            {
                if (PNULL != child_ptr)
                {
                    IGUICTRL_HandleEvent(child_ptr->child_ctrl_ptr,MSG_CTL_PAINT,PNULL);
                }
            }
        }
        
        //notify parent control
        GUICTRL_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_LOSE_ACTIVE);
    }
}

/*****************************************************************************/
//  Description : handle edit resize msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormNotifyResize(
                                          GUI_RECT_T        resize_rect,    //in:
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                          )
{
    BOOLEAN         is_update = FALSE;
    int32           display_top = 0;
    int32           display_left = 0;
    GUI_RECT_T      rect_before_resize = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //no parent control
    if (0 == MMK_GetParentCtrlHandle(base_ctrl_ptr->handle))
    {
        //set rect before resize
        rect_before_resize = base_ctrl_ptr->rect;
        
        //reset rect
        base_ctrl_ptr->rect = form_ctrl_ptr->original_rect;
        
        //modify form rect
        if ((resize_rect.top > form_ctrl_ptr->original_rect.top) &&
            (resize_rect.top <= form_ctrl_ptr->original_rect.bottom))
        {
            base_ctrl_ptr->rect.bottom = (int16)(resize_rect.top - 1);
        }
        
        //set display rect
        base_ctrl_ptr->display_rect = base_ctrl_ptr->rect;
        
        if (GUI_EqualRect(rect_before_resize,base_ctrl_ptr->rect))
        {
            //set display left and top
            display_left = form_ctrl_ptr->relative_disp_rect.left;
            display_top  = form_ctrl_ptr->relative_disp_rect.top;
            
            //adjust display top by edit cursor
            if (CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,form_ctrl_ptr->active_child_ptr))
            {
                //adjust display top
                CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr);
                
                is_update = TRUE;
            }
        }
        else
        {
            //is layout
            if (0 < form_ctrl_ptr->layout_index)
            {
                //need layout
                form_ctrl_ptr->layout_index = 0;
                
                //layout
                CTRLFORM_Layout(form_ctrl_ptr,TRUE,FALSE);
                
                is_update = TRUE;
            }
            
            //resize form display rect
            ResizeFormDisplayRect(form_ctrl_ptr);
        }
        
        //update
        if (is_update)
        {
            //set child rect
            CTRLFORM_SetChildRect(FALSE,form_ctrl_ptr);
            
            //update window
            if (MMK_IsFocusWin(form_ctrl_ptr->win_handle))
            {
                if (MMK_GetWinDisplayStyleState(form_ctrl_ptr->win_handle, WS_HAS_TRANSPARENT))
                {
                    MMK_UpdateScreen();
                }
                else
                {
                    MMK_SendMsg(form_ctrl_ptr->win_handle,MSG_FULL_PAINT,PNULL);
                }
            }
            
            //need to window
            result = MMI_RESULT_FALSE;
        }
        else
        {
            //not to window
            result = MMI_RESULT_TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : resize form display rect and adjust display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResizeFormDisplayRect(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 )
{
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //set child
    child_ptr = form_ctrl_ptr->active_child_ptr;
    if (PNULL == child_ptr)
    {
        child_ptr = form_ctrl_ptr->cur_child_ptr;
    }

#ifdef GUIF_FORM_DOCKABLE
    if (CTRLFORM_IsDockChild(child_ptr))
    {
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            //resize dock child
            ResizeFormDisplayRect((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr);
        }
    }
    else
#endif
    {
        //resize child
        ResizeDisplayRect(form_ctrl_ptr,child_ptr);
    }
}

/*****************************************************************************/
//  Description : resize form display rect and adjust display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResizeDisplayRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T   *child_ptr      //in:
                             )
{
    BOOLEAN         is_need_prg = FALSE;
    uint16          display_height = 0;
    int32           display_max_top = 0;
    int32           display_top = 0;
    uint32          relative_height = 0;
    GUI_BIG_RECT_T  relative_disp_rect = {0};

    if ((PNULL != child_ptr) &&
        (!CTRLFORM_IsDockChild(child_ptr)))
    {
        //get form relative hight
        relative_height = CTRLFORM_GetRelativeHeight(form_ctrl_ptr);

        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);

        //after resize, is need progress
        if (relative_height > display_height)
        {
            is_need_prg = TRUE;
        }

        //get display max top
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);

        //set display top
        display_top = form_ctrl_ptr->relative_disp_rect.top;

        //set relative display rect
        relative_disp_rect.top    = display_top;
        relative_disp_rect.bottom = display_top + display_height - 1;

        //active child is not display all,after resize,progress is exist,need adjust display rect
        if (is_need_prg)
        {
            //child height > display height
            if ((child_ptr->relative_rect.bottom - child_ptr->relative_rect.top + 1) > display_height)
            {
                //adust display top by edit cursor
                CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,child_ptr);
            }
            else if (child_ptr->relative_rect.top < relative_disp_rect.top)
            {
                //set display top
                display_top = child_ptr->relative_rect.top;
                display_top = (int32)MIN(display_top,display_max_top);
            
                //adust display top by edit cursor
                CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,child_ptr);
            }
            else if (child_ptr->relative_rect.bottom > relative_disp_rect.bottom)
            {
                //set display top
                display_top = child_ptr->relative_rect.bottom - display_height + 1;
                display_top = (int32)MIN(display_top,display_max_top);

                //adust display top by edit cursor
                CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,child_ptr);
            }
        }

        //adjust display rect
        form_ctrl_ptr->relative_disp_rect.top    = (int32)MIN(display_top,display_max_top);
        form_ctrl_ptr->relative_disp_rect.bottom = form_ctrl_ptr->relative_disp_rect.top + display_height - 1;
    }
}

/*****************************************************************************/
//  Description : handle line change msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLineChangeMsg(
                                       CTRLFORM_OBJ_T           *form_ctrl_ptr, //in:
                                       GUIFORM_NOTIFY_LINE_T    *notify_ptr     //in:
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_HANDLE_T        parent_handle = 0;
    GUI_BIG_RECT_T      prev_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    CTRLFORM_CHILD_T    *active_child_ptr = PNULL;
    
    //get parent control handle
    parent_handle = MMK_GetParentCtrlHandle(base_ctrl_ptr->handle);
    
    //no parent control or set size form
    if ((0 == parent_handle) ||
        (notify_ptr->is_set_size))
    {
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {
            if (CTRLFORM_GetChildPtrByHandle(notify_ptr->ctrl_handle,form_ctrl_ptr,&child_ptr))
            {
                //set child relative rect
                prev_rect = child_ptr->relative_rect;
                
                if (notify_ptr->is_line_num_change)
                {
                    if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
                    {
                        //need layout
                        if (CTRLFORM_IsDockChild(child_ptr))
                        {
                            form_ctrl_ptr->layout_index = 0;
                        }
                        else
                        {
                            form_ctrl_ptr->layout_index = child_ptr->index;
                        }
                    }
                    else
                    {
                        //need layout
                        form_ctrl_ptr->layout_index = 0;
                    }
                    
                    //layout
                    CTRLFORM_Layout(form_ctrl_ptr,FALSE,FALSE);

                    //only dock form handle this msg
                    if (notify_ptr->is_set_size)
                    {
                        //not to window
                        result = MMI_RESULT_TRUE;
                    }
                }
                else
                {
                    //not to window
                    result = MMI_RESULT_TRUE;
                }
                
                //adjust display top
                AdjustEditDisplayRect(&prev_rect,form_ctrl_ptr);
                
                if (MMK_IsFocusWin(form_ctrl_ptr->win_handle))
                {
                    //update display
                    CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
                    
                    //set child rect
                    CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
                }
                else
                {
                    //set child rect
                    CTRLFORM_SetChildRect(FALSE,form_ctrl_ptr);
                }

                //active child is set size form child
                active_child_ptr = form_ctrl_ptr->active_child_ptr;
                if ((PNULL != active_child_ptr) &&
                    (CTRLFORM_IsFormChild(active_child_ptr)) &&
                    (CTRLFORM_IsSetSize(active_child_ptr)) &&
                    (PNULL != ((CTRLFORM_OBJ_T *)active_child_ptr->child_ctrl_ptr)->active_child_ptr))
                {
                    //set size form need handle msg
                    notify_ptr->is_set_size = TRUE;
                    notify_ptr->ctrl_handle  = ((CTRLFORM_OBJ_T *)active_child_ptr->child_ctrl_ptr)->active_child_ptr->ctrl_handle;

                    MMK_SendMsg(active_child_ptr->ctrl_handle,MSG_NOTIFY_LINE_CHANGE,(DPARAM)notify_ptr);

                    //reset
                    notify_ptr->is_set_size = FALSE;
                }
            }
        }
        else
        {
            //not to window
            result = MMI_RESULT_TRUE;
        }
    }
    else
    {
        notify_ptr->ctrl_handle = base_ctrl_ptr->handle;

        //need layout
        form_ctrl_ptr->layout_index = 0;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : adjust active child edit/text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustEditDisplayRect(
                                 GUI_BIG_RECT_T *prev_rect_ptr, //in:child previous relative rect
                                 CTRLFORM_OBJ_T *form_ctrl_ptr  //in:
                                 )
{
    int32               display_top = 0;
    int32               display_min_top = 0;
    int32               display_max_top = 0;
    int32               display_left = 0;
    GUI_BIG_RECT_T      relative_disp_rect = form_ctrl_ptr->relative_disp_rect;
    CTRLFORM_CHILD_T    *child_ptr = form_ctrl_ptr->active_child_ptr;

    if ((PNULL != child_ptr) && 
        (!CTRLFORM_IsDockChild(child_ptr)))
    {
        //init display top and left
        display_top  = form_ctrl_ptr->relative_disp_rect.top;
        display_left = form_ctrl_ptr->relative_disp_rect.left;

        if (child_ptr->relative_rect.bottom != prev_rect_ptr->bottom)
        {
            //edit control is not display all
            if (!CTRLFORM_IsRectCovered(form_ctrl_ptr->relative_disp_rect,child_ptr->relative_rect))
            {
                //set display top
                if (child_ptr->relative_rect.bottom > prev_rect_ptr->bottom)
                {
                    display_top = display_top + child_ptr->relative_rect.bottom - prev_rect_ptr->bottom;
                }
                else
                {
                    if (display_top + child_ptr->relative_rect.bottom > prev_rect_ptr->bottom)
                    {
                        display_top = display_top + child_ptr->relative_rect.bottom - prev_rect_ptr->bottom;
                    }
                    else
                    {
                        display_top = 0;
                    }
                }
            }

            //get display min and max top
            display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
            display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);

            //adjust display top
            display_top = (int32)MAX(display_top,display_min_top);
            display_top = (int32)MIN(display_top,display_max_top);

            //get display rect by display top
            relative_disp_rect = CTRLFORM_GetDisplayRect(display_top,form_ctrl_ptr);
        }

        //edit control is not display all
        if (!CTRLFORM_IsRectCovered(relative_disp_rect,child_ptr->relative_rect))
        {
            //adust display top by edit cursor
            CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,child_ptr);
        }

        //adjust display top
        CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : handle anim display indication message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleAnimDisplayIndMsg(
                                   CTRLFORM_OBJ_T           *form_ctrl_ptr, //in:
                                   GUIANIM_DISPLAY_IND_T    *display_ind_ptr//in:
                                   )
{
    CTRLFORM_CHILD_T     *bg_child_ptr = form_ctrl_ptr->bg_child_ptr;
    
    if (PNULL != bg_child_ptr)
    {
        //anim is bg
        if ((MMK_GetCtrlId(bg_child_ptr->ctrl_handle) == display_ind_ptr->ctrl_handle) || 
            (bg_child_ptr->ctrl_handle == display_ind_ptr->ctrl_handle))
        {
            //set child rect
            CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
            
            //display scroll bar
            DisplayScrollBar(TRUE,form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : display form highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayFormHighlight(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;

    if ((form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num) &&
        (PNULL != form_ctrl_ptr->active_child_ptr))
    {
        //is unit form
        if (CTRLFORM_IsUnitForm(form_ctrl_ptr))
        {
            //display select highlight
            CTRLFORM_SetChildParam(form_ctrl_ptr,form_ctrl_ptr->active_child_ptr);

            if (CTRLFORM_SetParam(form_ctrl_ptr))
            {
                //update unit form and all child
                CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);

                //set child rect
                CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
            }
        }

        //notify parent control
        GUICTRL_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_HIGHLIGHT);
    }
}

/*****************************************************************************/
//  Description : is display form highlight,yes to display select highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 是否由TP模式转换为KEY模式,如果是,显示焦点
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayHighlight(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr
                                 )
{
    BOOLEAN     result = FALSE;

    //tp mode change to key mode
    if ((MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state) && 
        (form_ctrl_ptr->is_valid_child) &&
        (GUIFORM_TYPE_TP == form_ctrl_ptr->type) &&
        (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()) &&
        (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        //display highlight
        DisplayFormHighlight(form_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle form direction key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormDirectionKey(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                          MMI_MESSAGE_ID_E  msg_id
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    int32               display_top = 0;
    int32               display_left = 0;
    CTRLFORM_CHILD_T    *active_child_ptr = PNULL;

    if ((MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state) && 
        (form_ctrl_ptr->is_valid_child))
    {
        //is display highlight
        if (IsDisplayHighlight(form_ctrl_ptr))
        {
            result = MMI_RESULT_TRUE;
        }
        else if (CTRLFORM_GetDisplayTopForKey(&display_top,&display_left,form_ctrl_ptr,&active_child_ptr,msg_id))
        {
            //scroll form
            if (CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr))
            {
                //update display
                CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);

                //set child rect
                CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
            }

            if (active_child_ptr != form_ctrl_ptr->active_child_ptr)
            {
                //set active control
                MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,active_child_ptr->ctrl_handle);
            }

            result = MMI_RESULT_TRUE;
        }
    }

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle form tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleFormTpPrg(
                              CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                              MMI_MESSAGE_ID_E  msg_id,         //in:
                              DPARAM            param           //in:
                              )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_up = FALSE;
    uint16      display_height = 0;
    int32       display_min_top = 0;
    int32       display_max_top = 0;
    int32       scroll_step = 0;
    int32       display_top = 0;
    int32       display_left = 0;
    
    //get display height and max top
    display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
    
    //get display min and max top
    display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
    display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
    
    switch (msg_id)
    {
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
        is_up = TRUE;
        
        //get scroll step
        scroll_step = display_height * form_ctrl_ptr->theme.scroll_ratio.num1 / form_ctrl_ptr->theme.scroll_ratio.num2;
        break;
        
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
        is_up = FALSE;
        
        //get scroll step
        scroll_step = display_height * form_ctrl_ptr->theme.scroll_ratio.num1 / form_ctrl_ptr->theme.scroll_ratio.num2;
        break;
        
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
        is_up = TRUE;
        
        //get scroll step
        scroll_step = display_height;
        break;
        
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
        is_up = FALSE;
        
        //get scroll step
        scroll_step = display_height;
        break;
        
    case MSG_CTL_PRGBOX_MOVE:
        //set display top
        display_top = *(uint32*)param;
        display_top = display_top + display_min_top;
        display_top = (int32)MIN(display_top,display_max_top);
        break;
        
    default:
        SCI_PASSERT(FALSE,("HandleFormTpPrg: msg_id %d is error!",msg_id)); /*assert verified*/
        break;
    }
    
    if (MSG_CTL_PRGBOX_MOVE != msg_id)
    {
        //get display top by scroll step
        display_top = CTRLFORM_GetDisplayTopByStep(is_up,
            scroll_step,
            display_max_top,
            form_ctrl_ptr);
    }
    
    //set display left
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    //set display rect
    if (CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr))
    {
        //update display
        CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
        
        //set child rect
        CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
        
        result = TRUE;
    }
    
    return (result);
}
#endif

/*****************************************************************************/
//	Description : set active child
//	Global resource dependence :
//	Author: fangfang.yao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChild(
                             MMI_HANDLE_T    form_handle,
                             MMI_HANDLE_T   child_handle
                             )
{
    BOOLEAN             result = FALSE;
    uint16              display_height = 0;
    uint16              child_height = 0;
    int32               display_top = 0;
    int32               display_left = 0;
    int32               child_bottom = 0;
    int32               display_max_top = 0;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

    if(PNULL != form_ctrl_ptr)
    {
        //set init display top
        display_top  = form_ctrl_ptr->relative_disp_rect.top;
        display_left = form_ctrl_ptr->relative_disp_rect.left;

        //get child bottom
        child_bottom = CTRLFORM_GetChildBottom(form_ctrl_ptr->cur_child_ptr);

        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);

        //get display max top
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);

        //get child pointer by handle
        if ( CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr))
        {
            //set current child
            form_ctrl_ptr->cur_child_ptr = child_ptr;

            //set current child select index is first
            if (form_ctrl_ptr->theme.is_cir_handle_ud)
            {
                result = IGUICTRL_SetSelectIndex(child_ptr->child_ctrl_ptr,TRUE,FALSE);
            }

            //next control is all display
            {
                //get next control height
                child_height = CTRLFORM_GetChildHeight(child_ptr);

                //next control height < display height
                if (child_height > display_height)
                {
                    //set display top
                    display_top = child_ptr->relative_rect.top;
                }
                else
                {
                    //get child bottom
                    child_bottom = CTRLFORM_GetChildBottom(child_ptr);

                    //get display top by child bottom
                    if (child_bottom < display_height)
                    {
                        display_top = CTRLFORM_GetChildTop(child_ptr);
                    }
                    else
                    {
                        display_top = child_bottom - display_height + 1;
                    }
                }

                //adjust display top
                display_top = (int32)MIN(display_top,display_max_top);

                result = TRUE;
            }
        }
        if (CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr))
        {
            //update display
            CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);

            //set child rect
            CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
        }

        if (child_ptr != form_ctrl_ptr->active_child_ptr)
        {
            //set active control
            MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,child_ptr->ctrl_handle);
        }
    }
    return (result);
}

 /*****************************************************************************/
 //  Description : set form reset slide frame
 //  Global resource dependence :
 //  Author: fangfang.yao
 //  Note:
 /*****************************************************************************/
PUBLIC void CTRLFORM_SetResetSlideFrame( MMI_HANDLE_T form_handle, uint16 frame)
{
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;

    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

    //set reset_slide_frame;
    form_ctrl_ptr->theme.reset_slide_frame = frame;
}

/*****************************************************************************/
//  Description : set form rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetRect(
                             MMI_HANDLE_T    ctrl_handle,
                             GUI_RECT_T      *rect_ptr
                             )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if (PNULL != form_ctrl_ptr)
    {
        FormCtrlSetRect((CTRLBASE_OBJ_T *)form_ctrl_ptr,rect_ptr);
        
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {
            //set child rect
            CTRLFORM_SetChildRect(FALSE,form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set form is visible
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetVisible(
                                   MMI_HANDLE_T      form_handle,
                                   BOOLEAN           is_visible,
                                   BOOLEAN           is_update
                                   )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_invisible = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        is_invisible = (BOOLEAN)(!is_visible);
        
        //set bg child
        if (PNULL != form_ctrl_ptr->bg_child_ptr)
        {
            IGUICTRL_SetState((IGUICTRL_T *)(form_ctrl_ptr->bg_child_ptr->child_ctrl_ptr),GUICTRL_STATE_INVISIBLE,is_invisible);
        }
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_SetVisible( child_ptr->ctrl_handle, is_visible,is_update);
            }
            else
            {
                IGUICTRL_SetState((IGUICTRL_T *)(child_ptr->child_ctrl_ptr),GUICTRL_STATE_INVISIBLE,is_invisible);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        //set form state
        VTLBASE_SetVisible((CTRLBASE_OBJ_T *)form_ctrl_ptr,(BOOLEAN)!is_invisible);
        
        if (is_update)
        {
            //update display
            MMK_SendMsg(form_ctrl_ptr->win_handle,MSG_FULL_PAINT,PNULL);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get form type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIFORM_TYPE_E CTRLFORM_GetType(
                                       MMI_HANDLE_T ctrl_handle
                                       )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    GUIFORM_TYPE_E  type = GUIFORM_TYPE_COMMON;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if (PNULL != form_ctrl_ptr)
    {
        type = form_ctrl_ptr->type;
    }
    
    return (type);
}

/*****************************************************************************/
//  Description : set form type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetType(
                             MMI_HANDLE_T       ctrl_handle,
                             GUIFORM_TYPE_E     type
                             )
{
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if ((PNULL != form_ctrl_ptr) &&
        (type != form_ctrl_ptr->type) &&
        (GUIFORM_TYPE_MAX > type))
    {
        form_ctrl_ptr->type = type;
        
        //modify theme
        MMITHEME_ModifyFormTheme(type,&form_ctrl_ptr->theme);
        
        //set space
        CTRLFORM_SetSpace(ctrl_handle,PNULL,&form_ctrl_ptr->theme.ver_space);
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                //set type
                CTRLFORM_SetType(child_ptr->ctrl_handle,type);
                
                //not display dividing line
                CTRLFORM_SetDividingLine(child_ptr->ctrl_handle,0,form_ctrl_ptr->theme.line_color);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : get form style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIFORM_STYLE_E CTRLFORM_GetStyle(
                                         MMI_HANDLE_T   ctrl_handle
                                         )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    GUIFORM_STYLE_E style = GUIFORM_STYLE_NORMAL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if (PNULL != form_ctrl_ptr)
    {
        style = form_ctrl_ptr->style;
    }
    
    return (style);
}

/*****************************************************************************/
//  Description : set form style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetStyle(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_STYLE_E   style
                              )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if ((PNULL != form_ctrl_ptr) &&
        (GUIFORM_STYLE_MAX > style))
    {
        form_ctrl_ptr->style = style;
    }
}

/*****************************************************************************/
//  Description : set form bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetBg(
                              MMI_HANDLE_T   form_handle,//in:
                              GUI_BG_T       *bg_ptr     //in:
                              )
{
    
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != bg_ptr))
    {
        //set bg
        FormCtrlSetBg((CTRLBASE_OBJ_T *)form_ctrl_ptr,bg_ptr);
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is save bg rgb565 data
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 可以减少form bg解码次数,但是消耗内存
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSaveBg(
                                  MMI_HANDLE_T   form_handle,//in:
                                  BOOLEAN        is_save     //in:
                                  )
{
    
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set is save bg
        form_ctrl_ptr->bg_backup.is_save = is_save;
        
        //free backup bg
        if (PNULL != form_ctrl_ptr->bg_backup.buf_ptr)
        {
            SCI_FREE(form_ctrl_ptr->bg_backup.buf_ptr);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPadding(
                                   MMI_HANDLE_T     form_handle,
                                   GUI_PADDING_T    *padding_ptr
                                   )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != padding_ptr))
    {
        //set padding
        FormCtrlSetPadding((CTRLBASE_OBJ_T *)form_ctrl_ptr,padding_ptr);
        
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {
            //layout
            CTRLFORM_Layout(form_ctrl_ptr,FALSE,FALSE);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPaddingEx(
                                     MMI_HANDLE_T   form_handle,
                                     uint16         padding_space
                                     )
{
    BOOLEAN         result = FALSE;
    GUI_PADDING_T   padding = {0};

    //padding
    padding.left   = padding_space;
    padding.top    = padding_space;
    padding.right  = padding_space;
    padding.bottom = padding_space;
    
    //set padding
    result = CTRLFORM_SetPadding(form_handle,&padding);
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is display dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default don't display
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDividingLine(
                                        MMI_HANDLE_T    form_handle,//in:
                                        uint16          line_width, //in:0 don't display dividing line
                                        GUI_COLOR_T     line_color  //in:
                                        )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set dividing line
        form_ctrl_ptr->theme.line_width = line_width;
        form_ctrl_ptr->theme.line_color = line_color;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get child control id by child index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T CTRLFORM_GetChildCtrlId(
                                             MMI_HANDLE_T   form_handle,    //in:
                                             uint16         child_index     //in:
                                             )
{
    MMI_CTRL_ID_T       ctrl_id = 0;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        if (CTRLFORM_GetChildPtrByIndex(child_index,form_ctrl_ptr,&child_ptr))
        {
            ctrl_id = MMK_GetCtrlId(child_ptr->ctrl_handle);
        }
    }
    
    return (ctrl_id);
}

/*****************************************************************************/
//  Description : set form all child horizontal or vertical space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSpace(
                                 MMI_HANDLE_T    form_handle,    //in:
                                 uint16          *hor_space_ptr, //in:may PNULL
                                 uint16          *ver_space_ptr  //in:may PNULL
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //set horizontal space
            if (PNULL != hor_space_ptr)
            {
                child_ptr->hor_space = *hor_space_ptr;
            }
            
            //set vertical space
            if (PNULL != ver_space_ptr)
            {
                child_ptr->ver_space = *ver_space_ptr;
            }
            
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_SetSpace(child_ptr->ctrl_handle,hor_space_ptr,ver_space_ptr);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {
            //need layout
            form_ctrl_ptr->layout_index = 0;
            
            //layout
            CTRLFORM_Layout(form_ctrl_ptr,FALSE,FALSE);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT

/*****************************************************************************/
//  Description : set form layout_type
//  Global resource dependence : 
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetLayoutType(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_LAYOUT_E   layout_type
                              )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
    if ((PNULL != form_ctrl_ptr) &&
        (GUIFORM_LAYOUT_MAX > layout_type))
    {
        form_ctrl_ptr->layout_type = layout_type;
        
        //is layout
        if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
        {        
            //layout
            CTRLFORM_Layout(form_ctrl_ptr, FALSE, FALSE);
        }
    }
}

/***************************************************************************//*!
@brief 
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_LAYOUT_E
@note 
*******************************************************************************/
PUBLIC GUIFORM_LAYOUT_E CTRLFORM_GetLayoutType(
                              MMI_HANDLE_T      ctrl_handle
                              )
{
	CTRLFORM_OBJ_T	*form_ctrl_ptr = PNULL;
	 
	//get form pointer by control id
	form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
	if (PNULL != form_ctrl_ptr)
	{
		return form_ctrl_ptr->layout_type;
	}
	return GUIFORM_LAYOUT_ERROR;//bug1941758
}


#endif
/*****************************************************************************/
//  Description : set child horizontal or vertical space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildSpace(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      MMI_HANDLE_T   child_handle,   //in:
                                      uint16         *hor_space_ptr, //in:may PNULL
                                      uint16         *ver_space_ptr  //in:may PNULL
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
        //set horizontal space
        if ((PNULL != hor_space_ptr) &&
            (*hor_space_ptr != child_ptr->hor_space))
        {
            child_ptr->hor_space = *hor_space_ptr;
            result = TRUE;
        }
        
        //set vertical space
        if ((PNULL != ver_space_ptr) &&
            (*ver_space_ptr != child_ptr->ver_space))
        {
            child_ptr->ver_space = *ver_space_ptr;
            result = TRUE;
        }
        
        //is layout
        if ((result) && 
            (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num))
        {
            //need layout
            form_ctrl_ptr->layout_index = 0;
            
            //layout
            CTRLFORM_Layout(form_ctrl_ptr,FALSE,FALSE);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildWidth(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_WIDTH_T  *width_ptr      //in:
                                      )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)))
    {
        //set width info
        child_ptr->width_info.type     = width_ptr->type;
        child_ptr->width_info.add_data = width_ptr->add_data;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control height
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default calculate height by width
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildHeight(
                                       MMI_HANDLE_T              form_handle,    //in:
                                       MMI_HANDLE_T              child_handle,   //in:
                                       GUIFORM_CHILD_HEIGHT_T    *height_ptr     //in:
                                       )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)))
    {
        //set height info
        child_ptr->height_info.type     = height_ptr->type;
        child_ptr->height_info.add_data = height_ptr->add_data;

        //set child is rect adaptive
        switch (height_ptr->type)
        {
        case GUIFORM_CHILD_HEIGHT_FIXED:
        case GUIFORM_CHILD_HEIGHT_PERCENT:
        case GUIFORM_CHILD_HEIGHT_LEFT:
            SetChildRectAdaptive(child_ptr);
            break;

        default:
            break;
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child is need rect adaptive
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildRectAdaptive(
                                   CTRLFORM_CHILD_T     *child_ptr
                                   )
{
    BOOLEAN         result = FALSE;
    CAF_GUID_T      guid = 0;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;

    if (PNULL != child_ptr)
    {
        //dock edit not change rect
        guid = IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr);
        switch (guid)
        {
        case SPRD_GUI_FORM_ID:
            form_ctrl_ptr = (CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr;

            //set form may slide
            form_ctrl_ptr->is_display_prg = TRUE;
            form_ctrl_ptr->theme.is_support_slide = TRUE;
            IGUICTRL_SetState((IGUICTRL_T *)form_ctrl_ptr,GUICTRL_STATE_DISABLE_TP,FALSE);
            break;

        case SPRD_GUI_EDITBOX_ID:
        case SPRD_GUI_TEXTEDIT_ID:
        case SPRD_GUI_PHONENUMEDIT_ID:
        case SPRD_GUI_DIGITALEDIT_ID:
        case SPRD_GUI_PSWEDIT_ID:
        case SPRD_GUI_LISTEDIT_ID:
            CTRLBASEEDIT_SetRectAdaptive(child_ptr->ctrl_handle,FALSE);
            result = TRUE;
            break;
    
        default:
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set child theme,include border,bg and font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildTheme(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      GUI_BG_T       *bg_ptr,        //in/out:may PNULL
                                      GUI_BORDER_T   *border_ptr,    //in/out:may PNULL
                                      GUI_FONT_ALL_T *font_ptr       //in/out:may PNULL
                                      )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set bg
        if (PNULL != bg_ptr)
        {
            form_ctrl_ptr->theme.child.bg = *bg_ptr;
        }
        
        //set border
        if (PNULL != border_ptr)
        {
            form_ctrl_ptr->theme.child.border = *border_ptr;
        }
        
        //set font
        if (PNULL != font_ptr)
        {
            form_ctrl_ptr->theme.child.font = *font_ptr;
        }
        
        //set child theme
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_SetChildTheme(child_ptr->ctrl_handle,bg_ptr,border_ptr,font_ptr);
            }
            child_ptr = child_ptr->next_ptr;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set active child theme,include border,bg and font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChildTheme(
                                            MMI_HANDLE_T     form_handle,    //in:
                                            GUI_BG_T         *bg_ptr,        //in/out:may PNULL
                                            GUI_BORDER_T     *border_ptr,    //in/out:may PNULL
                                            GUI_FONT_ALL_T   *font_ptr       //in/out:may PNULL
                                            )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set bg
        if (PNULL != bg_ptr)
        {
            form_ctrl_ptr->theme.active_child.bg = *bg_ptr;
        }
        
        //set border
        if (PNULL != border_ptr)
        {
            form_ctrl_ptr->theme.active_child.border = *border_ptr;
        }
        
        //set font
        if (PNULL != font_ptr)
        {
            form_ctrl_ptr->theme.active_child.font = *font_ptr;
        }
        
        //set child theme
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_SetActiveChildTheme(child_ptr->ctrl_handle,bg_ptr,border_ptr,font_ptr);
            }
            child_ptr = child_ptr->next_ptr;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set gray child theme,include border,bg and font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetGrayChildTheme(
                                          MMI_HANDLE_T       form_handle,    //in:
                                          GUI_BG_T           *bg_ptr,        //in/out:may PNULL
                                          GUI_BORDER_T       *border_ptr,    //in/out:may PNULL
                                          GUI_FONT_ALL_T     *font_ptr       //in/out:may PNULL
                                          )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        //set bg
        if (PNULL != bg_ptr)
        {
            form_ctrl_ptr->theme.gray_child.bg = *bg_ptr;
        }
        
        //set border
        if (PNULL != border_ptr)
        {
            form_ctrl_ptr->theme.gray_child.border = *border_ptr;
        }
        
        //set font
        if (PNULL != font_ptr)
        {
            form_ctrl_ptr->theme.gray_child.font = *font_ptr;
        }
        
        //set child theme
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_SetGrayChildTheme(child_ptr->ctrl_handle,bg_ptr,border_ptr,font_ptr);
            }
            child_ptr = child_ptr->next_ptr;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form display height min/max size
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSizeRange(
                                     MMI_HANDLE_T   form_handle,    //in:
                                     MMI_HANDLE_T   child_handle,   //in:
                                     uint16         min_height,     //in:
                                     uint16         max_height      //in:
                                     )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) &&
        (min_height <= max_height) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)))
    {
        //set child size range
        child_ptr->size_range.min_size = min_height;
        child_ptr->size_range.max_size = max_height;

        //is form child
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            form_ctrl_ptr = (CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr;

            //set form size range
            form_ctrl_ptr->size_range.min_size = min_height;
            form_ctrl_ptr->size_range.max_size = max_height;

            //set progress
            form_ctrl_ptr->is_display_prg = TRUE;

            //support slide
            form_ctrl_ptr->theme.is_support_slide = TRUE;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set form control align,only for sbs
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAlign(
                                 MMI_HANDLE_T           form_handle,    //in:
                                 GUIFORM_CHILD_ALIGN_E  align           //in:
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        //set align
        form_ctrl_ptr->align = align;
        
        result = TRUE;
    }
    
    return (result);
}
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/*****************************************************************************/
//  Description : set the align for all child,support in layout order only.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAllChildAlign(
	MMI_HANDLE_T           form_handle,    //in:
	GUIFORM_CHILD_ALIGN_E  child_align           //in:
)
{
	BOOLEAN         result = FALSE;
	CTRLFORM_OBJ_T* form_ctrl_ptr = PNULL;

	//get form pointer by control id
	form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

	//get child pointer by handle
	if (PNULL != form_ctrl_ptr)
	{
		//set align
		form_ctrl_ptr->child_align = child_align;

		result = TRUE;
	}

	return (result);
}

/***************************************************************************//*!
@brief get the align for all child,support in layout order only.
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_CHILD_ALIGN_E
@note 
*******************************************************************************/

PUBLIC GUIFORM_CHILD_ALIGN_E CTRLFORM_GetChildAlign(
                              MMI_HANDLE_T      ctrl_handle
                              )
{
	CTRLFORM_OBJ_T	*form_ctrl_ptr = PNULL;
	 
	//get form pointer by control id
	form_ctrl_ptr = CTRLFORM_GetPtr(ctrl_handle);
	if (PNULL != form_ctrl_ptr)
	{
		return form_ctrl_ptr->child_align;
	}
	return GUIFORM_CHILD_ALIGN_ERROR; //-1bug1941758

}

#endif

/*****************************************************************************/
//  Description : set child control align,only for order child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildAlign(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_ALIGN_E  align           //in:
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
        SCI_ASSERT(GUIFORM_LAYOUT_ORDER == form_ctrl_ptr->layout_type); /*assert verified*/
        
        //set align
        child_ptr->align = align;
        
        //is form child
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            CTRLFORM_SetAlign(form_handle,align);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetChildDisplay(
                                        MMI_HANDLE_T             form_handle,        //in:
                                        MMI_HANDLE_T             child_handle,       //in:
                                        GUIFORM_CHILD_DISPLAY_E  *display_type_ptr   //in/out:
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
        if (PNULL != display_type_ptr)
        {
            *display_type_ptr = child_ptr->display_type;
            
            result = TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplay(
                                        MMI_HANDLE_T             form_handle,
                                        MMI_HANDLE_T             child_handle,
                                        GUIFORM_CHILD_DISPLAY_E  display_type
                                        )
{
    BOOLEAN     result = FALSE;
    
    //set child display
    result = SetChildDisplay(TRUE,
        form_handle,
        child_handle,
        display_type);
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildDisplay(
                              BOOLEAN                   is_update,
                              MMI_HANDLE_T              form_handle,
                              MMI_HANDLE_T              child_handle,
                              GUIFORM_CHILD_DISPLAY_E   display_type
                              )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_layout = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;
    CTRLFORM_OBJ_T      *parent_form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    CTRLFORM_CHILD_T    *next_child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)) &&
        (child_ptr->display_type != display_type))
    {
        //is update
        if ((is_update) &&
            (!MMK_IsFocusWin(form_ctrl_ptr->win_handle)))
        {
            is_update = FALSE;
        }
        
        //is need layout
        if (((GUIFORM_CHILD_DISP_HIDE == child_ptr->display_type) && (GUIFORM_CHILD_DISP_HIDE != display_type)) ||
            ((GUIFORM_CHILD_DISP_HIDE != child_ptr->display_type) && (GUIFORM_CHILD_DISP_HIDE == display_type)))
        {
            is_layout = TRUE;
        }
        
        //set child display state
        child_ptr->display_type = display_type;
        
        //is softkey or dock child
        if ((child_ptr == form_ctrl_ptr->softkey_child_ptr) ||
            (CTRLFORM_IsDockChild(child_ptr)))
        {
            //modify form relative display rect
            CTRLFORM_InitDisplayRect(form_ctrl_ptr);
        }
        
        //is layout
        if ((is_layout) && 
            (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num))
        {
            //get valid child index
            CTRLFORM_GetValidChild(form_ctrl_ptr);
            
            //parent has active child,modify active child
            if ((PNULL == form_ctrl_ptr->active_child_ptr) &&
                (CTRLFORM_IsGetAtvChild(child_ptr)) &&
                (!IsParentHasActiveChild(form_handle)))
            {
                //set active child
                IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,FALSE);
                MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,child_ptr->ctrl_handle);
            }
            else if (child_ptr == form_ctrl_ptr->active_child_ptr)
            {
                //get next get active child pointer
                if (GetNextActiveChildPtr(form_ctrl_ptr,child_ptr,&next_child_ptr))
                {
                    MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,next_child_ptr->ctrl_handle);
                }
                else
                {
                    MMK_SetAtvCtrl(form_ctrl_ptr->win_handle,base_ctrl_ptr->handle);
                }
            }
            
            //reset current child
            if (PNULL != form_ctrl_ptr->active_child_ptr)
            {
                form_ctrl_ptr->cur_child_ptr = form_ctrl_ptr->active_child_ptr;
            }
            
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                SetAllChildDisplay(CTRLFORM_GetPtr(child_ptr->ctrl_handle),display_type);
            }
            
            //获得最外层的Form指针
            if (GetParentFormCtrl(form_ctrl_ptr,&parent_form_ctrl_ptr))
            {
                form_ctrl_ptr = parent_form_ctrl_ptr;
            }
            
            //need layout
            SetFormLayout(form_ctrl_ptr);
            
            if (is_update)
            {
                //update display
                CTRLFORM_Display(FALSE,FALSE,form_ctrl_ptr);
                
                //layout
                CTRLFORM_Layout(form_ctrl_ptr,TRUE,TRUE);
                
                //display scroll bar
                DisplayScrollBar(TRUE,form_ctrl_ptr);
            }
        }
        else
        {
            //reset current child
            if (PNULL != form_ctrl_ptr->active_child_ptr)
            {
                form_ctrl_ptr->cur_child_ptr = form_ctrl_ptr->active_child_ptr;
            }
            
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                SetAllChildDisplay(CTRLFORM_GetPtr(child_ptr->ctrl_handle),display_type);
            }
            
            //set child param
            CTRLFORM_SetChildParam(form_ctrl_ptr,child_ptr);
            
            //获得最外层的Form指针
            if ((is_layout) && 
                (GetParentFormCtrl(form_ctrl_ptr,&parent_form_ctrl_ptr)))
            {
                //need layout
                SetFormLayout(form_ctrl_ptr);

                if (is_update)
                {
                    //update display
                    CTRLFORM_Display(FALSE,FALSE,parent_form_ctrl_ptr);
                
                    //layout
                    CTRLFORM_Layout(parent_form_ctrl_ptr,TRUE,TRUE);
                
                    //display scroll bar
                    DisplayScrollBar(TRUE,parent_form_ctrl_ptr);
                }
            }
            else
            {
                if (is_update)
                {
                    //update
                    IGUICTRL_HandleEvent(child_ptr->child_ctrl_ptr,MSG_CTL_PAINT,PNULL);
                }
            }
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAllChildDisplay(
                              CTRLFORM_OBJ_T            *form_ctrl_ptr,
                              GUIFORM_CHILD_DISPLAY_E   display_type
                              )
{
    if (PNULL != form_ctrl_ptr)
    {	
		CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
		CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
		//set first child
		child_ptr = form_ctrl_ptr->first_child_ptr;
    
		//parse child control
		while (PNULL != child_ptr)
		{
			//set child display
			SetChildDisplay(FALSE,
				base_ctrl_ptr->handle,
				child_ptr->ctrl_handle,
				display_type);
        
			child_ptr = child_ptr->next_ptr;
		}
    }
}

/*****************************************************************************/
//  Description : get order form parent form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 获得最外层的Form
/*****************************************************************************/
LOCAL BOOLEAN GetParentFormCtrl(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr,     //in:
                                CTRLFORM_OBJ_T  **parent_form_pptr  //in/out:
                                )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T    parent_handle = 0;
    IGUICTRL_T      *ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //get parent handle
    parent_handle = MMK_GetParentCtrlHandle(base_ctrl_ptr->handle);
    while (0 != parent_handle)
    {
        ctrl_ptr = MMK_GetCtrlPtr(parent_handle);
        if (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
            //set parent
            if (PNULL != parent_form_pptr)
            {
                *parent_form_pptr = (CTRLFORM_OBJ_T *)ctrl_ptr;
            }
            
            //get parent handle
            parent_handle = MMK_GetParentCtrlHandle(parent_handle);
            
            result = TRUE;
        }
        else
        {
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get parent control has active child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsParentHasActiveChild(
                                     MMI_HANDLE_T   form_handle
                                     )
{
    BOOLEAN         result = FALSE;
    IGUICTRL_T      *ctrl_ptr = PNULL;
    MMI_HANDLE_T    parent_handle = 0;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get parent handle
    parent_handle = MMK_GetParentCtrlHandle(form_handle);
    while (0 != parent_handle)
    {
        ctrl_ptr = MMK_GetCtrlPtr(parent_handle);
        if (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
            form_ctrl_ptr = (CTRLFORM_OBJ_T *)ctrl_ptr;
            if (PNULL != form_ctrl_ptr->active_child_ptr)
            {
                //当前form为Active
                if (MMK_ConvertIdToHandle(form_handle) == MMK_ConvertIdToHandle(form_ctrl_ptr->active_child_ptr->ctrl_handle))
                {
                    result = FALSE;
                }
                else
                {
                    result = TRUE;
                }
                break;
            }
            else
            {
                //get parent handle
                parent_handle = MMK_GetParentCtrlHandle(parent_handle);
            }
        }
        else
        {
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get next active child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextActiveChildPtr(
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                                    CTRLFORM_CHILD_T    *cur_child_ptr, //in:
                                    CTRLFORM_CHILD_T    **out_child_pptr//in/out:
                                    )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *out_child_ptr = PNULL;
    
    if (PNULL != out_child_pptr)
    {
        //get next get active child
        if (cur_child_ptr->index < form_ctrl_ptr->last_valid_index)
        {
            out_child_ptr = cur_child_ptr->next_ptr;
            while (PNULL != out_child_ptr)
            {
                if (CTRLFORM_IsGetAtvChild(out_child_ptr))
                {
                    result = TRUE;
                    break;
                }
                
                //next child
                out_child_ptr = out_child_ptr->next_ptr;
            }
        }
        
        if (!result)
        {
            //get previous valid child
            if (cur_child_ptr->index > form_ctrl_ptr->first_valid_index)
            {
                out_child_ptr = cur_child_ptr->prev_ptr;
                while (PNULL != out_child_ptr && out_child_ptr != cur_child_ptr)
                {
                    if (CTRLFORM_IsGetAtvChild(out_child_ptr))
                    {
                        result = TRUE;
                        break;
                    }
                    
                    if (out_child_ptr->index > form_ctrl_ptr->first_valid_index)
                    {
                        //previous child
                        out_child_ptr = out_child_ptr->prev_ptr;
                    }
                    else
                    {
                        break;
                    }
                }
                
                if (out_child_ptr == cur_child_ptr)
                {
                    out_child_ptr = PNULL;
                }
            }
        }
        
        //set previous or next valid child
        *out_child_pptr = out_child_ptr;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form need layout
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormLayout(
                         CTRLFORM_OBJ_T     *form_ctrl_ptr
                         )
{
#ifdef GUIF_FORM_DOCKABLE
    uint16              cur_priority = 0;
#endif
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (PNULL != form_ctrl_ptr)
    {
        //need layout
        form_ctrl_ptr->layout_index = 0;

#ifdef GUIF_FORM_DOCKABLE
        //find dock child
        cur_priority = 1;
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
        while (PNULL != child_ptr)
        {
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                SetFormLayout((CTRLFORM_OBJ_T*)child_ptr->child_ctrl_ptr);
            }

            //get next dock child
            child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
        }
#endif

        //other child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                SetFormLayout((CTRLFORM_OBJ_T*)child_ptr->child_ctrl_ptr);
            }

            child_ptr = child_ptr->next_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : only set child control display type,not layout,not update
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 在form未排版之前设置,可以提高速度,比如open,full paint msg设置
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplayOnly(
                                            MMI_HANDLE_T            form_handle,
                                            MMI_HANDLE_T            child_handle,
                                            GUIFORM_CHILD_DISPLAY_E display_type
                                            )
{
    BOOLEAN     result = FALSE;
    
    //only set child display
    result = SetChildDisplayOnly(FALSE,
        form_handle,
        child_handle,
        display_type);
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildDisplayOnly(
                                  BOOLEAN                   is_nested,
                                  MMI_HANDLE_T              form_handle,
                                  MMI_HANDLE_T              child_handle,
                                  GUIFORM_CHILD_DISPLAY_E   display_type
                                  )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_layout = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_OBJ_T      *parent_form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if ((PNULL != form_ctrl_ptr) && 
        (CTRLFORM_GetChildPtrByHandle(child_handle,form_ctrl_ptr,&child_ptr)) &&
        (child_ptr->display_type != display_type))
    {
        //is need layout
        if (((GUIFORM_CHILD_DISP_HIDE == child_ptr->display_type) && (GUIFORM_CHILD_DISP_HIDE != display_type)) ||
            ((GUIFORM_CHILD_DISP_HIDE != child_ptr->display_type) && (GUIFORM_CHILD_DISP_HIDE == display_type)))
        {
            is_layout = TRUE;
        }
        
        //set child display state
        child_ptr->display_type = display_type;
        
        //is layout,获得最外层的Form指针
        if ((is_layout) && 
            (!is_nested))
        {
            //获得最外层的Form指针
            if (GetParentFormCtrl(form_ctrl_ptr,&parent_form_ctrl_ptr))
            {
                form_ctrl_ptr = parent_form_ctrl_ptr;
            }

            //need layout
            SetFormLayout(form_ctrl_ptr);
        }
        
        //is form child
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            SetAllChildDisplayOnly(CTRLFORM_GetPtr(child_ptr->ctrl_handle),display_type);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : only set child control display type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAllChildDisplayOnly(
                                  CTRLFORM_OBJ_T            *form_ctrl_ptr,
                                  GUIFORM_CHILD_DISPLAY_E   display_type
                                  )
{
    if (PNULL != form_ctrl_ptr)
    {	
		CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
		CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
		//set first child
		child_ptr = form_ctrl_ptr->first_child_ptr;
    
		//parse child control
		while (PNULL != child_ptr)
		{
			//set child display
			SetChildDisplayOnly(TRUE,
				base_ctrl_ptr->handle,
				child_ptr->ctrl_handle,
				display_type);
        
			child_ptr = child_ptr->next_ptr;
		}
    }
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : set child control dock type
//  Global resource dependence : 
//  Author: Jassmine
//  Note: only set one control dock type
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDock(
                                     MMI_HANDLE_T           form_handle,
                                     MMI_HANDLE_T           child_handle,
                                     GUIFORM_CHILD_DOCK_E   dock_type
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
        switch (dock_type)
        {
        case GUIFORM_CHILD_DOCK_TOP:
            if (PNULL == form_ctrl_ptr->top_child_ptr)
            {
                //set top child
                form_ctrl_ptr->top_child_ptr = child_ptr;
                result = TRUE;
            }
            break;

        case GUIFORM_CHILD_DOCK_BOTTOM:
            if (PNULL == form_ctrl_ptr->bottom_child_ptr)
            {
                //set bottom child
                form_ctrl_ptr->bottom_child_ptr = child_ptr;
                result = TRUE;
            }
            break;

        case GUIFORM_CHILD_DOCK_LEFT:
            if (PNULL == form_ctrl_ptr->left_child_ptr)
            {
                //set left child
                form_ctrl_ptr->left_child_ptr = child_ptr;
                result = TRUE;
            }
            break;

        case GUIFORM_CHILD_DOCK_RIGHT:
            if (PNULL == form_ctrl_ptr->right_child_ptr)
            {
                //set right child
                form_ctrl_ptr->right_child_ptr = child_ptr;
                result = TRUE;
            }
            break;

        default:
            break;
        }

        if (result)
        {
            //set form current priority
            form_ctrl_ptr->cur_priority++;

            //set child dock
            child_ptr->dock_type     = dock_type;
            child_ptr->dock_priority = form_ctrl_ptr->cur_priority;

            //remove child form list
            CTRLFORM_RemoveChildFromList(form_ctrl_ptr,child_ptr);

            //set form child number
            form_ctrl_ptr->all_child_num++;

            //need layout
            form_ctrl_ptr->layout_index = 0;

            //dock edit not change rect
            SetChildRectAdaptive(child_ptr);
        }
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : set form all child is permit form set it's bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBg(
                                      MMI_HANDLE_T   form_handle,
                                      BOOLEAN        is_permit
                                      )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->is_permit_bg = is_permit;
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_PermitChildBg(child_ptr->ctrl_handle,is_permit);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form all child is permit form set it's border
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBorder(
                                          MMI_HANDLE_T   form_handle,
                                          BOOLEAN        is_permit
                                          )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->is_permit_border = is_permit;
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_PermitChildBorder(child_ptr->ctrl_handle,is_permit);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form all child is permit form set it's font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFontColor(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->is_permit_font_color = is_permit;
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_PermitChildFontColor(child_ptr->ctrl_handle,is_permit);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        result = TRUE;
    }
    
    return (result);


}

/*****************************************************************************/
//  Description : set form all child is permit form set it's font size
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFont(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->is_permit_font = is_permit;
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_PermitChildFont(child_ptr->ctrl_handle,is_permit);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is support slide,default TRUE
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSlide(
                                MMI_HANDLE_T   form_handle,
                                BOOLEAN        is_support
                                )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->theme.is_support_slide = is_support;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 是否根据语言自动调整child显示的相对位置
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 一般从左至右显示，阿拉伯等语言则从右往左显示
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAutoDir(
                                  MMI_HANDLE_T   form_handle,
                                  BOOLEAN        is_auto
                                  )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->is_auto_dir = is_auto;
        
        //set first child
        child_ptr = form_ctrl_ptr->first_child_ptr;
        
        //parse child control
        while (PNULL != child_ptr)
        {
            //is form child
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                CTRLFORM_IsAutoDir(child_ptr->ctrl_handle,is_auto);
            }
            
            child_ptr = child_ptr->next_ptr;
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is circular handle up/down key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleUpDown(
                                                MMI_HANDLE_T    form_handle,
                                                BOOLEAN         is_circular
                                                )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->theme.is_cir_handle_ud = is_circular;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is circular handle left/right key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleLeftRight(
                                                   MMI_HANDLE_T  form_handle,
                                                   BOOLEAN       is_circular
                                                   )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        form_ctrl_ptr->theme.is_cir_handle_lr = is_circular;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get form height by handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
// 获取form中所有内容的高度
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetHeight(
                                 MMI_HANDLE_T    form_handle //in:
                                 )
{
    uint16          height = 0;
    uint16          child_height = 0;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    if (PNULL != form_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;

        //is layout
        CTRLFORM_Layout(form_ctrl_ptr,TRUE,FALSE);
        
        height = (uint16)(form_ctrl_ptr->relative_rect.bottom + 1);

#ifdef GUIF_FORM_DOCKABLE
        //add bottom child
        child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->bottom_child_ptr);
        height = (uint16)(height + child_height);
#else
        GUI_INVALID_PARAM(child_height);
#endif

        //add softkey
        if (CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr))
        {
            height = (uint16)(height + form_ctrl_ptr->softkey_child_ptr->height);
        }

        //add border
        if (GUI_BORDER_NONE != base_ctrl_ptr->border.type)
        {
            height = (uint16)(height + (base_ctrl_ptr->border.width<<1));
        }

        //add padding
        height = (uint16)(height + base_ctrl_ptr->padding.top);
        height = (uint16)(height + base_ctrl_ptr->padding.bottom);
    }
    
    return (height);
}

/*****************************************************************************/
//  Description : force layout
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ForceLayout(
                                    MMI_HANDLE_T    form_handle //in:
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;

    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        //set form layout
        SetFormLayout(form_ctrl_ptr);

        //layout
        CTRLFORM_Layout(form_ctrl_ptr,TRUE,FALSE);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set form rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const GUI_RECT_T  *rect_ptr
                              )
{
    BOOLEAN             result = TRUE;
    GUI_RECT_T          old_rect = {0};
    GUI_RECT_T          prg_rect = {0};
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    if ((!GUI_IsRectEmpty(*rect_ptr)) && 
        (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr)))
    {
        //set old rect
        old_rect = base_ctrl_ptr->rect;
        
        //set rect
        base_ctrl_ptr->rect          = *rect_ptr;
        base_ctrl_ptr->display_rect  = *rect_ptr;
        form_ctrl_ptr->original_rect = *rect_ptr;
        
        //free backup bg
        if (PNULL != form_ctrl_ptr->bg_backup.buf_ptr)
        {
            SCI_FREE(form_ctrl_ptr->bg_backup.buf_ptr);
        }
        
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
        
        //reset slide state
        form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
        VTLBASE_SetSlideState((CTRLBASE_OBJ_T *)form_ctrl_ptr,FALSE);
#endif
        
        GUI_SetVOrHRect(base_ctrl_ptr->handle,(GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));
        
        //init display rect
        CTRLFORM_InitDisplayRect(form_ctrl_ptr);
        
        //set prg rect
        if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
        {
            prg_rect = CTRLFORM_GetScrollBarRect(form_ctrl_ptr);
            IGUICTRL_SetRect((IGUICTRL_T *)form_ctrl_ptr->prgbox_ctrl_ptr,&prg_rect);
        }

        //is layout
        if (!GUI_IsRectSizeEqual(old_rect,base_ctrl_ptr->rect))
        {
            //need layout
            form_ctrl_ptr->layout_index = 0;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get form rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN FormCtrlGetRect(
                               CTRLBASE_OBJ_T   *base_ctrl_ptr,
                               GUI_RECT_T       *rect_ptr
                               )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;

    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;

    if ((PNULL != form_ctrl_ptr) && 
        (PNULL != rect_ptr))
    {
        *rect_ptr = base_ctrl_ptr->rect;

        //win manage softkey focus
        if (PNULL != form_ctrl_ptr->softkey_child_ptr)
        {
            rect_ptr->bottom = (int16)(rect_ptr->bottom - form_ctrl_ptr->softkey_child_ptr->height);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set form lcd device
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetLcdDevInfo(
                                    CTRLBASE_OBJ_T          *base_ctrl_ptr,
                                    const GUI_LCD_DEV_INFO  *lcd_dev_ptr
                                    )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    if ((PNULL != base_ctrl_ptr) &&
        (PNULL != lcd_dev_ptr))
    {
        form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
        
        //set lcd device
        base_ctrl_ptr->lcd_dev_info = *lcd_dev_ptr;
        
        //set child lcd device
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            IGUICTRL_SetLcdDevInfo(child_ptr->child_ctrl_ptr,lcd_dev_ptr);
            child_ptr = child_ptr->next_ptr;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set form padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetPadding(
                                 CTRLBASE_OBJ_T *base_ctrl_ptr,
                                 GUI_PADDING_T  *padding_ptr
                                 )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    base_ctrl_ptr->padding = *padding_ptr;
        
    //is layout
    if (form_ctrl_ptr->layout_index == form_ctrl_ptr->all_child_num)
    {
        //need layout
        form_ctrl_ptr->layout_index = 0;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetBg(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    //set bg
    base_ctrl_ptr->bg = *bg_ptr;
    
    //free backup bg
    if (PNULL != form_ctrl_ptr->bg_backup.buf_ptr)
    {
        SCI_FREE(form_ctrl_ptr->bg_backup.buf_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form is display progress
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetProgress(
                                  CTRLBASE_OBJ_T    *base_ctrl_ptr,
                                  BOOLEAN           is_display
                                  )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    //set is display progress
    form_ctrl_ptr->is_display_prg = is_display;
    
    return (result);
}

/*****************************************************************************/
//  Description : get form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetDisplayRect(
                                     CTRLBASE_OBJ_T     *base_ctrl_ptr, //in:
                                     GUI_RECT_T         *rect_ptr       //in/out:
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    if (PNULL != rect_ptr)
    {
        *rect_ptr = base_ctrl_ptr->display_rect;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlSetDisplayRect(
                                     CTRLBASE_OBJ_T     *base_ctrl_ptr, //in:
                                     const GUI_RECT_T   *rect_ptr,      //in:
                                     BOOLEAN            is_update       //in:
                                     )
{
    BOOLEAN         result = TRUE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    //set display rect
    base_ctrl_ptr->display_rect = *rect_ptr;
    
    if (is_update)
    {
        //update unit form
        CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
    }
    
    //set all child rect
    CTRLFORM_SetChildRect(is_update,form_ctrl_ptr);
    
    return (result);
}

/*****************************************************************************/
//  Description : get form active child select rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetSelectRect(
                                    CTRLBASE_OBJ_T  *base_ctrl_ptr, //in:
                                    GUI_RECT_T      *rect_ptr       //in/out:
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    //get active child
    child_ptr = form_ctrl_ptr->active_child_ptr;
    if (PNULL != child_ptr)
    {
        result = IGUICTRL_GetSelectRect(child_ptr->child_ctrl_ptr,rect_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get form acitve child index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FormCtrlGetSelectIndex(
                                     CTRLBASE_OBJ_T *base_ctrl_ptr, //in:
                                     BOOLEAN        *is_first_ptr,  //in/out:
                                     BOOLEAN        *is_last_ptr    //in/out:
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T *child_ptr = PNULL;
    CTRLFORM_CHILD_T *first_child_ptr = PNULL;
    CTRLFORM_CHILD_T *last_child_ptr = PNULL;
    
    form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
    //get active child
    child_ptr = form_ctrl_ptr->active_child_ptr;
    if (PNULL != child_ptr)
    {
        result = IGUICTRL_GetSelectIndex(child_ptr->child_ctrl_ptr,is_first_ptr,is_last_ptr);
        
        //is first child
        if ((*is_first_ptr) &&
            (CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->first_valid_index,form_ctrl_ptr,&first_child_ptr)) &&
            (first_child_ptr == child_ptr))
        {
            *is_first_ptr = TRUE;
        }
        else
        {
            *is_first_ptr = FALSE;
        }
        
        //is last child
        if ((*is_last_ptr) &&
            (CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->last_valid_index,form_ctrl_ptr,&last_child_ptr)) &&
            (last_child_ptr == child_ptr))
        {
            *is_last_ptr = TRUE;
        }
        else
        {
            *is_last_ptr = FALSE;
        }
    }
    
    return (result);
}

#ifdef GUI_DUMP_CTRL_TREE
/*****************************************************************************/
//  Description : dump control tree by form ptr
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DumpTree(
                              MMI_HANDLE_T  form_handle
                              )
{
    CTRLFORM_OBJ_T  *form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);

    if (PNULL != form_ctrl_ptr)
    {
        SCI_TRACE_LOW("==============   Dump form control basic tree   ==============");
        DumpFormMainInfo(form_ctrl_ptr, 0);
        SCI_TRACE_LOW("==============================================================\n\n");

        SCI_TRACE_LOW("==============   Dump form control tree   ==============");
        DumpFormInfo(form_ctrl_ptr, 0);
        SCI_TRACE_LOW("==============================================================\n\n");
    }
}

/*****************************************************************************/
//  Description : dump child basic info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpFormMainInfo(
                            CTRLFORM_OBJ_T      *form_ctrl_ptr,
                            uint16              deep_level
                            )
{
    char                space[CTRLFORM_DUMP_MAX_INDENT*CTRLFORM_DUMP_INDENT_STEP + 1] = {0};
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (deep_level > CTRLFORM_DUMP_MAX_INDENT)
    {
        deep_level = CTRLFORM_DUMP_MAX_INDENT;
    }

    SCI_MEMSET(space, 0x20, deep_level*CTRLFORM_DUMP_INDENT_STEP);

#ifdef GUIF_FORM_DOCKABLE
    child_ptr = form_ctrl_ptr->top_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%s top index=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }
#endif

    child_ptr = form_ctrl_ptr->first_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%s index=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }

#ifdef GUIF_FORM_DOCKABLE
    child_ptr = form_ctrl_ptr->bottom_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%s bottom index=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }

    child_ptr = form_ctrl_ptr->left_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%s left index=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }

    child_ptr = form_ctrl_ptr->right_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%s right index=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }
#endif

    child_ptr = form_ctrl_ptr->softkey_child_ptr;
    while (PNULL != child_ptr)
    {
        SCI_TRACE_LOW("%sindex=%d, id=0x%08x, guid=%s",
                space,
                child_ptr->index,
                child_ptr->ctrl_handle,
                MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)));

        if (CTRLFORM_IsFormChild(child_ptr))
        {
            DumpFormMainInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, (uint16)(deep_level + 1));
        }

        child_ptr = child_ptr->next_ptr;
    }
}

/*****************************************************************************/
//  Description : dump form info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpFormInfo(
                        CTRLFORM_OBJ_T      *form_ctrl_ptr,
                        uint16              deep_level
                        )
{
    char                space[CTRLFORM_DUMP_MAX_INDENT*CTRLFORM_DUMP_INDENT_STEP + 1] = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (deep_level > CTRLFORM_DUMP_MAX_INDENT)
    {
        deep_level = CTRLFORM_DUMP_MAX_INDENT;
    }

    SCI_MEMSET(space, 0x20, deep_level*CTRLFORM_DUMP_INDENT_STEP);

    SCI_TRACE_LOW("%s== form == form_ctrl_ptr=0x%08x, rect={%d,%d,%d,%d}, dis_rect={%d,%d,%d,%d}, num=%d, active=0x%08x, cur=0x%08x, rel_rect={%d,%d,%d,%d}, rel_dis_rect={%d,%d,%d,%d}",
            space,
            form_ctrl_ptr,
            base_ctrl_ptr->rect.left,
            base_ctrl_ptr->rect.top,
            base_ctrl_ptr->rect.right,
            base_ctrl_ptr->rect.bottom,
            base_ctrl_ptr->display_rect.left,
            base_ctrl_ptr->display_rect.top,
            base_ctrl_ptr->display_rect.right,
            base_ctrl_ptr->display_rect.bottom,
            form_ctrl_ptr->all_child_num,
            form_ctrl_ptr->active_child_ptr,
            form_ctrl_ptr->cur_child_ptr,
            form_ctrl_ptr->relative_rect.left,
            form_ctrl_ptr->relative_rect.top,
            form_ctrl_ptr->relative_rect.right,
            form_ctrl_ptr->relative_rect.bottom,
            form_ctrl_ptr->relative_disp_rect.left,
            form_ctrl_ptr->relative_disp_rect.top,
            form_ctrl_ptr->relative_disp_rect.right,
            form_ctrl_ptr->relative_disp_rect.bottom);

#ifdef GUIF_FORM_DOCKABLE
    child_ptr = form_ctrl_ptr->top_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }
#endif

    child_ptr = form_ctrl_ptr->first_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }

#ifdef GUIF_FORM_DOCKABLE
    child_ptr = form_ctrl_ptr->bottom_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }

    child_ptr = form_ctrl_ptr->left_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }

    child_ptr = form_ctrl_ptr->right_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }
#endif

    child_ptr = form_ctrl_ptr->softkey_child_ptr;
    while (PNULL != child_ptr)
    {
        DumpChildInfo(child_ptr, (uint16)(deep_level + 1));

        child_ptr = child_ptr->next_ptr;
    }
}

/*****************************************************************************/
//  Description : dump child info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DumpChildInfo(
                         CTRLFORM_CHILD_T   *child_ptr,
                         uint16             deep_level
                         )
{
    char    space[CTRLFORM_DUMP_MAX_INDENT*CTRLFORM_DUMP_INDENT_STEP + 1] = {0};
    GUI_RECT_T  rect = {0};

    if (deep_level > CTRLFORM_DUMP_MAX_INDENT)
    {
        deep_level = CTRLFORM_DUMP_MAX_INDENT;
    }

    SCI_MEMSET(space, 0x20, deep_level*CTRLFORM_DUMP_INDENT_STEP);

    IGUICTRL_GetRect(child_ptr->child_ctrl_ptr,&rect);

    SCI_TRACE_LOW("%schild_ptr=0x%08x, index=%d, ctrl_ptr=0x%08x, id=0x%08x, guid=%s, rect={%d,%d,%d,%d}, rel_rect={%d,%d,%d,%d}, width_info={%d,%d}, height_info={%d,%d}",
            space,
            child_ptr,
            child_ptr->index,
            child_ptr->child_ctrl_ptr,
            child_ptr->ctrl_handle,
            MMITHEME_GetCtrlName(IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)),
            rect.left, rect.top, rect.right, rect.bottom,
            child_ptr->relative_rect.left,
            child_ptr->relative_rect.top,
            child_ptr->relative_rect.right,
            child_ptr->relative_rect.bottom,
            child_ptr->width_info.type,
            child_ptr->width_info.add_data,
            child_ptr->height_info.type,
            child_ptr->height_info.add_data);

    if (CTRLFORM_IsFormChild(child_ptr))
    {
        DumpFormInfo((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr, deep_level);
    }
}
#endif

//zmt add start
/*****************************************************************************/
//  Description : set form no display scrollbar
//  Global resource dependence : 
//  Author: fys
//  Note:2024.06.29
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDisplayScrollBar(
                                MMI_HANDLE_T form_handle, 
                                BOOLEAN is_display
                                )
{
    BOOLEAN result = FALSE;
    CTRLFORM_OBJ_T *form_ctrl_ptr = PNULL;
    
    //get form pointer by control id
    form_ctrl_ptr = CTRLFORM_GetPtr(form_handle);
    
    //get child pointer by handle
    if (PNULL != form_ctrl_ptr)
    {
        //set is display progress
        form_ctrl_ptr->is_display_prg = is_display;
        result = TRUE;
    }

    return result;
}
//zmt add end