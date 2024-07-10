/*************************************************************************
 ** File Name:      mmisearch_application.c                              *
 ** Author:         Xinhe.Yan                                            *
 ** Date:           2012/01/16                                           *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:    This file defines the function about search app      *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/01/16    Xinhe.Yan           Create.                            *
*************************************************************************/
#ifdef SEARCH_SUPPORT
#define _MMISEARCH_APPLICATION_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_search_trc.h"
#include "mmi_menutable.h"
#include "mmi_mainmenu_export.h"
#include "mmi_mainmenu_synchronize.h"
#include "mmi_appmsg.h"
#include "guilistbox.h"
#include "mmisearch_internal.h"
#include "mmisearch_export.h"
#include "mmi_resource.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMI_MENU_GROUP_ID_T         group_id;           // 菜单组ID */
    MMI_MENU_ID_T               menu_id;            // 菜单项ID */
    MMI_IMAGE_ID_T              item_icon1;         //static menu icon    
    MMI_STRING_T                item_str1;          //icon text 
    uint16                      dymenu_list_index;  //dynamic menu list index in dynamic menu global variables
} MMISEARCH_MENUDATA_STRUCT;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMISEARCH_MENUDATA_STRUCT * s_search_data_ptr = PNULL;

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
// 	Description : 本地搜索中app应用匹配项的"打开"回调函数
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
LOCAL void APP_OpenMatchedItem(uint32 index);

/*****************************************************************************/
// 	Description : 本地搜索中app应用匹配项的显示回调函数
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
LOCAL void APP_SetMatchItemData(
                                GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
                                uint32 cur_module_index
                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 本地搜索中app应用时不应被搜索到的项
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN APP_MenuSearchExclude( MMI_MENU_ID_T menu_id )
{
	BOOLEAN result = TRUE;
#if defined(MMI_ISTYLE_SUPPORT)	
	if( MMITHEME_IsIstyle() )
	{
		if( ID_MAINMENU_STYLE_SET == menu_id
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
			|| ID_GOMENU_CHANGE_STYLE == menu_id
#endif
			)
		{
			result = FALSE;
		}
	}
#endif	
	return result;
}

/*****************************************************************************/
// 	Description : 本地搜索中app应用匹配项的"打开"回调函数
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
LOCAL void APP_OpenMatchedItem(uint32 index)
{
    MMIMAINMENU_ITEM_INFO_T menu_item = {0};
    
    menu_item.group_id = s_search_data_ptr[index].group_id;
    menu_item.menu_id = s_search_data_ptr[index].menu_id;
    
#if defined(DYNAMIC_MAINMENU_SUPPORT)
    if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == s_search_data_ptr[index].group_id)
    {
        GUIMAINMENU_DY_ITEM_T item_info = {0};
        BOOLEAN is_info_get = FALSE;
        
        is_info_get = MMIMENU_DySlideGetRunInfo(menu_item.menu_id, &item_info);
        
        if (is_info_get
            && PNULL != item_info.ori_data_ptr
            && MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
        {
#ifdef MMI_ISTYLE_SUPPORT
            //to cut down memory for WRE
            if ( MMIAPIMENU_IsInIsyleSearch())
            {
                MMIAPIMENU_ReleaseIstyleSearch();
            }            
#endif

            (item_info.ori_data_ptr->dynamic_menu_info.link_function_ptr)(
                item_info.ori_data_ptr->dynamic_menu_info.param1ptr, 
                item_info.ori_data_ptr->dynamic_menu_info.param2ptr
                );
            
            MMIMENU_DyFreeParmInfo(&item_info);
        }
        
        return ;
    }
#endif

#if defined MMI_GRID_IDLE_SUPPORT || defined MMI_SMART_IDLE_SUPPORT
    MMIAPIMENU_HandleIdleMenuItem(menu_item.group_id, menu_item.menu_id);
    // MMIAPIMENU_HandleMainMenuWinMsg(VIRTUAL_WIN_ID,MSG_GRID_PEN_MENU_ITEM,&menu_item);
#else
    //SCI_TRACE_LOW:"MMI Search: APP_OpenMatchedItem MACRO MMI_GRID_IDLE_SUPPORT not defined"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_108_112_2_18_2_49_58_1,(uint8*)"");
#endif
}

/*****************************************************************************/
// 	Description : 本地搜索中app应用匹配项的显示回调函数
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
PUBLIC void APP_SetMatchItemContent(
                                          GUILIST_NEED_ITEM_CONTENT_T * need_item_content_ptr,
                                          uint32 cur_module_index
                                          )
{
    if (PNULL == need_item_content_ptr)
    {
        return ;
    }
    
#if defined(DYNAMIC_MAINMENU_SUPPORT)
    if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == s_search_data_ptr[cur_module_index].group_id)
    {
        GUIITEM_CONTENT_T           item_content = {0};        
        GUIANIM_DATA_INFO_T         anim_data = {0};
        GUIMAINMENU_DY_ITEM_T  item_info = {0};
        
        if (MMIMENU_DyListGetDispInfo(s_search_data_ptr[cur_module_index].dymenu_list_index,&item_info))
        {
            if (PNULL != item_info.ori_data_ptr
                && item_info.ori_data_ptr->dynamic_menu_info.has_icon
                && PNULL != item_info.ori_data_ptr->dynamic_menu_info.icon_ptr)
            {
                item_content.item_data_type = GUIITEM_DATA_ANIM_DATA;
                item_content.item_data.anim_data_ptr = &anim_data;

                anim_data.is_bitmap = FALSE;
                anim_data.is_save_data = TRUE;
                anim_data.data_ptr = item_info.ori_data_ptr->dynamic_menu_info.icon_ptr;
                anim_data.data_size = item_info.ori_data_ptr->dynamic_menu_info.icon_datasize;
                
                GUILIST_SetItemContent(need_item_content_ptr->ctrl_id,
                    &item_content,
                    need_item_content_ptr->item_index,
                    need_item_content_ptr->item_content_index);
                
                return;
            }            
        }        
        //SCI_TRACE_LOW:"APP_SetMatchItemContent: dynamic app not found"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_155_112_2_18_2_49_58_2,(uint8*)"");
    }
#endif    
}

/*****************************************************************************/
// 	Description : 本地搜索中app应用匹配项的显示回调函数
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
LOCAL void APP_SetMatchItemData(
                                GUILIST_NEED_ITEM_DATA_T * need_item_data_ptr,
                                uint32 cur_module_index
                                )
{   
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_COMMON_OPEN;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
        
#if defined(DYNAMIC_MAINMENU_SUPPORT) && !defined MMI_MAINMENU_MINI_SUPPORT
    if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == s_search_data_ptr[cur_module_index].group_id)
    {
        GUIMAINMENU_DY_ITEM_T item_info = {0};
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_MAINMENU_ICON_DYNAMIC_DEFAUL;

        if (MMIMENU_DySlideGetDispInfo(s_search_data_ptr[cur_module_index].menu_id,&item_info))
        {
            if (PNULL !=item_info.ori_data_ptr && item_info.ori_data_ptr->dynamic_menu_info.has_icon)
            {
                BOOLEAN is_get_icon = FALSE;
                
                is_get_icon = MMIMENU_DySlideGetIcon(item_info.ori_data_ptr->menu_id,
                    item_info.ori_data_ptr->dynamic_menu_info.icon_ptr);
                
                if (is_get_icon
                    && PNULL != item_info.ori_data_ptr->dynamic_menu_info.icon_ptr)
                {
                    item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"APP_SetMatchItemData: dynamic app not found"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_199_112_2_18_2_49_58_3,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"APP_SetMatchItemData: item don not have icon"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_204_112_2_18_2_49_58_4,(uint8*)"");
        }

        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = s_search_data_ptr[cur_module_index].item_str1;

        GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
        
        return;
    }
#endif
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = s_search_data_ptr[cur_module_index].item_icon1;
    
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer = s_search_data_ptr[cur_module_index].item_str1;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_COMMON_OPEN;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
    
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);    
}


