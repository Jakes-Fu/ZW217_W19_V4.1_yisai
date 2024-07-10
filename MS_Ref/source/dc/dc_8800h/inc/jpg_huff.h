/******************************************************************************
** File Name:      jhuff.h                                                   *
** Author:         yi.wang                                                *
** Date:           2007/07/19                                                *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    define the huffman codec operation interface              *
******************************************************************************

  ******************************************************************************
  **                        Edit History                                       *
  ** ------------------------------------------------------------------------- *
  ** DATE           NAME             DESCRIPTION                               *
  ** 2004/07/19     Zhemin.Lin       Create.                                   *
******************************************************************************/
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "sci_types.h"

#define MAX_BITS_SIZE	16
#define AC_SYMBOL_NUM	256
#define DC_SYMBOL_NUM	12
#define HUFF_FIRST_READ 8

#define HUFF_DECODE(result,htbl,slowlabel) \
{   register int nb, look; \
	if (jremain_bit_num < HUFF_FIRST_READ) { \
    JPEG_Fill_Bit_Buffer();\
    if (jremain_bit_num < HUFF_FIRST_READ) { \
	nb = 1; goto slowlabel; }\
	} \
	look = PEEK_BITS(HUFF_FIRST_READ); \
	if ((nb = htbl->look_nbits[look]) != 0) { \
    DROP_BITS(nb); \
    result = htbl->look_sym[look]; \
	} else { \
    nb = HUFF_FIRST_READ+1; \
slowlabel: \
    result=huff_DECODE(htbl,nb);\
} \
}

/*define huffman table*/
typedef struct {
	const uint8 	*bits;				/* bits[k] = # of symbols with codes of */										/* length k bits; bits[0] is unused */
	const uint8 	*huffval;			/* The symbols, in order of incr code length */
	uint16 	ehufco[AC_SYMBOL_NUM+1];	/* code for each symbol */
	uint8  	ehufsi[AC_SYMBOL_NUM+1];	/* length of code for each symbol */
	int32 	offset[MAX_BITS_SIZE+1];	/* smallest code of length k */
	int32 	maxcode[MAX_BITS_SIZE+1];	/* largest code of length k (-1 if none) */
//	uint8 	valptr[MAX_BITS_SIZE+1];	/* huffval[] index of 1st symbol of length k */

	int16 look_nbits[1<<HUFF_FIRST_READ]; /* # bits, or 0 if too long */
	uint8 look_sym[1<<HUFF_FIRST_READ]; /* symbol, or unused */
}HUFF_TBL_T;


/*default vaule for luminance component*/
extern const uint8 lum_dc_bits_default[MAX_BITS_SIZE+1];
extern const uint8 lum_dc_huffvalue_default[DC_SYMBOL_NUM];
extern const uint8 lum_ac_bits_default[MAX_BITS_SIZE+1];
extern const uint8 lum_ac_huffvalue_default[AC_SYMBOL_NUM];
/*default vaule for chroma component*/
extern const uint8 chr_dc_bits_default[MAX_BITS_SIZE+1];
extern const uint8 chr_dc_huffvalue_default[DC_SYMBOL_NUM];
extern const uint8 chr_ac_bits_default[MAX_BITS_SIZE+1];
extern const uint8 chr_ac_huffvalue_default[AC_SYMBOL_NUM];

/*table buffer for luminance component*/
extern uint8 ac_bits[2][17];
extern uint8 ac_huffvalue[2][256];
extern uint8 dc_bits[2][17];
extern uint8 dc_huffvalue[2][256];
extern 	uint8 huffsize[257];
extern  uint16 huffcode[257];

extern 

void InitHuffTblWithDefaultValue(void);
void InitHuffTbl(void);
void FixHuffTbl(HUFF_TBL_T *htbl);
void Vlc_Quant_Block(int16 *block, HUFF_TBL_T *dc_tbl, HUFF_TBL_T *ac_tbl, const uint16 *quant);
BOOLEAN Vld_DQuant_Block(int16 *block, HUFF_TBL_T *dc_tbl, HUFF_TBL_T *ac_tbl, const uint16 *quant);

#endif