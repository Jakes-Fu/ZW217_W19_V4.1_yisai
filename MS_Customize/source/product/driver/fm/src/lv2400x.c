/************************************************************************
*
*   Copyright(c) 2004 ItoM BV
*   All Rights Reserved.
*
*   LV2400x evaluation kit: LC24001 specific code
*   File name:	Lv2400x.c
*
*************************************************************************/

#include "ms_customize_trc.h"
#include <stdio.h>
#include "fm_common.h"
#include "Lv24Ekit.h"
#include "ref_outport.h"

//Add by Eric.wei
//#define SCI_TRACE_LOW SCI_TraceLow
//extern void SCI_TraceLow(const char*,...);
#include "os_api.h"


#define USE_FM_IRQ


// ==============================================================================
#ifdef USE_LV2400x 	// The whole file can be discarded if LV2400x is not used
// ==============================================================================
#include "Lv2400xReg.h"		// For register layout
/*-------------------------------------------------------------------
        global data
-------------------------------------------------------------------*/
//add by Eric.wei 2005/03/24
BYTE g_byStnFlag;		
BYTE g_bySwFlag1;		
BYTE g_byRegion;		
BYTE g_byScanLevel;		
BYTE g_byDynBassBoost;	
BYTE g_byBeepVol;		

BYTE  g_byHwFlag1;		
BYTE  g_byLvIrqFlg;		
BYTE  g_byBlock;		
WORD g_wLastSetRF;		
WORD g_wLastMsrRF;		
WORD g_wFreqGrid;		


WORD g_wHwRfLow;		
WORD g_wHwRfHigh;		
WORD g_wDisplayBandLow;	
WORD g_wDisplayBandHigh;
QSSF_ITEM Qssf1;		
QSSF_ITEM Qssf2;		

BYTE g_byInpPos;		
BYTE g_byaInp[5];		
BYTE g_byCurPreset;		

WORD g_wLastMsr;
DWORD g_dwCnt2ExtClk;

/*-------------------------------------------------------------------
        Local data
-------------------------------------------------------------------*/
typedef struct	// Register address-value structure (for default register settings)
{
	WORD wRegAddress;
	BYTE byValue;
} I3W_REG_VALUES;

// ----- Default register value list of LV2400x
I3W_REG_VALUES _rom g_Lv2400xDefault[] = 
{
	IR01_MSRC_SEL_REG,	0,			// 0x0102 - Measure source select: nothing selected

	//IR01_FM_OSC_REG,	0,			// 0x0103 - should be tuned
	//IR01_SD_OSC_REG,	0x80,			// 0x0104 - should be tuned
	//IR01_IF_OSC_REG,	0,			// 0x0105 - should be tuned
	//IR01_FM_CAP_REG,	0,			// 0x0109 - should be tuned
	IR01_CNT_CTRL_REG,	IR1_CCTL_SWP_CNT_L,	// 0x0106 - Counter control: select counter 1, no counter swapping
	IR01_IRQ_MSK_REG,	IR1_IRQM_ACT_LOW,	// 0x0108 - Interrupt mask: IRQ active low

	IR01_RADIO_CTRL1_REG,	(IR1_RCTL1_EN_AFC|
				 IR1_RCTL1_NA_H_4|
				 IR1_RCTL1_NA_H_1|
				 IR1_RCTL1_NA_H_0),	// 0x202 - Radio control 1 - Enable AFC as default - keep reserved bit high
	//IR01_IFCEN_OSC_REG,	xx,			// 0x0203 - IF Center Frequency Oscillator: init when setting IF 
	//IR01_IF_BW_REG,	xx,			// 0x0205 - IF Bandwidth: init when setting IF

	IR01_RADIO_CTRL3_REG,	(IR1_RCTL3_SE_FM|
				IR1_RCTL3_AGC_SETLVL),// 0x0207	- Radio Control 3: mute audio, mute tone, select FM source, Set AGC level for FM (V4)

	IR01_STEREO_CTRL_REG,	IR1_DEF_CS_VAL|
				IR1_STCTL_AUTO_SLEWRATE|
				IR1V6_CANCEL_PILOT,	// 0x0208 - Stereo Control: Mono, SD PLL mute off, set default CS value, auto slew rate on
	IR01_AUDIO_CTRL1_REG,	0x77,			// 0x0209 - Audio Control 1: default volume level
	IR01_AUDIO_CTRL2_REG,	IR1_BEEP_HIGH,		// 0x020A - Audio Control 2: treble/bass setting, don't enable the beep output

	IR01_RADIO_CTRL2_REG,	IR1_RCTL2_IF_PM_L|	
		 		IR1_RCTL2_AGC_SPD,	// 0x0206 - Radio Control 2: VREF2/VREF on, IF PLL mute off, Turn on AGC speed for quick fieldstrength measuring
	IR01_PW_SCTRL_REG,	(IR1_PSCTL_PW_RAD),	// 0x020B - Power and soft control: turn on FM. Set default soft mute, soft stereo head phone amp on
};
#define LV2400X_DEFAULT_REG_LSIZE (sizeof(g_Lv2400xDefault)/sizeof(g_Lv2400xDefault[0]))

/*-------------------------------------------------------------------
        Register address list to be shadowed
	(Only save the necessary registers to reduce RAM usage)
-------------------------------------------------------------------*/
WORD _rom g_Lv24ShadowList[] = 
{
	IR01_MSRC_SEL_REG,	// 0x0102 Block 1- Reg02 (W): Measure source select
	IR01_FM_OSC_REG,	// (*) 0x0103 Block 1- Reg03 (W): DAC control for FM-RF oscillator
	IR01_SD_OSC_REG,	// (*) 0x0104 Block 1- Reg04 (W): DAC control for stereo decoder oscillator
	IR01_IF_OSC_REG,	// (*) 0x0105 Block 1- Reg05 (W): DAC control for IF oscillator
	IR01_CNT_CTRL_REG,	// 0x0106 Block 1- Reg06 (W): Counter control
	IR01_IRQ_MSK_REG,	// 0x0108 Block 1- Reg08 (W): Interrupt mask
	IR01_FM_CAP_REG,	// 0x0109 Block 1- Reg09 (W): CAP bank control for RF-frequency

	IR01_RADIO_CTRL1_REG,	// 0x0202 Block 2- Reg02 (W): Radio control 1
	IR01_IFCEN_OSC_REG,	// (*) 0x0203 Block 2- Reg03 (W): IF Center Frequency Oscillator
	IR01_IF_BW_REG,		// (*) 0x0205 Block 1- Reg05 (W): IF Bandwidth
	IR01_RADIO_CTRL2_REG,	// 0x0206 Block 2- Reg06 (W): Radio Control 2
	IR01_RADIO_CTRL3_REG,	// 0x0207 Block 2- Reg07 (W): Radio Control 3
	IR01_STEREO_CTRL_REG,	// 0x0208 Block 2- Reg08 (W): Stereo Control	
	IR01_AUDIO_CTRL1_REG,	// 0x0209 Block 2- Reg09 (W): Audio Control 1
	IR01_AUDIO_CTRL2_REG,	// 0x020A Block 2- Reg0A (W): Audio Control 2
	IR01_PW_SCTRL_REG,	// 0x020B Block 2- Reg0B (W): Power and soft control
				// (*): not required if stand-alone mode only. 
				//      These registers are shadowed for retoring them when switching from USB to stand alone mode
};
#define LVSHADOW_LSIZE (sizeof(g_Lv24ShadowList)/sizeof(g_Lv24ShadowList[0]))

// Global for this module
BYTE g_byaShwRegValue[LVSHADOW_LSIZE];	// array to hold the shadowed value

/*-------------------------------------------------------------------
	Treble/Bass converting table
-------------------------------------------------------------------*/
BYTE _rom g_Lv24001Treble[]=
{
	IR1_ACTL2_TREB_N,	// Logical level 0
	0,			// Logical level 1
	IR1_ACTL2_TREB_P,	// Logical level 2
};

#define IMR1_TREBLE_LSIZE (sizeof(g_Lv24001Treble)/sizeof(g_Lv24001Treble[0]) )

BYTE _rom g_Lv24001Bass[]=
{
	//IR1_ACTL2_BASS_N|IR1_ACTL2_BASS_LVL,	// Logical level 0: invalid combination!
	IR1_ACTL2_BASS_N,			// 0	// Logical level 0
	0,					// 1	// Logical level 1
	IR1_ACTL2_BASS_P,			// 2	// Logical level 2
	IR1_ACTL2_BASS_P|IR1_ACTL2_BASS_LVL,	// 3	// Logical level 3
};

#define IMR1_BASS_LSIZE (sizeof(g_Lv24001Bass)/sizeof(g_Lv24001Bass[0]) )

/*-------------------------------------------------------------------
        LV24001 feature limit
-------------------------------------------------------------------*/
typedef struct
{
	BYTE byFeatureId;
	BYTE byUpperLimit;
} LV_FEAT_LIMIT;

LV_FEAT_LIMIT _rom g_Lv2400xFeatLimit[] = 
{
	IHCAP_TUNERPWR,		1,	// Tuner's power: 0=off, 1= On
	IHCAP_VOLUME,		20, 	// Volume level
	IHCAP_AMUTE,		1,	// Audio mute: 0=audio, 1= muted
	IHCAP_ATREBLE,		(IMR1_TREBLE_LSIZE-1),	// Audio treble
	IHCAP_ABASS,		(IMR1_BASS_LSIZE-1),	// Audio bass
	IHCAP_DYNBASSBOOST,	15, 	// Dynamic bass boost
	IHCAP_SMUTE,		7, 	// Audio soft mute

	IHCAP_STEREO,		1, 	// Stereo: 0=mono, 1=stereo
	IHCAP_SOFT_ST,		7, 	// Soft stereo

	IHCAP_RADIOSOURCE,	1, 	// Radio source: 0=Radio off, 1=FM
	IHCAP_EXTSOURCE,	1, 	// External source: 0=off, 1=on
	IHCAP_BEEPSOURCE,	3, 	// Beep tone: 0=off, 1=Freq1, 2=Freq2, 3=Freq3
#if defined(USE_FM_IRQ)	
	IHCAP_AFC,	       	1,	// Bit 14: AFC (automatic frequency control) supported
#endif
	IHCAP_HPA,		1,	// Bit 15: Hardware headphone amplifier presents

	// Software features
	IHCAP_SCANLVL,		7,	// Scan level (0...7)
	IHCAP_SCANWRP,		2,	// 0=no wrap, 1=wrap once, 2=wrap continue
	IHCAP_REGION,		4,	// Region: 0=None, 1=Europe, 2=Japan, 3=USA, 4=JapanWide
	IHCAP_BEEPVOL,		20,	// Beep volume - same limit as IHCAP_VOLUME
/*
#ifdef USE_EXTCLK
	IHCAP_EXTCLK,		1,	// 0= no external clock, 1= 12MHz, 2=32kHz
#endif //USE_EXTCLK
*/
};

