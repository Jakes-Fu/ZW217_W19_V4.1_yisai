/*****************************************************************************
** File Name:      mmiuzone_se.c                                         *
** Author: kyle.jin         kyle                                              *
** Date:           21/12/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe uzone se                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011      kyle            Creat
******************************************************************************/
#include "mmi_app_uzone_trc.h"
#ifdef MMIUZONE_SUPPORT

#define _MMIUZONE_SE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "graphics_3d_rotate.h"
#include "mmiuzone_se.h"
#include "MMK_app.h"
#include "Guicommon.h"
#include "mmk_timer.h"
#include "mmk_tp.h"
#include "mmitheme_update.h"
#include "mmiuzone_position.h"
#include "mmiuzone_internal.h"
#include "Graphics_2d.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SE_ONE_ITEM_TOTAL_FRAME    40//每一个item转动总帧数为20帧
#define SE_OPEN_ANIM_TOTAL_FRAME   10//开始动画的总帧数
#define SE_FLING_ANIM_TOTAL_FRAME  30//TP UP后FLING的帧数
#define SE_RECOVER_ANIM_TOTAL_FRAME  10//TP UP后RECOVER的帧数
#define SE_TP_MOVE_MIN_PIX   4//判断TP MOVE 的最小距离
#define SE_VELOCITY_ADJUST_PARAM  300//速度转化为item num的系数
#define SE_MAX_MOVE_CIRCLE    3
#define SE_MAX_STEP_NUM 5
//Benny
#define SE_IDLE_ANIM_TIME 50
#define SE_LAYER_BUF_TYPE COLOR32
#define SE_SHINING_CALC_POINT_NUM (4096)


#define UZONE_BORDER_SHINING
#define UZONE_STAR_EFFECT


//Anson

#define STARTOTALNUM (14)
#define PAGE_NUM (5)
#define ANGLE_ONE  (0)


#define SHADOW_GAP (1)
#define BASE_ANGLE (1800)
//#define CALTIME

#ifdef UZONE_STAR_EFFECT
    #define STARNUM 5
#endif
#ifdef MAINLCD_SIZE_320X480

#define MMIUZONE_LCD_W 480
#define MMIUZONE_LCD_H 320
#define TOUCH_DEEP (110)
#define ANGLE_TWO (570)
#define MMIUZONE_SHININGZONE_X 4
#define MMIUZONE_SHININGZONE_Y 3
#define MMIUZONE_SHININGZONE_W (UZONE_ITEM_BG_WIDTH-10)
#define MMIUZONE_SHININGZONE_H (UZONE_ITEM_BG_HEIGHT-8)
#define MMIUZONE_SHININGZONE_JOINT_X 5
#define MMIUZONE_SHININGZONE_JOINT_Y 5
#define PAGE_GAP (10)


#elif defined MAINLCD_SIZE_240X400

#define MMIUZONE_LCD_W 400
#define MMIUZONE_LCD_H 240
#define ANGLE_TWO (600)
#define TOUCH_DEEP (0)
#define MMIUZONE_SHININGZONE_X 4
#define MMIUZONE_SHININGZONE_Y 4
#define MMIUZONE_SHININGZONE_W (UZONE_ITEM_BG_WIDTH-8)
#define MMIUZONE_SHININGZONE_H (UZONE_ITEM_BG_HEIGHT-12)
#define MMIUZONE_SHININGZONE_JOINT_X 5
#define MMIUZONE_SHININGZONE_JOINT_Y 5
#define PAGE_GAP (10)

#elif defined MAINLCD_SIZE_240X320
#if defined(MMI_PDA_SUPPORT)
#define MMIUZONE_LCD_W 320
#define MMIUZONE_LCD_H 240
#define ANGLE_TWO (600)
#define TOUCH_DEEP (0)
#define MMIUZONE_SHININGZONE_X 4
#define MMIUZONE_SHININGZONE_Y 4
#define MMIUZONE_SHININGZONE_W (UZONE_ITEM_BG_WIDTH-8)
#define MMIUZONE_SHININGZONE_H (UZONE_ITEM_BG_HEIGHT-12)
#define MMIUZONE_SHININGZONE_JOINT_X 5
#define MMIUZONE_SHININGZONE_JOINT_Y 5
#define PAGE_GAP (10)

#else
#define MMIUZONE_LCD_W 240
#define MMIUZONE_LCD_H 320
#define ANGLE_TWO (600)
#define TOUCH_DEEP (0)
#define MMIUZONE_SHININGZONE_X 4
#define MMIUZONE_SHININGZONE_Y 4
#define MMIUZONE_SHININGZONE_W (UZONE_ITEM_BG_WIDTH-8)
#define MMIUZONE_SHININGZONE_H (UZONE_ITEM_BG_HEIGHT-12)
#define MMIUZONE_SHININGZONE_JOINT_X 5
#define MMIUZONE_SHININGZONE_JOINT_Y 5
#define PAGE_GAP (6)

#endif
#else 
#define MMIUZONE_LCD_W 400
#define MMIUZONE_LCD_H 240
#define ANGLE_TWO (600)
#define TOUCH_DEEP (0)
#define MMIUZONE_SHININGZONE_X 4
#define MMIUZONE_SHININGZONE_Y 4
#define MMIUZONE_SHININGZONE_W (UZONE_ITEM_BG_WIDTH-8)
#define MMIUZONE_SHININGZONE_H (UZONE_ITEM_BG_HEIGHT-12)
#define MMIUZONE_SHININGZONE_JOINT_X 5
#define MMIUZONE_SHININGZONE_JOINT_Y 5
#define PAGE_GAP (6)
#endif

#define MMIUZONE_DESIGN_BASE_LCD_W 400
#define MMIUZONE_DESIGN_BASE_LCD_H 240
/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/
typedef struct
{
	IMGREF_SIZE_T bg_win_size;
	IMGREF_SIZE_T page_size;
	uint32 *out_buff_ptr;
	uint32 *page_src_buff_ptr;
	uint32 *icon_src_buff_ptr;
	uint32 *str_src_buff_ptr;
	uint32 *str_shadow_buff_ptr;
}SE_CALC_PARAM_T;

typedef struct{
	COLOR32* ori_glass_buf;
	COLOR32* shining_use_buf;
	COLOR32* shadow_use_buf;
	FIX16_POINT_T* Point_Add_ptr;
    COLOR32* big_star_buf;
    COLOR32* small_star_buf;
    COLOR32* icon_buffer;
    IMGREF_RECT_T *icon_rect;
    COLOR32* Str_buffer;
    IMGREF_RECT_T *Str_rect;
    COLOR32* Str_Shadow_buffer;
    IMGREF_RECT_T *Str_Shadow_rect;
} S3D_UZONE_EXPANDPTR_T;

typedef struct
{
	SE_CALC_PARAM_T             calc_param_info;
    MMK_TP_SLIDE_E              slide_state;        //特效状态
    MMI_TP_STATUS_E             tp_status;          //TP STATUS 
    GUI_POINT_T                 slide_start_point;  //记录滑动开始点                     
    GUI_POINT_T                 slide_pre_point;    //记录滑动前一个点                     
    MMI_HANDLE_T                handle;				//窗口HANDLE
    float                       fling_velocity_x;   //x方向上的速度
    float                       fling_velocity_y;   //y方向上的速度
	uint16                      current_frame;		//当前帧数
	uint16                      pre_frame;          //上一帧
	uint16                       slide_item_num;		//滑动几个item
	uint16 						cur_select_item;     //当前选中的item
	uint16                      last_select_item;		//记录前一个选中的item
	uint16                      fling_recover_start_frame;
	uint16                      total_item_num;
    uint8                       anim_timer_id;      //刚进入时的动画timer
    uint8                       slide_timer_id;		//TP 滑动的timer
    uint8                       fling_timer_id;     //TP UP 后的timer
    uint8                       recover_timer_id;   //恢复的timer
    uint8                       idle_anim_timer_id;   //空闲时间动画的timer
    BOOLEAN                 	is_landscape;       //是否横屏
	BOOLEAN                     is_in_anim;    //是否在动画
	BOOLEAN                     is_in_idle_anim;    //是否在动画
	BOOLEAN                     direction;//顺时针为FALSE    逆时针为TRUE
	BOOLEAN                     is_init;
}SE_SLIDE_STATE_T;

#ifdef UZONE_STAR_EFFECT
typedef struct {
	IMGREF_RECT_T location; 
	BOOLEAN is_big;
	uint32 display_frame;
      uint32 speed_frame;   //每割几帧闪一次
      uint32 total_frame_num;
	}STAR_INFO_T;
/*
STAR_INFO_T s_star_collection[7]={	{{153,12,22,23},TRUE,0},
									{{181,26,16,17},FALSE,0},
									{{129,34,16,17},FALSE,0},
									{{165,67,22,23},TRUE,0},
									{{213,153,22,23},TRUE,0},
									{{236,180,16,17},FALSE,0},
									{{218,198,16,17},FALSE,0},
									};
									*/
STAR_INFO_T s_star_collection[STARTOTALNUM]={	
                                    {{169,6,UZONE_BIG_STAR_WIDTH,UZONE_BIG_STAR_HEIGHT},TRUE,0,0,0},
									{{185,29,UZONE_BIG_STAR_WIDTH,UZONE_BIG_STAR_HEIGHT},TRUE,0,0,0},
									{{332,138,UZONE_BIG_STAR_WIDTH,UZONE_BIG_STAR_HEIGHT},TRUE,0,0,0},
									{{165,69,UZONE_BIG_STAR_WIDTH,UZONE_BIG_STAR_HEIGHT},TRUE,0,0,0},
									{{95,20,UZONE_BIG_STAR_WIDTH,UZONE_BIG_STAR_HEIGHT},TRUE,0,0,0},
									{{316,29,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{280,20,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{206,20,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{218,148,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{51,144,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{57,32,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{119,150,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{45,32,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									{{145,32,UZONE_SMALL_STAR_WIDTH,UZONE_SMALL_STAR_HEIGHT},FALSE,0,0,0},
									};


#endif

LOCAL ROTA_DATA_T s_page_axis_array[PAGE_NUM]={
	{ANGLE_ONE+BASE_ANGLE,0,0,0,0},
	{-ANGLE_TWO+BASE_ANGLE,0,0,0,0},
	{ANGLE_ONE+BASE_ANGLE,0,0,0,0},
	{ANGLE_TWO+BASE_ANGLE,0,0,0,0},
	{ANGLE_ONE+BASE_ANGLE,0,0,0,0},
	
	};

/**--------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
extern   int s2d_sin(int angle);
extern  int s2d_cos(int angle);
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
LOCAL SE_SLIDE_STATE_T s_multi_se_info = {0};

LOCAL GUI_LCD_DEV_INFO* s_uzone_calc_layer_handle = PNULL; /*Jessica*/

LOCAL SE_ITEM_CALLBACK s_press_item_callback_ptr = NULL;

//LOCAL uint16  s_uzone_total_item = 0;
LOCAL uint16  s_uzone_cur_item = 0;
//ANSON
LOCAL uint32 * s_small_star = PNULL;
LOCAL uint32 * s_big_star = PNULL;
//BENNY
LOCAL uint8* s_uzone_alloc_mem_ptr = PNULL;
LOCAL SE_LAYER_BUF_TYPE* s_Ori_Pic_buf = PNULL;
LOCAL SE_LAYER_BUF_TYPE* s_shining_use_buf = PNULL;//[SE_UZONE_SHININGSIZE_H*SE_UZONE_SHININGSIZE_W] = {0};
LOCAL SE_LAYER_BUF_TYPE* s_shadow_use_buf = PNULL;
LOCAL FIX16_POINT_T* s_Point_Add_ptr = PNULL;//[4096] = {0};


LOCAL SE_DRAWITEM_CALLBACK s_update_mid_glass_centent_fptr = PNULL;

IMGREF_RECT_T s_icon_rect={0};
IMGREF_RECT_T s_str_rect={0};
IMGREF_RECT_T s_str_shadow_rect={0};


/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 初始化参数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_InitCalcParam(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : 重置参数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_DestroyCalcParam(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : Stop TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_StopTimer(SE_SLIDE_STATE_T *multi_se_info,uint8* timer_id_ptr);

/*****************************************************************************/
// Description : start TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_StartTimer(SE_SLIDE_STATE_T *multi_se_info,uint8* timer_id_ptr);


/*****************************************************************************/
// Description : 画一帧数据
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_DrawOneFrame(SE_SLIDE_STATE_T *multi_se_info,uint16 current_frame,uint16 total_frame);

/*****************************************************************************/
// Description : 画并刷新一帧数据
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_DrawAndUpdateOneFrame(SE_SLIDE_STATE_T *multi_se_info,uint16 current_frame,uint16 total_frame);

/*****************************************************************************/
// Description : 动画timer处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleAnimTimerMsg(SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : TP 滑动 处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_SlideMoveAnim(SE_SLIDE_STATE_T *multi_se_info,GUI_POINT_T  *tp_point_ptr);

/*****************************************************************************/
// Description : 滑动TIMER处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleSlideTimerMsg(SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : FLING模式下处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleFlingTimerMsg(SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : 恢复状态处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleRecoverTimerMsg(SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : OPEN 处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleOpenWinMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : SE_HandleWinGetFocusMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinGetFocusMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : SE_HandleWinLoseFocusMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinLoseFocusMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : SE_HandleCloseWinMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleCloseWinMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : SE_HandleWinFullPaintMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinFullPaintMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : SE_HandleTPDownMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPDownMsg(MMI_WIN_ID_T win_id,GUI_POINT_T *tp_point_ptr,SE_SLIDE_STATE_T *multi_se_info);


/*****************************************************************************/
// Description : SE_HandleTPMoveMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPMoveMsg(MMI_WIN_ID_T win_id,
								GUI_POINT_T *tp_point_ptr,
								SE_SLIDE_STATE_T *multi_se_info
								);


/*****************************************************************************/
// Description : SE_HandleTPUpMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPUpMsg(MMI_WIN_ID_T win_id,
							GUI_POINT_T *tp_point_ptr,
							SE_SLIDE_STATE_T *multi_se_info
							);


/*****************************************************************************/
// Description : SE_HandleKeyUpMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyUpMsg(MMI_WIN_ID_T win_id,
							SE_SLIDE_STATE_T *multi_se_info
							);

/*****************************************************************************/
// Description : SE_HandleKeyDownMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyDownMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								);

/*****************************************************************************/
// Description : SE_HandleKeyLeftMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyLeftMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								);


/*****************************************************************************/
// Description : SE_HandleKeyRightMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyRightMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								);

/*****************************************************************************/
// Description : SE_HandleAppOKMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleAppOKMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								);

/*****************************************************************************/
// Description : SE_HandleWinTimerMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleWinTimerMsg(MMI_WIN_ID_T win_id,
								DPARAM  param,
								SE_SLIDE_STATE_T *multi_se_info);

/*****************************************************************************/
// Description : SE_HandleWinLCDSwitch
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinLCDSwitch(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								);

/*****************************************************************************/
//  Description : SE_Update_Glass_Centent
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SE_Update_Glass_Centent(void);

/*****************************************************************************/
//  Description : Slim_Landscape_Mode
//  Global resource dependence : none
//  Author:Anson,Benny
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Landscape_Mode(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr);

/*****************************************************************************/
//  Description : Slim_Portrait_Mode
//  Global resource dependence : none
//  Author:Anson
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Portrait_Mode(IMGREF_SIZE_T bg_win_size,
								COLOR32 * out_buff_ptr, 
								IMGREF_SIZE_T page_size,
								COLOR32 * page_buff_array,
								int page_num,
								int start_location,
								int cur_frame_num,
								int total_frame_num,
								BOOLEAN Is_go_up);

/*****************************************************************************/
//  Description : Get_Page_Num
//  Global resource dependence : none
//  Author:Anson
//  Note:
/*****************************************************************************/
LOCAL int32 Get_Page_Num(IMGREF_POINT_T click_p,
							IMGREF_SIZE_T lcd_size,//屏幕大小。竖屏时240320的话横屏时320 240.
							IMGREF_SIZE_T page_size,//输入图片大小，横屏时带阴影的图片大小
							BOOLEAN Is_Landmode,
							uint32* bg_buff
							);
LOCAL BOOLEAN Slim_Landscape_Mode_New(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr);
LOCAL void Get_Buffer(IMGREF_SIZE_T page_size, //图片大小，不带阴影的图片大小
                                        IMGREF_SIZE_T shadow_size,    
                                        COLOR32 * pColor_in, 			//输入图片buffer
                                        COLOR32 * tempSrcColor_reverse,
                                        COLOR32 * icon_buffer,
                                        IMGREF_RECT_T * icon_rect,
                                        COLOR32 *  Str_buffer,
                                        IMGREF_RECT_T * Str_rect,
                                        COLOR32 * Str_Shadow_buffer,
                                        IMGREF_RECT_T * Str_Shadow_rect ,
                                        uint32 icon_num);
/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN SE_UZone_ReleaseMemory(void)
{

    if(PNULL != s_uzone_alloc_mem_ptr)
    {
        s_shining_use_buf = PNULL;    
        s_Point_Add_ptr = PNULL;
		s_shadow_use_buf = PNULL;
        SCI_FREE(s_uzone_alloc_mem_ptr);
		s_uzone_alloc_mem_ptr = PNULL;
    }
    return TRUE;

}

/*****************************************************************************/
//  Description :申请存放BUF
//  Global resource dependence : 
//  Author: Benny.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SE_UZone_CreatMemory(SE_SLIDE_STATE_T *multi_se_info)
{

    uint8* next_ptr = PNULL;
    uint32 alloc_size = 0;
    
    if(PNULL != s_uzone_alloc_mem_ptr)
    {
        SE_UZone_ReleaseMemory();
    }
    
// border shining bak buf size
    alloc_size = multi_se_info->calc_param_info.page_size.w*multi_se_info->calc_param_info.page_size.h*sizeof(SE_LAYER_BUF_TYPE);
    alloc_size += UZONE_ITEM_BG_WIDTH*UZONE_ITEM_BG_HEIGHT*sizeof(SE_LAYER_BUF_TYPE);//plus 1 for enough buf
	alloc_size += SE_SHINING_CALC_POINT_NUM*sizeof(FIX16_POINT_T);
//malloc mem
    s_uzone_alloc_mem_ptr = SCI_ALLOCA(alloc_size);
	//SCI_ASSERT(PNULL != s_uzone_alloc_mem_ptr);
    if(PNULL == s_uzone_alloc_mem_ptr)
    {
    	s_shining_use_buf = PNULL;    
        s_Point_Add_ptr = PNULL;
		s_shadow_use_buf = PNULL;
        return FALSE;
    }
    SCI_MEMSET(s_uzone_alloc_mem_ptr, 0,alloc_size);

//set every ptr
    next_ptr = s_uzone_alloc_mem_ptr;
    s_shadow_use_buf = (SE_LAYER_BUF_TYPE*)next_ptr;
    next_ptr += multi_se_info->calc_param_info.page_size.w*multi_se_info->calc_param_info.page_size.h*sizeof(SE_LAYER_BUF_TYPE);
    s_shining_use_buf = (SE_LAYER_BUF_TYPE*)next_ptr;
	next_ptr += UZONE_ITEM_BG_WIDTH*UZONE_ITEM_BG_HEIGHT*sizeof(SE_LAYER_BUF_TYPE);
	
	s_Point_Add_ptr = (FIX16_POINT_T*)next_ptr;
    next_ptr += SE_SHINING_CALC_POINT_NUM*sizeof(FIX16_POINT_T);

    return TRUE;
    
}


/*****************************************************************************/
//  Description : set callback function for one item is clicked
//  Global resource dependence : none
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetItemCallback(SE_ITEM_CALLBACK callback_ptr)
{
    if (NULL == callback_ptr || NULL == *callback_ptr)
    {
        //SCI_TRACE_LOW:"SEAPI_SetItemCallback failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_305_112_2_18_3_4_24_47,(uint8*)"");
        return;
    }
	s_press_item_callback_ptr = callback_ptr;
}
/*****************************************************************************/
// Description : 初始化参数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_InitCalcParam(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
	IMGREF_SIZE_T bg_win_size = {0,0};
	LCD_ANGLE_E cur_lcd_angle = GUILCD_GetLogicAngle(MAIN_LCD_ID);

	if(s_uzone_calc_layer_handle->block_id == UILAYER_NULL_HANDLE)
	{
		return;
	}
	//multi_se_info->total_item_num = s_uzone_total_item;
	multi_se_info->current_frame = 0;
	multi_se_info->handle = (MMI_HANDLE_T)win_id;
	multi_se_info->cur_select_item = SEAPI_GetCurItemIndex();
    if ((LCD_ANGLE_0 == cur_lcd_angle) || (LCD_ANGLE_180 == cur_lcd_angle))
    {
        multi_se_info->is_landscape = FALSE;
    }
    else
    {
        multi_se_info->is_landscape = TRUE;
    }
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &bg_win_size.w, &bg_win_size.h);


	multi_se_info->calc_param_info.bg_win_size.w = bg_win_size.w;
	multi_se_info->calc_param_info.bg_win_size.h = bg_win_size.h;
	multi_se_info->calc_param_info.out_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(s_uzone_calc_layer_handle);
	multi_se_info->calc_param_info.page_size.w = UZONE_ITEM_BG_WIDTH;
	multi_se_info->calc_param_info.page_size.h = UZONE_ITEM_BG_HEIGHT + UZONE_ITEM_SHADOW_HEIGHT_H;

    if(PNULL == multi_se_info->calc_param_info.page_src_buff_ptr
        ||PNULL == multi_se_info->calc_param_info.out_buff_ptr
        )
    {
        return;
    }
    SE_UZone_CreatMemory(multi_se_info);
	multi_se_info->is_init = TRUE;
}
/*****************************************************************************/
// Description : 重置参数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_DestroyCalcParam(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{	
	SCI_MEMSET(multi_se_info,0,sizeof(SE_SLIDE_STATE_T));
}
/*****************************************************************************/
// Description : Stop TP MOVE timer
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_StopTimer(SE_SLIDE_STATE_T *multi_se_info,uint8* timer_id_ptr)
{
    if (PNULL != multi_se_info && 0 != *timer_id_ptr)
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
LOCAL void SE_StartTimer(SE_SLIDE_STATE_T *multi_se_info,uint8* timer_id_ptr)
{
    if (PNULL != multi_se_info)
    {
        if(0 != *timer_id_ptr)
        {
            SE_StopTimer(multi_se_info,timer_id_ptr);
        }
		
        *timer_id_ptr = MMK_CreateWinTimer(
            multi_se_info->handle, 
            10,
            FALSE );
    }
}

/*****************************************************************************/
// Description : start timer
// Global resource dependence : 
// Author: Benny.LIU
// Note: 
/*****************************************************************************/
LOCAL void SE_Start_Conf_Timer(SE_SLIDE_STATE_T *multi_se_info,
							uint8* timer_id_ptr,
							uint32  time_out,   //Specifies the expire value in milliseconds
                            BOOLEAN is_period   //the period timer if is_period is TRUE
                        )
{
    if (PNULL != multi_se_info)
    {
        if(0 != *timer_id_ptr)
        {
            SE_StopTimer(multi_se_info,timer_id_ptr);
        }
		
        *timer_id_ptr = MMK_CreateWinTimer(
            multi_se_info->handle, 
            time_out,
            is_period );
    }
}
/*****************************************************************************/
// Description : 告知算法需要的目的BUF
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void SEAPI_SetDestLayerHandle(GUI_LCD_DEV_INFO* des_layer)
{
    if(PNULL == des_layer)
    {
        return;
    }
    s_uzone_calc_layer_handle = des_layer;
}

/*****************************************************************************/
// Description : 告知算法需要的源BUF
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

PUBLIC void SEAPI_SetSrcLayerHandle(GUI_LCD_DEV_INFO* page_src_layer,
                                            GUI_LCD_DEV_INFO* icon_src_layer,
                                            GUI_LCD_DEV_INFO* str_src_layer,
                                            GUI_LCD_DEV_INFO* str_shadow_layer
                                            )
{
    s_multi_se_info.calc_param_info.page_src_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(page_src_layer);
    s_multi_se_info.calc_param_info.icon_src_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(icon_src_layer);
    s_multi_se_info.calc_param_info.str_src_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(str_src_layer);
    s_multi_se_info.calc_param_info.str_shadow_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(str_shadow_layer);
}

/*****************************************************************************/
// Description : 告知算法总的ITEM NUM
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

PUBLIC void SEAPI_SetItemNumber(uint16 item_number)
{
	//s_uzone_total_item = item_number;
	s_multi_se_info.total_item_num = item_number;
}

/*****************************************************************************/
// Description : 设置当前选中的ITEM，也就是中间的ITEM
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void SEAPI_SetCurItemIndex(uint16 item_index)
{
    if(item_index>=s_multi_se_info.total_item_num)
    {
        item_index = 0;
    }
    
	s_multi_se_info.cur_select_item = item_index;
}

/*****************************************************************************/
// Description : 获取当前选中的ITEM
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC uint16 SEAPI_GetCurItemIndex(void)
{
	return s_multi_se_info.cur_select_item;
}

/*****************************************************************************/
// Description : 设置前一个选中的ITEM，也就是中间的ITEM
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void SEAPI_SetLastItemIndex(uint16 item_index)
{
	s_multi_se_info.last_select_item = item_index;
}

/*****************************************************************************/
// Description : 获取前一个选中的ITEM
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC uint16 SEAPI_GetLastItemIndex()
{
	return s_multi_se_info.last_select_item;
}
/*****************************************************************************/
// Description : 画一帧数据
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_DrawOneFrame(SE_SLIDE_STATE_T *multi_se_info,uint16 current_frame,uint16 total_frame)
{
	int start_location = 0;
	int cur_frame_num = current_frame;
	int total_frame_num = total_frame;
	// Benny add
	S3D_UZONE_EXPANDPTR_T exptr = {0};
    
	if(PNULL==s_Point_Add_ptr
		||PNULL==s_shining_use_buf
		||PNULL==s_shadow_use_buf
		||PNULL==s_Ori_Pic_buf
		||PNULL == s_uzone_alloc_mem_ptr
		||PNULL == s_big_star
        ||PNULL == s_small_star
		)
	{
		return;
	}
    
	exptr.Point_Add_ptr = s_Point_Add_ptr;
	exptr.shining_use_buf = s_shining_use_buf;
	exptr.shadow_use_buf = s_shadow_use_buf;
	exptr.ori_glass_buf = s_Ori_Pic_buf;
    exptr.big_star_buf = s_big_star;
    exptr.small_star_buf = s_small_star;

    if(PNULL == multi_se_info)
    {
        return;
    }

    start_location = (SEAPI_GetCurItemIndex()+multi_se_info->total_item_num-2)%multi_se_info->total_item_num;

    //add
    exptr.icon_buffer=multi_se_info->calc_param_info.icon_src_buff_ptr;
    exptr.icon_rect=&s_icon_rect;
    exptr.Str_buffer=multi_se_info->calc_param_info.str_src_buff_ptr;
    exptr.Str_rect=&s_str_rect;
    exptr.Str_Shadow_buffer=multi_se_info->calc_param_info.str_shadow_buff_ptr;
    exptr.Str_Shadow_rect=&s_str_shadow_rect;
    //add

    if(current_frame>total_frame)
    {
        return;
    }
    
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &multi_se_info->calc_param_info.bg_win_size.w, &multi_se_info->calc_param_info.bg_win_size.h);

    
	multi_se_info->calc_param_info.out_buff_ptr = (uint32*)UILAYER_GetLayerBufferPtr(s_uzone_calc_layer_handle);
	multi_se_info->calc_param_info.page_size.w = UZONE_ITEM_BG_WIDTH;
	multi_se_info->calc_param_info.page_size.h = UZONE_ITEM_BG_HEIGHT + UZONE_ITEM_SHADOW_HEIGHT_H;

    if(PNULL == multi_se_info->calc_param_info.page_src_buff_ptr
        ||PNULL == multi_se_info->calc_param_info.out_buff_ptr
        )
    {
        return;
    }
    
    SCI_MEMSET(multi_se_info->calc_param_info.out_buff_ptr,0,multi_se_info->calc_param_info.bg_win_size.w*multi_se_info->calc_param_info.bg_win_size.h*sizeof(uint32));

	//SCI_TRACE_LOW:"[MMIUZONE] SE_DrawOneFrame direction = %d,start_location = %d,cur_frame_num = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_491_112_2_18_3_4_24_48,(uint8*)"ddd",multi_se_info->direction,start_location,cur_frame_num);
	

        Slim_Landscape_Mode_New(multi_se_info->calc_param_info.bg_win_size,
							multi_se_info->calc_param_info.out_buff_ptr,
							multi_se_info->calc_param_info.page_size,
							multi_se_info->calc_param_info.page_src_buff_ptr,
							multi_se_info->total_item_num,
							start_location,
							cur_frame_num,
							total_frame_num,
							multi_se_info->direction,
							multi_se_info->is_in_idle_anim,
							&exptr
							);


}
/*****************************************************************************/
// Description : 画并刷新一帧数据
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_DrawAndUpdateOneFrame(SE_SLIDE_STATE_T *multi_se_info,uint16 current_frame,uint16 total_frame)
{
	IMGREF_SIZE_T bg_win_size = {0};
	GUI_RECT_T update_rect = {0};

    if(!multi_se_info->is_init)
    {
        return;
    }
    
	//计算
	SE_DrawOneFrame(multi_se_info,current_frame,total_frame);

	#ifdef WIN32
	SCI_Sleep(50);
	#endif
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &bg_win_size.w, &bg_win_size.h);
	
	update_rect.right = bg_win_size.w - 1;
	update_rect.bottom = bg_win_size.h - 1;
	
	//置脏
	MMITHEME_StoreUpdateRect(s_uzone_calc_layer_handle, update_rect);
}

/*****************************************************************************/
// Description : 动画timer处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleAnimTimerMsg(SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 current_frame = 0;//在总帧数中的实际帧数
	uint16 cur_item_index = SEAPI_GetCurItemIndex();
		
    multi_se_info->is_in_idle_anim = FALSE;
	if(multi_se_info->current_frame< SE_OPEN_ANIM_TOTAL_FRAME)
	{
		current_frame = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,
								        (int)SE_ONE_ITEM_TOTAL_FRAME,
								        (int)SE_OPEN_ANIM_TOTAL_FRAME, 
								        (int)multi_se_info->current_frame
								        );
		//最后一帧，切换当前选择的item
		if(current_frame == SE_ONE_ITEM_TOTAL_FRAME)
		{
			if(multi_se_info->direction)
			{
				if((cur_item_index+1)<multi_se_info->total_item_num)
				{
					cur_item_index = cur_item_index+1;
				}
				else
				{
					cur_item_index = 0;
				}
				
				SEAPI_SetCurItemIndex(cur_item_index);
				current_frame = 0;
			}
			else
			{
				if(cur_item_index==0)
				{
					cur_item_index = multi_se_info->total_item_num-1;
				}
				else
				{
					cur_item_index = cur_item_index-1;
				}
				
				SEAPI_SetCurItemIndex(cur_item_index);
				current_frame = 0;
			}
			
			multi_se_info->current_frame = SE_OPEN_ANIM_TOTAL_FRAME;
		}
		else
		{
			multi_se_info->current_frame++;
		}
		SE_DrawAndUpdateOneFrame(multi_se_info,current_frame,SE_ONE_ITEM_TOTAL_FRAME);
	}
    
	if(multi_se_info->current_frame >= SE_OPEN_ANIM_TOTAL_FRAME)
	{
		multi_se_info->is_in_anim = FALSE;
		multi_se_info->current_frame = 0;
        multi_se_info->is_in_idle_anim = TRUE;
        SE_Update_Glass_Centent();
		SE_Start_Conf_Timer(multi_se_info, &multi_se_info->idle_anim_timer_id,SE_IDLE_ANIM_TIME,FALSE);
	}

	
}
/*****************************************************************************/
// Description : TP 滑动 处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_SlideMoveAnim(SE_SLIDE_STATE_T *multi_se_info,GUI_POINT_T  *tp_point_ptr)
{
	uint16 cur_item_index = SEAPI_GetCurItemIndex();
	uint16 cur_frame = 0;
	int16  change_item = 0;
	
	//判断方向
	if(multi_se_info->is_landscape)
	{
		if(tp_point_ptr->x - multi_se_info->slide_start_point.x>0)
		{
			multi_se_info->direction = FALSE;
		}
		else
		{
			multi_se_info->direction = TRUE;
		}
		//计算帧数
		cur_frame= (abs(tp_point_ptr->x - multi_se_info->slide_start_point.x)/SE_TP_MOVE_MIN_PIX);
	}
	else
	{
		if(tp_point_ptr->y - multi_se_info->slide_start_point.y>0)
		{
			multi_se_info->direction = FALSE;
		}
		else
		{
			multi_se_info->direction = TRUE;
		}
		//计算帧数
		cur_frame = (abs(tp_point_ptr->y - multi_se_info->slide_start_point.y)/SE_TP_MOVE_MIN_PIX);
	}
	
	change_item = (cur_frame/SE_ONE_ITEM_TOTAL_FRAME - multi_se_info->current_frame/SE_ONE_ITEM_TOTAL_FRAME);

	if(abs(change_item)>0)
	{
		if(multi_se_info->direction)
		{
			cur_item_index = (cur_item_index+SE_MAX_MOVE_CIRCLE*multi_se_info->total_item_num+change_item)%multi_se_info->total_item_num;
			SEAPI_SetCurItemIndex(cur_item_index);
			multi_se_info->current_frame = cur_frame;
			cur_frame = cur_frame%SE_ONE_ITEM_TOTAL_FRAME;
		}
		else
		{
			cur_item_index = (cur_item_index+SE_MAX_MOVE_CIRCLE*multi_se_info->total_item_num-change_item)%multi_se_info->total_item_num;
			SEAPI_SetCurItemIndex(cur_item_index);
			multi_se_info->current_frame = cur_frame;
			cur_frame = cur_frame%SE_ONE_ITEM_TOTAL_FRAME;
		}
	}
	else
	{
		multi_se_info->current_frame = cur_frame;
		cur_frame = cur_frame%SE_ONE_ITEM_TOTAL_FRAME;
	}

	SE_DrawAndUpdateOneFrame(multi_se_info,cur_frame,SE_ONE_ITEM_TOTAL_FRAME);
}
/*****************************************************************************/
// Description : 滑动TIMER处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleSlideTimerMsg(SE_SLIDE_STATE_T *multi_se_info)
{
    GUI_POINT_T  point = {0};
    GUI_POINT_T  cur_tp_point = {0};
    MMI_TP_STATUS_E state = MMI_TP_MOVE;
    
    #ifdef TOUCH_PANEL_SUPPORT
    MMK_GetLogicTPMsg(&state, &cur_tp_point);
    #endif
    
    if(multi_se_info->slide_state == MMK_TP_SLIDE_NONE
        &&(multi_se_info->tp_status == MMI_TP_DOWN)
        )
    {
        if(multi_se_info->is_landscape)
        {
            if(abs(cur_tp_point.x-multi_se_info->slide_pre_point.x)>SE_TP_MOVE_MIN_PIX)
            {
                multi_se_info->slide_state = MMK_TP_SLIDE_TP_SCROLL;
                multi_se_info->tp_status = MMI_TP_MOVE;
                multi_se_info->current_frame = 0;
            }
        }
        else
        {
            if(abs(cur_tp_point.y-multi_se_info->slide_pre_point.y)>SE_TP_MOVE_MIN_PIX)
            {
                multi_se_info->slide_state = MMK_TP_SLIDE_TP_SCROLL;
                multi_se_info->tp_status = MMI_TP_MOVE;
                multi_se_info->current_frame = 0;
            }
        }
    }
    
	SE_StartTimer(multi_se_info, &multi_se_info->slide_timer_id);
    
	if(multi_se_info->slide_state == MMK_TP_SLIDE_TP_SCROLL
     &&(abs(cur_tp_point.x-multi_se_info->slide_pre_point.x)>SE_TP_MOVE_MIN_PIX&&multi_se_info->is_landscape)
        )
    {        
        if(abs(cur_tp_point.x-multi_se_info->slide_pre_point.x)>(SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX))
        {
            if(cur_tp_point.x>multi_se_info->slide_pre_point.x)
            {
                point.x = multi_se_info->slide_pre_point.x + SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX;
                point.y = cur_tp_point.y;
            }
            else
            {
                point.x = multi_se_info->slide_pre_point.x - SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX;
                point.y = cur_tp_point.y;
            }
        }
        else
        {
                point.x = cur_tp_point.x;
                point.y = cur_tp_point.y;
        }
        
	    multi_se_info->is_in_idle_anim = FALSE;
		// 增加速度点
	    MMK_AddVelocityItem(cur_tp_point.x, cur_tp_point.y);	
		
		SE_SlideMoveAnim(multi_se_info,&point);
		
		multi_se_info->slide_pre_point.x = point.x;
		multi_se_info->slide_pre_point.y = point.y;
	}
    else if(multi_se_info->slide_state == MMK_TP_SLIDE_TP_SCROLL
     &&(abs(cur_tp_point.y-multi_se_info->slide_pre_point.y)>SE_TP_MOVE_MIN_PIX&&!multi_se_info->is_landscape)
        )
    {
        if(abs(cur_tp_point.y-multi_se_info->slide_pre_point.y)>(SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX))
        {
            if(cur_tp_point.y>multi_se_info->slide_pre_point.y)
            {
                point.x = cur_tp_point.x;
                point.y = multi_se_info->slide_pre_point.y + SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX;
            }
            else
            {
                point.x = cur_tp_point.x;
                point.y = multi_se_info->slide_pre_point.y - SE_MAX_STEP_NUM*SE_TP_MOVE_MIN_PIX;
            }
        }
        else
        {
                point.x = cur_tp_point.x;
                point.y = cur_tp_point.y;
        }
        
	    multi_se_info->is_in_idle_anim = FALSE;
		// 增加速度点
	    MMK_AddVelocityItem(cur_tp_point.x, cur_tp_point.y);	
		
		SE_SlideMoveAnim(multi_se_info,&point);
		
		multi_se_info->slide_pre_point.x = point.x;
		multi_se_info->slide_pre_point.y = point.y;
    }
}
/*****************************************************************************/
// Description : 空闲动画TIMER处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleIdleAnimTimerMsg(SE_SLIDE_STATE_T *multi_se_info)
{
	SE_DrawAndUpdateOneFrame(multi_se_info,0,SE_ONE_ITEM_TOTAL_FRAME);
}
/*****************************************************************************/
// Description : FLING模式下处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleFlingTimerMsg(SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 current_frame = 0;//在总帧数中的实际帧数
	int16	                iLen;			//运动总位移
	int32                  iTotalFrame;	//总帧数
	int32                  iNowFrameNum;	//当前帧
	uint16 change_item =0;
	uint16 cur_item_index = SEAPI_GetCurItemIndex();
	
	iLen = multi_se_info->slide_item_num*SE_ONE_ITEM_TOTAL_FRAME + SE_ONE_ITEM_TOTAL_FRAME - multi_se_info->fling_recover_start_frame;
	iTotalFrame = MIN(SE_FLING_ANIM_TOTAL_FRAME,iLen);
	iNowFrameNum = multi_se_info->current_frame ;
    multi_se_info->is_in_idle_anim = FALSE;
	
	if(multi_se_info->current_frame< iTotalFrame)
	{

		current_frame = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,
										        iLen,
										        iTotalFrame, 
										        iNowFrameNum
										        );

		change_item = abs((multi_se_info->fling_recover_start_frame+current_frame)/SE_ONE_ITEM_TOTAL_FRAME-(multi_se_info->fling_recover_start_frame+multi_se_info->pre_frame)/SE_ONE_ITEM_TOTAL_FRAME);
		multi_se_info->pre_frame = current_frame;

		if(multi_se_info->direction)
		{
			cur_item_index = (cur_item_index+SE_MAX_MOVE_CIRCLE*multi_se_info->total_item_num+change_item)%multi_se_info->total_item_num;
		}
		else
		{
			cur_item_index = (cur_item_index+SE_MAX_MOVE_CIRCLE*multi_se_info->total_item_num-change_item)%multi_se_info->total_item_num;
		}

		SEAPI_SetCurItemIndex(cur_item_index);

		current_frame = (multi_se_info->fling_recover_start_frame+current_frame)%SE_ONE_ITEM_TOTAL_FRAME;

		SE_DrawAndUpdateOneFrame(multi_se_info,current_frame,SE_ONE_ITEM_TOTAL_FRAME);
		multi_se_info->current_frame++;
		
	}
	else
	{
		multi_se_info->slide_state = MMK_TP_SLIDE_NONE;
		multi_se_info->tp_status = MMI_TP_NONE;
		multi_se_info->current_frame = 0;
        multi_se_info->pre_frame = 0;
        
        multi_se_info->is_in_idle_anim = TRUE;
        SE_Update_Glass_Centent();
		SE_Start_Conf_Timer(multi_se_info, &multi_se_info->idle_anim_timer_id,SE_IDLE_ANIM_TIME,FALSE);
	}

}

