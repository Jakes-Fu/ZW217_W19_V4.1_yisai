/******************************************************************************
 ** File Name:      chip_phy_v3.c                                             *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/14/2002     Richard.Yang     Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                             *
 ** 08/23/2004     JImmy.Jia        Modify for SC6600D                        *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "os_param.h"
#include "chip_plf_export.h"
#include "sci_api.h"
#include "fiq_drvapi.h"
#include "busmonitor_drvapi.h"
#include "gpio_drvapi.h"
#include "mcu_reboot.h"
#include "adi_hal_internal.h"
#include "pinmap_drvapi.h"
#include "dal_lcd.h"
#include "mmu_drvapi.h"
#include "dma_drv_internal.h"
#include "wdg_drvapi.h"
#include "ldo_drvapi.h"
#include "adc_drvapi.h"
#include "clock_drvapi.h"
#include "ref_outport.h"
//#include "pwm_drvapi.h"
#include "boot_drvapi.h"
#include "chip_drvapi.h"
#include "gpio_prod_api.h"
#include "chg_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/
#define HW_RST_MASK       0x1F
#define HW_BL_MASK         BIT_5
#define HW_DSPEXCEPTION_MASK  BIT_6
#define HW_USB_MASK       BIT_7

#define HWRST_ADDR     DMA_CHx_SRC_ADDR(31)

#define ENABLE_DMA_MODULE \
    DRV_MACRO_START \
    if(!(CHIP_REG_GET(AHB_CTL0) & BIT_6)) \
    { \
        CHIP_REG_OR(AHB_CTL0, BIT_6);    \
    } \
    DRV_MACRO_END;

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

typedef enum
{
    CORE_VOLTAGE_1800MV = 0,
    CORE_VOLTAGE_1700MV,
    CORE_VOLTAGE_1600MV,
    CORE_VOLTAGE_1500MV
} CORE_VOLTAGE_E;

#if 0
typedef struct
{
    volatile uint32  TPWrite_idx;
    volatile uint32  TPRead_idx;
    volatile uint32  TPBuffBaseAddr;
    volatile uint32  TPBuffSize;
    volatile uint32  memDataWrite_idx;
    volatile uint32  memDataRead_idx;
    volatile uint32  memDataBuffBaseAddr;
    volatile uint32  memDataBuffSize;
    volatile uint32  dumpDataWrite_idx;
    volatile uint32  dumpDataRead_idx;
    volatile uint32  dumpDataBuffBaseAddr;
    volatile uint32  dumpDataBuffSize;
    volatile uint32  statusFlag;    /*bit 0:TP Force Flush   bit 1:MEM force flush    bit 7: DSP Assert Busy*/
    volatile uint32  dspNvSet;    /*DSP log set   0:disable   1:Uart1     2:from ARM COM_Debug*/
    volatile uint32  reserve[6];

} SIO_DSP_LOG_INFO_S;
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 chip_id = 0;
LOCAL CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;


LOCAL volatile uint32 BackReg;
LOCAL volatile uint32 g_UART1_log_en = SCI_TRUE;

extern void L1_Assert_Dsp (void);
extern void dsp_soft_reset (uint16  flag);
extern void DL_reset_tddsp (void);
//extern void CHNG_FREQ_ValidateArmClk (uint32 arm_clk_level);

const CHIP_INFO chip_info[] =
{
    {SC8800G1,             "SC8800G1\n"},
    {SC8800G2,             "SC8800G2\n"},
    {SC8801G2,             "SC8801G2\n"},
    {SC8802G2,             "SC8802G2\n"},
    {SC6810,               "SC6810\n"}
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP wake up
//  Author:         Jeff.li
//  Note:           Open UART1
/*****************************************************************************/
PUBLIC void CHIP_DSPUART1En (void)
{

}

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP sleep
//  Author:         Jeff.li
//  Note:           Close RFLDO/TD CAL/UART
/*****************************************************************************/
PUBLIC void CHIP_DSPSleep (void)
{
}

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP wake up
//  Author:         Jeff.li
//  Note:           Open RFLDO/TD CAL/UART
/*****************************************************************************/
PUBLIC void CHIP_DSPWakeUp (void)
{
}

/*****************************************************************************/
//  Description:    The function is used to init TD clock
//  Global resource dependence:
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_CEVAClk_Init (uint8 dsp_clk_sel)
{
}

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void)
{
    uint32 id =  * (uint32 *) (CHIP_TYPE);

    chip_id = id;

    switch (chip_id)
    {
        case 0x88000000:
            chip_type = SC8800G1;
            break;
        case 0x88000002:
            chip_type = (0 == (REG32 (GR_GEN3) & 0x3)) ?
                        (SC8800G2) :                     // 8800G2
                        ( (1 == (REG32 (GR_GEN3) & 0x3)) ?
                          (SC8801G2) :                     // 8801G2
                        ( (2 == (REG32 (GR_GEN3) & 0x3)) ?  
                          (SC8802G2):                     // 8802G2
                          (SC6810)));                     // SC6810
            break;
        default:
            break;
    }

    return;
}


/*****************************************************************************/
//  Description:    The function is used to detect the memory type
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Return value:
//                1      CS0 connects to SDRAM
//                0      CS0 connects to FLASH
/*****************************************************************************/
PUBLIC uint32 CHIP_DetectMemType (void)
{
    return TRUE;
}
/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void)   // return chip ID number.
{
    return chip_id;
}

/*****************************************************************************/
//  Description:    The function change the core voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_ChangeCoreVoltage (CORE_VOLTAGE_E core_voltage)
{

}

LOCAL uint32 CHIP_GetMcuSelClk()
{

    volatile uint32 clk_mcu_sel = REG32 (AHB_ARM_CLK);
    uint32 clk_mcu = 0;
    clk_mcu_sel = (clk_mcu_sel >> 23) & 0x3;

    switch (clk_mcu_sel)
    {
        case 0:
            clk_mcu = (0 == (REG32 (GR_GEN3) & 0x3)) ?
                      (CHIP_GetMPllClk()) :                     // 8800G2
                      ( (1 == (REG32 (GR_GEN3) & 0x3)) ?
                        (256000000) :                             // 8801G1
                        (192000000));                             // 8802G1
            break;
        case 1:
            clk_mcu = 153600000;
            break;
        case 2:
            clk_mcu = 64000000;
            break;
        case 3:
            clk_mcu = 26000000;
            break;
        default:
            // can't go there
            break;
    }

    return clk_mcu;
}
/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
    volatile uint32 arm_div = REG32 (AHB_ARM_CLK);
#ifdef FPGA_VERIFICATION
    return 36000000;
#else
    arm_div = (arm_div) & 7;
    arm_div = 1;

    if (REG32 (AHB_ARM_CLK) &BIT_30)
    {
        arm_div = arm_div << 1;
    }

    return (CHIP_GetMcuSelClk() / arm_div);
#endif
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    volatile uint32 ahb_div = REG32 (AHB_ARM_CLK);
#ifdef FPGA_VERIFICATION
    return 36000000;
#else
    ahb_div = (ahb_div >> 4) & 7;
    ahb_div = ahb_div + 1;

    if (REG32 (AHB_ARM_CLK) &BIT_30)
    {
        ahb_div = ahb_div << 1;
    }

    if (REG32 (AHB_ARM_CLK) &BIT_31)
    {
        ahb_div=ahb_div<<1;
    }

    return (CHIP_GetMcuSelClk() / ahb_div);
#endif
}


/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void)
{
    return ARM_CLK_26M;
}

/*****************************************************************************/
// Description :    This function is used to get current DSP clock.
// Global resource dependence :
// Author :         Benjamin.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetDspClk (void)
{
    return 0;
}

/*****************************************************************************/
// Description :    This function is used to disable all fiqs on sc6600d
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           every bit indicates one fiq src
/*****************************************************************************/
void CHIP_DisableFIQ (void)
{
    CHIPDRV_DisableFIQINTALL();
}

/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetPllClk (void)
{
    return CHIP_GetMPllClk();
}




uint32 CHIP_GetMPllClk (void)
{
    return (
               CHIP_GetAPBClk()
               / ( ( (* (volatile uint32 *) GR_MPLL_MN) & 0x003F0000) >>16) \
               * ( (* (volatile uint32 *) GR_MPLL_MN) & 0x00000FFF));
}

