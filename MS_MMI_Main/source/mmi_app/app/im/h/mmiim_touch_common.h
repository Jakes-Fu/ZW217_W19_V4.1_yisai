/*==============================================================================
File Name: mmiim_touch_common.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_COMMON_H_
#define _MMIIM_TOUCH_COMMON_H_





/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_draw.h"

#if defined (MMI_IM_PDA_SUPPORT)

#include "guistring.h"

#include "mmiim.h"
#include "mmiim_internal.h"

#include "mmiim_image.h"
#include "mmiim_text.h"



typedef struct KEY_INFO_TAG KEY_INFO_T;
typedef struct VKEYBOARD_TAG VKEYBOARD_T;
typedef struct IM_DATA_TAG IM_DATA_T;

#define SCI_FREEIF(X)       \
    do {                    \
        if (X != PNULL)      \
        {                   \
            SCI_FREE(X);    \
            X = PNULL;       \
        }                   \
    }                       \
    while(0)

/*! ���ܴ�Сд���ܼ��:
--1.������LeadUpper״̬ʱ������һ���ַ��򵥴ʺ��Զ��л���lower
--2.������Lower״̬����������û��Լ��л�����lower״̬������һ�仰�����󣬻ָ���LeadUpper״̬
*/
#define MMIIM_TOUCH_SMART_CAPS      //����Ӣ�Ĵ�Сд


#define VKB_SLIDING_THRESHOLD (5) //n�ɣ�Ҳ����ʮ��֮��
#define VKB_EFFECT_FRAME_CNT (12) //vkb��Ч����ʾ֡��
#define TIP_EXPAND_MULTIPLE ((float)1.6)     //tip�Ŵ󰴼��ı����������ʾ���£�������ֺ���ʾ
#define ABC_COMMIT_TICK (1100)   
#define TEXT_LEN_LIMIT (32)
#define PRINT_TEXT_LEN_LIMIT (TEXT_LEN_LIMIT)		//��ʱ�� TEXT_LEN_LIMIT����ʾ����ǰ�ڳ��򱣳�һ�£���Ȼʵ����ʾʱ�����ܾ�1~8���ַ� 
#define SUBTEXT_LEN_LIMIT (1)
#define CAND_PAGE_LIMIT (10) //��������10ҳ�����
#define LATIN_CANDIDATE_ROW (5)
#define LATIN_CANDIDATE_COUNT (32) //Ӣ�ĵ�ҳ������ʾ��������
#define CH_CANDIDATE_COL (4)
#define CH_CANDIDATE_ROW (4)
//���ֱ���20����ѡ�־Ϳ����� 4*4, ���ݾ���ļ��̲�����Ƹ���
#define CH_CANDIDATE_COUNT (CH_CANDIDATE_COL*CH_CANDIDATE_ROW)
//û��ƴ����ѡ����һ��, Ϊ��չģʽ(expand)
#define CH_CANDIDATE_COUNT_E (CH_CANDIDATE_COL*(CH_CANDIDATE_ROW + 1))
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#define HW_FULLSCREEN_EXPAND_CAND_COL 4
#define HW_FULLSCREEN_EXPAND_CAND_ROW 3
#define HW_FULLSCREEN_EXPAND_CAND_COUNT (HW_FULLSCREEN_EXPAND_CAND_COL * HW_FULLSCREEN_EXPAND_CAND_ROW)
#endif
#define PINYIN_SEPARATOR ('\'')
#define ASSO_PAGE_LIMIT (3)
#define PINYIN_CONFIRM_LIMIT (8) //����һ��ģ����ֵ�����Ѿ�ȷ������ô��֮�󣬾Ͳ�����������
#define ZHUYIN_COMMIT_TICK (1200)
#define ZHUYIN_CONFIRM_LIMIT (8) //����һ��ģ����ֵ�����Ѿ�ȷ������ô��֮�󣬾Ͳ�����������

#if defined IM_HANDWRITING_HANWANG
#define HW_TRACE_CNT_LIMIT (512)
#elif defined IM_HANDWRITING_SOGOU
#define HW_TRACE_CNT_LIMIT (512)
#endif
#define HW_RECOGNIZE_TICK (900)
#define HW_TRACE_WIDTH (2*MMIIM_HW_THICKNESS_MAX - 1)

#define CAND_WIDTH_MIN (32)

#define KEY_CHAR_LINE_MAX_CHARS_NUM (4)     //9�������ַ�����һ�������ʾ�ַ�����,�����������ַ�

#define VKB_PAGE_MAX	(4)		//һ�����뷨����������������ֵ

#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU) 
#define CHECK_DATA(data_ptr) \
    do {\
        SCI_ASSERT(PNULL != data_ptr); /*assert verified*/ \
        SCI_ASSERT(sizeof(IM_DATA_T) == data_ptr->check_info); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_term_imm); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_set_method); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_is_inputting); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_draw_all); /*assert verified*/ \
    } while (0)
#else
#define CHECK_DATA(data_ptr) \
    do {\
        SCI_ASSERT(PNULL != data_ptr); /*assert verified*/ \
        SCI_ASSERT(sizeof(IM_DATA_T) == data_ptr->check_info); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_term_imm); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_set_method); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_draw_all); /*assert verified*/ \
    } while (0)
#endif

#define CHECK_DATA_EX(data_ptr) \
    do {\
        CHECK_DATA(data_ptr); \
        SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->data); /*assert verified*/ \
    } while (0)





/*! @enum VKEY_CODE_TAG
@brief �����ֵ����
@note
- ���Ƽ�����
- ��ѡ��
- �������������ڶ��Ƽ��̵�ʱ��Ҫȷ���ģ��κ�һ�����̶��Ƕ�����Ƶ�
*/
enum VKEY_CODE_TAG
{
    VKEY_NULL,

    VKEY_1,
    VKEY_2,
    VKEY_3,
    VKEY_4,
    VKEY_5,
    VKEY_6,
    VKEY_7,
    VKEY_8,
    VKEY_9,
    VKEY_0,

    VKEY_A,
    VKEY_B,
    VKEY_C,
    VKEY_D,
    VKEY_E,
    VKEY_F,
    VKEY_G,
    VKEY_H,
    VKEY_I,
    VKEY_J,
    VKEY_K,
    VKEY_L,
    VKEY_M,
    VKEY_N,
    VKEY_O,
    VKEY_P,
    VKEY_Q,
    VKEY_R,
    VKEY_S,
    VKEY_T,
    VKEY_U,
    VKEY_V,
    VKEY_W,
    VKEY_X,
    VKEY_Y,
    VKEY_Z,

    //�˴�ΪĳЩ���Բ�ʹ����ͨ���̰������ã����޷�ʹ��abc�ȼ�����ʾ����
    //���һЩ�����򰴼�����
    //�˰���������ABCֵ��ͬ����ֹ�绰����ѯ��ƫ�����ȳ�������
    VKEY_IRREGULAR = VKEY_A,        
    VKEY_IRREGULAR_1 = VKEY_IRREGULAR,      //VKEY_A
    VKEY_IRREGULAR_2,
    VKEY_IRREGULAR_3,
    VKEY_IRREGULAR_4,
    VKEY_IRREGULAR_5,
    VKEY_IRREGULAR_6,
    VKEY_IRREGULAR_7,
    VKEY_IRREGULAR_8,
    VKEY_IRREGULAR_9,
    VKEY_IRREGULAR_10,
    VKEY_IRREGULAR_11,
    VKEY_IRREGULAR_12,
    VKEY_IRREGULAR_13,
    VKEY_IRREGULAR_14,
    VKEY_IRREGULAR_15,
    VKEY_IRREGULAR_16,
    VKEY_IRREGULAR_17,
    VKEY_IRREGULAR_18,
    VKEY_IRREGULAR_19,
    VKEY_IRREGULAR_20,
    VKEY_IRREGULAR_21,
    VKEY_IRREGULAR_22,
    VKEY_IRREGULAR_23,
    VKEY_IRREGULAR_24,
    VKEY_IRREGULAR_25,
    VKEY_IRREGULAR_26,          //VKEY_Z
    VKEY_IRREGULAR_27,
    VKEY_IRREGULAR_28,
    VKEY_IRREGULAR_29,
    VKEY_IRREGULAR_30,
    VKEY_IRREGULAR_31,
    VKEY_IRREGULAR_32,
    VKEY_IRREGULAR_33,
    VKEY_IRREGULAR_34,
    VKEY_IRREGULAR_35,
    VKEY_IRREGULAR_36,
    VKEY_IRREGULAR_37,
    VKEY_IRREGULAR_38,
    VKEY_IRREGULAR_39,
    VKEY_IRREGULAR_40,
    VKEY_IRREGULAR_MAX = VKEY_IRREGULAR + 40,

    VKEY_SPACE,
    VKEY_DEL,
    VKEY_ENTER,
    VKEY_CAPS,
    VKEY_123,
    VKEY_SYMBOL,
    VKEY_123SYMBOL,
    VKEY_SWITCH,
    VKEY_SET,
    VKEY_LANGUAGE_SET,  //�������Ӣ��֮���л�
    VKEY_HIDE,
    VKEY_EXPAND,
    VKEY_PAGEUP,
    VKEY_PAGEDOWN,
    VKEY_PY_PAGEUP,
    VKEY_PY_PAGEDOWN,
    VKEY_PY_SEPARATE,
    VKEY_FACE,
    VKEY_BACK,
    VKEY_LOCK,
    VKEY_PAGE,
    VKEY_DOT,       //���ֵ�С����
    VKEY_PERIOD,    //��ţ������ݱ��period��ͬ���������С������һ�����֣��������
    VKEY_PLUS,
    VKEY_STAR,
    VKEY_HASH,	//#
    VKEY_POUND,
    VKEY_MULTI_CHAR,    //���ְ�������Ϊ�������������밴���ϵĲ�ͬ�ַ�
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    VKEY_WIN_PANEL,     //��ʾ�������򰴼���������д��嵲ס���ڵĲ���
#endif
    VKEY_WRITE_PANEL,
	VKEY_SWITCH_VKB,	//���ż���֮���л�
	
    VKEY_CUSTOM,
    VKEY_CUSTOM_MAX = VKEY_CUSTOM + 10,

    VKEY_CANDIDATE,
    VKEY_CANDIDATE_MAX = VKEY_CANDIDATE + 64,

    VKEY_PY_CANDIDATE,
    VKEY_PY_CANDIDATE_MAX = VKEY_PY_CANDIDATE + 40,

    VKEY_SYMB_STR,
    VKEY_SYMB_STR_MAX = VKEY_SYMB_STR + 40,
};
typedef uint16 VKEY_CODE_T;



enum EDIT_STATE_TAG
{
    EDIT_STATE_NONE,
    EDIT_STATE_SEARCH,
    EDIT_STATE_WRITE,
    EDIT_STATE_COMMIT_DEFAULT,
    EDIT_STATE_ASSO,
};
typedef uint8 EDIT_STATE_T;


#if defined MMIIM_TOUCH_SMART_CAPS
enum CAPS_SWITCH_TYPE_E_
{
    CAPS_SWITCH_TYPE_NONE_E,    //��ʼ��״̬����Сд�ɳ�ʼ�������ģ���״̬û������
    CAPS_SWITCH_TYPE_USER_E,      //�û��Լ��л��Ĵ�Сд
    CAPS_SWITCH_TYPE_COMMIT_E,    //�ύ�ַ��󣬷�����Сд�л�
    CAPS_SWITCH_TYPE_CURSOR_E,    //����ƶ��󣬷�����Сд�л�

    CAPS_SWITCH_TYPE_MAX_E,
};
typedef uint8 CAPS_SWITCH_TYPE_E;
#endif



/*! @enum KEY_STYLE_TAG
@brief �����ķ��࣬���ǽ�panel�ϵ�item�����óɰ���
*/
enum KEY_STYLE_TAG
{
    KEY_STYLE_CHARACTER,        /*!< һ����ַ����� */
    KEY_STYLE_CONTROL,          /*!< һ��Ŀ��ư��� */
    KEY_STYLE_CUSTOM,           /*!< ���Ƶİ��� */
    KEY_STYLE_CANDIDATE,        /*!< ��ѡ���� */
    KEY_STYLE_SYSTEM,           /*!< ϵͳ���Ŀ��ư������Ƚ�����Ŀ��ư��� */
    KEY_STYLE_ART,              /*!< װ�β��� */
};
typedef uint8 KEY_STYLE_T;


