/*************************************************************************/
/*
* Copyright (c) 2006 The libXnes Project.  All rights reserved.
*
**************************************************************************
*
* FILE NAME    : /src/infoNES/InfoNES_Palette.cpp
*
* CREATED      : 2011-07-29  22:33:51
*
* AUTHOR       : LL&CFA
*
**************************************************************************/
#include "InfoNES_System.h"


WORD NesPalette[ 64 ];

unsigned short xnes_peer_map_rgb( unsigned r , unsigned g , unsigned b )
{
	return ((((r)&0xF8) << 8) | (((g)&0xFC) << 3) | ((b) >> 3));	
}

void InfoNES_InitPalette()
{
	int idx = 0;
#define XNES_MAP_RGB( r , g , b )		(NesPalette[idx++] = xnes_peer_map_rgb( r , g , b ))
	
	XNES_MAP_RGB(112, 112, 112 ),  XNES_MAP_RGB( 32,  24, 136 ), XNES_MAP_RGB(  0,   0, 168), XNES_MAP_RGB( 64,   0, 152),
		XNES_MAP_RGB(136,   0, 112 ),  XNES_MAP_RGB(168,   0,  16 ), XNES_MAP_RGB(160,   0,   0), XNES_MAP_RGB(120,   8,   0),
		XNES_MAP_RGB( 64,  40,   0 ),  XNES_MAP_RGB(  0,  64,   0 ), XNES_MAP_RGB(  0,  80,   0), XNES_MAP_RGB(  0,  56,  16),
		XNES_MAP_RGB( 24,  56,  88 ),  XNES_MAP_RGB(  0,   0,   0 ), XNES_MAP_RGB(  0,   0,   0), XNES_MAP_RGB(  0,   0,   0),
		XNES_MAP_RGB(184, 184, 184 ),  XNES_MAP_RGB(  0, 112, 232 ), XNES_MAP_RGB( 32,  56, 232), XNES_MAP_RGB(128,   0, 240),
		XNES_MAP_RGB(184,   0, 184 ),  XNES_MAP_RGB(224,   0,  88 ), XNES_MAP_RGB(216,  40,   0), XNES_MAP_RGB(200,  72,   8),
		XNES_MAP_RGB(136, 112,   0 ),  XNES_MAP_RGB(  0, 144,   0 ), XNES_MAP_RGB(  0, 168,   0), XNES_MAP_RGB(  0, 144,  56),
		XNES_MAP_RGB(  0, 128, 136 ),  XNES_MAP_RGB(  0,   0,   0 ), XNES_MAP_RGB(  0,   0,   0), XNES_MAP_RGB(  0,   0,   0),
		XNES_MAP_RGB(248, 248, 248 ),  XNES_MAP_RGB( 56, 184, 248 ), XNES_MAP_RGB( 88, 144, 248), XNES_MAP_RGB( 64, 136, 248),
		XNES_MAP_RGB(240, 120, 248 ),  XNES_MAP_RGB(248, 112, 176 ), XNES_MAP_RGB(248, 112,  96), XNES_MAP_RGB(248, 152,  56),
		XNES_MAP_RGB(240, 184,  56 ),  XNES_MAP_RGB(128, 208,  16 ), XNES_MAP_RGB( 72, 216,  72), XNES_MAP_RGB(88, 248, 152 ),
		XNES_MAP_RGB(  0, 232, 216 ),  XNES_MAP_RGB(  0,   0,   0 ), XNES_MAP_RGB(  0,   0,   0), XNES_MAP_RGB( 0,   0,   0 ),
		XNES_MAP_RGB(248, 248, 248 ),  XNES_MAP_RGB(168, 224, 248 ), XNES_MAP_RGB(192, 208, 248), XNES_MAP_RGB(208, 200, 248),
		XNES_MAP_RGB(248, 192, 248 ),  XNES_MAP_RGB(248, 192, 216 ), XNES_MAP_RGB(248, 184, 176), XNES_MAP_RGB(248, 216, 168),
		XNES_MAP_RGB(248, 224, 160 ),  XNES_MAP_RGB(224, 248, 160 ), XNES_MAP_RGB(168, 240, 184), XNES_MAP_RGB(176, 248, 200),
		XNES_MAP_RGB(152, 248, 240 ),  XNES_MAP_RGB(  0,   0,   0 ), XNES_MAP_RGB( 0,   0,   0 ), XNES_MAP_RGB( 0,   0,   0 )
		;
}


