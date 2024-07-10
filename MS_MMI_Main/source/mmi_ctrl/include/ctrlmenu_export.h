/***************************************************************************
** File Name:      ctrlmenu_export.h                                       *
** Author:         Jassmine                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011       Jassmine         Create                                   *
**												                                  						   *
****************************************************************************/
#ifndef _CTRLMENU_EXPORT_H_
#define _CTRLMENU_EXPORT_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "guisoftkey.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                      *
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/
#define GUIMENU_DYNA_NODE_MAX_NUM       50  //dynamic menu max node number

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
--------------------------------------------------------------------------*/
/*! @enum GUIMENU_STYLE_E
@brief 菜单风格
*/
typedef enum
{
    GUIMENU_STYLE_NULL        = 0,  /*!< 无 */
    GUIMENU_STYLE_SECOND      = 1,  /*!< 二级菜单风格，显示为列表形式，多用于由主菜单进入的二级菜单 */
    GUIMENU_STYLE_THIRD       = GUIMENU_STYLE_SECOND,  /*!< 已去掉此风格，其显示同GUIMENU_STYLE_SECOND */
    GUIMENU_STYLE_POPUP       = 5,  /*!< 弹出式菜单，由系统统一计算它的位置和宽度，用户不能设置 */
    GUIMENU_STYLE_POPUP_RADIO = 6,  /*!< 弹出式菜单，菜单项带有单选的radio风格 */
    GUIMENU_STYLE_POPUP_CHECK = 7,  /*!< 弹出式菜单，菜单项带有多选的check风格 */
    GUIMENU_STYLE_POPUP_AUTO  = 10, /*!< 弹出式菜单，其坐标可由用户设置，且会根据文本自适应宽度 */
    GUIMENU_STYLE_POPUP_RECT  = 11, /*!< 弹出式菜单，其坐标和宽度可由用户设置，系统不会做自适应 */
    GUIMENU_STYLE_OPTIONS     = 14, /*!< OptionsMenu */

    GUIMENU_STYLE_MAX
} GUIMENU_STYLE_E;

/*! @enum GUIMENU_EP_E
@brief 用于GUIMENU_STYLE_POPUP_AUTO风格下，哪个角是起点
@details
在GUIMENU_STYLE_POPUP_AUTO风格下，菜单的坐标可以由用户设置，默认情况下，这类菜单
按照左上角作为起点开始显示，但在某些情况下，按照其他的参考点则比较容易，例如，
点击左软键时，需要在左软键上方弹出菜单，那么，将左下角作为起点，菜单往上弹，
这样设置用户就比较容易理解。
*/
typedef enum
{
    GUIMENU_EP_TOP_LEFT     = 0,    /*!< 左上角为有效起点 */
    GUIMENU_EP_TOP_RIGHT    = 1,    /*!< 右上角为有效起点 */
    GUIMENU_EP_BOTTOM_LEFT  = 2,    /*!< 左下角为有效起点 */
    GUIMENU_EP_BOTTOM_RIGHT = 3     /*!< 右下角为有效起点 */
} GUIMENU_EP_E;

/*! @struct GUIMENU_BUTTON_STYLE_E
@brief Button显示类型
*/
typedef enum
{
    GUIMENU_BUTTON_STYLE_NONE,                  /*!< 不显示Button*/
    GUIMENU_BUTTON_STYLE_OK,                    /*!< 显示OK */
    GUIMENU_BUTTON_STYLE_CANCEL,                /*!< 显示Cancel */
    GUIMENU_BUTTON_STYLE_OK_CANCEL,             /*!< 显示OK, Cancel两个Button */
    GUIMENU_BUTTON_STYLE_MAX
} GUIMENU_BUTTON_STYLE_E;

/*! @struct GUIMENU_INIT_DATA_T
@brief 菜单初始化数据
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< 是否静态菜单 */
    uint16                  node_max_num;   /*!< 菜单的最大节点数，只用于动态菜单，默认为\link #GUIMENU_DYNA_NODE_MAX_NUM \endlink */
    GUI_BOTH_RECT_T         both_rect;      /*!< 横竖屏下的坐标 */
    GUIMENU_STYLE_E         menu_style;     /*!< 菜单风格，只在动态菜单有效，静态菜单的风格在定义菜单数组时已确定 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组id，只用于静态菜单 */
} GUIMENU_INIT_DATA_T;

