/******************************************************************************
 ** File Name:      sci_ftl_api.h                                             *
 ** Author:         Jason.Wu                                                  *
 ** DATE:           2010.12.14                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.12.14   Jason.Wu              Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _SCI_FTL_API_H_
#define _SCI_FTL_API_H_
/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/************************************************************************************************/
//
//	NOTES:
//		SCI_FTL只做客户端api进行简单的封装，对入口参数的检查和流程控制
//		API分为两类:
//			1 对 uplayer应用层的API
//				1.1 设备对象类API
//				1.2 partition 对象类API
//			2 对客户端ftl注册用的API
/************************************************************************************************/

/*-----------------------------------------------------------------------------------------------
SCI_FTL_MODULE_Init
	|----------------
	|				|
SCI_FTL_Load	SCI_FTL_Format
	|
	|
	|----------------------------------------------------
	|					|					|			|
SCI_FTL_Open	SCI_FTL_UnLoad	SCI_FTL_GetDevInfo	SCI_FTL_IOCtl
	|
	|
	|
	|----------------------------------------------------------------
	|				|					|				|			|
SCI_FTL_Read	SCI_FTL_Write	SCI_FTL_Erase	SCI_FTL_Close	SCI_FTL_IOCtl

-----------------------------------------------------------------------------------------------*/
#define	SCI_FTL_ERROR_NONE			0
#define SCI_FTL_ERROR_MACHINESTATE	1		// machine state error
#define SCI_FTL_ERROR_INVALIDPARAM	2		// param error
#define SCI_FTL_ERROR_UNFORMATED		3		// nand unformated

#define SCI_FTL_ERROR_RISK			4		// read has some bit risk
#define SCI_FTL_ERROR_FAIL			5		// read fail or write fail
#define SCI_FTL_ERROR_TRANSFERFAIL	6		// controller fail

#define SCI_FTL_ERROR_SYSTEM			7		// other error

/************************************************************************************************/
//
//				Application infterface to application
//				对应用层的接口
/************************************************************************************************/

/*-----------------------------------------------------------------*/
//	NO_OS
//	用来初始化SCI_FTL模块 ，同时也初始化客户端FTL软件
//
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_MODULE_Init(void);

/*-----------------------------------------------------------------*/
//
//	格式化nandId指定的nand设备，SCI_FTL只提供两种属性
//	一种是nand原始属性，一种则是sct属性
//
//	SCI_FTL_Format只格式化nand，但不加载也就是说格式化过程中产生的
//	任何控制结构都在函数返回时释放
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE	
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					fail

	SCI_FTL_ERROR_SYSTEM				other error
*/


/*
	only have following 4 attibe group
	1 SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW
	2 SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO|SCI_FTL_ATTR_FROZEN
	3 SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
	4 SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RW

	other choise is invalid
*/
// 8-16 bit
#define SCI_FTL_MODULE_SCT	(0<<8)		// block 类属性
#define SCI_FTL_MODULE_BLK	(1<<8)		// sect类属性
// 0-7 bit
#define SCI_FTL_ATTR_RO		(0<<0)		// 初始为只读，PARITION级IO命令可改变其属性
#define SCI_FTL_ATTR_RW		(1<<0)		// 初始为可读写，PARITION级IO命令可改变其属性
#define SCI_FTL_ATTR_FROZEN	(2<<0)		// 初始为只读，PARTITION级IO命令无法改变其属性，Device 级IO命令可以改变其属性
// ---
#define SCI_FTL_GET_MODULE(attr)	(attr&0xFF00)
#define SCI_FTL_GET_ATTR(attr)	(attr&0xFF)

/*

*/
#define SCI_FTL_REMAIN_AREA			((uint32)-1)	// 分区时该参数表示分区大小延伸到下一个分区开始之前，方便计算
#define SCI_FTL_MAX_PARTS_PER_NAND	100			// 最大分区数
typedef struct
{
	uint32	partId;
	uint32	attrib;								// atrrib of partition
	uint32	ofst;								// partition size
	uint32	len;
}SCI_FTL_PART_ENTRY;
typedef struct
{
	uint32					partsNum;
	SCI_FTL_PART_ENTRY		parts[SCI_FTL_MAX_PARTS_PER_NAND];
}SCI_FTL_PART;
uint32 SCI_FTL_Format(uint32 nandId, SCI_FTL_PART* partsInfo);

