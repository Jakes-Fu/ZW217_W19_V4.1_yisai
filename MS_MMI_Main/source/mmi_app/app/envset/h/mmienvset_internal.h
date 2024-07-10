/*****************************************************************************
** File Name:      mmienvset_internal.h                                               *
** Author:                                                                   *
** Date:           23/12/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe environment setting         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2004       maqianlin          Create
** 02/2012       xiyuan.ma          Edit
******************************************************************************/

#ifndef _MMIENVSET_INTERNAL_H_
#define _MMIENVSET_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

//#include    "mmiset_wintab.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_custom_define.h"
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
    
    // -----------------------------------------  来电音量-----------------------------------------------------------------
    // 普通环境
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_CALL_RING_VOL_STANDARD_MODE  MMISET_VOL_NINE
#else    
#define MMIENVSET_CALL_RING_VOL_STANDARD_MODE  MMISET_VOL_NINE
#endif 
    // 安静环境
#define MMIENVSET_CALL_RING_VOL_SILENT_MODE	   MMISET_VOL_ZERO
    // 会议环境
#define MMIENVSET_CALL_RING_VOL_MEETING_MODE   MMISET_VOL_ZERO
    // 车内环境
#define MMIENVSET_CALL_RING_VOL_INSIDE_MODE    MMISET_VOL_SEVEN
    // 户外环境
#define MMIENVSET_CALL_RING_VOL_NOISY_MODE     MMISET_VOL_MAX
   //耳机模式
#define MMIENVSET_CALL_RING_VOL_EARPHONE_MODE  MMISET_VOL_FIVE
   //蓝牙模式
#define MMIENVSET_CALL_RING_VOL_BLUETOOTH_MODE MMISET_VOL_FIVE
    // 飞行环境
//#define MMIENVSET_CALL_RING_VOL_FLY_MODE       MMISET_VOL_FIVE
    
    // ---------------------------------------------------短信音量----------------------------------------------------------
    // 普通环境
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_MSG_RING_VOL_STANDARD_MODE   MMISET_VOL_NINE
#else    
#define MMIENVSET_MSG_RING_VOL_STANDARD_MODE   MMISET_VOL_NINE
#endif
    // 安静环境
#define MMIENVSET_MSG_RING_VOL_SILENT_MODE	   MMISET_VOL_ZERO
    // 会议环境
#define MMIENVSET_MSG_RING_VOL_MEETING_MODE    MMISET_VOL_ZERO
    // 车内环境 
#define MMIENVSET_MSG_RING_VOL_INSIDE_MODE     MMISET_VOL_SIX
    // 户外环境
#define MMIENVSET_MSG_RING_VOL_NOISY_MODE      MMISET_VOL_MAX
   //耳机模式
#define MMIENVSET_MSG_RING_VOL_EARPHONE_MODE   MMISET_VOL_TWO
   //蓝牙模式
#define MMIENVSET_MSG_RING_VOL_BLUETOOTH_MODE  MMISET_VOL_FIVE
    // 飞行环境
//#define MMIENVSET_MSG_RING_VOL_FLY_MODE        MMISET_VOL_FOUR
    
    // -------------------------------------------------通话音量-----------------------------------------------------------
    // 普通环境
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_COMM_VOL_STANDARD_MODE       MMISET_VOL_SEVEN
#else    
#define MMIENVSET_COMM_VOL_STANDARD_MODE       MMISET_VOL_EIGHT
#endif 
    // 安静环境
#define MMIENVSET_COMM_VOL_SILENT_MODE	       MMISET_VOL_SEVEN
    // 会议环境
#define MMIENVSET_COMM_VOL_MEETING_MODE        MMISET_VOL_SEVEN
    // 车内环境
#define MMIENVSET_COMM_VOL_INSIDE_MODE         MMISET_VOL_SEVEN
    // 户外环境 
#define MMIENVSET_COMM_VOL_NOISY_MODE          MMISET_VOL_MAX
    //耳机模式
#define MMIENVSET_COMM_VOL_EARPHONE_MODE       MMISET_VOL_SEVEN
    //蓝牙模式
#define MMIENVSET_COMM_VOL_BLUETOOTH_MODE      MMISET_VOL_SEVEN

    // 飞行环境
