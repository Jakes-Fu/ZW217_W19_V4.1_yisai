// ============================================================================
// File Name:       ConPanLayer.h
// Author:          Jianyong Gao
// DATE:            2002-12-18
// Copyright:       2002 Spreadtrum, Incoporated. All Rights Reserved.         
// Description:     This file includes some definitions for ConPanLayer.cpp
// ============================================================================

// ============================================================================
//                        Edit History
// ----------------------------------------------------------------------------
// DATE             NAME                DESCRIPTION
// 2002-12-18       Jianyong Gao        Create.
// ============================================================================

#ifndef _CONPANLAYER_H
#define _CONPANLAYER_H

#include "sci_types.h"
#include "sci_api.h"
#include "scttypes.h"
#include "ctypes.h"
#include "gsm_gprs.h"
#include "cpanel_signal.h"
// ----------------------------------------------------------------------------
//                         Compiler Flag                                     
// ----------------------------------------------------------------------------
#ifdef __cplusplus
    extern   "C"
    {
#endif

int GetStrFromCall(CALLED_NUMBER_T * pcall,	
				unsigned char *pcallnostr,			
				uint8 strlength);

int SetStrToCall(CALLED_NUMBER_T * pcall,
			   unsigned char *pcallnostr);
int SetStrToCalling(CALLING_NUMBER_T * pcall,
			   unsigned char *pcallnostr);

int SetStrToMNCall(MN_CALLED_NUMBER_T *pcall,
			   unsigned char *pcallnostr);
int SetStrToTime(MN_SMS_TIME_STAMP_T * pcall,
				unsigned char *pcallnostr);
int SetStrToMR(MN_SMS_TP_MR_T * pcall,	
				unsigned char *pcallnostr);


#ifdef __cplusplus
    }
#endif

#endif  // End of ConPanLayer.h