
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

PUBLIC void RvDec_SetClipTab(void)
{
#if !defined(SC6800H_VIDEO_ROMCODE)
	int TabSize, maxVal, iOffset, i;
	
	TabSize = CLIP_TAB_SIZE;
	maxVal = 255;
	iOffset = (TabSize - maxVal)/2;
		
	//g_rgiRvClipTab = g_clip_tbl_rvdec_ptr + iOffset;
	g_rgiRvClipTab = g_ClipTab_rv89 + iOffset;
	for (i = -iOffset; i < TabSize - iOffset; i++)
	{
		g_rgiRvClipTab [i] = (i < 0) ? 0 : (i > maxVal) ? maxVal : i;		
	}
#else
	g_rgiRvClipTab = (uint8 *)g_clip_tbl_rvdec_ptr + 32*16;
#endif
}

/*****************************************************************************
 **	Name : 			Mp4Dec_InitDecoderPara
 ** Description:	Initialize the decode parameter. 
 ** Author:			Xiaowei Luo
 **	Note:
 *****************************************************************************/
PUBLIC void RvDec_InitDecoderPara(RV_DECODER_T *rv_decoder_ptr)
{
	g_nFrame_rv = 0;  
#if 0	
#if defined(BSLICE_SUPPORTED)
	g_nBFrame = 0;
	g_Bframe_eb = 1;
	
	if ((rv_decoder_ptr->frame_rate > 15) || rv_decoder_ptr->need_downsize)
	{
		g_Bframe_eb = 0;
	}
#endif
#endif

	rv_decoder_ptr->bInitSuceess = FALSE;

    rv_decoder_ptr->pCurRecFrame = (STORABLE_PICTURE_T *)RvDec_InterMemAlloc(sizeof(STORABLE_PICTURE_T));
	rv_decoder_ptr->pBckRefFrame = (STORABLE_PICTURE_T *)RvDec_InterMemAlloc(sizeof(STORABLE_PICTURE_T));
	rv_decoder_ptr->pFrdRefFrame = (STORABLE_PICTURE_T *)RvDec_InterMemAlloc(sizeof(STORABLE_PICTURE_T));

	rv_decoder_ptr->pCurRecFrame->bfrId = 0xFF;
	rv_decoder_ptr->pBckRefFrame->bfrId = 0xFF;
	rv_decoder_ptr->pFrdRefFrame->bfrId = 0xFF;

    memset(g_RvFrmYUVBfr,0,sizeof(g_RvFrmYUVBfr));
}

LOCAL MMDecRet RvDec_MallocFrmBfr(RV_DECODER_T *rv_decoder_ptr)
{
    if( MMDEC_OK != RvDec_InitYUVBfr(rv_decoder_ptr) )
    {
        return MMDEC_MEMORY_ERROR;
    }

	/*backward reference frame and forward reference frame after extention*/
	//all these address should be 64 word alighed
//	size = vop_mode_ptr->FrameWidth * vop_mode_ptr->FrameHeight;	
	rv_decoder_ptr->pCurRecFrame->pDecFrame = &g_RvFrmYUVBfr[0];  

    return MMDEC_OK;
}

