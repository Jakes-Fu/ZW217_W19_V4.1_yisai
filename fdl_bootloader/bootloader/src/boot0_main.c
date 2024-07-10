
#include "sci_types.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "nlc_drv.h"
#include "nand_spec.h"
#include "chng_freq.h"
#include "chip.h"
#include "sci_ftl_api.h"

#define BL_TRACE_ENABLED
#include "bl_trace.h"

//#include "bl_mmu.h"


extern void set_pc (int value);

extern uint32 g_ahb_clk;  //defined in entry.s


#define ECC_BOOT1_SIZE 0x14000

#define BOOT1_MEM_LOAD_ADDR		(16*1024*1024)	// 32MÄ©Î²128kµÄ¿Õ¼äÎ»ÖÃ
#define BOOT1_NAND_LOAD_ADDR		(48*1024)

static uint8	_tmpMBuf[2048];
static uint8	_tmpSBuf[64];
static uint8	_tmpPEcc[17];
PUBLIC int main(void)
{
#if 1
	NAND_SPEC* spec;
	unsigned __int64	ID = 0;
	uint8	idArr[4] = {0,0,0,0};

	NAND_PARA_T		nandPara;
	NAND_TIMING_T	nandTiming;

	uint8	*memAddr = (uint8*)(BOOT1_MEM_LOAD_ADDR);
	uint32	i,memPos;


	NANDCTL_Init (g_ahb_clk);
	NANDCTL_Read_ID(idArr);
	ID = (((unsigned __int64)idArr[0])<<8)|((unsigned __int64)idArr[1]);

	spec = GetNandParam(ID);
	if(0 == spec)
	{
		return 0;
	}

	nandTiming.nTeTime	= spec->nTeTime;
	nandTiming.nTpTime	= spec->nTwTime;
	nandTiming.nTrTime	= spec->nTrTime;
	nandTiming.nTwcTime	= spec->nTfTime;
	NANDCTL_InitParam(&nandTiming);
	nandPara.acycle = spec->nCycleDev;
	if(1 == spec->nSctPerPg)
	{
		nandPara.pagesize	= 512;
		nandPara.sparesize	= 16;
	}
	else
	{
		nandPara.pagesize	= 2048;
		nandPara.sparesize	= 64;
	}
	nandPara.buswidth	= spec->nBWidth;
	nandPara.advance	= spec->nAdvance;
	nandPara.eccpos		= spec->mEccPos;
	NANDCTL_Open(&nandPara);

	if(-1 != spec->safeDataPos)
	{
		NANDCTL_ONDIE_ECCOpen(FALSE);
	}
	memPos = 0;
	if(1 == spec->nSctPerPg)
	{
		for(i = 0; i < (ECC_BOOT1_SIZE/spec->nSctSize); i++)
		{
			if(ERR_NF_SUCCESS != NANDCTL_MS_Read_S(BOOT1_NAND_LOAD_ADDR+spec->nSctSize*i, memAddr+memPos, _tmpSBuf, _tmpPEcc))
			{
				break;
			}
			if(-1 == spec->safeDataPos)
			{
				if(
					(_tmpSBuf[spec->mEccPos] != _tmpPEcc[0])
					||(_tmpSBuf[spec->mEccPos+1] != _tmpPEcc[1])
					||(_tmpSBuf[spec->mEccPos+2] != _tmpPEcc[2])
					)
				{
					break;
				}
			}
			else
			{
				if(
					(_tmpSBuf[6] != _tmpPEcc[0])
					||(_tmpSBuf[6+1] != _tmpPEcc[1])
					||(_tmpSBuf[6+2] != _tmpPEcc[2])
					)
				{
					break;
				}
			}
			memPos += spec->nSctSize;
		}
	}
	else
	{
		for(i = 0; i < (ECC_BOOT1_SIZE/(4*spec->nSctSize)); i++)
		{
			if(ERR_NF_SUCCESS != NANDCTL_MS_Read_L(BOOT1_NAND_LOAD_ADDR+(4*spec->nSctSize)*i, 0, 4, memAddr+memPos, _tmpSBuf, _tmpPEcc))
			{
				break;
			}
			if(-1 == spec->safeDataPos)
			{
				if(
					(_tmpSBuf[spec->mEccPos] != _tmpPEcc[0])
					||(_tmpSBuf[spec->mEccPos+1] != _tmpPEcc[1])
					||(_tmpSBuf[spec->mEccPos+2] != _tmpPEcc[2])

					||(_tmpSBuf[spec->mEccPos+16] != _tmpPEcc[4])
					||(_tmpSBuf[spec->mEccPos+17] != _tmpPEcc[5])
					||(_tmpSBuf[spec->mEccPos+18] != _tmpPEcc[6])

					||(_tmpSBuf[spec->mEccPos+32] != _tmpPEcc[8])
					||(_tmpSBuf[spec->mEccPos+33] != _tmpPEcc[9])
					||(_tmpSBuf[spec->mEccPos+34] != _tmpPEcc[10])

					||(_tmpSBuf[spec->mEccPos+48] != _tmpPEcc[12])
					||(_tmpSBuf[spec->mEccPos+49] != _tmpPEcc[13])
					||(_tmpSBuf[spec->mEccPos+50] != _tmpPEcc[14])
				)
				{
					break;
				}
			}
			else
			{
				if(
					(_tmpSBuf[8] != _tmpPEcc[0])
					||(_tmpSBuf[8+1] != _tmpPEcc[1])
					||(_tmpSBuf[8+2] != _tmpPEcc[2])

					||(_tmpSBuf[8+16] != _tmpPEcc[4])
					||(_tmpSBuf[8+17] != _tmpPEcc[5])
					||(_tmpSBuf[8+18] != _tmpPEcc[6])

					||(_tmpSBuf[8+32] != _tmpPEcc[8])
					||(_tmpSBuf[8+33] != _tmpPEcc[9])
					||(_tmpSBuf[8+34] != _tmpPEcc[10])

					||(_tmpSBuf[8+48] != _tmpPEcc[12])
					||(_tmpSBuf[8+49] != _tmpPEcc[13])
					||(_tmpSBuf[8+50] != _tmpPEcc[14])
				)
				{
					break;
				}
			}
			memPos += (4*spec->nSctSize);
		}
	}
	if(-1 != spec->safeDataPos)
	{
		NANDCTL_ONDIE_ECCOpen(TRUE);
	}
	set_pc(memAddr);
#endif
	for(;;){}
	// should add power off here. Jason.Wu
	return 0;	/*lint !e527*/
}