//key add style-character,�ַ�����˿ӡ���ݴ����ݱ��ȡ������ʾʱ����ռ䲻��������ԭ���޷���������Ҫ��
//�����һ���ֶα�ʾ������
//��ͻ�����϶࣬������һ�����:��0~2λ��ʾtext�ֶ����ԣ�3,4λ��ʾsub_text����

#define KASC_TEXT_ATTR (7)		//((1 << 3) - 1)
#define KASC_SUBTEXT_TEXT_ATTR (24)	//((1 << 5) - KASC_TEXT_ATTR)	//

typedef enum _KEY_ADD_STYLE_CHARACTER_E
{
	//���������Ĭ�ϱ�ʾsubtext�������Ͻ���ʾ
    KASC_EXCLUDE_DIGIT,      //������ʾ�����ַ�������������һ���ַ�Ϊ���֣�����ʾ����
    KASC_ALL_TEXT,      //������ʾ�����ַ�����Ҫ������subtext�����ڵ����

    KASC_FIRST,      //��ʾ��һ���ַ�
    KASC_SELECT_CHAR,       //��ʾ���е��ַ����ֿ���ʾ������һ�������ַ���ʾѡ��״̬�������ַ�Ҳ���ܲ�����

    KASC_2LINE_EXCLUDE_DIGIT,      //������ʾ�����ַ�������������һ���ַ�Ϊ���֣�����ʾ����
    KASC_2LINE,      //������ʾ�����ַ�

    KASC_2X4_EXCLUDE_DIGIT,      //������ʾ��ÿ�����4���ַ�������������һ���ַ�Ϊ���֣�����ʾ����
    KASC_2X4,      //������ʾ��ÿ�����4���ַ�

	//��������α�ʾsubtext������ʾ�����������text��ʾ��ǰ������ʹ��
	KASC_SUBTEXT_LEFT = 0,		//Ĭ��״̬��subtext�������ʾ
	KASC_SUBTEXT_MIDDLE = 8,	//�����ǰ��ķ�����ϣ���ʾͬʱ֧�֣�������Ӧ�ò�Ҫ��2�еĻ��ʹ�ã�����һ��������ʾ3�л����غϣ���������
	KASC_SUBTEXT_RIGHT = 24,	//�����ǰ��ķ�����ϣ���ʾͬʱ֧�֣����������ʱû�д�����������Ҫ�����
	
} KEY_ADD_STYLE_CHARACTER_E;

//#if defined MMIIM_HW_FULLSCREEN_SUPPORT
//key add style-art:װ�β���û����һ�����֣�ĳЩ������װ�β��ֿ��ܲ�ͬ
//�����һ���ֶα�ʾ������
typedef enum _KEY_ADD_STYLE_ART_E
{
    KASA_DEFAULT_E,

    KASA_TRANSPARENT_E,     //��װ�ΰ�������͸��ɫ
    KASA_NOT_DRAW_E,     //��װ�ΰ���������屳��ɫ
    KASA_NOT_BG_E,     //��װ�ΰ�������䱳��
    
    KASA_MAX_E,
}KEY_ADD_STYLE_ART_E;
//#endif

typedef union _KEY_ADD_STYLE_T
{
    KEY_ADD_STYLE_CHARACTER_E text_disp;       //text �ַ�����ʾ��ʽ
//#if defined MMIIM_HW_FULLSCREEN_SUPPORT	
    KEY_ADD_STYLE_ART_E art_disp;       //art������ʾ��ʽ
//#endif	    
} KEY_ADD_STYLE_T;


/*! @struct KEY_THEME_TAG
@brief ��������ʾ����
*/
typedef struct KEY_THEME_TAG
{
    SHARP_T sharp;              /*!< ��״ */
    BORDER_T border;            /*!< �߿� */
    FILL_T fill;                /*!< ������� */
    FILL_DATA_T fill_data;      /*!< ������� */
    uint32 text_color;          /*!< ��������ɫ */
    uint32 subtext_color;       /*!< ��������ɫ */
    GUI_FONT_T text_font;       /*!< ���������� */
    GUI_FONT_T subtext_font;    /*!< ���������� */
} KEY_THEME_T;

/*! @struct MMIIM_TOUCH_THEME_TAG
@brief ���뷨������ʾ����
*/
typedef struct MMIIM_TOUCH_THEME_TAG
{
    //general define
    uint8 alpha;                            /*!< ����ȫ��alpha */
    GUI_FONT_T font_min;                    /*!< ��С���� */
    GUI_FONT_T font_max;                    /*!< ������� */

    //panel define
    SHARP_T panel_sharp;                    /*!< ������״ */
    uint8 panel_percent;                /*!< ���̴�С */
    uint32 panel_color;                     /*!< ������ɫ */
    BORDER_T panel_border;                  /*!< ���̱߿� */

    //key define
    //1 key size follow panel size
    uint8 key_hor_space;                        /*!< ����ˮƽ�հ� */
    uint8 key_ver_space;                        /*!< ���䴹ֱ�հ� */  
    uint8 cand_hor_space;                        /*!< ��ѡ��ˮƽ�հ� */ 
    uint8 cand_ver_space;                        /*!< ��ѡ�ִ�ֱ�հ� */ 
    uint8 key_padding;                      /*!< ���ڿհ� */
    KEY_THEME_T key_char[KEY_STATE_COUNT];  /*!< �ַ����� */
    KEY_THEME_T key_ctrl[KEY_STATE_COUNT];  /*!< ���ư��� */
    KEY_THEME_T key_cust[KEY_STATE_COUNT];  /*!< ���ư��� */
    KEY_THEME_T key_cand[KEY_STATE_COUNT];  /*!< ��ѡ���� */
    KEY_THEME_T key_sys[KEY_STATE_COUNT];   /*!< ϵͳ���� */
    KEY_THEME_T key_art[KEY_STATE_COUNT];   /*!< װ�ΰ���������װ�ΰ�����������״̬���岻ͬװ�� */

    //tip define
    //1 tip size follow panel size
    //2 key tip theme follow key theme
    KEY_THEME_T tip_theme;                  /*!< ����tip��ʾ */
    uint8 tip_padding;                      /*!< ��ʾ���ڿհ� */
    uint8 tip_alpha;                        /*!< tip���ر�͸������ */
    uint16 tip_delay;                       /*!< tip�ӳ���ʧ��ʱ�� */

    //��ʾ��Ϣ��ʧʱ��
    uint16 cue_delay;                       /*!< cue�ӳ���ʧ��ʱ�� */

    //edit define
    // 1 edit size follow panel size
    // 2 edit ���ü򵥵ķ���д�ֵ���ʽ
    uint32 edit_border_color;               /*!< ��������ɫ */
    uint32 edit_bg_color;                   /*!< ��������ɫ */
    uint32 edit_text_color;                 /*!< ��������ɫ */
    GUI_FONT_T edit_text_font;              /*!< ���������� */
    uint8 edit_padding;                     /*!< edit���ڲ����� */
} THEME_T;


enum KEY_TAG_TAG
{
    KEY_TAG_NONE = 0x00,
    KEY_TAG_NORMAL = 0x01,
    KEY_TAG_EXPAND = 0x02,
};
typedef uint8 KEY_TAG_T;


typedef BOOLEAN(*KEY_TP_FUNC_T)(IM_DATA_T *, KEY_INFO_T *);
typedef BOOLEAN(*KEY_DRAW_T)(IM_DATA_T const *, GUILCD_DRAW_DC_T const *, KEY_INFO_T *);

/*! @struct KEY_INFO_TAG
@brief ��������Ϣ
@note
- Ϊ��������key֮��ľ���һ�£����ǽ�ͳһʹ��"���"���ǣ������ǵ�right���ұߵ�һ����left

subtext
---------
icon text

*/
struct KEY_INFO_TAG
{
    GUI_RECT_T rect;                    /*!< ��panel�ϵ����λ�ã�panel�������СΪ1000*1000 */
    uint16 code;                        /*!< �����Ĵ��� */
    uint8 is_inactive;                  /*!< �Ƿ񼤻� */
    uint8 is_press;                     /*!< �Ƿ��� */
    uint8 is_hide;                      /*!< ʱ������ */
	uint8 is_paint;                      /*!< if true, will paint, or not paint this key when paint the keyboard*/
    uint8 has_tip;                      /*!< �Ƿ���ʾtips */
    KEY_TAG_T tag;                      /*!< ����ǩ�����ڱ�ʶ������; */
    //display style
    KEY_STYLE_T style;                  /*!< ���ͣ��ᰴ�����;�����ʾ������ */
    KEY_ADD_STYLE_T add_style;                  /*!< �������ͣ��ᰴ�����;����������ͣ��������;�����ʾ������ */
    //display data
    MMI_IMAGE_ID_T icon;                /*!< ͼ�� */
    wchar text[TEXT_LEN_LIMIT];         /*!< �ַ� ����������ص��ַ���������ѯ������ʹ�õ�unicode���ύ���ַ�Ҳ������ֶζ�ȡ�������Ǵ�display�ֶζ�ȡ*/
    wchar print_text[PRINT_TEXT_LEN_LIMIT];         /*!< ��ʾ�ַ��� ����text�������ǣ�����ַ�����������������ʾ*/
    wchar subtext[SUBTEXT_LEN_LIMIT];   /*!< �����ַ� */
    uint8 text_len;                     /*!< �ַ����� */
    uint8 print_text_len;                     /*!< ��ʾ�ַ����� */
    uint8 subtext_len;                  /*!< �����ַ����� */
    int8 font_size;                     /*!< �����С�ĵ��� */
    //key function
    KEY_TP_FUNC_T f_down;
    KEY_TP_FUNC_T f_move;
    KEY_TP_FUNC_T f_long;
    KEY_TP_FUNC_T f_up;
    KEY_DRAW_T f_draw;  //Ĭ�ϰ������ƺ�������DrawKey���������Ҫ���ã����д���
};

enum CAPITTAL_MODE_TAG
{
    CAPITTAL_MODE_LOWER,
    CAPITTAL_MODE_UPPER,
    CAPITTAL_MODE_INITIAL,
};
typedef uint8 CAPITTAL_MODE_T;


/*! @enum KEYBOARD_STATE_TAG
@brief ����״̬�����ڼ�¼���̵ļ�ʱ��Ϣ
*/
enum KEYBOARD_STATE_TAG
{
    KEYBOARD_INITIAL = 0x0000,
    KEYBOARD_CANDIDATE = 0x0001,
    KEYBOARD_ASSOCIATE = 0x0002,
    KEYBOARD_CANDIDATE_EXPAND = 0x0004,
    KEYBOARD_ASSOCIATE_EXPAND = 0x0008,
    KEYBOARD_CHOOSE_NUMBER = 0x0010,
    KEYBOARD_CHOOSE_SYMBOL = 0x0020,
    KEYBOARD_CHOOSE_NUMBER_LOCK = 0x0040,
    KEYBOARD_CHOOSE_SYMBOL_LOCK = 0x0080,
    KEYBOARD_CHOOSE_LETTER = 0x0100,        //����������밴����ѡ���ѡ�ַ�״̬
    KEYBOARD_CHOOSE_MENU = 0x0200,        //����iphone�˵�ѡ��״̬
};

//ȫ���̲�������
typedef enum _QWERT_KEYBOARD_LAYOUT_TYPE_E
{
    KEYBOARD_LAYOUT_26CHAR,     //KEYBOARD_LAYOUT_QWERT_DEFAULT,
    KEYBOARD_LAYOUT_28CHAR,   
    KEYBOARD_LAYOUT_40CHAR, 

	KEYBOARD_LAYOUT_ERR, 
}QWERT_KEYBOARD_LAYOUT_TYPE_E;

typedef uint32 KEYBOARD_STATE_T;

typedef BOOLEAN(*KB_SLIDING_FUNC_T)(IM_DATA_T*, int32);

/*! @struct VKEYBOARD_TAG
@brief ������̵�����
*/
struct VKEYBOARD_TAG
{
    //key board define
    uint8 key_cnt;                              /*!< ������������һ���Ӵ������еİ��������ز��ְ��� */
    KEY_INFO_T *keys;                           /*!< ���������Ŀռ䣬�򻯲��� */
    //kb state
    CAPITTAL_MODE_T cap;
    KEY_INFO_T *caps_key;
    KEY_INFO_T *hide_key;
    KEY_INFO_T *expand_key;
    KEY_INFO_T *vessel_key;
    KEY_INFO_T *pgup_key;
    KEY_INFO_T *pgdn_key;
    KEY_INFO_T *py_vessel_key;
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    KEY_INFO_T *bg_key;
#endif    
    KEY_INFO_T *py_pgup_key;
    KEY_INFO_T *py_pgdn_key;
    KEY_INFO_T *back_key;
    KEY_INFO_T *lock_key;
    KEY_INFO_T *tip_key;
    KEYBOARD_STATE_T state;
	uint8 is_paint;				//need to paint
	uint8 charactor_first;		//ʹ�õ��������ݱ��еĵ�һ��������indexֵ
    uint8 custom_first;
    uint8 custom_cnt;
    uint8 candidate_first;
    uint8 candidate_cnt;
    uint8 py_candidate_first;
    uint8 py_candidate_cnt;
    GUI_POINT_T down_point;                     /*!< ���ʰ��µ�λ�� */
    KEY_INFO_T *first_key;                      /*!< ����down�İ��� */
    KEY_INFO_T *last_key;                       /*!< ���İ��� */
    uint8 is_key_block;                         /*!< ���̱�������last key�������Ӧ�ã�������д */
    uint8 is_expand;
    //funtion
    KB_SLIDING_FUNC_T f_sliding;
};


/* ���ô��ڵ�������Ϣ */
typedef struct
{
    GUIIM_LANGUAGE_T lang;
    GUIIM_TYPE_T type;
    GUIIM_INPUT_TYPE_T input;
    MMI_TEXT_ID_T text_id;
} SETTING_MENU_DATA_T;


typedef struct MULTI_TAP_TAG
{
    KEY_INFO_T      *last_key;
    uint8           last_key_index;	//���������Ĵ��� - �����ַ����� * ѭ������
    uint8           commit_timer;
} MULTI_TAP_T;


//ĳЩ���뷨���ڶ��ż��̵�������ڶ�������£���ֵ��Ч��������������Ҫ�������
//������һ��Լ�������뷨�����������������ô��ڵȣ����غ󣬻��ǽ����һ�ż��̣�
//������Ʒ������Լ��ټ���ҳ���������⣬iphoneҲ���������
typedef struct VKB_MULTI_PAGE_TAG
{
	uint8           total_page;	//��ǰ���뷨���ڼ��̵�����
    uint8           cur_page;	//��ǰ���̵ĵڼ�ҳ����0��ʼ����
    uint16           reserved;	//��ǰ���̵ĵڼ�ҳ
	QWERT_KEYBOARD_LAYOUT_TYPE_E layout[VKB_PAGE_MAX];
} VKB_MULTI_PAGE_T;


//���뷨ѡ��˵�������ʽ
typedef enum _E_IM_SET_SELECT_TYPE 
{
    EISST_NONE,
    EISST_MOVE_SELECT,     //�����moveѡ��˵�������iphone������������Ĳ˵�
    EISST_CLICK_SELECT,    //����󣬵���ѡ��˵����ٴε��    ѡ��˵��������Ҽ��˵��Ĺ���
} E_IM_SET_SELECT_TYPE;

typedef void (*F_TermImm_T)(IM_DATA_T *data_ptr);
typedef BOOLEAN(*F_SetMethod_T)(IM_DATA_T *data_ptr, GUIIM_METHOD_T method);
typedef BOOLEAN(*F_IsInputting_T)(IM_DATA_T *data_ptr);
typedef BOOLEAN(*F_DrawAll_T)(IM_DATA_T *data_ptr);

/*! @struct DATA_TAG
@brief ���뷨����
*/
struct IM_DATA_TAG
{
    uint32                  check_info;         /*!< ���ݵļ����Ϣ�����ڼ򵥵����ݼ�� */
    CAF_HANDLE_T            ctrl_handle;        /*!< ���뷨��� - ���뷨�ؼ���Ϣ */
    GUIIM_INIT_KERNEL_T     init_param;         /*!< ��ʼ������ - ���뷨�ؼ���Ϣ */

    THEME_T                 theme;              /*!< ��ʾ���� */

    GUI_LCD_DEV_INFO        ui_layer;           /*!< ��Ҫ��������ʾ�� */
    GUI_LCD_DEV_INFO        tip_layer;          /*!< ��ʾ������ʾ�� */
    GUI_LCD_DEV_INFO        edit_layer;         /*!< Ԥ�༭������ʾ�� */
    GUI_LCD_DEV_INFO        setting_im_layer;   /*!< �ı����뷨��ʾ�� */

    GUI_RECT_T              tip_rect;           /*!< tip��Ĵ�С��λ����Ϣ */
    GUI_RECT_T              edit_rect;          /*!< Ԥ�༭��Ĵ�С��λ����Ϣ */

    BOOLEAN                 is_tip_display;     /*!< ��ʾ���Ƿ���ʾ */
    BOOLEAN                 is_setting_display; /*!< �л����뷨�Ƿ���ʾ */
    uint8                   tip_hide_timer;     /*!< ��ʾ������ض�ʱ�� - ��ʾ��Ҫ�ӳ���ʧ */

    MMIIM_KEYBOARD_SIZE_T   kb_size;            /*!< ���뷨���Ĵ�С����nv�� */
    GUI_RECT_T              rect;               /*!< ���뷨�Ŀ�������Ҳ�������Ĵ�С */
    GUI_RECT_T              rect_disp_tips;               /*!< tips������ʾ�ַ�����*/

    BOOLEAN                 is_symbol_panel;    /*!< �Ƿ�����˷�������״̬ */
    VKEYBOARD_T             vkb;                /*!< ������� */
	VKB_MULTI_PAGE_T 		vkb_multi_page;			/*!ĳЩ���Դ��ڶ��ż��̵����*/
    void*                   data;               /*!< ���뷨�ĺ������� */
    MULTI_TAP_T multi_tap_data;    /*����������������*/   
    
    GUI_POINT_T             first_tp;           /*!< tp�ĵ�һ�� */
    GUI_POINT_T             last_tp;            /*!< tp�����һ�� */

