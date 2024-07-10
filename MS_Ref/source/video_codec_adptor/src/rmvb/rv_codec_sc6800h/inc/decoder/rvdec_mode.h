
#ifndef _RVDEC_MODE_H_
#define _RVDEC_MODE_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_basic.h"
#include "rvdec_bitstrm.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

#ifndef WIN32
#define WORD_ALIGN __align(32)
#else
#define WORD_ALIGN
#endif

//#define IS_INTRA_MBTYPE(mbtype) ((mbtype) < MBTYPE_INTER)
//#define IS_INTER_MBTYPE(mbtype) ((mbtype) >= MBTYPE_INTER)
	
#define MAX_SLICE_NUM		100

typedef struct dec_buffer_seq_info_tag
{
	uint8 *imgYUV[3];		//should be 64 word alignment

	uint32 imgYUVAddr[3];	//frame address which are configured to VSP,  imgYAddr = ((uint32)imgY >> 8), 64 word aligment

	uint8	id;    	// buffer number 
	BOOLEAN bRef;   // FALSE£¬not to be  ref frame 
					// TRUE, to  the ref frame
	BOOLEAN bDisp; 	// FALSE, not to be display frame
					// TRUE, to be the display frame
	uint8 rsv;

	uint8 *rec_imgY;
}DEC_FRM_BFR;

typedef struct storable_pic
{
	int32		time;		//display time of the frame
	int32		bfrId;		//yuv buffer index	

	DEC_FRM_BFR *pDecFrame;

	int32	is_downsized;
	
}STORABLE_PICTURE_T;

typedef struct mb_pixel_tag
{
	uint8 pMBBfrY[256];
	uint8 pMBBfrU[64];
	uint8 pMBBfrV[64];
}MB_PIXEL_T;

typedef struct mb_pixel_downsize_tag
{
	uint8 pMBBfrY[64];
	uint8 pMBBfrU[16];
	uint8 pMBBfrV[16];
}MB_PIXEL_D_T;

typedef struct mb_mode_tag
{
	uint8 uMbType;
	uint8 uQp;
	uint8 uSilceNumber;
	uint8 mvdir;										//for B frame, b'01: indicate forward, b'10: indicate backward, b'11: indicate bi-directional
#if defined(DBK_SUPPORTED)
	uint32 uCbpcoef;
#endif
}MB_MODE_T;

typedef struct mb_cache_tag
{
	MB_PIXEL_T mbpix;								//for storing one mb's YUV pixel
	MB_PIXEL_T mbpix_bck;							//if bi-directional, storing the backward reference MB

	BOOLEAN bIsIntra;
	BOOLEAN intra16x16;
	BOOLEAN inter16x16;
	BOOLEAN b16;
	
	uint32 cbp;

	int8 mbAvailA;
	int8 mbAvailB;
	int8 mbAvailC;
	int8 mbAvailD;

	uint8 i16mode;
	uint8 intpDir;						//intpolation direction: b'01 forward, b'10 backward, b'11 bi-directional
	BOOLEAN bUse4Mv;
	int8 rsv;

	int16 Luma_DCCoeff[16];
	int16 LumaCoeff[16*16];				//coff_Y[16 * 4*4];
    int16 ChromaCoeff[2][4*16];

	int16 mv_cache[4*3*2];				//mv
	int16 bck_mv_cache[4*3*2];			//backward mv cache
	int8 IPMode4x4_cache[12*5];

	int32 luma_qp;
	int16 chroma_dc_qp;
	int16 chroma_ac_qp;

	MB_PIXEL_D_T mbpix_d;							// For downsize decoder

	uint8	leftPredPix_Y[16];
	uint8	leftPredPix_UV[2][8];

}MB_CACHE_T;

typedef struct short_VLC_tag 
{
    const int16 (*table)[2]; ///< code, bits    
    uint32 bits;
}SHORT_VLC_SYMBOL_T;

typedef struct byte_VLC_tag 
{
    const uint8 (*table)[2]; ///< code, bits    
    uint32 bits;
}BYTE_VLC_SYMBOL_T;

/**
 * VLC tables used by the decoder
 *
 * Intra frame VLC sets do not contain some of those tables.
 */
