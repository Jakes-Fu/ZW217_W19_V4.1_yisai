/*===================================================================*/
/*                                                                   */
/*  InfoNES_Mapper.h : InfoNES Mapper Function                       */
/*                                                                   */
/*  2000/05/16  InfoNES Project ( based on NesterJ and pNesX )       */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_MAPPER_H_INCLUDED
#define InfoNES_MAPPER_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include "InfoNES_Types.h"

/*-------------------------------------------------------------------*/
/*  Constants                                                        */
/*-------------------------------------------------------------------*/

#define DRAM_SIZE    0xA000

/*-------------------------------------------------------------------*/
/*  Mapper resources                                                 */
/*-------------------------------------------------------------------*/

/* Disk System RAM */
//extern BYTE DRAM[];
extern BYTE *DRAM;
/*-------------------------------------------------------------------*/
/*  Macros                                                           */
/*-------------------------------------------------------------------*/

/* The address of 8Kbytes unit of the ROM */
#define ROMPAGE(a)     &pGData->ROM[ (a) * 0x2000 ]
/* From behind the ROM, the address of 8kbytes unit */
#define ROMLASTPAGE(a) &pGData->ROM[ NesHeader.byRomSize * 0x4000 - ( (a) + 1 ) * 0x2000 ]
/* The address of 1Kbytes unit of the VROM */
#define VROMPAGE(a)    &pGData->VROM[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the CRAM */
#define CRAMPAGE(a)   &pGData->PPURAM[ 0x0000 + ((a)&0x1F) * 0x400 ]
/* The address of 1Kbytes unit of the VRAM */
#define VRAMPAGE(a)    &pGData->PPURAM[ 0x2000 + (a) * 0x400 ]
/* Translate the pointer to ChrBuf into the address of Pattern Table */ 
#define PATTBL(a)      ( ( (a) - ChrBuf ) >> 2 )

/*-------------------------------------------------------------------*/
/*  Macros ( Mapper specific )                                       */
/*-------------------------------------------------------------------*/

/* The address of 8Kbytes unit of the Map5 ROM */
#define Map5_ROMPAGE(a)     &Map5_Wram[ ( (a) & 0x07 ) * 0x2000 ]
/* The address of 1Kbytes unit of the Map6 Chr RAM */
#define Map6_VROMPAGE(a)    &Map6_Chr_Ram[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the Map19 Chr RAM */
#define Map19_VROMPAGE(a)   &Map19_Chr_Ram[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the Map85 Chr RAM */
#define Map85_VROMPAGE(a)   &Map85_Chr_Ram[ (a) * 0x400 ]

/*-------------------------------------------------------------------*/
/*  Table of Mapper initialize function                              */
/*-------------------------------------------------------------------*/

struct MapperTable_tag
{
  int nMapperNo;
  void (*pMapperInit)(	Nes_Global_T *pGData);
};

extern struct MapperTable_tag *MapperTable;

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

void Map0_Init(Nes_Global_T *pGData);
void Map0_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map0_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map0_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map0_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map0_VSync(Nes_Global_T *pGData);
void Map0_HSync(Nes_Global_T *pGData);
void Map0_PPU(Nes_Global_T *pGData, WORD wAddr );
void Map0_RenderScreen(Nes_Global_T *pGData, BYTE byMode );

void Map1_Init(Nes_Global_T *pGData);
void Map1_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map1_set_ROM_banks(Nes_Global_T *pGData);

void Map2_Init(Nes_Global_T *pGData);
void Map2_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map3_Init(Nes_Global_T *pGData);
void Map3_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map4_Init(Nes_Global_T *pGData);
void Map4_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map4_HSync(Nes_Global_T *pGData);
void Map4_Set_CPU_Banks(Nes_Global_T *pGData);
void Map4_Set_PPU_Banks(Nes_Global_T *pGData);

void Map5_Init(Nes_Global_T *pGData);
void Map5_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map5_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map5_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map5_HSync(Nes_Global_T *pGData);
void Map5_RenderScreen(Nes_Global_T *pGData, BYTE byMode );
void Map5_Sync_Prg_Banks( Nes_Global_T *pGData );