    MMI_HANDLE_T            more_setting_win_handle;/*!< �������õĴ��ھ�� */
    BOOLEAN                 is_in_effect_enabled;   /*!< ��ʾ��Ч�Ƿ�� */
    BOOLEAN                 is_out_effect_enabled;  /*!< ��ʾ��Ч�Ƿ�� */

    SETTING_MENU_DATA_T const *setting_menu_data;   /*!< ���ò˵������� */
    uint32                  setting_menu_size;      /*!< ���ò˵��Ĵ�С */

    F_TermImm_T             f_term_imm;         /*!< �������� */
    F_SetMethod_T           f_set_method;       /*!< �������뷨 */
    F_IsInputting_T         f_is_inputting;     /*!< ��ȡ����״̬ */
    F_DrawAll_T             f_draw_all;         /*!< ˢ��ui */

    //���뷨�л���ʾ����timer
    uint8 cue_hide_timer_id;
    
    //for setting im in istyle
    BOOLEAN 				settingim_halfposition; 	/*!< �л����뷨�а����ʾ��ȫʱ���Ϸ������·���ʾ��ȫ */
    E_IM_SET_SELECT_TYPE    setting_isst_e;     /*�ƶ������˵���orֱ�ӵ�������˵�*/
	uint8 					settingim_timer_id;			/*!< �л����뷨timer id*/
    int16 					settingim_top_item;			/*!< �л����뷨������ʾ����ǰ��ʾ�ĵ�һ��itemʵ�ʵ�index*/
    int16 					settingim_last_highlight;	/*!< �л����뷨���һ��������ʾ��item */
    int16 					settingim_item_count ;	/*!< setting_menu_index����Ŀ*/
    int16 * 				settingim_menu_index ; 		/*!< save actual im language index of setting_menu_data*/
};


#ifdef __cplusplus
extern "C"
{
#endif


extern THEME_T const g_theme_thumb_default;
extern THEME_T const g_theme_qwert_default;
extern THEME_T const g_theme_symbol_default;
extern THEME_T const g_theme_sys_default;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
extern THEME_T const g_hw_fullscreen_theme_default;
#endif
extern GUI_RECT_T const g_thumb_panel_position[];
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
extern GUI_RECT_T const g_hw_fullscreen_position[];
#endif
extern uint32 const g_thumb_panel_position_size;
extern GUI_RECT_T const g_thumb_panel_position_adjust_page_icon[];
extern uint32 const g_thumb_panel_position_adjust_page_icon_size;
extern uint8 const g_thumb_panel_expand_page_cnt;
extern uint8 const g_thumb_panel_expand_py_cnt;
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
extern wchar const * const g_thumb_cust_cn_symb;
#endif
extern wchar const * const g_thumb_cust_symb;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
extern wchar const * const g_hw_fullscreen_cust_cn_symb;
#endif
extern wchar const * const g_hw_fullscreen_cust_symb;
#endif

extern wchar const *g_thumb_key_disp[];
extern uint8 const g_thumb_key_disp_len[];
extern char const *g_thumb_data_lower[];
extern char const *g_thumb_data_upper[];
extern GUI_RECT_T const g_stroke_panel_position[];
extern uint32 const g_stroke_panel_position_size;
extern wchar const * const g_stroke_symb;
extern GUI_RECT_T const g_qwert_panel_position[];
extern uint32 const g_qwert_panel_position_size;
extern uint8 const g_qwert_panel_expand_page_cnt;
extern GUI_RECT_T const g_digit_panel_position[];
extern uint32 const g_digit_panel_position_size;
extern GUI_RECT_T const g_sys_panel_position[];
extern uint32 const g_sys_panel_position_size;

extern GUIIM_TYPE_T g_default_ch_type;
extern GUIIM_TYPE_T g_default_en_type;
extern GUIIM_INPUT_TYPE_T g_default_input_type;
//������дǰ����¼����д���뷽ʽ����ֹ��дʱ���л����ԣ��������뷽ʽs_default_input_type��¼����
extern GUIIM_INPUT_TYPE_T g_entry_hw_input_type;
#if defined MMIIM_TOUCH_SMART_CAPS
extern CAPS_SWITCH_TYPE_E g_caps_switch_type;
#endif
extern uint16 const g_kb_keymap[];
extern uint32 const g_kb_keymap_size;
extern int32 g_select_letter_index;
extern const wchar normal_mul_sym[TEXT_LEN_LIMIT];  
extern const wchar normal_9key_mul_sym[TEXT_LEN_LIMIT];
#if defined IM_ARABIC_SUPPORT
extern const wchar arabic_mul_sym[TEXT_LEN_LIMIT];
#endif

PUBLIC __inline uint16 RGB888TO565(uint32 rgb888)
{
    unsigned int r888 = (rgb888 & 0xff0000) >> 16;
    unsigned int g888 = (rgb888 & 0x00ff00) >> 8;
    unsigned int b888 = (rgb888 & 0x0000ff);
    unsigned int r565 = r888 >> 3;
    unsigned int g565 = g888 >> 2;
    unsigned int b565 = b888 >> 3;
    unsigned int rgb565 = (r565 << (5 + 6)) | (g565 << 5) | b565;

    return rgb565;
}


PUBLIC __inline uint32 RGB565TO888(uint16 rgb565)
{
    unsigned int r565 = (rgb565 >> 11) & 0x1f;
    unsigned int g565 = (rgb565 >> 5) & 0x3f;
    unsigned int b565 = rgb565 & 0x1f;
    unsigned int r888 = r565 << 3;
    unsigned int g888 = g565 << 2;
    unsigned int b888 = b565 << 3;
    unsigned int rgb888 = (r888 << 16) | (g888 << 8) | b888;

    return (rgb888 | 0xff000000); //set max alpha
}




PUBLIC __inline void ConvertRect(
    int16 width,
    int16 height,
    GUI_RECT_T const *relative,
    GUI_RECT_T *absolute,
    uint8 hor_space,
    uint8 ver_space)
{
    SCI_ASSERT(PNULL != relative); /*assert verified*/
    SCI_ASSERT(PNULL != absolute); /*assert verified*/

    absolute->left = (relative->left - 1 + hor_space / 2) * width / 1000;
    absolute->right = (relative->right - hor_space / 2)* width / 1000;
    absolute->top = (relative->top - 1 + ver_space *7 / 10) * height / 1000;   
    absolute->bottom = (relative->bottom - ver_space * 3 / 10)* height / 1000;   //�ײ�����Щ����

    return;
}



#if defined MMIIM_HW_FULLSCREEN_SUPPORT
/*==============================================================================
Description: ����������ת��Ϊ������Ļ����
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC __inline void HwFsConvertRect(
    int16 width,
    int16 height,
    GUI_RECT_T const *relative,
    int16 offset_y,     //ƫ���������ص����������index
    GUI_RECT_T *absolute,
    uint8 hor_space, 
    uint8 ver_space)
{
    if (PNULL == relative || PNULL == absolute)
    {
        SCI_TraceLow ("HwFsConvertRect");
        return;
    }

    absolute->left = (relative->left - 1 + hor_space / 2) * width / 1000;
    absolute->right = (relative->right - hor_space / 2)* width / 1000;
    absolute->top = (relative->top - 1 + ver_space * 7 / 10) * height / 1000 + offset_y;
    absolute->bottom = (relative->bottom - ver_space * 3 / 10) * height / 1000 + offset_y - 1;

    return;
}
#endif


PUBLIC __inline char UpperCase(char c)
{
    return (('a' <= c && c <= 'z') ? (c - 'a' + 'A') : c);
}


PUBLIC __inline char LowerCase(char c)
{
    return (('A' <= c && c <= 'Z') ? (c - 'A' + 'a') : c);
}


PUBLIC __inline void DcToDstStr(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUISTR_BUF_T *des_str_buf
)
{
    SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != des_str_buf); /*assert verified*/

    des_str_buf->buffer = dc_data_ptr->lcd_buffer_ptr;
    des_str_buf->mem_width = dc_data_ptr->mem_width;
    des_str_buf->width = dc_data_ptr->width;
    des_str_buf->height = dc_data_ptr->height;
#ifdef MMIIM_LAYER_565
    des_str_buf->type = GUISTR_BUF_RGB565;
#else
    des_str_buf->type = GUISTR_BUF_ARGB888;
#endif
    return;
}

