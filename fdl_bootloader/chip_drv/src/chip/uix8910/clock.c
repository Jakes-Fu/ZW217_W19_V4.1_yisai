
#include "sci_types.h"
#include "sys_ctrl.h"
#include "global_macros.h"
#include "analog_reg.h"
#include "clk_div.h"
#include "cp_clkrst.h"
#include "cp_idle.h"
#include "tgt_ap_clock_config.h"
#include "timer_drvapi.h"
//#include "arm_uart.h"

#define hwp_sysCtrlAp	((HWP_SYS_CTRL_T*)(REG_SYS_CTRL_BASE))
#define UART2_BAUD_REG   	(0x8817008)

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
	1000,  //800,    // .cpu_pll_freq
	800,    // .bus_pll_freq
	800,    // .mem_pll_freq

	/* AP sysctrl */
	500,    //400,    // .ap_cpu_clk =
	400,    // .axi_bus_clk =
	200,    // .ahb_bus_clk =
	200,    // .apb_bus_clk =
	_TGT_AP_CLK_MEM,    // .mem_bus_clk =
	200,    // .flash_ctrl_clk =
	200,    // .flash_dev_clk =
	0,       // .flash_dev1_clk =
};

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
{    //sys reg_dbg all enable
	//hwp_sysCtrlAp->REG_DBG = REG_WRITE_DISABLE;
}

static void sys_slowclk_sel(void)
{
	hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 0x0;
	hwp_idle->SLOW_CLK_SEL_HWEN = 0x0;
	sys_reg_write_enable();
	hwp_sysCtrlAp->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
	sys_reg_write_disable();
}

