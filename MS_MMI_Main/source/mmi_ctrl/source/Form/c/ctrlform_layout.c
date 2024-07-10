/*****************************************************************************
** File Name:      ctrlform_layout.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
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
#include "ctrlform.h"
#include "guilabel.h"
#include "guisetlist.h"
#include "mmi_theme.h"
#include "mmk_handle_internal.h"

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
//  Description : init form relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitFormRelativeRect(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in/out:
                                );

/*****************************************************************************/
//  Description : adjust form relative rect by child after dock
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustFormRelativeRect(
                                  CTRLFORM_CHILD_T  *child_ptr,     //in:
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                  );

/*****************************************************************************/
//  Description : set form relative rect by child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormRelativeRect(
                               CTRLFORM_CHILD_T *child_ptr,     //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               );

/*****************************************************************************/
//  Description : end form relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void EndFormRelativeRect(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               );

/*****************************************************************************/
//  Description : modify left or right position by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPosByAlign(
                            BOOLEAN                 is_left,        //in:
                            int32                   *pos_ptr,       //in/out:
                            uint16                  child_width,    //in:
                            CTRLFORM_OBJ_T          *form_ctrl_ptr, //in:
                            GUIFORM_CHILD_ALIGN_E   align           //in:
                            );

/*****************************************************************************/
//  Description : layout all child control,relative rect,set background,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LayoutAllChildCtrl(//is create prg
                                 CTRLFORM_OBJ_T *form_ctrl_ptr  //in/out:
                                 );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : layout dock child control,relative rect,set background,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LayoutDockChildCtrl(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               );
#endif

/*****************************************************************************/
//  Description : get the layout of start child pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetLayoutStartChildPtr(
                                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                                               );

/*****************************************************************************/
//  Description : set child form param,include bg,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormChildParam(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T    *child_ptr      //in:
                             );

/*****************************************************************************/
//  Description : set child other param,include bg,font,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetOtherChildParam(
                              CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                              CTRLFORM_CHILD_T  *child_ptr      //in:
                              );

/*****************************************************************************/
//  Description : get child bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BG_T GetChildBg(
                          BOOLEAN           is_form_child,
                          CTRLFORM_CHILD_T  *child_ptr,
                          CTRLFORM_OBJ_T    *form_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : is display bg and border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayBgBorder(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get child border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BORDER_T GetChildBorder(
                                  BOOLEAN           is_form_child,
                                  BOOLEAN           is_unit_form,
                                  CTRLFORM_CHILD_T  *child_ptr,
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : get child font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_FONT_ALL_T GetChildFont(
                                  CTRLFORM_CHILD_T  *child_ptr,
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : get child relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetChildRelativeRect(
                                CTRLFORM_CHILD_T    *child_ptr,     //in/out:
                                CTRLFORM_OBJ_T      *form_ctrl_ptr  //in/out:
                                );

/*****************************************************************************/
//  Description : get child control relative top coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetChildRelativeTop(
                                uint16              child_height,   //in:
                                CTRLFORM_CHILD_T    *child_ptr,     //in:
                                CTRLFORM_OBJ_T      *form_ctrl_ptr  //in:
                                );

/*****************************************************************************/
//  Description : get child control relative left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetChildRelativeLeft(
                                 uint16             child_width,    //in:
                                 CTRLFORM_CHILD_T   *child_ptr,     //in:
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : get child control width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetChildWidthHeight(
                               uint16           *width_ptr,     //in/out:
                               uint16           *height_ptr,    //in/out:
                               CTRLFORM_CHILD_T *child_ptr,     //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               );

/*****************************************************************************/
//  Description : adjust child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustChildHeight(
                             uint16             *height_ptr,    //in/out:
                             CTRLFORM_CHILD_T   *child_ptr,     //in:
                             CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                             );

/*****************************************************************************/
//  Description : destroy progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyPrgCtrl(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : init form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitFormDisplayRect(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               );

/*****************************************************************************/
//  Description : adjust form display rect,include dock and other child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustAllDisplayRect(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : adjust dock child form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustDockDisplayRect(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 );
#endif

/*****************************************************************************/
//  Description : adjust form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustDisplayRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                             );

/*****************************************************************************/
//  Description : set all child init rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetChildInitRect(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                            );

/*****************************************************************************/
//  Description : set softkey child rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetSoftkeyChildRect(
                               BOOLEAN          is_update,      //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : set dock child rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetDockChildRect(
                            BOOLEAN             is_update,          //in:
                            BOOLEAN             is_set_disp_rect,   //in:
                            CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                            CTRLFORM_CHILD_T    *child_ptr          //in:
                            );
#endif

/*****************************************************************************/
//  Description : set child rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetChildRect(
                        BOOLEAN             is_update,      //in:
                        CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                        CTRLFORM_CHILD_T    *child_ptr      //in:
                        );

/*****************************************************************************/
//  Description : is update child when write back buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUpdateChildCtrl(
                                BOOLEAN         is_update,      //in:
                                GUI_RECT_T      *rect_ptr,      //in:
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                );

/*****************************************************************************/
//  Description : get cross rect by two rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCrossRect(
                           GUI_BIG_RECT_T   *rect_dest_ptr,
                           GUI_BIG_RECT_T   rect1,
                           GUI_BIG_RECT_T   rect2
                           );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : set child tp rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildTpRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T   *child_ptr      //in:
                             );
#endif

/*****************************************************************************/
//  Description : set child no display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetChildNotDisplay(
                              CTRLFORM_CHILD_T   *child_ptr  //in:
                              );

