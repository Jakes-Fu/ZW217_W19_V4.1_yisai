/*****************************************************************************
** File Name:      mmiim_tp_cstar_internal.h                                 *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle tp cstar input                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/9/16     rui.xu        Create                                        *
******************************************************************************/

#ifndef _MMIIM_TP_CSTAR_INTERNAL_H_
#define _MMIIM_TP_CSTAR_INTERNAL_H_

#include "mmiim_sp_ml_data.h"
#include "mmiim_sp_ml26key_data.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"

#if defined(TOUCH_PANEL_SUPPORT)
#include "mmiim_tp_multitap_26key.h"
#include "mmiim_tp_multitap_9key.h"
#endif

//#if defined(IM_ENGINE_CSTAR)
#ifndef MMIIM_MULTI_TAP
#define MMIIM_MULTI_TAP
//#endif


#define CAND_MAX_NUM                            64
#define MMIIM_CHOICE_STR_MAX_SHOW_NUM           5
#define MMIIM_CHOICE_MAX_NUM                    30                      //30������������cstarһ�仰��ĳ����ƴ���ĸ����ڼ������ʱ�����ܴﵽ20��֮��
#define MMIIM_MAX_PYCHOICE_CHAR_NUM             7

#define TYPE_SET_KEYBOARD_NUM                   100
#define CAND_LEFT_ARROW_IDX                     (CAND_MAX_NUM + 1)
#define CAND_RIGHT_ARROW_IDX                    (CAND_MAX_NUM + 2)


#define MMIIM_TP_CSTAR_KEY_TYPE_MASK            (0xff00)                //���ڹ�����Ϣ����
#define MMIIM_TP_CSTAR_KEY_MASK                 (0x00ff)

#define TIPS_LEFT_MIN                           20
#define TIPS_SPACE_HEIGHT                       10
#define TIPS_FONT_T                             SONG_FONT_26
#define TIPS_MIN_WIDTH                          54
#define TIPS_CHAR_MAX_NUM                       64                      //���ڱ������tips�ַ���󳤶�


#define MMIIM_EXPL_TIMER_OUT_SHORT              (500)
#define MMIIM_EXPL_TIMER_OUT_NORMAL             (750)
#define MMIIM_EXPL_TIMER_OUT_LONG               (1000)
#define MMIIM_EXPL_TIMER_OUT_DEFAULT            MMIIM_EXPL_TIMER_OUT_NORMAL

#define MMIIM_TP_CSTAR_UPPER                    0x20
#define MMIIM_TP_CSTAR_CHARTIME                 1000

/*ע�����ﲻҪ��Ϊ��ǰ���previous��������1��cstar�ǰ���ǰ�������2*/
#define MMIIM_TP_CSTAR_PAGEFLAG_NONE            0x0
#ifdef IM_ENGINE_CSTAR
#define MMIIM_TP_CSTAR_PAGEFLAG_NEXT            0x0001
#define MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS        0x0002
#endif
#ifdef IM_ENGINE_SOGOU
#define MMIIM_TP_CSTAR_PAGEFLAG_NEXT            0x0002
#define MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS        0x0001
#endif

#define MMIIM_TP_KEY_DISPLAY_TEXT_LEN           10                      //�ı���ʾ�ַ��������ݳ���

enum
{
    MMIIM_HW_DEFAULT_TRACE_COLOR = 0x001f,
    MMIIM_HW_DEFAULT_TRACE_WIDTH = 3,
    MMIIM_HW_DEFAULT_LABLE_WIDTH = 3,
};

enum
{
    MMIIM_TP_CSTAR_IDC_NONE,
    MMIIM_TP_CSTAR_IDC_INACTIVE,
    MMIIM_TP_CSTAR_IDC_ACTIVE,
    MMIIM_TP_CSTAR_IDC_PRESSED,

    MMIIM_TP_CSTAR_IDC_MAX,
};

typedef BOOLEAN(*MMIIM_STATE_FUNC)(void*, GUIIM_EVENT_DATA_U*, uint32);   //�������Ķ���

typedef uint16 MMIIM_STATE;                                             //���뷨״̬

