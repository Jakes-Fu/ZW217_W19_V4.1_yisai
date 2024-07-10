/****************************************************************************
** File Name:      mmi_slide.c
** Author:                                                                  
** Date:           2012/02/10
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This is mmi slide win define file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 02/10/2012        xiaoming             Create
** 
****************************************************************************/
#define _MMI_SLIDE_C_

#include "mmi_app_common_trc.h"
#ifdef MMI_SLIDE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "mmi_slide.h"
#include "mmidisplay_data.h"
#include "guires.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define WIN_SYM_LAYER_HEIGHT      16                            //窗口指示器的层高
#define WIN_SYM_POINT_SPACE       8                             //窗口指示器的间距
#define WIN_SYM_POINT_WIDTH       16                            //窗口指示器的宽度
#define CHILD_WIN_MAX_COUNT       MMISLIDE_WIN_TOTAL_COUNT      //子窗口最大数目
#define RECOVER_STEP              20                            //自动move时的步长
#define SLIDE_TOUCH_FLING_TIME    (0.025f)                      //每秒40帧
#define UILAYER_ALPHA_MAX         255
#define AUTO_HIDE_TICK            1000                          //自动隐藏启动的毫秒数
#define HIDE_OUT_TICK             100                           //自动隐藏动画的帧间隔

#define TRACE_CHARS               ("[MMI_SLIDE]:")             //Trace关键字
#define SLIDE_DEBUG_TRACE         MMISLIDE_StrTraceOut

#define ROUND_VALUE(x)   ((x) &= 0xFFFE)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    SLIDE_MOVE_NONE = 0xff,
    SLIDE_MOVE_LEFT,
    SLIDE_MOVE_RIGHT,
    SLIDE_MOVE_UP,
    SLIDE_MOVE_DOWN,
    SLIDE_MOVE_INVALID
}SLIDE_MOVE_DIRECTION_E;

typedef enum
{
    UPDAT_TYPE_INVALID,
    UPDAT_TYPE_NONE,                          //不更新
    UPDAT_TYPE_SYNCH,                         //同步更新
    UPDAT_TYPE_ASYNCH                         //异步更新
}UPDATE_TYPR_E;

//备份的ctrl信息
typedef struct SLIDE_BACKUP_CTRL_INFO_T
{
    MMI_CTRL_ID_T active_ctrl_id;
    MMI_WIN_ID_T  active_win_id;
    MMI_CTRL_ID_T relative_ctl_id;
    MMI_WIN_ID_T  relative_win_id;
}SLIDE_BACKUP_CTRL_INFO;

typedef struct
{
    MMK_TP_SLIDE_E          state;
    GUI_POINT_T             start_point;        //起始点
    GUI_POINT_T             pre_point;          //前一点
    uint8                   scroll_timer_id;    //页面重绘
    float                   velocity;           //速度

    uint16                  cur_index;          //cur win index
    uint16                  pre_index;          //pre win index
    uint16                  next_index;         //next win index
    uint16                  new_index;          //new win index
    uint16                  sym_index;          //当前指示器index

    BOOLEAN                 is_moving;          //是否滑动状态
    BOOLEAN                 is_sleeping;        //是否休眠状态
    
    SLIDE_MOVE_DIRECTION_E  move_direction;     //当前方向

    SLIDE_BACKUP_CTRL_INFO  backup_ctl_info;   //备份的控件信息
    UILAYER_APPEND_BLT_T    append_array[UILAYER_TOTAL_BLT_COUNT]; //滑动前的blt layer 数组
    uint32                  blt_layer_count;                       //滑动前的blt layer 数组 size
}MMI_SLIDE_MOVE_T;

typedef struct
{
    MMI_SLIDE_INFO_T *cur_slide_info_ptr; //当前slide信息指针
    MMI_SLIDE_MOVE_T *cur_move_info_ptr;  //当前move信息指针
}MMI_SLIDE_ACTIVE_INFO_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL GUI_LCD_DEV_INFO  s_slide_old_layer_info = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE}; //当前窗口buffer
LOCAL GUI_LCD_DEV_INFO  s_slide_new_layer_info = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE}; //目标窗口buffer
LOCAL GUI_LCD_DEV_INFO  s_slide_bg_layer_info  = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE}; //非移动bg buffer
LOCAL GUI_LCD_DEV_INFO  s_slide_bg_layer1_info = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE}; //非移动bg buffer1
LOCAL GUI_LCD_DEV_INFO  s_slide_sym_layer_info = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE}; //窗口指示器buffer

LOCAL uint32            s_valis_handle_top_index                    = 0xff; //当前最大有效句柄索引
LOCAL MMI_SLIDE_HANDLE  s_valis_handle_array[MMISLIDE_TOTAL_COUNT]  = {0}; //有效句柄池
LOCAL MMI_SLIDE_INFO_T  s_slide_info_array[MMISLIDE_TOTAL_COUNT]    = {0}; //实例池 
LOCAL MMI_SLIDE_MOVE_T  s_move_info                                 = {0}; //移动时的数据

LOCAL MMI_SLIDE_ACTIVE_INFO_T s_cur_active_slide_info               = {0}; //激活的实例信息
LOCAL MMI_SLIDE_HANDLE  s_cur_backup_handle                         = MMI_SLIDE_HANDLE_INVALID; //失去焦点时备份当前handle并去激活
LOCAL MMI_WIN_ID_T      s_tp_down_win_id                            = 0;
LOCAL VELOCITY_TRACKER_T  s_slide_velocity_tracker_t = {0};
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

extern MMI_RESULT_E MMK_RunWinProc(
                                   MMI_HANDLE_T         win_handle,
                                   MMI_MESSAGE_ID_E 	msg_id,
                                   DPARAM 				param
                                   );
/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : hook function for slide handle   
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideSystermMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : UpdateAllLayerPos_X
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateAllLayerPos_X(MMI_SLIDE_MOVE_T *move_ptr, int16 x_offset,BOOLEAN is_slide);

/*****************************************************************************/
// Description : StartSlideTimer
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSlideTimer(uint8 *timer_id_ptr, uint32 time_out);

/*****************************************************************************/
// Description : StartSlideTimerEx
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSlideTimerEx(uint8 *timer_id_ptr, uint32 time_out, MMI_TIMER_FUNC func, uint32 param);

/*****************************************************************************/
// Description : StopSlideTimer
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StopSlideTimer(uint8 *timer_id_ptr);

