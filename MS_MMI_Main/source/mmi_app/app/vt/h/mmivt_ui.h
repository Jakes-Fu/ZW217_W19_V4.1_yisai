/*****************************************************************************
** File Name:      mmivt_ui.h                                                   *
** Author:                                                                   *
** Date:           21/12/2009                                               *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                 *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 21/12/2009     samboo.shen       Create
******************************************************************************/

#ifndef _MMIVT_UI_H_
#define _MMIVT_UI_H_ 

//#include "mmivt_export.h"
#ifdef VT_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmicc_export.h"
#include "cafapplet.h"
#include "mmivt_kernel.h"
#include "mmivt_export.h"
#include "mmivt_kernel_refAdapter.h"

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
typedef enum
{
    MMIVT_UI_MSG_DIAL,      //本地拨出
    MMIVT_UI_MSG_REMOTE_CONNECT,    //对方接听
    MMIVT_UI_MSG_REMOTE_INCOMING,   //来电
    MMIVT_UI_MSG_CONNECT,   //本地接听
    MMIVT_UI_MSG_CONNECT_END,   //底层已经完成接听，可以正常通话
    MMIVT_UI_MSG_HANGUP,    //挂断
    MMIVT_UI_MSG_DISCONNECT,//对方挂断电话，（此时电话并不一定完全挂断）
    MMIVT_UI_MSG_DISC_END,  //底层已经完成挂断
    MMIVT_UI_MSG_REMOTE_FB_REQ, //收到对方回落请求（语音呼叫）
    MMIVT_UI_MSG_FB_ACCEPT, //同意回落
    MMIVT_UI_MSG_FB_REJECT, //拒绝回落
    MMIVT_UI_MSG_FB_REQ,    //请求回落
    MMIVT_UI_MSG_QUIT,    //VT通话结束，关闭所有相关窗口(挂断信息显示窗口，挂断提示窗口，回落提示窗口，回落询问窗口，回落等待窗口关闭时需要发起)
    MMIVT_UI_MSG_REDIAL, //发起自动重拨

    MMIVT_UI_MSG_REMOTE_ALERTING, //VT呼出，对方开始振铃
    MMIVT_UI_MSG_ENTER_MMR, //进入多媒体彩铃状态
    MMIVT_UI_MSG_QUIT_MMR, //退出多媒体彩铃状态
    MMIVT_UI_MSG_REMOTE_NO_SIGNAL_START, //对端无数据
    MMIVT_UI_MSG_REMOTE_NO_SIGNAL_STOP, //对端数据回复
    MMIVT_UI_MSG_REMOTE_CAMERA_OFF, //对端关闭摄像头
    MMIVT_UI_MSG_REMOTE_CAMERA_ON, //对端开启摄像头
    MMIVT_UI_MSG_REF_ERROR_IND, //REF底层发生异常
    MMIVT_UI_MSG_UPDATE_AUDIO_DEVICE, //更新声音设备
    MMIVT_UI_MSG_MAX
}MMIVT_UI_MSG_E;    //VT UI上的相关操作

typedef enum
{
    MMIVT_UI_STATE_IDLE,           //非VT显示状态，不会显示VT窗口
    MMIVT_UI_STATE_MO_CALLING, //本端呼出界面显示状态
    MMIVT_UI_STATE_CONNECTED,  //通话界面显示状态
    MMIVT_UI_STATE_MT_INCOMING,  //来电界面显示状态
    MMIVT_UI_STATE_CONNECTING, //接听指令发送处理界面显示状态（用户选择接听，正在等待确认）
    MMIVT_UI_STATE_CONN_DISCONNECTING, //接通之后挂断中界面显示状态
    MMIVT_UI_STATE_CONN_DISCONNECTED,  //接通之后挂断完成界面显示状态
    MMIVT_UI_STATE_UNCONN_DISCONNECTING,   //未接通挂断中界面显示状态
    MMIVT_UI_STATE_UNCONN_DISCONNECTED,    //未接通挂断完成界面显示状态
    MMIVT_UI_STATE_FB_CALL_QUIT,   //回落退出提示界面显示状态
    MMIVT_UI_STATE_FB_CALL_QUERY,  //回落询问界面显示状态
    MMIVT_UI_STATE_FB_CALL_PROMPT, //回落提示界面显示状态
    MMIVT_UI_STATE_FB_WAITING, //回落等待界面显示状态
    MMIVT_UI_STATE_MAX
}MMIVT_UI_STATE_E;// VT UI上的相应状态

typedef struct
{
    MMIVT_UI_STATE_E cur_state; //当前状态
    MMIVT_UI_STATE_E last_state; //前一个状态
    //sub_state (待考虑是否加入)
    MMIVT_UI_MSG_E msg;
}MMIVT_CALL_UI_STATE_T;//记录VT CALL当前状态


typedef struct  
{
    MN_DUAL_SYS_E dual_sys;
    uint8* tele_num;
    uint8 tele_len;
}MMIVT_UI_INFO_DIAL_T; //UI执行MMIVT_UI_MSG_DIAL，中间所含参数


typedef struct  
{
    int32 redialed_times; //重拨的次数
    int32 redial_times_max; //重拨的最大次数
}MMIVT_UI_REDIAL_INFO_T; 
//typedef struct
//{
//    MN_DUAL_SYS_E dual_sys;
//    uint8* tele_num;
//    uint8 tele_len;
//}VT_UI_INFO_REMOTE_INCOMING_T; //UI执行MMIVT_UI_MSG_REMOTE_INCOMING，中间所含参数

//****通话窗口相关显示信息
typedef enum
{
    MMIVT_OPTION_BANK_MODE_NORMAL, //普通模式：只显示第一级选项icon
    MMIVT_OPTION_BANK_MODE_VIDEO_OPTION, //视频显示设置模式：显示第二级选项icon
    MMIVT_OPTION_BANK_MODE_VOLUME, //音量显示模式：只显示第一级选项icon和音量调节栏
    MMIVT_OPTION_BANK_MODE_USERINPUT,  //只显示数字输入框
    MMIVT_OPTION_BANK_MODE_MAX
}MMIVT_OPTION_BANK_MODE_E;//通话窗口选项面板显示方式

typedef enum
{
    MMIVT_1LEVEL_AUDIO_OPTION,    //听筒免提切换选项
    MMIVT_1LEVEL_VIDEO_OPTION,    //视频设置选项
    MMIVT_1LEVEL_CAMERA_OPTION,   //摄像头切换选项
    MMIVT_1LEVEL_DISPLAY_MODE_OPTION, //远近端显示选项
    MMIVT_1LEVEL_OPTION_FOCUS_MAX
}MMIVT_1LEVEL_OPTION_E;//通话窗口第一级选项

typedef enum
{
    MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION,    //屏幕背光选项
    MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION,    //对比度选项
    MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION,  //亮度选项
    MMIVT_2LEVEL_VIDEO_ROTATE_OPTION,  //旋转选项
    MMIVT_2LEVEL_VIDEO_OPTION_MAX
}MMIVT_2LEVEL_VIDEO_OPTION_E;//显示设置第二级选项



typedef struct  
{
    MMIVT_OPTION_BANK_MODE_E option_bank_mode;//通话窗口选项面板显示方式
    MMIVT_1LEVEL_OPTION_E option_focus;//第一级选项焦点位置
    MMIVT_2LEVEL_VIDEO_OPTION_E video_sub_option_focus;//显示设置第二级选项焦点位置
    //BOOLEAN is_option_active; //第一级选项是否激活
    BOOLEAN is_video_sub_option_active;//第二级选项是否激活（焦点移动到该项，处于非激活状态，按下OK键，进入激活状态，可以调节相应参数）
    MMIVT_IMAGE_ROTATE_MODE_E image_rotate_mode;//旋转设置项（第三级）焦点位置
    MMIVT_IMAGE_ROTATE_STATUS_E image_local_rotate_status;//当前图像的旋转状态

    BOOLEAN is_handfree_show; //TRUE:声音设置显示下次设置为handfree,FALSE:设置为handhold
    MMIVT_DISP_MODE_E display_mode; //远端近端显示方式
    MMIVT_CAMERA_ID_E camera_id; //camera切换
    MMIVT_VIDEO_LEVEL_E lcd_bright_level;
    MMIVT_VIDEO_LEVEL_E contrast_level;
    MMIVT_VIDEO_LEVEL_E brightness_level;
    MMIVT_VIDEO_LEVEL_E zoom_level;

    BOOLEAN is_recording; //是否正在录制
    MMIVT_RECORD_TYPE_E record_type;
    BOOLEAN is_replacing; //本端是否正在替代
    BOOLEAN is_remote_replacing; //远端是否在替代
    BOOLEAN is_camera_on; //摄像头是否打开
    BOOLEAN is_hangup_by_user; //是否是用户挂断
    
}VT_DISPLAY_INFO_T; //记录当前显示方面的相关状态
//******通话窗口相关显示信息

typedef struct 
{
    uint32 ui_call_time_count;  //显示时间
    MMIVT_CALL_UI_STATE_T ui_state_info; //ui的状态信息
    VT_DISPLAY_INFO_T ui_display_info;//ui的通话窗口界面显示状态
    MMIVT_UI_REDIAL_INFO_T redial_info; //自动重拨的信息
}MMIVT_INSTANCE_T;

typedef struct 
{
    CAF_APPLET_T caf_applet;
    MMIVT_INSTANCE_T vt_instance;
}MMIVT_APPLET_T;   //vt applet定义



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIVT_HandleUIMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来自ps和ui的操作，进行相应的状态跃迁。
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleUIMsg(
    MMIVT_UI_MSG_E msg,	//需要处理的msg
    DPARAM parm	//msg相关信息
    );

/*****************************************************************************/
//  Description : Get VT Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 可以返回PNULL，调用函数需要做判断
/*****************************************************************************/
PUBLIC MMIVT_APPLET_T *MMIVT_GetAppletInstance(void);

/*****************************************************************************/
//  Description : MMIVT_StartVTApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StartVTApplet(void);

/*****************************************************************************/
//  Description :MMIVT_StopVTApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StopVTApplet(void);

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_Redial(void);
    
/*****************************************************************************/
// 	Description : MMIVT_RegWinIdNameArr
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : MMIVT_StopRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVT_StopRecord(void);

/*****************************************************************************/
//  Description : ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:底层错误处理函数
/*****************************************************************************/
PUBLIC void MMIVT_ErrorNotify(MMIVT_REF_RET_E err_type);

/*****************************************************************************/
//  Description : MMIVT_ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:底层错误处理函数
/*****************************************************************************/
//PUBLIC void MMIVT_ErrorNotifyCallBack(DVT_MW_RET_E err_type);

PUBLIC  BOOLEAN MMIAPIVT_GetAudioPlayResumeStatus(void);

PUBLIC void MMIAPIVT_SetAudioPlayResumeStatus(BOOLEAN  status);



#ifdef   __cplusplus
    }
#endif
    
#endif //VT_SUPPORT
    
#endif //_MMIVT_UI_H_
    
