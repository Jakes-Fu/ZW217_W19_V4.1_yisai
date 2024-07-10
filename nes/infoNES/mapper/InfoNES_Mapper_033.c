/*===================================================================*/
/*                                                                   */
/*                 Mapper 33 (Taito TC0190/TC0350)                   */
/*                                                                   */
/*===================================================================*/

BYTE Map33_Regs[ 8 ];
BYTE Map33_Switch;

BYTE Map33_IRQ_Enable;
BYTE Map33_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 33                                             */
/*-------------------------------------------------------------------*/
void Map33_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map33_Init;

  /* Write to Mapper */
  MapperWrite = Map33_Write;

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

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
	  int nPage;
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
      Map33_Regs[ nPage ] = nPage;
    }
    InfoNES_SetupChr(pGData);
  } 
  else 
  {
	  int nPage;
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      Map33_Regs[ nPage ] = 0;
    }
  }

  /* Initialize State Registers */
  Map33_Switch = 0;
  Map33_IRQ_Enable = 0;
  Map33_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map33_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  switch ( wAddr )
  {
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      Map33_Regs[ 0 ] = byData * 2;
      Map33_Regs[ 1 ] = byData * 2 + 1;

      pGData->PPUBANK[ 0 ] = VROMPAGE( Map33_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( Map33_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x8003:
      Map33_Regs[ 2 ] = byData * 2;
      Map33_Regs[ 3 ] = byData * 2 + 1;

      pGData->PPUBANK[ 2 ] = VROMPAGE( Map33_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( Map33_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa000:
      Map33_Regs[ 4 ] = byData;
      pGData->PPUBANK[ 4 ] = VROMPAGE( Map33_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa001:
      Map33_Regs[ 5 ] = byData;
      pGData->PPUBANK[ 5 ] = VROMPAGE( Map33_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa002:
      Map33_Regs[ 6 ] = byData;
      pGData->PPUBANK[ 6 ] = VROMPAGE( Map33_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa003:
      Map33_Regs[ 7 ] = byData;
      pGData->PPUBANK[ 7 ] = VROMPAGE( Map33_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xc000:
      Map33_IRQ_Cnt = byData;
      break;

    case 0xc001:
    case 0xc002:
    case 0xe001:
    case 0xe002:
      if ( Map33_IRQ_Cnt == byData )
      {
        Map33_IRQ_Enable = 0xff;
      } else {
        Map33_IRQ_Enable = byData;
      }
      break;

    case 0xe000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring(pGData, 0 );
      } else {
        InfoNES_Mirroring(pGData, 1 );
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map33_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map33_IRQ_Enable )
  {
    if ( Map33_IRQ_Enable == 0xff )
    {
      if ( pGData->PPU_Scanline == (WORD)( 0xff - Map33_IRQ_Cnt ) )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      }
    } else {
      if ( Map33_IRQ_Cnt == 0xff )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      } else {
        Map33_IRQ_Cnt++;
      }
    }
  }
}
