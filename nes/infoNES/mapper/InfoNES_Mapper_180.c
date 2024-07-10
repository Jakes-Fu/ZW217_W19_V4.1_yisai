/*===================================================================*/
/*                                                                   */
/*                      Mapper 180  (Nichibutsu)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 180                                            */
/*-------------------------------------------------------------------*/
void Map180_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map180_Init;

  /* Write to Mapper */
  MapperWrite = Map180_Write;

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
/*  Mapper 180 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map180_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */ 
  byData &= 0x07;
  byData <<= 1;
  byData %= ( NesHeader.byRomSize << 1 );
  pGData->ROMBANK2 = ROMPAGE( byData );
  pGData->ROMBANK3 = ROMPAGE( byData + 1 );
}
