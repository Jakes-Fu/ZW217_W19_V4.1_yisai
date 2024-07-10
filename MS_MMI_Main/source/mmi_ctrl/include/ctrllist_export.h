/*! @file guilistbox.h
@brief 此文件介绍了listbox控件的对外接口和数据类型
@author Great.Tian
@version 1.0
@date 03/23/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 
-# 03/2004        Great.Tian       Create
-# 02/12/2006     Xiaoqing.Lu      Modify
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
TXT_HELLO_TEST_LIST	20	Test List	列表测试
TXT_HELLO_TEST_LIST1	20	Test List1	Test List1
TXT_HELLO_TEST_LIST2	20	Test List2	Test List2
TXT_HELLO_TEST_LIST3	20	Test List3	Test List3
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
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_LIST_CTRL_ID;

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
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
                          MMI_TEXT_ID_T	   left_softkey_id,
                          MMI_TEXT_ID_T	   right_softkey_id
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

#ifndef  _CTRLLIST_EXPORT_H_    
#define  _CTRLLIST_EXPORT_H_   

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h"
#include "guicommon.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guiprgbox.h"
#include "mmi_theme.h"
#include "mmitheme_list.h"
#include "guianim.h"
#include "guistring.h"

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
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
typedef struct _list_obj_tag* CTRL_LIST_HANDLE_T;

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/*! @enum GUILIST_EFFECT_STR_TYPE_E
@brief 
*/
typedef uint8 GUILIST_EFFECT_STR_TYPE_E;
enum
{
    GUILIST_EFFECT_NONE,
    GUILIST_EFFECT_STR,
    GUILIST_EFFECT_REGION
};



/*! @enum GUILIST_TYPE_E
@brief 控件显示类型
*/
typedef uint8 GUILIST_TYPE_E;
enum
{
    GUILIST_TYPE_NONE,		/*!<  none */
    GUILIST_RADIOLIST_E, 	/*!<  单选(Radio)类型 */
    GUILIST_CHECKLIST_E, 	/*!<  多选(check)类型 */
    GUILIST_TEXTLIST_E,  	/*!<  Text类型 */
    GUILIST_TYPE_MAX		/*!<  系统保留 */
};




/*! @enum GUILIST_TITLE_STYLE_E
@brief 控件显示的title信息类型
*/
typedef uint8 GUILIST_TITLE_STYLE_E;
enum
{
	GUILIST_TITLE_NONE,/*!<  none */
	GUILIST_TITLE_ONLY,/*!<  标题和信息, 如1/10 */
	GUILIST_TITLE_AND_INFO = GUILIST_TITLE_ONLY, /*!<  标题和信息, 如1/10 */
	GUILIST_TITLE_MAX /*!<  系统保留 */
};

/*! @enum GUILIST_TITLE_INDEX_E
@brief title所显示的索引信息风格
*/
typedef uint8 GUILIST_TITLE_INDEX_E;
enum
{
	GUILIST_TITLE_INDEX_NONE,/*!<  none */
	GUILIST_TITLE_INDEX_DEFAULT,       /*!<  current index + 1 */
	GUILIST_TITLE_INDEX_EXCLUDE_FIRST, /*!< current index -1, total num - 1 */
	GUILIST_TITLE_INDEX_MAX, /*!<  系统保留 */
};

/*! @enum GUIITEM_DATA_TYPE_E
@brief Item数据类型，通过这些类型定义数据信息的UNION
*/
typedef uint8 GUIITEM_DATA_TYPE_E;
enum
{
    GUIITEM_DATA_NONE,  /*!<  none */
    GUIITEM_DATA_TEXT_ID,   /*!<  文本资源ID */
	GUIITEM_DATA_TEXT_BUFFER, /*!<  文本buffer */
    GUIITEM_DATA_IMAGE_ID,    /*!<  图片资源ID */ 
    GUIITEM_DATA_ANIM_DATA,   /*!<  图片详细数据数据 */
    GUIITEM_DATA_ANIM_PATH,   /*!<  图片路径 */
	GUIITEM_DATA_MAX,
};

/*! @enum GUILIST_SLIDE_STATE_E
@brief 控件滑动状态，控件启动slide和slide结束后会通知父亲
*/
typedef uint8 GUILIST_SLIDE_STATE_E;
enum
{
    GUILIST_SLIDE_NONE,/*!<  none */
    GUILIST_START_SLIDE,/*!<  开始slide状态 */
    GUILIST_END_SLIDE,/*!<  slide状态结束 */
    GUILIST_STATE_MAX/*!<  系统保留 */
};


/*! @typedef GUIITEM_STATE_T
@brief Item状态
*/
typedef uint32 GUIITEM_STATE_T;
enum
{
    GUIITEM_STATE_NONE                  = 0x00000000, //item none state, start value
    GUIITEM_STATE_SELECTED              = 0x00000001, //item选中状态, 如check list选中或text list mark状态
    GUIITEM_STATE_INVALID               = 0x00000002, //item无效状态
    GUIITEM_STATE_NEED_ITEM_CONTENT     = 0x00000004, //item需要异步插入数据, 内部使用的状态
    GUIITEM_STATE_DISABLE_MARK          = 0x00000008, //在list标记情况下, 也禁止标记
    GUIITEM_STATE_HAS_NUM   			= 0x00000010, //在list显示数字序号情况下，不显示数字序号
    GUIITEM_STATE_GRAY                  = 0x00000040, //item会话状态
    GUIITEM_STATE_SPLIT                 = 0x00000080, //item作为分隔符，不接受焦点
    GUIITEM_STATE_COSTMER_FONT_COLOR    = 0x00000100, //item自定义字体
    GUIITEM_STATE_SELFADAPT_RECT        = 0x00000200, //自适应显示区域
    GUIITEM_STATE_CONTENT_CHECK         = 0x00000400, //检查item content区域
    GUIITEM_STATE_EXPAND                = 0x00000800, //有扩展项，靠右显示">"
    GUIITEM_STATE_CHECK_SELECT_RECT     = 0x00001000, //单独check select区域
    GUIITEM_STATE_NOTHOR_SLIDE			= 0x00002000, //item不支持横向移动
    GUIITEM_STATE_HAS_AVATAR_MASK       = 0x00010000, //
    GUIITEM_STATE_HAS_INITIAL_MASK      = 0x00020000, //
    GUIITEM_STATE_HAS_INITIAL_CHECKBOX  = 0x00040000, //
    GUIITEM_STATE_HAS_AVATAR_CHECKBOX   = 0x00080000, //
    GUIITEM_STATE_HAS_TOGGLE_MARK       = 0x00100000, //
    GUIITEM_STATE_HAS_SIM_SELECTION     = 0x00200000, //
	GUIITEM_STATE_HAS_ACCESSORY_MASK    = 0x00400000, //
    GUIITEM_STATE_MAX                   = 0x08000000, //item none state, Max value
};




/*! @typedef GUILIST_STATE_T
@brief 控件的状态
*/
typedef uint32 GUILIST_STATE_T;