/*****************************************************************************/
//  Description : layout child form
//  Global resource dependence : 
//  Author: Jassmine
//  Note: when child form height is fixed/percent/left,no layout form
//        need set form rect first, than layout, for edit line change
/*****************************************************************************/
LOCAL void LayoutChildForm(
                           CTRLFORM_OBJ_T *form_ctrl_ptr  //in
                           );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : layout all child control, set param and set display rect etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_Layout(//is re-layout
                               CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                               BOOLEAN           is_first_layout,//in:
                               BOOLEAN           is_update       //in:
                               )
{
    BOOLEAN         result = FALSE;
    
    if (form_ctrl_ptr->layout_index < form_ctrl_ptr->all_child_num)
    {
        if (is_first_layout)
        {
            //get child index
            CTRLFORM_GetValidChild(form_ctrl_ptr);
        }
        
        //destroy progress
        if (DestroyPrgCtrl(form_ctrl_ptr))
        {
            form_ctrl_ptr->layout_index = 0;
        }
        
        //layout all child control
        result = LayoutAllChildCtrl(form_ctrl_ptr);
        
        if (is_first_layout)
        {
            //adjust display rect
            AdjustAllDisplayRect(form_ctrl_ptr);
        }
        
        //set child control rect
        CTRLFORM_SetChildRect(is_update,form_ctrl_ptr);
        
        form_ctrl_ptr->layout_index = form_ctrl_ptr->all_child_num;

        //layout child form,when child form height is fixed/percent/left,no layout form
        LayoutChildForm(form_ctrl_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : init form relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitFormRelativeRect(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in/out:
                                )
{
    BOOLEAN         is_disp_from_right = FALSE;
    GUI_RECT_T      content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

    //display child from right
    is_disp_from_right = CTRLFORM_IsDisplayFromRight(form_ctrl_ptr);

    //init form relative rect
    form_ctrl_ptr->relative_rect.left   = 0;
    form_ctrl_ptr->relative_rect.top    = 0;
    form_ctrl_ptr->relative_rect.right  = 0;
    form_ctrl_ptr->relative_rect.bottom = 0;

    //display child from right
    if (is_disp_from_right)
    {
        //right
        form_ctrl_ptr->relative_rect.right = content_rect.right - content_rect.left;

        //left
        form_ctrl_ptr->relative_rect.left = form_ctrl_ptr->relative_rect.right;
    }
    else
    {
        //right
        if ((MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()) &&
            (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr))
        {
            form_ctrl_ptr->relative_rect.right = form_ctrl_ptr->relative_rect.right + CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : adjust form relative rect by child after dock
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustFormRelativeRect(
                                  CTRLFORM_CHILD_T  *child_ptr,     //in:
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                  )
{
    BOOLEAN     is_disp_from_right = FALSE;

    //display child from right
    is_disp_from_right = CTRLFORM_IsDisplayFromRight(form_ctrl_ptr);

    //adjust form relative rect
    switch (form_ctrl_ptr->layout_type)
    {
    case GUIFORM_LAYOUT_ORDER:
        //bottom
        if (child_ptr->index > form_ctrl_ptr->first_valid_index)
        {
            form_ctrl_ptr->relative_rect.bottom = child_ptr->relative_rect.top - child_ptr->ver_space - 1;
        }
        break;

    case GUIFORM_LAYOUT_SBS:
        if (child_ptr->index == form_ctrl_ptr->first_valid_index)
        {
            //display child from right
            if (is_disp_from_right)
            {
                //modify left by align
                ModifyPosByAlign(FALSE,
                    &form_ctrl_ptr->relative_rect.left,
                    form_ctrl_ptr->sbs_width,
                    form_ctrl_ptr,
                    form_ctrl_ptr->align);
            }
            else
            {
                //modify right by align
                ModifyPosByAlign(TRUE,
                    &form_ctrl_ptr->relative_rect.right,
                    form_ctrl_ptr->sbs_width,
                    form_ctrl_ptr,
                    form_ctrl_ptr->align);
            }
        }
        else
        {
            //display child from right
            if (is_disp_from_right)
            {
                form_ctrl_ptr->relative_rect.left = child_ptr->relative_rect.right + child_ptr->hor_space + 1;
            }
            else
            {
                form_ctrl_ptr->relative_rect.right = child_ptr->relative_rect.left - child_ptr->hor_space - 1;
            }
        }
        break;

    default:
        SCI_TRACE_LOW("InitFormRelativeRectAfterDock:layout_type %d is error!",form_ctrl_ptr->layout_type);
        break;
    }
}

/*****************************************************************************/
//  Description : set form relative rect by child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormRelativeRect(
                               CTRLFORM_CHILD_T *child_ptr,     //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               )
{
    BOOLEAN         is_disp_from_right = FALSE;
    uint16          child_height = 0;
    uint16          child_width = 0;
    uint16          font_height = 0;
    uint16          small_font_height = 0;
    GUI_FONT_ALL_T  ctrl_font = {0};

    //display child from right
    is_disp_from_right = CTRLFORM_IsDisplayFromRight(form_ctrl_ptr);

    switch (child_ptr->dock_type)
    {
    case GUIFORM_CHILD_DOCK_TOP:
#ifdef GUIF_FORM_DOCKABLE
        //get top child height
        child_height = CTRLFORM_GetDockChildHeight(child_ptr);
#endif
        
        //top/bottom
        form_ctrl_ptr->relative_rect.top    = form_ctrl_ptr->relative_rect.bottom + child_height;
        form_ctrl_ptr->relative_rect.bottom = form_ctrl_ptr->relative_rect.bottom + child_height;
        break;

    case GUIFORM_CHILD_DOCK_BOTTOM:
        //do nothing
        break;

    case GUIFORM_CHILD_DOCK_LEFT:
#ifdef GUIF_FORM_DOCKABLE
        //get left child width
        child_width = CTRLFORM_GetDockChildWidth(child_ptr);
#endif

        if (is_disp_from_right)
        {
            //left/right
            form_ctrl_ptr->relative_rect.right = form_ctrl_ptr->relative_rect.left - child_width;
            form_ctrl_ptr->relative_rect.left  = form_ctrl_ptr->relative_rect.left - child_width;

            //modify left by align
            ModifyPosByAlign(FALSE,
                &form_ctrl_ptr->relative_rect.left,
                form_ctrl_ptr->sbs_width,
                form_ctrl_ptr,
                form_ctrl_ptr->align);
        }
        else
        {
            //left/right
            form_ctrl_ptr->relative_rect.left  = form_ctrl_ptr->relative_rect.right + child_width;
            form_ctrl_ptr->relative_rect.right = form_ctrl_ptr->relative_rect.right + child_width;

            //modify right by align
            ModifyPosByAlign(TRUE,
                &form_ctrl_ptr->relative_rect.right,
                form_ctrl_ptr->sbs_width,
                form_ctrl_ptr,
                form_ctrl_ptr->align);
        }
        break;

    case GUIFORM_CHILD_DOCK_RIGHT:
        if (is_disp_from_right)
        {
            //modify left by align
            ModifyPosByAlign(FALSE,
                &form_ctrl_ptr->relative_rect.left,
                form_ctrl_ptr->sbs_width,
                form_ctrl_ptr,
                form_ctrl_ptr->align);
        }
        else
        {
            //modify right by align
            ModifyPosByAlign(TRUE,
                &form_ctrl_ptr->relative_rect.right,
                form_ctrl_ptr->sbs_width,
                form_ctrl_ptr,
                form_ctrl_ptr->align);
        }
        break;

    default:
        switch (form_ctrl_ptr->layout_type)
        {
        case GUIFORM_LAYOUT_ORDER:
            form_ctrl_ptr->relative_rect.bottom = child_ptr->relative_rect.bottom;
            break;

        case GUIFORM_LAYOUT_SBS:
            //display child from right
            if (CTRLFORM_IsDisplayFromRight(form_ctrl_ptr))
            {
                form_ctrl_ptr->relative_rect.left  = child_ptr->relative_rect.left;
            }
            else
            {
                form_ctrl_ptr->relative_rect.right  = child_ptr->relative_rect.right;
            }
            form_ctrl_ptr->relative_rect.bottom = (int32)MAX(form_ctrl_ptr->relative_rect.bottom,child_ptr->relative_rect.bottom);
            break;

        default:
            //SCI_TRACE_LOW:"GetChildRelativeRect:layout_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_LAYOUT_1100_112_2_18_3_18_30_162,(uint8*)"d",form_ctrl_ptr->layout_type);
            break;
        }

        //is unit form which include one control
        if ((CTRLFORM_IsOrderForm(form_ctrl_ptr)) &&
            (CTRLFORM_IsUnitFormSingle(form_ctrl_ptr)))
        {
            //get child height
            child_height = CTRLFORM_GetChildHeight(child_ptr);

            //get child font height
            IGUICTRL_GetFont(child_ptr->child_ctrl_ptr,&ctrl_font);
            font_height = GUI_GetFontHeight(ctrl_font.font,UNICODE_HANZI);
        
            //get unit small font
            small_font_height = GUI_GetFontHeight(form_ctrl_ptr->theme.unit_font.font,UNICODE_HANZI);

            //modify form relative rect bottom
            form_ctrl_ptr->relative_rect.bottom = form_ctrl_ptr->relative_rect.bottom + child_height + small_font_height - font_height + 1;
        }
        break;
    }

#ifdef CTRLFORM_DRUMP_LAYOUT_INFO
    {
        // 必须要与GUIFORM_CHILD_WIDTH_E一致
        char* layout_type[GUIFORM_LAYOUT_MAX] = 
        {
            "GUIFORM_LAYOUT_ORDER","GUIFORM_LAYOUT_SBS",
        };
        
        CTRLFORM_TRACE_LAYOUT(
            "^^^ AdjustFormRelativeRect: form_ctrl_ptr=0x%08x, "
            "layout_type=%s, "
            "relative_rect={%d,%d,%d,%d}",
            form_ctrl_ptr,
            layout_type[form_ctrl_ptr->layout_type],
            form_ctrl_ptr->relative_rect.left,
            form_ctrl_ptr->relative_rect.top,
            form_ctrl_ptr->relative_rect.right,
            form_ctrl_ptr->relative_rect.bottom);
    }
#endif
}

/*****************************************************************************/
//  Description : end form relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void EndFormRelativeRect(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               )
{
    BOOLEAN         is_disp_from_right = FALSE;
#ifdef GUIF_FORM_DOCKABLE
    uint16          child_width = 0;
#endif
    GUI_RECT_T      content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

    //reset form relative rect left,right
    form_ctrl_ptr->relative_rect.left   = 0;
    form_ctrl_ptr->relative_rect.right  = content_rect.right - content_rect.left;

    //display child from right
    is_disp_from_right = CTRLFORM_IsDisplayFromRight(form_ctrl_ptr);

#ifdef GUIF_FORM_DOCKABLE
    //except left and right child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->left_child_ptr);
    if (is_disp_from_right)
    {
        form_ctrl_ptr->relative_rect.right = form_ctrl_ptr->relative_rect.right - child_width;
    }
    else
    {
        form_ctrl_ptr->relative_rect.left = form_ctrl_ptr->relative_rect.left + child_width;
    }

    //except right child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->right_child_ptr);
    if (is_disp_from_right)
    {
        form_ctrl_ptr->relative_rect.left = form_ctrl_ptr->relative_rect.left + child_width;
    }
    else
    {
        form_ctrl_ptr->relative_rect.right = form_ctrl_ptr->relative_rect.right - child_width;
    }
#endif
}

/*****************************************************************************/
//  Description : modify left or right position by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPosByAlign(
                            BOOLEAN                 is_left,        //in:
                            int32                   *pos_ptr,       //in/out:
                            uint16                  child_width,    //in:
                            CTRLFORM_OBJ_T          *form_ctrl_ptr, //in:
                            GUIFORM_CHILD_ALIGN_E   align           //in:
                            )
{
    BOOLEAN     is_except_prg = FALSE;
    uint16      display_width = 0;
    
    //get max child width
    if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
    {
        is_except_prg = TRUE;
    }
    
    display_width = CTRLFORM_GetDisplayWidth(is_except_prg,form_ctrl_ptr);
    
    switch (align)
    {
    case GUIFORM_CHILD_ALIGN_NONE:
        //do nothing
        break;
        
    case GUIFORM_CHILD_ALIGN_LEFT:
        if (display_width > child_width)
        {
            if (!is_left)
            {
                *pos_ptr = *pos_ptr + child_width - display_width;
            }
        }
        break;
        
    case GUIFORM_CHILD_ALIGN_HMIDDLE:
        if (display_width > child_width)
        {
            if (is_left)
            {
                *pos_ptr  = *pos_ptr + ((display_width - child_width)>>1);
            }
            else
            {
                *pos_ptr  = *pos_ptr - ((display_width - child_width)>>1);
            }
        }
        break;
        
    case GUIFORM_CHILD_ALIGN_RIGHT:
        if (display_width > child_width)
        {
            if (is_left)
            {
                *pos_ptr = *pos_ptr + (display_width - child_width);
            }
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"ModifyPosByAlign:align %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_LAYOUT_611_112_2_18_3_18_29_161,(uint8*)"d",align);
        break;
    }
}

/*****************************************************************************/
//  Description : layout all child control,relative rect,set background,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LayoutAllChildCtrl(//is create prg
                                 CTRLFORM_OBJ_T *form_ctrl_ptr  //in/out:
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //set unit form param
    CTRLFORM_SetParam(form_ctrl_ptr);

    //init form relative rect
    InitFormRelativeRect(form_ctrl_ptr);

#ifdef GUIF_FORM_DOCKABLE
    if (0 == form_ctrl_ptr->layout_index)
    {
        //calculate all dock child width and height
        CTRLFORM_CalcAllDockChild(form_ctrl_ptr);
    }
#endif

    //reset sbs width and max height
    form_ctrl_ptr->sbs_width        = 0;
    form_ctrl_ptr->child_max_height = 0;

    //calculate all child width and height
    result = CTRLFORM_CalcAllChild(form_ctrl_ptr);

    //set display direction
    form_ctrl_ptr->display_dir = MMITHEME_GetDisDirection();

#ifdef GUIF_FORM_DOCKABLE
    if (0 == form_ctrl_ptr->layout_index)
    {
        //layout dock child
        LayoutDockChildCtrl(form_ctrl_ptr);
    }
#endif
    
    //get the start layout child pointer
    child_ptr = GetLayoutStartChildPtr(form_ctrl_ptr);
    if (PNULL != child_ptr)
    {
        //adjust form relative rect
        AdjustFormRelativeRect(child_ptr,form_ctrl_ptr);

        //parse child control
        while (PNULL != child_ptr)
        {
            if (CTRLFORM_IsValidChild(child_ptr))
            {
                //get child relative rect
                GetChildRelativeRect(child_ptr,form_ctrl_ptr);
            }
    
            child_ptr = child_ptr->next_ptr;
        }
    }

    //end form relative rect
    EndFormRelativeRect(form_ctrl_ptr);

    //modify form relative display rect
    InitFormDisplayRect(form_ctrl_ptr);

    return (result);
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : layout dock child control,relative rect,set background,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LayoutDockChildCtrl(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in/out:
                               )
{
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            //get child relative rect
            child_ptr->index = form_ctrl_ptr->first_valid_index;
            GetChildRelativeRect(child_ptr,form_ctrl_ptr);
        }

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : get the layout of start child pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetLayoutStartChildPtr(
                                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                                               )
{
    uint16              layout_index = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //get the start index
    layout_index = (uint16)MAX(form_ctrl_ptr->layout_index,form_ctrl_ptr->first_valid_index);

    CTRLFORM_GetChildPtrByIndex(layout_index,form_ctrl_ptr,&child_ptr);

    return (child_ptr);
}

/*****************************************************************************/
//  Description : set form child control param,include bg,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetParam(//is unit form
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                 )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_disable_active = FALSE;
    BOOLEAN         is_disable_tp = FALSE;
    GUI_BG_T        ctrl_bg = {0};
    GUI_BORDER_T    ctrl_border = {0};
    
    if (CTRLFORM_IsUnitForm(form_ctrl_ptr))
    {
        if (CTRLFORM_IsGetAtv(form_ctrl_ptr))
        {
            //set disable active
            is_disable_active = FALSE;
        }
        else
        {
            //set disable active
            is_disable_active = TRUE;
        }
        
        //set control state
        VTLBASE_SetCanActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,(BOOLEAN)!is_disable_active);
        
        if (CTRLFORM_IsGray(form_ctrl_ptr))
        {
            //set disable tp
            is_disable_tp = TRUE;
        }
        
        //set control state
        VTLBASE_SetTpEnabled((CTRLBASE_OBJ_T *)form_ctrl_ptr,(BOOLEAN)!is_disable_tp);
        
        //get bg and border
        ctrl_bg.bg_type = GUI_BG_NONE;
        if (PNULL != form_ctrl_ptr->active_child_ptr)
        {
            ctrl_bg = form_ctrl_ptr->theme.form_active_child.bg;
            ctrl_border = form_ctrl_ptr->theme.form_active_child.border;
        }
        else
        {
            ctrl_border = form_ctrl_ptr->theme.form_child.border;
        }
        
        //is display bg and border
        if (!IsDisplayBgBorder(form_ctrl_ptr))
        {
            ctrl_bg.bg_type = GUI_BG_NONE;
            if (GUI_BORDER_NONE != ctrl_border.type)
            {
                ctrl_border.type = GUI_BORDER_NOT_DISPLAY;
            }
        }
        
        //set bg
        VTLBASE_SetBg((CTRLBASE_OBJ_T *)form_ctrl_ptr,&ctrl_bg);
        
        //set border
        VTLBASE_SetBorder((CTRLBASE_OBJ_T *)form_ctrl_ptr,&ctrl_border);
        
        //unit form is handle tp first
        if ((is_disable_active) || 
            (PNULL != form_ctrl_ptr->active_child_ptr))
        {
            VTLBASE_SetTpPriority((CTRLBASE_OBJ_T *)form_ctrl_ptr,CTRL_TP_PRIORITY_NORMAL);
            VTLBASE_SetCanActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,FALSE);
        }
        else
        {
            VTLBASE_SetTpPriority((CTRLBASE_OBJ_T *)form_ctrl_ptr,CTRL_TP_PRIORITY_HIGH);
            VTLBASE_SetCanActive((CTRLBASE_OBJ_T *)form_ctrl_ptr,TRUE);
        }
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set child control param,include bg,border,font etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetChildParam(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                                   CTRLFORM_CHILD_T *child_ptr      //in:
                                   )
{
    BOOLEAN     is_disable_active = FALSE;
    
    if (PNULL != child_ptr)
    {
        if (CTRLFORM_IsGetAtvChild(child_ptr))
        {
            //set disable active
            is_disable_active = FALSE;
        }
        else
        {
            //set disable active
            is_disable_active = TRUE;
        }
        
        //set control state
        IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,is_disable_active);
        
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            //set form child param
            SetFormChildParam(form_ctrl_ptr,child_ptr);
        }
        else
        {
            //set other child param
            SetOtherChildParam(form_ctrl_ptr,child_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set child form param,include bg,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFormChildParam(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T   *child_ptr      //in:
                             )
{
    BOOLEAN             is_unit = FALSE;
    BOOLEAN             is_disable_tp = FALSE;
    GUI_BG_T            ctrl_bg = {0};
    GUI_BORDER_T        ctrl_border = {0};
    
    is_unit = CTRLFORM_IsUnitFormChild(child_ptr);
    
    if (is_unit)
    {
        //get bg
        ctrl_bg = GetChildBg(TRUE,child_ptr,form_ctrl_ptr);
        
        //set bg
        IGUICTRL_SetBg(child_ptr->child_ctrl_ptr,&ctrl_bg);
        
        if (GUIFORM_CHILD_DISP_GRAY == child_ptr->display_type)
        {
            //set disable tp
            is_disable_tp = TRUE;
        }
        
        //set control state
        IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_TP,is_disable_tp);
        
        //unit form is handle tp first
        if ((CTRLFORM_IsAtvFormChild(child_ptr)) ||
            (!CTRLFORM_IsGetAtvChild(child_ptr)))
        {
            IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_TOPMOST,FALSE);
            IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
        }
        else
        {
            IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_TOPMOST,TRUE);
            IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_ACTIVE,FALSE);
        }
    }
    
    if (form_ctrl_ptr->is_permit_border)
    {
        //get border
        ctrl_border = GetChildBorder(TRUE,is_unit,child_ptr,form_ctrl_ptr);
        
        //set border
        IGUICTRL_SetBorder(child_ptr->child_ctrl_ptr,&ctrl_border);
    }
}

/*****************************************************************************/
//  Description : set child other param,include bg,font,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetOtherChildParam(
                              CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                              CTRLFORM_CHILD_T  *child_ptr      //in:
                              )
{
    BOOLEAN             is_disable_tp = FALSE;
    GUI_BG_T            ctrl_bg = {0};
    GUI_BORDER_T        ctrl_border = {0};
    GUI_FONT_ALL_T      ctrl_font = {0};
    
    if (GUIFORM_CHILD_DISP_GRAY == child_ptr->display_type)
    {
        //set disable tp
        is_disable_tp = TRUE;
    }
    
    //set tp state
    IGUICTRL_SetState(child_ptr->child_ctrl_ptr,GUICTRL_STATE_DISABLE_TP,is_disable_tp);
    
    if (form_ctrl_ptr->is_permit_bg)
    {
        //get bg
        ctrl_bg = GetChildBg(FALSE,child_ptr,form_ctrl_ptr);
        
        //set bg
        IGUICTRL_SetBg(child_ptr->child_ctrl_ptr,&ctrl_bg);
    }
    
    if (form_ctrl_ptr->is_permit_font)
    {
        //get font
        ctrl_font = GetChildFont(child_ptr,form_ctrl_ptr);
        
        //set font
        IGUICTRL_SetFont(child_ptr->child_ctrl_ptr,&ctrl_font);
    }
    
    if (form_ctrl_ptr->is_permit_border)
    {
        //get border
        ctrl_border = GetChildBorder(FALSE,FALSE,child_ptr,form_ctrl_ptr);
        
        //set border
        IGUICTRL_SetBorder(child_ptr->child_ctrl_ptr,&ctrl_border);
    }
}

/*****************************************************************************/
//  Description : get child bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BG_T GetChildBg(
                          BOOLEAN           is_form_child,
                          CTRLFORM_CHILD_T  *child_ptr,
                          CTRLFORM_OBJ_T    *form_ctrl_ptr
                          )
{
    GUI_BG_T    ctrl_bg = {0};
    
    //set bg
    ctrl_bg.bg_type = GUI_BG_NONE;
    if ((IsDisplayBgBorder(form_ctrl_ptr)) &&
        (child_ptr->is_get_active) && 
        (GUIFORM_CHILD_DISP_NORMAL == child_ptr->display_type))
    {
        if (is_form_child)
        {
            if (CTRLFORM_IsAtvFormChild(child_ptr))
            {
                ctrl_bg = form_ctrl_ptr->theme.form_active_child.bg;
            }
        }
        else
        {
            //is unit form which include one ctrl
            if (!CTRLFORM_IsUnitFormSingle(form_ctrl_ptr))
            {
                if (child_ptr == form_ctrl_ptr->active_child_ptr)
                {
                    ctrl_bg = form_ctrl_ptr->theme.active_child.bg;
                }
                else
                {
                    ctrl_bg = form_ctrl_ptr->theme.child.bg;
                }
            }
        }
    }
    
    return (ctrl_bg);
}