/*****************************************************************************/
// Description : 恢复状态处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleRecoverTimerMsg(SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 current_frame = 0;//在总帧数中的实际帧数
	int16	                iLen;			//运动总位移
	int32                  iTotalFrame;	//总帧数
	int32                  iNowFrameNum;	//当前帧
	uint16 cur_item_index = SEAPI_GetCurItemIndex();
	BOOLEAN                is_turn = TRUE;
	multi_se_info->is_in_idle_anim = FALSE;
	//如果大于半数帧，则翻过去
	if(multi_se_info->fling_recover_start_frame>=SE_ONE_ITEM_TOTAL_FRAME/2)
	{
		iLen = SE_ONE_ITEM_TOTAL_FRAME - multi_se_info->fling_recover_start_frame;
		is_turn = TRUE;	
	}
	else
	{
		iLen = multi_se_info->fling_recover_start_frame;
		is_turn = FALSE;
	}
	
	iTotalFrame = MIN(SE_RECOVER_ANIM_TOTAL_FRAME,iLen);
	iNowFrameNum = multi_se_info->current_frame;
			
	if(multi_se_info->current_frame<iTotalFrame)
	{
		current_frame = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,
										        iLen,
										        iTotalFrame, 
										        iNowFrameNum
							        			);
		if(FALSE == is_turn)
		{
			current_frame = multi_se_info->fling_recover_start_frame - current_frame;
		}
		else
		{
			current_frame =  multi_se_info->fling_recover_start_frame + current_frame;
		}
		
		//最后一帧，切换当前选择的item
		if(current_frame == SE_ONE_ITEM_TOTAL_FRAME
			&&TRUE == is_turn)
		{
			if(multi_se_info->direction)
			{
				if((cur_item_index+1)<multi_se_info->total_item_num)
				{
					cur_item_index = cur_item_index+1;
				}
				else
				{
					cur_item_index = 0;
				}
			}
			else
			{
				if(cur_item_index==0)
				{
					cur_item_index = multi_se_info->total_item_num-1;
				}
				else
				{
					cur_item_index = cur_item_index-1;
				}
				
			}

			SEAPI_SetCurItemIndex(cur_item_index);
			current_frame = 0;
		}

		SE_DrawAndUpdateOneFrame(multi_se_info,current_frame,SE_ONE_ITEM_TOTAL_FRAME);
		

		if(current_frame == 0)
		{
			multi_se_info->current_frame = iTotalFrame;
		}
		else
		{
			multi_se_info->current_frame++;
		}
		
	}
	else
	{
		multi_se_info->slide_state = MMK_TP_SLIDE_NONE;
		multi_se_info->tp_status = MMI_TP_NONE;
		multi_se_info->current_frame = 0;
        multi_se_info->is_in_idle_anim = TRUE;
        SE_Update_Glass_Centent();
		SE_Start_Conf_Timer(multi_se_info, &multi_se_info->idle_anim_timer_id,SE_IDLE_ANIM_TIME,FALSE);
	}
}

/*****************************************************************************/
// Description : OPEN 处理函数
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleOpenWinMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 cur_item_index = 0;
	
	SE_InitCalcParam(win_id,multi_se_info);

	//SE_UZone_CreatMemory(multi_se_info);

	cur_item_index = SEAPI_GetCurItemIndex();
	
	if(cur_item_index==0)
	{
		cur_item_index = multi_se_info->total_item_num-1;
	}
	else
	{
		cur_item_index = cur_item_index-1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);
	multi_se_info->direction = TRUE;//开场动画向逆时针方向运动
	//画第一针
	SE_DrawAndUpdateOneFrame(multi_se_info,multi_se_info->current_frame,SE_ONE_ITEM_TOTAL_FRAME);

	if(cur_item_index== multi_se_info->total_item_num-1)
	{
		cur_item_index = 0;
	}
	else
	{
		cur_item_index = cur_item_index + 1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);
}

/*****************************************************************************/
// Description : SE_HandleWinGetFocusMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinGetFocusMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 cur_item_index = 0;
	
	SE_InitCalcParam(win_id,multi_se_info);

	cur_item_index = SEAPI_GetCurItemIndex();
	if(cur_item_index==0)
	{
		cur_item_index = multi_se_info->total_item_num-1;
	}
	else
	{
		cur_item_index = cur_item_index-1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);
	multi_se_info->direction = TRUE;//开场动画向逆时针方向运动
	//画第一针
	SE_DrawAndUpdateOneFrame(multi_se_info,multi_se_info->current_frame,SE_ONE_ITEM_TOTAL_FRAME);

	if(cur_item_index== multi_se_info->total_item_num-1)
	{
		cur_item_index = 0;
	}
	else
	{
		cur_item_index = cur_item_index + 1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);

}
/*****************************************************************************/
// Description : SE_HandleWinLoseFocusMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinLoseFocusMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
    uint16 cur_item_index = 0;

    cur_item_index = SEAPI_GetCurItemIndex();
    //不需要重置参数
    if(multi_se_info->is_in_anim)
    {
        if(cur_item_index== multi_se_info->total_item_num-1)
        {
            cur_item_index = 0;
        }
        else
        {
            cur_item_index = cur_item_index + 1;
        }
    }
    SEAPI_SetCurItemIndex(cur_item_index);
    //SE_DestroyCalcParam(win_id,multi_se_info);
    SE_UZone_ReleaseMemory();
    multi_se_info->is_in_idle_anim = FALSE;
    multi_se_info->is_in_anim = FALSE;
}
/*****************************************************************************/
// Description : SE_HandleCloseWinMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleCloseWinMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
	SE_DestroyCalcParam(win_id,multi_se_info);
	SE_UZone_ReleaseMemory();
}

/*****************************************************************************/
// Description : SE_HandleWinFullPaintMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinFullPaintMsg(MMI_WIN_ID_T win_id,SE_SLIDE_STATE_T *multi_se_info)
{
	uint16 cur_item_index = 0;

	if(!MMK_IsFocusWin(multi_se_info->handle))
	{
		return;
	}
	
	if(!multi_se_info->is_in_anim)
	{
		cur_item_index = SEAPI_GetCurItemIndex();
		if(cur_item_index==0)
		{
			cur_item_index = multi_se_info->total_item_num-1;
		}
		else
		{
			cur_item_index = cur_item_index-1;
		}	

		SEAPI_SetCurItemIndex(cur_item_index);
	}
	else
	{
		//如果还在之前的开场动画中，则不再重新设置-1了
	}
	
	SE_StartTimer(multi_se_info,&multi_se_info->anim_timer_id);
	multi_se_info->is_in_anim = TRUE;
}
/*****************************************************************************/
// Description : SE_HandleTPDownMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPDownMsg(MMI_WIN_ID_T win_id,GUI_POINT_T *tp_point_ptr,SE_SLIDE_STATE_T *multi_se_info)
{
	//如果在开场动画则不响应TP DOWN消息
	if(multi_se_info->is_in_anim)
	{
		return;
	}
        
	// 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y);
	
	// 停止timer
	SE_StopTimer(multi_se_info,&multi_se_info->anim_timer_id);
	SE_StopTimer(multi_se_info,&multi_se_info->slide_timer_id);
	
	//如果在滑动时按下TP，要停止滑动，改为跟手。
	SE_StopTimer(multi_se_info,&multi_se_info->fling_timer_id);

    // 首先设置页面的滑动相关数据
    // 记录起始点
    multi_se_info->slide_start_point.x = tp_point_ptr->x;
    multi_se_info->slide_start_point.y = tp_point_ptr->y;
    
    // 记录前一个点
    multi_se_info->slide_pre_point.x = tp_point_ptr->x;
    multi_se_info->slide_pre_point.y = tp_point_ptr->y;

	//设置TP STATUS
	multi_se_info->tp_status = MMI_TP_DOWN;
	multi_se_info->current_frame = 0;
    multi_se_info->pre_frame = 0;
    
	//开启滑动TIMER
	SE_StartTimer(multi_se_info,&multi_se_info->slide_timer_id);
}

/*****************************************************************************/
// Description : SE_HandleTPMoveMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPMoveMsg(MMI_WIN_ID_T win_id,
								GUI_POINT_T *tp_point_ptr,
								SE_SLIDE_STATE_T *multi_se_info
								)
{
	// 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y);	
    
	if(multi_se_info->tp_status == MMI_TP_DOWN)
	{
		//设置TP STATUS
		multi_se_info->tp_status = MMI_TP_MOVE;
        
        //如果已经滑动则不处理MOVE MSG
        if (MMK_TP_SLIDE_TP_SCROLL == multi_se_info->slide_state )
        {
            return;
        }
        
	    //如果没有在滑动,则判断是否滑动
	    //如果是横屏
	    if(multi_se_info->is_landscape)
	    {
			if(abs(tp_point_ptr->x - multi_se_info->slide_start_point.x) > SE_TP_MOVE_MIN_PIX)
		    {
		        multi_se_info->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				multi_se_info->current_frame = 0;
		    }
			
		}
		else
		{
			if(abs(tp_point_ptr->y - multi_se_info->slide_start_point.y) > SE_TP_MOVE_MIN_PIX)
		    {
		        multi_se_info->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				multi_se_info->current_frame = 0;
		    }			
		}

	}
}

