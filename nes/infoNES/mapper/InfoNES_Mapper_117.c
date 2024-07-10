/*===================================================================*/
/*                                                                   */
/*                    Mapper 117 (PC-Future)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map117_IRQ_Line;
BYTE Map117_IRQ_Enable1;
BYTE Map117_IRQ_Enable2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 117                                            */
/*-------------------------------------------------------------------*/
void Map117_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map117_Init;

  /* Write to Mapper */
  MapperWrite = Map117_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map117_HSync;

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

  /* Initialize IRQ Registers */
  Map117_IRQ_Line = 0;
  Map117_IRQ_Enable1 = 0;
  Map117_IRQ_Enable2 = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map117_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
  
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK2 = ROMPAGE( byData );
      break;

    /* Set PPU Banks */
    case 0xA000:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA001:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA002:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA003:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA004:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA005:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA006:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xA007:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0xc001:
    case 0xc002:
    case 0xc003:
      Map117_IRQ_Enable1 = Map117_IRQ_Line = byData;
      break;

    case 0xe000:
      Map117_IRQ_Enable2 = byData & 0x01;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map117_HSync(Nes_Global_T *pGData)
{
  if ( Map117_IRQ_Enable1 && Map117_IRQ_Enable2 )
  {
    if ( Map117_IRQ_Line == pGData->PPU_Scanline )
    {
      Map117_IRQ_Enable1 = 0x00;
      IRQ_REQ;
    }
  }
}
