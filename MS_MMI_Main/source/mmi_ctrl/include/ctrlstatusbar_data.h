/*! @file ctrlstatusbar_data.h
@brief 此文件介绍了statusbar_data控件的对外接口和数据类型
@author xiaoqing.lu
@version 1.0
@date 12/23/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 12/23/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(暂无)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_data模块
@details 该文件不对外开放，所有app都应该调用guiwin.h中的封装接口
@{
*******************************************************************************/

#ifndef _CTRL_STATUSBAR_DATA_H_
#define _CTRL_STATUSBAR_DATA_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guicommon.h"
#include "guifont.h"
#include "mmitheme_statusbar.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/*! @typedef GUISTBDATA_ITEM_STATE_T
@brief 状态栏控件的item状态
*/
typedef uint32 GUISTBDATA_ITEM_STATE_T;



/*! @typedef GUISTBDATA_APP_HANDLE_FUNC
@brief Item 处理的回调，例如可以通过此接口打开对应模块接口等
*/
typedef BOOLEAN (*GUISTBDATA_APP_HANDLE_FUNC) (void);



/*! @enum GUISTBDATA_ICON_STYLE_E
@brief item显示风格
*/
typedef enum
{
    GUISTBDATA_ICON_NONE = 0,/*!< 无*/
    GUISTBDATA_ICON_NORMAL,/*!< 正常，只显示资源ID*/
    GUISTBDATA_ICON_ANIM,/*!< 显示动画，如充电图标时*/
    GUISTBDATA_ICON_WRE,/*!< wre后台AP图标,从文件中加载图片*///add for wre_background by heng.xiao
    GUISTBDATA_ICON_MAX  /*!< 系统保留*/
} GUISTBDATA_ICON_STYLE_E;



/*! @struct GUISTBDATA_TEXT_T
@brief  文本信息
*/
typedef struct
{
    BOOLEAN         is_display; /*!< 是否显示*/
    uint8           font_space; /*!< 字间距*/
    GUI_FONT_T      font_type;  /*!< 字体*/
    GUI_COLOR_T     font_color; /*!< 文字颜色*/
    GUI_ALIGN_E     align;			/*!< 对齐方式*/
    wchar           wstr_ptr[MMISTATUSBAR_ITEM_TEXT_LEN];  /*!< 文字buffer*/
    uint32          wstr_len;   /*!< 文字长度*/
} GUISTBDATA_TEXT_T;



/*! @struct GUISTBDATA_ICON_T
@brief item显示的icon数据信息
*/
typedef struct
{
    BOOLEAN                     is_display; /*!< 是否显示*/
    uint32                      icon_num;   /*!< icon数，最大值为MMISTATUSBAR_ITEM_ICON_FRAME*/
    uint32                      icon_array[MMISTATUSBAR_ITEM_ICON_FRAME]; /*!< icon数组*/
    GUISTBDATA_ICON_STYLE_E     icon_type;  /*!< icon显示风格，是不是动态显示？*/
    BOOLEAN                     is_color_exchange;  /*!< 资源图片是否使用颜色替换*/
    uint32                      color;              /*!< 颜色替换值*/
} GUISTBDATA_ICON_T;



/*! @struct GUISTATUSBAR_OWNER_DRAW_T
@brief 控件自绘回调函数传出信息
*/
typedef struct
{
    GUI_RECT_T                   display_rect;   /*!<显示区域*/
    MMI_HANDLE_T                 ctrl_handle;    /*!<控件handle*/
    GUI_LCD_DEV_INFO             lcd_dev;        /*!<lcd信息*/
} GUISTBDATA_OWNER_DRAW_T;



/*! @typedef GUISTATUSBAR_OWNER_DRAW_FUNC
@brief 控件自绘回调函数,携带参数
*/
typedef void (*GUISTATUSBAR_OWNER_DRAW_FUNC) (
    GUISTBDATA_OWNER_DRAW_T *owner_draw_ptr
);



