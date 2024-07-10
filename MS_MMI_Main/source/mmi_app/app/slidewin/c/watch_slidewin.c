/****************************************************************************
** File Name:   watch_slidewin.c                                                                                   *
** Author:                                                                                                                   *
** Date:           02/26/2020                                                                                          *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                 *
** Description:    slidewin common win                                                                           *
*****************************************************************************
**                         Important Edit History                                                                       *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                   *
** 03/26/2020  zhikun.lv        Create                                                                             *
** 05/24/2021  zhikun.lv        Update                                                                             *
******************************************************************************/

#include "watch_slidewin.h"
#include "ui_layer.h"
#include "mmk_timer.h"
#include "mmicom_trace.h"

#ifdef TOUCH_PANEL_SUPPORT
#include "mmk_tp.h"
#endif


/***********************************Macro Definition**************************************/

#ifndef TRACE_APP_LAUNCHER
#define TRACE_APP_LAUNCHER
#endif

#define TRACE_APP_LAUNCHER_ENTER TRACE_APP_LAUNCHER("enter");
#define TRACE_APP_LAUNCHER_EXIT TRACE_APP_LAUNCHER("exit");

#define SYSTEM_SLIDEWIN_MAX_NUM 10
#define SYSTEM_SLIDEWIN_INVALID_INDEX (-1)

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SLIDEWIN_CHECK_ITEM_VALID(item) \
    do \
    {   \
        if (item == NULL) \
        { \
            TRACE_APP_LAUNCHER("item == NULL");   \
            SCI_ASSERT(0); \
        } \
    } while (0);


#define SYSTEM_SLIDEWIN_ACTIVEHOOK(item) \
do \
{   \
    tSlideWinInfo* it = (tSlideWinInfo*)item; \
    MMK_SetWinHookFunc(it->win_id, SlideWin_HandleMsgHook); \
} while (0);


#define SYSTEM_SLIDEWIN_DEACTIVEHOOK(item) \
do \
{   \
    tSlideWinInfo* it = (tSlideWinInfo*)item; \
    MMK_SetWinHookFunc(it->win_id, NULL); \
} while (0);

/***********************************Macro Definition End**************************************/



/***********************************Local Struct Declear**************************************/
typedef struct tSlideWin_Node
{
    tSlideWinInfo *item;    //pointer to slidewin entity
    BOOLEAN exist;          //have one flag
}tSlideWin_Node;

typedef BOOLEAN (*func_SlideWinBase)(tSlideWinInfo* item);
typedef BOOLEAN (*func_motionDetect)(GUI_POINT_T press_pos, GUI_POINT_T move_pos);
/***********************************Local Struct Declear End**************************************/



/***********************************Global Var Definition**************************************/
// the global slide win array
LOCAL tSlideWin_Node s_slidewin_array[SYSTEM_SLIDEWIN_MAX_NUM] = {0};

//the total count slidewin entity in slidewinarray
LOCAL uint8 s_slidewin_num = 0;
/***********************************Global Var Definition End**************************************/



/***********************************Local Function Declear**************************************/
LOCAL BOOLEAN IsHorizontalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos);
LOCAL BOOLEAN IsVerticalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos);
LOCAL void SwapInitToStopPos(int16 *a, int16 *b);
LOCAL tSlideWin_Node* SlideWinArray_GetArray(void);
LOCAL uint8 SlideWinArray_GetMaxLength(void);
LOCAL BOOLEAN SlideWinArray_Add(tSlideWinInfo* item);
LOCAL BOOLEAN SlideWinArray_Remove(tSlideWinInfo* item);
LOCAL int8 SlideWinArray_FindIndex(tSlideWinInfo* item);
LOCAL BOOLEAN SlideWinArray_IsItemValid(tSlideWinInfo* item);
LOCAL BOOLEAN SlideWinArray_FindFreeIndex(tSlideWin_Node* array, uint8 max, uint8* index);

LOCAL void SlideWin_CalcStopPosAfterTpUp(tSlideWinInfo *item);
LOCAL void SlideWin_StartTpDrapTimer(tSlideWinInfo* item);
LOCAL void SlideWin_StopTpDrapTimer(tSlideWinInfo* item);
LOCAL void SlideWin_StartAutoSlideTimer(tSlideWinInfo* item);
LOCAL void SlideWin_StopAutoSlideTimer(tSlideWinInfo* item);
LOCAL BOOLEAN SlideWin_RunAutoMode(tSlideWinInfo* item);
#ifdef TOUCH_PANEL_SUPPORT
LOCAL BOOLEAN SlideWin_RunTpMode(tSlideWinInfo* item);
#endif
LOCAL void SlideWin_CreateSlideLayer(MMI_WIN_ID_T win_id,
                                                    uint16 width,
                                                    uint16 height,
                                                    GUI_LCD_DEV_INFO *lcd_dev_ptr);
LOCAL void SlideWin_AppendSlideLayerToBlt(GUI_LCD_DEV_INFO lcd_dev_ptr);

