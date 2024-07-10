/******************************************************************************
 ** File Name:      JPG_DCT_Fast.h                                                   *
 ** Author:         Yi.Wang                                                *
 ** Date:           2007/04/28                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define the DCT/IDCT transform function*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2007/04/28     Yi.Wang          Create.                                   *
 ******************************************************************************/
#ifndef JPG_DCT_FAST_H
#define JPG_DCT_FAST_H

#include "jpg_codec.h"

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

#include "sci_types.h"

extern uint8	iclip[];
extern uint8	*pClip_table;

#define DCTSIZE	 8



#define ONE	((int32) 1)
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#define DESCALE(x,n)  RIGHT_SHIFT((x) + (ONE << ((n)-1)), n)

#define IRIGHT_SHIFT(x,shft)		((x) >> (shft))
#define IDESCALE(x,n)  				((int32) IRIGHT_SHIFT(x, n))
#define DEQUANTIZE(coef,quantval) 	(((int32) (coef)) * (quantval))

#define RANGE_MASK  0x3ff //(MAXJSAMPLE * 4 + 3) /* 2 bits wider than legal samples */

#define MAXJSAMPLE	255
#define CENTERJSAMPLE	128


#define CONST_SCALE (ONE << CONST_BITS)

/* Convert a positive real constant to an integer scaled by CONST_SCALE.
 * Caution: some C compilers fail to reduce "FIX(constant)" at compile time,
 * thus causing a lot of useless floating-point operations at run time.
 */

#define FIX(x)	((INT32) ((x) * CONST_SCALE + 0.5))

/* Descale and correctly round an INT32 value that's scaled by N bits.
 * We assume RIGHT_SHIFT rounds towards minus infinity, so adding
 * the fudge factor is correct for either sign of X.
 */



/* Multiply an INT32 variable by an INT32 constant to yield an INT32 result.
 * This macro is used only when the two inputs will actually be no more than
 * 16 bits wide, so that a 16x16->32 bit multiply can be used instead of a
 * full 32x32 multiply.  This provides a useful speedup on many machines.
 * Unfortunately there is no way to specify a 16x16->32 multiply portably
 * in C, but some C compilers will do the right thing if you provide the
 * correct combination of casts.
 */


#ifdef SHORTxSHORT_32		/* may work if 'int' is 32 bits */
#define MULTIPLY16C16(var,const)  (((int16) (var)) * ((int16) (const)))
#endif
#ifdef SHORTxLCONST_32		/* known to work with Microsoft C 6.0 */
#define MULTIPLY16C16(var,const)  (((int16) (var)) * ((int32) (const)))
#endif

#ifndef MULTIPLY16C16		/* default definition */
#define MULTIPLY16C16(var,const)  ((var) * (const))
#endif

/* Same except both inputs are variables. */

#ifdef SHORTxSHORT_32		/* may work if 'int' is 32 bits */
#define MULTIPLY16V16(var1,var2)  (((int16) (var1)) * ((int16) (var2)))
#endif

#ifndef MULTIPLY16V16		/* default definition */
#define MULTIPLY16V16(var1,var2)  ((var1) * (var2))
#endif


void JPEG_ForwardDCT_Fast (uint8 *block, int32 * data);
void JPEG_InverseDCT_Fast (int32 *block, uint8 * pixelData ,uint16* pQuant);


#endif