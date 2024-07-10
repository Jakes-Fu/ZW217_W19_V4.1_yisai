/*******************************************************************************
** File Name:      mmk_default.h                                               *
** Author:         Great.Tian                                                  *
** Date:           01/2003                                                     *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of            *
**                 system, applicaiton, window and control                     *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                 *
** 01/2003        Great.Tian       Create									   *
*******************************************************************************/
																				
/*!
 *  \addtogroup mmk_default_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_default.h
 *  \author	Great.Tian
 *  \date	Jan,2003
 *  \brief 	This file is used to describe the data struct of system, applicaiton, window and control
 */

#ifndef MMI_DEFAULT_H
#define MMI_DEFAULT_H

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_app.h"
#include "block_mem_def.h"
#include "mmitheme_special.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum
{
    MMI_3D_EFFECT_BROWSER          =  0x0001,
    MMI_3D_EFFECT_ATV              =  0x0002,
    MMI_3D_EFFECT_CAMERA           =  0x0004,
    MMI_3D_EFFECT_JAVA             =  0x0008,
    MMI_3D_EFFECT_VDOPLY           =  0x0010,
    MMI_3D_EFFECT_MTV              =  0x0020,
    MMI_3D_EFFECT_PIC_EDITOR       =  0x0040,
    MMI_3D_EFFECT_UDISK_PC         =  0x0080,
    MMI_3D_EFFECT_MMS_PREVIEW      =  0x0100,
    MMI_3D_EFFECT_CMMB_ENGINE      =  0x0200,    
    MMI_3D_EFFECT_MAGIC_VOICE      =  0x0400,
    MMI_3D_EFFECT_UIREADY_ENGINE   =  0x0800, 
    MMI_3D_EFFECT_VT               =  0x1000,
    MMI_3D_EFFECT_MV               =  0x2000,
#ifdef WRE_SUPPORT
    MMI_3D_EFFECT_WRE              =  0x4000,
#endif
#ifdef PDA_UI_DROPDOWN_WIN
    MMI_3D_EFFECT_DROPDOWN               =  0x8000,
#endif

#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT
    MMI_3D_EFFECT_SCREENCLOSE      =  0x00010000,
#endif
#ifdef MMIEMAIL_SUPPORT
    MMI_3D_EFFECT_EMAIL             =  0x00020000,
#endif
    MMI_3D_EFFECT_MOD              =  0xFFFFFFFF
}MMI_3D_EFFECT_MOD_E;

typedef enum
{
    MMI_BACKLIGHT_ON = 0,
    MMI_BACKLIGHT_HALF_ON,
    MMI_BACKLIGHT_OFF,
    MMI_BACKLIGHT_STATUS_MAX
}MMI_BACKLIGHT_STATUS_E;

//lcd slide status
typedef enum
{
    MMK_SLIDE_OPEN,
    MMK_SLIDE_CLOSE,

    MMK_SLIDE_MAX
} MMK_SLIDE_STATUS_E;

typedef enum
{
    SCR_EFFECT_BUF_TYPE_NONE = 0,
    SCR_EFFECT_BUF_TYPE_WIN_SWITCH,     /* 窗口切换，如进入下一级菜单*/
    SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE = SCR_EFFECT_BUF_TYPE_WIN_SWITCH,   /* picview slide with ripple and fade in/out*/
    SCR_EFFECT_BUF_TYPE_LCD_ROTATE,     /* 屏幕旋转，如Gsensor使屏幕旋转*/
    SCR_EFFECT_BUF_TYPE_MAX
}SCR_EFFECT_BUF_TYPE_E;

// 创建默认控件时的参数
typedef struct
{
    MMI_TEXT_ID_T       title_text_id;          // title text id

    MMI_TEXT_ID_T       softkey_left_id;        // softkey left button text id
    MMI_TEXT_ID_T       softkey_middle_id;      // softkey middle button text id
    MMI_TEXT_ID_T       softkey_right_id;       // softkey right button text id
} MMK_DEFAULT_CTRL_PARAM_T;