#define LV2400X_FEATLIMIT_LSIZE (sizeof(g_Lv2400xFeatLimit)/sizeof(g_Lv2400xFeatLimit[0]))

/*-------------------------------------------------------------------
        LV24001 routines
-------------------------------------------------------------------*/
void Close3wHardwareIo(void);
/*-------------------------------------------------------------------
        LV24001 routines
-------------------------------------------------------------------*/
#if 1
BYTE InitLv2400xChip_PowerOn(void) 
{
	BYTE byResult;
	BYTE i;
    Init3wHardwareIo();

  //  Init3wHardwareIo_PowerOn();
    
	g_dwCnt2ExtClk = 32768;
	g_byBlock = 0xFF;		// Mark no block is selected yet
	g_byHwFlag1 = HF1_NEG_IF_PHASE;	// LV2400x has negative IF phase//这个很重要
	g_byRegion = REGION_MOBILE;	// Default region
	g_byStnFlag = STNWRAP_ONCE;	// Default options
	g_byScanLevel = 2;		// Default scan level
	g_byBeepVol = 10;		// Default beep volume
	g_byDynBassBoost = 15;		// Default: dynamic bass boost level is max.
	
	// 1) Write the default values to the device
	for (i=0; i<LV2400X_DEFAULT_REG_LSIZE; i++)
		WriteReg(g_Lv2400xDefault[i].wRegAddress, g_Lv2400xDefault[i].byValue);

	// 2) Set the prefered IF frequency
	byResult = Set_IF_Freq(DEFAULT_IF_FREQ);
        //SCI_TRACE_LOW:"Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_225_112_2_18_0_32_49_1445,(uint8*)"dd",byResult,g_wLastMsr );
	// 3) Init tuning system
	if (byResult == LVLS_NO_ERROR)
		byResult = InitTuningRfCapOsc(); 
        //SCI_TRACE_LOW:"InitTuningRfCapOsc 0x%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_229_112_2_18_0_32_49_1446,(uint8*)"d",byResult);

	// 4) Set region to initialise FM band limits
	if (byResult == LVLS_NO_ERROR)
		byResult = SetRegion(g_byRegion);
        //SCI_TRACE_LOW:"SetRegion 0x%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_234_112_2_18_0_32_49_1447,(uint8*)"d",byResult);

	// 5) Set the prefered Stereo Decode clock
	Set_SD_Freq(DEFAULT_SD_FREQ);	//  This step can be postboned until stereo is enabled
      //SCI_TRACE_LOW:"Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_238_112_2_18_0_32_49_1448,(uint8*)"dd",byResult,g_wLastMsr);

	g_byHwFlag1 |= HF1_HP;
	
	
	// The chip is now ready - remember that it's still muted
	SetHwFeatureValue(IHCAP_AMUTE,1);//audio mute
	SetHwFeatureValue(IHCAP_HPA,0);//amp off
	SetHwFeatureValue(IHCAP_TUNERPWR,0);//lv chip power off
	
	return(byResult);
}// End InitLv2400xChip

#endif


// App init
BYTE InitLv2400xChip(void) 
{
	BYTE byResult;
	WORD i;
	BYTE reg_value;
	
    Init3wHardwareIo();

    //for(i = 0; i < 2000; i++);
    OS_TickDelay(10);

    
 	// Init software
	g_dwCnt2ExtClk 	 = 32768;
	g_byBlock 		 = 0xFF;					// Mark no block is selected yet
	g_byHwFlag1		 = HF1_NEG_IF_PHASE;		// LV2400x has negative IF phase//这个很重要
	g_byRegion 		 = REGION_US;				// Default region
	g_byStnFlag 	 = STNWRAP_ONCE;			// Default options
	g_byScanLevel 	 = 2;					// Default scan level
	g_byBeepVol 	 = 10;					// Default beep volume
	g_byDynBassBoost = 15;				// Default: dynamic bass boost level is max.

	
	// 1) Write the default values to the device
	for (i=0; i<LV2400X_DEFAULT_REG_LSIZE; i++)
	{
		WriteReg(g_Lv2400xDefault[i].wRegAddress, g_Lv2400xDefault[i].byValue);
	}
      
	// 2) Set the prefered IF frequency
	byResult = Set_IF_Freq(DEFAULT_IF_FREQ);
    //SCI_TRACE_LOW:"Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_286_112_2_18_0_32_49_1449,(uint8*)"dd",byResult,g_wLastMsr );
    
	// 3) Init tuning system
	if (byResult == LVLS_NO_ERROR)
		byResult = InitTuningRfCapOsc(); 
    //SCI_TRACE_LOW:"InitTuningRfCapOsc 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_291_112_2_18_0_32_49_1450,(uint8*)"d",byResult);

	// 4) Set region to initialise FM band limits
	if (byResult == LVLS_NO_ERROR)
		byResult = SetRegion(g_byRegion);
    //SCI_TRACE_LOW:"SetRegion 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_296_112_2_18_0_32_49_1451,(uint8*)"d",byResult);

	// 5) Set the prefered Stereo Decode clock
	Set_SD_Freq(DEFAULT_SD_FREQ);			//  This step can be postboned until stereo is enabled
    //SCI_TRACE_LOW:"Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_300_112_2_18_0_32_49_1452,(uint8*)"dd",byResult,g_wLastMsr);

	g_byHwFlag1 |= HF1_HP;

	reg_value = IoRead3W(0x00);
	//SCI_TRACE_LOW:"InitLv2400xChip: chip id -> %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_305_112_2_18_0_32_49_1453,(uint8*)"d", reg_value);
	if(0x05 != reg_value)
	{
		byResult = 0xFF;
	}
	// The chip is now ready - remember that it's still muted	
	
	return(byResult);
}// End InitLv2400xChip
// Create by Liangwen.zhen on 02/16/2006
void ShutDownLv2400xChip(void)
{
	// Standby supply current: Analog -> 10uA; Digital -> 10uA
	WriteReg(IR01_PW_SCTRL_REG, 0x00);	// Turn off soft stereo, audio mute, headphone amplifier and radio circuitry

	Close3wHardwareIo();
}

// Create by Liangwen.zhen on 02/28/2006
void PowerOnLv2400xChip(void)
{
	// Standby supply current: Analog -> 10uA; Digital -> 10uA
	WriteReg(IR01_PW_SCTRL_REG, IR1_PSCTL_PW_RAD);	// Turn off soft stereo, audio mute, headphone amplifier and radio circuitry
}

