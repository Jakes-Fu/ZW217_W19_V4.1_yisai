/*****************************************************************************
** File Name:      ctrliconlist_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/
/*! @page page_IconList 宫格列表(IconList)

-# @ref sec_iconlist_function
-# @ref sec_iconlist_def
-# @ref sec_iconlist_create
-# @ref sec_iconlist_function_list
-# @ref sec_iconlist_relation
-# @ref sec_iconlist_app

@section sec_iconlist_function 控件功能

IconList控件用来显示Icon缩略图的列表，用户可以选定缩略图列表的其中一项。目前只支持九宫格的显示风格，每页可以最多显示九个Icon缩略图，每行和每列都显示三个Icon缩略图。

IconList控件的功能描述如下：
-# 当一页无法显示所有的Icon缩略图时，在控件内的右侧会显示上下滚动条，通过操作滚动条可以浏览所有Icon缩略图。
-# 翻页功能使用“*”键和“\#”键实现。“*”键用来向上翻页，“\#”键用来向下翻页。也可以通过触笔点击滚动条实现翻页功能。翻页时，会切换当前选择的Icon。
-# 左右键可以用来切换当前选择的Icon。左（右）键使得当前选择的Icon切换为前（后）一个Icon。第一个Icon的前一个Icon为最后一个Icon，最后一个Icon的后一个Icon为第一个Icon。也可以通过触笔点击滚动条实现等同左右键的功能。
-# 上下键也可以用来切换当前选择的Icon。上（下）键使得当前选择的Icon切换为前（后）一行同一列的Icon。第一行的前一行是最后一行，最后一行的后一行是第一行。如果切换后的行是最后一行，那么有可能在同一列中不存在Icon，此时选择最后一个Icon。
-# 切换当前选择的Icon后，如果当前选择的Icon不在当前显示的页内，那么会删除不显示的Icon，添加需要显示的新的Icon，包括当前选择的Icon。
-# 触摸时，支持滑动操作，即，随着触笔的移动，icon列表随动。

IconList控件的效果如下图所示，采用了九宫格的显示风格；由于控件的Icon总数小于每页能显示的Icon总数，因此不包含滚动条。此控件包含8张Icon缩略图，当前选择了第6张Icon缩略图，标题栏中间位置显示当前选择的Icon的描述信息，标题栏右下侧显示当前选择的Icon的序号和Icon总数。
@image html iconlist_effect.jpg 
图 2 35 IconList控件的效果图

@section sec_iconlist_def 宏定义解析

@code
#define CREATE_ICONLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_ICONLIST, _CTRL_ID_
@endcode
- 用于创建一个Iconlist控件。
    - _CTRL_ID_ 控件ID

@section sec_iconlist_create 控件创建

代码部分仍然放在sample_iconlist_ctrl.c文件中。

- 第一步：创建一个测试控件的窗口，窗口Table名称为MMI_TEST_ICONLIST_WIN_TAB：
@code
WINDOW_TABLE(MMI_TEST_ICONLIST_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_ICONLIST_WIN_ID),
    WIN_TITLE(TXT_HELLO_TITLE),        
	WIN_FUNC( (uint32)HandleTestIconlistWinMsg),
	CREATE_ICONLIST_CTRL(TEST_ICONLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	END_WIN
};
@endcode
    - 该窗口的元素有窗口ID MMI_TEST_ICONLIST_WIN_ID，窗口标题控件，窗口处理函数HandleTestIconlistWinMsg，Iconlist控件，窗口的Softkey控件等，其中Iconlist控件的区域为窗口的客户区域（即去除标题控件和Softkey的区域）。
- 第二步：增加窗口的处理函数：
@code
LOCAL MMI_RESULT_E HandleTestIconlistWinMsg(
                 MMI_WIN_ID_T        win_id,
                 MMI_MESSAGE_ID_E    msg_id,
                 DPARAM              param
                 )
{
    uint16                      icon_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_STRING_T                title_text  = {0};
    MMI_CTRL_ID_T               ctrl_id = MMI_TEST_ICONLIST_CTRL_ID;
    GUIANIM_DATA_INFO_T         data_info = {0};
    GUIICONLIST_ICON_DISP_T     icon_list = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIICONLIST_SetTotalIcon(ctrl_id,MMIHELLO_ICONLIST_ICON_NUM);
        GUIICONLIST_SetCurIconIndex(TRUE,0,ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        //append icon item
        icon_index = *((uint16 *)param);

        MMI_GetLabelTextByLang(TXT_HELLO_TEST,&title_text);

        MMI_WSTRNCPY(icon_list.name_wstr,GUIICONLIST_NAME_MAX_LEN,
            title_text.wstr_ptr,title_text.wstr_len,
            title_text.wstr_len);
        icon_list.name_len = title_text.wstr_len;

        //softkey
        icon_list.softkey_id[0] = TXT_NULL;
        icon_list.softkey_id[1] = TXT_NULL;
        icon_list.softkey_id[2] = STXT_RETURN;

        GUIICONLIST_AppendItem(icon_index,ctrl_id,&icon_list);
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        //append icon
        icon_index = *((uint16 *)param);

        data_info.img_id = HELLO_SPRD_TEST;
        data_info.type   = GUIANIM_TYPE_IMAGE_BMP;
        
        //append icon
        GUIICONLIST_AppendIcon(icon_index,ctrl_id,&data_info,PNULL);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
@endcode
@note 注意：Open消息时我们设置了控件为Active控件，并设置控件显示的文本和图片信息。
- 第三步：增加创建窗口的函数，同时在头文件sample_iconlist_ctrl.h中增加函数定义
@code
PUBLIC void MMIHELLO_CreateIconlistWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ICONLIST_WIN_TAB, PNULL);
}
@endcode
- 第四步：在hello sprd窗口中增加入口（在接口HandleHelloSprdWinMsg中增加如下代码）：
@code
case MSG_APP_5:
        MMIHELLO_CreateAnimWin();
        break;
@endcode
    - 表示在sprd窗口中按5键打开动画 测试窗口。
- 第五步：编译运行
    - 首先编译运行模拟器，在hello sprd窗口中按5键，打开窗口如下：
@image html iconlist_test_win.jpg
图 2 36 Icnlist控件测试窗口

@section sec_iconlist_function_list 关键接口介绍

-# @ref GUIICONLIST_SetTotalIcon
-# @ref GUIICONLIST_SetCurIconIndex
-# @ref GUIICONLIST_GetCurIconIndex
-# GUIICONLIST_AppendItem
-# @ref GUIICONLIST_AppendIcon

@section sec_iconlist_relation 接口调用关系

Iconlist控件的接口在被调用时具有先后顺序的，GUIICONLIST_SetTotalIcon和GUIICONLIST_SetCurIconIndex必须放在加载Iconlist信息之前调用，原因是这两个接口在调用时将计算是否需要显示滚动条，当前页第一幅图片的索引等信息。

@section sec_iconlist_app 控件应用

Iconlist控件用于浏览图片的缩略图，例如在文件浏览器中，当浏览文件夹中的图片以缩略图显示时，如上图所示。

*******************************************************************************/

