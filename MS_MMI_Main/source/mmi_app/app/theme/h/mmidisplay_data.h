/*****************************************************************************
** File Name:      mmidisplay_data.h                                         *
** Author:                                                                   *
** Date:           23/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004        Edison.rao          Create                                 *
******************************************************************************/

#ifndef _MMIDISPLAY_DATA_H_
#define _MMIDISPLAY_DATA_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmi_position.h"
#include "mmi_custom_define.h"

#include "mmidisplay_color.h"
#include "mmidisplay_font.h"

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
typedef enum
{
	MMI_MAIN_BACKGROUND_PIC,		//MAIN LCD����ͼƬ
	MMI_MAIN_SOFTKEY_BAR,           //MAIN LCD SOFTKEY BAR

	MMI_MAIN_IDLE_SIGNAL_ICON,      //MAIN LCD�ź�����ʾ 
#ifdef MMI_MULTI_SIM_SYS_DUAL
	MMI_MAIN_IDLE_SIGNAL_ICON_2,    //MAIN LCD�ź���2��ʾ 
#endif
	MMI_MAIN_IDLE_GPRS_ICON,	//MAIN LCD gprs״̬��־��ʾ��@zhaohui add for gprs icon
    MMI_MAIN_IDLE_MESSAGE_ICON,     //MAIN LCD������ʾ
	MMI_MAIN_IDLE_EARPHONE_ICON,    //MAIN LCD ������ʾ
    MMI_MAIN_IDLE_ALARM_ICON,		//MAIN LCD������ʾ
	MMI_MAIN_IDLE_ENVSET_ICON,		//MAIN LCD�龰ģʽ������������������ʾ
	MMI_MAIN_IDLE_CALL_FORWARD,		//MAIN LCD����ת��
	MMI_MAIN_IDLE_BLUE_TOOTH,		//MAIN LCD����
	MMI_MAIN_IDLE_BATTERY_ICON,     //MAIN LCD�����ʾ
	MMI_MAIN_IS_CHARGING_ANIM,      //MAIN LCD�ػ��� ��綯��
	MMI_MAIN_IS_ALARM_ANIM,			//MAIN LCD�������嶯��
    MMI_MAIN_IDLE_LINE_INFO_ICON,	//MAIN LCDͨ����·״̬
    MMI_MAIN_IDLE_SD_ICON,          //MAIN LCD SD����ʾ
    TOTAL_IDLE_ICONANIM_NUMBER
} MMI_IDLE_ICON_INDEX_E;

typedef enum
{
	IDLE_ALIGN_LEFT,			//�����
	IDLE_ALIGN_RIGHT,			//�Ҷ���
	IDLE_ALIGN_X_CENTER,		//����Ļ�м�
	IDLE_ALIGN_CUSTOM,			//�Զ���
	IDLE_ALIGN_MAX		
}MMI_IDLE_DISPLAY_ALIGN_E;

//IDLE������ʾ�����ֵ����꣬��ɫ����С
typedef struct
{
	uint16						text_frame_x;		//�ı���ʾ�������ʵX����
    uint16						text_frame_top;		//�ı���ʾ�������ʼY����
	MMI_IDLE_DISPLAY_ALIGN_E	text_align;			//�ı���ʾ���뷽ʽ
    GUI_FONT_T					text_font;			//�ı���ʾ�����С
}MMI_IDLE_DISPLAY_T;	

//���������������Ҫ��ʾ��ö��
typedef enum
{
    MMI_MAIN_LCD_NONE,
    MMI_MAIN_LCD_PLMN,				//����MAIN LCD��Ӫ������
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_MAIN_LCD_PLMN2,				//����MAIN LCD��Ӫ������
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    MMI_MAIN_LCD_PLMN3,				//����MAIN LCD��Ӫ������
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMI_MAIN_LCD_PLMN4,				//����MAIN LCD��Ӫ������
#endif
#endif
//    MMI_MAIN_LCD_SPN,               //����MAIN LCD Service Providor name
    
	//MMI_MAIN_LCD_COMMEMORATE_DAY,	//����MAIN LCD������
	MMI_MAIN_LCD_CUSTOM_WORD,		//����MAIN LCD�Զ������
	MMI_MAIN_LCD_COUNTEDTIME,		//����MAIN LCD ����ʱ��ʾ,@zhaohui,counted time
	MMI_MAIN_LCD_CALENDAR,			//����MAIN LCD ������ʾ
	//MMI_MAIN_LCD_ALARM,				//����MIAN LCD���ӵ��˵�ʱ���ı���ʾ
	
    MMI_MAIN_LCD_MP3,				//����MAIN LCD MP3��������mp3 name
    MMI_MAIN_LCD_FM,                //����MAIN LCD FM��������FM name
	MMI_MAIN_LCD_TIME,				//����MAIN LCDʱ��
	MMI_MAIN_LCD_DATE,				//����MAIN LCD����
	MMI_MAIN_LCD_WEEK,				//����MAIN LCD����
    
	MMI_SUB_LCD_TIME,				//����SUB LCDʱ��
	MMI_SUB_LCD_DATE,				//����SUB LCD����
	MMI_SUB_LCD_WEEK,				//����SUB LCD����	
	MMI_SUB_LCD_ALARM,				//SUB LCD���ӵ��˵�ʱ���ı���ʾ
	MMI_SUB_LCD_COUNTEDTIME,        //SUB LCD����ʱ��ʾ,
	MMI_MAIN_LCD_STK_IDLE_MODE,     //STK  ��set up idle mode text��Ҫ��ʾ���ı�

    MMI_MAIN_LCD_LEFT_SOFTKEY,      //�����
    MMI_MAIN_LCD_RIGHT_SOFTKEY,     //�����

	TOTAL_IDLE_DISPLAY_NUMBER
} MMI_IDLE_DISPLAY_INDEX_E;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  
/*****************************************************************************/
// 	Description : get idle display style
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
MMI_IDLE_DISPLAY_T  MMITHEME_GetIdleDispStyle(MMI_IDLE_DISPLAY_INDEX_E e_idle_disp_index);

/*****************************************************************************/
// 	Description : get idle icon display position
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMITHEME_GetIdleIconPos(MMI_IDLE_ICON_INDEX_E e_icon);
  
  
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  
