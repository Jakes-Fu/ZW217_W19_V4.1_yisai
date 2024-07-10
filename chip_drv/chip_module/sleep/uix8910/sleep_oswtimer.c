/******************************************************************************
 ** File Name:      sleep_oswtimer.c                                       *
 ** Author:         bo.tang                                                   *
 ** DATE:           11/29/2018                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for time compensate of the bug for DSP.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/29/2018     bo.tang     		Create.                                   *

******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "../sleep_phy.h"
#include "task_monitor.h"
#include "pin_sleep.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "fiq_drvapi.h"
#include "dal_audio.h"
#include "ldo_drvapi.h"
#include "cp_pwrctrl_asm_arm.h"
#include "cp_sysreg_asm_arm.h"

static int g_si = 0;
static uint64 g_sOswStop_ref = 0;

/*******************************************************************************
FUNCTION                                                              
  hal_Osw2TickEnable                                             
DESCRIPTION                                                           
  @return if the OS timer is enable.
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
uint32 hal_Osw2TickEnable(void)
{
    return (hwp_idle->IDL_OSW2_EN & (1<<31));
}



/*******************************************************************************
FUNCTION                                                              
  hal_Osw2TickStart                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
void hal_Osw2TickStart(uint32 loadVal)
{
	uint64 temp=0;
	uint32 len=0;
	do
	{
		temp = hwp_idle->IDL_32K_REF;
		len = (temp + 0xFFFFFFFF - g_sOswStop_ref) % 0xFFFFFFFF;
	}while (len < 2);
	g_si++;
//	hwp_idle->IDL_OSW2_EN &= ~(0x1<<31);
	hwp_idle->IDL_OSW2_EN = (loadVal | (1<<31));
//	hwp_idle->IDL_OSW2_EN = loadVal;
//	hwp_idle->IDL_OSW2_EN |= (0x1<<31);
}


/*******************************************************************************
FUNCTION                                                              
  hal_Osw2TickStop                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
void hal_Osw2TickStop(void)
{
	hwp_idle->IDL_OSW2_EN &= (~(0x1<<31));
	g_sOswStop_ref = hwp_idle->IDL_32K_REF;
}


/*******************************************************************************
FUNCTION                                                              
  hal_Osw2TickGetVal                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
uint32 hal_Osw2TickGetVal(void)
{
    return hwp_idle->IDL_OSW2_CONT;
}

/*******************************************************************************
FUNCTION                                                              
  hal_Osw2IrqHandle                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
void hal_Osw2IrqHandle(void)
{
	hwp_idle->AP_INT_STA |= CP_IDLE_OSW2_IRQ;
	hwp_idle->IDL_AWK_ST |= CP_IDLE_AWK_OSW2_STAT;
	g_si--;
	if (g_si)
	{
		
		SCI_TraceLow("hal_Osw2IrqHandle LPS OSW2 AwkSt:0x%x,IDL_32K_REF=%d", 
					 hwp_idle->IDL_AWK_ST,  hwp_idle->IDL_32K_REF);
		g_si = 0;
	}
}


/*******************************************************************************
FUNCTION                                                              
  hal_Osw2IrqEnable                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
void hal_Osw2IrqEnable(void)
{
	hwp_idle->AP_INTEN_SET = 0x1;
}
