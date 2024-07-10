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
#ifndef _UWS6121E_MODULE_VER_EXTERN_H_
#define _UWS6121E_MODULE_VER_EXTERN_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
typedef volatile unsigned int REG32;
#include "sci_types.h"
//#include "sc6531efm_reg_ahb.h"
#include "sci_types.h"
#include "sys_ctrl.h"
#include "rda2720m_global.h"
#include "wcn_sys_ctrl.h"
#include "wcn_fm_dsp.h"
#include "wcn_rf_if.h"
#include "pwrctrl.h"
#include "idle.h"
#include "cp_sysreg.h"
#include "analog_reg.h"
#include "rf_registers.h"
#include "rf_rtc.h"

//Chip All module register miscs definitions
#include "../../chip_module/isp/v5/isp_reg_v5.h"
#include "../../chip_module/emc/v5/emc_reg_v5.h"
//#include "../../chip_module/int/uix8910/int_reg_uix8910.h"
#include "../../chip_module/int/uws6121e/int_reg_uws6121e.h"
#include "../../chip_module/timer/uws6121e/timer_reg_uws6121e.h"
#include "../../chip_module/lcdc/v5/lcdc_reg_v5.h"
#include "../../chip_module/pin/uws6121e/iomux_regasm_uws6121e.h"
#include "../../chip_module/pin/uws6121e/iomux_reg_uws6121e.h"
//#include "../../chip_module/sys_timer/v5/sys_timer_reg_v5.h"
#include "../../chip_module/usb/uws6121e/inc/musb_regs.h"
#include "../../chip_module/sfc/v7/sfc_reg_v7.h"

#include "../../chip_module/uart/uws6121e/uart_reg_uws6121e.h"
#include "../../chip_module/spinlock/uws6121e/spinlock_phy_uws6121e.h"
#include "../../chip_module/spinlock/uws6121e/spinlock_reg_6121e.h"

//#include "../../chip_module/watchdog/v5/watchdog_reg_v5.h"


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
typedef unsigned char		U8;
typedef char				S8;
typedef unsigned short		U16;
typedef short				S16;
typedef unsigned int        U32;
typedef int                 S32;


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
