/*****************************************************************************
** File Name:      mmiqbtheme_menuwin.c                                      *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget menu wintab          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmiset_wintab.h"
#include "Mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"
#include "mmi_menutable.h"
#include "mmiset_text.h"
#include "mmiwidget_internal.h"
#include "mmiqbtheme_menuwin.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_manager.h"
#endif
#if defined (PDA_UI_DROPDOWN_WIN)
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT           
#include "mmisearch_export.h"
#endif
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idleexport.h"
#endif
 /*
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"
*/

#define WIDGET_NODE_MAX_NUM            0x040

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     ( sizeof (a) / sizeof (a)[0] )
#endif

typedef enum
{
    MMIGRID_MENU_NODE_ID = 0,
    MMIGRID_MENU_TXT_ID  = 1,
    MMIGRID_MENU_IMG_ID  = 2,
    MMIGRID_MENU_ID_MAX
}MMIGRID_MENU_ID_E;     /*lint !e751 !e770*/

typedef enum
{
    MMIGRID_POPUP_MENU_ROOT = 0,
    MMIGRID_POPUP_MENU_WIDGET,
    MMIGRID_POPUP_MENU_SHORTCUT,
    MMIGRID_POPUP_MENU_WALLPAPER,
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMIGRID_POPUP_MENU_GAME_WALLPAPER,
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    MMIGRID_POPUP_MENU_FILM_WALLPAPER,
#endif
    MMIGRID_POPUP_MENU_MAX,
    MMIGRID_POPUP_WIDGET_ITEM_START,
    MMIGRID_POPUP_WIDGET_ITEM_MAX      = (MMIGRID_POPUP_WIDGET_ITEM_START + WIDGET_NODE_MAX_NUM),

    MMIGRID_IDLE_MENU_ADD,
    MMIGRID_IDLE_MENU_WALLPAPER,
    MMIGRID_IDLE_MENU_SEARCH,
    MMIGRID_IDLE_MENU_NOTIFY,
    MMIGRID_IDLE_MENU_SETTING,
    MMIGRID_IDLE_MENU_APP_MANAGER,
    
    MMIGRID_POPUP_SHORTCUT_ITEM_START
}MMIGRID_POPUP_MENU_IDX_E;  /*lint !e751 !e770 !e833 */


LOCAL MMIMAINMENU_PAGE_ITEM_INFO_T s_mainmenu_page_item[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM] = {0};


LOCAL const uint32 s_grid_popmenu_group[][MMIGRID_MENU_ID_MAX] = 
{
    {MMIGRID_POPUP_MENU_WIDGET,   TXT_WIDGET_ADD_WIDGET,    IMAGE_IDLE_WIDGETS_ADD},
    {MMIGRID_POPUP_MENU_SHORTCUT, TXT_WIDGET_ADD_SHORT,     IMAGE_IDLE_SHORTCUTS_ADD},
    //{MMIGRID_POPUP_MENU_WALLPAPER,TXT_WIDGET_SET_WALLPAPER, IMAGE_IDLE_WALLPAPER_SET}
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    {MMIGRID_POPUP_MENU_GAME_WALLPAPER,TXT_DYNAMIC_WALLPAPER, IMAGE_IDLE_GAME_WALLPAPER_SET},
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {MMIGRID_POPUP_MENU_FILM_WALLPAPER,TXT_SET_WALLPAPER_VIDEO, IMAGE_IDLE_FILM_WALLPAPER_SET}
#endif
};

LOCAL const uint32 s_grid_idle_menu_group[][MMIGRID_MENU_ID_MAX] = 
{
    {MMIGRID_IDLE_MENU_ADD,         TXT_ADD,                       IMAGE_COMMON_OPTMENU_ADD},
#ifdef MMI_TASK_MANAGER
    {MMIGRID_IDLE_MENU_APP_MANAGER, TXT_APPLICATION_MANAGER,       IMAGE_COMMON_OPTMENU_MANAGER},    
#endif/*MMI_TASK_MANAGER*/    
    {MMIGRID_IDLE_MENU_WALLPAPER,   TXT_SET_WALLPAPER_LIST_PREFIX, IMAGE_COMMON_OPTMENU_SET_WALLPAPER},
#ifdef SEARCH_SUPPORT     
    {MMIGRID_IDLE_MENU_SEARCH,      STXT_SEARCH,                   IMAGE_COMMON_OPTMENU_SEARCH},
#endif    
	
    {MMIGRID_IDLE_MENU_NOTIFY,      STXT_NOTIFICATION,             IMAGE_COMMON_OPTMENU_NOTIFY},
    {MMIGRID_IDLE_MENU_SETTING,     STXT_MAIN_SETTING,             IMAGE_COMMON_OPTMENU_SET},    
};

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  Description : HandleAddShortcutWindow
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddShortcutWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
/*****************************************************************************/
//  Description : HandleAddWidgetWindow
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddWidgetWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );
#endif

