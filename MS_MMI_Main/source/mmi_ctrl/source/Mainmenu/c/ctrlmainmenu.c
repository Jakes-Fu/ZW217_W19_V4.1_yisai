/*****************************************************************************
** File Name:      ctrlmainmenu.c                                               *
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

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "ctrlmainmenu_export.h"
#include "ctrlmainmenu.h"
#include "mmitheme_mainmenu.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
// TODO:
// for user to modify
typedef struct
{
    uint32 group_id;
    uint32 menu_id;

} CTRLMAINMENU_ITEM_INFO_T;

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
//  Description : init mainmenu class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlInitVtbl(
    CTRLMAINMENU_VTBL_T        *mainmenu_vtbl_ptr
);

/*****************************************************************************/
//  Description : mainmenu construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLMAINMENU_INIT_PARAM_T   *mainmenu_param_ptr
);

/*****************************************************************************/
//  Description : mainmenu destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void MainmenuCtrlPackInitParam (
    GUIMAINMENU_INIT_DATA_T     *mainmenu_init_ptr,   //in
    CTRLMAINMENU_INIT_PARAM_T   *mainmenu_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle mainmenu msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainmenuCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get mainmenu pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMAINMENU_OBJ_T* GetMainmenuPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN MainmenuTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlSetRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr      //in:
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get mainmenu type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MAINMENU_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "mainmenu",                                         // control name
                       sizeof (CTRLMAINMENU_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) MainmenuCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) MainmenuCtrlDestruct,      // control destructor function
                       sizeof (CTRLMAINMENU_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) MainmenuCtrlPackInitParam,     // convert init param
                       sizeof (CTRLMAINMENU_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) MainmenuCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init mainmenu class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlInitVtbl (
    CTRLMAINMENU_VTBL_T        *mainmenu_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) mainmenu_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = MainmenuCtrlHandleMsg;

    base_ctrl_vtbl_ptr->SetRect = MainmenuCtrlSetRect;
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
LOCAL void MainmenuCtrlPackInitParam (
    GUIMAINMENU_INIT_DATA_T     *mainmenu_init_ptr,   //in
    CTRLMAINMENU_INIT_PARAM_T   *mainmenu_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) mainmenu_param_ptr;

    if (PNULL == mainmenu_init_ptr || PNULL == mainmenu_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = mainmenu_init_ptr->both_rect;

    mainmenu_param_ptr->type = (MMIMAINMENU_TYPE_E)mainmenu_init_ptr->type;
}

/*****************************************************************************/
//  Description : mainmenu construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLMAINMENU_INIT_PARAM_T   *mainmenu_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLMAINMENU_OBJ_T     *mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*) base_ctrl_ptr;
    CTRLMAINMENU_REGISTER_PM reg_fun = PNULL; 

    if (PNULL == base_ctrl_ptr || PNULL == mainmenu_param_ptr)
    {
        return FALSE;
    }

    mainmenu_ctrl_ptr->type   = mainmenu_param_ptr->type; 
    
    reg_fun = THEMEMAINMENU_GetRegister(mainmenu_param_ptr->type); 

    if (PNULL == reg_fun)
    {
        return FALSE;
    }
    
    // 获取call back函数
    reg_fun(&(mainmenu_ctrl_ptr->call_back));

    mainmenu_ctrl_ptr->pm_data_ptr = mainmenu_ctrl_ptr->call_back.Initialize(base_ctrl_ptr->handle);

    return (result);
}

/*****************************************************************************/
//  Description : mainmenu destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLMAINMENU_OBJ_T  *mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*) base_ctrl_ptr;

    if (PNULL == mainmenu_ctrl_ptr)
    {
        return FALSE;
    }

    if (PNULL != mainmenu_ctrl_ptr->call_back.Destory)
    {
        result = mainmenu_ctrl_ptr->call_back.Destory(mainmenu_ctrl_ptr->pm_data_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle mainmenu msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainmenuCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLMAINMENU_OBJ_T  *menu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*) base_ctrl_ptr;

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

	if (PNULL != menu_ctrl_ptr->call_back.Handle)
	{
		result = menu_ctrl_ptr->call_back.Handle(menu_ctrl_ptr->pm_data_ptr, msg_id, param);
	}

    return (result);
}

/*****************************************************************************/
//  Description : get mainmenu pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMAINMENU_OBJ_T* GetMainmenuPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (MainmenuTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLMAINMENU_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN MainmenuTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_MAINMENU_TYPE);
}
/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MainmenuCtrlSetRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr      //in:
)
{
    BOOLEAN         result = TRUE;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)base_ctrl_ptr;

    if (PNULL != mainmenu_ctrl_ptr && !GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr))
    {
        //set rect
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        if (PNULL != mainmenu_ctrl_ptr->call_back.Handle)
        {
            mainmenu_ctrl_ptr->call_back.Handle(mainmenu_ctrl_ptr->pm_data_ptr, MSG_CTRLMAINMENU_SET_RECT, PNULL);
        }
    }

    return (result);
}


