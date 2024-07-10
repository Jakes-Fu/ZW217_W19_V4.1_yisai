
#ifndef  _RVDEC_VLD_H_
#define  _RVDEC_VLD_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_basic.h"
#include "rvdec_mode.h"
#include "rvdec_global.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

//now, bits = 9, max_depth = 2
__inline int32 RvDec_HuffDecode (BitStream *stream, const int16 tbl[][2]/*, uint32 bits, int32 max_depth*/)
{
	int32 n;
	uint32 idx;
	int32 code;
	uint32 val16;
	uint32 flush_bits = 0;

	val16 = RvDec_ShowBits(stream, 16/*bits*/);
	idx = val16 >> 7;
	code = tbl[idx][0];
	n = tbl[idx][1];

	if (n < 0)
	{
		flush_bits = 9;
		val16 <<= 25;
		idx = val16 >> (32+n);

	//	idx = (val16 >> (7+n)) & g_rv_msk[-n];
		idx += code;
		code = tbl[idx][0];
		n = tbl[idx][1];
	}

	flush_bits += n;
	RvDec_FlushBits(stream, flush_bits);

	return code;
}

PUBLIC uint32 RvDec_GetVlcBits(BitStream *stream, uint32 *pInfo, uint32 *pLen);
PUBLIC void RvDec_Dec4x4Block(RV_DECODER_T *rv_decoder_ptr, int16 *pBlock, int32 fc, int32 sc, int32 qdc, int32 qac);
PUBLIC void RvDec_InitVldTable(void);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_VLD_H_
