/*===================================================================*/
/*                                                                   */
/*                    Mapper 113 (PC-Sachen/Hacker)                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 113                                            */
/*-------------------------------------------------------------------*/
void Map113_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map113_Init;

  /* Write to Mapper */
  MapperWrite = Map113_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map113_Apu;

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
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map113_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x4100:
    case 0x4111:
    case 0x4120:
    case 0x4900:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      pGData->ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      pGData->ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      pGData->ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      pGData->ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr(pGData);
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map113_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  BYTE byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x8008:
    case 0x8009:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      pGData->ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      pGData->ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      pGData->ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      pGData->ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      pGData->PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr(pGData);
      break;
  }
}
