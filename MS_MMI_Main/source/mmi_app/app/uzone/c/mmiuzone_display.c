/*****************************************************************************
** File Name:      mmiuzone_display.c                                                *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica              Creat
******************************************************************************/
#include "mmi_app_uzone_trc.h"
#ifdef MMIUZONE_SUPPORT

#define _MMIUZONE_DISPLAY_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "guires.h"
#include "mmidisplay_data.h"
#include "mmi_modu_main.h"


#include "mmiuzone.h"
#include "mmiuzone_internal.h"
#include "mmiuzone_display.h"
#include "mmiuzone_se.h"
#include "mmiuzone_position.h"
#include "mmiuzone_image.h"

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
//  Description : fill bg layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillBgLayer(void);


/*****************************************************************************/
//  Description : fill big star layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillBigStarLayer(void);

/*****************************************************************************/
//  Description : fill small star layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillSmallStarLayer(void);


/*****************************************************************************/
//  Description : fill each item
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillItem(GUI_RECT_T* icon_bg_rect, 
                                            GUI_RECT_T* icon_rect,
                                            GUI_RECT_T* str_bg_rect,
                                            GUI_RECT_T* str_rect,
                                            MMI_WIN_ID_T win_id,
                                            MMIUZONE_ITEM_T* item_ptr,
                                            GUI_LCD_DEV_INFO* layer_ptr);


/*****************************************************************************/
//  Description : fill src layers for horizontal
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillHSrcLayer(void);


/*****************************************************************************/
//  Description : fill src layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillSrcLayer(void);


/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : fill bg layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillBgLayer(void)
{
    GUI_RECT_T bg_rect = {0};

    BOOLEAN res = TRUE;
    GUI_LCD_DEV_INFO* layer_ptr = PNULL;
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillHSrcLayer - running info is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_108_112_2_18_3_4_21_24,(uint8*)"");
        return FALSE;
    }    

    layer_ptr =  &running_info_ptr->bglayer_dev_info;

    if(!UILAYER_IsLayerActive(layer_ptr))
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillBgLayer - bg layer is not active!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_116_112_2_18_3_4_21_25,(uint8*)"");
        return FALSE;
    }
    
    if(running_info_ptr->is_landscape)
    {
        bg_rect =  running_info_ptr->bg_info.bg_both_rect.h_rect;
    }
    else
    {
        bg_rect =  running_info_ptr->bg_info.bg_both_rect.v_rect;
    }

    res = GUIRES_DisplayImg(PNULL, &bg_rect, PNULL,
                                running_info_ptr->main_win_id,
                                running_info_ptr->bg_info.bg_id, layer_ptr);

    if(!res)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillBgLayer - draw bg fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_142_112_2_18_3_4_21_26,(uint8*)"");
        return res;
    }

    
    return res;    
}


