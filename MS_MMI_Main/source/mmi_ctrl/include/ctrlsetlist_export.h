/*****************************************************************************
** File Name:      ctrlsetlist_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLSETLIST_EXPORT_H_
#define _CTRLSETLIST_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "ctrlsetlist_export.h"
#include "guicommon.h"
#include "guictrl.h"
#include "mmi_link.h"
#include "mmitheme_setlist.h"

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


/*! @def GUISETLIST_INVALID_ITEM_INDEX
@brief 无效的item索引号
*/
#define GUISETLIST_INVALID_ITEM_INDEX  0xFFFFFFFF // 无效的item索引号

/*! @typedef GUISETLIST_ITEM_STATE_T
@brief 定义item状态
*/
typedef uint32 GUISETLIST_ITEM_STATE_T;
/*! @def GUISETLIST_ITEM_STATE_NONE
@brief null state
*/
#define GUISETLIST_ITEM_STATE_NONE          0x00000000 // null state
/*! @def GUISETLIST_ITEM_STATE_TEXT
@brief item显示文本
*/
#define GUISETLIST_ITEM_STATE_TEXT          0x00000001 // item显示文本
/*! @def GUISETLIST_ITEM_STATE_IMAGE
@brief item显示图片
*/
#define GUISETLIST_ITEM_STATE_IMAGE         0x00000002 // item显示图片
/*! @def GUISETLIST_ITEM_STATE_INVISIBLE
@brief item不可见,默认可见
*/
#define GUISETLIST_ITEM_STATE_INVISIBLE     0x00000004 // item不可见,默认可见

/*! @typedef GUISETLIST_STATE_T
@brief 定义控件的状态
*/
typedef uint32 GUISETLIST_STATE_T;
/*! @def GUISETLIST_STATE_NONE
@brief null state
*/
#define GUISETLIST_STATE_NONE               0x00000000 // null state
/*! @def GUISETLIST_STATE_ISCYC
@brief 是否循环搜索，默认不循环
*/
#define GUISETLIST_STATE_ISCYC              0x00000001 // 是否循环搜索，默认不循环
/*! @def GUISETLIST_STATE_USER_OPEN
@brief 用户自己打开窗口
*/
#define GUISETLIST_STATE_USER_OPEN          0x00000002 // 用户自己打开窗口
/*! @def GUISETLIST_STATE_OPEN_POPWIN
@brief 自动打开pop list win
*/
#define GUISETLIST_STATE_OPEN_POPWIN        0x00000004 // 自动打开pop list win
/*! @def GUISETLIST_STATE_OPEN_FULLWIN
@brief 自动打开pop list win
*/
#define GUISETLIST_STATE_OPEN_FULLWIN       0x00000008 // 自动打开pop list win
/*! @def GUISETLIST_STATE_HAS_TITLE
@brief  是否有Title
*/
#define GUISETLIST_STATE_HAS_TITLE          0x00000010 // 是否有Title
/*! @def GUISETLIST_STATE_LEFTSK_SWITCH
@brief left softkey响应与web键相同的切换，默认为FALSE
*/
#define GUISETLIST_STATE_LEFTSK_SWITCH      0x00000020 // left softkey响应与web键相同的切换，默认为FALSE

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUISETLIST_DATA_TYPE_E
@brief 定义控件数据的类型
*/
typedef enum
{
    GUISETLIST_DATA_NONE,		/*!< 无*/
    GUISETLIST_DATA_TEXT,       /*!< 文本*/ 
    GUISETLIST_DATA_IMAGE_ID,   /*!< 图片id*/
    GUISETLIST_DATA_ANIM_DATA,  /*!< 图片数据*/ 
    GUISETLIST_DATA_ANIM_PATH,  /*!< 图片路径*/
    GUISETLIST_DATA_MAX,
}GUISETLIST_DATA_TYPE_E;

/*! @enum GUISETLIST_TYPE_E
@brief 定义控件显示类型
*/
typedef enum
{
    GUISETLIST_TYPE_DEFAULT,
    GUISETLIST_TYPE_RADIOLIST = GUISETLIST_TYPE_DEFAULT, /*!<  单选(Radio)类型 */
    GUISETLIST_TYPE_CHECKLIST,                           /*!<  多选(check)类型 */
}GUISETLIST_TYPE_E;