/*! @struct  GUIMENU_TITLE_INFO_T
@brief title信息数据结构
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< 是否静态菜单 */
    uint32                  node_id;        /*!< 菜单节点ID @note is_static为FALSE时有效 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组ID @note is_static为TRUE时有效 */
    MMI_IMAGE_ID_T          title_icon_id;  /*!< title的图片 */
    MMI_STRING_T            *title_ptr;     /*!< 指向title的指针 @note 如果为PNULL，表示忽略此项，如果指向的文本为空，表示清除title，如果指向有效文本，表示设置title */
    MMI_STRING_T            *sub_title_ptr; /*!< 指向sub title的指针 @note 如果为PNULL，表示忽略此项，如果指向的文本为空，表示清除sub title，如果指向有效文本，表示设置sub title */
} GUIMENU_TITLE_INFO_T;

/*! @struct  GUIMENU_ITEM_T
@brief 菜单项数据结构
*/
typedef struct
{
    MMI_MENU_ID_T               menu_id;                            /*!< 菜单项id */
    MMI_MENU_TIP_T              tip_id;                             /*!< 暂时不用，赋TIP_NULL */
    MMI_TEXT_ID_T               button_id[GUISOFTKEY_BUTTON_NUM];  /*!< softkey的提示信息 */
    MMI_TEXT_ID_T               text_str_id;                        /*!< 菜单项文本ID */
    MMI_IMAGE_ID_T              select_icon_id;                     /*!< 选中框的图标ID */
    MMI_IMAGE_ID_T              unselect_icon_id;                   /*!< 菜单图标ID */
    uint32                      active_image_id;                    /*!< 获得焦点时的页切换图标ID，用于GUIMENU_STYLE_OPTION_PAGE风格下 */
    MMI_MENU_GROUP_ID_T         link_group_id;                      /*!< 子菜单的组ID */
} GUIMENU_ITEM_T;

/*! @struct  GUIMENU_GROUP_T
@brief  菜单组数据结构
*/
typedef struct
{
    const GUIMENU_ITEM_T        *item_ptr;          /*!< 指向菜单项的数组首地址 */
    GUIMENU_STYLE_E             menu_style;         /*!< 菜单风格 */
    MMI_TEXT_ID_T               title_string_id;    /*!< 菜单标题文本ID */
    MMI_IMAGE_ID_T              title_icon_id;      /*!< 标题栏背景图片ID */
    MMI_IMAGE_ID_T              title_numicon_id;   /*!< 暂不支持 */
    MMI_IMAGE_ID_T              title_background;   /*!< 暂不支持 */
    uint16                      item_count;         /*!< 菜单项的个数，可以通过\link #GUIMENU_ITEM_NUM \endlink计算 */
} GUIMENU_GROUP_T;

// 动态菜单的图标类型
typedef enum
{
	DYNA_ITEM_ICON_ID,
	DYNA_ITEM_ICON_BUFFER,
}DYNA_ITEM_ICON_TYPE_E;

//动态菜单的图标类型为DYNA_ITEM_ICON_BUFFER时，需要填写
typedef struct  
{
	uint8* data;
	uint16 height;
	uint16 width;
 } DYNA_ITEM_ICON_BUFFER_T;

/*! @struct  GUIMENU_DYNA_ITEM_T
@brief 动态菜单项数据结构
*/
typedef struct
{
    BOOLEAN					is_grayed;      /*!< 是否灰显 */
    MMI_STRING_T			*item_text_ptr; /*!< 菜单项文本 */
	DYNA_ITEM_ICON_TYPE_E	item_type;  	/*动态菜单的item icon类型,默认为DYNA_ITEM_ICON_ID*/
    MMI_IMAGE_ID_T			select_icon_id; /*!< 菜单项图标ID，item_type为DYNA_ITEM_ICON_ID时，必填*/
	DYNA_ITEM_ICON_BUFFER_T icon_buffer;	/*!< 菜单项图标信息，item_type为DYNA_ITEM_ICON_BUFFER时，必填*/
} GUIMENU_DYNA_ITEM_T;

