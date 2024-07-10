/*===================================================================*/
/*                                                                   */
/*                 Mapper 70 (74161/32 Bandai)                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 70                                             */
/*-------------------------------------------------------------------*/
void Map70_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map70_Init;

  /* Write to Mapper */
  MapperWrite = Map70_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  pGData->SRAMBANK = pGData->SRAM;


  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMLASTPAGE( 1 );
  pGData->ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 70 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map70_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byChrBank = byData & 0x0f;
  BYTE byPrgBank = ( byData & 0x70 ) >> 4;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  pGData->ROMBANK0 = ROMPAGE( byPrgBank );
  pGData->ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr(pGData);

  /* Name Table Mirroring */
  if ( byData & 0x80 )
  {
    InfoNES_Mirroring(pGData, 2 );
  } else {
    InfoNES_Mirroring(pGData, 3 );
  }
}