typedef enum
{
    MMIIM_TP_CSTAR_FUNC_NONE,
    MMIIM_TP_CSTAR_FUNC_TIPS,

    MMIIM_TP_CSTAR_FUNC_STATE_MAX,
} MMIIM_TP_CSTAR_FUNC_STATE_T;


typedef struct MMIIM_TP_CSTSR_TIPS_DATA_T
{
    int16           frame_img_width;                                    //��߱߿�ͼ�Ŀ��
    int16           frame_img_height;
    int16           center_img_width;                                   //�м����ͼ�Ŀ��
    int16           center_img_height;
    GUI_RECT_T      disp_rect;                                          //��ʾ��ȫ����

} MMIIM_TP_CSTSR_TIPS_DATA_T;

/*�������Ľṹ
[0, 80)���ַ����ж�Ӧ���ַ�����
[80, 100)�������
[100, max)������״ֵ̬
*/
typedef struct MMIIM_TP_CSTAR_FUNC_T
{
    uint32           id;                                                 //��������Ҫ�Ĵ������
    int16           rect_num;                                           //ÿ����������ռ����(һ��ռ����С��)
    MMIIM_TP_CSTAR_FUNC_STATE_T state;
    MMIIM_STATE_FUNC func;                                              //ÿ�������Ĵ�����

} MMIIM_TP_CSTAR_FUNC_T;


//press up�ص������Ĳ���
typedef union
{
    uint16 iState;

} MMIIM_TP_CSTAR_CB_DATA_T;

typedef enum
{
    MMIIM_TP_CSTAR_MSG_NONE,
    MMIIM_TP_CSTAR_MSG_KEY_PRESS,
    MMIIM_TP_CSTAR_MSG_KEY_RELEASE,
    MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS,
    MMIIM_TP_CSTAR_MSG_KEY_LONG_RELEASE,
    MMIIM_TP_CSTAR_MSG_TPDOWN,
    MMIIM_TP_CSTAR_MSG_TPMOVE,
    MMIIM_TP_CSTAR_MSG_TPUP,
    MMIIM_TP_CSTAR_MSG_TPLONG,

    MMIIM_TP_CSTAR_MSG_MAX,
} MMIIM_TP_CSTAR_MSG_ID_T;

typedef enum
{
    MMIIM_TP_CSTAR_MSG_TP_NONE,
    MMIIM_TP_CSTAR_MSG_TP_KB,
    MMIIM_TP_CSTAR_MSG_TP_CAND, //����������bar
    MMIIM_TP_CSTAR_MSG_TP_EDIT,
    MMIIM_TP_CSTAR_MSG_TP_CHOICE, //�������򣬸�Ӧ���Զ��壬����9����������
    MMIIM_TP_CSTAR_MSG_TP_AREA1,

    MMIIM_TP_CSTAR_MSG_TP_MAX
} MMIIM_TP_CSTAR_MSG_TP_AREA_T;

typedef struct
{
    MMIIM_TP_CSTAR_MSG_TP_AREA_T area;
    int32 idx; // key idx of all keypad
    int8 h_idx;
    int8 v_idx;
    GUI_RECT_T rect; //������С������
    GUI_POINT_T point;
} MMIIM_TP_CSTAR_MSG_PARA_TP_T;

typedef union
{
    MMIIM_TP_CSTAR_MSG_PARA_TP_T tp;
    uint8 key;  //��������Ϸ�������Ϣ
} MMIIM_TP_CSTAR_MSG_PARAM_T;

typedef enum
{
    MMIIM_TP_CSTAR_STATE_NOINPUT,
    MMIIM_TP_CSTAR_STATE_INPUTING,
    MMIIM_TP_CSTAR_STATE_SELECTING,
    MMIIM_TP_CSTAR_STATE_ASSOCIATE,
    MMIIM_TP_CSTAR_STATE_CHOICE,
    MMIIM_TP_CSTAR_STATE_USER,

} MMIIM_TP_CSTAR_IM_STATE_T;

typedef struct
{
    MMIIM_TP_CSTAR_MSG_ID_T id;
    MMIIM_TP_CSTAR_MSG_PARAM_T para;
} MMIIM_TP_CSTAR_MSG_T;

typedef enum MMIIM_TP_CSTAR_KEY_STATE_T
{
    MMIIM_TP_CSTAR_KEY_STATE_NONE,                                      //����ʾ
    MMIIM_TP_CSTAR_KEY_STATE_INACTIVE,                                  //������״̬�����һ�
    MMIIM_TP_CSTAR_KEY_STATE_ACTIVE,                                    //����״̬
    MMIIM_TP_CSTAR_KEY_STATE_PRESS,                                     //������

    MMIIM_TP_CSTAR_KEY_STATE_MAX,
} MMIIM_TP_CSTAR_KEY_STATE_T;

typedef enum MMIIM_TP_KEY_DISPLAY_TYPE_T
{
    MMIIM_TP_KEY_DISPLAY_TYPE_NONE,
        
    MMIIM_TP_KEY_DISPLAY_TYPE_TEXT,                                     //���ı���ʾ����
    MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE,                                    //��ͼƬ��ʾ����
    MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT,                             //����ͼ+�ı���ʾ����
    
    MMIIM_TP_KEY_DISPLAY_TYPE_MAX
} MMIIM_TP_KEY_DISPLAY_TYPE_T;

//�滻�����Ľṹ
typedef struct MMIIM_TP_CSTAR_REPLACE_KEY_T
{
    uint8           hor_index;                                          //ˮƽƫ�Ƹ���
    uint8           ver_index;                                          //��ֱƫ�Ƹ���
    uint16          rect_cnt;                                           //ռ�ĸ���

    MMIIM_TP_KEY_DISPLAY_TYPE_T display_key_type;                       //�滻������ʾ��ʽ
    wchar text[MMIIM_TP_KEY_DISPLAY_TEXT_LEN];                          //�ı�����

    MMIIM_TP_CSTAR_KEY_STATE_T  state;                                  //��ǰ����״̬
    MMI_IMAGE_ID_T  img[MMIIM_TP_CSTAR_KEY_STATE_MAX];                  //��Ҫ�滻ͼƬ��Դ�б�

} MMIIM_TP_CSTAR_REPLACE_KEY_T;

typedef struct MMIIM_TP_CSTAR_CHOICE_BAR_T
{
    GUI_RECT_T                      rect;                               //��ʾPYchoice����������λ��
    MMI_WIN_ID_T                    win_id;                             //������������Ϊ�Ժ����ʾ����ȥ��׼��
    GUI_LCD_DEV_INFO                *lcd_info_ptr;
    MMI_IMAGE_ID_T                  bg_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];//����ͼƬ

    GUI_RECT_T                      idc_up_rect;                        //���ϼ�ͷ������ע������������ͼƬ������һ��ʼ����ͼƬ�ĳ��������ֵ����
    MMI_IMAGE_ID_T                  idc_up_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];
    MMIIM_TP_CSTAR_KEY_STATE_T      idc_up_state;                       //���¼�ͷ��״̬

    GUI_RECT_T                      idc_down_rect;                      //���¼�ͷ������������ȡͼƬ�Ŀ�ߣ�ֻҪ��һ�μ���
    MMI_IMAGE_ID_T                  idc_down_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];
    MMIIM_TP_CSTAR_KEY_STATE_T      idc_down_state;

    int8                            has_hot;                            //��ǰ�Ƿ��н���
    int8                            hot_index;                          //��ǰҳ�ĵ�ǰ����λ��

    GUI_RECT_T                      choice_str_rect[MMIIM_CHOICE_STR_MAX_SHOW_NUM]; //����������ַ�����������������Ч�Դ�0��ʼ
    uint8                           cur_page_line_cnt;                  //���ڱ�ʾ��ҳ��ʾ��ʵ������
    uint8                           choice_line_cnt;                    //���ݵ�ǰ�����õ�����߶������������������ʾ��ƴ��ʵ���������

    uint16                          total_choice_cnt;                   //Ŀǰ��Ҫ��ʾ��������������������

    int8                            is_choice_state;                    //��ǰ�Ƿ���chioce�����л�״̬
    int8                            choice_last_index;                  //��ǰҳ��ǰ��Ľ���λ��
    int8                            choice_index;                       //��ǰѡ����

    wchar                           pChoiceBuffer[MMIIM_CHOICE_MAX_NUM][MMIIM_MAX_PYCHOICE_CHAR_NUM];

    uint16                          page_cnt;
    uint16                          page_index;
    uint8                           pageflag;                           //���·�ҳ��־

} MMIIM_TP_CSTAR_CHOICE_BAR_T;