//#define MMIENVSET_COMM_VOL_FLY_MODE            MMISET_VOL_FIVE
    
    // -------------------------------------------------按键音量------------------------------------------------------------
    // 普通环境
#define MMIENVSET_KEY_RING_VOL_STANDARD_MODE       MMISET_VOL_SIX
    // 安静环境
#define MMIENVSET_KEY_RING_VOL_SILENT_MODE	       MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // 会议环境
#define MMIENVSET_KEY_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // 车内环境
#define MMIENVSET_KEY_RING_VOL_INSIDE_MODE         MMISET_VOL_THREE
    // 户外环境
#define MMIENVSET_KEY_RING_VOL_NOISY_MODE          MMISET_VOL_SEVEN
    //耳机模式
#define MMIENVSET_KEY_RING_VOL_EARPHONE_MODE       MMISET_VOL_TWO
    //蓝牙模式
#define MMIENVSET_KEY_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_SIX

    // 飞行环境
//#define MMIENVSET_KEY_RING_VOL_FLY_MODE            MMISET_VOL_THREE
     // -------------------------------------------------触屏音音量------------------------------------------------------------
    // 普通环境
#define MMIENVSET_TKEY_RING_VOL_STANDARD_MODE       MMISET_VOL_SIX
    // 安静环境
#define MMIENVSET_TKEY_RING_VOL_SILENT_MODE	        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // 会议环境
#define MMIENVSET_TKEY_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // 车内环境
#define MMIENVSET_TKEY_RING_VOL_INSIDE_MODE         MMISET_VOL_THREE
    // 户外环境
#define MMIENVSET_TKEY_RING_VOL_NOISY_MODE          MMISET_VOL_SEVEN
    //耳机模式
#define MMIENVSET_TKEY_RING_VOL_EARPHONE_MODE       MMISET_VOL_TWO
    //蓝牙模式
#define MMIENVSET_TKEY_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_SIX
    // --------------------------------------------------开关机音量-------------------------------------------------------------
    // 普通环境
#define MMIENVSET_POWER_RING_VOL_STANDARD_MODE      MMISET_VOL_NINE
    // 安静环境 
#define MMIENVSET_POWER_RING_VOL_SILENT_MODE	    MMISET_VOL_ZERO
    // 会议环境
#define MMIENVSET_POWER_RING_VOL_MEETING_MODE       MMISET_VOL_ZERO
    // 车内环境
#define MMIENVSET_POWER_RING_VOL_INSIDE_MODE        MMISET_VOL_ONE
    // 户外环境
#define MMIENVSET_POWER_RING_VOL_NOISY_MODE         MMISET_VOL_MAX
    //耳机模式
#define MMIENVSET_POWER_RING_VOL_EARPHONE_MODE      MMISET_VOL_FOUR
    //蓝牙模式
#define MMIENVSET_POWER_RING_VOL_BLUETOOTH_MODE     MMISET_VOL_FOUR
    // 飞行环境
//#define MMIENVSET_POWER_RING_VOL_FLY_MODE           MMISET_VOL_THREE
    
#ifdef MMI_READMENU_ENABLE
// --------------------------------------------------菜单朗读音量-------------------------------------------------------------
// 普通环境
#define MMIENVSET_MENU_READ_VOL_STANDARD_MODE       MMISET_VOL_THREE
// 安静环境 
#define MMIENVSET_MENU_READ_VOL_SILENT_MODE	         MMISET_VOL_ZERO
// 会议环境
#define MMIENVSET_MENU_READ_VOL_MEETING_MODE        MMISET_VOL_ZERO
// 车内环境
#define MMIENVSET_MENU_READ_VOL_INSIDE_MODE         MMISET_VOL_FIVE
// 户外环境
#define MMIENVSET_MENU_READ_VOL_NOISY_MODE          MMISET_VOL_MAX
//耳机模式
#define MMIENVSET_MENU_READ_VOL_EARPHONE_MODE       MMISET_VOL_THREE
//蓝牙模式
#define MMIENVSET_MENU_READ_VOL_BLUETOOTH_MODE      MMISET_VOL_THREE
// 飞行环境
//#define MMIENVSET_POWER_RING_VOL_FLY_MODE           MMISET_VOL_THREE
#endif

    // ----------------------------------------------来电提示类型----------------------------------------------------------------
    // 普通环境
