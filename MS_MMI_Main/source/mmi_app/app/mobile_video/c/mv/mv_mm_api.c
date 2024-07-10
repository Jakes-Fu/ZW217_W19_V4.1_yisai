#define _MV_MM_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_mm_api.h"
#include "cfl_dlmalloc.h"
#include "cfl_mem.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MV_DLMALLOC_SIZE (10*1024*1024)
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL CFL_MSPACE_HANDLE s_mvbuf_mem_handle = 0;
LOCAL void* s_mvbuf_mem_ptr = 0;

#define MVBUF_ALLOC(_SIZE) CFL_MspaceMalloc(s_mvbuf_mem_handle, _SIZE)
#define MVBUF_FREE(_ptr)  CFL_MspaceFree(s_mvbuf_mem_handle, _ptr)
#define MVBUF_REALLOC(_ptr, _SIZE)   CFL_MspaceRealloc(s_mvbuf_mem_handle, _ptr, _SIZE)
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : mallc memory
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#if 1
_VOID* CM_Malloc_MV(_INT nSize, const char* file, uint32 line)
#else
_VOID* CM_Malloc(_INT nSize)
#endif

{
	_VOID* p = NULL;
	if(nSize >0)
	{
	    // p = malloc(nSize);
        p = MVBUF_ALLOC(nSize);
#ifdef MV_MM_DEBUG
        //MV_TRACE_LOW:"CM_Malloc_Mv: file=%s line=%d addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_56_112_2_18_2_39_13_66,(uint8*)"sdd", file, line, p);
#endif    
	    return p;     
			
	}
	else
	{
#ifdef MV_MM_DEBUG
		//MV_TRACE_LOW:"[MV]--WD:[CM_Malloc_MV] malloc memory error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_64_112_2_18_2_39_13_67,(uint8*)"");
#endif
		return NULL;
	}
	
}

/*****************************************************************************/
//  Description : realloc memory
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#if 1
_VOID* CM_Realloc_MV(_VOID*pOldBuffer, _INT nResize, const char * file, uint32 line)
#else
_VOID* CM_Realloc(_VOID*pOldBuffer, _INT nResize)
#endif
{
    _VOID* pNewBuff = PNULL;
    if (nResize > 0)
    {
        if(pOldBuffer!= NULL)
        {
            pNewBuff = (_VOID*)MVBUF_REALLOC(pOldBuffer,nResize);
        #ifdef MV_MM_DEBUG
            //MV_TRACE_LOW:"[MV]CM_Malloc_Mv: file=%s line=%d addr=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_88_112_2_18_2_39_13_68,(uint8*)"sdd", file, line, pNewBuff);
        #endif
            return pNewBuff;
        }
    	else
    	{
            pNewBuff=MVBUF_ALLOC(nResize);
            return pNewBuff;
    	}
    }
    else
    {
#ifdef MV_MM_DEBUG
    	//MV_TRACE_LOW:"[MV]--WD:[CM_Realloc] Realloc memory error!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_101_112_2_18_2_39_13_69,(uint8*)"");
#endif
        return NULL;
    }
}

/*****************************************************************************/
//  Description : free memory
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Free(_VOID* pBuffer)
{
    _BOOLEAN ret = _FALSE;

    if(pBuffer != NULL)
    {
#ifdef MV_MM_DEBUG
        //MV_TRACE_LOW:"[MV]CM_Free: addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_118_112_2_18_2_39_13_70,(uint8*)"d", pBuffer);
#endif
		MVBUF_FREE(pBuffer);	
		ret = _TRUE;
#ifdef MV_MM_DEBUG
		//MV_TRACE_LOW:"[MV]--WD:[CM_Free] free memory ok!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_123_112_2_18_2_39_13_71,(uint8*)"");
#endif
	}
    else
    {
		ret =_FALSE;
#ifdef MV_MM_DEBUG
	   //MV_TRACE_LOW:"[MV]--WD:[CM_Free] free memory error!"
	   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_130_112_2_18_2_39_13_72,(uint8*)"");
#endif
	}
	
	return ret;
}

/*****************************************************************************/
//  Description : memset
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID* CM_MemSet(_VOID* pBuffer,_BYTE c, _INT nSize)
{
	SCI_MEMSET(pBuffer,c,nSize);
#ifdef MV_MM_DEBUG
	//MV_TRACE_LOW:"[MV]--WD:[CM_MemSet] set memory"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_145_112_2_18_2_39_13_73,(uint8*)"");
#endif
	return pBuffer;
}


