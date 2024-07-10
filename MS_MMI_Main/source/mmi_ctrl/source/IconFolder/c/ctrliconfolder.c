/*****************************************************************************
** File Name:      ctrliconfolder.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

#ifdef GUIF_ICONFOLDER

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
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
//  Description : init iconfolder class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlInitVtbl(
    CTRLICONFOLDER_VTBL_T        *iconfolder_vtbl_ptr
);

/*****************************************************************************/
//  Description : iconfolder construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLICONFOLDER_INIT_PARAM_T   *iconfolder_param_ptr
);

/*****************************************************************************/
//  Description : iconfolder destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlPackInitParam (
    CTRLICONFOLDER_INIT_DATA_T     *iconfolder_init_ptr,   //in
    CTRLICONFOLDER_INIT_PARAM_T   *iconfolder_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle iconfolder msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IconfolderCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : set icon folder rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlSetRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    const GUI_RECT_T    *rect_ptr
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : display icon folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlDraw(
                             BOOLEAN                is_update,  //is update anim
                             CTRLICONFOLDER_OBJ_T   *iconfolder_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : display icon folder bg image or color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlDrawBg(
                               GUI_RECT_T           bg_rect,
                               CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : handle icon folder tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlHandleTpDown(
                                  CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr    //in:
                                  );

/*****************************************************************************/
//  Description : handle icon folder tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlHandleTpUp(
                                CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr    //in:
                                );

/*****************************************************************************/
//  Description : alloc item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocItemMemory(
                           CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : free item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeItemMemory(
                          CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : free one item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeOneItemMemory(
                             ICONFOLDER_ITEM_T   *item_ptr   //in:
                             );

/*****************************************************************************/
//  Description : display all icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayAllIcon(
                          BOOLEAN               is_update,  //is update anim
                          CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : set anim control rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAnimRect(
                       BOOLEAN      is_update,          //in:is update anim
                       GUI_RECT_T   icon_rect,          //in:
                       GUI_RECT_T   folder_display_rect,//in:
                       void         *anim_ctrl_ptr      //in:
                       );

/*****************************************************************************/
//  Description : send msg to win, notify it append the icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SendMsgAppendIcon(
                             uint16                 index,              //in
                             CTRLICONFOLDER_OBJ_T   *iconfolder_ctrl_ptr//in
                             );

/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIcon(
                      uint16                index,                  //in:
                      CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr,   //in:
                      GUIANIM_DATA_INFO_T   *data_info_ptr,         //in:may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr          //in:may PNULL
                      );

/*****************************************************************************/
//  Description : icon anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void IconAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            );

/*****************************************************************************/
//  Description : display mark icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMarkIcon(
                           uint16               index,              //in:
                           CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                           );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init iconfolder class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlInitVtbl (
    CTRLICONFOLDER_VTBL_T        *iconfolder_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) iconfolder_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = IconfolderCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = IconfolderCtrlSetRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = IconfolderCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void IconfolderCtrlPackInitParam (
    CTRLICONFOLDER_INIT_DATA_T     *iconfolder_init_ptr,   //in
    CTRLICONFOLDER_INIT_PARAM_T   *iconfolder_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) iconfolder_param_ptr;

    if (PNULL == iconfolder_init_ptr || PNULL == iconfolder_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = iconfolder_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : iconfolder construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLICONFOLDER_INIT_PARAM_T   *iconfolder_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == iconfolder_param_ptr)
    {
        return FALSE;
    }
        
    //need anim call back
    iconfolder_ctrl_ptr->is_anim_callback = TRUE;    

    //get icon folder theme
    MMITHEME_GetIconFolderTheme(&iconfolder_ctrl_ptr->theme);
    base_ctrl_ptr->lcd_dev_info = iconfolder_ctrl_ptr->theme.lcd_dev;

    //set icon folder other info 
    iconfolder_ctrl_ptr->win_handle   = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
   
    return (result);
}

