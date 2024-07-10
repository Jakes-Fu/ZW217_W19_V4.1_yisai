
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

//extra memory
LOCAL uint32 s_used_extra_mem = 0x0;
LOCAL uint32 s_extra_mem_size = 0x1000000;	//16Mbyte

//inter memory
LOCAL uint32 s_used_inter_mem = 0x0;
LOCAL uint32 s_inter_mem_size = 0x400000;	//4Mbyte

LOCAL uint8 *s_extra_mem_bfr_ptr = NULL;
LOCAL uint8 *s_inter_mem_bfr_ptr = NULL;

PUBLIC void *RvDec_ExtraMemAlloc(uint32 mem_size)
{
	uint8 *pMem;
	mem_size = ((mem_size + 3) &(~3));

	if((0 == mem_size)||(mem_size> (s_extra_mem_size-s_used_extra_mem)))
	{
		//SCI_ASSERT(0);
	        return (void *)PNULL;
	}
	
	pMem = s_extra_mem_bfr_ptr + s_used_extra_mem;
	s_used_extra_mem += mem_size;
	
	return pMem;
}

PUBLIC void *RvDec_ExtraMemAlloc_64WordAlign(uint32 mem_size)
{
	uint32 CurrAddr, _64WordAlignAddr;
		
	CurrAddr = (uint32)s_extra_mem_bfr_ptr + s_used_extra_mem;

	_64WordAlignAddr = ((CurrAddr + 255) >>8)<<8;

	mem_size += (_64WordAlignAddr - CurrAddr);

	if((0 == mem_size)||(mem_size> (s_extra_mem_size-s_used_extra_mem)))
	{
		//SCI_ASSERT(0);
	        return (void *)PNULL;
	}
	
	s_used_extra_mem += mem_size;
	
	return (void *)_64WordAlignAddr;
}

PUBLIC void *RvDec_InterMemAlloc(uint32 mem_size)
{
	uint8 *pMem;
	mem_size = ((mem_size + 3) &(~3));

	if((0 == mem_size)||(mem_size> (s_inter_mem_size-s_used_inter_mem)))
	{
		//SCI_ASSERT(0);
	        return (void *)PNULL;
	}
	
	pMem = s_inter_mem_bfr_ptr + s_used_inter_mem;
	s_used_inter_mem += mem_size;
	
	return pMem;
}

PUBLIC void RvDec_FreeMem(void) 
{ 
	s_used_inter_mem = 0;
	s_used_extra_mem = 0;
}

PUBLIC void RvDec_FreeExtraMem(void) 
{ 
	s_used_extra_mem = 0;
}

PUBLIC void RvDec_InitInterMem(MMCodecBuffer *dec_buffer_ptr)
{
	s_inter_mem_bfr_ptr = dec_buffer_ptr->int_buffer_ptr;
	s_inter_mem_size = dec_buffer_ptr->int_size;
	memset(s_inter_mem_bfr_ptr, 0, s_inter_mem_size);
	
	//reset memory used count
	s_used_inter_mem = 0;
}

PUBLIC MMDecRet RvDecMemInit(MMCodecBuffer *pBuffer)
{
	s_extra_mem_bfr_ptr = pBuffer->common_buffer_ptr;
	s_extra_mem_size = pBuffer->size;
	memset(s_extra_mem_bfr_ptr, 0, s_extra_mem_size);
	
	//reset memory used count
	s_used_extra_mem = 0;

	return MMDEC_OK;
}
/*****************************************************************************/
//  Description:   Calculate the size of external memory for Real decoder
//	Global resource dependence: 
//  Author:        
//	Note:          
/*****************************************************************************/
MMDecRet RVDecMemSizeCal(uint32 picWidth, uint32 picHeight, MMDecMemSize *mem_size)
{
	uint32 uMbNumX, uMbNumY, uFrameWidth, uFrameHeight, uFrameExtendWidth, uFrameExtendHeight;
	uint32 uMbTotal;
	uint32 size;

	uMbNumX = (picWidth + 15)/MB_SIZE;
	uMbNumY = (picHeight + 15)/MB_SIZE;
	uMbTotal= uMbNumX * uMbNumY;
	
	uFrameWidth = uMbNumX * MB_SIZE;
	uFrameHeight = uMbNumY * MB_SIZE;
	
	uFrameExtendWidth = uFrameWidth + RV_Y_EXTEND_SIZE*2;
	uFrameExtendHeight = uFrameHeight + RV_Y_EXTEND_SIZE*2;	

	size = uFrameExtendWidth * uFrameExtendHeight * 3/2 * 3; // for YUV buffer
	size+= uFrameExtendWidth * 2; // for bottom line of mb of 16x16 size
	size+= 2 * uMbTotal * 4; // for MB_MODE_T
	size+= 1876; // for MB_CACHE
	size+= uMbNumX * 4; // for top Ipred mode
	size+= 4 * uMbTotal;// for MVD
	size+= 4 * uMbTotal * 4 * 3;// for motion vector
	size+= 56; // for luma top, chroma top
	size+= 2 * 1024;//for 64 Word Align

	mem_size->memSize = size;
	mem_size->frame_width = picWidth;
	mem_size->frame_height= picHeight;

	return MMDEC_OK;
	

}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
		
