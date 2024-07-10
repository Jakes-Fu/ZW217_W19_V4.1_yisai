/*===================================================================*/
/*                                                                   */
/*                        Mapper 89 (Sunsoft)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 89                                             */
/*-------------------------------------------------------------------*/
void Map89_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map89_Init;

  /* Write to Mapper */
  MapperWrite = Map89_Write;

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

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );

    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 89 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map89_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if ( ( wAddr & 0xFF00 ) == 0xC000 )
  {
    BYTE byPrgBank = (byData & 0x70) >> 4;
    BYTE byChrBank = ((byData & 0x80) >> 4) | (byData & 0x07);

    /* Set ROM Banks */
    pGData->ROMBANK0 = ROMPAGE( (byPrgBank*2+0) % (NesHeader.byRomSize << 1) );
    pGData->ROMBANK1 = ROMPAGE( (byPrgBank*2+1) % (NesHeader.byRomSize << 1) );

    pGData->PPUBANK[ 0 ] = VROMPAGE( (byChrBank*8+0) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 1 ] = VROMPAGE( (byChrBank*8+1) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 2 ] = VROMPAGE( (byChrBank*8+2) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 3 ] = VROMPAGE( (byChrBank*8+3) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 4 ] = VROMPAGE( (byChrBank*8+4) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 5 ] = VROMPAGE( (byChrBank*8+5) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 6 ] = VROMPAGE( (byChrBank*8+6) % (NesHeader.byVRomSize << 3) );
    pGData->PPUBANK[ 7 ] = VROMPAGE( (byChrBank*8+7) % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr(pGData);

    if ( byData & 0x08 )
    {
      InfoNES_Mirroring(pGData, 2 );
    }
    else
    {
      InfoNES_Mirroring(pGData, 3 );
    }
  }
}