/*****************************************************************************/
//  Description : iconfolder destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == iconfolder_ctrl_ptr)
    {
        return FALSE;
    }

    //free item memory
    FreeItemMemory(iconfolder_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : handle iconfolder msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IconfolderCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == iconfolder_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        IconfolderCtrlDraw(FALSE, iconfolder_ctrl_ptr);
        break;

    case MSG_NOTIFY_UPDATE:
        //not handle
        break;

    case MSG_APP_OK:
	 result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_OK);	
        break;

    case MSG_APP_WEB:
	 result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_MIDSK);
        break;

    case MSG_APP_CANCEL:
	 result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_CANCEL);
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_UP:
	 IconfolderCtrlHandleTpUp(iconfolder_ctrl_ptr);
        break;

    case MSG_TP_PRESS_DOWN:
        IconfolderCtrlHandleTpDown(iconfolder_ctrl_ptr);
        break;
#endif

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_ICONFOLDER_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set icon folder rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlSetRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr,   //control pointer
    const GUI_RECT_T    *rect_ptr
                                )
{
    BOOLEAN                 result = TRUE;
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;

    if(PNULL == base_ctrl_ptr || PNULL == rect_ptr)
	return FALSE;

    iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T *)base_ctrl_ptr;

    if (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr))
    {
        //set rect
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;
        
        GUI_SetVOrHRect(base_ctrl_ptr->handle,(GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));
    }

    return (result);
}

/*****************************************************************************/
//  Description : display iconfolder control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlDraw (
    BOOLEAN                is_update,  //is update anim	
    CTRLICONFOLDER_OBJ_T     *iconfolder_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if (PNULL == iconfolder_ctrl_ptr)
    {
        return;
    }

    //display bg
    IconfolderCtrlDrawBg(base_ctrl_ptr->display_rect,iconfolder_ctrl_ptr);

    //display border
    GUI_DisplayBorder(base_ctrl_ptr->rect,
        base_ctrl_ptr->display_rect,
        &iconfolder_ctrl_ptr->theme.border,
        &base_ctrl_ptr->lcd_dev_info);

    //display icon
    DisplayAllIcon(is_update,iconfolder_ctrl_ptr);

}

