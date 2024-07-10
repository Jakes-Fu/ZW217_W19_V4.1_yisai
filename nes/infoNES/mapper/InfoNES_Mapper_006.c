/*===================================================================*/
/*                                                                   */
/*                       Mapper 6 (FFE)                              */
/*                                                                   */
/*===================================================================*/

BYTE Map6_IRQ_Enable;
DWORD Map6_IRQ_Cnt;
//BYTE Map6_Chr_Ram[ 0x2000 * 4 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 6                                              */
/*-------------------------------------------------------------------*/
void Map6_Init(Nes_Global_T *pGData)
{
  int nPage;

  Map6_Chr_Ram = (BYTE *)NesMallocFromWRE(0x2000 * 4 * sizeof (BYTE));
  if (NULL == Map6_Chr_Ram)
  	return;

  /* Initialize Mapper */
  MapperInit = Map6_Init;

  /* Write to Mapper */
  MapperWrite = Map6_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map6_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map6_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  pGData->SRAMBANK = pGData->SRAM;


  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMPAGE( 14 );
  pGData->ROMBANK3 = ROMPAGE( 15 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr(pGData);
  }
  else
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      pGData->PPUBANK[ nPage ] = Map6_VROMPAGE( nPage );
    }
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void Map6_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Name Table Mirroring */
    case 0x42fe:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring(pGData, 2 );
      } else {
        InfoNES_Mirroring(pGData, 3 );
      }
      break;

    case 0x42ff:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring(pGData, 0 );
      } else {
        InfoNES_Mirroring(pGData, 1 );
      }
      break;

    case 0x4501:
      Map6_IRQ_Enable = 0;
      break;

    case 0x4502:
      Map6_IRQ_Cnt = ( Map6_IRQ_Cnt & 0xff00 ) | (DWORD)byData;
      break;

    case 0x4503:
      Map6_IRQ_Cnt = ( Map6_IRQ_Cnt & 0x00ff ) | ( (DWORD)byData << 8 );
      Map6_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map6_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = ( byData & 0x3c ) >> 2;
  BYTE byChrBank = byData & 0x03;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  pGData->ROMBANK0 = ROMPAGE( byPrgBank );
  pGData->ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  pGData->PPUBANK[ 0 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 0 * 0x400 ];
  pGData->PPUBANK[ 1 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 1 * 0x400 ];
  pGData->PPUBANK[ 2 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 2 * 0x400 ];
  pGData->PPUBANK[ 3 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 3 * 0x400 ];
  pGData->PPUBANK[ 4 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 4 * 0x400 ];
  pGData->PPUBANK[ 5 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 5 * 0x400 ];
  pGData->PPUBANK[ 6 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 6 * 0x400 ];
  pGData->PPUBANK[ 7 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 7 * 0x400 ];
  InfoNES_SetupChr(pGData);
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map6_HSync(Nes_Global_T *pGData)
{
  if ( Map6_IRQ_Enable )
  {
    Map6_IRQ_Cnt += 133;
    if ( Map6_IRQ_Cnt >= 0xffff )
    {
      IRQ_REQ;
      Map6_IRQ_Cnt = 0;
    }
  }
}
