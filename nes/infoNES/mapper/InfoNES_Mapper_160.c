/*===================================================================*/
/*                                                                   */
/*                        Mapper 160 (Pirates)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map160_IRQ_Enable;
BYTE Map160_IRQ_Cnt;
BYTE Map160_IRQ_Latch;
BYTE Map160_Refresh_Type;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 160                                            */
/*-------------------------------------------------------------------*/
void Map160_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map160_Init;

  /* Write to Mapper */
  MapperWrite = Map160_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map160_HSync;

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

  /* Initialize IRQ Registers */
  Map160_IRQ_Enable = 0;
  Map160_IRQ_Cnt = 0;
  Map160_IRQ_Latch = 0;
  Map160_Refresh_Type = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map160_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      pGData->ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      pGData->ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8002:
      pGData->ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9000:
      if ( byData == 0x2b )
      {
        Map160_Refresh_Type = 1;      /* Title */
      }
      else if ( byData == 0xa8 )
      {
        Map160_Refresh_Type = 2;      /* Passwd */
      }
      else if ( byData == 0x1f )
      {
        Map160_Refresh_Type = 3;      /* Game Over */
      }
      else if ( byData == 0x7c )
      {
        Map160_Refresh_Type = 4;      /* Continue */
      }
      else if ( byData == 0x18 )
      {
        Map160_Refresh_Type = 5;      /* Roulette */
      }
      else if ( byData == 0x60 )
      {
        Map160_Refresh_Type = 6;      /* Congratulation */
      }     
      else
      {
        Map160_Refresh_Type = 0;   
      }

      pGData->PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9001:
      pGData->PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9002:
      if ( Map160_Refresh_Type == 2 && byData != 0xe8 )
      {
        Map160_Refresh_Type = 0;     /* Not Passwd */
      }
      pGData->PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9003:
      pGData->PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9004:
      pGData->PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;
  
    case 0x9005:
      pGData->PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9006:
      pGData->PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;

    case 0x9007:
      pGData->PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
      break;      

    case 0xc000:
      Map160_IRQ_Cnt = Map160_IRQ_Latch;
      Map160_IRQ_Enable = Map160_IRQ_Latch;
      break;

    case 0xc001:
      Map160_IRQ_Latch = byData;
      break;

    case 0xc002:
      Map160_IRQ_Enable = 0;
      break;

    case 0xc003:
      Map160_IRQ_Cnt = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map160_HSync(Nes_Global_T *pGData)
{
/*
 *  Callback at HSync
 *
 */
  if ( pGData->PPU_Scanline == 0 || pGData->PPU_Scanline == 239 )
  {
    if ( Map160_Refresh_Type == 1 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else if ( Map160_Refresh_Type == 2 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else if ( Map160_Refresh_Type == 3 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x70 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x71 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x72 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x73 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else if ( Map160_Refresh_Type == 6 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x5c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x5d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }

  if ( pGData->PPU_Scanline == 64 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x6c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x6d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x6e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x6f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }

  if ( pGData->PPU_Scanline == 128 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x68 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x69 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x6a % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x6b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }

  if ( pGData->PPU_Scanline == 160 )
  {
    if ( Map160_Refresh_Type == 6 )
    {
      pGData->PPUBANK[ 0 ] = VROMPAGE( 0x60 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( 0x61 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }

  if ( Map160_IRQ_Enable )
  {
    if ( Map160_IRQ_Cnt == 0xff )
    {
      IRQ_REQ;
      Map160_IRQ_Cnt = 0;
      Map160_IRQ_Enable = 0;
    } else {
      Map160_IRQ_Cnt++;
    }
  }
}
