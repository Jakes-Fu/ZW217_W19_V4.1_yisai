/************************************************************************
*
*   Copyright(c) 2004 ItoM BV
*   All Rights Reserved.
*
*   LV2400x evaluation kit: Radio implementations
*   File name:	Radio.c
*	Radio implementation
*************************************************************************/

#include <stdio.h>
#include "fm_common.h"
#include "Lv24Ekit.h"
#include "os_api.h"
#include "fm_drv.h"



/* ************************************************************************************************
 *
 *  Function:   AdjustVolume
 *
 *  Authors:    Hung van Le
 *  Purpose:    Adjust the sotware volume level (ie mute is a level extra)
 *  Input:      byVolume: new volume level
 *			When byVolume = VOLUME_UP: increase volume level (defined in Lv24Ekit.h)
 *			When byVolume = VOLUME_DOWN: decrease volume level (defined in Lv24Ekit.h)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   
 *		Software add a level extra to volume level to implement volume level 0 = silent (ie mute audio)
 *		So there are n+1 volume levels (where n=maximal hardware level)
 *			0: Audio muted
 *			1: Audio unmute, hardware volume at 0
 *			2: Audio unmute, hardware volume at 1
 *			...
 *			n+1: Audio unmute, hardware volume at maximum (=n)
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE AdjustVolume(BYTE byVolume)
{
	BYTE byMaxVol, byCurVolume;

	// Determine current volume level and volume limit
	byCurVolume = GetHwFeatureValue(IHCAP_VOLUME);
	byMaxVol = GetHwFeatureLimit(IHCAP_VOLUME);

	// Do nothing if no change in volume level
	if (byCurVolume == byVolume) 
		return(LVLS_NO_ERROR);

	// Adjust volume
	if (byVolume==VOLUME_UP)
		byCurVolume++;
	else if (byVolume==VOLUME_DOWN)
		byCurVolume--;
	else
		byCurVolume = byVolume;

	// Check new volume level
	if (byCurVolume == 0xFF)	// Wrap from 0 to FF
		byCurVolume = 0;
	else if (byCurVolume>byMaxVol)
		byCurVolume = byMaxVol;

	// Update hardware setting
	if (byCurVolume == 0)
	{
		SetChipAudioMute(TRUE, MUTESRC_VOLUME);
	}
	else
	{
		SetChipAudioMute(FALSE, MUTESRC_VOLUME);
		byCurVolume--; // Compensate mute level
	}
	SetChipVolume(byCurVolume);

	return(LVLS_NO_ERROR);
} // End AdjustVolume

/* ************************************************************************************************
 *
 *  Function:   SetFrequency
 *
 *  Authors:    Hung van Le
 *  Purpose:    Set the frequency using display frequency
 *  Input:		
 *		WORD wDisplayFreq: the frequency to be set in 10 kHz-unit (ie 87.5 MHz is 8750)
 *  Output:     Status as defined in LvErr.h
 *  Global:	g_wLastMsrRF will be updated
 *  Comments:   None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE SetFrequency(WORD wDisplayFreq)
{
	BYTE byResult;

	byResult = SetRfFrequency(DisplayFreqToRf(wDisplayFreq));


	// Report the final status via CallBack before exit
	CallBack(CRSN_STN_CHNG);

	return(byResult);
} // End SetFrequency

/* ************************************************************************************************
 *
 *  Function:   GetDisplayFrequency
 *
 *  Authors:    Hung van Le
 *  Purpose:    Derive the display frequency from the current RF frequency
 *  Input:      WORD wSpacing: spacing in 10 kHz unit - see also comment 
 *  Output:     The display frequency in 10 kHz unit
 *  Global:	g_wLastMsrRF: the current RF frequency of the radio hardware
 *		g_wDisplayBandHigh, g_wDisplayBandLow: the current display frequency limit (depends on region)
 *  Comments:   The display frequency = RF frequency + (set_IF_frequency) * IF_phase
 *		Where:
 *			RF frequency: current RF oscillator frequency
 *			Set_IF_frequency: The calibrated IF frequency
 *			IF_phase: -1 or +1 (-1 for LV2400x)
 *		The display frequency will be rounded at the display spacing
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
WORD GetDisplayFrequency(WORD wSpacing)
{
	WORD wDispFreq;
	WORD wTmp;


       // Determine the rough display frequency
	wDispFreq = RfToDisplayFreq(g_wLastMsrRF);

	// Align the Display-Frequency according to spacing
	// Verify with display band
	if (wDispFreq < g_wDisplayBandLow)
	{
		wDispFreq = g_wDisplayBandLow;
	}
	else 
	{
		if (wDispFreq > g_wDisplayBandHigh)
		{
			wDispFreq = g_wDisplayBandHigh;
		}
		else
		{
			wTmp = (wDispFreq % wSpacing);
			if ( wTmp != 0 )
			{
				wDispFreq = wDispFreq - wTmp;	// Assume down rounding
				if ( wTmp >= (wSpacing/2))	// Rest frequency exceeds half
					wDispFreq += wSpacing;	// up rounding
			}
		}
	}
	return(wDispFreq);	
} // End GetDisplayFrequency

/* ************************************************************************************************
 *
 *  Function:   SetRegion
 *
 *  Authors:    Hung van Le
 *  Purpose:    Set the (FM) radio region
 *  Input:      byRegion: the required region to be set. Supports are:
 *			REGION_NONE:	use hardware limit
 *			REGION_JAPAN:	Japan FM band
 *			REGION_US:	US FM band
 * 			REGION_MOBLIE:	MOBILE FM band
 *  Output:     Status as defined in LvErr.h
 *  Global:	g_wHwRfHigh, g_wHwRfLow (RO): for determine the band limits of REGION_NONE
 *		g_wDisplayBandHigh, g_wDisplayBandLow (W): modified according to band limit of byRegion
 *		g_byRegion (W): keep track of current region
 *  Comments:   The region are defined Lv24Ekit.h
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE SetRegion(BYTE byRegion)
{
	// FM regions
	g_wFreqGrid = 10;	// Default: 100 kHz FM frequency grid for Japan/USA (in 10 kHz unit)
	switch (byRegion)
	{
	case REGION_NONE:	// No region - use hardware limit
		g_wDisplayBandLow = RfToDisplayFreq(g_wHwRfLow);
		g_wDisplayBandHigh = RfToDisplayFreq(g_wHwRfHigh);
		break;

	case REGION_JAPAN:	// Japan FM band
	case REGION_JAPAN_WIDE:	// Japan FM band
		SetDeemphasis(DEEMP_50);
		g_wDisplayBandLow  = 7600;		// Japan lower limit: 76 MHz
		g_wDisplayBandHigh = 9000;		// Japan upper limit: 90 MHz
		if (byRegion == REGION_JAPAN_WIDE)
			g_wDisplayBandHigh = 10775;	// Japan wide upper limit: 107.75 MHz
		break;

	case REGION_US:		// US FM band
		SetDeemphasis(DEEMP_75);
		g_wDisplayBandLow  = 8750;		// Europe lower limit: 87.5 MHz
		g_wDisplayBandHigh = 10800;		// Europe upper limit: 108 MHz
		break;

	case REGION_MOBILE:	// Use FM band as default
		byRegion = REGION_MOBILE;		// Make sure byRegion is valid
		SetDeemphasis(DEEMP_50);
		g_wDisplayBandLow  = 7600;		// Europe lower limit: 87.5 MHz
		g_wDisplayBandHigh = 10800;		// Europe upper limit: 108 MHz
		g_wFreqGrid = 10;			// Europe FM grid is 50 kHz (in 10 kHz unit)
		break;
/*	
	default:
		byRegion = REGION_EUROPE;		// Make sure byRegion is valid
		SetDeemphasis(DEEMP_50);
		g_wDisplayBandLow  = 8750;		// Europe lower limit: 87.5 MHz
		g_wDisplayBandHigh = 10800;		// Europe upper limit: 108 MHz
		g_wFreqGrid = 5;			// Europe FM grid is 50 kHz (in 10 kHz unit)
		break;
*/		
	}
	// Update current region
	 g_byRegion = byRegion;
	return(LVLS_NO_ERROR);
} // End SetRegion

/* ************************************************************************************************
 *
 *  Function:   ScanStation
 *
 *  Authors:    Hung van Le
 *  Purpose:    Search for FM station
 *  Input:      byFlag: flag to control the scan:
 *			SCANSTN_DIR: scan direction
 *				      1 => scan up (increase frequency).
 *				      0 => scan down (decrease frequency)
 *			SCANSTN_AUTO: 1=> Auto scan: scan whole band in specified direction with fieldstrength in g_byScanLevel
 *						     Return found station via callback with reason CBRN_ASCAN_FOUND
 *			              0=> Search for next/previous station with fieldstrength in g_byScanLevel
 *  Output:     Status as defined in LvErr.h
 *  Global:	g_byStnFlag:
 *			STN_VALID (W): clear when the scan starts, after the scan it reflects the valid station
 *		g_wDisplayBandLow (RO): start frequency for the scan when wBeginFreq is 0
 *		g_wDisplayBandHigh (RO): end frequency for the scan when wEndFreq is 0
 *		g_byScanLevel: field strength level to accept/reject a station (must be set before enter this function)
 *			
 *  Comments:   The scan will be aborted when the callback function reports any error. So host can abort the scan by returning 
 *		error with the call back (out of storage for auto scan, user abort...)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
extern BYTE  g_fm_freq_count;
extern WORD g_fm_autoscan_freq_tab[];

BYTE ScanStation(BYTE byFlag)
{
	BYTE byResult;
	BOOL bOrgAudioMute, bOrgAudioMode; //, bOrgAgcSpeed;
	BOOL bDone;
	BYTE byStationFound;
	int  iDir;
	WORD wCfg;
	WORD wBeginFreq, wEndFreq, wOrgFreq;
	WORD wRfLimitLo, wRfLimitHi;

	//Add by Eric.weii
	g_autoScanResult.validCnt = 0;//reset station result;
	// Scan direction
	if (byFlag & SCANSTN_DIR)
		iDir = 1;		// Scan up
	else
		iDir = -1;		// Scan down

	// Convert band limit to RF limt
	wRfLimitLo = DisplayFreqToRf(g_wDisplayBandLow) - g_wFreqGrid;	// Convert display to RF frequency (1 grid extra)
	wRfLimitHi = DisplayFreqToRf(g_wDisplayBandHigh) + g_wFreqGrid; // Convert display to RF frequency (1 grid extra)

	// Initialize the scan limits according to scan mode
	wOrgFreq = 0;	// To avoid compiler warning W303 (uninitialized var.)
	if (byFlag & SCANSTN_AUTO)	// Auto search 
	{
		g_fm_freq_count = 0;
		wBeginFreq = wRfLimitLo;
		wEndFreq = wRfLimitHi;
		// Determine start frequency
		if (iDir>0)
			g_wLastSetRF = wBeginFreq; 	// Scan up - start from lower limit
		else
			g_wLastSetRF = wEndFreq;	// Scan down - start from upper limit
		g_byStnFlag &= (~STN_VALID);	 	// No valid station
	}
	else // Search next mode
	{
		// Make sure we have a valid current frequency
		CheckRfLimit(&g_wLastSetRF, wRfLimitLo, wRfLimitHi, iDir);
		wOrgFreq = g_wLastSetRF; // This is our start frequency
		
		// Determine scan boundary
		if (iDir>0)	// Scan up
		{
			// Scan up from current frequency to upper limit
			wBeginFreq = g_wLastSetRF; // start at current frequency
			wEndFreq = wRfLimitHi;

			// Adjust the start frequency in special case
			if (wOrgFreq == wRfLimitHi) 	// Scanning up starting at high limit
				wOrgFreq++;
		}
		else		// Scan down
		{
			// Scan down from current frequency to lower limit
			wBeginFreq = wRfLimitLo;
			wEndFreq = g_wLastSetRF;

			// Adjust the start frequency in special case
			if (wOrgFreq == wRfLimitLo) 	// Scanning up starting at high limit
				wOrgFreq--;
		}
	}
		
	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(CO_RF_OSC|CHIP_MEASURE_MODE|CHIP_AFC_OFF);

	// Prepare the chip for scanning
	bOrgAudioMute = SetChipAudioMute(TRUE, MUTESRC_OPERATION);		// Mute audio
	// @@@ Speed up AGC for faster field strength measurement: already enabled by default
	// bOrgAgcSpeed = DriveBit(IR01_RADIO_CTRL2_REG, IR1_RCTL2_AGC_SPD, TRUE);
		
	// Force mono mode before scanning
	bOrgAudioMode = SetChipStereo(FALSE);

	if(FM_SEEK_TYPE_AUTO_ONE == FM_GetSeekType())
	{
		GPIO_OpenFMPA( SCI_FALSE );
	}
	
	// Perform scan
	byStationFound = 0;
	bDone = FALSE;
	while (!bDone)
	{

		byResult = ScanFmRoutine(iDir, wBeginFreq, wEndFreq);

		if (byResult==LVLS_NO_ERROR)
		{
			// Save the found station if auto scan mode
			if (byFlag & SCANSTN_AUTO)	// Auto search 
			{
				byStationFound++;
				byResult = CallBack(CRSN_ASCAN_FOUND);
				g_fm_autoscan_freq_tab[g_fm_freq_count++] = GetDisplayFrequency(10)/10;
				if (byResult != LVLS_NO_ERROR)
					bDone = TRUE;
			}
			else	// Normal scan mode
			{
				bDone = TRUE;	// Just scan once
					
				//if (m_bValidStation)
				//	FineTuneFmToIF(&m_bValidStation);
			}
		}
		else if (byResult == LVLS_RFFREQ_ENDOFBAND_ERR)	// End of band reached
		{
			if (byFlag & SCANSTN_AUTO)	// Auto scan mode 
			{
				bDone = TRUE;		// Done when end of band reached
			}
			else // Search mode
			{
				switch (g_byStnFlag & STN_WRAP_MODE)
				{
				case STNWRAP_NONE:	// No wrapping - terminate with end of band error
					bDone = TRUE;
					break;
					
				case STNWRAP_CONT:	// Continue wrapping - repeat the search at boundary
					wBeginFreq = wRfLimitLo;
					wEndFreq = wRfLimitHi;
					if (iDir>0)	// Scan up
						g_wLastSetRF = wBeginFreq;
					else		// Scan down
						g_wLastSetRF = wEndFreq;
					break;

				case STNWRAP_ONCE:	// Repeat the search until the start frequency reached again
					if (iDir>0)	// Scan up
					{
						if (g_wLastSetRF <= wOrgFreq)
						{
							byResult = LVLS_NO_STATION_FOUND_ERR;
							bDone = TRUE;
						}
						else
						{
							wBeginFreq = wRfLimitLo;
							wEndFreq = wOrgFreq;
							g_wLastSetRF = wBeginFreq;
						}
					}
					else		// Scan down
					{
						if (g_wLastSetRF >= wOrgFreq)
						{
							byResult = LVLS_NO_STATION_FOUND_ERR;
							bDone = TRUE;
						}
						else
						{
							wBeginFreq = wOrgFreq;
							wEndFreq = wRfLimitHi;
							g_wLastSetRF = wEndFreq;
						}
					}
					break;
				} // EndSwitch
			} // EndElse byFlag & SCANSTN_AUTO
		} // EndElseIf (byResult == LVLS_RFFREQ_ENDOFBAND_ERR
		else	// Other error - terminate
		{
			bDone = TRUE;
		}
	} // EndWhile
		
	// Restore chip config
	SetChipStereo(bOrgAudioMode);
	//@@@ DriveBit(IR01_RADIO_CTRL2_REG, IR1_RCTL2_AGC_SPD, bOrgAgcSpeed); //@@@ Restore AGC speed state
	SetChipAudioMute(bOrgAudioMute, MUTESRC_OPERATION);
	SetUpChipMode(wCfg);

	// Report the final status via callback
	CallBack(CRSN_STN_CHNG);
	return(byResult);
} // End ScanStation
