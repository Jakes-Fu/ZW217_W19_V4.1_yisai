 /*! @file guimenu.h
@brief 此文件介绍了menu控件的对外接口和数据类型
@author Jassmine
@version 1.0
@date 11/2006
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 11/2006      Jassmine              Creat
-# 04/2010      hua.fang        add function: SetItemVisible/SetNodeVisible


**                              SetItemGrayed
*******************************************************************************/

/*! @page page_Menu 菜单(Menu)

在Mocor平台的MMI开发过程中，Menu控件有着举足轻重的地位，它是MMI最基础最常用的控件之一，鉴于它的重要性，我们把它作为第一个控件介绍给用户。

-# @ref sec_menu_function
-# @ref sec_menu_intro
-# @ref sec_menu_grid
    -# @ref ssec_menu_grid_intro
    -# @ref ssec_menu_grid_item
    -# @ref ssec_menu_grid_item_def
    -# @ref ssec_menu_grid_layout
    -# @ref ssec_menu_grid_modify
-# @ref sec_menu_title
-# @ref sec_menu_slide
    -# @ref ssec_menu_slide_intro
    -# @ref ssec_menu_slide_item
    -# @ref ssec_menu_slide_level_1
    -# @ref ssec_menu_slide_level_2
    -# @ref ssec_menu_slide_item
    -# @ref ssec_menu_slide_layout
    -# @ref ssec_menu_slide_modify
-# @ref sec_menu_list
-# @ref sec_menu_popup
    -# @ref ssec_menu_popup_item
    -# @ref ssec_menu_popup_item_def
    -# @ref ssec_menu_popup_create
    -# @ref ssec_menu_popup_modify
-# @ref sec_menu_link
-# @ref sec_menu_function_list


@section sec_menu_function 控件的功能

菜单控件是GUI最基本的控件，在Mocor 10A以前的平台中，大部分窗口都是以Menu控件作为部件来创建的。我们把菜单分为主菜单、二级菜单、弹出菜单等三种形式。

Mocor10A中，提供了三种类型的主菜单，分别是宫格菜单、Title类型的主菜单、以及滑动菜单。

宫格菜单，用户可根据LCD Size任意定义宫格菜单显示的类型，如9宫格、12宫格、16宫格等等。宫格菜单的选项最多可以达到36项，但可视项为用户设置的宫格数，其他不可视的选项可通过按键切换焦点或触笔点击滚动条的方法变为可视。

Title类型的主菜单，它实际上是宫格菜单的延伸，因为很多小屏的手机中、或者多国语言时，如果把文字显示在每个选项的下方，限于屏幕的大小，字符串可能无法显示完整，因此设计了Title类型的主菜单，只将的选项的文字说明显示在窗口的上方。

滑动菜单是Mocor 10A新增的主菜单类型，这类主菜单的特点是，每页的显示条目固定（比如9、12、16等），页数可动态增加，但限于内存空间，应该有个最大值的。目前我们定义最大10页，足以满足用户的需求。

二级菜单的设计使菜单的操作更具有层次性，并且实现了菜单的级联效果，即可以在同一个菜单窗口中直接进入下级菜单，而不需要重新创建新的窗口。

Pop菜单是为了体现平台的多样性风格而设计的，其功能于二级菜单没有实质上的却别。但其半屏窗口的显示效果能使人机交互界面变得更加绚丽多姿。同样的Popmenu也支持多级级联的效果。

子菜单顾名思义是上述菜单的下级菜单，也可以称为级联菜单，从设计的角度来讲，子菜单的设计使得相同类型的功能能够统一到一个MMI入口当中。二级菜单、Pop菜单都可以包含子菜单，但这里请注意，主菜单是不能包含级联菜单的，后续章节介绍。子菜单与它的上级菜单同处在一个窗口中（即窗口id相同），在创建上级菜单时直接创建，其风格与主题都与上级菜单相同。

菜单控件提供了Get/Set接口来获取/设置菜单的属性及主题，例如应用通过接口函数得到光标所在位置的信息（静态菜单的当前光标group id和menu id，动态菜单的当前节点编号）。可以设置灰掉的效果。文本效果显示标题、软键盘，滚动条，菜单项文字（选中项反色显示，菜单项文字超过屏幕大小滚动显示）。

@section sec_menu_intro 菜单设计简介

-# CREATE_MENU_CTRL
@code
#define CREATE_MENU_CTRL(_GROUP_ID_, _CTRL_ID_) \
        MMK_CREATE_MENU, _GROUP_ID_, _CTRL_ID_
@endcode
    @arg _CTRL_ID_ 当前控件ID，为MMI_CONTROL_ID_E枚举类型
    @arg _GROUP_ID_ 菜单组的ID
-# @ref GUIMENU_STYLE_E
-# @ref GUIMENU_ITEM_T
-# @ref GUIMENU_GROUP_T

@note 关于菜单资源的定义:
- 菜单可以有静态菜单和动态菜单两种类型，静态菜单的资源是作为静态数据定义在平台中的，平台初始化时，注册每个模块的静态菜单资源，创建菜单控件时，根据菜单组的ID读取菜单资源。

@section sec_menu_grid 宫格主菜单

如图 2 10所示，示例所变现的是九宫格形式。所谓的九宫格菜单，它包括九个选项，以3行3列的方式排列，每个项包括要显示的图片、文本、动画等数据信息。
@image html menu_9_grid.jpg
图 2 10九宫格菜单

九宫格菜单可以扩展为12宫格菜单，以4行3列显示，当然也可以扩展成其它的排列方式。

@subsection ssec_menu_grid_intro 宫格主菜单的设计概述

- 宫格主菜单以矩阵形式显示，包含的信息有每个Item的图片、文本、当前item高亮标识等。可以显示超过一页的条目，最大条目为36个（用户可调），超过一屏时，右侧显示滚动条，可通过按键或tp多动滚动条的方式使任意一个item可见。
- 主菜单有组织功能，可通过TP长按item进入组织状态，并拖动图标进行位置更换。对于纯按键的平台，组织功能被屏蔽。

@subsection ssec_menu_grid_item 菜单项数组

数组定义在mmi_menutable.c文件中
@code
const GUIMENU_ITEM_T menu_mainmenu_icon[] =
{
    {ID_COMMUNICATION_CL,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_CL,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_CL,       0,MENU_NULL},
    {ID_COMMUNICATION_PB,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_PB,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PB,       0,MENU_NULL},
    {ID_COMMUNICATION_BROWSER,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_BROWSER,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_BROWSER,  0,MENU_NULL},
    {ID_MAINMENU_PLAYER,        TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_MULTIMEDIA,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PLAYER,   0,MENU_NULL},
    {ID_COMMUNICATION_MESSAGE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_MESSAGE,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_MESSAGE,  0,MENU_NULL},
    {ID_MAINMENU_TOOLS,         TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_APPS,     IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_TOOLS,    0,MENU_NULL},
    {ID_MAINMENU_PASTIME,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_PASTIME,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_GAME,     0,MENU_NULL},
    {ID_ENTERTAIMENT_FAVORITE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_EXPLORER,               IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_FAVORITE, 0,MENU_NULL},
    {ID_MAINMENU_SETTINGS,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_SETTINGS, IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_SET,      0,MENU_NULL},
};
@endcode

每一项表示一个菜单数据项，所包含的信息有菜单项的ID、softkey显示的文本（菜单项为当前焦点时显示）、文本、焦点图片、非焦点图片等。

@note 主菜单使用该数组显示初始化的信息，即默认的条目。当第一次开机之后进入宫格主菜单，控件读取该数组信息，然后保存到NV中，以后再次进入主菜单，控件将会读取NV中的相关信息进行显示。

@warning 这里一定注意两点：
-# 主菜单不能有级联菜单，即每个item的最后一项必须为MENU_NULL，否则，将会出现显示异常，因为在使用多层显示主菜单及二级菜单之后，级联菜单不会打开新窗口，这样多层数据不会释放，从而横显示问题，关于多层问题，请参考本文相关章节。
-# 主菜单所有的item都必须是静态数组的信息，不能动才创建主菜单Item。因为受NV空间的限制，NV中只保存了菜单Item的ID，及菜单组的group ID，显示的时候，仍然需要从它们所在的数组中读取数据。

@subsection ssec_menu_grid_item_def 菜单组的定义

@code
MENU_DEF( MENU_MAINMENU_ICON, menu_mainmenu_icon, GUIMENU_STYLE_ICON, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_mainmenu_icon))
@endcode

MENU_DEF宏的参数从前往后依次为：
- 菜单组ID
- 菜单数组地址
- 菜单类型（可以看到为GUIMENU_STYLE_ICON,）
- 四个空的文本或图片（NULL表示不需要）
- 菜单数组的项数

@subsection ssec_menu_grid_layout 宫格主菜单Layout设计

宫格菜单的显示包括图标显示、文本显示等，用户需要在头文件mmitheme_menu.h中定义宫格图片的整体区域和文字显示的整体区域，例如：
@image html menu_grid_layout.jpg
图 2 11宫格菜单Layout

从上图可以看到RECT_ICON(16, 30, 223, 253)是图片显示的区域，width_icon = 32;height_icon = 40为行距和列距。控件根据这写数据信息来计算宫格菜单的每个图标的显示位置。

同样的，RECT_TEXT(10, 80, 229, 271)是文字的显示区域，width_text = 20; height_text = 72为文字的行距和列距，这样，控件就可以计算出每个选项的文字显示区域。

宫格菜单的Layout信息都定义在 GUIMENU_ICONMENU_DATA_T 结构体中。

@note
-# Title类型主菜单时，title_text_rect是有效区域，相应的item_text_display_rect数组中所有区域都是空区域。但这个数组中的区域不是用户填入的，而是控件自己计算的，用户可不必关心。相反地，如果不是Title类型的宫格菜单，那么title_text_rect就一定是一个空区域。当然这些都是控件计算出来的。
-# MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM = page_row_num× page_col_num；这是一定要注意的，否则将会出现显示错误。
-# item_width与item_height是通过MENUICON_WIDTH和MENUICON_HEIGHT获得的，这两个宏比较特殊，它们与工程的make file相关联，在工程的make file中都有类似SPECIAL_EFFECT_ICON_SIZE = 48X48 的宏定义，48X48说明图片的宽度和高度为48。之所以这样规定，是受3D效果的限制，因为开启3D效果时，底层会通过make file在系统初始化时读取这个数值进行内存分配。切记！

@subsection ssec_menu_grid_modify 如何修改九宫格菜单

九宫格菜单在特定项目中只存在一组，被用作主菜单的初始化数据资源。

修改九宫格菜单风格包括修改图片显示位置、文本显示位置、菜单的行数和列数等位置信息；以及菜单项的图片、菜单图片的高亮图片、选项对应的文本等选项信息。

如果修改位置信息，那么只需要修改mmitheme_menu.h文件中的宏定义即可，有横竖屏的，应该兼顾。

如果修改选项数据信息，只需要修改菜单的静态数组，例如menu_mainmenu_icon。

@section sec_menu_title Title类型的宫格菜单

与上一节的宫格菜单类似，Title类型的宫格菜单只是把文字显示在窗口的上方，请看下图：
@image html menu_title.jpg
图 2 12Title类型的宫格菜单

Title类型的宫格菜单，沿用了宫格菜单的所有数据结构，所不同的就是Layout的设计，如下图：
@image html menu_title_layout.jpg
图 2 13 Title类型的宫格菜单的Layout

上图中的RECT_ICON(16， 75，223，263)为图片显示区域，width_icon = 32; height_icon = 25;为图片的行距与列距。最上方的title_rect（21,36, 218, 62）（图中没画，抱歉，这里大概估算一下吧），为title文字显示区域。

@section sec_menu_slide 滑动主菜单

滑动主菜单的显示形式类似与宫格菜单，但是滑动菜单支持多页功能，图 2 14中的下方显示了页数和当前页。
@image html menu_slide.jpg
图 2 14滑动主菜单

@subsection ssec_menu_slide_intro 滑动菜单设计概述

滑动主菜单同宫格菜单类似，以矩阵形式显示，包含的信息同样有每个Item的图片、文本、当前item高亮标识等。不同的是，滑动菜单每页所显示的item是受限制的，比如九宫格类型，就限制为9个图标。

滑动菜单具有多页功能，目前设计，最大可显示10页，这应该可以满足需求了。用户可通过按键或触笔滑动切换页面。

滑动主菜单有组织功能，可通过TP长按item进入组织状态，并拖动图标进行位置更换。对于纯按键的平台，组织功能被屏蔽。

@subsection ssec_menu_slide_item 菜单项数组

在mmi_menutable.c文件中定义了滑动菜单的静态数组
-# menu_mainmenu_slide_page数组，定义滑动菜单初始化的页以及关联的页面菜单项。
@code
const GUIMENU_ITEM_T menu_mainmenu_slide_page[] =
{
    {ID_MAINMENU_SLIDE_PAGE1, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_COMMUNICATION},
    {ID_MAINMENU_SLIDE_PAGE2, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PASTIME},
    {ID_MAINMENU_SLIDE_PAGE3, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_TOOLS},
    {ID_MAINMENU_SLIDE_PAGE4, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE4},
    {ID_MAINMENU_SLIDE_PAGE5, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE5},
    {ID_MAINMENU_SLIDE_PAGE6, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE6},
};
@endcode
-# 以关联的菜单MENU_SLIDE_COMMUNICATION为例：
@code
const GUIMENU_ITEM_T menu_slide_communication[] = // 注意，有最大数限制，最大数为宫格的最大数值
{
    // 第一行默认值：拨号盘	    通讯录	    通话记录
    {ID_COMMUNICATION_DIAL,     TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMM_DIALPAD,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DIAL,    0,MENU_NULL},
    {ID_COMMUNICATION_PB,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_PB,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PB,      0,MENU_NULL},
    {ID_COMMUNICATION_CL,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_CL,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_CL,      0,MENU_NULL},

    // 第二行默认值：蓝牙 	    信息服务 	浏览器
#ifdef BLUETOOTH_SUPPORT
	{ID_MAINMENU_BT,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BLUETOOTH, IMAGE_MAINMENU_ICON_SELECT,  IMAGE_MAINMENU_ICON_BT,0,MENU_NULL},
#endif
    {ID_COMMUNICATION_MESSAGE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_MESSAGE,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_MESSAGE, 0,MENU_NULL},
#ifdef BROWSER_SUPPORT
    {ID_COMMUNICATION_BROWSER,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_BROWSER,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_BROWSER, 0,MENU_NULL},
#endif

    // 第三行默认值：Wifi/wlan	    DCD         STK
#ifdef MMI_WIFI_SUPPORT
	{ID_CONNECTION_WIFI,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI, IMAGE_MAINMENU_ICON_SELECT,  IMAGE_MAINMENU_ICON_WIFI,0,MENU_NULL},
#endif
#ifdef DCD_SUPPORT
    {ID_TOOLS_DCD,          TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_DCD_QUICK_INFO,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DCD,     0,MENU_NULL},
#endif
    {ID_COMMUNICATION_STK,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_MAIN_STK,    IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_STK,     0,MENU_NULL},
};
@endcode

在这里，我们和级联菜单不期而遇，所谓的级联菜单就是当你在选种某一个菜单项的时候，打开了另一组菜单，这就是级联菜单的含义，这里我们点到为止，在后面的级联菜单一节中会作专门的介绍的。但是，冲突出来了，前文在介绍宫格菜单时，特别要求，主菜单不能设计级联菜单，这里为什么又出现级联菜单了呢？简直让人迷惑。

其实，主菜单的设计只是借用了guimenu(菜单)控件的显示流程，以及初始化菜单数据时的数组。滑动菜单所用到级联菜单是同时显示在一个窗口中的，即第一级用来显示title，第二级，才是真正的item内容。因此，这里只是借用了级联菜单的数据结构类定义滑动菜单的初始化数据罢了。

滑动主菜单的页数信息是通过menu_mainmenu_slide_page这样的数组定义的，而每页的选项数据则是通过诸如MENU_SLIDE_COMMUNICATION这样的菜单组定义出来的。

同样，这些信息只能是开机时的初始化信息，之后重新进入主菜单，所显示的信息都是从NV中毒出来的了，而且只读item id和group id。其他信息仍然需要通过静态数组查询。

例如MENU_SLIDE_COMMUNICATION和MENU_SLIDE_PASTIME中的选项可能会通过组合功能组合到一个页面内，查询时，显然它们又不在同一个数组中。

@subsection ssec_menu_slide_level_1 一级菜单组的定义

MENU_DEF( MENU_MAINMENU_SLIDE_PAGE, menu_mainmenu_slide_page, GUIMENU_STYLE_SLIDE_PAGE, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_mainmenu_slide_page))

菜单组ID、菜单数组地址、菜单类型（可以看到为GUIMENU_STYLE_SLIDE_PAGE），四个空的文本或图片（NULL表示不需要）、菜单数组的项数。

@subsection ssec_menu_slide_level_2 二级菜单组定义

@code
MENU_DEF( MENU_SLIDE_COMMUNICATION, menu_slide_communication, GUIMENU_STYLE_THIRD, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_slide_communication))
@endcode

菜单组ID、菜单数组地址、菜单类型（可以看到为GUIMENU_STYLE_THIRD，但这个类型在滑动主菜单中是无关紧要的，因为显示的时候用不到它），四个空的文本或图片（NULL表示不需要）、菜单数组的项数。

@subsection ssec_menu_slide_item 菜单项定义

@code
{ID_COMMUNICATION_DIAL,     TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMM_DIALPAD,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DIAL,    0,MENU_NULL},
@endcode

包含有Item id、softkey 信息、显示文本、高亮时背景图片，item图片等有效信息。

@subsection ssec_menu_slide_layout 滑动菜单的Layout设计

如图 2 14所示，滑动菜单显示与宫格类似，所不同的是多了一个显示页面的区域，我们以page_rect （80, 274, 160, 285）表示，显然，页面标识只显示1行，因此行间距为0，只需要给出列间距就可以计算了

滑动菜单的Layout信息都定义在 GUIMENU_SLIDEMENU_DATA_T 结构体中。

@note
-# MMITHEME_MENUMAIN_SLIDE_PAGE_NUM 定义页面的最大数。
-# MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM = page_row_num× page_col_num；这是一定要注意的，否则将会出现显示错误。
-# item_width与item_height是通过MENUICON_WIDTH和MENUICON_HEIGHT获得的，这两个宏比较特殊，它们与工程的make file相关联，在工程的make file中都有类似SPECIAL_EFFECT_ICON_SIZE = 48X48 的宏定义，48X48说明图片的宽度和高度为48。之所以这样规定，是受3D效果的限制，因为开启3D效果时，底层会通过make file在系统初始化时读取这个数值进行内存分配。切记！

@subsection ssec_menu_slide_modify 如何修改滑动菜单

-# 通过修改滑动菜单的页的定义数组，来达到增加和减少初始化页面的目的。
-# 通过修改页面的下级菜单数组，来达到增加和减少页面中显示项，以及显示内容。
-# 通过修改layout调整显示的位置。

@section sec_menu_list 列表菜单

关于列表菜单的使用我们在第1篇6.4一节中已经介绍的比较详细了，请参考相关章节。

@section sec_menu_popup 弹出式菜单

弹出式菜单即PopUp Menu，如图 2 15所示，弹出式菜单的窗口没有Title；它的虚拟背景区域可以显示半透明的效果；它的菜单项可以有下一级菜单，如果菜单的某一项有下一级菜单，则以箭头表示；在一个窗口中所能显示菜单条数得最大数用户可以自由设置，如果超过最大数的时候，将会显示滚动条（如图）。
@image html menu_popup.jpg
图 2 15 弹出式菜单

@subsection ssec_menu_popup_item 菜单项数组

-# 定义POP MENU的一级菜单
@code
const GUIMENU_ITEM_T menu_test_popupmenu_table[] =
{
    {ID_POPUP_MENU_1,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_2,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_3,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,MENU_TEST_SUBMENU_TABLE},
};
@endcode
-# 定义POPMENU的级联菜单
@code
const GUIMENU_ITEM_T menu_test_submenu_table[] =
{
    {ID_POPUP_MENU_SUB1,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_SUB2,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_SUB3,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,0},
};
@endcode

@subsection ssec_menu_popup_item_def 菜单组定义
-# 一级菜单组宏定义
@code
MENU_DEF( MENU_TEST_POPUPMENU_TABLE, menu_test_popupmenu_table, GUIMENU_STYLE_POPUP, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_test_popupmenu_table))
@endcode
-# 级联菜单组宏定义
@code
MENU_DEF( MENU_TEST_SUBMENU_TABLE, menu_test_submenu_table, GUIMENU_STYLE_POPUP, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_test_popupmenu_table))
@endcode
    -# 菜单组ID
    -# 菜单数组地址
    -# 菜单类型（可以看到为GUIMENU_STYLE_POPUP）
    -# 四个空的文本或图片（NULL表示不需要）
    -# 菜单数组的项数

从数组menu_test_popupmenu_table的定义可以看出，弹出菜单的第三项有下级菜单。

@subsection ssec_menu_popup_create 菜单创建

弹出式菜单和其它菜单不同，在创建的时候，弹出式菜单不需要使用CREATE_MENU_CTRL宏，也不需要依赖于窗口Table，无论是动态创建还是静态创建都是通过接口来实现的。
-# MMIPOPMENU_CreateStatic 静态创建弹出式菜单
-# MMIPOPMENU_CreateDynamic 动态创建弹出式菜单

@subsection ssec_menu_popup_modify 修改菜单风格

弹出试菜单的风格如下：
-# 菜单显示的最大项数，该值使用一个宏定义的常量，常量名称MMIMENU_POPUP_LINE_NUM_PAGE，定义在mmimenu_position.h文件中，目前我们对于240x320以及176x220的屏幕，弹出式菜单的最大显示项数为8，而240x400的最大显示数为10。
-# 菜单的背景修改，弹出式菜单的背景分为上下两个部分，上半部分为半透的效果，使用图片IMAGE_PUBWIN_UP_BG，这是一张固定资源图片，图片的大小可以自行修改。窗口的下半部分为背景色，颜色值为MMITHEME_POPUPMENU_BG_COLOR，在创建菜单控件的时候，调用MMITHEME_GetPopupMenuTheme（mmitheme_menu.c实现）获取。
-# 菜单的其他风格信息都可以在MMITHEME_GetPopupMenuTheme接口中修改。

@section sec_menu_link 级联菜单

顾名思义，级联菜单就是一个菜单项的下一级菜单，静态定义菜单数组资源的时候可以实现设置级联菜单的菜单组ID。当执行菜单项的时候，控件根据是否有级联菜单，自动显示。如上节介绍弹出式菜单时的级联菜单设置。

@section sec_menu_function_list 关键接口介绍

-# @ref GUIMENU_GetId
-# @ref GUIMENU_InsertNode
-# MMIAPICOM_EnableGrayed

*******************************************************************************/

