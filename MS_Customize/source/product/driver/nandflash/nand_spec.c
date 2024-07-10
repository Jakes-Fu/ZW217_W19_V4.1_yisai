
#include "sci_types.h"
#include "nand_spec.h"


NAND_SPEC nandSpec[] =
{
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ID			nBlkNum
						nNumOfPlane
							nBlkNumInRsv
								nSctSize
									nSpareSize
										nSctPerPg
											nPgPerBlk
												nWCntPerPg
													badPgId
														badSctId
															badPos
																badLen
																	safeDataPos
																		safeDataLen
																			mEccPos
																				mEccBitLen
																					mEccRiskBitNum
																						sEccPos
																							sEccBitLen
																								sEccRiskBitNum
																									nBWidth	nCycleDev		nAdvance		nTrTime	nTwTime	nTeTime	nTfTime
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    { 0xEC75,	2048,	1,	38,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_3CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xEC55,	2048,	1,	38,	512,16,	1,	32,	1,	0,	0,	11,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X16,	BVD_3CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xEC56,	4096,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X16,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xEC76,	4096,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xECBA,	2048,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20091215 for samsung 16bit largepage nand
    { 0xECDA,	2048,	1,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xEC79,	8192,	4,	143,512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xEC73,	1024,	1,	23,	512,16,	1,	64,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_3CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xECCA,	2048,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xECF1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xECC1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xECAA,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xEC00,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xECA1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},//K511F13ACA-A075 
    { 0xECB1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},

    { 0xAD76,	4096,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xADBA,	2048,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20091215 for hynix
    { 0xADBC,	4096,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16, BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20091215 for hynix
    { 0xADAA,	2048,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08, BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20101116 h8bcso0rj
    { 0xAD36,	4096,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xAD78,	8192,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},
    { 0xADA1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},//Hynix(1G+512SRD)
    { 0xAD79,	8192,	4,	73,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},//Hynix H8ACU0EG0ABR(1G+512SRD)
    { 0xADB1,	1024,	1,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20091215
    { 0xADBA,	2048,	2,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@20091215
    { 0xAD35,	2048,	1,	23,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_3CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},//add Hynix H9LA25G25HAMBR	

    { 0x2075,	2048,	1,	43,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_3CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},//ST NAND256W3A
    { 0x20A1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},//Numonyx NANDA9R3N1C(1G+512SDR)
    { 0x2CA1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	90},//Micron (1G+512MSDR)
    { 0x2076,	4096,	1,	83,	512,16,	1,	32,	1,	0,	0,	5,	1,	-1,	0,	6,	1,	1,	9,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_0,	20,	300,	2000,	80},	//Numonyx NAND98W3M0

    { 0xAA00,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0xC000,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},

    { 0x98A1,	1024,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80},
    { 0x98A0,	512,	1,	23,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //toshiba 8bit largepage
    { 0x98AA,	2048,	1,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //toshiba 8bit largepage
    { 0x98BA,	2048,	1,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@cr167676 for toshiba 16bit largepage
    { 0x98B1,	1024,	1,	43,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_4CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@cr167676 for toshiba 16bit largepage
    { 0x98BC,	4096,	1,	83,	512,16,	4,	64,	4,	0,	0,	0,	1,	-1,	0,	8,	1,	1,	11,	1,	1,	BW_X16,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	80}, //add@cr167676 for toshiba 16bit largepage
//  4bit onchip ecc nand
    { 0x2CAC,	4096,	2,	87,	512,16,	4,	64,	4,	0,	0,	0,	2,	4,	4,	8,	64,	4,	-1,	-1,	-1,	BW_X08, BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	90},//Micron (4G+2GDDR,16Bit),support ondie ecc
    { 0x2CBC,	4096,	2,	87,	512,16,	4,	64,	4,	0,	0,	0,	2,	4,	4,	8,	64,	4,	-1,	-1,	-1,	BW_X16, BVD_5CYCLES,	BVD_ADVANCE_1,	20,	300,	2000,	90},//Micron (4G+2GDDR,08Bit),support ondie ecc
//  MLC
    { 0xADD5,	1024,	2,	30,	1024,56,	8,	256,	1,	0,	0,	0,	1,	-1,	0,	1,	24,	16,	43,	24,	16,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	30,	20,		50}, //hynix	H27UAG8T2B		ID:0xADD5	added by jason.wu
    { 0x98D7,	4100,	1,	170,	1024,56,	8,	128,	1,	0,	0,	0,	1,	-1,	0,	1,	24,	16,	43,	24,	16,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	30,	20,		50}, //toshiba	TH58NVG6D2FTA20		ID:0x98D7	added by jason.wu
    { 0x98D5,	2084,	1,	120,	1024,47,	8,	128,	1,	0,	0,	0,	1,	-1,	0,	1,	24,	16,	43,	24,	16,	BW_X08,	BVD_5CYCLES,	BVD_ADVANCE_1,	20,	30,	20,		50}, //toshiba	TC58NVG4D2ETA00		ID:0x98D5	added by jason.wu
    
    { 0x0000,	0,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	0,	0,	0,	0,	0,	0,	0,	0,		0,				0,				0,	0,	0,		0}
};

NAND_SPEC* GetNandParam(unsigned __int64 ID)
{
	uint32 i;

	i = 0;
	while(0 != nandSpec[i].ID)
	{
		if(ID == nandSpec[i].ID)
		{
			return &nandSpec[i];
		}
		i++;
	}
	return (NAND_SPEC*)0;
}

