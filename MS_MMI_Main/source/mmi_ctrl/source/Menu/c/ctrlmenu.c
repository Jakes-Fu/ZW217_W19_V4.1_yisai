/*****************************************************************************
** File Name:      ctrlmenu.c                                                *
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
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "ctrlprogressbar_export.h"
#include "mmi_theme.h"
#include "mmi_gui_trc.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/
LOCAL CTRLMENU_NUMKEY_MAP_T  s_ctrlmenu_numkey_map[] = {
                                                      0, MSG_APP_1,
                                                      1, MSG_APP_2,
                                                      2, MSG_APP_3,
                                                      3, MSG_APP_4,
                                                      4, MSG_APP_5,
                                                      5, MSG_APP_6,
                                                      6, MSG_APP_7,
                                                      7, MSG_APP_8,
                                                      8, MSG_APP_9,
                                                      9, MSG_APP_STAR,
                                                      10,MSG_APP_0,
                                                      11,MSG_APP_HASH
                                                      };
/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
extern MMI_MESSAGE_ID_E MMIAPICOM_ConvertKeyMsg(MMI_MESSAGE_ID_E msg_id);/*lint -esym(526,MMIAPICOM_ConvertKeyMsg) */
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : menu construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuConstruct(
                            CTRLBASE_OBJ_T          *base_ctrl_ptr,
                            CTRLMENU_INIT_PARAM_T   *menu_param_ptr
                            );

/*****************************************************************************/
//  Description : init static menu control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitStaticMenu(
                          CTRLMENU_OBJ_T        *menu_ctrl_ptr, //control pointer
                          GUI_BOTH_RECT_T       both_rect,      //menu rect
                          MMI_MENU_GROUP_ID_T   group_id        //menu group id
                          );

/*****************************************************************************/
//  Description : init group info and item info for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStaticMenuInfo(
                                 CTRLMENU_OBJ_T         *menu_ctrl_ptr, //[in]
                                 MMI_MENU_GROUP_ID_T    group_id        //[in]
                                 );

/*****************************************************************************/
//  Description : calculate the size which group info and item info want
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL uint32 CalcStaticMenuInfoSize(
                                    MMI_MENU_GROUP_ID_T group_id,           // [in] current group id
                                    uint32              *group_count_ptr    // [out] how many group in static menu
                                    );

/*****************************************************************************/
//  Description : set all menu info, include group info and item info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL CTRLMENU_GROUP_INFO_T* SetStaticMenuInfo(                                      // [ret] point to next group info
                                              CTRLMENU_GROUP_INFO_T *group_info_ptr, // [in] set in this group info if it has group
                                              MMI_MENU_GROUP_ID_T   group_id,         // [in]
                                              uint32                *remain_group_count_ptr        // [in:out] the remaining group count
                                              );

/*****************************************************************************/
//  Description : init dynamic menu control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDynamicMenu(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr, //control pointer
                           uint16           node_max_num,   //node max number
                           GUI_BOTH_RECT_T  both_rect,      //menu rect
                           GUIMENU_STYLE_E  menu_style      //menu style
                           );

/*****************************************************************************/
//  Description : alloc memory for dynamic menu tree
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocDynamicMenuMemory(
                                  uint16                    max_node_num,   //max node number
                                  CTRLMENU_NODE_MEMORY_T    *node_mem_ptr   //dynamic menu node memory pointer
                                  );

/*****************************************************************************/
//  Description : menu destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuDestruct(
                           CTRLBASE_OBJ_T   *base_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : free all string buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_NODE_T* FreeStrBuffer(
                                     CTRLMENU_NODE_T    *root_node_ptr  //root node pointer
                                     );

/*****************************************************************************/
//  Description : get group info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu
/*****************************************************************************/
LOCAL void FreeGroupInfoBuffer(
                               CTRLMENU_GROUP_INFO_T    *root_group_info_ptr    //[in]
                               );

/*****************************************************************************/
//  Description : free all selected item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeSelListMemory(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : init menu class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuInitVtbl(
                           CTRLMENU_VTBL_T  *menu_vtbl_ptr
                           );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MenuPackInitParam(
                             GUIMENU_INIT_DATA_T    *menu_init_ptr,   //in
                             CTRLMENU_INIT_PARAM_T  *menu_param_ptr   //out
                             );

/*****************************************************************************/
//  Description : handle menu msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MenuHandleMsg(
                                 CTRLBASE_OBJ_T     *base_ctrl_ptr, //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,         //message
                                 DPARAM             param           //add data
                                 );

/*****************************************************************************/
//  Description : handle menu special
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuSpecial(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             MMI_MESSAGE_ID_E   msg_id,
                             DPARAM             param
                             );

/*****************************************************************************/
//  Description : handle menu full paint msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuPaint(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void HandleTextToSpeech(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                              );

/*****************************************************************************/
//  Description : HandleMenuOkKey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuOkKey(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              MMI_MESSAGE_ID_E  msg_id
                              );

/*****************************************************************************/
//  Description : push menu in stack
//  Global resource dependence : 
//  Author: Jassmine
//  Note: this method is to store the current information and show sub menu of the current item.
/*****************************************************************************/
LOCAL void PushMenuStack(
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr 
                         );

/*****************************************************************************/
//  Description : pop menu from stack
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopMenuStack(
                        CTRLMENU_OBJ_T  *menu_ctrl_ptr 
                        );

/*****************************************************************************/
//  Description : handle menu cancel key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuCancelKey(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                  MMI_MESSAGE_ID_E  msg_id
                                  );

/*****************************************************************************/
//  Description : is update previous pop-up and return update start menu level
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 当前是pop-up menu,cancel后需要根据Rect判断是否刷新之前的pop-up menu
/*****************************************************************************/
LOCAL BOOLEAN IsUpdatePrevPopMenu(
                                  uint16            *level_ptr,     //in
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  );

/*****************************************************************************/
//  Description : handle menu direction key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuDirectionKey(
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                          MMI_MESSAGE_ID_E  msg_id
                                          );

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuNumKey(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               MMI_MESSAGE_ID_E msg_id
                               );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle menu tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpDown(
                            DPARAM          param,
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle menu tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpUp(
                          DPARAM            param,
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : handle menu tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpMove(
                            DPARAM          param,
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle menu scroll bar touch panel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpScroll(
                              uint16            first_item_index,
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : handle redraw timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleRedrawTimer(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             );
#endif

/*****************************************************************************/
//  Description : show scroll item string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ShowScrollItemStr(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : set menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MenuCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const GUI_RECT_T  *rect_ptr
                              );

/*****************************************************************************/
//  Description : modify menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ModifyMenuRect(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : get menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuCtrlGetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              GUI_RECT_T        *rect_ptr
                              );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get menu type
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MENU_GetType(void)
{
    static TYPE_TAG_T   obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (CTRL_BASE_TYPE,
                        "menu",
                        sizeof (CTRLMENU_OBJ_T),
                        (OBJECT_CONSTRUCTOR_FUNC)MenuConstruct,
                        (OBJECT_DESTRUCTOR_FUNC)MenuDestruct,
                        sizeof(CTRLMENU_INIT_PARAM_T),
                        (PARAM_PACKING_FUNC)MenuPackInitParam,
                        sizeof (CTRLMENU_VTBL_T),
                        (VTBL_INIT_FUNC) MenuInitVtbl);
    }

    return (obj_type);
}

/*****************************************************************************/
//  Description : menu construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuConstruct(
                            CTRLBASE_OBJ_T          *base_ctrl_ptr,
                            CTRLMENU_INIT_PARAM_T   *menu_param_ptr
                            )
{
    BOOLEAN             result = TRUE;
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = (CTRLMENU_OBJ_T*)base_ctrl_ptr;

    if ((PNULL != menu_ctrl_ptr) &&
        (PNULL != menu_param_ptr))
    {
        if (menu_param_ptr->is_static)
        {
            //init static menu
            InitStaticMenu(menu_ctrl_ptr,
                menu_param_ptr->base_ctrl_param.both_rect,
                menu_param_ptr->group_id);
        }
        else
        {
            //init dynamic menu
            InitDynamicMenu(menu_ctrl_ptr,
                menu_param_ptr->node_max_num,
                menu_param_ptr->base_ctrl_param.both_rect,
                menu_param_ptr->style);
        }

        menu_ctrl_ptr->is_reset_rect = TRUE;

        CTRLMENU_CheckButtonStyle(menu_ctrl_ptr);

        if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
        {
            //set transparent
            MMK_SetWinDisplayStyleState(menu_ctrl_ptr->win_handle, WS_HAS_TRANSPARENT|WS_POPMENU_WIN, TRUE );

            //no animation
            MMK_SetWinMoveStyle(menu_ctrl_ptr->win_handle,MOVE_NOT_FULL_SCREEN_WINDOW);

            // TODO: PDA将动画禁止
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
            MMK_SetWinMoveStyle(menu_ctrl_ptr->win_handle,MOVE_FORBIDDEN);
#endif
        }
    
	    // 默认时窗口title由控件控制
	    CTRLMENU_SetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_CUSTOMER_TITLE, FALSE);

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        // 按象素滚动
        CTRLMENU_SetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_PRG_PIXEL_SCROLL, TRUE);
        CTRLMENU_SetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_DRAW_SPLIT_LINE, TRUE);
#endif
    }

    return (result);
}

/*****************************************************************************/
//  Description : init static menu control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitStaticMenu(
                          CTRLMENU_OBJ_T        *menu_ctrl_ptr, //control pointer
                          GUI_BOTH_RECT_T       both_rect,      //menu rect
                          MMI_MENU_GROUP_ID_T   group_id        //menu group id
                          )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //get menu theme
    MMITHEME_GetCommonMenuTheme(&menu_ctrl_ptr->common_theme);
    MMITHEME_GetSecondMenuTheme(&menu_ctrl_ptr->sec_theme);
    MMITHEME_GetPopMenuTheme(&menu_ctrl_ptr->pop_theme);
#ifdef MMI_OPTIONMENU_SUPPORT
    MMITHEME_GetOptionsMenuTheme(&menu_ctrl_ptr->options_theme);
#endif

    InitStaticMenuInfo(menu_ctrl_ptr,group_id);

    base_ctrl_ptr->lcd_dev_info = menu_ctrl_ptr->common_theme.lcd_dev;

    //set menu group
    menu_ctrl_ptr->cur_group_id  = group_id;
    menu_ctrl_ptr->cur_group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
    menu_ctrl_ptr->cur_group_info_ptr = menu_ctrl_ptr->root_group_info_ptr;
    menu_ctrl_ptr->item_total_num = menu_ctrl_ptr->cur_group_info_ptr->visible_child_item_num;

    //set other param
    menu_ctrl_ptr->is_static  = TRUE;
    base_ctrl_ptr->both_rect  = both_rect;
    base_ctrl_ptr->rect       = both_rect.v_rect;
    menu_ctrl_ptr->cur_style  = menu_ctrl_ptr->cur_group_ptr->menu_style;
    menu_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
	
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if ((GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style) || 
		(GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style))
	{
        GUI_RECT_T  client_rect = {0};
        
		menu_ctrl_ptr->cur_style = GUIMENU_STYLE_SECOND;
		
		client_rect = MMITHEME_GetClientRect();
		both_rect.v_rect.left   = client_rect.left;
		both_rect.v_rect.top    = client_rect.top;
		both_rect.v_rect.right  = client_rect.right;
		both_rect.v_rect.bottom = client_rect.bottom;
		both_rect.h_rect.left   = client_rect.left;
		both_rect.h_rect.top    = client_rect.top;
		both_rect.h_rect.right  = client_rect.right;
		both_rect.h_rect.bottom = client_rect.bottom;

		base_ctrl_ptr->both_rect  = both_rect;
		base_ctrl_ptr->rect = both_rect.v_rect;	
	}

#endif

 // PDA项目，将POPUP menu定义为OptionsMenu
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
        menu_ctrl_ptr->cur_style = GUIMENU_STYLE_OPTIONS;
    }
#else
    if (GUIMENU_STYLE_OPTIONS == menu_ctrl_ptr->cur_style)
    {
        menu_ctrl_ptr->cur_style = GUIMENU_STYLE_POPUP;
    }
#endif

    // 默认支持滚动条
    menu_ctrl_ptr->is_need_prgbox = TRUE;

    // 增加滚动条时，是否需要动态调整区域
    menu_ctrl_ptr->is_revise_rect = FALSE;

    menu_ctrl_ptr->max_horz_width = (uint16)-1;
    menu_ctrl_ptr->max_vert_width = (uint16)-1;

    menu_ctrl_ptr->is_transparent = TRUE;

    CTRLMENU_Init(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : init group info and item info for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStaticMenuInfo(
                                 CTRLMENU_OBJ_T         *menu_ctrl_ptr, //[in]
                                 MMI_MENU_GROUP_ID_T    group_id        //[in]
                                 )
{
    uint32  group_count = 0;
    uint32  size = 0;

    size = CalcStaticMenuInfoSize(group_id, &group_count);

    // it will assert when SCI_ALLOC_APP fails, so do not check return value.
    menu_ctrl_ptr->root_group_info_ptr = SCI_ALLOC_APP(size);

    SCI_MEMSET(menu_ctrl_ptr->root_group_info_ptr, 0, size);

    SetStaticMenuInfo(menu_ctrl_ptr->root_group_info_ptr, group_id, &group_count);

    return TRUE;
}

/*****************************************************************************/
//  Description : calculate the size which group info and item info want
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL uint32 CalcStaticMenuInfoSize(
                                    MMI_MENU_GROUP_ID_T group_id,           // [in] current group id
                                    uint32              *group_count_ptr    // [out] how many group in static menu
                                    )
{
    int32               i = 0;
    uint32              size = 0;
    GUIMENU_GROUP_T     *group_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != group_count_ptr); /*assert verified*/
    if (PNULL == group_count_ptr)
    {
        return size;
    }

    group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
    if (PNULL != group_ptr)
    {
        (*group_count_ptr)++;

        // the size which this group wants, it includes group info and item info.
        size += (sizeof(CTRLMENU_GROUP_INFO_T) + group_ptr->item_count * sizeof(CTRLMENU_ITEM_INFO_T));

        // calculate this sub menu group
        for (i = 0; i < group_ptr->item_count; i++)
        {
            size += CalcStaticMenuInfoSize(group_ptr->item_ptr[i].link_group_id, group_count_ptr);
        }
    }

    return size;
}

