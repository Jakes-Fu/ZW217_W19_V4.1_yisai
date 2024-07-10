/*===================================================================*/
/*                                                                   */
/*                       Mapper 32 (Irem G-101)                      */
/*                                                                   */
/*===================================================================*/

BYTE Map32_Saved;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 32                                             */
/*-------------------------------------------------------------------*/
void Map32_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map32_Init;

  /* Write to Mapper */
  MapperWrite = Map32_Write;

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

  /* Initialize state flag */
  Map32_Saved = 0x00;

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
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 32 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map32_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf000 )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );

      if ( Map32_Saved & 0x02 ) 
      {
        pGData->ROMBANK2 = ROMPAGE( byData );
      } else {
        pGData->ROMBANK0 = ROMPAGE( byData );
      }
      break;
      
    case 0x9000:
      Map32_Saved = byData;
      
      // Name Table Mirroring
      InfoNES_Mirroring(pGData, byData & 0x01 );
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      pGData->ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xb000:
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 3 );
      pGData->PPUBANK[ wAddr & 0x0007 ] = VROMPAGE( byData );
      InfoNES_SetupChr(pGData);
      break;

    default:
      break;
  }
}
