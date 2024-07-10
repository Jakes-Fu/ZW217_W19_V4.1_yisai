/* ************************************************************************************************
 *
 *  Filename:	LV24001Reg.h (ImRadioIc1Reg.h)
 *  Project:    LV24-EVK
 *  Authors:	Hung van Le
 *  Purpose:    register layout of LV24000/LV24001/LV24002-IC
 *  Comments:   
 * ************************************************************************************************
 *  History:
 *  Version	Date			Author		Reason
 *    1.00	02-Apr-2004		HLE			- Initial version
 *
 * ************************************************************************************************
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE.
 *
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */

#if !defined(IM_RADIO_IC1_H__INCLUDED_)
#define IM_RADIO_IC1_H__INCLUDED_

//////////////////////////////////////////////////////////////////////
// Chip IDs
//////////////////////////////////////////////////////////////////////
#define LV24000_ID			0x05	// LV24000
#define LV24001_ID			0x05	// LV24001 (same ID as LV24000)
#define LV24002_ID			0x04	// LV24002: headphone supported
#define LV24100_ID			0x07	// LV24100: AM supported
#define LV24101_ID			0x07	// LV24101: same as LV24100
#define LV24102_ID			0x06	// LV24102: headphone & AM supported

//////////////////////////////////////////////////////////////////////
// Register address of IMR01
//////////////////////////////////////////////////////////////////////
// Blocks addresses
#define IMR01_SM_BLOCK	0x01	// Block 1: Status and measurements
#define IMR01_CT_BLOCK	0x02	// Block 2: Control block

// Common registers (block address is don't care)
#define BLK_SEL 	0x01		// Address of Block Select register

// IMR01 registers
#define IR01_CHIP_ID_REG		0x0100	// Block 1- Reg00 (R): chip identify
#define IR01_MSRC_SEL_REG		0x0102	// Block 1- Reg02 (W): Measure source select
#define IR01_FM_OSC_REG			0x0103	// Block 1- Reg03 (W): DAC control for FM-RF oscillator
#define IR01_SD_OSC_REG			0x0104	// Block 1- Reg04 (W): DAC control for stereo decoder oscillator
#define IR01_IF_OSC_REG			0x0105	// Block 1- Reg05 (W): DAC control for IF oscillator
#define IR01_CNT_CTRL_REG		0x0106	// Block 1- Reg06 (W): Counter control
#define IR01_NA1_REG			0x0107	// Block 1- Reg07 (-): Reserved for RDS Oscillator
#define IR01_IRQ_MSK_REG		0x0108	// Block 1- Reg08 (W): Interrupt mask
#define IR01_FM_CAP_REG			0x0109	// Block 1- Reg09 (W): CAP bank control for RF-frequency
#define IR01_CNT_L_REG			0x010A	// Block 1- Reg0A (R): Counter value low byte
#define IR01_CNT_H_REG			0x010B	// Block 1- Reg0B (R): Counter value high byte
#define IR01_CTRL_STAT_REG		0x010C	// Block 1- Reg0C (R): Control status
#define IR01_RADIO_STAT_REG		0x010D	// Block 1- Reg0D (R): Radio station status
#define IR01_IRQ_ID_REG			0x010E	// Block 1- Reg0E (R): Interrupt identify
#define IR01_IRQ_OUT_REG		0x010F	// Block 1- Reg0F (W): Set interrupt on the DATA line

