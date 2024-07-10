/*************************************************************************/
/*
* Copyright (c) 2006 The libXnes Project.  All rights reserved.
*
**************************************************************************
*
* FILE NAME    : /src/infoNES/InfoNES_API.cpp
*
* CREATED      : 2011-07-31  01:55:48
*
* AUTHOR       : LL&CFA
*
**************************************************************************/
#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_Mapper.h"
#include "os_api.h"
//#include "InfoNES_pAPU.h"
#include "InfoNES_fpic.h"
#include "K6502.h"
#include "xnes_api.h"
#include "xnes_init.h"

static char  *szSRAMPath = NULL;//[260] = {0}; 

int  xnes_initialize( const char* rom )
{
	char  szRomPath[260]  = {0};
	Nes_Global_T *pGData = &gNesGlobalData;
	gNesGlobalData.RAM = (BYTE *)NesMallocFromWRE(RAM_SIZE * sizeof(BYTE));
	gNesGlobalData.SRAM = (BYTE *)NesMallocFromWRE(NES_SRAM_SIZE * sizeof(BYTE));
	gNesGlobalData.PPURAM = (BYTE *)NesMallocFromWRE(PPURAM_SIZE * sizeof(BYTE));
	if ((NULL == gNesGlobalData.RAM) || (NULL == gNesGlobalData.SRAM) || (NULL == gNesGlobalData.PPURAM))
	{
		return -1;
	}
	
	WorkFrame = (WORD *)NesMallocFromWRE( NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(WORD));
	if (WorkFrame == NULL)
	{
		return -1;
	}
	
	ChrBuf = (BYTE *)NesMallocFromWRE( 256 * 2 * 8 * 8 * sizeof(BYTE));
	if (ChrBuf == NULL)
	{
		return -1;
	}

	szSRAMPath = (char *)NesMallocFromWRE( 260 * sizeof(char));
	if (szSRAMPath == NULL)
	{
		return -1;
	}

	wave_final = (BYTE *)NesMallocFromWRE( 1024 * sizeof(BYTE));
	if (wave_final == NULL)
	{
		return -1;
	}

	if (InfoNES_pAPU_fpic_init() < 0 )
	{
		return -1;
	}
	
	if (InfoNES_Mapper_fpic_init() < 0 )
	{
		return -1;
	}
	
	if( InfoNES_Load( rom ) < 0 )
		return -1;

	strncpy( szRomPath  , rom , (sizeof( szRomPath ) - 10 )); 
	strcpy ( szSRAMPath , szRomPath );
	strcat ( szSRAMPath , ".srm" );

	InfoNES_LoadSRAM( szSRAMPath );
	
	if( InfoNES_Init(pGData) < 0 )
		return -1;
	
	K6502_init_mem_map(pGData);
	
	return 0;
}

void xnes_finalize  ( void )
{
	InfoNES_SaveSRAM( szSRAMPath );
	InfoNES_Fin();
	InfoNES_Mapper_fpic_finalize();
	InfoNES_pAPU_fpic_finalize ();   /*lint !e746*/
	NES_FREE(WorkFrame);
	NES_FREE(ChrBuf);
	NES_FREE(gNesGlobalData.RAM);
	NES_FREE(gNesGlobalData.SRAM);
	NES_FREE(gNesGlobalData.PPURAM);
	NES_FREE(szSRAMPath);
	NES_FREE(wave_final);

	k6502_finalize();
}

int xnes_next_frame( void )
{
	Nes_Global_T *pGData = &gNesGlobalData;
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
		{
			return -1;  // To the menu screen
		}
		
		if( 0 == pGData->PPU_Scanline )
		{
			return 0;
		}
	}
}

void xnes_set_frame_skip( int skip )
{
	Nes_Global_T *pGData = &gNesGlobalData;
	pGData->FrameSkip = (WORD)skip;
}

int xnes_get_frame_flush_cnt( void )
{
	Nes_Global_T *pGData = &gNesGlobalData;
	return pGData->FrameCnt==1?1:0;
}

void xnes_set_sound( int on )
{
	APU_Mute = !on;
	
	g_nes_ctx.xnes_peer_set_sound( on );
}

void xnes_reset( void )
{
	InfoNES_Reset();
}

