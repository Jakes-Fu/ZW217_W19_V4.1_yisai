/******************************************************************************
 ** File Name:      chip_phy_sc6531efm.c                                          *
 ** Author:                                                                   *
 ** DATE:           01/27/2016                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/27/2016                      Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//lint -e766 for "emc_drvapi.h" "sci_types.h" "sci_api.h" "sio.h"
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "power_manager.h"
#include "fiq_drvapi.h"
#include "busmonitor_drvapi.h"
#include "sim_drv.h"
#include "os_param.h"
#include "pinmap_drvapi.h"
#include "mmu_drvapi.h"
#include "analog_drvapi.h"
#include "mcu_reboot.h"
#include "clock_drvapi.h"
#include "Ref_outport.h"
#include "boot_drvapi.h"
#include "cmddef.h"
#include "dal_power.h"
#include "gpio_prod_api.h"
#include "emc_drvapi.h"
#include "dsp_drvapi.h"
#include "../chip_phy.h"
#include "osc_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

extern uint32 v_cp_jump_addr;

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

#define HW_RST_OFFSET           0
#define HW_RST_MASK       		0x1F
#define HW_BL_MASK         		BIT_5
#define HW_DSPEXCEPTION_MASK  	BIT_6
#define HW_USB_MASK       		BIT_7

/* boot flag, defined the same with romcode*/
#define BOOT_FLAG_EXT_RESET             (0x8A)
#define BOOT_FLAG_WDG_RESET     		(0x9A)

#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)

#define ADI_MST_ARM_RD_DATA_MASK (0xFFFF)
#define ADI_MST_ARM_RD_ADDR_SHIFT(n)  (((n>>16)&0x7FFF))
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)   
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL) 

#define ADIE_CHIPID         0x11310000
#define CHIPID_HIGHPART_SHIFT       0x10

#define PSRAM_FLAG_REG 0x20500168

#ifndef BIT0
#define BIT0		0x00000001
#endif
#pragma arm section rwdata = "CHIP_RESET_CODE"
#if 1
volatile  uint32 reset_data[] =
{
	0xff525354,   //magic number
	0x0010001C,   //reset code length[31:16] and offset[15:0]
	0x0ac04f7d,   //hash value[0]
	0xef86e3b1,   //hash value[1]
	0x92795ebd,   //hash value[2]
	0x2019878a,   //hash value[3]
	0x926c2022,   //hash value[4]
    0xe59f1004,   //reset code
    0xe5810000,
    0xe3a0f000,
    0x205000e0
};
#else
volatile  uint32 reset_data[] =
{
	0xff525354,   //magic number
	0x0138001C,   //reset code length[31:16] and offset[15:0]
	0xa7e411c2,   //hash value[0]
	0x3956d2f4,   //hash value[1]
	0x54aa614b,   //hash value[2]
	0x1e4e9ef6,   //hash value[3]
	0xe8ba5b4c,   //hash value[4]
	0xE3A00000,   //reset code
	0xE59F110C, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F1104, 
	0xE5810000, 
	0xE3A00066, 
	0xE59F10FC, 
	0xE5810000, 
	0xE3A00005, 
	0xE59F10F4, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F10EC, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F10E4, 
	0xE5810000, 
	0xE3A00C01, 
	0xE2400001, 
	0xE3500000, 
	0x1AFFFFFC, 
	0xE3A00001, 
	0xE59F10C4, 
	0xE5810000, 
	0xE3A00000, 
	0xE59F10A8, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F10A0, 
	0xE5810000, 
	0xE3A00099, 
	0xE59F1098, 
	0xE5810000, 
	0xE3A00005, 
	0xE59F1090, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F1088, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F1080, 
	0xE5810000, 
	0xE3A00C01, 
	0xE2400001, 
	0xE3500000, 
	0x1AFFFFFC, 
	0xE3A00001, 
	0xE59F1050, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F1048, 
	0xE5810000, 
	0xE3A00003, 
	0xE59F1040, 
	0xE5810000, 
	0xE59F0048, 
	0xE59F1038, 
	0xE5810000, 
	0xE3A00001, 
	0xE59F1030, 
	0xE5810000, 
	0xE3A00C01, 
	0xE2400001, 
	0xE3500000, 
	0x1AFFFFFC, 
	0xE3A00000, 
	0xE59F1020, 
	0xE5810000, 
	0xE3A0F000, 
	0x20A00000, 
	0x20A00008, 
	0x20A00040, 
	0x20A00070, 
	0x20A0000C, 
	0x20A00004, 
	0x00009101, 
	0x205000E0
};
#endif
#pragma arm section rwdata

typedef enum
{
    CAL_VDDRF = 0,
    CAL_VDD18 = 4,
    CAL_VDDSIM = 8,
    CAL_VDDCAMA = 12,
    CAL_VDDCAMD = 16
} CAL_VOLTAGE_GROUP_E;
/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 chip_id = 0;
LOCAL uint32 chip_id_ex = 0;
LOCAL CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;
LOCAL CHIP_TYPE_EX_E chip_type_ex = CHIP_TYPE_EX_MAX;
LOCAL RF_CHIP_TYPE_E rf_chip_type = RF_CHIP_TYPE_MAX;//lint !e551
LOCAL uint32 psram_type_flag = 0;

