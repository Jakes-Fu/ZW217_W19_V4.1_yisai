/****************************************************************************
** File Name:      mmiidle_upb.c                                                *
** Author:                                                                 *
** Date:           05/14/2012                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2012       kyle.jin         Create
** 
****************************************************************************/

#ifdef UPB_IDLE_SUPPORT

#define _MMI_IDLE_UPB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiidle_upb_export.h"
#include "graphics_3d_rotate.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmk_tp.h"
#include "window_parse.h"
#include "mmk_app.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT)
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_page.h"
#endif
#if (defined QBTHEME_SUPPORT)
#include "mmiqbtheme_idleexport.h"
#endif
#include "window_parse.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UPB_IDLE_PAGE_NUM  7
#ifdef MAINLCD_SIZE_240X320
#define UPB_PAGE_WIDTH  150
#define UPB_PAGE_HEIGHT 195
#define UPB_LIGHT_WIDTH   UPB_PAGE_WIDTH
#define UPB_UP_LIGHT_HEIGHT  12
#define UPB_DOWN_LIGHT_HEIGHT  20
#define UPB_UP_LIGHT_HEIGHT_START  19
#define UPB_DOWN_LIGHT_HEIGHT_START  174
#define UPB_ENTER_EXIT_ANIM_FRAME 20  //入场动画帧数
#define UPB_GAP 10   //面之间的缝隙
#define UPB_EYE_Z 220  //眼睛的深度
#define UPB_EYE_Y 0   //眼睛的高度   1  
#define ANGLEDOWN 600

#define UPB_SOFTKEY_ICON_WIDTH 65
#define UPB_CENTER_POINT_X  (MMI_MAINSCREEN_WIDTH/2)   //UPB的位置
#define UPB_CENTER_POINT_Y  (MMI_MAINSCREEN_HEIGHT*55/100)  //UPB的高度   2

#elif defined MAINLCD_SIZE_240X400
#define UPB_PAGE_WIDTH  150
#define UPB_PAGE_HEIGHT 243
#define UPB_LIGHT_WIDTH   UPB_PAGE_WIDTH
#define UPB_UP_LIGHT_HEIGHT  12
#define UPB_DOWN_LIGHT_HEIGHT  20
#define UPB_UP_LIGHT_HEIGHT_START  19
#define UPB_DOWN_LIGHT_HEIGHT_START  222
#define UPB_ENTER_EXIT_ANIM_FRAME 20  //入场动画帧数
#define UPB_GAP 10   //面之间的缝隙
#define UPB_EYE_Z 220  //眼睛的深度
#define UPB_EYE_Y 0   //眼睛的高度   1  
#define ANGLEDOWN 550
#define UPB_SOFTKEY_ICON_WIDTH 65
#define UPB_CENTER_POINT_X  (MMI_MAINSCREEN_WIDTH/2)   //UPB的位置
#define UPB_CENTER_POINT_Y  (MMI_MAINSCREEN_HEIGHT*55/100)  //UPB的高度   2

#elif defined MAINLCD_SIZE_320X480
#define UPB_PAGE_WIDTH  200
#define UPB_PAGE_HEIGHT 260
#define UPB_LIGHT_WIDTH   UPB_PAGE_WIDTH
#define UPB_UP_LIGHT_HEIGHT  16
#define UPB_DOWN_LIGHT_HEIGHT  24
#define UPB_UP_LIGHT_HEIGHT_START  19
#define UPB_DOWN_LIGHT_HEIGHT_START  235
#define UPB_ENTER_EXIT_ANIM_FRAME 20  //入场动画帧数
#define UPB_GAP 15   //面之间的缝隙
#define UPB_EYE_Z 300  //眼睛的深度
#define UPB_EYE_Y 0   //眼睛的高度   1  
#define ANGLEDOWN 450
#define UPB_SOFTKEY_ICON_WIDTH 65
#define UPB_CENTER_POINT_X  (MMI_MAINSCREEN_WIDTH/2)   //UPB的位置
#define UPB_CENTER_POINT_Y  (MMI_MAINSCREEN_HEIGHT*55/100)  //UPB的高度   2

#else
#define UPB_PAGE_WIDTH  200
#define UPB_PAGE_HEIGHT 260
#define UPB_LIGHT_WIDTH   UPB_PAGE_WIDTH
#define UPB_UP_LIGHT_HEIGHT  16
#define UPB_DOWN_LIGHT_HEIGHT  24
#define UPB_UP_LIGHT_HEIGHT_START  19
#define UPB_DOWN_LIGHT_HEIGHT_START  235
#define UPB_ENTER_EXIT_ANIM_FRAME 20  //入场动画帧数
#define UPB_GAP 15   //面之间的缝隙
#define UPB_EYE_Z 300  //眼睛的深度
#define UPB_EYE_Y 0   //眼睛的高度   1  
#define ANGLEDOWN 450
#define UPB_SOFTKEY_ICON_WIDTH 65
#define UPB_CENTER_POINT_X  (MMI_MAINSCREEN_WIDTH/2)   //UPB的位置
#define UPB_CENTER_POINT_Y  (MMI_MAINSCREEN_HEIGHT*55/100)  //UPB的高度   2

#endif


//用分辨率的宏控制
#define UPBSHADOWGAP  4
#define SHADOWPERSENTAGE 20
#define EXFRAME 2
/**-------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct{
int x;
int y;
}IMGREF_LONG_POINT_T;
typedef enum
{
    PRISM_STATE_NONE = 0,//静止
    PRISM_STATE_DOWN,   //拖动
    PRISM_STATE_MOVE,
    PRISM_STATE_ANAMI_IN,
    PRISM_STATE_ANAMI_OUT,
    PRISM_STATE_BACK,   //返回原位
    PRISM_STATE_ROTATE,// 幅度大的时候返回原位
    PRISM_STATE_TURN,//翻一页
 //   PRISM_STATE_STANDBY_ANIM,  //待机动画
    PRISM_STATE_MAX

}PRISM_STATUS_E;
typedef struct  {
    GUI_POINT_T Cur_Point;
    GUI_POINT_T Last_Point;
    GUI_POINT_T Down_Point;
    GUI_POINT_T Up_Point;
    int count_times;
    int speed;
    int turn_angle;
    int cur_frame;
    int total_frame;
    PRISM_STATUS_E cur_state;
}MOUSE_INFO_T;
typedef struct UPB_IDLE_INFO_TAG
{
    GUI_LCD_DEV_INFO page_layer_arr[UPB_IDLE_PAGE_NUM];
    GUI_LCD_DEV_INFO upb_main_layer;
    GUI_LCD_DEV_INFO softkey_layer;
    GUI_LCD_DEV_INFO light_layer;
    GUI_LCD_DEV_INFO page_bg_layer;
    uint16* bg_ptr;
    MOUSE_INFO_T Mouse_Info;
    BOOLEAN  is_open_win;
    MMI_HANDLE_T   win_handle;
    uint8   upb_slide_timer;
    uint8   upb_fling_timer;
    GUI_RECT_T softkey_rect;
    int16 set_num;
}UPB_IDLE_INFO_T;

//算法

typedef struct  {
	int radius;     //直径
	IMGREF_3D_POINT_T Ball_centre;   //中心坐标

}SEVEN_PRISM_DATA_PARAM_T;

typedef struct  {
	uint8  side_num;      //边个数
	uint16 * color_ptr;   //边的图像指针
	IMGREF_3D_POINT_T In_3D_ver [ 4 ];   //边的四个顶点
	
}SIDE_INFO_T;

typedef struct {
	uint16 * out_buff;  //输出buffer
	IMGREF_SIZE_T win_size;  //输出size
	IMGREF_SIZE_T src_size;   //图像size
	uint16 angle;     //无视
	int16 set_num;
	SIDE_INFO_T  S_Prism_Side[7];  
	MOUSE_INFO_T * M_Info_T_Ptr;   //  鼠标控制信息
	SEVEN_PRISM_DATA_PARAM_T Prism_geometry;   //几何信息
}SEVEN_PRISM_INPARAM_T;
typedef struct  {
	int32 deep_z;
	uint8 num;
}DEEP_NUM_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL UPB_IDLE_INFO_T *s_upb_idle_info_ptr = PNULL;

int16 s_Focus_Num=0;
int16 s_Start_F_Num=0;
static int32 s_Angle_X=0;
static int32 s_Angle_Y=0;
static int32 s_speed_x=0;
int32 start_angle_x=0;
int32 start_angle_y=0;


/*****************************************************************************/
//  Description : MMIIDLE_UPB_HandleMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_UPB_HandleMsg(
                                         MMI_WIN_ID_T         win_id,
                                         MMI_MESSAGE_ID_E     msg_id,
                                         DPARAM               param
                                         );
                                         
