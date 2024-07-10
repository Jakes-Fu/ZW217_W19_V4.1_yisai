/*! @file guitoolbar.h
@brief 此文件介绍了toolbar控件的对外接口和数据类型
@author James.Zhang
@version 1.0
@date 2011/02/18
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 9/2010         James.Zhang      Create
*******************************************************************************/

/*! @page page_ToolBar ToolBar

(暂无)

*******************************************************************************/

/*! @addtogroup ToolbarGrp ToolBar
@brief Toolbar控件
@details Toolbar控件有些类似menu。Toolbar使用一个长方形的小弹出窗口，排列一些命令选项。
@{
*******************************************************************************/

#ifndef _CTRLTOOLBAR_EXPORT_H_
#define _CTRLTOOLBAR_EXPORT_H_ 

/*--------------------------------------------------------------------------
                          Include Files                               
 --------------------------------------------------------------------------*/

#include "mmk_type.h"
#include "guifont.h"

#include "ctrlbase_export.h"

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                         Macro Declaration                                                                   
 --------------------------------------------------------------------------*/  
/*! @def GUITOOLBAR_INVALID_INDEX
@brief 无效的索引值
*/
#define GUITOOLBAR_INVALID_INDEX            (0xffff)

/*--------------------------------------------------------------------------
                         Data Structures                                  
 --------------------------------------------------------------------------*/
// toolbar handle
typedef struct _toolbar_obj_tag* CTRL_TOOLBAR_HANDLE_T;

/*! @struct GUITOOLBAR_INIT_DATA_T
@brief  初始数据
*/
typedef struct
{
    GUI_BOTH_RECT_T both_rect;/*!< 矩形*/ 
} GUITOOLBAR_INIT_DATA_T;


/*! @enum GUITOOLBAR_ITEM_TYPE_E
@brief  枚举item 类型
*/
typedef enum
{
    GUITOOLBAR_ITEM_NONE, /*!< 无*/ 
    GUITOOLBAR_ITEM_IMAGE_ID,/*!< 图片*/      
    GUITOOLBAR_ITEM_TEXT_ID,/*!< 字符*/    
    GUITOOLBAR_ITEM_BOTH_ID/*!< 图片加字符*/    
}GUITOOLBAR_ITEM_TYPE_E;

/*! @struct GUITOOLBAR_ITEM_T
@brief item 结构
*/
typedef struct
{
    uint32                  user_data;   /*!< 用户自定义数据 */
    GUITOOLBAR_ITEM_TYPE_E  type;/*!< item 类型 */
    MMI_IMAGE_ID_T          image_id;    /*!< 资源图片id */
    MMI_IMAGE_ID_T          image_gray; /*!< 灰化图 */
    MMI_TEXT_ID_T           text_id;    /*!< 资源字符串id */
}GUITOOLBAR_ITEM_T;

/*! @enum GUITOOLBAR_ANIM_DIRECTION_E
@brief   枚举Toolbar的动画方向
*/
typedef enum  
{
    GUITOOLBAR_DIR_INVALID      = 0x00000000,   /*!< 无效*/    
    
    GUITOOLBAR_DIR_UP      = 0x00000001,   /*!< 上*/    
    GUITOOLBAR_DIR_DOWN    = 0x00000002,   /*!< 下*/    
    GUITOOLBAR_DIR_VERTICAL     = 0x0000000f,/*!< 垂直*/    

    GUITOOLBAR_DIR_RIGHT     = 0x00000010,   /*!< 右*/    
    GUITOOLBAR_DIR_LEFT      = 0x00000020,   /*!< 左*/    
    GUITOOLBAR_DIR_HORIZONTAL   = 0x000000f0/*!< 水平*/    
}GUITOOLBAR_ANIM_DIRECTION_E;

/*! @enum  GUITOOLBAR_ANIM_ACTION_E 
@brief   枚举Toolbar的动画动作
*/
typedef enum
{
    GUITOOLBAR_ACT_NONE,         /*!< 无*/
    GUITOOLBAR_ACT_SHRINK,      /*!< 收缩Toolbar*/
    GUITOOLBAR_ACT_EXPAND       /*!< 展开 Toolbar*/
}GUITOOLBAR_ANIM_ACTION_E;

/*! @enum  GUITOOLBAR_SHOW_TYPE_E
@brief   枚举Toolbar的显示类型
*/
typedef enum
{
    GUITOOLBAR_INVALID,     /*!< 无效*/
    GUITOOLBAR_SHRINK,      /*!< 收缩*/
    GUITOOLBAR_EXPAND,      /*!< 展开*/

    GUITOOLBAR_TYPE_MAX   
}GUITOOLBAR_SHOW_TYPE_E;

/*! @typedef TOOLBAR_PRE_EXPAND_FUNC
@brief Toolbar控件扩展功能的函数指针
*/
typedef void (*TOOLBAR_PRE_EXPAND_FUNC)( 
                                        MMI_HANDLE_T        ctrl_handle,
                                        void*               param_ptr
                                        );


/*--------------------------------------------------------------------------
                        Function Declare                             
 --------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create dynamic toolbar
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLTOOLBAR_CreateDynamic(
                                             MMI_WIN_ID_T            win_id,     //in:
                                             MMI_CTRL_ID_T           ctrl_id,    //in:
                                             GUITOOLBAR_INIT_DATA_T   *init_ptr   //in:
                                             );

/***************************************************************************//*!
@brief 设置Toolbar控件的所有item
@author James.Zhang
@param ctrl_handle [in] Toolbar控件句柄
@param item_ptr [in] Toolbar控件的item资源的指针
@param total_num [in] 项目总数
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAllItem(
                                     MMI_HANDLE_T             ctrl_handle,
                                     const GUITOOLBAR_ITEM_T* item_ptr,
                                     uint16                   total_num
                                     );


/***************************************************************************//*!
@brief 获取点击Toolbar控件item的index
@author James.Zhang
@param ctrl_handle [in] Toolbar控件句柄
@return 返回选中item的索引值
@note 
*******************************************************************************/
PUBLIC uint16 CTRLTOOLBAR_GetClickItemIndex(
                                           MMI_HANDLE_T ctrl_handle
                                           );


