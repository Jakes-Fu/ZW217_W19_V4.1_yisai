/******************************************************************************
 ** File Name:      security_data_protect.c                                                    *
 ** Author:                                                       *
 ** DATE:                                                           *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 bsc data protection. It manages important data verify,        *
 **			 write and read.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             	DESCRIPTION                               *
 **										                                     *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "security_data_protect.h"
#include "sci_types.h"
#include "os_api.h"
#include "nvitem.h"
#include "bsc_debug.h"
#include "sha1_32.h"
#include "efuse_drvapi.h"
#include "mn_type.h"        /* MN_MAX_IMEI_LENGTH     */

#ifdef SECURE_BOOT_SUPPORT
#include "secure.h"
#endif

//#define BT_ADDERSS_PROTECT_SUPPORT
//#define RF_PARAMETER_PROTECT_SUPPORT
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#pragma pack(1)
#endif

#ifdef BT_ADDERSS_PROTECT_SUPPORT
#define BT_ADDRESS_SIZE	(6)
 typedef PACK struct _BT_ADDRESS 
 {
	uint8 addr[BT_ADDRESS_SIZE];
} BT_ADDRESS;
#endif

#define IMEI_SIZE              (8)
 typedef PACK struct _IMEI_ADDRESS
 {
	uint8 addr[IMEI_SIZE];
} IMEI_ADDRESS;

typedef PACK struct _SECURITY_DATA
{
	IMEI_ADDRESS imei[4];
#ifdef BT_ADDERSS_PROTECT_SUPPORT
	BT_ADDRESS bt_addr;
#endif
#ifdef RF_PARAMETER_PROTECT_SUPPORT
	uint32 harsh_data[5];
#endif
}SECURITY_DATA;

#ifdef WIN32
#pragma pack()
#endif
/******************************************************************************
 * Static Global Variables
 ******************************************************************************/
SECURITY_DATA security_data;

#define BSC_OPER 5

const uint16 protect_nv_id[]=
{
//IMEI
    (uint16)NV_IMEI,
    (uint16)NV_IMEI1,
    (uint16)NV_IMEI2,
    (uint16)NV_IMEI3,

#ifdef BT_ADDERSS_PROTECT_SUPPORT
//BT
    (uint16)NV_BT_CONFIG,
#endif

#ifdef RF_PARAMETER_PROTECT_SUPPORT
//WCDMA
    (uint16)NV_WCDMA_RFGPIO_CONFIG//not used  
#endif
};

#define PROTECT_SECURITYDATAID_SIZE (sizeof(protect_nv_id)/sizeof(uint16))
#define EFUSE_JTAG_SET_ID	15
#define JTAG_DISABLE	BIT_0
#define EFUSE_BOUND_ID	2
#define SECURE_BOND_EN1	BIT_0
#define SECURE_BOND_EN2	BIT_1

#ifdef RF_PARAMETER_PROTECT_SUPPORT
#define NV_DATA_SIZE	256
LOCAL uint8 Nv_Data_Buff[NV_DATA_SIZE] = {0};	
#endif
LOCAL uint8 s_protection_result = ERR_NONE;

typedef enum{
    SECURE_BOOT_ENABLE,
    SECURE_BOOT_DISABLE,
}SECURE_FLAG_T;

PUBLIC uint32   g_hashvalue[5] = {0};
static SECURE_FLAG_T   g_secure_flag = SECURE_BOOT_DISABLE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN Security_Data_Read_From_NV();
LOCAL uint8 Security_Data_Compare();
#ifdef RF_PARAMETER_PROTECT_SUPPORT
LOCAL BOOLEAN Security_Data_HarshVerify (uint32 *security_harsh_data,uint32 security_data_len);
#endif
LOCAL  void    SecurityDataCOMMON_GenerateRspMsg(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MSG_SECURITYDATA_RES_T *bsc_res_ptr);   // Inputed pointer of message header struct.
LOCAL BOOLEAN Security_Data_Read_Boot(uint8 *pBuf,uint16 cchSize);
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
// Description :    Security_Data_Init.
// Global resource dependence : 
// Author :         Yinhui.Huang
// Note :
/*****************************************************************************/
PUBLIC void Security_Data_Init(void)
{
	BSC_ERROR_E ret;
	uint8 err = ERR_NONE;

	BSC_LOG_TRACE(("[BSC:Security_Data_Init] start\r\n"));

	ret = BSC_Init();
	
	if(BSCERR_NONE != ret)
		 SCI_ASSERT( 0 );

#ifdef SECURE_BOOT_SUPPORT
	if (!CheckSecureBootState())
		return;
#else
	return;
#endif

	if(!Security_Data_Read_From_NV())
		SCI_ASSERT( 0 );

	err = Security_Data_Compare();

	if(ERR_BSC == err)
		SCI_ASSERT( 0 );
	else
        s_protection_result = err;

}

 /*****************************************************************************/
