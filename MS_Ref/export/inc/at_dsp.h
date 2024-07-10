/******************************************************************************
 ** File Name:      at_dsp.h                                                  *
 ** Author:         Jenny.xu                                                  *
 ** DATE:           03/05/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/05/2002     Jenny.xu          Create.                                  *
 ** 19/08/2002     Xueliang.Wang     Modify.                                  *
 ******************************************************************************/

#ifndef _AT_DSP_H
#define _AT_DSP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "at_common.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Handle all DSP command. If success, make a response message
//                  by dest_ptr, else don't make response message.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC	uint32 ATDSP_HandleDsp( // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    );
	
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End at_dsp.h