/*****************************************************************************/
//  Description : is display bg and border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayBgBorder(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                )
{
    BOOLEAN     result = FALSE;
    
    if (GUIFORM_TYPE_TP == form_ctrl_ptr->type)
    {
        if ((form_ctrl_ptr->is_tp_down) || 
            (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
        {
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get child border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BORDER_T GetChildBorder(
                                  BOOLEAN           is_form_child,
                                  BOOLEAN           is_unit_form,
                                  CTRLFORM_CHILD_T  *child_ptr,
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  )
{
    GUI_BORDER_T    ctrl_border = {0};
    
    //set border
    ctrl_border.type = GUI_BORDER_NONE;
    if (is_form_child)
    {
        if (is_unit_form)
        {
            if (CTRLFORM_IsAtvFormChild(child_ptr))
            {
                ctrl_border = form_ctrl_ptr->theme.form_active_child.border;
            }
            else
            {
                ctrl_border = form_ctrl_ptr->theme.form_child.border;
            }
        }
    }
    else if (!CTRLFORM_IsUnitFormSingle(form_ctrl_ptr))
    {
        if (child_ptr == form_ctrl_ptr->active_child_ptr)
        {
            ctrl_border = form_ctrl_ptr->theme.active_child.border;
        }
        else if (child_ptr->is_get_active)
        {
            if (GUIFORM_CHILD_DISP_NORMAL == child_ptr->display_type)
            {
                ctrl_border = form_ctrl_ptr->theme.child.border;
            }
            else
            {
                ctrl_border = form_ctrl_ptr->theme.gray_child.border;
            }
        }
    }
    
    //is display border
    if ((!IsDisplayBgBorder(form_ctrl_ptr)) &&
        (GUI_BORDER_NONE != ctrl_border.type))
    {
        ctrl_border.type = GUI_BORDER_NOT_DISPLAY;
    }
    
    return (ctrl_border);
}

/*****************************************************************************/
//  Description : get child font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_FONT_ALL_T GetChildFont(
                                  CTRLFORM_CHILD_T  *child_ptr,
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  )
{
    GUI_FONT_ALL_T  ctrl_font = {0};
    
    //get font
    IGUICTRL_GetFont(child_ptr->child_ctrl_ptr,&ctrl_font);
    
    //only set font color
    if (GUIFORM_CHILD_DISP_GRAY == child_ptr->display_type)
    {
        ctrl_font.color = form_ctrl_ptr->theme.gray_child.font.color;
    }
    else
    {
        if ((child_ptr->is_get_active) && 
            (!CTRLFORM_IsUnitFormSingle(form_ctrl_ptr)))
        {
            ctrl_font.color = form_ctrl_ptr->theme.active_child.font.color;
        }
        else
        {
			if(!form_ctrl_ptr->is_permit_font_color)
			{
				ctrl_font.color = form_ctrl_ptr->theme.child.font.color;
			}
        }
    }
    
    //set unit child font,except the first child
    if ((GUIFORM_TYPE_TP == form_ctrl_ptr->type) &&
        (CTRLFORM_IsUnitForm(form_ctrl_ptr)) &&
        (child_ptr->index > form_ctrl_ptr->first_valid_index))
    {
        if (GUIFORM_CHILD_DISP_GRAY == child_ptr->display_type)
        {
            ctrl_font.font = form_ctrl_ptr->theme.unit_font.font;
        }
        else
        {
            ctrl_font = form_ctrl_ptr->theme.unit_font;
        }
    }
    
    return (ctrl_font);
}

/*****************************************************************************/
//  Description : get child relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetChildRelativeRect(
                                CTRLFORM_CHILD_T    *child_ptr,     //in/out:
                                CTRLFORM_OBJ_T      *form_ctrl_ptr  //in/out:
                                )
{
    uint16          child_width = 0;
    uint16          child_height = 0;
    int32           max_right = 0;
    GUI_BIG_RECT_T  child_rect = {0};

    //get child width and height
    GetChildWidthHeight(&child_width,&child_height,child_ptr,form_ctrl_ptr);
    
    //get child relative rect top
    child_rect.top = GetChildRelativeTop(child_height,child_ptr,form_ctrl_ptr);
    
    //get child relative rect left
    child_rect.left = GetChildRelativeLeft(child_width,child_ptr,form_ctrl_ptr);
    
    if ((0 < child_width) &&
        (0 < child_height))
    {
        //get max right
        max_right = CTRLFORM_GetDisplayMaxRight(form_ctrl_ptr,child_ptr);
        
        //set child relative rect right and bottom
        if (CTRLFORM_IsSbsForm(form_ctrl_ptr))
        {
            child_rect.right  = (int32)(child_rect.left + child_width - 1);
        }
        else
        {
            child_rect.right  = (int32)MIN((child_rect.left + child_width - 1),max_right);
        }
        child_rect.bottom = child_rect.top + child_height - 1;
    }
    else
    {
        SCI_MEMSET(&child_rect,0,sizeof(GUI_BIG_RECT_T));
    }
    
    //set child rect
    child_ptr->relative_rect = child_rect;

    //set form relative rect
    SetFormRelativeRect(child_ptr,form_ctrl_ptr);
}

/*****************************************************************************/
//  Description : get child control relative top coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
// 这个函数是在计算子控件相对坐标时调用，这里会同时计算form的相对坐标
/*****************************************************************************/
LOCAL int32 GetChildRelativeTop(
                                uint16              child_height,   //in:
                                CTRLFORM_CHILD_T    *child_ptr,     //in:
                                CTRLFORM_OBJ_T      *form_ctrl_ptr  //in:
                                )
{
    int32           relative_top = 0;
    GUI_RECT_T      content_rect = {0};

    //get child relative top coordinate
    if (GUIFORM_CHILD_DOCK_BOTTOM == child_ptr->dock_type)
    {
        //get content rect
        content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

        relative_top = content_rect.bottom - content_rect.top + 1 - child_height;

        //except softkey
        if (CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr))
        {
            relative_top = relative_top - form_ctrl_ptr->softkey_child_ptr->height;
        }
    }
    else
    {
        switch (form_ctrl_ptr->layout_type)
        {
        case GUIFORM_LAYOUT_ORDER:
            if ((child_ptr->index == form_ctrl_ptr->first_valid_index) ||
                (0 == form_ctrl_ptr->relative_rect.bottom))
            {
                relative_top = form_ctrl_ptr->relative_rect.bottom;
            }
            else
            {
                relative_top = form_ctrl_ptr->relative_rect.bottom + child_ptr->ver_space + 1;
            }
            break;

        case GUIFORM_LAYOUT_SBS:
            if ((child_ptr->index == form_ctrl_ptr->first_valid_index) ||
                (0 == form_ctrl_ptr->relative_rect.bottom))
            {
                relative_top = form_ctrl_ptr->relative_rect.bottom;
            }
            else
            {
                relative_top = child_ptr->prev_ptr->relative_rect.top;
            }
            break;

        default:
            SCI_PASSERT(FALSE,("GetChildRelativeTop:layout_type %d is error!",form_ctrl_ptr->layout_type)); /*assert verified*/
            break;
        }
    }

    return (relative_top);
}

/*****************************************************************************/
//  Description : get child control relative left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetChildRelativeLeft(
                                 uint16             child_width,    //in:
                                 CTRLFORM_CHILD_T   *child_ptr,     //in:
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_disp_from_right = FALSE;
    int32       relative_left = 0;
    int32       relative_right = 0;
    GUI_RECT_T  content_rect = {0};

    //display child from right
    is_disp_from_right = CTRLFORM_IsDisplayFromRight(form_ctrl_ptr);

    //display child from right
    if (is_disp_from_right)
    {
        if (GUIFORM_CHILD_DOCK_LEFT == child_ptr->dock_type)
        {
            //get content rect
            content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

            relative_right = content_rect.right - content_rect.left;

            if (relative_right < child_width)
            {
                relative_left = 0;
            }
            else
            {
                relative_left = relative_right - child_width + 1;
            }

            result = TRUE;
        }
    }
    else
    {
        if (GUIFORM_CHILD_DOCK_RIGHT == child_ptr->dock_type)
        {
            //get content rect
            content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

            relative_left = content_rect.right - content_rect.left + 1 - child_width;

            result = TRUE;
        }
    }
    
    if (!result)
    {
        //get child relative left coordinate
        switch (form_ctrl_ptr->layout_type)
        {
        case GUIFORM_LAYOUT_ORDER:
            //display child from right
            if (is_disp_from_right)
            {
                relative_right = form_ctrl_ptr->relative_rect.left;
            
                //add indentation width
                if (relative_right > child_ptr->indent_width)
                {
                    relative_right = relative_right - child_ptr->indent_width;
                }
            
                //modify right by align
                ModifyPosByAlign(FALSE,
                    &relative_right,
                    child_width,
                    form_ctrl_ptr,
                    child_ptr->align);
            
                if (relative_right < child_width)
                {
                    relative_left = 0;
                }
                else
                {
                    relative_left = relative_right - child_width + 1;
                }
            }
            else
            {
                //set child relative rect left
                relative_left = form_ctrl_ptr->relative_rect.left;
            
                if ((MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()) &&
                    (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr))
                {
                    relative_left = relative_left + CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
                }
            
                //add indentation width
                if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
                {
                    relative_left = relative_left + child_ptr->indent_width;
                }
            
                //modify left by align
                ModifyPosByAlign(TRUE,
                    &relative_left,
                    child_width,
                    form_ctrl_ptr,
                    child_ptr->align);
            }
            break;
        
        case GUIFORM_LAYOUT_SBS:
            //display child from right
            if (is_disp_from_right)
            {
                if ((0 < child_ptr->hor_space) &&
                    (child_ptr->index > form_ctrl_ptr->first_valid_index) && 
                    (form_ctrl_ptr->relative_rect.left > child_ptr->hor_space))
                {
                    relative_right = form_ctrl_ptr->relative_rect.left - child_ptr->hor_space - 1;
                }
                else
                {
                    if (form_ctrl_ptr->relative_rect.left == form_ctrl_ptr->relative_rect.right)
                    {
                        relative_right = form_ctrl_ptr->relative_rect.left;
                    }
                    else
                    {
                        relative_right = form_ctrl_ptr->relative_rect.left - 1;
                    }
                }

                if (relative_right < child_width)
                {
                    relative_left = 0;
                }
                else
                {
                    relative_left = relative_right - child_width + 1;
                }
            }
            else
            {
                if ((child_ptr->index == form_ctrl_ptr->first_valid_index) || 
                    (0 == form_ctrl_ptr->relative_rect.right))
                {
                    relative_left = form_ctrl_ptr->relative_rect.right;
                }
                else
                {
                    relative_left = form_ctrl_ptr->relative_rect.right + child_ptr->hor_space + 1;
                }
            }
            break;
        
        default:
            SCI_PASSERT(FALSE,("GetChildRelativeLeft:layout_type %d is error!",form_ctrl_ptr->layout_type)); /*assert verified*/
            break;
        }
    }
    
    return (relative_left);
}

/*****************************************************************************/
//  Description : get child control width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetChildWidthHeight(
                               uint16           *width_ptr,     //in/out:
                               uint16           *height_ptr,    //in/out:
                               CTRLFORM_CHILD_T *child_ptr,     //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               )
{
    if ((PNULL != width_ptr) && 
        (PNULL != height_ptr))
    {
        switch (form_ctrl_ptr->layout_type)
        {
        case GUIFORM_LAYOUT_ORDER:
            //get child width and height
            *width_ptr  = child_ptr->width;
            *height_ptr = child_ptr->height;
            break;
            
        case GUIFORM_LAYOUT_SBS:
            //get child width and height
            if (CTRLFORM_IsDockChild(child_ptr))
            {
                *width_ptr  = child_ptr->width;
                *height_ptr = child_ptr->height;
            }
            else
            {
                *width_ptr  = child_ptr->width;
                *height_ptr = form_ctrl_ptr->child_max_height;
            }
            break;
            
        default:
            SCI_PASSERT(FALSE,("GetChildWidthHeight:layout_type %d is error!",form_ctrl_ptr->layout_type)); /*assert verified*/
            break;
        }

        //adjust child height
        AdjustChildHeight(height_ptr,child_ptr,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : adjust child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustChildHeight(
                             uint16             *height_ptr,    //in/out:
                             CTRLFORM_CHILD_T   *child_ptr,     //in:
                             CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                             )
{
    uint16      display_height = 0;
    GUI_RECT_T  content_rect = {0};

    //adjust child height
    CTRLFORM_AdjustHeightByRange(height_ptr,&child_ptr->size_range);

    if (CTRLFORM_IsDockChild(child_ptr))
    {
        //adjust dock child height
        CTRLFORM_AdjustHeightByRange(height_ptr,&form_ctrl_ptr->size_range);

        //get content rect
        content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
        if (!GUI_IsRectEmpty(content_rect))
        {
            //get display height
            display_height = (uint16)(content_rect.bottom - content_rect.top + 1);

            //except softkey
            if ((CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr)) &&
                (display_height >= form_ctrl_ptr->softkey_child_ptr->height))
            {
                display_height = (uint16)(display_height - form_ctrl_ptr->softkey_child_ptr->height);
            }

            //adjust display height
            CTRLFORM_AdjustHeightByRange(&display_height,&form_ctrl_ptr->size_range);

            if (*height_ptr > display_height)
            {
                *height_ptr = display_height;
            }
        }
    }
}

/*****************************************************************************/
//  Description : destroy progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyPrgCtrl(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr
                             )
{
    BOOLEAN             result = FALSE;
    uint16              child_height = 0;
    uint16              display_height = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
    {
        //is need destroy progress bar
        if (0 == form_ctrl_ptr->layout_index)
        {
            result = TRUE;
        }
        else if (CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->layout_index,form_ctrl_ptr,&child_ptr))
        {
            switch (form_ctrl_ptr->layout_type)
            {
            case GUIFORM_LAYOUT_ORDER:
                //get current child height
                child_height = (uint16)(child_ptr->relative_rect.bottom - child_ptr->relative_rect.top + 1);
                if ((child_ptr->index == form_ctrl_ptr->first_valid_index) && 
                    (PNULL != child_ptr->next_ptr))
                {
                    child_height = (uint16)(child_height + child_ptr->next_ptr->ver_space);
                }
                else
                {
                    child_height = (uint16)(child_height + child_ptr->ver_space);
                }
                
                //other all child height
                child_height = (uint16)(form_ctrl_ptr->relative_rect.bottom - child_height);
                
                //get display height
                display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
                
                //not need progress
                if (child_height <= display_height)
                {
                    result = TRUE;
                }
                break;
                
            case GUIFORM_LAYOUT_SBS:
                result = TRUE;
                break;
                
            default:
                //SCI_TRACE_LOW:"DestroyPrgCtrl:layout_type %d is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_LAYOUT_1416_112_2_18_3_18_31_163,(uint8*)"d",form_ctrl_ptr->layout_type);
                break;
            }
        }
        
        if (result)
        {
            //destroy progress bar control
            MMK_DestroyControl(IGUICTRL_GetCtrlHandle(form_ctrl_ptr->prgbox_ctrl_ptr));
            form_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : init form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitFormDisplayRect(
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               )
{
    int32       display_top = 0;
    int32       display_left = 0;
    int32       display_min_top = 0;
    int32       display_max_top = 0;
    int32       display_min_left = 0;
    int32       display_max_left = 0;

    //set display left and top
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    display_top  = form_ctrl_ptr->relative_disp_rect.top;

    //init display rect
    CTRLFORM_InitDisplayRect(form_ctrl_ptr);

    //get display min/max top and left
    display_min_top  = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
    display_max_top  = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
    display_min_left = CTRLFORM_GetDisplayMinLeft(form_ctrl_ptr);
    display_max_left = CTRLFORM_GetDisplayMaxLeft(form_ctrl_ptr);

    //adjust display top and left
    display_top  = (int32)MAX(display_top,display_min_top);
    display_top  = (int32)MIN(display_top,display_max_top);
    display_left = (int32)MAX(display_left,display_min_left);
    display_left = (int32)MIN(display_left,display_max_left);

    //set display left and top
    CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr);
}

/*****************************************************************************/
//  Description : adjust form display rect,include dock and other child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustAllDisplayRect(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                )
{
#ifdef GUIF_FORM_DOCKABLE
    //adjust dock display rect
    AdjustDockDisplayRect(form_ctrl_ptr);
#endif

    //adjust not dock display rect
    AdjustDisplayRect(form_ctrl_ptr);

#ifdef GUIFORM_DRUMP_LAYOUT_INFO
    {
        // 必须要与GUIFORM_CHILD_WIDTH_E一致
        char* layout_type[GUIFORM_LAYOUT_MAX] = 
        {
            "GUIFORM_LAYOUT_ORDER",
            "GUIFORM_LAYOUT_SBS",
        };

        GUIFORM_TRACE_LAYOUT(
            "^^^ AdjustAllDisplayRect: form_ctrl_ptr=0x%08x, "
            "layout_type=%s, "
            "relative_rect={%d,%d,%d,%d}",
                form_ctrl_ptr,
                layout_type[form_ctrl_ptr->layout_type],
                form_ctrl_ptr->relative_disp_rect.left,
                form_ctrl_ptr->relative_disp_rect.top,
                form_ctrl_ptr->relative_disp_rect.right,
                form_ctrl_ptr->relative_disp_rect.bottom);
    }
#endif
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : adjust dock child form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustDockDisplayRect(
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 )
{
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsFormChild(child_ptr))
        {
            //set top child rect
            SetDockChildRect(FALSE,FALSE,form_ctrl_ptr,child_ptr);

            //adjust display rect
            AdjustAllDisplayRect((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr);
        }

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : adjust form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustDisplayRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                             )
{
    int32               display_top = 0;
    int32               display_left = 0;
    int32               display_max_top = 0;
    GUI_BIG_RECT_T      relative_disp_rect = {0};
    CTRLFORM_CHILD_T    *child_ptr = form_ctrl_ptr->active_child_ptr;

    //no active child
    if ((PNULL == child_ptr) ||
        (CTRLFORM_IsDockChild(child_ptr)))
    {
        child_ptr = form_ctrl_ptr->cur_child_ptr;
    }

    //only one child except dock
    if ((PNULL != child_ptr) && 
        (CTRLFORM_IsDockChild(child_ptr)) && 
        (form_ctrl_ptr->is_valid_child) && 
        (form_ctrl_ptr->first_valid_index == form_ctrl_ptr->last_valid_index))
    {
        //get child pointer
        CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->first_valid_index,form_ctrl_ptr,&child_ptr);
    }

    //has progress
    if ((PNULL != form_ctrl_ptr->prgbox_ctrl_ptr) &&
        (PNULL != child_ptr) && 
        (!CTRLFORM_IsDockChild(child_ptr)))
    {
        //set child init rect,edit need get cursor rect adjust display rect
        SetChildInitRect(form_ctrl_ptr);

        //set display top and left
        display_top  = form_ctrl_ptr->relative_disp_rect.top;
        display_left = form_ctrl_ptr->relative_disp_rect.left;

        //get display max top
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);

        //adjust display top
        display_top = (int32)MIN(display_top,display_max_top);

        //adjust display top,hide child,need adjust display top
        relative_disp_rect = CTRLFORM_GetDisplayRect(display_top,form_ctrl_ptr);

        //child is not display all
        if (!CTRLFORM_IsRectCovered(relative_disp_rect,child_ptr->relative_rect))
        {
            //get child top
            display_top = CTRLFORM_GetChildTop(child_ptr);

            //adjust display top by edit cursor
            CTRLFORM_AdjustDisplayTopBySelect(&display_top,form_ctrl_ptr,child_ptr);

            //adjust display top
            display_top = (int32)MIN(display_top,display_max_top);
        }

        //adjust display top
        CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set all child init rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetChildInitRect(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                            )
{ 
    GUI_RECT_T          child_rect = {0};
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    GUI_BIG_RECT_T      child_big_rect = {0};
    
    child_ptr = form_ctrl_ptr->first_child_ptr;
    
    //parse child control
    while (PNULL != child_ptr)
    { 
        //get cross rect of the item rect and display rect
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            //set child control rect
            child_big_rect = child_ptr->relative_rect;
            CTRLFORM_ClientToScreen(form_ctrl_ptr,
                &child_big_rect);
            
            child_rect.left   = (int16)child_big_rect.left;
            child_rect.top    = (int16)child_big_rect.top;
            child_rect.right  = (int16)child_big_rect.right;
            child_rect.bottom = (int16)child_big_rect.bottom;
            
            if (!GUI_IsInvalidRect(child_rect))
            {
                IGUICTRL_SetRect(child_ptr->child_ctrl_ptr,&child_rect);
            }
            
            if (CTRLFORM_IsFormChild(child_ptr))
            {
                //set all child init rect
                SetChildInitRect((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr);
            }
        }
        
        child_ptr = child_ptr->next_ptr;
    }
}

/*****************************************************************************/
//  Description : set child control display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetChildRect(
                                  BOOLEAN            is_update,      //in:
                                  CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                  )
{
#ifdef GUIF_FORM_DOCKABLE
    uint16              cur_priority = 0;
#endif
    GUI_RECT_T          form_rect = {0};
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (PNULL != form_ctrl_ptr)
    {
        //set bg child
        if (PNULL != form_ctrl_ptr->bg_child_ptr)
        {
            //get form rect
            form_rect = CTRLFORM_GetRect(form_ctrl_ptr);

            IGUICTRL_SetDisplayRect(form_ctrl_ptr->bg_child_ptr->child_ctrl_ptr,&form_rect,FALSE);
        }

        //若无回写
        if (GUI_IsZeroRect(form_ctrl_ptr->writeback_rect))
        {
            //set softkey child
            SetSoftkeyChildRect(is_update,form_ctrl_ptr);

#ifdef GUIF_FORM_DOCKABLE
            //find dock child
            cur_priority = 1;
            child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
            while (PNULL != child_ptr)
            {
                //set child rect and display rect
                SetDockChildRect(is_update,TRUE,form_ctrl_ptr,child_ptr);

                //get next dock child
                child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
            }
#endif
        }
    
        //parse child control
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            //set child rect and display rect
            SetChildRect(is_update,form_ctrl_ptr,child_ptr);

            child_ptr = child_ptr->next_ptr;
        }
    }
}

/*******************************************************************************************/
//  Description : set softkey child rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL void SetSoftkeyChildRect(
                               BOOLEAN          is_update,      //in:
                               CTRLFORM_OBJ_T   *form_ctrl_ptr  //in:
                               )
{
    GUI_RECT_T          child_rect = {0};
    GUI_RECT_T          form_rect = {0};
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (PNULL != form_ctrl_ptr)
    {
        //set child
        child_ptr = form_ctrl_ptr->softkey_child_ptr;

        if ((PNULL != child_ptr) && 
            (MMK_IsHandleValid(child_ptr->ctrl_handle)))
        {
            if (CTRLFORM_IsValidChild(child_ptr))
            {
                //get form rect
                form_rect = CTRLFORM_GetRect(form_ctrl_ptr);

                //set softkey rect
                child_rect = form_rect;
                child_rect.top = (int16)(child_rect.bottom - child_ptr->height + 1);
            }
        
            //set child rect
            IGUICTRL_SetRect(child_ptr->child_ctrl_ptr,&child_rect);
            IGUICTRL_SetDisplayRect(child_ptr->child_ctrl_ptr,&child_rect,is_update);
        }
    }
}

#ifdef GUIF_FORM_DOCKABLE
/*******************************************************************************************/
//  Description : set dock child rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL void SetDockChildRect(
                            BOOLEAN             is_update,          //in:
                            BOOLEAN             is_set_disp_rect,   //in:
                            CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                            CTRLFORM_CHILD_T    *child_ptr          //in:
                            )
{
    GUI_RECT_T      content_rect = {0};
    GUI_RECT_T      child_rect = {0};
    GUI_RECT_T      intersect_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != child_ptr) && 
        (MMK_IsHandleValid(child_ptr->ctrl_handle)))
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            //get content rect
            content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
        
            //set child control rect
            child_rect.left   = (int16)(child_ptr->relative_rect.left + content_rect.left);
            child_rect.top    = (int16)(child_ptr->relative_rect.top + content_rect.top);
            child_rect.right  = (int16)(child_ptr->relative_rect.right + content_rect.left);
            child_rect.bottom = (int16)(child_ptr->relative_rect.bottom + content_rect.top);
        }

        //set child rect
        IGUICTRL_SetRect(child_ptr->child_ctrl_ptr,&child_rect);

        if (is_set_disp_rect)
        {
            //set display rect
            if (!GUI_IntersectRect(&intersect_rect,base_ctrl_ptr->display_rect,child_rect))
            {
                //no display rect
                intersect_rect.left   = 0;
                intersect_rect.top    = 0;
                intersect_rect.right  = 0;
                intersect_rect.bottom = 0;
            }
            IGUICTRL_SetDisplayRect(child_ptr->child_ctrl_ptr,&intersect_rect,is_update);
        }
    }
}
#endif

