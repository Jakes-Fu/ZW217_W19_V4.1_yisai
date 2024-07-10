/****************************************************************************
** File Name:      watch_slidepage.c                                                                                *
** Author:          zhikun.lv                                                                                             *
** Date:             3/2/2020                                                                                            *
** Copyright:      2020 Spreatrum, Incoporated. All Rights Reserved.                                    *
** Description:    This file is used to describe punctuation                                                   *
******************************************************************************
**                         Important Edit History                                                                        *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                    *
** 3/2/2020     zhikun.lv        Create
** 3/10/2020   zhikun.lv         Add Cycle attribute.
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                                                                      *
**---------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmk_timer.h"
#include "mmi_text.h"
#include "watch_slidepage.h"

#include "mmk_tp.h"
#include "mmicom_trace.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define TRACE_APP_SLIDEPAGE_ENTER TRACE_APP_SLIDEPAGE("enter")
#define TRACE_APP_SLIDEPAGE_EXIT TRACE_APP_SLIDEPAGE("exit")

#define SLIDE_PAGE_HANDLE_MAGIC 10000

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL tWatchSlidePage s_watchslidepage_entity[SLIDE_PAGE_ENTITY_MAX] = {0};
LOCAL tWatchSlidePage* s_actived_entity = NULL;
LOCAL const GUI_RECT_T SLIDE_PAGE_WIN_RECT[SLIDE_PAGE_NUM] =
                                            {
                                            {-SLIDE_PAGE_WIDTH,0,0,SLIDE_PAGE_WIDTH},
                                            {0,0,SLIDE_PAGE_WIDTH,SLIDE_PAGE_WIDTH},
                                            {SLIDE_PAGE_WIDTH,0,2*SLIDE_PAGE_WIDTH,SLIDE_PAGE_WIDTH},
                                            };

LOCAL const GUI_RECT_T SLIDE_PAGE_WIN_RECT_INVALID = {-2 * SLIDE_PAGE_WIDTH,0,-SLIDE_PAGE_WIDTH,SLIDE_PAGE_WIDTH};

LOCAL uint32 lasttime = 0;
//LOCAL MMK_DAMP_PLAY_TYPE_E play_type = DAMP_PLAY_TYPE_5; //cid 609996


typedef void (*funLoadSlidePageReady)(const tWatchSlidePage*, BOOLEAN);

LOCAL GUI_LCD_DEV_INFO s_slide_lcd[SLIDE_PAGE_NUM] = {0};
LOCAL BOOLEAN s_slide_lcd_ready = FALSE;
LOCAL BOOLEAN s_enable_slide_buffer = FALSE;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void CloseAllTpDrapTimer();
LOCAL void SetActivedEntity(tWatchSlidePage* handle);
LOCAL tWatchSlidePage* GetActivedEntity();
LOCAL MMI_RESULT_E HandleSlidePageMsgHook(MMI_WIN_ID_T win_id,
                                                                                      MMI_MESSAGE_ID_E msg_id,
                                                                                      DPARAM param);
LOCAL void RecordTpPressedWinRect(tWatchSlidePage *slide_page);
LOCAL BOOLEAN IsHorizontalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos);
LOCAL MMI_WIN_ID_T GetWinIdByIndex(const tWatchSlidePage *slide_page, uint8 index);
LOCAL GUI_RECT_T GetWinPressedRectByIndex(const tWatchSlidePage *slide_page, uint8 index);
LOCAL uint8 CalcNextPageIndex(tWatchSlidePage *slide_page, uint8 cur_index, uint8 page_cnt);
LOCAL uint8 CalcPrevPageIndex(tWatchSlidePage *slide_page, uint8 cur_index, uint8 page_cnt);

LOCAL void StartAutoSlide(tWatchSlidePage *slide_page);
LOCAL void StopAutoSlide(tWatchSlidePage *slide_page);
LOCAL BOOLEAN PrepareAutoSlideAfterTpUp(tWatchSlidePage *slide_page);
LOCAL void ProcessAutoSlideAfterTpUp(tWatchSlidePage *slide_page);
LOCAL void ProcessAutoSlideFinished(tWatchSlidePage *slide_page);
LOCAL void ProcessAutoSlideTranslation(tWatchSlidePage *slide_page,
                                                                         MMI_WIN_ID_T win_id,
                                                                         int16 offset);
LOCAL void LoadSlidePageReady(const tWatchSlidePage* slide_page, BOOLEAN b_screenshoot);
LOCAL void LoadSlidePageReady_Multi(const tWatchSlidePage* slide_page, BOOLEAN b_screenshoot);
LOCAL uint8 FindSlideIndexByWinId(MMI_WIN_ID_T win_id);
LOCAL tWatchSlidePage *FindSlideEntityByWinId(MMI_WIN_ID_T win_id);
//LOCAL void CloseAllDeactivedWinInEntity(tWatchSlidePage *slide_page);
LOCAL void SendAllDeactivedWinInEntity(tWatchSlidePage *slide_page);
LOCAL void HandleKeyRightMsg(tWatchSlidePage *slide_page);
LOCAL void HandleKeyLeftMsg(tWatchSlidePage *slide_page);
LOCAL void HandleKeyLeftMsgDirect(tWatchSlidePage *slide_page);
LOCAL void HandleKeyRightMsgDirect(tWatchSlidePage *slide_page);

LOCAL uint8 GetFirstValidOne(tWatchSlidePage *slide_page);
LOCAL uint8 GetLastValidOne(tWatchSlidePage *slide_page);

/****************************************************************/

LOCAL funLoadSlidePageReady s_funLoadSlidePageReady = LoadSlidePageReady;

LOCAL void CloseAllTpDrapTimer()
{
    uint8 i = 0;
    //close all active tp_drap_timer,
    for (i = 0; i < SLIDE_PAGE_ENTITY_MAX; i++)
    {
        if (s_watchslidepage_entity[i].tp_drap_timer != 0)
        {
            TRACE_APP_SLIDEPAGE("tp_drap_timer = %d", s_watchslidepage_entity[i].tp_drap_timer);
            MMK_StopTimer(s_watchslidepage_entity[i].tp_drap_timer);
            s_watchslidepage_entity[i].tp_drap_timer = 0;
        }
    }
}

LOCAL void SetActivedEntity(tWatchSlidePage* handle)
{
    TRACE_APP_SLIDEPAGE_ENTER;

    //close all active tp_drap_timer,
    CloseAllTpDrapTimer();
    
    if (s_actived_entity != handle)
    {
        s_funLoadSlidePageReady(handle, FALSE);
        s_actived_entity = handle;
    }
    TRACE_APP_SLIDEPAGE_EXIT;
}

LOCAL tWatchSlidePage* GetActivedEntity()
{
    TRACE_APP_SLIDEPAGE("s_actived_entity = %0x.", s_actived_entity);
    return s_actived_entity;
}


typedef struct{
    BOOLEAN is_backup;
    UILAYER_APPEND_BLT_T blt_array[UILAYER_TOTAL_BLT_COUNT];
    uint32 blt_layer_count;

}BLT_BACKUP;

BLT_BACKUP blt_backup_info = {0};

LOCAL void blt_backup(void)
{
     int32 len = sizeof(blt_backup_info);
     int32 i=0;
     memset(&blt_backup_info, 0 , len);
     blt_backup_info.blt_layer_count = UILAYER_GetBltLayer(blt_backup_info.blt_array, UILAYER_TOTAL_BLT_COUNT);
     blt_backup_info.is_backup = TRUE;
     for(i=0; i<blt_backup_info.blt_layer_count; i++)
     {
        UILAYER_RemoveBltLayer(&blt_backup_info.blt_array[i].lcd_dev_info);
     }
}

LOCAL void blt_restore(void)
{
    int32 i=0;

    if(blt_backup_info.blt_layer_count > UILAYER_TOTAL_BLT_COUNT)
    {
        blt_backup_info.blt_layer_count = UILAYER_TOTAL_BLT_COUNT;
    }
    if(blt_backup_info.is_backup)
    {
        for(i=0; i < blt_backup_info.blt_layer_count; i++)
        {
            UILAYER_AppendBltLayer(&blt_backup_info.blt_array[i]);
        }
    }
    blt_backup_info.is_backup = FALSE;
    blt_backup_info.blt_layer_count = 0;
}


#ifdef TOUCH_PANEL_SUPPORT
LOCAL void SlidePageDrapTimerOut( 
                               uint8  timer_id,
                               uint32 param
                               )
{
    int16 move_offset = 0;
    GUI_POINT_T tp_point = {0};
    tWatchSlidePage *entity = GetActivedEntity();
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    uint32 curtime = SCI_GetTickCount();
    uint32 endtime = 0;
    if (entity == NULL)
        return;

    if (timer_id != entity->tp_drap_timer)
    {
        return;
    }
    MMK_StopTimer(timer_id);
    TRACE_APP_SLIDEPAGE("time interval = %d.", curtime - lasttime);
    lasttime = curtime;

    MMK_GetLogicTPMsg(&tp_status, &tp_point);

    if (!entity->is_draped && IsHorizontalMotion(entity->tp_press_point, tp_point))
    {
        s_funLoadSlidePageReady(entity, TRUE);
        //drap move begin,repaint win detail to slide buffer;
        if (s_enable_slide_buffer)
        {
            uint8 loop = 0;
            blt_backup();
            for (loop = 0; loop < SLIDE_PAGE_NUM; loop++)
            {
                UILAYER_APPEND_BLT_T append_info = {0};
                append_info.layer_level = UILAYER_LEVEL_NORMAL;
                append_info.lcd_dev_info = s_slide_lcd[loop];
                UILAYER_AppendBltLayer(&append_info);
            }
        }

        TRACE_APP_SLIDEPAGE("begin drap.");
        entity->is_draped = TRUE;
    }

    if (entity->is_draped)
    {
#if 0
        if (abs(entity->tp_last_point.x - tp_point.x) < SLIDE_PAGE_TPDRAP_THRES)
        {
            TRACE_APP_SLIDEPAGE("lzk001. drap thres not exceed, do nothing.");
            return;
        }
#endif
        entity->tp_last_point = tp_point;
        move_offset = tp_point.x - entity->tp_press_point.x;
        entity->direction = tp_point.x > entity->tp_press_point.x ? WATCH_SLIDE_DIRECTION_RIGHT : WATCH_SLIDE_DIRECTION_LEFT;

        if (!entity->is_support_cycle)
        {
            if (entity->direction == WATCH_SLIDE_DIRECTION_RIGHT
                && GetFirstValidOne(entity) == entity->cur_page_index)
            {
                TRACE_APP_SLIDEPAGE("exit.");
                entity->tp_drap_timer = MMK_CreateTimerCallback(SLIDE_PAGE_TPDRAP_INTERVAL, SlidePageDrapTimerOut, NULL, TRUE);
                return;
            }
            if (entity->direction == WATCH_SLIDE_DIRECTION_LEFT
                && GetLastValidOne(entity) == entity->cur_page_index)
            {
                TRACE_APP_SLIDEPAGE("exit.");
                entity->tp_drap_timer = MMK_CreateTimerCallback(SLIDE_PAGE_TPDRAP_INTERVAL, SlidePageDrapTimerOut, NULL, TRUE);
                return;
            }
        }

        entity->target_page_index = entity->cur_page_index;
        entity->second_page_index = (entity->direction == WATCH_SLIDE_DIRECTION_RIGHT) ? entity->prev_page_index : entity->next_page_index;

        // modify win rect and fullpaint it
        do 
        {
            MMI_WIN_ID_T oper_win_id[2] = {0};
            uint8 oper_idx_id[2] = {0};
            uint8 loop = 0;

            oper_win_id[0] = GetWinIdByIndex(entity, entity->target_page_index);
            oper_win_id[1] = GetWinIdByIndex(entity, entity->second_page_index);
            oper_idx_id[0] = entity->target_page_index;
            oper_idx_id[1] = entity->second_page_index;

            if (s_enable_slide_buffer)
            {
                for (; loop < SLIDE_PAGE_NUM; loop++)
                {
                    UILAYER_SetLayerPosition(&s_slide_lcd[loop], -SLIDE_PAGE_WIDTH + loop * SLIDE_PAGE_WIDTH + move_offset, 0);
                }
            }
            else
            {
                for (; loop < 2; loop++)
                {
                    GUI_BOTH_RECT_T both_rect = {0};
                    GUI_RECT_T old_rect = GetWinPressedRectByIndex(entity, oper_idx_id[loop]);
                    old_rect.left += move_offset;
                    old_rect.right += move_offset;
                    both_rect.v_rect = old_rect;
                    MMK_SetWinRect(oper_win_id[loop], &old_rect);
                    MMK_SetBothRectNotifyCtrl(oper_win_id[loop], &both_rect);
                    MMK_SetWinHookFunc(oper_win_id[loop], NULL);
                    MMK_SendMsg(oper_win_id[loop], MSG_FULL_PAINT, NULL);
                    MMK_SetWinHookFunc(oper_win_id[loop], HandleSlidePageMsgHook);
                }
            }
        } while (0);
    }

    endtime = SCI_GetTickCount();
    TRACE_APP_SLIDEPAGE("lzk001. time elapsed = %d.", endtime - curtime);
    entity->tp_drap_timer = MMK_CreateTimerCallback(SLIDE_PAGE_TPDRAP_INTERVAL, SlidePageDrapTimerOut, NULL, TRUE);

}
#endif

