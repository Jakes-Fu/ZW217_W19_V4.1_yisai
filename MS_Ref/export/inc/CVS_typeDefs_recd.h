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
#ifndef __CVSF_TYPEDEFS__recd
#define __CVSF_TYPEDEFS__recd

#include <assert.h>
#include <stdio.h>


//#define TUREC   // c
#define INLINE_ASM  // asm 


#define N_BAND_recd
#define W_BAND_recd
#define S_BAND_recd
#define F_BAND_recd

//#define TX_DU

#ifdef F_BAND_recd
#define BD_recd 6
#else
#ifdef S_BAND_recd
#define BD_recd 4
#else
#ifdef W_BAND_recd
#define BD_recd 2
#else
#define BD_recd 1
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
#define MAX_32_recd 0x7fffffffL
#define CVSI_MALLOC_recd(size)						 malloc(size)
#define CVSI_MEMSET_recd(MP3_data, MP3_n, MP3_m)       memset(MP3_data, MP3_n, MP3_m)
#define CVSI_MEMCPY_recd(MP3_des, MP3_src, MP3_m)      memcpy(MP3_des, MP3_src, MP3_m)
#define CVSI_FREE_recd(data)						     free(data)
#define CVSI_MAX_recd(a, b) (((a) > (b)) ? (a) : (b))
#define MAX_LIMIT_recd(x,y) if(x>y)x=y;
#define MIN_LIMIT_recd(x,y) if(x<y)x=y;

	
#define inv32_recd(x) ((x&0xff)<<24)+((x&0xff00)<<8)+((x&0xff0000)>>8)+((x&0xff000000)>>24)
#define inv16_recd(x) ((x&0xff)<<8)+((x&0xff00)>>8)

#define SMULLS_recd(x,w,s) ((int32)(((int64)x*w)>>s))
#define SMULL_recd(x,w) ((int32)(((int64)x*w)>>32))
#define SMULWB_recd(x,w) ( (x>>16) * ((w<<16)>>16) + ( ( (x&0xffff) * ((w<<16)>>16))>>16 ) )
#define SMULWT_recd(x,w) ( (x>>16) * (w>>16      ) + ( ( (x&0xffff) * ( w>>16     ))>>16 ) )
#define SMULBB_recd(x,w) ((x<<16)>>16)*((w<<16)>>16)// xµÕ16 ≥À“‘ wµÕ16
#define SMULBT_recd(x,w) ((x<<16)>>16)*(w>>16)//xµÕ16 ≥À“‘ w∏ﬂ16

#define BASE_N_recd (258*BD_recd)
#define BASE_M_recd (160*BD_recd)
#define BASE_K_recd (48*BD_recd)
#define MAX_SUBS_recd 10
#define WAVSCALE_recd 14
#define SYNSCALE_recd 13
#define CVS_MAX_MODE_NAME_LEN_recd 16

#endif