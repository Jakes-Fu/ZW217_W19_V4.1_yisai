/****************************************************************************
** File Name:      cafmodule_gen.c                                         *
** Author:         James.Zhang                                             *
** Date:           04/15/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/
#define CAFMODULE_GEN_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafhelp.h"
/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                   *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
const CAF_HELP_T *g_caf_help_ptr = PNULL;
#endif
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : module query interface
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E ModuleQueryInterface(
                                        IMODULE_T* imodule_ptr,
                                        CAF_GUID_T guid,
                                        void**     ojbect_pptr
                                        );

/*****************************************************************************/
// 	Description : module add ref
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ModuleAddRef(
                          IMODULE_T *imodule_ptr
                          );

/*****************************************************************************/
// 	Description : module release
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ModuleRelease(
                           IMODULE_T *imodule_ptr
                           );

/*****************************************************************************/
// 	Description : module load
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
#ifdef CAF_DYANMIC_MODULE
#ifdef WIN32
__declspec(dllexport)
#endif
CAF_RESULT_E CAFMODULE_Load( 
                            const void  *help_ptr, 
                            IMODULE_T   **mod_pptr 
                            )
{
    return CAFMODULE_New( sizeof(CAF_MODULE_T), help_ptr, mod_pptr, CAFMODULE_CreateInstance, NULL );
}
#endif
/*****************************************************************************/
// 	Description : module new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E CAFMODULE_New( 
                                  uint32           size,
                                  const void       *help_ptr,
                                  IMODULE_T        **mod_pptr,
                                  MODULECREATEINST mod_cretae_func,
                                  MODULEFREEDATA   mod_free_func
                                  )
{
    CAF_MODULE_T *caf_module_ptr = PNULL;
    CAF_VTBL_T(IMODULE_T)  *vbtl_ptr = PNULL;
    
    if ( PNULL == mod_pptr 
        || PNULL == mod_cretae_func /*|| PNULL == ishell_ptr*/) 
    {
        return CAF_RESULT_FAILED;
    }
    
    *mod_pptr = PNULL;

#if defined WIN32 && defined DYNAMIC_MODULE_SUPPORT
    if ( PNULL == help_ptr ) 
    {
        return CAF_RESULT_FAILED;
    } 
    else 
    {
        g_caf_help_ptr = (const CAF_HELP_T *)help_ptr;
    }
#endif
    
    if ( size < sizeof(CAF_MODULE_T)) 
    {
        size += sizeof(CAF_MODULE_T);
    }
    
    //malloc module memory
    if ( PNULL == ( caf_module_ptr = (CAF_MODULE_T *)CAF_ALLOC( size + sizeof(CAF_VTBL_T(IMODULE_T)) ) ) ) 
    {
        return CAF_RESULT_NO_MEMORY;
    }

    CAF_MEMSET( caf_module_ptr, 0, (size + sizeof(CAF_VTBL_T(IMODULE_T))) );/*lint !e516 !e718*/
    
    vbtl_ptr = (CAF_VTBL_T(IMODULE_T) *)( (uint8*)caf_module_ptr + size );

    vbtl_ptr->QueryInterface = ModuleQueryInterface;
    vbtl_ptr->AddRef         = ModuleAddRef;
    vbtl_ptr->Release        = ModuleRelease;
    vbtl_ptr->CreateInstance = mod_cretae_func;
    
    // initialize the vtable
    caf_module_ptr->vtbl_ptr = vbtl_ptr;
    
    caf_module_ptr->FreeData   = mod_free_func;
    
    caf_module_ptr->ref_num    = 1;
    
    //output param
    *mod_pptr = (IMODULE_T*)caf_module_ptr;
    
    return CAF_RESULT_SUCCEEDED;
}


/*****************************************************************************/
// 	Description : module query interface
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E ModuleQueryInterface( 
                                        IMODULE_T* imodule_ptr,
                                        CAF_GUID_T guid,
                                        void**     ojbect_pptr
                                        )
{
    CAF_RESULT_E result = CAF_RESULT_NOT_SUPPORT;
    
    if ( IUNKNOWN_ID == guid || IMODULE_ID == guid )
    {
        *ojbect_pptr = imodule_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    else
    {
        *ojbect_pptr = PNULL;
    }
    
    if( CAF_RESULT_SUCCEEDED == result )
    {
        IUNKNOWN_AddRef( *ojbect_pptr );
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : module add ref
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ModuleAddRef(
                          IMODULE_T *imodule_ptr
                          )
{    
    return ( ++((CAF_MODULE_T *)imodule_ptr)->ref_num );
}

/*****************************************************************************/
// 	Description : module release
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ModuleRelease(
                           IMODULE_T *imodule_ptr
                           )
{
    CAF_MODULE_T *caf_module_ptr = (CAF_MODULE_T *)imodule_ptr;
    
    //ref num--
    if ( --caf_module_ptr->ref_num ) 
    {
        return caf_module_ptr->ref_num;
    }
    //free self
    else
    {
        if ( caf_module_ptr->FreeData ) 
        {
            caf_module_ptr->FreeData( imodule_ptr );
        }
        
        CAF_FREE( caf_module_ptr );

        return 0;
    }
}
