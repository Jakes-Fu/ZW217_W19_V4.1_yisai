/*****************************************************************************
** File Name:      mmitheme_mainmenu.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _MMITHEME_MAINMENU_H_
#define _MMITHEME_MAINMENU_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "mmi_font.h"
#include "mmk_type.h"

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
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
#define UI_SUPPORT_U_MAINMENU
#endif

#define MMIMENU_WEAKLAYERALPHA			165

#define MMITHEME_MENUMAIN_ICON_VIBRATE 200
#define MMITHEME_MENU_REDRAW_TIME               10   // ͼ���ƶ�timer

#ifdef CMCC_UI_STYLE
#define MMIMENU_MAIN_OPTION_PAGE_DEFAULT_SEL    4   //enter option page, default selected item
#else
#define MMIMENU_MAIN_OPTION_PAGE_DEFAULT_SEL    1   //enter option page, default selected item
#endif

#ifndef MMI_MAINMENU_MINI_SUPPORT
#define MMIMENU_MAIN_MAX_NUM                    100  //���˵�����ITEM����
#else
#define MMIMENU_MAIN_MAX_NUM                    9    //���˵�����ITEM����
#endif
//#define MMIMENU_SLIDE_PAGE_MAX_NUM              10  //�����˵����ҳ��

#define MMIMAINMENU_MOVE_SPACING             5    // when two tp point is larger than this value, then tp move

#if defined MAINLCD_SIZE_128X160
#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_12

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         3

#define MMITHEME_QBMENU_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         9
#define MMITHEME_GOMENU_PAGE_LINE_NUM        3
#define MMITHEME_GOMENU_PAGE_ROW_NUM         3

// ������˵���ҳ��
#ifdef MENU_CRYSTALCUBE_SUPPORT
#define MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM            4   // ������˵���ҳ��
#define MMITHEME_CRYSTALCUBE_MENU_TITLE_TEXT_RECT {11,16, 140, 36}
#define CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_ROW_NUM       3
#endif

// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_40X36һ����ͬ
// �����޷������˵�icon3D��Ч
#ifdef MMI_RES_SLIM_SUPPORT
#define MENUICON_WIDTH      32
#define MENUICON_HEIGHT     32
#define MENUICON_FOCUSITEM_W   40
#define MENUICON_FOCUSITEM_H   40
#define MENUTEXT_WIDTH      32
#define MENUTEXT_HEIGHT     0
#else
#define MENUICON_WIDTH      28
#define MENUICON_HEIGHT     28
#define MENUICON_FOCUSITEM_W   36
#define MENUICON_FOCUSITEM_H   36
#define MENUTEXT_WIDTH      36
#define MENUTEXT_HEIGHT     12
#endif
// end

#define MMITHEME_GOMENU_H_ITEM_MAX_NUM     8
#define MMITHEME_GOMENU_H_PAGE_LINE_NUM    2
#define MMITHEME_GOMENU_H_PAGE_ROW_NUM     4

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            4  //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         3

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              6   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM      2
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM       3

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH          3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM            10

// 128*160 �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           0
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              0

#ifdef MMI_RES_SLIM_SUPPORT
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        0
#else
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        4
#endif
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             60   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���

// 128*160  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0, 3, 127, 20}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           14
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             1
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            1
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              2
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             40   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {6, 24, 170, 67}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 16

#define MMITHEME_OPTMENU_ICON_RECT {3, 70, 168, 174}
#define MMITHEME_OPTMENU_ICON_H_SPC 14
#define MMITHEME_OPTMENU_ICON_V_SPC 24

#define MMITHEME_OPTMENU_TEXT_RECT {3, 112, 172, 189}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 44 

// 128*160 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           2
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          0 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {48, 145, 97, 157}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 1
        
// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//
// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         8
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        2
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         4

// 128*160 �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_ICONMENU_TOP_MARGIN_H              0

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             56   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���


// 128*160  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0, 3, 159, 19}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           20
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             44   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {6, 24, 170, 67}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 16

#define MMITHEME_H_OPTMENU_ICON_RECT {3, 70, 168, 174}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 14
#define MMITHEME_H_OPTMENU_ICON_V_SPC 24

#define MMITHEME_H_OPTMENU_TEXT_RECT {3, 112, 172, 189}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 44 

// 128*160 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {70, 139, 149, 151}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 1

#elif defined MAINLCD_SIZE_176X220 
#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_14

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         3

#define MMITHEME_QBMENU_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         9
#define MMITHEME_GOMENU_PAGE_LINE_NUM        3
#define MMITHEME_GOMENU_PAGE_ROW_NUM         3

// ������˵���ҳ��
#ifdef MENU_CRYSTALCUBE_SUPPORT
#define MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM            4   // ������˵���ҳ��
#define MMITHEME_CRYSTALCUBE_MENU_TITLE_TEXT_RECT {11,16, 140, 36}
#define CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_ROW_NUM       3
#endif

#define MENUNUMICON_HEIGHT  24
#define MENUNUMICON_WIDTH   24 

// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_40X36һ����ͬ
// �����޷������˵�icon3D��Ч
#define MENUICON_WIDTH      40
#define MENUICON_HEIGHT     40
#ifdef MMI_RES_SLIM_SUPPORT
#define MENUICON_FOCUSITEM_W   52
#define MENUICON_FOCUSITEM_H   52 
#define MENUTEXT_WIDTH      40
#define MENUTEXT_HEIGHT     0
#else
#define MENUICON_FOCUSITEM_W   48
#define MENUICON_FOCUSITEM_H   48 
#define MENUTEXT_WIDTH      52
#define MENUTEXT_HEIGHT     14
#endif
// end

#define MMITHEME_GOMENU_H_ITEM_MAX_NUM     8
#define MMITHEME_GOMENU_H_PAGE_LINE_NUM    2
#define MMITHEME_GOMENU_H_PAGE_ROW_NUM     4

#define MMITHEME_MENUMAIN_BUTTON_RECT               {104, 282, 135, 313}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT     MMITHEME_MENUMAIN_BUTTON_RECT

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            4  //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         3

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              6   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM      2
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM       3

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH          3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM            10

// 176*220 �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           2
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              2

#ifdef MMI_RES_SLIM_SUPPORT
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        0
#else
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        4
#endif
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             60   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          6    // icon��menu item���򶥲�֮��ĵľ���

// 176*220  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0, 3, 175, 25 + 3}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           16
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             4
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            4
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              2
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             56   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {6, 24, 170, 67}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 16

#define MMITHEME_OPTMENU_ICON_RECT {3, 70, 168, 174}
#define MMITHEME_OPTMENU_ICON_H_SPC 14
#define MMITHEME_OPTMENU_ICON_V_SPC 24

#define MMITHEME_OPTMENU_TEXT_RECT {3, 112, 172, 189}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 44 

// 176*220 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           24
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          0 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {46, 183, 125, 195}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 1
        
// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//
// ���˵�icon�������8����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         8
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        2
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         4

#define MMITHEME_MENUMAIN_BUTTON_RECT_H               {282, 104, 313, 135}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT_H     {135, 200, 183, 235}

// 176*220 �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           2
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_ICONMENU_TOP_MARGIN_H              2

#ifdef MMI_RES_SLIM_SUPPORT
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#else
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        4
#endif
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             56   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���


// 176*220  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0, 3, 219, 31}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           16
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             2
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            2
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             62   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {6, 24, 170, 67}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 16

#define MMITHEME_H_OPTMENU_ICON_RECT {3, 70, 168, 174}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 14
#define MMITHEME_H_OPTMENU_ICON_V_SPC 24

#define MMITHEME_H_OPTMENU_TEXT_RECT {3, 112, 172, 189}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 44 

// 176*220 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           24
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {70, 139, 149, 151}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 1


#elif defined MAINLCD_SIZE_240X320

#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_14//SONG_FONT_12

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         12
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         4

#define MMITHEME_QBMENU_ITEM_MAX_NUM         9//MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        3//MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         3//MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         MMITHEME_QBMENU_ITEM_MAX_NUM
#define MMITHEME_GOMENU_PAGE_LINE_NUM        MMITHEME_QBMENU_PAGE_LINE_NUM
#define MMITHEME_GOMENU_PAGE_ROW_NUM         MMITHEME_QBMENU_PAGE_ROW_NUM

#define MMITHEME_GOMENU_H_ITEM_MAX_NUM     8
#define MMITHEME_GOMENU_H_PAGE_LINE_NUM    2
#define MMITHEME_GOMENU_H_PAGE_ROW_NUM     4

// ������˵���ҳ��
#ifdef MENU_CRYSTALCUBE_SUPPORT
#define MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM            4   // ������˵���ҳ��
#define MMITHEME_CRYSTALCUBE_MENU_TITLE_TEXT_RECT {21,26, 218, 52}
#define CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_ROW_NUM       3
#endif

#define MENUNUMICON_HEIGHT  24
#define MENUNUMICON_WIDTH   24 

#ifdef MMI_PDA_SUPPORT
// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_48X48һ����ͬ
// �����޷������˵�icon3D��Ч
#define MENUICON_WIDTH      48
#define MENUICON_HEIGHT     48
#define MENUICON_FOCUSITEM_W   50
#define MENUICON_FOCUSITEM_H   50
#define MENUTEXT_WIDTH      60
#define MENUTEXT_HEIGHT     14
// end
#define MMITHEME_MENUMAIN_BUTTON_RECT               {104, 282, 135, 313}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT     MMITHEME_MENUMAIN_BUTTON_RECT

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������16����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         MMITHEME_QBMENU_ITEM_MAX_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        MMITHEME_QBMENU_PAGE_LINE_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         MMITHEME_QBMENU_PAGE_ROW_NUM

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              6   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page

#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        2
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         3

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10

//#ifdef RUNTIME_DRAW_SUPPORT
// 240*320 PDA �������˵�λ����Ϣ
//#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           40
//#define MMITHEME_ICONMENU_LEFT_MARGIN_V             5
//#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            5
//#define MMITHEME_ICONMENU_TOP_MARGIN_V              5
//
//#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
//#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             64   // ����˵���item�߶�
//#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
//#else
// 240*320 PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           40
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              5

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             85   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
//#endif
 
// 240*320 PDA  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 13, 239, MMI_STATUSBAR_HEIGHT + 25 + 13}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           40
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             5
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            5
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              5
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             75   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {20, 37, 220, 81}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_OPTMENU_ICON_RECT {21, 96, 218, 233}
#define MMITHEME_OPTMENU_ICON_H_SPC 31
#define MMITHEME_OPTMENU_ICON_V_SPC 57

#define MMITHEME_OPTMENU_TEXT_RECT {4, 156, 235, 267}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 70 

// 240*320 PDA �������˵�λ����Ϣ 
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           48
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�
/*
#ifndef RUNTIME_DRAW_SUPPORT
#else
//Ϊnor�汾��Сmenu�߶�40, ��ʡ�ڴ�20%
#define MMITHEME_ICONMENU_ICON_RECT {14, 75, 226, 216}//{14, 51, 226, 232}
#define MMITHEME_ICONMENU_ICON_H_SPC 23 // ˮƽ���
#define MMITHEME_ICONMENU_ICON_V_SPC 18//38 // ��ֱ���

#define MMITHEME_ICONMENU_TEXT_RECT {2, 112, 237, 245}//{2, 88, 237, 261}
#define MMITHEME_ICONMENU_TEXT_H_SPC 5
#define MMITHEME_ICONMENU_TEXT_V_SPC 28//48  
#endif
*/
#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {80, 274, 159, 285}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2


// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         18
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         6

#define MMITHEME_MENUMAIN_BUTTON_RECT_H               {282, 104, 313, 135}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT_H     {135, 200, 183, 235}

// #ifdef RUNTIME_DRAW_SUPPORT
// // 240*320 PDA �������˵�λ����Ϣ
// #define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           25
// #define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
// #define MMITHEME_ICONMENU_RIGHT_MARGIN_H            40
// #define MMITHEME_ICONMENU_TOP_MARGIN_H              25
// 
// #define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        2
// #define MMITHEME_ICONMENU_ITEM_HEIGHT_H             60   // ����˵���item�߶�
// #define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          10    // icon��menu item���򶥲�֮��ĵľ���
// #else
// 240*320 PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           5
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            40
#define MMITHEME_ICONMENU_TOP_MARGIN_H              5

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             70   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          10    // icon��menu item���򶥲�֮��ĵľ���
//#endif
 
// 240*320 PDA  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 10, 319, MMI_STATUSBAR_HEIGHT + 25 + 10}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            63
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             53   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {38, 32, 283, 76}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 23

#define MMITHEME_H_OPTMENU_ICON_RECT {38, 84, 283, 188}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 55
#define MMITHEME_H_OPTMENU_ICON_V_SPC 24

#define MMITHEME_H_OPTMENU_TEXT_RECT {24, 125, 297, 203}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 27
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 50 

// 240*320 PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           31
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {116, 195, 205, 208}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2

#else // pda support

// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_48X48һ����ͬ
// �����޷������˵�icon3D��Ч
#ifdef MAIN_MENU_48X48
#define MENUICON_WIDTH      48
#define MENUICON_HEIGHT     48
#define MENUICON_FOCUSITEM_W   48
#define MENUICON_FOCUSITEM_H   48 
#define MENUTEXT_WIDTH      80
#define MENUTEXT_HEIGHT     16
#else
#define MENUICON_WIDTH         60
#define MENUICON_HEIGHT        60
#define MENUICON_FOCUSITEM_W   60
#define MENUICON_FOCUSITEM_H   60
#define MENUTEXT_WIDTH         70
#define MENUTEXT_HEIGHT        16
#endif
// end

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         3

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              6   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page

#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        2
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         3

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10

// 240*320 ��PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           80//35
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              9

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        0// 2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             80//86   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
  
//  240*320 ��PDA �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 13, 239, MMI_STATUSBAR_HEIGHT + 25 + 13}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             75   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {20, 37, 220, 81}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_OPTMENU_ICON_RECT {21, 96, 218, 233}
#define MMITHEME_OPTMENU_ICON_H_SPC 31
#define MMITHEME_OPTMENU_ICON_V_SPC 57

#define MMITHEME_OPTMENU_TEXT_RECT {4, 156, 235, 267}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 70 

// 240*320 ��PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           48
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {80, 274, 159, 285}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2


// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

// ���˵�icon�������9����
#ifdef MAIN_MENU_48X48
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         12
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         4
#else
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         8
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        2
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         4
#endif

// 240*320 ��PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           28
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             5
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            5
#define MMITHEME_ICONMENU_TOP_MARGIN_H              2

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#ifdef MAIN_MENU_48X48
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             65   // ����˵���item�߶�
#else
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             96   // ����˵���item�߶�
#endif
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          2    // icon��menu item���򶥲�֮��ĵľ���

// 240*320 ��PDA  �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 10, 319, MMI_STATUSBAR_HEIGHT + 25 + 10}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           28
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             5
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            5
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              5
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             53   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {38, 32, 283, 76}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 23

#define MMITHEME_H_OPTMENU_ICON_RECT {38, 84, 283, 188}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 55
#define MMITHEME_H_OPTMENU_ICON_V_SPC 24

#define MMITHEME_H_OPTMENU_TEXT_RECT {24, 125, 297, 203}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 27
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 50 

// 240*320 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           31
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {116, 195, 205, 208}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2
#endif // pda support


#elif defined MAINLCD_SIZE_128X64
#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_12

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         9
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         3

#define MMITHEME_QBMENU_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         20
#define MMITHEME_GOMENU_PAGE_LINE_NUM        5
#define MMITHEME_GOMENU_PAGE_ROW_NUM         4
// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_32X32һ����ͬ
// �����޷������˵�icon3D��Ч
#define MENUICON_WIDTH      32
#define MENUICON_HEIGHT     32
#define MENUICON_FOCUSITEM_W   32
#define MENUICON_FOCUSITEM_H   32 
#define MENUTEXT_WIDTH      60
#define MENUTEXT_HEIGHT     16
// end

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         1
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        1
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM        1

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              6   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page

#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        2
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         3

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10

// 128*64 �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           7
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              5

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             85   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
 

// 128*64 �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 239, MMI_STATUSBAR_HEIGHT + 25 + 3}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             75   // ��title�Ĺ���˵���item�߶�



// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {20, 37, 220, 81}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_OPTMENU_ICON_RECT {21, 96, 218, 233}
#define MMITHEME_OPTMENU_ICON_H_SPC 31
#define MMITHEME_OPTMENU_ICON_V_SPC 57

#define MMITHEME_OPTMENU_TEXT_RECT {4, 156, 235, 267}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 70 

// 128*64 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           48
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {80, 274, 159, 285}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2


// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM

// 128*64 �������˵�λ����Ϣ 
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           2
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              2

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        0
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             64   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���


// 128*64 �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 319, MMI_STATUSBAR_HEIGHT + 25 + 3}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             53   // ��title�Ĺ���˵���item�߶�



// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {38, 32, 283, 76}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 23

#define MMITHEME_H_OPTMENU_ICON_RECT {38, 84, 283, 188}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 55
#define MMITHEME_H_OPTMENU_ICON_V_SPC 24

#define MMITHEME_H_OPTMENU_TEXT_RECT {24, 125, 297, 203}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 27
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 50 

// 128*64 �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           31
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {116, 195, 205, 208}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2

#elif  defined MAINLCD_SIZE_240X400
#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_14

// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         12
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         3

#define MMITHEME_QBMENU_ITEM_MAX_NUM         9//MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        3//MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         3//MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         12
#define MMITHEME_GOMENU_PAGE_LINE_NUM        4
#define MMITHEME_GOMENU_PAGE_ROW_NUM         3

#define MMITHEME_GOMENU_H_ITEM_MAX_NUM     8
#define MMITHEME_GOMENU_H_PAGE_LINE_NUM    2
#define MMITHEME_GOMENU_H_PAGE_ROW_NUM     4
// ������˵���ҳ��
#ifdef MENU_CRYSTALCUBE_SUPPORT//jinx
#define MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM            4   // ������˵���ҳ��
#define MMITHEME_CRYSTALCUBE_MENU_TITLE_TEXT_RECT {21,26, 218, 52}
#define CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM        3
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_ROW_NUM         4
#endif

#define MENUNUMICON_HEIGHT  24
#define MENUNUMICON_WIDTH   24 

#ifdef MMI_PDA_SUPPORT
// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_46X46һ����ͬ
// �����޷������˵�icon3D��Ч
#define MENUICON_WIDTH      48
#define MENUICON_HEIGHT     48
#define MENUICON_FOCUSITEM_W   50
#define MENUICON_FOCUSITEM_H   50
#define MENUTEXT_WIDTH      60
#define MENUTEXT_HEIGHT     14
// end

#define MMITHEME_MENUMAIN_BUTTON_RECT               {104, 362, 135, 393}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT     MMITHEME_MENUMAIN_BUTTON_RECT

#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         12
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         3

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              9   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         3
        
#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10

// 240*400 pda �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           56
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              15

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             78   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
 

// 240*400 pda �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 20, 239, MMI_STATUSBAR_HEIGHT + 25 + 20}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           32
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              10
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             68   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {20, 37, 219, 80}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_OPTMENU_ICON_RECT {20, 100, 219, 320}
#define MMITHEME_OPTMENU_ICON_H_SPC 28
#define MMITHEME_OPTMENU_ICON_V_SPC 38

#define MMITHEME_OPTMENU_TEXT_RECT {7, 162, 231, 351}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 70 

// 240*400 PDA�������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           54
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {80, 353, 159, 365}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         18
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         6

#define MMITHEME_MENUMAIN_BUTTON_RECT_H               {359, 104, 391, 135}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT_H     {175, 200, 223, 235}

// 240*400 pda �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           22
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            56
#define MMITHEME_ICONMENU_TOP_MARGIN_H              10

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             62   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���
  

// 240*400 pda �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 399, MMI_STATUSBAR_HEIGHT + 25 + 3}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            56
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              3
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             53   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {20, 37, 219, 80}    //{20, 37, 219, 80}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_H_OPTMENU_ICON_RECT {20, 100, 219, 320}        //{20, 100, 219, 320}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 28
#define MMITHEME_H_OPTMENU_ICON_V_SPC 38

#define MMITHEME_H_OPTMENU_TEXT_RECT {7, 162, 231, 351}     //{7, 162, 231, 351}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 70 

// 240*400 PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           31
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT  {160, 194, 239, 206}    //{83, 346, 158, 351}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2

#else // MMI_PDA_SUPPORT

// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_46X46һ����ͬ
// �����޷������˵�icon3D��Ч
#define MENUICON_WIDTH      48
#define MENUICON_HEIGHT     48
#define MENUICON_FOCUSITEM_W   50
#define MENUICON_FOCUSITEM_H   50
#define MENUTEXT_WIDTH      60
#define MENUTEXT_HEIGHT     14
// end


#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������


// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         12
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         3

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              9   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         3
        
#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10

// 240*400 ��pda �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           56
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              15

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             78   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
 

// 240��400 ��pda �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 20, 239, MMI_STATUSBAR_HEIGHT + 25 + 20}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           32
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              10
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             68   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {20, 37, 219, 80}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_OPTMENU_ICON_RECT {20, 100, 219, 320}
#define MMITHEME_OPTMENU_ICON_H_SPC 28
#define MMITHEME_OPTMENU_ICON_V_SPC 38

#define MMITHEME_OPTMENU_TEXT_RECT {7, 162, 231, 351}
#define MMITHEME_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_OPTMENU_TEXT_V_SPC 70 

// 240*400 ��PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           54
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {80, 353, 159, 365}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//
// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM
// 240��400 ��pda �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           2
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_ICONMENU_TOP_MARGIN_H              2

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        4
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             62   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���
 

// 240��400 ��pda �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 399, MMI_STATUSBAR_HEIGHT + 25 + 3}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              3
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             53   // ��title�Ĺ���˵���item�߶�


// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {20, 37, 219, 80}    //{20, 37, 219, 80}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 8

#define MMITHEME_H_OPTMENU_ICON_RECT {20, 100, 219, 320}        //{20, 100, 219, 320}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 28
#define MMITHEME_H_OPTMENU_ICON_V_SPC 38

#define MMITHEME_H_OPTMENU_TEXT_RECT {7, 162, 231, 351}     //{7, 162, 231, 351}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 3
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 70 

// 240*400 ��PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           31
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT  {160, 194, 239, 206}    //{83, 346, 158, 351}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2
#endif /////MMI_PDA_SUPPORT

#elif  defined MAINLCD_SIZE_320X480
#define MMITHEME_MAINMENU_TEXT_FONT              SONG_FONT_16

// ���˵�icon�������16����
#define MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM         16
#define MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM         4

#define MMITHEME_QBMENU_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define MMITHEME_QBMENU_PAGE_LINE_NUM        MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM
#define MMITHEME_QBMENU_PAGE_ROW_NUM         MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM
#define MMITHEME_QBMENU_LEFT_RIGHT_MARGIN    0

#define MMITHEME_GOMENU_ITEM_MAX_NUM         16
#define MMITHEME_GOMENU_PAGE_LINE_NUM        4
#define MMITHEME_GOMENU_PAGE_ROW_NUM         4

#define MMITHEME_GOMENU_H_ITEM_MAX_NUM     8
#define MMITHEME_GOMENU_H_PAGE_LINE_NUM    2
#define MMITHEME_GOMENU_H_PAGE_ROW_NUM     4

#ifdef MENU_CRYSTALCUBE_SUPPORT//jinx
#define MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM            4   // ������˵���ҳ��
#define MMITHEME_CRYSTALCUBE_MENU_TITLE_TEXT_RECT {41,46, 280, 72}
#define CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM         MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM        4
#define CRYSTALCUBE_MENUMAIN_ICON_PAGE_ROW_NUM         4
#endif

#define MENUNUMICON_HEIGHT  27
#define MENUNUMICON_WIDTH   27 

#ifdef MMI_PDA_SUPPORT
// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_48X48һ����ͬ
// �����޷������˵�icon3D��Ч
#ifdef MAIN_MENU_48X48
#define MENUICON_WIDTH         48
#define MENUICON_HEIGHT        48
#define MENUICON_FOCUSITEM_W   50
#define MENUICON_FOCUSITEM_H   50
#define MENUTEXT_WIDTH         72
#define MENUTEXT_HEIGHT        16
#else
#define MENUICON_WIDTH         60
#define MENUICON_HEIGHT        60
#define MENUICON_FOCUSITEM_W   60
#define MENUICON_FOCUSITEM_H   60
#define MENUTEXT_WIDTH         72
#define MENUTEXT_HEIGHT        16
#endif
// end

#define MMITHEME_MENUMAIN_BUTTON_RECT               {136, 427, 183, 475}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT     MMITHEME_MENUMAIN_BUTTON_RECT


#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������

// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         16
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         4

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              12   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         4

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10   

// 320*480 PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           64
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              15
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             94   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
  
// �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0, MMI_STATUSBAR_HEIGHT + 3, 319, MMI_STATUSBAR_HEIGHT + 40}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           63
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             90 //��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {25, 59, 293, 102}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 31

#define MMITHEME_OPTMENU_ICON_RECT {18, 131, 300, 382}
#define MMITHEME_OPTMENU_ICON_H_SPC 17
#define MMITHEME_OPTMENU_ICON_V_SPC 39

#define MMITHEME_OPTMENU_TEXT_RECT {13, 200, 307, 409}
#define MMITHEME_OPTMENU_TEXT_H_SPC 5
#define MMITHEME_OPTMENU_TEXT_V_SPC 81 

// 320*480 PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0 
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           60
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        2  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          15 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {116, 415, 195, 430}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

#define MMITHEME_MENUMAIN_BUTTON_RECT_H               {431, 143, 470, 177}
#define MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT_H     {215, 267, 279, 319}


// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         18
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        3
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         6

// 320*480 PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           25
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             2
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            58
#define MMITHEME_ICONMENU_TOP_MARGIN_H              15
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             64   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���
  
// 320*480 PDA �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 479, MMI_STATUSBAR_HEIGHT + 30}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           38
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            48
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              0
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             60 // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {25, 59, 293, 102}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 31

#define MMITHEME_H_OPTMENU_ICON_RECT {18, 131, 300, 382}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 17
#define MMITHEME_H_OPTMENU_ICON_V_SPC 39

#define MMITHEME_H_OPTMENU_TEXT_RECT {13, 200, 307, 409}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 5
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 81 

// 320*480 PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           40
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          0 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {200, 264, 279, 279}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2

#else // pda support

// ���˵�ͼ��Ŀ�Ⱥ͸߶ȣ���make file�е�SPECIAL_EFFECT_ICON_SIZE_48X48һ����ͬ
// �����޷������˵�icon3D��Ч
#ifdef MAIN_MENU_48X48
#define MENUICON_WIDTH         48
#define MENUICON_HEIGHT        48
#define MENUICON_FOCUSITEM_W   50
#define MENUICON_FOCUSITEM_H   50
#define MENUTEXT_WIDTH         72
#define MENUTEXT_HEIGHT        16
#else
#define MENUICON_WIDTH         60
#define MENUICON_HEIGHT        60
#define MENUICON_FOCUSITEM_W   60
#define MENUICON_FOCUSITEM_H   60
#define MENUTEXT_WIDTH         72
#define MENUTEXT_HEIGHT        16
#endif
// end


#define MMITHEME_ICON_MENU_DEFAULT_INDEX            0   //�Ź���Ĭ��ѡ�е�ѡ������


// ���˵�icon�������12����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM         16
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM        4
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM         4

// ���˵�ѡ�ÿҳ���������ѡ��˵�
#define MMITHEME_MENUMAIN_ITEM_MAX_NUM              12   //the number of main menu item

// ѡ����˵�������
#define MMITHEME_MENUMAIN_OPTION_PAGE_NUM           4   //the number of option page
#define MMITHEME_MENUMAIN_OPTION_PAGE_LINE_NUM        3
#define MMITHEME_MENUMAIN_OPTION_PAGE_ROW_NUM         4

#define MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH         3
// �����˵���ҳ��
#define MMITHEME_MENUMAIN_SLIDE_PAGE_NUM           10   

// 320*480 ��PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_V           64
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V        2
#define MMITHEME_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_ICONMENU_TOP_MARGIN_V              15
#define MMITHEME_ICONMENU_ITEM_HEIGHT_V             94   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_V          0    // icon��menu item���򶥲�֮��ĵľ���
  
// 320*480 ��PDA �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT {0, MMI_STATUSBAR_HEIGHT + 3, 319, MMI_STATUSBAR_HEIGHT + 40}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V           10
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V              7
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V             90   //��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_OPTMENU_TITLE_ICON_RECT {25, 59, 293, 102}
#define MMITHEME_OPTMENU_TITLE_ICON_H_SPC 31

#define MMITHEME_OPTMENU_ICON_RECT {18, 131, 300, 382}
#define MMITHEME_OPTMENU_ICON_H_SPC 17
#define MMITHEME_OPTMENU_ICON_V_SPC 39

#define MMITHEME_OPTMENU_TEXT_RECT {13, 200, 307, 409}
#define MMITHEME_OPTMENU_TEXT_H_SPC 5
#define MMITHEME_OPTMENU_TEXT_V_SPC 81 

// 320*480 ��PDA �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_V              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V           49
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V          10 // icon�ඥ�߶�

#define MMITHEME_SLIDEMENU_TITLE_ICON_RECT {116, 415, 195, 430}
#define MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC 2

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//
// ���˵�icon�������9����
#define MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H         MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H        MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM
#define MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H         MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM

// 320*480 ��PDA �������˵�λ����Ϣ
#define MMITHEME_ICONMENU_BOTTOM_MARGIN_H           2
#define MMITHEME_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_ICONMENU_TOP_MARGIN_H              2

#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H        0
#define MMITHEME_ICONMENU_ITEM_HEIGHT_H             64   // ����˵���item�߶�
#define MMITHEME_ICONMENU_ICON_TOP_SPACE_H          0    // icon��menu item���򶥲�֮��ĵľ���
  
// 320*480 ��PDA �������˵�������ʾ��Titleλ��ʱ��λ����Ϣ��Ϣ
#define MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT {0,MMI_STATUSBAR_HEIGHT + 3, 479, MMI_STATUSBAR_HEIGHT + 30}

#define MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H           0
#define MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H             0
#define MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H            0
#define MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H              10
#define MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H             55   // ��title�Ĺ���˵���item�߶�

// ѡ����˵�λ����Ϣ
#define MMITHEME_H_OPTMENU_TITLE_ICON_RECT {25, 59, 293, 102}
#define MMITHEME_H_OPTMENU_TITLE_ICON_H_SPC 31

#define MMITHEME_H_OPTMENU_ICON_RECT {18, 131, 300, 382}
#define MMITHEME_H_OPTMENU_ICON_H_SPC 17
#define MMITHEME_H_OPTMENU_ICON_V_SPC 39

#define MMITHEME_H_OPTMENU_TEXT_RECT {13, 200, 307, 409}
#define MMITHEME_H_OPTMENU_TEXT_H_SPC 5
#define MMITHEME_H_OPTMENU_TEXT_V_SPC 81 

// 320*480 ��PDA  �������˵�λ����Ϣ
#define MMITHEME_SLIDEMENU_TOP_MARGIN_H              0
#define MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H           38
#define MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H        0  // item��ͼƬ����֮�����
#define MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H          10 // icon�ඥ�߶�

#define MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT {200, 264, 279, 279}
#define MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC 2
#endif // pda support

#else
#error
#endif

//istyle menu
#define MMITHEME_ISTYLEMENU_INDEX_HEIGHT            12
#define MMITHEME_ISTYLEMENU_BAR_ITEM_MAX            4

#define MMITHEME_QBTHEMEMENU_INDEX_HEIGHT            15
//qbtheme menu
#ifdef MAINLCD_SIZE_320X480
#define MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX            4
#define MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN       7
#define MMITHEME_QBTHEMEMENU_ITEM_HEIGHT             MENUICON_HEIGHT + MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN
#define MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN       (21 - (MENUICON_HEIGHT - 48)/2)  //Ϊ�˻�60��ͼʱ�����겻�Ķ�
#elif  MAINLCD_SIZE_240X320
#define MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX            3
#define MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN       7
#define MMITHEME_QBTHEMEMENU_ITEM_HEIGHT             MENUICON_HEIGHT + MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN
#define MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN       14
#elif  MAINLCD_SIZE_240X400
#define MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX            3
#define MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN       7
#define MMITHEME_QBTHEMEMENU_ITEM_HEIGHT             MENUICON_HEIGHT + MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN
#define MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN       17
#else
#define MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX            4
#define MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN       7
#define MMITHEME_QBTHEMEMENU_ITEM_HEIGHT             (MENUICON_HEIGHT + 10)
#define MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN       14
#endif

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    MMIMENU_LANGUAGE_AUTO = 1,          // Auto.
    MMIMENU_LANGUAGE_ENGLISH,        //Ӣ��
#else
    MMIMENU_LANGUAGE_ENGLISH = 1,  //Ӣ��
#endif
    MMIMENU_LANGUAGE_SIMP_CHINESE,   //���ļ��� 
    MMIMENU_LANGUAGE_TRAD_CHINESE,   //���ķ���
    MMIMENU_LANGUAGE_ARABIC,         //������
    MMIMENU_LANGUAGE_FRENCH,         //����
    MMIMENU_LANGUAGE_HINDI,          //ӡ��
    MMIMENU_LANGUAGE_HUNGARIAN,      //������
    MMIMENU_LANGUAGE_INDONESIAN,     //ӡ��������
    MMIMENU_LANGUAGE_MALAY,          //����
    MMIMENU_LANGUAGE_PORTUGUESE,     //������
    MMIMENU_LANGUAGE_RUSSIAN,        //����
    MMIMENU_LANGUAGE_SPANISH,        //������
    MMIMENU_LANGUAGE_TAGALOG,        //����ŵ
    MMIMENU_LANGUAGE_THAI,           //̩��
    MMIMENU_LANGUAGE_VIETNAMESE,     //Խ����
    MMIMENU_LANGUAGE_URDU,           //�ڶ���
    MMIMENU_LANGUAGE_ITALIAN,        //�����
    MMIMENU_LANGUAGE_PERSIAN,        //��˹
    MMIMENU_LANGUAGE_TURKISH,        //������
    MMIMENU_LANGUAGE_GERMAN,         //����
    MMIMENU_LANGUAGE_GREEK,          //ϣ��
    MMIMENU_LANGUAGE_HEBREW,         //ϣ����
    MMIMENU_LANGUAGE_BENGALI,        //�ϼ���
    MMIMENU_LANGUAGE_CZECH,          //�ݿ�
    MMIMENU_LANGUAGE_SLOVENIAN,      //˹ŵ������
    MMIMENU_LANGUAGE_ROMANIAN,       //��������
    MMIMENU_LANGUAGE_TELUGU,
    MMIMENU_LANGUAGE_MARATHI,
    MMIMENU_LANGUAGE_TAMIL,
    MMIMENU_LANGUAGE_GUJARATI,
    MMIMENU_LANGUAGE_KANNADA,
    MMIMENU_LANGUAGE_MALAYALAM,
    MMIMENU_LANGUAGE_ORIYA,
    MMIMENU_LANGUAGE_PUNJABI,
    MMIMENU_LANGUAGE_AFRIKAANS,
    MMIMENU_LANGUAGE_ALBANIAN,
    MMIMENU_LANGUAGE_ARMENIAN,
    MMIMENU_LANGUAGE_AZERBAIJANI,
    MMIMENU_LANGUAGE_BASQUE,
    MMIMENU_LANGUAGE_BULGARIAN,
    MMIMENU_LANGUAGE_CATALAN,
    MMIMENU_LANGUAGE_CROATIAN,
    MMIMENU_LANGUAGE_DANISH,
    MMIMENU_LANGUAGE_DUTCH,
    MMIMENU_LANGUAGE_ESTONIAN,
    MMIMENU_LANGUAGE_FILIPINO,
    MMIMENU_LANGUAGE_FINNISH,
    MMIMENU_LANGUAGE_GALICIAN,
    MMIMENU_LANGUAGE_GEORGIAN,
    MMIMENU_LANGUAGE_HAUSA,
    MMIMENU_LANGUAGE_ICELANDIC,
    MMIMENU_LANGUAGE_IGBO,
    MMIMENU_LANGUAGE_IRISH,
    MMIMENU_LANGUAGE_KAZAKH,
    MMIMENU_LANGUAGE_LATVIAN,
    MMIMENU_LANGUAGE_LITHUANIAN,
    MMIMENU_LANGUAGE_MACEDONIAN,
    MMIMENU_LANGUAGE_MOLDOVAN,
    MMIMENU_LANGUAGE_NORWEGIAN,
    MMIMENU_LANGUAGE_POLISH,
    MMIMENU_LANGUAGE_SERBIAN,
    MMIMENU_LANGUAGE_SESOTHO,
    MMIMENU_LANGUAGE_SLOVAK,
    MMIMENU_LANGUAGE_SWEDISH,
    MMIMENU_LANGUAGE_UKRAINIAN,
    MMIMENU_LANGUAGE_YORUBA,

    MMIMENU_LANGUAGE_XHOSA,     //������    add. chenyg@spread. 2011-05-12
    MMIMENU_LANGUAGE_ZULU,      //��³��    add. chenyg@spread. 2011-05-12
    MMIMENU_LANGUAGE_ASSAMESE,      //(ӡ����֧��)����ķ��  add. chenyg@spread. 2011-05-12
    MMIMENU_LANGUAGE_SWAHILI,   //˹������  add. chenyg@spread. 2011-05-12
    MMIMENU_LANGUAGE_MYANMAR,    //�����
#if 0   
    MMIMENU_LANGUAGE_AMHARIC,    //��ķ����
    MMIMENU_LANGUAGE_KHMER,    //����կ
    MMIMENU_LANGUAGE_LAO,    //����
#endif
    
    MMIMENU_MAX_LANGUAGE
}MMIMENU_LANGUAGE_TYPE_E;

// main menu type
typedef enum
{
#ifdef ADULT_WATCH_SUPPORT
    MMIMAINMENU_LIST_E,
#endif
    MMIMAINMENU_MATRIX_E,
    MMIMAINMENU_MATRIX_TITLE_E,
    MMIMAINMENU_QBTHEME_E,
    MMIMAINMENU_ISTYLE_E,
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
/*//19��
	STYLE_FLASH,
	STYLE_CLASSIC,
	STYLE_BINARY_STAR,
	STYLE_SHUTTER,
	STYLE_SPHERE,
	STYLE_CYLINDER,
	STYLE_SLOPE, 
	STYLE_WHEEL,
	STYLE_OCEANWAVE,
	STYLE_PEGTOP,
	STYLE_TURNTABLE,
	STYLE_BOXROT,
	STYLE_ICONBOXROT,
	STYLE_BOOKPAGE,
	STYLE_MIDROTATE,
	STYLE_SMALLITEMS,
	STYLE_CONSTICK,
	STYLE_SYMMETRY,
	STYLE_BENZITEM,
*/
#endif // PDA_UI_SUPPORT_MANIMENU_GO
    MMIMAINMENU_GO_SLIDE_E,

    MMIMAINMENU_SWEETS_E,
    MMIMAINMENU_TYPE_MAX    
}MMIMAINMENU_TYPE_E;

typedef void(*CTRLMAINMENU_ITEM_FUNC)(uint32, uint32);
typedef BOOLEAN(*CTRLMAINMENU_ITEM_DEL_FUNC)(uint32, uint32);

// main menu init data
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<����������*/
    uint32              type;
} GUIMAINMENU_INIT_DATA_T;

// �ı�����
typedef enum
{
    CTRL_TEXT_ID,
    CTRL_TEXT_BUFFER,
    CTRL_TEXT_BUFFER_USER,
}CTRL_TEXT_TYPE_E;

// �ı�����
typedef union
{
    uint32 text_id;          /*!< �ı�ID */
    MMI_STRING_T str_info;
}CTRL_TEXT_DATA_T;

// �ı�
typedef struct
{
    CTRL_TEXT_TYPE_E type;
    CTRL_TEXT_DATA_T data;
}CTRL_TEXT_T;

// ͼ������
typedef enum
{
    CTRL_ICON_NONE,
    CTRL_ICON_ID,
    CTRL_ICON_BUFFER,
    CTRL_ICON_FILE
}CTRL_ICON_TYPE_E;

// ͼ������
typedef union
{
    uint32 icon_id;          /*!< �ı�ID */
    struct  
    {
        uint8* data;
        uint16 height;
        uint16 width;
    } buffer;
    MMI_STRING_T file_path;
}CTRL_ICON_DATA_T;

// ͼ��
typedef struct
{
    CTRL_ICON_TYPE_E type;
    CTRL_ICON_DATA_T data;
}CTRL_ICON_T;

// main menu item data
typedef struct
{
    uint32 group_id;
    uint32 menu_id;            /*!< �˵� ID */
    CTRL_TEXT_T text;          /*!< �ı� */
    CTRL_ICON_T icon;          /*!< ͼ�� */
    CTRL_ICON_T active_icon;   /*!< ����״̬�¿���ʹ�ö�̬��ԴͼƬ */

    CTRLMAINMENU_ITEM_FUNC call_back_start; /*!< callback */
    CTRLMAINMENU_ITEM_DEL_FUNC call_back_delete; /*!< callback */
    uint32 param_data1;
    uint32 param_data2;

    BOOLEAN is_disable;
    BOOLEAN is_invisiable;
} CTRLMAINMENU_ITEM_T;

