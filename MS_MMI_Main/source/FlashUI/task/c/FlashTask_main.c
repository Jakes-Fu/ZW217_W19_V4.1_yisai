/*
 * FileName: FlashTask_main.c
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

/* =======================================================================
 * Include headers
 * ======================================================================= */ 
#include "os_apiext.h"
#include "threadx_os.h"
#include "tasks_id.h"
#include "dal_power.h"
#include "chng_freq.h"
#include "chip.h"
#include "mmi_position.h"
#include "mmiidle_export.h"
#include "FlashTask_msg.h"
#include "FlashTask_main.h"
#include "amuse_file_device.h"



/* =======================================================================
 *  Macro defines
 * ======================================================================= */
#define FLASH_SCREEN_WIDTH	MMI_MAINSCREEN_WIDTH
#define FLASH_SCREEN_HEIGHT	MMI_MAINSCREEN_HEIGHT
#define FLASH_ENABLE_DEBUG	0

#if FLASH_ENABLE_DEBUG
#include <stdarg.h>
#endif
/* =======================================================================
 *  External variables
 * ======================================================================= */


/* =======================================================================
 *  Global variables
 * ======================================================================= */
static uint32 				sFlash_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL; 
static FLASHTASK_MODE		sFlashTask_Mode = FLASHTASK_MODE_IDLE;
static FP_Param_t			sFlashTask_param;
static void *				sFlashTask_Thread = NULL;
static uint32				sFlashTask_CurrentPriority = 31;

/* =======================================================================
 *  Functions implement
 * ======================================================================= */ 
void
FlashTask_StartLogo(void)
{
}

void
FlashTask_EndLogo(void)
{
}

void
FlashTask_StartUI(void)
{
	if (sFlashTask_Mode != FLASHTASK_MODE_PLAY && sFlashTask_Mode != FLASHTASK_MODE_PAUSE)
	{
		if(STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
		{
			//sFlashTask_param.fname = FLASH_UI_ALARM_ENTRY_PATH;
		}
		else
		{
			strcpy(sFlashTask_param.fname, FLASH_UI_ENTRY_PATH);
		}
		sFlashTask_param.width = FLASH_SCREEN_WIDTH;
		sFlashTask_param.height = FLASH_SCREEN_HEIGHT;
		FlashTask_SendSignal(FLASH_MSG_PLAYER_OPEN, (uint32)&sFlashTask_param);
	}
	else
	{
		FlashTask_ResumeUI();
	}
}

/*
  *	Async
  */
//向发Flash Task 发送载入消息
void FlashTask_Play(const char *file_path)
{
	TX_THREAD *pth= tx_thread_identify();
	int nRet = 0;
	
	if (sFlashTask_Mode != FLASHTASK_MODE_PLAY && sFlashTask_Mode != FLASHTASK_MODE_PAUSE)
	{
		//SCI_ASSERT(file_path);
		//SCI_ASSERT(strlen(file_path) <= FILE_PATH_LENGTH);
        if(NULL == file_path||strlen(file_path) > FILE_PATH_LENGTH)
		{
		  return;//这里直接选择退出应该没啥问题 
		}
		strcpy(sFlashTask_param.fname, file_path);		
		sFlashTask_param.width = FLASH_SCREEN_WIDTH;
		sFlashTask_param.height = FLASH_SCREEN_HEIGHT;
		nRet = FlashTask_SendSignal(FLASH_MSG_PLAYER_OPEN, (uint32)&sFlashTask_param);		
	}
	else
	{
		FlashTask_ResumeUI();
	}

	if(pth && (strcmp(pth->tx_thread_name, "T_P_FLASH")!=0))
	{
		while(FLASHTASK_MODE_IDLE == sFlashTask_Mode)
			SCI_Sleep(5);
			
	}
}

void FlashTask_Stop()
{
	TX_THREAD *pth= tx_thread_identify();
	
	FlashTask_SendSignal(FLASH_MSG_PLAYER_EXIT, NULL);	
	if(pth && (strcmp(pth->tx_thread_name, "T_P_FLASH")!=0))
	{
		while(FLASHTASK_MODE_IDLE != sFlashTask_Mode)
		{	
			SCI_Sleep(5);
		}

	}
}

void
FlashTask_ReloadUI(void)
{
	FlashTask_SendSignal(FLASH_MSG_PLAYER_RELOAD, 0);
}

void
FlashTask_SetMode(FLASHTASK_MODE mode)
{
	sFlashTask_Mode = mode;
}

FLASHTASK_MODE
FlashTask_GetMode(void)
{
	return sFlashTask_Mode;
}

unsigned int 
FlashTask_IsFlashRunning(void)
{
	return 1;//FlashTask_is_canvas_enable();
}

unsigned int
FlashTask_IsRunningUI(void)
{
	return (sFlashTask_Mode == FLASHTASK_MODE_PLAY || sFlashTask_Mode == FLASHTASK_MODE_PAUSE);
}

void
FlashTask_ChangeFreq(unsigned int freq)
{
	//CHIP_SetArmClk(sFlash_freq_handler,freq);
}

void
FlashTask_RestoreFreq(void)
{
	if (sFlash_freq_handler != CHNG_FREQ_REQ_HANDLER_NULL)
	{
		//CHIP_RestoreARMClk(sFlash_freq_handler);
	}
}

void
FlashTask_SuspendUI(void)
{
	if (sFlashTask_Mode == FLASHTASK_MODE_PLAY)
	{
		FlashTask_SendSignal(FLASH_MSG_PLAYER_PAUSE,0);
		amuse_printf("Suspend Flash UI! \n");
	}
}

void
FlashTask_ResumeUI(void)
{
	if (sFlashTask_Mode == FLASHTASK_MODE_PAUSE)
	{
		FlashTask_SendSignal(FLASH_MSG_PLAYER_RESUME,0);		
		amuse_printf("Resume Flash UI! \n");
	}
}


void
FlashTask_ChangePriority(unsigned int priority)
{
	UINT oldpriority, ret;
	TX_THREAD *th = (TX_THREAD*)sFlashTask_Thread;

	if (sFlashTask_CurrentPriority == priority)
	{
		return;
	}
	//SCI_ASSERT(th);
	if(NULL == th)
	{
	  return;//线程为空时，相当于啥也没做
	}
	amuse_printf("Going to change priority, current = %d", th->tx_priority);
	ret = tx_thread_priority_change(th, priority, &oldpriority);
	//SCI_PASSERT(ret == SCI_SUCCESS, ("Setting Flash Thread priority failed!"));
	if(ret == SCI_SUCCESS)
	{
		sFlashTask_CurrentPriority = priority;
	}
}

unsigned int
FlashTask_GetPriority(void)
{
	return sFlashTask_CurrentPriority;
}


//================================================================
// The Entry of this TASK
//================================================================
//callback of flash task
#ifdef WIN32
void FLASH_Task(uint32 argc, void * argv)
#else
THREAD_ENTRY(P_FLASH)
#endif
{
	// Initialize the task running mode
	sFlashTask_Mode = FLASHTASK_MODE_IDLE;
	sFlashTask_Thread = (void*)tx_thread_identify();

	// Initialize the message handlers
	FlashTask_MsgHandle_Init();

		
	// Main message loop of this task
	FlashTask_MainLoop();	
}

