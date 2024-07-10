#ifndef _UDC_PLATFORM_UWS6121E_H
#define _UDC_PLATFORM_UWS6121E_H

#include <stdbool.h>
#include "global_macros.h"

#include "usb_device_uws6121e.h"
#include "lps_apb.h"
#include "pwrctrl.h"
#include "sys_ctrl.h"
#include "aon_clk.h"
#include "analog_g2.h"
#include "musb_regs.h"
#include "globals.h"


bool udcPlatformInit(udc_t *udc);
void udcPlatformExit(udc_t *udc);

#endif
