#include "sci_types.h"



#ifndef __CARD_DRV_H_
#define __CARD_DRV_H_

struct CARD_Card_struct;
typedef struct CARD_Card_struct* CARD_SPI_HANDLE;

typedef enum CARD_SPIPhyPort_enum
{
	CARD_SPI_PORT_0 = 0,
	CARD_SPI_PORT_1,
	CARD_SPI_PORT_2,
	CARD_MAX_SPI_NUM
}
CARD_SPIPORT_E;

typedef enum CARD_SPICS_enum
{
	CARD_CS0 = 0,
	CARD_CS1,
	CARD_CS2,
	CARD_CS3,
	CARD_MAX_CS_NUM
}
CARD_SPICS_E;

//PUBLIC void _CARD_Init(void);

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
PUBLIC CARD_SPI_HANDLE CARD_SPI_Open(  // success - return the hardware interface vector
                                                            // failure - return NULL
	CARD_SPIPORT_E SPI_Id, // SPI port num
	CARD_SPICS_E CS_Id, // Chip select id
	uint32 Max_ClockRate // SPI clock
);

typedef enum
{
	CARD_SPI_PWR_OFF_AFTER,
	CARD_SPI_PWR_ON_PRE
}CARD_SPI_PWR_E;
PUBLIC BOOLEAN CARD_SPI_PwrHook(CARD_SPI_HANDLE cardHandle, CARD_SPI_PWR_E onOrOff);

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
PUBLIC BOOLEAN CARD_SPI_Close(CARD_SPI_HANDLE cardHandle);


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
PUBLIC BOOLEAN CARD_SPI_SetBlockLength(CARD_SPI_HANDLE cardHandle,uint32 length);


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
//PUBLIC BOOLEAN CARD_SPI_ReadOCR(CARD_SPI_HANDLE cardHandle,uint32* OCR);


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
//PUBLIC BOOLEAN CARD_SPI_ReadCID(CARD_SPI_HANDLE cardHandle,CARD_CID_T* CID);


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
//PUBLIC BOOLEAN CARD_SPI_ReadCSD(CARD_SPI_HANDLE cardHandle,CARD_CSD_STRUCT* CSD);


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
PUBLIC BOOLEAN CARD_SPI_InitCard(CARD_SPI_HANDLE cardHandle);


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
PUBLIC BOOLEAN CARD_SPI_ReadSingleBlock(CARD_SPI_HANDLE cardHandle,uint32 BlockId,uint8* buf);


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
PUBLIC BOOLEAN CARD_SPI_WriteSingleBlock(CARD_SPI_HANDLE cardHandle,uint32 BlockId,uint8* buf);


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
PUBLIC BOOLEAN CARD_SPI_ReadMultiBlock(CARD_SPI_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);


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
PUBLIC BOOLEAN CARD_SPI_WriteMultiBlock(CARD_SPI_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);


/********************************************************************


********************************************************************/
typedef struct
{
	uint32	num;
	uint8*	buf;
}CARD_SPI_SCTBUF_ITEM_INFO_T;

typedef struct
{
	uint32				cnt;
	CARD_SPI_SCTBUF_ITEM_INFO_T*	item;
}
CARD_SPI_SCTBUF_INFO_T;

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
PUBLIC BOOLEAN CARD_SPI_ReadMultiBlock_Ext(CARD_SPI_HANDLE cardHandle,uint32 startBlock,CARD_SPI_SCTBUF_INFO_T* sctInfo);

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
PUBLIC BOOLEAN CARD_SPI_WriteMultiBlock_Ext(CARD_SPI_HANDLE cardHandle,uint32 startBlock,CARD_SPI_SCTBUF_INFO_T* sctInfo);

/********************************************************************
*	Description: This function is used to Read serval number continue blocks from MMC card
*	Dependence: the MMC must have run MMC_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by MMC_Register
*		startBlock: that the first block Id you want to read
*		num: the number blocks that you want to read from MMC
*		buf: the space used to save the value that readout from MMC
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_MMC_ReadMultiBlock_PreSetNum(CARD_SPI_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);

/********************************************************************
*	Description: This function is used to write serval number continue blocks to MMC card
*	Dependence: the MMC must have run MMC_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by MMC_Register
*		startBlock: that the first block Id you want to write
*		num: the number blocks that you want to write to MMC
*		buf: the space used to save the value that will be writen to MMC
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC BOOLEAN CARD_SPI_MMC_WriteMultiBlock_PreSetNum(CARD_SPI_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);

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
PUBLIC BOOLEAN CARD_SPI_Erase(CARD_SPI_HANDLE cardHandle,uint32 startBlock,uint32 endBlock);


/********************************************************************
*	Description: get the capacity of SD card
*	Dependence: the SD must have run CARD_SPI_InitCard success
*	Author: Jason.wu
*	Param:
*		cardHandle: the handler returned by CARD_SPI_Open
*	Result:
*		None zero:the Capacity, unit is byte
*		0: Fail
********************************************************************/
PUBLIC uint32 CARD_SPI_GetCapacity(CARD_SPI_HANDLE cardHandle);


PUBLIC BOOLEAN CARD_SPI_EnterMcex(CARD_SPI_HANDLE cardHandle,BOOLEAN* ifEnterMcex);

PUBLIC BOOLEAN CARD_SPI_SendPsi(CARD_SPI_HANDLE cardHandle,uint32 arg,uint8* buf);

PUBLIC BOOLEAN CARD_SPI_ReadSecCmd(CARD_SPI_HANDLE cardHandle,uint8* buf);

PUBLIC BOOLEAN CARD_SPI_WriteSecCmd(CARD_SPI_HANDLE cardHandle,uint8* buf);

PUBLIC BOOLEAN CARD_SPI_CtlTrm(CARD_SPI_HANDLE cardHandle);

PUBLIC BOOLEAN CARD_SPI_ExitMcex(CARD_SPI_HANDLE cardHandle);

#endif

