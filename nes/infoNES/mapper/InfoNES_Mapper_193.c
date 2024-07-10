/*===================================================================*/
/*                                                                   */
/*         Mapper 193 : MEGA SOFT (NTDEC) : Fighting Hero            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 193                                            */
/*-------------------------------------------------------------------*/
void Map193_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map193_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map193_Sram;

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
  pGData->ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  pGData->ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  pGData->ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  pGData->ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

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
/*  Mapper 193 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map193_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x6000:
    pGData->PPUBANK[ 0 ] = VROMPAGE( ((byData&0xfc) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    pGData->PPUBANK[ 1 ] = VROMPAGE( ((byData&0xfc) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    pGData->PPUBANK[ 2 ] = VROMPAGE( ((byData&0xfc) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
    pGData->PPUBANK[ 3 ] = VROMPAGE( ((byData&0xfc) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0x6001:
    pGData->PPUBANK[ 4 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    pGData->PPUBANK[ 5 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0x6002:
    pGData->PPUBANK[ 6 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    pGData->PPUBANK[ 7 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0x6003:
    pGData->ROMBANK0 = ROMPAGE( ((byData<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK1 = ROMPAGE( ((byData<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK2 = ROMPAGE( ((byData<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK3 = ROMPAGE( ((byData<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );
    break;
  }
}
