/* ************************************************************************************************
 *
 *  Filename:	LvErr.h
 *  Project:    LV24EKIT
 *  Authors:	Hung van Le
 *  Purpose:    Error codes of LV24 evaluation kit
 *  Comments:   None
 * ************************************************************************************************
 *  History:
 *  Version	Date			Author		Reason
 *    1.00	15-May-2004		HLE			- Initial version
 *
 * ************************************************************************************************
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE.
 *
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */

#if !defined(LVERR_H_INCLUDED_)
#define LVERR_H_INCLUDED_

// ------------------------
// Error codes
// ------------------------
#define LVLS_NO_ERROR					0		// no error

// Software failures
#define LVLS_NOT_DEFINED_ERR			0x01	// Not defined error
#define LVLS_NO_MEMORY_ERR				0x02	// Error allocating memory
#define LVLS_USER_ABORT_ERR				0x03	// User abort
#define LVLS_CAN_NOT_ABORT_ERR			0x04	// Abort is not possible
#define LVLS_CHIP_NOT_SUPPORT_ERR		0x05	// Not supported chip version
#define LVLS_INVALID_PARAMETER_ERR		0x06	// Invalid paramter
#define LVLS_CHIP_BUSY_ERR				0x07	// Chip busy
#define LVLS_INVALID_REGION_ERR			0x08	// Attemp to set a not supported region
#define LVLS_INV_LIMIT_SETTING_ERR		0x09	// Invalid limit (like low limit is greate than high limit)
#define LVLS_NSU_FEATURE_ERR			0x0A	// Not supported feature (device capabilities ID)
#define LVLS_INV_FEATURE_VALUE_ERR		0x0B	// Invalid feature value
#define LVLS_IMPROPER_HW_SETTING_ERR	0x0C	// Attempt to set the hardware in improper mode
#define LVLS_NSU_FUNCTION_ERR			0x0D	// Function not supported
#define LVLS_FEATURE_NA_NOW_ERR			0x0E	// Feature (is not available in current radio mode

#define LVLS_NO_CHIP_ERR				0x10	// No chip found
#define LVLS_NO_MORE_PRST_MEM			0x11	// Out of preset storage memory (for storing frequency)
#define LVLS_EMPTY_PRESET				0x12	// Attempt to recall an empty preset
#define LVLS_FLASH_READ_ERR				0x13	// Flash read error
#define LVLS_FLASH_WRITE_ERR			0x14	// Flash write error
#define LVLS_FLASH_CORRUPT_ERR			0x15	// Flash corrupted

#define LVLS_TUNE_OSC_ERR				0x20	// Tuning Osc-register failed
#define LVLS_IFOSC_OUTOFLIMIT_ERR		0x21	// An invalid value is sent to IF Osc register
#define LVLS_STEREOSC_OUTOFLIMIT_ERR	0x22	// An invalid value is sent to Stereo Osc register
#define LVLS_RFOSC_OUTOFLIMIT_ERR		0x23	// An invalid value is sent to RF Osc register
#define LVLS_RFCAP_OUTOFLIMIT_ERR		0x24	// An invalid value is sent to RF CAP register
#define LVLS_RDSOSC_OUTOFLIMIT_ERR		0x25	// An invalid value is sent to RDS Osc register
#define LVLS_INV_REG_ADDRESS_ERR		0x26	// Attemp to write to an invalid register
#define LVLS_INV_RFLIMIT_ERR			0x27	// Improper RF limit
#define LVLS_CAPOSC_CALC_ERR			0x28	// Unable to calculate CAP/OSC value
#define LVLS_POLLING_CNT_TIMEOUT		0x29	// Polling counting done timeout
#define LVLS_AACAP_OUTOFLIMIT_ERR		0x2A	// An invalid value is sent to AM Antenna capacitor register

#define LVLS_RFFREQ_ENDOFBAND_ERR		0x30	// RF frequency reaches the lower or upper band limit
#define LVLS_RFFREQ_IF_FINETUNE_ERR		0x31	// Fine tune RF to IF failed (Not a valid station ?)
#define LVLS_HW_AFC_FINETUNE_ERR		0x32	// Fine tune by hardware AFC failed (drift is too big?)
#define LVLS_UNREACHABLE_FREQ_ERR		0x33	// Requested frequency exceeds hardware limit
#define LVLS_NO_STATION_FOUND_ERR		0x34	// No station found after searching the whole band
#define LVLS_FREQ_IN_LIMIT_GAP_ERR		0x35	// Invalid frequency to be set

#define LVLS_INV_KPMENU_ERR				0x40	// Invalid keypad menu

// Internal errors
#define LVLS_FREQ_JUMP_ERR				0x50	// Warning - frequency is adjusted

#define LVLS_EARPHONE_ERR               0x60    // earphone not detect...add by Eric.wei
// ////////////////////////////////////////////////////////////////////////////
#endif // !defined(LVERR_H_INCLUDED_)

