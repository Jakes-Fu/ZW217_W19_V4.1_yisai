/*===================================================================*/
/*                                                                   */
/*                        Mapper 41 ()                               */
/*                                                                   */
/*===================================================================*/

BYTE Map41_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 41                                             */
/*-------------------------------------------------------------------*/
void Map41_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map41_Init;

  /* Write to Mapper */
  MapperWrite = Map41_Write;

  /* Write to SRAM */
  MapperSram = Map41_Sram;

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

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
	  int nPage;
    for ( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 41 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map41_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set PPU Banks */
  if ( Map41_Regs[ 0 ] )
  {
    BYTE byChrBank;
    
    byChrBank = Map41_Regs[ 1 ] | ( byData & 0x0003 );
    byChrBank <<= 3;
    byChrBank %= ( NesHeader.byVRomSize << 3 );

    pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank );
    pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
    pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
    pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
    pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
    pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
    pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
    pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );

    InfoNES_SetupChr(pGData);
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 41 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map41_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byBank;

  if ( wAddr < 0x6800 )
  {
    byData = ( BYTE )( wAddr & 0xff );

    /* Set CPU Banks */
    byBank = ( byData & 0x07 ) << 2;
    byBank %= ( NesHeader.byRomSize << 1 );

    pGData->ROMBANK0 = ROMPAGE( byBank );
    pGData->ROMBANK1 = ROMPAGE( byBank + 1 );
    pGData->ROMBANK2 = ROMPAGE( byBank + 2 );
    pGData->ROMBANK3 = ROMPAGE( byBank + 3 );

    Map41_Regs[ 0 ] = ( byData & 0x04 );
    Map41_Regs[ 1 ] = ( byData & 0x18 ) >> 1;

    /* Name Table Mirroring */
    if ( byData & 0x20 )
    {
      InfoNES_Mirroring(pGData, 0 );         /* Horizontal */
    } else {
      InfoNES_Mirroring(pGData, 1 );         /* Vertical */
    }
  }
}