/* list state */
/*! @def GUILIST_STATE_ACTIVE
@brief list active状态
*/
#define GUILIST_STATE_ACTIVE            0x00000001   //list active状态
/*! @def GUILIST_STATE_FOCUS
@brief list focus状态
*/
#define GUILIST_STATE_FOCUS             0x00000002   //list focus状态
/*! @def GUILIST_STATE_NEED_TITLE
@brief list 需要标题状态，默认情况下需要
*/
#define GUILIST_STATE_NEED_TITLE        0x00000004   //list 需要标题状态，默认情况下需要
/*! @def GUILIST_STATE_NEED_HIGHTBAR
@brief list 需要高亮条状态，默认情况下需要
*/
#define GUILIST_STATE_NEED_HIGHTBAR     0x00000008   //list 需要高亮条状态，默认情况下需要
/*! @def GUILIST_STATE_NEED_PRGBAR
@brief list 需要滚动条状态，默认情况下需要
*/
#define GUILIST_STATE_NEED_PRGBAR       0x00000010   //list 需要滚动条状态，默认情况下需要
/*! @def GUILIST_STATE_NEED_PRGBAR_BLOCK
@brief list上的滚动条是否需要滑块，默认情况下需要
*/
#define GUILIST_STATE_NEED_PRGBAR_BLOCK 0x00000020   //list上的滚动条是否需要滑块，默认情况下需要
/*! @def GUILIST_STATE_SET_PRGBAR_IMG
@brief list上的滚动条滑块由用户设置
*/
#define GUILIST_STATE_SET_PRGBAR_IMG    0x00000040   //list上的滚动条滑块由用户设置
/*! @def GUILIST_STATE_NEED_SOFTKEY
@brief list 需要软键状态，默认情况下需要
*/
#define GUILIST_STATE_NEED_SOFTKEY      0x00000080   //list 需要软键状态，默认情况下需要
/*! @def GUILIST_STATE_ENABLE_MARK
@brief list 能够mark状态
*/
#define GUILIST_STATE_ENABLE_MARK       0x00000100   //list 能够mark状态
/*! @def GUILIST_STATE_OWN_SOFTKEY
@brief list 需要自定义软键状态
*/
#define GUILIST_STATE_OWN_SOFTKEY       0x00000200   //list 需要自定义软键状态
/*! @def GUILIST_STATE_INVALID
@brief list 需要更新top index状态
*/
#define GUILIST_STATE_INVALID           0x00000400   //list 需要更新top index状态
/*! @def GUILIST_STATE_SYNC_DECODE_ANIM
@brief list上的Item动画是否需要同步显示（解码），默认情况下是异步的
*/
#define GUILIST_STATE_SYNC_DECODE_ANIM  0x00000800   //list上的Item动画是否需要同步显示（解码），默认情况下是异步的
/*! @def GUILIST_STATE_CYC_SCROLL_ITEM
@brief list按上下键滚动时,是否循环滚动,默认是可以的循环滚动
*/
#define GUILIST_STATE_CYC_SCROLL_ITEM   0x00001000   //list按上下键滚动时,是否循环滚动,默认是可以的循环滚动
/*! @def GUILIST_STATE_HANDLE_LEFT_RIGHT
@brief list是否处理左右按键, 默认不处理
*/
#define GUILIST_STATE_HANDLE_LEFT_RIGHT 0x00002000   //list是否处理左右按键, 默认不处理
/*! @def GUILIST_STATE_TEXT_STYLE
@brief list使用用户自定义的text style
*/
#define GUILIST_STATE_TEXT_STYLE        0x00004000   //list使用用户自定义的text style
/*! @def GUILIST_STATE_DROPDOWNLIST
@brief list使用在dropdownlist控件中
*/
#define GUILIST_STATE_DROPDOWNLIST      0x00008000   //list使用在dropdownlist控件中
/*! @def GUILIST_STATE_ENTER
@brief list进入
*/
#define GUILIST_STATE_ENTER             0x00010000   //list进入
/*! @def GUILIST_STATE_SLIDE_IN
@brief list滑动进入，从下而上
*/
#define GUILIST_STATE_SLIDE_IN          0x00020000   //list滑动进入，从下而上
/*! @def GUILIST_STATE_SELFSET_BG
@brief list用户自设背景
*/
#define GUILIST_STATE_SELFSET_BG        0x00040000   //list用户自设背景
/*! @def GUILIST_STATE_SET_CHECK_AUTO
@brief 对于单选和多选的列表，切换时自动设置被选中的项
*/
#define GUILIST_STATE_SET_CHECK_AUTO    0x00080000   //对于单选和多选的列表，切换时自动设置被选中的项
/*! @def GUILIST_STATE_SPLIT_LINE
@brief list item 需要画分割线
*/
#define GUILIST_STATE_SPLIT_LINE        0x00100000   //list item 需要画分割线
/*! @def GUILIST_STATE_AUTO_SWITCH
@brief 自动切换Item
*/
#define GUILIST_STATE_AUTO_SWITCH       0x00200000   //自动切换Item
/*! @def GUILIST_STATE_TEXT_COLOR
@brief 这个状态只有form控件使用
*/
#define GUILIST_STATE_TEXT_COLOR        0x00400000   //这个状态只有form控件使用

#define GUILIST_STATE_AUTO_SCROLL      0x00800000   //这个状态设置中间的item滚动

/*! @def GUILIST_STATE_SPLIT_ONSIDE
@brief item作为分隔符，不接受焦点
*/
#define GUILIST_STATE_SPLIT_ONSIDE      0x01000000   //item作为分隔符时，是否支持分隔符浮动。默认不支持

/*! @def GUILIST_STATE_CHECK_ITEM
@brief content需要处理
*/
#define GUILIST_STATE_CHECK_ITEM        0x02000000   //content需要处理

/*! @def GUILIST_STATE_EFFECT_STR
@brief content文字显示特效
*/
#define GUILIST_STATE_EFFECT_STR        0x04000000   //content文字显示特效

/*! @def GUILIST_STATE_EFFECT_STR
@brief content文字显示特效
*/
#define GUILIST_STATE_TEXTSCROLL_ENABLE 0x08000000   //content文字显示特效

/*! @def GUILIST_STATE_ZOOM_ANIM
@brief list上的Item动画是否需要缩放，默认情况下是不缩放的
*/
#define GUILIST_STATE_ZOOM_ANIM         0x10000000   //list上的Item动画是否需要缩放，默认情况下是不缩放的

#define GUIITEM_GROUP_NO_BG             0x8000

/*! @struct GUILIST_NEED_ITEM_DATA_T
@brief 动态加载item时，通知应用，需要获取的数据时所携带的参数
*/
typedef struct
{
    MMI_HANDLE_T                ctrl_handle;   /*!< 控件句柄*/
    MMI_CTRL_ID_T               ctrl_id;       /*!< 控件ID*/
    uint16                      item_index;    /*!< 控件索引*/
}GUILIST_NEED_ITEM_DATA_T;

/*! @struct GUILIST_NEED_ITEM_CONTENT_T
@brief 动态加载item时，通知应用，需要获取的数据时所携带的参数
*/
typedef struct
{
    MMI_HANDLE_T                ctrl_handle;   /*!< 控件句柄 */
    MMI_CTRL_ID_T               ctrl_id;			 /*!< 控件ID */
    uint16                      item_index;		 /*!< 控件索引*/
    uint16                      item_content_index; /*!< 内容项索引*/
}GUILIST_NEED_ITEM_CONTENT_T;

typedef GUILIST_NEED_ITEM_CONTENT_T GUILIST_ITEM_CONTENT_CLICK_T;

/*! @union GUIITEM_DATA_U
@brief 数据联合体
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;           /*!< 文本资源ID */
    MMI_STRING_T                text_buffer;       /*!< 字符串信息 */
    MMI_IMAGE_ID_T              image_id;          /*!< 图片资源ID */
    GUIANIM_DATA_INFO_T*        anim_data_ptr;     /*!< 图片数据, 在set item content时传入, 不拷贝数据 */
    GUIANIM_FILE_INFO_T*        anim_path_ptr;     /*!< 图片路径, 在set item content时传入, 不拷贝数据 */
}GUIITEM_DATA_U;

/*! @struct GUIITEM_FILE_INFO_T
@brief 文件信息
*/
typedef struct
{
    wchar       *full_path_wstr_ptr; /*!<全路径名*/
    uint16      full_path_wstr_len;  /*!<全路径名长度*/
} GUIITEM_FILE_INFO_T;

/*! @struct MMI_BUTTON_T
@brief button信息
*/
typedef struct
{
    GUIITEM_FILE_INFO_T    image_info;
    MMI_STRING_T           str;
} MMI_BUTTON_T;

/*! @struct GUIITEM_CONTENT_T
@brief 数据内容
*/
typedef struct
{
    GUIITEM_DATA_U              item_data;      /*!< 数据联合体*/
    MMI_IMAGE_ID_T              icon_id;        /*!< content上左下角显示小icon，目前只对content类型为GUIITEM_DATA_IMAGE_ID时有效*/
    GUIITEM_DATA_TYPE_E         item_data_type; /*!< 数据类型*/
    MMITHEME_COLOR_ID_E         bg_color_id;    /*!< content小背景，为0的时候默认不画背景色，目前只对content类型为GUIITEM_DATA_IMAGE_ID时有效**/
    MMITHEME_COLOR_ID_E         font_color_id;  /*!< 文字背景色ID，为0的时候取默认色*/
	BOOLEAN                     is_default;     /*!< 是否是缺省值*/
#ifdef _SW_ZDT_PRODUCT_
	GUI_COLOR_T                 custom_font_color;
	GUI_COLOR_T                 custom_font_color_focus;
	BOOLEAN                     is_custom_font_color;
	uint8                       font_color_opacity;
	uint8                       font_color_opacity_focus;
	BOOLEAN                     is_opacity;
#endif
}GUIITEM_CONTENT_T;

/*! @struct GUILIST_ITEM_DATA_T
@brief 包含softkey和数据内容的结构体
*/
typedef struct
{
    GUIITEM_CONTENT_T          item_content[GUIITEM_CONTENT_MAX_NUM]; /*!< item的content, 最多支持双行5个content*/
    MMI_TEXT_ID_T			   softkey_id[GUISOFTKEY_BUTTON_NUM];     /*!< 软键对应的文字资源id*/
}GUILIST_ITEM_DATA_T;

/*! @struct CTRLLIST_ITEM_T
@brief 控件ITEM数据
@note 第一个split之前的是第0组，之后的是第1组，第一个split本身是第1组
*/
typedef struct
{
    uint32                      style_id;
    THEMELIST_ITEM_STYLE_T      *style_ptr;
    GUILIST_ITEM_DATA_T         *data_ptr; /*!< item数据*/

    GUIITEM_STATE_T             state;    /*!< item状态*/

    uint16                      first_letter;  /*!< 索引字母*/
    uint16                      item_group;    /*!< item的归类，同一个分组为一类，背景相同，从1开始，0表示不使用背景不需要分组*/
    uint16                      group_idx;     /*!< item的组索引，组即split自然分割形成的 */

    uint32                      user_data;     /*!< 用户自定义数据*/
} CTRLLIST_ITEM_T;


/*! @struct GUILIST_INIT_DATA_T
@brief 初始化控件时的数据
*/
typedef struct 
{
    GUI_BOTH_RECT_T              both_rect;             /*!< 控件的区域*/
    GUILIST_TYPE_E               type;				    /*!< 控件类型*/
	GUIPRGBOX_STYLE_E            scroll_bar_type;       /*!< 滚动条类型>*/
    uint16                       max_item_num;		    /*!< 控件最大项*/
    uint16                       max_selected_num;	    /*!< 多选框最大选项*/
    BOOLEAN                      is_dynamic_item_data;  /*!< 滑动选择移动类型*/

}GUILIST_INIT_DATA_T;

/*! @struct GUILIST_EMPTY_INFO_T
@brief 
*/
typedef struct
{
    MMI_TEXT_ID_T               text_id;           /*!< 文本资源ID*/
    MMI_STRING_T                text_buffer;       /*!< wstr_ptr不为空, 则使用text buffer, 否则使用text id或默认的id*///
}GUILIST_EMPTY_INFO_T;

/*! @struct GUILIST_POS_INFO_T
@brief 
*/
typedef struct
{
    uint16 top_item_idx;				    /*!< 显示在最顶端的Item 索引 */
    uint16 cur_item_idx;    			  /*!< 当前光标选中的ITEM 索引 */
    int32 offset_y;            /*!< 第一个ITEM的顶端相对于控件区域Y方向的偏移 */
}GUILIST_POS_INFO_T;

/*! @typedef GUILIST_ITEM_INFO_T
@brief  list内部保存item数据的结构
*/
// typedef struct GUILIST_ITEM_INFO_T GUILIST_ITEM_INFO_T;

/*! @struct GUILIST_EMPTY_INFO_T
@brief 
*/
typedef struct  
{
    // 控件传给APP
    uint16          item_index;
    uint16          content_index;

    // 以下由APP填写数据，返回给控件 begin
    GUISTR_STATE_T  state;
    GUI_COLOR_T     font_color;

    GUILIST_EFFECT_STR_TYPE_E effect_type;
    // effect_type == GUILIST_EFFECT_STR时，用户填写该数据
    MMI_STRING_T    str_info; 

    // effect_type == GUILIST_EFFECT_REGION时，用户填写该数据
    GUIFONT_REGION_STYLE_T *region_ptr;/*!< 特殊文字区域信息*/
    uint16                 region_num;/*!< 特殊文字的区域数*/

    // end
}GUILIST_EFFECT_STR_T;


