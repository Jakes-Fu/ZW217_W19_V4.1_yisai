/** File Name:      simlock_aescrpt.c                                         *
 ** Author:                                                       *
 ** Date:               
 *  aes-ecb encryption program
 *
 ** Copyright       2001-2015 by Spreadtrum PLD_CDRV_MODEM,Inc.               *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.
 *** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 02/04/2016    gloria.he      Create.                                   *
 **                                   *
 ******************************************************************************//*aescrpt*/
#include "aes.h"
#include "sci_types.h"
#include "os_api.h"
/**---------------------------------------------------------------------------*
 **                         macro define                                 *
 **---------------------------------------------------------------------------*/
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

/**---------------------------------------------------------------------------*
 **                         function Declaration  
 
/*****************************************************************************/
//  Description : do aes_ecb Encrypted algrithm 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
BOOLEAN simlock_aes_encrypt(unsigned char *input, unsigned short input_len, unsigned char *output, unsigned char *inkey,unsigned short key_len)
{
	int i,n;
	int offset;
	unsigned char buffer[16]={0};
	unsigned char key[32]={0};
	aes_context aes_ctx={0};
	

	SCI_ASSERT(input != NULL && output != NULL && inkey != NULL);

	if((input_len&0x0F)!=0)
	{
		SCI_TraceLow("SPRD_AES_Enc():input size=%d not a multiple of 16",input_len);
		return FALSE;
	}
	aes_setkey_enc( &aes_ctx, inkey, key_len<<3);	
	
	for( offset = 0; offset < input_len; offset += 16 )
	{
		memcpy(buffer,&input[offset],16);	
		aes_crypt_ecb( &aes_ctx, AES_ENCRYPT, buffer, buffer );
		memcpy(&output[offset],buffer,16);
	}
	return TRUE;
}


/*****************************************************************************/
//  Description : do aes_ecb Decrypted algrithm 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
BOOLEAN simlock_aes_decrypt(unsigned char *input, unsigned short input_len, unsigned char *output, unsigned char *inkey,unsigned short key_len)
{
	int i,n;
	int offset;
	unsigned char buffer[16]={0};
	unsigned char key[32]={0};
	aes_context aes_ctx={0};

	SCI_ASSERT(input != NULL && output != NULL && inkey != NULL);
	if((input_len&0x0F)!=0)
	{
		SCI_TraceLow("SPRD_AES_Enc():input size=%d not a multiple of 16,",input_len);
		return FALSE;
	}
	
	aes_setkey_dec( &aes_ctx, inkey, key_len<<3 );
	
	for( offset = 0; offset < input_len; offset += 16 )
	{

		memcpy(buffer,&input[offset],16);
		aes_crypt_ecb( &aes_ctx, AES_DECRYPT, buffer, buffer );
		memcpy(&output[offset],buffer,16);
	}
	return TRUE;
}
