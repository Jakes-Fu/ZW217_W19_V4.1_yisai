/*===================================================================*/
/*                                                                   */
/*                       Mapper 95 (Namco 1??)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map95_Regs[ 1 ];
DWORD Map95_Prg0, Map95_Prg1;
DWORD Map95_Chr01, Map95_Chr23;
DWORD Map95_Chr4, Map95_Chr5, Map95_Chr6, Map95_Chr7;

#define Map95_Chr_Swap()    ( Map95_Regs[ 0 ] & 0x80 )
#define Map95_Prg_Swap()    ( Map95_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 95                                             */
/*-------------------------------------------------------------------*/
void Map95_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map95_Init;

  /* Write to Mapper */
  MapperWrite = Map95_Write;

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


  /* Initialize State Registers */
  Map95_Regs[ 0 ] = 0;

  /* Set ROM Banks */
  Map95_Prg0 = 0;
  Map95_Prg1 = 1;
  Map95_Set_CPU_Banks(pGData);

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map95_Chr01 = 0;
    Map95_Chr23 = 2;
    Map95_Chr4  = 4;
    Map95_Chr5  = 5;
    Map95_Chr6  = 6;
    Map95_Chr7  = 7;
    Map95_Set_PPU_Banks(pGData);
  } else {
    Map95_Chr01 = Map95_Chr23 = 0;
    Map95_Chr4 = Map95_Chr5 = Map95_Chr6 = Map95_Chr7 = 0;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map95_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map95_Regs[ 0 ] = byData;
      Map95_Set_PPU_Banks(pGData);
      Map95_Set_CPU_Banks(pGData);
      break;

    case 0x8001:
      if ( Map95_Regs[ 0 ] <= 0x05 )
      {
        if ( byData & 0x20 ) 
        {
          InfoNES_Mirroring(pGData, 2 );
        } else {
          InfoNES_Mirroring(pGData, 3 );
        }
        byData &= 0x1f;
      }

      dwBankNum = byData;

      switch ( Map95_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr01 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr23 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr4 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr5 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr6 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr7 = dwBankNum;
            Map95_Set_PPU_Banks(pGData);
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map95_Prg0 = dwBankNum;
          Map95_Set_CPU_Banks(pGData);
          break;

        case 0x07:
          Map95_Prg1 = dwBankNum;
          Map95_Set_CPU_Banks(pGData);
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_CPU_Banks(Nes_Global_T *pGData)
{
  if ( Map95_Prg_Swap() )
  {
    pGData->ROMBANK0 = ROMLASTPAGE( 1 );
    pGData->ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK2 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    pGData->ROMBANK0 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    pGData->ROMBANK2 = ROMLASTPAGE( 1 );
    pGData->ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_PPU_Banks(Nes_Global_T *pGData)
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map95_Chr_Swap() )
    { 
      pGData->PPUBANK[ 0 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    } else {
      pGData->PPUBANK[ 0 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }
}