/*! @struct  GUIMENU_CHECK_INFO_T
@brief CHECK状态的菜单项信息
*/
typedef struct
{
    uint32                  node_id;        /*!< 菜单节点ID @note 动态菜单时有效 */
    MMI_MENU_ID_T           menu_id;        /*!< 菜单项ID @note 静态菜单时有效 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组ID @note 静态菜单时有效 */
} GUIMENU_CHECK_INFO_T;

/*! @struct  GUIMENU_BUTTON_INFO_T
@brief button信息数据结构
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< 是否静态菜单 */
    uint32                  node_id;        /*!< 菜单节点ID @note is_static为FALSE时有效 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组ID @note is_static为TRUE时有效 */
    GUIMENU_BUTTON_STYLE_E  button_style;   /*!< button style */
} GUIMENU_BUTTON_INFO_T;

/*! @struct  GUIMENU_POP_SEL_INFO_T
@brief 指定菜单项数据结构
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< 是否静态菜单 */
    uint32                  node_id;        /*!< 菜单节点ID @note is_static为FALSE时有效 */
    MMI_CTRL_ID_T           ctrl_id;        /*!< 菜单控件ID */
    MMI_MENU_ID_T           menu_id;        /*!< 菜单项ID @note is_static为TRUE时有效 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组ID @note is_static为TRUE时有效 */
} GUIMENU_POP_SEL_INFO_T;

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : return menu group id and menu id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_GetId(
                           MMI_HANDLE_T         ctrl_handle,    //in:
                           MMI_MENU_GROUP_ID_T  *group_id_ptr,  //in/out:
                           MMI_MENU_ID_T        *menu_id_ptr    //in/out:
                           );

/*****************************************************************************/
//  Description : set menu the first and selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetFirstAndSelItem(
                                        uint16          first_item_index,
                                        uint16          cur_item_index,
                                        MMI_HANDLE_T    ctrl_handle
                                        );

/*****************************************************************************/
//  Description :set menu title,for dynamic menu or volatile static menu title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetTitle(
                              MMI_STRING_T      *title_text_ptr,//title text
                              MMI_CTRL_ID_T     ctrl_id         //control id
                              );

/***************************************************************************//*!
//  Description :设置Title的扩展方法
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetTitleEx(
                                   MMI_CTRL_ID_T        ctrl_id,        // [in]
                                   GUIMENU_TITLE_INFO_T *title_info_ptr // [in]
                                   );

/***************************************************************************//*!
@brief 创建动态菜单
@author Jassmine
@param rect_ptr [in] 指向控件显示区域的指针。如果是弹出式菜单的话，该值可以为PNULL。
@param win_id [in] 窗口ID
@param ctrl_id [in] 控件ID
@param menu_style [in] 控件风格
@note 该方法并没有指定节点数，默认为\link #GUIMENU_DYNA_NODE_MAX_NUM \endlink，
如果想要自定义节点数，请使用\link GUIMENU_CreatDynamicEx() \endlink。
*******************************************************************************/
PUBLIC void CTRLMENU_CreatDynamic(
                                  GUI_BOTH_RECT_T   *rect_ptr,  //may PNULL,pop-up no need rect
                                  MMI_WIN_ID_T      win_id,
                                  MMI_CTRL_ID_T     ctrl_id,
                                  GUIMENU_STYLE_E   menu_style  //menu style
                                  );

/***************************************************************************//*!
@brief 创建动态菜单
@author Jassmine
@param node_max_num [in] 最大的节点数
@param rect_ptr [in] 指向控件显示区域的指针。如果是弹出式菜单的话，该值可以为PNULL。
@param win_id [in] 窗口ID
@param ctrl_id [in] 控件ID
@param menu_style [in] 控件风格
@note
*******************************************************************************/
PUBLIC void CTRLMENU_CreatDynamicEx(
                                    uint16          node_max_num,   //in:
                                    GUI_BOTH_RECT_T *rect_ptr,      //may PNULL,pop-up no need rect
                                    MMI_WIN_ID_T    win_id,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    GUIMENU_STYLE_E menu_style      //menu style
                                    );

/***************************************************************************//*!
@brief 获取当前选中项的ID
@author Jassmine
@param ctrl_id [in] 控件ID
@return 选中项的ID
@note 该方法只适用于动态菜单\n
静态菜单获取当前选中项，参考\link GUIMENU_GetId() \endlink
*******************************************************************************/
PUBLIC uint32 CTRLMENU_GetCurNodeId(
                                    MMI_CTRL_ID_T   ctrl_id //control id
                                    );

