/******************************************************************************
 ** File Name:      dsp_compensate.c                                          *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/01/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for time compensate of the bug for DSP.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/01/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define DSP_MCU_CMD_BASE_ADDR   0x30000fa0
#else
#define DSP_MCU_CMD_BASE_ADDR   0x10000fa0
#endif
#define DSP_SLEEP_REQ          0x0
#define DSP_NORMAL_END_REQ     0x1
#define DSP_WAKEUP_REQ         0x2
#define DSP_OVERRUN_REQ        0xffff
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
typedef struct CMD_QUEUEtag
{
	uint16  CmdID;             // FOR DSP side: 0 Normal Start to Sleep
	                           //               1 Normal wakeup
	                           //               2 Wakeup by MCU
	uint16  SeqNum;
	uint16  Reserved0;
	uint16  Reserved1;
	uint16  Reserved2;
} CMD_QUEUE_T;

typedef struct SHR_MEMtag
{
	CMD_QUEUE_T  MCU_Req;
	CMD_QUEUE_T  DSP_Req;
	uint16       IsTimeValid;      // DSP clear 0 before send start to sleep and MCU will set 
	                               // it to 1 after caculated the delta_time.
	uint16       delta_fn;         // Each number means 0.1ms
	uint16       qbc;              
} DSP_MCU_CMD_T;

DSP_MCU_CMD_T * cmd_ptr = (DSP_MCU_CMD_T *) DSP_MCU_CMD_BASE_ADDR;
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
#if 0 
// For temp usage
uint32 start_time, end_time;

void DSP_FiqHandler(void)
{
	uint32 temp;
	
	*(volatile uint32 *)    GR_ICLR |= 0x2;
	
	switch (cmd_ptr->DSP_Req.CmdID)
	{
		case DSP_SLEEP_REQ:
			start_time = SYSTEM_CURRENT_CLOCK;			
		break;
		case DSP_NORMAL_END_REQ:
		break;
		case DSP_WAKEUP_REQ:
			end_time = SYSTEM_CURRENT_CLOCK;
			end_time = end_time - start_time;
			
			temp = end_time * 13 / 60;
			
			cmd_ptr->delta_fn = (uint16) temp;
			temp = (end_time * 13 * 1000) / 12 - temp * 5000;
			
			cmd_ptr->qbc = (uint16) temp; 
			cmd_ptr->IsTimeValid = 1;
		break;
		default:
		break;
	}
}
#endif

#if 1
uint32 cur_int_num = 0;
uint32 end_time;

void DSP_FiqHandler(void)
{
	uint32 temp;
	
	*(volatile uint32 *)    GR_ICLR |= 0x2;
	
	switch (cmd_ptr->DSP_Req.CmdID)
	{
		case DSP_SLEEP_REQ:
			cur_int_num = 0;
						
			// Startup timer
			*(volatile uint32 *)    TM1_LOAD =  0xffff;
			*(volatile uint32 *)    TM1_CTL =  0xC8;
		break;
		case DSP_NORMAL_END_REQ:
			*(volatile uint32 *)    TM1_CTL =  0x0;
		break;
		case DSP_WAKEUP_REQ:
			end_time = *(volatile uint32 *) TM1_VALUE;
			*(volatile uint32 *)    TM1_CTL =  0x0;
			
			end_time = (65536 - end_time) * 256 / 1200 + cur_int_num * 13981; // 100 qbC
			
			temp = end_time / 50;
			cmd_ptr->delta_fn = (uint16) temp;
			temp = (end_time - temp * 50) * 100;
			
			
			cmd_ptr->qbc = (uint16) temp; 
			cmd_ptr->IsTimeValid = 1;
		break;
		case DSP_OVERRUN_REQ:
			SCI_ASSERT(0);/*assert verified*/
		break;
		default:
		break;
	}
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //