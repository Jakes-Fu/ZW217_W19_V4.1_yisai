/*****************************************************************************
** File Name:      ctrlmenu.h                                                *
** Author:         Jassmine                                                  *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

#ifndef _CTRLMENU_H_
#define _CTRLMENU_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlmenu_export.h"
#include "mmitheme_menu.h"
#ifndef WIN32
#include "chng_freq.h"//arvin zhang fixed compile error
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
//menu type info
#define CTRL_MENU_TYPE     (MENU_GetType())

#define CTRLMENU_BUTTON_NUM         2
#define CTRLMENU_STACK_MAX_LEVEL    10  //from 0 to max-1

#define CTRLMENU_TITLE_MAX_LEN      40  //title text max length

// 定义菜单状态，为以后扩展使用
/*! @typedef CTRLMENU_STATE_T
@brief 菜单的状态信息
*/
typedef uint32 CTRLMENU_STATE_T;
/*! @def CTRLMENU_STATE_ENTER
@brief 如果置该位，表示menu处于初始状态，一旦刷新后，该状态设置为FALSE
*/
#define CTRLMENU_STATE_ENTER          0x00000001
/*! @def CTRLMENU_STATE_CUSTOMER_TITLE
@brief 如果置该位，表示菜单不去管理标题，由用户自己控制
*/
#define CTRLMENU_STATE_CUSTOMER_TITLE 0x00000002 // 用户自定义title

/*! @def CTRLMENU_STATE_DRAW_SPLIT_LINE
@brief 如果置该位，表示菜单不去管理标题，由用户自己控制
*/
#define CTRLMENU_STATE_DRAW_SPLIT_LINE 0x00000004 // 画分割线，主要对于列表类型的菜单，如二级菜单，popmenu

/*! @def CTRLMENU_STATE_PRG_PIXEL_SCROLL
@brief 按象素滚动
*/
#define CTRLMENU_STATE_PRG_PIXEL_SCROLL 0x00000008 // 按象素滚动

// the item info address is offset to group info which owns items.
// _group_info_ptr is group info address, and item info is sizeof(CTRLMENU_GROUP_INFO_T)
// behind group info.
#define GET_ITEM_INFO(_group_info_ptr) (CTRLMENU_ITEM_INFO_T*)((uint32)(_group_info_ptr) + sizeof(CTRLMENU_GROUP_INFO_T));

/**--------------------------------------------------------------------------*
**                         TYPE AND STRUCT                                   *
**---------------------------------------------------------------------------*/

//init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T   base_ctrl_param;

    BOOLEAN                 is_static;      /*!< 是否静态菜单 */
    uint16                  node_max_num;   /*!< 菜单的最大节点数，只用于动态菜单，默认为\link #CTRLMENU_DYNA_NODE_MAX_NUM \endlink */
    GUIMENU_STYLE_E         style;          /*!< 菜单风格，只在动态菜单有效，静态菜单的风格在定义菜单数组时已确定 */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< 菜单组id，只用于静态菜单 */
} CTRLMENU_INIT_PARAM_T;

// 静态菜单项扩展信息数据结构
// 该数据结构内部使用，因为GUIMENU_ITEM_T已经对外发布，为了扩展新功能和兼容，
// 才在内部使用这个结构。
typedef struct
{
    BOOLEAN     is_grayed;       // 是否灰显 */
    BOOLEAN     is_visible;      // 是否可见 */
    GUI_COLOR_T item_font_color; // 字体颜色 */
    uint16      is_set_color;
    uint16      reserved;       // 保留位，用来4字节对齐 */
} CTRLMENU_ITEM_INFO_T;

// 静态菜单组扩展信息数据结构
// 该数据结构内部使用，因为GUIMENU_GROUP_T已经对外发布，为了扩展新功能和兼容，
// 才在内部使用这个结构。
typedef struct ctrlmenu_group_info_tag
{
    MMI_MENU_GROUP_ID_T             group_id;               // 菜单组ID */
    uint16                          visible_child_item_num; // 该菜单组中可见菜单项的个数 */
    MMI_IMAGE_ID_T                  title_icon_id;          // 菜单项图标 */
    MMI_STRING_T                    title_str;              // TITLE文本
    MMI_STRING_T                    sub_title_str;          // SUB TITLE文本
    GUIMENU_BUTTON_STYLE_E          button_style;           // button style
    struct ctrlmenu_group_info_tag  *next_group_info_ptr;   // 指向下一个菜单组扩展信息 */
} CTRLMENU_GROUP_INFO_T;

// OptionsMenu的item信息
typedef struct
{
    BOOLEAN         is_more_item;       // 是否是more项
    uint16          line_index;         // 行索引(注意，行是从底下开始算起)
    uint16          column_index;       // 列索引(注意，列是从右边开始算起)
    GUI_RECT_T      rect;               // 显示区域
} CTRLMENU_OPTIONS_ITEM_T;

// 二级滑动菜单记录的信息
typedef struct
{
    uint16      item_index;
    GUI_RECT_T  item_rect;
} CTRLMENU_ITEM_POS_INFO_T;

// 图标数据
typedef union
{
    uint32 icon_id;          /*!< 图片ID */
    struct  
    {
        uint8* data;
        uint16 height;
        uint16 width;
    } buffer;
}CTRLMENU_SELECT_ICON_DATA_T;

// 内部存储动态菜单项数据结构
typedef struct ctrlmenu_node_tag
{
    BOOLEAN                     is_visible;                 // 是否可见 */
    BOOLEAN                     is_grayed;                  // 是否灰显 */
    GUIMENU_STYLE_E             sub_menu_style;             // 子菜单风格 */
    wchar                       *node_wstr_ptr;             // 菜单项文本 */
    uint16                      node_wstr_len;              // 菜单项文本长度 */
    uint16                      child_node_num;             // 子菜单的节点数 */
    uint16                      visible_child_node_num;     // 子菜单的可见节点数 */
    uint32                      node_id;                    // 菜单项ID */
	DYNA_ITEM_ICON_TYPE_E       select_icon_type;           // 菜单项图标类型 */
    CTRLMENU_SELECT_ICON_DATA_T select_icon_data;           // 菜单项图标数据 */
    MMI_IMAGE_ID_T              title_icon_id;              // 菜单项图标 */
    MMI_STRING_T                title_str;                  // TITLE文本
    MMI_STRING_T                sub_title_str;              // SUB TITLE文本
    GUIMENU_BUTTON_STYLE_E      button_style;               // button style
    struct ctrlmenu_node_tag    *child_node_ptr;            // 指向子菜单 */
    struct ctrlmenu_node_tag    *parent_node_ptr;           // 指向父菜单 */
    struct ctrlmenu_node_tag    *neighbour_node_ptr;        // 指向兄弟菜单 */
    struct ctrlmenu_node_tag    *next_free_node_ptr;        // 串联暂时不用的节点 */
} CTRLMENU_NODE_T;

// dynamic menu node memory
typedef struct
{
    uint16              free_node_num;  //free node number
    CTRLMENU_NODE_T      *free_node_ptr; //free node pointer
    CTRLMENU_NODE_T      *init_mem_ptr;  //init memory pointer
} CTRLMENU_NODE_MEMORY_T;

// menu stack
typedef struct
{
    uint16              first_item_index;   //display item index at the top
    uint16              cur_item_index;     //selected item index
    int16               offset_y;           //
    GUI_RECT_T          rect;               //menu rect
    CTRLMENU_NODE_T     *parent_node_ptr;   //parent node
    MMI_MENU_GROUP_ID_T group_id;           //group id
} CTRLMENU_STACK_T;

// 存储pop-up radio/check menu的选中信息
//  双向不循环链表结构
typedef struct ctrlmenu_pop_sel_item_tag
{
    GUIMENU_POP_SEL_INFO_T              item_info;
    struct ctrlmenu_pop_sel_item_tag    *prev_ptr;
    struct ctrlmenu_pop_sel_item_tag    *next_ptr;
} CTRLMENU_POP_SEL_ITEM_T;

// pop-up radio/check menu选中信息链表
typedef struct
{
    CTRLMENU_POP_SEL_ITEM_T  *init_item_ptr;
    CTRLMENU_POP_SEL_ITEM_T  *last_item_ptr;
} CTRLMENU_POP_SELECT_LIST_T;

typedef struct ctrlmenu_static_item_tag
{
    MMI_MENU_GROUP_ID_T             group_id;
    MMI_MENU_ID_T                   menu_id;
    MMI_STRING_T                    text_str;
    MMI_IMAGE_ID_T                  icon_id;
    struct ctrlmenu_static_item_tag *next_item_ptr;
} CTRLMENU_STATIC_ITEM_T;

typedef struct
{
    //BOOLEAN                     is_on_slide_paint;  //is on slide paint
    //uint8                       slide_timer_id;     //slide timer id
    float                       fling_velocity;     //fling velocity
    GUI_POINT_T                 start_point;        //slide start point
    GUI_POINT_T                 prev_point;         //slide previous point
    MMK_TP_SLIDE_E              slide_state;        //slide state
    MMK_TP_SLIDE_E              tpdown_slide_state; //tp down slide state
} CTRLMENU_SLIDE_INFO_T;

//menu stack
typedef struct
{
    uint16              num_index;  //number index
    MMI_MESSAGE_ID_E    key_msg;    //number key message
} CTRLMENU_NUMKEY_MAP_T;

//menu control
typedef struct _menu_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_static;      //is static menu
    BOOLEAN                     is_active;      //is active
    BOOLEAN                     is_transparent; //是否半透背景
    BOOLEAN                     is_reset_rect;  //是否需要重设区域

    BOOLEAN                     is_need_prgbox; //is need prgbox
    BOOLEAN                     is_revise_rect; //is need revise rect or not，当增加滚动条的时候，需要重新调整区域
    BOOLEAN                     is_first_disp;  //是否第一次显示，在特效时使用；防止重新获得焦点时再次显示特效
    
    BOOLEAN                     is_move_state;  //item是否在抖动
    BOOLEAN                     is_item_pressed;//tp down时，是否在item上
    BOOLEAN                     is_tp_pressed;  //is tp down?

    BOOLEAN                     is_set_font_color;  //is set font color
    GUI_COLOR_T                 font_color;         //font color

    uint8                       redrew_timer_id;// 滑动时，slide和fling状态下，画图的timer
    uint8                       item_timer_id;  //item timer id
    uint16                      item_str_index; //item string start index
    uint16                      menu_level;     //menu level

    int16                       offset_y;           //offset y
    uint16                      cur_item_index;     //current selected menu index
    uint16                      item_total_num;     //current total visible item number
    uint16                      first_item_index;   //item index at the top
    uint16                      end_item_index;     //item index at the top

    uint16                      pre_msg_loop_item_index;    // 每次消息循环后，focus项index
    uint16                      pre_msg_loop_menu_level;    // 每次消息循环后，focus项的level

    GUIMENU_STYLE_E             cur_style;      //menu style
    CTRLMENU_STATE_T            menu_state;     //menu state

    wchar                       title_wstr[CTRLMENU_TITLE_MAX_LEN + 1]; //for dynamic menu or volatile static menu title
    uint16                      title_len;                              //for dynamic menu or volatile static menu title

    MMI_MENU_GROUP_ID_T         cur_group_id;           //current menu group id
    GUIMENU_GROUP_T             *cur_group_ptr;         //current menu group info
    CTRLMENU_GROUP_INFO_T       *cur_group_info_ptr;    //current additional menu group info
    CTRLMENU_GROUP_INFO_T       *root_group_info_ptr;   //additional menu group info memory
    CTRLMENU_STATIC_ITEM_T      *static_item_ptr;       //text and image for static item

    CTRLMENU_NODE_MEMORY_T      node_memory;            //dynamic menu,node memory
    CTRLMENU_NODE_T             *root_node_ptr;         //dynamic menu,root node pointer
    CTRLMENU_NODE_T             *cur_parent_node_ptr;   //dynamic menu,current parent node pointer

    uint16                      max_horz_width;     // 菜单的横向最大宽度，用于popupmenu_auto动态计算宽度时用
    uint16                      max_vert_width;     // 菜单的纵向最大宽度，用于popupmenu_auto动态计算宽度时用
    GUIMENU_EP_E                effective_point;    // 弹出的基点，用于popup_auto风格的菜单

    CTRLMENU_ITEM_POS_INFO_T    *item_pos_info_ptr; // 记录Item位置信息
    GUI_LCD_DEV_INFO            item_layer;         // 二级菜单画item的层信息

    // options menu
    GUI_LCD_DEV_INFO            popup_layer_handle;         // 动画用
    GUI_LCD_DEV_INFO            mask_layer_handle;          // 动画用
    CTRLMENU_OPTIONS_ITEM_T     *options_item_info_ptr;     // item显示信息
    uint16                      *item_len_array_ptr;        // 一行的长度与item的长度的比值
    uint16                      *num_in_line_ptr;           // 每行显示的个数
    uint16                      line_count;                 // 行数
    uint16                      *temp_invisible_index_ptr;  // 暂时被隐藏的项索引
    uint16                      temp_invisible_num;         // 临时被隐藏的项数
    uint16                      dis_num_in_options_menu;    // options一级菜单页面显示的个数

    // context menu
    GUI_RECT_T                  title_rect;                         // title区域
    GUI_RECT_T                  button_rect;                        // button区域
    GUI_RECT_T                  bg_rect;                            // item区域
    GUIMENU_BUTTON_STYLE_E      cur_button_style;                   // button风格
    MMI_HANDLE_T                button_handle[CTRLMENU_BUTTON_NUM]; // button控件

    MMI_TEXT_ID_T               dyna_softkey_id[GUISOFTKEY_BUTTON_NUM]; //dynamic menu,softkey text id
    CTRLMENU_STACK_T            menu_stack[CTRLMENU_STACK_MAX_LEVEL];    //menu stack info

    IGUICTRL_T                  *prgbox_ctrl_ptr;   //progress box control pointer

    GUI_RECT_T                  writeback_rect; //双buffer回写
    CTRLMENU_POP_SELECT_LIST_T  selected_list;  //all selected item info only for pop-up radio/check menu
    CTRLMENU_SLIDE_INFO_T       slide_info;     //slide info

    MMITHEME_MENU_COMMON_T      common_theme;   //menu common theme
    MMITHEME_SECONDMENU_T       sec_theme;      //second menu theme
    MMITHEME_POPMENU_T          pop_theme;      //pop-up menu theme
    MMITHEME_OPTIONSMENU_T      options_theme;  // options menu theme
    
    MMI_HANDLE_T                win_handle;         //window handle
} CTRLMENU_OBJ_T;

//menu class for public or protected method
typedef struct _menu_vtbl_tag
{
    CTRLBASE_VTBL_T     base_ctrl_vtbl;

} CTRLMENU_VTBL_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
PUBLIC GUI_COLOR_T CTRLMENU_GetItemFontColor(
                                           CTRLMENU_OBJ_T*      menu_ctrl_ptr,  // [in]
                                           uint16               item_index      // [in]
                                           );

/*****************************************************************************/
//  Description : get menu type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MENU_GetType(void);

/*****************************************************************************/
//  Description : get menu pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_OBJ_T* CTRLMENU_GetPtr(
                                       MMI_HANDLE_T   ctrl_handle
                                       );

/*****************************************************************************/
//  Description : init menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Init( 
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : creat one new node
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_CreatNewNode(
                                              CTRLMENU_NODE_MEMORY_T    *node_mem_ptr
                                              );

/*****************************************************************************/
//  Description : display menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Draw(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : destroy menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Destroy(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetStatePtr( 
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                    uint32          src_state
                                    );

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetStatePtr(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                 uint32         src_state,
                                 BOOLEAN        is_true
                                 );

/*****************************************************************************/
//  Description : is pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsPopMenu(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  );
/*****************************************************************************/
//  Description : is second menu
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsSecMenu(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  );
/*****************************************************************************/
//  Description : get pop sel info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetPopSelInfo(
                                      CTRLMENU_OBJ_T            *menu_ctrl_ptr, //[in]
                                      uint16                    item_index,     //[in]
                                      GUIMENU_POP_SEL_INFO_T    *item_info_ptr  //[out]
                                      );

