/*****************************************************************************
** File Name:      ctrlim_base.h                                             *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLIM_BASE_H_
#define _CTRLIM_BASE_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guifont.h"
#include "guilcd.h"

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
/*! @enum _GUIIM_BASE_CONSTANT
@brief ��������
*/
enum _GUIIM_BASE_CONSTANT
{
    GUIIM_COMMIT_BUFFER_MAX = 64, /*!< �ύ�ַ����ڴ��С */
    GUIIM_CUSTOM_CHAR_LIMIT = 18, /*!< ��CUSTOM���뷨�У�ÿ�������������ɵĿ�ѡ�ַ��� */
    GUIIM_NOTIFY_QUEUE_LEN = 3, /*!< notify��Ϣ���еĳ��ȣ�ͨ�����ֵ����ܴ������һ����Ϣ������ķ��ش��� */
    GUIIM_TEXT_CFG_MAX_LEN = 6
};

/*! @enum _GUIIM_CUSTOM_KEY_T
@brief ���������뷨�����ü�ֵ�б�
@warning 
- Ϊʲô����������? 
    - ԭ���ϣ�����ļ���Ķ��壬�û��ǲ��ɼ��ġ���Ϊcustom��һ�������������������ر��֪����
- �û�ʲôʱ����õ���Щ����?
    - ���û���Ҫ����multitap��ĳ����ĳЩ��ֵ��ʱ���û�����ͨ���ӿ����趨��
@sa GUIIM_CUSTOM_KEY_T
*/
enum _GUIIM_CUSTOM_KEY_T
{
    GUIIM_CUSTOM_KEY_CHR_A,
    GUIIM_CUSTOM_KEY_CHR_B,
    GUIIM_CUSTOM_KEY_CHR_C,
    GUIIM_CUSTOM_KEY_CHR_D,
    GUIIM_CUSTOM_KEY_CHR_E,
    GUIIM_CUSTOM_KEY_CHR_F,
    GUIIM_CUSTOM_KEY_CHR_G,
    GUIIM_CUSTOM_KEY_CHR_H,
    GUIIM_CUSTOM_KEY_CHR_I,
    GUIIM_CUSTOM_KEY_CHR_J,
    GUIIM_CUSTOM_KEY_CHR_K,
    GUIIM_CUSTOM_KEY_CHR_L,
    GUIIM_CUSTOM_KEY_CHR_M,
    GUIIM_CUSTOM_KEY_CHR_N,
    GUIIM_CUSTOM_KEY_CHR_O,
    GUIIM_CUSTOM_KEY_CHR_P,
    GUIIM_CUSTOM_KEY_CHR_Q,
    GUIIM_CUSTOM_KEY_CHR_R,
    GUIIM_CUSTOM_KEY_CHR_S,
    GUIIM_CUSTOM_KEY_CHR_T,
    GUIIM_CUSTOM_KEY_CHR_U,
    GUIIM_CUSTOM_KEY_CHR_V,
    GUIIM_CUSTOM_KEY_CHR_W,
    GUIIM_CUSTOM_KEY_CHR_X,
    GUIIM_CUSTOM_KEY_CHR_Y,
    GUIIM_CUSTOM_KEY_CHR_Z,

    GUIIM_CUSTOM_KEY_DEL,
    GUIIM_CUSTOM_KEY_COMMA,
    GUIIM_CUSTOM_KEY_PERIOD,
    GUIIM_CUSTOM_KEY_ENTER,
    GUIIM_CUSTOM_KEY_FN,
    GUIIM_CUSTOM_KEY_SHIFT,
    GUIIM_CUSTOM_KEY_AT_QWERTY,
    GUIIM_CUSTOM_KEY_SPACE,
    GUIIM_CUSTOM_KEY_AND,
    GUIIM_CUSTOM_KEY_QUESTION,
    GUIIM_CUSTOM_KEY_CTRL,

    GUIIM_CUSTOM_KEY_PLUS,                  // +
    GUIIM_CUSTOM_KEY_LEFT_PARENTHESIS,      // (
    GUIIM_CUSTOM_KEY_RIGHT_PARENTHESIS,     // )
    GUIIM_CUSTOM_KEY_MINUS,                 // -
    GUIIM_CUSTOM_KEY_DOUBLE_QUOTES,         // "
    GUIIM_CUSTOM_KEY_SEMICOLON,             // ;
    GUIIM_CUSTOM_KEY_COLON,                 // :
    GUIIM_CUSTOM_KEY_SLASH,                 // /
    GUIIM_CUSTOM_KEY_SHIFT_AND,             // &
    GUIIM_CUSTOM_KEY_EXCLAMATION,           // !

