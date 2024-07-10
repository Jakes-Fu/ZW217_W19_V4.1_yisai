/*==============================================================================
File Name: mmiim_im_switching.h
Author: Haiwu.Chen
Date: 
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 参照周连祥文件mmiim_sp_setting.c的实现形式
在多国语下，力争实现各种输入法的切换功能
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/26   Haiwu.Chen      Create.
==============================================================================*/

#ifndef _MMIIM_IM_SWITHING_H_
#define _MMIIM_IM_SWITHING_H_

#include "mmiim_base.h"
//#include "mmiset.h"
#include "mmiset_export.h"

//各种输入法对应的显示语言
/*
method 唯一的值，用来标志输入法的，与配置表中定义的值一致
kernel 这个值也肯定是要有的，否则无法启动输入
is_display 表示是否显示出来以供用户选择，在特定应用环境下有些输入法并不被允许使用，如输发件人电话号码时
*/
typedef struct
{
    GUIIM_METHOD_T          method;
    //wchar const *         method_name_str;
    MMI_TEXT_ID_T           txt_id;
    BOOLEAN                 is_display;
}MMIIM_IM_NAME_T;

//建一个索引表，让菜单的每行与GUIIM_METHOD_T的值对应起来，
//数组的赋值在菜单的初始化时一并构造
typedef struct 
{
    uint16                  idx;
    GUIIM_METHOD_T          method;     
}MMIIM_IM_MENU_T;


//const define
//部分值来自周连详，窗口相关相关的一些设置，只是一个参考，待需求确认
enum
{
    
    MMIIM_INPUT_METHOD_COMMON_COUNT = 4,            //通用输入法切换的最大值

    //以下参数来自周连祥，弃用
    MMIIM_SP_SETTING_COLOR_BG = 0x2104,
    MMIIM_SP_SETTING_COLOR_BORDER = 0xc618,
    MMIIM_SP_SETTING_COLOR_FONT = 0xe71c,
    MMIIM_SP_SETTING_COLOR_HOT = 0xfcc0,
    MMIIM_SP_SETTING_FONT = SONG_FONT_14,
    //item min width
    MMIIM_SP_SETTING_ITEM_WIDTH = 48,
    MMIIM_SP_SETTING_ITEM_MARGIN = 20,
    //item default height
    MMIIM_SP_SETTING_ITEM_HEIGHT = 24,
};

//popmenu 弹出的起点方向
typedef enum
{
    MMIIM_EP_TOP_LEFT     = 0,
    MMIIM_EP_TOP_RIGHT    = 1,
    MMIIM_EP_BOTTOM_LEFT  = 2,
    MMIIM_EP_BOTTOM_RIGHT = 3
} MMIIM_POPMENU_EP_E;


//win data, 切换输入法窗口相关的数据
typedef struct
{
    GUIIM_INIT_KERNEL_T*    im_kernel_ptr;          //输入法传过来的
    GUI_BOTH_RECT_T         disp_rect;              //显示区域，应用调用时，传进来
    MMIIM_POPMENU_EP_E      effective_point;        //popmenu弹出的起点
    
    void*                   user_data_ptr;          //窗口的特有数据，预留

    GUIIM_LANGUAGE_T        last_lang;
    uint16                  item_hot;
    uint16                  item_counts;
    MMIIM_IM_NAME_T*        item_ptr;

    MMIIM_IM_MENU_T*        menu_table_ptr;         //菜单对应的输入法索引表

    //在此扩展
    GUIIM_METHOD_T          default_method;         //用户切换输入语言，默认第一项为当前输入法

} MMIIM_SWITCHING_IM_WIN_DATA_T;


//win data, 设置当前的输入语言
typedef struct
{
    GUIIM_INIT_KERNEL_T*    im_kernel_ptr;          //输入法传过来的
    GUI_BOTH_RECT_T         disp_rect;              //显示区域，应用调用时，传进来
    MMIIM_POPMENU_EP_E      effective_point;        //popmenu弹出的起点
    
    uint16                  hot_lang;               //进入后默认焦点语言
    
    void*                   user_data_ptr;          //窗口的特有数据，预留

    //在此扩展

} MMIIM_SET_INPUT_LANG_WIN_DATA_T;




/*****************************************************************************/
//  Description : 添加输入法选择的radio list的item
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_AppendIMListItem(
                                   MMI_CTRL_ID_T ctrl_id, 
                                   MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr
                                   );

/*****************************************************************************/
//  Description : 完成窗口创建之前的准备
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 要显示的区域
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SwitchingCreateWin(
                                             GUIIM_INIT_KERNEL_T* im_kernel_ptr, 
                                             GUI_BOTH_RECT_T disp_rect,
                                             MMIIM_POPMENU_EP_E effective_point
                                             );

/*****************************************************************************/
//  Description : 完成设置当前输入语言窗口的创建
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SetLangCreateWin(
                                           GUIIM_INIT_KERNEL_T* im_kernel_ptr,
                                           GUI_BOTH_RECT_T disp_rect,
                                           MMIIM_POPMENU_EP_E effective_point
                                           );

/*****************************************************************************/
//  Description : 由set中的语言序，得到控件中的语言值
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetGUILangBySetLangIdx(
                                              MMISET_LANGUAGE_TYPE_E set_lang_index
                                              );

/*****************************************************************************/
//  Description : 由控件中的语言值，得到set中的语言序
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetSetLangByGUILangIdx(
                                              GUIIM_LANGUAGE_T gui_lang_index
                                              );

#endif /*_MMIIM_IM_SWITHING_H_*/