//the main menu theme
typedef struct
{
    BOOLEAN                     is_used_org;		// �Ƿ�Ҫ֧����֯
    BOOLEAN                     is_drag_item2idle;  // item�Ƿ�����϶���idle
    BOOLEAN                     is_used_slide;		// �Ƿ�Ҫ����
    BOOLEAN                     is_hl_disappear;	// �����Ƿ���ʧ
    BOOLEAN                     is_has_button;		// �Ƿ���button
    BOOLEAN                     is_circle;    

	GUI_COLOR_T                 bg_color;   // background color
    MMI_IMAGE_ID_T              bg_image;   // background image
    
    uint16              		default_item_index;    // Ĭ�ϵĽ���������
    uint16              		default_page_index;    // Ĭ�ϵ�ǰҳ��������

    uint16              		focus_item_w;           // ����ͼ��Ŀ��
    uint16              		focus_item_h;           // ����ͼ��ĸ߶�
    
    uint16                      gap_width;  // font and border gap width

    GUI_FONT_T          		item_font;        // menu item font
    GUI_FONT_T          		cur_item_font;      // cur item font
    GUI_FONT_T          		title_font;       // menu title text font

    MMI_IMAGE_ID_T      		text_image_id;    // text����
    BOOLEAN             		is_moving_page;   // ��ʱ�ģ������Ƿ���page�ƶ�״̬

	// ���˵�item��������Ч��Ϣ
	MMI_IMAGE_ID_T      		to_idle_icon_id;        //�������˵�ͼƬ
    MMI_IMAGE_ID_T      		delete_icon_id;         //ɾ��Ӧ�ù���ͼ��
    MMI_IMAGE_ID_T				to_idle_sle_icon_id;    //�������˵�ѡ��ͼ��
    MMI_IMAGE_ID_T				delete_sel_icon_id;     //ɾ��Ӧ��ѡ�б���
    uint32			            icon_shake_period;	// icon ��ʱ����
    uint32                      item_exchange_period;
    uint32                      auto_move_period;
    uint32                      icon_move_start_period;
} MMITHEME_MAINMENU_COMMON_T;

#define MAX_NUM MAX(MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM, MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM_H)

// �������˵���λ����Ϣ
typedef struct
{
    uint16          item_width;
    uint16          item_height;
	
    uint16          text_width;
    uint16          text_height;
	
    GUI_RECT_T      title_text_rect;   // title_icon_menu�е�title����ʾ��text��rect

    uint16          page_row_num;      // Ĭ�ϵ�����
    uint16          page_col_num;      // Ĭ�ϵ�����
    uint16          page_num;          // Ĭ�ϵ�ҳ�����Ź���֧��һҳ
    
    uint16          icon_top_space;    //icon���ϱ߾�
    uint16          top_margin;        //menu������ϱ߾࣬��Ĭ��ֵ�ɵ���
    uint16          bottom_margin;     //menu������±߾࣬��Ĭ��ֵ�ɵ���
    uint16          left_margin;       //menu�������߾࣬��Ĭ��ֵ�ɵ���
	uint16          right_margin;      //menu������ұ߾࣬��Ĭ��ֵ�ɵ���
    uint16          icon_text_margin;  //menu item��icon��text�ļ��                
	    
    void            *data_ptr;
    BOOLEAN         is_need_button;
    GUI_POINT_T     start_point[MAX_NUM];   //item image display rect

} GUIMENU_ICONMENU_DATA_T;

// ����ҳ���˵���λ����Ϣ
typedef struct
{
    uint16          item_width;
    uint16          item_height;

    // ����ҳ�˵���ѡ���ڱ������е�������Ϣ
    GUI_RECT_T      title_display_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM];       // title bg image display rect
    MMI_IMAGE_ID_T  title_unsel_bg_image[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM];     // active menu title bg image
	MMI_IMAGE_ID_T  title_sel_bg_image[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM];       // active menu title bg image

    // ����ҳ�˵���ÿ��ѡ���е�Item����ʾ������Ϣ
    GUI_RECT_T      item_image_display_rect[MMITHEME_GOMENU_ITEM_MAX_NUM];   // ÿ��menu item��ͼƬ����ʾ����
    GUI_RECT_T      item_text_display_rect[MMITHEME_GOMENU_ITEM_MAX_NUM];    // ÿ��menu item�����ֵ���ʾ����
    GUI_RECT_T      item_rect[MMITHEME_GOMENU_ITEM_MAX_NUM];                 // ÿ��menu item����ʾ����
	
    uint16          page_row_num;     // Ĭ�ϵ�����
    uint16          page_col_num;     // Ĭ�ϵ�����
    uint16          page_num;         // Ĭ�ϵ�ҳ��
    MMI_IMAGE_ID_T  title_image_id;   // title����ͼƬ

    
    uint16          top_margin;       // menu������ϱ߾࣬��Ĭ��ֵ�ɵ���
	uint16          bottom_margin;    // menu������±߾࣬��Ĭ��ֵ�ɵ��� 
    uint16          left_margin;      // menu�������߾࣬��Ĭ��ֵ�ɵ���
    uint16          right_margin;     // menu������ұ߾࣬��Ĭ��ֵ�ɵ���
	uint16          icon_text_margin; // menu item��icon��text�ļ�� 

    void            *data_ptr;    
    GUI_POINT_T      start_point[MMITHEME_GOMENU_ITEM_MAX_NUM];   //item image display rect

} GUIMENU_SLIDEMENU_DATA_T;

// ѡ����˵���λ����Ϣ
typedef struct
{
    uint16          item_width;
    uint16          item_height;

    uint16          page_row_num;     // Ĭ�ϵ�����
    uint16          page_col_num;     // Ĭ�ϵ�����
    uint16          page_num;         // Ĭ�ϵ�ҳ����ѡ�֧�ֶ�ҳ���Ź���֧��һҳ

    MMI_IMAGE_ID_T  bar_unsel_bg_image;    //active menu bar bg image
    MMI_IMAGE_ID_T  bar_sel_bg_image;      //active menu bar bg image
    MMI_IMAGE_ID_T  bar_image_id;          //bar����
    MMI_IMAGE_ID_T  folder_image_id;       //�ļ���
    MMI_IMAGE_ID_T  folder_bg_id;          //�ļ��б���

    GUI_RECT_T      bar_index_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM+1]; //add for istyle search page
    GUI_RECT_T      item_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM];
    GUI_RECT_T      bar_item_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX];
    GUI_RECT_T      bar_rect;

    GUI_POINT_T     start_point[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM];      //anim init item rect
    GUI_POINT_T     bar_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX];  //anim init item rect

    uint8           zoom_out;
    uint8           icon_mask_alpha;
    uint8           bg_mask_alpha;
    GUI_FONT_T      item_font;        // menu item font
} GUIMENU_ISTYLE_DATA_T;

