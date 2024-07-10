/******************************************************************************
** File Name:     	secure_drv.c                                              *
 ** Author:                                                       			  *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Some functions about secure boot, because Kernel set 	  *	
 **					remap, all api in RomCode for secure boot can't work,     *
 **					so we rework.											  *	
 ******************************************************************************/

#include "NAND_PartitionId.h"
#include "sci_ftl_api.h"
//#include "nfc_drv.h"
#include "sci_api.h"
#include "efuse_drvapi.h"
#include "bsd.h"
#include "xsr_partition.h"
#include "secure.h"
#include "sha1_32.h"
#include "rsa.h"
#include "adi_hal_internal.h"
#include "uws6121e_aes.h"
#include "secure_write_flash.h"

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define EFUSE_BOUND_ID	2
#define SECURE_BOND_EN1	BIT_0
#define SECURE_BOND_EN2	BIT_1
#else
#define SECURE_BOND_EN	BIT_11
#endif

static unsigned char sprd_magic[]	= "SPRD-SECUREFLAG";
static unsigned int efuse_harsh_data[5] = {0}; //to save efuse hash data, set this variable gloable is to for asic simulation check
static unsigned int soft_harsh_data[5] = {0}; // to save soft hash data, set this variable gloable is to for asic simulation check
static BOOLEAN s_SecureBootEnCheck = FALSE;
static BOOLEAN s_SecureBootEn = FALSE;

static SCI_FTL_HANDLE		ftlHandle;
static uint8 pubkey_addr[] =  {
	RDA_PUBKEY0_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY1_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY2_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY3_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY4_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY5_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY6_EFUSE_BLOCK_INDEX,
	RDA_PUBKEY7_EFUSE_BLOCK_INDEX,
};
#define NOR_FLASH_SECTOR_SIZE (1024*64)
//static uint8 temp_buffer[NOR_FLASH_SECTOR_SIZE];

LOCAL BOOLEAN Get_SecurityBondingValue(void)
{
#if 0//for debug,because bondValue does not set by hardware.
	return TRUE;
#else
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	volatile unsigned int bondValue = 0;
	EFUSE_RETURN_E harshReadResult = EFUSE_RESULT_SUCCESS;

	/*Purpose:To get the bonding vaule of security boot*/
	harshReadResult = EFUSE_HAL_Read(EFUSE_BOUND_ID, &bondValue);

	if(EFUSE_RESULT_SUCCESS == harshReadResult)
	{
		if((bondValue & ((unsigned int)SECURE_BOND_EN1)) && (bondValue & ((unsigned int)SECURE_BOND_EN2)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
			
	}
	else
	{
		return FALSE;
	}
#else
	volatile unsigned int bondValue = 0;
	
	/*Purpose:To get the bonding vaule of security boot*/
	bondValue = ANA_REG_GET(0x82001320);//ANA_MISC_STS0
	if(bondValue & ((unsigned int)SECURE_BOND_EN))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#endif
#endif
}

#if defined(PLATFORM_UWS6121E)
//bringup_tmp   secureboot enable bit need to be confirmed
LOCAL BOOLEAN CheckSecureBootEnable(void)
{
	volatile unsigned int bondValue = 0;
	//EFUSE_PHY1_Open();
	bondValue = EFUSE1_HAL_Read(RDA_EFUSE_SECURITY_CFG_INDEX);
	if (((bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT0) &&
	    (bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT1)) ||
	    ((bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT0) &&
	    (bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT2)) ||
	    ((bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT1) &&
	    (bondValue & RDA_SE_CFG_SECURITY_ENABLE_BIT2)))
			s_SecureBootEn = TRUE;
	else
			s_SecureBootEn = FALSE;
	//EFUSE_PHY1_Close();
	return s_SecureBootEn;
}
#else

/***************************************************************************** 
 *  Description:    this function check if secure boot is enable           * 
 *  Global Resource Dependence:                                              * 
 *  Author: junqiang.wang                                                     * 
 *  return:  TRUE if secure endable ,else return FALSE            * 
******************************************************************************/
LOCAL BOOLEAN CheckSecureBootEnable()
{
	EFUSE_RETURN_E harshReadResult = EFUSE_RESULT_SUCCESS;
	BOOLEAN isBonding;
#ifdef SECURE_BOOT_DEBUG
    	return 1;// for debug 
#else
	if(TRUE == s_SecureBootEnCheck)
	{
		return s_SecureBootEn;
	}

    EFUSE_HAL_Open();
	isBonding = Get_SecurityBondingValue();
	if(isBonding)
	{
		harshReadResult = EFUSE_HAL_ReadHash((unsigned int *)efuse_harsh_data);

		if(EFUSE_RESULT_SUCCESS == harshReadResult)
		{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
			if(efuse_harsh_data[0] 
				& efuse_harsh_data[1] 
				& efuse_harsh_data[2] 
				& efuse_harsh_data[3] 
				& efuse_harsh_data[4] 
				& (unsigned int)BIT_31)
#else
			if(efuse_harsh_data[0] & (unsigned int)BIT_31)
#endif
			{
				s_SecureBootEn = TRUE;
			}
			else
			{
				s_SecureBootEn = FALSE;
			}
				
		}
		else
		{
			s_SecureBootEn = FALSE;
		}
	}
	else
	{
		s_SecureBootEn = FALSE;
	}
	EFUSE_HAL_Close();

	s_SecureBootEnCheck = TRUE;
	return s_SecureBootEn;
#endif
}
#endif //#if defined(PLATFORM_UWS6121E)


/***************************************************************************** 
 *  Description:    this function check the harsh value of efuse and soft    * 
 *  Global Resource Dependence:                                              * 
 *  Author: junqiang.wang                                                    * 
 *  return:  1 if harsh verify ok ,else return 0	            			 * 
******************************************************************************/

LOCAL int HarshVerify(unsigned char *start_data_ptr, uint32 data_len)
{
	uint32 i;
	SHA1Context_32 sha;
	
	SHA1Reset_32(&sha);
    	SHA1Input_32(&sha,(const unsigned int *) start_data_ptr, data_len);
    	SHA1Result_32(&sha, (unsigned char *)soft_harsh_data);

	for(i = 0; i < 5; i++)
	{
		soft_harsh_data[i] &= (~BIT_31);
		efuse_harsh_data[i] &= (~BIT_31);
	}

	if((soft_harsh_data[0] == efuse_harsh_data[0]) && (soft_harsh_data[1] == efuse_harsh_data[1]) \
		&& (soft_harsh_data[2] == efuse_harsh_data[2]) && (soft_harsh_data[3] == efuse_harsh_data[3]) \
		&& (soft_harsh_data[4] == efuse_harsh_data[4]))
	{
		//SCI_TRACE_LOW("TRACE_HASH_SUCESS");
		return 1;
	}

	//SCI_TRACE_LOW("TRACE_HASH_FAIL");
	
	return 0;
}
	
//if remap enable in bootloader, some function address will be changed.
#if 0
CONST uint32 __harsh_func_hack[] = {//see romcode mapfile
	0x20c0b000,     //g_TraceValue
	0x20c0b988,     //efuse_harsh_data
	0x20c0b99c,     //soft_harsh_data
	0xffff1114 + 1, //CheckSecureBootEnable (thumb mode)
	0xffff11b0 + 1,	//HarshVerify   (thumb mode)
	0xffff2874,     //RSA_ModPower    
};
#endif
#if 1
PUBLIC CONST uint32 __harsh_func_hack[] = {//see romcode mapfile
	0,     					//g_TraceValue
	(uint32)efuse_harsh_data,     	//efuse_harsh_data
	(uint32)soft_harsh_data,     	//soft_harsh_data
	(uint32)CheckSecureBootEnable, 	//CheckSecureBootEnable (thumb mode)
	(uint32)HarshVerify,			//HarshVerify   (thumb mode)
	(uint32)RSA_ModPower,      		//RSA_ModPower    
};
#endif

PUBLIC uint32* get_harsh_func()
{
	return (uint32*)&(__harsh_func_hack[0]);
}

PUBLIC BOOLEAN CheckSecureBootState(void)
{
#ifdef SECURE_BOOT_DEBUG
    return TRUE;	//for Debug
#else    
    harsh_func_t *harsh = (harsh_func_t *)__harsh_func_hack;
    return harsh->CheckSecureBootEnable();
#endif
}

#define MAKE_DWORD(a,b,c,d) (unsigned int)((((unsigned int)(a))<<24) | (((unsigned int)(b)) <<16) | (((unsigned int)(c))<<8) | ((unsigned int)(d)));
void RSA_Decrypt(unsigned char * p, unsigned char *m, unsigned char *r2, unsigned char * e)
{
	unsigned int _e = 0;
	unsigned int _m[32]  = {0};
	unsigned int _p[32]  = {0};
	unsigned int _r2[32] = {0};
	int i = 0;
	harsh_func_t *harsh = (harsh_func_t *)__harsh_func_hack;

	_e = MAKE_DWORD(e[0],e[1],e[2],e[3]);

	for(i=31;i>=0;i--)
	{
		_m[31-i] = MAKE_DWORD(m[4*i],m[4*i+1],m[4*i+2],m[4*i+3]);
		_p[31-i] = MAKE_DWORD(p[4*i],p[4*i+1],p[4*i+2],p[4*i+3]);
		_r2[31-i] = MAKE_DWORD(r2[4*i],r2[4*i+1],r2[4*i+2],r2[4*i+3]);
	}

	harsh->RSA_ModPower(_p, _m, _r2, _e);

	for(i=31;i>=0;i--)
	{
		p[4*(31-i)] = (unsigned char)(_p[i]>>24);
		p[4*(31-i)+1] = (unsigned char)(_p[i]>>16);
		p[4*(31-i)+2] = (unsigned char)(_p[i]>>8);
		p[4*(31-i)+3] = (unsigned char)(_p[i]);
	}
}

int long_jump(uint8 *, int);

/******************************************************************************/
//  Description:   Set Read command into Nand controller
//  Parameter:
//      data		:hash resource data
//      data_len	:hash resource length
//      data_hash	:verify hash value, commonly, it is encrypted with rsa and store in VLR
//      data_key	:rsa public key info
//  Return:
//           1     hash verify ok
//           0     hash verify fail
/******************************************************************************/
PUBLIC int harshVerify(uint8 *data, uint32 data_len, uint8 *data_hash, uint8 *data_key)
{
	int ret = 1;//ok
	harsh_func_t *harsh = (harsh_func_t *)__harsh_func_hack;
#ifdef SECURE_BOOT_DEBUG//BUGBUG: for DEBUG enable secureboot
	*(BOOLEAN *)0x20c0b981/*s_SecureBootEn*/ = 1;
#endif

	if (!harsh->CheckSecureBootEnable()) return ret;

	if (!data_hash && !data_key) return harsh->HarshVerify(data, data_len >> 2);
	else {
		puk_info_t *bsc = (puk_info_t *)data_key;//load public key from bsc
		if (data_len) memcpy(harsh->efuse_harsh_data, data_hash, SHA1NUM_SIZ);
		else {//load hash length from vlr
			vlr_info_t *vlr = (vlr_info_t *)data_hash;
			if (vlr->magic != VLR_MAGIC) return 0;//invalid vlr
			data_len = vlr->length;

			RSA_Decrypt(vlr->hash, bsc->m, bsc->r2,(unsigned char *)&bsc->e);
			memcpy(harsh->efuse_harsh_data, &vlr->hash[sizeof(vlr->hash) - SHA1NUM_SIZ], SHA1NUM_SIZ);
		}
		ret = harsh->HarshVerify(data, data_len >> 2);//dword align
	}
	return ret;
}

PUBLIC uint32 Write_Public_Key(void)
{
#define PUBLICWORDS 8
#define FDL_SIGNED_SIZE 	(0xbce0)
#define PUBLICBYTES (32)
//#define SECURE_DEBUG
	int i;
	uint32 keys[PUBLICWORDS];
	uint32 start_addr = FLASH_GetStartAddr();
	uint8 *pubkey;
	#ifdef SECURE_DEBUG
	unsigned char data[] = {0xb9, 0x57, 0x8c, 0xc9,
				0x5e, 0x1a, 0xc3, 0xc8,
				0x95, 0xd6, 0x1f, 0x64,
				0xa2, 0xd0, 0x10, 0xcb,
				0x7e, 0x5a, 0x66, 0x4b,
				0xf3, 0xd5, 0xbe, 0x57,
				0x61, 0xf6, 0xc9, 0xf9,
				0x2c, 0x6a, 0xa9, 0x95,	
				};
	pubkey = &data[0];
	#endif
    if (CheckSecureBootEnable()) {
        return 1;
    }
	pubkey = (uint8 *)(FLASH_GetStartAddr() + FDL_SIGNED_SIZE - PUBLICBYTES);
	for (i = 0; i < PUBLICWORDS; i++)
	{
		keys[i] = *(pubkey + i*4) << 24 |
				*(pubkey + i*4 + 1) << 16 |
				*(pubkey + i*4 + 2) << 8 |
				*(pubkey + i*4 + 3);
		EFUSE1_HAL_Write(pubkey_addr[i], keys[i]);
	}
	return 0;
}


PUBLIC uint8 Security_Enable(void)
{
	uint32 val;
    if (CheckSecureBootEnable()) {
        return 1;
    }
	val = RDA_SE_CFG_SECURITY_ENABLE_BIT0 | RDA_SE_CFG_SECURITY_ENABLE_BIT1
		| RDA_SE_CFG_SECURITY_ENABLE_BIT2;
	EFUSE1_HAL_Write(RDA_EFUSE_SECURITY_CFG_INDEX, val);
	return 0;
}


PUBLIC uint32 Anti_Clone_Enable()
{
	int i;
	uint32 start_addr = FLASH_GetStartAddr();
    unsigned char * temp_buffer = NULL;
	if (!CheckSecureBootEnable()) {
        temp_buffer = (unsigned char *)SCI_ALLOC_APP(NOR_FLASH_SECTOR_SIZE); 
	    memcpy(temp_buffer, start_addr, NOR_FLASH_SECTOR_SIZE);
	    rda_encrypt_obj((unsigned char *)temp_buffer + ENCRYPT_OFF, ENCRYPT_LEN);
        SecureBootWriteFlash(start_addr, temp_buffer);
        SCI_FREE(temp_buffer);
	} else {
        return 1;
    }
	return 0;
}


PUBLIC void SecureMode_WCNuartEnable(void)
{
    if (!CheckSecureBootEnable()) {
        return ;
    }

	EFUSE1_HAL_enWCNuart();
	return;
}


