/*===================================================================*/
/*                                                                   */
/*              Mapper 92 (Jaleco Early Mapper #1)                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 92                                             */
/*-------------------------------------------------------------------*/
void Map92_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map92_Init;

  /* Write to Mapper */
  MapperWrite = Map92_Write;

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
/*  Mapper 92 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map92_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byRomBank;
  BYTE byChrBank;
  
  byData = (BYTE)wAddr & 0xff;
  byRomBank = ( byData & 0x0f ) << 1;
  byChrBank = byData & 0x0f;

  if ( wAddr >= 0x9000 )
  {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xd0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        pGData->ROMBANK2 = ROMPAGE( byRomBank );
        pGData->ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;
        
      /* Set PPU Banks */
      case 0xe0:
        byChrBank <<= 3;
        byChrBank %= ( NesHeader.byVRomSize << 3 );
        pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank );
        pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
        pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
        pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
        pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
        pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
        pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
        pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
        InfoNES_SetupChr(pGData);
        break;
    }
  } else {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xb0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        pGData->ROMBANK2 = ROMPAGE( byRomBank );
        pGData->ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;

      /* Set PPU Banks */
      case 0x70:
        byChrBank <<= 3;
        byChrBank %= ( NesHeader.byVRomSize << 3 );
        pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank );
        pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
        pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
        pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
        pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
        pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
        pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
        pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
        InfoNES_SetupChr(pGData);
        break;
    }
  }
}
