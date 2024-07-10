/*****************************************************************************
** File Name:      mmidisplay_data_64x128.c                                 *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMIDISPLAY_DATA_64x128_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"
#include "mmi_custom_define.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef IDLE_WEEK_TOP  //@ alex modified
#define IDLE_WEEK_TOP 251
#endif
#ifndef IDLE_DATE_TOP
#define IDLE_DATE_TOP 254
#endif
//


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//IDLE������ʾ�����ֵ����꣬��ɫ����С
LOCAL const MMI_IDLE_DISPLAY_T  mmi_idle_display[TOTAL_IDLE_DISPLAY_NUMBER] =
{               
	//@zhaohui,cr102440,������idle����ʾ��λ��
	//����MAIN LCD��Ӫ������ 	
 //added by lxf for dphone

	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#endif
#endif

	//����MAIN LCD Service Providor Name 	
	{82,55,
    IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},
   
	//����MAIN LCD MP3��������mp3 name
	{0,	144, IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},
	
	//����MAIN LCD FM��������FM name
	{0,	144, IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},

	//����MAIN LCDʱ��
	{94,MMI_MAIN_TIME_Y, IDLE_ALIGN_CUSTOM,
    MMI_DEFAULT_TEXT_FONT}, 
	

	{0, 32, IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
	{0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},


    //����MAIN LCD������
	{0,MMI_IDLE_ICON_AREA_HEIGH + (MMI_DEFAULT_FONT_HEIGHT+2) + 1,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT},
	{0, 32, IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
//	{0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},


    //����MAIN LCD�Զ������ 
	//{0,95,IDLE_ALIGN_X_CENTER,
   // MMI_DEFAULT_TEXT_FONT,},

	//����MAIN LCD����ʱ��ʾ
	{0,95,//MMI_IDLE_ICON_AREA_HEIGH + 2*(MMI_DEFAULT_FONT_HEIGHT+2) + 1,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT,},

	//����MAIN LCD������ʾ
	{0,120,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT,},
        
	//MIAN  LCD���ӵ��˵�ʱ���ı���ʾ
	{0, 87,IDLE_ALIGN_X_CENTER,
    MMI_DEFAULT_TEXT_FONT},
	
    //����SUB LCDʱ��
	{0,28,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_29},

	//����SUB LCD����  
	{0,65,
        IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},

	//����SUB LCD����
	{0,50,
	    IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT},

	//SUB LCD���ӵ��˵�ʱ���ı���ʾ
	{0,50, 
	    IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},

    //STK set up idle mode ��ʾ���ı�
	{0,MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - 3 * MMI_DEFAULT_FONT_HEIGHT - 5,
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






















