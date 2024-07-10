/*===================================================================*/
/*                                                                   */
/*                     Mapper 3 (VROM Switch)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 3                                              */
/*-------------------------------------------------------------------*/
void Map3_Init(Nes_Global_T *pGData)
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map3_Init;

  /* Write to Mapper */
  MapperWrite = Map3_Write;

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
  if ( ( NesHeader.byRomSize << 1 ) > 2 )
  {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 1 );
    pGData->ROMBANK2 = ROMPAGE( 2 );
    pGData->ROMBANK3 = ROMPAGE( 3 );    
  } else {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 1 );
    pGData->ROMBANK2 = ROMPAGE( 0 );
    pGData->ROMBANK3 = ROMPAGE( 1 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  /* "DragonQuest" doesn't run if IRQ isn't made to occur in CLI */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 3 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map3_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  DWORD dwBase;

  /* Set PPU Banks */
  byData %= NesHeader.byVRomSize;
  dwBase = ( (DWORD)byData ) << 3;

  pGData->PPUBANK[ 0 ] = VROMPAGE( dwBase + 0 );
  pGData->PPUBANK[ 1 ] = VROMPAGE( dwBase + 1 );
  pGData->PPUBANK[ 2 ] = VROMPAGE( dwBase + 2 );
  pGData->PPUBANK[ 3 ] = VROMPAGE( dwBase + 3 );
  pGData->PPUBANK[ 4 ] = VROMPAGE( dwBase + 4 );
  pGData->PPUBANK[ 5 ] = VROMPAGE( dwBase + 5 );
  pGData->PPUBANK[ 6 ] = VROMPAGE( dwBase + 6 );
  pGData->PPUBANK[ 7 ] = VROMPAGE( dwBase + 7 );

  InfoNES_SetupChr(pGData);
}
