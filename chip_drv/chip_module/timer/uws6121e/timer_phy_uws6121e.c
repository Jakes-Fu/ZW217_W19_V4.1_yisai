/******************************************************************************
 ** File Name:      timer_phy_v0.c                                            *
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
#define SCI_TICKTOMILLISECOND(_TICK)        (((uint64)_TICK *1000)  / 16384)
#define SCI_32TICKTOMILLISECOND(_TICK)    (((uint64)_TICK *1000) / 32768)

uint32 g_timer_ctrl_value[TIMER_PHY_NUMBER];

#define  TIMER_SAVE_REG_NUM     (3)
struct HAL_IP_SUSPEND_RESUME_T
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};

typedef struct
{
    uint32	SavedTIMER_WDCTRL;
    uint32	SavedTIMER_HWCTRL;
    uint32	SavedTIMER_IRQMASK;
}HAL_SAVED_TIMERREGS_T;

HAL_SAVED_TIMERREGS_T g_timer_regs_data[2];
static struct HAL_IP_SUSPEND_RESUME_T g_timer_node[2];

typedef void (*pfunc_void)(void);
void timer_PhySuspendResumeLogin(uint32 id);
void timer_PhySuspendResumeLogout(uint32 id);


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
#if 0
    uint32  time_out = 0;
    volatile uint32 tmp_count1;
    volatile uint32 tmp_count2;
    timer_s *timer_ctl = TB_NULL;

    SCI_ASSERT ( (TIMER_0 == timer_id) || (TIMER_1 == timer_id));/*assert verified*/

    // Set local variable.
    timer_ctl = (timer_s *) ( (uint32) TIMER0_BASE + timer_id*0x20);

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
#else
    return 0;
#endif
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
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    uint32 *timer_ctl_value;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (config == NULL)
    {
        return -2;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            break;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            break;
        }
    }

    timer_ctl_value = &g_timer_ctrl_value[timer_id];
    *timer_ctl_value = 0;

    if (config->mode == TIMER_MODE_FREE)
    {
        *timer_ctl_value |= TIMER_WRAP;
    }
    else
    {
        *timer_ctl_value &= ~TIMER_WRAP;
        *timer_ctl_value |= TIMER_REPEAT;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
    		if (config->irq_op)
    		{
        		timer_ctl_1->Timer_Irq_Mask_Set = TIMER_OSTIMER_MASK;
    		}
    		else
    		{
        		timer_ctl_1->Timer_Irq_Mask_Clr = TIMER_OSTIMER_MASK;
    		}
        }
	break;

        case TIMER_2:
        {
    		if (config->irq_op)
    		{
        		timer_ctl_2->timer_irq_mask_set = TIMER_OSTIMER_MASK;
    		}
    		else
    		{
        		timer_ctl_2->timer_irq_mask_clr = TIMER_OSTIMER_MASK;
    		}
        }
	break;

        default:
        {
    		if (config->irq_op)
    		{
        		timer_ctl_2->timer_irq_mask_set = TIMER_OSTIMER_MASK;
    		}
    		else
    		{
        		timer_ctl_2->timer_irq_mask_clr = TIMER_OSTIMER_MASK;
    		}
        }
	break;
    }

    *timer_ctl_value |= TIMER_LOAD | TIMER_LOADVAL(config->load_count);

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

    timer_PhySuspendResumeLogin(timer_id);
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
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
	     timer_ctl_1->OSTimer_Ctrl = g_timer_ctrl_value[timer_id] | TIMER_ENABLE;
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
    	     timer_ctl_2->ostimer_ctrl = g_timer_ctrl_value[timer_id] | TIMER_ENABLE;
            break;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
    	     timer_ctl_2->ostimer_ctrl = g_timer_ctrl_value[timer_id] | TIMER_ENABLE;
            break;
        }
    }

    return 0;

}

//******************************************************************************
//  Description:  to stop timer counting
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
int32 timer_phy_disable (uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
	     timer_ctl_1->OSTimer_Ctrl &= ~TIMER_ENABLE;
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
	      timer_ctl_2->ostimer_ctrl &= ~TIMER_ENABLE;
            break;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
	     timer_ctl_2->ostimer_ctrl &= ~TIMER_ENABLE;
            break;
        }
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
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
		timer_ctl_1->Timer_Irq_Clr = TIMER_OSTIMER_CLR;
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
	     timer_ctl_2->timer_irq_clr = TIMER_OSTIMER_CLR;
            break;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
	     timer_ctl_2->timer_irq_clr = TIMER_OSTIMER_CLR;
            break;
        }
    }

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
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    uint32 temp;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (status == NULL)
    {
        return -2;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            break;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            break;
        }
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
    		status->current_value = timer_ctl_1->OSTimer_CurVal;

    		temp= timer_ctl_1->OSTimer_Ctrl;
    		status->mode = (temp & TIMER_WRAP) ? TIMER_MODE_PERIOD : TIMER_MODE_FREE;
    		status->running_status = (temp& (0x1<<7)) ? 1 : 0;

    		temp= timer_ctl_1->Timer_Irq_Mask_Set;
    		status->irq_status = (temp & TIMER_OSTIMER_MASK) ? 1 : 0;
        }
		break;

        case TIMER_2:
        {
    		status->current_value = timer_ctl_2->ostimer_curval_l;

    		temp= timer_ctl_2->ostimer_ctrl;
    		status->mode = (temp & TIMER_WRAP) ? TIMER_MODE_PERIOD : TIMER_MODE_FREE;
    		status->running_status = (temp& (0x1<<7)) ? 1 : 0;

    		temp= timer_ctl_2->timer_irq_mask_set;
    		status->irq_status = (temp & TIMER_OSTIMER_MASK) ? 1 : 0;
        }
		break;

        default:
        {
    		status->current_value = timer_ctl_2->ostimer_curval_l;

    		temp= timer_ctl_2->ostimer_ctrl;
    		status->mode = (temp & TIMER_WRAP) ? TIMER_MODE_PERIOD : TIMER_MODE_FREE;
    		status->running_status = (temp& (0x1<<7)) ? 1 : 0;

    		temp= timer_ctl_2->timer_irq_mask_set;
    		status->irq_status = (temp & TIMER_OSTIMER_MASK) ? 1 : 0;
        }
		break;
    }


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

