/*===================================================================*/
/*                                                                   */
/*                         Mapper 185  (Tecmo)                       */
/*                                                                   */
/*===================================================================*/

//BYTE Map185_Dummy_Chr_Rom[ 0x400 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 185                                            */
/*-------------------------------------------------------------------*/
void Map185_Init(Nes_Global_T *pGData)
{
  int nPage; 
  Map185_Dummy_Chr_Rom = (BYTE *)NesMallocFromWRE(0x400 * sizeof(BYTE));
  if (NULL == Map185_Dummy_Chr_Rom)
  	return;
  /* Initialize Mapper */
  MapperInit = Map185_Init;

  /* Write to Mapper */
  MapperWrite = Map185_Write;

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
  pGData->ROMBANK2 = ROMPAGE( 2 );
  pGData->ROMBANK3 = ROMPAGE( 3 );

  /* Initialize Dummy VROM */
  for( nPage = 0; nPage < 0x400; nPage++ )
  {
    Map185_Dummy_Chr_Rom[ nPage ] = 0xff;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 185 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map185_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  /* Set PPU Banks */ 
  if ( byData & 0x03 )
  {
    pGData->PPUBANK[ 0 ] = VROMPAGE( 0 );
    pGData->PPUBANK[ 1 ] = VROMPAGE( 1 );
    pGData->PPUBANK[ 2 ] = VROMPAGE( 2 );
    pGData->PPUBANK[ 3 ] = VROMPAGE( 3 );
    pGData->PPUBANK[ 4 ] = VROMPAGE( 4 );
    pGData->PPUBANK[ 5 ] = VROMPAGE( 5 );
    pGData->PPUBANK[ 6 ] = VROMPAGE( 6 );
    pGData->PPUBANK[ 7 ] = VROMPAGE( 7 );
    InfoNES_SetupChr(pGData);
  } else {
    pGData->PPUBANK[ 0 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 1 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 2 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 3 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 4 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 5 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 6 ] = Map185_Dummy_Chr_Rom;
    pGData->PPUBANK[ 7 ] = Map185_Dummy_Chr_Rom;
    InfoNES_SetupChr(pGData);
  }
}
