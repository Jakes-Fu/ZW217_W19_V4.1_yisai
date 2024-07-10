/*****************************************************************************
** File Name:      ctrltab_export.h                                        *
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

#ifndef _CTRLTAB_EXPORT_H_
#define _CTRLTAB_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmk_app.h"
#include "guifont.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guianim.h"
#include "guiprgbox.h"
#include "mmitheme_tab.h"

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
// default max item name length
/*! @def GUITAB_MAX_ITEM_NAME_LENGTH
@brief Item显示文字的最大长度
*/
#define GUITAB_MAX_ITEM_NAME_LENGTH     32
/*! @def GUITAB_MAX_ITEM_NUM
@brief TAB Item 最大项数
*/
#define GUITAB_MAX_ITEM_NUM             20

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUITAB_TIMER_STATE_T
@brief 保留状态，系统未使用
*/
typedef uint32 GUITAB_TIMER_STATE_T;
// timer status
/*! @def GUITAB_TIMER_NULL
@brief 保留状态，系统未使用
*/
#define GUITAB_TIMER_NULL					0X0000
/*! @def GUITAB_TIMER_LEFT_SCROLL
@brief 保留状态，系统未使用
*/
#define GUITAB_TIMER_LEFT_SCROLL			0X0001
/*! @def GUITAB_TIMER_RIGHT_SCROLL
@brief 保留状态，系统未使用
*/
#define GUITAB_TIMER_RIGHT_SCROLL			0X0002

/*! @typedef GUITAB_ITEM_STATE_T
@brief Item State
*/
typedef uint32 GUITAB_ITEM_STATE_T;
/*! @def GUITAB_ITEMSTATE_NULL
@brief null 状态
*/
#define GUITAB_ITEMSTATE_NULL           0x00000000
/*! @def GUITAB_ITEMSTATE_NEED_CONTENT
@brief 需要内容，动态加载item使用
*/
#define GUITAB_ITEMSTATE_NEED_CONTENT   0x00000001  // need item content
/*! @def GUITAB_ITEMSTATE_SHOW_IMG
@brief item只显示显示图片
*/
#define GUITAB_ITEMSTATE_SHOW_IMG       0x00000002  // only show image in item rectangle
/*! @def GUITAB_ITEMSTATE_SHOW_TEXT
@brief item只显示文本
*/
#define GUITAB_ITEMSTATE_SHOW_TEXT      0x00000004  // only show text in item rectangle
/*! @def GUITAB_ITEMSTATE_INVALID
@brief 无效的item，设置无效时，item不显示
*/
#define GUITAB_ITEMSTATE_INVALID        0x00000008  // is invalid item
/*! @def GUITAB_ITEMSTATE_UIMG_DTXT
@brief item显示上图下文，默认左图右文
*/
#define GUITAB_ITEMSTATE_UIMG_DTEXT		0x00000010  // item显示上图下文，默认左图右文

// define state
/*! @typedef GUITAB_STATE_T
@brief TAB控件状态
*/
typedef uint32 GUITAB_STATE_T;

/*! @def GUITAB_STATE_NULL
@brief null状态
*/
#define GUITAB_STATE_NULL               0x00000000  // no state
// text, item, tab status
/*! @def GUITAB_STATE_DISABLE
@brief 系统保留
*/
#define GUITAB_STATE_DISABLE            0x00000001  // disable or not
// item, tab status
/*! @def GUITAB_STATE_NONEED_SCROLL
@brief tab不显示滚动箭头，默认为显示
*/
#define GUITAB_STATE_NONEED_SCROLL      0x00000010  // need scroll card or not

