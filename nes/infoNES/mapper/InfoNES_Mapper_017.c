/*===================================================================*/
/*                                                                   */
/*                 Mapper 17 (FFE F8 Series)                         */
/*                                                                   */
/*===================================================================*/

BYTE  Map17_IRQ_Enable;
DWORD Map17_IRQ_Cnt;
DWORD Map17_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 17                                             */
/*-------------------------------------------------------------------*/
void Map17_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map17_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map17_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map17_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Set SRAM Banks */
  pGData->SRAMBANK = pGData->SRAM;


  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMLASTPAGE( 1 );
  pGData->ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
	  int nPage;
    for ( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Initialize State Registers */
  Map17_IRQ_Enable = 0;
  Map17_IRQ_Cnt = 0;
  Map17_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map17_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x42fe:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring(pGData, 3 );
      } else {
        InfoNES_Mirroring(pGData, 2 );
      }
      break;

    case 0x42ff:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring(pGData, 1 );
      } else {
        InfoNES_Mirroring(pGData, 0 );
      }
      break;

    case 0x4501:
      Map17_IRQ_Enable = 0;
      break;

    case 0x4502:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0xff00 ) | byData;
      break;

    case 0x4503:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0x00ff ) | ( (DWORD)byData << 8 );
      Map17_IRQ_Cnt = Map17_IRQ_Latch;
      Map17_IRQ_Enable = 1;
      break;

    case 0x4504:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x4505:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x4506:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK2 = ROMPAGE( byData );
      break;

    case 0x4507:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK3 = ROMPAGE( byData );
      break;

    case 0x4510:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4511:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4512:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4513:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4514:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4515:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4516:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    case 0x4517:
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map17_HSync(Nes_Global_T *pGData)
{
  if ( Map17_IRQ_Enable )
  {
    if ( Map17_IRQ_Cnt >= 0xffff - 113 )
    {
      IRQ_REQ;
      Map17_IRQ_Cnt = 0;
      Map17_IRQ_Enable = 0;
    } else {
      Map17_IRQ_Cnt += 113;
    }
  }
}