// struct for key index and key chars
// һ������Ӧ������ĸ����
//#define MMIIM_SP_26KEY_CHAR_MAX 12
typedef struct _MMIIM_26KEY_DISPLAY_KEYCHAR_T
{
    //uint16  key_num;                              // key index
    wchar  key_chars[MMIIM_SP_26KEY_CHAR_MAX+1];   // chars on this key. the first char is count of chars
} MMIIM_KEY_TEXT_DISPLAY_DATA_T;

//�������ʹ���ı��ַ�or�ַ�����ʾ��ť��Ϣ���ù��ܴ���ͼƬ��ʾ������̰�ť
//�ýṹû�н�MMIIM_26KEY_DISPLAY_KEYCHAR_T�ϲ���һ����Ϊ��ĳЩ�ṹ���ݵĶ����ԣ�
//��������ĳЩ�ط�ʹ�ã�����˴����Թ��ṹ��MMIIM_SP_26KEY_KEYCHAR_Tʹ��
typedef struct _MMIIM_TP_BUTTON_DISPLAY_TEXT
{
//Ҫ��ʾ����һ���ַ�����ֵ <= disp_data.key_chars[0] -1; < 0 ��ʾ���⺬�壬��Ҫ�����滻����9���������
//�벻�����õİ취�ˣ���Ȼ����ֶλ�������ϵķ�ʽ�ȽϺã������ṹ��͸����˵�
//MMIIM_KB_KEY_DISP_TEXT_ALL�� MMIIM_KB_KEY_DISP_TEXT_HALF_ALL�� MMIIM_KB_KEY_DISP_TEXT_NONE��ֵ
    int16 disp_index;	
    MMIIM_KEY_TEXT_DISPLAY_DATA_T disp_data;
}MMIIM_TP_KEY_DISPLAY_TEXT;

//�����ֵ��������MMIIM_TP_BUTTON_DISPLAY_TEXT.disp_indexʹ�ã��м�
#define MMIIM_KB_KEY_DISP_TEXT_ALL (-1)		//ĳЩ����£�������Ҫ��ʾ���е��ַ�
#define MMIIM_KB_KEY_DISP_TEXT_CHAR_AND_DOT (-2)		////ĳЩ����£�������Ҫ��ʾ���е��ַ�����ʵ�ʳ��Ȳ������Ե�ȡ��
#define MMIIM_KB_KEY_DISP_TEXT_NONE (-3)	//
#define MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT (-4)	//Ӧ����ĳЩ����£���������ʾ�����ַ�

//���뷨״̬�ṹ
typedef struct MMIIM_TP_CSTAR_STATE_T
{
    int16                           vnum;                               //��ֱ�����С����
    int16                           hnum;                               //ˮƽ�����С����
    const MMIIM_TP_CSTAR_FUNC_T*    functab;                            //�����ϸ���������Ӧ�ĺ���������
    const uint16* const *           char_tab;                           //Сд�ַ���
    uint16                          char_cnt;                           //�ַ�������Сд�ʹ�д������һһ��Ӧ�ģ����Ը���һ�����
    const MMIIM_TP_CSTAR_REPLACE_KEY_T* replace_key_tab;                //��Ҫ�滻�İ���
    uint16                          replace_key_cnt;                    //��Ҫ�滻���ĸ���
    uint16                          priv_data;                          //�˼��̿ɲ�����Ȩ�޼���
    GUI_RECT_T                      rect;                               //���̷��þ�������
    GUI_RECT_T                      img_rect;                           //��ʾͼƬ�������
    MMI_IMAGE_ID_T                  normal_image_id;                    //������ʾͼƬID��Ĭ��СдͼƬ
    MMI_IMAGE_ID_T                  press_image_id;                     //ѡ��ʱ��ʾͼƬID
    const MMIIM_TP_KEY_DISPLAY_TEXT **p_disp_text;                           //������̰�ť����ʾ�ַ�or�ַ���
    uint16                          disp_text_cnt;                           //������̰�ť��������������char_cnt��ȣ���Ϊ�˳���Ŀɶ��Լ���չ�ԣ�ʹ�ò�ͬ��ֵ��ʾ
} MMIIM_TP_CSTAR_STATE_T;


