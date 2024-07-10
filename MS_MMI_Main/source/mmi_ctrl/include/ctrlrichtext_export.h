/*! @file guirichtext.h
@brief Richtext�ؼ��Ĵ����Ϳ�����ص����ͺͺ��� 
@author lianxiang.zhou
@version 1.0
@date 05/30/2007
@remarks Copyright 2007-2011 Spreadtrum, Incoporated. All Rights Reserved.
@details ���ļ���richtext�ؼ������岿�֣��ṩ�����Ϳ���richtext����Ҫ���������ͺͺ�����
- ����������richtext
- �趨richtext�ĸ������
- ���ӡ�ɾ�����޸�richtext����ʾ��

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

@section sec_richtext_intro Richtext�ؼ�����

Richtext�ؼ�����ʾͼƬ�����ֵĿؼ��������趨����(item)���ϵ���˳����ʾ����һ�����п���������һ��ͼƬ��һ�����֣�������һ��ͼƬ����һ�����֡���һ������ܣ��ҽ������������г����������
-# ��ͼƬ��֧�ֵĸ�ʽ��� @ref _GUIRICHTEXT_IMAGE_TYPE_E
-# �����֣�֧�ֵĸ�ʽ��� @ref _GUIRICHTEXT_TEXT_TYPE_E
-# ͼƬ�����ֵĻ�ϣ����ֺ�ͼƬ�����λ���趨��� @ref _GUIRICHTEXT_ALIGN_E

@section sec_richtext_usage richtext����;

������Ӧ������ʾ���ֺ�ͼƬ���ɶ����ֺ�ͼƬ����ʾ��ʽ���ض��ĵ��ڡ��������顢���ź͵绰�����й̶����ʵ�Ӧ����ʾ��

@section sec_richtext_oprator Richtext�Ĳ���

����richtext�����ǿɿصķ��������
- ����������
- ������ɾ�����޸���(item)
- ����richtext�ؼ���������ʾ����
- ���õ��������ʾ����
- ����richtext�ؼ��ĸ��ӹ��ܣ�������������

@section sec_richtext_support richtext֧�ֵĹ���

richtext�Ļ��������ǡ���ʾ�������ԣ�����ʾ��ʱ�򣬻���һЩ��Ҫ��֧�֣�
- �Զ���ҳ����ҳ���ܡ�
- ֧�ְ����ʹ��ʣ����ϵͳ֧�֣��Ĳ�����
- ��ҳ���Զ�����������ڷ�ҳ�Ĺ����У�΢����ҳ���룬ʹ�á����Ե���롣
- ��ҳ���Զ��������֣������ڷ�ҳ�Ĺ����У�΢����ҳ���룬ʹ�á������С���Ե���롣
- ϵͳ�����а塱���ܣ����ϵͳ֧�֣�Ĭ�ϴ򿪡�����趨����Ϊrichtext��֧�ָù��ܣ����μ� @ref GUIRICHTEXT_DisableClipBoard
- �ַ����������ܣ��������ַ������������url��email��phone number���μ� @ref page_Url
- �Խ����������ַ�����������ʾ���㡣Ҳ�����ڲ�����������£���item���ý��㡣�μ� @ref GUIRICHTEXT_TAG_TYPE_E

@section sec_richtext_settings richtext�Ŀ����ò���

��richtext����ʾ�У��û������ʾ��ϸ�������ͻ��������������ṩһЩ�������趨����Щ�趨�������ؼ�������趨�͵�������趨��
- �ؼ��趨 @ref ssec_richtext_function_settings , @ref ssec_richtext_display_settings
- ���趨 @ref ssec_richtext_item
- ���ڸ�space���ο���ͼ
@image html richtext_space_settings.jpg

@warning richtext�����������޵ģ���������£�������һЩ�ضϣ�
- ���ֳ��ȳ�����󳤶� @ref GUIRICHTEXT_TEXT_BUF_MAX_LEN
- ���ֵ���ʾ�߶ȳ���һ�����ȣ�int16�����ֵ��ϵͳrect�����ƣ����������ڽ����ع���ȥ�������ơ�

@section sec_richtext_how_to_use ���ʹ��Richtext�ؼ���

ʹ��richtext�ؼ���Ҫ�ȴ����ÿؼ���Ȼ�������ʾ�����ʵ֮ǰ���������ø�����ʾ�������������������԰������²�����С�
@note ����ϸ����Ϣ����ο� @ref richtextGrp ��

@subsection ssec_richtext_create richtext�ؼ��Ĵ���������

��ƽ̨�������ؼ�һ����richtext�ؼ����Զ�̬������Ҳ���Ծ�̬�������ο��ؼ���һ����� @ref page_general ��

@subsection ssec_richtext_item ���ӡ�ɾ�����޸�item

richtext����ʾ������Ǹ�����(item)�����ԣ�ֻ�������richtext�������塣���ǿ������ӡ�ɾ�����޸��
-# @ref GUIRICHTEXT_AddItem
-# @ref GUIRICHTEXT_DeleteItem
-# @ref GUIRICHTEXT_ModifyItem
-# �����й���Ĳ�������ο� @ref richtextGrp ���ṩ�ĺ����б�

@note �ڲ������ͬʱ�����á������ʾ�趨���������ʾֻ���ڲ������ʱ��ı䡣�μ� @ref GUIRICHTEXT_ITEM_T ��

@subsection ssec_richtext_function_settings �����趨

��Ϊһ����ʾ�ؼ���richtext�����õĹ�����Խ��٣�Ŀǰֻ�У�
-# ���� @ref GUIRICHTEXT_SetFocusMode , @ref GUIRICHTEXT_DisableClipBoard
-# ���а� @ref GUIRICHTEXT_EnableClipBoard
-# @ref GUIRICHTEXT_SaveInfo
-# @ref GUIRICHTEXT_RestoreInfo

@subsection ssec_richtext_display_settings ��ʾ�趨

��ʾ��richtext����Ҫ���ܣ����Թ�����ʾ�趨��������Ƚ϶ࡣ
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

@note ������˵�ṩ����ô�����������Ǿ���Ҫ����ÿһ�ͨ������£�������һ����ԱȽϺ����Ĭ������ԣ�ֻ������Ҫ��ʱ�򣬲�ȥ�����ر����ʾ�

@sa @ref page_Url
@sa richtextGrp
*******************************************************************************/


/*! @addtogroup richtextGrp RichText
@brief Richtext�ؼ�ģ��
@details ��ģ����������
- richtext�����岿�֣�����������richtext�ؼ�
- richtext���ַ��ж����֣���url���֡��ⲿ����һ��ͨ�õ��ַ�����������ģ��

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
@brief �ؼ��ܽ��ܵ�����ļ�������
@details �ļ������ȵ��޶��������Ǳ���ģ�����Ϊ�˱���һЩ�޶˵Ĵ��󣬱��紫��һ���������ļ�������ƽ̨���ļ��������������Ƶģ��ʶ����ٴ˼���һ�����ơ��ļ����Ĵ洢�ؼ�����ʵ�������еĴ洢���õĶ�̬�����ڴ档
*/
#define GUIRICHTEXT_FILENAME_MAX_LEN (256)

/*! @def GUIRICHTEXT_TEXT_BUF_MAX_LEN
@brief �ؼ�һ��item�����ֵ���󳤶�
@details ��������Ǻ���Ҫ�ģ���Ϊһ���ؼ�������һЩ����ʶ����������һЩ���ƣ�richtext��Ϊһ������Ŀ��ʾ�Ŀؼ���ÿ����Ŀ��ʾ4k�����֣�Ӧ�����㹻�ˡ��ⲿ�ֵ����ƣ������ܱ�֤��̬�ڴ�����볬��ϵͳ�����ƣ���Ϊ����Ŀ��������û�����Ƶġ�
*/
#define GUIRICHTEXT_TEXT_BUF_MAX_LEN (10*1024)

/*! @def GUIRICHTEXT_TEXT_MAX_LEN
@brief Ϊ��ͨitem��������ֳ��ȣ�����Ϊ����Ӧ��
@details ��ֱ�׵Ľ������������Ϊ�˼��ݾɰ汾��richtext������һ�����ԡ������ļ��ݣ����Բ�����ᣬ������ʹ���������ֵ����Ϊһ���ض���Ӧ�ã�Ӧ�ð����Լ�����Ҫȥ����ռ�Ĵ�С��Ȼ����ݿؼ�֧�ֵ���󳤶ȣ���һ���Ƚ��жϣ�������ֱ�Ӵӿؼ���ȡ֧�ֵ����̶�ȥ���붯̬�ؼ����������Ǻ�Σ�գ�Ҳ��û�е���ġ��������һ�ε��ع��У����ǻ᳢��ȡ��������塣
*/
#define GUIRICHTEXT_TEXT_MAX_LEN (256)

/*! @def GUIRICHTEXT_PHONENUM_MAX_LEN
@brief phone number����󳤶�
*/
#define GUIRICHTEXT_PHONENUM_MAX_LEN (50)

/*! @def GUIRICHTEXT_EMAIL_MAX_LEN
@brief email����󳤶�
*/
#define GUIRICHTEXT_EMAIL_MAX_LEN (50)

/*! @def GUIRICHTEXT_URL_MAX_LEN
@brief url����󳤶�
*/
#define GUIRICHTEXT_URL_MAX_LEN GUIRICHTEXT_TEXT_MAX_LEN

/*! @def GUIRICHTEXT_SETTINGS_SPACE_LIMIT
@brief �����õ�������������������Ľ��ᱻ�Զ����ó������� 
@details ��Ϊһ����ʾ���������ʾԪ�ؼ�ļ����һ������Ҫ�Ĳ�������Ϊ���ֱ��Ӱ�쵽�û����Ӿ����顣�����ṩ�˷ḻ�ġ���������û��Զ��壬��������ʾ����ʶ���Ͽ���������Щ�������Ӧ�ò������Ƶġ��󡱣����ԣ������趨һ�������ƣ�����֤���ǵ���ʾ������һ��������״̬��
*/
#define GUIRICHTEXT_SETTINGS_SPACE_LIMIT (12)

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<����������*/
    MMI_IMAGE_ID_T bg_image_id; /*!< �ؼ��ı���ͼƬ */
} GUIRICHTEXT_INIT_DATA_T;


//1 ͼƬ�����ְ�������Դ����ö��
/*! @enum _GUIRICHTEXT_IMAGE_TYPE_E
@brief ͼƬ��ʽ������Դ����
@sa GUIRICHTEXT_IMAGE_TYPE_E
*/
enum _GUIRICHTEXT_IMAGE_TYPE_E
{
    GUIRICHTEXT_IMAGE_NONE, /*!< û��ͼƬ */
    GUIRICHTEXT_IMAGE_RES, /*!< ��ԴͼƬ */
    GUIRICHTEXT_IMAGE_BUF, /*!< �ڴ�ͼƬ */
    GUIRICHTEXT_IMAGE_FILE, /*!< ͼƬ�ļ� */

    GUIRICHTEXT_IMAGE_INVALID,
};

/*! @typedef GUIRICHTEXT_IMAGE_TYPE_E
@brief ͼƬ����
@sa _GUIRICHTEXT_IMAGE_TYPE_E
*/
typedef uint8 GUIRICHTEXT_IMAGE_TYPE_E;

//1.2 ����
/*! @enum _GUIRICHTEXT_TEXT_TYPE_E
@brief ���ָ�ʽ������Դ����
@sa GUIRICHTEXT_TEXT_TYPE_E
*/
enum _GUIRICHTEXT_TEXT_TYPE_E
{
    GUIRICHTEXT_TEXT_NONE, /*!< û������ */
    GUIRICHTEXT_TEXT_RES, /*!< ��Դ���� */
    GUIRICHTEXT_TEXT_BUF, /*!< �ڴ��ַ��� */

    GUIRICHTEXT_TEXT_INVALID,
};

/*! @typedef GUIRICHTEXT_TEXT_TYPE_E
@brief ��������
@sa _GUIRICHTEXT_TEXT_TYPE_E
*/
typedef uint8 GUIRICHTEXT_TEXT_TYPE_E;

//2 FRAME type, ��ʾ�߿򣬿�����չ�ɸ��ַ��߿�
/*! @enum _GUIRICHTEXT_FRAME_TYPE_E
@brief ��ʾ�߿�����
@note ���ﱾ���Ǵ������ö��ָ�ʽ�ı߿�ģ�����û��Ӧ������
@sa GUIRICHTEXT_FRAME_TYPE_E
*/
enum _GUIRICHTEXT_FRAME_TYPE_E
{
    GUIRICHTEXT_FRAME_NONE, /*!< û�б߿� */
    GUIRICHTEXT_FRAME_NORMAL, /*!< ��ͨ�߿� */

    GUIRICHTEXT_FRAME_MAX,
};

/*! @typedef GUIRICHTEXT_FRAME_TYPE_E
@brief �߿�����
@sa _GUIRICHTEXT_FRAME_TYPE_E
*/
typedef uint8 GUIRICHTEXT_FRAME_TYPE_E;


//3 align
/*! @enum _GUIRICHTEXT_ALIGN_E
@brief �������ͼƬ��λ��
@note �����ֵ�λ��Ϊ׼
@sa GUIRICHTEXT_ALIGN_E
@sa _GUIRICHTEXT_IMG_ALIGN_T
*/
enum _GUIRICHTEXT_ALIGN_E
{
    GUIRICHTEXT_ALIGN_DEFAULT, /*!< Ĭ��λ�ã�����ϵͳ�Ķ��뷽ʽ */
    GUIRICHTEXT_ALIGN_REVERSE,/*!< ��ϵͳ�Ķ��뷽ʽ�෴�� */
    GUIRICHTEXT_ALIGN_RIGHT, /*!< �������� */
    GUIRICHTEXT_ALIGN_DOWN, /*!< �������� */
    GUIRICHTEXT_ALIGN_LEFT, /*!< �������� */
    GUIRICHTEXT_ALIGN_UP, /*!< �������� */

    GUIRICHTEXT_ALIGN_INVALID,
};

/*! @typedef GUIRICHTEXT_ALIGN_E
@brief ͼƬ���������������
@sa _GUIRICHTEXT_ALIGN_E
*/
typedef uint8 GUIRICHTEXT_ALIGN_E;

/*! @enum _GUIRICHTEXT_IMG_ALIGN_T
@brief ͼƬ����ʾλ��
@sa GUIRICHTEXT_IMG_ALIGN_T
@sa _GUIRICHTEXT_ALIGN_E
@details ��Ϊ�������ǰ���ͼƬ�����ֵĸ߶���ȷ��item�ĸ߶ȵģ�����������ͼƬ��û�����ɵ�
*/
enum _GUIRICHTEXT_IMG_ALIGN_T
{
    GUIRICHTEXT_IMG_ALIGN_DEFAULT, /*!< Ĭ�Ͼ�����ʾ */
    GUIRICHTEXT_IMG_ALIGN_AUTO, /*!< ����ϵͳ�Ķ��뷽ʽ���� */
    GUIRICHTEXT_IMG_ALIGN_REVERSE, /*!< ����ϵͳ�Ķ��뷽ʽ�෴ */
    GUIRICHTEXT_IMG_ALIGN_MIDDLE, /*!< ���� */
    GUIRICHTEXT_IMG_ALIGN_LEFT, /*!< ���� */
    GUIRICHTEXT_IMG_ALIGN_RIGHT, /*!< ���� */
};

