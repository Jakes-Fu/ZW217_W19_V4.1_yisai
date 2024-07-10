/*! @file guirichtext.h
@brief Richtext控件的创建和控制相关的类型和函数 
@author lianxiang.zhou
@version 1.0
@date 05/30/2007
@remarks Copyright 2007-2011 Spreadtrum, Incoporated. All Rights Reserved.
@details 本文件是richtext控件的主体部分，提供创建和控制richtext所需要的所有类型和函数。
- 创建和销毁richtext
- 设定richtext的各类参数
- 增加、删除和修改richtext的显示项

@note 05/30/2007 - liqing.peng - Create
@sa richtextGrp
@sa @ref page_RichText
*******************************************************************************/

/*! @page page_RichText RichText(RichText)

-# @ref sec_richtext_intro
-# @ref sec_richtext_usage
-# @ref sec_richtext_oprator
-# @ref sec_richtext_support
-# @ref sec_richtext_settings
-# @ref sec_richtext_how_to_use
    -# @ref ssec_richtext_create
    -# @ref ssec_richtext_item
    -# @ref ssec_richtext_function_settings
    -# @ref ssec_richtext_display_settings

@section sec_richtext_intro Richtext控件概述

Richtext控件是显示图片和文字的控件。按照设定的项(item)从上到下顺次显示，在一个项中可以有至多一个图片和一段文字，至少有一个图片或者一个文字。即一个项可能，且仅可能是下面列出来的情况：
-# 仅图片，支持的格式详见 @ref _GUIRICHTEXT_IMAGE_TYPE_E
-# 仅文字，支持的格式详见 @ref _GUIRICHTEXT_TEXT_TYPE_E
-# 图片与文字的混合，文字和图片的相对位置设定详见 @ref _GUIRICHTEXT_ALIGN_E

@section sec_richtext_usage richtext的用途

用于在应用中显示文字和图片，可对文字和图片的显示形式做特定的调节。比如详情、短信和电话本等有固定合适的应用显示。

@section sec_richtext_oprator Richtext的操作

对于richtext，我们可控的方面包括：
- 创建和销毁
- 增减、删除和修改项(item)
- 设置richtext控件的整体显示参数
- 设置单独项的显示参数
- 设置richtext控件的附加功能，如剪贴板操作。

@section sec_richtext_support richtext支持的功能

richtext的基本功能是“显示”，所以，在显示的时候，会有一些必要的支持：
- 自动分页、翻页功能。
- 支持按键和触笔（如果系统支持）的操作。
- 翻页中自动对齐项，即会在翻页的过程中，微调翻页距离，使得“项”边缘对齐。
- 翻页中自定对齐文字，即会在翻页的过程中，微调翻页距离，使得“文字行”边缘对齐。
- 系统“剪切板”功能（如果系统支持，默认打开。亦可设定参数为richtext不支持该功能）。参见 @ref GUIRICHTEXT_DisableClipBoard
- 字符串解析功能，即按照字符串特征，标出url、email和phone number。参见 @ref page_Url
- 对解析出来的字符串，给予显示焦点。也可以在不解析的情况下，对item设置焦点。参见 @ref GUIRICHTEXT_TAG_TYPE_E

@section sec_richtext_settings richtext的可设置参数

在richtext的显示中，用户会对显示的细节作出客户化的需求，我们提供一些参数的设定，这些设定，包括控件整体的设定和单独项的设定。
- 控件设定 @ref ssec_richtext_function_settings , @ref ssec_richtext_display_settings
- 项设定 @ref ssec_richtext_item
- 关于各space，参看下图
@image html richtext_space_settings.jpg

@warning richtext的容量是有限的，下列情况下，将会做一些截断：
- 文字长度超过最大长度 @ref GUIRICHTEXT_TEXT_BUF_MAX_LEN
- 文字的显示高度超过一定长度（int16的最大值，系统rect的限制）——考虑在将来重构中去除此限制。

@section sec_richtext_how_to_use 如果使用Richtext控件？

使用richtext控件需要先创建该控件，然后加入显示项，在现实之前，可以设置各种显示参数，大致来讲，可以按照如下步骤进行。
@note 更详细的信息，请参考 @ref richtextGrp 。

@subsection ssec_richtext_create richtext控件的创建和销毁

跟平台的其他控件一样，richtext控件可以动态创建，也可以静态创建。参看控件的一般操作 @ref page_general 。

@subsection ssec_richtext_item 增加、删除和修改item

richtext的显示主题就是各个项(item)，所以，只有有了项，richtext才有意义。我们可以增加、删除和修改项。
-# @ref GUIRICHTEXT_AddItem
-# @ref GUIRICHTEXT_DeleteItem
-# @ref GUIRICHTEXT_ModifyItem
-# 更多有关项的操作，请参看 @ref richtextGrp 中提供的函数列表

@note 在操作项的同时，设置“项的显示设定”，项的显示只能在操作项的时候改变。参见 @ref GUIRICHTEXT_ITEM_T 。

@subsection ssec_richtext_function_settings 功能设定

作为一个显示控件，richtext可设置的功能相对较少，目前只有：
-# 焦点 @ref GUIRICHTEXT_SetFocusMode , @ref GUIRICHTEXT_DisableClipBoard
-# 剪切板 @ref GUIRICHTEXT_EnableClipBoard
-# @ref GUIRICHTEXT_SaveInfo
-# @ref GUIRICHTEXT_RestoreInfo

@subsection ssec_richtext_display_settings 显示设定

显示是richtext的主要功能，所以关于显示设定的设置项比较多。
-# @ref GUIRICHTEXT_SetRect
-# @ref GUIRICHTEXT_SetFont
-# @ref GUIRICHTEXT_SetFontColor
-# @ref GUIRICHTEXT_SetCharSpace
-# @ref GUIRICHTEXT_SetLineSpace
-# @ref GUIRICHTEXT_SetImgTxtSpace
-# @ref GUIRICHTEXT_SetItemSpace
-# @ref GUIRICHTEXT_SetBorderSpace
-# @ref GUIRICHTEXT_SetItemBorderSpace
-# @ref GUIRICHTEXT_SetTxtMargin
-# @ref GUIRICHTEXT_SetFocusLineColor
-# @ref GUIRICHTEXT_SetFocusBgColor
-# @ref GUIRICHTEXT_SetBgColor
-# @ref GUIRICHTEXT_SetBgImage
-# @ref GUIRICHTEXT_SetBorder
-# @ref GUIRICHTEXT_SetBorderColor
-# @ref GUIRICHTEXT_SetBorderFocusColor
-# @ref GUIRICHTEXT_SetAnimParsingIcon
-# @ref GUIRICHTEXT_SetAnimErrorIcon
-# @ref GUIRICHTEXT_SetFocusItem
-# @ref GUIRICHTEXT_SetImgDecodeSyn

@note 并不是说提供了这么多的设置项，我们就需要设置每一项。通常情况下，都会有一个相对比较合理的默认项，所以，只有在需要的时候，才去设置特别的显示项。

@sa @ref page_Url
@sa richtextGrp
*******************************************************************************/


