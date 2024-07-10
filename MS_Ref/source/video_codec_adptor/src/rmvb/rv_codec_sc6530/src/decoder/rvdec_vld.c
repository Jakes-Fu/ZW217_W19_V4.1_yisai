
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_video_header.h"
/*lint -save -e778 */
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

PUBLIC uint32 RvDec_GetVlcBits(BitStream *stream, uint32 *pInfo, uint32 *pLen)
{
	uint32 uInfo = 0, uLen = 0;
	uint32 uTmpVar;

	uTmpVar = RvDec_ShowBits(stream, 32);

	if (uTmpVar & 0xaa800000)
	{
		uTmpVar >>= (32-8);
		uLen = g_interleaved_golomb_vlc[uTmpVar][1];
		uTmpVar = g_interleaved_golomb_vlc[uTmpVar][0];	
		uInfo = (uTmpVar+1)-(1 << (uLen >>1));
	}else
	{
		uint32 uLenAndInfo;
		uint32 uThisChunksLen;
		uint32 i = 0, ubits;
			
		do
		{
			ubits = (uTmpVar >> (24-i*8)) & 0xff;
			uLenAndInfo = gs_VLCDecodeTable[ubits];
			uThisChunksLen = uLenAndInfo >> 4;

			uInfo = (uInfo << (uThisChunksLen >> 1)) | (uLenAndInfo & 0xf);
			uLen += uThisChunksLen;

			i++;
		}while(!(uLen & 1));
		
		uTmpVar = (1 << (uLen >>1)) + uInfo - 1;
	}
	
	RvDec_FlushBits(stream, uLen);

	*pInfo  = uInfo;
	*pLen = uLen;

	return uTmpVar;
}

/**
 * Get one coefficient value from the bistream and store it.
 */
#define decode_coeff(esc)	\
{\
	if (coeffs == esc)\
	{\
		coeffs = RvDec_HuffDecode(stream, tbl/*, 9, 2*/);\
		if (coeffs > 23)	\
		{\
			int32 tmpVar;\
			coeffs -= 23;\
			tmpVar = RvDec_ReadBits(stream, coeffs);\
			coeffs = (22 + ((1<<coeffs)|tmpVar)); \
		}\
		coeffs += esc;\
	}\
	if (RvDec_Read1Bits(stream))	\
	{\
		coeffs = -coeffs;\
	}\
}

/**
 * Decode 2x2 subblock of coefficients.
 */
LOCAL void decode_subblock (int16 *pBuf, int32 code, SHORT_VLC_SYMBOL_T *vlc, int32 qac)
{
	int32 coeffs;
	const uint8 *tmp_ptr = modulo_three_table[code];
	const int16 (*tbl)[2] = vlc->table;
	BitStream * stream = g_rv_decoder_ptr->stream;

	coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(3);	pBuf[0] = (coeffs * qac + 8) >> 4;}
	coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(2);	pBuf[1] = (coeffs * qac + 8) >> 4;}
	coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(2);	pBuf[4] = (coeffs * qac + 8) >> 4;}
	coeffs = *tmp_ptr;		if (coeffs) {decode_coeff(2);	pBuf[5] = (coeffs * qac + 8) >> 4;}
}
/**
 * Decode coefficients for 4x4 block.
 *
 * This is done by filling 2x2 subblocks with decoded coefficients
 * in this order (the same for subblocks and subblock coefficients):
 *  o--o
 *    /
 *   /
 *  o--o
 */

