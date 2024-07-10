/*****************************************************************************
** File Name:      ctrlmsgbox.h                                              *
** Author:                                                                   *
** Date:           16/04/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe message box control         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2004       panda.luo          Create                                   *
******************************************************************************/

#ifndef _CTRLMSGBOX_H_
#define _CTRLMSGBOX_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "guianim.h"
#include "guisoftkey.h"
#include "guimsgbox.h"
#include "guictrl.h"

#include "ctrlbase.h"
#include "ctrlmsgbox_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// msgbox type info
#define CTRL_MSGBOX_TYPE     (MSGBOX_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

// msgbox init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    GUIMSGBOX_STYLE_T   style;/*!<���� */
    uint8               lcd_id;/*!<lcd��id */
    GUI_RECT_T			*rect_ptr;/*!<��ʾ����*/
} CTRLMSGBOX_INIT_PARAM_T;

/*
GUIMSGBOX_TEXT_INFO_T��������Ϣ������ı����ݵ����ԡ�
��Ϣ��û�й���������ˣ�������ݳ�����ʾ�������Զ�������ʾ��
*/
typedef struct
{
    MMI_STRING_T			string;         //��Ҫ��ʾ���ַ���
    GUI_RECT_T				text_rect;      //��ʾ�ַ���������
    GUI_COLOR_T				back_color;     //��ʾ�ı�����ı���ɫ
    uint8					move_line;      //ÿ�ι�������
    uint8					space_line;     //�м��
    uint8					space_char;     //�ּ��    
    GUI_FONT_T				font;			//�ַ�������
    GUI_COLOR_T				font_color;		//������ɫ
    uint8					char_perline;   //ÿ�п�����ʾ���ַ���
    uint8					max_lines;      //�ؼ�������ʾ������ı���
    uint8					total_lines;    //��Ҫ��ʾ���ı�������
    uint16					first_line;     //��ʾ��lcd�ϵĵ�һ���ַ���λ��
    BOOLEAN					is_timer_need;  //�Ƿ���Ҫ������ʱ��
    GUI_TIMER_ID_T			text_timer;     //�ı�������ʱ��
    GUI_TIMER_DURATION_T    text_timeout;   //��ʱ���¹����ı�
}GUIMSGBOX_TEXT_INFO_T;


//GUI_MESSAGEBOX_T��������Ϣ��ؼ������ݽṹ
typedef struct _msgbox_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    GUIMSGBOX_STYLE_T       style;              //��Ϣ��ķ��
    BOOLEAN                 is_active;          //�ؼ��Ƿ���Active
    BOOLEAN                 is_focus;           //�ؼ����ڵĴ����Ƿ���Focus
    GUI_COLOR_T             back_color;         //�ؼ�������ɫ
    MMI_IMAGE_ID_T  		back_image_id;		//�ؼ�����ͼƬ
    GUIMSGBOX_TEXT_INFO_T   text_info;          //��ʾ����ʾ��Ϣ
    MMI_TEXT_ID_T           title_text_id;      //title text id
    uint8                   title_height;       //title height
    GUI_COLOR_T             title_color;        //���ⱳ����ɫ    
    GUIMSGBOX_IMAGE_E       image_type;         //ͼƬ����
    GUI_RECT_T              image_rect;         //ͼƬ��ʾ����
    void                    *animate_ptr;        //����ָ��
    MMI_IMAGE_ID_T          image_id;           //ͼƬid
    MMI_ANIM_ID_T           anim_id;            //����id    
    GUI_TIMER_DURATION_T    ani_timeout;        //��������Ƶ��
    uint8                   img_vspace;         //ͼ���߿��ϱߺ��±ߵľ���
    uint8                   img_hspace;          //ͼ���߿���ߺ��ұߵľ���
    MMI_TEXT_ID_T           softkey_id[GUISOFTKEY_BUTTON_NUM];     
    GUI_RECT_T              sk_rect;
    GUI_COLOR_T             sk_bg_color;	
	MMI_WIN_ID_T			win_id;
} CTRLMSGBOX_OBJ_T;    //��Ϣ��ؼ������ݽṹ

// msgbox class for public or protected method
typedef struct _msgbox_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLMSGBOX_VTBL_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get msgbox type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MSGBOX_GetType(void);



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif




