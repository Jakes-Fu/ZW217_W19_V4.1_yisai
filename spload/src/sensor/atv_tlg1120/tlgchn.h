#ifndef _TLGCHN_H_
#define _TLGCHN_H_

/****************************************************************************/
/*!@file  tlgchn.h
 *
 * @brief Header file for TLGCHN (Channel Map Management).
 *
 * Detailed description of subsystem, what it does, who may use it, any
 * advice to the user.
 *
 * @sa TLGAPP
 *
 ****************************************************************************/
/* Revision Information:
 *
 * $Id: //software/dev/main/mod/template/template.h#3 $
 *
 * $Revision: #3 $
 *
 * Copyright (c) 2010 Telegent Systems.  All rights reserved.
 *
 * This software is the confidential and proprietary information of Telegent
 * Systems, Inc. ("Confidential Information"). You shall not disclose such
 * Confidential Information and shall use it only in accordance with the
 * Terms of Sale of Telegent products and the terms of any license agreement
 * you entered into with Telegent for such products.
 *
 * TELEGENT PROVIDES THE TELEGENT SOURCE CODE STRICTLY ON AN "AS IS" BASIS,
 * WITHOUT ANY WARRANTY WHATSOEVER, AND TELEGENT EXPRESSLY DISCLAIMS ALL
 * WARRANTIES, EXPRESS, IMPLIED OR STATUTORY WITH REGARD THERETO, INCLUDING
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE OR NON-INFRINGEMENT OF THIRD PARTY RIGHTS. TELEGENT SHALL
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY YOU AS A RESULT OF USING,
 * MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * INCLUDES
 *
 ****************************************************************************/
#include "tlg_error.h"

/*****************************************************************************
 *
 * CONSTANTS
 *
 ****************************************************************************/

#define TLGCHN_RESOLUTION_720_480 ((uint8_t) 0)
#define TLGCHN_RESOLUTION_720_576 ((uint8_t) 1)

#define TLGCHN_MAX_CHANNELS_IN_MAP (255)

/* TLGCHN_SMALLEST_SUPPORTED_COUNTRY_CODE represents Greece.
   The country codes are from 
   "Annex to ITU Operational Bulletin, No. 897 - 1.XII.2007"*/
#define TLGCHN_SMALLEST_SUPPORTED_COUNTRY_CODE (202)

/* TLGCHN_LARGEST_SUPPORTED_COUNTRY_CODE represents the Falkland Islands */
#define TLGCHN_LARGEST_SUPPORTED_COUNTRY_CODE (750)

/* Start of TLGCHN additions to the ITU country codes.
   These defines can be passed in to TLGCHN_SetChannelMap
*/
#define TLGCHN_ITU_CC_SHENZHEN_CHINA (462)
/* End of TLGCHN additions to the ITU country codes */

/*****************************************************************************
 *
 * TYPES
 *
 ****************************************************************************/

#ifdef __cplusplus
extern "C"  {
#endif /* __cplusplus */

/*****************************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 ****************************************************************************/

/*****************************************************************************
 * TLGCHN_SetChannelMap                                                      *
 ****************************************************************************/
/** Selects the channel map based on an ITU Country Code.
 *
 * Selects the channel map based on an ITU Country Code.  A channel map must be
 * selected before other calls to the TLGCHN can be made.
 *
 * ITU Country Codes can be found in the document titled:
 * "Annex to ITU Operational Bulletin, No. 897 – 1.XII.2007"
 *
 * @param itu_code  ITU Country Code
 *                  Additions to ITU country codes:
 *                  462 has been added to represent Shenzhen, China
 * 
 * @returns
 *
 * @retval  TLG_ERR_SUCCESS       Channel map has been selected for the country code
 * @retval  TLG_ERR_BAD_PARAM     No channel map exists for the given country code
 *
 ****************************************************************************/
TLGDLL_API tlg_errcode_t TLGCHN_SetChannelMap(uint16_t itu_code);

/*****************************************************************************
 * TLGCHN_ScanStart                                                          *
 ****************************************************************************/
/** Start the Telegent Subsystem Scan mode.
 *
 * This procedure puts the Telegent subsystem into scan mode which tells
 * the subsystem that the application code is attempting to detect channel
 * presence rather than normal service playing. The subsystem
 * can use this information to optimize performance for a scan.
 *
 * This call should be followed by a series of 
 * TLGCHN_SetChannel() ending with a call to TLGCHN_ScanStop()
 *
 * NOTE: TLGAPP_StartChannelScan will be called by TLGCHN_ScanStart.  Users
 *       of TLGCHN_ScanStart do not need to also call TLGAPP_StartChannelScan.
 *
 * @param p_tlg_scan_map  map to fill with locked channels. If no scan map
 *                      is to be used, this parameter may be set to NULL.
 *                      When the scan is stopped, the first 0 found in the 
 *                      scan map denotes the rest of the entries in the scan 
 *                      map are invalid.  If there is no 0 in the scan map,
 *                      all entries are valid.
 * @param max_scan      total number of channel entries in tlg_scan_map. If
 *                      no scan map is being used, this parameter is ignored.
 *
 * @retval TLG_ERR_SUCCESS  Call succeeded.
 * @retval TLG_ERR_PARAM    Bad parameter value passed.
 * @retval TLG_ERR_FAIL     Error during call.
 *
 * @sa TLGCHN_ScanStop
 * @sa TLGCHN_SetChannel
 */ 
TLGDLL_API tlg_errcode_t TLGCHN_ScanStart(uint8_t* p_tlg_scan_map, uint8_t max_scan);

/****************************************************************************
 * TLGCHN_ScanStop()
 ****************************************************************************/
/** Exit the Telegent Subsystem Scan mode.
 *
 * This procedure stops/exits the Telegent subsystem scan mode and returns
 * the subsystem to "normal" tuning charactistics.
 *
 * NOTE: TLGAPP_StopChannelScan will be called by TLGCHN_ScanStop.  Users
 *       of TLGCHN_ScanStop do not need to also call TLGAPP_StopChannelScan.
 *
 * @returns none
 *
 * @sa TLGCHN_ScanStart
 * @sa TLGCHN_SetChannel
 *
 ****************************************************************************/
TLGDLL_API void TLGCHN_ScanStop(void);

/****************************************************************************
 * TLGCHN_SetChannel()
 ****************************************************************************/
/** Tune and play or scan the specified assigned channel number.
 *
 * This procedure tunes to the frequency and settings for the specified
 * physical channel according the channel number.
 * When it returns, the signal is reported as either locked (TLG_ERR_SUCCESS)
 * or non-locked (TLG_ERR_FAIL).
 * If the Telegent subsystem is in scan mode, then there may be no output
 * on the video bus after the call to TLGCHN_SetChannel.
 *
 * @param chn   The channel number
 *
 * @retval TLG_ERR_SUCCESS      Call succeeded.
 * @retval TLG_ERR_PARAM        Parameter chn does not exist in the current
 *                              channel map.
 * @retval TLG_ERR_FAIL         Generic error during call or signal not locked.
 *
 ****************************************************************************/
TLGDLL_API tlg_errcode_t TLGCHN_SetChannel(uint8_t chn);

/****************************************************************************
 * TLGCHN_GetChannelList()
 ****************************************************************************/
/** Retreive a list of all channel numbers available in the current channel map.
 *
 * This returns a list of all channel numbers available in the current channel 
 * map.  It also gives the full size resolution of the video standard of
 * the current channel map.
 * If no channel map has been selected, 0 is returned.
 *
 * @param  p_channel_list       Pointer to memory in which channel list will 
 *                              be returned.  The memory should be 256 bytes. 
 * @param  p_resolution         Pointer to default full size resolution of 
 *                              video standard of the current channel map.
 *                              This will be either
 *                              TLGCHN_RESOLUTION_720_480
 *                                        or
 *                              TLGCHN_RESOLUTION_720_576
 *
 * @retval Number of channel numbers returned in the list.
 *
 ****************************************************************************/
TLGDLL_API uint8_t TLGCHN_GetChannelList(uint8_t *p_channel_list, uint8_t *p_resolution);

/****************************************************************************
 * TLGCHN_GetVersion()
 ****************************************************************************/
/** Get the current product version information for the TLGCHN module.
 *
 * This procedure returns the version number for the TLGCHN module.
 * The version is in the form major.minor.patch.
 *
 * @param major             The major version number.
 * @param minor             The minor version number.
 * @param patch             The current patch level.
 * @param build             The current build.
 *
 */
TLGDLL_API void TLGCHN_GetVersion(uint16_t *major, uint16_t *minor, uint16_t *patch, uint16_t *build);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* _TLGCHN_H_ */
