/************************************************************************
*
*   Copyright(c) 2004 ItoM BV
*   All Rights Reserved.
*
*   LV2400x evaluation kit: Take care of shadowing LV2400x registers
*   File name:	DgtParser.c
*
*************************************************************************/

#include <stdio.h>
#include "fm_common.h"
#include "Lv24Ekit.h"


/*-------------------------------------------------------------------
        Parser routines
-------------------------------------------------------------------*/
void WriteReg(WORD wRegAddress, BYTE byValue)
{
	// Select the block
	SelRegBlock(MSB(wRegAddress));	// High byte of wRegAddress is block number
	
	// Shadow the write-register
	ShadowReg(wRegAddress, byValue);

	// Write data to register 
 	IoWrite3W(byValue, LSB(wRegAddress)); // low byte of wRegAddress is Register offset
 	
} // End WriteReg

/* ************************************************************************************************
 *
 *  Function:   ReadReg
 *
 *  Authors:    Hung van Le
 *  Purpose:    Read a register from the chip
 *  Input:	wRegAddress: complete address (with block/offset) of the register to be read
 *  Output:     The 8 bit read data 
 *  Comments:   The block select register (BLK_SEL) won't be restored after the read
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
BYTE ReadReg(WORD wRegAddress)
{
	//WORD wOrgOutput;

	// TODO: validate wRegAddress
	//if (Invalidate address )
	//	return(LVLS_INVALID_REG_ADDRESS_ERR);

	// Select register output to read data
	//wOrgOutput=SetUpChipMode(CO_REG);

	// Select the block
	SelRegBlock(MSB(wRegAddress));	// High byte of wRegAddress is block number

	// Read data at the register offset
	return(IoRead3W(LSB(wRegAddress)));

	// Restore output
	//SetUpChipMode(wOrgOutput);
} // End ReadReg

BYTE DriveBit(WORD wRegAddress, BYTE byBitMask, BYTE byState) 
{
	// Driving a bit (specified by byBitMask) of wRegAddress to byState - Positive logic (ie: TRUE = set bit)
	//		wRegAddress: register address
	//		byBitMask: mask to toggle bit value
	//		byState: TRUE -> enable function (set bit), FALSE -> disable function (clear bit)
	// Return the original state:
	//		TRUE: function was enabled (bit was set)
	//		FALSE: function was disabled (bit was clear)
	BYTE byWriteValue;

	// Fetch current value
	byWriteValue = GetSwRegValue(wRegAddress);

	// Manipulate bit
	if (byState) // Enable function -> Set bit request
	{
		if ( byWriteValue & byBitMask)
			return(TRUE); // Bit already set (function is already enabled) - return ON state
		else
			byWriteValue |= byBitMask; // set bit for ON state
	}
	else // Disable function -> Clear bit request
	{
		if ( (byWriteValue & byBitMask) == 0 )
			return(FALSE); // Bit already clear (function is already disabled) - return OFF state
		else
			byWriteValue &= ~byBitMask; // clear bit for off state
	}

	// Update change
	WriteReg(wRegAddress, byWriteValue);

	// Return the previous state (ie if the bit is now on (enabled), previous state was disabled->return false, and vice versa)
	return(( (byWriteValue) & byBitMask) == 0);
} // End DriveBit

BYTE DriveBitNeg(WORD wRegAddress, BYTE byBitMask, BYTE byState)
{
	// Driving a bit (specified by byBitMask) of wRegAddress to byState - Negative logic (ie: enable = clear bit)
	//		wRegAddress: register address
	//		byBitMask: mask to toggle bit value
	//		byState: TRUE -> Enable function (clear bit), FALSE -> disable function (set bit)
	// Return the original state:
	//		TRUE: function was enabled (bit was clear)
	//		FALSE: function was disable (bit was set)
	BYTE byWriteValue;

	// Fetch current value
	byWriteValue = GetSwRegValue(wRegAddress);

	// Manipulate bit
	if (byState) // Enable function -> clear bit
	{
		if ( (byWriteValue & byBitMask) == 0 )
			return(TRUE); // Bit already clear (function is already enabled) - return ON state
		else
			byWriteValue &= (BYTE)~byBitMask; // Bit is set - clear it to enable function
	}
	else // Disable function -> set bit
	{
		if ( byWriteValue & byBitMask)
			return(FALSE); // Bit already set (function is already disabled) - return OFF state
		else
			byWriteValue |= byBitMask; // Bit is clear - set bit to disable function
	}

	// Update change
	WriteReg(wRegAddress, byWriteValue);

	// Return the previous state (ie if the bit is now on (disabled), previous state was enabled->return true, and vice versa)
	return((byWriteValue & byBitMask) != 0);
} // End DriveBitNeg

BYTE SetRegBits(WORD wRegAddress, BYTE byValueMask, BYTE bySetValue)
{
	// Set a group bits of wRegAddress
	// Return org. value
	BYTE byWriteValue, byOrgValue;

	// Fetch current value
	byWriteValue = GetSwRegValue(wRegAddress);

	// Determine org. value of byValueMask
	byOrgValue = byWriteValue & byValueMask;

	// Skip writing to the register if value is already set
	if (byOrgValue != bySetValue)
	{
		byWriteValue &= (BYTE)(~byValueMask);	// Clear current value
		byWriteValue |= bySetValue;		// Patch new value
		WriteReg(wRegAddress, byWriteValue); 	// Write it
	}

	// Return the org. value
	return(byOrgValue);
} // End SetRegBits
