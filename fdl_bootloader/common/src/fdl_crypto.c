#include "string.h"
#include "sci_types.h"
#include "fdl_stdio.h"
#include "fdl_crc.h"
#include "md5.h"
#include "rsa.h"
#include "packet.h"
#define RSA_DATA_LEN 128
static uint8 __align (4) rsaValueSaved[RSA_DATA_LEN];
typedef struct {
	uint32 e;
	uint32 r[32];	
} RSA_Context;
static RSA_Context rsa_context;
typedef struct
{
	uint32 imgSize; //image size
	uint32 rcvdSize; //received size
	uint32 rsaRcvSize; //rsa value received size
} CRYPTO_FILE_STATUS;

static CRYPTO_FILE_STATUS sCryptoStatus;

int FDL_CryptoInit (uint32 size, uint8 * key, uint32 e)
{
	md5_init();
	memset(&rsa_context, 0, sizeof(rsa_context));
	rsa_context.e = e;
	memcpy(rsa_context.r, key, RSA_DATA_LEN);
	memset(&sCryptoStatus, 0, sizeof(sCryptoStatus));
	sCryptoStatus.imgSize = size;
	sCryptoStatus.rcvdSize = 0;
	sCryptoStatus.rsaRcvSize = 0;
}
/******************************************************************************/
//  Description:   update the crypto 
//  Parameter:
//      src: the data received from tool
//		len: received len
//  Return:
//		-1: fail 
//		other: the valid data size
/******************************************************************************/
int FDL_CryptoUpdate(uint8 *src, uint32 len)
{
	int rsaSize = 0;
	int dataSize;
	uint8 *rsaP = 0;
	//received is only rsa data
	if(sCryptoStatus.rsaRcvSize > 0)
	{
		if((sCryptoStatus.rcvdSize + len) > RSA_DATA_LEN)
		{
			return -1;
		}
		memcpy(rsaValueSaved + sCryptoStatus.rsaRcvSize, src, len);
		sCryptoStatus.rsaRcvSize += len;
		return 0;
	}
	//recevied rsa data and image data
	else if((len + sCryptoStatus.rcvdSize) >= (sCryptoStatus.imgSize - RSA_DATA_LEN))
	{
		rsaSize = (len + sCryptoStatus.rcvdSize) - (sCryptoStatus.imgSize - RSA_DATA_LEN);
		rsaP = src + len - rsaSize;		
		if(rsaSize > RSA_DATA_LEN)
		{
			return -1;
		}
		memcpy(rsaValueSaved, rsaP, rsaSize);
		sCryptoStatus.rsaRcvSize += rsaSize;
	}
	dataSize = len - rsaSize;
	md5_update(src, dataSize);
	sCryptoStatus.rcvdSize += len;
	return dataSize;
}
/******************************************************************************/
//  Description:   check the data validility
//  Parameter:
//      len:
//		data:
//  Return:
//		FALSE: Check fail
//		TRUE: Check sucessful
/******************************************************************************/
BOOLEAN FDL_CryptoCheck()
{
	uint8 md5Value[16];
	uint32 md5_pointer;	
	uint32 i;
	
	md5_final(md5Value);
	
	for(i = 0; i < 16; i++)
	{
		if((*(rsaValueSaved + 112 + i)) != md5Value[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}
