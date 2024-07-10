
#include "sci_types.h"
#include "sys_ctrl.h"
#include "global_macros.h"
#include "analog_reg.h"
#include "clk_div.h"
#include "cp_clkrst.h"
#include "cp_pwrctrl.h"
#include "tgt_ap_clock_config.h"

#define hwp_sysCtrlAp	((HWP_SYS_CTRL_T*)(REG_SYS_CTRL_BASE))

struct clock_config {
	/* PLL Freqs (MHz) */
	uint32 cpu_pll_freq;	/* 05, 06 */
	uint32 bus_pll_freq;	/* 25, 26 */
	uint32 mem_pll_freq;	/* 65, 66 */
	//uint32 PLL_FREQ_USB;	/* 85, 86 */

	/* AP sysctrl */
	uint16 ap_cpu_clk;
	uint16 axi_bus_clk;
	uint16 ahb_bus_clk;
	uint16 apb_bus_clk;
	uint16 mem_bus_clk;
	uint16 flash_ctrl_clk;   /* spi flash controller */
	uint16 flash_dev_clk;   /* spi flash device */
	uint16 flash_dev1_clk;  /* spi flash device 1 */
};

/* PLL Freqs (MHz) */
const static struct clock_config clock_default_config = {
	_TGT_AP_PLL_CPU_FREQ,   // .cpu_pll_freq = 
	_TGT_AP_PLL_BUS_FREQ,   // .bus_pll_freq = 
	_TGT_AP_PLL_MEM_FREQ,  // .mem_pll_freq = 
	//0, PLL_FREQ_USB;

	/* AP sysctrl */
	_TGT_AP_CLK_CPU,    // .ap_cpu_clk	= 
	_TGT_AP_CLK_AXI,    // .axi_bus_clk	= 
	_TGT_AP_CLK_AHB,   // .ahb_bus_clk	= 
	_TGT_AP_CLK_APB,   // .apb_bus_clk	= 
	_TGT_AP_CLK_MEM,  // .mem_bus_clk	= 
	_TGT_AP_CLK_FLH_CTRL, // .flash_ctrl_clk	= 
	_TGT_AP_CLK_FLH_DEV0, // .flash_dev_clk	= 
	_TGT_AP_CLK_FLH_DEV1, // .flash_dev1_clk = 
};

#if 0
static int usb_in_use = 0;

static void check_usb_usage(void)
{
	usb_in_use = 0;
}
#endif

static const struct clock_config *clock_config;

#define REG_WRITE_ENABLE	0xA50001
#define REG_WRITE_DISABLE	0xA50000

void udelay(uint32 usec)
{
	int i;
	
	for (i = 0; i < usec; i++);
}

static void sys_reg_write_enable(void)
{
	hwp_sysCtrlAp->REG_DBG = REG_WRITE_ENABLE;
}

static void sys_reg_write_disable(void)
{
	//hwp_sysCtrlAp->REG_DBG = REG_WRITE_DISABLE;
}

static void sys_shutdown_pll(void)
{
	sys_reg_write_enable();
	hwp_sysCtrlAp->Pll_Ctrl =
	    SYS_CTRL_PLL_ENABLE_POWER_DOWN | SYS_CTRL_PLL_LOCK_RESET;

	 /*MEMPLL*/
	hwp_sysCtrlAp->Cfg_MISC1_CFG &= ~SYS_CTRL_MEMPLL_ENABLE;
	sys_reg_write_disable();
}

static void sys_pll_lowpower_cfg(void)
{
	sys_reg_write_enable();
	hwp_analogReg->APLL_reg0 |= ANALOG_REG_APLL_LP_MODE_EN;
	hwp_analogReg->MEMPLL_reg0 |= ANALOG_REG_MEMPLL_LP_MODE_EN;
	sys_reg_write_disable();
	rprintf("sys pll lp mode enable!\n");
}

static void sys_pll_module_disable(void)
{
	sys_reg_write_enable();
	/* camera pll disable */
	hwp_sysCtrlAp->CLK_OTHERS_DISABLE = SYS_CTRL_DISABLE_OC_ID_CAM;
	rprintf("camera module pll disable!\n");
	/* lcd dsi module pll disable */
	hwp_analogReg->SDM_DSIPLL_reg0 |= ANALOG_REG_SDM_DSIPLL_PU_PLL_DR;
	hwp_analogReg->SDM_DSIPLL_reg0 &= ~ANALOG_REG_SDM_DSIPLL_PU_PLL_REG;
	rprintf("lcd dsi module pll disable!\n");
	sys_reg_write_disable();
}

