/*==============================================================================
File Name: mmiim_sp_symbol.h
Author: Lianxiang.Zhou
Date: 2010/05/26
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/26   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_SP_SYMBOL_H_
#define _MMIIM_SP_SYMBOL_H_

#include "mmiim_sp_tp_basic.h"

#include "mmiim_sp_symbol_data.h"

#include "mmi_font.h"



//const define
enum
{
    MMIIM_SP_SYMB_CNT_LIMIT = 320,
    //���� count limit ���尴������
    MMIIM_SP_SYMB_BOTTOM_IDX_PU,
    MMIIM_SP_SYMB_BOTTOM_IDX_PD,

//�����Ļ�ߴ��ַ���ʾ��http://���Ȳ������ʼ���������
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)   
    MMIIM_SP_SYMB_PANEL_ROW = 6,
    MMIIM_SP_SYMB_PANEL_COL = 3, //4,
#else
    MMIIM_SP_SYMB_PANEL_ROW = 7,
    MMIIM_SP_SYMB_PANEL_COL = 5,
#endif

#if defined (MAINLCD_SIZE_128X160)
#ifdef MAINLCD_DEV_SIZE_160X128
    MMIIM_SP_SYMB_MARGIN_V = 0,
#else//���������̵������,�������������ʾ�ص�
	MMIIM_SP_SYMB_MARGIN_V = 7,
#endif//#ifdef MAINLCD_DEV_SIZE_160X128

	MMIIM_SP_SYMB_MARGIN_H = 7,

#else
    MMIIM_SP_SYMB_MARGIN_V = 10,
	MMIIM_SP_SYMB_MARGIN_H = 10,
#endif

    MMIIM_SP_SYMB_COLOR_BG = 0x2104,
    MMIIM_SP_SYMB_COLOR_BORDER = 0xc618,
    MMIIM_SP_SYMB_COLOR_FONT = 0xe71c,
    MMIIM_SP_SYMB_COLOR_HOT = 0x6e7e,
//////////////////////////////////////////////////////////////////////////
//fixed by hongjun.jia. 128X160 �ֱ����£���С���壬�����ַ���ʾ����
#ifdef MAINLCD_SIZE_128X160  
	MMIIM_SP_SYMB_FONT = SONG_FONT_12,
    MMIIM_SP_SYMB_FONT_STR = SONG_FONT_10,
    MMIIM_SP_SYMB_FONT_LABEL = SONG_FONT_7,
    MMIIM_SP_SYMB_KEY_HEIGHT = 16,
#else
	MMIIM_SP_SYMB_FONT = SONG_FONT_16,
    MMIIM_SP_SYMB_FONT_STR = SONG_FONT_14,
    MMIIM_SP_SYMB_FONT_LABEL = SONG_FONT_14,
    MMIIM_SP_SYMB_KEY_HEIGHT = 24,
#endif


//�����Ļ�ߴ�����С����Ҫ���ٷ�ҳ������ȣ�����Ὣ��ǰҳ���ַ�����
#ifdef MAINLCD_SIZE_128X160     
	MMIIM_SP_SYMB_KEY_WIDTH = 38,
#elif defined MAINLCD_SIZE_176X220     
    MMIIM_SP_SYMB_KEY_WIDTH = 45,
#else
    MMIIM_SP_SYMB_KEY_WIDTH = 72,
#endif    
};

enum
{
    MMIIM_SP_SYMB_TYPE_NONE,
    MMIIM_SP_SYMB_TYPE_HALF,
    MMIIM_SP_SYMB_TYPE_FULL,
    MMIIM_SP_SYMB_TYPE_HINDI,
    MMIIM_SP_SYMB_TYPE_EUR,

    MMIIM_SP_SYMB_TYPE_MAX
};
typedef uint8 MMIIM_SP_SYMB_TYPE_T;



typedef struct
{
    MMI_HANDLE_T im_handle;
    MMIIM_SP_SYMB_TYPE_T symb_type;
    GUI_LCD_DEV_INFO lcd_dev_info;

    MMI_WIN_ID_T win_id;
    GUI_RECT_T win_rect;

    //panel
    GUI_RECT_T rect_symb;
    uint16 row;
    uint16 col;

    //key list
    MMIIM_SP_COMMON_KEY_T *last_key_ptr;
    MMIIM_SP_COMMON_KEY_T key_pageup;
    MMIIM_SP_COMMON_KEY_T key_pagedown;

    //page tips
    GUI_RECT_T rect_page_tip;

    //color
    GUI_COLOR_T color_bg;
    GUI_COLOR_T color_border;
    GUI_COLOR_T color_font;
    GUI_COLOR_T color_hot;

    GUI_FONT_T font;
    GUI_FONT_T font_min;
    GUI_FONT_T font_label;

    //disp
    uint16 row_cnt;
    uint16 first_row;
    uint16 hot_idx;

    //data
    wchar symb_list[MMIIM_SP_SYMB_CNT_LIMIT];
    uint32 symb_cnt;
} MMIIM_SP_SYMB_WIN_DATA_T;


/*==============================================================================
Function Name: MMIIM_SP_CreateSymbWin
Description: �������Ŵ���
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SymbCreateWin(MMI_HANDLE_T im_handle, MMIIM_SP_SYMB_TYPE_T type);

/*==============================================================================
Description: ������������ѡ��򿪵ķ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SymbCreateWinByLang(MMI_HANDLE_T im_handle, GUIIM_LANGUAGE_T lang);

/*==============================================================================
Function Name: MMIIM_SP_GetSymbType
Description: ͨ�����뷨��ȡ��������
Author: Haiwu.Chen
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC MMIIM_SP_SYMB_TYPE_T MMIIM_GetSymbType(GUIIM_METHOD_T method);

#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Function Name: MMIIM_ConvertToFullSymbol
Description: �������ȫ�Ƿ��ţ�����Ƿ���ת��Ϊȫ�Ƿ��ź����������Ӧ��
                    ���Ų��ڴ˺����У��򷵻�ԭֵ
Author: yaoguang.chen
Global resource dependence: Ӧ�ý������Ĵ���ȫ�Ƿ��Ű�
Note:
==============================================================================*/
PUBLIC wchar MMIIM_ConvertToFullSymbol (wchar symbol);
#endif

#endif /*_MMIIM_SP_SYMBOL_H_*/

