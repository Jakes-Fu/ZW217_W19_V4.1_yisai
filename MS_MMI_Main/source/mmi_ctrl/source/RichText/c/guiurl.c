/*! @file guiurl.c
@brief ����url, RFC1738
@author Lianxiang.Zhou
@version 1.0
@date 2011/03/25
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@note
- 2009/09/01   Lianxiang.Zhou  Create.
@details �����ַ����е�url�ַ���
- URL�������֣���RFC1738Ϊ����
- ���ʵ�ʣ���ϸ�ڲ�������Ӧ����ǿ�ͷſ�
- ����绰����Ľ�������������Ϊ��������
- ���Զ�һ���ַ���������ȫ�����������ַ��������е������ַ���
- �ڸ�ģ�飬���ǻ�Ϊ�˼���������������ɺܶ�Ķ�����ж�
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
@brief urlЭ��ͷ�Ĵ洢������֧��ָ����Э��ͷ������ʶ��ָ����Э��ͷ
@note ��ʵ����ͨ�����ַ����ṹ
*/
typedef struct
{
    URL_CHAR_T const *ptr;
    size_t len;
}URL_HEAD_STRNG_T;

/*! @var s_str_http
@brief �ַ���"http://"
@note URL_CHAR_T��ȷ��������£������������ʽ����ͬ
*/
static URL_CHAR_T const s_str_http[] = {'h', 't', 't', 'p', ':', '/', '/', '\0'};
/*! @var s_str_https
@brief �ַ���"https://"
*/
static URL_CHAR_T const s_str_https[] = {'h', 't', 't', 'p', 's',':', '/', '/', '\0'};
/*! @var s_str_rtsp
@brief �ַ���"rtsp://"
*/
static URL_CHAR_T const s_str_rtsp[] = {'r', 't', 's', 'p', ':', '/', '/', '\0'};
/*! @var s_str_mailto
@brief �ַ���"mailto:"
*/
static URL_CHAR_T const s_str_mailto[] = {'m', 'a', 'i', 'l', 't', 'o', ':', '\0'};

/*! @var s_http_heads_list
@brief �ж���ַ��Э��ͷ��������Ϊhttp��https�ǺϷ���
*/
static URL_HEAD_STRNG_T const s_http_heads_list[] =
{
    {s_str_http, ARR_SIZE(s_str_http) - 1},
    {s_str_https, ARR_SIZE(s_str_https) - 1},
};

/*! @var s_url_heads_list
@brief �ж�ϵͳʶ���Э��ͷ��Ŀǰ��ʶ�𲿷�
*/
static URL_HEAD_STRNG_T const s_url_heads_list[] =
{
    {s_str_http, ARR_SIZE(s_str_http) - 1},
    {s_str_https, ARR_SIZE(s_str_https) - 1},
    {s_str_rtsp, ARR_SIZE(s_str_rtsp) - 1},
};