/*! @addtogroup iconlistGrp IconList
@brief iconlist控件
@details 
    -#  图片以宫格形式显示\n
    -#  支持标记,删除等功能\n
@{
*******************************************************************************/

#ifndef _CTRLICONLIST_EXPORT_H_
#define _CTRLICONLIST_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "mmitheme_iconlist.h"
#include "guiprgbox.h"
#include "guisoftkey.h"
#include "guianim.h"

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

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUIICONLIST_STYLE_E
@brief 宫格列表类型
*/
typedef enum
{
    GUIICONLIST_STYLE_ICON,         /*!<只有图片列表*/
    GUIICONLIST_STYLE_ICON_UIDT,    /*!<上图下文图片列表*/
    GUIICONLIST_STYLE_FOLDER_UIDT,  /*!<上图下文文件夹列表*/
    GUIICONLIST_STYLE_MAX           /*!<保留位*/
} GUIICONLIST_STYLE_E;

/*! @enum GUIICONLIST_LOAD_E
@brief Anim加载方式
*/
typedef enum
{
    GUIICONLIST_LOAD_CUR_PAGE,      /*!<一次性加载当前页图片*/
    GUIICONLIST_LOAD_ONE_BY_ONE,    /*!<当前页图片一个一个加载*/
    GUIICONLIST_LOAD_ALL,           /*!<一次性加载所有*/
    GUIICONLIST_LOAD_MAX            /*!<保留位*/
} GUIICONLIST_LOAD_E;

/*! @struct GUIICONLIST_LAYOUT_STYLE_E
@brief iconlist的排版风格，默认为垂直排版
*/
typedef enum
{
	GUIICONLIST_LAYOUT_V,           // 垂直排版，可支持上下滑动和切换
	GUIICONLIST_LAYOUT_H,           // 水平排版，可支持水平滑动
	GUIICONLIST_LAYOUT_MAX          // 保留位
}GUIICONLIST_LAYOUT_STYLE_E;

/*! @struct GUIICONLIST_INIT_DATA_T
@brief 宫格列表控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
} GUIICONLIST_INIT_DATA_T;

/*! @struct GUIICONLIST_DISP_T
@brief 标题和软件信息
*/
typedef struct
{
    MMI_STRING_T    name_str;                           /*!<图片名称*/
    MMI_TEXT_ID_T   softkey_id[GUISOFTKEY_BUTTON_NUM];  /*!<左中右软件*/
} GUIICONLIST_DISP_T;

typedef struct  
{
	uint16            icon_item_hspace;
	uint16            icon_item_vspace;
	uint16            iconlist_hmargin;
	uint16            iconlist_vmargin;
}GUIICONLIST_MARGINSPACE_INFO_T;

#ifdef GUIF_ICONLIST_DELIMITER
//iconlist group info
typedef struct
{
    uint16          start_item_index;         // 第一个icon的起始索引值
	MMI_STRING_T    delimiter_str_info;       // 分隔符内的字符串信息
} GUIICONLIST_DELIMITER_INFO_T;
#endif
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 获得宫格列表风格
@author Jassmine
@param ctrl_id   [in] 控件id
@param style     [in] 风格
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetStyle(
                                MMI_HANDLE_T        ctrl_id,
                                GUIICONLIST_STYLE_E style
                                );

/***************************************************************************//*!
@brief 设置宫格列表load anim方式
@author Jassmine
@param ctrl_id   [in] 控件id
@param load_type [in] load anim方式
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetLoadType(
                                    MMI_HANDLE_T        ctrl_id,
                                    GUIICONLIST_LOAD_E  load_type
                                    );

/***************************************************************************//*!
@brief 获得宫格列表控件背景
@author Jassmine
@param ctrl_id   [in] 控件id
@param bg_ptr    [in] 背景
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetBg(
                                 MMI_CTRL_ID_T  ctrl_id,//in
                                 GUI_BG_T       *bg_ptr //in
                                 );

/***************************************************************************//*!
@brief 获得宫格列表控件区域
@author Jassmine
@param ctrl_id   [in] 控件id
@param rect_ptr  [in] 区域
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetRect(
                                MMI_HANDLE_T    ctrl_id,
                                GUI_RECT_T      *rect_ptr
                                );

/***************************************************************************//*!
@brief 获得宫格列表控件总图片个数
@author Jassmine
@param ctrl_id   [in] 控件id
@return 总个数
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetTotalIcon(
									   MMI_CTRL_ID_T	ctrl_id	//in
									   );

/***************************************************************************//*!
@brief 设置宫格列表控件总图片个数
@author Jassmine
@param ctrl_id   [in] 控件id
@param total_num [in] 总个数
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetTotalIcon(
                                     MMI_CTRL_ID_T    ctrl_id,  //in
                                     uint16           total_num //in
                                     );

/***************************************************************************//*!
@brief 添加显示文本信息
@author Jassmine
@param index       [in] item索引
@param ctrl_id     [in] 控件id
@param display_ptr [in] 文本显示信息
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendText(
                                      uint16                index,      //in
                                      MMI_CTRL_ID_T         ctrl_id,    //in
                                      GUIICONLIST_DISP_T    *display_ptr//in
                                      );

/***************************************************************************//*!
@brief  添加显示文本的附加信息
@author xiyuan.ma
@param  index       [in] item索引
@param  ctrl_id     [in] 控件id
@param  display_ptr [in] 文本显示信息
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendTextAddtion(
											 uint16                index,      //in
											 MMI_CTRL_ID_T         ctrl_id,    //in
											 GUIICONLIST_DISP_T    *display_ptr//in
											 );
/***************************************************************************//*!
@brief 添加图片信息
@author Jassmine
@param index         [in] item索引
@param ctrl_id       [in] 控件id
@param data_info_ptr [in] 图片数据信息
@param file_info_ptr [in] 图片文件信息
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIcon(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
									  GUIANIM_DATA_INFO_T   *data_info_ptr,	//in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr  //in
                                      );

/***************************************************************************//*!
@brief 添加图片信息
@author Jassmine
@param index         [in] item索引
@param ctrl_id       [in] 控件id
@param data_info_ptr [in] 图片数据信息
@param file_info_ptr [in] 图片文件信息
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIconEx(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
									  GUIANIM_DATA_INFO_T   *data_info_ptr,	//in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr , //in
                                      GUIANIM_DISPLAY_INFO_T * display_info
                                      );


/*****************************************************************************/
//  Description : set icon number in one folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconNum(
                                      uint16            index,      //in
                                      uint16            icon_num,   //in
                                      MMI_CTRL_ID_T     ctrl_id     //in
                                      );

/***************************************************************************//*!
@brief 获得当前选中item索引
@author Jassmine
@param ctrl_id [in] 控件id
@return 当前图片索引
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetCurIconIndex(
                                          MMI_CTRL_ID_T     ctrl_id //in
                                          );

/***************************************************************************//*!
@brief 设置当前选中图片索引
@author Jassmine
@param cur_index [in] 当前索引,从0开始
@param ctrl_id   [in] 控件id
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetCurIconIndex(
                                        uint16			cur_index,  //in:
                                        MMI_CTRL_ID_T	ctrl_id	    //in:
                                        );

/***************************************************************************//*!
@brief 设置允许选中的最大个数
@author Jassmine
@param ctrl_id          [in] 控件id
@param selected_max_num [in] 选中的最大个数
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMaxSelectedItem(
                                              MMI_CTRL_ID_T ctrl_id,
                                              uint16        selected_max_num
                                              );

/***************************************************************************//*!
@brief 获得宫格列表是否处于标记状态
@author Jassmine
@param ctrl_id [in] 控件id
@return 是否处于标记状态
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_GetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id
                                       );

/***************************************************************************//*!
@brief 设置宫格列表是否处于标记状态
@author Jassmine
@param ctrl_id     [in] 控件id
@param is_markable [in] 是否标记状态
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_markable
                                       );

/***************************************************************************//*!
@brief 设置所有item全选或者全不选set selected all or not
@author Jassmine
@param ctrl_id     [in] 控件id
@param is_selected [in] 是否选中
@return 操作是否成功
@note 如果不能全部选中,则按照最大选中个数选择
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetAllSelected(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_selected
                                          );

/***************************************************************************//*!
@brief 获得item是否处于选中状态get selected item or not
@author Jassmine
@param ctrl_id [in] 控件id
@param index   [in] item索引
@return 是否选中
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_IsSelectedItem(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16        index
                                          );

/***************************************************************************//*!
@brief 获得选中item索引信息
@author Jassmine
@param ctrl_id   [in] 控件id
@param index_ptr [in] 选中索引数组
@param array_len [in] 选中索引数组长度
@return 选中icon个数
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetSelectedItemIndex(//selected item number
                                               MMI_CTRL_ID_T    ctrl_id,    //in:
                                               uint16*		    index_ptr,  //in/out:may PNULL
                                               uint16		    array_len   //in:
                                               );

/***************************************************************************//*!
@brief 设置item是否选中
@author Jassmine
@param ctrl_id     [in] 控件id
@param index       [in] item索引
@param is_selected [in] 是否选中
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSelectedItem(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           uint16           index,
                                           BOOLEAN          is_selected
                                           );

/***************************************************************************//*!
@brief 删除某个指定item
@author Jassmine
@param ctrl_id	[in] 控件id
@param index	[in] item索引
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveItem(
                                      MMI_CTRL_ID_T ctrl_id,
                                      uint16        index
                                      );

/***************************************************************************//*!
@brief set icon width and height
@author Jassmine
@param ctrl_id	[in] 控件id
@param width	[in] 控件icon宽度
@param height	[in] 控件icon高度
@note 如果不设置，则使用默认值
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconWidthHeight(
                                              MMI_CTRL_ID_T     ctrl_id,  //in
                                              uint16            width,    //in
                                              uint16            height    //in
                                              );

/***************************************************************************//*!
@brief 准备动画
@author hua.fang
@param from_ctrl_id	[in] 起始控件id
@param to_ctrl_id	[in] 目标控件id
@note 
如果from_ctrl_id是folder风格，to_ctrl_id是icon风格，会做文件夹里图片展开的动画，
这里需保证做动画时，from_ctrl_id还存在；
如果from_ctrl_id是icon风格，to_ctrl_id是folder风格，会做文件里的图片合拢的动画；
如果from_ctrl_id是icon风格，to_ctrl_id为空，会做所点击图片放大的动画；
如果from_ctrl_id是空，to_ctrl_id是icon风格，会做图片合拢的效果
*******************************************************************************/
PUBLIC void CTRLICONLIST_PrepareSpecial(
                                       MMI_CTRL_ID_T    from_ctrl_id,
                                       MMI_CTRL_ID_T    to_ctrl_id
                                       );
									   
/*****************************************************************************//*!
@brief  设置支持横向滑动
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] 控件ptr
@param  is_v_slide	        [in] 是否支持横向滑动
@note   如果设置了横向移动，也会启用横向布局
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetLayoutStyle(
										  MMI_CTRL_ID_T   ctrl_id,
										  GUIICONLIST_LAYOUT_STYLE_E layout_style
										  );

/*****************************************************************************//*!
@brief  是否支持横向滑动
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] 控件ptr
@note   判定横向移动
*****************************************************************************/
PUBLIC GUIICONLIST_LAYOUT_STYLE_E CTRLICONLIST_GetLayoutStyle(
															 MMI_CTRL_ID_T   ctrl_id
															 );

/*****************************************************************************//*!
@brief  修改Iconlist的字体、字色信息
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] 控件id
@param  iconlist_text_style_ptr	    [in] iconlist的字体信息修改
@note   如果不设置，则默认为iconlist的字体和颜色
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconListTextInfo(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   GUI_FONT_ALL_T iconlist_text_style //in
											   );

/*****************************************************************************//*!
@brief  Set iconitem margin & space
@author xiyuan.ma 2011.12
@param  ctrl_id	                                [in] 控件id
@param  GUIICONLIST_MARGINSPACE_INFO_T	        [in] 边距、间距
@note   如果不设置，则默认为iconlist的行间距、列间距、上下左右边距
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconItemSpace(
											MMI_CTRL_ID_T                     ctrl_id,       //in
											GUIICONLIST_MARGINSPACE_INFO_T    margin_space   //in
											);

/*****************************************************************************//*!
@brief  设置item的icon是否支持缩放
@author xiyuan.ma 2011.12
@param  Is_ItemIcon_Zoom	        [in] 是否可缩放
@note   
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemIconZoom(
										   MMI_CTRL_ID_T  ctrl_id,
										   BOOLEAN        is_itemicon_zoom
										   );

/*****************************************************************************/
//  Description : 用户可自定义Item的Border的颜色、风格、粗细 
//  Author: xiyuan.ma 2012.1
//  param   is_select_border	        [in]设置的是select border还是unselect
//  param   select_border	            [in]border的颜色、粗细、风格
//  Note:   is_select_border == TRUE时， 用于设置select border；
//          is_select_border == FALSE时，用于设置unselect border； 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemBorderStyle(
                                              MMI_CTRL_ID_T  ctrl_id,
                                              BOOLEAN        is_select_border,
                                              GUI_BORDER_T   *select_border
                                              );
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************//*!
@brief  设置分隔符的背景
@author xiyuan.ma 2011.12
@param  ctrl_id	       [in] 控件id
@param  *bg_ptr	       [in] 分隔符背景结构体指针
@note   如果不设置，则使用透明
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBg(
										  MMI_CTRL_ID_T  ctrl_id,//in
										  GUI_BG_T       *bg_ptr //in
										  );