/*! @def GUITAB_STATE_SETTING_ITEM_DATA
@brief 设置DATA，系统保留
*/
#define GUITAB_STATE_SETTING_ITEM_DATA  0x00000100  // is setting item content, 设置 add_data_ptr 参数,记录动画内容.
/*! @def GUITAB_STATE_ACTIVE
@brief 系统保留
*/
#define GUITAB_STATE_ACTIVE             0x00000200  // active or not
/*! @def GUITAB_STATE_FOCUS
@brief 系统保留
*/
#define GUITAB_STATE_FOCUS              0x00000400  // on focus or not
/*! @def GUITAB_STATE_INVALID
@brief 系统保留
*/
#define GUITAB_STATE_INVALID            0x00000800  // invalid or not
/*! @def GUITAB_STATE_AUTO_SWITCH
@brief 自动切换item，目前是系统保留状态
*/
#define GUITAB_STATE_AUTO_SWITCH        0x00001000  // automatically switch to next item
/*! @def GUITAB_STATE_SINGLE_LINE
@brief 当行显示，只显示item，不显示title，默认时，显示title
*/
#define GUITAB_STATE_SINGLE_LINE        0x00002000  // display on single line, |item image|_focus item text_|item image|
/*! @def GUITAB_STATE_SYNC_DECODE_ANIM
@brief 是否同步解码，默认时同步
*/
#define GUITAB_STATE_SYNC_DECODE_ANIM   0x00004000  // tab上的Item动画是否需要同步显示（解码），默认情况下是同步的
/*! @def GUITAB_STATE_COMMON_TITLE
@brief tab是否显示统一的title，即tab title是否随item的切换而改变
*/
#define GUITAB_STATE_COMMON_TITLE       0x00008000  // TAB显示统一的TITLE

/*! @def GUITAB_STATE_IS_PAINT
@brief 当tab控件接收到full paint之后，会自动设置该状态，并且保持到控件被销毁
*/
#define GUITAB_STATE_IS_PAINT           0x00010000  // TAB已经刷新过

/*! @def GUITAB_STATE_IS_ADAPT_WIDTH
@brief 自适应区域，当item总宽度小于tab宽度时才会自适应
*/
#define GUITAB_STATE_IS_ADAPT_WIDTH     0x00020000  // 自适应ITEM区域，填充整个TAB区域

/*! @def GUITAB_STATE_SWITCH_ITEM
@brief 左右键是否切换tab页，当tp点击时，该状态自动设置为TRUE,默认值为TRUE
*/
#define GUITAB_STATE_SWITCH_ITEM		0x00040000  // 左右键是否切换tab页


#define GUITAB_STATE_MAX                0x08000000  // TAB状态的最大值
// 0x10000000之后的状态不要定义

// 图片的显示状态
/*! @typedef GUITAB_IMAGE_STATE_T
@brief 图片的显示状态，系统保留
*/
typedef uint32 GUITAB_IMAGE_STATE_T;
/*! @def GUITAB_IMAGE_NULL
@brief null状态，表示item中的图片还没有设置，系统保留
*/
#define GUITAB_IMAGE_NULL               0x00000000  // image is null, 表示item中的图片还没有设置
/*! @def GUITAB_IMAGE_FILLED
@brief 表示item中的图片数据已经设置了，系统保留
*/
#define GUITAB_IMAGE_FILLED             0x00000001  // image is filled, 表示item中的图片数据已经设置了
/*! @def GUITAB_IMAGE_INVALID
@brief 图片内容无效，系统保留
*/
#define GUITAB_IMAGE_INVALID            0x00000002  // image info is invalid
/*! @def GUITAB_IMAGE_NEED_SEL_BG_IMG
@brief 是否需要显示选中时的背景图片
*/
#define GUITAB_IMAGE_NEED_SEL_BG_IMG     0x00000004	//need select bg image


/*! @enum  GUITAB_SCROLL_STATE_E 
@brief  左右滚动卡的状态
*/
typedef enum
{
    GUITAB_SCROLL_CARD_STATE_NONE, /*!< 无*/
    GUITAB_SCROLL_CARD_PRESSED,    /*!< 按下*/
    GUITAB_SCROLL_CARD_RELEASED,   /*!< 弹起*/
    GUITAB_SCROLL_CARD_DISABLED,   /*!< 无效*/
    GUITAB_SCROLL_CARD_STATE_MAX   /*!< 系统保留*/
}GUITAB_SCROLL_STATE_E;

