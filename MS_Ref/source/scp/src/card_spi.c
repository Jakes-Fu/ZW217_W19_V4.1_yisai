#include "ms_ref_scp_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "card_spi.h"
#include "StorageCard_crc.h"
#include "spi_card_pal.h"


//========START===========
// when port it to system follow definition will be replace the by system

#define CARD_SPI_PRINT(x) SCI_TRACE_LOW x
#define CARD_SPI_ASSERT SCI_ASSERT
//=========END===========


//#define CARD_CHK_CRC
#define CARD_SPEEDUP
//#define CARD_INIT_STRICTLY
//======================================

#define DEFAULT_CARD_BLOCKLEN 512
#define VOL27_28 15
#define OPVOLTAGE VOL27_28

#if 0  //solve pclint warning 751
typedef enum VOLTAGE_RANGE_E_TAG
{
	VOL20_21 = 8,
	VOL21_22 = 9,
	VOL22_23 = 10,
	VOL23_24 = 11,
	VOL24_25 = 12,
	VOL25_26 = 13,
	VOL26_27 = 14,
	VOL27_28 = 15,
	VOL28_29 = 16,
	VOL29_30 = 17,
	VOL30_31 = 18,
	VOL31_32 = 19,
	VOL32_33 = 20,
	VOL33_34 = 21,
	VOL34_35 = 22,
	VOL35_36 = 23
}VOLTAGE_RANGE_E;
#endif

typedef enum CARD_VERTION_E_TAG
{
	CARD_V_UNKONWN,
	CARD_SD_V1_X,
	CARD_SD_V2_0_STANDARD,
	CARD_SD_V2_0_HIGHCAP,
	CARD_MMC_331
}CARD_VERTION_E;

typedef struct CARD_SD_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit
	uint16 OID;	// 16 bit
	uint8 PNM[5];	// 40 bit
	uint8 PRV;	// 8 bit
	uint32 PSN;	// 32 bit
	uint16 MDT;	// 12bit
	uint8 CRC;	// 1 bit
	uint8 lastBit;	// 1 bit :always 1
}CARD_SD_CID_T;

typedef struct CARD_MMC_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit
	uint16 OID;	// 16 bit
	uint8 PNM[6];	// 48 bit
	uint8 PRV;	// 8 bit
	uint32 PSN;	// 32 bit
	uint8 MDT;	// 8bit
	uint8 CRC;	// 8 bit
	uint8 lastBit;	// 1 bit :always 1
}CARD_MMC_CID_T;

typedef union CARD_CID_STRUCT_TAG
{
	CARD_SD_CID_T SD_CID;
	CARD_MMC_CID_T MMC_CID;
}CARD_CID_T;


typedef struct CARD_SD_CSD10_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 reserved1;			// 6 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit

	uint8 reserved2;			// 2 bit
	uint16 C_SIZE;			//12 bit
	uint8 VDD_R_CURR_MIN;	// 3 bit
	uint8 VDD_R_CURR_MAX;	// 3 bit
	uint8 VDD_W_CURR_MIN;	// 3 bit
	uint8 VDD_W_CURR_MAX;	// 3 bit
	uint8 C_SIZE_MULT;		// 3 bit

	uint8 ERASE_BLK_EN;		// 1 bit
	uint8 SECTOR_SIZE;		// 7 bit
	uint8 WP_GRP_SIZE;		// 7 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 reserved3;		// 2 bit

	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved4;			// 5 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 reserved5;			// 2 bit
	uint8 CRC;				// 7 bit
	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD10_T;


typedef struct CARD_SD_CSD20_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 reserved1;			// 6 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit

	uint8 reserved2;			// 6 bit
	uint32 C_SIZE;			//22 bit
	uint8 reserved3;			// 1 bit

	uint8 ERASE_BLK_EN;		// 1 bit
	uint8 SECTOR_SIZE;		// 7 bit
	uint8 WP_GRP_SIZE;		// 7 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 reserved4;		// 2 bit

	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved5;			// 5 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 reserved6;			// 2 bit
	uint8 CRC;				// 7 bit
	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD20_T;

typedef struct CARD_MMC_CSD_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 SPEC_VERS;			// 4 bit
	uint8 reserved1;			// 2 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit
	uint8 reserved2;			// 2 bit
	uint16 C_SIZE;			//12 bit
	uint8 VDD_R_CURR_MIN;	// 3 bit
	uint8 VDD_R_CURR_MAX;	// 3 bit
	uint8 VDD_W_CURR_MIN;	// 3 bit
	uint8 VDD_W_CURR_MAX;	// 3 bit
	uint8 C_SIZE_MULT;		// 3 bit
	uint8 ERASE_GRP_SIZE;	// 5 bit
	uint8 ERASE_GRP_MULT;	// 5 bit
	uint8 WP_GRP_SIZE;		// 5 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 DEFAULT_ECC;		// 2 bit
	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved3;			// 4 bit
	uint8 CONTENT_PROT_APP;	// 1 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 ECC;				// 2 bit
	uint8 CRC;				// 7 bit
	uint8 LastBit;				// 1 bit: always 1
}CARD_MMC_CSD_T;

typedef union CARD_CSD_STRUCT_TAG
{
	CARD_SD_CSD10_T SDCSD10;
	CARD_SD_CSD20_T SDCSD20;
	CARD_MMC_CSD_T MMCCSD;
}CARD_CSD_STRUCT;

typedef struct CARD_Card_struct
{
	BOOLEAN open_flag;
	SPI_CARD_PAL_HANDLE spiHandle;

	uint32 ClockRate;
	uint32 BlockLen;

	CARD_VERTION_E vertion;
	unsigned __int64 Nac_Max;
	uint32 Nac_T;
	uint32 GrpSize;
	uint32 Capacity;
	
	uint16  CCC;
	BOOLEAN ifSupportEC;
	BOOLEAN ifSupportHighSpeed;

	BOOLEAN ifEnterEC;
	BOOLEAN ifEnterHighSpeed;

}CARD_CARD_T;




//======================================

#define CARD_DEFAULT_CLOCK 400000
#define CARD_CMD_LEN 6
#define CARD_CID_LEN 16
#define CARD_CSD_LEN 16

#define CARD_TRY_TIMES 3000

#define CARD_DEFAULT_MAX_TIMEOUT 100000LL	// unit is clock
#define CARD_NAC_T_MAX		10000	// unit is ms
#define CARD_NAC_T_MIN		5000	// unit is ms
#define CARD_NCS_MIN 0
//#define CARD_NCS_MAX CARD_DEFAULT_MAX_TIMEOUT
#define CARD_NCR_MIN 1
#define CARD_NCR_MAX 8
#define CARD_NRC_MIN 1
//#define CARD_NRC_MAX CARD_DEFAULT_MAX_TIMEOUT
#ifdef CARD_INIT_STRICTLY
#define CARD_NAC_MIN 1
#else
#define CARD_NAC_MIN 0
#endif
#if 1
#define CARD_NAC_MAX cardHandle->Nac_Max
#else
#define CARD_NAC_MAX CARD_DEFAULT_MAX_TIMEOUT
#endif
#define CARD_NWR_MIN 1
//#define CARD_NWR_MAX CARD_DEFAULT_MAX_TIMEOUT
#define CARD_NEC_MIN 0
//#define CARD_NEC_MAX CARD_DEFAULT_MAX_TIMEOUT
#define CARD_NDS_MIN 0
//#define CARD_NDS_MAX CARD_DEFAULT_MAX_TIMEOUT
#define CARD_NBR_MIN_SD 0 /*SD*/
#define CARD_NBR_MIN_MMC 1 /*MMC*/
#define CARD_NBR_MAX 1
#define CARD_NCX_MIN 0
#define CARD_NCX_MAX 8

#define CARD_NCS CARD_NCS_MIN
//#define CARD_NCR CARD_NCR_MIN
#define CARD_NRC CARD_NRC_MIN
//#define CARD_NAC CARD_NAC_MIN
#define CARD_NWR (CARD_NWR_MIN+15)
#define CARD_NEC (CARD_NEC_MIN+8)
//#define CARD_NDS CARD_NDS_MIN
#define CARD_NBR CARD_NBR_MAX
//#define CARD_NCX CARD_NCX_MIN

typedef enum CARD_RSP_TYPE_TAG
{
	CARD_R1 = 0,
	CARD_R1B,
	CARD_R2,
	CARD_R3,
	CARD_R7
}CARD_RSP_TYPE;

//--Command type
#define CARD_CMD0_GO_IDLE_STATE			0,CARD_R1
#define CARD_CMD1_SEND_OP_COND			1,CARD_R1
#define CARD_CMD6_SWITCH_FUNC /*SD*/		6,CARD_R1
#define CARD_CMD8_SEND_IF_COND /*SD*/	8,CARD_R7
#define CARD_CMD9_SEND_CSD				9,CARD_R1
#define CARD_CMD10_SEND_CID				10,CARD_R1
#define CARD_CMD12_STOP_TRANSMISSION_SD /*SD*/		12,CARD_R1B
#define CARD_CMD12_STOP_TRANSMISSION_MMC /*MMC*/	12,CARD_R1
#define CARD_CMD13_SEND_STATUS			13,CARD_R2
#define CARD_CMD16_SET_BLOCKLEN			16,CARD_R1
#define CARD_CMD17_READ_SINGLE_BLOCK		17,CARD_R1
#define CARD_CMD18_READ_MULTIPLE_BLOCK	18,CARD_R1
#define CARD_CMD23_SET_BLOCK_COUNT /*MMC*/	23,CARD_R1
#define CARD_CMD24_WRITE_BLOCK			24,CARD_R1
#define CARD_CMD25_WRITE_MULTIPLE_BLOCK	25,CARD_R1
#define CARD_CMD27_PROGRAM_CSD			27,CARD_R1
#define CARD_CMD28_SET_WRITE_PROT		28,CARD_R1B
#define CARD_CMD29_CLR_WRITE_PROT		29,CARD_R1B
#define CARD_CMD30_SEND_WRITE_PROT		30,CARD_R1
#define CARD_CMD32_ERASE_WR_BLK_START /*SD*/	32,CARD_R1
#define CARD_CMD33_ERASE_WR_BLK_END /*SD*/		33,CARD_R1
#define CARD_CMD35_ERASE_GROUP_START /*MMC*/	35,CARD_R1
#define CARD_CMD36_ERASE_GROUP_END /*MMC*/		36,CARD_R1
#define CARD_CMD38_ERASE					38,CARD_R1B
#define CARD_CMD42_LOCK_UNLOCK_SD /*SD*/	42,CARD_R1
#define CARD_CMD42_LOCK_UNLOCK_MMC /*MMC*/	42,CARD_R1B

#if defined(SPRD_SUPPORT_MCEX)
#define CARD_CMD34_READ_SEC_CMD		34,CARD_R1
#define CARD_CMD35_WRITE_SEC_CMD		35,CARD_R1
#define CARD_CMD36_SEND_PSI			36,CARD_R1
#define CARD_CMD37_CTL_TRM			37,CARD_R1
#endif
//--
#define CARD_CMD55_APP_CMD				55,CARD_R1
#define CARD_CMD56_GEN_CMD_SD /*SD*/		56,CARD_R1
#define CARD_CMD56_GEN_CMD_MMC /*MMC*/	56,CARD_R1B
//--
#define CARD_CMD58_READ_OCR				58,CARD_R3
#define CARD_CMD59_CRC_ON_OFF			59,CARD_R1
//-----------------------------------
#define CARD_ACMD13_SD_STATUS /*SD*/					13,CARD_R2
#define CARD_ACMD22_SEND_NUM_WR_BLOCKS /*SD*/		22,CARD_R1
#define CARD_ACMD23_SET_WR_BLK_ERASE_COUNT /*SD*/	23,CARD_R1
#define CARD_ACMD41_SD_SEND_OP_COND /*SD*/			41,CARD_R1
#define CARD_ACMD42_SET_CLR_CARD_DETECT /*SD*/		42,CARD_R1
#define CARD_ACMD51_SEND_SCR /*SD*/					51,CARD_R1


//---DataToken---
#define CARD_SINGLE_READ_START 0xFE
#define CARD_MULTI_READ_START 0xFE
#define CARD_SINGLE_WRITE_START 0xFE
#define CARD_MULTI_WRITE_START 0xFC
#define CARD_STOP_TRAN 0xFD

#define CARD_WD_BUSY_TIMEOUT 350  //fei.zhang 250

static CARD_CARD_T sd20_stack[CARD_MAX_SPI_NUM * CARD_MAX_CS_NUM + 1];	// +1 is just for remove the pclint warning
/*lint -save -e506 -e774*/
#define _WaitTmVal(cardHandle,tmVal)				\
{								\
	uint8 idleClk[1 + tmVal];				\
	if(0 != tmVal)						\
	{							\
		Spi_Card_Pal_Read(cardHandle->spiHandle,idleClk,tmVal);	\
	}							\
}

#define _EnableCARD(cardHandle)				\
{								\
	Spi_Card_Pal_SetCs(cardHandle->spiHandle, SPI_CARD_PAL_CS_LOW);	\
	_WaitTmVal(cardHandle,CARD_NCS);			\
}
#define _DisableCARD(cardHandle)				\
{								\
	_WaitTmVal(cardHandle,CARD_NEC);			\
	Spi_Card_Pal_SetCs(cardHandle->spiHandle, SPI_CARD_PAL_CS_HIGH);	\
}

LOCAL void _SendCARDIdleClk(CARD_CARD_T*cardHandle,uint32 clkNum)
{
	uint32 i;
	uint8 tmpChar;

	for(i = 0; i < clkNum; i++)
	{
		Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
	}
}

LOCAL void _CARD_Init(void)
{
	static BOOLEAN flag = FALSE;
	uint32 i;

	if(FALSE == flag)
	{
		flag = TRUE;
		for(i = 0; i < CARD_MAX_SPI_NUM * CARD_MAX_CS_NUM; i++)
		{
			sd20_stack[i].open_flag = FALSE;
#if defined(SPRD_SUPPORT_MCEX)
			sd20_stack[i].ifEnterEC = FALSE;
#endif

		}
	}
	return;
}

/*
	TRUE: valid
	FALSE: invalid
*/
LOCAL BOOLEAN _RegisterVerifyCARD(CARD_CARD_T*cardHandle)
{
	uint32 index;

	_CARD_Init();
	for(index = 0; index < CARD_MAX_SPI_NUM * CARD_MAX_CS_NUM; index++)
	{
		if((cardHandle == &sd20_stack[index])||(TRUE == sd20_stack[index].open_flag))
		{
			return TRUE;
		}
	}
	return FALSE;
}

#if 1
#define _BuildCARDcmd(pdata, cmdid, arg)	\
{						\
	uint8 crc;				\
						\
	pdata[0] = cmdid;			\
	pdata[0] |= BIT_6;			\
						\
	pdata[1] = (uint8)(arg>>24);		\
	pdata[2] = (uint8)(arg>>16);		\
	pdata[3] = (uint8)(arg>>8);		\
	pdata[4] = (uint8)arg;			\
						\
	crc = SC_GetCrc7(pdata, 5);		\
	pdata[5] = (crc<<1) | BIT_0;		\
}
#else
#define _BuildCARDcmd(pdata, cmdid, arg)	\
{						\
	pdata[0] = cmdid;			\
	pdata[0] |= 0x40;			\
						\
	pdata[1] = (uint8)(arg>>24);		\
	pdata[2] = (uint8)(arg>>16);		\
	pdata[3] = (uint8)(arg>>8);		\
	pdata[4] = (uint8)arg;			\
						\
	pdata[5] = 0x95;			\
						\
}
#endif

LOCAL BOOLEAN _SendCARDcmd(CARD_CARD_T*cardHandle,uint8 cmdid, CARD_RSP_TYPE type,uint32 arg,uint8* response)
{
	uint8 cmdbuf[CARD_NRC+CARD_CMD_LEN+CARD_NCR_MIN];
	uint32 i;
	uint8 tmpChar;
	uint32 time_out;
	BOOLEAN ifTimeOut;

	response[0] = 0;
	response[1] = 0;
	response[2] = 0;
	response[3] = 0;
	response[4] = 0;

#if defined(CARD_SPEEDUP)
	//set card idle clk
	for(i = 0; i < CARD_NRC; i++)
	{
		cmdbuf[i]= 0xFF;	
	}
	_BuildCARDcmd((cmdbuf+CARD_NRC), cmdid, arg);
	Spi_Card_Pal_Write(cardHandle->spiHandle, cmdbuf,CARD_NRC+CARD_CMD_LEN);
	//set card idle clk
	_WaitTmVal(cardHandle, CARD_NCR_MIN);
#else
	_WaitTmVal(cardHandle, CARD_NRC);
	_BuildCARDcmd(cmdbuf, cmdid, arg);
	Spi_Card_Pal_Write(cardHandle->spiHandle, cmdbuf,CARD_CMD_LEN);
	_WaitTmVal(cardHandle, CARD_NCR_MIN);
#endif
	for(i = CARD_NCR_MIN; i < (CARD_NCR_MAX+1); i++)
	{
		Spi_Card_Pal_Read(cardHandle->spiHandle,response,1);
		if(0 == (response[0] & 0x80))
		{
			break;
		}
	}
	if((CARD_NCR_MAX+1) == i)
	{
		return FALSE;//fail
	}
	//recieve other byte
	switch(type)
	{
		case CARD_R1:
		{
		}
		break;

		case CARD_R1B:
		{
			time_out = SCI_GetTickCount();
			ifTimeOut = FALSE;
			do
			{
				if(38 == cmdid)
				{
					if(20000 <= (SCI_GetTickCount() - time_out))
					{
						//CARD_SPI_PRINT:"card Read busy 1B signal timeout"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_528_112_2_18_1_52_33_148,(uint8*)"");
						ifTimeOut = TRUE;
					}
				}
				else
				{
					if(cardHandle->Nac_T <= (SCI_GetTickCount() - time_out))
					{
						//CARD_SPI_PRINT:"card Read busy 1B signal timeout"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_536_112_2_18_1_52_33_149,(uint8*)"");
						ifTimeOut = TRUE;
					}
				}
				//--------------
				Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
				if(tmpChar == 0xFF)		//may be fail here jason.wu
				{
					break;				// success
				}
				//--------------
				if(TRUE == ifTimeOut)
				{
					return FALSE;		// fail timeout
				}
			}
			while (1);
		}
		break;

		case CARD_R2:
			Spi_Card_Pal_Read(cardHandle->spiHandle,response+1,1);
		break;

		case CARD_R3:
			Spi_Card_Pal_Read(cardHandle->spiHandle,response+1,4);
			break;

		case CARD_R7:
			Spi_Card_Pal_Read(cardHandle->spiHandle,response+1,4);
		break;

		default:
			CARD_SPI_ASSERT(0);
			return FALSE;     /*lint !e527*/
	}

	return TRUE; //success

}
//	This funtion used to Read a block data from SD card
//	Author:	Jason.wu
//	Param:
//		cardHandle: the handle of SD device ,it is returned by CARD_SPI_Open
//		StarToken: the first byte of datablock that indicate the start of transmittiong
//		buf:the data that read out will be saved in this buffer
//		len:the length of data block that will be read
//		CRC:the CRC of datablock
//		ifReg:indicate that whether read CID or CSD : TRUE: Read CID or CSD.FALSE: Read a datablock
//	Return:
//		TRUE: Read Success
//		FALSE: Fail
//
//		Get:startToken-----DataBlock-----CRC
//
LOCAL BOOLEAN _ReadCARDDataToken(CARD_CARD_T*cardHandle,uint8 StartToken,uint8* buf,uint32 len,uint16* CRC,BOOLEAN ifReg)
{
	unsigned __int64 i;
	uint32 time_out;
	BOOLEAN ifTimeOut;
#if defined(CARD_CHK_CRC)
	uint16 tmpCrc;
#endif
	uint8 tmpChar;
	uint8 crcBuf[2];
	BOOLEAN result;

//-- wait start token
	if(TRUE == ifReg)
	{
		_WaitTmVal(cardHandle, CARD_NCX_MIN);
		for(i = CARD_NCX_MIN; i < CARD_NCX_MAX+1; i++)
		{
			Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
			if(tmpChar == StartToken)
			{
				break;
			}
		}
		if((CARD_NCX_MAX+1) == i)
		{
			return FALSE;
		}

	}
	else
	{
		_WaitTmVal(cardHandle, CARD_NAC_MIN);
		time_out = SCI_GetTickCount();
		ifTimeOut = FALSE;
		do
		{
			if(cardHandle->Nac_T <= (SCI_GetTickCount() - time_out))
			{
				//CARD_SPI_PRINT:"card Read busy signal timeout"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_630_112_2_18_1_52_33_150,(uint8*)"");
				ifTimeOut = TRUE;
			}
			//--------------
			Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
			if(StartToken == tmpChar)
			{
				break;				// success
			}
			//--------------
			if(TRUE == ifTimeOut)
			{
				return FALSE;		// fail wait start token timeout
			}
		}
		while(1);
	}
//--
	result = TRUE;
	if(FALSE == Spi_Card_Pal_Read(cardHandle->spiHandle, buf, len))
	{
		result = FALSE;
	}

	Spi_Card_Pal_Read(cardHandle->spiHandle,crcBuf,2);
#if defined(CARD_CHK_CRC)
#ifndef CARD_INIT_STRICTLY
if(
	(FALSE == ifReg)
	&&(DEFAULT_CARD_BLOCKLEN == len)
)
#endif
{
	tmpCrc = crcBuf[0];
	tmpCrc = tmpCrc<<8;
	tmpCrc |= crcBuf[1];
	if(tmpCrc != SC_GetCrc16(buf, len))
	{
		//CARD_SPI_PRINT:"card CRC read error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_668_112_2_18_1_52_33_151,(uint8*)"");
		return FALSE;
	}
#ifndef CARD_INIT_STRICTLY
}
#endif
#endif
	return result;
}

//	This funtion used to Read a block data from SD card
//	Author:	Jason.wu
//	Param:
//		cardHandle: the handle of SD device ,it is returned by CARD_SPI_Open
//		StarToken: the first byte of datablock that indicate the start of transmittiong
//		buf:the data that will be write is saved in this buffer
//		len:the length of data block that will be write
//		CRC:the CRC of datablock
//		ifReg:indicate that whether read CID or CSD : TRUE: Read CID or CSD.FALSE: Read a datablock
//	Return:
//		TRUE: Write Success
//		FALSE: Fail
//
//		Send:startToken-----DataBlock-----CRC Get:DataRsp
//
LOCAL BOOLEAN _WriteCARDDataToken(CARD_CARD_T*cardHandle,uint8 StartToken,uint8* buf,uint32 len,uint8* dataRsp)
{
#if defined(CARD_SPEEDUP)
	uint32 i;
#endif
	uint32 time_out;
	BOOLEAN ifTimeOut;
#if defined(CARD_CHK_CRC)
	uint16 tmpCrc;
#endif
	uint8 tmpChar;
	uint8 tmpBuf[CARD_NWR+1];
	BOOLEAN result = TRUE;

#if defined(CARD_SPEEDUP)
	//set the idle clk
	for(i = 0; i < CARD_NWR; i++)
	{
		tmpBuf[i] = 0xFF;
	}
	//add the start token
	tmpBuf[CARD_NWR] = StartToken;
	Spi_Card_Pal_Write(cardHandle->spiHandle, tmpBuf , CARD_NWR+1);
#else
	_WaitTmVal(cardHandle, CARD_NWR);
	Spi_Card_Pal_Write(cardHandle->spiHandle, &StartToken, 1);
#endif
	//write the data
	if(FALSE == Spi_Card_Pal_Write(cardHandle->spiHandle, buf , len))
	{
		result = FALSE;
	}

	//CRC
#if defined(CARD_CHK_CRC)
//	_WaitTmVal(cardHandle, 2);
	tmpCrc = SC_GetCrc16(buf, len);
	tmpBuf[0] = (uint8)(0xFF&(tmpCrc>>8));
	tmpBuf[1] = (uint8)(0xFF&tmpCrc);
	Spi_Card_Pal_Write(cardHandle->spiHandle, tmpBuf , 2);
	Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
	*dataRsp = 0x0E&tmpChar;
	if(/*(0x04 == *dataRsp)&&*/(0x0A == *dataRsp)||(0x0C == *dataRsp))
	{
		//CARD_SPI_PRINT:"card write error1 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_737_112_2_18_1_52_33_152,(uint8*)"d",*dataRsp);
		return FALSE;
	}
//	if(0x04 != *dataRsp)
//	{
//		CARD_SPI_PRINT(("card write error2 0x%x",*dataRsp));
//		return FALSE;
//	}
#elif defined(CARD_SPEEDUP)
	Spi_Card_Pal_Read(cardHandle->spiHandle,tmpBuf,3);
	*dataRsp = 0x0E&tmpBuf[2];
	if(/*(0x04 == *dataRsp)&&*/(0x0A == *dataRsp)||(0x0C == *dataRsp))
	{
		return FALSE;
	}
	
#else
//	_WaitTmVal(cardHandle, 2);
	tmpBuf[0] = 0;
	tmpBuf[1] = 0;
	Spi_Card_Pal_Write(cardHandle->spiHandle, tmpBuf , 2);
	Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
	*dataRsp = 0x0E&tmpChar;
	if(/*(0x04 == *dataRsp)&&*/(0x0A == *dataRsp)||(0x0C == *dataRsp))
	{
		//CARD_SPI_PRINT:"card write error1 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_762_112_2_18_1_52_33_153,(uint8*)"d",*dataRsp);
		return FALSE;
	}
//	if(0x04 != *dataRsp)
//	{
//		CARD_SPI_PRINT(("card write error2 0x%x",*dataRsp));
//		return FALSE;
//	}
#endif
	time_out = SCI_GetTickCount();
	ifTimeOut = FALSE;
	do
	{
		if(CARD_WD_BUSY_TIMEOUT <= (SCI_GetTickCount() - time_out))
		{
			//CARD_SPI_PRINT:"card write busy signal timeout"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_777_112_2_18_1_52_33_154,(uint8*)"");
			ifTimeOut = TRUE;
		}
		//--------------
		Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
		if(0xFF == tmpChar)
		{
			break;
		}
		//--------------
		if(TRUE == ifTimeOut)
		{
			return FALSE;		// fail wait start token timeout
		}
	}
	while(1);
	return result;
}

