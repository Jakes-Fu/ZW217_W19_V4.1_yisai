/****************************************************************************************
** File Name:		la_tp.h															    *	
** Author:			Harry QIN															*
** DATE:			11/20/2001															*
** CopyRight		2001 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This file contains the test points defination for software 	        *
			debugging using logic analyzer					                            *
****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 11/20/2001		Bin Lu					Create.									    *
*****************************************************************************************/

#ifndef __LA_TP_H__
#define __LA_TP_H__

#include "sci_types.h"

#ifdef WIN32

uint16 *tp;
#define TP0			*tp
#define TP1			*tp
#define TP2			*tp
#define TP3			*tp
#define TP4			*tp
#define TP5			*tp
#define TP6			*tp
#define TP7			*tp
#define TP8			*tp
#define TP9			*tp
#define TP10		*tp


#define TPP1			*tp
#define TPP2			*tp
#define TPP3			*tp
#define TPP4			*tp
#define TPP5			*tp
#define TPP6			*tp
#define TPP7			*tp
#define TPP8			*tp
#define TPP9			*tp
#define TPP10			*tp

#else

#define TP0			*(volatile uint16 *)0x4300000
#define TP1			*(volatile uint16 *)0x4300004
#define TP2			*(volatile uint16 *)0x4300008
#define TP3			*(volatile uint16 *)0x430000c
#define TP4			*(volatile uint16 *)0x4300010
#define TP5			*(volatile uint16 *)0x4300014
#define TP6			*(volatile uint16 *)0x4300018
#define TP7			*(volatile uint16 *)0x430001c
#define TP8			*(volatile uint16 *)0x4300020
#define TP9			*(volatile uint16 *)0x4300024
#define TP10			*(volatile uint16 *)0x4300028


#define TPP1			*(volatile uint32 *)0x43f0000
#define TPP2			*(volatile uint32 *)0x43f0004
#define TPP3			*(volatile uint32 *)0x43f0008
#define TPP4			*(volatile uint32 *)0x43f000c
#define TPP5			*(volatile uint32 *)0x43f0010
#define TPP6			*(volatile uint32 *)0x43f0014
#define TPP7			*(volatile uint32 *)0x43f0018
#define TPP8			*(volatile uint32 *)0x43f001c 
#define TPP9			*(volatile uint32 *)0x43f0020

#define	L1_INST_IN		*(volatile uint16 *)0x43f0024
#define L1_INST_OUT             *(volatile uint16 *)0x43f0028
#define L1_SPEED_TP		*(volatile uint16 *)0x43f002c

#define L1_GPRS_BCCH      	*(volatile uint16 *)0x43f0030
#define L1_GPRS_LOAD1      	*(volatile uint16 *)0x43f0032
#define L1_GPRS_LOAD2      	*(volatile uint16 *)0x43f0034
#define L1_GPRS_LOAD3      	*(volatile uint16 *)0x43f0036
#define L1_GPRS_LOAD4      	*(volatile uint16 *)0x43f0038

#define RLC_GPRS_LOAD      	*(volatile uint16 *)0x43f0038
//#define L1_GPRS_LOAD4      	*(volatile uint16 *)0x43f0060
#define L1_GPRS_LOAD5      	*(volatile uint16 *)0x43f0062

#define L1_GPRS_INT	      	*(volatile uint16 *)0x43f0070

#define RLC_GPRS_DATA      	 (volatile uint16 *)0x43f0040
#define RLC_GPRS_DATA_ST      	 (volatile uint16 *)0x43f0050


#define SHARED_MEM_IMAGE	(volatile uint32 *)0x4ff0000

#define SHARED_MEM_FRAME	(volatile uint16 *)0x4ff0000         
#define SHARED_MEM_TX_DATA	(volatile uint16 *)0x4ff0244       
#define SHARED_MEM_CMDQ		(volatile uint16 *)0x4ff0334
#define SHARED_MEM_RX_DATA	(volatile uint16 *)0x4ff03FE

#define SHARED_MEM_RESQ		(volatile uint16 *)0x4ff0388
#define SHARED_MEM_DSPMeasRpts	*(volatile uint16 *)0x4ff05D2
#define SHARED_MEM_PWRMONIQ	(volatile uint16 *)0x4ff051A


#define SHARED_RESP_ID		*(volatile uint32 *)0x4ff1000
#define SHARED_RESP_PARAM0	*(volatile uint32 *)0x4ff1004
#define SHARED_RESP_PARAM1	*(volatile uint32 *)0x4ff1008
#define SHARED_RESP_PARAM2	*(volatile uint32 *)0x4ff100c
#define SHARED_RESP_PARAM3	*(volatile uint32 *)0x4ff1010

#define DSP_ASK_MORE_INT	*(volatile uint32 *)0x4ff1100

#endif //end of win32
#endif // end of __LA_TP_H__