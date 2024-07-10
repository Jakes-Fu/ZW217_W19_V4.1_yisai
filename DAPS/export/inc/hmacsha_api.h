/*****************************************************************************
** File Name:      rsa.h                                                *
** Author:         xianhe.yang                                                *
** Date:           2015.6                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2015.6     xianhe.yang         Create                                 *
******************************************************************************/

#ifndef _HMACSHA_INTERFACE_H_
#define _HMACSHA_INTERFACE_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
    /**-------------------------------------------------------------------------*
    **                         STRUCTURE DEFINITION                             *
    **--------------------------------------------------------------------------*/
    

    /**-------------------------------------------------------------------------*
    **                          FUNCTION DECLARE                           *
    **--------------------------------------------------------------------------*/
    /*****************************************************************************/
    // 	Description : RSA public encryption.
    //	Global resource dependence :
    //  Author: xianhe.yang
    //	Note: The outlen param must be set to the strength of the key:  key->size
    //		  if success return key->size,else return < 0
    /*****************************************************************************/

#ifdef USE_SHA1
/******************************************************************************/
/*
	HMAC-SHA1
	http://www.faqs.org/rfcs/rfc2104.html
*/

PUBLIC int32 HmacSha1(unsigned char *key, uint32 keyLen, const unsigned char *buf,
				uint32 len, unsigned char *hash, unsigned char *hmacKey,
				uint32 *hmacKeyLen);

#endif

#ifdef USE_SHA256
/******************************************************************************/
/*
	HMAC-SHA256
*/

PUBLIC int32 HmacSha2(unsigned char *key, uint32 keyLen, const unsigned char *buf,
				uint32 len, unsigned char *hash, unsigned char *hmacKey,
				uint32 *hmacKeyLen, uint32 hashSize);
#endif
    
#endif