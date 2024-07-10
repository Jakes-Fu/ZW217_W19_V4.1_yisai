/*
 * FileName: FlashTask_main.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#ifndef FLASHTASK_MAIN_H
#define FLASHTASK_MAIN_H


//=================================================
// FLASH TASK Stack size
//=================================================
#define FLASHTASK_STACK_SIZE	(80*1024)


//=================================================
// FLASH TASK entry file path
//=================================================
#ifdef WIN32
#define FLASH_UI_ENTRY_PATH				"D:\\APP\\gui.swf"
#define FLASHTASK_FONT_FILE_PATH			"D:\\FONT\\font.df2"
#else
#define FLASH_UI_ENTRY_PATH				"C:\\APP\\gui.swf"
#define FLASHTASK_FONT_FILE_PATH			"C:\\FONT\\font.df2"
#endif


//=================================================
// System core frequency
//=================================================
#define FREQ_ARM200_AHB_100			0xA		//arm:1 div, ahb:2 div
#define FREQ_ARM_100_AHB_100			0x9		//arm:2 div, ahb:2 div
#define FREQ_ARM_66_AHB_66			0x8		//arm:3 div, ahb:3 div
#define FREQ_ARM_100_AHB_50			0x7		//arm:2 div, ahb:4 div
#define FREQ_ARM_50_AHB_50			0x6		//arm:4 div, ahb:4 div
#define FREQ_ARM_40_AHB_40			0x5		//arm:5 div, ahb:5 div
#define FREQ_ARM_66_AHB_33			0x4		//arm:3 div, ahb:6 div
#define FREQ_ARM_33_AHB_33			0x3		//arm:6 div, ahb:6 div
#define FREQ_ARM_50_AHB_25			0x2		//arm:4 div, ahb:8 div
#define FREQ_ARM_25_AHB_25			0x1		//arm:8 div, ahb:8 div

//=================================================
// FLASH TASK mode
//=================================================
typedef enum
{	
	FLASHTASK_MODE_IDLE=0,
	FLASHTASK_MODE_PLAY,
	FLASHTASK_MODE_PAUSE,
	FLASHTASK_MODE_ERROR,
}FLASHTASK_MODE;




/****************************************************************
 * Function:       FlashTask_StartLogo
 * Description:   Play the starting SWF animation logo
 * Params:        None
 * Return:         None
 * Others:         After finished playing the starting logo, it will start the UI
 ****************************************************************/
void FlashTask_StartLogo(void);


/****************************************************************
 * Function:       FlashTask_EndLogo
 * Description:   Play the ending SWF animation logo
 * Params:        None
 * Return:         None
 * Others:         After finished playing the edning logo, it will shutdown the device
 ****************************************************************/
void FlashTask_EndLogo(void);


/****************************************************************
 * Function:       FlashTask_StartUI
 * Description:   Start the FLASH UI
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_StartUI(void);


/****************************************************************
 * Function:       FlashTask_ReloadUI
 * Description:   Restart the UI
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_ReloadUI(void);


/****************************************************************
 * Function:       FlashTask_SetMode
 * Description:   Set the FLASH TASK running mode
 * Params:        
 			@mode: 
 				FLASHTASK_MODE_NONE: no FLASH running
 				FLASHTASK_MODE_LOGO: running the logo animation
 				FLASHTASK_MODE_UI: running the FLASH UI
 				FLASHTASK_MODE_PLAYER: running as a FLASH player
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_SetMode(FLASHTASK_MODE mode);


/****************************************************************
 * Function:       FlashTask_GetMode
 * Description:   Get the running mode of the FLASH TASK
 * Params:        None
 * Return:         
			FLASHTASK_MODE_NONE: no FLASH running
			FLASHTASK_MODE_LOGO: running the logo animation
			FLASHTASK_MODE_UI: running the FLASH UI
			FLASHTASK_MODE_PLAYER: running as a FLASH player
 * Others:         
 ****************************************************************/
FLASHTASK_MODE FlashTask_GetMode(void);


/****************************************************************
 * Function:       FlashTask_IsFlashRunning
 * Description:   Get the running status of FLASH
 * Params:        None
 * Return:
 			1: The FLASH is running
 			0: The FLASH is not running
 * Others:         
 ****************************************************************/
unsigned int FlashTask_IsFlashRunning(void);


/****************************************************************
 * Function:       FlashTask_IsRunningUI
 * Description:   Get if the FLASH TASK is running in the UI mode
 * Params:        None
 * Return:
 			1: It's running in UI mode
 			0: It's NOT running in UI mode
 * Others:         
 ****************************************************************/
unsigned int FlashTask_IsRunningUI(void);


/****************************************************************
 * Function:       FlashTask_ChangeFreq
 * Description:   Change the core frequency
 * Params:
 			@freq: The frequency to be changed to, it can be FREQ_ARM200_AHB_100...
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_ChangeFreq(unsigned int freq);


/****************************************************************
 * Function:       FlashTask_RestoreFreq
 * Description:   Restore the core frequency
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_RestoreFreq(void);


/****************************************************************
 * Function:       FlashTask_SuspendUI
 * Description:   Suspend the FLASH UI
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_SuspendUI(void);



/****************************************************************
 * Function:       FlashTask_ResumeUI
 * Description:   Resume the FLASH UI
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_ResumeUI(void);



/****************************************************************
 * Function:       FlashTask_SuspendThread
 * Description:   Resume the FLASH Thread
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_SuspendThread(void);



/****************************************************************
 * Function:       FlashTask_ResumeThread
 * Description:   Resume the FLASH Thread
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
void FlashTask_ResumeThread(void);



/****************************************************************
 * Function:       FlashTask_ChangePriority
 * Description:   Change the running priority of FLASH Thread
 * Params:        
 	@priority:	the new priority to set to
 * Return:         None
 * Others:         
 ****************************************************************/
void
FlashTask_ChangePriority(unsigned int priority);



/****************************************************************
 * Function:       FlashTask_GetPriority
 * Description:   Get the current running priority of FLASH Thread
 * Params:        None
 * Return:         The current priority of FLASH Thread
 * Others:         
 ****************************************************************/
unsigned int
FlashTask_GetPriority(void);



/****************************************************************
 * Function:       FlashTask_GetTick
 * Description:   Get the tick count of Flash Task, the interval between the ticks is 500MS
 * Params:        None
 * Return:         None
 * Others:         
 ****************************************************************/
unsigned long FlashTask_GetTick(void);


void FlashTask_Play(const char *file_path);
 
#endif // FLASHTASK_MAIN_H

