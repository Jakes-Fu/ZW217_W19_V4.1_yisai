/******************************************************************************
 ** File Name:      jquant.h                                                  *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define DCT quantization interface                         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
#ifndef QUANT_H
#define QUANT_H

extern const uint8 lum_quant_tbl_default[64];
extern const uint8 chr_quant_tbl_default[64];
extern uint8 quant_tbl[2][64];
extern uint16 quant_tbl_new[2][64];


void InitQuantTbl(uint16 level);
void AdjustQuantTbl_Dec();
void AdjustQuantTbl_Enc();
#endif
