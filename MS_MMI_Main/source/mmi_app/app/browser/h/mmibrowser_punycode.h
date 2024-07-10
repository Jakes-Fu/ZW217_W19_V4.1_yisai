/*****************************************************************************
** File Name:      mmibrowser_punycode.h                                     *
** Author:         Jiaoyou.wu                                                  *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#ifndef BROWSER_PUNYCODE_H
#define BROWSER_PUNYCODE_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "limits.h"

typedef enum{
    MMI_PUNYCODE_SUCCESS,
    MMI_PUNYCODE_BAD_INPUT,
    MMI_PUNYCODE_BIG_OUTPUT,
    MMI_PUNYCODE_OVERFLOW,
    MMI_PUNYCODE_MAX,

}MMI_PUNYCODE_E;

typedef uint32 punycode_uint;

LOCAL MMI_PUNYCODE_E punycode_encode(uint32 input_length, const uint32 input[], const unsigned char case_flags[], uint32 *output_length, char output[]);

LOCAL MMI_PUNYCODE_E punycode_decode(uint32 input_length, const char input[], uint32 *output_length, uint32 output[], unsigned char case_flags[]);

PUBLIC MMIBROWSER_Unicode_punycode_encode(uint16 *input, uint16 input_len, int32 *output_len, char *output);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
