
#ifndef _NAND_PARTITION_CFG_H_
#define _NAND_PARTITION_CFG_H_

#include "sci_types.h"

//------------------------------------------------------------------------
//	NAND_FULL_SIZE means:
//		From current blkStart to the next blkStart is occupied
//------------------------------------------------------------------------
#define NAND_REMAIN_AREA		((uint32)-1)

//------------------------------------------------------------------------
//	NAND_PARTITION_MAX_NUM:
//		max partition num can be create in one nand
//------------------------------------------------------------------------
#define NAND_PARTITION_MAX_NUM	30

//------------------------------------------------------------------------
//	partition define
//------------------------------------------------------------------------
typedef struct
{
	uint32	partId;
	uint32	attr;
	uint32	blkStart;
	uint32	blkNum;
}NAND_PARTITION_ELM;

typedef struct
{
	uint32				partNum;
	NAND_PARTITION_ELM	partI[NAND_PARTITION_MAX_NUM];
}NAND_PARTITION;

//------------------------------------------------------------------------
//	Get suitable partition config of current project
//------------------------------------------------------------------------
PUBLIC const NAND_PARTITION *GetPartCfg (uint32 blkSize);
#endif