/*****************************************************************************/
//  Description : set all menu info, include group info and item info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL CTRLMENU_GROUP_INFO_T* SetStaticMenuInfo(                                      // [ret] point to next group info
                                              CTRLMENU_GROUP_INFO_T *group_info_ptr, // [in] set in this group info if it has group
                                              MMI_MENU_GROUP_ID_T   group_id,         // [in]
                                              uint32                *remain_group_count_ptr        // [in:out] the remaining group count
                                              )
{
    int32                   i = 0;
    GUIMENU_GROUP_T         *group_ptr = PNULL;
    CTRLMENU_GROUP_INFO_T   *next_group_info_ptr = group_info_ptr;
    CTRLMENU_ITEM_INFO_T    *current_item_info_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != remain_group_count_ptr); /*assert verified*/
    if (PNULL == remain_group_count_ptr)
    {
        return PNULL;
    }

    group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
    if (PNULL != group_ptr)
    {
        (*remain_group_count_ptr)--;

        // if it is the last group, set next_group_info_ptr NULL, otherwise,
        // next_group_info_ptr is pointer to next group info.
        if (0 == (*remain_group_count_ptr))
        {
            group_info_ptr->next_group_info_ptr = PNULL;
        }
        else
        {
            // each info has group info and item info. so next group info is
            // offset to the size of them.
            group_info_ptr->next_group_info_ptr = 
                    (CTRLMENU_GROUP_INFO_T*)((uint32)group_info_ptr          // the prev group info address
                    + sizeof(CTRLMENU_GROUP_INFO_T)                          // group info size
                    + group_ptr->item_count * sizeof(CTRLMENU_ITEM_INFO_T)); // item info size
        }

        group_info_ptr->visible_child_item_num = group_ptr->item_count;
        group_info_ptr->group_id = group_id;

        next_group_info_ptr = group_info_ptr->next_group_info_ptr;

        current_item_info_ptr = GET_ITEM_INFO(group_info_ptr);

        // set all sub group info
        for (i = 0; i < group_ptr->item_count; i++)
        {
            current_item_info_ptr[i].is_grayed = FALSE;
            current_item_info_ptr[i].is_visible = TRUE;

            next_group_info_ptr = SetStaticMenuInfo(
                                        next_group_info_ptr, 
                                        group_ptr->item_ptr[i].link_group_id,
                                        remain_group_count_ptr);
        }
    }

    return next_group_info_ptr;
}

/*****************************************************************************/
//  Description : init dynamic menu control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDynamicMenu(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr, //control pointer
                           uint16           node_max_num,   //node max number
                           GUI_BOTH_RECT_T  both_rect,      //menu rect
                           GUIMENU_STYLE_E  menu_style      //menu style
                           )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //get menu theme
    MMITHEME_GetCommonMenuTheme(&menu_ctrl_ptr->common_theme);
    MMITHEME_GetSecondMenuTheme(&menu_ctrl_ptr->sec_theme);
    MMITHEME_GetPopMenuTheme(&menu_ctrl_ptr->pop_theme);
#ifdef MMI_OPTIONMENU_SUPPORT
    MMITHEME_GetOptionsMenuTheme(&menu_ctrl_ptr->options_theme);
#endif

    //alloc memory for dynamic menu tree
    AllocDynamicMenuMemory(node_max_num,&menu_ctrl_ptr->node_memory);
    menu_ctrl_ptr->root_node_ptr = CTRLMENU_CreatNewNode(&menu_ctrl_ptr->node_memory);
    menu_ctrl_ptr->root_node_ptr->sub_menu_style = menu_style;
    menu_ctrl_ptr->cur_parent_node_ptr = menu_ctrl_ptr->root_node_ptr;

    //set other param
    menu_ctrl_ptr->is_static  = FALSE;
    base_ctrl_ptr->both_rect  = both_rect;
    base_ctrl_ptr->rect       = both_rect.v_rect;
    menu_ctrl_ptr->cur_style  = menu_style;
    menu_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
    
 // PDA项目，将POPUP menu定义为OptionsMenu
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
        menu_ctrl_ptr->cur_style = GUIMENU_STYLE_OPTIONS;
    }
#endif
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style
		|| GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
	{
        GUI_RECT_T  client_rect = {0};
        
		menu_ctrl_ptr->cur_style = GUIMENU_STYLE_SECOND;
		
		client_rect = MMITHEME_GetClientRect( );
		both_rect.v_rect.left = client_rect.left;
		both_rect.v_rect.top = client_rect.top;
		both_rect.v_rect.right = client_rect.right;
		both_rect.v_rect.bottom = client_rect.bottom;
		both_rect.h_rect.left = client_rect.left;
		both_rect.h_rect.top = client_rect.top;
		both_rect.h_rect.right = client_rect.right;
		both_rect.h_rect.bottom = client_rect.bottom;
		base_ctrl_ptr->both_rect  = both_rect;
		base_ctrl_ptr->rect = both_rect.v_rect;	
	}