/*! @union GUISETLIST_DATA_U
@brief 定义图片数据联合体
*/
typedef union
{
    MMI_IMAGE_ID_T              image_id;    /*!< image id*/  
    GUIANIM_DATA_INFO_T         anim_data;   /*!< 图片数据, 在set item content时传入, 不拷贝数据*/ 
    GUIANIM_FILE_INFO_T         anim_path;   /*!< 图片路径, 在set item content时传入, 不拷贝数据*/ 
}GUISETLIST_DATA_U;

/*! @struct GUISETLIST_IMAGE_T
@brief 定义图片数据的内容
*/
typedef struct
{
    uint16                  image_width;      /*!< 图片显示的宽度*/
    uint16                  image_height;     /*!< 图片显示的高度*/  
    GUISETLIST_DATA_TYPE_E  image_data_type;  /*!< 图片数据类型*/ 
    GUISETLIST_DATA_U       image_data;       /*!< 图片数据信息联合体*/ 
}GUISETLIST_IMAGE_T; 

/*! @struct GUISETLIST_BG_T
@brief 定义背景数据信息
*/
typedef struct  
{
    MMI_GUISETLIST_BG_TYPE_E bg_type;   /*!< 背景类型*/
    GUI_COLOR_T          color;			/*!< 背景颜色*/
    MMI_IMAGE_ID_T       image_id; 		/*!< 背景图片ID*/
}GUISETLIST_BG_T;

/*! @struct guisetlist_item_t_tag
@brief   定义item数据信息
*/
typedef struct guisetlist_item_t_tag
{
    GUISETLIST_ITEM_STATE_T     item_state; /*!< item状态*/
    MMI_STRING_T                str_info;   /*!< item字符串信息*/
    GUISETLIST_IMAGE_T          image_info; /*!< item图片信息*/
    uint32                      add_data;   /*!< 该字段无需用户填写,系统使用*/ 
    BOOLEAN                     is_selected;/*!< 多选格式下，item是否被选中*/ 
}GUISETLIST_ITEM_INFO_T;

/*! @struct GUISETLIST_ITEM_T
@brief Item链表
*/
typedef struct  
{
    uint32          item_total_num; /*!< 总数*/
    MMI_LINK_NODE_T *item_head_ptr; /*!< 链表*/
}GUISETLIST_ITEM_T;

/*! @struct GUISETLIST_INIT_DATA_T
@brief 控件的初始化数据——控件区域
*/
typedef struct 
{
    GUI_BOTH_RECT_T           both_rect; /*!< 控件横竖屏区域*/

}GUISETLIST_INIT_DATA_T; 

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 增加一个Item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_info_ptr [in] 控件Item数据信息
@retval 返回当前item的总数，既Append Item之后的总的item数
@return 返回当前item的总数
@note 在item数据链表的末尾处增加一个item数据，同时item总数会+1
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItem(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                 );