#define IR01_RADIO_CTRL1_REG	0x0202	// Block 2- Reg02 (W): Radio control 1
#define IR01_IFCEN_OSC_REG		0x0203	// Block 2- Reg03 (W): IF Center Frequency Oscillator
#define IR01_NA2_REG			0x0204	// Block 2- Reg04 (W): Reserved fro AM Antenna Capacitor
#define IR01_IF_BW_REG			0x0205	// Block 1- Reg05 (W): IF Bandwidth
#define IR01_RADIO_CTRL2_REG	0x0206	// Block 2- Reg06 (W): Radio Control 2
#define IR01_RADIO_CTRL3_REG	0x0207	// Block 2- Reg07 (W): Radio Control 3
#define IR01_STEREO_CTRL_REG	0x0208	// Block 2- Reg08 (W): Stereo Control
#define IR01_AUDIO_CTRL1_REG	0x0209	// Block 2- Reg09 (W): Audio Control 1
#define IR01_AUDIO_CTRL2_REG	0x020A	// Block 2- Reg0A (W): Audio Control 2
#define IR01_PW_SCTRL_REG		0x020B	// Block 2- Reg0B (W): Power and soft control

//////////////////////////////////////////////////////////////////////
// Register layout - Block 1
//////////////////////////////////////////////////////////////////////
// ----- IR01_MSRC_SEL_REG layout
											// Bit 7-5: Reserved
#define	IR1_MSRCS_MSR_OUT	((BYTE)0x1<<4)	// Bit 4: Output measure source to DATA-pin when 1
											// Bit 3: Reserved
#define	IR1_MSRCS_MSS_SD	((BYTE)0x1<<2)	// Bit 2: Enable stereo decoder oscillator measurement when 1
#define	IR1_MSRCS_MSS_FM	((BYTE)0x1<<1)	// Bit 1: Enable FM RF oscillator measurement when 1
#define	IR1_MSRCS_MSS_IF	((BYTE)0x1<<0)	// Bit 0: Enable IF oscillator measurement when 1

// Measured source mask
#define IR1_MSRCS_MSS_MASK	(IR1_MSRCS_MSS_FM|IR1_MSRCS_MSS_SD|IR1_MSRCS_MSS_IF)

// ----- IR01_FM_OSC_REG layout
#define FMOSC_VALUE_MASK	DGT2_DATA_MASK

// ----- IR01_SD_OSC_REG layout
#define SDOSC_VALUE_MASK	DGT2_DATA_MASK

// ----- IR01_IF_OSC_REG layout
#define IFOSC_VALUE_MASK	DGT2_DATA_MASK

// ----- IR01_CNT_CTRL_REG layout
#define	IR1_CCTL_CNT1_CLR	((BYTE)0x1<<7)	// Bit 7: Clear and keep counter 1 in reset mode when 1
#define	IR1_CCTL_CNT_TAB	((BYTE)7<<4)	// Bit [6:4]: tab select for counter 2 measuring interval
	#define IR1_CTAB_2     ((BYTE)0<<4)		// Stop after 2 counts
	#define IR1_CTAB_8     ((BYTE)1<<4)		// Stop after 8 counts 
	#define IR1_CTAB_32    ((BYTE)2<<4)		// Stop after 32 counts 
	#define IR1_CTAB_128   ((BYTE)3<<4)		// Stop after 128 counts
	#define IR1_CTAB_512   ((BYTE)4<<4)		// Stop after 512 counts
	#define IR1_CTAB_2048  ((BYTE)5<<4)		// Stop after 2048 counts
	#define IR1_CTAB_8192  ((BYTE)6<<4)		// Stop after 8192 counts
	#define IR1_CTAB_32768 ((BYTE)7<<4)		// Stop after 327682 counts
#define	IR1_CCTL_SWP_CNT_L	((BYTE)0x1<<3)	// Bit 3: Swap clock source 1 to counter 2, clock source 2 to counter 1 when 1
#define	IR1_CCTL_CNT_EN		((BYTE)0x1<<2)	// Bit 2: Enable counter (counting mode) when 1
#define	IR1_CCTL_CNT_SEL	((BYTE)0x1<<1)	// Bit 1: Select counter 2 when 1 (counter 1 when 0)
#define	IR1_CCTL_CNT_SET	((BYTE)0x1<<0)	// Bit 0: Set both counter 1 and counter 2 to FFFFh and keep them set when 1

// ----- IR01_IRQ_MSK_REG layout
												// Bit 7: Reserved
#define	IR1_IRQM_EN_MS			((BYTE)0x1<<6)	// Bit 6: Enable mono/stereo change interrupt when 1
												// Bit 5-4: Reserved
#define	IR1_IRQM_ACT_LOW		((BYTE)0x1<<3)	// Bit 3 is 1: IRQ active low
	#define	IR1_IRQM_ACT_HIGH	((BYTE)0x0<<3)	// Bit 3 is 0: IRQ active high
#define	IR1_IRQM_EN_AFC			((BYTE)0x1<<2)	// Bit 2: Enable AFC out of range interrupt whne 1
#define	IR1_IRQM_EN_FS			((BYTE)0x1<<1)	// Bit 1: Enable field strength change interrupt when 1
#define	IR1_IRQM_EN_CNT2		((BYTE)0x1<<0)	// Bit 0: Enable counter 2 counting done interrupt when 1

// ----- IR01_FM_CAP_REG layout
#define FMCAP_VALUE_MASK		DGT2_DATA_MASK

// ----- IR01_CTRL_STAT_REG layout
												// Bit [7:1]: Reserved
#define	IR1_CSTAT_AFC_FLAG		((BYTE)0x1<<0)	// Bit 0: AFC is out of control range when 1

// ----- IR01_RADIO_STAT_REG layout
#define	IR1_RSTAT_STEREO		((BYTE)0x1<<7)	// Bit 7: Radio station is stereo when 1 (0=mono)
#define	IR1_RSTAT_FS			((BYTE)0x7F<<0)	// Bit [6:0]: Radio station field strength bits

// ----- IR01_IRQ_ID_REG layout
												// Bit [7:6]: Reserved (read as 1)
#define	IR1_IRQID_CNT2			((BYTE)0x1<<5)	// Bit 5: counter 2 counting done interrupt when 1
												// Bit 4: Reserved (read as 0)
#define	IR1_IRQID_AFC			((BYTE)0x1<<3)	// Bit 3: AFC interrupt when 1
												// Bit 2: Reserved (read as 0)
// V3
//#define	IR1_IRQID_MS		((BYTE)0x1<<1)	// Bit 1: Mono/Stereo changed interrupt when 1
//#define	IR1_IRQID_FS		((BYTE)0x1<<0)	// Bit 0: Field strength changed interrupt when 1
// V4
												// Bit 1: Reserved (read as 0)
#define	IR1_IRQID_FS_MS			((BYTE)0x1<<0)	// Bit 0: Field strength/Mono-Stereo changed interrupt when 1

//////////////////////////////////////////////////////////////////////
// Register layout - Block 2
//////////////////////////////////////////////////////////////////////
// ----- IR01_RADIO_CTRL1_REG layout
#define	IR1_RCTL1_EN_MEAS		((BYTE)0x1<<7)	// Bit 7: Measurement mode if 1
#define	IR1_RCTL1_EN_AFC		((BYTE)0x1<<6)	// Bit 6: Enable AFC if 1
												// Bit 5: Reserved (write with 0)
#define	IR1_RCTL1_NA_H_4		((BYTE)0x1<<4)	// Bit 4: Reserved - must be written with 1
#define	IR1_RCTL1_DIR_AFC		((BYTE)0x1<<3)	// Bit 3: reverse AFC direction if 1
#define	IR1_RCTL1_RST_AFC		((BYTE)0x1<<2)	// Bit 2: Reset AFC to the middle of the control range if 1
#define	IR1_RCTL1_NA_H_1		((BYTE)0x1<<1)	// Bit 1: Reserved - must be written with 1
#define	IR1_RCTL1_NA_H_0		((BYTE)0x1<<0)	// Bit 0: Reserved - must be written with 1

// ----- IR01_RADIO_CTRL2_REG layout
#define	IR1_RCTL2_VREF2_L		((BYTE)0x1<<7)	// Bit 7: VREF2 control (0=ON)
#define	IR1_RCTL2_VREF_L		((BYTE)0x1<<6)	// Bit 6: VREF control (0=ON)
#define	IR1_RCTL2_BYPASS_STABI	((BYTE)0x1<<5)	// Bit 5: Bypass the stabi circuit (should be 0: not bypassed)
#define	IR1_RCTL2_IF_PM_L		((BYTE)0x1<<4)	// Bit 4: IF PLL mute on (presetting IF mode) if 0
#define IR1_RCTL2_DCFB_SPEED	((BYTE)0x1<<3)	// Bit 3: DC feedback speed
#define IR1_RCTL2_DCFB_OFF		((BYTE)0x1<<2)	// Bit 2: Turn off DC feedback if 1
#define	IR1_RCTL2_AGC_SPD		((BYTE)0x1<<1)	// Bit 1: AGC high speed if 1
												// Bit 0: Reserved

// ----- IR01_RADIO_CTRL3_REG layout
#define	IR1_RCTL3_AGC_SETLVL	((BYTE)0x1<<7)	// Bit 7: AGC set level (=1 for FM)
#define	IR1_RCTL3_VOL_SHIFT		((BYTE)0x1<<6)	// Bit 6: Shift volume extra 12dB if 1
#define	IR1_RCTL3_TONE_MUTE_L	((BYTE)0x1<<5)	// Bit 5: Tone mute control (0=mute)
#define	IR1_RCTL3_AUDIO_MUTE_L	((BYTE)0x1<<4)	// Bit 4: Audio mute if 0
#define	IR1_RCTL3_SE_FM			((BYTE)0x1<<3)	// Bit 3: Source enable: FM
												// Bit 2: Reserved
#define	IR1_RCTL3_SE_BEEP		((BYTE)0x1<<1)	// Bit 1: Source enable: Beep
#define	IR1_RCTL3_SE_EXT		((BYTE)0x1<<0)	// Bit 0: Source enable: external

// ----- IR01_STEREO_CTRL_REG layout
#define	IR1_STCTL_FRC_STEREO	((BYTE)0x1<<7)	// Bit 7: Force stereo if 1
#define	IR1_STCTL_FMCS			((BYTE)7<<4)	// Bit 6-4: FM channel separation level
#define	IR1_STCTL_AUTO_SLEWRATE	((BYTE)0x1<<3)	// Bit 3: Stereo auto slew rate
//#define	IR1_STCTL_SITG_OFF		((BYTE)0x1<<2)	// Bit 2: turn stereo integrator off if 1 for V4
#define	IR1V6_CANCEL_PILOT		((BYTE)0x1<<2)	// Bit 2: Pilot cancellation for V6
#define	IR1_STCTL_SD_PM			((BYTE)0x1<<1)	// Bit 1: Mute stereo decoder PLL if 1 (presetting mode)
#define	IR1_STCTL_STEREO_L		((BYTE)0x1<<0)	// Bit 0: Enable stereo if 0

// ----- IR01_AUDIO_CTRL1_REG layout
#define	IR1_ACTL1_TONE_LVL		((BYTE)0xF<<4)	// Bit [7:4]: Tone level
#define	IR1_ACTL1_VOL_LVL		((BYTE)0xF<<0)	// Bit [3:0]: Volume level

// ----- IR01_AUDIO_CTRL2_REG layout
#define	IR1_ACTL2_BPFREQ		((BYTE)0x3<<6)	// Bit [7:6]: Beep frequency bits
		#define	IR1_BEEP_2KHz	((BYTE)0<<6)		// 2 kHz beep tone
		#define	IR1_BEEP_1KHz	((BYTE)1<<6)		// 1 kHz beep tone
		#define	IR1_BEEP_500Hz  ((BYTE)2<<6)		// 0.5 kHz beep tone
		#define	IR1_BEEP_HIGH	((BYTE)3<<6)		// Beep output high
		#define	IR1_BEEP_OFF	IR1_BEEP_HIGH		// Beep off (drive output high)
