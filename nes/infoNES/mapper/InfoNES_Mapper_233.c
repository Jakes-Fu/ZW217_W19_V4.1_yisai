/*===================================================================*/
/*                                                                   */
/*                     Mapper 233 : 42-in-1                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 233                                            */
/*-------------------------------------------------------------------*/
void Map233_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map233_Init;

  /* Write to Mapper */
  MapperWrite = Map233_Write;

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
/*  Mapper 233 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map233_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if( byData & 0x20 ) {
    pGData->ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
    pGData->ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1)); 
    pGData->ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
  } else {
    pGData->ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+0) % (NesHeader.byRomSize<<1)); 
    pGData->ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+1) % (NesHeader.byRomSize<<1)); 
    pGData->ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+3) % (NesHeader.byRomSize<<1)); 
  }

  if( (byData&0xC0) == 0x00 ) {
    InfoNES_Mirroring(pGData, 5 );
  } else if( (byData&0xC0) == 0x40 ) {
    InfoNES_Mirroring(pGData, 1 );
  } else if( (byData&0xC0) == 0x80 ) {
    InfoNES_Mirroring(pGData, 0 );
  } else {
    InfoNES_Mirroring(pGData, 2 );
  }
}
