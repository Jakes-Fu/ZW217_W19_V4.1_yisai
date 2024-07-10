/*****************************************************************************
** File Name:      mmigrid_idle.c                                            *
** Author:                                                                   *
** Date:           06/2011                                                   *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe widget control              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME               DESCRIPTION                             *
** 23/06/2011     hongbo.lan     Create
******************************************************************************/

#ifdef PDA_UI_SUPPORT_U_IDLE

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmiset_internal.h"
#include "mmiset_export.h"

#include "mmiwidget.h"
#include "mmitheme_widget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiidle_export.h"
#include "mmiwidget_item.h"

#include "mmiwidget_page.h"

#include "mmi_mainmenu_export.h"
#include "mmigrid_uidle_se.h"
#include "u_launcher_internal.h"
#include "u_launcher_export.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Constant Variables                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         EXTERNAL DECLARE                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//u idle item info.
LOCAL GO_MAINMENU_ICON_INFO_T s_prev_item_info[MMI_IDLE_GRID_PAGE_CELL_NUM] = {0}; /*lint !e728 !e551*/
LOCAL GO_MAINMENU_ICON_INFO_T s_cur_item_info[MMI_IDLE_GRID_PAGE_CELL_NUM]  = {0};
LOCAL GO_MAINMENU_ICON_INFO_T s_next_item_info[MMI_IDLE_GRID_PAGE_CELL_NUM] = {0};

//u idle page info.
LOCAL MMIGRID_U_PAGE_INFO_T s_prev_page_info = {0};
LOCAL MMIGRID_U_PAGE_INFO_T s_cur_page_info  = {0};
LOCAL MMIGRID_U_PAGE_INFO_T s_next_page_info = {0};

LOCAL GO_MAINMENU_PAGE_INFO_T *s_cur_page_info_ptr  = PNULL;
LOCAL GO_MAINMENU_PAGE_INFO_T *s_next_page_info_ptr = PNULL;
LOCAL FIX16_POINT_T           *s_calc_point_buf     = PNULL;
LOCAL S3D_EXPANDPTR_T*        *s_u_calc_param_ptr   = PNULL;  /*lint !e728*/
LOCAL uint8                   *s_u_shining_ctrl_ptr = PNULL;
LOCAL uint32                   s_cur_random_style   = 0;
//**************************************************************************//
//********************* U IDLE EFFECT LOCAL API DECLARE ********************//
//**************************************************************************//

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateUIdlePMALayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : cache all widgets and menu icons on prev,cur,next pages.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CacheItemsOn3Pages(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : free cache icons and some widgets.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void FreeCacheItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : init grid u idle environment.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitGridUIdle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : cache all widgets and menu icons on prev,cur,next pages.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CacheItemsOnSpecificPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIGRID_U_PAGE_INFO_T *u_page_ptr, int32 page_idx);

/*****************************************************************************/
//  Description : get ramdom uidle effect style.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void GetUIdleRandomStyle(void);

/*****************************************************************************/
//  Description : u idle effect paint.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawGridUIdleEffect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                uint16 first_page,
                                uint16 second_page,
                                GO_MOVE_DIRECTION_E direction,
                                uint16 current_frame
                               );

//**************************************************************************//
//********************* U IDLE EFFECT CODES BEGIN **************************//
//**************************************************************************//

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateUIdlePMALayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&idle_grid_ptr->page_layer,TRUE,FALSE);        
    }
}