// handle slidepage hook function
LOCAL MMI_RESULT_E HandleSlidePageMsgHook(MMI_WIN_ID_T win_id,
                                                                                      MMI_MESSAGE_ID_E msg_id,
                                                                                      DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    tWatchSlidePage *entity = GetActivedEntity();
    TRACE_APP_SLIDEPAGE("enter. entity = 0x%0x.", entity);

    TRACE_APP_SLIDEPAGE("begin. win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);

    if (entity == NULL)
    {
        if (msg_id != MSG_GET_FOCUS && msg_id != MSG_LOSE_FOCUS && msg_id != MSG_CLOSE_WINDOW)
        {
            return FALSE;
        }
    }

    switch (msg_id)
    {
        case MSG_SLIDEPAGE_OPENED:
        {
            MMK_SetWinHookFunc(win_id, NULL);
            MMK_SetFocusWin(MMK_ConvertIdToHandle(win_id));
            if (entity->fun_cb != NULL)
            {
                entity->fun_cb(win_id, MSG_SLIDEPAGE_OPENED, NULL);
                entity->fun_cb(win_id, MSG_SLIDEPAGE_GETFOCUS, NULL);
                entity->fun_cb(win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
            }

            MMK_SendMsg(win_id, MSG_GET_FOCUS, NULL);
            MMK_SetWinHookFunc(win_id, HandleSlidePageMsgHook);
            entity->focus_win_id = win_id;
            break;
        }

        case MSG_FULL_PAINT:
        {
            if (entity->is_draped || entity->is_auto_sliding)
            {
                return MMI_RESULT_TRUE;
            }
            else
            {
                return MMI_RESULT_FALSE;
            }

            break;
        }

        case MSG_CLOSE_WINDOW:
        {

            result = MMI_RESULT_FALSE;
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T tp_point = {0};
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            TRACE_APP_SLIDEPAGE("MSG_TP_PRESS_DOWN");
            if (entity->is_draped)
            {
                entity->is_draped = FALSE;
            }
            //if in autosliding. do nothing
            if (entity->is_auto_sliding)
            {
                result = MMI_RESULT_TRUE;
                break;
            }

            //save tp press point
            entity->tp_press_point = tp_point;
            entity->is_tp_pressed = TRUE;
            entity->is_tp_moved = FALSE;
            entity->is_draped = FALSE;
            entity->tp_last_point = tp_point;
            RecordTpPressedWinRect(entity);

            //stop auto sliding timer.
            StopAutoSlide(entity);

            //Start Drap Timer.
            TRACE_APP_SLIDEPAGE("MSG_TP_PRESS_DOWN tp_drap_timer :%d.", entity->tp_drap_timer);
            CloseAllTpDrapTimer();

            entity->tp_drap_timer = MMK_CreateTimerCallback(SLIDE_PAGE_TPDRAP_INTERVAL, SlidePageDrapTimerOut, NULL, TRUE);
            TRACE_APP_SLIDEPAGE("start tp_drap_timer :%d.", entity->tp_drap_timer);
            result = MMI_RESULT_FALSE;
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            if (entity->is_draped || entity->is_auto_sliding)
            {
                return MMI_RESULT_TRUE;
            }
            else
            {
                return MMI_RESULT_FALSE;
            }

            break;
        }

        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_point = {0};
            TRACE_APP_SLIDEPAGE("MSG_TP_PRESS_UP");
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            CloseAllTpDrapTimer();

            entity->tp_up_point = tp_point;
            if (entity->is_draped)
            {
                TRACE_APP_SLIDEPAGE("tp up. process drap.");
                result = MMI_RESULT_TRUE;
                entity->is_draped = FALSE;
                ProcessAutoSlideAfterTpUp(entity);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

            entity->is_tp_pressed = FALSE;
            entity->is_tp_moved = FALSE;

            break;
        }
#endif
        case MSG_KEYDOWN_RIGHT:
        {
            TRACE_APP_SLIDEPAGE("MSG_KEYDOWN_RIGHT");
            HandleKeyRightMsg(entity);
            break;
        }

        case MSG_KEYDOWN_LEFT:
        {
            TRACE_APP_SLIDEPAGE("MSG_KEYDOWN_LEFT");
            HandleKeyLeftMsg(entity);
            break;
        }

        case MSG_GET_FOCUS:
        {
            tWatchSlidePage *new_entity = FindSlideEntityByWinId(win_id);
            tWatchSlidePage *current_entity = GetActivedEntity();
            if (new_entity == NULL)
            {
                TRACE_APP_SLIDEPAGE("if (new_entity == NULL)");
                SCI_ASSERT(0);
            }

            if (new_entity != current_entity)
            {
                MMI_WIN_ID_T new_cur_win_id = GetWinIdByIndex(new_entity, new_entity->cur_page_index);
                if (new_entity->fun_cb != NULL)
                {
                    new_entity->fun_cb(new_cur_win_id, MSG_SLIDEPAGE_GETFOCUS, NULL);
                    new_entity->fun_cb(new_cur_win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
                }
                SetActivedEntity(new_entity);

                MMK_SetWinHookFunc(new_cur_win_id, NULL);
                MMK_SetFocusWin(MMK_ConvertIdToHandle(new_entity->focus_win_id));
                MMK_SendMsg(new_entity->focus_win_id, MSG_GET_FOCUS, NULL);
                MMK_SetWinHookFunc(new_cur_win_id, HandleSlidePageMsgHook);
                result = MMI_RESULT_TRUE;
            }
            else
            {
                MMK_SetFocusWin(MMK_ConvertIdToHandle(win_id));
                current_entity->focus_win_id = win_id;
            }

            result = MMI_RESULT_FALSE;
            break;
        }

        case MSG_LOSE_FOCUS:
        {
            MMI_WIN_ID_T foucs_win_id = MMK_GetFocusWinId();
            tWatchSlidePage *lose_entity = FindSlideEntityByWinId(win_id);
            tWatchSlidePage *focus_entity = FindSlideEntityByWinId(foucs_win_id);
            
            if (GetActivedEntity() == NULL)
            {
                return FALSE;
            }

            entity->is_draped = FALSE;
            CloseAllTpDrapTimer();
            if(lose_entity == GetActivedEntity() && focus_entity != lose_entity)
			{
                if (s_enable_slide_buffer)
                {
                    uint8 i = 0;
                    for (; i < SLIDE_PAGE_NUM; i++)
                    {
                        UILAYER_SetLayerPosition(&s_slide_lcd[i], -SLIDE_PAGE_WIDTH + i * SLIDE_PAGE_WIDTH, 0);
                        UILAYER_RemoveBltLayer(&s_slide_lcd[i]);
                    }
                }

                MMK_SetWinHookFunc(win_id, NULL);
                if (lose_entity->fun_cb != NULL)
                {
                    lose_entity->fun_cb(win_id, MSG_SLIDEPAGE_LOSEFOCUS, NULL);
                }

                if (lose_entity->auto_sliding_timer)
                {
                    MMK_StopTimer(lose_entity->auto_sliding_timer);
                    lose_entity->auto_sliding_timer = 0;
                    lose_entity->is_auto_sliding = FALSE;
                }
                SetActivedEntity(NULL);
                MMK_SetWinHookFunc(win_id, HandleSlidePageMsgHook);
			}

            result = MMI_RESULT_FALSE;
            break;
        }

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    TRACE_APP_SLIDEPAGE("end. win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);

    return result;
}

LOCAL void RecordTpPressedWinRect(tWatchSlidePage *slide_page)
{
    uint8 i = 0;
    MMI_WIN_ID_T oper_win_arr[SLIDE_PAGE_NUM] = {0};
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    oper_win_arr[0] = slide_page->prev_page_index;
    oper_win_arr[1] = slide_page->cur_page_index;
    oper_win_arr[2] = slide_page->next_page_index;

    for (; i < SLIDE_PAGE_NUM; i++)
    {
        uint8 page_idx = oper_win_arr[i];
        MMI_WIN_ID_T win_id = slide_page->pages[page_idx].win_id;
        if (MMK_IsOpenWin(win_id))
        {
            MMK_GetWinRect(win_id, &slide_page->pages[page_idx].win_rect_tp_pressed);
        }
    }
}

LOCAL BOOLEAN IsHorizontalMotion(GUI_POINT_T press_pos, GUI_POINT_T move_pos)
{
    return (abs(move_pos.x - press_pos.x) > 1) && (abs(move_pos.y - press_pos.y) < 20);
}

LOCAL MMI_WIN_ID_T GetWinIdByIndex(const tWatchSlidePage *slide_page, uint8 index)
{
    if (slide_page == NULL)
        return 0;

    return slide_page->pages[index].win_id;
}

LOCAL GUI_RECT_T GetWinPressedRectByIndex(const tWatchSlidePage *slide_page, uint8 index)
{
    GUI_RECT_T rect ={0};
    if (slide_page == NULL)
        return rect;

    return slide_page->pages[index].win_rect_tp_pressed;
}

LOCAL uint8 CalcNextPageIndex(tWatchSlidePage *slide_page, uint8 cur_index, uint8 page_cnt)
{
    uint8 i = 0;
    uint8 loop = 0;
    uint8 idx = 0;
    uint8 ret_idx = 0;
    uint8 *arr_enabled = SCI_ALLOC_APPZ(sizeof(uint8) * page_cnt);

    for (i = 0; i < page_cnt; i++)
    {
        if (!slide_page->pages[i].is_disabled)
        {
            arr_enabled[loop] = i;
            if (i == cur_index)
            {
                idx = loop;
            }
            loop++;
        }
    }

    ret_idx = arr_enabled[(idx + 1) % loop];
    if (arr_enabled != NULL)
        SCI_FREE(arr_enabled);

    return ret_idx;
}

LOCAL uint8 CalcPrevPageIndex(tWatchSlidePage *slide_page, uint8 cur_index, uint8 page_cnt)
{
    uint8 i = 0;
    uint8 loop = 0;
    uint8 idx = 0;
    uint8 ret_idx = 0;
    uint8 *arr_enabled = SCI_ALLOC_APPZ(sizeof(uint8) * page_cnt);

    for (i = 0; i < page_cnt; i++)
    {
        if (!slide_page->pages[i].is_disabled)
        {
            arr_enabled[loop] = i;
            if (i == cur_index)
            {
                idx = loop;
            }
            loop++;
        }
    }

    idx = idx == 0 ? (loop - 1) : idx - 1;
    ret_idx = arr_enabled[idx];
    if (arr_enabled != NULL)
        SCI_FREE(arr_enabled);

    return ret_idx;
}

LOCAL void StartAutoSlide(tWatchSlidePage *slide_page)
{
    MMI_WIN_ID_T win_id = 0;
    tWatchSlidePage *entity = slide_page;
    TRACE_APP_SLIDEPAGE("enter");
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }
    win_id = GetWinIdByIndex(slide_page, slide_page->target_page_index);
    StopAutoSlide(slide_page);

    if (!MMK_IsOpenWin(win_id))
    {
        TRACE_APP_SLIDEPAGE("fail");
        return;
    }

    slide_page->is_auto_sliding = TRUE;
    TRACE_APP_SLIDEPAGE("enter2");
    if (s_enable_slide_buffer)
    {
        int16 i = 0;
        uint16 j = 0;
#ifdef PLATFORM_UWS6121E
    #ifdef TOUCH_PANEL_SUPPORT
        int16 total_frame = 2;
    #else
        int16 total_frame = 12;
    #endif
#else
    #ifdef TOUCH_PANEL_SUPPORT
        int16 total_frame = 10;
    #else
        int16 total_frame = 16;
    #endif
#endif

        int16 remain = entity->total_vaule_to_slide;
        int16 offset_step = remain / (total_frame);
		int16 delta = remain % (total_frame);
        GUI_RECT_T dirty_rect = {0,0,SLIDE_PAGE_WIDTH,SLIDE_PAGE_HEIGHT};
        GUI_LCD_DEV_INFO main_lcd = {0};
        GUI_POINT_T old_layer_pos[3] = {0};

        if (abs(entity->total_vaule_to_slide) < 10)
        {
            ProcessAutoSlideFinished(entity);
            return;
        }

        for (j = 0; j < SLIDE_PAGE_NUM; j++)
        {
            UILAYER_GetLayerPosition(&s_slide_lcd[j], &old_layer_pos[j].x, &old_layer_pos[j].y);
        }

        if (slide_page->direction == WATCH_SLIDE_DIRECTION_LEFT)
        {
            for (i = 0; i >= remain; i+= offset_step)
            {
                for (j = 0; j < SLIDE_PAGE_NUM; j++)
                {
                    UILAYER_SetLayerPosition(&s_slide_lcd[j], old_layer_pos[j].x + i, old_layer_pos[j].y);
                }
                GUILCD_Invalidate(0, 0);
#ifdef WIN32
                SCI_Sleep(50);
#endif
            }

            //if (delta != 0)//bug2113342
            {
                UILAYER_SetLayerPosition(&s_slide_lcd[1], -SLIDE_PAGE_WIDTH, 0);
                UILAYER_SetLayerPosition(&s_slide_lcd[2], 0, 0);
                GUILCD_Invalidate(0, 0);
            }
        }
        else
        {
            for (i = 0; i <= remain; i+= offset_step)
            {
                for (j = 0; j < SLIDE_PAGE_NUM; j++)
                {
                    UILAYER_SetLayerPosition(&s_slide_lcd[j], old_layer_pos[j].x + i, old_layer_pos[j].y);
                }
                GUILCD_Invalidate(0, 0);
#ifdef WIN32
                SCI_Sleep(50);
#endif
            }

            //if (delta != 0)//bug2113342
            {
                UILAYER_SetLayerPosition(&s_slide_lcd[0], 0, 0);
                UILAYER_SetLayerPosition(&s_slide_lcd[1], SLIDE_PAGE_WIDTH, 0);
                GUILCD_Invalidate(0, 0);
            }

        }

        ProcessAutoSlideFinished(entity);
    }
    else
    {
        GUI_RECT_T rect_update = SLIDEPAGE_UPDATE_RECT;
        int32 step = 0;
        uint8 i = 0;
        uint32 start_time = SCI_GetTickCount();
        uint32 remain = abs(entity->total_vaule_to_slide);
        uint32 one_step = 0;
        uint32 cur_val = 0;
    #ifdef TOUCH_PANEL_SUPPORT
        int16  total_frame = 8;
    #else
        int16 total_frame = 16;
    #endif
        int32 last_step = 0;
        int32 offset_step = 1;
        
        MMI_WIN_ID_T sliding_win_id1 = GetWinIdByIndex(entity, entity->target_page_index);
        MMI_WIN_ID_T sliding_win_id2 = GetWinIdByIndex(entity, entity->second_page_index);

        TRACE_APP_SLIDEPAGE("total_vaule_to_slide = %d,step = %d.",  entity->total_vaule_to_slide, step);
        if (remain < 5)
        {
            ProcessAutoSlideFinished(entity);
            MMK_SetWinHookFunc(sliding_win_id1, NULL);
            MMK_SendMsg(sliding_win_id1, MSG_FULL_PAINT, NULL);
            MMK_SetWinHookFunc(sliding_win_id1, HandleSlidePageMsgHook);
            return;
        }

        for (i = 1; i < total_frame; i++)
        {
            //int32 offset_step = MMK_ClcltDampPlayOffset(play_type, entity->total_vaule_to_slide, total_frame, i);
            int32 offset_step =(int32) ((entity->total_vaule_to_slide * i ) / (total_frame));
            uint32 start_time2 = SCI_GetTickCount();
            step = offset_step - last_step;

            if (abs(step) <= 5)
            {
                TRACE_APP_SLIDEPAGE("not exceed auto slide thres, continue.");
                continue;
            }

            TRACE_APP_SLIDEPAGE("i = %d, offset_step = %d, step = %d", i, offset_step,step);
            ProcessAutoSlideTranslation(entity, sliding_win_id1, step);
            MMK_SetWinHookFunc(sliding_win_id1, NULL);
            MMK_SendMsg(sliding_win_id1, MSG_FULL_PAINT, NULL);
            MMK_SetWinHookFunc(sliding_win_id1, HandleSlidePageMsgHook);

            ProcessAutoSlideTranslation(entity, sliding_win_id2, step);
            MMK_SetWinHookFunc(sliding_win_id2, NULL);
            MMK_SendMsg(sliding_win_id2, MSG_FULL_PAINT, NULL);
            MMK_SetWinHookFunc(sliding_win_id2, HandleSlidePageMsgHook);

            GUILCD_Invalidate(0, 0);
            //SCI_Sleep(1);
            SCI_TRACE_LOW("Time Item Elasped: %d.",SCI_GetTickCount() - start_time2);
            last_step = offset_step;
        }

        ProcessAutoSlideFinished(entity);
        SCI_TRACE_LOW("Time Count Elasped: %d.",SCI_GetTickCount() - start_time);
    }

    StopAutoSlide(slide_page);

}

LOCAL void StopAutoSlide(tWatchSlidePage *slide_page)
{
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }
    if (slide_page->auto_sliding_timer != 0)
    {
        MMK_StopTimer(slide_page->auto_sliding_timer);
        slide_page->auto_sliding_timer = 0;
    }

    slide_page->is_auto_sliding = FALSE;
}

LOCAL BOOLEAN PrepareAutoSlideAfterTpUp(tWatchSlidePage *slide_page)
{
    //calc distance in horz
    int16 offset =slide_page->tp_up_point.x - slide_page->tp_press_point.x;
//    int16 slide_page_offset1 = 0; //cid 609638
//    int16 slide_page_offset2 = 0;
    uint8 slide_page_idx1 = 0;
    uint8 slide_page_idx2 = 0;
    GUI_RECT_T win_rect = {0};
    BOOLEAN is_need_slide = FALSE;
	int16 remain = 0;
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return FALSE;
    }

    slide_page_idx1 = slide_page->cur_page_index;

    //recalc direction
    slide_page->direction = offset > 0? WATCH_SLIDE_DIRECTION_RIGHT : WATCH_SLIDE_DIRECTION_LEFT;
    slide_page_idx2 = (slide_page->direction == WATCH_SLIDE_DIRECTION_RIGHT) ? slide_page->prev_page_index: slide_page->next_page_index;

    TRACE_APP_SLIDEPAGE("offset = %d.", offset);
    if (abs(offset) > SLIDE_CHANGE_PAGE_THR)
    {
        slide_page->target_page_index = slide_page_idx2;
        slide_page->second_page_index = slide_page_idx1;

        if (!slide_page->is_support_cycle)
        {
            //not support cycle. restore old index.
            if (slide_page->direction == WATCH_SLIDE_DIRECTION_RIGHT
                && GetFirstValidOne(slide_page) == slide_page->cur_page_index)
            {
                slide_page->target_page_index = slide_page_idx1;
                slide_page->second_page_index = slide_page_idx2;

                return is_need_slide;//bug id:2061113
            }
            if (slide_page->direction == WATCH_SLIDE_DIRECTION_LEFT
                && GetLastValidOne(slide_page) == slide_page->cur_page_index)
            {
                slide_page->target_page_index = slide_page_idx1;
                slide_page->second_page_index = slide_page_idx2;

                return is_need_slide;
            }
        }

        is_need_slide = TRUE;
    }
    else
    {
        slide_page->target_page_index = slide_page_idx1;
        slide_page->second_page_index = slide_page_idx2;
        is_need_slide = FALSE;
        return is_need_slide;
    }


    if (s_enable_slide_buffer)
    {
        slide_page->total_vaule_to_slide = slide_page->tp_up_point.x - slide_page->tp_press_point.x;
		
		if (slide_page->direction == WATCH_SLIDE_DIRECTION_LEFT)
		{
			remain = -(SLIDE_PAGE_WIDTH-abs(slide_page->total_vaule_to_slide));
		}
		else
		{
			remain = SLIDE_PAGE_WIDTH - slide_page->total_vaule_to_slide;
		}
		
		slide_page->total_vaule_to_slide = remain;
    }
    else
    {
        //record slide distance
        MMK_GetWinRect(GetWinIdByIndex(slide_page, slide_page->target_page_index), &win_rect);
        slide_page->total_vaule_to_slide = -win_rect.left;
    }

    slide_page->cur_value_in_slide = 0;
    return is_need_slide;
}

LOCAL void ProcessAutoSlideAfterTpUp(tWatchSlidePage *slide_page)
{
    BOOLEAN is_Slide = FALSE;
    is_Slide = PrepareAutoSlideAfterTpUp(slide_page);
    TRACE_APP_SLIDEPAGE("target id = %d, current id = %d.", slide_page->target_page_index, slide_page->cur_page_index);
    if (is_Slide)
    {
        StartAutoSlide(slide_page);
    }
    else
    {
        ProcessAutoSlideFinished(slide_page);
    }
}

LOCAL void ProcessAutoSlideFinished(tWatchSlidePage *slide_page)
{
    MMI_WIN_ID_T win_id = 0;
    int32 i=0;
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }
    SCI_TRACE_LOW("[SLIDEPAGE][SFL]ProcessAutoSlideFinished enter");
    slide_page->cur_page_index = slide_page->target_page_index;
    slide_page->prev_page_index = CalcPrevPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->next_page_index = CalcNextPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);

    //close all deactive win in entity
    //SendAllDeactivedWinInEntity(slide_page);
    if (s_enable_slide_buffer)
    {
        blt_restore();
    }
    s_funLoadSlidePageReady(slide_page, FALSE);
    MMK_SetFocusWin(MMK_ConvertIdToHandle(GetWinIdByIndex(slide_page, slide_page->cur_page_index)));
    SendAllDeactivedWinInEntity(slide_page);//get focus & lost focus

    //stop auto slide
    StopAutoSlide(slide_page);

    if (slide_page->fun_cb != NULL)
    {
        win_id = GetWinIdByIndex(slide_page, slide_page->cur_page_index);
        slide_page->fun_cb(win_id, MSG_FULL_PAINT, NULL);
        slide_page->fun_cb(win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
        slide_page->fun_cb(win_id, MSG_SLIDEPAGE_END, NULL);
    }

    slide_page->is_auto_sliding = FALSE;
}

LOCAL void ProcessAutoSlideTranslation(tWatchSlidePage *slide_page,
                                                                         MMI_WIN_ID_T win_id,
                                                                         int16 offset)
{
    GUI_RECT_T win_rect = {0};
    GUI_BOTH_RECT_T both_rect = {0};

    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    MMK_GetWinRect(win_id, &win_rect);
    #if 0
    if (abs(slide_page->cur_value_in_slide) < abs(offset))
    {
        win_rect.right += -slide_page->cur_value_in_slide;
        win_rect.left += -slide_page->cur_value_in_slide;
    }
    else
    #endif
    {
        win_rect.left += offset;
        win_rect.right += offset;
    }

    both_rect.v_rect = win_rect;
    MMK_SetWinRect(win_id, &win_rect);
    MMK_SetBothRectNotifyCtrl(win_id, &both_rect);
}

//depend the [cur] [prev] [next] page is correct values.
LOCAL void LoadSlidePageReady(const tWatchSlidePage* slide_page, BOOLEAN b_screenshoot)
{
    uint8 prev_idx = 0;
    uint8 cur_idx = 0;
    uint8 next_idx = 0;
    PFUN_WatchSlidePageItemWin_Enter pfun_enter = NULL;
    MMI_WIN_ID_T win_id = 0;
    GUI_BOTH_RECT_T both_rect[3] = {0};
    GUI_BOTH_RECT_T def_rect = {0,0,SLIDE_PAGE_WIDTH,SLIDE_PAGE_HEIGHT};
    PROCESSMSG_FUNC *fun_hook = HandleSlidePageMsgHook;
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    prev_idx = slide_page->prev_page_index;
    cur_idx = slide_page->cur_page_index;
    next_idx = slide_page->next_page_index;

#if 1
    //startup prev page win.
    pfun_enter = slide_page->pages[prev_idx].fun_enter_win;
    win_id = slide_page->pages[prev_idx].win_id;

    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
        }
        MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[0]);
    }
    else
    {
        both_rect[0].v_rect = SLIDE_PAGE_WIN_RECT[0];
        MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[0]);
        MMK_SetBothRectNotifyCtrl(win_id, &both_rect[0]);
    }

    MMK_SetWinHookFunc(win_id, fun_hook);