    GUIIM_CUSTOM_KEY_0,
    GUIIM_CUSTOM_KEY_1,
    GUIIM_CUSTOM_KEY_2,
    GUIIM_CUSTOM_KEY_3,
    GUIIM_CUSTOM_KEY_4,
    GUIIM_CUSTOM_KEY_5,
    GUIIM_CUSTOM_KEY_6,
    GUIIM_CUSTOM_KEY_7,
    GUIIM_CUSTOM_KEY_8,
    GUIIM_CUSTOM_KEY_9,
    GUIIM_CUSTOM_KEY_STAR,
    GUIIM_CUSTOM_KEY_HASH,

    GUIIM_CUSTOM_KEY_MAX,
};

/*! @typedef GUIIM_CUSTOM_KEY_T
@brief ���������뷨�����ü�ֵ����
@sa _GUIIM_CUSTOM_KEY_T
*/
typedef int32 GUIIM_CUSTOM_KEY_T;


/*! @typedef GUIIM_METHOD_T
@brief ���뷨���Ͷ���
*/
typedef uint16 GUIIM_METHOD_T;

/*! @typedef GUIIM_LANGUAGE_T
@brief �������Զ���
*/
typedef uint8 GUIIM_LANGUAGE_T;

#define MAKE_HIGHLIGHT_PARAM(start, end)    ((start<<16)|(0xffff&end))
#define SET_HIGHLIGHT_START(v)              (((uint32)v)>>16)
#define SET_HIGHLIGHT_END(v)                ((uint16)v&0xffff)


/*! @enum _GUIIM_INPUT_TYPE_T
@brief ���뷽ʽ�������̡���д�ʹ��ʡ�
@note ��ͬ�����뷨���ڲ�ͬ�����뷽ʽ�£���Ҫ�������ں��ǲ�һ���ġ�
@sa GUIIM_INPUT_TYPE_T
*/
enum _GUIIM_INPUT_TYPE_T
{
    GUIIM_INPUT_TYPE_NONE        = 0x00, /*!< ���������� */
    GUIIM_INPUT_TYPE_KEYBOARD    = 1 << 0, /*!< ��������(�ż�) */
    GUIIM_INPUT_TYPE_TOUCHPEN    = 1 << 1, /*!< ��������(ģ��ż�) */
    GUIIM_INPUT_TYPE_QWERT       = 1 << 2, /*!< ȫ�������� */
    GUIIM_INPUT_TYPE_TP_QWERT    = 1 << 3, /*!< ȫ���̴��� */
    GUIIM_INPUT_TYPE_HANDWRITING = 1 << 4, /*!< ��д���� */
    
    GUIIM_INPUT_TYPE_ALL = ((1 << 5) - 1) /*!< ȫ���������͵ļ��� */
};

/*! @typedef GUIIM_INPUT_TYPE_T
@brief ���뷽ʽ����
@sa _GUIIM_INPUT_TYPE_T
*/
typedef uint8 GUIIM_INPUT_TYPE_T;


/*! @enum _GUIIM_CAPITAL_MODE_T
@brief ��Сдģʽ������д��Сд������ĸ��д��
@note ��������󲿷����뷨���ǲ���Ҫ�ģ���ʹ���������뷨�õ�����Ҳ��Ҫ����Ĵ���
- ���ԣ���������鲻���о������Ǻܺ���
- ������һ�����ԣ�С�����ԡ�
- Ŀǰ���Ǳ�����һ��������ԡ�
@sa GUIIM_CAPITAL_MODE_T
*/
enum _GUIIM_CAPITAL_MODE_T
{
    GUIIM_MODE_LEAD_UPPER, /*!< ����ĸ��д */
    GUIIM_MODE_LOWER, /*!< Сд */
    GUIIM_MODE_UPPER, /*!< ��д */

    GUIIM_MODE_MAX,
};

/*! @typedef GUIIM_CAPITAL_MODE_T
@brief ��Сдģʽ������
@sa _GUIIM_CAPITAL_MODE_T
*/
typedef uint8 GUIIM_CAPITAL_MODE_T;

