/*===================================================================*/
/*                                                                   */
/*                   Mapper 48 (Taito TC0190V)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map48_Regs[ 1 ];
BYTE Map48_IRQ_Enable;
BYTE Map48_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 48                                             */
/*-------------------------------------------------------------------*/
void Map48_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map48_Init;

  /* Write to Mapper */
  MapperWrite = Map48_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map48_HSync;

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

  /* Initialize IRQ Registers */
  Map48_Regs[ 0 ] = 0;
  Map48_IRQ_Enable = 0;
  Map48_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map48_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Name Table Mirroring */ 
      if ( !Map48_Regs[ 0 ] )
      {
        if ( byData & 0x40 )
        {
          InfoNES_Mirroring(pGData, 0 );
        } else {
          InfoNES_Mirroring(pGData, 1 );
        }
      }
      /* Set ROM Banks */
      pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      /* Set ROM Banks */
      pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;  
 
    /* Set PPU Banks */
    case 0x8002:
      pGData->PPUBANK[ 0 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x8003:
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa000:
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa001:
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa002:
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa003:
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xc000:
      Map48_IRQ_Cnt = byData;
      break;

    case 0xc001:
      Map48_IRQ_Enable = byData & 0x01;
      break;

    case 0xe000:
      /* Name Table Mirroring */ 
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring(pGData, 0 );
      } else {
        InfoNES_Mirroring(pGData, 1 );
      }
      Map48_Regs[ 0 ] = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map48_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map48_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ pGData->PPU_Scanline <= 239 )
    {
      if ( pGData->PPU_R1 & R1_SHOW_SCR || pGData->PPU_R1 & R1_SHOW_SP )
      {
        if ( Map48_IRQ_Cnt == 0xff )
        {
          IRQ_REQ;
          Map48_IRQ_Enable = 0;
        } else {
          Map48_IRQ_Cnt++;
        }
      }
    }
  }
}