static void sys_setup_pll(void)
{
	uint32 mask;
	uint32 locked;
	int cnt = 10;	//10us, according to IC, the pll must be locked

	sys_reg_write_enable();
	/*PLL*/
	hwp_sysCtrlAp->Pll_Ctrl =
	    SYS_CTRL_PLL_ENABLE | SYS_CTRL_PLL_LOCK_RESET_NO_RESET;
	mask = SYS_CTRL_PLL_LOCKED_MASK;
	locked = SYS_CTRL_PLL_LOCKED_LOCKED;
	while (((hwp_sysCtrlAp->Sel_Clock & mask) != locked) && cnt) {
		udelay(1);
		cnt--;
	}
	/*mempll */
	hwp_sysCtrlAp->Cfg_MISC1_CFG |= SYS_CTRL_MEMPLL_ENABLE;
	mask = SYS_CTRL_MEMPLL_LOCKED_H;
	locked = SYS_CTRL_MEMPLL_LOCKED_H;
	while (((hwp_sysCtrlAp->Sel_Clock & mask) != locked) && cnt) {
		udelay(1);
		cnt--;
	}

	/*select fast */
	hwp_sysCtrlAp->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE;

	hwp_sysCtrlAp->Sel_Clock &= (~SYS_CTRL_SYS_SEL_FAST_SLOW);
	hwp_sysCtrlAp->Sel_Clock &= (~SYS_CTRL_SOFT_SEL_MEM_BRIDGE);
	hwp_sysCtrlAp->Sel_Clock &= (~SYS_CTRL_SOFT_SEL_SPIFLASH);
	hwp_sysCtrlAp->Sel_Clock &= (~SYS_CTRL_SOFT_SEL_SPIFLASH1);
	sys_reg_write_disable();
}

static unsigned int pll_freq_to_divider(const char *name, unsigned int freq)
{
	unsigned long long div = (1 << 25);

	div = div * freq;
	div = div / 52;

	rprintf("%s pll: %d MHz, div: 0x%x\n", name, freq, (int)div);
	return div;
}

static void sys_setup_clk(void)
{
	uint32 div;
	// Init clock gating mode
	/*hwp_sysCtrlAp->CLK_SYS_AXI_MODE = 0;
	   hwp_sysCtrlAp->CLK_SYS_AHB_MODE = 0;
	   hwp_sysCtrlAp->CLK_AP_APB_MODE = 0;
	   hwp_sysCtrlAp->CLK_OTHERS_MODE = 0;
	   hwp_sysCtrlAp->CLK_OTHERS_1_MODE = 0; */

	// set MEM PLL freq
	div = pll_freq_to_divider("MEM", clock_config->mem_pll_freq);
	hwp_analogReg->SDM_MEMPLL_reg1 = div;

	// set CPU PLL freq
	div = pll_freq_to_divider("AP", clock_config->cpu_pll_freq);
	hwp_analogReg->SDM_APLL_reg1 = div;

	sys_reg_write_enable();
	// Init module frequency
	div = clk_get_regval_by_freq(clock_config->cpu_pll_freq,
                           clock_config->ap_cpu_clk);
	hwp_sysCtrlAp->CFG_PLL_AP_CPU_DIV =
			div | SYS_CTRL_CFG_PLL_AP_CPU_DIV_UPDATE;

	div = clk_get_regval_by_freq(clock_config->cpu_pll_freq,
                           clock_config->axi_bus_clk);
	hwp_sysCtrlAp->CFG_PLL_SYS_AXI_DIV =
			div | SYS_CTRL_CFG_PLL_SYS_AXI_DIV_UPDATE;

	div = clk_get_regval_by_freq(clock_config->axi_bus_clk,
			clock_config->apb_bus_clk);
	hwp_sysCtrlAp->CFG_PLL_SYS_AHB_APB_DIV =
			div | SYS_CTRL_CFG_PLL_AP_APB_DIV_UPDATE;

	div = clk_get_regval_by_freq(clock_config->mem_pll_freq,
                           clock_config->mem_bus_clk * 2);
	hwp_sysCtrlAp->CFG_PLL_MEM_BRIDGE_DIV =
			div | SYS_CTRL_CFG_PLL_MEM_BRIDGE_DIV_UPDATE;

	div = clk_get_regval_by_freq(clock_config->axi_bus_clk,
			clock_config->flash_ctrl_clk);
	hwp_sysCtrlAp->CFG_PLL_SYS_SPIFLASH_DIV =
			div | SYS_CTRL_CFG_PLL_SYS_SPIFLASH_DIV_UPDATE;

	div = clk_get_regval_by_freq(clock_config->cpu_pll_freq,
			clock_config->flash_dev_clk);
	hwp_sysCtrlAp->CFG_PLL_SPIFLASH_DIV =
			div | SYS_CTRL_CFG_PLL_SPIFLASH_DIV_UPDATE;
	sys_reg_write_disable();
}

static void print_sys_reg(char *name, uint32 clk, uint32 div)
{
	rprintf("%s clk: %d MHz, div: 0x%x\n", name, clk, div);
}

