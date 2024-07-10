/*************************************************************************
 ** File Name:      mmiim_touch_set.c                                    *
 ** Author:         Jassmine                                             *
 ** Date:           2011/12/14                                           *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:    This file defines the function about set im          *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/12/14    Xinhe.Yan           Create.                            *
*************************************************************************/

#define _MMIIM_TOUCH_SET_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_im_trc.h"
#include "mmiim_touch_set.h"
#include "mmitheme_pos.h"
#include "guires.h"
#include "mmiim_im_switching.h"
#include "mmi_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//SETTING_IM_RECT_HEIGHT must be n.5 times of SETTING_IM_ITEM_HEIGHT
//SETTING_IM_ITEM_HEIGHT must be even number

#if defined (MMI_IM_PDA_SUPPORT)

#if defined MAINLCD_SIZE_240X320
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 198
#define SETTING_IM_ITEM_HEIGHT 36
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 176
#define SETTING_IM_ITEM_HEIGHT_H 32
#define SETTING_IM_FONT SONG_FONT_20
#elif defined MAINLCD_SIZE_240X400
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 198
#define SETTING_IM_ITEM_HEIGHT 36
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 176
#define SETTING_IM_ITEM_HEIGHT_H 32
#define SETTING_IM_FONT SONG_FONT_20
#elif defined MAINLCD_SIZE_320X480
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 220
#define SETTING_IM_ITEM_HEIGHT 40 
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 220
#define SETTING_IM_ITEM_HEIGHT_H 40
#define SETTING_IM_FONT SONG_FONT_22
#else 
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 220
#define SETTING_IM_ITEM_HEIGHT 40
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 220
#define SETTING_IM_ITEM_HEIGHT_H 40
#define SETTING_IM_FONT SONG_FONT_22
#endif

#define SETTINGIM_DARK_COLOR 0xFF3C3B3C
#define SETTINGIM_BRIGHT_COLOR 0xFF272627
#define SETTINGIM_ITEM_COLOR 0xFF313031

#define SETTINGIM_DOWN_HALF FALSE //下面多半个item显示
#define SETTINGIM_UP_HALF TRUE	//上面多半个item显示
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
//	Description: handle set im slide timer msg
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:在手放到im选择界面的最上面和最下面半个区域，使用timer滑动显示im
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIM_HandleSettingIMTimerMSG(
                                                 uint8 timer_id, 
                                                 uint32 param
                                                 );

/*****************************************************************************/
//	Description: is slide upward set key
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:是否向上划过set键
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIM_IsSettingIMSlide(
                                          IM_DATA_T *data_ptr,
                                          GUIIM_EVENT_DATA_U const *event_data_ptr
                                          );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description: create layer for setting im 
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:istyle创建输入法选择界面，使用层来显示
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_CreateSettingIMLayer(
                                               IM_DATA_T *data_ptr,
                                               KEY_INFO_T *key_ptr
                                               )
{
    int16                   index =0 ;//indicate index of setting_menu_data
    int16                   setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16                   setting_im_rect_height = SETTING_IM_RECT_HEIGHT;
    int16                   setting_im_item_height = SETTING_IM_ITEM_HEIGHT;
    UILAYER_CREATE_T        create_info = {0};
    UILAYER_APPEND_BLT_T    append_layer = {0};
    MMI_RESULT_E            result = MMI_RESULT_FALSE;
    uint32 item_idx = 0;
        
    CHECK_DATA_EX(data_ptr);
    
    if (VKEY_SET != key_ptr->code)
    {
        return result;
    }   
    
    if(TRUE == data_ptr->is_setting_display)
    {
        //SCI_TRACE_LOW:"mmiim_touch_set.c: MMIIM_CreateSettingIMLayer setting_im_layer is already created"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_SET_133_112_2_18_2_28_14_195,(uint8*)"");
        return result;
    }
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        setting_im_rect_height = SETTING_IM_RECT_HEIGHT_H;
        setting_im_item_height = SETTING_IM_ITEM_HEIGHT_H;
    }

    //english + local language
    {
        //static max number of im languages in current version
        for (item_idx = 0; item_idx < data_ptr->setting_menu_size; item_idx++)
        {
            //首次download版本
            if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)
            {
                GUIIM_LANGUAGE_T guiim_disp_lang = GUIIM_LANG_NONE;
                MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
                
                MMIAPISET_GetLanguageType(&language_type);
                guiim_disp_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
                
                //可能当前显示语言也为英文
                if (guiim_disp_lang == GUIIM_LANG_NONE || guiim_disp_lang == GUIIM_LANG_ENGLISH)
                {
                    continue;
                }
                else
                {
                    MMIAPIIM_SetPrevLanguage(guiim_disp_lang);
                }
            }
            else if (MMIAPIIM_GetPrevLanguage() != data_ptr->setting_menu_data[item_idx].lang    //非前一次使用的非英语语言
                && GUIIM_LANG_NONE != data_ptr->setting_menu_data[item_idx].lang        //其他类型菜单，没有设置语言
                && GUIIM_LANG_ENGLISH != data_ptr->setting_menu_data[item_idx].lang      //英文为标配
                )    //
            {
                continue;
            }
            
            //英文手写和其他语言手写只匹配其中一个
            if (data_ptr->setting_menu_data[item_idx].lang == GUIIM_LANG_ENGLISH
                && data_ptr->setting_menu_data[item_idx].input == GUIIM_INPUT_TYPE_HANDWRITING)		//英文手写
            {
                if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE
                    && MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)	//有且仅有英语输入时
                {
                    
                }
                else
                {
                    continue;
                }
            }

            if ((GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
                && item_idx < 3)
            {
                //node_item.is_grayed = TRUE;
                continue;
            }
            //else
            //{
            //    node_item.is_grayed = FALSE;
            //}
            
            index++;
        }
        
        data_ptr->settingim_item_count  = index ; //store max number of languages
        
        //alloc memory for data_ptr->s_settingim_menu_index
        data_ptr->settingim_menu_index = (int16 *)SCI_ALLOCA( data_ptr->settingim_item_count * sizeof(int16));  /*lint !e737*/
        
        if(PNULL == data_ptr->settingim_menu_index)
        {
            //SCI_TRACE_LOW:"mmiim_touch_set.c: MMIIM_CreateSettingIMLayer data_ptr->settingim_menu_index malloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_SET_212_112_2_18_2_28_14_196,(uint8*)"");
            
            return result;
        }
        
        //store actual used im languages in current version to data_ptr->settingim_menu_index
        index = 0;
        
        for (item_idx = 0; item_idx < data_ptr->setting_menu_size; item_idx++)
        {
            //首次download版本
            if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)
            {
                GUIIM_LANGUAGE_T guiim_disp_lang = GUIIM_LANG_NONE;
                MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
                
                MMIAPISET_GetLanguageType(&language_type);
                guiim_disp_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
                
                //可能当前显示语言也为英文
                if (guiim_disp_lang == GUIIM_LANG_NONE || guiim_disp_lang == GUIIM_LANG_ENGLISH)
                {
                    continue;
                }
                else
                {
                    MMIAPIIM_SetPrevLanguage(guiim_disp_lang);
                }
            }
            else if (MMIAPIIM_GetPrevLanguage() != data_ptr->setting_menu_data[item_idx].lang    //非前一次使用的非英语语言
                && GUIIM_LANG_NONE != data_ptr->setting_menu_data[item_idx].lang        //其他类型菜单，没有设置语言
                && GUIIM_LANG_ENGLISH != data_ptr->setting_menu_data[item_idx].lang      //英文为标配
                )    //
            {
                continue;
            }
            
            //英文手写和其他语言手写只匹配其中一个
            if (data_ptr->setting_menu_data[item_idx].lang == GUIIM_LANG_ENGLISH
                && data_ptr->setting_menu_data[item_idx].input == GUIIM_INPUT_TYPE_HANDWRITING)		//英文手写
            {
                if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE
                    && MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)	//有且仅有英语输入时
                {
                    
                }
                else
                {
                    continue;
                }
            }

            if ((GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
                && item_idx < 3)
            {
                //node_item.is_grayed = TRUE;
                continue;
            }

            //recode language index to settingim_menu_index
            data_ptr->settingim_menu_index[index++] = item_idx;
            
            if (index >= data_ptr->settingim_item_count)
            {
                break;
            }
        }
    }
    
    //create layer
    create_info.lcd_id = data_ptr->init_param.lcd_info_ptr->lcd_id;
    create_info.owner_handle = data_ptr->init_param.im_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = setting_im_rect_width;
    create_info.height = (data_ptr->settingim_item_count * setting_im_item_height < setting_im_rect_height)? 
        (data_ptr->settingim_item_count * setting_im_item_height):setting_im_rect_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(&create_info, &data_ptr->setting_im_layer);
    
    UILAYER_RemoveBltLayer(&data_ptr->setting_im_layer);
    
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info = data_ptr->setting_im_layer;
    UILAYER_AppendBltLayer(&append_layer);
    
    data_ptr->is_setting_display = TRUE;
    
    //initialize global params
    data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
    data_ptr->settingim_top_item = 0;
    data_ptr->settingim_last_highlight = 0;
    
    result = MMI_RESULT_TRUE;
    
    return result;
}

/*****************************************************************************/
//	Description: release setting im layer
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:删除切换输入法创建的图层
/*****************************************************************************/
PUBLIC void MMIIM_ReleaseSettingIMLayer(IM_DATA_T *data_ptr)
{  
    CHECK_DATA(data_ptr);

    UILAYER_RELEASELAYER(&data_ptr->setting_im_layer);      /*lint !e774*/
}

/*****************************************************************************/
//	Description: destroy setting im data
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:release setting im layer and free settingim_menu_index
/*****************************************************************************/
PUBLIC void MMIIM_DestroySettingIMData(IM_DATA_T *data_ptr)
{        
    uint8 key_idx = 0;

    CHECK_DATA(data_ptr);

    if (data_ptr->settingim_timer_id != 0)
    {  
        MMK_StopTimer(data_ptr->settingim_timer_id);

        data_ptr->settingim_timer_id = 0 ;
    }
    	
    SCI_FREE(data_ptr->settingim_menu_index);

    MMIIM_ReleaseSettingIMLayer(data_ptr);

    if (FALSE == data_ptr->is_setting_display)
    {
        return ;
    }

    data_ptr->is_setting_display = FALSE;

//NEWMS00180440 [6530][64X64_320X240TK_QW]cstar手写中点击输入法切换键，该键一直高亮
#if 0           //需求变更，不需要写掉该函数，尤其是全屏手写
    //recover VKEY_SET move function
    for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
    {       
        if (VKEY_SET == data_ptr->vkb.keys[key_idx].code)
        {
            data_ptr->vkb.keys[key_idx].f_move= VkbMove;
            
            break;
        }
    }
#endif    
}

