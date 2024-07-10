/*****************************************************************************
** File Name:      mmitheme_update.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_UPDATE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guiref_scale.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "mmitheme_update.h"
#include "mmitheme_special_func.h"
#include "mmi_default.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "ui_layer.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_MIN_V_SPACE    40

#define MMITHEME_CREATE_MUTEX(mutex)  do{(mutex) = SCI_CreateMutex("MMI_UPDATE_MUTEX", SCI_INHERIT);}while(0)
#define MMITHEME_GET_MUTEX(mutex)     SCI_GetMutex((mutex), SCI_WAIT_FOREVER)
#define MMITHEME_PUT_MUTEX(mutex)     SCI_PutMutex((mutex))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL SCI_MUTEX_PTR             s_update_mutex_ptr = PNULL; //互斥信号量
LOCAL uint8                     s_update_delay_count = 0;
LOCAL MMITHEME_UPDATE_RECT_T    s_main_lcd_update = {0};
LOCAL MMITHEME_UPDATE_RECT_T    s_sub_lcd_update = {0};
LOCAL uint32                    s_main_lcd_block = 0;
LOCAL uint32                    s_sub_lcd_block = 0;
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
// 	Description : init update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitUpdateRect(void);

/*****************************************************************************/
// 	Description : free update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void FreeUpdateRect(
                          MMITHEME_RECT_T   *first_rect_ptr
                          );

/*****************************************************************************/
// 	Description : init update block
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitUpdateBlock(void);

/*****************************************************************************/
// 	Description : store the update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreUpdateRect(
                           BOOLEAN                  is_sub_lcd, //in:
                           GUI_RECT_T               update_rect,//in:
                           MMITHEME_UPDATE_RECT_T   *update_ptr //in/out:
                           );

/*****************************************************************************/
// 	Description : rect is empty
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEmptyRect(
                          GUI_RECT_T    rect
                          );

/*****************************************************************************/
// 	Description : is add update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAddUpdateRect(
                              GUI_RECT_T               *update_rect_ptr,//in/out:
                              MMITHEME_UPDATE_RECT_T   *update_ptr      //in/out:
                              );

/*****************************************************************************/
// 	Description : rect is conjoint and get conjoint rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetConjointRect(
                              GUI_RECT_T    *dest_rect_ptr, //in/out:
                              GUI_RECT_T    rect1,          //in:
                              GUI_RECT_T    rect2           //in:
                              );

/*****************************************************************************/
// 	Description : remove one update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void RemoveUpdateRect(
                            MMITHEME_RECT_T         *rect_ptr,  //in:
                            MMITHEME_UPDATE_RECT_T  *update_ptr //in/out:
                            );

/*****************************************************************************/
// 	Description : add new update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AddNewUpdateRect(
                            GUI_RECT_T              update_rect,//in:
                            MMITHEME_UPDATE_RECT_T  *update_ptr //in/out:
                            );

/*****************************************************************************/
// 	Description : update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void UpdateRect(
                      BOOLEAN                   is_sub_lcd,             //in:
                      BOOLEAN                   is_update_multilayer,   //in:
                      uint32                    update_block_set,       //in:
                      MMITHEME_UPDATE_RECT_T    *update_ptr             //in:
                      );

/*****************************************************************************/
// 	Description : store the update block set
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreUpdateBlock(
                            GUI_LCD_ID_E    lcd_id,
                            GUIBLOCK_ID_E   block_id
                            );

#ifdef ENG_SUPPORT  
/*****************************************************************************/
// 	Description : set debug string
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetDebugString( void );
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init update
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitUpdate(void)
{
    MMITHEME_CREATE_MUTEX(s_update_mutex_ptr);

    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    s_update_delay_count = 0;

    //init update rect
    InitUpdateRect();

    //init update block
    InitUpdateBlock();

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);
}

/*****************************************************************************/
// 	Description : init update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitUpdateRect(void)
{
    //init main lcd
    FreeUpdateRect(s_main_lcd_update.first_rect_ptr);
    s_main_lcd_update.first_rect_ptr = PNULL;
    s_main_lcd_update.num = 0;

    //init sub lcd
    FreeUpdateRect(s_sub_lcd_update.first_rect_ptr);
    s_sub_lcd_update.first_rect_ptr = PNULL;
    s_sub_lcd_update.num = 0;
}