/*****************************************************************************/
// Description : SE_HandleTPUpMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleTPUpMsg(MMI_WIN_ID_T win_id,
							GUI_POINT_T *tp_point_ptr,
							SE_SLIDE_STATE_T *multi_se_info
							)
{
    float       fling_velocity;   //速度
	int32         index = 0;
	
	if(multi_se_info->tp_status == MMI_TP_NONE)
	{
		return;
	}
	
	//设置TP STATUS
	multi_se_info->tp_status = MMI_TP_UP;
	// 停止timer
	SE_StopTimer(multi_se_info,&multi_se_info->fling_timer_id);

	multi_se_info->fling_recover_start_frame = multi_se_info->current_frame%SE_ONE_ITEM_TOTAL_FRAME;

	//SCI_TRACE_LOW:"[MMIUZONE] SE_HandleTPUpMsg multi_se_info->fling_recover_start_frame = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1129_112_2_18_3_4_25_49,(uint8*)"d",multi_se_info->fling_recover_start_frame);
	
	multi_se_info->current_frame = 2;//从第二针阻尼，第一帧已经显示过

    if (MMK_TP_SLIDE_NONE != multi_se_info->slide_state)
    {
		// 增加速度点
	    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y);	
		
		if(multi_se_info->is_landscape)
		{
			//判断方向
			if(tp_point_ptr->x - multi_se_info->slide_start_point.x>0)
			{
				multi_se_info->direction = FALSE;
			}
			else
			{
				multi_se_info->direction = TRUE;
			}
			//计算当前速度
	        if (multi_se_info->slide_state == MMK_TP_SLIDE_FLING)
	        {
	            MMK_ComputeCurrentVelocity(&multi_se_info->fling_velocity_x,PNULL, (float)FLING_MAX_VELOCITY, (float)FLING_MAX_VELOCITY);
	        }
	        else
	        {
	            MMK_ComputeCurrentVelocity(&multi_se_info->fling_velocity_x,PNULL, (float)FLING_MAX_VELOCITY,  (float)FLING_MAX_VELOCITY);
	        }

			fling_velocity = multi_se_info->fling_velocity_x;
		}
		else
		{
			//判断方向
			if(tp_point_ptr->y - multi_se_info->slide_start_point.y>0)
			{
				multi_se_info->direction = FALSE;
			}
			else
			{
				multi_se_info->direction = TRUE;
			}
			//计算当前速度
	        if (multi_se_info->slide_state == MMK_TP_SLIDE_FLING)
	        {
	            MMK_ComputeCurrentVelocity(PNULL, &multi_se_info->fling_velocity_y, (float)FLING_MAX_VELOCITY, (float)FLING_MAX_VELOCITY);
	        }
	        else
	        {
	            MMK_ComputeCurrentVelocity(PNULL, &multi_se_info->fling_velocity_y, (float)FLING_MAX_VELOCITY,  (float)FLING_MAX_VELOCITY);
	        }

			fling_velocity = multi_se_info->fling_velocity_y;
		}
		
		//SCI_TRACE_LOW:"[MMIUZONE] SE_HandleTPUpMsg fling_velocity = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1185_112_2_18_3_4_25_50,(uint8*)"d",fling_velocity);

		// 最小速度
        if (FLING_MIN_VELOCITY <= abs((int32)fling_velocity))
        {
        	//计算出总共要转几个ITEM
        	multi_se_info->slide_item_num =  abs((int32)fling_velocity)/SE_VELOCITY_ADJUST_PARAM;
			//开启FLING TIMER
			SE_StartTimer(multi_se_info,&multi_se_info->fling_timer_id);
            multi_se_info->slide_state = MMK_TP_SLIDE_FLING;
        }
        else
        {
			//开启 TIMER
			SE_StartTimer(multi_se_info,&multi_se_info->recover_timer_id);
			
            MMK_ResetVelocityItem();
            multi_se_info->fling_velocity_x = 0;
			multi_se_info->fling_velocity_y = 0;
			
            multi_se_info->slide_state = MMK_TP_SLIDE_RECOVER;            
        }
		
    }
	else
	{
		IMGREF_POINT_T tp_point = {0,0};
		tp_point.x = tp_point_ptr->x;
		tp_point.y = tp_point_ptr->y;
		
		//判断点击区域是否在ITEM上，如果在则计算ITEM_INDEX,调用相应函数
		index = Get_Page_Num(tp_point,
		multi_se_info->calc_param_info.bg_win_size,
		multi_se_info->calc_param_info.page_size,
		multi_se_info->is_landscape,
		multi_se_info->calc_param_info.out_buff_ptr
		);
		
		//SCI_TRACE_LOW:"[MMIUZONE] SE_HandleTPUpMsg index = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1223_112_2_18_3_4_26_51,(uint8*)"d",index);

		if(-1 != index)
		{
			index = (multi_se_info->cur_select_item+multi_se_info->total_item_num+index-2)%multi_se_info->total_item_num;
			SEAPI_SetCurItemIndex(index);
            if(PNULL != s_press_item_callback_ptr)
            {
                (*s_press_item_callback_ptr)((uint32)index);
            }
		}
	}
}
/*****************************************************************************/
// Description : SE_HandleKeyDirectionMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyDirectionMsg(
        							SE_SLIDE_STATE_T *multi_se_info,
        							BOOLEAN direction
        							)
{
    if(!multi_se_info->is_in_anim)
    {
    	multi_se_info->direction = direction;//向direction方向运动
    	multi_se_info->is_in_anim = TRUE;
    	multi_se_info->current_frame = 0;
    	SE_StartTimer(multi_se_info,&multi_se_info->anim_timer_id);
    }
}
/*****************************************************************************/
// Description : SE_HandleKeyUpMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyUpMsg(MMI_WIN_ID_T win_id,
							SE_SLIDE_STATE_T *multi_se_info
							)
{

}
/*****************************************************************************/
// Description : SE_HandleKeyDownMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyDownMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								)
{

}
/*****************************************************************************/
// Description : SE_HandleKeyLeftMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyLeftMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								)
{

		if(MMK_TP_SLIDE_NONE == multi_se_info->slide_state)
		{
            SE_HandleKeyDirectionMsg(multi_se_info,FALSE);
		}
	
}

/*****************************************************************************/
// Description : SE_HandleKeyRightMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleKeyRightMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								)
{

		if(MMK_TP_SLIDE_NONE == multi_se_info->slide_state)
		{
            SE_HandleKeyDirectionMsg(multi_se_info,TRUE);
		}

}
/*****************************************************************************/
// Description : SE_HandleAppOKMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleAppOKMsg(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								)
{
	if(multi_se_info == PNULL||s_press_item_callback_ptr == PNULL)
	{
		return;
	}
	
	(*s_press_item_callback_ptr)((uint32)multi_se_info->cur_select_item);
}
/*****************************************************************************/
// Description : SE_HandleAppOKMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleAppNumKeyMsg(MMI_WIN_ID_T win_id,
								MMI_MESSAGE_ID_E     msg_id, 
								SE_SLIDE_STATE_T *multi_se_info
								)
{
	uint16 aim_index = 0;

    	if(multi_se_info->is_in_anim)
    	{
		return;
	}
	aim_index = msg_id - MSG_APP_1;
	
	if(aim_index>multi_se_info->total_item_num-1)
	{
		return;
	}

	SEAPI_SetCurItemIndex(aim_index);
	
	(*s_press_item_callback_ptr)(aim_index);
}
/*****************************************************************************/
// Description : SE_HandleWinTimerMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
LOCAL void SE_HandleWinTimerMsg(MMI_WIN_ID_T win_id,
								DPARAM  param,
								SE_SLIDE_STATE_T *multi_se_info)
{
    if(PNULL == param)
    {
        return;
    }
    
	if (!MMK_IsFocusWin(multi_se_info->handle))
	{    
		SE_StopTimer(multi_se_info, &multi_se_info->anim_timer_id);
		SE_StopTimer(multi_se_info, &multi_se_info->slide_timer_id);
		SE_StopTimer(multi_se_info, &multi_se_info->fling_timer_id);
		SE_StopTimer(multi_se_info, &multi_se_info->recover_timer_id);
        SE_StopTimer(multi_se_info, &multi_se_info->idle_anim_timer_id);

		return;
	}
	
	//SCI_TRACE_LOW:"[MMIUZONE] SE_HandleWinTimerMsg param = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1359_112_2_18_3_4_26_52,(uint8*)"d",param);
	
	if(multi_se_info->anim_timer_id ==  *( GUI_TIMER_ID_T*)param)
	{
		SE_StopTimer(multi_se_info, &multi_se_info->anim_timer_id);
		
        if (MMK_IsFocusWin(multi_se_info->handle))
        {                        
            if (multi_se_info->is_in_anim)
            {
                SE_StartTimer(multi_se_info, &multi_se_info->anim_timer_id);

                SE_HandleAnimTimerMsg(multi_se_info);
            }
        }	
	}
	else if(multi_se_info->slide_timer_id ==  *( GUI_TIMER_ID_T*)param)
	{
		SE_StopTimer(multi_se_info, &multi_se_info->slide_timer_id);
		
        if (MMK_IsFocusWin(multi_se_info->handle))
        {                        
           SE_HandleSlideTimerMsg(multi_se_info);
        }			
	}
	else if(multi_se_info->fling_timer_id ==  *( GUI_TIMER_ID_T*)param)
	{
		SE_StopTimer(multi_se_info, &multi_se_info->fling_timer_id);
		
        if (MMK_IsFocusWin(multi_se_info->handle))
        {                        
            if (MMK_TP_SLIDE_FLING== multi_se_info->slide_state)
            {
            	SE_StartTimer(multi_se_info, &multi_se_info->fling_timer_id);
				
                SE_HandleFlingTimerMsg(multi_se_info);
            }
        }			
	}
	else if(multi_se_info->recover_timer_id ==  *( GUI_TIMER_ID_T*)param)
	{
		SE_StopTimer(multi_se_info, &multi_se_info->recover_timer_id);
		
        if (MMK_IsFocusWin(multi_se_info->handle))
        {                        
            if (MMK_TP_SLIDE_RECOVER== multi_se_info->slide_state)
            {
            	SE_StartTimer(multi_se_info, &multi_se_info->recover_timer_id);

				SE_HandleRecoverTimerMsg(multi_se_info);
            }
        }	
	}
       else if(multi_se_info->idle_anim_timer_id ==  *( GUI_TIMER_ID_T*)param)
	{
		SE_StopTimer(multi_se_info, &multi_se_info->idle_anim_timer_id);

            if (MMK_IsFocusWin(multi_se_info->handle)&&multi_se_info->is_in_idle_anim)
            {                        
               //SE_StartTimer(multi_se_info, &multi_se_info->idle_anim_timer_id);
			   SE_Start_Conf_Timer(multi_se_info, &multi_se_info->idle_anim_timer_id,SE_IDLE_ANIM_TIME,FALSE);
               SE_HandleIdleAnimTimerMsg(multi_se_info);
            }

	}
	else
	{
		//SCI_TRACE_LOW:"[MMIUZONE] SE_HandleWinTimerMsg MSG_ID = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1414_112_2_18_3_4_26_53,(uint8*)"d",param);
	}
}
/*****************************************************************************/
// Description : SE_HandleWinLCDSwitch
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

LOCAL void SE_HandleWinLCDSwitch(MMI_WIN_ID_T win_id,
								SE_SLIDE_STATE_T *multi_se_info
								)
{
	uint16 cur_item_index = 0;

    if(!MMK_IsFocusWin(multi_se_info->handle))
    {
        return;
    }

	
	SE_StopTimer(multi_se_info, &multi_se_info->anim_timer_id);
	SE_StopTimer(multi_se_info, &multi_se_info->slide_timer_id);
	SE_StopTimer(multi_se_info, &multi_se_info->fling_timer_id);
	SE_StopTimer(multi_se_info, &multi_se_info->recover_timer_id);

	SE_InitCalcParam(win_id,multi_se_info);

	//SE_UZone_CreatMemory(multi_se_info);

	multi_se_info->direction = TRUE;//开场动画向逆时针方向运动
	
	cur_item_index = SEAPI_GetCurItemIndex();
	if(cur_item_index==0)
	{
		cur_item_index = multi_se_info->total_item_num-1;
	}
	else
	{
		cur_item_index = cur_item_index-1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);
	
	//画第一针
	SE_DrawAndUpdateOneFrame(multi_se_info,multi_se_info->current_frame,SE_ONE_ITEM_TOTAL_FRAME);

	if(cur_item_index== multi_se_info->total_item_num-1)
	{
		cur_item_index = 0;
	}
	else
	{
		cur_item_index = cur_item_index + 1;
	}		
	SEAPI_SetCurItemIndex(cur_item_index);
}


/*****************************************************************************/
// Description : SEAPI_HandleMsg
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E SEAPI_HandleMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_RESULT_E              result = MMI_RESULT_TRUE;
	GUI_POINT_T               point = {0};
	
	//SCI_TRACE_LOW:"[MMIUZONE] SEAPI_HandleMsg msg_id = 0x%04x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_SE_1481_112_2_18_3_4_26_54,(uint8*)"d",msg_id);

    switch(msg_id)
    {
	    case MSG_OPEN_WINDOW:            
			//初始化数据，参数，以及计算BUF
			SE_HandleOpenWinMsg(win_id,&s_multi_se_info);
	        break;
	    case MSG_BACKLIGHT_TURN_ON:
	    case MSG_GET_FOCUS:
			//初始化数据，参数，以及计算BUF
			SE_HandleWinGetFocusMsg(win_id,&s_multi_se_info);
	        break;
	    case MSG_BACKLIGHT_TURN_OFF:
	    case MSG_LOSE_FOCUS:
			//释放内存，停止timer
			SE_HandleWinLoseFocusMsg(win_id,&s_multi_se_info);
	        break;
	    case MSG_CLOSE_WINDOW:
			//释放内存，停止timer
			SE_HandleCloseWinMsg(win_id,&s_multi_se_info);
	        break;    
	    case MSG_FULL_PAINT:
			//开始timer
			SE_HandleWinFullPaintMsg(win_id,&s_multi_se_info);
	        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_TP_PRESS_DOWN:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
			SE_HandleTPDownMsg(win_id,&point,&s_multi_se_info);
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_TP_PRESS_UP:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
			SE_HandleTPUpMsg(win_id,&point,&s_multi_se_info);
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_TP_PRESS_MOVE:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
			SE_HandleTPMoveMsg(win_id,&point,&s_multi_se_info);
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_APP_UP:
    	case MSG_KEYREPEAT_UP:
			SE_HandleKeyUpMsg(win_id,&s_multi_se_info);
			break;
		case MSG_APP_DOWN:
    	case MSG_KEYREPEAT_DOWN:
			SE_HandleKeyDownMsg(win_id,&s_multi_se_info);
			break;
		case MSG_APP_LEFT:
    	case MSG_KEYREPEAT_LEFT:
			SE_HandleKeyLeftMsg(win_id,&s_multi_se_info);
			break;
		case MSG_APP_RIGHT:
    	case MSG_KEYREPEAT_RIGHT:
			SE_HandleKeyRightMsg(win_id,&s_multi_se_info);
			break;
		case MSG_TIMER:
			SE_HandleWinTimerMsg(win_id,param,&s_multi_se_info);
			break;
		case MSG_APP_CANCEL:
        	MMK_CloseWin(win_id);
        	break;
        case MSG_APP_WEB:
		case MSG_APP_OK:
        	SE_HandleAppOKMsg(win_id,&s_multi_se_info);
        	break;	
        case MSG_APP_1:
        case MSG_APP_2:
        case MSG_APP_3:
        case MSG_APP_4:
        case MSG_APP_5:
        case MSG_APP_6:
        case MSG_APP_7:
        case MSG_APP_8:
        case MSG_APP_9:
		SE_HandleAppNumKeyMsg(win_id,msg_id,&s_multi_se_info);
		break;
	    default:
                result = MMI_RESULT_FALSE;
	        break;
    }
    return (result);
}
/*****************************************************************************/
// Description : SEAPI_CreateShadow
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void SEAPI_CreateShadow(uint8* src_ptr, 
                                        uint8* des_ptr,
                                        uint32 *icon_buff_array,
                                        GUI_RECT_T *icon_rect_ptr,
                                        uint32 *str_buff_array,
                                        GUI_RECT_T *str_rect_ptr,
                                        uint32 *str_shadow_buff_array
                                        )
{
    IMGREF_SIZE_T page_size = {0};
    IMGREF_SIZE_T shadow_size = {0};
    
    page_size.w = UZONE_ITEM_BG_WIDTH;
    page_size.h = UZONE_ITEM_BG_HEIGHT;
    shadow_size.w = UZONE_ITEM_BG_WIDTH;
    shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
    
    s_icon_rect.x = icon_rect_ptr->left;
    s_icon_rect.y = icon_rect_ptr->top;
    s_icon_rect.w = icon_rect_ptr->right - icon_rect_ptr->left+1;
    s_icon_rect.h = icon_rect_ptr->bottom - icon_rect_ptr->top+1;

    s_str_rect.x = str_rect_ptr->left;
    s_str_rect.y = str_rect_ptr->top;
    s_str_rect.w = str_rect_ptr->right - str_rect_ptr->left+1;
    s_str_rect.h = str_rect_ptr->bottom - str_rect_ptr->top+1;
    
    Get_Buffer( page_size,
                shadow_size,
                (COLOR32*)src_ptr,
                (COLOR32*)des_ptr,
                icon_buff_array,
                &s_icon_rect, 
                str_buff_array,
                &s_str_rect, 
                str_shadow_buff_array,
                &s_str_shadow_rect,
                MMIUZONE_ITEM_ID_MAX
                );
}
/*****************************************************************************/
//  Description : SE_Update_Glass_Centent
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SE_Update_Glass_Centent(void)
{

    uint16 cur_select_item = 0;
    cur_select_item = SEAPI_GetCurItemIndex();
    
    (*s_update_mid_glass_centent_fptr)(cur_select_item);

}

/*****************************************************************************/
//  Description : set star layers handle
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetStarLayerHandle(GUI_LCD_DEV_INFO* big_star_layer, 
                                                GUI_LCD_DEV_INFO* small_star_layer)
{
    s_small_star = (uint32*)UILAYER_GetLayerBufferPtr(small_star_layer);
    s_big_star = (uint32*)UILAYER_GetLayerBufferPtr(big_star_layer);  
}

/*****************************************************************************/
//  Description : set focus item layers handle
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetFocusItemLayerHandle(GUI_LCD_DEV_INFO* focus_item_layer)
{
    s_Ori_Pic_buf = (SE_LAYER_BUF_TYPE*)UILAYER_GetLayerBufferPtr(focus_item_layer);
}

/*****************************************************************************/
//  Description : set draw item callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetDrawItemCb(SE_DRAWITEM_CALLBACK cb)
{
    if(PNULL == cb)
    {
        SCI_TRACE_LOW("[MMIUZONE] SEAPI_SetDrawItemCb   cb = PNULL ");
    }
    
    s_update_mid_glass_centent_fptr = cb;
}