#endif
#if 1
    //startup next page win.
    pfun_enter = slide_page->pages[next_idx].fun_enter_win;
    win_id = slide_page->pages[next_idx].win_id;

    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
            MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[2]);
        }
    }
    else
    {
        both_rect[2].v_rect = SLIDE_PAGE_WIN_RECT[2];
        MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[2]);
        MMK_SetBothRectNotifyCtrl(win_id, &both_rect[2]);
    }

    MMK_SetWinHookFunc(win_id, fun_hook);
#endif

    //startup cur page win.
    pfun_enter = slide_page->pages[cur_idx].fun_enter_win;
    win_id = slide_page->pages[cur_idx].win_id;

    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
            MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[1]);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, NULL);
        }
    }
    else
    {
        both_rect[1].v_rect = SLIDE_PAGE_WIN_RECT[1];
        MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT[1]);
        MMK_SetBothRectNotifyCtrl(win_id, &both_rect[1]);

        MMK_SetWinHookFunc(win_id, NULL);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
        GUILCD_Invalidate(0, 0);
    }

    MMK_SetWinHookFunc(win_id, fun_hook);
}



/*****************************************************************************/
// Description : 设置焦点到某一个窗口
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GrabWindowFocus(MMI_WIN_ID_T win_id)
{
    SCI_TRACE_LOW("[SFL]GrabWindowFocus win_id:0x%x, is_focus:%d, is_open:%d", win_id, MMK_IsFocusWin(win_id), MMK_IsOpenWin(win_id));
    if ( MMK_IsFocusWin(win_id) || !MMK_IsOpenWin(win_id))
    {
        return FALSE;
    }

    MMK_WinGrabFocus(win_id);

    MMK_DeleteMSGByHwndAndMsg(win_id, MSG_FULL_PAINT);

    if (MMK_IsChildWin(win_id))
    {
        MMK_DeleteMSGByHwndAndMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT);
    }
    else if (MMK_GetAllChildWinHandle(win_id, PNULL, NULL) > 0)
    {
        MMK_DeleteMSGByHwndAndMsg(MMK_GetFocusChildWinId(), MSG_FULL_PAINT);
    }
