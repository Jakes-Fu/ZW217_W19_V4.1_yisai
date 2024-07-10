/*****************************************************************************
** File Name:      mmivt_id.h                                                   *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/3/13     bin.ji      Create                                    *
******************************************************************************/

#ifndef _MMIVT_ID_H_
#define _MMIVT_ID_H_ 

#ifdef VT_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    VT_WIN_ID_START = (MMI_MODULE_VT << 16),    
        
#include "mmivt_id.def"
        
        MMIVT_MAX_WIN_ID
}MMIVT_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    VT_CTRL_ID_ID_START = MMIVT_MAX_WIN_ID, //(MMI_MODULE_VT << 16),  
    MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID,  //��屳��
    MMIVT_LABEL_PHOTO_CTRL_ID,   //��ͷ��

    MMIVT_LABEL_CALLSTATE_CTRL_ID,  //��ʾ����״̬
    MMIVT_LABEL_CALLNAME_CTRL_ID, //��ʾ�Է����ƻ����
    MMIVT_LABEL_CALLTIME_CTRL_ID, //��ʾͨ��ʱ��

    //main option
    MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID,   //�����豸ѡ��
    MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID,  //��Ƶ����ѡ��
    MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID,   //����ͷ�л�ѡ��
    MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID, //��ʾģʽѡ��
    
    //video option
    MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, //��Ƶ���ñ���
    MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID, //��Ƶ����LCD���ȵ���ͼ��
    MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID, //��Ƶ���öԱȶȵ���ͼ��
    MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID, //��Ƶ�������ȵ���ͼ��
    MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID, //��Ƶ���û�����תͼ��

    //video parameter control
    MMIVT_LABEL_PROMOPT_CTRL_ID, //��Ƶָ������
    MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, //��Ƶָ��ȼ�
    MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID, //��Ƶ�ȼ�����
    MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID, //��Ƶ�ȼ���С

    //video rotation control
    MMIVT_BUTTON_ROTATE_90_CTRL_ID, //��Ƶ����90����תͼ��
    MMIVT_BUTTON_ROTATE_180_CTRL_ID, //��Ƶ����180����תͼ��

    //audio volume control
    MMIVT_LABEL_VOLUME_PARAM_CTRL_ID, //����������ʾͼ��
    MMIVT_BUTTON_VOLUME_ADD_CTRL_ID, //��������ͼ��       
    MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID, //������Сͼ��

    MMIVT_EDIT_USERINPUT_CTRL_ID, //user input��ʾeditbox�ؼ�
#ifdef TOUCH_PANEL_SUPPORT
    MMIVT_BUTTON_DIAL_DEL_CTRL_ID, //user input panel del button 
    MMIVT_BUTTON_DIAL_1_CTRL_ID, //user input panel 1 button 
    MMIVT_BUTTON_DIAL_2_CTRL_ID, //user input panel 2 button 
    MMIVT_BUTTON_DIAL_3_CTRL_ID, //user input panel 3 button 
    MMIVT_BUTTON_DIAL_4_CTRL_ID, //user input panel 4 button 
    MMIVT_BUTTON_DIAL_5_CTRL_ID, //user input panel 5 button 
    MMIVT_BUTTON_DIAL_6_CTRL_ID, //user input panel 6 button 
    MMIVT_BUTTON_DIAL_7_CTRL_ID, //user input panel 7 button 
    MMIVT_BUTTON_DIAL_8_CTRL_ID, //user input panel 8 button 
    MMIVT_BUTTON_DIAL_9_CTRL_ID, //user input panel 9 button 
    MMIVT_BUTTON_DIAL_STAR_CTRL_ID, //user input panel STAR button
    MMIVT_BUTTON_DIAL_0_CTRL_ID, //user input panel 0 button         
    MMIVT_BUTTON_DIAL_HASH_CTRL_ID, //user input panel HASH button 
#endif

    MMIVT_ANIM_MO_CALLING_CTRL_ID, //��������
    MMIVT_ANIM_INCOMING_CALL_CTRL_ID, //���綯��
    MMIVT_ANIM_CALL_CONNECTING_CTRL_ID, //ͨ�������ж���
    MMIVT_ANIM_CALL_DISCONNECTING_CTRL_ID, //�Ҷ��ж���
    MMIVT_RICHTEXT_SHOWTIME_CTRL_ID, //ͨ���Ҷ�ͨ����Ϣ��ʾ

    MMIVT_CONNECTED_POPMENU_CTRL_ID, //ͨ��ѡ��˵�menu�ؼ�
    MMIVT_MAX_CTRL_ID
}MMIVT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //#ifdef VT_SUPPORT

#endif