/*! @addtogroup richtextGrp RichText
@brief Richtext控件模块
@details 该模块有两部分
- richtext的主体部分，创建并控制richtext控件
- richtext的字符判定部分，即url部分。这部分是一个通用的字符串特征解析模块

@sa urlGrp
@sa @ref page_RichText
@{
*******************************************************************************/

#ifndef _CTRLRICHTEXT_EXPORT_H_
#define _CTRLRICHTEXT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

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

/*! @def GUIRICHTEXT_FILENAME_MAX_LEN
@brief 控件能接受的最大文件名长度
@details 文件名长度的限定，本不是必须的，但是为了避免一些无端的错误，比如传入一个超长的文件名，而平台的文件名长度是有限制的，故而，再此加入一个限制。文件名的存储控件，在实例数据中的存储是用的动态申请内存。
*/
#define GUIRICHTEXT_FILENAME_MAX_LEN (256)

/*! @def GUIRICHTEXT_TEXT_BUF_MAX_LEN
@brief 控件一个item中文字的最大长度
@details 这个长度是很重要的，作为一个控件，会在一些“常识”基础上做一些控制，richtext作为一个按条目显示的控件，每个条目显示4k的文字，应该是足够了。这部分的限制，并不能保证动态内存的申请超出系统的限制，因为对条目的数量是没有限制的。
*/
#define GUIRICHTEXT_TEXT_BUF_MAX_LEN (10*1024)

/*! @def GUIRICHTEXT_TEXT_MAX_LEN
@brief 为普通item定义的文字长度，仅是为方便应用
@details 更直白的讲，这个概念是为了兼容旧版本的richtext，这是一个稍显“无理”的兼容，可以不加理会，不建议使用这个定义值。作为一个特定的应用，应该按照自己的需要去申请空间的大小，然后根据控件支持的最大长度，做一个比较判断，而不是直接从控件获取支持的最大程度去申请动态控件，这样做是很危险，也很没有道理的。大概在下一次的重构中，我们会尝试取消这个定义。
*/
#define GUIRICHTEXT_TEXT_MAX_LEN (256)

/*! @def GUIRICHTEXT_PHONENUM_MAX_LEN
@brief phone number的最大长度
*/
#define GUIRICHTEXT_PHONENUM_MAX_LEN (50)

/*! @def GUIRICHTEXT_EMAIL_MAX_LEN
@brief email的最大长度
*/
#define GUIRICHTEXT_EMAIL_MAX_LEN (50)

