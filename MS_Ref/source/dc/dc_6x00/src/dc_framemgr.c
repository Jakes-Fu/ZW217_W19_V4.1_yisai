/******************************************************************************
 ** File Name:	dc_bufmgr.c                                               	  
 ** Author:		Frank.Yang		                                          
 ** DATE:		8/4/2008                                               
 ** Copyright:		2008 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION                            
 ******************************************************************************/
#include "ms_ref_dc_trc.h"
#include "dc_app_6x00.h"
#include "dc_framemgr.h"


/**
 * there are three mode to use dc frame buffer:
 * 1. capture mode, used to stored compressed data
 * 2. review mode( review a jpg file), if the file size is smaller than the frame size
 *    read jpg file to the frame buffer, parser and decode it. if the file size is 
 *    larger than the frame buffer size, the frame buffer split two parts, used as
 *    ping-pong buffer. when one is using for decoding the other is used for reading.
 * 3. for mjpg capture mode, used as ring buffer to store compressed data
 *
 * in this file, we implement the frame buffer for DC capturing / MJPG recording
 */


static DCAM_BUFFER_MGR_T g_dc_bufmgr;


/******************************************************************************/
// Description: 	init frames mgr
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		
/******************************************************************************/
PUBLIC void DC_FrameMgr_Init(uint8 *p_buf, uint32 dw_len)
{
	DCAM_BUFFER_MGR_T* pBufMgr=&g_dc_bufmgr;


	
	/*init ring buffer*/
	pBufMgr->base_addr		= (uint32) p_buf;
	pBufMgr->uplimited_addr	= pBufMgr->base_addr + dw_len;
	
	/*ring buffer is empty*/
	pBufMgr->ring_begin		= pBufMgr->base_addr;
	pBufMgr->ring_end		= pBufMgr->base_addr;
	pBufMgr->remain_len		= dw_len;
	pBufMgr->buf_len		= dw_len;

	/*frame fifo is empty*/
	pBufMgr->first_frame		= 0;
	pBufMgr->last_frame		= 0;

	//clear memory
	SCI_MEMSET(pBufMgr->frames, 0x0, (sizeof(DC_FRAME_T)*MAX_FIFO_FRAME_NUM));
	
	//DCAM_PRINT:"DC_FrameMgr_Init(),base_addr = %x, uplimited_addr = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_62_112_2_18_1_23_30_107,(uint8*)"dd",pBufMgr->base_addr, pBufMgr->uplimited_addr);
	
}


