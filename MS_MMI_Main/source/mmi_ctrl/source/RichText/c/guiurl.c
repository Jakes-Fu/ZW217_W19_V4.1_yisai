/*! @file guiurl.c
@brief 解析url, RFC1738
@author Lianxiang.Zhou
@version 1.0
@date 2011/03/25
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@note
- 2009/09/01   Lianxiang.Zhou  Create.
@details 解析字符串中的url字符串
- URL解析部分，以RFC1738为基础
- 结合实际，在细节部分做相应的增强和放宽
- 加入电话号码的解析，解析规则为经验做法
- 可以对一个字符串进行完全分析，返回字符串中所有的特殊字符串
- 在该模块，我们会为了减少依赖而自行完成很多的定义和判断
*******************************************************************************/

#define _GUIURL_C_

/*------------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include "os_api.h"
#include "guiurl.h"

/*------------------------------------------------------------------------------
    Typedef and constant
------------------------------------------------------------------------------*/
/*! @struct URL_HEAD_STRNG_T
@brief url协议头的存储，我们支持指定的协议头，或者识别指定的协议头
@note 其实就是通常的字符串结构
*/
typedef struct
{
    URL_CHAR_T const *ptr;
    size_t len;
}URL_HEAD_STRNG_T;

/*! @var s_str_http
@brief 字符串"http://"
@note URL_CHAR_T不确定的情况下，采用数组的形式，下同
*/
static URL_CHAR_T const s_str_http[] = {'h', 't', 't', 'p', ':', '/', '/', '\0'};
/*! @var s_str_https
@brief 字符串"https://"
*/
static URL_CHAR_T const s_str_https[] = {'h', 't', 't', 'p', 's',':', '/', '/', '\0'};
/*! @var s_str_rtsp
@brief 字符串"rtsp://"
*/
static URL_CHAR_T const s_str_rtsp[] = {'r', 't', 's', 'p', ':', '/', '/', '\0'};
/*! @var s_str_mailto
@brief 字符串"mailto:"
*/
static URL_CHAR_T const s_str_mailto[] = {'m', 'a', 'i', 'l', 't', 'o', ':', '\0'};

/*! @var s_http_heads_list
@brief 判定网址的协议头，我们认为http和https是合法的
*/
static URL_HEAD_STRNG_T const s_http_heads_list[] =
{
    {s_str_http, ARR_SIZE(s_str_http) - 1},
    {s_str_https, ARR_SIZE(s_str_https) - 1},
};

/*! @var s_url_heads_list
@brief 判定系统识别的协议头，目前仅识别部分
*/
static URL_HEAD_STRNG_T const s_url_heads_list[] =
{
    {s_str_http, ARR_SIZE(s_str_http) - 1},
    {s_str_https, ARR_SIZE(s_str_https) - 1},
    {s_str_rtsp, ARR_SIZE(s_str_rtsp) - 1},
};


/*------------------------------------------------------------------------------
    Inline Function(常用小函数) 
------------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 判定字符是否小写字母a-z
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsLowalpha(URL_CHAR_T c)
{
    return ('a' <= c && c <= 'z');
}

/***************************************************************************//*!
@brief 判定字符是否大写字母A-Z
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsHialpha(URL_CHAR_T c)
{
    return ('A' <= c && c <= 'Z');
}

/***************************************************************************//*!
@brief 判定字符是否字母a-zA-Z
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsAlpha(URL_CHAR_T c)
{
    return (IsLowalpha(c) || IsHialpha(c));
}

/***************************************************************************//*!
@brief 判定字符是否数字0-9
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsDigit(URL_CHAR_T c)
{
    return ('0' <= c && c <= '9');
}

/***************************************************************************//*!
@brief 判定字符是否"safe"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "safe char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsSafe(URL_CHAR_T c)
{
    return ('$' == c 
        || '-' == c
        || '_' == c
        || '.' == c
        || '+' == c);
}

/***************************************************************************//*!
@brief 判定字符是否"extra"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "extra char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsExtra(URL_CHAR_T c)
{
    return ('!' == c 
        || '*' == c
        || '\'' == c
        || '(' == c
        || ')' == c
        || ',' == c);    
}

/***************************************************************************//*!
@brief 判定字符是否"national"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "national char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsNational(URL_CHAR_T c)
{
    return ('0' == c 
        || '}' == c
        || '|' == c
        || '\\' == c
        || '^' == c
        || '~' == c
        || '[' == c
        || ']' == c
        || '`' == c);    
}

/***************************************************************************//*!
@brief 判定字符是否"punctuation"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "punctuation char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsPunctuation(URL_CHAR_T c)
{
    return ('<' == c 
        || '>' == c
        || '#' == c
        || '%' == c
        || '"' == c);    
}

/***************************************************************************//*!
@brief 判定字符是否"reserved"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "reserved char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsReserved(URL_CHAR_T c)
{
    return (';' == c 
        || '/' == c
        || '?' == c
        || ':' == c
        || '@' == c
        || '&' == c
        || '=' == c);    
}

/***************************************************************************//*!
@brief 判定字符是否"hex"字符, 即16进制数字字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsHex(URL_CHAR_T c)
{
    return (IsDigit(c)
        || ('a' <= c && c <= 'f')
        || ('A' <= c && c <= 'F'));    
}

/***************************************************************************//*!
@brief 判定字符是否"escape"字符
@author Lianxiang.Zhou
@param pc [in] 被判定的字符串指针
@return TURE or FALSE
@note "escape char"是协议里的一个概念，可以理解为一个字符集
@note 一个"escap char"是"%hh"的模式，由3个字符构成
*******************************************************************************/
LOCAL __inline BOOLEAN IsEscape(URL_CHAR_T const *pc)
{
    return ('%' == *pc && IsHex(*(pc+1)) && IsHex(*(pc+2)));
}

