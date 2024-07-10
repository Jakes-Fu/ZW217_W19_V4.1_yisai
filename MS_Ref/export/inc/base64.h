/******************************************************************************
** File Name:      mbbms_base64.h                                            *
** Author:                                                                    *
** Date:           2008/9/24                                                  *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    header file of MD5 encode/decode and base64 encode/decode    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 2008/9        Hong.Yu           Create                                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 
#ifndef __BASE64_HEADER__
#define __BASE64_HEADER__

#include "sci_types.h"

#define XYSSL_ERR_BASE64_BUFFER_TOO_SMALL -0x0010
#define XYSSL_ERR_BASE64_INVALID_CHARACTER -0x0012

#ifdef __cplusplus
extern   "C"
{
#endif


/**
* \brief Encode a buffer into base64 format
*
* \param dst destination buffer
* \param dlen size of the buffer
* \param src source buffer
* \param slen amount of data to be encoded
*
* \return 0 if successful, or XYSSL_ERR_BASE64_BUFFER_TOO_SMALL.
* *dlen is always updated to reflect the amount
* of data that has (or would have) been written.
*
* \note Call this function with *dlen = 0 to obtain the
* required buffer size in *dlen
*/

PUBLIC int base64_encode( unsigned char *dst, int *dlen, unsigned char *src, int slen );

/**
* \brief Decode a base64-formatted buffer
*
* \param dst destination buffer
* \param dlen size of the buffer
* \param src source buffer
* \param slen amount of data to be decoded
*
* \return 0 if successful, XYSSL_ERR_BASE64_BUFFER_TOO_SMALL, or
* XYSSL_ERR_BASE64_INVALID_DATA if the input data is not
* correct. *dlen is always updated to reflect the amount
* of data that has (or would have) been written.
*
* \note Call this function with *dlen = 0 to obtain the
* required buffer size in *dlen
*/
PUBLIC int base64_decode( unsigned char *dst, int *dlen, unsigned char *src, int slen );

#ifdef __cplusplus
}
#endif
#endif

