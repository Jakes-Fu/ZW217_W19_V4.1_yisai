/******************************************************************************
 ** File Name:    sc6531efm_module_ver.h                                            *
 ** Author:       Yuhua.Shi                                                 *
 ** DATE:         08/23/2011                                                  *
 ** Copyright:    2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 08/23/2011    Yuhua.Shi      Create.                                     *
 ** 04/11/2012    Yong.Li        Modified.                                    *
 ******************************************************************************/
#ifndef _UIX8910_MODULE_VER_H_
#define _UIX8910_MODULE_VER_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sys_ctrl.h"
#include "rda2720m_global.h"
#include "uix8910_reg_analog.h"
#include "wcn_sys_ctrl.h"
#include "cp_pwrctrl.h"
#include "cp_idle.h"
#include "cp_sysreg.h"
#include "wcn_fm_dsp.h"
#include "wcn_rf_if.h"
#include "rf_registers.h"
#include "rf_rtc.h"

//Chip All module register miscs definitions
#include "../../chip_module/adi/uix8910/adi_reg_uix8910.h"
#include "../../chip_module/gpio/v5/gpio_reg_v5.h"
#include "../../chip_module/sim/v5/sim_reg_v5.h"

#include "../../chip_module/sdio/v5/sdio_reg_v5.h"
#include "../../chip_module/spi/v5/spi_reg_v5.h"

#include "../../chip_module/timer/uix8910/timer_reg_uix8910.h"
//#include "../../chip_module/sys_timer/v5/sys_timer_reg_v5.h"

#include "../../chip_module/pcm/v5/pcm_reg_v5.h"
#include "../../chip_module/uart/uix8910/uart_reg_uix8910.h"
#include "../../chip_module/pin/uix8910/iomux_regasm_uix8910.h"

#include "../../chip_module/rotation/v5/rotation_reg_v5.h"
#include "../../chip_module/ldo/v5/ldo_reg_v5.h"

#include "../../chip_module/dma/uix8910/dma_reg_uix8910.h"
#include "../../chip_module/int/uix8910/int_reg_uix8910.h"

#include "../../chip_module/usb/uix8910/usb20_reg_uix8910.h"
#include "../../chip_module/sfc/v7/sfc_reg_v7.h"

#include "../../chip_module/spinlock/uix8910/spinlock_phy_uix8910.h"
#include "../../chip_module/spinlock/uix8910/spinlock_reg_uix8910.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