#endif

    // 是否移动状态，目前主菜单才有效
    menu_ctrl_ptr->is_move_state = FALSE;

    // 默认支持滚动条
    menu_ctrl_ptr->is_need_prgbox = TRUE;

    // 增加滚动条时，是否需要动态调整区域
    menu_ctrl_ptr->is_revise_rect = FALSE;

    menu_ctrl_ptr->max_horz_width = (uint16)-1;
    menu_ctrl_ptr->max_vert_width = (uint16)-1;

    menu_ctrl_ptr->is_transparent = TRUE;

    //init menu
    CTRLMENU_Init(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : alloc memory for dynamic menu tree
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocDynamicMenuMemory(
                                  uint16                    max_node_num,   //max node number
                                  CTRLMENU_NODE_MEMORY_T    *node_mem_ptr   //dynamic menu node memory pointer
                                  )
{
    uint16          i = 0;
    CTRLMENU_NODE_T *cur_free_node_ptr = PNULL;

    if (PNULL == node_mem_ptr)
    {
        return;
    }
    
    //alloc dynamic menu memory,include root node
    max_node_num = (uint16)(max_node_num + 1);
    node_mem_ptr->init_mem_ptr = SCI_ALLOC_APP(max_node_num * sizeof(CTRLMENU_NODE_T));
    SCI_MEMSET(node_mem_ptr->init_mem_ptr,0,(max_node_num*sizeof(CTRLMENU_NODE_T)));

    //set free node value
    node_mem_ptr->free_node_num = max_node_num;
    node_mem_ptr->free_node_ptr = node_mem_ptr->init_mem_ptr;

    //set next free node pointer
    cur_free_node_ptr = node_mem_ptr->free_node_ptr;
    for (i=0; i<(max_node_num-1); i++,cur_free_node_ptr++)
    {
        cur_free_node_ptr->next_free_node_ptr = cur_free_node_ptr + 1;
    }
    cur_free_node_ptr->next_free_node_ptr = PNULL;
}

/*****************************************************************************/
//  Description : menu destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuDestruct(
                           CTRLBASE_OBJ_T   *base_ctrl_ptr
                           )
{
    BOOLEAN                 result = TRUE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr = (CTRLMENU_OBJ_T*)base_ctrl_ptr;
    CTRLMENU_STATIC_ITEM_T  *cur_static_item_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr)
    {
        //reset menu info
        CTRLMENU_ResetInfo(menu_ctrl_ptr);

        while (PNULL != menu_ctrl_ptr->static_item_ptr)
        {
            cur_static_item_ptr = menu_ctrl_ptr->static_item_ptr;
            menu_ctrl_ptr->static_item_ptr = menu_ctrl_ptr->static_item_ptr->next_item_ptr;

            if (PNULL != cur_static_item_ptr->text_str.wstr_ptr)
            {
                SCI_FREE(cur_static_item_ptr->text_str.wstr_ptr);
            }

            SCI_FREE(cur_static_item_ptr);
        }

        //destroy menu
        CTRLMENU_Destroy(menu_ctrl_ptr);
    
        //free dynamic menu node memory
        if (!menu_ctrl_ptr->is_static)
        {
            FreeStrBuffer(menu_ctrl_ptr->root_node_ptr);

            if (PNULL != menu_ctrl_ptr->node_memory.init_mem_ptr)
            {
                SCI_FREE(menu_ctrl_ptr->node_memory.init_mem_ptr);
            }
        }
        else
        {
            FreeGroupInfoBuffer(menu_ctrl_ptr->root_group_info_ptr);

            if (PNULL != menu_ctrl_ptr->root_group_info_ptr)
            {
                SCI_FREE(menu_ctrl_ptr->root_group_info_ptr);
            }
        }

        //free selected item list memory
        FreeSelListMemory(menu_ctrl_ptr);

        //free prgbox control memory
        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }

        if (PNULL != menu_ctrl_ptr->temp_invisible_index_ptr)
        {
            SCI_FREE(menu_ctrl_ptr->temp_invisible_index_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : free all string buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_NODE_T* FreeStrBuffer(
                                     CTRLMENU_NODE_T    *root_node_ptr  //root node pointer
                                     )
{
    CTRLMENU_NODE_T *node_ptr = PNULL;

    if (PNULL != root_node_ptr)
    {
        //find child node
        node_ptr = FreeStrBuffer(root_node_ptr->child_node_ptr);
        if (PNULL == node_ptr)
        {
            //find neighbour node
            node_ptr = FreeStrBuffer(root_node_ptr->neighbour_node_ptr);
            if (PNULL == node_ptr)
            {
                //free item string buffer
                if (PNULL != root_node_ptr->node_wstr_ptr)
                {
                    SCI_FREE(root_node_ptr->node_wstr_ptr);
                }

                // free title string buffer
                if (PNULL != root_node_ptr->title_str.wstr_ptr)
                {
                    SCI_FREE(root_node_ptr->title_str.wstr_ptr);
                    root_node_ptr->title_str.wstr_len = 0;
                }

                // free sub title string buffer
                if (PNULL != root_node_ptr->sub_title_str.wstr_ptr)
                {
                    SCI_FREE(root_node_ptr->sub_title_str.wstr_ptr);
                    root_node_ptr->sub_title_str.wstr_len = 0;
                }
            }
        }
    }

    return (node_ptr);
}

/*****************************************************************************/
//  Description : get group info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu
/*****************************************************************************/
LOCAL void FreeGroupInfoBuffer(
                               CTRLMENU_GROUP_INFO_T    *root_group_info_ptr    //[in]
                               )
{
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = root_group_info_ptr;

    while(PNULL != group_info_ptr)
    {
        if (PNULL != group_info_ptr->title_str.wstr_ptr)
        {
            SCI_FREE(group_info_ptr->title_str.wstr_ptr);
            group_info_ptr->title_str.wstr_len = 0;
        }

        if (PNULL != group_info_ptr->sub_title_str.wstr_ptr)
        {
            SCI_FREE(group_info_ptr->sub_title_str.wstr_ptr);
            group_info_ptr->sub_title_str.wstr_len = 0;
        }

        group_info_ptr = group_info_ptr->next_group_info_ptr;
    }
}

/*****************************************************************************/
//  Description : free all selected item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeSelListMemory(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    CTRLMENU_POP_SEL_ITEM_T *sel_item_ptr = PNULL;
    CTRLMENU_POP_SEL_ITEM_T *free_item_ptr = PNULL;

    sel_item_ptr = menu_ctrl_ptr->selected_list.init_item_ptr;
    while(PNULL != sel_item_ptr)
    {
        free_item_ptr = sel_item_ptr;
        sel_item_ptr = sel_item_ptr->next_ptr;
        SCI_FREE(free_item_ptr);
    }
}


/*****************************************************************************/
//  Description : init menu class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuInitVtbl(
                           CTRLMENU_VTBL_T  *menu_vtbl_ptr
                           )
{
    BOOLEAN             result = TRUE;
    CTRLBASE_VTBL_T     *base_vtbl_ptr = (CTRLBASE_VTBL_T*)menu_vtbl_ptr;
    
    base_vtbl_ptr->HandleEvent = MenuHandleMsg;
    base_vtbl_ptr->SetRect     = MenuCtrlSetRect;
    base_vtbl_ptr->GetRect     = MenuCtrlGetRect;
    base_vtbl_ptr->GetDisplayRect     = BaseCtrlGetDisplayRectNotImp;

    return (result);
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MenuPackInitParam(
                             GUIMENU_INIT_DATA_T    *menu_init_ptr,   //in
                             CTRLMENU_INIT_PARAM_T  *menu_param_ptr   //out
                             )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)menu_param_ptr;

    //set both rect
    base_ctrl_param_ptr->both_rect = menu_init_ptr->both_rect;

    menu_param_ptr->is_static    = menu_init_ptr->is_static;
    menu_param_ptr->group_id     = menu_init_ptr->group_id;
    menu_param_ptr->node_max_num = menu_init_ptr->node_max_num;
    menu_param_ptr->style        = menu_init_ptr->menu_style;
}

/*****************************************************************************/
//  Description : handle menu msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MenuHandleMsg(
                                 CTRLBASE_OBJ_T     *base_ctrl_ptr, //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,         //message
                                 DPARAM             param           //add data
                                 )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = (CTRLMENU_OBJ_T*)base_ctrl_ptr;
    
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
	msg_id = MMIAPICOM_ConvertKeyMsg(msg_id);
