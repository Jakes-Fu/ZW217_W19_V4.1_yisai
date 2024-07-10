/*===================================================================*/
/*                                                                   */
/*  InfoNES.cpp : NES Emulator for Win32, Linux(x86), Linux(PS2)     */
/*                                                                   */
/*  2000/05/18  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------
* File List :
*
* [NES Hardware]
*   InfoNES.cpp
*   InfoNES.h
*   K6502_rw.h
*
* [Mapper function]
*   InfoNES_Mapper.cpp
*   InfoNES_Mapper.h
*
* [The function which depends on a system]
*   InfoNES_System_ooo.cpp (ooo is a system name. win, ...)
*   InfoNES_System.h
*
* [CPU]
*   K6502.cpp
*   K6502.h
*
* [Others]
*   InfoNES_Types.h
*
--------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_Mapper.h"
#include "InfoNES_pAPU.h"
#include "K6502.h"

/*-------------------------------------------------------------------*/
/*  NES resources                                                    */
/*-------------------------------------------------------------------*/

Nes_Global_T gNesGlobalData = {0};



/*-------------------------------------------------------------------*/
/*  Display and Others resouces                                      */
/*-------------------------------------------------------------------*/



/* Display Buffer */
#if 0
WORD DoubleFrame[ 2 ][ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
WORD *WorkFrame;
WORD WorkFrameIdx;
#else
WORD *WorkFrame = NULL;//[ NES_DISP_WIDTH * NES_DISP_HEIGHT ]; //
#endif

/* Character Buffer */
BYTE *ChrBuf = NULL;//[ 256 * 2 * 8 * 8 ];

/* Update flag for ChrBuf */
BYTE ChrBufUpdate;

/* Palette Table */
WORD PalTable[ 32 ];

/* Table for Mirroring */
BYTE PPU_MirrorTable[][ 4 ] =
{
	{ NAME_TABLE0, NAME_TABLE0, NAME_TABLE1, NAME_TABLE1 },
	{ NAME_TABLE0, NAME_TABLE1, NAME_TABLE0, NAME_TABLE1 },
	{ NAME_TABLE1, NAME_TABLE1, NAME_TABLE1, NAME_TABLE1 },
	{ NAME_TABLE0, NAME_TABLE0, NAME_TABLE0, NAME_TABLE0 },
	{ NAME_TABLE0, NAME_TABLE1, NAME_TABLE2, NAME_TABLE3 },
	{ NAME_TABLE0, NAME_TABLE0, NAME_TABLE0, NAME_TABLE1 }
};

/*-------------------------------------------------------------------*/
/*  APU and Pad resources                                            */
/*-------------------------------------------------------------------*/

/* APU Register */
BYTE APU_Reg[ 0x18 ];

/* APU Mute ( 0:OFF, 1:ON ) */
int APU_Mute = 0;

/* Pad data */
DWORD PAD1_Latch;
DWORD PAD2_Latch;
DWORD PAD_System;
DWORD PAD1_Bit;
DWORD PAD2_Bit;

/*-------------------------------------------------------------------*/
/*  Mapper Function                                                  */
/*-------------------------------------------------------------------*/

/* Initialize Mapper */
void (*MapperInit)(Nes_Global_T *pGData);
/* Write to Mapper */
void (*MapperWrite)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Write to SRAM */
void (*MapperSram)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Write to Apu */
void (*MapperApu)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Read from Apu */
BYTE (*MapperReadApu)(Nes_Global_T *pGData, WORD wAddr );
/* Callback at VSync */
void (*MapperVSync)(Nes_Global_T *pGData);
/* Callback at HSync */
void (*MapperHSync)(Nes_Global_T *pGData);
/* Callback at PPU read/write */
void (*MapperPPU)(Nes_Global_T *pGData, WORD wAddr );
/* Callback at Rendering Screen 1:BG, 0:Sprite */
void (*MapperRenderScreen)(Nes_Global_T *pGData, BYTE byMode );

/*-------------------------------------------------------------------*/
/*  ROM information                                                  */
/*-------------------------------------------------------------------*/

/* .nes File Header */
struct NesHeader_tag NesHeader;

/* Mapper Number */
BYTE MapperNo;

/* Mirroring 0:Horizontal 1:Vertical */
BYTE ROM_Mirroring;
/* It has SRAM */
BYTE ROM_SRAM;
/* It has Trainer */
BYTE ROM_Trainer;
/* Four screen VRAM  */
BYTE ROM_FourScr;

/*===================================================================*/
/*                                                                   */
/*                InfoNES_Init() : Initialize InfoNES                */
/*                                                                   */
/*===================================================================*/
int InfoNES_Init(Nes_Global_T *pGData)
{
/*
*  Initialize InfoNES
*
*  Remarks
*    Initialize K6502 and Scanline Table.
	*/
	int nIdx;
	
	// Video INIT
	InfoNES_VideoInit();
	
	// PaletteINIT
	InfoNES_InitPalette();
	
	// Initialize 6502
	if (K6502_Init() < 0)
		return -1;
	
	// Initialize Scanline Table
	for ( nIdx = 0; nIdx < 263; ++nIdx )
	{
		if ( nIdx < SCAN_ON_SCREEN_START )
		{
			pGData->PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
		}
		else
		{
			if ( nIdx < SCAN_BOTTOM_OFF_SCREEN_START )
			{
				pGData->PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
			}
			else
			{
				if ( nIdx < SCAN_UNKNOWN_START )
				{
					pGData->PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
				}
				else
				{
					if ( nIdx < SCAN_VBLANK_START )
						pGData->PPU_ScanTable[ nIdx ] = SCAN_UNKNOWN;
					else
						pGData->PPU_ScanTable[ nIdx ] = SCAN_VBLANK;
				}
			}
		}
	}
	return 0;
}

/*===================================================================*/
/*                                                                   */
/*                InfoNES_Fin() : Completion treatment               */
/*                                                                   */
/*===================================================================*/
void InfoNES_Fin()
{
/*
*  Completion treatment
*
*  Remarks
*    Release resources
	*/
	// Finalize pAPU
	InfoNES_pAPUDone();
	
	// Release a memory for ROM
	InfoNES_ReleaseRom();
	
	InfoNES_VideoClose();
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Load() : Load a cassette                 */
/*                                                                   */
/*===================================================================*/
int InfoNES_Load( const char *pszFileName )
{
/*
*  Load a cassette
*
*  Parameters
*    const char *pszFileName            (Read)
*      File name of ROM image
*
*  Return values
*     0 : It was finished normally.
*    -1 : An error occurred.
*
*  Remarks
*    Read a ROM image in the memory. 
*    Reset InfoNES.
	*/
	
	// Release a memory for ROM
	InfoNES_ReleaseRom();
	
	// Read a ROM image in the memory
	if ( InfoNES_ReadRom( pszFileName ) < 0 )
		return -1;
	
	// Reset InfoNES
	if ( InfoNES_Reset() < 0 )
		return -1;
	
	// Successful
	return 0;
}
/*===================================================================*/
/*                                                                   */
/*                 InfoNES_Reset() : Reset InfoNES                   */
/*                                                                   */
/*===================================================================*/
int InfoNES_Reset()
{
/*
*  Reset InfoNES
*
*  Return values
*     0 : Normally
*    -1 : Non support mapper
*
*  Remarks
*    Initialize Resources, PPU and Mapper.
*    Reset CPU.
	*/
	
	int nIdx;
	Nes_Global_T *pGData = &gNesGlobalData;
	
	/*-------------------------------------------------------------------*/
	/*  Get information on the cassette                                  */
	/*-------------------------------------------------------------------*/
	
	// Get Mapper Number
	MapperNo = NesHeader.byInfo1 >> 4;
	
	// Check bit counts of Mapper No.
	for ( nIdx = 4; nIdx < 8 && NesHeader.byReserve[ nIdx ] == 0; ++nIdx )
		;
	
	if ( nIdx == 8 )
	{
		// Mapper Number is 8bits
		MapperNo |= ( NesHeader.byInfo2 & 0xf0 );
	}
	
	// Get information on the ROM
	ROM_Mirroring = NesHeader.byInfo1 & 1;
	ROM_SRAM = NesHeader.byInfo1 & 2;
	ROM_Trainer = NesHeader.byInfo1 & 4;
	ROM_FourScr = NesHeader.byInfo1 & 8;
	
	/*-------------------------------------------------------------------*/
	/*  Initialize resources                                             */
	/*-------------------------------------------------------------------*/
	
	// Clear RAM
	InfoNES_MemorySet( pGData->RAM, 0, sizeof(BYTE)*RAM_SIZE );
	
	// Reset frame skip and frame count
	pGData->FrameSkip = 0;
	pGData->FrameCnt = 0;
	
#if 0
	// Reset work frame
	WorkFrame = DoubleFrame[ 0 ];
	WorkFrameIdx = 0;
#endif
	
	// Reset update flag of ChrBuf
	ChrBufUpdate = 0xff;
	
	// Reset palette table
	InfoNES_MemorySet( PalTable, 0, sizeof PalTable );
	
	// Reset APU register
	InfoNES_MemorySet( APU_Reg, 0, sizeof APU_Reg );
	
	// Reset joypad
	PAD1_Latch = PAD2_Latch = PAD_System = 0;
	PAD1_Bit = PAD2_Bit = 0;
	
	/*-------------------------------------------------------------------*/
	/*  Initialize PPU                                                   */
	/*-------------------------------------------------------------------*/
	
	InfoNES_SetupPPU(pGData);
	
	/*-------------------------------------------------------------------*/
	/*  Initialize pAPU                                                  */
	/*-------------------------------------------------------------------*/
	
	InfoNES_pAPUInit();
	
	/*-------------------------------------------------------------------*/
	/*  Initialize Mapper                                                */
	/*-------------------------------------------------------------------*/
	
	// Get Mapper Table Index
	for ( nIdx = 0; MapperTable[ nIdx ].nMapperNo != -1; ++nIdx )
	{
		if ( MapperTable[ nIdx ].nMapperNo == MapperNo )
			break;
	}
	
	if ( MapperTable[ nIdx ].nMapperNo == -1 )
	{
		// Non support mapper
		//InfoNES_MessageBox( "Mapper #%d is unsupported.\n", MapperNo );
		return -1;
	}
	
	// Set up a mapper initialization function
	MapperTable[ nIdx ].pMapperInit(pGData);
	
	/*-------------------------------------------------------------------*/
	/*  Reset CPU                                                        */
	/*-------------------------------------------------------------------*/
	
	K6502_Reset(pGData);
	
	// Successful
	return 0;
}

/*===================================================================*/
/*                                                                   */
/*                InfoNES_SetupPPU() : Initialize PPU                */
/*                                                                   */
/*===================================================================*/
void InfoNES_SetupPPU(Nes_Global_T *pGData)
{
/*
*  Initialize PPU
*
	*/
	int nPage;
	
	// Clear PPU and Sprite Memory
	InfoNES_MemorySet( pGData->PPURAM, 0, sizeof(BYTE)*PPURAM_SIZE );
	InfoNES_MemorySet( pGData->SPRRAM, 0, sizeof(BYTE)*SPRRAM_SIZE );
	
	// Reset PPU Register
	pGData->PPU_R0 = pGData->PPU_R1 = pGData->PPU_R2 = pGData->PPU_R3 = pGData->PPU_R7 = 0;
	
	// Reset latch flag
	pGData->PPU_Latch_Flag = 0;
	
	// Reset up and down clipping flag
	pGData->PPU_UpDown_Clip = 0;
	
	pGData->FrameStep = 0;
	pGData->FrameIRQ_Enable = 0;
	
	// Reset Scroll values
	pGData->PPU_Scr_V = pGData->PPU_Scr_V_Next = pGData->PPU_Scr_V_Byte = pGData->PPU_Scr_V_Byte_Next = pGData->PPU_Scr_V_Bit = pGData->PPU_Scr_V_Bit_Next = 0;
	pGData->PPU_Scr_H = pGData->PPU_Scr_H_Next = pGData->PPU_Scr_H_Byte = pGData->PPU_Scr_H_Byte_Next = pGData->PPU_Scr_H_Bit = pGData->PPU_Scr_H_Bit_Next = 0;
	
	// Reset PPU address
	pGData->PPU_Addr = 0;
	pGData->PPU_Temp = 0;
	
	// Reset scanline
	pGData->PPU_Scanline = 0;
	
	// Reset hit position of sprite #0 
	pGData->SpriteJustHit = 0;
	
	// Reset information on PPU_R0
	pGData->PPU_Increment = 1;
	pGData->PPU_NameTableBank = NAME_TABLE0;
	pGData->PPU_BG_Base = ChrBuf;
	pGData->PPU_SP_Base = ChrBuf + 256 * 64;
	pGData->PPU_SP_Height = 8;
	
	// Reset PPU banks
	for ( nPage = 0; nPage < 16; ++nPage )
		pGData->PPUBANK[ nPage ] = &pGData->PPURAM[ nPage * 0x400 ];
	
	/* Mirroring of Name Table */
	InfoNES_Mirroring(pGData, ROM_Mirroring );
	
	/* Reset VRAM Write Enable */
	pGData->byVramWriteEnable = ( NesHeader.byVRomSize == 0 ) ? 1 : 0;
}

/*===================================================================*/
/*                                                                   */
/*       InfoNES_Mirroring() : Set up a Mirroring of Name Table      */
/*                                                                   */
/*===================================================================*/
void InfoNES_Mirroring(Nes_Global_T *pGData, int nType )
{
/*
*  Set up a Mirroring of Name Table
*
*  Parameters
*    int nType          (Read)
*      Mirroring Type
*        0 : Horizontal
*        1 : Vertical
*        2 : One Screen 0x2400
*        3 : One Screen 0x2000
*        4 : Four Screen
*        5 : Special for Mapper #233
	*/
	
	pGData->PPUBANK[ NAME_TABLE0 ] = &pGData->PPURAM[ PPU_MirrorTable[ nType ][ 0 ] * 0x400 ];
	pGData->PPUBANK[ NAME_TABLE1 ] = &pGData->PPURAM[ PPU_MirrorTable[ nType ][ 1 ] * 0x400 ];
	pGData->PPUBANK[ NAME_TABLE2 ] = &pGData->PPURAM[ PPU_MirrorTable[ nType ][ 2 ] * 0x400 ];
	pGData->PPUBANK[ NAME_TABLE3 ] = &pGData->PPURAM[ PPU_MirrorTable[ nType ][ 3 ] * 0x400 ];
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_Main() : The main loop of InfoNES            */
/*                                                                   */
/*===================================================================*/
#if 0
void InfoNES_Main()
{
/*
*  The main loop of InfoNES
*
	*/
	Nes_Global_T *pGData = &gNesGlobalData;
	
	// Initialize InfoNES
	InfoNES_Init(pGData);
	
	// Main loop
	for(;;)
	{
		/*-------------------------------------------------------------------*/
		/*  To the menu screen                                               */
		/*-------------------------------------------------------------------*/
		if ( InfoNES_Menu() < 0 )
			break;  // Quit
		
		/*-------------------------------------------------------------------*/
		/*  Start a NES emulation                                            */
		/*-------------------------------------------------------------------*/
		InfoNES_Cycle(pGData);
	}
	
	// Completion treatment
	InfoNES_Fin();
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_Cycle() : The loop of emulation              */
/*                                                                   */
/*===================================================================*/
void InfoNES_Cycle(Nes_Global_T *pGData)
{
/*
*  The loop of emulation
*
	*/
	
#if 0
	// Set the PPU adress to the buffered value
	if ( ( PPU_R1 & R1_SHOW_SP ) || ( PPU_R1 & R1_SHOW_SCR ) )
		PPU_Addr = PPU_Temp;
#endif
	
	// Emulation loop
	for (;;)
	{    
		WORD nStep;
		
		// Set a flag if a scanning line is a hit in the sprite #0
		if ( pGData->SpriteJustHit == pGData->PPU_Scanline &&
			pGData->PPU_ScanTable[ pGData->PPU_Scanline ] == SCAN_ON_SCREEN )
		{
			// # of Steps to execute before sprite #0 hit
			nStep = pGData->SPRRAM[ SPR_X ] * STEP_PER_SCANLINE / NES_DISP_WIDTH;
			
			// Execute instructions
			K6502_Step(pGData, nStep );
			
			// Set a sprite hit flag
			if ( ( pGData->PPU_R1 & R1_SHOW_SP ) && ( pGData->PPU_R1 & R1_SHOW_SCR ) )
				pGData->PPU_R2 |= R2_HIT_SP;
			
			// NMI is required if there is necessity
			if ( ( pGData->PPU_R0 & R0_NMI_SP ) && ( pGData->PPU_R1 & R1_SHOW_SP ) )
				NMI_REQ;
			
			if (STEP_PER_SCANLINE > nStep)
			{
				// Execute instructions
				K6502_Step(pGData, (WORD)(STEP_PER_SCANLINE - nStep) );
			}
		}
		else
		{
			// Execute instructions
			K6502_Step(pGData, STEP_PER_SCANLINE );
		}
		
		// Frame IRQ in H-Sync
		pGData->FrameStep += STEP_PER_SCANLINE;
		if ( pGData->FrameStep > STEP_PER_FRAME && pGData->FrameIRQ_Enable )
		{
			pGData->FrameStep %= STEP_PER_FRAME;
			IRQ_REQ;
			APU_Reg[ /*0x4015*/0x15 ] |= 0x40;
		}
		
		// A mapper function in H-Sync
		MapperHSync(pGData);
		
		// A function in H-Sync
		if ( InfoNES_HSync(pGData) == -1 )
			return;  // To the menu screen
		
		// HSYNC Wait
		if ( pGData->FrameCnt == 0)
			InfoNES_Wait();
	}
}
#endif

/*===================================================================*/
/*                                                                   */
/*              InfoNES_HSync(Nes_Global_T *pGData) : A function in H-Sync               */
/*                                                                   */
/*===================================================================*/
int InfoNES_HSync(Nes_Global_T *pGData)
{
/*
*  A function in H-Sync
*
*  Return values
*    0 : Normally
*   -1 : Exit an emulation
	*/
	
	/*-------------------------------------------------------------------*/
	/*  Render a scanline                                                */
	/*-------------------------------------------------------------------*/
	if ( pGData->FrameCnt == 0 &&
		pGData->PPU_ScanTable[ pGData->PPU_Scanline ] == SCAN_ON_SCREEN )
	{
		InfoNES_DrawLine(pGData);
	}
	
	/*-------------------------------------------------------------------*/
	/*  Set new scroll values                                            */
	/*-------------------------------------------------------------------*/
	pGData->PPU_Scr_V      = pGData->PPU_Scr_V_Next;
	pGData->PPU_Scr_V_Byte = pGData->PPU_Scr_V_Byte_Next;
	pGData->PPU_Scr_V_Bit  = pGData->PPU_Scr_V_Bit_Next;
	
	pGData->PPU_Scr_H      = pGData->PPU_Scr_H_Next;
	pGData->PPU_Scr_H_Byte = pGData->PPU_Scr_H_Byte_Next;
	pGData->PPU_Scr_H_Bit  = pGData->PPU_Scr_H_Bit_Next;
	
	/*-------------------------------------------------------------------*/
	/*  Next Scanline                                                    */
	/*-------------------------------------------------------------------*/
	pGData->PPU_Scanline = ( pGData->PPU_Scanline == SCAN_VBLANK_END ) ? 0 : pGData->PPU_Scanline + 1;
	
	/*-------------------------------------------------------------------*/
	/*  Operation in the specific scanning line                          */
	/*-------------------------------------------------------------------*/
    switch ( pGData->PPU_Scanline )
	{
	case SCAN_TOP_OFF_SCREEN:
		{
			// Reset a PPU status
			pGData->PPU_R2 = 0;
			
			// Set up a character data
			if ( NesHeader.byVRomSize == 0 && pGData->FrameCnt == 0 )
				InfoNES_SetupChr(pGData);
			
			// Get position of sprite #0
			InfoNES_GetSprHitY(pGData);
			break;
		}
	case SCAN_UNKNOWN_START:
		{
			if ( pGData->FrameCnt == 0 )
			{
				// Transfer the contents of work frame on the screen
				InfoNES_LoadFrame();   
#if 0
				// Switching of the double buffer
				WorkFrameIdx = 1 - WorkFrameIdx;
				WorkFrame = DoubleFrame[ WorkFrameIdx ];
#endif
			}
			break;
		}
	case SCAN_VBLANK_START:
		{
			// FrameCnt + 1
			pGData->FrameCnt = ( pGData->FrameCnt >= pGData->FrameSkip ) ? 0 : pGData->FrameCnt + 1;
			// Set a V-Blank flag
			pGData->PPU_R2 = R2_IN_VBLANK;
			// Reset latch flag
			pGData->PPU_Latch_Flag = 0;
			// pAPU Sound function in V-Sync
			if ( !APU_Mute )
				InfoNES_pAPUVsync(pGData);
			// A mapper function in V-Sync
			MapperVSync(pGData);
			// Get the condition of the joypad
			InfoNES_PadState( &PAD1_Latch, &PAD2_Latch, &PAD_System );
			// NMI on V-Blank
			if ( pGData->PPU_R0 & R0_NMI_VB )
			{
				NMI_REQ;
			}
			
			// Exit an emulation if a QUIT button is pushed
			if ( PAD_PUSH( PAD_System, PAD_SYS_QUIT ) )
			{
				return -1;  // Exit an emulation 
			}
			break;
		}
	default:
		{
			break;
		}
	}
    // Successful
    return 0;
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_DrawLine() : Render a scanline               */
/*                                                                   */
/*===================================================================*/
void InfoNES_DrawLine(Nes_Global_T *pGData)
{
/*
*  Render a scanline
*
	*/
	
	int nX;
	int nY;
	int nY4;
	int nYBit;
	WORD *pPalTbl;
	BYTE *pAttrBase;
	WORD *pPoint;
	int nNameTable;
	BYTE *pbyNameTable;
	BYTE *pbyChrData;
	BYTE *pSPRRAM;
	int nAttr;
	int nSprCnt;
	int nIdx;
	int nSprData;
	BYTE bySprCol;
	BYTE pSprBuf[ NES_DISP_WIDTH + 7 ];
	
	/*-------------------------------------------------------------------*/
	/*  Render Background                                                */
	/*-------------------------------------------------------------------*/
	
	/* MMC5 VROM switch */
	MapperRenderScreen(pGData, 1 );
	
	// Pointer to the render position
	pPoint = &WorkFrame[ pGData->PPU_Scanline * NES_DISP_WIDTH ];
	
	// Clear a scanline if screen is off
	if ( !( pGData->PPU_R1 & R1_SHOW_SCR ) )
	{
		InfoNES_MemorySet( pPoint, 0, NES_DISP_WIDTH << 1 );
	}
	else
	{
		nNameTable = pGData->PPU_NameTableBank;
		
		nY = pGData->PPU_Scr_V_Byte + ( pGData->PPU_Scanline >> 3 );
		
		nYBit = pGData->PPU_Scr_V_Bit + ( pGData->PPU_Scanline & 7 );
		
		if ( nYBit > 7 )
		{
			++nY;
			nYBit &= 7;
		}
		nYBit <<= 3;
		
		if ( nY > 29 )
		{
			// Next NameTable (An up-down direction)
			nNameTable ^= NAME_TABLE_V_MASK;
			nY -= 30;
		}
		
		nX = pGData->PPU_Scr_H_Byte;
		
		nY4 = ( ( nY & 2 ) << 1 );
		
		/*-------------------------------------------------------------------*/
		/*  Rendering of the block of the left end                           */
		/*-------------------------------------------------------------------*/
		
		pbyNameTable = pGData->PPUBANK[ nNameTable ] + nY * 32 + nX;
		pbyChrData = pGData->PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
		pAttrBase = pGData->PPUBANK[ nNameTable ] + 0x3c0 + ( nY / 4 ) * 8;
		pPalTbl =  &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
		
		for ( nIdx = pGData->PPU_Scr_H_Bit; nIdx < 8; ++nIdx )
		{
			*( pPoint++ ) = pPalTbl[ pbyChrData[ nIdx ] ];
		}
		
		// Callback at PPU read/write
		MapperPPU(pGData, (WORD)(PATTBL( pbyChrData )) );
		
		++nX;
		++pbyNameTable;
		
		/*-------------------------------------------------------------------*/
		/*  Rendering of the left table                                      */
		/*-------------------------------------------------------------------*/
		
		for ( ; nX < 32; ++nX )
		{
			pbyChrData = pGData->PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
			pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
			
			pPoint[ 0 ] = pPalTbl[ pbyChrData[ 0 ] ]; 
			pPoint[ 1 ] = pPalTbl[ pbyChrData[ 1 ] ];
			pPoint[ 2 ] = pPalTbl[ pbyChrData[ 2 ] ];
			pPoint[ 3 ] = pPalTbl[ pbyChrData[ 3 ] ];
			pPoint[ 4 ] = pPalTbl[ pbyChrData[ 4 ] ];
			pPoint[ 5 ] = pPalTbl[ pbyChrData[ 5 ] ];
			pPoint[ 6 ] = pPalTbl[ pbyChrData[ 6 ] ];
			pPoint[ 7 ] = pPalTbl[ pbyChrData[ 7 ] ];
			pPoint += 8;
			
			// Callback at PPU read/write
			MapperPPU(pGData, (WORD)(PATTBL( pbyChrData )) );
			
			++pbyNameTable;
		}
		
		// Holizontal Mirror
		nNameTable ^= NAME_TABLE_H_MASK;
		
		pbyNameTable = pGData->PPUBANK[ nNameTable ] + nY * 32;
		pAttrBase = pGData->PPUBANK[ nNameTable ] + 0x3c0 + ( nY / 4 ) * 8;
		
		/*-------------------------------------------------------------------*/
		/*  Rendering of the right table                                     */
		/*-------------------------------------------------------------------*/
		
		for ( nX = 0; nX < pGData->PPU_Scr_H_Byte; ++nX )
		{
			pbyChrData = pGData->PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
			pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
			
			pPoint[ 0 ] = pPalTbl[ pbyChrData[ 0 ] ]; 
			pPoint[ 1 ] = pPalTbl[ pbyChrData[ 1 ] ];
			pPoint[ 2 ] = pPalTbl[ pbyChrData[ 2 ] ];
			pPoint[ 3 ] = pPalTbl[ pbyChrData[ 3 ] ];
			pPoint[ 4 ] = pPalTbl[ pbyChrData[ 4 ] ];
			pPoint[ 5 ] = pPalTbl[ pbyChrData[ 5 ] ];
			pPoint[ 6 ] = pPalTbl[ pbyChrData[ 6 ] ];
			pPoint[ 7 ] = pPalTbl[ pbyChrData[ 7 ] ];
			pPoint += 8;
			
			// Callback at PPU read/write
			MapperPPU(pGData, (WORD)(PATTBL( pbyChrData )));
			
			++pbyNameTable;
		}
		
		/*-------------------------------------------------------------------*/
		/*  Rendering of the block of the right end                          */
		/*-------------------------------------------------------------------*/
		
		pbyChrData = pGData->PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
		pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
		for ( nIdx = 0; nIdx < pGData->PPU_Scr_H_Bit; ++nIdx )
		{
			pPoint[ nIdx ] = pPalTbl[ pbyChrData[ nIdx ] ];
		}
		
		// Callback at PPU read/write
		MapperPPU(pGData, (WORD)(PATTBL( pbyChrData )) );
		
		/*-------------------------------------------------------------------*/
		/*  Backgroud Clipping                                               */
		/*-------------------------------------------------------------------*/
		if ( !( pGData->PPU_R1 & R1_CLIP_BG ) )
		{
			WORD *pPointTop;
			
			pPointTop = &WorkFrame[ pGData->PPU_Scanline * NES_DISP_WIDTH ];
			InfoNES_MemorySet( pPointTop, 0, 8 << 1 );
		}
		
		/*-------------------------------------------------------------------*/
		/*  Clear a scanline if up and down clipping flag is set             */
		/*-------------------------------------------------------------------*/
		if ( pGData->PPU_UpDown_Clip && 
			( SCAN_ON_SCREEN_START > pGData->PPU_Scanline || pGData->PPU_Scanline > SCAN_BOTTOM_OFF_SCREEN_START ) )
		{
			WORD *pPointTop;
			
			pPointTop = &WorkFrame[ pGData->PPU_Scanline * NES_DISP_WIDTH ];
			InfoNES_MemorySet( pPointTop, 0, NES_DISP_WIDTH << 1 );
		}  
  }
  
  /*-------------------------------------------------------------------*/
  /*  Render a sprite                                                  */
  /*-------------------------------------------------------------------*/
  
  /* MMC5 VROM switch */
  MapperRenderScreen(pGData, 0 );
  
  if ( pGData->PPU_R1 & R1_SHOW_SP )
  {
	  // Reset Scanline Sprite Count
	  pGData->PPU_R2 &= ~R2_MAX_SP;
	  
	  // Reset sprite buffer
	  InfoNES_MemorySet( pSprBuf, 0, sizeof pSprBuf );
	  
	  // Render a sprite to the sprite buffer
	  nSprCnt = 0;
	  for ( pSPRRAM = pGData->SPRRAM + ( 63 << 2 ); pSPRRAM >= pGData->SPRRAM; pSPRRAM -= 4 )
	  {
		  nY = pSPRRAM[ SPR_Y ] + 1;
		  if ( nY > pGData->PPU_Scanline || nY + pGData->PPU_SP_Height <= pGData->PPU_Scanline )
			  continue;  // Next sprite
		  
		  /*-------------------------------------------------------------------*/
		  /*  A sprite in scanning line                                        */
		  /*-------------------------------------------------------------------*/
		  
		  // Holizontal Sprite Count +1
		  ++nSprCnt;
		  
		  nAttr = pSPRRAM[ SPR_ATTR ];
		  nYBit = pGData->PPU_Scanline - nY;
		  nYBit = ( nAttr & SPR_ATTR_V_FLIP ) ? ( pGData->PPU_SP_Height - nYBit - 1 ) << 3 : nYBit << 3;
		  
		  if ( pGData->PPU_R0 & R0_SP_SIZE )
		  {
			  // Sprite size 8x16
			  if ( pSPRRAM[ SPR_CHR ] & 1 )
			  {
				  pbyChrData = ChrBuf + 256 * 64 + ( ( pSPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit;
			  }
			  else
			  {
				  pbyChrData = ChrBuf + ( ( pSPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit;
			  }
		  }
		  else
		  {
			  // Sprite size 8x8
			  pbyChrData = pGData->PPU_SP_Base + ( pSPRRAM[ SPR_CHR ] << 6 ) + nYBit;
		  }
		  
		  nAttr ^= SPR_ATTR_PRI;
		  bySprCol = ( nAttr & ( SPR_ATTR_COLOR | SPR_ATTR_PRI ) ) << 2;
		  nX = pSPRRAM[ SPR_X ];
		  
		  if ( nAttr & SPR_ATTR_H_FLIP )
		  {
			  // Horizontal flip
			  if ( pbyChrData[ 7 ] )
				  pSprBuf[ nX ]     = bySprCol | pbyChrData[ 7 ];
			  if ( pbyChrData[ 6 ] )
				  pSprBuf[ nX + 1 ] = bySprCol | pbyChrData[ 6 ];
			  if ( pbyChrData[ 5 ] )
				  pSprBuf[ nX + 2 ] = bySprCol | pbyChrData[ 5 ];
			  if ( pbyChrData[ 4 ] )
				  pSprBuf[ nX + 3 ] = bySprCol | pbyChrData[ 4 ];
			  if ( pbyChrData[ 3 ] )
				  pSprBuf[ nX + 4 ] = bySprCol | pbyChrData[ 3 ];
			  if ( pbyChrData[ 2 ] )
				  pSprBuf[ nX + 5 ] = bySprCol | pbyChrData[ 2 ];
			  if ( pbyChrData[ 1 ] )
				  pSprBuf[ nX + 6 ] = bySprCol | pbyChrData[ 1 ];
			  if ( pbyChrData[ 0 ] )
				  pSprBuf[ nX + 7 ] = bySprCol | pbyChrData[ 0 ];
		  }
		  else
		  {
			  // Non flip
			  if ( pbyChrData[ 0 ] )
				  pSprBuf[ nX ]     = bySprCol | pbyChrData[ 0 ];
			  if ( pbyChrData[ 1 ] )
				  pSprBuf[ nX + 1 ] = bySprCol | pbyChrData[ 1 ];
			  if ( pbyChrData[ 2 ] )
				  pSprBuf[ nX + 2 ] = bySprCol | pbyChrData[ 2 ];
			  if ( pbyChrData[ 3 ] )
				  pSprBuf[ nX + 3 ] = bySprCol | pbyChrData[ 3 ];
			  if ( pbyChrData[ 4 ] )
				  pSprBuf[ nX + 4 ] = bySprCol | pbyChrData[ 4 ];
			  if ( pbyChrData[ 5 ] )
				  pSprBuf[ nX + 5 ] = bySprCol | pbyChrData[ 5 ];
			  if ( pbyChrData[ 6 ] )
				  pSprBuf[ nX + 6 ] = bySprCol | pbyChrData[ 6 ];
			  if ( pbyChrData[ 7 ] )
				  pSprBuf[ nX + 7 ] = bySprCol | pbyChrData[ 7 ];
		  }
	  }
	  
	  // Rendering sprite
	  pPoint -= ( NES_DISP_WIDTH - pGData->PPU_Scr_H_Bit );
	  for ( nX = 0; nX < NES_DISP_WIDTH; ++nX )
	  {
		  nSprData = pSprBuf[ nX ];
		  if ( nSprData  && ( nSprData & 0x80 || pPoint[ nX ] & 0x8000 ) )
		  {
			  pPoint[ nX ] = PalTable[ ( nSprData & 0xf ) + 0x10 ];
		  }
	  }
	  
	  /*-------------------------------------------------------------------*/
	  /*  Sprite Clipping                                                  */
	  /*-------------------------------------------------------------------*/
	  if ( !( pGData->PPU_R1 & R1_CLIP_SP ) )
	  {
		  WORD *pPointTop;
		  
		  pPointTop = &WorkFrame[ pGData->PPU_Scanline * NES_DISP_WIDTH ];
		  InfoNES_MemorySet( pPointTop, 0, 8 << 1 );
	  }
	  
	  if ( nSprCnt >= 8 )
		  pGData->PPU_R2 |= R2_MAX_SP;  // Set a flag of maximum sprites on scanline
    }
}

/*===================================================================*/
/*                                                                   */
/* InfoNES_GetSprHitY() : Get a position of scanline hits sprite #0  */
/*                                                                   */
/*===================================================================*/
void InfoNES_GetSprHitY(Nes_Global_T *pGData)
{
/*
* Get a position of scanline hits sprite #0
*
	*/
	
	int nYBit;
	DWORD *pdwChrData;
	int nOff;
	
	if ( pGData->SPRRAM[ SPR_ATTR ] & SPR_ATTR_V_FLIP )
	{
		// Vertical flip
		nYBit = ( pGData->PPU_SP_Height - 1 ) << 3;
		nOff = -2;
	}
	else
	{
		// Non flip
	    nYBit = 0;
		nOff = 2;
	}
	
	if ( pGData->PPU_R0 & R0_SP_SIZE )
	{
		// Sprite size 8x16
		if ( pGData->SPRRAM[ SPR_CHR ] & 1 )
		{
			pdwChrData = (DWORD *)( ChrBuf + 256 * 64 + ( ( pGData->SPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit );
		}
		else
		{
			pdwChrData = (DWORD * )( ChrBuf + ( ( pGData->SPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit );
		} 
	}
	else
	{
		// Sprite size 8x8
		pdwChrData = (DWORD *)( pGData->PPU_SP_Base + ( pGData->SPRRAM[ SPR_CHR ] << 6 ) + nYBit );
	}
	
	if ( ( pGData->SPRRAM[ SPR_Y ] + 1 <= SCAN_UNKNOWN_START ) && ( pGData->SPRRAM[SPR_Y] > 0 ) )
	{
		int nLine;
		for ( nLine = 0; nLine < pGData->PPU_SP_Height; nLine++ )
		{
			if ( pdwChrData[ 0 ] | pdwChrData[ 1 ] )
			{
				// Scanline hits sprite #0
				pGData->SpriteJustHit = pGData->SPRRAM[SPR_Y] + 1 + nLine;
				nLine = SCAN_VBLANK_END;
			}
			pdwChrData += nOff;
		}
	} 
	else 
	{
		// Scanline didn't hit sprite #0
		pGData->SpriteJustHit = SCAN_UNKNOWN_START + 1;
	}
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_SetupChr() : Develop character data            */
/*                                                                   */
/*===================================================================*/
void InfoNES_SetupChr(Nes_Global_T *pGData)
{
/*
*  Develop character data
*
	*/
	
	BYTE *pbyBGData;
	BYTE byData1;
	BYTE byData2;
	int nIdx;
	int nY;
	int nOff;
	static BYTE *pbyPrevBank[ 8 ] = {0};
	int nBank;
	
	for ( nBank = 0; nBank < 8; ++nBank )
	{
		if ((pbyPrevBank[ nBank ] == pGData->PPUBANK[ nBank ]) && !( ( ChrBufUpdate >> nBank ) & 1 ))
		{
			continue;  // Next bank
		}
		
		/*-------------------------------------------------------------------*/
		/*  An address is different from the last time                       */
		/*    or                                                             */
		/*  An update flag is being set                                      */
		/*-------------------------------------------------------------------*/
		
		for ( nIdx = 0; nIdx < 64; ++nIdx )
		{
			nOff = ( nBank << 12 ) + ( nIdx << 6 );
			
			for ( nY = 0; nY < 8; ++nY )
			{
				pbyBGData = pGData->PPUBANK[ nBank ] + ( nIdx << 4 ) + nY;
				
				byData1 = ( ( pbyBGData[ 0 ] >> 1 ) & 0x55 ) | ( pbyBGData[ 8 ] & 0xAA );
				byData2 = ( pbyBGData[ 0 ] & 0x55 ) | ( ( pbyBGData[ 8 ] << 1 ) & 0xAA );
				
				ChrBuf[ nOff ]     = ( byData1 >> 6 ) & 3;
				ChrBuf[ nOff + 1 ] = ( byData2 >> 6 ) & 3;
				ChrBuf[ nOff + 2 ] = ( byData1 >> 4 ) & 3;
				ChrBuf[ nOff + 3 ] = ( byData2 >> 4 ) & 3;
				ChrBuf[ nOff + 4 ] = ( byData1 >> 2 ) & 3;
				ChrBuf[ nOff + 5 ] = ( byData2 >> 2 ) & 3;
				ChrBuf[ nOff + 6 ] = byData1 & 3;
				ChrBuf[ nOff + 7 ] = byData2 & 3;
				
				nOff += 8;
			}
		}
		// Keep this address
		pbyPrevBank[ nBank ] = pGData->PPUBANK[ nBank ];
	}
	
	// Reset update flag
	ChrBufUpdate = 0;
}


