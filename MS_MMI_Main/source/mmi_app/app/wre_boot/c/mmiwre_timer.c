#ifdef WRE_SUPPORT
#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmk_app.h"
//#include "app_tcp_if.h"
//#include "in_Message.h"
//#include "tasks_id.h"
//#include "tx_port.h"
#include "mmiwre_timer.h"
#include "mmiwclk_export.h"

#define ADAPT_TX_INT_DISABLE           0x80        /* Disable IRQ interrupts   */
#define ADAPT_TX_INT_ENABLE            0x00        /* Enable IRQ interrupts    */

/*****************************************************************************/
//  Description : Adapt_CreatePeriodTimer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
void * Adapt_CreatePeriodTimer( // If successful, returns pointer to
                                            // the control block of the timer,
                                            // else return SCI_NULL.
    const char        *timer_name,          // Name string of the timer
    ADAPT_TIMER_FUN   timer_fun,            // Timer callback function
    uint32            input,                // Input value for timer callback function 
    uint32            timer_expire,         // Specifies the timer expire value in
                                            // milliseconds. 
    uint32            auto_activate,         // Option to check if auto active the timer
                                            // after create. 
                                             // SCI_NO_ACTIVATE      Don't auto active
                                             // SCI_AUTO_ACTIVATE    Auto active
    uint32              period          
    )
{
	if(period == TRUE)
	{
		return (void*)SCI_CreatePeriodTimer(timer_name,timer_fun,input,timer_expire,auto_activate);
	}
	else
	{
		return (void*)SCI_CreateTimer(timer_name,timer_fun,input,timer_expire,auto_activate);
	}
}


/*****************************************************************************/
//  Description : Adapt_DeactiveTimer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_DeactiveTimer(    // If successful, returns SCI_SUCCESS,
                                    // else return SCI_ERROR
    void * timer_ptr         // Pointer to a previously created
                                    // application timer.
    )
{
	return SCI_DeactiveTimer((SCI_TIMER_PTR)timer_ptr);
}


/*****************************************************************************/
//  Description : Adapt_DeleteTimer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_DeleteTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    void* timer_ptr     // Pointer to a previously created application
                                // timer.
    )
{
	return SCI_DeleteTimer((SCI_TIMER_PTR)timer_ptr);
}


/*****************************************************************************/
//  Description : Adapt_ActiveTimer
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_ActiveTimer(  // If active successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    void * timer_ptr     // Pointer to a previously created application
                                // timer.
    )
{
	return SCI_ActiveTimer((SCI_TIMER_PTR)timer_ptr);
}
    
#ifndef WIN32
unsigned int  _tx_thread_interrupt_control(unsigned int);
#endif

/*****************************************************************************/
//  Description : adapt_arm_disable_irq
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
int adapt_arm_disable_irq(void)
{
	//增加中断禁止处理
#ifndef WIN32
	_tx_thread_interrupt_control(ADAPT_TX_INT_DISABLE);/*lint !e746*/
#endif
	return 1;
}
/*****************************************************************************/
//  Description : adapt_arm_enable_irq
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
void adapt_arm_enable_irq(int ov)
{
#ifndef WIN32
     //恢复中断处理
     _tx_thread_interrupt_control(ADAPT_TX_INT_ENABLE);
#endif     
}  

/************************************************************************/
//  Description : adapt_arm_enable_irq
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/************************************************************************/
uint32 Adapt_GetLocalTimeZone(void)
{
   	float min = (float)60.0;
    uint32 uMinRes = (uint32)(MMIAPIACC_GetLocalTimeZone()*min);
    return uMinRes;
}

BOOLEAN Adapt_SetLocalTimeZone(const uint8 time_zone, BOOLEAN is_summer)
{
#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMIAPIWCLK_SetTimeZone(0,time_zone, 0,(uint8)is_summer);
    return TRUE;
#else
    return FALSE;
#endif
}
#endif