//
//	Expand SD CSD1.x to CSD struct member
//
LOCAL void _SD_CSD10_Analyze(uint8* CSDbuf,CARD_SD_CSD10_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3F;//0b00111111;
	CSD->reserved1 = tmp8;			// 6 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit

	tmp8 = CSDbuf[6]&0x0C;//0b00001100;
	CSD->reserved2 = tmp8>>2;			// 2 bit

	tmp16 = CSDbuf[6]&0x03;//0b00000011;
	tmp16 = tmp16<<8;
	tmp16 += CSDbuf[7];
	tmp16 = tmp16<<2;
	tmp8 = CSDbuf[8]&0xC0;//0b11000000;
	tmp8 = tmp8>>6;
	tmp16 = tmp16 + tmp8;
	CSD->C_SIZE = tmp16;				//12 bit

	tmp8 = CSDbuf[8]&0x38;//0b00111000;
	CSD->VDD_R_CURR_MIN = tmp8>>3;	// 3 bit

	tmp8 = CSDbuf[8]&0x07;//0b00000111;
	CSD->VDD_R_CURR_MAX = tmp8;		// 3 bit

	tmp8 = CSDbuf[9]&0xE0;//0b11100000;
	CSD->VDD_W_CURR_MIN = tmp8>>5;	// 3 bit

	tmp8 = CSDbuf[9]&0x1C;//0b00011100;
	CSD->VDD_W_CURR_MAX = tmp8>>2;	// 3 bit

	tmp8 = CSDbuf[9]&0x03;//0b00000011;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[10]&0x80/*0b10000000*/)>>7);
	CSD->C_SIZE_MULT = tmp8;			// 3 bit
//--
	tmp8 = CSDbuf[10]&0x40;//0b01000000;
	CSD->ERASE_BLK_EN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[10]&0x3F;//0b00111111;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[11]&0x80/*0b10000000*/)>>7);
	CSD->SECTOR_SIZE = tmp8;		// 7 bit

	tmp8 = CSDbuf[11]&0x7F;//0b01111111;
	CSD->WP_GRP_SIZE = tmp8;			// 7 bit
//--
	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->reserved3 = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1F;//0b00011111;
	CSD->reserved4 = tmp8;			// 5 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->reserved5 = tmp8;					// 2 bit

	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
	CSD->CRC = tmp8>>1;				// 7 bit

	tmp8 = CSDbuf[15]&0x01;//0b00000001;
	CSD->LastBit = tmp8;				// 1 bit: always 1
}


//
//	Expand SD SDCSD20. to CSD struct member
//
LOCAL void _SD_CSD20_Analyze(uint8* CSDbuf,CARD_SD_CSD20_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;
	uint32 tmp32;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3F;//0b00111111;
	CSD->reserved1 = tmp8;			// 6 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit
//--
	tmp8 = CSDbuf[6]&0x0F;//0b00001111;
	tmp8 = tmp8<<2;
	tmp8 += ((CSDbuf[7]&0xC0/*0b11000000*/)>>6);
	CSD->reserved2 = tmp8;				// 6 bit

	tmp32 = CSDbuf[7]&0x3F;//0b00111111
	tmp32 = tmp32<<8;
	tmp32 += CSDbuf[8];
	tmp32 = tmp32<<8;
	tmp32 += CSDbuf[9];
	CSD->C_SIZE = tmp32;				// 22 bit

	tmp8 = CSDbuf[10]&0x80;//0b10000000
	tmp8 = tmp8>>7;
	CSD->reserved3 = tmp8;
//--
	tmp8 = CSDbuf[10]&0x40;//0b01000000;
	CSD->ERASE_BLK_EN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[10]&0x3F;//0b00111111;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[11]&0x80/*0b10000000*/)>>7);
	CSD->SECTOR_SIZE = tmp8;		// 7 bit

	tmp8 = CSDbuf[11]&0x7F;//0b01111111;
	CSD->WP_GRP_SIZE = tmp8;			// 7 bit
//--
	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->reserved4 = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1F;//0b00011111;
	CSD->reserved5 = tmp8;			// 5 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->reserved6 = tmp8;					// 2 bit

	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
	CSD->CRC = tmp8>>1;				// 7 bit

	tmp8 = CSDbuf[15]&0x01;//0b00000001;
	CSD->LastBit = tmp8;				// 1 bit: always 1
}


//
//	Expand MMC CSD to CSD struct member
//
LOCAL void _MMC_CSD_Analyze(uint8* CSDbuf,CARD_MMC_CSD_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3C;//0b00111100;
	CSD->SPEC_VERS = tmp8>>2;	// 4 bit

	tmp8 = CSDbuf[0]&0x03;//0b00000011;
	CSD->reserved1 = tmp8;			// 2 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit

	tmp8 = CSDbuf[6]&0x0C;//0b00001100;
	CSD->reserved2 = tmp8>>2;			// 2 bit

	tmp16 = CSDbuf[6]&0x03;//0b00000011;
	tmp16 = tmp16<<8;
	tmp16 += CSDbuf[7];
	tmp16 = tmp16<<2;
	tmp8 = CSDbuf[8]&0xC0;//0b11000000;
	tmp8 = tmp8>>6;
	tmp16 = tmp16 + tmp8;
	CSD->C_SIZE = tmp16;				//12 bit

	tmp8 = CSDbuf[8]&0x38;//0b00111000;
	CSD->VDD_R_CURR_MIN = tmp8>>3;	// 3 bit

	tmp8 = CSDbuf[8]&0x07;//0b00000111;
	CSD->VDD_R_CURR_MAX = tmp8;		// 3 bit

	tmp8 = CSDbuf[9]&0xE0;//0b11100000;
	CSD->VDD_W_CURR_MIN = tmp8>>5;	// 3 bit

	tmp8 = CSDbuf[9]&0x1C;//0b00011100;
	CSD->VDD_W_CURR_MAX = tmp8>>2;	// 3 bit

	tmp8 = CSDbuf[9]&0x03;//0b00000011;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[10]&0x80/*0b10000000*/)>>7);
	CSD->C_SIZE_MULT = tmp8;			// 3 bit

	tmp8 = CSDbuf[10]&0x7C;//0b01111100;
	CSD->ERASE_GRP_SIZE = tmp8>>2;	// 5 bit

	tmp8 = CSDbuf[10]&0x03;//0b00000011;
	tmp8 = tmp8<<3;
	tmp8 = tmp8 + ((CSDbuf[11]&0xE0/*0b11100000*/)>>5);
	CSD->ERASE_GRP_MULT = tmp8;		// 5 bit

	tmp8 = CSDbuf[11]&0x1F;//0b00011111;
	CSD->WP_GRP_SIZE = tmp8;			// 5 bit

	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->DEFAULT_ECC = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1E;//0b00011110;
	CSD->reserved3 = tmp8>>1;			// 4 bit

	tmp8 = CSDbuf[13]&0x01;//0b00000001;
	CSD->CONTENT_PROT_APP = tmp8;	// 1 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->ECC = tmp8;					// 2 bit

	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
	CSD->CRC = tmp8>>1;				// 7 bit

	tmp8 = CSDbuf[15]&0x01;//0b00000001;
	CSD->LastBit = tmp8;				// 1 bit: always 1
}



//
//	Expand CID to CID struct member
//
LOCAL void _SD_CID_Analyze(uint8* CIDbuf,CARD_SD_CID_T *CID)
{
	uint16 tmp16;
	uint32 tmp32;

	CID->MID = CIDbuf[0];

	tmp16 = CIDbuf[1];
	tmp16 = tmp16<<8;
	CID->OID = tmp16 + CIDbuf[2];

	CID->PNM[0] = CIDbuf[3];
	CID->PNM[1] = CIDbuf[4];
	CID->PNM[2] = CIDbuf[5];
	CID->PNM[3] = CIDbuf[6];
	CID->PNM[4] = CIDbuf[7];

	CID->PRV = CIDbuf[8];

	tmp32 = CIDbuf[9];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[10];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[11];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[12];
	CID->PSN = tmp32;

	tmp16 = CIDbuf[13]&0x0F;
	tmp16 = tmp16<<8;
	CID->MDT = tmp16 + CIDbuf[14];

	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
	CID->lastBit = (CIDbuf[15]&0x01);
}


LOCAL void _MMC_CID_Analyze(uint8* CIDbuf,CARD_MMC_CID_T *CID)
{
	uint16 tmp16;
	uint32 tmp32;

	CID->MID = CIDbuf[0];

	tmp16 = CIDbuf[1];
	tmp16 = tmp16<<8;
	CID->OID = tmp16 + CIDbuf[2];

	CID->PNM[0] = CIDbuf[3];
	CID->PNM[1] = CIDbuf[4];
	CID->PNM[2] = CIDbuf[5];
	CID->PNM[3] = CIDbuf[6];
	CID->PNM[4] = CIDbuf[7];
	CID->PNM[5] = CIDbuf[8];

	CID->PRV = CIDbuf[9];

	tmp32 = CIDbuf[10];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[11];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[12];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[13];
	CID->PSN = tmp32;

	CID->MDT = CIDbuf[14];

	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
	CID->lastBit = (CIDbuf[15]&0x01);
}



/********************************************************************
*	Description: This function is used to registe a SD card to SPI driver
*	Dependence: the SD must have run CARD_Init success
*	Author: Jason.wu
*	Param:
*		SPI_Id: the port that SD used
*		CS_Id:  the enable line of SPI_Id port that SD used
*		Max_ClockRate: the clock rate that SD is normal used
*	Result:
*		None zero: Success,this handler is used as param in other function
*		zero: Fail
********************************************************************/
PUBLIC CARD_CARD_T* CARD_SPI_Open(  // success - return the hardware interface vector
                                                            // failure - return NULL
	CARD_SPIPORT_E SPI_Id, // SPI port num
	CARD_SPICS_E CS_Id, // Chip select id
	uint32 Max_ClockRate// SPI clock
)
{
	uint32 index;

	CARD_SPI_ASSERT((CARD_MAX_SPI_NUM > SPI_Id)||(CARD_MAX_CS_NUM > CS_Id));

	_CARD_Init();

	index = CARD_MAX_SPI_NUM * SPI_Id + CS_Id;
	if(TRUE == sd20_stack[index].open_flag)
	{
		return 0;
	}

	sd20_stack[index].open_flag = TRUE;
	sd20_stack[index].spiHandle = Spi_Card_Pal_Open((SPI_CARD_PORT_E)SPI_Id, (SPI_CARD_CS_E)CS_Id);

	sd20_stack[index].ClockRate = Max_ClockRate;
	sd20_stack[index].BlockLen = 0;
	sd20_stack[index].vertion = CARD_V_UNKONWN;

	return &sd20_stack[index];
}

PUBLIC BOOLEAN CARD_SPI_PwrHook(CARD_SPI_HANDLE cardHandle, CARD_SPI_PWR_E onOrOff)
{
	switch(onOrOff)
	{
		case CARD_SPI_PWR_ON_PRE:
		{
			if(FALSE == Spi_Card_Pal_PwrHook(cardHandle->spiHandle, SPI_CARD_PAL_PWR_ON_PRE))
			{
				CARD_SPI_ASSERT(0);
				return FALSE;	/*lint !e527*/
			}
		}
		break;

		case CARD_SPI_PWR_OFF_AFTER:
		{
			if(FALSE == Spi_Card_Pal_PwrHook(cardHandle->spiHandle, SPI_CARD_PAL_PWR_OFF_AFTER))
			{
				CARD_SPI_ASSERT(0);
				return FALSE;	/*lint !e527*/
			}
		}
		break;

		default:
		{
			CARD_SPI_ASSERT(0);
			return FALSE;		/*lint !e527*/
		}
		break;					/*lint !e527*/
	}
	return TRUE;
}

/********************************************************************
*	Description: This function is used to remove a SD card from SPI driver
*	Dependence: the SD must have run CARD_SPI_Open success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_Close(CARD_CARD_T*cardHandle)
{
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	Spi_Card_Pal_Close(cardHandle->spiHandle);
	cardHandle->open_flag = FALSE;
	return TRUE;
	
}

/********************************************************************
*	Description: This function is used to set block length for read and write
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler is returned by CARD_SPI_Open
*		length: the block length that you want to set ,typecally 512
*	Result:
*		TRUE: Success
*		FALSE: Fail
*********************************************************************/
PUBLIC BOOLEAN CARD_SPI_SetBlockLength(CARD_CARD_T*cardHandle,uint32 length)
{
	uint8 response[5];
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)&&(512 != length))
	{
		//CARD_SPI_PRINT:"Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1377_112_2_18_1_52_34_155,(uint8*)"d",length);
		return FALSE;
	}
	else if(length == cardHandle->BlockLen)
	{
		return TRUE;
	}
	else
	{
		Spi_Card_Pal_SetClk(cardHandle->spiHandle,cardHandle->ClockRate);
		_EnableCARD(cardHandle);
	//--
		result = _SendCARDcmd(cardHandle,CARD_CMD16_SET_BLOCKLEN, length,response);
		if(0 != response[0])
		{
			result = FALSE;
		}
	//--
		_DisableCARD(cardHandle);
		if(TRUE == result)
		{
			cardHandle->BlockLen = length;
		}
	 //--
	 	return result;
	}
}