/*****************************************************************************/
//  Description : HandleWidgetAddMenuWinMsg
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridAddItemWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  FUNCTION:      AddShortCutItems
//  Description :
//  Global resource dependence : 
//  Author:       juan.wu
//  Note:
/*****************************************************************************/
LOCAL void AddShortCutItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_CTRL_ID_T ctrl_id,uint32 parent_node_id);

/*****************************************************************************/
//  FUNCTION:      AddShortCutItems
//  Description :
//  Global resource dependence : 
//  Author:       juan.wu
//  Note:
/*****************************************************************************/
LOCAL void AddWidgetItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_CTRL_ID_T ctrl_id,uint32 parent_node_id);

/*****************************************************************************/
//  FUNCTION:      GetWidgetid
//  Description :
//  Global resource dependence : 
//  Author:       juan.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetWidgetid(uint32 node_id);
LOCAL uint16 GetMenuid(uint32 node_id);


/*****************************************************************************/
//  Description : Handle grid idle menu win.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridMenuWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

WINDOW_TABLE(MMI_QBTHEME_IDLE_ADD_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleGridAddItemWinMsg),
    WIN_ID(MMI_GRID_IDLE_ADD_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMI_QBTHEME_IDLE_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleGridMenuWinMsg),
	WIN_ID(MMI_GRID_IDLE_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),    
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

#ifdef QBTHEME_SUPPORT
WINDOW_TABLE( MMIQBTHEME_ADD_SHORTCUT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAddShortcutWindow ),    
    WIN_ID( MMIQBTHEME_ADD_SHORTCUT_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),    
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};