/*! @struct GUILIST_MARK_ITEM_T
@brief list item标记信息
*/
typedef struct
{
    MMI_HANDLE_T                    ctrl_handle;   /*!< 控件句柄 */
    MMI_CTRL_ID_T                   ctrl_id;       /*!< 控件id */
    uint16                          item_idx;      /*!< item索引 */
    BOOLEAN                         had_maked;     /*!< 是否标记 */
}GUILIST_MARK_ITEM_T;

/*! @struct GUILIST_MARK_ITEM_T
@brief list item标记信息
*/
typedef struct
{
    MMI_HANDLE_T                    ctrl_handle;   /*!< 控件句柄 */
    MMI_CTRL_ID_T                   ctrl_id;       /*!< 控件id */
    uint16                          item_idx;      /*!< item索引 */
	MMI_MESSAGE_ID_E				msg_id;
    BOOLEAN                         is_check_select; /*!< 询问app是否执行选择/取消选择 */
}GUILIST_QRERY_SELECT_ITEM_T;

/*! @struct GUILIST_ANIM_DISP_RESULT_T
@brief 发送解析的结果给app
*/
typedef struct
{
    GUIANIM_RESULT_E    result;     /*!< 解码结果 */
    uint32              item_index; /*!< item索引 */
}GUILIST_ANIM_DISP_RESULT_T;

/*! @struct CTRLLIST_GET_DATA_FUNC
@brief 用户自绘接口
*/
typedef void (*CTRLLIST_GET_DATA_FUNC)(MMITHEME_LIST_CUSTDATA_T* data_ptr);

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Lin.Lin 
@param ctrl_id	[in]			控件ID
@param max_item_num [in]	控件项最大数
@param is_dynamic_item_data [in]	是否动态数据
@retval true/false
@return 返回成功或失败
@note 当为动态数据时，控件显示每个item都都会向窗口要数据
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxItem(
                                  MMI_CTRL_ID_T 		ctrl_id,
                                  uint16				max_item_num,
                                  BOOLEAN               is_dynamic_item_data
                                  );

/***************************************************************************//*!
@brief 设置最大选中的ITEM数
@author Lin.Lin 
@param ctrl_id	[in]	 控件ID
@param mark_cnt_max	[in]	最大选择项
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxSelectedItem(
                                          MMI_CTRL_ID_T 	ctrl_id,
                                          uint16 		mark_cnt_max
                                          );

/***************************************************************************//*!
@brief 设置控件title显示风格
@author Tracy Zhang
@param ctrl_id	[in]	 控件ID
@param title_style [in]	风格
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTitleStyle(
                                     MMI_CTRL_ID_T				ctrl_id,	//IN:
                                     GUILIST_TITLE_STYLE_E	title_style	//IN:
                                     );

/***************************************************************************//*!
@brief Append控件项
@author Great.Tian 
@param ctrl_id	[in]		控件ID
@param item_ptr	Item [in]	数据指针
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_AppendItem(
                                  MMI_CTRL_ID_T		ctrl_id,		// control id
                                  const CTRLLIST_ITEM_T*   item_ptr	// appointed Item
                                  );


/***************************************************************************//*!
@brief Insert控件项
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@param item_ptr [in]	数据指针
@param pos	[in]	制定Insert位置
@retval true/false
@return 返回成功或失败
@note pos小于最大项
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_InsertItem(
                                  MMI_CTRL_ID_T		 ctrl_id,		// control id
                                  const CTRLLIST_ITEM_T* item_ptr,	// appointed Item
                                  uint16              pos				// appointed Position, from 0 to total_item_num
                                  );

/***************************************************************************//*!
@brief Remove一个Item
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@param pos	[in]	制定被删除的项的索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveItem(
                                  MMI_CTRL_ID_T ctrl_id,	// control id
                                  uint16 		index			// Position, from 0 to total_item_num-1
                                  );


/***************************************************************************//*!
@brief Remove所有Item项
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveAllItems(
                                      MMI_CTRL_ID_T 		ctrl_id  // control id
                                      );

/***************************************************************************//*!
@brief Replace一个Item
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@param replace_item_ptr [in] 用于替换的数据
@param pos	[in]	制定被替换的item索引
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ReplaceItem(
                                   MMI_CTRL_ID_T		ctrl_id,			// control id
                                   const CTRLLIST_ITEM_T*  replace_item_ptr,	// appointed Item
                                   uint16            pos					//  appointed Position, from 0 to total_item_num-1
                                   );


/***************************************************************************//*!
@brief 设置控件项的详细数据
@author yelianna
@param ctrl_id	[in]	控件ID
@param item_data_ptr [in]	用于设置的数据
@param pos	[in]	制定被设置的item索引
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemData(
                                   MMI_CTRL_ID_T	   ctrl_id, // control id
                                   const GUILIST_ITEM_DATA_T* item_data_ptr,
                                   uint16           pos
                                   );

/***************************************************************************//*!
@brief 设置Item数据内容
@author yelianna
@param ctrl_id	[in]	控件ID
@param item_content_ptr  [in]	用于设置的数据
@param item_index	[in]	制定被设置的item索引
@param item_content_index [in]	数据内容的坐标（item_content_ptr为数组）
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContent(
                                      MMI_CTRL_ID_T	           ctrl_id, // control id
                                      const GUIITEM_CONTENT_T* item_content_ptr,
                                      uint16                    item_index,
                                      uint16                    item_content_index
                                      );

/***************************************************************************//*!
@brief 设置Item数据内容,用于透明处理
@author yelianna
@param ctrl_id  [in]	  控件ID
@param item_content_ptr  [in]   用于设置的数据
@param item_index   [in]	  制定被设置的item索引
@param item_content_index [in]  数据内容的坐标（item_content_ptr为数组）
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContentWithAlpha(
	MMI_CTRL_ID_T			 ctrl_id, // control id
	const GUIITEM_CONTENT_T *item_content_ptr,
	uint16					  item_index,
	uint16					  item_content_index
);                               

/***************************************************************************//*!
@brief 设置Item数据内容
@author xiaoqing.lu
@param ctrl_id	[in]	控件ID
@param item_content_ptr  [in]	用于设置的数据
@param item_index	[in]	制定被设置的item索引
@param item_content_index [in]	数据内容的坐标（item_content_ptr为数组）
@param is_syn_decode	[in]	是否异步
@param is_update [in]	是否立即刷新
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContentExt(
                                         MMI_CTRL_ID_T	         ctrl_id, // control id
                                         const GUIITEM_CONTENT_T *item_content_ptr,
                                         uint16                  item_index,
                                         uint16                  item_content_index,
                                         BOOLEAN                 is_syn_decode, // 只对动画控件有效
                                         BOOLEAN                 is_update // 只对动画空间有效
                                         );

/***************************************************************************//*!
@brief 获得被选中的项的索引
@author Great.Tian
@param ctrl_id	[in]	控件ID
@param idx_ptr [in/out]	用于存放所有索引的数组，可以为空
@param array_len	[in]	数组大小
@retval uint16类型
@return 返回被选中的总项数
@note 用户如果不确定传入的数组到底应该多大，那么可首先调用该接口获得总数，然后再分配内存获取具体被选中项的索引号
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemIndex(
                                           MMI_CTRL_ID_T    ctrl_id,    //in:
                                           uint16*		    idx_ptr,    //in/out:selected index array,may PNULL
                                           uint16		    array_len   //in
                                           );


/***************************************************************************//*!
@brief 设置某一项为选中项
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@param item_pos  [in]	被设置的索引
@param is_selected	[in]	TRUE : selected; FALSE: unselected
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSelectedItem(
                                       MMI_CTRL_ID_T 	ctrl_id,
                                       uint16 			item_pos,		//from 0 to total_item_num-1
                                       BOOLEAN 			is_selected		//TRUE : selected; FALSE: unselected
                                       );

/***************************************************************************//*!
@brief 获得一个Item指针
@author James.Zhang
@param ctrl_id	[in]	控件ID
@param index  [in]	索引
@retval 指针
@return 返回某一项的指针
@note 
*******************************************************************************/
PUBLIC const CTRLLIST_ITEM_T* CTRLLIST_GetItem(
                                               MMI_CTRL_ID_T ctrl_id,
                                               uint16        index  //from 0 to total_item_num-1
                                               );