/*******************************************************************************************/
//  Description : set child rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL void SetChildRect(
                        BOOLEAN             is_update,      //in:
                        CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                        CTRLFORM_CHILD_T    *child_ptr      //in:
                        )
{
    BOOLEAN             is_need_draw = FALSE;
    GUI_RECT_T          child_rect = {0};
    GUI_RECT_T          display_rect = {0};
    GUI_RECT_T          intersect_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    GUI_BIG_RECT_T      cross_rect = {0};
    GUI_BIG_RECT_T      child_big_rect = {0};

    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != child_ptr))
    {
        child_big_rect = child_ptr->relative_rect;
        CTRLFORM_ClientToScreen(form_ctrl_ptr,
            &child_big_rect);

        //set child control rect
        child_rect.left   = (int16)child_big_rect.left;
        child_rect.top    = (int16)child_big_rect.top;
        child_rect.right  = (int16)child_big_rect.right;
        child_rect.bottom = (int16)child_big_rect.bottom;

        if (!GUI_IsInvalidRect(child_rect))
        {
            IGUICTRL_SetRect(child_ptr->child_ctrl_ptr,&child_rect);
        }

        //get cross rect of the item rect and display rect
        if ((CTRLFORM_IsValidChild(child_ptr)) && 
            (GetCrossRect(&cross_rect,form_ctrl_ptr->relative_disp_rect,child_ptr->relative_rect)))
        {
            CTRLFORM_ClientToScreen(form_ctrl_ptr,&cross_rect);

            //set display rect
            display_rect.left   = (int16)cross_rect.left;
            display_rect.top    = (int16)cross_rect.top;
            display_rect.right  = (int16)cross_rect.right;
            display_rect.bottom = (int16)cross_rect.bottom;
        
            if (GUI_IntersectRect(&intersect_rect,base_ctrl_ptr->display_rect,display_rect))
            {
                is_need_draw = IsUpdateChildCtrl(is_update,&intersect_rect,form_ctrl_ptr);
                IGUICTRL_SetDisplayRect(child_ptr->child_ctrl_ptr,&intersect_rect,is_need_draw);

                if (is_update)
                {
                    //display child dividing line
                    CTRLFORM_DisplayChildLine(form_ctrl_ptr,child_ptr,child_rect);
                }
            }
            else
            {
                //set no display rect
                SetChildNotDisplay(child_ptr);
            }
        
#ifdef TOUCH_PANEL_SUPPORT
            //set tp rect
            SetChildTpRect(form_ctrl_ptr,child_ptr);
#endif
        }
        else
        {
            //set no display rect
            SetChildNotDisplay(child_ptr);
        }
    }
}

/*******************************************************************************************/
//  Description : is update child when write back buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL BOOLEAN IsUpdateChildCtrl(
                                BOOLEAN         is_update,      //in:
                                GUI_RECT_T      *rect_ptr,      //in:
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                )
{
    BOOLEAN     result = TRUE;
    GUI_RECT_T  cur_rect = {0};
    
    if (is_update)
    {
        //如果做了内存回写加速
        if (!GUI_IsZeroRect(form_ctrl_ptr->writeback_rect))
        {
            //if the the item in the write back rect 
            if (GUI_IntersectRect(&cur_rect,*rect_ptr,form_ctrl_ptr->writeback_rect))
            {
                //判断边界item是否是完整的，如果不是完整的需要将拷贝的区域重置
                if (!GUI_RectIsCovered(form_ctrl_ptr->writeback_rect,*rect_ptr))
                {
                    //clear bg in rect cur_rect
                    CTRLFORM_DisplayRect(&cur_rect,form_ctrl_ptr);
                }
                else
                {
                    result = FALSE;
                }
            }
        }
    }
    else
    {
        result = FALSE;
    }
    
    return (result);
}

/*******************************************************************************************/
//  Description : get cross rect by two rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL BOOLEAN GetCrossRect(
                           GUI_BIG_RECT_T   *rect_dest_ptr,
                           GUI_BIG_RECT_T   rect1,
                           GUI_BIG_RECT_T   rect2
                           )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != rect_dest_ptr)
    {
        //get cross rect
        rect_dest_ptr->left   = (int32)(MAX(rect1.left,rect2.left));
        rect_dest_ptr->top    = (int32)(MAX(rect1.top,rect2.top));
        rect_dest_ptr->right  = (int32)(MIN(rect1.right,rect2.right));
        rect_dest_ptr->bottom = (int32)(MIN(rect1.bottom,rect2.bottom));
        
        if ((rect_dest_ptr->left <= rect_dest_ptr->right) &&
            (rect_dest_ptr->top <= rect_dest_ptr->bottom))
        {
            result = TRUE;
        }
    }
    
    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*******************************************************************************************/