LOCAL void CHIP_SetMPllClk (uint32 mpll_clk)
{
    uint32 save_ahb_arm_clk;
    uint32 temp;

    SCI_PASSERT ( (380000000 < mpll_clk) && (mpll_clk < 410000000) && (! (mpll_clk%2000000)), /*assert verified*/
                  ("mpll_clk error:%d!",mpll_clk));

    mpll_clk /= 1000000;

    //reduce MCU CLK for systom stable(G2 192M)
    save_ahb_arm_clk = CHIP_REG_GET (AHB_ARM_CLK);
    temp = save_ahb_arm_clk & (~CLK_MCU_SEL_MSK);
    temp |= (2 << CLK_MCU_SEL_SHIFT);
    CHIP_REG_SET (AHB_ARM_CLK,temp);

    //MPLL_CTRL register write enable
    CHIP_REG_OR (GR_GEN1, GEN1_MPLL_MN_EN);

    //set mpll_clk
    temp = CHIP_REG_GET (GR_MPLL_MN);
    temp &= ~ (GR_MPLL_MN_M_MSK | GR_MPLL_MN_N_MSK);
    temp |= (13 << GR_MPLL_MN_M_SHIFT) | (mpll_clk >> 1);
    CHIP_REG_SET (GR_MPLL_MN,temp);

    //MPLL_CTRL register write disable
    CHIP_REG_AND (GR_GEN1, ~GEN1_MPLL_MN_EN);

    //resume
    CHIP_REG_SET (AHB_ARM_CLK,save_ahb_arm_clk);
}

/*****************************************************************************/
// Description :    This function is used to Set Mpll CLK when net switch.
// net_mode:0-2G,1-3G
// Author :         Mingwei.Zhang
// Note :
/*****************************************************************************/
void CHIP_ClkForNetSwitch (uint32 net_mode)
{
	/*
		Modify the function for SC6530 200M Arm clk test based on SC8800G2 
	openphone platform. Remove the function.
	*/
	#if 0
    if (net_mode == 0)  //2G
    {
        CHIP_SetMPllClk (ARM_CLK_400M);
    }
    else if (net_mode == 1) //3G
    {
        CHIP_SetMPllClk (ARM_CLK_408M);
    }
    else
    {
        SCI_ASSERT (0); /*assert verified*/
    }
	#endif
}

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Daniel.ding add it for SC6800 .
/*****************************************************************************/
uint32 CHIP_GetVPllClk (void)
{
    return NULL;
}

/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_TurnOffPower (void)
{
#ifndef FPGA_VERIFICATION

    // Disable FIQ
    CHIPDRV_DisableFIQINTALL();
    // Disable IRQ
    CHIPDRV_DisableIRQINTALL();

    // Close All LDO.
    LDO_TurnOffAllLDO();

    for (;;) {}

#endif
}


/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void)  //reset interrupt disable??
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    while (i--) {};

    /* disable bus monitor if any */
    if (CHIP_DetectMemType())
    {
        /* disable bus monitor if any */
        bm_disable();
    }

    // Send Fiq to DSP to wake up scheduler
    // Especially when charger is connected
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;
    * (volatile uint32 *) (GR_IRQ) |= 0x8;
    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }
    // Hold dsp
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);

    // Indicate that DSP is resetted.
    * (volatile uint16 *) 0x50000FFE = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();
    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    WDG_ResetMCU();
    OS_TickDelay (100);

    * (volatile uint32 *) GR_ICLR |= (BIT_1|BIT_0 | BIT_2 | BIT_3);

    if (CHIP_DetectMemType())
    {
        mcu_nand_reboot();
    }
    else
    {
        EMC_Close();
        __tx_reset();   // Jump to 0
    }

}