/********************************************************************
*	Description: This function is used to Read OCR regist of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		OCR: used to save the value of OCR register
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
LOCAL BOOLEAN CARD_SPI_ReadOCR(CARD_CARD_T*cardHandle,uint32* OCR)
{
	uint8 response[5];
	BOOLEAN result = TRUE;
	uint32 tmpOCR;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle,cardHandle->ClockRate);
	_EnableCARD(cardHandle);
//--
	result = _SendCARDcmd(cardHandle,CARD_CMD58_READ_OCR, 0,response);
	if(0 != (response[0]&0xFE))
	{
		result = FALSE;
	}
//--
	_DisableCARD(cardHandle);

	tmpOCR = response[1];
	tmpOCR = (tmpOCR<<8)+response[2];
	tmpOCR = (tmpOCR<<8)+response[3];
	tmpOCR = (tmpOCR<<8)+response[4];
	*OCR = tmpOCR;

	return result;
}

/********************************************************************
*	Description: This function is used to Read CID regist of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		CID: used to save the value of CID register
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
LOCAL BOOLEAN CARD_SPI_ReadCID(CARD_CARD_T*cardHandle,CARD_CID_T* CID)
{
	uint8 response[5];
	uint8 tmpChar[CARD_CID_LEN];
	uint16 CRC;
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle,cardHandle->ClockRate);
	_EnableCARD(cardHandle);
//--
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD10_SEND_CID, 0,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START, tmpChar,CARD_CID_LEN,&CRC,TRUE);

//--
	_DisableCARD(cardHandle);
//--
	if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		_SD_CID_Analyze(tmpChar,&(CID->SD_CID));
		//CARD_SPI_PRINT:"Response = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1491_112_2_18_1_52_35_156,(uint8*)"d",response[0]);

		//CARD_SPI_PRINT:"-----CID value---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1493_112_2_18_1_52_35_157,(uint8*)"");
		//CARD_SPI_PRINT:"MID\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1494_112_2_18_1_52_35_158,(uint8*)"d",CID->SD_CID.MID);
		//CARD_SPI_PRINT:"OID\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1495_112_2_18_1_52_35_159,(uint8*)"d",CID->SD_CID.OID);
		//CARD_SPI_PRINT:"PNM\t\t = %c,%c,%c,%c,%c"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1496_112_2_18_1_52_35_160,(uint8*)"ccccc",CID->SD_CID.PNM[0],CID->SD_CID.PNM[1],CID->SD_CID.PNM[2],CID->SD_CID.PNM[3],CID->SD_CID.PNM[4]);
		//CARD_SPI_PRINT:"PRV\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1497_112_2_18_1_52_35_161,(uint8*)"d",CID->SD_CID.PRV);
		//CARD_SPI_PRINT:"PSN\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1498_112_2_18_1_52_35_162,(uint8*)"d",CID->SD_CID.PSN);
		//CARD_SPI_PRINT:"MDT\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1499_112_2_18_1_52_35_163,(uint8*)"d",CID->SD_CID.MDT);
		//CARD_SPI_PRINT:"CRC\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1500_112_2_18_1_52_35_164,(uint8*)"d",CID->SD_CID.CRC);
		//CARD_SPI_PRINT:"lastBit\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1501_112_2_18_1_52_35_165,(uint8*)"d",CID->SD_CID.lastBit);
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		_MMC_CID_Analyze(tmpChar,&(CID->MMC_CID));
		//CARD_SPI_PRINT:"Response = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1506_112_2_18_1_52_35_166,(uint8*)"d",response[0]);

		//CARD_SPI_PRINT:"-----CID value---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1508_112_2_18_1_52_35_167,(uint8*)"");
		//CARD_SPI_PRINT:"MID\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1509_112_2_18_1_52_35_168,(uint8*)"d",CID->MMC_CID.MID);
		//CARD_SPI_PRINT:"OID\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1510_112_2_18_1_52_35_169,(uint8*)"d",CID->MMC_CID.OID);
		//CARD_SPI_PRINT:"PNM\t\t = %c,%c,%c,%c,%c,%c"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1511_112_2_18_1_52_35_170,(uint8*)"cccccc",CID->MMC_CID.PNM[0],CID->MMC_CID.PNM[1],CID->MMC_CID.PNM[2],CID->MMC_CID.PNM[3],CID->MMC_CID.PNM[4],CID->MMC_CID.PNM[5]);
		//CARD_SPI_PRINT:"PRV\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1512_112_2_18_1_52_35_171,(uint8*)"d",CID->MMC_CID.PRV);
		//CARD_SPI_PRINT:"PSN\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1513_112_2_18_1_52_35_172,(uint8*)"d",CID->MMC_CID.PSN);
		//CARD_SPI_PRINT:"MDT\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1514_112_2_18_1_52_35_173,(uint8*)"d",CID->MMC_CID.MDT);
		//CARD_SPI_PRINT:"CRC\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1515_112_2_18_1_52_35_174,(uint8*)"d",CID->MMC_CID.CRC);
		//CARD_SPI_PRINT:"lastBit\t\t = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1516_112_2_18_1_52_35_175,(uint8*)"d",CID->MMC_CID.lastBit);
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}

//--
	return result;

}


LOCAL void _GetNac(CARD_CARD_T*cardHandle,CARD_CSD_STRUCT* CSD)
{
	unsigned __int64 unit = 0;	// ns
	unsigned __int64 value = 0;	// 10x
	uint8 TAAC = 0, NSAC = 0;
	uint8 tmpVal;

	if((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))
	{
		TAAC = CSD->SDCSD10.TAAC;
		NSAC = CSD->SDCSD10.NSAC;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		TAAC = CSD->SDCSD20.TAAC;
		NSAC = CSD->SDCSD20.NSAC;
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		TAAC = CSD->MMCCSD.TAAC;
		NSAC = CSD->MMCCSD.NSAC;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}


	tmpVal = (0x7&TAAC);
	if(0 == tmpVal)
	{
		unit = 1;
	}
	else if(1 == tmpVal)
	{
		unit = 10;
	}
	else if(2 == tmpVal)
	{
		unit = 100;
	}
	else if(3 == tmpVal)
	{
		unit = 1000;
	}
	else if(4 == tmpVal)
	{
		unit = 10000;
	}
	else if(5 == tmpVal)
	{
		unit = 100000;
	}
	else if(6 == tmpVal)
	{
		unit = 1000000;
	}
	else if(7 == tmpVal)
	{
		unit = 10000000;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	tmpVal = ((0x78&TAAC)>>3);
	if(0 == tmpVal)
	{
		value = 80;	// reserved
	}
	else if(1 == tmpVal)
	{
		value = 10;
	}
	else if(2 == tmpVal)
	{
		value = 12;
	}
	else if(3 == tmpVal)
	{
		value = 13;
	}
	else if(4 == tmpVal)
	{
		value = 15;
	}
	else if(5 == tmpVal)
	{
		value = 20;
	}
	else if(6 == tmpVal)
	{
		value = 25;
	}
	else if(7 == tmpVal)
	{
		value = 30;
	}
	else if(8 == tmpVal)
	{
		value = 35;
	}
	else if(9 == tmpVal)
	{
		value = 40;
	}
	else if(0x0A == tmpVal)
	{
		value = 45;
	}
	else if(0x0B == tmpVal)
	{
		value = 50;
	}
	else if(0x0C == tmpVal)
	{
		value = 55;
	}
	else if(0x0D == tmpVal)
	{
		value = 60;
	}
	else if(0x0E == tmpVal)
	{
		value = 70;
	}
	else if(0x0F == tmpVal)
	{
		value = 80;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	if((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))
	{
		cardHandle->Nac_Max = (unsigned __int64)(((unit*value*cardHandle->ClockRate)/10000000000LL + (100*NSAC))*100);
//		cardHandle->Nac_T = (uint32)((unit*value*100)/1000000LL) + (uint32)((NSAC*10000000LL)/cardHandle->ClockRate);
		cardHandle->Nac_T = 120;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		cardHandle->Nac_Max = (unsigned __int64)(((unit*value*cardHandle->ClockRate)/10000000000LL + (100*NSAC))*100);
//		cardHandle->Nac_T = (uint32)((unit*value*100)/1000000LL) + (uint32)((NSAC*10000000LL)/cardHandle->ClockRate);
		cardHandle->Nac_T = 120;
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		cardHandle->Nac_Max = (unsigned __int64)((((unit*value*cardHandle->ClockRate)/10000000000LL + (100*NSAC))*10) / 8);
		cardHandle->Nac_T = (uint32)((unit*value*10)/8000000LL) + (uint32)((NSAC*1000000LL)/(uint32)(8*cardHandle->ClockRate));
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	if(CARD_DEFAULT_MAX_TIMEOUT > cardHandle->Nac_Max)
	{
		cardHandle->Nac_Max = CARD_DEFAULT_MAX_TIMEOUT;
	}
#if 0
	if(CARD_NAC_T_MIN > cardHandle->Nac_T)
	{
		cardHandle->Nac_T = CARD_NAC_T_MIN;
	}
	else if(CARD_NAC_T_MAX < cardHandle->Nac_T)
	{
		cardHandle->Nac_T = CARD_NAC_T_MAX;
	}
#endif
	return;
}


/********************************************************************
*	Description: This function is used to Read CSD regist of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		CSD: used to save the value of CSD register
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
LOCAL BOOLEAN CARD_SPI_ReadCSD(CARD_CARD_T*cardHandle,CARD_CSD_STRUCT* CSD)
{
	uint8 response[5];
	uint8 tmpChar[CARD_CSD_LEN];
	uint16 CRC;
	BOOLEAN result = TRUE;
	
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	cardHandle->CCC = 0;
	Spi_Card_Pal_SetClk(cardHandle->spiHandle,cardHandle->ClockRate);
	_EnableCARD(cardHandle);
//--
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD9_SEND_CSD, 0,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START, tmpChar,CARD_CSD_LEN,&CRC,TRUE);
//--
	_DisableCARD(cardHandle);
//--
	if(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	{
		_SD_CSD20_Analyze(tmpChar,&(CSD->SDCSD20));
		cardHandle->CCC = CSD->SDCSD20.CCC;

		//CARD_SPI_PRINT:"Response = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1746_112_2_18_1_52_35_176,(uint8*)"d",response[0]);

		//CARD_SPI_PRINT:"CSD_STRUCTURE = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1748_112_2_18_1_52_35_177,(uint8*)"d",CSD->SDCSD20.CSD_STRUCTURE);
		//CARD_SPI_PRINT:"reserved1= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1749_112_2_18_1_52_35_178,(uint8*)"d",CSD->SDCSD20.reserved1);
		//CARD_SPI_PRINT:"TAAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1750_112_2_18_1_52_35_179,(uint8*)"d",CSD->SDCSD20.TAAC);
		//CARD_SPI_PRINT:"NSAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1751_112_2_18_1_52_35_180,(uint8*)"d",CSD->SDCSD20.NSAC);
		//CARD_SPI_PRINT:"TRAN_SPEED= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1752_112_2_18_1_52_35_181,(uint8*)"d",CSD->SDCSD20.TRAN_SPEED);
		//CARD_SPI_PRINT:"CCC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1753_112_2_18_1_52_35_182,(uint8*)"d",CSD->SDCSD20.CCC);
		//CARD_SPI_PRINT:"READ_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1754_112_2_18_1_52_35_183,(uint8*)"d",CSD->SDCSD20.READ_BL_LEN);
		//CARD_SPI_PRINT:"READ_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1755_112_2_18_1_52_35_184,(uint8*)"d",CSD->SDCSD20.READ_BL_PARTIAL);
		//CARD_SPI_PRINT:"WRITE_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1756_112_2_18_1_52_35_185,(uint8*)"d",CSD->SDCSD20.WRITE_BLK_MISALIGN);
		//CARD_SPI_PRINT:"READ_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1757_112_2_18_1_52_35_186,(uint8*)"d",CSD->SDCSD20.READ_BLK_MISALIGN);
		//CARD_SPI_PRINT:"DSR_IMP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1758_112_2_18_1_52_35_187,(uint8*)"d",CSD->SDCSD20.DSR_IMP);

		//CARD_SPI_PRINT:"reserved2= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1760_112_2_18_1_52_35_188,(uint8*)"d",CSD->SDCSD20.reserved2);
		//CARD_SPI_PRINT:"C_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1761_112_2_18_1_52_35_189,(uint8*)"d",CSD->SDCSD20.C_SIZE);
		//CARD_SPI_PRINT:"reserved3= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1762_112_2_18_1_52_35_190,(uint8*)"d",CSD->SDCSD20.reserved3);

		//CARD_SPI_PRINT:"ERASE_BLK_EN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1764_112_2_18_1_52_35_191,(uint8*)"d",CSD->SDCSD20.ERASE_BLK_EN);
		//CARD_SPI_PRINT:"SECTOR_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1765_112_2_18_1_52_35_192,(uint8*)"d",CSD->SDCSD20.SECTOR_SIZE);
		//CARD_SPI_PRINT:"WP_GRP_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1766_112_2_18_1_52_35_193,(uint8*)"d",CSD->SDCSD20.WP_GRP_SIZE);
		//CARD_SPI_PRINT:"WP_GRP_ENABLE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1767_112_2_18_1_52_35_194,(uint8*)"d",CSD->SDCSD20.WP_GRP_ENABLE);
		//CARD_SPI_PRINT:"reserved4= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1768_112_2_18_1_52_35_195,(uint8*)"d",CSD->SDCSD20.reserved4);

		//CARD_SPI_PRINT:"R2W_FACTOR= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1770_112_2_18_1_52_35_196,(uint8*)"d",CSD->SDCSD20.R2W_FACTOR);
		//CARD_SPI_PRINT:"WRITE_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1771_112_2_18_1_52_35_197,(uint8*)"d",CSD->SDCSD20.WRITE_BL_LEN);
		//CARD_SPI_PRINT:"WRITE_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1772_112_2_18_1_52_35_198,(uint8*)"d",CSD->SDCSD20.WRITE_BL_PARTIAL);
		//CARD_SPI_PRINT:"reserved5= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1773_112_2_18_1_52_35_199,(uint8*)"d",CSD->SDCSD20.reserved5);

		//CARD_SPI_PRINT:"FILE_FORMAT_GRP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1775_112_2_18_1_52_35_200,(uint8*)"d",CSD->SDCSD20.FILE_FORMAT_GRP);
		//CARD_SPI_PRINT:"COPY= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1776_112_2_18_1_52_35_201,(uint8*)"d",CSD->SDCSD20.COPY);
		//CARD_SPI_PRINT:"PERM_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1777_112_2_18_1_52_35_202,(uint8*)"d",CSD->SDCSD20.PERM_WRITE_PROTECT);
		//CARD_SPI_PRINT:"TMP_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1778_112_2_18_1_52_35_203,(uint8*)"d",CSD->SDCSD20.TMP_WRITE_PROTECT);
		//CARD_SPI_PRINT:"FILE_FORMAT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1779_112_2_18_1_52_35_204,(uint8*)"d",CSD->SDCSD20.FILE_FORMAT);
		//CARD_SPI_PRINT:"reserved6= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1780_112_2_18_1_52_35_205,(uint8*)"d",CSD->SDCSD20.reserved6);
		//CARD_SPI_PRINT:"CRC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1781_112_2_18_1_52_35_206,(uint8*)"d",CSD->SDCSD20.CRC);
		//CARD_SPI_PRINT:"LastBit= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1782_112_2_18_1_52_35_207,(uint8*)"d",CSD->SDCSD20.LastBit);
		//CARD_SPI_PRINT:"---------"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1783_112_2_18_1_52_35_208,(uint8*)"");

		cardHandle->Capacity = (CSD->SDCSD20.C_SIZE+1)<<10; // (CSD.SDCSD20.C_SIZE+1)*1024 unit is 512byte

	}
	else if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
	)
	{
		_SD_CSD10_Analyze(tmpChar,&(CSD->SDCSD10));
		cardHandle->CCC = CSD->SDCSD10.CCC;

		//CARD_SPI_PRINT:"Response = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1796_112_2_18_1_52_35_209,(uint8*)"d",response[0]);

		//CARD_SPI_PRINT:"CSD_STRUCTURE = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1798_112_2_18_1_52_35_210,(uint8*)"d",CSD->SDCSD10.CSD_STRUCTURE);
		//CARD_SPI_PRINT:"reserved1= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1799_112_2_18_1_52_35_211,(uint8*)"d",CSD->SDCSD10.reserved1);
		//CARD_SPI_PRINT:"TAAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1800_112_2_18_1_52_35_212,(uint8*)"d",CSD->SDCSD10.TAAC);
		//CARD_SPI_PRINT:"NSAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1801_112_2_18_1_52_35_213,(uint8*)"d",CSD->SDCSD10.NSAC);
		//CARD_SPI_PRINT:"TRAN_SPEED= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1802_112_2_18_1_52_35_214,(uint8*)"d",CSD->SDCSD10.TRAN_SPEED);
		//CARD_SPI_PRINT:"CCC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1803_112_2_18_1_52_35_215,(uint8*)"d",CSD->SDCSD10.CCC);
		//CARD_SPI_PRINT:"READ_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1804_112_2_18_1_52_35_216,(uint8*)"d",CSD->SDCSD10.READ_BL_LEN);
		//CARD_SPI_PRINT:"READ_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1805_112_2_18_1_52_35_217,(uint8*)"d",CSD->SDCSD10.READ_BL_PARTIAL);
		//CARD_SPI_PRINT:"WRITE_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1806_112_2_18_1_52_35_218,(uint8*)"d",CSD->SDCSD10.WRITE_BLK_MISALIGN);
		//CARD_SPI_PRINT:"READ_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1807_112_2_18_1_52_35_219,(uint8*)"d",CSD->SDCSD10.READ_BLK_MISALIGN);
		//CARD_SPI_PRINT:"DSR_IMP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1808_112_2_18_1_52_35_220,(uint8*)"d",CSD->SDCSD10.DSR_IMP);

		//CARD_SPI_PRINT:"reserved2= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1810_112_2_18_1_52_35_221,(uint8*)"d",CSD->SDCSD10.reserved2);
		//CARD_SPI_PRINT:"C_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1811_112_2_18_1_52_35_222,(uint8*)"d",CSD->SDCSD10.C_SIZE);
		//CARD_SPI_PRINT:"VDD_R_CURR_MIN = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1812_112_2_18_1_52_35_223,(uint8*)"d",CSD->SDCSD10.VDD_R_CURR_MIN);
		//CARD_SPI_PRINT:"VDD_R_CURR_MAX = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1813_112_2_18_1_52_35_224,(uint8*)"d",CSD->SDCSD10.VDD_R_CURR_MAX);
		//CARD_SPI_PRINT:"VDD_W_CURR_MIN = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1814_112_2_18_1_52_35_225,(uint8*)"d",CSD->SDCSD10.VDD_W_CURR_MIN);
		//CARD_SPI_PRINT:"VDD_W_CURR_MAX = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1815_112_2_18_1_52_35_226,(uint8*)"d",CSD->SDCSD10.VDD_W_CURR_MAX);
		//CARD_SPI_PRINT:"C_SIZE_MULT = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1816_112_2_18_1_52_35_227,(uint8*)"d",CSD->SDCSD10.C_SIZE_MULT);

		//CARD_SPI_PRINT:"ERASE_BLK_EN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1818_112_2_18_1_52_35_228,(uint8*)"d",CSD->SDCSD10.ERASE_BLK_EN);
		//CARD_SPI_PRINT:"SECTOR_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1819_112_2_18_1_52_35_229,(uint8*)"d",CSD->SDCSD10.SECTOR_SIZE);
		//CARD_SPI_PRINT:"WP_GRP_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1820_112_2_18_1_52_35_230,(uint8*)"d",CSD->SDCSD10.WP_GRP_SIZE);
		//CARD_SPI_PRINT:"WP_GRP_ENABLE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1821_112_2_18_1_52_35_231,(uint8*)"d",CSD->SDCSD10.WP_GRP_ENABLE);
		//CARD_SPI_PRINT:"reserved3= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1822_112_2_18_1_52_35_232,(uint8*)"d",CSD->SDCSD10.reserved3);

		//CARD_SPI_PRINT:"R2W_FACTOR= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1824_112_2_18_1_52_35_233,(uint8*)"d",CSD->SDCSD10.R2W_FACTOR);
		//CARD_SPI_PRINT:"WRITE_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1825_112_2_18_1_52_35_234,(uint8*)"d",CSD->SDCSD10.WRITE_BL_LEN);
		//CARD_SPI_PRINT:"WRITE_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1826_112_2_18_1_52_35_235,(uint8*)"d",CSD->SDCSD10.WRITE_BL_PARTIAL);
		//CARD_SPI_PRINT:"reserved4= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1827_112_2_18_1_52_35_236,(uint8*)"d",CSD->SDCSD10.reserved4);

		//CARD_SPI_PRINT:"FILE_FORMAT_GRP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1829_112_2_18_1_52_35_237,(uint8*)"d",CSD->SDCSD10.FILE_FORMAT_GRP);
		//CARD_SPI_PRINT:"COPY= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1830_112_2_18_1_52_35_238,(uint8*)"d",CSD->SDCSD10.COPY);
		//CARD_SPI_PRINT:"PERM_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1831_112_2_18_1_52_35_239,(uint8*)"d",CSD->SDCSD10.PERM_WRITE_PROTECT);
		//CARD_SPI_PRINT:"TMP_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1832_112_2_18_1_52_35_240,(uint8*)"d",CSD->SDCSD10.TMP_WRITE_PROTECT);
		//CARD_SPI_PRINT:"FILE_FORMAT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1833_112_2_18_1_52_35_241,(uint8*)"d",CSD->SDCSD10.FILE_FORMAT);
		//CARD_SPI_PRINT:"reserved5= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1834_112_2_18_1_52_35_242,(uint8*)"d",CSD->SDCSD10.reserved5);
		//CARD_SPI_PRINT:"CRC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1835_112_2_18_1_52_35_243,(uint8*)"d",CSD->SDCSD10.CRC);
		//CARD_SPI_PRINT:"LastBit= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1836_112_2_18_1_52_35_244,(uint8*)"d",CSD->SDCSD10.LastBit);
		//CARD_SPI_PRINT:"---------"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1837_112_2_18_1_52_35_245,(uint8*)"");

		cardHandle->Capacity = (CSD->SDCSD10.C_SIZE+1) <<(CSD->SDCSD10.C_SIZE_MULT+2 + CSD->SDCSD10.READ_BL_LEN-9); //unit is 512byte

	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		_MMC_CSD_Analyze(tmpChar,&(CSD->MMCCSD));
		cardHandle->CCC = CSD->MMCCSD.CCC;
		cardHandle->CCC &= (~((1<<10)|(1<<11)));	// for MMC mode This two classes is reserved

		//CARD_SPI_PRINT:"Response = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1848_112_2_18_1_52_35_246,(uint8*)"d",response[0]);

		//CARD_SPI_PRINT:"----Read only bit---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1850_112_2_18_1_52_35_247,(uint8*)"");
		//CARD_SPI_PRINT:"CSD_STRUCTURE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1851_112_2_18_1_52_35_248,(uint8*)"d",CSD->MMCCSD.CSD_STRUCTURE);
		//CARD_SPI_PRINT:"SPEC_VERS\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1852_112_2_18_1_52_35_249,(uint8*)"d",CSD->MMCCSD.SPEC_VERS);
		//CARD_SPI_PRINT:"reserved1\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1853_112_2_18_1_52_35_250,(uint8*)"d",CSD->MMCCSD.reserved1);
		//CARD_SPI_PRINT:"TAAC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1854_112_2_18_1_52_35_251,(uint8*)"d",CSD->MMCCSD.TAAC);
		//CARD_SPI_PRINT:"NSAC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1855_112_2_18_1_52_35_252,(uint8*)"d",CSD->MMCCSD.NSAC);
		//CARD_SPI_PRINT:"TRAN_SPEED\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1856_112_2_18_1_52_35_253,(uint8*)"d",CSD->MMCCSD.TRAN_SPEED);
		//CARD_SPI_PRINT:"CCC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1857_112_2_18_1_52_35_254,(uint8*)"d",CSD->MMCCSD.CCC);
		//CARD_SPI_PRINT:"READ_BL_LEN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1858_112_2_18_1_52_35_255,(uint8*)"d",CSD->MMCCSD.READ_BL_LEN);
		//CARD_SPI_PRINT:"READ_BL_PARTIAL\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1859_112_2_18_1_52_35_256,(uint8*)"d",CSD->MMCCSD.READ_BL_PARTIAL);
		//CARD_SPI_PRINT:"WRITE_BLK_MISALIGN\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1860_112_2_18_1_52_35_257,(uint8*)"d",CSD->MMCCSD.WRITE_BLK_MISALIGN);
		//CARD_SPI_PRINT:"READ_BLK_MISALIGN\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1861_112_2_18_1_52_35_258,(uint8*)"d",CSD->MMCCSD.READ_BLK_MISALIGN);
		//CARD_SPI_PRINT:"DSR_IMP\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1862_112_2_18_1_52_35_259,(uint8*)"d",CSD->MMCCSD.DSR_IMP);
		//CARD_SPI_PRINT:"reserved2\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1863_112_2_18_1_52_35_260,(uint8*)"d",CSD->MMCCSD.reserved2);
		//CARD_SPI_PRINT:"C_SIZE\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1864_112_2_18_1_52_35_261,(uint8*)"d",CSD->MMCCSD.C_SIZE);
		//CARD_SPI_PRINT:"VDD_R_CURR_MIN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1865_112_2_18_1_52_35_262,(uint8*)"d",CSD->MMCCSD.VDD_R_CURR_MIN);
		//CARD_SPI_PRINT:"VDD_R_CURR_MAX\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1866_112_2_18_1_52_35_263,(uint8*)"d",CSD->MMCCSD.VDD_R_CURR_MAX);
		//CARD_SPI_PRINT:"VDD_W_CURR_MIN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1867_112_2_18_1_52_35_264,(uint8*)"d",CSD->MMCCSD.VDD_W_CURR_MIN);
		//CARD_SPI_PRINT:"VDD_W_CURR_MAX\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1868_112_2_18_1_52_35_265,(uint8*)"d",CSD->MMCCSD.VDD_W_CURR_MAX);
		//CARD_SPI_PRINT:"C_SIZE_MULT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1869_112_2_18_1_52_35_266,(uint8*)"d",CSD->MMCCSD.C_SIZE_MULT);
		//CARD_SPI_PRINT:"ERASE_GRP_SIZE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1870_112_2_18_1_52_35_267,(uint8*)"d",CSD->MMCCSD.ERASE_GRP_SIZE);
		//CARD_SPI_PRINT:"ERASE_GRP_MULT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1871_112_2_18_1_52_35_268,(uint8*)"d",CSD->MMCCSD.ERASE_GRP_MULT);
		//CARD_SPI_PRINT:"WP_GRP_SIZE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1872_112_2_18_1_52_35_269,(uint8*)"d",CSD->MMCCSD.WP_GRP_SIZE);
		//CARD_SPI_PRINT:"WP_GRP_ENABLE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1873_112_2_18_1_52_35_270,(uint8*)"d",CSD->MMCCSD.WP_GRP_ENABLE);
		//CARD_SPI_PRINT:"DEFAULT_ECC\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1874_112_2_18_1_52_35_271,(uint8*)"d",CSD->MMCCSD.DEFAULT_ECC);
		//CARD_SPI_PRINT:"R2W_FACTOR\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1875_112_2_18_1_52_35_272,(uint8*)"d",CSD->MMCCSD.R2W_FACTOR);
		//CARD_SPI_PRINT:"WRITE_BL_LEN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1876_112_2_18_1_52_35_273,(uint8*)"d",CSD->MMCCSD.WRITE_BL_LEN);
		//CARD_SPI_PRINT:"WRITE_BL_PARTIAL\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1877_112_2_18_1_52_35_274,(uint8*)"d",CSD->MMCCSD.WRITE_BL_PARTIAL);
		//CARD_SPI_PRINT:"reserved3\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1878_112_2_18_1_52_35_275,(uint8*)"d",CSD->MMCCSD.reserved3);
		//CARD_SPI_PRINT:"CONTENT_PROT_APP\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1879_112_2_18_1_52_35_276,(uint8*)"d",CSD->MMCCSD.CONTENT_PROT_APP);
		//CARD_SPI_PRINT:"----RW bit---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1880_112_2_18_1_52_35_277,(uint8*)"");
		//CARD_SPI_PRINT:"FILE_FORMAT_GRP\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1881_112_2_18_1_52_35_278,(uint8*)"d",CSD->MMCCSD.FILE_FORMAT_GRP);
		//CARD_SPI_PRINT:"COPY\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1882_112_2_18_1_52_35_279,(uint8*)"d",CSD->MMCCSD.COPY);
		//CARD_SPI_PRINT:"PERM_WRITE_PROTECT\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1883_112_2_18_1_52_35_280,(uint8*)"d",CSD->MMCCSD.PERM_WRITE_PROTECT);
		//CARD_SPI_PRINT:"TMP_WRITE_PROTECT\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1884_112_2_18_1_52_35_281,(uint8*)"d",CSD->MMCCSD.TMP_WRITE_PROTECT);
		//CARD_SPI_PRINT:"FILE_FORMAT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1885_112_2_18_1_52_35_282,(uint8*)"d",CSD->MMCCSD.FILE_FORMAT);
		//CARD_SPI_PRINT:"ECC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1886_112_2_18_1_52_35_283,(uint8*)"d",CSD->MMCCSD.ECC);
		//CARD_SPI_PRINT:"CRC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1887_112_2_18_1_52_35_284,(uint8*)"d",CSD->MMCCSD.CRC);
		//CARD_SPI_PRINT:"LastBit\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1888_112_2_18_1_52_35_285,(uint8*)"d",CSD->MMCCSD.LastBit);
		//CARD_SPI_PRINT:"---------"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1889_112_2_18_1_52_35_286,(uint8*)"");

		cardHandle->GrpSize = (CSD->MMCCSD.ERASE_GRP_SIZE+1)*(CSD->MMCCSD.ERASE_GRP_MULT+1)*(1 << CSD->MMCCSD.WRITE_BL_LEN);
		cardHandle->Capacity = (CSD->MMCCSD.C_SIZE+1) <<(CSD->MMCCSD.C_SIZE_MULT+2 + CSD->MMCCSD.READ_BL_LEN - 9); //unit is 512 byte

	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	_GetNac(cardHandle,CSD);
	//CARD_SPI_PRINT:"Nac_Max = 0x%x %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1900_112_2_18_1_52_35_287,(uint8*)"dd",(uint32)((cardHandle->Nac_Max>>32)&0xFFFFFFFF),(uint32)(cardHandle->Nac_Max&0xFFFFFFFF));
	//CARD_SPI_PRINT:"Nac_T = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1901_112_2_18_1_52_35_288,(uint8*)"d",cardHandle->Nac_T);
	//CARD_SPI_PRINT:"clock 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1902_112_2_18_1_52_35_289,(uint8*)"d",cardHandle->ClockRate);

//--
	if(FALSE == result)
	{
		cardHandle->CCC = 0;
	}
//--
	return result;
}

/*

*/


typedef struct
{
	uint16	currentVal;
	uint16	grp6Supported;
	uint16	grp5Supported;
	uint16	grp4Supported;
	uint16	grp3Supported;
	uint16	grp2Supported;
	uint16	grp1Supported;

	uint8	grp6SwitchResult;
	uint8	grp5SwitchResult;
	uint8	grp4SwitchResult;
	uint8	grp3SwitchResult;
	uint8	grp2SwitchResult;
	uint8	grp1SwitchResult;

	uint8	version;

	uint16	grp6BusyStatus;
	uint16	grp5BusyStatus;
	uint16	grp4BusyStatus;
	uint16	grp3BusyStatus;
	uint16	grp2BusyStatus;
	uint16	grp1BusyStatus;

}CARD_CMD6_STATUS_T;


LOCAL void _CMD6_Response_Analyze(uint8* resBuf,CARD_CMD6_STATUS_T* cmdStatus)
{
	cmdStatus->currentVal		= ((resBuf[0]<<8)|(resBuf[1]));
	cmdStatus->grp6Supported	= ((resBuf[2]<<8)|(resBuf[3]));
	cmdStatus->grp5Supported	= ((resBuf[4]<<8)|(resBuf[5]));
	cmdStatus->grp4Supported	= ((resBuf[6]<<8)|(resBuf[7]));
	cmdStatus->grp3Supported	= ((resBuf[8]<<8)|(resBuf[9]));
	cmdStatus->grp2Supported	= ((resBuf[10]<<8)|(resBuf[11]));
	cmdStatus->grp1Supported	= ((resBuf[12]<<8)|(resBuf[13]));

	cmdStatus->grp6SwitchResult	= ((resBuf[14]&0xF0)>>4);
	cmdStatus->grp5SwitchResult	= (resBuf[14]&0x0F);
	cmdStatus->grp4SwitchResult	= ((resBuf[15]&0xF0)>>4);
	cmdStatus->grp3SwitchResult	= (resBuf[15]&0x0F);
	cmdStatus->grp2SwitchResult	= ((resBuf[16]&0xF0)>>4);
	cmdStatus->grp1SwitchResult	= (resBuf[16]&0x0F);

	cmdStatus->version		= resBuf[17];

	cmdStatus->grp6BusyStatus	= ((resBuf[18]<<8)|(resBuf[19]));
	cmdStatus->grp5BusyStatus	= ((resBuf[20]<<8)|(resBuf[21]));
	cmdStatus->grp4BusyStatus	= ((resBuf[22]<<8)|(resBuf[23]));
	cmdStatus->grp3BusyStatus	= ((resBuf[24]<<8)|(resBuf[25]));
	cmdStatus->grp2BusyStatus	= ((resBuf[26]<<8)|(resBuf[27]));
	cmdStatus->grp1BusyStatus	= ((resBuf[28]<<8)|(resBuf[29]));

	//CARD_SPI_PRINT:"currentVal = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1972_112_2_18_1_52_35_290,(uint8*)"d",cmdStatus->currentVal);

	//CARD_SPI_PRINT:"grp6Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1974_112_2_18_1_52_35_291,(uint8*)"d",cmdStatus->grp6Supported);
	//CARD_SPI_PRINT:"grp5Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1975_112_2_18_1_52_35_292,(uint8*)"d",cmdStatus->grp5Supported);
	//CARD_SPI_PRINT:"grp4Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1976_112_2_18_1_52_35_293,(uint8*)"d",cmdStatus->grp4Supported);
	//CARD_SPI_PRINT:"grp3Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1977_112_2_18_1_52_35_294,(uint8*)"d",cmdStatus->grp3Supported);
	//CARD_SPI_PRINT:"grp2Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1978_112_2_18_1_52_35_295,(uint8*)"d",cmdStatus->grp2Supported);
	//CARD_SPI_PRINT:"grp1Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1979_112_2_18_1_52_35_296,(uint8*)"d",cmdStatus->grp1Supported);

	//CARD_SPI_PRINT:"grp6SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1981_112_2_18_1_52_35_297,(uint8*)"d",cmdStatus->grp6SwitchResult);
	//CARD_SPI_PRINT:"grp5SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1982_112_2_18_1_52_35_298,(uint8*)"d",cmdStatus->grp5SwitchResult);
	//CARD_SPI_PRINT:"grp4SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1983_112_2_18_1_52_35_299,(uint8*)"d",cmdStatus->grp4SwitchResult);
	//CARD_SPI_PRINT:"grp3SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1984_112_2_18_1_52_35_300,(uint8*)"d",cmdStatus->grp3SwitchResult);
	//CARD_SPI_PRINT:"grp2SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1985_112_2_18_1_52_35_301,(uint8*)"d",cmdStatus->grp2SwitchResult);
	//CARD_SPI_PRINT:"grp1SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1986_112_2_18_1_52_35_302,(uint8*)"d",cmdStatus->grp1SwitchResult);

	//CARD_SPI_PRINT:"status version = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1988_112_2_18_1_52_35_303,(uint8*)"d",cmdStatus->version);

	//CARD_SPI_PRINT:"grp6BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1990_112_2_18_1_52_36_304,(uint8*)"d",cmdStatus->grp6BusyStatus);
	//CARD_SPI_PRINT:"grp5BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1991_112_2_18_1_52_36_305,(uint8*)"d",cmdStatus->grp5BusyStatus);
	//CARD_SPI_PRINT:"grp4BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1992_112_2_18_1_52_36_306,(uint8*)"d",cmdStatus->grp4BusyStatus);
	//CARD_SPI_PRINT:"grp3BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1993_112_2_18_1_52_36_307,(uint8*)"d",cmdStatus->grp3BusyStatus);
	//CARD_SPI_PRINT:"grp2BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1994_112_2_18_1_52_36_308,(uint8*)"d",cmdStatus->grp2BusyStatus);
	//CARD_SPI_PRINT:"grp1BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_1995_112_2_18_1_52_36_309,(uint8*)"d",cmdStatus->grp1BusyStatus);

	return;
}
#if defined(SPRD_SUPPORT_MCEX)

LOCAL BOOLEAN ifSupportMcex(CARD_CARD_T*cardHandle)
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifSupportEC = FALSE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}

	if(0 == ((1<<10)&cardHandle->CCC))
	{
		return TRUE;
	}

	Spi_Card_Pal_SetClk(cardHandle->spiHandle,cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD6_SWITCH_FUNC, 0x00FFFF1F,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0x04 == response[0])
	{
		_DisableCARD(cardHandle);
		return TRUE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START,buf,64, &CRC,FALSE);
//--
 	_DisableCARD(cardHandle);
	if(FALSE == result)
	{
		return result;
	}
	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x01 == cmdStatus.grp2SwitchResult)
		&&(0 != ((0x1<<1)&(cmdStatus.grp2Supported)))
	)
	{
		cardHandle->ifSupportEC = TRUE;
	}
	return TRUE;
}



#endif


/********************************************************************
*	Description: This function is used to initialize a SD card internal function
*	Dependence: the SD must have run CARD_SPI_Open success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_InitCard(CARD_CARD_T*cardHandle)
{
	uint8 response[5];
	uint32 OCR;
	CARD_CSD_STRUCT CSD;
	BOOLEAN vertion_flag; //TRUE: SD2.0 FALSE: SD1.x
	uint32 time_out = 0;

	//CARD_SPI_PRINT:"CARD_SPI_InitCard"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2080_112_2_18_1_52_36_310,(uint8*)"");
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
//	if(FALSE == Spi_Card_Pal_PwrPreprocess(cardHandle->spiHandle, SPI_CARD_PAL_PWR_ON))
//	{
//		CARD_SPI_ASSERT(0);
//	}

	cardHandle->vertion = CARD_V_UNKONWN;
	//pull up cs and set default clk
	Spi_Card_Pal_SetClk(cardHandle->spiHandle,CARD_DEFAULT_CLOCK);
	_DisableCARD(cardHandle);
	_SendCARDIdleClk(cardHandle, 100);
//--send CMD0 and get response
	time_out = SCI_GetTickCount();
	do
	{
		_EnableCARD(cardHandle);
		if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD0_GO_IDLE_STATE,0,response))
		{
			_DisableCARD(cardHandle);
			return FALSE;
		}
		_DisableCARD(cardHandle);
		if(CARD_TRY_TIMES <= (SCI_GetTickCount() - time_out) )
		{
			//CARD_SPI_PRINT:"Card waitting for CMD0's response is time out"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2108_112_2_18_1_52_36_311,(uint8*)"");
			return FALSE;
		}
	}
	while(0x01 != (response[0]&0xFF));
//--
//	CARD_SPI_PRINT(("response0 = %x\r\n",response[0]));
//---send CMD8 to complete initialization process
	time_out = SCI_GetTickCount();
	do
	{
		vertion_flag = TRUE;

		_EnableCARD(cardHandle);
		if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD8_SEND_IF_COND, 0x000001AA,response))
		{
			//SD2.0 not support current voltage
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"SD2.0 not response to CMD8,SD2.0 not surpport current operation voltage range"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2126_112_2_18_1_52_36_312,(uint8*)"");
			return FALSE;
		}
		_DisableCARD(cardHandle);
		if( 0 != (response[0]&0x04))
		{
			//not SD2.0 card,maybe it is SD1.1 card or MMC card or others
			vertion_flag = FALSE;
			//CARD_SPI_PRINT:"not SD2.0 Card ,Maybe it is SD1.x Card or MMC card"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2134_112_2_18_1_52_36_313,(uint8*)"");
			break;
		}
		if((0xAA == response[4])&&(0x00 == response[3]))
		{
			//SD2.0 not support current voltage
			//CARD_SPI_PRINT:"VCA = 0, SD2.0 not surpport current operation voltage range"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2140_112_2_18_1_52_36_314,(uint8*)"");
			return FALSE;
		}
		if(200 <= (SCI_GetTickCount() - time_out) )	
		{
			//CARD_SPI_PRINT:"SD2.0 waitting for CMD8's response is time out"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2145_112_2_18_1_52_36_315,(uint8*)"");
#ifdef CARD_INIT_STRICTLY
			return FALSE;
#else
			break;
#endif
		}
	}
	while(0xAA != response[4]);
//--
	if(FALSE == vertion_flag) // It is v1.x card
	{
		do
		{
	//--Read OCR
		_EnableCARD(cardHandle);
		if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD58_READ_OCR, 0,response))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It maybe SD1.x or MMC But it is Not response to CMD58 "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2164_112_2_18_1_52_36_316,(uint8*)"");
			return FALSE;
		}
		if(0 != (response[0]&0xFE))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It look CMD58 as illegal command so it is not SD card"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2170_112_2_18_1_52_36_317,(uint8*)"");
			break;
		}
		_DisableCARD(cardHandle);
		OCR = response[1];
		OCR = (OCR<<8)+response[2];
		OCR = (OCR<<8)+response[3];
		OCR = (OCR<<8)+response[4];
		//CARD_SPI_PRINT:"OCR is 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2178_112_2_18_1_52_36_318,(uint8*)"d",OCR);
		if( 0 == (OCR & (0x1 << OPVOLTAGE)))
		{
			//CARD_SPI_PRINT:"SD 1.x But not surpport current voltage"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2181_112_2_18_1_52_36_319,(uint8*)"");
			return FALSE;
		}

	//--Send ACMD41 to make card ready
		time_out = SCI_GetTickCount();
		_EnableCARD(cardHandle);
		do 
		{
			if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD55_APP_CMD,0,response))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card1"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2193_112_2_18_1_52_36_320,(uint8*)"");
				return FALSE;
			}
			if(0 != (response[0]&0xFE))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card2 , may be MMC"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2199_112_2_18_1_52_36_321,(uint8*)"");
				break;
			}
			if(FALSE == _SendCARDcmd(cardHandle,CARD_ACMD41_SD_SEND_OP_COND,0x00000000,response))//send acmd 41 with param 0
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card3"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2205_112_2_18_1_52_36_322,(uint8*)"");
				return FALSE;
			}
			if(0 != (response[0]&0xFE))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card4 , may be MMC"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2211_112_2_18_1_52_36_323,(uint8*)"");
				break;
			}
			if(0 == (response[0]&0xFF))
			{
				_DisableCARD(cardHandle);
				cardHandle->vertion = CARD_SD_V1_X;
				//CARD_SPI_PRINT:"It is SD1.x Card!!!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2218_112_2_18_1_52_36_324,(uint8*)"");
				break;
			}
			if(CARD_TRY_TIMES <= (SCI_GetTickCount() - time_out))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"SD2.0 waitting for ACMD41's response is time out"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2224_112_2_18_1_52_36_325,(uint8*)"");
				return FALSE;
			}
		}
		while(1); /*lint !e506*/
		break;
		}
		while(1); /*lint !e506*/
		if(CARD_V_UNKONWN == cardHandle->vertion)
		{
			//pull up cs and set default clk
			Spi_Card_Pal_SetClk(cardHandle->spiHandle,CARD_DEFAULT_CLOCK);
			_DisableCARD(cardHandle);
			_SendCARDIdleClk(cardHandle, 10000);
		//--send CMD0 and get response
			time_out = SCI_GetTickCount();
			do
			{
				_EnableCARD(cardHandle);
				if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD0_GO_IDLE_STATE,0,response))
				{
					_DisableCARD(cardHandle);
					return FALSE;
				}
				_DisableCARD(cardHandle);
				if(CARD_TRY_TIMES <= (SCI_GetTickCount() - time_out) )	
				{
					//CARD_SPI_PRINT:"MMC CMD0 response is time out"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2251_112_2_18_1_52_36_326,(uint8*)"");
					return FALSE;
				}
			}
			while(0x01 != (response[0]&0xFF));

		//--
		//	CARD_SPI_PRINT(("response0 = %x\r\n",response[0]));
		//---send CMD1 to complete initialization process
			time_out = SCI_GetTickCount();
			do
			{
				_EnableCARD(cardHandle);
				if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD1_SEND_OP_COND, 0,response))
				{
					_DisableCARD(cardHandle);
					return FALSE;
				}
				_DisableCARD(cardHandle);
				if(CARD_TRY_TIMES <= (SCI_GetTickCount() - time_out))
				{
					//CARD_SPI_PRINT:"MMC CMD1 response is time out"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2272_112_2_18_1_52_36_327,(uint8*)"");
					return FALSE;
				}
			}
			while(0 != response[0]);
			cardHandle->vertion = CARD_MMC_331;
		}
	}
	else // maybe it is v2.0 high capacity card or v2.0 standard card
	{
	//--Read OCR
		_EnableCARD(cardHandle);
		if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD58_READ_OCR, 0,response))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It maybe SD2.0 But it is Not response to CMD58 "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2287_112_2_18_1_52_36_328,(uint8*)"");
			return FALSE;
		}
		if(0 != (response[0]&0xFE))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It look CMD58 as illegal command so it is not SD card"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2293_112_2_18_1_52_36_329,(uint8*)"");
			return FALSE;
		}
		_DisableCARD(cardHandle);

		OCR = response[1];
		OCR = (OCR<<8)+response[2];
		OCR = (OCR<<8)+response[3];
		OCR = (OCR<<8)+response[4];
		if( 0 == (OCR & (0x1 << OPVOLTAGE)))
		{
			//CARD_SPI_PRINT:"SD 2.0 But not surpport current voltage"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2304_112_2_18_1_52_36_330,(uint8*)"");
			return FALSE;
		}
	//--Send ACMD41 to make card ready
		time_out = SCI_GetTickCount();
		_EnableCARD(cardHandle);
		do 
		{
			if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD55_APP_CMD,0,response))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card5"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2315_112_2_18_1_52_36_331,(uint8*)"");
				return FALSE;
			}
			if(0 != (response[0]&0xFE))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card6"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2321_112_2_18_1_52_36_332,(uint8*)"");
				return FALSE;
			}
			if(FALSE == _SendCARDcmd(cardHandle,CARD_ACMD41_SD_SEND_OP_COND,0x40000000,response))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card7"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2327_112_2_18_1_52_36_333,(uint8*)"");
				return FALSE;
			}
			if(0 != (response[0]&0xFE))
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"Not SD card8"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2333_112_2_18_1_52_36_334,(uint8*)"");
				return FALSE;
			}
			if(CARD_TRY_TIMES <= (SCI_GetTickCount() - time_out) )	
			{
				_DisableCARD(cardHandle);
				//CARD_SPI_PRINT:"SD2.0 ACMD41's response is time out"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2339_112_2_18_1_52_36_335,(uint8*)"");
				return FALSE;
			}
		}
		while(0 != (response[0]&0xFF));
		_DisableCARD(cardHandle);
	//--Read OCR again
		_EnableCARD(cardHandle);
		if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD58_READ_OCR, 0,response))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It maybe SD2.0 But it is Not response to 2nd CMD58 "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2350_112_2_18_1_52_36_336,(uint8*)"");
			return FALSE;
		}
		if(0 != (response[0]&0xFE))
		{
			_DisableCARD(cardHandle);
			//CARD_SPI_PRINT:"It look 2nd CMD58 as illegal command so it is not SD card"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2356_112_2_18_1_52_36_337,(uint8*)"");
			return FALSE;
		}
		_DisableCARD(cardHandle);

		OCR = response[1];
		OCR = (OCR<<8)+response[2];
		OCR = (OCR<<8)+response[3];
		OCR = (OCR<<8)+response[4];

		if(0 != ((OCR & 0x40000000)))
		{
			//CARD_SPI_PRINT:"It is SD2.0 high capacity Card!!!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2368_112_2_18_1_52_36_338,(uint8*)"");
			cardHandle->vertion = CARD_SD_V2_0_HIGHCAP;
		}
		else
		{
			//CARD_SPI_PRINT:"It is SD2.0 standard capacity Card!!!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2373_112_2_18_1_52_36_339,(uint8*)"");
			cardHandle->vertion = CARD_SD_V2_0_STANDARD;
		}
	}