/***************************************************************************//*!
@brief 往动态菜单上插入节点
@author Jassmine
@param nodex_index [in] 节点索引值，从0开始
@param node_id [in] 用户自定义的节点ID，从1开始
@param parent_node_id [in] 父节点ID，如果是0，代表根节点
@param node_item_ptr [in] 指向菜单项信息的指针
@param ctrl_id [in] 控件ID
@note
-# 该方法只适用于动态菜单
-# nodex_index的索引值，是根据插入之前，菜单里已有菜单项的顺序来计算，从0开始
-# node_id必须从1开始，因为0代表根节点
*******************************************************************************/
PUBLIC void CTRLMENU_InsertNode(
                                uint16                  nodex_index,    //node index,from 0
                                uint32                  node_id,        //node id,from 1
                                uint32                  parent_node_id, //parent node id
                                GUIMENU_DYNA_ITEM_T     *node_item_ptr, //node item
                                MMI_CTRL_ID_T           ctrl_id         //control id
                                );

/***************************************************************************//*!
@brief 重新设置动态菜单项的信息
@author James
@param nodex_index [in] 节点索引值，从0开始
@param node_id [in] 重新设置的用户自定义节点ID，从1开始
@param parent_node_id [in] 父节点ID，如果是0，代表根节点
@param node_text_ptr [in] 指向菜单项文本的指针
@param ctrl_id [in] 控件ID
@return
@note
-# 该方法只适用于动态菜单
-# nodex_index的索引值，是根据插入之前，菜单里已有菜单项的顺序来计算，从0开始
-# node_id必须从1开始，因为0代表根节点
*******************************************************************************/
PUBLIC void CTRLMENU_ReplaceNode(
                                 uint16         nodex_index,    //node index,from 0
                                 uint32         node_id,        //node id,from 1
                                 uint32         parent_node_id, //parent node id
                                 MMI_STRING_T   *node_text_ptr, //node text
                                 MMI_CTRL_ID_T  ctrl_id         //control id
                                 );

#if defined (MRAPP_SUPPORT)||defined (HERO_ENGINE_SUPPORT ) 
/*****************************************************************************/
//  Description : in dynamic menu, replace node, 携带icon变更
//  Global resource dependence : 
//  Author: sky.qgp
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ReplaceNodeEx(
                                   uint16               node_index,     //node index,from 0
                                   uint32               node_id,        //node id,from 1
                                   uint32               parent_node_id, //parent node id
                                   GUIMENU_DYNA_ITEM_T  *node_item_ptr, //node item
                                   MMI_CTRL_ID_T        ctrl_id         //control id
                                   );
#endif

/***************************************************************************//*!
@brief 设置动态菜单项是否灰显
@author Jassmine
@param is_grayed [in] 是否灰显
@param nodex_index [in] 节点索引值，从0开始
@param parent_node_id [in] 父节点ID，如果是0，代表根节点
@param ctrl_id [in] 控件ID
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayed(
                                  BOOLEAN       is_grayed,      //set node grayed
                                  uint16        nodex_index,    //node index,from 0
                                  uint32        parent_node_id, //parent node id
                                  MMI_CTRL_ID_T ctrl_id         //control id
                                  );

/*****************************************************************************/
//  Description : in dynamic menu, set node grayed
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayedByNodeId(
                                           BOOLEAN          is_grayed,      //set node grayed
                                           uint32           node_id,                // [in]
                                           uint32           parent_node_id, //parent node id
                                           MMI_CTRL_ID_T    ctrl_id         //control id
                                           );

/***************************************************************************//*!
@brief 设置菜单项是否可见
@author hua.fang
@param ctrl_id [in] 控件ID
@param nodex_index [in] 节点索引值，从0开始
@param parent_node_id [in] 父节点ID，如果是0，代表根节点
@param is_visible [in] 是否可见
@return 是否设置成功
@note 该方法只适用于动态菜单，静态菜单可调用\link GUIMENU_SetItemVisible() \endlink
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetNodeVisible(
                                       MMI_CTRL_ID_T    ctrl_id,        // [in]
                                       uint16           nodex_index,    // [in] node index,from 0
                                       uint32           parent_node_id, // [in]
                                       BOOLEAN          is_visible      // [in]
                                       );

/***************************************************************************//*!
@brief  设置softkey的显示内容
@author Jassmine
@param ctrl_id [in] 控件ID
@param left_softkey_text_id [in] 左软键文本ID
@param middle_softkey_text_id [in] 中软键文本ID
@param right_softkey_text_id [in] 右软键文本ID
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetDynamicMenuSoftkey(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMI_TEXT_ID_T    left_softkey_text_id,
                                           MMI_TEXT_ID_T    middle_softkey_text_id,
                                           MMI_TEXT_ID_T    right_softkey_text_id
                                           );

/***************************************************************************//*!
@brief 设置菜单背景色
@author Jassmine
@param ctrl_id [in] 控件ID
@param bg_color [in] 背景色
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetBgColor(
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		bg_color
                                );

/***************************************************************************//*!
@brief 设置菜单的文本颜色
@author Jassmine
@param ctrl_id [in] 控件ID
@param font_color [in] 文本颜色
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetFontColor(
                                  MMI_CTRL_ID_T ctrl_id,
                                  GUI_COLOR_T   font_color
                                  );

/***************************************************************************//*!
@brief 获取radio或check风格处于选中状态的菜单项
@author hua.fang
@param ctrl_id [in] 控件ID
@param check_info_ptr [in] 选中状态的菜单项信息数组
@param array_len [in] 数组长度
@return 总共的选中个数
@note 用户如果不确定传入的数组到底应该多大，那么可首先调用该接口获得总数，然后再分配内存获取具体被选中项
*******************************************************************************/
PUBLIC uint16 CTRLMENU_GetCheckInfo(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   GUIMENU_CHECK_INFO_T *check_info_ptr,
                                   uint16               array_len
                                   );

/***************************************************************************//*!
@brief 设置菜单的显示区域
@author Jassmine
@param ctrl_handle [in] 控件ID
@param rect_ptr [in] 指向显示区域的指针
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetRect(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_RECT_T     *rect_ptr
                             );

/***************************************************************************//*!
@brief 设置菜单项是否灰显
@author hua.fang
@param ctrl_id [in] 控件ID
@param group_id [in] 菜单组ID
@param menu_id [in] 菜单项ID
@param is_grayed [in] 是否灰显
@return 是否设置成功
@note 该方法只适用于静态菜单
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemGrayed(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      BOOLEAN               is_grayed   // [in]
                                      );

/***************************************************************************//*!
@brief 设置菜单项是否可见
@author hua.fang
@param ctrl_id [in] 控件ID
@param group_id [in] 菜单组ID
@param menu_id [in] 菜单项ID
@param is_visible [in] 是否可见
@return 是否设置成功
@note 该方法只适用于静态菜单，动态菜单可调用\link GUIMENU_SetNodeVisible() \endlink
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemVisible(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       MMI_MENU_ID_T        menu_id,    // [in]
                                       BOOLEAN              is_visible  // [in]
                                       );

/*****************************************************************************/
//  Description : set item text and image
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetStaticItem(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      MMI_STRING_T          *text_ptr,  // [in] PNULL表示不设置文本
                                      MMI_IMAGE_ID_T        icon_id     // [in] 0或者IMAGE_NULL表示不设置图片
                                      );

/***************************************************************************//*!
@brief 设置自适应菜单的最大宽度
@author hua.fang
@param ctrl_id [in] 控件ID
@param max_horz_width [in] 横屏下的最大宽度
@param max_vert_width [in] 竖屏下的最大宽度
@return 是否设置成功
@note 该方法仅在\link #GUIMENU_STYLE_POPUP_AUTO \endlink风格下才起作用
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetMaxWidth(
                                    MMI_CTRL_ID_T   ctrl_id,        // [in]
                                    uint16          max_horz_width, // [in]
                                    uint16          max_vert_width  // [in]
                                    );

/***************************************************************************//*!
@brief 设置\link #GUIMENU_STYLE_POPUP_AUTO \endlink风格下，哪个角是起点值
@author hua.fang
@param ctrl_id [in] 控件ID
@param effective_point [in] 指定有效点
@return 是否设置成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetEffectivePoint(
                                          MMI_CTRL_ID_T     ctrl_id,          // [in]
                                          GUIMENU_EP_E      effective_point   // [in]
                                          );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state
                                 );

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state,
                                 BOOLEAN        is_true
                                 );

/*****************************************************************************/
//  Description : set current selected item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 可以自动调整top值
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSelectedItem(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     uint16         cur_item_index
                                     );

