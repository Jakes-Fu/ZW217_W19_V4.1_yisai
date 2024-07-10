#ifdef GUIF_ICONFOLDER
/*****************************************************************************
** File Name:      guiiconfolder_internal.c                                  *
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
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "guicommon.h"
#include "guianim.h"
#include "ctrliconfolder.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IconFolderTypeOf(
                               CTRLBASE_OBJ_T   *iconfolder_ptr
                               );

/*****************************************************************************/
//  Description : get icon folder rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIconFolderRect(
                                   CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr
                                   );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon folder pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLICONFOLDER_OBJ_T* ICONFOLDER_GetPtr(
                                                  MMI_HANDLE_T  ctrl_handle
                                                  )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (IconFolderTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLICONFOLDER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IconFolderTypeOf(
                               CTRLBASE_OBJ_T   *iconfolder_ptr
                               )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)iconfolder_ptr, CTRL_ICONFOLDER_TYPE); 
}

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONFOLDER_GetIconRect(
                                            uint16                  index,              //in:
                                            CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr//in:
                                            )
{
    uint16          icon_width = 0;
    uint16          icon_height = 0;
    uint16          rotate_angle = 0;
    GUI_RECT_T      icon_rect = {0};
    GUI_RECT_T      iconfolder_rect = {0};

    if (index < iconfolder_ctrl_ptr->total_num)
    {
        //get icon folder rect
        iconfolder_rect = GetIconFolderRect(iconfolder_ctrl_ptr);

        //get src icon width,height and angle
        icon_width   = iconfolder_ctrl_ptr->theme.icon_width;
        icon_height  = iconfolder_ctrl_ptr->theme.icon_height;
        rotate_angle = ICONFOLDER_GetIconAngle(index,iconfolder_ctrl_ptr);

        //get icon width and height after rotate
        GUIANIM_GetRotateSize(&icon_width,&icon_height,rotate_angle);

        //get icon rect
        icon_rect = MMITHEME_GetIconRect(index,
                        icon_width,
                        icon_height,
                        iconfolder_rect);
    }

    return (icon_rect);
}
/*****************************************************************************/
//  Description : get icon folder rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIconFolderRect(
                                   CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr
                                   )
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      iconfolder_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;
    
    //icon folder is exist border
    if (GUI_BORDER_NONE == iconfolder_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    if ((is_border) &&
        (!GUI_IsRectEmpty(base_ctrl_ptr->rect)))
    {
        iconfolder_rect.left   = (int16)(base_ctrl_ptr->rect.left + iconfolder_ctrl_ptr->theme.border.width);
        iconfolder_rect.right  = (int16)(base_ctrl_ptr->rect.right - iconfolder_ctrl_ptr->theme.border.width);
        iconfolder_rect.top    = (int16)(base_ctrl_ptr->rect.top + iconfolder_ctrl_ptr->theme.border.width);
        iconfolder_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - iconfolder_ctrl_ptr->theme.border.width);
    }
    else
    {
        iconfolder_rect = base_ctrl_ptr->rect;
    }
    SCI_ASSERT(iconfolder_rect.top <= iconfolder_rect.bottom); /*assert verified*/

    return (iconfolder_rect);
}

/*****************************************************************************/
//  Description : get icon angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetIconAngle(
                                         uint16                 index,              //in:
                                         CTRLICONFOLDER_OBJ_T   *iconfolder_ctrl_ptr//in:
                                         )
{
    uint16      rotate_angle = 0;

    if (index < iconfolder_ctrl_ptr->total_num)
    {
        //get rotate angle
        rotate_angle = MMITHEME_GetIconAngle(index);
    }

    return (rotate_angle);
}

/*****************************************************************************/
//  Description : 获取总个数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetTotalNum(
                                        CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                                        )
{
    uint16      total_num = 0;

    if (PNULL != iconfolder_ctrl_ptr)
    {
        total_num = iconfolder_ctrl_ptr->total_num;
    }

    return total_num;
}

/*****************************************************************************/
//  Description : 获取指定的动画控件
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASE_OBJ_T* ICONFOLDER_GetAnimCtrl(
                                        CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr,//in:
                                        uint16          index
                                        )
{
    CTRLBASE_OBJ_T          *anim_ctrl_ptr   = PNULL;

    if (PNULL != iconfolder_ctrl_ptr
        && PNULL != iconfolder_ctrl_ptr->item_ptr
        && index < iconfolder_ctrl_ptr->total_num)
    {
        anim_ctrl_ptr = (CTRLBASE_OBJ_T*)iconfolder_ctrl_ptr->item_ptr[index].icon.anim_ctrl_add;
    }

    return anim_ctrl_ptr;
}

/*****************************************************************************/
//  Description : 获取最大个数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetMaxIconNum(
                                          CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                                          )
{
    uint16  max_icon_num = 0;

    if (PNULL != iconfolder_ctrl_ptr)
    {
        max_icon_num = iconfolder_ctrl_ptr->theme.icon_max_num;
    }

    return max_icon_num;
}

#endif

