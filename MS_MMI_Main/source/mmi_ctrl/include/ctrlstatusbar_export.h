/*****************************************************************************
** File Name:      ctrlstatusbar_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma              Creat
******************************************************************************/

#ifndef _CTRLSTATUSBAR_EXPORT_H_
#define _CTRLSTATUSBAR_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"


#include "guicommon.h"
#include "guictrl.h"
#include "mmitheme_statusbar.h"
#include "mmitheme_pos.h"
#include "ui_layer.h"
#include "caf.h"
#include "mmk_type.h"
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*! @typedef GUISTATUSBAR_STATE_T
@brief 控件的状态
*/
typedef uint32 GUISTATUSBAR_STATE_T;


/*! @def GUISTATUSBAR_STATE_INVALID
@brief 控件无效
*/
#define GUISTATUSBAR_STATE_INVALID 0x00000001 // 控件无效


// 控件是否支持回调, 状态栏每个item也有一个单独控制item本身是否
// 执行回调函数的状态，首先执行该状态。即如果该状态为false的时候，
// 即使item单独设置有效，也不会执行，item单独设置的函数为GUIWIN_SetItemHandleFuncValid
// 该状态默认为FALSE
/*! @def GUISTATUSBAR_STATE_EVENT
@brief 控件是否支持回调, 状态栏每个item也有一个单独控制item本身是否执行回调函数的状态，首先执行该状态。
即如果该状态为false的时候，即使item单独设置有效，也不会执行，item单独设置的函数为GUIWIN_SetItemHandleFuncValid该状态默认为FALSE
*/
#define GUISTATUSBAR_STATE_EVENT 0x00000002

#define GUISTATUSBAR_STATE_USE_LAYER 0x00000008 // use stb layer

#define GUISTATUSBAR_STATE_SCROLLMSG_PAUSE 0x00000010 // pause msg

#define GUISTATUSBAR_STATE_CUSTOMER_LAYER 0x00000004 // customer stb layer

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @enum  GUISTATUSBAR_TYPE_E
@brief  item show style, 目前只支持水平显示，以后扩展
*/
typedef enum
{
    GUISTATUSBAR_TYPE_NONE = 0, /*!< 无*/
    GUISTATUSBAR_TYPE_HOR,  		/*!< 水平显示*/
    GUISTATUSBAR_TYPE_MAX				/*!< 系统保留*/
} GUISTATUSBAR_TYPE_E;


/*! @struct GUISTATUSBAR_INIT_DATA_T
@brief 状态栏初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T           both_rect;   /*!< 控件的横竖屏区域*/
} GUISTATUSBAR_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                                                                        *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 创建statusbar
@author xiyuan.ma
@param param_ptr  [in] 初始化参数
@param ictrl_ptr  [in] CAF控件对象
@return 控件handle
@note
*******************************************************************************/
// PUBLIC CTRL_STATUSBAR_HANDLE_T CTRLSTATUSBAR_Create (
//     void *param_ptr,
//     IGUICTRL_T *ictrl_ptr
// );


/***************************************************************************//*!
@brief 销毁statusbar
@author xiyuan.ma
@param label_handle   [in] 控件句柄
@return 是否成功
@note
*******************************************************************************/
// PUBLIC BOOLEAN CTRLSTATUSBAR_Destroy (
//     CTRL_STATUSBAR_HANDLE_T   statusbar_handle
// );


/***************************************************************************//*!
@brief 动态创建状态栏控件
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@retval true/false
@return 创建成功或失败
@note 使用该函数，动态创建时，系统会使用统一的控件id：MMICOMMON_STATUSBAR_CTRL_ID。控件的区域使用默认区域（窗口顶部）
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CreateCtrlDyn (
    MMI_HANDLE_T win_handle
);


/***************************************************************************//*!
@brief 设置控件的背景
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param image_id [in] 图片资源ID
@retval void
@return void
@note 如果不设置，将使用默认背景
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgImageId (
    MMI_HANDLE_T win_handle,
    MMI_IMAGE_ID_T image_id
);


/***************************************************************************//*!
@brief
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param bg_info [in] 图片数据信息
@retval void
@return void
@note 如果不设置，将使用默认背景
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
);


/***************************************************************************//*!
@brief post messages of status bar
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param is_full_paint [in] 是否刷新整个窗口
@retval true/false
@return 成功或失败
@note 当控件不显示自己的背景时，需要刷新窗口的背景（is_full_paint=true）
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLSTATUSBAR_UpdateCtrl (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_full_paint
);


/***************************************************************************//*!
@brief 设置是否显示控件自身背景
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param is_desplay_bg [in] 是否显示控件自身背景
@retval void
@return void
@note 当控件不显示自身背景时，刷新控件将刷新整个窗口
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgIsDesplay (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_desplay_bg
);


/*****************************************************************************//*!
@brief      设置是否清空状态栏的用户背景
@author   xiyuan.ma
@param   win_handle              [in] 控件的父窗口handle或id
@param   clear_customer_bg   [in]
@retval    void
@return    void
@note      TRUE时,刷新背景清空用户背景
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_ClearCustomerBg (
    MMI_HANDLE_T win_handle,
    BOOLEAN clear_customer_bg
);


/***************************************************************************//*!
@brief  动态创建状态栏控件，用户可以自定义控件的区域
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param rect_ptr [in] 控件横竖屏区域
@retval true/false
@return 成功或失败
@note 该函数与 \link CTRLSTATUSBAR_CustomCreate() \endlink 类似，区别在于用户可自定义区域
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CustomCreate (
    MMI_HANDLE_T win_handle,
    GUI_BOTH_RECT_T *rect_ptr
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetCommonBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
);


/***************************************************************************//*!
@brief 设置控件状态
@author xiaoqing.lu
@param win_handle [in] 控件的父窗口handle或id
@param ctrl_state [in] 控件状态
@param is_set true [in] 表示设置，false表示取消
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state,
    BOOLEAN is_set
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_GetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state
);


/*****************************************************************************/
//  Description : 设置item的bg
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_SetItemBg (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
);


/*****************************************************************************/
//  Description : 用户获取statusbar的层信息
//  Global resource dependence :
//  Author:xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_GetCtrlLayer (
    MMI_HANDLE_T win_handle,
    GUI_LCD_DEV_INFO *dev_info_ptr
);


#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetDropDownState (
    MMI_HANDLE_T win_handle,
    BOOLEAN enable
);


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsDropDown (
    MMI_HANDLE_T win_handle
);


/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetPressed (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_true
);


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsPressed (
    MMI_HANDLE_T win_handle
);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