// Description :    Security Data Read From NV.
// Local resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN Security_Data_Read_From_NV()
{
	uint16 len       = 0;
	uint16 i = 0;
	uint16 id = 0;
	NVITEM_ERROR_E  status = NVERR_NONE;
	uint32 sum_len = 0;
	uint8 *bt_config;

#ifdef RF_PARAMETER_PROTECT_SUPPORT
	memset(Nv_Data_Buff,0,NV_DATA_SIZE);
#endif
	memset(&security_data,0,sizeof(SECURITY_DATA));

	for(i = 0;i < PROTECT_SECURITYDATAID_SIZE;i++)
	{
		id = protect_nv_id[i];
		len = MN_MAX_IMEI_LENGTH;//(uint16)NVITEM_GetLength(id);
		if(i < 4)
		{
			status = EFS_NvitemRead(id, (uint16)len,(uint8 *)(security_data.imei[i].addr));
		}
#ifdef BT_ADDERSS_PROTECT_SUPPORT
		else if(NV_BT_CONFIG == id)
		{
			bt_config = SCI_ALLOC_APP(len);
			if (NULL == bt_config)
			{
				break;
			}
			status = EFS_NvitemRead(id, (uint16)len,(uint8 *)(bt_config));

			len = sizeof(BT_ADDRESS);
			SCI_MEMCPY((uint8*)(&security_data.bt_addr), bt_config, len);

			if (NULL != bt_config)
			{
				SCI_FREE(bt_config);
				bt_config = NULL;
			}
		}
#endif
#ifdef RF_PARAMETER_PROTECT_SUPPORT
		else
		{
			status = EFS_NvitemRead(id, (uint16)len,(uint8 *)(Nv_Data_Buff + sum_len));
			sum_len = sum_len + len;
		}
#endif
		if (NVERR_NONE != status)
		{
			BSC_ERR_TRACE(("[BSC:Security_Data_Read_From_NV] Read id=%d Error\r\n",id));
			break;
		}

	}

	if(PROTECT_SECURITYDATAID_SIZE != i)
		return FALSE;

	return TRUE;
}

 /*****************************************************************************/
// Description :    Security Data Compare.
// Local resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL uint8 Security_Data_Compare()
{
	BSC_ERROR_E ret;
	unsigned int security_harsh_data[5] = {0};
	BOOLEAN err = FALSE;
	uint32 security_data_len;
	uint32 i = 0;
	SECURITY_DATA s_data;

	security_data_len = sizeof(SECURITY_DATA);
	ret = BSC_Read(BSC_CUSTOM_DATA, sizeof(SECURITY_DATA), &s_data);

	if(ret == BSCERR_NONE)
	{
		for(i = 0;i < 4;i++)
		{
			if(0 != memcmp((uint8 *)s_data.imei[i].addr,(uint8 *)security_data.imei[i].addr,IMEI_SIZE))
				return ERR_IMEI_ADDRESS;
		}

#ifdef BT_ADDERSS_PROTECT_SUPPORT
		if(0 != memcmp((uint8 *)s_data.bt_addr.addr,(uint8 *)security_data.bt_addr.addr,BT_ADDRESS_SIZE))
				return ERR_BT_ADDRESS;
#endif
#ifdef RF_PARAMETER_PROTECT_SUPPORT
		for(i = 0; i < 5;i++)
		{
			security_harsh_data[i] = s_data.harsh_data[i];
		}

		err = Security_Data_HarshVerify(security_harsh_data,sizeof(security_harsh_data));
		if(!err)
			return ERR_RF_PARAMETER;
#endif
	}
	else
	{
		return ERR_BSC;
	}

	return ERR_NONE;
}

 /*****************************************************************************/