LOCAL void SlideWin_SetState(tSlideWinInfo* item, uint32 st);
LOCAL void SlideWin_ClearState(tSlideWinInfo* item, uint32 st);
LOCAL BOOLEAN SlideWin_IsInState(tSlideWinInfo* item, uint32 st);
LOCAL MMI_RESULT_E SlideWin_ProcOpenWindow(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcCloseWindow(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcGetFocus(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcLoseFocus(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcFullPaint(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_HandleTpDrapTimer(tSlideWinInfo *item);
LOCAL MMI_RESULT_E SlideWin_HandleAutoSlideTimer(tSlideWinInfo *item);
LOCAL MMI_RESULT_E SlideWin_ProcOnTimer(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMI_RESULT_E SlideWin_ProcTpPress(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcTpMove(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SlideWin_ProcTpUp(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E SlideWin_HandleMsgHook(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/***********************************Local Function Declear End**************************************/


/***********************************Local Function Definition**************************************/

/*****************************************************************************/
//  Description : judge the horizontal motion is exist
//  Parameter: [In] press_pos: the tp press positon, which coordiate in lcd
//                  [In] move_pos: the tp move positon, which coordiate in lcd
//             [Return]whether horizontal motion is true
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsHorizontalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos)
{
    BOOLEAN ret = FALSE;
    TRACE_APP_LAUNCHER_ENTER
    ret = (abs(move_pos.x - press_pos.x) > SLIDEWIN_MOTION_THRES);
    TRACE_APP_LAUNCHER("ret = %d.", ret);
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}

/*****************************************************************************/
//  Description : judge the vertical motion is exist
//  Parameter: [In] press_pos: the tp press positon, which coordiate in lcd
//                  [In] move_pos: the tp move positon, which coordiate in lcd
//             [Return]whether vertical motion is true
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsVerticalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos)
{
    BOOLEAN ret = FALSE;
    TRACE_APP_LAUNCHER_ENTER
    ret = (abs(move_pos.y - press_pos.y) > SLIDEWIN_MOTION_THRES);
    TRACE_APP_LAUNCHER("ret = %d.", ret);
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}

/*****************************************************************************/
//  Description : common swap function
//  Parameter: [In] a: the address of first value
//                  [In] b: the address of second value
//             [Return] n/a
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void SwapInitToStopPos(int16 *a, int16 *b)
{
    int16 temp = *a;
    *a = *b;
    *b = temp;
}


/*****************************************************************************/
//  Description : Get SlideWin Array Header Ptr
//  Parameter: [In] n/a
//            [Return] return s_slidewin_array
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL tSlideWin_Node* SlideWinArray_GetArray(void)
{
    TRACE_APP_LAUNCHER_ENTER
    TRACE_APP_LAUNCHER("s_slidewin_array = %0X.", s_slidewin_array);
    TRACE_APP_LAUNCHER_EXIT
    return s_slidewin_array;
}


/*****************************************************************************/
//  Description : Get Slide Win Array Max Length
//  Parameter: [In] n/a
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL uint8 SlideWinArray_GetMaxLength(void)
{
    TRACE_APP_LAUNCHER_ENTER
    TRACE_APP_LAUNCHER("SYSTEM_SLIDEWIN_MAX_NUM = %d.", SYSTEM_SLIDEWIN_MAX_NUM);
    TRACE_APP_LAUNCHER_EXIT
    return SYSTEM_SLIDEWIN_MAX_NUM;
}


/*****************************************************************************/
//  Description : Add slidewin item to array
//  Parameter: [In] item: which will be add to array
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWinArray_Add(tSlideWinInfo* item)
{
    uint8 i = 0;
    BOOLEAN ret = FALSE;
    tSlideWin_Node* array = SlideWinArray_GetArray();
    uint8 max_len = SlideWinArray_GetMaxLength();
    uint8 index_for_add = 0;
    TRACE_APP_LAUNCHER_ENTER
    
    if (item == NULL)
    {
        TRACE_APP_LAUNCHER("item == NULL");
        return ret;
    }

    if (s_slidewin_num >= SYSTEM_SLIDEWIN_MAX_NUM)
    {
        TRACE_APP_LAUNCHER("SlideWinList is full.");
        return ret;
    }

    if (TRUE == SlideWinArray_FindFreeIndex(array, max_len, &index_for_add))
    {
        TRACE_APP_LAUNCHER("index_for_add = %d", index_for_add);
        array[index_for_add].item = item;
        array[index_for_add].exist = TRUE;
        ret = TRUE;
    }
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : remove slidewin item from array
//  Parameter: [In] item: which will be remove from array
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWinArray_Remove(tSlideWinInfo* item)
{
    BOOLEAN ret = FALSE;
    tSlideWin_Node* array = SlideWinArray_GetArray();
    uint8 max_len = SlideWinArray_GetMaxLength();
    int8 index = SYSTEM_SLIDEWIN_INVALID_INDEX;
    TRACE_APP_LAUNCHER_ENTER

    if ((index = SlideWinArray_FindIndex(item)) == SYSTEM_SLIDEWIN_INVALID_INDEX)
    {
        TRACE_APP_LAUNCHER("error. not find item, index = %d.", SYSTEM_SLIDEWIN_INVALID_INDEX);
        return ret;
    }

    array[index].item = NULL;
    array[index].exist = FALSE;
    
    TRACE_APP_LAUNCHER_EXIT
    ret = TRUE;
    return ret;
}


/*****************************************************************************/
//  Description : find slidewin item index from array
//  Parameter: [In] item: which will be remove from array
//            [Return] SYSTEM_SLIDEWIN_INVALID_INDEX/index
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL int8 SlideWinArray_FindIndex(tSlideWinInfo* item)
{
    uint8 i = 0;

    tSlideWin_Node* array = SlideWinArray_GetArray();
    uint8 max_len = SlideWinArray_GetMaxLength();
    TRACE_APP_LAUNCHER_ENTER

    if (item == NULL)
    {
        TRACE_APP_LAUNCHER("param in == NULL");
        return SYSTEM_SLIDEWIN_INVALID_INDEX;
    }

    for (i = 0; i < max_len; i++)
    {
        if (array[i].item == item)
        {
            TRACE_APP_LAUNCHER("find index = %d.", i);
            return i;
        }
    }

    TRACE_APP_LAUNCHER("find SYSTEM_SLIDEWIN_INVALID_INDEX.");
    TRACE_APP_LAUNCHER_EXIT
    return SYSTEM_SLIDEWIN_INVALID_INDEX;
}


/*****************************************************************************/
//  Description : is slidewin item in slide array
//  Parameter: [In] item: which will be find from array
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWinArray_IsItemValid(tSlideWinInfo* item)
{ 
    BOOLEAN ret = FALSE;
    TRACE_APP_LAUNCHER_ENTER
    ret = SlideWinArray_FindIndex(item) != SYSTEM_SLIDEWIN_INVALID_INDEX;
    TRACE_APP_LAUNCHER("ret = %d.", ret);
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : find free space in slide win array
//  Parameter: [In] array: slide win array address
//                  [In] max: slide win array length
//                [Out] index: address for storage find index
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWinArray_FindFreeIndex(tSlideWin_Node* array, uint8 max, uint8* index)
{
    BOOLEAN ret = FALSE;
    uint8 i = 0;
    TRACE_APP_LAUNCHER_ENTER
    if (array == NULL)
    {
        TRACE_APP_LAUNCHER("array == NULL");
        return ret;
    }

    for (i = 0; i < max; i++)
    {
        if (array[i].exist == FALSE)
        {
            *index = i;
            ret = TRUE;
            break;
        }
    }

    TRACE_APP_LAUNCHER("find free index = %d", i);
    TRACE_APP_LAUNCHER_EXIT

    return ret;
}


/*****************************************************************************/
//  Description : Compute Stop Postion After tp up message,support horizontal && vertical direction
//  Parameter: [In] item : the slide entity
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_CalcStopPosAfterTpUp(tSlideWinInfo *item)
{
    TRACE_APP_LAUNCHER_ENTER
    if (item->direction == SLIDEWIN_DIRECTION_VERT)
    {
        TRACE_APP_LAUNCHER("vertical direction begin");
        if (item->init_pos < item->stop_pos)
        {
            TRACE_APP_LAUNCHER("init_pos less than stop_pos. init_pos = %d, stop_pos = %d.", \
                item->init_pos , item->stop_pos);
            if (item->tp_up_pos.y > item->tp_press_pos.y)
            {
                TRACE_APP_LAUNCHER("motion down.");
            }
            else
            {
                TRACE_APP_LAUNCHER("motion up.");
                SwapInitToStopPos(&item->init_pos, &item->stop_pos);
            }
        }
        else
        {
            TRACE_APP_LAUNCHER("init_pos larger than stop_pos. init_pos = %d, stop_pos = %d.", \
                item->init_pos , item->stop_pos);

            if (item->tp_up_pos.y > item->tp_press_pos.y)
            {
                TRACE_APP_LAUNCHER("motion down.");
                SwapInitToStopPos(&item->init_pos, &item->stop_pos);
            }
            else
            {
                TRACE_APP_LAUNCHER("motion up.");
            }
        }
        TRACE_APP_LAUNCHER("vertical direction end.");
    }
    else if (item->direction == SLIDEWIN_DIRECTION_HORZ)
    {
        TRACE_APP_LAUNCHER("horizontal direction begin");
        if (item->init_pos < item->stop_pos)
        {
            TRACE_APP_LAUNCHER("init_pos less than stop_pos. init_pos = %d, stop_pos = %d.", \
                item->init_pos , item->stop_pos);
            if (item->tp_up_pos.x > item->tp_press_pos.x)
            {
                TRACE_APP_LAUNCHER("motion right.");
            }
            else
            {
                TRACE_APP_LAUNCHER("motion left.");
                SwapInitToStopPos(&item->init_pos, &item->stop_pos);
            }
        }
        else
        {
            TRACE_APP_LAUNCHER("init_pos larger than stop_pos. init_pos = %d, stop_pos = %d.", \
                item->init_pos , item->stop_pos);
            if (item->tp_up_pos.x > item->tp_press_pos.x)
            {
                TRACE_APP_LAUNCHER("motion right.");
                SwapInitToStopPos(&item->init_pos, &item->stop_pos);
            }
            else
            {
                TRACE_APP_LAUNCHER("motion left.");
            }
        }
        TRACE_APP_LAUNCHER("horizontal direction end");
    }
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : Start TouchPanel Drap Timer.
//  Parameter: [In] item : the slide entity
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_StartTpDrapTimer(tSlideWinInfo* item)
{
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    SlideWin_StopAutoSlideTimer(item);
    item->tp_drap_timer = MMK_CreateWinTimer(item->win_id, item->tp_drap_interval, TRUE);
    TRACE_APP_LAUNCHER("item->tp_drap_timer  = %d", item->tp_drap_timer );
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : Stop TouchPanel Drap Timer.
//  Parameter: [In] item : the slide entity
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_StopTpDrapTimer(tSlideWinInfo* item)
{
    TRACE_APP_LAUNCHER_ENTER
    if (item->tp_drap_timer != 0)
    {
        MMK_StopTimer(item->tp_drap_timer);
        item->tp_drap_timer = 0;
    }
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : Start Auto Slide Timer.
//  Parameter: [In] item : the slide entity
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_StartAutoSlideTimer(tSlideWinInfo* item)
{
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    SlideWin_StopAutoSlideTimer(item);
    item->auto_slide_timer = MMK_CreateWinTimer(item->win_id, item->auto_slide_interval, TRUE);
    TRACE_APP_LAUNCHER("item->auto_slide_interval  = %d", item->auto_slide_interval );
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : Stop Auto Slide Timer.
//  Parameter: [In] item : the slide entity
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_StopAutoSlideTimer(tSlideWinInfo* item)
{
    TRACE_APP_LAUNCHER_ENTER
    if (item->auto_slide_timer != 0)
    {
        MMK_StopTimer(item->auto_slide_timer);
        item->auto_slide_timer = 0;
    }
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : run process at auto mode
//  Parameter: [In] item : the slide entity
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWin_RunAutoMode(tSlideWinInfo* item)
{
    TRACE_APP_LAUNCHER_ENTER
    SlideWin_StartAutoSlideTimer(item);
    TRACE_APP_LAUNCHER_EXIT
    return TRUE;
}


/*****************************************************************************/
//  Description : run process at tp mode
//  Parameter: [In] item : the slide entity
//            [Return] true/false
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
#ifdef TOUCH_PANEL_SUPPORT
LOCAL BOOLEAN SlideWin_RunTpMode(tSlideWinInfo* item)
{
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    TRACE_APP_LAUNCHER_ENTER
    MMK_GetLogicTPMsg(&tp_status, &item->tp_press_pos);
	TRACE_APP_LAUNCHER("tp_press_pos.x = %d, tp_press_pos.y = %d.", item->tp_press_pos.x, item->tp_press_pos.y);
    SlideWin_SetState(item, SLIDEWIN_STATE_TP_PRESSED);
    MMK_SetTPDownWin(0);

    SlideWin_StartTpDrapTimer(item);
    TRACE_APP_LAUNCHER_EXIT
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : create slide layer
//  Parameter: [In] win_id : window for slide display
//                  [In] width : window width
//                  [In] width : window height
//                  [Out] lcd_dev_ptr : the addr for slide layer
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_CreateSlideLayer(MMI_WIN_ID_T win_id,
                                                    uint16 width,
                                                    uint16 height,
                                                    GUI_LCD_DEV_INFO *lcd_dev_ptr)
{
    
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_CREATE_T create_info = {0};
    TRACE_APP_LAUNCHER_ENTER
    if (PNULL == lcd_dev_ptr)
    {
        TRACE_APP_LAUNCHER("PNULL == lcd_dev_ptr.");
        return;
    }

    TRACE_APP_LAUNCHER("win_id = %0X, width = %d, height = %d", win_id, width, height);
    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;

    // ´´½¨²ã
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE;

    UILAYER_CreateLayer(
        &create_info,
        lcd_dev_ptr
        );

    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(lcd_dev_ptr);

 TRACE_APP_LAUNCHER("*lcd_dev_ptr= %d.", *lcd_dev_ptr);
#endif
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : append slide layer to blt layer
//  Parameter: [In] lcd_dev_ptr : lcd id
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_AppendSlideLayerToBlt(GUI_LCD_DEV_INFO lcd_dev_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_APPEND_BLT_T append_layer = {0};
    TRACE_APP_LAUNCHER_ENTER
    append_layer.lcd_dev_info = lcd_dev_ptr;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
#endif
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : set slide item state 
//  Parameter: [In] item : the slide entity
//                  [In] st : the slide state
//                              SLIDEWIN_STATE_IDLE 0X0000
//                              SLIDEWIN_STATE_TP_PRESSED 0X0001
//                              SLIDEWIN_STATE_TP_MOVING  0X0002
//                              SLIDEWIN_STATE_TP_UPED       0X0004
//                              SLIDEWIN_STATE_TP_AUTO_SLIDING 0X0008
//                              SLIDEWIN_STATE_DRAPING 0X0010
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_SetState(tSlideWinInfo* item, uint32 st)
{
    TRACE_APP_LAUNCHER_ENTER
    if (item == NULL)
        return;
    item->state |= st;
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : clear slide item state 
//  Parameter: [In] item : the slide entity
//                  [In] st : the slide state
//                              SLIDEWIN_STATE_IDLE 0X0000
//                              SLIDEWIN_STATE_TP_PRESSED 0X0001
//                              SLIDEWIN_STATE_TP_MOVING  0X0002
//                              SLIDEWIN_STATE_TP_UPED       0X0004
//                              SLIDEWIN_STATE_TP_AUTO_SLIDING 0X0008
//                              SLIDEWIN_STATE_DRAPING 0X0010
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL void SlideWin_ClearState(tSlideWinInfo* item, uint32 st)
{
    TRACE_APP_LAUNCHER_ENTER
    if (item == NULL)
        return;
    item->state &= (~st);
    TRACE_APP_LAUNCHER_EXIT
}


/*****************************************************************************/
//  Description : judge slide entity state is open
//  Parameter: [In] item : the slide entity
//                  [In] st : the slide state
//                              SLIDEWIN_STATE_IDLE 0X0000
//                              SLIDEWIN_STATE_TP_PRESSED 0X0001
//                              SLIDEWIN_STATE_TP_MOVING  0X0002
//                              SLIDEWIN_STATE_TP_UPED       0X0004
//                              SLIDEWIN_STATE_TP_AUTO_SLIDING 0X0008
//                              SLIDEWIN_STATE_DRAPING 0X0010
//            [Return] n/a
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlideWin_IsInState(tSlideWinInfo* item, uint32 st)
{
    TRACE_APP_LAUNCHER_ENTER
    return (item->state & st) == st; 
}


/*****************************************************************************/
//  Description : process msg_open_window at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcOpenWindow(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    //get win rect;
    GUI_RECT_T win_rect ={0};
    MMI_WIN_ID_T win_id = 0;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    win_id = item->win_id;
        
    MMK_GetWinRect(win_id, &win_rect);
    TRACE_APP_LAUNCHER("left = %d, top = %d, right = %d, bottom = %d,", \
        win_rect.left, win_rect.right, win_rect.top, win_rect.bottom);

    TRACE_APP_LAUNCHER("slide_layer width = %d, heigth = %d", \
        win_rect.right - win_rect.left + 1, win_rect.bottom- win_rect.top + 1);

    TRACE_APP_LAUNCHER("invoke SlideWin_CreateSlideLayer().");
    SlideWin_CreateSlideLayer(win_id,
                                win_rect.right - win_rect.left + 1,
                                win_rect.bottom- win_rect.top + 1,
                                &item->slide_layer);

    if (item->direction == SLIDEWIN_DIRECTION_HORZ)
        UILAYER_SetLayerPosition(&item->slide_layer, item->init_pos, 0);

    if (item->direction == SLIDEWIN_DIRECTION_VERT)
        UILAYER_SetLayerPosition(&item->slide_layer, 0, item->init_pos);

    SYSTEM_SLIDEWIN_DEACTIVEHOOK(item)
    SYSTEM_SLIDEWIN_ACTIVEHOOK(item)

    SlideWin_AppendSlideLayerToBlt(item->slide_layer);

    MMK_SetWinLcdDevInfoNotifyCtrl(win_id, &item->slide_layer);
    TRACE_APP_LAUNCHER_EXIT

    return ret;
}


/*****************************************************************************/
//  Description : process msg_close_window at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcCloseWindow(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    if ((item->style & SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE) == SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE)
    {
        WatchSLIDEWIN_Destory((uint32)item);
        return ret;
    }
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}

/*****************************************************************************/
//  Description : process msg_get_focus at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcGetFocus(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    SlideWin_AppendSlideLayerToBlt(item->slide_layer);

    SYSTEM_SLIDEWIN_DEACTIVEHOOK(item)
    UILAYER_SetLayerPosition(&item->slide_layer, 0, 0);
    MMK_SendMsg(item->win_id, msg_id, param);
    MMK_SendMsg(item->win_before_id, MSG_FULL_PAINT, NULL);
    SYSTEM_SLIDEWIN_ACTIVEHOOK(item)
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process msg_lose_focus at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcLoseFocus(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    UILAYER_RemoveBltLayer(&item->slide_layer);
    SlideWin_StopTpDrapTimer(item);//fix bug1643155
    return ret;
}


/*****************************************************************************/
//  Description : process msg_full_paint at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcFullPaint(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    TRACE_APP_LAUNCHER_ENTER

    SLIDEWIN_CHECK_ITEM_VALID(item)

    MMK_SetWinLcdDevInfoNotifyCtrl(item->win_id, &item->slide_layer);

    SYSTEM_SLIDEWIN_DEACTIVEHOOK(item)
    //must translate layer postion to orgin point, before repaint window's content.
    UILAYER_SetLayerPosition(&item->slide_layer, 0, 0);
    MMK_SendMsg(MMK_ConvertIdToHandle(item->win_id), msg_id, param);
    SYSTEM_SLIDEWIN_ACTIVEHOOK(item)


    //the param below control the really window display 
    if (item->direction == SLIDEWIN_DIRECTION_HORZ)
    {
        TRACE_APP_LAUNCHER("SLIDEWIN_DIRECTION_HORZ. item->cur_pos = %d.", item->cur_pos);
        UILAYER_SetLayerPosition(&item->slide_layer, item->cur_pos, 0);
    }
    else if (item->direction == SLIDEWIN_DIRECTION_VERT)
    {
        TRACE_APP_LAUNCHER("SLIDEWIN_DIRECTION_VERT. item->cur_pos = %d.", item->cur_pos);
        UILAYER_SetLayerPosition(&item->slide_layer, 0, item->cur_pos);
    }
    else
    {
        TRACE_APP_LAUNCHER("error.no support direction");
    }

    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process tp move timer at hook
//  Parameter: [In]item : the slide entity
//            [Return] MMI_RESULT_TRUE/MMI_RESULT_FALSE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_HandleTpDrapTimer(tSlideWinInfo *item)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    GUI_POINT_T point = {0};
    int16 offset = 0;
    func_motionDetect fun_motionDetect = NULL;
    TRACE_APP_LAUNCHER_ENTER

    if (FALSE == SlideWin_IsInState(item, SLIDEWIN_STATE_TP_PRESSED))
    {
        TRACE_APP_LAUNCHER("do nothing at tp move before tp pressed.");
        return ret;
    }

    //get current tp postion
    MMK_GetLogicTPMsg(&tp_status, &point);
    TRACE_APP_LAUNCHER("point.x = %d, point.y = %d.", point.x, point.y);

    if (item->direction == SLIDEWIN_DIRECTION_VERT)
    {
        offset = point.y - item->tp_press_pos.y;
        TRACE_APP_LAUNCHER("SLIDEWIN_DIRECTION_VERT,offset = %d.", offset);
        fun_motionDetect = IsVerticalMotion;
    }
    else if (item->direction == SLIDEWIN_DIRECTION_HORZ)
    {
        offset = point.x - item->tp_press_pos.x;
        TRACE_APP_LAUNCHER("SLIDEWIN_DIRECTION_HORZ,offset = %d.", offset);
        fun_motionDetect = IsHorizontalMotion;
    }
    else
    {
        TRACE_APP_LAUNCHER("error.no support direction");
    }

    if(FALSE == SlideWin_IsInState(item, SLIDEWIN_STATE_DRAPING))
    {
        if(fun_motionDetect(item->tp_press_pos, point))
        {
            SlideWin_SetState(item, SLIDEWIN_STATE_DRAPING);
            TRACE_APP_LAUNCHER("SLIDEWIN_STATE_DRAPING = TRUE.");

            SYSTEM_SLIDEWIN_DEACTIVEHOOK(item)
            MMK_SendMsg(item->win_id, MSG_SLIDEWIN_BEGIN, NULL);
            SYSTEM_SLIDEWIN_ACTIVEHOOK(item)
        }
        else
        {
            TRACE_APP_LAUNCHER("not expand drap thres.");
            return ret;
        }
    }
    
    item->cur_pos = item->init_pos + offset;
    TRACE_APP_LAUNCHER("[item->cur_pos = item->init_pos + offset], \
        item->init_pos = %d, offset =%d, item->cur_pos = %d.", item->init_pos, offset, item->cur_pos);

    if ((item->style & SLIDEWIN_STYLE_ENABLE_RANGE) == SLIDEWIN_STYLE_ENABLE_RANGE)
    {
        item->cur_pos = max(min(item->cur_pos, item->range_max), item->range_min);
        TRACE_APP_LAUNCHER("range detect enable. after range detect, item->cur_pos = %d.", item->cur_pos);
    }

    if (item->direction == SLIDEWIN_DIRECTION_HORZ)
    {
        UILAYER_SetLayerPosition(&item->slide_layer, item->cur_pos, 0);
    }
    else if (item->direction == SLIDEWIN_DIRECTION_VERT)
    {
        UILAYER_SetLayerPosition(&item->slide_layer, 0, item->cur_pos);
    }
    else
    {
        TRACE_APP_LAUNCHER("error.no support direction");
    }
#endif

    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process auto sldie timer at hook
//  Parameter: [In]item : the slide entity
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_HandleAutoSlideTimer(tSlideWinInfo *item)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    int16 step = 0;
    TRACE_APP_LAUNCHER_ENTER
    if (item == NULL)
    {
        return ret;
    }
    step = item->stop_pos > item->cur_pos ?
              SLIDEWIN_AUTOSLIDE_DEFAULT_STEP
              : -SLIDEWIN_AUTOSLIDE_DEFAULT_STEP;

    TRACE_APP_LAUNCHER("step = %d.", step);
    if (step >= 0)
    {
        item->cur_pos  = item->cur_pos + step >= item->stop_pos
            ? item->stop_pos
            : item->cur_pos + step;
    }
    else
    {
        item->cur_pos  = item->cur_pos + step <= item->stop_pos
            ? item->stop_pos
            : item->cur_pos + step;
    }

    if (item->direction == SLIDEWIN_DIRECTION_HORZ)
    {
        UILAYER_SetLayerPosition(&item->slide_layer, item->cur_pos, 0);
    }
    else if (item->direction == SLIDEWIN_DIRECTION_VERT)
    {
        UILAYER_SetLayerPosition(&item->slide_layer, 0, item->cur_pos);
    }

    TRACE_APP_LAUNCHER("item->cur_pos = %d.", item->cur_pos);

    //if auto slide finished, stop auto slide time 
    if (item->stop_pos == item->cur_pos)
    {
        TRACE_APP_LAUNCHER("stop auto slide.");
        SwapInitToStopPos(&item->init_pos, &item->stop_pos);
        SlideWin_StopAutoSlideTimer(item);
        MMK_SendMsg(item->win_id, MSG_SLIDEWIN_END, NULL);
    }

    //if support auto free.
    if ((item->style & SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE) == SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE)
    {
        TRACE_APP_LAUNCHER("support SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE.");
        if (item->cur_pos == item->close_pos)
        {
            TRACE_APP_LAUNCHER("auto close win when item->cur_pos == item->close_pos.");
            MMITHEME_UpdateRect();//2087419
            MMK_CloseWin(item->win_id);
            return MMI_RESULT_TRUE;
        }
    }

	ret = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process msg_timer at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcOnTimer(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    uint8 timer_id = 0;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)

    if (param == NULL)
    {
        return FALSE;
    }

    timer_id = *(uint8*)(param);

    if (timer_id == item->tp_drap_timer)
    {
        //do tp drap
        TRACE_APP_LAUNCHER("invoke SlideWin_HandleTpDrapTimer().");
        SlideWin_HandleTpDrapTimer(item);
    }
    else if (timer_id == item->auto_slide_timer)
    {
        //do auto slide
        TRACE_APP_LAUNCHER("invoke SlideWin_HandleAutoSlideTimer().");
        SlideWin_HandleAutoSlideTimer(item);
    }
    else
    {
        ret = MMI_RESULT_FALSE;
    }

    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process tp press at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMI_RESULT_E SlideWin_ProcTpPress(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    uint8 timer_id = 0;
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    GUI_POINT_T point = {0};
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    TRACE_APP_LAUNCHER("tp pressed. point.x = %d, point.y = %d.", point.x, point.y);

    //clear all state
    item->state = 0;
    TRACE_APP_LAUNCHER("clear item state,item->state = 0.");

    if ((item->style & SLIDEWIN_STYLE_ENABLE_ROI) == SLIDEWIN_STYLE_ENABLE_ROI)
    {
        TRACE_APP_LAUNCHER("support style SLIDEWIN_STYLE_ENABLE_ROI.");
        if (GUI_PointIsInRect(point, item->roi_region))
        {
            TRACE_APP_LAUNCHER("tp press in roi.");
            SlideWin_SetState(item, SLIDEWIN_STATE_TP_PRESSED);
        }
    }
    else
    {
        TRACE_APP_LAUNCHER("no support style SLIDEWIN_STYLE_ENABLE_ROI.");
        SlideWin_SetState(item, SLIDEWIN_STATE_TP_PRESSED);
    }

    if (TRUE == SlideWin_IsInState(item, SLIDEWIN_STATE_TP_PRESSED))
    {
        TRACE_APP_LAUNCHER("set state - SLIDEWIN_STATE_TP_PRESSED");
        //save tp press position
        item->tp_press_pos = point;

        SlideWin_StartTpDrapTimer(item);
        ret = MMI_RESULT_TRUE;
    }

    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process tp move at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcTpMove(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    uint8 timer_id = 0;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    SlideWin_SetState(item, SLIDEWIN_STATE_TP_MOVING);
    TRACE_APP_LAUNCHER("set state - SLIDEWIN_STATE_TP_MOVING");
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : process tp up at hook
//  Parameter: [In]item : the slide entity
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_ProcTpUp(tSlideWinInfo *item, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    uint8 timer_id = 0;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)

    //save tp up pos
    item->tp_up_pos.x = MMK_GET_TP_X(param);
    item->tp_up_pos.y = MMK_GET_TP_Y(param);
    TRACE_APP_LAUNCHER("tp up. point.x = %d, point.y = %d.", item->tp_up_pos.x, item->tp_up_pos.y);

    TRACE_APP_LAUNCHER("invoke SlideWin_StopTpDrapTimer().");
    SlideWin_StopTpDrapTimer(item);

    //if support auto free.
    if ((item->style & SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE) == SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE)
    {
        TRACE_APP_LAUNCHER("support style - SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE.");
        if (item->cur_pos == item->close_pos)
        {
            TRACE_APP_LAUNCHER("item->cur_pos == item->close_pos = %d.", item->cur_pos);
            TRACE_APP_LAUNCHER("invoke MMK_CloseWin(), win_id = %d.", item->win_id);
            MMK_CloseWin(item->win_id);
            return MMI_RESULT_TRUE;
        }
    }

    if(TRUE == SlideWin_IsInState(item, SLIDEWIN_STATE_DRAPING))
    {
        TRACE_APP_LAUNCHER("stop drap.");

        TRACE_APP_LAUNCHER("stop drap timer.");
        SlideWin_StartAutoSlideTimer(item);

        TRACE_APP_LAUNCHER("invoke SlideWin_CalcStopPosAfterTpUp().");
        SlideWin_CalcStopPosAfterTpUp(item);
        ret = MMI_RESULT_TRUE;
    }
    else
    {
        ret = MMI_RESULT_FALSE;
    }

    SlideWin_SetState(item, SLIDEWIN_STATE_TP_UPED);
	item->state = 0;
    TRACE_APP_LAUNCHER("set state - SLIDEWIN_STATE_TP_UPED");
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}
#endif

/*****************************************************************************/
//  Description : slide win hook function
//  Parameter: [In]win_id : the window id
//                  [In]msg_id : the message id
//                  [In]param : the additional param
//            [Return] MMI_RESULT_TRUE
//  Author: zhikun.lv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SlideWin_HandleMsgHook(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    tSlideWinInfo *item = (tSlideWinInfo *)MMK_GetWinUserData(win_id);
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)
    TRACE_APP_LAUNCHER("win_id = %d, msg_id = %0x", win_id, msg_id);

	if (SlideWin_IsInState(item, SLIDEWIN_STATE_DRAPING) || SlideWin_IsInState(item, SLIDEWIN_STATE_TP_AUTO_SLIDING))
	{
		if (msg_id == MSG_TP_PRESS_DOWN || msg_id == MSG_TP_PRESS_MOVE || msg_id == MSG_FULL_PAINT)
		{
			TRACE_APP_LAUNCHER("error. return");
			return ret;
		}
	}

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_LAUNCHER("MSG_OPEN_WINDOW.");
            ret = SlideWin_ProcOpenWindow(item, msg_id, param);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_LAUNCHER("MSG_CLOSE_WINDOW.");
            ret = SlideWin_ProcCloseWindow(item, msg_id, param);
            break;
        }

        case MSG_GET_FOCUS:
        {
            TRACE_APP_LAUNCHER("MSG_GET_FOCUS.");
            ret = SlideWin_ProcGetFocus(item, msg_id, param);
            break;
        }

        case MSG_LOSE_FOCUS:
        {
            TRACE_APP_LAUNCHER("MSG_LOSE_FOCUS.");
            ret = SlideWin_ProcLoseFocus(item, msg_id, param);
            break;
        }

        case MSG_FULL_PAINT:
        {
            TRACE_APP_LAUNCHER("MSG_FULL_PAINT.");
            ret = SlideWin_ProcFullPaint(item, msg_id, param);
            break;
        }

        case MSG_TIMER:
        {
            TRACE_APP_LAUNCHER("MSG_TIMER.");
            ret = SlideWin_ProcOnTimer(item, msg_id, param);
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_DOWN.");
            ret = SlideWin_ProcTpPress(item, msg_id, param);
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_MOVE.");
            ret = SlideWin_ProcTpMove(item, msg_id, param);
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_MOVE.");
            ret = SlideWin_ProcTpUp(item, msg_id, param);
            break;
        }

#endif

        default:
            TRACE_APP_LAUNCHER("default.");
            ret = MMI_RESULT_FALSE;
            break;
    }

    //in any msg, if this win closed,must destory slidewin item,forbid mem leak.
    if (FALSE == MMK_IsOpenWin(item->win_id))
    {
        WatchSLIDEWIN_Destory((uint32)item);
        ret =  MMI_RESULT_TRUE;
    }

    TRACE_APP_LAUNCHER("ret = %d", ret);
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : create slide win entity, which return reference slide handle,if error occur, null will be return.
//  Parameter: [In]init_data : initilize information
//            [Return] slide win handle, in heap memory
//  Author: zhikun.lv
//  Note: must invoke this function after mmk_createwin.
//           also, the win userdata ptr must be null !!!!!!
/*****************************************************************************/
PUBLIC uint32 WatchSLIDEWIN_Create(tSlideWinInitData* init_data)
{
    tSlideWinInfo *item = NULL;
    TRACE_APP_LAUNCHER_ENTER

    if (init_data == NULL)
    {
        TRACE_APP_LAUNCHER("init_data == NULL");
        return (uint32)item;
    }

    // to bind
    if (FALSE == MMK_IsOpenWin(init_data->win_id))
    {
        TRACE_APP_LAUNCHER("error!window to bind is not opened!");
        return (uint32)item;
    }

    TRACE_APP_LAUNCHER("init_data info:");
    TRACE_APP_LAUNCHER("win_id = %0X, win_before_id = %0X, direction = %d, mode = %d, style = %d, \
        init_pos = %d, stop_pos= %d, close_pos = %d.", \
        init_data->win_id,
        init_data->win_before_id,
        init_data->direction,
        init_data->mode,
        init_data->style,
        init_data->init_pos,
        init_data->stop_pos,
        init_data->close_pos);

	TRACE_APP_LAUNCHER("s_slidewin_num = %d.",s_slidewin_num);

    item = (tSlideWinInfo *)SCI_ALLOC_APPZ(sizeof(tSlideWinInfo));

    if (NULL == item)
    {
        TRACE_APP_LAUNCHER("pSlideWin == NULL");
        return (uint32)item;
    }

    //init data cpy
    SCI_MEMCPY(item, init_data, sizeof(tSlideWinInitData));

    //remove it for slidewinarray
    if (FALSE == SlideWinArray_Add(item))
    {
        return item;
    }

    MMK_SetWinUserData(MMK_ConvertIdToHandle(item->win_id), item);

    TRACE_APP_LAUNCHER("handle = 0X%X.", item);
    SYSTEM_SLIDEWIN_ACTIVEHOOK(item)
    item->tp_drap_interval = SLIDEWIN_TPMOVE_INTERVAL;
    item->auto_slide_interval = SLIDEWIN_AUTOSLIDE_INTERVAL;
    item->cur_pos = item->init_pos;
    item->slide_layer.block_id = UILAYER_NULL_HANDLE;

    TRACE_APP_LAUNCHER_EXIT
    return (uint32)item;
}


/*****************************************************************************/
//  Description : destory slide win entity
//  Parameter: [In]handle:slide win handle
//            [Return] TRUE/FALSE
//  Author: zhikun.lv
//  Note: must call this function with WatchSLIDEWIN_Create on pair,otherwise,forbid leak memory.
/*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEWIN_Destory(uint32 handle)
{
    
    BOOLEAN ret = FALSE;
    tSlideWinInfo* item = (tSlideWinInfo*)handle;
    TRACE_APP_LAUNCHER_ENTER
    SLIDEWIN_CHECK_ITEM_VALID(item)

    if (FALSE == SlideWinArray_IsItemValid(item))
    {
        TRACE_APP_LAUNCHER("not vaild handle.");
        return ret;
    }

    //remove it for slidewinarray
    if (FALSE == SlideWinArray_Remove(item))
    {
        TRACE_APP_LAUNCHER("FALSE == SlideWinArray_Remove().");
        return ret;
    }

    TRACE_APP_LAUNCHER("handle = 0X%X.", item);

    //free move layer;
    UILAYER_RELEASELAYER(&item->slide_layer);

    //stop timer
    MMK_StopTimer(item->tp_drap_timer);
    MMK_StopTimer(item->auto_slide_timer);

    MMK_SetWinHookFunc(item->win_id, NULL);
    //MMK_SetFocusWin(MMK_ConvertIdToHandle(item->win_before_id));
    TRACE_APP_LAUNCHER("set focus win,win_before_id = 0X%d.", item->win_before_id);

    //free item self
    SCI_FREE(item);
    
    ret = TRUE;
    TRACE_APP_LAUNCHER_EXIT
    return ret;
}


/*****************************************************************************/
//  Description : start slide win entity
//  Parameter: [In]handle:slide win handle
//            [Return] TRUE/FALSE
//  Author: zhikun.lv
//  Note: must call this function after WatchSLIDEWIN_Create().use handle return by WatchSLIDEWIN_Create
/*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEWIN_Start(uint32 handle)
{
    BOOLEAN ret = FALSE;
    tSlideWinInfo* item = (tSlideWinInfo*)handle;
    func_SlideWinBase func_runMode = NULL;
    TRACE_APP_LAUNCHER_ENTER
    if (item == NULL)
    {
        TRACE_APP_LAUNCHER("item == NULL");
        return ret;
    }


    switch (item->mode)
    {
        case SLIDEWIN_STARTUPMODE_TP:
        {
#ifdef TOUCH_PANEL_SUPPORT
            func_runMode = SlideWin_RunTpMode;
#endif
            break;
        }

        case SLIDEWIN_STARTUPMODE_AUTO:
        {
            func_runMode = SlideWin_RunAutoMode;
            break;
        }
        default:
            func_runMode = SlideWin_RunAutoMode;
            break;
    }

    if (func_runMode != NULL)
    {
        func_runMode(item);
    }
	else
	{
		TRACE_APP_LAUNCHER("func_runMode == NULL");
	}
    TRACE_APP_LAUNCHER_EXIT
    ret = TRUE;
    return ret;
}