/*****************************************************************************//*!
@brief  分隔符的背景空白区域大小
@author xiyuan.ma 2012.05
@param  ctrl_id	                   [in] 控件id
@param  *delimiter_bg_space	       [in] 分隔符背景空白区域结构体指针
@note   当需要画得分隔符背景区域比实际分隔符区域小时，设置
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBgSpace(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   DELIMITER_BG_SPACE_T *delimiter_bg_space //in
											   );

/*****************************************************************************//*!
@brief  设置分隔符显示的字符串的字体信息
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] 控件id
@param  *delimiter_text_style_ptr	[in] 分隔符的字符串的字体信息
@note   如果不设置，则默认为iconlist的字体和颜色
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterStrInfo(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   GUI_FONT_ALL_T *delimiter_text_style_ptr, //in
											   uint16         delimiter_str_left_space
											   );

/*****************************************************************************//*!
@brief  设置分隔符的高度
@author xiyuan.ma 2011.12
@param  ctrl_id	            [in] 控件id
@param  delimiter_height	[in] 分隔符的高度
@note   如果不设置，则默认高度为theme中的delimiter的高度
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterHeight(
											  MMI_CTRL_ID_T  ctrl_id,//in
											  uint16         delimiter_height //in
											  );

/***********************************************************************************//*!
@brief  设置iconlist的每组的信息
@author xiyuan.ma 2011.12
@param  *iconlist_ctrl_ptr	[in] 控件指针
@param  *group_info_ptr	    [in] 分隔符结构体指针
@note   需要设置start_item_index(起始item index)、delimiter_str_info(分隔符内的字符串)
        插入时，请按照从小到大的顺序插入start_item_index
***********************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_CreateGroup(
									   MMI_CTRL_ID_T                 ctrl_id,         //in
									   GUIICONLIST_DELIMITER_INFO_T  *group_info_ptr	 //in									
									   );

/*****************************************************************************//*!
@brief  删除某个分隔符
@author xiyuan.ma 2011.12
@param  index	                [in] group id
@param  ctrl_id	                [in] 控件  id
@note   只用于删除某个分隔符，并不删除任何元素
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveGroup(
                                       MMI_CTRL_ID_T      ctrl_id, 
                                       uint16             index
                                       );

/*****************************************************************************//*!
@brief  remove all group
@author xiyuan.ma 2012.03
@param  ctrl_id	                [in] 控件  id
@note   
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveAllDelimiter(
                                              MMI_CTRL_ID_T      ctrl_id
                                              );

/*****************************************************************************//*!
@brief  获取delimiter group num
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@note   获取分隔符的数量
*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDelimiterGroupNum(
                                               MMI_CTRL_ID_T      ctrl_id
                                               );
#endif

/*****************************************************************************//*!
@brief  设置是否显示softkey
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@param  is_true	                [in] 是否用户自绘
@note   TURE的时候用户自绘
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDispCustomerSoftkey(
                                                  MMI_CTRL_ID_T ctrl_id, 
                                                  BOOLEAN       is_true
                                                  );

/*****************************************************************************//*!
@brief  iconlist是否支持滑动
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@param  is_support_slide	    [in] 是否支持滑动
@note   否，为不支持
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSlideState(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         BOOLEAN       is_support_slide
                                         );

/*****************************************************************************/
//  Description : set icon list cache page num to cache icon thumbnail
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetIconCacheNum(
    MMI_HANDLE_T        ctrl_id,
    uint16  num
);

/*****************************************************************************//*!
@brief  用户可自定义默认图、Error图
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件    id
@param  def_img_id	            [in] 默认图  id
@param  err_img_id	            [in] Error图 id
@note
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDefaultIconByPtr(
											   MMI_CTRL_ID_T    ctrl_id,
                                               MMI_IMAGE_ID_T   def_img_id,
                                               MMI_IMAGE_ID_T   err_img_id
											   );
/*****************************************************************************/
//  Description : 封装对外接口---get icon list relative height,except border
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLICONLIST_GetRelativeHeight(
                                            MMI_CTRL_ID_T    ctrl_id
                                            );

/*****************************************************************************/
//  Description : 封装对外接口---get icon list display rect height
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDispHeight(
                                        MMI_CTRL_ID_T    ctrl_id
                                        );


/*****************************************************************************/
//  Description : 封装对外接口---get icon list empty string id
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetEmptyStrId(
                                        MMI_CTRL_ID_T    ctrl_id , 
                                        MMI_TEXT_ID_T empty_text_id
                                        );


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
