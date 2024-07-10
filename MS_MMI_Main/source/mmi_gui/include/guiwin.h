/*! @file guiwin.h
@brief   此文件描述了gui win各类接口和结构体
@author  liqing.peng
@version  1.0
@date    01/05/2009 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note    01/05/2009     liqing.peng		 Create
*******************************************************************************/

/*! @page page_Win Win

(暂无)

*******************************************************************************/

/*! @addtogroup winGrp Win
@brief   此文件描述了gui win各类接口和结构体
@details 此文件对窗口控件元素进行封装(如状态栏，softkey，title)
@{
*******************************************************************************/

#ifndef _GUI_WIN_H_
#define _GUI_WIN_H_ 

/*--------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guistatusbar.h"
#include "guistatusbar_data.h"
#include "guititle.h"
#include "guisoftkey.h"
#ifdef GUIF_SCROLLKEY
#include "guiscrollkey.h"
#endif
#include "ctrltitle_export.h"
/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/
        
/*--------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                  */
/*--------------------------------------------------------------------------*/

/*! @typedef GUIWIN_STBDATA_APP_HANDLE_FUNC
@brief   Item 处理的回调，例如可以通过此接口打开对应模块接口等
@note    如触笔点击item是可以通过此接口指向相应的函数，以调用接口
*/
typedef BOOLEAN(*GUIWIN_STBDATA_APP_HANDLE_FUNC)(void);

                                                  
/*! @enum  GUIWIN_STBDATA_ICON_STYLE_E 
@brief  item show style
@note   状态栏显示类型的枚举
*/
typedef enum
{
  GUIWIN_STBDATA_ICON_NONE = GUISTBDATA_ICON_NONE,     /*!<无icon显示*/
  GUIWIN_STBDATA_ICON_NORMAL = GUISTBDATA_ICON_NORMAL, /*!<icon显示为一般图片*/
  GUIWIN_STBDATA_ICON_ANIM = GUISTBDATA_ICON_ANIM,     /*!<icon显示为动画*/
  GUIWIN_STBDATA_ICON_WRE = GUISTBDATA_ICON_WRE,        /*!< wre后台AP图标,从文件中加载图片*/ //add for wre_background by heng.xiao
  GUIWIN_STBDATA_ICON_MAX = GUISTBDATA_ICON_MAX        
}GUIWIN_STBDATA_ICON_STYLE_E;

/*! @struct GUIWIN_STBDATA_TEXT_T
@brief  文本信息
@note   状态栏文本显示类型数据结构，状态栏既可以显示图片也可以显示文本,
        或者都显示,当设置为无效的时候，不显示文本
*/
typedef struct
{
    BOOLEAN         is_display;   /*!<是否显示*/                         
    GUI_FONT_T      font_type;    /*!<字体风格*/
    GUI_COLOR_T     font_color;   /*!<字体颜色*/
    GUI_ALIGN_E     align;        /*!<排列方式*/
    wchar           wstr_ptr[MMISTATUSBAR_ITEM_TEXT_LEN]; /*!<字符串数组*/
    uint32          wstr_len;     /*!<字符串长度*/
} GUIWIN_STBDATA_TEXT_T;

/*! @struct GUIWIN_STBDATA_ICON_T
@brief 图标信息
@note  状态栏的icon状态数据结构
*/
typedef struct
{
    BOOLEAN                     is_display; /*!<是否显示*/
    uint32                      icon_num;   /*!<icon的数目*/
    uint32                      icon_array[MMISTATUSBAR_ITEM_ICON_FRAME];/*!<字符串数组*/
    GUIWIN_STBDATA_ICON_STYLE_E icon_type;  /*!<icon类型*/
} GUIWIN_STBDATA_ICON_T;

/*! @struct GUISTATUSBAR_OWNER_DRAW_T
@brief 控件自绘回调函数传出信息
*/
typedef struct 
{
    GUI_RECT_T                   display_rect;   /*!<显示区域*/
    MMI_HANDLE_T                 ctrl_handle;    /*!<控件handle*/
    GUI_LCD_DEV_INFO             lcd_dev;        /*!<lcd信息*/
} GUIWIN_STADATA_OWNER_DRAW_T;

/*! @typedef GUISTATUSBAR_OWNER_DRAW_FUNC
@brief 控件自绘回调函数,携带参数
*/
typedef void (*GUIWIN_STBDATA_OWNER_DRAW_FUNC)(
                                               GUIWIN_STADATA_OWNER_DRAW_T *owner_draw_ptr
                                               );

/*! @struct GUIWIN_STBDATA_ITEM_DATA_T
@brief item 数据结构
@note  状态栏中item的状态数据结构，item可以是文本或图片
*/
typedef struct
{
    uint32                          cur_frame;         /*!<当前画面*/
    GUIWIN_STBDATA_TEXT_T           text_info;         /*!<字符信息*/
    GUIWIN_STBDATA_ICON_T           icon_info;         /*!<icon信息*/
	GUI_BG_T                        bg_info;           /*!<背景信息*/
    GUIWIN_STBDATA_APP_HANDLE_FUNC  app_handle_func;   /*!<应用回调函数*/
    BOOLEAN 					    is_handle_callback;/*!<是否调用回调*/
	GUIWIN_STBDATA_OWNER_DRAW_FUNC  owner_draw_func;   /*!< owner draw function*/
}GUIWIN_STBDATA_ITEM_DATA_T;

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 动态创建softkey控件
@author xiaoqing.lu
@param    win_handle    [in]   窗口句柄
@param    left_text_id  [in]   左软键的文本id
@param    mid_text_id   [in]   中软键的文本id
@param    right_text_id [in]   右软键的文本id
@retval TRUE  设置成功
@retval FALSE 设置失败
@return 是否设置成功
@note   此函数封装了guisoftkey.c中的GUISOFTKEY_CreateCtrlDyn函数，用于动态创建
        softkey并设置好text_id 
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateSoftkeyDyn(
                                       MMI_HANDLE_T  win_handle,
                                       MMI_TEXT_ID_T left_text_id,
                                       MMI_TEXT_ID_T mid_text_id,
                                       MMI_TEXT_ID_T right_text_id
                                       );


