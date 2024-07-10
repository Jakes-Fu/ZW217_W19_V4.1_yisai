/*===================================================================*/
/*                                                                   */
/*                            Mapper 61                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 61                                             */
/*-------------------------------------------------------------------*/
void Map61_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map61_Init;

  /* Write to Mapper */
  MapperWrite = Map61_Write;

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


  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMLASTPAGE( 1 );
  pGData->ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 61 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map61_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
	BYTE byBank;

	switch( wAddr & 0x30 ) {
		case	0x00:
		case	0x30:
			pGData->ROMBANK0 = ROMPAGE((((wAddr&0x0F)<<2)+0) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK1 = ROMPAGE((((wAddr&0x0F)<<2)+1) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK2 = ROMPAGE((((wAddr&0x0F)<<2)+2) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK3 = ROMPAGE((((wAddr&0x0F)<<2)+3) % (NesHeader.byRomSize<<1));
			break;
		case	0x10:
		case	0x20:
			byBank = ((wAddr & 0x0F)<<1)|((wAddr&0x20)>>4);

			pGData->ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			pGData->ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			break;
	}

	if( wAddr & 0x80 ) InfoNES_Mirroring(pGData, 0 );
	else		   InfoNES_Mirroring(pGData, 1 );
}