/*! @struct GUISTBDATA_ITEM_DATA_T
@brief item 数据信息
*/
typedef struct
{
    uint32                          cur_frame;          /*!< icon当前帧*/
    GUI_RECT_T                      item_rect;          /*!< item区域*/
    GUISTBDATA_TEXT_T               text_info;          /*!< 文字信息*/
    GUISTBDATA_ICON_T               icon_info;          /*!< icon信息*/
    GUI_BG_T                        bg_info;            /*!< bg信息*/
    GUISTBDATA_ITEM_STATE_T         item_state;         /*!< item状态*/
    GUISTBDATA_APP_HANDLE_FUNC      app_handle_func;    /*!< item对应的可执行功能接口*/
    BOOLEAN 					    is_handle_callback; /*!< 是否执行上面的功能接口*/
    GUISTATUSBAR_OWNER_DRAW_FUNC    owner_draw_func;      /*!< owner draw function*/
} GUISTBDATA_ITEM_DATA_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief initial the item data of status bar
@author Xiaoqing.Lu
@param item_max_num item [in] 最大数，用户设置
@retval true/false
@return 初始化成功或失败
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_InitData (
    uint32 item_max_num
);


/***************************************************************************//*!
@brief Append item
@author Xiaoqing.Lu
@param item_data_ptr [in] item数据指针
@retval true/false
@return 成功或失败
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_AppendItemData (
    GUISTBDATA_ITEM_DATA_T *item_data_ptr
);


/***************************************************************************//*!
@brief 设置item是否可见
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_visible [in] 是否可见
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemVisible (
    uint32   item_index,
    BOOLEAN  is_visible
);


/***************************************************************************//*!
@brief 判断item是否可见
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemVisible (
    uint32   item_index
);

/***************************************************************************//*!
@brief 设置item是否有效
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_visible [in] 是否有效
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemValid (
    uint32   item_index,
    BOOLEAN  is_valid
);

/***************************************************************************//*!
@brief 获取item是否有效
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemValid (
    uint32   item_index
);

/***************************************************************************//*!
@brief 设置disable状态
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_disable [in] 是否disable
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemDisable (
    uint32   item_index,
    BOOLEAN  is_disable
);


/***************************************************************************//*!
@brief 判断disable
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemDisable (
    uint32   item_index
);


/***************************************************************************//*!
@brief set item handle function
@author xiaoqing.lu
@param item_index [in] 索引号
@param app_handle_func [in] 函数指针
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFunc (
    uint32   item_index,
    GUISTBDATA_APP_HANDLE_FUNC app_handle_func
);


/***************************************************************************//*!
@brief set item handle function is valid or not
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_valid [in] 是否可执行
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFuncValid (
    uint32  item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief 判断是否可执行
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_ItemIsHandleFuncValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief 设置item icon
@author xiaoqing.lu
@param item_index [in] 索引号
@param icon_info_ptr [in] icon数据
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIcon (
    uint32            item_index,
    const GUISTBDATA_ICON_T *icon_info_ptr
);


/***************************************************************************//*!
@brief 获得icon数据
@author xiaoqing.lu
@param item_index [in] 索引号
@param icon_info_ptr [in/out] icon数据
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIcon (
    uint32            item_index,
    GUISTBDATA_ICON_T *icon_info_ptr
);


/***************************************************************************//*!
@brief 设置当前item frame
@author xiaoqing.lu
@param item_index [in] 索引号
@param cur_frame [in] 当前帧
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconCurFrame (
    uint32 item_index,
    uint32 cur_frame
);


/***************************************************************************//*!
@brief 获取当前帧
@author xiaoqing.lu
@param item_index [in] 索引号
@param cur_frame_ptr [in/out] 当前帧
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconCurFrame (
    uint32 item_index,
    uint32 *cur_frame_ptr
);


/***************************************************************************//*!
@brief 计算下一帧
@author xiaoqing.lu
@param item_index [in] 索引号
@param next_frame_ptr [in/out] 下一帧
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconNextFrame (
    uint32 item_index,
    uint32 *next_frame_ptr
);


/***************************************************************************//*!
@brief 设置icon是否有效
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_valid [in] 是否有效
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconValid (
    uint32 item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief 判断icon是否有效
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemIconValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief 设置显示类型，是否动画？
@author xiaoqing.lu
@param item_index [in] 索引号
@param icon_style icon [in] 显示类型，是否动画？
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E icon_style
);


/***************************************************************************//*!
@brief 获得显示类型
@author xiaoqing.lu
@param item_index [in] 索引号
@param icon_style_ptr icon [in/out] 显示类型，是否动画？
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E *icon_style_ptr
);


/***************************************************************************//*!
@brief 设置icon id
@author xiaoqing.lu
@param item_index [in] 索引号
@param icon_id [in] icon资源ID
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconId (
    uint32         item_index,
    MMI_IMAGE_ID_T icon_id
);


/***************************************************************************//*!
@brief 设置文字
@author xiaoqing.lu
@param item_index [in] 索引号
@param text_info_ptr [in] 文字信息
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemText (
    uint32            item_index,
    const GUISTBDATA_TEXT_T *text_info_ptr
);


/***************************************************************************//*!
@brief 获得文字信息
@author xiaoqing.lu
@param item_index [in] 索引号
@param text_info_ptr [in/out] 文字信息
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemText (
    uint32            item_index,
    GUISTBDATA_TEXT_T *text_info_ptr
);


/***************************************************************************//*!
@brief 设置text是否显示
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_valid [in] valid or not
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemTextValid (
    uint32 item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief 判断text "valid or not?"
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemTextValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief 设置item rect
@author xiaoqing.lu
@param item_index [in] 索引号
@param rect [in] 区域
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRect (
    uint32     item_index,
    GUI_RECT_T rect
);


/***************************************************************************//*!
@brief 获得item区域
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC GUI_RECT_T CTRLSTBDATA_GetItemRect (
    uint32     item_index
);


/***************************************************************************//*!
@brief 获得total item
@author xiaoqing.lu
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC uint32 CTRLSTBDATA_GetItemTotalNum (
    void
);


/***************************************************************************//*!
@brief 获得item数据
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC GUISTBDATA_ITEM_DATA_T *CTRLSTBDATA_GetItemData (
    uint32 item_index
);


/***************************************************************************//*!
@brief set item is displayed by right side
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_right_align [in] 是否靠右显示
@retval true/false
@return true/false
@note 内部接口：如果不是靠右显示，则肯定是靠左显示，没有第三种情况
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRightAlign (
    uint32   item_index,
    BOOLEAN  is_right_align
);


/***************************************************************************//*!
@brief set istyle item is displayed by which side
@author xiyuan.ma
@param item_index [in] 索引号
@param align_type [in] 选择item在iphone style下，是靠右显示、居中显示还是靠左显示
@retval true/false
@return true/false
@note 内部接口：iphone风格下，item有三种显示位置可以选择，靠左、靠右、居中
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemCenterAlign (
    uint32   item_index,
    BOOLEAN  is_center_align
);


/***************************************************************************//*!
@brief judge item is displayed by right side or not
@author xiaoqing.lu
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口：如果不是靠右显示，则肯定是靠左显示，没有第三种情况
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemRightAlign (
    uint32   item_index
);


/***************************************************************************//*!
@brief judge item is displayed by right side、left side or center
@authorxiyuan.ma
@param item_index [in] 索引号
@retval true/false
@return true/false
@note 内部接口：iphone风格的item居中
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemCenterAlign (
    uint32   item_index
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTABATA_SetItemBgInter (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
);


/*****************************************************************************/
//  Description : set STATUSBAR owner draw callback function
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTABATA_SetOwnerDraw (
    uint32                          item_index,
    GUISTATUSBAR_OWNER_DRAW_FUNC    func
);


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _CTRL_STATUSBAR_DATA_H_


/*@}*/
