/*! @file guiurl.h
@brief 按字符串特征解析字符串
@author Lianxiang.Zhou
@version 1.0
@date 2009/08/08
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 一般来讲，我们会需要判定一个字符串中是否包含了特定的字符串，比如
- url
- email
- phone number
这部分本来是内嵌于richtext的，但是考虑到并非richtext所特有的，便将其独立出来。

@note 2009/09/01-Lianxiang.Zhou-Create.
@warning 此处，我们
- 并不会一下子支持所有的协议的解析，会不断的根据需要加入
- 如果是有明确协议的，则按照协议的规范去解析
- 没有明确协议的，即省略了协议头部分的，按照http协议解析
@warning 本模块所有字符串，需要根据字符串的结尾字符'\0'来判定字符串的结束
@sa urlGrp
@sa @ref page_Url
*******************************************************************************/

/*! @page page_Url 字符串特征识别(URL)

-# @ref sec_url_intro
-# @ref sec_url_include
-# @ref sec_url_how_to_use

@section sec_url_intro 文字解析的概念

- 字符串解析，不仅仅是URL解析，还包括EMAIL和Phone Number的解析。
- 解析的结果没有正确或者不正确的说法，只有准确不准确的说法。不准确未必就是错的。
- 仅仅根据字符串特征来解析，是在解析的精度和解析的难度之间做一个平衡。
- 这里的解析结果，只是在一定情况下，是合理的。

@section sec_url_include 解析的范围

- 我们会提供一些类型的字符串的识别，包括：
    -# 电子邮件地址
    -# 网址
    -# 电话号码
- 对于电子邮件地址、网址的识别规则，是按照RFC1738为基准，在此基础上根据手机的实际需求做了一些小的改动。
- 对于电话号码的识别，则是按照一般的常用规则来识别的。
- 识别详情，请参照相关的函数说明。

@section sec_url_how_to_use 如何使用解析功能？

直接调用本模块提供的解析函数，函数列表及函数说明参见模块 @ref urlGrp 。

@sa urlGrp
*******************************************************************************/

/*! @addtogroup urlGrp Url
@brief 字符串特征解析
@details 纯字符特征的解析，即无视字符的具体内容
- 该模块是从Richtext模块中独立出来的
- 该模块仅包含guiurl.c、guiurl.h两个文件

@sa richtextGrp
@sa @ref page_Url
@{
*******************************************************************************/


#ifndef _URL_H_
#define _URL_H_

/*------------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"


/*------------------------------------------------------------------------------
    Macro Declaration
------------------------------------------------------------------------------*/
/*! @def GUIURL_PHONE_NUMBER_LEN_MIN
@brief 电话号码的最短长度
*/
#define GUIURL_PHONE_NUMBER_LEN_MIN (5)

/*! @def GUIURL_PHONE_NUMBER_LEN_MAX
@brief 电话号码的最大长度
*/
#define GUIURL_PHONE_NUMBER_LEN_MAX (40)

/*------------------------------------------------------------------------------
    TYPE AND STRUCT
------------------------------------------------------------------------------*/
/*! @typedef URL_CHAR_T
@brief 定义url识别的字符类型，目前我们的系统默认的是宽字符
*/
typedef wchar URL_CHAR_T;


/*! @enum URL_PARSE_TYPE_T
@brief URL识别类型，可组合
*/
typedef enum
{
    URL_PARSE_URL = 1, /*!< 识别url */
    URL_PARSE_EMAIL = 1 << 1, /*!< 识别email */
    URL_PARSE_PHONENUM = 1 << 2, /*!< 识别电话号码 */

    URL_PARSE_ALL = (URL_PARSE_URL | URL_PARSE_EMAIL | URL_PARSE_PHONENUM)
} URL_PARSE_TYPE_T;


/*! @struct URL_DATA_T
@brief 解析得出的url数据
*/
typedef struct url_data_tag  URL_DATA_T;
struct url_data_tag
{
    URL_DATA_T *prev;
    URL_DATA_T *next;
    URL_PARSE_TYPE_T type;
    uint32 off_set;
    uint32 len;
};


/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*------------------------------------------------------------------------------
    FUNCTION DEFINITION
