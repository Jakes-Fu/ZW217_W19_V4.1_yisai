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
//		SCI_FTLֻ���ͻ���api���м򵥵ķ�װ������ڲ����ļ������̿���
//		API��Ϊ����:
//			1 �� uplayerӦ�ò��API
//				1.1 �豸������API
//				1.2 partition ������API
//			2 �Կͻ���ftlע���õ�API
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
//				��Ӧ�ò�Ľӿ�
/************************************************************************************************/

/*-----------------------------------------------------------------*/
//	NO_OS
//	������ʼ��SCI_FTLģ�� ��ͬʱҲ��ʼ���ͻ���FTL���
//
/*-----------------------------------------------------------------*/
/*
	SCI_FTL_ERROR_NONE					no error
	SCI_FTL_ERROR_SYSTEM				other error
*/
uint32 SCI_FTL_MODULE_Init(void);

/*-----------------------------------------------------------------*/
//
//	��ʽ��nandIdָ����nand�豸��SCI_FTLֻ�ṩ��������
//	һ����nandԭʼ���ԣ�һ������sct����
//
//	SCI_FTL_Formatֻ��ʽ��nand����������Ҳ����˵��ʽ�������в�����
//	�κο��ƽṹ���ں�������ʱ�ͷ�
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
#define SCI_FTL_MODULE_SCT	(0<<8)		// block ������
#define SCI_FTL_MODULE_BLK	(1<<8)		// sect������
// 0-7 bit
#define SCI_FTL_ATTR_RO		(0<<0)		// ��ʼΪֻ����PARITION��IO����ɸı�������
#define SCI_FTL_ATTR_RW		(1<<0)		// ��ʼΪ�ɶ�д��PARITION��IO����ɸı�������
#define SCI_FTL_ATTR_FROZEN	(2<<0)		// ��ʼΪֻ����PARTITION��IO�����޷��ı������ԣ�Device ��IO������Ըı�������
// ---
#define SCI_FTL_GET_MODULE(attr)	(attr&0xFF00)
#define SCI_FTL_GET_ATTR(attr)	(attr&0xFF)

/*

*/
#define SCI_FTL_REMAIN_AREA			((uint32)-1)	// ����ʱ�ò�����ʾ������С���쵽��һ��������ʼ֮ǰ���������
#define SCI_FTL_MAX_PARTS_PER_NAND	100			// ��������
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
//	���豸���󣬺�������ɹ����أ����ʾ��nand��
//	������Ϣ���ڴ��д��ڡ���API�ɹ�����֮�󣬺����API�ſ��Ե���
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
//	�õ��豸����Ļ�����Ϣ��������SCI_FTL_Load�ɹ�֮�����ʹ��
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
//	�ͷ��豸����
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
//	��partition���󣬱�����SCI_FTL_Load���豸����֮�����ʹ��
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
//	�õ�partition���������
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
//	��partition������ж�������SCI_FTL����partition��������ԶԲ���
//	������Чֵ�жϣ��ͻ��˺���ָ�벻�ع��Ĳ�����Ч��
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
//	��partition�������д������SCI_FTL����partition��������ԶԲ���
//	������Чֵ�жϣ��ͻ��˺���ָ�벻�ع��Ĳ�����Ч��
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
//	��partition������в���������FTL�����partition��������ԶԲ���
//	������Чֵ�ж�
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
//	��partition������ж�������SCI_FTL����partition��������ԶԲ���
//	������Чֵ�жϣ��ͻ��˺���ָ�벻�ع��Ĳ�����Ч��
//
//	��API�ṩ��MMUʹ��
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
//	�ر�partition����
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
			�豸��������Ƿ�������
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
//				�Եײ�ͻ���ftl����Ľӿ�
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

