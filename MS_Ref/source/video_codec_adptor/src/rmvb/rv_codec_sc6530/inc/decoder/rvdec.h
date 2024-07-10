
#ifndef _MPEG4_DEC_H_
#define _MPEG4_DEC_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "mmcodec.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: Init mpeg4 decoder	
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
MMDecRet RvDecInit(MMCodecBuffer * pBuffer,MMDecVideoFormat * pVideoFormat);

/*****************************************************************************/
//  Description: Init mpeg4 decoder	memory
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
PUBLIC MMDecRet RvDecMemInit(MMCodecBuffer *pBuffer);

/*****************************************************************************/
//  Description:   Calculate the size of external memory for Real decoder
//	Global resource dependence: 
//  Author:        
//	Note:          
/*****************************************************************************/
MMDecRet RVDecMemSizeCal(uint32 picWidth, uint32 picHeight, MMDecMemSize *mem_size);

/*****************************************************************************/
//  Description: Decode one vop	
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
MMDecRet RvDecDecode(MMDecInput *pInput,MMDecOutput *pOutput);

/*****************************************************************************/
//  Description: frame buffer no longer used for display
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
MMDecRet RV_DecReleaseDispBfr(uint8 *pBfrAddr);

/*****************************************************************************/
//  Description: Close mpeg4 decoder	
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
MMDecRet RvDecRelease(void);

/*****************************************************************************/
//  Description: check whether VSP can used for video decoding or not
//	Global resource dependence: 
//  Author:        
//	Note: return VSP status:
//        1: dcam is idle and can be used for vsp   0: dcam is used by isp           
/*****************************************************************************/
//BOOLEAN MPEG4DEC_VSP_Available (void);

/*****************************************************************************/
//  Description: for display, return one frame for display
//	Global resource dependence: 
//  Author:        
//	Note:  the transposed type is passed from MMI "req_transposed"
//         req_transposed£º 1£ºtranposed  0: normal    
/*****************************************************************************/
PUBLIC MMDecRet RvDecGetOneDispFrm(MMDecOutput *pOutput, int req_transposed, int is_last_frame);

PUBLIC MMDecRet RvDecReleaseDispBfr(uint8 *pFrameY);

/*****************************************************************************/
//  Description: get picture type of current frame 
//	Global resource dependence: 
//  Author:        
//	Note:  0: I(key) frame, 1: P frame, 2: B frame.
/*****************************************************************************/
PUBLIC int32 RvDec_GetPictureType(uint8 first_byte);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
