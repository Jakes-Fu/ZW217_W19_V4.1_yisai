/************************************************************************
*
*   Copyright(c) 2004 ItoM BV
*   All Rights Reserved.
*
*   LV2400x evaluation kit: Radio implementations
*   File name:	RadioSub.c
*	Several subroutines to be used by Radio.c
*
*************************************************************************/

#include "ms_customize_trc.h"
#include <stdio.h>
#include "fm_common.h"
#include "Lv24Ekit.h"
#include "os_api.h"
//#define SCI_TRACE_LOW SCI_TraceLow
//extern void SCI_TraceLow(const char*, ...);

/*-------------------------------------------------------------------
        Local data
-------------------------------------------------------------------*/
// Structure for band limit
typedef struct
{
	WORD wBandLo;	// Band low limit (in 10 kHz)
	WORD wBandHi;	// Band high limit (in 10 kHz)
} BAND_LIMIT_ST;

// ----- Japan Wide limit list
BAND_LIMIT_ST _rom g_JapanWideFmBandList[] = 
{
	{ (7600-15),	(9000+15), },	// Base Japan FM limit 76-90 MHz
	{ ( 9575-15),	( 9575+15), },	// Channel 1: 95.75 MHz
	{ (10175-15),	(10175+15), },	// Channel 2: 101.75 MHz
	{ (10775-15),	(10775+15), },	// Channel 3: 107.75 MHz
};
#define JAPANWIDE_FMBAND_LSIZE (sizeof(g_JapanWideFmBandList)/sizeof(g_JapanWideFmBandList[0]))