/*! @typedef GUITAB_SCROLL_STATE_T
@brief 左右滚动卡的状态
*/
typedef GUITAB_SCROLL_STATE_E GUITAB_SCROLL_STATE_T;

/*! @enum  GUITAB_SWITCH_TYPE_E 
@brief  切换item类型，向前切换还是向后切换
*/
typedef enum
{
    GUITAB_SWITCH_NONE,		    /*!< none*/
    GUITAB_SWITCH_PRE,		    /*!< 前一个*/
    GUITAB_SWITCH_NEXT,		    /*!< 后一个*/
    GUITAB_SWITCH_MAX			/*!< 系统保留*/
}GUITAB_SWITCH_TYPE_E;

/*! @enum  GUI_TAB_STYLE_E 
@brief 显示风格，系统保留，目前只支持靠左显示
*/
typedef enum
{
    GUI_TAB_ITEM_NONE,                          /*!< 无*/
    GUI_TAB_ITEM_LEFT, 		                    /*!< 靠左，默认*/
    GUI_TAB_ITEM_RIGHT = GUI_TAB_ITEM_LEFT, 	/*!< 靠右，未实现*/
    GUI_TAB_ITEM_MIDDLE = GUI_TAB_ITEM_LEFT,	/*!< 居中，未实现*/
    GUI_TAB_ITEM_MAX			                /*!< 系统保留*/
} GUI_TAB_STYLE_E;

/*! @enum  GUITAB_ITEM_DATA_TYPE_E 
@brief  item的数据类型
*/
typedef enum
{
    TABITEM_DATA_NONE,				/*!< none*/
    TABITEM_DATA_TEXT_ID,			/*!< 文本资源ID*/
	TABITEM_DATA_TEXT_BUFFER,		/*!< 文本buffer*/
    TABITEM_DATA_IMAGE_ID,		    /*!< 图片资源ID*/
    TABITEM_DATA_ANIM_ID,			/*!< 动画资源ID*/
    TABITEM_DATA_ANIM_DATA,		    /*!< 动画数据*/
    TABITEM_DATA_ANIM_PATH,		    /*!< 动画路径*/
	TABITEM_DATA_MAX,				/*!< 系统保留*/
}GUITAB_ITEM_DATA_TYPE_E;

/*! @struct  GUITAB_INIT_DATA_T 
@brief  初始化数据
*/
typedef struct 
{
    GUI_BOTH_RECT_T              both_rect;			    /*!< 横竖屏区域*/
    GUITAB_STATE_T               state;					/*!< tab状态*/
    GUI_TAB_STYLE_E              style;	                /*!< 对其方式 */
    uint32                       max_item_num;	        /*!< 最大项数，最大值为 ：\link #GUITAB_MAX_ITEM_NUM \endlink*/
    BOOLEAN                      is_active_item;        /*!< 无*/    // to load item actively

}GUITAB_INIT_DATA_T;

/*! @struct  GUITAB_NEED_ITEM_DATA_T 
@brief   控件和窗口之间交换数据时使用的item数据结构，即动态加载item所用 
*/
typedef struct
{
    MMI_HANDLE_T                handle;					/*!< tab控件的handle*/
    BOOLEAN                     is_child;				/*!< 是否通知儿子，系统保留，3D特效时使用*/
    BOOLEAN                     is_to_left;				/*!< 切换item的方向，系统保留，3D特效使用*/
    GUI_POINT_T                 point;					/*!< item的左上起始点*/
    uint32                      item_index;				/*!< 当前item索引号*/
    uint32                      pre_item_index;		    /*!< 切换到当前item时，前一个item索引号*/
}GUITAB_NEED_ITEM_DATA_T;