//���뷨�ľ��
typedef struct MMIIM_TP_CSTAR_HANDLE_T
{
    CAF_HANDLE_T                    ctrl_handle;                        //ƽ̨���
    GUI_LCD_DEV_INFO const*         lcd_info_ptr;                       //ƽ̨���
    GUI_LCD_DEV_INFO                tips_lcd_info;                      //tips_lcd

    GUIIM_INIT_KERNEL_T             init_param;                         //���뷨�ؼ���ز���
    GUIIM_EVENT_DATA_U*             event_data_ptr;                     //���뷨�ؼ��¼���أ�����handlemsg

    MMIIM_TP_CSTAR_STATE_ID_T       stateId;                            //��״̬ID���ڱ��������̵�����ֵ��ͨ����ID�����ҵ���Ӧ�ļ���
    MMIIM_TP_CSTAR_STATE_ID_T       bak_stateId;                        //������һ�μ���״̬
    MMIIM_TP_CSTAR_STATE_ID_T       bak_stateIdii;                      //�������ϴμ���״̬
    BOOLEAN                         is_full_keypad;                     //�Ƿ�ȫ����
    uint32                          bak_id;                             //���ڱ��水����ID����Ӧ��str�ַ�����߶�Ӧ����״̬ID��fun��һ�ֶ�
    GUI_RECT_T                      press_rect;                         //�����¼��ľ�������
    GUI_RECT_T                      press_img_rect;                     //�����¼���ͼƬ�����������Զ������ͼƬ���Ͻ����
    GUI_RECT_T                      cand_rect;                          //��ѡ���ľ�������
    GUI_RECT_T                      edit_rect;                          //�༭���ľ�������
    GUI_RECT_T                      down_rect;                          //���·�ҳ��ť�ľ�������
    GUI_RECT_T                      up_rect;                            //���Ϸ�ҳ��ť�ľ�������
    GUI_RECT_T                      lock_rect;                          //������������
    GUI_RECT_T                      lock_img_rect;                      //����ͼƬ�������

    //tips���
    GUI_RECT_T                      tips_rect;                          //tips�������
    uint8                           tips_timer_id;

    MMIIM_TP_CSTAR_CB_DATA_T        cb_data;                            //���ڱ���press up �ص�up���������ļ�ֵ

    BOOLEAN                         is_need_lock;                       //�Ƿ�����
    BOOLEAN                         is_ass;                             //[]
    uint8                           pageflag;                           //��ҳ��־

    void*                           cstar_handle;                       //cstar���������ں˾��
    unsigned short                  cstar_window_state;                 //ֱ��ʹ��cstar��״̬��ȫ��cstar�����ݣ�
    //���ֵҲ�ñ໤����ʼ��ʱ0��ÿ�ε�������ʱ�����޸Ĵ�״̬

    uint16                          index;                              //���浱ǰ������
    wchar                           result[CAND_MAX_NUM];
    uint16                          result_len;                         //��ѡ�ֻ��������ַ��صĸ���
    wchar                           pPreBuffer[CAND_MAX_NUM];
    uint16                          nPreEdit;                           //�༭�ָ���
    wchar                           pCandTipsBuffer[TIPS_CHAR_MAX_NUM]; //���ڱ���candbar�б�������ַ���
    uint16                          nCandTips;
    wchar                           pUnderLine[CAND_MAX_NUM];
    uint16                          nUnderLine;                         //UnderLine����
    wchar                           pUserWordBuffer[CAND_MAX_NUM];
    uint16                          nUserWord;
    
    uint32                          type_set;                           //����

    void (*func)(struct MMIIM_TP_CSTAR_HANDLE_T*, MMIIM_TP_CSTAR_CB_DATA_T);

    MMIIM_TP_CSTAR_CHOICE_BAR_T     choice_bar;

    MMIIM_TP_CSTAR_IM_STATE_T       ui_state;                               //���뷨Ӧ��״̬

    //draw flag
    MMIIM_TP_CSTAR_MSG_PARA_TP_T    tp_last_msg_param;                  //������һ�ε����������
    BOOLEAN                         is_rect_change;
    BOOLEAN                         is_kb_dirty;
    BOOLEAN                         is_bar_edit_dirty;
    BOOLEAN                         is_bar_cand_dirty;
    BOOLEAN                         is_bar_choice_dirty;

    //�����multitap �����������ָ��
#if defined(TOUCH_PANEL_SUPPORT)
    MMIIM_TP_MULTITAP_26KEY_DATA_T* multitap_26key_date_ptr;            // multitap 9key pointer.
    MMIIM_TP_MULTITAP_9KEY_DATA_T*  multitap_9key_date_ptr;             // multitap 9key pointer.
#endif

    BOOLEAN mul_timer_opened;   //cstar��user���״̬�������ַ���Ҫ���ö�ʱ�������Ƿ���������������multitap����

} MMIIM_TP_CSTAR_HANDLE_T;


typedef uint8 MMIIM_TP_CSTAR_IDC_STATE_T;

typedef struct
{
    MMI_IMAGE_ID_T img_bg;
    MMI_IMAGE_ID_T img_idc_left[MMIIM_TP_CSTAR_IDC_MAX];
    MMI_IMAGE_ID_T img_idc_right[MMIIM_TP_CSTAR_IDC_MAX];

    GUI_FONT_T font_text;
    GUI_FONT_T font_label;
    GUI_COLOR_T color_text;
    GUI_COLOR_T color_label;
    GUI_COLOR_T color_hot;
} MMIIM_TP_CSTAR_TEXT_BAR_THEME_T;




/*****************************************************************************/
//  Description : �������ϸ���״̬֮���л�
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicth(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : �ַ�������
//  Global resource dependence : none
//  Author: rui.xu
//  Note: ����ֱ�����������ݣ�ע�����ü��̺�����ʱ��Ҫע��
/*****************************************************************************/
PUBLIC BOOLEAN HandleChar(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 26���̴���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 9����26���������������뷨��������ģ���Ҫ����CS_EVENT_KEYBOARD,CS_EVENT_CHAR
//  ��������ͬ���¼������������Ϣֵ��ͬ����ģ������ڱ���ȫ�ֱ����԰�����Ӧ�Ĳ�������
//  �����¼�ʱ��Ҫ���ѡ��26: 'a', 'char'; 9: 'cs_vk_1','keyboard'
/*****************************************************************************/
PUBLIC BOOLEAN Handle26Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 9���̴���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ˵��ͬ�ϣ�Ҫע����˼�
/*****************************************************************************/
PUBLIC BOOLEAN Handle9Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : ���ؼ�����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleReturn(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U*event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : ���뷨ѡ��
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleMethodSelect(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : ��Сд֮���л�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleCaps(
    void* handle_ptr, //in:���뷨���
    GUIIM_EVENT_DATA_U* event_data_ptr,//in&out:���봫������
    uint32 id                 //in:�����id��
);

/*****************************************************************************/
//  Description : 9->26֮���л�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle9KeyTo26Key(
    void* handle_ptr, //in:���뷨���
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id  //in:�����id��
);

/*****************************************************************************/
//  Description : 26->9֮���л�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle26KeyTo9Key(
    void* handle_ptr, //in:���뷨���
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id  //in:�����id��
);

/*****************************************************************************/
//  Description : �����̴���
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleLock(
    void *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : �л������Ŵ���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicthSymbol(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);



#endif//endif MMIIM_MULTI_TAP

#endif//_MMIIM_TP_CSTAR_INTERNAL_H_