/*-----------------------------------------------------------------*/
//	NO_OS
//	打开设备对象，函数如果成功返回，则表示该nand的
//	控制信息在内存中存在。该API成功返回之后，后面的API才可以调用
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_UNFORMATED			nand unformated
	SCI_FTL_ERROR_FAIL					fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Load(uint32 nandId);

/*-----------------------------------------------------------------*/
//
//	得到设备对象的基本信息，必须在SCI_FTL_Load成功之后才能使用
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef struct
{
	//---basic info
	uint32			sctSize;
	uint32			sctsPerPg;
	uint32			pgPerBlk;
	//---partition info
	SCI_FTL_PART	partsInfo;
}SCI_FTL_DEV_INFO;
uint32 SCI_FTL_GetDevInfo(uint32 nandId, SCI_FTL_DEV_INFO* devInfo);

/*-----------------------------------------------------------------*/
//	NO_OS
//	释放设备对象
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_UnLoad(uint32 nandId);


/*-----------------------------------------------------------------*/
//
//	打开partition对象，必须在SCI_FTL_Load打开设备对象之后才能使用
/*-----------------------------------------------------------------*/
typedef uint32 SCI_FTL_HANDLE;
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					no this partition

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Open(uint32 nandId, uint32 partId, SCI_FTL_HANDLE* ftlHandle, uint32 jumpPoint);

/*-----------------------------------------------------------------*/
//
//	得到partition对象的属性
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef struct
{
	uint16	attrib;
	//---basic info
	uint32	sctSize;
	uint32	sctTotalNum;
	//---
	uint32	eraseUnit;			// unit is sect num
}SCI_FTL_PARTITION_INFO;

uint32 SCI_FTL_GetPartInfo(SCI_FTL_HANDLE ftlHandle, SCI_FTL_PARTITION_INFO* partitionInfo);

/*-----------------------------------------------------------------*/
//
//	对partition对象进行读操作，SCI_FTL根据partition对象的属性对参数
//	进行有效值判断，客户端函数指针不必关心参数有效性
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_RISK					read has some bit risk
	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Read(SCI_FTL_HANDLE ftlHandle, uint32 sctId, uint32 sctNum, uint8* buf);

/*-----------------------------------------------------------------*/
//
//	对partition对象进行写操作，SCI_FTL根据partition对象的属性对参数
//	进行有效值判断，客户端函数指针不必关心参数有效性
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Write(SCI_FTL_HANDLE ftlHandle, uint32 sctId, uint32 sctNum, uint8* buf);

/*-----------------------------------------------------------------*/
//
//	对partition对象进行擦除操作，FTL会跟据partition对象的属性对参数
//	进行有效值判断
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					erase fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Erase(SCI_FTL_HANDLE ftlHandle, uint32 sctId, uint32 sctNum);

/*-----------------------------------------------------------------*/
//
//	对partition对象进行读操作，SCI_FTL根据partition对象的属性对参数
//	进行有效值判断，客户端函数指针不必关心参数有效性
//
//	该API提供给MMU使用
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_RISK					read has some bit risk
	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_DPRead(SCI_FTL_HANDLE ftlHandle, uint32 sctId, uint32 sctNum, uint8* buf);

/*-----------------------------------------------------------------*/
//
//	关闭partition对象
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_Close(SCI_FTL_HANDLE ftlHandle);

