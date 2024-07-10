/*===================================================================*/
/*                                                                   */
/*                   Mapper 236 : 800-in-1                           */
/*                                                                   */
/*===================================================================*/

BYTE    Map236_Bank, Map236_Mode;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 236                                            */
/*-------------------------------------------------------------------*/
void Map236_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map236_Init;

  /* Write to Mapper */
  MapperWrite = Map236_Write;

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

  /* Set Registers */
  Map236_Bank = Map236_Mode = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 236 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map236_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if( wAddr >= 0x8000 && wAddr <= 0xBFFF ) {
    Map236_Bank = ((wAddr&0x03)<<4)|(Map236_Bank&0x07);
  } else {
    Map236_Bank = (wAddr&0x07)|(Map236_Bank&0x30);
    Map236_Mode = wAddr&0x30;
  }

  if( wAddr & 0x20 ) {
    InfoNES_Mirroring(pGData, 0 );
  } else {
    InfoNES_Mirroring(pGData, 1 );
  }

  switch( Map236_Mode ) {
  case	0x00:
    Map236_Bank |= 0x08;
    pGData->ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x10:
    Map236_Bank |= 0x37;
    pGData->ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x20:
    Map236_Bank |= 0x08;
    pGData->ROMBANK0 = ROMPAGE((((Map236_Bank&0xFE)<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((((Map236_Bank&0xFE)<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((((Map236_Bank&0xFE)<<1)+2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((((Map236_Bank&0xFE)<<1)+3) % (NesHeader.byRomSize<<1));
    break;
  case	0x30:
    Map236_Bank |= 0x08;
    pGData->ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  }
}