/*****************************************************************************/
//  Description : memory cpy
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID* CM_MemCopy(_VOID* pDes, _CONST _VOID* pSrc,_INT nSize)
{
	SCI_MEMCPY(pDes,pSrc,nSize);
#ifdef MV_MM_DEBUG
	//MV_TRACE_LOW:"[MV]--WD:[CM_MemCopy] memory copy"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_159_112_2_18_2_39_13_74,(uint8*)"");
#endif
	return pDes;
}

/*****************************************************************************/
//  Description : memory cmp
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MemCmp(_CONST _VOID* pDes, _CONST _VOID*pSrc,_INT nSize)
{
    if((NULL==pDes)||(NULL==pSrc))
    {
#ifdef MV_MM_DEBUG
        //MV_TRACE_LOW:"[MV]--WD:[CM_MemCmp] memory copy parameter error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_173_112_2_18_2_39_13_75,(uint8*)"");
#endif
		 return -1;
    }
    else
    {
#ifdef MV_MM_DEBUG
        //MV_TRACE_LOW:"[MV]--WD:[CM_MemCmp] memory copy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_180_112_2_18_2_39_13_76,(uint8*)"");
#endif
    }
    
    return memcmp(pDes, pSrc, nSize);
        
}

/*****************************************************************************/
//  Description :  memory move
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID* CM_MemMove(_CONST _VOID* pDes, _CONST _VOID* pSrc,_INT nSize)
{
	void *p = NULL;

    if(NULL == pDes || NULL == pSrc)
    {
    #ifdef MV_MM_DEBUG
	    //MV_TRACE_LOW:"[MV]--WD:[CM_MemMove] NULL == pDes || NULL == pSrc memory move Fail!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_199_112_2_18_2_39_14_77,(uint8*)"");
    #endif
        return (void *)pDes;
    }

	p = SCI_ALLOCA(nSize);
    if (PNULL != p)
    {
        SCI_MEMSET(p,0x00,nSize);
	    SCI_MEMCPY(p, pSrc, nSize);
	    SCI_MEMCPY((void *)pDes, p, nSize);
	    SCI_FREE(p);
    }
	
#ifdef MV_MM_DEBUG
	//MV_TRACE_LOW:"[MV]--WD:[CM_MemMove] memory move"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_214_112_2_18_2_39_14_78,(uint8*)"");
#endif
	return (void *)pDes;
}


/****************************************************************************/
//Description : Create dlmalloc handle for mv block.
//Global resource dependence : 
//Author: 
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MV_CreatMemHandle(void)
{   
#ifdef MV_MM_DEBUG    
    //MV_TRACE_LOW:"[MV] MV_CreatMemHandle!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_227_112_2_18_2_39_14_79,(uint8*)"");
#endif
	if(0 == s_mvbuf_mem_handle)
	{
        s_mvbuf_mem_ptr = CFL_ALLOC(MV_DLMALLOC_SIZE);
        if(s_mvbuf_mem_ptr != NULL)
        {
            s_mvbuf_mem_handle = CFL_CreateMspace(s_mvbuf_mem_ptr, MV_DLMALLOC_SIZE);
             if(0 == s_mvbuf_mem_handle)
             {
                goto clear;
             }
        }
        else
        {
            goto clear;
        }
    }
    else
    {
    #ifdef MV_MM_DEBUG
        //MV_TRACE_LOW:"[MV] MV_CreatMemHandle:Mem Handle has been initiated!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_248_112_2_18_2_39_14_80,(uint8*)"");
    #endif
    }   
	
  
    return TRUE;


clear:
#ifdef MV_MM_DEBUG
    //MV_TRACE_LOW:"[MV] MV_CreatMemHandle fail!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_258_112_2_18_2_39_14_81,(uint8*)"");
#endif
    MV_DeleteMemHandle();
    return FALSE;            
}

/****************************************************************************/
//Description : Free dlmalloc handle for mv block.
//Global resource dependence : 
//Author: 
//Note:	
/****************************************************************************/
PUBLIC void MV_DeleteMemHandle(void)
{
#ifdef MV_MM_DEBUG
    //MV_TRACE_LOW:"[MV] MV_DeleteMemHandle!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_MM_API_271_112_2_18_2_39_14_82,(uint8*)"");
#endif
    if(0 != s_mvbuf_mem_handle)
    {
        CFL_DestroyMspace(s_mvbuf_mem_handle);
        s_mvbuf_mem_handle = 0;
    }
    
    if(s_mvbuf_mem_ptr != NULL)
    {
        CFL_FREE(s_mvbuf_mem_ptr);
        s_mvbuf_mem_ptr = NULL;   
    }
    
}
