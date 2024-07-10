/*===================================================================*/
/*                                                                   */
/*                       Mapper 230 : 22-in-1                        */
/*                                                                   */
/*===================================================================*/

BYTE Map230_RomSw;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 230                                            */
/*-------------------------------------------------------------------*/
void Map230_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map230_Init;

  /* Write to Mapper */
  MapperWrite = Map230_Write;

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


  /* Initialize Registers */
  if( Map230_RomSw ) {
    Map230_RomSw = 0;
  } else {
    Map230_RomSw = 1;
  }

  /* Set ROM Banks */
  if( Map230_RomSw ) {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 1 );
    pGData->ROMBANK2 = ROMPAGE( 14 );
    pGData->ROMBANK3 = ROMPAGE( 15 );
  } else {
    pGData->ROMBANK0 = ROMPAGE( 16 );
    pGData->ROMBANK1 = ROMPAGE( 17 );
    pGData->ROMBANK2 = ROMLASTPAGE( 1 );
    pGData->ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 230 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map230_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if( Map230_RomSw ) {
    pGData->ROMBANK0 = ROMPAGE((((byData&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((((byData&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  } else {
    if( byData & 0x20 ) {
      pGData->ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
    } else {
      pGData->ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+16) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+17) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+18) % (NesHeader.byRomSize<<1));
      pGData->ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+19) % (NesHeader.byRomSize<<1));
    }
    if( byData & 0x40 ) {
      InfoNES_Mirroring(pGData, 1 );
    } else {
      InfoNES_Mirroring(pGData, 0 );
    }
  }
}