#define MMISRVAUD_EARPHONE_EVENT_CLK        0xe100
#define MMISRVAUD_EARPHONE_EVENT_LONG       0xe200
#define MMISRVAUD_EARPHONE_EVENT_DBLCLK     0xe300
typedef uint32 MMISRVAUD_EARPHONE_EVENT_ID;

/*----------------------------------------------------------------------------*/
/*                         Functions                                          */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief handling the key message by default process which may process in all  
 * 	window (include to start backlight and ring other key voice and handle other situation).
 *  \author Louis wei 
 *  \param[in]	keystatus	the message id
 *  \param[in]	keycode		the pointer of message param
 *  \return 
 *		- <i><c>TRUE</c></i>: 	function process sucess	
 * 		- <i><c>FALSE</c></i>:	function process failed 
 */ 
/******************************************************************************/
PUBLIC BOOLEAN MMK_DefaultMSGKbd(
                                 uint32	keystatus,
                                 uint32	keycode
                                 );

/******************************************************************************/
/*! \brief handle the public message after window .
 *  \author Great.Tian 
 *  \param[in]  msg_id	the message id
 *  \param[in]  param_ptr	param
 */
/******************************************************************************/
PUBLIC void MMK_HandlePublicKey(
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param_ptr
                                );
                                
/*****************************************************************************/
// 	Description : Handle red key process
//	Global resource dependence : 
//  Author: Great.Tian
//	Note: 
/*****************************************************************************/
PUBLIC void MMK_HandleRedKeyProcess(void);

/*****************************************************************************/
// 	Description : 是否含statusbar
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinIncludeStatusbar(
                                         MMI_HANDLE_T win_handle
                                         );

/*****************************************************************************/
// 	Description : 是否含softkey
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinIncludeSoftkey(
                                       MMI_HANDLE_T win_handle
                                       );

/*****************************************************************************/
// 	Description : 是否含title
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinIncludeTitle(
                                     MMI_HANDLE_T win_handle
                                     );

/*****************************************************************************/
// 	Description : create default control
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CreateDefaultControl(
                                   MMI_HANDLE_T win_handle,
                                   MMK_DEFAULT_CTRL_PARAM_T *param_ptr
                                   );

