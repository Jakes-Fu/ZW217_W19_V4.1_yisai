/*===================================================================*/
/*                                                                   */
/*            Mapper 194 : Meikyuu Jiin Dababa                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 194                                            */
/*-------------------------------------------------------------------*/
void Map194_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map194_Init;

  /* Write to Mapper */
  MapperWrite = Map194_Write;

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
  pGData->ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  pGData->ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  pGData->ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  pGData->ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 194 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map194_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  pGData->SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