/*****************************************************************************/
//  Description : init grid u idle environment.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitGridUIdle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    BOOLEAN ret = TRUE;

    CreateUIdlePMALayer(idle_grid_ptr);

    BLKMEM_GarbageClean();

    if (PNULL == s_calc_point_buf)
    {
        s_calc_point_buf      = (FIX16_POINT_T*)SCI_ALLOCA(1024*sizeof(FIX16_POINT_T));

        if (PNULL == s_calc_point_buf)
        {
            ret = FALSE;
        }
    }

    if (ret && PNULL == s_u_shining_ctrl_ptr)
    {
        s_u_shining_ctrl_ptr = (uint8*)SCI_ALLOCA(SHINING_MAX_RADIUS+1);
    
        if (PNULL == s_u_shining_ctrl_ptr)
        {
            ret = FALSE;
        }
    }

    if (ret && PNULL == s_u_calc_param_ptr)
    {
        s_u_calc_param_ptr   = (S3D_EXPANDPTR_T*)SCI_ALLOCA(sizeof(S3D_EXPANDPTR_T)); 

        if (PNULL == s_u_calc_param_ptr)
        {
            ret = FALSE;
        }
    }

    if (ret)
    {
        ret = CacheItemsOn3Pages(idle_grid_ptr);
    }

    if (!ret)
    {
        MMIUIDLE_FreeUIdleAction(idle_grid_ptr);
        return ret;
    }

    GetUIdleRandomStyle();

    return ret;
}

/*****************************************************************************/
//  Description : get ramdom uidle effect style.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void GetUIdleRandomStyle(void)
{
	if(UIDLE_STYLE_RANDOM == (UIDLE_STYLE_E)MMIAPISET_GetUIdleStyle())
    {
        do
        {
            s_cur_random_style = rand() % UIDLE_STYLE_MAX;
        }
        while((UIDLE_STYLE_RANDOM == s_cur_random_style) || (UIDLE_STYLE_CLASSIC == s_cur_random_style));
    }
}

/*****************************************************************************/
//  Description : u idle effect paint.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawGridUIdleEffect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                uint16 first_page,
                                uint16 second_page,
                                GO_MOVE_DIRECTION_E direction,
                                uint16 current_frame
                               )
{     
    IMGREF_SIZE_T bg_window_size = {0, 0};
    GUI_RECT_T    rect = {0};
    UIDLE_STYLE_E cur_style      = (UIDLE_STYLE_E)MMIAPISET_GetUIdleStyle();
    uint32 cost_time = 0;
    void* bg_window_buf = PNULL;
    go_calc_func calc_func = PNULL;
    GO_CALC_DATA_INFO_T calc_data_info = {0};
    static uint16 pre_frame = 0;

	//如果祯数一样则不进行重画
	if((pre_frame == current_frame)
		&&(current_frame!=0)
		&&(current_frame!=GO_MAINMENU_TOTAL_FRAME_MAX)
		)
	{
		return;
	}

    if (PNULL == s_u_calc_param_ptr || PNULL == s_u_shining_ctrl_ptr)
    {
        return;
    }
    
    rect = UILAYER_GetLayerRect(&idle_grid_ptr->page_layer);

    bg_window_size.w = rect.right - rect.left + 1;
    bg_window_size.h = rect.bottom - rect.top + 1;

	SCI_TRACE_LOW("[GO_MENU]MMIMENU_DrawGOMenuLayer current time  = %d ", SCI_GetTickCount());   

    if (first_page > second_page)
    {
        s_cur_page_info_ptr  = &s_cur_page_info.page_info;
        s_next_page_info_ptr = &s_prev_page_info.page_info;        
    }
    else
    {
        s_cur_page_info_ptr  = &s_cur_page_info.page_info;
        s_next_page_info_ptr = &s_next_page_info.page_info;            
    }
      
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_PMARGB888); 
        calc_data_info.param = s_u_calc_param_ptr;/*lint !e64*/
        calc_data_info.param->shiningCtrl_ptr = s_u_shining_ctrl_ptr;

        if((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            UILAYER_SwitchDoubleMem(&idle_grid_ptr->page_layer, FALSE );
        }
        else
        {
            UILAYER_SetDirectDraw( TRUE );
            UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            UILAYER_SwitchDoubleMem(&idle_grid_ptr->page_layer, TRUE);
        }
        
        UILAYER_Clear(&idle_grid_ptr->page_layer);

        //算法输出BUF
        if(PNULL == (bg_window_buf = (uint32*)UILAYER_GetLayerBufferPtr(&idle_grid_ptr->page_layer)))
        {
            return;
        }  
    }
    
	cost_time = SCI_GetTickCount();
   
    if(UIDLE_STYLE_RANDOM == cur_style)
    {
        calc_func = UAPIIdle_GetCallBackfunc((UIDLE_STYLE_E)s_cur_random_style);
    }
    else
    {
        calc_func = UAPIIdle_GetCallBackfunc(cur_style);
    }        
    
    if(PNULL == calc_func)
    {
        return;
    }
    
    calc_func(  s_cur_page_info_ptr,
                s_next_page_info_ptr,
                PNULL,
                current_frame,
                GO_MAINMENU_TOTAL_FRAME_MAX,
                bg_window_size,
                bg_window_buf,
                direction,
                s_calc_point_buf, 
                &calc_data_info
                );        

    cost_time = SCI_GetTickCount() - cost_time;  
    SCI_TRACE_LOW("[GO_MENU]MMIMENU_GO_Effect_calc calcfunc cost time = %d         STYLE = %d      frame = %d", cost_time,cur_style,current_frame);   
    pre_frame = current_frame;

    //MMIMENU_Go_Reset_Magnification_Frame(&current_frame);
}

/*****************************************************************************/
//  Description: handle page u idle tp move.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUIdle_HandleUIdleSlideTpMov(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    uint16              first_page     = 0;
    uint16              second_page     = 0;
    uint16              current_frame = 0;    
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    if (0 != idle_grid_ptr->slide_anim.slide_flag)
    {
        return TRUE;
    }
    if(!GUI_PointIsInRect( point,  idle_grid_ptr->display_rect))
    {
        return TRUE;
    }
    
    if ((point.x > idle_grid_ptr->down_point.x) && (0 == idle_grid_ptr->cur_page_index))
    {
        return TRUE;
    }

    if ((point.x < idle_grid_ptr->down_point.x) && (idle_grid_ptr->page_num - 1 == idle_grid_ptr->cur_page_index))
    {
        return TRUE;
    }    

    if (idle_grid_ptr->is_Uidle_1st_mov)
    {
        if (!InitGridUIdle(idle_grid_ptr))
        {
            //memory is not enough, close u idle effect.
            MMISET_SetUIdleStyle(0);
            return FALSE;
        }
        
        idle_grid_ptr->is_Uidle_1st_mov = FALSE;
    }
    
    first_page    = idle_grid_ptr->cur_page_index;
    
    if (point.x > idle_grid_ptr->down_point.x) // 向右移动
    {
        direction = GO_MOVE_RIGHT;
    }
    else
    {
        direction = GO_MOVE_LEFT;
    }
    
    current_frame = (abs(point.x - idle_grid_ptr->down_point.x)/GO_ONE_FRAME_PIX);
    
    if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }

    if (point.x > idle_grid_ptr->down_point.x)
    {
        second_page = idle_grid_ptr->cur_page_index - 1;
    }
    else
    {
        second_page = idle_grid_ptr->cur_page_index + 1;
    }
         
    DrawGridUIdleEffect(idle_grid_ptr,first_page,second_page,direction,current_frame); 

    idle_grid_ptr->page_layer_offset.x += point.x - idle_grid_ptr->save_point.x;
    idle_grid_ptr->save_point = point;  
    
    return TRUE;
}