/*! @def GUIRICHTEXT_URL_MAX_LEN
@brief url的最大长度
*/
#define GUIRICHTEXT_URL_MAX_LEN GUIRICHTEXT_TEXT_MAX_LEN

/*! @def GUIRICHTEXT_SETTINGS_SPACE_LIMIT
@brief 可设置的最大间隔，超过这个间隔的将会被自动设置成这个间隔 
@details 作为一个显示软件，各显示元素间的间隔是一个很重要的参数，因为这会直接影响到用户的视觉体验。我们提供了丰富的“间隔”给用户自定义，但是在显示“常识”上看，我们这些间隔，不应该不加限制的“大”，所以，我们设定一个上限制，来保证我们的显示总是在一个正常的状态。
*/
#define GUIRICHTEXT_SETTINGS_SPACE_LIMIT (12)

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
    MMI_IMAGE_ID_T bg_image_id; /*!< 控件的背景图片 */
} GUIRICHTEXT_INIT_DATA_T;


//1 图片和文字按数据来源类型枚举
/*! @enum _GUIRICHTEXT_IMAGE_TYPE_E
@brief 图片格式，按来源定义
@sa GUIRICHTEXT_IMAGE_TYPE_E
*/
enum _GUIRICHTEXT_IMAGE_TYPE_E
{
    GUIRICHTEXT_IMAGE_NONE, /*!< 没有图片 */
    GUIRICHTEXT_IMAGE_RES, /*!< 资源图片 */
    GUIRICHTEXT_IMAGE_BUF, /*!< 内存图片 */
    GUIRICHTEXT_IMAGE_FILE, /*!< 图片文件 */

    GUIRICHTEXT_IMAGE_INVALID,
};

/*! @typedef GUIRICHTEXT_IMAGE_TYPE_E
@brief 图片类型
@sa _GUIRICHTEXT_IMAGE_TYPE_E
*/
typedef uint8 GUIRICHTEXT_IMAGE_TYPE_E;

//1.2 文字
/*! @enum _GUIRICHTEXT_TEXT_TYPE_E
@brief 文字格式，按来源定义
@sa GUIRICHTEXT_TEXT_TYPE_E
*/
enum _GUIRICHTEXT_TEXT_TYPE_E
{
    GUIRICHTEXT_TEXT_NONE, /*!< 没有文字 */
    GUIRICHTEXT_TEXT_RES, /*!< 资源文字 */
    GUIRICHTEXT_TEXT_BUF, /*!< 内存字符串 */

    GUIRICHTEXT_TEXT_INVALID,
};

/*! @typedef GUIRICHTEXT_TEXT_TYPE_E
@brief 文字类型
@sa _GUIRICHTEXT_TEXT_TYPE_E
*/
typedef uint8 GUIRICHTEXT_TEXT_TYPE_E;

//2 FRAME type, 显示边框，可以扩展成各种风格边框
/*! @enum _GUIRICHTEXT_FRAME_TYPE_E
@brief 显示边框类型
@note 这里本来是打算设置多种格式的边框的，但是没有应用需求。
@sa GUIRICHTEXT_FRAME_TYPE_E
*/
enum _GUIRICHTEXT_FRAME_TYPE_E
{
    GUIRICHTEXT_FRAME_NONE, /*!< 没有边框 */
    GUIRICHTEXT_FRAME_NORMAL, /*!< 普通边框 */

    GUIRICHTEXT_FRAME_MAX,
};

/*! @typedef GUIRICHTEXT_FRAME_TYPE_E
@brief 边框类型
@sa _GUIRICHTEXT_FRAME_TYPE_E
*/
typedef uint8 GUIRICHTEXT_FRAME_TYPE_E;


//3 align
/*! @enum _GUIRICHTEXT_ALIGN_E
@brief 文字相对图片的位置
@note 以文字的位置为准
@sa GUIRICHTEXT_ALIGN_E
@sa _GUIRICHTEXT_IMG_ALIGN_T
*/
enum _GUIRICHTEXT_ALIGN_E
{
    GUIRICHTEXT_ALIGN_DEFAULT, /*!< 默认位置，根据系统的对齐方式 */
    GUIRICHTEXT_ALIGN_REVERSE,/*!< 和系统的对齐方式相反的 */
    GUIRICHTEXT_ALIGN_RIGHT, /*!< 文字在右 */
    GUIRICHTEXT_ALIGN_DOWN, /*!< 文字在下 */
    GUIRICHTEXT_ALIGN_LEFT, /*!< 文字在左 */
    GUIRICHTEXT_ALIGN_UP, /*!< 文字在上 */

    GUIRICHTEXT_ALIGN_INVALID,
};