#define MMIENVSET_CALL_RING_TYPE_STANDARD_MODE      MMISET_CALL_VIBRA_AND_RING
    // 安静环境
#define MMIENVSET_CALL_RING_TYPE_SILENT_MODE        MMISET_CALL_SILENT
    // 会议环境
#define MMIENVSET_CALL_RING_TYPE_MEETING_MODE       MMISET_CALL_VIBRA
    // 车内环境
#define MMIENVSET_CALL_RING_TYPE_INSIDE_MODE        MMISET_CALL_RING
    // 户外环境
#define MMIENVSET_CALL_RING_TYPE_NOISY_MODE         MMISET_CALL_VIBRA_AND_RING
    //耳机模式
#define MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE      MMISET_CALL_RING
    //蓝牙模式
#define MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE     MMISET_CALL_RING
    // 飞行环境
//#define MMIENVSET_CALL_RING_TYPE_FLY_MODE      MMISET_CALL_RING
    
    // ------------------------------------------------短信提示方式----------------------------------------------------------------
    // 普通环境
#define MMIENVSET_MSG_RING_TYPE_STANDARD_MODE  MMISET_MSG_VIBRA_AND_RING
    // 安静环境
#define MMIENVSET_MSG_RING_TYPE_SILENT_MODE	   MMISET_MSG_SILENT
    // 会议环境
#define MMIENVSET_MSG_RING_TYPE_MEETING_MODE   MMISET_MSG_VIBRA
// 车内环境
#define MMIENVSET_MSG_RING_TYPE_INSIDE_MODE    MMISET_MSG_RING
// 户外环境
#define MMIENVSET_MSG_RING_TYPE_NOISY_MODE     MMISET_MSG_VIBRA_AND_RING
    //耳机模式
#define MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE  MMISET_MSG_RING
    //蓝牙模式
#define MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE MMISET_MSG_RING
// 飞行环境
//#define MMIENVSET_MSG_RING_TYPE_FLY_MODE       MMISET_MSG_RING

// -------------------------------------------- 按键音 类型 ------------------------------------------------------------------
// 普通环境
#define MMIENVSET_KEY_RING_TYPE_STANDARD_MODE         MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_KEY_RING_TYPE_SILENT_MODE	          MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_KEY_RING_TYPE_MEETING_MODE          MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_KEY_RING_TYPE_INSIDE_MODE           MMISET_OTHER_RING1
// 户外环境
#define MMIENVSET_KEY_RING_TYPE_NOISY_MODE            MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_KEY_RING_TYPE_EARPHONE_MODE         MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_KEY_RING_TYPE_BLUETOOTH_MODE        MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_KEY_RING_TYPE_FLY_MODE              MMISET_OTHER_RING1

// -------------------------------------------- 触屏音 类型 ------------------------------------------------------------------
#define	MMIENVSET_TKEY_RING_TYPE_SWITCH_OPEN					0
#define	MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE					1	
// 普通环境
#define MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE         MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// 安静环境
#define MMIENVSET_TKEY_RING_TYPE_SILENT_MODE	       MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_TKEY_RING_TYPE_MEETING_MODE          MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_TKEY_RING_TYPE_INSIDE_MODE           MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// 户外环境
#define MMIENVSET_TKEY_RING_TYPE_NOISY_MODE            MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
    //耳机模式
#define MMIENVSET_TKEY_RING_TYPE_EARPHONE_MODE         MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
    //蓝牙模式
#define MMIENVSET_TKEY_RING_TYPE_BLUETOOTH_MODE        MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// 飞行环境
//#define MMIENVSET_TKEY_RING_TYPE_FLY_MODE              MMIENVSET_TKEY_RING_TYPE_SWITCH_OPEN

// -----------------------------------------翻盖提示音类型 ---------------------------------------------------------------------
// 普通环境
#define MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_FLIP_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_FLIP_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_FLIP_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// 户外环境
#define MMIENVSET_FLIP_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_FLIP_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_FLIP_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------翻盖提示音类型 ---------------------------------------------------------------------
// 普通环境
#define MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_FLIP_DOWN_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_FLIP_DOWN_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_FLIP_DOWN_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// 户外环境
#define MMIENVSET_FLIP_DOWN_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_FLIP_DOWN_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_FLIP_DOWN_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------滑盖（开盖）提示音类型 ---------------------------------------------------------------------
// 普通环境
#define MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_SLIDE_ON_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_SLIDE_ON_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_SLIDE_ON_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// 户外环境
#define MMIENVSET_SLIDE_ON_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_SLIDE_ON_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_SLIDE_ON_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------滑盖（关盖）提示音类型 ---------------------------------------------------------------------
// 普通环境
#define MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_SLIDE_OFF_RING_TYPE_SILENT_MODE	       MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_SLIDE_OFF_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_SLIDE_OFF_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// 户外环境
#define MMIENVSET_SLIDE_OFF_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_SLIDE_OFF_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_SLIDE_OFF_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------开启/关闭低电压告警音--------------------------------------------------------------
#define	MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN					0
#define	MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE					1						
// 普通环境
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE          MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// 安静环境
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_SILENT_MODE	        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_MEETING_MODE           MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_INSIDE_MODE            MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// 户外环境
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_NOISY_MODE             MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_EARPHONE_MODE          MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_BLUETOOTH_MODE         MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// 飞行环境
//#define MMIENVSET_NOTIFY_SWITCH_SWITCH_FLY_MODE               MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

// -----------------------------------------开启/关闭低电压告警音--------------------------------------------------------------
#define	MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN					0
#define	MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE					1						
// 普通环境
#define MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE          MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// 安静环境
#define MMIENVSET_VOLTAGE_WARN_SWITCH_SILENT_MODE	         MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_VOLTAGE_WARN_SWITCH_MEETING_MODE           MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_VOLTAGE_WARN_SWITCH_INSIDE_MODE            MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// 户外环境
#define MMIENVSET_VOLTAGE_WARN_SWITCH_NOISY_MODE             MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
    //耳机模式
#define MMIENVSET_VOLTAGE_WARN_SWITCH_EARPHONE_MODE          MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
    //蓝牙模式
#define MMIENVSET_VOLTAGE_WARN_SWITCH_BLUETOOTH_MODE         MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
// 飞行环境
//#define MMIENVSET_VOLTAGE_WARN_SWITCH_FLY_MODE               MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN

// -----------------------------------------开启/关闭开始充电提示告警音-------------------------------------------------------
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN                 1
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE                0
// 普通环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE          MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// 安静环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_SILENT_MODE            MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_MEETING_MODE           MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_INSIDE_MODE            MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// 户外环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_NOISY_MODE             MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// 耳机模式
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_EARPHONE_MODE          MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// 蓝牙模式
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_BLUETOOTH_MODE         MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// 飞行环境
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_FLY_MODE               MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE

// -----------------------------------------开启/关闭开关机声音--------------------------------------------------------------
#define	MMIENVSET_POWER_RING_SWITCH_OPEN					0
#define	MMIENVSET_POWER_RING_SWITCH_CLOSE					1						
// 普通环境
#define MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE          MMIENVSET_POWER_RING_SWITCH_OPEN
// 安静环境
#define MMIENVSET_POWER_RING_SWITCH_SILENT_MODE	           MMIENVSET_POWER_RING_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_POWER_RING_SWITCH_MEETING_MODE           MMIENVSET_POWER_RING_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_POWER_RING_SWITCH_INSIDE_MODE            MMIENVSET_POWER_RING_SWITCH_OPEN
// 户外环境
#define MMIENVSET_POWER_RING_SWITCH_NOISY_MODE             MMIENVSET_POWER_RING_SWITCH_OPEN
    //耳机模式
#define MMIENVSET_POWER_RING_SWITCH_EARPHONE_MODE	       MMIENVSET_POWER_RING_SWITCH_OPEN
    //蓝牙模式
#define MMIENVSET_POWER_RING_SWITCH_BLUETOOTH_MODE	       MMIENVSET_POWER_RING_SWITCH_OPEN
// 飞行环境
//#define MMIENVSET_POWER_RING_SWITCH_FLY_MODE               MMIENVSET_POWER_RING_SWITCH_OPEN