// Description :    Write Security Data to Bsc.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN Security_Data_Write()
{
	BSC_ERROR_E ret;
	SHA1Context_32 sha;
	BOOLEAN err = FALSE;

#ifdef SECURE_BOOT_SUPPORT
	if (CheckSecureBootState())
		return FALSE;
#endif

	if(!Security_Data_Read_From_NV())
		return FALSE;
	
#ifdef RF_PARAMETER_PROTECT_SUPPORT
	SHA1Reset_32(&sha);
	SHA1Input_32(&sha,(const unsigned char *) Nv_Data_Buff, NV_DATA_SIZE);
	SHA1Result_32(&sha, (unsigned char *)security_data.harsh_data);
#endif

	ret = BSC_Write(BSC_CUSTOM_DATA, sizeof(SECURITY_DATA), &security_data);
	SCI_PASSERT (ret == BSCERR_NONE, ("Security_Data_Write:: %d", ret));/*assert verified*/
	if(ret == BSCERR_NONE)
		err = TRUE;
	else
		err = FALSE;

	return err;
}

 /*****************************************************************************/
// Description :    Get security Data Protection Result.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint8 Security_Data_Protection_Result()
{
	BSC_LOG_TRACE(("[BSC:Security_Data_Protection_Result] g_call_mode = %d\r\n",s_protection_result));

	return s_protection_result;
}

#ifdef RF_PARAMETER_PROTECT_SUPPORT
 /*****************************************************************************/
// Description :    Security Data HarshVerify.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN Security_Data_HarshVerify (uint32 *security_harsh_data,uint32 security_data_len)
  {
	uint32 i;
	SHA1Context_32 sha;


	SHA1Reset_32(&sha);
	SHA1Input_32(&sha,(const unsigned char *) Nv_Data_Buff, NV_DATA_SIZE);
	SHA1Result_32(&sha, (unsigned char *)security_data.harsh_data);

	for (i = 0; i < 5; i++)
	{
		security_data.harsh_data[i] &= (~BIT_31);
		security_harsh_data[i] &= (~BIT_31);
	}

	if ((security_data.harsh_data[0] == security_harsh_data[0]) && (security_data.harsh_data[1] == security_harsh_data[1])
		&& (security_data.harsh_data[2] == security_harsh_data[2]) && (security_data.harsh_data[3] == security_harsh_data[3])
		&& (security_data.harsh_data[4] == security_harsh_data[4]))
	{
		//verify success.
		BSC_LOG_TRACE(("[BSC:Security_Data_HarshVerify] Harsh Verify Ok!\r\n"));
		return TRUE;
	}
	//harsh verify fail!
	BSC_LOG_TRACE(("[BSC:Security_Data_HarshVerify] Harsh Verify Failed!\r\n"));

	return FALSE;
}
#endif

 /*****************************************************************************/
// Description :    Read Boot0 Data from bsc.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN Security_Data_Read_Boot(uint8 *pBuf,uint16 cchSize)
{
    BSC_ERROR_E ret;

    ret = BSC_Read(BSC_BOOT_DATA, cchSize, pBuf);

    if(BSCERR_NONE == ret)
        return TRUE;

    return FALSE;
}