typedef struct rv_vlc_set_tag
{
    SHORT_VLC_SYMBOL_T cbppattern[2];     ///< VLCs used for pattern of coded block patterns decoding
    BYTE_VLC_SYMBOL_T cbp[2][4];         ///< VLCs used for coded block patterns decoding
    SHORT_VLC_SYMBOL_T first_pattern[4];  ///< VLCs used for decoding coefficients in the first subblock
    SHORT_VLC_SYMBOL_T second_pattern[2]; ///< VLCs used for decoding coefficients in the subblocks 2 and 3
    SHORT_VLC_SYMBOL_T third_pattern[2];  ///< VLCs used for decoding coefficients in the last subblock
    SHORT_VLC_SYMBOL_T coefficient;       ///< VLCs used for decoding big coefficients
}RV_VLC_SET_T;

typedef struct decoder_tag
{
	uint8 uMbPosX;
	uint8 uMbPosY;
	uint8 uMbNumX;
	uint8 uMbNumY;
	
	uint16 uMbNumTotal;
	uint16 uMbNumDec;

	uint16 uOrgFrameWidth;	
	uint16 uOrgFrameHeight;

	uint16 uFrameWidth;
	uint16 uFrameHeight;

	uint16 uFrameExtendWidth;
	uint16 uFrameExtendHeight;

	uint16 uStartInFrameY;
	uint16 uStartInFrameUV;	
	
	uint8 uMbASize;
	uint8 uPicCodeType;
	uint8 uMcFactor;
	uint8 uSilceNumber;
	
	BOOLEAN bIsRV8;
	BOOLEAN bInitSuceess;
	int16 iSkipModesLeft;

	MB_MODE_T *mb_mode_ptr;
	MB_MODE_T *mb_mode_b_ptr;
	MB_CACHE_T *mb_cache_ptr;
	BitStream *stream;
	
	int32 iRatio0;					//for B frame's interpolation
	int32 iRatio1;

	uint8 ISQUANT;
	uint8 prevQuant;
	uint8 OSVQUANT;
	uint8 uPQuant;
	
	uint8 uEntropyQP;
	uint8 pixel_aspect_ratio;
	uint16 uTR;

	uint32 *mvd_ptr;	//flag to affect edge deblocking, which is determined by motion vector    
	int8 *pIPredModeTop;			//for storing one top row block's intra prediction mode 

	STORABLE_PICTURE_T *pCurRecFrame;
	STORABLE_PICTURE_T *pBckRefFrame;
	STORABLE_PICTURE_T *pFrdRefFrame;

	MOTION_VECTOR_T *pMVFrameP;		
	MOTION_VECTOR_T *pMVFrameB_f;
	MOTION_VECTOR_T *pMVFrameB_b;

	//for motion compensation, direct mode
	int16 xmax_luma;	///1mv
	int16 ymax_luma;
	int16 xmax_chroma;
	int16 ymax_chroma;

	int16 xmax4v_luma;	///4mv
	int16 ymax4v_luma;
	int16 xmax4v_chroma;
	int16 ymax4v_chroma;

	uint32 numSizes;
	uint32 pctszSize;
	uint32 Sizes[18];
	uint32 deblocking_filter_passthrough;

	int32 error_flag;
	uint32 frame_rate;

    RV_VLC_SET_T *cur_vlcs;       ///< VLC set used for current frame decoding

	void (*DecIPredMode4x4)(BitStream * stream, MB_CACHE_T *mb_cache_ptr, int8 *pIPCache);

	// Frame size info for downsize dec
	uint16 uFrameWidth_d;
	uint16 uFrameHeight_d;

	uint16 uFrameExtendWidth_d;
	uint16 uFrameExtendHeight_d;

	uint16 uStartInFrameY_d;
	uint16 uStartInFrameUV_d;

	uint32 need_downsize;
	uint32 uMcFactor_d;

	uint32 total_slice_num;
	uint32 slice_offset[MAX_SLICE_NUM]; //byte
	uint8  slice_valid[MAX_SLICE_NUM];
}RV_DECODER_T;

typedef int32 (*set_deblock_coeff_func)(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *mv_ptr, int32 mbtype);
typedef void (*read_mbtype_PB_func)(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
typedef BOOLEAN (*get_slice_sc_func)(RV_DECODER_T *rv_decoder_ptr, BitStream *stream);
typedef void (*mc_func)(uint8 *pRef, uint8 *pPred,  uint32 uFrmWidth);
typedef void (*Intra4x4Pred_func)(uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *mb_cache_ptr);
typedef void (*Intra16x16Pred_func)(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *mb_cache_ptr);
typedef void (*decMV_B_func)(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b);
typedef void (*weighted_average_func)(RV_DECODER_T *rv_decoder_ptr, MB_PIXEL_T *pmbpix, MB_PIXEL_T *pmbpix_b);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_MODE_H_
