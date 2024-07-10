/*****************************************************************************
** File Name:      mmimenu_u_mainmenu_se.c                                   *
** Author:                                                                   *
** Date:           05/05/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2012      Xiaoqing.Lu        Creat
******************************************************************************/

#include "mmi_app_menu_trc.h"

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "mmitheme_mainmenu.h"
#include "guires.h"
#include "ui_layer.h"
#include "guilcd.h"
#include "mmk_tp.h"
#include "graphics_3d_rotate.h"
#include "u_launcher_internal.h"
#include "mmi_mainmenu_u_se.h"
#include "mmi_id.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_istyle.h"
#include "mmi_mainmenu_istyle_u_se.h"

//#ifdef UI_SUPPORT_U_MAINMENU
#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//u mainmenu item info.
LOCAL GO_MAINMENU_ICON_INFO_T s_prev_item_info[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0}; 
LOCAL GO_MAINMENU_ICON_INFO_T s_cur_item_info [MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
LOCAL GO_MAINMENU_ICON_INFO_T s_next_item_info[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};

LOCAL GUI_LCD_DEV_INFO        s_item_page_layer[GO_MEM_PAGE_NUM] = {0};

//u mainmenu page info.
LOCAL U_MAINMENU_PAGE_INFO_T s_prev_page_info = {0};
LOCAL U_MAINMENU_PAGE_INFO_T s_cur_page_info  = {0};
LOCAL U_MAINMENU_PAGE_INFO_T s_next_page_info = {0};

LOCAL GO_MAINMENU_PAGE_INFO_T *s_cur_page_info_ptr  = PNULL;
LOCAL GO_MAINMENU_PAGE_INFO_T *s_next_page_info_ptr = PNULL;

LOCAL FIX16_POINT_T           *s_calc_point_buf     = PNULL;
LOCAL S3D_EXPANDPTR_T         *s_u_calc_param_ptr   = PNULL;  /*lint !e728*/

LOCAL uint8                   *s_u_shining_ptr      = PNULL;
LOCAL uint32                  s_cur_random_style    = GO_MENU_STYLE_CLASSIC;

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
//  Description : init u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitUMainmenuEnv(MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UninitUMainmenuEnv(void);

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReleaseUmainmenuItemLayer(
                                        U_MAINMENU_PAGE_INFO_T *page_info_ptr
                                        );

/*****************************************************************************/
//  Description : release GO all item layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReleaseALLItemLayer( void );

/*****************************************************************************/
//  Description : 根据point，设置下一页
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuNextPageX(
                                   MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                   GUI_POINT_T      *tp_point_ptr,
                                   BOOLEAN is_circles // 是否循环
                                   );

/*****************************************************************************/
//  Description : 处理tpup时
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleUMenuSlideOverX(
                                    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                    GUI_POINT_T      *tp_point_ptr,
                                    BOOLEAN is_circles
                                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawUMenuSlideOverX(
							   MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr, 
							   GO_MOVE_DIRECTION_E direction,
							   uint16              current_frame,
							   BOOLEAN             is_change_page
							   );

/*****************************************************************************/
//  Description : u idle effect paint.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawUMainmenuffect(
                              MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                              GO_MOVE_DIRECTION_E direction,
                              uint16 current_frame
                              );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetMainmenuCurFrame(
                                 MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                 GUI_POINT_T*    point_ptr
                                 );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GO_MOVE_DIRECTION_E GetMainmenuMoveDiretion(
                                                  MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                                  GUI_POINT_T*    point_ptr
                                                  );

/*****************************************************************************/
//  Description : 获得各页面索引号
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetUMainmenuDispPageIndex(
                                        MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                        uint16*    prev_page_idx_ptr,
                                        uint16*    cur_page_idx_ptr,
                                        uint16*    next_page_idx_ptr,
                                        BOOLEAN    is_circles
                                        );
    
/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void U_ChangeGOMenuStyle(
                               MMI_WIN_ID_T win_id
                               );

/*****************************************************************************/
//  Description :最后一针
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN U_Effect_Zero_Type_Calc(
                                      MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                      GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
                                      IMGREF_SIZE_T bg_window_size,//屏幕尺寸
                                      uint32* bg_window_buf,//背景buffer
                                      GO_CALC_DATA_INFO_T* calc_data_info
                                      );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateItemLayer(
                           MMI_HANDLE_T handle,
                           uint16 icon_width,
                           uint16 icon_height,
                           uint16 item_total_num,
                           GUI_LCD_DEV_INFO *layer_handle
                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateAllItemsLayer(MMI_HANDLE_T handle);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitUMainmenuEnv(MMI_HANDLE_T handle)
{
    BOOLEAN ret                 = TRUE;
    uint32  calc_point_buf_size = 0;//算法所需BUF
    uint32  shining_buf_size    = SHINING_MAX_RADIUS+1;
    uint32  calc_param_size     = 0;
    //uint8   *next_ptr           = PNULL;

    calc_point_buf_size = 1024*sizeof(FIX16_POINT_T);//1024为算法所需
    calc_param_size = sizeof(S3D_EXPANDPTR_T);

    s_calc_point_buf = SCI_ALLOC_APP(calc_point_buf_size);
    SCI_MEMSET(s_calc_point_buf, 0,calc_point_buf_size);

    s_u_shining_ptr = SCI_ALLOC_APP(shining_buf_size);
    SCI_MEMSET(s_u_shining_ptr, 0,shining_buf_size);

    s_u_calc_param_ptr = SCI_ALLOC_APP(calc_param_size);
    SCI_MEMSET(s_u_calc_param_ptr, 0, calc_param_size);
    
    CreateAllItemsLayer(handle);

    s_prev_page_info.page_info.icon_num = 0;
    s_prev_page_info.page_info.icon_info = s_prev_item_info;   
    s_prev_page_info.lcd_dev_info = s_item_page_layer[0];
    ReleaseUmainmenuItemLayer(&s_prev_page_info);

    s_cur_page_info.page_info.icon_num = 0;
    s_cur_page_info.page_info.icon_info = s_cur_item_info;
    s_cur_page_info.lcd_dev_info = s_item_page_layer[1];
    ReleaseUmainmenuItemLayer(&s_cur_page_info);
    
    s_next_page_info.page_info.icon_num = 0;
    s_next_page_info.page_info.icon_info = s_next_item_info;
    s_next_page_info.lcd_dev_info = s_item_page_layer[2];
    ReleaseUmainmenuItemLayer(&s_next_page_info);
    
    return ret;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateItemLayer(
                           MMI_HANDLE_T handle,
                           uint16 icon_width,
                           uint16 icon_height,
                           uint16 item_total_num,
                           GUI_LCD_DEV_INFO *layer_handle
                           )
{
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == layer_handle )
    {
        return;
    }
    //创建所有LAYER的层
    layer_handle->lcd_id = 0;
    layer_handle->block_id = UILAYER_NULL_HANDLE;
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = icon_width;
    create_info.height = item_total_num*icon_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(&create_info, layer_handle);
    
    UILAYER_SetLayerColorKey(layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
    // 使用color清除层
    UILAYER_Clear(layer_handle);
    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateAllItemsLayer(
                               MMI_HANDLE_T handle
                               )
{
    uint16 item_num     = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 i            = 0;
        
    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        UILAYER_RELEASELAYER(&s_item_page_layer[i]);/*lint !e506 !e774*/
    }
    
    icon_width = MMITHEME_MENU_ITEM_WIDTH;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT;
    item_num = MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM;
    
    CreateItemLayer(handle, icon_width,icon_height,item_num,&s_item_page_layer[0]);
    if(s_item_page_layer[0].block_id == UILAYER_NULL_HANDLE)
    {
        return;
    }
    
    CreateItemLayer(handle, icon_width,icon_height,item_num,&s_item_page_layer[1]);
    if(s_item_page_layer[1].block_id == UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_item_page_layer[0]);/*lint !e506 !e774*/
        return;
    }

    CreateItemLayer(handle, icon_width,icon_height,item_num,&s_item_page_layer[2]);
    if(s_item_page_layer[2].block_id == UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_item_page_layer[1]);/*lint !e506 !e774*/
        UILAYER_RELEASELAYER(&s_item_page_layer[0]);/*lint !e506 !e774*/
        return;
    }
        
    return;
}

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UninitUMainmenuEnv(void)
{
    BOOLEAN ret                 = TRUE;

    GO_FREEIF(s_calc_point_buf);
    GO_FREEIF(s_u_calc_param_ptr);
    GO_FREEIF(s_u_shining_ptr);

    // 释放所有的item layer
	ReleaseALLItemLayer();

    return ret;
}

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReleaseUmainmenuItemLayer(
                                        U_MAINMENU_PAGE_INFO_T *page_info_ptr
                                        )
{
    uint32 i = 0;
    BOOLEAN result = FALSE;

    if (PNULL != page_info_ptr)
    {
        if (PNULL != page_info_ptr->page_info.icon_info)
        {
            for (i = 0; i < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM; i++)
            {
                // UILAYER_RELEASELAYER(&page_info_ptr->lcd_dev_info[i]);
                
                page_info_ptr->page_info.icon_info[i].icon_buf = PNULL;
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 根据point，设置下一页
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuNextPageX(
                                   MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                   GUI_POINT_T      *tp_point_ptr,
                                   BOOLEAN is_circles // 是否循环
                                   )
{
    uint16 current_frame = 0;

    if (0 ==menu_ctrl_ptr->page_info.cnt)
    {
        return FALSE;
    }
    
    current_frame = (abs(tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);
    
    if (tp_point_ptr->x < menu_ctrl_ptr->slide_start_point.x) // 向左移动
    {
        // 判断是否最后一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
            || menu_ctrl_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index < menu_ctrl_ptr->page_info.cnt - 1) // 不是最后一页
            {
                menu_ctrl_ptr->cur_page_index++; // 当前页索引号增加
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->cur_page_index%menu_ctrl_ptr->page_info.cnt;
                menu_ctrl_ptr->cur_item_index = 0;
            }
            else
            {
                if (is_circles)
                {
                    menu_ctrl_ptr->cur_page_index = 0;
                    menu_ctrl_ptr->cur_item_index = 0;
                }
            }
            
        }
    }
    else // 向右移动
    {
        // 判断是否第一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
            || menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index != 0)
            {
                menu_ctrl_ptr->cur_page_index--;
                menu_ctrl_ptr->cur_item_index = 0;
            }
            else
            {
                if (is_circles)
                {
                    menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->page_info.cnt - 1;
                    menu_ctrl_ptr->cur_item_index = 0;
                }
            }
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 处理tpup时
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_HandleUMenuSlideOverX(
                                               MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                               GUI_POINT_T      *tp_point_ptr,
                                               BOOLEAN is_circles
                                               )
{
    return HandleUMenuSlideOverX(menu_ctrl_ptr, tp_point_ptr, is_circles);
}

/*****************************************************************************/
//  Description : 处理tpup时
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleUMenuSlideOverX(
                                    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                    GUI_POINT_T      *tp_point_ptr,
                                    BOOLEAN is_circles
                                    )

{
    BOOLEAN     result = FALSE;
    uint16      first_page  = 0; // 记录移动之前的页
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    uint16 current_frame = 0;
    BOOLEAN is_change_page = FALSE;
    
    first_page = menu_ctrl_ptr->cur_page_index;
        
    // 设置下一页
    SetMainmenuNextPageX(menu_ctrl_ptr, tp_point_ptr, is_circles);
    
    direction = GetMainmenuMoveDiretion(menu_ctrl_ptr, tp_point_ptr);
    current_frame = GetMainmenuCurFrame(menu_ctrl_ptr, tp_point_ptr);

    if(first_page == menu_ctrl_ptr->cur_page_index)
    {
        is_change_page = FALSE;
    }
    else
    {
        is_change_page = TRUE;
    }

    DrawUMenuSlideOverX(menu_ctrl_ptr,direction,current_frame,is_change_page);

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_DrawUMenuLastFrame(
                                            MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr, 
                                            GO_MOVE_DIRECTION_E direction,
                                            BOOLEAN             is_change_page
                                            )
{
    int16   i = 0; // 
    BOOLEAN result = TRUE;
    
    if (is_change_page)
    {
        i = GO_MAINMENU_TOTAL_FRAME_MAX;
        DrawUMainmenuffect(menu_ctrl_ptr,direction,i);
        MMITHEME_UpdateRect();
    }
    else
    {
        i = 0;
        DrawUMainmenuffect(menu_ctrl_ptr, direction,i);
        MMITHEME_UpdateRect();
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DrawUMenuSlideOverX(
							   MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr, 
							   GO_MOVE_DIRECTION_E direction,
							   uint16              current_frame,
							   BOOLEAN             is_change_page
							   )
{
    int16 i = 0;

    if (is_change_page)
    {
        for (i = current_frame; i<= GO_MAINMENU_TOTAL_FRAME_MAX; i+=GO_FRAME_STEP_NUM)
        {
            DrawUMainmenuffect(menu_ctrl_ptr,direction,i);
            MMITHEME_UpdateRect();
        }  
        
        if((GO_MAINMENU_TOTAL_FRAME_MAX+GO_FRAME_STEP_NUM) != i)
        {
            i = GO_MAINMENU_TOTAL_FRAME_MAX;
            DrawUMainmenuffect(menu_ctrl_ptr,direction,i);
            MMITHEME_UpdateRect();
        }
    }
    else
    {
        for (i = current_frame; i>= 0;i-=GO_FRAME_STEP_NUM)
        {
            DrawUMainmenuffect(menu_ctrl_ptr,direction,i);
            MMITHEME_UpdateRect();
        }
        
        if(-GO_FRAME_STEP_NUM != i)
        {
            i = 0;
            DrawUMainmenuffect(menu_ctrl_ptr,direction,i);
            MMITHEME_UpdateRect();
        }
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GO_MOVE_DIRECTION_E GetMainmenuMoveDiretion(
                                                  MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                                  GUI_POINT_T*    point_ptr
                                                  )

{   
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    
    SCI_ASSERT(PNULL != point_ptr); /*assert verified*/   
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
        
    if (point_ptr->x > menu_ctrl_ptr->slide_start_point.x) // 向右移动
    {
        direction = GO_MOVE_RIGHT;
    }
    else
    {
        direction = GO_MOVE_LEFT;
    }
    
    return direction;
}
 
/*****************************************************************************/
//  Description : 获得各页面索引号
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetUMainmenuDispPageIndex(
                                        MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                        uint16*    prev_page_idx_ptr,
                                        uint16*    cur_page_idx_ptr,
                                        uint16*    next_page_idx_ptr,
                                        BOOLEAN    is_circles
                                        )
{
    uint16 prev_page_idx = INVALID_PAGE_INDEX;
    uint16 cur_page_idx = INVALID_PAGE_INDEX;
    uint16 next_page_idx = INVALID_PAGE_INDEX;
    uint16 item_total_num = menu_ctrl_ptr->page_info.cnt;

    SCI_ASSERT(PNULL != prev_page_idx_ptr); /*assert verified*/  
    SCI_ASSERT(PNULL != cur_page_idx_ptr); /*assert verified*/  
    SCI_ASSERT(PNULL != next_page_idx_ptr); /*assert verified*/  

    if (0 == item_total_num) // 如果空页
    {
        cur_page_idx = prev_page_idx = next_page_idx = INVALID_PAGE_INDEX;
    }
    else if (1 == item_total_num) // 如果只有一页
    {
        prev_page_idx = cur_page_idx = next_page_idx = menu_ctrl_ptr->cur_page_index;
        if (!is_circles)
        {
            prev_page_idx = next_page_idx = INVALID_PAGE_INDEX;
        }
    }
    else if (0 == menu_ctrl_ptr->cur_page_index) // 如果当前页为第一页
    {
        prev_page_idx = item_total_num - 1;
        cur_page_idx  = menu_ctrl_ptr->cur_page_index;
        next_page_idx = menu_ctrl_ptr->cur_page_index + 1;
        if (!is_circles)
        {
            prev_page_idx = INVALID_PAGE_INDEX;
        }
    }
    else if (item_total_num - 1 == menu_ctrl_ptr->cur_page_index) // 如果当前页为最后一页
    {
        prev_page_idx = menu_ctrl_ptr->cur_page_index - 1;
        cur_page_idx  = menu_ctrl_ptr->cur_page_index;
        next_page_idx = 0;
        if (!is_circles)
        {
            next_page_idx = INVALID_PAGE_INDEX;
        }
    }
    else // 请他情况
    {
        prev_page_idx = menu_ctrl_ptr->cur_page_index - 1;
        cur_page_idx  = menu_ctrl_ptr->cur_page_index;
        next_page_idx = menu_ctrl_ptr->cur_page_index + 1;     
    }

    *prev_page_idx_ptr = prev_page_idx;
    *cur_page_idx_ptr = cur_page_idx;
    *next_page_idx_ptr = next_page_idx;

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetMainmenuCurFrame(
                                 MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                 GUI_POINT_T*    point_ptr
                                 )

{
    uint16  current_frame = 0;    
    
    SCI_ASSERT(PNULL != point_ptr); /*assert verified*/   
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    
    current_frame = (abs(point_ptr->x - menu_ctrl_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);
    
    if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }
    
    return current_frame;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIISTYLE_U_GenerateRandomStyle(
                                            MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
                                            )
{
	if(GO_MENU_STYLE_RANDOM== MMIAPISET_GetGOmenuStyle())
    {        
        do{
            s_cur_random_style = rand()%GO_MENU_STYLE_MAX;
            }while((s_cur_random_style == GO_MENU_STYLE_RANDOM)
#ifdef GO_FLASH_MAIN_MENU_SUPPORT  
            || (GO_MENU_STYLE_FLASH == s_cur_random_style)
#endif
            ||(UAPIMenu_GetCalcStyleSlideType(s_cur_random_style) == GO_SLIDE_SUPPORT_ONLY_Y)
            );

        // menu_ctrl_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(s_cur_random_style);
    }

    return s_cur_random_style;
}

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_ChangeMenuStyle(
                                      MMI_WIN_ID_T win_id
                                      )
{
    U_ChangeGOMenuStyle(win_id);
}

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void U_ChangeGOMenuStyle(
                               MMI_WIN_ID_T win_id
                               )
{
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr = PNULL;
    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();
    
    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(MMK_GetCtrlHandleByWin(win_id, MAINMENUWIN_MENU_CTRL_ID));
    
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    do{
        cur_style +=1;
        cur_style = (uint32)cur_style%GO_MENU_STYLE_MAX;
        MMISET_SetGOmenuStyle(cur_style);
        
    }while(
#ifdef GO_FLASH_MAIN_MENU_SUPPORT  
        (GO_MENU_STYLE_FLASH == cur_style)
#else
        0
#endif
        );
    
    page_num = MIN(menu_ctrl_ptr->page_info.cnt, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
    
    first_page = menu_ctrl_ptr->cur_page_index;
    next_page = (first_page+page_num-1)%page_num;
    
    for(current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2);current_frame>=0;current_frame-=GO_FRAME_STEP_NUM)
    {
        DrawUMainmenuffect(menu_ctrl_ptr,GO_MOVE_LEFT,current_frame);
        MMITHEME_UpdateRect();
    }
    if(-GO_FRAME_STEP_NUM != current_frame)
    {
        current_frame = 0;
        DrawUMainmenuffect(menu_ctrl_ptr,GO_MOVE_LEFT,current_frame);
        MMITHEME_UpdateRect();
    }

    return;
}

/*****************************************************************************/
//  Description :最后一针
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN U_Effect_Zero_Type_Calc(
                                      MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                      GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
                                      IMGREF_SIZE_T bg_window_size,//屏幕尺寸
                                      uint32* bg_window_buf,//背景buffer
                                      GO_CALC_DATA_INFO_T* calc_data_info
                                      )
{
    int32 i=0,row=0,column=0;
    FIX16_POINT_T window_offset={0};
    uint16 icon_num = 0;
    
    row=cur_page_info->total_row_num;
    column=cur_page_info->total_col_num;

    icon_num = cur_page_info->icon_num;
    
    for(i = 0; i < icon_num; i++)
    {
        if(NULL!=(*(cur_page_info->icon_info+i)).icon_buf)
        {	
            //贴图标
            window_offset.x=(cur_page_info->icon_info+i)->icon_offset.x;
            window_offset.y=(cur_page_info->icon_info+i)->icon_offset.y;
            //画高亮框
            S2D_TypeBlending(bg_window_size, 
                bg_window_buf,
                calc_data_info->output_type,(cur_page_info->icon_info+i)->icon_size,
                window_offset,
                (cur_page_info->icon_info +i)->icon_buf,
                calc_data_info->input_type, 
                bg_window_buf,
                PNULL
                );
        }
    }

    return TRUE;
}


/*****************************************************************************/
//  Description : u idle effect paint.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawUMainmenuffect(
                              MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                              GO_MOVE_DIRECTION_E direction,
                              uint16 current_frame
                              )
{     
    IMGREF_SIZE_T bg_window_size = {0, 0};
    GUI_RECT_T    rect = {0};
    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();
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
    
    rect = UILAYER_GetLayerRect(&menu_ctrl_ptr->page_layer_handle[0]);

    bg_window_size.w = rect.right - rect.left + 1;
    bg_window_size.h = rect.bottom - rect.top + 1;

    if (menu_ctrl_ptr->is_cur_pagesearch && 0 == menu_ctrl_ptr->cur_page_index)
    {
        if (GO_MOVE_LEFT == direction || GO_MOVE_UP == direction)
        {
            s_cur_page_info_ptr  = &s_prev_page_info.page_info;
            s_next_page_info_ptr = &s_cur_page_info.page_info;        
        }
    }
    else
    {
        s_cur_page_info_ptr  = &s_cur_page_info.page_info;
        
        if (GO_MOVE_LEFT == direction || GO_MOVE_UP == direction)
        {
            s_next_page_info_ptr = &s_next_page_info.page_info;        
        }
        else
        {
            s_next_page_info_ptr = &s_prev_page_info.page_info;            
        }
    }
      
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0],TRUE,FALSE);
        
        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());

        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_PMARGB888); 

        calc_data_info.param = s_u_calc_param_ptr;

        calc_data_info.param->shiningCtrl_ptr = s_u_shining_ptr;

        if((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            UILAYER_SwitchDoubleMem(&menu_ctrl_ptr->page_layer_handle[0], FALSE );
        }
        else
        {
            UILAYER_SetDirectDraw( TRUE );
            UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            UILAYER_SwitchDoubleMem(&menu_ctrl_ptr->page_layer_handle[0], TRUE);
        }
        
        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);

        //算法输出BUF
        if(PNULL == (bg_window_buf = (uint32*)UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->page_layer_handle[0])))
        {
            return;
        }  
    }

    if((0 == current_frame)
        &&(MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
        )
    {
        U_Effect_Zero_Type_Calc(
            menu_ctrl_ptr,
            s_cur_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            &calc_data_info
            );
    }
    else if(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame
        &&(MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state))
    {
        U_Effect_Zero_Type_Calc(
            menu_ctrl_ptr,
            s_next_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            &calc_data_info
            );
    }
    else
    {
        if(GO_MENU_STYLE_RANDOM == cur_style)
        {
            calc_func = UAPIMenu_GetCallBackfunc(s_cur_random_style);
        }
        else
        {
            calc_func = UAPIMenu_GetCallBackfunc(cur_style);
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

    }  
    pre_frame = current_frame;
}

/*****************************************************************************/
//  Description : u mainmenu display
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_Display(
                                 MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                 GUI_POINT_T    *point_ptr
                                 )
{
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    uint16  current_frame = 0; 
    
    direction = GetMainmenuMoveDiretion(menu_ctrl_ptr, point_ptr);
    current_frame = GetMainmenuCurFrame(menu_ctrl_ptr, point_ptr);

    DrawUMainmenuffect(menu_ctrl_ptr, direction, current_frame);

    return TRUE;
}

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_UninitUMainmenuEnv(void)
{
    return UninitUMainmenuEnv();
}

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_InitUMainmenuEnv(MMI_HANDLE_T handle)
{
    return InitUMainmenuEnv(handle);
}

/*****************************************************************************/
//  Description : release GO all item layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReleaseALLItemLayer( void )
{
	ReleaseUmainmenuItemLayer(&s_cur_page_info);
    ReleaseUmainmenuItemLayer(&s_prev_page_info);
    ReleaseUmainmenuItemLayer(&s_next_page_info);
	
	return TRUE;
}

/*****************************************************************************/
//  Description : release GO all item layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_ReleaseAllItemLayer( void)
{
    return ReleaseALLItemLayer();   
}

/*****************************************************************************/
//  Description : 获得页面数据
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_SetPageInfo(
                                     MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                     BOOLEAN is_circles // 是否循环
                                     )
{
    uint16 prev_page_idx = 0;
    uint16 cur_page_idx = 0;
    uint16 next_page_idx = 0;
    //GUIMENU_STYLE_E cur_style = GUIMENU_ISTYLE;
    
    if(PNULL==menu_ctrl_ptr)
    {
        return FALSE;
    }

    // 先释放所有的item layer
	ReleaseALLItemLayer();

    // 获取页面索引
    GetUMainmenuDispPageIndex(menu_ctrl_ptr, &prev_page_idx, &cur_page_idx, &next_page_idx, is_circles);

    // 画各页面的数据
    if (PNULL != menu_ctrl_ptr->U_SetPageInfo)
    {
        menu_ctrl_ptr->U_SetPageInfo(menu_ctrl_ptr, cur_page_idx, (void*)&s_cur_page_info);
        menu_ctrl_ptr->U_SetPageInfo(menu_ctrl_ptr, prev_page_idx, (void*)&s_prev_page_info);
        menu_ctrl_ptr->U_SetPageInfo(menu_ctrl_ptr, next_page_idx, (void*)&s_next_page_info);
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_ResetIstyleItemLayerPtr(
                                              uint16 item_num,
                                              U_MAINMENU_PAGE_INFO_T *umenu_page_info_ptr
                                              )
{
    uint16 i = 0;
    uint32* next_ptr = PNULL;
    uint16 icon_width = 0;
    uint16 icon_height = 0; 
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == umenu_page_info_ptr)
    {
        return;
    }

    lcd_dev_info = umenu_page_info_ptr->lcd_dev_info;
    next_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);

    if (PNULL == next_ptr)
    {
        return;
    }
    
    icon_width  = MMITHEME_MENU_ITEM_WIDTH;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT;

    item_num = MIN(item_num, MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM);

    for (i = 0; i < item_num; i++)
    {
        umenu_page_info_ptr->page_info.icon_info[i].icon_buf = next_ptr;
        next_ptr += icon_width*icon_height;
    }

    for (; i < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM; i++)
    {
        umenu_page_info_ptr->page_info.icon_info[i].icon_buf = PNULL;
    }

    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_DistoryPageLayer(void)
{
    uint16 i            = 0;
    
    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        UILAYER_RELEASELAYER(&s_item_page_layer[i]);/*lint !e506 !e774*/
    }
}

#endif
//#endif // UI_SUPPORT_U_MAINMENU
