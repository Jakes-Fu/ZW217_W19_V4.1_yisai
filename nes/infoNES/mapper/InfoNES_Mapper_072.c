/*===================================================================*/
/*                                                                   */
/*           Mapper 72 (Jaleco Early Mapper #0)                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 72                                             */
/*-------------------------------------------------------------------*/
void Map72_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map72_Init;

  /* Write to Mapper */
  MapperWrite = Map72_Write;

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
/*  Mapper 72 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map72_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byBank = byData & 0x0f;

  if ( byData & 0x80 )
  {
    /* Set ROM Banks */
    byBank <<= 1;
    byBank %= ( NesHeader.byRomSize << 1 );
    pGData->ROMBANK0 = ROMPAGE( byBank );
    pGData->ROMBANK1 = ROMPAGE( byBank + 1 );
  } else 
  if ( byData & 0x40 )
  {
    /* Set PPU Banks */
    byBank <<= 3;
    byBank %= ( NesHeader.byVRomSize << 3 );
    pGData->PPUBANK[ 0 ] = VROMPAGE( byBank );
    pGData->PPUBANK[ 1 ] = VROMPAGE( byBank + 1 );
    pGData->PPUBANK[ 2 ] = VROMPAGE( byBank + 2 );
    pGData->PPUBANK[ 3 ] = VROMPAGE( byBank + 3 );
    pGData->PPUBANK[ 4 ] = VROMPAGE( byBank + 4 );
    pGData->PPUBANK[ 5 ] = VROMPAGE( byBank + 5 );
    pGData->PPUBANK[ 6 ] = VROMPAGE( byBank + 6 );
    pGData->PPUBANK[ 7 ] = VROMPAGE( byBank + 7 );
    InfoNES_SetupChr(pGData);
  }
}