//  Description : set child tp rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL BOOLEAN SetChildTpRect(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                             CTRLFORM_CHILD_T   *child_ptr      //in:
                             )
{
    BOOLEAN         result = FALSE;
    CAF_GUID_T      guid = 0;
    GUI_RECT_T      tp_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //unit form,only one get active child
    if (PNULL != form_ctrl_ptr &&
        (CTRLFORM_IsUnitForm(form_ctrl_ptr)) && 
        (1 == form_ctrl_ptr->active_child_num) &&
        (child_ptr == form_ctrl_ptr->active_child_ptr))
    {
        //set tp rect
        tp_rect = base_ctrl_ptr->display_rect;
        
        //get guid
        guid = IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr);
        switch (guid)
        {
        case SPRD_GUI_LABEL_ID:
            result = GUILABEL_SetTpRect(child_ptr->ctrl_handle,&tp_rect);
            break;
            
        case SPRD_GUI_SETLIST_ID:
            result = GUISETLIST_SetTpRect(child_ptr->ctrl_handle,&tp_rect);
            break;
            
        default:
            result = FALSE;
            break;
        }
    }
    
    return (result);
}
#endif

/*******************************************************************************************/
//  Description : set child no display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL void SetChildNotDisplay(
                              CTRLFORM_CHILD_T   *child_ptr  //in:
                              )
{
    GUI_RECT_T      display_rect = {0};
#ifdef TOUCH_PANEL_SUPPORT
    CAF_GUID_T      guid = 0;
    GUI_RECT_T      tp_rect = {0};
#endif

    if (PNULL != child_ptr)
    {
        //set no display rect
        IGUICTRL_SetDisplayRect(child_ptr->child_ctrl_ptr,&display_rect,FALSE);
    
#ifdef TOUCH_PANEL_SUPPORT
        //get guid
        guid = IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr);
        switch (guid)
        {
        case SPRD_GUI_LABEL_ID:
            GUILABEL_SetTpRect(child_ptr->ctrl_handle,&tp_rect);
            break;
        
        case SPRD_GUI_SETLIST_ID:
            GUISETLIST_SetTpRect(child_ptr->ctrl_handle,&tp_rect);
            break;
        
        default:
            break;
        }
#endif
    }
}

