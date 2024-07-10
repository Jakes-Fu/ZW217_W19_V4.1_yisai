/****************************************************************************
** File Name:      mmislide_export.h 
** Author:                                                                  
** Date:           2012/02/10
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This is mmi slide win include file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 02/10/2012        xiaoming             Create
** 
****************************************************************************/
#ifndef _MMISLIDE_EXPORT_H
#define _MMISLIDE_EXPORT_H

#ifdef MMI_SLIDE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "sci_types.h"
#include "guitab.h"
#include "Mmk_tp.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISLIDE_SCROLL_TIME              20                  //刷屏定时器时长

#define MMISLIDE_WIN_TOTAL_COUNT          10                  //实例包含的最大窗口数
#define MMISLIDE_TOTAL_COUNT              8                   //最大实例数目

#define MMISLIDE_TP_MOVE_DIS              5                   //超过该值则认为是slide move

#define WIN_SYM_RADIUS                    4                   //窗口指示器的半径
#define WIN_SYM_BORDER_COLOR              MMI_GRAY_COLOR      //窗口指示器的边框颜色
#define WIN_SYM_NORMAL_FILL_COLOR         MMI_WHITE_COLOR     //窗口指示器的填充颜色
#define WIN_SYM_FOCUS_FILL_COLOR          MMI_BLUE_COLOR      //窗口指示器的焦点颜色

#define MSG_SLIDE_MOVE_BEGIN              0xf100              //move启动时给窗口发送的消息
#define MSG_SLIDE_MOVE_END                0xf101              //move停止时给窗口发送的消息
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMI_SLIDE_ALL_CHILD_WIN = 0xff,      //所有窗口均为子窗口，且他们存在同一个父窗口
    MMI_SLIDE_ALL_NO_CHILD_WIN,          //所有窗口均非子窗口
    MMI_SLIDE_MANY_PAINT_ONE_WIN,         //仅有一个窗口，多次刷新(暂未实现)
    MMI_SLIDE_INVALID
}MMI_SLIDE_TYPE;

typedef enum
{
    MMI_SLIDE_HANDLE_START = 0xff,  //起始句柄 255
    MMI_SLIDE_HANDLE_MAX   = MMI_SLIDE_HANDLE_START + MMISLIDE_TOTAL_COUNT,
    MMI_SLIDE_HANDLE_INVALID
}MMI_SLIDE_HANDLE;

typedef enum
{
    SLIDE_RECT_TYPE_FULLSCREEN = 0xff, //滑动区域为全屏
    SLIDE_RECT_TYPE_CLIENTRECT,        //滑动区域为客户区域(以默认窗口为基准)
    SLIDE_RECT_TYPE_CUSTOM,            //自定义滑动区域，初始化时both_rect 不允许为空
    SLIDE_RECT_TYPE_INVALID
}MMI_SLIDE_RECT_TYPE;

typedef enum
{
    SLIDE_SYMBOL_TYPE_DEFAULT = 0xff, //默认(无背景，指示器为gui绘制的圆点)
    SLIDE_SYMBOL_TYPE_CUSTOM_IMG,     //自定义套图
    SLIDE_SYMBOL_TYPE_OWNERDRAW,      //自绘
    SLIDE_SYMBOL_TYPE_INVALID
}MMI_SLIDE_SYMBOL_TYPE;

typedef enum
{
    SLIDE_SYM_WORK_NONE,              //不显示
    SLIDE_SYM_WORK_NORMAL,            //仅滑动时显示
    SLIDE_SYM_WORK_ALWAYS,            //一直显示
    SLIDE_SYM_WORK_AUTO_HIDE,         //自动隐藏
}SLIDE_SYM_WORK_TYPR_E;

typedef enum
{
    SYMBOL_NORMAL_IMG,               //默认
    SYMBOL_FOCUS_IMG,                //焦点
    SYMBOL_BG_IMG,                   //背景
    SYMBOL_BGL_IMG,                  //左背景
    SYMBOL_BGR_IMG,                  //右背景
    SYMBOL_IMG_MAX
}MMI_SLIDE_SYMBOL_IMG_E;

typedef enum
{
    MMISLIDE_NORMAL_SWITCH,                  //默认效果，支持平滑移动特效
    MMISLIDE_PROMPTLY_SWITCH,                //立即切换，无特效(normal模式内存不足时会临时切换为该模式)
    MMISLIDE_NONE_SWITCH,                    //不切换窗口(某些情况下需要打开slide数组中的某一个窗口)
}SLIDE_WIN_SWITCH_TYPE_E;

typedef struct
{
    MMI_SLIDE_TYPE      type;                  //类型
    MMI_WIN_ID_T       *win_id_arr_ptr;        //窗口id数组指针
    uint32              win_id_arr_size;       //窗口id数组size
    MMI_WIN_ID_T        default_win;           //进入系统后的默认窗口id
    MMI_SLIDE_RECT_TYPE slide_rect_type;       //滑动区域类型
    GUI_BOTH_RECT_T    *slide_both_rect_ptr;   //滑动区域指针
    MMI_CTRL_ID_T      *parent_win_tab_ptr;    //父窗口的tab控件id指针，无父窗口则置空
    BOOLEAN             is_has_symbol;         //是否包含窗口指示器
}MMI_SLIDE_INIT_T;

typedef void (*SLIDE_DRAW_SYMBOL_BG_FUNC)   (GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T bg_rect);
typedef void (*SLIDE_DRAW_SYMBOL_ITEM_FUNC) (GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T item_rect, BOOLEAN is_focus);