/*! @typedef GUIRICHTEXT_ALIGN_E
@brief 图片文字相对排列类型
@sa _GUIRICHTEXT_ALIGN_E
*/
typedef uint8 GUIRICHTEXT_ALIGN_E;

/*! @enum _GUIRICHTEXT_IMG_ALIGN_T
@brief 图片的显示位置
@sa GUIRICHTEXT_IMG_ALIGN_T
@sa _GUIRICHTEXT_ALIGN_E
@details 因为我们总是按照图片和文字的高度来确定item的高度的，所以在纵向图片是没有自由的
*/
enum _GUIRICHTEXT_IMG_ALIGN_T
{
    GUIRICHTEXT_IMG_ALIGN_DEFAULT, /*!< 默认居中显示 */
    GUIRICHTEXT_IMG_ALIGN_AUTO, /*!< 根据系统的对齐方式对齐 */
    GUIRICHTEXT_IMG_ALIGN_REVERSE, /*!< 根据系统的对齐方式相反 */
    GUIRICHTEXT_IMG_ALIGN_MIDDLE, /*!< 居中 */
    GUIRICHTEXT_IMG_ALIGN_LEFT, /*!< 靠左 */
    GUIRICHTEXT_IMG_ALIGN_RIGHT, /*!< 靠右 */
};

/*! @typedef GUIRICHTEXT_IMG_ALIGN_T
@brief 图片显示对齐类型
@sa _GUIRICHTEXT_IMG_ALIGN_T
*/
typedef uint8 GUIRICHTEXT_IMG_ALIGN_T;


//4 焦点
/*! @typedef GUIRICHTEXT_TAG_TYPE_E
@brief 焦点元素的设定定义
@warning GUIRICHTEXT_TAG_HYPER是一个简单的超文本设置，没有经过实际应用。
@sa GUIRICHTEXT_FOCUS_TYPE_T
@details 前16bits是text的tag，后16bits是img的tag\n
关于焦点类型的特别说明:
-# 图片焦点是简单明了的，即有和无
-# 文字焦点的概念分为解析类型和结果类型
    - 解析类型，即设置焦点信息的时候指定的解析方式
    - 结果类型，即最终获取焦点信息时得到的焦点最终类型
-# 文字焦点概念再分为基本类型和扩展类型
    - 基本类型，即作为解析类型和结果类型是一致的
    - 扩展类型，即解析类型对应某一基本类型
    即，只有基本类型可以作为结果类型
-# 基本类型
    - 此时的焦点类型是制定的，不做任何解析，即按照编程者的意图设定
    - GUIRICHTEXT_TAG_NONE - 无焦点
    - GUIRICHTEXT_TAG_NORMAL - 整体焦点
    - GUIRICHTEXT_TAG_IMG_NORMAL - 图片焦点
    - GUIRICHTEXT_TAG_TXT_NORMAL - 整体文字item焦点，注意与txt all的区别，这个是item
    - GUIRICHTEXT_TAG_TXT_ALL - 整体文字焦点，，注意与txt all的区别，这个是text
    - GUIRICHTEXT_TAG_PHONENUM - 文字作为电话号码
    - GUIRICHTEXT_TAG_EMAIL - 文字作为email
    - GUIRICHTEXT_TAG_URL - 文字作为url
-# 扩展类型
    - parse类，按字符特征解析
    - hyper类，按超文本信息解析
    - GUIRICHTEXT_TAG_PARSE_PHONENUM - GUIRICHTEXT_TAG_PHONENUM
    - GUIRICHTEXT_TAG_PARSE_EMAIL - GUIRICHTEXT_TAG_EMAIL
    - GUIRICHTEXT_TAG_PARSE_URL - GUIRICHTEXT_TAG_URL
    - GUIRICHTEXT_TAG_HYPER - GUIRICHTEXT_TAG_TXT_ALL、三类型
-# 组合类型，仅仅是为了方便编程者的设置
    - GUIRICHTEXT_TAG_PARSE - parse类的组合
*/
typedef enum
{
    GUIRICHTEXT_TAG_NONE       = 0x00000000, /*!< 不设置焦点 */
    GUIRICHTEXT_TAG_NORMAL     = 0xFFFFFFFF, /*!< 整体得到焦点，焦点信息无 */

    //img
    GUIRICHTEXT_TAG_IMG_MASK   = 0xFFFF0000, /*!< 图片焦点模式的mask */
    GUIRICHTEXT_TAG_IMG_NORMAL = 0x00010000, /*!< IMG整体得到焦点，焦点信息无 */

    //text
    GUIRICHTEXT_TAG_TXT_MASK   = 0x0000FFFF, /*!< 文字焦点模式的mask */

    GUIRICHTEXT_TAG_TXT_NORMAL = 0x00000001, /*!< TEXT整体得到焦点，焦点信息为全部text */
    GUIRICHTEXT_TAG_TXT_ALL    = 0x00000002, /*!< TEXT全部字符得到焦点，焦点信息为全部text */
    GUIRICHTEXT_TAG_PHONENUM   = 0x00000004, /*!< TEXT全部字符得到焦点，且被当作ph，焦点信息为全部text */
    GUIRICHTEXT_TAG_EMAIL      = 0x00000008, /*!< TEXT全部字符得到焦点，且被当作email，焦点信息为全部text */
    GUIRICHTEXT_TAG_URL        = 0x00000010, /*!< TEXT全部字符得到焦点，且被当作url，焦点信息为全部text */

    GUIRICHTEXT_TAG_PARSE_PHONENUM = 0x00000020, /*!< 得到GUIRICHTEXT_TAG_PHONENUM */
    GUIRICHTEXT_TAG_PARSE_EMAIL    = 0x00000040, /*!< 得到GUIRICHTEXT_TAG_EMAIL */
    GUIRICHTEXT_TAG_PARSE_URL      = 0x00000080, /*!< 得到GUIRICHTEXT_TAG_URL */
    GUIRICHTEXT_TAG_HYPER      = 0x00000100, /*!< 以上三种，皆有可能 */

    GUIRICHTEXT_TAG_PARSE      = (GUIRICHTEXT_TAG_PARSE_PHONENUM | GUIRICHTEXT_TAG_PARSE_EMAIL | GUIRICHTEXT_TAG_PARSE_URL), /*!< 以上三种，皆有可能 */
} GUIRICHTEXT_TAG_TYPE_E;

