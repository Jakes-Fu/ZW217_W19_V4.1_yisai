/******************************************************************************
 ** File Name:      card_sdio.c
 ** Author:         Jason.wu
 ** DATE:           09/17/2007
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SD host.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 09/17/2007     Jason.wu        Create.
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "ms_ref_scp_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "card_sdio.h"
#include "sdio_card_pal.h"
#ifdef MBBMS_SUPPORT  
#include "sdua_config.h"
#endif
#include "mcd_m.h"

//#define CARD_SDIO_HIGHSPEED_SUPPORT

//#define CARD_SDIO_PRINT(x)
#define CARD_SDIO_PRINT(x) SCI_TRACE_LOW x
#define CARD_SDIO_ASSERT SCI_ASSERT
#define DEFAULT_CARD_BLOCKLEN 512
#define CARD_CMD_MAX_TIME 5000




/*****************************************************************************/
//	The struct of Card IDentification (CID) register of MMC. please refer to <Multi media card System Specification version 3.31>
/*****************************************************************************/
typedef struct CARD_MMC_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit	Manufacturer ID
	uint16 OID;	// 16 bit	OEM/Application ID
	uint8 PNM[6];	// 48 bit	Product name
	uint8 PRV;	// 8 bit	Product revision
	uint32 PSN;	// 32 bit	Product serial number
	uint8 MDT;	// 8bit	Manufacturing date
//	uint8 CRC;	// 8 bit
//	uint8 lastBit;	// 1 bit :always 1
}CARD_MMC_CID_T;


MCD_HANDLE_CDS_T *g_handmcdCsd;
/*****************************************************************************/
// The struct of Card-Specific Data register of MMC. please refer to <Multi media card System Specification version 3.31>
/*****************************************************************************/
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
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_MMC_CSD_T;

/*****************************************************************************/
//	The struct of Card IDentification (CID) register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
typedef struct CARD_SD_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit
	uint16 OID;	// 16 bit
	uint8 PNM[5];	// 40 bit
	uint8 PRV;	// 8 bit
	uint32 PSN;	// 32 bit
	uint16 MDT;	// 12bit
//	uint8 CRC;	// 1 bit
//	uint8 lastBit;	// 1 bit :always 1
}CARD_SD_CID_T;

/*****************************************************************************/
// The struct of Card-Specific Data register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
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
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD10_T;

/*****************************************************************************/
// The struct of Card-Specific Data register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
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
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD20_T;

typedef union CARD_CSD_STRUCT_TAG
{
	CARD_SD_CSD10_T SD_CSD10;
	CARD_SD_CSD20_T SD_CSD20;
	CARD_MMC_CSD_T MMC_CSD;
}CARD_CSD_T;

typedef union CARD_CID_STRUCT_TAG
{
	CARD_SD_CID_T SD_CID;
	CARD_MMC_CID_T MMC_CID;
}CARD_CID_T;


typedef struct CARD_PORT_TAG
{
	BOOLEAN open_flag;

	SDIO_CARD_PAL_HANDLE sdioPalHd;

	CARD_VERTION_E vertion;
	uint16 RCA;
	CARD_BUS_WIDTH_E bus_width;

	uint32 ClockRate;
	uint32 BlockLen;

	uint32 Nac_Max;
	uint32 GrpSize;
	uint32 Capacity;

	uint16  CCC;
	BOOLEAN ifSupportEC;
	BOOLEAN ifSupportHighSpeed;

	BOOLEAN ifEnterEC;
	BOOLEAN ifEnterHighSpeed;
	#if defined (ATV_WORKAROUND)
    SCI_MUTEX_PTR mutex;
    #endif
}CARD_PORT_T;

#if defined (ATV_WORKAROUND)
#define ERR_CARD_SDIO_NO_MUTEX 1
#define CARD_SDIO_GetMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_CARD_SDIO_NO_MUTEX;\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }

#define CARD_SDIO_PutMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_CARD_SDIO_NO_MUTEX;\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_PutMutex( mutex );\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }
#endif

LOCAL CARD_PORT_T cardPort[CARD_SDIO_SLOT_MAX] = {0};

//-----------------------------------------------------------------------------------
//	To judge whether the handle is valid
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _IsCardHandleValid(CARD_SDIO_HANDLE cardHandle)
{
	uint32 i;

	for(i = 0; i < CARD_SDIO_SLOT_MAX; i++)
	{
		if(cardHandle == &cardPort[i])
		{
			if(TRUE == cardHandle->open_flag)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return FALSE;

}


/*****************************************************************************/
//  Description:  Get Card operation handle,after we get the handle ,we can get next operation way of card
//  Author: Jason.wu
//  Param
//		slotNo: slot number
//  Return:
//		Not zero: success
//		zero : fail
//  Note: 
/*****************************************************************************/
PUBLIC CARD_SDIO_HANDLE CARD_SDIO_Open(CARD_SDIO_SLOT_NO slotNo)
{
	CARD_SDIO_ASSERT(slotNo < CARD_SDIO_SLOT_MAX);	/*assert verified*/

	if(TRUE == cardPort[slotNo].open_flag)
	{
		return NULL;
	}

	cardPort[slotNo].open_flag = TRUE;
	cardPort[slotNo].sdioPalHd = SDIO_Card_Pal_Open((SDIO_CARD_PAL_SLOT_E)slotNo);
	cardPort[slotNo].BlockLen = 0;
	cardPort[slotNo].RCA = 0x1000 | slotNo;
	cardPort[slotNo].bus_width = CARD_WIDTH_1_BIT;
	#if defined (ATV_WORKAROUND)
    cardPort[slotNo].mutex = SCI_CreateMutex ("SDIO SYNC MUTEX", SCI_INHERIT);
    #endif
	return &cardPort[slotNo];
}

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void CARD_SDIO_Close(CARD_SDIO_HANDLE cardHandle)
{
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	//SDIO_Card_Pal_Close(cardHandle->sdioPalHd);
	mcd_Close(0);
	cardHandle->open_flag = FALSE;
	#if defined (ATV_WORKAROUND)
    SCI_DeleteMutex (cardHandle->mutex);
    cardHandle->mutex = NULL;
    #endif
}

PUBLIC void CARD_SDIO_PwrCtl(CARD_SDIO_HANDLE cardHandle,BOOLEAN switchCtl)
{
       return;

	 #if 0
	if(TRUE == switchCtl)
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_ON);
	}
	else if(FALSE == switchCtl)
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_OFF);
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}
	#endif
	
}

//-----------------------------------------------------------------------------------
//	Analyze SD CID buffer
//-----------------------------------------------------------------------------------
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

//	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
//	CID->lastBit = (CIDbuf[15]&0x01);
}

//-----------------------------------------------------------------------------------
//	Analyze MMC CID buffer
//-----------------------------------------------------------------------------------
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

//	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
//	CID->lastBit = (CIDbuf[15]&0x01);
}


//-----------------------------------------------------------------------------------
//	Analyze SD CSD buffer
//
//	Expand SD CSD1.x to CSD struct member
//
//-----------------------------------------------------------------------------------
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

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}


//-----------------------------------------------------------------------------------
//	Analyze SD CSD buffer
//
//	Expand SD CSD2.0 to CSD struct member
//
//-----------------------------------------------------------------------------------
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

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}

//-----------------------------------------------------------------------------------
//	Analyze MMC CSD buffer
//
//	Expand MMC CSD to CSD struct member
//
//-----------------------------------------------------------------------------------
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

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}



/*****************************************************************************/
//  Description:  Read CSD register and analyze it
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		CSD: CSD data will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadCSD(CARD_SDIO_HANDLE cardHandle,CARD_CSD_T* CSD)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	cardHandle->CCC = 0;

	argument = RCA;
	argument = argument<<16;


	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD9_SEND_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}
//--
	//CARD_SDIO_PRINT:"SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_832_112_2_18_1_52_28_0,(uint8*)"ddddddddddddddd",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3],rspBuf[4],rspBuf[5],rspBuf[6],rspBuf[7],rspBuf[8],rspBuf[9],rspBuf[10],rspBuf[11],rspBuf[12],rspBuf[13],rspBuf[14]);

	if(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	{
		_SD_CSD20_Analyze(rspBuf,&(CSD->SD_CSD20));
		cardHandle->CCC = CSD->SD_CSD20.CCC;

		//CARD_SDIO_PRINT:"CARD_SD_V2_0_HIGHCAP:"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_839_112_2_18_1_52_28_1,(uint8*)"");
		//CARD_SDIO_PRINT:"CSD_STRUCTURE = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_840_112_2_18_1_52_28_2,(uint8*)"d",CSD->SD_CSD20.CSD_STRUCTURE);
		//CARD_SDIO_PRINT:"reserved1= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_841_112_2_18_1_52_28_3,(uint8*)"d",CSD->SD_CSD20.reserved1);
		//CARD_SDIO_PRINT:"TAAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_842_112_2_18_1_52_28_4,(uint8*)"d",CSD->SD_CSD20.TAAC);
		//CARD_SDIO_PRINT:"NSAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_843_112_2_18_1_52_28_5,(uint8*)"d",CSD->SD_CSD20.NSAC);
		//CARD_SDIO_PRINT:"TRAN_SPEED= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_844_112_2_18_1_52_28_6,(uint8*)"d",CSD->SD_CSD20.TRAN_SPEED);
		//CARD_SDIO_PRINT:"CCC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_845_112_2_18_1_52_28_7,(uint8*)"d",CSD->SD_CSD20.CCC);
		//CARD_SDIO_PRINT:"READ_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_846_112_2_18_1_52_28_8,(uint8*)"d",CSD->SD_CSD20.READ_BL_LEN);
		//CARD_SDIO_PRINT:"READ_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_847_112_2_18_1_52_28_9,(uint8*)"d",CSD->SD_CSD20.READ_BL_PARTIAL);
		//CARD_SDIO_PRINT:"WRITE_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_848_112_2_18_1_52_28_10,(uint8*)"d",CSD->SD_CSD20.WRITE_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"READ_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_849_112_2_18_1_52_28_11,(uint8*)"d",CSD->SD_CSD20.READ_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"DSR_IMP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_850_112_2_18_1_52_28_12,(uint8*)"d",CSD->SD_CSD20.DSR_IMP);

		//CARD_SDIO_PRINT:"reserved2= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_852_112_2_18_1_52_28_13,(uint8*)"d",CSD->SD_CSD20.reserved2);
		//CARD_SDIO_PRINT:"C_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_853_112_2_18_1_52_28_14,(uint8*)"d",CSD->SD_CSD20.C_SIZE);
		//CARD_SDIO_PRINT:"reserved3= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_854_112_2_18_1_52_28_15,(uint8*)"d",CSD->SD_CSD20.reserved3);

		//CARD_SDIO_PRINT:"ERASE_BLK_EN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_856_112_2_18_1_52_28_16,(uint8*)"d",CSD->SD_CSD20.ERASE_BLK_EN);
		//CARD_SDIO_PRINT:"SECTOR_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_857_112_2_18_1_52_28_17,(uint8*)"d",CSD->SD_CSD20.SECTOR_SIZE);
		//CARD_SDIO_PRINT:"WP_GRP_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_858_112_2_18_1_52_28_18,(uint8*)"d",CSD->SD_CSD20.WP_GRP_SIZE);
		//CARD_SDIO_PRINT:"WP_GRP_ENABLE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_859_112_2_18_1_52_28_19,(uint8*)"d",CSD->SD_CSD20.WP_GRP_ENABLE);
		//CARD_SDIO_PRINT:"reserved4= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_860_112_2_18_1_52_28_20,(uint8*)"d",CSD->SD_CSD20.reserved4);

		//CARD_SDIO_PRINT:"R2W_FACTOR= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_862_112_2_18_1_52_28_21,(uint8*)"d",CSD->SD_CSD20.R2W_FACTOR);
		//CARD_SDIO_PRINT:"WRITE_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_863_112_2_18_1_52_28_22,(uint8*)"d",CSD->SD_CSD20.WRITE_BL_LEN);
		//CARD_SDIO_PRINT:"WRITE_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_864_112_2_18_1_52_28_23,(uint8*)"d",CSD->SD_CSD20.WRITE_BL_PARTIAL);
		//CARD_SDIO_PRINT:"reserved5= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_865_112_2_18_1_52_28_24,(uint8*)"d",CSD->SD_CSD20.reserved5);

		//CARD_SDIO_PRINT:"FILE_FORMAT_GRP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_867_112_2_18_1_52_28_25,(uint8*)"d",CSD->SD_CSD20.FILE_FORMAT_GRP);
		//CARD_SDIO_PRINT:"COPY= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_868_112_2_18_1_52_28_26,(uint8*)"d",CSD->SD_CSD20.COPY);
		//CARD_SDIO_PRINT:"PERM_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_869_112_2_18_1_52_28_27,(uint8*)"d",CSD->SD_CSD20.PERM_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"TMP_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_870_112_2_18_1_52_28_28,(uint8*)"d",CSD->SD_CSD20.TMP_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"FILE_FORMAT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_871_112_2_18_1_52_28_29,(uint8*)"d",CSD->SD_CSD20.FILE_FORMAT);
		//CARD_SDIO_PRINT:"reserved6= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_872_112_2_18_1_52_28_30,(uint8*)"d",CSD->SD_CSD20.reserved6);
	//	CARD_SDIO_PRINT(("CRC= %x",CSD->SD_CSD20.CRC));
	//	CARD_SDIO_PRINT(("LastBit= %x",CSD->SD_CSD20.LastBit));
	}
	else if(
		(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V1_X == cardHandle->vertion)
	)
	{
		_SD_CSD10_Analyze(rspBuf,&(CSD->SD_CSD10));
		cardHandle->CCC = CSD->SD_CSD10.CCC;

		//CARD_SDIO_PRINT:"CARD_SD_V2_0_STANDARD||CARD_SD_V1_X:"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_884_112_2_18_1_52_28_31,(uint8*)"");
		//CARD_SDIO_PRINT:"CSD_STRUCTURE = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_885_112_2_18_1_52_28_32,(uint8*)"d",CSD->SD_CSD10.CSD_STRUCTURE);
		//CARD_SDIO_PRINT:"reserved1= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_886_112_2_18_1_52_28_33,(uint8*)"d",CSD->SD_CSD10.reserved1);
		//CARD_SDIO_PRINT:"TAAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_887_112_2_18_1_52_28_34,(uint8*)"d",CSD->SD_CSD10.TAAC);
		//CARD_SDIO_PRINT:"NSAC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_888_112_2_18_1_52_28_35,(uint8*)"d",CSD->SD_CSD10.NSAC);
		//CARD_SDIO_PRINT:"TRAN_SPEED= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_889_112_2_18_1_52_28_36,(uint8*)"d",CSD->SD_CSD10.TRAN_SPEED);
		//CARD_SDIO_PRINT:"CCC= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_890_112_2_18_1_52_28_37,(uint8*)"d",CSD->SD_CSD10.CCC);
		//CARD_SDIO_PRINT:"READ_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_891_112_2_18_1_52_28_38,(uint8*)"d",CSD->SD_CSD10.READ_BL_LEN);
		//CARD_SDIO_PRINT:"READ_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_892_112_2_18_1_52_28_39,(uint8*)"d",CSD->SD_CSD10.READ_BL_PARTIAL);
		//CARD_SDIO_PRINT:"WRITE_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_893_112_2_18_1_52_28_40,(uint8*)"d",CSD->SD_CSD10.WRITE_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"READ_BLK_MISALIGN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_894_112_2_18_1_52_28_41,(uint8*)"d",CSD->SD_CSD10.READ_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"DSR_IMP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_895_112_2_18_1_52_28_42,(uint8*)"d",CSD->SD_CSD10.DSR_IMP);

		//CARD_SDIO_PRINT:"reserved2= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_897_112_2_18_1_52_28_43,(uint8*)"d",CSD->SD_CSD10.reserved2);
		//CARD_SDIO_PRINT:"C_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_898_112_2_18_1_52_28_44,(uint8*)"d",CSD->SD_CSD10.C_SIZE);
		//CARD_SDIO_PRINT:"VDD_R_CURR_MIN = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_899_112_2_18_1_52_28_45,(uint8*)"d",CSD->SD_CSD10.VDD_R_CURR_MIN);
		//CARD_SDIO_PRINT:"VDD_R_CURR_MAX = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_900_112_2_18_1_52_28_46,(uint8*)"d",CSD->SD_CSD10.VDD_R_CURR_MAX);
		//CARD_SDIO_PRINT:"VDD_W_CURR_MIN = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_901_112_2_18_1_52_28_47,(uint8*)"d",CSD->SD_CSD10.VDD_W_CURR_MIN);
		//CARD_SDIO_PRINT:"VDD_W_CURR_MAX = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_902_112_2_18_1_52_28_48,(uint8*)"d",CSD->SD_CSD10.VDD_W_CURR_MAX);
		//CARD_SDIO_PRINT:"C_SIZE_MULT = %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_903_112_2_18_1_52_28_49,(uint8*)"d",CSD->SD_CSD10.C_SIZE_MULT);

		//CARD_SDIO_PRINT:"ERASE_BLK_EN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_905_112_2_18_1_52_28_50,(uint8*)"d",CSD->SD_CSD10.ERASE_BLK_EN);
		//CARD_SDIO_PRINT:"SECTOR_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_906_112_2_18_1_52_28_51,(uint8*)"d",CSD->SD_CSD10.SECTOR_SIZE);
		//CARD_SDIO_PRINT:"WP_GRP_SIZE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_907_112_2_18_1_52_28_52,(uint8*)"d",CSD->SD_CSD10.WP_GRP_SIZE);
		//CARD_SDIO_PRINT:"WP_GRP_ENABLE= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_908_112_2_18_1_52_28_53,(uint8*)"d",CSD->SD_CSD10.WP_GRP_ENABLE);
		//CARD_SDIO_PRINT:"reserved3= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_909_112_2_18_1_52_28_54,(uint8*)"d",CSD->SD_CSD10.reserved3);

		//CARD_SDIO_PRINT:"R2W_FACTOR= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_911_112_2_18_1_52_28_55,(uint8*)"d",CSD->SD_CSD10.R2W_FACTOR);
		//CARD_SDIO_PRINT:"WRITE_BL_LEN= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_912_112_2_18_1_52_28_56,(uint8*)"d",CSD->SD_CSD10.WRITE_BL_LEN);
		//CARD_SDIO_PRINT:"WRITE_BL_PARTIAL= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_913_112_2_18_1_52_28_57,(uint8*)"d",CSD->SD_CSD10.WRITE_BL_PARTIAL);
		//CARD_SDIO_PRINT:"reserved4= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_914_112_2_18_1_52_28_58,(uint8*)"d",CSD->SD_CSD10.reserved4);

		//CARD_SDIO_PRINT:"FILE_FORMAT_GRP= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_916_112_2_18_1_52_28_59,(uint8*)"d",CSD->SD_CSD10.FILE_FORMAT_GRP);
		//CARD_SDIO_PRINT:"COPY= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_917_112_2_18_1_52_28_60,(uint8*)"d",CSD->SD_CSD10.COPY);
		//CARD_SDIO_PRINT:"PERM_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_918_112_2_18_1_52_28_61,(uint8*)"d",CSD->SD_CSD10.PERM_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"TMP_WRITE_PROTECT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_919_112_2_18_1_52_28_62,(uint8*)"d",CSD->SD_CSD10.TMP_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"FILE_FORMAT= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_920_112_2_18_1_52_28_63,(uint8*)"d",CSD->SD_CSD10.FILE_FORMAT);
		//CARD_SDIO_PRINT:"reserved5= %x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_921_112_2_18_1_52_28_64,(uint8*)"d",CSD->SD_CSD10.reserved5);
	//	CARD_SDIO_PRINT(("CRC= %x",CSD->SD_CSD10.CRC));
	//	CARD_SDIO_PRINT(("LastBit= %x",CSD->SD_CSD10.LastBit));
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		_MMC_CSD_Analyze(rspBuf,&(CSD->MMC_CSD));
		cardHandle->CCC = CSD->MMC_CSD.CCC;
		cardHandle->CCC &= (~((1<<10)|(1<<11)));	// for MMC mode This two classes is reserved

		//CARD_SDIO_PRINT:"CARD_MMC_331:"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_931_112_2_18_1_52_28_65,(uint8*)"");
		//CARD_SDIO_PRINT:"CSD_STRUCTURE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_932_112_2_18_1_52_28_66,(uint8*)"d",CSD->MMC_CSD.CSD_STRUCTURE);
		//CARD_SDIO_PRINT:"SPEC_VERS\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_933_112_2_18_1_52_28_67,(uint8*)"d",CSD->MMC_CSD.SPEC_VERS);
		//CARD_SDIO_PRINT:"reserved1\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_934_112_2_18_1_52_28_68,(uint8*)"d",CSD->MMC_CSD.reserved1);
		//CARD_SDIO_PRINT:"TAAC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_935_112_2_18_1_52_28_69,(uint8*)"d",CSD->MMC_CSD.TAAC);
		//CARD_SDIO_PRINT:"NSAC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_936_112_2_18_1_52_28_70,(uint8*)"d",CSD->MMC_CSD.NSAC);
		//CARD_SDIO_PRINT:"TRAN_SPEED\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_937_112_2_18_1_52_28_71,(uint8*)"d",CSD->MMC_CSD.TRAN_SPEED);
		//CARD_SDIO_PRINT:"CCC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_938_112_2_18_1_52_28_72,(uint8*)"d",CSD->MMC_CSD.CCC);
		//CARD_SDIO_PRINT:"READ_BL_LEN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_939_112_2_18_1_52_28_73,(uint8*)"d",CSD->MMC_CSD.READ_BL_LEN);
		//CARD_SDIO_PRINT:"READ_BL_PARTIAL\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_940_112_2_18_1_52_28_74,(uint8*)"d",CSD->MMC_CSD.READ_BL_PARTIAL);
		//CARD_SDIO_PRINT:"WRITE_BLK_MISALIGN\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_941_112_2_18_1_52_28_75,(uint8*)"d",CSD->MMC_CSD.WRITE_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"READ_BLK_MISALIGN\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_942_112_2_18_1_52_28_76,(uint8*)"d",CSD->MMC_CSD.READ_BLK_MISALIGN);
		//CARD_SDIO_PRINT:"DSR_IMP\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_943_112_2_18_1_52_28_77,(uint8*)"d",CSD->MMC_CSD.DSR_IMP);
		//CARD_SDIO_PRINT:"reserved2\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_944_112_2_18_1_52_28_78,(uint8*)"d",CSD->MMC_CSD.reserved2);
		//CARD_SDIO_PRINT:"C_SIZE\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_945_112_2_18_1_52_28_79,(uint8*)"d",CSD->MMC_CSD.C_SIZE);
		//CARD_SDIO_PRINT:"VDD_R_CURR_MIN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_946_112_2_18_1_52_28_80,(uint8*)"d",CSD->MMC_CSD.VDD_R_CURR_MIN);
		//CARD_SDIO_PRINT:"VDD_R_CURR_MAX\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_947_112_2_18_1_52_28_81,(uint8*)"d",CSD->MMC_CSD.VDD_R_CURR_MAX);
		//CARD_SDIO_PRINT:"VDD_W_CURR_MIN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_948_112_2_18_1_52_28_82,(uint8*)"d",CSD->MMC_CSD.VDD_W_CURR_MIN);
		//CARD_SDIO_PRINT:"VDD_W_CURR_MAX\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_949_112_2_18_1_52_28_83,(uint8*)"d",CSD->MMC_CSD.VDD_W_CURR_MAX);
		//CARD_SDIO_PRINT:"C_SIZE_MULT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_950_112_2_18_1_52_28_84,(uint8*)"d",CSD->MMC_CSD.C_SIZE_MULT);
		//CARD_SDIO_PRINT:"ERASE_GRP_SIZE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_951_112_2_18_1_52_28_85,(uint8*)"d",CSD->MMC_CSD.ERASE_GRP_SIZE);
		//CARD_SDIO_PRINT:"ERASE_GRP_MULT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_952_112_2_18_1_52_28_86,(uint8*)"d",CSD->MMC_CSD.ERASE_GRP_MULT);
		//CARD_SDIO_PRINT:"WP_GRP_SIZE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_953_112_2_18_1_52_28_87,(uint8*)"d",CSD->MMC_CSD.WP_GRP_SIZE);
		//CARD_SDIO_PRINT:"WP_GRP_ENABLE\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_954_112_2_18_1_52_28_88,(uint8*)"d",CSD->MMC_CSD.WP_GRP_ENABLE);
		//CARD_SDIO_PRINT:"DEFAULT_ECC\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_955_112_2_18_1_52_28_89,(uint8*)"d",CSD->MMC_CSD.DEFAULT_ECC);
		//CARD_SDIO_PRINT:"R2W_FACTOR\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_956_112_2_18_1_52_28_90,(uint8*)"d",CSD->MMC_CSD.R2W_FACTOR);
		//CARD_SDIO_PRINT:"WRITE_BL_LEN\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_957_112_2_18_1_52_28_91,(uint8*)"d",CSD->MMC_CSD.WRITE_BL_LEN);
		//CARD_SDIO_PRINT:"WRITE_BL_PARTIAL\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_958_112_2_18_1_52_28_92,(uint8*)"d",CSD->MMC_CSD.WRITE_BL_PARTIAL);
		//CARD_SDIO_PRINT:"reserved3\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_959_112_2_18_1_52_28_93,(uint8*)"d",CSD->MMC_CSD.reserved3);
		//CARD_SDIO_PRINT:"CONTENT_PROT_APP\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_960_112_2_18_1_52_28_94,(uint8*)"d",CSD->MMC_CSD.CONTENT_PROT_APP);
		//CARD_SDIO_PRINT:"----RW bit---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_961_112_2_18_1_52_28_95,(uint8*)"");
		//CARD_SDIO_PRINT:"FILE_FORMAT_GRP\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_962_112_2_18_1_52_28_96,(uint8*)"d",CSD->MMC_CSD.FILE_FORMAT_GRP);
		//CARD_SDIO_PRINT:"COPY\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_963_112_2_18_1_52_28_97,(uint8*)"d",CSD->MMC_CSD.COPY);
		//CARD_SDIO_PRINT:"PERM_WRITE_PROTECT\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_964_112_2_18_1_52_28_98,(uint8*)"d",CSD->MMC_CSD.PERM_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"TMP_WRITE_PROTECT\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_965_112_2_18_1_52_28_99,(uint8*)"d",CSD->MMC_CSD.TMP_WRITE_PROTECT);
		//CARD_SDIO_PRINT:"FILE_FORMAT\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_966_112_2_18_1_52_28_100,(uint8*)"d",CSD->MMC_CSD.FILE_FORMAT);
		//CARD_SDIO_PRINT:"ECC\t\t\t  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_967_112_2_18_1_52_28_101,(uint8*)"d",CSD->MMC_CSD.ECC);
//		CARD_SDIO_PRINT(("CRC\t\t\t  = 0x%x",CSD->MMC_CSD.CRC));
//		CARD_SDIO_PRINT(("LastBit\t\t\t  = 0x%x",CSD->MMC_CSD.LastBit));

	}
	else
	{
		SCI_TRACE_LOW("[SCM: Card_sdio] Err Card vertion unkonwn func[%s] line[%d] ",__FUNCTION__,__LINE__);
		return FALSE;
		//CARD_SDIO_ASSERT(0);	/*assert verified*/
	}


	return TRUE;
}