/* ************************************************************************************************
 *
 *  Function:   Set_IF_Freq
 *  Authors:    Hung van Le
 *  Purpose:    Set the IF frequency to specified dwInputIF
 *  Input:		
 *			DWORD dwInputIF: the IF frequency to be set in Hz
 *  Output:     Status as defined in LvErr.h
 *  Comments:	None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE Set_IF_Freq(WORD wInputIF)
{
	BYTE byResult;
	BYTE byOrgDemState;
	WORD wCfg;

	// turn on demodulator PLL mute to set the IF
	byOrgDemState = DriveBitNeg(IR01_RADIO_CTRL2_REG, IR1_RCTL2_IF_PM_L, TRUE);


	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(CO_IF_OSC|CHIP_MEASURE_MODE);

	// Do the tuning 
	byResult = LinTuneDac(wInputIF, LV_MSR_TIME_32ms, WriteIfOsc, 50, 150, 1);

	// Tuning done - restore status
	DriveBitNeg(IR01_RADIO_CTRL2_REG, IR1_RCTL2_IF_PM_L, byOrgDemState);

	// Restore chip config
	SetUpChipMode(wCfg);

	return(byResult);
} // End Set_IF_Freq

/* ************************************************************************************************
 *
 *  Function:   Set_SD_Freq
 *  Authors:    Hung van Le
 *  Purpose:    Set the stereo decoder clock frequency to specified dwInputSC
 *  Input:		
 *		DWORD dwInputSC: the stereo decoder clock frequency to be set in Hz
 *  Output:     Status as defined in LvErr.h
 *  Comments:	the set SC will be shadowed in m_dwStereoClock
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE Set_SD_Freq(WORD wInputSC)
{
	BYTE byResult;
	BOOL bOrgPllState;
	WORD wCfg;
	WORD wCurFreq;

	// turn on the stereo PLL mute measure the stereo decoder clock
	bOrgPllState = DriveBit(IR01_STEREO_CTRL_REG, IR1_STCTL_SD_PM, TRUE);

	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(CO_SD_OSC|CHIP_MEASURE_MODE);
		
	// Get current stereo clock
	byResult = CountPulse(LV_MSR_TIME_100ms, &wCurFreq);

	// Skip tuning if the frequency is already good
	if (byResult == LVLS_NO_ERROR)
	{
		if ( IsFrequencyOk(wCurFreq, DEFAULT_SD_FREQ, LV_MSR_TIME_100ms) != 0 )
			byResult = LVLS_NO_ERROR+1;
	}

	// Do the tuning - Some device has dead point below StereoOsc<20, so interpolate between 50-150
	if (byResult != LVLS_NO_ERROR)
		byResult = LinTuneDac(wInputSC, LV_MSR_TIME_100ms, WriteSdOsc, 50, 150, 1);

	// Tuning done - restore status
	DriveBit(IR01_STEREO_CTRL_REG, IR1_STCTL_SD_PM, bOrgPllState);
	SetUpChipMode(wCfg);

	return(byResult);
} // End Set_SD_Freq

/* ************************************************************************************************
 *
 *  Function:   GetOutputSelect
 *
 *  Authors:    Hung van Le
 *  Purpose:    Select an output of IMR01 chip
 *  Input:      Nothing
 *  Output:     The output select
 *  Comments:   The output select is virtualized.
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE GetOutputSelect(void)
{ 
	BYTE byCurOutputSel;

	// Get current measuring mask
	byCurOutputSel = GetSwRegValue(IR01_MSRC_SEL_REG) & IR1_MSRCS_MSS_MASK;
	if (byCurOutputSel == IR1_MSRCS_MSS_FM)
		return(CO_RF_OSC);	// RF frequency
	else if (byCurOutputSel == IR1_MSRCS_MSS_IF)
		return(CO_IF_OSC);	// IF frequency
	else if (byCurOutputSel == IR1_MSRCS_MSS_SD)
		return(CO_SD_OSC);	// SD frequency
	else // other settings
		return(CO_NONE);
} // End GetOutputSelect

/* ************************************************************************************************
 *
 *  Function:   SetOutputSelect
 *
 *  Authors:    Hung van Le
 *  Purpose:    Select an output of IMR01 chip
 *  Input:      
 *		WORD wOutputCfg: desired output (See SetUpChipMode in Lv24Ekit.h for possible outputs)
 *  Output:     The original output select
 *  Comments:   The output select is virtualized.
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD SetOutputSelect(WORD wOutputCfg)
{
	WORD wCurOutputCfg;

	// Determine current chip output
	wCurOutputCfg = GetOutputSelect();

	// Do nothing if no change
	if (wCurOutputCfg == wOutputCfg)
		return(wCurOutputCfg);		

	// Output select change - update chip
	switch (wOutputCfg)
	{
	case CO_NONE:	// Disable all measurements
		SetRegBits(IR01_MSRC_SEL_REG, IR1_MSRCS_MSS_MASK, 0);
		break;

	case CO_RF_OSC:	// RF frequency
		SetRegBits(IR01_MSRC_SEL_REG, IR1_MSRCS_MSS_MASK, IR1_MSRCS_MSS_FM);
		break;

	case CO_IF_OSC:	// IF Osccilator output
		SetRegBits(IR01_MSRC_SEL_REG, IR1_MSRCS_MSS_MASK, IR1_MSRCS_MSS_IF);
		break;

	case CO_SD_OSC:	// Stereo decoder clock output
		SetRegBits(IR01_MSRC_SEL_REG, IR1_MSRCS_MSS_MASK, IR1_MSRCS_MSS_SD);
		break;
	default:
		// Invalid output select
		break;
	}

	return(wCurOutputCfg);
} // End SetOutputSelect

/* ************************************************************************************************
 *
 *  Function:   SetUpChipMode
 *
 *  Authors:    Hung van Le
 *  Purpose:    Prepare/Restore chip mode by starting/ending measurement
 *  Input:      
 *		WORD wCfgIn: desired config (See SetUpChipMode in Lv24Ekit.h for definitions)
 *  Output:     The original config (for restoring chip mode later)
 *  Comments:   Use this routine before measuring chip's frequencies. It manages the measurement mode 
 *		and the AFC state of the hardware
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD SetUpChipMode(WORD wCfgIn)
{
	WORD wCfgOut;

	// Handle the restore flag first
	if (wCfgIn & CHR01_MSR_MODE_FLG	)	// Restore chip measure mode flag set
		DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_EN_MEAS, ((wCfgIn & CHR01_MSR_MODE_ON)!=0));

	// Select the desired oscillator output to measure
	wCfgOut = SetOutputSelect(wCfgIn & CO_SELECT_MASK);

	// Enable measuring with the chip
	if (wCfgIn & CHIP_MEASURE_MODE)
	{
		wCfgOut |= CHR01_MSR_MODE_FLG;	// Mark that we have to restore it later
		if ( DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_EN_MEAS, TRUE) )
			wCfgOut |= CHR01_MSR_MODE_ON;	// Remember the state for restoring
	}

	// Do we have to adjust the AFC state?
	if (wCfgIn & CHIP_SET_AFC )	
	{
		// Yes - Set AFC to new state and remember the previous state for retoring it later
		if ( SetAFCMode((wCfgIn & CHIP_AFC_STATE)!=0) )
			wCfgOut |= CHIP_AFC_ON;
		else
			wCfgOut |= CHIP_AFC_OFF;
	}

	return(wCfgOut);
} // End SetUpChipMode

/* ************************************************************************************************
 *
 *  Function:   Virtualize register access to the hardware
 *			WriteIfOsc (adjust the DAC of IF PLL )
 *			WriteSdOsc (adjust the DAC of Stereo decoder clock)
 *			WriteRfOsc (adjust the DAC of RF varicap)
 *  Authors:    Hung van Le
 *  Purpose:    These functions provide access to registers of the device which are used for setting frequencies
 *  Comments:   None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE WriteIfOsc(WORD wIfOscValue) 
{
	WORD wTmp;

	// Positive IF osc (ie IF frequency increases when the osc value increases)
	if (wIfOscValue > 0xFF)
		return(LVLS_IFOSC_OUTOFLIMIT_ERR);
	WriteReg(IR01_IF_OSC_REG, (BYTE)wIfOscValue);

	// Program IF Center Frequency Oscillator and IF bandwidth according to the value of IF_OSC
	WriteReg(IR01_IFCEN_OSC_REG, (BYTE)wIfOscValue);	// IF center has the same value as IF DAC

	// IF BW is 65% of IF DAC value
	wTmp = (WORD)wIfOscValue*65;
	wTmp /= 100;
	WriteReg(IR01_IF_BW_REG, (BYTE)wTmp);	
	return(LVLS_NO_ERROR);
} // End WriteIfOsc

BYTE WriteSdOsc(WORD wSdOscValue)
{
	// Positive Stereo decoder clock (ie SD frequency increases when the osc value increases)
	if (wSdOscValue > 0xFF)
		return(LVLS_STEREOSC_OUTOFLIMIT_ERR);
	WriteReg(IR01_SD_OSC_REG, (BYTE)wSdOscValue);
	return(LVLS_NO_ERROR);
} // End WriteSdOsc

BYTE WriteRfOsc(WORD wRfOscValue)
{
	// Positive FM osc (ie FM frequency increases when the osc value increases)
	if (wRfOscValue > 0xFF)
		return(LVLS_RFOSC_OUTOFLIMIT_ERR);
	WriteReg(IR01_FM_OSC_REG, (BYTE)wRfOscValue);
	return(LVLS_NO_ERROR);
} // End WriteRfOsc

/* ************************************************************************************************
 *
 *  Function:   WriteRfCap
 *  Authors:    Hung van Le
 *  Purpose:    Virtualized register access to FM-capacitor bank switch
 *  Input:		
 *			WORD wCapValue: the logical CAP value (will be converted to hardware value)
 *  Output:     Status as defined in LvErr.h
 *  Comments:
 *		Write the CAP bank select - using software logical value to deal with 7.5 bits FM CAP
 *			Logical value	Physical value
 *			0...63			255...193
 *			64..191			127...0
 *		Following conversions are used:
 *			- Hardware to software conversion: 
 *				if hw<128 
 *					sw=~(hw+64) 
 *				else 
 *					sw=~hw
 *			- Software to hardware conversion: 
 *				if sw<64 
 *					hw=~sw 
 *				else 
 *					hw=~sw-64
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE WriteRfCap(WORD wCapValue)
{
	// Make sure value is valid
	if ( wCapValue > QSS_MAX_RFCAP ) // Value exceeds limit
		return(LVLS_RFCAP_OUTOFLIMIT_ERR);

	// Convert the software value to hardware value before writing it
	if (wCapValue < 64)
		wCapValue = 255 - wCapValue;	
	else
		wCapValue = 255 - wCapValue - 64;

	// Write value to the chip
	WriteReg(IR01_FM_CAP_REG, (BYTE)wCapValue);
	return(LVLS_NO_ERROR);
} // End WriteRfCap

BYTE GetRfCapValue(void) 
{
	// The shadow register contains the physical value, software expects logical value as return value.
	// so convert the value
	BYTE byLogicalCap, byCurCapValue;

	byCurCapValue = GetSwRegValue(IR01_FM_CAP_REG);
	byLogicalCap = 255 - byCurCapValue;
	if ( byCurCapValue < 128 )
		byLogicalCap -= 64;
	return(byLogicalCap);
} // End GetRfCapValue

BYTE SetAFCMode(BYTE byEnable)
{ 
	// Reset AFC bit = !EnableState
	DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_RST_AFC, !byEnable);

	// Set IOSC_ON bit and return it previous state
	return(DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_EN_AFC, byEnable));
} // End SetAFCMode

BYTE SetDeemphasis(BYTE byDeemp)  
{ 
	return(DriveBit(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_DEEMP75, byDeemp)); 
} // End SetDeemphasis

BYTE SetStereoMono(BOOL bStereo)
{
	// Input: 
	//	bStereo: TRUE: stereo - FALSE: mono
	BYTE byResult;

	// No error yet
	byResult = LVLS_NO_ERROR;

	if (bStereo)	// stereo mode request
	{
		byResult = Set_SD_Freq(DEFAULT_SD_FREQ);	// Set the stereo decoder clock 
	}

	// Enable/Disable stereo according to input
	if (byResult != LVLS_NO_ERROR)
		bStereo = FALSE;	// Force mono mode 
	SetChipStereo(bStereo);

	return(byResult);
} // End SetStereoMono

BYTE SetChipStereo(BYTE byStereo) 
{	
	BYTE byOrgValue;
	
	byOrgValue = DriveBitNeg(IR01_STEREO_CTRL_REG, IR1_STCTL_STEREO_L, byStereo); 
	// Show value
	CallBack(CRSN_MS_CHNG);
	return(byOrgValue);
} // End SetChipStereo

BYTE GetChipVolume(void)
{
	// The LV2400x has 4 bits volume and 1 volume shift bit.
	// The logical levels are defined as follow:
	// Level	Vol[3:0]	VolShiftBit	Range
	// 0...15	0...15		    0		3...48 dB
	// 16..19	12..15		    1		51..60 dB
	BYTE byVolLevel;

	// Fetch the 4 bits volume
	byVolLevel = GetSwRegValue(IR01_AUDIO_CTRL1_REG) & IR1_ACTL1_VOL_LVL;
	
	// Volume bit is inverted @@@@
	byVolLevel = 15 - byVolLevel;

	// Determine the volume shift bit
	if ( GetSwRegValue(IR01_RADIO_CTRL3_REG) & IR1_RCTL3_VOL_SHIFT )
		byVolLevel += 4;
	return(byVolLevel);
} // End GetChipVolume

void SetChipVolume(BYTE byLevel)
{
	// Input: the logical volume level (0..19)
	BYTE byRegValue;
	BOOL bShiftVol;

	if (byLevel<16)
	{
		bShiftVol = FALSE;
	}
	else
	{
		if (byLevel>19)
			byLevel = 19; // Make sure we have valid level
		byLevel -= 4;
		bShiftVol = TRUE;
	}

	// IR01_AUDIO_CTRL1_REG has bit[7:4] as tone control, [3:0] as volume. We want to keep the tone control
	// as same as the volume level until the dynamic bass boost level reached
	byRegValue = (15-byLevel);		// Volume value is the invertion of logical level (volume bits are inverted)
	if (byLevel > g_byDynBassBoost)		// Check tone level against dynamic bass boost
		byLevel = g_byDynBassBoost;	// If exceeds, wrap it back
	byLevel = 15-byLevel;			// The tone value is the invertion of tone level (tone bits are inverted)
	byRegValue |= (byLevel<<4);		// Shift tone value to its position and patch into volume value

	// Update hardware 
	DriveBit(IR01_RADIO_CTRL3_REG, IR1_RCTL3_VOL_SHIFT, bShiftVol);
	WriteReg(IR01_AUDIO_CTRL1_REG, byRegValue);
} // End SetChipVolume

/* ************************************************************************************************
 *
 *  Function:   SetChipAudioMute
 *
 *  Authors:    Hung van Le
 *  Purpose:    Handle the sotware audio mute
 *  Input:      BYTE byMute: new mute state
 *		BYTE byMuteSrc: mute source
 *  Output:     Original (software) audio mute state 
 *  Comments:   Mute state: 0=no mute, 1=muted
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE SetChipAudioMute(BYTE byMute, BYTE bySource)
{
	BYTE byCurState;
	
	// Determine current state (for returning it later)
	if ( (g_bySwFlag1 & bySource)==0 )
		byCurState = 0; // unmute
	else
		byCurState = 1; // mute
	
	if (byMute) // mute request
	{
		// Mute the chip
		DriveBitNeg(IR01_RADIO_CTRL3_REG, IR1_RCTL3_AUDIO_MUTE_L, 1);
		g_bySwFlag1 |= bySource; // register mute source
	}
	else // unmute request
	{
		g_bySwFlag1 &= (~bySource); // register unmute source
		if ( (g_bySwFlag1 & MUTESRC_MASK) == 0) // the chip comes in unmute mode
			DriveBitNeg(IR01_RADIO_CTRL3_REG, IR1_RCTL3_AUDIO_MUTE_L, 0);
	}
	return(byCurState);
} // End SetChipAudioMute

/* ************************************************************************************************
 *
 *  Function:   GetDividerFactor
 *
 *  Authors:    Hung van Le
 *  Purpose:    Determine the (internal) divider factor by current chip output select
 *  Input:      None
 *  Output:     The divider factor belongs to current chip output
 *  Comments:   For this device: the divider factor is 256 for FM output, 1 for other outputs
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD GetDividerFactor(void)
{
	if ( GetOutputSelect() == CO_RF_OSC )
		return(IMR01_FM_DIVIDER);		// usualy 256
	else
		return(1);
} // End GetDividerFactor

BYTE CountPulse(DWORD dwMeasureTimeUs, PWORD pwFreq)
{
	// Decide counter1/Counter2 usage here

	return(CountPulseCnt2(dwMeasureTimeUs, pwFreq)); // Use counter 2 (with swapping) for other cases

} // End CountPulse

/* ************************************************************************************************
 *
 *  Function:   CountPulseCnt1
 *
 *  Authors:    Hung van Le
 *  Purpose:    Counting the pulse (Measure frequencies) of the currently 
 *		selected chip's output. Software is responsible for measuring interval
 *  Input:      
 *		DWORD dwMeasureTimeUs: time to measure in us
 *		PDWORD pwFreq: buffer to receive the measured frequency (unit depends on chip output)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   Frequencies (depend on current output select) are shadowed when this function is invoked
 *		This function prepares the chip for counting pulse (reset counter...) and uses timer 0 interrupt
  *		to do the time critical part (pulse NR_W low for counting).
 *		Then the counter is disabled and can be read back.
 *		The interrupt returns the extra overhead time sothat the frequency can be
 *		calculated.
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE CountPulseCnt1(DWORD dwMeasureTimeUs, PWORD pwFreq)
{
	WORD wStart, wStop;
	WORD wPulseCnt;
	BYTE byTmp;

	// Return value 0 means no pulses counted (chip removed?)
	*pwFreq = 0;

	// Reset counter
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT1_CLR, TRUE);	// Drive Clear counter bit high
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT1_CLR, FALSE);	// Drive Clear counter bit low

	// Create the StartPattern (enable LV2400x counter) and StopPattern (disable LV2400x counter)
	byTmp = GetSwRegValue(IR01_CNT_CTRL_REG);
	byTmp |= IR1_CCTL_CNT_EN;			// Enable counter pattern (start pattern)
	wStart = MAKEWORD(byTmp, LSB(IR01_CNT_CTRL_REG));
	byTmp &= (BYTE)(~IR1_CCTL_CNT_EN);		// Disable counter pattern
	wStop = MAKEWORD(byTmp, LSB(IR01_CNT_CTRL_REG));
			
	// Start LV2400x counter and Pulse the NRW line
	dwMeasureTimeUs = Pulse3wNRW(wStart, wStop, dwMeasureTimeUs);

	// Read counter1 of LV2400x
	wPulseCnt = ReadReg(IR01_CNT_H_REG);	// High byte
	wPulseCnt <<=8;
	wPulseCnt |= ReadReg(IR01_CNT_L_REG);	// Patch low byte

	// Calculate measure frequency
	*pwFreq = PulseToFrequency1(wPulseCnt, dwMeasureTimeUs);

	return(LVLS_NO_ERROR);
} // End CountPulseCnt1

/* ************************************************************************************************
 *
 *  Function:   PulseToFrequency1
 *
 *  Authors:    Hung van Le
 *  Purpose:    Convert 16 bit pulse count to frequency
 *  Input:      
 *		wPulseCnt: 16 bit pulse count value
 *		dwMeasureTimeUs: measured time in us
 *  Output:     The calculated frequency in Hz
 *  Comments:   Use formula f = n * d / t
 *		where:	f: frequency in Hz
 *			n: pulse count
 *			d: divider factor (fetched with GetDividerFactor-function)
 *			t: measured time in us
 *		This function is meant for measuring with counter 1 (software controlled timing)
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
 /*
WORD PulseToFrequency1(WORD wPulseCnt, DWORD dwMeasureTimeUs)
{
	DWORD dwFreqHz;
	DWORD dwUnit;

	switch ( GetOutputSelect() )
	{
		case CO_RF_OSC:
			dwUnit=10000;	// 10 KHz unit for RF
			break;
		case CO_IF_OSC:
			dwUnit=10;	// 10 Hz unit for IF
			break;
		//case CO_SD_OSC:
		default:
			dwUnit=1;	// 1 Hz unit for SD and other freq.
			break;
	}

	dwFreqHz = (DWORD)wPulseCnt * GetDividerFactor();

	// Pure unsigned 32 bit variant
	{
		DWORD dwM, dwK, dwH;
		DWORD dwTmp1, dwTmp2;
	
		dwM = dwFreqHz/dwMeasureTimeUs;			// First dividing gives MHz because t is in us
		dwTmp1 = dwFreqHz%dwMeasureTimeUs;		// Remainder of MHz
		dwTmp2 = dwTmp1/1000;				// Extract KHz
		dwK = (dwTmp2 * 1000000)/dwMeasureTimeUs;	// KHz
		dwH = (dwTmp2 * 1000000)%dwMeasureTimeUs;	// Remainder of KHz
		dwH = (dwH *1000)/dwMeasureTimeUs;		// Hz from KHz
		dwTmp2 = dwTmp1%1000;				// Hz from MHz	
		dwH += ((dwTmp2 * 1000000)/dwMeasureTimeUs);	// Total Hz
		dwFreqHz = dwM*1000000 + dwK*1000 + dwH;	// Total frequency
	}

	// MulDiv variant
	//dwFreqHz = MulDiv(dwFreqHz, 1000000, dwMeasureTimeUs);
	
	// Apply unit
	dwFreqHz /= dwUnit;

	// Shadow the measured frequency to simulates (frequency) read-registers
	ShadowMeasuredFrequency(dwFreqHz);

	return((WORD)dwFreqHz);
} // End PulseToFrequency1
*/
WORD PulseToFrequency1(WORD wPulseCnt, DWORD dwMeasureTimeUs)
{
	DWORD dwFreq;

	switch ( GetOutputSelect() )
	{
	case CO_RF_OSC:
		dwFreq = (DWORD)wPulseCnt * IMR01_FM_DIVIDER;
		dwFreq *= 100; // 10 KHz unit for RF
		dwFreq/= dwMeasureTimeUs;
		/*
		// Round the measured frequency up to 10 kHz - this must also be done to all RF-usage (example. in FindFmStation)
		dwFreq = (DWORD)wPulseCnt * IMR01_FM_DIVIDER;
		dwFreq *= 1000; // RF: Calculate in kHz to round it up to 10 KHz unit later
		dwFreq/= dwMeasureTimeUs;	// RF freq is now in kHz
		dwFreq = (dwFreq+5)/10;	// 10 KHz unit for RF (with up rouding)
		*/
		break;
	case CO_IF_OSC:
		dwFreq = (DWORD)wPulseCnt * 100000; // 10 Hz unit for IF
		dwFreq/= dwMeasureTimeUs;
		break;
	//case CO_SD_OSC:
	default:
		// 1 Hz unit for SD and other freq.
		dwFreq = (DWORD)wPulseCnt;
		// Pure unsigned 32 bit variant
		{
			DWORD dwM, dwK, dwH;
			DWORD dwTmp1, dwTmp2;
	
			dwM = dwFreq/dwMeasureTimeUs;			// First dividing gives MHz because t is in us
			dwTmp1 = dwFreq%dwMeasureTimeUs;		// Remainder of MHz
			dwTmp2 = dwTmp1/1000;				// Extract KHz
			dwK = (dwTmp2 * 1000000)/dwMeasureTimeUs;	// KHz
			dwH = (dwTmp2 * 1000000)%dwMeasureTimeUs;	// Remainder of KHz
			dwH = (dwH *1000)/dwMeasureTimeUs;		// Hz from KHz
			dwTmp2 = dwTmp1%1000;				// Hz from MHz	
			dwH += ((dwTmp2 * 1000000)/dwMeasureTimeUs);	// Total Hz
			dwFreq = dwM*1000000 + dwK*1000 + dwH;	// Total frequency
		}
		// MulDiv variant
		//dwFreqHz = MulDiv(dwFreq, 1000000, dwMeasureTimeUs);
		break;
	}
	
	// Shadow the measured frequency to simulates (frequency) read-registers
	ShadowMeasuredFrequency(dwFreq);

	return((WORD)dwFreq);
} // End PulseToFrequency1





// 2005-03-03
/* ************************************************************************************************
 *
 *  Function:   GetcTabValue
 *  -Para1   :   dwMeasureTimeUs
 *
 * ************************************************************************************************ */
void cTabSetting(DWORD dwMeasureTimeUs ,PDWORD pdwPulseCnt2,PWORD pwRegValue)
{
	if(dwMeasureTimeUs == 100000){
		*pdwPulseCnt2 = 2048;
		*pwRegValue = IR1_CTAB_2048;
	}else if((dwMeasureTimeUs == 64000)||(dwMeasureTimeUs == 32000)){
		*pdwPulseCnt2 = 512;
		*pwRegValue = IR1_CTAB_512;
	}else{
		*pdwPulseCnt2 = 128;
		*pwRegValue = IR1_CTAB_128;
	}
}
/* ************************************************************************************************
 *
 *  Function:   CountPulseCnt2
 *
 *  Authors:    Hung van Le
 *  Purpose:    Counting the pulse (Measure frequencies) of the currently 
 *		selected chip's output. Measuring interval is made with external clock
 *  Input:      
 *		PWORD pwFreq: buffer to receive the measured frequency (unit depends on chip output)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   See also CountPulseCnt1
 *		The code is fixed for 12MHz external clock (counter swapping enabled)
 *		Counter 2 usage
 *			RF (100 MHz): IR1_CTAB_128, 128 (measuring time 0.66ms - 127ppm)
 *			RF (100 MHz): IR1_CTAB_512, 512 (measuring time 2.62ms - 32ppm)
 *
 *			IF (45  kHz): IR1_CTAB_128, 128 (overflow !)
 *			IF (100 kHz): IR1_CTAB_128, 128 (measuring time 2.56ms - 33ppm)
 *			
 *			IF (45  kHz): IR1_CTAB_32, 32 (measuring time 1.42ms - 59ppm)
 *			IF (100 kHz): IR1_CTAB_32, 32 (measuring time 0.64ms - 130ppm)
 *			
 *			SD (38  kHz): IR1_CTAB_32, 32 (measuring time 1.68ms - 49ppm)
 *			
 *			RDS(57  kHz): IR1_CTAB_32, 32 (measuring time 1.12ms - 74ppm)
 *			
 *		Because of short measuring time, this method is not suitable for measuring IF/SD/RDS PLL 
 *		in lock mode.
 *		In free running mode (calibration mode), this method can be used.
 *
 *		RF is not PLL, so this methode always works
 *		We use following:
 *			RF 128
 *			IF/SD in free running mode: 32
 *			IF in lock mode: software window
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE CountPulseCnt2(DWORD dwMeasureTimeUs, PWORD pwFreq)
{
	DWORD dwResult;
	DWORD dwPulseCnt2, dwTmp;
	WORD  bSwapCnt;
	WORD  wPulseCnt1, wTmp, wRegValue;
	WORD  byCntSelState;
	WORD wOrgIrqReg;

	wOrgIrqReg = 0;
	dwResult = LVLS_NO_ERROR;
    
	// Find out counter 2 method: swap or no swapping
	if(g_dwCnt2ExtClk < 100000)
		bSwapCnt = FALSE;		
	else
		bSwapCnt = TRUE; // Swapped

	wPulseCnt1 = 0;
	dwPulseCnt2 = 0;
	wRegValue = 0;	

	// Setting Ctab value
	cTabSetting(dwMeasureTimeUs ,&dwPulseCnt2,&wRegValue);

	// Clear any pending interrupt of LV24xxx
	ReadReg(IR01_CTRL_STAT_REG);	// Read IR01_CTRL_STAT_REG to clear AFC interrupt
	ReadReg(IR01_RADIO_STAT_REG);	// Read IR01_RADIO_STAT_REG to clear field strength, Stereo interrupt
	
	// Reset counter 1
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT1_CLR, TRUE);	// Drive Clear counter bit of high
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT1_CLR, FALSE);	// Drive Clear counter bit of low

	// Create the StartPattern (enable LV2400x counter) and StopPattern (disable LV2400x counter)
	byCntSelState = GetSwRegValue(IR01_CNT_CTRL_REG);
	
	// Program swap counter bit, tab bit, select counter 2
	DriveBitNeg(IR01_CNT_CTRL_REG, IR1_CCTL_SWP_CNT_L, bSwapCnt);	// Swap counter
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT_SEL, TRUE);			// Select counter 2
	
	SetRegBits(IR01_CNT_CTRL_REG, IR1_CCTL_CNT_TAB,wRegValue); // counter Tab setting
//test
//wTmp = ReadReg(IR01_IRQ_ID_REG);
//SCI_TRACE_LOW("before wTmp=%d\n",wTmp);	
	// Enable counter to start counting
	DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT_EN, TRUE);

	// The Counter2 done is known by Polling
	dwTmp=120;
	do
	{
		wTmp = ReadReg(IR01_IRQ_ID_REG);
		if (wTmp & IR1_IRQID_CNT2)	// Counting done bit set
			break;
		dwTmp--;
		DelayUs(DLT_1ms);
	} while (dwTmp>0);
	//SCI_TRACE_LOW("after dwTmp = %d wTmp=%d\n",dwTmp,wTmp);
	// Check timeout
	if (dwTmp==0)
	{
		dwResult = LVLS_POLLING_CNT_TIMEOUT;
	}
	else
	{
		dwResult = LVLS_NO_ERROR;
	}
		
	// The Counter2 done is known by interrupt ** Add Kaori T **
	if(dwResult == LVLS_NO_ERROR)
	{
		// Counting done - Disable the counter (also clear IR1_IRQID_CNT2 bit)	
		DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT_EN, FALSE);		

		// Fetch pulse count by reading the counter 1 value
		wPulseCnt1 = 0;
		wTmp =ReadReg(IR01_CNT_L_REG);	// Low byte
		if (wTmp != 0)
		{
			wPulseCnt1 = wTmp;	// Store low byte
			wTmp = ReadReg(IR01_CNT_H_REG);
			wPulseCnt1 |= (wTmp<<8); // Patch high byte
		}
		//SCI_TRACE_LOW("wPulseCnt1 = %d \n",wPulseCnt1);
		// restore counter select bit
		DriveBit(IR01_CNT_CTRL_REG, IR1_CCTL_CNT_SEL, byCntSelState);
		WriteReg(IR01_IRQ_MSK_REG,wOrgIrqReg);
		// Calculate and save the measured frequency to simulate the (frequency) read-register

		// Formula to calculate frequency from pulse counts
		//	if NoSwapping = 0
		//		f = (N * fext * DividerFactor) / (CTAB * 2)
		//  if Swapping = 1
		//		f = (fext * CTAB * 2) / N
		*pwFreq = PulseToFrequency2((DWORD)wPulseCnt1, dwPulseCnt2, g_dwCnt2ExtClk);
	}
	return(dwResult);	
} // End CountPulseCnt2

DWORD PulseToFrequency2(DWORD wPulseCnt, DWORD dwCtab, DWORD dwFext)
{
	DWORD dwFreq;
	
	// Avoid dividing by 0
	dwFreq = 0;
	if((dwFext == 32768)&&(dwCtab!=0)){
		switch ( GetOutputSelect() )
		{
		case CO_RF_OSC:
			dwCtab = (DWORD)dwCtab * IMR01_CNT2_PRESCALER;
			dwFreq = (DWORD)(wPulseCnt * dwFext);
			dwFreq = (DWORD)dwFreq/ dwCtab;
			dwFreq = (DWORD)dwFreq * IMR01_FM_DIVIDER;
			dwFreq = (DWORD)dwFreq/10000;
			break;
		case CO_IF_OSC:
			// Double variant
			dwCtab*=IMR01_CNT2_PRESCALER;
			dwFreq = (DWORD)( (wPulseCnt * dwFext)/(dwCtab));
			dwFreq = (DWORD)dwFreq /10;
			break;
		//case CO_SD_OSC:
		default:
			// Double variant
			dwCtab*=IMR01_CNT2_PRESCALER;
			dwFreq = (DWORD)( (wPulseCnt * dwFext)/(dwCtab));
			dwFreq = (DWORD)dwFreq ;//remove div 10
			break;
		}
	}
	// Shadow the measured frequency to simulates (frequency) read-registers 
	ShadowMeasuredFrequency(dwFreq);

	return((WORD)dwFreq);
} // End PulseToFrequency2












BYTE GetFmFieldStrength(void)
{
	BYTE byRegValue;
	BYTE byOrgMode;
	BYTE byFs;
		
	// Make sure the chip is in measuring mode for measure field strenggth
	byOrgMode = DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_EN_MEAS, TRUE);

	// Read Radio status register to determine field strength
	byRegValue = ReadReg(IR01_RADIO_STAT_REG);

	// Extract FieldStrength
	byRegValue &= IR1_RSTAT_FS; // extract field strength bits
	byRegValue ^= IR1_RSTAT_FS; // Invert field strength level (0 becomes 1)
	
	byFs = 0; // field strength to be returned
	while (byRegValue != 0)
	{
		if ( byRegValue & 1 )
			byFs++;
		byRegValue>>=1;
	}
	
	// Restore measure mode
	DriveBit(IR01_RADIO_CTRL1_REG, IR1_RCTL1_EN_MEAS, byOrgMode);

	return(byFs);
} // End GetFmFieldStrength

BYTE GetStereoState(void)
{
	// Determine stereo state of LV2400x
	// Return: GSS_MONO: mono mode
	//	   GSS_STEREO: stereo is enabled but no stereo recieving
	//	   GSS_STEREO_DET: stereo is enabled and stereo detected

	// Determine stereo enable state
	if ( (GetSwRegValue(IR01_STEREO_CTRL_REG) & IR1_STCTL_STEREO_L) == 0 ) // Stereo bit = 0: stereo is enabled
	{
		if (ReadReg(IR01_RADIO_STAT_REG) & IR1_RSTAT_STEREO) 	// Stereo state bit = 1 (Stereo)
			return(GSS_STEREO_DET);		// Stereo is detected by the chip
		else
			return(GSS_STEREO);		// Stereo is enabled but not detected
	}
	else
		return(GSS_MONO);
} // End GetStereoState

/* ************************************************************************************************
 *
 *  Function:   ShadowMeasuredFrequency
 *
 *  Authors:    Hung van Le
 *  Purpose:    Simulates some (frequency) read-registers which are missing in the chip
 *  Input:      
 *		DWORD dwFreqHz: frequency (in Hz) to be shadowed
 *  Output:     None
 *  Comments:   Use GetOutputSelect to determine the frequency source
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
void ShadowMeasuredFrequency(DWORD dwFreqHz)
{
	// Only save the RF frequency
	if ( GetOutputSelect() == CO_RF_OSC)
	{
		g_wLastMsrRF = dwFreqHz; 
	}
	g_wLastMsr = dwFreqHz; 
} // End ShadowMeasuredFrequency

BYTE SetRfFrequency(WORD wRfFreq)
{
	BYTE byResult;
	BYTE byOrgAudioMute, byOrgAudioMode;
	WORD wCfg;

	// Check limit
	{
		WORD wRfBandLo, wRfBandHi;

		wRfBandHi = DisplayFreqToRf(g_wDisplayBandHigh);
		wRfBandLo = DisplayFreqToRf(g_wDisplayBandLow);
		byResult = CheckRfLimit(&wRfFreq, wRfBandLo, wRfBandHi, 2);
		if (byResult != LVLS_NO_ERROR)
			return(byResult);
	}

	// No valid station by a frequency set
	g_byStnFlag &= (~STN_VALID);

	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(CO_RF_OSC|CHIP_MEASURE_MODE|CHIP_AFC_OFF);

	// Mute audio and force mono mode
	byOrgAudioMute = SetChipAudioMute(TRUE, MUTESRC_OPERATION);
	byOrgAudioMode = SetChipStereo(FALSE);

	// Save the new RF frequency
	g_wLastSetRF = wRfFreq;

	// Quick frequency set
	{
		WORD wCap, wOsc;
		
		CalculateRfCapOsc(wRfFreq, &wCap, &wOsc, QSSF_PRECISION_MED);
		//WriteRfCap(wCap); // Will be done in FineTuneRfOsc
		byResult = FineTuneRfOsc(wRfFreq, LV_MSR_TIME_32ms, wOsc, FALSE);
	}

	// Do finetuning if frequency is set
	//if ( (dwResult == LVLS_NO_ERROR) && (dwOption & SETSTN_FINETUNE) )
	//	FineTuneFmToIF(&m_bValidStation);

	// Restore chip config
	SetUpChipMode(wCfg);
	// Restore status
	SetChipAudioMute(byOrgAudioMute, MUTESRC_OPERATION);
	SetChipStereo(byOrgAudioMode);

	return(byResult);
} // End SetRfFrequency

BYTE GetHwFeatureLimit(BYTE byFeatureId)
{
	// Return upper limit of a feature (lower limit is always 0)
	// Upper limit 0 means feature is not supported
	BYTE i;

	// Headphone amplifier: depends on hardware capapbility
	if (byFeatureId == IHCAP_HPA)
	{
		if ( (g_byHwFlag1 & HF1_HP)==0 )
			return(0);	// Not supported by hardware
	}

	for (i=0; i<LV2400X_FEATLIMIT_LSIZE; i++)
	{
		if ( g_Lv2400xFeatLimit[i].byFeatureId == byFeatureId)
			return(g_Lv2400xFeatLimit[i].byUpperLimit);
	}
	return(0); // Not support - no upper limit
} // End GetHwFeatureLimit

BYTE GetHwFeatureValue(BYTE byFeatureId)
{
	// Return current value of a feature
	BYTE byTmp, byValue;

	// Default returned value
	byValue=0;

	switch (byFeatureId)
	{
	case IHCAP_TUNERPWR:
		byTmp = GetSwRegValue(IR01_PW_SCTRL_REG);
		if ( byTmp & IR1_PSCTL_PW_RAD )	// FM power on bit = 1: power is on
			byValue = 1;
		break;

	case IHCAP_AMUTE:
		if ( g_bySwFlag1 & MUTESRC_APPLICATION )
			byValue = 1; // Muted
		break;

	case IHCAP_VOLUME:
		byValue = GetChipVolume();
		if ( (g_bySwFlag1 & MUTESRC_VOLUME)==0 )
			byValue++;	// Convert hardware level to logical level (add 1 when we are not muted)
		break;

	case IHCAP_STEREO:
		byTmp = GetSwRegValue(IR01_STEREO_CTRL_REG);
		if ( (byTmp & IR1_STCTL_STEREO_L) == 0)	// Stereo bit = 0: stereo is enabled
			byValue=1;
		break;

//	case IHCAP_DEEMP:
//		wTmp = GetSwRegValue(IR01_AUDIO_CTRL2_REG);
//		if ( wTmp & IR1_ACTL2_DEEMP75 )	// Deemphasis bit = 1: 75us
//			pValue->State.bState = DEEMP_75;
//		else
//			pValue->State.bState = DEEMP_50;
//		break;

	case IHCAP_SMUTE:		// Soft mute
		// Soft mute is in bit [4:2] of IR01_PW_SCTRL_REG
		byTmp = (BYTE)(GetSwRegValue(IR01_PW_SCTRL_REG) & IR1_PSCTL_SOFT_MUTE);
		byValue = byTmp>>2; // Shift to logical level
		break;

	case IHCAP_SOFT_ST:		// Soft stereo
		// Soft stereo is in bit [7:5] of IR01_PW_SCTRL_REG
		byTmp = (BYTE)(GetSwRegValue(IR01_PW_SCTRL_REG) & IR1_PSCTL_SOFT_ST);
		byValue = byTmp>>5; // Shift to logical level
		break;
#if defined(USE_FM_IRQ)	

	case IHCAP_AFC:			// AFC (automatic frequency control)
		byTmp = GetSwRegValue(IR01_RADIO_CTRL1_REG);
		if (byTmp & IR1_RCTL1_EN_AFC)
			byValue = 1;
		break;
#endif
	case IHCAP_ATREBLE:		// Audio treble
		// Fetch current setting
		byTmp = (BYTE)(GetSwRegValue(IR01_AUDIO_CTRL2_REG) & IR1_ATREBLE_MASK);
		
		// Return the logical level
		byValue = SearchInByteArray(byTmp, g_Lv24001Treble, IMR1_TREBLE_LSIZE);
		break;

	case IHCAP_ABASS:		// Audio bass
		// Fetch current setting
		byTmp = (BYTE)(GetSwRegValue(IR01_AUDIO_CTRL2_REG) & IR1_ABASS_MASK);
		
		// Return the logical level
		byValue = SearchInByteArray(byTmp, g_Lv24001Bass, IMR1_BASS_LSIZE);
		break;

	case IHCAP_RADIOSOURCE:
		// Return: 0: off, 1: FM
		byTmp = GetSwRegValue(IR01_RADIO_CTRL3_REG);
		if (byTmp & IR1_RCTL3_SE_FM)
			byValue = 1;
		break;
		
	case IHCAP_EXTSOURCE:
		// Input: on/off (true/false)
		byTmp = GetSwRegValue(IR01_RADIO_CTRL3_REG);
		if (byTmp & IR1_RCTL3_SE_EXT)
			byValue = 1;
		break;

	case IHCAP_BEEPSOURCE:
		// Beep source: check beep source enable bit and beep tone select bits
		// Source enable	BeepSelect		HW beep Value
		//	Disable		X			off (=0)
		//	Enable		Beep_High		off
		//	Enable		500/1000/2000Hz		1,2,3

		// Fist beep source enable bit
		byTmp = GetSwRegValue(IR01_RADIO_CTRL3_REG);
		if ( byTmp & IR1_RCTL3_SE_BEEP )		// Beep source is enabled
		{
			byTmp = (BYTE)(GetSwRegValue(IR01_AUDIO_CTRL2_REG) & IR1_ACTL2_BPFREQ);
			if (byTmp == IR1_BEEP_500Hz) 		// 0.5 kHz beep tone
				byValue = 1;
			else if (byTmp == IR1_BEEP_1KHz)	// 1 kHz beep tone
				byValue = 2;
			else if (byTmp == IR1_BEEP_2KHz)	// 2 kHz beep tone
				byValue = 3;
		}
		break;

	case IHCAP_DYNBASSBOOST:	// Dynamic bass boost feature
		byValue = g_byDynBassBoost;
		break;

	case IHCAP_HPA:			// Headphone amplifier
		if ( g_byHwFlag1 & HF1_HP )
		{
			if ( GetSwRegValue(IR01_PW_SCTRL_REG) & IR2_PSCTL_HPA )
				byValue=1;
		}
		break;

//	case IHCAP_CNT2:

	// Software settings
	case IHCAP_SCANLVL:	// Scan level
		byValue = g_byScanLevel;
		break;

	case IHCAP_SCANWRP:	// Scan wrapping
		byValue = (BYTE)(g_byStnFlag & STN_WRAP_MODE);
		break;

	case IHCAP_REGION:	// Region
		byValue = g_byRegion;
		break;

	case IHCAP_BEEPVOL:	// Beep volume
		byValue = g_byBeepVol;
		break;
/*
#ifdef USE_EXTCLK
	case IHCAP_EXTCLK:
		byValue = (BYTE)(g_bySwFlag1 & SF1_EXTCLK);
		break;
#endif //USE_EXTCLK
*/
	default:
		break;
		//break;
	} // EndSwitch

	return(byValue);
} // End GetHwFeatureValue
/* ************************************************************************************************
 *
 *  Function:   SetHwFeatureValue
 *
 *  Authors:    Hung van Le
 *  Purpose:    Setting capability value of the device
 *  Input:	byFeatureId: see Lv24Ekit.h for definitions
 *		pbyValue: pointer to the value to be set
 *  Output:     Status as defined in LvErr.h
 *  Comments:   None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE SetHwFeatureValue(BYTE byFeatureId, BYTE byValue)
{
	BYTE byResult;
	BYTE byTmp;
	
	return 0;
	// Init local
	byResult = LVLS_NO_ERROR;
	
	switch (byFeatureId)
	{
	case IHCAP_TUNERPWR:
		DriveBit(IR01_PW_SCTRL_REG, IR1_PSCTL_PW_RAD, byValue);
		break;

	case IHCAP_AMUTE:
		SetChipAudioMute(byValue, MUTESRC_APPLICATION);
		break;

	case IHCAP_VOLUME:
		AdjustVolume(byValue);
		break;

	case IHCAP_STEREO:
		byResult = SetStereoMono(byValue);
		break;

	//case IHCAP_DEEMP:
	//	SetDeemphasis(byValue);
	//	break;

	case IHCAP_SMUTE:		// Soft mute
		// Soft mute is in bit [4:2] of IR01_PW_SCTRL_REG
		byTmp = (BYTE)((byValue) & 0x7);	// Make sure we have valid level
		SetRegBits(IR01_PW_SCTRL_REG, IR1_PSCTL_SOFT_MUTE, (byTmp<<2));
		break;

	case IHCAP_SOFT_ST:		// Soft stereo
		// Soft stereo is in bit [7:5] of IR01_PW_SCTRL_REG
		byTmp = (BYTE)((byValue) & 0x7);	// Make sure we have valid level
		SetRegBits(IR01_PW_SCTRL_REG, IR1_PSCTL_SOFT_ST, (byTmp<<5));
		break;
#if defined(USE_FM_IRQ)	
	case IHCAP_AFC:		// AFC (automatic frequency control)
		SetAFCMode(byValue);
		break;
#endif
	case IHCAP_ATREBLE:		// Audio treble
		if ( byValue >= IMR1_TREBLE_LSIZE)
			byResult = LVLS_INV_FEATURE_VALUE_ERR;
		else
		{
			SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ATREBLE_MASK, g_Lv24001Treble[byValue]);
			AdjustHwToneControl();
		}
		break;

	case IHCAP_ABASS:		// Audio bass
		if ( byValue >= IMR1_BASS_LSIZE)
			byResult = LVLS_INV_FEATURE_VALUE_ERR;
		else
		{
			SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ABASS_MASK, g_Lv24001Bass[byValue]);
			AdjustHwToneControl();
		}
		break;

	case IHCAP_RADIOSOURCE:
		// Input: 0: off, 1: FM, 2: AM
		DriveBit(IR01_RADIO_CTRL3_REG, IR1_RCTL3_SE_FM, byValue);
		break;

	case IHCAP_EXTSOURCE:
		// Input: 0=off 1=on
		DriveBit(IR01_RADIO_CTRL3_REG, IR1_RCTL3_SE_EXT, byValue);
		break;

	case IHCAP_BEEPSOURCE:
		// Beep source: Driving beep source enable bit and beep tone select bits
		// HW beep Value	Source enable	BeepSelect
		//	0 (off)		Disable		High    
		//	1,2,3		Enable		500/1000/2000Hz

	       	// Adjust registers according to input value
		if ( byValue==1)		// Beep at 500 Hz
			byTmp = IR1_BEEP_500Hz;
		else if ( byValue==2)	// Beep at 1000 Hz
			byTmp = IR1_BEEP_1KHz;
		else if ( byValue==3)	// Beep at 2000 Hz
			byTmp = IR1_BEEP_2KHz;
		else
			byTmp = IR1_BEEP_OFF;		// 0 or unknown values: turn off

		// Change beep frequency select bits
		SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, byTmp);
			
		// Change beep source enable bit
		DriveBit(IR01_RADIO_CTRL3_REG, IR1_RCTL3_SE_BEEP, (byTmp != IR1_BEEP_OFF));
		break;

	/*
	case IHCAP_BEEPBIT:
		if (pValue->Beep.byDuration != SET_BEEP_OFF) // Always OK to turn off
		{
			if (GetSwRegValue(IR01_RADIO_CTRL3_REG) & IR1_RCTL3_SE_BEEP ) // Hardware beep busy - can not set software beep value
				dwResult = LVLS_IMPROPER_HW_SETTING_ERR;
		}
		if (dwResult == LVLS_NO_ERROR)
		{
			// Update the shadow software beep state
			if (pValue->Beep.byDuration == SET_BEEP_OFF) // Always OK to turn off
				m_bSwBeepOn = false;
			else
				m_bSwBeepOn = true;
			dwResult = BeepHandler(pValue->Beep.wFreqHz, pValue->Beep.byDuration);
		}
		break;
	*/
	case IHCAP_DYNBASSBOOST:	// Dynamic bass boost feature
		if (g_byDynBassBoost != byValue )
		{
			BYTE byTone, byVolume;

			// Dynamic bass level changed
			g_byDynBassBoost = byValue;		// Fecth new dynamic bass boost level

			// IR01_AUDIO_CTRL1_REG has bit[7:4] as tone control, [3:0] as volume. 
			// Set the tone control level if it is not correct
			byVolume = GetSwRegValue(IR01_AUDIO_CTRL1_REG);		// Fetch tone and volume value
			byTone = (BYTE)((byVolume & IR1_ACTL1_TONE_LVL)>>4);	// extract tone value
			byVolume &= (IR1_ACTL1_VOL_LVL);			// extract volume value
			byVolume = 15-byVolume;					// IMR01 has volume bits inverted - we have now the logical volume level
			byTone = 15 - byTone;									// The 4 tone bits are inverted - we have now the logical tone level
			// Modify tone when it's not equal to dynamic bass level
			if ( byTone != g_byDynBassBoost )
			{
				if (byVolume < g_byDynBassBoost )	// Logical volume level doesn't exceed dynamic bass boost level
					byTone = byVolume;		// Let tone level follow volume level
				else					// Volume level exceeds dynamic bass boost
					byTone = g_byDynBassBoost;	// Keep tone level fixed at dynamic bass boost level
				byTone = 15 - byTone;			// Convert tone level to tone value (the 4 tone bits are inverted)
				byTone <<= 4;				// Shift tone to correction position
				SetRegBits(IR01_AUDIO_CTRL1_REG, IR1_ACTL1_TONE_LVL, byTone);
			}
		}
		break;

	case IHCAP_HPA:	// Headphone amplifier
		// Input: 0=off 1=on
		// Check if headphone support
		if ( g_byHwFlag1 & HF1_HP)
			DriveBit(IR01_PW_SCTRL_REG, IR2_PSCTL_HPA, byValue);
		break;
		
