/*****************************************************************************
** File Name:      mmiwidget_clock.c                                        *
** Author:                                                                   *
** Date:           09/20/2010                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/20/2010                       Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"    

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)

#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmicc_text.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiwidget_position.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define MMIWIDGET_ELVE_NORMAL_IMG_MAX_INDEX       8
#define MMIWIDGET_ELVE_CLICK_IMG_MAX_INDEX        13
#define MMIWIDGET_ELVE_MOVING_IMG_MAX_INDEX       32

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct
{
    MMIWIDGET_ELVE_DISPLAY_E elve_action;
    uint32                   cur_index;
}MMIWIDGET_ELVE_T;

typedef enum
{
   MMIWIDGET_ELVE_NOR_MIN_INDEX = IMAGE_WIDGET_ELVE_NOR_0,
   MMIWIDGET_ELVE_NOR_MAX_INDEX = IMAGE_WIDGET_ELVE_NOR_7
}MMIWIDGET_ELVE_NORMAL_ANIM_E;

typedef enum
{
   MMIWIDGET_ELVE_CLK_MIN_INDEX = IMAGE_WIDGET_ELVE_CLK_0,
   MMIWIDGET_ELVE_CLK_MAX_INDEX = IMAGE_WIDGET_ELVE_CLK_12
}MMIWIDGET_ELVE_CLK_ANIM_E;

typedef enum
{
   MMIWIDGET_ELVE_MOV_MIN_INDEX = IMAGE_WIDGET_ELVE_MOV_0,
   MMIWIDGET_ELVE_MOV_MAX_INDEX = IMAGE_WIDGET_ELVE_MOV_31
}MMIWIDGET_ELVE_MOV_ANIM_E;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#define MMIWIDGET_ELVE_ANIMATION_GAP    100   //5ms between two animation frame.

LOCAL MMIWIDGET_ELVE_T s_elve_context = {0};
//LOCAL MMIWIDGET_ELVE_DISPLAY_E s_elve_display_action = MMIWIDGET_ELVE_NORMAL_INVALID;

//elve animation timer
LOCAL uint8  g_widget_elve_timer_id = 0;

/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetElveWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayElve(
                       MMI_WIN_ID_T win_id
                       );

/*****************************************************************************/
//  Description : set elve display type.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetElveDisplayType(MMIWIDGET_ELVE_DISPLAY_E display_type);

/*****************************************************************************/
//  Description : set elve display type.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetElveDisplayFrame(void);
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_elve_widget =
{
    MMIWIDGET_ELVE_ID, 
    HandleWidgetElveWinMsg, 
    PNULL, 
    PNULL,
    PNULL,
    WIDGET_ELVE_WIN_ID, 
    IMAGE_WIDGET_ELVE_SHORTCUT, 
    IMAGE_WIDGET_ELVE_NOR_0, 
    TXT_WIDGET_ELVE,
    50,//    75, 
    50,//    75
    FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create elve animation timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateElveAnimTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    g_widget_elve_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
}