/*! @addtogroup menuGrp Menu
@brief menu控件
@details
菜单控件分为两大类：
- 静态菜单\n
  静态菜单的数据事先通过菜单表的形式已经确定，菜单控件分析这张菜单表解析出菜单项，
  用户不能够再动态添加和删除菜单项。\n
  现在\link #GUIMENU_STYLE_ICON \endlink、\link #GUIMENU_STYLE_OPTION_PAGE \endlink、
  \link #GUIMENU_STYLE_TITLE_ICON \endlink、\link #GUIMENU_STYLE_SLIDE_PAGE \endlink、
  \link #GUIMENU_STYLE_CUBE \endlink必须是静态菜单。
- 动态菜单\n
  动态菜单的数据是用户通过控件提供的方法来插入的。\n
  现在主要弹出式的菜单用动态菜单。
@{
*******************************************************************************/


#ifndef _GUIMENU_H_
#define _GUIMENU_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guictrl.h"
#include "guiprgbox.h"
#include "mmitheme_menu.h"
#include "mmi_link.h"
#include "ui_layer.h"
#include "guiimg.h"
#include "ctrlmenu_export.h"
#include "ctrlmainmenu_export.h"

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
/*! @def GUIMENU_ITEM_NUM
@brief 计算菜单项的个数
*/
#define GUIMENU_ITEM_NUM(_ITEMPTR)     (sizeof(_ITEMPTR)/sizeof(GUIMENU_ITEM_T))

#define GUIMENU_SLIDE_PAGE_NUM           2

/*! @def GUIMENU_DYMAINMENU_STR_MAX_NUM
@brief 动态主菜单字符串长度最大值
*/
#define GUIMENU_DYMAINMENU_STR_MAX_NUM     20

#define GUIMENU_BUTTON_NUM              2

#define GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM 8
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUIMENU_SOFTKEY_STATE_T
@brief
*/
typedef uint32 GUIMENU_SOFTKEY_STATE_T;
/*! @def GUIMENU_LEFTSK_NEED
@brief
*/
#define GUIMENU_LEFTSK_NEED     0x00000001
/*! @def GUIMENU_MIDSK_NEED
@brief
*/
#define GUIMENU_MIDSK_NEED      0x00000002
/*! @def GUIMENU_RIGHTSK_NEED
@brief
*/
#define GUIMENU_RIGHTSK_NEED    0x00000004

/*! @typedef GUIMENU_MOVE_STATE_T
@brief 通知用户如何处理菜单的move动作，该值作为MSG_NOTIFY_MENU_SET_STATUS消息的附加信息进行传递
*/
typedef uint32 GUIMENU_MOVE_STATE_T; // 定义了 菜单move时的处理
/*! @def GUIMENU_STATE_MOVE_NONE
@brief 不处理
*/
#define GUIMENU_STATE_MOVE_NONE    0x00000000
/*! @def GUIMENU_STATE_MOVE_SET
@brief 通知用户需要将菜单设置成move状态
*/
#define GUIMENU_STATE_MOVE_SET     0x00000001 // 设置组织功能
/*! @def GUIMENU_STATE_MOVE_CANCEL
@brief 通知用户取消菜单的move状态
*/
#define GUIMENU_STATE_MOVE_CANCEL  0x00000002 // cancel键处理
/*! @def GUIMENU_STATE_MOVE_OK
@brief 通知用户已经完成菜单的move动作
*/
#define GUIMENU_STATE_MOVE_OK      0x00000003 // ok键处理

// 定义菜单状态，为以后扩展使用
/*! @typedef GUIMENU_STATE_T
@brief 菜单的状态信息
*/
typedef uint32 GUIMENU_STATE_T;
/*! @def GUIMENU_STATE_ENTER
@brief 如果置该位，表示menu处于初始状态，一旦刷新后，该状态设置为FALSE
*/
#define GUIMENU_STATE_ENTER          0x00000001
/*! @def GUIMENU_STATE_CUSTOMER_TITLE
@brief 如果置该位，表示菜单不去管理标题，由用户自己控制
*/
#define GUIMENU_STATE_CUSTOMER_TITLE 0x00000002 // 用户自定义title

/*! @typedef LINK_PARAMETER_T
@brief 菜单函数指针参数
*/
typedef void *DY_MENU_LINK_PARAMETER_T;
/*! @typedef NEW_ITEM_LINK_FUNC_T
@brief 动态菜单函数指针
*/
typedef void (*DY_MENU_ITEM_LINK_FUNC_T)(DY_MENU_LINK_PARAMETER_T param1, DY_MENU_LINK_PARAMETER_T param2);

/*! @typedef NEW_ITEM_LINK_DELETE_FUNC_T
@brief 动态菜单函数指针
*/
typedef BOOLEAN (*DY_MENU_ITEM_LINK_DELETE_FUNC_T)(DY_MENU_LINK_PARAMETER_T param1, DY_MENU_LINK_PARAMETER_T param2);

// 定义进入menu时的动画类型
typedef enum
{
    GUIMENU_ENTER_ANIM_NONE,        // 无动画 */
    GUIMENU_ENTER_ANIM_SLIDE_ICON,  // 打开主菜单时，item图标从八个方向向中间滑入 */
    // 扩展
    GUIMENU_ENTER_ANIM_MAX
}GUIMENU_ENTER_ANIM_TYPE_E;

/*! @typedef GUIMENU_FILE_DEVICE_E
@brief 数据存储路径类型
*/
typedef enum
{
    GUIMENU_DEVICE_SYSTEM,    /*!< C盘，不显示给用户，只供应用保存配置文件 */
    GUIMENU_DEVICE_UDISK,     /*!< U盘 */
    GUIMENU_DEVICE_SDCARD_1,  /*!< 卡1 */ 
    GUIMENU_DEVICE_SDCARD_2,  /*!< 卡2 */ 
    GUIMENU_DEVICE_MAX
} GUIMENU_FILE_DEVICE_E;

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
typedef struct  
{
    uint32* icon_layer_buf;//由于申请的是一整块BUF，不能用layer_info
    uint16 width;
    uint16 height;
}GO_GUIMENU_ITEM_BUF_T;

#endif

typedef enum
{
    GO_SLIDE_SUPPORT_NONE = 0,// 不支持滑动     
    GO_SLIDE_SUPPORT_ONLY_X,//支持X方向滑动
    GO_SLIDE_SUPPORT_ONLY_Y,//支持Y方向滑动
    GO_SLIDE_SUPPORT_XY,   //两个方向都支持 
    GO_SLIDE_SUPPORT_MAX
}GUIMENU_GO_SLIDE_TYPE_E;

