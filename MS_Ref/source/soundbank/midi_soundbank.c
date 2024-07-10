/******************************************************************************
 ** File Name:      midi_soundbank.c                                             *
 ** Author:         Kai.Wan                                                   *
 ** DATE:           May. 29, 2010                                             *
 ** Copyright:      2004 - 2010 Spreatrum, Incoporated. All Rights Reserved   *
 ** Description:    This file define soft-decode midi api.                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** May. 29, 2010   Kai.Wan         Create.                                   *
 ******************************************************************************/

#ifdef __arm
__align(8)
#endif

const unsigned char puiSbank[]=

#if	defined(SOUNDBANK_VER_ARM_512KB)

#include "../soundbank/sbank_v14_512.dat"

#elif defined (SOUNDBANK_VER_ARM_2MB)
#if defined CHIP_ENDIAN_LITTLE
#include "../soundbank/gm1_A21_little_endian.dat" 
#else
#include "../soundbank/gm1_A21_big_endian.dat"
#endif
#elif defined (SOUNDBANK_VER_ARM_256KB)
#if defined CHIP_ENDIAN_LITTLE
#include "../soundbank/soundbank_dy_alaw8bit_128k_little.dat"
#else
#include "../soundbank/sbank_dy_8bit.dat"
#endif

#elif defined (SOUNDBANK_VER_ARM_700KB)

#if defined CHIP_ENDIAN_LITTLE
#include "../soundbank/swlsf2pcm16b_22050_alaw8bits_le.dat" 
#else
#include "../soundbank/soundbank_22050_alaw8bits.dat" 
#endif

#elif defined (SOUNDBANK_VER_ARM_300KB)
#if defined CHIP_ENDIAN_LITTLE
#include "../soundbank/soundbank_22050_alaw8bits_dy_300k.dat"
#else
#include "../soundbank/soundbank_22050_alaw8bits_dy.dat"
#endif
#elif defined (SOUNDBANK_VER_ARM_4MB)

#include "../soundbank/diyou_new_soundbank_44100_16bits.dat"

#else

#error	"choose soundbank"

#endif