#endif

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        menu_ctrl_ptr->is_first_disp = TRUE;

        // 初始值
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;
        
        HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_CTL_PAINT:
        HandleMenuPaint(menu_ctrl_ptr);
        break;

    case MSG_CTL_GET_FOCUS:
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;

        HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_CTL_GET_ACTIVE:
        menu_ctrl_ptr->is_active = TRUE;
        break;

    case MSG_CTL_LOSE_FOCUS:
        MMITheme_StopControlText();

        //reset menu info
        CTRLMENU_ResetInfo(menu_ctrl_ptr);
        
        HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        if (MSG_CTL_LOSE_ACTIVE == msg_id)
        {
            menu_ctrl_ptr->is_active = FALSE;
        }

        MMITheme_StopControlText();

        //reset menu info
        CTRLMENU_ResetInfo(menu_ctrl_ptr);

        HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_NOTIFY_MENU_INFO:
    case MSG_APP_MENU:
        result = HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
        {
            HandleMenuOkKey(menu_ctrl_ptr,msg_id);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;

    case MSG_APP_RED:
        if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_info.slide_state)
        {
            SCI_TRACE_LOW("MenuHandleMsg msg_id = %d,slide_state = %d",msg_id,menu_ctrl_ptr->slide_info.slide_state);
            result = MMI_RESULT_TRUE;
        }

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_info.slide_state)
        { 
            HandleMenuCancelKey(menu_ctrl_ptr, MSG_APP_RED);
        }
