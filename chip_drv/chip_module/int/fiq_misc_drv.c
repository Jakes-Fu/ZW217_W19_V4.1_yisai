/******************************************************************************
 ** File Name:      fiq_misc_drv.c
 ** Author:         Yuhua.Shi
 ** DATE:           06/01/2009
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file defines the related driver of fiq include dsp fiq
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/01/2009     Yuhua.Shi        Create                                    *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"

#include "fiq_drvapi.h"
#include "task_monitor.h"
#include "timer_drvapi.h"

#if defined(PLATFORM_SC6800H)|| defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "tx_port_thumb.h"
#endif


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
#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
#define GET_CURRENT_TIMER1_VAL      REG32(TM1_VALUE)
#define GET_CURRENT_RTC_CYCLE(_x_)  do {while ((_x_ = GET_CURRENT_TIMER1_VAL)!= GET_CURRENT_TIMER1_VAL){};}while(0)
#define DSP_RX_SLOT_DLY_CNT_MAX     (85)
#define DSP_TX_SLOT_DLY_US_MAX     (2400)//4 //4 slot
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define DSP_RX_STATUS_ADDR          0x30000FDA
#else
#define DSP_RX_STATUS_ADDR          0x10000FDA
#endif
#define GET_DSP_RX_STATUS(_addr_)   REG16(_addr_)
#define DSP_RX_STATUS_HOLD          0x5a5a
#endif

#define TIMER_COUNT_MASK            0x007FFFFF      //bit0~bit22

/******************************************************************************
**
**                          Date Structures
**
******************************************************************************/
#if 0
/* FIQ ISR information structure */
typedef struct fiq_info_tag
{
    FIQ_HANDLE    fiq_handler;
    FIQ_CALLBACK  callback_handler;
} FIQ_INFO_T;
#endif

/******************************************************************************
**
**                          Global Variables
**
******************************************************************************/
#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530) ||defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
LOCAL  uint32 s_arm_hold_cnt = 0;
LOCAL  uint32 s_dsp_queue_sd_cnt = 0;
#endif

/******************************************************************************
**
**                          Function Prototype
**
******************************************************************************/


/******************************************************************************
**
**                          Function Definitions
**
******************************************************************************/

#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
/*****************************************************************************/
// Description :    This function is used to reset the hold arm flag
// Global resource dependence : 
// Author :
// Note :
/*****************************************************************************/
LOCAL void _ResetHoldArmFlag(void)
{
    volatile uint32 rx_status_addr = DSP_RX_STATUS_ADDR;

    //reset hold arm flag to 0
    GET_DSP_RX_STATUS(rx_status_addr)	= 0;
}

/*****************************************************************************/
// Description :    This function is used to init the module of dsp hold arm
// Global resource dependence :
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void DSP_HoldArmInit(void)
{
    //reset hold arm flag
    _ResetHoldArmFlag();
}

/*****************************************************************************/
// Description :    This function is used to process when arm is hold timeout
// Global resource dependence :
// Author : Yuhua.Shi
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN _HoldARMTimerOut (//FALSE: not timeout, TRUE: timeout
    uint32 base_cycle,      //timer tick when arm starts to be hold
    uint32 status           //pulling value when time out, should be 0x5a5a
)
{
    volatile uint32 cur_cycle;
    uint32 retval = TRUE;

    GET_CURRENT_RTC_CYCLE (cur_cycle);

    if ( (cur_cycle < base_cycle) &&
            ( (base_cycle - cur_cycle) > DSP_RX_SLOT_DLY_CNT_MAX))
    {
        TM_SendTestPointRequest (0x90525010,   0xFF);
        retval = TRUE;
    }
    else
    {
        TM_SendTestPointRequest (0x90525000,   base_cycle);
        TM_SendTestPointRequest (0x90525001,   cur_cycle);
        TM_SendTestPointRequest (0x90525002,   status);

        //if timer overrun and time out, still do not assert, just return TRUE
        if ( (cur_cycle >= base_cycle) &&
                ( (TIMER_COUNT_FULL + base_cycle - cur_cycle) > DSP_RX_SLOT_DLY_CNT_MAX))
        {
            retval = TRUE;
        }
        else
        {
            retval = FALSE;
        }
    }

    return retval;
}

