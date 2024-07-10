/*===================================================================*/
/*                                                                   */
/*                        Mapper 91 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 91                                             */
/*-------------------------------------------------------------------*/
void Map91_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map91_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map91_Sram;

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
  pGData->ROMBANK0 = ROMLASTPAGE( 1 );
  pGData->ROMBANK1 = ROMLASTPAGE( 0 );
  pGData->ROMBANK2 = ROMLASTPAGE( 1 );
  pGData->ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set Name Table Mirroring */
  InfoNES_Mirroring(pGData, 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 91 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map91_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF00F)
  {
    /* Set PPU Banks */
    case 0x6000:
      pGData->PPUBANK[ 0 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x6001:
      pGData->PPUBANK[ 2 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x6002:
      pGData->PPUBANK[ 4 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x6003:
      pGData->PPUBANK[ 6 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    /* Set CPU Banks */
    case 0x7000:
      pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;

    case 0x7001:
      pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;
  }
}
