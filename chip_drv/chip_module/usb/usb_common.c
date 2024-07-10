/******************************************************************************
 ** File Name:      DRV_usb.c                                                 *
 ** Author:         JiaYong.Yang                                              *
 ** DATE:           09/01/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "ldo_drvapi.h"
#include "usb_drv.h"
#include "tb_drv.h"
#include "usb20_drvapi.h"

#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))

PUBLIC void USB_Reset (void)
{
    usb_core_init();
    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EnableIrq (void)
{
    CHIPDRV_EnableIRQINT(TB_USB_INT);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_DisableIrq (void)
{
    CHIPDRV_DisableIRQINT(TB_USB_INT);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len)
{
    memcpy (des, src, len<<2);
    return TRUE ;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_DWordEndianSwap (uint32 DWord)
{
    return DWORD_ENDIAN_SWAP (DWord);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_LDO_Control (BOOLEAN flag)
{
    LDO_ID_E id = LDO_LDO_USB;
#if defined(PLATFORM_UWS6121E)
    id = LDO_LDO_USB;
#endif
    if (flag)
    {
        LDO_TurnOnLDO(id);
    }
    else
    {
        LDO_TurnOffLDO(id);
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
static void USB_Module_Enable (uint32  isEnable)
{
//bringup_tmp
#if 0
    if (isEnable == 0) {
        hwp_analogReg->usb_reg2 = 0;
        hwp_analogReg->USB_DIGPHY_ANA1 = 0;
        #if 0//bringup_tmp
        hwp_sysCtrl->REG_DBG = 0xA50001;
        hwp_sysCtrl->CLK_OTHERS_DISABLE  = SYS_CTRL_DISABLE_OC_ID_USBPHY;
        hwp_sysCtrl->CLK_OTHERS_1_DISABLE= SYS_CTRL_DISABLE_OC_ID_USB_UTMI_48M;
        hwp_sysCtrl->CLK_OTHERS_1_DISABLE= SYS_CTRL_DISABLE_OC_ID_USB11_48M;

        hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USBC;
        hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USB11;
        #endif
        hwp_idle->IDLE_CG_SW_SET  = 0x10;
        hwp_idle->IDLE_CG_SEL_SET = 0x10;
        hwp_idle->PD_PLL_SW_SET   = 0x10;
        hwp_idle->PD_PLL_SEL_SET  = 0x10;

        //hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_CLR_SYS_AHB_RST_ID_USBC;//bringup_tmp
    } else {
        //hwp_sysCtrl->REG_DBG = 0xA50001;//bringup_tmp

        hwp_idle->IDLE_CG_SW_CLR = 0x10;
        hwp_idle->IDLE_CG_SEL_CLR = 0x10;
        hwp_idle->PD_PLL_SW_CLR = 0x10;
        hwp_idle->PD_PLL_SEL_CLR = 0x10;

        hwp_analogReg->USB_DIGPHY_ANA1 = 0x3f800;

        hwp_analogReg->usb_reg2 |= ANALOG_REG_USB_PU_1V8 |
                                   ANALOG_REG_USB_PU_1V2 |
                                   ANALOG_REG_USB_PWR_ON;

        hwp_analogReg->usb_pll1 = ANALOG_REG_USB_PLL_VREG_BIT(8) |
                                  ANALOG_REG_USB_REG_RES_BIT(2) |
                                  ANALOG_REG_USB_PLL_CPBIAS_IBIT(4) |
                                  ANALOG_REG_USB_PLL_CPC2_IBIT(4) |
                                  ANALOG_REG_USB_PLL_CPR2_IBIT(4);

        hwp_analogReg->usb_pll2 = ANALOG_REG_USB_REFMULTI2_EN |
                                  ANALOG_REG_USB_PLL_CLK_960M_EN;

        hwp_analogReg->SDM_USBPLL_reg0 = ANALOG_REG_SDM_USBPLL_PU |
                                        ANALOG_REG_SDM_USBPLL_SDM_CLK_SEL_RST;

        hwp_analogReg->SDM_USBPLL_reg1 = 0x24ec4ec4;

        hwp_analogReg->SDM_USBPLL_reg2 = ANALOG_REG_SDM_USBPLL_DITHER_BYPASS |
                                         ANALOG_REG_SDM_USBPLL_SDM_RESET_TIME_SEL(1) |
                                         ANALOG_REG_SDM_USBPLL_SDMCLK_SEL_TIME_SEL(1) |
                                         ANALOG_REG_SDM_USBPLL_CLK_GEN_EN_REG |
                                         ANALOG_REG_SDM_USBPLL_CLKOUT_EN_COUNTER_SEL(2) |
                                         ANALOG_REG_SDM_USBPLL_LOCK_COUNTER_SEL(1);

        while (!(hwp_analogReg->usb_mon & ANALOG_REG_USB_PLL_LOCK)) {}
    }
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Connect (void)
{
    USB_Module_Enable (1);

    USB_LDO_Control (TRUE);

    hwp_analogReg->usb_reg0 = ANALOG_REG_USB_PHASE_SEL(2) |
                              ANALOG_REG_USB_CDR_GAIN(9);

    hwp_analogReg->usb_reg1 = ANALOG_REG_USB_VREF_IBIT(2) |
                              ANALOG_REG_USB_VREF_VBIT(4) |
                              ANALOG_REG_USB_V575M_SEL_BIT(4) |
                              ANALOG_REG_USB_V125M_SEL_BIT(4) |
                              ANALOG_REG_USB_HS_LVLOUT_BIT(0xf);

    hwp_analogReg->usb_reg2 |= ANALOG_REG_USB_PU_OTG |
                               ANALOG_REG_USB_PU_USB_DEV |
                               ANALOG_REG_USB_PU_PKD |
                               ANALOG_REG_USB_PU_IREF |
                               ANALOG_REG_USB_PU_HSRX |
                               ANALOG_REG_USB_PU_HSTX |
                               ANALOG_REG_USB_PU_LPTX;

    hwp_analogReg->usb_reg3 = 0;

    hwp_analogReg->usb_reg4 = ANALOG_REG_USB_IO33_ENABLE |
                              ANALOG_REG_USB_RES_TERM_BIT(2) |
                              ANALOG_REG_USB_LPTX_DRV_SEL(0xf);

    // USB UTMI+ Dig regs
    hwp_analogReg->USB_DIGPHY_ANA1 = 0;
    hwp_analogReg->USB_DIGPHY_ANA2 = 0;
    hwp_analogReg->USB_DIGPHY_ANA3 = (1 << 5) | (1 << 6);
    #if 0////bringup_tmp
    hwp_sysCtrl->REG_DBG = 0xA50001;
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_USBC;
    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_USBC;
    hwp_sysCtrl->CLK_OTHERS_ENABLE = SYS_CTRL_ENABLE_OC_ID_USBPHY;
    hwp_sysCtrl->CLK_OTHERS_1_ENABLE = SYS_CTRL_ENABLE_OC_ID_USB_UTMI_48M;
    #endif
    SCI_Sleep (10);

    usb_core_init();
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Disconnect (void)
{
    hwp_analogReg->usb_reg2 = 0;

    // Disable USB LDO
    USB_LDO_Control (FALSE);

    USB_Module_Enable (0);

    //SCI_TRACE_LOW:"USB LDO disable!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_COMMON_224_112_2_17_23_7_57_1458,(uint8*)"");

    //unregister usb pll
    SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_ClkReset (void)
{
    return;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_PowerSet (USB_POWER_E power)
{
    return;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_ErrorHandle (USB_ERROR_TYPE_E value)
{
    return;
}
/*****************************************************************************/
//  Description:    Call this function to control epx data in ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataInCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    return;
}

/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataOutCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    return;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
uint32 USB_EP1InDmaReq (uint32 *pBuf,uint32 len)
{
    return 0;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_RxDWordFromFIFO (
    uint32 *buf ,
    uint8 len ,
    uint8 ep_id)
{
    return TRUE ;
}       //Send data to FIFO ;
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_TxDWordToFIFO (
    uint32 *buf ,
    uint8 len ,
    uint8 ep_id)
{

    return TRUE ;
}