/*! @typedef GUIRICHTEXT_IMG_ALIGN_T
@brief ͼƬ��ʾ��������
@sa _GUIRICHTEXT_IMG_ALIGN_T
*/
typedef uint8 GUIRICHTEXT_IMG_ALIGN_T;


//4 ����
/*! @typedef GUIRICHTEXT_TAG_TYPE_E
@brief ����Ԫ�ص��趨����
@warning GUIRICHTEXT_TAG_HYPER��һ���򵥵ĳ��ı����ã�û�о���ʵ��Ӧ�á�
@sa GUIRICHTEXT_FOCUS_TYPE_T
@details ǰ16bits��text��tag����16bits��img��tag\n
���ڽ������͵��ر�˵��:
-# ͼƬ�����Ǽ����˵ģ����к���
-# ���ֽ���ĸ����Ϊ�������ͺͽ������
    - �������ͣ������ý�����Ϣ��ʱ��ָ���Ľ�����ʽ
    - ������ͣ������ջ�ȡ������Ϣʱ�õ��Ľ�����������
-# ���ֽ�������ٷ�Ϊ�������ͺ���չ����
    - �������ͣ�����Ϊ�������ͺͽ��������һ�µ�
    - ��չ���ͣ����������Ͷ�Ӧĳһ��������
    ����ֻ�л������Ϳ�����Ϊ�������
-# ��������
    - ��ʱ�Ľ����������ƶ��ģ������κν����������ձ���ߵ���ͼ�趨
    - GUIRICHTEXT_TAG_NONE - �޽���
    - GUIRICHTEXT_TAG_NORMAL - ���役��
    - GUIRICHTEXT_TAG_IMG_NORMAL - ͼƬ����
    - GUIRICHTEXT_TAG_TXT_NORMAL - ��������item���㣬ע����txt all�����������item
    - GUIRICHTEXT_TAG_TXT_ALL - �������ֽ��㣬��ע����txt all�����������text
    - GUIRICHTEXT_TAG_PHONENUM - ������Ϊ�绰����
    - GUIRICHTEXT_TAG_EMAIL - ������Ϊemail
    - GUIRICHTEXT_TAG_URL - ������Ϊurl
-# ��չ����
    - parse�࣬���ַ���������
    - hyper�࣬�����ı���Ϣ����
    - GUIRICHTEXT_TAG_PARSE_PHONENUM - GUIRICHTEXT_TAG_PHONENUM
    - GUIRICHTEXT_TAG_PARSE_EMAIL - GUIRICHTEXT_TAG_EMAIL
    - GUIRICHTEXT_TAG_PARSE_URL - GUIRICHTEXT_TAG_URL
    - GUIRICHTEXT_TAG_HYPER - GUIRICHTEXT_TAG_TXT_ALL��������
-# ������ͣ�������Ϊ�˷������ߵ�����
    - GUIRICHTEXT_TAG_PARSE - parse������
*/
typedef enum
{
    GUIRICHTEXT_TAG_NONE       = 0x00000000, /*!< �����ý��� */
    GUIRICHTEXT_TAG_NORMAL     = 0xFFFFFFFF, /*!< ����õ����㣬������Ϣ�� */

    //img
    GUIRICHTEXT_TAG_IMG_MASK   = 0xFFFF0000, /*!< ͼƬ����ģʽ��mask */
    GUIRICHTEXT_TAG_IMG_NORMAL = 0x00010000, /*!< IMG����õ����㣬������Ϣ�� */

    //text
    GUIRICHTEXT_TAG_TXT_MASK   = 0x0000FFFF, /*!< ���ֽ���ģʽ��mask */

    GUIRICHTEXT_TAG_TXT_NORMAL = 0x00000001, /*!< TEXT����õ����㣬������ϢΪȫ��text */
    GUIRICHTEXT_TAG_TXT_ALL    = 0x00000002, /*!< TEXTȫ���ַ��õ����㣬������ϢΪȫ��text */
    GUIRICHTEXT_TAG_PHONENUM   = 0x00000004, /*!< TEXTȫ���ַ��õ����㣬�ұ�����ph��������ϢΪȫ��text */
    GUIRICHTEXT_TAG_EMAIL      = 0x00000008, /*!< TEXTȫ���ַ��õ����㣬�ұ�����email��������ϢΪȫ��text */
    GUIRICHTEXT_TAG_URL        = 0x00000010, /*!< TEXTȫ���ַ��õ����㣬�ұ�����url��������ϢΪȫ��text */

    GUIRICHTEXT_TAG_PARSE_PHONENUM = 0x00000020, /*!< �õ�GUIRICHTEXT_TAG_PHONENUM */
    GUIRICHTEXT_TAG_PARSE_EMAIL    = 0x00000040, /*!< �õ�GUIRICHTEXT_TAG_EMAIL */
    GUIRICHTEXT_TAG_PARSE_URL      = 0x00000080, /*!< �õ�GUIRICHTEXT_TAG_URL */
    GUIRICHTEXT_TAG_HYPER      = 0x00000100, /*!< �������֣����п��� */

    GUIRICHTEXT_TAG_PARSE      = (GUIRICHTEXT_TAG_PARSE_PHONENUM | GUIRICHTEXT_TAG_PARSE_EMAIL | GUIRICHTEXT_TAG_PARSE_URL), /*!< �������֣����п��� */
} GUIRICHTEXT_TAG_TYPE_E;

/*! @enum _GUIRICHTEXT_FOCUS_SHIFT_E
@brief �����л�ģʽ
@sa GUIRICHTEXT_FOCUS_SHIFT_E
*/
enum _GUIRICHTEXT_FOCUS_SHIFT_E
{
    GUIRICHTEXT_FOCUS_SHIFT_NONE, /*!< ���������� */
    GUIRICHTEXT_FOCUS_SHIFT_BASIC, /*!< �������л����㣬�����л����޽���Ԫ�� */
    GUIRICHTEXT_FOCUS_SHIFT_ADVANCED, /*!< �������л����㣬ʼ���ڽ���֮���л� */

    GUIRICHTEXT_FOCUS_SHIFT_MAX,
};

/*! @typedef GUIRICHTEXT_FOCUS_SHIFT_E
@brief �����л�ģʽ����
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
*/
typedef uint8 GUIRICHTEXT_FOCUS_SHIFT_E;

/*! @struct GUIRICHTEXT_DATA_IMAGE_RES_T
@brief ͼƬ��Դ�ļ������ݽṹ
@sa GUIRICHTEXT_DATA_IMAGE_BUF_T
@sa GUIRICHTEXT_DATA_IMAGE_FILE_T
*/
typedef struct
{
    MMI_IMAGE_ID_T id; /*!< ��ԴͼƬ��image id */
} GUIRICHTEXT_DATA_IMAGE_RES_T;

/*! @struct GUIRICHTEXT_DATA_IMAGE_BUF_T
@brief �ڴ�ͼƬ�����ݽṹ
@note bmp buffer��ʽ������
@sa GUIRICHTEXT_DATA_IMAGE_RES_T
@sa GUIRICHTEXT_DATA_IMAGE_FILE_T
*/
typedef struct
{
    uint8 *buf_ptr; /*!< ͼƬ�ڴ�����ָ�� */
    uint32 size; /*!< ͼƬ�Ĵ�С */
    uint16 width; /*!< ͼƬ�Ŀ�����ָ��ͼƬ�Ŀ�ߣ��ſ��Խ���ͼƬ���� */
    uint16 height; /*!< ͼƬ�ĸ� */
} GUIRICHTEXT_DATA_IMAGE_BUF_T;

/*! @struct GUIRICHTEXT_DATA_IMAGE_FILE_T
@brief ͼƬ�ļ������ļ�ϵͳ�洢���ļ�
@sa GUIRICHTEXT_DATA_IMAGE_RES_T
@sa GUIRICHTEXT_DATA_IMAGE_BUF_T
*/
typedef struct
{
    wchar *filename_ptr; /*!< ͼƬ�ļ����ַ���ָ�� */
    uint16 filename_len; /*!< ͼƬ�ļ������� */
} GUIRICHTEXT_DATA_IMAGE_FILE_T;

/*! @union GUIRICHTEXT_DATA_IMAGE_U
@brief ͼƬ���ݵ����Ͻṹ
*/
typedef union
{
    GUIRICHTEXT_DATA_IMAGE_RES_T res; /*!< ��ԴͼƬ */
    GUIRICHTEXT_DATA_IMAGE_BUF_T buf; /*!< �ڴ�ͼƬ */
    GUIRICHTEXT_DATA_IMAGE_FILE_T file; /*!< �ļ�ͼƬ */
} GUIRICHTEXT_DATA_IMAGE_U;


/*! @struct GUIRICHTEXT_DATA_TEXT_RES_T
@brief ��Դ�ַ��������ݽṹ
@sa GUIRICHTEXT_DATA_TEXT_BUF_T
*/
typedef struct
{
    MMI_TEXT_ID_T id; /*!< ��ʾ��Դ���ֵ�text id */
} GUIRICHTEXT_DATA_TEXT_RES_T;

/*! @struct GUIRICHTEXT_DATA_TEXT_BUF_T
@brief �ַ������ݽṹ
@sa GUIRICHTEXT_DATA_TEXT_RES_T
*/
typedef struct
{
    wchar *str_ptr; /*!< ��ʾ���ַ���ָ�� */
    uint16 len; /*!< ��ʾ���ַ������� */
} GUIRICHTEXT_DATA_TEXT_BUF_T;