/***************************************************************************//*!
@brief 判定字符是否"unreserved"字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note "unreserved char"是协议里的一个概念，可以理解为一个字符集
*******************************************************************************/
LOCAL __inline BOOLEAN IsUnreserved(URL_CHAR_T c)
{
    return (IsAlpha(c) || IsDigit(c) || IsSafe(c) || IsExtra(c));
}

/***************************************************************************//*!
@brief 判定字符是否"uchar"字符
@author Lianxiang.Zhou
@param pc [in] 被判定的字符串指针
@param len_ptr [out] 识别字符(串)的长度
@return TURE or FALSE
@note "uchar char"是协议里的一个概念，可以理解为一个字符集
@note 可以是一个字符，也可以是三个字符(escape)
*******************************************************************************/
LOCAL __inline BOOLEAN IsUchar(URL_CHAR_T const *pc, int32 *len_ptr)
{
    if(IsUnreserved(*pc))
    {
        *len_ptr = 1;
    }
    else if(IsEscape(pc))
    {
        *len_ptr = 3;
    }
    else
    {
        *len_ptr = 0;
    }

    return (*len_ptr > 0);
}

/***************************************************************************//*!
@brief 判定字符是否"xchar"字符
@author Lianxiang.Zhou
@param pc [in] 被判定的字符串指针
@param len_ptr [out] 识别字符(串)的长度
@return TURE or FALSE
@note "xchar char"是协议里的一个概念，可以理解为一个字符集
@note 可以是一个字符，也可以是三个字符(escape)
*******************************************************************************/
LOCAL __inline BOOLEAN IsXchar(URL_CHAR_T const *pc, int32 *len_ptr)
{
    if(IsUnreserved(*pc) || IsReserved(*pc))
    {
        *len_ptr = 1;
    }
    else if(IsEscape(pc))
    {
        *len_ptr = 3;
    }
    else
    {
        *len_ptr = 0;
    }

    return (*len_ptr > 0);
}

/***************************************************************************//*!
@brief 判定字符是否构成电话号码的字符[*#0-9]
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note 因为暗码的存在以及操作码，我们简化判断，将"*#"当作简单的数字去识别
*******************************************************************************/
LOCAL __inline BOOLEAN IsPhoneNumberChar(URL_CHAR_T c)
{
    return (IsDigit(c)
        || '*' == c
        || '#' == c);
}

/***************************************************************************//*!
@brief 判定字符是否电话号码连接符字符"-"
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return TURE or FALSE
@note 电话号码的通常连接符就是"-", 不会有别的
*******************************************************************************/
LOCAL __inline BOOLEAN IsPhoneNumberHyphen(URL_CHAR_T c)
{
    return ('-' == c);
}

/***************************************************************************//*!
@brief 将字符转化成小写字符
@author Lianxiang.Zhou
@param c [in] 被判定的字符
@return 转化后的字符
@note 分几种情况:
    - 大小转小写
    - 其他原字符返回
*******************************************************************************/
LOCAL __inline URL_CHAR_T ToLower(URL_CHAR_T c)
{
    return (IsHialpha(c) ? (c + 'a' - 'A') : c);
}