LOCAL int32 Get_Page_Num(IMGREF_POINT_T click_p,
							IMGREF_SIZE_T lcd_size,//屏幕大小。竖屏时240320的话横屏时320 240.
							IMGREF_SIZE_T page_size,//输入图片大小，横屏时带阴影的图片大小
							BOOLEAN Is_Landmode,
							uint32* bg_buff
							)
{	
		int page_wid=0,page_height=0;
		int32 i=0;
		int shAngle=0;
		FIX16_POINT_T point_ptr={0};
		IMGREF_3D_POINT_T centre_p={0},in_Point={0};
		IMGREF_3D_POINT_T sEye={0};
		FIX16_POINT_T aVertex_ptr[4]={0};
	
		
		
		point_ptr.x=click_p.x;
		point_ptr.y=click_p.y;
		page_wid=page_size.w;
		page_height=page_size.h;
		
		sEye.x=lcd_size.w/2;
		sEye.y=lcd_size.h/2;
		sEye.z=-2*lcd_size.h;
			
		if(Is_Landmode)
		{     
			sEye.x=lcd_size.w;
			lcd_size.w=lcd_size.h;
			lcd_size.h=sEye.x;
			
			point_ptr.x=click_p.y;
			point_ptr.y=click_p.x;
			
			sEye.x=lcd_size.w*5/16;
			sEye.y=lcd_size.h/2;
			sEye.z=-2*lcd_size.h;

			page_wid=page_size.h;
			page_height=page_size.w;
			
			
		}
		
		
		
			for(i=0;i<5;i++)
				{
					centre_p.x=s_page_axis_array[i].axis_x;
					centre_p.y=s_page_axis_array[i].axis_y;
					centre_p.z=s_page_axis_array[i].axis_z;
					shAngle=-(s_page_axis_array[i].angle-1800);
							if(Is_Landmode)
					{
						centre_p.x=lcd_size.w-s_page_axis_array[i].axis_y;
						centre_p.y=s_page_axis_array[i].axis_x;
					}
					
					in_Point.x = centre_p.x - page_wid/2;
					in_Point.y = centre_p.y - (page_height/2)*s2d_cos(shAngle)/65536;
					in_Point.z = centre_p.z + (page_height/2)*s2d_sin(shAngle)/65536;

					s3d_Free_Point_change(in_Point,
										sEye,
										&aVertex_ptr[0]
										);

					in_Point.x = centre_p.x + page_wid/2;
					in_Point.y = centre_p.y - (page_height/2)*s2d_cos(shAngle)/65536;
					in_Point.z = centre_p.z + (page_height/2)*s2d_sin(shAngle)/65536;

					s3d_Free_Point_change(in_Point,
										sEye,
										&aVertex_ptr[1]
										);

					in_Point.x = centre_p.x - page_wid/2;
					in_Point.y = centre_p.y + (page_height/2)*s2d_cos(shAngle)/65536;
					in_Point.z = centre_p.z - (page_height/2)*s2d_sin(shAngle)/65536;

					s3d_Free_Point_change(in_Point,
										sEye,
										&aVertex_ptr[3]
										);

					in_Point.x = centre_p.x + page_wid/2;
					in_Point.y = centre_p.y + (page_height/2)*s2d_cos(shAngle)/65536;
					in_Point.z = centre_p.z - (page_height/2)*s2d_sin(shAngle)/65536;

					s3d_Free_Point_change(in_Point,
										sEye,
										&aVertex_ptr[2]
										);
					if(Is_Landmode)
						{
							aVertex_ptr[0].x=aVertex_ptr[0].x+(aVertex_ptr[1].x-aVertex_ptr[0].x)/5;
							aVertex_ptr[3].x=aVertex_ptr[3].x+(aVertex_ptr[2].x-aVertex_ptr[3].x)/5;
						}
								
					/*
					for(m=0;m<4;m++)
						{
							for(i0=aVertex_ptr[m].y-1;i0<=aVertex_ptr[m].y+1;i0++)
								{
									for(j0=aVertex_ptr[m].x-1;j0<=aVertex_ptr[m].x+1;j0++)
										{    if((i0>=0)&&(i0<lcd_size.h)&&(j0>0)&&(j0<lcd_size.w))
											{
												bg_buff[i0*lcd_size.w+j0]=0xffff0000;
											}
										}
								}
						}	
					*/
						
					
					if(S3D_QuadrilateralCheckPoint_Crystal(aVertex_ptr,  &point_ptr))
						{
							return i;
						}
					
						
			}
		return -1;
}


LOCAL void Slim_Landscape_Make_Rotate(IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,COLOR32 * Out_Color_Buffer,COLOR32 * In_Color_Buffer,ROTA_DATA_T img_data,int is_aixs_x)
{    
     S3D_ROTATION_GEOMETRY_T  s3d_geometry;
     S3D_ROTATION_IMG_T   s3d_src_img;
     S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img;
     
     s3d_geometry.img_size.w=img_size.w;
     s3d_geometry.img_size.h=img_size.h;
     s3d_geometry.img_size.x =0;
     s3d_geometry.img_size.y = 0;

     s3d_geometry.angle=img_data.angle;
     s3d_geometry.eye.x=win_size.w/2;
     s3d_geometry.eye.y=win_size.h*11/16;
     s3d_geometry.eye.z=-2*win_size.w;

     s3d_geometry.axis.x=img_data.axis_x;
     s3d_geometry.axis.y=img_data.axis_y;
     s3d_geometry.axis.z=img_data.axis_z;
     s3d_geometry.axis.axis=is_aixs_x;
     s3d_geometry.is_axis_use_default = SCI_FALSE;
     s3d_geometry.is_eye_use_default = SCI_FALSE;

     s3d_src_img.src_ptr=In_Color_Buffer;
     s3d_src_img.src_type=S3D_DATA_ARGB888;

     s3d_img.color_ptr=Out_Color_Buffer;
     s3d_img.size.h=win_size.h;
     s3d_img.size.w=win_size.w;

     S3D_ImageMidAxisRotation ( &(s3d_geometry),&(s3d_src_img),&(s3d_img));
}
LOCAL BOOLEAN Slim_Landscape_Get_Location(uint8 quadrant,
											ROTA_DATA_T * rotation_data,
											int cur_frame_num,
											int total_frame_num,
											BOOLEAN Is_go_up)
{	
        int32 x_total=0;
        int32 Amp=0;
        int32  alpha=0;
        if((0==total_frame_num)||(PNULL==rotation_data))
        {
        	return SCI_FALSE;
        }
         x_total=s_page_axis_array[4].axis_x-s_page_axis_array[0].axis_x;
         Amp=abs(s_page_axis_array[2].axis_z-s_page_axis_array[0].axis_z)/2;
       
        if(Is_go_up)
        {
        	 alpha=900*quadrant-900*cur_frame_num/total_frame_num;
        }
        else
        {
        	alpha=900*quadrant+900*cur_frame_num/total_frame_num;
        }
            
        rotation_data->axis_x=x_total*alpha/3600+s_page_axis_array[0].axis_x;
        rotation_data->axis_z=Amp*s2d_cos_new(alpha)/65536+Amp+s_page_axis_array[2].axis_z;
        rotation_data->angle=BASE_ANGLE-ANGLE_TWO*s2d_sin_new(alpha)/65536;
        
        return SCI_TRUE;
}

LOCAL BOOLEAN Slim_Landscape_Mode(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr)
{	

	int i=0,page_wid=0,page_height=0,touch_height=0;
	ROTA_DATA_T  current_page_axis_array[PAGE_NUM+1]={0};
	COLOR32 * current_page_buffer_array[PAGE_NUM+1]={0};
	uint32 image_size = page_size.w*page_size.h*sizeof(uint8);
    S3D_UZONE_EXPANDPTR_T* exptr = PNULL;
#ifdef UZONE_STAR_EFFECT
	//stars
	uint32 icon_num=0,star_number=0,w=0,h=0;
	uint32 * current_buff=PNULL;
	IMGREF_SIZE_T Small_Size={0},Big_Size={0};
	static uint32 temp_frame=0;
     uint8 random=0;
    IMGREF_SIZE_T	star_up_window = {0};
	FIX16_POINT_T	star_offset = {0};
    uint32 big_star_size = 0;
    uint32 small_star_size = 0;
	//stars
	#endif
#ifdef UZONE_BORDER_SHINING	
    //Benny added
	COLOR32* output = PNULL;
    PIC_CIRCULAR_JOINT_T Joint = {0};
	IMGREF_RECT_T ShiningZone = {0};
    IMGREF_SIZE_T shadow_size = {0};	
	IMGREF_SIZE_T orignal_size={0};    
	COLOR32* temp_reverse = PNULL;
    //uint32* shining_ori_img = PNULL;
#endif
#ifdef CALTIME
	static int cal_time=0,whole_time1=0,whole_time2=0;
	whole_time1=SCI_GetTickCount();
	SCI_TRACE_LOW("Uzone_total, cost_time ONE frame = %d ms", whole_time1-whole_time2);
	whole_time2=SCI_GetTickCount();
	cal_time=SCI_GetTickCount();
#endif	
//PopCalcTest();
	touch_height=bg_win_size.w/2;
	page_wid=page_size.w;
	page_height=page_size.h;
	
   	if((page_num<5)||(0==total_frame_num)||(start_location>page_num))
   		{
   			return SCI_FALSE;
   		}
    //shining_ori_img = page_buff_array + image_size*(page_num);

	  //静止态
	if(0== cur_frame_num) 
       {
       	for(i=0;i<PAGE_NUM;i++)
       		{
       			s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/16;
       			//s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/32;
			}
		s_page_axis_array[0].axis_z=TOUCH_DEEP+(page_wid+2*PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[1].axis_z=TOUCH_DEEP+(page_wid/2+PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[2].axis_z=TOUCH_DEEP;
		s_page_axis_array[3].axis_z=s_page_axis_array[1].axis_z;
		s_page_axis_array[4].axis_z=s_page_axis_array[0].axis_z;
		

		s_page_axis_array[1].axis_x=touch_height-page_wid/2-(page_wid/2+PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536;
		s_page_axis_array[0].axis_x=touch_height-page_wid/2-(page_wid+2*PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536-page_wid/2;
		s_page_axis_array[2].axis_x=touch_height;
		s_page_axis_array[3].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[1].axis_x);
		s_page_axis_array[4].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[0].axis_x);
	}
	//切换时叶片开始动起来，计算个帧时叶片的转轴信息。并找出要显示的六幅图像
	if(SCI_TRUE==Is_go_up)
		{	//头结点赋值
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=1;i<PAGE_NUM;i++)
       		{	Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
       				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[PAGE_NUM-1].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location+PAGE_NUM<page_num)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location+PAGE_NUM)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location+PAGE_NUM-page_num)*image_size;
				}
			
		}
	else if(SCI_FALSE==Is_go_up)
		{
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=0;i<PAGE_NUM-1;i++)
       		{	
       			Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			
			current_page_axis_array[PAGE_NUM-1].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM-1].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+(current_page_axis_array[PAGE_NUM-1].axis_x-s_page_axis_array[PAGE_NUM-1].axis_x)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM-1<page_num)
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+((uint32)start_location+PAGE_NUM-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+(uint32)(start_location+PAGE_NUM-1-page_num)*image_size;
				}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[0].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[0].axis_x-PAGE_GAP-page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[0].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location-1<0)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(page_num-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location-1)*image_size;
				}
		}
	//找出六幅要显示的图像


    //expand ptr
    if(PNULL == exp_ptr)
	{
		return FALSE;
	}
  	exptr = (S3D_UZONE_EXPANDPTR_T*)exp_ptr;

    #ifdef UZONE_BORDER_SHINING	
	if(PNULL==exptr->shining_use_buf
		||PNULL==exptr->Point_Add_ptr
		||PNULL==exptr->shadow_use_buf
		||PNULL==exptr->ori_glass_buf
		)
	{
		return FALSE;
	}
	#endif
    #ifdef UZONE_STAR_EFFECT
	if(PNULL==exptr->small_star_buf
		||PNULL==exptr->big_star_buf
		)
	{
		return FALSE;
	}
    #endif
	//进行旋转贴图
if(0==cur_frame_num&&is_in_idle)
{
#ifdef UZONE_BORDER_SHINING	
    	orignal_size.w=UZONE_ITEM_BG_WIDTH;
    	orignal_size.h=UZONE_ITEM_BG_HEIGHT;
        output = exptr->shining_use_buf;//(COLOR32*)SCI_ALLOCA(page_size.w*page_size.h*sizeof(COLOR32));
    	SCI_MEMCPY(output,exptr->ori_glass_buf,orignal_size.w * orignal_size.h*sizeof(COLOR32));
       /// SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));

      ShiningZone.x = MMIUZONE_SHININGZONE_X;
    	ShiningZone.y = MMIUZONE_SHININGZONE_Y;
    	ShiningZone.w = MMIUZONE_SHININGZONE_W;
    	ShiningZone.h = MMIUZONE_SHININGZONE_H;
      Joint.xDelta_vertex = MMIUZONE_SHININGZONE_JOINT_X;
    	Joint.yDelta_vertex = MMIUZONE_SHININGZONE_JOINT_Y;
        
    	Slim_Mode_borderShiningCalc(&orignal_size,output,&ShiningZone,&Joint,exptr->Point_Add_ptr);

    	temp_reverse = output;
    	output = exptr->shadow_use_buf;    

        shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
        shadow_size.w = page_size.w; 
    	Slim_Landscape_Create_Shadow(orignal_size,shadow_size,temp_reverse,output);

    	//SCI_MEMSET(output,0xff,page_size.w*page_size.h*sizeof(COLOR32));
    	//SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
   	
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								output,
    								current_page_axis_array[2], 
    							1);


#else
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								current_page_buffer_array[2], 
    								//centre_buff,
    								current_page_axis_array[2], 
    							1);	
#endif
}
		else
			{
				Slim_Landscape_Make_Rotate(bg_win_size, 
								page_size, 
								out_buff_ptr, 
								current_page_buffer_array[2], 
								//centre_buff,
								current_page_axis_array[2], 
								1);
			}
		for(i=0;i<PAGE_NUM+1;i++)
	       		{	if(2!=i)
					{
		       			Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											current_page_buffer_array[i], 
											current_page_axis_array[i], 
											1);
					}
				}
#ifdef UZONE_STAR_EFFECT
    big_star_size = UZONE_BIG_STAR_HEIGHT*UZONE_BIG_STAR_WIDTH;
    small_star_size = UZONE_SMALL_STAR_HEIGHT*UZONE_SMALL_STAR_WIDTH;
    
		//blend stars
	Small_Size.w=UZONE_SMALL_STAR_WIDTH;
	Small_Size.h=UZONE_SMALL_STAR_HEIGHT;
	Big_Size.w=UZONE_BIG_STAR_WIDTH;
	Big_Size.h=UZONE_BIG_STAR_HEIGHT;
        //星星相位速度
      
      
     #ifdef WIN32
	SCI_Sleep(20);
     #endif
	
	            for(icon_num=0;icon_num<STARTOTALNUM;icon_num++)
        			{	
        			        if(0==s_star_collection[icon_num].display_frame)
        	                    {
        	                        //星星出现的概率
        			            random=rand()%12;
                                        if(1==random)
                                            {
                                                s_star_collection[icon_num].display_frame=2*STARNUM;
                                                s_star_collection[icon_num].speed_frame=2+rand()%6;
                                                s_star_collection[icon_num].total_frame_num=2*STARNUM* s_star_collection[icon_num].speed_frame;
                                            }
        	                    }
        				else
        					{
        						if(s_star_collection[icon_num].display_frame<=STARNUM)
        							{
        								star_number=s_star_collection[icon_num].display_frame-1;
        							}
        						else 
        							{
        								star_number=STARNUM*2-s_star_collection[icon_num].display_frame;
        							}
        						if(s_star_collection[icon_num].is_big)
        							{ 
        								w=Big_Size.w;
        								h=Big_Size.h;
        								current_buff = exptr->big_star_buf + star_number*big_star_size;
        							}
        						else
        							{
        								w=Small_Size.w;
        								h=Small_Size.h;
        								current_buff = exptr->small_star_buf + star_number*small_star_size;
        							}						
        						star_up_window.w = w;
                                                star_up_window.h = h;
                                                star_offset.x = s_star_collection[icon_num].location.x*MMIUZONE_LCD_W/MMIUZONE_DESIGN_BASE_LCD_W;
                                                star_offset.y =s_star_collection[icon_num].location.y*MMIUZONE_LCD_H/MMIUZONE_DESIGN_BASE_LCD_H;
        						S2D_BlendingBehind_ARGB888(bg_win_size,out_buff_ptr,star_up_window,star_offset,current_buff,out_buff_ptr);
                                              s_star_collection[icon_num].total_frame_num--;
                                              s_star_collection[icon_num].display_frame= (s_star_collection[icon_num].total_frame_num+1)/ s_star_collection[icon_num].speed_frame;
                                              
        					}
        			}
	    
	temp_frame++;
	if(temp_frame>=10)
		{
			temp_frame=0;
		}
    
		//blend stars

#endif
#ifdef CALTIME
	cal_time=SCI_GetTickCount()-cal_time;
	SCI_TRACE_LOW("Uzone_calc, cost_time ONE frame = %d ms",cal_time);
#endif	
	return SCI_TRUE;
}