const CHIP_INFO chip_info[] =
{
    {CHIP_ID_SC6531EFM,              "SC6531EFM\n"},
    {CHIP_ID_SC6531EFM_AB,        "SC6531EFM_AB\n"},
};

const CHIP_INFO_EX chip_info_ex[] =
{
    {CHIP_TYPE_EX_AC,        "SC6531EFM_AC\n"},
};

LOCAL uint32 ana_chip_id = 0;

const CHIP_INFO ana_chip_info[] =
{
    {CHIP_ID_SC1161,				 "SC1161\n"},
};
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void CHIP_PHY_SetANAReg (uint32 addr, uint32 value);
PUBLIC uint32 CHIP_PHY_GetANAReg (uint32 addr);
PUBLIC void CHIP_PHY_AndANAReg (uint32 addr, uint32 value);
PUBLIC void CHIP_PHY_OrANAReg (uint32 addr, uint32 value);
/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:        
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetWDGHWFlag (WDG_HW_FLAG_T type);

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         
//  Modified by:  
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val);

/*****************************************************************************/
//  Description:  Restore Jtag Pin Function selection after Assert
//  Author:         
//  Modified by:
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_Restore_Jtag_Function (void)
{
    // restore jtag pin function select
    CHIP_REG_SET (REG_IOMUX_PAD_AP_JTAG_TCK_CFG_REG, IOMUX_PAD_AP_JTAG_TCK_SEL(0));    
    CHIP_REG_SET (REG_IOMUX_PAD_AP_JTAG_TRST_CFG_REG, IOMUX_PAD_AP_JTAG_TRST_SEL(0));    
    CHIP_REG_SET (REG_IOMUX_PAD_AP_JTAG_TMS_CFG_REG, IOMUX_PAD_AP_JTAG_TMS_SEL(0));    
    CHIP_REG_SET (REG_IOMUX_PAD_AP_JTAG_TDI_CFG_REG, IOMUX_PAD_AP_JTAG_TDI_SEL(0));    
    CHIP_REG_SET (REG_IOMUX_PAD_AP_JTAG_TDO_CFG_REG, IOMUX_PAD_AP_JTAG_TDO_SEL(0));    
}

/*********************************************************************************************/
//Description:   This function is used to initializtion system registers
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
LOCAL void _CHIP_PHY_Reg_Init (void)
{
#if 0
    //set the emc port's ahb master priority, dcam>dma>arm
    CHIP_REG_OR (AHB_SYS_CTL5, ( (2 << MTX_PRI_M5TOS5_SEL_SHIFT) | (1 << MTX_PRI_M6TOS5_SEL_SHIFT) | (1 << MTX_PRI_M7TOS5_SEL_SHIFT)));

	//set cm4 priority
	CHIP_REG_OR (AHB_SYS_CTL1, (3 << MTX_PRI_M3TOS5_SEL_SHIFT));
#endif
}

/*****************************************************************************/
//  Description:    The function is used to get the Init ADI.
//  Global resource dependence:
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void CHIP_InitAdi (void)
{
	uint32 i = 100;
	
	//enable adi module
    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_ADI;

	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_ADI;

	while(i--);
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_ADI;

    ADI_Phy_init();
}

PUBLIC void CHIP_Set7sSwRstMode(void)
{
	uint32  val = 0;
	
  	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
  	val &= ~(BIT_0);
	CHIP_PHY_SetANAReg(ANA_POR_7S_CTRL, val);
}

PUBLIC void CHIP_Enable7sRstMode(void)
{
	uint32  val = 0;
	
  	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val &= ~BIT_1;
	CHIP_PHY_SetANAReg(ANA_POR_7S_CTRL, val);
}

PUBLIC void CHIP_Disable7sRstMode(void)
{
	uint32  val = 0;
	
  	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val |= BIT_1;
	CHIP_PHY_SetANAReg(ANA_POR_7S_CTRL, val);
}

PUBLIC void CHIP_DisablePowerKeyInt(void)
{
	uint16 tmp_val = 0;

	//disable power key interrupt
	tmp_val = ANA_REG_GET(ANA_EIC_BASE+0x18);
	tmp_val &= ~(BIT_1);
	ANA_REG_SET((ANA_EIC_BASE+0x18), tmp_val);
}

PUBLIC void CHIP_LDOSFPDEnAfterRst(void)
{
	uint16 tmp_val = 0;

	//software reset ldo sf power down enable
	tmp_val = ANA_REG_GET(ANA_SWRST_CTRL1);
	tmp_val |= BIT_2;
	ANA_REG_SET((ANA_SWRST_CTRL1), tmp_val);
	
	tmp_val = ANA_REG_GET(ANA_SWRST_CTRL0);
	tmp_val |= (BIT_6 | BIT_9 | BIT_10);
	ANA_REG_SET((ANA_SWRST_CTRL0), tmp_val);
}

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void)
{
    ana_chip_id = (CHIP_PHY_GetANAReg (ANA_CHIP_ID_HIGH) & CHIP_ID_H16_MSK) << CHIPID_HIGHPART_SHIFT;
    ana_chip_id |= (CHIP_PHY_GetANAReg (ANA_CHIP_ID_LOW) & CHIP_ID_L16_MSK);

    //psram_type_flag = REG32(PSRAM_FLAG_REG);
    chip_id = hwp_sysCtrl->Cfg_CHIP_PROD_ID;
    chip_id_ex = hwp_sysCtrl->Cfg_CHIP_PROD_ID;//rom space last word

    switch (chip_id)
    {
        case 0x89100000:
            chip_type = UIX8910;
            break;
        default:
            break;
    }

    {
        uint32 id_ex =  chip_id_ex & (BIT_1|BIT_0);
        switch (id_ex)
        {
            case 0x0:
                chip_type_ex = CHIP_TYPE_EX_AA_AB;
                break;
            case 0x3:
                chip_type_ex = CHIP_TYPE_EX_AC;
                break;
            default:
                chip_type_ex = CHIP_TYPE_EX_MAX;
                break;
        }
    }
    return;
}