/******************************************************************************/
/*! \brief  init the default modual.	
 *  \author Great.Tian  
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_InitDefault(BOOLEAN is_backlight_on);

/******************************************************************************/
/*! \brief  turn on lcd back light and keyboard light
 *  \author Bruce.Chi  
 *  \param[in]  void
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_TurnOnBackLight(void);

/******************************************************************************/
/*! \brief  set the back light of the mmi moudal, include the lcd and the keybad.
 *  \author Great.Tian  
 *  \param[in]  is_light	light or not
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_SetBackLight(BOOLEAN is_light);

/******************************************************************************/
/*! \brief  get the back light state of the mmi moudal.
 *  \author Great.Tian  
 *  \return 
 * 		- <i><c>TRUE</c></i>:		light
 *		- <i><c>FALSE</c></i>:		not light
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_GetBackLightState(void);

/******************************************************************************/
/*! \brief  start the lcd backlight timer         
 *  \author Great.Tian  
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_StartLcdBackLightTimer(void);
                                              
/******************************************************************************/
/*! \brief  stop the lcd backlight timer           
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_StopLcdBackLightTimer(void);

/******************************************************************************/
/*! \brief  whether allow turn off backlight           
 *  \author Great.Tian
 *  \param[in] allow_light whether allow light.
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_AllowTurnOffBackLight(BOOLEAN allow_turn_off);
                                      
/******************************************************************************/
/*! \brief  whether allow backlight           
 *  \author Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>:		do allow
 *		- <i><c>FALSE</c></i>:		dont allow 
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsAllowBackLight(void);
                                           
/******************************************************************************/
/*! \brief  set the backlight of the lcd, this is the fuction setting backlight，and combine LCD Sleep
 *                so it is not permit to call for app
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_SetLcdBackLight(BOOLEAN is_light);
                                       
/******************************************************************************/
/*! \brief  set the backlight of the keybad           
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_SetKeybadBackLight(BOOLEAN is_light);
                                          
/******************************************************************************/
/*! \brief  start the Keybad backlight timer           
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_StartKeybadBackLightTimer(void);
                                                 
/******************************************************************************/
/*! \brief  stop the Keybad backlight timer           
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_StopKeybadBackLightTimer(void);
                                                
/******************************************************************************/
/*! \brief  stop lcd sleep timer         
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_StopLcdSleepTimer(void);
                                         
/******************************************************************************/
/*! \brief  stop lcd sleep timer         
 *  \author Great.Tian
 *  \param[in]  timer_id  current timer id
 *  \return is lcd sleep timer or not
 *		- <i><c>TRUE</c></i>:		is lcd sleep timer
 *		- <i><c>FALSE</c></i>:		is not lcd sleep timer
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsLcdSleepTimer(uint8 timer_id);
                                          
/******************************************************************************/
/*! \brief  get the current flip status         
 *  \author Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>:		open
 *		- <i><c>FALSE</c></i>:		close
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_GetFlipStatus(void);
                                        
/******************************************************************************/
/*! \brief  set the deep sleep status         
 *  \author Great.Tian
 *  \param[in]  is_deepsleep	whether deepsleep
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_SetDeepSleep(BOOLEAN is_deepsleep);
                                    
/******************************************************************************/
/*! \brief  while init can call this fuction and allow DeepSleep, only for PB and SMS calling
 *  \author Great.Tian
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_AllowDeepSleepForMMIInit(void);
                                                
/******************************************************************************/
/*! \brief  judge if MMI INIT over，have allow DEEP SLEEP
 *  \author Tracy Zhang
 *  \return 
 *		- <i><c>TRUE</c></i>:		mmi enable deep sleep
 *		- <i><c>FALSE</c></i>:		mmi dont enable deep sleep
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsMmiEnableDeepSleep(void);

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
/******************************************************************************/
/*! \brief  handle the flip key         
 *  \author Great.Tian
 *  \param[in] is_flipup \n
 *		- <i><c>TRUE</c></i>:		flip up
 *		- <i><c>FALSE</c></i>:		flip down
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_HandleFlipKey(BOOLEAN is_flipup);
#endif
                                     
/******************************************************************************/
/*! \brief  open and close the backlight according to Sleep situation            
 *  \author Great.Tian
 *  \param[in] is_light	whether set the backlight to light or not
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_SetLcdBackLightBySleep(BOOLEAN is_light);
                                              
/******************************************************************************/
/*! \brief  to stop backlight or handle other operation when the backlight timer received   
 *  \author louis.wei
 *  \return 
 * 		- <i><c>TRUE</c></i>:		handle success
 *		- <i><c>FALSE</c></i>:		handle failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_HandleBackLightOver(void);

/******************************************************************************/
/*! \brief  judge if lcd is light     
 *  \author Phoebe.Chen
 *  \return 
 * 		- <i><c>TRUE</c></i>:		lcd is light
 *		- <i><c>FALSE</c></i>:		lcd is not light 
 */
/******************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsBacklightOn(void);

/******************************************************************************/
/*! \brief  Get the Table size.
 *  \author Jassmine.Meng 
 *  \return the size of table
 */
/******************************************************************************/
PUBLIC uint8 MMIDEFAULT_GetKeyTableSize(void);

/*****************************************************************************/
/*! \brief  turn off back light
 *  \author bruce.chi
 *  \return none
 */
/*****************************************************************************/
PUBLIC void MMIDEFAULT_TurnOffBackLight(void);

/*****************************************************************************/
// 	Description : start lcd back light timer according to setting
//	Global resource dependence : 
//  Author: xiangjie
//	Note:
/*****************************************************************************/    
PUBLIC void MMIDEFAULT_StartSettingLCDBackLightTimer(void);

/*****************************************************************************/
// 	Description : handling the key message by default process which may process
//                in all window (include to start backlight and ring other key voice 
//                and handle other situation
//                
//	Global resource dependence : 
//  Author:Robert Lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DefaultMSGKtp(
                          MMI_MESSAGE_ID_E  msg_id
                          );

/*****************************************************************************/
// 	Description : handle other mmi task's signal 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMK_HandleMmiSig(
                             uint16     sig_code,
                             DPARAM     sig_data_ptr
                             );

/*****************************************************************************/
// Description : 3D特效是否开启
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_Is3DMMIEnable(void);

/*****************************************************************************/
// Description : disable 3d effect module. 
// Global resource dependence : 
// Author: .
// Note:
/*****************************************************************************/
PUBLIC void MMI_Disable3DEffect(MMI_3D_EFFECT_MOD_E mod_val);

/*****************************************************************************/
// Description : enable 3d effect module.
// Global resource dependence : 
// Author: .
// Note:
/*****************************************************************************/
PUBLIC void MMI_Enable3DEffect(MMI_3D_EFFECT_MOD_E mod_val);

/*****************************************************************************/
// Description : 是否允许启用3D特效
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_IsPermit3D(BLOCK_MEM_ID_E block_id);

/*****************************************************************************/
//  Description : 保存旧的场景内存
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetTrans3DBuf(
                              uint16 **old_buf_pptr,
                              uint16 **new_buf_pptr
                              );

/*****************************************************************************/
//  Description : 保存旧的场景内存
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_E buf_type);

/*****************************************************************************/
//  Description : 保存旧的场景内存到New Buff
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SaveMoveToNewBuf(void);

/*****************************************************************************/
//  Description : 释放3d场景切换的内存，场景切换结束时调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_E buf_type);

/*****************************************************************************/
//  Description : 特效灭屏
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMISEPC_CloseAllLight(void);

/*****************************************************************************/
//  Description : 灭屏
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_CloseAllLight(void);
/*****************************************************************************/
//  Description : stop the auto keylock timer           
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_StopAutoKeylockTimer(void);
/*****************************************************************************/
//  Description : to stop auto keylock or handle other operation when the keylock
//                timer received   
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_HandleAutoKeylockOver( void );
/*****************************************************************************/
//  Description : start the auto keylock timer           
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_StartAutoKeylockTimer(void);
/******************************************************************************/
/*! \brief  whether allow open key lock        
 *  \author apple.zhang
 *  \param[in] allow_keylock whether allow keylock.
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_AllowOpenKeylocWin(BOOLEAN allow_keylock);

/*****************************************************************************/
//  Description : convert qwerty key to typical key
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIDEFAULT_ConvertQwertyKeyToTypicalKey( uint8 src_key );

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
/*****************************************************************************/
//  Description : convert typical key to qwerty key
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIDEFAULT_ConvertTypicalKeyToQwertyKey( uint8 src_key );

/*****************************************************************************/
//  Description : is shift key
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsShiftKey( 
                                     uint8 key
                                     );

/*****************************************************************************/
//  Description : get shift key
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIDEFAULT_GetShiftKey(uint8 src_key);

#endif
         
#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
//  Description : convert slide key           
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_ConvertSlideKey(
                                       uint32 *key_code_ptr,
                                       uint16 *single_code_ptr
                                       );

/*****************************************************************************/
//  Description : get the slide status         
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC MMK_SLIDE_STATUS_E MMIDEFAULT_GetSlideStatus(void);

/*****************************************************************************/
//  Description : handle the slide key         
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_HandleSlideKey(
                                      MMI_MESSAGE_ID_E  msg_id //[IN] TRUE: flip up; FALSE: flip down
                                      );
#endif

/*****************************************************************************/
//  Description : whether allow turn off backlight  (是否允许关闭背光)         
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsAllowBackLightTurnOff(void);

/*****************************************************************************/
// Description : Make a judgement that if a Win is POPUP Win.
// Global resource dependence : 
// Author: Yintang.Ren
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsPopupWin(
                                     MMI_HANDLE_T win_handle
                                     );