WINDOW_TABLE(IDLE_UPB_WIN_TAB) =
{
    WIN_FUNC((uint32)MMIIDLE_UPB_HandleMsg),
    WIN_ID(IDLE_UPB_WIN_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),//禁止转场动画
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    END_WIN
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL UPB_IDLE_INFO_T *UPB_GetUpbInfoPtr(void);

LOCAL void Creat_Seven_Prism( SEVEN_PRISM_INPARAM_T * indata_param);
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_DrawOneFrame(UPB_IDLE_INFO_T *upb_info_ptr,BOOLEAN is_direct_draw);
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleAnimOut(UPB_IDLE_INFO_T *upb_info_ptr);


/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                        *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL UPB_IDLE_INFO_T *UPB_GetUpbInfoPtr(void)
{
    return s_upb_idle_info_ptr;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_CreateAllPageLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    uint16 i = 0;
    UILAYER_CREATE_T create_info = {0};
    
    for(i=0;i<UPB_IDLE_PAGE_NUM;i++)
    {
    	// 申请之前先释放
    	UILAYER_RELEASELAYER(&upb_info_ptr->page_layer_arr[i]);
    	
        upb_info_ptr->page_layer_arr[i].lcd_id = 0;
        upb_info_ptr->page_layer_arr[i].block_id = UILAYER_NULL_HANDLE;

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = upb_info_ptr->win_handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0; 
        create_info.width = UPB_PAGE_WIDTH;
        create_info.height = UPB_PAGE_HEIGHT;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE;
        
        UILAYER_CreateLayer(
                            &create_info,  
                            &(upb_info_ptr->page_layer_arr[i])
                            );
                            
        UILAYER_SetLayerColorKey(&(upb_info_ptr->page_layer_arr[i]), TRUE, UILAYER_TRANSPARENT_COLOR);

        UILAYER_Clear(&(upb_info_ptr->page_layer_arr[i]));
        
    }
    
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_ReleaseAllPageLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    uint16 i = 0;
    
    for(i=0;i<UPB_IDLE_PAGE_NUM;i++)
    {
    	UILAYER_RELEASELAYER(&upb_info_ptr->page_layer_arr[i]);
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_CreateSoftkeyBar(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    UILAYER_RESULT_E ret = UILAYER_RESULT_SUCCESS;
    
    if(upb_info_ptr == PNULL)
    {
        return;
    }
    
	// 申请之前先释放
	UILAYER_RELEASELAYER(&upb_info_ptr->softkey_layer);

    upb_info_ptr->softkey_layer.lcd_id = 0;
    upb_info_ptr->softkey_layer.block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = upb_info_ptr->win_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0; 
	GUIRES_GetImgWidthHeight((uint16*)&create_info.width,(uint16*)&create_info.height,IMAGE_UPB_SOFTKEY_ICON,upb_info_ptr->win_handle);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    
    ret = UILAYER_CreateLayer(
                        &create_info,  
                        &(upb_info_ptr->softkey_layer)
                        );

    UILAYER_SetLayerColorKey(&(upb_info_ptr->softkey_layer), TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_Clear(&(upb_info_ptr->softkey_layer));
    
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_ReleaseSoftkeyBar(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_RELEASELAYER(&upb_info_ptr->softkey_layer);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_DrawSoftkeyBar(UPB_IDLE_INFO_T *upb_info_ptr)
{
    GUI_POINT_T display_point = {0,0};
    IMGREF_SIZE_T	bg_window = {0};
    UILAYER_INFO_T layer_info = {0};
    
    if(upb_info_ptr == PNULL||upb_info_ptr->bg_ptr == PNULL)
    {
        return;
    }
                   
    if (UILAYER_IsLayerActive(&upb_info_ptr->softkey_layer))
    {
        GUIRES_DisplayImg(&display_point,
            			PNULL,
            			PNULL,
            			upb_info_ptr->win_handle,
            			IMAGE_UPB_SOFTKEY_ICON,
            			(const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer);
            			
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&bg_window.w,&bg_window.h);
        UILAYER_GetLayerInfo((const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer,&layer_info);

        upb_info_ptr->softkey_rect.left = (bg_window.w-UPB_SOFTKEY_ICON_WIDTH)/2;
        upb_info_ptr->softkey_rect.right = upb_info_ptr->softkey_rect.left+UPB_SOFTKEY_ICON_WIDTH;
        upb_info_ptr->softkey_rect.top = bg_window.h - layer_info.layer_height;
        upb_info_ptr->softkey_rect.bottom = bg_window.h -1;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void *UPB_GetIdleDataPtr(void)
{
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    #ifdef MMIWIDGET_SUPPORT
    MMIWIDGET_CTRL_E  ctrl_id = MMIWIDGET_CTRL_IDLE_ID;
    #endif
    void *ptr = PNULL;
    
    //get idle style
    idle_style = MMIAPISET_GetIdleStyle();
    
    switch (idle_style)
    {
        #ifdef MMIWIDGET_SUPPORT
        case MMISET_IDLE_STYLE_WIDGET: 
            ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);
            break;
        #endif

        #ifdef MMI_GRID_IDLE_SUPPORT
        case MMISET_IDLE_STYLE_GRID: 
            ptr = MMIAPIGRID_GetGridPtr();
            break;
        #endif

        #ifdef QBTHEME_SUPPORT
        case MMISET_IDLE_QBTHEME:      
            ptr = MMIAPIQBIDLE_GetGridPtr();
            break;
        #endif
        default:
            break;
    }
    
    return ptr;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
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
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_Grid_DrawWidgetToPage(const GUI_LCD_DEV_INFO *dev_info_ptr,MMI_IDLE_GRID_DATA_T *idle_grid_ptr,uint16 page_idx,FIX16_POINT_T *Point_Add_ptr)
{
    uint32 page_grid_num = 0;
    uint16 j = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T display_rc = {0};
    uint32 item_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id  = 0;
    CTRLMAINMENU_ITEM_T menu_item_info = {0};
    IMGREF_SIZE_T window = {0};
	IMGREF_SIZE_T img = {0};
	IMGREF_POINT_T window_mid = {0};
	IMGREF_POINT_T img_mid = {0};
	int32 angle = 0;
	COLOR32 *in_src_ptr = PNULL;
	COLOR16 *out_src_ptr = PNULL;
	int32 z_para = 4096;
	uint8 zero_size = 2;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    
    //如果没有七页
    if (page_idx >= idle_grid_ptr->page_num)
    {
        return;
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


    z_para = 4096*UPB_PAGE_WIDTH/lcd_width;
    
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


                MMIWIDGET_GetWinRect(idle_grid_ptr, widget_id, &display_rc); 

                display_rc.left = (display_rc.left + idle_grid_ptr->page_num*lcd_width)%lcd_width;
                display_rc.right = (display_rc.right+idle_grid_ptr->page_num*lcd_width)%lcd_width;
                if(display_rc.right<display_rc.left)
                {
                    display_rc.right = lcd_width;
                }
                window.w = UPB_PAGE_WIDTH;
                window.h = UPB_PAGE_HEIGHT;
                img.w = display_rc.right - display_rc.left + 1;
                img.h = display_rc.bottom - display_rc.top + 1;
                
                window_mid.x = display_rc.left*UPB_PAGE_WIDTH/lcd_width;
                window_mid.y = display_rc.top*UPB_PAGE_WIDTH/lcd_width;
                in_src_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&item_ptr->lcd_dev_cache);
                out_src_ptr = (uint16*)UILAYER_GetLayerBufferPtr(dev_info_ptr);

                out_src_ptr = out_src_ptr;
                //画到upb page层位置

                
                S2d_4_in_one_fast_zerosize(
                                            window,
                                            img,
                                            window_mid,
                                            img_mid,
                                            angle,
                                            in_src_ptr,
                                            out_src_ptr,
                                            zero_size,
                                            z_para,
                                            0xff,
                                            Point_Add_ptr
                                            );

                //释放缓存
                UILAYER_RELEASELAYER(&item_ptr->lcd_dev_cache);
                
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

                display_rc = MMIWIDGET_GetIconRectFromGridRect(display_rc);
                display_rc.left = (display_rc.left+idle_grid_ptr->page_num*lcd_width)%lcd_width;
                display_rc.right = (display_rc.right+idle_grid_ptr->page_num*lcd_width)%lcd_width;
                if(display_rc.right<display_rc.left)
                {
                    display_rc.right = lcd_width;
                }
                window.w = UPB_PAGE_WIDTH;
                window.h = UPB_PAGE_HEIGHT;
                img.w = MMITHEME_MENU_ITEM_WIDTH;
                img.h = MMITHEME_MENU_ITEM_HEIGHT;
                
                window_mid.x = display_rc.left*UPB_PAGE_WIDTH/lcd_width;
                window_mid.y = display_rc.top*UPB_PAGE_WIDTH/lcd_width;
                in_src_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&cache_layer);
                out_src_ptr = (uint16*)UILAYER_GetLayerBufferPtr(dev_info_ptr);

                out_src_ptr = out_src_ptr;
                //画到upb page层位置

                S2d_4_in_one_fast_zerosize(
                                                window,
                                                img,
                                                window_mid,
                                                img_mid,
                                                angle,
                                                in_src_ptr,
                                                out_src_ptr,
                                                zero_size,
                                                z_para,
                                                0xff,
                                                Point_Add_ptr
                                                );

                //释放层
                UILAYER_RELEASELAYER(&cache_layer);
                
            }            
        }
    } 
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_DrawWidgetToPage(const GUI_LCD_DEV_INFO *dev_info_ptr,uint16 upb_page_index,FIX16_POINT_T *Point_Add_ptr)
{
    void *idle_data_ptr = PNULL;
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMI_IDLE_GRID_DATA_T* idle_grid_ptr = PNULL;
    
    idle_data_ptr = UPB_GetIdleDataPtr();
    
    if(PNULL == idle_data_ptr)
    {
        return;
    }
    
    //get idle style
    idle_style = MMIAPISET_GetIdleStyle();
    

    if(idle_style == MMISET_IDLE_STYLE_GRID
        ||idle_style == MMISET_IDLE_QBTHEME
        )
    {
        idle_grid_ptr = (MMI_IDLE_GRID_DATA_T*)idle_data_ptr;
        
        UPB_Grid_DrawWidgetToPage(dev_info_ptr,idle_grid_ptr,upb_page_index,Point_Add_ptr);
    }
    else if(idle_style == MMISET_IDLE_STYLE_WIDGET)
    {

    }
    else
    {
        return;
    }
    
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_CreateMAINLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_CREATE_T create_info = {0};

	// 申请之前先释放
	UILAYER_RELEASELAYER(&upb_info_ptr->upb_main_layer);
	
    upb_info_ptr->upb_main_layer.lcd_id = 0;
    upb_info_ptr->upb_main_layer.block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = upb_info_ptr->win_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0; 
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,(uint16*)&create_info.width,(uint16*)&create_info.height);
    create_info.is_bg_layer = TRUE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;
    UILAYER_CreateLayer(
                        &create_info,  
                        &(upb_info_ptr->upb_main_layer)
                        );
                        
    UILAYER_SetLayerColorKey(&(upb_info_ptr->upb_main_layer), TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_Clear(&(upb_info_ptr->upb_main_layer));
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_ReleaseMAINLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_RELEASELAYER(&upb_info_ptr->upb_main_layer);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_CreateDrawLightLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_POINT_T display_point = {0,0};
    
	// 申请之前先释放
	UILAYER_RELEASELAYER(&upb_info_ptr->light_layer);
	
    upb_info_ptr->light_layer.lcd_id = 0;
    upb_info_ptr->light_layer.block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = upb_info_ptr->win_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0; 
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width,(uint16*)&create_info.height,IMAGE_UPB_LIGHT,upb_info_ptr->win_handle);
    create_info.is_bg_layer = TRUE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(
                        &create_info,  
                        &(upb_info_ptr->light_layer)
                        );
                        
    UILAYER_SetLayerColorKey(&(upb_info_ptr->light_layer), TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_Clear(&(upb_info_ptr->light_layer));
    
    GUIRES_DisplayImg(&display_point,
            			PNULL,
            			PNULL,
            			upb_info_ptr->win_handle,
            			IMAGE_UPB_LIGHT,
            			(const GUI_LCD_DEV_INFO*)&upb_info_ptr->light_layer);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_ReleaseLightLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
   UILAYER_RELEASELAYER(&upb_info_ptr->light_layer);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_CreateDrawPageBgLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_POINT_T display_point = {0,0};
    
	// 申请之前先释放
	UILAYER_RELEASELAYER(&upb_info_ptr->page_bg_layer);
	
    upb_info_ptr->page_bg_layer.lcd_id = 0;
    upb_info_ptr->page_bg_layer.block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = upb_info_ptr->win_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0; 
    //GUIRES_GetImgWidthHeight((uint16*)&create_info.width,(uint16*)&create_info.height,IMAGE_UPB_PAGE_BG,upb_info_ptr->win_handle);
    create_info.width = UPB_PAGE_WIDTH;
    create_info.height = UPB_PAGE_HEIGHT;
    create_info.is_bg_layer = TRUE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(
                        &create_info,  
                        &(upb_info_ptr->page_bg_layer)
                        );
                        
    UILAYER_SetLayerColorKey(&(upb_info_ptr->page_bg_layer), TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_Clear(&(upb_info_ptr->page_bg_layer));
    
    GUIRES_DisplayImg(&display_point,
            			PNULL,
            			PNULL,
            			upb_info_ptr->win_handle,
            			IMAGE_UPB_PAGE_BG,
            			(const GUI_LCD_DEV_INFO*)&upb_info_ptr->page_bg_layer);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_ReleasePageBgLayer(UPB_IDLE_INFO_T *upb_info_ptr)
{
   UILAYER_RELEASELAYER(&upb_info_ptr->page_bg_layer);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_DrawAllPageLayerBg(UPB_IDLE_INFO_T *upb_info_ptr)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 k = 0;
    uint16 *light_ptr = PNULL;
    uint16 *page_ptr = PNULL;
    uint16 * page_bg_ptr = PNULL;
    
    UPB_CreateDrawPageBgLayer(upb_info_ptr);
    UPB_CreateDrawLightLayer(upb_info_ptr);
    
    //把统一的背景图片画到层上
    if(!UILAYER_IsLayerActive(&upb_info_ptr->light_layer)
    ||!UILAYER_IsLayerActive(&upb_info_ptr->page_bg_layer)
    )
    {
        UPB_ReleaseLightLayer(upb_info_ptr);
        UPB_ReleasePageBgLayer(upb_info_ptr);
        return;
    }
    
    light_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&upb_info_ptr->light_layer);
    page_bg_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&upb_info_ptr->page_bg_layer);
    
    for(i=0;i<UPB_IDLE_PAGE_NUM;i++)
    {
        if(!UILAYER_IsLayerActive(&upb_info_ptr->page_layer_arr[i]))
        {
            UPB_ReleaseLightLayer(upb_info_ptr);
            UPB_ReleasePageBgLayer(upb_info_ptr);
            return;
        }
        page_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&upb_info_ptr->page_layer_arr[i]);

        //把统一的背景图片画到层上
        SCI_MEMCPY(page_ptr,page_bg_ptr,UPB_PAGE_WIDTH*UPB_PAGE_HEIGHT*sizeof(uint16));
        
        //画每个page上的光
        for(j=0;j<UPB_UP_LIGHT_HEIGHT;j++)
        {
            for(k=0;k<UPB_LIGHT_WIDTH;k++)
            {
                page_ptr[(UPB_UP_LIGHT_HEIGHT_START+j)*UPB_PAGE_WIDTH+k] = light_ptr[(i*(UPB_UP_LIGHT_HEIGHT+UPB_DOWN_LIGHT_HEIGHT)+UPB_DOWN_LIGHT_HEIGHT+j)*UPB_LIGHT_WIDTH+k];
            }
        }

        for(j=0;j<UPB_DOWN_LIGHT_HEIGHT;j++)
        {
            for(k=0;k<UPB_LIGHT_WIDTH;k++)
            {
                page_ptr[(UPB_DOWN_LIGHT_HEIGHT_START+j)*UPB_PAGE_WIDTH+k] = light_ptr[(i*(UPB_UP_LIGHT_HEIGHT+UPB_DOWN_LIGHT_HEIGHT)+j)*UPB_LIGHT_WIDTH+k];
            }
        }           			
    }

    UPB_ReleaseLightLayer(upb_info_ptr);
    UPB_ReleasePageBgLayer(upb_info_ptr);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL GUI_RECT_T UPB_GetCurPageRect(void)
{
    GUI_RECT_T rect = {0};
    GUI_POINT_T center_point = {UPB_CENTER_POINT_X,UPB_CENTER_POINT_Y};
    

    rect.left = center_point.x-UPB_PAGE_WIDTH/2;
    rect.right = rect.left + UPB_PAGE_WIDTH;
    rect.top = center_point.y - UPB_PAGE_HEIGHT/2;
    rect.bottom = rect.top + UPB_PAGE_HEIGHT;

    return rect;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_DrawOneFrame(UPB_IDLE_INFO_T *upb_info_ptr,BOOLEAN is_direct_draw)
{
    int i = 0;
    SEVEN_PRISM_INPARAM_T data_param={0};
    uint16 width = 0;
    uint16 height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T update_rect = {0};
    static uint32 one_frame_time = 0;
    uint32 calc_time = 0;
#if defined(MAINLCD_SIZE_240X320)||defined(MAINLCD_SIZE_240X400)
    uint32* softkey_ptr = PNULL;
    IMGREF_SIZE_T	bg_window = {0};
    IMGREF_SIZE_T	up_window = {0};
    FIX16_POINT_T	offset = {0};
    UILAYER_INFO_T layer_info = {0};
    #endif
    
    SCI_TRACE_LOW("[UPB] ONE FRAME TIME = %d",SCI_GetTickCount()-one_frame_time);
    one_frame_time = SCI_GetTickCount();
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
    lcd_dev_info = upb_info_ptr->upb_main_layer;
    
    if(is_direct_draw)
    {
        UILAYER_SetDirectDraw( TRUE );
        UILAYER_SwitchDoubleMem(&lcd_dev_info,TRUE);
    }
    else
    {
        UILAYER_SetDirectDraw( FALSE );
        UILAYER_SwitchDoubleMem(&lcd_dev_info,FALSE);
    }

    data_param.out_buff=(uint16*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    if(PNULL == data_param.out_buff)
    {
        return;
    }
    data_param.src_size.w=UPB_PAGE_WIDTH;
    data_param.src_size.h=UPB_PAGE_HEIGHT;
    data_param.Prism_geometry.Ball_centre.x=UPB_CENTER_POINT_X;
    data_param.Prism_geometry.Ball_centre.y=UPB_CENTER_POINT_Y;
    
    data_param.Prism_geometry.Ball_centre.z=((data_param.src_size.w/2+UPB_GAP)*s2d_cos_new(1800/7)/s2d_sin_new(1800/7));
    data_param.Prism_geometry.radius=(data_param.src_size.w/2+UPB_GAP)*65536/s2d_sin_new(1800/7);;
    
    data_param.M_Info_T_Ptr=&upb_info_ptr->Mouse_Info;
    data_param.set_num = upb_info_ptr->set_num;
    
    for(i=0;i<UPB_IDLE_PAGE_NUM;i++)
    	{	

    		data_param.S_Prism_Side[i].color_ptr=(uint16 *)UILAYER_GetLayerBufferPtr(&upb_info_ptr->page_layer_arr[i]);//给面赋图像
    	}
    	
    data_param.win_size.w=width;
    data_param.win_size.h=height;
    data_param.angle=0;
    
    SCI_MEMCPY(data_param.out_buff,upb_info_ptr->bg_ptr,width*height*sizeof(uint16));

    SCI_TRACE_LOW("[UPB] memcpy TIME = %d",SCI_GetTickCount()-one_frame_time);
    calc_time = SCI_GetTickCount();
    
    Creat_Seven_Prism(&data_param); 

    #if defined(MAINLCD_SIZE_240X320)||defined(MAINLCD_SIZE_240X400)
    if (UILAYER_IsLayerActive(&upb_info_ptr->softkey_layer) ) 
    {
        softkey_ptr = (uint32*)UILAYER_GetLayerBufferPtr((const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer);

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&bg_window.w,&bg_window.h);
        UILAYER_GetLayerInfo((const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer,&layer_info);
        up_window.w = layer_info.layer_width;
        up_window.h = layer_info.layer_height;
        
        offset.y = bg_window.h - up_window.h+4;
        
        S2D_Blending(bg_window,upb_info_ptr->bg_ptr,up_window,offset,softkey_ptr,data_param.out_buff );
    }
    #endif
    
    SCI_TRACE_LOW("[UPB] calc TIME = %d",SCI_GetTickCount()-calc_time);

    update_rect.bottom = height-1;
    update_rect.right = width-1;
    
    MMITHEME_StoreUpdateRect(&lcd_dev_info, update_rect);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
PUBLIC void UPB_DestoryAnim(void)
{
    s_speed_x = 0;
    
    if(PNULL == s_upb_idle_info_ptr)
    {
        return;
    }
    
    SCI_FREE(s_upb_idle_info_ptr->bg_ptr);
    s_upb_idle_info_ptr->bg_ptr = PNULL;
    
    UPB_ReleaseAllPageLayer(s_upb_idle_info_ptr);
    UPB_ReleaseMAINLayer(s_upb_idle_info_ptr);
    UPB_ReleaseSoftkeyBar(s_upb_idle_info_ptr);

    //放最后
    SCI_FREE(s_upb_idle_info_ptr);
    s_upb_idle_info_ptr = PNULL;

}
/*****************************************************************************/
// Description : Stop TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_UPB_Start(void)
{
    uint16 width = 0;
    uint16 height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint16 * mainlayer_ptr = PNULL;
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMI_HANDLE_T    handle          = MMI_INVALID_ID;
    
    //get idle style
    idle_style = MMIAPISET_GetIdleStyle();
    
    #ifdef MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID != idle_style)
    {
        return;
    }
    #else
    return;
    #endif
    
    SCI_TRACE_LOW("[UPB]START time = %d",SCI_GetTickCount());
    
    if(PNULL != s_upb_idle_info_ptr)
    {
        if(s_upb_idle_info_ptr->bg_ptr != PNULL)
        {
            SCI_FREE(s_upb_idle_info_ptr->bg_ptr);
        }

        SCI_FREE(s_upb_idle_info_ptr);
    }

    

    
    s_upb_idle_info_ptr = SCI_ALLOCA(sizeof(UPB_IDLE_INFO_T));
    if(PNULL == s_upb_idle_info_ptr)
    {
        return;
    }
    
    SCI_MEMSET(s_upb_idle_info_ptr,0,sizeof(UPB_IDLE_INFO_T));
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);

    mainlayer_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    
    s_upb_idle_info_ptr->bg_ptr = SCI_ALLOCA(width*height*sizeof(uint16));
    if(PNULL == s_upb_idle_info_ptr->bg_ptr)
    {
        SCI_FREE(s_upb_idle_info_ptr);
        s_upb_idle_info_ptr = PNULL;
        return;
    }
    
    SCI_MEMCPY(s_upb_idle_info_ptr->bg_ptr,mainlayer_ptr,width*height*sizeof(uint16));

    handle = MMK_CreateWin((uint32*)IDLE_UPB_WIN_TAB, PNULL);

    if(!handle||(MMI_INVALID_ID==handle)) 
    {
        UPB_DestoryAnim();
        return;
    }
}

/*****************************************************************************/
// Description : Stop TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void UPB_StopTimer(uint8* timer_id_ptr)
{
    if (0 != *timer_id_ptr)
    {
        MMK_StopTimer(*timer_id_ptr);
        *timer_id_ptr = 0;
    }
}
/*****************************************************************************/
// Description : start TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void UPB_StartTimer(UPB_IDLE_INFO_T *upb_info_ptr,uint8* timer_id_ptr)
{
    if (PNULL != upb_info_ptr)
    {
        if(0 != *timer_id_ptr)
        {
            UPB_StopTimer(timer_id_ptr);
        }
		
        *timer_id_ptr = MMK_CreateWinTimer(
                                            upb_info_ptr->win_handle, 
                                            10,
                                            FALSE );
    }
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleTpDownMsg(UPB_IDLE_INFO_T *upb_info_ptr,
                                    DPARAM               param
                                    )
{
    GUI_POINT_T  point = {0};
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    MMK_AddVelocityItem(point.x,point.y);	

    UPB_StopTimer(&upb_info_ptr->upb_slide_timer);
    UPB_StopTimer(&upb_info_ptr->upb_fling_timer);
    
	//upb_info_ptr->Mouse_Info.cur_frame=0;
	upb_info_ptr->Mouse_Info.Down_Point.x=point.x;
	upb_info_ptr->Mouse_Info.Down_Point.y=point.y;
	upb_info_ptr->Mouse_Info.Cur_Point.x=upb_info_ptr->Mouse_Info.Down_Point.x;
	upb_info_ptr->Mouse_Info.Cur_Point.y=upb_info_ptr->Mouse_Info.Down_Point.y;
    upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_DOWN;
    start_angle_y=s_Angle_Y;
	start_angle_x=s_Angle_X;
	s_Start_F_Num=s_Focus_Num;
    UPB_StartTimer(upb_info_ptr,&upb_info_ptr->upb_slide_timer);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleTpMoveMsg(UPB_IDLE_INFO_T *upb_info_ptr,
                                    DPARAM               param
                                    )
{
    GUI_POINT_T  point = {0};
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    MMK_AddVelocityItem(point.x,point.y);
    if(upb_info_ptr->Mouse_Info.cur_state == PRISM_STATE_MOVE)
    {
        return;
    }
    
    if(abs(point.x-upb_info_ptr->Mouse_Info.Cur_Point.x)>4
            ||abs(point.y-upb_info_ptr->Mouse_Info.Cur_Point.y)>4)
    {
        upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_MOVE;
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleTpUpMsg(UPB_IDLE_INFO_T *upb_info_ptr,
                                    DPARAM               param
                                    )
{   
    GUI_POINT_T  point = {0};
    float fling_velocity_x = 0;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    MMK_AddVelocityItem(point.x,point.y);
    
    UPB_StopTimer(&upb_info_ptr->upb_slide_timer);
    
    if(PRISM_STATE_MOVE == upb_info_ptr->Mouse_Info.cur_state)
    {
        MMK_ComputeCurrentVelocity(&fling_velocity_x,PNULL, (float)FLING_MAX_VELOCITY, (float)FLING_MAX_VELOCITY);

    	upb_info_ptr->Mouse_Info.Up_Point.x=point.x;
    	upb_info_ptr->Mouse_Info.Up_Point.y=point.y;
    	
        if(abs((int)fling_velocity_x)>=200)
        {
            upb_info_ptr->Mouse_Info.cur_state=PRISM_STATE_ROTATE;
            //Mouse_Info.total_frame=GDI_sqrt(Mouse_Info.speed*Mouse_Info.k);
            upb_info_ptr->Mouse_Info.total_frame=200;/*upb_info_ptr->Mouse_Info.speed;*/
            upb_info_ptr->Mouse_Info.cur_frame=0;
            upb_info_ptr->Mouse_Info.speed = (int)fling_velocity_x;
        }
        else
        {
        	upb_info_ptr->Mouse_Info.total_frame=15;
        	upb_info_ptr->Mouse_Info.cur_frame=0;
        	upb_info_ptr->Mouse_Info.cur_state=PRISM_STATE_BACK;
        }
        start_angle_y=s_Angle_Y;
	    start_angle_x=s_Angle_X;
        s_Start_F_Num=s_Focus_Num;
        UPB_StartTimer(upb_info_ptr,&upb_info_ptr->upb_fling_timer);
    }
    else if(PRISM_STATE_DOWN == upb_info_ptr->Mouse_Info.cur_state)
    {
        //处理点击事件，分清楚两个区域:page和切换icon
        //画开场动画，使用for循环
        //for(cur_frame=0;cur_frame<=UPB_ENTER_EXIT_ANIM_FRAME;cur_frame++)
        //{
        //    UPB_DrawOneFrame(upb_info_ptr,FALSE);
        //    MMITHEME_UpdateRect();
        //}
        if(GUI_PointIsInRect(point,upb_info_ptr->softkey_rect))
        {
            UPB_HandleAnimOut(upb_info_ptr);
            MMK_CloseWin(upb_info_ptr->win_handle);
        }
        else
        {
            if(abs(s_speed_x)>200)
            {
                upb_info_ptr->Mouse_Info.total_frame=15;
                upb_info_ptr->Mouse_Info.cur_frame=0;
            	upb_info_ptr->Mouse_Info.cur_state=PRISM_STATE_BACK;
                start_angle_y=s_Angle_Y;
                start_angle_x=s_Angle_X;
                s_Start_F_Num=s_Focus_Num;
                s_speed_x = 0;
            	UPB_StartTimer(upb_info_ptr,&upb_info_ptr->upb_fling_timer);
            }
            else
            {
                if(GUI_PointIsInRect(point,UPB_GetCurPageRect()))
                {
                    UPB_HandleAnimOut(upb_info_ptr);
                    MMK_CloseWin(upb_info_ptr->win_handle);
                }
            }
        }
    }
    else
    {
        
    }

}
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleOpenMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{
    SCI_TRACE_LOW("[UPB]OPEN time = %d",SCI_GetTickCount());
    //创建七个page层   565
    UPB_CreateAllPageLayer(upb_info_ptr);

    UPB_CreateMAINLayer(upb_info_ptr);

    UPB_CreateSoftkeyBar(upb_info_ptr);
    
    GUIWIN_SetStbState(upb_info_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    GUIWIN_SetStbBgIsDesplay(upb_info_ptr->win_handle,TRUE);
	GUIWIN_SetStbBgImageId(upb_info_ptr->win_handle, IMAGE_IDLE_STATUS_BAR);

    upb_info_ptr->is_open_win = TRUE;
    
    UILAYER_RemoveMainLayer();
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleGetFocusMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{

}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleFullPaintMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer;
    FIX16_POINT_T *Point_Add_ptr = PNULL;
    MMI_IDLE_GRID_DATA_T* idle_grid_ptr = PNULL;
    uint16 index = 0;
    #ifdef MAINLCD_SIZE_320X480
    uint32* softkey_ptr = PNULL;
    IMGREF_SIZE_T	bg_window = {0};
    IMGREF_SIZE_T	up_window = {0};
    FIX16_POINT_T	offset = {0};
    UILAYER_INFO_T layer_info = {0};
    #endif
    
    if(PNULL == upb_info_ptr)
    {
        return;
    }
    
    if(!MMK_IsFocusWin(upb_info_ptr->win_handle))
    {
        return;
    }
    if(PRISM_STATE_NONE != upb_info_ptr->Mouse_Info.cur_state)
    {
        return;
    }
    
    SCI_TRACE_LOW("[UPB]FULLPAINT START time = %d",SCI_GetTickCount());
    
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info = upb_info_ptr->upb_main_layer;
    UILAYER_AppendBltLayer(&append_layer);
    
    UPB_DrawSoftkeyBar(upb_info_ptr);
    
    SCI_TRACE_LOW("[UPB]INITSOFTKEY END time = %d",SCI_GetTickCount());

    #ifdef MAINLCD_SIZE_320X480
    if (UILAYER_IsLayerActive(&upb_info_ptr->softkey_layer) 
    )
    {
        softkey_ptr = (uint32*)UILAYER_GetLayerBufferPtr((const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer);

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&bg_window.w,&bg_window.h);
        UILAYER_GetLayerInfo((const GUI_LCD_DEV_INFO*)&upb_info_ptr->softkey_layer,&layer_info);
        up_window.w = layer_info.layer_width;
        up_window.h = layer_info.layer_height;
        
        offset.y = bg_window.h - up_window.h;
        
        S2D_Blending(bg_window,upb_info_ptr->bg_ptr,up_window,offset,softkey_ptr,upb_info_ptr->bg_ptr);
    }
    #endif

    SCI_TRACE_LOW("[UPB]INIT PAGE END time = %d",SCI_GetTickCount());
    
    Point_Add_ptr = (FIX16_POINT_T*)SCI_ALLOCA(2048*sizeof(FIX16_POINT_T));

    //入场动画
    if(upb_info_ptr->is_open_win)
    {
        //画所有的page层   
        UPB_DrawAllPageLayerBg(upb_info_ptr);
        
        upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_ANAMI_IN;
        
        idle_grid_ptr = UPB_GetIdleDataPtr();
        if( PNULL == idle_grid_ptr)
        {
            return;
        }
        upb_info_ptr->set_num = idle_grid_ptr->cur_page_index;
        upb_info_ptr->Mouse_Info.total_frame = 13*2;
        upb_info_ptr->Mouse_Info.cur_frame = 0;
         
        while(upb_info_ptr->Mouse_Info.cur_frame<=upb_info_ptr->Mouse_Info.total_frame)
        {   
            if(upb_info_ptr->Mouse_Info.cur_frame<UPB_IDLE_PAGE_NUM)
            {
                //画所有的page层  widget 
                index = (idle_grid_ptr->cur_page_index+(UPB_IDLE_PAGE_NUM-1) - upb_info_ptr->Mouse_Info.cur_frame)%UPB_IDLE_PAGE_NUM;
                UPB_DrawWidgetToPage((const GUI_LCD_DEV_INFO*)&upb_info_ptr->page_layer_arr[index],index,Point_Add_ptr);
            }
            UPB_DrawOneFrame(upb_info_ptr,TRUE);
            GUIWIN_UpdateStbIcon();
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            upb_info_ptr->Mouse_Info.cur_frame++;
        }
        upb_info_ptr->is_open_win = FALSE;
        s_Angle_X = 0;
    	s_Angle_Y = 0;
        s_Focus_Num = idle_grid_ptr->cur_page_index;
        s_Start_F_Num = s_Focus_Num;
        s_speed_x = 0;
    }
    

    upb_info_ptr->Mouse_Info.total_frame = 0;
    upb_info_ptr->Mouse_Info.cur_frame = 0;
    upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_NONE;
    SCI_FREE(Point_Add_ptr);
    SCI_TRACE_LOW("[UPB]end full paint time = %d",SCI_GetTickCount());
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleCloseMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{   
    UPB_StopTimer(&upb_info_ptr->upb_slide_timer);
    UPB_StopTimer(&upb_info_ptr->upb_fling_timer);
    
    UPB_DestoryAnim();

    #ifdef MMI_GRID_IDLE_SUPPORT
    MMIGRID_SetCurPageIndex(s_Focus_Num);
    #endif
    UILAYER_RestoreMainLayer();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL MMI_RESULT_E UPB_HandleSlideTimer(UPB_IDLE_INFO_T *upb_info_ptr)
{
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    
    MMK_GetLogicTPMsg(&state, &point);

    MMK_AddVelocityItem(point.x,point.y);
    
    if((( upb_info_ptr->Mouse_Info.cur_state == PRISM_STATE_DOWN)&&(abs(point.x-upb_info_ptr->Mouse_Info.Cur_Point.x)>6
            ||abs(point.y-upb_info_ptr->Mouse_Info.Cur_Point.y)>6)
            )
       ||(( upb_info_ptr->Mouse_Info.cur_state == PRISM_STATE_MOVE)
            &&(abs(point.x-upb_info_ptr->Mouse_Info.Cur_Point.x)>3||abs(point.y-upb_info_ptr->Mouse_Info.Cur_Point.y)>3)
            )
        )
	{
	    upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_MOVE;
		upb_info_ptr->Mouse_Info.Last_Point.x=upb_info_ptr->Mouse_Info.Cur_Point.x;
		upb_info_ptr->Mouse_Info.Last_Point.y=upb_info_ptr->Mouse_Info.Cur_Point.y;
		upb_info_ptr->Mouse_Info.Cur_Point.x=point.x;
		upb_info_ptr->Mouse_Info.Cur_Point.y=point.y;

        UPB_DrawOneFrame(upb_info_ptr,TRUE);
	}
	
	return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL MMI_RESULT_E UPB_HandleFlingTimer(UPB_IDLE_INFO_T *upb_info_ptr)
{
	if(upb_info_ptr->Mouse_Info.cur_frame<=upb_info_ptr->Mouse_Info.total_frame)
	{
	    if(upb_info_ptr->Mouse_Info.cur_frame==upb_info_ptr->Mouse_Info.total_frame)
	    {
            UPB_DrawOneFrame(upb_info_ptr,FALSE);
	    }
	    else
	    {
            UPB_DrawOneFrame(upb_info_ptr,TRUE);
	    }
        
        
		upb_info_ptr->Mouse_Info.cur_frame++;
	}
	else
	{
        upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_NONE;
        upb_info_ptr->Mouse_Info.cur_frame = 0;
	}
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL MMI_RESULT_E UPB_HandleTimerMsg(UPB_IDLE_INFO_T *upb_info_ptr,
                                    DPARAM               param
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    if(PNULL == upb_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    if(!MMK_IsFocusWin(upb_info_ptr->win_handle))
    {
        return MMI_RESULT_FALSE;
    }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527    
    if (upb_info_ptr->upb_slide_timer == *(uint8*)param)
    {
        UPB_StopTimer(&upb_info_ptr->upb_slide_timer);
        UPB_StartTimer(upb_info_ptr,&upb_info_ptr->upb_slide_timer);  
        UPB_HandleSlideTimer(upb_info_ptr);
    }
    else 
#endif
		if(upb_info_ptr->upb_fling_timer == *(uint8*)param)
    {
        UPB_StopTimer(&upb_info_ptr->upb_fling_timer);
        
        if(PRISM_STATE_NONE !=upb_info_ptr->Mouse_Info.cur_state)
        {
            UPB_StartTimer(upb_info_ptr,&upb_info_ptr->upb_fling_timer);
            UPB_HandleFlingTimer(upb_info_ptr);
        }
    }
    else
    {
        return MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleKeyUpMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{

}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleKeyDownMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{

}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleKeyLeftMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{
    
    upb_info_ptr->Mouse_Info.total_frame=15;
    upb_info_ptr->Mouse_Info.turn_angle=3600;
    upb_info_ptr->Mouse_Info.cur_state=PRISM_STATE_TURN;
        for( upb_info_ptr->Mouse_Info.cur_frame=0; upb_info_ptr->Mouse_Info.cur_frame<=upb_info_ptr->Mouse_Info.total_frame;upb_info_ptr->Mouse_Info.cur_frame++)
    {
        UPB_DrawOneFrame(upb_info_ptr,FALSE);
        MMITHEME_UpdateRect();
    }

}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleKeyRightMsg(UPB_IDLE_INFO_T *upb_info_ptr)
{
    upb_info_ptr->Mouse_Info.cur_frame=0;
    upb_info_ptr->Mouse_Info.total_frame=15;
    upb_info_ptr->Mouse_Info.turn_angle=-3600;
    upb_info_ptr->Mouse_Info.cur_state=PRISM_STATE_TURN;
    
        for( upb_info_ptr->Mouse_Info.cur_frame=0; upb_info_ptr->Mouse_Info.cur_frame<=upb_info_ptr->Mouse_Info.total_frame;upb_info_ptr->Mouse_Info.cur_frame++)
    {
        UPB_DrawOneFrame(upb_info_ptr,FALSE);
        MMITHEME_UpdateRect();
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: kyle jin
//  Note:UPB
/*****************************************************************************/
LOCAL void UPB_HandleAnimOut(UPB_IDLE_INFO_T *upb_info_ptr)
{
    upb_info_ptr->set_num = s_Focus_Num;
    upb_info_ptr->Mouse_Info.total_frame = 7*2+EXFRAME;
    upb_info_ptr->Mouse_Info.cur_frame = 0;
    upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_ANAMI_OUT;
    
    while(upb_info_ptr->Mouse_Info.cur_frame<=upb_info_ptr->Mouse_Info.total_frame)
    {
        UPB_DrawOneFrame(upb_info_ptr,TRUE);
        MMITHEME_UpdateRect();
        upb_info_ptr->Mouse_Info.cur_frame++;
    }

    upb_info_ptr->Mouse_Info.total_frame = 0;
    upb_info_ptr->Mouse_Info.cur_frame = 0;
    upb_info_ptr->Mouse_Info.cur_state = PRISM_STATE_NONE;
}
/*****************************************************************************/
//  Description : MMIIDLE_UPB_HandleMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_UPB_HandleMsg(
                                         MMI_WIN_ID_T         win_id,
                                         MMI_MESSAGE_ID_E     msg_id,
                                         DPARAM               param
                                         )
{
    MMI_RESULT_E	             result = MMI_RESULT_TRUE;
    UPB_IDLE_INFO_T *upb_info_ptr = UPB_GetUpbInfoPtr();
    
    if(PNULL == upb_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    upb_info_ptr->win_handle = win_id;

    SCI_TRACE_LOW("[UPB]handlemsg  %d",msg_id);

	switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            UPB_HandleOpenMsg(upb_info_ptr);
            break;
        case MSG_GET_FOCUS:
            UPB_HandleGetFocusMsg(upb_info_ptr);
            break;
        case MSG_FULL_PAINT:
            SCI_TRACE_LOW("[UPB]full paint");
            UPB_HandleFullPaintMsg(upb_info_ptr);
            break;
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
       case MSG_CLOSE_WINDOW:
            UPB_HandleCloseMsg(upb_info_ptr);
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:       
            UPB_HandleTpDownMsg(upb_info_ptr,param);
            break;

        case MSG_TP_PRESS_MOVE:
            UPB_HandleTpMoveMsg(upb_info_ptr,param);
            break;
        case MSG_TP_PRESS_UP:
            UPB_HandleTpUpMsg(upb_info_ptr,param);
            break;
#endif            
        case MSG_TIMER:   
            UPB_HandleTimerMsg(upb_info_ptr,param);
            break;

        case MSG_APP_CANCEL:
            UPB_HandleAnimOut(upb_info_ptr);
            MMK_CloseWin(win_id);
            break;
		case MSG_APP_UP:
    	case MSG_KEYREPEAT_UP:
			UPB_HandleKeyUpMsg(upb_info_ptr);
			break;
		case MSG_APP_DOWN:
    	case MSG_KEYREPEAT_DOWN:
			UPB_HandleKeyDownMsg(upb_info_ptr);
			break;
		case MSG_APP_LEFT:
    	case MSG_KEYREPEAT_LEFT:
			UPB_HandleKeyLeftMsg(upb_info_ptr);
			break;
		case MSG_APP_RIGHT:
    	case MSG_KEYREPEAT_RIGHT:
			UPB_HandleKeyRightMsg(upb_info_ptr);
			break;   
    	default:
        	result = MMI_RESULT_FALSE;
        	break;
    }

	return(result);  
}
LOCAL void IMG_3D_free_PRISM(	IMGREF_SIZE_T window,
						IMGREF_SIZE_T src_all_size,
						T_RECT src_rect,
						IMGREF_SIZE_T shadow_size,
						T_RECT shadow_rect,
						IMGREF_3D_POINT_T In_3D_ver[4],	//base 4
						IMGREF_3D_POINT_T In_Shadow_ver[4],	
						uint16 *src_ptr,
						uint16 *window_ptr,
						uint16 camera_z)
{
	int i = 0;
	POINT_2D_FIX8 ver[5] = {0};
	IMGREF_3D_POINT_T  eye = {0};
        POINT_2D_FIX8 sub_ver[4] = {0};

	eye.x = window.w/2;
	eye.y = UPB_EYE_Y;
	eye.z = (int16)(-camera_z);
	for( i = 0 ; i < 4 ; i ++)
	{
		s3d_Free_Point_change_POINT_2D_FIX8(In_3D_ver[i],eye,&ver[i]);
	}
	sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(src_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(src_rect.w-1);
	sub_ver[2].y = FIX8_SET(src_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(src_rect.h-1);
    
	S3D_All_Axis_Interface_Type(window,
						 src_all_size,
						 sub_ver,
						 &ver[0],				//顶点数据表指针
						 window_ptr	,
						 S3D_DATA_RGB565,
						 src_ptr,
						 S3D_DATA_RGB565,
						 PNULL);
	for( i = 0 ; i < 4 ; i ++)
	{
		s3d_Free_Point_change_POINT_2D_FIX8(In_Shadow_ver[i],eye,&ver[i]);
	}
    sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(shadow_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(shadow_rect.w-1);
	sub_ver[2].y = FIX8_SET(shadow_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(shadow_rect.h-1);
	S3D_All_Axis_Interface_shadow(window,
						 shadow_size,
						 sub_ver,
						 &ver[0],				//顶点数据表指针
						 window_ptr	,
						 src_ptr,
						 src_all_size.h);


}

 void Get_Rotation_Points(IMGREF_3D_POINT_T * OutPoint,
									int alpha_x,
									int alpha_y,
									IMGREF_3D_POINT_T InRoPoint,
									IMGREF_3D_POINT_T InFixPoint)
{	
	
	IMGREF_LONG_POINT_T fix_ro={0};
	IMGREF_LONG_POINT_T fix_temp={0};
	alpha_y=-alpha_y;
	if(0!=alpha_x)
		{
			fix_ro.x=InFixPoint.x;
			fix_ro.y=InFixPoint.z;
			fix_temp.x=(InRoPoint.x-fix_ro.x);
			fix_temp.y=(InRoPoint.z-fix_ro.y);
			
			InRoPoint.x=(fix_ro.x+fix_temp.x*s2d_cos_new(alpha_x)/65536-fix_temp.y*s2d_sin_new(alpha_x)/65536);
			InRoPoint.z=(fix_ro.y+fix_temp.x*s2d_sin_new(alpha_x)/65536+fix_temp.y*s2d_cos_new(alpha_x)/65536);
			//InRoPoint.y=InRoPoint.y;
		}
	if(0!=alpha_y)
		{
			fix_ro.x=InFixPoint.z;
			fix_ro.y=InFixPoint.y;
			fix_temp.x=(InRoPoint.z-fix_ro.x);
			fix_temp.y=(InRoPoint.y-fix_ro.y);
			
			//InRoPoint.x=InRoPoint.x;
			InRoPoint.z=(fix_ro.x+fix_temp.x*s2d_cos_new(alpha_y)/65536-fix_temp.y*s2d_sin_new(alpha_y)/65536);
			InRoPoint.y=(fix_ro.y+fix_temp.x*s2d_sin_new(alpha_y)/65536+fix_temp.y*s2d_cos_new(alpha_y)/65536);
		}
	OutPoint->x=InRoPoint.x;
	OutPoint->y=InRoPoint.y;
	OutPoint->z=InRoPoint.z;

	
}

LOCAL void Insert_Sorts(DEEP_NUM_T * node,int num)//插入排序算法
{
 int32 i=0,j=0;
 DEEP_NUM_T key={0};
 for(j=1;j<num;j++)
 	{
 		i=j-1;
		key.deep_z=node[j].deep_z;
		key.num=node[j].num;
		while((i>=0)&&(node[i].deep_z>key.deep_z))
			{
				node[i+1].deep_z=node[i].deep_z;
				node[i+1].num=node[i].num;
			  	i--;
			}
		node[i+1].deep_z=key.deep_z;
		node[i+1].num=key.num;
 	}
 
}

void  ConTrol_Points(MOUSE_INFO_T * Mouse,SIDE_INFO_T * Prism_Sides,IMGREF_3D_POINT_T * fix_centre,uint8 num,
						int * origin_angle_x,
						int * origin_angle_y,
						IMGREF_3D_POINT_T * Init_VerS,
						IMGREF_3D_POINT_T * Shadow_Base_Vers,
						IMGREF_3D_POINT_T * Shadow_Vers)
{	
	int32 Vec_X=0,Vec_Y=0;
	int32 Angle_Period_Y=0,Up_max_Y=0,Down_max_Y=0,Max_X=0,Angle_Period_X=0,Angle_Remain=0;
	int16 Max_Pixel_X=0,Max_Pixel_Y=0,Go_Over_Num=0,temp_num=0;
	int32 V_angle_x=0,V_angle_y=0;
	int32 i=0,j=0;
	int32 real_angle_x=0,real_angle_y=0;
	uint16 * temporay_ptr_array[7]={0};
	//Angle_Period_Y=450;
	Up_max_Y=200*7;
	Down_max_Y=ANGLEDOWN*7;
	//Angle_Period_X=2000;
	Max_X=2000*7;
	Max_Pixel_X=320;
	Max_Pixel_Y=160;
	if(PRISM_STATE_MOVE==Mouse->cur_state)
		{
			Vec_Y=Mouse->Cur_Point.y-Mouse->Down_Point.y;
			Vec_X=Mouse->Cur_Point.x-Mouse->Down_Point.x;
			if((Vec_X)>Max_Pixel_X)
				{
					Angle_Period_X=Max_X;
				}
			else if(Vec_X<(-Max_Pixel_X))
				{
					Angle_Period_X=-Max_X;
				}
			else
				{
					Angle_Period_X=Max_X*Vec_X/Max_Pixel_X;
				}
			Angle_Period_X=Angle_Period_X+start_angle_x;
			if((Vec_Y)>Max_Pixel_Y)
				{
					Angle_Period_Y=Down_max_Y;
				}
			else if(Vec_Y<(-Max_Pixel_Y))
				{
					Angle_Period_Y=-Up_max_Y;
				}
			else
				{	if(Vec_Y>0)
					{
						Angle_Period_Y=Down_max_Y*Vec_Y/Max_Pixel_Y;
					}
					else
						{
							Angle_Period_Y=Up_max_Y*Vec_Y/Max_Pixel_Y;
						}
				}
			Angle_Period_Y=Angle_Period_Y+start_angle_y;
			if((Angle_Period_Y>Down_max_Y)||(Angle_Period_Y<-Up_max_Y))
				{
					if(Angle_Period_Y>0)
						{
							Angle_Period_Y=Down_max_Y;
						}
					else
						{
							Angle_Period_Y=-Up_max_Y;
						}
				}

			
			while(Angle_Period_X<0)
				{
				Angle_Period_X=3600*7+Angle_Period_X;
				}
			Go_Over_Num=Angle_Period_X/3600;
			if(Go_Over_Num>7)
			{
				Go_Over_Num=7;
			}
			Angle_Remain=Angle_Period_X-Go_Over_Num*3600;
			
			s_Angle_Y=Angle_Period_Y;
			if(Angle_Remain>=(1800))
				{
					Go_Over_Num++;
					Angle_Remain=Angle_Remain-3600;
				}
			s_Angle_X=Angle_Remain;	
			s_Focus_Num=(7+s_Start_F_Num-Go_Over_Num)%7;
			
	
			
		}
	else if(PRISM_STATE_BACK==Mouse->cur_state)
		{
			s_Angle_X=start_angle_x+(0-start_angle_x)*s2d_sin_new(900*Mouse->cur_frame/Mouse->total_frame)/65536;
			s_Angle_Y=start_angle_y+(0-start_angle_y)*s2d_sin_new(900*Mouse->cur_frame/Mouse->total_frame)/65536;
		//	Mouse->cur_frame++;
			if(Mouse->cur_frame>=Mouse->total_frame)
				{
					Mouse->cur_frame=0;
					Mouse->cur_state=PRISM_STATE_NONE;
				}
		}
	else if(PRISM_STATE_ROTATE==Mouse->cur_state)
		{	
			if(0!=Mouse->total_frame)
				{
					V_angle_x=Mouse->speed*(Mouse->total_frame-Mouse->cur_frame)/Mouse->total_frame;
					//V_angle_x=Mouse->speed;
					//V_angle_x=30;
					s_speed_x = V_angle_x;
				}
			
			V_angle_y=(0-start_angle_y)/(Mouse->total_frame/10);
			Angle_Period_X=start_angle_x=start_angle_x+V_angle_x;
			Angle_Period_Y=start_angle_y=start_angle_y+V_angle_y;
			if(start_angle_y*Angle_Period_Y<0)
				{
					Angle_Period_Y=0;
				}
			while(Angle_Period_X<0)
				{
				Angle_Period_X=3600*7+Angle_Period_X;
				}
			while(Angle_Period_X>3600*7)
				{
				Angle_Period_X=Angle_Period_X-3600*7;
				}
			Go_Over_Num=Angle_Period_X/3600;
			if(Go_Over_Num>7)
			{
				Go_Over_Num=7;
			}
			Angle_Remain=Angle_Period_X-Go_Over_Num*3600;
			
			s_Angle_Y=Angle_Period_Y;
			if(Angle_Remain>=(1800))
				{
					Go_Over_Num++;
					Angle_Remain=Angle_Remain-3600;
				}
			s_Angle_X=Angle_Remain;	
			s_Focus_Num=(7+s_Start_F_Num-Go_Over_Num)%7;
			//Mouse->cur_frame++;
			if(Mouse->cur_frame>=Mouse->total_frame)
				{
					Mouse->cur_state=PRISM_STATE_BACK;
					Mouse->cur_frame=0;
					Mouse->total_frame=15;
					start_angle_x=s_Angle_X;
					start_angle_y=s_Angle_Y;
					s_Start_F_Num=s_Focus_Num;
					
				}
			//start_angle_y=s_Angle_Y;
			//start_angle_x=s_Angle_X;
			
		}
	else if(PRISM_STATE_TURN==Mouse->cur_state)
		{   
		    if(0!=Mouse->total_frame)
				{
		             V_angle_x=Mouse->turn_angle*s2d_sin_new(900*Mouse->cur_frame/Mouse->total_frame)/65536;
		        }
		    Angle_Period_X=start_angle_x+V_angle_x;
			Angle_Period_Y=start_angle_y+V_angle_y;
			if(start_angle_y*Angle_Period_Y<0)
				{
					Angle_Period_Y=0;
				}
			while(Angle_Period_X<0)
				{
				Angle_Period_X=3600*7+Angle_Period_X;
				}
			while(Angle_Period_X>3600*7)
				{
				Angle_Period_X=Angle_Period_X-3600*7;
				}
			Go_Over_Num=Angle_Period_X/3600;
			if(Go_Over_Num>7)
			{
				Go_Over_Num=7;
			}
			Angle_Remain=Angle_Period_X-Go_Over_Num*3600;
			
			s_Angle_Y=Angle_Period_Y;
			if(Angle_Remain>=(1800))
				{
					Go_Over_Num++;
					Angle_Remain=Angle_Remain-3600;
				}
			s_Angle_X=Angle_Remain;	
			s_Focus_Num=(7+s_Start_F_Num-Go_Over_Num)%7;
			//Mouse->cur_frame++;
			if(Mouse->cur_frame>=Mouse->total_frame)
				{
					Mouse->cur_state=PRISM_STATE_NONE;
					Mouse->cur_frame=0;
					Mouse->total_frame=15;
					start_angle_x=s_Angle_X;
					start_angle_y=s_Angle_Y;
					s_Start_F_Num=s_Focus_Num;
					
				}
		}
	//根据全局的状态信息调整姿势
	for(i=0;i<num;i++)
			{
				temp_num=(s_Focus_Num+i)%7;
				temporay_ptr_array[i]=Prism_Sides[temp_num].color_ptr;
			}
			for(i=0;i<num;i++)
			{
				
				Prism_Sides[i].color_ptr=temporay_ptr_array[i];
			}
				for(i=0;i<num;i++)
			{	
				real_angle_x=(origin_angle_x[i]+s_Angle_X)/7;
				real_angle_y=(origin_angle_y[i]+s_Angle_Y)/7;
				for(j=0;j<4;j++)
					{
						
						Get_Rotation_Points(&(Prism_Sides[i].In_3D_ver[j]), 
											 real_angle_x, 
											 real_angle_y,
											 (Init_VerS[j]),
											 * fix_centre);
						
					}
				for(j=0;j<4;j++)
    				{
    					
    					Get_Rotation_Points((Shadow_Vers+i*4+j), 
    										 real_angle_x, 
    										 real_angle_y,
    										 Shadow_Base_Vers[j],
    										 * fix_centre);
    					
    				}
			}
}
void Upb_Anami_Out(SEVEN_PRISM_INPARAM_T * indata_param,
						int16 side_num,
						int * origin_angle_x,
						int * origin_angle_y,
						IMGREF_3D_POINT_T * Init_VerS,
						IMGREF_3D_POINT_T * Shadow_Base_Vers,
						IMGREF_3D_POINT_T * Shadow_Vers)
{
	int32 t_frame=0,c_frame=0,step_c_frame=0,step_two_frame=0;
	int32 i=0,j=0,act_num=0,set_num=0,k=0;
	int16 src_wid=0,Go_Over_Num=0,temp_num=0;
	int16 src_height=0;
	int32 distance=0,deep_z=0;
	IMGREF_3D_POINT_T fix_point[4]={0};
	int32 Angle_Period_X=0,Angle_Remain=0;
	int32 real_angle_x=0,real_angle_y=0;
	int32 whole_angle=0;
	int32 SinData45=0,SinDataRemian=0;
	BOOLEAN Is_Rotate[7]={0};
	BOOLEAN Is_Next_Step=FALSE;
	src_wid=indata_param->src_size.w;
	src_height=indata_param->src_size.h;
	deep_z=indata_param->Prism_geometry.Ball_centre.z*2+60;
	set_num=indata_param->set_num;
	distance=deep_z+100;
	t_frame=indata_param->M_Info_T_Ptr->total_frame;
	c_frame=indata_param->M_Info_T_Ptr->cur_frame;
	whole_angle=3600*6;
	
	step_two_frame=(t_frame-EXFRAME)/7;
	SinData45=s2d_sin_new(450);
	SinDataRemian=65536-SinData45;
	for(i=0;i<side_num;i++)
		{     //act_num=(3+side_num-i)%side_num;
			act_num=(set_num+side_num+3-i)%side_num;
			if(c_frame<(i*step_two_frame))
				{
					Is_Rotate[i]=TRUE;
					Is_Next_Step=TRUE;
					
				}
			else if((c_frame>=(i*step_two_frame))&&(c_frame<((i+1)*step_two_frame+2)))
				{
					for(j=0;j<4;j++)
						{
							
							Get_Rotation_Points(&(fix_point[j]), 
												 3600*3/7, 
												 0,
												 Init_VerS[j],
												indata_param->Prism_geometry.Ball_centre);
							
						}
					step_c_frame=c_frame-(i*step_two_frame);
					for(j=0;j<4;j++)
						{    if(j==0)
						      {
						      		k=1;
							}
							else if(j==1)
								{
									k=0;
								}
							else if(j==2)
								{
									k=3;
								}
							else if(j==3)
								{
									k=2;
								}
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].x=fix_point[j].x+(Init_VerS[k].x-fix_point[j].x)*step_c_frame/(step_two_frame+EXFRAME);
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].y=fix_point[j].y+((Init_VerS[k].y-20)-fix_point[j].y)*step_c_frame/(step_two_frame+EXFRAME);
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].z=fix_point[j].z+((deep_z+(side_num-i)*40+500)-fix_point[j].z)*step_c_frame/(step_two_frame+EXFRAME);
						}
				}
			else 
				{	/*
					for(j=0;j<4;j++)
						{
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].x=Init_VerS[j].x;
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].y=Init_VerS[j].y-(side_num-i)*10-250;
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].z=(deep_z+(side_num-i)*40);
						}
					*/
				}
		}
		if(Is_Next_Step)
			{	
				if(side_num==1)

				{
				    side_num = 7;
				}
					Angle_Period_X=whole_angle*(c_frame)/((side_num-1)*step_two_frame);
				
				
					//Angle_Period_X=whole_angle*s2d_sin_new((c_frame-(step_two_frame+step_one_frame))*900/step_thr_frame)/65536;
				if(c_frame==t_frame)
					{
					c_frame=c_frame;
					}
				while(Angle_Period_X<0)
					{
					Angle_Period_X=3600*7+Angle_Period_X;
					}
				while(Angle_Period_X>3600*7)
					{
					Angle_Period_X=Angle_Period_X-3600*7;
					}
				Go_Over_Num=Angle_Period_X/3600;
				if(Go_Over_Num>7)
				{
					Go_Over_Num=7;
				}
				Angle_Remain=Angle_Period_X-Go_Over_Num*3600;
				if(Angle_Remain>=(1800))
					{
						Go_Over_Num++;
						Angle_Remain=Angle_Remain-3600;
					}
				s_Angle_X=Angle_Remain;	
				//s_Focus_Num=(7+set_num-Go_Over_Num)%7;
				for(i=0;i<side_num;i++)
					{	if(Is_Rotate[i])
						{	act_num=(set_num+side_num+3-i)%side_num;
							temp_num=(7+3-i+Go_Over_Num)%7;
							real_angle_x=(origin_angle_x[temp_num]+s_Angle_X)/7;
							real_angle_y=0;
							for(j=0;j<4;j++)
							{
								Get_Rotation_Points(&(indata_param->S_Prism_Side[act_num].In_3D_ver[j]), 
													 real_angle_x, 
													 real_angle_y,
													 (Init_VerS[j]),
													indata_param->Prism_geometry.Ball_centre);
								
							}
							for(j=0;j<4;j++)
							{
								
								Get_Rotation_Points((Shadow_Vers+act_num*4+j), 
													 real_angle_x, 
													 real_angle_y,
													 Shadow_Base_Vers[j],
													indata_param->Prism_geometry.Ball_centre);
								
							}
						}
					}
			}
}

void Upb_Anami_In(SEVEN_PRISM_INPARAM_T * indata_param,
						int16 side_num,
						int * origin_angle_x,
						int * origin_angle_y,
						IMGREF_3D_POINT_T * Init_VerS,
						IMGREF_3D_POINT_T * Shadow_Base_Vers,
						IMGREF_3D_POINT_T * Shadow_Vers)
{
	int32 t_frame=0,c_frame=0,step_c_frame=0,step_one_frame=0,step_two_frame=0,step_thr_frame=0;
	int32 i=0,j=0,k=0,act_num=0,set_num=2;
	int16 src_wid=0,Go_Over_Num=0,temp_num=0;
	int16 src_height=0;
	int32 distance=0,deep_z=0;
	IMGREF_3D_POINT_T fix_point[4]={0};
	int32 Angle_Period_X=0,Angle_Remain=0;
	int32 real_angle_x=0,real_angle_y=0;
	int32 whole_angle=0;
	BOOLEAN Is_Rotate[7]={0};
	BOOLEAN Is_Next_Step=FALSE;
	int32 SinData45=0,SinDataRemian=0;
	src_wid=indata_param->src_size.w;
	src_height=indata_param->src_size.h;
	deep_z=indata_param->Prism_geometry.Ball_centre.z*2+60;
	set_num=indata_param->set_num;
	distance=deep_z+100;
	t_frame=indata_param->M_Info_T_Ptr->total_frame;
	c_frame=indata_param->M_Info_T_Ptr->cur_frame;
	whole_angle=3600*9;
	step_one_frame=0;
	step_two_frame=(t_frame-step_one_frame)/13;
	step_thr_frame=t_frame-step_one_frame-step_two_frame;
	SinData45=s2d_sin_new(450);
	SinDataRemian=65536-SinData45;
	for(i=0;i<side_num;i++)
		{     act_num=(set_num+side_num-1-i)%side_num;
			 if((c_frame>(i*step_two_frame+step_one_frame-1))&&(c_frame<=((i+1)*step_two_frame+step_one_frame)))
				{	
					for(j=0;j<4;j++)
						{
							
							Get_Rotation_Points(&(fix_point[j]), 
												 -3600*3/7, 
												 0,
												 Init_VerS[j],
												indata_param->Prism_geometry.Ball_centre);
							
						}
					step_c_frame=c_frame-(i*step_two_frame+step_one_frame);
					for(j=0;j<4;j++)
						{	 if(j==0)
						      {
						      		k=1;
							}
							else if(j==1)
								{
									k=0;
								}
							else if(j==2)
								{
									k=3;
								}
							else if(j==3)
								{
									k=2;
								}
							
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].x=Init_VerS[k].x+(fix_point[j].x-Init_VerS[k].x)*step_c_frame/(step_two_frame+1);
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].y=(Init_VerS[k].y-20)+(fix_point[j].y-(Init_VerS[k].y-20))*step_c_frame/(step_two_frame+1);
							indata_param->S_Prism_Side[act_num].In_3D_ver[j].z=(deep_z+i*40+500)+(fix_point[j].z-(deep_z+i*40+500))*step_c_frame/(step_two_frame+1);
						}
				}
			else if(c_frame>((i+1)*step_two_frame+step_one_frame))
				{
				    if(i>=0&&i<7)
				    {
                      Is_Rotate[i]=TRUE;
				    }

					Is_Next_Step=TRUE;
					
				}
		}
		if(Is_Next_Step)
			{	
				if(c_frame<(step_one_frame+side_num*step_two_frame))
				{
					Angle_Period_X=whole_angle*(c_frame-(step_two_frame+step_one_frame))/((side_num+2)*step_two_frame);
				}
				else
					{
						Angle_Period_X=whole_angle*2/3+whole_angle/3*(s2d_sin_new(450+(c_frame-(step_one_frame+side_num*step_two_frame))*450/(6*step_two_frame))-SinData45)/SinDataRemian;
					}
					
					//Angle_Period_X=whole_angle*s2d_sin_new((c_frame-(step_two_frame+step_one_frame))*900/step_thr_frame)/65536;
				if(c_frame==t_frame)
					{
					c_frame=c_frame;
					}
				while(Angle_Period_X<0)
					{
					Angle_Period_X=3600*7+Angle_Period_X;
					}
				while(Angle_Period_X>3600*7)
					{
					Angle_Period_X=Angle_Period_X-3600*7;
					}
				Go_Over_Num=Angle_Period_X/3600;
				if(Go_Over_Num>7)
				{
					Go_Over_Num=7;
				}
				Angle_Remain=Angle_Period_X-Go_Over_Num*3600;
				if(Angle_Remain>=(1800))
					{
						Go_Over_Num++;
						Angle_Remain=Angle_Remain-3600;
					}
				s_Angle_X=Angle_Remain;	
				s_Focus_Num=(7+3+Go_Over_Num)%7;
				for(i=0;i<side_num;i++)
					{	if(Is_Rotate[i])
						{	act_num=(set_num+side_num-1-i)%side_num;
							temp_num=(7+s_Focus_Num-i+1)%7;
							real_angle_x=(origin_angle_x[temp_num]+s_Angle_X)/7;
							real_angle_y=0;
							for(j=0;j<4;j++)
							{
								Get_Rotation_Points(&(indata_param->S_Prism_Side[act_num].In_3D_ver[j]), 
													 real_angle_x, 
													 real_angle_y,
													 (Init_VerS[j]),
													indata_param->Prism_geometry.Ball_centre);
								
							}
							for(j=0;j<4;j++)
							{
								
								Get_Rotation_Points((Shadow_Vers+act_num*4+j), 
													 real_angle_x, 
													 real_angle_y,
													 Shadow_Base_Vers[j],
													indata_param->Prism_geometry.Ball_centre);
								
							}
						}
					}
			}
}

void Creat_Seven_Prism( SEVEN_PRISM_INPARAM_T * indata_param)
{	uint16 ro_angle=3600/7;
	IMGREF_3D_POINT_T First_vers [4]={0};
	int32 radius=0;
	int32 center_x=0,center_y=0,center_z=0;
	int16 src_wid=0;
	int16 src_height=0;
	int origin_angle_x[7]={0};
	int origin_angle_y[7]={0};
	DEEP_NUM_T d_num[7]={0};
	uint32 i=0;
	uint8 current_num=0,side_num=7;
	T_RECT src_rect={0};
	//s
	IMGREF_SIZE_T temp_size={0};
	T_RECT shadow_rect={0};
	IMGREF_3D_POINT_T shadow_vers[7][4]={0};
	IMGREF_3D_POINT_T shadow_First_vers [4]={0};
	int32 shadow_height=0;
	//s
	radius=indata_param->Prism_geometry.radius;
	center_x=indata_param->Prism_geometry.Ball_centre.x;
	center_y=indata_param->Prism_geometry.Ball_centre.y;
	center_z=indata_param->Prism_geometry.Ball_centre.z;
	src_wid=indata_param->src_size.w;
	src_height=indata_param->src_size.h;
	src_rect.x=0;
	src_rect.y=0;
	src_rect.w=src_wid;
	src_rect.h=src_height;
	//s
	shadow_height=src_height*SHADOWPERSENTAGE/100;
	
	temp_size.w=src_wid;
	temp_size.h=shadow_height;
	shadow_rect.x=0;
	shadow_rect.y=0;
	shadow_rect.w=src_wid;
	shadow_rect.h=shadow_height;
	//s
	for(i=0;i<7;i++)
		{
			
			origin_angle_x[i]=3600*i;
			
		}
	//计算顶点
	
	First_vers[0].z=center_z-(radius*s2d_cos_new( ro_angle/2))/65536;
	First_vers[0].x=center_x-src_wid/2;
	First_vers[0].y=center_y-src_height/2;
	First_vers[1].z=First_vers[0].z;
	First_vers[1].x=center_x+src_wid/2;
	First_vers[1].y=center_y-src_height/2;
	First_vers[2].z=First_vers[0].z;
	First_vers[2].x=center_x+src_wid/2;
	First_vers[2].y=center_y+src_height/2;
	First_vers[3].z=First_vers[0].z;
	First_vers[3].x=center_x-src_wid/2;
	First_vers[3].y=center_y+src_height/2;
    //s
    for(i=0;i<4;i++)
		{
			shadow_First_vers[i].x=First_vers[i].x;
			
			shadow_First_vers[i].z=First_vers[i].z;
		}
	shadow_First_vers[0].y=First_vers[0].y+src_height+UPBSHADOWGAP;
	shadow_First_vers[1].y=shadow_First_vers[0].y;
	shadow_First_vers[2].y=shadow_First_vers[0].y+shadow_height;
	shadow_First_vers[3].y=shadow_First_vers[0].y+shadow_height;
    //s
   if(PRISM_STATE_ANAMI_IN==indata_param->M_Info_T_Ptr->cur_state)
		{
			Upb_Anami_In( indata_param, 
							 side_num,
							 origin_angle_x, 
							origin_angle_y, 
							 &First_vers[0],
						 	&shadow_First_vers[0],
						 	* shadow_vers);
		}
	else if(PRISM_STATE_ANAMI_OUT==indata_param->M_Info_T_Ptr->cur_state)
		{
			Upb_Anami_Out( indata_param, 
							 side_num,
							 origin_angle_x, 
							origin_angle_y, 
							 &First_vers[0],
						 	&shadow_First_vers[0],
						 	* shadow_vers);
		}
	else
		{
			ConTrol_Points( indata_param->M_Info_T_Ptr,
						indata_param->S_Prism_Side,  
						&indata_param->Prism_geometry.Ball_centre,
						side_num,
						origin_angle_x,
						 origin_angle_y,
						 &First_vers[0],
						 &shadow_First_vers[0],
						 * shadow_vers);
		}
	
	//
	for(i=0;i<side_num;i++)
		{
			d_num[i].deep_z=indata_param->S_Prism_Side[i].In_3D_ver[0].z+indata_param->S_Prism_Side[i].In_3D_ver[1].z;
			d_num[i].deep_z=d_num[i].deep_z+indata_param->S_Prism_Side[i].In_3D_ver[2].z+indata_param->S_Prism_Side[i].In_3D_ver[3].z;
			d_num[i].deep_z=d_num[i].deep_z/4;
			d_num[i].num=i;
		}
	
	//sort through z
	Insert_Sorts( d_num,side_num);
	
	//sort through z
	for(i=0;i<7;i++)
		{	
			if((d_num[6-i].deep_z)<0)
			{
				current_num=current_num;
			}
			current_num=d_num[6-i].num;
			
			IMG_3D_free_PRISM(indata_param->win_size, 
				indata_param->src_size,
				 src_rect,
				 temp_size,
				shadow_rect, 
				indata_param->S_Prism_Side[current_num].In_3D_ver, 
				shadow_vers[current_num], 
				indata_param->S_Prism_Side[current_num].color_ptr, 
				indata_param->out_buff, 
				UPB_EYE_Z);
		}
	
	
	
}

#if 0
void  ConTrol_Points_BAK(MOUSE_INFO_T * Mouse,SIDE_INFO_T * Prism_Sides,IMGREF_3D_POINT_T * fix_centre,uint8 num)
{	
	int32 Vec_X=0,Vec_Y=0;
	int32 Angle_Period_Y=0,Angle_Period_X=0,Angle_Remain=0;
	int16 Max_Pixel_X=0,Max_Pixel_Y=0,Go_Over_Num=0,temp_num=0;
	uint32 i=0,j=0;
	
	uint16 * temporay_ptr_array[7]={0};
	Angle_Period_Y=450;
	Angle_Period_X=2000;
	Max_Pixel_X=320;
	Max_Pixel_Y=160;
	if(Mouse->is_mouse_Down)
		{
			Vec_Y=Mouse->Cur_Point.y-Mouse->Down_Point.y;
			Vec_X=Mouse->Cur_Point.x-Mouse->Down_Point.x;
			if((Vec_X)>Max_Pixel_X)
				{
					Angle_Period_X=Angle_Period_X;
				}
			else if(Vec_X<(-Max_Pixel_X))
				{
					Angle_Period_X=-Angle_Period_X;
				}
			else
				{
					Angle_Period_X=Angle_Period_X*Vec_X/Max_Pixel_X;
				}
			if(Angle_Period_X<0)
				{
				Angle_Period_X=3600+Angle_Period_X;
				}
			if((Vec_Y)>Max_Pixel_Y)
				{
					Angle_Period_Y=Angle_Period_Y;
				}
			else if(Vec_Y<(-Max_Pixel_Y))
				{
					Angle_Period_Y=-Angle_Period_Y;
				}
			else
				{
					Angle_Period_Y=Angle_Period_Y*Vec_Y/Max_Pixel_Y;
				}
			Go_Over_Num=Angle_Period_X*num/3600;
			if(Go_Over_Num>7)
			{
				Go_Over_Num=7;
			}
			Angle_Remain=Angle_Period_X-Go_Over_Num*3600/7;
			
			s_Angle_Y=Angle_Period_Y;
			if(Angle_Remain>=(1800/7))
				{
					Go_Over_Num++;
					Angle_Remain=Angle_Remain-3600/7;
				}
			s_Angle_X=Angle_Remain;	
			s_Focus_Num=(7+s_Start_F_Num-Go_Over_Num)%7;
		//	s_Focus_Num=(s_Focus_Num-Go_Over_Num)%7;
			//convert buffer
			
		}
	else if(Mouse->Is_back)
		{
			s_Angle_X=s_Angle_X+(0-s_Angle_X)*s2d_sin_new(900*Mouse->cur_frame/Mouse->total_frame)/65536;
			s_Angle_Y=s_Angle_Y+(0-s_Angle_Y)*s2d_sin_new(900*Mouse->cur_frame/Mouse->total_frame)/65536;
			
		}
	//根据全局的状态信息调整姿势
	for(i=0;i<num;i++)
			{
				temp_num=(s_Focus_Num+i)%7;
				temporay_ptr_array[i]=Prism_Sides[temp_num].color_ptr;
			}
			for(i=0;i<num;i++)
			{
				
				Prism_Sides[i].color_ptr=temporay_ptr_array[i];
			}
				for(i=0;i<num;i++)
			{
				for(j=0;j<4;j++)
					{
						
						Get_Rotation_Points(&(Prism_Sides[i].In_3D_ver[j]), 
											 s_Angle_X, 
											 s_Angle_Y,
											 (Prism_Sides[i].In_3D_ver[j]),
											 * fix_centre);
						
					}
			}
}
void Creat_Seven_Prism_BAK( SEVEN_PRISM_INPARAM_T * indata_param)
{	uint16 ro_angle=3600/7;
	IMGREF_3D_POINT_T First_vers [4]={0};
	int32 radius=0;
	int32 center_x=0,center_y=0,center_z=0;
	int16 src_wid=0;
	int16 src_height=0;
	
	DEEP_NUM_T d_num[7]={0};
	uint32 i=0,j=0;
	uint8 current_num=0,side_num=7;
	T_RECT src_rect={0};
	radius=indata_param->Prism_geometry.radius;
	center_x=indata_param->Prism_geometry.Ball_centre.x;
	center_y=indata_param->Prism_geometry.Ball_centre.y;
	center_z=indata_param->Prism_geometry.Ball_centre.z;
	src_wid=indata_param->src_size.w;
	src_height=indata_param->src_size.h;
	src_rect.x=0;
	src_rect.y=0;
	src_rect.w=src_wid;
	src_rect.h=src_height;
	//计算顶点
	First_vers[0].z=center_z-(radius*s2d_cos_new( ro_angle/2))/65536;
	First_vers[0].x=center_x-src_wid/2;
	First_vers[0].y=center_y-src_height/2;
	First_vers[1].z=First_vers[0].z;
	First_vers[1].x=center_x+src_wid/2;
	First_vers[1].y=center_y-src_height/2;
	First_vers[2].z=First_vers[0].z;
	First_vers[2].x=center_x+src_wid/2;
	First_vers[2].y=center_y+src_height/2;
	First_vers[3].z=First_vers[0].z;
	First_vers[3].x=center_x-src_wid/2;
	First_vers[3].y=center_y+src_height/2;
	for(i=0;i<4;i++)
		{
			
			Get_Rotation_Points(&(indata_param->S_Prism_Side[0].In_3D_ver[i]), 
										 indata_param->angle, 
										 0,
										First_vers[i],
										indata_param->Prism_geometry.Ball_centre);
			
		}
	for(i=1;i<side_num;i++)
		{
			for(j=0;j<4;j++)
				{
					
					Get_Rotation_Points(&(indata_param->S_Prism_Side[i].In_3D_ver[j]), 
										 ro_angle, 
										 0,
										 indata_param->S_Prism_Side[i-1].In_3D_ver[j],
										  indata_param->Prism_geometry.Ball_centre);
					
				}
			//indata_param->S_Prism_Side[i].color_ptr=indata_param->S_Prism_Side[i-1].color_ptr;
		}
	//
	ConTrol_Points( indata_param->M_Info_T_Ptr,
				indata_param->S_Prism_Side,  
				&indata_param->Prism_geometry.Ball_centre,
				side_num);
	//
	for(i=0;i<side_num;i++)
		{
			d_num[i].deep_z=indata_param->S_Prism_Side[i].In_3D_ver[0].z+indata_param->S_Prism_Side[i].In_3D_ver[1].z;
			d_num[i].deep_z=d_num[i].deep_z+indata_param->S_Prism_Side[i].In_3D_ver[2].z+indata_param->S_Prism_Side[i].In_3D_ver[3].z;
			d_num[i].deep_z=d_num[i].deep_z/4;
			d_num[i].num=i;
		}
	
	//sort through z
	Insert_Sorts( d_num,side_num);
	//sort through z
	for(i=0;i<7;i++)
		{	
			current_num=d_num[6-i].num;
			IMG_3D_free_PRISM(indata_param->win_size, 
				indata_param->src_size,
				 src_rect,
				indata_param->S_Prism_Side[current_num].In_3D_ver, 
				indata_param->S_Prism_Side[current_num].color_ptr, 
				indata_param->out_buff, 
				350);
		}
	
	
	
}
#endif
#endif
