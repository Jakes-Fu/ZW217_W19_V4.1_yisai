/*===================================================================*/
/*                                                                   */
/*               Mapper 67 (Sunsoft Mapper #3)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map67_IRQ_Enable;
BYTE Map67_IRQ_Cnt;
BYTE Map67_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 67                                             */
/*-------------------------------------------------------------------*/
void Map67_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map67_Init;

  /* Write to Mapper */
  MapperWrite = Map67_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map67_HSync;

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
  pGData->PPUBANK[ 0 ] = VROMPAGE( 0 );
  pGData->PPUBANK[ 1 ] = VROMPAGE( 1 );
  pGData->PPUBANK[ 2 ] = VROMPAGE( 2 );
  pGData->PPUBANK[ 3 ] = VROMPAGE( 3 );
  pGData->PPUBANK[ 4 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 4 );
  pGData->PPUBANK[ 5 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 3 );
  pGData->PPUBANK[ 6 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 2 );
  pGData->PPUBANK[ 7 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 1 );
  InfoNES_SetupChr(pGData);

  /* Initialize IRQ Registers */
  Map67_IRQ_Enable = 0;
  Map67_IRQ_Cnt = 0;
  Map67_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map67_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf800 )
  {
    /* Set PPU Banks */
    case 0x8800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 0 ] = VROMPAGE( byData + 0 );
      pGData->PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 2 ] = VROMPAGE( byData + 0 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr(pGData);
      break;

    case 0xa800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 4 ] = VROMPAGE( byData + 0 );
      pGData->PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      pGData->PPUBANK[ 6 ] = VROMPAGE( byData + 0 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr(pGData);
      break;

    case 0xc800:
      Map67_IRQ_Cnt = Map67_IRQ_Latch;
      Map67_IRQ_Latch = byData;
      break;

    case 0xd800:
      Map67_IRQ_Enable = byData & 0x10;
      break;

    case 0xe800:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring(pGData, 1 );
          break;
        case 1:
          InfoNES_Mirroring(pGData, 0 );
          break;
        case 2:
          InfoNES_Mirroring(pGData, 3 );
          break;
        case 3:
          InfoNES_Mirroring(pGData, 2 );
          break;
      }
      break;

    /* Set ROM Banks */
    case 0xf800:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );

      pGData->ROMBANK0 = ROMPAGE( byData + 0 );
      pGData->ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map67_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map67_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ pGData->PPU_Scanline <= 239 )
    {
      if ( pGData->PPU_R1 & R1_SHOW_SCR || pGData->PPU_R1 & R1_SHOW_SP )
      {
        if ( --Map67_IRQ_Cnt == 0xf6 )
        {
          Map67_IRQ_Cnt = Map67_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}
