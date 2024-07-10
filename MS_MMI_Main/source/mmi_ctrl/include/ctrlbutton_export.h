/***************************************************************************
** File Name:      ctrlbutton_export.h                                     *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBUTTON_EXPORT_H_
#define _CTRLBUTTON_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guictrl.h"
#include "guifont.h"

#include "ctrlbase_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/*! @enum GUIBUTTON_TYPE_E
@brief button类型
*/
typedef enum
{
    GUIBUTTON_TYPE_NORMAL,  /*!<默认*/
    GUIBUTTON_TYPE_RADIO,   /*!<单选*/
    GUIBUTTON_TYPE_CHECK,   /*!<多选*/
    GUIBUTTON_TYPE_MAX,     /*!<保留位*/
} GUIBUTTON_TYPE_E;

/*! @enum GUIBUTTON_STYLE_E
@brief button图文时的显示风格
*/
typedef enum
{
    GUIBUTTON_STYLE_DEFAULT,    /*!<默认，图文重叠*/
    GUIBUTTON_STYLE_UIDT,       /*!<上图下文*/
    GUIBUTTON_STYLE_LIRTM,       /*!<左图右文(图文居中)*/
    GUIBUTTON_STYLE_LIRTL,       /*!<左图右文(图文居左)*/
    GUIBUTTON_STYLE_MAX
} GUIBUTTON_STYLE_E;

/*! @enum GUIBUTTON_OWNDRAW_STATE_E
@brief button自绘时的状态
*/
typedef enum
{
    GUIBUTTON_OWNDRAW_NONE,    /*!<none*/
    GUIBUTTON_OWNDRAW_TP_DOWN, /*!<tp dow*/
    GUIBUTTON_OWNDRAW_TP_MOVE, /*!<tp move*/
    GUIBUTTON_OWNDRAW_TP_UP,   /*!<tp up*/
    GUIBUTTON_OWNDRAW_MAX
} GUIBUTTON_OWNDRAW_STATE_E;

/*! @enum GUIBUTTON_ANIM_TYPE_E
@brief button的按键特效类型
*/
typedef enum
{
    GUIBUTTON_NONE_ANIM,            /*!<无动画特效*/
    GUIBUTTON_RUNSHEEN_ANIM,    /*!<runsheen 动画特效*/
    GUIBUTTON_POPBLINK_ANIM,     /*!<popblink 动画特效*/
    GUIBUTTON_LOAD_MAX               /*!<保留位*/
} GUIBUTTON_EFFECT_TYPE_E;

/*! @typedef GUIBUTTON_CALLBACK_FUNC
@brief 控件回调函数,不携带参数
*/
typedef MMI_RESULT_E (*GUIBUTTON_CALLBACK_FUNC)(void);

/*! @typedef GUIBUTTON_CALLBACK_FUNC_EXT
@brief 控件回调函数,不携带参数
*/
typedef MMI_RESULT_E (*GUIBUTTON_CALLBACK_FUNC_EXT)(MMI_HANDLE_T ctrl_handle);

/*! @struct GUIBUTTON_INIT_DATA_T
@brief button控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖区域*/
    GUI_BG_T            bg;         /*!<松开状态背景*/
} GUIBUTTON_INIT_DATA_T;

/*! @struct GUIBUTTON_OWNER_DRAW_T
@brief 控件自绘回调函数传出信息
*/
typedef struct 
{
    int16               display_x;      /*!<背景显示x坐标*/
    int16               display_y;      /*!<背景显示y坐标*/
    int16               fg_display_x;   /*!<前景显示x坐标*/
    int16               fg_display_y;   /*!<前景显示y坐标*/
    GUI_RECT_T          display_rect;   /*!<显示区域*/
    MMI_HANDLE_T        ctrl_handle;    /*!<控件handle*/
    GUIBUTTON_OWNDRAW_STATE_E draw_state;
    GUI_LCD_DEV_INFO    lcd_dev;        /*!<lcd信息*/
} GUIBUTTON_OWNER_DRAW_T;

