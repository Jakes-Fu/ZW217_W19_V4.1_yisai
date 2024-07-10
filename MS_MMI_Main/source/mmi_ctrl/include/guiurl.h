/*! @file guiurl.h
@brief ���ַ������������ַ���
@author Lianxiang.Zhou
@version 1.0
@date 2009/08/08
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details һ�����������ǻ���Ҫ�ж�һ���ַ������Ƿ�������ض����ַ���������
- url
- email
- phone number
�ⲿ�ֱ�������Ƕ��richtext�ģ����ǿ��ǵ�����richtext�����еģ��㽫�����������

@note 2009/09/01-Lianxiang.Zhou-Create.
@warning �˴�������
- ������һ����֧�����е�Э��Ľ������᲻�ϵĸ�����Ҫ����
- ���������ȷЭ��ģ�����Э��Ĺ淶ȥ����
- û����ȷЭ��ģ���ʡ����Э��ͷ���ֵģ�����httpЭ�����
@warning ��ģ�������ַ�������Ҫ�����ַ����Ľ�β�ַ�'\0'���ж��ַ����Ľ���
@sa urlGrp
@sa @ref page_Url
*******************************************************************************/

/*! @page page_Url �ַ�������ʶ��(URL)

-# @ref sec_url_intro
-# @ref sec_url_include
-# @ref sec_url_how_to_use

@section sec_url_intro ���ֽ����ĸ���

- �ַ�����������������URL������������EMAIL��Phone Number�Ľ�����
- �����Ľ��û����ȷ���߲���ȷ��˵����ֻ��׼ȷ��׼ȷ��˵������׼ȷδ�ؾ��Ǵ�ġ�
- ���������ַ������������������ڽ����ľ��Ⱥͽ������Ѷ�֮����һ��ƽ�⡣
- ����Ľ��������ֻ����һ������£��Ǻ���ġ�

@section sec_url_include �����ķ�Χ

- ���ǻ��ṩһЩ���͵��ַ�����ʶ�𣬰�����
    -# �����ʼ���ַ
    -# ��ַ
    -# �绰����
- ���ڵ����ʼ���ַ����ַ��ʶ������ǰ���RFC1738Ϊ��׼���ڴ˻����ϸ����ֻ���ʵ����������һЩС�ĸĶ���
- ���ڵ绰�����ʶ�����ǰ���һ��ĳ��ù�����ʶ��ġ�
- ʶ�����飬�������صĺ���˵����

@section sec_url_how_to_use ���ʹ�ý������ܣ�

ֱ�ӵ��ñ�ģ���ṩ�Ľ��������������б�����˵���μ�ģ�� @ref urlGrp ��

@sa urlGrp
*******************************************************************************/

/*! @addtogroup urlGrp Url
@brief �ַ�����������
@details ���ַ������Ľ������������ַ��ľ�������
- ��ģ���Ǵ�Richtextģ���ж���������
- ��ģ�������guiurl.c��guiurl.h�����ļ�

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
@brief �绰�������̳���
*/
#define GUIURL_PHONE_NUMBER_LEN_MIN (5)

/*! @def GUIURL_PHONE_NUMBER_LEN_MAX
@brief �绰�������󳤶�
*/
#define GUIURL_PHONE_NUMBER_LEN_MAX (40)

/*------------------------------------------------------------------------------
    TYPE AND STRUCT
------------------------------------------------------------------------------*/
/*! @typedef URL_CHAR_T
@brief ����urlʶ����ַ����ͣ�Ŀǰ���ǵ�ϵͳĬ�ϵ��ǿ��ַ�
*/
typedef wchar URL_CHAR_T;


/*! @enum URL_PARSE_TYPE_T
@brief URLʶ�����ͣ������
*/
typedef enum
{
    URL_PARSE_URL = 1, /*!< ʶ��url */
    URL_PARSE_EMAIL = 1 << 1, /*!< ʶ��email */
    URL_PARSE_PHONENUM = 1 << 2, /*!< ʶ��绰���� */

    URL_PARSE_ALL = (URL_PARSE_URL | URL_PARSE_EMAIL | URL_PARSE_PHONENUM)
} URL_PARSE_TYPE_T;