// -----------------------------------------闹钟铃声音量-------------------------------------------------------------------------
// 普通环境
#define MMIENVSET_ALARM_RING_VOL_STANDARD_MODE       MMISET_VOL_NINE
// 安静环境
#define MMIENVSET_ALARM_RING_VOL_SILENT_MODE	     MMISET_VOL_ZERO
// 会议环境
#define MMIENVSET_ALARM_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO
// 车内环境
#define MMIENVSET_ALARM_RING_VOL_INSIDE_MODE         MMISET_VOL_FIVE
// 户外环境
#define MMIENVSET_ALARM_RING_VOL_NOISY_MODE          MMISET_VOL_MAX
    //耳机模式
#define MMIENVSET_ALARM_RING_VOL_EARPHONE_MODE       MMISET_VOL_FOUR
    //蓝牙模式
#define MMIENVSET_ALARM_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_FOUR
// 飞行环境
//#define MMIENVSET_ALARM_RING_VOL_FLY_MODE           MMISET_VOL_THREE

// -----------------------------------------闹钟提示方式-------------------------------------------------------------------------
// 普通环境
#define MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE      MMISET_MSG_VIBRA_AND_RING
// 安静环境
#define MMIENVSET_ALARM_RING_TYPE_SILENT_MODE	     MMISET_MSG_SILENT
// 会议环境
#define MMIENVSET_ALARM_RING_TYPE_MEETING_MODE       MMISET_MSG_VIBRA
// 车内环境
#define MMIENVSET_ALARM_RING_TYPE_INSIDE_MODE        MMISET_MSG_RING
// 户外环境
#define MMIENVSET_ALARM_RING_TYPE_NOISY_MODE         MMISET_MSG_VIBRA_AND_RING
    //耳机模式
#define MMIENVSET_ALARM_RING_TYPE_EARPHONE_MODE      MMISET_MSG_RING
    //蓝牙模式
#define MMIENVSET_ALARM_RING_TYPE_BLUETOOTH_MODE     MMISET_MSG_RING
// 飞行环境
//#define MMIENVSET_ALARM_RING_TYPE_FLY_MODE           MMISET_MSG_RING

// -------------------------------------------开关机铃声类型---------------------------------------------------------------------
// 普通环境
#define MMIENVSET_POWER_RING_TYPE_STANDARD_MODE      MMISET_OTHER_RING1
// 安静环境
#define MMIENVSET_POWER_RING_TYPE_SILENT_MODE	     MMISET_OTHER_SILENT
// 会议环境
#define MMIENVSET_POWER_RING_TYPE_MEETING_MODE       MMISET_OTHER_SILENT
// 车内环境
#define MMIENVSET_POWER_RING_TYPE_INSIDE_MODE        MMISET_OTHER_RING1
// 户外环境
#define MMIENVSET_POWER_RING_TYPE_NOISY_MODE         MMISET_OTHER_RING1
    //耳机模式
#define MMIENVSET_POWER_RING_TYPE_EARPHONE_MODE	     MMISET_OTHER_RING1
    //蓝牙模式
#define MMIENVSET_POWER_RING_TYPE_BLUETOOTH_MODE	 MMISET_OTHER_RING1
// 飞行环境
//#define MMIENVSET_POWER_RING_TYPE_FLY_MODE         MMISET_OTHER_RING1

//-----------------------------------------拨号盘触屏振动------------------------------------------------------------------------
#define	MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN					0
#define	MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE					1						

// 普通环境
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE          MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
// 安静环境
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_SILENT_MODE	        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_MEETING_MODE           MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_INSIDE_MODE            MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
// 户外环境
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_NOISY_MODE             MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
    //耳机模式
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_EARPHONE_MODE          MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
    //蓝牙模式
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_BLUETOOTH_MODE         MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
//-----------------------------------------拨号音------------------------------------------------------------------------
#define	MMIENVSET_DIAL_TONE_SWITCH_OPEN					    0
#define	MMIENVSET_DIAL_TONE_SWITCH_CLOSE					1						

