/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmicmndefs.h
**		These are the common defines that are being used by the ASIC driver.
**
** 
*******************************************************************************/

#ifndef _NMICMNDEFS_H_
#define _NMICMNDEFS_H_

/******************************************************************************
**
**	NMI Bus Type
**
*******************************************************************************/

typedef enum {
	nI2C,
	nUSB
} tNmiBus;

/******************************************************************************
**
**	NMI Analog TV Standard
**	(*Note: Don't modify the order here)
**
*******************************************************************************/

typedef enum {
	nNTSC = 0,
 	nPAL_G,
	nPAL_H,
 	nPAL_M,
 	nPAL_N,
 	nPAL_K,
 	nPAL_K1,
 	nPAL_D,
 	nPAL_D1,
 	nPAL_B,
 	nPAL_B1,
 	nPAL_I,
 	nFM_RADIO,
	nSECAM_B,
	nSECAM_D,
	nSECAM_G,
	nSECAM_H,
	nSECAM_K,
	nSECAM_K1
} tNmiTvStandard;

typedef enum {
	nAuto = 1,
	nNoiseReduce,
	nMotionEnhance
} tNmiBeMode;

typedef enum {
	nClk6_75 = 1,
	nClk13_5,
	nClk27
} tNmiBt656Clk;

typedef enum {
	nBTYCbCr = 0,
	nBTRGB
} tNmiBtFormat;

typedef enum {
	nI2S_SIGDEL = 0,
	nI2S_ADEC,
	nI2S_STD,
	nI2S_ADAC
} tNmiI2SFormat;

typedef enum {
	n42_18KHz = 0,
	n35_15KHz,
	n46_87KHz,
	n38_35KHz,
	n32_45KHz,
	n28_12KHz,
	nI2S_RATE_END	
} tNmiI2SRate;

typedef enum {
	n320x240 = 0,
	n80x60,
	n176x144,
	n220x176,
	n240x180,
	n426x320,
	n360x240,
	n480x280,
	n480x360,
} tNmiOutputSize;

typedef enum {
	n2_8Volt = 0,
	n1_8Volt
} tNmiIOVoltage;

/******************************************************************************
**
**	Debug Flags Defines
**
*******************************************************************************/

#define N_INIT			0x1
#define N_ERR			0x2
#define N_WARN		0x4
#define N_FUNC			0x8
#define N_INFO			0x10
#define N_TUNE			0x20
#define N_VERB			0x40
#define N_UV				0x80
#define N_SCAN			0x100
#define N_SEEK			0x200
#endif