/*! @enum _GUIRICHTEXT_FOCUS_SHIFT_E
@brief 焦点切换模式
@sa GUIRICHTEXT_FOCUS_SHIFT_E
*/
enum _GUIRICHTEXT_FOCUS_SHIFT_E
{
    GUIRICHTEXT_FOCUS_SHIFT_NONE, /*!< 不解析焦点 */
    GUIRICHTEXT_FOCUS_SHIFT_BASIC, /*!< 解析并切换焦点，可以切换至无焦点元素 */
    GUIRICHTEXT_FOCUS_SHIFT_ADVANCED, /*!< 解析并切换焦点，始终在焦点之间切换 */

    GUIRICHTEXT_FOCUS_SHIFT_MAX,
};

/*! @typedef GUIRICHTEXT_FOCUS_SHIFT_E
@brief 焦点切换模式类型
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
*/
typedef uint8 GUIRICHTEXT_FOCUS_SHIFT_E;

/*! @struct GUIRICHTEXT_DATA_IMAGE_RES_T
@brief 图片资源文件的数据结构
@sa GUIRICHTEXT_DATA_IMAGE_BUF_T
@sa GUIRICHTEXT_DATA_IMAGE_FILE_T
*/
typedef struct
{
    MMI_IMAGE_ID_T id; /*!< 资源图片的image id */
} GUIRICHTEXT_DATA_IMAGE_RES_T;

/*! @struct GUIRICHTEXT_DATA_IMAGE_BUF_T
@brief 内存图片的数据结构
@note bmp buffer形式的数据
@sa GUIRICHTEXT_DATA_IMAGE_RES_T
@sa GUIRICHTEXT_DATA_IMAGE_FILE_T
*/
typedef struct
{
    uint8 *buf_ptr; /*!< 图片内存数据指针 */
    uint32 size; /*!< 图片的大小 */
    uint16 width; /*!< 图片的宽，必须指定图片的宽高，才可以解析图片数据 */
    uint16 height; /*!< 图片的高 */
} GUIRICHTEXT_DATA_IMAGE_BUF_T;

/*! @struct GUIRICHTEXT_DATA_IMAGE_FILE_T
@brief 图片文件，即文件系统存储的文件
@sa GUIRICHTEXT_DATA_IMAGE_RES_T
@sa GUIRICHTEXT_DATA_IMAGE_BUF_T
*/
typedef struct
{
    wchar *filename_ptr; /*!< 图片文件名字符串指针 */
    uint16 filename_len; /*!< 图片文件名长度 */
} GUIRICHTEXT_DATA_IMAGE_FILE_T;

/*! @union GUIRICHTEXT_DATA_IMAGE_U
@brief 图片数据的联合结构
*/
typedef union
{
    GUIRICHTEXT_DATA_IMAGE_RES_T res; /*!< 资源图片 */
    GUIRICHTEXT_DATA_IMAGE_BUF_T buf; /*!< 内存图片 */
    GUIRICHTEXT_DATA_IMAGE_FILE_T file; /*!< 文件图片 */
} GUIRICHTEXT_DATA_IMAGE_U;


