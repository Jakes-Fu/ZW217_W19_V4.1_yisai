/******************************************************************************
** File Name:      mmitheme_tips.h                                            *
** Author:         xiaoqing.lu                                                *
** Date:           2/20/2009                                                  *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
**                              guistatusbar control                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 2/20/2009      Xiaoqing.lu      Create			                          *
******************************************************************************/

#ifndef _MMITHEME_TIPS_H_
#define _MMITHEME_TIPS_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guistring.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMITIPS_TEXT_MAX_LEN                   40   // ����ͼ��������

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// type of background, image or color
typedef enum
{
    MMITIPS_BGINFO_IMAGE,
    MMITIPS_BGINFO_COLOR,
    MMITIPS_BGINFO_MAX
}MMITIPS_BGINFO_TYPE_E;

// type of background, image or color
typedef enum
{
    MMITIPS_RECT_RIGHTANGLE,
    MMITIPS_RECT_ROUNDANGLE,
    MMITIPS_RECT_MAX
}MMITIPS_RECT_TYPE_E;

// ��ʾ�Ű�,����ԭͼ���ڵ�����
typedef enum
{
    MMITIPS_ALIGN_LEFT,        // ��ԭʼ��������Ϊ��׼
    MMITIPS_ALIGN_MIDDLE,      // ��ԭʼ������м�Ϊ��׼
    MMITIPS_ALIGN_RIGHT,       // ��ԭʼ������Ҳ�Ϊ��׼
    MMITIPS_ALIGN_MAX
}MMITIPS_ALIGN_E;

// text info       
typedef struct
{
    GUI_FONT_T      font_type;
    GUI_COLOR_T     font_color;
    GUI_ALIGN_E     align;
    GUISTR_STATE_T  str_state;
} MMITIPS_TEXT_STYLE_T;

// image info of background
typedef struct  
{
    BOOLEAN         is_display;
	MMI_IMAGE_ID_T	bg_image;
}MMITIPS_BGIMAGE_T;

// color info of background
typedef struct  
{
    BOOLEAN         is_display;

    MMITIPS_RECT_TYPE_E rect_type;
    GUI_COLOR_T	    bg_color;
    GUI_COLOR_T	    line_color;
}MMITIPS_BGCOLOR_T;

// background info
typedef struct
{
    MMITIPS_BGIMAGE_T   bg_image_info; // ����ͼƬ��Ϣ
    MMITIPS_BGCOLOR_T   bg_color_info; // ����ɫ����Ϣ
	MMI_IMAGE_ID_T	    tail1_id;      // β��1�����ϼ�ͷ
    MMI_IMAGE_ID_T	    tail2_id;      // β��2�����¼�ͷ
}MMITIPS_BGSTYLE_T;

typedef struct  
{
    BOOLEAN             timer_is_valid;   // TRUE��������timer��FALSE��������Timer
    uint32              start_timerout;   // ������û���ʾ
    uint32              display_timerout; // ��ʾ֮������ʾ�೤ʱ��
    GUI_RECT_T          edge_rect;
    MMITIPS_ALIGN_E     tips_align;
    uint32              min_height;
    uint32              max_width;

    MMITIPS_BGSTYLE_T    bg_style;    
    MMITIPS_TEXT_STYLE_T text_style;
}MMITIPS_STYLE_INFO_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ȡtips�ؼ��ķ����Ϣ.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTipsStyle(
                                     MMITIPS_STYLE_INFO_T *tips_style_ptr
                                     );

/*****************************************************************************/
// 	Description : get the tips ctrl id
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetTipsCtrlId(void);

/*****************************************************************************/
//    Description : ��ȡĬ����ʾ��Ϣ�ַ�����ʽ
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTipsStrInfo(GUI_RECT_T  *clip_rect_ptr, GUISTR_STYLE_T  *style_ptr, GUISTR_STATE_T *state, MMI_IMAGE_ID_T   *bg_img_ptr);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_TIPS_H_
/*! @} */

