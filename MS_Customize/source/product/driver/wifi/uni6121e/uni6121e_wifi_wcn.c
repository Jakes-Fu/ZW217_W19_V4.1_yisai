#include "uni6121e_wifi_wcn.h"
#include "ldo_drvapi.h"

static void uni8910_wcnClkEnable()
{
	// enable 26M for wcn
	hwp_clkrst->CLKEN_BB_SYSCTRL_SET = 0x1;
	hwp_pwrctrl->BTFM_PWR_CTRL = 0x2; // power on digital
	//halClock26MRequest(CLK_26M_USER_WCN);
	PMIC_26M_open(BIT_2);
	// setting BT clk
	hwp_sysCtrl->CFG_PLL_SYS_AHB_BTFM_DIV = 0x3; // 8 div 100MHZ
	hwp_sysCtrl->CFG_PLL_SYS_AHB_BTFM_DIV |= SYS_CTRL_CFG_PLL_AHB_BTFM_DIV_UPDATE;/*update*/

}
static void uni8910_wcnClkDisable()
{
    //halClock26MRelease(CLK_26M_USER_WCN);
    PMIC_26M_close(BIT_2);//PMIC_26M_USED_BY_WCN is bit_2
}

static void uni8910_wcnPwrOn()
{
    while (!(hwp_pwrctrl->BTFM_PWR_STAT & CP_PWRCTRL_BTFM_STABLE))
        ;
    if (hwp_pwrctrl->BTFM_PWR_STAT != 0x3)
    {
        hwp_pwrctrl->BTFM_PWR_CTRL = 0x2;
    }
    while (hwp_pwrctrl->BTFM_PWR_STAT != 0x3)
        ;

    SCI_Sleep(100);//100ms
    //WCN RF Power
    LDO_SetVoltValue(LDO_LDO_BTPHY, 1700);
    LDO_TurnOnLDO(LDO_LDO_BTPHY);
    SCI_Sleep(100);//100ms
}
static void uni8910_wcnPwrOff()
{
    //WCN RF Power Off
    LDO_TurnOffLDO(LDO_LDO_BTPHY);
    //WCN Power Off
    hwp_pwrctrl->BTFM_PWR_CTRL = 0x1;

    SCI_Sleep(100);
    SCI_TRACE_LOW("uni8910wifirx, pwroff");
}