/*****************************************************************************/
//  Description : fill big star layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillBigStarLayer(void)
{
    uint16 i = 0;
    BOOLEAN res = TRUE;
    GUI_RECT_T star_rect = {0};    
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();
    MMI_IMAGE_ID_T img_list[] = 
    {
        IMG_UZONE_STAR1_1,
        IMG_UZONE_STAR1_2,
        IMG_UZONE_STAR1_3,
        IMG_UZONE_STAR1_4,
        IMG_UZONE_STAR1_5
    };

    if(PNULL == running_info_ptr)
    {
        SCI_TRACE_LOW("[MMIUZONE] FillBigStarLayer - running info is PNULL!");    
        return FALSE;
    }    

    star_rect.left = 0;
    star_rect.right = star_rect.left + running_info_ptr->big_star_size.width - 1;
    
    for(i = 0; i < UZONE_STAR_NUMBER; i++)
    {
        star_rect.bottom = star_rect.top + running_info_ptr->big_star_size.height - 1;

        res = GUIRES_DisplayImg(PNULL,
                            &star_rect,
                            PNULL,
                            running_info_ptr->main_win_id,
                            img_list[i],
                            &running_info_ptr->bigstarlayer_dev_info);

         if(!res)
        {
            SCI_TRACE_LOW("[MMIUZONE] FillBigStarLayer (%d) - draw star fail!", i);
            return res;
        }
       star_rect.top = star_rect.bottom + 1;
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : fill small star layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillSmallStarLayer(void)
{
    uint16 i = 0;
    BOOLEAN res = TRUE;
    GUI_RECT_T star_rect = {0};    
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();
    MMI_IMAGE_ID_T img_list[] = 
    {
        IMG_UZONE_STAR2_1,
        IMG_UZONE_STAR2_2,
        IMG_UZONE_STAR2_3,
        IMG_UZONE_STAR2_4,
        IMG_UZONE_STAR2_5
    };

    if(PNULL == running_info_ptr)
    {
        SCI_TRACE_LOW("[MMIUZONE] FillSmallStarLayer - running info is PNULL!");    
        return FALSE;
    }    

    star_rect.left = 0;
    star_rect.right = star_rect.left + running_info_ptr->small_star_size.width - 1;
    
    for(i = 0; i < UZONE_STAR_NUMBER; i++)
    {
        star_rect.bottom = star_rect.top + running_info_ptr->small_star_size.height - 1;

        res = GUIRES_DisplayImg(PNULL,
                            &star_rect,
                            PNULL,
                            running_info_ptr->main_win_id,
                            img_list[i],
                            &running_info_ptr->smallstarlayer_dev_info);

         if(!res)
        {
            SCI_TRACE_LOW("[MMIUZONE] FillSmallStarLayer (%d) - draw star fail!", i);
            return res;
        }
       star_rect.top = star_rect.bottom + 1;
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : fill src layers for horizontal
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillFocusItem(uint16 item_index)
{
    BOOLEAN res = TRUE;
    MMIUZONE_ITEM_T* item_ptr = PNULL;
    MMIUZONE_ITEM_T item  = {0};
    GUI_RECT_T icon_bg_rect = {0};
    GUI_RECT_T icon_rect = {0};
    GUI_RECT_T str_rect = {0};
    int16 height = 0,width = 0;
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        SCI_TRACE_LOW("[MMIUZONE] FillFocusItem - running info is PNULL!");    
        return FALSE;
    }    

    if(!UILAYER_IsLayerActive(&running_info_ptr->focusitemlayer_dev_info))
    {
        SCI_TRACE_LOW("[MMIUZONE] FillFocusItem - src layer or focus item layer is not active!");
        return FALSE;
    }



    item_ptr = running_info_ptr->item_ptr + item_index;
    SCI_MEMCPY(&item,item_ptr,sizeof(MMIUZONE_ITEM_T));
    
    height = running_info_ptr->item_size.height;
    width = running_info_ptr->item_size.width;

    icon_bg_rect = item_ptr->item_info.bg_rect;
    icon_rect = item_ptr->item_info.icon_rect;
    str_rect = item_ptr->item_info.str_rect;

    item.item_info.item_res.bg_id = IMG_UZONE_GLASS_FOCUS;
    UILAYER_Clear(&running_info_ptr->focusitemlayer_dev_info);
    
    res = FillItem(&icon_bg_rect, &icon_rect, PNULL, &str_rect, running_info_ptr->main_win_id, 
                    &item, &running_info_ptr->focusitemlayer_dev_info);
    if(!res)
    {
        SCI_TRACE_LOW("[MMIUZONE] FillFocusItem: file item %d fail!", item_ptr->item_info.item_id);
        return FALSE;
    }

    return res;
}


/*****************************************************************************/
//  Description : fill each item
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillItem(GUI_RECT_T* icon_bg_rect, 
                                            GUI_RECT_T* icon_rect,
                                            GUI_RECT_T* str_bg_rect,
                                            GUI_RECT_T* str_rect,
                                            MMI_WIN_ID_T win_id,
                                            MMIUZONE_ITEM_T* item_ptr,
                                            GUI_LCD_DEV_INFO* layer_ptr)
{
    BOOLEAN res = TRUE;
    MMI_STRING_T text_str = {0};

    GUISTR_STATE_T str_state  = MMIUZONE_GetStringState();
    GUISTR_STYLE_T str_style = MMIUZONE_GetStringStyle();

    if((PNULL == icon_bg_rect) || (PNULL == icon_rect) || (PNULL == str_rect) 
        || (PNULL == item_ptr) || (PNULL == layer_ptr) || (PNULL == win_id))
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillItem - Invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_180_112_2_18_3_4_21_28,(uint8*)"");
        return FALSE;
    }

    res = GUIRES_DisplayImg(PNULL,
                        icon_bg_rect,
                        PNULL,
                        win_id,
                        item_ptr->item_info.item_res.bg_id,
                        layer_ptr);

    if(!res)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillItem(%d) - draw icon bg fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_193_112_2_18_3_4_21_29,(uint8*)"d",item_ptr->item_info.item_id);
        return res;
    }
    
    res = GUIRES_DisplayImg(PNULL,
                        icon_rect,
                        PNULL,
                        win_id,
                        item_ptr->item_info.item_res.icon_id,
                        layer_ptr);

    if(!res)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillItem(%d) - draw icon fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_206_112_2_18_3_4_21_30,(uint8*)"d",item_ptr->item_info.item_id);
        return res;
    }

    if(PNULL != str_bg_rect)
    {
        res = GUIRES_DisplayImg(PNULL,
                            str_bg_rect,
                            PNULL,
                            win_id,
                            item_ptr->item_info.item_res.bg_id,
                            layer_ptr);
    }

    if(!res)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillItem(%d) - draw string bg fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_222_112_2_18_3_4_21_31,(uint8*)"d",item_ptr->item_info.item_id);
        return res;
    }


    MMI_GetLabelTextByLang(item_ptr->item_info.item_res.text_id, &text_str);
    GUISTR_DrawTextToLCDInRect(layer_ptr,
        str_rect, str_rect, &text_str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);

    return res;
}