/*****************************************************************************/
// Description :    This function is used to reset MCU from boot code.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_BootReset (void)
{

    /* Disable bus monitor */

    if (CHIP_DetectMemType())
    {
        /* Disable bus monitor */
        bm_disable();
    }

    // Hold dsp
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);

    // Indicate that DSP is resetted.
    * (volatile uint16 *) 0x50000FFE = 0;


    //Disable all fiq src.
    CHIP_DisableFIQ ();


    //Clear FIQ and IRQ from DSP
    * (volatile uint32 *) GR_ICLR |= (BIT_1|BIT_0 | BIT_2 | BIT_3);


    if (!CHIP_DetectMemType())
    {
        EMC_Close();
    }

    mcu_uart_reboot();

}

/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void)
{
    MMU_Init_Phase1();
    // Init Pin function and GPIOs setting
    PM_Init();
    ADI_init();

    // Init system clock
    CLOCK_Config();

    //provide some CLK for analog die chip id register reset
    ANA_REG_SET (ANA_ADIE_CHIP_ID,0);

    // headdetect pd control
    ANA_REG_AND (ANA_LED_CTL, ~BIT_15);
}

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
    return chip_type;
}

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetChipInfo (void)
{
    uint8 i;
    const char *info = NULL;

    SCI_ASSERT (NULL != chip_info);/*assert verified*/

    for (i=0; i<sizeof (chip_info) /sizeof (CHIP_INFO); i++)
    {
        if (chip_info[i].type == chip_type)
        {
            info = (const char *) chip_info[i].info;
            break;
        }
    }

    SCI_ASSERT (info != NULL);/*assert verified*/

    return info;
}

/*****************************************************************************/
// Description :    This function is active/deactive share-memory clock
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHIP_ActiveSMCLK (BOOLEAN is_active)
{
}

/*****************************************************************************/
// Description :    This function is Get Usb DM Value
// Global resource dependence : none
// Author : Mingwei.Zhang
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void)
{
    return 0; //dummy
}


/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_BeforeAssert (void)
{
    uint8 assert_val;
    //in other platform, deal relative case here ...

    OSPARAM_GetAssertMode (&assert_val);

    if (SCI_ASSERT_DEBUG_MODE == assert_val)
    {
        // If audio device is being controlled by DSP now, switch it back to ARM.
        VB_SWTICH_ARM_CTL (SCI_TRUE);

        // Shut down the speaker PA.
        GPIO_EnableAmplifier (SCI_FALSE);
    }
}


/*below 3 functions are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void)
{
    BOOLEAN is_dsp_respond = SCI_FALSE;

    // reset GSM DSP
    L1_Assert_Dsp();

    // RESET TD DSP
    dsp_soft_reset (1);

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 200)
        {
            tick2 = SCI_GetTickCount();
        }
    }
    return is_dsp_respond;
}

// The function is used to reset and set dsp to output dsp assert log to pc by uart1
PUBLIC void CHIP_DspOutPutAssertInfo (void)
{
    DL_reset_tddsp();//reset dsp
}
// The function is used to get the IRAM/SHARE-MEMORY buf to store assert information
// in the assert mode.
// The buffer size must be larger than size(IRAM_buf) structure
PUBLIC void *CHIP_GetAssertInfoBufBase (void)
{
    uint32 addr_base = 0xffffffff;
    addr_base = SHARE_MEM_BEGIN;
    return (void *) addr_base;
}

/******************************************************************************/
// Description:   Enable MCU can access switched-ram and shared-ram.
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:          Beforce invoking this function, the DSP/ISP should be hold on!
/******************************************************************************/
PUBLIC uint32 Chip_SwitchIRAM2Arm (void)
{
    REG32 (AHB_CTL1) |= BIT_0; //DCAM_BUF_SW
    return 0;
}
/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note :          Because there is no register which can restore information
//                  when watchdog resets the system, so we choose IRAM.
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstAddr (void)
{
    ENABLE_DMA_MODULE

    return HWRST_ADDR;
}

/*****************************************************************************/
//  Description:   This fuction return the reset flag of HWRST reg
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_RST_MASK);
}

/*****************************************************************************/
//  Description:   This fuction return the backlight flag of HWRST reg
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwBLFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_BL_MASK);
}

/*****************************************************************************/
//  Description:    This fuction return the dsp exception flag of HWRST reg
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwDSPExceptionFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_DSPEXCEPTION_MASK);
}

