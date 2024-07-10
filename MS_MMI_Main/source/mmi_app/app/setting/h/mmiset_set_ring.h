/*****************************************************************************
** File Name:      mmiset_set_ring.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li      Create
******************************************************************************/

#ifndef _MMISET_SET_RING_H_
#define _MMISET_SET_RING_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_LEFT              10
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_WIDHT             143
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_HEIGHT            46
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE          18

#define MMIPHONE_ADJUST_VOL_BG_WIDHT                    107
#define MMIPHONE_ADJUST_VOL_BG_HEIGHT                   89
#define MMIPHONE_ADJUST_VOL_PLUG_WIDTH                  47
#define MMIPHONE_ADJUST_VOL_PLUG_HEIGHT                 25
#define MMIPHONE_ADJUST_VOL_MINUS_LEFT                  22//音量调节减少按钮左边坐标
#define MMIPHONE_VOL_WIDTH  					        11//18//音量调节单个条的宽度
#define MMIPHONE_VOL_HEIGHT 					        50//音量调节单个条的高度
#else
#define MMIPHONE_VOL_WIDTH  								15 //音量调节单个条的宽度
#define MMIPHONE_VOL_HEIGHT 								64//音量调节单个条的高度
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE          24
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_HEIGHT            MMIPHONE_VOL_HEIGHT
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_WIDHT             (MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE*(MMISET_VOL_MAX - 1)+MMIPHONE_VOL_WIDTH)

#define MMIPHONE_ADJUST_VOL_BG_WIDHT                    69
#define MMIPHONE_ADJUST_VOL_BG_HEIGHT                   122
#define MMIPHONE_ADJUST_VOL_PLUG_WIDTH                  55
#define MMIPHONE_ADJUST_VOL_PLUG_HEIGHT                 33
#define MMIPHONE_ADJUST_VOL_MINUS_LEFT                  0 //音量调节减少按钮左
#endif 




/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


//固定来电铃声选择窗口的结构参数
typedef struct  
{
    MN_DUAL_SYS_E               dual_sys;
    uint16                      selected_ring_id; //已选项的铃声ID
    MMI_WIN_ID_T                win_id;           //发送消息的目的窗口
}MMISET_SELECT_WIN_PARAM_T;

//按键铃声和开关机铃声的相关信息
typedef struct
{
    uint8                       other_ring_volume;    //the volume of other ring
    MMISET_OTHER_RING_TYPE_E    other_ring_type;      //the type of other ring
}MMISET_OTHER_RING_T;

#ifdef MMI_ETWS_SUPPORT
typedef struct
{
    BOOLEAN is_personal;
    BOOLEAN is_need_torch;
    MN_DUAL_SYS_E dual_sys;
    MMISET_MSG_RING_TYPE_E  ring_type;
} MMISET_ETWS_RING_T;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
void MMISET_RingInit(void);

/*****************************************************************************/
//  Description : 设置完固定铃声后，将ID信息存如nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedCallRingId(
//                                      MN_DUAL_SYS_E dual_sys,
//                                      uint16 ring_id
//                                       );

/*****************************************************************************/
//  Description : 设置完更多信息铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetMoreMsgRingInfo(
                                      MN_DUAL_SYS_E dual_sys,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      MMISET_CALL_MORE_RING_T ring_info
                                      );

/*****************************************************************************/
//  Description : 将所有信息铃声的种类放入列表中，包括固定铃声和更多音乐
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendMsgRingClassItem(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMI_CTRL_ID_T ctrl_id
                                          );

/*****************************************************************************/
//  Description : get message ring id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMISET_GetFixedMsgRingId(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set the type of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetCallRingType(
//                            MMISET_CALL_RING_TYPE_E  call_ring_type,//in
//                            MN_DUAL_SYS_E            dual_sys
//                             );

/*****************************************************************************/
//  Description : get msg ring info including ring class and index and ring colum
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
// PUBLIC MMISET_MSG_RING_T MMISET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id);