/*------------------------------------------------------------------------------
    LOCAL FUNCTION DECLARE
------------------------------------------------------------------------------*/
LOCAL int32 CmpNoCase(URL_CHAR_T const *src_ptr, URL_CHAR_T const *dst_ptr, int32 len);
LOCAL BOOLEAN IsHostname(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsHostnumber(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsHost(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsPort(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsHostPort(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsHsegment(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsHpath(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsSearch(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsUser(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr);
LOCAL BOOLEAN IsUrlDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);
LOCAL BOOLEAN IsEmailDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);
LOCAL void Addnode(
    URL_DATA_T **head_pptr,
    URL_PARSE_TYPE_T type,
    uint32 off_set,
    uint32 len
);


/*------------------------------------------------------------------------------
    FUNCTION DEFINITION
------------------------------------------------------------------------------*/


/***************************************************************************//*!
@brief 忽略大小写，比较字符串
@author Lianxiang.Zhou
@param src_ptr  [in] 源字符串
@param dst_ptr  [in] 目标字符串
@param len      [in] 比较的长度
@retval 1   src > dst
@retval 0   src = dst
@retval -1  src < dst
@return 忽略大小写之后的比较结果
@note 我们将字符都转化为小写，然后比较
*******************************************************************************/
LOCAL int32 CmpNoCase(URL_CHAR_T const *src_ptr, URL_CHAR_T const *dst_ptr, int32 len)
{
    int32 i = 0;
    int32 result = 0;
    
    for(i=0; i<len; i++)
    {
        int32 dif = ToLower(src_ptr[i]) - ToLower(dst_ptr[i]);
        
        if(0 != dif)
        {
            result = (dif > 0) ? 1 : (-1);
            break;
        }
    }
    
    return result;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的hostname
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note hostname的regEx大致为: ([\w\d][-\w\d]+[\w\d].)+[\w\d][-\w\d]+[\w\d]
@note 简单讲，即为两个以上的片段，每个片段由数字和字母开始和结束，中间可以是字母、数字和字符"-"，一个片段可以有一个或多个字符
@note 根据实际经验值，我们对最后一个片段做强化检查: 两个或者三个字母
*******************************************************************************/
LOCAL BOOLEAN IsHostname(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_last_dot = TRUE;
    BOOLEAN is_last_hyphen = FALSE;
    int32 seg_count = 0;
    int32 seg_len = 0;
    /*增加一个回溯计数，因为我们检测到".."或者".--"这样的结尾，会去掉这个长度，
    而只判定之前的字符串是否符合格式*/
    int32 append_len = 0;
    BOOLEAN is_hostname = FALSE;
    BOOLEAN is_last_seg_all_alpha = TRUE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    //解析，获取字符特征数据
    do
    {
        if(IsAlpha(*(str_ptr + *confirm_len_ptr)) || IsDigit(*(str_ptr + *confirm_len_ptr)))
        {
            if(is_last_dot)
            {
                seg_count++;
                seg_len = 0;
            }
            *confirm_len_ptr = *confirm_len_ptr + 1;
            seg_len++;
            is_last_hyphen = FALSE;
            is_last_dot = FALSE;
        }
        else if('-' == *(str_ptr + *confirm_len_ptr))
        {
            if(is_last_dot)
            {
                //如果 . 之后是 - 则这两个符号都去掉，不能以 - 开头
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
                break;
            }
            *confirm_len_ptr = *confirm_len_ptr + 1;
            is_last_hyphen = TRUE;
            is_last_dot = FALSE;
        }
        else if('.' == *(str_ptr + *confirm_len_ptr))
        {
            if(is_last_dot || is_last_hyphen)
            {
                //如果连续两个都是 . 或者 -，则两个都去掉
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
                break;
            }
            else
            {
                *confirm_len_ptr = *confirm_len_ptr + 1;
                is_last_hyphen = FALSE;
                is_last_dot = TRUE;
            }
        }
        else
        {
            if(is_last_dot)
            {
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
            }
            break;
        }
    }while(1); /*lint !e506*/

    //检查数据
    do
    {
        //至少有两段
        if(seg_count < 2)
        {
            break;
        }

        //最后一段是有特别要求的

        //只能是两个或者三个
        if(seg_len != 2 && seg_len != 3)
        {
            break;
        }

        //且全是字母
        while(seg_len)
        {
            if(!IsAlpha(*(str_ptr + *confirm_len_ptr - seg_len)))
            {
                is_last_seg_all_alpha = FALSE;
                break;
            }

            seg_len--;
        };

        if(!is_last_seg_all_alpha)
        {
            break;
        }

        is_hostname = TRUE;
    }while(0);

    if(!is_hostname)
    {
        *confirm_len_ptr = *confirm_len_ptr + append_len;
    }
    
    return is_hostname;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的hostnumber
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 简单的regEx: (\d+.){3,3}\d+
@note 即4段由纯数字组成的，用点连接的字符串
@note 附加判断，每段的数值不能大于255
@note 当我们获取到超过4段的时候，截取前4段作为一个hostnumber
*******************************************************************************/
LOCAL BOOLEAN IsHostnumber(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    int32 seg_count = 0; //有且仅有 4 段
    int32 seg_number = 0; // [0,255]
    BOOLEAN is_last_dot = TRUE; //第一个必须是数字，同样，点后必须是数字
    BOOLEAN is_hostnumber = FALSE;
    int32 append_len = 0; //增加一个回溯计数
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    do
    {
        if(IsDigit(*(str_ptr + *confirm_len_ptr)))
        {
            if(is_last_dot)
            {
                seg_count++;
                seg_number = 0;
            }
            seg_number *= 10;
            seg_number += (*(str_ptr + *confirm_len_ptr) - '0');

            if(seg_number > 255)
            {
                break;
            }

            *confirm_len_ptr = *confirm_len_ptr + 1;
            is_last_dot = FALSE;
        }
        else if('.' == *(str_ptr + *confirm_len_ptr))
        {
            if(is_last_dot)
            {
                //如果连续两个点，则两个点都去掉
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
                break;
            }
            else if(4 == seg_count)
            {
                //满了 4 段，截一部分作为url的host
                break;
            }
            else
            {
                *confirm_len_ptr = *confirm_len_ptr + 1;
                is_last_dot = TRUE;
            }
        }
        else
        {
            if(is_last_dot)
            {
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
            }

            break;
        }
    }while(1);/*lint !e506*/

    if(seg_count == 4)
    {
        is_hostnumber = TRUE;
    }
    else
    {
        *confirm_len_ptr = *confirm_len_ptr + append_len;
    }

    return is_hostnumber;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的host
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 一个host，可能是一个hostname，或者是一个hostnumber
@note 所以，大致的regEx: (hostname|hostnumber)
*******************************************************************************/
LOCAL BOOLEAN IsHost(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    return (IsHostname(str_ptr, confirm_len_ptr)
        || IsHostnumber(str_ptr, confirm_len_ptr));
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的port定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note port是一个小于65535的数
@note 大致的regEx: \d+
@note 我们会判定这个数的大小
*******************************************************************************/
LOCAL BOOLEAN IsPort(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    int32 port_number = 0;
    BOOLEAN is_port = TRUE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    while(IsDigit(*(str_ptr + *confirm_len_ptr)))
    {
        port_number *= 10;
        port_number += (*(str_ptr + *confirm_len_ptr) - '0');

        *confirm_len_ptr = *confirm_len_ptr + 1;

        if(port_number > 65535)
        {
            is_port = FALSE;
            break;
        }
    };

    return is_port;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的hostport定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note hostport即host和port的组合，用冒号":"连接
@note 大致的regEx: host(:port)?
@note 其中port是可选的，即一个host也是一个hostport
*******************************************************************************/
LOCAL BOOLEAN IsHostPort(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_host = FALSE;
    int32 host_len = 0;
    BOOLEAN is_port = FALSE;
    int32 port_len = 0;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    is_host = IsHost(str_ptr, &host_len);

    *confirm_len_ptr = host_len;

    if(!is_host)
    {
        return FALSE;
    }


    if(':' == *(str_ptr + *confirm_len_ptr))
    {
        //1 是 : 的长度
        is_port = IsPort(str_ptr + *confirm_len_ptr + 1, &port_len);

        if(is_port)
        {
            *confirm_len_ptr = *confirm_len_ptr + 1 + port_len;
        }
    }

    return TRUE;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的hsegment定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note hsegment即host中的片段，路径片段，搜索片段
@note 由特定的字符集组成，这里的判断相对比较宽松
@note 大致的regEx: [[:uchar:];:@&=]+
*******************************************************************************/
LOCAL BOOLEAN IsHsegment(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_uchar = FALSE;
    int32 uchar_len = 0;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    do
    {
        is_uchar = IsUchar(str_ptr + *confirm_len_ptr, &uchar_len);

        if(is_uchar)
        {
            *confirm_len_ptr = *confirm_len_ptr + uchar_len;
        }
        else if(';' == *(str_ptr + *confirm_len_ptr)
            ||':' == *(str_ptr + *confirm_len_ptr)
            ||'@' == *(str_ptr + *confirm_len_ptr)
            ||'&' == *(str_ptr + *confirm_len_ptr)
            ||'=' == *(str_ptr + *confirm_len_ptr))
        {
            *confirm_len_ptr = *confirm_len_ptr + 1;
        }
        else
        {
            break;
        }
    }while(1);/*lint !e506*/
    

    return (*confirm_len_ptr > 0);
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的hpath定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 一个hpath是一个或多个hsegment的组合，用"/"连接
@note 大致的regEx: hsegment(/hsegment)*
*******************************************************************************/
LOCAL BOOLEAN IsHpath(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_hsegment = FALSE;
    int32 hsegment_len = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    do
    {
        is_hsegment = IsHsegment(str_ptr  + *confirm_len_ptr, &hsegment_len);

        if(is_hsegment)
        {
            *confirm_len_ptr = *confirm_len_ptr + hsegment_len;
        }
        else
        {
            break;
        }

        if('/' == *(str_ptr + *confirm_len_ptr))
        {
            *confirm_len_ptr = *confirm_len_ptr + 1;
        }
        else
        {
            break;
        }
    }while(1);/*lint !e506*/

    return (*confirm_len_ptr > 0);
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的search定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 协议定义为: ([:uchar:];:@&=)+ (同hsegment)
@note 在实际使用，即目前的计算机应用中，已经被扩展
@note 增加识别的范围
@note 大致regEx: [[:xchar:][:national:][:punctuation:]]+
*******************************************************************************/
LOCAL BOOLEAN IsSearch(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_xchar = FALSE;
    int32 xchar_len = 0;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    do
    {
        is_xchar = IsXchar(str_ptr + *confirm_len_ptr, &xchar_len);

        if(is_xchar)
        {
            *confirm_len_ptr = *confirm_len_ptr + xchar_len;
        }
        else if(IsNational(*(str_ptr + *confirm_len_ptr))
            || IsPunctuation(*(str_ptr + *confirm_len_ptr)))
        {
            *confirm_len_ptr = *confirm_len_ptr + 1;
        }
        else
        {
            break;
        }
    }while(1);/*lint !e506*/    

    return (*confirm_len_ptr > 0);
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足url定义的user定义
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 即email的用户名，这是一个比较宽松的判定
@note 在实际应用中，我们比协议的定义更严格一些，即缩小了字符集
@note 大致的regEx: [[:alpha:][:digital:][:safe:]]+
*******************************************************************************/
LOCAL BOOLEAN IsUser(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    while(IsAlpha(*(str_ptr + *confirm_len_ptr))
        || IsDigit(*(str_ptr + *confirm_len_ptr))
        || IsSafe(*(str_ptr + *confirm_len_ptr)))
    {
        *confirm_len_ptr = *confirm_len_ptr + 1;
    }    

    return (*confirm_len_ptr > 0);
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足省略了协议部分的URL
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 即前面的http和https等协议头部分省略的URL
@note regEx: hostport[/hpath[?search]]
@note 如果单独出现，则会被默认为http
*******************************************************************************/
LOCAL BOOLEAN IsUrlDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    BOOLEAN is_hostport = FALSE;
    int32 hostport_len = 0;
    BOOLEAN is_hpath = FALSE;
    int32 hpath_len = 0;
    BOOLEAN is_search = FALSE;
    int32 search_len = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    is_hostport = IsHostPort(str_ptr, &hostport_len);

    *confirm_len_ptr = hostport_len;

    if(!is_hostport)
    {
        return FALSE;
    }

    //has hpath
    if('/' == *(str_ptr + *confirm_len_ptr))
    {
        *confirm_len_ptr = *confirm_len_ptr + 1;

        //增加无hpath的识别
        if('?' == *(str_ptr + *confirm_len_ptr))
        {
            is_hpath = TRUE;
            hpath_len = 0;
        }
        else
        {
            is_hpath = IsHpath(str_ptr + *confirm_len_ptr, &hpath_len);
        }

        if(is_hpath)
        {
            *confirm_len_ptr = *confirm_len_ptr + hpath_len;

            //has search
            if('?' == *(str_ptr + *confirm_len_ptr))
            {
                //1 是 ? 的长度
                is_search = IsSearch(str_ptr + *confirm_len_ptr + 1, &search_len);

                if(is_search)
                {
                    *confirm_len_ptr = *confirm_len_ptr + 1 + search_len;
                }
            }
        }
    }

    return TRUE;
}


/***************************************************************************//*!
@brief 判定一个字符串是不是满足Email省略了"mailto"的格式
@author Lianxiang.Zhou
@param str_ptr          [in] 判定的字符串
@param confirm_len_ptr  [out] 识别的字符串长度
@return BOOLEAN, *confirm_len_ptr会赋予不同的值
@note TRUE - *confirm_len_ptr - 识别的长度
@note FALSE - *confirm_len_ptr - 已检查过的长度
@note 协议定义的一个完整的email是有"mailto:"前缀的
@note 通常都是不写的，即这里的default模式
@note 单独出现的时候，当作email
@note regEx: user@host
*******************************************************************************/
LOCAL BOOLEAN IsEmailDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    BOOLEAN is_user = FALSE;
    int32 user_len = 0;
    BOOLEAN is_host = FALSE;
    int32 host_len = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != confirm_len_ptr); /*assert verified*/
    if (PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    is_user = IsUser(str_ptr, &user_len);

    *confirm_len_ptr = user_len;

    if(!is_user)
    {
        return FALSE;
    }

    //紧跟着必须是 @
    *confirm_len_ptr = *confirm_len_ptr + 1;
    
    if('@' != *(str_ptr + *confirm_len_ptr - 1))
    {
        return FALSE;
    }

    is_host = IsHost(str_ptr + *confirm_len_ptr, &host_len);

    if(is_host)
    {
        *confirm_len_ptr = *confirm_len_ptr + host_len;
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

//public...
//public的函数注释分成两部分，一部分是实现的注释，一部分是头文件的注释
//实现的注释重细节
//头文件的注释，重使用描述

/*==============================================================================
Description: URL_IsHttp
Global resource dependence: s_http_heads_list
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 即协议头为https?, regEx: https?://hostport[/hpath[?search]]
==============================================================================*/
PUBLIC BOOLEAN URL_IsHttp(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    BOOLEAN is_url_default = FALSE;
    int32 http_default_len = 0;
    size_t head_idx = 0;
    
    if (PNULL == str_ptr || PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    for (head_idx = 0; head_idx < ARR_SIZE(s_http_heads_list); head_idx++)
    {
        if (0 == CmpNoCase(s_http_heads_list[head_idx].ptr, str_ptr, s_http_heads_list[head_idx].len))
        {
            *confirm_len_ptr = s_http_heads_list[head_idx].len;
            break;
        }
    }

    if(0 == *confirm_len_ptr)
    {
        return FALSE;
    }

    is_url_default = IsUrlDefault(str_ptr + *confirm_len_ptr, &http_default_len);

    *confirm_len_ptr = *confirm_len_ptr + http_default_len;

    if(!is_url_default)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: 判定一个字符串是不是满足给定【头格式】的url定义
Global resource dependence: s_url_heads_list
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 按照系统定义的协议头列表去识别, regEx: (head)://hostport[/hpath[?search]]
==============================================================================*/
PUBLIC BOOLEAN URL_IsNormalFormat(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    BOOLEAN is_url_default = FALSE;
    int32 http_default_len = 0;
    size_t head_idx = 0;
    
    if (PNULL == str_ptr || PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    *confirm_len_ptr = 0;

    for (head_idx = 0; head_idx < ARR_SIZE(s_url_heads_list); head_idx++)
    {
        if (0 == CmpNoCase(s_url_heads_list[head_idx].ptr, str_ptr, s_url_heads_list[head_idx].len))
        {
            *confirm_len_ptr = s_url_heads_list[head_idx].len;
            break;
        }
    }

    if(0 == *confirm_len_ptr)
    {
        return FALSE;
    }

    is_url_default = IsUrlDefault(str_ptr + *confirm_len_ptr, &http_default_len);

    *confirm_len_ptr = *confirm_len_ptr + http_default_len;

    if(!is_url_default)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: 判定一个字符串是不是满足缺省协议头的url定义
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 协议头缺省的时候，默认为http
==============================================================================*/
PUBLIC BOOLEAN URL_IsHttpDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    return IsUrlDefault(str_ptr, confirm_len_ptr);
}


/*==============================================================================
Description: 判定一个字符串是不是满足email的定义
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: mailto:user@host
==============================================================================*/
PUBLIC BOOLEAN URL_IsMailto(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    int32 const str_len = ARR_SIZE(s_str_http) - 1;
    BOOLEAN is_mailto_default = FALSE;
    int32 mailto_default_len = 0;
    
    if (PNULL == str_ptr || PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    if(0 == CmpNoCase(s_str_mailto, str_ptr, str_len))
    {
        *confirm_len_ptr = str_len;
    }
    else
    {
        return FALSE;
    }

    is_mailto_default = IsEmailDefault(str_ptr + *confirm_len_ptr, &mailto_default_len);

    *confirm_len_ptr = *confirm_len_ptr + mailto_default_len;

    if(!is_mailto_default)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: 判定一个字符串是不是满足缺省email的定义
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 即省略"mailto:", user@host. 缺省形式是更成用的形式
==============================================================================*/
PUBLIC BOOLEAN URL_IsMailtoDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    return IsEmailDefault(str_ptr, confirm_len_ptr);
}


/*==============================================================================
Description: 判定一个字符串是不是满足电话号码的模式
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 简单的regEx可以写作: ([+0][*#\d])?[*#\d]{5,}(([Pp]+|[Ww])[*#\d]+))*
    1 这个不是url的一部分，是为了程序处理的统一方便
    2 这个正则表达式比较难写...所以，这个正则表达式并不是真正的判别模式
    3 增加连字符"-"的识别，识别规则
        1 在首位不识别
        2 连续的连字符不识别
    4 增加字符限制，去除连字符之后超过最大字符数的，不识别
==============================================================================*/
PUBLIC BOOLEAN URL_IsPhoneNumber(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    int32 extra_char_count = 0;
    int32 extra_digit_count = 0;
    int32 hyphen_count = 0;
    int32 tailer_hyphen_count = 0;
    int32 min_number_len = GUIURL_PHONE_NUMBER_LEN_MIN;
    URL_CHAR_T const *str_pos_ptr = PNULL;
    
    
    if (PNULL == str_ptr || PNULL == confirm_len_ptr)
    {
        return FALSE;
    }

    //首字符必须是 [+*#\d]
    *confirm_len_ptr = 1;

    if('+' == *str_ptr)
    {
        min_number_len += 2;
    }
    else if('0' == *str_ptr)
    {
        min_number_len += 2;
    }
    else if(!IsPhoneNumberChar(*str_ptr))
    {
        return FALSE;
    }

    //然后必须有一连串的数字，五个(或者区号一类的一个，共六个)
    while(IsPhoneNumberChar(*(str_ptr + *confirm_len_ptr))
        || IsPhoneNumberHyphen(*(str_ptr + *confirm_len_ptr)))
    {
        *confirm_len_ptr = *confirm_len_ptr + 1;
        
        if(IsPhoneNumberHyphen(*(str_ptr + *confirm_len_ptr - 1)))
        {
            hyphen_count++;
            tailer_hyphen_count++;
            if(tailer_hyphen_count > 1) break;
        }
        else
        {
            tailer_hyphen_count = 0;
        }
    }

    //([+0]\d{2,2})?\d{3,3}
    if(*confirm_len_ptr - hyphen_count < min_number_len)
    {
        return FALSE;
    }

    if(tailer_hyphen_count <= 1)
    {
        //(([Pp]+|[Ww])[*#\d]+))*
        do
        {
            extra_char_count = 0;
            extra_digit_count = 0;

            str_pos_ptr = str_ptr + *confirm_len_ptr;

            if('w' == *str_pos_ptr || 'W' == *str_pos_ptr)
            {
                extra_char_count = 1;
            }
            else if('p' == *str_pos_ptr || 'P' == *str_pos_ptr)
            {
                extra_char_count = 1;
                
                while('p' == *(str_pos_ptr + extra_char_count)
                    || 'P' == *(str_pos_ptr + extra_char_count))
                {
                    extra_char_count++;
                }
            }

            if(extra_char_count == 0) break;

            tailer_hyphen_count = 0;

            str_pos_ptr = str_ptr + *confirm_len_ptr + extra_char_count;
            while(IsPhoneNumberChar(*(str_pos_ptr + extra_digit_count))
                || IsPhoneNumberHyphen(*(str_pos_ptr + extra_digit_count)))
            {
                extra_digit_count++;

                if(IsPhoneNumberHyphen(*(str_pos_ptr + extra_digit_count - 1)))
                {
                    hyphen_count++;
                    tailer_hyphen_count++;
                    if(tailer_hyphen_count > 1) break;
                }
                else
                {
                    tailer_hyphen_count = 0;
                }
            }

            if(extra_digit_count - tailer_hyphen_count == 0) break;

            *confirm_len_ptr = *confirm_len_ptr + extra_char_count + extra_digit_count;

            if(tailer_hyphen_count > 1) break;
        }while(1);/*lint !e506*/
    }

    *confirm_len_ptr = *confirm_len_ptr - tailer_hyphen_count;
    hyphen_count -= tailer_hyphen_count;
    

    return (*confirm_len_ptr - hyphen_count <= GUIURL_PHONE_NUMBER_LEN_MAX);
}



/***************************************************************************//*!
@brief 构造一个双向单循环链表(prev循环，next不循环)
@author Lianxiang.Zhou
@param head_pptr [in, out] 链表头结点
@param type [in] 新节点的类型
@param off_set [in] 新节点的偏移
@param len [in] 新节点的长度
*******************************************************************************/
LOCAL void Addnode(
    URL_DATA_T **head_pptr,
    URL_PARSE_TYPE_T type,
    uint32 off_set,
    uint32 len
)
{
    URL_DATA_T *new_node_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != head_pptr); /*assert verified*/
    if (PNULL == head_pptr)
    {
        return;
    }

    new_node_ptr = SCI_ALLOC_APP(sizeof(URL_DATA_T));

    new_node_ptr->type = type;
    new_node_ptr->off_set = off_set;
    new_node_ptr->len = len;
    new_node_ptr->next = PNULL;

    if (PNULL == *head_pptr)
    {
        *head_pptr = new_node_ptr;
    }
    else
    {
        SCI_ASSERT(PNULL != (*head_pptr)->prev); /*assert verified*/
        (*head_pptr)->prev->next = new_node_ptr;
        new_node_ptr->prev = (*head_pptr)->prev;
    }

    (*head_pptr)->prev = new_node_ptr;

    return;
}

    


/*==============================================================================
Description: 解析字符串中的可识别字符串
Author: Lianxiang.Zhou
Note: 简单的解析算法，由guiurl.c完成
Warning: 因为电话号码可以作为email和url的一部分，我们必须按照
    (email, url) -> phone number 的顺序去解析
==============================================================================*/
PUBLIC int32 URL_ParseDataGen(
    URL_CHAR_T const * str_ptr,
    URL_PARSE_TYPE_T types,
    URL_DATA_T** data_pptr
)
{
    URL_CHAR_T const *p = PNULL;
    int32 pos = 0;
    int32 cnt = 0;

    //本着局部变量"专款专用"的原则，我们分别定义变量
    //x_confirm_len 是用来避免重复识别已经在某一类型上识别失败的字符串
    BOOLEAN is_parse_email = FALSE;
    int32 email_confirm_len = 0;
    int32 email_len = 0;
    BOOLEAN is_email = FALSE;

    BOOLEAN is_parse_url = FALSE;
    int32 url_confirm_len = 0;
    int32 url_len = 0;
    BOOLEAN is_url = FALSE;

    BOOLEAN is_parse_phonenum = FALSE;
    int32 pn_confirm_len = 0;
    int32 pn_len = 0;
    BOOLEAN is_pn = FALSE;


    if (PNULL == str_ptr || PNULL == data_pptr)
    {
        return 0;
    }

    //快速判断，无解析需求
    if (!(URL_PARSE_ALL & types)) /*lint !e655*/
    {
        return 0;
    }

    is_parse_email = URL_PARSE_EMAIL & types; /*lint !e655*/
    is_parse_url = URL_PARSE_URL & types; /*lint !e655*/
    is_parse_phonenum = URL_PARSE_PHONENUM & types; /*lint !e655*/

    while (*(str_ptr + pos))
    {
        p = str_ptr + pos;

        do
        {
            if (is_parse_email && email_confirm_len <= pos)
            {
                is_email = (URL_IsMailto(p, &email_len) || URL_IsMailtoDefault(p, &email_len));

                email_confirm_len = pos + email_len;

                if (is_email)
                {
                    Addnode(data_pptr, URL_PARSE_EMAIL, pos, email_len);
                    cnt++;
                    pos += email_len;

                    break;
                }
            }

            if (is_parse_url && url_confirm_len <= pos)
            {
                is_url = (URL_IsNormalFormat(p, &url_len) || URL_IsHttpDefault(p, &url_len));

                url_confirm_len = pos + url_len;

                if (is_url)
                {
                    Addnode(data_pptr, URL_PARSE_URL, pos, url_len);
                    cnt++;
                    pos += url_len;

                    break;
                }
            }

            if (is_parse_phonenum && pn_confirm_len <= pos)
            {
                is_pn = (URL_IsPhoneNumber(p, &pn_len));

                pn_confirm_len = pos + pn_len;

                if (is_pn)
                {
                    Addnode(data_pptr, URL_PARSE_PHONENUM, pos, pn_len);
                    cnt++;
                    pos += pn_len;

                    break;
                }
            }

            pos++;

        }
        while (0);
    }

    return cnt;
}


/*==============================================================================
Description: 释放解析数据的空间
Author: Lianxiang.Zhou
==============================================================================*/
PUBLIC void URL_ParseDataRelease(URL_DATA_T *data_ptr)
{
    URL_DATA_T *cur_node_ptr = data_ptr;
    URL_DATA_T *free_node_ptr = PNULL;
    
    while (cur_node_ptr)
    {
        free_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next;
        SCI_FREE(free_node_ptr);
    }
    
    return;
}


