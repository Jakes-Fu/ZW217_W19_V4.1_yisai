#ifdef WRE_SUPPORT
#ifndef _MMIWRE_TIMER_H_
#define _MMIWRE_TIMER_H_

typedef void    (*ADAPT_TIMER_FUN)(uint32);//timer callback function

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
    );


uint32 Adapt_DeactiveTimer(    // If successful, returns SCI_SUCCESS,
                                    // else return SCI_ERROR
    void * timer_ptr         // Pointer to a previously created
                                    // application timer.
);

uint32 Adapt_DeleteTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    void* timer_ptr     // Pointer to a previously created application
                                // timer.
    );


int adapt_arm_disable_irq(void);

void adapt_arm_enable_irq(int ov);

uint32 Adapt_GetLocalTimeZone(void);

BOOLEAN Adapt_SetLocalTimeZone(const uint8 time_zone, BOOLEAN is_summer);
#endif
#endif