/*! @enum _GUIIM_TYPE_T
@brief ���뷽������
@details һ�����뷨��ȷ�����ĸ�����
-# ����
-# ���뷨
-# ���뷽ʽ
-# ��Сдģʽ
@note - ������Ŀǰʹ�õķ�ʽ������Ҳ�ǳ��õķ�ʽ�������ǲ��Ǻ�����ϰ�߾�������
- �������룬��ͬ�����������smart�ǲ�ͬ�ģ����ľ���ƴ��
@note - ͨ���������������Ƶ����뷨��ʱ�򣬻���english��abc����ʾӢ�ģ�����ʵ�ʵ�
Ӧ���У����ǻ���multitap&smart����english���趨Ӣ�ĵ���ĸ����������
@sa GUIIM_TYPE_T
*/
enum _GUIIM_TYPE_T
{
    GUIIM_TYPE_NONE     = 0x00, /*!< �����뷨���� */
    GUIIM_TYPE_SMART    = 1 << 0, /*!< �������� */
    GUIIM_TYPE_STROKE   = 1 << 1, /*!< �ʻ����� */
    GUIIM_TYPE_MULTITAP = 1 << 2, /*!< ��ΰ������� */
    GUIIM_TYPE_DIGITAL  = 1 << 3, /*!< �������� */
    GUIIM_TYPE_ABC      = 1 << 4, /*!< Ӣ��abc���� */
    GUIIM_TYPE_ENGLISH  = 1 << 5, /*!< ����Ӣ������ */
    GUIIM_TYPE_SYS      = 1 << 6, /*!< ϵͳ���� */
    GUIIM_TYPE_COUNT    = 7,
};

/*! @typedef GUIIM_TYPE_T
@brief ���뷽�����Ͷ���
@note ����������û����ܿ�����һ���������
@sa _GUIIM_TYPE_T
*/
typedef uint8 GUIIM_TYPE_T;



/*! @enum _GUIIM_LIMIT_T
@brief ��������
@note ͨ���������ƣ�����ֹ�л���ĳЩ���뷨
@sa GUIIM_LIMIT_T
*/
enum _GUIIM_LIMIT_T
{
    GUIIM_LIMIT_LOCAL = 1, /*!< ��ֹ������������ */
    GUIIM_LIMIT_ENGLISH = 1 << 1, /*!< ��ֹӢ������ */
    GUIIM_LIMIT_DIGITAL = 1 << 2, /*!< ��ֹ�������� */
    GUIIM_LIMIT_SYMBOL = 1 << 3, /*!< ��ֹ�������� */
};

/*! @typedef GUIIM_LIMIT_T
@brief ������������
@sa _GUIIM_LIMIT_T
*/
typedef uint16 GUIIM_LIMIT_T;


/*! @enum _GUIIM_TAG_T
@brief ���뷨�ĸ��������趨���������ö����������
*/
enum _GUIIM_TAG_T
{
    GUIIM_TAG_NONE, /*!< Ĭ�����޸������Ե� */
    GUIIM_TAG_CLASSIC_NUM, /*!< ���䣹������0-9*# */
    GUIIM_TAG_PURE_NUM, /*!< ���������� */
    GUIIM_TAG_DOT_NUM, /*!< ���ֺ�С�������� */
    GUIIM_TAG_PHONE_NUM, /*!< �绰�������� */
    GUIIM_TAG_PHONE_NUM_MIN, /*!< �����Ƶĵ绰�������룬��+������ */
	GUIIM_TAG_NUM_WITH_SWITCHING, /*!< ֧���л����������룬���л����ַ�����ͷ������� */
    GUIIM_TAG_EMAIL, /*!< Email���� */
    GUIIM_TAG_URL, /*!< ��ַ���� */
    GUIIM_TAG_PB_HUOPIN, /*!< �绰����ƴ���Ƽ��� */
};

/*! @typedef GUIIM_TAG_T
@brief ���뷨�ĸ������Զ���
*/
typedef uint32 GUIIM_TAG_T;


/*! @struct GUIIM_CUSTOM_KEY_PARAM_T
@brief ���ư�����ʱ��Ĵ��ݲ����ṹ
@sa guiim_custom_key_param_tag
*/
typedef struct guiim_custom_key_param_tag GUIIM_CUSTOM_KEY_PARAM_T;

/*! @struct guiim_custom_key_param_tag
@brief ���ư�����ʱ��Ĵ��ݲ����ṹ
@sa GUIIM_CUSTOM_KEY_PARAM_T
*/
struct guiim_custom_key_param_tag
{
    GUIIM_TYPE_T im_type; /*!< ���Ƶ����뷨 */
    GUIIM_CUSTOM_KEY_T key; /*!< ���Ƶİ��� */
    wchar key_values[GUIIM_CUSTOM_CHAR_LIMIT+1]; /*!< �����ϵ��ַ��б� */
    uint32 key_count; /*!< �����ϵ��ַ����� */
    GUIIM_CUSTOM_KEY_PARAM_T *next_ptr; /*!< ���������һ���ڵ� */
};