//	case IHCAP_CNT2:
//  		m_dwCnt2ExtClk = pValue->Cnt2.dwExtClock; // Save the external clock (in Hz) for later usage
//		break;

	// Software settings
	case IHCAP_SCANLVL:	// Scan level
		g_byScanLevel = byValue;
		break;

	case IHCAP_SCANWRP:	// Scan wrapping
		g_byStnFlag &= (~STN_WRAP_MODE);	// clear old setting
		g_byStnFlag |= (byValue & STN_WRAP_MODE);
		break;

	case IHCAP_REGION:	// Region
		byResult = SetRegion(byValue);
		break;

	case IHCAP_BEEPVOL:	// Audible feedback volume
		g_byBeepVol = byValue;
/*
#ifdef USE_EXTCLK
	case IHCAP_EXTCLK:
		g_bySwFlag1 &= (BYTE)(~SF1_EXTCLK);
		g_bySwFlag1 |= (BYTE)(byValue & SF1_EXTCLK);
		break;
#endif //USE_EXTCLK
*/
	default:
		byResult = LVLS_NSU_FEATURE_ERR;
		break;
	} // EndSwitch
	return(byResult);
} // End SetHwFeatureValue

void AdjustHwToneControl(void)
{
	// Control the ToneMute bit (IR1_RCTL3_TONE_MUTE_L of IR01_RADIO_CTRL3_REG)
	// Unmute the tone control if bass or treble is not 0
	BYTE byTmp;
	BOOL bState;
		
	byTmp = GetSwRegValue(IR01_AUDIO_CTRL2_REG);
	
	if (byTmp & (IR1_ATREBLE_MASK|IR1_ABASS_MASK)) // One or more tone bit is set
		bState = FALSE;	// Don't mute the tone
	else
		bState = TRUE;	// All tone bits are 0, mute the tone control
	DriveBitNeg(IR01_RADIO_CTRL3_REG, IR1_RCTL3_TONE_MUTE_L, bState);
} // End AdjustHwToneControl