uint32 timer_TimTickEnable(uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        SCI_ASSERT(0);
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            return (timer_ctl_1->OSTimer_Ctrl & TIMER_ENABLE);
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->ostimer_ctrl & TIMER_ENABLE);
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->ostimer_ctrl & TIMER_ENABLE);
        }
    }

}

uint32 timer_TimTickIsEnabled(uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            return (timer_ctl_1->OSTimer_Ctrl & TIMER_ENABLED);
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->ostimer_ctrl & TIMER_ENABLED);
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->ostimer_ctrl & TIMER_ENABLED);
        }
    }
}

uint32 timer_TimTickGetVal(uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            return timer_ctl_1->OSTimer_CurVal;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return timer_ctl_2->ostimer_curval_l;
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return timer_ctl_2->ostimer_curval_l;
        }
     }
}

#pragma arm section code = "TIMER_REALTICK_CODE"
uint32 timer_TimRealTickGet(uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            return (timer_ctl_1->HWTimer_CurVal);
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->hwtimer_curval_l);
        }
        default:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            return (timer_ctl_2->hwtimer_curval_l);
        }
     }
}
#pragma arm section code

static uint32 timer_Idle32KRefGet(void)
{
    return hwp_idleLps->lps_32k_ref;
}

uint32 timer_TimTickGetHwVal(uint32 timer_id)
{
    //return (SCI_TICKTOMILLISECOND(timer_TimRealTickGet(timer_id)));
    return (SCI_32TICKTOMILLISECOND(timer_Idle32KRefGet()));
}

void timer_PhyRegSaveId1(void)
{
    uint32 timer_regs_protect_table[TIMER_SAVE_REG_NUM] =
    {
        REG_TIMER1_BASE+0x08,
        REG_TIMER1_BASE+0x10,
        REG_TIMER1_BASE+0x18,
    };
    Slp_LpsRegsSave((uint32*)&g_timer_regs_data[0], timer_regs_protect_table, TIMER_SAVE_REG_NUM);
}

void timer_PhyRegResumeId1(void)
{
    uint32 timer_regs_protect_table[TIMER_SAVE_REG_NUM] =
    {
        REG_TIMER1_BASE+0x08,
        REG_TIMER1_BASE+0x10,
        REG_TIMER1_BASE+0x18,
    };

    Slp_LpsRegsResume((uint32*)&g_timer_regs_data[0], timer_regs_protect_table, TIMER_SAVE_REG_NUM);
}

void timer_PhyRegSaveId2(void)
{
#if 0
    uint32 timer_regs_protect_table[TIMER_SAVE_REG_NUM] =
    {
        REG_TIMER1_BASE+0x08,
        REG_TIMER2_BASE+0x18,
        REG_TIMER2_BASE+0x2c,
    };
    Slp_LpsRegsSave((uint32*)&g_timer_regs_data[1], timer_regs_protect_table, TIMER_SAVE_REG_NUM);
#endif
}

void timer_PhyRegResumeId2(void)
{
#if 0
    uint32 timer_regs_protect_table[TIMER_SAVE_REG_NUM] =
    {
        REG_TIMER1_BASE+0x08,
        REG_TIMER2_BASE+0x18,
        REG_TIMER2_BASE+0x2c,
    };

    Slp_LpsRegsResume((uint32*)&g_timer_regs_data[1], timer_regs_protect_table, TIMER_SAVE_REG_NUM);
#endif
}


void timer_PhySuspendResumeLogin(uint32 id)
{
    if (id > TIMER_2)
    {
        return ;
    }
    else if (TIMER_1 == id)
    {
    	id -= 1;
        strcpy(g_timer_node[id].module_name, "TIMER1");
        g_timer_node[id].reg_suspend = (pfunc_void)timer_PhyRegSaveId1;
        g_timer_node[id].reg_resume = (pfunc_void)timer_PhyRegResumeId1;
        Slp_LpsSuspendLogin(&g_timer_node[id]);
    }
    else
    {
    	id -= 1;
        strcpy(g_timer_node[id].module_name, "TIMER2");
        g_timer_node[id].reg_suspend = (pfunc_void)timer_PhyRegSaveId2;
        g_timer_node[id].reg_resume = (pfunc_void)timer_PhyRegResumeId2;
        Slp_LpsSuspendLogin(&g_timer_node[id]);
    }

}

void timer_PhySuspendResumeLogout(uint32 id)
{
    if (id > 2)
    {
        return ;
    }
    Slp_LpsSuspendLogout(&g_timer_node[id]);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of timer_drv.c

