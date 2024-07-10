/*==============================================================================
File Name: mmiim_sp_tp_common.h
Author: Lianxiang.Zhou
Date: 2010/06/04
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/04   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_SP_TP_COMMON_H_
#define _MMIIM_SP_TP_COMMON_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_sp_tp_basic.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//const def
enum
{
    MMIIM_SP_TPCOM_KEY_PER_ROW_MAX = 13,
    MMIIM_SP_TPCOM_TAG_MAX_LEN = 8 + 1, // 1 �� '\0'
    //���尴���ĸ߶ȼ����, pixel
    MMIIM_SP_TPCOM_VKEY_HEIGHT = 16,
    MMIIM_SP_TPCOM_VKEY_SPACE = 3,
};


/*
�������ϵ�еĶ��壬��0xe0000��ʼ������Ϊ���unicode�ǿհ׵ģ����Ըɾ�����Ӱ��
��Щ�����character�����뷨Ӧ�ÿ��Դ������뷨�ؼ������
��Ϊ�ڴ��������ʱ��һЩ����İ������߰�ť�����ͳһ����Ļ�����򻯳���
*/
/*
ÿһ���MAX������һ�����㣬Ҳ����˵MAX�ڱ�������Ч�ģ�����һ���"��"
*/
#define MMIIM_SP_TPCOM_TAG_START (0xE000)

//�л���
#define MMIIM_SP_TPCOM_TAG_SHFIT_START (MMIIM_SP_TPCOM_TAG_START)
enum
{
    //ͬһ�����������л����ּ���?5��Ӧ�ù���
    MMIIM_SP_TPCOM_TAG_SHIFT_0 = MMIIM_SP_TPCOM_TAG_SHFIT_START,
    MMIIM_SP_TPCOM_TAG_SHIFT_1,
    MMIIM_SP_TPCOM_TAG_SHIFT_2,
    MMIIM_SP_TPCOM_TAG_SHIFT_3,
    MMIIM_SP_TPCOM_TAG_SHIFT_4,
    //��Сд�л�
    MMIIM_SP_TPCOM_TAG_SHIFT_CAP,
    //�л�������
    MMIIM_SP_TPCOM_TAG_SHIFT_SYMBOL,
    //������һ�����̣���һ��������ϵͳ�����
    MMIIM_SP_TPCOM_TAG_SHIFT_BACK,
    //hindi vkb
    MMIIM_SP_TPCOM_TAG_SHIFT_HINDI_M1,
    MMIIM_SP_TPCOM_TAG_SHIFT_HINDI_M2,
    MMIIM_SP_TPCOM_TAG_SHIFT_HINDI_V1,
    MMIIM_SP_TPCOM_TAG_SHIFT_HINDI_V2,

    MMIIM_SP_TPCOM_TAG_SHFIT_MAX,
};

//����ѡ����
#define MMIIM_SP_TPCOM_TAG_LANG_START (MMIIM_SP_TPCOM_TAG_SHFIT_MAX)
enum
{
    MMIIM_SP_TPCOM_TAG_LANG_ARABIC = MMIIM_SP_TPCOM_TAG_LANG_START,
    MMIIM_SP_TPCOM_TAG_LANG_ENGLISH,
    MMIIM_SP_TPCOM_TAG_LANG_FRENCH,
    MMIIM_SP_TPCOM_TAG_LANG_GERMAN,
    MMIIM_SP_TPCOM_TAG_LANG_GREEK,
    MMIIM_SP_TPCOM_TAG_LANG_HINDI,
    MMIIM_SP_TPCOM_TAG_LANG_HUNGARIAN,
    MMIIM_SP_TPCOM_TAG_LANG_INDONESIAN,
    MMIIM_SP_TPCOM_TAG_LANG_ITALIAN,
    MMIIM_SP_TPCOM_TAG_LANG_MALAY,
    MMIIM_SP_TPCOM_TAG_LANG_PERSIAN,
    MMIIM_SP_TPCOM_TAG_LANG_PORTUGUESE,
    MMIIM_SP_TPCOM_TAG_LANG_RUSSIAN,
    MMIIM_SP_TPCOM_TAG_LANG_SPANISH,
    MMIIM_SP_TPCOM_TAG_LANG_TAGALOG,
    MMIIM_SP_TPCOM_TAG_LANG_THAI,
    MMIIM_SP_TPCOM_TAG_LANG_TURKISH,
    MMIIM_SP_TPCOM_TAG_LANG_URDU,
    MMIIM_SP_TPCOM_TAG_LANG_VIETNAMESE,
    
    MMIIM_SP_TPCOM_TAG_LANG_MAX,
};

//�����ַ���
#define MMIIM_SP_TPCOM_TAG_CHAR_START (MMIIM_SP_TPCOM_TAG_LANG_MAX)
enum
{
    MMIIM_SP_TPCOM_TAG_CHAR_123 = MMIIM_SP_TPCOM_TAG_CHAR_START,
    MMIIM_SP_TPCOM_TAG_CHAR_ABC,
    MMIIM_SP_TPCOM_TAG_CHAR_VKEYBOARD,
    MMIIM_SP_TPCOM_TAG_CHAR_SYMBOL,
    
    MMIIM_SP_TPCOM_TAG_CHAR_MAX,
};

//�����ַ���
#define MMIIM_SP_TPCOM_TAG_CTRL_START (MMIIM_SP_TPCOM_TAG_CHAR_MAX)
enum
{
    MMIIM_SP_TPCOM_TAG_CTRL_COMBINE = MMIIM_SP_TPCOM_TAG_CTRL_START,

    MMIIM_SP_TPCOM_TAG_CTRL_MAX,
};

//�ⲿ��ת��
#define MMIIM_SP_TPCOM_TAG_SWITCH_START (MMIIM_SP_TPCOM_TAG_CTRL_MAX)
enum
{
    MMIIM_SP_TPCOM_TAG_SWITCH_HANDWRITING = MMIIM_SP_TPCOM_TAG_SWITCH_START,

    MMIIM_SP_TPCOM_TAG_SWITCH_MAX,
};


//-----------�µļ��̶���--------------
/*
��Ϊ�����֣�һ�����Ƕ�����̲��֣�һ�����ǽ�����������ɵ�����
*/
typedef struct
{
    uint8 max_keys;
    uint8 fact_keys;
    wchar keys[MMIIM_SP_TPCOM_KEY_PER_ROW_MAX];
} const MMIIM_SP_TPCOM_VKEY_ROW_T;

typedef struct
{
    MMIIM_SP_TPCOM_VKEY_ROW_T *vkey_rows;
    int32 row_count;
    wchar const *tag_ptr;
    int32 tag_count;
} const MMIIM_SP_TPCOM_VKEY_LAYOUT_T;

/* �����ַ���ʾ���ַ����ṹ���� */
typedef struct
{
    wchar tag;
    wchar str[MMIIM_SP_TPCOM_TAG_MAX_LEN];
}MMIIM_SP_TPCOM_TAG_STRING_T;

//�����״̬
enum
{
    MMIIM_SP_TPCOM_KEY_STATE_INACTIVE = 0x00, //������״̬�����һ�
    MMIIM_SP_TPCOM_KEY_STATE_ACTIVE = 0x01, //����״̬
    MMIIM_SP_TPCOM_KEY_STATE_SELECT = 0x02, //��ѡ��
    MMIIM_SP_TPCOM_KEY_STATE_PRESS = 0x04, //������

    MMIIM_SP_TPCOM_KEY_STATE_MAX,
};
typedef uint8 MMIIM_SP_TPCOM_KEY_STATE_T;

enum
{
    //����
    MMIIM_SP_TPCOM_KEY_TYPE_NORMAL,
    //�����ַ�����Ҫ�ر���ʾ������ո񡢻��С�ɾ����
    MMIIM_SP_TPCOM_KEY_TYPE_SPECIAL_CHAR,
    //�����л�
    MMIIM_SP_TPCOM_KEY_TYPE_LANG,
    //�����л�
    MMIIM_SP_TPCOM_KEY_TYPE_VKEY,
    //���뷨�л�
    MMIIM_SP_TPCOM_KEY_TYPE_IM,
    //��ʾ��ǩ
    MMIIM_SP_TPCOM_KEY_TYPE_TAG,
    //���ƿ�
    MMIIM_SP_TPCOM_KEY_TYPE_CTRL,
    //��ת��
    MMIIM_SP_TPCOM_KEY_TYPE_SWITCH,


    MMIIM_SP_TPCOM_KEY_TYPE_MAX,
};
typedef uint8 MMIIM_SP_TPCOM_KEY_TYPE_T;

typedef struct
{
    MMIIM_SP_TPCOM_KEY_STATE_T state;
    MMIIM_SP_TPCOM_KEY_TYPE_T type;
    wchar disp_char;
    GUI_RECT_T rect;
}MMIIM_SP_TPCOM_KEY_T;

typedef struct
{
    BOOLEAN is_initialized;
    GUI_RECT_T rect;
    int32 key_count;
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T *layout_ptr;
    MMIIM_SP_TPCOM_KEY_T *keys_ptr;
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T *auto_back_layout_ptr;
    wchar last_press_key;
}MMIIM_SP_TPCOM_VKB_T;

//�������뷨��Ӧ�ĳ�ʼ������
typedef struct
{
    GUIIM_METHOD_T method;
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T *layout_ptr;
}MMIIM_SP_TPCOM_VKB_CFG_T;

typedef struct
{
    GUI_COLOR_T color_panel_bg;
    
    GUI_COLOR_T color_bg_inactive[4];
    
    GUI_COLOR_T color_bg_blk[4];
    GUI_COLOR_T color_bg_blk_press[4];

    GUI_COLOR_T color_bg_tag[4];
    GUI_COLOR_T color_bg_tag_press[4];
    GUI_COLOR_T color_bg_tag_select[4];

    GUI_COLOR_T color_font_default;
    GUI_COLOR_T color_font_normal;
    GUI_COLOR_T color_font_tag;
    
    GUI_FONT_T font_default;
    GUI_FONT_T font_normal;
    GUI_FONT_T font_tag;
}MMIIM_SP_TPCOM_VKEY_UI_T;

typedef struct
{
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T * layout_from_ptr;
    wchar disp_char;
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T * layout_to_ptr;
}MMIIM_SP_TPCOM_VKB_SWITCH_T;

typedef struct
{
    wchar disp_char;
    GUIIM_METHOD_T method;
}MMIIM_SP_TPCOM_VKEY_IM_T;

typedef struct
{
    wchar disp_char;
    MMIIM_SP_TPCOM_VKEY_LAYOUT_T * layout_ptr;
}MMIIM_SP_TPCOM_VKEY_VKB_T;

//-----------�µļ��̶���--------------


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T init_param;
    
    GUIIM_METHOD_T method;

    MMIIM_SP_TPCOM_VKB_T vkb;
}MMIIM_SP_TPCOM_DATA_T;


#endif /*_MMIIM_SP_TP_COMMON_H_*/



