/*===================================================================*/
/*                                                                   */
/*                          Mapper 222                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 222                                            */
/*-------------------------------------------------------------------*/
void Map222_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map222_Init;

  /* Write to Mapper */
  MapperWrite = Map222_Write;

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
  if ( NesHeader.byVRomSize > 0 ) {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set Mirroring */
  InfoNES_Mirroring(pGData, 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 222 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map222_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF003 ) {
  case	0x8000:
    pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xA000:
    pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xB000:
    pGData->PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xB002:
    pGData->PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xC000:
    pGData->PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xC002:
    pGData->PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xD000:
    pGData->PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xD002:
    pGData->PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xE000:
    pGData->PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  case	0xE002:
    pGData->PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr(pGData);
    break;
  }
}
