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
#include "global_macros.h"
#include "efuse_drvapi.h"
#include "efuse_phy_uix8910.h"
#include "efuse_ctrl.h"
#include "bsd.h"
#include "xsr_partition.h"
#include "secure.h"
#include "sha1_32.h"
#include "rsa.h"

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

LOCAL BOOLEAN Get_SecurityBondingValue(void)
{
#if 0//for debug,because bondValue does not set by hardware.
	return TRUE;
#else
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	volatile unsigned int bondValue = 0;
	EFUSE_RETURN_E harshReadResult = EFUSE_RESULT_SUCCESS;

	/*Purpose:To get the bonding vaule of security boot*/
	harshReadResult = EFuseRead(EFUSE_BOUND_ID, &bondValue);

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
	REG_READ_ADI(0x82001320,bondValue);//ANA_MISC_STS0
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


/***************************************************************************** 
 *  Description:    this function check if secure boot is enable           * 
 *  Global Resource Dependence:                                              * 
 *  Author: junqiang.wang                                                     * 
 *  return:  TRUE if secure endable ,else return FALSE            * 
******************************************************************************/
#if defined(PLATFORM_UWS6121E)
LOCAL void EFUSE_PHY1_Open(void)
{
	hwp_efuseCtrl->EFUSE_SEC_MAGIC_NUMBER = 0x8910;
	hwp_efuseCtrl->EFUSE_SEC_EN = 0x1;
	hwp_efuseCtrl->EFUSE_PW_SWT = 0x5;
}

LOCAL void EFUSE_PHY1_Close(void)
{
	hwp_efuseCtrl->EFUSE_SEC_MAGIC_NUMBER = 0x8910;
	hwp_efuseCtrl->EFUSE_SEC_EN = 0x0;
	hwp_efuseCtrl->EFUSE_PW_SWT = 0x2;
}

LOCAL uint32 EFUSE_PHY1_Read(int page_index)
{
	uint32 val = readl(REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4) |
			readl(REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4 + 4);
	return val;
}

LOCAL void EFUSE_PHY1_Write(int page_index, uint32 val)
{
	writel(val, REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4);
	writel(val, REG_EFUSE_CTRL_BASE + 0x800 + page_index * 4 + 4);
}

LOCAL BOOLEAN CheckSecureBootEnable()
{
	volatile unsigned int bondValue = 0;
	//EFUSE_PHY1_Open();
	bondValue = EFUSE_PHY1_Read(RDA_EFUSE_SECURITY_CFG_INDEX);
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

	EFuseInitilize();
	isBonding = Get_SecurityBondingValue();
	if(isBonding)
	{
		harshReadResult = EFuseReadHash((unsigned int *)efuse_harsh_data);

		if(EFUSE_RESULT_SUCCESS == harshReadResult)
		{
#if defined(PLATFORM_SC6531EFM)
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
	EFuseClose();

	s_SecureBootEnCheck = TRUE;
	return s_SecureBootEn;
#endif
}
#endif// #if defined(PLATFORM_UWS6121E)

SHA1Context_32 g_s_sha;
/***************************************************************************** 
 *  Description:    this function check the harsh value of efuse and soft    * 
 *  Global Resource Dependence:                                              * 
 *  Author: junqiang.wang                                                    * 
 *  return:  1 if harsh verify ok ,else return 0	            			 * 
******************************************************************************/

LOCAL int HarshVerify(unsigned char *start_data_ptr, uint32 data_len)
{
	uint32 i;
	//SHA1Context_32 sha;
	
	SHA1Reset_32(&g_s_sha);
	SHA1Input_32(&g_s_sha,(const unsigned int *) start_data_ptr, data_len);
	SHA1Result_32(&g_s_sha, (unsigned char *)soft_harsh_data);

	for(i = 0; i < 5; i++)
	{
		soft_harsh_data[i] &= (~BIT_31);
		efuse_harsh_data[i] &= (~BIT_31);
	}

	if((soft_harsh_data[0] == efuse_harsh_data[0]) && (soft_harsh_data[1] == efuse_harsh_data[1]) \
		&& (soft_harsh_data[2] == efuse_harsh_data[2]) && (soft_harsh_data[3] == efuse_harsh_data[3]) \
		&& (soft_harsh_data[4] == efuse_harsh_data[4]))
	{
		return 1;
	}
	
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
		rprintf("bsc->m = %x\n", bsc->e);
		rprintf("bsc->m = %02x %02x %02x %02x\n", bsc->m[0], bsc->m[1], bsc->m[2], bsc->m[3]);
		rprintf("bsc->r2 = %02x %02x %02x %02x\n", bsc->r2[0], bsc->r2[1], bsc->r2[2], bsc->r2[3]);
		rprintf("data start addr is 0x%08x: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
			(uint32)data, data[0], data[1], data[2], data[3]);
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




