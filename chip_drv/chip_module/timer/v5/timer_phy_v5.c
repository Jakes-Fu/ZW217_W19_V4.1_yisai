/******************************************************************************
 ** File Name:      timer_phy_v5.c                                            *
 ** Author:         Ji.Ding                                                   *
 ** DATE:           08/09/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 timer count .                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME         DESCRIPTION                                   *
 ** 08/09/2010     DingJi       Create.                                       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "com_drvapi.h"
#include "tb_hal.h"
#include "chip_plf_export.h"
#include "../timer_phy.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#define TIMER_UPDATE_TIME_OUT 0xffff
#define TIMER2_5_CLR_IRQ_MAGIC_NUM	0x5170

uint32 timer_base_addr[] = {TIMER0_BASE, TIMER1_BASE, TIMER2_BASE, TIMER3_BASE, TIMER4_BASE, TIMER5_BASE};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get TIMER register value with double read.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 _GetTimerValue (
    uint32 timer_id       // Indicate which timer to be get.
)
{
    uint32  time_out = 0;
    volatile uint32 tmp_count1;
    volatile uint32 tmp_count2;
    timer_s *timer_ctl = TB_NULL;

    SCI_ASSERT ( (TIMER_0 == timer_id) || (TIMER_1  == timer_id));/*assert verified*/

    // Set local variable.
    timer_ctl = (timer_s *) timer_base_addr[timer_id]; 

    tmp_count1 = timer_ctl->value;
    tmp_count2 = timer_ctl->value;

    while(tmp_count1 != tmp_count2)
    {
        time_out++;
        if(time_out > TIMER_UPDATE_TIME_OUT)
        {
            SCI_PASSERT (0, ("Timer read value time out!"));/*assert verified*/
        }
        tmp_count1 = tmp_count2;
        tmp_count2 = timer_ctl->value;
    }

    return tmp_count1;
}

//******************************************************************************
//  Description:  to set the timer register according to the configuration
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         config: the configuration from user
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_config (uint32 timer_id, TIMER_PHY_CONFIG_T *config)
{
    timer_s *timer_ctl;

    static uint32 s_old_time0_val=0;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (config == NULL)
    {
        return -2;
    }

    timer_ctl = (timer_s *) timer_base_addr[timer_id]; 

    // wait busy flag
    if ((timer_id != TIMER_2) && (timer_id != TIMER_5))
    {
    	  s_old_time0_val = TIMER_GetSystemCounterReg();
        for (;;)
        {

            if (! (timer_ctl->clr & BIT_4))       //  pulling busy flag
            {
                break;
            }

            if ( (TIMER_GetSystemCounterReg() - s_old_time0_val) > 2)   // 2ms timeout
            {
                SCI_ASSERT (0);/*assert to do*/
            }

        }
    }

    CLR_BIT (timer_ctl->ctl, 7);    //stop timer

    if (config->mode == TIMER_MODE_FREE)
    {
        CLR_BIT (timer_ctl->ctl, 6);    //set mode one-shot
    }
    else
    {
        SET_BIT (timer_ctl->ctl, 6);    //set mode period
    }

    if (config->irq_op)
    {
        SET_BIT (timer_ctl->clr, 0);    //enable timer irq
    }
    else
    {
        CLR_BIT (timer_ctl->clr, 0);    //disable timer irq
    }

    timer_ctl->load = config->load_count;

    return 0;
}

//******************************************************************************
//  Description:  initialize the related timer register and environment
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_init (uint32 timer_id)
{
    TIMER_PHY_CONFIG_T config;
    TIMER_PHY_STATUS_T *status;	

    if (timer_id >= (uint32) TIMER_CFG_get_number()) 
    {
        return -1;
    }

    status = TIMER_CFG_get_status(timer_id);
    if (!status)
    {
    	return -1;
    }

    config.irq_op = status->irq_status;
    config.mode = status->mode;
    config.load_count = status->max_count;

    timer_phy_config (timer_id, &config);

    return 0;

}

//******************************************************************************
//  Description:  to start timer counting
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_enable (uint32 timer_id)
{
    timer_s *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    timer_ctl = (timer_s *) timer_base_addr[timer_id]; 
    SET_BIT (timer_ctl->ctl, 7);

    return 0;

}

//******************************************************************************
//  Description:  to stop timer counting
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_disable (uint32 timer_id)
{
    timer_s *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    timer_ctl = (timer_s *) timer_base_addr[timer_id];
    CLR_BIT (timer_ctl->ctl, 7);

	if(timer_id == TIMER_5)
	{
		//when timer2 & timer5 cnt=1 it's irq can not be cleared
		//so do follow statement
		timer_ctl->load = TIMER2_5_CLR_IRQ_MAGIC_NUM;
		SET_BIT(timer_ctl->clr, 3);		//clear timer5 irq	
	}
    return 0;
}

//******************************************************************************
//  Description:  to clr the irq signal after timer generate
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_clrirq (uint32 timer_id)
{
    timer_s *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    timer_ctl = (timer_s *) timer_base_addr[timer_id]; 

    SET_BIT (timer_ctl->clr, 3);

    return 0;
}

//******************************************************************************
//  Description:  to get the current status of timer through checking the register
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         status: the struct pointer of status
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_query (uint32 timer_id, TIMER_PHY_STATUS_T *status)
{
    timer_s *timer_ctl;
    uint32 temp;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (status == NULL)
    {
        return -2;
    }

    timer_ctl = (timer_s *) timer_base_addr[timer_id]; 

    if((TIMER_0  == timer_id) || (TIMER_1  == timer_id))  
    {
        status->current_value = _GetTimerValue(timer_id);
    }
    else  // get timer2 value
    {
        status->current_value = timer_ctl->value;
    }
    
    temp= timer_ctl->ctl;
    status->mode = (temp& (0x1<<6)) ? TIMER_MODE_PERIOD : TIMER_MODE_FREE;
    status->running_status = (temp& (0x1<<7)) ? 1 : 0;

    temp= timer_ctl->clr;
    status->irq_status = (temp&0x1) ? 1 : 0;

    return 0;
}

//******************************************************************************
//  Description:  to change the count register of timer in special situation
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  this function just be used in some situation
//******************************************************************************
LOCAL int32 timer_phy_reload (uint32 timer_id)
{
    return 0;
}

/* the operation set of timer phy */
LOCAL TIMER_PHY_OPERATION_T timer_phy_op =
{
    timer_phy_init,
    timer_phy_enable,
    timer_phy_disable,
    timer_phy_config,
    timer_phy_query,
    timer_phy_clrirq,
    timer_phy_reload,
};

//******************************************************************************
//  Description:  to get the operation set of timer phy
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  NONE
//  Note:  NONE
//******************************************************************************
PUBLIC TIMER_PHY_OPERATION_T *timer_get_phy_operation (void)
{
    return &timer_phy_op;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of timer_drv.c