/*****************************************************************************/
// 	Description : 本地搜索退出，app模块释放模块内部资源
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAPP_FreeLocalSearchResource(void)
{
    //GUIMENU_GROUP_T *               group_ptr = PNULL;
    uint16                          i = 0;
    uint16                          append_count = 0;
    MMI_STRING_T                    item_str = {0};
    MMI_STRING_T                    keyword_str = {0};
    MMIMAINEMENU_ITEM_LIST_T        *item_list_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T         *item_data_ptr = PNULL;
    CTRLMAINMENU_ITEM_T             item           = {0};
    MMI_LINK_NODE_T                 *node_ptr = PNULL;
	
    if(PNULL == s_search_data_ptr)
    {
        return;
    }
    else
    {
        item_list_ptr = MMIMAINMENU_GetItemList();
        keyword_str = MMISEARCH_GetKeyWord();
        
        if(PNULL != item_list_ptr )
        {
            node_ptr = item_list_ptr->list_head_ptr;
            if(PNULL == node_ptr)
            {
                //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5,(uint8*)"");
                return ;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5,(uint8*)"");
            return ;
        }
        
        for (i = 0; i < item_list_ptr->item_count; i++)
        {
            item_data_ptr = (MMIMAINMENU_ITEM_INFO_T*)node_ptr->data;
            
            if (PNULL == item_data_ptr)
            {
                break;
            }
            
            node_ptr = node_ptr->next_ptr;
            
            if (!MMIMAINMENU_GetItemData(item_data_ptr->menu_id, item_data_ptr->group_id, &item))
            {
                continue;
            }
            
            if(CTRL_ICON_ID != item.icon.type)
            {
                continue;
            }
            
            if (CTRL_TEXT_ID == item.text.type)
            {
                MMIRES_GetText(item.text.data.text_id/*group_ptr->item_ptr[i].text_str_id*/,PNULL,&item_str);
            }
            else
            {
                item_str.wstr_len = item.text.data.str_info.wstr_len;
                item_str.wstr_ptr = item.text.data.str_info.wstr_ptr;
            }
            
            if (0 == item_str.wstr_len || PNULL == item_str.wstr_ptr)
            {
                continue;
            }
            
            if ( MMIAPICOM_WstrnstrExt(item_str.wstr_ptr,item_str.wstr_len, keyword_str.wstr_ptr,keyword_str.wstr_len,FALSE)
                && APP_MenuSearchExclude(item_data_ptr->menu_id) )
            {
                append_count++;
            }
            // 
        }
        for(i=0; i<append_count; i++)
        {
            if(PNULL != s_search_data_ptr[i].item_str1.wstr_ptr)
            {
                SCI_FREE(s_search_data_ptr[i].item_str1.wstr_ptr);
            }
        }
        SCI_FREE(s_search_data_ptr)
    }
}
    
