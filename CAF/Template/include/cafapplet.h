/****************************************************************************
** File Name:      cafapplet.h                                             *
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

#ifndef  _CAF_APPLET_H_    
#define  _CAF_APPLET_H_  

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
#define INHERIT_IAPPLET( _iname_ ) \
   INHERIT_IUNKNOWN( _iname_ ); \
   CAF_HANDLE_RESULT_E  (*HandleEvent)( _iname_ * me, CAF_MESSAGE_ID_E msg_id, void* param ); \
   BOOLEAN              (*SetAppletHandle)( _iname_ * me, CAF_HANDLE_T applet_handle );\
   CAF_HANDLE_T         (*GetAppletHandle)( _iname_ * me )

CAF_INTERFACE(IAPPLET_T)
{
    INHERIT_IAPPLET(IAPPLET_T);
};

struct IAPPLET_T
{
   CAF_VTBL_T(IAPPLET_T)        *pvt;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

static __inline CAF_RESULT_E IAPPLET_QueryInterface( IAPPLET_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->QueryInterface( me, guid, object_pptr );
}

static __inline uint32 IAPPLET_AddRef( IAPPLET_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->AddRef( me );
}

static __inline uint32 IAPPLET_Release( IAPPLET_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->Release( me );
}

static __inline CAF_HANDLE_RESULT_E IAPPLET_HandleEvent( IAPPLET_T *me, CAF_MESSAGE_ID_E msg_id, void* param ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->HandleEvent( me, msg_id, param );
}

static __inline BOOLEAN IAPPLET_SetAppletHandle( IAPPLET_T *me, CAF_HANDLE_T applet_handle ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->SetAppletHandle( me, applet_handle );
}

static __inline CAF_HANDLE_T IAPPLET_GetAppletHandle( IAPPLET_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IAPPLET_T )->GetAppletHandle( me );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