/*! @struct GUIRICHTEXT_DATA_TEXT_RES_T
@brief 资源字符串的数据结构
@sa GUIRICHTEXT_DATA_TEXT_BUF_T
*/
typedef struct
{
    MMI_TEXT_ID_T id; /*!< 显示资源文字的text id */
} GUIRICHTEXT_DATA_TEXT_RES_T;

/*! @struct GUIRICHTEXT_DATA_TEXT_BUF_T
@brief 字符串数据结构
@sa GUIRICHTEXT_DATA_TEXT_RES_T
*/
typedef struct
{
    wchar *str_ptr; /*!< 显示的字符串指针 */
    uint16 len; /*!< 显示的字符串长度 */
} GUIRICHTEXT_DATA_TEXT_BUF_T;

/*! @union GUIRICHTEXT_DATA_TEXT_U
@brief 文字数据的联合结构
*/
typedef union
{
    GUIRICHTEXT_DATA_TEXT_RES_T res; /*!< 资源文字 */
    GUIRICHTEXT_DATA_TEXT_BUF_T buf; /*!< 内存中的字符串 */
} GUIRICHTEXT_DATA_TEXT_U;


/*! @struct GUIRICHTEXT_ITEM_T
@brief item的结构定义
@note 文字和图片的属性设定，要同时设置set和setting，即明确表示设置以及给出设定的值，在未显式设定的情况下，无视设定值
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
@details 应用在增加一个item的时候，可以设置的item属性
- 图片属性
    -# 图片的类型
    -# 图片的数据
    -# 图片的边框
    -# 图片的可设置属性
        -# 宽、高
        -# 显示的时候在显示区域内的对齐方式
- 文字属性
    -# 文本的类型
    -# 文本的数据
    -# 文本的边框
    -# 文本的可设置属性
        -# 字体的大小、颜色
        -# 文字在显示区域内的对齐方式
        -# 文字的背景色
- item属性
    -# tag_type 焦点属性
    -# align_type 图片和文字的相对位置属性
    -# frame_type item的边框
*/
typedef struct
{
    GUIRICHTEXT_IMAGE_TYPE_E img_type; /*!< 图片类型 */
    GUIRICHTEXT_DATA_IMAGE_U img_data; /*!< 图片数据 */
    GUIRICHTEXT_FRAME_TYPE_E img_frame_type; /*!< 图片边框 */
    BOOLEAN is_bit_map;/*!< 是否Bitmap */
    BOOLEAN img_set_size; /*!< 是否指定图片大小 */
    uint16 img_setting_width; /*!< 指定图片宽 */
    uint16 img_setting_height; /*!< 指定图片高 */
    BOOLEAN img_set_align; /*!< 是否制定图片对齐方式 */
    GUIRICHTEXT_IMG_ALIGN_T img_setting_align; /*!< 指定图片对齐方式 */

    GUIRICHTEXT_TEXT_TYPE_E text_type; /*!< 文字类型 */
    GUIRICHTEXT_DATA_TEXT_U text_data; /*!< 文字数据 */
    GUIRICHTEXT_FRAME_TYPE_E text_frame_type; /*!< 文字边框 */
    BOOLEAN text_set_font; /*!< 是否指定字体 */
    GUI_FONT_T setting_font; /*!< 指定文字字体 */
    BOOLEAN text_set_font_color; /*!< 是否设定文字颜色 */
    GUI_COLOR_T setting_font_color; /*!< 指定的文字颜色 */
    BOOLEAN text_set_align; /*!< 是否指定文字对齐方式 */
    GUI_ALIGN_E setting_align; /*!< 指定的文字对齐方式 */
    BOOLEAN text_set_bg; /*!< 是否设定文字背景色 */
    GUI_COLOR_T setting_bg_color; /*!< 指定的文字背景色 */

    GUIRICHTEXT_TAG_TYPE_E tag_type; /*!< 焦点方式设定 */
    GUIRICHTEXT_ALIGN_E align_type; /*!< 文字和图片的相对位置设定 */
    GUIRICHTEXT_FRAME_TYPE_E frame_type; /*!< 整个项目的边框设定 */
} GUIRICHTEXT_ITEM_T;


//8 元素-从焦点取出的内容
/*! @struct GUIRICHTEXT_ELEMENT_DATA_T
@brief 焦点元素数据
*/
typedef struct
{
    wchar *str_ptr; /*!< 焦点字符的指针 */
    uint16 len; /*!< 焦点字符的长度 */
} GUIRICHTEXT_ELEMENT_DATA_T;