/*****************************************************************************/
// 	Description : 本地搜索中app应用搜索接口
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_LocalSearchAPP(
                                  const MMI_STRING_T *key_word_str, 
                                  MMISEARCH_DATA_T  *match_data_ptr,
                                  uint32 max_num
                                  )
{
    uint16                          i = 0;
    uint16                          append_count = 0;
    MMI_STRING_T                    item_str = {0};    
    // GUIMENU_GROUP_T *               group_ptr = PNULL;
    MMISEARCH_MATCH_ITEM_T*         match_item_ptr = PNULL;
    MMIMAINEMENU_ITEM_LIST_T        *item_list_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T         *item_data_ptr = PNULL;
    CTRLMAINMENU_ITEM_T             item           = {0};
    MMI_LINK_NODE_T                 *node_ptr = PNULL;
        
    if (PNULL == key_word_str || PNULL == key_word_str->wstr_ptr|| PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr)
    {
        return ;
    }

    item_list_ptr = MMIMAINMENU_GetItemList();

    match_item_ptr = match_data_ptr->match_item_ptr;
    
    if (PNULL == s_search_data_ptr)
    {    
        s_search_data_ptr = (MMISEARCH_MENUDATA_STRUCT*)SCI_ALLOCAZ( MMIAPP_SEARCH_MAX_NUM*sizeof(MMISEARCH_MENUDATA_STRUCT));
        
        if (PNULL == s_search_data_ptr)
        {
            return ;
        }
    }
    else
    {
        SCI_MEMSET(s_search_data_ptr,0,MMIAPP_SEARCH_MAX_NUM*sizeof(MMISEARCH_MENUDATA_STRUCT));
    }
	 if(PNULL != item_list_ptr )
       {
	    node_ptr = item_list_ptr->list_head_ptr;
	    if(PNULL == node_ptr)
	    {
		    //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5,(uint8*)"");
	           return ;
	    }
	 }
	 else
	 {
		  //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5,(uint8*)"");
	        return ;
	 }

    for (i = 0; i < item_list_ptr->item_count; i++)
    {
        item_data_ptr = (MMIMAINMENU_ITEM_INFO_T*)node_ptr->data;

        if (PNULL == item_data_ptr)
        {
            break;
        }

        node_ptr = node_ptr->next_ptr;

        if (!MMIMAINMENU_GetItemData(item_data_ptr->menu_id, item_data_ptr->group_id, &item))
        {
            continue;
        }

        if(CTRL_ICON_ID != item.icon.type)
        {
            continue;
        }

        if (CTRL_TEXT_ID == item.text.type)
        {
            MMIRES_GetText(item.text.data.text_id/*group_ptr->item_ptr[i].text_str_id*/,PNULL,&item_str);
        }
        else
        {
            item_str.wstr_len = item.text.data.str_info.wstr_len;
            item_str.wstr_ptr = item.text.data.str_info.wstr_ptr;
        }

        if (0 == item_str.wstr_len || PNULL == item_str.wstr_ptr)
        {
            continue;
        }
        
        if ( MMIAPICOM_WstrnstrExt(item_str.wstr_ptr,item_str.wstr_len,key_word_str->wstr_ptr,key_word_str->wstr_len, FALSE)
			&& APP_MenuSearchExclude(item_data_ptr->menu_id) )
        {
            if (append_count >= max_num)
            {
                //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: append_count cross MMIAPP_SEARCH_MAX_NUM limited"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_304_112_2_18_2_49_58_6,(uint8*)"");
                break;
            }

            s_search_data_ptr[append_count].item_icon1 = item.icon.data.icon_id;//group_ptr->item_ptr[i].unselect_icon_id;
            s_search_data_ptr[append_count].item_str1.wstr_ptr = (wchar*)SCI_ALLOCAZ((item_str.wstr_len+1)*sizeof(wchar));
            if(PNULL !=s_search_data_ptr[append_count].item_str1.wstr_ptr )
            {
                MMI_WSTRNCPY(s_search_data_ptr[append_count].item_str1.wstr_ptr,item_str.wstr_len+1,item_str.wstr_ptr,item_str.wstr_len+1,item_str.wstr_len);
                s_search_data_ptr[append_count].item_str1.wstr_len = item_str.wstr_len;
            }
            else
            {
                break;
            }
            s_search_data_ptr[append_count].menu_id = item_data_ptr->menu_id;// group_ptr->item_ptr[i].menu_id;            
            
            match_item_ptr[append_count].match_item_type = MMISEARCH_TYPE_APP;
            match_item_ptr[append_count].match_item_style = GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT;
            match_item_ptr[append_count].user_data = append_count;
            match_item_ptr[append_count].open_pfunc = APP_OpenMatchedItem;
            match_item_ptr[append_count].set_data_pfunc = APP_SetMatchItemData;
            match_item_ptr[append_count].set_content_pfunc = APP_SetMatchItemContent;
            append_count++;
        }
    }

    //search dynamic menu
#if defined(DYNAMIC_MAINMENU_SUPPORT)
    {
        GUIMAINMENU_DY_ITEM_T item_info = {0};
        uint16              dymenu_num = 0;
        
        dymenu_num =MMIMENU_DyGetItemCount();
        
        if (0 ==dymenu_num)
        {
            match_data_ptr->match_num = append_count;
            match_data_ptr->match_item_ptr = match_item_ptr;
            return ;
        }
        
        for (i = 0; i < dymenu_num; i++)
        {
            MMIMENU_DyListGetDispInfoEx(i,&item_info);

            if (PNULL !=item_info.ori_data_ptr && MMIAPICOM_WstrnstrExt(item_info.ori_data_ptr->dynamic_menu_info.text,item_info.ori_data_ptr->dynamic_menu_info.text_len,key_word_str->wstr_ptr,key_word_str->wstr_len, FALSE))
            {
                if (append_count >= max_num)
                {
                    //SCI_TRACE_LOW:"MMISEARCH_LocalSearchAPP: dyanmic append_count cross MMIAPP_SEARCH_MAX_NUM limited"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_SEARCHAPPLICATION_344_112_2_18_2_49_58_7,(uint8*)"");
                    break;
                }

                s_search_data_ptr[append_count].item_str1.wstr_ptr = item_info.ori_data_ptr->dynamic_menu_info.text;
                s_search_data_ptr[append_count].item_str1.wstr_len = item_info.ori_data_ptr->dynamic_menu_info.text_len;
                s_search_data_ptr[append_count].menu_id = item_info.ori_data_ptr->menu_id;
                s_search_data_ptr[append_count].dymenu_list_index = i;
                s_search_data_ptr[append_count].group_id = DYNAMIC_MAINMENU_DEFAULT_GROUP_ID;
                
                
                match_item_ptr[append_count].match_item_type = MMISEARCH_TYPE_APP;
                match_item_ptr[append_count].match_item_style = GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT;
                match_item_ptr[append_count].user_data = append_count;
                match_item_ptr[append_count].open_pfunc = APP_OpenMatchedItem;
                match_item_ptr[append_count].set_data_pfunc = APP_SetMatchItemData;
                match_item_ptr[append_count].set_content_pfunc = APP_SetMatchItemContent;
                append_count++;
            }
        }   
    }    
#endif

    match_data_ptr->match_num = append_count;
    match_data_ptr->match_item_ptr = match_item_ptr;
}
#endif