/*****************************************************************************/
//  Description:  Read CID register and analyze it
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		CID: CID data will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadCID(CARD_SDIO_HANDLE cardHandle,CARD_CID_T* CID)
{

	return TRUE;
#if 0
	uint8 rspBuf[16];

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD2_ALL_SEND_CID,0,NULL,rspBuf))
	{
		return FALSE;
	}
//--
	//CARD_SDIO_PRINT:"SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1002_112_2_18_1_52_29_102,(uint8*)"ddddddddddddddd",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3],rspBuf[4],rspBuf[5],rspBuf[6],rspBuf[7],rspBuf[8],rspBuf[9],rspBuf[10],rspBuf[11],rspBuf[12],rspBuf[13],rspBuf[14]);

	if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		_SD_CID_Analyze(rspBuf,&CID->SD_CID);
		//CARD_SDIO_PRINT:"-----CID value---\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1011_112_2_18_1_52_29_103,(uint8*)"");
		//CARD_SDIO_PRINT:"MID\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1012_112_2_18_1_52_29_104,(uint8*)"d",CID->SD_CID.MID);
		//CARD_SDIO_PRINT:"OID\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1013_112_2_18_1_52_29_105,(uint8*)"d",CID->SD_CID.OID);
		//CARD_SDIO_PRINT:"PNM\t\t = %c,%c,%c,%c,%c\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1014_112_2_18_1_52_29_106,(uint8*)"ccccc",CID->SD_CID.PNM[0],CID->SD_CID.PNM[1],CID->SD_CID.PNM[2],CID->SD_CID.PNM[3],CID->SD_CID.PNM[4]);
		//CARD_SDIO_PRINT:"PRV\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1015_112_2_18_1_52_29_107,(uint8*)"d",CID->SD_CID.PRV);
		//CARD_SDIO_PRINT:"PSN\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1016_112_2_18_1_52_29_108,(uint8*)"d",CID->SD_CID.PSN);
		//CARD_SDIO_PRINT:"MDT\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1017_112_2_18_1_52_29_109,(uint8*)"d",CID->SD_CID.MDT);
	//	CARD_SDIO_PRINT(("CRC\t\t = 0x%x\r\n",CID->SD_CID.CRC));
	//	CARD_SDIO_PRINT(("lastBit\t\t = 0x%x\r\n",CID->SD_CID.lastBit));
