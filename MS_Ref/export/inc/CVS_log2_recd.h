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

#ifndef CVS_LOG2_H_recd
#define CVS_LOG2_H_recd

extern int32 qlog2_recd[1025];
extern int32 invTabF_recd[2049];
int32 fast_log2_cvs_recd(int32 sx);//output has 6 bits precsion

int32 loop_exp2_recd(int32 x);
#define INTEXP_recd(x)		loop_exp2_recd(x)


#if (defined WIN32)||(defined TUREC)
#define INTLOG_recd(x)		int_log2_recd(x)
int32 int_log2_recd(int32 x); 

int32 div32fs_recd(int32 x,int32 r,int32 s);
#define DIV32S_recd(x,r,s)	div32fs_recd(x,r,s)
//int32 div32fsd(int32 *x,int32 *y,int32 r,int32 s);
//#define DIV32SD(x,y,r,s)	div32fsd(x,y,r,s)
int32 CVSI_sum_abs_recd(int32 *X,int32 L,int32 scale);

#else

extern int32 asm_int_log2_recd	(int32 x ,int32 *qlog2_recd);
#define INTLOG_recd(x)		asm_int_log2_recd(x,qlog2_recd)

extern int32 asm_div32fs_recd	(int32 x,int32 r,int32 s,int32* invTabF_recd);
#define DIV32S_recd(x,r,s)	asm_div32fs_recd(x,r,s,invTabF_recd)

#define CVSI_sum_abs_recd asm_CVSI_sum_abs_recd
int32 asm_CVSI_sum_abs_recd(int32 *X,int32 L,int32 scale);
#endif

#endif