/*****************************************************************************/
//  Description : stop elve animation timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopElveAnimTimer(void)
{
    if (0 != g_widget_elve_timer_id)
    {
        MMK_StopTimer(g_widget_elve_timer_id);
        g_widget_elve_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : create elve animation timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartElveAnimTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    StopElveAnimTimer();
    CreateElveAnimTimer(win_id, time_out);
}

/*****************************************************************************/
//     Description : Handle Elve Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetElveWinMsg(
                                      MMI_WIN_ID_T        win_id,      //IN:
                                      MMI_MESSAGE_ID_E    msg_id,      //IN:
                                      DPARAM              param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetElveDisplayType(MMIWIDGET_ELVE_NORMAL_DISPLAY);
        break;

    case MSG_WIDGET_ELVE_SET_STATUS:
         if (MMIWIDGET_ELVE_NORMAL_DISPLAY == *(MMIWIDGET_ELVE_DISPLAY_E *)param)
         {
             SetElveDisplayType(MMIWIDGET_ELVE_NORMAL_DISPLAY);
         }
         else
         if (MMIWIDGET_ELVE_CLICK_DISPLAY == *(MMIWIDGET_ELVE_DISPLAY_E *)param)
         {
             SetElveDisplayType(MMIWIDGET_ELVE_CLICK_DISPLAY);                     
         }
         else
         if (MMIWIDGET_ELVE_MOVING_DISPLAY == *(MMIWIDGET_ELVE_DISPLAY_E *)param)
         {
             SetElveDisplayType(MMIWIDGET_ELVE_MOVING_DISPLAY);                  
         }     
        break;

    case MSG_WIDGET_ELVE_START_ANIM:
         if (*(BOOLEAN *)param)
         {
             RestartElveAnimTimer(win_id, MMIWIDGET_ELVE_ANIMATION_GAP);
         }
         else
         {
             StopElveAnimTimer();
         }
         break;

    case MSG_TIMER:
  		if(g_widget_elve_timer_id == *(uint8 *)param)
  		{
  		    RestartElveAnimTimer(win_id, MMIWIDGET_ELVE_ANIMATION_GAP);
  		    SetElveDisplayFrame();

  		    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
  		    {
  		        DisplayElve(win_id);
  		    }
  		}
        break;
        
    case MSG_FULL_PAINT:
        DisplayElve(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : set elve display type.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetElveDisplayType(MMIWIDGET_ELVE_DISPLAY_E display_type)
{
    if (s_elve_context.elve_action == display_type)
    {
        return;
    }
    
    s_elve_context.elve_action = display_type;

    if (MMIWIDGET_ELVE_NORMAL_DISPLAY == display_type)
    {
        s_elve_context.cur_index   = MMIWIDGET_ELVE_NOR_MIN_INDEX;    
    }
    else
    if (MMIWIDGET_ELVE_CLICK_DISPLAY == display_type)
    {
        s_elve_context.cur_index   = MMIWIDGET_ELVE_CLK_MIN_INDEX;   
    }
    else
    if (MMIWIDGET_ELVE_MOVING_DISPLAY == display_type)
    {
        s_elve_context.cur_index   = MMIWIDGET_ELVE_MOV_MIN_INDEX;    
    }
}

/*****************************************************************************/
//  Description : set elve display type.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetElveDisplayFrame()
{
    if (MMIWIDGET_ELVE_NORMAL_DISPLAY == s_elve_context.elve_action)
    {
        if (s_elve_context.cur_index < MMIWIDGET_ELVE_NOR_MAX_INDEX)
        {
            s_elve_context.cur_index++;            
        }
        else
        {
            s_elve_context.cur_index = MMIWIDGET_ELVE_NOR_MIN_INDEX; 
        }
    }
    else
    if (MMIWIDGET_ELVE_CLICK_DISPLAY == s_elve_context.elve_action)
    {
        if (s_elve_context.cur_index < MMIWIDGET_ELVE_CLK_MAX_INDEX)
        {
            s_elve_context.cur_index++;            
        }
        else
        {
            s_elve_context.cur_index = MMIWIDGET_ELVE_CLK_MIN_INDEX; 
        }
    }
    else
    if (MMIWIDGET_ELVE_MOVING_DISPLAY == s_elve_context.elve_action)
    {
        if (s_elve_context.cur_index < MMIWIDGET_ELVE_MOV_MAX_INDEX)
        {
            s_elve_context.cur_index++;            
        }
        else
        {
            s_elve_context.cur_index = MMIWIDGET_ELVE_MOV_MIN_INDEX; 
        }   
    }
}


/*****************************************************************************/
//  Description : display elve
//  Global resource dependence: 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayElve(
                       MMI_WIN_ID_T             win_id
                       )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T img_rect = {0};
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = PNULL;
   
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    MMK_GetWinRect(win_id, &win_rect);
    
    //fix red bug when not full screen above it.
    if (   widget_ctrl_ptr->elve_layer.block_id == lcd_dev_info.block_id
        || widget_ctrl_ptr->widget_layer.block_id == lcd_dev_info.block_id)
    {
        UILAYER_Clear(&lcd_dev_info);
        UILAYER_SetLayerPosition(&lcd_dev_info, win_rect.left, win_rect.top);
    }
   
    img_rect.left = 0;
    img_rect.top = 0;
    img_rect.right = win_rect.right+1-win_rect.left;
    img_rect.bottom = win_rect.bottom+1-win_rect.top;   
    
    GUIRES_DisplayImg(
        PNULL,
        &win_rect,
        &img_rect,
        win_id,
        s_elve_context.cur_index,//IMAGE_WIDGET_3,
        &lcd_dev_info
        ); 
}
#endif