/*****************************************************************************/
//  Description : layout child form
//  Global resource dependence : 
//  Author: Jassmine
//  Note: when child form height is fixed/percent/left,no layout form
//        need set form rect first, than layout, for edit line change
/*****************************************************************************/
LOCAL void LayoutChildForm(
                           CTRLFORM_OBJ_T *form_ctrl_ptr  //in
                           )
{
#ifdef GUIF_FORM_DOCKABLE
    uint16              cur_priority = 0;
#endif
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //layout
    if (form_ctrl_ptr->layout_index != form_ctrl_ptr->child_num)
    {
        CTRLFORM_Layout(form_ctrl_ptr,TRUE,FALSE);
    }
    
#ifdef GUIF_FORM_DOCKABLE
    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        if ((CTRLFORM_IsValidChild(child_ptr)) && 
            (CTRLFORM_IsFormChild(child_ptr)))
        {
            LayoutChildForm((CTRLFORM_OBJ_T*)child_ptr->child_ctrl_ptr);
        }

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }
#endif

    //other child
    child_ptr = form_ctrl_ptr->first_child_ptr;
    while (PNULL != child_ptr)
    {
        if ((CTRLFORM_IsValidChild(child_ptr)) && 
            (CTRLFORM_IsFormChild(child_ptr)))
        {
            LayoutChildForm((CTRLFORM_OBJ_T*)child_ptr->child_ctrl_ptr);
        }

        child_ptr = child_ptr->next_ptr;
    }
}

