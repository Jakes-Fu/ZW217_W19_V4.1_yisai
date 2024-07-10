

#include "sci_types.h"
#include "sci_ftl_api.h"
#include "nand_partitionCfg.h"
#include "fdl_nand.h"
#include <string.h>

//-----------------------------------------------
#include "NAND_PartitionId.h"
#include "nand_spec.h"
#include "sc_reg.h"
#include "nand_controller.h"

PUBLIC BOOLEAN       g_IsNbl;
static BOOLEAN			s_bootEccEn;

static const NAND_SPEC	*s_spec;

static uint32			NBLDataBufPos;
#define					NBL_BUF_SIZE	0x18000		// 16k(boot0)+80k(boot1) = 96k
static uint8			NBLDataBuf[NBL_BUF_SIZE];
static uint8			NBLMBuf[2048];
static uint8			NBLSBuf[64];

//-----------------------------------------------


#define ADDR_MASK               0x80000000

/* cr224849 fei.zhang */
//Input 0x4xxx_xxxx to read main data
//Input 0x4xxx_xxx4 to read spare data
#define READ_PHYNAND_MASK			0x40000000
#define READ_PHYNAND_SPR_MASK	0x00000004
static unsigned int g_NandSize;   /* in bytes */

#define _S_TMPBUF_LEN	(1024*128)
SCI_FTL_HANDLE		ftlHandle;
static uint32		s_sctId;
static uint32		s_tmpBufPos;
static uint8		s_tmpBuf[_S_TMPBUF_LEN];


//-----------------------------------------------
static void _UpateEccEnable(void)
{
	uint32 chipId = 0;
	chipId = *((volatile uint32*)CHIP_TYPE);

	switch (chipId)
	{
		case 0x8850A013: // 8800s4
			s_bootEccEn = TRUE;
		break;
		case 0x8850A012: // 8800s4
			s_bootEccEn = TRUE;
		break;
		case 0x68000000: // SC6800H1
			s_bootEccEn = TRUE;
		break;
        case 0x68030000: // SC6800H3
            s_bootEccEn = TRUE;
        break;                    
		case 0x88000000:	//8800G1
			s_bootEccEn = TRUE;
		break;
		case 0x88000002:	//8800G2
			s_bootEccEn = TRUE;
		break;
		default:
			s_bootEccEn = FALSE;
	}
}
//-----------------------------------------------

static BOOLEAN _PartitionSwap(SCI_FTL_PART* ftlPartitionParam, const NAND_PARTITION* nandParts, const NAND_SPEC* spec)
{
	uint32 i,k;
	SCI_FTL_PART_ENTRY tmpEntry;

	// 0 check risk
	if(
		(SCI_FTL_MAX_PARTS_PER_NAND < nandParts->partNum)
		||(NAND_PARTITION_MAX_NUM < nandParts->partNum)
	)
	{
		return FALSE;
	}
	// 1 load info
	ftlPartitionParam->partsNum		= nandParts->partNum;
	for(i = 0; i < nandParts->partNum; i++)
	{
		ftlPartitionParam->parts[i].partId	= nandParts->partI[i].partId;
		ftlPartitionParam->parts[i].attrib	= nandParts->partI[i].attr;
		ftlPartitionParam->parts[i].ofst		= nandParts->partI[i].blkStart;
		if(NAND_REMAIN_AREA == nandParts->partI[i].blkNum)
		{
			ftlPartitionParam->parts[i].len		= SCI_FTL_REMAIN_AREA;
		}
		else
		{
			ftlPartitionParam->parts[i].len		= nandParts->partI[i].blkNum;
		}
	}
	// 2 order by ofst
	for(i = 0; i < ftlPartitionParam->partsNum; i++)
	{
		for(k = i+1; k < ftlPartitionParam->partsNum; k++)
		{
			if(ftlPartitionParam->parts[i].ofst > ftlPartitionParam->parts[k].ofst)
			{// swap
				tmpEntry = ftlPartitionParam->parts[k];
				ftlPartitionParam->parts[k] = ftlPartitionParam->parts[i];
				ftlPartitionParam->parts[i] = tmpEntry;
			}
			else if(ftlPartitionParam->parts[i].ofst == ftlPartitionParam->parts[k].ofst)
			{// param error
				return FALSE;
			}
		}
	}
	// 3 replace SCI_FTL_REMAIN_AREA by number
	for(i = 0; i < ftlPartitionParam->partsNum-1; i++)
	{
		if(SCI_FTL_REMAIN_AREA == ftlPartitionParam->parts[i].len)
		{
			ftlPartitionParam->parts[i].len = ftlPartitionParam->parts[i+1].ofst - ftlPartitionParam->parts[i].ofst;
		}
	}
	if(SCI_FTL_REMAIN_AREA == ftlPartitionParam->parts[i].len)
	{
		ftlPartitionParam->parts[i].len = spec->nBlkNum - spec->nBlkNumInRsv	- ftlPartitionParam->parts[i].ofst;
	}

	return TRUE;
}

