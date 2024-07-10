#include "sci_types.h"
#include "arm_reg.h"
#include "chip_plf_export.h"
#include "global_macros.h"
#include "efuse_drvapi.h"
#include "../efuse_phy.h"
#include "efuse_reg_uix8910.h"

PUBLIC void EFUSE_PHY1_Open(void)
{
#if 0
	hwp_efuseCtrl->EFUSE_SEC_MAGIC_NUMBER = 0x8910;
	hwp_efuseCtrl->EFUSE_SEC_EN = 0x1;
	hwp_efuseCtrl->EFUSE_PW_SWT = 0x5;
#endif
}

PUBLIC void EFUSE_PHY1_Close(void)
{
#if 0
	hwp_efuseCtrl->EFUSE_SEC_MAGIC_NUMBER = 0x8910;
	hwp_efuseCtrl->EFUSE_SEC_EN = 0x0;
	hwp_efuseCtrl->EFUSE_PW_SWT = 0x2;
#endif
}

PUBLIC uint32 EFUSE_PHY1_Read(int page_index)
{
	uint32 val = readl(REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4) |
			readl(REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4 + 4);

	return val;
}

PUBLIC void EFUSE_PHY1_Write(int page_index, uint32 val)
{
	writel(val, REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4);
	writel(val, REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4 + 4);
}

PUBLIC void EFUSE_PHY1_enWCNuart(void)
{
	hwp_efuseCtrl->wcn_uart_disable_reg = 0x0;
}



