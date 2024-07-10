/*===================================================================*/
/*                                                                   */
/*                      Mapper 93 (74161/32)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 93                                             */
/*-------------------------------------------------------------------*/
void Map93_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map93_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map93_Sram;

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
/*  Mapper 93 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map93_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  if ( wAddr == 0x6000 )
  {
    byData <<= 1;
    byData %= ( NesHeader.byRomSize << 1 );
  
    pGData->ROMBANK0 = ROMPAGE( byData );
    pGData->ROMBANK1 = ROMPAGE( byData + 1 );
  }
}