/*
	input param must has been order already
*/
static BOOLEAN _PartitionCmp(const SCI_FTL_PART* ftlParts0, const SCI_FTL_PART* ftlParts1)
{
	uint32 i;

	if(ftlParts0->partsNum != ftlParts1->partsNum)
	{
		return FALSE;
	}

	for(i = 0; i < ftlParts0->partsNum; i++)
	{
		if(
			(ftlParts0->parts[i].partId	!= ftlParts1->parts[i].partId)
			||(ftlParts0->parts[i].attrib	!= ftlParts1->parts[i].attrib)
			||(ftlParts0->parts[i].ofst	!= ftlParts1->parts[i].ofst)
			||(
				(ftlParts0->parts[i].len	!= ftlParts1->parts[i].len)
				&&(SCI_FTL_REMAIN_AREA != ftlParts0->parts[i].len)
				&&(SCI_FTL_REMAIN_AREA != ftlParts1->parts[i].len)
			)
		)
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*
	success	: VBM Load state
	fail	: VBM UnLoad state
*/
int nand_flash_init (void)
{
//---
	uint32			nandId;
//---
	const NAND_PARTITION *partCfg;
	SCI_FTL_PART	ftlPartitionParam;
	SCI_FTL_DEV_INFO	ftlDevInfo;
	uint32 lRet;
//-----------------------------------------------
	unsigned __int64	ID = 0;
	uint8	idArr[4] = {0,0,0,0};

#if  defined(PLATFORM_SC6800H)
	NANDCTL_Init(100000000);
#elif  defined(PLATFORM_SC8800G)
    if(0 == (REG32(GR_GEN3) & 0x3))      // 8800g2: ARM400_EMC200_AHB100
		NANDCTL_Init(100000000);
    else if(1 == (REG32(GR_GEN3) & 0x3)) // 8801G2: ARM256M_EMC192M_AHB64M
		NANDCTL_Init(64000000);
    else if(2 == (REG32(GR_GEN3) & 0x3)) // 8802G2: ARM192M_EMC192M_AHB96M
		NANDCTL_Init(96000000);
    else
        for( ; ; ) {}
#else
#endif

    NANDCTL_Reset();


	NANDCTL_Read_ID(idArr);
	ID = (((unsigned __int64)idArr[0])<<8)|((unsigned __int64)idArr[1]);
	s_spec = GetNandParam(ID);
	if(0 == s_spec)
	{
		return NAND_SYSTEM_ERROR;
	}
	
    /* cr224849 fei.zhang */
    g_NandSize = s_spec->nSctSize*s_spec->nPgPerBlk*s_spec->nSctPerPg*s_spec->nBlkNum;
	
	partCfg = GetPartCfg (s_spec->nSctSize*s_spec->nPgPerBlk*s_spec->nSctPerPg);
	if(0 == partCfg)
	{
		return NAND_SYSTEM_ERROR;
	}
	if(FALSE == _PartitionSwap(&ftlPartitionParam, partCfg,s_spec))
	{
		return NAND_SYSTEM_ERROR;
	}
//-----------------------------------------------

	nandId = 0;
	g_IsNbl = FALSE;
	// 0 check chip whether support ecc boot
	_UpateEccEnable();
//-----------------------------------------------
	if(SCI_FTL_ERROR_NONE != SCI_FTL_MODULE_Init())
	{
		return NAND_SYSTEM_ERROR;
	}

	lRet = SCI_FTL_Load(nandId);
	if(SCI_FTL_ERROR_UNFORMATED == lRet)
	{
		// format force
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Format(nandId, &ftlPartitionParam))
		{
			return NAND_SYSTEM_ERROR;
		}
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Load(nandId))
		{
			return NAND_SYSTEM_ERROR;
		}
	}
	else if(SCI_FTL_ERROR_NONE != lRet)
	{
		return NAND_SYSTEM_ERROR;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetDevInfo(nandId, &ftlDevInfo))
	{
		SCI_FTL_UnLoad(nandId);
		return NAND_SYSTEM_ERROR;
	}
	if(TRUE != _PartitionCmp(&ftlDevInfo.partsInfo, &ftlPartitionParam))
	{
		//SCI_FTL_UnLoad(nandId);
		return NAND_INCOMPATIBLE_PART;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_IOCtl((void*)nandId, SCI_FTL_Operation_UnLock, 0, 0, 0, 0, 0))
	{
		SCI_FTL_UnLoad(nandId);
		return NAND_SYSTEM_ERROR;
	}
//-----------------------------------------------
	return NAND_SUCCESS;
}

