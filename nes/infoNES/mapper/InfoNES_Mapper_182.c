/*===================================================================*/
/*                                                                   */
/*                      Mapper 182  (Pirates)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map182_Regs[1];
BYTE Map182_IRQ_Enable;
BYTE Map182_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 182                                            */
/*-------------------------------------------------------------------*/
void Map182_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map182_Init;

  /* Write to Mapper */
  MapperWrite = Map182_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map182_HSync;

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
    {
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr(pGData);
  }

  /* Initialize IRQ registers */
  Map182_Regs[0] = 0;
  Map182_IRQ_Enable = 0;
  Map182_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map182_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF003 )
  {
    /* Name Table Mirroring */
    case 0x8001:
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring(pGData, 0 );   /* Horizontal */
      }
      else
      {
        InfoNES_Mirroring(pGData, 1 );   /* Vertical */
      }
      break;

    case 0xA000:
      Map182_Regs[0] = byData & 0x07;
      break;

    case 0xC000:
      switch( Map182_Regs[0] )
      { 
        /* Set PPU Banks */ 
        case 0x00:
          pGData->PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          pGData->PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;

        case 0x01:
          pGData->PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;

        case 0x02:
          pGData->PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          pGData->PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;

        case 0x03:
          pGData->PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;

        /* Set ROM Banks */ 
        case 0x04:
          pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        case 0x05:
          pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        /* Set PPU Banks */ 
        case 0x06:
          pGData->PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;

        case 0x07:
          pGData->PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr(pGData);
          break;
      }
      break;

   case 0xE003:
     Map182_IRQ_Cnt = byData;
     Map182_IRQ_Enable = byData;
     break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map182_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map182_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ pGData->PPU_Scanline <= 240 )
    {
      if ( pGData->PPU_R1 & R1_SHOW_SCR || pGData->PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map182_IRQ_Cnt ) )
        {
          Map182_IRQ_Cnt = 0;
          Map182_IRQ_Enable = 0;
          IRQ_REQ;
        }
      }
    }
  }
}