/*****************************************************************************/
//	Description: handle set im slide timer msg
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:在手放到im选择界面的最上面和最下面半个区域，使用timer滑动显示im
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIM_HandleSettingIMTimerMSG(
                                                 uint8 timer_id, 
                                                 uint32 param
                                                 )
{
    IM_DATA_T *     data_ptr = (IM_DATA_T *)param;
    int16           setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16           setting_im_rect_height = SETTING_IM_RECT_HEIGHT;
    int16           setting_im_item_height = SETTING_IM_ITEM_HEIGHT;	
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

    CHECK_DATA_EX(data_ptr);
    
    if (0 == timer_id || timer_id != data_ptr->settingim_timer_id)
    {
        return result;
    }
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_height = SETTING_IM_RECT_HEIGHT_H;
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        setting_im_item_height = SETTING_IM_ITEM_HEIGHT_H;
    }
    
    //change half item position
    if (SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
    {
        data_ptr->settingim_halfposition = SETTINGIM_UP_HALF;
    }
    else
    {
        data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
    }
    
    //hold top rect of im set area, surface move downward 
    if (data_ptr->settingim_last_highlight == 0)
    {
        if (data_ptr->settingim_top_item >0)
        {
            //move forward one item
            if(SETTINGIM_UP_HALF == data_ptr->settingim_halfposition)
            {			
                data_ptr->settingim_top_item--;
            }
        }
        else
        {
            MMK_StopTimer(data_ptr->settingim_timer_id);
            data_ptr->settingim_timer_id = 0;
            
            //reset params
            data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
            data_ptr->settingim_last_highlight = 0;
            
            MMIIM_DrawSettingIm(data_ptr, FindKey(data_ptr,&data_ptr->vkb.down_point),TRUE,data_ptr->settingim_last_highlight);
            
            return result;
        }
    }
    
    if (data_ptr->settingim_last_highlight == (int16)(setting_im_rect_height/setting_im_item_height) -1)
    {      
        //move to bottom half rect of setting im,data_ptr->settingim_last_highlight have not change
        data_ptr->settingim_last_highlight++;
    }
    else if(data_ptr->settingim_last_highlight == (int16)(setting_im_rect_height/setting_im_item_height) )
    {
        if ( (data_ptr->settingim_top_item >= data_ptr->settingim_item_count - (int16)(setting_im_rect_height/setting_im_item_height) -1)
            && SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition  )
        {
            MMK_StopTimer(data_ptr->settingim_timer_id);
            data_ptr->settingim_timer_id = 0;
            
            //reset params
            data_ptr->settingim_halfposition = SETTINGIM_UP_HALF;
            data_ptr->settingim_last_highlight = (int16)(setting_im_rect_height/setting_im_item_height);
            
            MMIIM_DrawSettingIm(data_ptr, FindKey(data_ptr,&data_ptr->vkb.down_point),TRUE,data_ptr->settingim_last_highlight);
            
            return result;
        }
        else
        {
            //surface move half item every time
            if(SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
            {
                data_ptr->settingim_top_item++;
            }            
        }        
    }
    
    MMIIM_DrawSettingIm(data_ptr, FindKey(data_ptr,&data_ptr->vkb.down_point),FALSE,data_ptr->settingim_last_highlight);
    
    result = MMI_RESULT_TRUE;
    
    return result;
}

/*****************************************************************************/
//	Description: is slide upward set key
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:是否向上划过set键
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIM_IsSettingIMSlide(
                                          IM_DATA_T *data_ptr,
                                          GUIIM_EVENT_DATA_U const *event_data_ptr
                                          )
{
    uint8           i = 0;
    GUI_POINT_T     point = {0};
    GUI_POINT_T     pre_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

    CHECK_DATA_EX(data_ptr);
    
    point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
    point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
    
    pre_point.x = ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(event_data_ptr->sys_msg.param))->pre_tp_point.x;
    pre_point.y = ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(event_data_ptr->sys_msg.param))->pre_tp_point.y;
    
    if (pre_point.y <= point.y)
    {
        return result;
    }
    
    for (i = 0; i < data_ptr->vkb.key_cnt; i++)
    {       
        if (VKEY_SET == data_ptr->vkb.keys[i].code)
        {
            point.y -= data_ptr->rect.top;
            
            if (GUI_PointIsInRect( point, data_ptr->vkb.keys[i].rect))
            {
                result = MMI_RESULT_TRUE;
            }    
            
            break;
        }
    }	
    
	//delete slide set key module
	result = MMI_RESULT_FALSE;

    return result;
}