/*
	success	: VBM Load state
	fail	: VBM UnLoad state
*/
int nand_format(void)
{
	uint32			nandId;
	const NAND_PARTITION*	partCfg;
//---
	SCI_FTL_PART	ftlPartitionParam;
//---
	unsigned __int64	ID = 0;
	uint8	idArr[4] = {0,0,0,0};
//----------------------------------
	nandId = 0;

	SCI_FTL_UnLoad(nandId);
//----------------------------------

	// 1 Get nand param
    NANDCTL_Reset();

	NANDCTL_Read_ID(idArr);
	ID = (((unsigned __int64)idArr[0])<<8)|((unsigned __int64)idArr[1]);
	s_spec = GetNandParam(ID);
	if(0 == s_spec)
	{
		return NAND_SYSTEM_ERROR;
	}
	// 2 get system partition param
	partCfg = GetPartCfg (s_spec->nSctSize*s_spec->nPgPerBlk*s_spec->nSctPerPg);
	if(0 == partCfg)
	{
		return NAND_SYSTEM_ERROR;
	}
	if(FALSE == _PartitionSwap(&ftlPartitionParam, partCfg,s_spec))
	{
		return NAND_SYSTEM_ERROR;
	}
//----------------------------------
	// 3 format device
	SCI_FTL_UnLoad(nandId);
	if(SCI_FTL_ERROR_NONE != SCI_FTL_Format(nandId, &ftlPartitionParam))
	{
		return NAND_SYSTEM_ERROR;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_Load(nandId))
	{
		return NAND_SYSTEM_ERROR;
	}

//	SCI_FTL_UnLoad(nandId);
	return NAND_SUCCESS;
}

int nand_start_write (unsigned int addr, unsigned int size)
{
	uint32			nandId;
	SCI_FTL_PARTITION_INFO	ftlPartitionInfo;
	uint32			partId;
	uint32			blkNum;
	uint32			i;
//---
	nandId = 0;
	// 1 get part ID
	if(0 == (ADDR_MASK&addr))
	{
		// not support
		return NAND_INVALID_ADDR;
	}
	partId = ((~ADDR_MASK)&addr);

	if(SCI_PARTITION_NBL == partId)
	{
		g_IsNbl = TRUE;
		NBLDataBufPos = 0;

		if(1 == s_spec->nSctPerPg)
		{
			for(i = 0; i < 8; i++)
			{
				if(ERR_NF_SUCCESS != NANDCTL_Erase_Block_S(i*s_spec->nSctSize*s_spec->nSctPerPg*s_spec->nPgPerBlk))
				{
					return NAND_SYSTEM_ERROR;
				}
			}
		}
		else if(4 == s_spec->nSctPerPg)
		{
			if(ERR_NF_SUCCESS != NANDCTL_Erase_Block_L(0))
			{
				return NAND_SYSTEM_ERROR;
			}
		}
		else
		{
			// not support
			return NAND_SYSTEM_ERROR;
		}
		return NAND_SUCCESS;
	}
	else
	{
		g_IsNbl		= FALSE;
		s_tmpBufPos	= 0;
		s_sctId		= 0;
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, partId, &ftlHandle, 4))
		{
			return NAND_SYSTEM_ERROR;
		}
		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
		{
			SCI_FTL_Close(ftlHandle);
			return NAND_SYSTEM_ERROR;
		}
		if(size > (ftlPartitionInfo.sctTotalNum*ftlPartitionInfo.sctSize))
		{
			SCI_FTL_Close(ftlHandle);
			return NAND_INVALID_SIZE;
		}
		if(0 != ftlPartitionInfo.eraseUnit)
		{
			blkNum = ftlPartitionInfo.sctTotalNum/ftlPartitionInfo.eraseUnit;
			for(i = 0; i < blkNum; i++)
			{
				if(SCI_FTL_ERROR_NONE != SCI_FTL_Erase(ftlHandle, i*ftlPartitionInfo.eraseUnit, ftlPartitionInfo.eraseUnit))
				{
					SCI_FTL_Close(ftlHandle);
					return NAND_SYSTEM_ERROR;
				}
			}
		}
		memset(s_tmpBuf,0xFF,_S_TMPBUF_LEN);
		return NAND_SUCCESS;
	}
}