/*
	object: device object or partition object
			设备对象或者是分区对象
*/
/*-----------------------------------*/
// device object command
#define SCI_FTL_Operation_UnLock			(0x00000001)
#define SCI_FTL_Operation_Lock			(0x00000002)
/*-----------------------------------*/
// parititon object command
#define SCI_FTL_Operation_ChangeAttr		(0x00000003)
/*-----------------------------------*/
// get bad block map info
#define SCI_FTL_Get_MapInfo				(0x00000010)
typedef struct
{
	uint32	badBlk;
	uint32	rpBlk;
}_SCI_FTL_RP_MAP;
#define MAP_MAX_NUM	300
typedef struct
{
	uint32	mapNum;
	_SCI_FTL_RP_MAP	map[MAP_MAX_NUM];
}SCI_FTL_MAP_INFO;
/*-----------------------------------*/
// get nand basic info, such as nand ID, sector size, etc.
#define SCI_FTL_Get_NandPhysicalInfo				(0x00000011)
typedef struct
{
	unsigned __int64 ID;
	uint32		blkNum;		// total block number
	uint32		sctSize;	// sector area size , unit is bytes.
	uint32		spareSize;	// spare area size , unit is bytes.
	uint32		sctPerPg;	// sectors per page
	uint32		pgPerBlk;	// page per block
	uint32		wnPerPg;	// program times per page
}SCI_FTL_NAND_HW_INFO;

/*-----------------------------------*/

// IOCTL
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_MACHINESTATE			follow error or this partition has been opened already
	SCI_FTL_ERROR_INVALIDPARAM			param error
*/
uint32 SCI_FTL_IOCtl(void* object, uint32 ioCmd, const void *pIn, uint32 InSize,
	void *pOut, uint32 OutSize, uint32 *pBytesReturned);


/************************************************************************************************/
//
//				Application infterface to low layer
//				对底层客户端ftl软件的接口
/************************************************************************************************/


typedef void (*_FTL_MODULE_Init)(void);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Format)(uint32 nandId, SCI_FTL_PART* partsInfo);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_UNFORMATED			nand unformated
	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Load)(uint32 nandId, SCI_FTL_DEV_INFO* devInfo);


/*
	SCI_FTL_ERROR_NONE					no error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_UnLoad)(uint32 nandId);

/*
	SCI_FTL_ERROR_NONE					no error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Open)(uint32 nandId, uint32 partId, void** userDataContext,uint32 jumpPoint);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_GetPartInfo)(void* userDataContext, SCI_FTL_PARTITION_INFO* partitionInfo);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_RISK					read has some bit risk
	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Read)(void* userDataContext, uint32 sctId, uint32 sctNum, uint8* buf);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Write)(void* userDataContext, uint32 sctId, uint32 sctNum, uint8* buf);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Erase)(void* userDataContext, uint32 sctId, uint32 sctNum);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_RISK					read has some bit risk
	SCI_FTL_ERROR_FAIL					read fail or write fail
	SCI_FTL_ERROR_TRANSFERFAIL			controller fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_DPRead)(void* userDataContext, uint32 sctId, uint32 sctNum, uint8* buf);

/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_Close)(void* userDataContext);


/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_INVALIDPARAM			param error

	SCI_FTL_ERROR_FAIL					read fail or write fail

	SCI_FTL_ERROR_SYSTEM				other error
*/
typedef uint32 (*_FTL_IOCtl)(void* object, uint32 ioCmd, const void *pIn, uint32 InSize,
	void *pOut, uint32 OutSize, uint32 *pBytesReturned);

typedef struct
{
//--- device object
	_FTL_MODULE_Init	_init;
	_FTL_Format			_format;
	_FTL_Load			_load;
	_FTL_UnLoad			_unLoad;
//--- partition object
	_FTL_Open			_open;
	_FTL_GetPartInfo	_GetPartInfo;
	_FTL_Read			_read;
	_FTL_Write			_write;
	_FTL_Erase			_erase;
	_FTL_DPRead			_dpRead;
	_FTL_Close			_close;
//---
	_FTL_IOCtl		_ioctl;
}SCI_FTL_PROBE;

/*
	This function must be applied by custom_drv. It return a function ptr group.
	sci ftl layer will use it to manage nand flash
	return:
		none zero:	success
		zero	:	fail
*/
SCI_FTL_PROBE* SCI_GetVendorSwProbe(unsigned __int64 ID);

/******************************************************************************/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif

