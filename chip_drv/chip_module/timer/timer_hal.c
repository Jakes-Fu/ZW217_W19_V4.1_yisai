/******************************************************************************
 ** File Name:      timer_hal.c                                               *
 ** Author:         Ji.Ding                                                   *
 ** DATE:           08/09/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 timer count .                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 08/09/2010      DingJi          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
///#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "timer_drvapi.h"
#include "timer_phy.h"
//#include "pwm_drvapi.h"
//#include "tb_drv.h"
//#include "isr_drvapi.h"
//#include "tb_drv.h"
//#include "isr_drvapi.h"
#include "fiq_drvapi.h"

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
// The backup setting of timer.
typedef struct
{
    uint32 timer_reset_count; //just for period type
    uint32 timer_reset_loop;  //just for period type

    uint32 timer_remaining_count;
    uint32 timer_remaining_loop;

    TIMER_CALLBACK_FUNC timer_cb_func_ptr;
} TIMER_COUNT_SETTING_T;

LOCAL TIMER_COUNT_SETTING_T s_timer_setting[TIMER_MAX];

// some information about timer
typedef struct
{
    TIMER_PHY_STATUS_T  *timer_st;
    TIMER_UNIT_E        unit_type;
    uint32              count_per_unit;
    BOOLEAN             open_flag;
} TIMER_STATUS_T;

LOCAL TIMER_STATUS_T s_timer_state[TIMER_MAX];

// the lower driver of timer
LOCAL TIMER_PHY_OPERATION_T *s_timer_func_ptr=NULL;

// the lower configure of timer
LOCAL TIMER_PHY_CONFIG_T s_timer_phy_config[TIMER_MAX];

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/* this function transfer us to the timer count depends on the timer configuration */
LOCAL uint32 TIMER_Calculate_unit2cnt (TIMER_ID_E timer_id, uint32 unit_number)
{
    uint32 clk=s_timer_state[timer_id].timer_st->clk_freq;
    uint32 cnt=s_timer_state[timer_id].count_per_unit;
    uint32 temp=0;

    if (0 == unit_number)
    {
        return 0;
    }

    if (cnt)
    {
        return (cnt*unit_number);
    }
    else if (TIMER_UNIT_ms == s_timer_state[timer_id].unit_type) //time unit is ms
    {
        //divide time_unit to Xs and Yms
        temp = (unit_number/TIMER_MILSEC_PER_SECOND); // get Xs
        cnt += temp*clk; // Xs to cnt

        unit_number -= temp*TIMER_MILSEC_PER_SECOND; //get Yms
        cnt += unit_number*clk/TIMER_MILSEC_PER_SECOND; //Yms to cnt

        return cnt;
    }
    else ////time unit is us
    {
        //divide time_unit to Xs and Yms and Zus
        temp = unit_number/ (TIMER_MILSEC_PER_SECOND*TIMER_MICSEC_PER_MILSEC); //get Xs
        cnt += temp*clk; //Xs to cnt

        unit_number -= temp* (TIMER_MILSEC_PER_SECOND*TIMER_MICSEC_PER_MILSEC);
        temp = unit_number/TIMER_MILSEC_PER_SECOND; //get Yms
        clk = clk/TIMER_MILSEC_PER_SECOND; //to avoid overlap, down the clk
        cnt += temp*clk; //Yms to cnt

        unit_number -= temp*TIMER_MICSEC_PER_MILSEC; //get Zus
        cnt += unit_number*clk/TIMER_MICSEC_PER_MILSEC; //Zus to cnt

        return cnt;
    }
}

/* this function transfer count to the us depends on the timer configuration */
LOCAL uint32  TIMER_Calculate_cnt2unit (TIMER_ID_E timer_id, uint32 cnt)
{
    uint32 clk=s_timer_state[timer_id].timer_st->clk_freq;
    uint32 temp=s_timer_state[timer_id].count_per_unit;
    uint32 result=0;

    if (0 == cnt)
    {
        return 0;
    }

    if (temp)
    {
        return (cnt/temp);
    }
    else if (TIMER_UNIT_ms == s_timer_state[timer_id].unit_type) //time unit is ms
    {
        temp = cnt/clk; //get Xs
        result += temp*TIMER_MILSEC_PER_SECOND; //(X*1000)ms

        cnt -= temp*clk;
        result += (cnt*TIMER_MILSEC_PER_SECOND) /clk; //get Yms

        return result;
    }
    else //time unit is us
    {
        clk = clk/TIMER_MILSEC_PER_SECOND;

        temp = cnt/clk;
        result += temp*TIMER_MICSEC_PER_MILSEC;

        cnt -= temp*clk;
        result += (cnt*TIMER_MICSEC_PER_MILSEC) /clk;

        return result;
    }
}

//******************************************************************************
//  Description:  configure timer as needed
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:   timer_id: the timer index
//          config: the user needed configuration of timer
//  Note:  NONE
//******************************************************************************
LOCAL int32 TIMER_Configure (TIMER_ID_E timer_id, TIMER_CONFIG_T *config)
{
    uint32 cnt;
    TIMER_COUNT_SETTING_T *timer_setting = &s_timer_setting[timer_id];
    TIMER_STATUS_T *timer_sp = &s_timer_state[timer_id];

    if (NULL == config)
    {
        return TIMER_ERR_POINTER_NULL;
    }

    s_timer_func_ptr->disable (timer_id);
    s_timer_func_ptr->clr_irq (timer_id);

    if (TIMER_UNIT_ms == config->unit_type)
    {
        if (TIMER_UNIT_ms == timer_sp->unit_type)
        {
            timer_setting->timer_remaining_loop = config->unit_number / timer_sp->timer_st->max_unit;
            timer_setting->timer_remaining_count = \
                                                   TIMER_Calculate_unit2cnt (timer_id, config->unit_number%timer_sp->timer_st->max_unit);
        }
        else if (TIMER_UNIT_us == timer_sp->unit_type)
        {
            SCI_PASSERT (config->unit_number < (0xFFFFFFFF/TIMER_MICSEC_PER_MILSEC), ("Over MAX Timer Setting")); /*assert verified*/

            cnt = config->unit_number * TIMER_MICSEC_PER_MILSEC; //complain with us
            timer_setting->timer_remaining_loop = cnt / timer_sp->timer_st->max_unit;
            timer_setting->timer_remaining_count = \
                                                   TIMER_Calculate_unit2cnt (timer_id, cnt%timer_sp->timer_st->max_unit);
        }
        else
        {
            SCI_PASSERT (0, ("wrong timer unit type")); /*assert verified*/
        }
    }
    else if (TIMER_UNIT_us == config->unit_type)
    {
        if (TIMER_UNIT_ms == timer_sp->unit_type)
        {
            cnt = config->unit_number / TIMER_MICSEC_PER_MILSEC; //complain with ms
            timer_setting->timer_remaining_loop = cnt / timer_sp->timer_st->max_unit;
            timer_setting->timer_remaining_count = \
                                                   TIMER_Calculate_unit2cnt (timer_id, cnt%timer_sp->timer_st->max_unit);
        }
        else if (TIMER_UNIT_us == timer_sp->unit_type)
        {
            timer_setting->timer_remaining_loop = config->unit_number / timer_sp->timer_st->max_unit;
            timer_setting->timer_remaining_count = \
                                                   TIMER_Calculate_unit2cnt (timer_id, config->unit_number%timer_sp->timer_st->max_unit);
        }
        else
        {
            SCI_PASSERT (0, ("wrong timer unit type")); /*assert verified*/
        }
    }
    else
    {
        SCI_PASSERT (0, ("wrong timer set unit type")); /*assert verified*/
    }

    if (! (timer_setting->timer_remaining_count) && (timer_setting->timer_remaining_loop))
    {
        timer_setting->timer_remaining_count = timer_sp->timer_st->max_count;
        timer_setting->timer_remaining_loop--;
    }

    switch (config->mode)
    {
        case TIMER_ONESHOT_MODE:
            timer_setting->timer_reset_count = 0;
            timer_setting->timer_reset_loop = 0;
            break;

        case TIMER_PERIOD_MODE:
            timer_setting->timer_reset_count = timer_setting->timer_remaining_count;
            timer_setting->timer_reset_loop = timer_setting->timer_remaining_loop;
            break;

        default:
            break;
    }

    timer_setting->timer_cb_func_ptr = config->call_back;

#if defined(PLATFORM_UWS6121E)
    if (OS_TIMER == timer_id)
    {
        s_timer_phy_config[timer_id].mode = (TIMER_PHY_MODE_E)config->mode;
        s_timer_phy_config[timer_id].load_count = (TIMER_PHY_MODE_E)config->unit_number;
    }
    else
    {
        s_timer_phy_config[timer_id].mode = (TIMER_PHY_MODE_E)config->mode;
        s_timer_phy_config[timer_id].load_count = (timer_setting->timer_remaining_count>2) ? \
                timer_setting->timer_remaining_count : 2;
    }
#else
        s_timer_phy_config[timer_id].mode = (TIMER_PHY_MODE_E)config->mode;
        s_timer_phy_config[timer_id].load_count = (timer_setting->timer_remaining_count>2) ? \
                timer_setting->timer_remaining_count : 2;
#endif

    if (0 != s_timer_func_ptr->config (timer_id, &s_timer_phy_config[timer_id]))
    {
        return TIMER_ERR_SET_FAIL;
    }

#if !defined(PLATFORM_UWS6121E)
    if (timer_id == TIMER_2 || timer_id == TIMER_5) //wait for timer2 load effective
    {
        for (cnt=10; cnt>0;)
        {
            cnt--;
        }
    }
#endif
    return TIMER_SUCCESS;

}

//******************************************************************************
//  Description:  to handle logical timer event like expire in the ISR of timer interrupt
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:   timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 TIMER_Process (TIMER_ID_E timer_id)
{
    uint32 cnt=0;

    TIMER_COUNT_SETTING_T   *timer_setting = &s_timer_setting[timer_id];
    TIMER_PHY_CONFIG_T      *timer_phy = &s_timer_phy_config[timer_id];
    TIMER_STATUS_T          *timer_sp = &s_timer_state[timer_id];

    if (timer_setting->timer_remaining_loop) // no expired yet
    {
        timer_setting->timer_remaining_loop--;
        timer_setting->timer_remaining_count = 0;

        timer_phy->load_count = timer_sp->timer_st->max_count;
        s_timer_func_ptr->config (timer_id, timer_phy);

        if (TIMER_2 == timer_id)
        {
            for (cnt=10; cnt>0;)
            {
                cnt--;
            }
        }
        else if (TIMER_1 == timer_id)
        {
            for (cnt=10; cnt>0;)
            {
                cnt--;
            }
        }


        s_timer_func_ptr->enable (timer_id); //next loop
    }
    else //expired already
    {
        if (timer_setting->timer_reset_count) //periodic mode
        {
            timer_setting->timer_remaining_count = timer_setting->timer_reset_count;
            timer_setting->timer_remaining_loop = timer_setting->timer_reset_loop;

            timer_phy->load_count = timer_setting->timer_remaining_count;

#ifdef TB_TIMER5_INT
    if(TIMER_5 != timer_id)
    {
#endif
            s_timer_func_ptr->config (timer_id, timer_phy);

            if (TIMER_2 == timer_id)
            {
                for (cnt=10; cnt>0;)
                {
                    cnt--;
                }
            }

            s_timer_func_ptr->enable (timer_id); //start next turn
#ifdef TB_TIMER5_INT
    }
#endif
            if (timer_setting->timer_cb_func_ptr) //call back
            {
                timer_setting->timer_cb_func_ptr();
            }
        }
        else //oneshot over, clean timer
        {
            if (timer_setting->timer_cb_func_ptr)
            {
                timer_setting->timer_cb_func_ptr(); //call back
            }

            SCI_MEMSET (timer_setting, 0x00, sizeof (TIMER_COUNT_SETTING_T));
        }
    }

    return TIMER_SUCCESS;
}


//******************************************************************************
//  Description:  the ISR of timer interrupt(IRQ)
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  int_num: the number of timer's IRQ which is defined by CHIP SET
//  Note:  NONE
//******************************************************************************
LOCAL void TIMER_IRQ_Handler (uint32 int_num)
{
    TIMER_ID_E timer_id;

    switch (int_num)
    {
        case (uint32) TB_TIMER0_INT:
            timer_id = TIMER_0;
            break;

        case (uint32) TB_TIMER1_INT:
            timer_id = TIMER_1;
            break;
		case (uint32) TB_TIMER1_OS_INT:
            timer_id = TIMER_1;
            break;

#ifdef TB_TIMER2_INT
        case (uint32) TB_TIMER2_INT:
            timer_id = TIMER_2;
            break;
#endif

#ifdef TB_TIMER3_OS_INT
        case (uint32) TB_TIMER3_OS_INT:
            timer_id = TIMER_3;
            break;
#endif

#ifdef TB_TIMER5_INT
		case (uint32) TB_TIMER5_INT:
			timer_id = TIMER_5;
			break;

#endif

        default:
            return;
    }

#if !defined(PLATFORM_UWS6121E)
#ifdef TB_TIMER5_INT
    if(TIMER_5 != timer_id)
    {
#endif
    s_timer_func_ptr->disable (timer_id);
#ifdef TB_TIMER5_INT
    }
#endif
#endif

    s_timer_func_ptr->clr_irq (timer_id);

    TIMER_Process (timer_id);

}

//******************************************************************************
//  Description:  the ISR of timer interrupt(FIQ)
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  int_num: the number of timer's FIQ which is defined by CHIP SET
//  Note:  NONE
//******************************************************************************
LOCAL void TIMER_FIQ_Handler (uint32 int_num)
{
    TIMER_ID_E timer_id;

    switch (int_num)
    {
        case (uint32) TB_TIMER0_INT:
            timer_id = TIMER_0;
            break;

        case (uint32) TB_TIMER1_INT:
            timer_id = TIMER_1;
            break;

#ifdef TB_TIMER2_INT
        case (uint32) TB_TIMER2_INT:
            timer_id = TIMER_2;
            break;
#endif

#ifdef TB_TIMER5_INT
		case (uint32) TB_TIMER5_INT:
			timer_id = TIMER_5;
			break;
#endif

        default:
            return;
    }

#ifdef TB_TIMER5_INT
    if(TIMER_5 != timer_id)
    {
#endif
    s_timer_func_ptr->disable (timer_id);
#ifdef TB_TIMER5_INT
    }
#endif

    s_timer_func_ptr->clr_irq (timer_id);

    // Disable TIMER interrupt.
    CHIPDRV_DisableFIQINT (int_num);

}

//******************************************************************************
//  Description:  get the remaining time till timer expire
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         remaining_us: the pointer of remaining time
//  Note:  NONE
//******************************************************************************
LOCAL int32 TIMER_Get_Remaining (TIMER_ID_E timer_id, TIMER_REMAINING_T *result)
{
    uint32 cnt=0;
    uint32 unit=0;
    TIMER_COUNT_SETTING_T *timer_setting = &s_timer_setting[timer_id];
    TIMER_STATUS_T *timer_sp = &s_timer_state[timer_id];

    s_timer_func_ptr->query (timer_id, timer_sp->timer_st);
    cnt = timer_sp->timer_st->current_value;

    cnt = (cnt < timer_setting->timer_remaining_count) ? cnt : 0;
    unit = TIMER_Calculate_cnt2unit (timer_id, cnt);

    cnt = unit + timer_setting->timer_remaining_loop * timer_sp->timer_st->max_unit;

    if (timer_sp->unit_type == result->unit_type)
    {
        result->unit_number = cnt;
    }
    else if ( (TIMER_UNIT_ms == timer_sp->unit_type) && (TIMER_UNIT_us == result->unit_type))
    {
        SCI_PASSERT (cnt < (0xFFFFFFFF/TIMER_MICSEC_PER_MILSEC), ("remaining result overflow")); /*assert verified*/

        result->unit_number = cnt*TIMER_MICSEC_PER_MILSEC;
    }
    else
    {
        result->unit_number = cnt/TIMER_MICSEC_PER_MILSEC;
    }

    return TIMER_SUCCESS;

}

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
//******************************************************************************
//  Description:  initialize and open timer module
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
PUBLIC int32 TIMER_HAL_Open (TIMER_ID_E timer_id)
{
    int32 ret=0;
    uint32 status=0;
    uint32 clk=0;

    if ( (uint32) timer_id >= TIMER_CFG_get_number())
    {
        return TIMER_ERR_NO_SUCH_TIMER;
    }

    //below as initialize
    if (s_timer_func_ptr == NULL)
    {
        SCI_MEMSET (&s_timer_state, 0x00, sizeof (s_timer_state));
        s_timer_func_ptr = timer_get_phy_operation();
    }

    if (s_timer_state[timer_id].open_flag) //have been opened already
    {
        return TIMER_SUCCESS;
    }

    SCI_MEMSET (&s_timer_setting[timer_id], 0x00, sizeof (TIMER_COUNT_SETTING_T));

    s_timer_state[timer_id].timer_st = TIMER_CFG_get_status (timer_id);
    SCI_PASSERT (s_timer_state[timer_id].timer_st!=0, ("timer phy state is NULL"));/*assert verified*/

    clk = s_timer_state[timer_id].timer_st->clk_freq;
    SCI_PASSERT (clk>TIMER_MILSEC_PER_SECOND, ("timer clk is too lower")); /*assert verified*/

    if (clk < TIMER_MILSEC_PER_SECOND*TIMER_MICSEC_PER_MILSEC) //this timer support 1ms accurancy
    {
        s_timer_state[timer_id].unit_type = TIMER_UNIT_ms;

        if (clk % TIMER_MILSEC_PER_SECOND)
        {
            s_timer_state[timer_id].count_per_unit = 0; //can't be integrate divided
        }
        else
        {
            s_timer_state[timer_id].count_per_unit = clk/TIMER_MILSEC_PER_SECOND;
        }
    }
    else //this timer support 1us accurancy
    {
        s_timer_state[timer_id].unit_type = TIMER_UNIT_us;

        if (clk % (TIMER_MILSEC_PER_SECOND*TIMER_MICSEC_PER_MILSEC))
        {
            s_timer_state[timer_id].count_per_unit = 0; //can't be integrate divided
        }
        else
        {
            s_timer_state[timer_id].count_per_unit = clk/ (TIMER_MILSEC_PER_SECOND*TIMER_MICSEC_PER_MILSEC);
        }
    }

    ret=s_timer_func_ptr->init (timer_id);

    if (ret!=0)
    {
        return TIMER_ERR_INIT_FAIL;
    }

    ret=s_timer_func_ptr->query (timer_id, s_timer_state[timer_id].timer_st);

    if (ret!=0)
    {
        return TIMER_ERR_QUERY_FAIL;
    }

    switch (timer_id)
    {
        case TIMER_0:
            // Register ISR of TIMER0
            status = ISR_RegHandler (TB_TIMER0_INT, (TB_ISR) TIMER_IRQ_Handler);

            if (status != TB_SUCCESS)
            {
                return status;
            }

            s_timer_phy_config[0].irq_op = 1;
            CHIPDRV_EnableIRQINT (TB_TIMER0_INT);
            break;

        case TIMER_1:

			#if 0
            // Register TIMER1 FIQ
            status = FIQ_RegHandler (TB_TIMER1_INT, TIMER_FIQ_Handler);
            SCI_PASSERT ( (FIQ_SUCCESS == status || FIQ_ISR_ASSIGNED == status),/*assert verified*/
                          ("%s, %d, fiq regist status: %d", __MODULE__, __LINE__, status));

            s_timer_phy_config[1].irq_op = 1;
			#endif

			// Register ISR of TIMER0
            status = ISR_RegHandler (TB_TIMER1_OS_INT, (TB_ISR) TIMER_IRQ_Handler);

            if (status != TB_SUCCESS)
            {
                return status;
            }

            s_timer_phy_config[1].irq_op = 1;
            CHIPDRV_EnableIRQINT (TB_TIMER1_OS_INT);


            break;

#ifdef TB_TIMER2_INT
        case TIMER_2:
            // Register ISR of TIMER0
            status = ISR_RegHandler (TB_TIMER2_INT, (TB_ISR) TIMER_IRQ_Handler);

            if (status != TB_SUCCESS)
            {
                return status;
            }

            s_timer_phy_config[2].irq_op = 1;
            CHIPDRV_EnableIRQINT (TB_TIMER2_INT);
            break;
#endif
#ifdef TB_TIMER3_OS_INT
        case TIMER_3:
            // Register ISR of TIMER0
            status = ISR_RegHandler (TB_TIMER3_OS_INT, (TB_ISR) TIMER_IRQ_Handler);

            if (status != TB_SUCCESS)
            {
                return status;
            }

            s_timer_phy_config[3].irq_op = 1;
            CHIPDRV_EnableIRQINT (TB_TIMER3_OS_INT);
            break;
#endif

#ifdef TB_TIMER5_INT
		case TIMER_5:
			status = ISR_RegHandler(TB_TIMER5_INT,(TB_ISR)TIMER_IRQ_Handler);
            if (status != TB_SUCCESS)
            {
                SCI_TraceLow("[Kui Wang 1]Timer 5 register fail",status);
                return status;
            }

            s_timer_phy_config[5].irq_op = 1;
            CHIPDRV_EnableIRQINT (TB_TIMER5_INT);
            break;
#endif
        default:
            break;
    }

    s_timer_state[timer_id].open_flag = 1; //open successful

    return TIMER_SUCCESS;

}

//******************************************************************************
//  Description:  close timer module
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
PUBLIC int32 TIMER_HAL_Close (TIMER_ID_E timer_id)
{
    int32 ret;

    if ( (uint32) timer_id >= TIMER_CFG_get_number())
    {
        return TIMER_ERR_NO_SUCH_TIMER;
    }

    if (s_timer_state[timer_id].open_flag == 0) //never open before
    {
        return 0;
    }

    SCI_DisableIRQ();

    ret=s_timer_func_ptr->disable (timer_id);

    if (ret!=0)
    {
        SCI_RestoreIRQ();
        return TIMER_ERR_DISABLE_FAIL;
    }

    ret=s_timer_func_ptr->clr_irq (timer_id);

    if (ret!=0)
    {
        SCI_RestoreIRQ();
        return TIMER_ERR_CLRINT_FAIL;
    }

    switch (timer_id)
    {
        case TIMER_0:
            CHIPDRV_DisableIRQINT (TB_TIMER0_INT);
            break;

        case TIMER_1:
            break;

#ifdef TB_TIMER2_INT
        case TIMER_2:
            CHIPDRV_DisableIRQINT (TB_TIMER2_INT);
            break;
#endif

        default:
            break;
    }

    s_timer_state[timer_id].open_flag = 0; //close successful

    SCI_RestoreIRQ();

    return TIMER_SUCCESS;
}

//******************************************************************************
//  Description:  timer control
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         cmd: different command definition
//         arg: parameter pointer with command
//  Note:  NONE
//******************************************************************************
PUBLIC int32 TIMER_HAL_IOctl (TIMER_ID_E timer_id, uint32 cmd, void *arg)
{
    int32 ret;
    TIMER_CONFIG_T *config;

    if ( (uint32) timer_id >= TIMER_CFG_get_number())
    {
        return TIMER_ERR_NO_SUCH_TIMER;
    }

    if (s_timer_state[timer_id].open_flag == 0) //never open before
    {
        return TIMER_ERR_NOT_OPEN;
    }

    SCI_DisableIRQ();

    switch (cmd)
    {
        case TIMER_IOCTL_CMD_START: //timer start counting

            ret=s_timer_func_ptr->enable (timer_id);

            if (ret != 0)
            {
                ret = TIMER_ERR_ENABLE_FAIL;
            }

            break;

        case TIMER_IOCTL_CMD_STOP: //timer stop counting

            ret=s_timer_func_ptr->disable (timer_id);

            if (ret != 0)
            {
                ret = TIMER_ERR_DISABLE_FAIL;
            }

            break;

        case TIMER_IOCTL_CMD_CONFIG: //timer set

            if (arg == NULL)
            {
                ret=TIMER_ERR_POINTER_NULL;
                break;
            }
            else
            {
                config = (TIMER_CONFIG_T *) arg;
            }

            ret=TIMER_Configure (timer_id, config);

            if (ret != 0)
            {
                ret = TIMER_ERR_SET_FAIL;
                break;
            }

            break;

        case TIMER_IOCTL_CMD_GET_REMAINING: //get remaining time

            if (arg == NULL)
            {
                ret=TIMER_ERR_POINTER_NULL;
                break;
            }

            ret=TIMER_Get_Remaining (timer_id, (TIMER_REMAINING_T *) arg);

            break;

        default:
            ret=TIMER_ERR_PARAM;
            break;
    }

    SCI_RestoreIRQ();

    return ret;

}

//******************************************************************************
//  Description:  get the system count value (1ms unit)
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  NONE
//  Note:  NONE
//******************************************************************************
PUBLIC uint32 TIMER_GetSystemCounterReg (void) // return the value of system count.
{
#if defined(PLATFORM_UWS6121E)
	return timer_TimTickGetHwVal(OS_TIMER);
#else
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
#endif
}

//******************************************************************************
//  Description:  delay untill timeout
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  us: the value of timeout
//  Note:  this function just use timer2
//******************************************************************************
PUBLIC void TIMER_DelayUs (uint32 us)
{
    uint32 ms = us/TIMER_MICSEC_PER_MILSEC;
    static uint32 tick=0;

    TIMER_CONFIG_T timer2_config;

    us = us%TIMER_MICSEC_PER_MILSEC;

    if (ms)
    {
        tick = TIMER_GetSystemCounterReg();

        while ( (TIMER_GetSystemCounterReg() - tick) < (ms + 1)) {}
    }

    if (us)
    {
        SCI_DisableIRQ();

#ifdef TB_TIMER2_INT
        CHIPDRV_DisableIRQINT (TB_TIMER2_INT);
#endif

        timer2_config.mode = TIMER_ONESHOT_MODE;
        timer2_config.unit_type = TIMER_UNIT_us;
        timer2_config.unit_number = us;
        timer2_config.call_back = 0;
        TIMER_Configure (TIMER_2, &timer2_config);

        tick = TIMER_GetSystemCounterReg();
        s_timer_func_ptr->enable (TIMER_2);

        do
        {
            s_timer_func_ptr->query (TIMER_2, s_timer_state[TIMER_2].timer_st);

            if (TIMER_GetSystemCounterReg() - tick >= 2)
            {
                break;
            }
        }
        while (s_timer_state[TIMER_2].timer_st->current_value);

        SCI_RestoreIRQ();
    }

}

//******************************************************************************
//  Description:  reload the timer
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  NONE
//  Note:  this function is special for OS module
//******************************************************************************
PUBLIC void TIMER_ReLoad (void)
{
    s_timer_func_ptr->reload (TIMER_0);
}

//******************************************************************************
//  Description: Get timer register count value (TIMER_0-TIMER_2)
//  Global resource dependence:  NONE
//  Author:  Mingwei.Zhang
//  para:  NONE
//  Note:  NONE
//******************************************************************************
PUBLIC uint32 TIMER_GetCntValue (TIMER_ID_E timer_id)
{
    TIMER_PHY_STATUS_T temp;
    int32 ret;

    ret = s_timer_func_ptr->query (timer_id, &temp);

    SCI_ASSERT ( (ret == 0));/*assert verified*/

    return temp.current_value;
}

//******************************************************************************
//  Description:
//  Global resource dependence:  NONE
//  Author:
//  para:  NONE
//  Note:  NONE
//******************************************************************************
LOCAL uint32 periodtime_ref_cnt = 0x0;
typedef void (*TM_FUNC)(uint32);
PUBLIC uint32 TIMER_CreatePeriodTimer (TIMER_ID_E timer_id,uint32 expire_time, TIMER_CALLBACK_FUNC tm_func,uint32 is_autoactive)
{
	TIMER_CONFIG_T config;
	int32 status;
    if(!periodtime_ref_cnt)
    {
		TIMER_HAL_Open(timer_id);
		periodtime_ref_cnt++;
    }
	config.mode = TIMER_PERIOD_MODE;
	config.call_back = tm_func;
	config.unit_type = TIMER_UNIT_us;
	config.unit_number = expire_time;
	status = TIMER_Configure(timer_id,&config);

	if(is_autoactive)
	{
		TIMER_HAL_IOctl(timer_id,TIMER_IOCTL_CMD_START,0);
	}
	return status;
}

PUBLIC uint32 TIMER_ActivePeriodTimer(TIMER_ID_E timer_id,uint32 is_active)
{
	int32 status;
	if(is_active)
	{
		status = TIMER_HAL_IOctl(timer_id,TIMER_IOCTL_CMD_START,0);
	}
	else
	{
		status = TIMER_HAL_IOctl(timer_id,TIMER_IOCTL_CMD_STOP,0);
	}
	return status;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of timer_drv.c
