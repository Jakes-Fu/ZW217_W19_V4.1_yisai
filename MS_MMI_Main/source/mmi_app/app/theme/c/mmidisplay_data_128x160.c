/*****************************************************************************
** File Name:      mmidisplay_data_176x220.c                                 *
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

#define _MMIDISPLAY_DATA_128x160_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"
#include "mmi_signal_ext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MMI_IDLE_COMON_INTREVAL             5 //juan.wu cr255113
//IDLE������ʾ�����ֵ����꣬��ɫ����С
LOCAL const MMI_IDLE_DISPLAY_T  mmi_idle_display[TOTAL_IDLE_DISPLAY_NUMBER] =
{                  
	//����MAIN LCD��Ӫ������ 	
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL,IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},

#ifndef MMI_MULTI_SIM_SYS_SINGLE
	//����MAIN LCD SIM2 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 2*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	//����MAIN LCD SIM3 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 3*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 4,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	//����MAIN LCD SIM4 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 4*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#endif
#endif

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    //����MAIN LCD�Զ������ 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4) + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD����ʱ��ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD������ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD MP3��������mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//����MAIN LCD FM��������FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT+5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4) + MMI_IDLE_COMON_INTREVAL, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_DUAL
    //����MAIN LCD�Զ������ 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD����ʱ��ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD������ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD MP3��������mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//����MAIN LCD FM��������FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_TRI
    //����MAIN LCD�Զ������ 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD����ʱ��ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD������ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD MP3��������mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//����MAIN LCD FM��������FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_QUAD
    //����MAIN LCD�Զ������ 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD����ʱ��ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD������ʾ
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCD MP3��������mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//����MAIN LCD FM��������FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#endif

#ifndef DCD_SUPPORT
	//����MAIN LCDʱ��
	{0,22, IDLE_ALIGN_CUSTOM,SONG_FONT_14}, 
#ifdef MMI_RES_ORIENT_PORTRAIT	
	//����MAIN LCD����
    {0, 21+14+5,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_13},//SONG_FONT_12

	//����MAIN LCD���� 
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 4,21+14+5,IDLE_ALIGN_RIGHT,SONG_FONT_13},
#else
	//����MAIN LCD����
    {0, 21+14+3,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_13},//SONG_FONT_12

	//����MAIN LCD���� 
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 4,21+14+3,IDLE_ALIGN_RIGHT,SONG_FONT_13},
#endif
   
#else
	{0,27, IDLE_ALIGN_CUSTOM,MMI_DEFAULT_TEXT_FONT}, 
	//����MAIN LCD����
	{60, 27,
	    IDLE_ALIGN_CUSTOM,MMI_DEFAULT_NORMAL_FONT},
	
    //����MAIN LCD���� 
	{90,27,IDLE_ALIGN_CUSTOM,MMI_DEFAULT_NORMAL_FONT},
#endif
    //����SUB LCDʱ��
	{0,28,
	    IDLE_ALIGN_X_CENTER, SONG_FONT_29},

	//����SUB LCD����  
	{0,65,
        IDLE_ALIGN_X_CENTER, MMI_DEFAULT_NORMAL_FONT},

	//����SUB LCD����
	{0,50,
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},

	//SUB LCD���ӵ��˵�ʱ���ı���ʾ
	{0,50, 
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},

	//SUB LCD����ʱ��ʾ,
	{0,50, 
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},
    //STK set up idle mode ��ʾ���ı�
	{MMI_MAINSCREEN_IDLE_STK_LEFT, MMI_COMMOM_IDLE_STK_TEXT_Y,
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT}
};

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






