/*****************************************************************************/
//  Description:    The function is used to detect the memory type
//  Global resource dependence:
//  Author:         
//  Return value:
//                1      CS0 connects to SDRAM
//                0      CS0 connects to FLASH
/*****************************************************************************/
PUBLIC uint32 CHIP_DetectMemType (void)
{
    uint32 bSDRAM;

#ifdef NOR_SRAM_PLATFORM
    bSDRAM = FALSE;
#else
    bSDRAM = TRUE;
#endif

    return bSDRAM;
}
/*****************************************************************************/
//  Description:    The function is used to return rf chip ID.
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL  RF_CHIP_TYPE_E CHIP_InitRFChipType (void)
{
    return NULL;
}
/*****************************************************************************/
//  Description:    The function is used to return rf chip ID.
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC  RF_CHIP_TYPE_E CHIP_GetRFChipType (void)
{
    return NULL;
}
/*****************************************************************************/
//  Description:    The function is used to return rf & hw chip ID.
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC  uint8 CHIP_GetChipID (void)
{	
    return NULL;
}

/*****************************************************************************/
//  Description:    The function is used to return rf & hw chip ID.
//  Global resource dependence:  chip_id ->  7 6 5 | 4 3 2 1 <-0 rf_id
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC  uint8 CHIP_GetChipIDWithRF (void)
{	
    return NULL;
}
/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void)   // return chip ID number.
{
    return chip_id;
}

/*****************************************************************************/
//  Description:    The function is used to return psram type.
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetPsramTypeFlag (void)
{
    return psram_type_flag;
}

/*****************************************************************************/
//  Description:    The function change the core voltage
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_ChangeCoreVoltage (CORE_VOLTAGE_E core_voltage)
{

}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
    uint32 arm_clk = ARM_CLK_26M;

    volatile uint32 clk_reg = REG32 (APB_CLK_CFG3);

    clk_reg = ( (clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT);

    switch (clk_reg)
    {
        case 0:
            arm_clk = ARM_CLK_26M;
            break;
        case 1:
            arm_clk = ARM_CLK_104M;
            break;
        case 2:
            arm_clk = ARM_CLK_208M;//CPLL
            break;
        case 3:
            arm_clk = ARM_CLK_208M;//APLL
            break;
        default:
            break;
    }

    return arm_clk;
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    volatile uint32 reg_val = REG32 (APB_CLK_CFG3);

    if (reg_val & CLK_MCU_RATIO)
    {
        return ARM_CLK_104M;
    }
    else
    {
        return (CHIP_GetArmClk() >> 1);
    }
}


/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void)
{
    //return ARM_CLK_26M;
    return ARM_CLK_200M;
}


/*****************************************************************************/
// Description :    This function is used to disable all fiqs on sc6600d
// Global resource dependence :
// Author :         
// Note :           every bit indicates one fiq src
/*****************************************************************************/
void CHIP_DisableFIQ (void)
{
    CHIPDRV_DisableFIQINTALL();
}

/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 CHIP_GetPllClk (void)
{
    return CHIP_GetMPllClk();
}