/*****************************************************************************/
//  Description : get form height by width,include border,padding etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetHeightByWidth(
                                         CTRLBASE_OBJ_T *base_ctrl_ptr, //in:
                                         uint16         width,          //in:
                                         uint16         *height_ptr     //in/out:
                                         )
{
    BOOLEAN         result = TRUE;
#ifdef GUIF_FORM_DOCKABLE
    uint16          child_height = 0;
#endif
    GUI_RECT_T      form_rect = {0};
    CTRLFORM_OBJ_T  *form_ctrl_ptr = PNULL;

    if ((PNULL != base_ctrl_ptr) && 
        (PNULL != height_ptr))
    {
        form_ctrl_ptr = (CTRLFORM_OBJ_T *)base_ctrl_ptr;
    
        //init rect
        SCI_MEMSET(&base_ctrl_ptr->rect,0,sizeof(GUI_RECT_T));
        SCI_MEMSET(&form_ctrl_ptr->relative_rect,0,sizeof(GUI_BIG_RECT_T));

        //set form rect right
        base_ctrl_ptr->rect.right = (int16)(base_ctrl_ptr->rect.left + width - 1);

        //get child index
        CTRLFORM_GetValidChild(form_ctrl_ptr);

        //re-layout
        form_ctrl_ptr->layout_index = 0;

        //destroy progress
        DestroyPrgCtrl(form_ctrl_ptr);

        //layout all child control
        LayoutAllChildCtrl(form_ctrl_ptr);

        //set rect
        form_rect.right  = (int16)(width - 1);
        form_rect.bottom = (int16)(form_ctrl_ptr->relative_rect.bottom);
        
#ifdef GUIF_FORM_DOCKABLE
        //add bottom child
        child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->bottom_child_ptr);
        form_rect.bottom = (uint16)(form_rect.bottom + child_height);
#endif

        //add softkey
        if (CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr))
        {
            form_rect.bottom = (uint16)(form_rect.bottom + form_ctrl_ptr->softkey_child_ptr->height);
        }
        
        //add border
        if (GUI_BORDER_NONE != base_ctrl_ptr->border.type)
        {
            form_rect.bottom = (int16)(form_rect.bottom + (base_ctrl_ptr->border.width<<1));
        }

        //add padding
        form_rect.bottom = (int16)(form_rect.bottom + base_ctrl_ptr->padding.top + base_ctrl_ptr->padding.bottom);

        //adjust height
        if (form_rect.bottom - form_rect.top + 1 < form_ctrl_ptr->size_range.min_size)
        {
            form_rect.bottom = (int16)(form_ctrl_ptr->size_range.min_size + form_rect.top - 1);
        }
        else if (form_rect.bottom - form_rect.top + 1 > form_ctrl_ptr->size_range.max_size)
        {
            form_rect.bottom = (int16)(form_ctrl_ptr->size_range.max_size + form_rect.top - 1);
        }

        IGUICTRL_SetRect((IGUICTRL_T *)form_ctrl_ptr,&form_rect);
    
        //layout end
        form_ctrl_ptr->layout_index = form_ctrl_ptr->all_child_num;

        //set form height
        *height_ptr = (uint16)(form_rect.bottom - form_rect.top + 1);
    }

    return (result);
}