static void uni8910_wcnRfInit()
{
    hwp_wcnRfIf->sys_control = 0x261;
    hwp_wcnRfIf->bt_control = 0x27;
    hwp_wcnRfIf->inter_freq_setting = 0x8002BC;
    hwp_wcnRfIf->pga_setting0 = 0x1820;
    hwp_wcnRfIf->pga_setting1 = 0xEC222;
    hwp_wcnRfIf->mdll_setting = 0x61c8;

    hwp_wcnRfIf->power_dr = 0x40400;
    hwp_wcnRfIf->power_reg = 0x40400;

    hwp_wcnRfIf->lna_rmx_setting = 0x80C1D8;
    hwp_wcnRfIf->bt_dac_setting = 0x1208A;
    hwp_wcnRfIf->bt_txrf_setting = 0x1e000777;
    hwp_wcnRfIf->new_add = 0x23;
    hwp_wcnRfIf->vco_setting = 0x197881C;
    hwp_wcnRfIf->pll_setting0 = 0x22211000;
    hwp_wcnRfIf->pll_setting1 = 0x1555C81A;
    hwp_wcnRfIf->pll_setting2 = 0xD000028;
    hwp_wcnRfIf->pll_cal_setting0 = 0x5001cb;
    hwp_wcnRfIf->rxflt_cal_setting0 = 0x4B9027F;
    hwp_wcnRfIf->rxflt_cal_setting2 = 0x472b2b27;
    hwp_wcnRfIf->pll_sdm_setting0 = 0x144800;

    hwp_wcnModem->sw_swap_dccal = 0x5230;
    hwp_wcnRfIf->rxflt_setting = 0xb361112;
    hwp_wcnRfIf->pga_setting0 = 0x1820;
    hwp_wcnModem->sw_ctrl = 0x4170;
    hwp_wcnModem->tx_polar_mode_ctl = 0xb;
    hwp_wcnModem->tx_gain_ctrl = 0xA37E;
    hwp_wcnModem->dpsk_gfsk_tx_ctrl = 0xD048;

    hwp_wcnRfIf->bt_tx_gain_table_0 = 0x11C01195;
    hwp_wcnRfIf->bt_tx_gain_table_1 = 0x112E139D;
    hwp_wcnRfIf->bt_tx_gain_table_2 = 0x154C1700;
    hwp_wcnRfIf->bt_tx_gain_table_3 = 0x91E1D1E;

    hwp_wcnRfIf->ana_reserved = 0xF07900;
    hwp_wcnRfIf->dig_reserved = 0xD244;

    hwp_wcnRfIf->bt_gain_table_0 = 0x00000010;
    hwp_wcnRfIf->bt_gain_table_1 = 0x00000810;
    hwp_wcnRfIf->bt_gain_table_2 = 0x00001010;
    hwp_wcnRfIf->bt_gain_table_3 = 0x00001810;
    hwp_wcnRfIf->bt_gain_table_4 = 0x00001820;
    hwp_wcnRfIf->bt_gain_table_5 = 0x00001830;
    hwp_wcnRfIf->bt_gain_table_6 = 0x00001840;
    hwp_wcnRfIf->bt_gain_table_7 = 0x00001841;
    hwp_wcnRfIf->bt_gain_table_8 = 0x00005041;
    hwp_wcnRfIf->bt_gain_table_9 = 0x00005043;
    hwp_wcnRfIf->bt_gain_table_a = 0x0000D043;
    hwp_wcnRfIf->bt_gain_table_b = 0x0001D043;
    hwp_wcnRfIf->bt_gain_table_c = 0x0001D047;
    hwp_wcnRfIf->bt_gain_table_d = 0x0001F847;
    hwp_wcnRfIf->bt_gain_table_e = 0x0001F84B;
    hwp_wcnRfIf->bt_gain_table_f = 0x0001F84F;

    hwp_wcnBtReg->timgencntl = 0x81c200c8;
    hwp_wcnRfIf->fm_gain_table_7 = 0x2FFC2FFE;
    hwp_wcnRfIf->fm_gain_table_6 = 0x2FF42FF8;
    hwp_wcnRfIf->fm_gain_table_5 = 0x27E02FF0;
    hwp_wcnRfIf->fm_gain_table_4 = 0x27C027D0;
    hwp_wcnRfIf->fm_gain_table_3 = 0x15401DC0;
    hwp_wcnRfIf->fm_gain_table_2 = 0x12C01340;
    hwp_wcnRfIf->fm_gain_table_1 = 0x10401240;
    hwp_wcnRfIf->fm_gain_table_0 = 0x10201030;

    // wifi set AGC table
    hwp_wcnRfIf->wf_gain_table_f = 0x1F84F;
    hwp_wcnRfIf->wf_gain_table_e = 0x1F84D;
    hwp_wcnRfIf->wf_gain_table_d = 0x1F84C;
    hwp_wcnRfIf->wf_gain_table_c = 0x1F848;
    hwp_wcnRfIf->wf_gain_table_b = 0x1F844;
    hwp_wcnRfIf->wf_gain_table_a = 0x1F840;
    hwp_wcnRfIf->wf_gain_table_9 = 0x1F830;
    hwp_wcnRfIf->wf_gain_table_8 = 0x1F820;
    hwp_wcnRfIf->wf_gain_table_7 = 0x1F810;
    hwp_wcnRfIf->wf_gain_table_6 = 0x1F800;
    hwp_wcnRfIf->wf_gain_table_5 = 0x17000;
    hwp_wcnRfIf->wf_gain_table_4 = 0x14800;
    hwp_wcnRfIf->wf_gain_table_3 = 0x14000;
    hwp_wcnRfIf->wf_gain_table_2 = 0xA800;
    hwp_wcnRfIf->wf_gain_table_1 = 0xA000;
    hwp_wcnRfIf->wf_gain_table_0 = 0x8000;


    hwp_wcnRfIf->adc_setting0 = 0x620; // change adc_clk_edge to 0
    hwp_wcnWlan->phy_reg_offset_addr = 0xff;
    hwp_wcnWlan->phy_reg_write_data = 0x1;

    hwp_wcnWlan->phy_reg_offset_addr = 0x0;
    hwp_wcnWlan->phy_reg_write_data = 0xfb1f810;

    hwp_wcnWlan->phy_reg_offset_addr = 0x5;
    // hwp_wcnWlan->phy_reg_write_data = 0xb020a452;
    hwp_wcnWlan->phy_reg_write_data = 0xA820a452;

    // Set the initial gain to F
    hwp_wcnWlan->phy_reg_offset_addr = 0x0C;
    hwp_wcnWlan->phy_reg_write_data = 0xF1585800;

    hwp_wcnRfIf->sys_control = 0x263;
    SCI_SLEEP(5);
    SCI_TRACE_LOW("uni8910wifirx, rfInit");
}

void uni8910_wcnStart()
{

    uni8910_wcnClkEnable();
    uni8910_wcnPwrOn();
    uni8910_wcnRfInit();
}
void uni8910_wcnStop()
{
    uni8910_wcnClkDisable();
    uni8910_wcnPwrOff();

}