------------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 判定一个字符串是不是满足http格式的url定义
@details 是 @ref URL_IsNormalFormat 的特定版本，加强了协议头的判断，即限定协议头为(https?)。大致的regEx是：
@code
(https?://)?hostport[/hpath[?search]]
@endcode
详细部分参考 @ref URL_IsNormalFormat 中的定义。

@author Lianxiang.Zhou
@param str_ptr [in] 字符串指针
@param confirm_len_ptr [int/out] 满足http格式的字符的长度
@return 从当前字符的开头是否获得一个http格式的url
@note 包括http和https
@sa URL_IsNormalFormat
*******************************************************************************/
PUBLIC BOOLEAN URL_IsHttp(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief 判定一个字符串是否URL格式，并且协议头在预设范围内
@details 大致的regEx是：
@code
(head://)?hostport[/hpath[?search]]
head - (https?|rtsp)
hostport - host(:port)?
	host - (hostname|hostnumber)
		hostname - ([\w\d][-\w\d]+[\w\d].)+[\w\d][-\w\d]+[\w\d]
		hostnumber- (\d+.){3,3}\d+
	port - \d+
hpath - hsegment(/hsegment)*
	hsegment - [[:uchar:];:@&=]+
		[:uchar:] - 字符集，([-_$.+!*\()a-zA-z0-9]|%[0-9a-fA-F]{2})
search - 同hsegment
@endcode
当然，我们会在RFC1738协议的基础上增加一些限制，比如：
- 对于协议头的限定。合法的协议头是很多的，而我们针对目前系统需要识别的协议头，做了一个限制。
- 对于hostname的最后一个域，我们按照经验值，限定为2-3个字母。

@author Lianxiang.Zhou
@param str_ptr [in] 字符串指针
@param confirm_len_ptr [in/out] 满足url格式的字符的长度
@return 从当前字符的开头是否获得一个url
@sa URL_IsHttp
*******************************************************************************/
PUBLIC BOOLEAN URL_IsNormalFormat(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief 判定一个字符串是不是满足http格式的url定义，但是省略了协议部分的
@details 是 @ref URL_IsHttp 的简化版本，即默认协议头为(https?)。大致的regEx是：
@code
hostport[/hpath[?search]]
@endcode
详细部分参考 @ref URL_IsNormalFormat 中的定义。

@author Lianxiang.Zhou
@param str_ptr [in] 字符的指针
@param confirm_len_ptr [in] 字符的长度
@return 从当前字符的开头是否获得一个简化的url
@sa URL_IsHttp
@sa URL_IsNormalFormat
*******************************************************************************/
PUBLIC BOOLEAN URL_IsHttpDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief 判定一个字符串是不是满足mailto格式的url定义
@details 按照RFC1738协议为基准的一个判定，大致的regEx是：
@code
(mailto:)?user@host
user - [[:alpha:][:digital:][:safe:]]+
	[:alpha:] - [a-zA-Z]
	[:digital:] - [0-9]
	[:safe:] - [-_$.+]
host - 同url的host
@endcode
- host部分，参考 @ref URL_IsNormalFormat 中的定义。
- 这里，我们对user的字符集也做了一些修改，将其中一些“非常用”的字符集去掉，以强化判断

@author Lianxiang.Zhou
@param str_ptr [in] 字符的指针
@param confirm_len_ptr [in] 字符的长度
@return 从当前字符的开头是否获得一个email字符串
*******************************************************************************/
PUBLIC BOOLEAN URL_IsMailto(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief 判定一个字符串是不是满足mailto格式的url定义，但是省略了协议部分的
@details 这是 @ref URL_IsMailto 的简化版本，省略了协议头“mailto:”部分。这部分在通常的判定中，都是不需要的。大致的regEx是：
@code
user@host
@endcode
详细定义，参考  @ref URL_IsMailto 中的定义

@author Lianxiang.Zhou
@param str_ptr [in] 字符的指针
@param confirm_len_ptr [in] 字符的长度
@return 从当前字符的开头是否获得一个简化的email字符串
*******************************************************************************/
PUBLIC BOOLEAN URL_IsMailtoDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief 判定一个字符串是不是满足电话号码的正则表达式
@details 电话号码的判别，不是url的一部分，但属于字符串特征判别的一部分。大致的regEx是：
@code
(([+0][*#\d]{6,})|([*#\d]{5,}))(([Pp]+|[Ww])[*#\d]+))*
@endcode
- 增加连字符“-”规则：
    -# 在首位不识别
    -# 连续的连字符不识别
    -# 连字符不算入位数
- 增加位数限制40：在超过最大识别字符后，不识别。
- 规则简介：
    -# 号码部分@code (([+0][*#\d]{6,})|([*#\d]{5,})) @endcode
        -# @code [+0][*#\d]{6,} @endcode - 以+或者0开头的，后面至少6个连续的数字
        -# @code [*#1-9][*#\d]{4,} @endcode - 非零开头的数字串，至少5位
    -# 增加操作部分 @code (([Pp]+|[Ww])[*#\d]+))* @endcode
        -# 暂停（p或者P） - 可连续多个暂停，后继续输入字符
        -# 等待（w或者W） - 只能一个等待，后接一串字符
- 补充：此处对等待的后的数字没有严格要求是纯数字，是为了简化判断。

@author Lianxiang.Zhou
@param str_ptr [in] 字符的指针
@param confirm_len_ptr [in] 字符的长度
@return 从当前字符的开头是否获得一个电话号码字符串
@note 关于号码的判断，在不同的应用中，会有不同的要求。
-# 这个不是url的一部分，是为了程序处理的统一方便
-# 这个正则表达式比较难写...
*******************************************************************************/
PUBLIC BOOLEAN URL_IsPhoneNumber(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);


/***************************************************************************//*!
@brief 构造字符串的特征解析数据
@author Lianxiang.Zhou
@param str_ptr [in] 字符串
@param types [in] 需要解析的类型
@param data_pptr [out] 解析结果的存放指针
@return 返回解析的有特征的字符串的个数
@note 字符串必须由'\0'结尾
@note 解析结果的空间由解析函数动态申请
@warning data_pptr即结果存放控件是一个可写的指针的指针
@warning 必须调用 @ref URL_ParseDataRelease 释放数据的存储控件
@sa URL_ParseDataRelease
*******************************************************************************/
PUBLIC int32 URL_ParseDataGen(
    URL_CHAR_T const * str_ptr,
    URL_PARSE_TYPE_T types,
    URL_DATA_T** data_pptr
);

/***************************************************************************//*!
@brief 释放构造数据时动态申请的存储空间
@author Lianxiang.Zhou
@param data_ptr [in] 数据存储空间指针
@note 此处释放的数据是函数 @ref URL_ParseDataGen 中生成的数据
@sa URL_ParseDataGen
*******************************************************************************/
PUBLIC void URL_ParseDataRelease(URL_DATA_T *data_ptr);

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_URL_H_

/*@}*/