/*****************************************************************************/
//  Description : fill each item
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Fillicon(MMIUZONE_ITEM_T* item_ptr,
                            MMI_WIN_ID_T win_id,
                            GUI_RECT_T* rect,
                            GUI_RECT_T* img_rect,
                            GUI_LCD_DEV_INFO* layer_ptr
                            )
{
    BOOLEAN res = TRUE;
    
    
    if((PNULL == rect)
        || (PNULL == item_ptr) 
        || (PNULL == layer_ptr)
        )
    {
        return FALSE;
    }
    
    res = GUIRES_DisplayImg(PNULL,
                        rect,
                        img_rect,
                        win_id,
                        item_ptr->item_info.item_res.bg_id,
                        layer_ptr);
                        
    res = GUIRES_DisplayImg(PNULL,
                        rect,
                        PNULL,
                        win_id,
                        item_ptr->item_info.item_res.icon_id,
                        layer_ptr);
                        
    return res;
}
/*****************************************************************************/
//  Description : fill each item
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Fillstr(MMIUZONE_ITEM_T* item_ptr,
                            MMI_WIN_ID_T win_id,
                            GUI_RECT_T* rect,
                            GUI_LCD_DEV_INFO* layer_ptr
                            )
{
    MMI_STRING_T text_str = {0};
    BOOLEAN res = TRUE;
    GUISTR_STATE_T str_state  = MMIUZONE_GetStringState();
    GUISTR_STYLE_T str_style = MMIUZONE_GetStringStyle();
    
    if((PNULL == rect)
        || (PNULL == item_ptr) 
        || (PNULL == layer_ptr)
        )
    {
        return FALSE;
    }

    MMI_GetLabelTextByLang(item_ptr->item_info.item_res.text_id, &text_str);
    
    GUISTR_DrawTextToLCDInRect(layer_ptr,
        rect, rect, &text_str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);
        
    return res;
}