/*! @union GUIRICHTEXT_DATA_TEXT_U
@brief �������ݵ����Ͻṹ
*/
typedef union
{
    GUIRICHTEXT_DATA_TEXT_RES_T res; /*!< ��Դ���� */
    GUIRICHTEXT_DATA_TEXT_BUF_T buf; /*!< �ڴ��е��ַ��� */
} GUIRICHTEXT_DATA_TEXT_U;


/*! @struct GUIRICHTEXT_ITEM_T
@brief item�Ľṹ����
@note ���ֺ�ͼƬ�������趨��Ҫͬʱ����set��setting������ȷ��ʾ�����Լ������趨��ֵ����δ��ʽ�趨������£������趨ֵ
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
@details Ӧ��������һ��item��ʱ�򣬿������õ�item����
- ͼƬ����
    -# ͼƬ������
    -# ͼƬ������
    -# ͼƬ�ı߿�
    -# ͼƬ�Ŀ���������
        -# ����
        -# ��ʾ��ʱ������ʾ�����ڵĶ��뷽ʽ
- ��������
    -# �ı�������
    -# �ı�������
    -# �ı��ı߿�
    -# �ı��Ŀ���������
        -# ����Ĵ�С����ɫ
        -# ��������ʾ�����ڵĶ��뷽ʽ
        -# ���ֵı���ɫ
- item����
    -# tag_type ��������
    -# align_type ͼƬ�����ֵ����λ������
    -# frame_type item�ı߿�
*/
typedef struct
{
    GUIRICHTEXT_IMAGE_TYPE_E img_type; /*!< ͼƬ���� */
    GUIRICHTEXT_DATA_IMAGE_U img_data; /*!< ͼƬ���� */
    GUIRICHTEXT_FRAME_TYPE_E img_frame_type; /*!< ͼƬ�߿� */
    BOOLEAN is_bit_map;/*!< �Ƿ�Bitmap */
    BOOLEAN img_set_size; /*!< �Ƿ�ָ��ͼƬ��С */
    uint16 img_setting_width; /*!< ָ��ͼƬ�� */
    uint16 img_setting_height; /*!< ָ��ͼƬ�� */
    BOOLEAN img_set_align; /*!< �Ƿ��ƶ�ͼƬ���뷽ʽ */
    GUIRICHTEXT_IMG_ALIGN_T img_setting_align; /*!< ָ��ͼƬ���뷽ʽ */

    GUIRICHTEXT_TEXT_TYPE_E text_type; /*!< �������� */
    GUIRICHTEXT_DATA_TEXT_U text_data; /*!< �������� */
    GUIRICHTEXT_FRAME_TYPE_E text_frame_type; /*!< ���ֱ߿� */
    BOOLEAN text_set_font; /*!< �Ƿ�ָ������ */
    GUI_FONT_T setting_font; /*!< ָ���������� */
    BOOLEAN text_set_font_color; /*!< �Ƿ��趨������ɫ */
    GUI_COLOR_T setting_font_color; /*!< ָ����������ɫ */
    BOOLEAN text_set_align; /*!< �Ƿ�ָ�����ֶ��뷽ʽ */
    GUI_ALIGN_E setting_align; /*!< ָ�������ֶ��뷽ʽ */
    BOOLEAN text_set_bg; /*!< �Ƿ��趨���ֱ���ɫ */
    GUI_COLOR_T setting_bg_color; /*!< ָ�������ֱ���ɫ */

    GUIRICHTEXT_TAG_TYPE_E tag_type; /*!< ���㷽ʽ�趨 */
    GUIRICHTEXT_ALIGN_E align_type; /*!< ���ֺ�ͼƬ�����λ���趨 */
    GUIRICHTEXT_FRAME_TYPE_E frame_type; /*!< ������Ŀ�ı߿��趨 */
} GUIRICHTEXT_ITEM_T;


//8 Ԫ��-�ӽ���ȡ��������
/*! @struct GUIRICHTEXT_ELEMENT_DATA_T
@brief ����Ԫ������
*/
typedef struct
{
    wchar *str_ptr; /*!< �����ַ���ָ�� */
    uint16 len; /*!< �����ַ��ĳ��� */
} GUIRICHTEXT_ELEMENT_DATA_T;

/*! @typedef GUIRICHTEXT_FOCUS_TYPE_T
@brief ����Ԫ�ص�����
@note ��ΪGUIRICHTEXT_FOCUS_URLʱ����ҪӦ�������ж�Э��ͷ������http����rtsp�ȡ�
@sa GUIRICHTEXT_TAG_TYPE_E
@details Ϊ��ʹ�ý����Ĺ��̺ͽ���ʶ��Ľ�����������֣����Ƕ��役������
һ���������Ϳ��Զ�Ӧһ����������ʶ������\n
��ʵ�ϣ��ڿռ��ڲ�����ʹ�õ�ʶ��������ͣ�ֻ�Ƕ��⣬ʹ�ý�������
*/
typedef enum
{
    GUIRICHTEXT_FOCUS_NONE = GUIRICHTEXT_TAG_NONE, /*!< �ǽ��� */
    GUIRICHTEXT_FOCUS_NORMAL = GUIRICHTEXT_TAG_NORMAL, /*!< ����item */
    GUIRICHTEXT_FOCUS_IMG_NORMAL = GUIRICHTEXT_TAG_NORMAL, /*!< ����img���� */
    GUIRICHTEXT_FOCUS_TXT_NORMAL = GUIRICHTEXT_TAG_TXT_NORMAL, /*!< ����text���֣�ȫ�� */
    GUIRICHTEXT_FOCUS_TXT_ALL = GUIRICHTEXT_TAG_TXT_ALL, /*!< ����text���֣����ַ� */
    GUIRICHTEXT_FOCUS_PHONENUM = GUIRICHTEXT_TAG_PHONENUM, /*!< �绰���� */
    GUIRICHTEXT_FOCUS_EMAIL = GUIRICHTEXT_TAG_EMAIL, /*!< email */
    GUIRICHTEXT_FOCUS_URL = GUIRICHTEXT_TAG_URL, /*!< url */
} GUIRICHTEXT_FOCUS_TYPE_T;