/*****************************************************************************/
//  Description : 设置button的显示风格
//  Global resource dependence : 
//  Author: hua.fang
//  Note:该方法只对GUIMENU_STYLE_POPUP_AUTO,GUIMENU_STYLE_POPUP_CHECK,GUIMENU_STYLE_POPUP_RADIO,GUIMENU_STYLE_POPUP_RECT有效
/*****************************************************************************/
PUBLIC void CTRLMENU_SetButtonStyle(
                                    MMI_CTRL_ID_T           ctrl_id,         // [in]
                                    GUIMENU_BUTTON_INFO_T   *button_info_ptr // [in]
                                    );

/*****************************************************************************/
//  Description : get node string
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetNodeStr(
                                   MMI_CTRL_ID_T    ctrl_id,        //in:
                                   uint16           node_index,     //in:
                                   MMI_STRING_T     *node_str_ptr   //in/out:
                                   );

/*****************************************************************************/
//  Description : is pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsPopMenuEx(
                                    MMI_HANDLE_T    ctrl_handle
                                    );

/***************************************************************************//*!
@brief 设置格数
@author hua.fang
@param ctrl_id [in] 控件ID
@param item_height [in] 二级菜单的杭高
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetPopItemHeight(
                                         MMI_CTRL_ID_T  ctrl_id,    // [in]
                                         uint16         item_height // [in]
                                         );

/*****************************************************************************/
//  Description : set pop-up menu item status for radio or check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetPopItemStatus(
                                      BOOLEAN                   is_selected,
                                      GUIMENU_POP_SEL_INFO_T    *selected_info_ptr
                                      );

/***************************************************************************//*!
@brief 设置二级子菜单的风格
@author hua.fang
@param ctrl_id [in] 控件ID
@param node_id [in] 节点ID
@param style [in] 菜单风格
@return
@note
-# 该方法只适用于动态菜单
-# style的取值范围为{\link #GUIMENU_STYLE_POPUP \endlink,
   \link #GUIMENU_STYLE_POPUP_RADIO \endlink, \link #GUIMENU_STYLE_POPUP_CHECK \endlink}，
原来的风格也必须是以上几种。
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetSubMenuStyle(
                                        MMI_CTRL_ID_T       ctrl_id,    // [in]
                                        uint32              node_id,    // [in]
                                        GUIMENU_STYLE_E     style       // [in]
                                        );

/*****************************************************************************/
//  Description : is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsFirstDisp(
                                    MMI_CTRL_ID_T   ctrl_id //in:
                                    );

/*****************************************************************************/
//  Description : set is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetFirstDisp(
                                     MMI_CTRL_ID_T  ctrl_id,    //in:
                                     BOOLEAN        is_first    //in:
                                     );

/*****************************************************************************/
//  Description : get menu level
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetLevel(
                                MMI_CTRL_ID_T   ctrl_id //in:
                                );

/*****************************************************************************/
//  Description : get menu rect, display rect,not full screen
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLMENU_GetRect(
                                   MMI_CTRL_ID_T    ctrl_id //in:
                                   );
/*****************************************************************************/
//  Description : set menu font size
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuFontSize(MMI_CTRL_ID_T ctrl_id,GUI_FONT_T font,GUI_FONT_T expand_font);
/*****************************************************************************/
//  Description : set menu item height
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuItemHeight(MMI_CTRL_ID_T ctrl_id,uint16 item_height,uint16 expand_item_height);
/***************************************************************************//*!
@brief 设置某个菜单项文本的颜色
@author longwei.qiao
@param ctrl_id [in] 控件ID
@param group_id [in]
@param item_index [in] 菜单项的索引值
@param font_color [in] 需要显示的颜色
@return 是否设置成功
@note 该方法仅在静态二级菜单风格下才起作用
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemFontColor(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       uint16               item_index,// [in]
                                       GUI_COLOR_T          font_color  // [in]
                                       );

/**--------------------------------------------------------------------------
                          Compiler Flag
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