/***************************************************************************//*!
@brief 获得当前高亮的Item索引
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@retval uint16
@return 返回Item索引
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurItemIndex(
                                      MMI_CTRL_ID_T 		ctrl_id
                                      );


/***************************************************************************//*!
@brief 设置当前高亮的索引
@author  Lin.Lin
@param ctrl_id	[in]	控件ID
@param current_index	[in]	被设置的索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurItemIndex(
                                       MMI_CTRL_ID_T 		ctrl_id,
                                       uint16 				current_index   //current index
                                       );

/***************************************************************************//*!
@brief 改变显示类型
@author Guanyong.Zhang 
@param ctrl_id	[in]	控件ID
@param type	[in]	显示类型
@param is_flash [in]	是否立即刷新
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ChangeDisplayType(
                                         MMI_CTRL_ID_T		ctrl_id,
                                         GUILIST_TYPE_E	type,
                                         BOOLEAN				is_flash       // if display immediately
                                         );


/***************************************************************************//*!
@brief 获得一页显示的项数
@author Lin.Lin 
@param ctrl_id	[in]	控件ID
@retval uint16
@return 返回项数
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetPageItemNum(
                                     MMI_CTRL_ID_T 		ctrl_id // control id
                                     );


/***************************************************************************//*!
@brief 获取顶端Item索引
@author Lin.Lin 
@param ctrl_id	[in]	控件ID
@retval uint16
@return 返回索引号
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetTopItemIndex(
                                      MMI_CTRL_ID_T 		ctrl_id
                                      );


/***************************************************************************//*!
@brief 设置顶端的Item
@author Lin.Lin 
@param ctrl_id	[in]	控件ID
@param top_index	[in]	被设置的Item
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTopItemIndex(
                                       MMI_CTRL_ID_T 		ctrl_id,
                                       uint16 				top_index   //top index
                                       );


/***************************************************************************//*!
@brief I当前Item是否有效
@author Lianxiang
@param ctrl_id	[in]	控件ID
@retval true/false
@return 返回成功或失败
@note 无效的Item将灰化显示，但会执行按键及触笔操作
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsCurrItemInvalid(
                                         MMI_CTRL_ID_T 		ctrl_id
                                         );


/***************************************************************************//*!
@brief 是否显示Title信息
@author Lin.Lin
@param ctrl_id	[in]	控件ID
@param title_index_type	[in]	Title信息显示的类型
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetTitleIndexType(
                                      MMI_CTRL_ID_T		ctrl_id,
                                      GUILIST_TITLE_INDEX_E title_index_type
                                      );

/***************************************************************************//*!
@brief 是否需要显示滚动条
@author Lin.Lin 
@param ctrl_id	[in]	控件ID
@param is_need_prgbar 	[in]	TRUE：显示；FALSE：不显示
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbar(
                                  MMI_CTRL_ID_T  		ctrl_id,
                                  BOOLEAN 			is_need_prgbar
                                  );

#ifdef ADULT_WATCH_SUPPORT
/***************************************************************************//*!
@brief set progressbar theme
@author fulu.song
@param ctrl_id	[in]	控件ID
@param theme 	[in]	theme to apply
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetPrgbarTheme(
    MMI_CTRL_ID_T  ctrl_id,
    MMITHEME_PRGBOX_TYPE_E theme
);
#endif
/***************************************************************************//*!
@brief 获得所有Item数
@author Tracy Zhang
@param ctrl_id [in]	控件ID
@retval uint16
@return 返回总项数
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalItemNum(							//RETURN: 
                                      MMI_CTRL_ID_T	ctrl_id		//IN:
                                      );

/***************************************************************************//*!
@brief 设置所有项被选中
@author hanbing.zhang
@param ctrl_id	[in]	控件ID
@param is_select_all	[in]	TRUE：所有被选中，FALSE：取消选中所有
@retval void
@return void
@note 如果不能选中所有，那么将根据最大数从第一个开始选中
*******************************************************************************/
PUBLIC void CTRLLIST_SetAllSelected(
                                   MMI_CTRL_ID_T ctrl_id,
                                   BOOLEAN       is_select_all //TRUE : selected; FALSE: unselected
                                   );


/***************************************************************************//*!
@brief  设置控件是否可以标记
@author hanbing.zhang
@param ctrl_id	[in]	控件ID
@param is_enable_mark	[in]	TRUE：可以被标记，FALSE：不能被标记
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextListMarkable(
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN       is_enable_mark
                                           );


/***************************************************************************//*!
@brief  设置控件Item前的数字的起始值
@author James.Zhang
@param ctrl_id	[in]	控件ID
@param num_start_index	[in]	起始值
@retval void
@return void
@note 系统只保留1-20个数字的图片资源ID供list控件使用
*******************************************************************************/
PUBLIC void CTRLLIST_SetNumberListStartIndex(
                                            MMI_CTRL_ID_T ctrl_id,
                                            uint16        num_start_index
                                            );


/***************************************************************************//*!
@brief 设置Softkey是否有应用控制显示
@author hanbing.zhang
@param ctrl_id	[in]	控件ID
@param is_own_softkey	[in]	TRUE:窗口自定义softkey，FALSE:使用控件softkey
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetOwnSofterKey(
                                    MMI_CTRL_ID_T ctrl_id,
                                    BOOLEAN       is_own_softkey
                                    );

/*****************************************************************************/
//  Description : 设置控件是否需要分割线
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedSplidLine(
                                     MMI_CTRL_ID_T ctrl_id,
                                     BOOLEAN       is_true
                                     );

