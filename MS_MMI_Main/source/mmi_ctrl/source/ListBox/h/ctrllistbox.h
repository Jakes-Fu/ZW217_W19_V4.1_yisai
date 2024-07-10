/*! @file ctrllistbox.h
@brief 此文件介绍了listbox控件的对外接口和数据类型
@author Great.Tian
@version 1.0
@date 24/07/2012
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 24/07/2012     Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_ListBox 列表(ListBox)

-# @ref sec_listbox_function
-# @ref sec_listbox_usage
    -# @ref ssec_listbox_def
    -# @ref ssec_listbox_create
    -# @ref ssec_listbox_type_def
    -# @ref ssec_listbox_example
    -# @ref ssec_listbox_struct
    -# @ref ssec_listbox_append
      -# @ref sssec_listbox_append_setmax
      -# @ref sssec_listbox_append_item
      -# @ref sssec_listbox_append_data
    -# @ref ssec_listbox_get_index
    -# @ref ssec_listbox_type_note
    -# @ref ssec_listbox_softkey
    -# @ref ssec_listbox_title
    -# @ref ssec_listbox_layout
-# @ref sec_listbox_app

@section sec_listbox_function 控件的功能

列表控件（Listbox）是窗口基本部件之一，列表控件能将用户数据以表单的形式显示出来，主要支持一下功能：
-# 支持单行显示和双行显示的列表；
-# 能显示带有列表项图标的列表；
-# 单选框列表；
-# 多选框列表；
-# 列表项可以被选中；
-# 支持滚动条，当列表项超过每页可显示的列表数目时将分也显示，在侧边增加滚动条，为用户提供上下滚动及翻页功能。
-# 列表项（item）的字符超过一行时能够滚动显示。
-# 支持Get/Set接口，方便用户获取/设置列表控件的属性。

@section sec_listbox_usage 控件的使用

@subsection ssec_listbox_def 宏定义解析

@code
#define CREATE_LISTBOX_CTRL(_LIST_TYPE_, _CTRL_ID_) \
        MMK_CREATE_LISTBOX, _LIST_TYPE_, _CTRL_ID_
@endcode
- 用于创建一个List Box控件。
    - _CTRL_ID_ 当前控件ID，为MMI_CONTROL_ID_E枚举类型
    - _LIST_TYPE_ List类型，为GUI_LISTBOX_TYPE_E枚举类型，列表的类型有单选按钮列表、复选框列表、文本列表三种类型。

@subsection ssec_listbox_create 如何创建控件

平台中，所有的控件在静态创建时的创建方式都是相同的。以闹钟列表为例，显示效果如下图：
@image html listbox_example_alarm.jpg
图 2 17闹钟窗口列表控件

创建闹钟窗口Table定义如下（源代码可以在平台的mmialarm.c文件中找到）：
@code
WINDOW_TABLE( ALARM_MAINWIN_TAB ) =
{
    WIN_FUNC((uint32)HandleAlarmMainWinMsg ),
    WIN_ID( MMIALM_MAINWIN_ID ),
    WIN_TITLE(TXT_ALARM_CLOCK),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_LISTALL_CTRL_ID),
    END_WIN
};
@endcode

这个窗口列表包含的元素有窗口回调函数指针、窗口ID、窗口Title、窗口Softkey、列表控件等元素。这里创建的列表控件的类型为GUILIST_TEXTLIST_E，这是普通类型的list控件。

@subsection ssec_listbox_type_def List控件的类型定义：

- @ref GUILIST_TYPE_E

@subsection ssec_listbox_example 创建列表控件事例

上面我们举的闹钟列表的控件的例子不够完善，相信很多人也许不会去查看闹钟模块的源代码，我这里还是举一个简单的创建列表控件的例子吧。

- 第一步：创建窗口Table
@code
WINDOW_TABLE( MMI_TEST_LIST_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleTestListWinMsg ),
    WIN_ID( MMI_TEST_LIST_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_LIST),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(
                    GUILIST_TEXTLIST_E,
                    MMI_TEST_LIST_CTRL_ID
                    ),
     END_WIN
};
@endcode
@note 这里没有控件位置信息，因为窗口创建时，系统会自动把title、softkey、list控件的区域设置好，而且此时的list区域实际就是窗口的客户区域。
- 第二步：添加控件和窗口的ID
我们的事例仍然在sample模块，窗口和控件的ID依然定义在sample_id.h中，增加如下代码：
@code
// 窗口ID
typedef enum
{
    MMIHELLO_WIN_ID_START = (MMI_MODULE_SAMPLE << 16),

    MMI_HELLO_SPRD_WIN_ID,
    MMI_TEST_MENU_WIN_ID,
    MMI_TEST_LIST_WIN_ID,

    MMIHELLO_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

// 控件ID（control ID）
typedef enum
{
    MMIHELLO_CTRL_ID_ID_START = MMIHELLO_MAX_WIN_ID,
    MMI_TEST_MENU_CTRL_ID,
    MMI_TEST_LIST_CTRL_ID,
    MMIHELLO_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;
@endcode
- 第三步：增加字符串资源
本例我们打算在列表控件中包含三个选项，需要用到字符串资源，另外，窗口的Title也需要一个字符串，softkey我们将使用平台中固有的几个字符串就可以了。
@code
在str_table.xls文件中增加如下几项：
TXT_HELLO_TEST_LIST 20  Test List   列表测试
TXT_HELLO_TEST_LIST1    20  Test List1  Test List1
TXT_HELLO_TEST_LIST2    20  Test List2  Test List2
TXT_HELLO_TEST_LIST3    20  Test List3  Test List3
然后在sample_mdu_def.h文件中增加如下代码：
RES_ADD_STRING(TXT_HELLO_TEST_LIST,"Test menu 2")
RES_ADD_STRING(TXT_HELLO_TEST_LIST1,"Test menu 3")
RES_ADD_STRING(TXT_HELLO_TEST_LIST2,"Test Menu")
RES_ADD_STRING(TXT_HELLO_TEST_LIST3,"Test menu 2")
@endcode
编译资源并生成。
- 第四步：增加窗口处理函数
@code
LOCAL MMI_RESULT_E HandleTestListWinMsg(
                                        MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_LIST_CTRL_ID;

    lcd_dev_info.lcd_id     = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   = GUI_BLOCK_MAIN;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_LIST_CTRL_ID);
        GUILIST_SetMaxItem(ctrl_id, MMI_HELLO_LIST_NUM, FALSE);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_CALL, TXT_HELLO_TEST_LIST1, STXT_OK, STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_PHONE, TXT_HELLO_TEST_LIST2, STXT_SELECT,STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_DISPLAY, TXT_HELLO_TEST_LIST3, STXT_ANSWER, STXT_RETURN);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
        break;

    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
@endcode
这里要注意GUILIST_SetMaxItem 设置了控件的最大数为3，使用宏定义的常量；AppendListItem接口用来Append列表项数据，实现如下：
@code
LOCAL void AppendListItem(
                          MMI_CTRL_ID_T    ctrl_id,
                          MMI_IMAGE_ID_T   image_id,
                          MMI_TEXT_ID_T    text_id,
                          MMI_TEXT_ID_T    left_softkey_id,
                          MMI_TEXT_ID_T    right_softkey_id
                          )
{
    CTRLLIST_ITEM_T          item_t     = {0};
    GUILIST_ITEM_DATA_T     item_data  = {0};
    uint32                  item_index = 0;
    // 设置列表项为单行带有一个icon和text的类型
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    // 设置列表项数据的内容
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.text_id = image_id;
    // 设置列表项数据的内容
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;
    // 设置对应的softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    // append 列表项
    GUILIST_AppendItem(ctrl_id, &item_t);

    return;
}
@endcode
- 第五步：编译并运行模拟器
在编译之前，我们先为打开这个窗口寻找一个入口，首先实现一个接口：
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateListWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_LIST_WIN_TAB, PNULL);
}
@endcode
还记得hello sprd那个小程序吧？我们就在那个窗口处理函数中增加如下代码：
@code
case MSG_APP_DOWN:
        MMIHELLO_CreateListWin();
        break;
@endcode

然后把这两个文件添加到模块对应的头文件和源文件的对应的位置，并在vc工程中增加sample_test_ctrl.c文件，然后编译运行模拟器。在idld窗口按左软键，进入hello sprd窗口，然后按向下的软键就可以得到如下窗口了：
@image html listbox_test_win.jpg
图 2 18列表控件测试窗口

这个时候，试着按上下键移动光标，你会发现softkey也会随之变化的，因为我们之前为每项item都设置了自己的softkey字符串。

@subsection ssec_listbox_struct 列表项数据结构简介

列表控件的列表项数据结构比较复杂，我们在设计时不仅仅考虑控件的显示风格问题，还要考虑系统内存的大小，对于列表项数目不多的，对于列表项很多的列表控件，你不可能一次性申请很大的动态内存来保存所有列表项数据，我们采用的方式是动态加载数据的方式，即列表项在显示的时候才去向APP索取要显示的内容，这是后话，我们还是先来看一下列表项的相关的数据结构：

-# @ref GUIITEM_DATA_TYPE_E
-# @ref GUIITEM_STATE_T
-# @ref GUILIST_STATE_T
-# GUIITEM_DATA_TEXT_BUFFER_T
-# @ref GUIITEM_DATA_U
-# @ref GUIITEM_CONTENT_T
-# @ref GUILIST_ITEM_DATA_T
-# @ref CTRLLIST_ITEM_T

@subsection ssec_listbox_append 如何Append列表项

在窗口Table中增加列表控件的宏，只是告诉MMK这个窗口创建时要创建一个列表控件的实例，并且定义了该列表控件的区域、类型以及控件的ID。但是并没有告诉窗口如何显示这个控件，实际上显示控件也是窗口做不了的，必须控件自己来完成。而要想显示列表控件那么我们必须给它增加要显示的数据。

@subsubsection sssec_listbox_append_setmax 设置列表项的最大值
在窗口创建时，窗口所包含的列表控件也会被同时创建，但上面说过，此时所创建的列表控件只是一个控件的实例，并没有设置任何列表项数据信息。如果你不设置列表项的数据信息，那么你所创建的列表控件什么都不会显示，原因很简单，列表控件显示的内容就是列表项数据。

在设置列表项数据之前你必须首先给出这个列表控件最多能包含的列表项的最大值，因为控件需要为列表项申请一整段的内存来保存数据信息。设置列表项数的最大值很简单，只需要调用接口GUILIST_SetMaxItem，这个接口必须在窗口的Open消息中调用，原因很简单，窗口的Open是在Full paint消息之前发送的，而Full paint消息是刷新屏幕的消息，如果你在此消息之后设置，那么将会不起作用。有的人可能会问，为什么不能在Full paint消息时设置呢，full paint消息不是先发送给窗口再发送给控件的吗？是的，但是这也是最容易出问题的地方，窗口只能接收到一次Open 消息，即在窗口创建的时候接收的，其他时间不会接收到这个消息，而Full paint消息可能接受很多次，比如窗口重新获取焦点的时候，如果每次都去重新设置，这显然降低了程序的执行效率，还可能引起其他的问题。

-# @ref GUILIST_SetMaxItem

@subsubsection sssec_listbox_append_item Append列表项

上一步只是设置了列表控件的最大项数，申请了内存空间，但是还没有增加列表项数据。GUILIST_AppendItem接口是用来Append列表项数据的接口，该接口必须在GUILIST_SetMaxItem接口之后才能调用，原因很简单，在没有申请列表项数据的内存之前你所Append的任何列表项数据都不可能成功的。Append是什么意思？你可以把它翻译成添加，但又觉得不是那么贴切，可以解释为“添加/增加”列表项数据，调用Append接口增加数据，是逐项增加的，从第一条开始顺序添加，它不是Insert（插入）。怎么翻译不重要，只要你理解就可以了。

-# @ref GUILIST_AppendItem

@subsubsection sssec_listbox_append_data 关于动态加载数据和静态加载数据

用户调用GUILIST_AppendItem接口时，所添加的列表项数据是一个CTRLLIST_ITEM_T类型的数据，它包含了一个数据内容的指针，类型为GUILIST_ITEM_DATA_T*，这个结构具有相当大的Size，这个我没有仔细的算过，但估计有二三百个字节吧。
因此，对于列表项很少的控件，你可以一次性把所有的数据都填充进去，例如闹钟列表只有三项数据。但是对于列表项很多的数据，你不可能把所有的列表项数据内容一次性添加完整。因为那将同时占用很大的内存，例如你有1000张图片的列表，每个列表项需要显示图片的缩略图。

为此，我们提供了两种数据的加载方式。
-# 静态加载：
    静态加载比较简单，在调用GUILIST_SetMaxItem（请注意静态加载时最后一个参数设置为FALSE）接口之后，你就可以直接调用GUILIST_AppendItem接口，在调用GUILIST_AppendItem接口之前，把CTRLLIST_ITEM_T中的所有数据都填充进去，例如mmialarm.c中为闹钟列表添加数据的接口AppendTwoLineCheckItem，我这里不列出来了，请查看源代码。
-# 动态加载：
    相对于静态加载数据来说，动态加载的过程会复杂一些，在调用GUILIST_SetMaxItem（请注意静态加载时最后一个参数设置为TRUE）接口之后，用户再Append数据的时候只需要增加CTRLLIST_ITEM_T的值，而不需要填充其中的GUILIST_ITEM_DATA_T*的内容（参见列表项数据结构定义），如果你填充了该项内容，但是由于之前GUILIST_SetMaxItem已经告诉控件动态加载了，此时控件也会忽略这项数据的。这样，当列表控件显示的时候，发现该项内容为空，就会发送MSG_CTL_LIST_NEED_ITEM_DATA消息给窗口，并携带GUILIST_NEED_ITEM_DATA_T类型的参数。

-# @ref GUILIST_NEED_ITEM_DATA_T

窗口接收到消息之后，解析参数，获取控件的ID、句柄、索引号，然后调用GUILIST_SetItemData接口设置数据内容。
如果你要显示动画，即GUIITEM_CONTENT_T的数据类型为GUIITEM_DATA_ANIM_DATA或者GUIITEM_DATA_ANIM_PATH时，在接收到MSG_CTL_LIST_NEED_ITEM_DATA消息时，你也没有必要增加对应的GUIITEM_DATA_U中的内容（当类型为GUIITEM_DATA_ANIM_PATH时，GUIITEM_DATA_U联合体中GUIANIM_FILE_INFO_T是有效的，当类型为GUIITEM_DATA_ANIM_DATA时，GUIANIM_DATA_INFO_T有效），但GUIITEM_DATA_TYPE_E一定要填写好。列表控件在显示动画的时候，如果发现动画的指针为空，那么会发送另一个消息MSG_CTL_LIST_NEED_ITEM_CONTENT给窗口，去索要数据，并携带GUILIST_NEED_ITEM_CONTENT_T类型的参数：

-# @ref GUILIST_NEED_ITEM_CONTENT_T

当窗口接收到消息MSG_CTL_LIST_NEED_ITEM_CONTENT时，解析参数，获取相关信息，调用GUILIST_SetItemContent接口设置具体的动画内容就可以了。

这个动态加载的过程有些复杂，但总结出来不外乎以下几步：
- 第一步：设置控件最大数；
- 第二步：append选项数据，不需要携带内容；
- 第三步：接收MSG_CTL_LIST_NEED_ITEM_DATA消息，并设置内容，如果是要显示动画，此时动画的相关数据不需要填写，但是数据类型一定要填写；
- 第四步：接收MSG_CTL_LIST_NEED_ITEM_CONTENT消息，并设置动画数据的内容。
平台的代码中有相关的事例，请参考mmifmm_wintab.c文件中，MMIFMM_PATH_SDCARD_CHILD_WIN_TAB窗口的处理。

@subsection ssec_listbox_get_index 获取当前被选中的索引号

在列表控件窗口中，按左键或右键（或者触笔点击屏幕上左右软键），窗口会接收到MSG_CTL_OK和MSG_CTL_CANCEL消息，通常情况下我们需要在处理OK消息的时候去获取当前选项的索引号。
获取当前选项的索引号很简单，只要直接调用接口GUILIST_GetCurItemIndex就可以了，传入的参数是控件的ID。

Example1:

@code
uint16 list_index = 0;
list_index = GUILIST_GetCurItemIndex(ctrl_id );
@endcode

对于单选或多选列表控件，可以调用 @ref GUILIST_GetSelectedItemIndex 接口。

Example2:

对于单选况，因为只有一个item被选中，所以：
@code
uint16                      cur_selection = 0;
GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
@endcode

Example3：

对于多选框，同时可有多个被选中，已经设置了最大选中数为3：
@code
uint16 sel_items[3] = {0};
uint16 sel_count = 0;
sel_count = GUILIST_GetSelectedItemIndex(ctrl_id,sel_items, sel_count);
@endcode

Example4：

设置多选框的最大选中项：
@code
GUILIST_SetMaxSelectedItem(ctrl_id,3);
@endcode

@subsection ssec_listbox_type_note 关于列表控件类型的说明
- @ref GUILIST_TYPE_E;

@subsection ssec_listbox_softkey 关于列表控件的softkey

列表控件的每一项都有自己的softkey显示内容，前面介绍数据结构时有所提及，这里再说明一下，你需要把每项列表项对应的softkey都设置好，如果你要动态改变softkey的显示，那么也请修改列表项中的softkey内容，而不是直接设置窗口的softkey。因为这个时候窗口的softkey是受列表控件所控制。

@subsection ssec_listbox_title 关于列表控件的Title

列表控件除了控制softkey显示的内容之外，还会控制窗口的Title的显示内容，例如显示第几条记录，这些都是控件来控制的，用户可以查看guilistbox.c中有关title的相关接口。这里不需要迷惑，因为列表控件比较特殊，它需要自己控制很多内容。

@subsection ssec_listbox_layout 关于列表控件的Layout

所谓Layout，就是列表控件显示的整体布局，例如显示单行的列表控件时，携带的小Icon和Text的显示排版；或者显示双行时如何排版等。

列表项的类型由GUIITEM_STYLE_E定义，该定义可以在mmi_theme.h中找到，定义了多种风格类型。

列表控件显示的时候会根据这个类型去读取相关的布局信息，这些布局信息是定义的静态数组，数组名称s_item_layout_data_table，在mmi_theme.c中定义，如果用户需要修改Layout，例如显示的Icon距离控件区域左边的距离的增减，那么可以直接修改数组中相关的Layout信息就可以了。

@section sec_listbox_app 控件的应用

列表是系统中最常用的控件之一，如电话号码本号码列表、消息列表、书签列表、mp3、电子书列表，图片列表、闹钟列表、输入法列表等。这些列表的应用中包括单选、多选、文本列表等的应用。图 2 19显示列表控件在资源管理器中的应用。
@image html listbox_app_example.jpg
图 2 19列表控件在资源管理器中的应用

*******************************************************************************/