//brief 缓存信息数据结构
//note 该结构用于缓存菜单项的信息，为加快绘制速度
typedef struct
{
    GUI_LCD_DEV_INFO icon_layer;                    // 层信息 */
    uint16 width;                                   // 宽度 */
    uint16 height;                                  // 高度 */
    BOOLEAN is_draw;
}GUIMENU_ITEM_BUF_T;

typedef struct 
{
    MMIMENU_LANGUAGE_TYPE_E  language_type;                     /*!< 语言类型   */
    wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1];             /*!< 字符串信息 */
}GUIMAINMENU_DYNAMIC_STRING_DATA_T;

/*! @struct  GUIMAINMENU_DYNAMIC_DATA_T
@brief 动态菜单数据
*/
typedef struct
{
    wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1];             /*!< 字符串信息 */
    uint16 text_len;                                            /*!< 字符串长度 */
    GUIMAINMENU_DYNAMIC_STRING_DATA_T  str_lang_info[GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM];   /*!< 字符串语言信息 */  
    BOOLEAN has_icon;                                           /*!< 是否有图片 */
    uint8 *icon_ptr;                                            /*!< 图片信息 */
    uint16 icon_height;                                         /*!< 图片实际高度 */
    uint16 icon_width;                                          /*!< 图片实际宽度 */
    DY_MENU_LINK_PARAMETER_T param1ptr;                         /*!< link_function 参数1 ,也作为删除时标识位*/
    DY_MENU_LINK_PARAMETER_T param2ptr;                         /*!< link_function 参数2 ,也作为删除时标识位*/
    uint32 parm1_size;                                          /*!< link_function 参数1的大小 */
    uint32 parm2_size;                                          /*!< link_function 参数2的大小 */
    DY_MENU_ITEM_LINK_FUNC_T link_function_ptr;                 /*!< 打开应用程序函数指针 */
    DY_MENU_ITEM_LINK_DELETE_FUNC_T delete_function_ptr;        /*!< 删除应用程序函数指针 */ 
    MMI_IMAGE_ID_T select_icon_id;                              /*!< 高亮图片id */
    uint32 icon_datasize;                                       /*!< 图片数据大小 */
    
} GUIMAINMENU_DYNAMIC_DATA_T;

