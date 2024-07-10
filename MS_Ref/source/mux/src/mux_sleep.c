#include "sci_types.h"
#include "gpio_prod_api.h"
#include "deep_sleep.h"
#include "mux_sleep.h"
#include "os_api.h"

/*lint -save -e766 */	

#ifdef MODEM_WAKEUP_SUPPORT

typedef struct MUX_SLEEP_Tag
{
   volatile uint32 uart_tx_state;
   volatile uint32 uart_rx_state;   
   volatile uint32 ap_wakeup_bb;
   volatile uint32 bb_wakeup_ap;
}MUX_SLEEP_T;

LOCAL MUX_SLEEP_T  s_mux_sleep;


#define  UART_IDLE_STATUS   0x00
#define  UART_RUNNING_STATUS   0x01

LOCAL void __SwitchMuxState(void)
{

   SCI_DisableIRQ();
  // if(s_mux_sleep.uart_tx_state || s_mux_sleep.uart_rx_state || s_mux_sleep.ap_wakeup_bb) 
   if(s_mux_sleep.uart_tx_state || s_mux_sleep.uart_rx_state || !GPIO_GetAPSleepStatus()) 
   {
        TM_SendTestPointRequest(0xaaa,  s_mux_sleep.ap_wakeup_bb);
        //SCI_TRACE_LOW("__SwitchMuxState:Normal running  0x%X, 0x%X,0x%X\r\n", s_mux_sleep.uart_tx_state, s_mux_sleep.uart_rx_state, s_mux_sleep.ap_wakeup_bb);
        //禁止系统进入DeepSleep状态
        SCI_ATC_EnableDeepSleep(DISABLE_DEEP_SLEEP);
   }
   else
   {
       TM_SendTestPointRequest(0xbbb,  s_mux_sleep.ap_wakeup_bb);
    	SCI_TRACE_LOW("__SwitchMuxState:Enable System sleep \r\n");
	//允许系统进入DeepSleep状态
        SCI_ATC_EnableDeepSleep(ENABLE_DEEP_SLEEP);
   }
   SCI_RestoreIRQ();
}

void Mux_SetUartState(uint32 state)
{
   switch(state) 
   {
   case UART_TX_IDLE:
   {
        s_mux_sleep.uart_tx_state  =  UART_IDLE_STATUS;
        break; 
   }
   case UART_TX_RUNING:
   {
         s_mux_sleep.uart_tx_state  =  UART_RUNNING_STATUS;
         break; 
   }
   case UART_RX_IDLE:
   {
        s_mux_sleep.uart_rx_state  =  UART_IDLE_STATUS;
        break; 
   }
   case UART_RX_RUNING:
   {
        s_mux_sleep.uart_rx_state  =  UART_RUNNING_STATUS;
        break; 
   }    
   default:
   {
        SCI_ASSERT(0);/*assert verified*/
        break; 
   }   
   }
   __SwitchMuxState();   
}

void Mux_SleepInit(void)
{
	   s_mux_sleep.uart_tx_state = 0;
	   s_mux_sleep.uart_rx_state = 0;
	   s_mux_sleep.ap_wakeup_bb = 0;
	   s_mux_sleep.bb_wakeup_ap = 0;  
}


void Mux_GetAPWakeUpStatus(uint32  status)
{
     s_mux_sleep.ap_wakeup_bb = status;
     __SwitchMuxState();
}



BOOLEAN  Mux_IsAPSleep(void)
{
	return (GPIO_GetAPSleepStatus());
}



void Mux_WakeUpAP(void)
{
   SCI_TRACE_LOW("[MUX]:Mux_WakeUpAP: state=0x%X\r\n", s_mux_sleep.bb_wakeup_ap); 
   if(s_mux_sleep.bb_wakeup_ap) 
   {
      GPIO_WakeUpAP(SCI_TRUE);        
   }
   else
   {
      GPIO_WakeUpAP(SCI_FALSE);
   }
   s_mux_sleep.bb_wakeup_ap = !s_mux_sleep.bb_wakeup_ap;
}
#endif //MODEM_WAKEUP_SUPPORT