/*****************************************************************************/
//  Description:    This fuction return the usb flag of HWRST reg
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwUSBFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_USB_MASK);
}

/*****************************************************************************/
//  Description:   This fuction set the reset flag of HWRST reg
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:           The valid bit filed is from bit4 to bit0
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr(), ~HW_RST_MASK);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr(), val & HW_RST_MASK);
}

/*****************************************************************************/
//  Description:  This fuction set the backlight flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwBLFlag (BOOLEAN val)
{
    if (val)
    {
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_BL_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_BL_MASK);
    }
}

/*****************************************************************************/
//  Description:   This fuction set the dsp exception flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwDSPExceptionFlag (BOOLEAN val)
{
    if (val)
    {
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_DSPEXCEPTION_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_DSPEXCEPTION_MASK);
    }
}

/*****************************************************************************/
//  Description:   This fuction set the usb flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwUSBFlag (BOOLEAN val)
{
    if (val)
    {
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_USB_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_USB_MASK);
    }
}

/*****************************************************************************/
//  Description:    Gets the register in analog die to judge the reset mode.
//  Author:         Jeff.Li
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetANAReg (void)
{
    uint32 adi_rd_data;

    // Set read command
    * (volatile uint32 *) ADI_ARM_RD_CMD = ANA_HWRST_STATUS;

    // Wait read operation complete, RD_data[31] will be cleared after the read
    // operation complete
    do
    {
        adi_rd_data = * (volatile uint32 *) ADI_RD_DATA;
    }
    while (adi_rd_data & BIT_31);

    return ( (adi_rd_data & 0x0000FFFF));
}

/*****************************************************************************/
//  Description:    Sets the register in analog die.
//  Author:         Yong.Li
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
LOCAL void CHIP_PHY_SetANAReg (uint32 value)
{
    for (;;)             ////ADI_wait_fifo_empty
    {
        if ( ( (CHIP_REG_GET (ADI_FIFO_STS) & ( (uint32) ADI_FIFO_EMPTY)) != 0))
        {
            break;
        }
    }

    CHIP_REG_SET (ANA_HWRST_STATUS, value);
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_ResetHWFlag. It resets the HW
//                  reset register after system initialization.
//  Author:         Jeff.Li
//  Note:           The valid bit filed of analog register is from bit11 to bit0.
//                  | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
//
//                  The valid bit filed of HW_RST is from bit11 to bit0.
/*****************************************************************************/
PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val)
{
    //set WDG Mon to 0xF
    ANA_REG_AND (ANA_HWRST_STATUS, ~0xF);
    ANA_REG_OR (ANA_HWRST_STATUS, (val&0xF));

    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr(), ~HW_RST_MASK);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr(), (val & HW_RST_MASK));
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule ()
{
    // Only open I-Cache for __main and kernel image coping operation
    MMU_EnableICache();
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:           Do nothing. There are 32KB internal ram dedicated for ARM.
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    if ( (CHIP_PHY_GetHwRstFlag() & HW_RST_MASK) == (flag & HW_RST_MASK))
    {
        return SCI_FALSE;
    }
    else
    {
        return SCI_TRUE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by power
//                  up.
// Author :         Jeff.Li
// Note :           | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByPowerUp()
{
    if ( (CHIP_PHY_GetANAReg () & 0xF0) == 0x0)
    {
        //HWRST reg is 0 when Power On, and to be 1 in other cases
        CHIP_PHY_SetANAReg (CHIP_PHY_GetANAReg() | (1 << 4));

        //clear reg in dma chn
        CHIP_REG_SET (CHIP_PHY_GetHwRstAddr(), 0);

        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by watch-
//                  dog reset.
// Author :         Jeff.Li
// Note :           | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByWatchDog()
{
    if ( (CHIP_PHY_GetANAReg () & 0xF) == 0x0)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
// Description :    This function initializes the modules before assert while
//                  the system has just been reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIP_PHY_WDGAssertInit ()
{
    CLOCK_Config();
    PM_Init();
    DMA_HAL_Init ();
    GPIO_Init();
    ADC_Init();
}

//******************************************************************************
//  Description:  DSP_Reset
//  Global resource dependence:  reset dsp system
//  Author:  andy.chen
//  para:  void
//  Note:  this function should be put in dsp relative file later,
//         so name this function as "dsp_reset" but not "chip_phy_dspreset"
//******************************************************************************
PUBLIC void DSP_Reset (void)
{
    SCI_DisableIRQ();
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);
    SCI_RestoreIRQ();
    CHIP_REG_SET (DSP_RST, 1);

    SCI_SLEEP (2);   // make sure dsp reset success, in cases of 32khz clk area

    SCI_DisableIRQ();
    CHIP_REG_AND (GR_GEN0, ~GEN0_MCU_DSP_RST);
    SCI_RestoreIRQ();
    CHIP_REG_SET (DSP_RST, 0);

}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetWDGHWFlag (WDG_HW_FLAG_T type)
{
    uint32 val = 0;

    switch (type)
    {
        case TYPE_RESET:
            {
                val = CHIP_PHY_GetHwRstFlag ();
            }
            break;

        case TYPE_BACKLIGHT:
            {
                val = CHIP_PHY_GetHwBLFlag ();
            }
            break;

        case TYPE_DSPEXCEPTION:
            {
                val = CHIP_PHY_GetHwDSPExceptionFlag ();
            }
            break;

        case TYPE_USB:
            {
                val = CHIP_PHY_GetHwUSBFlag ();
            }
            break;

        default:
            {
                SCI_ASSERT (0); /*assert verified*/
            }
            break;
    }

    return val;
}

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
    switch (type)
    {
        case TYPE_RESET:
            {
                CHIP_PHY_SetHwRstFlag (val);
            }
            break;

        case TYPE_BACKLIGHT:
            {
                CHIP_PHY_SetHwBLFlag (val == 0 ? 0:1);
            }
            break;

        case TYPE_DSPEXCEPTION:
            {
                CHIP_PHY_SetHwDSPExceptionFlag (val == 0 ? 0:1);
            }
            break;

        case TYPE_USB:
            {
                CHIP_PHY_SetHwUSBFlag (val == 0 ? 0:1);
            }
            break;

        default:
            {
                SCI_ASSERT (0); /*assert verified*/
            }
            break;
    }

}

PUBLIC void CHIP_PHY_CLKAUX_OnOff (
    CLKAUX_ID_E id,
    BOOLEAN is_on,
    CLKAUX_SRC_E clk,
    uint32 divider)
{
    if (is_on)
    {
        switch (id)
        {
            case CLKAUX_ID_0:
                REG32 (GR_GEN1) &= ~0xFF;             // div
                REG32 (GR_GEN1) |= (divider-1) &0xFF;
                REG32 (GR_PLL_SCR) &= ~ (BIT_10|BIT_11); // clk
                REG32 (GR_PLL_SCR) |= (clk<<10);
                REG32 (GR_GEN1) |= GEN1_CLK_AUX0_EN;  // en
                break;
            case CLKAUX_ID_1:
                REG32 (GR_PCTL) &= ~ (0xFF<<22);      // div
                REG32 (GR_GEN1) |= ( ( (divider-1) &0xFF) <<22);
                REG32 (GR_PLL_SCR) &= ~ (BIT_12|BIT_13); // clk
                REG32 (GR_PLL_SCR) |= (clk<<12);
                REG32 (GR_GEN1) |= GEN1_CLK_AUX1_EN;  // en
                break;
            default:
                SCI_ASSERT (0); /*assert verified*/
        }
    }
    else
    {
        switch (id)
        {
            case CLKAUX_ID_0:
                REG32 (GR_GEN1) &= ~GEN1_CLK_AUX0_EN;
                break;
            case CLKAUX_ID_1:
                REG32 (GR_GEN1) &= ~GEN1_CLK_AUX1_EN;
                break;
            default:
                SCI_ASSERT (0); /*assert verified*/
        }
    }
}


#define DSP_MCU_CMD_OFFSET_ADDR       (0x00012600)//0x01F01DE0
volatile DSP_MCU_CMD_T *cmd_ptr    = (DSP_MCU_CMD_T *) (DSP_MCU_CMD_OFFSET_ADDR + DSP_SHAREMEM_VIR_BASE_ADDR);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