/******************************************************************************/
// Description: 	create a frame
// Author:     		Frank.yang
// Input:      		param_ptr   point to the preview parameter
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC DC_FRAME_T *DC_FrameMgr_CreateAFrame(uint32 dw_frame_size)
{
	uint32 real_size;
	int32 tmp, tmp1;
	DCAM_BUFFER_MGR_T*	pBufMgr=&g_dc_bufmgr;
	DC_FRAME_T *p_frame;

	  
      //SCI_TRACE_LOW("DC_FrameMgr_C,  base: 0x%x, up:0x%x",  pBufMgr->base_addr, pBufMgr->uplimited_addr);      

	/*hardware is 256 bytes aligned*/
	real_size = ((dw_frame_size+255)>>8)<<8;

	SCI_DisableIRQ();


  	if (real_size > pBufMgr->remain_len)
	{
		/*ring buffer overflow*/
		/*DC task manager should stop the recording routine*/
		//DCAM_PRINT:"DC DC_FrameMgr_C,ring buffer overflowed!,line %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_93_112_2_18_1_23_30_108,(uint8*)"d",__LINE__);
		SCI_RestoreIRQ();

		return NULL;
	}

	tmp = (pBufMgr->last_frame + 1)&MAX_FIFO_FRAME_MASK;
	if ( tmp == pBufMgr->first_frame)
	{
		/*frame fifo is over flow*/
		//DCAM_PRINT:"DC DC_FrameMgr_C,frame fifo overflowed!, line %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_103_112_2_18_1_23_30_109,(uint8*)"d",__LINE__);
		SCI_RestoreIRQ();

		return NULL;
	}


	
	/*update frame buffer*/
	tmp1 = pBufMgr->last_frame;
	pBufMgr->last_frame = tmp;

	/*fill the frame*/
	p_frame = &pBufMgr->frames[tmp1];
	p_frame->dw_total_section_len = real_size;
	if ((pBufMgr->ring_end+ real_size) > pBufMgr->uplimited_addr)
	{

		if(0 != pBufMgr->tail_fragment_len)
		{
			pBufMgr->tail_fragment_len = pBufMgr->uplimited_addr - pBufMgr->ring_end;  //set tail
		}
		else  //remain including tail
		{
			pBufMgr->tail_fragment_len = pBufMgr->uplimited_addr - pBufMgr->ring_end;  //set tail
			pBufMgr->remain_len -= pBufMgr->tail_fragment_len;
		
		}
		pBufMgr->ring_end = pBufMgr->base_addr;
		
		p_frame->sections[0].pbuf = (uint8 *)pBufMgr->ring_end;
		p_frame->sections[0].dw_len = real_size;
		p_frame->dw_total_section_len = p_frame->sections[0].dw_len;
		//p_frame->sections[0].pbuf = (uint8 *)pBufMgr->ring_end;
		//p_frame->sections[0].dw_len = pBufMgr->uplimited_addr - pBufMgr->ring_end;
		//p_frame->sections[1].pbuf = (uint8 *)pBufMgr->base_addr;
		//p_frame->sections[1].dw_len = real_size - p_frame->sections[0].dw_len;
		pBufMgr->ring_end += p_frame->sections[0].dw_len;
		//DCAM_PRINT:"DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_141_112_2_18_1_23_30_110,(uint8*)"ud", pBufMgr->tail_fragment_len, __LINE__);
	}
	else
	{
		p_frame->sections[0].pbuf = (uint8 *)pBufMgr->ring_end;
		p_frame->sections[0].dw_len = real_size;
		p_frame->dw_total_section_len = p_frame->sections[0].dw_len;
		//p_frame->sections[1].pbuf = NULL;
		//p_frame->sections[1].dw_len = 0;
		pBufMgr->ring_end += p_frame->sections[0].dw_len;
/*
		//if the tail part is not enough, omit it and re-add it the remain_len when ring_begin falls in the tail fragment area
		if(pBufMgr->ring_end +pBufMgr->tail_fragment_len >= pBufMgr->uplimited_addr)
		{
			pBufMgr->tail_fragment_len = pBufMgr->uplimited_addr - pBufMgr->ring_end;
			pBufMgr->remain_len -= pBufMgr->tail_fragment_len;
			pBufMgr->ring_end = pBufMgr->base_addr;
			//DCAM_PRINT:"DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_157_112_2_18_1_23_30_111,(uint8*)"ud", pBufMgr->tail_fragment_len, __LINE__);
		}
*/
	}

	pBufMgr->remain_len -= real_size;

	//DCAM_PRINT:"DC_FrameMgr_CreateAFrame,remain ,len = %lu, line %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_164_112_2_18_1_23_30_112,(uint8*)"ud", pBufMgr->remain_len, __LINE__);
	
	SCI_RestoreIRQ();

	return p_frame;
		
}

/******************************************************************************/
// Description: 	get a frame
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC DC_FRAME_T * DC_FrameMgr_GetAFrame(void)
{
	return (&g_dc_bufmgr.frames[g_dc_bufmgr.first_frame]);
}

/******************************************************************************/
// Description: 	free a frame
// Author:     		Frank.yang
// Input:      		p_frame--point to the frame needed to be free
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC int DC_FrameMgr_FreeAFrame(DC_FRAME_T *p_frame)
{
	DCAM_BUFFER_MGR_T* pBufMgr=&g_dc_bufmgr;


	SCI_DisableIRQ();

	if (pBufMgr->first_frame == pBufMgr->last_frame)
	{
		/*frame fifo currentis empty*/
		
		SCI_RestoreIRQ();
		return SCI_ERROR;
	}

	if (p_frame != &pBufMgr->frames[pBufMgr->first_frame])
	{
		SCI_RestoreIRQ();
		SCI_ASSERT(0);/*assert verified*/
	
		//return SCI_ERROR;
	}

	
	pBufMgr->first_frame = (pBufMgr->first_frame+1)&MAX_FIFO_FRAME_MASK;

	pBufMgr->remain_len += p_frame->dw_total_section_len;
	pBufMgr->ring_begin += p_frame->dw_total_section_len;
	SCI_MEMSET(p_frame, 0x0, sizeof(DC_FRAME_T));
	
	//re-add the tail fragment
	if(pBufMgr->ring_begin + pBufMgr->tail_fragment_len >= pBufMgr->uplimited_addr)
	{
		pBufMgr->remain_len += pBufMgr->tail_fragment_len;
		pBufMgr->tail_fragment_len = 0;  //clear tail
		pBufMgr->ring_begin = pBufMgr->base_addr;//recover to original
	}

       //SCI_TRACE_LOW:"DC_FrameMgr_F, pBufMgr->tail_fragment_len:%d, remain:%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_FRAMEMGR_227_112_2_18_1_23_30_113,(uint8*)"dd",pBufMgr->tail_fragment_len, pBufMgr->remain_len);
	SCI_RestoreIRQ();

      

	return SCI_SUCCESS;
}



