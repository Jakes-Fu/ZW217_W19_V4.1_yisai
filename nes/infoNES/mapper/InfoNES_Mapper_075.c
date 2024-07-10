/*===================================================================*/
/*                                                                   */
/*           Mapper 75 (Konami VRC 1 and Jaleco SS8805)              */
/*                                                                   */
/*===================================================================*/

BYTE Map75_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 75                                             */
/*-------------------------------------------------------------------*/
void Map75_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map75_Init;

  /* Write to Mapper */
  MapperWrite = Map75_Write;

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
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Initialize State Flag */
  Map75_Regs[ 0 ] = 0;
  Map75_Regs[ 1 ] = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 75 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map75_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf000 )
  {
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x9000:
      /* Set Mirroring */
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring(pGData, 0 );
      } else {
        InfoNES_Mirroring(pGData, 1 );
      }

      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x02 ) << 3 );
      pGData->PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );

      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x04 ) << 2 );
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr(pGData);
      break;

    /* Set ROM Banks */
    case 0xA000:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK1 = ROMPAGE( byData );
      break;

    /* Set ROM Banks */
    case 0xC000:
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK2 = ROMPAGE( byData );
      break;

    case 0xE000:
      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x10 ) | ( byData & 0x0f );
      pGData->PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );
      InfoNES_SetupChr(pGData);
      break;

    case 0xF000:
      /* Set PPU Banks */
      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x10 ) | ( byData & 0x0f );
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr(pGData);
      break;
  }
}