/***************************************************************************//*!
@brief 设置softkey button的文本ID,并告知控件是否立即刷新
@author Great.Tian
@param      win_handle        [in] 窗口句柄
@param      str_info          [in] 要设置的文本信息
@param      button_num        [in] 要设置的buttom的索引
@param      is_need_update    [in] 是否立即刷新(若为TRUE则会立即进行局部刷新，为FALSE
                                   则在收到刷新消息时才进行刷新
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftkey.c中的GUISOFTKEY_SetButtonTextPtr函数，调用时会重新
        设置softkey上的文本
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyButtonTextPtr(
                                              MMI_HANDLE_T     win_handle,
                                              MMI_STRING_T     str_info,   
                                              GUISOFTKEY_INDEX button_num,
                                              BOOLEAN          is_need_update       // whether update
                                              );


/***************************************************************************//*!
@brief get softkey text id
@author Jassmine
@param      win_handle        [in] 窗口句柄
@param      left_id_ptr       [in/out] 左软键的text id指针(可以为空)
@param      middle_id_ptr   [in/out] 中软键的text id指针(可以为空)
@param      right_id_ptr      [in/out] 右软键的text id指针(可以为空)
@retval TRUE    获取成功
@retval FALSE   获取失败
@return 是否获取成功
@note   此函数封装了guisoftkey.c中的GUISOFTKEY_GetTextId，用于获取softkey上的文本
        id
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // 窗口句柄
                                       MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                       MMI_TEXT_ID_T   *middle_id_ptr, //in:may PNULL
                                       MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                       );


/***************************************************************************//*!
@brief 设置softkey button上的Icon图片ID,并告知控件是否立即刷新
@author Liqing.Peng
@param      win_handle         [in] 窗口句柄
@param      icon_id            [in] icon图片资源id
@param      button_num         [in] 要设置的软键索引
@param      is_need_update     [in] 是否立即刷新
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftkey.c中的GUISOFTKEY_SetButtonIconId函数，用于设置软键
        的icon图片，其方法类似于设置软键文本
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonIconId(
                                             MMI_HANDLE_T    win_handle,    // 窗口句柄
                                             MMI_IMAGE_ID_T  icon_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             );

#ifdef GUIF_SCROLLKEY
PUBLIC BOOLEAN GUIWIN_SetScrollkeytButtonIconId(
                                             MMI_HANDLE_T        win_handle,    // 窗口句柄
                                             MMI_IMAGE_ID_T      icon_id,
                                             MMI_TEXT_ID_T       text_id,
                                             GUISCROLLSKIN_INDEX skin_idx,
                                             GUISCROLLKEY_INDEX  button_num,
                                             BOOLEAN             is_need_update       // whether update
                                             );
#endif

/***************************************************************************//*!
@brief 设置softkey button的文本ID,并告知控件是否立即刷新
@author Great.Tian
@param      win_handle       [in] 窗口句柄
@param      text_id          [in] 文本资源id
@param      button_num       [in] 要设置的软键索引
@param      is_need_update   [in] 是否立即刷新
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftkey.c中的GUISOFTKEY_SetButtonTextId，用于设置软键的文本
        与GUIWIN_SetSoftkeyButtonTextPtr的区别在于，GUIWIN_SetSoftkeyButtonTextPtr
        传入的文本参数是MMI_STRING_T，而此函数传入的是文本资源的id
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonTextId(
                                             MMI_HANDLE_T    win_handle,    // 窗口句柄
                                             MMI_IMAGE_ID_T  text_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             );


/***************************************************************************//*!
@brief  设置softkey 文本ID,并告知控件是否立即刷新
@author Liqing.Peng
@param      win_handle     [in] 窗口句柄
@param      leftsoft_id    [in]   左软键的text id
@param      middlesoft_id  [in]   中软键的text id
@param      rightsoft_id   [in]   右软键的text_id
@param      is_need_update [in]   是否立即刷新
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftkey.c 中的GUISOFTKEY_SetTextId，用于设置左、中、右软键
        的文本，通过获取文本资源id来设置
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // 窗口句柄
                                       MMI_TEXT_ID_T   leftsoft_id,   // 左软键文本id
                                       MMI_TEXT_ID_T   middlesoft_id, // 中软键文本id
                                       MMI_TEXT_ID_T   rightsoft_id,  // t右软键文本id
                                       BOOLEAN         is_need_update // 是否立即刷新
                                       );


/***************************************************************************//*!
@brief 立即刷新Softkey控件
@author Liqing.Peng
@param    win_handle   [in]   窗口句柄
@retval TRUE    刷新成功
@retval FALSE   刷新失败
@return 是否刷新成功
@note   此函数封装了guisoftkey.c 中的GUISOFTKEY_Update，可以在需要时刷新softkey，
        如在对sotkey属性进行一系列初始化后可以刷新显示出来
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_UpdateSoftkey(
                                    MMI_HANDLE_T    win_handle
                                    );


/***************************************************************************//*!
@brief 设置Softkey的Style
@author liqing.peng
@param      win_handle [in]   窗口句柄
@param      font_color [in]   字体颜色
@param      bg_type    [in]   背景风格
@param      bg_color   [in]   背景颜色
@param      bg_img     [in]   背景图片id
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftley.c 中的GUISOFTKEY_SetStyle，用于设置softkey的风格，
        主要有背景和字体风格的设置
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      );


//++++CR242519
/***************************************************************************//*!
@brief 设置Softkey press时的Style
@author liqing.peng
@param      win_handle [in]   窗口句柄
@param      font_color [in]   字体颜色
@param      bg_type    [in]   背景风格
@param      bg_color   [in]   背景颜色
@param      bg_img     [in]   背景图片id
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftley.c 中的GUISOFTKEY_SetStyle，用于设置softkey的风格，
        主要有背景和字体风格的设置
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyPressedStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      );

//++++CR242519

/***************************************************************************//*!
@brief 设置Softkey的bg Style
@author liqing.peng
@param      win_handle [in]   窗口句柄
@param      bg_type    [in]   背景风格(0则表示背景为NULL，1表示为图片，2表示只有
                              颜色)
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   此函数封装了guisoftley.c 中的GUISOFTKEY_SetBgType，用于设置softkey的背景
        风格，
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyBgStyle(
                                        MMI_HANDLE_T          win_handle,
                                        GUI_SOFTKEY_BG_TYPE_T bg_type
                                        );


/***************************************************************************//*!
@brief 设置softkey common的背景
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      bg_ptr     [in]   控件公有的背景数据结构
@retval none
@return none
@note   此函数封装了guictrl_api.c 中的GUIAPICTRL_SetBg，间接调用softkey控件函数
        虚表上的设置背景函数来设置softkey背景
*******************************************************************************/
PUBLIC void GUIWIN_SetSoftkeyCommonBg(
                                      MMI_HANDLE_T  win_handle,
                                      GUI_BG_T      *bg_ptr
                                      );


