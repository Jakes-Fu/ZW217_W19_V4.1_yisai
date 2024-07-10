/***************************************************************************************
** File Name:      mpeg4_api.h                                         		 			   *
** DATE:           21/02/2006                                                		   *
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         		   *
** Description:    																       *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 21/02/2006     	       Create.													   *
****************************************************************************************/
#ifndef _MPEG4_H
#define _MPEG4_H

/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "tasks_id.h"

/**------------------------------------------------------------------------------------*
 **							Compiler Flag											   *
 **------------------------------------------------------------------------------------*/
#ifdef __cpluslus
	extern "C"
	{
#endif


#define P_MPEG4_STACK_SIZE  	    (1024 * 3)
#define P_MPEG4_DISK_QUEUE_NUM      30
#define P_MPEG4_DETECT_TIMER        30


typedef struct MPEG4SVR_SIG_tag
{
	SIGNAL_VARS
} MPEG4SVR_SIG_T;

typedef enum mpeg4_task_command_tag
{
	MPEG4_TASK_TIMER_SIG = 1,
	MPEG4_TASK_RECORDER_START_SIG,
	MPEG4_TASK_RECORDER_STOP_SIG,
	MPEG4_TASK_RECORDER_PAUSE_SIG,
	MPEG4_TASK_RECORDER_RESUME_SIG,
	MPEG4_TASK_RECORDER_CANCEL_SIG,
	MPEG4_TASK_RECORDER_CLOSE_SIG,
	MPEG4_TASK_PLAYER_PLAY_SIG,
	MPEG4_TASK_PLAYER_STOP_SIG,
	MPEG4_TASK_PLAYER_PAUSE_SIG,
	MPEG4_TASK_PLAYER_RESUME_SIG,
	MPEG4_TASK_PLAYER_OPEN_SIG,
	MPEG4_TASK_PLAYER_CLOSE_SIG,
	MPEG4_TASK_PLAYER_SET_PARAM_SIG,
	MPEG4_TASK_PLAYER_SETTIME_SIG,
	MPEG4_TASK_PLAYER_GETTIME_SIG,
	MPEG4_TASK_PLAYER_REFRESH_SCREEN_SIG,
	MPEG4_TASK_PLAYER_GETCURRENTFRAMEJPEG_SIG,
	MPEG4_TASK_RECORDER_TEST_SIG,
}MPEG4_TASK_COMMAND_E;

typedef enum mpeg4_read_task_command_tag
{
	MPEG4_READ_TASK_READ_AUDIO = 1,
	MPEG4_READ_TASK_READ_VIDEO,
	MPEG4_READ_TASK_READ_VIDEO_ONE_FRAME,
	
}MPEG4_READ_TASK_COMMAND_E;


/*****************************************************************************/
//  Description:   Register MPEG4 Triggle function 
//	Global resource dependence: 
//  Author:         Jianping.wang
//	Note:           
/*****************************************************************************/
PUBLIC void MPEG4_RegFunc(int32 (*func)());
/*****************************************************************************/
//  Description: Free Register Function 
//	Global resource dependence: 
//  Author:         Jianping.wang
//	Note:           
/*****************************************************************************/
PUBLIC void MPEG4_UnRegFunc(void);
/*****************************************************************************/
// Description:	Create P_MPEG4 Task
// Global resource dependence:
// Author: Jianping.wang
// Note:
/*****************************************************************************/
PUBLIC void MPEG4_Init(void);
/*****************************************************************************/
//  Description:   Start Timer for P_MPEG4
//	Global resource dependence: 
//  Author:         Jianping.wang
//	Note:           
/*****************************************************************************/
PUBLIC void MPEG4_StartTimer(uint32 period);
/*****************************************************************************/
//  Description:   Close Timer for P_MPEG4
//	Global resource dependence: 
//  Author:         Jianping.wang
//	Note:           
/*****************************************************************************/
PUBLIC void MPEG4_CloseTimer(void);
/*****************************************************************************/
//  Description:   
//	Global resource dependence: 
//  Author:         Jianping.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN MPEG4_SendCommand (MPEG4_TASK_COMMAND_E sig_code);

//for later use
PUBLIC BOOLEAN MPEG4_Read_SendCommand (MPEG4_READ_TASK_COMMAND_E sig_code) ;

/**------------------------------------------------------------------------------------*
 **								Compiler Flag										   *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif


#endif//_MPEG4_H

//end of mpeg4.h

