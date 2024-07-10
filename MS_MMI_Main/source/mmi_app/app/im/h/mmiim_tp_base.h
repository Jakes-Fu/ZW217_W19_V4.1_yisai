/*******************************************************************************
** File Name: mmiim_base.h
** Author: Lianxiang.Zhou
** Date: 2009/08/08
** Copyright: 2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description: Ӧ�ò��ֵĹ��ö���
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2009/08/08   Lianxiang.Zhou  Create.
*******************************************************************************/

#ifndef _MMIIM_TP_BASE_H_
#define _MMIIM_TP_BASE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(MMIIM_USE_TOUCH_PANEL)

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//const def
enum
{
    MMIIM_TP_KEY_PER_ROW_MAX = 13,
    MMIIM_TP_TAG_MAX_LEN = 8 + 1, // 1 �� '\0'
    MMIIM_TP_SHIFT_VKEY_MAX = 5,
    //���尴���ĸ߶ȼ����, pixel
    MMIIM_TP_VKEY_HEIGHT = 16,
    MMIIM_TP_VKEY_SPACE = 3,
};


/*
�������ϵ�еĶ��壬��0xe0000��ʼ������Ϊ���unicode�ǿհ׵ģ����Ըɾ�����Ӱ��
��Щ�����character�����뷨Ӧ�ÿ��Դ������뷨�ؼ������
��Ϊ�ڴ��������ʱ��һЩ����İ������߰�ť�����ͳһ����Ļ�����򻯳���
*/
/*
ÿһ���MAX������һ�����㣬Ҳ����˵MAX�ڱ�������Ч�ģ�����һ���"��"
*/
#define MMIIM_TP_TAG_START (0xE000)

//�����ĸ�࣬���ڿ������
#define MMIIM_TP_TAG_WORD_START (MMIIM_TP_TAG_START)
enum
{
    MMIIM_TP_TAG_WORD_DCOM = MMIIM_TP_TAG_WORD_START,
        
    MMIIM_TP_TAG_WORD_MAX
};


//�л���
#define MMIIM_TP_TAG_SHFIT_START (MMIIM_TP_TAG_WORD_MAX)
enum
{
    //ͬһ�����������л����ּ���?5��Ӧ�ù���
    MMIIM_TP_TAG_SHIFT_0 = MMIIM_TP_TAG_SHFIT_START,
    MMIIM_TP_TAG_SHIFT_1,
    MMIIM_TP_TAG_SHIFT_2,
    MMIIM_TP_TAG_SHIFT_3,
    MMIIM_TP_TAG_SHIFT_4,
    //��Сд�л�
    MMIIM_TP_TAG_SHIFT_CAP,
    //�л�������
    MMIIM_TP_TAG_SHIFT_SYMBOL,
    //������һ�����̣���һ��������ϵͳ�����
    MMIIM_TP_TAG_SHIFT_BACK,
    //hindi vkb
    MMIIM_TP_TAG_SHIFT_HINDI_M1,
    MMIIM_TP_TAG_SHIFT_HINDI_M2,
    MMIIM_TP_TAG_SHIFT_HINDI_V1,
    MMIIM_TP_TAG_SHIFT_HINDI_V2,

    MMIIM_TP_TAG_SHFIT_MAX,
};

//����ѡ����
#define MMIIM_TP_TAG_LANG_START (MMIIM_TP_TAG_SHFIT_MAX)
enum
{
    MMIIM_TP_TAG_LANG_ARABIC = MMIIM_TP_TAG_LANG_START,
    MMIIM_TP_TAG_LANG_CHS,
    MMIIM_TP_TAG_LANG_ENGLISH,
    MMIIM_TP_TAG_LANG_FRENCH,
    MMIIM_TP_TAG_LANG_GERMAN,
    MMIIM_TP_TAG_LANG_GREEK,
    MMIIM_TP_TAG_LANG_HINDI,
    MMIIM_TP_TAG_LANG_HUNGARIAN,
    MMIIM_TP_TAG_LANG_INDONESIAN,
    MMIIM_TP_TAG_LANG_ITALIAN,
    MMIIM_TP_TAG_LANG_MALAY,
    MMIIM_TP_TAG_LANG_PERSIAN,
    MMIIM_TP_TAG_LANG_PORTUGUESE,
    MMIIM_TP_TAG_LANG_RUSSIAN,
    MMIIM_TP_TAG_LANG_SPANISH,
    MMIIM_TP_TAG_LANG_TAGALOG,
    MMIIM_TP_TAG_LANG_THAI,
    MMIIM_TP_TAG_LANG_TURKISH,
    MMIIM_TP_TAG_LANG_URDU,
    MMIIM_TP_TAG_LANG_VIETNAMESE,
    
    MMIIM_TP_TAG_LANG_MAX,
};

//�����ַ���
#define MMIIM_TP_TAG_CHAR_START (MMIIM_TP_TAG_LANG_MAX)
enum
{
    MMIIM_TP_TAG_CHAR_123 = MMIIM_TP_TAG_CHAR_START,
    MMIIM_TP_TAG_CHAR_ABC,
    MMIIM_TP_TAG_CHAR_PINYIN,
    MMIIM_TP_TAG_CHAR_VKEYBOARD,
    MMIIM_TP_TAG_CHAR_SYMBOL,
    
    MMIIM_TP_TAG_CHAR_MAX,
};

//�����ַ���
#define MMIIM_TP_TAG_CTRL_START (MMIIM_TP_TAG_CHAR_MAX)
enum
{
    MMIIM_TP_TAG_CTRL_COMBINE = MMIIM_TP_TAG_CTRL_START,

    MMIIM_TP_TAG_CTRL_MAX,
};

//�ⲿ��ת��
#define MMIIM_TP_TAG_SWITCH_START (MMIIM_TP_TAG_CTRL_MAX)
enum
{
    MMIIM_TP_TAG_SWITCH_HANDWRITING = MMIIM_TP_TAG_SWITCH_START,

    MMIIM_TP_TAG_SWITCH_MAX,
};


//-----------�µļ��̶���--------------
/*
��Ϊ�����֣�һ�����Ƕ�����̲��֣�һ�����ǽ�����������ɵ�����
*/
typedef struct
{
    uint8 max_keys;
    uint8 fact_keys;
    wchar keys[MMIIM_TP_KEY_PER_ROW_MAX];
} const MMIIM_TP_VKEY_ROW_T;

typedef struct
{
    MMIIM_TP_VKEY_ROW_T *vkey_rows;
    int32 row_count;
    wchar const *tag_ptr;
    int32 tag_count;
} const MMIIM_TP_VKEY_LAYOUT_T;

/* �����ַ���ʾ���ַ����ṹ���� */
typedef struct
{
    wchar tag;
    wchar str[MMIIM_TP_TAG_MAX_LEN];
}MMIIM_TP_TAG_STRING_T;

//�����״̬
enum
{
    MMIIM_TP_KEY_STATE_INACTIVE = 0x00, //������״̬�����һ�
    MMIIM_TP_KEY_STATE_ACTIVE = 0x01, //����״̬
    MMIIM_TP_KEY_STATE_SELECT = 0x02, //��ѡ��
    MMIIM_TP_KEY_STATE_PRESS = 0x04, //������

    MMIIM_TP_KEY_STATE_MAX,
};
typedef uint8 MMIIM_TP_KEY_STATE_T;

enum
{
    //����
    MMIIM_TP_KEY_TYPE_NORMAL,
    //�����ַ�����Ҫ�ر���ʾ������ո񡢻��С�ɾ����
    MMIIM_TP_KEY_TYPE_SPECIAL_CHAR,
    //�����ĸ
    MMIIM_TP_KEY_TYPE_WORD,
    //�����л�
    MMIIM_TP_KEY_TYPE_LANG,
    //�����л�
    MMIIM_TP_KEY_TYPE_VKEY,
    //���뷨�л�
    MMIIM_TP_KEY_TYPE_IM,
    //��ʾ��ǩ
    MMIIM_TP_KEY_TYPE_TAG,
    //���ƿ�
    MMIIM_TP_KEY_TYPE_CTRL,
    //��ת��
    MMIIM_TP_KEY_TYPE_SWITCH,


    MMIIM_TP_KEY_TYPE_MAX,
};
typedef uint8 MMIIM_TP_KEY_TYPE_T;

typedef struct
{
    MMIIM_TP_KEY_STATE_T state;
    MMIIM_TP_KEY_TYPE_T type;
    wchar disp_char;
    GUI_RECT_T rect;
}MMIIM_TP_KEY_T;

typedef struct
{
    BOOLEAN is_initialized;
    GUI_RECT_T rect;
    int32 key_count;
    MMIIM_TP_VKEY_LAYOUT_T *layout_ptr;
    MMIIM_TP_KEY_T *keys_ptr;
    MMIIM_TP_VKEY_LAYOUT_T *auto_back_layout_ptr;
    wchar last_press_key;
}MMIIM_TP_VKB_T;

