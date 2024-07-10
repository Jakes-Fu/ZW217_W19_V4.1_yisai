/*===================================================================*/
/*                                                                   */
/*                        Mapper 13 : CPROM                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 13                                             */
/*-------------------------------------------------------------------*/
void Map13_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map13_Init;

  /* Write to Mapper */
  MapperWrite = Map13_Write;

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
  pGData->PPUBANK[ 0 ] = CRAMPAGE( 0 );
  pGData->PPUBANK[ 1 ] = CRAMPAGE( 1 );
  pGData->PPUBANK[ 2 ] = CRAMPAGE( 2 );
  pGData->PPUBANK[ 3 ] = CRAMPAGE( 3 );
  pGData->PPUBANK[ 4 ] = CRAMPAGE( 0 );
  pGData->PPUBANK[ 5 ] = CRAMPAGE( 1 );
  pGData->PPUBANK[ 6 ] = CRAMPAGE( 2 );
  pGData->PPUBANK[ 7 ] = CRAMPAGE( 3 );
  InfoNES_SetupChr(pGData);

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 13 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map13_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE((((byData&0x30)>>2)+0) % (NesHeader.byRomSize<<1));
  pGData->ROMBANK1 = ROMPAGE((((byData&0x30)>>2)+1) % (NesHeader.byRomSize<<1));
  pGData->ROMBANK2 = ROMPAGE((((byData&0x30)>>2)+2) % (NesHeader.byRomSize<<1));
  pGData->ROMBANK3 = ROMPAGE((((byData&0x30)>>2)+3) % (NesHeader.byRomSize<<1));

  /* Set PPU Banks */
  pGData->PPUBANK[ 4 ] = CRAMPAGE(((byData&0x03)<<2)+0);
  pGData->PPUBANK[ 5 ] = CRAMPAGE(((byData&0x03)<<2)+1);
  pGData->PPUBANK[ 6 ] = CRAMPAGE(((byData&0x03)<<2)+2);
  pGData->PPUBANK[ 7 ] = CRAMPAGE(((byData&0x03)<<2)+3);
  InfoNES_SetupChr(pGData);
}