#if defined(USE_FM_IRQ)//removed by Eric.wei
//////////////////////////////////////////////////////////////////////
// LV24xxx Interrupt functions
//////////////////////////////////////////////////////////////////////
void EnableLv2400xIrq(void)
{
	// Enable interrupt of 3-wire bus 1 (INT5 (vector 1Bh) - rising edge trigger)
	// We use following LV24xxx interrupts:
	//	- Field strength
	//	- Stereo/Mono
	// 	- AFC out of range

	// Clear any pending interrupt of LV24xxx
	ReadReg(IR01_CTRL_STAT_REG);	// Read IR01_CTRL_STAT_REG to clear AFC interrupt
	ReadReg(IR01_RADIO_STAT_REG);	// Read IR01_RADIO_STAT_REG to clear field strength, Stereo interrupt

	// Program the LV24xxx interrupt level: high active, enable Mono/Stereo, fieldstrength and AFC interrupt
	WriteReg(IR01_IRQ_MSK_REG, IR1_IRQM_ACT_HIGH|IR1_IRQM_EN_MS|IR1_IRQM_EN_FS|IR1_IRQM_EN_AFC);

	// Write to IR01_IRQ_OUT_REG-register to select interrupt output of LV24xxx
	WriteReg(IR01_IRQ_OUT_REG, 0);

	// Enable system interrupt INT5
	Enable3wIrq(TRUE);

	// Mark chip interrupt is on
	g_byLvIrqFlg |= IFL_CHIP_IRQ_EN;
} // End EnableLv2400xIrq

