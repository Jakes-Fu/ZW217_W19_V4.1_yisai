/*===================================================================*/
/*                                                                   */
/*  InfoNES_Mapper.cpp : InfoNES Mapper Function                     */
/*                                                                   */
/*  2000/05/16  InfoNES Project ( based on NesterJ and pNesX )       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_Mapper.h"
#include "K6502.h"
#include "InfoNES_fpic.h"

/*-------------------------------------------------------------------*/
/*  Mapper resources                                                 */
/*-------------------------------------------------------------------*/

/* Disk System RAM */
BYTE *DRAM = NULL;//[ DRAM_SIZE ];
BYTE *Map5_Ex_Ram = NULL;//[ 0x400 ]; 
BYTE *Map5_Ex_Vram = NULL;//[ 0x400 ];
BYTE *Map5_Ex_Nam = NULL;//[ 0x400 ];
BYTE *Map5_Wram = NULL;//[ 0x2000 * 8 ];

BYTE *Map6_Chr_Ram =NULL;//[ 0x2000 * 4 ];
BYTE  *Map19_Chr_Ram =NULL;//[ 0x2000 ];
BYTE *Map185_Dummy_Chr_Rom =NULL;//[ 0x400 ];
BYTE *Map188_Dummy =NULL;//[ 0x2000 ];
BYTE *Map85_Chr_Ram =NULL;//[ 0x100 * 0x400 ];

/*-------------------------------------------------------------------*/
/*  Table of Mapper initialize function                              */
/*-------------------------------------------------------------------*/

//struct MapperTable_tag MapperTable[140] ;
struct MapperTable_tag *MapperTable = NULL;

int InfoNES_Mapper_fpic_init( void )
{
	int idx = 0;
#define XInfoNES_FPIC_init( n , p )				\
	MapperTable[idx].nMapperNo   = (n);		\
	MapperTable[idx].pMapperInit = (p);		\
	++idx;
	
	DRAM = (BYTE *)NesMallocFromWRE(DRAM_SIZE * sizeof (BYTE));
	if (NULL == DRAM)
		return -1;
	MapperTable = (struct MapperTable_tag *)NesMallocFromWRE(140 * sizeof (struct MapperTable_tag));
	if (NULL == MapperTable)
		return -1;
	
	XInfoNES_FPIC_init(   0, Map0_Init   );
	XInfoNES_FPIC_init(   1, Map1_Init   );
	XInfoNES_FPIC_init(   2, Map2_Init   );
	XInfoNES_FPIC_init(   3, Map3_Init   );
	XInfoNES_FPIC_init(   4, Map4_Init   );
	XInfoNES_FPIC_init(   5, Map5_Init   );
	XInfoNES_FPIC_init(   6, Map6_Init   );
	XInfoNES_FPIC_init(   7, Map7_Init   );
	XInfoNES_FPIC_init(   8, Map8_Init   );
	XInfoNES_FPIC_init(   9, Map9_Init   );
	XInfoNES_FPIC_init(  10, Map10_Init  );
	XInfoNES_FPIC_init(  11, Map11_Init  );
	XInfoNES_FPIC_init(  13, Map13_Init  );
	XInfoNES_FPIC_init(  15, Map15_Init  );
	XInfoNES_FPIC_init(  16, Map16_Init  );
	XInfoNES_FPIC_init(  17, Map17_Init  );
	XInfoNES_FPIC_init(  18, Map18_Init  );
	XInfoNES_FPIC_init(  19, Map19_Init  );
	XInfoNES_FPIC_init(  21, Map21_Init  );
	XInfoNES_FPIC_init(  22, Map22_Init  );
	XInfoNES_FPIC_init(  23, Map23_Init  );
	XInfoNES_FPIC_init(  24, Map24_Init  );
	XInfoNES_FPIC_init(  25, Map25_Init  );
	XInfoNES_FPIC_init(  26, Map26_Init  );
	XInfoNES_FPIC_init(  32, Map32_Init  );
	XInfoNES_FPIC_init(  33, Map33_Init  );
	XInfoNES_FPIC_init(  34, Map34_Init  );
	XInfoNES_FPIC_init(  40, Map40_Init  );
	XInfoNES_FPIC_init(  41, Map41_Init  );
	XInfoNES_FPIC_init(  42, Map42_Init  );
	XInfoNES_FPIC_init(  43, Map43_Init  );
	XInfoNES_FPIC_init(  44, Map44_Init  );
	XInfoNES_FPIC_init(  45, Map45_Init  );
	XInfoNES_FPIC_init(  46, Map46_Init  );
	XInfoNES_FPIC_init(  47, Map47_Init  );
	XInfoNES_FPIC_init(  48, Map48_Init  );
	XInfoNES_FPIC_init(  49, Map49_Init  );
	XInfoNES_FPIC_init(  50, Map50_Init  );
	XInfoNES_FPIC_init(  51, Map51_Init  );
	XInfoNES_FPIC_init(  57, Map57_Init  );
	XInfoNES_FPIC_init(  58, Map58_Init  );
	XInfoNES_FPIC_init(  60, Map60_Init  );
	XInfoNES_FPIC_init(  61, Map61_Init  );
	XInfoNES_FPIC_init(  62, Map62_Init  );
	XInfoNES_FPIC_init(  64, Map64_Init  );
	XInfoNES_FPIC_init(  65, Map65_Init  );
	XInfoNES_FPIC_init(  66, Map66_Init  );
	XInfoNES_FPIC_init(  67, Map67_Init  );
	XInfoNES_FPIC_init(  68, Map68_Init  );
	XInfoNES_FPIC_init(  69, Map69_Init  );
	XInfoNES_FPIC_init(  70, Map70_Init  );
	XInfoNES_FPIC_init(  71, Map71_Init  );
	XInfoNES_FPIC_init(  72, Map72_Init  );
	XInfoNES_FPIC_init(  73, Map73_Init  );
	XInfoNES_FPIC_init(  74, Map74_Init  );
	XInfoNES_FPIC_init(  75, Map75_Init  );
	XInfoNES_FPIC_init(  76, Map76_Init  );
	XInfoNES_FPIC_init(  77, Map77_Init  ); 
	XInfoNES_FPIC_init(  78, Map78_Init  ); 
	XInfoNES_FPIC_init(  79, Map79_Init  ); 
	XInfoNES_FPIC_init(  80, Map80_Init  ); 
	XInfoNES_FPIC_init(  82, Map82_Init  ); 
	XInfoNES_FPIC_init(  83, Map83_Init  );
	XInfoNES_FPIC_init(  85, Map85_Init  );
	XInfoNES_FPIC_init(  86, Map86_Init  );
	XInfoNES_FPIC_init(  87, Map87_Init  );
	XInfoNES_FPIC_init(  88, Map88_Init  );
	XInfoNES_FPIC_init(  89, Map89_Init  );
	XInfoNES_FPIC_init(  90, Map90_Init  );
	XInfoNES_FPIC_init(  91, Map91_Init  );
	XInfoNES_FPIC_init(  92, Map92_Init  );
	XInfoNES_FPIC_init(  93, Map93_Init  );
	XInfoNES_FPIC_init(  94, Map94_Init  );
	XInfoNES_FPIC_init(  95, Map95_Init  );
	XInfoNES_FPIC_init(  96, Map96_Init  );
	XInfoNES_FPIC_init(  97, Map97_Init  );
	XInfoNES_FPIC_init(  99, Map99_Init  );
	XInfoNES_FPIC_init( 100, Map100_Init );
	XInfoNES_FPIC_init( 101, Map101_Init );
	XInfoNES_FPIC_init( 105, Map105_Init );
	XInfoNES_FPIC_init( 107, Map107_Init );
	XInfoNES_FPIC_init( 108, Map108_Init );
	XInfoNES_FPIC_init( 109, Map109_Init );
	XInfoNES_FPIC_init( 110, Map110_Init );
	XInfoNES_FPIC_init( 112, Map112_Init );
	XInfoNES_FPIC_init( 113, Map113_Init );
	XInfoNES_FPIC_init( 114, Map114_Init );
	XInfoNES_FPIC_init( 115, Map115_Init );
	XInfoNES_FPIC_init( 116, Map116_Init );
	XInfoNES_FPIC_init( 117, Map117_Init );
	XInfoNES_FPIC_init( 118, Map118_Init );
	XInfoNES_FPIC_init( 119, Map119_Init );
	XInfoNES_FPIC_init( 122, Map122_Init );
	XInfoNES_FPIC_init( 133, Map133_Init );
	XInfoNES_FPIC_init( 134, Map134_Init );
	XInfoNES_FPIC_init( 135, Map135_Init );
	XInfoNES_FPIC_init( 140, Map140_Init );
	XInfoNES_FPIC_init( 151, Map151_Init ); 
	XInfoNES_FPIC_init( 160, Map160_Init ); 
	XInfoNES_FPIC_init( 180, Map180_Init ); 
	XInfoNES_FPIC_init( 181, Map181_Init ); 
	XInfoNES_FPIC_init( 182, Map182_Init ); 
	XInfoNES_FPIC_init( 183, Map183_Init ); 
	XInfoNES_FPIC_init( 185, Map185_Init ); 
	XInfoNES_FPIC_init( 187, Map187_Init ); 
	XInfoNES_FPIC_init( 188, Map188_Init ); 
	XInfoNES_FPIC_init( 189, Map189_Init ); 
	XInfoNES_FPIC_init( 191, Map191_Init ); 
	XInfoNES_FPIC_init( 193, Map193_Init ); 
	XInfoNES_FPIC_init( 194, Map194_Init ); 
	XInfoNES_FPIC_init( 200, Map200_Init );
	XInfoNES_FPIC_init( 201, Map201_Init );
	XInfoNES_FPIC_init( 202, Map202_Init );
	XInfoNES_FPIC_init( 222, Map222_Init );
	XInfoNES_FPIC_init( 225, Map225_Init );
	XInfoNES_FPIC_init( 226, Map226_Init );
	XInfoNES_FPIC_init( 227, Map227_Init );
	XInfoNES_FPIC_init( 228, Map228_Init );
	XInfoNES_FPIC_init( 229, Map229_Init );
	XInfoNES_FPIC_init( 230, Map230_Init );
	XInfoNES_FPIC_init( 231, Map231_Init );
	XInfoNES_FPIC_init( 232, Map232_Init );
	XInfoNES_FPIC_init( 233, Map233_Init );
	XInfoNES_FPIC_init( 234, Map234_Init );
	XInfoNES_FPIC_init( 235, Map235_Init );
	XInfoNES_FPIC_init( 236, Map236_Init );
	XInfoNES_FPIC_init( 240, Map240_Init );
	XInfoNES_FPIC_init( 241, Map241_Init );
	XInfoNES_FPIC_init( 242, Map242_Init );
	XInfoNES_FPIC_init( 243, Map243_Init );
	XInfoNES_FPIC_init( 244, Map244_Init );
	XInfoNES_FPIC_init( 245, Map245_Init );
	XInfoNES_FPIC_init( 246, Map246_Init );
	XInfoNES_FPIC_init( 248, Map248_Init );
	XInfoNES_FPIC_init( 249, Map249_Init );
	XInfoNES_FPIC_init( 251, Map251_Init );
	XInfoNES_FPIC_init( 252, Map252_Init );
	XInfoNES_FPIC_init( 255, Map255_Init );
	XInfoNES_FPIC_init( -1, NULL );
	return 0;
}

