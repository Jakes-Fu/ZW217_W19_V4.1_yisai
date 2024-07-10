/*! @file guiiconlist.h
@brief 此文件介绍了icon list控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date  04/26/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  8/2007       Jassmine              Creat
*******************************************************************************/

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


#ifndef _GUIICONLIST_H_
#define _GUIICONLIST_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_iconlist.h"
#include "guiprgbox.h"
#include "guisoftkey.h"
#include "guianim.h"
#include "ctrliconlist_export.h"

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
#define GUIICONLIST_SetStyle		CTRLICONLIST_SetStyle 

/***************************************************************************//*!
@brief 设置宫格列表load anim方式
@author Jassmine
@param ctrl_id   [in] 控件id
@param load_type [in] load anim方式
@return
@note 
*******************************************************************************/
#define GUIICONLIST_SetLoadType		CTRLICONLIST_SetLoadType

/***************************************************************************//*!
@brief 获得宫格列表控件背景
@author Jassmine
@param ctrl_id   [in] 控件id
@param bg_ptr    [in] 背景
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_SetBg	CTRLICONLIST_SetBg                               

/***************************************************************************//*!
@brief 获得宫格列表控件区域
@author Jassmine
@param ctrl_id   [in] 控件id
@param rect_ptr  [in] 区域
@return
@note 
*******************************************************************************/
#define GUIICONLIST_SetRect		CTRLICONLIST_SetRect
                              

/***************************************************************************//*!
@brief 获得宫格列表控件总图片个数
@author Jassmine
@param ctrl_id   [in] 控件id
@return 总个数
@note 
*******************************************************************************/
#define GUIICONLIST_GetTotalIcon	CTRLICONLIST_GetTotalIcon

/***************************************************************************//*!
@brief 设置宫格列表控件总图片个数
@author Jassmine
@param ctrl_id   [in] 控件id
@param total_num [in] 总个数
@note 
*******************************************************************************/
#define GUIICONLIST_SetTotalIcon		CTRLICONLIST_SetTotalIcon

/***************************************************************************//*!
@brief 添加显示文本信息
@author Jassmine
@param index       [in] item索引
@param ctrl_id     [in] 控件id
@param display_ptr [in] 文本显示信息
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_AppendText		CTRLICONLIST_AppendText

/***************************************************************************//*!
@brief  添加显示文本的附加信息
@author xiyuan.ma
@param  index       [in] item索引
@param  ctrl_id     [in] 控件id
@param  display_ptr [in] 文本显示信息
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_AppendTextAddtion		CTRLICONLIST_AppendTextAddtion

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
#define GUIICONLIST_AppendIcon		CTRLICONLIST_AppendIcon

/*****************************************************************************/
//  Description : set icon number in one folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetIconNum		CTRLICONLIST_SetIconNum

/***************************************************************************//*!
@brief 获得当前选中item索引
@author Jassmine
@param ctrl_id [in] 控件id
@return 当前图片索引
@note 
*******************************************************************************/
#define GUIICONLIST_GetCurIconIndex	CTRLICONLIST_GetCurIconIndex

/***************************************************************************//*!
@brief 设置当前选中图片索引
@author Jassmine
@param cur_index [in] 当前索引,从0开始
@param ctrl_id   [in] 控件id
@note 
*******************************************************************************/
#define GUIICONLIST_SetCurIconIndex		CTRLICONLIST_SetCurIconIndex

/***************************************************************************//*!
@brief 设置允许选中的最大个数
@author Jassmine
@param ctrl_id          [in] 控件id
@param selected_max_num [in] 选中的最大个数
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_SetMaxSelectedItem	CTRLICONLIST_SetMaxSelectedItem

/***************************************************************************//*!
@brief 获得宫格列表是否处于标记状态
@author Jassmine
@param ctrl_id [in] 控件id
@return 是否处于标记状态
@note 
*******************************************************************************/
#define GUIICONLIST_GetMarkable	CTRLICONLIST_GetMarkable

/***************************************************************************//*!
@brief 设置宫格列表是否处于标记状态
@author Jassmine
@param ctrl_id     [in] 控件id
@param is_markable [in] 是否标记状态
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_SetMarkable		CTRLICONLIST_SetMarkable

/***************************************************************************//*!
@brief 设置所有item全选或者全不选set selected all or not
@author Jassmine
@param ctrl_id     [in] 控件id
@param is_selected [in] 是否选中
@return 操作是否成功
@note 如果不能全部选中,则按照最大选中个数选择
*******************************************************************************/
#define GUIICONLIST_SetAllSelected	CTRLICONLIST_SetAllSelected