/*****************************************************************************/
/*  Description :the  security data diag handle including the  security data write and selftest
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
 uint32 HandleSecurityDataWriteReq(const uint8  *src_ptr,uint16  src_len)
{
	MSG_HEAD_T *msg = (MSG_HEAD_T *)src_ptr;
	MSG_HEAD_T *msg_response;
	BOOLEAN err_w = FALSE;
	BOOLEAN err_r = FALSE;
#ifdef SECURE_BOOT_SUPPORT
	BOOLEAN err_effuse = FALSE;
#endif
	ERR_STATUS err_status;
	uint16 stage = 0;
	uint8 result_status;

	BSC_LOG_TRACE(("[HandleSecurityDataWriteReq] (type = %d,subtype = %d)",msg->type, msg->subtype));
   
	msg_response = (MSG_HEAD_T *)SCI_ALLOC(1 + sizeof(MSG_HEAD_T));
	msg_response->seq_num   = msg->seq_num;
	msg_response->type 		= msg->type;
	msg_response->subtype	= msg->subtype;
	msg_response->len		= sizeof(MSG_HEAD_T) + 1;	

	err_w = Security_Data_Write();

	if(err_w)
	{
		stage = 1;
		if(Security_Data_Read_From_NV())
		{
			err_status = Security_Data_Compare();
		}
		else
		{
			err_status = ERR_BSC;
		}

		if(ERR_NONE == err_status)
		{
#ifndef SECURE_BOOT_SUPPORT
			result_status = 1;
#endif
			err_r = TRUE;
#ifdef SECURE_BOOT_SUPPORT	
		stage = 2;
		if(!WriteBscHashValue())
		{
			result_status = 1;
			err_effuse = TRUE;
		}
		else
		{
			result_status = 0;
			err_effuse = FALSE;
		}
#endif
		}
		else
		{
			result_status = 0;
			err_r = FALSE;
		}
	}
	else
	{
		result_status = 0;
	}

#ifdef SECURE_BOOT_SUPPORT
	BSC_LOG_TRACE(("[BSC:HandleSecurityDataWriteReq] (err_w = %d,err_status = %d,err_r = %d,err_effuse = %d,result_status = %d)",err_w,err_status,err_r,err_effuse,result_status));
#else
	BSC_LOG_TRACE(("[BSC:HandleSecurityDataWriteReq] (err_w = %d,err_status = %d,err_r = %d,result_status = %d)",err_w,err_status,err_r,result_status));
#endif

	SCI_MEMCPY((void *)(msg_response + 1), (void *)&result_status, 1);
	SIO_SendPacket(msg_response, 1 + sizeof(MSG_HEAD_T));
	SCI_FREE(msg_response);

	if(stage == 0)
		return err_w;
	if(stage == 1)
		return err_r;
#ifdef SECURE_BOOT_SUPPORT
	if(stage == 2)
		return err_effuse;
#endif
}

/*****************************************************************************/
/*  Description :the  security data write and check including the security data write and selftest
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN Security_Data_WriteAndSelftest()
{
	BOOLEAN err_w = FALSE;
	BOOLEAN err_r = FALSE;
	ERR_STATUS err_status;
	BOOLEAN flag = FALSE;

	BSC_LOG_TRACE(("[BSC:Security_Data_WriteAndSelftest] start\r\n"));

	err_w = Security_Data_Write();

	if(err_w)
	{
		flag = TRUE;
		if(Security_Data_Read_From_NV())
		{
			err_status = Security_Data_Compare();
		}
		else
		{
			err_status = ERR_BSC;
		}

		if(ERR_NONE == err_status)
		{
			err_r = TRUE;
		}
		else
		{
			err_r = FALSE;
		}
	}

	BSC_LOG_TRACE(("[BSC:Security_Data_WriteAndSelftest] (err_w = %d,err_status = %d,err_r = %d)",err_w,err_status,err_r));

	if( flag == TRUE)
	{
		return err_r;
	}
	else
	{
		return err_w;
	}
}
/******************************************************************************
 * [Description] :                                                            *
 *      Calculate the hash value of BSC area                                  *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      the hash value of BSC area                                            *
 * [Note]:                                                                    *
 *      1.the BSC area contains the vector table of bootloader, flash Info, *
 *          IMEI and HASH of RF, and PUK                      *
 ******************************************************************************/
static BOOLEAN CalcBscHashValue(uint8 *pdata, uint32 len)
{
    SHA1Context_32 sha;
    uint32* pValue = g_hashvalue;

    SHA1Reset_32(&sha);
    SHA1Input_32(&sha, (const unsigned char *)pdata, len);
    SHA1Result_32(&sha, (unsigned char *)pValue);

    g_hashvalue[0] &= ~BIT_31;
    g_hashvalue[1] &= ~BIT_31;
    g_hashvalue[2] &= ~BIT_31;
    g_hashvalue[3] &= ~BIT_31;
    g_hashvalue[4] &= ~BIT_31;

    return TRUE;
}