/*! @addtogroup listboxGrp ListBox
@brief listbox控件
@details listbox控件
@{
*******************************************************************************/

#ifndef  _CTRLLIST_LISTBOX_H_
#define  _CTRLLIST_LISTBOX_H_

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h"
#include "ctrllist_export.h"
#include "guicommon.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guiprgbox.h"
#include "mmi_theme.h"
#include "mmitheme_list.h"
#include "guianim.h"
#include "guistring.h"
#include "type_manager.h"
#include "ctrlbase.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration                                  */
/*----------------------------------------------------------------------------*/
// list type info
#define CTRL_LIST_TYPE     (LIST_GetType())

#define CTRLLIST_WEAKLAYERALPHA 165

#define SCROLLBAR_WIDTH          (MMITHEME_GetScrollBarWidth()) // 滚动条的宽度

#define CTRLLIST_MAX_STRLEN      0xFF //list真正支持的最大字符个数, 不是GUILIST_STRING_MAX_NUM

//list内部状态
#define INNER_STATE_SET_ITEM_DATA     0x00000001   //list 需要异步插入数据状态
#define INNER_STATE_ON_ITEM_DATA      0x00000002   //list 正在插入item data

#define INNER_STATE_SET_ITEM_CONTENT  0x00000004   //list 需要异步插入图片数据状态
#define INNER_STATE_ON_ITEM_CONTENT   0x00000008   //list 正在插入item content
#define INNER_STATE_ON_3D             0x00000010   //list 正在播放3d效果
#define INNER_STATE_STOP_SWITCH       0x00000020   //list 停止自动切换item
#define INNER_STATE_RESET_ITEM_RECT   0x00000040   //list 是否重新计算item初始区域
#define INNER_STATE_CUSTORMER_LAYER   0x00000080   //list 是否重新计算item初始区域
#define INNER_STATE_SLIDE_ITEM        0x00000100   //list 进入横向滑动item状态，而不是纵向滑动控件
#define INNER_STATE_ABLE_SLIDE_ITEM   0x00000200   //list 是否支持item横向滑动
#define INNER_STATE_ABLE_LEFT_SLIDE_ITEM    0x00000400   //list 是否支持item向左滑动,该宏请搭配 INNER_STATE_ABLE_SLIDE_ITEM 配置使用
#define INNER_STATE_NEED_HIGHBAR_IF_CLICK   0x00000800   //当item被点击时，list需要高亮条状态，默认情况下不显示高亮条

