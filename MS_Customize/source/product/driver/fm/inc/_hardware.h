/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 * 
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __ASM_ARCH_SCI_HARDWARE_H
#define __ASM_ARCH_SCI_HARDWARE_H

#define CTL_BASE_IROM               0x00000000
#define CTL_BASE_IRAM               0x00100000
#define CTL_BASE_DUMMY_MEM          0x00200000


#define CTL_BASE_PINREG				0x40000000
#define CTL_BASE_INTC0              0x80001000
#define CTL_BASE_SYS_TMR            0x81000000

#define CTL_BASE_TMR0            	0x40030000
#define CTL_BASE_TMR1            	0x40030020
#define CTL_BASE_TMR2            	0x40030040
#define CTL_BASE_TMR3            	0x40030060
#define CTL_BASE_TMR4            	0x40030080
#define CTL_BASE_TMR5            	0x400300a0
#define CTL_BASE_TMR6            	0x400300c0
#define CTL_BASE_TMR7            	0x400300e0
#define CTL_BASE_TMR8            	0x40030100
#define CTL_BASE_TMR9            	0x40030120

#define CTL_BASE_WDG               	0x40040000
#define CTL_BASE_GPIO               0x40050000
#define CTL_BASE_GLOABL             0x8B000000
#define CTL_BASE_UART0              0x83000000
#define CTL_BASE_UART1              0x84000000
#define CTL_BASE_IIS0               0x40090000
#define CTL_BASE_BT               	0x400A0000
#define CTL_BASE_FM               	0x88002000
#define CTL_BASE_EIC               	0x400C0000


#define CTL_BASE_BUSMON0            0x60000000
#define CTL_BASE_BUSMON1            0x60100000
#define CTL_BASE_DMA                0x60200000
#define CTL_BASE_AHB                0x20500000
#define CTL_BASE_SDIO               0x60400000
#define CTL_BASE_RF               	0x8d000000


#define CTL_BASE_WIFI_REG           0x70000000
#define CTL_BASE_WIFI_RAM           0x70040000


#define SYSTIMER_BASE           CTL_BASE_SYS_TMR
#define ARM_UART0_BASE          CTL_BASE_UART0
#define ARM_UART1_BASE          CTL_BASE_UART1


#endif
