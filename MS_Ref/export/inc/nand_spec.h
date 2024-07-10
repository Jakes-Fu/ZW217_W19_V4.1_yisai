
#ifndef _NAND_SPEC_H_
#define _NAND_SPEC_H_

#include "sci_types.h"

#define BW_X08			0
#define BW_X16			1
#define BW_X32			2

#define BVD_3CYCLES	3
#define BVD_4CYCLES	4
#define BVD_5CYCLES	5

#define BVD_ADVANCE_0	0
#define BVD_ADVANCE_1	1
typedef struct
{
	unsigned __int64	ID;

	uint32		nBlkNum;
	uint32		nNumOfPlane;			/* not support yet										*/
	uint32		nBlkNumInRsv;			/* NVB													*/

	uint16		nSctSize;				/* ecc protect size 512 or 1k or 2k.....				*/
	uint16		nSpareSize;			/* relate with nSctSize									*/
	uint16		nSctPerPg;
	uint16		nPgPerBlk;
	uint8		nWCntPerPg;			/* partial program times per page						*/

	uint8		badPgId;
	uint8		badSctId;
	uint8		badPos;					/* unit is bytes, lenth is 1 bytes default				*/
	uint8		badLen;					/* bad flag byes lenth									*/

	int8		safeDataPos;			/* data that be protected by ecc(-1 is all reserved area)*/
	uint8		safeDataLen;			/* data that be protected by ecc(0 is none)				*/

	uint8		mEccPos;				/* unit is bytes										*/
	uint8		mEccBitLen;			/* unit is bits											*/
	uint8		mEccRiskBitNum;		/* unit is bits,This value must small to ecc capability	*/

	int8		sEccPos;				/* unit is bytes										*/
	int8		sEccBitLen;			/* unit is bits											*/
	int8		sEccRiskBitNum;		/* unit is bits,This value must small to ecc capability	*/

//	host driver need next infomation
	uint8		nBWidth;				/* Nand Organization X8 or X16							*/
	uint8		nCycleDev;				/* 3 Cycle, 4 Cycle, 5 Cycle device						*/
	uint8		nAdvance;				/* the property of Advance								*/

    uint16		nTrTime;				/* read-time of NAND device (unit: usec)				*/
    uint16		nTwTime;				/* write-time of NAND device (unit : usec)				*/
    uint16		nTeTime;				/* erase-time of NAND device (unit : usec)				*/
    uint16		nTfTime;				/* transfer-time from NAND device to host				*/
}NAND_SPEC;


NAND_SPEC* GetNandParam(unsigned __int64 ID);

#endif

