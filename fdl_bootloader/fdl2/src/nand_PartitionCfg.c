


#include "sci_types.h"
#include "nand_partitionCfg.h"
#include "nand_partTable_16k.h"
#include "nand_partTable_128k.h"

/*
	blkRank:
		0: 16k
		1: 16k*2^1	= 32k
		2: 16k*2^2	= 64k
		3: 16k*2^3	= 128k
		4: 16k*2^4	= 256k
	partCfg:
		partition config
*/

/*LOCAL*/typedef struct
{
/*I*/	uint32			blkSize;	// unit is bytes
/*O*/	NAND_PARTITION*	partCfg;
}_PARTITION_CFG;

const _PARTITION_CFG _partCfgGrp[] =
{
	{ 16*1024,	&PartI_16k	},
	{ 128*1024,	&PartI_128k	},
	{ 0			,0				}
};

PUBLIC const NAND_PARTITION *GetPartCfg (uint32 blkSize)
{
	uint32 i;

	i = 0;
	while(0 != _partCfgGrp[i].blkSize)
	{
		if(blkSize == _partCfgGrp[i].blkSize)
		{
			return _partCfgGrp[i].partCfg;
		}
		i++;
	}
	return (NAND_PARTITION const *)0;
}