/*****************************************************************************/
//  Description : 根据radio list选中的item，设置来电提示方式,消息提示方式，按键音和翻盖提示音
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetSelectRadioItem(
//                                      MMI_CTRL_ID_T         ctrl_id,
//                                      MMISET_ALL_RING_TYPE_E   ring_type
//                                       );

/*****************************************************************************/
//  Description : 添加低电压报警音的开启关闭的单选list items
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendVoltageWarningListItem(
//                                                MMI_CTRL_ID_T         ctrl_id
//                                                 );

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的来电提示方式，
//                  来播放相应的铃声，振动或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayCallTypeByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                           );

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的消息提示方式，
//                  来播放相应的铃声，振动或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayMsgTypeByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                          );

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的按键音，
//                  来播放相应的铃声或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayKeyToneByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                          );

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的按键音，
//                  来播放相应的铃声或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayFlipRingByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                           );

/*****************************************************************************/
//  Description : 根据音量的大小显示音量图片
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_DisplayVolumePicture(
//                                 uint8   volume,
//                                 MMI_WIN_ID_T win_id
//                                  );
/*****************************************************************************/
//  Description : 左右方向键调节音量的大小
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_TuneUpVolume(
//                                MMI_MESSAGE_ID_E           msg_id,
//                                MMISET_ALL_VOLUME_TYPE_E   volume_type
//                                 );
/*****************************************************************************/
//  Description : 将所有铃声的种类放入列表中，包括固定铃声和更多音乐
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id);

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的铃声来播放来电铃声和消息铃声
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_PlayRingByListItem(
                                      MMI_CTRL_ID_T         ctrl_id,
                                      MMISET_ALL_RING_TYPE_E   ring_type
                                      );

/*****************************************************************************/
//  Description : 根据radio list来设置来消息铃声
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMsgRingByRadioItem(
                                         MN_DUAL_SYS_E  dual_sys,
                                         MMI_CTRL_ID_T  ctrl_id
                                         );
/*****************************************************************************/
//  Description : get fixed call ring index from nv
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
// PUBLIC uint16 MMISET_GetFixedCallRingId(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set open or close low voltage warning
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_IsOpenVoltageWarning(
//                                        BOOLEAN  is_open
//                                         );

/*****************************************************************************/
//  Description : 添加固定铃声铃声list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendFixedCallRingItem(
                                           MN_DUAL_SYS_E dual_sys,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMISET_SELECT_WIN_PARAM_T     *win_param_ptr
                                           );

///*****************************************************************************/
////Description : 预览铃声 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMISET_PreviewRing(
//                        MN_DUAL_SYS_E dual_sys,
//                        uint16                       ring_id,    // the ring id
//                        uint8                       ring_vol,   // the ring volume 
//                        uint32                      play_times, // play times
//                        MMISET_ALL_RING_TYPE_E      ring_type,  // in: the type of all ring
//                        MMISET_RING_CALLBACK_PFUNC  callback_func
//                        );
//
//
///*****************************************************************************/
////Description : 开启震动 
////Global resource dependence : 
////Author: maqianlin
////Note: 
///*****************************************************************************/
//PUBLIC void MMIENVSET_StartVibrator(MMISET_ALL_RING_TYPE_E  ring_type);
//
///*****************************************************************************/
////Description : 开启震动计时器 
////Global resource dependence : 
////Author: maqianlin
////Note:在开启计时器
///*****************************************************************************/
//PUBLIC void MMISET_StartVibratTimer(void);
//
///*****************************************************************************/
////Description : 关闭震动 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMISET_StopVibrator(
//                              MMISET_ALL_RING_TYPE_E  ring_type
//                              );
//
///*****************************************************************************/
////Description : 关闭震动计时器 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMIENVSET_StopVibratTimer(void);
//
///*****************************************************************************/
////Description : 判断是否震动计时器时间到 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMISET_IsVibratTimerOut(DPARAM param);
//
///*****************************************************************************/
////    Description : to STOP the vibrator
////    Global resource dependence : 
////  Author:jassmine.meng
////    Note: 
///*****************************************************************************/
//PUBLIC void MMISET_RingCallBack(MMIAUDIO_RESULT result, DPARAM param);
//
///*****************************************************************************/
////Description : 判断震动计时器是否开启 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
// PUBLIC BOOLEAN MMISET_IsVibratTimerActive(void);

/*****************************************************************************/
// 	Description : 将NV中保存的铃声的种类放入列表中，包括固定铃声和更多音乐
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendNVCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id);
#ifdef MONOCHROME_LCD_SUPPORT 
/*****************************************************************************/
// 	Description : DPHONE 音量调节绘制
//	Global resource dependence : 
//  Author:yiwen.man
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_DrawAdjustVolumePicture(uint8   volume,
								MMI_WIN_ID_T win_id);
#endif
#endif
