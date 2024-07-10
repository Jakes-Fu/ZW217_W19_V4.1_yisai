/*****************************************************************************
** File Name:      ctrlform_calc.c                                           *
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
#include "mmi_gui_trc.h"
#include "ctrlform_export.h"
#include "ctrlform.h"

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
#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : reset all dock child width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetDockChild(
                          CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                          );

/*****************************************************************************/
//  Description : calculate dock child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcDockChild(
                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in/out:
                         CTRLFORM_CHILD_T   *child_ptr      //in/out:
                         );
#endif

/*****************************************************************************/
//  Description : get all child horizontal or vertical space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAllChildSpace(
                              BOOLEAN           is_ver,         //in:
                              CTRLFORM_CHILD_T  *first_child_ptr//in:
                              );

/*****************************************************************************/
//  Description : get display width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetDisplayWidthHeight(
                                 uint16             *width_ptr,     //in/out:
                                 uint16             *height_ptr,    //in/out:
                                 uint32             all_space,      //in:
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : calculate all child control width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildWidthHeight(//is create prg
                                        uint32              all_space,      //in:order:ver space;sbs:hor space
                                        CTRLFORM_OBJ_T      *form_ctrl_ptr, //in/out:
                                        CTRLFORM_CHILD_T    *first_child_ptr//in:
                                        );

/*****************************************************************************/
//  Description : calculate child width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildWidth(//is calculate
                                  uint16            display_width,      //in:display width without space
                                  uint16            *auto_num_ptr,      //in/out:calculated child num
                                  uint32            all_child_width,    //in:calculated all child width
                                  CTRLFORM_CHILD_T  *child_ptr,         //in/out:
                                  CTRLFORM_CHILD_T  **left_child_pptr,  //in/out:not calculate child
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr      //in/:
                                  );

/*****************************************************************************/
//  Description : calculate child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildHeight(//is calculate
                                   uint16           display_height,     //in:display height without space
                                   CTRLFORM_CHILD_T *child_ptr,         //in/out:
                                   CTRLFORM_CHILD_T **left_child_pptr,  //in/out:not calculate child
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr      //in/:
                                   );

/*****************************************************************************/
//  Description : is create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCreatePrgCtrl(//is create
                              uint32            all_space,          //in:
                              uint32            all_child_height,   //in:
                              CTRLFORM_OBJ_T    *form_ctrl_ptr      //in:
                              );

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* CreatePrgCtrl(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : calculate left width child's width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcLeftWidthChild(
                              uint16            display_width,          //in:
                              uint16            display_height,         //in:
                              uint32            *all_width_ptr,         //in/out:
                              uint32            *all_height_ptr,        //in/out:
                              CTRLFORM_OBJ_T    *form_ctrl_ptr,         //in/out:
                              CTRLFORM_CHILD_T  *left_child_ptr,        //in/out:
                              CTRLFORM_CHILD_T  **left_height_child_pptr//in/out:
                              );