/***************************************************************************//*!
@brief 根据text id增加一个Item
@author jinwang
@param ctrl_id [in] 控件ID
@param text_id [in] text id
@retval 返回当前item的总数，既Append Item之后的总的item数
@return 返回当前item的总数
@note 在item数据链表的末尾处增加一个item数据，同时item总数会+1
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItemById(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 MMI_TEXT_ID_T text_id
                                 );

/***************************************************************************//*!
@brief 插入一个Item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_info_ptr [in] 控件Item数据信息
@param item_index [in] 插入链表的位置
@retval TRUE 成功
@retval FALSE 失败
@return 返回成功或失败
@note item_index 大于等于总数时，会增加在链表末尾
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_InsertItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     );


/***************************************************************************//*!
@brief 修改一个Item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_info_ptr [in] 控件Item数据信息
@param item_index [in] 插入链表的位置
@retval TRUE 成功；FALSE失败
@return 返回成功或失败
@note 当修改一个item的时候，使用item_info_ptr的值替换item_index对应的值，
如果item_index大于等于总数，则返回FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ModifyItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     );


/***************************************************************************//*!
@brief 增加一个Item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引号
@retval 成功或失败
@return  返回成功或失败
@note 当索引号大于等于总数时（索引号从0开始）或者控件不存在，则返回FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     uint32 item_index
                                     );

/***************************************************************************//*!
@brief 删除所有节点
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 成功或失败
@return  返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteAllItem(
                MMI_CTRL_ID_T ctrl_id
                         );

/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] 控件ID
@param bg_ptr [in] 背景数据信息
@retval TRUE/FALSE
@return 返回成功或失败
@note 设置控件的背景,FALSE,设置错误,TRUE设置成功，当输入参数为空，或控件不存在时
返回FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBackGround(
                                        MMI_CTRL_ID_T ctrl_id, 
                                        GUI_BG_T *bg_ptr
                                        );


/***************************************************************************//*!
@brief  设置当前索引号，即当前显示的Item索引
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引号
@retval TRUE/FALSE
@return 如果设置的item index对应的不存在,或者存在了但是不可见,返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCurIndex(
                                      MMI_CTRL_ID_T ctrl_id, 
                                      uint32 item_index
                                      );


/***************************************************************************//*!
@brief 获取当前索引,如果当前总数为0,返回0xffffffff
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 返回当前Item索引
@return 如果总数为0,返回GUISETLIST_INVALID_ITEM_INDEX
@note 
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_GetCurIndex(
                                     MMI_CTRL_ID_T ctrl_id
                                     );


/***************************************************************************//*!
@brief 动态创建控件
@author xiaoqing.lu
@param win_handle [in] 窗口句柄，或窗口ID
@param ctrl_id [in] 控件ID
@param rect_ptr [in] 控件区域
@retval TRUE/FALSE
@return 返回创建成功或失败
@note 当控件已经存在，或申请内存失败时，返回FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id,
                                        GUI_BOTH_RECT_T *rect_ptr
                                        );


/***************************************************************************//*!
@brief  设置控件的显示区域
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param rect_ptr [in] 显示区域
@param is_update [in] 是否立即刷新
@retval TRUE/FALSE
@return 返回成功或失败
@note 显示区域即所见区域，与控件的rect有区别
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         const GUI_RECT_T *rect_ptr,      //in:
                                         BOOLEAN          is_update       //in:
                                         );


/***************************************************************************//*!
@brief 获得控件的显示区域
@author Jassmine
@param ctrl_id [in] 控件ID
@param rect_ptr [in/out] 控件显示区域
@retval TRUE/FALSE
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         GUI_RECT_T   *rect_ptr       //in/out:
                                         );


#ifdef TOUCH_PANEL_SUPPORT
/***************************************************************************//*!
@brief 设置控件的tp区域，即tp点击时的响应区域
@author Jassmine 
@param ctrl_id [in] 控件ID
@param rect_ptr [in] 区域
@retval TRUE/FALSE
@return 返回成功或失败
@note 只有form控件才使用该接口，其他应用不应该使用
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTpRect(
                                    MMI_CTRL_ID_T   ctrl_id,    //in:
                                    GUI_RECT_T      *rect_ptr   //in:
                                    );
#endif


/***************************************************************************//*!
@brief set control boder info
@author Jassmine
@param ctrl_id [in] 控件ID
@param border_ptr [in] 边界信息
@retval TRUE/FALSE
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBorder(
                                    MMI_CTRL_ID_T ctrl_id,  //in:
                                    GUI_BORDER_T  *border_ptr
                                    );


/***************************************************************************//*!
@brief set control boder info
@author Jassmine
@param ctrl_id [in] 控件ID
@param font_ptr [in] 文字信息，如字体或颜色
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFont(
                                  MMI_CTRL_ID_T ctrl_id,  //in:
                                  GUI_FONT_ALL_T  *font_ptr
                                  );


/***************************************************************************//*!
@brief set setting list is visible
@author Jassmine
@param ctrl_id [in] 控件ID
@param is_visible [in] 控件是否可见
@param is_update [in] 是否立即刷新
@retval true/false
@return 返回成功或失败
@note 应用时注意刷新的时机
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetVisible(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_visible,
                                     BOOLEAN        is_update
                                     );


/***************************************************************************//*!
@brief set setting list is circular handle
@author Jassmine
@param ctrl_id [in] 控件ID
@param is_circular [in] 是否循环切换
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCircular(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_circular
                                      );


/***************************************************************************//*!
@brief  set setting list left/right arrow icon
@author Jassmine
@param ctrl_id [in] 控件ID
@param left_down [in] 是否循环切换
@param left_up [in] 控件ID
@param right_down [in] 是否循环切换
@param right_up [in] 是否循环切换
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetArrow(
                                   MMI_CTRL_ID_T     ctrl_id,
                                   MMI_IMAGE_ID_T   left_down,
                                   MMI_IMAGE_ID_T   left_up,
                                   MMI_IMAGE_ID_T   right_down,
                                   MMI_IMAGE_ID_T   right_up
                                   );


/***************************************************************************//*!
@brief 一个回调函数，该函数用于打开list窗口时
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param list_ctrl_ptr [in] 内嵌的guilist控件指针
@param param [in] 输入数据
@param call_type [in] 执行回调的类型
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLSETLIST_CallBack(
                                MMI_CTRL_ID_T   ctrl_id,
                                IGUICTRL_T      *list_ctrl_ptr,
                                DPARAM          param,
                                MMI_SETLIST_CALL_TYPE_E call_type
                                );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param str_ptr [in] 字符串
@param is_set [in] 设置或取消设置
@retval true/false
@return 返回成功或失败
@note if is_set = TRUE, set title; else if is_set = FALSE, release title
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleInfo(
                                       MMI_CTRL_ID_T ctrl_id,
                                       MMI_STRING_T *str_ptr,
                                       BOOLEAN is_set
                                       );


/***************************************************************************//*!
@brief set title by text id
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_id [in] 字符串的资源ID
@retval true/false
@return 返回成功或失败
@note 函数内部将解析text id，转化为字符串信息，再设置
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleTextId(
                                         MMI_CTRL_ID_T ctrl_id,
                                         MMI_TEXT_ID_T text_id
                                         );


/***************************************************************************//*!
@brief 设置控件状态
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param aim_state [in] 状态值
@param is_set [in] 设置或取消设置
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLSETLIST_SetCtrlState(
                                    MMI_CTRL_ID_T     ctrl_id, 
                                    uint32 aim_state,
                                    BOOLEAN is_set
                                    );


/***************************************************************************//*!
@brief set setting list string effect
@author Jassmine
@param ctrl_id [in] 控件ID
@param effect [in] 字符串特效
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStrEffect(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       GUIFONT_EFFECT_T effect
                                       );


/***************************************************************************//*!
@brief set setting list need arrow
@author Xiaoqing.Lu
@param ctrl_id [in] 控件ID
@param is_need_arrow [in] 是否需要箭头
@retval true/false
@return 返回成功或失败
@note default FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetNeedArrow(
                                       MMI_CTRL_ID_T     ctrl_id,
                                       BOOLEAN is_need_arrow
                                       );


/***************************************************************************//*!
@brief 设置文字对齐方式
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param font_align [in] 对齐方式
@retval true/false
@return 返回成功或失败
@note default ALIGN_HVMIDDLE（竖屏方向和垂直方向都居中）
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFontAlign(
                                       MMI_CTRL_ID_T ctrl_id,  //in:
                                       GUI_ALIGN_E font_align
                                       );

/***************************************************************************//*!
@brief 设置显示风格(单选/多选)
@author jin.wang
@param ctrl_id  [in] 控件ID
@param type     [in] 风格
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStyleById(
    MMI_CTRL_ID_T ctrl_id, 
    GUISETLIST_TYPE_E type
);

/***************************************************************************//*!
@brief 获取item选中状态
@author jin.wang
@param ctrl_id      [in] 控件ID
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ResetAllItemState(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 获取item选中状态
@author jin.wang
@param ctrl_id      [in] 控件ID
@param item_index   [in] item的index
@return 返回item状态
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index
);

/***************************************************************************//*!
@brief 设置item选中状态
@author jin.wang
@param ctrl_id      [in] 控件ID
@param item_index   [in] item的index
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    BOOLEAN item_state
);

/***************************************************************************//*!
@brief 设置选中item显示字符内容
@author jin.wang
@param ctrl_id          [in] 控件ID
@param str_ptr          [in] 字符信息
@param is_need_update   [in] 是否需要刷新
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDispText(
    MMI_CTRL_ID_T ctrl_id, 
    MMI_STRING_T *str_ptr,
    BOOLEAN is_need_update
);

/***************************************************************************//*!
@brief 设置多选模式下选中item的text内容
@author jin.wang
@param ctrl_id      [in] 控件ID
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDefaultText(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 设置选中item的index
@author jin.wang
@param ctrl_id      [in] 控件ID
@param idx_ptr      [in/out] 用于存放所有索引的数组，可以为空
@param array_len    [in] 被选中的总个数
@return 选中item的个数
@note 用户如果不确定传入的数组到底应该多大，那么可首先调用该接口获得总数，然后再分配内存获取具体被选中项的索引号
*******************************************************************************/
PUBLIC uint16 CTRLSETLIST_GetSelectItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16* idx_ptr,
    uint16 array_len
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