/*
 *  chroma - 1, if chroma block, 0 if luma
 *  qp    - quantization parameter (1..31)
*/
PUBLIC void RvDec_Dec4x4Block(RV_DECODER_T *rv_decoder_ptr, int16 *pBlock, int32 fc, int32 sc, int32 qdc, int32 qac)
{
	RV_VLC_SET_T *vlc = rv_decoder_ptr->cur_vlcs;
	BitStream *stream = rv_decoder_ptr->stream;
	int32 code, pattern;
	
	/* step #1: set all coeffs to 0: */
    //memset(pBlock, 0, 16 * sizeof(pBlock[0]));   /* !!! */
   	((int32 *)pBlock)[0] = 0;    	((int32 *)pBlock)[1] = 0;
   	((int32 *)pBlock)[2] = 0;    	((int32 *)pBlock)[3] = 0;
   	((int32 *)pBlock)[4] = 0;    	((int32 *)pBlock)[5] = 0;
   	((int32 *)pBlock)[6] = 0;    	((int32 *)pBlock)[7] = 0;

	/*decoder 4x4 dsc and first 2x2 dsc*/
	pattern = RvDec_HuffDecode(stream, vlc->first_pattern[fc].table/*, 9, 2*/);

	code = (pattern >> 3);
	if (code)
	{
	//	decode_subblock(pBlock, code, &vlc->coefficient, qac, qdc);
		int32 coeffs;
		const uint8 *tmp_ptr = modulo_three_table[code];
		const int16 (*tbl)[2] = vlc->coefficient.table;

		coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(3);	pBlock[0] = (coeffs * qdc + 8) >> 4;}
		coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(2);	pBlock[1] = (coeffs * qac + 8) >> 4;}
		coeffs = *tmp_ptr++;	if (coeffs) {decode_coeff(2);	pBlock[4] = (coeffs * qac + 8) >> 4;}
		coeffs = *tmp_ptr;		if (coeffs) {decode_coeff(2);	pBlock[5] = (coeffs * qac + 8) >> 4;}
	}
	
	if (pattern & 4)
	{
		code = RvDec_HuffDecode(stream, vlc->second_pattern[sc].table/*, 9, 2*/);
		decode_subblock(pBlock+2, code, &vlc->coefficient, qac);
	}

	if (pattern & 2)
	{
		int16 tmp;
		
		code = RvDec_HuffDecode(stream, vlc->second_pattern[sc].table/*, 9, 2*/);
		decode_subblock(pBlock+8, code, &vlc->coefficient, qac);
		tmp = pBlock[9];	pBlock[9] = pBlock[12]; pBlock[12] = tmp;
	}

	if (pattern & 1)
	{
		code = RvDec_HuffDecode(stream, vlc->third_pattern[sc].table/*, 9, 2*/);
		decode_subblock(pBlock+10, code, &vlc->coefficient, qac);
	}
}

PUBLIC void RvDec_InitVldTable(void)
{
	RV_VLC_SET_T *intra_vlcs = g_intra_decode_tables;
	RV_VLC_SET_T *inter_vlcs = g_inter_decode_tables;

	#define MAKE_INTRA_CBPPATTERN(i,j)		{intra_vlcs[i].cbppattern[j].table = g_intra_cbppattern_tbl_##i##_##j;}
	#define MAKE_INTER_CBPPATTERN(i)		{inter_vlcs[i].cbppattern[0].table = g_inter_cbppattern_tbl_##i;}
	#define MAKE_SECOND_PATTERN(type,i,j)	{type##_vlcs[i].second_pattern[j].table = g_##type##_second_pattern_tbl_##i##_##j;}
	#define MAKE_THIRD_PATTERN(type,i,j)	{type##_vlcs[i].third_pattern[j].table = g_##type##_third_pattern_tbl_##i##_##j;}
	#define MAKE_INTRA_CPB_TBL(i,j,k)		{intra_vlcs[i].cbp[j][k].table = g_intra_cbp_tbl_##i##_##j##_##k;}
	#define MAKE_INTRA_CPB_BITS(i,j,k,bit)	{intra_vlcs[i].cbp[j][k].bits = bit;}
	#define MAKE_INTER_CPB_TBL(i,j)			{inter_vlcs[i].cbp[0][j].table = g_inter_cbp_tbl_##i##_##j;}
	#define MAKE_INTER_CPB_BITS(i,j,bit)	{inter_vlcs[i].cbp[0][j].bits = bit;}
	#define MAKE_FIRST_PATTERN(type,i,j)	{type##_vlcs[i].first_pattern[j].table = g_##type##_first_pattern_tbl_##i##_##j;}
	#define MAKE_COEFFICIENT(type,i)		{type##_vlcs[i].coefficient.table = g_##type##_coefficient_tbl_##i;}
	#define MAKE_PBTYPE_TBL(pb,i)			{g_##pb##type_vlc[i].table = g_##pb##type_vlc_tbl_##i;}
	#define MAKE_AIC_TBL(idx,i)				{g_aic_mode##idx##_vlc[i].table = g_aic_mode##idx##_vlc_tbl_##i;}

	//cbppattern
	MAKE_INTRA_CBPPATTERN(0, 0);	MAKE_INTRA_CBPPATTERN(0, 1);
	MAKE_INTRA_CBPPATTERN(1, 0);	MAKE_INTRA_CBPPATTERN(1, 1);
	MAKE_INTRA_CBPPATTERN(2, 0);	MAKE_INTRA_CBPPATTERN(2, 1);
	MAKE_INTRA_CBPPATTERN(3, 0);	MAKE_INTRA_CBPPATTERN(3, 1);
	MAKE_INTRA_CBPPATTERN(4, 0);	MAKE_INTRA_CBPPATTERN(4, 1);
	
	//second_pattern
	MAKE_SECOND_PATTERN(intra, 0, 0);	MAKE_SECOND_PATTERN(intra, 0, 1);
	MAKE_SECOND_PATTERN(intra, 1, 0);	MAKE_SECOND_PATTERN(intra, 1, 1);
	MAKE_SECOND_PATTERN(intra, 2, 0);	MAKE_SECOND_PATTERN(intra, 2, 1);
	MAKE_SECOND_PATTERN(intra, 3, 0);	MAKE_SECOND_PATTERN(intra, 3, 1);
	MAKE_SECOND_PATTERN(intra, 4, 0);	MAKE_SECOND_PATTERN(intra, 4, 1);
	
	//third_pattern
	MAKE_THIRD_PATTERN(intra, 0, 0);	MAKE_THIRD_PATTERN(intra, 0, 1);
	MAKE_THIRD_PATTERN(intra, 1, 0);	MAKE_THIRD_PATTERN(intra, 1, 1);
	MAKE_THIRD_PATTERN(intra, 2, 0);	MAKE_THIRD_PATTERN(intra, 2, 1);
	MAKE_THIRD_PATTERN(intra, 3, 0);	MAKE_THIRD_PATTERN(intra, 3, 1);
	MAKE_THIRD_PATTERN(intra, 4, 0);	MAKE_THIRD_PATTERN(intra, 4, 1);

	//cbp
	MAKE_INTRA_CPB_TBL(0, 0, 0);	MAKE_INTRA_CPB_TBL(0, 0, 1);	MAKE_INTRA_CPB_TBL(0, 0, 2);	MAKE_INTRA_CPB_TBL(0, 0, 3);
	MAKE_INTRA_CPB_TBL(0, 1, 0);	MAKE_INTRA_CPB_TBL(0, 1, 1);	MAKE_INTRA_CPB_TBL(0, 1, 2);	MAKE_INTRA_CPB_TBL(0, 1, 3);
	MAKE_INTRA_CPB_TBL(1, 0, 0);	MAKE_INTRA_CPB_TBL(1, 0, 1);	MAKE_INTRA_CPB_TBL(1, 0, 2);	MAKE_INTRA_CPB_TBL(1, 0, 3);
	MAKE_INTRA_CPB_TBL(1, 1, 0);	MAKE_INTRA_CPB_TBL(1, 1, 1);	MAKE_INTRA_CPB_TBL(1, 1, 2);	MAKE_INTRA_CPB_TBL(1, 1, 3);
	MAKE_INTRA_CPB_TBL(2, 0, 0);	MAKE_INTRA_CPB_TBL(2, 0, 1);	MAKE_INTRA_CPB_TBL(2, 0, 2);	MAKE_INTRA_CPB_TBL(2, 0, 3);
	MAKE_INTRA_CPB_TBL(2, 1, 0);	MAKE_INTRA_CPB_TBL(2, 1, 1);	MAKE_INTRA_CPB_TBL(2, 1, 2);	MAKE_INTRA_CPB_TBL(2, 1, 3);
	MAKE_INTRA_CPB_TBL(3, 0, 0);	MAKE_INTRA_CPB_TBL(3, 0, 1);	MAKE_INTRA_CPB_TBL(3, 0, 2);	MAKE_INTRA_CPB_TBL(3, 0, 3);
	MAKE_INTRA_CPB_TBL(3, 1, 0);	MAKE_INTRA_CPB_TBL(3, 1, 1);	MAKE_INTRA_CPB_TBL(3, 1, 2);	MAKE_INTRA_CPB_TBL(3, 1, 3);
	MAKE_INTRA_CPB_TBL(4, 0, 0);	MAKE_INTRA_CPB_TBL(4, 0, 1);	MAKE_INTRA_CPB_TBL(4, 0, 2);	MAKE_INTRA_CPB_TBL(4, 0, 3);
	MAKE_INTRA_CPB_TBL(4, 1, 0);	MAKE_INTRA_CPB_TBL(4, 1, 1);	MAKE_INTRA_CPB_TBL(4, 1, 2);	MAKE_INTRA_CPB_TBL(4, 1, 3);

	MAKE_INTRA_CPB_BITS(0, 0, 0, 6);	MAKE_INTRA_CPB_BITS(0, 0, 1, 5);	MAKE_INTRA_CPB_BITS(0, 0, 2, 5);	MAKE_INTRA_CPB_BITS(0, 0, 3, 6);
	MAKE_INTRA_CPB_BITS(0, 1, 0, 8);	MAKE_INTRA_CPB_BITS(0, 1, 1, 6);	MAKE_INTRA_CPB_BITS(0, 1, 2, 5);	MAKE_INTRA_CPB_BITS(0, 1, 3, 5);
	MAKE_INTRA_CPB_BITS(1, 0, 0, 5);	MAKE_INTRA_CPB_BITS(1, 0, 1, 5);	MAKE_INTRA_CPB_BITS(1, 0, 2, 5);	MAKE_INTRA_CPB_BITS(1, 0, 3, 6);
	MAKE_INTRA_CPB_BITS(1, 1, 0, 8);	MAKE_INTRA_CPB_BITS(1, 1, 1, 6);	MAKE_INTRA_CPB_BITS(1, 1, 2, 5);	MAKE_INTRA_CPB_BITS(1, 1, 3, 5);
	MAKE_INTRA_CPB_BITS(2, 0, 0, 5);	MAKE_INTRA_CPB_BITS(2, 0, 1, 5);	MAKE_INTRA_CPB_BITS(2, 0, 2, 5);	MAKE_INTRA_CPB_BITS(2, 0, 3, 6);
	MAKE_INTRA_CPB_BITS(2, 1, 0, 8);	MAKE_INTRA_CPB_BITS(2, 1, 1, 6);	MAKE_INTRA_CPB_BITS(2, 1, 2, 5);	MAKE_INTRA_CPB_BITS(2, 1, 3, 6);
	MAKE_INTRA_CPB_BITS(3, 0, 0, 5);	MAKE_INTRA_CPB_BITS(3, 0, 1, 5);	MAKE_INTRA_CPB_BITS(3, 0, 2, 5);	MAKE_INTRA_CPB_BITS(3, 0, 3, 5);
	MAKE_INTRA_CPB_BITS(3, 1, 0, 7);	MAKE_INTRA_CPB_BITS(3, 1, 1, 6);	MAKE_INTRA_CPB_BITS(3, 1, 2, 5);	MAKE_INTRA_CPB_BITS(3, 1, 3, 5);
	MAKE_INTRA_CPB_BITS(4, 0, 0, 7);	MAKE_INTRA_CPB_BITS(4, 0, 1, 5);	MAKE_INTRA_CPB_BITS(4, 0, 2, 5);	MAKE_INTRA_CPB_BITS(4, 0, 3, 5);
	MAKE_INTRA_CPB_BITS(4, 1, 0, 7);	MAKE_INTRA_CPB_BITS(4, 1, 1, 6);	MAKE_INTRA_CPB_BITS(4, 1, 2, 5);	MAKE_INTRA_CPB_BITS(4, 1, 3, 5);

	//first_pattern
	MAKE_FIRST_PATTERN(intra, 0, 0);	MAKE_FIRST_PATTERN(intra, 0, 1);	MAKE_FIRST_PATTERN(intra, 0, 2);	MAKE_FIRST_PATTERN(intra, 0, 3);
	MAKE_FIRST_PATTERN(intra, 1, 0);	MAKE_FIRST_PATTERN(intra, 1, 1);	MAKE_FIRST_PATTERN(intra, 1, 2);	MAKE_FIRST_PATTERN(intra, 1, 3);
	MAKE_FIRST_PATTERN(intra, 2, 0);	MAKE_FIRST_PATTERN(intra, 2, 1);	MAKE_FIRST_PATTERN(intra, 2, 2);	MAKE_FIRST_PATTERN(intra, 2, 3);
	MAKE_FIRST_PATTERN(intra, 3, 0);	MAKE_FIRST_PATTERN(intra, 3, 1);	MAKE_FIRST_PATTERN(intra, 3, 2);	MAKE_FIRST_PATTERN(intra, 3, 3);
	MAKE_FIRST_PATTERN(intra, 4, 0);	MAKE_FIRST_PATTERN(intra, 4, 1);	MAKE_FIRST_PATTERN(intra, 4, 2);	MAKE_FIRST_PATTERN(intra, 4, 3);

	//coeff
	MAKE_COEFFICIENT(intra, 0);	MAKE_COEFFICIENT(intra, 1);
	MAKE_COEFFICIENT(intra, 2);	MAKE_COEFFICIENT(intra, 3);
	MAKE_COEFFICIENT(intra, 4);
	
////////////////////////////////
	//cbppattern
	MAKE_INTER_CBPPATTERN(0);	MAKE_INTER_CBPPATTERN(1);	
	MAKE_INTER_CBPPATTERN(2);	MAKE_INTER_CBPPATTERN(3);
	MAKE_INTER_CBPPATTERN(4);	MAKE_INTER_CBPPATTERN(5);
	MAKE_INTER_CBPPATTERN(6);

	//cbp
	MAKE_INTER_CPB_TBL(0, 0);	MAKE_INTER_CPB_TBL(0, 1);	MAKE_INTER_CPB_TBL(0, 2);	MAKE_INTER_CPB_TBL(0, 3);
	MAKE_INTER_CPB_TBL(1, 0);	MAKE_INTER_CPB_TBL(1, 1);	MAKE_INTER_CPB_TBL(1, 2);	MAKE_INTER_CPB_TBL(1, 3);
	MAKE_INTER_CPB_TBL(2, 0);	MAKE_INTER_CPB_TBL(2, 1);	MAKE_INTER_CPB_TBL(2, 2);	MAKE_INTER_CPB_TBL(2, 3);
	MAKE_INTER_CPB_TBL(3, 0);	MAKE_INTER_CPB_TBL(3, 1);	MAKE_INTER_CPB_TBL(3, 2);	MAKE_INTER_CPB_TBL(3, 3);
	MAKE_INTER_CPB_TBL(4, 0);	MAKE_INTER_CPB_TBL(4, 1);	MAKE_INTER_CPB_TBL(4, 2);	MAKE_INTER_CPB_TBL(4, 3);
	MAKE_INTER_CPB_TBL(5, 0);	MAKE_INTER_CPB_TBL(5, 1);	MAKE_INTER_CPB_TBL(5, 2);	MAKE_INTER_CPB_TBL(5, 3);
	MAKE_INTER_CPB_TBL(6, 0);	MAKE_INTER_CPB_TBL(6, 1);	MAKE_INTER_CPB_TBL(6, 2);	MAKE_INTER_CPB_TBL(6, 3);

	MAKE_INTER_CPB_BITS(0, 0, 6);	MAKE_INTER_CPB_BITS(0, 1, 6);	MAKE_INTER_CPB_BITS(0, 2, 7);	MAKE_INTER_CPB_BITS(0, 3, 7);
	MAKE_INTER_CPB_BITS(1, 0, 6);	MAKE_INTER_CPB_BITS(1, 1, 6);	MAKE_INTER_CPB_BITS(1, 2, 6);	MAKE_INTER_CPB_BITS(1, 3, 7);
	MAKE_INTER_CPB_BITS(2, 0, 6);	MAKE_INTER_CPB_BITS(2, 1, 5);	MAKE_INTER_CPB_BITS(2, 2, 6);	MAKE_INTER_CPB_BITS(2, 3, 7);
	MAKE_INTER_CPB_BITS(3, 0, 6);	MAKE_INTER_CPB_BITS(3, 1, 5);	MAKE_INTER_CPB_BITS(3, 2, 6);	MAKE_INTER_CPB_BITS(3, 3, 7);
	MAKE_INTER_CPB_BITS(4, 0, 5);	MAKE_INTER_CPB_BITS(4, 1, 5);	MAKE_INTER_CPB_BITS(4, 2, 6);	MAKE_INTER_CPB_BITS(4, 3, 7);
	MAKE_INTER_CPB_BITS(5, 0, 5);	MAKE_INTER_CPB_BITS(5, 1, 5);	MAKE_INTER_CPB_BITS(5, 2, 5);	MAKE_INTER_CPB_BITS(5, 3, 6);
	MAKE_INTER_CPB_BITS(6, 0, 5);	MAKE_INTER_CPB_BITS(6, 1, 5);	MAKE_INTER_CPB_BITS(6, 2, 5);	MAKE_INTER_CPB_BITS(6, 3, 5);

	//first_pattern
	MAKE_FIRST_PATTERN(inter, 0, 0);	MAKE_FIRST_PATTERN(inter, 0, 1);	
	MAKE_FIRST_PATTERN(inter, 1, 0);	MAKE_FIRST_PATTERN(inter, 1, 1);
	MAKE_FIRST_PATTERN(inter, 2, 0);	MAKE_FIRST_PATTERN(inter, 2, 1);
	MAKE_FIRST_PATTERN(inter, 3, 0);	MAKE_FIRST_PATTERN(inter, 3, 1);
	MAKE_FIRST_PATTERN(inter, 4, 0);	MAKE_FIRST_PATTERN(inter, 4, 1);
	MAKE_FIRST_PATTERN(inter, 5, 0);	MAKE_FIRST_PATTERN(inter, 5, 1);
	MAKE_FIRST_PATTERN(inter, 6, 0);	MAKE_FIRST_PATTERN(inter, 6, 1);

	
	//second_pattern
	MAKE_SECOND_PATTERN(inter, 0, 0);	MAKE_SECOND_PATTERN(inter, 0, 1);
	MAKE_SECOND_PATTERN(inter, 1, 0);	MAKE_SECOND_PATTERN(inter, 1, 1);
	MAKE_SECOND_PATTERN(inter, 2, 0);	MAKE_SECOND_PATTERN(inter, 2, 1);
	MAKE_SECOND_PATTERN(inter, 3, 0);	MAKE_SECOND_PATTERN(inter, 3, 1);
	MAKE_SECOND_PATTERN(inter, 4, 0);	MAKE_SECOND_PATTERN(inter, 4, 1);
	MAKE_SECOND_PATTERN(inter, 5, 0);	MAKE_SECOND_PATTERN(inter, 5, 1);
	MAKE_SECOND_PATTERN(inter, 6, 0);	MAKE_SECOND_PATTERN(inter, 6, 1);

	
	//third_pattern
	MAKE_THIRD_PATTERN(inter, 0, 0);	MAKE_THIRD_PATTERN(inter, 0, 1);
	MAKE_THIRD_PATTERN(inter, 1, 0);	MAKE_THIRD_PATTERN(inter, 1, 1);
	MAKE_THIRD_PATTERN(inter, 2, 0);	MAKE_THIRD_PATTERN(inter, 2, 1);
	MAKE_THIRD_PATTERN(inter, 3, 0);	MAKE_THIRD_PATTERN(inter, 3, 1);
	MAKE_THIRD_PATTERN(inter, 4, 0);	MAKE_THIRD_PATTERN(inter, 4, 1);
	MAKE_THIRD_PATTERN(inter, 5, 0);	MAKE_THIRD_PATTERN(inter, 5, 1);
	MAKE_THIRD_PATTERN(inter, 6, 0);	MAKE_THIRD_PATTERN(inter, 6, 1);

	//coeff
	MAKE_COEFFICIENT(inter, 0);	MAKE_COEFFICIENT(inter, 1);
	MAKE_COEFFICIENT(inter, 2);	MAKE_COEFFICIENT(inter, 3);
	MAKE_COEFFICIENT(inter, 4);	MAKE_COEFFICIENT(inter, 5);
	MAKE_COEFFICIENT(inter, 6);

	g_aic_top_vlc.table = g_aic_top_vlc_tbl;
	MAKE_AIC_TBL(2, 0);		MAKE_AIC_TBL(2, 1);		MAKE_AIC_TBL(2, 2);		MAKE_AIC_TBL(2, 3);
	MAKE_AIC_TBL(2, 4);		MAKE_AIC_TBL(2, 5);		MAKE_AIC_TBL(2, 6);		MAKE_AIC_TBL(2, 7);
	MAKE_AIC_TBL(2, 8);		MAKE_AIC_TBL(2, 9);		MAKE_AIC_TBL(2, 10);	MAKE_AIC_TBL(2, 11);
	MAKE_AIC_TBL(2, 12);	MAKE_AIC_TBL(2, 13);	MAKE_AIC_TBL(2, 14);	MAKE_AIC_TBL(2, 15);
	MAKE_AIC_TBL(2, 16);	MAKE_AIC_TBL(2, 17);	MAKE_AIC_TBL(2, 18);	MAKE_AIC_TBL(2, 19);

	MAKE_AIC_TBL(1, 0);		MAKE_AIC_TBL(1, 1);		MAKE_AIC_TBL(1, 2);		MAKE_AIC_TBL(1, 3);
	MAKE_AIC_TBL(1, 4);		MAKE_AIC_TBL(1, 5);		MAKE_AIC_TBL(1, 6);		MAKE_AIC_TBL(1, 7);
	MAKE_AIC_TBL(1, 8);		g_aic_mode1_vlc[9].table = NULL;
	MAKE_AIC_TBL(1, 10);	MAKE_AIC_TBL(1, 11);	MAKE_AIC_TBL(1, 12);	MAKE_AIC_TBL(1, 13);
	MAKE_AIC_TBL(1, 14);	MAKE_AIC_TBL(1, 15);	MAKE_AIC_TBL(1, 16);	MAKE_AIC_TBL(1, 17);
	MAKE_AIC_TBL(1, 18);	g_aic_mode1_vlc[19].table = NULL;
	MAKE_AIC_TBL(1, 20);	MAKE_AIC_TBL(1, 21);	MAKE_AIC_TBL(1, 22);	MAKE_AIC_TBL(1, 23);
	MAKE_AIC_TBL(1, 24);	MAKE_AIC_TBL(1, 25);	MAKE_AIC_TBL(1, 26);	MAKE_AIC_TBL(1, 27);
	MAKE_AIC_TBL(1, 28);	g_aic_mode1_vlc[29].table = NULL;
	MAKE_AIC_TBL(1, 30);	MAKE_AIC_TBL(1, 31);	MAKE_AIC_TBL(1, 32);	MAKE_AIC_TBL(1, 33);
	MAKE_AIC_TBL(1, 34);	MAKE_AIC_TBL(1, 35);	MAKE_AIC_TBL(1, 36);	MAKE_AIC_TBL(1, 37);
	MAKE_AIC_TBL(1, 38);	g_aic_mode1_vlc[39].table = NULL;
	MAKE_AIC_TBL(1, 40);	MAKE_AIC_TBL(1, 41);	MAKE_AIC_TBL(1, 42);	MAKE_AIC_TBL(1, 43);
	MAKE_AIC_TBL(1, 44);	MAKE_AIC_TBL(1, 45);	MAKE_AIC_TBL(1, 46);	MAKE_AIC_TBL(1, 47);
	MAKE_AIC_TBL(1, 48);	g_aic_mode1_vlc[49].table = NULL;
	MAKE_AIC_TBL(1, 50);	MAKE_AIC_TBL(1, 51);	MAKE_AIC_TBL(1, 52);	MAKE_AIC_TBL(1, 53);
	MAKE_AIC_TBL(1, 54);	MAKE_AIC_TBL(1, 55);	MAKE_AIC_TBL(1, 56);	MAKE_AIC_TBL(1, 57);
	MAKE_AIC_TBL(1, 58);	g_aic_mode1_vlc[59].table = NULL;
	MAKE_AIC_TBL(1, 60);	MAKE_AIC_TBL(1, 61);	MAKE_AIC_TBL(1, 62);	MAKE_AIC_TBL(1, 63);
	MAKE_AIC_TBL(1, 64);	MAKE_AIC_TBL(1, 65);	MAKE_AIC_TBL(1, 66);	MAKE_AIC_TBL(1, 67);
	MAKE_AIC_TBL(1, 68);	g_aic_mode1_vlc[69].table = NULL;
	MAKE_AIC_TBL(1, 70);	MAKE_AIC_TBL(1, 71);	MAKE_AIC_TBL(1, 72);	MAKE_AIC_TBL(1, 73);
	MAKE_AIC_TBL(1, 74);	MAKE_AIC_TBL(1, 75);	MAKE_AIC_TBL(1, 76);	MAKE_AIC_TBL(1, 77);
	MAKE_AIC_TBL(1, 78);	g_aic_mode1_vlc[79].table = NULL;
	MAKE_AIC_TBL(1, 80);	MAKE_AIC_TBL(1, 81);	MAKE_AIC_TBL(1, 82);	MAKE_AIC_TBL(1, 83);
	MAKE_AIC_TBL(1, 84);	MAKE_AIC_TBL(1, 85);	MAKE_AIC_TBL(1, 86);	MAKE_AIC_TBL(1, 87);
	MAKE_AIC_TBL(1, 88);	g_aic_mode1_vlc[89].table = NULL;

	MAKE_PBTYPE_TBL(p, 0);	MAKE_PBTYPE_TBL(p, 1);	MAKE_PBTYPE_TBL(p, 2);	MAKE_PBTYPE_TBL(p, 3);
	MAKE_PBTYPE_TBL(p, 4);	MAKE_PBTYPE_TBL(p, 5);	MAKE_PBTYPE_TBL(p, 6);
	
	MAKE_PBTYPE_TBL(b, 0);	MAKE_PBTYPE_TBL(b, 1);	MAKE_PBTYPE_TBL(b, 2);	MAKE_PBTYPE_TBL(b, 3);
	MAKE_PBTYPE_TBL(b, 4);	MAKE_PBTYPE_TBL(b, 5);	


	#undef MAKE_INTRA_CBPPATTERN	
	#undef MAKE_INTER_CBPPATTERN	
	#undef MAKE_SECOND_PATTERN
	#undef MAKE_THIRD_PATTERN
	#undef MAKE_INTRA_CPB_TBL
	#undef MAKE_INTRA_CPB_BITS
	#undef MAKE_INTER_CPB_TBL	
	#undef MAKE_INTER_CPB_BITS
	#undef MAKE_FIRST_PATTERN
	#undef MAKE_COEFFICIENT
	#undef MAKE_PBTYPE_TBL	
	#undef MAKE_AIC_TBL	
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