/*! @struct GUIRICHTEXT_FOCUS_ELEM_T
@brief Ԫ����������
*/
typedef struct
{
    GUIRICHTEXT_FOCUS_TYPE_T type; /*!< �������� */
    GUIRICHTEXT_ELEMENT_DATA_T data; /*!< ����Ԫ�ص����� */
} GUIRICHTEXT_FOCUS_ELEM_T;
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/***************************************************************************//*!
@brief GUIRICHTEXT_AddItem
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param item_ptr [in] item����
@param index_ptr [in/out]���ص�ǰitem��index���ɸ��ݸ�id����item
@return �Ƿ����ӳɹ� 
@note ������һ��ͼƬitem, ����ͼƬ��buf����ʱ, GUIRICHTEXT_IMAGE_T�ṹ�е�buf_ptr���ɴ��������ڴ��, �ؼ�������������ͷ�
@note �������͵����ã���ο�GUIRICHTEXT_TAG_TYPE_E���崦����ϸ˵��
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
@param ctrl_id [in] �ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �����Ƿ�ɹ�
@note delete֮�������Ҫˢ����ʾ�������GUIRICHTEXT_Update
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
@param ctrl_id [in] �ؼ�id
@param item_index [in] item��index����add item��ʱ����Եõ�
@param item_ptr [in] item����
@return �����Ƿ�ɹ�
@note modify֮�������Ҫˢ����ʾ�������GUIRICHTEXT_Update
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
@brief ɾ�����е�item
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_DeleteAllItems(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief ��ʾ�ؼ�
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note ��ˢ��֮ǰ������¿ؼ�������
@sa GUIRICHTEXT_UpdateData
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_Update(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief �����¿ؼ������ݣ�����ˢ��
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note �кܶ�������ͨ������õ��ģ�������ֻ������Ҫ��ʱ��Ž��У�ͨ��������ʾ��ʱ
�������Ҫ��ʱ����ؼ������ݣ�����Ҫͨ��������������ɡ�
@sa GUIRICHTEXT_Update
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_UpdateData(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief ��ȡָ��index��item
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@param item_ptr [in]item����ָ��
@return �����Ƿ�ɹ�
@note ��item�����ݲ���ԭʼ���ݣ����ܻ���Ϊ��ʾ��Ҫ���޸Ĺ�������hyper parse����
��ʾ���ַ�����ʽ���ˣ�ͬ��text id��ʽ������Ҳ���������buffer��ʽ��
@sa GUIRICHTEXT_AddItem
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index,
    GUIRICHTEXT_ITEM_T *item_ptr
);

/***************************************************************************//*!
@brief ��ȡ����Ԫ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param element [in]�������ݵ�ָ��
@return �����Ƿ�ɹ�
@note 
- ����ǰ��������Ԫ�ص���Ϣ
- �������ͣ���ο�GUIRICHTEXT_TAG_TYPE_E���崦����ϸ˵��
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusElement(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_ELEM_T *element
);

/***************************************************************************//*!
@brief ��ȡ����Item��������
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param index_ptr [in]������ָ��
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 *index_ptr
);

/***************************************************************************//*!
@brief ���������ؼ���Ĭ������
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param font [in]����
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFont(
    MMI_CTRL_ID_T ctrl_id,
    GUI_FONT_T font
);

/***************************************************************************//*!
@brief ���������ؼ���Ĭ��������ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFontColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ������ʾ����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param rect [in]��ʾ����
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetRect(
    MMI_CTRL_ID_T ctrl_id,
    GUI_RECT_T  rect
);

/***************************************************************************//*!
@brief �����ּ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�ּ��
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetCharSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief �����ı����м��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�м��
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetLineSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief ����ͼƬ���ı��ľ���
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�ı���ͼƬ֮��ľ���
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgTxtSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief ����item֮��ļ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item֮��ļ��
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief ���ñ߽�࣬��item�Ϳؼ���Ե�ľ���
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item�Ϳؼ���Ե�ľ���
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief ���ñ߽�࣬item���ܵ�����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item���ܵ�����
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
);

/***************************************************************************//*!
@brief �������ֵ��߿�Ŀհ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param margin [in]�������ֵ��߿�Ŀհ�
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetTxtMargin(
    MMI_CTRL_ID_T ctrl_id,
    uint8 margin
);

/***************************************************************************//*!
@brief �����л�����ķ�ʽ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param focus_mode [in]����ģʽ
@return �����Ƿ�ɹ�
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusMode(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_SHIFT_E focus_mode
);

/***************************************************************************//*!
@brief ���ý����ַ����»�����ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in] ��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusLineColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ���ý�������ѡ��ʱ�ı���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ���ÿؼ��ı���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param color [in] ��ɫ
@return �����Ƿ�ɹ�
@note ��û�б���ͼƬ���߱���ͼƬʧЧ��ʱ�򣬱���ɫ������
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ���ÿؼ��ı���ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param bg_img_id [in]����ͼƬid
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgImage(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T bg_img_id
);

/***************************************************************************//*!
@brief ���ñ߿�,Ĭ��ΪGUIRICHTEXT_FRAME_NONE
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param border_type [in]�߿�����
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorder(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FRAME_TYPE_E border_type
);

/***************************************************************************//*!
@brief ���ñ߿����ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ���ý���Ԫ�صı߿���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
@note ����item����ʾ�߿��ʱ�򣬻��в�ͬ����ɫ
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderFocusColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
);

/***************************************************************************//*!
@brief ������ͼƬ������������ʾ��ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param img_id [in]ͼƬ��id
@return �����Ƿ�ɹ�
@note ��ЩͼƬ����ʾ��Ҫ�ȴ����ڵȴ������У�����ʾһ���ȴ�ͼƬ
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimParsingIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
);

/***************************************************************************//*!
@brief ������ͼƬ��������ʱ��ʾ��ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param img_id [in]ͼƬ��id
@return �����Ƿ�ɹ�
@note ��ͼƬ������ʾ��ʱ����ʾָ��ͼƬ
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimErrorIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
);

/***************************************************************************//*!
@brief ָ������item
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �����Ƿ�ɹ�
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
);

/***************************************************************************//*!
@brief �趨ͼƬ�����Ƿ�ͬ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param is_img_syn_decode [in]�Ƿ�ͬ������
@return �����Ƿ�ɹ�
@note ��һЩ����ʾ�ٶ���Ҫ���Ӧ�ã�������첽���롣
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgDecodeSyn(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_img_syn_decode
);

/***************************************************************************//*!
@brief ���³�ʼ���ؼ�����ʾ����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@note �������������ÿؼ���Ŀ����Ҫ����״̬�����
*******************************************************************************/
PUBLIC void CTRLRICHTEXT_ResetState(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief ������ʾ������Ԫ������ҳ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
*******************************************************************************/
PUBLIC void CTRLRICHTEXT_ScrollToFocus(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
@brief ������ʾָ��item����ҳ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �Ƿ�����ɹ�
@note �¼����item������update֮�������ȷ�Ĺ�����ʾ����Ϊ��update��ʱ�����rect
update�����ĵ�һ��ˢ�»��Զ�����Ҳ���Ե��ú���GUIRICHTEXT_Update�����
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_ScrollToItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
);

/***************************************************************************//*!
@brief ���ü����幦��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ����óɹ�
@sa GUIRICHTEXT_DisableClipBoard
@note Ĭ�ϼ��а��ǿ�����
-# ѡ�֣�ѡ�������漸�ַ�ʽ��
    -# ͨ�������϶�ѡ��
    -# �������ʣ������˵�ȫѡ
-# ���ƣ��ɸ��Ƶ����������漸�֣�
    -# ѡ��������
    -# ��������
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_EnableClipBoard(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief ���ü����幦��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ����óɹ�
@sa GUIRICHTEXT_EnableClipBoard
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_DisableClipBoard(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief ����ؼ�����ʾ�м���Ϣ���������а塢����λ�õ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ񱣴�ɹ�
@sa GUIRICHTEXT_RestoreInfo
@warning һ��Ҫ����GUIRICHTEXT_RestoreInfo���ָ�������������ʾ����
@warning ������Ϣ���޷����棬��Ϊ������ϢҪ������(item)��Ϣ����item����Ϣ��ȫ��
��ʧ��item����Ϣ�ֱȽϸ��ӣ��������ܱ�֤�ڲ���֮��ԭ����item�����ڣ��ʶ����޷�
���档
@details һЩӦ�û���Ϊ��Ӧ�õ���Ҫ���Կؼ����дݻ��ؽ��������ؽ��Ĺ����У���
����һЩ��Ϣ�Ķ�ʧ��������а���Ϣ������λ����Ϣ�ȡ���ʱ�����ǾͿ��Բ��ö�����
��һЩ��Ϣ���б�����
*******************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_SaveInfo(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief �ָ��ؼ�����ʾ�м���Ϣ���������а塢����λ�õ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ�ָ��ɹ�
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
