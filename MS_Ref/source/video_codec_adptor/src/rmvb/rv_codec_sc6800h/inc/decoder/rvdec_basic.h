
#ifndef _RVDEC_BASIC_H_
#define _RVDEC_BASIC_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "video_common.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

#define BSLICE_SUPPORTED
//#define DBK_SUPPORTED
//#define RV8_SUPPORTED

typedef enum 
{
    INTRAPIC	= 0,
	INTERPIC	= 1,
	TRUEBPIC	= 2,
	FRUPIC  	= 3
}PicCodType_E;

typedef enum {
    MBTYPE_INTRA,			/* 0 */
    MBTYPE_INTRA_16x16,		/* 1 */
    MBTYPE_INTER,			/* 2 */
    MBTYPE_INTER_4V,		/* 3 */
    MBTYPE_FORWARD,			/* 4 */
    MBTYPE_BACKWARD,		/* 5 */
    MBTYPE_SKIPPED,			/* 6 */
    MBTYPE_DIRECT,			/* 7 */
    MBTYPE_INTER_16x8V,		/* 8 */
    MBTYPE_INTER_8x16V,		/* 9  */
    MBTYPE_BIDIR,			/* 10 */
	MBTYPE_INTER_16x16,		/* 11 */
    NUMBER_OF_MBTYPES		/* 12 */
} MB_Type_E;

#define MB_SIZE			16
#define MB_CHROMA_SIZE	8
#define BLOCK_SIZE		8

#define MB_SIZE_X2		32
#define MB_SIZE_X3		48
#define MB_SIZE_X4		64
#define MB_SIZE_X5		80
#define MB_SIZE_X6		96
#define MB_SIZE_X7		112
#define MB_SIZE_X8		128
#define MB_SIZE_X9		144
#define MB_SIZE_X10		160
#define MB_SIZE_X11		176
#define MB_SIZE_X12		192
#define MB_SIZE_X13		208
#define MB_SIZE_X14		224
#define MB_SIZE_X15		240

#define CLIP_TAB_SIZE   1024

#define RV_CTX_CACHE_WIDTH	12
#define RV_Y_EXTEND_SIZE	16
#define RV_UV_EXTEND_SIZE	(RV_Y_EXTEND_SIZE/2)

#define YUV_BUFFER_NUM   3

//for vld
#define MAX_INTRA_QP_REGIONS 5
#define MAX_INTER_QP_REGIONS 7

#define AIC_MODE1_NUM  90
#define AIC_MODE1_SIZE  9
#define AIC_MODE1_BITS  7

#define AIC_MODE2_NUM  20
#define AIC_MODE2_SIZE 81
#define AIC_MODE2_BITS  9

#define NUM_PTYPE_VLCS 7
#define PTYPE_VLC_SIZE 8
#define PTYPE_VLC_BITS 7

#define NUM_BTYPE_VLCS 6
#define BTYPE_VLC_SIZE 7
#define BTYPE_VLC_BITS 6
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_BASIC_H_