#ifdef MBBMS_SUPPORT        
              SDUACFG_GetPnm(CID->SD_CID.PNM);
#endif	
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		_MMC_CID_Analyze(rspBuf,&CID->MMC_CID);
		//CARD_SDIO_PRINT:"-----MMC CID value---\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1027_112_2_18_1_52_29_110,(uint8*)"");
		//CARD_SDIO_PRINT:"MID\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1028_112_2_18_1_52_29_111,(uint8*)"d",CID->MMC_CID.MID);
		//CARD_SDIO_PRINT:"OID\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1029_112_2_18_1_52_29_112,(uint8*)"d",CID->MMC_CID.OID);
		//CARD_SDIO_PRINT:"PNM\t\t = %c,%c,%c,%c,%c,%c\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1030_112_2_18_1_52_29_113,(uint8*)"cccccc",CID->MMC_CID.PNM[0],CID->MMC_CID.PNM[1],CID->MMC_CID.PNM[2],CID->MMC_CID.PNM[3],CID->MMC_CID.PNM[4],CID->MMC_CID.PNM[5]);
		//CARD_SDIO_PRINT:"PRV\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1031_112_2_18_1_52_29_114,(uint8*)"d",CID->MMC_CID.PRV);
		//CARD_SDIO_PRINT:"PSN\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1032_112_2_18_1_52_29_115,(uint8*)"d",CID->MMC_CID.PSN);
		//CARD_SDIO_PRINT:"MDT\t\t = 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1033_112_2_18_1_52_29_116,(uint8*)"d",CID->MMC_CID.MDT);
#ifdef MBBMS_SUPPORT        
              SDUACFG_GetPnm(CID->MMC_CID.PNM);
#endif
	//	CARD_SDIO_PRINT(("CRC\t\t = 0x%x\r\n",CID->MMC_CID.CRC));
	//	CARD_SDIO_PRINT(("lastBit\t\t = 0x%x\r\n",CID->MMC_CID.lastBit));
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	return TRUE;

	#endif
}