/*****************************************************************************/
//  Description: free u Idle action.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIUIDLE_FreeUIdleAction(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{

    SCI_FREE(s_calc_point_buf); 
    SCI_FREE(s_u_shining_ctrl_ptr);
    SCI_FREE(s_u_calc_param_ptr);
  
    FreeCacheItems(idle_grid_ptr);
    MMIWIDGET_ReleaseAllWidgetsCache(idle_grid_ptr);
    
    SCI_MEMSET(&s_prev_page_info, 0, sizeof(MMIGRID_U_PAGE_INFO_T));
    SCI_MEMSET(&s_cur_page_info, 0, sizeof(MMIGRID_U_PAGE_INFO_T));
    SCI_MEMSET(&s_next_page_info, 0, sizeof(MMIGRID_U_PAGE_INFO_T));
    SCI_MEMSET(s_prev_item_info, 0, MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(GO_MAINMENU_ICON_INFO_T));
    SCI_MEMSET(s_cur_item_info, 0, MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(GO_MAINMENU_ICON_INFO_T));
    SCI_MEMSET(s_next_item_info, 0, MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(GO_MAINMENU_ICON_INFO_T));

    UILAYER_SetPreMultiplyAlpha(&idle_grid_ptr->page_layer,FALSE,FALSE);        
}

/*****************************************************************************/
//  Description: Handle u Idle slide tp up. 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIUIdle_HandleUIdleSlideTpUp(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)

{
    uint16              second_page     = 0; // 第二页
    uint16              pre_page_index  = 0; // 记录移动之前的页
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    uint16 current_frame = 0;    
    BOOLEAN is_change_page = FALSE;
    float fling_velocity_x = 0;
    float fling_velocity_y = 0;
    MMIGRID_U_PAGE_INFO_T *page_info_ptr = PNULL;
    uint16 free_page_index = 0;
    int16 i = 0;

    if (idle_grid_ptr->is_Uidle_1st_mov)
    {
        MMIUIDLE_FreeUIdleAction(idle_grid_ptr);
        return free_page_index;        
    }

    MMK_ComputeCurrentVelocity((float*)&fling_velocity_x, &fling_velocity_y,(float)MMI_IDLE_FLING_MAX_VELOCITY,(float)MMI_IDLE_FLING_MAX_VELOCITY );
    
    pre_page_index = idle_grid_ptr->cur_page_index;

    if (point.x > idle_grid_ptr->down_point.x) //right move
    {
        direction = GO_MOVE_RIGHT;
        second_page = (pre_page_index + idle_grid_ptr->page_num - 1) % idle_grid_ptr->page_num;
    }
    else
    {
        direction = GO_MOVE_LEFT;
        second_page = (pre_page_index + 1) % idle_grid_ptr->page_num;
    }

    if ((GO_MOVE_RIGHT == direction && 0 == idle_grid_ptr->cur_page_index)
        || (GO_MOVE_LEFT == direction && idle_grid_ptr->page_num - 1 == idle_grid_ptr->cur_page_index))
    {
        MMIUIDLE_FreeUIdleAction(idle_grid_ptr);
        return FALSE;
    }
           
    current_frame = (abs(point.x - idle_grid_ptr->down_point.x) / GO_ONE_FRAME_PIX);
    
    if (point.x < idle_grid_ptr->down_point.x) // left move
    {
        // last page?
        if ((current_frame >= GO_MAINMENU_TOTAL_FRAME_MAX/2))
        {
            free_page_index = idle_grid_ptr->cur_page_index - 1;
            is_change_page = TRUE;
            page_info_ptr = &s_prev_page_info;                                
        }
    }
    else // right move
    {
        // first page?
        if ((current_frame >= GO_MAINMENU_TOTAL_FRAME_MAX/2))
        {
            free_page_index = idle_grid_ptr->cur_page_index + 1;
            is_change_page = TRUE;
            page_info_ptr = &s_next_page_info;
        }
    }
       
    if(current_frame > GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }

    if(is_change_page)
    {
        for (i = current_frame; i <= GO_MAINMENU_TOTAL_FRAME_MAX; i += GO_MAINMENU_TOTAL_FRAME_MAX/20)
        {            
            DrawGridUIdleEffect(idle_grid_ptr,pre_page_index,second_page,direction,i);

            idle_grid_ptr->page_layer_offset.x += ((GO_MOVE_LEFT == direction) ? (-GO_MAINMENU_TOTAL_FRAME_MAX/20) : (GO_MAINMENU_TOTAL_FRAME_MAX/20));
            
            MMITHEME_UpdateRectEx();         
        }  

        if (GO_MAINMENU_TOTAL_FRAME_MAX != i)
        {
            DrawGridUIdleEffect(idle_grid_ptr,pre_page_index,second_page,direction,GO_MAINMENU_TOTAL_FRAME_MAX);
            MMITHEME_UpdateRectEx();          
        }

        if(GO_MOVE_LEFT == direction)
        {
            idle_grid_ptr->cur_page_index++;
        }
        else
        {
            idle_grid_ptr->cur_page_index--;
        }
    }
    else
    {
        for (i = current_frame; i>= 0;i-=GO_MAINMENU_TOTAL_FRAME_MAX/20)
        {
            DrawGridUIdleEffect(idle_grid_ptr,pre_page_index,second_page,direction,i);
            idle_grid_ptr->page_layer_offset.x += ((GO_MOVE_LEFT == direction) ? (GO_MAINMENU_TOTAL_FRAME_MAX/20) : (-GO_MAINMENU_TOTAL_FRAME_MAX/20));            
            MMITHEME_UpdateRectEx(); 
        }

        if (0 != i)  /*lint !e506 !e774*/
        {
            DrawGridUIdleEffect(idle_grid_ptr,pre_page_index,second_page,direction,0);
            MMITHEME_UpdateRectEx();          
        }        
    }

    MMIUIDLE_FreeUIdleAction(idle_grid_ptr);           
    
    return free_page_index;
}

