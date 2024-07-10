/****************************************************************************
** File Name:      cafmodule.h                                             *
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

#ifndef  _CAF_MODULE_H_    
#define  _CAF_MODULE_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct IMODULE_T IMODULE_T;

#define INHERIT_IMODULE( _iname_ ) \
   INHERIT_IUNKNOWN( _iname_ ); \
   CAF_RESULT_E  (*CreateInstance)( _iname_ * me, CAF_GUID_T guid, void ** object_pptr )

CAF_INTERFACE(IMODULE_T)
{
    INHERIT_IMODULE(IMODULE_T);
};

struct IMODULE_T
{
    CAF_VTBL_T(IMODULE_T)  *vtbl_ptr;
};

typedef CAF_RESULT_E (*MODULEENTRY)( const void * help_ptr, IMODULE_T** module_pptr );


/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

static __inline CAF_RESULT_E IMODULE_QueryInterface( IMODULE_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IMODULE_T )->QueryInterface( me, guid, object_pptr );
}

static __inline uint32 IMODULE_AddRef( IMODULE_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IMODULE_T )->AddRef( me );
}

static __inline uint32 IMODULE_Release( IMODULE_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IMODULE_T )->Release( me );
}

static __inline CAF_HANDLE_RESULT_E IMODULE_CreateInstance( IMODULE_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IMODULE_T )->CreateInstance( me, guid, object_pptr );
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
