/*===================================================================*/
/*                                                                   */
/*                      Mapper 231 : 20-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 231                                            */
/*-------------------------------------------------------------------*/
void Map231_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map231_Init;

  /* Write to Mapper */
  MapperWrite = Map231_Write;

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
/*  Mapper 231 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map231_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE	byBank = wAddr & 0x1E;

  if( wAddr & 0x0020 ) {
    pGData->ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE(((byBank<<1)+2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE(((byBank<<1)+3) % (NesHeader.byRomSize<<1));
  } else {
    pGData->ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
  }

  if( wAddr & 0x0080 ) {
    InfoNES_Mirroring(pGData, 0 );
  } else {
    InfoNES_Mirroring(pGData, 1 );
  }
}
