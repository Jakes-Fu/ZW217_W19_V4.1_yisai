/*****************************************************************************
** File Name:      mmiacc_id.h                                               *
** Author:         xiaoqing.lu                                               *
** Date:           2009/04/03                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/04/03     xiaoqing.lu       Create                                   *
******************************************************************************/

#ifndef _MMIHELLO_ID_H_
#define _MMIHELLO_ID_H_ 

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

// ����ID
typedef enum
{
    ALARMCLOCK_WIN_ID_START = (MMI_MODULE_ALARMCLOCK << 16),

	MMI_ALARMCLOCK_MAIN_WIN_ID,//����������

	MMI_ALARMCLOCK_ALARM_MAX_WIN_ID,//����������

	MMI_ALARMCLOCK_SET_ALARM_WIN_ID,//��������״̬

	MMI_ALARMCLOCK_SET_TIME_WIN_ID,//����ʱ�����

	MMI_ALARMCLOCK_SET_DATE_WIN_ID,//�������ڽ���

	MMI_ALARMCLOCK_TIME_REMAIN_ID,//ʣ��ʱ�����

	MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID,//�����������

    ALARMCLOCK_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

#undef WIN_ID_DEF

// �ؼ�ID
typedef enum
{
    PEDOMETER_CTRL_ID_ID_START = ALARMCLOCK_MAX_WIN_ID,

	//ctrl id
	MMI_ALARMCLOCK_SETLIST_CTRL_ID,//�����б�

	MMI_ALARMCLOCK_ADD_CLOCK_CTRL_ID,//������Ӱ�ť

	MMI_ALARMCLOCK_SAVE_BUTTON_CTRL_ID,//���水ť

	MMI_ALARMCLOCK_NEXT_PAGE_CTRL_ID,//��һҳ��ť

	MMI_ALARMCLOCK_SELECT_LIST_CTR_ID,//���á���/�رա�ɾ��

	MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID,//����ʱ��

	MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID,//��������

	MMI_ALARMCLOCK_EXTEND_CTRL_ID,//�ӳ���ť

	MMI_ALARMCLOCK_CLOSE_CTRL_ID,//�رհ�ť

	MMI_ALARMCLOCK_CLOSE2_CTRL_ID,//�رհ�ť

    ALARMCLOCK_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIHELLO_ID_H_