/*****************************************************************************/
//  Description:  Set relate address to card ,this address is used to select card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		RCA: new relative address
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_SetRCA(CARD_SDIO_HANDLE cardHandle,uint16 RCA)
{
	return TRUE;
#if 0 
	uint8 rspBuf[16];
	uint32 argument = 0;

	if(CARD_MMC_331 != cardHandle->vertion)
	{
	// only MMC card can set RCA
		return FALSE;
	}

	argument = RCA;
	argument = argument<<16;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD3_SET_RELATIVE_ADDR,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
#endif

}

/*****************************************************************************/
//  Description:  Get relate address from card ,this address is used to select card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		RCA: relative address will stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadRCA(CARD_SDIO_HANDLE cardHandle,uint16*RCA)
{

	return TRUE;
	#if 0
	uint8 rspBuf[16];
	uint16 tmpRCA;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(
	// only SD card can Read RCA
		!(
			(CARD_SD_V1_X == cardHandle->vertion)
			||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
			||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
		)
	)
	{
		return FALSE;
	}
	//else
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD3_SEND_RELATIVE_ADDR,0,NULL,rspBuf))
	{
		return FALSE;
	}
	tmpRCA = 0;
	tmpRCA = rspBuf[0];
	tmpRCA = tmpRCA<<8;
	tmpRCA |= rspBuf[1];

	*RCA = tmpRCA;

	//CARD_SDIO_PRINT:"SD20 RCA = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1121_112_2_18_1_52_29_117,(uint8*)"d",tmpRCA);

	return TRUE;
	#endif
}

//-----------------------------------------------------------------------------------
//	Before operate card ,we must active card first,this function is used to active card
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SelectCard(CARD_SDIO_HANDLE cardHandle)
{

       return TRUE;
	#if 0
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = RCA;
	argument = argument<<16;

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD7_SELECT_DESELECT_CARD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
	#endif
}

//-----------------------------------------------------------------------------------
//	Set data bus width of card
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SetBusWidth(CARD_SDIO_HANDLE cardHandle,CARD_BUS_WIDTH_E width)
{ 

  return TRUE;
#if 0
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;
      

	if(width == cardHandle->bus_width)
	{
		return TRUE;
	}

	argument = RCA;
	argument = argument<<16;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD55_APP_CMD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	switch(width)
	{
		case CARD_WIDTH_1_BIT:
		{
			argument = 0;
		}
		break;

		case CARD_WIDTH_4_BIT:
		{
			argument = 2;
		}
		break;

		default:
		{
			CARD_SDIO_ASSERT(0);	/*assert verified*/
		}
		break;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_BUS_WIDTH,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	switch(width)
	{
		case CARD_WIDTH_1_BIT:
		{
			SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_1_BIT);
		}
		break;

		case CARD_WIDTH_4_BIT:
		{
			SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_4_BIT);
		}
		break;

		default:
		{
			CARD_SDIO_ASSERT(0);	/*assert verified*/
		}
		break;
	}
	cardHandle->bus_width = width;

	return TRUE;
	#endif
}

/*****************************************************************************/
//  Description:  Set block length of card.  the data will be transmited in block unit
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		length: block length
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,uint32 length)
{
 return TRUE;
 #if 0
	uint8 rspBuf[16];
      
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)&&(512 != length))
	{
		//CARD_SDIO_PRINT:"SD20 Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1235_112_2_18_1_52_29_118,(uint8*)"d",length);
		return FALSE;
	}
	if(length == cardHandle->BlockLen)
	{
		return TRUE;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD16_SET_BLOCKLEN,length,NULL,rspBuf))
	{
		return FALSE;
	}

	cardHandle->BlockLen = length;

	return TRUE;
#endif
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

	//CARD_SDIO_PRINT:"currentVal = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1313_112_2_18_1_52_29_119,(uint8*)"d",cmdStatus->currentVal);

	//CARD_SDIO_PRINT:"grp6Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1315_112_2_18_1_52_29_120,(uint8*)"d",cmdStatus->grp6Supported);
	//CARD_SDIO_PRINT:"grp5Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1316_112_2_18_1_52_29_121,(uint8*)"d",cmdStatus->grp5Supported);
	//CARD_SDIO_PRINT:"grp4Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1317_112_2_18_1_52_29_122,(uint8*)"d",cmdStatus->grp4Supported);
	//CARD_SDIO_PRINT:"grp3Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1318_112_2_18_1_52_29_123,(uint8*)"d",cmdStatus->grp3Supported);
	//CARD_SDIO_PRINT:"grp2Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1319_112_2_18_1_52_29_124,(uint8*)"d",cmdStatus->grp2Supported);
	//CARD_SDIO_PRINT:"grp1Supported = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1320_112_2_18_1_52_29_125,(uint8*)"d",cmdStatus->grp1Supported);

	//CARD_SDIO_PRINT:"grp6SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1322_112_2_18_1_52_29_126,(uint8*)"d",cmdStatus->grp6SwitchResult);
	//CARD_SDIO_PRINT:"grp5SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1323_112_2_18_1_52_29_127,(uint8*)"d",cmdStatus->grp5SwitchResult);
	//CARD_SDIO_PRINT:"grp4SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1324_112_2_18_1_52_29_128,(uint8*)"d",cmdStatus->grp4SwitchResult);
	//CARD_SDIO_PRINT:"grp3SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1325_112_2_18_1_52_29_129,(uint8*)"d",cmdStatus->grp3SwitchResult);
	//CARD_SDIO_PRINT:"grp2SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1326_112_2_18_1_52_29_130,(uint8*)"d",cmdStatus->grp2SwitchResult);
	//CARD_SDIO_PRINT:"grp1SwitchResult = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1327_112_2_18_1_52_29_131,(uint8*)"d",cmdStatus->grp1SwitchResult);

	//CARD_SDIO_PRINT:"status version = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1329_112_2_18_1_52_29_132,(uint8*)"d",cmdStatus->version);

	//CARD_SDIO_PRINT:"grp6BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1331_112_2_18_1_52_29_133,(uint8*)"d",cmdStatus->grp6BusyStatus);
	//CARD_SDIO_PRINT:"grp5BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1332_112_2_18_1_52_29_134,(uint8*)"d",cmdStatus->grp5BusyStatus);
	//CARD_SDIO_PRINT:"grp4BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1333_112_2_18_1_52_29_135,(uint8*)"d",cmdStatus->grp4BusyStatus);
	//CARD_SDIO_PRINT:"grp3BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1334_112_2_18_1_52_29_136,(uint8*)"d",cmdStatus->grp3BusyStatus);
	//CARD_SDIO_PRINT:"grp2BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1335_112_2_18_1_52_29_137,(uint8*)"d",cmdStatus->grp2BusyStatus);
	//CARD_SDIO_PRINT:"grp1BusyStatus = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1336_112_2_18_1_52_29_138,(uint8*)"d",cmdStatus->grp1BusyStatus);

	return;
}


#if defined(CARD_SDIO_HIGHSPEED_SUPPORT)

LOCAL BOOLEAN ifSupportHighSpeed(CARD_SDIO_HANDLE cardHandle)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;
	static __align(32) uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifSupportHighSpeed = FALSE;
	if(FALSE == SDIO_Card_Pal_SetSpeedMode(cardHandle->sdioPalHd, SDIO_CARD_PAL_HIGH_SPEED))
	{
		return TRUE;
	}
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(!(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	))
	{
		return TRUE;
	}
	if(0 == ((1<<10)&cardHandle->CCC))
	{
		return TRUE;
	}

	data.blkLen = 64;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD6_SWITCH_FUNC,0x00FFFFF1,&data,rspBuf))
	{
		return FALSE;
	}

	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x01 == cmdStatus.grp1SwitchResult)
		&&(0 != ((0x1<<1)&(cmdStatus.grp1Supported)))
	)
	{
		cardHandle->ifSupportHighSpeed = TRUE;
	}
	return TRUE;
}

LOCAL BOOLEAN CARD_SDIO_EnterHighSpeed(CARD_SDIO_HANDLE cardHandle)
{
return FALSE;
#if 0
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;
	static __align(32) uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifEnterHighSpeed = FALSE;
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifSupportHighSpeed)
	{
		return TRUE;
	}

	data.blkLen = 64;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD6_SWITCH_FUNC,0x80FFFFF1,&data,rspBuf))
	{
		return FALSE;
	}

	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x01 == cmdStatus.grp1SwitchResult)
		&&(0 != ((0x1<<1)&(cmdStatus.grp1Supported)))
	)
	{
		cardHandle->ifEnterHighSpeed = TRUE;
		SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd, SDIO_CARD_PAL_50MHz);
		//CARD_SDIO_PRINT:"SD20 enter high speed mode!!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1420_112_2_18_1_52_30_139,(uint8*)"");
	}

	return TRUE;
#endif

}

#endif

PUBLIC BOOLEAN CARD_SDIO_IsEnterHighSpeed(CARD_SDIO_HANDLE cardHandle)
{
	return (cardHandle->ifEnterHighSpeed);
}

#if defined(SPRD_SUPPORT_MCEX)

LOCAL BOOLEAN ifSupportMcex(CARD_SDIO_HANDLE cardHandle)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;
	static __align(32) uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifSupportEC = FALSE;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(!(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	))
	{
		return TRUE;
	}
	if(0 == ((1<<10)&cardHandle->CCC))
	{
		return TRUE;
	}

	data.blkLen = 64;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD6_SWITCH_FUNC,0x00FFFF1F,&data,rspBuf))
	{
		return FALSE;
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

#if defined(OS_NONE)
extern MCD_HANDLE_CDS_T g_handleCsd;
#endif
/*****************************************************************************/
//  Description:  Initialize card, change card from idle state to standy by state ,and get some information from card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_InitCard(CARD_SDIO_HANDLE cardHandle, CARD_SPEED_MODE speedmode)
{
	uint8 rspBuf[16];
	uint32 pre_tick, cur_tick;
	CARD_CID_T CID;
	CARD_CSD_T CSD;
	CARD_BUS_WIDTH_E busWidth = CARD_WIDTH_1_BIT;
	uint16 RCA;
	BOOLEAN vertion_flag; //TRUE: SD2.0 FALSE: SD1.x
	SDIO_CARD_PAL_ERROR_E errCode;
       mon_Event(0xdd110010);
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

    CARD_SDIO_PRINT(("[SCM: Card_sdio] CARD_SDIO_InitCard+++"));
//	CARD_SDIO_PwrCtl(cardHandle,TRUE);
	cardHandle->bus_width = CARD_WIDTH_1_BIT;
	cardHandle->BlockLen = 0;
	cardHandle->vertion = CARD_V_UNKONWN;

      if(FALSE ==  Sdio_open())
        return FALSE;
    
#if defined(OS_NONE)
    g_handmcdCsd = (MCD_HANDLE_CDS_T *)(&g_handleCsd);
#else
    g_handmcdCsd = mcd_get_mcdCsd();
#endif
    cardHandle->vertion = g_handmcdCsd->vertion;
    cardHandle->RCA = g_handmcdCsd->RCA;
    cardHandle->ifEnterHighSpeed = FALSE;
    cardHandle->Capacity = g_handmcdCsd->Capacity;
    cardHandle->BlockLen = 512;  //DEFAULT_CARD_BLOCKLEN
    cardHandle->bus_width = g_handmcdCsd->bus_width;
      
      CARD_SDIO_PRINT(("[SCM: Card_sdio] CARD_SDIO_InitCard---"));
       mon_Event(0xdd1100ed);

       return TRUE;

	   #if 0
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD0_GO_IDLE_STATE,0,NULL,rspBuf))
	{
		return FALSE;
	}
//Now the card is in Idle State

	vertion_flag = TRUE;
       pre_tick = SCI_GetTickCount();
	cur_tick = pre_tick;
	do
	{
		if(CARD_CMD_MAX_TIME < (cur_tick - pre_tick))
		{
			return FALSE;
		}
		errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD8_SEND_IF_COND,0x000001AA,NULL,rspBuf);
		if(SDIO_CARD_PAL_ERR_NONE != errCode)
		{
			if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
			{
				//SD2.0 not support current voltage or it is SD1.x
				vertion_flag = FALSE;
				break;
			}
			else
			{
				cur_tick = SCI_GetTickCount();
				continue;
			}
		}
		//CARD_SDIO_PRINT:"SD20 %x,%x,%x,%x"
		if((0xAA == rspBuf[3])&&(0x00 == rspBuf[2]))
		{
			//SD2.0 not support current voltage
			CARD_SDIO_ASSERT(0);	/*assert verified*/
			return FALSE;    /*lint !e527*/
		}
		cur_tick = SCI_GetTickCount();
	}
	while(0xAA != rspBuf[3]);	/*lint !e644 */

    CARD_SDIO_PRINT(("[SCM: Card_sdio] Send CMD8 finished!"));
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1537_112_2_18_1_52_30_140,(uint8*)"dddd",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]);
	//CARD_SDIO_PRINT:"SD20 vertion_flag = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1548_112_2_18_1_52_30_141,(uint8*)"d",vertion_flag);

       pre_tick = SCI_GetTickCount(); /*set start tick value*/
	if(FALSE == vertion_flag)
	{
		do
		{
			errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD55_APP_CMD,0,NULL,rspBuf);
			if(SDIO_CARD_PAL_ERR_NONE != errCode)
			{
				if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
				{
					//not SD1.x maybe it is MMC
					break;
				}
				else
				{
					return FALSE;
				}
			}

			errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD41_SD_SEND_OP_COND,0x00FF8000,NULL,rspBuf);
			if(SDIO_CARD_PAL_ERR_NONE != errCode)
			{
				if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
				{
					//not SD1.x maybe it is MMC
					break;
				}
				else
				{
					return FALSE;
				}
			}
			//---
			//CARD_SDIO_PRINT:"SD20 %x,%x,%x,%x"
			//SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1583_112_2_18_1_52_30_142,(uint8*)"dddd",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]);
			if(0 != (rspBuf[0]&BIT_7))
			{
				cardHandle->vertion = CARD_SD_V1_X;
				break;
			}

			cur_tick = SCI_GetTickCount();
			if(CARD_CMD_MAX_TIME < (cur_tick - pre_tick))
			{
				/*cmd time out, return false*/
                CARD_SDIO_PRINT(("[SCM: Card_sdio] Send ACMD41 timeout!!!"));
				return FALSE;
			}
		}
		while(1); /*lint !e506*/

		//MMC_INIT
		if(CARD_V_UNKONWN== cardHandle->vertion)
		{
		       pre_tick = SCI_GetTickCount(); /*set start tick value*/

			do
			{
				if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD1_SEND_OP_COND,0x00FF8000,NULL,rspBuf))
				{
					return FALSE;
				}
				if(0 != (rspBuf[0]&BIT_7))
				{
					cardHandle->vertion = CARD_MMC_331;
					break;
				}

				cur_tick = SCI_GetTickCount();
				if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
				{
					/*cmd time out, return false*/
					return FALSE;
				}
			}
			while(1); /*lint !e506*/
		}


	}
	else
	{
		do
		{
			if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD55_APP_CMD,0,NULL,rspBuf))
			{
				return FALSE;
			}

			if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD41_SD_SEND_OP_COND,0x40FF8000,NULL,rspBuf))
			{
				return FALSE;
			}
			//---
			//CARD_SDIO_PRINT:"SD20 %x,%x,%x,%x"
			if(0 != (rspBuf[0]&BIT_7))
			{
				if(0 != (rspBuf[0]&BIT_6))
				{
					cardHandle->vertion = CARD_SD_V2_0_HIGHCAP;
				}
				else
				{
					cardHandle->vertion = CARD_SD_V2_0_STANDARD;
				}
				break;
			}

			cur_tick = SCI_GetTickCount();
			if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
			{
				/*cmd time out, return false*/
                CARD_SDIO_PRINT(("[SCM: Card_sdio] Send ACMD41 timeout!!!"));
				return FALSE;
			}
		}
		while(1); /*lint !e506*/

	}

    CARD_SDIO_PRINT(("[SCM: Card_sdio] Send ACMD41 finished! cardHandle->vertion = %x", cardHandle->vertion));
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1642_112_2_18_1_52_30_143,(uint8*)"dddd",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]);
	if(CARD_V_UNKONWN == cardHandle->vertion)
	{
		return FALSE;
	}

// Now SD is in Ready State
	if(FALSE == CARD_SDIO_ReadCID(cardHandle,&CID))
	{
		return FALSE;
	}
// Now SD is in Identification State

	if(CARD_MMC_331 == cardHandle->vertion)
	{
		if(FALSE == CARD_SDIO_SetRCA(cardHandle,cardHandle->RCA))
		{
			return FALSE;
		}
	}
	else if(
	// only SD card can Read RCA
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		if(FALSE == CARD_SDIO_ReadRCA(cardHandle,&RCA))
		{
			return FALSE;
		}
		cardHandle->RCA = RCA;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;    /*lint !e527*/
	}

// Now SD is in Stand-by State

	//CARD_SDIO_PRINT:"SD20 type is %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1706_112_2_18_1_52_30_144,(uint8*)"d",cardHandle->vertion);

	if(FALSE == CARD_SDIO_ReadCSD(cardHandle,&CSD))
	{
		return FALSE;
	}

	if(!(
		(((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))&&(0 == CSD.SD_CSD10.CSD_STRUCTURE))
		||((CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)&&(1 == CSD.SD_CSD20.CSD_STRUCTURE))
		||((CARD_MMC_331 == cardHandle->vertion))
	))
	{
		return FALSE;
	}
	if((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))
	{
		cardHandle->Nac_Max = (uint32)((CSD.SD_CSD10.TAAC*cardHandle->ClockRate) + (100*CSD.SD_CSD10.NSAC));
		cardHandle->Capacity = (CSD.SD_CSD10.C_SIZE+1) <<(CSD.SD_CSD10.C_SIZE_MULT+2 + CSD.SD_CSD10.READ_BL_LEN-9); //unit is 512byte
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		cardHandle->Nac_Max = (uint32)((CSD.SD_CSD20.TAAC*cardHandle->ClockRate) + (100*CSD.SD_CSD20.NSAC));
		cardHandle->Capacity = (CSD.SD_CSD20.C_SIZE+1)<<10; // (CSD.SD_CSD20.C_SIZE+1)*1024 unit is 512byte
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		cardHandle->GrpSize = (CSD.MMC_CSD.ERASE_GRP_SIZE+1)*(CSD.MMC_CSD.ERASE_GRP_MULT+1)*(1 << CSD.MMC_CSD.WRITE_BL_LEN);
		cardHandle->Nac_Max = (uint32)((((CSD.MMC_CSD.TAAC*cardHandle->ClockRate) + (100*CSD.MMC_CSD.NSAC))*10) / 8);
		cardHandle->Capacity = (CSD.MMC_CSD.C_SIZE+1) <<(CSD.MMC_CSD.C_SIZE_MULT+2 + CSD.MMC_CSD.READ_BL_LEN - 9); //unit is 512 byte

	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

//---
	if(CARD_MMC_331 == cardHandle->vertion)
	{
		SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_20MHz);
	}
	else if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD== cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	)
	{
		SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_25MHz);
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;	/*lint !e527*/
	}

	if(FALSE == _SelectCard(cardHandle))
	{
		return FALSE;
	}
	if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		busWidth = CARD_WIDTH_4_BIT;
		if(FALSE == _SetBusWidth(cardHandle,CARD_WIDTH_4_BIT))
		{
			return FALSE;
		}
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		busWidth = CARD_WIDTH_1_BIT;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

	if(FALSE == CARD_SDIO_SetBlockLength( cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

#if defined(CARD_SDIO_HIGHSPEED_SUPPORT)
	if(HIGH_SPEED_MODE == speedmode)
	{
		if(FALSE == ifSupportHighSpeed(cardHandle))
		{
			return FALSE;
		}
		if(TRUE == cardHandle->ifSupportHighSpeed)
		{
			if(FALSE == CARD_SDIO_EnterHighSpeed(cardHandle))
			{
				return FALSE;
			}
		}
	}
	else
	{
		cardHandle->ifEnterHighSpeed = FALSE;
		//CARD_SDIO_PRINT:"[Card_sdio] CARD_SDIO_InitCard: Disable HighSpeed Mode !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1812_112_2_18_1_52_30_145,(uint8*)"");
	}
#endif

#if defined(SPRD_SUPPORT_MCEX)
	if(FALSE == ifSupportMcex(cardHandle))
	{
		return FALSE;
	}
	if(TRUE == cardHandle->ifEnterEC)
	{
		BOOLEAN ifEnterMcex;
		if(FALSE == CARD_SDIO_EnterMcex(cardHandle,&ifEnterMcex))
		{
			return FALSE;
		}
	}
#endif



//---
    CARD_SDIO_PRINT(("[SCM: Card_sdio] CARD_SDIO_InitCard---"));
	return TRUE;
#endif
}


//-----------------------------------------------------------------------------------
//	Inquire Card whethe ready for data
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _IsCardReady(CARD_SDIO_HANDLE cardHandle)
{
	return TRUE;
#if 0
	uint32 pre_tick = 0, cur_tick = 0;
	uint8 rspBuf[16] = {0};
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;
     
	argument = RCA;
	argument = argument<<16;
	pre_tick = SCI_GetTickCount(); /*set start tick value*/

	do
	{
		if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS,argument,NULL,rspBuf))
		{
			return FALSE;
		}

//		CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
		
		if(0 != (rspBuf[2]&BIT_0))
		{
			return TRUE;
		}

		cur_tick = SCI_GetTickCount();
		if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
		{
			/*cmd time out, return false*/
			//CARD_SDIO_PRINT:"[Card_sdio] _IsCardReady timeout !"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_1871_112_2_18_1_52_31_146,(uint8*)"");
			return FALSE;
		}
	}
	while(1); /*lint !e506*/
#endif

}

/*****************************************************************************/
//  Description:  Read single block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note:    MCD_ERR_T 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadSingleBlock(CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		//address = BlockId*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		//address = BlockId;
	}
	else
	{
		SCI_TRACE_LOW("[SCM: Card_sdio] Err Card vertion unkonwn func[%s] line[%d] ",__FUNCTION__,__LINE__);
		return FALSE;
		//CARD_SDIO_ASSERT(0);	/*assert verified*/
	}
	
	//if(FALSE == _IsCardReady(cardHandle))
	{
		//return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = 1;
	data.databuf = buf;
#if defined (ATV_WORKAROUND)
    CARD_SDIO_GetMutex(cardHandle->mutex);
#endif
	//if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD17_READ_SINGLE_BLOCK,address,&data,rspBuf))
     if(MCD_ERR_NO != mcd_Read(BlockId,buf,512))
	{
#if defined (ATV_WORKAROUND)
        CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
		return FALSE;
	}
#if defined (ATV_WORKAROUND)
    CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
	return TRUE;

}

/*****************************************************************************/
//  Description:  Read multi block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock:  start block number that you want to read
//		num: the number of block you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadMultiBlock(CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		//address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		//address = startBlock;
	}
	else
	{
		SCI_TRACE_LOW("[SCM: Card_sdio] Err Card vertion unkonwn func[%s] line[%d] ",__FUNCTION__,__LINE__);
		return FALSE;
		//CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = num;
	data.databuf = buf;
#if defined (ATV_WORKAROUND)
    CARD_SDIO_GetMutex(cardHandle->mutex);
#endif
	//if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD18_READ_MULTIPLE_BLOCK,address,&data,rspBuf))
	//{
		//SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf);
		//return FALSE;
	//}
	if(MCD_ERR_NO != mcd_Read(startBlock,buf,num*512))
	{
#if defined (ATV_WORKAROUND)
        CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
		return FALSE;
	}
#if defined (ATV_WORKAROUND)
    CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
	return TRUE;

}