#ifdef QBTHEME_SUPPORT
// ǧ���������˵�λ����Ϣ
typedef struct
{
    uint16          item_width;
    uint16          item_height;

    uint16          page_row_num;     // Ĭ�ϵ�����
    uint16          page_col_num;     // Ĭ�ϵ�����
    uint16          page_num;         // Ĭ�ϵ�ҳ����ѡ�֧�ֶ�ҳ���Ź���֧��һҳ

    uint16          select_idx;       //ѡ��item
    MMI_IMAGE_ID_T  bar_unsel_bg_image;     //unactive menu bar bg image
    MMI_IMAGE_ID_T  bar_image_id;           //bar����

    MMI_IMAGE_ID_T  bg_id;                  //����ͼƬ
    MMI_IMAGE_ID_T  item_bg_id;             //item���̱���
    
    GUI_LCD_DEV_INFO  move_page_layer_handle;   //�ƶ��ɰ�� 
    GUI_RECT_T      to_idle_rect;   //�����˵�icon����
    GUI_RECT_T      delete_rect;    //ɾ���˵�icon����
    
    GUI_RECT_T      bar_index_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM];
    GUI_RECT_T      item_rect[MMITHEME_QBMENU_ITEM_MAX_NUM];
    GUI_RECT_T      bar_item_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX];
    GUI_RECT_T      bar_rect;

    GUI_POINT_T     start_point[MMITHEME_QBMENU_ITEM_MAX_NUM];      //anim init item rect

    BOOLEAN         is_dy_item;             //�϶����Ƿ��Ƕ�̬�˵�
    BOOLEAN         is_disp_anim;           //�Ƿ񲥷�ת������
    BOOLEAN         is_disp_item_bg;        //�Ƿ���ʾ����
    BOOLEAN         is_need_save;        //�Ƿ���Ҫ�������˵�����
    uint8           zoom_out;
    uint8           icon_mask_alpha;
    uint8           bg_mask_alpha;
    GUI_FONT_T      item_font;        // menu item font
} GUIMENU_QBTHEME_DATA_T;
#endif// QBTHEME_SUPPORT

#define GUIMENU_FILEFOLD_MAX_ITEM_NUM (MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM - MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM)
#define MMITHEME_ICONMENU_ICON_TEXT_MARGIN MMITHEME_MainmenuIconTextMargin()

#define MMITHEME_MENU_ITEM_WIDTH   MENUTEXT_WIDTH
#define MMITHEME_MENU_ITEM_HEIGHT  (MENUICON_HEIGHT + MENUTEXT_HEIGHT + MMITHEME_ICONMENU_ICON_TEXT_MARGIN)

typedef uint32  GUIMENU_DISPLAY_ITEM_FORMAT_T;
#define GUIMENU_DISPLAY_ITEM_FORMAT_DELETE_ICON        0x00000001
#define GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER           0x00000002
#define GUIMENU_DISPLAY_ITEM_FORMAT_FOLDER_THUMBNAIL   0x00000004
//#define GUIMENU_DISPLAY_ITEM_FORMAT_NO_NUM_ICON        0x00000008

//���Ʋ���
typedef struct
{
    GUIMENU_DISPLAY_ITEM_FORMAT_T format;

    MMI_IMAGE_ID_T delete_icon_id;

    MMI_MENU_ID_T       menu_id[GUIMENU_FILEFOLD_MAX_ITEM_NUM];
    MMI_MENU_GROUP_ID_T group_id[GUIMENU_FILEFOLD_MAX_ITEM_NUM];
    uint32              menu_item_num;

    GUI_LCD_DEV_INFO    dst_layer;
    GUI_RECT_T          dst_rect;
    GUI_RECT_T*         clip_rect_ptr;

} GUIMENU_DISPLAY_ITEM_PARAM_T;

//brief menu process function
typedef struct 
{
    void*           (*Initialize)(MMI_HANDLE_T);                   // init menu
    BOOLEAN         (*Destory)(void*);                             // destory menu
    MMI_RESULT_E    (*Handle)(void*, MMI_MESSAGE_ID_E, DPARAM);    // display menu
} CTRLMAINMENU_PM_APIS_T;

typedef BOOLEAN (*CTRLMAINMENU_REGISTER_PM)(CTRLMAINMENU_PM_APIS_T *);

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : main menu get register
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC CTRLMAINMENU_REGISTER_PM THEMEMAINMENU_GetRegister(MMIMAINMENU_TYPE_E type);

/*****************************************************************************/
//  Description : get menu common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetMainMenuTheme(
                                 MMITHEME_MAINMENU_COMMON_T     *theme_ptr //in/out:
                                 );

#ifdef MATRIX_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : ��ȡ����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIconMenuInfo(
                                        MMI_HANDLE_T          win_handle,
                                        GUIMENU_ICONMENU_DATA_T *icon_menu_info_ptr, //in/out:
                                        BOOLEAN          is_title_style
                                        );
#endif


#ifndef MAIN_MENU_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSlideMenuInfo(
                                         MMI_HANDLE_T          win_handle,
                                         GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr //in/out:
                                         );
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO 
/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetGoMenuInfo(
										 MMI_HANDLE_T          win_handle,
                                         GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr
                                         );
/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSlideMenuTitleIdxRect(
                                                 GUI_RECT_T *rect_ptr
                                                 );

/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_ResetSlideMenuItemTitleRect(
                                                    GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr, //in/out:
                                                    uint16 item_num
                                                    );
#endif

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : ��ȡistyle�˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIStyleMenuInfo(
                                          MMI_HANDLE_T          win_handle,
                                          GUIMENU_ISTYLE_DATA_T *menu_info_ptr //in/out:
                                          );
#endif

#ifdef QBTHEME_SUPPORT

/*****************************************************************************/
//  Description : ��ȡǧ��˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetQbthemeMenuInfo(
                                          MMI_HANDLE_T          win_handle,
                                          GUIMENU_QBTHEME_DATA_T *menu_info_ptr //in/out:
                                          );
#endif

/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_MenuIconVibrate(void);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetIconMenuButtonRect(void);

/*****************************************************************************/
//  Description : ��ȡ�����˵����λ����Ϣ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSlideMenuButtonRect(void);
#endif

//#ifdef RUNTIME_DRAW_SUPPORT
/*****************************************************************************/
//  Description : init menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitMenuItemCache(  
                                       void
                                       );

/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyMenuItemCache(  
                                          void
                                          );

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : init menu folder cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitMenuFolderCache(  
                                         void
                                         );

/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyMenuFolderCache(  
                                            void
                                            );

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:ɾ�������������item
/*****************************************************************************/
PUBLIC void MMITHEME_DeleteMenuItem(
                                    MMI_MENU_ID_T menu_id
                                    );
#endif

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:���Ʋ˵���ͨ��item���ڽ������������Ż�
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMITHEME_DrawMainmenuItem(
                                                  MMI_HANDLE_T win_handle,
                                                  CTRLMAINMENU_ITEM_T *menu_item_ptr,
                                                  const GUIMENU_DISPLAY_ITEM_PARAM_T* param
                                                  );

/*****************************************************************************/
//  Description : ���������ͼƬ�ļ��
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC uint16 MMITHEME_MainmenuIconTextMargin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_MAINMENU_H_

/*@}*/