typedef uint32 INNER_UPDATE_STATE_T;
#define INNER_STATE_UPDATE_BG         0x00000001   //list 画背景
#define INNER_STATE_UPDATE_PRG        0x00000002   //list 画滚动条
#define INNER_STATE_UPDATE_SK         0x00000004   //list 画Softkey
#define INNER_STATE_UPDATE_TITLE      0x00000008   //list 画Title
#define INNER_STATE_UPDATE_ITEM       0x00000010   //list 画Item

// list item内部状态
#define INNER_STATE_TO_UPDATE_HEIGHT  0x10000000   //list 画Item
#define INNER_STATE_UPDATE_HEIGHT_END 0x20000000   //list 画Item

// invalid value
#define INVALID_ITEM_INDEX            0xFFFF


#define INNERLIST_ITEM_NUMBER         30

//list的tip字符自动调整比率的上限
#define LIST_TIP_FONT_FACTOR_LIMIT (10)

//list的分组间距
#define LIST_GROUP_SPACE DP2PX_VALUE(10)
//list滑动到两端震动提示间隔
#define LSIT_VIBRATE_TIMER_OUT      (200)
#define LIST_SLIDE_TIP_HIDE_TICK    (200)

#define MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT DP2PX_VALUE(24)
#define MMICOM_LIST_SLIDE_ICON_MARGIN       DP2PX_VALUE(8)
#define MMICOM_LIST_SLIDE_ICON_NUM          (2)

typedef uint8 CTRLLIST_STYLE_TD_E;
enum{	
	CTRLLIST_STYLE_NONE = 0,	
	CTRLLIST_STYLE_WECHAT_MINE,
	CTRLLIST_STYLE_WECHAT_OTHER,
	CTRLLIST_STYLE_WECHAT_MINE_MORE,
	CTRLLIST_STYLE_WECHAT_OTHER_MORE,
	CTRLLIST_STYLE_WECHAT_PHOTO,
	CTRLLIST_STYLE_ALARMCLOCK,
	CTRLLIST_STYLE_PEDOMETER,
	CTRLLIST_STYLE_SETTING,
	CTRLLIST_STYLE_WECHAT_TEXTLIST,
};

typedef uint8 CTRLLIST_DIRECTION_KEY_E;
enum
{
    CTRLLIST_DIRECTION_KEY_NONE,
    CTRLLIST_DIRECTION_KEY_DOWN,
    CTRLLIST_DIRECTION_KEY_UP,
    CTRLLIST_DIRECTION_KEY_LEFT,
    CTRLLIST_DIRECTION_KEY_RIGHT,
    CTRLLIST_DIRECTION_KEY_MAX
};

typedef uint8 CTRLLIST_MARK_TYPE_T;
enum
{
    CTRLLIST_MARK_CHECK,
    CTRLLIST_MARK_RADIO,
};


typedef uint8 CTRLLIST_MARKING_TYPE_T;
enum
{
    CTRLLIST_MARKING_NONE,
    CTRLLIST_MARKING_ENABLED,
    CTRLLIST_MARKING_FIXED,
};



