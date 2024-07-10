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
#include "cs_types.h"
#include "audp_debug.h"

#include "hal_error.h"
#include "hal_sys.h"
#include "adi_mst.h"

#include "adi_drv.h"
#include "global_macros.h"

#include "tra_si.hem"



/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status& ADI_MST_WFIFO_EMPTY )   
#define ADI_FIFO_IS_FULL    ( hwp_adiMst->arm_cmd_status&  ADI_MST_WFIFO_FULL ) 
#define ADI_READ_WIRTE_TIMOUT 10

#define ADI_MST_ARM_RD_DATA_MASK (0xFFFF)
#define ADI_MST_ARM_RD_ADDR_SHIFT(n)  (((n>>16)&0x7FFF))




/***************************************************************************** 
 *  Description:    this function is used to init analog to digital module.   * 
 *              it will enable adi_acc and soft reset adi_module,    *
 *              and then config the priority of each channel.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:                                                                   * 
******************************************************************************/
PUBLIC void ADI_Init(void)
{
        UINT32 adi_ctrl_val;
        UINT32 value;

        //AUD_TRACE(AUD_INFO_TRC, 0, "ADI_Init\n");

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
        /* value = 0x80020143; */
        value = ADI_MST_RF_GSSI_STRTBIT_MODE |
                            ADI_MST_RF_GSSI_DUMMY_LEN(2) |
                            ADI_MST_RF_GSSI_CLK_DIV(80) |
                            ADI_MST_RF_GSSI_NG_RX |
                            ADI_MST_RF_GSSI_NG_TX;
        hwp_adiMst->adi_cfg1 = value;


        /* set ADI clock auto gate */
        value = hwp_adiMst->adi_cfg0;
        value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;
        hwp_adiMst->adi_cfg0 = value;	

        //hal_TimDelay(20000);

}


PUBLIC void ADI_close(void)
{
}

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/***************************************************************************** 
 *  Description:    this function performs read operation to the analog die reg . * 
 *              it will disable all interrupt and polling the ready bit,  *
 *              and return a half-word value after read complete.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:   return register value                                               * 
******************************************************************************/
PUBLIC UINT16 ADI_REG_READ(UINT32 addr )
{
    uint32 adi_rd_data;
    uint32 rd_addr = 0;
    
    uint32 old_tick;
    uint32 new_tick;
    UINT32 status = hal_SysEnterCriticalSection();

    /*Purpose:Set read address*/
    hwp_adiMst->arm_rd_cmd = ADI_MST_ARM_RD_CMD(addr);
    old_tick = hal_TimGetUpTime ();
    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
        /*Purpose:Read data*/
        adi_rd_data =   hwp_adiMst->arm_rd_data;
        new_tick = hal_TimGetUpTime ();
        if ((new_tick - old_tick) >= ADI_READ_WIRTE_TIMOUT)
        {
            hal_SysExitCriticalSection(status);
            hal_HstSendEvent(0xffffff04); 
            return (uint16)(0xffff);
        }
    }    while(adi_rd_data & ADI_MST_ARM_RD_CMD_BUSY);

    rd_addr = ADI_MST_ARM_RD_ADDR_SHIFT(adi_rd_data);
    if(rd_addr != addr>>2)
    {
        adi_rd_data= 0xffff;
    }
    
    hal_SysExitCriticalSection(status);

    return((UINT16)(adi_rd_data & ADI_MST_ARM_RD_DATA_MASK));
}

/***************************************************************************** 
 *  Description:    this function performs write operation to the analog die reg .   * 
 *                      it will write the analog die register if the fifo is not full *
 *				It will polling the fifo full status til it is not full               * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                          * 
 *  Note:                                                                      * 
******************************************************************************/
PUBLIC void ADI_REG_WRITE(UINT32 addr, UINT32 data)
{
    uint32 old_tick;
    uint32 new_tick;
    UINT32 status = hal_SysEnterCriticalSection();
    UINT16 value =0;

    new_tick = old_tick = hal_TimGetUpTime ();

    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }

        //Timeout
        if ( (new_tick - old_tick) > 60)
        {
            AUD_ASSERT(0, ("ADI Write Timeout!"));/*assert to do*/
        }
        new_tick = hal_TimGetUpTime();
    }

    (* (VOLATILE UINT32 *)( REG_ADI_MST_BASE+0x8000+addr)) =ADI_MST_ADI_DAT_WR(data);

    new_tick = old_tick = hal_TimGetUpTime();
    // wait write fifo empty after adi analog write
    //wait until all data have been sent
    do {
        value = ADI_REG_READ(addr);

        if ( (new_tick - old_tick) > 60)
        {
            AUD_ASSERT(0, ("ADI Wait fifo empty Timeout!"));/*assert to do*/
        }
        new_tick = hal_TimGetUpTime();
        if((ADI_MST_ADI_DAT_WR(data)) != value)
        {
        TRACE_SI_1(TRA_SI,"SC2720:write again:0x%x\n",( REG_ADI_MST_BASE + 0x8000 +addr));
        (* (VOLATILE UINT32 *)( REG_ADI_MST_BASE + 0x8000 +addr)) =ADI_MST_ADI_DAT_WR(data);
        }
        else
        {
        break;
        }

    }while(1);

    hal_SysExitCriticalSection(status);

}