/*! @struct  GUITAB_ANIM_DISP_RESULT_T 
@brief   发送解析的结果给app
*/
typedef struct
{
    GUIANIM_RESULT_E            result;                 /*!< 动画解码结果*/
    uint32                      item_index;             /*!< item索引*/
}GUITAB_ANIM_DISP_RESULT_T;


/*! @struct GUITAB_TEXT_STYLE_T
@brief  text style
*/
typedef struct
{
    GUI_RECT_T				    rect;                   /*!< 显示区域*/
    uint8					    font_space; 		    /*!< 字间距*/
	GUI_FONT_T				    font_type;              /*!< 字号*/
	GUI_COLOR_T				    font_color;             /*!< 字体颜色*/
    GUI_ALIGN_E                 align;                  /*!< 排版方式*/
} GUITAB_TEXT_STYLE_T;

/*! @struct GUITAB_TEXT_T
@brief text buffer information
*/
typedef struct
{
    uint16					    wstr_len;	                            /*!< 文本长度*/
	wchar					    wstr[GUITAB_MAX_ITEM_NAME_LENGTH+1];    /*!< 文本buffer*/
}GUITAB_TEXT_T;

/*! @union GUITAB_TEXT_DATA_U
@brief text data union, text id or text buffer 
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;                /*!< text id*/
    GUITAB_TEXT_T               text_buffer;            /*!< text buffer*/
}GUITAB_TEXT_DATA_U;

/*! @struct GUITAB_CAPTION_T
@brief tab caption struct
*/
typedef struct
{
    BOOLEAN                     is_disp_font_bg;        /*!< 是否显示背景*/
    MMI_IMAGE_ID_T			    pressed_image_id; 		/*!< 滚动卡按下的图片ID*/
	MMI_IMAGE_ID_T			    released_image_id; 			/*!< 滚动卡释放的图片ID*/

    GUITAB_ITEM_DATA_TYPE_E     text_type; 				/*!< 文字类型*/
    GUITAB_TEXT_DATA_U          text;      				/*!< 文字信息*/
    GUITAB_TEXT_STYLE_T         text_style;				/*!< 文字风格*/
} GUITAB_CAPTION_T;

/*! @union GUITAB_IMAGE_DATA_U
@brief   image information union: id, data info, file info
*/
typedef union
{
    GUIANIM_DATA_INFO_T         anim_data;              /*!< 动画数据*/
    GUIANIM_FILE_INFO_T         anim_path;              /*!< 动画路径*/
}GUITAB_IMAGE_DATA_U;

/*! @struct GUITAB_IMAGE_DATA_T
@brief image info data
*/
typedef struct
{
    int16						comp_image_w;           /*!< 图片显示的压缩宽度*/
	int16						comp_image_h;           /*!< 图片显示的压缩高度*/
    GUITAB_IMAGE_STATE_T        state;                  /*!< 当item焦点时，是否显示焦点图片*/
    GUITAB_ITEM_DATA_TYPE_E     image_data_type;        /*!< id, anim data, anim path*/
    GUITAB_IMAGE_DATA_U         image_data_u;           /*!< 数据信息*/
}GUITAB_IMAGE_DATA_T;

/*! @struct  GUITAB_ITEM_CONTENT_T 
@brief   item content
*/
typedef struct
{
    GUITAB_CAPTION_T            text_data;              /*!< caption info*/
    MMI_IMAGE_ID_T			    sel_image_id;           /*!< 选中状态下时的图片*/
	MMI_IMAGE_ID_T			    bg_image_id;            /*!< 这个是item特有的背景图*/
    GUITAB_IMAGE_DATA_T         image_data;             /*!< icon card image*/
}GUITAB_ITEM_CONTENT_T;

/*! @struct _guitab_item_tag
@brief   tab item data
*/
typedef struct _guitab_item_tag
{
    GUITAB_ITEM_CONTENT_T*       item_data_ptr;         /*!< item数据内容*/
    uint32                       item_width;		    /*!< item宽度，控件自己计算*/
    GUI_RECT_T                   rect;		            /*!< item区域，控件自己计算*/
    GUITAB_ITEM_STATE_T          item_state;	        /*!< item状态*/
    ADD_DATA                     add_data_ptr;          /*!< 可记录anim控件数据，或其他用处*/
    MMI_HANDLE_T			     child_win_handle;      /*!< tab item对应的子窗口ID*/

    struct _guitab_item_tag      *next_ptr;             /*!< 链表指针*/
}GUITAB_ITEM_T;

/*! @struct GUITAB_ITEM_LIST_T
@brief  item list
*/
typedef struct  
{
    uint32			            item_num;		        /*!< item数*/
	uint32			            total_width;	        /*!< 所有item宽度*/
    GUITAB_ITEM_T	            *item_ptr;		        /*!< item指针*/
}GUITAB_ITEM_LIST_T;

/*! @struct GUITAB_SCROLL_CARD_T
@brief  scroll  card data 
*/
typedef struct 
{
   	GUI_RECT_T				    card_rect;              /*!< show rectangle of scroll card*/
    GUITAB_SCROLL_STATE_T       card_state;             /*!< card state*/
	MMI_IMAGE_ID_T			    pressed_image_id;       /*!< scroll card pressed image */
	MMI_IMAGE_ID_T			    released_image_id;      /*!< scroll card released image*/
	MMI_IMAGE_ID_T			    disabled_image_id;      /*!< scroll card disabled image*/
}GUITAB_SCROLL_CARD_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 设置最大Item数,这里分配内存,最大数目不能超过GUITAB_MAX_ITEM_NUM = 20
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param max_item_num [in] 最大tab项数
@retval true/false
@return 返回成功或失败
@note 这个函数主要用来申请内存，传入的最大数不要超过20，如果实在需要更多项，那么请同时修改GUITAB_MAX_ITEM_NUM的定义，并考虑内存
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetMaxItemNum(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 max_item_num
                                    );

/***************************************************************************//*!
@brief 增加Tab项对应的字窗口ID
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param child_win_handle [in] TAB项子窗口句柄或ID
@param pos [in] TAB项索引号
@retval true/false
@return 返回成功或失败
@note 每个tab项在切换时，都肯能显示自己的子窗口，当然这些子窗口可以是同一个窗口
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AddChildWindow(
                                     MMI_CTRL_ID_T     ctrl_id, 
                                     MMI_HANDLE_T      child_win_handle,
                                     uint32            pos
                                     );

/***************************************************************************//*!
@brief 简单的Append接口,只设置字符串和两个图片的ID,Item中其它数据不会被替换
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param text_ptr [in] 文本信息
@param sel_image_id [in] 选中时显示的图片
@param unsel_image_id [in] 未选中时显示的图片
@retval true/false
@return 返回成功或失败
@note 该函数对于简单的、只显示资源图片选项的控件增加TAB项时使用
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendSimpleItem(
                                       MMI_CTRL_ID_T     ctrl_id, 
                                       MMI_STRING_T *    text_ptr,
                                       MMI_IMAGE_ID_T    sel_image_id,
                                       MMI_IMAGE_ID_T    unsel_image_id
                                       );

/***************************************************************************//*!
@brief 在pos对应的位置插入一项
@author Great.Tian
@param ctrl_id [in] 控件ID
@param item_ptr [in] TAB项数据
@param pos [in] TAB项索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_InsertItem(
                                  MMI_CTRL_ID_T            ctrl_id, // control id
                                  const GUITAB_ITEM_T      *item_ptr,   // 指定的Item
                                  uint32                    pos     // 指定的位置, from 0 to total_item_num
                                  );

/***************************************************************************//*!
@brief 删除一个Item项
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param pos [in] TAB项索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteItem(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 uint32 pos
                                 );

/***************************************************************************//*!
@brief 删除所有的Item项
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@retval false/ture
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteAllItems(
                                     MMI_CTRL_ID_T ctrl_id
                                     );

/***************************************************************************//*!
@brief 获取控件的项数
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@retval 项数
@return 项数
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetItemNum(
                                MMI_CTRL_ID_T ctrl_id
                                );

/***************************************************************************//*!
@brief 修改pos对应的Item信息
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param text_ptr [in] 字符串指针
@param sel_image_id [in] 选中时图片资源ID
@param unsel_image_id [in] 未选中时图片资源ID
@param pos tab[in] 项索引
@retval true/false
@return 成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSimpleItem( 
                                    MMI_CTRL_ID_T ctrl_id,
                                    const MMI_STRING_T * text_ptr,
                                    MMI_IMAGE_ID_T sel_image_id,
                                    MMI_IMAGE_ID_T unsel_image_id,
                                    uint32 pos
                                    );

/***************************************************************************//*!
@brief 设置当前项的索引号为item_index
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param item_index [in] tab项索引
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetCurSel(
                                MMI_CTRL_ID_T ctrl_id, 
                                uint32 item_index
                                );

/***************************************************************************//*!
@brief  获得当前项的索引号
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@retval 当前项索引值
@return 返回当前项索引值
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetCurSel(
                               MMI_CTRL_ID_T ctrl_id
                               );

/*****************************************************************************/
//  Description : get current selected item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC GUITAB_ITEM_T  * CTRLTAB_GetCurItem(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 设置背景图片
@author  guanyong.zhang
@param ctrl_id [in] 控件ID
@param image_id [in] 背景图片资源ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundImage(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/***************************************************************************//*!
@brief 设置背景颜色,注意背景颜色和背景图片同时设置时背景图片优先显示
@author Lianxiang
@param ctrl_id [in] 控件ID
@param bg_color [in] 背景色
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundColor(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         GUI_COLOR_T bg_color
                                         );

/***************************************************************************//*!
@brief 设置文本的风格:字体\颜色\字间距等
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param font [in] 字体大小
@param font_color [in] 字体颜色
@param font_space [in] 字间距
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFontParam( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   GUI_FONT_T font,
                                   GUI_COLOR_T font_color,
                                   uint8 font_space
                                   );

/***************************************************************************//*!
@brief 设置附加显示信息
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param added_string [in] 附加字符串信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAddedString( 
                                     MMI_CTRL_ID_T     ctrl_id,
                                     GUITAB_CAPTION_T  added_string
                                     );

/***************************************************************************//*!
@brief  获取附加显示的数据信息
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@param added_string_ptr [in/out] 附加字符串信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAddedString( 
                                     MMI_CTRL_ID_T               ctrl_id,
                                     GUITAB_CAPTION_T *    added_string_ptr
                                     );

/***************************************************************************//*!
@brief  设置控件状态
@author Xiaoqing.Lu
@param ctrl_id [in] 控件ID
@param src_state [in] 状态
@param is_true [in] 设置或取消
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetState( 
                            MMI_CTRL_ID_T ctrl_id,
                            GUITAB_STATE_T src_state,
                            BOOLEAN is_true
                            );

/***************************************************************************//*!
@brief 获得控件的状态是否设置
@author Xiaoqing.Lu
@param ctrl_id [in] 控件ID
@param src_state [in]  状态
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetState(
                               MMI_CTRL_ID_T ctrl_id,
                               GUITAB_STATE_T src_state
                               );

/***************************************************************************//*!
@brief 设置控件的Item项的状态
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in]  索引号
@param src_state [in]  状态
@param is_true [in] 设置或取消
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetItemState( 
                                MMI_CTRL_ID_T ctrl_id,
                                uint32 item_index,
                                GUITAB_ITEM_STATE_T  src_state,
                                BOOLEAN is_true
                                );

/***************************************************************************//*!
@brief 获得Item的某种状态是否设置
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in]  索引号
@param src_state [in]  状态
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetItemState(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_index,
                                   GUITAB_ITEM_STATE_T src_state
                                   );

/***************************************************************************//*!
@brief 设置图片的显示状态
@author xiaoqing.lu
@param image_data_ptr [in] 图片信息指针
@param src_state [in] 状态
@param  is_true [in]设置或取消设置
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetImageState( 
                                 GUITAB_IMAGE_DATA_T* image_data_ptr,
                                 GUITAB_IMAGE_STATE_T  src_state,
                                 BOOLEAN is_true
                                 );

/***************************************************************************//*!
@brief 获得图片的显示状态
@author xiaoqing.lu
@param image_data_ptr [in] 图片信息指针
@param src_state [in]  状态
@retval true/false
@return 返回是否状态是否设置
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetImageState(
                                    const GUITAB_IMAGE_DATA_T* image_data_ptr,
                                    GUITAB_IMAGE_STATE_T src_state
                                    );

/***************************************************************************//*!
@brief 设置Item的内容
@author yelianna
@param ctrl_id [in] 控件ID
@param item_content_ptr [in] item 内容数据
@param  item_index [in]位置
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemContent(
                                     MMI_CTRL_ID_T                ctrl_id, // control id
                                     const GUITAB_ITEM_CONTENT_T *item_content_ptr,
                                     uint32                      item_index
                                     );

/***************************************************************************//*!
@brief 设置图标数据信息
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param img_ptr [in] 图片数据
@param start_point [in] 起始位置
@param index [in] 索引号
@retval true/false
@return 返回成功或失败
@note 设置非选中状态下的图标
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemImage(
                                  MMI_CTRL_ID_T ctrl_id,   // 控件ID
                                  GUITAB_IMAGE_DATA_T *img_ptr,  // 设置数据
                                  GUI_POINT_T start_point,
                                  uint32 index             // 索引号
                                  );

/***************************************************************************//*!
@brief  设置文本
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_ptr [in] 图片数据
@param index [in] 起始位置
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemText(
                                  MMI_CTRL_ID_T ctrl_id, 
                                  GUITAB_TEXT_T *text_ptr, 
                                  uint32 index
                                  );

/***************************************************************************//*!
@brief 设置是否自动切换, 如果is_true为1,则自动切换的间隔为timer
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param timer [in] 时钟启动的时间间隔
@param is_true [in] 是否设置自动切换
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAutoSwitch(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 timer, 
                                    BOOLEAN is_true
                                    );

/***************************************************************************//*!
@brief 获得是否自动切换Item,返回TRUE,则timer参数是自动切换的间隔
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param timer [in/out] 时钟启动的时间间隔
@retval true/false
@return 是否切换
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAutoSwitch(
                                    MMI_CTRL_ID_T ctrl_id,
                                    uint32 *timer
                                    );

/***************************************************************************//*!
@brief 切换到下一个Item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param type [in] 切换类型
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SwitchToNextItem(
                                       MMI_CTRL_ID_T ctrl_id,
                                       GUITAB_SWITCH_TYPE_E type
                                       );

/***************************************************************************//*!
@brief Append 一个Item,和GUITAB_AppendSimpleItem的区别就是该接口会替换原有Item的所有数据
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_ptr [in] item数据
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendItem(
                                 MMI_CTRL_ID_T         ctrl_id,
                                 const GUITAB_ITEM_T   *item_ptr
                                 );

/***************************************************************************//*!
@brief set space between two items
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_space [in] 间距
@param is_use_default [in] 是否使用默认值
@retval true/false
@return 返回成功或失败
@note is_use_default: 是否使用缺省值，TRUE:使用GUITAB_DEFAULT_ITEM_SPACE = 5；
                  FALSE:设置值为MIN(item_space, GUITAB_DEFAULT_ITEM_SPACE)
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemSpace(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_space,
                                   BOOLEAN is_use_default
                                   );

/***************************************************************************//*!
@brief 设置TAB项的宽度,如果不设置将取缺省值
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param width [in] 图片资源ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemWidth(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 width
                                   );

/***************************************************************************//*!
@brief 设置tab控件是否循环显示。返回设置之前的状态
@author Lianxiang
@param ctrl_id [in] 控件ID
@param is_rotative [in] 是否循环切换
@retval true/false
@return 返回成功或失败
@note 循环显示说明可以从在最后一个Item和第一个Item之间直接切换.
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTabRotative(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     BOOLEAN is_rotative
                                     );

/***************************************************************************//*!
@brief 设置Tab控件的默认图片ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param def_img_id [in] 默认显示图片
@param err_img_id [in] 解析错误的图片
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDefaultIcon(
                                     MMI_CTRL_ID_T     ctrl_id, //in
                                     MMI_IMAGE_ID_T    def_img_id,  // default image id
                                     MMI_IMAGE_ID_T    err_img_id   // error image id
                                     );

/***************************************************************************//*!
@brief set the tab item valid or not
@author Xiaoqing.Lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引
@param is_valid [in] 是否有效
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemValid( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_index,
                                   BOOLEAN is_valid
                                   );


/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] 控件ID
@param bg_color [in] 背景色
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemBgColor(
                                          MMI_CTRL_ID_T ctrl_id, 
                                          GUI_COLOR_T bg_color
                                          );


/***************************************************************************//*!
@brief 设置焦点项的文字背景图
@author Lianxiang
@param ctrl_id [in] 控件ID
@param image_id [in] 资源图片ID
@param item_index [in] 索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemTextBgImage(
                                              MMI_CTRL_ID_T ctrl_id, 
                                              MMI_IMAGE_ID_T image_id,
                                              uint32 item_index
                                              );


/***************************************************************************//*!
@brief set 开始位置的item index
@author  xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetStartIndex(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 item_index
                                    );


/***************************************************************************//*!
@brief 获得开始位置的item index
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 获得起始项索引号
@return 获得起始项索引号
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetStartIndex(
                                   MMI_CTRL_ID_T ctrl_id
                                   );


/***************************************************************************//*!
@brief 设置显示动画的时候是否同步解码，默认是异步解码
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_sync_decode [in] true/false
@retval true/false
@return 返回成功或失败
@note 返回设置之前的状态
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSyncDecodeAnim(
                                        MMI_CTRL_ID_T  ctrl_id,
                                        BOOLEAN is_sync_decode
                                        );


/***************************************************************************//*!
@brief set the item's default width
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_height [in] tab项高度
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemDefaultHeight(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint32 item_height
                                          );


/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] 控件ID
@param image_id [in] 图片资源ID
@param is_update [in] 是否立即刷新
@retval void
@return void
@note 当item处于焦点状态时，会显示焦点状态的背景，调用该接口可改变焦点背景图片
*******************************************************************************/
PUBLIC void CTRLTAB_SetItemSelBgImage(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     MMI_IMAGE_ID_T image_id,
                                     BOOLEAN is_update
                                     );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param str_ptr [in]  字符串
@param is_set [in] 设置或取消设置，当为false时，str_ptr可以为NULL
@retval true/false
@return 返回成功或失败
@note if is_set = TRUE, set title; else if is_set = FALSE, release title
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleInfo(
                                   MMI_CTRL_ID_T ctrl_id,
                                   MMI_STRING_T *str_ptr,
                                   BOOLEAN is_set
                                   );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_id [in] 字符资源ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleTextId(
                                     MMI_CTRL_ID_T ctrl_id,
                                     MMI_TEXT_ID_T text_id
                                     );


/***************************************************************************//*!
@brief set title icon
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param icon_id [in] 图片资源ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleIcon(
                                   MMI_CTRL_ID_T ctrl_id,
                                   MMI_IMAGE_ID_T icon_id
                                   );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDividingLine(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTpDownBg(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetIsDisplayPrg(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         BOOLEAN is_display_prg
                                         );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