/*! @typedef GUIRICHTEXT_FOCUS_TYPE_T
@brief 焦点元素的类型
@note 作为GUIRICHTEXT_FOCUS_URL时，需要应用自行判定协议头，即是http还是rtsp等。
@sa GUIRICHTEXT_TAG_TYPE_E
@details 为了使得解析的过程和焦点识别的结果清晰的区分，我们定义焦点类型
一个焦点类型可以对应一个或多个焦点识别类型\n
事实上，在空间内部，是使用的识别过程类型，只是对外，使用焦点类型
*/
typedef enum
{
    GUIRICHTEXT_FOCUS_NONE = GUIRICHTEXT_TAG_NONE, /*!< 非焦点 */
    GUIRICHTEXT_FOCUS_NORMAL = GUIRICHTEXT_TAG_NORMAL, /*!< 整个item */
    GUIRICHTEXT_FOCUS_IMG_NORMAL = GUIRICHTEXT_TAG_NORMAL, /*!< 整个img部分 */
    GUIRICHTEXT_FOCUS_TXT_NORMAL = GUIRICHTEXT_TAG_TXT_NORMAL, /*!< 整个text部分，全体 */
    GUIRICHTEXT_FOCUS_TXT_ALL = GUIRICHTEXT_TAG_TXT_ALL, /*!< 整个text部分，仅字符 */
    GUIRICHTEXT_FOCUS_PHONENUM = GUIRICHTEXT_TAG_PHONENUM, /*!< 电话号码 */
    GUIRICHTEXT_FOCUS_EMAIL = GUIRICHTEXT_TAG_EMAIL, /*!< email */
    GUIRICHTEXT_FOCUS_URL = GUIRICHTEXT_TAG_URL, /*!< url */
} GUIRICHTEXT_FOCUS_TYPE_T;