#define	IR1_ACTL2_DEEMP75		((BYTE)0x1<<5)	// Bit 5: De-emphasis 75 if 1 (0:50us)
#define	IR1_ACTL2_TREB_N		((BYTE)0x1<<4)	// Bit 4: Negative treble if 1
#define	IR1_ACTL2_TREB_P		((BYTE)0x1<<3)	// Bit 3: Positive treble if 1
#define	IR1_ACTL2_BASS_N		((BYTE)0x1<<2)	// Bit 2: Negative bass if 1
#define	IR1_ACTL2_BASS_P		((BYTE)0x1<<1)	// Bit 1: Positive bass if 1
#define	IR1_ACTL2_BASS_LVL		((BYTE)0x1<<0)	// Bit 0: Bass level

#define	IR1_ATREBLE_MASK	(IR1_ACTL2_TREB_N | IR1_ACTL2_TREB_P)
#define	IR1_ABASS_MASK		(IR1_ACTL2_BASS_N | IR1_ACTL2_BASS_P | IR1_ACTL2_BASS_LVL)
/*
	Treble levels
	-------------
	Treble_N (Bit4) - Treble_P (Bit 3) - Level - Value
	1					0					-1		2
	0					0					0		0
	0					1					+1		1

	Bass levels
	-------------
	Bass_N (Bit2) - Bass_P (Bit 1) - Bass_Lvl (bit 0) - Level - Value
	1					0				1				-2		5
	1					0				0				-1		4
	0					0				0				0		0
	0					1				0				+1		2
	0					1				1				+2		3

  When treble level != 0 OR bass level != 0
	IR1_RCTL3_TONE_MUTE_L bit must be high (unmute tone control)
*/

// ----- IR01_PW_SCTRL_REG layout
#define	IR1_PSCTL_SOFT_ST	((BYTE)0x7<<5)	// Bit [7:5]: Soft stereo control
#define	IR1_PSCTL_SOFT_MUTE	((BYTE)0x7<<2)	// Bit [4:2]: Soft mute control
#define	IR1_PSCTL_NA1		((BYTE)0x1<<1)	// Bit 1: NA for IR1
#define	IR2_PSCTL_HPA		((BYTE)0x1<<1)	// Bit 1: Headphone power on for LV24xx2
#define	IR1_PSCTL_PW_RAD	((BYTE)0x1<<0)	// Bit 0: Radio circuitry power on if 1

//////////////////////////////////////////////////////////////////////
// Frequency divider factor
#define IMR01_FM_DIVIDER		256			// FM frequency is divided before measuring
#define IMR01_CNT2_PRESCALER	2			// Counter 2 prescaler

//////////////////////////////////////////////////////////////////////
// Some default value
#define IR1_DEF_CS_VAL			((BYTE)0x4<<4)	// IR01_STEREO_CTRL_REG: default channel separation value	

#define	IR1_DEF_SOFT_ST			((BYTE)0x3<<5)	// IR01_PW_SCTRL_REG[7:5]: Soft stereo control
#define	IR1_DEF_SOFT_MUTE		((BYTE)0x3<<2)	// IR01_PW_SCTRL_REG[4:2]: Soft mute control

//////////////////////////////////////////////////////////////////////
// Equation for quick scan data
#define QSS_MIN_RFCAP	0
#define QSS_MAX_RFCAP	191

// V4 values
//#define QSS_MIN_RFOSC	30
//#define QSS_MAX_RFOSC	200
// V6 values
#define QSS_MIN_RFOSC	0
#define QSS_MAX_RFOSC	240
#endif // !define IM_RADIO_IC1_H__INCLUDED_ 
