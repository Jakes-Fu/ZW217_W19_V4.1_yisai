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

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADI_FIFO_IS_EMPTY   ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_EMPTY_MASK )   
#define ADI_FIFO_IS_FULL    ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK ) 


#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)


#define API_CLK_ADI_78M  0
#define API_CLK_ADI_62M4 1
LOCAL DEVICE_HANDLE s_dev_adi = SCI_NULL; 
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
    //enable ADI_ACC to put the adi master to normal operation mode
    SCI_ASSERT (DEVICE_Find ("ADI", &s_dev_adi) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_adi);    

    //reset ADI module
    DEVICE_Reset(s_dev_adi);  

    // reset to "0" after chip is initialized to auto gate ADI_SCK to save power
    *(volatile uint32 *)ADI_GSSI_CTRL0 &= ~ADI_GSSI_SCK_ALL_ON;

    //config channel priority, arm read, arm write, dsp read, dsp write
    CHIP_REG_SET (ADI_CTL_REG, (0<<ARM_WR_PRI)|(0<<ARM_RD_PRI) |\
                                (0<<DSP_WR_PRI)|(0<<DSP_RD_PRI));
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
#if defined(FPGA_VERIFICATION) 
	return 0;
#endif
    if (SCI_FALSE == ADI_IS_Analogdie_reg(addr)	)/*lint !e527*/
    {
        SCI_PASSERT(0, ("ADI_Analogdie_reg_read, addr = 0x%08x is error!", addr));/*assert to do*/		
    }
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    // wait write fifo empty before adi analog read
    cur_tick = next_tick = SCI_GetTickCount();
    while(ADI_FIFO_IS_EMPTY == 0)
    {
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170004, (next_tick - cur_tick));
            SCI_PASSERT(0, ("ADI Wait fifo empty Timeout!"));/*assert to do*/
        }
        next_tick = SCI_GetTickCount();
    }

    // Set read address
    REG32(ADI_ARM_RD_CMD) = ((addr)&ADI_ARMREG_ADDR_MASK) | ADI_ARMREG_FLAG_MASK;
    
    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    cur_tick = next_tick;
    do
    {
        adi_rd_data = REG32(ADI_ARM_RD_DATA);

        //Timeout
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170002, (next_tick - cur_tick));
            SCI_PASSERT(0, ("ADI Read Timeout!"));/*assert to do*/
        }

        next_tick = SCI_GetTickCount();
    }
    while(adi_rd_data & ADI_RD_CMD_BUSY_MASK);
    
    // rd_data high part should be the address of the last read operation
    if ((adi_rd_data & ADI_ARM_RD_ADDR_MASK) != ((addr & ADI_ARMREG_ADDR_MASK)<<16))
    {
        SCI_PASSERT(0, ("high 13bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr));/*assert verified*/
    }

    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    return((uint16)(adi_rd_data & ADI_ARM_RD_DATA_MASK));
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
#if defined(FPGA_VERIFICATION) 
	return ;
#endif

    SCI_DisableIRQ();/*lint !e527*/
    SCI_DisableFIQ();

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

    REG32(addr) = (uint32)data;

    // wait write fifo empty after adi analog write
    cur_tick = next_tick = SCI_GetTickCount();
    while(ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170005, (next_tick - cur_tick));
            SCI_PASSERT(0, ("ADI Wait fifo empty Timeout!"));/*assert to do*/
        }
        next_tick = SCI_GetTickCount();
    }

    SCI_RestoreFIQ();
    SCI_RestoreIRQ();	
}

