
#ifndef _RVDEC_MALLOC_H_
#define _RVDEC_MALLOC_H_
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_basic.h"
#include "rvdec_mode.h"
#include "mmcodec.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
		
PUBLIC void *RvDec_ExtraMemAlloc(uint32 mem_size);
PUBLIC void *RvDec_ExtraMemAlloc_64WordAlign(uint32 mem_size);
PUBLIC void *RvDec_InterMemAlloc(uint32 mem_size);
PUBLIC void RvDec_FreeMem(void); 
PUBLIC void Rv4Dec_FreeExtraMem(void);
PUBLIC void RvDec_InitInterMem(MMCodecBuffer *pBuffer);
PUBLIC MMDecRet RvDecMemInit(MMCodecBuffer *pBuffer);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif // _RVDEC_MALLOC_H_
