#include "usb_platform_uws6121e.h"
//#include "osi_api.h"
//#include "osi_log.h"
//#include "hwregs.h"
//#include "drv_names.h"
//#include "hal_chip.h"
//#include "osi_clock.h"
//#include "hal_adi_bus.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ldo_drvapi.h>
#include <deepsleep_drvapi.h>
char plogbuf[30];
typedef struct
{
    bool clk_constrain;
    //osiClockConstrainRegistry_t usb_clk;
    //osiPmSource_t *pm_lock;
} udcPlat_t;

static void _usbClkLock(udcPlat_t *p)
{
    if (!p->clk_constrain)
    {
        p->clk_constrain = true;
        //osiRequestSysClkActive(&p->usb_clk);
    }
}

static void _usbClkUnlock(udcPlat_t *p)
{
    if (p->clk_constrain)
    {
        //osiReleaseClk(&p->usb_clk);
        p->clk_constrain = false;
    }
}

static void _setpower_uws6121e(udc_t *udc, bool on)
{
    udcPlat_t *p;
    REG_PWRCTRL_USB_PWR_CTRL_T usb_pwr_ctrl;
    REG_PWRCTRL_USB_PWR_STAT_T usb_pwr_stat;
	USB_LOG_TRACE("_setpower_6121e  on:%d:\n",on);


    //halPmuSwitchPower(HAL_POWER_USB, on, on);
    //halPmuSwitchPower(HAL_POWER_ANALOG, true, on);
    p = (udcPlat_t *)udc->platform_priv;

    if (!on)
    {
        _usbClkUnlock(p);
         LDO_TurnOffLDO(LDO_LDO_USB);
        //osiPmWakeUnlock(p->pm_lock);
        //SCI_UPM_EnableDeepSleep(TRUE);//do not enter pm1 even usb is not working
        hwp_pwrctrl->usb_pwr_ctrl = 0;
        do
        {
            usb_pwr_stat.v = hwp_pwrctrl->usb_pwr_stat;
        } while (((usb_pwr_stat.v) & 0x3) == 0x3);
    }
    else
    {
         LDO_TurnOnLDO(LDO_LDO_USB);
         LDO_TurnOnLDO(LDO_LDO_ANA);

        usb_pwr_ctrl.v = 1;
        hwp_pwrctrl->usb_pwr_ctrl = usb_pwr_ctrl.v;

        do
        {
            usb_pwr_stat.v = hwp_pwrctrl->usb_pwr_stat;
        } while (((usb_pwr_stat.v) & 0x3) != 0x3);
        //osiPmWakeLock(p->pm_lock);
        SCI_UPM_EnableDeepSleep(FALSE);

        _usbClkLock(p);
    }

}
static void _setpower_for_vcom(bool on)
{
    REG_PWRCTRL_USB_PWR_CTRL_T usb_pwr_ctrl;
    REG_PWRCTRL_USB_PWR_STAT_T usb_pwr_stat;
    USB_LOG_TRACE("_setpower_for_vcom  on:%d:\n",on);

    if (!on)
    {
         LDO_TurnOffLDO(LDO_LDO_USB);

        SCI_UPM_EnableDeepSleep(TRUE);
        hwp_pwrctrl->usb_pwr_ctrl = 0;
        do
        {
            usb_pwr_stat.v = hwp_pwrctrl->usb_pwr_stat;
        } while (((usb_pwr_stat.v) & 0x3) == 0x3);
    }
    else
    {
         LDO_TurnOnLDO(LDO_LDO_USB);
         LDO_TurnOnLDO(LDO_LDO_ANA);

        usb_pwr_ctrl.v = 1;
        hwp_pwrctrl->usb_pwr_ctrl = usb_pwr_ctrl.v;

        do
        {
            usb_pwr_stat.v = hwp_pwrctrl->usb_pwr_stat;
        } while (((usb_pwr_stat.v) & 0x3) != 0x3);

        SCI_UPM_EnableDeepSleep(FALSE);
    }

}

