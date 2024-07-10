/*===================================================================*/
/*                                                                   */
/*                     Mapper 42 (Pirates)                           */
/*                                                                   */
/*===================================================================*/

BYTE Map42_IRQ_Cnt;
BYTE Map42_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 42                                             */
/*-------------------------------------------------------------------*/
void Map42_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map42_Init;

  /* Write to Mapper */
  MapperWrite = Map42_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map42_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  pGData->SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMLASTPAGE( 3 );
  pGData->ROMBANK1 = ROMLASTPAGE( 2 );
  pGData->ROMBANK2 = ROMLASTPAGE( 1 );
  pGData->ROMBANK3 = ROMLASTPAGE( 0 );

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
/*  Mapper 42 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map42_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xe003 )
  {
    /* Set ROM Banks */
    case 0xe000:
      pGData->SRAMBANK = ROMPAGE( ( byData & 0x0f ) % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xe001:
      if ( byData & 0x08 )
      {
        InfoNES_Mirroring(pGData, 0 );
      } else {
        InfoNES_Mirroring(pGData, 1 );
      }
      break;

    case 0xe002:
      if ( byData & 0x02 )
      {
        Map42_IRQ_Enable = 1;
      } else {
        Map42_IRQ_Enable = 0;
        Map42_IRQ_Cnt = 0;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 42 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map42_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map42_IRQ_Enable )
  {
    if ( Map42_IRQ_Cnt < 215 )
    {
      Map42_IRQ_Cnt++;
    }
    if ( Map42_IRQ_Cnt == 215 )
    {
      IRQ_REQ;
      Map42_IRQ_Enable = 0;
    }
  }
}