/***************************************************************************//*!
@brief 获得item是否处于选中状态get selected item or not
@author Jassmine
@param ctrl_id [in] 控件id
@param index   [in] item索引
@return 是否选中
@note 
*******************************************************************************/
#define GUIICONLIST_IsSelectedItem	CTRLICONLIST_IsSelectedItem

/***************************************************************************//*!
@brief 获得选中item索引信息
@author Jassmine
@param ctrl_id   [in] 控件id
@param index_ptr [in] 选中索引数组
@param array_len [in] 选中索引数组长度
@return 选中icon个数
@note 
*******************************************************************************/
#define GUIICONLIST_GetSelectedItemIndex	CTRLICONLIST_GetSelectedItemIndex

/***************************************************************************//*!
@brief 设置item是否选中
@author Jassmine
@param ctrl_id     [in] 控件id
@param index       [in] item索引
@param is_selected [in] 是否选中
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_SetSelectedItem	CTRLICONLIST_SetSelectedItem

/***************************************************************************//*!
@brief 删除某个指定item
@author Jassmine
@param ctrl_id	[in] 控件id
@param index	[in] item索引
@return 操作是否成功
@note 
*******************************************************************************/
#define GUIICONLIST_RemoveItem	CTRLICONLIST_RemoveItem

/***************************************************************************//*!
@brief set icon width and height
@author Jassmine
@param ctrl_id	[in] 控件id
@param width	[in] 控件icon宽度
@param height	[in] 控件icon高度
@note 如果不设置，则使用默认值
*****************************************************************************/
#define GUIICONLIST_SetIconWidthHeight	CTRLICONLIST_SetIconWidthHeight

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
#define GUIICONLIST_PrepareSpecial	CTRLICONLIST_PrepareSpecial
									   
/*****************************************************************************//*!
@brief  设置支持横向滑动
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] 控件ptr
@param  is_v_slide	        [in] 是否支持横向滑动
@note   如果设置了横向移动，也会启用横向布局
*****************************************************************************/
#define GUIICONLIST_SetLayoutStyle	CTRLICONLIST_SetLayoutStyle

/*****************************************************************************//*!
@brief  是否支持横向滑动
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] 控件ptr
@note   判定横向移动
*****************************************************************************/
#define GUIICONLIST_GetLayoutStyle	CTRLICONLIST_GetLayoutStyle

/*****************************************************************************//*!
@brief  修改Iconlist的字体、字色信息
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] 控件id
@param  iconlist_text_style_ptr	    [in] iconlist的字体信息修改
@note   如果不设置，则默认为iconlist的字体和颜色
*****************************************************************************/
#define GUIICONLIST_SetIconListTextInfo	CTRLICONLIST_SetIconListTextInfo

/*****************************************************************************//*!
@brief  Set iconitem margin & space
@author xiyuan.ma 2011.12
@param  ctrl_id	                                [in] 控件id
@param  GUIICONLIST_MARGINSPACE_INFO_T	        [in] 边距、间距
@note   如果不设置，则默认为iconlist的行间距、列间距、上下左右边距
*****************************************************************************/
#define GUIICONLIST_SetIconItemSpace	CTRLICONLIST_SetIconItemSpace

/*****************************************************************************//*!
@brief  设置item的icon是否支持缩放
@author xiyuan.ma 2011.12
@param  Is_ItemIcon_Zoom	        [in] 是否可缩放
@note   
*****************************************************************************/
#define GUIICONLIST_SetItemIconZoom	CTRLICONLIST_SetItemIconZoom

/*****************************************************************************/
//  Description : 用户可自定义Item的Border的颜色、风格、粗细 
//  Author: xiyuan.ma 2012.1
//  param   is_select_border	        [in]设置的是select border还是unselect
//  param   select_border	            [in]border的颜色、粗细、风格
//  Note:   is_select_border == TRUE时， 用于设置select border；
//          is_select_border == FALSE时，用于设置unselect border； 
/*****************************************************************************/
#define GUIICONLIST_SetItemBorderStyle	CTRLICONLIST_SetItemBorderStyle

#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************//*!
@brief  设置分隔符的背景
@author xiyuan.ma 2011.12
@param  ctrl_id	       [in] 控件id
@param  *bg_ptr	       [in] 分隔符背景结构体指针
@note   如果不设置，则使用透明
*****************************************************************************/
#define GUIICONLIST_SetDelimiterBg	CTRLICONLIST_SetDelimiterBg