/*****************************************************************************/
// Description : StartMoving
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartMoving(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// Description : StopMoving
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StopMoving(MMI_SLIDE_MOVE_T *move_ptr);

/*****************************************************************************/
// Description : 重设 slide move param
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL void ResetMoveParam(MMI_SLIDE_MOVE_T *move_ptr);

/*****************************************************************************/
//  Description : hook function for slide win tp down
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpDown(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win tp move
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpMove(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param);




/*****************************************************************************/
//  Description : DoSlideBackAnim
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL void  DoSlideBackAnim(MMI_SLIDE_MOVE_T *move_ptr);
/*****************************************************************************/
//  Description : hook function for slide win tp up
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpUp(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win timer
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTimer(uint8 time_id, uint32 param);

/*****************************************************************************/
//  Description : hook function for slide win lose focus
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinLoseFocus(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win get focus
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinGetFocus(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win lcd switch
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinLcdSwitch(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win close window
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinCloseWindow(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param);

/*****************************************************************************/
//  Description : hook function for slide win exit slide move
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  void ExitSlideMove(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, BOOLEAN is_update_screen);

/*****************************************************************************/
// 	Description : 初始化layer数据
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAllSlideLayerData(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : 创建所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : 销毁所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ReleaseAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : 重置所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ResetAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : 创建layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateSlideLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T init_rect);

/*****************************************************************************/
// 	Description : 销毁layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ReleaseSlideLayer(GUI_LCD_DEV_INFO *layer_ptr);

/*****************************************************************************/
// 	Description : Copy lcd buffer到layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyLCDbufferToLayer(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *clip_rect_ptr);

/*****************************************************************************/
// 	Description : Copy窗口buffer到layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyWinbufferToLayerEx(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T dst_win_id, GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *clip_rect_ptr);

/*****************************************************************************/
// 	Description : RunWinMsgEx
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 消息需要该窗口以及它的父窗口或者焦点子窗口处理
/*****************************************************************************/
LOCAL BOOLEAN RunWinMsgEx(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Get slide info pointer from slide handle
//  Global resource dependence :
//  Author:xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL MMI_SLIDE_INFO_T* GetSlideInfoPtr(MMI_SLIDE_HANDLE slide_handle);

/*****************************************************************************/
// 	Description : clean
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ClearSymbolDisInfo(MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : remove
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 显示后隐藏的timer回调
/*****************************************************************************/
LOCAL void RemoveSymbolCB(uint8 timer_id, uint32 param);

/*****************************************************************************/
// 	Description : out
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 渐隐效果的timer回调
/*****************************************************************************/
LOCAL void OutSymboCB(uint8 timer_id, uint32 param);

/*****************************************************************************/
// 	Description : 设置窗口数组 的hook 函数
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlideHookFunc(MMI_WIN_ID_T *win_arr_ptr, uint16 arr_size, BOOLEAN is_true);

/*****************************************************************************/
// 	Description : 窗口是否在某个滑动数组中
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWinInSlideArray(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T win_id, uint16 *index_ptr);

/*****************************************************************************/
// 	Description : 窗口是否在所有滑动数组中
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWinInAllSlideArray(MMI_WIN_ID_T win_id, uint16 *arr_index_ptr, uint16 *win_index_ptr);

/*****************************************************************************/
// 	Description : IsSlideLoseFocus
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSlideLoseFocus(MMI_WIN_ID_T win_id, MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// 	Description : 查找某窗口在数组中的位置
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindWinIndexByWinId(MMI_WIN_ID_T *win_arr_ptr, uint16 arr_size, MMI_WIN_ID_T win_id, uint16 *dst_index_ptr);

/*****************************************************************************/
// 	Description : 查找前一index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindPreIndex(MMI_SLIDE_INFO_T *info_ptr, uint16 src_index, uint16 *dst_index_ptr);

/*****************************************************************************/
// 	Description : 查找后一index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindNextIndex(MMI_SLIDE_INFO_T *info_ptr, uint16 src_index, uint16 *dst_index_ptr);

/*****************************************************************************/
// 	Description : 查找前一窗口ID
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindPreWinId(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T src_win_id, MMI_WIN_ID_T *dst_win_id_ptr);

/*****************************************************************************/
// 	Description : 查找后一窗口ID
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindNextWinId(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T src_win_id, MMI_WIN_ID_T *dst_win_id_ptr);

/*****************************************************************************/
// 	Description : 获取移动的方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL SLIDE_MOVE_DIRECTION_E GetMoveDirection(GUI_POINT_T *begin_ptr, GUI_POINT_T *end_ptr, int32 move_max_pixel);

/*****************************************************************************/
// 	Description : 改变方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ChangeDirection(SLIDE_MOVE_DIRECTION_E *cur_direction_Ptr);

/*****************************************************************************/
// 	Description : 切换窗口
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void SwitchToWin(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : 更新屏幕
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateScreen(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr, GUI_POINT_T *cur_point_ptr,BOOLEAN is_need_update);

/*****************************************************************************/
// 	Description : 更新滑动方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateSlideDirection(MMI_SLIDE_MOVE_T *move_ptr);

/*****************************************************************************/
// 	Description : 计算新指示器所在窗口的index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL uint16 CalcNewSymWinIndex(MMI_SLIDE_MOVE_T *move_ptr, GUI_POINT_T *cur_point_ptr);

/*****************************************************************************/
// 	Description : 隐藏所有blt layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void HideAllBltLayer(MMI_SLIDE_MOVE_T *move_ptr);

/*****************************************************************************/
// 	Description : 恢复所有blt layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void RecoverAllBltLayer(MMI_SLIDE_MOVE_T *move_ptr);

/*****************************************************************************/
// 	Description : 隐藏激活的控件，防止控件刷新时会刷入新层
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: back_info_ptr可为空
/*****************************************************************************/
LOCAL void HideActiveCtrl(MMI_WIN_ID_T win_id, BOOLEAN is_true, SLIDE_BACKUP_CTRL_INFO *back_info_ptr);

/*****************************************************************************/
// Description : 绘制一个窗口指示器
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawSymbolItem(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T item_rect, BOOLEAN is_focus);

/*****************************************************************************/
// Description : 绘制窗口指示器背景
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawSymbolBg(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T bg_rect);

/*****************************************************************************/
// Description : 绘制窗口指示器
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawCurWinSym(GUI_LCD_DEV_INFO* dev_info_ptr, uint16 sym_count, uint16 cur_index);

/*****************************************************************************/
// Description : 显示通用背景图片
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DisplayCommonBg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T *dis_rect_ptr);

/*****************************************************************************/
// 	Description : 显示窗口指示器
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayWinSymbol(MMI_SLIDE_INFO_T *info_ptr);

/*****************************************************************************/
// Description : 初始化数据是否有效
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckInitDataIsValid(MMI_SLIDE_INIT_T *init_data_ptr);

/*****************************************************************************/
// Description : Id 是否有效
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckSlideHandleIsValid(MMI_SLIDE_HANDLE *handle_ptr, uint16 *index_ptr);

/*****************************************************************************/
// Description : 关闭窗口(若窗口非open状态，则退出 )
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CloseWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : 设置焦点到某一个窗口
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GrabWindowFocus(MMI_WIN_ID_T win_id, UPDATE_TYPR_E update_type);

/*****************************************************************************/
//  Description : 输出str trace
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
LOCAL void MMISLIDE_StrTraceOut(const char *x_format, ...);

/*****************************************************************************/
//  Description : hook function for slide handle   
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideSystermMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result           = MMI_RESULT_FALSE;

    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_LONG:
        {
            result = HandleSlideWinTpDown(&s_cur_active_slide_info, win_id, param);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            result = HandleSlideWinTpMove(&s_cur_active_slide_info, win_id, param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            result = HandleSlideWinTpUp(&s_cur_active_slide_info, param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT

    case MSG_GET_FOCUS:
        {
            result = HandleSlideWinGetFocus(&s_cur_active_slide_info, param);
        }
        break;

    case MSG_LOSE_FOCUS:
        {
            result = HandleSlideWinLoseFocus(&s_cur_active_slide_info, win_id, param);
        }
        break;
    case MSG_LCD_SWITCH:
        {
            result = HandleSlideWinLcdSwitch(&s_cur_active_slide_info, win_id, param);
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            result = HandleSlideWinCloseWindow(&s_cur_active_slide_info, param);
        }
        break;

    default:
        break;
    }
    
    return  result;
}

/*****************************************************************************/
// Description : UpdateAllLayerPos_X
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateAllLayerPos_X(MMI_SLIDE_MOVE_T *move_ptr, int16 x_offset,BOOLEAN is_slide)
{
    BOOLEAN                ret        = FALSE;
    BOOLEAN                is_recover = FALSE;
    SLIDE_MOVE_DIRECTION_E direction  = SLIDE_MOVE_INVALID;
    GUI_POINT_T            layer_pos  = {0};
    uint16                 lcd_width  = 0;
    int16                  new_offset = 0;

    if (PNULL == move_ptr)
    {
        return ret;
    }

    direction = move_ptr->move_direction;

    if (SLIDE_MOVE_LEFT == direction || SLIDE_MOVE_RIGHT == direction)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
        UILAYER_GetLayerPosition(&s_slide_old_layer_info, &layer_pos.x, &layer_pos.y);

        if (MMK_TP_SLIDE_RECOVER == move_ptr->state && direction != GetMoveDirection(&move_ptr->start_point, &move_ptr->pre_point, MMISLIDE_TP_MOVE_DIS))
        {
            is_recover = TRUE;
        }

        new_offset = layer_pos.x + x_offset;

        if (SLIDE_MOVE_LEFT == direction)
        {
            ret = (new_offset >= -lcd_width && new_offset <= 0) ? (is_recover ? FALSE : TRUE) : (is_recover ? TRUE : FALSE);
        }
        else
        {
            ret = (new_offset <= lcd_width && new_offset >= 0) ? (is_recover ? FALSE : TRUE) : (is_recover ? TRUE : FALSE);
        }
        
        if (ret)
        {
            new_offset &= 0xFFFE;
            UILAYER_SetLayerPosition(&s_slide_old_layer_info, new_offset, layer_pos.y);

            new_offset += ((SLIDE_MOVE_LEFT == direction) ? ((is_recover ? -1 : 1) * (lcd_width)) : ((is_recover ? 1 : -1) * (lcd_width)));
            
            UILAYER_SetLayerPosition(&s_slide_new_layer_info, new_offset, layer_pos.y);

            SLIDE_DEBUG_TRACE("UpdateAllLayerPos_X, old layer move x is: (%d-------->%d)", layer_pos.x, layer_pos.x + x_offset);
        }
        else if(is_slide)
        {
            if(new_offset < -lcd_width)
            {
                new_offset = -lcd_width;
            }else if(new_offset > lcd_width)
            {
                new_offset = lcd_width;
            }
            else
            {
//                new_offset = 0;
            }
            
            new_offset &= 0xFFFE;
            UILAYER_SetLayerPosition(&s_slide_old_layer_info, new_offset, layer_pos.y);

            //if ((move_ptr->pre_point.x + x_offset)/*cur_point_ptr->x*/ < move_ptr->start_point.x)
            if (new_offset < 0)
            {
                new_offset += lcd_width;
            }
            else
            {
                new_offset += -lcd_width;
            }
//            new_offset += ((SLIDE_MOVE_LEFT == direction) ? ((is_recover ? -1 : 1) * (lcd_width)) : ((is_recover ? 1 : -1) * (lcd_width)));
            
            UILAYER_SetLayerPosition(&s_slide_new_layer_info, new_offset, layer_pos.y);

            SLIDE_DEBUG_TRACE("UpdateAllLayerPos_X, old layer move x is: (%d-------->%d)", layer_pos.x, layer_pos.x + x_offset);
        }
        
    }

    return ret;
}

/*****************************************************************************/
// Description : StartSlideTimer
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSlideTimer(uint8 *timer_id_ptr, uint32 time_out)
{
    return StartSlideTimerEx(timer_id_ptr, time_out, (MMI_TIMER_FUNC)HandleSlideWinTimer, (uint32)(&s_cur_active_slide_info));
}

/*****************************************************************************/
// Description : StartSlideTimerEx
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSlideTimerEx(uint8 *timer_id_ptr, uint32 time_out, MMI_TIMER_FUNC func, uint32 param)
{
    BOOLEAN      ret        = TRUE;
    
    if (PNULL == timer_id_ptr || 0 == time_out)
    {
        return FALSE;
    }
    
    if (0 != *timer_id_ptr)
    {
        StopSlideTimer(timer_id_ptr);
    }
    
    *timer_id_ptr = MMK_CreateTimerCallback(time_out, func, param, FALSE);

    SLIDE_DEBUG_TRACE("StartSlideTimer SUCCESS timer_id = %d", *timer_id_ptr);
    
    return ret;
}

/*****************************************************************************/
// Description : StopSlideTimer
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StopSlideTimer(uint8 *timer_id_ptr)
{
    BOOLEAN      ret        = TRUE;

    if (PNULL == timer_id_ptr)
    {
        return FALSE;
    }

    if (0 != *timer_id_ptr)
    {
        SLIDE_DEBUG_TRACE("StopSlideTimer SUCCESS timer_id = %d", *timer_id_ptr);
        MMK_StopTimer(*timer_id_ptr);
        *timer_id_ptr = 0;
    }

    return ret;
}

/*****************************************************************************/
// Description : StartMoving
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartMoving(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr)
{
    BOOLEAN      ret        = FALSE;
    
    if (PNULL == move_ptr || PNULL == info_ptr)
    {
        return ret;
    }

    if (SLIDE_MOVE_NONE == move_ptr->move_direction)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("StartMoving, Enter is_moving = %d", move_ptr->is_moving);

    if (!move_ptr->is_moving)
    {
        //初始化buffer
        if (InitAllSlideLayerData(move_ptr, info_ptr))
        {
            uint32 i = 0;
            //隐藏其他blt layer,append slide layer
            HideAllBltLayer(move_ptr);
            
            for (i = 0; i < info_ptr->win_total_num; i++)
            {
                RunWinMsgEx(info_ptr->win_array[i], MSG_SLIDE_MOVE_BEGIN, PNULL);
            }
            if (0 != s_tp_down_win_id)
            {
                HideActiveCtrl(s_tp_down_win_id, TRUE, &move_ptr->backup_ctl_info);
            }
            
            //启动定时更新屏幕timer
            StartSlideTimer(&move_ptr->scroll_timer_id, MMISLIDE_SCROLL_TIME);
            
            ret = TRUE;
        }
    }

    SLIDE_DEBUG_TRACE("StartMoving Exit ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// Description : StopMoving
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN StopMoving(MMI_SLIDE_MOVE_T *move_ptr)
{
    BOOLEAN ret = FALSE;

    SLIDE_DEBUG_TRACE("StopMoving, Enter");
    
    if (PNULL != move_ptr)
    {
        MMI_SLIDE_INFO_T *slide_ptr   = s_cur_active_slide_info.cur_slide_info_ptr;

        StopSlideTimer(&move_ptr->scroll_timer_id);
        
        RecoverAllBltLayer(move_ptr);
        
        if (0 != s_tp_down_win_id)
        {
            HideActiveCtrl(s_tp_down_win_id, FALSE, &move_ptr->backup_ctl_info);
        }

        if (PNULL != slide_ptr)
        {
            uint32 i = 0;
            
            for (i = 0; i < slide_ptr->win_total_num; i++)
            {
                RunWinMsgEx(slide_ptr->win_array[i], MSG_SLIDE_MOVE_END, PNULL);
            }
        }
        
        ResetMoveParam(move_ptr);
        
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("StopMoving Exit ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// Description : 重设 slide move param
// Global resource dependence : 
// Author:Xiaoming.Ren
// Note: 
/*****************************************************************************/
LOCAL void ResetMoveParam(MMI_SLIDE_MOVE_T *move_ptr)
{
    if (PNULL != move_ptr)
    {
        SLIDE_DEBUG_TRACE("ResetMoveParam Enter");

        move_ptr->velocity        = 0;
        move_ptr->blt_layer_count = 0;
        move_ptr->is_moving       = FALSE;
        move_ptr->is_sleeping     = FALSE;
        
        move_ptr->next_index      = 0xff;
        move_ptr->pre_index       = 0xff;
        move_ptr->cur_index       = 0xff;
        move_ptr->sym_index       = 0xff;
        move_ptr->new_index       = 0xff;
        move_ptr->state           = MMK_TP_SLIDE_NONE;
        move_ptr->move_direction  = SLIDE_MOVE_NONE;        

        SCI_MEMSET(&move_ptr->pre_point, 0x00, sizeof(GUI_POINT_T));
        SCI_MEMSET(&move_ptr->start_point, 0x00, sizeof(GUI_POINT_T));
        SCI_MEMSET(move_ptr->append_array, 0x00, sizeof(move_ptr->append_array));

        SLIDE_DEBUG_TRACE("ResetMoveParam Exit");
    }
}


/*****************************************************************************/
//  Description : hook function for slide win tp down
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpDown(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E  result        = MMI_RESULT_FALSE;
    GUI_POINT_T   point         = {0};
    uint16        cur_win_index = 0;
    BOOLEAN       cur_win_is_ok = FALSE;

    MMI_SLIDE_MOVE_T *move_ptr  = PNULL;
    MMI_SLIDE_INFO_T *slide_ptr = PNULL;


    if (PNULL == active_slide_info_ptr)
    {
        return result;
    }

    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;

    if (PNULL == slide_ptr || !slide_ptr->is_valid)
    {
        return result;
    }

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (IsWinInSlideArray(slide_ptr, win_id, &cur_win_index))
    {
        cur_win_is_ok = TRUE;
    }
    else if (IsWinInSlideArray(slide_ptr, MMK_GetWinId(MMK_GetParentWinHandle(win_id)), &cur_win_index))
    {
        cur_win_is_ok = TRUE;
    }
    
    if (cur_win_is_ok)
    {
        if (PNULL != move_ptr && MMK_TP_SLIDE_NONE != move_ptr->state)
        {
            ExitSlideMove(active_slide_info_ptr, FALSE);
        }

        move_ptr = &s_move_info;
        
        move_ptr->start_point = point;
        move_ptr->pre_point   = point;
        ROUND_VALUE(move_ptr->pre_point.x);
        move_ptr->cur_index   = cur_win_index;

        s_tp_down_win_id     = win_id;
        
        active_slide_info_ptr->cur_move_info_ptr = move_ptr;
        
        move_ptr->state = MMK_TP_SLIDE_NONE;
        MMK_ResetVelocityItemByApp(&s_slide_velocity_tracker_t);
        MMK_AddVelocityItemByApp(&s_slide_velocity_tracker_t,point.x, point.y,MMI_TP_DOWN);
    }

    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win tp move
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpMove(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E           result        = MMI_RESULT_FALSE;
    SLIDE_MOVE_DIRECTION_E direction     = SLIDE_MOVE_INVALID;
    GUI_POINT_T            point         = {0};
    uint16                 dst_index     = 0;

    MMI_SLIDE_MOVE_T      *move_ptr      = PNULL;
    MMI_SLIDE_INFO_T      *slide_ptr     = PNULL;


    if (PNULL == active_slide_info_ptr)
    {
        return result;
    }

    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;

    if (PNULL == slide_ptr || PNULL == move_ptr)
    {
        return result;
    }

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    //MMK_GetLogicTPMsg(&tp_status, &point);

    if (0 == s_tp_down_win_id || win_id != s_tp_down_win_id)
    {
        result = MMI_RESULT_FALSE;
    }
    else if (move_ptr->is_moving)
    {
        if (move_ptr->is_sleeping)
        {
            if (MMK_TP_SLIDE_TP_SCROLL == move_ptr->state && IS_TP_MOVE_X_EXT(move_ptr->pre_point.x, point.x, MMISLIDE_TP_MOVE_DIS))
            {
                //唤醒timer
                StartSlideTimer(&move_ptr->scroll_timer_id, MMISLIDE_SCROLL_TIME);
                move_ptr->is_sleeping = FALSE;
            }
        }
        result = MMI_RESULT_TRUE;
    }
    else
    {
        if (slide_ptr->is_valid && MMISLIDE_NONE_SWITCH != slide_ptr->win_switch_type)
        {
            direction = GetMoveDirection(&move_ptr->start_point, &point, MMISLIDE_TP_MOVE_DIS);
            
            if (SLIDE_MOVE_LEFT == direction || SLIDE_MOVE_RIGHT == direction)
            {
                FindPreIndex(slide_ptr, move_ptr->cur_index, &move_ptr->pre_index);
                FindNextIndex(slide_ptr, move_ptr->cur_index, &move_ptr->next_index);
                
                dst_index = (SLIDE_MOVE_LEFT == direction) ? move_ptr->next_index : move_ptr->pre_index;

                // 增加速度点
                MMK_AddVelocityItemByApp(&s_slide_velocity_tracker_t,move_ptr->pre_point.x, move_ptr->pre_point.y,MMI_TP_MOVE);
                
                move_ptr->pre_point      = point;
                ROUND_VALUE(move_ptr->pre_point.x);
                move_ptr->move_direction = direction;
                move_ptr->sym_index      = move_ptr->cur_index;
                move_ptr->new_index      = dst_index;
                
                if (MMISLIDE_NORMAL_SWITCH == slide_ptr->win_switch_type)
                {
                    if (StartMoving(move_ptr, slide_ptr))
                    {
                        move_ptr->state       = MMK_TP_SLIDE_TP_SCROLL;
                        move_ptr->is_moving   = TRUE;
                    }
                    else
                    {
                        move_ptr->sym_index = move_ptr->new_index;
                        ExitSlideMove(active_slide_info_ptr, TRUE);
                    }
                }
                else
                {
                    //MMISLIDE_PROMPTLY_SWITCH
                    move_ptr->sym_index = move_ptr->new_index;
                    ExitSlideMove(active_slide_info_ptr, TRUE);
                }

                result = MMI_RESULT_TRUE;
            }
        }
    }

    if (MMI_RESULT_FALSE == result)
    {
        ExitSlideMove(active_slide_info_ptr, FALSE);
    }
    
    return result;
}
/*****************************************************************************/
//  Description : DoSlideBackAnim
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL void  DoSlideBackAnim(MMI_SLIDE_MOVE_T *move_ptr)
{

    GUI_POINT_T              point             = {0};
    BOOLEAN  is_move_left = FALSE;
    MMI_SLIDE_ACTIVE_INFO_T* active_info_ptr = &s_cur_active_slide_info;
    MMI_SLIDE_INFO_T        *slide_ptr         = PNULL;


    is_move_left = (SLIDE_MOVE_LEFT == move_ptr->move_direction) ? TRUE : FALSE;
     
    do
    {
        slide_ptr = active_info_ptr->cur_slide_info_ptr;
        move_ptr  = active_info_ptr->cur_move_info_ptr;
  #if 0      
        if (FLING_RUN_MIN_VELOCITY <= abs((int32)move_ptr->velocity)) 
        {
            point = move_ptr->pre_point;
            point.x = MMK_GetFlingOffset(point.x, move_ptr->velocity, SLIDE_TOUCH_FLING_TIME, &move_ptr->velocity);
        }
        else
#endif
        {
            point = move_ptr->pre_point;
            point.x += (is_move_left ? -RECOVER_STEP : RECOVER_STEP);
//            point.x = (is_move_left ? MAX(move_ptr->pre_point.x-RECOVER_STEP, move_ptr->start_point.x)  : MIN(move_ptr->pre_point.x+RECOVER_STEP, move_ptr->start_point.x));
            if ((MMK_TP_SLIDE_RECOVER == move_ptr->state) &&
                ((is_move_left && (point.x<move_ptr->start_point.x)) || (!is_move_left && (point.x>move_ptr->start_point.x))))
            {
                break;
            }
        }
    }while(UpdateScreen(move_ptr, slide_ptr, &point,TRUE));
    
    ExitSlideMove(active_info_ptr, TRUE);
}
/*****************************************************************************/
//  Description : hook function for slide win tp up
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTpUp(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param)
{
    MMI_RESULT_E       result          = MMI_RESULT_FALSE;
    MMI_SLIDE_MOVE_T  *move_ptr        = PNULL;
    MMI_SLIDE_INFO_T  *slide_ptr       = PNULL;
    GUI_POINT_T        point           = {0};
    uint16             lcd_width       = 0;
    MMI_TP_STATUS_E tp_status = MMI_TP_UP;
    
    if (PNULL == active_slide_info_ptr || PNULL == active_slide_info_ptr->cur_slide_info_ptr)
    {
        return result;
    }
    
    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;
    
    if (!slide_ptr->is_valid)
    {
        return result;
    }
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    MMK_GetLogicTPMsg(&tp_status, &point);
    
    if (PNULL != move_ptr)
    {
        if (move_ptr->is_moving)
        {
            if (MMK_TP_SLIDE_TP_SCROLL == move_ptr->state)
            {
                GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
                StopSlideTimer(&move_ptr->scroll_timer_id);
                // 增加速度点
                MMK_AddVelocityItemByApp(&s_slide_velocity_tracker_t,point.x, point.y,MMI_TP_UP);
                MMK_ComputeCurrentVelocityByApp(&s_slide_velocity_tracker_t,&move_ptr->velocity, (float *)PNULL, (float)FLING_MAX_VELOCITY, (float)0);
                
                //保证move_ptr->pre_point.x和layer_pos的一致性
                UpdateScreen(move_ptr, slide_ptr, &point,FALSE);
//                move_ptr->pre_point = point;
//                ROUND_VALUE(move_ptr->pre_point.x);
                
                if (FLING_MIN_VELOCITY <= abs((int32)move_ptr->velocity) && move_ptr->new_index != move_ptr->cur_index)  // 达到一定速度，需要滑动
                {
                    move_ptr->state = MMK_TP_SLIDE_FLING;
                    
                    DoSlideBackAnim(move_ptr);
                }
                else
                {
                    //转换为匀速运动
                    move_ptr->state = MMK_TP_SLIDE_RECOVER;
                    UpdateSlideDirection(move_ptr);
                    DoSlideBackAnim(move_ptr);
                }
                
                result = MMI_RESULT_TRUE;
            }
            else
            {
                ExitSlideMove(active_slide_info_ptr, TRUE);
            }
        }
        else
        {
            ExitSlideMove(active_slide_info_ptr, FALSE);
        }
        
    }
    
    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win timer
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinTimer(uint8 time_id, uint32 param)
{
    MMI_RESULT_E             result            = MMI_RESULT_FALSE;
    MMI_TP_STATUS_E          state             = MMI_TP_NONE;
    GUI_POINT_T              point             = {0};
    MMI_SLIDE_MOVE_T        *move_ptr          = PNULL;
    MMI_SLIDE_INFO_T        *slide_ptr         = PNULL;
    MMI_SLIDE_ACTIVE_INFO_T *active_info_ptr   = (MMI_SLIDE_ACTIVE_INFO_T *)param;
    uint16                   lcd_width         = 0;
    BOOLEAN                  is_move_left      = FALSE;


    if (PNULL == active_info_ptr)
    {
        return result;
    }
    
    slide_ptr = active_info_ptr->cur_slide_info_ptr;
    move_ptr  = active_info_ptr->cur_move_info_ptr;
    
    if (PNULL == move_ptr)
    {
        return result;
    }

    SLIDE_DEBUG_TRACE("HandleSlideWinTimer Enter, timer_id = %d, move_state = %d", time_id, move_ptr->state);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
    is_move_left = (SLIDE_MOVE_LEFT == move_ptr->move_direction) ? TRUE : FALSE;

    if (move_ptr->scroll_timer_id == time_id)
    {
        StopSlideTimer(&move_ptr->scroll_timer_id);
        
        if (MMK_TP_SLIDE_TP_SCROLL == move_ptr->state)
        {
            MMK_GetLogicTPMsg(&state, &point);
            
            MMK_AddVelocityItemByApp(&s_slide_velocity_tracker_t,point.x, point.y,MMI_TP_MOVE);

            if (UpdateScreen(move_ptr, slide_ptr, &point,FALSE))
            {
                StartSlideTimer(&move_ptr->scroll_timer_id, MMISLIDE_SCROLL_TIME);
            }
            else
            {
                move_ptr->is_sleeping = TRUE;
            }
        }
        
        result = MMI_RESULT_TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win lose focus
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinLoseFocus(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E       result     = MMI_RESULT_FALSE;
    MMI_SLIDE_MOVE_T  *move_ptr   = PNULL;
    MMI_SLIDE_INFO_T  *slide_ptr  = PNULL;
    MMI_SLIDE_HANDLE   cur_handle = 0;
    
    if (PNULL == active_slide_info_ptr || PNULL == active_slide_info_ptr->cur_slide_info_ptr)
    {
        return result;
    }
    
    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;

    if (slide_ptr->is_valid)
    {
        if (IsSlideLoseFocus(win_id, slide_ptr))
        {
            ExitSlideMove(active_slide_info_ptr, FALSE);
            
            s_cur_backup_handle = slide_ptr->handle;
            cur_handle          = slide_ptr->handle;
            MMIAPISLIDE_Deactive(&cur_handle);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win get focus
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinGetFocus(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param)
{
    MMI_RESULT_E       result     = MMI_RESULT_FALSE;
    MMI_SLIDE_INFO_T  *slide_ptr  = PNULL;
    
    
    if (PNULL == active_slide_info_ptr || PNULL == active_slide_info_ptr->cur_slide_info_ptr)
    {
        return result;
    }
    
    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;

    if (!slide_ptr->is_valid)
    {
        if (MMI_SLIDE_HANDLE_INVALID != s_cur_backup_handle)
        {
            if (MMISLIDE_Active(&s_cur_backup_handle))
            {
                s_cur_backup_handle = MMI_SLIDE_HANDLE_INVALID;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win lcd switch
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinLcdSwitch(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E       result     = MMI_RESULT_FALSE;
    MMI_SLIDE_INFO_T  *slide_ptr  = PNULL;
    
    if (PNULL == active_slide_info_ptr || PNULL == active_slide_info_ptr->cur_slide_info_ptr)
    {
        return result;
    }

    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;

    if (win_id == slide_ptr->cur_win_id)
    {
        ExitSlideMove(active_slide_info_ptr, FALSE);
        if (slide_ptr->is_valid)
        {
            ReleaseAllSlideLayer(slide_ptr);
            CreateAllSlideLayer(slide_ptr);
            if (SLIDE_SYM_WORK_ALWAYS == slide_ptr->sym_dis_info.work_type || SLIDE_SYM_WORK_AUTO_HIDE == slide_ptr->sym_dis_info.work_type)
            {
                DisplayWinSymbol(slide_ptr);
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win close window
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleSlideWinCloseWindow(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, DPARAM param)
{
    MMI_RESULT_E       result     = MMI_RESULT_FALSE;
    MMI_SLIDE_HANDLE   cur_handle = 0;
    MMI_SLIDE_MOVE_T  *move_ptr   = PNULL;
    MMI_SLIDE_INFO_T  *slide_ptr  = PNULL;
    
    
    if (PNULL == active_slide_info_ptr || PNULL == active_slide_info_ptr->cur_slide_info_ptr)
    {
        return result;
    }
    
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;
    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;

    ExitSlideMove(active_slide_info_ptr, FALSE);

    if (PNULL != slide_ptr)
    {
        cur_handle = slide_ptr->handle;
        MMIAPISLIDE_Release(&cur_handle);

        active_slide_info_ptr->cur_move_info_ptr  = PNULL;
        active_slide_info_ptr->cur_slide_info_ptr = PNULL;
    }

    return result;
}

/*****************************************************************************/
//  Description : hook function for slide win exit slide move
//  Global resource dependence : 
//  Author: Xiaoming.Ren
//  Note:
/*****************************************************************************/
LOCAL  void ExitSlideMove(MMI_SLIDE_ACTIVE_INFO_T *active_slide_info_ptr, BOOLEAN is_update_screen)
{
    MMI_SLIDE_MOVE_T  *move_ptr   = PNULL;
    MMI_SLIDE_INFO_T  *slide_ptr  = PNULL;
    uint16             dst_index  = 0;
    
    if (PNULL == active_slide_info_ptr)
    {
        return;
    }
    
    move_ptr  = active_slide_info_ptr->cur_move_info_ptr;
    slide_ptr = active_slide_info_ptr->cur_slide_info_ptr;

    if (PNULL != move_ptr)
    {
        SLIDE_DEBUG_TRACE("ExitSlideMove, dst_index = %d, is_update_screen = %d", move_ptr->sym_index, is_update_screen);
        
        dst_index = move_ptr->sym_index;

        ResetAllSlideLayer(slide_ptr);
        StopMoving(move_ptr);

        if (is_update_screen)
        {
            if (PNULL != slide_ptr && dst_index < slide_ptr->win_total_num)
            {
                SwitchToWin(slide_ptr, slide_ptr->win_array[dst_index]);

                if (SLIDE_SYM_WORK_ALWAYS != slide_ptr->sym_dis_info.work_type)
                {
                    BOOLEAN is_backup_normal = FALSE;
                    
                    if (SLIDE_SYM_WORK_NORMAL == slide_ptr->sym_dis_info.work_type)
                    {
                        is_backup_normal = TRUE;
                        slide_ptr->sym_dis_info.work_type = SLIDE_SYM_WORK_AUTO_HIDE;
                    }
                    
                    DisplayWinSymbol(slide_ptr);
                    
                    if (is_backup_normal)
                    {
                        is_backup_normal = FALSE;
                        slide_ptr->sym_dis_info.work_type = SLIDE_SYM_WORK_NORMAL;
                    }
                }
            }
        }

        MMK_ResetVelocityItem();

        active_slide_info_ptr->cur_move_info_ptr = PNULL;
        s_tp_down_win_id = 0;
    }
}

/*****************************************************************************/
// 	Description : 初始化layer数据
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAllSlideLayerData(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr)
{
    BOOLEAN           ret        = FALSE;
    BOOLEAN           is_remove  = FALSE;

    if (PNULL == move_ptr || PNULL == info_ptr)
    {
        return ret;
    }
    
    if (info_ptr->type != MMI_SLIDE_MANY_PAINT_ONE_WIN)
    {
        if (UILAYER_IsBltLayer(&s_slide_sym_layer_info))
        {
            is_remove = TRUE;
            ClearSymbolDisInfo(info_ptr);
        }
        
        ResetAllSlideLayer(info_ptr);
        
        //初始化各个buffer的数据
        ret = CopyWinbufferToLayerEx(info_ptr, info_ptr->win_array[move_ptr->cur_index], &s_slide_old_layer_info, &info_ptr->clip_info.main_rect);
        
        if (ret)
        {
            if (!UILAYER_IsEmptyLayerHandle(&s_slide_bg_layer_info))
            {
                ret &= CopyWinbufferToLayerEx(info_ptr, info_ptr->win_array[move_ptr->cur_index], &s_slide_bg_layer_info, &info_ptr->clip_info.top_bg_rect);
            }
            
            if (!UILAYER_IsEmptyLayerHandle(&s_slide_bg_layer1_info))
            {
                ret &= CopyWinbufferToLayerEx(info_ptr, info_ptr->win_array[move_ptr->cur_index], &s_slide_bg_layer1_info, &info_ptr->clip_info.bottom_bg_rect);
            }
        }

        if (ret)
        {
            if (move_ptr->new_index != move_ptr->cur_index)
            {
                ret = CopyWinbufferToLayerEx(info_ptr, info_ptr->win_array[move_ptr->new_index], &s_slide_new_layer_info, &info_ptr->clip_info.main_rect);
            }
            else
            {
                DisplayCommonBg(info_ptr->cur_win_id, &s_slide_new_layer_info, &info_ptr->clip_info.main_rect);
            }
        }
    }

    //初始化layer point
    UpdateAllLayerPos_X(move_ptr, move_ptr->pre_point.x - move_ptr->start_point.x,FALSE);
    
    DrawCurWinSym(&s_slide_sym_layer_info, info_ptr->win_total_num, move_ptr->cur_index);
    
    if (is_remove)
    {
        UILAYER_APPEND_BLT_T blt = {0};
        blt.layer_level = UILAYER_LEVEL_NORMAL;
        blt.lcd_dev_info = s_slide_sym_layer_info;
        UILAYER_AppendBltLayer(&blt);
    }
    
    return ret;
}

/*****************************************************************************/
// 	Description : 创建所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr)
{
    BOOLEAN    result      = FALSE;
    GUI_RECT_T lcd_rect    = {0};
    GUI_RECT_T move_rect   = {0};
    GUI_RECT_T layer_rect  = {0};
    
    if (PNULL == info_ptr)
    {
        return result;
    }
    
    SLIDE_DEBUG_TRACE("CreateAllSlideLayer Enter, handle = %d, switch type = %d", info_ptr->handle, info_ptr->win_switch_type);

    if (MMISLIDE_NONE_SWITCH == info_ptr->win_switch_type)
    {
        return TRUE;
    }
    
    move_rect = MMK_IsWindowLandscape(info_ptr->cur_win_id) ? info_ptr->slide_both_rect.h_rect : info_ptr->slide_both_rect.v_rect;
    
    SLIDE_DEBUG_TRACE("CreateAllSlideLayer , move_rect = [%d, %d, %d, %d]", move_rect.left, move_rect.top, move_rect.right, move_rect.bottom);
    
    if (!GUI_IsRectEmpty(move_rect))
    {
        lcd_rect = MMITHEME_GetFullScreenRect();
        
        SLIDE_DEBUG_TRACE("CreateAllSlideLayer , lcd_rect = [%d, %d, %d, %d]", lcd_rect.left, lcd_rect.top, lcd_rect.right, lcd_rect.bottom);

        if (move_rect.left == lcd_rect.left && move_rect.right == lcd_rect.right)
        {
            if (MMISLIDE_NORMAL_SWITCH == info_ptr->win_switch_type)
            {
                info_ptr->clip_info.main_rect = move_rect;
                result =  CreateSlideLayer(info_ptr->default_win_id, &s_slide_old_layer_info, move_rect);
                result &= CreateSlideLayer(info_ptr->default_win_id, &s_slide_new_layer_info, move_rect);
                
                if (!GUI_EqualRect(lcd_rect, move_rect))
                {
                    if (GUI_RectIsCovered(lcd_rect, move_rect))
                    {
                        if (move_rect.bottom < lcd_rect.bottom && move_rect.top > lcd_rect.top)
                        {
                            layer_rect = lcd_rect;
                            layer_rect.bottom = move_rect.top;
                            info_ptr->clip_info.top_bg_rect = layer_rect;
                            result &= CreateSlideLayer(info_ptr->default_win_id, &s_slide_bg_layer_info, layer_rect);
                            
                            layer_rect = lcd_rect;
                            layer_rect.top = move_rect.bottom;
                            info_ptr->clip_info.bottom_bg_rect = layer_rect;
                            result &= CreateSlideLayer(info_ptr->default_win_id, &s_slide_bg_layer1_info, layer_rect);
                        }
                        else
                        {
                            layer_rect = lcd_rect;
                            if (move_rect.top == lcd_rect.top && move_rect.bottom < lcd_rect.bottom)
                            {
                                //截取下方区域
                                layer_rect.top = move_rect.bottom;
                            }
                            else if (move_rect.bottom == lcd_rect.bottom && move_rect.top > lcd_rect.top)
                            {
                                //截取上方区域
                                layer_rect.bottom = move_rect.top;
                            }
                            info_ptr->clip_info.top_bg_rect = layer_rect;
                            result &= CreateSlideLayer(info_ptr->default_win_id, &s_slide_bg_layer_info, layer_rect);
                        }
                    }
                }

                if (!result)
                {
                    SLIDE_DEBUG_TRACE("error :normal switch type creat layer fail");
                    ReleaseAllSlideLayer(info_ptr);
                    result = TRUE;
                }
            }
            
            if (SLIDE_SYM_WORK_NONE != info_ptr->sym_dis_info.work_type)
            {
                layer_rect = lcd_rect;
                if (info_ptr->is_sym_on_top)
                {
                    if (0 != info_ptr->symbol_point.y)
                    {
                        layer_rect.top += info_ptr->symbol_point.y;
                    }
                    else
                    {
                        layer_rect.top += WIN_SYM_LAYER_HEIGHT;
                    }    
                    layer_rect.bottom = layer_rect.top + WIN_SYM_LAYER_HEIGHT;
                }
                else
                {
                    if (0 != info_ptr->symbol_point.y)
                    {
                        layer_rect.bottom -= info_ptr->symbol_point.y;
                    }
                    else
                    {
                        layer_rect.bottom -= WIN_SYM_LAYER_HEIGHT;
                    }    

                    layer_rect.top    = layer_rect.bottom - WIN_SYM_LAYER_HEIGHT;
                }
                
                result &= CreateSlideLayer(info_ptr->default_win_id, &s_slide_sym_layer_info, layer_rect);
            }
        }
        else
        {
            result = TRUE;
            SLIDE_DEBUG_TRACE("CreateAllSlideLayer , move rect width != lcd rect width");
        }
    }

    if (!result)
    {
        ReleaseAllSlideLayer(info_ptr);
    }
    
    SLIDE_DEBUG_TRACE("CreateAllSlideLayer Exit, result = %d", result);

    return result;
}

/*****************************************************************************/
// 	Description : 销毁所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ReleaseAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr)
{
    if (PNULL != info_ptr)
    {
        SCI_MEMSET(&info_ptr->clip_info.main_rect,      0x00, sizeof(GUI_RECT_T));
        SCI_MEMSET(&info_ptr->clip_info.top_bg_rect,    0x00, sizeof(GUI_RECT_T));
        SCI_MEMSET(&info_ptr->clip_info.bottom_bg_rect, 0x00, sizeof(GUI_RECT_T));
    }
    
    ReleaseSlideLayer(&s_slide_old_layer_info);
    ReleaseSlideLayer(&s_slide_new_layer_info);
    ReleaseSlideLayer(&s_slide_bg_layer_info);
    ReleaseSlideLayer(&s_slide_bg_layer1_info);
    ReleaseSlideLayer(&s_slide_sym_layer_info);
}

/*****************************************************************************/
// 	Description : 重置所有layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ResetAllSlideLayer(MMI_SLIDE_INFO_T *info_ptr)
{
    GUI_POINT_T layer_pos = {0};

    if (PNULL == info_ptr)
    {
        return;
    }

    UILAYER_Clear(&s_slide_old_layer_info);
    UILAYER_Clear(&s_slide_new_layer_info);
    UILAYER_Clear(&s_slide_bg_layer_info);
    UILAYER_Clear(&s_slide_bg_layer1_info);
    if (SLIDE_SYM_WORK_ALWAYS != info_ptr->sym_dis_info.work_type)
    {
        ClearSymbolDisInfo(info_ptr);
    }

    UILAYER_GetLayerPosition(&s_slide_old_layer_info, &layer_pos.x, &layer_pos.y);
    UILAYER_SetLayerPosition(&s_slide_old_layer_info, 0, layer_pos.y);
    UILAYER_SetLayerPosition(&s_slide_new_layer_info, 0, layer_pos.y);
}

/*****************************************************************************/
// 	Description : 创建layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateSlideLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T init_rect)
{
    BOOLEAN          result      = FALSE;
    UILAYER_CREATE_T create_info = {0};

    if (PNULL == layer_ptr)
    {
        return result;
    }

    SLIDE_DEBUG_TRACE("CreateSlideLayer Enter, Block id = %d", layer_ptr->block_id);
    SLIDE_DEBUG_TRACE("CreateSlideLayer Enter, init_rect = [%d, %d, %d, %d]", init_rect.left, init_rect.top, init_rect.right, init_rect.bottom);

    if (UILAYER_IsEmptyLayerHandle(layer_ptr) && !GUI_IsInvalidRect(init_rect))
    {
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = init_rect.left;
        create_info.offset_y        = init_rect.top;
        create_info.width           = init_rect.right - init_rect.left + 1;
        create_info.height          = init_rect.bottom - init_rect.top + 1;
        create_info.is_bg_layer     = (BOOLEAN) (layer_ptr != &s_slide_sym_layer_info);
        create_info.format          = UILAYER_MEM_FORMAT_IMMUTABLE;
        create_info.is_static_layer = TRUE;

        if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, layer_ptr))
        {
            if (UILAYER_IsLayerActive(layer_ptr)) // 判断多层是否有效
            {
                // 设置color key
                UILAYER_SetLayerColorKey(layer_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
                // 使用color清除层
                UILAYER_Clear(layer_ptr);
                
                UILAYER_SetLayerPosition(layer_ptr, init_rect.left, init_rect.top);
                
                result = TRUE;
            }
        }
    }

    SLIDE_DEBUG_TRACE("CreateSlideLayer Exit, Block id = %d, ret = %d", layer_ptr->block_id, result);

    return result;
}

/*****************************************************************************/
// 	Description : 销毁layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ReleaseSlideLayer(GUI_LCD_DEV_INFO *layer_ptr)
{
    if (PNULL == layer_ptr)
    {
        return;
    }
    
    SLIDE_DEBUG_TRACE("ReleaseSlideLayer Enter, Block id = %d", layer_ptr->block_id);
    
    if (!UILAYER_IsEmptyLayerHandle(layer_ptr))
    {
        UILAYER_ReleaseLayer(layer_ptr);
        layer_ptr->block_id = UILAYER_NULL_HANDLE;
    }
    
    SLIDE_DEBUG_TRACE("ReleaseSlideLayer Exit, Block id = %d", layer_ptr->block_id);
}

/*****************************************************************************/
// 	Description : Copy lcd buffer到layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyLCDbufferToLayer(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *clip_rect_ptr)
{
    BOOLEAN                   ret            = FALSE;
    uint16                    lcd_height     = 0;
    uint16                    lcd_width      = 0;
    UILAYER_COPYBUF_PARAM_T   copy_param     = {0};
    UILAYER_COPYBUF_T         copy_buf       = {0};
    GUI_COLOR_T              *lcd_buffer_ptr = PNULL;
    GUI_POINT_T               layer_point = {0};
    GUI_RECT_T                clip_rect = {0};

    if (PNULL == layer_ptr || UILAYER_IsEmptyLayerHandle(layer_ptr))
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("CopyLCDbufferToLayer Enter, layer block id = %d", layer_ptr->block_id);
    
    if (PNULL != clip_rect_ptr)
    {
        SLIDE_DEBUG_TRACE("CopyLCDbufferToLayer , clip_rect = [%d, %d, %d, %d]", clip_rect_ptr->left, clip_rect_ptr->top, clip_rect_ptr->right, clip_rect_ptr->bottom);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        
        lcd_buffer_ptr     = GUILCD_GetMainLcdBufPtr();
        copy_param.rect    = MMITHEME_GetFullScreenRect();

        UILAYER_GetLayerPosition(layer_ptr, &layer_point.x, &layer_point.y);
//        copy_param.clip_rect_ptr = clip_rect_ptr;
        clip_rect.left = clip_rect_ptr->left + layer_point.x;
        clip_rect.top = clip_rect_ptr->top;
        clip_rect.right = clip_rect_ptr->right + layer_point.x;
        clip_rect.bottom = clip_rect_ptr->bottom;
        copy_param.rect.left += layer_point.x;
        copy_param.rect.right += layer_point.x;        
        copy_param.clip_rect_ptr = &clip_rect;

        copy_buf.buf_ptr   = (uint8*)lcd_buffer_ptr;
        copy_buf.data_type = DATA_TYPE_RGB565;
        copy_buf.width     = lcd_width;
        copy_buf.height    = lcd_height;
        
        UILAYER_Clear(layer_ptr);
        UILAYER_CopyBufferToLayer(layer_ptr, &copy_param, &copy_buf);
        
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("CopyLCDbufferToLayer Exit, ret = %d", ret);
    
    return ret;
}

/*****************************************************************************/
// 	Description : Copy窗口buffer到layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyWinbufferToLayerEx(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T dst_win_id, GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *clip_rect_ptr)
{
    BOOLEAN           ret                = FALSE;
    uint16            dst_win_index      = 0;
    MMI_WIN_ID_T      focus_win_id       = 0;
    uint32            src_item_index     = 0;
    GUI_RECT_T        lcd_rect           = {0};
    GUI_LCD_DEV_INFO  main_lcd_dev       = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    UILAYER_APPEND_BLT_T blt_array[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32               blt_layer_count                    = 0;
    UILAYER_APPEND_BLT_T append_array_temp[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32               array_size                                 = 0;

    if (PNULL == info_ptr || UILAYER_IsEmptyLayerHandle(layer_ptr))
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("CopyWinbufferToLayerEx Enter, dst_win_id = %d, layer block id = %d", dst_win_id, layer_ptr->block_id);

    if (IsWinInSlideArray(info_ptr, dst_win_id, &dst_win_index))
    {
        //备份layer
        blt_layer_count = UILAYER_GetBltLayer(blt_array, UILAYER_TOTAL_BLT_COUNT);
        
        if (0 != info_ptr->parent_win_tab_id)
        {
            focus_win_id = MMK_GetFocusChildWinId();    
        }
        else
        {
            focus_win_id = MMK_GetFocusWinId();
        }
        
        //备份
        if (dst_win_id != focus_win_id)
        {
            //仅仅保留主层
            append_array_temp[array_size].lcd_dev_info = main_lcd_dev;
            append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
            UILAYER_SetBltLayer(append_array_temp,array_size + 1);

            GrabWindowFocus(dst_win_id, UPDAT_TYPE_NONE);
            
            if (0 != info_ptr->parent_win_tab_id)
            {
                src_item_index = GUITAB_GetCurSel(info_ptr->parent_win_tab_id);
                GUITAB_SetCurSel(info_ptr->parent_win_tab_id, dst_win_index);
            }
        }
        
        lcd_rect   = MMITHEME_GetFullScreenRect();
        
        //拷贝目标屏幕
        UILAYER_Clear(layer_ptr);
        if (PNULL != clip_rect_ptr)
        {
            if (dst_win_id != focus_win_id)
            {
                MMK_SendMsg(dst_win_id, MSG_FULL_PAINT, PNULL);
            }
            UILAYER_InvalidateRect(GUI_MAIN_LCD_ID, &lcd_rect);
            
            ret = CopyLCDbufferToLayer(layer_ptr, clip_rect_ptr);
        }
        
        //恢复
        if (dst_win_id != focus_win_id)
        {
            GrabWindowFocus(focus_win_id, UPDAT_TYPE_NONE);
            
            if (0 != info_ptr->parent_win_tab_id)
            {
                GUITAB_SetCurSel(info_ptr->parent_win_tab_id, src_item_index);
            }
        }

         //恢复layer
        UILAYER_SetBltLayer(blt_array, blt_layer_count);
    }

    SLIDE_DEBUG_TRACE("CopyWinbufferToLayerEx Exit, ret = %d", ret);
    
    return ret;
    
}

/*****************************************************************************/
// 	Description : RunWinMsgEx
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 消息需要该窗口以及它的父窗口或者焦点子窗口处理
/*****************************************************************************/
LOCAL BOOLEAN RunWinMsgEx(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    BOOLEAN ret = TRUE;

    MMK_RunWinProc(win_id, msg_id, param);
    
    if (MMK_IsChildWin(win_id))
    {
        MMK_RunWinProc(MMK_GetParentWinHandle(win_id), msg_id, param);
    }
    else if (0 != MMK_GetFocusChildWinHandle(win_id))
    {
        MMK_RunWinProc(MMK_GetFocusChildWinHandle(win_id), msg_id, param);
    }

    return ret;
}

/*****************************************************************************/
//  Description : Get slide info pointer from slide handle
//  Global resource dependence :
//  Author:xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL MMI_SLIDE_INFO_T* GetSlideInfoPtr(MMI_SLIDE_HANDLE slide_handle)
{
    uint16            index     = 0;
    MMI_SLIDE_INFO_T* info_ptr  = PNULL;

    if (CheckSlideHandleIsValid(&slide_handle, &index))
    {
        info_ptr = &s_slide_info_array[index];
    }

    return info_ptr;
}

/*****************************************************************************/
// 	Description : clean
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void ClearSymbolDisInfo(MMI_SLIDE_INFO_T *info_ptr)
{
    if (PNULL == info_ptr)
    {
        return;
    }


    if (0 != info_ptr->sym_remove_timer_id)
    {
        MMK_StopTimer(info_ptr->sym_remove_timer_id);
        info_ptr->sym_remove_timer_id = 0;
    }

    UILAYER_RemoveBltLayer(&s_slide_sym_layer_info);

    return;
}

/*****************************************************************************/
// 	Description : remove
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 显示后隐藏的timer回调
/*****************************************************************************/
LOCAL void RemoveSymbolCB(uint8 timer_id, uint32 param)
{
    MMI_SLIDE_INFO_T *info_ptr = GetSlideInfoPtr(param);

    if (PNULL == info_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != info_ptr->sym_remove_timer_id)
    {
        return;
    }

    StartSlideTimerEx(&info_ptr->sym_remove_timer_id, HIDE_OUT_TICK, OutSymboCB, (uint32)info_ptr->handle);

    return;
}

/*****************************************************************************/
// 	Description : out
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 渐隐效果的timer回调
/*****************************************************************************/
LOCAL void OutSymboCB(uint8 timer_id, uint32 param)
{
    MMI_SLIDE_INFO_T *info_ptr = GetSlideInfoPtr(param);

    if (PNULL == info_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != info_ptr->sym_remove_timer_id)
    {
        return;
    }

    info_ptr->out_state -= UILAYER_ALPHA_MAX / 10;

    UILAYER_WeakLayerAlpha(&s_slide_sym_layer_info, info_ptr->out_state);
    GUILCD_Invalidate(s_slide_sym_layer_info.lcd_id, 0);

    if (info_ptr->out_state > UILAYER_ALPHA_MAX / 10)
    {
        StartSlideTimerEx(&info_ptr->sym_remove_timer_id, HIDE_OUT_TICK, OutSymboCB, (uint32)info_ptr->handle);
    }
    else
    {
        ClearSymbolDisInfo(info_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : 设置窗口数组 的hook 函数
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlideHookFunc(MMI_WIN_ID_T *win_arr_ptr, uint16 arr_size, BOOLEAN is_true)
{
    BOOLEAN result       = FALSE;
    PROCESSMSG_FUNC func = (PROCESSMSG_FUNC)PNULL;
    uint32 i             = 0;
    uint16 child_win_num = 0;

    if (PNULL == win_arr_ptr)
    {
        return result;
    }

    if (is_true)
    {
        func = (PROCESSMSG_FUNC)HandleSlideSystermMsg;
    }

    for (i = 0; i < arr_size; i++)
    {
        if (0 != win_arr_ptr[i])
        {
            MMI_WIN_ID_T child_win_arr[CHILD_WIN_MAX_COUNT] = {0};
            
            child_win_num = MMK_GetAllChildWinHandle(win_arr_ptr[i], child_win_arr, CHILD_WIN_MAX_COUNT);
            if (0 < child_win_num)
            {
                child_win_num = MIN(child_win_num, CHILD_WIN_MAX_COUNT);
                SetSlideHookFunc(child_win_arr, child_win_num, is_true);
            }

            MMK_SetWinHookFunc(win_arr_ptr[i], func);
        }
    }

    result = TRUE;

    return result;
}

/*****************************************************************************/
// 	Description : 窗口是否在某个滑动数组中
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWinInSlideArray(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T win_id, uint16 *index_ptr)
{
    BOOLEAN result  = FALSE;

    if (PNULL == info_ptr)
    {
        return result;
    }

    result = FindWinIndexByWinId(info_ptr->win_array, info_ptr->win_total_num, win_id, index_ptr);

    SLIDE_DEBUG_TRACE("IsWinInSlideArray Exit, win_id = %d, ret = %d", win_id, result);

    return result;
}

/*****************************************************************************/
// 	Description : 窗口是否在所有滑动数组中
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWinInAllSlideArray(MMI_WIN_ID_T win_id, uint16 *arr_index_ptr, uint16 *win_index_ptr)
{
    BOOLEAN ret       = FALSE;
    uint16  arr_index = 0;
    uint16  win_index = 0;

    if (s_valis_handle_top_index < MMISLIDE_TOTAL_COUNT && 0 != win_id)
    {
        for (arr_index = 0; arr_index <= s_valis_handle_top_index; arr_index++)
        {
            if (IsWinInSlideArray(&s_slide_info_array[arr_index], win_id, &win_index))
            {
                if (PNULL != arr_index_ptr)
                {
                    *arr_index_ptr = arr_index;
                }

                if (PNULL != win_index_ptr)
                {
                    *win_index_ptr = win_index;
                }

                ret = TRUE;
                break;
            }
        }
    }

    return ret;
}

/*****************************************************************************/
// 	Description : IsSlideLoseFocus
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSlideLoseFocus(MMI_WIN_ID_T win_id, MMI_SLIDE_INFO_T *info_ptr)
{
    BOOLEAN      ret          = FALSE;
    MMI_WIN_ID_T focus_win_id = 0;
    
    if (PNULL == info_ptr)
    {
        return ret;
    }
    
    focus_win_id = MMK_GetFocusWinId();
    
    if (MMI_SLIDE_ALL_CHILD_WIN == info_ptr->type)
    {
        if (MMK_GetParentWinHandle(win_id) != MMK_ConvertIdToHandle(focus_win_id))
        {
            ret = TRUE;
        }
    }
    else if (MMI_SLIDE_ALL_NO_CHILD_WIN == info_ptr->type)
    {
        if (!IsWinInSlideArray(info_ptr, focus_win_id, PNULL))
        {
            ret = TRUE;
        }
    }
    else if (MMI_SLIDE_MANY_PAINT_ONE_WIN == info_ptr->type)
    {
        if (win_id != focus_win_id)
        {
            ret = TRUE;
        }
    }
    
    return ret;
}

/*****************************************************************************/
// 	Description : 查找某窗口在数组中的位置
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindWinIndexByWinId(MMI_WIN_ID_T *win_arr_ptr, uint16 arr_size, MMI_WIN_ID_T win_id, uint16 *dst_index_ptr)
{
    BOOLEAN ret = FALSE;
    uint32  i   = 0;

    if (PNULL == win_arr_ptr)
    {
        return ret;
    }

    arr_size = MIN(arr_size, MMISLIDE_WIN_TOTAL_COUNT);

    for (i = 0; i < arr_size; i++)
    {
        if (win_id == win_arr_ptr[i])
        {
            if (PNULL != dst_index_ptr)
            {
                *dst_index_ptr = i;
            }
            
            ret = TRUE;
            break;
        }
    }

    if (!ret)
    {
        SLIDE_DEBUG_TRACE("FindWinIndexByWinId FAIL, win_id = %d", win_id);
    }
    else
    {
        SLIDE_DEBUG_TRACE("FindWinIndexByWinId OK, win_id %d ------ index = %d", win_id, i);
    }

    return ret;
}

/*****************************************************************************/
// 	Description : 查找前一index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindPreIndex(MMI_SLIDE_INFO_T *info_ptr, uint16 src_index, uint16 *dst_index_ptr)
{
    BOOLEAN result             = FALSE;
    uint32  i                  = 0;
    int16   max_index          = 0;

    if (PNULL == dst_index_ptr || PNULL == info_ptr)
    {
        return result;
    }
    
    if (src_index < info_ptr->win_total_num)
    {
        max_index = src_index - 1;
        
        if (max_index < 0)
        {
            //越界
            if (info_ptr->is_loop_move)
            {
                for (i = info_ptr->win_total_num - 1; i >= src_index; i--)
                {
                    if (0 != info_ptr->win_array[i])
                    {
                        *dst_index_ptr = (uint16)i;
                        result = TRUE;
                        break;
                    }
                }
            }
            else
            {
                *dst_index_ptr = src_index;
                result = TRUE;
            }
        }
        else
        {
            for (; max_index >= 0; max_index--)
            {
                if (0 != info_ptr->win_array[max_index])
                {
                    *dst_index_ptr = (uint16)max_index;
                    result = TRUE;
                    break;
                }
            }
            
            if (info_ptr->is_loop_move && FALSE == result)
            {
                for (i = info_ptr->win_total_num - 1; i >= src_index; i--)
                {
                    if (0 != info_ptr->win_array[i])
                    {
                        *dst_index_ptr = (uint16)i;
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }


    if (FALSE == result)
    {
        SLIDE_DEBUG_TRACE("FindPreIndex FAIL, src_index = %d", src_index);
    }
    else
    {
        SLIDE_DEBUG_TRACE("FindPreIndex OK, src_index = %d, dst_index = %d", src_index, *dst_index_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : 查找后一index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindNextIndex(MMI_SLIDE_INFO_T *info_ptr, uint16 src_index, uint16 *dst_index_ptr)
{
    BOOLEAN result             = FALSE;
    uint32  i                  = 0;
    int16   max_index          = 0;

    if (PNULL == dst_index_ptr || PNULL == info_ptr)
    {
        return result;
    }

    if (src_index < info_ptr->win_total_num)
    {
        max_index = src_index + 1;
        
        if (max_index >= info_ptr->win_total_num)
        {
            //越界
            if (info_ptr->is_loop_move)
            {
                for (i = 0; i <= src_index; i++)
                {
                    if (0 != info_ptr->win_array[i])
                    {
                        *dst_index_ptr = (uint16)i;
                        result = TRUE;
                        break;
                    }
                }
            }
            else
            {
                *dst_index_ptr = src_index;
                result = TRUE;
            }
        }
        else
        {
            for (i = max_index; i < info_ptr->win_total_num; i++)
            {
                if (0 != info_ptr->win_array[i])
                {
                    *dst_index_ptr = (uint16)i;
                    result = TRUE;
                    break;
                }
            }
            
            if (info_ptr->is_loop_move && FALSE == result)
            {
                for (i = 0; i <= src_index; i++)
                {
                    if (0 != info_ptr->win_array[i])
                    {
                        *dst_index_ptr = (uint16)i;
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }
    
    if (FALSE == result)
    {
        SLIDE_DEBUG_TRACE("FindNextIndex FAIL, src_index = %d", src_index);
    }
    else
    {
        SLIDE_DEBUG_TRACE("FindNextIndex OK, src_index = %d, dst_index = %d", src_index, *dst_index_ptr);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 查找前一窗口handle
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindPreWinId(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T src_win_id, MMI_WIN_ID_T *dst_win_id_ptr)
{
    BOOLEAN result        = FALSE;
    uint16  cur_index     = 0xff;
    uint16  dst_index     = 0xff;

    if (PNULL == dst_win_id_ptr || PNULL == info_ptr)
    {
        return result;
    }

    if (IsWinInSlideArray(info_ptr, src_win_id, &cur_index))
    {
        if (FindPreIndex(info_ptr, cur_index, &dst_index))
        {
            *dst_win_id_ptr = info_ptr->win_array[dst_index];
            result = TRUE;
        }
    }

    if (FALSE == result)
    {
        SLIDE_DEBUG_TRACE("FindPreWinId FAIL, src_win_id = %d", src_win_id);
    }
    else
    {
        SLIDE_DEBUG_TRACE("FindPreWinId OK, src_win_id = %d, dst_win_id = %d", src_win_id, *dst_win_id_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : 查找后一窗口handle
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindNextWinId(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T src_win_id, MMI_WIN_ID_T *dst_win_id_ptr)
{
    BOOLEAN result        = FALSE;
    uint16  cur_index     = 0xff;
    uint16  dst_index     = 0xff;

    if (PNULL == dst_win_id_ptr)
    {
        return result;
    }

    if (IsWinInSlideArray(info_ptr, src_win_id, &cur_index))
    {
        if (FindNextIndex(info_ptr, cur_index, &dst_index))
        {
            *dst_win_id_ptr = info_ptr->win_array[dst_index];
            result = TRUE;
        }
    }

    if (FALSE == result)
    {
        SLIDE_DEBUG_TRACE("FindNextWinId FAIL, src_win_id = %d", src_win_id);
    }
    else
    {
        SLIDE_DEBUG_TRACE("FindNextWinId OK, src_win_id = %d, dst_win_id = %d", src_win_id, *dst_win_id_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : 获取移动的方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL SLIDE_MOVE_DIRECTION_E GetMoveDirection(GUI_POINT_T *begin_ptr, GUI_POINT_T *end_ptr, int32 move_max_pixel)
{
    SLIDE_MOVE_DIRECTION_E direction       = SLIDE_MOVE_INVALID;
    SLIDE_MOVE_DIRECTION_E direction_x     = SLIDE_MOVE_INVALID;
    SLIDE_MOVE_DIRECTION_E direction_y     = SLIDE_MOVE_INVALID;
    int16                  move_x_offset   = 0;
    int16                  move_y_offset   = 0;
    
    if (PNULL == begin_ptr || PNULL == end_ptr)
    {
        return direction;
    }
    
    if (IS_TP_MOVE_X_EXT(begin_ptr->x, end_ptr->x, move_max_pixel) || IS_TP_MOVE_X_EXT(begin_ptr->y, end_ptr->y, move_max_pixel))
    {
        move_x_offset = end_ptr->x - begin_ptr->x;
        move_y_offset = end_ptr->y - begin_ptr->y;
        
        direction_x = (move_x_offset < 0) ? SLIDE_MOVE_LEFT : SLIDE_MOVE_RIGHT;
        direction_y = (move_y_offset < 0) ? SLIDE_MOVE_UP : SLIDE_MOVE_DOWN;
        
        direction = (abs(move_x_offset) > abs(move_y_offset)) ? direction_x : direction_y;
    }
    else
    {
        direction = SLIDE_MOVE_NONE;
    }
    
    return direction;
}

/*****************************************************************************/
// 	Description : 改变方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ChangeDirection(SLIDE_MOVE_DIRECTION_E *cur_direction_Ptr)
{
    BOOLEAN ret                = TRUE;
    SLIDE_MOVE_DIRECTION_E dir = SLIDE_MOVE_INVALID;

    if (PNULL == cur_direction_Ptr)
    {
        return FALSE;
    }

    dir = *cur_direction_Ptr;

    SLIDE_DEBUG_TRACE("ChangeDirection Enter, direction = %d", dir);

    switch(*cur_direction_Ptr)
    {
    case SLIDE_MOVE_NONE:
        dir = SLIDE_MOVE_NONE;
    	break;
    case SLIDE_MOVE_LEFT:
        dir = SLIDE_MOVE_RIGHT;
    	break;
    case SLIDE_MOVE_RIGHT:
        dir = SLIDE_MOVE_LEFT;
        break;
    case SLIDE_MOVE_UP:
        dir = SLIDE_MOVE_DOWN;
        break;
    case SLIDE_MOVE_DOWN:
        dir = SLIDE_MOVE_UP;
        break;
    default:
        ret = FALSE;
        break;
    }

    SLIDE_DEBUG_TRACE("ChangeDirection Exit, direction = %d", dir);

    if (ret)
    {
        *cur_direction_Ptr = dir;
    }
    return ret;
}

/*****************************************************************************/
// 	Description : 切换窗口
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void SwitchToWin(MMI_SLIDE_INFO_T *info_ptr, MMI_WIN_ID_T win_id)
{
    uint16 index = 0;

    if (PNULL == info_ptr)
    {
        return;
    }

    if (IsWinInSlideArray(info_ptr, win_id, &index))
    {
        if (info_ptr->cur_win_id != win_id)
        {
            if (0 != info_ptr->parent_win_tab_id)
            {
                GUITAB_SetCurSel(info_ptr->parent_win_tab_id, index);
            }
            
            GrabWindowFocus(win_id, UPDAT_TYPE_ASYNCH);
            
            if (SLIDE_SYM_WORK_ALWAYS == info_ptr->sym_dis_info.work_type)
            {
                DrawCurWinSym(&s_slide_sym_layer_info, info_ptr->win_total_num, index);
            }

            SLIDE_DEBUG_TRACE("SwitchToWin Success, %d ----> %d", info_ptr->cur_win_id, win_id);

            info_ptr->cur_win_id = win_id;
        }
        else
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        
    }
}

/*****************************************************************************/
// 	Description : 更新屏幕
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateScreen(MMI_SLIDE_MOVE_T *move_ptr, MMI_SLIDE_INFO_T *info_ptr, GUI_POINT_T *cur_point_ptr,BOOLEAN is_need_update)
{
    BOOLEAN                ret       = FALSE;
    uint16                 dst_index = 0;
    BOOLEAN is_change = FALSE;
    
    if (PNULL == move_ptr || PNULL == info_ptr || PNULL == cur_point_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("UpdateScreen Enter, move state is: %d", move_ptr->state);

    if (cur_point_ptr->x != move_ptr->pre_point.x)
    {
        if (MMK_TP_SLIDE_TP_SCROLL == move_ptr->state)
        {
            dst_index = (cur_point_ptr->x < move_ptr->start_point.x) ? move_ptr->next_index : move_ptr->pre_index;
            if (dst_index != move_ptr->new_index)
            {
                if (dst_index != move_ptr->cur_index)
                {
                    if (!CopyWinbufferToLayerEx(info_ptr, info_ptr->win_array[dst_index], &s_slide_new_layer_info, &info_ptr->clip_info.main_rect))
                    {
                        SLIDE_DEBUG_TRACE("UpdateScreen , CopyWinbufferToLayerEx Fail");
                    }
                }
                else
                {
                    DisplayCommonBg(info_ptr->cur_win_id, &s_slide_new_layer_info, &info_ptr->clip_info.main_rect);
                }
                is_change = TRUE;
                SLIDE_DEBUG_TRACE("UpdateScreen , change new_layer buffer: [%d]===========>[%d]", move_ptr->new_index, dst_index);
                move_ptr->new_index = dst_index;
            }
        }
        
        if (MMK_TP_SLIDE_RECOVER != move_ptr->state)
        {
            dst_index = CalcNewSymWinIndex(move_ptr, cur_point_ptr);
            if (move_ptr->sym_index != dst_index)
            {
                SLIDE_DEBUG_TRACE("UpdateScreen , change sym_focus_index: (%d)===========>(%d)", move_ptr->sym_index, dst_index);
                move_ptr->sym_index = dst_index;
                DrawCurWinSym(&s_slide_sym_layer_info, info_ptr->win_total_num, dst_index);
            }
        }

        ret = UpdateAllLayerPos_X(move_ptr, cur_point_ptr->x - move_ptr->pre_point.x,TRUE);
        move_ptr->pre_point = *cur_point_ptr;
        ROUND_VALUE(move_ptr->pre_point.x);
        is_change = TRUE;
        HideAllBltLayer(move_ptr);
    }

    if(is_need_update && is_change)
    {
        MMITHEME_UpdateRect();
    }
    SLIDE_DEBUG_TRACE("UpdateScreen Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 更新滑动方向
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateSlideDirection(MMI_SLIDE_MOVE_T *move_ptr)
{
    BOOLEAN                ret       = FALSE;

    if (PNULL == move_ptr)
    {
        return ret;
    }

    if (MMK_TP_SLIDE_RECOVER == move_ptr->state)
    {
        if (move_ptr->cur_index == move_ptr->sym_index)
        {
            move_ptr->move_direction = GetMoveDirection(&move_ptr->start_point, &move_ptr->pre_point, MMISLIDE_TP_MOVE_DIS);
            ChangeDirection(&move_ptr->move_direction);
        }

        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("UpdateSlideDirection , ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 计算新指示器所在窗口的index
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL uint16 CalcNewSymWinIndex(MMI_SLIDE_MOVE_T *move_ptr, GUI_POINT_T *cur_point_ptr)
{
    uint16                 lcd_width       = 0;
    uint16                 dst_index       = 0;
    uint16                 move_space      = 0;

    if (PNULL == move_ptr || PNULL == cur_point_ptr)
    {
        SLIDE_DEBUG_TRACE("CalcNewSymWinIndex Input Error");
        return dst_index;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

    if (MMK_TP_SLIDE_FLING == move_ptr->state)
    {
        move_space = (MMISLIDE_TP_MOVE_DIS + RECOVER_STEP);
    }
    else
    {
        move_space = lcd_width >> 1;
    }

    if (IS_TP_MOVE_X_EXT(cur_point_ptr->x, move_ptr->start_point.x, move_space))
    {
        dst_index = (cur_point_ptr->x < move_ptr->start_point.x) ? move_ptr->next_index : move_ptr->pre_index;
    }
    else
    {
        dst_index = move_ptr->cur_index;
    }

    SLIDE_DEBUG_TRACE("CalcNewSymWinIndex Exit, %d ---->%d", move_ptr->cur_index, dst_index);
    
    return dst_index;
}

/*****************************************************************************/
// 	Description : 隐藏所有blt layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void HideAllBltLayer(MMI_SLIDE_MOVE_T *move_ptr)
{
    UILAYER_APPEND_BLT_T append_array_temp[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32               array_size                                 = 0;
    
    if (PNULL == move_ptr)
    {
        return;
    }

    SLIDE_DEBUG_TRACE("HideAllBltLayer Enter, backup layer count = %d", move_ptr->blt_layer_count);
    
    if (0 == move_ptr->blt_layer_count)
    {
        move_ptr->blt_layer_count = UILAYER_GetBltLayer(move_ptr->append_array, UILAYER_TOTAL_BLT_COUNT);
    }

    append_array_temp[array_size].lcd_dev_info = s_slide_bg_layer_info;
    append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
    
    array_size++;
    append_array_temp[array_size].lcd_dev_info = s_slide_bg_layer1_info;
    append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
    
    array_size++;
    append_array_temp[array_size].lcd_dev_info = s_slide_old_layer_info;
    append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
    
    array_size++;
    append_array_temp[array_size].lcd_dev_info = s_slide_new_layer_info;
    append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
    
    array_size++;
    append_array_temp[array_size].lcd_dev_info = s_slide_sym_layer_info;
    append_array_temp[array_size].layer_level  = UILAYER_LEVEL_NORMAL;
    
    UILAYER_SetBltLayer(append_array_temp,array_size + 1);
    
    SLIDE_DEBUG_TRACE("HideAllBltLayer Exit");
}

/*****************************************************************************/
// 	Description : 恢复所有blt layer
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL void RecoverAllBltLayer(MMI_SLIDE_MOVE_T *move_ptr)
{
    
    if (PNULL == move_ptr)
    {
        return;
    }
    
    if (0 < move_ptr->blt_layer_count)
    {
        UILAYER_SetBltLayer(move_ptr->append_array, move_ptr->blt_layer_count);
        SCI_MEMSET(move_ptr->append_array, 0x00, sizeof(move_ptr->append_array));
        move_ptr->blt_layer_count = 0;
        
        SLIDE_DEBUG_TRACE("RecoverAllBltLayer Success");
    }
}


/*****************************************************************************/
// 	Description : 隐藏激活的控件，防止控件刷新时会刷入新层
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: back_info_ptr可为空
/*****************************************************************************/
LOCAL void HideActiveCtrl(MMI_WIN_ID_T win_id, BOOLEAN is_true, SLIDE_BACKUP_CTRL_INFO *back_info_ptr)
{
    SLIDE_DEBUG_TRACE("HideActiveCtrl Enter");

    if (!MMK_IsOpenWin(win_id))
    {
        return;
    }

    if (is_true)
    {
        SLIDE_BACKUP_CTRL_INFO backup_info = {0};

        //隐藏
        backup_info.active_win_id  = win_id;
        backup_info.active_ctrl_id = MMK_GetActiveCtrlId(backup_info.active_win_id);
        
        if (MMK_IsChildWin(win_id))
        {
            backup_info.relative_win_id = MMK_GetParentWinHandle(win_id);
            backup_info.relative_ctl_id = MMK_GetActiveCtrlId(backup_info.relative_win_id);
        }
        else if (MMK_GetAllChildWinHandle(win_id, PNULL, NULL) > 0)
        {
            backup_info.relative_win_id = MMK_GetFocusChildWinId();
            backup_info.relative_ctl_id = MMK_GetActiveCtrlId(backup_info.relative_win_id);
        }

        if (0 != backup_info.active_win_id)
        {
            MMK_WinInactiveCtrl(backup_info.active_win_id, FALSE);
            SLIDE_DEBUG_TRACE("HideActiveCtrl hide success, info is: (%d %d)", backup_info.active_win_id, backup_info.active_ctrl_id);
        }
        
        if (0 != backup_info.relative_win_id)
        {
            MMK_WinInactiveCtrl(backup_info.relative_win_id, FALSE);
            SLIDE_DEBUG_TRACE("HideActiveCtrl hide success, info is: (%d %d)", backup_info.relative_win_id, backup_info.relative_ctl_id);
        }

        if (PNULL != back_info_ptr)
        {
            SCI_MEMCPY(back_info_ptr, &backup_info, sizeof(SLIDE_BACKUP_CTRL_INFO));
        }
    }
    else
    {
        //恢复
        if (PNULL != back_info_ptr)
        {
            if (0 != back_info_ptr->active_win_id && 0 != back_info_ptr->active_ctrl_id)
            {
                MMK_SetAtvCtrl(back_info_ptr->active_win_id, back_info_ptr->active_ctrl_id);
                SLIDE_DEBUG_TRACE("HideActiveCtrl show success, info is: (%d %d)", back_info_ptr->active_win_id, back_info_ptr->active_ctrl_id);
            }

            if (0 != back_info_ptr->relative_win_id && 0 != back_info_ptr->relative_ctl_id)
            {
                MMK_SetAtvCtrl(back_info_ptr->relative_win_id, back_info_ptr->relative_ctl_id);
                SLIDE_DEBUG_TRACE("HideActiveCtrl show success, info is: (%d %d)", back_info_ptr->relative_win_id, back_info_ptr->relative_ctl_id);
            }
        }
    }

    SLIDE_DEBUG_TRACE("HideActiveCtrl Exit");
}

/*****************************************************************************/
// Description : 绘制一个窗口指示器
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawSymbolItem(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T item_rect, BOOLEAN is_focus)
{
    GUI_RECT_T     dis_rect = {0};

    MMI_SLIDE_INFO_T *slide_ptr = s_cur_active_slide_info.cur_slide_info_ptr;

    if (PNULL == slide_ptr || PNULL == dev_info_ptr || UILAYER_IsEmptyLayerHandle(dev_info_ptr))
    {
        return;
    }

    dis_rect = UILAYER_GetLayerRect(dev_info_ptr);

    if (!GUI_RectIsCovered(dis_rect, item_rect))
    {
        return;
    }

    switch(slide_ptr->sym_dis_info.dis_type)
    {
    case SLIDE_SYMBOL_TYPE_DEFAULT:
        {
            GUI_COLOR_T cur_color  = 0;
            uint16      cur_radius = 0;
            GUI_POINT_T circle_pos = {0};
            
            circle_pos.x = item_rect.left + ((item_rect.right  - item_rect.left + 1) >> 1);
            circle_pos.y = item_rect.top +  ((item_rect.bottom - item_rect.top + 1) >> 1);
            
            //FIll Circle
            cur_color  = is_focus ? WIN_SYM_FOCUS_FILL_COLOR : WIN_SYM_NORMAL_FILL_COLOR;
            cur_radius = is_focus ? WIN_SYM_RADIUS : WIN_SYM_RADIUS - 1;
            LCD_FillCircle(dev_info_ptr, &dis_rect, circle_pos.x, circle_pos.y, cur_radius, cur_color);
            
            if (!is_focus)
            {
                LCD_DrawCircle(dev_info_ptr, &dis_rect, circle_pos.x, circle_pos.y, WIN_SYM_RADIUS, WIN_SYM_BORDER_COLOR);
            }
        }
        break;
    case SLIDE_SYMBOL_TYPE_CUSTOM_IMG:
        {
            uint16         img_w    = 0;
            uint16         img_h    = 0;
            MMI_IMAGE_ID_T img_id   = 0;
            MMI_WIN_ID_T   win_id   = slide_ptr->cur_win_id;
            
            img_id = is_focus ? slide_ptr->sym_dis_info.img_id_arr[SYMBOL_FOCUS_IMG] : slide_ptr->sym_dis_info.img_id_arr[SYMBOL_NORMAL_IMG];
            if (GUIRES_GetImgWidthHeight(&img_w, &img_h, img_id, win_id))
            {
                dis_rect = GUI_GetCenterRectExt(item_rect, img_w, img_h);
                GUIRES_DisplayImg(PNULL, &dis_rect, PNULL, win_id, img_id, dev_info_ptr);
            }
        }
        break;
    case SLIDE_SYMBOL_TYPE_OWNERDRAW:
        {
            if (PNULL != slide_ptr->sym_dis_info.OwnerDrawSymItemFunc)
            {
                slide_ptr->sym_dis_info.OwnerDrawSymItemFunc(dev_info_ptr, item_rect, is_focus);
            }
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
// Description : 绘制窗口指示器背景
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawSymbolBg(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T bg_rect)
{
    GUI_RECT_T     dis_rect = {0};

    MMI_SLIDE_INFO_T *slide_ptr   = s_cur_active_slide_info.cur_slide_info_ptr;

    if (PNULL == slide_ptr || PNULL == dev_info_ptr || UILAYER_IsEmptyLayerHandle(dev_info_ptr))
    {
        return;
    }

    dis_rect = UILAYER_GetLayerRect(dev_info_ptr);

    if (!GUI_RectIsCovered(dis_rect, bg_rect))
    {
        return;
    }

    switch(slide_ptr->sym_dis_info.dis_type)
    {
    case SLIDE_SYMBOL_TYPE_CUSTOM_IMG:
        {
            uint16         img_w    = 0;
            uint16         img_h    = 0;
            MMI_IMAGE_ID_T img_id   = 0;
            MMI_WIN_ID_T   win_id   = slide_ptr->cur_win_id;
            
            img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BG_IMG];
            if (GUIRES_GetImgWidthHeight(&img_w, &img_h, slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BG_IMG], win_id))
            {
                BOOLEAN        ret       = FALSE;
                uint16         imgl_w    = 0;
                uint16         imgl_h    = 0;
                uint16         imgr_w    = 0;
                uint16         imgr_h    = 0;

                img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BGL_IMG];
                ret = GUIRES_GetImgWidthHeight(&imgl_w, &imgl_h, img_id, win_id);

                img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BGR_IMG];
                ret &= GUIRES_GetImgWidthHeight(&imgr_w, &imgr_h, img_id, win_id);

                dis_rect = bg_rect;

                img_w = (dis_rect.right - dis_rect.left + 1);

                if (ret)
                {
                    //L
                    dis_rect.left  = bg_rect.left;
                    dis_rect.right = dis_rect.left + imgl_w;
                    dis_rect = GUI_GetCenterRectExt(dis_rect, imgl_w, imgl_h);
                    img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BGL_IMG];
                    GUIRES_DisplayImg(PNULL, &dis_rect, PNULL, win_id, img_id, dev_info_ptr);

                    //M
                    img_w -= (imgl_w + imgr_w);
                    dis_rect.left  = dis_rect.right + 1;
                    dis_rect.right = dis_rect.left + img_w;
                    dis_rect = GUI_GetCenterRectExt(dis_rect, img_w, img_h);
                    img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BG_IMG];
                    GUIRES_DisplayImg(PNULL, &dis_rect, PNULL, win_id, img_id, dev_info_ptr);

                    //R
                    dis_rect.left  = dis_rect.right + 1;
                    dis_rect.right = dis_rect.left + imgr_w;
                    dis_rect = GUI_GetCenterRectExt(dis_rect, imgl_w, imgl_h);
                    img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BGR_IMG];
                    GUIRES_DisplayImg(PNULL, &dis_rect, PNULL, win_id, img_id, dev_info_ptr);
                }
                else
                {
                    dis_rect = GUI_GetCenterRectExt(dis_rect, img_w, img_h);
                    img_id = slide_ptr->sym_dis_info.img_id_arr[SYMBOL_BG_IMG];
                    GUIRES_DisplayImg(PNULL, &dis_rect, PNULL, win_id, img_id, dev_info_ptr);
                }
            }
        }
        break;
    case SLIDE_SYMBOL_TYPE_OWNERDRAW:
        {
            if (PNULL != slide_ptr->sym_dis_info.OwnerDrawSymBgFunc)
            {
                slide_ptr->sym_dis_info.OwnerDrawSymBgFunc(dev_info_ptr, bg_rect);
            }
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
// Description : 绘制窗口指示器
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DrawCurWinSym(GUI_LCD_DEV_INFO* dev_info_ptr, uint16 sym_count, uint16 cur_index)
{
    uint32      i                 = 0;
    GUI_RECT_T  lcd_rect          = {0};
    GUI_RECT_T  bg_rect           = {0};
    GUI_RECT_T  item_rect         = {0};
    uint16      item_width        = WIN_SYM_POINT_WIDTH;
    uint16      bg_width          = 0;
    uint16      lcd_width         = 0;
    uint16      lcd_height        = 0;
    uint16      point_space       = 0;
    BOOLEAN     is_focus          = FALSE;
    
    MMI_SLIDE_INFO_T *slide_ptr   = s_cur_active_slide_info.cur_slide_info_ptr;

    if (PNULL == slide_ptr || PNULL == dev_info_ptr || cur_index >= sym_count || UILAYER_IsEmptyLayerHandle(dev_info_ptr))
    {
        SLIDE_DEBUG_TRACE("DrawCurWinSym, Error");
        return;
    }

    if (SLIDE_SYM_WORK_NONE == slide_ptr->sym_dis_info.work_type)
    {
        return;
    }
    
    lcd_rect = UILAYER_GetLayerRect(dev_info_ptr);
    
    lcd_width  = lcd_rect.right - lcd_rect.left + 1;
    lcd_height = lcd_rect.bottom - lcd_rect.top + 1;
    
    SLIDE_DEBUG_TRACE("DrawCurWinSym, cur index = %d", cur_index);
    
    if (1 < sym_count)
    {
        int16 tmp_space = 0;
        
        tmp_space = (lcd_width - (item_width * sym_count)) / sym_count;
        
        if (tmp_space <= 0)
        {
            return;
        }
        
        point_space = MIN(tmp_space, WIN_SYM_POINT_SPACE);
        bg_width    = sym_count * (item_width + point_space);

        bg_rect = lcd_rect;
        bg_rect.left  = lcd_rect.left + ((lcd_width - bg_width) >> 1);
        bg_rect.right = bg_rect.left + bg_width;

        UILAYER_Clear(dev_info_ptr);

        DrawSymbolBg(dev_info_ptr, bg_rect);

        item_rect = bg_rect;

        for (i = 0; i < sym_count; i++)
        {
            is_focus = (i == cur_index) ? TRUE : FALSE;
            item_rect.left  = bg_rect.left + (int16)(i * (item_width + point_space)) + (point_space >> 1);
            item_rect.right = item_rect.left + item_width;
            DrawSymbolItem(dev_info_ptr, item_rect, is_focus);
        }
    }
}

/*****************************************************************************/
// Description : 显示通用背景图片
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL void DisplayCommonBg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T *dis_rect_ptr)
{
    GUI_POINT_T layer_pos = {0};

    if (PNULL == dev_info_ptr || UILAYER_IsEmptyLayerHandle(dev_info_ptr))
    {
        return;
    }

    UILAYER_Clear(dev_info_ptr);

    UILAYER_GetLayerPosition(dev_info_ptr, &layer_pos.x, &layer_pos.y);
    
    UILAYER_SetLayerPosition(dev_info_ptr, 0, layer_pos.y);

    if (PNULL != dis_rect_ptr)
    {
        MMITHEME_DisplayCommonBg(dis_rect_ptr, win_id, dev_info_ptr);
    }

    UILAYER_SetLayerPosition(dev_info_ptr, layer_pos.x, layer_pos.y);
}


/*****************************************************************************/
// 	Description : 显示窗口指示器
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayWinSymbol(MMI_SLIDE_INFO_T *info_ptr)
{
    BOOLEAN               result    = FALSE;
    SLIDE_SYM_WORK_TYPR_E work_type = SLIDE_SYM_WORK_NONE;
    
    if (PNULL == info_ptr)
    {
        return result;
    }

    work_type = info_ptr->sym_dis_info.work_type;

    switch(work_type)
    {
    case SLIDE_SYM_WORK_NONE:
    	break;
    case SLIDE_SYM_WORK_NORMAL:
    case SLIDE_SYM_WORK_ALWAYS:
    case SLIDE_SYM_WORK_AUTO_HIDE:
        {
            UILAYER_APPEND_BLT_T blt   = {0};
            uint16               index = 0;

            if (IsWinInSlideArray(info_ptr, info_ptr->cur_win_id, &index))
            {
                DrawCurWinSym(&s_slide_sym_layer_info, info_ptr->win_total_num, index);
                blt.layer_level = UILAYER_LEVEL_NORMAL;
                blt.lcd_dev_info = s_slide_sym_layer_info;
                ClearSymbolDisInfo(info_ptr);
                UILAYER_AppendBltLayer(&blt);

                if (SLIDE_SYM_WORK_AUTO_HIDE == work_type)
                {
                    info_ptr->out_state = UILAYER_ALPHA_MAX;
                    StartSlideTimerEx(&info_ptr->sym_remove_timer_id, AUTO_HIDE_TICK, RemoveSymbolCB, (uint32)info_ptr->handle);
                }

                result = TRUE;
            }
        }
        break;
    default:
        break;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 初始化数据是否有效
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckInitDataIsValid(MMI_SLIDE_INIT_T *init_data_ptr)
{
    BOOLEAN result        = TRUE;
    uint16  total_num     = 0;
    uint32  i =           0;

    if (PNULL == init_data_ptr)
    {
        SLIDE_DEBUG_TRACE("CheckInitDataIsValid , input is empty");
        return FALSE;
    }

    if (MMI_SLIDE_INVALID == init_data_ptr->type)
    {
        SLIDE_DEBUG_TRACE("CheckInitDataIsValid , type error");
        result = FALSE;
    }
    else if (PNULL == init_data_ptr->win_id_arr_ptr)
    {
        SLIDE_DEBUG_TRACE("CheckInitDataIsValid , win_id_arr_ptr error");
        result = FALSE;
    }
    else if (SLIDE_RECT_TYPE_INVALID == init_data_ptr->slide_rect_type)
    {
        SLIDE_DEBUG_TRACE("CheckInitDataIsValid , slide_rect_type error");
        result = FALSE;
    }
    else
    {
        total_num = MIN(init_data_ptr->win_id_arr_size , MMISLIDE_WIN_TOTAL_COUNT);

        if (MMI_SLIDE_MANY_PAINT_ONE_WIN == init_data_ptr->type)
        {
            if (total_num != 1)
            {
                result = FALSE;
            }
        }
        else
        {
            MMI_WIN_ID_T src_win_id = 0;
            MMI_HANDLE_T parent_win = 0;

            if (MMI_SLIDE_ALL_CHILD_WIN == init_data_ptr->type)
            {
                parent_win = MMK_GetParentWinHandle(init_data_ptr->default_win);
                
                for (i = 0; i < total_num; i++)
                {
                    src_win_id = init_data_ptr->win_id_arr_ptr[i];

                    if (0 != src_win_id)
                    {
                        MMI_HANDLE_T tmp_win = MMK_GetParentWinHandle(src_win_id);

                        if (tmp_win != parent_win)
                        {
                            result = FALSE;
                            break;
                        }
                    }
                }
            }
            else
            {
                src_win_id = init_data_ptr->win_id_arr_ptr[i];
                
                if (0 != src_win_id)
                {
                    parent_win = MMK_GetParentWinHandle(src_win_id);
                    
                    if (0 != parent_win)
                    {
                        result = FALSE;
                    }
                }
            }
        }
        
    }

    return result;
}

/*****************************************************************************/
// Description : handle 是否有效
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckSlideHandleIsValid(MMI_SLIDE_HANDLE *handle_ptr, uint16 *index_ptr)
{
    uint32  i      = 0;
    BOOLEAN result = FALSE;

    if (PNULL == handle_ptr)
    {
        return FALSE;
    }

    SLIDE_DEBUG_TRACE("CheckSlideHandleIsValid Enter, handle = %d", *handle_ptr);

    for (i = 0; i < MMISLIDE_TOTAL_COUNT; i++)
    {
        if (*handle_ptr == s_valis_handle_array[i])
        {
            SLIDE_DEBUG_TRACE("Find Success, index = %d", i);
            if (PNULL != index_ptr)
            {
                *index_ptr = i;
            }
            result = TRUE;
            break;
        }
    }

    SLIDE_DEBUG_TRACE("CheckSlideHandleIsValid Exit, result = %d", result);

    return result;
}

/*****************************************************************************/
// Description : 关闭窗口(若窗口非open状态，则退出 )
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CloseWin(MMI_WIN_ID_T win_id)
{
    BOOLEAN      ret                                = FALSE;
    uint16       child_win_num                      = 0;
    MMI_WIN_ID_T child_win_arr[CHILD_WIN_MAX_COUNT] = {0};
    uint32       i                                  = 0;

    ret = MMK_IsOpenWin(win_id);

    if (ret)
    {
        child_win_num = MMK_GetAllChildWinHandle(win_id, child_win_arr, CHILD_WIN_MAX_COUNT);

        if (child_win_num > 0)
        {
            child_win_num = MIN(child_win_num, CHILD_WIN_MAX_COUNT);

            for (i = 0; i < child_win_num; i++)
            {
                CloseWin(child_win_arr[i]);
            }
        }
        MMK_CloseWin(win_id);
    }

    return ret;
}

/*****************************************************************************/
// Description : 设置焦点到某一个窗口
// Global resource dependence : 
// Author:xiaoming.ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GrabWindowFocus(MMI_WIN_ID_T win_id, UPDATE_TYPR_E update_type)
{
    if (UPDAT_TYPE_INVALID == update_type || MMK_IsFocusWin(win_id) || !MMK_IsOpenWin(win_id))
    {
        return FALSE;
    }

    MMK_WinGrabFocus(win_id);
    
    if (UPDAT_TYPE_NONE == update_type)
    {
        MMK_DeleteMSGByHwndAndMsg(win_id, MSG_FULL_PAINT);

        if (MMK_IsChildWin(win_id))
        {
            MMK_DeleteMSGByHwndAndMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT);
        }
        else if (MMK_GetAllChildWinHandle(win_id, PNULL, NULL) > 0)
        {
            MMK_DeleteMSGByHwndAndMsg(MMK_GetFocusChildWinId(), MSG_FULL_PAINT);
        }
    }
    else if (UPDAT_TYPE_SYNCH == update_type)
    {
        MMK_DeleteMSGByHwndAndMsg(win_id, MSG_FULL_PAINT);

        if (MMK_IsChildWin(win_id))
        {
            MMK_DeleteMSGByHwndAndMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT);
        }
        else if (MMK_GetAllChildWinHandle(win_id, PNULL, NULL) > 0)
        {
            MMK_DeleteMSGByHwndAndMsg(MMK_GetFocusChildWinId(), MSG_FULL_PAINT);
        }

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 输出str trace
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
#define MAX_TRACE_BUFFER     (0xff + 1)
LOCAL void MMISLIDE_StrTraceOut(const char *x_format, ...)
{
    char    format_str[MAX_TRACE_BUFFER] = {0};
    va_list argp                      = {0};
    
    va_start(argp, x_format); /*lint -esym(628,__va_start)*/
    
    _vsnprintf(format_str, MAX_TRACE_BUFFER, x_format, argp);
    SCI_TRACE_LOW("%s %s\n", TRACE_CHARS, format_str);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SLIDE_2773_112_2_18_1_59_2_74,(uint8*)"ss", TRACE_CHARS, format_str);
    
    va_end(argp);
}

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 创建Slide实例
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Creat(MMI_SLIDE_INIT_T *slide_init_ptr, MMI_SLIDE_HANDLE *handle_ptr)
{
    BOOLEAN           result        = FALSE;
    uint32            i             = 0;
    uint16            index         = 0;
    uint32            arr_size      = 0;
    MMI_WIN_ID_T      src_win       = 0;
    MMI_SLIDE_INFO_T cur_slide_info = {0};
    MMI_SLIDE_INFO_T *info_ptr      = PNULL;

    if (PNULL == slide_init_ptr || PNULL == handle_ptr)
    {
        SLIDE_DEBUG_TRACE("MMIAPISLIDE_Creat, input is NULL");
        return result;
    }

    if (CheckInitDataIsValid(slide_init_ptr))
    {
        //查找是否有空池存放当前实例
        for (i = 0; i < MMISLIDE_TOTAL_COUNT; i++)
        {
            if (0 == s_valis_handle_array[i])
            {
                index = i;
                result = TRUE;
                break;
            }
        }

        if (result)
        {
           //找到空池
            result   = FALSE;
            info_ptr = &cur_slide_info;

            info_ptr->is_loop_move      = TRUE;    //默认循环滚动
            info_ptr->is_sym_on_top     = FALSE;   //默认位于底部
            info_ptr->is_valid          = FALSE;   //默认非激活状态
            
            info_ptr->type              = slide_init_ptr->type;
            info_ptr->default_win_id    = slide_init_ptr->default_win;
            info_ptr->cur_win_id        = info_ptr->default_win_id;

            info_ptr->sym_dis_info.dis_type  = SLIDE_SYMBOL_TYPE_DEFAULT;
            info_ptr->sym_dis_info.work_type = SLIDE_SYM_WORK_NORMAL;

            info_ptr->win_switch_type        = MMISLIDE_NORMAL_SWITCH;

            if (!slide_init_ptr->is_has_symbol)
            {
                info_ptr->sym_dis_info.work_type = SLIDE_SYM_WORK_NONE;
            }
            
            if (PNULL != slide_init_ptr->parent_win_tab_ptr && MMI_SLIDE_ALL_CHILD_WIN == slide_init_ptr->type)
            {
                info_ptr->parent_win_tab_id = *slide_init_ptr->parent_win_tab_ptr;
            }

            arr_size = MIN(slide_init_ptr->win_id_arr_size, MMISLIDE_WIN_TOTAL_COUNT);
            for (i = 0; i < arr_size; i++)
            {
                src_win = slide_init_ptr->win_id_arr_ptr[i];
                if (0 != src_win && !IsWinInAllSlideArray(src_win, PNULL, PNULL))
                {
                    info_ptr->win_array[i] = src_win;
                    info_ptr->win_total_num ++;
                }
            }

            if (info_ptr->win_total_num > 1)
            {
                info_ptr->handle = MMI_SLIDE_HANDLE_START + index;
                
                if (!IsWinInSlideArray(info_ptr, info_ptr->default_win_id, PNULL))
                {
                    info_ptr->default_win_id = info_ptr->win_array[0];
                }
                
                if (MMK_IsOpenWin(info_ptr->default_win_id))
                {
                    if (SLIDE_RECT_TYPE_FULLSCREEN == slide_init_ptr->slide_rect_type)
                    {
                        info_ptr->slide_both_rect = MMITHEME_GetFullScreenBothRect();
                    }
                    else if (SLIDE_RECT_TYPE_CLIENTRECT == slide_init_ptr->slide_rect_type)
                    {
                        info_ptr->slide_both_rect = MMITHEME_GetWinClientBothRect(info_ptr->default_win_id);
                    }
                    else
                    {
                        if (PNULL != slide_init_ptr->slide_both_rect_ptr)
                        {
                            SCI_MEMCPY(&info_ptr->slide_both_rect, slide_init_ptr->slide_both_rect_ptr, sizeof(GUI_BOTH_RECT_T));
                        }
                    }

                    if (!GUI_IsRectEmpty(info_ptr->slide_both_rect.h_rect) && !GUI_IsRectEmpty(info_ptr->slide_both_rect.v_rect))
                    {
                        s_valis_handle_array[index] = info_ptr->handle;
                        *handle_ptr = info_ptr->handle;
                        info_ptr->handle_ptr = handle_ptr;
                        s_valis_handle_top_index = index;
                        SetSlideHookFunc(info_ptr->win_array, info_ptr->win_total_num, TRUE);
                        SCI_MEMCPY(&s_slide_info_array[index], info_ptr, sizeof(MMI_SLIDE_INFO_T));

                        SLIDE_DEBUG_TRACE("MMISLIDE_Creat Success, handle = %d, index = %d", *handle_ptr, index);
                        
                        result = TRUE;
                    }
                }
            }
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Creat Exit, ret = %d", result);
    
    return result;
}

/*****************************************************************************/
// 	Description : 销毁Slide系统
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Release(MMI_SLIDE_HANDLE *handle_ptr)
{
    return MMIAPISLIDE_ReleaseEx(handle_ptr, TRUE);
}

/*****************************************************************************/
// 	Description : 激活
//	Global resource dependence: 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Active(MMI_SLIDE_HANDLE *handle_ptr)
{
    BOOLEAN                  ret        = FALSE;
    uint16                   index      = 0;
    MMI_SLIDE_INFO_T        *info_ptr   = PNULL;
    MMI_SLIDE_ACTIVE_INFO_T *active_ptr = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Active Enter, handle = %d", *handle_ptr);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr   = &s_slide_info_array[index];
        active_ptr = &s_cur_active_slide_info;

        if (!info_ptr->is_valid)
        {
            if (CreateAllSlideLayer(info_ptr))
            {
                if (0 == info_ptr->cur_win_id)
                {
                    info_ptr->cur_win_id = info_ptr->default_win_id;
                }

                SLIDE_DEBUG_TRACE("MMIAPISLIDE_Active, cur_win_id = %d", info_ptr->cur_win_id);

                if (IsWinInSlideArray(info_ptr, info_ptr->cur_win_id, &index))
                {
                    info_ptr->is_valid   = TRUE;
                    
                    active_ptr->cur_slide_info_ptr = info_ptr;

                    if (MMI_SLIDE_ALL_CHILD_WIN == info_ptr->type)
                    {
                        if (0 != info_ptr->parent_win_tab_id)
                        {
                            GUITAB_SetCurSel(info_ptr->parent_win_tab_id, index);
                        }
                    }
                    
                    GrabWindowFocus(info_ptr->cur_win_id, UPDAT_TYPE_ASYNCH);
                    
                    if (SLIDE_SYM_WORK_ALWAYS == info_ptr->sym_dis_info.work_type || SLIDE_SYM_WORK_AUTO_HIDE == info_ptr->sym_dis_info.work_type)
                    {
                        DisplayWinSymbol(info_ptr);
                    }

                    ret = TRUE;
                }
                else
                {
                    ReleaseAllSlideLayer(info_ptr);
                    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Active FAIL, cur_win_id is error");
                }
            }
            else
            {
                SLIDE_DEBUG_TRACE("MMIAPISLIDE_Active FAIL, No Memory");
            }
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Active Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 去激活
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Deactive(MMI_SLIDE_HANDLE *handle_ptr)
{
    BOOLEAN                 ret         = FALSE;
    uint16                  index       = 0;
    MMI_SLIDE_INFO_T        *info_ptr   = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Deactive Enter, handle = %d", *handle_ptr);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        if (info_ptr->is_valid)
        {
            info_ptr->is_valid   = FALSE;
            ReleaseAllSlideLayer(info_ptr);
        }
        
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Deactive Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 销毁Slide系统
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_ReleaseEx(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_close_window)
{
    uint16            index     = 0;
    uint32            i         = 0;
    BOOLEAN           ret       = FALSE;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;
    MMI_SLIDE_INFO_T cur_slide  = {0};

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Release Enter, handle = %d, is_close_window = %d", *handle_ptr, is_close_window);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        if (info_ptr->is_valid)
        {
            MMIAPISLIDE_Deactive(handle_ptr);
        }
        
        if (s_valis_handle_top_index < MMISLIDE_TOTAL_COUNT && index <= s_valis_handle_top_index)
        {
            SCI_MEMCPY(&cur_slide, info_ptr, sizeof(MMI_SLIDE_INFO_T));

            SetSlideHookFunc(info_ptr->win_array, info_ptr->win_total_num, FALSE);

            if (index != s_valis_handle_top_index)
            {
                for (i = index; i < s_valis_handle_top_index; i++)
                {
                    SCI_MEMCPY(&s_slide_info_array[i], &s_slide_info_array[i + 1], sizeof(MMI_SLIDE_INFO_T));
                }
            }
            
            s_valis_handle_array[s_valis_handle_top_index] = 0;
            SCI_MEMSET(&s_slide_info_array[s_valis_handle_top_index], 0x00, sizeof(MMI_SLIDE_INFO_T));

            if (s_valis_handle_top_index > 0)
            {
                s_valis_handle_top_index--;
            }
            
            //关闭所有窗口
            if (is_close_window)
            {
                if (MMI_SLIDE_ALL_CHILD_WIN == cur_slide.type)
                {
                    CloseWin(MMK_GetParentWinHandle(cur_slide.default_win_id));
                }
                else if (MMI_SLIDE_MANY_PAINT_ONE_WIN == cur_slide.type)
                {
                    CloseWin(cur_slide.default_win_id);
                }
                else
                {
                    for (i = 0; i < cur_slide.win_total_num; i++)
                    {
                         if (cur_slide.cur_win_id != cur_slide.win_array[i])
                         {
                            CloseWin(cur_slide.win_array[i]);
                         }
                    }
                    
                     CloseWin(cur_slide.cur_win_id);
                }
            }
            
            *handle_ptr = MMI_SLIDE_HANDLE_INVALID;

            if (PNULL != cur_slide.handle_ptr)
            {
                *cur_slide.handle_ptr = MMI_SLIDE_HANDLE_INVALID;
            }
            
            ret = TRUE;
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_Release Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 是否激活
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_IsActive(MMI_SLIDE_HANDLE *handle_ptr)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_IsActive Enter, handle = %d", *handle_ptr);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        ret = info_ptr->is_valid;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_IsActive Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置循环滚动
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetLoopSlide(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_loop)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetLoopSlide Enter, handle = %d, is_loop = %d", *handle_ptr, is_loop);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        info_ptr->is_loop_move = is_loop;

        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetLoopSlide Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置指示器是否在窗口顶部
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolIsOnTop(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_on_top)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;
    
    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolIsOnTop Enter, handle = %d, is_on_top = %d", *handle_ptr, is_on_top);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        info_ptr->is_sym_on_top = is_on_top;

        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolIsOnTop Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置指示器相对位置
//	Global resource dependence : 
//  Author: Jiaoyou.wu
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolPosition(MMI_SLIDE_HANDLE *handle_ptr, GUI_POINT_T *symbol_point_ptr)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;
    
    if (PNULL == handle_ptr || PNULL == symbol_point_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolPosition Enter, handle = %d, x = %d, y = %d", *handle_ptr, symbol_point_ptr->x, symbol_point_ptr->y);

    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        SCI_MEMCPY(&info_ptr->symbol_point, symbol_point_ptr, sizeof(GUI_POINT_T));

        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolIsOnTop Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置指示器的显示类型
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolWorkType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_WORK_TYPR_E work_type)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolWorkType Enter, handle = %d, work_type = %d", *handle_ptr, work_type);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];
        
        info_ptr->sym_dis_info.work_type = work_type;
        
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolWorkType Exit, ret = %d", ret);
    
    return ret;
}
/*****************************************************************************/
// 	Description : 设置symbol 显示信息
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolDisplayInfo(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_DIS_INFO_T *dis_info_ptr)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;
    
    if (PNULL == handle_ptr || PNULL == dis_info_ptr)
    {
        return ret;
    }
    
    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolDisplayInfo Enter, handle = %d", *handle_ptr);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];
        
        SCI_MEMCPY(&info_ptr->sym_dis_info, dis_info_ptr, sizeof(SLIDE_SYM_DIS_INFO_T));
        
        ret = TRUE;
    }
    
    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetSymbolDisplayInfo Exit, ret = %d", ret);
    
    return ret;
}

/*****************************************************************************/
// 	Description : 设置默认窗口的id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetDefaultWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetDefaultWin Enter, handle = %d, win_id = %d", *handle_ptr, win_id);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];
        
        if (IsWinInSlideArray(info_ptr, win_id, PNULL))
        {
            info_ptr->default_win_id = win_id;
            ret = TRUE;
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetDefaultWin Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置当前窗口的id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetCurWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetCurWin Enter, handle = %d, win_id = %d", *handle_ptr, win_id);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];
        
        if (IsWinInSlideArray(info_ptr, win_id, PNULL))
        {
            info_ptr->cur_win_id = win_id;
            ret = TRUE;
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetCurWin Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 设置窗口切换类型
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetWinSwitchType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_WIN_SWITCH_TYPE_E switch_type)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetWinSwitchType Enter, handle = %d, switch_type = %d", *handle_ptr, switch_type);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];

        info_ptr->win_switch_type = switch_type;
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SetWinSwitchType Exit, ret = %d", ret);

    return ret;
}

/*****************************************************************************/
// 	Description : 获取Slide Info
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_GetInfo(MMI_SLIDE_HANDLE *handle_ptr, MMI_SLIDE_INFO_T *slide_info_ptr)
{
    BOOLEAN           ret       = FALSE;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == slide_info_ptr || PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_GetInfo Enter, handle = %d", *handle_ptr);
    
    info_ptr = GetSlideInfoPtr(*handle_ptr);
    if (PNULL != info_ptr)
    {
        SCI_MEMCPY(slide_info_ptr, info_ptr, sizeof(MMI_SLIDE_INFO_T));
        ret = TRUE;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_GetInfo Exit, ret = %d", ret);
    
    return ret;
}

/*****************************************************************************/
// 	Description : 切换到某一个窗口
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SwitchToWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id)
{
    BOOLEAN           ret       = FALSE;
    uint16            index     = 0;
    MMI_SLIDE_INFO_T *info_ptr  = PNULL;

    if (PNULL == handle_ptr)
    {
        return ret;
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SwitchToWin Enter, handle = %d, win_id = %d", *handle_ptr, win_id);
    
    if (CheckSlideHandleIsValid(handle_ptr, &index))
    {
        info_ptr = &s_slide_info_array[index];
        
        if (IsWinInSlideArray(info_ptr, win_id, &index))
        {
            SLIDE_DEBUG_TRACE("MMIAPISLIDE_SwitchToWin, handle is_valid = %d, back_up_handle = %d", s_cur_backup_handle);

            if (!info_ptr->is_valid)
            {
                if (info_ptr->handle == s_cur_backup_handle)
                {
                    MMI_WIN_ID_T backup_win_id  = info_ptr->cur_win_id;

                    info_ptr->cur_win_id = win_id;
                    if (MMIAPISLIDE_Active(handle_ptr))
                    {
                        ret = TRUE;
                        s_cur_backup_handle = MMI_SLIDE_HANDLE_INVALID;
                    }
                    else
                    {
                        uint16  cur_index = 0;

                        if (MMI_SLIDE_ALL_CHILD_WIN == info_ptr->type)
                        {
                            if (0 != info_ptr->parent_win_tab_id)
                            {
                                if (IsWinInSlideArray(info_ptr, backup_win_id, &cur_index))
                                {
                                    GUITAB_SetCurSel(info_ptr->parent_win_tab_id, cur_index);
                                }
                                
                            }
                        }

                        info_ptr->cur_win_id = backup_win_id;
                    }
                }
                else
                {
                    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SwitchToWin Fail, Handle Is invalid");
                }
            }
            else
            {
                SwitchToWin(info_ptr, win_id);
                ret = TRUE;
            }
        }
    }

    SLIDE_DEBUG_TRACE("MMIAPISLIDE_SwitchToWin Exit, ret = %d", ret);

    return ret;
}

#endif /*#ifdef MMI_SLIDE_SUPPORT*/




/*Edit by script, ignore 4 case. Thu Apr 26 19:01:13 2012*/ //IGNORE9527
