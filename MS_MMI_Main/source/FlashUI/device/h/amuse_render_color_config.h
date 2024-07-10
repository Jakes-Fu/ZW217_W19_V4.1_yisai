/*
*  amuse_render_color_config.h -- config platform color
*  Copyright (C) 2005~2011 Microrapid, Inc. All Rights Reserved.
*  Authors: Microrapid AMUSE GROUP
*  This file is part of Amuse(TM).
*  Date:
*  $Id$
*/
#ifndef AMUSE_RENDER_COLOR_CONFIG_H
#define AMUSE_RENDER_COLOR_CONFIG_H

#if USE_BPP==4
//from high to low byte sequence
#define ARGB  0
#define ABGR  1
#define RGBA  2
#define BGRA  3
#define RGBE  4 // trident
// modify here for your platform
#define COLOR_TYPE  ARGB

#endif
// modify here for your platform 
#if COLOR_TYPE==ARGB
#define SCREEN_ALPHA  0xFF000000  // append to color for alpha
#define SCREEN_ALPHA_32  0xFF000000  // append to color when 16bits color convert to 32 bits
#endif

#if (COLOR_TYPE== 4)//RGBE)
#define SCREEN_ALPHA 0xFE
#define SCREEN_ALPHA_32  SCREEN_ALPHA
#define allocColor32_alpha(x) ((x)->red)<<24 | ((x)->green)<<16 | ((x)->blue)<<8|((x)->alpha)
#define allocColor32(x) (((x)->red)<<24 | ((x)->green)<<16 | ((x)->blue)<<8|SCREEN_ALPHA)  //SCREEN_ALPHA
#define allocColorAlpha32(red,green,blue,alpha) ((alpha)&0xFE|(red)<<24 | (green)<<16 | (blue)<<8)
#define unpark_16to32(x) ((x&0xF800)<<16)|((x&0x7E0)<<13)|((x&0x1F)<<11)|SCREEN_ALPHA_32 //ARGB
#define clear_alpha(x)  (x&0xFFFFFF00)
#define Add_Alpha(x,alpha) ((x&0xFFFFFF00)|(alpha&0xFE))

#define allocColor16(x) (((x)->red >> 3)<<11 | ((x)->green>>2)<<5 | ((x)->blue>>3))
#define allocColor16_alpha(x) (((x)->red >> 3)<<19 | ((x)->green>>2)<<13 | ((x)->blue>>3)<<8| ((x)->alpha &0xFE))

#define GetAlpha(color) ((color&0xFF)|0x1)

#endif //(COLOR_TYPE==RGBE)

#endif