/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetRect (
                                   MMI_HANDLE_T ctrl_handle, 
                                   GUI_RECT_T* rect_ptr //in/out
)
{
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_handle);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return;
	}

	*rect_ptr = mainmenu_ctrl_ptr->base_ctrl.rect;

	return;
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_TYPE_E CTRLMAINMENU_GetStyle(
                                  MMI_HANDLE_T ctrl_handle
                                  )
{
    MMIMAINMENU_TYPE_E style = MMIMAINMENU_TYPE_MAX;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_handle);

    if (PNULL != mainmenu_ctrl_ptr)
    {
        style = mainmenu_ctrl_ptr->type;
    }

	return style;
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetlayerInfo (
                                       MMI_HANDLE_T ctrl_handle, 
                                       GUI_LCD_DEV_INFO* layer_info_ptr //in/out
                                       )
{
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_handle);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return;
	}

	*layer_info_ptr = mainmenu_ctrl_ptr->base_ctrl.lcd_dev_info;

	return;
}

/*****************************************************************************/
//  Description : 设置菜单是否处于移动状态
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_InitMoveState(
                                          MMI_HANDLE_T  ctrl_handle
                                          )
{
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_handle);

    if(PNULL == mainmenu_ctrl_ptr || PNULL == mainmenu_ctrl_ptr->call_back.Handle)
    {
        return FALSE;
    }

    mainmenu_ctrl_ptr->call_back.Handle(
        mainmenu_ctrl_ptr->pm_data_ptr,
        MSG_CTRLMAINMENU_SET_MOVE_STATE, PNULL);

    return TRUE;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_CancelMoveIcon(
                                      MMI_CTRL_ID_T        ctrl_id
                                      )
{
    BOOLEAN result = FALSE;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_id);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return FALSE;
	}

	if (PNULL != mainmenu_ctrl_ptr->call_back.Handle)
	{
		mainmenu_ctrl_ptr->call_back.Handle(
		        mainmenu_ctrl_ptr->pm_data_ptr,
		        MSG_CTRLMAINMENU_CANCEL_MOVE_ICON, 0);

        result = TRUE;
	}

    return result;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetId(
                       MMI_CTRL_ID_T        ctrl_id,
                       MMI_MENU_GROUP_ID_T  *group_id_ptr,  //group id
                       MMI_MENU_ID_T        *menu_id_ptr    //menu id
                       )
{
    CTRLMAINMENU_ITEM_INFO_T item_info = {0};
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_id);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return;
	}

	if (PNULL != mainmenu_ctrl_ptr->call_back.Handle)
	{
		mainmenu_ctrl_ptr->call_back.Handle(
		    mainmenu_ctrl_ptr->pm_data_ptr,
		    MSG_CTRLMAINMENU_GET_ID, (DPARAM)&item_info);

        if (PNULL != group_id_ptr)
        {
            *group_id_ptr = item_info.group_id;
        }

        if (PNULL != menu_id_ptr)
        {
            *menu_id_ptr = item_info.menu_id;
        }
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_IsMoveState(
                                   MMI_CTRL_ID_T        ctrl_id
                                   )
{
    BOOLEAN result = FALSE;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_id);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return FALSE;
	}

	if (PNULL != mainmenu_ctrl_ptr->call_back.Handle)
	{
		mainmenu_ctrl_ptr->call_back.Handle(
		    mainmenu_ctrl_ptr->pm_data_ptr,
		    MSG_CTRLMAINMENU_IS_MOVE_STATE, (DPARAM)&result);
	}

    return result;
}

/*****************************************************************************/
//  Description : get page manager
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void* CTRLMAINMENU_GetPM(
                            MMI_CTRL_ID_T      ctrl_id
                            )
{
    void* pm_ptr = PNULL;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_id);

	if(PNULL != mainmenu_ctrl_ptr)
	{
		pm_ptr = mainmenu_ctrl_ptr->pm_data_ptr;
	}

    return pm_ptr;
}
#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  Description : QBTHEME reload item
//  Global resource dependence : 
//  Author: guodong.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_ReloadItem(
                                   MMI_CTRL_ID_T        ctrl_id
                                   )
{
    BOOLEAN result = FALSE;
    CTRLMAINMENU_OBJ_T* mainmenu_ctrl_ptr = (CTRLMAINMENU_OBJ_T*)GetMainmenuPtr(ctrl_id);

	if(PNULL == mainmenu_ctrl_ptr)
	{
		return FALSE;
	}

	if (PNULL != mainmenu_ctrl_ptr->call_back.Handle)
	{
		mainmenu_ctrl_ptr->call_back.Handle(
		    mainmenu_ctrl_ptr->pm_data_ptr,
		    MSG_CTRLMAINMENU_RELOAD_ITEM, (DPARAM)&result);
		result = TRUE;
	}

    return result;
}
#endif