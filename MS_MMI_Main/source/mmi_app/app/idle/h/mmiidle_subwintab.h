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
//С����ʾ������
typedef enum
{
    SUB_CONTENT_NONE,                   //û��
    SUB_CONTENT_DATE,                   //����
    SUB_CONTENT_PWD,                    //���뿪������
    SUB_CONTENT_PIN,                    //����PIN��
    SUB_CONTENT_PUK,                    //����PUK��
    SUB_CONTENT_SIM_REJECTED,           //SIM������
    SUB_CONTENT_SIM_REGISTRATION_FAILED,//SIM��ע��ʧ��
    SUB_CONTENT_PUK_BLOCKED,            //PUK����
    SUB_CONTENT_PUK2_BLOCKED,           //PUK2����
    SUB_CONTENT_NO_SIM,                 //û��SIM��
    SUB_CONTENT_LOW_BAT,                //������
    SUB_CONTENT_MP3,                    //Mp3 name
    SUB_CONTENT_ALARM,                  //Alarm enevt
    SUB_CONTENT_COUNTEDTIME,    //COUNTEDTIME enevt,    @juan.zhao for sub_lcd 20100301
    SUB_CONTENT_FM,    //fM��Ϣ,    @juan.zhao for sub_lcd 20100301
    SUB_CONTENT_CALL,                   //call event
    SUB_CONTENT_MISSED_CALL,            //missed call info
    SUB_CONTENT_CHARGE_OFF,             //�ػ����
    SUB_CONTENT_CHARGE_FINISH,          //�ػ�������
    SUB_CONTENT_CHARGE_OVER_VOLTAGE,    //�ػ�������ѹ����
    SUB_CONTENT_CAMERA_WAIT,            //����ȴ���
    SUB_CONTENT_SIM_LOCK,            //SIM Locked
    SUB_CONTENT_MAX_NUM
} SUB_CONTENT_E;

//С����ʾmp3 name��Ϣ
typedef struct
{
//    BOOLEAN     is_ucs2;
    wchar       name[MMIMP3_NAME_MAX_LEN + 1];
    uint16      length;
} SUB_MP3_INFO_T;

//С����ʾcall��Ϣ
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[CALL_INFO_MAX_LEN];
    uint16      length;
} SUB_CALL_INFO_T;

//С����ʾalarm event��Ϣ
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[MMIACC_EVENT_TEXT_MAX_LEN];
    uint16      length;
} SUB_ALARM_INFO_T;
//С����ʾ����ʱ event��Ϣ
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       info[MMIACC_EVENT_TEXT_MAX_LEN];
    uint16      length;
} SUB_COUNTEDTIME_INFO_T;

//С����ʾfm ��Ϣ
typedef struct
{
   // BOOLEAN     is_ucs2;
    wchar       name[MMIMP3_NAME_MAX_LEN+1];
    uint16      length;
} SUB_FM_INFO_T;

//С����ʾmissed call��Ϣ
typedef struct
{
    //BOOLEAN     is_ucs2;
    wchar       info[MISSED_CALL_INFO_MAX_LEN];
    uint16      length;
} SUB_MISSED_CALL_INFO_T;

//С����ʾ��Ϣ
typedef struct
{
    BOOLEAN                 is_update;          //�Ƿ�����ˢ��С��
    BOOLEAN                 is_poweron;         //�Ƿ���poweron״̬
    uint8                   cur_batlevel;       //��ǰ�ĵ���
    uint8                   timer_id;           //С����ʾ�����ַ���,�����Ķ�ʱ��(���еĹ���һ����ʱ��)
    uint8                   context_cur_index;  //С����ʾ�����ַ���������
    uint8                   content_num;        //С����ʾ���ڻ��������ַ����ĸ���
    SUB_MP3_INFO_T          mp3_name;           //С����ʾmp3���ֵ���Ϣ
    SUB_CALL_INFO_T         call_info;          //С����ʾcall����Ϣ
    SUB_ALARM_INFO_T        alarm_info;         //С����ʾalarm����Ϣ
    SUB_COUNTEDTIME_INFO_T  countedtime_info;         //@juan.zhao for sub_lcd 20100301 ��С����ʾ����ʱ����Ϣ
    SUB_FM_INFO_T           fm_name;         //@juan.zhao for sub_lcd 20100301 ��fm��Ϣ
    SUB_MISSED_CALL_INFO_T  missed_call;        //С����ʾmissed call����Ϣ
    SUB_CONTENT_E           sub_content[SUB_CONTENT_MAX_NUM];//С����ʾ���ڻ��������ַ���
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
//	Note:����С��ǽֽ��ֻ��Ҫ���Ĵ˺�������
/*****************************************************************************/
void MMISUB_DisplayIdlePic(void);

/*****************************************************************************/
// 	Description : �����ֻ���״̬���ǹػ����ǿ���״̬
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:ֻ��Ҫ�ػ�״̬charge,alarm���ü���
/*****************************************************************************/
void MMISUB_SetMsStatus(
                        BOOLEAN     is_poweron
                        );

/*****************************************************************************/
// 	Description : С���Ƿ�����ˢ��
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:������ģ�����
/*****************************************************************************/
void MMISUB_IsPermitUpdate(
                           BOOLEAN  is_update   //�Ƿ�����ˢ��
                           );

/*****************************************************************************/
// 	Description : update display the sub lcd icon,time,date or string
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:������ģ�����
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
//	Note:������ģ�����
/*****************************************************************************/
void MMISUB_SetSubLcdDisplay(
                             BOOLEAN            is_replace, //�Ƿ��滻��һ��(��һ��ֻ��ʾ���ڣ�SIM�����Ϣ)
                             BOOLEAN            is_display, //��ʾ�������
                             SUB_CONTENT_E      content,    //��ʾ������
                             MMI_STRING_T       *str_ptr    //��ʾ��Ϣ������Ϊ�գ������磬���ӵ���д
                             );


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif
