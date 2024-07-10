/******************************************************************************
 ** File Name:      watchdog_phy_v5.c                                                 *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2010     Jie Dai            Create.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "watchdog_reg_v5.h"
#include "../watchdog_phy.h"
#include "adi_hal_internal.h"
#include "cmddef.h"
#if defined(PLATFORM_UWS6121E)
#define HW_RST_OFFSET           0
#define HW_RST_MASK       		0x1F
#define HW_BL_MASK         		BIT_5
#define HW_DSPEXCEPTION_MASK  	BIT_6
#define HW_USB_MASK       		BIT_7
#endif

#include "cmddef.h"
#if defined(PLATFORM_UWS6121E)
#define HW_RST_OFFSET           0
#define HW_RST_MASK       		0x1F
#define HW_BL_MASK         		BIT_5
#define HW_DSPEXCEPTION_MASK  	BIT_6
#define HW_USB_MASK       		BIT_7
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define ANA_WDG_LOAD_TIMEOUT_NUM    (10000)
#define ANA_WDG_CLR_INT_TIMEOUT_NUM (10000)
#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)
#define REG32(x)   (*((volatile uint32 *)(x)))

#if defined(PLATFORM_UWS6121E)
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)   
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL) 
#else
#define ADI_FIFO_IS_EMPTY   ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_EMPTY_MASK )   
#define ADI_FIFO_IS_FULL    ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK ) 
#endif

#pragma arm section rwdata = "CHIP_RESET_CODE"
volatile  uint32 reset_data[] =
{
    0xe59f1004,
    0xe5810000,
    0xe3a0f000,
    0x205000e0
};
#pragma arm section rwdata

LOCAL uint32 CHIP_PHY_GetHwRstAddr (void)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    return ANA_POR_RST_MONITOR;
#else
    return ANA_HW_RST2;
#endif
}

LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    //ANA_REG_AND (HW_RST_ADDR, ~HW_RST_MASK);
   // ANA_REG_OR (HW_RST_ADDR, (val&HW_RST_MASK));
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
   ANA_REG_AND (ANA_POR_RST_MONITOR, ~0x1F);
    ANA_REG_OR (ANA_POR_RST_MONITOR, (val&0x1F));
#else
   ANA_REG_AND (ANA_HW_RST2, ~0x1F);
    ANA_REG_OR (ANA_HW_RST2, (val&0x1F));
#endif
}

PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val)
{

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	ANA_REG_AND (ANA_POR_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_POR_RST_MONITOR, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_WDG_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_WDG_RST_MONITOR, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_POR_PIN_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_POR_PIN_RST_MONITOR, (0x1FFF&0xFFFF));
#else
    ANA_REG_AND (ANA_HW_RST0, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST0, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_HW_RST1, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST1, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_HW_RST2, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST2, (0x1FFF&0xFFFF));
#endif

    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HWRST1_REG_MSK);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), ((0x1FFF&0xFF) << HWRST1_REG_SHIFT));	
    CHIP_PHY_SetHwRstFlag (val);

}

PUBLIC uint32 SCI_GetTickCount(void)  // return the value of system count.
{
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
}

LOCAL void ADI_Reset( uint32 cycles)
{
    *(volatile uint32*)APB_RST0_SET = ADI_SOFT_RST_SET;
    
    while(cycles--){}
        
    *(volatile uint32*)APB_RST0_CLR = ADI_SOFT_RST_CLR;    
}

PUBLIC void ADI_init (void)
{
#if defined(PLATFORM_UWS6121E)
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
    
    /* value = 0x80020143; */   
    value = ADI_MST_RF_GSSI_STRTBIT_MODE |          
            ADI_MST_RF_GSSI_DUMMY_LEN(2) |          
            ADI_MST_RF_GSSI_CLK_DIV(1) |           
            ADI_MST_RF_GSSI_NG_RX |         
            ADI_MST_RF_GSSI_NG_TX;  
    hwp_adiMst->adi_cfg1 = value;
    
    /* set ADI clock auto gate */   
    value = hwp_adiMst->adi_cfg0; 
    value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;   
    hwp_adiMst->adi_cfg0 = value;
#else
    // volatile ADI_CFG_REG_T *adi_handle = (ADI_CFG_REG_T *) ADI_BASE_ADDR;

    //enable ADI_ACC to put the adi master to normal operation mode
    CHIP_REG_SET (APB_EB0_SET, ADI_EB_SET);

    //reset ADI module
    ADI_Reset(100);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    *(volatile uint32 *)ADI_GSSI_CTRL0 &= ~ADI_GSSI_SCK_ALL_ON;
    CHIP_REG_SET (ADI_CTL_REG, (0<<ARM_WR_PRI)|(0<<ARM_RD_PRI) |\
                                (0<<DSP_WR_PRI)|(0<<DSP_RD_PRI));

#else
    // reset to "0" after chip is initialized to auto gate ADI_SCK to save power
    *(volatile uint32*)ADI_CTL_REG &= ~ADI_CTL_SCK_EN;

    //config channel priority
    CHIP_REG_SET (ADI_CHANNEL_PRI,(0<<ARM_WR_PRI)|(0<<ARM_RD_PRI) |\
	                              (0<<DSP_WR_PRI)|(0<<DSP_RD_PRI) |\
	                              (0<<PD_WR_PRI)|(0<<BURST_ON_PRI)|(1<<FRM_INT_PRI)|\
	                              (1<<RF_WR_PRI)|(1<<FM_AGC_PRI)|(1<<FM_BAND_PRI) );
#endif
#endif
}
PUBLIC  void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{
#if defined(PLATFORM_UWS6121E)
    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)data;

    // wait write fifo empty after adi analog write
    while(ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
    }
#else
    uint32 cur_tick, next_tick;

    cur_tick = next_tick = SCI_GetTickCount();

    for(;;)              ////ADI_wait_fifo_empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    *(volatile uint32*)addr = (uint32)data;
#endif
}

PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr)
{
#if defined(PLATFORM_UWS6121E)
    uint32 adi_rd_data;
    uint32 rd_addr = 0;

    // Set read address
    hwp_adiMst->arm_rd_cmd = ADI_MST_ARM_RD_CMD(addr);

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
        adi_rd_data = hwp_adiMst->arm_rd_data;
    }
    while(adi_rd_data & ADI_MST_ARM_RD_CMD_BUSY);

    // rd_data high part should be the address of the last read operation
    rd_addr = (adi_rd_data >> 16) & 0x7FFF;
    if(rd_addr != addr >> 2)
    {
        rprintf("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr);/*assert verified*/
    }    

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
#else
    uint32 cur_tick, next_tick;
    uint32 adi_rd_data;
    // Set read address
    *(volatile uint32*)ADI_ARM_RD_CMD = ((addr)&ADI_ARMREG_ADDR_MASK) | ADI_ARMREG_FLAG_MASK;

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    cur_tick = next_tick = SCI_GetTickCount();
    do
    {
        adi_rd_data = *(volatile uint32*)ADI_ARM_RD_DATA;

        next_tick = SCI_GetTickCount();
    }
    while(adi_rd_data & ADI_RD_CMD_BUSY_MASK);

    return((uint16)(adi_rd_data & ADI_ARM_RD_DATA_MASK));
#endif
}

PUBLIC  void ADI_Analogdie_reg_and (uint32 addr, uint16 data)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif

    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(and) value==== */
    adi_tmp_val &= data;

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);

}

//extern int hwspin_lock_timeout(uint32 lock_id, uint32 to);

//extern void hwspin_unlock(uint32  lock_id);

PUBLIC  void ADI_Analogdie_reg_mask_or (uint32 addr, uint16 data, uint16 msk)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif
    /* ============read============== */
    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(mask or) value==== */
    adi_tmp_val &= (uint16)(~(msk)); \
    adi_tmp_val |= (uint16)((data)&(msk)); \

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);
}

PUBLIC  void ADI_Analogdie_reg_or (uint32 addr, uint16 data)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif
    /* ============read============== */
    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(or) value==== */
    adi_tmp_val |= data;

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);
}


#define WDG_LOAD_TIMER_VALUE(value) \
    do{\
        uint32   cnt          =  0;\
        ANA_REG_SET( WDG_LOAD_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_LOAD_LOW , (uint16)((value)  & 0xffff) );\
        while((ANA_REG_GET(WDG_INT_RAW) & wdg_ld_busy) && ( cnt < ANA_WDG_LOAD_TIMEOUT_NUM )) cnt++;\
    }while(0)

#define WDG_LOAD_IRQTIMER_VALUE(value) \
    do{\
        uint32   cnt          =  0;\
        ANA_REG_SET( WDG_IRQVALUE_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_IRQVALUE_LOW , (uint16)((value)  & 0xffff) );\
        while((ANA_REG_GET(WDG_INT_RAW) & wdg_ld_busy) && ( cnt < ANA_WDG_LOAD_TIMEOUT_NUM )) cnt++;\
    }while(0)

#define CLEAR_WDG_INT(msk) \
    do{ \
        uint32   cnt          =  0;\
        ANA_REG_SET(WDG_INT_CLR, (msk));\
        while((ANA_REG_GET(WDG_INT_RAW) & (msk))&&(cnt < ANA_WDG_CLR_INT_TIMEOUT_NUM)) cnt++; \
    }while(0)


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define  WDG_TRACE  SCI_TRACE_LOW

/*****************************************************************************/
//  Description:    This function config the watch dog module.
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_CONFIG (WDG_CONFIG_T *cfg)
{
    //uint32 ctrl = 0;
    //uint32 val  = 0;

    ///WDG_TRACE("Watch Dog Trace: Watch Dog Value 0x%8.8x", CHIP_REG_GET(WDG_VALUE));

    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);

    switch (cfg->mode)
    {
        case WDG_TIMEOUT_MODE_RESET:
            ANA_REG_OR (WDG_CTRL, (wdg_rst_en|wdg_irq_en));  // int before reset
            WDG_LOAD_IRQTIMER_VALUE(WDG_IRQ_COUNT);
            
            break;

        case WDG_TIMEOUT_MODE_INT:
            ANA_REG_AND (WDG_CTRL, ~(wdg_rst_en));  //int mode only
            ANA_REG_OR (WDG_CTRL, wdg_irq_en);

            WDG_LOAD_IRQTIMER_VALUE(WDG_IRQ_COUNT);

            break;

        default:
            break;  //No need to change
    }

    if (WDG_TIMER_STATE_STOP != cfg->state)
    {
        WDG_LOAD_TIMER_VALUE (cfg->val);
    }    

    switch (cfg->state)
    {
        case WDG_TIMER_STATE_STOP:
            ANA_REG_AND (WDG_CTRL, (~wdg_run));
            break;

        case WDG_TIMER_STATE_START:
            ANA_REG_OR (WDG_CTRL, wdg_run);
            break;

        default:
            break;  //No need to change
    }

    //WDG_TRACE:"Watch Dog Trace: Watch Dog Control 0x%8.8x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_PHY_V5_115_112_2_17_23_8_6_1492,(uint8*)"d", ANA_REG_GET (WDG_CTRL));
    ///    WDG_TRACE ("Watch Dog Trace: Watch Dog LOAD    0x%8.8x", CHIP_REG_GET (WDG_LOAD));

    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));

    return 0;
}

/*****************************************************************************/
//  Description:    This function clear the watch dog interrupt
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_INT_CLR (void)
{
    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);
    CLEAR_WDG_INT (wdg_int_clr);
    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));
    return 0;
}

PUBLIC  void CHIP_SOFT_Reset(void)
{
    uint32 i = 10000;
    uint16 value = 0;

#if defined(PLATFORM_UWS6121E)
    rprintf("Soft Reset MCU ...\n");
    ADI_init();
    CHIP_PHY_SetHwRstFlag (AUTO_TEST_MODE);
    do
    {
        if (AUTO_TEST_MODE == (ADI_Analogdie_reg_read(ANA_POR_RST_MONITOR) & 0x1F))
        {    
            break;
	  }
    }while(1);

	ANA_REG_OR(ANA_POR_7S_CTRL, BIT_9);
       //REG_ADI_CHANGE1(hwp_pmicPsm->por_timer_thd, psm_por_timer_thd, ext_rst_timer_thd, 0x3FF);
       ANA_REG_OR(0x700+0x78, 0x3FF);
       //halAdiBusWrite(&PMU_BOOT_MODE_REG, mode);
       ANA_REG_AND(ANA_POR_RST_MONITOR, 0);
       //swrst_ctrl0.b.reg_rst_en = 1;
       ANA_REG_OR(ANA_SWRST_CTRL0, (0x1 << 4));
       //swrst_ctrl0.b.reg_rst_pd_en = 1;
       ANA_REG_OR(ANA_SWRST_CTRL0, (0x1 << 8));
       //soft_rst_hw.b.reg_soft_rst_sw = 1;
       ANA_REG_OR(ANA_SOFT_RST_HW, 0x1);
       while(1);
#else
    ADI_init();
#if defined(PLATFORM_SC6531EFM)
	value = ADI_Analogdie_reg_read(ANA_MODULE_EN0_RTC);
	value |= ANA_WDG_EN;
    ADI_Analogdie_reg_write (ANA_MODULE_EN0_RTC,  value);

	value = ADI_Analogdie_reg_read(ANA_RTC_CLK_EN0_RTC);
	value |= ANA_RTC_WDG_EN;
    ADI_Analogdie_reg_write (ANA_RTC_CLK_EN0_RTC,  value);
#else
    ADI_Analogdie_reg_write (ANA_EB_SET0,  ANA_WDG_EB_SET);
    ADI_Analogdie_reg_write (ANA_EB_SET1,  ANA_RTC_WDG_EB_SET);
#endif
    CHIP_PHY_ResetHWFlag(0x12);//0x12=18 is AUTO_TEST_MODE
    WDG_TimerInit ();
	
    memcpy((uint8*)0x40002000, (uint8*)reset_data, 4*sizeof(reset_data)/sizeof(reset_data[0]));

    while (i--){}

    WDG_ResetMCU(); 
#endif

    {
        while ( 1)
        {

        }
    }

}

PUBLIC  void CHIP_PHY_PB1KeyHwPD(void)
{

    rprintf("Turn off power ...\n");
    ADI_init();

	ADI_Analogdie_reg_write (ANA_POWER_PD_HW, PWR_OFF_SEQ_EN);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif

/*  End Of File */