/*! @struct  GUIMAINMENU_DYNAMIC_INFO_T
@brief 动态菜单信息
*/
typedef struct
{
    uint16 page_index;                                      /*!< 动态菜单位置信息,页码数 */
    uint16 item_index;                                      /*!< 动态菜单位置信息，序号 */
    MMI_MENU_ID_T menu_id;                                  /*!< 动态菜单menu_id,传入"0",由系统动态赋值 */
    GUIMAINMENU_DYNAMIC_DATA_T dynamic_menu_info;     /*!< 动态菜单数据信息 */
} GUIMAINMENU_DYNAMIC_INFO_T;

/*! @struct  GUIMAINMENU_DY_ITEM_T
@brief 动态菜单详细信息
*/
typedef struct
{
    GUIMAINMENU_DYNAMIC_INFO_T *ori_data_ptr;          /*!< 源数据 */
    GUIMENU_FILE_DEVICE_E store_dev;                        /*!< 存储位置 */
} GUIMAINMENU_DY_ITEM_T;

typedef enum
{
    GUIMENU_ITEM_IN_PAGE,  //in page
    GUIMENU_ITEM_IN_BAR,   //bar
    GUIMENU_ITEM_IN_FOLDER //in folder
}GUIMENU_ITEM_POSITION_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUIMENU_GetId                   CTRLMENU_GetId
#define GUIMENU_SetPopMenuItemHeight    CTRLMENU_SetPopItemHeight
#define GUIMENU_SetFirstAndSelectedItem CTRLMENU_SetFirstAndSelItem
#define GUIMENU_SetMenuTitle            CTRLMENU_SetTitle
#define GUIMENU_SetMenuTitleEx          CTRLMENU_SetTitleEx
#define GUIMENU_CreatDynamic            CTRLMENU_CreatDynamic
#define GUIMENU_CreatDynamicEx          CTRLMENU_CreatDynamicEx
#define GUIMENU_InsertNode              CTRLMENU_InsertNode
#define GUIMENU_ReplaceNode             CTRLMENU_ReplaceNode
#if defined (MRAPP_SUPPORT)||defined (HERO_ENGINE_SUPPORT ) 
#define GUIMENU_ReplaceNodeEx           CTRLMENU_ReplaceNodeEx
#endif
#define GUIMENU_SetNodeGrayed           CTRLMENU_SetNodeGrayed
#define GUIMENU_SetNodeGrayedByNodeId   CTRLMENU_SetNodeGrayedByNodeId
#define GUIMENU_SetNodeVisible          CTRLMENU_SetNodeVisible
#define GUIMENU_SetDynamicMenuSoftkey   CTRLMENU_SetDynamicMenuSoftkey
#define GUIMENU_SetMenuBgColor          CTRLMENU_SetBgColor
#define GUIMENU_SetMenuFontColor        CTRLMENU_SetFontColor
#define GUIMENU_GetCheckInfo            CTRLMENU_GetCheckInfo
#define GUIMENU_SetRect                 CTRLMENU_SetRect
#define GUIMENU_GetCurNodeId            CTRLMENU_GetCurNodeId
#define GUIMENU_SetItemGrayed           CTRLMENU_SetItemGrayed
#define GUIMENU_SetItemVisible          CTRLMENU_SetItemVisible
#define GUIMENU_SetStaticItem           CTRLMENU_SetStaticItem
#define GUIMENU_SetMaxWidth             CTRLMENU_SetMaxWidth
#define GUIMENU_SetEffectivePoint       CTRLMENU_SetEffectivePoint
#define GUIMENU_GetState                CTRLMENU_GetState
#define GUIMENU_SetState                CTRLMENU_SetState
#define GUIMENU_SetSelectedItem         CTRLMENU_SetSelectedItem
#define GUIMENU_SetButtonStyle          CTRLMENU_SetButtonStyle
#define GUIMENU_SetPopItemStatus        CTRLMENU_SetPopItemStatus
#define GUIMENU_SetSubMenuStyle         CTRLMENU_SetSubMenuStyle
#define GUIMENU_IsPopMenuEx             CTRLMENU_IsPopMenuEx
#define GUIMENU_GetNodeStr              CTRLMENU_GetNodeStr
#define GUIMENU_GetFontColor            CTRLMENU_GetFontColor
#define GUIMENU_IsFirstDisp             CTRLMENU_IsFirstDisp
#define GUIMENU_GetLevel                CTRLMENU_GetLevel
#define GUIMENU_SetFirstDisp            CTRLMENU_SetFirstDisp
#define GUIMENU_GetRect                 CTRLMENU_GetRect
#ifdef _SW_ZDT_PRODUCT_
#define GUIMENU_SetMenuBgImage          CTRLMENU_SetBgImage
#endif

#define GUIMAINMENU_GetPM               CTRLMAINMENU_GetPM
#define GUIMAINMENU_GetId               CTRLMAINMENU_GetId
#define GUIMAINMENU_InitMoveState       CTRLMAINMENU_InitMoveState
#define GUIMAINMENU_IsMoveState         CTRLMAINMENU_IsMoveState
#define GUIMAINMENU_CancelMoveIcon      CTRLMAINMENU_CancelMoveIcon
#define GUIMAINMENU_GetStyle            CTRLMAINMENU_GetStyle

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