//    MMITHEME_SetUpdateDelayCount(0);
//    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);

    return TRUE;
}

LOCAL void ScreenShot(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev)
{
    int32 i = 0; 
    int32 j = 0;
    GUI_RECT_T  rect = {0};
    uint16      logic_lcd_width = 0;
    uint16      logic_lcd_height = 0;
    UILAYER_APPEND_BLT_T blt_array[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32 blt_layer_count = 0;
    DISPLAY_BLENDLAYER_PARM_T*  blend_param_ptr = NULL;
    DISPLAY_BLENDLAYER_PARM_T   blend_param = {0};
    UILAYER_BLEND_T             ui_layer_blend = {0};
    DISPLAY_BLEND_LAYER_T*      src_layer[UILAYER_TOTAL_BLT_COUNT] = {0};
    BOOLEAN     bFlag = FALSE;
    GUI_LCD_DEV_INFO mainDev = {0, 0};
 
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&logic_lcd_width,&logic_lcd_height);
    rect.right  = (int16)(logic_lcd_width - 1);
    rect.bottom = (int16)(logic_lcd_height - 1);

    blt_layer_count = UILAYER_GetBltLayersOfHandle(win_id, blt_array, UILAYER_TOTAL_BLT_COUNT);
    SCI_TRACE_LOW("[SLIDEPAGE][SFL]ScreenShot: src blt array len:%d ", blt_layer_count);

    if(blt_layer_count > UILAYER_TOTAL_BLT_COUNT)//total blt layer is blt_layer_count + 1 ==> dest_layer
    {
        SCI_TRACE_LOW("[SLIDEPAGE][SFL]ScreenShot blt_layer_count got wrong value,this should not happen");
        blt_layer_count = UILAYER_TOTAL_BLT_COUNT;
    }
    //check whether main block exist or not
    i =0;
    while(i < blt_layer_count)
    {
        if(blt_array[i].lcd_dev_info.block_id == 0)
        {
            bFlag = 1;
            break;
        }
        i++;
    }

    //currently all windows use main block as background, but mainblock may not assign to win_id,
    //so it request to add main block to src layers to take complete screen shot.
    if(!bFlag)
    {
        i = (blt_layer_count >= UILAYER_TOTAL_BLT_COUNT)? UILAYER_TOTAL_BLT_COUNT-1 : blt_layer_count;
        //shift layer.
        while(i>0)
        {
            blt_array[i] = blt_array[i-1];
            i--;
        }
        blt_array[0].lcd_dev_info = mainDev;
        blt_array[0].layer_level = UILAYER_LEVEL_NORMAL;
        blt_layer_count +=1;
    }

    if(UILAYER_GetLayersBlendInfo(blt_array, blt_layer_count, dev, &rect, &ui_layer_blend))
    {
        //底层需要的是指针数组，在此适配一下
        for ( i = 0; i < ui_layer_blend.layer_num; i++ )
        {
            src_layer[i] = &ui_layer_blend.src_layer[i];
        }
        blend_param.dst_layer_ptr = &ui_layer_blend.dst_layer;
        blend_param.layer_arr = src_layer;
        blend_param.layer_num = ui_layer_blend.layer_num;
        blend_param.rect_ptr = &(ui_layer_blend.rect);
        blend_param.callback = ui_layer_blend.callback;
        blend_param.param = ui_layer_blend.param;
        blend_param_ptr = &blend_param;
        blend_param_ptr->dst_layer_ptr->layer_info.is_enable = TRUE;
        UILAYER_MultiBlendLayerEx(MAIN_LCD_ID, blend_param_ptr);
    }

}

