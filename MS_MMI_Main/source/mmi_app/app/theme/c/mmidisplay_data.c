/*****************************************************************************
** File Name:      mmidisplay_data.c                                         *
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

#define _MMIDISPLAY_DATA_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_theme_trc.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"

#ifdef DP_SCREEN_SIZE_240X240
    #include "mmidisplay_data_240x320.c"
#elif defined MAINLCD_SIZE_128X160
    #include "mmidisplay_data_128x160.c"
#elif defined MAINLCD_SIZE_176X220
    #include "mmidisplay_data_176x220.c"
#elif defined(MAINLCD_SIZE_128X64)
    #include "mmidisplay_data_128x64.c"	
#else
    #include "mmidisplay_data_240x320.c"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//������������icon�Ͷ����ĵ���ʾλ�ã���MMI_IDLE_ICON_INDEX_E����һһ��Ӧ
#ifdef MMI_MULTI_SIM_SYS_DUAL
#define TITLE_ICON_NUM      11
#else
#define TITLE_ICON_NUM      10
#endif

LOCAL const GUI_POINT_T mmi_idle_icon_anim[TOTAL_IDLE_ICONANIM_NUMBER] =   
{
    0,0,    //MAIN LCD����ͼƬ��ʼλ������ 
    0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,    //MAIN LCD��SOFTKEY BAR ��ʼλ������
    0,1,												    //MAIN LCD�ź��� 
#ifdef MMI_MULTI_SIM_SYS_DUAL
    30, 1,                                                  //MAIN LCD�ź���2
#endif    
    1 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM +10, 1,		/*27*/	//MAIN LCD gprs״̬��־
    3 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 8, 1,		/*59*/	//MAIN LCD������ʾ�Ͷ���Ϣ����ʾ
    8 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3   , 1,	/*139*/	//MAIN LCD ����
    2 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 12, 1,		/*46*/	//MAIN LCD������ʾ
    7 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3   , 1,	/*122*/	//MAIN LCD�龰ģʽ������������������ʾ
    4 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 8, 1,		/*76*/	//MAIN LCD����ת��
    6 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3  , 1,		/*105*/	//MAIN LCD����
    9 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM - 2, 1, 		/*151*/	//MAIN LCD���
    24,18,          //�ػ���main lcd ��綯��
    29,9,           //MAIN LCD�������嶯��
    5 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 6, 1,	 	/*91*/   //MAIN LCDͨ����·״̬
    8 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM ,    1, 		/*��Ч*/	//MAIN LCD SD����ʾ
};

//IDLE������ʾ�����ֵ����꣬��ɫ����С
#if defined SUBLCD_DEV_SIZE_96X64
LOCAL const MMI_IDLE_DISPLAY_T  mmi_sub_96x64_display[] =
{
    //����SUB LCDʱ�� 
	{0,
     25,
	 IDLE_ALIGN_X_CENTER,
	 SONG_FONT_22},
	
	//����SUB LCD����  
	{0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16,
	},

	//����SUB LCD����
	{0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16, 
	},

	//SUB LCD���ӵ��˵�ʱ���ı���ʾ
	{0,
	32, 
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_20            
	},
	//SUB LCD����ʱ�ı���ʾ
    {0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16
	},
};
#else
LOCAL const MMI_IDLE_DISPLAY_T  mmi_sub_96x96_display[] =
{
    //����SUB LCDʱ�� 
	{0,
     50,
	 IDLE_ALIGN_X_CENTER,
	 SONG_FONT_22},
	
	//����SUB LCD����  
	{0,
	75,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16,
	},

	//����SUB LCD����
	{0,
	50,
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_NORMAL_FONT, 
	},

	//SUB LCD���ӵ��˵�ʱ���ı���ʾ
	{0,
	32, 
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_BIG_FONT            
	},
	//SUB LCD����ʱ�ı���ʾ
    {0,
	50,
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_TEXT_FONT
	},
};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern const MMI_IDLE_DISPLAY_T  mmi_idle_display[];

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
// 	Description : get idle display style
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMI_IDLE_DISPLAY_T MMITHEME_GetIdleDispStyle(
                                                    MMI_IDLE_DISPLAY_INDEX_E    e_idle_disp_index
                                                    )
{
    //uint16  height = 0;

    if ((MMI_MAIN_LCD_NONE >= e_idle_disp_index) && (TOTAL_IDLE_DISPLAY_NUMBER <= e_idle_disp_index))
    {
        //SCI_TRACE_LOW:"== MMITHEME_GetIdleDispStyle == e_idle_disp_index %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDISPLAY_DATA_119_112_2_18_3_1_51_5,(uint8*)"d", e_idle_disp_index);
        return mmi_idle_display[0];
    }

    switch(e_idle_disp_index)
    {
    case MMI_SUB_LCD_TIME:
    case MMI_SUB_LCD_DATE:
    case MMI_SUB_LCD_WEEK:
    case MMI_SUB_LCD_ALARM:			
	case MMI_SUB_LCD_COUNTEDTIME:
#ifdef SUBLCD_DEV_SIZE_96X64
        return mmi_sub_96x64_display[e_idle_disp_index - (uint32)(MMI_SUB_LCD_TIME)];
#else
        return mmi_sub_96x96_display[e_idle_disp_index - (uint32)(MMI_SUB_LCD_TIME)];
#endif
    default:
        break;
    }

    return (mmi_idle_display[e_idle_disp_index-1]);
}

/*****************************************************************************/
// 	Description : get idle icon display position
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMITHEME_GetIdleIconPos(
                                           MMI_IDLE_ICON_INDEX_E    e_icon
                                           )
{
    if (e_icon < TOTAL_IDLE_ICONANIM_NUMBER)
    {
    	return (mmi_idle_icon_anim[e_icon]);
    }
    else
    {
        //SCI_TRACE_LOW:"== MMITHEME_GetIdleIconPos == e_icon: %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDISPLAY_DATA_161_112_2_18_3_1_52_7,(uint8*)"d",e_icon);
    	return (mmi_idle_icon_anim[0]);
    }
}