//-----------------------------------------------
static unsigned short _CheckSum (const unsigned int *src, int len)
{
    unsigned int   sum = 0;
    unsigned short *src_short_ptr = PNULL;

    while (len > 3)
    {
        sum += *src++;
        len -= 4;
    }

    src_short_ptr = (unsigned short *) src;

    if (0 != (len&0x2))
    {
        sum += * (src_short_ptr);
        src_short_ptr++;
    }

    if (0 != (len&0x1))
    {
        sum += * ( (unsigned char *) (src_short_ptr));
    }

    sum  = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);

    return (unsigned short) (~sum);
}

//bootloader header flag offset from the beginning
#define BOOTLOADER_HEADER_OFFSET   32
#define NAND_PAGE_LEN              512
#define NAND_MAGIC_DATA            0xaa55a5a5

//define nand data bus len
#define NAND_BUS_SIZE_8              8
#define NAND_BUS_SIZE_16              16
#define NAND_BUS_SIZE_32              32

#define ECC_BOOT0_SIZE 0x4000
#define ECC_BOOT1_SIZE 0x14000
#define _BOOT0_NUM	3

static void _setHeadInfo(void)
{
    unsigned int       *start_addr = (unsigned int*)NBLDataBuf;
    unsigned short     check_sum = 0;

    //set pointer to nand parameter config start address
    start_addr += BOOTLOADER_HEADER_OFFSET/4;

    //set nand page attribute
    *(start_addr+1) = ((1 == s_spec->nSctPerPg)?0:1);
    //set nand address cycle
    *(start_addr+2) = s_spec->nCycleDev;
    //set nand data bus len
    if(BW_X08 == s_spec->nBWidth)
    {
        *(start_addr+3) = NAND_BUS_SIZE_8;
    }
    else if (BW_X16 == s_spec->nBWidth)
    {
        * (start_addr+3) = NAND_BUS_SIZE_16;
    }
    else if (BW_X32 == s_spec->nBWidth)
    {
        * (start_addr+3) = NAND_BUS_SIZE_32;
    }

    if (TRUE != s_bootEccEn) // for 6800h
    {
        //set magic data
        * (start_addr+4) = NAND_MAGIC_DATA;

        //make checksum of first 504 bytes
        check_sum = _CheckSum ( (unsigned int *) (start_addr+1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }
    else
    {
        if (BVD_ADVANCE_1 == s_spec->nAdvance)
        {
            *(start_addr+4) = 1;
        }
        else
        {
            *(start_addr+4) = 0;
        }

        //set magic data
        *(start_addr+5) = NAND_MAGIC_DATA;

        //make checksum of first 504 bytes
        check_sum = _CheckSum ( (unsigned int *) (start_addr+1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }

    //set checksum
    * (start_addr) = (unsigned int) check_sum;
    return;
}

//-----------------------------------------------

int nand_write (unsigned int size, unsigned char *buf)
{
	uint32			sctSize;
	uint8			*tmpBuf;
	uint32			tmpSize;
	SCI_FTL_PARTITION_INFO	ftlPartitionInfo;

	if(TRUE == g_IsNbl)
	{
		if(NBL_BUF_SIZE < (NBLDataBufPos+size))
		{
			return NAND_SYSTEM_ERROR;
		}
		memcpy(NBLDataBuf+NBLDataBufPos,buf,size);
		NBLDataBufPos += size;
		return NAND_SUCCESS;
	}
	else
	{
		if(_S_TMPBUF_LEN > (s_tmpBufPos+size))
		{
			memcpy(s_tmpBuf+s_tmpBufPos,buf,size);
			s_tmpBufPos += size;
			return NAND_SUCCESS;
		}

		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
		{
			return NAND_SYSTEM_ERROR;
		}
		tmpBuf	= buf;
		tmpSize	= size;
		sctSize	= ftlPartitionInfo.sctSize;

		memcpy(s_tmpBuf+s_tmpBufPos,tmpBuf,_S_TMPBUF_LEN-s_tmpBufPos);
		tmpBuf	+= (_S_TMPBUF_LEN-s_tmpBufPos);
		tmpSize	-= (_S_TMPBUF_LEN-s_tmpBufPos);
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, s_sctId, _S_TMPBUF_LEN/sctSize, s_tmpBuf))
		{
			return NAND_SYSTEM_ERROR;
		}
		memset(s_tmpBuf,0xFF,_S_TMPBUF_LEN);
		s_sctId		+= _S_TMPBUF_LEN/sctSize;
		s_tmpBufPos	= 0;

		while(_S_TMPBUF_LEN <= tmpSize)
		{
			if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, s_sctId, _S_TMPBUF_LEN/sctSize, tmpBuf))
			{
				return NAND_SYSTEM_ERROR;
			}
			tmpBuf	+= _S_TMPBUF_LEN;
			tmpSize	-= _S_TMPBUF_LEN;
			s_sctId		+= _S_TMPBUF_LEN/sctSize;			
		}
		if(0 != tmpSize)
		{
			memcpy(s_tmpBuf,tmpBuf,tmpSize);
			s_tmpBufPos	= tmpSize;
		}
		return NAND_SUCCESS;
	}
}