static void sys_clkdisable_init(void)
{
	sys_reg_write_enable();
	hwp_sysCtrlAp->CLK_SYS_AXI_DISABLE = SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_SDMMC1;   /*disable sdmmc1,as 200MHz when pll open,the clk will be error*/
	sys_reg_write_disable();
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

static void sys_setup_pll(void)
{
	uint32 mask;
	uint32 locked;
	int32 cnt = 10;	//10us, according to IC, the pll must be locked

	sys_reg_write_enable();

	// enable APLL low power mode
	hwp_analogReg->APLL_reg0 |= ANALOG_REG_APLL_LP_MODE_EN;
	// enable MEMPLL low power mode
	hwp_analogReg->MEMPLL_reg0 |= ANALOG_REG_MEMPLL_LP_MODE_EN;

	/*PLL*/
	hwp_sysCtrlAp->Pll_Ctrl =
	    SYS_CTRL_PLL_ENABLE | SYS_CTRL_PLL_LOCK_RESET_NO_RESET;
	mask = SYS_CTRL_PLL_LOCKED_MASK;
	locked = SYS_CTRL_PLL_LOCKED_LOCKED;
	while (((hwp_sysCtrlAp->Sel_Clock & mask) != locked) && cnt) {
		udelay(1);
             rprintf("wapll\n");
		cnt--;
	}
	/*mempll */
	hwp_sysCtrlAp->Cfg_MISC1_CFG |= SYS_CTRL_MEMPLL_ENABLE;
	mask = SYS_CTRL_MEMPLL_LOCKED_H;
	locked = SYS_CTRL_MEMPLL_LOCKED_H;
	while (((hwp_sysCtrlAp->Sel_Clock & mask) != locked) && cnt) {
		udelay(1);
             rprintf("wmpll\n");
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

#if defined(CHIP_VER_UWS6121E)
	div = clk_get_regval_by_freq(clock_config->cpu_pll_freq,
			clock_config->flash_dev_clk);
	hwp_sysCtrlAp->CFG_PLL_SPIFLASH1_DIV =
			div | SYS_CTRL_CFG_PLL_SPIFLASH1_DIV_UPDATE;
#endif

	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV &= ~(SYS_CTRL_CFG_CLK_494M_LTE_APLL_DIV(0xF));
	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV |= 0xd | (1 << 4);
	hwp_sysCtrlAp->Cfg_MISC1_CFG |= SYS_CTRL_APLL_ZSP_CLK_SEL;

	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV &= ~(SYS_CTRL_CFG_CLK_96M_DIV(0x7FFFFF));
	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV |= SYS_CTRL_CFG_CLK_96M_DIV(0xD005);    /*dived of APLL/2(500M),48M*/
	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV |= SYS_CTRL_CFG_CLK_96M_DIV_UPDATE;
	hwp_sysCtrlAp->CFG_CLK_494M_LTE_DIV |= (SYS_CTRL_CFG_CLK_96M_26M_SEL | SYS_CTRL_CFG_CLKSW_494M_CLK_SEL);    /*select 48M clk from apll as the base_clk of zspuart*/

       /*uart1 aon basefreq only to 26MHz, hwp_sysCtrlAp->Cfg_Clk_Uart[1]ÎÞÐ§*/

       hwp_sysCtrlAp->Cfg_Clk_Uart[2] &= ~(SYS_CTRL_UART_DIVIDER(0xFFFFFF));
       hwp_sysCtrlAp->Cfg_Clk_Uart[2] |= (SYS_CTRL_UART_DIVIDER(0x6805));        /*dived of APLL/2(500M),96M*/
       hwp_sysCtrlAp->Cfg_Clk_Uart[2] |= SYS_CTRL_UART_DIVIDER_UPDATE;
       hwp_sysCtrlAp->Cfg_Clk_Uart[2] |= SYS_CTRL_UART_SEL_PLL;
       /*as print_uart use uart2 921600 with baseclk 26MHz,so now need change settings of baud register by 96MHz div*/
       *(volatile unsigned int *)UART2_BAUD_REG = 0xC0007;    /*921600 div by 96MHz*/

       hwp_sysCtrlAp->Cfg_Clk_Uart[3] &= ~(SYS_CTRL_UART_DIVIDER(0xFFFFFF));
       hwp_sysCtrlAp->Cfg_Clk_Uart[3] |= (SYS_CTRL_UART_DIVIDER(0x6805));        /*dived of APLL/2(500M),96M*/
       hwp_sysCtrlAp->Cfg_Clk_Uart[3] |= SYS_CTRL_UART_DIVIDER_UPDATE;
       hwp_sysCtrlAp->Cfg_Clk_Uart[3] |= SYS_CTRL_UART_SEL_PLL;

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
				(~SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_MANUAL));

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

#define hwp_CPidle hwp_idle
static void cp_idle_cfg(void)
{
	hwp_CPidle->SLOW_SYS_CLK_SEL_HWEN |= CP_IDLE_HWEN;
	hwp_CPidle->SLOW_CLK_SEL_HWEN |= CP_IDLE_HWEN;
}

static void clock_setup_init(void)
{
	clock_config = &clock_default_config;
}

#define CLOCK_VER_MAJOR		2
#define CLOCK_VER_MINOR		0

int clock_init(void)
{
#ifndef CHIP_ECO_SUPPORT
    *((volatile uint32 *)(0x500340AC)) = 0x54;
    TIMER_DELAY_US(1000);
#endif

#if  !defined(FPGA_SUPPORT_UIX8910)
	rprintf("Init Clock ...\n");
	clock_setup_init();

	sys_clkdisable_init();
	sys_slowclk_sel();      /*softrest can not reset idle,so fast_clk_set is still exist,but next function will close pll,so need sel slowclk at here */
	sys_shutdown_pll();

	sys_setup_pll();
	sys_setup_clk();

	sys_dump_clk();

	//sys_pll_lowpower_cfg();
	//sys_bus_auto_sleep_cfg();
	//sys_clk_autogate_cfg();
	//zsp_bus_clk_autogate_cfg();
	//cp_idle_cfg();
#endif
	return 0;
}
