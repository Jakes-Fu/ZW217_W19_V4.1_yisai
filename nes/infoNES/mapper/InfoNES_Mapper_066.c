/*===================================================================*/
/*                                                                   */
/*                        Mapper 66 (GNROM)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 66                                             */
/*-------------------------------------------------------------------*/
void Map66_Init(Nes_Global_T *pGData)
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map66_Init;

  /* Write to Mapper */
  MapperWrite = Map66_Write;

  /* Write to SRAM */
  MapperSram = Map66_Write;

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
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 0 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 66 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map66_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byRom;
  BYTE byVRom;

  byRom  = ( byData >> 4 ) & 0x0F;
  byVRom = byData & 0x0F;

  /* Set ROM Banks */
  byRom <<= 1;
  byRom %= NesHeader.byRomSize;
  byRom <<= 1;

  pGData->ROMBANK0 = ROMPAGE( byRom );
  pGData->ROMBANK1 = ROMPAGE( byRom + 1 );
  pGData->ROMBANK2 = ROMPAGE( byRom + 2 );
  pGData->ROMBANK3 = ROMPAGE( byRom + 3 );

  /* Set PPU Banks */
  byVRom <<= 3;
  byVRom %= ( NesHeader.byVRomSize << 3 );

  pGData->PPUBANK[ 0 ] = VROMPAGE( byVRom );
  pGData->PPUBANK[ 1 ] = VROMPAGE( byVRom + 1 );
  pGData->PPUBANK[ 2 ] = VROMPAGE( byVRom + 2 );
  pGData->PPUBANK[ 3 ] = VROMPAGE( byVRom + 3 );
  pGData->PPUBANK[ 4 ] = VROMPAGE( byVRom + 4 );
  pGData->PPUBANK[ 5 ] = VROMPAGE( byVRom + 5 );
  pGData->PPUBANK[ 6 ] = VROMPAGE( byVRom + 6 );
  pGData->PPUBANK[ 7 ] = VROMPAGE( byVRom + 7 );
  InfoNES_SetupChr(pGData);
}