/*! @typedef GUIBUTTON_OWNER_DRAW_FUNC
@brief 控件自绘回调函数,携带参数
*/
typedef void (*GUIBUTTON_OWNER_DRAW_FUNC)(
                                          GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr
                                          );

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 动态创建button控件
@author Jassmine
@param win_id   [in] 窗口id
@param ctrl_id  [in] 控件id
@param init_ptr [in] 初始化数据
@return 控件handle
@note 
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBUTTON_CreateDynamic(
                                            MMI_WIN_ID_T            win_id,     //in:
                                            MMI_CTRL_ID_T           ctrl_id,    //in:
                                            GUIBUTTON_INIT_DATA_T   *init_ptr   //in:
                                            );

/***************************************************************************//*!
@brief 设置button控件显示风格
@author hua.fang
@param ctrl_id [in] 控件id
@param style  [in] 显示风格
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetStyle(
                                  MMI_CTRL_ID_T         ctrl_id,        //in
                                  GUIBUTTON_STYLE_E     style           //in:
                                  );

/***************************************************************************//*!
@brief 设置button控件背景,即松开状态背景信息
@author Jassmine
@param ctrl_id [in] 控件id
@param bg_ptr  [in] 背景,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *bg_ptr     //in:may PNULL
                               );

/***************************************************************************//*!
@brief 设置button控件前景,即松开状态前景信息
@author Jassmine
@param ctrl_id [in] 控件id
@param fg_ptr  [in] 前景信息
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *fg_ptr     //in:may PNULL
                               );

/***************************************************************************//*!
@brief 设置button控件按下背景信息
@author Jassmine
@param ctrl_id [in] 控件id
@param bg_ptr  [in] 按下背景信息,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedBg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *bg_ptr     //in:may PNULL
                                      );

/***************************************************************************//*!
@brief 设置button控件按下前景信息
@author Jassmine
@param ctrl_id [in] 控件id
@param fg_ptr  [in] 按下前景信息,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedFg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *fg_ptr     //in:may PNULL
                                      );

/***************************************************************************//*!
@brief 设置button控件是否灰化,以及灰化背景信息
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_gray   [in] 是否灰化
@param gray_ptr  [in] 灰化背景信息
@param is_update [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayed(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   BOOLEAN          is_gray,    //in:
                                   GUI_BG_T         *gray_ptr,  //in:
                                   BOOLEAN          is_update   //in:
                                   );

/***************************************************************************//*!
@brief 设置button控件是否灰化,以及灰化前景信息
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_gray   [in] 是否灰化
@param gray_ptr  [in] 灰化前景信息
@param is_update [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayedFg(
                                     MMI_CTRL_ID_T      ctrl_id,    //in:
                                     BOOLEAN            is_gray,    //in:
                                     GUI_BG_T           *gray_ptr,  //in:
                                     BOOLEAN            is_update   //in:
                                     );

/***************************************************************************//*!
@brief 设置button控件文本id
@author Jassmine
@param ctrl_id [in] 控件id
@param text_id [in] 文本id
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextId(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_TEXT_ID_T    text_id     //in
                                   );

/***************************************************************************//*!
@brief 设置button控件文本信息
@author Jassmine
@param ctrl_id [in] 控件id
@param str_ptr [in] 文本内容
@param str_len [in] 文本长度
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetText(
                                 MMI_CTRL_ID_T  ctrl_id,    //in:
                                 wchar          *str_ptr,   //in:
                                 uint16         str_len     //in:
                                 );

/***************************************************************************//*!
@brief 设置button控件文本排列方式
@author Jassmine
@param ctrl_id [in] 控件id
@param align   [in] 排列方式
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextAlign(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      GUI_ALIGN_E       align   //in
                                      );

/*****************************************************************************/
//  Description : set button font color
//  Parameter: [In] ctrl_id : button control id
//             [In] font_color : button font color
//             [Out] None
//             [Return] result : TRUE:secceed , FALSE : failed
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFontColor(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      CAF_COLOR_T       font_color//in
                                      );

/***************************************************************************//*!
@brief 设置button控件回调函数,不携带参数
@author Jassmine
@param ctrl_id [in] 控件id
@param func    [in] 回调函数
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFunc(
                                         MMI_CTRL_ID_T              ctrl_id,
                                         GUIBUTTON_CALLBACK_FUNC    func
                                         );

/***************************************************************************//*!
@brief 设置button控件回调函数,不携带参数
@author Jassmine
@param ctrl_id [in] 控件id
@param func    [in] 回调函数
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFuncExt(
                                            MMI_CTRL_ID_T              ctrl_id,
                                            GUIBUTTON_CALLBACK_FUNC_EXT    func
                                            );

/***************************************************************************//*!
@brief 设置button控件是否处理tp long消息
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_handle [in] 是否处理
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetHandleLong(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_handle
                                       );

/***************************************************************************//*!
@brief 设置button控件是否半透显示背景
@author Jassmine
@param ctrl_id        [in] 控件id
@param is_transparent [in] 是否半透显示
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTransparent(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN         is_transparent
                                        );

/***************************************************************************//*!
@brief 设置button控件是否使用光晕效果
@author Jassmine
@param ctrl_id     [in] 控件id
@param is_runsheen [in] 是否使用光晕效果
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRunSheen(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_runsheen
                                     );

/***************************************************************************//*!
@brief 设置button控件区域
@author Jassmine
@param ctrl_id  [in] 控件id
@param rect_ptr [in] 区域
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr   //in:
                                );

/***************************************************************************//*!
@brief 设置button控件是否允许显示边框
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_permit [in] 是否显示边框
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_PermitBorder(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_permit
                                      );

/***************************************************************************//*!
@brief 设置button控件字体
@author Jassmine
@param ctrl_id  [in] 控件id
@param font_ptr [in] 字体信息
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFont(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 GUI_FONT_ALL_T     *font_ptr   //in:
                                 );

/***************************************************************************//*!
@brief 设置button控件是否可见
@author Jassmine
@param ctrl_id    [in] 控件id
@param is_visible [in] 是否可见
@param is_update  [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetVisible(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_visible,
                                    BOOLEAN         is_update
                                    );

/***************************************************************************//*!
@brief 设置button控件自绘回调函数
@author Jassmine
@param ctrl_id [in] 控件id
@param func    [in] 自绘回调函数
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC void CTRLBUTTON_SetOwnerDraw(
                                   MMI_CTRL_ID_T                ctrl_id,
                                   GUIBUTTON_OWNER_DRAW_FUNC    func
                                   );

/***************************************************************************//*!
@brief 刷新button控件
@author Jassmine
@param ctrl_id [in] 控件id
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_Update(
                                MMI_CTRL_ID_T   ctrl_id
                                );

/*****************************************************************************/
//  Description : update button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_UpdateText(
                                    MMI_CTRL_ID_T   ctrl_id
                                    );

/***************************************************************************//*!
@brief 是否禁止自动刷新
@author hua.fang
@param ctrl_id [in] 控件id
@param is_forbid_paint [in] 是否禁止自动刷新
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLBUTTON_SetForbidPaint(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_forbid_paint
                                     );

/*****************************************************************************//*!
@brief  设置button的border
@author Sam.hua
@param  ctrl_id    [in] 控件id
@param  border_ptr [in] border的参数指针
@param  is_update  [in] 是否刷新
@return TRUE，设置成功 FALSE，设置失败
@note
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBorder(
                                   MMI_CTRL_ID_T ctrl_id,    //in:
                                   GUI_BORDER_T  *border_ptr,
                                   BOOLEAN       is_update
                                   );

/*****************************************************************************//*!
@brief  设置button pop动画的相关参数
@author xiyuan.ma
@param  ctrl_id      [in] 控件id
@param  pop_info_ptr [in] pop动画的参数指针
@return TRUE，设置成功 FALSE，设置失败
@note   button的pop动画已有默认值，用户可以通过此接口来变更这些默认值，达到最佳效果
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPopAnimInfo(
                                        MMI_CTRL_ID_T ctrl_id,
                                        int32*        total_frame_ptr,    
                                        int32*        radius_ptr,         
                                        uint32*       filled_color_ptr 
                                        );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBUTTON_EXPORT_H_