LOCAL BOOLEAN Slim_Landscape_Mode_bak2(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr)
{	

	int i=0,page_wid=0,page_height=0,touch_height=0;
	ROTA_DATA_T  current_page_axis_array[PAGE_NUM+1]={0};
	COLOR32 * current_page_buffer_array[PAGE_NUM+1]={0};
	uint32 image_size = page_size.w*page_size.h*sizeof(uint8);
    S3D_UZONE_EXPANDPTR_T* exptr = PNULL;
#ifdef UZONE_STAR_EFFECT
	//stars
	uint32 icon_num=0,speed=0,star_number=0,w=0,h=0;
	uint32 * current_buff=PNULL;
	IMGREF_SIZE_T Small_Size={0},Big_Size={0};
	static uint temp_frame=0;
     uint8 random=0;
    IMGREF_SIZE_T	star_up_window = {0};
	FIX16_POINT_T	star_offset = {0};
    uint32 big_star_size = 0;
    uint32 small_star_size = 0;
	//stars
	#endif
#ifdef UZONE_BORDER_SHINING	
    //Benny added
	COLOR32* output = PNULL;
    PIC_CIRCULAR_JOINT_T Joint = {0};
	IMGREF_RECT_T ShiningZone = {0};
    IMGREF_SIZE_T shadow_size = {0};	
	IMGREF_SIZE_T orignal_size={0};    
	COLOR32* temp_reverse = PNULL;
    //uint32* shining_ori_img = PNULL;
#endif
#ifdef CALTIME
	static int cal_time=0,whole_time1=0,whole_time2=0;
	whole_time1=SCI_GetTickCount();
	SCI_TRACE_LOW("Uzone_total, cost_time ONE frame = %d ms", whole_time1-whole_time2);
	whole_time2=SCI_GetTickCount();
	cal_time=SCI_GetTickCount();
#endif	

	touch_height=bg_win_size.w/2;
	page_wid=page_size.w;
	page_height=page_size.h;
	
   	if((page_num<5)||(0==total_frame_num)||(start_location>page_num))
   		{
   			return SCI_FALSE;
   		}
    //shining_ori_img = page_buff_array + image_size*(page_num);

	  //静止态
	if(0== cur_frame_num) 
       {
       	for(i=0;i<PAGE_NUM;i++)
       		{
       			s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/16;
       			//s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/32;
			}
		s_page_axis_array[0].axis_z=TOUCH_DEEP+(page_wid+2*PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[1].axis_z=TOUCH_DEEP+(page_wid/2+PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[2].axis_z=TOUCH_DEEP;
		s_page_axis_array[3].axis_z=s_page_axis_array[1].axis_z;
		s_page_axis_array[4].axis_z=s_page_axis_array[0].axis_z;
		

		s_page_axis_array[1].axis_x=touch_height-page_wid/2-(page_wid/2+PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536;
		s_page_axis_array[0].axis_x=touch_height-page_wid/2-(page_wid+2*PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536-page_wid/2;
		s_page_axis_array[2].axis_x=touch_height;
		s_page_axis_array[3].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[1].axis_x);
		s_page_axis_array[4].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[0].axis_x);
	}
	//切换时叶片开始动起来，计算个帧时叶片的转轴信息。并找出要显示的六幅图像
	if(SCI_TRUE==Is_go_up)
		{	//头结点赋值
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=1;i<PAGE_NUM;i++)
       		{	Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
       				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[PAGE_NUM-1].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location+PAGE_NUM<page_num)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location+PAGE_NUM)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location+PAGE_NUM-page_num)*image_size;
				}
			
		}
	else if(SCI_FALSE==Is_go_up)
		{
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=0;i<PAGE_NUM-1;i++)
       		{	
       			Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			
			current_page_axis_array[PAGE_NUM-1].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM-1].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+(current_page_axis_array[PAGE_NUM-1].axis_x-s_page_axis_array[PAGE_NUM-1].axis_x)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM-1<page_num)
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+((uint32)start_location+PAGE_NUM-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+(uint32)(start_location+PAGE_NUM-1-page_num)*image_size;
				}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[0].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[0].axis_x-PAGE_GAP-page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[0].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location-1<0)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(page_num-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location-1)*image_size;
				}
		}
	//找出六幅要显示的图像


    //expand ptr
    if(PNULL == exp_ptr)
	{
		return FALSE;
	}
  	exptr = (S3D_UZONE_EXPANDPTR_T*)exp_ptr;

    #ifdef UZONE_BORDER_SHINING	
	if(PNULL==exptr->shining_use_buf
		||PNULL==exptr->Point_Add_ptr
		||PNULL==exptr->shadow_use_buf
		||PNULL==exptr->ori_glass_buf
		)
	{
		return FALSE;
	}
	#endif
    #ifdef UZONE_STAR_EFFECT
	if(PNULL==exptr->small_star_buf
		||PNULL==exptr->big_star_buf
		)
	{
		return FALSE;
	}
    #endif
	//进行旋转贴图
if(0==cur_frame_num&&is_in_idle)
{
#ifdef UZONE_BORDER_SHINING	
    	orignal_size.w=UZONE_ITEM_BG_WIDTH;
    	orignal_size.h=UZONE_ITEM_BG_HEIGHT;
        output = exptr->shining_use_buf;//(COLOR32*)SCI_ALLOCA(page_size.w*page_size.h*sizeof(COLOR32));
    	SCI_MEMCPY(output,exptr->ori_glass_buf,orignal_size.w * orignal_size.h*sizeof(COLOR32));
       /// SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
        ShiningZone.x = 4;
    	ShiningZone.y = 4;
    	ShiningZone.w = orignal_size.w-8;
    	ShiningZone.h = orignal_size.h-12;
    	/*ShiningZone.x = 1;
    	ShiningZone.y = 2;
    	ShiningZone.w = orignal_size.w-2;
    	ShiningZone.h = orignal_size.h-4;*/

        Joint.xDelta_vertex = 5;
    	Joint.yDelta_vertex = 5;
    	Slim_Mode_borderShiningCalc(&orignal_size,output,&ShiningZone,&Joint,exptr->Point_Add_ptr);

    	temp_reverse = output;
    	output = exptr->shadow_use_buf;    

        shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
        shadow_size.w = page_size.w; 
    	Slim_Landscape_Create_Shadow(orignal_size,shadow_size,temp_reverse,output);

    	//SCI_MEMSET(output,0xff,page_size.w*page_size.h*sizeof(COLOR32));
    	//SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
   	
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								output,
    								current_page_axis_array[2], 
    							1);


#else
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								current_page_buffer_array[2], 
    								//centre_buff,
    								current_page_axis_array[2], 
    							1);	
#endif
}
		else
			{
				Slim_Landscape_Make_Rotate(bg_win_size, 
								page_size, 
								out_buff_ptr, 
								current_page_buffer_array[2], 
								//centre_buff,
								current_page_axis_array[2], 
								1);
			}
		for(i=0;i<PAGE_NUM+1;i++)
	       		{	if(2!=i)
					{
		       			Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											current_page_buffer_array[i], 
											current_page_axis_array[i], 
											1);
					}
				}
#ifdef UZONE_STAR_EFFECT
    big_star_size = UZONE_BIG_STAR_HEIGHT*UZONE_BIG_STAR_WIDTH;
    small_star_size = UZONE_SMALL_STAR_HEIGHT*UZONE_SMALL_STAR_WIDTH;
    
		//blend stars
	Small_Size.w=UZONE_SMALL_STAR_WIDTH;
	Small_Size.h=UZONE_SMALL_STAR_HEIGHT;
	Big_Size.w=UZONE_BIG_STAR_WIDTH;
	Big_Size.h=UZONE_BIG_STAR_HEIGHT;
        //星星相位速度
      speed=2;
      
     #ifdef WIN32
	SCI_Sleep(20);
     #endif
	
	            for(icon_num=0;icon_num<STARTOTALNUM;icon_num++)
        			{	
        			        if(0==s_star_collection[icon_num].display_frame)
        	                    {
        	                        //星星出现的概率
        			            random=rand()%7;
                                        if(1==random)
                                            {
                                                s_star_collection[icon_num].display_frame=speed*STARNUM;
                                            }
        	                    }
        				else
        					{
        						if(s_star_collection[icon_num].display_frame<=STARNUM)
        							{
        								star_number=s_star_collection[icon_num].display_frame-1;
        							}
        						else 
        							{
        								star_number=STARNUM*speed-s_star_collection[icon_num].display_frame;
        							}
        						if(s_star_collection[icon_num].is_big)
        							{ 
        								w=Big_Size.w;
        								h=Big_Size.h;
        								current_buff = exptr->big_star_buf + star_number*big_star_size;
        							}
        						else
        							{
        								w=Small_Size.w;
        								h=Small_Size.h;
        								current_buff = exptr->small_star_buf + star_number*small_star_size;
        							}						
        						star_up_window.w = w;
                                                star_up_window.h = h;
                                                star_offset.x = s_star_collection[icon_num].location.x;
                                                star_offset.y =s_star_collection[icon_num].location.y;
        						S2D_BlendingBehind_ARGB888(bg_win_size,out_buff_ptr,star_up_window,star_offset,current_buff,out_buff_ptr);
                                               if(0==temp_frame%speed)
                                                {
                                                       s_star_collection[icon_num].display_frame--;
                                                }
        					}
        			}
	    
	temp_frame++;
	if(temp_frame>=20*speed)
		{
			temp_frame=0;
		}
    
		//blend stars

#endif
#ifdef CALTIME
	cal_time=SCI_GetTickCount()-cal_time;
	SCI_TRACE_LOW("Uzone_calc, cost_time ONE frame = %d ms",cal_time);
#endif	
	return SCI_TRUE;
}
#if 0
LOCAL BOOLEAN Slim_Landscape_Mode_Back_Up(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr)
{	

	int i=0,page_wid=0,page_height=0,touch_height=0;
	ROTA_DATA_T  current_page_axis_array[PAGE_NUM+1]={0};
	COLOR32 * current_page_buffer_array[PAGE_NUM+1]={0};
	uint32 image_size = page_size.w*page_size.h*sizeof(uint8);
    S3D_UZONE_EXPANDPTR_T* exptr = PNULL;
#ifdef UZONE_STAR_EFFECT
	//stars
	uint32 icon_num=0,speed=0,star_frame=0,move_frame=0,star_number=0,w=0,h=0;
	uint32 * current_buff=PNULL;
	IMGREF_SIZE_T Small_Size={0},Big_Size={0};
	static uint temp_frame=0;
    IMGREF_SIZE_T	star_up_window = {0};
	FIX16_POINT_T	star_offset = {0};
    uint32 big_star_size = 0;
    uint32 small_star_size = 0;
	//stars
	#endif
#ifdef UZONE_BORDER_SHINING	
    //Benny added
	COLOR32* output = PNULL;
    PIC_CIRCULAR_JOINT_T Joint = {0};
	IMGREF_RECT_T ShiningZone = {0};
    IMGREF_SIZE_T shadow_size = {0};	
	IMGREF_SIZE_T orignal_size={0};    
	COLOR32* temp_reverse = PNULL;
    //uint32* shining_ori_img = PNULL;
#endif
#ifdef CALTIME
	static int cal_time=0,whole_time1=0,whole_time2=0;
	whole_time1=SCI_GetTickCount();
	SCI_TRACE_LOW("Uzone_total, cost_time ONE frame = %d ms", whole_time1-whole_time2);
	whole_time2=SCI_GetTickCount();
	cal_time=SCI_GetTickCount();
#endif	

	touch_height=bg_win_size.w/2;
	page_wid=page_size.w;
	page_height=page_size.h;
	
   	if((page_num<5)||(0==total_frame_num)||(start_location>page_num))
   		{
   			return SCI_FALSE;
   		}
    //shining_ori_img = page_buff_array + image_size*(page_num);

	  //静止态
	if(0== cur_frame_num) 
       {
       	for(i=0;i<PAGE_NUM;i++)
       		{
       			//s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/16;
       			s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/32;
			}
		s_page_axis_array[0].axis_z=TOUCH_DEEP+(page_wid+2*PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[1].axis_z=TOUCH_DEEP+(page_wid/2+PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[2].axis_z=TOUCH_DEEP;
		s_page_axis_array[3].axis_z=s_page_axis_array[1].axis_z;
		s_page_axis_array[4].axis_z=s_page_axis_array[0].axis_z;
		

		s_page_axis_array[1].axis_x=touch_height-page_wid/2-(page_wid/2+PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536;
		s_page_axis_array[0].axis_x=touch_height-page_wid/2-(page_wid+2*PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536-page_wid/2;
		s_page_axis_array[2].axis_x=touch_height;
		s_page_axis_array[3].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[1].axis_x);
		s_page_axis_array[4].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[0].axis_x);
	}
	//切换时叶片开始动起来，计算个帧时叶片的转轴信息。并找出要显示的六幅图像
	if(SCI_TRUE==Is_go_up)
		{	//头结点赋值
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=1;i<PAGE_NUM;i++)
       		{	
       			current_page_axis_array[i].axis_z=s_page_axis_array[i].axis_z+(s_page_axis_array[i-1].axis_z-s_page_axis_array[i].axis_z)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].axis_x=s_page_axis_array[i].axis_x+(s_page_axis_array[i-1].axis_x-s_page_axis_array[i].axis_x)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].angle=s_page_axis_array[i].angle+(s_page_axis_array[i-1].angle-s_page_axis_array[i].angle)*cur_frame_num/total_frame_num;
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			//current_page_axis_array[2].angle=s_page_axis_array[2].angle+(s_page_axis_array[1].angle-s_page_axis_array[2].angle)*GDI_sqrt(cur_frame_num*total_frame_num)/(total_frame_num);
			current_page_axis_array[3].angle=s_page_axis_array[3].angle+(s_page_axis_array[2].angle-s_page_axis_array[3].angle)*(cur_frame_num*cur_frame_num)/(total_frame_num*total_frame_num);
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[PAGE_NUM-1].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location+PAGE_NUM<page_num)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location+PAGE_NUM)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location+PAGE_NUM-page_num)*image_size;
				}
			
		}
	else if(SCI_FALSE==Is_go_up)
		{
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			current_page_buffer_array[0]=page_buff_array+((uint32)start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=0;i<PAGE_NUM-1;i++)
       		{	
       			current_page_axis_array[i].axis_z=s_page_axis_array[i].axis_z+(s_page_axis_array[i+1].axis_z-s_page_axis_array[i].axis_z)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].axis_x=s_page_axis_array[i].axis_x+(s_page_axis_array[i+1].axis_x-s_page_axis_array[i].axis_x)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].angle=s_page_axis_array[i].angle+(s_page_axis_array[i+1].angle-s_page_axis_array[i].angle)*cur_frame_num/total_frame_num;
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			current_page_axis_array[2].angle=s_page_axis_array[2].angle+(int)((s_page_axis_array[3].angle-s_page_axis_array[2].angle)*(int)(GDI_sqrt((uint32)(cur_frame_num*total_frame_num)))/(total_frame_num));
			current_page_axis_array[1].angle=s_page_axis_array[1].angle+(s_page_axis_array[2].angle-s_page_axis_array[1].angle)*(cur_frame_num*cur_frame_num)/(total_frame_num*total_frame_num);

			current_page_axis_array[PAGE_NUM-1].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM-1].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+(current_page_axis_array[PAGE_NUM-1].axis_x-s_page_axis_array[PAGE_NUM-1].axis_x)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM-1<page_num)
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+((uint32)start_location+PAGE_NUM-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+(uint32)(start_location+PAGE_NUM-1-page_num)*image_size;
				}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[0].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[0].axis_x-PAGE_GAP-page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[0].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location-1<0)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(page_num-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+((uint32)start_location-1)*image_size;
				}
		}
	//找出六幅要显示的图像


    //expand ptr
    if(PNULL == exp_ptr)
	{
		return FALSE;
	}
  	exptr = (S3D_UZONE_EXPANDPTR_T*)exp_ptr;

    #ifdef UZONE_BORDER_SHINING	
	if(PNULL==exptr->shining_use_buf
		||PNULL==exptr->Point_Add_ptr
		||PNULL==exptr->shadow_use_buf
		||PNULL==exptr->ori_glass_buf
		)
	{
		return FALSE;
	}
	#endif
    #ifdef UZONE_STAR_EFFECT
	if(PNULL==exptr->small_star_buf
		||PNULL==exptr->big_star_buf
		)
	{
		return FALSE;
	}
    #endif
	//进行旋转贴图
if(0==cur_frame_num&&is_in_idle)
{
#ifdef UZONE_BORDER_SHINING	
    	orignal_size.w=UZONE_ITEM_BG_WIDTH;
    	orignal_size.h=UZONE_ITEM_BG_HEIGHT;
        output = exptr->shining_use_buf;//(COLOR32*)SCI_ALLOCA(page_size.w*page_size.h*sizeof(COLOR32));
    	SCI_MEMCPY(output,exptr->ori_glass_buf,orignal_size.w * orignal_size.h*sizeof(COLOR32));
       /// SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
        ShiningZone.x = 4;
    	ShiningZone.y = 4;
    	ShiningZone.w = orignal_size.w-8;
    	ShiningZone.h = orignal_size.h-12;
    	/*ShiningZone.x = 1;
    	ShiningZone.y = 2;
    	ShiningZone.w = orignal_size.w-2;
    	ShiningZone.h = orignal_size.h-4;*/

        Joint.xDelta_vertex = 5;
    	Joint.yDelta_vertex = 5;
    	Slim_Mode_borderShiningCalc(&orignal_size,output,&ShiningZone,&Joint,exptr->Point_Add_ptr);

    	temp_reverse = output;
    	output = exptr->shadow_use_buf;    

        shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
        shadow_size.w = page_size.w; 
    	Slim_Landscape_Create_Shadow(orignal_size,shadow_size,temp_reverse,output);

    	//SCI_MEMSET(output,0xff,page_size.w*page_size.h*sizeof(COLOR32));
    	//SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
   	
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								output,
    								current_page_axis_array[2], 
    							1);


#else
    	Slim_Landscape_Make_Rotate(bg_win_size, 
    								page_size, 
    								out_buff_ptr, 
    								current_page_buffer_array[2], 
    								//centre_buff,
    								current_page_axis_array[2], 
    							1);	
#endif
}
		else
			{
				Slim_Landscape_Make_Rotate(bg_win_size, 
								page_size, 
								out_buff_ptr, 
								current_page_buffer_array[2], 
								//centre_buff,
								current_page_axis_array[2], 
								1);
			}
		for(i=0;i<PAGE_NUM+1;i++)
	       		{	if(2!=i)
					{
		       			Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											current_page_buffer_array[i], 
											current_page_axis_array[i], 
											1);
					}
				}