// Now the initialization is completed
//--
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);

#if defined(CARD_CHK_CRC)

	_EnableCARD(cardHandle);
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD59_CRC_ON_OFF, 0x00000001,response))
	{
		_DisableCARD(cardHandle);
		//CARD_SPI_PRINT:"Turn on CRC fail 1"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2387_112_2_18_1_52_36_340,(uint8*)"");
		return FALSE;
	}
	_DisableCARD(cardHandle);
	if(0 != (response[0]&0xFE))
	{
		//CARD_SPI_PRINT:"Turn on CRC fail 2"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2393_112_2_18_1_52_36_341,(uint8*)"");
		return FALSE;
	}
#endif

	if(FALSE == CARD_SPI_ReadCSD(cardHandle, &CSD))
	{
//		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
//		_DisableCARD(cardHandle);
		return FALSE;
	}
//--
#if defined(SPRD_SUPPORT_MCEX)
	if(FALSE == ifSupportMcex(cardHandle))
	{
		return FALSE;
	}
	if(TRUE == cardHandle->ifEnterEC)
	{
		BOOLEAN ifEnterMcex;
		if(FALSE == CARD_SPI_EnterMcex(cardHandle,&ifEnterMcex))
		{
			return FALSE;
		}
	}
#endif
	if(!(
		(((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))&&(0 == CSD.SDCSD10.CSD_STRUCTURE))
		||((CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)&&(1 == CSD.SDCSD20.CSD_STRUCTURE))
		||((CARD_MMC_331 == cardHandle->vertion))
	))
	{
		return FALSE;
	}


	return TRUE;

}

/********************************************************************
*	Description: This function is used to Read a single block of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		BlockId: the number of block in SD
*		buf: the space used to save the value that readout from SD
*	Result:
*		TRUE: Success
*		FALSE: Fail
*	Note:
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_ReadSingleBlock(CARD_CARD_T*cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = BlockId*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = BlockId;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
	_EnableCARD(cardHandle);
//--
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD17_READ_SINGLE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
 	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START,buf,cardHandle->BlockLen, &CRC,FALSE);
//--
	_DisableCARD(cardHandle);
 //--
//	CARD_SPI_PRINT(("Response = %x\r\n",response[0]));
//--
	return result;
}

/********************************************************************
*	Description: This function is used to Write a single block of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		BlockId: the Id of block in SD
*		buf: the space used to save the value that is to write to SD
*	Result:
*		TRUE: Success
*		FALSE: Fail
*	Note:
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_WriteSingleBlock(CARD_CARD_T*cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 response[5];
	BOOLEAN result = TRUE;
	uint8 dataRsp;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = BlockId*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = BlockId;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
//--
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
	_EnableCARD(cardHandle);
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD24_WRITE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//--
	result = _WriteCARDDataToken(cardHandle,CARD_SINGLE_WRITE_START,buf,cardHandle->BlockLen,&dataRsp);
//--
	_DisableCARD(cardHandle);
//--
	return result;

}

LOCAL BOOLEAN _StopCARDMultiRead(CARD_CARD_T*cardHandle)
{
	uint8 response[5];
	BOOLEAN result = TRUE;

//send stop command
	result = _SendCARDcmd(cardHandle,CARD_CMD12_STOP_TRANSMISSION_SD, 0,response);
//	if(0 != response[0])
//	{
//		result = FALSE;
//	}
//--
	return result;
}

/********************************************************************
*	Description: This function is used to Read serval number continue blocks from SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startBlock: that the first block Id you want to read
*		num: the number blocks that you want to read from SD
*		buf: the space used to save the value that readout from SD
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_ReadMultiBlock(CARD_CARD_T*cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint32 i;
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}

	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//send multiread command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD18_READ_MULTIPLE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//read data
	for(i = 0; i < num; i++)
	{
		result = _ReadCARDDataToken(cardHandle,CARD_MULTI_READ_START,buf+(i * cardHandle->BlockLen),cardHandle->BlockLen ,&CRC,FALSE);
		if(FALSE == result)
		{
			break;
		}
	}
//send stop command
	if(FALSE == _StopCARDMultiRead(cardHandle))
	{
		result = FALSE;
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}

PUBLIC BOOLEAN CARD_SPI_ReadMultiBlock_Ext(CARD_SPI_HANDLE cardHandle,uint32 startBlock,CARD_SPI_SCTBUF_INFO_T* sctInfo)
{
	uint32 i,k;
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}

	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//send multiread command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD18_READ_MULTIPLE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//read data
	for(k = 0;  k < sctInfo->cnt; k++)
	{
		for(i = 0; i < sctInfo->item[k].num; i++)
		{
			result = _ReadCARDDataToken(cardHandle,CARD_MULTI_READ_START,sctInfo->item[k].buf+(i * cardHandle->BlockLen),cardHandle->BlockLen ,&CRC,FALSE);
			if(FALSE == result)
			{
				break;
			}
		}
	}
//send stop command
	if(FALSE == _StopCARDMultiRead(cardHandle))
	{
		result = FALSE;
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}



LOCAL void _StopCARDMultiWrite(CARD_CARD_T*cardHandle)
{
	uint32 time_out;
	BOOLEAN ifTimeOut;
	uint8 tmpChar;
#if defined(CARD_SPEEDUP)
	uint32 i;
	uint8 tmpBuf[CARD_NWR+1+CARD_NBR];
	for(i = 0; i < CARD_NWR+1+CARD_NBR; i++)
	{
		tmpBuf[i] = 0xFF;
	}
	tmpBuf[CARD_NWR] = CARD_STOP_TRAN;
	Spi_Card_Pal_Write(cardHandle->spiHandle, tmpBuf,CARD_NWR+1+CARD_NBR);
#else
	_WaitTmVal(cardHandle, CARD_NWR);
	tmpChar = CARD_STOP_TRAN;
	Spi_Card_Pal_Write(cardHandle->spiHandle, &tmpChar,1);
	_WaitTmVal(cardHandle, CARD_NBR);
#endif
	time_out = SCI_GetTickCount();
	ifTimeOut = FALSE;
	do
	{
		if(CARD_WD_BUSY_TIMEOUT <= (SCI_GetTickCount() - time_out))
		{
			//CARD_SPI_PRINT:"card write stop busy signal timeout"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_2727_112_2_18_1_52_37_342,(uint8*)"");
			ifTimeOut = TRUE;
		}
		//--------------
		Spi_Card_Pal_Read(cardHandle->spiHandle,&tmpChar,1);
		if(0xFF == tmpChar)
		{
			break;
		}
		//--------------
		if(TRUE == ifTimeOut)
		{
			break;
		}
	}
	while(1);
}

/********************************************************************
*	Description: This function is used to write serval number continue blocks to SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startBlock: that the first block Id you want to write
*		num: the number blocks that you want to write to SD
*		buf: the space used to save the value that will be writen to SD
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_WriteMultiBlock(CARD_CARD_T*cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 response[5];
	uint32 i;
	BOOLEAN result = TRUE;
	uint8 dataRsp;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//send multi write command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD25_WRITE_MULTIPLE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//write data
	for(i = 0; i < num; i++)
	{
		result = _WriteCARDDataToken(cardHandle,CARD_MULTI_WRITE_START,buf+(i * cardHandle->BlockLen),cardHandle->BlockLen,&dataRsp);
		if(FALSE == result)
		{
			break;
		}
	}
//send stop tran token
#if 0
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD_STOP_WRITING, 0,response))
	{
		result = FALSE;
	}
	if(0 != response[0])
	{
		result = FALSE;
	}
#else
	_StopCARDMultiWrite(cardHandle);
#endif
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}


PUBLIC BOOLEAN CARD_SPI_WriteMultiBlock_Ext(CARD_SPI_HANDLE cardHandle,uint32 startBlock,CARD_SPI_SCTBUF_INFO_T* sctInfo)
{
	uint8 response[5];
	uint32 i,k;
	BOOLEAN result = TRUE;
	uint8 dataRsp;
	uint32 address = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//send multi write command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD25_WRITE_MULTIPLE_BLOCK, address,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//write data
	for(k = 0;  k < sctInfo->cnt; k++)
	{
		for(i = 0; i < sctInfo->item[k].num; i++)
		{
			result = _WriteCARDDataToken(cardHandle,CARD_MULTI_WRITE_START,sctInfo->item[k].buf+(i * cardHandle->BlockLen),cardHandle->BlockLen,&dataRsp);
			if(FALSE == result)
			{
				break;
			}
		}
	}
//send stop tran token
#if 0
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD_STOP_WRITING, 0,response))
	{
		result = FALSE;
	}
	if(0 != response[0])
	{
		result = FALSE;
	}
#else
	_StopCARDMultiWrite(cardHandle);
#endif
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}


LOCAL BOOLEAN _SetMMCBlockCount(CARD_CARD_T*cardHandle,uint32 count)
{
	uint8 response[5];
	BOOLEAN result = TRUE;

	result = _SendCARDcmd(cardHandle,CARD_CMD23_SET_BLOCK_COUNT, count,response);
	if(0 != response[0])
	{
		result = FALSE;
	}
 	return result;
}


/********************************************************************
*	Description: This function is used to Read serval number continue blocks from MMC card
*	Dependence: the MMC must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startBlock: that the first block Id you want to read
*		num: the number blocks that you want to read from MMC
*		buf: the space used to save the value that readout from MMC
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_MMC_ReadMultiBlock_PreSetNum(CARD_CARD_T*cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint32 i;
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(CARD_MMC_331 != cardHandle->vertion)
	{
		CARD_SPI_ASSERT(0);
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//set num that want to read
	if(FALSE == _SetMMCBlockCount(cardHandle,num))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//send multiread command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD18_READ_MULTIPLE_BLOCK, startBlock*cardHandle->BlockLen,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//read data
	for(i = 0; i < num; i++)
	{
		result = _ReadCARDDataToken(cardHandle,CARD_MULTI_READ_START,buf+(i * cardHandle->BlockLen),cardHandle->BlockLen ,&CRC,FALSE);
		if(FALSE == result)
		{
			_StopCARDMultiRead(cardHandle);
			break;
		}
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}


/********************************************************************
*	Description: This function is used to write serval number continue blocks to MMC card
*	Dependence: the MMC must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startBlock: that the first block Id you want to write
*		num: the number blocks that you want to write to MMC
*		buf: the space used to save the value that will be writen to MMC
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_MMC_WriteMultiBlock_PreSetNum(CARD_CARD_T*cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 response[5];
	uint32 i;
	BOOLEAN result = TRUE;
	uint8 dataRsp;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(CARD_MMC_331 != cardHandle->vertion)
	{
		CARD_SPI_ASSERT(0);
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//set num that want to write
	if(FALSE == _SetMMCBlockCount(cardHandle,num))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//send multi write command
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD25_WRITE_MULTIPLE_BLOCK, startBlock*cardHandle->BlockLen,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//write data
	for(i = 0; i < num; i++)
	{
		result = _WriteCARDDataToken(cardHandle,CARD_MULTI_WRITE_START,buf+(i * cardHandle->BlockLen),cardHandle->BlockLen,&dataRsp);
		if(FALSE == result)
		{
			_StopCARDMultiWrite(cardHandle);
			break;
		}
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}




LOCAL BOOLEAN _CARD_SetEraseStartBlock(CARD_CARD_T*cardHandle,uint32 startBlock)
{
	uint8 response[5];

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD32_ERASE_WR_BLK_START, startBlock,response))
	{
		return FALSE;
	}
	if(0 != response[0])
	{
		return FALSE;
	}

 	return TRUE;
}

LOCAL BOOLEAN _CARD_SetEraseEndBlock(CARD_CARD_T*cardHandle,uint32 endBlock)
{
	uint8 response[5];

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD33_ERASE_WR_BLK_END, endBlock,response))
	{
		return FALSE;
	}
	if(0 != response[0])
	{
		return FALSE;
	}
 	return TRUE;
}

/********************************************************************
*	Description: This function is used to erase the group in SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startGrpId: the first group id
*		endGrpId: the last group id
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_Erase(CARD_CARD_T*cardHandle,uint32 startBlock,uint32 endBlock)
{
	uint8 response[5];
	BOOLEAN result = TRUE;
	uint32 startAddress = 0xFFFFFFFF, endAddress = 0xFFFFFFFF;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion))
	{
		startAddress = startBlock*cardHandle->BlockLen;
		endAddress = endBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	{
		startAddress = startBlock;
		endAddress = endBlock;
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		return TRUE;
	}
	else
	{
		CARD_SPI_ASSERT(0);
	}

	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//set start group Id
	if(FALSE == _CARD_SetEraseStartBlock(cardHandle,startAddress))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//set end group Id
	if(FALSE == _CARD_SetEraseEndBlock(cardHandle,endAddress))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//send erase command
	result = _SendCARDcmd(cardHandle,CARD_CMD38_ERASE, 0,response);
	if(0 != response[0])
	{
		result = FALSE;
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}

/********************************************************************
*	Description: get the capacity of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*	Result:
*		None zero:the Capacity, unit is 512 byte
*		0: Fail
********************************************************************/
PUBLIC uint32 CARD_SPI_GetCapacity(CARD_CARD_T*cardHandle)
{
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return 0;
	}
	return cardHandle->Capacity;
}

/********************************************************************
*	Description: get the group size of MMC card
*	Dependence: the CARD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*	Result:
*		None zero:the group size, unit is byte
*		0: Fail
********************************************************************/
LOCAL uint32 CARD_SPI_MMC_GetGrpSize(CARD_CARD_T*cardHandle)
{
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return 0;
	}
	return cardHandle->GrpSize;
}


LOCAL BOOLEAN _CARD_SetEraseStartGrp(CARD_CARD_T*cardHandle,uint32 startGrpId)
{
	uint8 response[5];

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD35_ERASE_GROUP_START, startGrpId*cardHandle->GrpSize,response))
	{
		return FALSE;
	}
	if(0 != response[0])
	{
		return FALSE;
	}

 	return TRUE;
}

LOCAL BOOLEAN _CARD_SetEraseEndGrp(CARD_CARD_T*cardHandle,uint32 endGrpId)
{
	uint8 response[5];

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD36_ERASE_GROUP_END, endGrpId*cardHandle->GrpSize,response))
	{
		return FALSE;
	}
	if(0 != response[0])
	{
		return FALSE;
	}
 	return TRUE;
}

/********************************************************************
*	Description: This function is used to erase the group in MMC card
*	Dependence: the MMC must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*		startGrpId: the first group id
*		endGrpId: the last group id
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
LOCAL BOOLEAN CARD_SPI_MMC_Erase(CARD_CARD_T*cardHandle,uint32 startGrpId,uint32 endGrpId)
{
	uint8 response[5];
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);
//set start group Id
	if(FALSE == _CARD_SetEraseStartGrp(cardHandle,startGrpId))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//set end group Id
	if(FALSE == _CARD_SetEraseEndGrp(cardHandle,endGrpId))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//send erase command
	result = _SendCARDcmd(cardHandle,CARD_CMD38_ERASE, 0,response);
	if(0 != response[0])
	{
		result = FALSE;
	}
//--
 	_DisableCARD(cardHandle);
	return result;
 //--
}

#if defined(SPRD_SUPPORT_MCEX)

#define ENTER_MCEX	0x80FFFF1F
PUBLIC BOOLEAN CARD_SPI_EnterMcex(CARD_CARD_T*cardHandle,BOOLEAN* ifEnterMcex )
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifEnterEC = FALSE;
	*ifEnterMcex = FALSE;
	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifSupportEC)
	{
		return TRUE;
	}

	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD6_SWITCH_FUNC, ENTER_MCEX,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//	CARD_SPI_PRINT(("response is 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",response));
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	// question : if need command 12
	// start token is CARD_SINGLE_READ_START?
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START,buf,64, &CRC,FALSE);
//--
 	_DisableCARD(cardHandle);
	if(FALSE == result)
	{
		return result;
	}
	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x01 == cmdStatus.grp2SwitchResult)
		&&(0 != ((0x1<<1)&(cmdStatus.grp2Supported)))
	)
	{
		cardHandle->ifEnterEC = TRUE;
		*ifEnterMcex = TRUE;
	}

	SCI_Sleep(500);
	return result;

}


PUBLIC BOOLEAN CARD_SPI_SendPsi(CARD_CARD_T*cardHandle,uint32 arg,uint8* buf)
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;		/*lint !e527*/
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,32))
	{
		return FALSE;
	}
	_EnableCARD(cardHandle);
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD36_SEND_PSI, arg,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START, buf,32,&CRC,FALSE);

	_DisableCARD(cardHandle);
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}
	return TRUE;

}

PUBLIC BOOLEAN CARD_SPI_ReadSecCmd(CARD_CARD_T*cardHandle,uint8* buf)
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;		/*lint !e527*/
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,512))
	{
		return FALSE;
	}
	_EnableCARD(cardHandle);
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD34_READ_SEC_CMD, 1,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START, buf,512,&CRC,FALSE);
	_DisableCARD(cardHandle);
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

	return result;

}

PUBLIC BOOLEAN CARD_SPI_WriteSecCmd(CARD_CARD_T*cardHandle,uint8* buf)
{
	uint8 response[5];
	BOOLEAN result = TRUE;
	uint8 dataRsp;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;		/*lint !e527*/
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC

	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,512))
	{
		return FALSE;
	}
	_EnableCARD(cardHandle);
	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD35_WRITE_SEC_CMD, 1,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	result = _WriteCARDDataToken(cardHandle,CARD_MULTI_WRITE_START,buf,512,&dataRsp);

	_DisableCARD(cardHandle);
	if(FALSE == CARD_SPI_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

	return result;

}

PUBLIC BOOLEAN CARD_SPI_CtlTrm(CARD_CARD_T*cardHandle)
{
	uint8 response[5];

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;		/*lint !e527*/
	}
	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD37_CTL_TRM, 1,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	_DisableCARD(cardHandle);
	SCI_Sleep(500);
	return TRUE;

}

#define EXIT_MCEX	0x80FFFF0F
PUBLIC BOOLEAN CARD_SPI_ExitMcex(CARD_CARD_T*cardHandle)
{
	uint8 response[5];
	uint16 CRC;
	BOOLEAN result = TRUE;
	uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	if(FALSE == _RegisterVerifyCARD(cardHandle))
	{
		return FALSE;
	}
	if(FALSE == cardHandle->ifEnterEC)
	{
		return TRUE;
	}

	cardHandle->ifEnterEC = FALSE;

	Spi_Card_Pal_SetClk(cardHandle->spiHandle, cardHandle->ClockRate);
//enable MMC
	_EnableCARD(cardHandle);

	if(FALSE == _SendCARDcmd(cardHandle,CARD_CMD6_SWITCH_FUNC, EXIT_MCEX,response))
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
//	CARD_SPI_PRINT(("response is 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",response));
	if(0 != response[0])
	{
		_DisableCARD(cardHandle);
		return FALSE;
	}
	// question : if need command 12
	// start token is CARD_SINGLE_READ_START?
	result = _ReadCARDDataToken(cardHandle,CARD_SINGLE_READ_START,buf,64, &CRC,FALSE);
//--
 	_DisableCARD(cardHandle);
	if(FALSE == result)
	{
		return result;
	}
	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x00 == cmdStatus.grp2SwitchResult)
		&&(0 != ((0x1<<0)&(cmdStatus.grp2Supported)))
	)
	{
		//CARD_SPI_PRINT:"card exit mcex success"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SPI_3523_112_2_18_1_52_38_343,(uint8*)"");
	}

	SCI_Sleep(500);
	return result;


}


#endif

/*lint -restore*/