void Map6_Init(Nes_Global_T *pGData);
void Map6_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map6_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map6_HSync(Nes_Global_T *pGData);

void Map7_Init(Nes_Global_T *pGData);
void Map7_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map8_Init(Nes_Global_T *pGData);
void Map8_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map9_Init(Nes_Global_T *pGData);
void Map9_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map9_PPU(Nes_Global_T *pGData, WORD wAddr );

void Map10_Init(Nes_Global_T *pGData);
void Map10_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map10_PPU(Nes_Global_T *pGData, WORD wAddr );

void Map11_Init(Nes_Global_T *pGData);
void Map11_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map13_Init(Nes_Global_T *pGData);
void Map13_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map15_Init(Nes_Global_T *pGData);
void Map15_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map16_Init(Nes_Global_T *pGData);
void Map16_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map16_HSync(Nes_Global_T *pGData);

void Map17_Init(Nes_Global_T *pGData);
void Map17_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map17_HSync(Nes_Global_T *pGData);

void Map18_Init(Nes_Global_T *pGData);
void Map18_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map18_HSync(Nes_Global_T *pGData);

void Map19_Init(Nes_Global_T *pGData);
void Map19_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map19_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map19_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map19_HSync(Nes_Global_T *pGData);

void Map21_Init(Nes_Global_T *pGData);
void Map21_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map21_HSync(Nes_Global_T *pGData);

void Map22_Init(Nes_Global_T *pGData);
void Map22_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map23_Init(Nes_Global_T *pGData);
void Map23_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map23_HSync(Nes_Global_T *pGData);

void Map24_Init(Nes_Global_T *pGData);
void Map24_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map24_HSync(Nes_Global_T *pGData);

void Map25_Init(Nes_Global_T *pGData);
void Map25_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map25_Sync_Vrom(Nes_Global_T *pGData, int nBank );
void Map25_HSync(Nes_Global_T *pGData);

void Map26_Init(Nes_Global_T *pGData);
void Map26_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map26_HSync(Nes_Global_T *pGData);

void Map32_Init(Nes_Global_T *pGData);
void Map32_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map33_Init(Nes_Global_T *pGData);
void Map33_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map33_HSync(Nes_Global_T *pGData);

void Map34_Init(Nes_Global_T *pGData);
void Map34_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map34_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map40_Init(Nes_Global_T *pGData);
void Map40_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map40_HSync(Nes_Global_T *pGData);

void Map41_Init(Nes_Global_T *pGData);
void Map41_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map41_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map42_Init(Nes_Global_T *pGData);
void Map42_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map42_HSync(Nes_Global_T *pGData);

void Map43_Init(Nes_Global_T *pGData);
void Map43_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map43_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map43_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map43_HSync(Nes_Global_T *pGData);

void Map44_Init(Nes_Global_T *pGData);
void Map44_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map44_HSync(Nes_Global_T *pGData);
void Map44_Set_CPU_Banks(Nes_Global_T *pGData);
void Map44_Set_PPU_Banks(Nes_Global_T *pGData);

void Map45_Init(Nes_Global_T *pGData);
void Map45_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map45_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map45_HSync(Nes_Global_T *pGData);
void Map45_Set_CPU_Bank4(Nes_Global_T *pGData,  BYTE byData );
void Map45_Set_CPU_Bank5(Nes_Global_T *pGData,  BYTE byData );
void Map45_Set_CPU_Bank6(Nes_Global_T *pGData,  BYTE byData );
void Map45_Set_CPU_Bank7(Nes_Global_T *pGData,  BYTE byData );
void Map45_Set_PPU_Banks(Nes_Global_T *pGData);

void Map46_Init(Nes_Global_T *pGData);
void Map46_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map46_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map46_Set_ROM_Banks(Nes_Global_T *pGData);

