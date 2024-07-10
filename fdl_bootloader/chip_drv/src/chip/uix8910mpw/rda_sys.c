
#include "global_macros.h"
#include "iomux.h"

/*
 * Hardware and software configuration
 */
#define hwp_apiomux hwp_iomux

void hwcfg_iomux(void)
{
	hwp_apiomux->pad_uart_1_cts_cfg_reg |= IOMUX_PAD_UART_1_CTS_SEL(2);
	hwp_apiomux->pad_uart_1_rts_cfg_reg |= IOMUX_PAD_UART_1_RTS_SEL(2);
}

void hwcfg_swcfg_init(void)
{
	hwcfg_iomux();
}

