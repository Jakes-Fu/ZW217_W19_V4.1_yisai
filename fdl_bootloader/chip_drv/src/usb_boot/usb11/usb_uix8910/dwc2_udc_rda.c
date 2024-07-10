/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * This is file is based on
 * repository git.gitorious.org/u-boot-omap3/mainline.git,
 * branch omap3-dev-usb, file drivers/usb/host/omap3530_usb.c
 *
 * This is the unique part of its copyright :
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (c) 2009 Texas Instruments
 *
 * ------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
//#include <common.h>
#include "global_macros.h"
#include "analog_reg.h"

static void dwc2_phy_init(void)
{
	hwp_analogReg->USB_DIGPHY_ANA3 = 1 << 5;
}

int dwc2_platform_init(void)
{
	dwc2_phy_init();
	return 0;
}