/*! @struct GUIRICHTEXT_FOCUS_ELEM_T
@brief 元素数据类型
*/
typedef struct
{
    GUIRICHTEXT_FOCUS_TYPE_T type; /*!< 焦点类型 */
    GUIRICHTEXT_ELEMENT_DATA_T data; /*!< 焦点元素的数据 */
} GUIRICHTEXT_FOCUS_ELEM_T;
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/***************************************************************************//*!
@brief GUIRICHTEXT_AddItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_ptr [in] item数据
@param index_ptr [in/out]返回当前item的index，可根据该id操作item
@return 是否增加成功 
@note 当增加一个图片item, 并且图片是buf数据时, GUIRICHTEXT_IMAGE_T结构中的buf_ptr是由窗口申请内存的, 控件不负责申请和释放
@note 焦点类型的设置，请参看GUIRICHTEXT_TAG_TYPE_E定义处的详细说明
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_AddItem(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_ITEM_T const *item_ptr,
    uint16 *index_ptr
);

/***************************************************************************//*!
@brief GUIRICHTEXT_DeleteItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 操作是否成功
@note delete之后，如果需要刷新显示，请调用GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_DeleteItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
);

/***************************************************************************//*!
@brief GUIRICHTEXT_ModifyItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_index [in] item的index，在add item的时候可以得到
@param item_ptr [in] item数据
@return 操作是否成功
@note modify之后，如果需要刷新显示，请调用GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_ModifyItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index,
    GUIRICHTEXT_ITEM_T const *item_ptr
);

/***************************************************************************//*!
@brief 删除所有的item
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_DeleteAllItems(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief 显示控件
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@note 在刷新之前，会更新控件的数据
@sa GUIRICHTEXT_UpdateData
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_Update(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief 仅更新控件的数据，而不刷新
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@note 有很多数据是通过计算得到的，而计算只是在需要的时候才进行，通常是在显示的时
候。如果需要即时计算控件的数据，则需要通过更新数据来完成。
@sa GUIRICHTEXT_Update
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_UpdateData(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief 获取指定index的item
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@param item_ptr [in]item数据指针
@return 操作是否成功
@note 该item的内容并非原始内容，可能会因为显示需要而修改过。比如hyper parse给出
显示的字符，格式丢了，同样text id格式的文字也被构造成了buffer形式的
@sa GUIRICHTEXT_AddItem
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index,
    GUIRICHTEXT_ITEM_T *item_ptr
);

/***************************************************************************//*!
@brief 获取焦点元素
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param element [in]焦点数据的指针
@return 操作是否成功
@note 
- 即当前焦点所在元素的信息
- 焦点类型，请参看GUIRICHTEXT_TAG_TYPE_E定义处的详细说明
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusElement(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_ELEM_T *element
);

/***************************************************************************//*!
@brief 获取焦点Item的索引号
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param index_ptr [in]索引号指针
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 *index_ptr
);

/***************************************************************************//*!
@brief 设置整个控件的默认字体
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param font [in]字体
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFont(
    MMI_CTRL_ID_T ctrl_id,
    GUI_FONT_T font
);

/***************************************************************************//*!
@brief 设置整个控件的默认字体颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFontColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置显示区域
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param rect [in]显示区域
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetRect(
    MMI_CTRL_ID_T ctrl_id,
    GUI_RECT_T  rect
);

/***************************************************************************//*!
@brief 设置字间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]字间距
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetCharSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置文本的行间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]行间距
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetLineSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置图片和文本的距离
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]文本和图片之间的距离
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgTxtSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置item之间的间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item之间的间距
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置边界距，即item和控件边缘的距离
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item和控件边缘的距离
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置边界距，item四周的留白
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item四周的留白
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief 设置文字到边框的空白
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param margin [in]设置文字到边框的空白
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetTxtMargin(
    MMI_CTRL_ID_T ctrl_id,
    uint8 margin
);

/***************************************************************************//*!
@brief 设置切换焦点的方式
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param focus_mode [in]焦点模式
@return 操作是否成功
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusMode(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_SHIFT_E focus_mode
);

/***************************************************************************//*!
@brief 设置焦点字符的下划线颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in] 颜色
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusLineColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置焦点区域选择时的背景色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置控件的背景色
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param color [in] 颜色
@return 操作是否成功
@note 当没有背景图片或者背景图片失效的时候，背景色才启动
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置控件的背景图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param bg_img_id [in]背景图片id
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgImage(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T bg_img_id
);

/***************************************************************************//*!
@brief 设置边框,默认为GUIRICHTEXT_FRAME_NONE
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param border_type [in]边框类型
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorder(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FRAME_TYPE_E border_type
);

/***************************************************************************//*!
@brief 设置边框的颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置焦点元素的边框颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
@note 焦点item在显示边框的时候，会有不同的颜色
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderFocusColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief 设置在图片解析过程中显示的图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param img_id [in]图片的id
@return 操作是否成功
@note 有些图片的显示会要等待，在等待过程中，会显示一个等待图片
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimParsingIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
);

/***************************************************************************//*!
@brief 设置在图片解析错误时显示的图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param img_id [in]图片的id
@return 操作是否成功
@note 在图片不能显示的时候，显示指定图片
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimErrorIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
);

/***************************************************************************//*!
@brief 指定焦点item
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 操作是否成功
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
);

/***************************************************************************//*!
@brief 设定图片解码是否同步
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param is_img_syn_decode [in]是否同步解码
@return 操作是否成功
@note 在一些对显示速度有要求的应用，会采用异步解码。
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgDecodeSyn(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_img_syn_decode
);

/***************************************************************************//*!
@brief 重新初始化控件的显示参数
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@note 用于在重新设置控件条目后不需要保持状态的情况
*******************************************************************************/
PUBLIC void CTRLRICHTEXT_ResetState(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief 设置显示到焦点元素所在页面
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
*******************************************************************************/
PUBLIC void CTRLRICHTEXT_ScrollToFocus(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief 设置显示指定item所在页面
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 是否滚动成功
@note 新加入的item必须在update之后才能正确的滚动显示。因为在update的时候计算rect
update在其后的第一个刷新会自动做，也可以调用函数GUIRICHTEXT_Update来完成
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_ScrollToItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
);

/***************************************************************************//*!
@brief 启用剪贴板功能
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否设置成功
@sa GUIRICHTEXT_DisableClipBoard
@note 默认剪切板是开启的
-# 选字，选字有下面几种方式：
    -# 通过触笔拖动选择
    -# 长按触笔，弹出菜单全选
-# 复制，可复制的文字有下面几种：
    -# 选定的文字
    -# 焦点文字
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_EnableClipBoard(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 禁用剪贴板功能
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否设置成功
@sa GUIRICHTEXT_EnableClipBoard
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_DisableClipBoard(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 保存控件的显示中间信息，包括剪切板、滚动位置等
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否保存成功
@sa GUIRICHTEXT_RestoreInfo
@warning 一定要调用GUIRICHTEXT_RestoreInfo来恢复，否则引起显示混乱
@warning 焦点信息等无法保存，因为焦点信息要依赖项(item)信息，而item的信息会全部
丢失，item的信息又比较复杂，甚至不能保证在操作之后，原来的item还存在，故而，无法
保存。
@details 一些应用会因为简化应用的需要，对控件进行摧毁重建，而在重建的过程中，会
导致一些信息的丢失，比如剪切板信息、滚动位置信息等。这时候，我们就可以采用对其中
的一些信息进行保护。
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_SaveInfo(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief 恢复控件的显示中间信息，包括剪切板、滚动位置等
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否恢复成功
@sa GUIRICHTEXT_SaveInfo
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_RestoreInfo(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_GetHighlightPos(
    MMI_CTRL_ID_T    ctrl_id,
    GUI_RECT_T       *min_highlight_rect_ptr,
    GUI_RECT_T       *max_highlight_rect_ptr
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