/*! @struct URL_DATA_T
@brief �����ó���url����
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
@brief �ж�һ���ַ����ǲ�������http��ʽ��url����
@details �� @ref URL_IsNormalFormat ���ض��汾����ǿ��Э��ͷ���жϣ����޶�Э��ͷΪ(https?)�����µ�regEx�ǣ�
@code
(https?://)?hostport[/hpath[?search]]
@endcode
��ϸ���ֲο� @ref URL_IsNormalFormat �еĶ��塣

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [int/out] ����http��ʽ���ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ��http��ʽ��url
@note ����http��https
@sa URL_IsNormalFormat
*******************************************************************************/
PUBLIC BOOLEAN URL_IsHttp(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief �ж�һ���ַ����Ƿ�URL��ʽ������Э��ͷ��Ԥ�跶Χ��
@details ���µ�regEx�ǣ�
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
		[:uchar:] - �ַ�����([-_$.+!*\()a-zA-z0-9]|%[0-9a-fA-F]{2})
search - ͬhsegment
@endcode
��Ȼ�����ǻ���RFC1738Э��Ļ���������һЩ���ƣ����磺
- ����Э��ͷ���޶����Ϸ���Э��ͷ�Ǻܶ�ģ����������Ŀǰϵͳ��Ҫʶ���Э��ͷ������һ�����ơ�
- ����hostname�����һ�������ǰ��վ���ֵ���޶�Ϊ2-3����ĸ��

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [in/out] ����url��ʽ���ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ��url
@sa URL_IsHttp
*******************************************************************************/
PUBLIC BOOLEAN URL_IsNormalFormat(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������http��ʽ��url���壬����ʡ����Э�鲿�ֵ�
@details �� @ref URL_IsHttp �ļ򻯰汾����Ĭ��Э��ͷΪ(https?)�����µ�regEx�ǣ�
@code
hostport[/hpath[?search]]
@endcode
��ϸ���ֲο� @ref URL_IsNormalFormat �еĶ��塣

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [in] �ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ���򻯵�url
@sa URL_IsHttp
@sa URL_IsNormalFormat
*******************************************************************************/
PUBLIC BOOLEAN URL_IsHttpDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������mailto��ʽ��url����
@details ����RFC1738Э��Ϊ��׼��һ���ж������µ�regEx�ǣ�
@code
(mailto:)?user@host
user - [[:alpha:][:digital:][:safe:]]+
	[:alpha:] - [a-zA-Z]
	[:digital:] - [0-9]
	[:safe:] - [-_$.+]
host - ͬurl��host
@endcode
- host���֣��ο� @ref URL_IsNormalFormat �еĶ��塣
- ������Ƕ�user���ַ���Ҳ����һЩ�޸ģ�������һЩ���ǳ��á����ַ���ȥ������ǿ���ж�

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [in] �ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ��email�ַ���
*******************************************************************************/
PUBLIC BOOLEAN URL_IsMailto(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������mailto��ʽ��url���壬����ʡ����Э�鲿�ֵ�
@details ���� @ref URL_IsMailto �ļ򻯰汾��ʡ����Э��ͷ��mailto:�����֡��ⲿ����ͨ�����ж��У����ǲ���Ҫ�ġ����µ�regEx�ǣ�
@code
user@host
@endcode
��ϸ���壬�ο�  @ref URL_IsMailto �еĶ���

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [in] �ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ���򻯵�email�ַ���
*******************************************************************************/
PUBLIC BOOLEAN URL_IsMailtoDefault(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);

/***************************************************************************//*!
@brief �ж�һ���ַ����ǲ�������绰�����������ʽ
@details �绰������б𣬲���url��һ���֣��������ַ��������б��һ���֡����µ�regEx�ǣ�
@code
(([+0][*#\d]{6,})|([*#\d]{5,}))(([Pp]+|[Ww])[*#\d]+))*
@endcode
- �������ַ���-������
    -# ����λ��ʶ��
    -# ���������ַ���ʶ��
    -# ���ַ�������λ��
- ����λ������40���ڳ������ʶ���ַ��󣬲�ʶ��
- �����飺
    -# ���벿��@code (([+0][*#\d]{6,})|([*#\d]{5,})) @endcode
        -# @code [+0][*#\d]{6,} @endcode - ��+����0��ͷ�ģ���������6������������
        -# @code [*#1-9][*#\d]{4,} @endcode - ���㿪ͷ�����ִ�������5λ
    -# ���Ӳ������� @code (([Pp]+|[Ww])[*#\d]+))* @endcode
        -# ��ͣ��p����P�� - �����������ͣ������������ַ�
        -# �ȴ���w����W�� - ֻ��һ���ȴ������һ���ַ�
- ���䣺�˴��Եȴ��ĺ������û���ϸ�Ҫ���Ǵ����֣���Ϊ�˼��жϡ�

@author Lianxiang.Zhou
@param str_ptr [in] �ַ���ָ��
@param confirm_len_ptr [in] �ַ��ĳ���
@return �ӵ�ǰ�ַ��Ŀ�ͷ�Ƿ���һ���绰�����ַ���
@note ���ں�����жϣ��ڲ�ͬ��Ӧ���У����в�ͬ��Ҫ��
-# �������url��һ���֣���Ϊ�˳������ͳһ����
-# ���������ʽ�Ƚ���д...
*******************************************************************************/
PUBLIC BOOLEAN URL_IsPhoneNumber(URL_CHAR_T const * str_ptr, int32 *confirm_len_ptr);


/***************************************************************************//*!
@brief �����ַ�����������������
@author Lianxiang.Zhou
@param str_ptr [in] �ַ���
@param types [in] ��Ҫ����������
@param data_pptr [out] ��������Ĵ��ָ��
@return ���ؽ��������������ַ����ĸ���
@note �ַ���������'\0'��β
@note ��������Ŀռ��ɽ���������̬����
@warning data_pptr�������ſؼ���һ����д��ָ���ָ��
@warning ������� @ref URL_ParseDataRelease �ͷ����ݵĴ洢�ؼ�
@sa URL_ParseDataRelease
*******************************************************************************/
PUBLIC int32 URL_ParseDataGen(
    URL_CHAR_T const * str_ptr,
    URL_PARSE_TYPE_T types,
    URL_DATA_T** data_pptr
);

/***************************************************************************//*!
@brief �ͷŹ�������ʱ��̬����Ĵ洢�ռ�
@author Lianxiang.Zhou
@param data_ptr [in] ���ݴ洢�ռ�ָ��
@note �˴��ͷŵ������Ǻ��� @ref URL_ParseDataGen �����ɵ�����
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