/*! @struct CTRLLIST_MOVE_INFO_T
@brief 滑动的记录信息
*/
typedef struct
{
    BOOLEAN                      had_moved;                          /*!< 已经滑动过*/
    BOOLEAN                      list_tp_press_down;                 /*!< 触笔按下*/
    BOOLEAN                      tp_down_handled;                    /*!< 触笔按下时的标记已经处理*/
    GUI_RECT_T                   prev_item_rect;                     /*!< 已经滑过项的区域*/
    uint16                       prev_item_index;                    /*!< 已经滑过的item id*/
    CONTINUE_MOVE_TYPE_E         continue_move_type;                 /*!< 滑动选择移动类型*/
} CTRLLIST_MOVE_INFO_T;

/*! @struct CTRLLIST_INIT_PARAM_T
@brief 初始化信息
*/
typedef struct
{
    CTRLBASE_INIT_PARAM_T  base_ctrl_param;

    GUILIST_TYPE_E         type;                  /*!< 控件类型*/
    GUIPRGBOX_STYLE_E      scroll_bar_type;       /*!< 滚动条类型>*/
    uint16                 max_item_num;          /*!< 控件最大项*/
    uint16                 max_selected_num;      /*!< 多选框最大选项*/
    BOOLEAN                is_dynamic_item_data;  /*!< 滑动选择移动类型*/
} CTRLLIST_INIT_PARAM_T;

//list内部保存item数据的结构
typedef struct
{
    int32                   item_top;        // 相对于控件区域的坐标
    int32                   item_height;     // 控件ITEM高度
    int16                   num_left;
    int16                   mark_left;
    GUI_RECT_T              content_rect[GUIITEM_CONTENT_MAX_NUM];

    CTRLLIST_ITEM_T          item;

    MMI_HANDLE_T            inner_handle[GUIITEM_CONTENT_MAX_NUM]; //每个元素都可能内嵌控件
} CTRLLIST_ITEM_INFO_T;

