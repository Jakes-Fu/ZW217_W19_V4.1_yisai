/*===================================================================*/
/*                                                                   */
/*                   Mapper 135 : SACHEN CHEN                        */
/*                                                                   */
/*===================================================================*/

BYTE    Map135_Cmd;
BYTE	Map135_Chr0l, Map135_Chr1l, Map135_Chr0h, Map135_Chr1h, Map135_Chrch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 135                                            */
/*-------------------------------------------------------------------*/
void Map135_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map135_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map135_Apu;

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
  Map135_Cmd = 0;
  Map135_Chr0l = Map135_Chr1l = Map135_Chr0h = Map135_Chr1h = Map135_Chrch = 0;

  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMPAGE( 2 );
  pGData->ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map135_Set_PPU_Banks(pGData);

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map135_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr & 0x4101 ) {
  case	0x4100:
    Map135_Cmd = byData & 0x07;
    break;
  case	0x4101:
    switch( Map135_Cmd ) {
    case	0:
      Map135_Chr0l = byData & 0x07;
      Map135_Set_PPU_Banks(pGData);
      break;
    case	1:
      Map135_Chr0h = byData & 0x07;
      Map135_Set_PPU_Banks(pGData);
      break;
    case	2:
      Map135_Chr1l = byData & 0x07;
      Map135_Set_PPU_Banks(pGData);
      break;
    case	3:
      Map135_Chr1h = byData & 0x07;
      Map135_Set_PPU_Banks(pGData);
      break;
    case	4:
      Map135_Chrch = byData & 0x07;
      Map135_Set_PPU_Banks(pGData);
      break;
    case	5:
      /* Set ROM Banks */
      pGData->ROMBANK0 = ROMPAGE( (((byData%0x07)<<2) + 0 ) % (NesHeader.byRomSize << 1) );
      pGData->ROMBANK1 = ROMPAGE( (((byData%0x07)<<2) + 1 ) % (NesHeader.byRomSize << 1) );
      pGData->ROMBANK2 = ROMPAGE( (((byData%0x07)<<2) + 2 ) % (NesHeader.byRomSize << 1) );
      pGData->ROMBANK3 = ROMPAGE( (((byData%0x07)<<2) + 3 ) % (NesHeader.byRomSize << 1) );
      break;
    case	6:
      break;
    case	7:
      switch( (byData>>1)&0x03 ) {
      case	0: InfoNES_Mirroring(pGData, 2 ); break;
      case	1: InfoNES_Mirroring(pGData, 0  ); break;
      case	2: InfoNES_Mirroring(pGData, 1  ); break;
      case	3: InfoNES_Mirroring(pGData, 2 ); break;
      }
      break;
    }
    break;
  }
  //Map135_Wram[ wAddr & 0x1fff ] = byData;
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map135_Set_PPU_Banks(Nes_Global_T *pGData)
{
  /* Set PPU Banks */
  pGData->PPUBANK[ 0 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 1 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 2 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 3 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 4 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 5 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 6 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  pGData->PPUBANK[ 7 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr(pGData);
}