/*****************************************************************************//*!
@brief Set the item state
@author xiaoqing.lu
@param ctrl_id	[in]	控件ID
@param item_index [in]		item 索引
@param src_state [in]		要设置的状态
@retval true/false
@return TRUE：设置成功或者失败
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemStateById(
                                        MMI_CTRL_ID_T   ctrl_id, 
                                        uint32          item_index, 
                                        GUIITEM_STATE_T src_state,
                                        BOOLEAN is_true
                                        );

/***************************************************************************//*!
@brief 设置控件的状态
@author James.Zhang
@param list_ptr	[in]	控件指针
@param src_state	[in]	要设置的状态
@param is_true  [in]	 设置/取消设置
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetListState( 
                                 MMI_CTRL_ID_T   ctrl_id,
                                 GUILIST_STATE_T    src_state,
                                 BOOLEAN is_true
                                 );


/***************************************************************************//*!
@brief 获得控件的某种状态是否被设置
@author James.Zhang
@param list_ptr	[in]	控件指针
@param src_state	[in]	要获得的状态
@retval true/false
@return TRUE：控件具有该状态，FALSE：控件没有该状态
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListState(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    GUILIST_STATE_T    src_state
                                    );


/***************************************************************************//*!
@brief 获得控件的某种状态是否被设置
@author James.Zhang
@param ctrl_id	[in]	控件ID
@param src_state [in]		要获得的状态
@retval true/false
@return TRUE：控件具有该状态，FALSE：控件没有该状态
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListStateById(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUILIST_STATE_T src_state
                                        );


/***************************************************************************//*!
@brief  获得控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr	[in]	控件指针
@param index	 [in]	要获得的Item索引
@param user_data_ptr	[in]	用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemData(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 index,
                                   uint32* user_data_ptr
                                   );


/***************************************************************************//*!
@brief  设置控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr	[in]	控件指针
@param index	 [in]	要获得的Item索引
@param user_data_ptr	[in]	用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemUserData(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 index,
                                   uint32* user_data_ptr
                                   );

/***************************************************************************//*!
@brief 矫正当前Item索引
@author Lin.Lin
@param ctrl_id [in]	控件ID
@retval void
@return void
@note 当删除某一个项时，可能存在当前Item设置错误的情况，通过该接口可以矫正
*******************************************************************************/
PUBLIC void CTRLLIST_ValidCurItemPos( 
									MMI_CTRL_ID_T	ctrl_id 
									);

/***************************************************************************//*!
@brief 设置控件的背景色
@author James.Zhang
@param ctrl_id [in]	控件ID
@param back_color [in]	背景色
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgColor(
                                  MMI_CTRL_ID_T	ctrl_id ,
                                  GUI_COLOR_T   back_color
                                  );

/***************************************************************************//*!
@brief 设置List控件的默认图片ID
@author xiaoqing.lu
@param ctrl_id [in]	控件ID
@param def_img_id [in]	解码过程中的默认图片
@param err_img_id [in]	解码失败后的默认图片
@retval true/false
@return 返回成功或失败
@note 两个默认图片都是为动态加载图片时，内嵌的动画控件所用
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIcon(
                                      MMI_CTRL_ID_T     ctrl_id,        //in
                                      MMI_IMAGE_ID_T    def_img_id,  // default image id
                                      MMI_IMAGE_ID_T    err_img_id   // error image id
                                      );
/*****************************************************************************/
//  Description : 设置List控件的默认图片ID,为了能够将image_id为NULL,能够设置成功
//  Global resource dependence :
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIconEx(
    MMI_CTRL_ID_T     ctrl_id, //in
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
);

/***************************************************************************//*!
@brief 设置是否需要显示高亮条
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true [in] TRUE要显示高亮条，FALSE不要显示。默认为TRUE
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedHiLightBar(
                                      MMI_CTRL_ID_T  ctrl_id,
                                      BOOLEAN is_true
                                      );


/***************************************************************************//*!
@brief  设置空列表信息
@author James.Zhang
@param ctrl_id [in] 控件ID
@param empty_ptr [in] 空列表信息
@retval void
@return void
@note 请在窗口open消息时设置
*******************************************************************************/
PUBLIC void CTRLLIST_SetEmptyInfo(
                                 MMI_CTRL_ID_T               ctrl_id,   //控件id
                                 const GUILIST_EMPTY_INFO_T* empty_ptr  //空列表信息
                                 );

/***************************************************************************//*!
@brief 设置是否显示滚动条的滑块
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true [in] TRUE显示，FALSE不显示。默认为TRUE
@retval void
@return void
@note 这个接口设置是否需要滚动条上的滑块
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbarBlock(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN is_true
                                       );


/***************************************************************************//*!
@brief 设置背景图片资源ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param image_id [in] 图片资源ID
@param is_transparent [in] 是否透明显示
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgImage(
                                  MMI_CTRL_ID_T	ctrl_id,
                                  MMI_IMAGE_ID_T image_id,
                                  BOOLEAN		 is_transparent
                                  );

/*****************************************************************************/
//  Description : set the user bg 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetUserBg(
                               MMI_CTRL_ID_T	ctrl_id,
                               BOOLEAN		is_user_bg
                               );

/***************************************************************************//*!
@brief 设置显示动画的时候是否同步解码，默认是异步解码
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_sync_decode [in] TRUE：同步，FALSE：异步
@retval true/false
@return 返回设置之前的状态
@note 返回设置之前的状态
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSyncDecodeAnim(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         BOOLEAN is_sync_decode
                                         );

/*****************************************************************************//*!
@brief list上的Item动画是否需要缩放，默认情况下是不缩放的
@author fangfang.yao
@param ctrl_id [in] 控件ID
@param is_zoom [in] TRUE：缩放，FALSE：不缩放
@retval TRUE/FALSE
@return 操作是否成功
@note 操作是否成功
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetZoomAnim(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_zoom
);

/***************************************************************************//*!
@brief 设置是否循环滚动
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_cyc_scroll [in] TRUE：循环，FALSE：不循环
@note 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
*******************************************************************************/
PUBLIC void CTRLLIST_SetCycScroll(
                                  MMI_CTRL_ID_T  ctrl_id,
                                  BOOLEAN is_cyc_scroll
                                  );


/***************************************************************************//*!
@brief 设置是否响应左右按键
@author James.Zhang
@param ctrl_id [in] 控件ID
@param is_handle [in] TRUE：响应，FALSE：不响应
@note 默认不响应左右按键
*******************************************************************************/
PUBLIC void CTRLLIST_SetHandleLeftRight(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN        is_handle
                                       );

/***************************************************************************//*!
@brief 设置是否允许显示边框
@author Jassmine
@param ctrl_id		[in] 控件ID
@param is_permit	[in] 是否允许显示
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_PermitBorder(
									MMI_CTRL_ID_T	ctrl_id,
                                    BOOLEAN         is_permit
                                    );

/***************************************************************************//*!
@brief 设置控件的字体和颜色
@author James.Zhang
@param ctrl_id [in] 控件ID
@param font [in] 字体
@param font_color [in] 字体颜色
@retval true/false
@return 返回成功或失败
@note  默认不需要设置.与GUILIST_STATE_TEXT_STYLE状态同时使用
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextFont(
                                   MMI_CTRL_ID_T   ctrl_id,
                                   GUI_FONT_T      font,
                                   GUI_COLOR_T     font_color
                                   );
/***************************************************************************//*!
@brief 设置控件当前item的字体颜色
@author dandan.cao
@param ctrl_id [in] 控件ID
@param font [in] 字体
@param font_color [in] 字体颜色
@retval true/false
@return 返回成功或失败
@note  默认不需要设置.与GUIITEM_STATE_COSTMER_FONT_COLOR状态同时使用
*******************************************************************************/
PUBLIC BOOLEAN CTRLIST_SetOneItemFontColor(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                uint32                 item_index,
                                                GUI_COLOR_T     item_font_color,
                                                BOOLEAN            is_true
                                            );
/*****************************************************************************//*!
@brief 设置list text font color
@author xiaoqing.lu
@param ctrl_id	[in]	控件ID
@param font_color [in]		item 字体颜色
@retval true/false
@return TRUE：设置成功或者失败
@note 默认不需要设置, 与GUIITEM_STATE_COSTMER_FONT_COLOR状态同时使用
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemTextFontColor(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_COLOR_T     font_color
                                            );

/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_handle [in] 控件HANDLE
@param rect_ptr [in] 区域
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetRect(
                            MMI_HANDLE_T    ctrl_handle,
                            GUI_RECT_T      *rect_ptr
                            );


/***************************************************************************//*!
@brief 设置控件是否支持*,\#切换页
@author Jassmine
@param ctrl_handle [in] 控件HANDLE
@param is_handle [in] TRUE:支持，FALSE:不支持
@retval true/false
@return 返回成功或失败
@note 默认支持
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetHandleStarHash(
                                         MMI_HANDLE_T   ctrl_handle,
                                         BOOLEAN        is_handle
                                         );


/***************************************************************************//*!
@brief 设置是否支持slide状态
@author Xiaoqing.Lu
@param ctrl_id [in] 控件HANDLE
@param is_slide_used [in] TRUE:支持，FALSE:不支持
@retval true/false
@return 返回成功或失败
@note  默认情况，我们支持slide状态
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSlideState(
                                     MMI_CTRL_ID_T ctrl_id,
                                     BOOLEAN is_slide_used
                                     );

/***************************************************************************//*!
@brief 获取是否支持slide状态
@author Xiaoming.Ren
@param ctrl_id [in] 控件HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetSlideState(
                                     MMI_CTRL_ID_T ctrl_id
                                     );

/***************************************************************************//*!
@brief 设置顶端Item的偏移
@author bin.ji
@param ctrl_id [in] 控件ID
@param offset_y [in] Y方向的偏移
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetTopItemOffset(
                                      MMI_CTRL_ID_T ctrl_id,
                                      int32 offset_y
                                      );


/***************************************************************************//*!
@brief get top item offset
@author xiaoqinglu
@param ctrl_id [in] 控件ID
@param offset_y_ptr [in/out] Y方向的偏移
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetTopItemOffset(
                                        MMI_CTRL_ID_T ctrl_id,
                                        int32* offset_y_ptr
                                        );

/***************************************************************************//*!
@brief 设置是否支持3d
@author James.Zhang
@param ctrl_id [in] 控件ID
@param is_disable_3d [in] TRUE支持，FALSE不支持
@retval true/false
@return 返回成功或失败
@note 默认支持
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDisable3D(
                                    MMI_CTRL_ID_T ctrl_id,
                                    BOOLEAN is_disable_3d
                                    );

/*****************************************************************************/
//  Description : set scroll bar style
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetListScrollBarStyle(
                                    MMI_CTRL_ID_T	  ctrl_id, // control id
                                    GUIPRGBOX_STYLE_E scroll_style
                                    );

/***************************************************************************//*!
@brief 设置边距
@author Lin.Lin
@param ctrl_id [in] 控件ID
@param left_space [in] 左边距
@param right_space [in] 右边距
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMargin(
                                 MMI_CTRL_ID_T ctrl_id, // control id
                                 uint16        left_space,
                                 uint16        right_space
                                 );


/***************************************************************************//*!
@brief 设置内嵌滚动条的背景图
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param image_id [in] 资源图片ID
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetPrgBarImageId(
                                        MMI_CTRL_ID_T	ctrl_id,
                                        MMI_IMAGE_ID_T image_id
                                        );


/***************************************************************************//*!
@brief 设置Item是否灰化
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引
@param is_gray [in] 是否灰化
@retval true/false
@return 返回成功或失败
@note 被设置灰化的Item，将不执行按键及触笔操作
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemGray(
                                   MMI_CTRL_ID_T ctrl_id, 
                                   uint32 item_index, 
                                   BOOLEAN is_gray
                                   );

/***************************************************************************//*!
@brief 设置ITEM是否有效
@author Great.Tian
@param ctrl_id [in] 控件指针
@param pos [in] 索引
@param is_invalid [in] 是否有效
@retval true/false
@return 返回成功或失败
@note 被设置无效的Item，但仍执行按键及触笔操作
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemInvalid(
                                      MMI_CTRL_ID_T ctrl_id,	// control id
                                      uint16        pos,		// 指定的位置, from 0 to total_item_num-1
                                      BOOLEAN       is_invalid
                                      );


/***************************************************************************//*!
@brief 获得Item区域
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in]  索引
@param rect_ptr [in/out] 矩形的指针
@retval true/false
@return 返回成功或失败
@note Item区域可能随时都会改变
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemRect(
                                   MMI_CTRL_ID_T  ctrl_id,    //in:
                                   uint16		  item_index, //in
                                   GUI_RECT_T     *rect_ptr     //in/out
                                   );

/***************************************************************************//*!
@brief 改变多选框的默认softkey ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_cancel_id  [in] TEXT ID
@param text_select_id [in] TEXT ID
@param text_return_id [in] TEXT ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListDefSoftkeyID(
                                             MMI_CTRL_ID_T ctrl_id,
                                             MMI_TEXT_ID_T       text_cancel_id,
                                             MMI_TEXT_ID_T       text_select_id,
                                             MMI_TEXT_ID_T       text_return_id
                                             );
/*****************************************************************************/
//  Description : set check list default image
//  Global resource dependence :
//  Author:miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImage(
                                             MMI_CTRL_ID_T ctrl_id,
                                             MMI_IMAGE_ID_T       image_unselected,
                                             MMI_IMAGE_ID_T       image_selected
                                             );
/*****************************************************************************/
//  Description : set check list image display left or right
//  Global resource dependence :
//  Author:miao.xi
//  Note:    BOOLEAN  is_left: (1:left 0:right)
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImagePosition(
                                             MMI_CTRL_ID_T ctrl_id,
                                             BOOLEAN       is_left// 1:left 0:right
                                             );
