/*===================================================================*/
/*                                                                   */
/*                   Mapper 225 : 72-in-1                            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 225                                            */
/*-------------------------------------------------------------------*/
void Map225_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map225_Init;

  /* Write to Mapper */
  MapperWrite = Map225_Write;

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
  pGData->ROMBANK2 = ROMPAGE( 2 );
  pGData->ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 225 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map225_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = (wAddr & 0x0F80) >> 7;
  BYTE byChrBank = wAddr & 0x003F;
  
  pGData->PPUBANK[ 0 ] = VROMPAGE(((byChrBank<<3)+0) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 1 ] = VROMPAGE(((byChrBank<<3)+1) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 2 ] = VROMPAGE(((byChrBank<<3)+2) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 3 ] = VROMPAGE(((byChrBank<<3)+3) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 4 ] = VROMPAGE(((byChrBank<<3)+4) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 5 ] = VROMPAGE(((byChrBank<<3)+5) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 6 ] = VROMPAGE(((byChrBank<<3)+6) % ( NesHeader.byVRomSize << 3 ) );
  pGData->PPUBANK[ 7 ] = VROMPAGE(((byChrBank<<3)+7) % ( NesHeader.byVRomSize << 3 ) );
  InfoNES_SetupChr(pGData);

  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring(pGData, 0 );
  } else {
    InfoNES_Mirroring(pGData, 1 );
  }
  
  if( wAddr & 0x1000 ) {
    // 16KBbank
    if( wAddr & 0x0040 ) {
      pGData->ROMBANK0 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK1 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
    } else {
      pGData->ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK2 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK3 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    }
  } else {
    pGData->ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
  }
}
