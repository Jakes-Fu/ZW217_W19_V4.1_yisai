/*===================================================================*/
/*                                                                   */
/*                      Mapper 122 (Sunsoft)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 122                                            */
/*-------------------------------------------------------------------*/
void Map122_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map122_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map122_Sram;

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
  pGData->ROMBANK2 = ROMPAGE( 2 );
  pGData->ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 122 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map122_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if ( wAddr == 0x6000 )
  {
    BYTE byChrBank0 = byData & 0x07;
    BYTE byChrBank1 = ( byData & 0x70 ) >> 4;

    byChrBank0 = ( byChrBank0 << 2 ) % ( NesHeader.byVRomSize << 3 );
    byChrBank1 = ( byChrBank1 << 2 ) % ( NesHeader.byVRomSize << 3 );

    pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank0 + 0 );
    pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank0 + 1 );
    pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank0 + 2 );
    pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank0 + 3 );
    pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank1 + 0 );
    pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank1 + 1 );
    pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank1 + 2 );
    pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank1 + 3 );
    InfoNES_SetupChr(pGData);
  }
}