/*****************************************************************************/
// Description :    This function is used to hold arm by dsp side
// Global resource dependence :
// Author :  Yuhua.Shi
// Note :
/*****************************************************************************/
#pragma arm section code = "DSP_FIQ_CODE_IRAM"
PUBLIC void _DSP_HoldArmByFIQ (uint32 hold_time)
{
    register volatile uint32 cur_rtc_cycle = 0;
    register volatile uint32 tmp_cycle = 0;
    register uint32 rx_status_hold = DSP_RX_STATUS_HOLD;
    register uint32 rx_status_addr = DSP_RX_STATUS_ADDR;

    GET_CURRENT_RTC_CYCLE (cur_rtc_cycle);

    TM_SendTestPointRequest (0x90525020,   hold_time);
    TM_SendTestPointRequest (0x90525021,   cur_rtc_cycle);
    
    if(HOLD_ARM_TIME_BY_POLLING == hold_time)
    {
#ifdef ARM_CPLLCLK_DEEPSLEEP_SUPPORT
        if(SC6531EFM == CHIP_GetChipType())
        {
            OS_TickDelay (5);//delay 5ms
        }
#endif
        //only for rx hold
        while (rx_status_hold == GET_DSP_RX_STATUS (rx_status_addr))
        {
            GET_CURRENT_RTC_CYCLE (tmp_cycle);

            if ( (cur_rtc_cycle - tmp_cycle) > DSP_RX_SLOT_DLY_CNT_MAX)
            {
                if (_HoldARMTimerOut (cur_rtc_cycle, GET_DSP_RX_STATUS (rx_status_addr)))
                {
                    break;
                }
            }
        }
    }
    else
    {
        //for tx hold
        #if 0
        if((hold_time != 0) && (hold_time < ((DSP_TX_SLOT_DLY_CNT_MAX*1000)>>5)))
        {
            uint32 hold_cycle =  (hold_time*32)/1000 + 1; //hold rtc cycle

            while (1)//lint !e716
            {
                GET_CURRENT_RTC_CYCLE (tmp_cycle);

                if (( (tmp_cycle <= cur_rtc_cycle) && ( (cur_rtc_cycle - tmp_cycle) > hold_cycle)) ||
                    ((tmp_cycle > cur_rtc_cycle) && ( (TIMER_COUNT_FULL + cur_rtc_cycle - tmp_cycle) > hold_cycle)))
                {
                    TM_SendTestPointRequest (0x90525022,   tmp_cycle);
                    break;//the hold time is expired, return directly
                }
            }
        }
        #else
        if(hold_time != 0) 
        {
            //SCI_TRACE_LOW("_DSP_HoldArmByFIQ:cur_rtc_cycle = %d", cur_rtc_cycle);
            uint32 hold_us = (hold_time <= DSP_TX_SLOT_DLY_US_MAX) ? hold_time : DSP_TX_SLOT_DLY_US_MAX;
            
            TIMER_DELAY_US(hold_us);
            
            GET_CURRENT_RTC_CYCLE (tmp_cycle);
            TM_SendTestPointRequest (0x90525022,   tmp_cycle);
            //SCI_TRACE_LOW("_DSP_HoldArmByFIQ:tmp_cycle = %d", tmp_cycle);
        }
        #endif
 
        
    }

}
#pragma arm section code

/*****************************************************************************/
// Description :    This function is used to hold arm not to operate emc.
// Global resource dependence :
// Author :  Yuhua.Shi
// Note : is_enable :    TURE: hold the arm; FALSE: not hold the arm
/*****************************************************************************/
PUBLIC void DSP_HoldArmByFIQ (uint32 hold_time)
{
#if  defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530)  || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    uint32 dsp_clk_flag = 0;

    //ensure dsp_clk is open for sharememory access
    if (!SCI_ShareMemClkStat())
    {
        SCI_ShareMemClkOpen();
        dsp_clk_flag = 1;
    }

    _DSP_HoldArmByFIQ(hold_time);

    if (dsp_clk_flag)
    {
        //close the dsp_clk control bit for sharememory access
        SCI_ShareMemClkClose();
    }

#endif
}

/*****************************************************************************/
// Description :    This function is used to check whether arm hold is enable
// Global resource dependence :
// Author :  Yuhua.Shi
// Note   :  SCI_TRUE:enable, SCI_FALSE:disable
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsEnalbeArmHold (void)
{
    return ( (0 == s_arm_hold_cnt) ? SCI_TRUE:SCI_FALSE);
}

/*****************************************************************************/
// Description :    This function is used to enable the arm hold function.
// Global resource dependence : s_arm_hold_cnt
// Author : Yuhua.Shi
// Note : is_enable :    TURE: hold the arm; FALSE: not hold the arm
/*****************************************************************************/
PUBLIC void SCI_EnableArmHold (
    BOOLEAN is_enable       //TURE: hold the arm; FALSE: not hold the arm
)
{
    TX_IRQ_SAVE_AREA

    TX_DISABLE_IRQ

    TM_SendTestPointRequest (0x90523000 + s_arm_hold_cnt, is_enable);

    if (is_enable)
    {
        if (0 != s_arm_hold_cnt)
        {
            s_arm_hold_cnt--;
        }
    }
    else
    {
        s_arm_hold_cnt++;
    }

    TX_RESTORE_IRQ
}
#endif

/*****************************************************************************/
// Description :    This function is used to enable the function to speed down dsp queue
// Global resource dependence : s_dsp_queue_sd_cnt
// Author :
// Note :
/*****************************************************************************/
PUBLIC void SCI_EnableDspQueueSpeedDown (BOOLEAN is_enable)
{
#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H))
    TX_IRQ_SAVE_AREA

    TX_DISABLE_IRQ

    TM_SendTestPointRequest (0x90524000 + s_dsp_queue_sd_cnt, is_enable);

    if (!is_enable)
    {
        if (0 != s_dsp_queue_sd_cnt)
        {
            s_dsp_queue_sd_cnt--;
        }
    }
    else
    {
        s_dsp_queue_sd_cnt++;
    }

    TX_RESTORE_IRQ
#endif
}

/*****************************************************************************/
// Description :    This function is used to check whether need to speed down dsp queue
// Global resource dependence : s_arm_hold_cnt
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsNeedDspQueueSpeedDown (void)
{
#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H))
    return ( (0 == s_dsp_queue_sd_cnt) ? SCI_FALSE:SCI_TRUE);
#else
    return SCI_FALSE;
#endif
}

/*****************************************************************************/
// Description :    Timer1 fiq handler
// Global resource dependence :
// Author : Yuhua.Shi
// Note :
/*****************************************************************************/
PUBLIC void TIMER1_FiqHandle (
    uint32 int_num                  // Interrupt num.
)
{
#if !defined(PLATFORM_UWS6121E)
    timer_s *tm_ptr;

    // Get the proper TIMER.
    tm_ptr  = (timer_s *) (TIMER1_BASE);
    // Disable TIMER interrupt.
    CHIPDRV_DisableFIQINT (TB_TIMER1_INT);
    // Disable TIMER counting.
    tm_ptr->ctl = 0x0;

    // Clear the old raw-interrupt flag of this timer.
    tm_ptr->clr = 0xFFFF;
#endif
}

#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530)  || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
//*****************************************************************************
//  Description:    This function is called to open share memory clock.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ShareMemClkOpen (void)
{
#ifdef PLATFORM_SC6600L
    REG32 (GR_PCTL) |= BIT_9;
#endif
}
//*****************************************************************************
//  Description:    This function is called to close share memory clock.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ShareMemClkClose (void)
{
#ifdef PLATFORM_SC6600L
    REG32 (GR_PCTL) &= ~ (uint32) BIT_9;
#endif
}
//*****************************************************************************
//  Description:    This function is called to get share memory clock status.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC uint32 SCI_ShareMemClkStat (void)
{
#ifdef PLATFORM_SC6600L
    return (REG32 (GR_PCTL) & BIT_9);
#elif defined (PLATFORM_SC6800H)
    return 1;
#elif defined (PLATFORM_SC6530)
    return 0;//for pclint warning
#elif defined (PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    return 0;
#endif
}
#endif

//*****************************************************************************
//  Description:    This function is called to clear DSP FIQ.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ClrDspFiqInt (void)
{
#ifdef PLATFORM_SC6800H
    REG32 (GR_IRQ) |= (uint32) APB_DSP_FIQ_CLR;
#elif defined(PLATFORM_SC6530)
  *(volatile uint32 *)APB_INT_SET_CLR0  = (uint32)(DSP_FRQ_CLR);
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
  *(volatile uint32 *)APB_INT_SET_CLR0  = (uint32)(DSP_FRQ_CLR);
#elif defined (PLATFORM_SC6600L) || defined (PLATFORM_SC8800G)
    REG32 (GR_ICLR) |= BIT_1;
#endif
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif //End of file