static void _setclock_uws6121e(udc_t *udc, bool on)
{
    unsigned int regval = 0;

    REG_SYS_CTRL_SYSCTRL_REG0_T sysctrl_reg0;
    REG_ANALOG_G2_ANALOG_USB20_USB20_UTMI_CTL1_T analog_usb20_usb20_utmi_ctl1;
//提高驱动能力，没有作用
/*
	REG_ANALOG_G2_ANALOG_USB20_USB20_TRIMMING_T analog_usb20_usb20_trimming;
	analog_usb20_usb20_trimming.v = hwp_analogG2->analog_usb20_usb20_trimming;
	analog_usb20_usb20_trimming.b.analog_usb20_usb20_tunehsamp = 0x2;
	hwp_analogG2->analog_usb20_usb20_trimming = analog_usb20_usb20_trimming.v;
*/
	USB_LOG_TRACE("_setclock_uws6121e  on:%d:\n",on);
    if (on)
    {
        hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x5);

        /* UTMI width 16bit */
        sysctrl_reg0.v = hwp_sysCtrl->sysctrl_reg0;
        sysctrl_reg0.b.usb20_utmi_width_sel = 1;
        hwp_sysCtrl->sysctrl_reg0 = sysctrl_reg0.v;

        /* UTMI data width 16bit && clock select 30MHz */
        analog_usb20_usb20_utmi_ctl1.v = hwp_analogG2->analog_usb20_usb20_utmi_ctl1;
        analog_usb20_usb20_utmi_ctl1.b.analog_usb20_usb20_databus16_8 = 1;
        hwp_analogG2->analog_usb20_usb20_utmi_ctl1 = analog_usb20_usb20_utmi_ctl1.v;

        /* vbus valid */
        sysctrl_reg0.v = hwp_sysCtrl->sysctrl_reg0;
        sysctrl_reg0.b.usb20_vbus_valid_sel = 1;
        hwp_sysCtrl->sysctrl_reg0 = sysctrl_reg0.v;

        analog_usb20_usb20_utmi_ctl1.v = hwp_analogG2->analog_usb20_usb20_utmi_ctl1;
        regval = analog_usb20_usb20_utmi_ctl1.v;
        regval &= ~0xFFFF;
        regval |= 0x3;
        hwp_analogG2->analog_usb20_usb20_utmi_ctl1 = regval;
        analog_usb20_usb20_utmi_ctl1.v = hwp_analogG2->analog_usb20_usb20_utmi_ctl1;

        sysctrl_reg0.v = hwp_sysCtrl->sysctrl_reg0;
        sysctrl_reg0.b.usb20_iddig = 1;
        hwp_sysCtrl->sysctrl_reg0 = sysctrl_reg0.v;
    }
    else
    {
        sysctrl_reg0.v = hwp_sysCtrl->sysctrl_reg0;
        sysctrl_reg0.b.usb20_utmi_width_sel = 0;
        hwp_sysCtrl->sysctrl_reg0 = sysctrl_reg0.v;

        analog_usb20_usb20_utmi_ctl1.v = hwp_analogG2->analog_usb20_usb20_utmi_ctl1;
        analog_usb20_usb20_utmi_ctl1.b.analog_usb20_usb20_databus16_8 = 0;
        hwp_analogG2->analog_usb20_usb20_utmi_ctl1 = analog_usb20_usb20_utmi_ctl1.v;

        sysctrl_reg0.v = hwp_sysCtrl->sysctrl_reg0;
        sysctrl_reg0.b.usb20_vbus_valid_sel = 0;
        hwp_sysCtrl->sysctrl_reg0 = sysctrl_reg0.v;

        // TO DO
    }
}

static void _usbEnable_uws6121e(udc_t *udc)
{
    int i;

    /* reset usb phy */ /* reset usb controller */
    REG_LPS_APB_AON_IRAM_CTRL_T aon_iram_ctrl;
    REG_LPS_APB_RESET_SYS_SOFT_T reset_sys_soft;
    REG_SYS_CTRL_AON_CLOCK_EN0_T aon_clock_en0;
	USB_LOG_TRACE("_usbEnable_6121e\n");

    aon_iram_ctrl.v = hwp_lpsApb->aon_iram_ctrl;

    reset_sys_soft.v = hwp_lpsApb->reset_sys_soft;

    aon_iram_ctrl.b.__3_3 = 1;
    hwp_lpsApb->aon_iram_ctrl = aon_iram_ctrl.v;
    aon_iram_ctrl.v = hwp_lpsApb->aon_iram_ctrl;

    reset_sys_soft.b.usb_sys_soft_reset = 1;
    hwp_lpsApb->reset_sys_soft = reset_sys_soft.v;
    reset_sys_soft.v = hwp_lpsApb->reset_sys_soft;

    for (i = 0; i < 10000; i++)
        ;

    aon_iram_ctrl.b.__3_3 = 0;
    hwp_lpsApb->aon_iram_ctrl = aon_iram_ctrl.v;
    reset_sys_soft.b.usb_sys_soft_reset = 0;
    hwp_lpsApb->reset_sys_soft = reset_sys_soft.v;
    // aon_iram_ctrl.v = hwp_lpsApb->aon_iram_ctrl;
    // reset_sys_soft.v = hwp_lpsApb->reset_sys_soft;
    aon_clock_en0.v = hwp_sysCtrl->aon_clock_en0;
    aon_clock_en0.b.usb_32k_en = 1;
    hwp_sysCtrl->aon_clock_en0 = aon_clock_en0.v;

}

static void _usbDisable_uws6121e(udc_t *udc)
{
    REG_LPS_APB_AON_IRAM_CTRL_T aon_iram_ctrl;
    REG_LPS_APB_RESET_SYS_SOFT_T reset_sys_soft;
    REG_SYS_CTRL_AON_CLOCK_EN0_T aon_clock_en0;
	USB_LOG_TRACE("_usbDisable_6121e\n");

    aon_iram_ctrl.v = hwp_lpsApb->aon_iram_ctrl;

    reset_sys_soft.v = hwp_lpsApb->reset_sys_soft;

    aon_iram_ctrl.b.__3_3 = 0;
    hwp_lpsApb->aon_iram_ctrl = aon_iram_ctrl.v;
    aon_iram_ctrl.v = hwp_lpsApb->aon_iram_ctrl;

    reset_sys_soft.b.usb_sys_soft_reset = 0;
    hwp_lpsApb->reset_sys_soft = reset_sys_soft.v;
    reset_sys_soft.v = hwp_lpsApb->reset_sys_soft;

    aon_clock_en0.v = hwp_sysCtrl->aon_clock_en0;
    aon_clock_en0.b.usb_32k_en = 0;
    hwp_sysCtrl->aon_clock_en0 = aon_clock_en0.v;
}

static void _suspend_uws6121e(udc_t *udc)
{
    udcPlat_t *p = (udcPlat_t *)udc->platform_priv;
    _usbClkUnlock(p);
	USB_LOG_TRACE("_suspend_6121e\n");
    //SCI_UPM_EnableDeepSleep(TRUE); //do not enter pm1 even usb is suspended
}

static void _resume_uws6121e(udc_t *udc)
{
    udcPlat_t *p = (udcPlat_t *)udc->platform_priv;
	USB_LOG_TRACE("_resume_6121e\n");
    SCI_UPM_EnableDeepSleep(FALSE);
    _usbClkLock(p);
}

bool udcPlatformInit(udc_t *udc)
{
    udcPlat_t *p = (udcPlat_t *)calloc(1, sizeof(udcPlat_t));
    if (p == NULL)
        return false;
    /*
    p->pm_lock = osiPmSourceCreate(DRV_NAME_USB20, NULL, NULL);
    if (p->pm_lock == NULL)
    {
        osiExitCritical(critical);
        free(p);
        return false;
    }
    p->usb_clk.tag = HAL_NAME_USB;
    */
    p->clk_constrain = false;

    udc->plat_ops.setpower = _setpower_uws6121e;
    udc->plat_ops.setclk = _setclock_uws6121e;
    udc->plat_ops.enable = _usbEnable_uws6121e;
    udc->plat_ops.disable = _usbDisable_uws6121e;
    udc->plat_ops.suspend = _suspend_uws6121e;
    udc->plat_ops.resume = _resume_uws6121e;

    udc->platform_priv = (unsigned long)p;
    return true;
}

void udcPlatformExit(udc_t *udc)
{
    udcPlat_t *p;
    if (udc->platform_priv == 0)
        return;
    p = (udcPlat_t *)udc->platform_priv;
    udc->platform_priv = 0;
    //osiPmSourceDelete(p->pm_lock);
    free(p);
}

void udcPlatformStart(void)
{
    _setpower_for_vcom(true);
    _usbEnable_uws6121e(0);
    _setclock_uws6121e(0,true);
}

void udcPlatformStop(void)
{
    _usbDisable_uws6121e(0);
    _setclock_uws6121e(0,false);
    _setpower_for_vcom(false);
}