/*****************************************************************************/
//  Description: is uidle effect close or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUIdle_IsOpen(void)
{
    BOOLEAN ret = FALSE;
    
    if (UIDLE_STYLE_CLASSIC != MMIAPISET_GetUIdleStyle())
    {
        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateItemLayer(void)
{
    GUI_LCD_DEV_INFO  item_layer  = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    UILAYER_CREATE_T  create_info = {0};

    create_info.lcd_id   = GUI_MAIN_LCD_ID;

    create_info.offset_x = 0;
    create_info.offset_y = 0;    
    create_info.width    = MMITHEME_MENU_ITEM_WIDTH;
    create_info.height   = MMITHEME_MENU_ITEM_HEIGHT;

    create_info.is_static_layer = TRUE; 
    
    UILAYER_CreateLayer(&create_info, &item_layer);

    return item_layer;
}

/*****************************************************************************/
//  Description : free cache icons and some widgets.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void FreeCacheItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 i = 0;
    
    for (i = 0; i < s_prev_page_info.page_info.icon_num; i++)
    {
        if (MMIGRID_ITEM_TYPE_ICON == s_prev_page_info.item_info[i].item_type)
        {
            UILAYER_RELEASELAYER(&s_prev_page_info.item_info[i].item_dev); /*lint !e774*/ 
        }
    }

    for (i = 0; i < s_cur_page_info.page_info.icon_num; i++)
    {
        if (MMIGRID_ITEM_TYPE_ICON == s_cur_page_info.item_info[i].item_type)
        {
            UILAYER_RELEASELAYER(&s_cur_page_info.item_info[i].item_dev); /*lint !e774*/ 
        }
    }

    for (i = 0; i < s_next_page_info.page_info.icon_num; i++)
    {
        if (MMIGRID_ITEM_TYPE_ICON == s_next_page_info.item_info[i].item_type)
        {
            UILAYER_RELEASELAYER(&s_next_page_info.item_info[i].item_dev);  /*lint !e774*/ 
        }
    }    
}

/*****************************************************************************/
//  Description : cache all widgets and menu icons on prev,cur,next pages.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CacheItemsOn3Pages(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{ 
    BOOLEAN ret1 = TRUE;
    BOOLEAN ret2 = TRUE;
    BOOLEAN ret3 = TRUE;    
    int32 prev_page_idx = 0;
    int32 cur_page_idx = 0;
    int32 next_page_idx = 0;

    if (0 == idle_grid_ptr->cur_page_index)
    {
        prev_page_idx = idle_grid_ptr->page_num - 1;
        cur_page_idx  = idle_grid_ptr->cur_page_index;
        next_page_idx = idle_grid_ptr->cur_page_index + 1;        
    }
    else
    if (idle_grid_ptr->page_num - 1 == idle_grid_ptr->cur_page_index)
    {
        prev_page_idx = idle_grid_ptr->cur_page_index - 1;
        cur_page_idx  = idle_grid_ptr->cur_page_index;
        next_page_idx = 0;     
    }
    else
    {
        prev_page_idx = idle_grid_ptr->cur_page_index - 1;
        cur_page_idx  = idle_grid_ptr->cur_page_index;
        next_page_idx = idle_grid_ptr->cur_page_index + 1;     
    }

    s_prev_page_info.page_info.icon_num = 0;
    s_prev_page_info.page_info.icon_info = s_prev_item_info;

    s_cur_page_info.page_info.icon_num = 0;
    s_cur_page_info.page_info.icon_info = s_cur_item_info;

    s_next_page_info.page_info.icon_num = 0;
    s_next_page_info.page_info.icon_info = s_next_item_info;

    ret1 = CacheItemsOnSpecificPage(idle_grid_ptr, &s_prev_page_info, prev_page_idx);

    if (ret1)
    {
        ret2 = CacheItemsOnSpecificPage(idle_grid_ptr, &s_cur_page_info,  cur_page_idx);

        if (ret2)
        {
            ret3 = CacheItemsOnSpecificPage(idle_grid_ptr, &s_next_page_info, next_page_idx);     
        }
    }

    ret1 = ret1 && ret2 && ret3;
    return ret1;
}

/*****************************************************************************/
//  Description : cache all widgets and menu icons on prev,cur,next pages.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CacheItemsOnSpecificPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIGRID_U_PAGE_INFO_T *u_page_ptr, int32 page_idx)
{
    BOOLEAN ret = TRUE;
    uint16 j = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T display_rc = {0};
    uint32 item_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id  = 0;
    CTRLMAINMENU_ITEM_T menu_item_info = {0};
    GUI_RECT_T  page_rect = {0};
    uint32 page_grid_num = 0;
    uint32      cur_idx = 0;
    
    if (page_idx >= idle_grid_ptr->page_num)
    {
        return FALSE;
    }

    if(!MMK_IsWindowLandscape( idle_grid_ptr->win_id))   
    {   
        page_grid_num = MMI_IDLE_GRID_PAGE_CELL_NUM;
    }
    else
    {
        page_grid_num = MMI_IDLE_GRID_PAGE_CELL_NUM_LANDSCAPE;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    page_rect = idle_grid_ptr->display_rect;
  
    for (j = 0; j < page_grid_num; j++)
    {         
        if (!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page_idx][j]))
        {
            if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_idx][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                widget_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);

                if (0 == widget_id || widget_id > MMIWIDGET_WIDGET_MAX)
                {
                    continue;
                }

                item_ptr = &idle_grid_ptr->item_ptr[widget_id - 1];
                    
                if(0 == MMIWIDGET_IsWidgetCacheValid(item_ptr))
                {
                    MMIWIDGET_CreateWidgetCache(item_ptr);                   
                    MMIWIDGET_UpdateWidgetCache(item_ptr);
                }


                MMIWIDGET_GetWinRect(idle_grid_ptr , widget_id, &display_rc); 

                if(DATA_TYPE_PMARGB888== SEAPI_GetCalcInputDataType())
                {    
                    UILAYER_SetPreMultiplyAlpha(&item_ptr->lcd_dev_cache,TRUE,TRUE);
                }
                
                cur_idx = ++u_page_ptr->page_info.icon_num - 1;
                u_page_ptr->page_info.icon_info[cur_idx].icon_buf      = (uint32*)UILAYER_GetLayerBufferPtr(&item_ptr->lcd_dev_cache);

                if (PNULL == u_page_ptr->page_info.icon_info[cur_idx].icon_buf)
                {
                    ret = FALSE;
                    break;
                }
                
                u_page_ptr->page_info.icon_info[cur_idx].icon_offset.x = (int16)(display_rc.left + lcd_width) % lcd_width;
                u_page_ptr->page_info.icon_info[cur_idx].icon_offset.y = (int16)(display_rc.top - page_rect.top);
                u_page_ptr->page_info.icon_info[cur_idx].icon_size.w   = (uint16)(display_rc.right - display_rc.left + 1);
                u_page_ptr->page_info.icon_info[cur_idx].icon_size.h   = (uint16)(display_rc.bottom - display_rc.top + 1); 
                u_page_ptr->item_info[cur_idx].item_dev  = item_ptr->lcd_dev_cache;
                u_page_ptr->item_info[cur_idx].item_type = MMIGRID_ITEM_TYPE_WIDGET;                
            }
            else
            if (IS_NODE_ICON(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                GUI_LCD_DEV_INFO cache_layer = {0};
                GUI_RECT_T       icon_rect   = {0};
                GUIMENU_DISPLAY_ITEM_PARAM_T para = {0};
                
                item_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);
                group_id = idle_grid_ptr->icon_ptr[item_id].icon_info.group_id;
                menu_id  = idle_grid_ptr->icon_ptr[item_id].icon_info.menu_id;
                MMIMAINMENU_GetItemData(menu_id, group_id, &menu_item_info);
                               
                display_rc = idle_grid_ptr->page_grid_rect[j];

                cache_layer = CreateItemLayer();

                icon_rect.left   = 0;
                icon_rect.top    = 0;
                icon_rect.right  = MMITHEME_MENU_ITEM_WIDTH - 1;
                icon_rect.bottom = MMITHEME_MENU_ITEM_HEIGHT - 1;
                
                para.format = GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER;
                para.dst_layer = cache_layer;
                para.dst_rect  = icon_rect;
                
                MMITHEME_DrawMainmenuItem(idle_grid_ptr->win_id, &menu_item_info, &para);

                if(DATA_TYPE_PMARGB888== SEAPI_GetCalcInputDataType())
                {    
                    UILAYER_SetPreMultiplyAlpha(&cache_layer,TRUE,TRUE);
                }
                display_rc = MMIWIDGET_GetIconRectFromGridRect(display_rc);
                
                cur_idx = ++u_page_ptr->page_info.icon_num - 1;
                u_page_ptr->page_info.icon_info[cur_idx].icon_buf      = (uint32*)UILAYER_GetLayerBufferPtr(&cache_layer);

                if (PNULL == u_page_ptr->page_info.icon_info[cur_idx].icon_buf)
                {
                    ret = FALSE;
                    break;
                }
                
                u_page_ptr->page_info.icon_info[cur_idx].icon_offset.x = (int16)display_rc.left;
                u_page_ptr->page_info.icon_info[cur_idx].icon_offset.y = (int16)(display_rc.top - page_rect.top); 
                u_page_ptr->page_info.icon_info[cur_idx].icon_size.w   = (uint16)MMITHEME_MENU_ITEM_WIDTH; 
                u_page_ptr->page_info.icon_info[cur_idx].icon_size.h   = (uint16)MMITHEME_MENU_ITEM_HEIGHT;
                u_page_ptr->item_info[cur_idx].item_dev  = cache_layer;
                u_page_ptr->item_info[cur_idx].item_type = MMIGRID_ITEM_TYPE_ICON;                 
            }            
        }
    } 

    return ret; 
}
//**************************************************************************//
//********************* U IDLE EFFECT CODES END ****************************//
//**************************************************************************//

#endif