/*****************************************************************************/
//  Description : fill src layers for horizontal
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillHSrcLayer(void)
{
    uint16 i = 0;
    BOOLEAN res = TRUE;
    MMIUZONE_ITEM_T* item_ptr = PNULL;
    GUI_RECT_T icon_bg_rect = {0};
    GUI_RECT_T icon_rect = {0};
    GUI_RECT_T str_rect = {0};
    GUI_LCD_DEV_INFO temp_layer  = {0};
    int16 height = 0,width = 0;
    uint8* src_ptr = PNULL; 
    uint8* des_ptr = PNULL;
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();
    uint8* icon_src_ptr = PNULL; 
    uint8* str_src_ptr = PNULL;
    uint8* str_shadow_ptr = PNULL;
    GUI_RECT_T icon_dis_rect = {0};
    GUI_RECT_T str_dis_rect = {0};
    
    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillHSrcLayer - running info is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_258_112_2_18_3_4_21_32,(uint8*)"");
        return FALSE;
    }    


    height = running_info_ptr->item_size.height;
    width = running_info_ptr->item_size.width;
    temp_layer = running_info_ptr->focusitemlayer_dev_info;

    icon_bg_rect = running_info_ptr->item_ptr->item_info.bg_rect;
    icon_dis_rect = running_info_ptr->item_ptr->item_info.icon_rect;
    str_dis_rect = running_info_ptr->item_ptr->item_info.str_rect;
  
    src_ptr = UILAYER_GetLayerBufferPtr(&temp_layer);
    des_ptr = UILAYER_GetLayerBufferPtr(&running_info_ptr->page_srclayer_dev_info);
    GUIRES_DisplayImg(PNULL,
                        &icon_bg_rect,
                        PNULL,
                        running_info_ptr->main_win_id,
                        running_info_ptr->item_ptr->item_info.item_res.bg_id,
                        &temp_layer);
                        
    icon_src_ptr = UILAYER_GetLayerBufferPtr(&running_info_ptr->icon_srclayer_dev_info);
    str_src_ptr = UILAYER_GetLayerBufferPtr(&running_info_ptr->str_srclayer_dev_info);
    str_shadow_ptr = UILAYER_GetLayerBufferPtr(&running_info_ptr->str_shadow_dev_info);
    
    icon_rect.left = 0;
    icon_rect.top = 0;
    icon_rect.right = icon_rect.left + UZONE_ITEM_ICON_WIDTH;
    icon_rect.bottom = icon_rect.top + UZONE_ITEM_ICON_HEIGHT;
    
    str_rect.left = 0;
    str_rect.top = 0;
    str_rect.right = str_rect.left + UZONE_ITEM_STR_WIDTH;
    str_rect.bottom = str_rect.top + UZONE_ITEM_STR_HEIGHT;
    
    for(i=0; i<MMIUZONE_ITEM_ID_MAX; i++)
    {
        item_ptr = running_info_ptr->item_ptr + i;
        
        
        Fillicon(item_ptr,running_info_ptr->main_win_id,&icon_rect,&icon_dis_rect,&running_info_ptr->icon_srclayer_dev_info);

        icon_rect.top += UZONE_ITEM_ICON_HEIGHT;
        icon_rect.bottom = icon_rect.top + UZONE_ITEM_ICON_HEIGHT;
        
        Fillstr(item_ptr,running_info_ptr->main_win_id,&str_rect,&running_info_ptr->str_srclayer_dev_info);

        str_rect.top += UZONE_ITEM_STR_HEIGHT;
        str_rect.bottom = str_rect.top + UZONE_ITEM_STR_HEIGHT;
        
    }
    
    SEAPI_CreateShadow(src_ptr,
                        des_ptr,
                        (uint32*)icon_src_ptr,
                        &icon_dis_rect,
                        (uint32*)str_src_ptr,
                        &str_dis_rect,
                        (uint32*)str_shadow_ptr
                        ); //Jessica remove temporarily 


    return res;
}