#ifdef UZONE_STAR_EFFECT
    big_star_size = UZONE_BIG_STAR_HEIGHT*UZONE_BIG_STAR_WIDTH;
    small_star_size = UZONE_SMALL_STAR_HEIGHT*UZONE_SMALL_STAR_WIDTH;
    
		//blend stars
	Small_Size.w=UZONE_SMALL_STAR_WIDTH;
	Small_Size.h=UZONE_SMALL_STAR_HEIGHT;
	Big_Size.w=UZONE_BIG_STAR_WIDTH;
	Big_Size.h=UZONE_BIG_STAR_HEIGHT;
    
	if(0==temp_frame)
		{
		for(i=0;i<7;i++)
			{
			  s_star_collection[i].start_frame=rand()%(STARNUM*2);
			}
		}
	speed=2;
    
    #ifdef WIN32
	SCI_Sleep(20);
    #endif
    
	star_frame=temp_frame/speed;
	for(icon_num=0;icon_num<7;icon_num++)
			{	move_frame=star_frame-s_star_collection[icon_num].start_frame;
				if(move_frame<=STARNUM*2-1)
					{
						if(move_frame<STARNUM)
							{
								star_number=move_frame;
							}
						else 
							{
								star_number=STARNUM*2-1-move_frame;
							}
						if(s_star_collection[icon_num].is_big)
							{ 
								w=Big_Size.w;
								h=Big_Size.h;
								current_buff = exptr->big_star_buf + star_number*big_star_size;
							}
						else
							{
								w=Small_Size.w;
								h=Small_Size.h;
								current_buff = exptr->small_star_buf + star_number*small_star_size;
							}						
						star_up_window.w = w;
                        star_up_window.h = h;
                        star_offset.x = s_star_collection[icon_num].location.x;
                        star_offset.y =s_star_collection[icon_num].location.y;
						S2D_BlendingBehind_ARGB888(bg_win_size,out_buff_ptr,star_up_window,star_offset,current_buff,out_buff_ptr);
					}
			}
	temp_frame++;
	if(temp_frame>=20*speed)
		{
			temp_frame=0;
		}
		//blend stars
#endif
#ifdef CALTIME
	cal_time=SCI_GetTickCount()-cal_time;
	SCI_TRACE_LOW("Uzone_calc, cost_time ONE frame = %d ms",cal_time);
#endif	
	return SCI_TRUE;
}
#endif
LOCAL void Slim_Portrait_Make_Rotate(IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,COLOR32 * Out_Color_Buffer,COLOR32 * In_Color_Buffer,ROTA_DATA_T img_data,int is_aixs_x)
{    
     S3D_ROTATION_GEOMETRY_T  s3d_geometry;
     S3D_ROTATION_IMG_T   s3d_src_img;
     S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img;
     
     s3d_geometry.img_size.w=img_size.w;
     s3d_geometry.img_size.h=img_size.h;
     s3d_geometry.img_size.x =0;
     s3d_geometry.img_size.y = 0;

     s3d_geometry.angle=img_data.angle;
     s3d_geometry.eye.x=win_size.w/2;
     s3d_geometry.eye.y=win_size.h/2;
     s3d_geometry.eye.z=-2*win_size.h;

     s3d_geometry.axis.x=img_data.axis_x;
     s3d_geometry.axis.y=img_data.axis_y;
     s3d_geometry.axis.z=img_data.axis_z;
     s3d_geometry.axis.axis=is_aixs_x;
     s3d_geometry.is_axis_use_default = SCI_FALSE;
     s3d_geometry.is_eye_use_default = SCI_FALSE;

     s3d_src_img.src_ptr=In_Color_Buffer;
     s3d_src_img.src_type=S3D_DATA_ARGB888;

     s3d_img.color_ptr=Out_Color_Buffer;
     s3d_img.size.h=win_size.h;
     s3d_img.size.w=win_size.w;

     S3D_ImageMidAxisRotation ( &(s3d_geometry),&(s3d_src_img),&(s3d_img));
}
LOCAL BOOLEAN Slim_Portrait_Mode(IMGREF_SIZE_T bg_win_size,
								COLOR32 * out_buff_ptr, 
								IMGREF_SIZE_T page_size,
								COLOR32 * page_buff_array,
								int page_num,
								int start_location,
								int cur_frame_num,
								int total_frame_num,
								BOOLEAN Is_go_up)
{
	int i=0,page_wid=0,page_height=0, touch_height=bg_win_size.h/2;
	ROTA_DATA_T  current_page_axis_array[PAGE_NUM+1]={0};
	COLOR32 * current_page_buffer_array[PAGE_NUM+1]={0};
	uint32 image_size = page_size.w*page_size.h*sizeof(uint8);
	
	page_wid=page_size.w;
	page_height=page_size.h;
	
   	if((page_num<5)||(0==total_frame_num)||(start_location>page_num))
   		{
   			return SCI_FALSE;
   		}

	for(i=0;i<page_num;i++)
	{

	}
	  //静止态
	if(0== cur_frame_num) 
       {
       	for(i=0;i<PAGE_NUM;i++)
       		{
       			s_page_axis_array[i].axis_x=bg_win_size.w/2;
			}
		s_page_axis_array[0].axis_z=TOUCH_DEEP+(page_height+2*PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[1].axis_z=TOUCH_DEEP+(page_height/2+PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[2].axis_z=TOUCH_DEEP;
		s_page_axis_array[3].axis_z=s_page_axis_array[1].axis_z;
		s_page_axis_array[4].axis_z=s_page_axis_array[0].axis_z;
		

		s_page_axis_array[1].axis_y=touch_height-page_height/2-(page_height/2+PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536;
		s_page_axis_array[0].axis_y=touch_height-page_height/2-(page_height+2*PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536-page_height/2;
		s_page_axis_array[2].axis_y=touch_height;
		s_page_axis_array[3].axis_y=touch_height+(s_page_axis_array[2].axis_y-s_page_axis_array[1].axis_y);
		s_page_axis_array[4].axis_y=touch_height+(s_page_axis_array[2].axis_y-s_page_axis_array[0].axis_y);
	}
	//切换时叶片开始动起来，计算个帧时叶片的转轴信息。并找出要显示的六幅图像
	if(SCI_TRUE==Is_go_up)
		{	//头结点赋值
			for(i=0;i<PAGE_NUM+1;i++)
       		{
       			current_page_axis_array[i].axis_x=bg_win_size.w/2;
			}
			
			current_page_buffer_array[0]=page_buff_array+(uint32)start_location*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_y=s_page_axis_array[0].axis_y-(page_height+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=1;i<PAGE_NUM;i++)
       		{	
       			current_page_axis_array[i].axis_z=s_page_axis_array[i].axis_z+(s_page_axis_array[i-1].axis_z-s_page_axis_array[i].axis_z)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y+(s_page_axis_array[i-1].axis_y-s_page_axis_array[i].axis_y)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].angle=s_page_axis_array[i].angle+(s_page_axis_array[i-1].angle-s_page_axis_array[i].angle)*cur_frame_num/total_frame_num;
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			//current_page_axis_array[2].angle=s_page_axis_array[2].angle+(s_page_axis_array[1].angle-s_page_axis_array[2].angle)*GDI_sqrt(cur_frame_num*total_frame_num)/(total_frame_num);
			current_page_axis_array[3].angle=s_page_axis_array[3].angle+(s_page_axis_array[2].angle-s_page_axis_array[3].angle)*(cur_frame_num*cur_frame_num)/(total_frame_num*total_frame_num);
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y+PAGE_GAP+page_height;	
			current_page_axis_array[PAGE_NUM].axis_y=current_page_axis_array[PAGE_NUM].axis_y+(s_page_axis_array[PAGE_NUM-1].axis_y-current_page_axis_array[PAGE_NUM].axis_y)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM<page_num)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location+PAGE_NUM)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location+PAGE_NUM-page_num)*image_size;
				}
			
		}
	else if(SCI_FALSE==Is_go_up)
		{
			for(i=0;i<PAGE_NUM+1;i++)
       		{
       			current_page_axis_array[i].axis_x=bg_win_size.w/2;
			}
			
			current_page_buffer_array[0]=page_buff_array+(uint32)(start_location)*image_size;
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_y=s_page_axis_array[0].axis_y-(page_height+PAGE_GAP)*cur_frame_num/total_frame_num;
			//判断第一页的视角
			
			for(i=0;i<PAGE_NUM-1;i++)
       		{	
       			current_page_axis_array[i].axis_z=s_page_axis_array[i].axis_z+(s_page_axis_array[i+1].axis_z-s_page_axis_array[i].axis_z)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y+(s_page_axis_array[i+1].axis_y-s_page_axis_array[i].axis_y)*cur_frame_num/total_frame_num;
				current_page_axis_array[i].angle=s_page_axis_array[i].angle+(s_page_axis_array[i+1].angle-s_page_axis_array[i].angle)*cur_frame_num/total_frame_num;
				if(start_location+i<page_num)
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i)*image_size;
					}
				else
					{
						current_page_buffer_array[i]=page_buff_array+(uint32)(start_location+i-page_num)*image_size;
					}
			}
			current_page_axis_array[2].angle=s_page_axis_array[2].angle+(s_page_axis_array[3].angle-s_page_axis_array[2].angle)*(int)(GDI_sqrt((uint32)(cur_frame_num*total_frame_num)))/(total_frame_num);
			current_page_axis_array[1].angle=s_page_axis_array[1].angle+(s_page_axis_array[2].angle-s_page_axis_array[1].angle)*(cur_frame_num*cur_frame_num)/(total_frame_num*total_frame_num);

			current_page_axis_array[PAGE_NUM-1].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM-1].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM-1].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y+PAGE_GAP+page_height;	
			current_page_axis_array[PAGE_NUM-1].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y+(current_page_axis_array[PAGE_NUM-1].axis_y-s_page_axis_array[PAGE_NUM-1].axis_y)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM-1<page_num)
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+(uint32)(start_location+PAGE_NUM-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM-1]=page_buff_array+(uint32)(start_location+PAGE_NUM-1-page_num)*image_size;
				}
			
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[0].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[0].axis_y-PAGE_GAP-page_height;	
			current_page_axis_array[PAGE_NUM].axis_y=current_page_axis_array[PAGE_NUM].axis_y+(s_page_axis_array[0].axis_y-current_page_axis_array[PAGE_NUM].axis_y)*cur_frame_num/total_frame_num;
			if(start_location-1<0)
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(page_num-1)*image_size;
				}
			else
				{
					current_page_buffer_array[PAGE_NUM]=page_buff_array+(uint32)(start_location-1)*image_size;
				}
		}
	//找出六幅要显示的图像
	
	
	
	//进行旋转贴图
		Slim_Portrait_Make_Rotate(bg_win_size, 
							page_size, 
							out_buff_ptr, 
							current_page_buffer_array[2], 
							current_page_axis_array[2], 
							0);
		for(i=0;i<PAGE_NUM+1;i++)
	       		{	if(2!=i)
					{
		       			Slim_Portrait_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											current_page_buffer_array[i], 
											current_page_axis_array[i], 
											0);
					}
				}
		
	
	return SCI_TRUE;
}

LOCAL void Draw_Item(int32 glass_w,int32 glass_h,uint32 * glass_buff,IMGREF_RECT_T draw_rect,uint32 * item_buffer)
{   
    
    int32 y=0;
    int32 start_x=0,end_x=0,start_y=0,end_y=0;
    uint32 * dst_ptr=PNULL;
    uint32 * fill_ptr=PNULL;
    uint32 * end_ptr=PNULL;
    if((PNULL==glass_buff)||(PNULL==item_buffer))
        {
            return;
        }
    start_y=MAX(draw_rect.y,0);
    end_y=MIN(draw_rect.y+draw_rect.h,glass_h);
    start_x=MAX(draw_rect.x,0);
    end_x=MIN(draw_rect.x+draw_rect.w,glass_w);
    for(y=start_y;y<end_y;y++)
        {
            dst_ptr=&glass_buff[y*glass_w+start_x];
            fill_ptr=&item_buffer[(y-draw_rect.y)*draw_rect.w];
            end_ptr=fill_ptr+(end_x-start_x);
            while(fill_ptr<end_ptr)
                {   
                    
                    * dst_ptr++=*fill_ptr++;
                  
                  
                }
        }
    
}

LOCAL void Get_Size_Reverted_Buffer(IMGREF_SIZE_T src_size, //带阴影的卡皮大学w=page_h*5/4 ,h=page_w.
                                                                   COLOR32         * tempSrcColor_reverse,
                                                                    COLOR32         *  item_buffer,
                                                                    IMGREF_RECT_T  draw_rect   //翻转后的图像的偏移和大小
                                                                    )
{
    int32 y=0;
    uint32 * dst_ptr=PNULL;
    uint32 * fill_ptr=PNULL;
    uint32 * end_ptr=PNULL;
    if((PNULL==tempSrcColor_reverse)||(PNULL==item_buffer))
        {
            return;
        }
    
    for(y=0;y<draw_rect.h;y++)
        {
            fill_ptr=&tempSrcColor_reverse[(y+draw_rect.y)*src_size.w+draw_rect.x];
            dst_ptr=&item_buffer[y*draw_rect.w];
            end_ptr=dst_ptr+draw_rect.w;
            while(dst_ptr<end_ptr)
                {
                   * dst_ptr++=*fill_ptr++;
                }
        }
    
   
}

//得到带影子的page, 准备算法需要的图标，字串，影子上的倒转buffer。
LOCAL void Get_Buffer(IMGREF_SIZE_T page_size, //图片大小，不带阴影的图片大小
                                        IMGREF_SIZE_T shadow_size,    
                                        COLOR32 * pColor_in, 			//输入图片buffer
                                        COLOR32 * tempSrcColor_reverse,
                                        COLOR32 * icon_buffer,
                                        IMGREF_RECT_T * icon_rect,
                                        COLOR32 *  Str_buffer,
                                        IMGREF_RECT_T * Str_rect,
                                        COLOR32 * Str_Shadow_buffer,
                                        IMGREF_RECT_T * Str_Shadow_rect ,
                                        uint32 icon_num)
{
    uint32 i=0;
    IMGREF_RECT_T new_icon_rect={0};
    IMGREF_RECT_T new_Str_rect={0};
    IMGREF_RECT_T new_Str_Shadow_rect={0};
    IMGREF_SIZE_T src_size={0};
    //取图片的翻转图像和 翻转宽高
    new_icon_rect.x=icon_rect->y;
    new_icon_rect.y=icon_rect->x;
    new_icon_rect.w=icon_rect->h;
    new_icon_rect.h=icon_rect->w;
    new_Str_rect.x=Str_rect->y;
    new_Str_rect.y=Str_rect->x;
    new_Str_rect.w=Str_rect->h;
    new_Str_rect.h=Str_rect->w;
    new_Str_Shadow_rect.x=2*(page_size.h-4)-(Str_rect->y+Str_rect->h);
    new_Str_Shadow_rect.y=Str_rect->x;
    new_Str_Shadow_rect.w=Str_rect->h;
    new_Str_Shadow_rect.h=Str_rect->w;
    src_size.w=page_size.h+shadow_size.h;
    src_size.h=page_size.w;
    for(i=0;i<icon_num;i++)
        {   
            //卡片上画小图标
           Draw_Item(page_size.w,page_size.h, pColor_in, *icon_rect,(icon_buffer+i*icon_rect->w*icon_rect->h));
            //卡片上画字串
          Draw_Item(page_size.w,page_size.h, pColor_in, *Str_rect,(Str_buffer+i*Str_rect->w*Str_rect->h));
            //取得倒影
            Slim_Landscape_Create_Shadow(page_size, shadow_size,  pColor_in, tempSrcColor_reverse);
           //从倒影中抠出图片
           Get_Size_Reverted_Buffer( src_size, tempSrcColor_reverse,(icon_buffer+i*icon_rect->w*icon_rect->h), new_icon_rect);
           Get_Size_Reverted_Buffer( src_size, tempSrcColor_reverse,(Str_buffer+i*Str_rect->w*Str_rect->h), new_Str_rect);
           Get_Size_Reverted_Buffer( src_size, tempSrcColor_reverse,(Str_Shadow_buffer+i*Str_rect->w*Str_rect->h), new_Str_Shadow_rect);
            
        }
    icon_rect->x=new_icon_rect.x;
    icon_rect->y=new_icon_rect.y;
    icon_rect->w=new_icon_rect.w;
    icon_rect->h=new_icon_rect.h;
    Str_rect->x=new_Str_rect.x;
    Str_rect->y=new_Str_rect.y;
    Str_rect->w=new_Str_rect.w;
    Str_rect->h=new_Str_rect.h;
    Str_Shadow_rect->x=new_Str_Shadow_rect.x;
    Str_Shadow_rect->y=new_Str_Shadow_rect.y;
    Str_Shadow_rect->w=new_Str_Shadow_rect.w;
    Str_Shadow_rect->h=new_Str_Shadow_rect.h;
   
}