// 普通环境
#define MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE          MMIENVSET_DIAL_TONE_SWITCH_OPEN
// 安静环境
#define MMIENVSET_DIAL_TONE_SWITCH_SILENT_MODE	        MMIENVSET_DIAL_TONE_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_DIAL_TONE_SWITCH_MEETING_MODE           MMIENVSET_DIAL_TONE_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_DIAL_TONE_SWITCH_INSIDE_MODE            MMIENVSET_DIAL_TONE_SWITCH_OPEN
// 户外环境
#define MMIENVSET_DIAL_TONE_SWITCH_NOISY_MODE             MMIENVSET_DIAL_TONE_SWITCH_OPEN
    //耳机模式
#define MMIENVSET_DIAL_TONE_SWITCH_EARPHONE_MODE          MMIENVSET_DIAL_TONE_SWITCH_OPEN
    //蓝牙模式
#define MMIENVSET_DIAL_TONE_SWITCH_BLUETOOTH_MODE         MMIENVSET_DIAL_TONE_SWITCH_OPEN
#endif

//-----------------------------------------锁屏音振动------------------------------------------------------------------------
#define	MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN					0
#define	MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE					1						

// 普通环境
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE          MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
// 安静环境
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_SILENT_MODE	         MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_MEETING_MODE           MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_INSIDE_MODE            MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
// 户外环境
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_NOISY_MODE             MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
    //耳机模式
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_EARPHONE_MODE          MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
    //蓝牙模式
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_BLUETOOTH_MODE         MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
#ifdef MMI_READMENU_ENABLE
//-----------------------------------------菜单朗读------------------------------------------------------------------------

#define	MMIENVSET_MENU_READ_SWITCH_OPEN					        0
#define	MMIENVSET_MENU_READ_SWITCH_CLOSE			            1						

// 普通环境
#define MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE                MMIENVSET_MENU_READ_SWITCH_OPEN
// 安静环境
#define MMIENVSET_MENU_READ_SWITCH_SILENT_MODE	                MMIENVSET_MENU_READ_SWITCH_CLOSE
// 会议环境
#define MMIENVSET_MENU_READ_SWITCH_MEETING_MODE                 MMIENVSET_MENU_READ_SWITCH_CLOSE
// 车内环境
#define MMIENVSET_MENU_READ_SWITCH_INSIDE_MODE                  MMIENVSET_MENU_READ_SWITCH_OPEN
// 户外环境
#define MMIENVSET_MENU_READ_SWITCH_NOISY_MODE                   MMIENVSET_MENU_READ_SWITCH_OPEN
//耳机模式
#define MMIENVSET_MENU_READ_SWITCH_EARPHONE_MODE                MMIENVSET_MENU_READ_SWITCH_OPEN
//蓝牙模式
#define MMIENVSET_MENU_READ_SWITCH_BLUETOOTH_MODE               MMIENVSET_MENU_READ_SWITCH_OPEN
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 在环境设置的一级菜单(listbox)里追加(append)列表项
//	Global resource dependence : none
// Author: maqianlin
//	Note:  这个listbox是文本列表
/*****************************************************************************/
void MMIENVSET_MainMenu_Append_LBItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      );


/*****************************************************************************/
// Description : 把新的环境名称写入nv
//	Global resource dependence : none
// Author: maqianlin
//	Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_EnvName_WriteInNV(
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
// 	Description : 列出所有的铃声类型
//	 Global resource dependence : 
//  Author: maqianlin
//	 Note:
/*****************************************************************************/
void MMIENVSET_RingType_AppendListItem(
                                       MN_DUAL_SYS_E dual_sys,
                                       MMI_CTRL_ID_T	        ctrl_id,
                                       MMISET_ALL_RING_TYPE_E   ring_type,
                                       MMIENVSET_SET_OPT_E      set_opt
                                       );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//   Description : 下载完成开启和关闭列表
//   Global resource dependence : none
//   Author: jiaoyou.wu
//   Note:
//*****************************************************************************/
void MMIENVSET_DownloadCompleteNotify_AppendListItem(
                                       MMI_CTRL_ID_T        ctrl_id
                                       );

/*****************************************************************************/
// 	Description : 低电压报警开启和关闭列表
//	 Global resource dependence : none
//  Author: maqianlin
//	 Note:
//*****************************************************************************/
void MMIENVSET_VoltWarn_AppendListItem(
                                       MMI_CTRL_ID_T	    ctrl_id
                                       );

