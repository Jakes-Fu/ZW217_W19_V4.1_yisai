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
    MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID,  //面板背景
    MMIVT_LABEL_PHOTO_CTRL_ID,   //大头贴

    MMIVT_LABEL_CALLSTATE_CTRL_ID,  //显示呼叫状态
    MMIVT_LABEL_CALLNAME_CTRL_ID, //显示对方名称或号码
    MMIVT_LABEL_CALLTIME_CTRL_ID, //显示通话时间

    //main option
    MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID,   //声音设备选项
    MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID,  //视频设置选项
    MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID,   //摄像头切换选项
    MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID, //显示模式选项
    
    //video option
    MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, //视频设置背景
    MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID, //视频设置LCD亮度调节图标
    MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID, //视频设置对比度调节图标
    MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID, //视频设置亮度调节图标
    MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID, //视频设置画幅旋转图标

    //video parameter control
    MMIVT_LABEL_PROMOPT_CTRL_ID, //视频指标类型
    MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, //视频指标等级
    MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID, //视频等级增加
    MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID, //视频等级减小

    //video rotation control
    MMIVT_BUTTON_ROTATE_90_CTRL_ID, //视频设置90°旋转图标
    MMIVT_BUTTON_ROTATE_180_CTRL_ID, //视频设置180°旋转图标

    //audio volume control
    MMIVT_LABEL_VOLUME_PARAM_CTRL_ID, //音量级别显示图标
    MMIVT_BUTTON_VOLUME_ADD_CTRL_ID, //音量增加图标       
    MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID, //音量减小图标

    MMIVT_EDIT_USERINPUT_CTRL_ID, //user input显示editbox控件
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

    MMIVT_ANIM_MO_CALLING_CTRL_ID, //呼出动画
    MMIVT_ANIM_INCOMING_CALL_CTRL_ID, //来电动画
    MMIVT_ANIM_CALL_CONNECTING_CTRL_ID, //通话连接中动画
    MMIVT_ANIM_CALL_DISCONNECTING_CTRL_ID, //挂断中动画
    MMIVT_RICHTEXT_SHOWTIME_CTRL_ID, //通话挂断通话信息提示

    MMIVT_CONNECTED_POPMENU_CTRL_ID, //通话选项菜单menu控件
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