/*****************************************************************************/
//  Description : calculate left height child's width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcLeftHeightChild(
                               uint16            display_height, //in:
                               uint32            *all_height_ptr,//in/out:
                               CTRLFORM_OBJ_T    *form_ctrl_ptr, //in/out:
                               CTRLFORM_CHILD_T  *left_child_ptr //in/out:
                               );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : calculate all dock child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_CalcAllDockChild(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                      )
{
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //reset dock child width and height
    ResetDockChild(form_ctrl_ptr);

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            //calculate dock child width and height
            CalcDockChild(form_ctrl_ptr,child_ptr);
        }

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : reset all dock child width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetDockChild(
                          CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                          )
{
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        //reset width and height
        child_ptr->width  = 0;
        child_ptr->height = 0;

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : calculate dock child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcDockChild(
                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in/out:
                         CTRLFORM_CHILD_T   *child_ptr      //in/out:
                         )
{
    BOOLEAN             result = FALSE;
    uint16              display_width = 0;
    uint16              display_height = 0;
    uint16              calc_auto_num = 0;
    uint16              child_height = 0;
    uint32              all_child_width = 0;
    uint32              all_child_height = 0;
    CTRLFORM_CHILD_T    *left_width_child_ptr = PNULL;
    CTRLFORM_CHILD_T    *left_height_child_ptr = PNULL;

    if (CTRLFORM_IsValidChild(child_ptr))
    {
        //get display width and height
        GetDisplayWidthHeight(&display_width,&display_height,0,form_ctrl_ptr);

        //set child param
        CTRLFORM_SetChildParam(form_ctrl_ptr,child_ptr);

        //calculate child width
        if (CalculateChildWidth(display_width,
                &calc_auto_num,
                0,
                child_ptr,
                &left_width_child_ptr,
                form_ctrl_ptr))
        {
            //calculate child height
            if (CalculateChildHeight(display_height,
                    child_ptr,
                    &left_height_child_ptr,
                    form_ctrl_ptr))
            {
                //adjust child height
                child_height = child_ptr->height;
                CTRLFORM_AdjustHeightByRange(&child_height,&child_ptr->size_range);

                //set max child height
                form_ctrl_ptr->child_max_height = (uint16)MAX(form_ctrl_ptr->child_max_height,child_height);

                result = TRUE;
            }
        }

        if ((!result) && 
            (PNULL != left_width_child_ptr))
        {
            //calculate left width child's width/height
            CalcLeftWidthChild(display_width,
                display_height,
                &all_child_width,
                &all_child_height,
                form_ctrl_ptr,
                left_width_child_ptr,
                &left_height_child_ptr);
        }

        if ((!result) && 
            (PNULL != left_height_child_ptr))
        {
            //calculate left height child's width/height
            CalcLeftHeightChild(display_height,
                &all_child_height,
                form_ctrl_ptr,
                left_height_child_ptr);
        }
    }
}
#endif

/*****************************************************************************/
//  Description : calculate all child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CalcAllChild(//is create prg
                                     CTRLFORM_OBJ_T *form_ctrl_ptr  //in/out:
                                     )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_ver = FALSE;
    uint32              all_space = 0;
    CTRLFORM_CHILD_T    *first_child_ptr = PNULL;

    //get first child pointer
    CTRLFORM_GetChildPtrByIndex(form_ctrl_ptr->first_valid_index,form_ctrl_ptr,&first_child_ptr);
    if (PNULL != first_child_ptr)
    {
        //get all child space
        is_ver    = CTRLFORM_IsOrderForm(form_ctrl_ptr);
        all_space = GetAllChildSpace(is_ver,first_child_ptr);

        //calculate child width and height
        result = CalculateChildWidthHeight(all_space,form_ctrl_ptr,first_child_ptr);
        if (result)
        {
            //set layout start index
            form_ctrl_ptr->layout_index = 0;

            //calculate child width and height
            CalculateChildWidthHeight(all_space,form_ctrl_ptr,first_child_ptr);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get all child horizontal or vertical space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAllChildSpace(
                              BOOLEAN           is_ver,         //in:
                              CTRLFORM_CHILD_T  *first_child_ptr//in:
                              )
{
    uint32              all_space = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    
    //set child
    child_ptr = first_child_ptr;
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            if (is_ver)
            {
                all_space = all_space + child_ptr->ver_space;
            }
            else
            {
                all_space = all_space + child_ptr->hor_space;
            }
        }
        
        child_ptr = child_ptr->next_ptr;
    }
    
    if (0 < all_space)
    {
        //reduce the first child space
        if (is_ver)
        {
            all_space = all_space - first_child_ptr->ver_space;
        }
        else
        {
            all_space = all_space - first_child_ptr->hor_space;
        }
    }
    
    return (all_space);
}

