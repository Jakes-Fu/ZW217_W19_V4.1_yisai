/*******************************************************************
**  File Name:     CVSF_typeDefs.h									   *
**  Author:        Sheng Wu										   *
**  Date:         04/09/2011                                       *
**  Copyright:    2011 Spreadtrum, Incorporated. All Rights Reserved.*
**  Description:   .											   *
********************************************************************
********************************************************************
**  Edit History                                                   *
**-----------------------------------------------------------------*
**  DATE            NAME            DESCRIPTION                    *
**  04/09/2011     Sheng Wu        Create.						   *
**  11/17/2011     Sheng Wu      Modified.						   *
**  05/16/2012     Sheng Wu      Modified.						   *
**  11/13/2012     Sheng Wu      Modified to float point.		   *
*******************************************************************/
#ifndef __CVSF_TYPEDEFS__
#define __CVSF_TYPEDEFS__

#include <assert.h>
#include <stdio.h>
#include <string.h>

//#define TUREC   // c
#define INLINE_ASM  // asm 
#define ADD_NOISE

#define N_BAND
#define W_BAND
//#define S_BAND
//#define F_BAND

#define TX_DU

#ifdef F_BAND
#define BD 6
#else
#ifdef S_BAND
#define BD 4
#else
#ifdef W_BAND
#define BD 2
#else
#define BD 1
#endif
#endif
#endif

#ifdef WIN32
#include <malloc.h>
#include <memory.h>
#endif

//#include "typeDefs.h"
//#include "sci_types.h"

typedef unsigned char			BOOLEAN;
typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned long int		uint32;
typedef unsigned int			uint;

typedef signed char				int8;
typedef signed short			int16;
typedef signed long int			int32;
typedef __int64					int64;


#define LIT_END 

#define MAX_32 0x7fffffffL
#define MIN_32 0x80000000L

#define MAX_16 0x7fff
#define MIN_16 0x8000

#define CVSI_MALLOC(size)						 malloc(size)
#define CVSI_MEMSET(MP3_data, MP3_n, MP3_m)       memset(MP3_data, MP3_n, MP3_m)
#define CVSI_MEMCPY(MP3_des, MP3_src, MP3_m)      memcpy(MP3_des, MP3_src, MP3_m)
#define CVSI_MEMMOVE(des,src,size)				memmove((des), (src), (size))
#define CVSI_FREE(data)						     free(data)
#define CVSI_ASSERT                               assert
#define CVSI_NULL                                 0
#define CVSI_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CVSI_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CVSI_ABS(a)     ((a >= 0)   ? (a) : (-a))

#define ABS(x) if(x<0)x=-x;
#define ABS_SIGN(x, sign) if(x<0){x=-x;sign = 1;}else{sign = 0;}
#define MAX_LIMIT(x,y) if(x>y)x=y;
#define MIN_LIMIT(x,y) if(x<y)x=y;

	
#define inv32(x) ((x&0xff)<<24)+((x&0xff00)<<8)+((x&0xff0000)>>8)+((x&0xff000000)>>24)
#define inv16(x) ((x&0xff)<<8)+((x&0xff00)>>8)

#define SMULLS(x,w,s) ((int32)(((int64)(x)*(w))>>(s)))
#define SMULL(x,w) ((int32)(((int64)(x)*(w))>>32))

#define SMULWB(x,w) ( (x>>16) * ((w<<16)>>16) + ( ( (x&0xffff) * ((w<<16)>>16))>>16 ) )
#define SMULWT(x,w) ( (x>>16) * (w>>16      ) + ( ( (x&0xffff) * ( w>>16     ))>>16 ) )
#define SMULBB(x,w) ((x<<16)>>16)*((w<<16)>>16)// xµÕ16 ≥À“‘ wµÕ16
#define SMULBT(x,w) ((x<<16)>>16)*(w>>16)//xµÕ16 ≥À“‘ w∏ﬂ16
#define SMULTB(x,w) (x>>16)*((w<<16)>>16)
#define SMULTT(x,w) (x>>16)*(w>>16)
#define M32_16 SMULWB
#define SMULLS_SMOOTH_NS(xp, xn, coeff, Qk)  		(int32)(((int64)((1<<(Qk))-(coeff))*(xp) + (int64)(coeff)*(xn))>>(Qk))

#define F2IS(f,s) ((int32)((f)*(1<<(s))))


#define BASE_N (258*BD)
#define BASE_M (160*BD)
#define BASE_K (48*BD)
#define MAX_SUBS 10
#define MAX_TDE 13	//cannot increase under this structure
#define NFRAME 1
/*
#define ANASCALE 13
#define WAVSCALE 12
#define SYNSCALE 11
*/

#define ANASCALE 15
#define WAVSCALE 14
#define SYNSCALE 13

#define GLimit 326
#define GVLimit 326//3000

#define scalalph 16

#define CVS_MAX_MODE_NAME_LEN 16
#define CVS_CONFIG_RESERVE_LEN 128

//20200305 WJI
#define NPSD			(BASE_N>>1)
#define NUM_W			1
#define NUM_B			(2*NUM_W+1)
#define FAC_A			49320 //floor(10/log2(10)*2^14)
#define FAC_B			23637 //floor(log2(exp(1))*2^14)
#define FAC_C			394197467 //floor(1.4685*2^28)
#endif