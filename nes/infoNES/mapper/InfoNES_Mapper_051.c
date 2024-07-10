/*===================================================================*/
/*                                                                   */
/*                     Mapper 51 : 11-in-1                           */
/*                                                                   */
/*===================================================================*/

int     Map51_Mode, Map51_Bank;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 51                                             */
/*-------------------------------------------------------------------*/
void Map51_Init(Nes_Global_T *pGData)
{
	int nPage; 

  /* Initialize Mapper */
  MapperInit = Map51_Init;

  /* Write to Mapper */
  MapperWrite = Map51_Write;

  /* Write to SRAM */
  MapperSram = Map51_Sram;

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


  /* Set Registers */
  Map51_Bank = 0;
  Map51_Mode = 1;

  /* Set ROM Banks */
  Map51_Set_CPU_Banks(pGData);

  /* Set PPU Banks */
  for( nPage = 0; nPage < 8; ++nPage )
    pGData->PPUBANK[ nPage ] = CRAMPAGE( nPage );
  InfoNES_SetupChr(pGData);

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map51_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  Map51_Bank = (byData & 0x0f) << 2;
  if( 0xC000 <= wAddr && wAddr <= 0xDFFF ) {
    Map51_Mode = (Map51_Mode & 0x01) | ((byData & 0x10) >> 3);
  }
  Map51_Set_CPU_Banks(pGData);
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if( wAddr>=0x6000 ) {
    Map51_Mode = ((byData & 0x10) >> 3) | ((byData & 0x02) >> 1);
    Map51_Set_CPU_Banks(pGData);
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Set_CPU_Banks(Nes_Global_T *pGData)
{
  switch(Map51_Mode) {
  case 0:
    InfoNES_Mirroring(pGData, 1 );
    pGData->SRAMBANK = ROMPAGE((Map51_Bank|0x2c|3) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((Map51_Bank|0x0c|2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((Map51_Bank|0x0c|3) % (NesHeader.byRomSize<<1));
    break;
  case 1:
    InfoNES_Mirroring(pGData, 1 );
    pGData->SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  case 2:
    InfoNES_Mirroring(pGData, 1 );
    pGData->SRAMBANK = ROMPAGE((Map51_Bank|0x2e|3) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK0 = ROMPAGE((Map51_Bank|0x02|0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((Map51_Bank|0x02|1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((Map51_Bank|0x0e|2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((Map51_Bank|0x0e|3) % (NesHeader.byRomSize<<1));
    break;
  case 3:
    InfoNES_Mirroring(pGData, 0 );
    pGData->SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    pGData->ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  }
}