#endif
        else
        {
			if (MMITHEME_IsIstyle())
			{
				HandleMenuCancelKey(menu_ctrl_ptr, MSG_APP_RED);
			}

            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_NOTIFY_CANCEL:   
    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_info.slide_state)
        { 
            HandleMenuCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        else
        {
            SCI_TRACE_LOW("MenuHandleMsg msg_id = %d,slide_state = %d",msg_id,menu_ctrl_ptr->slide_info.slide_state);
        }
        break;
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
        {
            // 先停止，避免和3D产生纠结，导致exp
            MMITheme_StopControlText();

            result = HandleMenuDirectionKey(menu_ctrl_ptr,msg_id);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        else
        {
            SCI_TRACE_LOW("MenuHandleMsg msg_id = %d,slide_state = %d",msg_id,menu_ctrl_ptr->slide_info.slide_state);
        }
        break;

    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
        {
            HandleMenuNumKey(menu_ctrl_ptr,msg_id);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        else
        {
            SCI_TRACE_LOW("MenuHandleMsg msg_id = %d,slide_state = %d",msg_id,menu_ctrl_ptr->slide_info.slide_state);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        // 先停止，避免和3D产生纠结，导致exp
        MMITheme_StopControlText();

        HandleMenuTpDown(param,menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;

    case MSG_TP_PRESS_UP:
        HandleMenuTpUp(param,menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;

    case MSG_TP_PRESS_MOVE:
        HandleMenuTpMove(param,menu_ctrl_ptr);
        break;

    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_SHORT:
        HandleMenuSpecial(menu_ctrl_ptr,msg_id,param);
        break;

    case MSG_CTL_PRGBOX_MOVEUP_ONE:
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
    case MSG_CTL_PRGBOX_MOVE:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
        {
            HandleMenuTpScroll((uint16)(*(uint32*)param),menu_ctrl_ptr);
        }
        break;
#endif

    case MSG_TIMER:
        if (menu_ctrl_ptr->item_timer_id == *(uint8*)param)
        {
            //stop timer
            CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

            //scroll display item text
            if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state && 
                MMK_IsFocusWin(menu_ctrl_ptr->win_handle)) // slide状态下，不画
            {
                ShowScrollItemStr(menu_ctrl_ptr);
            }
        }
#ifdef TOUCH_PANEL_SUPPORT
        else if (menu_ctrl_ptr->redrew_timer_id == *(uint8*)param)
        {
            HandleRedrawTimer(menu_ctrl_ptr);
        }   
#endif
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle menu special
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuSpecial(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             MMI_MESSAGE_ID_E   msg_id,
                             DPARAM             param
                             )
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL != menu_ctrl_ptr)
    {
#ifdef UI_MULTILAYER_SUPPORT
        // 如果支持多层，创建所需动画的层
        if (UILAYER_IsMultiLayerEnable())
        {
            switch (menu_ctrl_ptr->cur_style)
            {
            case GUIMENU_STYLE_SECOND:
                result = CTRLMENU_SecSpecial(menu_ctrl_ptr,msg_id,param);
                break;

            case GUIMENU_STYLE_POPUP:
            case GUIMENU_STYLE_POPUP_RADIO:
            case GUIMENU_STYLE_POPUP_CHECK:
            case GUIMENU_STYLE_POPUP_AUTO:
            case GUIMENU_STYLE_POPUP_RECT:
                result = CTRLMENU_PopSpecial(menu_ctrl_ptr,msg_id);
                break;

            case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
                result = CTRLMENU_OptionsSpecial(menu_ctrl_ptr,msg_id);
#endif
                break;

            default:
                SCI_TRACE_LOW("HandleMenuOpen:menu style %d is error!",menu_ctrl_ptr->cur_style);
                break;
            }
        }
#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : handle menu full paint msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuPaint(
                           CTRLMENU_OBJ_T    *menu_ctrl_ptr
                           )
{
    //calculate pop-up menu rect
    if (menu_ctrl_ptr->is_reset_rect)
    {
        menu_ctrl_ptr->is_reset_rect = FALSE;
        CTRLMENU_CalculateRect(0,menu_ctrl_ptr);
    }

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
    //pop-up menu need update all,include main and sub etc.
    CTRLMENU_UpdatePrevPop(0,menu_ctrl_ptr);
#endif
    
    //display menu
    CTRLMENU_Draw(menu_ctrl_ptr);
    CTRLMENU_SetStatePtr(menu_ctrl_ptr,CTRLMENU_STATE_ENTER, FALSE);

    HandleTextToSpeech(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void HandleTextToSpeech(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                              )
{
    if (MMITheme_IsControlTTSOpen())
    {
        // 当focus item有变化时，才进行朗读
        if (menu_ctrl_ptr->pre_msg_loop_item_index != menu_ctrl_ptr->cur_item_index ||
            menu_ctrl_ptr->pre_msg_loop_menu_level != menu_ctrl_ptr->menu_level)
        {
            BOOLEAN is_grayed;
            MMI_IMAGE_ID_T img_id = 0;
            MMI_STRING_T wstr[GUIITEM_CONTENT_MAX_NUM] = {0};
            
            if (CTRLMENU_GetItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &wstr[0], &img_id, &is_grayed))
            {
                MMITheme_PlayControlText(wstr, 1);
                
                menu_ctrl_ptr->pre_msg_loop_item_index = menu_ctrl_ptr->cur_item_index;
                menu_ctrl_ptr->pre_msg_loop_menu_level = menu_ctrl_ptr->menu_level;
            }
        }
    }
}

/*****************************************************************************/
//  Description : HandleMenuOkKey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuOkKey(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              MMI_MESSAGE_ID_E  msg_id
                              )
{
    BOOLEAN             is_need_send_msg = FALSE;
    BOOLEAN             is_handled = FALSE;
    BOOLEAN             is_grayed = FALSE;
    BOOLEAN             is_exist_child = FALSE;
    uint16              cur_item_top = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUIMENU_ITEM_T      cur_item = {0};
    CTRLMENU_NODE_T     *cur_node_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr)
    {
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            is_handled = CTRLMENU_SecHandleOk(menu_ctrl_ptr,msg_id);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            is_handled = CTRLMENU_PopHandleOk(menu_ctrl_ptr,msg_id);
            break;

        case GUIMENU_STYLE_OPTIONS:
    #ifdef MMI_OPTIONMENU_SUPPORT
            is_handled = CTRLMENU_OptionsHandleOk(menu_ctrl_ptr,msg_id);
    #endif
            break;

        default:
            SCI_TRACE_LOW("HandleMenuOkKey:menu style %d is error!",menu_ctrl_ptr->cur_style);
            break;
        }

        if (!is_handled)
        {
             if (CTRLMENU_GetInfoInOK(menu_ctrl_ptr, &is_grayed, &is_exist_child, &cur_item, &cur_node_ptr))
             {
                if (!is_grayed)
                {
                    //adjust menu first item
                    CTRLMENU_AdjustFirstAndDisplay(menu_ctrl_ptr,TRUE);

                    if (!is_exist_child)
                    {
                        // 需要发消息给应用
                        is_need_send_msg = TRUE;
                    }
                    else
                    {
#ifdef UI_P3D_SUPPORT
                        const GUI_LCD_DEV_INFO *lcd_dev_info = MMITHEME_GetDefaultLcdDev();
                        GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
#endif 

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
					
						// 全屏的menu 不需要重刷前一窗口 xiyuan edit for CR
						if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
						{
                            CTRLMENU_UpdatePrevWindow(menu_ctrl_ptr);
						}
#endif

                        //push menu in stack
                        PushMenuStack(menu_ctrl_ptr);
                        
                        //reset menu info
                        CTRLMENU_ResetInfo(menu_ctrl_ptr);
                        
                        //get current item top
                        if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
                        {
                            cur_item_top = CTRLMENU_GetCurItemTop(menu_ctrl_ptr);
                        }
                        else
                        {
                            menu_ctrl_ptr->offset_y = base_ctrl_ptr->rect.top;
                        }

                        CTRLMENU_RestoreInfoInOK(menu_ctrl_ptr, &cur_item, cur_node_ptr);

                        //calculate pop-up menu rect
                        CTRLMENU_CalculateRect(cur_item_top,menu_ctrl_ptr);

#ifdef UI_P3D_SUPPORT    
                        //save old lcd buf
                        MMK_HandleWinMoveMsg(menu_ctrl_ptr->win_handle, MSG_LOSE_FOCUS, PNULL);
                        MMK_HandleWinMoveMsg(menu_ctrl_ptr->win_handle, MSG_OPEN_WINDOW, PNULL);                    
#endif

                        //destroy progress
                        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
                        {
                            MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
                            menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
                        }

                        //display next menu
                        CTRLMENU_Draw(menu_ctrl_ptr);

#ifdef UI_P3D_SUPPORT    
                        MMITHEME_StoreUpdateRect(lcd_dev_info, lcd_rect);
                        //paint move 3d effect.
                        MMK_HandleWinMoveMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);
#endif
                    }
                }
                else
                {
                    // 处理灰化
                    HandleMenuSpecial(menu_ctrl_ptr, msg_id, (DPARAM)&is_grayed);
                }
            }
        }
    }

    if (is_need_send_msg)
    {
        // 在app接到ok以后，可能先干一些事情再关菜单，这样可能会导致关菜单时停止tts失败，
        // 所以在发通知之前就关tts
        MMITheme_StopControlText();

        //notify parent control or parent window
        CTRLMENU_NotifyMsg(base_ctrl_ptr->handle,msg_id);
    }
    
    return (is_handled);
}

/*****************************************************************************/
//  Description : push menu in stack
//  Global resource dependence : 
//  Author: Jassmine
//  Note: this method is to store the current information and show sub menu of the current item.
/*****************************************************************************/
LOCAL void PushMenuStack(
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr 
                         )
{
    uint16          menu_level = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
	menu_level = menu_ctrl_ptr->menu_level;
    base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (CTRLMENU_STACK_MAX_LEVEL > menu_ctrl_ptr->menu_level)
    {
        menu_ctrl_ptr->menu_stack[menu_level].group_id         = menu_ctrl_ptr->cur_group_id;
        menu_ctrl_ptr->menu_stack[menu_level].first_item_index = menu_ctrl_ptr->first_item_index;
        menu_ctrl_ptr->menu_stack[menu_level].cur_item_index   = menu_ctrl_ptr->cur_item_index;
        menu_ctrl_ptr->menu_stack[menu_level].rect             = base_ctrl_ptr->rect;
        menu_ctrl_ptr->menu_stack[menu_level].parent_node_ptr  = menu_ctrl_ptr->cur_parent_node_ptr;
        menu_ctrl_ptr->menu_stack[menu_level].offset_y         = menu_ctrl_ptr->offset_y;

        menu_ctrl_ptr->menu_level++;
    }
    else
    {
        //SCI_TRACE_LOW:"PushMenuStack: menu stack %d is full!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_2340_112_2_18_3_20_22_239,(uint8*)"d",menu_ctrl_ptr->menu_level);
    }
}

/*****************************************************************************/
//  Description : pop menu from stack
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopMenuStack(
                        CTRLMENU_OBJ_T  *menu_ctrl_ptr 
                        )
{
    uint16          menu_level = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (0 < menu_ctrl_ptr->menu_level)
    {
        menu_ctrl_ptr->menu_level--;

        menu_level                      = menu_ctrl_ptr->menu_level;
        menu_ctrl_ptr->cur_group_id     = menu_ctrl_ptr->menu_stack[menu_level].group_id;
        menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->menu_stack[menu_level].first_item_index;
        menu_ctrl_ptr->cur_item_index   = menu_ctrl_ptr->menu_stack[menu_level].cur_item_index;
        base_ctrl_ptr->rect             = menu_ctrl_ptr->menu_stack[menu_level].rect;
        menu_ctrl_ptr->offset_y         = menu_ctrl_ptr->menu_stack[menu_level].offset_y;
    }
    else
    {
        //SCI_TRACE_LOW:"PopMenuStack: menu stack %d is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_2374_112_2_18_3_20_22_240,(uint8*)"d",menu_ctrl_ptr->menu_level);
    }
}

/*****************************************************************************/
//  Description : handle menu cancel key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuCancelKey(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                  MMI_MESSAGE_ID_E  msg_id
                                  )
{
    BOOLEAN         is_handled = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
        {
            is_handled = CTRLMENU_PopHandleCancel(menu_ctrl_ptr,msg_id);
        }

// PDA风格的二级菜单，返回时直接退出
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        if (!is_handled && 
            0 != menu_ctrl_ptr->menu_level && 
            GUIMENU_STYLE_SECOND == menu_ctrl_ptr->cur_style)
#else
        if (!is_handled && 0 != menu_ctrl_ptr->menu_level)
#endif
        {
            BOOLEAN             is_update = FALSE;
            uint16              start_level = 0;
#ifdef UI_P3D_SUPPORT
            BOOLEAN             is_pop_menu = CTRLMENU_IsPopMenu(menu_ctrl_ptr);
            
            GUI_RECT_T          pre_menu_rect = base_ctrl_ptr->rect;
            GUI_RECT_T          lcd_rect = MMITHEME_GetFullScreenRect();
            const GUI_LCD_DEV_INFO *lcd_dev_info = MMITHEME_GetDefaultLcdDev();


            if (is_pop_menu && MMITHEME_Is3DOpen())
            {
                MMIDEFAULT_SaveMoveToNewBuf();
            }
#endif
            //is need update previous pop-up menu
            is_update = IsUpdatePrevPopMenu(&start_level,menu_ctrl_ptr);

            //pop menu from stack
            PopMenuStack(menu_ctrl_ptr);

            //reset menu info
            CTRLMENU_ResetInfo(menu_ctrl_ptr);

            // reset info 
            CTRLMENU_RestoreInfoInCancel(menu_ctrl_ptr);

            //destroy progress
            if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
            {
                MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
                menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
            }

            if (is_update)
            {
#if defined(GUIPOPMENU_POP_POP_SUPPORT)
                //update previous pop-up menu
                CTRLMENU_UpdatePrevPop(start_level,menu_ctrl_ptr);
#endif
            }

#ifdef UI_P3D_SUPPORT 
            if (!is_pop_menu && MMITHEME_Is3DOpen())
            {
                //save old lcd buf
                MMK_HandleWinMoveMsg(menu_ctrl_ptr->win_handle, MSG_CLOSE_WINDOW, PNULL);                 
            }
#endif
    
            //display previous menu
            CTRLMENU_Draw(menu_ctrl_ptr);

#ifdef UI_P3D_SUPPORT
            //animate effect
            if (MMITHEME_Is3DOpen())
            {
                if (is_pop_menu)
                {
                    UILAYER_InvalidateRect( LCD_ID_0, &lcd_rect );
                    MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_WIN_SWITCH);

                    MMITHEME_PopupMenuEffect(&pre_menu_rect, start_level + 1, FALSE);
                }
                else
                {
                    MMITHEME_StoreUpdateRect(lcd_dev_info, lcd_rect);
                    //paint move 3d effect.
                    MMK_HandleWinMoveMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);                    
                }
            }