PUBLIC BOOLEAN HasConfigSelectInputType(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_TYPE_T type);
PUBLIC BOOLEAN HasConfigSelectInput(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_INPUT_TYPE_T input);


PUBLIC KEY_INFO_T *FindKey(
    IM_DATA_T *data_ptr,
    GUI_POINT_T *point_ptr
);
PUBLIC KEY_INFO_T *FindKeyWithCode(
    IM_DATA_T *data_ptr,
    uint16 code
);
PUBLIC BOOLEAN SlidingPanel(IM_DATA_T *data_ptr, int32 dir);
/* settings */
PUBLIC MMI_HANDLE_T CreateSettingWin(IM_DATA_T *data_ptr);
PUBLIC MMI_RESULT_E HandleSettingWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
PUBLIC BOOLEAN SettingMethod(
    IM_DATA_T *data_ptr,
    GUIIM_LANGUAGE_T lang,
    GUIIM_TYPE_T type,
    GUIIM_INPUT_TYPE_T input
);
PUBLIC void ResetMethod(IM_DATA_T *data_ptr);
PUBLIC GUIIM_METHOD_T GetMethodBySwitchLang(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T lang);

/* ui */
/*==============================================================================
Description: Because the speed is too slow when use etrump font. 
    			IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key paint status    			
Global resource dependence: NONE
Author: yaoguang.chen
Note:   
==============================================================================*/
PUBLIC void MMIIM_SetVKeyPaintStatus (KEY_INFO_T *key_ptr, BOOLEAN is_paint);

/*==============================================================================
Description: Because the speed is too slow when use etrump font. 
    			IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key paint status  
		All keys need to paint, but when is hide status, it won't paint.
Global resource dependence: NONE
Author: yaoguang.chen
Note:   
==============================================================================*/
PUBLIC void MMIIM_SetVkbPaintStatus (IM_DATA_T *data_ptr, BOOLEAN is_paint);

PUBLIC BOOLEAN DrawVkb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN MMIIM_DrawAll(IM_DATA_T *data_ptr);
PUBLIC int32 GetThemeIndex(KEY_INFO_T *key_ptr);
PUBLIC void DrawKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC BOOLEAN DrawLangSwitchKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC void DrawKeyRaw(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
);

PUBLIC void DrawKeySelectLetter(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,         //tip_rect
    KEY_INFO_T *key_ptr
);