/***************************************************************************//*!
@brief 获取item的资源指针
@author James.Zhang
@param ctrl_handle [in] Toolbar控件句柄
@param index [in] item索引值
@return 返回item资源的指针
@note 
*******************************************************************************/
PUBLIC const GUITOOLBAR_ITEM_T* CTRLTOOLBAR_GetItemPtr(
                                                      MMI_HANDLE_T ctrl_handle,
                                                      uint16       index
                                                      );


/***************************************************************************//*!
@brief 获取item的用户自定义数据
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param index [in] item索引值
@return 返回用户自定义数据
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTOOLBAR_GetItemUserDate(
                                         MMI_HANDLE_T ctrl_handle,
                                         uint16       index
                                         );


/***************************************************************************//*!
@brief 设置item是否变灰
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param index [in] item索引值
@param is_gray [in] 是否显示变灰(无效)
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetItemGray(
                                   MMI_HANDLE_T ctrl_handle,
                                   uint16       index,
                                   BOOLEAN      is_gray
                                   );


/***************************************************************************//*!
@brief 获取Toolbar控件的显示类型 
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@return 返回Toolbar控件的显示类型
@note 显示类型包括(无效，收缩，展开)
*******************************************************************************/
PUBLIC GUITOOLBAR_SHOW_TYPE_E CTRLTOOLBAR_GetShowType(
                                                      MMI_HANDLE_T ctrl_handle
                                                      );


/***************************************************************************//*!
@brief 设置Toolbar控件的显示类型 
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param show_type  [in] Toolbar控件的显示类型
@note 显示类型包括(无效，收缩，展开)
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetShowType(
                                   MMI_HANDLE_T             ctrl_handle,
                                   GUITOOLBAR_SHOW_TYPE_E   show_type
                                   );


/***************************************************************************//*!
@brief 设置Toolbar控件的是否显示
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param is_visible  [in] Toolbar控件是否显示可见
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetVisible(
                                  MMI_HANDLE_T             ctrl_handle,
                                  BOOLEAN                  is_visible
                                  );


/***************************************************************************//*!
@brief 设置Toolbar控件是否自动收缩
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param is_open  [in] Toolbar控件是否自动收缩
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetAutoShrink(
                                     MMI_HANDLE_T   ctrl_handle,
                                     BOOLEAN        is_open
                                     );


/***************************************************************************//*!
@brief 设置Toolbar控件自动收缩的时间
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param interval  [in] Toolbar控件自动收缩的间隙时间
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAutoShrinkInterval(
                                                MMI_HANDLE_T ctrl_handle,
                                                uint32        interval
                                                );


/***************************************************************************//*!
@brief 设置Toolbar控件的扩展功能
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param func_ptr [in] Toolbar控件的扩展功能的函数指针
@param param_ptr [in] Toolbar控件的扩展功能的参数的指针
@return Toolbar控件的旧的扩展功能的函数指针
@note 
*******************************************************************************/
PUBLIC TOOLBAR_PRE_EXPAND_FUNC CTRLTOOLBAR_SetPreExpandFunc(
                                                           MMI_HANDLE_T                ctrl_handle,
                                                           TOOLBAR_PRE_EXPAND_FUNC     func_ptr,
                                                           void                        *param_ptr
                                                           );


/***************************************************************************//*!
@brief 设置Toolbar控件图标的宽度和高度
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param width_ptr [in] Toolbar控件的图标宽度的指针
@param height_ptr [in] Toolbar控件的图标高度的指针
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_GetIconWidthHeight(
                                             MMI_HANDLE_T    ctrl_handle,
                                             uint16          *width_ptr,
                                             uint16          *height_ptr
                                             );


/***************************************************************************//*!
@brief 设置Toolbar控件的位置
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param rect_ptr  [in] Toolbar控件的图标位置的指针
@param is_update [in] Toolbar控件的图标是否更新
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 
-# 如果选择更新将发送消息立即重绘Toolbar控件。
-# 如果没有选择更新则将延迟重绘。
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetIconRect(
                                      MMI_HANDLE_T              ctrl_handle,
                                      const GUI_RECT_T          *rect_ptr,
                                      BOOLEAN                   is_update
                                      );


/***************************************************************************//*!
@brief 设置Toolbar控件的基准点(即为左下点)
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param pt_ptr  [in] Toolbar控件的左下点坐标的指针
@param is_update [in] Toolbar控件是否更新
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 
-# 如果选择更新将发送消息立即重绘Toolbar控件。
-# 如果没有选择更新则将延迟重绘。
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelBasePt(
                                         MMI_HANDLE_T              ctrl_handle,
                                         const GUI_POINT_T         *pt_ptr,
                                         BOOLEAN                   is_update
                                         );


/***************************************************************************//*!
@brief 设置Toolbar控件的动画方向
@author cheng.luo
@param ctrl_handle [in] Toolbar控件句柄
@param direction [in] 控件动画的方向
@retval TRUE 设置成功
@retval FALSE 设置失败
@return 返回是否设置成功
@note 控件动画的方向(包括上下左右，水平，垂直)
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelPopForm(
                                          MMI_HANDLE_T                   ctrl_handle,
                                          GUITOOLBAR_ANIM_DIRECTION_E    direction
                                          );

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