void DisableLv2400xIrq(void)
{
	// Disable system interrupt INT5
	Enable3wIrq(FALSE);
} // End DisableLv2400xIrq

void HandleLv2400xIrq(void)
{
	// We handle following LV24xxx interrupts:
	//	- Field strength: Update LCD display
	//	- Stereo/Mono: Update LCD display
	// 	- AFC out of range: re-tune the frequency
	BYTE byEnMask, byIrqOcc, byTmp1;

	// Fetch the interrupt enable mask
	byEnMask = GetSwRegValue(IR01_IRQ_MSK_REG);

	// Read the interrupt ID register (see what interrupt occurred)
	byIrqOcc = ReadReg(IR01_IRQ_ID_REG);

	// Handle the interrupts
	if ( byIrqOcc & (IR1_IRQID_FS_MS) ) // Mono or fieldstrength
	{
		// Read IR01_RADIO_STAT_REG to clear field strength, Stereo interrupt
		byTmp1 = ReadReg(IR01_RADIO_STAT_REG);	

		// Hanlde field strength interrupt
		if (byEnMask & IR1_IRQM_EN_FS)	// Was field strength interrupt enabled?
		{
			CallBack(CRSN_FS_CHNG);
		} // End fieldstrength interrupt handling

		// Hanlde Mono/Stereo interrupt
		if (byEnMask & IR1_IRQM_EN_MS)	// Was its interrupt enabled?
		{
			CallBack(CRSN_MS_CHNG);
		} // End Mono/Stereo interrupt handling
	} // EndIf Field strength/Mono-stereo

	// AFC interrupt
	if (byIrqOcc & IR1_IRQID_AFC)	// AFC interrupt occurred?
	{
		if (byEnMask & IR1_IRQM_EN_AFC)	// Was its interrupt enabled?
		{
			CallBack(CRSN_AFC_FAILED);
		}
	}
} // End HandleLc2400xIrq
#endif
/* ************************************************************************************************
 *
 *  Function:   AudioFeedback
 *
 *  Authors:    Hung van Le
 *  Purpose:	Generate beep with the LV2400x
 *  Input:	byType: pre-defined beep type
 *			AUDFB_TYPE1: tone 500 Hz for 40ms
 *			AUDFB_TYPE2: tone 2kHz for 40ms, pause 40ms, tone 2kHz for 40ms
 *  Comments:	- This function accesses the registers of LV2400x directly. These accesses can be moved to 
 *		  chip specific layer when multiple chip support is needed. For example the Stereo decoder PLL can be 
 *		  muted through a chip function like BOOL MuteStereoDecoderPll(BOOL bState) instead of driving the bit 
 *		  directly.
 *		- Function SetHwFeatureValue(IHCAP_BEEPSOURCE, byValue) can also be used to avoid accessing the LV2400x 
 *		  directly. byValue for LV2400x is: 0=beep off, 1=beep 500 Hz, 2=beep 1kHz, 3=beep 2kHz
 *		- Make sure that the LV2400x is in idle state before invoking this function.
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
void AudioFeedback(BYTE byType)
{
// Local equations
#define _AFB_DEMPLL	((BYTE)0x01)
#define _AFB_SDPLL	((BYTE)0x02)
#define _AFB_AUD_MUTE	((BYTE)0x04)
	BYTE byOrgFlag, byCurVolume, byOrgBeep,  byOrgBand;

	// Do nothing if beep is not desired
	if (g_byBeepVol==0)
		return;

	// Init local.
	byOrgFlag = 0;

	// Turn off radio band, enable beep source
	byOrgBand = SetRegBits(IR01_RADIO_CTRL3_REG, (IR1_RCTL3_SE_FM|IR1_RCTL3_SE_BEEP), IR1_RCTL3_SE_BEEP);

	// Turn on the stereo decoder PLL mute for stable beep frequency
	if ( DriveBit(IR01_STEREO_CTRL_REG, IR1_STCTL_SD_PM, TRUE) )
		byOrgFlag |= _AFB_SDPLL;

	// Unmute audio if necessary
	if ( GetHwFeatureValue(IHCAP_AMUTE) )
	{
		SetHwFeatureValue(IHCAP_AMUTE, 0);	// Un-mute audio
		byOrgFlag |= _AFB_AUD_MUTE;
	}

	// Set volume if necessary
	byCurVolume = GetHwFeatureValue(IHCAP_VOLUME); // Determine current volume level
	if (byCurVolume<g_byBeepVol)
		SetHwFeatureValue(IHCAP_VOLUME, g_byBeepVol);

	// turn on demodulator PLL mute to mute the radio
//	if (DriveBitNeg(IR01_RADIO_CTRL2_REG, IR1_RCTL2_IF_PM_L, TRUE) )
//		byOrgFlag |= _AFB_DEMPLL;

	if (byType == AUDFB_TYPE1)
	{
		// Beep at 500 Hz
		byOrgBeep = SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_500Hz);
		DelayUs(DLT_20ms);	
  		DelayUs(DLT_20ms);
	}
	else //if (byType == AUDFB_TYPE2)
	{
		// Beep at 2kHz
		byOrgBeep = SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_2KHz);
		DelayUs(DLT_20ms);
  		DelayUs(DLT_20ms);
		// Pause
		SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_OFF);
  		DelayUs(DLT_20ms);
  		DelayUs(DLT_20ms);
		// Beep at 2kHz
		SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_2KHz);
  		DelayUs(DLT_20ms);
  		DelayUs(DLT_20ms);
	}

	// Restore everything
	if (byCurVolume != g_byBeepVol)		// Restore volume
		SetHwFeatureValue(IHCAP_VOLUME, byCurVolume);

//	if ( (byOrgFlag &_AFB_DEMPLL)==0 )	// Restore demodulator PLL mute
//		DriveBitNeg(IR01_RADIO_CTRL2_REG, IR1_RCTL2_IF_PM_L, FALSE);

	if ( byOrgFlag & _AFB_AUD_MUTE )	// Restore audio mute
		SetHwFeatureValue(IHCAP_AMUTE, 1);

	if ( (byOrgFlag & _AFB_SDPLL)==0 )	// Restore the stereo decoder PLL mute
		DriveBit(IR01_STEREO_CTRL_REG, IR1_STCTL_SD_PM, FALSE);

	// Restore beep frequency bits
	SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, byOrgBeep);

	// Restore radio band, beep source setting
	SetRegBits(IR01_RADIO_CTRL3_REG, (IR1_RCTL3_SE_FM|IR1_RCTL3_SE_BEEP), byOrgBand);
} // End AudioFeedback


//-----------------------------------------------------------------------------
// Registers shadowing function
//-----------------------------------------------------------------------------
void ShadowReg(WORD wRegAddress, BYTE byValue)
{
	BYTE i;

	for (i=0; i<LVSHADOW_LSIZE; i++)
	{ 
		if ( g_Lv24ShadowList[i] == wRegAddress )
		{
			g_byaShwRegValue[i] = byValue;
			break;
		}
	}
	g_byBlock = MSB(wRegAddress);	//  Save last access block
} // End ShadowReg

BYTE GetSwRegValue(WORD wRegAddress)
{
	BYTE i;

	for (i=0; i<LVSHADOW_LSIZE; i++)
	{ 
		if ( g_Lv24ShadowList[i] == wRegAddress )
			return(g_byaShwRegValue[i]);
	}
	return(0); // not found
} // End GetSwRegValue

void SelRegBlock(BYTE byBlock)
{
	// Skip selecting the block when it's already selected
	if (g_byBlock != byBlock)
	{
		g_byBlock = byBlock; // Save block number for next time
 		IoWrite3W(byBlock, BLK_SEL);	// Write the block number to BLK_SEL register
	}
} // End SelRegBlock

/* ************************************************************************************************
 *
 *  Function:   IsFrequencyOk
 *
 *  Authors:    Hung van Le
 *  Purpose:    Verify if wCurFreq is in range of wExpFreq +/- margin
 *  Input:
 *			wCurFreq: current frequency
 *			wExpFreq: the expected frequency
 *			dwPrecise: precise of the compare (max=1000000, 1000000 is exact comparing)
 *  Output:
 *			0 : wCurFreq within the margin
 *			-1: wCurFreq too low (i.e. wCurFreq < (wExpFreq - margin)
 *			+1: wCurFreq too high (i.e. wCurFreq > (wExpFreq + margin)
 *  Comments:   margin = (1000000 * DividerFactor) / dwPrecise
 *		Divider factor is dependant on current output select of the chip
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
int IsFrequencyOk(WORD wCurFreq, WORD wExpFreq, DWORD dwPrecise)
{
	WORD wMargin;
	DWORD dwFactor;

	if (dwPrecise != 1000000)
	{
		switch ( GetOutputSelect() )
		{
			case CO_RF_OSC:
				// wMargin = (1000000 * (DWORD)GetDividerFactor())/dwPrecise;
				// wMargin /= 10000;	// 10 KHz unit for RF
				dwFactor = 100 * IMR01_FM_DIVIDER;
				break;
			case CO_IF_OSC:
				// wMargin = (1000000 * (DWORD)GetDividerFactor())/dwPrecise;
				// wMargin /= 10;	// 10 Hz unit for IF
				dwFactor = 100000;
				break;
			//case CO_SD_OSC:
			default:
				// wMargin = (1000000 * (DWORD)GetDividerFactor())/dwPrecise;
				// wMargin /= 1;	// 1 Hz unit for other freq
				// 1 Hz unit for SD and other freq.
				dwFactor = 1000000;
				break;
		}
	}
	else
		dwFactor = 0;
	if (dwPrecise < dwFactor)
		wMargin = (WORD)(dwFactor / dwPrecise);
	else
		wMargin = 0;

	if ( wCurFreq < (wExpFreq - wMargin) )
		return(-1);
	else if ( wCurFreq > (wExpFreq + wMargin) )
		return(1);
	else
		return(0);
} // End IsFrequencyOk










BYTE g_byBeepVol;

void BeepTest(BYTE );

void test(){
	

		BYTE Chip_ID;

		WriteReg(IR01_AUDIO_CTRL1_REG,0x77);// 0x0102 Block 1- Reg02 (W): Measure source select
		
		WriteReg(IR01_PW_SCTRL_REG,(IR1_PSCTL_PW_RAD|
				0x3<<5|
				0x3<<3|0x1<<1));
		
		Chip_ID = ReadReg(IR01_CHIP_ID_REG);
		Chip_ID = ReadReg(IR01_CHIP_ID_REG);
		//SCI_TRACE_LOW:"chip id1 = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_2012_112_2_18_0_32_53_1454,(uint8*)"d",Chip_ID);
	
		WriteReg(IR01_MSRC_SEL_REG,0);	// 0x0102 Block 1- Reg02 (W): Measure source select
		WriteReg(IR01_FM_OSC_REG,0);	// (*) 0x0103 Block 1- Reg03 (W): DAC control for FM-RF oscillator
		WriteReg(IR01_SD_OSC_REG,0);	// (*) 0x0104 Block 1- Reg04 (W): DAC control for stereo decoder oscillator
		WriteReg(IR01_IF_OSC_REG,0);	// (*) 0x0105 Block 1- Reg05 (W): DAC control for IF oscillator
		WriteReg(IR01_CNT_CTRL_REG,0);	// 0x0106 Block 1- Reg06 (W): Counter control
		WriteReg(IR01_IRQ_MSK_REG,0);	// 0x0108 Block 1- Reg08 (W): Interrupt mask
		WriteReg(IR01_FM_CAP_REG,0);	// 0x0109 Block 1- Reg09 (W): CAP bank control for RF-frequency

		WriteReg(IR01_RADIO_CTRL1_REG,0);	// 0x0202 Block 2- Reg02 (W): Radio control 1
		WriteReg(IR01_IFCEN_OSC_REG,0);	// (*) 0x0203 Block 2- Reg03 (W): IF Center Frequency Oscillator
		WriteReg(IR01_IF_BW_REG,	0);	// (*) 0x0205 Block 1- Reg05 (W): IF Bandwidth
		WriteReg(IR01_RADIO_CTRL2_REG,0);	// 0x0206 Block 2- Reg06 (W): Radio Control 2
		WriteReg(IR01_RADIO_CTRL3_REG,18);
		WriteReg(IR01_STEREO_CTRL_REG,2);	
		WriteReg(IR01_AUDIO_CTRL1_REG,246);	
		WriteReg(IR01_AUDIO_CTRL2_REG,0);
		Chip_ID = ReadReg(IR01_CHIP_ID_REG);
		Chip_ID = ReadReg(IR01_CHIP_ID_REG);

		BeepTest(AUDFB_TYPE2);
		
		//SCI_TRACE_LOW:"chip id = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LV2400X_2035_112_2_18_0_32_53_1455,(uint8*)"d",Chip_ID);
		


}

//beeptest
void BeepTest(BYTE byType)
{
// Local equations
#define _AFB_DEMPLL	((BYTE)0x01)
#define _AFB_SDPLL	((BYTE)0x02)
#define _AFB_AUD_MUTE_	((BYTE)0x04)
	BYTE byOrgFlag, byCurVolume, byOrgBeep,  byOrgBand;
	
	g_byBeepVol = 10;
	// Do nothing if beep is not desired
	if (g_byBeepVol==0)
		return;

	// Init local.
	byOrgFlag = 0;

	// Turn off radio band, enable beep source
	byOrgBand = SetRegBits(IR01_RADIO_CTRL3_REG, (IR1_RCTL3_SE_FM|IR1_RCTL3_SE_BEEP), IR1_RCTL3_SE_BEEP);

	// Turn on the stereo decoder PLL mute for stable beep frequency
	if ( DriveBit(IR01_STEREO_CTRL_REG, IR1_STCTL_SD_PM, TRUE) )
		byOrgFlag |= _AFB_SDPLL;

	// Unmute audio if necessary
	if ( GetHwFeatureValue(IHCAP_AMUTE) )
	{
		SetHwFeatureValue(IHCAP_AMUTE, 0);	// Un-mute audio
		byOrgFlag |= _AFB_AUD_MUTE_;
	}

	// Set volume if necessary
	byCurVolume = GetHwFeatureValue(IHCAP_VOLUME); // Determine current volume level
	if (byCurVolume<g_byBeepVol)
		SetHwFeatureValue(IHCAP_VOLUME, g_byBeepVol);

	// turn on demodulator PLL mute to mute the radio
//	if (DriveBitNeg(IR01_RADIO_CTRL2_REG, IR1_RCTL2_IF_PM_L, TRUE) )
//		byOrgFlag |= _AFB_DEMPLL;

	if (byType == AUDFB_TYPE1)
	{
		// Beep at 500 Hz
		byOrgBeep = SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_500Hz);
		
	}
	else //if (byType == AUDFB_TYPE2)
	{  		
		// Beep at 2kHz
		SetRegBits(IR01_AUDIO_CTRL2_REG, IR1_ACTL2_BPFREQ, IR1_BEEP_2KHz);
  		
	}

} // End AudioFeedbac













// ==============================================================================
#endif //USE_LVC2400x 	// The whole file can be discarded if LV2400x is not used
// ==============================================================================