/*! @struct GUIIM_METHOD_SET_T
@brief ���뷨�����ṹ
@details ���뷨�������ã����������������һ�����뷨�ĸ��������ġ������ɶ������������һ�����뷨��
\arg method ���ֵ�϶���Ҫ�еģ�����Ψһ������
\arg lang ��������ã����е����Զ����ã�Ĭ��GUIIM_LANG_NONE
\arg type ��������ã��������е��������ͣ�Ĭ��GUIIM_TYPE_NONE
\arg input ��������ã�֧�����е����뷽ʽ��Ĭ��GUIIM_INPUT_TYPE_NONE
\arg capital ��������ã�˵�����벻���ִ�Сд��Ĭ��GUIIM_MODE_MAX
*/
typedef struct
{
    GUIIM_METHOD_T method; /*!< ���뷨 */
    GUIIM_LANGUAGE_T lang; /*!< �������� */
    GUIIM_TYPE_T type; /*!< �������� */
    GUIIM_INPUT_TYPE_T input; /*!< ���뷽ʽ */
    GUIIM_CAPITAL_MODE_T capital; /*!< ��Сдģʽ */
} GUIIM_METHOD_SET_T;

/*! @typedef GUIIM_METHOD_CFG_T
@brief ����Ϊ���������ýṹ����
*/
typedef GUIIM_METHOD_SET_T const GUIIM_METHOD_CFG_T;



/*! @struct GUIIM_METHOD_PARAM_T
@brief ���뷨����ز�������
@details ����һ�����뷨��صĸ���ϸ�Ĳ�����ϡ�
@note 
- input_lang
    - ��ǰʵ������������
    - ͨ������»���Ĭ�ϵ��������ԣ���GUIIM_LANG_NONE - ȥϵͳ���ԣ���LOCAL
    - Ĭ����������Ե��л��ᱻ����
    - ��Ĭ������£����������Ե��л�ѡ�� - ����������ΪENGLISH

- ȫ����صĲ���
\arg input_set ֧�ֵ����뷽������
\arg type_set ֧�ֵ����뷨���ͼ���
\arg limit �������ƣ������ͨ����������������������ս��

- Ŀǰ��limit�����ã��Ǹ����������ƹ����ظ��ģ���һ���ۺ���������
*/
typedef struct
{
    GUIIM_METHOD_SET_T im_set; /*!< ���뷨�趨 */
    GUIIM_METHOD_CFG_T *im_def_ptr; /*!< ���뷨�������ݵ�ַ */

    GUIIM_LANGUAGE_T input_lang; /*!< �������� */
    
    GUIIM_INPUT_TYPE_T input_set; /*!< ���뷽ʽ���� */
    GUIIM_TYPE_T type_set; /*!< �������ͼ��� */
    
    GUIIM_LIMIT_T limit; /*!< �������� */
    GUIIM_TAG_T tag; /*!< ���뷨�ĸ������� */
}GUIIM_METHOD_PARAM_T;


/*! @struct GUIIM_CONTEXT_T
@brief ��ȡ��������Ϣ�Ľṹ
*/
typedef struct
{
    wchar* str_ptr; /*!< ��ǰ�༭������ָ�� */
    uint32 str_len; /*!< ���ֳ��� */
    uint32 pos; /*!< ��ǰ�༭������ */
    
    BOOLEAN is_underline;   /*!highlight or not*/
    uint16 start_pos;      /*!highlight start pos*/
    uint16 end_pos;        /*!highlight end pos*/
    
} GUIIM_CONTEXT_T;

/*! @typedef GUIIM_GetContextFunction_T
@brief ��ȡ��������Ϣ�ĺ�������
@warning - �ؼ��򸸿ؼ���ȡ���ݵĻص��������塣
- ���뷨���ϲ�Ľ�������һЩӦ���л�����ϸ���ơ�
- ǰ���Ǳ༭�ؼ������뷨Ӧ��ͬʱ֧�ֲſ��ԡ�
@sa GUIIM_GetCapacityFunction_T
*/
typedef void (*GUIIM_GetContextFunction_T)(MMI_HANDLE_T, GUIIM_CONTEXT_T *);

/*! @struct GUIIM_CAPACITY_T
@brief ��ȡ������Ϣ
*/
typedef struct
{
    uint32 max_len; /*!< �����������󳤶� */
    uint32 cur_len; /*!< ��ǰ���볤�� */
} GUIIM_CAPACITY_T;

/*! @typedef GUIIM_GetCapacityFunction_T
@brief ��ȡ������������Ϣ�ĺ�������
@sa GUIIM_GetContextFunction_T
*/
typedef void (*GUIIM_GetCapacityFunction_T)(MMI_HANDLE_T, GUIIM_CAPACITY_T *);

/*! @struct GUIIM_INIT_KERNEL_T
@brief �ؼ������뷨Ӧ�õĳ�ʼ������
@note ��ʼ������Ӧ���Ǿ����ٵġ�����ͳ�ʼ�����뷨�ؼ��Ĳ����ǲ�ͬ�ġ����û����ɼ���
-# ������ʾapp��
-# ���ڳ�ʼ�������뷨��
-# �ر���Ҫ�Ĳ���
*/
typedef struct
{
    CAF_HANDLE_T im_handle; /*!< ���뷨�ؼ�handle */
    MMI_HANDLE_T win_handle; /*!< ����handle */
    MMI_WIN_ID_T win_id; /*!< ����id */

    GUI_LCD_DEV_INFO *lcd_info_ptr; /*!< ��ǰ��ʾ�豸��Ϣ */

    GUIIM_METHOD_PARAM_T *method_param_ptr; /*!< ���뷨���ò��� */

    GUIIM_GetContextFunction_T f_get_context; /*!< ��ȡ�����ĺ��� */
    GUIIM_GetCapacityFunction_T f_get_capacity; /*!< ��ȡ������Ϣ���� */
} GUIIM_INIT_KERNEL_T;


/*
*/
/*! @enum _GUIIM_EVENT_T
@brief �ؼ��¼�����
@details ���ؼ���Ҫ��Ӧ�÷��������ʱ���������¼��ķ�ʽ�����ݲ���
��Щ�¼�����ϵͳ�¼��Ϳռ䶨����¼�
-# ϵͳ��Ϣ
    - ���е�ϵͳ��Ϣ�����ȸ�Ӧ�ô����ؼ������䷵��ֵ������һ������
-# �ؼ�������Ϣ
    - �ؼ���Ӧ�õĿ��Ʋ���
        -# draw ui - ���ؼ���Ҫ��ʾ��ʱ����ʵ����ʾ��Ӧ��
        -# sleep - ��Ӧ�õ�״̬���������Ӧ�� - �����Ҫ����Ҫ��
        -# force commit (underline) - һЩӦ�ã������������룬��Ҫ�ڼ��̵�ʱ��ȷ��
        underline�����ݣ��Ӷ�����ǿ���ύ����Ȼ�˴����������ִٵ���������û�
        ����ܲ����ȷʵ������Ӧ�õ���������������˺����������Ӧ����Ҫ��
        �ǵģ�im�ṩ��ɱ�˵ĵ����û��ǲ���
-# set��
    - �ؼ�Ҫ��Ӧ�øı�����
        -# ���뷨
        -# �������ԡ��������͡����뷽ʽ����Сдģʽ
        -# �������ͼ��ϡ����뷽ʽ����
-# get��
    - �ؼ���Ӧ�û�ȡӦ��ʵ��������
        -# ���뷨
        -# �������ԡ��������͡����뷽ʽ����Сдģʽ
        -# �������򡢿�������
        -# �༭״̬
-# custom��
    - �ؼ�Ϊ�����ض�Ӧ�õ����������һЩ����
        -# ��������
        -# �����������
@sa GUIIM_EVENT_T
*/
enum _GUIIM_EVENT_T
{
    GUIIM_EVENT_NONE,

    GUIIM_EVENT_SYS_MSG,

    GUIIM_EVENT_PAINT,
    GUIIM_EVENT_SLEEP,
    GUIIM_EVENT_FORCE_COMMIT,

    GUIIM_EVENT_SET_METHOD,
    GUIIM_EVENT_SET_LANG,
    GUIIM_EVENT_SET_TYPE,
    GUIIM_EVENT_SET_INPUT,
    GUIIM_EVENT_SET_CAPITAL,
    GUIIM_EVENT_SET_LIMIT,
    GUIIM_EVENT_SET_INPUT_SET,

    GUIIM_EVENT_GET_METHOD,
    GUIIM_EVENT_GET_LANG,
    GUIIM_EVENT_GET_TYPE,
    GUIIM_EVENT_GET_INPUT,
    GUIIM_EVENT_GET_CAPITAL,
    GUIIM_EVENT_GET_RECT,
    GUIIM_EVENT_GET_CTRL_RECT,
    GUIIM_EVENT_GET_EDIT_STATE,

    GUIIM_EVENT_CUSTOM_KEY,
    GUIIM_EVENT_CUSTOM_VKB,

    GUIIM_EVENT_MAX,
};

/*! @typedef GUIIM_EVENT_T
@brief ���뷨�ؼ��¼�����
@sa _GUIIM_EVENT_T
*/
typedef uint8 GUIIM_EVENT_T;


/*! @enum _GUIIM_EDIT_STATE_T
@brief ���뷨Ӧ�õ�����״̬
@details ��ϸ�������뷨״̬��
- �ڲ�ͬ��״̬�£�������ͬ��softkey��ʾ
-# �ַ�����״̬
    - ȷ�� x ɾ��
-# �ַ���ѡ����С��ǩ״̬
    - ȷ�� x ȡ��
-# �ַ���ѡ����С��ǩ״̬
    - ȷ�� x ����
-# ��д״̬
    ���ֲ���
@sa GUIIM_EDIT_STATE_T
*/
enum _GUIIM_EDIT_STATE_T
{
    GUIIM_EDIT_NONE,
    GUIIM_EDIT_DEALING, /*!< ���в����ֵĴ���ͳ�ƣ�Ӧ�ò���ϸ�ֵ�ʱ�򣬿�ͳһ���� */
    GUIIM_EDIT_INPUTING,
    GUIIM_EDIT_SELECTING, /*!< ��ѡ������С��ǩ�ַ�״̬ */
    GUIIM_EDIT_CANDITATE, /*!< ��ѡ����С��ǩ״̬ */
    GUIIM_EDIT_HANDWRITING,
    GUIIM_EDIT_SPELL,       /*spell to add new word for samsung style*/
};

/*! @typedef GUIIM_EDIT_STATE_T
@brief ���뷨����״̬����
@sa _GUIIM_EDIT_STATE_T
*/
typedef uint8 GUIIM_EDIT_STATE_T;

/*! @union GUIIM_EVENT_DATA_U
@brief �� GUIIM_EVENT_T ��Ӧ�Ĳ���
*/
typedef union
{
    struct
    {
        MMI_MESSAGE_ID_E msg_id;
        DPARAM param;
    } sys_msg;
    int32 paint;
    int32 sleep;

    GUIIM_METHOD_T set_method;
    GUIIM_LANGUAGE_T set_lang;
    GUIIM_TYPE_T set_type;
    GUIIM_INPUT_TYPE_T set_input;
    GUIIM_CAPITAL_MODE_T set_capital;
    GUIIM_TYPE_T set_limit;
    GUIIM_INPUT_TYPE_T set_input_set;

    GUIIM_METHOD_T get_method;
    GUIIM_LANGUAGE_T get_lang;
    GUIIM_TYPE_T get_type;
    GUIIM_INPUT_TYPE_T get_input;
    GUIIM_CAPITAL_MODE_T get_capital;
    GUI_RECT_T get_rect;
    GUI_RECT_T get_ctrl_rect;
    GUIIM_EDIT_STATE_T edit_state;

    GUIIM_CUSTOM_KEY_PARAM_T *custom_key_info_ptr;
} GUIIM_EVENT_DATA_U;


/*! @enum _GUIIM_NOTIFY_T
@brief ���뷨Ӧ�õķ���
@details ����ͨ�����뷨�ؼ��ͳ���
-# �ύ��
    - ���뷨Ӧ�������뷨�ؼ��ύ��������ύ�ַ���������
        -# commit char - �ύ��ȷ���ַ��ʹ����ַ�
        -# edit operatr - ����ɾ�����������ҵ�----
-# ��ʾ��
    - ���뷨Ӧ���ڲ����ݵı仯��֪ͨ���뷨�ؼ�
        -# ���뷨Ӧ�������趨�����뷨�����ԡ��л������뷽ʽ����Сдģʽ��
        -# ���뷨Ӧ�õĻ������򡢿�������ȷ����˱仯
-# ������
    - ���뷨Ӧ�ô���һ�������ܴ���Ĳ�������Ҫ�ؼ������
        -# �����л����뷨
        -# �����л��������ԡ��������͡����뷽ʽ����Сдģʽ
        -# ����ˢ��
@sa GUIIM_NOTIFY_T
*/
enum _GUIIM_NOTIFY_T
{
    GUIIM_NOTIFY_NONE,

    GUIIM_NOTIFY_COMMIT,
	GUIIM_NOTIFY_DELETE,
    GUIIM_NOTIFY_LONG_DEL,

    GUIIM_NOTIFY_CHANGE_RECT,
    GUIIM_NOTIFY_CHANGE_EDIT_STATE,
    GUIIM_NOTIFY_SWITCH_METHOD,
    GUIIM_NOTIFY_SWITCH_CAPITAL,

    GUIIM_NOTIFY_SET_METHOD,
    GUIIM_NOTIFY_SET_LANG,
    GUIIM_NOTIFY_SET_TYPE,
    GUIIM_NOTIFY_SET_INPUT,
    GUIIM_NOTIFY_SET_CAPITAL,
    GUIIM_NOTIFY_PAINT,

    GUIIM_NOTIFY_TRANS_MSG,

    GUIIM_NOTIFY_HIDE,

    GUIIM_NOTIFY_SETCURSOR,
    GUIIM_NOTIFY_GETCURSOR,

    GUIIM_NOTIFY_SET_HIGHLIGHT,
    GUIIM_NOTIFY_COMMIT_UNDERLINE,
    
    GUIIM_NOTIFY_AUTO_SWITCH_METHOD,

    GUIIM_NOTIFY_MAX,
};

/*! @typedef GUIIM_NOTIFY_T
@brief ���뷨Ӧ�õķ������Ͷ���
@sa _GUIIM_NOTIFY_T
*/
typedef uint8 GUIIM_NOTIFY_T;


/*! @struct GUIIM_COMMIT_DATA_T
@brief ���ظ�Ӧ�õ���Ϣ����
*/
typedef struct
{
    wchar commit[GUIIM_COMMIT_BUFFER_MAX + 1];
    uint16 commit_len;
    wchar underline[GUIIM_COMMIT_BUFFER_MAX + 1];
    uint16 underline_len;
} GUIIM_COMMIT_DATA_T;

typedef struct
{
    MMI_MESSAGE_ID_E msg_id;
    DPARAM param;    
} GUIIM_TRANS_MSG_DATA_T;

typedef enum
{
    GUIIM_SETCURSOR_TYPE_CUR,           // set offset value by current cursor
    GUIIM_SETCURSOR_TYPE_START,         // set offset value by start
    GUIIM_SETCURSOR_TYPE_END,           // set offset value by end

    GUIIM_SETCURSOR_TYPE_MAX
} GUIIM_SETCURSOR_TYPE_E;

// these macro is used by guiedit and guiim to pack up the param data
#define MAKE_CURSOR_PARAM(type, value)  ((type<<16)|(value&0xffff))
#define SETCURSOR_TYPE(v)               (((uint32)v)>>16)
#define SETCURSOR_VALUE(v)              ((uint16)v&0xffff)

typedef struct
{
    GUIIM_SETCURSOR_TYPE_E  type;       // set cursor type
    int16                   value;      // the cursor offset
} GUIIM_SETCURSOR_MSG_DATA_T;

typedef struct
{
    uint16                  start_index;
    uint16                  end_index;
} GUIIM_SET_HIGHLIGHT_MSG_DATA_T;

/*! @union GUIIM_NOTIFY_DATA_U
@brief �� GUIIM_NOTIFY_T ��Ӧ�����ݵ����϶���
*/
typedef union
{
    GUIIM_COMMIT_DATA_T commit_data;
    uint32 long_del;

    GUI_RECT_T change_rect;
    GUIIM_EDIT_STATE_T edit_state;
    GUIIM_METHOD_T switch_method;
    GUIIM_LANGUAGE_T switch_lang;
    GUIIM_TYPE_T switch_type;
    GUIIM_INPUT_TYPE_T switch_input;
    GUIIM_CAPITAL_MODE_T switch_capital;

    GUIIM_METHOD_T set_method;
    GUIIM_LANGUAGE_T set_lang;
    GUIIM_TYPE_T set_type;
    GUIIM_INPUT_TYPE_T set_input;
    GUIIM_CAPITAL_MODE_T set_capital;

    GUIIM_TRANS_MSG_DATA_T trans_msg;

    GUIIM_SETCURSOR_MSG_DATA_T  setcursor_msg;
    uint16                      getcursor_value;

    GUIIM_SET_HIGHLIGHT_MSG_DATA_T  set_highlight_msg;
    uint32 commit_underline;

    uint32 paint;
} GUIIM_NOTIFY_DATA_U;

/*! @struct GUIIM_NOTIFY_PACK_T
@brief notify ��Ϣ���������ݰ��Ķ���
*/
typedef struct
{
    GUIIM_NOTIFY_T notify;
    GUIIM_NOTIFY_DATA_U data;
}GUIIM_NOTIFY_PACK_T;

/*! @struct GUIIM_NOTIFY_QUEUE_T
@brief notify ��Ϣ���ж���
*/
typedef struct
{
    GUIIM_NOTIFY_PACK_T pack[GUIIM_NOTIFY_QUEUE_LEN];
    int8 pack_count;
    int8 pack_index;
}GUIIM_NOTIFY_QUEUE_T;


/*! @typedef GUIIM_APIS_INITIALIZE_T
@brief Ӧ�ó�ʼ������
@sa GUIIM_APIS_TERMINATE_T
@sa GUIIM_APIS_HANDLE_T
*/
typedef void* (*GUIIM_APIS_INITIALIZE_T)(GUIIM_INIT_KERNEL_T *init_data_ptr);
/*! @typedef GUIIM_APIS_TERMINATE_T
@brief Ӧ�����ٺ���
@sa GUIIM_APIS_INITIALIZE_T
@sa GUIIM_APIS_HANDLE_T
*/
typedef BOOLEAN (*GUIIM_APIS_TERMINATE_T)(void *handle);
/*! @typedef GUIIM_APIS_HANDLE_T
@brief Ӧ���¼�������
@sa GUIIM_APIS_INITIALIZE_T
@sa GUIIM_APIS_TERMINATE_T
*/
typedef BOOLEAN (*GUIIM_APIS_HANDLE_T)(void* handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *data_ptr);

/*! @struct guiim_api_tag
@brief �������Ĵ���ṹ������ͳһ����
*/
struct guiim_api_tag
{
    GUIIM_APIS_INITIALIZE_T init;
    GUIIM_APIS_TERMINATE_T term;
    GUIIM_APIS_HANDLE_T handle;
};

/*! @struct GUIIM_APIS_T
@brief �������Ĵ���ṹ
*/
typedef struct guiim_api_tag GUIIM_APIS_T;

/*! @typedef GUIIM_KERNEL_APIS_T
@brief �������Ĵ���ṹ
*/
typedef GUIIM_APIS_T GUIIM_KERNEL_APIS_T;

/*! @typedef GUIIM_APIS_PTR_T
@brief �������Ĵ���ṹָ��
*/
typedef GUIIM_APIS_T const *GUIIM_APIS_PTR_T;

/*! @struct GUIIM_APP_CFG_T
@brief ���뷨�ں˹����ṹ
*/
typedef struct
{
    GUIIM_METHOD_T method;
    GUIIM_APIS_PTR_T apis_ptr;
} GUIIM_APP_CFG_T;

/*! @struct GUIIM_ICON_CFG_T
@brief ���뷨��ص�ͼ�궨��
*/
typedef struct
{
    GUIIM_METHOD_T method;
    MMI_IMAGE_ID_T icon;
} GUIIM_ICON_CFG_T;

/*! @struct GUIIM_TEXT_CFG_T
@brief ���뷨��ص�ͼ�궨��
*/
typedef struct
{
    GUIIM_METHOD_T method;
    wchar text[GUIIM_TEXT_CFG_MAX_LEN];
} GUIIM_TEXT_CFG_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ���뷨Ӧ�������뷨�ؼ��Ĵ�����Ϣ�������Ӽ�¼
@author Lianxiang.Zhou
@param im_handle [in] ���뷨�ؼ����
@param pack_data_ptr [in] ��Ϣ����ָ��
@retval TRUE ������Ϣ�ɹ�
@retval FALSE ������Ϣʧ��
@return ���������Ƿ�ɹ�
@note ���������ִ�����첽�ģ��ڷ������뷨�ؼ�����Ϣ����֮��ͳһ������Ϣ����
@sa GUIIM_CommitData
*******************************************************************************/
PUBLIC BOOLEAN CTRLIM_SetNotify(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);


/***************************************************************************//*!
@brief �ύ���뷨��Ϣ���ؼ�
@author Lianxiang.Zhou
@param im_handle [in] ���뷨�ؼ����
@param pack_data_ptr [in] ��Ϣ����ָ��
@retval TRUE ������Ϣ�ɹ�
@retval FALSE ������Ϣʧ��
@return ���������Ƿ�ɹ�
@note �����ͬ������ģ���Ϣ�ᱻ���̴����ύ�����뷨�ؼ��ĸ��ؼ�
@sa GUIIM_SetNotify
*******************************************************************************/
PUBLIC BOOLEAN CTRLIM_CommitData(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);

/***************************************************************************//*!
@brief ��¼һ�������뷨�ؼ���صĴ���
@author Lianxiang.Zhou
@param im_handle [in] ���뷨���
@param win_handle [in] Ҫ��¼�Ĵ��ھ��
@note һЩ���뷨�ؼ���صĴ��ڣ������뷨�ؼ��ر�֮��Ҫ����رա�
@sa GUIIM_RemoveWin
*******************************************************************************/
PUBLIC void CTRLIM_AppendWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
);

/***************************************************************************//*!
@brief �Ƴ������뷨�ؼ���صĴ��ڣ������뷨�ؼ����ٹ�ע�ô���
@author Lianxiang.Zhou
@param im_handle [in] ���뷨���
@param win_handle [in] Ҫ��¼�Ĵ��ھ��
@note һЩ���뷨�ؼ���صĴ��ڣ������뷨�ؼ��ر�֮��Ҫ����رա�
@sa GUIIM_RemoveWin
*******************************************************************************/
PUBLIC void CTRLIM_RemoveWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
