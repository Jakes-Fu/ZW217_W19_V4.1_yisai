/*===================================================================*/
/*                                                                   */
/*  InfoNES.h : NES Emulator for Win32, Linux(x86), Linux(PS2)       */
/*                                                                   */
/*  2000/05/14  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_H_INCLUDED
#define InfoNES_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/


#include "InfoNES_Types.h"

/*-------------------------------------------------------------------*/
/*  NES resources                                                    */
/*-------------------------------------------------------------------*/

#define RAM_SIZE     0x2000
#define NES_SRAM_SIZE    0x2000
#define PPURAM_SIZE  0x4000
#define SPRRAM_SIZE  256


typedef	struct tag_nesGlobalStruct
{
	/* RAM */
	//BYTE RAM[ RAM_SIZE ];
	BYTE *RAM;

	/* SRAM */
	//BYTE SRAM[ NES_SRAM_SIZE ];
	BYTE *SRAM;

	/* ROM */
	BYTE *ROM;

	/* SRAM BANK ( 8Kb ) */
	BYTE *SRAMBANK;

	/* ROM BANK ( 8Kb * 4 ) */
	BYTE *ROMBANK0;
	BYTE *ROMBANK1;
	BYTE *ROMBANK2;
	BYTE *ROMBANK3;

	/*-------------------------------------------------------------------*/
	/*  PPU resources                                                    */
	/*-------------------------------------------------------------------*/

	/* PPU RAM */
	//BYTE PPURAM[ PPURAM_SIZE ];
	BYTE *PPURAM;

	/* VROM */
	BYTE *VROM;

	/* PPU BANK ( 1Kb * 16 ) */
	BYTE *PPUBANK[ 16 ];

	/* Sprite RAM */
	BYTE SPRRAM[ SPRRAM_SIZE ];

	/* PPU Register */
	BYTE PPU_R0;
	BYTE PPU_R1;
	BYTE PPU_R2;
	BYTE PPU_R3;
	BYTE PPU_R7;

	/* Vertical scroll value */
	BYTE PPU_Scr_V;
	BYTE PPU_Scr_V_Next;
	BYTE PPU_Scr_V_Byte;
	BYTE PPU_Scr_V_Byte_Next;
	BYTE PPU_Scr_V_Bit;
	BYTE PPU_Scr_V_Bit_Next;

	/* Horizontal scroll value */
	BYTE PPU_Scr_H;
	BYTE PPU_Scr_H_Next;
	BYTE PPU_Scr_H_Byte;
	BYTE PPU_Scr_H_Byte_Next;
	BYTE PPU_Scr_H_Bit;
	BYTE PPU_Scr_H_Bit_Next;

	/* PPU Address */
	WORD PPU_Addr;

	/* PPU Address */
	WORD PPU_Temp;

	/* The increase value of the PPU Address */
	WORD PPU_Increment;

	/* Current Scanline */
	WORD PPU_Scanline;

	/* Scanline Table */
	BYTE PPU_ScanTable[ 263 ];

	/* Name Table Bank */
	BYTE PPU_NameTableBank;

	/* BG Base Address */
	BYTE *PPU_BG_Base;

	/* Sprite Base Address */
	BYTE *PPU_SP_Base;

	/* Sprite Height */
	WORD PPU_SP_Height;

	/* Sprite #0 Scanline Hit Position */
	int SpriteJustHit;

	/* VRAM Write Enable ( 0: Disable, 1: Enable ) */
	BYTE byVramWriteEnable;

	/* PPU Address and Scroll Latch Flag*/
	BYTE PPU_Latch_Flag;

	/* Up and Down Clipping Flag ( 0: non-clip, 1: clip ) */ 
	BYTE PPU_UpDown_Clip;

	/* Frame IRQ ( 0: Disabled, 1: Enabled )*/
	BYTE FrameIRQ_Enable;
	WORD FrameStep;

	/*-------------------------------------------------------------------*/
	/*  Display and Others resouces                                      */
	/*-------------------------------------------------------------------*/

	/* Frame Skip */
	WORD FrameSkip;
	WORD FrameCnt;

}Nes_Global_T;

extern Nes_Global_T gNesGlobalData;



#define NAME_TABLE0  8
#define NAME_TABLE1  9
#define NAME_TABLE2  10
#define NAME_TABLE3  11

#define NAME_TABLE_V_MASK 2
#define NAME_TABLE_H_MASK 1


#define SPR_Y    0
#define SPR_CHR  1
#define SPR_ATTR 2
#define SPR_X    3
#define SPR_ATTR_COLOR  0x3
#define SPR_ATTR_V_FLIP 0x80
#define SPR_ATTR_H_FLIP 0x40
#define SPR_ATTR_PRI    0x20


#define R0_NMI_VB      0x80
#define R0_NMI_SP      0x40
#define R0_SP_SIZE     0x20
#define R0_BG_ADDR     0x10
#define R0_SP_ADDR     0x08
#define R0_INC_ADDR    0x04
#define R0_NAME_ADDR   0x03

#define R1_BACKCOLOR   0xe0
#define R1_SHOW_SP     0x10
#define R1_SHOW_SCR    0x08
#define R1_CLIP_SP     0x04
#define R1_CLIP_BG     0x02
#define R1_MONOCHROME  0x01

#define R2_IN_VBLANK   0x80
#define R2_HIT_SP      0x40
#define R2_MAX_SP      0x20
#define R2_WRITE_FLAG  0x10

#define SCAN_TOP_OFF_SCREEN     0
#define SCAN_ON_SCREEN          1
#define SCAN_BOTTOM_OFF_SCREEN  2
#define SCAN_UNKNOWN            3
#define SCAN_VBLANK             4

#define SCAN_TOP_OFF_SCREEN_START       0 
#define SCAN_ON_SCREEN_START            8
#define SCAN_BOTTOM_OFF_SCREEN_START  232
#define SCAN_UNKNOWN_START            240
#define SCAN_VBLANK_START             243
#define SCAN_VBLANK_END               262

#define STEP_PER_SCANLINE             113
#define STEP_PER_FRAME                29828

/* Develop Scroll Registers */
#define InfoNES_SetupScr() \
{ \
  /* V-Scroll Register */ \
  /* PPU_Scr_V_Byte_Next = ( BYTE )( ( PPU_Addr & 0x03e0 ) >> 5 ); */ \
  /* PPU_Scr_V_Bit_Next = ( BYTE )( ( PPU_Addr & 0x7000 ) >> 12 ); */ \
  /* H-Scroll Register */ \
  /* PPU_Scr_H_Byte_Next = ( BYTE )( PPU_Addr & 0x001f ); */ \
  /* NameTableBank */ \
  pGData->PPU_NameTableBank = NAME_TABLE0 + ( ( pGData->PPU_Addr & 0x0C00 ) >> 10 ); \
}



/* NES display size */
#define NES_DISP_WIDTH      256
#define NES_DISP_HEIGHT     240

/*-------------------------------------------------------------------*/
/*  Display and Others resouces                                      */
/*-------------------------------------------------------------------*/

#if 0
extern WORD DoubleFrame[ 2 ][ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
extern WORD *WorkFrame;
extern WORD WorkFrameIdx;
#else
//extern WORD WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
extern WORD *WorkFrame;
#endif

//extern BYTE ChrBuf[];
extern BYTE *ChrBuf;

extern BYTE ChrBufUpdate;

extern WORD PalTable[];

/*-------------------------------------------------------------------*/
/*  APU and Pad resources                                            */
/*-------------------------------------------------------------------*/

extern BYTE APU_Reg[];
extern int APU_Mute;

extern DWORD PAD1_Latch;
extern DWORD PAD2_Latch;
extern DWORD PAD_System;
extern DWORD PAD1_Bit;
extern DWORD PAD2_Bit;

#define PAD_SYS_QUIT   1
#define PAD_SYS_OK     2
#define PAD_SYS_CANCEL 4
#define PAD_SYS_UP     8
#define PAD_SYS_DOWN   0x10
#define PAD_SYS_LEFT   0x20
#define PAD_SYS_RIGHT  0x40

#define PAD_PUSH(a,b)  ( ( (a) & (b) ) != 0 )

/*-------------------------------------------------------------------*/
/*  Mapper Function                                                  */
/*-------------------------------------------------------------------*/

/* Initialize Mapper */
extern void (*MapperInit)(Nes_Global_T *pGData);
/* Write to Mapper */
extern void (*MapperWrite)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Write to SRAM */
extern void (*MapperSram)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Write to APU */
extern void (*MapperApu)(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
/* Read from Apu */
extern BYTE (*MapperReadApu)(Nes_Global_T *pGData, WORD wAddr );
/* Callback at VSync */
extern void (*MapperVSync)(Nes_Global_T *pGData);
/* Callback at HSync */
extern void (*MapperHSync)(Nes_Global_T *pGData);
/* Callback at PPU read/write */
extern void (*MapperPPU)(Nes_Global_T *pGData, WORD wAddr );
/* Callback at Rendering Screen 1:BG, 0:Sprite */
extern void (*MapperRenderScreen)(Nes_Global_T *pGData, BYTE byMode );

/*-------------------------------------------------------------------*/
/*  ROM information                                                  */
/*-------------------------------------------------------------------*/

/* .nes File Header */
struct NesHeader_tag
{
  BYTE byID[ 4 ];
  BYTE byRomSize;
  BYTE byVRomSize;
  BYTE byInfo1;
  BYTE byInfo2;
  BYTE byReserve[ 8 ];
};

/* .nes File Header */
extern struct NesHeader_tag NesHeader;

/* Mapper No. */
extern BYTE MapperNo;

/* Other */
extern BYTE ROM_Mirroring;
extern BYTE ROM_SRAM;
extern BYTE ROM_Trainer;
extern BYTE ROM_FourScr;

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Initialize InfoNES */
int InfoNES_Init(Nes_Global_T *pGData);

/* Completion treatment */
void InfoNES_Fin(void);

/* Load a cassette */
int InfoNES_Load( const char *pszFileName );

/* Reset InfoNES */
int InfoNES_Reset(void);

/* Initialize PPU */
void InfoNES_SetupPPU(Nes_Global_T *pGData);

/* Set up a Mirroring of Name Table */
void InfoNES_Mirroring(Nes_Global_T *pGData, int nType );

/* The main loop of InfoNES */ 
void InfoNES_Main(void);

/* The loop of emulation */
void InfoNES_Cycle(Nes_Global_T *pGData);

/* A function in H-Sync */
int InfoNES_HSync(Nes_Global_T *pGData);

/* Render a scanline */
void InfoNES_DrawLine(Nes_Global_T *pGData);

/* Get a position of scanline hits sprite #0 */
void InfoNES_GetSprHitY(Nes_Global_T *pGData);

/* Develop character data */
void InfoNES_SetupChr(Nes_Global_T *pGData);

#endif /* !InfoNES_H_INCLUDED */