WINDOW_TABLE( MMIQBTHEME_ADD_WIDGET_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAddWidgetWindow ),    
    WIN_ID( MMIQBTHEME_ADD_WIDGET_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),    
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

 /*****************************************************************************/
 //  FUNCTION:      AddShortCutItems
 //  Description :
 //  Global resource dependence : 
 //  Author:       juan.wu
 //  Note:
 /*****************************************************************************/
 LOCAL void AddShortCutItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_CTRL_ID_T ctrl_id,uint32 parent_node_id)
 {
     uint16 k = 0;
     uint16 i = 0;
     CTRLMAINMENU_ITEM_T menuitem_info = {0};
     GUIMENU_DYNA_ITEM_T menu_item = {0};
     MMI_STRING_T  string = {0};
     
 
     for (i = 0; i < idle_grid_ptr->grid_menu_info.menu_num; i++)
     {
         MMIMAINMENU_GetItemData(
                idle_grid_ptr->grid_menu_info.grid_menu[i].menu_id,
                idle_grid_ptr->grid_menu_info.grid_menu[i].group_id,
                &menuitem_info);
         
         if (0 == menuitem_info.group_id)
         {
             //MMI_GetLabelTextByLang(menuitem_info.text.data.text_id,&string);
             if(CTRL_TEXT_ID == menuitem_info.text.type)
            {
                MMI_GetLabelTextByLang(menuitem_info.text.data.text_id,&string);
            }
            else
            {
                string.wstr_len = menuitem_info.text.data.str_info.wstr_len;
                string.wstr_ptr = menuitem_info.text.data.str_info.wstr_ptr;
            }            
			 if (CTRL_ICON_BUFFER == menuitem_info.icon.type)
			 {
				 menu_item.item_type = DYNA_ITEM_ICON_BUFFER;
				 menu_item.icon_buffer.data = menuitem_info.icon.data.buffer.data;
				 menu_item.icon_buffer.height = menuitem_info.icon.data.buffer.height;
				 menu_item.icon_buffer.width = menuitem_info.icon.data.buffer.width;
					
			 } 
			 else if(CTRL_ICON_ID == menuitem_info.icon.type)
			 {				
				 menu_item.item_type = DYNA_ITEM_ICON_ID;
				 menu_item.select_icon_id = menuitem_info.icon.data.icon_id;
			 }
         }
#ifdef DYNAMIC_MAINMENU_SUPPORT
         else
         {
            if(CTRL_TEXT_ID == menuitem_info.text.type)
            {
                MMI_GetLabelTextByLang(menuitem_info.text.data.text_id,&string);
            }
            else
            {
                string.wstr_len = menuitem_info.text.data.str_info.wstr_len;
                string.wstr_ptr = menuitem_info.text.data.str_info.wstr_ptr;
            }
           // menu_item.select_icon_id = menuitem_info.active_icon.data.icon_id;
			if (CTRL_ICON_BUFFER == menuitem_info.icon.type)
			{
				menu_item.item_type = DYNA_ITEM_ICON_BUFFER;
				menu_item.icon_buffer.data = menuitem_info.icon.data.buffer.data;
				menu_item.icon_buffer.height = menuitem_info.icon.data.buffer.height;
				menu_item.icon_buffer.width = menuitem_info.icon.data.buffer.width;
				
			} 
			else if(CTRL_ICON_ID == menuitem_info.icon.type)
			{				
				menu_item.item_type = DYNA_ITEM_ICON_ID;
				menu_item.select_icon_id = menuitem_info.icon.data.icon_id;
			}
// 			 if (CTRL_ICON_BUFFER == menuitem_info.icon.type
// 				 && PNULL != menuitem_info.icon.data.buffer.data)
// 			 {
//                  SCI_FREE(menuitem_info.icon.data.buffer.data);
// 			 }
         }
#endif
         menu_item.item_text_ptr = &string;
         GUIMENU_InsertNode(k++, MMIGRID_POPUP_SHORTCUT_ITEM_START + i, parent_node_id,&menu_item,ctrl_id);
   
// #ifdef DYNAMIC_MAINMENU_SUPPORT    
//          if (GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC == menu_item_ptr->menu_data_type)
//          {             
//              SCI_FREE(menu_item_ptr->menu_data.dynamic_menu_ptr);
//          }
// #endif
//          
//          SCI_FREE(menu_item_ptr);   
         // MMITHEME_DestroyMenuDataInfo(menu_item_ptr);
     } 
 }
 
 /*****************************************************************************/
 //  FUNCTION:      AddShortCutItems
 //  Description :
 //  Global resource dependence : 
 //  Author:       juan.wu
 //  Note:
 /*****************************************************************************/
 LOCAL void AddWidgetItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_CTRL_ID_T ctrl_id,uint32 parent_node_id)
 {
     uint16 k = 0;
     uint16 i = 0;
     uint32 widget_id = 0;
     GUIMENU_DYNA_ITEM_T menu_item = {0};
     MMI_STRING_T  string = {0};
     
 
     //add widgets to popup menu.  
     for(i = 0; i < idle_grid_ptr->shortcut.shortcut_num; i++)
     {
         widget_id = idle_grid_ptr->shortcut.shortcut_item[i].widget_id;
#ifdef MMI_GRID_CONTACT
         if (widget_id >= MMIWIDGET_CONTACT_ID0 && widget_id <= MMIWIDGET_CONTACT_ID_MAX)
         {
             continue;
         } 
         if (MMIWIDGET_CONTACT_ADD_ID == widget_id && MMIAPIGRID_PB_IsContactFull())
         {
             continue;
         }
#endif         

#ifdef MMI_WIDGET_MEMO 
         if (widget_id >= MMIWIDGET_MEMO_ID0 && widget_id <= MMIWIDGET_MEMO_ID_MAX)
         {
             continue;
         }         
         if(MMIWIDGET_MEMO_ID == widget_id && MMIGRID_MEMO_IsNotesFull())
         {
             continue;
         }
#endif

         if(0 != idle_grid_ptr->item_ptr[widget_id - 1].item_info.text_id)
         {
             MMI_GetLabelTextByLang(idle_grid_ptr->item_ptr[widget_id - 1].item_info.text_id, &string);
             menu_item.item_text_ptr = &string;
         	 menu_item.select_icon_id = idle_grid_ptr->item_ptr[widget_id - 1].item_info.small_icon_id;
         	 GUIMENU_InsertNode(k++, MMIGRID_POPUP_WIDGET_ITEM_START + widget_id, parent_node_id, &menu_item, ctrl_id);
         }       
     }
 }
