/******************************************************************************
 ** File Name:      os_import.h                                                 *
 ** Author:         hanjun.liu                                              *
 ** DATE:           01/19/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/19/2004     hanjun.liu     Create.                                   *
 ******************************************************************************/
#ifndef _OS_IMPORT_H
#define _OS_IMPORT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
 /*****************************************************************************/
// Description :    This function is used to set the next tick use in RTOS.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC void OS_SetNextTick(
			uint32 mil_sec,         // Time value to be set.
			void (* func_ptr)() // When timer is expired, call this function.
							  );

/*****************************************************************************/
// Description :    This function is used to get escaped ticks form power on.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_GetTickCount(void);  // return the number of tick count.

 /*****************************************************************************/
// Description :    This function is used to get escaped seconds form power on.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_GetCurrentTime(void);

 /*****************************************************************************/
// Description :    This function is used to get escaped miliseconds form power on.(user defined beginning time)
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_GetCurrentMilsec(void);

void _tx_timer_interrupt(void);

#ifndef _ROM
	/* Hardware driver of TIMER */
	#define TX_SET_NEXT_TICK(tick)	\
			OS_SetNextTick(tick,&_tx_timer_interrupt);
			
	#define TX_GET_TICK_COUNT()	\
			OS_GetTickCount()
#else
	#include "rom_timer_drv.h"
	#define TX_SET_NEXT_TICK(tick)	TIMER_SetNextTick(tick);			
	#define TX_GET_TICK_COUNT()		TIMER_GetTickCount()
#endif	// _ROM

/*****************************************************************************/
// Description :    This function is used to read OS parameter.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

PUBLIC BOOLEAN OS_ReadOsParameter(
								uint16      length,
								uint8       *buf_ptr
								);

 /*****************************************************************************/
// Description :    This function is used to write OS parameter.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

PUBLIC void OS_WriteOsParameter(
								uint16      length,
								uint8       *buf_ptr
								)
;

/*****************************************************************************/
// Description :    This function is used to reset MCU which used in OS.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC void OS_ResetMCU(void);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     int SIO_DumpGetChar()                                                 *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function scanf a char from uart receive register at debug.       *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Returns a char from uart receive register.                            *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC int OS_DebugGetChar(void);

PUBLIC void OS_SendTestPointRequest(uint32 param1, uint32 param2);

PUBLIC void OS_SendRemainLogMessage(void);

PUBLIC void OS_DumpFlush(void);
/*****************************************************************************/
// Description :    This function is used to reset in RTOS.
// Global resource dependence : 
// Author :         hanjun.liu
// Note :
/*****************************************************************************/
PUBLIC void  OS_Reset(void);

/*****************************************************************************/
//  Description:    The SCI_DumpPrint function is used to print string by 
//                  serial port.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC  void OS_DumpPrint(
    char * string,      // String to be printed out
    uint32 size         // Size of string to be printed out
    );

/*****************************************************************************/
//  Description:    The DumpMemory function is used to dump out len bytes
//                  memory from begin address.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void OS_DumpMemory(void);

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

PUBLIC void OS_SendPacket (uint32 sendpacketmode);


/*****************************************************************************/
//  Description:    This function is used to get SRAM end address.
//                  It equals SRAM_BASE_ADDR + SRAM_SIZE.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 OS_GetSramEndAddr(void);

/*****************************************************************************/
//  Description:    This function is used to get Flash end address.
//                  It equals FLASH_BASE_ADDR + FLASH_SIZE.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 OS_GetFlashEndAddr(void);

/*****************************************************************************/
//  Description:    Dump size_in_byte bytes definite memory out which begin from
//                  start_addr address.
//	Global resource dependence:
//  Author:         Daniel.Luo	2006/09/11
//	Note:
/*****************************************************************************/
PUBLIC void OS_DumpDefiniteMemory(uint32 start_addr, uint32 size_in_byte);

/**----------------------------- ----------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*End _OS_IMPORT_H*/