/*****************************************************************************//*!
@brief  分隔符的背景空白区域大小
@author xiyuan.ma 2012.05
@param  ctrl_id	                   [in] 控件id
@param  *delimiter_bg_space	       [in] 分隔符背景空白区域结构体指针
@note   当需要画得分隔符背景区域比实际分隔符区域小时，设置
*****************************************************************************/
#define GUIICONLIST_SetDelimiterBgSpace	CTRLICONLIST_SetDelimiterBgSpace

/*****************************************************************************//*!
@brief  设置分隔符显示的字符串的字体信息
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] 控件id
@param  *delimiter_text_style_ptr	[in] 分隔符的字符串的字体信息
@note   如果不设置，则默认为iconlist的字体和颜色
*****************************************************************************/
#define GUIICONLIST_SetDelimiterStrInfo	CTRLICONLIST_SetDelimiterStrInfo

/*****************************************************************************//*!
@brief  设置分隔符的高度
@author xiyuan.ma 2011.12
@param  ctrl_id	            [in] 控件id
@param  delimiter_height	[in] 分隔符的高度
@note   如果不设置，则默认高度为theme中的delimiter的高度
*****************************************************************************/
#define GUIICONLIST_SetDelimiterHeight	CTRLICONLIST_SetDelimiterHeight

/***********************************************************************************//*!
@brief  设置iconlist的每组的信息
@author xiyuan.ma 2011.12
@param  *iconlist_ctrl_ptr	[in] 控件指针
@param  *group_info_ptr	    [in] 分隔符结构体指针
@note   需要设置start_item_index(起始item index)、delimiter_str_info(分隔符内的字符串)
        插入时，请按照从小到大的顺序插入start_item_index
***********************************************************************************/
#define GUIICONLIST_CreateGroup	CTRLICONLIST_CreateGroup

/*****************************************************************************//*!
@brief  删除某个分隔符
@author xiyuan.ma 2011.12
@param  index	                [in] group id
@param  ctrl_id	                [in] 控件  id
@note   只用于删除某个分隔符，并不删除任何元素
*****************************************************************************/
#define GUIICONLIST_RemoveGroup	CTRLICONLIST_RemoveGroup

/*****************************************************************************//*!
@brief  remove all group
@author xiyuan.ma 2012.03
@param  ctrl_id	                [in] 控件  id
@note   
/*****************************************************************************/
#define GUIICONLIST_RemoveAllDelimiter	CTRLICONLIST_RemoveAllDelimiter

/*****************************************************************************//*!
@brief  获取delimiter group num
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@note   获取分隔符的数量
*****************************************************************************/
#define GUIICONLIST_GetDelimiterGroupNum	CTRLICONLIST_GetDelimiterGroupNum
#endif

/*****************************************************************************//*!
@brief  设置是否显示softkey
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@param  is_true	                [in] 是否用户自绘
@note   TURE的时候用户自绘
*****************************************************************************/
#define GUIICONLIST_SetDispCustomerSoftkey	CTRLICONLIST_SetDispCustomerSoftkey

/*****************************************************************************//*!
@brief  iconlist是否支持滑动
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件  id
@param  is_support_slide	    [in] 是否支持滑动
@note   否，为不支持
*****************************************************************************/
#define GUIICONLIST_SetSlideState	CTRLICONLIST_SetSlideState

/*****************************************************************************/
//  Description : set icon list cache page num to cache icon thumbnail
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetIconCacheNum	CTRLICONLIST_SetIconCacheNum

/*****************************************************************************//*!
@brief  用户可自定义默认图、Error图
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] 控件    id
@param  def_img_id	            [in] 默认图  id
@param  err_img_id	            [in] Error图 id
@note
/*****************************************************************************/
#define GUIICONLIST_SetDefaultIconByPtr	CTRLICONLIST_SetDefaultIconByPtr

/*****************************************************************************/
//  Description : 封装对外接口---get icon list relative height,except border
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
#define GUIICONLIST_GetRelativeHeight	CTRLICONLIST_GetRelativeHeight

/*****************************************************************************/
//  Description : 封装对外接口---get icon list display rect height
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
#define GUIICONLIST_GetDispHeight	CTRLICONLIST_GetDispHeight

/*****************************************************************************/
//  Description : 设置隐藏滚动条 即设滚动条宽度为0
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetHideSlideBar	CTRLICONLIST_SetHideSlideBar

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/