/*****************************************************************************/
//  Description : calculate all child control width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildWidthHeight(//is create prg
                                        uint32              all_space,      //in:order:ver space;sbs:hor space
                                        CTRLFORM_OBJ_T      *form_ctrl_ptr, //in/out:
                                        CTRLFORM_CHILD_T    *first_child_ptr//in:
                                        )
{
    BOOLEAN             result = FALSE;
    uint16              display_width = 0;
    uint16              display_height = 0;
    uint16              calc_auto_num = 0;
    uint16              child_height = 0;
    uint32              all_child_width = 0;
    uint32              all_child_height = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    CTRLFORM_CHILD_T    *left_width_child_ptr = PNULL;
    CTRLFORM_CHILD_T    *left_height_child_ptr = PNULL;
    
    //get display width and height
    GetDisplayWidthHeight(&display_width,&display_height,all_space,form_ctrl_ptr);
    
    //set child
    child_ptr = first_child_ptr;
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            //set child param
            CTRLFORM_SetChildParam(form_ctrl_ptr,child_ptr);
            
            //calculate child width
            if (CalculateChildWidth(display_width,
                    &calc_auto_num,
                    all_child_width,
                    child_ptr,
                    &left_width_child_ptr,
                    form_ctrl_ptr))
            {
                all_child_width = all_child_width + child_ptr->width;
                
                //calculate child height
                if (CalculateChildHeight(display_height,
                        child_ptr,
                        &left_height_child_ptr,
                        form_ctrl_ptr))
                {
                    //adjust child height
                    child_height = child_ptr->height;
                    CTRLFORM_AdjustHeightByRange(&child_height,&child_ptr->size_range);

                    //set max child height
                    form_ctrl_ptr->child_max_height = (uint16)MAX(form_ctrl_ptr->child_max_height,child_height);
                    
                    //set all child height
                    all_child_height = (uint32)(all_child_height + child_ptr->height);
                    
                    //is create progress control
                    if (IsCreatePrgCtrl(all_space,all_child_height,form_ctrl_ptr))
                    {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
        
        child_ptr = child_ptr->next_ptr;
    }
    
    if ((!result) && 
        (PNULL != left_width_child_ptr))
    {
        //calculate left width child's width/height
        CalcLeftWidthChild(display_width,
            display_height,
            &all_child_width,
            &all_child_height,
            form_ctrl_ptr,
            left_width_child_ptr,
            &left_height_child_ptr);

        //is create progress control
        if (IsCreatePrgCtrl(all_space,all_child_height,form_ctrl_ptr))
        {
            result = TRUE;
        }
    }

    if ((!result) && 
        (PNULL != left_height_child_ptr))
    {
        //calculate left height child's width/height
        CalcLeftHeightChild(display_height,
            &all_child_height,
            form_ctrl_ptr,
            left_height_child_ptr);

        //is create progress control
        if (IsCreatePrgCtrl(all_space,all_child_height,form_ctrl_ptr))
        {
            result = TRUE;
        }
    }

    //all sbs child width
    form_ctrl_ptr->sbs_width = (uint16)(all_child_width + all_space);
    
    return (result);
}

/*****************************************************************************/
//  Description : get display width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetDisplayWidthHeight(
                                 uint16             *width_ptr,     //in/out:
                                 uint16             *height_ptr,    //in/out:
                                 uint32             all_space,      //in:
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                 )
{
    BOOLEAN     is_except_prg = FALSE;
    
    if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
    {
        is_except_prg = TRUE;
    }
    *width_ptr  = CTRLFORM_GetDisplayWidth(is_except_prg,form_ctrl_ptr);
    *height_ptr = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
    
    //is order form
    if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
    {
        //order,height - all space
        if (*height_ptr > all_space)
        {
            *height_ptr = (uint16)(*height_ptr - all_space);
        }
    }
    else
    {
        //sbs,width - all space
        if (*width_ptr > all_space)
        {
            *width_ptr = (uint16)(*width_ptr - all_space);
        }
    }
}

/*****************************************************************************/
//  Description : calculate child width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildWidth(//is calculate
                                  uint16            display_width,      //in:display width without space
                                  uint16            *auto_num_ptr,      //in/out:calculated child num
                                  uint32            all_child_width,    //in:calculated all child width
                                  CTRLFORM_CHILD_T  *child_ptr,         //in/out:
                                  CTRLFORM_CHILD_T  **left_child_pptr,  //in/out:not calculate child
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr      //in/:
                                  )
{
    BOOLEAN     result = TRUE;
    
    switch (child_ptr->width_info.type)
    {
    case GUIFORM_CHILD_WIDTH_AUTO:
        if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
        {
            child_ptr->width = display_width;
        }
        else
        {
            if (PNULL == *left_child_pptr)
            {
                *left_child_pptr = child_ptr;
                
                result = FALSE;
            }
            else
            {
                (*auto_num_ptr)++;
                child_ptr->width = (uint16)((display_width * (*auto_num_ptr))/form_ctrl_ptr->child_num - all_child_width);
            }
        }
        break;
        
    case GUIFORM_CHILD_WIDTH_FIXED:
        child_ptr->width = child_ptr->width_info.add_data;
        break;
        
    case GUIFORM_CHILD_WIDTH_PERCENT:
        child_ptr->width = (uint16)(display_width * child_ptr->width_info.add_data / CTRLFORM_PERCENT_MAX);
        break;
        
    default:
        SCI_PASSERT(FALSE,("CalculateChildWidth:width type %d is error!",child_ptr->width_info.type)); /*assert verified*/
        break;
    }
    
    //unit form child indentation
    if ((child_ptr->index > form_ctrl_ptr->first_valid_index) &&
        (CTRLFORM_IsOrderForm(form_ctrl_ptr)) &&
        (CTRLFORM_IsUnitForm(form_ctrl_ptr)) &&
        ((GUIFORM_CHILD_ALIGN_NONE == child_ptr->align) || (GUIFORM_CHILD_ALIGN_LEFT == child_ptr->align)))
    {
        //calculate indentation width
        child_ptr->indent_width = (uint16)(child_ptr->width * form_ctrl_ptr->theme.indent_ratio.num1 / form_ctrl_ptr->theme.indent_ratio.num2);
        
        //adjust child width
        child_ptr->width = (uint16)(child_ptr->width - child_ptr->indent_width);
    }
    
#ifdef CTRLFORM_DRUMP_LAYOUT_INFO
    if (result)
    {
        // 必须要与GUIFORM_CHILD_WIDTH_E一致
        char* width_info[GUIFORM_CHILD_WIDTH_MAX] = 
        {
            "GUIFORM_CHILD_WIDTH_AUTO",
                "GUIFORM_CHILD_WIDTH_FIXED",
                "GUIFORM_CHILD_WIDTH_PERCENT",
        };
        
        CTRLFORM_TRACE_LAYOUT(
            "^^^ CalculateChildWidth: ctrl_ptr=0x%08x, width_info=%s, width=%d",
            child_ptr->child_ctrl_ptr,
            width_info[child_ptr->width_info.type],
            child_ptr->width);
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : calculate child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateChildHeight(//is calculate
                                   uint16           display_height,     //in:display height without space
                                   CTRLFORM_CHILD_T *child_ptr,         //in/out:
                                   CTRLFORM_CHILD_T **left_child_pptr,  //in/out:not calculate child
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr      //in/:
                                   )
{
    BOOLEAN     result = TRUE;
    uint16      child_width = 0;
    uint16      child_height = 0;
    uint16      prg_width = 0;
    
    //dock top/bottom child not support left
    if (((GUIFORM_CHILD_DOCK_TOP == child_ptr->dock_type) || (GUIFORM_CHILD_DOCK_BOTTOM == child_ptr->dock_type)) && 
        ((GUIFORM_CHILD_HEIGHT_LEFT == child_ptr->height_info.type) ||(GUIFORM_CHILD_HEIGHT_LEFT_MIN == child_ptr->height_info.type)))
    {
        SCI_TRACE_LOW("CalculateChildHeight:child is dock top or bottom,not support left type!");
        child_ptr->height_info.type = GUIFORM_CHILD_HEIGHT_AUTO;
    }

    switch (child_ptr->height_info.type)
    {
    case GUIFORM_CHILD_HEIGHT_AUTO:
    case GUIFORM_CHILD_HEIGHT_LEFT_MIN:
        //get control height by width
        child_width = child_ptr->width;
        if (IGUICTRL_GetHeightByWidth(child_ptr->child_ctrl_ptr,child_width,&child_height))
        {
            child_ptr->height = child_height;
        }
        else
        {
            //动画控件可能有滚动条后,高度变小,反而没有滚动条
            if ((PNULL != form_ctrl_ptr->prgbox_ctrl_ptr) &&
                (CTRLFORM_IsAnimChild(child_ptr)))
            {
                //get progress width
                prg_width = CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
                
                if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
                {
                    child_width = (uint16)(child_width + prg_width);
                }
                else
                {
                    switch (child_ptr->width_info.type)
                    {
                    case GUIFORM_CHILD_WIDTH_AUTO:
                        child_width = (uint16)(child_width + prg_width);
                        break;
                        
                    case GUIFORM_CHILD_WIDTH_PERCENT:
                        child_width = (uint16)(child_width + (prg_width * child_ptr->width_info.add_data / CTRLFORM_PERCENT_MAX) + 1);
                        break;
                        
                    default:
                        break;
                    }
                    break;
                }
            }
            
            //get child height
            child_ptr->height = (uint16)(child_width * form_ctrl_ptr->theme.img_ratio.num1 / form_ctrl_ptr->theme.img_ratio.num2);
        }
        
        if (GUIFORM_CHILD_HEIGHT_LEFT_MIN == child_ptr->height_info.type)
        {
            if (PNULL == *left_child_pptr)
            {
                *left_child_pptr = child_ptr;
            }
            else
            {
                SCI_PASSERT(FALSE,("CalculateChildHeight: only one child height type is left!")); /*assert verified*/
            }
        }
        break;
        
    case GUIFORM_CHILD_HEIGHT_FIXED:
        child_ptr->height = child_ptr->height_info.add_data;
        break;
        
    case GUIFORM_CHILD_HEIGHT_PERCENT:
        child_ptr->height = (uint16)(display_height * child_ptr->height_info.add_data / CTRLFORM_PERCENT_MAX);
        break;
        
    case GUIFORM_CHILD_HEIGHT_LEFT:
        if (PNULL == *left_child_pptr)
        {
            *left_child_pptr = child_ptr;
        }
        else
        {
            SCI_PASSERT(FALSE,("CalculateChildHeight: only one child height type is left!")); /*assert verified*/
        }
        result = FALSE;
        break;
        
    default:
        //SCI_TRACE_LOW:"CalChildHeight: height type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_CALC_675_112_2_18_3_18_24_159,(uint8*)"d",child_ptr->height_info.type);
        result = FALSE;
        break;
    }
    
#ifdef CTRLFORM_DRUMP_LAYOUT_INFO
    if (result)
    {
        // 必须要与GUIFORM_CHILD_HEIGHT_E一致
        char* height_info[GUIFORM_CHILD_HEIGHT_MAX] = 
        {
            "GUIFORM_CHILD_HEIGHT_AUTO",
                "GUIFORM_CHILD_HEIGHT_FIXED",
                "GUIFORM_CHILD_HEIGHT_PERCENT",
                "GUIFORM_CHILD_HEIGHT_LEFT",
                "GUIFORM_CHILD_HEIGHT_LEFT_MIN",
        };
        
        CTRLFORM_TRACE_LAYOUT(
            "^^^ CalculateChildHeight: ctrl_ptr=0x%08x, height_info=%s, height=%d",
            child_ptr->child_ctrl_ptr,
            height_info[child_ptr->height_info.type],
            child_ptr->height);
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : is create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCreatePrgCtrl(//is create
                              uint32            all_space,          //in:
                              uint32            all_child_height,   //in:
                              CTRLFORM_OBJ_T    *form_ctrl_ptr      //in:
                              )
{
    BOOLEAN     result = FALSE;
    uint16      display_height = 0;
    uint32      form_height = 0;
    
    if (form_ctrl_ptr->is_display_prg)
    {
        //set form height
        if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
        {
            form_height = (uint16)(all_child_height + all_space);
        }
        else
        {
            form_height = form_ctrl_ptr->child_max_height;
        }
        
        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
        if ((0 == display_height) &&
            (0xFFFF != form_ctrl_ptr->size_range.max_size))
        {
            display_height = form_ctrl_ptr->size_range.max_size;
        }

        //is need progress
        if ((0 < display_height) &&
            (form_height > display_height) && 
            (PNULL == form_ctrl_ptr->prgbox_ctrl_ptr))
        {
            //create progress bar control
            form_ctrl_ptr->prgbox_ctrl_ptr = CreatePrgCtrl(form_ctrl_ptr);
    
            result = TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* CreatePrgCtrl(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                )
{
    GUI_RECT_T              prg_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    IGUICTRL_T              *prgbox_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    
    //get progress bar rect
    prg_rect = CTRLFORM_GetScrollBarRect(form_ctrl_ptr);
    
    //init data
    init_data.lcd_id = base_ctrl_ptr->lcd_dev_info.lcd_id;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = prg_rect;
    init_data.style  = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
    init_data.is_forbid_paint = TRUE;
    
    //create progress
    ctrl_create.guid               = SPRD_GUI_PRGBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = base_ctrl_ptr->handle;
    ctrl_create.parent_win_handle  = form_ctrl_ptr->win_handle;
    
    //create progress bar control
    prgbox_ctrl_ptr = MMK_CreateControl(&ctrl_create);
    
    return (prgbox_ctrl_ptr);
}

/*****************************************************************************/
//  Description : calculate left width child's width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcLeftWidthChild(
                              uint16            display_width,          //in:
                              uint16            display_height,         //in:
                              uint32            *all_width_ptr,         //in/out:
                              uint32            *all_height_ptr,        //in/out:
                              CTRLFORM_OBJ_T    *form_ctrl_ptr,         //in/out:
                              CTRLFORM_CHILD_T  *left_child_ptr,        //in/out:
                              CTRLFORM_CHILD_T  **left_height_child_pptr//in/out:
                              )
{
    uint16      child_height = 0;

    switch (form_ctrl_ptr->layout_type)
    {
    case GUIFORM_LAYOUT_ORDER:
        left_child_ptr->width = display_width;

        *all_width_ptr = (uint16)(*all_width_ptr + left_child_ptr->width);

        //calculate child width
        if (CalculateChildHeight(display_height,
                left_child_ptr,
                left_height_child_pptr,
                form_ctrl_ptr))
        {
            //adjust child height
            child_height = left_child_ptr->height;
            CTRLFORM_AdjustHeightByRange(&child_height,&left_child_ptr->size_range);

            //set all child height
            *all_height_ptr = (uint16)(*all_height_ptr + child_height);
        }
        break;

    case GUIFORM_LAYOUT_SBS:
        if (display_width >= *all_width_ptr)
        {
            left_child_ptr->width = (uint16)(display_width - *all_width_ptr);

            *all_width_ptr = (uint16)(*all_width_ptr + left_child_ptr->width);

            //calculate child width
            if (CalculateChildHeight(display_height,
                    left_child_ptr,
                    left_height_child_pptr,
                    form_ctrl_ptr))
            {
                //adjust child height
                child_height = left_child_ptr->height;
                CTRLFORM_AdjustHeightByRange(&child_height,&left_child_ptr->size_range);

                //set all child height
                *all_height_ptr = (uint16)(*all_height_ptr + child_height);
            }
        }
        break;

    default:
        SCI_TRACE_LOW("CalcLeftWidthChild:layout_type %d is error!",form_ctrl_ptr->layout_type);
        break;
    }

    //set max child height
    form_ctrl_ptr->child_max_height = (uint16)MAX(form_ctrl_ptr->child_max_height,child_height);

#ifdef GUIFORM_DRUMP_LAYOUT_INFO
        {
            // 必须要与GUIFORM_CHILD_WIDTH_E一致
            char* width_info[GUIFORM_CHILD_WIDTH_MAX] = 
            {
                "GUIFORM_CHILD_WIDTH_AUTO",
                "GUIFORM_CHILD_WIDTH_FIXED",
                "GUIFORM_CHILD_WIDTH_PERCENT",
            };

            GUIFORM_TRACE_LAYOUT(
                "^^^ CalculateChildWidth: ctrl_ptr=0x%08x, width_info=%s, width=%d",
                    left_child_ptr->child_ctrl_ptr,
                    width_info[left_child_ptr->width_info.type],
                    left_child_ptr->width);
        }
#endif
}

/*****************************************************************************/
//  Description : calculate left height child's width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CalcLeftHeightChild(
                               uint16            display_height, //in:
                               uint32            *all_height_ptr,//in/out:
                               CTRLFORM_OBJ_T    *form_ctrl_ptr, //in/out:
                               CTRLFORM_CHILD_T  *left_child_ptr //in/out:
                               )
{
    uint16      child_height = 0;

    switch (form_ctrl_ptr->layout_type)
    {
    case GUIFORM_LAYOUT_ORDER:
        //calculate left child height
        if (display_height >= *all_height_ptr)
        {
            //set all child height
            if ((GUIFORM_CHILD_HEIGHT_LEFT_MIN == left_child_ptr->height_info.type) &&
                (*all_height_ptr >= left_child_ptr->height))
            {
                *all_height_ptr = (uint16)(*all_height_ptr - left_child_ptr->height);
            }

            //set left child height
            left_child_ptr->height = (uint16)(display_height - *all_height_ptr);

            //adjust child height
            child_height = left_child_ptr->height;
            CTRLFORM_AdjustHeightByRange(&child_height,&left_child_ptr->size_range);

            //set all child height
            *all_height_ptr = (uint16)(*all_height_ptr + child_height);
        }
        break;

    case GUIFORM_LAYOUT_SBS:
        if ((GUIFORM_CHILD_HEIGHT_LEFT_MIN == left_child_ptr->height_info.type) && 
            (left_child_ptr->height >= display_height))
        {
            //do nothing
        }
        else
        {
            left_child_ptr->height = display_height;

            //adjust child height
            child_height = left_child_ptr->height;
            CTRLFORM_AdjustHeightByRange(&child_height,&left_child_ptr->size_range);
        }

        //set all child height
        *all_height_ptr = (uint16)MAX(*all_height_ptr,child_height);
        break;

    default:
        SCI_TRACE_LOW("CalcLeftHeightChild:layout_type %d is error!",form_ctrl_ptr->layout_type);
        break;
    }

    //set max child height
    form_ctrl_ptr->child_max_height = (uint16)MAX(form_ctrl_ptr->child_max_height,child_height);

#ifdef GUIFORM_DRUMP_LAYOUT_INFO
        {
            // 必须要与GUIFORM_CHILD_HEIGHT_E一致
            char* height_info[GUIFORM_CHILD_HEIGHT_MAX] = 
            {
                "GUIFORM_CHILD_HEIGHT_AUTO",
                "GUIFORM_CHILD_HEIGHT_FIXED",
                "GUIFORM_CHILD_HEIGHT_PERCENT",
                "GUIFORM_CHILD_HEIGHT_LEFT",
                "GUIFORM_CHILD_HEIGHT_LEFT_MIN",
            };

            GUIFORM_TRACE_LAYOUT(
                "^^^ CalculateChildHeight: ctrl_ptr=0x%08x, height_info=%s, height=%d",
                    left_child_ptr->child_ctrl_ptr,
                    height_info[left_child_ptr->height_info.type],
                    child_height);
        }
#endif
}