//�������뷨��Ӧ�ĳ�ʼ������
typedef struct
{
    GUIIM_METHOD_T method;
    MMIIM_TP_VKEY_LAYOUT_T *layout_ptr;
}MMIIM_TP_VKB_CFG_T;

typedef struct
{
    GUI_RECT_T rect_tips;
    GUI_COLOR_T color_tips_bg;
    GUI_COLOR_T color_font_tips;
    GUI_FONT_T font_tips;

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
}MMIIM_TP_VKEY_UI_T;

typedef struct
{
    MMIIM_TP_VKEY_LAYOUT_T * layout_from_ptr;
    wchar disp_char;
    MMIIM_TP_VKEY_LAYOUT_T * layout_to_ptr;
}MMIIM_TP_VKB_SWITCH_T;

typedef struct
{
    wchar disp_char;
    GUIIM_METHOD_T method;
}MMIIM_TP_VKEY_IM_T;

typedef struct
{
    wchar disp_char;
    MMIIM_TP_VKEY_LAYOUT_T * layout_ptr;
}MMIIM_TP_VKEY_VKB_T;

//-----------�µļ��̶���--------------
extern MMIIM_TP_VKEY_UI_T const g_mmiim_tp_ui_cfg;

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

/******************************************************************************/
// Description: �ɼ��̲��ֵõ����̵ĳ�ʼ������
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_ConstructVkb(
                                  MMIIM_TP_VKB_T *vkb_ptr,
                                  MMIIM_TP_VKEY_LAYOUT_T *layout_ptr,
                                  GUI_RECT_T *rect_ptr
                                  );

/******************************************************************************/
// Description: ���ټ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DestroyVkb(MMIIM_TP_VKB_T *vkb_ptr);

/******************************************************************************/
// Description: ���Ƽ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawVkb(
                          GUI_LCD_DEV_INFO *lcd_info_ptr,
                          MMIIM_TP_VKB_T *vkb_ptr
                          );

/******************************************************************************/
// Description: ��������
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawKey(GUI_LCD_DEV_INFO *lcd_info_ptr, MMIIM_TP_KEY_T *key_ptr);

/******************************************************************************/
// Description: ������̵ļ��̵�ĳ��״̬
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ClearVkbState(
                                MMIIM_TP_VKB_T *vkb_ptr,
                                MMIIM_TP_KEY_STATE_T state
                                );

/******************************************************************************/
// Description: �趨�����ϵ�ĳ����Ϊָ��״̬
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_KeyStateSet(
                              MMIIM_TP_VKB_T *vkb_ptr,
                              wchar c,
                              MMIIM_TP_KEY_STATE_T state
                              );

/******************************************************************************/
// Description: �趨�����ϵ�ĳ������ָ��״̬
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_KeyStateUnset(
                                MMIIM_TP_VKB_T *vkb_ptr,
                                wchar c,
                                MMIIM_TP_KEY_STATE_T state
                                );

/******************************************************************************/
// Description: �õ����ʵ���İ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_KEY_T* MMIIM_GetPenKey(
                                       MMIIM_TP_VKB_T *vkb_ptr,
                                       GUI_POINT_T const *point_ptr
                                       );

/******************************************************************************/
// Description: �õ����ʵ���İ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_KEY_T* MMIIM_GetKeyByChar(
                                          MMIIM_TP_VKB_T *vkb_ptr,
                                          wchar c
                                          );

/******************************************************************************/
// Description: �����뷨��ü��̲���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetVkbLayout(GUIIM_METHOD_T method);

/******************************************************************************/
// Description: ��ȡ�л����̵Ĳ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetSwitchLayout(
                                                      MMIIM_TP_VKEY_LAYOUT_T *layout_ptr,
                                                      wchar disp_char
                                                      );

/******************************************************************************/
// Description: ��ȡ�л����̵Ĳ���
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetVkeyLayout(wchar disp_char);

/******************************************************************************/
// Description: ��ȡ������Ӧ�����뷨
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC GUIIM_METHOD_T MMIIM_GetVkeyIm(wchar disp_char);

/******************************************************************************/
// Description: ��ʾ��������ʾ��
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ShowTips(
                           GUI_LCD_DEV_INFO const *lcd_info_ptr,
                           MMIIM_TP_VKB_T const *vkb_ptr,
                           MMIIM_TP_KEY_T const *key_ptr
                           );

/******************************************************************************/
// Description: ���ذ�������ʾ��
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_HideTips(GUI_LCD_DEV_INFO const *lcd_info_ptr);


#ifdef __cplusplus 
}
#endif

#endif //MMIIM_USE_TOUCH_PANEL

#endif //_MMIIM_TP_BASE_H_




