#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafheap.c                                               *
** Author:         Wei.Zhou                                                *
** Date:           02/06/2009                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/2008        Wei.Zhou          Create                                 *
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafheap.h"
#include "mmk_app.h"
#include "mmk_module_manager.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/

typedef struct
{
    CAF_VTBL_T(IHEAP_T)  *vtbl_ptr; // º¯ÊýÁÐ±í
    CAF_GUID_T       guid;
    uint32           ref_num;
}CAFHEAP_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : heap class created.
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFHEAP_ClassNew(CAF_GUID_T guid, void** object_pptr);

/*****************************************************************************/
// 	Description : heap query interface
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFHEAP_QueryInterface(IHEAP_T * heap_ptr, CAF_GUID_T guid, void ** object_pptr );

/*****************************************************************************/
// 	Description : heap add ref
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFHEAP_AddRef(IHEAP_T * heap_ptr);

/*****************************************************************************/
// 	Description : heap release
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFHEAP_Release(IHEAP_T * heap_ptr);

/*****************************************************************************/
// 	Description : heap room allocate
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
LOCAL void *CAFHEAP_Malloc(IHEAP_T * heap_ptr, uint32 size);

/*****************************************************************************/
// 	Description : heap room free
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
LOCAL void CAFHEAP_Free(IHEAP_T * heap_ptr, void * memFree_ptr);


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// functions table

LOCAL const CAF_VTBL_T(IHEAP_T) s_heap_vtbl =
{
    CAFHEAP_QueryInterface,
    CAFHEAP_AddRef,
    CAFHEAP_Release,
    CAFHEAP_Malloc,
    CAFHEAP_Free
};


LOCAL const CAF_STATIC_CLASS_T s_heap_class_list[] = 
{
    { SPRD_GUID_HEAP,      CAFHEAP_ClassNew,        0 }
};

const CAF_STATIC_CLASS_INFO_T g_heap_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_heap_class_list, 
    sizeof(s_heap_class_list) / sizeof(s_heap_class_list[0])
};

/*****************************************************************************/
// 	Description : heap class created.
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFHEAP_ClassNew( 
                                      CAF_GUID_T guid, 
                                      void**     object_pptr 
                                      )
{
    CAF_RESULT_E  result  = CAF_RESULT_NOT_SUPPORT;
    CAFHEAP_T     *cafheap_ptr = PNULL;
    
    if( SPRD_GUID_HEAP == guid )
    {
        cafheap_ptr = SCI_ALLOC_APP(sizeof(CAFHEAP_T));
        
        SCI_MEMSET((char *)cafheap_ptr, 0, sizeof(CAFHEAP_T));
        cafheap_ptr->vtbl_ptr = (CAF_VTBL_T(IHEAP_T)*)&s_heap_vtbl;
        
        cafheap_ptr->guid       = guid;
        cafheap_ptr->ref_num    = 1;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    *object_pptr = cafheap_ptr;
    return result;
}

/*****************************************************************************/
// 	Description : heap query interface
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFHEAP_QueryInterface(IHEAP_T * heap_ptr, CAF_GUID_T guid, void ** object_pptr )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAFHEAP_T *cafheap_ptr = (CAFHEAP_T *)heap_ptr;
    
    if ( IUNKNOWN_ID == guid ||  cafheap_ptr->guid == guid )
    {
        *object_pptr = heap_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    if( result )
    {
        IUNKNOWN_AddRef( *object_pptr );
    }
    return result;
}

/*****************************************************************************/
// 	Description : heap add ref
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFHEAP_AddRef(IHEAP_T * heap_ptr)
{
    return ++(((CAFHEAP_T *)heap_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : heap release
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFHEAP_Release(IHEAP_T * heap_ptr)
{
    CAFHEAP_T * cafheap_ptr = (CAFHEAP_T *)heap_ptr;
    
    if ( --cafheap_ptr->ref_num )
    {
        return( cafheap_ptr->ref_num );
    }
    else
    {
        SCI_FREE( cafheap_ptr );
        return 0;
    }
}


/*****************************************************************************/
// 	Description : heap room allocate
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
LOCAL void *CAFHEAP_Malloc(IHEAP_T * heap_ptr, uint32 size)
{
    CAFHEAP_T     *cafheap_ptr;

    SCI_ASSERT(heap_ptr);
    SCI_ASSERT(((CAFHEAP_T *)heap_ptr)->ref_num);
    cafheap_ptr = (CAFHEAP_T *)heap_ptr;

    return (void *)MMKMNG_MemMalloc(size);
}

/*****************************************************************************/
// 	Description : heap room free
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
void CAFHEAP_Free(IHEAP_T * heap_ptr, void * memFree_ptr)
{
    SCI_ASSERT(heap_ptr);
    SCI_ASSERT(((CAFHEAP_T *)heap_ptr)->ref_num);

    MMKMNG_MemFree((uint8*) memFree_ptr);
}
#endif
