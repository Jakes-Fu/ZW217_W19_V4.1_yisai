/*==============================================================================
File Name: mmiim_sp_basic.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _MMIIM_SP_BASIC_H_
#define _MMIIM_SP_BASIC_H_

#include "mmiim_base.h"

#include "guilcd.h"
#include "mmi_font.h"

#include "mmitheme_im.h"
#include "mmiim_image.h"
#include "mmidisplay_data.h"
#include "mmitheme_pos.h"

/*
���ļ���Ҫ��� spim �� 10a �ϵ� ui �����صĻ������
������ʾ�򡢶�ʱ����

*/

//paramter
//ui data
#if defined (MAINLCD_SIZE_320X480)
#define MMIIM_SP_TEXT_BAR_FONT_TEXT (SONG_FONT_20)
#define MMIIM_SP_TEXT_BAR_FONT_LABEL (SONG_FONT_10)
#define MMIIM_SP_TEXT_BAR_SPACE (5)

#define MMIIM_SP_TEXT_BAR_WIDTH (320)
#define MMIIM_SP_TEXT_BAR_HEIGHT (29)
#define MMIIM_SP_TEXT_BAR_IDC_W (35)
#define MMIIM_SP_TEXT_BAR_IDC_H (29)
#define MMIIM_SP_TEXT_BAR_IDC_Y (3)
#define MMIIM_SP_TEXT_BAR_IDC_LEFT_X (243)
#define MMIIM_SP_TEXT_BAR_IDC_RIGHT_X (280)

#define MMIIM_SP_TEXT_BAR_COLOR_TEXT (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_LABEL (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_HOT (0x7EFF)
#define MMIIM_SP_TEXT_BAR_CAND_Y (3)

#elif defined (MAINLCD_SIZE_176X220)
#define MMIIM_SP_TEXT_BAR_FONT_TEXT (SONG_FONT_16)
#define MMIIM_SP_TEXT_BAR_FONT_LABEL (SONG_FONT_10)
#define MMIIM_SP_TEXT_BAR_SPACE (5)

#define MMIIM_SP_TEXT_BAR_WIDTH (176)
#define MMIIM_SP_TEXT_BAR_HEIGHT (20)
#define MMIIM_SP_TEXT_BAR_IDC_W (17)
#define MMIIM_SP_TEXT_BAR_IDC_H (17)
#define MMIIM_SP_TEXT_BAR_IDC_Y (2)
#define MMIIM_SP_TEXT_BAR_IDC_LEFT_X (140)
#define MMIIM_SP_TEXT_BAR_IDC_RIGHT_X (158)

#define MMIIM_SP_TEXT_BAR_COLOR_TEXT (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_LABEL (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_HOT (0x7EFF)
#define MMIIM_SP_TEXT_BAR_CAND_Y (1)

#else
#define MMIIM_SP_TEXT_BAR_FONT_TEXT (SONG_FONT_20)
#define MMIIM_SP_TEXT_BAR_FONT_LABEL (SONG_FONT_10)
#define MMIIM_SP_TEXT_BAR_SPACE (5)

#define MMIIM_SP_TEXT_BAR_WIDTH (240)
#define MMIIM_SP_TEXT_BAR_HEIGHT (32)
#define MMIIM_SP_TEXT_BAR_IDC_W (30)
#define MMIIM_SP_TEXT_BAR_IDC_H (30)
#define MMIIM_SP_TEXT_BAR_IDC_Y (1)
#define MMIIM_SP_TEXT_BAR_IDC_LEFT_X (174)
#define MMIIM_SP_TEXT_BAR_IDC_RIGHT_X (206)

#define MMIIM_SP_TEXT_BAR_COLOR_TEXT (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_LABEL (MMI_BACKGROUND_COLOR)
#define MMIIM_SP_TEXT_BAR_COLOR_HOT (0x7EFF)
#define MMIIM_SP_TEXT_BAR_CAND_Y (3)
#endif


#define MMIIM_SP_TEXT_BAR_CAND_WIDTH (MMIIM_SP_TEXT_BAR_IDC_LEFT_X - 3)

#define MMIIM_SP_PRESS_IDC(idc) do{if (MMIIM_SP_IDC_ACTIVE == idc) idc = MMIIM_SP_IDC_PRESSED;}while(0)
#define MMIIM_SP_KEY_TYPE_MASK (0xff00)
#define MMIIM_SP_KEY_MASK (0x00ff)


/* �������� */
enum
{
    //�����ֵĸ���
    MMIIM_SP_ASSO_CNT_MAX = 32,
    //��ʾ����������
    MMIIM_SP_TEXT_BAR_CHAR_MAX = 64,
    //��ʾ������Ƭ����
    MMIIM_SP_TEXT_BAR_SEG_MAX = 20,
    //���ҷ�ҳ��־ת���ɵ�������ֵ
    MMIIM_SP_TEXT_BAR_TP_IDC_LEFT = MMIIM_SP_TEXT_BAR_SEG_MAX,
    MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT,
};




//������Ϣ��ת��Ϊ���뷨��Ϣ������ͣ����ӵ��ⲿ��Ϣ���򵥵��ڲ���Ϣ
enum
{
    MMIIM_SP_MSG_NONE,
    MMIIM_SP_MSG_KEY_PRESS,
    MMIIM_SP_MSG_KEY_RELEASE,
    MMIIM_SP_MSG_KEY_LONG_PRESS,
    MMIIM_SP_MSG_KEY_LONG_RELEASE,
    MMIIM_SP_MSG_KEY_REPEAT_PRESS,
    MMIIM_SP_MSG_TPDOWN,
    MMIIM_SP_MSG_TPMOVE,
    MMIIM_SP_MSG_TPUP,
    MMIIM_SP_MSG_TPLONG,

    MMIIM_SP_MSG_MAX,
};
typedef uint32 MMIIM_SP_MSG_ID_T;


enum
{
    MMIIM_SP_MSG_TP_NONE,
    MMIIM_SP_MSG_TP_KB,
    MMIIM_SP_MSG_TP_BAR0, //����������bar
    MMIIM_SP_MSG_TP_BAR1,
    MMIIM_SP_MSG_TP_BAR2,
    MMIIM_SP_MSG_TP_BAR3,
    MMIIM_SP_MSG_TP_AREA0, //�������򣬸�Ӧ���Զ���
    MMIIM_SP_MSG_TP_AREA1,

    MMIIM_SP_MSG_TP_MAX
};
typedef uint8 MMIIM_SP_MSG_TP_AREA_T;

typedef struct
{
    MMIIM_SP_MSG_TP_AREA_T area;
    int32 idx; // key idx or seg idx
    GUI_POINT_T point;
} MMIIM_SP_MSG_PARA_TP_T;



enum
{
    MMIIM_SP_KEY_NONE,

    MMIIM_SP_KEY_0,
    MMIIM_SP_KEY_1,
    MMIIM_SP_KEY_2,
    MMIIM_SP_KEY_3,
    MMIIM_SP_KEY_4,
    MMIIM_SP_KEY_5,
    MMIIM_SP_KEY_6,
    MMIIM_SP_KEY_7,
    MMIIM_SP_KEY_8,
    MMIIM_SP_KEY_9,
    MMIIM_SP_KEY_STAR,
    MMIIM_SP_KEY_HASH,

    MMIIM_SP_KEY_UP,
    MMIIM_SP_KEY_DOWN,
    MMIIM_SP_KEY_LEFT,
    MMIIM_SP_KEY_RIGHT,

    MMIIM_SP_KEY_OK,
    MMIIM_SP_KEY_CANCEL,

    MMIIM_SP_KEY_A,
    MMIIM_SP_KEY_B,
    MMIIM_SP_KEY_C,
    MMIIM_SP_KEY_D,
    MMIIM_SP_KEY_E,
    MMIIM_SP_KEY_F,
    MMIIM_SP_KEY_G,
    MMIIM_SP_KEY_H,
    MMIIM_SP_KEY_I,
    MMIIM_SP_KEY_J,
    MMIIM_SP_KEY_K,
    MMIIM_SP_KEY_L,
    MMIIM_SP_KEY_M,
    MMIIM_SP_KEY_N,
    MMIIM_SP_KEY_O,
    MMIIM_SP_KEY_P,
    MMIIM_SP_KEY_Q,
    MMIIM_SP_KEY_R,
    MMIIM_SP_KEY_S,
    MMIIM_SP_KEY_T,
    MMIIM_SP_KEY_U,
    MMIIM_SP_KEY_V,
    MMIIM_SP_KEY_W,
    MMIIM_SP_KEY_X,
    MMIIM_SP_KEY_Y,
    MMIIM_SP_KEY_Z,

    MMIIM_SP_KEY_DEL,
    MMIIM_SP_KEY_COMMA,
    MMIIM_SP_KEY_PERIOD,
    MMIIM_SP_KEY_ENTER,
    MMIIM_SP_KEY_FN,
    MMIIM_SP_KEY_SHIFT,
    MMIIM_SP_KEY_AT_QWERTY,
    MMIIM_SP_KEY_SPACE,
    MMIIM_SP_KEY_AND,
    MMIIM_SP_KEY_QUESTION,
    MMIIM_SP_KEY_CTRL,

    MMIIM_SP_KEY_PLUS,                  // +
    MMIIM_SP_KEY_LEFT_PARENTHESIS,      // (
    MMIIM_SP_KEY_RIGHT_PARENTHESIS,     // )
    MMIIM_SP_KEY_MINUS,                 // -
    MMIIM_SP_KEY_DOUBLE_QUOTES,         // "
    MMIIM_SP_KEY_SEMICOLON,             // ;
    MMIIM_SP_KEY_COLON,                 // : 
    MMIIM_SP_KEY_SLASH,                 // /
    MMIIM_SP_KEY_SHIFT_AND,             // &
    MMIIM_SP_KEY_EXCLAMATION,           // !

    MMIIM_SP_KEY_QUOTES,        //'
    MMIIM_SP_KEY_EQUAL,     //=
    MMIIM_SP_KEY_RIGHT_SLASH,       //\
    
    MMIIM_SP_KEY_MAX
};
typedef uint8 MMIIM_SP_VKEY_T;

//����ȫ���̵�ʱ���Ӧ�����ּ�
enum
{
    MMIIM_SP_KEY_V0 = MMIIM_SP_KEY_M,
    MMIIM_SP_KEY_V1 = MMIIM_SP_KEY_R,
    MMIIM_SP_KEY_V2 = MMIIM_SP_KEY_T,
    MMIIM_SP_KEY_V3 = MMIIM_SP_KEY_Y,
    MMIIM_SP_KEY_V4 = MMIIM_SP_KEY_F,
    MMIIM_SP_KEY_V5 = MMIIM_SP_KEY_G,
    MMIIM_SP_KEY_V6 = MMIIM_SP_KEY_H,
    MMIIM_SP_KEY_V7 = MMIIM_SP_KEY_V,
    MMIIM_SP_KEY_V8 = MMIIM_SP_KEY_B,
    MMIIM_SP_KEY_V9 = MMIIM_SP_KEY_N,
};

//�����ֵ����Ӧ�Ĳ��� - һЩ��ֵ�����ύ���ַ�����������һ������
enum
{
    MMIIM_SP_KEY_OP_SYMBOL_PANEL = 0xe000,
    MMIIM_SP_KEY_OP_SWITCH_MENU,
};


typedef union
{
    MMIIM_SP_MSG_PARA_TP_T tp;
    MMIIM_SP_VKEY_T key;
} MMIIM_SP_MSG_PARA_T;


typedef struct
{
    MMIIM_SP_MSG_ID_T id;
    MMIIM_SP_MSG_PARA_T para;
} MMIIM_SP_MSG_T;


enum
{
    MMIIM_SP_STATE_NOINPUT,
    MMIIM_SP_STATE_INPUTING,
    MMIIM_SP_STATE_SELECTING,
    MMIIM_SP_STATE_ASSOCIATE,
    MMIIM_SP_STATE_SPELL,       //spell to add new word for samsung style
};
typedef uint8 MMIIM_SP_STATE_T;



/*
��ʾ�򲿷�
��Ҫ������ʾ��ѡ�ִʡ�Ԥ�༭�����ȵĺ�����ʾ����
�����������
    1 ��ʾ�ִʲ���
    2 ָʾ��ͷ����
���У���ʾ�ִʲ��ֵ�������Ҫ��
    1 �Ƿ���ʾ��ǩ���
    2 �Ƿ���ʾ������Ϣ
    3 ��������λ��
    4 �ִʵ����弰��ɫ
    5 ��ǩ�����弰��ɫ
ָʾ��ͷ���ֵ���Ҫ������
    1 �Ƿ���ʾ�����ҷֱ�����
    2 ��ʾ״̬�����ҷֱ�����
        1 �һ�
        2 �ɵ��
        3 �����
�����ʾ���ǵ��еģ���֧�ֶ�����ʾͬһ���Եĺ�ѡ��
��Ҫ��������
    1 ��ʼ�����������еĲ���������
    2 ��ʾ�����յ�ǰ���õĲ�������ʾ
    3 ������ݣ��������������ã����ݹ���
    4 �ָ������Ƭ�Σ�ͨ��Ӧ�û��ṩһ���ַ����ָ��
*/
enum
{
    MMIIM_SP_IDC_NONE,
    MMIIM_SP_IDC_INACTIVE,
    MMIIM_SP_IDC_ACTIVE,
    MMIIM_SP_IDC_PRESSED,

    MMIIM_SP_IDC_MAX,
};
typedef uint8 MMIIM_SP_IDC_STATE_T;


typedef struct
{
    MMI_IMAGE_ID_T img_bg;
    MMI_IMAGE_ID_T img_idc_left[MMIIM_SP_IDC_MAX];
    MMI_IMAGE_ID_T img_idc_right[MMIIM_SP_IDC_MAX];

    GUI_RECT_T rect_idc_left;
    GUI_RECT_T rect_idc_right;

    GUI_FONT_T font_text;
    GUI_FONT_T font_label;
    GUI_COLOR_T color_text;
    GUI_COLOR_T color_label;
    GUI_COLOR_T color_hot;
} MMIIM_SP_TEXT_BAR_THEME_T;


typedef struct
{
    wchar * ptr;
    uint16 len;
    uint16 width;
} MMIIM_SP_TEXT_BAR_SEG_INFO_T;


typedef struct
{
    GUI_RECT_T rect; //��ʾ�ľ���λ��
    MMI_WIN_ID_T win_id;
    GUI_LCD_DEV_INFO *lcd_info_ptr;

    uint8 width_label; // ��ǩ���
    uint8 width_space; // ������

    int8 has_label; // 0 - none, 1 - has
    int8 has_hot; // 0 - none, 1 - has
    uint8 hot_index; // [0, n] - hot index

    uint8 seg_cnt;
    uint8 text_len;
    wchar text[MMIIM_SP_TEXT_BAR_CHAR_MAX]; //�ÿո�ָ�������Ƭ��
    MMIIM_SP_TEXT_BAR_SEG_INFO_T seg[MMIIM_SP_TEXT_BAR_SEG_MAX];

    MMIIM_SP_IDC_STATE_T idc_left;
    MMIIM_SP_IDC_STATE_T idc_right;
} MMIIM_SP_TEXT_BAR_T;



typedef struct
{
    wchar *list_ptr;
    uint8 cnt;
    uint8 per_page;
    uint8 cur_page;
} MMIIM_SP_BAR_DATA_T;



/*==============================================================================
Function Name: MMIIM_SP_TextBarInit
Description: MMIIM_SP_TextBarInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarInit(MMIIM_SP_TEXT_BAR_T * bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarDraw
Description: MMIIM_SP_TextBarDraw
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarDraw(MMIIM_SP_TEXT_BAR_T const * bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarClear
Description: MMIIM_SP_TextBarClear
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarClear(MMIIM_SP_TEXT_BAR_T * bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarGenTextSeg
Description: MMIIM_SP_TextBarGenTextSeg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarGenTextSeg(
    MMIIM_SP_TEXT_BAR_T * bar_ptr,
    wchar const *text_ptr,
    size_t text_cnt_limit,
    wchar split,
    uint8 seg_cnt
);

/*==============================================================================
Function Name: MMIIM_SP_TextBarGenTextStr
Description: MMIIM_SP_TextBarGenTextStr
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarGenTextStr(
    MMIIM_SP_TEXT_BAR_T * bar_ptr,
    wchar const *text_ptr,
    size_t text_len
);

/*==============================================================================
Function Name: MMIIM_SP_TextBarUpdate
Description: MMIIM_SP_TextBarUpdate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarUpdate(
    MMIIM_SP_TEXT_BAR_T *bar_ptr,
    MMIIM_SP_BAR_DATA_T *data_ptr
);

/*==============================================================================
Function Name: MMIIM_SP_TextBarGetTp
Description: MMIIM_SP_TextBarGetTp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarGetTp(MMIIM_SP_TEXT_BAR_T *bar_ptr, GUI_POINT_T *point_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarLower
Description: MMIIM_SP_TextBarLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarLower(MMIIM_SP_TEXT_BAR_T *bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarUpper
Description: MMIIM_SP_TextBarUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarUpper(MMIIM_SP_TEXT_BAR_T *bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarLeadUpper
Description: MMIIM_SP_TextBarLeadUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarLeadUpper(MMIIM_SP_TEXT_BAR_T *bar_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TextBarClearIDC
Description: MMIIM_SP_TextBarClearIDC
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_TextBarClearIDC(MMIIM_SP_TEXT_BAR_T *bar_ptr);

/*==============================================================================
Description: MMIIM_SP_StringWidth
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint32 MMIIM_SP_StringWidth(wchar const *str_ptr, uint32 str_len);

#endif //_MMIIM_SP_BASIC_H_





