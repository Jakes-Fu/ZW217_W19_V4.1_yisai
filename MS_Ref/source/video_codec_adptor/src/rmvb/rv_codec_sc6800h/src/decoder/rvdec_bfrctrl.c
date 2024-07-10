
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
/*lint -save -e553 */
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

PUBLIC MMDecRet RvDec_InitYUVBfr(RV_DECODER_T *rv_decoder_ptr)
{
    int32 id;
    int32 size_y = rv_decoder_ptr->uFrameExtendWidth * rv_decoder_ptr->uFrameExtendHeight;	
    int32 size_c = size_y >> 2;
    DEC_FRM_BFR *pDecFrame;

	memset(g_RvFrmYUVBfr,0,sizeof(g_RvFrmYUVBfr));

//	if (size_y >= ((640+32)*(480+32)))
        if (rv_decoder_ptr->need_downsize)
	{
		pDecFrame = g_RvFrmYUVBfr+YUV_BUFFER_NUM;
		pDecFrame->imgYUV[0] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_y * sizeof(uint8));  //y
      	pDecFrame->imgYUV[1] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_c * sizeof(uint8));  //u
       	pDecFrame->imgYUV[2] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_c * sizeof(uint8));  //v
		
		size_y = size_c; // rv_decoder_ptr->uFrameExtendWidth_d * rv_decoder_ptr->uFrameExtendHeight_d;	
		size_c = size_y >> 2;
	}

    pDecFrame = g_RvFrmYUVBfr;
    for(id = 0; id < YUV_BUFFER_NUM; id++)
    {
        pDecFrame->id = id;    
        pDecFrame->bRef = FALSE;
        pDecFrame->bDisp = FALSE;

        pDecFrame->imgYUV[0] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_y * sizeof(uint8));  //y
      	pDecFrame->imgYUV[1] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_c * sizeof(uint8));  //u
       	pDecFrame->imgYUV[2] = (uint8 *)RvDec_ExtraMemAlloc_64WordAlign((uint32)size_c * sizeof(uint8));  //v
    
        if( (NULL == pDecFrame->imgYUV[0]) || (NULL == pDecFrame->imgYUV[1]) || (NULL == pDecFrame->imgYUV[2]))
        {
            return MMDEC_MEMORY_ERROR;
        }

	//	pDecFrame->imgYUVAddr[0] = (uint32)pDecFrame->imgYUV[0] >> 8;  //y
	//	pDecFrame->imgYUVAddr[1] = (uint32)pDecFrame->imgYUV[1] >> 8;  //u
	//	pDecFrame->imgYUVAddr[2] = (uint32)pDecFrame->imgYUV[2] >> 8;  //v

		pDecFrame++;
	}

	//for bottom line of mb of 16x16 size;
	g_bottom_line_bfr[0] = (uint8 *)RvDec_ExtraMemAlloc(rv_decoder_ptr->uFrameExtendWidth);
	g_bottom_line_bfr[1] = (uint8 *)RvDec_ExtraMemAlloc(rv_decoder_ptr->uFrameExtendWidth/2);
	g_bottom_line_bfr[2] = (uint8 *)RvDec_ExtraMemAlloc(rv_decoder_ptr->uFrameExtendWidth/2);

	return MMDEC_OK;
}

/**********************************************************
check is there free frame buffer, if has, return the frame buffer ID
************************************************************/
/*find a buffer which has been displayed and will no longer be a reference frame from g_dec_frame,
and set the buffer to be reference frame, if current frame picture type is not B frame */
LOCAL BOOLEAN RvDec_GetOneFreeBfr(RV_DECODER_T *rv_decoder_ptr)
{
	int id;
	
	for(id = 0; id < YUV_BUFFER_NUM; id++)
	{
		if((!g_RvFrmYUVBfr[id].bRef) && (!g_RvFrmYUVBfr[id].bDisp))
		{
			rv_decoder_ptr->pCurRecFrame->pDecFrame = &g_RvFrmYUVBfr[id];
			rv_decoder_ptr->pCurRecFrame->bfrId = id;

			break;
		}
	}

	if(id == YUV_BUFFER_NUM)
	{
		return FALSE;
	}else
	{
		return TRUE;
	}
}

/*allocate frame buffer for current frame before current frame decoding*/
PUBLIC BOOLEAN RvDec_GetCurRecFrameBfr(RV_DECODER_T *rv_decoder_ptr)
{
	while(!RvDec_GetOneFreeBfr(rv_decoder_ptr))
	{
		//wait for display free buffer
	//	PRINTF("no buffer is available!\n");
		return FALSE;
	}

	return TRUE;
}

/****************************************************
input: the address of Y frame to be released which has been displayed
output: set the corresponding frame's display type to 0
****************************************************/
PUBLIC MMDecRet RV_DecReleaseDispBfr(uint8 *pBfrAddr)
{
 	int i;
	SCI_ASSERT(pBfrAddr != NULL);/*assert verified*/

	for(i = 0; i < YUV_BUFFER_NUM; i++)
	{
		if(pBfrAddr == g_RvFrmYUVBfr[i].imgYUV[0])
		{
			g_RvFrmYUVBfr[i].bDisp = 0;
			break;
		}
	}

	if(YUV_BUFFER_NUM == i)
	{
		return MMDEC_ERROR;
	}

	return MMDEC_OK;
}

PUBLIC void RvDec_SetRefFlag ()
{

}

PUBLIC void RvDec_SetDspFlag ()
{

}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
