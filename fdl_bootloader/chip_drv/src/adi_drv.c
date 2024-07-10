/******************************************************************************
 ** File Name:      adi_drv.c                                                 *
 ** Author:         tim.luo                                             *
 ** DATE:           2/25/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                  *
 **                                                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2/25/2010     Tim Luo      Create.                                        *
 **                                                                           *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adi_drv.h"
/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADI_FIFO_IS_EMPTY   ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_EMPTY_MASK )   
#define ADI_FIFO_IS_FULL    ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK ) 
#define ADI_READ_WIRTE_TIMOUT 10


#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)
#define REG32(x)                (*((volatile uint32 *)(x)))//Added by ankur
LOCAL void ADI_Reset( uint32 count)
{
    /*Purpose:reset ADI*/
    REG_WRITE(APB_RST0_SET, ADI_SOFT_RST_SET);
    
    while(count--);

    /*Purpose:reset ADI*/
    REG_WRITE(APB_RST0_CLR, ADI_SOFT_RST_CLR);          
}

/***************************************************************************** 
 *  Description:    this function is used to init analog to digital module.   * 
 *                      it will enable adi_acc and soft reset adi_module,    *
 *				and then config the priority of each channel.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:                                                                   * 
******************************************************************************/
PUBLIC void ADI_Init(void)
{   
	/*Purpose:enable ADI mode*/
	REG_WRITE(APB_EB0_SET, ADI_EB_SET);

	/*Purpose:reset ADI module*/
	ADI_Reset(100); //100 150

	/*Purpose:reset to "0" after chip is initialized to auto gate ADI_SCK to save power*/
	REG_AND(ADI_GSSI_CTRL0, ~ADI_GSSI_SCK_ALL_ON);
}

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/***************************************************************************** 
 *  Description:    this function performs read operation to the analog die reg . * 
 *                      it will disable all interrupt and polling the ready bit,  *
 *				and return a half-word value after read complete.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:   return register value                                               * 
******************************************************************************/
PUBLIC uint16 ADI_REG_READ(uint32 addr)
{
    uint32 adi_rd_data;
    uint32 old_tick;
    uint32 new_tick;

    /*Purpose:Set read address*/
    REG_WRITE(ADI_ARM_RD_CMD, ((addr)&ADI_ARMREG_ADDR_MASK) | ADI_ARMREG_FLAG_MASK);
    
    old_tick = fdl_GetTickCount ();
    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
      /*Purpose:Read data*/    
    	REG_READ(ADI_ARM_RD_DATA, adi_rd_data);
    	new_tick = fdl_GetTickCount ();
    	if ((new_tick - old_tick) >= ADI_READ_WIRTE_TIMOUT)
    	    return (uint16)(0xffff);
    }   while(adi_rd_data & ADI_RD_CMD_BUSY_MASK);

    return((uint16)(adi_rd_data & ADI_ARM_RD_DATA_MASK));
}

/***************************************************************************** 
 *  Description:    this function performs write operation to the analog die reg .   * 
 *                      it will write the analog die register if the fifo is not full *
 *				It will polling the fifo full status til it is not full               * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                          * 
 *  Note:                                                                      * 
******************************************************************************/
PUBLIC void ADI_REG_WRITE(uint32 addr, uint32 data)
{
    uint32 old_tick;
    uint32 new_tick;
    old_tick = fdl_GetTickCount ();
    while( ADI_FIFO_IS_FULL )
    {
        new_tick = fdl_GetTickCount ();
        if ((new_tick - old_tick) >= ADI_READ_WIRTE_TIMOUT)
            return;
    }

      /*Purpose:Write data*/    
	REG_WRITE(addr, data);
}