#ifdef SECURE_BOOT_SUPPORT
PUBLIC EFUSE_RETURN_E WriteBscHashValue()
{
    EFUSE_RETURN_E result = EFUSE_WRITE_FAIL;
    uint8 *pBuf;
    uint32 i = 0;
    uint32 mHashSize = 0;

    mHashSize = BSC_GetHashLength();
    pBuf = (uint8 *)SCI_ALLOC(mHashSize<<2);
    if(pBuf != NULL)
    {
        memset(pBuf, 0, mHashSize<<2);

        if(FALSE == Security_Data_Read_Boot(pBuf, (mHashSize<<2)))
        {
            SCI_FREE(pBuf);
            return EFUSE_PTR_NULL_ERROR;
        }
    }
    else
    {
        return EFUSE_PTR_NULL_ERROR;
    }
    
    CalcBscHashValue(pBuf, mHashSize);

    g_hashvalue[0] |= BIT_31;
    g_hashvalue[1] |= BIT_31;
    g_hashvalue[2] |= BIT_31;
    g_hashvalue[3] |= BIT_31;
    g_hashvalue[4] |= BIT_31;

    //for debug
    //SCI_PASSERT(0,("g_hashvalue:: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x",g_hashvalue[0],g_hashvalue[1],g_hashvalue[2],g_hashvalue[3],g_hashvalue[4]));

    result = Efuse_HAL_WriteSecurityHash((uint32*)g_hashvalue);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if(EFUSE_RESULT_SUCCESS == result)
    {//disable JTAG 
        //result = EFUSE_HAL_Write (EFUSE_JTAG_SET_ID, JTAG_DISABLE);
    }
#endif

    SCI_FREE(pBuf);

    return result;
}
#endif

#if defined(PLATFORM_UWS6121E)
#if 0
PUBLIC BOOLEAN Write_SecurityFlag(void)
{

    if (Security_Enable() == 0)
    {
         return TRUE;
    }
    else
    {
         return FALSE;
    }
}
PUBLIC BOOLEAN Read_SecurityFlag(void)
{
    if (CheckSecureBootState())
    {
         return TRUE;
    }
    else
    {
         return FALSE;
    }

}
#endif
#else
PUBLIC BOOLEAN Write_SecurityFlag(void)
{
    EFUSE_RETURN_E bondReadResult = EFUSE_RESULT_SUCCESS;
    uint32 bondValue = 0;

    /*Purpose:To get the bonding vaule of security boot*/
    EFUSE_HAL_Open();
    bondReadResult = EFUSE_HAL_Read(EFUSE_BOUND_ID, &bondValue);
    EFUSE_HAL_Close();    

    if(EFUSE_RESULT_SUCCESS == bondReadResult)
    {
    	if(!(bondValue & ((unsigned int)SECURE_BOND_EN1)) || !(bondValue & ((unsigned int)SECURE_BOND_EN2)))
    	{
           EFUSE_HAL_Open();
           bondReadResult=EFUSE_HAL_Write (EFUSE_BOUND_ID, (SECURE_BOND_EN2|SECURE_BOND_EN1));
           EFUSE_HAL_Close();
           if(EFUSE_RESULT_SUCCESS == bondReadResult)
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
    		return TRUE;
    	}    		
    }
    else
    {
    	return FALSE;
    }

}

PUBLIC BOOLEAN Read_SecurityFlag(void)
{
    EFUSE_RETURN_E rRet = EFUSE_RESULT_SUCCESS;
    EFUSE_RETURN_E bondReadResult = EFUSE_RESULT_SUCCESS;
    uint32 bondValue = 0;
    BOOLEAN bondRet = FALSE;    

    /*Purpose:To get the bonding vaule of security boot*/
    EFUSE_HAL_Open();
    bondReadResult = EFUSE_HAL_Read(EFUSE_BOUND_ID, &bondValue);
    EFUSE_HAL_Close();      

    if(EFUSE_RESULT_SUCCESS == bondReadResult)
    {
    	if((bondValue & ((unsigned int)SECURE_BOND_EN1)) && (bondValue & ((unsigned int)SECURE_BOND_EN2)))
    	{
    		bondRet = TRUE;
    	}
    	else
    	{
    		bondRet = FALSE;
    	}
    		
    }
    else
    {
    	bondRet = FALSE;
    }

    if( (rRet == EFUSE_HAL_ReadSecurityFlag()) && (bondRet = TRUE) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif //defined(PLATFORM_UWS6121E)

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