/***************************************************************************//*!
@brief 设置softkey的状态
@author Lianxiang
@param      win_handle    [in]  窗口句柄
@param      button_index  [in]  要设置的button索引
@param      is_gray       [in]  是否灰化
@param      is_long_press [in]  是否长按
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note   如果窗口需要处理某个Softkey的Tp long消息,那么is_long_press设置为True
*******************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnState(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray,
                                          BOOLEAN		 is_long_press
                                          );

/***************************************************************************//*!
@brief 设置softkey的状态
@author Lianxiang
@param      win_handle    [in]  窗口句柄
@param      button_index  [in]  要设置的button索引
@param      is_gray       [in]  是否灰化
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note
*******************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnGray(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray
                                          );


/***************************************************************************//*!
@brief 判断软键是否可以处理tp消息
@author Liqing.Peng
@param      win_handle  [in]  窗口句柄
@retval TRUE    可以响应tp消息
@retval FALSE   不能相应tp消息
@return 是否可以处理tp消息
@note   此函数封装了guisoftkey中的GUISOFTKEY_IsTPEnable，用于判断软键是否可以处理
        tp消息
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsSoftkeyTPEnable(
                                        MMI_HANDLE_T   win_handle
                                        );


/***************************************************************************//*!
@brief 设置Softkey是否响应触笔消息
@author Liqing.Peng
@param      win_handle    [in]  窗口句柄
@param      sk_tp_enable  [in]  是否可以响应tp消息，TRUE可以，FALSE不可以
@retval TRUE    可以响应tp消息
@retval FALSE   不能相应tp消息
@return 是否可以响应tp消息
@note   此函数封装了guisoftkey中的GUISOFTKEY_SetTPEnable，可以对softkey是否能响应
        tp消息进行设置
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTPEnable(
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        sk_tp_enable
                                         );


/***************************************************************************//*!
@brief 设置字体和颜色
@author xiaoqing.lu
@param       win_handle [in]   窗口句柄
@param       color_ptr  [in]  颜色值(uint16)
@param       font_ptr   [in]   字体类型
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 是否设置成功
@note   此函数封装了GUISOFTKEY_Setfont，可以设置softkey上的文本字体和颜色
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftKeyfont(
                                     MMI_HANDLE_T    win_handle,
                                     GUI_COLOR_T     *color_ptr,
                                     GUI_FONT_T      *font_ptr
                                     );

// status bar
/***************************************************************************//*!
@brief  初始化item数据和状态栏
@author Lianxiang
@param      item_max_num [in]   最大item数
@retval TRUE    初始化成功
@retval FALSE   初始化失败
@return 是否初始化成功
@note   此函数封装了GUISTBDATA_InitData，初始化状态栏的数据,由于系统中状态栏的
        数据相同,必须在开机时初始化
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_InitStbData(
                                  uint32 item_max_num
                                  );


/***************************************************************************//*!
@brief 增加item
@author Xiaoqing.Lu
@param       item_data_ptr  [in]  item数据信息
@retval TRUE     添加成功
@retval FALSE    添加失败
@return 是否添加成功
@note   此函数封装了GUISTBDATA_AppendItemData，添加Item数据信息
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_AppendStbItemData(
                                        GUIWIN_STBDATA_ITEM_DATA_T *item_data_ptr
                                        );


/***************************************************************************//*!
@brief 设置item是否可见
@author xiaoqing.lu
@param       item_index [in]   item索引
@param       is_visible [in]   是否可见
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemVisible，设置item是否可见
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemVisible(
                                        uint32   item_index,
                                        BOOLEAN  is_visible
                                        );


/***************************************************************************//*!
@brief 判断item是否可见
@author xiaoqing.lu
@param       item_index [in]   要判断的item索引
@retval TRUE     可见
@retval FALSE    不可见
@return 是否可见
@note 此函数封装了GUISTBDATA_IsItemVisible， 判断item是否可见
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemVisible(
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
PUBLIC BOOLEAN GUIWIN_SetStbItemValid(
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
PUBLIC BOOLEAN GUIWIN_IsStbItemValid(
                                       uint32   item_index
                                       );


/***************************************************************************//*!
@brief 设置状态栏item处理的应用接口指针
@author xiaoqing.lu
@param       item_index      [in]   item索引
@param       app_handle_func [in]   指向相应接口的指针
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemHandleFunc，设置状态栏Item处理的应用接口指针,
        例如触笔点击的时候能够执行该应用
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemHandleFunc(
                                           uint32   item_index,
                                           GUIWIN_STBDATA_APP_HANDLE_FUNC app_handle_func
                                           );


/***************************************************************************//*!
@brief 设置item是否靠右显示
@author xiaoqing.lu
@param       item_index     [in]   item索引
@param       is_right_align [in]   是否靠右对齐
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 是否设置成功
@note   如果不是靠右显示，则肯定是靠左显示，没有第三种情况
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRightAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_right_align
                                           );

/***************************************************************************//*!
@brief 设置item是否居中显示
@author xiyuan.ma
@param       item_index     [in]   item索引
@param       is_right_align [in]   是否靠右对齐
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 是否设置成功
@note   是否居中显示
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemCenterAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_center_align
                                           );
/***************************************************************************//*!
@brief 判断item是否靠右显示
@author xiaoqing.lu
@param       item_index [in]   item索引
@retval TRUE     靠右显示
@retval FALSE    靠左显示
@return 是否靠右对齐
@note   此函数封装了GUISTBDATA_IsItemRightAlign，用于判断状态栏item是否靠右对齐
        如果不是靠右显示，则肯定是靠左显示，没有第三种情况
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemRightAlign(
                                          uint32 item_index
                                          );
/***************************************************************************//*!
@brief 判断item是否居中显示
@author xiaoqing.lu
@param       item_index [in]   item索引
@retval TRUE     靠右显示
@retval FALSE    靠左显示
@return 是否靠右对齐
@note   是否居中显示
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemCenterAlign(
                                          uint32 item_index
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
PUBLIC BOOLEAN GUIWIN_SetStbItemDisable(
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
PUBLIC BOOLEAN GUIWIN_IsStbItemDisable(
                                       uint32   item_index
                                       );

/***************************************************************************//*!
@brief 设置item对应的应用接口是否有效
@author xiaoqing.lu
@param       item_index [in]   item索引
@param       is_valid   [in]     是否有效 TRUE表示item会调用接口处理应用
@retval TRUE     设置成功
@retval FALSE    设置失败
@return 设置是否成功
@note   此函数封装了GUISTBDATA_SetItemHandleFuncValid，设置是否处理应用,
        如果为False则不会处理
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetItemHandleFuncValid(
                                             uint32  item_index,
                                             BOOLEAN is_valid
                                             );


/***************************************************************************//*!
@brief 判断item所处理的接口是否有效
@author xiaoqing.lu
@param      item_index  [in]  item索引
@retval TRUE    处理接口
@retval FALSE   不处理接口
@return 是否处理接口
@note   此函数封装了GUISTBDATA_SetItemHandleFuncValid，获取应用处理接口的有效性
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbItemIsHandleFuncValid(
                                               uint32 item_index
                                               );


/***************************************************************************//*!
@brief 设置Item的对应显示的icon
@author xiaoqing.lu
@param      item_index    [in]   item索引
@param      icon_info_ptr [in]   状态栏icon状态数据结构
@retval TRUE    设置成功
@retval FALSE   设置失败
@return 是否设置成功
@note 此函数封装了GUISTBDATA_SetItemIcon，每个item最多显示
      MMISTATUSBAR_ITEM_ICON_FRAME个Icon,在控件中以数组形式存在
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIcon(
                                     uint32                      item_index,
                                     const GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     );

/*****************************************************************************/
//  Description : 设置状态栏隐藏
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbVisible(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN      is_visible
                                 );

/***************************************************************************//*!
@brief 获取控件的某一项的icon信息
@author xiaoqing.lu
@param      item_index    [in]     item索引
@param      icon_info_ptr [in/out]   状态栏icon状态数据结构
@retval TRUE    获取成功
@return FALSE   获取失败
@note 此函数封装了GUISTBDATA_GetItemIcon，可以获取item中的icon状态
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIcon(
                                     uint32                item_index,
                                     GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     );


/***************************************************************************//*!
@brief 设置图片Icon显示类型,可显示为静态图片,也可显示为动态图片
@author xiaoqing.lu
@param      item_index [in]   item索引
@param      icon_style [in]   icon显示风格
@retval TRUE    设置成功
@retval FALSE   设置失败  
@return 是否设置成功
@note 此函数封装了GUISTBDATA_SetItemIconStyle，静态时只显示数组中的第一张图片,
      动态时显示数组中所有图片以动画显示
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconStyle(
                                          uint32                      item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E icon_style
                                          );


/***************************************************************************//*!
@brief 获取Item的图片显示状态,动态或静态
@author xiaoqing.lu
@param       item_index    [in]    item索引
@param      icon_style_ptr [in/out]   icon状态 
@retval TRUE    获取成功
@return FALSE   获取失败
@return 是否获取成功
@note   此函数封装了GUISTBDATA_GetItemIconStyle，可以获取item的icon状态
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconStyle(
                                          uint32            item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E *icon_style_ptr
                                          );


/***************************************************************************//*!
@brief 设置Item的Icon图片
@author xiaoqing.lu
@param     item_index  [in]    item索引
@param      icon_id    [in]   icon的图片id
@retval TRUE    设置成功
@retval FALSE   设置失败  
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemIconId，该接口和GUIWIN_SetStbItemIcon的
        区别在于它只设置item的icon数组的第一个icon,当控件显示静态icon的时候显
        示第一张图片
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconId(
                                       uint32         item_index,
                                       MMI_IMAGE_ID_T icon_id
                                       );


/***************************************************************************//*!
@brief 设置控件Item动画的当前显示的图片
@author xiaoqing.lu
@param      item_index  [in]   item索引
@param      cur_frame   [in]  当前显示的图片
@retval TRUE    设置成功
@retval FALSE   设置失败  
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemIconCurFrame，动态显示icon的时候,控件需要
        知道当前显示的icon数组下标
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 cur_frame
                                             );


/***************************************************************************//*!
@brief 获取icon数组当前显示的数组下标
@author xiaoqing.lu
@param       item_index    [in]    item索引
@param      cur_frame_ptr  [in/out]   当前显示的图片
@retval TRUE    获取成功
@return FALSE   获取失败
@return 是否获取成功
@note   此函数封装了GUISTBDATA_GetItemIconCurFrame，可以获取动态显示的icon当前
        显示的图片
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 *cur_frame_ptr
                                             );


/***************************************************************************//*!
@brief 设置Icon是否有效,当设置为无效的时候,item对应的状态不显示图片
@author xiaoqing.lu
@param      item_index [in]   item索引
@param      is_valid   [in]   是否有效
@retval TRUE    设置成功
@retval FALSE   设置失败  
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemIconValid状态栏既可以显示图片也可以显示文
        本,或者都显示,当设置为无效的时候,不显示图片
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          );


/***************************************************************************//*!
@brief 判断item的icon是否有效
@author xiaoqing.lu
@param      item_index [in]   item索引
@retval TRUE    有效
@retval FALSE   无效
@return 是否有效
@note   此函数封装了GUISTBDATA_IsItemIconValid，判断icon是否有效
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbIsItemIconValid(
                                         uint32 item_index
                                         );


/***************************************************************************//*!
@brief  设置item显示的字符串
@author xiaoqing.lu
@param      item_index    [in]   item索引
@param      text_info_ptr [in]   状态栏文本显示类型
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemText，状态栏既可以显示图片也可以显示文本,
        或者都显示,当设置为无效的时候,不显示文本
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemText(
                                     uint32            item_index,
                                     const GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     );


/***************************************************************************//*!
@brief 获取item的字符串
@author xiaoqing.lu
@param       item_index    [in]   item索引
@param      text_info_ptr  [in/out]   获取的item字符串
@retval TRUE    获取成功
@return FALSE   获取失败
@return 是否获取成功
@note   此函数封装了GUISTBDATA_GetItemText，可以获取item的字符串
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemText(
                                     uint32            item_index,
                                     GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     );


/***************************************************************************//*!
@brief 设置item的文本是否有效
@author xiaoqing.lu
@param      item_index [in]   item索引
@param      is_valid   [in]   文本是否有效
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemTextValid，设置item的文本是否有效
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemTextValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          );


/***************************************************************************//*!
@brief 判断文本是否有效
@author xiaoqing.lu
@param      item_index [in]   item索引
@retval TRUE    有效
@retval FALSE   无效
@return 是否有效
@note   此函数封装了GUISTBDATA_IsItemTextValid，判断文本是否有效
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemTextValid(
                                         uint32 item_index
                                         );


/***************************************************************************//*!
@brief 设置Item的rect
@author xiaoqing.lu
@param      item_index [in]   item索引
@param      rect       [in]   设置的区域
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUISTBDATA_SetItemRect，用于设置item在窗口中的区域
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRect(
                                     uint32     item_index,
                                     GUI_RECT_T rect
                                     );


/***************************************************************************//*!
@brief 获得item的rect
@author xiaoqing.lu
@param      item_index [in]   item索引
@retval TRUE    获取成功
@return FALSE   获取失败
@return 是否获取成功
@note   此函数封装了GUISTBDATA_GetItemRect，用于获取item的区域
*******************************************************************************/
PUBLIC GUI_RECT_T GUIWIN_GetStbItemRect(
                                        uint32 item_index
                                        );


