/****************************************************************************
 ** File Name:      cafheap.h                                               *
 ** Author:         wei.zhou                                                *
 ** Date:           03/02/2009                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the application modual.   *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 03/2009        wei.zhou           Create
 ** 
 ****************************************************************************/

#ifndef  _CAFHEAP_H_    
#define  _CAFHEAP_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct IHEAP_T IHEAP_T;


CAF_INTERFACE(IHEAP_T)
{
    INHERIT_IUNKNOWN(IHEAP_T);
    void     *(*Malloc)(IHEAP_T * heap_ptr, uint32 size);
    void     (*Free)(IHEAP_T * heap_ptr, void * memFree_ptr);
};

// Define pointer to point the virtual table of the control base class
struct IHEAP_T
{
    CAF_VTBL_T(IHEAP_T)  *vtbl_ptr;
};

/*****************************************************************************/
// 	Description : heap query interface
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline CAF_RESULT_E IHEAP_QueryInterface( IHEAP_T *heap_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != heap_ptr );
    return CAF_GET_VTBLPTR( heap_ptr, IHEAP_T )->QueryInterface( heap_ptr, guid, object_pptr );
}

/*****************************************************************************/
// 	Description : heap add ref
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 IHEAP_AddRef( IHEAP_T *heap_ptr ) 
{
    CAF_ASSERT( PNULL != heap_ptr );
    return CAF_GET_VTBLPTR( heap_ptr, IHEAP_T )->AddRef( heap_ptr );
}

/*****************************************************************************/
// 	Description : heap release
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 IHEAP_Release( IHEAP_T *heap_ptr ) 
{
    CAF_ASSERT( PNULL != heap_ptr );
    return CAF_GET_VTBLPTR( heap_ptr, IHEAP_T )->Release( heap_ptr );
}

/*****************************************************************************/
// 	Description : heap room allocate
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline void *IHEAP_Malloc(IHEAP_T * heap_ptr, uint32 size)
{
    CAF_ASSERT( PNULL != heap_ptr );
    return CAF_GET_VTBLPTR( heap_ptr, IHEAP_T )->Malloc( heap_ptr, size);
}

/*****************************************************************************/
// 	Description : heap room free
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline void IHEAP_Free(IHEAP_T * heap_ptr, void * memFree_ptr)
{
    CAF_ASSERT( PNULL != heap_ptr );
    CAF_GET_VTBLPTR( heap_ptr, IHEAP_T )->Free( heap_ptr, memFree_ptr);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFTAPI_H_
