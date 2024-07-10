/******************************************************************************
 ** File Name:    dc_cm.h  			                                          *
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

#ifndef _DC_CM_H_                                                           
#define _DC_CM_H_                                                           
                                                                                
/**---------------------------------------------------------------------------* 
 **                          Compiler Flag                                    * 
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert start of extern C construct */                    
extern "C" {                                                                    
#endif                                                                                                                                                                                             
                                                                                                                  
__inline void dcRotCvtWH(uint32 width, uint32 height, uint32 rotAngle,          
	ROTATION_PARAM_T * pRotPara)                                                
{                                                                                             
 	pRotPara->img_size.w = width ;                                          
  	pRotPara->img_size.h = height;                                          
}                                                                               
                                                                                
__inline void dcRotCvtAddr(DCAMERA_YUV_CHN_ADDR_T *pSrc,                        
	DCAMERA_YUV_CHN_ADDR_T *pDst, ROTATION_PARAM_T *pRotPara)                   
{                                                                               
	pRotPara->src_addr.y_addr	= pSrc->y_chn_addr;                             
	pRotPara->src_addr.uv_addr	= pSrc->u_chn_addr;                             
	pRotPara->src_addr.v_addr	= pSrc->v_chn_addr;	                            
                                                                                
	pRotPara->dst_addr.y_addr	= pDst->y_chn_addr;                             
	pRotPara->dst_addr.uv_addr	= pDst->u_chn_addr;                             
	pRotPara->dst_addr.v_addr	= pDst->v_chn_addr;                             
}                                                                               
                                                                                                                                        
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* Insert end of extern C construct 	*/                  
}                   /* The C header file can now be 		*/                  
#endif              /* included in either C or C++ code. 	*/                  
                                                                                
#endif                                                                          
                                                                                