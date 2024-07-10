#ifndef __MUX_SLEEP__
#define __MUX_SLEEP__

#include "sci_types.h"

/**---------------------------------------------------------------------------* 
 **                          Compiler Flag                                    * 
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert start of extern C construct */                    
extern "C" {                                                                    
#endif                                                                           

#define  UART_TX_IDLE     0x00
#define  UART_TX_RUNING   0x01  
#define  UART_RX_IDLE     0x02
#define  UART_RX_RUNING   0x03  


void Mux_SetUartState(uint32 state);
void Mux_GetAPWakeUpStatus(uint32  status);

BOOLEAN  Mux_IsAPSleep(void);
void Mux_WakeUpAP(void);

void Mux_SleepInit(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert end of extern C construct 	*/                  
}                   /* The C header file can now be 		*/                  
#endif              /* included in either C or C++ code. 	*/                  

#endif /*__MUX_SLEEP__*/