/*****************************************************************************/
// Description :    This function is used to get current MPLL clock.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 CHIP_GetMPllClk (void)
{
    return 0;
}

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
uint32 CHIP_GetVPllClk (void)
{
    return NULL;
}
/*****************************************************************************/
// Description :    This function is used to tune current APLL clock according to specified tune mode
// Global resource dependence :fout=26M*(nint+kint/2^23)
// Author :
// Note :
/*****************************************************************************/
void CHIP_FineTuneAPllClk(APLL_FTUNE_MODE_E clkft_mode)
{
         uint32 APLL_NINT = 0x18;
         uint32 APLL_KINT = 0x0;
         uint32 bb_top6_val;
         uint32 bb_top7_val;

         switch(clkft_mode)
         {
                   case APLL_FT_P1D5:
                            //fout:624MHz+1.5MHz
                            APLL_KINT = 0x76276;
                            APLL_NINT = 0x18;
                            break;
                   case APLL_FT_M1D5:
                            //fout:624MHz-1.5MHz
                            APLL_KINT = 0x789D8A;
                            APLL_NINT = 0x17;
                            break;
                   case APLL_FT_P3:
                            //fout:624MHz+3MHz
                            APLL_KINT = 0xEC4EC;
                            APLL_NINT = 0x18;
                            break;
                   case APLL_FT_M3:
                            //fout:624MHz-3MHz
                            APLL_KINT = 0x713B13;
                            APLL_NINT = 0x17;
                            break;
                   case APLL_FT_P4D5:
                            //fout:624MHz+4.5MHz
                            APLL_KINT = 0x162762;
                            APLL_NINT = 0x18;
                            break;
                   case APLL_FT_M4D5:
                            //fout:624MHz-4.5MHz
                            APLL_KINT = 0x69D89D;
                            APLL_NINT = 0x17;
                            break;
                   case APLL_FT_P6:
                            //fout:624MHz+6MHz
                            APLL_KINT = 0x1D89D8;
                            APLL_NINT = 0x18;
                            break;
                   case APLL_FT_M6:
                            //fout:624MHz-6MHz
                            APLL_KINT = 0x627627;
                            APLL_NINT = 0x17;
                            break;          
                   default:
                            //fout:624MHz
                            APLL_KINT = 0x0;
                            APLL_NINT = 0x18;
                            break;
         }
         //To get original value from top6
         bb_top6_val = REG32(ANALOG_BB_TOP6);
         //To get original value from top7
         bb_top7_val = REG32(ANALOG_BB_TOP7);

         bb_top6_val &= ~(APLL_NINT_MSK | APLL_MOD_EN);
         //To enable SDM&DIV_S, and set NINT
         bb_top6_val |= ((APLL_NINT<<APLL_NINT_SHIFT) | APLL_SDM_EN | APLL_DIV_S);

         //To set KINT value
         bb_top7_val &= ~(APLL_KINT_MSK);
         bb_top7_val |= (APLL_KINT<<APLL_KINT_SHIFT);

         SCI_DisableIRQ();
         
         //To active the value of NINT/KINT
          if(SC6531EFM_AB == CHIP_GetChipType()){
			// VSET[0] set 1                
			REG32(ANALOG_BB_TOP6) |= (BIT0<<APLL_VSET_SHIFT);
			// KINT set to new value during vset: 1
			REG32(ANALOG_BB_TOP7) = bb_top7_val;
			// NINT set to new value during vset: 1
			bb_top6_val |= (BIT0<<APLL_VSET_SHIFT);
			REG32(ANALOG_BB_TOP6) = bb_top6_val;
			// VSET[0] set 0  
			bb_top6_val &=~(BIT0<<APLL_VSET_SHIFT);
			REG32(ANALOG_BB_TOP6) = bb_top6_val;
          }else{
            REG32(ANALOG_BB_TOP6) = bb_top6_val;
            REG32(ANALOG_BB_TOP7) = bb_top7_val &(~(BIT0<<APLL_KINT_SHIFT));
            REG32(ANALOG_BB_TOP7) = bb_top7_val |(BIT0<<APLL_KINT_SHIFT);
            REG32(ANALOG_BB_TOP7) = bb_top7_val &(~(BIT0<<APLL_KINT_SHIFT));
            REG32(ANALOG_BB_TOP7) = bb_top7_val |(BIT0<<APLL_KINT_SHIFT);
            REG32(ANALOG_BB_TOP7) = bb_top7_val &(~(BIT0<<APLL_KINT_SHIFT));
          }
         SCI_RestoreIRQ();

         SCI_TRACE_LOW("Tune APLL freq(NINT:%d/KINT:%d)", APLL_NINT, APLL_KINT);
}

/******************************************************************************/
//  Description:   Get product mode
//  Parameter:
//      None
//    Return:
//           1     In product mode
//           0     Product mode close
/******************************************************************************/
PUBLIC BOOLEAN CHIP_IsSPINorMode (void)
{
    return TRUE;
}

/*****************************************************************************/
/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
LOCAL void CHIP_OpenOSCSelfBeforePD (void)
{
    //32K less for RC Calibration 
    OSC_HAL_EnableClkSource (CLOCK_SELF_32K,SCI_TRUE);
    OSC_HAL_SelectClkSource (CLOCK_SELF_32K);
    OSC_HAL_SELF_EnableCalibrate (SCI_TRUE);
    OSC_HAL_SELF_StartCalibrate (0xFFFF);
    OSC_PHY_EnableClkSource(CLOCK_SELF_32K,SCI_FALSE);

    //Low power mode for 32k less
    OSC_HAL_EnableLowPowerClock32k(SCI_TRUE);

    CHIP_PHY_OrANAReg(ANA_LDO_CAMD_REG0,LDO_CAMD_PD);
    CHIP_PHY_OrANAReg(ANA_LDO_RF_REG0,LDO_RF_PD);
    CHIP_PHY_OrANAReg(ANA_LDO_USB_REG0,LDO_USB_PD);	
    CHIP_PHY_OrANAReg(ANA_LDO_SIM1_PD_REG,LDO_SIM1_PD_REG);	
    CHIP_PHY_OrANAReg(ANA_LDO_SIM0_REG2,LDO_SIM0_PD);	
    CHIP_PHY_OrANAReg(ANA_LDO_SD_REG2,LDO_SD_PD);	
    CHIP_PHY_OrANAReg(ANA_LDO_CAMA_REG0,LDO_CAMA_PD);	
    CHIP_PHY_OrANAReg(ANA_LDO_BTPA_REG0,LDO_BTPA_PD);	
    
    CHIP_PHY_SetANAReg(ANA_PWR_WR_PROT_VALUE,0x6E7F);
    while(1)
    {
        if(CHIP_PHY_GetANAReg(ANA_PWR_WR_PROT_VALUE) == 0x8000)
        {
            CHIP_PHY_OrANAReg(ANA_POWER_PD_SW,(LDO_DCXO_PD |LDO_CORE_PD|LDO_SF_PD|
        				     LDO_VDD18_PD|LDO_VDD28_PD|BG_PD));
            break;
        }
    }
}

void CHIP_IsPowerOpenDCXO32(BOOLEAN isPower)
{
    //32K less for RC Calibration::Hardware switch to DCXO-32K after about 4ms by RC power on. 
    OSC_HAL_SELF_EnableCalibrate (SCI_TRUE);
    OSC_HAL_SELF_StartCalibrate (0xFFFF);
    OSC_PHY_EnableClkSource(CLOCK_SELF_32K,SCI_FALSE);

    if(isPower)
    {
         OSC_HAL_EnableDcxoAutoPD (SCI_FALSE);
    }
    else
    {	   
         CHIP_PHY_OrANAReg(ANA_LDO_CAMD_REG0,LDO_CAMD_PD);
         CHIP_PHY_OrANAReg(ANA_LDO_RF_REG0,LDO_RF_PD);
         CHIP_PHY_OrANAReg(ANA_LDO_USB_REG0,LDO_USB_PD);	
         CHIP_PHY_OrANAReg(ANA_LDO_SIM1_PD_REG,LDO_SIM1_PD_REG);	
         CHIP_PHY_OrANAReg(ANA_LDO_SIM0_REG2,LDO_SIM0_PD);	
         CHIP_PHY_OrANAReg(ANA_LDO_SD_REG2,LDO_SD_PD);	
         CHIP_PHY_OrANAReg(ANA_LDO_CAMA_REG0,LDO_CAMA_PD);	
         CHIP_PHY_OrANAReg(ANA_LDO_BTPA_REG0,LDO_BTPA_PD);	
         
         CHIP_PHY_SetANAReg(ANA_PWR_WR_PROT_VALUE,0x6E7F);
         while(1)
         {
             if(CHIP_PHY_GetANAReg(ANA_PWR_WR_PROT_VALUE) == 0x8000)
             {
                 CHIP_PHY_OrANAReg(ANA_POWER_PD_SW,(LDO_CORE_PD|LDO_SF_PD|
             		      LDO_VDD18_PD|LDO_VDD28_PD|BG_PD));
                 break;
             }
         }

  	}
}

void CHIP_TurnOffPower (void)
{
    //set rtc spare
    RTC_SetSpareReg();

    // Disable FIQ
    CHIPDRV_DisableFIQINTALL();
    // Disable IRQ
    CHIPDRV_DisableIRQINTALL();

#if defined(OSC32K_CLOCK_SELECT_SELF)
    CHIP_OpenOSCSelfBeforePD();
#endif
    //power down all the ldo

#if defined(OSC32K_CLOCK_SELECT_DCXO)
    CHIP_IsPowerOpenDCXO32(SCI_FALSE);
#endif

    for (;;) {}
}


/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void)
{
#if 0 //temp_stub_sgq
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    while (i--) {}

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

    REG32 (APB_INT_SET_CLR0) = MCU_FRQ_SET;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }



    DSP_HAL_DspinResetMode (TRUE);

    // Indicate that DSP is resetted.
    REG16 (0x30000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    WDG_ResetMCU();
    OS_TickDelay (100);
    //Clear FIQ and IRQ from DSP
    DSP_HAL_ClrDspIrqInt (DSP_INT_IRQ0);
    DSP_HAL_ClrDspIrqInt (DSP_INT_FRQ0);

    if (CHIP_DetectMemType())
    {
        mcu_nand_reboot();
    }
#endif
}

/*****************************************************************************/
// Description :    This function is used to reset MCU from boot code.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
void CHIP_BootReset (void)
{
    WDG_TimerStop ();

    /* Disable bus monitor */
    if (CHIP_DetectMemType())
    {
        /* Disable bus monitor */
        bm_disable();
    }

    // Hold dsp
    DSP_HAL_DspinResetMode (TRUE);

    // Indicate that DSP is resetted.
    REG16 (0x30000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    //Clear FIQ and IRQ from DSP
    DSP_HAL_ClrDspIrqInt (DSP_INT_IRQ0);
    DSP_HAL_ClrDspIrqInt (DSP_INT_FRQ0);

    mcu_uart_reboot();

#ifdef FLASH_TYPE_SPI
    // Dummy operation, to prevent "CHIP_Reset" be optimized
    //SCI_TraceLow:"%x, %x"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHIP_PHY_V5_414_112_2_17_23_3_42_497, (uint8 *) "dd", reset_data[0], reset_data[1]);
#endif

}

/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void)
{
    /* Init system registers */
    _CHIP_PHY_Reg_Init();

    /* Init system clock */
    CLOCK_Config();

    rf_chip_type = CHIP_InitRFChipType();

    //cal vdd 18
    //Should read parameter from efuse or nv to calibration LDO voltage
    //CHIP_CaliVoltageLevel(CAL_VDD18,0x4);
    if(SC6531EFM == CHIP_GetChipType())
    {
        //cpll ldo voltage modify
        *((volatile uint32 *)CPLL_APLL_CONFIG) = 0x6175303;
    }
#ifdef RELEASE_INFO
	CHIP_Disable7sRstMode();
#else
	/* enable software 7s reset mode*/
	CHIP_Set7sSwRstMode();
#endif
	
#if	defined(OSC32K_CLOCK_SELECT_DCXO) || defined(OSC32K_CLOCK_SELECT_SELF)
    //CHIP_IsPowerOpenDCXO32(SCI_TRUE);     //linker_ljj 0805
#endif  

//#ifndef  FPGA_VERIFICATION
    // Init Pin function and GPIOs setting
    PM_Init ();

    //init the module of dsp hold arm
    //DSP_HoldArmInit();

//#endif
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
// Description :    This function is used get the chip type ex
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_EX_E CHIP_GetChipTypeEx (void)
{
    return chip_type_ex;
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


    SCI_ASSERT (NULL != chip_id); /*assert verified*/

    for (i=0; i<sizeof (chip_info) /sizeof (CHIP_INFO); i++)
    {
        if (chip_info[i].chip_id == chip_id)
        {
            info = (const char *) chip_info[i].info;
            break;
        }
    }

    if(chip_type_ex > CHIP_TYPE_EX_AA_AB)
    {
        for (i=0; i<sizeof (chip_info_ex) /sizeof (CHIP_INFO_EX); i++)
        {
            if (chip_info_ex[i].chip_type_ex == chip_type_ex)
            {
                info = (const char *) chip_info_ex[i].info;
                break;
            }
        }		
    }
	
    SCI_ASSERT (info != NULL);/*assert verified*/

    return info;
}

/*****************************************************************************/
// Description :    This function is used get the a die chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetADieChipInfo (void)
{
    uint8 i;
    const char *info = NULL;
    SCI_ASSERT (NULL != ana_chip_id); /*assert verified*/

    for (i=0; i<sizeof (ana_chip_info) /sizeof (CHIP_INFO); i++)
    {
        if (ana_chip_info[i].chip_id == ana_chip_id)
        {
            info = (const char *) ana_chip_info[i].info;
            break;
        }
    }

    SCI_ASSERT (info != NULL); /*assert verified*/
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
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void)
{
    REG32 (APB_MISC_CTL0) |= USB_STATE_REN;
    return ( ( (REG32 (APB_MISC_STS0)) & USB_RX_DM) == USB_RX_DM);
}


/*below are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void)
{
    BOOLEAN is_dsp_respond = SCI_FALSE;
#if !defined(PLATFORM_UWS6121E)

    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;

    REG32 (APB_INT_SET_CLR0) |= MCU_FRQ_SET;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }

    if (1 == cmd_ptr->RefCmd_block.cmd_done)
    {
        // dsp can reponse to FIQ
        is_dsp_respond = SCI_TRUE;
    }
    else
    {
        // dsp can not reponse to FIQ and run away
        is_dsp_respond = SCI_FALSE;
    }
#endif
    return is_dsp_respond;
}

/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_BeforeAssert (void)
{
    uint8 assert_val;
    //in other platform, deal relative case here ...

    OSPARAM_GetAssertMode (&assert_val);

#if 0 //temp_stub_sgg
    if (SCI_ASSERT_DEBUG_MODE == assert_val)
    {
        // If audio device is being controlled by DSP now, switch it back to ARM.
        VB_SWTICH_ARM_CTL (SCI_TRUE);

        // Shut down the speaker PA.
        GPIO_EnableAmplifier (SCI_FALSE);

        // power off sim
        SIM_PowerOffSIM();
    }

    bm_disable();
#endif

    CHIP_PHY_Restore_Jtag_Function();

}

// The function is used to reset and set dsp to output dsp assert log to pc by uart1
PUBLIC void CHIP_DspOutPutAssertInfo (void)
{

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
// Author:        
// Note:          Beforce invoking this function, the DSP/ISP should be hold on!
/******************************************************************************/
PUBLIC uint32 Chip_SwitchIRAM2Arm (void)
{
    REG32 (APB_MCU_CTL0) |= (ARM_MEM0_EN | ARM_MEM1_EN | ARM_MEM2_EN | ARM_MEM3_EN);
    return 0;
}

PUBLIC uint32 Chip_SwitchIRAMDefault (void)
{
    /* IRAM0 and IRAM1 for arm by default */
    REG32 (APB_MCU_CTL0) |= (ARM_MEM0_EN | ARM_MEM1_EN);
    /* IRAM2 and IRAM3 for dsp by default */
    REG32 (APB_MCU_CTL0) &= ~ (ARM_MEM2_EN | ARM_MEM3_EN);
    return 0;
}

/*****************************************************************************/
//  Description:    Gets the register in analog die to judge the reset mode.
//  Author:         
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetANAReg (uint32 addr)
{
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
    rd_addr = ADI_MST_ARM_RD_ADDR_SHIFT(adi_rd_data);
    if(rd_addr != addr >> 2)
    {
        SCI_PASSERT(0, ("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr));/*assert verified*/
    }    

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
}

/*****************************************************************************/
//  Description:    The function is used to return ADIE chip ID (a uint32 number).
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_GetADIEChipID (void)
{
    return ana_chip_id;
}

/*****************************************************************************/
//  Description:    Sets the register in analog die.
//  Author:         
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetANAReg (uint32 addr, uint32 value)
{
    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)value;

    // wait write fifo empty after adi analog write
    while(ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
    }
}

PUBLIC void CHIP_PHY_AndANAReg (uint32 addr, uint32 value)
{
	uint16 tmp_value = 0;

	tmp_value = CHIP_PHY_GetANAReg(addr);
	tmp_value &= (uint16)value;
    CHIP_PHY_SetANAReg(addr, tmp_value);
}

PUBLIC void CHIP_PHY_OrANAReg (uint32 addr, uint32 value)
{
    uint16 tmp_value = 0;

	tmp_value = CHIP_PHY_GetANAReg(addr);
	tmp_value |= (uint16)value;
    CHIP_PHY_SetANAReg(addr, tmp_value);
}

/*****************************************************************************/
//  Description:    Returns the POR_HW_RST monitor register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetPORHwRstMonAddr (void)
{
    return ANA_POR_RST_MONITOR;
}

/*****************************************************************************/
//  Description:    Returns the WDG_HW_RST monitor register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetWDGHwRstMonAddr (void)
{
    return ANA_WDG_RST_MONITOR;
}

/*****************************************************************************/
//  Description:    Returns the POR_PIN_HW_RST monitor register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetPORPinRstMonAddr (void)
{
    return ANA_POR_PIN_RST_MONITOR;
}

/*****************************************************************************/
//  Description:    Returns the HW_RST monitor register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL __inline uint32 CHIP_PHY_GetHwRstMonitorAddr (void)
{
    return CHIP_PHY_GetPORHwRstMonAddr();
}

/*****************************************************************************/
//  Description:    Returns the Hardware reset mode record register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL __inline uint32 CHIP_PHY_GetHwRstModeAddr (void)
{
    return APB_MISC_CTL2;
}

/*****************************************************************************/
//  Description:    Returns the Power on source flag register address.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL __inline uint32 CHIP_PHY_GetPorSrcFlagAddr (void)
{
	return ANA_POR_SRC_FLAG;
}

/*****************************************************************************/
//  Description:    Returns the extern reset flag.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetExtRstFlag (void)
{
    return CHIP_PHY_GetANAReg(CHIP_PHY_GetPorSrcFlagAddr()) & POR_EXT_RST_FLAG;
}

/*****************************************************************************/
//  Description:    clear extern reset flag.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_ClearExtRstFlag (void)
{
	uint32  val = 0;
	
  	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val |= EXT_RSTN_FLAG_CLR;
    CHIP_PHY_SetANAReg (ANA_POR_7S_CTRL, val);

	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val &= ~EXT_RSTN_FLAG_CLR;
    CHIP_PHY_SetANAReg (ANA_POR_7S_CTRL, val);
}

/*****************************************************************************/
//  Description:    Returns the 7S reset software flag.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_Get7SRstSWFlag(void)
{
	return CHIP_PHY_GetANAReg (CHIP_PHY_GetPorSrcFlagAddr()) & POR_PBINT_7S_SW_FLAG;
}

/*****************************************************************************/
//  Description:    Returns the 7S reset hardware flag.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_Get7SRstHWFlag(void)
{
	return CHIP_PHY_GetANAReg (CHIP_PHY_GetPorSrcFlagAddr()) & POR_PBINT_7S_HW_FLAG;
}

/*****************************************************************************/
//  Description:    clear 7S reset software flag.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_Clear7SRstSWFlag (void)
{
	uint32  val = 0;
	
  	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val |= PBINT_7S_FLAG_CLR;
    CHIP_PHY_SetANAReg (ANA_POR_7S_CTRL, val);

	val = CHIP_PHY_GetANAReg(ANA_POR_7S_CTRL);
	val &= ~PBINT_7S_FLAG_CLR;
    CHIP_PHY_SetANAReg (ANA_POR_7S_CTRL, val);
}

/*****************************************************************************/
//  Description:    This fuction returns the reset mode value.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstFlag (void)
{
    return (ANA_REG_GET (CHIP_PHY_GetHwRstMonitorAddr()) & HW_RST_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the backlight mode value.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwBLFlag (void)
{
    return (ANA_REG_GET (CHIP_PHY_GetHwRstMonitorAddr()) & HW_BL_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the dsp exception mode value.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwDSPExceptionFlag (void)
{
    return (ANA_REG_GET (CHIP_PHY_GetHwRstMonitorAddr()) & HW_DSPEXCEPTION_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the usb mode value.
//  Author:         
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwUSBFlag (void)
{
    return (ANA_REG_GET (CHIP_PHY_GetHwRstMonitorAddr()) & HW_USB_MASK);
}

/*****************************************************************************/
//  Description:  This fuction set the reset flag of HWRST reg
//  Author:         
//  Modified by:  
//  Note:           The valid bit filed is from bit4 to bit0
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    ANA_REG_AND (CHIP_PHY_GetHwRstMonitorAddr(), ~HW_RST_MASK);
    ANA_REG_OR (CHIP_PHY_GetHwRstMonitorAddr(), (val&HW_RST_MASK));
}

/*****************************************************************************/
//  Description:  This fuction set the backlight flag of HWRST reg
//  Author:         
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwBLFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (CHIP_PHY_GetHwRstMonitorAddr(), HW_BL_MASK);
    }
    else
    {
        ANA_REG_AND (CHIP_PHY_GetHwRstMonitorAddr(), ~HW_BL_MASK);
    }
}

/*****************************************************************************/
//  Description:   This fuction set the dsp exception flag of HWRST reg
//  Author:         
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwDSPExceptionFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (CHIP_PHY_GetHwRstMonitorAddr(), HW_DSPEXCEPTION_MASK);
    }
    else
    {
        ANA_REG_AND (CHIP_PHY_GetHwRstMonitorAddr(), ~HW_DSPEXCEPTION_MASK);
    }
}

/*****************************************************************************/
//  Description:  This fuction set the usb flag of HWRST reg
//  Author:         
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwUSBFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (CHIP_PHY_GetHwRstMonitorAddr(), HW_USB_MASK);
    }
    else
    {
        ANA_REG_AND (CHIP_PHY_GetHwRstMonitorAddr(), ~HW_USB_MASK);
    }
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_ResetHWFlag. It resets the HW
//                  reset monitor register after system initialization.
//  Author:         
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val)
{
	//POR reset
    ANA_REG_AND (ANA_POR_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_POR_RST_MONITOR, (val&0xFFFF));

	//watchdog reset
    ANA_REG_AND (ANA_WDG_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_WDG_RST_MONITOR, (val&0xFFFF));

	//pin reset
    ANA_REG_AND (ANA_POR_PIN_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_POR_PIN_RST_MONITOR, (val&0xFFFF));

	//APB_MISC_CTL2 record reset flag
    CHIP_REG_SET (CHIP_PHY_GetHwRstModeAddr(), val);
}

#pragma arm section code = "MCU_CLK_INIT_IN_IRAM"
PUBLIC void CHIP_PHY_MCU_CLK_Init (void)
{
    //clock_init();
}
#pragma arm section code

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule ()
{
#ifdef NOR_SRAM_PLATFORM
    //CHIP_PHY_MCU_CLK_Init();   //ÆÁ±Î´ò×®
#endif

#ifndef NOR_BOOTLOADER_SUPPORT
	// Init PSRAM
	//EMC_Init();   //ÆÁ±Î´ò×®
#endif
    MMU_EnableICache();

}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
    //CHIP_REG_AND (APB_MCU_CTL0, ~ (ARM_MEM1_EN | ARM_MEM2_EN | ARM_MEM3_EN));  //ÆÁ±Î´ò×®
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    if ( (CHIP_PHY_GetANAReg (CHIP_PHY_GetHwRstMonitorAddr()) & HW_RST_MASK) == (flag & HW_RST_MASK))
    {
        return SCI_FALSE;
    }
    else
    {
        return SCI_TRUE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by ext
//                  reset.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByExtReset (void)
{
    if (CHIP_PHY_GetExtRstFlag())
    {
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
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByWatchDog(void)
{
    if (CHIP_PHY_GetANAReg (ANA_WDG_RST_MONITOR) == 0 && CHIP_PHY_GetANAReg (CHIP_PHY_GetHwRstMonitorAddr()) != 0)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by 7S 
//                  soft reset.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetBySW7S(void)
{
    if (CHIP_PHY_Get7SRstSWFlag() && (CHIP_PHY_Get7SRstHWFlag() == 0))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by 7S 
//                  hard reset.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByHW7S(void)
{
    if (CHIP_PHY_Get7SRstSWFlag() && CHIP_PHY_Get7SRstHWFlag())
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

#if 0
/*****************************************************************************/
// Description :    This function returns whether the reset is caused by power
//                  up.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByPowerUp(void)
{
	if(CHIP_PHY_IsResetByExtReset() || CHIP_PHY_IsResetByWatchDog() || CHIP_PHY_IsResetBySW7S())
	{
		return SCI_FALSE;
	}
	else
	{
		return SCI_TRUE;
	}
}
#endif

/*****************************************************************************/
// Description :    This function initializes the modules before assert while
//                  the system has just been reset.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC void CHIP_PHY_WDGAssertInit ()
{
    // Get current chip type
    CHIP_InitHWChipID();

    CLOCK_Config();
    PM_Init ();
    DMA_HAL_Init ();
    GPIO_Init ();
    ADC_Init ();

    // Open LCD backlight
    ANA_SetDevValule (ANA_DEV_ID_LCM_BL, ANA_DEV_CLOSE);
    ANA_SetDevValule (ANA_DEV_ID_LCM_BL, ANA_DEV_OPEN);
}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         
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
//  Author:         
//  Modified by:  
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

/*****************************************************************************/
//  Description:    ADC Sample Prepare Enter
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC int CHIP_ADC_Sample_Prepare_Enter (void)
{
	return SCI_TRUE;
}

/*****************************************************************************/
//  Description:    ADC Sample Prepare Exit
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC int CHIP_ADC_Sample_Prepare_Exit (void)
{	
	return SCI_TRUE;
}

/*****************************************************************************/
//  Description:    CHIP_MODEM_CPU_Start
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_MODEM_CP_Start(void)
{	
       //hwp_sysCtrl->CPCPU_RST_SET = 0xF; //reset cp core
       hwp_sysCtrl->Cfg_Reserve3 = 0xC8DACAFE;
	hwp_sysCtrl->Cfg_Reserve4 = v_cp_jump_addr;   
       hwp_sysCtrl->CPCPU_RST_CLR = 0xF; //release cp core
}


/*****************************************************************************/
//  Description:    CHIP_MODEM_ZSP_Start
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_MODEM_ZSP_Start(void)
{	
    
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

