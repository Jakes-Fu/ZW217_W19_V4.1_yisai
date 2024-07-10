/*===================================================================*/
/*                                                                   */
/*                   Mapper 202 (150-in-1)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 202                                            */
/*-------------------------------------------------------------------*/
void Map202_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map202_Init;

  /* Write to Mapper */
  MapperWrite = Map202_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map202_Apu;

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


  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 12 );
  pGData->ROMBANK1 = ROMPAGE( 13 );
  pGData->ROMBANK2 = ROMPAGE( 14 );
  pGData->ROMBANK3 = ROMPAGE( 15 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map202_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
	Map202_WriteSub(pGData, wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map202_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
	Map202_WriteSub(pGData, wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Sub Function                                    */
/*-------------------------------------------------------------------*/
void Map202_WriteSub(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
	int bank = (wAddr>>1) & 0x07;

	/* Set ROM Banks */
	pGData->ROMBANK0 = ROMPAGE(((bank<<1)+0) % (NesHeader.byRomSize<<1));
	pGData->ROMBANK1 = ROMPAGE(((bank<<1)+1) % (NesHeader.byRomSize<<1));

	if ((wAddr&0x0c) == 0x0c) {
		pGData->ROMBANK2 = ROMPAGE((((bank+1)<<1)+0) % (NesHeader.byRomSize<<1));
		pGData->ROMBANK3 = ROMPAGE((((bank+1)<<1)+1) % (NesHeader.byRomSize<<1));
	} else {		
		pGData->ROMBANK2 = ROMPAGE((((bank+0)<<1)+0) % (NesHeader.byRomSize<<1));
		pGData->ROMBANK3 = ROMPAGE((((bank+0)<<1)+1) % (NesHeader.byRomSize<<1));
	}	

	/* Set PPU Banks */
	pGData->PPUBANK[0] = VROMPAGE(((bank<<3)+0) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[1] = VROMPAGE(((bank<<3)+1) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[2] = VROMPAGE(((bank<<3)+2) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[3] = VROMPAGE(((bank<<3)+3) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[4] = VROMPAGE(((bank<<3)+4) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[5] = VROMPAGE(((bank<<3)+5) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[6] = VROMPAGE(((bank<<3)+6) % (NesHeader.byVRomSize<<3));
	pGData->PPUBANK[7] = VROMPAGE(((bank<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr(pGData);

	if ( wAddr & 0x01 ) {
		InfoNES_Mirroring(pGData, 0 );
	} else {
		InfoNES_Mirroring(pGData, 1 );
	}
}

