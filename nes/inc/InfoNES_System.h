/*===================================================================*/
/*                                                                   */
/*  InfoNES_System.h : The function which depends on a system        */
/*                                                                   */
/*  2000/05/29  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_SYSTEM_H_INCLUDED
#define InfoNES_SYSTEM_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#ifndef NES_FREE
#define NES_FREE(x) do { if (x) { NesFreeFromWRE(x); x = NULL;}} while (0)
#endif

#include "InfoNES_Types.h"
//#include "xnes_peer.h"

#ifdef __cplusplus
extern "C" {
#endif 

struct xnes_peer_file_
{
	int FD;
};
extern BYTE *wave_final;

typedef struct xnes_peer_file_  xnes_peer_file;

/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/
extern WORD NesPalette[];

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

void InfoNES_InitPalette(void);

/* Menu screen */
int InfoNES_Menu(void);

/* Read ROM image file */
int InfoNES_ReadRom( const char *pszFileName );

/* Release a memory for ROM */
void InfoNES_ReleaseRom(void);

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame(void);

/* Get a joypad state */
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem );

/* memcpy */
void *InfoNES_MemoryCopy( void *dest, const void *src, int count );

/* memset */
void *InfoNES_MemorySet( void *dest, int c, int count );

/* Print debug message */
void InfoNES_DebugPrint( char *pszMsg );

/* Wait */
void InfoNES_Wait(void);

/* Sound Initialize */
void InfoNES_SoundInit( void );

/* Sound Open */
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate );

/* Sound Close */
void InfoNES_SoundClose( void );

/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5);

/* Print system message */
//void InfoNES_MessageBox( char *pszMsg, ... );
#define InfoNES_MessageBox	xnes_peer_messagebox

void InfoNES_VideoInit ( void );
void InfoNES_VideoClose( void );

int InfoNES_SaveSRAM( const char* szSaveName );
int InfoNES_LoadSRAM( const char* szSaveName );

#ifdef __cplusplus
};
#endif 
#include "xnes_init.h"

#endif /* !InfoNES_SYSTEM_H_INCLUDED */