/*****************************************************************************/
//  Description:  Wrtie single block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to write
//		buf : that data to be writen to card is stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteSingleBlock(CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		//address = BlockId*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		//address = BlockId;
	}
	else
	{
		SCI_TRACE_LOW("[SCM: Card_sdio] Err Card vertion unkonwn func[%s] line[%d] ",__FUNCTION__,__LINE__);
		return FALSE;
		//CARD_SDIO_ASSERT(0);	/*assert verified*/
	}


	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = 1;
	data.databuf = buf;
#if defined (ATV_WORKAROUND)
    CARD_SDIO_GetMutex(cardHandle->mutex);
#endif
	if(MCD_ERR_NO != mcd_Write(BlockId,buf,512))
	{
#if defined (ATV_WORKAROUND)
        CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
		return FALSE;
	}

#if defined (ATV_WORKAROUND)
    CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
	return TRUE;

}

/*****************************************************************************/
//  Description:  Wrtie multi block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to write
//		num: the number of block you want to write
//		buf : the data to be writen to card is stored in this place
//  Return:
//		TRUE: success
 //		FALSE: fail
 //  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteMultiBlock(CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion)||(CARD_MMC_331 == cardHandle->vertion))
	{
		//address = startBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		//address = startBlock;
	}
	else
	{
		SCI_TRACE_LOW("[SCM: Card_sdio] Err Card vertion unkonwn func[%s] line[%d] ",__FUNCTION__,__LINE__);
		return FALSE;
		//CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	//if(FALSE == _IsCardReady(cardHandle))
	//{
	//	return FALSE;
	//}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = num;
	data.databuf = buf;
#if defined (ATV_WORKAROUND)
    CARD_SDIO_GetMutex(cardHandle->mutex);
#endif
	//if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD25_WRITE_MULTIPLE_BLOCK,address,&data,rspBuf))
	
	if(MCD_ERR_NO != mcd_Write(startBlock,buf,num*512))
	{
#if defined (ATV_WORKAROUND)
        CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
		return FALSE;
	}

#if defined (ATV_WORKAROUND)
    CARD_SDIO_PutMutex(cardHandle->mutex);
#endif
	return TRUE;

}


//-----------------------------------------------------------------------------------
//	Set erase start address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SD20_SetEraseStart(CARD_SDIO_HANDLE cardHandle,uint32 startBlock)
{
	return TRUE;

#if 0
	uint8 rspBuf[16];
     
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD32_ERASE_WR_BLK_START,startBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
#endif
}

//-----------------------------------------------------------------------------------
//	Set erase end address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SD20_SetEraseEnd(CARD_SDIO_HANDLE cardHandle,uint32 endBlock)
{ 
	return TRUE;
#if 0
	uint8 rspBuf[16]; 
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD33_ERASE_WR_BLK_END,endBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
#endif
}

/*****************************************************************************/
//  Description:  erase card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to erase
//		endBlock : end block number that you want to erase
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_Erase(CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 endBlock)
{

	return TRUE;
#if 0
	uint8 rspBuf[16];
	uint32 startAddress = 0xFFFFFFFF, endAddress = 0xFFFFFFFF;
      
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)||(CARD_SD_V1_X == cardHandle->vertion))
	{
		startAddress = startBlock*cardHandle->BlockLen;
		endAddress = endBlock*cardHandle->BlockLen;
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
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
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

//set start group Id
	if(FALSE == _SD20_SetEraseStart( cardHandle,startAddress))
	{
		return FALSE;
	}
//set end group Id
	if(FALSE == _SD20_SetEraseEnd( cardHandle,endAddress))
	{
		return FALSE;
	}
//send erase command
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD38_ERASE,0,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
 //--
 #endif
}

/*****************************************************************************/
//  Description:  Get total block number of card ,block length is fix to 512 byte
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		uint32 value : total block num
//  Note: 
/*****************************************************************************/
PUBLIC uint32 CARD_SDIO_GetCapacity(CARD_SDIO_HANDLE cardHandle)
{
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	return cardHandle->Capacity;
}


#if defined(SPRD_SUPPORT_MCEX)
#error
#define ENTER_MCEX	0x80FFFF1F
PUBLIC BOOLEAN CARD_SDIO_EnterMcex(CARD_SDIO_HANDLE cardHandle,BOOLEAN* ifEnterMcex )
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;
	static __align(32) uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	cardHandle->ifEnterEC = FALSE;
	*ifEnterMcex = FALSE;
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifSupportEC)
	{
		return TRUE;
	}

	data.blkLen = 64;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD6_SWITCH_FUNC,ENTER_MCEX,&data,rspBuf))
	{
		return FALSE;
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
	return TRUE;

}


PUBLIC BOOLEAN CARD_SDIO_SendPsi(CARD_SDIO_HANDLE cardHandle,uint32 arg,uint8* buf)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,32))
	{
		return FALSE;
	}
	data.blkLen = 32;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD36_SEND_PSI,arg,&data,rspBuf))
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

	return TRUE;

}


PUBLIC BOOLEAN CARD_SDIO_ReadSecCmd(CARD_SDIO_HANDLE cardHandle,uint8* buf)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,512))
	{
		return FALSE;
	}
	data.blkLen = 512;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD34_READ_SEC_CMD,1,&data,rspBuf))
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

	return TRUE;

}

PUBLIC BOOLEAN CARD_SDIO_WriteSecCmd(CARD_SDIO_HANDLE cardHandle,uint8* buf)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,512))
	{
		return FALSE;
	}
	data.blkLen = 512;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD35_WRITE_SEC_CMD,1,&data,rspBuf))
	{
		return FALSE;
	}

	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

	return TRUE;

}

PUBLIC BOOLEAN CARD_SDIO_CtlTrm(CARD_SDIO_HANDLE cardHandle)
{
	uint8 rspBuf[16];

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifEnterEC)
	{
		return FALSE;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD37_CTL_TRM,1,0,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

#define EXIT_MCEX	0x80FFFF0F
PUBLIC BOOLEAN CARD_SDIO_ExitMcex(CARD_SDIO_HANDLE cardHandle)
{
	uint8 rspBuf[16];
	CARD_DATA_PARAM_T data;
	static __align(32) uint8 buf[64];
	CARD_CMD6_STATUS_T cmdStatus;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	if(FALSE == cardHandle->ifEnterEC)
	{
		return TRUE;
	}
	cardHandle->ifEnterEC = FALSE;

	data.blkLen = 64;
	data.blkNum = 1;
	data.databuf = buf;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD6_SWITCH_FUNC,EXIT_MCEX,&data,rspBuf))
	{
		return FALSE;
	}

	_CMD6_Response_Analyze(buf,&cmdStatus);
	if(
		(0x00 == cmdStatus.grp2SwitchResult)
		&&(0 != ((0x1<<0)&(cmdStatus.grp2Supported)))
	)
	{
		//CARD_SDIO_PRINT:"card exit mcex success"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CARD_SDIO_2367_112_2_18_1_52_32_147,(uint8*)"");
	}
	return TRUE;

}


#endif
#if defined (ATV_WORKAROUND)
/*****************************************************************************/
//  Description:  Switch SDIO base clock 
//  Author: Wenjun.Shi
//  Param
//		slotNo: slot number
//  Return:
//		Not zero: success
//		zero : fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_SwichBaseClk(CARD_SDIO_SLOT_NO slotNo, uint32 sdio_base_clk)
{
    BOOLEAN ret_val;
    CARD_SDIO_GetMutex((&cardPort[slotNo])->mutex);
    ret_val = SDIO_Card_Pal_SwichBaseClk( SDIO_Card_PAL_Get_Handle((SDIO_CARD_PAL_SLOT_E)slotNo), sdio_base_clk );
    CARD_SDIO_PutMutex((&cardPort[slotNo])->mutex);
	return ret_val;
}
#endif



