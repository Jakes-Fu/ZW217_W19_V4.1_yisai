/*===================================================================*/
/*                                                                   */
/*                            Mapper 108                             */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 108                                            */
/*-------------------------------------------------------------------*/
void Map108_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map108_Init;

  /* Write to Mapper */
  MapperWrite = Map108_Write;

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
  pGData->SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0xC % ( NesHeader.byRomSize << 1 ) );
  pGData->ROMBANK1 = ROMPAGE( 0xD % ( NesHeader.byRomSize << 1 ) );
  pGData->ROMBANK2 = ROMPAGE( 0xE % ( NesHeader.byRomSize << 1 ) );
  pGData->ROMBANK3 = ROMPAGE( 0xF % ( NesHeader.byRomSize << 1 ) );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 108 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map108_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set SRAM Banks */
  pGData->SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