/*****************************************************************************/
// 	 Description : 设置开关机声音选项的列表
//	 Global resource dependence : none
//   Author: kelly.li
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_AppendPowerRingSwitchListItem(
                                                    MMI_CTRL_ID_T	    ctrl_id
                                                    );
#endif

/*****************************************************************************/
// 	Description : 记录前一次的情景模式，供外部使用
//	 Global resource dependence : none
//  Author: juan.wu
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_SetPreActiveMode(uint8 mode_id);

/*****************************************************************************/
// Description :判断现设置的环境模式是否正在使用的环境模式,是返回true ，不是返回false 
//	Global resource dependence : none
// Author: maqianlin
//	Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_IsActiveMode(uint8 mode_id);

/*****************************************************************************/
//  Description : 初始化编辑框
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void ENVSET_InitChangeNameTextEditBox(
                                      MMI_CTRL_ID_T ctrl_id
                                      );

/*****************************************************************************/
//  Description : 通过指定音量预览铃声
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/****************************************************************************/
void MMIENVSET_PreviewRingByVol(
                                MN_DUAL_SYS_E dual_sys,
                                MMISET_ALL_RING_TYPE_E  ring_type,
                                uint8 				    ring_vol,
                                uint8				    play_times
                                );
/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void   MMIENVSET_PlayRingByListItem(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMISET_ALL_RING_TYPE_E  	ring_type,   //铃声类型
                                    MMI_CTRL_ID_T	       	ctrl_id,      //控件ID
                                    uint8                       play_times   //播放次数
                                    );



/*****************************************************************************/
//  Description :通过指定ID 播放铃声
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMIENVSET_PreviewRingById(
                               MMISET_ALL_RING_TYPE_E  ring_type,	
                               uint8 				    ring_id,
                               uint8				    play_times   //播放次数
                               );


/*****************************************************************************/
//Description : 关闭窗口之前关闭所有的震动和铃声 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
void   MMIENVSET_StopVibraAndRing(
                                  MMISET_ALL_RING_TYPE_E  ring_type  //铃声类型
                                  );


/*****************************************************************************/
//Description : 震动时间到 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
void   MMIENVSET_VibraTimerOut(
                               MN_DUAL_SYS_E dual_sys,
                               MMISET_ALL_RING_TYPE_E   ring_type,   //铃声类型
                               MMI_CTRL_ID_T	       ctrl_id     //控件ID
                               );

#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//  Description :设置启动环境设置的图标 
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMIENVSET_SetActiveModeIcon(void);
#endif
/*****************************************************************************/
//  Description : 打开环境设置主菜单                        
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_OpenMainMenuWindow(void) ;

/*****************************************************************************/
// 	Description : play music file 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_GetPlayResult(void);

/*****************************************************************************/
// 	Description : play music file 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENVSET_SetPlayResult(BOOLEAN result);

/*****************************************************************************/
//  Description : 删除自定义情景模式
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_HandleDeleteItem(
                                    uint16 item_index // IN
                                    );

/*****************************************************************************/
//  Description : 无修改添加自定义情景模式
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_HandleAddItem(void);

/*****************************************************************************/
//  Description : 获取真正的模式ID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 根据原有设计，自建模式可能会被删除，中间会漏空，引用原函数GetEnvRealIndex
/*****************************************************************************/
PUBLIC uint16 MMIENVSET_GetEnvRealIndex(uint8 select_index);

/*************************************************************************************/
//  Description : update main menu list
//  Global resource dependence : none
//  Author: jian.ma
//  Note:  
/**************************************************************************************/
PUBLIC void MMIENVSET_UpdateMainMenuList(MMI_CTRL_ID_T ctrl_id);

//

/*****************************************************************************/
//Description : 开启震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: maryxiao
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartWinOnetimeVibTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//Description : 判断是否震动计时器时间到 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerOut(DPARAM param);

/*****************************************************************************/
//Description : 开启震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: xiaoqing.lu 
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartOnetimeVibTimer(void);

/*****************************************************************************/
//Description : 开启震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: maryxiao
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartWinOnetimeVibTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : create more ring dir if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_CreateMoreRingDir(void);

/*****************************************************************************/
//  Description : create more ring ini file if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_CreateMoreRingFile(void);

/*****************************************************************************/
// 	Description : to enter ring setting menu
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_EnterRingSettingMenu(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