LOCAL void LoadSlidePageReady_Multi(const tWatchSlidePage* slide_page, BOOLEAN b_screenshoot)
{
    uint8 i = 0;
    uint8 prev_idx = 0;
    uint8 cur_idx = 0;
    uint8 next_idx = 0;
    PFUN_WatchSlidePageItemWin_Enter pfun_enter = NULL;
    PROCESSMSG_FUNC *fun_hook = HandleSlidePageMsgHook;
    GUI_RECT_T def_rect = {0,0,SLIDE_PAGE_WIDTH,SLIDE_PAGE_HEIGHT};
    MMI_WIN_ID_T win_id = 0;

    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    SCI_TRACE_LOW("[SLIDEPAGE][SFL]LoadSlidePageReady_Multi Enter: prev_idx: %d, cur_idx: %d, next_idx: %d ", 
            slide_page->prev_page_index,
            slide_page->cur_page_index,
            slide_page->next_page_index);
    prev_idx = slide_page->prev_page_index;
    cur_idx = slide_page->cur_page_index;
    next_idx = slide_page->next_page_index;

    //startup prev page win.
    pfun_enter = slide_page->pages[prev_idx].fun_enter_win;
    win_id = slide_page->pages[prev_idx].win_id;
    SCI_TRACE_LOW("[SFL]LoadSlidePageReady_Multi: prev win_id: 0x%x, isOpen[%d]", 
            win_id,
            MMK_IsOpenWin(win_id));
    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
        }
    }

    if(b_screenshoot)
    {
        MMK_SetWinHookFunc(win_id, NULL);
        UILAYER_SetLayerPosition(&s_slide_lcd[0], 0, 0);
        GrabWindowFocus(win_id);
        //MMITHEME_SetUpdateDelayCount(0);
        MMK_UpdateScreen();//fullpaint
        ScreenShot(win_id, &s_slide_lcd[0]);
    }

    //. 2
    pfun_enter = slide_page->pages[next_idx].fun_enter_win;
    win_id = slide_page->pages[next_idx].win_id;
    SCI_TRACE_LOW("[SFL]LoadSlidePageReady_Multi: next win_id: 0x%x, isOpen[%d]", 
            win_id,
            MMK_IsOpenWin(win_id));
    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
        }
    }

    if(b_screenshoot)
    {
        MMK_SetWinHookFunc(win_id, NULL);
        UILAYER_SetLayerPosition(&s_slide_lcd[2], 0, 0);
        GrabWindowFocus(win_id);
    //    MMITHEME_SetUpdateDelayCount(0);
        MMK_UpdateScreen();//fullpaint
        ScreenShot(win_id, &s_slide_lcd[2]);
    }

    //. 3
    pfun_enter = slide_page->pages[cur_idx].fun_enter_win;
    win_id = slide_page->pages[cur_idx].win_id;
    SCI_TRACE_LOW("[SFL]LoadSlidePageReady_Multi: cur win_id: 0x%x, isOpen[%d]", 
            win_id,
            MMK_IsOpenWin(win_id));
    if (!MMK_IsOpenWin(win_id))
    {
        if (pfun_enter != NULL)
        {
            pfun_enter();
        }
    }

    MMK_SetWinHookFunc(win_id, NULL);
    UILAYER_SetLayerPosition(&s_slide_lcd[1], 0, 0);
    GrabWindowFocus(win_id);
//    MMITHEME_SetUpdateDelayCount(0);
    MMK_UpdateScreen();//fullpaint

    if(b_screenshoot) // if b_screenshoot is false, still grab focus and update screen incase next win run open_window .
    {
        ScreenShot(win_id, &s_slide_lcd[1]);
    }

    for (; i < SLIDE_PAGE_NUM; i++)
    {
        UILAYER_SetLayerPosition(&s_slide_lcd[i], -SLIDE_PAGE_WIDTH + i * SLIDE_PAGE_WIDTH, 0);
        UILAYER_RemoveBltLayer(&s_slide_lcd[i]);
    }

    do
    {
        GUI_LCD_DEV_INFO main_lcd = {0};
//        MMITHEME_SetUpdateDelayCount(0);
        MMITHEME_StoreUpdateRect(&main_lcd,def_rect);
    }while (0);

    MMK_SetWinHookFunc(slide_page->pages[prev_idx].win_id, fun_hook);
    MMK_SetWinHookFunc(slide_page->pages[next_idx].win_id, fun_hook);
    MMK_SetWinHookFunc(slide_page->pages[cur_idx].win_id, fun_hook);

    SCI_TRACE_LOW("[SLIDEPAGE][SFL]LoadSlidePageReady_Multi Exit.");
}

LOCAL uint8 FindSlideIndexByWinId(MMI_WIN_ID_T win_id)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 find_id = 0;
    BOOLEAN b_find = FALSE;
    for (i = 0; i < SLIDE_PAGE_ENTITY_MAX; i++)
    {
        for (j = 0; j <SLIDE_PAGE_MAXNUM; j++)
        {
            if (s_watchslidepage_entity[i].pages[j].win_id == win_id)
            {
                b_find = TRUE;
                break;
            }
        }

        if (b_find)
        {
            find_id = i;
            return find_id;
        }
    }

    return SLIDE_PAGE_ENTITY_MAX;
}

LOCAL tWatchSlidePage *FindSlideEntityByWinId(MMI_WIN_ID_T win_id)
{
    uint8 index = FindSlideIndexByWinId(win_id);
    if (index == SLIDE_PAGE_ENTITY_MAX)
    {
        TRACE_APP_SLIDEPAGE("index == SLIDE_PAGE_ENTITY_MAX");
        return NULL;
    }

    return &s_watchslidepage_entity[index];
}

#if 0  //cid 609212
LOCAL void CloseAllDeactivedWinInEntity(tWatchSlidePage *slide_page)
{
    uint32 j = 0;
    MMI_WIN_ID_T cur_id = 0;
    MMI_WIN_ID_T prev_id = 0;
    MMI_WIN_ID_T next_id = 0;

    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    if (slide_page != s_actived_entity)
    {
        TRACE_APP_SLIDEPAGE("slide_page != s_actived_entity");
        return;
    }

    for (; j <SLIDE_PAGE_MAXNUM; j++)
    {
        MMI_WIN_ID_T win_id = slide_page->pages[j].win_id;

        if (j != slide_page->cur_page_index
            && j != slide_page->prev_page_index
            && j != slide_page->next_page_index)
        {
            if (MMK_IsOpenWin(win_id))
            {
                MMK_CloseWin(win_id);
            }
        }
    }
}
#endif

LOCAL void SendAllDeactivedWinInEntity(tWatchSlidePage *slide_page)
{
    uint32 j = 0;
    MMI_WIN_ID_T cur_id = 0;
    MMI_WIN_ID_T prev_id = 0;
    MMI_WIN_ID_T next_id = 0;

    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }

    if (slide_page != s_actived_entity)
    {
        TRACE_APP_SLIDEPAGE("slide_page != s_actived_entity");
        return;
    }

    for (; j <SLIDE_PAGE_MAXNUM; j++)
    {
        MMI_WIN_ID_T win_id = slide_page->pages[j].win_id;

        if (j != slide_page->cur_page_index)
        {
            if (MMK_IsOpenWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_LOSE_FOCUS, NULL);
            }
        }
        else
        {
            if (MMK_IsOpenWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_GET_FOCUS, NULL);
            }
        }
    }
}

LOCAL void HandleKeyRightMsg(tWatchSlidePage *slide_page)
{
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }
    if (slide_page->is_draped || slide_page->is_auto_sliding)
    {
        TRACE_APP_SLIDEPAGE("slide_page->is_draped = %d.", slide_page->is_draped);
        TRACE_APP_SLIDEPAGE("slide_page->is_auto_sliding = %d.", slide_page->is_auto_sliding);
        return;
    }

    if (!slide_page->is_support_cycle)
    if (GetLastValidOne(slide_page) == slide_page->cur_page_index)
    {
        TRACE_APP_SLIDEPAGE("same page.");
        return;
    }

    slide_page->target_page_index = CalcNextPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->second_page_index= slide_page->cur_page_index;

    slide_page->total_vaule_to_slide = -SLIDE_PAGE_WIDTH;
    slide_page->cur_value_in_slide = 0;
    
    s_funLoadSlidePageReady(slide_page, TRUE);

    if (s_enable_slide_buffer)
    {
        uint8 loop = 0;
        blt_backup();
        slide_page->direction = WATCH_SLIDE_DIRECTION_LEFT;
        for (loop = 0; loop < SLIDE_PAGE_NUM; loop++)
        {
            UILAYER_APPEND_BLT_T append_info = {0};
            append_info.layer_level = UILAYER_LEVEL_NORMAL;
            append_info.lcd_dev_info = s_slide_lcd[loop];
            UILAYER_AppendBltLayer(&append_info);
        }
    }
    StartAutoSlide(slide_page);
}

LOCAL void HandleKeyLeftMsg(tWatchSlidePage *slide_page)
{
    if (slide_page == NULL)
    {
        TRACE_APP_SLIDEPAGE("slide_page == NULL");
        return;
    }
    if (slide_page->is_draped || slide_page->is_auto_sliding)
    {
        TRACE_APP_SLIDEPAGE("slide_page->is_draped = %d.", slide_page->is_draped);
        TRACE_APP_SLIDEPAGE("slide_page->is_auto_sliding = %d.", slide_page->is_auto_sliding);
        return;
    }
    if (!slide_page->is_support_cycle)
    if (GetFirstValidOne(slide_page) == slide_page->cur_page_index)
    {
        return;
    }
    slide_page->target_page_index = CalcPrevPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->second_page_index= slide_page->cur_page_index;

    slide_page->total_vaule_to_slide = SLIDE_PAGE_WIDTH;
    slide_page->cur_value_in_slide = 0;
    
    s_funLoadSlidePageReady(slide_page, TRUE);

    if (s_enable_slide_buffer)
    {
        uint8 loop = 0;
        blt_backup();
        slide_page->direction = WATCH_SLIDE_DIRECTION_RIGHT;
        for (loop = 0; loop < SLIDE_PAGE_NUM; loop++)
        {
            UILAYER_APPEND_BLT_T append_info = {0};
            append_info.layer_level = UILAYER_LEVEL_NORMAL;
            append_info.lcd_dev_info = s_slide_lcd[loop];
            UILAYER_AppendBltLayer(&append_info);
        }
    }
    StartAutoSlide(slide_page);
}

LOCAL void HandleKeyLeftMsgDirect(tWatchSlidePage *slide_page)
{
    slide_page->cur_page_index= CalcPrevPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->prev_page_index= CalcPrevPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->next_page_index= CalcNextPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);


    s_funLoadSlidePageReady(slide_page, TRUE);
    if (slide_page->fun_cb != NULL)
    {
        MMI_WIN_ID_T win_id = GetWinIdByIndex(slide_page, slide_page->cur_page_index);
        slide_page->fun_cb(win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
    }
    MMK_SendMsg(slide_page->pages[slide_page->cur_page_index].win_id, MSG_GET_FOCUS, NULL);
    MMK_SetFocusWin(MMK_ConvertIdToHandle(GetWinIdByIndex(slide_page, slide_page->cur_page_index)));
}


LOCAL void HandleKeyRightMsgDirect(tWatchSlidePage *slide_page)
{
    slide_page->cur_page_index= CalcNextPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->prev_page_index= CalcPrevPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);
    slide_page->next_page_index= CalcNextPageIndex(slide_page, slide_page->cur_page_index, slide_page->page_cnt);

    s_funLoadSlidePageReady(slide_page, TRUE);
    if (slide_page->fun_cb != NULL)
    {
        MMI_WIN_ID_T win_id = GetWinIdByIndex(slide_page, slide_page->cur_page_index);
        slide_page->fun_cb(win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
    }
    MMK_SendMsg(slide_page->pages[slide_page->cur_page_index].win_id, MSG_GET_FOCUS, NULL);
    MMK_SetFocusWin(MMK_ConvertIdToHandle(GetWinIdByIndex(slide_page, slide_page->cur_page_index)));
}
LOCAL uint8 GetFirstValidOne(tWatchSlidePage *slide_page)
{
    uint8 i = 0;
    for (i = 0; i < slide_page->page_cnt; i++)
    {
        if (!slide_page->pages[i].is_disabled)
        {
            return i;
        }
    }
    return i;
}
LOCAL uint8 GetLastValidOne(tWatchSlidePage *slide_page)
{
    uint8 i = 0;
    for (i = slide_page->page_cnt - 1; i > 0; i--)
    {
        if (!slide_page->pages[i].is_disabled)
        {
            return i;
        }
    }
    return i;
}

/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/

PUBLIC void WatchSLIDEPAGE_SetSlideBufferEnable(BOOLEAN bEnableSlideBuffer)
{
    s_enable_slide_buffer = bEnableSlideBuffer;

    if (s_enable_slide_buffer)
    {
        s_funLoadSlidePageReady = LoadSlidePageReady_Multi;
    }
    else
    {
        s_funLoadSlidePageReady = LoadSlidePageReady;
    }
}

/*****************************************************************************
//  Description : 从系统中申请创建一个SlidePage 对象
//  Author: zhikun.lv
//  Param:N/A
//  Return: 返回申请到的Handle句柄，非0为有效值
//  Note:
*****************************************************************************/
PUBLIC MMI_HANDLE_T WatchSLIDEPAGE_CreateHandle(void)
{
    uint32 i = 0;
    TRACE_APP_SLIDEPAGE_ENTER;
    for (; i < SLIDE_PAGE_ENTITY_MAX; i++)
    {
        if (!s_watchslidepage_entity[i].is_used)
        {
            break;
        }
    }

    if (s_enable_slide_buffer)
    {
        uint8 loop = 0;
        if (!s_slide_lcd_ready)
        {
            UILAYER_CREATE_T create_info = {0};
            create_info.is_bg_layer = TRUE;
            create_info.is_static_layer = TRUE;
            create_info.owner_handle = 0;
            create_info.width = SLIDE_PAGE_WIDTH;
            create_info.height = SLIDE_PAGE_HEIGHT;
            for (loop = 0; loop < SLIDE_PAGE_NUM; loop++)
            {
                UILAYER_CreateLayer(&create_info, &s_slide_lcd[loop]);
                UILAYER_FillColor(&s_slide_lcd[loop], 0X821);
            }
            s_slide_lcd_ready = TRUE;
        }
    }

    if (i == SLIDE_PAGE_ENTITY_MAX)
    {
        return 0;
    }
    else
    {
        return i + SLIDE_PAGE_HANDLE_MAGIC;
    }
    TRACE_APP_SLIDEPAGE_EXIT;
}


/*****************************************************************************
//  Description : 释放SlidePage 对象.可能不需要显示的调用，
//                     因为当SlidePage的某个页面关闭时，会自动关闭SlidePage
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEPAGE_DestoryHandle(MMI_HANDLE_T handle)
{
    uint32 i = 0;
    if (handle == 0)
    {
        return FALSE;
    }

    if (handle >= SLIDE_PAGE_HANDLE_MAGIC + SLIDE_PAGE_ENTITY_MAX)
    {
        return FALSE;
    }

    i = handle - SLIDE_PAGE_HANDLE_MAGIC;

    if (&s_watchslidepage_entity[i] == s_actived_entity)
    {
        s_actived_entity = NULL;
    }
    if (s_watchslidepage_entity[i].is_used)
    {
        WatchSLIDEAGE_Close(handle);
        SCI_MEMSET(&s_watchslidepage_entity[i], 0 , sizeof(tWatchSlidePage));
    }

    return TRUE;
}


/*****************************************************************************
//  Description : 启动SlidePage
//  Author: zhikun.lv
//  Param:
      handle ---handle of slidepage
      page_arr --- slidepageItem info array
      page_cnt --- count of slidepageItem
      focus_index --- the default index of window to garb focus
      is_support_cycle --- is support cycle tranform in slidepage
      fun_cb --- reserved
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEPAGE_Open(MMI_HANDLE_T handle, 
                                                                            tWatchSlidePageItem *page_arr,
                                                                            uint8 page_cnt,
                                                                            uint8 focus_index, 
                                                                            BOOLEAN is_support_cycle,
                                                                            PROCESSMSG_FUNC fun_cb)
{
    uint32 i = 0;
    tWatchSlidePage *slide_page = NULL;
    TRACE_APP_SLIDEPAGE_ENTER;
    if (handle == 0
        || page_arr == NULL
        || page_cnt == 0)
    {
        TRACE_APP_SLIDEPAGE("error. param not valid.");
        return FALSE;
    }

    i = handle - SLIDE_PAGE_HANDLE_MAGIC;

    if (s_watchslidepage_entity[i].is_used)
    {
        TRACE_APP_SLIDEPAGE("!s_watchslidepage_entity[i].is_used");
        return FALSE;
    }

    slide_page = &s_watchslidepage_entity[i];

    //setup slide item data;
    for (i = 0; i < page_cnt; i++)
    {
        slide_page->pages[i] = page_arr[i];
    }

    slide_page->page_cnt = page_cnt;
    slide_page->page_width = SLIDE_PAGE_WIDTH;

    slide_page->cur_page_index = focus_index;
    slide_page->prev_page_index = CalcPrevPageIndex(slide_page, focus_index, page_cnt);
    slide_page->next_page_index = CalcNextPageIndex(slide_page, focus_index, page_cnt);
    slide_page->is_used = TRUE;
    slide_page->fun_cb = fun_cb;
    slide_page->is_support_cycle = is_support_cycle;

    SetActivedEntity(slide_page);

    //MMK_PostMsg(GetWinIdByIndex(slide_page, focus_index), MSG_GET_FOCUS, NULL, NULL);
    MMK_SendMsg(GetWinIdByIndex(slide_page, focus_index), MSG_SLIDEPAGE_OPENED, NULL);
    TRACE_APP_SLIDEPAGE_EXIT;
    return TRUE;
}


/*****************************************************************************
//  Description : 关闭SlidePage 对象.目前不需要显示的调用，
//                     因为当SlidePage的某个页面关闭时，会自动关闭SlidePage
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEAGE_Close(MMI_HANDLE_T handle)
{
    uint32 i = 0;
    uint32 loop = 0;
    i = handle - SLIDE_PAGE_HANDLE_MAGIC;

    if (!s_watchslidepage_entity[i].is_used)
    {
        TRACE_APP_SLIDEPAGE("!s_watchslidepage_entity[i].is_used");
        return FALSE;
    }

    for (; loop < SLIDE_PAGE_MAXNUM; loop++)
    {
        MMI_WIN_ID_T win_id = s_watchslidepage_entity[i].pages[loop].win_id;
        if (MMK_IsOpenWin(win_id))
        {
            MMK_SetWinHookFunc(win_id, NULL);
            MMK_CloseWin(win_id);
        }
    }
    return TRUE;
}


/*****************************************************************************
//  Description : Get SlidePage Infomation
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC tWatchSlidePage* WatchSLIDEPAGE_GetSlideInfoByHandle(MMI_HANDLE_T handle)
{
    uint32 i = 0;
    uint32 loop = 0;
    i = handle - SLIDE_PAGE_HANDLE_MAGIC;
    TRACE_APP_SLIDEPAGE_ENTER;
    if (!s_watchslidepage_entity[i].is_used)
    {
        TRACE_APP_SLIDEPAGE("!s_watchslidepage_entity[i].is_used");
        return NULL;
    }

    TRACE_APP_SLIDEPAGE_EXIT;
    return &s_watchslidepage_entity[i];
}


/*****************************************************************************
//  Description : set current page index of slide entity
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchSLIDEAGE_SetCurrentPageIndex(uint8 currentIndex)
{
    tWatchSlidePage *entity = GetActivedEntity();
    TRACE_APP_SLIDEPAGE_ENTER;
    if (entity == NULL)
    {
        TRACE_APP_SLIDEPAGE("entity == NULL");
        return;
    }

    entity->cur_page_index = currentIndex;
    entity->prev_page_index = CalcPrevPageIndex(entity, currentIndex, entity->page_cnt);
    entity->next_page_index= CalcNextPageIndex(entity, currentIndex, entity->page_cnt);
    s_funLoadSlidePageReady(entity, FALSE);
    if (entity->fun_cb != NULL)
    {
        MMI_WIN_ID_T win_id = GetWinIdByIndex(entity, entity->cur_page_index);
        entity->fun_cb(win_id, MSG_SLIDEPAGE_PAGECHANGED, NULL);
    }
    MMK_SendMsg(entity->pages[currentIndex].win_id, MSG_GET_FOCUS, NULL);
    MMK_SetFocusWin(MMK_ConvertIdToHandle(GetWinIdByIndex(entity, entity->cur_page_index)));
    TRACE_APP_SLIDEPAGE_EXIT;
}


/*****************************************************************************
//  Description : set page enable by winid.
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchSLIDEAGE_SetPageEnable(MMI_WIN_ID_T win_id, BOOLEAN bEnable)
{
    uint8 i = 0;
    tWatchSlidePage *entity = NULL;
    uint8 idx = FindSlideIndexByWinId(win_id);
    TRACE_APP_SLIDEPAGE("enter win_id:%x  enable:%d", win_id, bEnable );

    if (idx == SLIDE_PAGE_ENTITY_MAX)
    {
        //do nothing.
        TRACE_APP_SLIDEPAGE("win_id is not valid");
        return;
    }

    entity = FindSlideEntityByWinId(win_id);

    //entity != NULL ,代表在slidepage array中找到了这个win
    if (entity != NULL)
    {
        for (i = 0; i < entity->page_cnt; i++)
        {
            if (entity->pages[i].win_id == win_id)
            {
                entity->pages[i].is_disabled = !bEnable;
                break;
            }
        }

        entity->prev_page_index = CalcPrevPageIndex(entity, entity->cur_page_index, entity->page_cnt);
        entity->next_page_index = CalcNextPageIndex(entity, entity->cur_page_index, entity->page_cnt);
        //if (entity != NULL/*entity == GetActivedEntity()*/)
        {
            for (i = 0; i < entity->page_cnt; i++)
            {
                if (entity->pages[i].win_id == win_id)
                {
                    entity->pages[i].is_disabled = !bEnable;
                    if (!s_enable_slide_buffer)
                        MMK_SetWinRect(win_id, &SLIDE_PAGE_WIN_RECT_INVALID);
                    //else
                    //    MMK_SetWinLcdDevInfo(win_id,&s_slide_lcd[0]);//use block 0
                }
            }

            if (!bEnable && entity->pages[entity->cur_page_index].win_id == win_id)
            {
                TRACE_APP_SLIDEPAGE("entity->cur_page_index = %d.", entity->cur_page_index);
                entity->cur_page_index += 1;
                entity->prev_page_index = CalcPrevPageIndex(entity, entity->cur_page_index, entity->page_cnt);
                entity->next_page_index = CalcNextPageIndex(entity, entity->cur_page_index, entity->page_cnt);
            }

            if (GetActivedEntity() != NULL && entity == GetActivedEntity())
            {
                //close all deactive win in entity
                MMK_SetFocusWin(MMK_ConvertIdToHandle(GetWinIdByIndex(entity, entity->cur_page_index)));
                SendAllDeactivedWinInEntity(entity);

                s_funLoadSlidePageReady(entity, FALSE);

                if (entity->fun_cb != NULL)
                {
                    MMI_WIN_ID_T win_id2 = GetWinIdByIndex(entity, entity->cur_page_index);
                    entity->fun_cb(win_id2, MSG_SLIDEPAGE_PAGECHANGED, NULL);
                }
            }

        }
    }

    TRACE_APP_SLIDEPAGE("exit");
}