/***************************************************************************//*!
@brief 获取item的总数
@author xiaoqing.lu
@retval item总数
@return item总数
@note   此函数封装了GUISTBDATA_GetItemTotalNum，用于获取状态栏item总数
*******************************************************************************/
PUBLIC uint32 GUIWIN_GetStbItemTotalNum(void);


/***************************************************************************//*!
@brief 设置状态栏是否显示背景
@author xiaoqing.lu
@param      win_handle     [in]  窗口句柄
@param      is_desplay_bg  [in]   设置是否显示背景(TRUE显示)
@retval void
@return void
@note   此函数封装了GUISTATUSBAR_SetBgIsDesplay有时候状态栏是直接显示在lcd上,
        而不需要显示自己的背景,例如idle或者主菜单窗口
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBgIsDesplay(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN is_desplay_bg
                                     );


/***************************************************************************//*!
@brief          设置状态栏是否清除用户背景
@author       xiyuan.ma
@param       win_handle              [in]  窗口句柄
@param       clear_customer_bg   [in]   设置是否清除用户背景(TRUE 清空)
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void GUIWIN_IsClearCustomerBg(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN clear_customer_bg
                                     );

/***************************************************************************//*!
@brief 设置状态栏的状态
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      ctrl_state [in]   控件状态值(uint32)
@param      is_set     [in]      状态是否激活(TRUE则激活设置的状态)
@retval void
@return void
@note   此函数封装了GUISTATUSBAR_SetState，对状态栏进行状态设置，传入一个状态值
        ctrl_state后，还需要判断该状态是否激活(is_set)
*******************************************************************************/
PUBLIC void GUIWIN_SetStbState(
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
PUBLIC BOOLEAN GUIWIN_GetStbState(
                                  MMI_HANDLE_T win_handle,
                                  uint32 ctrl_state
                                  );

/***************************************************************************//*!
@brief 设置控件的背景图片ID
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      image_id   [in]   图片资源id
@retval void
@return void 
@note   此函数封装了GUISTATUSBAR_SetBgImageId，用于设置背景图片
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBgImageId(
                                   MMI_HANDLE_T win_handle,
                                   MMI_IMAGE_ID_T image_id
                                   );


/***************************************************************************//*!
@brief  设置控件的背景颜色
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      bg_info    [in]   背景属性数据结构(有背景类型，形状，图片id和颜色)
@retval void
@return void
@note   此函数封装了GUISTATUSBAR_SetBg，用于设置状态栏的背景各个属性
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBg(
                            MMI_HANDLE_T win_handle,
                            GUI_BG_T  bg_info
                            );


/***************************************************************************//*!
@brief  只刷新icon，不刷洗背景
@author xiaoqing.lu
@retval MMI_RESULT_FALSE    刷新失败
@retval MMI_RESULT_TRUE     刷新成功
@return 刷新结果是否成功
@note   此函数封装了GUISTATUSBAR_UpdateCtrl只刷新控件的icon,不刷新控件背景,当控件
        背景无效的时候或者应用自己刷新背景的时候调用该接口刷新控件,该接口不会给窗
        口发送Fullpaint消息
*******************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStbIcon(void);


/***************************************************************************//*!
@brief 设置状态栏的横竖屏显示区域
@author hua.fang
@param      win_handle    [in]   窗口句柄
@param      both_rect_ptr [in]   横竖屏各自的区域
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   设置横竖屏显示的区域
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbBothRect(
                                     MMI_HANDLE_T      win_handle,
                                     GUI_BOTH_RECT_T   *both_rect_ptr
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
PUBLIC MMI_RESULT_E GUIWIN_ReDrawStbCtrl(
                                         MMI_HANDLE_T win_handle,
                                         BOOLEAN is_full_paint
                                         );

/***************************************************************************//*!
@brief  刷新状态栏
@author xiaoqing.lu
@retval MMI_RESULT_FALSE    刷新失败
@retval MMI_RESULT_TRUE     刷新成功
@return 刷新结果是否成功
@note   刷新状态栏背景和item,当背景无效的时候会发送fullpaint给窗口,主动让窗口
        刷新背景
*******************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStb(void);


/***************************************************************************//*!
@brief  动态创建状态栏控件
@author xiaoqing.lu
@param       win_handle [in]   窗口句柄
@retval TRUE     创建成功
@retval FALSE    创建失败 
@return 是否创建成功
@note   此函数封装了GUISTATUSBAR_CreateCtrlDyn，用于动态创建状态栏控件
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateStbDyn(
                                   MMI_HANDLE_T win_handle
                                   );


/***************************************************************************//*!
@brief  动态创建状态栏控件，用户可以自定义控件的区域
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      rect_ptr   [in]   用户自己设置的显示区域
@retval TRUE     创建成功
@retval FALSE    创建失败 
@return 是否创建成功
@note   此函数封装了GUISTATUSBAR_CustomCreate，跟GUIWIN_CreateStbDyn的区别在于，
        此函数可以自己定义状态栏的区域
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CustomStbCreate(
                                      MMI_HANDLE_T win_handle,
                                      GUI_BOTH_RECT_T *rect_ptr
                                      );


/***************************************************************************//*!
@brief  设置STATUSBAR控件item自绘回调函数
@author xiyuan.ma
@param  item_index     [in] item index
@param  func           [in] 自绘回调函数
@return 
@note 
*******************************************************************************/
PUBLIC void GUIWIN_SetStbOwnerDraw(
                                   uint32                          item_index,
                                   GUIWIN_STBDATA_OWNER_DRAW_FUNC  func
                                   );
								   
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbDropDownState(
                                       MMI_HANDLE_T win_handle,
                                       BOOLEAN enable
                                       );

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbDropDown(
                                    MMI_HANDLE_T win_handle
                                    );

/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbPressed(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN is_true
                                 );

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbPressed(
                                   MMI_HANDLE_T win_handle
                                   );
#endif

/***************************************************************************//*!
@brief 设置窗口title字符串
@author Liqing.peng
@param      win_handle     [in]   窗口句柄
@param      wstr_ptr       [in]   字符串指针
@param      wstr_len       [in]   字符串长度(<=16)
@param      is_need_update [in]   是否立即刷新(TRUE则在设置后立即刷新title，)
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetText，通过传入的字符串和长度设置title的文本
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleText(
                                   MMI_HANDLE_T		win_handle,
                                   const wchar		*wstr_ptr, //[IN] 字符串指针
                                   uint16			wstr_len, //[IN] 字符串长度, <=16
                                   BOOLEAN			is_need_update //[IN] 是否立即刷新
                                   );


/***************************************************************************//*!
@brief 设置窗口title字符串，基于字符串资源id
@author Liqing.peng
@param      win_handle      [in]  窗口句柄
@param      text_id         [in]   字符串资源id
@param      is_need_update  [in]  是否立即刷新(TRUE则在设置后立即刷新title，)
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetTextId，通过传入的字符串资源id来设置title文本
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextId(
                                     MMI_HANDLE_T	 win_handle,
                                     MMI_TEXT_ID_T   text_id, //[IN] 字符串资源id
                                     BOOLEAN         is_need_update //[IN] 是否立即刷新
                                     );


/***************************************************************************//*!
@brief 设置窗口title排版
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      align      [in]   字符串排版方式
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetTextAlignType，设置标题的文本排版方式
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextAlign(
                                        MMI_HANDLE_T		win_handle,
                                        GUI_ALIGN_E	        align
                                        );


/***************************************************************************//*!
@brief  设置标题控件的滚动方式
@author Liqing.peng
@param      win_handle  [in]   窗口句柄
@param      scroll_type [in]   滚动方式
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetTextScrollType，设置标题控件的滚动方式,默认
        从左向右滚动,请参考GUITITLE_SCROLL_TYPE_E定义
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextScroll(
                                         MMI_HANDLE_T			win_handle,
                                         GUITITLE_SCROLL_TYPE_E	scroll_type
                                         );


/***************************************************************************//*!
@brief 设置title上是否有arrow
@author Liqing.peng
@param      win_handle          [in]   窗口句柄
@param      is_need_title_arrow [in]   是否需要title上的arrow图标    
@retval TRUE    设置成功，title需要显示arrow
@retval FALSE   设置失败，title不需要显示arrow
@return title是否需要arrow
@note   此函数封装了GUITITLE_SetRollArrow，设置标题控件上的左右图标是否需要显示
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrow(    
                                        MMI_HANDLE_T   win_handle,
                                        BOOLEAN        is_need_title_arrow  //是否需要title上的arrow图标
                                        );


/***************************************************************************//*!
@brief  设置title上arrow的状态
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      index      [in]   title arrow的索引(0表示左arrow，1表示右arrow)
@param      status     [in]   设置的arrow状态
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetRollArrowStatus，设置标题控件上左右图标的状态
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrowStatus(    
                                              MMI_HANDLE_T               win_handle,
                                              GUITITLE_BUTTON_INDEX_T   index,
                                              GUITITLE_BUTTON_STATE_E  status
                                              );


/***************************************************************************//*!
@brief 获得title上arrow状态
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      index      [in]   title arrow索引(0表示左arrow，1表示右arrow)
@retval TITLE_BUTTON_NORMAL    正常高亮，可以响应tp触笔点击
@retval TITLE_BUTTON_INVISIBLE 不可见
@retval TITLE_BUTTON_GRAYED    灰化，不能响应tp触笔点击
@return title button的状态
@note   此函数封装了GUITITLE_GetRollArrowStatus，获得标题图标的状态
*******************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleRollArrowStatus(    
                                                             MMI_HANDLE_T               win_handle,
                                                             GUITITLE_BUTTON_INDEX_T   index
                                                             );


/*****************************************************************************/
// 	Description : 设置title上是否有button
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: 设置标题控件上的左右图标是否需要显示
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleNeedButton(    
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        is_need_button//是否需要title上的button图标
                                         );

/*****************************************************************************/
// 	Description : 设置title上是否有button
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: 设置标题控件上左右图标的状态
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonStatus(    
                                           MMI_HANDLE_T               win_handle,
                                           GUITITLE_BUTTON_INDEX_T   index,
                                           GUITITLE_BUTTON_STATE_E  status
                                           );

/*****************************************************************************/
// 	Description : 获得title上button状态
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: 获得标题图标的状态
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleButtonStatus(    
                                                           MMI_HANDLE_T               win_handle,
                                                           GUITITLE_BUTTON_INDEX_T   index
                                                           );

/***************************************************************************//*!
@brief 设置title右下角列表索引信息
@author Liqing.peng
@param      win_handle       [in]   窗口句柄
@param      is_need_sub_text [in]   是否需要列表索引信息
@param      wstr_ptr         [in]     传入的字符串
@param      wstr_len         [in]   字符串长度
@param      is_need_update   [in]   是否立即刷新显示
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetSubText，设置标题控件显示的附加字符串
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubText(    
                                      MMI_HANDLE_T   win_handle,
                                      BOOLEAN        is_need_sub_text,   //是否需要列表索引信息
                                      const wchar*	 wstr_ptr,		//[IN] string pointer
                                      uint16		 wstr_len,		//[IN] string length
                                      BOOLEAN        is_need_update        //是否立即显示
                                      );


/***************************************************************************//*!
@brief  设置title右下角列表索引信息参数
@author Liqing.peng
@param      win_handle    [in]   窗口句柄
@param      text_rect_ptr [in]   子字符串的区域
@param      font_ptr      [in]   子字符串字体
@param      align_ptr     [in]   排版方式
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetSubTextParam，设置标题控件显示的附加字符串的风格
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubTextParam(
                                           MMI_HANDLE_T     win_handle,
                                           GUI_RECT_T       *text_rect_ptr, // 子字符串的区域
                                           GUI_FONT_T       *font_ptr,      // 子字符串字体
                                           GUI_ALIGN_E      *align_ptr      // align type
                                           );


/***************************************************************************//*!
@brief 清除sub text的显示信息
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@retval TRUE    sub text被清除
@retval FALSE   清除失败
@return 是否清除成功
@note   此函数封装了GUITITLE_ClearSubTextInfo，Clear标题控件的附加字符串
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_ClearTitleSubTextInfo(    
                                            MMI_HANDLE_T   win_handle
                                            );


/***************************************************************************//*!
@brief 更新title
@author Liqing.peng
@param       win_handle [in]   窗口句柄
@retval TRUE     更新成功
@retval FALSE    更新失败  
@return 是否更新成功
@note   此函数封装了GUITITLE_Update，更新title，重新显示
*******************************************************************************/
PUBLIC void GUIWIN_UpdateTitle(
                               MMI_HANDLE_T win_handle
                               );


/***************************************************************************//*!
@brief  设置title 显示的区域
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      rect       [in]   设置的区域
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   此函数封装了GUITITLE_SetRect，设置title 显示的区域
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRect(
                                   MMI_HANDLE_T win_handle,
                                   GUI_RECT_T	rect
                                   );


/***************************************************************************//*!
@brief  设置title控件的横竖屏显示区域
@author Jassmine
@param      win_handle [in]   窗口句柄
@param      rect_ptr   [in]   横竖屏区域
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功
@note   设置title控件的横竖屏显示区域
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBothRect(
                                       MMI_HANDLE_T     win_handle,
                                       GUI_BOTH_RECT_T  *rect_ptr
                                       );


/***************************************************************************//*!
@brief 设置title控件的背景
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      bg_image   [in]   背景图片id
@param      bg_color   [in]   背景颜色
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功 
@note   此函数封装了GUITITLE_SetBackground，设置title控件的背景
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBackground(
                                         MMI_HANDLE_T		win_handle,
                                         MMI_IMAGE_ID_T		bg_image,
                                         GUI_COLOR_T		bg_color
                                         );


/***************************************************************************//*!
@brief 动态创建title控件
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      text_id    [in]   字符串资源id
@retval TRUE    创建成功
@retval FALSE   创佳失败
@return 是否创建成功
@note   此函数封装了GUITITLE_CreateCtrlDyn，动态创建title控件
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateTitleDyna(
                                      MMI_HANDLE_T	win_handle,
                                      MMI_TEXT_ID_T	text_id
                                      );


/***************************************************************************//*!
@brief 设置title的icon
@author Liqing.peng
@param      win_handle [in]   窗口句柄
@param      icon_id    [in]   icon图片资源id
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功 
@note   此函数封装了GUITITLE_SetIcon，用于设置title的icon，注意当title字符串很长
        并且滚动显示时，title上左右各有45像素宽度可以显示icon图片，此时图片宽裤
        不能超过45像素，否则造成覆盖现象
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleIcon(
                                   MMI_HANDLE_T     win_handle,
                                   MMI_IMAGE_ID_T   icon_id
                                   );


/***************************************************************************//*!
@brief 设置title字体颜色
@author Jassmine
@param      win_handle [in]   窗口句柄
@param      font_color [in]   字体颜色
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功 
@note   此函数封装了GUITITLE_SetFontColor，用于设置title字体颜色
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleFontColor(
                                        MMI_HANDLE_T     win_handle,
                                        GUI_COLOR_T      font_color
                                        );

/***************************************************************************//*!
@brief 获取title字体颜色
@author qi.liu1
@param      win_handle [in]   窗口句柄
@param      font_color [in]   字体颜色
@return 当前窗体title的颜色
@note   此函数封装了GUIWIN_GetTitleColor，用于获取当前title字体颜色
*******************************************************************************/
PUBLIC GUI_COLOR_T GUIWIN_GetTitleColor(    
                                        MMI_HANDLE_T win_handle
                                        );

/***************************************************************************//*!
@brief  设置是否可见
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      is_visible [in]   是否可见
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功 
@note   此函数封装了GUISOFTKEY_SetVisible，设置软键是否可见
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle,
                                        BOOLEAN         is_visible
                                        );

/***************************************************************************//*!
@brief  获取Softkey是否可见
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否可见
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle
                                        );

/***************************************************************************//*!
@brief  设置控件的背景
@author xiaoqing.lu
@param      win_handle [in]   窗口句柄
@param      bg_ptr     [in]   背景属性数据结构
@retval TRUE    设置成功
@retval FALSE   设置失败 
@return 是否设置成功 
@note   此函数封装了GUISTATUSBAR_SetCommonBg，设置状态栏控件背景
*******************************************************************************/
PUBLIC void GUIWIN_SetStbCommonBg(
                                  MMI_HANDLE_T  win_handle,
                                  GUI_BG_T      *bg_ptr
                                  );

/*****************************************************************************/
// 	Description : Set Title Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleIsVisible(    
                                        MMI_HANDLE_T win_handle,
                                        BOOLEAN is_visible,
                                        BOOLEAN is_update
                                        );