void InfoNES_Mapper_fpic_finalize(void)
{
	NES_FREE(DRAM);
	NES_FREE(MapperTable);

	NES_FREE(Map5_Wram);
	NES_FREE(Map5_Ex_Ram);
	NES_FREE(Map5_Ex_Vram);
	NES_FREE(Map5_Ex_Nam);

	NES_FREE(Map6_Chr_Ram);
	NES_FREE(Map19_Chr_Ram);
	NES_FREE(Map185_Dummy_Chr_Rom);
	NES_FREE(Map188_Dummy);
	NES_FREE(Map85_Chr_Ram);
}


/*-------------------------------------------------------------------*/
/*  body of Mapper functions                                         */
/*-------------------------------------------------------------------*/

#include "mapper/InfoNES_Mapper_000.c"
#include "mapper/InfoNES_Mapper_001.c"
#include "mapper/InfoNES_Mapper_002.c"
#include "mapper/InfoNES_Mapper_003.c"
#include "mapper/InfoNES_Mapper_004.c"
#include "mapper/InfoNES_Mapper_005.c"
#include "mapper/InfoNES_Mapper_006.c"
#include "mapper/InfoNES_Mapper_007.c"
#include "mapper/InfoNES_Mapper_008.c"
#include "mapper/InfoNES_Mapper_009.c"
#include "mapper/InfoNES_Mapper_010.c"
#include "mapper/InfoNES_Mapper_011.c"
#include "mapper/InfoNES_Mapper_013.c"
#include "mapper/InfoNES_Mapper_015.c"
#include "mapper/InfoNES_Mapper_016.c"
#include "mapper/InfoNES_Mapper_017.c"
#include "mapper/InfoNES_Mapper_018.c"
#include "mapper/InfoNES_Mapper_019.c"
#include "mapper/InfoNES_Mapper_021.c"          
#include "mapper/InfoNES_Mapper_022.c"  
#include "mapper/InfoNES_Mapper_023.c"  
#include "mapper/InfoNES_Mapper_024.c"  
#include "mapper/InfoNES_Mapper_025.c"  
#include "mapper/InfoNES_Mapper_026.c"  
#include "mapper/InfoNES_Mapper_032.c"  
#include "mapper/InfoNES_Mapper_033.c" 
#include "mapper/InfoNES_Mapper_034.c" 
#include "mapper/InfoNES_Mapper_040.c"
#include "mapper/InfoNES_Mapper_041.c"
#include "mapper/InfoNES_Mapper_042.c"
#include "mapper/InfoNES_Mapper_043.c"
#include "mapper/InfoNES_Mapper_044.c"
#include "mapper/InfoNES_Mapper_045.c"
#include "mapper/InfoNES_Mapper_046.c"
#include "mapper/InfoNES_Mapper_047.c"
#include "mapper/InfoNES_Mapper_048.c"
#include "mapper/InfoNES_Mapper_049.c"
#include "mapper/InfoNES_Mapper_050.c"
#include "mapper/InfoNES_Mapper_051.c"
#include "mapper/InfoNES_Mapper_057.c"
#include "mapper/InfoNES_Mapper_058.c"
#include "mapper/InfoNES_Mapper_060.c"
#include "mapper/InfoNES_Mapper_061.c"
#include "mapper/InfoNES_Mapper_062.c"
#include "mapper/InfoNES_Mapper_064.c"
#include "mapper/InfoNES_Mapper_065.c"
#include "mapper/InfoNES_Mapper_066.c"
#include "mapper/InfoNES_Mapper_067.c"
#include "mapper/InfoNES_Mapper_068.c"
#include "mapper/InfoNES_Mapper_069.c"
#include "mapper/InfoNES_Mapper_070.c"
#include "mapper/InfoNES_Mapper_071.c"
#include "mapper/InfoNES_Mapper_072.c"
#include "mapper/InfoNES_Mapper_073.c"
#include "mapper/InfoNES_Mapper_074.c"
#include "mapper/InfoNES_Mapper_075.c"
#include "mapper/InfoNES_Mapper_076.c"
#include "mapper/InfoNES_Mapper_077.c"
#include "mapper/InfoNES_Mapper_078.c"
#include "mapper/InfoNES_Mapper_079.c"
#include "mapper/InfoNES_Mapper_080.c"
#include "mapper/InfoNES_Mapper_082.c"
#include "mapper/InfoNES_Mapper_083.c"
#include "mapper/InfoNES_Mapper_085.c"
#include "mapper/InfoNES_Mapper_086.c"
#include "mapper/InfoNES_Mapper_087.c"
#include "mapper/InfoNES_Mapper_088.c"
#include "mapper/InfoNES_Mapper_089.c"
#include "mapper/InfoNES_Mapper_090.c"
#include "mapper/InfoNES_Mapper_091.c"
#include "mapper/InfoNES_Mapper_092.c"
#include "mapper/InfoNES_Mapper_093.c"
#include "mapper/InfoNES_Mapper_094.c"
#include "mapper/InfoNES_Mapper_095.c"
#include "mapper/InfoNES_Mapper_096.c"
#include "mapper/InfoNES_Mapper_097.c"
#include "mapper/InfoNES_Mapper_099.c"
#include "mapper/InfoNES_Mapper_100.c"
#include "mapper/InfoNES_Mapper_101.c"
#include "mapper/InfoNES_Mapper_105.c"
#include "mapper/InfoNES_Mapper_107.c"
#include "mapper/InfoNES_Mapper_108.c"
#include "mapper/InfoNES_Mapper_109.c"
#include "mapper/InfoNES_Mapper_110.c"
#include "mapper/InfoNES_Mapper_112.c"
#include "mapper/InfoNES_Mapper_113.c"
#include "mapper/InfoNES_Mapper_114.c"
#include "mapper/InfoNES_Mapper_115.c"
#include "mapper/InfoNES_Mapper_116.c"
#include "mapper/InfoNES_Mapper_117.c"
#include "mapper/InfoNES_Mapper_118.c"
#include "mapper/InfoNES_Mapper_119.c"
#include "mapper/InfoNES_Mapper_122.c"
#include "mapper/InfoNES_Mapper_133.c"
#include "mapper/InfoNES_Mapper_134.c"
#include "mapper/InfoNES_Mapper_135.c"
#include "mapper/InfoNES_Mapper_140.c"
#include "mapper/InfoNES_Mapper_151.c"
#include "mapper/InfoNES_Mapper_160.c"
#include "mapper/InfoNES_Mapper_180.c"
#include "mapper/InfoNES_Mapper_181.c"
#include "mapper/InfoNES_Mapper_182.c"
#include "mapper/InfoNES_Mapper_183.c"
#include "mapper/InfoNES_Mapper_185.c"
#include "mapper/InfoNES_Mapper_187.c"
#include "mapper/InfoNES_Mapper_188.c"
#include "mapper/InfoNES_Mapper_189.c"
#include "mapper/InfoNES_Mapper_191.c"
#include "mapper/InfoNES_Mapper_193.c"
#include "mapper/InfoNES_Mapper_194.c"
#include "mapper/InfoNES_Mapper_200.c"
#include "mapper/InfoNES_Mapper_201.c"
#include "mapper/InfoNES_Mapper_202.c"
#include "mapper/InfoNES_Mapper_222.c"
#include "mapper/InfoNES_Mapper_225.c"
#include "mapper/InfoNES_Mapper_226.c"
#include "mapper/InfoNES_Mapper_227.c"
#include "mapper/InfoNES_Mapper_228.c"
#include "mapper/InfoNES_Mapper_229.c"
#include "mapper/InfoNES_Mapper_230.c"
#include "mapper/InfoNES_Mapper_231.c"
#include "mapper/InfoNES_Mapper_232.c"
#include "mapper/InfoNES_Mapper_233.c"
#include "mapper/InfoNES_Mapper_234.c"
#include "mapper/InfoNES_Mapper_235.c"
#include "mapper/InfoNES_Mapper_236.c"
#include "mapper/InfoNES_Mapper_240.c"
#include "mapper/InfoNES_Mapper_241.c"
#include "mapper/InfoNES_Mapper_242.c"
#include "mapper/InfoNES_Mapper_243.c"
#include "mapper/InfoNES_Mapper_244.c"
#include "mapper/InfoNES_Mapper_245.c"
#include "mapper/InfoNES_Mapper_246.c"
#include "mapper/InfoNES_Mapper_248.c"
#include "mapper/InfoNES_Mapper_249.c"
#include "mapper/InfoNES_Mapper_251.c"
#include "mapper/InfoNES_Mapper_252.c"
#include "mapper/InfoNES_Mapper_255.c"

/* End of InfoNES_Mapper.cpp */


