/*===================================================================*/
/*                                                                   */
/*                     Mapper 10 (MMC4)                              */
/*                                                                   */
/*===================================================================*/

struct Map10_Latch 
{
  BYTE lo_bank;
  BYTE hi_bank;
  BYTE state;
};

struct Map10_Latch latch3;    // Latch Selector #1
struct Map10_Latch latch4;    // Latch Selector #2

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 10                                             */
/*-------------------------------------------------------------------*/
void Map10_Init(Nes_Global_T *pGData)
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map10_Init;

  /* Write to Mapper */
  MapperWrite = Map10_Write;

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
  MapperPPU = Map10_PPU;

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
    for ( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Init Latch Selector */
  latch3.state = 0xfe;
  latch3.lo_bank = 0;
  latch3.hi_bank = 0;
  latch4.state = 0xfe;
  latch4.lo_bank = 0;
  latch4.hi_bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map10_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  WORD wMapAddr;

  wMapAddr = wAddr & 0xf000;
  switch ( wMapAddr )
  {
    case 0xa000:
      /* Set ROM Banks */
      byData %= NesHeader.byRomSize;
      byData <<= 1;
      pGData->ROMBANK0 = ROMPAGE( byData );
      pGData->ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    case 0xb000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.lo_bank = byData;

      if (0xfd == latch3.state)
      {
        /* Set PPU Banks */
        pGData->PPUBANK[ 0 ] = VROMPAGE( byData );
        pGData->PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        pGData->PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        pGData->PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr(pGData);
      }
      break;

    case 0xc000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.hi_bank = byData;

      if (0xfe == latch3.state)
      {
        /* Set PPU Banks */
        pGData->PPUBANK[ 0 ] = VROMPAGE( byData );
        pGData->PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        pGData->PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        pGData->PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr(pGData);
      }
      break;

    case 0xd000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.lo_bank = byData;

      if (0xfd == latch4.state)
      {
        /* Set PPU Banks */
        pGData->PPUBANK[ 4 ] = VROMPAGE( byData );
        pGData->PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        pGData->PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        pGData->PPUBANK[ 7 ] = VROMPAGE( byData + 3 );    
        InfoNES_SetupChr(pGData);
      }
      break;

    case 0xe000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.hi_bank = byData;

      if (0xfe == latch4.state)
      {
        /* Set PPU Banks */
        pGData->PPUBANK[ 4 ] = VROMPAGE( byData );
        pGData->PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        pGData->PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        pGData->PPUBANK[ 7 ] = VROMPAGE( byData + 3 ); 
        InfoNES_SetupChr(pGData);
      }
      break;

    case 0xf000:
      /* Name Table Mirroring */
      InfoNES_Mirroring(pGData, byData & 0x01 ? 0 : 1);
      break;
  }  
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 PPU Function                                           */
/*-------------------------------------------------------------------*/
void Map10_PPU(Nes_Global_T *pGData, WORD wAddr )
{
  /* Control Latch Selector */ 
  switch ( wAddr & 0x3ff0 )
  {
    case 0x0fd0:
      /* Latch Control */
      latch3.state = 0xfd;
      /* Set PPU Banks */
      pGData->PPUBANK[ 0 ] = VROMPAGE( latch3.lo_bank );
      pGData->PPUBANK[ 1 ] = VROMPAGE( latch3.lo_bank + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( latch3.lo_bank + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( latch3.lo_bank + 3 );     
      InfoNES_SetupChr(pGData);
      break;

    case 0x0fe0:
      /* Latch Control */
      latch3.state = 0xfe;
      /* Set PPU Banks */
      pGData->PPUBANK[ 0 ] = VROMPAGE( latch3.hi_bank );
      pGData->PPUBANK[ 1 ] = VROMPAGE( latch3.hi_bank + 1 );
      pGData->PPUBANK[ 2 ] = VROMPAGE( latch3.hi_bank + 2 );
      pGData->PPUBANK[ 3 ] = VROMPAGE( latch3.hi_bank + 3 );     
      InfoNES_SetupChr(pGData);      
      break;

    case 0x1fd0:
      /* Latch Control */
      latch4.state = 0xfd;
      /* Set PPU Banks */
      pGData->PPUBANK[ 4 ] = VROMPAGE( latch4.lo_bank );
      pGData->PPUBANK[ 5 ] = VROMPAGE( latch4.lo_bank + 1 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( latch4.lo_bank + 2 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( latch4.lo_bank + 3 );     
      InfoNES_SetupChr(pGData);
      break;      

    case 0x1fe0:
      /* Latch Control */
      latch4.state = 0xfe;
      /* Set PPU Banks */
      pGData->PPUBANK[ 4 ] = VROMPAGE( latch4.hi_bank );
      pGData->PPUBANK[ 5 ] = VROMPAGE( latch4.hi_bank + 1 );
      pGData->PPUBANK[ 6 ] = VROMPAGE( latch4.hi_bank + 2 );
      pGData->PPUBANK[ 7 ] = VROMPAGE( latch4.hi_bank + 3 );     
      InfoNES_SetupChr(pGData);            
      break;
  }
}