PUBLIC MMDecRet RvDec_InitSessionDecode(RV_DECODER_T *rv_decoder_ptr)
{
	rv_decoder_ptr->uMbNumX = (rv_decoder_ptr->uOrgFrameWidth + 15)/MB_SIZE;
	rv_decoder_ptr->uMbNumY = (rv_decoder_ptr->uOrgFrameHeight + 15)/MB_SIZE;
	rv_decoder_ptr->uMbNumTotal = rv_decoder_ptr->uMbNumX * rv_decoder_ptr->uMbNumY;
	
	rv_decoder_ptr->uFrameWidth = rv_decoder_ptr->uMbNumX * MB_SIZE;
	rv_decoder_ptr->uFrameHeight = rv_decoder_ptr->uMbNumY * MB_SIZE;
	
	rv_decoder_ptr->uFrameExtendWidth = rv_decoder_ptr->uFrameWidth + RV_Y_EXTEND_SIZE*2;
	rv_decoder_ptr->uFrameExtendHeight = rv_decoder_ptr->uFrameHeight + RV_Y_EXTEND_SIZE*2;
	
	rv_decoder_ptr->uStartInFrameY = rv_decoder_ptr->uFrameExtendWidth * RV_Y_EXTEND_SIZE + RV_Y_EXTEND_SIZE;
	rv_decoder_ptr->uStartInFrameUV = (rv_decoder_ptr->uFrameExtendWidth/2) * RV_UV_EXTEND_SIZE + RV_UV_EXTEND_SIZE;

	rv_decoder_ptr->uFrameWidth_d = rv_decoder_ptr->uMbNumX * (MB_SIZE/2);
	rv_decoder_ptr->uFrameHeight_d = rv_decoder_ptr->uMbNumY * (MB_SIZE/2);
	
	rv_decoder_ptr->uFrameExtendWidth_d = rv_decoder_ptr->uFrameWidth_d + (RV_Y_EXTEND_SIZE/2)*2;
	rv_decoder_ptr->uFrameExtendHeight_d = rv_decoder_ptr->uFrameHeight_d + (RV_Y_EXTEND_SIZE/2)*2;
	
	rv_decoder_ptr->uStartInFrameY_d = rv_decoder_ptr->uFrameExtendWidth_d * (RV_Y_EXTEND_SIZE/2) + (RV_Y_EXTEND_SIZE/2);
	rv_decoder_ptr->uStartInFrameUV_d = (rv_decoder_ptr->uFrameExtendWidth_d/2) * (RV_UV_EXTEND_SIZE/2) + (RV_UV_EXTEND_SIZE/2);		
	
	rv_decoder_ptr->mb_mode_ptr = (MB_MODE_T *)RvDec_ExtraMemAlloc (rv_decoder_ptr->uMbNumTotal*sizeof(MB_MODE_T));
	if(NULL == rv_decoder_ptr->mb_mode_ptr)
    {
        return MMDEC_MEMORY_ERROR;
    }
#if defined(BSLICE_SUPPORTED)
	rv_decoder_ptr->mb_mode_b_ptr = (MB_MODE_T *)RvDec_ExtraMemAlloc (rv_decoder_ptr->uMbNumTotal*sizeof(MB_MODE_T));
	if(NULL == rv_decoder_ptr->mb_mode_b_ptr)
    {
        return MMDEC_MEMORY_ERROR;
    }
#endif	
	rv_decoder_ptr->mb_cache_ptr = (MB_CACHE_T *)RvDec_ExtraMemAlloc (sizeof(MB_CACHE_T));
	if(NULL == rv_decoder_ptr->mb_cache_ptr)
    {
        return MMDEC_MEMORY_ERROR;
    }
	rv_decoder_ptr->pIPredModeTop = (int8 *)RvDec_ExtraMemAlloc (sizeof(char) * rv_decoder_ptr->uMbNumX*4);
	if(NULL == rv_decoder_ptr->pIPredModeTop)
    {
        return MMDEC_MEMORY_ERROR;
    }
	
	rv_decoder_ptr->mvd_ptr = (uint32 *)RvDec_ExtraMemAlloc (sizeof(uint32) * rv_decoder_ptr->uMbNumTotal);
	if(NULL == rv_decoder_ptr->mvd_ptr)
    {
        return MMDEC_MEMORY_ERROR;
    }
	
	rv_decoder_ptr->pMVFrameP = (MOTION_VECTOR_T *)RvDec_ExtraMemAlloc(sizeof(MOTION_VECTOR_T) * rv_decoder_ptr->uMbNumTotal * 4);
	if(NULL == rv_decoder_ptr->pMVFrameP)
    {
        return MMDEC_MEMORY_ERROR;
    }
#if defined(BSLICE_SUPPORTED)
	rv_decoder_ptr->pMVFrameB_f	= (MOTION_VECTOR_T *)RvDec_ExtraMemAlloc(sizeof(MOTION_VECTOR_T) * rv_decoder_ptr->uMbNumTotal * 4);
	if(NULL == rv_decoder_ptr->pMVFrameB_f)
    {
        return MMDEC_MEMORY_ERROR;
    }
	rv_decoder_ptr->pMVFrameB_b	= (MOTION_VECTOR_T *)RvDec_ExtraMemAlloc(sizeof(MOTION_VECTOR_T) * rv_decoder_ptr->uMbNumTotal * 4);
	if(NULL == rv_decoder_ptr->mb_mode_ptr)
    {
        return MMDEC_MEMORY_ERROR;
    }
#endif
	if( MMDEC_OK != RvDec_MallocFrmBfr(rv_decoder_ptr) )
    {
        return MMDEC_MEMORY_ERROR;
    }

	{
		uint8 *tmp;

		tmp = (uint8 *)RvDec_ExtraMemAlloc(4);
		luma_top = (uint8 *)RvDec_ExtraMemAlloc(20);
		tmp = (uint8 *)RvDec_ExtraMemAlloc(4);
		chroma_top[0] = (uint8 *)RvDec_ExtraMemAlloc(12);
		tmp = (uint8 *)RvDec_ExtraMemAlloc(4);
		chroma_top[1] = (uint8 *)RvDec_ExtraMemAlloc(12);
	}
	
{
	uint32 uMcFactor = rv_decoder_ptr->uMcFactor;
	rv_decoder_ptr->xmax_luma = (rv_decoder_ptr->uFrameWidth - 3) * uMcFactor - 1;
	rv_decoder_ptr->ymax_luma = (rv_decoder_ptr->uFrameHeight - 6) * uMcFactor - 1; 
	rv_decoder_ptr->xmax_chroma = ((rv_decoder_ptr->uFrameWidth>>1) - 1) * uMcFactor - 1;
	rv_decoder_ptr->ymax_chroma = ((rv_decoder_ptr->uFrameHeight>>1) - 1) * uMcFactor - 1;

	rv_decoder_ptr->xmax4v_luma = (rv_decoder_ptr->uFrameWidth + 5) * uMcFactor - 1;
	rv_decoder_ptr->ymax4v_luma = (rv_decoder_ptr->uFrameHeight + 2) * uMcFactor - 1;
	rv_decoder_ptr->xmax4v_chroma = ((rv_decoder_ptr->uFrameWidth>>1) - 3) * uMcFactor - 1;
	rv_decoder_ptr->ymax4v_chroma = ((rv_decoder_ptr->uFrameHeight>>1) - 3) * uMcFactor - 1;
}
	
    return MMDEC_OK;
}

#if defined(SC6800H_VIDEO_ROMCODE)

//#define SC6800H1	0
//#define SC6800H3	1

PUBLIC void RvDec_init_ROM_TABLE()
{
	uint32 chip_id = CHIP_GetChipType();
	if (chip_id == SC6800H3)
	{
		g_msk_rvdec_ptr = (const uint32 *)0x30002a58 ;
		g_clip_tbl_rvdec_ptr = (const uint8 *)0x30002adc;
	}else
	{
		g_msk_rvdec_ptr = (const uint32 *)0x30004800 ;
		g_clip_tbl_rvdec_ptr = (const uint8 *)0x300048a0;
	}
	
}
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
		