/*-------------------------------------------------------------------
        Code
-------------------------------------------------------------------*/
/* ************************************************************************************************
 *
 *  Function:   MeasureFrequency
 *
 *  Authors:    Hung van Le
 *  Purpose:    Virtualize the I3W frequency measuring
 *  Input:      
 *		WORD wSource: source to be measured. See definitions in I3wChip.h (MeasureFrequency, SetUpChipMode)
 *		DWORD dwMeasureTimeUs: measure time in us
 *		PDWORD pdwFreqHz: Buffer to receive the measured frequency (in Hz)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   None
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE MeasureFrequency(WORD wSource, DWORD dwMeasureTimeUs, PWORD pwFreq)
{
	// Measure frequency of specified source
	BYTE byResult;
	WORD wCfg;

	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(wSource | CHIP_MEASURE_MODE);

	// Measuring
	byResult = CountPulse(dwMeasureTimeUs, pwFreq);

	// Restore chip config
	SetUpChipMode(wCfg);

	 return(byResult);
} // End MeasureFrequency

/* ************************************************************************************************
 *
 *  Function:   DisplayFreqToRf
 *
 *  Authors:    Hung van Le
 *  Purpose:    Convert the display frequency to RF-frequency
 *  Input:      
 *		WORD wDisp: the display frequency i 10 kHz unit (example 87.5 MHz is 8750)
 *  Output:     Status as defined in LvErr.h
 *  Global:	
 *		g_byHwFlag1: HF1_NEG_IF_PHASE-flag for IF phase
 *  Comments:   The display frequency has 2 components: The RF and IF frequency
 *		The radio hardware has 2 separated circuitries to deal with these components.
 *		The IF frequency is set once during radio hardware initialization.
 *		Changing RF frequency will tune the radio hardware to another station
 *		When the IF has negative phase:
 *			RF = DisplayFrequency + CalibratedIF
 *		When the IF has positive phase:
 *			RF = DisplayFrequency - CalibratedIF
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD DisplayFreqToRf(WORD wDisp) 
{

	// Negative IF phase
	if ( g_byHwFlag1 & HF1_NEG_IF_PHASE) 
		return(wDisp + DEFAULT_IF_FREQ/1000);	// RF (in 10 kHz) = display + IF (remark: IF in 10 Hz)

	// Positive IF
	return(wDisp - DEFAULT_IF_FREQ/1000);		// RF (in 10 kHz) = display - IF

} // End DisplayFreqToRf

/* ************************************************************************************************
 *
 *  Function:   RfToDisplayFreq
 *
 *  Authors:    Hung van Le
 *  Purpose:    Convert the RF-frequency to display frequency
 *  Input:      
 *		WORD wRf: the RF-frequency i 10 kHz unit (example 87.5 MHz is 8750)
 *  Output:     Status as defined in LvErr.h
 *  Global:	
 *		g_byHwFlag1: HF1_NEG_IF_PHASE-flag for IF phase
 *  Comments:   See also DisplayFreqToRf
 *		We use following equation to calculate the display frequency:
 *		When the IF has negative phase:
 *			DisplayFrequency = RF - CalibratedIF
 *		When the IF has positive phase:
 *			DisplayFrequency = RF + CalibratedIF
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD RfToDisplayFreq(WORD wRf)
{
	// Negative IF phase
	if ( g_byHwFlag1 & HF1_NEG_IF_PHASE)
		return(wRf - DEFAULT_IF_FREQ/1000);	// display = RF - IF

	// Positive IF
	return(wRf + DEFAULT_IF_FREQ/1000);		// display = RF + IF
} // End DisplayFreqToRf

/* ************************************************************************************************
 *
 *  Function:   CheckRfLimit
 *
 *  Authors:    Hung van Le
 *  Purpose:    Check input frequency against the specified RF-limit in dwBandLow/dwBandHigh, 
 *		Wrap the input frequency if bWrapFreq is true
 *		The frequency will be checked against the hardware limit (stored in m_dwHwRfLow/m_dwHwRfHigh)
 *  Input:      
 *		PWORD pwCurFreq: pointer to frequency to be checked (in 10 kHz unit)
 *		WORD wBandLow, WORD wBandHigh: the band limit in 10 kHz
 *		iAdjust: adjust pwCurFreq when error 
 *			<=0: adjust for down scanning
 *			1: adjust for up scanning
 *			2: no adjustment, wrap high to low, low to high (default wrapping is low to low, high to high)
 *
 *  Output:     Status as defined in LvErr.h
 *		LVLS_FREQ_JUMP_ERR is returned when adjustment is done.

 *  Global:	g_wHwRfLow: the lower limit of hardware RF frequency
 *		g_wHwRfHigh: the upper limit of hardware RF frequency
 *  Comments:	Scheme
 *			*pwCurFreq		bWrapFreq	New freq		Status
 *		a)	<wBandLow		true		wBandHigh		No error
 *		b)				false		wBandLow		Out of band error
 *		c)	>wBandHigh		true		wBandLow		No error
 *		d)				false		wBandHigh		Out of band error
 *		e)	Other cases		X 		No change		No error
 *		Extra check by (a), (c), (e)
 *			*pwCurFreq	New freq   	Status
 *			<g_wHwRfLow	g_wHwRfLow	Error: Unreachable frequency
 *			>g_wHwRfHigh	g_wHwRfHigh	Error: Unreachable frequency
 *			Other cases	No change	No error (within hardware limit)
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE CheckRfLimit(PWORD pwCurFreq, WORD wRfBandLow, WORD wRfBandHigh, int iAdjust)
{
	BYTE byResult;
	BYTE i;

	byResult = LVLS_NO_ERROR;
	i=0;	// Use i to enable JapanWide alignment
	if (*pwCurFreq < wRfBandLow) // exceed lower limit
	{
		if (iAdjust == 2)
		{
			*pwCurFreq = wRfBandHigh;	// No error by wrapping
		}
		else
		{
			*pwCurFreq = wRfBandLow;
			byResult = LVLS_RFFREQ_ENDOFBAND_ERR;
		}
		i=1;	// skip JapanWide alignment
	}
	else if (*pwCurFreq > wRfBandHigh) // exceed upper limit
	{
		if (iAdjust == 2)
		{
			*pwCurFreq = wRfBandLow;	// No error by wrapping
		}
		else
		{
			*pwCurFreq = wRfBandHigh;
			byResult = LVLS_RFFREQ_ENDOFBAND_ERR;
		}
		i=1;	// skip JapanWide alignment
	}

	// Special handling for Japan-wide support: adjust the RF according to iAdjust and current RF frequency
	if (i == 0)	// Go on if JapanWide alignment is allowed
	{

		if (g_byRegion == REGION_JAPAN_WIDE)
			i=1;

		if (i != 0)	// Japan Wide enabled
		{
			// Determine which limit band to use
			for (i=0; i<(JAPANWIDE_FMBAND_LSIZE-1); i++)
			{
				if ( ((*pwCurFreq) > DisplayFreqToRf(g_JapanWideFmBandList[i].wBandLo)) &&
				     ((*pwCurFreq) < DisplayFreqToRf(g_JapanWideFmBandList[i+1].wBandLo)) )
					break;	// found band
			}

			wRfBandLow = DisplayFreqToRf(g_JapanWideFmBandList[i].wBandLo);
			wRfBandHigh = DisplayFreqToRf(g_JapanWideFmBandList[i].wBandHi);
			if (iAdjust > 1)	// Check frequency
			{
				if ( (*pwCurFreq) > wRfBandHigh )
				{
					byResult = LVLS_FREQ_IN_LIMIT_GAP_ERR;
					(*pwCurFreq) = wRfBandHigh;
				}
				else if ( (*pwCurFreq) < wRfBandLow )
				{
					byResult = LVLS_FREQ_IN_LIMIT_GAP_ERR;
					(*pwCurFreq) = wRfBandLow;
				}
			} // EndIf CheckFreq
			else // Align the frequency
			{
				if ( (*pwCurFreq) > wRfBandHigh )
				{
					// Go to next band. Above code already make sure the we are always in the band, so no need to check
					// for max. band
					if (iAdjust == 1)	// Align frequency for up scanning
					{
						if (i==(JAPANWIDE_FMBAND_LSIZE-1))
							(*pwCurFreq) = DisplayFreqToRf(g_JapanWideFmBandList[i].wBandHi);
						else
							(*pwCurFreq) = DisplayFreqToRf(g_JapanWideFmBandList[i+1].wBandLo);
					}
					else		// Align frequency for down scanning
						(*pwCurFreq) = wRfBandHigh; // ie: DisplayFreqToRf(g_JapanWideFmBandList[i].wBandHi);
					byResult = LVLS_FREQ_JUMP_ERR;
				}
				else if ( (*pwCurFreq) < wRfBandLow )
				{
					if (iAdjust == 1)	// Align frequency for up scanning
					    /* lint -save -e661 */
						(*pwCurFreq) = DisplayFreqToRf(g_JapanWideFmBandList[i+1].wBandLo);
						/* lint -restore */
					else
						(*pwCurFreq) = wRfBandHigh; // ie: DisplayFreqToRf(g_JapanWideFmBandList[i].wBandHi);
					byResult = LVLS_FREQ_JUMP_ERR;
				}
			}
		}
	}

	// Check against hardware limit
	if ( (*pwCurFreq<g_wHwRfLow) || (*pwCurFreq>g_wHwRfHigh) )
	{
		byResult = LVLS_UNREACHABLE_FREQ_ERR;
		(*pwCurFreq<g_wHwRfLow)?(*pwCurFreq=g_wHwRfLow):(*pwCurFreq=g_wHwRfHigh);
	}
	return(byResult);
} // End CheckRfLimit