int nand_end_write(void)
{
	uint32			sctSize,sctNum;
	SCI_FTL_PARTITION_INFO	ftlPartitionInfo;

	uint32			i,k;
	uint8	tmpSBuf[64];
	uint8	tmpPEcc[17];

	if(TRUE == g_IsNbl)
	{
		if(NBL_BUF_SIZE != NBLDataBufPos)
		{
			return NAND_SYSTEM_ERROR;
		}
		if(-1 != s_spec->safeDataPos)
		{
			NANDCTL_ONDIE_ECCOpen(FALSE);
		}
		_setHeadInfo();
		memset(tmpSBuf,0xFF,64);
		do
		{
			if(1 == s_spec->nSctPerPg)
			{
				// 1 write boot0;
				for(k = 0; k < _BOOT0_NUM; k++)
				{
					for(i = 0; i < ECC_BOOT0_SIZE/s_spec->nSctSize; i++)
					{
						if(ERR_NF_SUCCESS != NANDCTL_MS_Write_S ((k*ECC_BOOT0_SIZE) + (s_spec->nSctSize*i),NBLDataBuf+s_spec->nSctSize*i,tmpSBuf,1))
						{
							return NAND_SYSTEM_ERROR;
						}
					}
				}
				// 2 write boot1
				for(i = 0; i < (ECC_BOOT1_SIZE/s_spec->nSctSize); i++)
				{
					if(ERR_NF_SUCCESS != NANDCTL_MS_Write_S ((_BOOT0_NUM*ECC_BOOT0_SIZE) + (s_spec->nSctSize*i),ECC_BOOT0_SIZE+NBLDataBuf+s_spec->nSctSize*i,tmpSBuf,1))
					{
						return NAND_SYSTEM_ERROR;
					}
				}
				// 3 check data
				for(i = 0; i < ((_BOOT0_NUM*ECC_BOOT0_SIZE+ECC_BOOT1_SIZE)/s_spec->nSctSize); i++)
				{
					if(ERR_NF_SUCCESS != NANDCTL_MS_Read_S(s_spec->nSctSize*i, NBLMBuf, NBLSBuf, tmpPEcc))
					{
						break;
					}
					if(-1 == s_spec->safeDataPos)
					{
						if(
							(NBLSBuf[s_spec->mEccPos] != tmpPEcc[0])
							||(NBLSBuf[s_spec->mEccPos+1] != tmpPEcc[1])
							||(NBLSBuf[s_spec->mEccPos+2] != tmpPEcc[2])
						)
						{
							break;
						}
					}
					else
					{
						if(
							(NBLSBuf[6] != tmpPEcc[0])
							||(NBLSBuf[6+1] != tmpPEcc[1])
							||(NBLSBuf[6+2] != tmpPEcc[2])
						)
						{
							break;
						}
					}
				}
				if(((_BOOT0_NUM*ECC_BOOT0_SIZE+ECC_BOOT1_SIZE)/s_spec->nSctSize) != i)
				{
					for(k = 0; k < 8; k++)
					{
						if(ERR_NF_SUCCESS != NANDCTL_Erase_Block_S(k*s_spec->nSctSize*s_spec->nSctPerPg*s_spec->nPgPerBlk))
						{
							return NAND_SYSTEM_ERROR;
						}
					}
					return NAND_SYSTEM_ERROR;
				}
				break;
			}
			else if(4 == s_spec->nSctPerPg)
			{
				// 1 write boot0;
				for(k = 0; k < _BOOT0_NUM; k++)
				{
					for(i = 0; i < ECC_BOOT0_SIZE/(4*s_spec->nSctSize); i++)
					{
						if(ERR_NF_SUCCESS != NANDCTL_MS_Write_L ((k*ECC_BOOT0_SIZE)+(4*s_spec->nSctSize)*i,0,4,NBLDataBuf+(4*s_spec->nSctSize)*i,tmpSBuf,1))
						{
							return NAND_SYSTEM_ERROR;
						}
					}
				}
				// 2 write boot1
				for(i = 0; i < (ECC_BOOT1_SIZE/(4*s_spec->nSctSize)); i++)
				{
					if(ERR_NF_SUCCESS != NANDCTL_MS_Write_L ((_BOOT0_NUM*ECC_BOOT0_SIZE)+(4*s_spec->nSctSize)*i,0,4,ECC_BOOT0_SIZE+NBLDataBuf+(4*s_spec->nSctSize)*i,tmpSBuf,1))
					{
						return NAND_SYSTEM_ERROR;
					}
				}
				// 3 check data
				for(i = 0; i < ((_BOOT0_NUM*ECC_BOOT0_SIZE+ECC_BOOT1_SIZE)/(4*s_spec->nSctSize)); i++)
				{
					if(ERR_NF_SUCCESS != NANDCTL_MS_Read_L((4*s_spec->nSctSize)*i, 0, 4, NBLMBuf, NBLSBuf, tmpPEcc))
					{
						break;
					}
					if(-1 == s_spec->safeDataPos)
					{
						if(
							(NBLSBuf[s_spec->mEccPos] != tmpPEcc[0])
							||(NBLSBuf[s_spec->mEccPos+1] != tmpPEcc[1])
							||(NBLSBuf[s_spec->mEccPos+2] != tmpPEcc[2])

							||(NBLSBuf[s_spec->mEccPos+16] != tmpPEcc[4])
							||(NBLSBuf[s_spec->mEccPos+17] != tmpPEcc[5])
							||(NBLSBuf[s_spec->mEccPos+18] != tmpPEcc[6])

							||(NBLSBuf[s_spec->mEccPos+32] != tmpPEcc[8])
							||(NBLSBuf[s_spec->mEccPos+33] != tmpPEcc[9])
							||(NBLSBuf[s_spec->mEccPos+34] != tmpPEcc[10])

							||(NBLSBuf[s_spec->mEccPos+48] != tmpPEcc[12])
							||(NBLSBuf[s_spec->mEccPos+49] != tmpPEcc[13])
							||(NBLSBuf[s_spec->mEccPos+50] != tmpPEcc[14])
						)
						{
							break;
						}
					}
					else
					{
						if(
							(NBLSBuf[8] != tmpPEcc[0])
							||(NBLSBuf[8+1] != tmpPEcc[1])
							||(NBLSBuf[8+2] != tmpPEcc[2])

							||(NBLSBuf[8+16] != tmpPEcc[4])
							||(NBLSBuf[8+17] != tmpPEcc[5])
							||(NBLSBuf[8+18] != tmpPEcc[6])

							||(NBLSBuf[8+32] != tmpPEcc[8])
							||(NBLSBuf[8+33] != tmpPEcc[9])
							||(NBLSBuf[8+34] != tmpPEcc[10])

							||(NBLSBuf[8+48] != tmpPEcc[12])
							||(NBLSBuf[8+49] != tmpPEcc[13])
							||(NBLSBuf[8+50] != tmpPEcc[14])
						)
						{
							break;
						}
					}
				}
				if(((_BOOT0_NUM*ECC_BOOT0_SIZE+ECC_BOOT1_SIZE)/(4*s_spec->nSctSize)) != i)
				{
					if(ERR_NF_SUCCESS != NANDCTL_Erase_Block_L(0))
					{
						return NAND_SYSTEM_ERROR;
					}
					return NAND_SYSTEM_ERROR;
				}
				break;
			}
			else
			{
				// not support
				return NAND_SYSTEM_ERROR;
			}
		}while(1);

		if(-1 != s_spec->safeDataPos)
		{
			NANDCTL_ONDIE_ECCOpen(TRUE);
		}
		return NAND_SUCCESS;
	}
	else
	{
		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
		{
			return NAND_SYSTEM_ERROR;
		}
		sctSize	= ftlPartitionInfo.sctSize;

		if(SCI_FTL_MODULE_SCT == SCI_FTL_GET_MODULE(ftlPartitionInfo.attrib))
		{
			if(0 != s_tmpBufPos)
			{
				sctNum = (s_tmpBufPos/sctSize);
				if(0 != (s_tmpBufPos%sctSize))
				{
					sctNum++;
				}
				if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, s_sctId, sctNum, s_tmpBuf))
				{
					return NAND_SYSTEM_ERROR;
				}
			}
		}
		else
		{
			while((_S_TMPBUF_LEN/sctSize) <= (ftlPartitionInfo.sctTotalNum-s_sctId))
			{
				if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, s_sctId, (_S_TMPBUF_LEN/sctSize), s_tmpBuf))
				{
					return NAND_SYSTEM_ERROR;
				}
				s_sctId += (_S_TMPBUF_LEN/sctSize);
				memset(s_tmpBuf,0xFF,_S_TMPBUF_LEN);
			}
			if(0 != (ftlPartitionInfo.sctTotalNum-s_sctId))
			{
				if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, s_sctId, ftlPartitionInfo.sctTotalNum-s_sctId, s_tmpBuf))
				{
					return NAND_SYSTEM_ERROR;
				}
			}
		}
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Close(ftlHandle))
		{
			return NAND_SYSTEM_ERROR;
		}
	}
	return NAND_SUCCESS;
}

int nand_read (unsigned int addr, unsigned int ofst, unsigned int size,
               unsigned char *buf)
{
	uint32			nandId;
	uint32			partId;
	uint32			sctSize,sctId,sctNum;

//---
	SCI_FTL_PARTITION_INFO ftlPartitionInfo;
	uint32			rRet;
//---
	uint32			tmpSize;
	uint8*			tmpBuf;
//---
    /* cr224849 fei.zhang */
    unsigned int read_addr = 0;
    unsigned int read_size = 0;
    unsigned int rSct_Index = 0;
    unsigned int rScts = 0;

    /* check addr if physical nand access not through BBM function */
    read_addr = addr & READ_PHYNAND_MASK ;
    
    if (READ_PHYNAND_MASK == read_addr ) 
    {

        /* get real read addr */
        read_addr = addr & ~(READ_PHYNAND_MASK | READ_PHYNAND_SPR_MASK);
        read_size = size;


        if (0 != (read_addr % s_spec->nSctSize)) 
        {
            /* read address not align to 512Bytes */
            return NAND_INVALID_ADDR;
        }

        if ( (read_addr + read_size) > g_NandSize)
        {
            /* max read size limit to nand size */
            return NAND_INVALID_SIZE;
        }

        if (0 != (read_size % s_spec->nSctSize) )
        {
            /* read size must align to 512Bytes */
            return NAND_INVALID_SIZE;
        }

        /* initialize buffer to 0 */
        memset(buf, 0x0, read_size);
	
        /* read physical add from nand device */
        if (READ_PHYNAND_SPR_MASK == (addr & READ_PHYNAND_SPR_MASK))
        {
            /* read spare data */
            unsigned char *tmp_buf = buf;
            
            /* get read start sector index */
            rSct_Index = read_addr / s_spec->nSctSize;
            rScts      = read_size / s_spec->nSctSize;


            /* read spare data according to sector */

            while (rScts > 0) 
            {
	         if(1 == s_spec->nSctPerPg)
		  {
		      if(ERR_NF_SUCCESS != NANDCTL_S_Read_S (s_spec->nSctSize*rSct_Index, tmp_buf))
		      {
			    return NAND_SYSTEM_ERROR;
		       } 
                }
		  else
		  {
		      if(ERR_NF_SUCCESS != NANDCTL_S_Read_L ((rSct_Index >> 2) * s_spec->nSctSize*s_spec->nSctPerPg, rSct_Index & 0x03, 1,  tmp_buf))
		      {
			    return NAND_SYSTEM_ERROR;
		       }
                }
		  rSct_Index++;
                rScts--;
                tmp_buf += s_spec->nSctSize; 
            }            
            
        }
        else
        {
            /* read main data */

            /* get read start sector index */
            rSct_Index = read_addr / s_spec->nSctSize;
            rScts      = read_size / s_spec->nSctSize;
            while (rScts > 0) 
            {
	         if(1 == s_spec->nSctPerPg)
		  {
		      if(ERR_NF_SUCCESS != NANDCTL_M_Read_S (s_spec->nSctSize*rSct_Index, buf))
		      {
			    return NAND_SYSTEM_ERROR;
		       }		  
                }
		  else
		  {
		      if(ERR_NF_SUCCESS != NANDCTL_M_Read_L ((rSct_Index >> 2) * s_spec->nSctSize*s_spec->nSctPerPg, rSct_Index & 0x03, 1,  buf))
		      {
			    return NAND_SYSTEM_ERROR;
		       } 
                }
		  rSct_Index++;
                rScts--;
                buf += s_spec->nSctSize; 
            }
        }

        return NAND_SUCCESS;

    }
    else
    {
	nandId	= 0;
	if(0 == (ADDR_MASK&addr))
	{
		return NAND_SYSTEM_ERROR;
	}
	partId = ((~ADDR_MASK)&addr);

	if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, partId, &ftlHandle, 4))
	{
		return NAND_SYSTEM_ERROR;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
	{
		SCI_FTL_Close(ftlHandle);
		return NAND_SYSTEM_ERROR;
	}
	if(
			(ofst >= (ftlPartitionInfo.sctTotalNum*ftlPartitionInfo.sctSize))
			||((ofst+size) > (ftlPartitionInfo.sctTotalNum*ftlPartitionInfo.sctSize))
	)
	{
		SCI_FTL_Close(ftlHandle);
		return NAND_INVALID_SIZE;
	}
	sctSize = ftlPartitionInfo.sctSize;
	//---
	sctId	= ofst/sctSize;
	tmpBuf	= buf;
	tmpSize	= size;
	if(0 != (ofst%sctSize))
	{
		rRet	= SCI_FTL_Read(ftlHandle, sctId, 1, s_tmpBuf);
		if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK!= rRet))
		{
			SCI_FTL_Close(ftlHandle);
			return NAND_SYSTEM_ERROR;
		}
		if((sctSize-(ofst%sctSize)) > tmpSize)
		{
			memcpy(tmpBuf, s_tmpBuf+(ofst%sctSize),tmpSize);
			SCI_FTL_Close(ftlHandle);
			return NAND_SUCCESS;
		}
		else
		{
			memcpy(tmpBuf, s_tmpBuf+(ofst%sctSize),(sctSize-(ofst%sctSize)));
			tmpBuf += (sctSize-(ofst%sctSize));
			tmpSize -= (sctSize-(ofst%sctSize));
			sctId++;
		}
	}
	sctNum = tmpSize/sctSize;
	if(0 != sctNum)
	{
		rRet	= SCI_FTL_Read(ftlHandle, sctId, sctNum, tmpBuf);
		if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK!= rRet))
		{
			SCI_FTL_Close(ftlHandle);
			return NAND_SYSTEM_ERROR;
		}
		sctId	+= sctNum;
		tmpBuf	+= (sctSize*sctNum);
		tmpSize	-= (sctSize*sctNum);
	}
	if(0 != tmpSize)
	{
		rRet	= SCI_FTL_Read(ftlHandle, sctId, 1, s_tmpBuf);
		if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK!= rRet))
		{
			SCI_FTL_Close(ftlHandle);
			return NAND_SYSTEM_ERROR;
		}
		memcpy(tmpBuf,s_tmpBuf,tmpSize);
	}
	//---
	SCI_FTL_Close(ftlHandle);
	return NAND_SUCCESS;

	}
	/* cr224849 fei.zhang */
}

// skip size param. Jason.Wu
int nand_erase (unsigned int addr, unsigned int size)
{
	uint32			nandId;
	uint32			partId;

	SCI_FTL_PARTITION_INFO ftlPartitionInfo;
	nandId = 0;
	if(0 == (ADDR_MASK&addr))
	{
		return NAND_INVALID_ADDR;
	}
	partId = ((~ADDR_MASK)&addr);

	if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, partId, &ftlHandle, 4))
	{
		return NAND_SYSTEM_ERROR;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
	{
		SCI_FTL_Close(ftlHandle);
		return NAND_SYSTEM_ERROR;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_Erase(ftlHandle, 0, ftlPartitionInfo.sctTotalNum))
	{
		SCI_FTL_Close(ftlHandle);
		return NAND_SYSTEM_ERROR;
	}
	SCI_FTL_Close(ftlHandle);
	return NAND_SUCCESS;
}