/*****************************************************************************/
// 	Description : free update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void FreeUpdateRect(
                          MMITHEME_RECT_T   *first_rect_ptr
                          )
{
    MMITHEME_RECT_T     *rect_ptr = PNULL;
    MMITHEME_RECT_T     *next_rect_ptr = PNULL;

    rect_ptr = first_rect_ptr;
    while (PNULL != rect_ptr)
    {
        next_rect_ptr = rect_ptr->next_ptr;

        //free
        SCI_FREE(rect_ptr);

        rect_ptr = next_rect_ptr;
    }
}

/*****************************************************************************/
// 	Description : init update block
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitUpdateBlock(void)
{
    s_main_lcd_block = 0;
    s_sub_lcd_block  = 0;
}

/*****************************************************************************/
// 	Description : init main lcd store the update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitMainLcdStortUpdateRect(void)
{
    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    //init main lcd
    FreeUpdateRect(s_main_lcd_update.first_rect_ptr);
    s_main_lcd_update.first_rect_ptr = PNULL;
    s_main_lcd_update.num = 0;

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);
}

#ifdef ENG_SUPPORT  
/*****************************************************************************/
// 	Description : set screen tag
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetDebugString( void )
{
    if ( MMIAPIENG_GetIsScreenLogOn()
        && s_main_lcd_update.num > 0 )
    {
        LOCAL uint32 s_ticks = 0;
        uint32 ticks = 0;
        uint32 temp = 0;
        GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
#ifdef UI_MULTILAYER_SUPPORT			
        uint32 layer_num = UILAYER_GetBltNum(GUI_MAIN_LCD_ID, &full_rect);
#endif
        
        ticks = SCI_GetTickCount();
        temp = ticks - s_ticks;
        s_ticks = ticks;
        
        if ( temp > 0)
        {
            temp = 1000/temp;
        }
        
        MMIAPIENG_SetScreenTag( "FPS(fps)", MMIENG_SCREEN_LOG_LCD, temp );       
#ifdef UI_MULTILAYER_SUPPORT			
        MMIAPIENG_SetScreenTag( "Layer_num", MMIENG_SCREEN_LOG_LAYER_NUM, layer_num );
#endif
        MMIAPIENG_SetScreenTag( "Avalid_mem(KB)", MMIENG_SCREEN_LOG_AVALID_MEM, SCI_GetSystemSpaceTotalAvalidMem()/1024 );
    }
}

#endif

/*****************************************************************************/
// 	Description : store the update rect, include the main and sub lcd
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_StoreUpdateRect(
                                        GUI_LCD_DEV_INFO const  *lcd_dev_ptr,   //in:
                                        GUI_RECT_T              update_rect     //in:
                                        )
{
    BOOLEAN     result = FALSE;

    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    //rect is valid
    if ((update_rect.left > update_rect.right) ||
        (update_rect.top > update_rect.bottom) )
    {
        //SCI_TRACE_LOW:"MMITHEME_StoreUpdateRect:update rect is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_UPDATE_260_112_2_18_3_3_26_52,(uint8*)"");
    }
    else
    {
        if ( PNULL != lcd_dev_ptr
#ifdef ENG_SUPPORT 
            && !MMIAPIENG_IsDebugLayer( lcd_dev_ptr )
#endif
            ) 
        {
            result = TRUE;
            
            switch (lcd_dev_ptr->lcd_id)
            {
            case GUI_MAIN_LCD_ID:
            case GUI_TV_ID:
#ifdef UI_MULTILAYER_SUPPORT                
                //            if (UILAYER_IsMultiLayerHandle((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                {
                    if (UILAYER_IsBltLayer((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                    {
                        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)lcd_dev_ptr, &update_rect, &update_rect);
                        
                        //store update rect
                        StoreUpdateRect(FALSE,update_rect,&s_main_lcd_update);
                    }
                }
                else if (UILAYER_HANDLE_INVALID != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
#endif
                {
                    //store update rect
                    StoreUpdateRect(FALSE,update_rect,&s_main_lcd_update);
                }
                break;
                
            case GUI_SUB_LCD_ID:
                StoreUpdateRect(TRUE,update_rect,&s_sub_lcd_update);
                break;
                
            default:
                result = FALSE;
                //SCI_TRACE_LOW:"MMITHEME_StoreUpdateRect: the lcd_id = %d is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_UPDATE_293_112_2_18_3_3_27_53,(uint8*)"d",lcd_dev_ptr->lcd_id);
                break;
            }
            
            //        if (!UILAYER_IsMultiLayerHandle((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
#ifdef UI_MULTILAYER_SUPPORT             
            if ((UILAYER_HANDLE_MULTI != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                && (UILAYER_HANDLE_INVALID != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)lcd_dev_ptr)))
#endif                
            {
                StoreUpdateBlock(lcd_dev_ptr->lcd_id,lcd_dev_ptr->block_id);
            }
        }
    }

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);

    return (result);
}

/*****************************************************************************/
// 	Description : store the update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreUpdateRect(
                           BOOLEAN                  is_sub_lcd, //in:
                           GUI_RECT_T               update_rect,//in:
                           MMITHEME_UPDATE_RECT_T   *update_ptr //in/out:
                           )
{
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    LCD_ID_E    lcd_id = GUI_MAIN_LCD_ID;
    
    //is sub lcd
    if (is_sub_lcd)
    {
        lcd_id = GUI_SUB_LCD_ID;
    }
    else
    {
#ifdef ENG_SUPPORT 
        MMIAPIENG_DisplayDebugRect( update_rect );
#endif
    }

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(lcd_id,&lcd_width,&lcd_height);

    //adjust update rect
    update_rect.right  = (int16)MIN(update_rect.right,(int16)(lcd_width - 1));
    update_rect.bottom = (int16)MIN(update_rect.bottom,(int16)(lcd_height - 1));

    //is add update rect
    if ((!IsEmptyRect(update_rect)) && 
        (IsAddUpdateRect(&update_rect,update_ptr)))
    {
        //add new rect
        AddNewUpdateRect(update_rect,update_ptr);
    }
}

/*****************************************************************************/
// 	Description : rect is empty
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEmptyRect(
                          GUI_RECT_T    rect
                          )
{
    BOOLEAN     result = FALSE;

    if ((0 == rect.left) &&
        (0 == rect.top) &&
        (0 == rect.right) &&
        (0 == rect.bottom))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : is add update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAddUpdateRect(
                              GUI_RECT_T               *update_rect_ptr,//in/out:
                              MMITHEME_UPDATE_RECT_T   *update_ptr      //in/out:
                              )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_remove = FALSE;
    uint16              i = 0;
    MMITHEME_RECT_T     *rect_ptr = PNULL;

    if (0 < update_ptr->num)
    {
        //set rect
        rect_ptr = update_ptr->first_rect_ptr;

        result = TRUE;
        
        for (i=0; i<update_ptr->num; i++)
        {
            //rect is cover
            if (GUI_RectIsCovered(rect_ptr->rect,*update_rect_ptr))
            {
                result = FALSE;
                break;
            }
            else if (GetConjointRect(update_rect_ptr,rect_ptr->rect,*update_rect_ptr))
            {
                //rect is conjoint
                is_remove = TRUE;
                break;
            }

            //next update
            rect_ptr = rect_ptr->next_ptr;
        }

        //remove rect
        if (is_remove)
        {
            //remove rect
            RemoveUpdateRect(rect_ptr,update_ptr);

            //check conjoint rect
            result = IsAddUpdateRect(update_rect_ptr,update_ptr);
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : rect is conjoint and get conjoint rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetConjointRect(
                              GUI_RECT_T    *dest_rect_ptr, //in/out:
                              GUI_RECT_T    rect1,          //in:
                              GUI_RECT_T    rect2           //in:
                              )
{
    BOOLEAN     result = FALSE;
    uint16      v_space = 0;
    uint16      v_space_temp = 0;
    GUI_RECT_T  cross_rect = {0};

    if (GUI_IntersectRect(&cross_rect,rect1,rect2))
    {
        result = TRUE;
    }
    else
    {
        //get min vertical space
        v_space      = (uint16)abs(rect1.top - rect2.top);
        v_space_temp = (uint16)abs(rect1.top - rect2.bottom);
        v_space      = (uint16)MIN(v_space,v_space_temp);
        v_space_temp = (uint16)abs(rect1.bottom - rect2.top);
        v_space      = (uint16)MIN(v_space,v_space_temp);
        v_space_temp = (uint16)abs(rect1.bottom - rect2.bottom);
        v_space      = (uint16)MIN(v_space,v_space_temp);

        if (MMITHEME_MIN_V_SPACE > v_space)
        {
            result = TRUE;
        }
    }

    if (result)
    {
        //get conjoint rect
        dest_rect_ptr->left   = (int16)(MIN(rect1.left,rect2.left));
        dest_rect_ptr->top    = (int16)(MIN(rect1.top,rect2.top));
        dest_rect_ptr->right  = (int16)(MAX(rect1.right,rect2.right));
        dest_rect_ptr->bottom = (int16)(MAX(rect1.bottom,rect2.bottom));
    }

    return (result);
}

/*****************************************************************************/
// 	Description : remove one update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void RemoveUpdateRect(
                            MMITHEME_RECT_T         *rect_ptr,  //in:
                            MMITHEME_UPDATE_RECT_T  *update_ptr //in/out:
                            )
{
    //is first rect
    if (rect_ptr == update_ptr->first_rect_ptr)
    {
        if (PNULL == rect_ptr->next_ptr)
        {
            //only one rect
            update_ptr->first_rect_ptr = PNULL;
        }
        else
        {
            rect_ptr->next_ptr->prev_ptr = rect_ptr->prev_ptr;

            //set first rect
            update_ptr->first_rect_ptr = rect_ptr->next_ptr;
        }
    }
    else
    {
        //last rect
        if (PNULL == rect_ptr->next_ptr)
        {
            rect_ptr->prev_ptr->next_ptr = PNULL;

            update_ptr->first_rect_ptr->prev_ptr = rect_ptr->prev_ptr;
        }
        else
        {
            rect_ptr->prev_ptr->next_ptr = rect_ptr->next_ptr;
            rect_ptr->next_ptr->prev_ptr = rect_ptr->prev_ptr;
        }
    }

    //free memory
    SCI_FREE(rect_ptr);
    
    //set rect number
    update_ptr->num--;
}

/*****************************************************************************/
// 	Description : add new update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AddNewUpdateRect(
                            GUI_RECT_T              update_rect,//in:
                            MMITHEME_UPDATE_RECT_T  *update_ptr //in/out:
                            )
{
    MMITHEME_RECT_T     *new_rect_ptr = PNULL;

    //alloc rect memory
    new_rect_ptr = SCI_ALLOC_APP(sizeof(MMITHEME_RECT_T));

    //set rect
    new_rect_ptr->rect = update_rect;

    //add to rect list,将末尾指针存放在头指针的prev
    if (PNULL == update_ptr->first_rect_ptr)
    {
        //set new child previous and next
        new_rect_ptr->prev_ptr = new_rect_ptr;
        new_rect_ptr->next_ptr = PNULL;
    
        //set first rect
        update_ptr->first_rect_ptr = new_rect_ptr;
    }
    else
    {
        //set new rect previous and next
        new_rect_ptr->prev_ptr = update_ptr->first_rect_ptr->prev_ptr;
        new_rect_ptr->next_ptr = PNULL;

        //set previous rect next
        update_ptr->first_rect_ptr->prev_ptr->next_ptr = new_rect_ptr;

        //set first child previous
        update_ptr->first_rect_ptr->prev_ptr = new_rect_ptr;
    }

    //set rect number
    update_ptr->num++;
}
/*****************************************************************************/
// 	Description : update the rect, include the main and sub lcd
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRect(void)
{
    BOOLEAN     result = FALSE;
    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    if (0 < s_update_delay_count)
    {
        //SCI_TRACE_LOW:"MMITHEME_UpdateRect: the s_update_delay_count = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_UPDATE_576_112_2_18_3_3_27_54,(uint8*)"d",s_update_delay_count);
        s_update_delay_count--;
    }
    else
    {
        //check backlight
        if (MMIDEFAULT_IsBacklightOn())
        {
#ifdef ENG_SUPPORT
            SetDebugString();
#endif
            //update main lcd rect
            UpdateRect(FALSE,FALSE,s_main_lcd_block,&s_main_lcd_update);

#ifdef SUBLCD_SIZE
            //update sub lcd
            UpdateRect(TRUE,FALSE,s_sub_lcd_block,&s_sub_lcd_update);
#endif

            //init update rect
            InitUpdateRect();

            //init update block
            InitUpdateBlock();

            result = TRUE;
        }
        else
        {
            result = TRUE;
        }
    }

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);

    return (result);
}

/*****************************************************************************/
// 	Description : update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void UpdateRect(
                      BOOLEAN                   is_sub_lcd,             //in:
                      BOOLEAN                   is_update_multilayer,   //in:
                      uint32                    update_block_set,       //in:
                      MMITHEME_UPDATE_RECT_T    *update_ptr             //in:
                      )
{
    uint16              i = 0;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    LCD_ID_E            lcd_id = GUI_MAIN_LCD_ID;
    GUI_RECT_T          lcd_rect = {0};
    GUI_RECT_T          update_rect = {0};
    MMITHEME_RECT_T     *rect_ptr = PNULL;
    
    if (0 < update_ptr->num)
    {
        //is sub lcd
        if (is_sub_lcd)
        {
            lcd_id = GUI_SUB_LCD_ID;
        }
        else
        {
#ifdef ENG_SUPPORT
            if ( MMIENG_SCREEN_DEBUG_RECT == MMIAPIENG_GetIsScreenLogOn() )
            {
                GUILCD_Invalidate( lcd_id, update_block_set );                
                MMIAPIENG_ClearDebugRect();
                return;
            }
#endif            
        }
        //get lcd width and height
        GUILCD_GetLogicWidthHeight(lcd_id,&lcd_width,&lcd_height);

        //set lcd rect
        lcd_rect.right  = (int16)(lcd_width - 1);
        lcd_rect.bottom = (int16)(lcd_height - 1);

        //set rect
        rect_ptr = update_ptr->first_rect_ptr;

        //update main lcd
        for (i=0; i<update_ptr->num; i++)
        {
            SCI_ASSERT(PNULL != rect_ptr);  /*assert verified*/

            if (GUI_IntersectRect(&update_rect,rect_ptr->rect,lcd_rect))
            {
                if (is_sub_lcd)
                {
                    GUILCD_InvalidateRect(lcd_id,
                        update_rect,
                        update_block_set);
                }
                else
                {
#ifdef UI_MULTILAYER_SUPPORT 
                    if (is_update_multilayer)
                    {
                        UILAYER_InvalidateRect(lcd_id,&update_rect);
                    }
                    else
#endif
                    {
                        if (MMITHEME_IsTVOpen())
                        {
                            GUILCD_InvalidateRect(lcd_id,
                                update_rect,
                                update_block_set);
                        }
                        else
                        {
                            if (!MMITHEME_UpdateRectBySpecialEffect(&update_rect))
                            {
                                GUILCD_InvalidateRect(lcd_id,
                                    update_rect,
                                    update_block_set);
                            }
                        }
                    }
                }
            }

            //next update
            rect_ptr = rect_ptr->next_ptr;
        }
    }
}

/*****************************************************************************/
// 	Description : store the update block set
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreUpdateBlock(
                            GUI_LCD_ID_E    lcd_id,
                            GUIBLOCK_ID_E   block_id
                            )
{
    uint32  update_block_set = 0;

    update_block_set = GUIREF_GetUpdateBlockSet(block_id);

    switch (lcd_id)
    {
    case GUI_MAIN_LCD_ID:
    case GUI_TV_ID:
        s_main_lcd_block = s_main_lcd_block | update_block_set;
        break;

    case GUI_SUB_LCD_ID:
        s_sub_lcd_block = s_sub_lcd_block | update_block_set;
        break;

    default:
        //SCI_TRACE_LOW:"StoreUpdateBlock:lcd_id %d, block_id %dis error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_UPDATE_742_112_2_18_3_3_27_55,(uint8*)"di",lcd_id, block_id);
        break;
    }
}

/*****************************************************************************/
// 	Description : set update delay count
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetUpdateDelayCount(
                                         uint8  delay_count
                                         )
{
    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    s_update_delay_count = delay_count;

    //SCI_TRACE_LOW:"MMITHEME_SetUpdateDelayCount: the s_update_delay_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_UPDATE_759_112_2_18_3_3_27_56,(uint8*)"d",s_update_delay_count);

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);
}

/*****************************************************************************/
// 	Description : update multilayer to lcd buffer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_UpdateMultiLayer(void)
{
    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    //update main lcd multilayer
    UpdateRect(FALSE,TRUE,s_main_lcd_block,&s_main_lcd_update);

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);
}

/*****************************************************************************/
// 	Description :is update full screen
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUpdateFullScreen(void)
{
    BOOLEAN     result = FALSE;

    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    if (1 == s_main_lcd_update.num)
    {
        result = MMITHEME_IsFullScreenRect(s_main_lcd_update.first_rect_ptr->rect);
    }
    
    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);

    return (result);
}

/*****************************************************************************/
// 	Description :is update rect dirty
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUpdateRectDirty(void)
{
    BOOLEAN     result = FALSE;

    MMITHEME_GET_MUTEX(s_update_mutex_ptr);

    if (s_main_lcd_update.num > 0)
    {
        result = TRUE;
    }

    MMITHEME_PUT_MUTEX(s_update_mutex_ptr);

    return (result);
}

/*****************************************************************************/
// 	Description : update the rect, include the main and sub lcd ,ex for the livewallpaper
//	Global resource dependence : 
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRectEx(void)
{
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    /*if current wallpaper is livewallpaper ,play the frame*/
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        MMIAPILIVEWALLPAPER_PlayFrame(MMK_GetFocusWinId());
    }
#endif

    return MMITHEME_UpdateRect();
}