/*****************************************************************************/
//  Description : display icon folder bg image or color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlDrawBg(
                               GUI_RECT_T           bg_rect,
                               CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr
                               )
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if(PNULL == iconfolder_ctrl_ptr)
    {
	return;
    }

    //set image rect
    img_rect.left   = (int16)(bg_rect.left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16)(bg_rect.top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16)(img_rect.left + bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16)(img_rect.top + bg_rect.bottom - bg_rect.top);

    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = bg_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = iconfolder_ctrl_ptr->win_handle;
    
    //display bg
    GUI_DisplayBg(&iconfolder_ctrl_ptr->theme.bg,
        &bg_display,
        &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconfolderCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;

    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;

    iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T *)base_ctrl_ptr;

    //set display rect
    base_ctrl_ptr->display_rect = *rect_ptr;

    if (is_update)
    {
        //update icon folder
        IconfolderCtrlDraw(TRUE,iconfolder_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle icon folder tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlHandleTpDown(
                                  CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr    //in:
                                  )
{
    int32           i = 0;
    GUI_RECT_T      icon_rect = {0};
    MMI_HANDLE_T    ctrl_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;
    
    if (0 < iconfolder_ctrl_ptr->total_num)
    {
        //display bg
        IconfolderCtrlDrawBg(base_ctrl_ptr->display_rect,iconfolder_ctrl_ptr);

        //display border
        GUI_DisplayBorder(base_ctrl_ptr->rect,
            base_ctrl_ptr->display_rect,
            &iconfolder_ctrl_ptr->theme.border,
            &base_ctrl_ptr->lcd_dev_info);

        //set all anim rect
        for (i=(iconfolder_ctrl_ptr->total_num-1); i>=0; i--)
        {
            if (iconfolder_ctrl_ptr->item_ptr[i].icon.is_load)
            {
                //get icon display rect
                icon_rect = ICONFOLDER_GetIconRect((uint16)i,iconfolder_ctrl_ptr);
                
                //get tp down rect
                icon_rect = MMITHEME_GetTpDownIconRect((uint16)i,icon_rect);

                //get control handle
                ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);

                //set no anim call back
                iconfolder_ctrl_ptr->is_anim_callback = FALSE;

                //set anim rect
                SetAnimRect(TRUE,
                    icon_rect,
                    base_ctrl_ptr->display_rect,
                    (void *)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);

                //reset call back
                iconfolder_ctrl_ptr->is_anim_callback = TRUE;
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle icon folder tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IconfolderCtrlHandleTpUp(
                                CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr    //in:
                                )
{
    int32           i = 0;
    GUI_RECT_T      icon_rect = {0};
    MMI_HANDLE_T    ctrl_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if (0 < iconfolder_ctrl_ptr->total_num)
    {
        //display bg
        IconfolderCtrlDrawBg(base_ctrl_ptr->display_rect,iconfolder_ctrl_ptr);

        //display border
        GUI_DisplayBorder(base_ctrl_ptr->rect,
            base_ctrl_ptr->display_rect,
            &iconfolder_ctrl_ptr->theme.border,
            &base_ctrl_ptr->lcd_dev_info);

        //set all anim rect
        for (i=(iconfolder_ctrl_ptr->total_num-1); i>=0; i--)
        {
            if (iconfolder_ctrl_ptr->item_ptr[i].icon.is_load)
            {
                //get icon display rect
                icon_rect = ICONFOLDER_GetIconRect((uint16)i,iconfolder_ctrl_ptr);

                //get control handle
                ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);

                //set no anim call back
                iconfolder_ctrl_ptr->is_anim_callback = FALSE;

                //set anim rect
                SetAnimRect(TRUE,
                    icon_rect,
                    base_ctrl_ptr->display_rect,
                    (void *)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);

                //reset call back
                iconfolder_ctrl_ptr->is_anim_callback = TRUE;
            }
        }
    }
}

/*****************************************************************************/
//  Description : get iconfolder type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ICONFOLDER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "iconfolder",                                         // control name
                       sizeof (CTRLICONFOLDER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) IconfolderCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) IconfolderCtrlDestruct,      // control destructor function
                       sizeof (CTRLICONFOLDER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) IconfolderCtrlPackInitParam,     // convert init param
                       sizeof (CTRLICONFOLDER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) IconfolderCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
// 	Description : set icon folder background
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_SetBg(
                                   MMI_HANDLE_T     ctrl_id,//in
                                   GUI_BG_T         *bg_ptr //in
                                   )
{
    BOOLEAN					result				= FALSE;
    CTRLICONFOLDER_OBJ_T 	*iconfolder_ctrl_ptr	= PNULL;

    //get icon folder control pointer
    iconfolder_ctrl_ptr = ICONFOLDER_GetPtr(ctrl_id);

    if (PNULL != iconfolder_ctrl_ptr)
    {
        //set bg
        iconfolder_ctrl_ptr->theme.bg = *bg_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set icon folder rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONFOLDER_SetRect(
                                  MMI_HANDLE_T      ctrl_id,
                                  GUI_RECT_T        *rect_ptr
                                  )
{
    CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr = PNULL;

    //get icon folder control pointer
    iconfolder_ctrl_ptr = ICONFOLDER_GetPtr(ctrl_id);

    if (PNULL != iconfolder_ctrl_ptr)
    {
        IconfolderCtrlSetRect((CTRLBASE_OBJ_T *)iconfolder_ctrl_ptr, rect_ptr);
    }
}

/*****************************************************************************/
//  Description : set icon folder total icons number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONFOLDER_SetTotalIcon(
                                       MMI_CTRL_ID_T    ctrl_id,    //in
                                       uint16           total_num,  //in
                                       uint16           folde_index //in
                                       )
{
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;

    //get icon folder control pointer
    iconfolder_ctrl_ptr = ICONFOLDER_GetPtr(ctrl_id);

    if (PNULL != iconfolder_ctrl_ptr)
    {
        //set folder index
        iconfolder_ctrl_ptr->folder_index = folde_index;

        //free item memory
        FreeItemMemory(iconfolder_ctrl_ptr);
        
        //set max icon
        iconfolder_ctrl_ptr->total_num = (uint16)MIN(total_num,iconfolder_ctrl_ptr->theme.icon_max_num);

        //alloc
        AllocItemMemory(iconfolder_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : append one icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_AppendIcon(
                                        uint16              index,          //in
                                        MMI_CTRL_ID_T       ctrl_id,        //in
                                        GUIANIM_DATA_INFO_T *data_info_ptr, //in
                                        GUIANIM_FILE_INFO_T *file_info_ptr  //in
                                        )
{
    BOOLEAN                 result = FALSE;
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;

    if (PNULL != file_info_ptr || PNULL != data_info_ptr)
    {
        //get icon folder control pointer
        iconfolder_ctrl_ptr = ICONFOLDER_GetPtr(ctrl_id);
        if ((PNULL != iconfolder_ctrl_ptr) &&
            (index < iconfolder_ctrl_ptr->total_num))
        {
            //inset icon
            InsertIcon(index,iconfolder_ctrl_ptr,data_info_ptr,file_info_ptr);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : alloc item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocItemMemory(
                           CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr
                           )
{
    if (0 < iconfolder_ctrl_ptr->total_num)
    {
        iconfolder_ctrl_ptr->item_ptr = SCI_ALLOC_APP(iconfolder_ctrl_ptr->total_num*sizeof(ICONFOLDER_ITEM_T));
        SCI_MEMSET(iconfolder_ctrl_ptr->item_ptr,0,(iconfolder_ctrl_ptr->total_num*sizeof(ICONFOLDER_ITEM_T)));
    }
}

/*****************************************************************************/
//  Description : free item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeItemMemory(
                          CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr
                          )
{
    uint16      i = 0;

    if (PNULL != iconfolder_ctrl_ptr->item_ptr)
    {
        for (i=0; i<iconfolder_ctrl_ptr->total_num; i++)
        {
            //free one item
            FreeOneItemMemory(&iconfolder_ctrl_ptr->item_ptr[i]);
        }
        
        SCI_FREE(iconfolder_ctrl_ptr->item_ptr);
    }
}

/*****************************************************************************/
//  Description : free one item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeOneItemMemory(
                             ICONFOLDER_ITEM_T   *item_ptr   //in:
                             )
{
    //free icon
    if (0 != item_ptr->icon.anim_ctrl_add)
    {
        MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_ptr->icon.anim_ctrl_add));
        item_ptr->icon.anim_ctrl_add = PNULL;
        item_ptr->icon.is_load = FALSE;
    }
}

/*****************************************************************************/
//  Description : display all icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayAllIcon(
                          BOOLEAN               is_update,  //is update anim
                          CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr
                          )
{
    int32       i = 0;
    GUI_RECT_T  icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if(PNULL == iconfolder_ctrl_ptr)
    {
	return;
    }
    
    if (0 < iconfolder_ctrl_ptr->total_num)
    {
        //display all icon
        for (i=(iconfolder_ctrl_ptr->total_num-1); i>=0; i--)
        {
            //get icon display rect
            icon_rect = ICONFOLDER_GetIconRect((uint16)i,iconfolder_ctrl_ptr);

            if (iconfolder_ctrl_ptr->item_ptr[i].icon.is_load)
            {
                //set no anim call back
                iconfolder_ctrl_ptr->is_anim_callback = FALSE;

                //set anim rect
                SetAnimRect(is_update,
                    icon_rect,
                    base_ctrl_ptr->display_rect,
                    (void *)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);

                //reset call back
                iconfolder_ctrl_ptr->is_anim_callback = TRUE;
            }
            else
            {
                //append icon
                SendMsgAppendIcon((uint16)i,iconfolder_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : set anim control rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetAnimRect(
                       BOOLEAN      is_update,          //in:is update anim
                       GUI_RECT_T   icon_rect,          //in:
                       GUI_RECT_T   folder_display_rect,//in:
                       void         *anim_ctrl_ptr      //in:
                       )
{
    GUI_RECT_T  display_rect = {0};

    //set control rect
    GUIANIM_SetCtrlRectByPtr(anim_ctrl_ptr,&icon_rect);

    //get display rect
    if (GUI_IntersectRect(&display_rect,icon_rect,folder_display_rect))
    {
        //set display rect
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            is_update);
    }
    else
    {
        //set display rect
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            FALSE);
    }
}

/*****************************************************************************/
//  Description : send msg to win, notify it append the icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SendMsgAppendIcon(
                             uint16                 index,              //in
                             CTRLICONFOLDER_OBJ_T   *iconfolder_ctrl_ptr//in
                             )
{
    CTRLICONFOLDER_NOTIFY_T  notify_msg 	= {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr 			= (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if(PNULL == iconfolder_ctrl_ptr)
    {
	return;
    }

    //append the current icon
    if (!iconfolder_ctrl_ptr->item_ptr[index].icon.is_load)
    {
        //set notify
        notify_msg.icon_index			= index;
        notify_msg.folder_index      		= iconfolder_ctrl_ptr->folder_index;
        notify_msg.notify.code       		= MSG_NOTIFY_ICONFOLDER_APPEND_ICON;
        notify_msg.notify.src_handle 	= base_ctrl_ptr->handle;
        notify_msg.notify.src_id     		= MMK_GetCtrlId(base_ctrl_ptr->handle);

        //notify to append the next item
        GUICTRL_SendNotifyEx(base_ctrl_ptr->handle, MSG_NOTIFY_ICONFOLDER_APPEND_ICON, &notify_msg);
    }
}
/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIcon(
                      uint16                index,                  //in:
                      CTRLICONFOLDER_OBJ_T  *iconfolder_ctrl_ptr,   //in:
                      GUIANIM_DATA_INFO_T   *data_info_ptr,         //in,may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr          //in,may PNULL
                      )
{
    void                    *anim_ctrl_ptr = PNULL;
    GUI_RECT_T              icon_rect = {0};
    GUI_RECT_T              display_rect = {0};
    GUIANIM_INIT_DATA_T     init_data = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    ICONFOLDER_ICON_T    *icon_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;
    //get icon rect
    icon_rect = ICONFOLDER_GetIconRect(index,iconfolder_ctrl_ptr);

    //get icon pointer
    icon_ptr = &iconfolder_ctrl_ptr->item_ptr[index].icon;
    if (0 == icon_ptr->anim_ctrl_add)
    {
        //init anim control
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;

        create.guid               = SPRD_GUI_ANIM_ID;
        create.parent_win_handle  = iconfolder_ctrl_ptr->win_handle;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr      = &init_data;

        anim_ctrl_ptr = MMK_CreateControl(&create);
        SCI_ASSERT(PNULL != anim_ctrl_ptr); /*assert verified*/

        //set anim control address
        icon_ptr->anim_ctrl_add = (uint32)anim_ctrl_ptr;
    }
    else
    {
        anim_ctrl_ptr = (void *)icon_ptr->anim_ctrl_add;
    }

    //set layer
    GUICTRL_SetLcdDevInfo((IGUICTRL_T *)anim_ctrl_ptr,
        &base_ctrl_ptr->lcd_dev_info);

    //set anim frame
    GUIANIM_SetFrame(GUICTRL_GetCtrlHandle((IGUICTRL_T *)anim_ctrl_ptr),
        &iconfolder_ctrl_ptr->theme.icon_border);

    //set the icon has load
    icon_ptr->is_load = TRUE;

    //set call back
    call_back.param         = (uint32)iconfolder_ctrl_ptr;
    call_back.OwnerDrawFunc = IconAnimCallback;
    GUIANIM_SetOwnerDrawEx(GUICTRL_GetCtrlHandle((IGUICTRL_T *)anim_ctrl_ptr),&call_back);

    //set anim param
    ctrl_info.is_ctrl_id = FALSE;
    ctrl_info.ctrl_ptr   = anim_ctrl_ptr;

    //set display info
    display_info.bg                = iconfolder_ctrl_ptr->theme.icon_bg;
    display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_crop_rect      = TRUE;
    display_info.is_disp_one_frame = TRUE;
    display_info.is_disp_frame     = TRUE;
    display_info.specify_width     = iconfolder_ctrl_ptr->theme.icon_width;
    display_info.specify_height    = iconfolder_ctrl_ptr->theme.icon_height;
    display_info.rotate_angle      = ICONFOLDER_GetIconAngle(index,iconfolder_ctrl_ptr);
    
    if (PNULL != data_info_ptr)
    {
        data_info.data_ptr  = data_info_ptr->data_ptr;
        data_info.data_size = data_info_ptr->data_size;
        data_info.img_id    = data_info_ptr->img_id;
        data_info.anim_id   = data_info_ptr->anim_id;

        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    }
    else
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(PNULL != file_info_ptr); /*assert verified*/
        if (PNULL != file_info_ptr)
        {
            file_info.full_path_wstr_ptr = file_info_ptr->full_path_wstr_ptr;
            file_info.full_path_wstr_len = file_info_ptr->full_path_wstr_len;
        
            GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
        }        
    }

    //get display rect
    if (GUI_IntersectRect(&display_rect,icon_rect,base_ctrl_ptr->display_rect))
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            TRUE);
    }
    else
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            FALSE);
    }
}

/*****************************************************************************/
//  Description : icon anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void IconAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            )
{
    int32                   i = 0;
    MMI_HANDLE_T            ctrl_handle = 0;
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  		   *base_ctrl_ptr = PNULL;

    if(PNULL == owner_draw_ptr)
    {
        return;
    }

    //get icon folder control pointer
    iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T *)owner_draw_ptr->param;
    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if ((iconfolder_ctrl_ptr->is_anim_callback) &&
        (!MMK_IsWinOnPaint(iconfolder_ctrl_ptr->win_handle)))
    {
        //display bg
        IconfolderCtrlDrawBg(base_ctrl_ptr->display_rect,iconfolder_ctrl_ptr);

        //display border
        GUI_DisplayBorder(base_ctrl_ptr->rect,
            base_ctrl_ptr->display_rect,
            &iconfolder_ctrl_ptr->theme.border,
            &base_ctrl_ptr->lcd_dev_info);

        //update all icon
        for (i=(iconfolder_ctrl_ptr->total_num-1); i>=0; i--)
        {
            if (iconfolder_ctrl_ptr->item_ptr[i].icon.is_load)
            {
                ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconfolder_ctrl_ptr->item_ptr[i].icon.anim_ctrl_add);
                GUIANIM_UpdateDisplay(ctrl_handle);
            }
        }
    }

    //display mark icon
    DisplayMarkIcon(0,iconfolder_ctrl_ptr);
}

/*****************************************************************************/
//  Description : display mark icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMarkIcon(
                           uint16               index,              //in:
                           CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                           )
{
    uint16          img_width = 0;
    uint16          img_height = 0;
    GUI_RECT_T      icon_rect = {0};
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      img_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconfolder_ctrl_ptr;

    if ((PNULL !=iconfolder_ctrl_ptr) && iconfolder_ctrl_ptr->is_selected)
    {
        //get icon display rect
        icon_rect = ICONFOLDER_GetIconRect(index,iconfolder_ctrl_ptr);

        //get image width and height
        GUIRES_GetImgWidthHeight(&img_width,
            &img_height,
            iconfolder_ctrl_ptr->theme.mark_img_id,
            iconfolder_ctrl_ptr->win_handle);

        //set display rect
        display_rect.left   = (int16)(icon_rect.right - img_width);
        display_rect.top    = (int16)(icon_rect.bottom - img_height);
        display_rect.right  = (int16)(display_rect.left + img_width - 1);
        display_rect.bottom = (int16)(display_rect.top + img_height - 1);

        if (GUI_IntersectRect(&cross_rect,display_rect,base_ctrl_ptr->display_rect))
        {
            //set image rect
            img_rect.left   = (int16)(cross_rect.left - display_rect.left);
            img_rect.top    = (int16)(cross_rect.top - display_rect.top);
            img_rect.right  = (int16)(img_rect.left + cross_rect.right - cross_rect.left);
            img_rect.bottom = (int16)(img_rect.top + cross_rect.bottom - cross_rect.top);

            //display selected image
            GUIRES_DisplayImg(PNULL,
                &cross_rect,
                &img_rect,
                iconfolder_ctrl_ptr->win_handle,
                iconfolder_ctrl_ptr->theme.mark_img_id,
                &base_ctrl_ptr->lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : set selected item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_SetSelected(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         BOOLEAN        is_selected
                                         )
{
    BOOLEAN                 result = FALSE;
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr = PNULL;

    //get icon folder control pointer
    iconfolder_ctrl_ptr = ICONFOLDER_GetPtr(ctrl_id);
    if (PNULL != iconfolder_ctrl_ptr)
    {
        iconfolder_ctrl_ptr->is_selected = is_selected;

        result = TRUE;
    }

    return (result);
}


#endif