/*****************************************************************************/
// Description : 判断窗口是否全屏的list窗口
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 暂时的使用接口，因为这样判断并不严谨，如一个窗口包含
// 多个list时或者list是半屏窗口时，不能这么判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsFullListWin(
                                        MMI_HANDLE_T win_handle
                                        );

/*****************************************************************************/
// Description : 判断是否主菜单
// Global resource dependence : 
// Author: Yintang.Ren
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsMainMenuWin(
                                        MMI_HANDLE_T win_handle
                                        );

#ifdef MAINLCD_SIZE_128X64
/*****************************************************************************/
// 	Description : set the tone on       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SetToneStates(
										BOOLEAN is_on//TRUE: on, FALSE:off
									 );

/*****************************************************************************/
// 	Description : set the handfree states       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SetHandFreeStatesAndLed(
										        BOOLEAN state //TRUE:handfree used,FALSE:handfree unused
										       );

/*****************************************************************************/
// 	Description : init the hook hand free and tone status       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_InitHookAndHF(void);


/*****************************************************************************/
// 	Description :     
//	Global resource dependence : 
//  Author: Will.Shao
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsToneOn(void);

/*****************************************************************************/
// 	Description : set the hook states       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SetHookStates(
									 BOOLEAN state //TRUE:hook used,FALSE:hook unused
									 );


/*****************************************************************************/
// 	Description : set the handfree states       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SetHandFreeStates(
										 BOOLEAN state //TRUE:handfree used,FALSE:handfree unused
										 );
/*****************************************************************************/
// 	Description : get the handfree states       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_GetHandFreeStates(void);

/*****************************************************************************/
// 	Description : get the hook states       
//	Global resource dependence : 
//  Author: alex.ye
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_GetHookStates(void);
#endif //dphone_support
 
#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
// Description : Do the prepare for LCD rotate.
// Global resource dependence : 
// Author: Yintang.Ren
// Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_HandlePreLCDRotate(
                                          void
                                          );
#endif

/*****************************************************************************/
//  Description : cancel current window effect          
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_CancelCurWin3DEffect(void);

/*****************************************************************************/
//  Description : set current window effect          
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_SetCurWin3DEffect(MMI_WIN_ANIM_TYPE_E type);

/*****************************************************************************/
//  Description : get the back light state of the mmi moudal           
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_BACKLIGHT_STATUS_E MMIDEFAULT_GetBackLightStateEx(void);


/*****************************************************************************/
//  Description : Recording the time of key ring turn off.
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_EnableKeyRing(char *modelname_ptr, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : Recording the time of tp ring turn off.
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_EnableTpRing(const char *modelname_ptr, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : Recording the time of tp and key ring turn off.
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_EnableKeyTpRing(char *modelname_ptr, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : If enable TP down ring.
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsEnableTpRing(void);

/*****************************************************************************/
//  Description : If enable key down ring.
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsEnableKeyRing(void);

/*****************************************************************************/
//  Description : judge whether can play new message alert ring
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_IsPermitPlayNewMsgRing(void);

/*****************************************************************************/
//  Description : start screen save timer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_StartScreenSaverTimer(void);

/*****************************************************************************/
//  Description : stop screen save timer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDEFAULT_StopScreenSaverTimer(void);

/*****************************************************************************/
//  Description : handle screen save timer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEFAULT_HandleScreenSaverTimer( void );

#ifdef MOTION_SENSOR_TYPE_OPTICAL
/*****************************************************************************/
// 	Description : handle optical sensor msg
//	Global resource dependence : none
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDEFAULT_HandleOpsensorMsg(
                                        DPARAM param
                                        );
#endif

PUBLIC void MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_ID event, uint32 keycode);
PUBLIC void MMIDEFAULT_OnlyTurnOffBackLight(void);
/*****************************************************************************/
//  Description : close all light
//  Global resource dependence : 
//  Author:chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDEFAULT_CloseAllLight_Watch(void);


#ifdef   __cplusplus
    }
#endif

#endif  /* MMI_DEFAULT_H */
/*! @} */