/*****************************************************************************/
//  Description : fill src layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FillSrcLayer(void)
{
    BOOLEAN res = TRUE;
    
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillSrcLayer - running info is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_383_112_2_18_3_4_22_37,(uint8*)"");
        return FALSE;
    }    
    
    if(!UILAYER_IsLayerActive(&running_info_ptr->icon_srclayer_dev_info)
    ||!UILAYER_IsLayerActive(&running_info_ptr->str_srclayer_dev_info)
    ||!UILAYER_IsLayerActive(&running_info_ptr->page_srclayer_dev_info)
    )
    {
        //SCI_TRACE_LOW:"[MMIUZONE] FillSrcLayer - src layer is not active!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_389_112_2_18_3_4_22_38,(uint8*)"");
        return FALSE;
    }

        res = FillHSrcLayer();
        if(!res)
        {
            //SCI_TRACE_LOW:"[MMIUZONE] FillSrcLayer - fill h src layer fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_396_112_2_18_3_4_22_39,(uint8*)"");
        }

    return res;
}

/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : draw focus item callback
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawFocusItem(uint16 item_index)
{
    BOOLEAN res = TRUE;
    if(item_index>=MMIUZONE_ITEM_ID_MAX)
    {
        return;
    }
    
    res = FillFocusItem(item_index);
    if(!res)
    {
        SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_DrawFocusItem - draw fail!");
    }
}


/*****************************************************************************/
//  Description : draw bg to main layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawBgToMainLayer(void)
{
    GUI_RECT_T bg_rect = {0};
    BOOLEAN res = TRUE;
    GUI_LCD_DEV_INFO main_layer = {0};
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_DrawBgToMainLayer - running info is PNULL!");
        return;
    }    

    if(running_info_ptr->is_landscape)
    {
        bg_rect =  running_info_ptr->bg_info.bg_both_rect.h_rect;
    }
    else
    {
        bg_rect =  running_info_ptr->bg_info.bg_both_rect.v_rect;
    }

    res = GUIRES_DisplayImg(PNULL, &bg_rect, PNULL,
                                running_info_ptr->main_win_id,
                                running_info_ptr->bg_info.bg_id, &main_layer);

    if(!res)
    {
        SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_DrawBgToMainLayer - draw bg fail!");
        return;
    }
}


/*****************************************************************************/
//  Description : fill uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_FillLayers(void)
{
    if(!FillBgLayer())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_FillLayers - fill bg layer fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_417_112_2_18_3_4_22_41,(uint8*)"");
        return FALSE;
    }

    if(!FillBigStarLayer())
    {
        SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_FillLayers - fill big star layer fail!");
        return FALSE;
    }

    if(!FillSmallStarLayer())
    {
        SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_FillLayers - fill small star layer fail!");
        return FALSE;
    }


    if(!FillSrcLayer())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_FillLayers - fill src layer fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_423_112_2_18_3_4_22_42,(uint8*)"");
        return FALSE;
    }


    return TRUE;
}

/*****************************************************************************/
//  Description : draw uzone window
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawWindow(void)
{
    UILAYER_APPEND_BLT_T append_info = {0};
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_DrawWindow - running info is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_442_112_2_18_3_4_22_43,(uint8*)"");
        return;
    }
#if 0
    UILAYER_RemoveMainLayer();

    append_info.lcd_dev_info = running_info_ptr->bglayer_dev_info;
    append_info.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_info);
#endif
    append_info.lcd_dev_info = running_info_ptr->fglayer_dev_info;
    append_info.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_info);

}


/*****************************************************************************/
//  Description : exist draw
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ExistDraw(void)
{
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();
    
    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_ExistDraw - running info is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_DISPLAY_468_112_2_18_3_4_22_44,(uint8*)"");
        return;
    }
    
    //UILAYER_RemoveBltLayer(&running_info_ptr->bglayer_dev_info);
    UILAYER_RemoveBltLayer(&running_info_ptr->fglayer_dev_info);
    MMIUZONE_ReleaseLayers();
    //UILAYER_RestoreMainLayer();
}

#endif //MMIUZONE_SUPPORT


