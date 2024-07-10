/*****************************************************************************
** File Name:      mmiidle_subwintab.h                                           *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
** 02/2012       murphy.xie          Change Name
******************************************************************************/

#ifndef _MMIIDLE_SUBWINTAB_H_
#define _MMIIDLE_SUBWINTAB_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmimp3_export.h"
#include "mmialarm_export.h"
#include "mmipb_export.h"
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
#ifndef SUBLCD_SIZE_NONE
#define CALL_INFO_MAX_LEN           MMIPB_MAX_STRING_LEN
#define MISSED_CALL_INFO_MAX_LEN    MMIPB_MAX_STRING_LEN

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//小屏显示的内容
typedef enum
{
    SUB_CONTENT_NONE,                   //没有
    SUB_CONTENT_DATE,                   //日期
    SUB_CONTENT_PWD,                    //输入开机密码
    SUB_CONTENT_PIN,                    //输入PIN码
    SUB_CONTENT_PUK,                    //输入PUK码
    SUB_CONTENT_SIM_REJECTED,           //SIM卡被拒
    SUB_CONTENT_SIM_REGISTRATION_FAILED,//SIM卡注册失败
    SUB_CONTENT_PUK_BLOCKED,            //PUK被锁
    SUB_CONTENT_PUK2_BLOCKED,           //PUK2被锁
    SUB_CONTENT_NO_SIM,                 //没有SIM卡
    SUB_CONTENT_LOW_BAT,                //电量低
    SUB_CONTENT_MP3,                    //Mp3 name
    SUB_CONTENT_ALARM,                  //Alarm enevt
    SUB_CONTENT_COUNTEDTIME,    //COUNTEDTIME enevt,    @juan.zhao for sub_lcd 20100301
    SUB_CONTENT_FM,    //fM信息,    @juan.zhao for sub_lcd 20100301
    SUB_CONTENT_CALL,                   //call event
    SUB_CONTENT_MISSED_CALL,            //missed call info
    SUB_CONTENT_CHARGE_OFF,             //关机充电
    SUB_CONTENT_CHARGE_FINISH,          //关机充电充满
    SUB_CONTENT_CHARGE_OVER_VOLTAGE,    //关机充电充电电压过大
    SUB_CONTENT_CAMERA_WAIT,            //照相等待中
    SUB_CONTENT_SIM_LOCK,            //SIM Locked
    SUB_CONTENT_MAX_NUM
} SUB_CONTENT_E;

//小屏显示mp3 name信息
typedef struct
{
//    BOOLEAN     is_ucs2;
    wchar       name[MMIMP3_NAME_MAX_LEN + 1];
    uint16      length;
} SUB_MP3_INFO_T;

//小屏显示call信息
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[CALL_INFO_MAX_LEN];
    uint16      length;
} SUB_CALL_INFO_T;

//小屏显示alarm event信息
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[MMIACC_EVENT_TEXT_MAX_LEN];
    uint16      length;
} SUB_ALARM_INFO_T;
//小屏显示倒计时 event信息
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[MMIACC_EVENT_TEXT_MAX_LEN];
    uint16      length;
} SUB_COUNTEDTIME_INFO_T;

//小屏显示fm 信息
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       name[MMIMP3_NAME_MAX_LEN+1];
    uint16      length;
} SUB_FM_INFO_T;

//小屏显示missed call信息
typedef struct
{
    //BOOLEAN     is_ucs2;
    wchar       info[MISSED_CALL_INFO_MAX_LEN];
    uint16      length;
} SUB_MISSED_CALL_INFO_T;

//小屏显示信息
typedef struct
{
    BOOLEAN                 is_update;          //是否允许刷新小屏
    BOOLEAN                 is_poweron;         //是否处于poweron状态
    uint8                   cur_batlevel;       //当前的电量
    uint8                   timer_id;           //小屏显示滚动字符串,动画的定时器(所有的公用一个定时器)
    uint8                   context_cur_index;  //小屏显示滚动字符串的索引
    uint8                   content_num;        //小屏显示日期或者其他字符串的个数
    SUB_MP3_INFO_T          mp3_name;           //小屏显示mp3名字的信息
    SUB_CALL_INFO_T         call_info;          //小屏显示call的信息
    SUB_ALARM_INFO_T        alarm_info;         //小屏显示alarm的信息
    SUB_COUNTEDTIME_INFO_T  countedtime_info;         //@juan.zhao for sub_lcd 20100301 ，小屏显示倒计时的信息
    SUB_FM_INFO_T           fm_name;         //@juan.zhao for sub_lcd 20100301 ，fm信息
    SUB_MISSED_CALL_INFO_T  missed_call;        //小屏显示missed call的信息
    SUB_CONTENT_E           sub_content[SUB_CONTENT_MAX_NUM];//小屏显示日期或者其他字符串
} SUB_LCD_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init lcd display info
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_Init(void);

/*****************************************************************************/
// 	Description : display the sub lcd idle picture
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:更换小屏墙纸，只需要更改此函数即可
/*****************************************************************************/
void MMISUB_DisplayIdlePic(void);

/*****************************************************************************/
// 	Description : 设置手机的状态，是关机还是开机状态
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:只需要关机状态charge,alarm调用即可
/*****************************************************************************/
void MMISUB_SetMsStatus(
                        BOOLEAN     is_poweron
                        );

/*****************************************************************************/
// 	Description : 小屏是否允许刷新
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_IsPermitUpdate(
                           BOOLEAN  is_update   //是否允许刷新
                           );

/*****************************************************************************/
// 	Description : update display the sub lcd icon,time,date or string
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_UpdateDisplaySubLcd(void);

/*****************************************************************************/
// 	Description : set the battery sub lcd level for display
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/ 
void MMISUB_SetBatteryLevel(void);

/*****************************************************************************/
// 	Description : start the timer of the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_StartSubLcdTimer(void);

/*****************************************************************************/
// 	Description : stop the timer of the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_StopSubLcdTimer(void);

/*****************************************************************************/
// 	Description : handle the timer msg of subidle window
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
void MMISUB_HandleTimerMsg(
                              uint8 timer_id,
                              uint32   param
                              );

/*****************************************************************************/
// 	Description : set the sub lcd display
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_SetSubLcdDisplay(
                             BOOLEAN            is_replace, //是否替换第一项(第一项只显示日期，SIM相关信息)
                             BOOLEAN            is_display, //显示还是清除
                             SUB_CONTENT_E      content,    //显示的内容
                             MMI_STRING_T       *str_ptr    //显示信息，可以为空，由来电，闹钟等填写
                             );


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif
