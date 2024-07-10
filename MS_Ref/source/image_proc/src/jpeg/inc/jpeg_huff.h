#ifndef _JPEG_HUFF_H_
#define _JPEG_HUFF_H_

#include "sci_types.h"
#include "jpeg_common.h"


/*
#define HUFF_DECODE(result,htbl,slowlabel,pContext) \
{   register int nb, look; \
	if (jremain_bit_num < HUFF_FIRST_READ) { \
    Jpegd_Fill_Bit_Buffer(pContext);\
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
    result=huff_DECODE(pContext,htbl,nb);\
} \
}
*/

/*define huffman table*/
typedef struct {
	const uint8 	*bits;				/* bits[k] = # of symbols with codes of */										/* length k bits; bits[0] is unused */
	const uint8 	*huffval;			/* The symbols, in order of incr code length */
	//uint16 	ehufco[AC_SYMBOL_NUM+1];	/* code for each symbol */
	//uint8  	ehufsi[AC_SYMBOL_NUM+1];	/* length of code for each symbol */
	//int32 	offset[MAX_BITS_SIZE+1];	/* smallest code of length k */
	//int32 	maxcode[MAX_BITS_SIZE+1];	/* largest code of length k (-1 if none) */
	uint16 	*ehufco;	/* code for each symbol */
	uint8  	*ehufsi;	/* length of code for each symbol */
	int32 	*offset;	/* smallest code of length k */
	int32 	*maxcode;	/* largest code of length k (-1 if none) */

    //needed only when decoding
	//int16 look_nbits[1<<HUFF_FIRST_READ]; /* # bits, or 0 if too long */
	//uint8 look_sym[1<<HUFF_FIRST_READ]; /* symbol, or unused */
	int16 *look_nbits; /* # bits, or 0 if too long */
	uint8 *look_sym; /* symbol, or unused */
}HUFF_TBL_T;



#endif