//结构信息
typedef struct 
{
    SLIDE_SYM_WORK_TYPR_E       work_type;                  //类型
    MMI_SLIDE_SYMBOL_TYPE       dis_type;                   //显示类型
    MMI_IMAGE_ID_T              img_id_arr[SYMBOL_IMG_MAX]; //图片数组
    SLIDE_DRAW_SYMBOL_BG_FUNC   OwnerDrawSymBgFunc;         //绘制背景
    SLIDE_DRAW_SYMBOL_ITEM_FUNC OwnerDrawSymItemFunc;       //绘制每一项
}SLIDE_SYM_DIS_INFO_T;

//剪贴区域信息
typedef struct 
{
    GUI_RECT_T       main_rect;
    GUI_RECT_T       top_bg_rect;
    GUI_RECT_T       bottom_bg_rect;
}SLIDE_CLIP_INFO_T;

typedef struct
{
    MMI_SLIDE_TYPE   type;                                 //类型
    MMI_WIN_ID_T     win_array[MMISLIDE_WIN_TOTAL_COUNT];  //窗口id数组
    uint16           win_total_num;                        //窗口总数
    MMI_WIN_ID_T     default_win_id;                       //默认窗口id
    MMI_WIN_ID_T     cur_win_id;                           //当前窗口id
    MMI_CTRL_ID_T    parent_win_tab_id;                    //父窗口tab id
    GUI_BOTH_RECT_T  slide_both_rect;                      //滑动区域组 
    BOOLEAN          is_loop_move;                          //循环滑动
    BOOLEAN          is_sym_on_top;                         //指示器位于顶部
    BOOLEAN          is_valid;                              //激活状态
    GUI_POINT_T      symbol_point;                          //指示器位置
    uint8            sym_remove_timer_id;                   //指示器移除timer
    uint32           out_state;                             //消隐的状态

    SLIDE_WIN_SWITCH_TYPE_E     win_switch_type;            //窗口切换类型
    SLIDE_SYM_DIS_INFO_T        sym_dis_info;              //指示器显示信息    
    SLIDE_CLIP_INFO_T           clip_info;                 //剪切区域相关信息
    MMI_SLIDE_HANDLE handle;                                //句柄
    MMI_SLIDE_HANDLE *handle_ptr;                           //句柄指针
}MMI_SLIDE_INFO_T;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#define MMISLIDE_Creat                          MMIAPISLIDE_Creat
#define MMISLIDE_Release                        MMIAPISLIDE_Release
#define MMISLIDE_Active                         MMIAPISLIDE_Active
#define MMISLIDE_Deactive                       MMIAPISLIDE_Deactive
#define MMISLIDE_ReleaseEx                      MMIAPISLIDE_ReleaseEx
#define MMISLIDE_IsActive                       MMIAPISLIDE_IsActive
#define MMISLIDE_SetLoopSlide                   MMIAPISLIDE_SetLoopSlide
#define MMISLIDE_SetSymbolIsOnTop               MMIAPISLIDE_SetSymbolIsOnTop
#define MMISLIDE_SetSymbolPosition              MMIAPISLIDE_SetSymbolPosition
#define MMISLIDE_SetSymbolWorkType              MMIAPISLIDE_SetSymbolWorkType
#define MMISLIDE_SetSymbolDisplayInfo           MMIAPISLIDE_SetSymbolDisplayInfo
#define MMISLIDE_SetDefaultWin                  MMIAPISLIDE_SetDefaultWin
#define MMISLIDE_SetSetCurWin                   MMIAPISLIDE_SetCurWin
#define MMISLIDE_SetWinSwitchType               MMIAPISLIDE_SetWinSwitchType
#define MMISLIDE_GetInfo                        MMIAPISLIDE_GetInfo
#define MMISLIDE_SwitchToWin                    MMIAPISLIDE_SwitchToWin
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 创建Slide实例
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Creat(MMI_SLIDE_INIT_T *slide_init_ptr, MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : 销毁Slide系统
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Release(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : 激活
//	Global resource dependence: 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Active(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : 去激活
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Deactive(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : 销毁Slide系统
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_ReleaseEx(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_close_window);

/*****************************************************************************/
// 	Description : 是否激活
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_IsActive(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : 设置循环滚动
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetLoopSlide(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_loop);

/*****************************************************************************/
// 	Description : 设置指示器是否在窗口顶部
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolIsOnTop(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_on_top);

/*****************************************************************************/
// 	Description : 设置指示器相对位置
//	Global resource dependence : 
//  Author: Jiaoyou.wu
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolPosition(MMI_SLIDE_HANDLE *handle_ptr, GUI_POINT_T *symbol_point_ptr);

/*****************************************************************************/
// 	Description : 设置指示器的显示类型
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolWorkType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_WORK_TYPR_E work_type);

/*****************************************************************************/
// 	Description : 设置symbol 显示信息
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolDisplayInfo(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_DIS_INFO_T *dis_info_ptr);

/*****************************************************************************/
// 	Description : 设置默认窗口的id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetDefaultWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : 设置当前窗口的id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetCurWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : 设置窗口切换类型
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetWinSwitchType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_WIN_SWITCH_TYPE_E switch_type);

/*****************************************************************************/
// 	Description : 获取Slide Info
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_GetInfo(MMI_SLIDE_HANDLE *handle_ptr, MMI_SLIDE_INFO_T *slide_info_ptr);

/*****************************************************************************/
// 	Description : 切换到某一个窗口
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SwitchToWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_SLIDE_SUPPORT*/
#endif //_MMISLIDE_EXPORT_H