/*****************************************************************************/
//  FUNCTION:      GetWidgetid
//  Description :
//  Global resource dependence : 
//  Author:       juan.wu
//  Note:
/*****************************************************************************/
PUBLIC uint32 GetWidgetid(uint32 node_id)
{
 if(node_id >= MMIGRID_POPUP_WIDGET_ITEM_START && node_id < MMIGRID_POPUP_WIDGET_ITEM_MAX)
 {
     return (node_id - MMIGRID_POPUP_WIDGET_ITEM_START);
 }
 return ~0;
}

PUBLIC uint16 GetMenuid(uint32 node_id)
{
 if(node_id >= MMIGRID_POPUP_SHORTCUT_ITEM_START)
 {
     return (node_id - MMIGRID_POPUP_SHORTCUT_ITEM_START);
 }     
 return ~0;
}
 
 /*****************************************************************************/
//  Description : HandleWidgetAddMenuWinMsg
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateGridPopupMenu(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_WIN_ID_T  win_id)
{
    uint16 i = 0;
    uint16 k = 0;
    uint16 root_item_num = 0;
    MMI_STRING_T  string = {0};
    GUIMENU_DYNA_ITEM_T menu_item = {0};

              
    GUIMENU_CreatDynamicEx(200, PNULL, win_id, MMIGRID_POP_MENU_ID, GUIMENU_STYLE_POPUP_AUTO);

#ifdef MAIN_MENU_60X60    
    GUIMENU_SetPopMenuItemHeight(MMIGRID_POP_MENU_ID, 64);
#elif defined MAIN_MENU_48X48
    GUIMENU_SetPopMenuItemHeight(MMIGRID_POP_MENU_ID, 52);
#endif

    MMI_GetLabelTextByLang(TXT_WIDGET_SET_TITLE, &string);

    GUIMENU_SetMenuTitle(&string, MMIGRID_POP_MENU_ID);

    root_item_num = ARRAY_SIZE(s_grid_popmenu_group);
    
    for(i = 0; i < root_item_num;i++)
    {
        MMI_GetLabelTextByLang(s_grid_popmenu_group[i][MMIGRID_MENU_TXT_ID], &string);
        menu_item.item_text_ptr = &string;
        menu_item.select_icon_id = s_grid_popmenu_group[i][MMIGRID_MENU_IMG_ID];
        GUIMENU_InsertNode(k++, s_grid_popmenu_group[i][MMIGRID_MENU_NODE_ID],0,&menu_item,MMIGRID_POP_MENU_ID);        
    }

    AddShortCutItems(idle_grid_ptr, MMIGRID_POP_MENU_ID, MMIGRID_POPUP_MENU_SHORTCUT);
    AddWidgetItems(idle_grid_ptr, MMIGRID_POP_MENU_ID, MMIGRID_POPUP_MENU_WIDGET);
}