#endif

            is_handled = TRUE;
        }
    }

    if (!is_handled)
    {
        //notify parent control or parent window
        CTRLMENU_NotifyMsg(base_ctrl_ptr->handle,MSG_APP_CANCEL);
    }

    return (is_handled);
}

/*****************************************************************************/
//  Description : is update previous pop-up and return update start menu level
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 当前是pop-up menu,cancel后需要根据Rect判断是否刷新之前的pop-up menu
/*****************************************************************************/
LOCAL BOOLEAN IsUpdatePrevPopMenu(
                                  uint16            *level_ptr,     //in
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  )
{
    BOOLEAN         result = FALSE;
//    BOOLEAN         is_covered = FALSE;
    uint16          i = 0;
    uint16          menu_level = 0;
    GUI_RECT_T      cur_rect = {0};
    GUI_RECT_T      prev_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == menu_ctrl_ptr)||(PNULL == level_ptr))
    {
        return FALSE;
    }

    if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        cur_rect = base_ctrl_ptr->rect;
        for (i=menu_ctrl_ptr->menu_level; i>0; i--)
        {
            prev_rect = menu_ctrl_ptr->menu_stack[i-1].rect;
            //前一个pop-up menu rect是否完全覆盖后面一个pop-up menu rect
            if (GUI_RectIsCovered(prev_rect,cur_rect))
            {
//                 is_covered = TRUE;
                cur_rect = prev_rect;
                menu_level = (uint16)(i- 1);
//                 break;
            }
        }
#ifndef GUI_FULLSCREEN_POPMENU_SUPPORT
        //need previous pop-up menu
        //if (i != menu_ctrl_ptr->menu_level)
        {
            result = TRUE;
            //if (!is_covered)
            {
                //update previous window
                CTRLMENU_UpdatePrevWindow(menu_ctrl_ptr);

                result = TRUE;
            }
        }