/*------------------------------------------------------------------------------
    Inline Function(����С����) 
------------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief �ж��ַ��Ƿ�Сд��ĸa-z
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsLowalpha(URL_CHAR_T c)
{
    return ('a' <= c && c <= 'z');
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ��д��ĸA-Z
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsHialpha(URL_CHAR_T c)
{
    return ('A' <= c && c <= 'Z');
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ���ĸa-zA-Z
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsAlpha(URL_CHAR_T c)
{
    return (IsLowalpha(c) || IsHialpha(c));
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�����0-9
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsDigit(URL_CHAR_T c)
{
    return ('0' <= c && c <= '9');
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�"safe"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "safe char"��Э�����һ������������Ϊһ���ַ���
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
@brief �ж��ַ��Ƿ�"extra"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "extra char"��Э�����һ������������Ϊһ���ַ���
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
@brief �ж��ַ��Ƿ�"national"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "national char"��Э�����һ������������Ϊһ���ַ���
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
@brief �ж��ַ��Ƿ�"punctuation"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "punctuation char"��Э�����һ������������Ϊһ���ַ���
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
@brief �ж��ַ��Ƿ�"reserved"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "reserved char"��Э�����һ������������Ϊһ���ַ���
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
@brief �ж��ַ��Ƿ�"hex"�ַ�, ��16���������ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
*******************************************************************************/
LOCAL __inline BOOLEAN IsHex(URL_CHAR_T c)
{
    return (IsDigit(c)
        || ('a' <= c && c <= 'f')
        || ('A' <= c && c <= 'F'));    
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�"escape"�ַ�
@author Lianxiang.Zhou
@param pc [in] ���ж����ַ���ָ��
@return TURE or FALSE
@note "escape char"��Э�����һ������������Ϊһ���ַ���
@note һ��"escap char"��"%hh"��ģʽ����3���ַ�����
*******************************************************************************/
LOCAL __inline BOOLEAN IsEscape(URL_CHAR_T const *pc)
{
    return ('%' == *pc && IsHex(*(pc+1)) && IsHex(*(pc+2)));
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�"unreserved"�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note "unreserved char"��Э�����һ������������Ϊһ���ַ���
*******************************************************************************/
LOCAL __inline BOOLEAN IsUnreserved(URL_CHAR_T c)
{
    return (IsAlpha(c) || IsDigit(c) || IsSafe(c) || IsExtra(c));
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�"uchar"�ַ�
@author Lianxiang.Zhou
@param pc [in] ���ж����ַ���ָ��
@param len_ptr [out] ʶ���ַ�(��)�ĳ���
@return TURE or FALSE
@note "uchar char"��Э�����һ������������Ϊһ���ַ���
@note ������һ���ַ���Ҳ�����������ַ�(escape)
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
@brief �ж��ַ��Ƿ�"xchar"�ַ�
@author Lianxiang.Zhou
@param pc [in] ���ж����ַ���ָ��
@param len_ptr [out] ʶ���ַ�(��)�ĳ���
@return TURE or FALSE
@note "xchar char"��Э�����һ������������Ϊһ���ַ���
@note ������һ���ַ���Ҳ�����������ַ�(escape)
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
@brief �ж��ַ��Ƿ񹹳ɵ绰������ַ�[*#0-9]
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note ��Ϊ����Ĵ����Լ������룬���Ǽ��жϣ���"*#"�����򵥵�����ȥʶ��
*******************************************************************************/
LOCAL __inline BOOLEAN IsPhoneNumberChar(URL_CHAR_T c)
{
    return (IsDigit(c)
        || '*' == c
        || '#' == c);
}

/***************************************************************************//*!
@brief �ж��ַ��Ƿ�绰�������ӷ��ַ�"-"
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return TURE or FALSE
@note �绰�����ͨ�����ӷ�����"-", �����б��
*******************************************************************************/
LOCAL __inline BOOLEAN IsPhoneNumberHyphen(URL_CHAR_T c)
{
    return ('-' == c);
}

/***************************************************************************//*!
@brief ���ַ�ת����Сд�ַ�
@author Lianxiang.Zhou
@param c [in] ���ж����ַ�
@return ת������ַ�
@note �ּ������:
    - ��СתСд
    - ����ԭ�ַ�����
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
@brief ���Դ�Сд���Ƚ��ַ���
@author Lianxiang.Zhou
@param src_ptr  [in] Դ�ַ���
@param dst_ptr  [in] Ŀ���ַ���
@param len      [in] �Ƚϵĳ���
@retval 1   src > dst
@retval 0   src = dst
@retval -1  src < dst
@return ���Դ�Сд֮��ıȽϽ��
@note ���ǽ��ַ���ת��ΪСд��Ȼ��Ƚ�
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
@brief �ж�һ���ַ����ǲ�������url�����hostname
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note hostname��regEx����Ϊ: ([\w\d][-\w\d]+[\w\d].)+[\w\d][-\w\d]+[\w\d]
@note �򵥽�����Ϊ�������ϵ�Ƭ�Σ�ÿ��Ƭ�������ֺ���ĸ��ʼ�ͽ������м��������ĸ�����ֺ��ַ�"-"��һ��Ƭ�ο�����һ�������ַ�
@note ����ʵ�ʾ���ֵ�����Ƕ����һ��Ƭ����ǿ�����: ��������������ĸ
*******************************************************************************/
LOCAL BOOLEAN IsHostname(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    BOOLEAN is_last_dot = TRUE;
    BOOLEAN is_last_hyphen = FALSE;
    int32 seg_count = 0;
    int32 seg_len = 0;
    /*����һ�����ݼ�������Ϊ���Ǽ�⵽".."����".--"�����Ľ�β����ȥ��������ȣ�
    ��ֻ�ж�֮ǰ���ַ����Ƿ���ϸ�ʽ*/
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

    //��������ȡ�ַ���������
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
                //��� . ֮���� - �����������Ŷ�ȥ���������� - ��ͷ
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
                //��������������� . ���� -����������ȥ��
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

    //�������
    do
    {
        //����������
        if(seg_count < 2)
        {
            break;
        }

        //���һ�������ر�Ҫ���

        //ֻ����������������
        if(seg_len != 2 && seg_len != 3)
        {
            break;
        }

        //��ȫ����ĸ
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
@brief �ж�һ���ַ����ǲ�������url�����hostnumber
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note �򵥵�regEx: (\d+.){3,3}\d+
@note ��4���ɴ�������ɵģ��õ����ӵ��ַ���
@note �����жϣ�ÿ�ε���ֵ���ܴ���255
@note �����ǻ�ȡ������4�ε�ʱ�򣬽�ȡǰ4����Ϊһ��hostnumber
*******************************************************************************/
LOCAL BOOLEAN IsHostnumber(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    int32 seg_count = 0; //���ҽ��� 4 ��
    int32 seg_number = 0; // [0,255]
    BOOLEAN is_last_dot = TRUE; //��һ�����������֣�ͬ����������������
    BOOLEAN is_hostnumber = FALSE;
    int32 append_len = 0; //����һ�����ݼ���
    
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
                //������������㣬�������㶼ȥ��
                *confirm_len_ptr = *confirm_len_ptr - 1;
                append_len = 2;
                break;
            }
            else if(4 == seg_count)
            {
                //���� 4 �Σ���һ������Ϊurl��host
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
@brief �ж�һ���ַ����ǲ�������url�����host
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note һ��host��������һ��hostname��������һ��hostnumber
@note ���ԣ����µ�regEx: (hostname|hostnumber)
*******************************************************************************/
LOCAL BOOLEAN IsHost(URL_CHAR_T const * str_ptr, int32* confirm_len_ptr)
{
    return (IsHostname(str_ptr, confirm_len_ptr)
        || IsHostnumber(str_ptr, confirm_len_ptr));
}


/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������url�����port����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note port��һ��С��65535����
@note ���µ�regEx: \d+
@note ���ǻ��ж�������Ĵ�С
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
@brief �ж�һ���ַ����ǲ�������url�����hostport����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note hostport��host��port����ϣ���ð��":"����
@note ���µ�regEx: host(:port)?
@note ����port�ǿ�ѡ�ģ���һ��hostҲ��һ��hostport
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
        //1 �� : �ĳ���
        is_port = IsPort(str_ptr + *confirm_len_ptr + 1, &port_len);

        if(is_port)
        {
            *confirm_len_ptr = *confirm_len_ptr + 1 + port_len;
        }
    }

    return TRUE;
}


/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������url�����hsegment����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note hsegment��host�е�Ƭ�Σ�·��Ƭ�Σ�����Ƭ��
@note ���ض����ַ�����ɣ�������ж���ԱȽϿ���
@note ���µ�regEx: [[:uchar:];:@&=]+
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
@brief �ж�һ���ַ����ǲ�������url�����hpath����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note һ��hpath��һ������hsegment����ϣ���"/"����
@note ���µ�regEx: hsegment(/hsegment)*
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
@brief �ж�һ���ַ����ǲ�������url�����search����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note Э�鶨��Ϊ: ([:uchar:];:@&=)+ (ͬhsegment)
@note ��ʵ��ʹ�ã���Ŀǰ�ļ����Ӧ���У��Ѿ�����չ
@note ����ʶ��ķ�Χ
@note ����regEx: [[:xchar:][:national:][:punctuation:]]+
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
@brief �ж�һ���ַ����ǲ�������url�����user����
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note ��email���û���������һ���ȽϿ��ɵ��ж�
@note ��ʵ��Ӧ���У����Ǳ�Э��Ķ�����ϸ�һЩ������С���ַ���
@note ���µ�regEx: [[:alpha:][:digital:][:safe:]]+
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
@brief �ж�һ���ַ����ǲ�������ʡ����Э�鲿�ֵ�URL
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note ��ǰ���http��https��Э��ͷ����ʡ�Ե�URL
@note regEx: hostport[/hpath[?search]]
@note ����������֣���ᱻĬ��Ϊhttp
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

        //������hpath��ʶ��
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
                //1 �� ? �ĳ���
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
@brief �ж�һ���ַ����ǲ�������Emailʡ����"mailto"�ĸ�ʽ
@author Lianxiang.Zhou
@param str_ptr          [in] �ж����ַ���
@param confirm_len_ptr  [out] ʶ����ַ�������
@return BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
@note TRUE - *confirm_len_ptr - ʶ��ĳ���
@note FALSE - *confirm_len_ptr - �Ѽ����ĳ���
@note Э�鶨���һ��������email����"mailto:"ǰ׺��
@note ͨ�����ǲ�д�ģ��������defaultģʽ
@note �������ֵ�ʱ�򣬵���email
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

    //�����ű����� @
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
//public�ĺ���ע�ͷֳ������֣�һ������ʵ�ֵ�ע�ͣ�һ������ͷ�ļ���ע��
//ʵ�ֵ�ע����ϸ��
//ͷ�ļ���ע�ͣ���ʹ������

/*==============================================================================
Description: URL_IsHttp
Global resource dependence: s_http_heads_list
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
Note: ��Э��ͷΪhttps?, regEx: https?://hostport[/hpath[?search]]
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
Description: �ж�һ���ַ����ǲ������������ͷ��ʽ����url����
Global resource dependence: s_url_heads_list
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
Note: ����ϵͳ�����Э��ͷ�б�ȥʶ��, regEx: (head)://hostport[/hpath[?search]]
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
Description: �ж�һ���ַ����ǲ�������ȱʡЭ��ͷ��url����
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
Note: Э��ͷȱʡ��ʱ��Ĭ��Ϊhttp
==============================================================================*/
PUBLIC BOOLEAN URL_IsHttpDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    return IsUrlDefault(str_ptr, confirm_len_ptr);
}


/*==============================================================================
Description: �ж�һ���ַ����ǲ�������email�Ķ���
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
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
Description: �ж�һ���ַ����ǲ�������ȱʡemail�Ķ���
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
Note: ��ʡ��"mailto:", user@host. ȱʡ��ʽ�Ǹ����õ���ʽ
==============================================================================*/
PUBLIC BOOLEAN URL_IsMailtoDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr)
{
    return IsEmailDefault(str_ptr, confirm_len_ptr);
}


/*==============================================================================
Description: �ж�һ���ַ����ǲ�������绰�����ģʽ
Author: Lianxiang.Zhou
Return: BOOLEAN, *confirm_len_ptr�ḳ�費ͬ��ֵ
    TRUE - ��
        *confirm_len_ptr - ʶ��ĳ���
    FALSE - ��
        *confirm_len_ptr - �Ѽ����ĳ���
Note: �򵥵�regEx����д��: ([+0][*#\d])?[*#\d]{5,}(([Pp]+|[Ww])[*#\d]+))*
    1 �������url��һ���֣���Ϊ�˳������ͳһ����
    2 ���������ʽ�Ƚ���д...���ԣ����������ʽ�������������б�ģʽ
    3 �������ַ�"-"��ʶ��ʶ�����
        1 ����λ��ʶ��
        2 ���������ַ���ʶ��
    4 �����ַ����ƣ�ȥ�����ַ�֮�󳬹�����ַ����ģ���ʶ��
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

    //���ַ������� [+*#\d]
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

    //Ȼ�������һ���������֣����(��������һ���һ����������)
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
@brief ����һ��˫��ѭ������(prevѭ����next��ѭ��)
@author Lianxiang.Zhou
@param head_pptr [in, out] ����ͷ���
@param type [in] �½ڵ������
@param off_set [in] �½ڵ��ƫ��
@param len [in] �½ڵ�ĳ���
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
Description: �����ַ����еĿ�ʶ���ַ���
Author: Lianxiang.Zhou
Note: �򵥵Ľ����㷨����guiurl.c���
Warning: ��Ϊ�绰���������Ϊemail��url��һ���֣����Ǳ��밴��
    (email, url) -> phone number ��˳��ȥ����
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

    //���žֲ�����"ר��ר��"��ԭ�����Ƿֱ������
    //x_confirm_len �����������ظ�ʶ���Ѿ���ĳһ������ʶ��ʧ�ܵ��ַ���
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

    //�����жϣ��޽�������
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
Description: �ͷŽ������ݵĿռ�
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