static void sys_dump_clk(void)
{
	print_sys_reg("CPU", clock_config->ap_cpu_clk,
				hwp_sysCtrlAp->CFG_PLL_AP_CPU_DIV);

	print_sys_reg("AXI", clock_config->axi_bus_clk,
				hwp_sysCtrlAp->CFG_PLL_SYS_AXI_DIV);

	print_sys_reg("APB", clock_config->apb_bus_clk,
				hwp_sysCtrlAp->CFG_PLL_SYS_AHB_APB_DIV);

	print_sys_reg("MEM", clock_config->mem_bus_clk,
				hwp_sysCtrlAp->CFG_PLL_MEM_BRIDGE_DIV);

	print_sys_reg("SPIFLHC", clock_config->flash_ctrl_clk,
				hwp_sysCtrlAp->CFG_PLL_SYS_SPIFLASH_DIV);

	print_sys_reg("SPIFLHD", clock_config->flash_dev_clk,
				hwp_sysCtrlAp->CFG_PLL_SPIFLASH_DIV);
}

static void sys_bus_auto_sleep_cfg(void)
{
	sys_reg_write_enable();
	hwp_sysCtrlAp->Cfg_FORCE_LP_MODE_LP = SYS_CTRL_CFG_MODE_LP_A5
					| SYS_CTRL_CFG_MODE_LP_AHB
					| SYS_CTRL_CFG_MODE_LP_CONNECT
					| SYS_CTRL_CFG_MODE_LP_CP_A5
					| SYS_CTRL_CFG_MODE_LP_PSRAM
					| SYS_CTRL_CFG_MODE_LP_SPIFLASH
					| SYS_CTRL_CFG_MODE_LP_SPIFLASH1;
	sys_reg_write_disable();
	rprintf("sys bus auto sleep enable! \n");
}

static void sys_clk_autogate_cfg(void)
{
	sys_reg_write_enable();
	hwp_sysCtrlAp->CLK_SYS_AXI_MODE &= (~SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_MANUAL);
	hwp_sysCtrlAp->CLK_SYS_AHB_MODE &= ((~SYS_CTRL_MODE_SYS_AHB_CLK_ID_GOUDA_MANUAL) & \
					(~SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_MANUAL));
	hwp_sysCtrlAp->CLK_AP_APB_MODE &= ~(SYS_CTRL_MODE_CLK_AP_APB(0x1FFFFFF) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_REV2_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_UART1_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_UART2_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_UART3_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_REV0_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_REV1_MANUAL) & \
				(~SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_MANUAL));
	hwp_sysCtrlAp->CLK_AIF_APB_MODE &= ~(SYS_CTRL_MODE_CLK_AIF_APB(0xFFF) & \
				(~SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_MANUAL) & \
				(~SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_MANUAL) & \
				(~SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_MANUAL) & \
				(~SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_MANUAL) & \
				(~SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_MANUAL));
	hwp_sysCtrlAp->CLK_AON_APB_MODE &= (
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_SCID1_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_SCID2_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_MANUAL) & \
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_I2C_MANUAL));
	 hwp_sysCtrlAp->CLK_OTHERS_MODE &= ((~SYS_CTRL_MODE_OC_ID_HOST_UART_MANUAL) & \
				(~SYS_CTRL_MODE_OC_ID_BCK1_MANUAL) & \
				(~SYS_CTRL_MODE_OC_ID_BCK2_MANUAL));
	sys_reg_write_disable();
	rprintf("sys clk auto gating enable! \n");
}

#define hwp_zspClkrst  hwp_clkrst
static void zsp_bus_clk_autogate_cfg(void)
{
	hwp_zspClkrst->ZSP_BUSLPMC &= (~CP_CLKRST_BUSLPMC_SW2);
}

#define hwp_CPpwrctrl hwp_pwrctrl
static void cp_pwrctrl_cfg(void)
{
	hwp_CPpwrctrl->SLOW_SYS_CLK_SEL_HWEN |= CP_PWRCTRL_HWEN;
	hwp_CPpwrctrl->SLOW_CLK_SEL_HWEN |= CP_PWRCTRL_HWEN;
}

#if 0
/*
 * we initialize usb clock, but this won't cause the usb clock jitter,
 * because wo don't setup the usb pll
 */
static void usb_clock_pre_init(void)
{
}
#endif

static void clock_setup_init(void)
{
	clock_config = &clock_default_config;
}

#define CLOCK_VER_MAJOR		2
#define CLOCK_VER_MINOR		0

int clock_init(void)
{
	rprintf("Init Clock ...\n");
	clock_setup_init();
	//rprintf("Clock config ver: %d.%d\n", CLOCK_VER_MAJOR, CLOCK_VER_MINOR);

	sys_shutdown_pll();

	sys_setup_pll();
	sys_setup_clk();

	sys_dump_clk();

#ifndef CONFIG_RDA_FPGA
	sys_pll_lowpower_cfg();
	sys_bus_auto_sleep_cfg();
	sys_clk_autogate_cfg();
	zsp_bus_clk_autogate_cfg();
	cp_pwrctrl_cfg();
	sys_pll_module_disable();
#endif
	return 0;
}
