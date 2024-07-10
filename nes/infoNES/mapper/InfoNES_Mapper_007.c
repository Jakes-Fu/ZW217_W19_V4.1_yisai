/*===================================================================*/
/*                                                                   */
/*                        Mapper 7 (AOROM)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 7                                              */
/*-------------------------------------------------------------------*/
void Map7_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map7_Init;

  /* Write to Mapper */
  MapperWrite = Map7_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 7 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map7_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byBank;

  /* Set ROM Banks */
  byBank = ( byData & 0x07 ) << 2;
  byBank %= ( NesHeader.byRomSize << 1 );

  pGData->ROMBANK0 = ROMPAGE( byBank );
  pGData->ROMBANK1 = ROMPAGE( byBank + 1 );
  pGData->ROMBANK2 = ROMPAGE( byBank + 2 );
  pGData->ROMBANK3 = ROMPAGE( byBank + 3 );

  /* Name Table Mirroring */
  InfoNES_Mirroring(pGData, byData & 0x10 ? 2 : 3 );
}
