/************************************************************************
*
*   Copyright(c) 2004 ItoM BV
*   All Rights Reserved.
*
*   LV2400x evaluation kit: Algortihm used by LV2400x
*   File name:	TuneAlg..c
*
*************************************************************************/

#include "ms_customize_trc.h"
#include <stdio.h>
#include <stdlib.h> // for abs
#include "fm_common.h"
#include "Lv24Ekit.h"

#ifdef USE_LV2400x
#include "Lv2400xReg.h"	// for QSS_xxx equations
#endif //USE_LV2400x

//#define SCI_TRACE_LOW SCI_TraceLow
//extern void SCI_TraceLow(const char*,...);
#include "os_api.h"


/*-------------------------------------------------------------------
        Helpers
-------------------------------------------------------------------*/
BYTE SearchInByteArray(BYTE byToFind, BYTE _rom pbyArray[], BYTE byArraySize)
{
	BYTE i;

	for (i=0; i<byArraySize; i++)
	{
		if (byToFind == pbyArray[i])
			return(i);
	}
	return((BYTE)-1); // Not found
} // End SearchInByteArray

/*-------------------------------------------------------------------
        Algorithm routines
-------------------------------------------------------------------*/
/* ************************************************************************************************
 *
 *  Function:   LinTuneDac
 *
 *  Authors:    Hung van Le
 *  Purpose:    Generic routine to tune all OSC registers of the chip (using lineair interpolating)
 *  Input:      
 *			DWORD dwExpFreq: expected frequency in Hz
 *			DWORD dwMeasureTimeUs: measure time in us
 *			FPTR_WRITE_OSC fpWriteOsc: pointer to Write_xxx_Osc routine
 *			WORD wX1, WORD wX2: start X value - see comment
 *			int iOscRes: Osc resolution - see comment
 *			BYTE bPrecision: need precision if 1
 *  Output:     Status as defined in LvErr.h
 *  Comments:
 *		Algorithm:
 *			x1 = point1, measure f1
 *			x2 = point2;
 *			Loop
 *				write x2
 *				measure f2
 *				xe = interpolate(x1,f1,x2,f2,fexp)
 *				fe = measure
 *				check fe against fexp, exit if OK
 *				Move X2,f2 to X1, f1; xe,fe to X2,f2
 *			goto loop
 *
 *		Input wX1, wX2:
 *			use wX1, wX2 for point 1,2
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE LinTuneDac(WORD wExpFreq, DWORD dwMeasureTimeUs, FPTR_WRITE_OSC fpWriteOsc, WORD wX1, WORD wX2, BYTE bPrecision)
{
	BYTE  byResult;
	WORD  wY1, wY2;
	WORD  wOscValue;
	int   iStep, iCmpRes;
	BYTE  bDone;

	// Init var.
	bDone = FALSE;
	iStep = 0; // Avoid compiler's complain

	// ----- Draw a line to approach the desired point --------
	// Determine point1
	fpWriteOsc(wX1);				// Set X1
	byResult = CountPulse(dwMeasureTimeUs, &wY1);	// Get Y1
	
	
	if (byResult != LVLS_NO_ERROR)
		goto return_LinTuneDac;

	while (!bDone)
	{
		byResult = fpWriteOsc(wX2);			// Set X2
		
	    
		if (byResult != LVLS_NO_ERROR)
			goto return_LinTuneDac;
		byResult = CountPulse(dwMeasureTimeUs, &wY2);	// Get Y2
		//SCI_TRACE_LOW("Get wY2= 0x%x,return 0x%x\n",wY2,byResult);
		if (byResult != LVLS_NO_ERROR)
			goto return_LinTuneDac;
		
		// Check if we have correct frequency
		if ( (iCmpRes=IsFrequencyOk(wY2, wExpFreq, dwMeasureTimeUs)) == 0 )
		{
			bDone = TRUE;		// frequency within margin
		}
		else
		{
			// Interpolate the 2 points for the OscStep
			iStep = InterpolateOscStep(wExpFreq, wX1, wX2, wY1, wY2);
			wOscValue = wX1 + iStep; // this is the new X2
			// Move X2 to X1 when the X2 is nearer the expected frequency
			//if (dwY1>dwExpFreq)
			if ( (abs((int)wY2-(int)wExpFreq)) < (abs((int)wY1-(int)wExpFreq)) )
			{
				wX1 = wX2;
				wY1 = wY2;
			}

			// Calculate the new X2
			wX2 = wOscValue;
			if ( abs(iStep) < 1 )
			{
				// When the slew of the curve is too small, we can not interpolate anymore (step=0)
				// Prepare to step to the frequency
				// Validate wX2
				if ( fpWriteOsc(wX2) != LVLS_NO_ERROR )	
					wX2= wX1;	// Not OK - use the other point

				// Force stepping if not specified by caller
				if ( !bPrecision )
				{
					iStep = 20;		// Step to approach (will be divided by 2)
					bPrecision = 1;		// Force stepping
				}
				break; 
			}

		}
	}

	// Can not tune osc with interpolating - trying step (until step=1)
	if ( bPrecision && (!bDone) )
	{
	
	    
#define FTO_TOO_BIG		0x01
#define FTO_TOO_SMALL		0x02
#define FTO_APPROACH_UP		0x04
#define FTO_APPROACH_DOWN	0x08
#define FTO_BOTH_DIR		(FTO_TOO_BIG|FTO_TOO_SMALL)
#define FTO_APPROACH_BOTH	(FTO_APPROACH_UP|FTO_APPROACH_DOWN)
		WORD wFlag;
		BYTE byLoopCnt;
		
		iStep /=2;
		if (iStep==0)
			iStep = 1;
		wFlag=0;
		byLoopCnt = 10;
		while (!bDone)
		{
			byResult = fpWriteOsc(wX2);			// Set X2
			if (byResult != LVLS_NO_ERROR)
				goto return_LinTuneDac;
			byResult = CountPulse(dwMeasureTimeUs, &wY2);	// Get Y2
			if (byResult != LVLS_NO_ERROR)
				goto return_LinTuneDac;
			
			// Check if we have correct frequency
			if ( (iCmpRes=IsFrequencyOk(wY2, wExpFreq, dwMeasureTimeUs)) == 0 )
			{
				bDone = TRUE;		// frequency within margin
			}
			else
			{				
				if (iCmpRes <0)
				{
					wFlag |= FTO_TOO_SMALL; // increase OSC)
					iStep = abs(iStep);
					if (iStep == 1)
						wFlag |= FTO_APPROACH_UP;
				}
				else // iCmdRes > 0
				{
					wFlag |= FTO_TOO_BIG;
					iStep = -abs(iStep);
					if (iStep == -1)
						wFlag |= FTO_APPROACH_DOWN;
				}
			}
			if ( (wFlag & FTO_APPROACH_BOTH)==FTO_APPROACH_BOTH) // we did approach with step=1 for both directions
			{
				bDone = TRUE;
			}
			else if ((wFlag & FTO_BOTH_DIR)==FTO_BOTH_DIR)		// we did approach for both directions
			{
				iStep /= 2;			// Keep approaching with half step
				if (iStep==0)
					iStep = 1;		// sign will be corrected by comparing
			}
			wX2 += iStep;
			byLoopCnt--;
			if ( byLoopCnt==0 )
				break;
		} // EndWhile
	} // EndIf !bDone

return_LinTuneDac:
	//return(MakeError(0, 0, LVLS_TUNE_OSCREG_OP, byResult));
	return(byResult);
} // End LinTuneDac 

WORD CalculateCoeff(WORD wFreq) 
{ 
	// Coefficient calculation: Coeff = Constance/f^2 
	// The contance is fixed at (2^32-1) * 2^9 (for range 65 MHz - 130MHz)
	// The calculation is 
	//		  2^32-1
	//		---------- * 2^9
	//		f in 10 kHz
	//		------------------
	//			f in 10 kHz

#define _C1 0xFFFFFFFF		// 2^32-1
#define _C2_SHIFT 9
#define _C2 (1<<_C2_SHIFT)	// 2^9
	DWORD dwTmp;

	// Can not calculate when freq is 0
	if (wFreq == 0)
		return(0);

	dwTmp = _C1/wFreq;
	//dwTmp *= _C2;
	dwTmp <<= _C2_SHIFT;

	return((WORD)(dwTmp/wFreq));
//	return( ((_C1/wFreq) * _C2)/wFreq );
} // End CalculateCoeff

WORD InterpolateY(WORD wX, WORD wX1, WORD wX2, WORD wY1, WORD wY2)    
{
	INT32 iDeltaX;
	INT32 sA,sB, sC;

	iDeltaX = (int)wX1-(int)wX2;
	if (iDeltaX==0)
		return(0);
//	return( ((((int)dwY1-(int)dwY2)*((int)wX-(int)wX1) +(iDeltaX/2))/iDeltaX) + (int)dwY1 );
	sA = (INT32)wY1 - (INT32)wY2;
	sB = sA * ((int)wX - (int)wX1);
	sB += (iDeltaX/2);
	sC = sB/iDeltaX;
	sC += (INT32)wY1;
	return((WORD)sC);
} // End InterpolateY

WORD InterpolateX(WORD wY, WORD wX1, WORD wX2, WORD wY1, WORD wY2)
{
	INT32 iDeltaY;
	INT32 sA,sB, sC;

	iDeltaY = (INT32)wY1 - (INT32)wY2;
	if (iDeltaY==0)
		return(0);
//	return( (WORD)(((((signed long)dwY-(signed long)dwY1)*((int)wX1-(int)wX2) + (iDeltaY/2))/iDeltaY) + (int)wX1) );
	sA = (INT32)wY - (INT32)wY1;
	sB = sA * ((int)wX1 - (int)wX2);
	sB += (iDeltaY/2);
	sC = sB/iDeltaY;
	sC += (int)wX1;
	return((WORD)sC);
} // End InterpolateX

int InterpolateOscStep(WORD wY, WORD wX1, WORD wX2, WORD wY1, WORD wY2)
{
	// Draw a line from (X1,Y1) to (X2,Y2) to calculate the X (ie the OSC-value) belongs to desired Y (ie the dwExpFreq)
	// formula:
	//					x2 - x1
	//			step = (y - y1)---------
	//					y2 - y1
	// The OscValue is then (x1+step)
	INT32 iDeltaY;
	INT32 sA,sB, sC;

	iDeltaY = (INT32)wY2-(INT32)wY1;
	if (iDeltaY == 0 )
		return(0);
//	return( (int)((((signed long)dwY - (signed long)dwY1) * ((int)wX2 - (int)wX1)) + (iDeltaY/2))/iDeltaY );
	sA = (INT32)wY - (INT32)wY1;
	sB = sA * ((int)wX2 - (int)wX1);
	sB += (iDeltaY/2);
	sC = sB/iDeltaY;
	return((int)sC);
} // End InterpolateOscStep

BYTE InitTuningRfCapOsc(void)
{
	WORD wCfg;
	WORD wTmp;

	// Select correct oscillator output and enable measuring mode
	wCfg = SetUpChipMode(CO_RF_OSC | CHIP_MEASURE_MODE | CHIP_AFC_OFF);
    //SCI_TRACE_LOW:"SetUpChipMode() return 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TUNEALG_308_112_2_18_0_32_57_1477,(uint8*)"d",wCfg);
	// Determine the min/max RF frequency (hardware RF-limit)
	// Set CAP/OSC at minimal value and measure the the minimal hardware RF
	GetRfAtCapOsc(QSS_MIN_RFCAP, QSS_MIN_RFOSC, LV_MSR_TIME_32ms, &g_wHwRfLow);
    //SCI_TRACE_LOW:"GetRfAtCapOsc() g_wHwRfLow 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TUNEALG_312_112_2_18_0_32_57_1478,(uint8*)"d",g_wHwRfLow);
	// Set CAP/OSC at maximal value and measure the maximal hardware RF
	GetRfAtCapOsc(QSS_MAX_RFCAP, QSS_MAX_RFOSC, LV_MSR_TIME_32ms, &g_wHwRfHigh);
    //SCI_TRACE_LOW:"GetRfAtCapOsc() g_wHwRfHigh 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TUNEALG_315_112_2_18_0_32_57_1479,(uint8*)"d",g_wHwRfHigh);

	// Init QSSF-data
	// Determine the 2 points for CAP-low line and the OSC-low resolution
	Qssf1.CoefLo = CalculateCoeff(g_wHwRfLow);	// Point1_0: Cap=min, Osc=min)
	GetRfAtCapOsc(QSS_MIN_RFCAP, QSS_MAX_RFOSC, LV_MSR_TIME_32ms, &wTmp);
	Qssf1.CoefHi = CalculateCoeff(wTmp);		// Point1_1: Cap=min, Osc=max)

	// Determine the 2 points for CAP-high line and the OSC-high resolution
	GetRfAtCapOsc(QSS_MAX_RFCAP, QSS_MIN_RFOSC, LV_MSR_TIME_32ms, &wTmp);
	Qssf2.CoefLo = CalculateCoeff(wTmp);		// Point2_0: Cap=max, Osc=min)
	Qssf2.CoefHi = CalculateCoeff(g_wHwRfHigh);	// Point2_0: Cap=max, Osc=max)
    //SCI_TRACE_LOW:"Qssf1.CoefLo 0x%x,Qssf1.CoefHi 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TUNEALG_327_112_2_18_0_32_57_1480,(uint8*)"dd",Qssf1.CoefLo,Qssf1.CoefHi );
    //SCI_TRACE_LOW:"Qssf2.CoefLo 0x%x,Qssf2.CoefHi 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TUNEALG_328_112_2_18_0_32_57_1481,(uint8*)"dd",Qssf2.CoefLo,Qssf2.CoefHi );

	// Restore chip config
	SetUpChipMode(wCfg);

	// Error check
	if ( g_wHwRfHigh <= g_wHwRfLow)
		return(LVLS_INV_RFLIMIT_ERR);
	return(LVLS_NO_ERROR);
} // End InitTuningRfCapOsc

BYTE GetRfAtCapOsc(WORD wCapValue, WORD wOscValue, DWORD dwMsrTimeUs, PWORD pwFreq)
{
	WriteRfCap(wCapValue);
	WriteRfOsc(wOscValue);
	return(CountPulse(dwMsrTimeUs, pwFreq)); // Measure the RF
} // End GetRfAtCapOsc

BYTE ScanSetFreq(WORD wTuneFreq, BYTE byPrecisionLevel)
{
	// iPrecisionLevel		ScanSetFreq	  	CalculateRfCapOsc
	//	QSSF_PRECISION_NONE	WriteOsc	  	No CAP verification
	//	QSSF_PRECISION_LOW	WriteOsc	  	CAP verification with 8ms measuring
	//	QSSF_PRECISION_MED	FineTuning 8ms	  	CAP verification with 32ms measuring
	//	QSSF_PRECISION_HIGH	FineTuning 32ms	  	CAP verification with 64ms measuring
	BYTE byResult;
	WORD  wCapValue, wOscValue;

	// Calculate Cap, Osc value
	byResult = CalculateRfCapOsc(wTuneFreq, &wCapValue, &wOscValue, byPrecisionLevel);

	// Write CAP value
	if (byResult == LVLS_NO_ERROR)
	{
		if (GetRfCapValue() != wCapValue)
			byResult = WriteRfCap(wCapValue);
	}
			
	// Fine tuning if precision needed
	if (byResult == LVLS_NO_ERROR)
	{
		if (byPrecisionLevel==QSSF_PRECISION_MED)
		{
			byResult = FineTuneRfOsc(wTuneFreq, LV_MSR_TIME_8ms, wOscValue, FALSE);
		}
		else if (byPrecisionLevel==QSSF_PRECISION_HIGH)
		{
			byResult = FineTuneRfOsc(wTuneFreq, LV_MSR_TIME_32ms, wOscValue, FALSE);
		}
		else // byPrecisionLevel==QSSF_PRECISION_NONE or QSSF_PRECISION_LOW
		{
			// Just write the OscValue, (CapValue is already written by CalculateRfCapOsc
			byResult = WriteRfOsc(wOscValue);
		}
	}
	if (byResult != LVLS_NO_ERROR)
		byResult = FineTuneRfOsc(wTuneFreq, LV_MSR_TIME_32ms, QSS_MIN_RFOSC, FALSE);
	return(byResult);
} // End ScanSetFreq

/* ************************************************************************************************
 *
 *  Function:   FineTuneRfOsc
 *
 *  Authors:    Hung van Le
 *  Purpose:    Validate and possibly correcting the calculated CAP/OSC
 *  Input:      
 *  Output:     Status as defined in LvErr.h
 *  Comments:
 *
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE FineTuneRfOsc(WORD wRfFreq, DWORD dwMeasureTimeUs, WORD wExpOscValue, BYTE byNeedPrecision)
{	
	BYTE i;
	BYTE byResult;
	WORD wCapValue;
	
	wCapValue = GetRfCapValue();
	for (i=0; i<10; i++)
	{
		byResult = WriteRfCap(wCapValue);
		if (byResult == LVLS_NO_ERROR)
		{
			byResult = LinTuneDac(wRfFreq, dwMeasureTimeUs, WriteRfOsc, wExpOscValue, QSS_MAX_RFOSC, byNeedPrecision);
			wExpOscValue = QSS_MIN_RFOSC; // Change to minimun value for next approach
		}
		if ( byResult != LVLS_RFOSC_OUTOFLIMIT_ERR)
			break;
		if (g_wLastMsrRF < wRfFreq )
			wCapValue++;
		else
			wCapValue--;
	}
	
	return(byResult);
} // End FineTuneRfOsc

BYTE CalculateRfCapOsc(WORD wFreq, PWORD pwCap, PWORD pwOsc, BYTE byPrecisionLevel)
{
	// Calculate the CAP/OSC value for dwFreqHz (frequency in Hz)

	WORD wCoeff, wCorFreq;
	BYTE byRetry;
	WORD wCap;
	DWORD dwMsrTimeUs;
	
	// TODO: Check if QSSF-data initialised
//	if (m_pQssfData==NULL)
//		return(LVLS_NO_SSF_DATA_ERR); 
	
	// calculate Coeff of desired frequency
	wCoeff = CalculateCoeff(wFreq);
	
	// Determine measuring deviation
	if (byPrecisionLevel==QSSF_PRECISION_NONE)
	{
		dwMsrTimeUs = 0;
		wCorFreq = 0;
	}
	else if (byPrecisionLevel==QSSF_PRECISION_LOW)
	{
		dwMsrTimeUs = LV_MSR_TIME_8ms;
		wCorFreq = 3;	// (128 Hz * 256)/10 kHz (128Hz: fault by 8ms, 256: RF divider factor, 10kHz:RF unit
	}
	else if (byPrecisionLevel==QSSF_PRECISION_MED)
	{
		dwMsrTimeUs = LV_MSR_TIME_32ms;
		wCorFreq = 0;	// (32 Hz * 256)/10 kHz (32Hz: fault by 32ms, 256: RF divider factor, 10kHz:RF unit
	}
	else //if (byPrecisionLevel==QSSF_PRECISION_HIGH)
	{
		dwMsrTimeUs = LV_MSR_TIME_64ms;
		wCorFreq = 0;	// (16 Hz * 256)/10 kHz (16Hz: fault by 64ms, 256: RF divider factor, 10kHz:RF unit
	}	
	//wCorFreq *= GetDividerFactor();
	//wCorFreq /= 10000; //@@@ 10 kHz unit for RF

	// Derive CAP from low data
	wCap = InterpolateX( wCoeff, QSS_MIN_RFCAP, QSS_MAX_RFCAP, Qssf1.CoefLo, Qssf2.CoefLo);

	// Calculate OSC value
	byRetry = 3;
	WriteRfOsc(QSS_MIN_RFOSC);	// Kep OSC at fixed value during CAP calculation
	while (byRetry)
	{
		WORD wCoefLo, wCoefHi, wCoefRange, wCoefReal, wCoefCor;
		WORD wMsrFreq;
		
		// Get coefficient of calculated CAP from low Qfm-Data 
		wCoefLo = InterpolateY(wCap, QSS_MIN_RFCAP, QSS_MAX_RFCAP, Qssf1.CoefLo, Qssf2.CoefLo);
		
		// Get coefficient of calculated CAP from high Qfm-Data 
		wCoefHi = InterpolateY(wCap, QSS_MIN_RFCAP, QSS_MAX_RFCAP, Qssf1.CoefHi, Qssf2.CoefHi);
		
		// Calculate range
		wCoefRange = wCoefLo - wCoefHi;	// vCoefLo should be greater than vCoefHi
		
		// Verify CAP value: Write CAP, OSC and measure the frequency
		if ( WriteRfCap(wCap) == LVLS_NO_ERROR)
		{
			// Determine the real coefficient of this CAP
			if (byPrecisionLevel != QSSF_PRECISION_NONE)
			{
				CountPulse(dwMsrTimeUs, &wMsrFreq);
				wCoefReal = CalculateCoeff(wMsrFreq);
				if (wCorFreq > 0)
					wCoefCor = wCoefReal - CalculateCoeff(wMsrFreq+wCorFreq);
				else
					wCoefCor = 0;
			}
			else
			{
				wCoefReal = wCoefLo;
				wCoefCor = 0;
			}
			
			// Do we have the correct CAP
			if ( (wCoeff<=(wCoefReal+wCoefCor)) && (wCoeff>=(wCoefReal-wCoefRange-wCoefCor)) )
			{
				// Derive the OSC value
				*pwOsc = InterpolateX(wCoeff, QSS_MIN_RFOSC, QSS_MAX_RFOSC, wCoefReal+wCoefCor, wCoefReal-wCoefRange-wCoefCor);
				
				// Done when approriate OSC value
				if ( (*pwOsc >= QSS_MIN_RFOSC) && (*pwOsc <= QSS_MAX_RFOSC))
					break;
			}
			
			// Not success, recalculate and try again
			{
				WORD wCapNew;
				
				wCapNew = InterpolateX(wCoeff, wCap, QSS_MAX_RFCAP, wCoefReal, Qssf2.CoefLo);
				if (wCapNew == wCap) // Interpolate doesn't work - stepping
				{
					if (wCoeff<wCoefReal)
						wCap++;	// Increase CAP to decrease vCoefReal
					else
						wCap--;
				}
				else
					wCap = wCapNew;
			}
			// Retry
			byRetry--;
		}
		else
		{
			// Write Cap failed - terminate
			wCap = QSS_MAX_RFCAP; //xx
			*pwOsc=QSS_MAX_RFOSC;  //xx
			break;
			//iRetry=0;
		}
	} // EndWhile

	*pwCap=wCap;
	if (byRetry==0)
		return(LVLS_CAPOSC_CALC_ERR);
	return(LVLS_NO_ERROR);
} // End CalculateRfCapOsc