/*****************************************************************************/
//  Description : HandleWidgetAddMenuWinMsg
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridAddItemWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)(MMK_GetWinAddDataPtr(win_id ));
    MMI_RESULT_E          result        = MMI_RESULT_TRUE;
    uint32                node_id = 0;
    uint32                widget_id = 0;
        
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW: 
            CreateGridPopupMenu(idle_grid_ptr, win_id);
            break;
            
        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            node_id = GUIMENU_GetCurNodeId(MMIGRID_POP_MENU_ID);

			if(MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
			{
				//enter eding mode
#ifdef QBTHEME_SUPPORT
			 if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
			 {
				 if ((node_id >= MMIGRID_POPUP_WIDGET_ITEM_START && node_id < MMIGRID_POPUP_WIDGET_ITEM_MAX)||(node_id >= MMIGRID_POPUP_SHORTCUT_ITEM_START))
				 {
					MMIQBIDLE_SetWidgetWinMode(idle_grid_ptr,MMIGRID_RUN_MODE_EDITING);
				 }

			 }
#endif
			}
            if(node_id >= MMIGRID_POPUP_WIDGET_ITEM_START && node_id < MMIGRID_POPUP_WIDGET_ITEM_MAX)
            {
                widget_id = (node_id - MMIGRID_POPUP_WIDGET_ITEM_START);
                
                MMIAPIGRID_AddNewWidget(widget_id);  
#ifdef QBTHEME_SUPPORT
                MMIQBIDLE_AddNewWidget(widget_id);      
#endif                
            }
            else
            if((node_id >= MMIGRID_POPUP_SHORTCUT_ITEM_START))
            {
                uint16 menu_id = (node_id - MMIGRID_POPUP_SHORTCUT_ITEM_START);
                MMIMAINMENU_ITEM_INFO_T menu_info = {0};
                
                menu_info.group_id = idle_grid_ptr->grid_menu_info.grid_menu[menu_id].group_id;
                menu_info.menu_id  = idle_grid_ptr->grid_menu_info.grid_menu[menu_id].menu_id;                

                MMIGRID_AddNewShortcut(&menu_info);     
#ifdef QBTHEME_SUPPORT
                MMIQBIDLE_AddNewShortcut(&menu_info);    
#endif                
                
            }
            else
            if (MMIGRID_POPUP_MENU_WALLPAPER == node_id)
            {
#if defined MMI_WALLPAPER_SUPPORT
                MMIAPISET_SetWallpaper();
#endif
            }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
            else
            if (MMIGRID_POPUP_MENU_GAME_WALLPAPER == node_id)
            {
                MMIAPISET_SetGameWallpaper();
            }
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
            else 
            if(MMIGRID_POPUP_MENU_FILM_WALLPAPER == node_id)
            {
                MMIAPISET_SetFilmWallpaper();
            }
#endif 
    
            MMK_CloseWin(win_id);
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
    
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}


/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                        	     MMI_CTRL_ID_T      ctrl_id,        //控件id
                        	     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                                 MMI_IMAGE_ID_T     img_id,
                        	     uint16          	node_id,        //节点编号
                        	     uint16          	parent_node_id, //父节点编号
                        	     uint16          	index           //位置
                                 )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;

    if(img_id != 0)
    {
        node_item.select_icon_id = img_id;
    }

    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*****************************************************************************/
//  Description : create grid idle menu.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateGridMenu(MMI_WIN_ID_T  win_id)
{
    uint16 i = 0;
    uint16 root_item_num = 0;

    GUIMENU_CreatDynamic(PNULL, win_id, MMIGRID_IDLE_MENU_ID, GUIMENU_STYLE_POPUP);              

    root_item_num = ARRAY_SIZE(s_grid_idle_menu_group);
    
    for(i = 0; i < root_item_num; i++)
    {
        InsertMenuNodeByLabel(MMIGRID_IDLE_MENU_ID, s_grid_idle_menu_group[i][MMIGRID_MENU_TXT_ID], s_grid_idle_menu_group[i][MMIGRID_MENU_IMG_ID],s_grid_idle_menu_group[i][MMIGRID_MENU_NODE_ID], 0, i);
    }
}

/*****************************************************************************/
//  Description : Handle grid idle menu win.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridMenuWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)(MMK_GetWinAddDataPtr(win_id ));
    MMI_RESULT_E          result        = MMI_RESULT_TRUE;
    uint32                node_id = 0;
    
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW: 
            CreateGridMenu(win_id);
            MMK_SetAtvCtrl(win_id, MMIGRID_IDLE_MENU_ID);
            break;
            
        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            node_id = GUIMENU_GetCurNodeId(MMIGRID_IDLE_MENU_ID);

            if(MMIGRID_IDLE_MENU_ADD == node_id)
            {
                MMIWIDGET_CreatePopupAddWin(idle_grid_ptr);
            }
            else
            if (MMIGRID_IDLE_MENU_WALLPAPER == node_id)
            {
#if defined MMI_WALLPAPER_SUPPORT
                MMIAPISET_SetWallpaper(); 
#endif
            }
#ifdef SEARCH_SUPPORT            
            else
            if (MMIGRID_IDLE_MENU_SEARCH == node_id)
            {
                MMIAPISEARCH_Entry();
            }
#endif                        
            else
            if (MMIGRID_IDLE_MENU_NOTIFY == node_id)
            {
#if defined (PDA_UI_DROPDOWN_WIN) && defined (MMI_PDA_SUPPORT)     
				MMIDROPDOWNWIN_Open();
#endif				
            }
            else
            if (MMIGRID_IDLE_MENU_SETTING == node_id)
            {
                MMIAPISET_EnterPhoneSettingWin();
            }
#ifdef MMI_TASK_MANAGER
            else
            if (MMIGRID_IDLE_MENU_APP_MANAGER == node_id)
            {
                MMI_OpenAppletManager();
            }
#endif            
            MMK_CloseWin(win_id);
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
    
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : creat popup menu to add new widget/shortcuts to desktop.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_CreatePopupAddWin(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMK_CreateWin((uint32*)MMI_QBTHEME_IDLE_ADD_WIN_TAB, (ADD_DATA)idle_grid_ptr);
}

/*****************************************************************************/
//  Description : creat grid idle menu win.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_CreateGridMenuWin(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{   
    MMK_CreateWin((uint32*)MMI_QBTHEME_IDLE_OPTION_WIN_TAB, (ADD_DATA)idle_grid_ptr);
}

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  FUNCTION:      MMIQBTHEME_CreateShortCutWin
//  Description :
//  Global resource dependence : 
//  Author:        juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_CreateAddShortCutWin(void)
{    
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    MMK_CreateWin((uint32*)MMIQBTHEME_ADD_SHORTCUT_WIN_TAB, (ADD_DATA)idle_grid_ptr);
}

/*****************************************************************************/
//  FUNCTION:      MMIQBIDLE_CreateAddWidgetWin
//  Description :
//  Global resource dependence : 
//  Author:        juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_CreateAddWidgetWin(void)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    MMK_CreateWin((uint32*)MMIQBTHEME_ADD_WIDGET_WIN_TAB, (ADD_DATA)idle_grid_ptr);
}

/*****************************************************************************/
//  Description : HandleAddShortcutWindow
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddShortcutWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    uint16       menu_id = 0;
    uint32       node_id = 0;
    MMI_RESULT_E recode  = MMI_RESULT_TRUE;
    MMI_STRING_T string  = {0};
    MMIMAINMENU_ITEM_INFO_T menu_info = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)(MMK_GetWinAddDataPtr(win_id ));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_CreatDynamicEx(idle_grid_ptr->grid_menu_info.menu_num, PNULL, win_id, MMIQBTHEME_SHORTCUT_MENU_ID, GUIMENU_STYLE_POPUP_AUTO);
        MMI_GetLabelTextByLang(TXT_WIDGET_ADD_SHORT, &string);
        GUIMENU_SetMenuTitle(&string, MMIQBTHEME_SHORTCUT_MENU_ID);
        
#ifdef MAIN_MENU_60X60    
       GUIMENU_SetPopMenuItemHeight(MMIQBTHEME_SHORTCUT_MENU_ID, 64);
#endif
        AddShortCutItems(idle_grid_ptr, MMIQBTHEME_SHORTCUT_MENU_ID, 0);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(MMIQBTHEME_SHORTCUT_MENU_ID);
        menu_id = GetMenuid(node_id);
        menu_info.group_id = idle_grid_ptr->grid_menu_info.grid_menu[menu_id].group_id;
        menu_info.menu_id  = idle_grid_ptr->grid_menu_info.grid_menu[menu_id].menu_id;                
        MMIQBIDLE_AddNewShortcut(&menu_info);     
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}

/*****************************************************************************/
//  Description : HandleAddWidgetWindow
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddWidgetWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    uint32                node_id = 0;
    uint32                widget_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T string = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)(MMK_GetWinAddDataPtr(win_id ));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_CreatDynamicEx(idle_grid_ptr->shortcut.shortcut_num, PNULL, win_id, MMIQBTHEME_WIDGET_MENU_ID, GUIMENU_STYLE_POPUP_AUTO);
        MMI_GetLabelTextByLang(TXT_WIDGET_WIDGET, &string);
        GUIMENU_SetMenuTitle(&string, MMIQBTHEME_WIDGET_MENU_ID);
        AddWidgetItems(idle_grid_ptr, MMIQBTHEME_WIDGET_MENU_ID, 0);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(MMIQBTHEME_WIDGET_MENU_ID);
        widget_id =  GetWidgetid(node_id);
        MMIQBIDLE_AddNewWidget(widget_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