/***************************************************************************//*!
@brief 改变单选框的默认softkey ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_ok_id [in] TEXT ID 
@param text_return_id  [in/out] TEXT ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_ChangeRadioListDefSoftkeyID(
                                                MMI_CTRL_ID_T ctrl_id,
                                                MMI_TEXT_ID_T       text_ok_id,
                                                MMI_TEXT_ID_T       text_return_id
                                                );
                                                                                             
/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetSwitchItemTime(
                                      MMI_CTRL_ID_T ctrl_id,
                                      uint32 switch_time
                                      );

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StartSwitchItem(
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
//  Description : stop auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StopSwitchItem(
                                   MMI_CTRL_ID_T ctrl_id
                                   );

/*****************************************************************************/
//  Description : 设置是否自动切换item
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 默认时不自动切换
/*****************************************************************************/
PUBLIC void CTRLLIST_SetAutoSwitchItem(
                                      MMI_CTRL_ID_T  ctrl_id,
                                      BOOLEAN is_true
                                      );

/*****************************************************************************/
//  Description : 获得总的页数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 每页显示的条目根据list区域及item高度计算所得
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalPageNum(							//RETURN: 
                                      MMI_CTRL_ID_T	ctrl_id		//IN:
                                      );

/*****************************************************************************/
//  Description : 获得当前页索引号
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 以当前页的最上面一个Item为准计算
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurrentPageIndex(							//RETURN: 
                                          MMI_CTRL_ID_T	ctrl_id		//IN:
                                          );

/***************************************************************************//*!
@brief 获取纯净的item index
@author lianxiang.zhou
@param ctrl_id [in] 控件ID
@param gross_index  [in] TEXT ID
@param pure_index_ptr [in] TEXT ID
@retval true/false
@return 返回是否获得了纯净的index
@note 所谓纯净的，即不计split类的
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetPureIndex(
                                    MMI_CTRL_ID_T ctrl_id,
                                    uint16 gross_index,
                                    uint16 *pure_index_ptr
                                    );

/***************************************************************************//*!
@brief 设置是否支持分隔符浮动效果，默认不支持
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true  [in] 设置或取消设置
@retval true/false
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetSplitOnside(
                                    MMI_CTRL_ID_T	ctrl_id,
                                    BOOLEAN    is_true
                                    );

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu 
@param base_win_handle	[in] Must valid win handle, if not, create failed
@param base_ctrl_handle [in] You can set 0
@param ctrl_id          [in] You can set 0
@param init_data_ptr    [in] if null, will create ctrl by default param
@retval 0 or valid handle
@return 返回0或者有效的控件ID
@note 注意，当初始化数据init_data_ptr为空时，将创建一个空的控件，该控件不能添加Item
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLLIST_CreateListBox(
                                          MMI_HANDLE_T base_win_handle, 
                                          MMI_HANDLE_T base_ctrl_handle,
                                          MMI_CTRL_ID_T ctrl_id,
                                          GUILIST_INIT_DATA_T *init_data_ptr
                                          );


/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu 
@param ctrl_id          [in] ctrl ID
@retval true or false
@return 
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsTpDown(
                                MMI_CTRL_ID_T ctrl_id
                                );

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu 
@param ctrl_id          [in] ctrl ID
@retval item number
@return item number
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemNum(
                                         MMI_CTRL_ID_T    ctrl_id//in:
                                         );

/*****************************************************************************/
//  Description : 设置是否由用户built item layer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 默认FALSE,控件自动built
/*****************************************************************************/
PUBLIC void CTRLLIST_SetCusBltItemLayer(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN        is_cus_blt_layer
                                       );
                                       
/*****************************************************************************//*!
@brief 设置控件的Item显示的layer
@author Xiaoqing.lu 
@param ctrl_id              [in] ctrl ID
@param lcd_dev_info_ptr     [in] lcd_dev_info_ptr ID
@retval item number
@return item number
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemLayer(
                                    MMI_CTRL_ID_T ctrl_id,
                                    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                    );

/*****************************************************************************//*!
@brief 设置控件的Item显示的layer
@author Xiaoqing.lu 
@param ctrl_id              [in] ctrl ID
@param get_data_func     [in] func ptr
@retval true/false
@return true/false
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RegGetDataFunc(
                                      MMI_CTRL_ID_T ctrl_id,
                                      CTRLLIST_GET_DATA_FUNC own_draw_func
                                      );

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:设置list支持横向滑动，默认不支持
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemHorMove( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   BOOLEAN         is_true
                                   );

/*****************************************************************************/
//  Description : CTRLLIST_SetItemLayer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemLayer(
                                    MMI_CTRL_ID_T ctrl_id,
                                    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                    );

/***************************************************************************//*!
@brief 设置禁止tip显示的状态
@author Lianxiang.Zhou
@param ctrl_id       [in] ctrl ID
@param is_forbid     [in] 是否禁止tip
@retval true/false
@return true/false
@note 默认是显示的
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetForbidTip(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_forbid
);
                                               
#ifdef ATEST_SUPPORT
/*****************************************************************************/
// 	Description : get item content rect
//	Global resource dependence : 
//  Author: lin.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_GetItemContentRect(
                                 MMI_CTRL_ID_T   ctrl_id,
                                 uint16          item_index,
                                 uint16          item_content_index,
                                 GUI_RECT_T*     disp_rect_ptr,//out
                                 GUI_RECT_T*     clip_rect_ptr //out
                                 );
#endif //ATEST_SUPPORT

/***************************************************************************//*!
@brief 获得List位置信息
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@retval 
@return
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetCurPosInfo(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUILIST_POS_INFO_T *pos_info_ptr
                                      );

/***************************************************************************//*!
@brief 设置List位置信息
@author Great.Tian 
@param ctrl_id	[in]	控件ID
@retval 
@return
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurPosInfo(
                                       MMI_CTRL_ID_T ctrl_id,
                                       GUILIST_POS_INFO_T *pos_info_ptr
                                       );

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListSoftkeyIndex(
                                                MMI_CTRL_ID_T ctrl_id,
                                                GUISOFTKEY_INDEX  select_cancel_index
                                                );

/*****************************************************************************/
//  Description : 设置Item 显示风格
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    THEMELIST_ITEM_STYLE_T *style_ptr
);

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:设置list支持向左滑动，默认不支持；该函数请搭配 CTRLLIST_SetItemHorMove() 配置使用
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemLeftMove(
                                                    MMI_CTRL_ID_T ctrl_id,
                                                    BOOLEAN       is_true
                                                );

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author: fangfang.yao
//  Note: item被点击时，list是否需要高亮条状态，默认情况下不显示高亮条
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemIsNeedHighBarIfClick(
                                                                MMI_CTRL_ID_T ctrl_id,
                                                                BOOLEAN       is_true
                                                            );

/*****************************************************************************/
//  Description : whether support dec anim when sliding or not
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetIsSupportSlideDecAnim(
                                            MMI_CTRL_ID_T ctrl_id,
                                            BOOLEAN is_support_slide_dec_anim
                                          );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _CTRLLIST_EXPORT_H_  */


/*@}*/