void Map47_Init(Nes_Global_T *pGData);
void Map47_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map47_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map47_HSync(Nes_Global_T *pGData);
void Map47_Set_CPU_Banks(Nes_Global_T *pGData);
void Map47_Set_PPU_Banks(Nes_Global_T *pGData);

void Map48_Init(Nes_Global_T *pGData);
void Map48_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map48_HSync(Nes_Global_T *pGData);

void Map49_Init(Nes_Global_T *pGData);
void Map49_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map49_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map49_HSync(Nes_Global_T *pGData);
void Map49_Set_CPU_Banks(Nes_Global_T *pGData);
void Map49_Set_PPU_Banks(Nes_Global_T *pGData);

void Map50_Init(Nes_Global_T *pGData);
void Map50_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map50_HSync(Nes_Global_T *pGData);

void Map51_Init(Nes_Global_T *pGData);
void Map51_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map51_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map51_Set_CPU_Banks(Nes_Global_T *pGData);

void Map57_Init(Nes_Global_T *pGData);
void Map57_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map58_Init(Nes_Global_T *pGData);
void Map58_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map60_Init(Nes_Global_T *pGData);
void Map60_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map61_Init(Nes_Global_T *pGData);
void Map61_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map62_Init(Nes_Global_T *pGData);
void Map62_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map64_Init(Nes_Global_T *pGData);
void Map64_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map65_Init(Nes_Global_T *pGData);
void Map65_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map65_HSync(Nes_Global_T *pGData);

void Map66_Init(Nes_Global_T *pGData);
void Map66_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map67_Init(Nes_Global_T *pGData);
void Map67_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map67_HSync(Nes_Global_T *pGData);

void Map68_Init(Nes_Global_T *pGData);
void Map68_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map68_SyncMirror(Nes_Global_T *pGData);

void Map69_Init(Nes_Global_T *pGData);
void Map69_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map69_HSync(Nes_Global_T *pGData);

void Map70_Init(Nes_Global_T *pGData);
void Map70_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map71_Init(Nes_Global_T *pGData);
void Map71_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map72_Init(Nes_Global_T *pGData);
void Map72_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map73_Init(Nes_Global_T *pGData);
void Map73_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map73_HSync(Nes_Global_T *pGData);

void Map74_Init(Nes_Global_T *pGData);
void Map74_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map74_HSync(Nes_Global_T *pGData);
void Map74_Set_CPU_Banks(Nes_Global_T *pGData);
void Map74_Set_PPU_Banks(Nes_Global_T *pGData);

void Map75_Init(Nes_Global_T *pGData);
void Map75_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map76_Init(Nes_Global_T *pGData);
void Map76_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map77_Init(Nes_Global_T *pGData);
void Map77_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map78_Init(Nes_Global_T *pGData);
void Map78_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map79_Init(Nes_Global_T *pGData);
void Map79_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map80_Init(Nes_Global_T *pGData);
void Map80_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map82_Init(Nes_Global_T *pGData);
void Map82_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map83_Init(Nes_Global_T *pGData);
void Map83_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map83_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map83_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map83_HSync(Nes_Global_T *pGData);

void Map85_Init(Nes_Global_T *pGData);
void Map85_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map85_HSync(Nes_Global_T *pGData);

void Map86_Init(Nes_Global_T *pGData);
void Map86_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map87_Init(Nes_Global_T *pGData);
void Map87_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map88_Init(Nes_Global_T *pGData);
void Map88_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map89_Init(Nes_Global_T *pGData);
void Map89_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map90_Init(Nes_Global_T *pGData);
void Map90_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map90_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map90_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map90_HSync(Nes_Global_T *pGData);
void Map90_Sync_Mirror( Nes_Global_T *pGData );
void Map90_Sync_Prg_Banks( Nes_Global_T *pGData );
void Map90_Sync_Chr_Banks( Nes_Global_T *pGData );

void Map91_Init(Nes_Global_T *pGData);
void Map91_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map92_Init(Nes_Global_T *pGData);
void Map92_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map93_Init(Nes_Global_T *pGData);
void Map93_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map94_Init(Nes_Global_T *pGData);
void Map94_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map95_Init(Nes_Global_T *pGData);
void Map95_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map95_Set_CPU_Banks(Nes_Global_T *pGData);
void Map95_Set_PPU_Banks(Nes_Global_T *pGData);

void Map96_Init(Nes_Global_T *pGData);
void Map96_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map96_PPU(Nes_Global_T *pGData, WORD wAddr );
void Map96_Set_Banks(Nes_Global_T *pGData);

void Map97_Init(Nes_Global_T *pGData);
void Map97_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map99_Init(Nes_Global_T *pGData);
void Map99_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map99_ReadApu(Nes_Global_T *pGData, WORD wAddr );

void Map100_Init(Nes_Global_T *pGData);
void Map100_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map100_HSync(Nes_Global_T *pGData);
void Map100_Set_CPU_Banks(Nes_Global_T *pGData);
void Map100_Set_PPU_Banks(Nes_Global_T *pGData);

void Map101_Init(Nes_Global_T *pGData);
void Map101_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map105_Init(Nes_Global_T *pGData);
void Map105_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map105_HSync(Nes_Global_T *pGData);

void Map107_Init(Nes_Global_T *pGData);
void Map107_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map108_Init(Nes_Global_T *pGData);
void Map108_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map109_Init(Nes_Global_T *pGData);
void Map109_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map109_Set_PPU_Banks(Nes_Global_T *pGData);

void Map110_Init(Nes_Global_T *pGData);
void Map110_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map112_Init(Nes_Global_T *pGData);
void Map112_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map112_HSync(Nes_Global_T *pGData);
void Map112_Set_CPU_Banks(Nes_Global_T *pGData);
void Map112_Set_PPU_Banks(Nes_Global_T *pGData);

void Map113_Init(Nes_Global_T *pGData);
void Map113_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map113_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map114_Init(Nes_Global_T *pGData);
void Map114_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map114_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map114_HSync(Nes_Global_T *pGData);
void Map114_Set_CPU_Banks(Nes_Global_T *pGData);
void Map114_Set_PPU_Banks(Nes_Global_T *pGData);

void Map115_Init(Nes_Global_T *pGData);
void Map115_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map115_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map115_HSync(Nes_Global_T *pGData);
void Map115_Set_CPU_Banks(Nes_Global_T *pGData);
void Map115_Set_PPU_Banks(Nes_Global_T *pGData);

void Map116_Init(Nes_Global_T *pGData);
void Map116_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map116_HSync(Nes_Global_T *pGData);
void Map116_Set_CPU_Banks(Nes_Global_T *pGData);
void Map116_Set_PPU_Banks(Nes_Global_T *pGData);

void Map117_Init(Nes_Global_T *pGData);
void Map117_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map117_HSync(Nes_Global_T *pGData);

void Map118_Init(Nes_Global_T *pGData);
void Map118_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map118_HSync(Nes_Global_T *pGData);
void Map118_Set_CPU_Banks(Nes_Global_T *pGData);
void Map118_Set_PPU_Banks(Nes_Global_T *pGData);

void Map119_Init(Nes_Global_T *pGData);
void Map119_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map119_HSync(Nes_Global_T *pGData);
void Map119_Set_CPU_Banks(Nes_Global_T *pGData);
void Map119_Set_PPU_Banks(Nes_Global_T *pGData);

void Map122_Init(Nes_Global_T *pGData);
void Map122_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map133_Init(Nes_Global_T *pGData);
void Map133_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map134_Init(Nes_Global_T *pGData);
void Map134_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map135_Init(Nes_Global_T *pGData);
void Map135_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map135_Set_PPU_Banks(Nes_Global_T *pGData);

void Map140_Init(Nes_Global_T *pGData);
void Map140_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map140_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map151_Init(Nes_Global_T *pGData);
void Map151_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map160_Init(Nes_Global_T *pGData);
void Map160_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map160_HSync(Nes_Global_T *pGData);

void Map180_Init(Nes_Global_T *pGData);
void Map180_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map181_Init(Nes_Global_T *pGData);
void Map181_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map182_Init(Nes_Global_T *pGData);
void Map182_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map182_HSync(Nes_Global_T *pGData);

void Map183_Init(Nes_Global_T *pGData);
void Map183_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map183_HSync(Nes_Global_T *pGData);

void Map185_Init(Nes_Global_T *pGData);
void Map185_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map187_Init(Nes_Global_T *pGData);
void Map187_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map187_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map187_ReadApu(Nes_Global_T *pGData, WORD wAddr );
void Map187_HSync(Nes_Global_T *pGData);
void Map187_Set_CPU_Banks(Nes_Global_T *pGData);
void Map187_Set_PPU_Banks(Nes_Global_T *pGData);

void Map188_Init(Nes_Global_T *pGData);
void Map188_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map189_Init(Nes_Global_T *pGData);
void Map189_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map189_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map189_HSync(Nes_Global_T *pGData);

void Map191_Init(Nes_Global_T *pGData);
void Map191_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map191_Set_CPU_Banks(Nes_Global_T *pGData);
void Map191_Set_PPU_Banks(Nes_Global_T *pGData);

void Map193_Init(Nes_Global_T *pGData);
void Map193_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map194_Init(Nes_Global_T *pGData);
void Map194_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map200_Init(Nes_Global_T *pGData);
void Map200_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map201_Init(Nes_Global_T *pGData);
void Map201_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map202_Init(Nes_Global_T *pGData);
void Map202_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map202_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map202_WriteSub(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map222_Init(Nes_Global_T *pGData);
void Map222_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map225_Init(Nes_Global_T *pGData);
void Map225_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map226_Init(Nes_Global_T *pGData);
void Map226_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map227_Init(Nes_Global_T *pGData);
void Map227_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map228_Init(Nes_Global_T *pGData);
void Map228_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map229_Init(Nes_Global_T *pGData);
void Map229_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map230_Init(Nes_Global_T *pGData);
void Map230_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map231_Init(Nes_Global_T *pGData);
void Map231_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map232_Init(Nes_Global_T *pGData);
void Map232_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map233_Init(Nes_Global_T *pGData);
void Map233_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map234_Init(Nes_Global_T *pGData);
void Map234_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map234_Set_Banks(Nes_Global_T *pGData);

void Map235_Init(Nes_Global_T *pGData);
void Map235_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map236_Init(Nes_Global_T *pGData);
void Map236_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map240_Init(Nes_Global_T *pGData);
void Map240_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map241_Init(Nes_Global_T *pGData);
void Map241_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map242_Init(Nes_Global_T *pGData);
void Map242_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map243_Init(Nes_Global_T *pGData);
void Map243_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map244_Init(Nes_Global_T *pGData);
void Map244_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map245_Init(Nes_Global_T *pGData);
void Map245_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map245_HSync(Nes_Global_T *pGData);
#if 0
void Map245_Set_CPU_Banks(Nes_Global_T *pGData);
void Map245_Set_PPU_Banks(Nes_Global_T *pGData);
#endif 

void Map246_Init(Nes_Global_T *pGData);
void Map246_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

void Map248_Init(Nes_Global_T *pGData);
void Map248_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map248_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map248_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map248_HSync(Nes_Global_T *pGData);
void Map248_Set_CPU_Banks(Nes_Global_T *pGData);
void Map248_Set_PPU_Banks(Nes_Global_T *pGData);

void Map249_Init(Nes_Global_T *pGData);
void Map249_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map249_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map249_HSync(Nes_Global_T *pGData);

void Map251_Init(Nes_Global_T *pGData);
void Map251_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map251_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map251_Set_Banks(Nes_Global_T *pGData);

void Map252_Init(Nes_Global_T *pGData);
void Map252_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map252_HSync(Nes_Global_T *pGData);

void Map255_Init(Nes_Global_T *pGData);
void Map255_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
void Map255_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
BYTE Map255_ReadApu(Nes_Global_T *pGData, WORD wAddr );

#endif /* !InfoNES_MAPPER_H_INCLUDED */
