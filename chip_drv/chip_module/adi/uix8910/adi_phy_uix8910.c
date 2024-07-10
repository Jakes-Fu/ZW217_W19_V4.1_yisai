/******************************************************************************
 ** File Name:      adi_phy_v7.c                                              *
 ** Author:         tim.luo                                                   *
 ** DATE:           2/25/2010                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                 *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2/25/2010     Tim Luo      Create.                                   *
 **                                                                      *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "task_monitor.h"
#include "adi_hal_internal.h"
#include "adi_reg_uix8910.h"
#include "timer_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)   
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL) 

#define ADI_MST_ARM_RD_DATA_MASK (0xFFFF)
#define ADI_MST_ARM_RD_ADDR_SHIFT(n)  (((n>>16)&0x7FFF))

#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)

#define ANA_REG_ADDR_START      (ANA_REG_BASE)
#define ANA_REG_ADDR_END        (ANA_REG_BASE + 0x1FFF)

#define API_CLK_ADI_78M  0
#define API_CLK_ADI_62M4 1
LOCAL DEVICE_HANDLE s_dev_adi = SCI_NULL; 

#define TIMER_GET_USTICK()      REG32(0x0880E01C)

LOCAL void ADI_SetClk( uint32 clksrc_sel)
{
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    // 0:78M,  1:62.4M 
    if(clksrc_sel == API_CLK_ADI_62M4)
	    *(volatile uint32 *)APB_CLK_CFG1 |= CLK_ADI_SEL_MCU;
	else
	    *(volatile uint32 *)APB_CLK_CFG1 &= ~CLK_ADI_SEL_MCU;
#endif
}

PUBLIC void ADI_Phy_init (void)
{
    uint32 value;
        
    /* initialize the adi registers */
    hwp_adiMst->adi_ctrl = 0;    

    /* set the channel priority */
    hwp_adiMst->adi_pril = ADI_MST_CHNL0_PRI(0)|ADI_MST_CHNL1_PRI(1);

    /* get the readback addr mask */
   
    /* value = 0x7490835d; */   
    value = ADI_MST_RF_GSSI_FAST_MODE | 
            ADI_MST_RF_GSSI_DUMMY_CLK_EN | 
            ADI_MST_RF_GSSI_OE_CFG | 
            ADI_MST_RF_GSSI_SYNC_MODE |         
            ADI_MST_RF_GSSI_WR_POS(16) |            
            ADI_MST_RF_GSSI_DATA_LEN(16) |          
            ADI_MST_RF_GSSI_CMD_LEN(12+1) |         
            ADI_MST_RF_GSSI_FRAME_LEN(12+1+16) |            
            ADI_MST_RF_GSSI_SCK_ALL_ON; 
    hwp_adiMst->adi_cfg0 = value;
    
    /* value = 0x80040143; ADI_MST_RF_GSSI_DUMMY_LEN need set to 4 */   
    value = ADI_MST_RF_GSSI_STRTBIT_MODE |          
            ADI_MST_RF_GSSI_DUMMY_LEN(4) |          
            ADI_MST_RF_GSSI_CLK_DIV(1) |           
            ADI_MST_RF_GSSI_NG_RX |         
            ADI_MST_RF_GSSI_NG_TX;  
    hwp_adiMst->adi_cfg1 = value;
    
    /* set ADI clock auto gate */   
    value = hwp_adiMst->adi_cfg0; 
    value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;   
    hwp_adiMst->adi_cfg0 = value;

}
/*****************************************************************************
 *  Description:    this function is used to init analog to digital module.   *
 *                      it will enable adi_acc and soft reset adi_module,        *
 *              and then config the priority of each channel.             *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:                                                                                     *
******************************************************************************/
PUBLIC void ADI_init (void)
{
    uint32 value;
    
    //enable ADI_ACC to put the adi master to normal operation mode
    SCI_ASSERT (DEVICE_Find ("ADI", &s_dev_adi) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_adi);    

    //reset ADI module
    DEVICE_Reset(s_dev_adi);  

    //AUD_TRACE(AUD_INFO_TRC, 0, "ADI_Init\n");
    ADI_Phy_init();
}

/*****************************************************************************
 *  Description:    ADI_IS_Analogdie_reg                                     *
 *  Global Resource Dependence:                                              *
 *  Author: Mingwei Zhang                                                    *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC BOOLEAN ADI_IS_Analogdie_reg (uint32 addr)
{
    if ( (addr >= ANA_REG_ADDR_START) && (addr <= ANA_REG_ADDR_END))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/***************************************************************************** 
 *  Description:    this function performs read operation to the analog die reg . * 
 *                      it will disable all interrupt and polling the ready bit,  *
 *				and return a half-word value after read complete.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:   return register value                                               * 
******************************************************************************/
PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr)
{
    uint32 cur_tick, next_tick;
    uint32 adi_rd_data;
    uint32 rd_addr = 0;
    int32 ret = 0;
    uint32 v_curTick, v_endTick;
#if defined(FPGA_VERIFICATION) 
	return 0;
#endif
    uint32 time1_value;
    uint32 time2_value;
    uint32 time3_value;
    uint32 time4_value;    
    
    time1_value = TIMER_GET_USTICK();
    if (SCI_FALSE == ADI_IS_Analogdie_reg(addr)	)/*lint !e527*/
    {
        SCI_PASSERT(0, ("ADI_Analogdie_reg_read, addr = 0x%08x is error!", addr));/*assert to do*/		
    }
    
    v_endTick = (10 * 16384) / 1000;
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    //get the current counter,
    v_curTick = timer_TimRealTickGet(OS_TIMER);
    do
    {
            // Try to take the hwspinlock
            if (0x0 == hwspin_lock_get(HWSPINLOCK_ADI)) 
            {
                break;            
            }
            SCI_RestoreFIQ();
            SCI_RestoreIRQ();
            //SCI_Sleep(1);
            //The lock is already taken, let's check if the user wants us to try again
            if((uint32)((timer_TimRealTickGet(OS_TIMER) + (uint64)(0xFFFFFFFF) - v_curTick) % 0xFFFFFFFF) > v_endTick)
            {
                SCI_PASSERT(0, ("ADI Read get spinlock Timeout!"));/*assert to do*/
            }
            else
            {
                SCI_DisableIRQ();
                SCI_DisableFIQ();
            }
    }while(1);

    // wait write fifo empty before adi analog read
    cur_tick = next_tick = SCI_GetTickCount();
    time2_value = TIMER_GET_USTICK();

    // Set read address
    hwp_adiMst->arm_rd_cmd = ADI_MST_ARM_RD_CMD(addr);

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    cur_tick = next_tick;
    do
    {
        adi_rd_data = hwp_adiMst->arm_rd_data;

        //Timeout
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170002, (next_tick - cur_tick));
            SCI_TRACE_LOW("ADI read rd_data=0x%x,next_tick=0x%x,cur_tick=0x%x",adi_rd_data,next_tick,cur_tick);
            //Removed by Qin.zhou, temply removed
            SCI_PASSERT(0, ("ADI Read Timeout!"));/*assert to do*/
        }

        next_tick = SCI_GetTickCount();
    }
    while(adi_rd_data & ADI_MST_ARM_RD_CMD_BUSY);

    time3_value = TIMER_GET_USTICK();
    
    // rd_data high part should be the address of the last read operation
    rd_addr = ADI_MST_ARM_RD_ADDR_SHIFT(adi_rd_data);
    if(rd_addr != addr >> 2)
    {
        SCI_PASSERT(0, ("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr));/*assert verified*/
    }    
	
    hwspin_unlock(HWSPINLOCK_ADI);
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    time4_value = TIMER_GET_USTICK();
    if (time4_value - time1_value > 100)   /*50us*/
    {
        SCI_TRACE_LOW("adi read addr=%x,t1=%x,t2=%X,t3=%x,t4=%x\n",addr,time1_value,time2_value,time3_value,time4_value);
    }
    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
}


/***************************************************************************** 
 *  Description:    this function performs write operation to the analog die reg .   * 
 *                      it will write the analog die register if the fifo is not full *
 *				It will polling the fifo full status til it is not full               * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:                                                                      * 
******************************************************************************/
PUBLIC  void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{
    uint32 cur_tick, next_tick;
    int32 ret = 0;
    uint32 v_curTick, v_endTick;
    uint32 time1_value;
    uint32 time2_value;
    uint32 time3_value;
    uint32 time4_value; 

    
#if defined(FPGA_VERIFICATION) 
	return ;
#endif
    time1_value = TIMER_GET_USTICK();

    v_endTick = (10 * 16384) / 1000;
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    //get the current counter,
    v_curTick = timer_TimRealTickGet(OS_TIMER);
    do
    {
            // Try to take the hwspinlock
            if (0x0 == hwspin_lock_get(HWSPINLOCK_ADI)) 
            {
                break;            
            }
            SCI_RestoreFIQ();
            SCI_RestoreIRQ();
            //SCI_Sleep(1);
            //The lock is already taken, let's check if the user wants us to try again
            if((uint32)((timer_TimRealTickGet(OS_TIMER) + (uint64)(0xFFFFFFFF) - v_curTick) % 0xFFFFFFFF) > v_endTick)
            {
                SCI_PASSERT(0, ("ADI write get spinlock Timeout!"));/*assert to do*/
            }
            else
            {
                SCI_DisableIRQ();
                SCI_DisableFIQ();
            }
    }while(1);

    cur_tick = next_tick = SCI_GetTickCount();

    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }

        //Timeout
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170003, (next_tick - cur_tick));
            SCI_PASSERT(0, ("ADI Write Timeout!"));/*assert to do*/
        }
        next_tick = SCI_GetTickCount();
    }

    time2_value = TIMER_GET_USTICK();
    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)data;
    time3_value = TIMER_GET_USTICK();
	
    hwspin_unlock(HWSPINLOCK_ADI);
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();	
    time4_value = TIMER_GET_USTICK();
	
	//SCI_TRACE_LOW("adiwrite addr=%x,write value=%x\n",(REG_ADI_MST_BASE + 0x8000 + addr) ,data);

    if (time4_value - time1_value > 100)   /*50us*/
    {
        SCI_TRACE_LOW("adi write addr=%x,t1=%x,t2=%X,t3=%x,t4=%x\n",addr,time1_value,time2_value,time3_value,time4_value);
    }
}