/*****************************************************************************/
//  Description : 设置item的bg
//  Global resource dependence : 
//  Author:xiyuan.ma 2012.1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemBg(
                                   uint32     item_index,
                                   GUI_BG_T   *bg_info_ptr
                                   );

/*****************************************************************************//*!
@brief  获取状态栏的LCD DEV INFO
@author xiyuan.ma
@param  win_handle       [in]   窗口句柄
@param  dev_info_ptr     [in]   LCD DEV INFO  
@note   此接口用于帮助用户获取状态栏的LCD信息
/*****************************************************************************/
PUBLIC void GUIWIN_GetStatusbarLayer(
									 MMI_HANDLE_T win_handle,
									 GUI_LCD_DEV_INFO *dev_info_ptr
									 );

/*****************************************************************************/
// 	Description : Set Istyle Icon state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonState(    
                                          MMI_HANDLE_T win_handle,
                                          GUITITLE_BUTTON_INDEX_T icon_index,
                                          GUITITLE_BUTTON_STATE_E icon_state,
                                          BOOLEAN is_need_update
                                          );

/*****************************************************************************/
// 	Description : Set Istyle Icon Text
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonTextId(    
                                           MMI_HANDLE_T win_handle,
                                           GUITITLE_BUTTON_INDEX_T icon_index,
                                           MMI_TEXT_ID_T text_id,
                                           BOOLEAN is_need_update
                                           );

/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonText(    
                                         MMI_HANDLE_T win_handle,
                                         GUITITLE_BUTTON_INDEX_T icon_index,
                                         MMI_STRING_T *str_ptr,
                                         BOOLEAN is_need_update
                                         );

/*****************************************************************************/
// 	Description : Set Istyle Icon Image
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonImage(    
                                          MMI_HANDLE_T win_handle,
                                          GUITITLE_BUTTON_INDEX_T icon_index,
                                          MMI_IMAGE_ID_T image_id,
                                          MMI_IMAGE_ID_T press_image_id,
                                          BOOLEAN is_need_update
                                          );

/*****************************************************************************/
// 	Description : Set Istyle Icon Msg
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonHandleMsgInfo(    
                                                  MMI_HANDLE_T win_handle,
                                                  GUITITLE_BUTTON_INDEX_T icon_index,
                                                  MMI_MESSAGE_ID_E button_msg_id,
                                                  MMI_CTRL_ID_T handle_msg_ctrl_id
                                                  );

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _GUI_WIN_H_


/*@}*/
