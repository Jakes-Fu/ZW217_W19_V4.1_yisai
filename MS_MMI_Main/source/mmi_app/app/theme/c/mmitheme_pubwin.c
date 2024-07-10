/*****************************************************************************
** File Name:      mmitheme_pubwin.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_PUBWIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_pubwin.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "mmipub_internal.h"
#include "mmi_position.h"
#include "mmi_theme.h"
#include "mmitheme_button.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPUB_UPDATE_DELAY_TICKS 100

#ifdef MAINLCD_SIZE_320X480//syy modify 2010.11.30 -begin
#define MMIPUB_WIN_MARGIN               14      // PUBWIN��������Ļ��ߵľ��룬������ͼƬ��ߵĿհ״�С
#define MMIPUB_TITLE_START              5       // TITLE����ʼλ�ã��������Ч�Ĵ�������
#define MMIPUB_TITLE_IMAGE_TEXT_SPACE   10      // TITLE��ͼƬ�����ֵļ��
#define MMIPUB_TITLE_V_MARGIN_TOP       12      // TITLE�Ĵ�ֱ�߾�(�붥��)
#define MMIPUB_TITLE_V_MARGIN_BOTTOM    2       // TITLE�Ĵ�ֱ�߾�(��ײ�)
#define MMIPUB_CLIENT_H_MARGIN          0       // �ͻ�������PUBWIN���ڵľ���
#define MMIPUB_CLIENT_V_MARGIN          2       // �ͻ����Ĵ�ֱ�߾�
#define MMIPUB_H_ADD_MARGIN             ((480-320)/2)      // ����ʱ��������
#define MMIPUB_BUTTON_MARGIN            10      // BUTTON��ͻ����ı߾�
#define MMIPUB_BUTTON_BOTTOM_MARGIN     18      // BUTTON�±߾�
#define MMIPUB_BUTTON_SPACE             10      // BUTTON���
#define MMIPUB_LINE_SPACE               5       // �����м��

#elif defined(MAINLCD_SIZE_240X400) || defined(MAINLCD_SIZE_240X320)
#define MMIPUB_WIN_MARGIN               DP2PX_VALUE(12)      // PUBWIN��������Ļ��ߵľ��룬������ͼƬ��ߵĿհ״�С
#define MMIPUB_TITLE_START              DP2PX_VALUE(5)       // TITLE����ʼλ�ã��������Ч�Ĵ�������
#define MMIPUB_TITLE_IMAGE_TEXT_SPACE   DP2PX_VALUE(5)       // TITLE��ͼƬ�����ֵļ��
#define MMIPUB_TITLE_V_MARGIN_TOP       DP2PX_VALUE(12)      // TITLE�Ĵ�ֱ�߾�(�붥��)
#define MMIPUB_TITLE_V_MARGIN_BOTTOM    DP2PX_VALUE(2)       // TITLE�Ĵ�ֱ�߾�(��ײ�)
#define MMIPUB_CLIENT_H_MARGIN          DP2PX_VALUE(0)       // �ͻ�������PUBWIN���ڵľ���
#define MMIPUB_CLIENT_V_MARGIN          DP2PX_VALUE(2)       // �ͻ����Ĵ�ֱ�߾�
#ifdef MAINLCD_SIZE_240X400
#define MMIPUB_H_ADD_MARGIN             DP2PX_VALUE((400-240)/2)      // ����ʱ��������
#else
#define MMIPUB_H_ADD_MARGIN             DP2PX_VALUE((320-240)/2)      // ����ʱ��������
#endif
#define MMIPUB_BUTTON_MARGIN            DP2PX_VALUE(4)       // BUTTON��ͻ����ı߾�
#define MMIPUB_BUTTON_BOTTOM_MARGIN     DP2PX_VALUE(18)      // BUTTON�±߾�
#define MMIPUB_BUTTON_SPACE             DP2PX_VALUE(5)       // BUTTON���
#define MMIPUB_LINE_SPACE               DP2PX_VALUE(5)       // �����м��
//added by hongjun.jia for bugzilla 2443/2444/2445/2446
#elif defined MAINLCD_SIZE_176X220
#define MMIPUB_WIN_MARGIN               5//9       // PUBWIN��������Ļ��ߵľ��룬������ͼƬ��ߵĿհ״�С
#define MMIPUB_TITLE_START              5       // TITLE����ʼλ�ã��������Ч�Ĵ�������
#define MMIPUB_TITLE_IMAGE_TEXT_SPACE   5       // TITLE��ͼƬ�����ֵļ��
#define MMIPUB_TITLE_V_MARGIN_TOP       15//10      // TITLE�Ĵ�ֱ�߾�(�붥��)
#define MMIPUB_TITLE_V_MARGIN_BOTTOM    2       // TITLE�Ĵ�ֱ�߾�(��ײ�)
#define MMIPUB_CLIENT_H_MARGIN          0       // �ͻ�������PUBWIN���ڵľ���
#define MMIPUB_CLIENT_V_MARGIN          2       // �ͻ����Ĵ�ֱ�߾�
#define MMIPUB_H_ADD_MARGIN             10      // ����ʱ��������
#define MMIPUB_BUTTON_MARGIN            0//4       // BUTTON��ͻ����ı߾�
#define MMIPUB_BUTTON_BOTTOM_MARGIN     5//10      // BUTTON�±߾�
#define MMIPUB_BUTTON_SPACE             5       // BUTTON���
#define MMIPUB_LINE_SPACE               5       // �����м��
#elif defined  MAINLCD_SIZE_128X160
#define MMIPUB_WIN_MARGIN               5//9       // PUBWIN��������Ļ��ߵľ��룬������ͼƬ��ߵĿհ״�С
#define MMIPUB_TITLE_START              5       // TITLE����ʼλ�ã��������Ч�Ĵ�������
#define MMIPUB_TITLE_IMAGE_TEXT_SPACE   5       // TITLE��ͼƬ�����ֵļ��
#define MMIPUB_TITLE_V_MARGIN_TOP       15//10      // TITLE�Ĵ�ֱ�߾�(�붥��)
#define MMIPUB_TITLE_V_MARGIN_BOTTOM    2       // TITLE�Ĵ�ֱ�߾�(��ײ�)
#define MMIPUB_CLIENT_H_MARGIN          0       // �ͻ�������PUBWIN���ڵľ���
#define MMIPUB_CLIENT_V_MARGIN          2       // �ͻ����Ĵ�ֱ�߾�
#define MMIPUB_H_ADD_MARGIN             10      // ����ʱ��������
#define MMIPUB_BUTTON_MARGIN            0//4       // BUTTON��ͻ����ı߾�
#define MMIPUB_BUTTON_BOTTOM_MARGIN     5//10      // BUTTON�±߾�
#define MMIPUB_BUTTON_SPACE             5       // BUTTON���
#define MMIPUB_LINE_SPACE               4//0       // �����м��
#else
#define MMIPUB_WIN_MARGIN               9       // PUBWIN��������Ļ��ߵľ��룬������ͼƬ��ߵĿհ״�С
#define MMIPUB_TITLE_START              5       // TITLE����ʼλ�ã��������Ч�Ĵ�������
#define MMIPUB_TITLE_IMAGE_TEXT_SPACE   5       // TITLE��ͼƬ�����ֵļ��
#define MMIPUB_TITLE_V_MARGIN_TOP       10      // TITLE�Ĵ�ֱ�߾�(�붥��)
#define MMIPUB_TITLE_V_MARGIN_BOTTOM    2       // TITLE�Ĵ�ֱ�߾�(��ײ�)
#define MMIPUB_CLIENT_H_MARGIN          0       // �ͻ�������PUBWIN���ڵľ���
#define MMIPUB_CLIENT_V_MARGIN          2       // �ͻ����Ĵ�ֱ�߾�
#define MMIPUB_H_ADD_MARGIN             10      // ����ʱ��������
#define MMIPUB_BUTTON_MARGIN            4       // BUTTON��ͻ����ı߾�
#define MMIPUB_BUTTON_BOTTOM_MARGIN     10      // BUTTON�±߾�
#define MMIPUB_BUTTON_SPACE             5       // BUTTON���
#define MMIPUB_LINE_SPACE               5       // �����м��
#endif//syy modify 2010.11.30 -end

#if defined(MAINLCD_DEV_SIZE_240X284)
#define MMIPUB_TITLE_V_MARGIN_TOP       DP2PX_VALUE(2)      // TITLE�Ĵ�ֱ�߾�(�붥��)
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPubTheme(
                          uint32             type,
                          MMITHEME_PUBWIN_T* theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }

    MMITHEME_GetPubLayoutTheme(&theme_ptr->layout);

    //set background image
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->bg_id = 0;
#else

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    if ( MMIPUBWIN_WAITING == type )
    {
        theme_ptr->bg_id = IMAGE_PUBWIN_WAITING_BG;
    }
    else
    {
        theme_ptr->bg_id = theme_ptr->layout.client_bg_id;
    }
#else
    theme_ptr->bg_id = IMAGE_PUBWIN_BG;
#endif

#endif//syy modify 2010.11.30 -end

    theme_ptr->bg_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MASK_LAYER);

    //set background lcd dev
    theme_ptr->bg_lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->bg_lcd_dev.block_id = GUI_BLOCK_MAIN;
    
    // �������pubwin�Ƿ�֧��pngͼƬ������TRUE��pubwinΪpng��FALSE��pubwinΪbmp
    // Ŀǰֻ��HVGA��֧��png
#ifdef MAINLCD_SIZE_320X480
    theme_ptr->is_transparent_softkey = TRUE; 
#else
    theme_ptr->is_transparent_softkey = FALSE;
#endif

#ifdef MMI_PUBWIN_ANIM_SUPPORT // ��ʱ���ζ���
    theme_ptr->is_support_anim = TRUE;
#else
    theme_ptr->is_support_anim = FALSE;
#endif
    
    theme_ptr->update_delay_ticks = MMIPUB_UPDATE_DELAY_TICKS;

    if (MMIPUBWIN_WAITING == type)
    {
        // waiting�����ɰ��Ϊ͸����������ʾ�ɰ壬���ڻ�������������������Ҫ�ɰ�
        // �㣬������ʱ��͸������Ϊ0
        theme_ptr->bg_alpha = 0;
    }
    else
    {
        theme_ptr->bg_alpha = MMI_MASK_ALPHA;//65%͸��;
    }

    //set font
	//fixed by hongjun.jia for bugzilla 2677
	theme_ptr->big_font       = MMI_PUBWIN_BIG_FONT;
    theme_ptr->normal_font    = MMI_PUBWIN_NORMAL_FONT;
	theme_ptr->small_font     = MMI_PUBWIN_SMALL_FONT;

    theme_ptr->line_space = MMIPUB_LINE_SPACE;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
	if (MMIPUBWIN_WAITING == type)
	{
		theme_ptr->font_color          = MMI_WHITE_COLOR;
	}
	else
	{
		theme_ptr->font_color          = MMI_BLACK_COLOR;//MMI_DEFAULT_TEXT_COLOR;
	}
#else
    theme_ptr->font_color      = MMITHEME_GetCurThemeFontColor(MMI_THEME_TEXT_FONT);
#endif
}

/*****************************************************************************/
//  Description : get common public windows theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPubLayoutTheme(
                                MMITHEME_PUBWIN_LAYOUT_T    *layout_theme_ptr
                                )
{
    if (PNULL == layout_theme_ptr)
    {
        return;
    }

    // title����
    layout_theme_ptr->title_start_margin = MMIPUB_WIN_MARGIN + MMIPUB_TITLE_START;
    layout_theme_ptr->title_image_text_space = MMIPUB_TITLE_IMAGE_TEXT_SPACE;
    layout_theme_ptr->title_v_top_margin = MMIPUB_TITLE_V_MARGIN_TOP;
    layout_theme_ptr->title_v_bottom_margin = MMIPUB_TITLE_V_MARGIN_BOTTOM;

    // client����
    layout_theme_ptr->client_h_margin = MMIPUB_WIN_MARGIN + MMIPUB_CLIENT_H_MARGIN;
    layout_theme_ptr->client_v_margin = MMIPUB_CLIENT_V_MARGIN;
    layout_theme_ptr->win_h_add_margin = MMIPUB_H_ADD_MARGIN;

    // button����
    layout_theme_ptr->button_margin = MMIPUB_WIN_MARGIN + MMIPUB_BUTTON_MARGIN;
    layout_theme_ptr->button_bottom_margin = MMIPUB_BUTTON_BOTTOM_MARGIN;
    layout_theme_ptr->button_space = MMIPUB_BUTTON_SPACE;

    // ������ɫ
    layout_theme_ptr->button_font.color   = MMI_BLACK_COLOR;
    layout_theme_ptr->button_font.font    = MMI_DEFAULT_BUTTON_FONT;
    layout_theme_ptr->ctrl_bg_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PUBWIN_BG);

    // ͼƬ
#if defined(MMIPUBWIN_PDASTYLE_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT)
    layout_theme_ptr->top_bg_id             = IMAGE_PUBWIN_TOP_BG_PDA;
    layout_theme_ptr->client_bg_id          = IMAGE_PUBWIN_BG;
    layout_theme_ptr->bottom_bg_id          = IMAGE_PUBWIN_BOTTOM_BG_PDA;
    layout_theme_ptr->bottom_bg_without_button_id = IMAGE_PUBWIN_BOTTOM_BG_WITHOUT_BUTTON_PDA;
    layout_theme_ptr->button_img_id         = IMAGE_PUBWIN_BUTTON_PDA;
    layout_theme_ptr->button_img_hl_id      = IMAGE_PUBWIN_BUTTON_HL_PDA;
    layout_theme_ptr->button_img_gray_id    = IMAGE_PUBWIN_BUTTON_GRAY_PDA;
//fixed by hongjun.jia for bugzilla 2443/2444
#elif defined MAINLCD_SIZE_176X220
    layout_theme_ptr->client_bg_id          = IMAGE_PUBWIN_BG;
    layout_theme_ptr->button_img_id         = IMAGE_PUBWIN_BUTTON;
#endif
}