#endif
        *level_ptr = menu_level;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle menu direction key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuDirectionKey(
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                          MMI_MESSAGE_ID_E  msg_id
                                          )
{
    BOOLEAN         result = FALSE;
    MMI_RESULT_E    handle_result = MMI_RESULT_TRUE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            handle_result = CTRLMENU_SecHandleUpDown(menu_ctrl_ptr,MSG_APP_UP);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            handle_result = CTRLMENU_PopHandleUpDown(menu_ctrl_ptr,MSG_APP_UP);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            handle_result = CTRLMENU_OptionsHandleUpDown(menu_ctrl_ptr,TRUE);
#endif
            break;

        default:
            break;
        }
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            handle_result = CTRLMENU_SecHandleUpDown(menu_ctrl_ptr,MSG_APP_DOWN);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            handle_result = CTRLMENU_PopHandleUpDown(menu_ctrl_ptr,MSG_APP_DOWN);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            handle_result = CTRLMENU_OptionsHandleUpDown(menu_ctrl_ptr,FALSE);
#endif
            break;

        default:
            break;
        }
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            //do nothing
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            result = CTRLMENU_PopHandleLeft(menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            result = CTRLMENU_OptionsHandleLeft(menu_ctrl_ptr);
#endif
            break;

        default:
            break;
        }
        
        if (result)
        {
            if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
            {
                //enter the next menu or notify ok msg
                HandleMenuOkKey(menu_ctrl_ptr,MSG_APP_OK);
            }
            else
            {
                //return the parent menu or notify cancel msg
                HandleMenuCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            }
        }
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            //do nothing
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            result = CTRLMENU_PopHandleRight(menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            result = CTRLMENU_OptionsHandleRight(menu_ctrl_ptr);
#endif
            break;

        default:
            break;
        }

        if (result)
        {
            if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
            {
                //return the parent menu or notify cancel msg
                HandleMenuCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            }
            else
            {
                //enter the next menu or notify ok msg
                HandleMenuOkKey(menu_ctrl_ptr,MSG_APP_OK);
            }
        }
        break;

    default:
        //SCI_TRACE_LOW:"== HandleMenuDirectionKey == msg_id %d is not handled"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_2703_112_2_18_3_20_23_241,(uint8*)"d", msg_id);
        break;
    }

    return handle_result;
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuNumKey(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               MMI_MESSAGE_ID_E msg_id
                               )
{
    BOOLEAN             result = FALSE;
    uint16              i = 0;
    uint16              key_max_num = 0;
    uint16              num_idnex = 0;
    uint16              total_num = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    //get number key index
    key_max_num = sizeof(s_ctrlmenu_numkey_map)/sizeof(CTRLMENU_NUMKEY_MAP_T);
    for (i=0; i<key_max_num; i++)
    {
        if (msg_id == s_ctrlmenu_numkey_map[i].key_msg)
        {
            num_idnex = s_ctrlmenu_numkey_map[i].num_index;
            break;
        }
    }

    //set total number
    total_num = menu_ctrl_ptr->item_total_num;

    if (num_idnex < total_num)
    {
        //handle number key
        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            result = CTRLMENU_SecHandleNum(num_idnex,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            result = CTRLMENU_PopHandleNum(num_idnex,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            result = CTRLMENU_OptionsHandleNum(num_idnex,menu_ctrl_ptr);
#endif
            break;

        default:
            break;
        }

        if (result)
        {
            //enter the next menu or notify ok msg
            result = HandleMenuOkKey(menu_ctrl_ptr,MSG_APP_WEB);
        }
    }

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle menu tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpDown(
                            DPARAM          param,
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            )
{
    GUI_POINT_T     tp_point = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            CTRLMENU_SecHandleTpDown(&tp_point,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            CTRLMENU_PopHandleTpDown(&tp_point,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            CTRLMENU_OptionsHandleTpDown(&tp_point,menu_ctrl_ptr);
#endif
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : handle menu tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpUp(
                          DPARAM            param,
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          )
{
	BOOLEAN         result = FALSE;
    GUI_POINT_T     tp_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        result = CTRLMENU_SecHandleTpUp(&tp_point,menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        result = CTRLMENU_PopHandleTpUp(&tp_point,menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
        result = CTRLMENU_OptionsHandleTpUp(&tp_point,menu_ctrl_ptr);
#endif
        break;

    default:
        break;
    }

    if (result)
    {
        //enter the next menu or notify ok msg
        HandleMenuOkKey(menu_ctrl_ptr,MSG_TP_PRESS_UP);
    }
    else
    {
        if ((CTRLMENU_IsPopMenu(menu_ctrl_ptr)) && // pop menu
            (!GUI_PointIsInRect(tp_point,base_ctrl_ptr->rect) // tp up点不在menu区域
            && !GUI_PointIsInRect(menu_ctrl_ptr->slide_info.start_point,base_ctrl_ptr->rect) // 起始点也不在区域
            && MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)) // 控件滑动状态为none
        {
            //return the parent menu or notify cancel msg
            HandleMenuCancelKey(menu_ctrl_ptr, MSG_TP_PRESS_UP);
        }
    } 
}

/*****************************************************************************/
//  Description : handle menu tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpMove(
                            DPARAM          param,
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            )
{
    GUI_POINT_T     tp_point = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        switch (menu_ctrl_ptr->cur_style)
        {
        case GUIMENU_STYLE_SECOND:
            CTRLMENU_SecHandleTpMove(&tp_point,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_POPUP:
        case GUIMENU_STYLE_POPUP_RADIO:
        case GUIMENU_STYLE_POPUP_CHECK:
        case GUIMENU_STYLE_POPUP_AUTO:
        case GUIMENU_STYLE_POPUP_RECT:
            CTRLMENU_PopHandleTpMove(&tp_point,menu_ctrl_ptr);
            break;

        case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
            CTRLMENU_OptionsHandleTpMove(&tp_point,menu_ctrl_ptr);
#endif
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : handle menu scroll bar touch panel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuTpScroll(
                              uint16            first_item_index,
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              )
{
    uint32  total_item_num = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_PRG_PIXEL_SCROLL))
        {
            total_item_num = CTRLPROGRESSBAR_GetTotalItemNum(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
        } 
        else
        {
            total_item_num = (uint32)menu_ctrl_ptr->item_total_num;
        }
        if (first_item_index < total_item_num)
        {
            //menu scroll bar handle tp
            switch (menu_ctrl_ptr->cur_style)
            {
            case GUIMENU_STYLE_SECOND:
                CTRLMENU_SecHandleTpScroll(first_item_index,menu_ctrl_ptr);
                break;

            case GUIMENU_STYLE_POPUP:
            case GUIMENU_STYLE_POPUP_RADIO:
            case GUIMENU_STYLE_POPUP_CHECK:
            case GUIMENU_STYLE_POPUP_AUTO:
            case GUIMENU_STYLE_POPUP_RECT:
                CTRLMENU_PopHandleTpScroll(first_item_index,menu_ctrl_ptr);
                break;

            case GUIMENU_STYLE_OPTIONS:
                break;

            default:
                break;
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle redraw timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleRedrawTimer(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_SecRedrawTimer(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_PopRedrawTimer(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
//  Description : show scroll item string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ShowScrollItemStr(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_SecScrollItemStr(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_PopScrollItemStr(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : set menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MenuCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const GUI_RECT_T  *rect_ptr
                              )
{
    BOOLEAN         result = TRUE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (CTRLMENU_OBJ_T *)base_ctrl_ptr;

    if (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr))
    {
        //set rect
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect(base_ctrl_ptr->handle,(GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        //stop item text timer
        CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);
        menu_ctrl_ptr->item_str_index = 0;

        //destroy progress
        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
            menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }

        //get modify rect
        ModifyMenuRect(menu_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : modify menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ModifyMenuRect(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_SecModifyRect(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_PopModifyRect(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
        CTRLMENU_OptionsModifyRect(menu_ctrl_ptr);
#endif
        break;

    default:
        SCI_TRACE_LOW("ModifyMenuRect:menu style %d is error!",menu_ctrl_ptr->cur_style);
        break;
    }
}

/*****************************************************************************/
//  Description : get menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MenuCtrlGetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              GUI_RECT_T        *rect_ptr
                              )
{
    BOOLEAN         result = TRUE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = (CTRLMENU_OBJ_T*)base_ctrl_ptr;
    
    if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style || 
            GUIMENU_STYLE_OPTIONS == menu_ctrl_ptr->cur_style)
        {
            rect_ptr->left   = 0;
            rect_ptr->top    = 0;
            rect_ptr->right  = (int16)(MMITHEME_GetMainScreenRightPixel() - 1);
            rect_ptr->bottom = (int16)(MMITHEME_GetMainScreenBottomPixel() - 1);
        }
        else
        {
            *rect_ptr = base_ctrl_ptr->rect;
        }
#else
        //pop-up menu, rect is full screen - softkey
        //因为它需要处理非menu rect区域,关闭pop-up menu
        rect_ptr->left   = 0;
        rect_ptr->top    = 0;
        rect_ptr->right  = (int16)(MMITHEME_GetMainScreenRightPixel() - 1);

        if (GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
        {
            rect_ptr->bottom = (int16)(MMITHEME_GetMainScreenBottomPixel() - 1);
        }
        else
        {
            rect_ptr->bottom = (int16)(MMITHEME_GetMainScreenBottomPixel() - MMITHEME_GetSoftkeyHeight()- 1);
        }
#endif
    }
    else
    {
        *rect_ptr = base_ctrl_ptr->rect;
    }

    return (result);
}