/* tip */
/*==============================================================================
Description: ���ư��� - ��֪theme
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void DrawTipKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC void ShowTip(IM_DATA_T *data_ptr,  KEY_INFO_T *key_ptr);
PUBLIC void HideTip(IM_DATA_T *data_ptr);
PUBLIC void HideTipCB(uint8 timer_id, uint32 param);

/* ��ʾ��Ĵ������ͷ� */
PUBLIC BOOLEAN CreateUILayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseUILayer(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN CreateTipLayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseTipLayer(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN CreateEditLayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseEditLayer(IM_DATA_T *data_ptr);

/* ������̺��� */
/* ���� */
PUBLIC void InitVkbCaps(IM_DATA_T *data_ptr);
PUBLIC void SetVkbNextCaps(IM_DATA_T *data_ptr);
PUBLIC void SetVkbCaps(IM_DATA_T *data_ptr, CAPITTAL_MODE_T cap);
PUBLIC void SetHideKey(IM_DATA_T * data_ptr, uint8 cand_cnt);
PUBLIC void SwitchExpandKey(IM_DATA_T *data_ptr);
PUBLIC void ReleaseVkb(VKEYBOARD_T *vkb_ptr);
PUBLIC void SetPanel(IM_DATA_T *data_ptr);
PUBLIC uint16 GetPanelKeyRowNum (IM_DATA_T *data_ptr);

/* ���� */
PUBLIC BOOLEAN VkbDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpDeal(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void HidePanel(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN VkeyUpHide(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyLongSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyLongDel(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeySetDown (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpLangSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN PanelSlideCueCloseByTimer (IM_DATA_T *data_ptr);
PUBLIC void PanelSlideCueCloseCB (uint8 timer_id, uint32 param);
PUBLIC BOOLEAN PanelSlideCueClose (IM_DATA_T *data_ptr, BOOLEAN immediately_paint);

/* ����ύ */
PUBLIC void CommitChar(IM_DATA_T *data_ptr, wchar c);
PUBLIC void CommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len);
PUBLIC BOOLEAN CapsCommitChar(IM_DATA_T *data_ptr, wchar c);
PUBLIC BOOLEAN CapsCommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len);
#if defined MMIIM_TOUCH_SMART_CAPS
PUBLIC void SetCapsSwitchType (CAPS_SWITCH_TYPE_E switch_type);
PUBLIC CAPS_SWITCH_TYPE_E GetCapsSwitchType ();
PUBLIC BOOLEAN DialogSeperateChar (wchar c);
PUBLIC BOOLEAN WordSeperateChar (wchar c);    
#endif



PUBLIC void SetMethodSelectMenuDefaultIndex (int32 index);
PUBLIC int32 GetMethodSelectMenuDefaultIndex ( );
PUBLIC void SetLetterSelectIndex(int32 index);
PUBLIC int32 GetLetterSelectIndex();
PUBLIC int32 MMIIM_MethodSettingMenuDown (IM_DATA_T *    data_ptr, GUI_POINT_T point, BOOLEAN repaint);
PUBLIC int32 RefreshTipLetterSelectIndex(IM_DATA_T *data_ptr, GUI_POINT_T *point_ptr, BOOLEAN repaint);
PUBLIC BOOLEAN ChooseLetter(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC const wchar *  GetMultiSymCharPtr(GUIIM_LANGUAGE_T lang);
PUBLIC BOOLEAN MultiCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC QWERT_KEYBOARD_LAYOUT_TYPE_E QwertKbLayoutType (GUIIM_METHOD_T method, uint8 cur_page);
//PUBLIC THEME_T QwertKbGetTheme (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);  //��ʱû�����ּ�������
PUBLIC GUI_RECT_T const*QwertKbGetPosition_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint32 QwertKbGetKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint8 QwertKbGetCharKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint16 const*QwertKbGetKeyMap_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);

PUBLIC BOOLEAN MMIIM_IsLimitSwitchLang (GUIIM_METHOD_PARAM_T *method_param_ptr);
PUBLIC int32 MMIIM_ConfigSettingMenu (IM_DATA_T *data_ptr);
PUBLIC int32 MMIIM_CreateSettingMenuLayer (IM_DATA_T *data_ptr, GUI_POINT_T pos);
PUBLIC void MMIIM_DrawSettingMenu (IM_DATA_T *data_ptr, int16 highlight_index);
PUBLIC int32 MMIIM_OpenMethodSettingMenu (IM_DATA_T *    data_ptr, GUI_POINT_T pos);
PUBLIC int32 MMIIM_CloseMethodSettingMenu (IM_DATA_T *    data_ptr);
PUBLIC int32 MMIIM_MethodSettingMenuRefresh (IM_DATA_T *    data_ptr, GUI_POINT_T point, BOOLEAN repaint);
PUBLIC MMI_RESULT_E MMIIM_MethodSettingMenuUp(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN MMIIM_VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC GUIIM_CAPITAL_MODE_T MMIIM_GetIMCaps(CAPITTAL_MODE_T cap);

PUBLIC int32 MMIIM_TOUCH_KbUnifiedCharKeysFontSize (IM_DATA_T *data_ptr);
/*==============================================================================
Description: ��ȡ���ͳһ�ַ���С(data, ��������, ������������)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC GUI_FONT_T MMIIM_TOUCH_ComputKbUnifiedFontSize (IM_DATA_T *data_ptr, KEY_STYLE_T key_style);

/*==============================================================================
Description: ����ͳһ�ַ���С (data, ��������, ������������)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SetKeyFontSize (
    IM_DATA_T *data_ptr, 
    KEY_STYLE_T key_style, 
    GUI_FONT_T font_size
);


PUBLIC int32 MMIIM_TOUCH_InitVkbMultiPageParam (
    GUIIM_METHOD_T method, 
    VKB_MULTI_PAGE_T *vkb_multi_page_ptr
);

/*==============================================================================
Description: ����������̵ĵ�ǰҳ��ĳЩ���뷨���ڶ��ż��̵��������
	�˴�Լ�������ַ��������
	����������ַ��������ȵļ��̣��緱�����ĵļ����ַ������Ͳ�ͬ
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ResetQwertVkbChars (
    IM_DATA_T *data_ptr, 
    uint8 set_page
);

/*==============================================================================
Description: ��ǰ�����޷�������ʾ�ַ����õ������������Ҫ���������ַ�����
			������ʾ�ַ������룬�˴�Ϊ�˱���ǰ�ڳ���һ���ԣ����������ַ���
			ͬ������ʾ�ַ��������ĳЩ�ط���Ҫ�޸���ʾ�ַ����������������д���
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeDisplayText (IM_DATA_T *data_ptr, KEY_STYLE_T key_style);

/*==============================================================================
Description: ��ǰ�����޷�������ʾ�ַ����õ������������Ҫ���������ַ�����
			������ʾ�ַ������룬�˴�Ϊ�˱���ǰ�ڳ���һ���ԣ����������ַ���
			ͬ������ʾ�ַ��������ĳЩ�ط���Ҫ�޸���ʾ�ַ����������������д���
Global resource dependence: NONE
Author: yaoguang.chen
Note: 1.���ڳ���ά��ʱ�����ܽ�display_text�ֶοռ��С�����Ե�ʱ���ͬ������
		�ͻ��������display_text�ܹ�����ĳ��ȣ���ֹ�ڴ�Խ��
	  2.�������ʹ���������������Ὣ���е��ַ���ͬ���ˣ���ĳЩ
	    ����Ҫͬ���Ļ������	
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeAllDisplayText (IM_DATA_T *data_ptr);

/*==============================================================================
Description: ĳЩ���ԣ���ʾ�ַ����Ͱ����벻ͬ����Ҫ�Զ�����ʾ���ַ���
Global resource dependence: NONE
Author: yaoguang.chen
Note: 
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ThumbKbRedefinePrintText (IM_DATA_T *data_ptr);

#ifdef __cplusplus
}
#endif





#endif //defined (MMI_IM_PDA_SUPPORT) 


#endif /*_MMIIM_TOUCH_COMMON_H_*/