/*****************************************************************************/
//  Description : get node pointer by visible index
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_GetNodeByVisibleIndex(
                                                       uint16           node_index,      //[in]
                                                       CTRLMENU_NODE_T  *parent_node_ptr //[in]
                                                       );

/*****************************************************************************/
//  Description : set pop-up menu item status for radio or check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetPopItemStatus(
                                         GUIMENU_POP_SEL_INFO_T     *selected_info_ptr
                                         );

/*****************************************************************************/
//  Description : get item text string, icon id, and is_grayed state
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
//        if it is option page style, get the item in page.
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetItem(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr, // [in]
                                uint16          item_index,     // [in]
                                MMI_STRING_T    *text_ptr,      // [out]
                                MMI_IMAGE_ID_T  *icon_id_ptr,   // [out]
                                BOOLEAN         *is_grayed_ptr  // [out]
                                );

/*****************************************************************************/
//  Description : get item text string, icon buffer data, and is_grayed state
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:   this method is for dynamic menu
//			只适用于item icon为buffer的情况。
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetItemForIconBuffer(
											 CTRLMENU_OBJ_T					*menu_ctrl_ptr, 	// [in]
											 uint16							item_index,     	// [in]
											 MMI_STRING_T					*text_ptr,      	// [out]
											 CTRLMENU_SELECT_ICON_DATA_T	*icon_buffer_ptr,   // [out]
											 BOOLEAN						*is_grayed_ptr  	// [out]
                                );

/*****************************************************************************/
//  Description : get menu font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLMENU_GetFontColor(
                                         CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StartItemTextTimer(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : stop item text timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StopItemTextTimer(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : get title text
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetTitleText(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr, //[in]
                                     MMI_STRING_T       *title_str_ptr, //[out]
                                     MMI_STRING_T       *sub_title_str_ptr, // [out]
                                     MMI_IMAGE_ID_T     *title_icon_id_ptr  // [out]
                                     );

/*****************************************************************************/
//  Description : set menu softkey param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSoftkey(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_NotifyMsg(
                                       MMI_HANDLE_T       ctrl_handle,
                                       MMI_MESSAGE_ID_E   msg_id
                                       );

/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* CTRLMENU_GetPopLcdDev(
                                               CTRLMENU_OBJ_T     *menu_ctrl_ptr,     //[in]
                                               BOOLEAN            *is_common_lcd      // [out]
                                               );

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 判断层是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsLayerValid(
                                     GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                                     );
#endif

/*****************************************************************************/
//  Description : current menu is have child menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view.
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsExistChildMenu(                               // [ret] TRUE when has child menu
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr, // [in]
                                         uint16         visible_index,  // [in]
                                         BOOLEAN        *is_grayed_ptr  // [out] may PNULL
                                         );

/*****************************************************************************/
//  Description : calculate menu rect for pop-up,options
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_CalculateRect(
                                   uint16           rect_top,       //only for sub pop-up menu
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get group info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu
/*****************************************************************************/
PUBLIC CTRLMENU_GROUP_INFO_T* CTRLMENU_GetGroupInfo(
                                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr, //[in]
                                                    MMI_MENU_GROUP_ID_T group_id        //[in]
                                                    );

/*****************************************************************************/
//  Description : check button style
//  Global resource dependence : 
//  Author: hua.fang
//  Note: button指的是context menu底下的按钮
/*****************************************************************************/
PUBLIC void CTRLMENU_CheckButtonStyle(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : set menu info,include menu level,current and first index etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetInfo(
                             uint16                 menu_level,
                             uint16                 first_item_index,
                             uint16                 cur_item_index,
                             GUI_RECT_T             menu_rect,
                             int16                  offset_y,
                             MMI_MENU_GROUP_ID_T    cur_group_id,
                             CTRLMENU_NODE_T        *cur_parent_node_ptr,
                             CTRLMENU_OBJ_T         *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : reset menu, stop timer and free resource etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ResetInfo(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : find node pointer by index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_FindNodeByIndex(
                                                 uint16             node_index,
                                                 CTRLMENU_NODE_T    *parent_node_ptr
                                                 );

/*****************************************************************************/
//  Description : 更新下面的窗口
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_UpdatePrevWindow(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr  // [in]
                                      );

/*****************************************************************************/
//  Description : get item information
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetInfoInOK(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    BOOLEAN             *is_gray_ptr,
                                    BOOLEAN             *is_exist_child_ptr,
                                    GUIMENU_ITEM_T      *cur_item_ptr,
                                    CTRLMENU_NODE_T     **cur_node_pptr
                                    );

/*****************************************************************************/
//  Description : restore menu info in ok
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_RestoreInfoInOK(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                     GUIMENU_ITEM_T     *cur_item_ptr,
                                     CTRLMENU_NODE_T    *cur_node_ptr
                                     );

/*****************************************************************************/
//  Description : 重置经过cancel后menu数据
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_RestoreInfoInCancel(
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : get current item pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: this method is for static menu style
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetCurItem(
                                   CTRLMENU_OBJ_T       *menu_ctrl_ptr, //in
                                   GUIMENU_ITEM_T       *item_ptr,      //out
                                   CTRLMENU_ITEM_INFO_T *item_info_ptr  //out
                                   );

/*****************************************************************************/
//  Description : get current group id
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu style
/*****************************************************************************/
PUBLIC MMI_MENU_GROUP_ID_T CTRLMENU_GetCurGroupId(
                                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                                  );

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_AdjustFirstAndDisplay(
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                           BOOLEAN          is_need_update
                                           );

/*****************************************************************************/
//  Description : set first and current item index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetCurAndFirstItemIndex(
                                                CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                                uint16              line_num_page,
                                                MMI_MESSAGE_ID_E    msg_id
                                                );

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StopRedrawTimer(
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StartRedrawTimer(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InitSec(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : second menu handle special msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecSpecial(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr, 
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawSec(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : handle second menu ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleOk(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    MMI_MESSAGE_ID_E    msg_id
                                    );

/*****************************************************************************/
// 	Description : 销毁菜单，释放内存
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_DestroySec(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecAdjustFirstAndDisplay(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                              BOOLEAN           is_need_update
                                              );

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecHandleUpDown(
                                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                             MMI_MESSAGE_ID_E   msg_id
                                             );

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecResetDisplayItem(
                                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                            int16             *offset_y_ptr,
                                            BOOLEAN           is_true
                                            );

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleNum(
                                     uint16         num_index,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle menu touch panel press down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpDown(
                                     GUI_POINT_T        *tp_point_ptr,
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle menu touch panel press up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleTpUp(
                                      GUI_POINT_T       *tp_point_ptr,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : handle menu tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpMove(
                                     GUI_POINT_T    *tp_point_ptr,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle menu scroll bar touch panel press msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpScroll(
                                       uint16           first_item_index,
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : handle redraw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecRedrawTimer(
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            );
#endif

/*****************************************************************************/
//  Description : display scroll item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecScrollItemStr(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : update previous second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_SecModifyRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InitPop(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : pop-up menu handle special msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopSpecial(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr, 
                                        MMI_MESSAGE_ID_E    msg_id
                                        );

/*****************************************************************************/
//  Description : display menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawPop(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopAdjustFirstAndDisplay(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                              BOOLEAN           is_need_update
                                              );

/*****************************************************************************/
//  Description : calculate pop-up menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_CalcPopupRect(
                                   uint16           rect_top,       //only for sub pop-up menu
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : update previous pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  当前是pop-up menu,lose focus后get focus需要刷新之前所有的pop-up menu
//         当前是pop-up menu,cancel后需要根据Rect判断是否刷新之前的pop-up menu
/*****************************************************************************/
PUBLIC void CTRLMENU_UpdatePrevPop(
                                   uint16           start_level,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );
#endif

/*****************************************************************************/
//  Description : handle OK key
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |-----------------------------------------------------------------|
//    |      event     |    Popup(More item)      |   Popup_Auto        |
//    |-----------------------------------------------------------------|
//    |OK(menu key)    |    close                 |   none              |
//    |-----------------------------------------------------------------|
//    |Web             |    has active: OK        |   the same as Popup |
//    |                |    not active: none      |                     |
//    |-----------------------------------------------------------------|
//    |back            |    back to Options       |   close             |
//    |-----------------------------------------------------------------|
//    |tp              |    in client: OK         |   in client: OK     |
//    |                |    not: close            |   not : none        |
//    |-----------------------------------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleOk(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    MMI_MESSAGE_ID_E    msg_id
                                    );

/*****************************************************************************/
//  Description : handle cancel key
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |-----------------------------------------------------------------|
//    |      event     |    Popup(More item)      |   Popup_Auto        |
//    |-----------------------------------------------------------------|
//    |OK(menu key)    |    close                 |   none              |
//    |-----------------------------------------------------------------|
//    |Web             |    has active: OK        |   the same as Popup |
//    |                |    not active: none      |                     |
//    |-----------------------------------------------------------------|
//    |back            |    back to Options       |   close             |
//    |-----------------------------------------------------------------|
//    |tp              |    in client: OK         |   in client: OK     |
//    |                |    not: close            |   not : none        |
//    |-----------------------------------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleCancel(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                        MMI_MESSAGE_ID_E    msg_id
                                        );

/*****************************************************************************/
//  Description : destroy popmenu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DestroyPop(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get menu current item top value
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetCurItemTop(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle pop menu up/down key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopHandleUpDown(
                                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                             MMI_MESSAGE_ID_E   msg_id
                                             );

/*****************************************************************************/
//  Description : handle pop menu left key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleLeft(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : handle pop menu right key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleRight(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleNum(
                                     uint16         num_index,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle menu touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopHandleTpDown(
                                     GUI_POINT_T    *tp_point_ptr,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle menu touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleTpUp(
                                      GUI_POINT_T       *tp_point_ptr,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : handle menu touch panel press down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopHandleTpMove(
                                     GUI_POINT_T        *tp_point_ptr,
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle menu scroll bar touch panel press msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopHandleTpScroll(
                                       uint16           first_item_index,
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopRedrawTimer(
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            );
#endif

/*****************************************************************************/
//  Description : display scroll item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopScrollItemStr(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : update previous pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  当前是pop-up menu,旋转后需要刷新之前所有的pop-up menu
/*****************************************************************************/
PUBLIC void CTRLMENU_PopModifyRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

#ifdef MMI_OPTIONMENU_SUPPORT
/*****************************************************************************/
//  Description : optionsmenu 初始化
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InitOptions(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_OptionsSpecial(
                                            CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                            MMI_MESSAGE_ID_E    msg_id
                                            );

/*****************************************************************************/
//  Description : 更新布局数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 现在区域包括:
//  1、顶上的阴影区
//  2、周围的边线
//  3、中间的item
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsLayout(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : optionsmenu 显示
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawOptions(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : handle OK key
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |--------------------------------------|
//    |key             |    Options          |
//    |--------------------------------------|
//    |OK(menu key)    |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|

//    |Web             |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|
//    |back            |    close            |
//    |--------------------------------------|
//    |tp              |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleOk(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                        MMI_MESSAGE_ID_E    msg_id
                                        );

/*****************************************************************************/
//  Description : 处理上下键消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_OptionsHandleUpDown(
                                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                                 BOOLEAN        is_up
                                                 );

/*****************************************************************************/
//  Description : handle key left
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleLeft(
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : handle key right
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleRight(
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                           );

/*****************************************************************************/
//  Description : handle number key
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleNum(
                                         uint16         num_index,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 处理tp down消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsHandleTpDown(
                                         GUI_POINT_T    *tp_point_ptr,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : 处理tp up消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleTpUp(
                                          GUI_POINT_T       *tp_point_ptr,
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : 处理tp move消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsHandleTpMove(
                                         GUI_POINT_T    *tp_point_ptr,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );
#endif

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsModifyRect(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       );
#endif

/*****************************************************************************/
//  Description : optionsmenu与moremenu之间的切换
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SwitchOptions(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                      BOOLEAN           is_optionsmenu
                                      );

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_HandleAnimMsg(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                   MMI_MESSAGE_ID_E msg_id
                                   );

/*****************************************************************************/
//  Description : reset layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ResetAnimLayer(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : move down
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_MoveDown(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              BOOLEAN           is_down
                              );

/*****************************************************************************/
//  Description : zoom anim
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Zoom(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                          BOOLEAN           is_zoom_out
                          );
#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