/*! @struct CTRLLIST_OBJ_T
@brief 控件数据
*/
typedef struct _list_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;          /*!< 控件基类数据 */

    BOOLEAN                         is_full_dirty;           // 置脏变量，全数据需要更新
    BOOLEAN                         is_pos_dirty;           // 置脏变量，显示位置需要更新
    GUILIST_STATE_T                 list_state;         /*!< 控件外部状态*/
    uint32                          inner_state;        /*!< 内部状态 */
    int16                           item_base_width; /*!< item的基准宽度，用来做缩放 */
    BOOLEAN                         show_num; /*!< 是否显示数字序号 */
    CTRLLIST_MARKING_TYPE_T         marking_type; /*!< 是否可标识 */
    BOOLEAN                         is_mark_icon_left; /*!< 是否将标识显示在左边 */
    CTRLLIST_MARK_TYPE_T            mark_type;          /*!< List的标记类型 */
	uint16                          mark_left_margin;
	uint16                          mark_top_margin;
    uint16                          mark_top_margin_for_multi; 
    uint16                          avatar_checkbox_left_margin;
    uint16                          avatar_checkbox_top_margin;
	BOOLEAN         				is_use_special_check_image; // ?????????check???
	MMI_IMAGE_ID_T  				check_selected_image_id;    // ?????????
	MMI_IMAGE_ID_T  				check_unselected_image_id;  // ???δ??????
    //for check box
    BOOLEAN                         is_check_used_for_multi_line;  /*!< ???check???????list style??????FALSE*/
    CONTINUE_MOVE_TYPE_E            continue_move_type; /*!< 连续移动的类型, List只支持上移和下移 */
    GUIPRGBOX_STYLE_E               scroll_bar_type;    /*<滚动条类型>*/
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    MMITHEME_PRGBOX_TYPE_E          scroll_bar_theme;
#endif
    MMITHEME_LIST_STYLE_T           display_style;      /*!< 控件默认风格 */

    uint8                           text_scroll_timer_id;               /*!< 文本滚动时的时钟ID*/
    uint8                           tp_timer_id;                          /*!< 为支持TP连续移动而加的timer */
    uint8                           switch_timer_id;            // 为支持TP连续移动而加的timer
    uint16                          str_start_pos;    /*!< 文字滚动后,显示的起始位置 */

    // item信息
    uint16                          move_item_num;     /*!< 一次移动的item数*/
    uint16                          max_item_num;      /*!< 允许输入的最大Item数*/
    uint16                          total_item_num;    /*!< 所有ITEM的总数,小于等于最大数*/
    uint16                          top_item_idx;      /*!< 显示在最顶端的Item 索引 */
    uint16                          cur_item_idx;      /*!< 当前光标选中的ITEM 索引 */
    uint16                          end_item_idx;      /*!< 一页中最底部的item 索引 */
    uint16                          tp_select_item_idx;/*!< tp点击的item索引 */
    uint16                          tp_select_item_content_idx;   /*!< tp点击的tp_select_item_idx 对应的content区域 */
    uint16                          mark_cnt_max;       /*!< 多选中允许同时选中的数目 */
    uint16                          selected_item_num;                /*!< 目前选中的item数目*/
    uint16                          num_start_index;  /*!< 当控件项显示数字时,该值表示起始数字*/

    CTRLLIST_ITEM_INFO_T            *item_info_ptr;    /*!< 控件项信息：保存Init时分配的item memory 指针, 类型为CTRLLIST_ITEM_INFO_T */

    // move item, to record pre item info when move highlight bar
    uint16                          pre_top_item_idx;             /*!< 切换ITEM焦点之前，窗口顶端ITEM索引 */
    uint16                          pre_cur_item_idx;       /*!< 切换ITEM焦点之前，被选中的ITEM索引 */
    uint16                          pre_end_item_idx;       /*!< 切换ITEM焦点之前，被选中的ITEM索引 */

    int32                           cur_object_index; /*!< 当前正在解析的控件项索引 */
    int32                           total_item_height; /*!< item总高度 */

    // title and prgbox信息
    GUILIST_TITLE_STYLE_E           title_style;            /*!< 控件中标题的显示风格，是否显示当前数目等*/
    IGUICTRL_T                     *prgbox_ptr;                 /*!< 滚动条实例指针 */
    GUILIST_TITLE_INDEX_E           title_index_type;   /*!< 是否需要在标题上显示总数和当前数目 */

    MMI_WIN_ID_T                    win_id;           /*!< 窗口 ID */
    CTRLLIST_MOVE_INFO_T             move_info;        /*!< 滑动信息 */

    /* keep old softkey id */
    MMI_TEXT_ID_T                   pre_left_sk_id;         /*!< item切换之前对应的窗口softkey 文字资源id */
    MMI_TEXT_ID_T                   pre_middle_sk_id;       /*!< item切换之前对应的窗口softkey 文字资源id */
    MMI_TEXT_ID_T                   pre_right_sk_id;        /*!< item切换之前对应的窗口softkey 文字资源id */

    BOOLEAN                         is_disable_3dmmi; /*!< 是否显示3D效果，系统保留，目前未使用 */
    BOOLEAN                         is_tpdown_select; /*!< tpdown是否点中ITEM */
    BOOLEAN                         is_transparent;   /*!< 是否透明显示背景 */
    uint8                           bg_alpha;         /*!< ????????????? */
    BOOLEAN                         is_click_item;   /*!< 触笔点击时，是否点击在item上 */
    BOOLEAN                         is_tp_down;     /*!< 是否tpdown */
    BOOLEAN                         is_permit_border;   /*!<是否允许显示边框*/
    BOOLEAN                         is_tp_content_valid; /*!< 有效的tp content状态 */
    BOOLEAN                         is_long_pressing;
    BOOLEAN                         is_left_slide_unfold; /*!< 当前item项已左滑展开 */
    BOOLEAN                         is_support_slide_dec_anim; /*!< 是否支持anim在滑动中解码 */

    GUILIST_EMPTY_INFO_T            empty_info;       /*!< 空item信息 */

    GUISTR_STYLE_T                  text_style;       /*!< 用户自定义文字风格 */
    BOOLEAN                         is_handle_starhash; /*!< 是否支持*,\#键切换页面 */
    BOOLEAN                         is_pre_send_ok;  /*!< 前一个tp up时的状态*/

    // 滑动信息
    uint8                           tp_move_timer_id;                   /*!< 为支持TP连续移动而加的timer */

    MMK_TP_SLIDE_E                  slide_state;      /*!< slide状态 */
    MMK_TP_SLIDE_E                  tpdown_slide_state;  /*!< TPDOWN时的滑动状态*/
    GUI_POINT_T                     slide_start_point;   /*!< 滑动的起始点 */
    GUI_POINT_T                     slide_pre_point;     /*!< 滑动时，前一点 */
    int32                           offset_y;            /*!< 顶端相对于控件区域Y方向的偏移*/
    GUI_LCD_DEV_INFO                item_layer;          /*!< Item显示的层信息 */
    BOOLEAN                         is_cus_blt_layer;    /*!< 是否由用户built item layer，默认情况下为FALSE */

    float                           fling_velocity;      /*!< 滑动速度 */
    int32                           total_distance;
    int32                           total_duration;
    uint16                          cur_msg_loop_item_idx; /*!< 每次消息循环后，focus项index */

    BOOLEAN                         is_forbid_tip;
    GUI_LCD_DEV_INFO                tip_layer; /*!< tip的显示层 */
    GUI_RECT_T                      tip_rect; /*!< tip的显示区域 */
    uint8                           tip_hide_timer; /*!< tip的隐藏timer */
    GUI_LCD_DEV_INFO                split_layer; /*!< split的显示层 */
    GUI_RECT_T                      split_rect; /*!< split的显示区域 */

    BOOLEAN                         is_need_reinit_scrollbar;/*!< 是否需要重新初始化滚动条 */
    CTRLLIST_GET_DATA_FUNC          get_data_func;

    GUI_RECT_T                      writeback_rect;//双buffer回写

    uint8                           line_space;
    uint8                           char_space;
    GUI_COLOR_T                     invalid_color;

	//6pixel spec
	BOOLEAN                         is_permit_bottom_margin;   /*!<???????end item??softkey???б??*/
	uint8							end_item_bottom_margin;   /*!<end item??softkey?????*/

    MMI_IMAGE_ID_T                  left_quick_icon_image;    //Qucik????image id
    MMI_IMAGE_ID_T                  right_quick_icon_image;    //Qucik????image id
	BOOLEAN         				is_show_left_triange_image; //??????????????????????????????Focus????
	BOOLEAN         				is_show_right_triange_image; //??????????????????????????????Focus????
    BOOLEAN                         is_handle_left_key;         //?????????
    BOOLEAN                         is_handle_right_key;        //?????????
    MMI_IMAGE_ID_T                  simsel_icon_image;      //Quick action icon for sim select

	BOOLEAN is_through_click; // ???????????
	MMI_HANDLE_T parent_win; //??????id
} CTRLLIST_OBJ_T;

// label class for public or protected method
typedef struct _list_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;
} CTRLLIST_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LIST_GetType(void);

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Lin.Lin
@param list_ptr [in]            控件指针
@param max_item_num [in]    控件项最大数
@param is_dynamic_item_data [in]    是否动态数据
@retval true/false
@return 返回成功或失败
@note 当为动态数据时，控件显示每个item都都会向窗口要数据
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxItemByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
);

/***************************************************************************//*!
@brief 设置最大选中的ITEM数
@author James.Zhang
@param list_ptr [in]     控件指针
@param mark_cnt_max [in]    最大选择项
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxSelectedItemByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16 mark_cnt_max
);

/***************************************************************************//*!
@brief  设置控件项的详细数据
@author yelianna
@param list_ptr [in]    控件指针
@param item_data_ptr [in]   用于设置的数据
@param pos  [in]    制定被设置的item索引
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetItemDataByPtr(
    CTRLLIST_OBJ_T      *list_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr,
    uint16           pos
);

/***************************************************************************//*!
@brief 设置Item数据内容
@author yelianna
@param list_ptr [in]    控件指针
@param item_content_ptr  [in]   用于设置的数据
@param item_index   [in]    制定被设置的item索引
@param item_content_index  [in] 数据内容的坐标（item_content_ptr为数组）
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetItemContentByPtr(
    CTRLLIST_OBJ_T       *list_ptr,
    const GUIITEM_CONTENT_T *item_content_ptr,
    uint16                    item_index,
    uint16                    item_content_index
);

/***************************************************************************//*!
@brief 设置控件的状态
@author James.Zhang
@param list_ptr [in]    控件指针
@param src_state    [in]    要设置的状态
@param is_true  [in]     设置/取消设置
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void LIST_SetListStateByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUILIST_STATE_T    src_state,
    BOOLEAN is_true
);


/***************************************************************************//*!
@brief 获得控件的某种状态是否被设置
@author James.Zhang
@param list_ptr [in]    控件指针
@param src_state    [in]    要获得的状态
@retval true/false
@return TRUE：控件具有该状态，FALSE：控件没有该状态
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetListStateByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUILIST_STATE_T    src_state
);

/***************************************************************************//*!
@brief  获得控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr [in]    控件指针
@param index     [in]   要获得的Item索引
@param user_data_ptr    [in]    用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetItemDataByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16 index,
    uint32 *user_data_ptr
);

/***************************************************************************//*!
@brief 矫正当前Item索引
@author Lin.Lin
@param list_ptr [in]    控件指针
@retval void
@return void
@note 当删除某一个项时，可能存在当前Item设置错误的情况，通过该接口可以矫正
*******************************************************************************/
PUBLIC void LIST_ValidCurItemPosByPtr(
    CTRLLIST_OBJ_T  *list_ptr
);

/***************************************************************************//*!
@brief 设置控件的背景色
@author James.Zhang
@param list_ptr [in]    控件指针
@param back_color [in]   背景色
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetBgColorByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUI_COLOR_T    back_color
);

/***************************************************************************//*!
@brief 设置List控件的默认图片ID
@author xiaoqing.lu
@param list_ptr [in]    控件指针
@param def_img_id [in]  解码过程中的默认图片
@param err_img_id [in]  解码失败后的默认图片
@retval true/false
@return 返回成功或失败
@note 两个默认图片都是为动态加载图片时，内嵌的动画控件所用
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetDefaultIconByPtr(
    CTRLLIST_OBJ_T  *list_ptr,
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
);

/***************************************************************************//*!
@brief 设置背景图片资源ID
@author xiaoqing.lu
@param list_ptr [in] 控件指针
@param image_id [in] 图片资源ID
@param is_transparent [in] 是否透明显示
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetBgImageByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    MMI_IMAGE_ID_T image_id,
    BOOLEAN        is_transparent
);

/***************************************************************************//*!
@brief 设置是否支持slide状态
@author Xiaoqing.Lu
@param list_ptr [in] 控件指针
@param is_slide_used [in] TRUE:支持，FALSE:不支持
@retval true/false
@return 返回成功或失败
@note 默认情况，我们支持slide状态
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ptr,
    BOOLEAN is_slide_used
);

/***************************************************************************//*!
@brief 获取是否支持slide状态
@author Xiaoming.Ren
@param ctrl_id [in] 控件HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ptr
);

/*****************************************************************************/
//  Description : 判断是move item 还是move整个控件，即横向还是纵向
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetMoveListItem(
    CTRLLIST_OBJ_T   *list_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list内部状态
/*****************************************************************************/
PUBLIC void LIST_SetItemMoveState(
    CTRLLIST_OBJ_T *list_ptr,
    BOOLEAN         is_true
);

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemDispLayer(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_RECT_T item_rect
);

/*****************************************************************************/
//  Description : 显示字符串
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DisplayText(
    CTRLLIST_OBJ_T *list_ptr,
    MMI_TEXT_ID_T  text_id,
    GUI_RECT_T     *disp_rect_ptr,
    GUI_RECT_T     *clip_rect_ptr,
    GUISTR_STYLE_T *str_style_ptr
);

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemDispLayer(void);

/*****************************************************************************/
//  Description : 获得左侧的区域
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_GetDispRect(
    GUI_RECT_T item_rect,
    MMITHEME_LIST_CUSTDATA_T user_data,
    BOOLEAN is_true,
    MMI_HANDLE_T win_handle
);

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemDispLayer(void);

/*****************************************************************************/
//  Description : 画图
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemSlideData(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_POINT_T      *point_ptr,
    GUI_RECT_T       item_rect
);

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemMistLayer(void);

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemMistLayer(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_RECT_T item_rect
);

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemMistLayer(void);

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemDispLayerActive(void);

/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DisplayItemSlideColor(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_POINT_T      *tp_point_ptr,
    GUI_RECT_T       item_rect
);

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawListItemSplitLine(
    CTRLLIST_OBJ_T *list_ptr,
    uint16         item_index,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : 计算item的实际rect
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CalculateListItemRect(
    GUI_RECT_T     *rect_ptr,   // OUT, item rect
    CTRLLIST_OBJ_T *list_ptr,  // 控件指针
    uint16          item_index    // IN, 当前页中item索引号，从0到page_item_num - 1
);

/*****************************************************************************/
//  Description : get the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:获得list内部状态
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetInnerState(
    CTRLLIST_OBJ_T  *list_ptr,
    GUILIST_STATE_T src_state
);

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list内部状态
/*****************************************************************************/
PUBLIC void LIST_SetInnerState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_STATE_T src_state,
    BOOLEAN         is_true
);

/*****************************************************************************/
//  Description : unselected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_UnSelectItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
);

/*****************************************************************************/
//  Description : selected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SelectItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
);

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CTRLLIST_ITEM_INFO_T *LIST_GetItemInfoPtr(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 LIST_GetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description : get the item text
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetListItemContentTextInfo(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_info_ptr,
    int16 content_index,
    MMI_STRING_T *out_str
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetState(
    uint32 *dst_state_ptr,
    uint32  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description : get state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetState(
    uint32 dst_state,
    uint32 src_state
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetItemState(
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemState(
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T src_state
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetAllItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 计算单个item的显示宽度，高度
//  显示宽度指content的显示宽度，除掉num、select和mark宽度（此三者宽度定义相同）
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ResetItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    int32 index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 当控件的区域改变，重新设置item top
/*****************************************************************************/
PUBLIC void LIST_ResetItemTop(CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : list type of
//  Global resource dependence :
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
PUBLIC BOOLEAN LIST_TypeOf(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetValidRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr
);
/*****************************************************************************/
//  Description : 终止当前正在解码的item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DestroyItemDecode(
    CTRLLIST_OBJ_T *list_ctrl_ptr   //in
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    BOOLEAN                  is_page
);

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN              is_page
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPage(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    BOOLEAN        is_page
);

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemInlineCtrl(//is destroy anim ctrl
    CTRLLIST_ITEM_INFO_T  *item_ptr,
    uint16               item_content_index
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPageExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    uint16         pre_top_index,
    uint16         pre_end_index
);

/*****************************************************************************/
//  Description : get selected icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetSelectIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr
);

/*****************************************************************************/
//  Description : get number icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetNumberIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index
);

/*****************************************************************************/
//  Description : 调整控件的区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_AdjustRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get the item page num
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetPageItemNum(CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : get the item num by height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemNumByHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              total_height,
    uint16              index,  //from 0 to total_item_num-1
    BOOLEAN             is_next
);

/*****************************************************************************/
//  Description : 判断是move item 还是move整个控件，即横向还是纵向
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetTotalHeight(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : Get current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetCurItemIndexByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : Get total item number
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetTotalItemNumByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : draw icon in item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIcon(
    const CTRLLIST_OBJ_T   *list_ctrl_ptr,
    const GUI_RECT_T       *rect_ptr,       //the rect to display this item
    const GUI_RECT_T       *disp_rect_ptr,
    MMI_IMAGE_ID_T          icon_id,
    const GUI_LCD_DEV_INFO *dev_info_ptr
);

/*****************************************************************************/
//  Description : item content画背景
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgIcon(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    GUI_RECT_T                item_rect,
    GUI_RECT_T                content_rect,
    MMI_IMAGE_ID_T            icon_id
);

/*****************************************************************************/
//  Description : item content画背景
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgColor(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    MMITHEME_COLOR_ID_E       bg_color_id,
    GUI_RECT_T                rect,
    GUI_RECT_T                inter_rect,
    MMI_IMAGE_ID_T            image_id
);


/*****************************************************************************/
//  Description : create item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CreateItemData(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T       *item_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_AdjustItemHeightAndTop(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16 pre_idx,  // 移动方向
    uint16 cur_idx
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16 pre_idx,  // 移动方向
    uint16 cur_idx
);

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
);

/*****************************************************************************/
//  Description : send need item data message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemDataMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index
);

/*****************************************************************************/
//  Description : send item content click message
//  Parameter: [In] p_list_ctrl
//             [In] item_index
//             [In] item_content_index
//             [Return] none
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendItemContentClickMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index
);

/*****************************************************************************/
//  Description : send need item content message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemContentMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index
);

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_PostMarkItemMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                item_index,
    BOOLEAN               is_selected
);

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsNeedScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get item state by index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemStateByIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    GUIITEM_STATE_T item_state
);

/*****************************************************************************/
//  Description : get item's backgroup image id
//  Global resource dependence :
//  Author:
//  Note: if(is_highlight == TRUE): get item's highlisth backgroup image
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetItemBgImageID(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index,
    BOOLEAN         is_highlight
);

/*****************************************************************************/
//  Description : 判断是否组的开头
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ItemIsGroupHead(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index
);

/*****************************************************************************/
//  Description : 获取本纪录的split字符串
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemSplitText(
    CTRLLIST_OBJ_T *list_ctrl_ptr,//IN
    uint16         cur_index,//IN
    MMI_STRING_T   *item_text_ptr//OUT
);

/*****************************************************************************/
//  Description : 判断item是否可以焦点显示
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemGetFocus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUILIST_QRERY_SELECT_ITEM_T LIST_QueryMarkItemMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16               item_index,
    MMI_MESSAGE_ID_E     msg_id
);

/*==============================================================================
Description: 计算splitbar的区域
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC GUI_RECT_T LIST_GetSplitBarRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

#ifdef UI_MULTILAYER_SUPPORT
/*==============================================================================
Description: 创建splitbar的显示层
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC BOOLEAN LIST_CreateSplitBarLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *split_rect_ptr
);

/*==============================================================================
Description: 清除Split层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void LIST_ReleaseSplitLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

#endif

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : calc top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalcEndItemByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 top_item_index
);

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 move_space
);

/*******************s**********************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemPostion(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : reset offset y by end item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByEndIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 total_height
);

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
);

/*****************************************************************************/
//  Description : get item index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemIndexByPoint(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    GUI_POINT_T    *point_ptr
);

/*****************************************************************************/
//  Description : handle the tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_HandleItemTpMove(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : ListDrawItemAnim
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemAnim(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    int32 start_x, // x起始点
    int32 end_x, // x终点
    GUI_RECT_T item_rect,
    uint32 sleep_time
);

/*****************************************************************************/
//  Description : display icon bg or color
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIconBg(
    GUI_RECT_T        *rect_ptr,      //in
    CTRLLIST_OBJ_T    *list_ctrl_ptr,      //in
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in
);

/*****************************************************************************/
//  Description : move to next item, 有滑动效果
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalculateMoveItemNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,            // 控件指针
    CTRLLIST_DIRECTION_KEY_E direct_type  // 移动方向
);

/*****************************************************************************/
//  Description : 将当前的索引调整到有效的索引
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ChangeCurItemToValidItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr // 控件指针
);

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUIITEM_THEME_STATE_E LIST_GetItemThemeState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN             is_highlight
);

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemContentEffectStr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUISTR_STYLE_T *str_style_ptr,
    uint16 item_index,
    uint16 content_index
);

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T LIST_GetItemContentTextColor(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    uint16 content_index
);

/*****************************************************************************/
//  Description : draw highlight item for slide
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawHighLightItem_ForSlide(
                                                            CTRLLIST_OBJ_T *list_ctrl_ptr,
                                                            GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                                            );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _CTRLLIST_LISTBOX_H_  */


/*@}*/