/* ************************************************************************************************
 *
 *  Function:   ScanFmRoutine
 *
 *  Authors:    Hung van Le
 *  Purpose:    Scan FM band for station
 *  Input:      
 *			int iDir: Scan direction (iDir = 1: scan up, -1: scan down)
 *			WORD wFmBandLow, WORD wFmBandHigh: RF-frequency range for scanning (in 10 kHz-unit)
 *  Output:     Status as defined in LvErr.h
 *  Global:	g_byScanLevel: the field strength level to accept/reject a station (must be set before invoking this routine)
 *		g_byStnFlag: STN_VALID flag to determine the initial start frequency: when this flag is set (we're at a valid station)
 *			     extra offset will be added to avoid finding the same station.
 *		g_byHwFlag1: HF1_NEG_IF_PHASE flag for the IF phase (the scan search for IF edge, the phase indicates which edge (Positive/Negative) is valid)
 *		g_wLastSetRF: the RF frequency (change during scanning)
 *		g_wLastMsrRF: the measured RF frequency (change during scanning)
 *  Comments:	The chip should be correctly set up before invoking this routine (Measure mode, mute , stereo ...)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE ScanFmRoutine(int iDir, WORD wFmBandLow, WORD wFmBandHigh)
{
#define SCFM_FROMSTATION_STEP	((WORD)20)	// 200 kHz - Step size to step away from a station
#define SCFM_NOSTATION_STEP	((WORD)10)	// 100 kHz - Step size when a station is not found
#define SCFM_DISPLAY_DISTANCE	((WORD)85)	// 850 kHz - Update display distance

#define SCFM_SCANSTEP		((int)8) 	// 80 kHz - scan step (10 kHz-unit)
#define SCFM_IFSWING		((int)3500)	// 35 kHz - IF swing (10 Hz-unit)
#define SCFM_PRECISION		QSSF_PRECISION_LOW // precision level use by ScanSetFreq

	BYTE  byResult;
	WORD  wIfFreq, wIfFreq2;
	BOOL  bCorrectEdge;
	int   iStepFreq, iIfDif;
	int   iIfSwing;
	BYTE  byMsrFs, bySwFsLevel, byDispCnt;

	// Init var.
	byResult = LVLS_NO_ERROR;
	wIfFreq =0;
	if (g_byStnFlag & STN_VALID)			// Valid station
		iStepFreq = SCFM_FROMSTATION_STEP;	// Step away from current stattion
	else
		iStepFreq = 0;
	g_byStnFlag &= (~STN_VALID);			// No valid station now
	
	// Scan routine uses bySwFsLevel, i32ScanStepHz, i32IfSwingHz, which are derived from g_byScanLevel
	// Adjust the scan level because the radio chip can not measure it quickly
	bySwFsLevel = g_byScanLevel;
	if (g_byScanLevel > 0)
		bySwFsLevel--;	// Leave 0 at 0, convert 1..7 to 0..6
	if (bySwFsLevel>3)
		bySwFsLevel--;	// Convert scan level 7/6/5 to 5/4/3 (hardware measuring!!!)
		
	// Determine sign of the IF swing (depend on the scan direction and the IF phase)
	iIfSwing = SCFM_IFSWING * iDir;
	if ( g_byHwFlag1 & HF1_NEG_IF_PHASE) // Negative IF phase
		iIfSwing = -iIfSwing;

	// Scan loop
	byDispCnt = 0;
	while ( (g_byStnFlag & STN_VALID)==0 )	// While not valid station
	{
		// Calculate the new frequency
		g_wLastSetRF += (iStepFreq*iDir);

		// Check frequency
		byResult = CheckRfLimit(&g_wLastSetRF, wFmBandLow, wFmBandHigh, iDir);
		if (byResult != LVLS_NO_ERROR)
		{
			wIfFreq = 0;
			if (byResult != LVLS_FREQ_JUMP_ERR)
			{
				// Set frequency to valid one
				ScanSetFreq(g_wLastSetRF, QSSF_PRECISION_HIGH);
				//SCI_TRACE_LOW:"scan set freq error = %d\r\n"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,RADIOSUB_352_112_2_18_0_32_55_1456,(uint8*)"d",g_wLastMsrRF);
				goto return_ScanFmRoutine;
			}
		}

		// Set the frequency
		byResult = ScanSetFreq(g_wLastSetRF, SCFM_PRECISION);
		if (byResult != LVLS_NO_ERROR)
			goto return_ScanFmRoutine;
        //SCI_TRACE_LOW:"scan set freq ok = %d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RADIOSUB_361_112_2_18_0_32_55_1457,(uint8*)"d",g_wLastMsrRF);

		// Callback - report current frequency
		byDispCnt++;
		//if (byDispCnt>9)
		if (byDispCnt>1)
		{
			byResult = CallBack(CRSN_SCAN_UPD);
			if (byResult != LVLS_NO_ERROR)
				goto return_ScanFmRoutine;
			byDispCnt = 0;
		}
		
		// Check 1: field strength should be sufficient
		if ( bySwFsLevel>0 )
		{
			//DelayUs(DLT_5ms);	// The hardware fieldstrength measuring circuitry is not very fast, give it sometimes
			byMsrFs = GetFmFieldStrength();
			if ( byMsrFs < bySwFsLevel )
			{
				DelayUs(DLT_5ms);	// The hardware fieldstrength measuring circuitry is not very fast, give it sometimes
				byMsrFs = GetFmFieldStrength();
			}
			if ( byMsrFs < bySwFsLevel )
			{
				iStepFreq = SCFM_SCANSTEP;
				wIfFreq = 0;
				continue;
			}
		}
				
		// Measure the IF frequency at current point
		byResult = MeasureFrequency(CO_IF_OSC, LV_MSR_TIME_8ms, &wIfFreq2);
		if (byResult != LVLS_NO_ERROR)
			goto return_ScanFmRoutine;
		
		// Checking the IF edge when we have the 1st IF
		bCorrectEdge = FALSE;
		if (wIfFreq != 0)
		{
			iIfDif = (int)wIfFreq-(int)wIfFreq2;
			if (iIfSwing > 0)
			{
				if ( iIfDif >= iIfSwing ) // found edge
					bCorrectEdge = TRUE;
			}
			else
			{
				if ( iIfDif <= iIfSwing ) // found edge
					bCorrectEdge = TRUE;
			}
		}

		// Move current point to 1st point for next time
		wIfFreq = wIfFreq2;

		// Check if we have a correct IF-edge
		if ( !bCorrectEdge ) 
		{
			iStepFreq = SCFM_SCANSTEP;
			continue;
		}
		
		// Final step: pinpoint the station if field strength is sufficient
		if ( bySwFsLevel>0 )
		{
			byMsrFs = GetFmFieldStrength();
			if ( byMsrFs < bySwFsLevel )
			{
				iStepFreq = SCFM_SCANSTEP;
				continue;
			}
		}

		// Last update before FindFmStation
		byResult = CallBack(CRSN_SCAN_UPD);
		if (byResult != LVLS_NO_ERROR)
			goto return_ScanFmRoutine;

		// Pinpoint the station
		if ( FindFmStation() )
			g_byStnFlag |= STN_VALID;
		else
		{
			// Reset scan variable for next scan when not found
			iStepFreq = SCFM_NOSTATION_STEP;
			wIfFreq =0;
			continue;
		}

		// Verfy that we have the station with correct field strength
		if ( g_byScanLevel > 0 )
		{
			byMsrFs = GetFmFieldStrength();
			if ( byMsrFs < g_byScanLevel )
			{
				g_byStnFlag &= (~STN_VALID); // Wrong field strength
				iStepFreq = SCFM_SCANSTEP;
			}
			//SCI_TRACE_LOW:"field strength = %d\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,RADIOSUB_460_112_2_18_0_32_55_1458,(uint8*)"d",byMsrFs);
			//SCI_TRACE_LOW:"g_wLastMsrRF = %d\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,RADIOSUB_461_112_2_18_0_32_55_1459,(uint8*)"d",g_wLastMsrRF);
		}
	} // End ScanLoop

return_ScanFmRoutine:
	if (byResult == LVLS_NO_ERROR)
	{
		// We found a station, extra verification when the found station is at 50 kHz grid
		MeasureFrequency(CO_RF_OSC, LV_MSR_TIME_32ms, &wIfFreq);	// Update g_wLastMsrRF
		wIfFreq = GetDisplayFrequency(5); // Temp. use wIfFreq to store current display frequency, aligned at 50 kHz
		if ( (wIfFreq % 10) == 5 )	// Display frequency end at 50 kHz?
		{
			// Determine the station frequency again
			MeasureFrequency(CO_IF_OSC, LV_MSR_TIME_32ms, &wIfFreq);	// IF

			// Compensate RF to get with the last measured IF
			// Calculate the delta IF 
			iIfDif = (int)wIfFreq - (int)DEFAULT_IF_FREQ;
			if ( g_byHwFlag1 & HF1_NEG_IF_PHASE) // Apply IF-sign
				iIfDif = -iIfDif;	     // Negative IF phase

			// Compensate RF to get the desired IF (IF in 10 Hz, RF in 10 kHz unit)
			// g_wLastMsrRF is updated above
			g_wLastMsrRF = (int)g_wLastMsrRF + (iIfDif/1000);
		}
	}
	return(byResult);
} // End ScanFmRoutine

/* ************************************************************************************************
 *
 *  Function:   FindFmStation
 *
 *  Authors:    Hung van Le
 *  Purpose:    Try to find a FM station
 *  Input:      None
 *  Output:     TRUE : station found
 *		FALSE: no station found
 *  Global:	g_byHwFlag1: HF1_NEG_IF_PHASE flag for the IF phase
 *		g_wLastMsrRF: the measured RF frequency (compensated for display frequency calculation)
 *  Comments:	- A FM station is characterized by the IF-edge of +/- 45 degree (positive/negative IF). 
 *		  When we have the correct edge, the station can be pinpointed by using equation RF1 +/- IF1 = RF2 +/- IF2
 *		  (+ or - depends on the IF-phase)
 *
 *		- When a station is found, the RF will be compensated with dIF:
 *		  To avoid wrong display frequency, we have to correct the RF-frequency according to the measured IF-frequency.
 *		  When the IF-phase is negative, we have relation:
 *			DisplayFreq = RF - IF	(for positive phase the equation is DisplayFreq = RF + IF)
 *		  When we find a station, the display frequency should be
 *			DisplayFreq = RF' - MeasuredIF
 *			where MeasuredIF = CalibratedIF + dIF. 
 *			or dIF = MeasuredIF - CalibratedIF (dIF can be negative and is 0 when the RF is correctly tuned)
 *			so RF' = RF + dIF
 *		  For example when the CalibratedIF is 110 kHz, and we find a station with 100 kHz as last measured IF,
 *		  dIF = 100 - 110 = -10 kHz
 *		  When the IF phase is negative, The RF' is then  RF' = RF + 10 kHz
 *
 *		- This routine can also be used to validate the FM station
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BOOL FindFmStation(void)
{
//	#define FFS_IF_MARGIN	1500	// in 10 Hz unit
	#define FFS_IF_MARGIN	3500	// in 10 Hz unit

	BYTE  byLoopCnt;
	WORD  wCurFM;
	WORD  wCurIF;
	int   iDifIF;

	// Determine current IF
	MeasureFrequency(CO_IF_OSC, LV_MSR_TIME_32ms, &wCurIF);

	if ( (wCurIF < (DEFAULT_IF_FREQ-FFS_IF_MARGIN)) || (wCurIF >(DEFAULT_IF_FREQ+FFS_IF_MARGIN)) )
	{
		// Determine the current FM frequency (always measure RF for correcting it here after)
		MeasureFrequency(CO_RF_OSC, LV_MSR_TIME_32ms, &wCurFM);

		// Enter find station point loop if IF-frequency is not in OK range
		byLoopCnt = 4;
		do
		{
			byLoopCnt--;
			if (byLoopCnt == 0)
				return(FALSE);

			// Calculate the delta IF 
			iDifIF = (int)wCurIF - (int)DEFAULT_IF_FREQ;
			if ( g_byHwFlag1 & HF1_NEG_IF_PHASE ) // Apply IF-sign
				iDifIF = -iDifIF;	     // Negative IF phase
			wCurFM = (int)wCurFM + (iDifIF/1000);	// Compensate RF to get the desired IF (IF in 10 Hz, RF in 10 kHz unit)
			if ( ScanSetFreq(wCurFM, QSSF_PRECISION_HIGH) != LVLS_NO_ERROR )
				return(FALSE);

			MeasureFrequency(CO_IF_OSC, LV_MSR_TIME_32ms, &wCurIF);
		} while ( (wCurIF<(DEFAULT_IF_FREQ-FFS_IF_MARGIN)) || (wCurIF>(DEFAULT_IF_FREQ+FFS_IF_MARGIN)) );
	}
	return(TRUE);
} // End FindFmStation