/*****************************************************************************/
//	Description: handle setting im move
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:处理istyle切换输入法时的手势移动
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_HandleSettingIMMove(
                                              IM_DATA_T *data_ptr,
                                              GUIIM_EVENT_DATA_U const *event_data_ptr
                                              )
{ 
    uint8           i = 0;
    int16           index =0;//indicate which item was slided 
    GUI_POINT_T     point = {0};
    GUI_RECT_T      list_rect = {0};//whole rect of setting im
    GUI_RECT_T      hold_rect = {0};  //half rect of top or bottom item
    GUI_RECT_T      highlight_item_rect = {0}; //item rect to highlight display
    int16           setting_im_rect_height = SETTING_IM_RECT_HEIGHT;
    int16           setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16           setting_im_item_height = SETTING_IM_ITEM_HEIGHT;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

    CHECK_DATA_EX(data_ptr);
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_height = SETTING_IM_RECT_HEIGHT_H;
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        setting_im_item_height = SETTING_IM_ITEM_HEIGHT_H;
    }
    
    point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
    point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
    
    UILAYER_GetLayerPosition(&data_ptr->setting_im_layer,&list_rect.left,&list_rect.top);
    
    list_rect.right = list_rect.left + setting_im_rect_width;
    list_rect.bottom = list_rect.top + ((data_ptr->settingim_item_count * setting_im_item_height< setting_im_rect_height)?
        (data_ptr->settingim_item_count * setting_im_item_height) : setting_im_rect_height);
    
    result = MMIIM_IsSettingIMSlide(data_ptr,event_data_ptr);
    
    if ( MMI_RESULT_TRUE == result)
    {
        //slide upward set key
        for (i = 0; i < data_ptr->vkb.key_cnt; i++)
        {       
            if (VKEY_SET == data_ptr->vkb.keys[i].code)
            {
                point.y -= data_ptr->rect.top;
                
                if (!GUI_PointIsInRect(point,data_ptr->vkb.keys[i].rect))
                {
                    //not in set key rect
                    return result;
                }
                
                if (PNULL != data_ptr->vkb.keys[i].f_move)
                {
                    data_ptr->vkb.keys[i].f_move= PNULL;
                    
                    data_ptr->vkb.last_key = PNULL;
                    
                    MMIIM_CreateSettingIMLayer(data_ptr,&data_ptr->vkb.keys[i]);
                    
                    MMIIM_DrawSettingIm(data_ptr,&data_ptr->vkb.keys[i],FALSE,0);
                }
                
                return result;
                
            }    
        }
    }
    else if (!GUI_PointIsInRect(point,list_rect))
    {
        if (data_ptr->settingim_timer_id != 0)
        {
            MMK_StopTimer(data_ptr->settingim_timer_id);
            data_ptr->settingim_timer_id = 0 ;
        } 
        
        return result;
    }
    
    //expand half item range
    if( SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
    {
        list_rect.bottom += setting_im_item_height/2;
    }
    else
    {    
        list_rect.top -= setting_im_item_height/2;
    }
    
    index = (point.y - list_rect.top)/setting_im_item_height;
    
    //not need scroll
    if (data_ptr->settingim_item_count <= (int16)(setting_im_rect_height/setting_im_item_height) )
    {
        //highlight item already draw
        if(index == data_ptr->settingim_last_highlight)
        {
            result = MMI_RESULT_TRUE;
            
            return result ;
        }
        else 
        {
            data_ptr->settingim_last_highlight = index;
        }
        
        //find set key
        for (i = 0; i < data_ptr->vkb.key_cnt; i++)
        {       
            if (VKEY_SET == data_ptr->vkb.keys[i].code)
            {
                break;
            }
        }	
        
        if (VKEY_SET != data_ptr->vkb.keys[i].code)
        {
            return result;
        }
        
        MMIIM_DrawSettingIm(data_ptr,&data_ptr->vkb.keys[i],TRUE,index); 
        
        result = MMI_RESULT_TRUE;
        
        return result;
    }
    
    highlight_item_rect = list_rect;
    highlight_item_rect.top += index*setting_im_item_height;
    highlight_item_rect.bottom= highlight_item_rect.top + setting_im_item_height;
    
    if (!GUI_PointIsInRect(point,highlight_item_rect))
    {
        //SCI_TRACE_LOW:"mmiim_touch_set.c: MMIIM_HandleSettingIMMove---point not in heightligth rect---data_ptr->settingim_top_item %d,point.y %d highlight top %d bottom %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_SET_650_112_2_18_2_28_15_197,(uint8*)"dddd",data_ptr->settingim_top_item,point.y,highlight_item_rect.top,highlight_item_rect.bottom);
        
        return result;
    }
    
    hold_rect = highlight_item_rect;
    
    //need scroll
    if( 0 ==index || ((int16)setting_im_rect_height/setting_im_item_height) == index)
    {
        //start timer when move to hold rect
        if( 0 ==index)
        {      
            if (SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
            {
                hold_rect.bottom = hold_rect.top + setting_im_item_height/2;
            }
            else
            {
                hold_rect.top = hold_rect.bottom - setting_im_item_height/2;
            }            
        }
        else
        {
            if (SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
            {
                hold_rect.bottom = hold_rect.top + setting_im_item_height/2;
            }
            else
            {
                hold_rect.top = hold_rect.bottom - setting_im_item_height/2;
            }      
        }
        
        if(GUI_PointIsInRect(point,hold_rect))
        {          
            if (data_ptr->settingim_timer_id == 0)
            {
                data_ptr->settingim_last_highlight = index;
                
                data_ptr->settingim_timer_id = MMK_CreateTimerCallback(300, MMIIM_HandleSettingIMTimerMSG, (void *)data_ptr,TRUE);  /*lint !e64*/
            }
            
            return result;
        }      
        else
        {
            if (data_ptr->settingim_timer_id != 0)
            {
                MMK_StopTimer(data_ptr->settingim_timer_id);
                
                data_ptr->settingim_timer_id = 0 ;
            } 
        }
    }
    
    if (data_ptr->settingim_timer_id != 0)
    {
        MMK_StopTimer(data_ptr->settingim_timer_id);
        
        data_ptr->settingim_timer_id = 0 ;
    } 

    //细节改进，显示方式发生改变
    if (! GUI_PointIsInRect(point, list_rect))  //菜单外面move时
    {
        index = GetMethodSelectMenuDefaultIndex ();
    }
    
    //highlight item already draw
    if(index == data_ptr->settingim_last_highlight)
    {
        result = MMI_RESULT_TRUE;
        
        return result ;
    }
    else 
    {
        data_ptr->settingim_last_highlight = index;
    }
    
    //find set key
    for (i = 0; i < data_ptr->vkb.key_cnt; i++)
    {       
        if (VKEY_SET == data_ptr->vkb.keys[i].code)
        {
            break;
        }
    }	
    
    if (VKEY_SET != data_ptr->vkb.keys[i].code)
    {
        return result;
    }
    
    MMIIM_DrawSettingIm(data_ptr,&data_ptr->vkb.keys[i],TRUE,index); 
        
    result = MMI_RESULT_TRUE;
    
    return result;
}

/*****************************************************************************/
//	Description: istyle handle set key up
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:处理输入法按键抬起
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_HandleSettingIMUp(
                                            IM_DATA_T *data_ptr,
                                            GUIIM_EVENT_DATA_U const *event_data_ptr
                                            )
{
    int16           index = 0;//indicate which item was slided
    GUI_POINT_T     point = {0};
    GUI_RECT_T      list_rect = {0};//whole rect of setting im
    GUI_RECT_T      highlight_item_rect = {0}; //item rect to highlight display
    int16           setting_im_rect_height = SETTING_IM_RECT_HEIGHT;
    int16           setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16           setting_im_item_height = SETTING_IM_ITEM_HEIGHT;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
    CHECK_DATA_EX(data_ptr);

    if (FALSE == data_ptr->is_setting_display)
    {
        return result;
    }
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_height = SETTING_IM_RECT_HEIGHT_H;
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        setting_im_item_height = SETTING_IM_ITEM_HEIGHT_H;
    }
    
    UILAYER_GetLayerPosition(&data_ptr->setting_im_layer,&list_rect.left,&list_rect.top);
    
    list_rect.right = list_rect.left + setting_im_rect_width;
    list_rect.bottom = list_rect.top + (( data_ptr->settingim_item_count*setting_im_item_height< setting_im_rect_height)?data_ptr->settingim_item_count*setting_im_item_height:setting_im_rect_height) ;
    
    point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
    point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
    
    if ( GUI_PointIsInRect(point, list_rect))
    {
        //expand half item range
        if( SETTINGIM_DOWN_HALF == data_ptr->settingim_halfposition)
        {
            list_rect.bottom += setting_im_item_height/2;
        }
        else
        {    
            list_rect.top -= setting_im_item_height/2;
        }
        
        index = (point.y - list_rect.top)/setting_im_item_height;
        
        highlight_item_rect = list_rect;
        highlight_item_rect.top += index*setting_im_item_height;
        highlight_item_rect.bottom= highlight_item_rect.top + setting_im_item_height;
        
        if (!GUI_PointIsInRect(point,highlight_item_rect))
        {
            //SCI_TRACE_LOW:"mmiim_touch_set.c: MMIIM_HandleSettingIMUp point is not in the expected rect"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_SET_812_112_2_18_2_28_16_198,(uint8*)"");

            return result;
        }
                
        index += data_ptr->settingim_top_item;
        
        if ( data_ptr->settingim_menu_index[index] < data_ptr->setting_menu_size - 1)   /*lint !e574 !e737  */
        {
            //SettingMethod will destroy im data,RECODE im index before SettingMethod
            int16 cur_im_index = data_ptr->settingim_menu_index[index];
            
            MMIIM_DestroySettingIMData(data_ptr);

            SettingMethod(data_ptr,
                data_ptr->setting_menu_data[cur_im_index].lang,
                data_ptr->setting_menu_data[cur_im_index].type,
                data_ptr->setting_menu_data[cur_im_index].input);

            return result;
        }
        else if (data_ptr->settingim_menu_index[index] == data_ptr->setting_menu_size - 1)  /*lint !e574 !e737  */
        {
            MMI_HANDLE_T win_handle = 0;
            
            win_handle = MMIAPIIM_OpenSetWin();
            
            //如下情况不再处理消息
            //1 数据校验信息被破坏
            //2 控件已经被销毁
            if (sizeof(IM_DATA_T) != data_ptr->check_info) return MMI_RESULT_TRUE;
            if (PNULL == MMK_GetCtrlPtr(data_ptr->init_param.im_handle)) return MMI_RESULT_TRUE;
            
            //enter more setting...
            data_ptr->more_setting_win_handle = win_handle;
            
            if (win_handle != 0)
            {
                GUIIM_AppendWin(data_ptr->init_param.im_handle, win_handle);
            }
        }  
    }
    else
    {
        MMIIM_ReleaseSettingIMLayer(data_ptr);
    }    
    
    //reset params
    data_ptr->settingim_last_highlight = 0;
    data_ptr->settingim_top_item = 0;
    data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
    data_ptr->settingim_item_count = 0;
    
    MMIIM_DestroySettingIMData(data_ptr);
    
    result = MMI_RESULT_TRUE;
    
    return result;
}

/*****************************************************************************/
//	Description: draw set im in istyle
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:if item height is 36(0-35),divide to(0-0)(1-34)(35-35) to display up border\item\down border
/*****************************************************************************/
PUBLIC void MMIIM_DrawSettingIm(
                                IM_DATA_T *    data_ptr,
                                KEY_INFO_T *   key_ptr,
                                BOOLEAN        is_highlight,  //weather heighlight current item
                                int16          highlight_index //index of item will highlight
                                )
{
    int16 item_index = 0;
    GUI_RECT_T list_rect={0};
    GUI_RECT_T item_rect={0};
    GUI_RECT_T str_rect = {0};
    GUI_RECT_T highlight_rect = {0};
    GUISTR_STYLE_T str_style = {0};
    int16 setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16 setting_im_rect_height = SETTING_IM_RECT_HEIGHT;
    int16 setting_im_item_height = SETTING_IM_ITEM_HEIGHT;
    
    CHECK_DATA_EX(data_ptr);

    if (PNULL == key_ptr || VKEY_SET != key_ptr->code)
    {
        return;
    }
    
    if (!UILAYER_IsLayerActive(&data_ptr->setting_im_layer) || FALSE == data_ptr->is_setting_display)
    {
        return;
    }  
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        setting_im_rect_height = SETTING_IM_RECT_HEIGHT_H;
        setting_im_item_height = SETTING_IM_ITEM_HEIGHT_H;
    }
    
    list_rect.left = 0;
    list_rect.right = setting_im_rect_width-1;
    list_rect.top = 0; 
    list_rect.bottom = ((data_ptr->settingim_item_count * setting_im_item_height< setting_im_rect_height)?
        (data_ptr->settingim_item_count * setting_im_item_height):setting_im_rect_height)-1;
    
    UILAYER_SetLayerPosition(&data_ptr->setting_im_layer,0,0);
    
    UILAYER_ClearRect(&data_ptr->setting_im_layer,list_rect);
    
    str_style.font = SETTING_IM_FONT;
    str_style.font_color = MMI_WHITE_COLOR;
    str_style.align = ALIGN_LVMIDDLE; 
    
    item_rect = list_rect;
    item_rect.bottom = item_rect.top + setting_im_item_height-1;
    
    //move up half item
    if (SETTINGIM_UP_HALF== data_ptr->settingim_halfposition)
    {
        item_rect.top -= setting_im_item_height/2;
        item_rect.bottom -= setting_im_item_height/2;
    }
    
    for (item_index = data_ptr->settingim_top_item; item_index < data_ptr->settingim_item_count; item_index++)
    {
        MMI_STRING_T disp_str = {0}; 
        GUI_RECT_T border_rect = {0};
        
        //display up border
        border_rect = item_rect;
        border_rect.bottom = border_rect.top;
        
        LCD_FillRectU32(&data_ptr->setting_im_layer,border_rect,SETTINGIM_DARK_COLOR);
        
        //display item
        item_rect.top +=1;
        item_rect.bottom -=1;
        
        if( (TRUE == is_highlight) && (item_index == highlight_index + data_ptr->settingim_top_item))
        {
            //display highlight image
            highlight_rect = item_rect;
            
            if (0 == highlight_index)
            {
                highlight_rect.top -= 1; //not not display border, display heighligth img
            }
            
            GUIRES_DisplayImg(PNULL,&highlight_rect,&highlight_rect,data_ptr->init_param.win_handle,IMAGE_COMMON_SELECTED_BAR,&data_ptr->setting_im_layer);
            str_style.font_color = MMI_BLACK_COLOR;
        }
        else
        {
            LCD_FillRectU32(&data_ptr->setting_im_layer,item_rect,SETTINGIM_ITEM_COLOR);
        }
        
        //display text
        str_rect = item_rect;
        str_rect.left += 18;
        
        MMITHEME_GetResText(data_ptr->setting_menu_data[data_ptr->settingim_menu_index[item_index]].text_id,
            data_ptr->init_param.win_handle, &disp_str);
        
        GUISTR_DrawTextToLCDInRect(&data_ptr->setting_im_layer,&str_rect,&str_rect,&disp_str,&str_style,GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS,GUISTR_TEXT_DIR_RTL);
        str_style.font_color = MMI_WHITE_COLOR; //reset font color
        
        item_rect.top -= 1;
        item_rect.bottom += 1;
        
        //display bottom border
        border_rect = item_rect;
        border_rect.top = border_rect.bottom;
        
        LCD_FillRectU32(&data_ptr->setting_im_layer,border_rect,SETTINGIM_BRIGHT_COLOR);
        
        //change item_rect to next item
        item_rect.top += setting_im_item_height;
        item_rect.bottom += setting_im_item_height;
        
        if (item_rect.top >= list_rect.bottom)
        {
            break;
        }
    }
    
    UILAYER_SetLayerPosition(&data_ptr->setting_im_layer,key_ptr->rect.right - setting_im_rect_width, key_ptr->rect.top - ( ( data_ptr->settingim_item_count * setting_im_item_height< setting_im_rect_height)? (data_ptr->settingim_item_count * setting_im_item_height):setting_im_rect_height) + data_ptr->rect.top -4);    
    
    //SCI_TRACE_LOW:"mmiim_touch_set.c: DrawSettingIm settingim_halfposition %d,highlight_index %d,data_ptr->settingim_top_item %d,data_ptr->settingim_last_highlight %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_SET_1000_112_2_18_2_28_16_199,(uint8*)"dddd",data_ptr->settingim_halfposition,highlight_index,data_ptr->settingim_top_item,data_ptr->settingim_last_highlight);
}


#endif  //#if defined (MMI_IM_PDA_SUPPORT)