LOCAL BOOLEAN Slim_Landscape_Mode_New(IMGREF_SIZE_T bg_win_size,//横屏时屏幕宽高。
										COLOR32 * out_buff_ptr, 
										IMGREF_SIZE_T page_size,//带阴影的图片大小宽为小图片宽，高为小图片高的四分之五。
										COLOR32 * page_buff_array,
										int page_num,
										int start_location,
										int cur_frame_num,
										int total_frame_num,
										BOOLEAN Is_go_up,
										BOOLEAN is_in_idle,
										void* exp_ptr)
{	

    int i=0,page_wid=0,page_height=0,touch_height=0;
    ROTA_DATA_T  current_page_axis_array[PAGE_NUM+1]={0};
   // COLOR32 * current_page_buffer_array[PAGE_NUM+1]={0};
    uint32 icon_size={0};
    uint32 str_size={0};
     uint32 * icon_buffer=PNULL; 
      uint32 * str_buffer=PNULL;
      uint32 * str_shadow_buffer=PNULL;
    S3D_UZONE_EXPANDPTR_T* exptr = PNULL;
    uint32 num=0;
#ifdef UZONE_STAR_EFFECT
	//stars
	uint32 icon_num=0,star_number=0,w=0,h=0;
	uint32 * current_buff=PNULL;
	IMGREF_SIZE_T Small_Size={0},Big_Size={0};
	static uint32 temp_frame=0;
     uint8 random=0;
    IMGREF_SIZE_T	star_up_window = {0};
	FIX16_POINT_T	star_offset = {0};
    uint32 big_star_size = 0;
    uint32 small_star_size = 0;
	//stars
	#endif
#ifdef UZONE_BORDER_SHINING	
    //Benny added
	COLOR32* output = PNULL;
    PIC_CIRCULAR_JOINT_T Joint = {0};
	IMGREF_RECT_T ShiningZone = {0};
    IMGREF_SIZE_T shadow_size = {0};	
	IMGREF_SIZE_T orignal_size={0};    
	COLOR32* temp_reverse = PNULL;
    //uint32* shining_ori_img = PNULL;
#endif

#ifdef CALTIME
	static int cal_time=0,whole_time1=0,whole_time2=0;
	whole_time1=SCI_GetTickCount();
	SCI_TRACE_LOW("Uzone_total, cost_time ONE frame = %d ms", whole_time1-whole_time2);
	whole_time2=SCI_GetTickCount();
	cal_time=SCI_GetTickCount();
#endif	
    if(PNULL == exp_ptr)
	{
		return FALSE;
	}
  	exptr = (S3D_UZONE_EXPANDPTR_T*)exp_ptr;
    icon_size=exptr->icon_rect->w*exptr->icon_rect->h*sizeof(uint8);
    str_size=exptr->Str_rect->w*exptr->Str_rect->h*sizeof(uint8);
    icon_buffer=exptr->icon_buffer;
    str_buffer=exptr->Str_buffer;
    str_shadow_buffer=exptr->Str_Shadow_buffer;
    #ifdef UZONE_BORDER_SHINING	
	if(PNULL==exptr->shining_use_buf
		||PNULL==exptr->Point_Add_ptr
		||PNULL==exptr->shadow_use_buf
		||PNULL==exptr->ori_glass_buf
		)
	{
		return FALSE;
	}
	#endif
       #ifdef UZONE_STAR_EFFECT
	if(PNULL==exptr->small_star_buf
		||PNULL==exptr->big_star_buf
		)
	{
		return FALSE;
	}
    #endif
//PopCalcTest();
	touch_height=bg_win_size.w/2;
	page_wid=page_size.w;
	page_height=page_size.h;
	
   	if((page_num<5)||(0==total_frame_num)||(start_location>page_num))
   		{
   			return SCI_FALSE;
   		}

	  //静止态
	if(0== cur_frame_num) 
       {
       	for(i=0;i<PAGE_NUM;i++)
       		{
       			s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/16;
       			//s_page_axis_array[i].axis_y=bg_win_size.h/2+bg_win_size.h/32;
			}
		s_page_axis_array[0].axis_z=TOUCH_DEEP+(page_wid+2*PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[1].axis_z=TOUCH_DEEP+(page_wid/2+PAGE_GAP)*s2d_sin_new(ANGLE_TWO)/65536;
		s_page_axis_array[2].axis_z=TOUCH_DEEP;
		s_page_axis_array[3].axis_z=s_page_axis_array[1].axis_z;
		s_page_axis_array[4].axis_z=s_page_axis_array[0].axis_z;
		

		s_page_axis_array[1].axis_x=touch_height-page_wid/2-(page_wid/2+PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536;
		s_page_axis_array[0].axis_x=touch_height-page_wid/2-(page_wid+2*PAGE_GAP)*s2d_cos_new(ANGLE_TWO)/65536-page_wid/2;
		s_page_axis_array[2].axis_x=touch_height;
		s_page_axis_array[3].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[1].axis_x);
		s_page_axis_array[4].axis_x=touch_height+(s_page_axis_array[2].axis_x-s_page_axis_array[0].axis_x);
	}
	//切换时叶片开始动起来，计算个帧时叶片的转轴信息。并找出要显示的六幅图像
	if(SCI_TRUE==Is_go_up)
		{	//头结点赋值
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			//贴第一张图
			Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+(uint32)start_location*icon_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+(uint32)start_location*str_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+(uint32)start_location*str_size);
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
                    Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[0], 
											1);
                   
			//贴第一张图
			for(i=1;i<PAGE_NUM;i++)
       		{	Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
       				if(start_location+i<page_num)
					{
						num=(uint32)(start_location+i);
					}
				else
					{
						num=(uint32)(start_location+i-page_num);
					}
                        Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+num*icon_size);
                        Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+num*str_size);
                        Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+num*str_size);
                        
                    ////贴中间部分图
                        if(2!=i)
					{
		       			Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[i], 
											1);
					}
                         ////贴中间部分图
                         
                         //贴第二 张图
                        else
                            {
                                if(0==cur_frame_num&&is_in_idle)
                                    {
#ifdef UZONE_BORDER_SHINING	
                                        	orignal_size.w=UZONE_ITEM_BG_WIDTH;
                                        	orignal_size.h=UZONE_ITEM_BG_HEIGHT;
                                            output = exptr->shining_use_buf;//(COLOR32*)SCI_ALLOCA(page_size.w*page_size.h*sizeof(COLOR32));
                                        	SCI_MEMCPY(output,exptr->ori_glass_buf,orignal_size.w * orignal_size.h*sizeof(COLOR32));
                                           /// SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));

                                          ShiningZone.x = MMIUZONE_SHININGZONE_X;
                                        	ShiningZone.y = MMIUZONE_SHININGZONE_Y;
                                        	ShiningZone.w = MMIUZONE_SHININGZONE_W;
                                        	ShiningZone.h = MMIUZONE_SHININGZONE_H;
                                          Joint.xDelta_vertex = MMIUZONE_SHININGZONE_JOINT_X;
                                        	Joint.yDelta_vertex = MMIUZONE_SHININGZONE_JOINT_Y;
                                            
                                        	Slim_Mode_borderShiningCalc(&orignal_size,output,&ShiningZone,&Joint,exptr->Point_Add_ptr);

                                        	temp_reverse = output;
                                        	output = exptr->shadow_use_buf;    

                                            shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
                                            shadow_size.w = page_size.w; 
                                        	Slim_Landscape_Create_Shadow(orignal_size,shadow_size,temp_reverse,output);

                                        	//SCI_MEMSET(output,0xff,page_size.w*page_size.h*sizeof(COLOR32));
                                        	//SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
                                       	
                                        	Slim_Landscape_Make_Rotate(bg_win_size, 
                                        								page_size, 
                                        								out_buff_ptr, 
                                        								output,
                                        								current_page_axis_array[2], 
                                        							1);


#else
                                        	Slim_Landscape_Make_Rotate(bg_win_size, 
                                        								page_size, 
                                        								out_buff_ptr, 
                                        								page_buff_array, 
                                        								//centre_buff,
                                        								current_page_axis_array[2], 
                                        							1);	
#endif
                                    }
                        		else
                        			{
                        				Slim_Landscape_Make_Rotate(bg_win_size, 
                        								page_size, 
                        								out_buff_ptr, 
                        								page_buff_array, 
                        								//centre_buff,
                        								current_page_axis_array[2], 
                        								1);
                        			}
                            }
			}
			//贴第二张图
			
			//贴最后一张图
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[PAGE_NUM-1].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location+PAGE_NUM<page_num)
				{
					num=((uint32)start_location+PAGE_NUM);
				}
			else
				{
					num=(uint32)(start_location+PAGE_NUM-page_num);
				}
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+num*icon_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+num*str_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+num*str_size);
                Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[PAGE_NUM], 
											1);
                //贴最后一张图
			
		}
	else if(SCI_FALSE==Is_go_up)
		{
			for(i=0;i<PAGE_NUM;i++)
       		{
       			current_page_axis_array[i].axis_y=s_page_axis_array[i].axis_y;
			}
			
			/*
			current_page_axis_array[0].angle=s_page_axis_array[0].angle;
			current_page_axis_array[0].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[0].axis_x=s_page_axis_array[0].axis_x-(page_wid+PAGE_GAP)*cur_frame_num/total_frame_num;
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+start_location*icon_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+start_location*str_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+start_location*str_size);
                    Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[0], 
											1);
                    //判断第一页的视角
			*/
			for(i=0;i<PAGE_NUM-1;i++)
       		{	
       			Slim_Landscape_Get_Location( i, 
       		                                                    &current_page_axis_array[i],
       		                                                     cur_frame_num,
       		                                                     total_frame_num, 
       		                                                     Is_go_up);
				if(start_location+i<page_num)
					{
						num=(uint32)(start_location+i);
					}
				else
					{
						num=(uint32)(start_location+i-page_num);
					}
                         Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+num*icon_size);
                         Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+num*str_size);
                         Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+num*str_size);
                        if(2!=i)
					{
		       			Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[i], 
											1);
					}
                        else
                            {
                                if(0==cur_frame_num&&is_in_idle)
                                    {
#ifdef UZONE_BORDER_SHINING	
                                        	orignal_size.w=UZONE_ITEM_BG_WIDTH;
                                        	orignal_size.h=UZONE_ITEM_BG_HEIGHT;
                                            output = exptr->shining_use_buf;//(COLOR32*)SCI_ALLOCA(page_size.w*page_size.h*sizeof(COLOR32));
                                        	SCI_MEMCPY(output,exptr->ori_glass_buf,orignal_size.w * orignal_size.h*sizeof(COLOR32));
                                           /// SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));

                                          ShiningZone.x = MMIUZONE_SHININGZONE_X;
                                        	ShiningZone.y = MMIUZONE_SHININGZONE_Y;
                                        	ShiningZone.w = MMIUZONE_SHININGZONE_W;
                                        	ShiningZone.h = MMIUZONE_SHININGZONE_H;
                                          Joint.xDelta_vertex = MMIUZONE_SHININGZONE_JOINT_X;
                                        	Joint.yDelta_vertex = MMIUZONE_SHININGZONE_JOINT_Y;
                                            
                                        	Slim_Mode_borderShiningCalc(&orignal_size,output,&ShiningZone,&Joint,exptr->Point_Add_ptr);

                                        	temp_reverse = output;
                                        	output = exptr->shadow_use_buf;    

                                            shadow_size.h = UZONE_ITEM_SHADOW_HEIGHT_H;
                                            shadow_size.w = page_size.w; 
                                        	Slim_Landscape_Create_Shadow(orignal_size,shadow_size,temp_reverse,output);

                                        	//SCI_MEMSET(output,0xff,page_size.w*page_size.h*sizeof(COLOR32));
                                        	//SCI_MEMSET(output,0xff,orignal_size.w * orignal_size.h*sizeof(COLOR32));
                                       	
                                        	Slim_Landscape_Make_Rotate(bg_win_size, 
                                        								page_size, 
                                        								out_buff_ptr, 
                                        								output,
                                        								current_page_axis_array[2], 
                                        							1);


#else
                                        	Slim_Landscape_Make_Rotate(bg_win_size, 
                                        								page_size, 
                                        								out_buff_ptr, 
                                        								page_buff_array, 
                                        								//centre_buff,
                                        								current_page_axis_array[2], 
                                        							1);	
#endif
                                    }
                        		else
                        			{
                        				Slim_Landscape_Make_Rotate(bg_win_size, 
                        								page_size, 
                        								out_buff_ptr, 
                        								page_buff_array, 
                        								//centre_buff,
                        								current_page_axis_array[2], 
                        								1);
                        			}
                            }
			}
			
			current_page_axis_array[PAGE_NUM-1].angle=s_page_axis_array[PAGE_NUM-1].angle;
			current_page_axis_array[PAGE_NUM-1].axis_z=s_page_axis_array[PAGE_NUM-1].axis_z;
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+PAGE_GAP+page_wid;	
			current_page_axis_array[PAGE_NUM-1].axis_x=s_page_axis_array[PAGE_NUM-1].axis_x+(current_page_axis_array[PAGE_NUM-1].axis_x-s_page_axis_array[PAGE_NUM-1].axis_x)*cur_frame_num/total_frame_num;
			if(start_location+PAGE_NUM-1<page_num)
				{
					num=((uint32)start_location+PAGE_NUM-1);
				}
			else
				{
					num=(uint32)(start_location+PAGE_NUM-1-page_num);
				}
                     Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+num*icon_size);
                    Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+num*str_size);
                     Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+num*str_size);
			 Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[PAGE_NUM-1], 
											1);
			current_page_axis_array[PAGE_NUM].angle=s_page_axis_array[0].angle;
			current_page_axis_array[PAGE_NUM].axis_z=s_page_axis_array[0].axis_z;
			current_page_axis_array[PAGE_NUM].axis_x=s_page_axis_array[0].axis_x-PAGE_GAP-page_wid;	
			current_page_axis_array[PAGE_NUM].axis_x=current_page_axis_array[PAGE_NUM].axis_x+(s_page_axis_array[0].axis_x-current_page_axis_array[PAGE_NUM].axis_x)*cur_frame_num/total_frame_num;
			current_page_axis_array[PAGE_NUM].axis_y=s_page_axis_array[PAGE_NUM-1].axis_y;
			if(start_location-1<0)
				{
					num=(uint32)(page_num-1);
				}
			else
				{
					num=((uint32)start_location-1);
				}
                     Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->icon_rect)) , icon_buffer+num*icon_size);
                     Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_rect)) , str_buffer+num*str_size);
                     Draw_Item( page_height,page_wid,page_buff_array ,(* (exptr->Str_Shadow_rect)) , str_shadow_buffer+num*str_size);
                     Slim_Landscape_Make_Rotate(bg_win_size, 
											page_size, 
											out_buff_ptr, 
											page_buff_array, 
											current_page_axis_array[PAGE_NUM], 
											1);
		}


#ifdef UZONE_STAR_EFFECT
    big_star_size = UZONE_BIG_STAR_HEIGHT*UZONE_BIG_STAR_WIDTH;
    small_star_size = UZONE_SMALL_STAR_HEIGHT*UZONE_SMALL_STAR_WIDTH;
    
		//blend stars
	Small_Size.w=UZONE_SMALL_STAR_WIDTH;
	Small_Size.h=UZONE_SMALL_STAR_HEIGHT;
	Big_Size.w=UZONE_BIG_STAR_WIDTH;
	Big_Size.h=UZONE_BIG_STAR_HEIGHT;
        //星星相位速度
      
      
     #ifdef WIN32
	SCI_Sleep(20);
     #endif
	
	            for(icon_num=0;icon_num<STARTOTALNUM;icon_num++)
        			{	
        			        if(0==s_star_collection[icon_num].display_frame)
        	                    {
        	                        //星星出现的概率
        			            random=rand()%12;
                                        if(1==random)
                                            {
                                                s_star_collection[icon_num].display_frame=2*STARNUM;
                                                s_star_collection[icon_num].speed_frame=2+rand()%6;
                                                s_star_collection[icon_num].total_frame_num=2*STARNUM* s_star_collection[icon_num].speed_frame;
                                            }
        	                    }
        				else
        					{
        						if(s_star_collection[icon_num].display_frame<=STARNUM)
        							{
        								star_number=s_star_collection[icon_num].display_frame-1;
        							}
        						else 
        							{
        								star_number=STARNUM*2-s_star_collection[icon_num].display_frame;
        							}
        						if(s_star_collection[icon_num].is_big)
        							{ 
        								w=Big_Size.w;
        								h=Big_Size.h;
        								current_buff = exptr->big_star_buf + star_number*big_star_size;
        							}
        						else
        							{
        								w=Small_Size.w;
        								h=Small_Size.h;
        								current_buff = exptr->small_star_buf + star_number*small_star_size;
        							}						
        						star_up_window.w = w;
                                                star_up_window.h = h;
                                                star_offset.x = s_star_collection[icon_num].location.x*MMIUZONE_LCD_W/MMIUZONE_DESIGN_BASE_LCD_W;
                                                star_offset.y =s_star_collection[icon_num].location.y*MMIUZONE_LCD_H/MMIUZONE_DESIGN_BASE_LCD_H;
        						S2D_BlendingBehind_ARGB888(bg_win_size,out_buff_ptr,star_up_window,star_offset,current_buff,out_buff_ptr);
                                              s_star_collection[icon_num].total_frame_num--;
                                              s_star_collection[icon_num].display_frame= (s_star_collection[icon_num].total_frame_num+1)/ s_star_collection[icon_num].speed_frame;
                                              
        					}
        			}
	    
	temp_frame++;
	if(temp_frame>=10)
		{
			temp_frame=0;
		}
    
		//blend stars

#endif
#ifdef CALTIME
	cal_time=SCI_GetTickCount()-cal_time;
	SCI_TRACE_LOW("Uzone_calc, cost_time ONE frame = %d ms",cal_time);
#endif	
	return SCI_TRUE;
}

#endif



/*Edit by script, ignore 3 case. Thu Apr 26 19:01:09 2012*/ //IGNORE9527
