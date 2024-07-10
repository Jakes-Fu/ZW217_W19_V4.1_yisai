/****************************************************************************
** File Name:      cafapplet_gen.c                                         *
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
#define CAFAPPLET_GEN_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafapplet_gen.h"
#include "cafhelp.h"
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : applet query interface
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AppletQueryInterface(
                                        IAPPLET_T* iapplet_ptr,
                                        CAF_GUID_T guid,
                                        void**     ojbect_pptr
                                        );

/*****************************************************************************/
// 	Description : app add ref
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 AppletAddRef(
                          IAPPLET_T * iapplet_ptr
                          );

/*****************************************************************************/
// 	Description : applet release
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 AppletRelease(
                           IAPPLET_T * iapplet_ptr
                           );

/*****************************************************************************/
// 	Description : applet set applet handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletSetAppletHandle( 
                                    IAPPLET_T*   iapplet_ptr,
                                    CAF_HANDLE_T applet_handle
                                    );

/*****************************************************************************/
// 	Description : applet set applet handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_T AppletGetAppletHandle( 
                                         IAPPLET_T*   iapplet_ptr
                                         );

/*****************************************************************************/
// 	Description : applet new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E CAFAPPLET_New( 
                                  uint32                size,
                                  CAF_GUID_T            guid,
                                  IMODULE_T*            imodule_ptr, 
                                  IAPPLET_T**           iapplet_pptr, 
                                  APPLET_HANDLEEVENT    app_handle_func,
                                  APPLET_QUERYINTERFACE app_query_func,
                                  APPLET_FREEDATA       app_free_func
                                  )
{
    CAF_APPLET_T *  caf_applet_ptr = PNULL;
    CAF_VTBL_T(IAPPLET_T)* vtbl_ptr = PNULL;
    
    if ( PNULL == imodule_ptr
        || PNULL == app_handle_func
        || PNULL == iapplet_pptr )
    {
        return CAF_RESULT_FAILED;
    }

    *iapplet_pptr = PNULL;
    
    if(size < sizeof(CAF_APPLET_T))
    {
        size += sizeof(CAF_APPLET_T);
    }
    
    //malloc applet memory
    if (PNULL == (caf_applet_ptr = (CAF_APPLET_T*)CAF_ALLOC(size + sizeof(CAF_VTBL_T(IAPPLET_T)))))
    {
        return CAF_RESULT_NO_MEMORY;
    }
    
    CAF_MEMSET( caf_applet_ptr, 0, (size + sizeof(CAF_VTBL_T(IAPPLET_T))) );/*lint !e718 !e746*/
    
    vtbl_ptr = (CAF_VTBL_T(IAPPLET_T) *)((uint8 *)caf_applet_ptr + size);
    
    vtbl_ptr->QueryInterface  = AppletQueryInterface;
    vtbl_ptr->AddRef          = AppletAddRef;
    vtbl_ptr->Release         = AppletRelease;
    vtbl_ptr->HandleEvent     = app_handle_func;
    vtbl_ptr->SetAppletHandle = AppletSetAppletHandle;
    vtbl_ptr->GetAppletHandle = AppletGetAppletHandle;
    
    //initial vtbl
    caf_applet_ptr->vtbl_ptr = vtbl_ptr;
    
    caf_applet_ptr->guid         = guid;
    caf_applet_ptr->ref_num      = 1;
    caf_applet_ptr->module_ptr   = imodule_ptr;

    caf_applet_ptr->QueryInterface = app_query_func;
    caf_applet_ptr->FreeData       = app_free_func;
    
    IMODULE_AddRef(imodule_ptr);
    
    //output param
    *iapplet_pptr = (IAPPLET_T*)caf_applet_ptr;
    
    return CAF_RESULT_SUCCEEDED;
}

/*****************************************************************************/
// 	Description : applet query interface
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AppletQueryInterface( 
                                        IAPPLET_T* iapplet_ptr,
                                        CAF_GUID_T guid,
                                        void**     ojbect_pptr
                                        )
{
    CAF_RESULT_E result = CAF_RESULT_NOT_SUPPORT;
    
    CAF_APPLET_T* caf_applet_ptr = (CAF_APPLET_T*)iapplet_ptr;
    
    if ( IUNKNOWN_ID == guid || caf_applet_ptr->guid == guid )
    {
        *ojbect_pptr = iapplet_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    else
    {
        if( PNULL != caf_applet_ptr->QueryInterface )
        {
            result = caf_applet_ptr->QueryInterface( iapplet_ptr, guid, ojbect_pptr );
        }
        else
        {
            *ojbect_pptr = PNULL;
        }
    }
    
    if( CAF_RESULT_SUCCEEDED == result )
    {
        IUNKNOWN_AddRef( *ojbect_pptr );
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : app add ref
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 AppletAddRef(
                          IAPPLET_T * iapplet_ptr
                          )
{
    return ++(((CAF_APPLET_T *)iapplet_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : applet release
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 AppletRelease(
                           IAPPLET_T * iapplet_ptr
                           )
{   
    CAF_APPLET_T * caf_applet_ptr = (CAF_APPLET_T *)iapplet_ptr;
    
    //ref num--
    if ( --caf_applet_ptr->ref_num )
    {
        return( caf_applet_ptr->ref_num );
    }
    //free self
    else
    {
        if( PNULL != caf_applet_ptr->FreeData )
        {
            caf_applet_ptr->FreeData(iapplet_ptr);
        }
        
        IMODULE_Release(caf_applet_ptr->module_ptr);
        
        CAF_FREE(caf_applet_ptr);   
        
        return 0;
    }
}

/*****************************************************************************/
// 	Description : applet set applet handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletSetAppletHandle( 
                                    IAPPLET_T*   iapplet_ptr,
                                    CAF_HANDLE_T applet_handle
                                    )
{
    CAF_APPLET_T* caf_applet_ptr = (CAF_APPLET_T *)iapplet_ptr;
    
    caf_applet_ptr->app_handle = applet_handle;

    return TRUE;
}

/*****************************************************************************/
// 	Description : applet set applet handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_T AppletGetAppletHandle( 
                                         IAPPLET_T*   iapplet_ptr
                                         )
{
    CAF_APPLET_T* caf_applet_ptr = (CAF_APPLET_T *)iapplet_ptr;

    return caf_applet_ptr->app_handle;
}