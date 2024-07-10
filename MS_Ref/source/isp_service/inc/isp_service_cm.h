/******************************************************************************
 ** File Name:    isp_service_cm.h                                            *
 ** Author:       Michael Guo                                                 *
 ** DATE:         10/29/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/

/******************************************************************************
 **                   Edit    History                                         *
 ******************************************************************************
 ** DATE           NAME            DESCRIPTION                       Version  *
 ** 10/29/2010     Michael Guo     Initial version 				     1.0      *
 ******************************************************************************/

#ifndef _ISP_SERVICE_CM_H_
#define _ISP_SERVICE_CM_H_

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert start of extern C construct */
extern "C" {
#endif


__inline void ispRotCvtFmt(uint32 fmt, ROTATION_PARAM_T * pRotPara)
{	
	switch(fmt)
	{
	    case ISP_IMAGE_FORMAT_RGB565:
	        pRotPara->data_format = ROTATION_RGB565;
	        break;
	    case ISP_IMAGE_FORMAT_YUV422:
	        pRotPara->data_format = ROTATION_YUV422;
	        break;
	    case ISP_IMAGE_FORMAT_YUV420:
	        pRotPara->data_format = ROTATION_YUV420;
	        break;	
		case ISP_IMAGE_FORMAT_RGB888:
	        pRotPara->data_format = ROTATION_RGB888;
	        break;
	    case ISP_IMAGE_FORMAT_RGB666:
	        pRotPara->data_format = ROTATION_RGB666;
	        break;
		case ISP_IMAGE_FORMAT_YUV400:
		case ISP_IMAGE_FORMAT_YUV420_3FRAME:
	        pRotPara->data_format = ROTATION_YUV400;
	        break;
	    default:
	        pRotPara->data_format = ROTATION_MAX;
	        break;            			
	}		
} 

__inline void ispRotCvtWH(uint32 width, uint32 height, uint32 rotAngle, 
	ROTATION_PARAM_T * pRotPara)
{
 	pRotPara->img_size.w = width ;
  	pRotPara->img_size.h = height;
}

__inline void ispRotCvtAddr(ISP_ADDRESS_T *pSrc, ISP_ADDRESS_T *pDst, 
	ROTATION_PARAM_T *pRotPara)
{
	pRotPara->src_addr.y_addr	= pSrc->yaddr;
	pRotPara->src_addr.uv_addr	= pSrc->uaddr;
	pRotPara->src_addr.v_addr	= pSrc->vaddr;	

	pRotPara->dst_addr.y_addr	= pDst->yaddr;
	pRotPara->dst_addr.uv_addr	= pDst->uaddr;
	pRotPara->dst_addr.v_addr	= pDst->vaddr;		
}

__inline void ispRotYUV4203Frame(uint32 width, uint32 height, 
	ROTATION_PARAM_T *pRotPara)
{
 	pRotPara->img_size.w		= width/2 ;
  	pRotPara->img_size.h		= height/2;
  	
	pRotPara->src_addr.y_addr	= pRotPara->src_addr.uv_addr;
	pRotPara->dst_addr.y_addr	= pRotPara->dst_addr.uv_addr;		
	
	if(!Rotation_AppStart(pRotPara))
		SCI_TRACE_LOW("ISP_SERVICE: _ISP_ServiceRotateImage-U, SUCCESS");
	else
	{	
		SCI_TRACE_LOW("ISP_SERVICE: _ISP_ServiceRotateImage-U, FAIL");
		goto exitflg;
	}
	
	pRotPara->src_addr.y_addr	= pRotPara->src_addr.v_addr;
	pRotPara->dst_addr.y_addr	= pRotPara->dst_addr.v_addr;		
	
	if(!Rotation_AppStart(pRotPara))
		SCI_TRACE_LOW("ISP_SERVICE: _ISP_ServiceRotateImage-V, SUCCESS");
	else
	{	
		SCI_TRACE_LOW("ISP_SERVICE: _ISP_ServiceRotateImage-V, FAIL");
		goto exitflg;
	}
	
exitflg:
	return;			
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert end of extern C construct 	*/
}                   /* The C header file can now be 		*/
#endif              /* included in either C or C++ code. 	*/


/**---------------------------------------------------------------------------*/
#endif	//#ifndef _ISP_SERVICE_CM_H_
// End
