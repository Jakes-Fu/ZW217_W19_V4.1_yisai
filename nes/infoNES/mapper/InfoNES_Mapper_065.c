/*===================================================================*/
/*                                                                   */
/*                      Mapper 65 (Irem H3001)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map65_IRQ_Enable;
DWORD Map65_IRQ_Cnt;
DWORD Map65_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 65                                             */
/*-------------------------------------------------------------------*/
void Map65_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map65_Init;

  /* Write to Mapper */
  MapperWrite = Map65_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map65_HSync;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 65 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map65_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0x9000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring(pGData, 1 );
      } else {
        InfoNES_Mirroring(pGData, 0 );
      }
      break;

    case 0x9003:
      Map65_IRQ_Enable = byData & 0x80;
      break;

    case 0x9004:
      Map65_IRQ_Cnt = Map65_IRQ_Latch;
      break;

    case 0x9005:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0x00ff ) | ((DWORD)byData << 8 );
      break;

    case 0x9006:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0xff00 ) | (DWORD)byData;
      break;

    /* Set PPU Banks */
    case 0xb000:
      pGData->PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb001:
      pGData->PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb002:
      pGData->PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb003:
      pGData->PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb004:
      pGData->PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb005:
      pGData->PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb006:
      pGData->PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0xb007:
      pGData->PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    /* Set ROM Banks */
    case 0xa000:
      pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0xc000:
      pGData->ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 65 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map65_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( Map65_IRQ_Enable )
  {
    if ( Map65_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map65_IRQ_Enable = 0;
      Map65_IRQ_Cnt = 0xffff;
    } else {
      Map65_IRQ_Cnt -= 113;
    }
  }
}
