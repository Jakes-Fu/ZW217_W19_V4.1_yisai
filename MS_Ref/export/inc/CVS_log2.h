/*******************************************************************
**  File Name:     CVS_api.c									   *
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
*******************************************************************/

#ifndef CVS_LOG2_H
#define CVS_LOG2_H

#define LOG2_FRACBITS  18
#define ATAN_FRACBITS  15

//#define LOG2_FIXED(x)  int_log2(x)
//#define EXP2_FIXED(x)  int_exp2(x)
#ifdef OLD_FUN
extern int32 invTab32[257];
extern int32 invTab[32*32];
extern int32 xyMap[32*32];
#endif

extern int32 qlog2[1025];
extern int32 invTabF[2049];
int32 fast_log2_cvs(int32 sx);//output has 6 bits precsion

int32 loop_exp2(int32 x);
int32 loop_log2(int32 x);
#define INTEXP(x)		loop_exp2(x)
//#define INTLOG(x)		loop_log2(x)

#if (defined WIN32)||(defined TUREC)
#define INTLOG(x)		int_log2(x)
int32 int_log2(int32 x); //output has 18 bits precsion

int32 div32fs(int32 x,int32 r,int32 s);
#define DIV32S(x,r,s)	div32fs(x,r,s)

int32 CVSI_sum_abs(int32 *X,int32 L,int32 scale);

#else

extern int32 asm_int_log2	(int32 x ,int32 *qlog2);
#define INTLOG(x)		asm_int_log2(x,qlog2)

extern int32 asm_div32fs	(int32 x,int32 r,int32 s,int32* invTabF);
#define DIV32S(x,r,s)	asm_div32fs(x,r,s,invTabF)

#define CVSI_sum_abs asm_CVSI_sum_abs
int32 asm_CVSI_sum_abs(int32 *X,int32 L,int32 scale);
#endif


#endif



