/****************************************************************************
 ** File Name:       cafctrlmenu.h                                           			  *
 ** Author:           Andrew.Zhang                                             		  *
 ** Date:             05/05/2009                                              			  *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is caf head of button control					  *
 **                   												  *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                       *
 ** 09/2009      Andrew.Zhang       Create                                 *
 **                                                                                          *
 ****************************************************************************/

#ifndef  _CAFGUIPOPMENU_H_    
#define  _CAFGUIPOPMENU_H_  

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
#include "cafcontrol.h"
#include "caf.h"
#include "cafdc.h"
#include "cafctrlmenu.h"


typedef struct  IGUIPOPMENU_T   IGUIPOPMENU_T;


CAF_INTERFACE (IGUIPOPMENU_T)
{
    INHERIT_IGUICTRL;// ¼Ì³Ð»ùÀà

    BOOLEAN (*InsertNode)( IGUIPOPMENU_T *me_ptr, uint16 nodex_index, uint32 node_id, uint32 parent_node_id, CAF_GUIMENU_DYNA_ITEM_T *node_item_ptr );
    uint32 (*GetCurNodeId)( IGUIPOPMENU_T *me_ptr );
    BOOLEAN (*SetBackgroundColor) ( IGUIPOPMENU_T *me_ptr, CAF_COLOR_T bg_color );   
};

struct IGUIPOPMENU_T
{
    CAF_VTBL_T(IGUIPOPMENU_T)        *pvt;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   increase reference count of the control
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       uint32: the last reference count 
//  Note: 
/*****************************************************************************/
static __inline uint32 IGUIPOPMENU_AddRef( IGUIPOPMENU_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->AddRef( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   decrease reference count of the control
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       uint32: the last reference count 
//  Note: 
/*****************************************************************************/
static __inline uint32 IGUIPOPMENU_Release( IGUIPOPMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->Release( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   get control guid
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       control guid
//  Note: 
/*****************************************************************************/
static __inline CAF_GUID_T IGUIPOPMENU_GetCtrlGuid( IGUIPOPMENU_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   get control handle
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       control handle
//  Note: 
/*****************************************************************************/
static __inline CAF_HANDLE_T IGUIPOPMENU_GetCtrlHandle( IGUIPOPMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   set control properties
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		properties:control properties type
//		is_true:TRUE,set properties;FALSE,clear properties
//  Return:
//       TRUE:success; FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_SetProperties( IGUIPOPMENU_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
}

/*****************************************************************************/
//  Description:   get control properties
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       control properties
//  Note: 
/*****************************************************************************/
static __inline CAF_GUICTRL_PROPERTIES_T IGUIPOPMENU_GetProperties ( IGUIPOPMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->GetProperties( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   test control properties
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		properties:properties by tested
//  Return:
//       TRUE:all properties tested are exist;otherwise,FALSE
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_TestProperties ( IGUIPOPMENU_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
}

/*****************************************************************************/
//  Description:   set control rectangle
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		rect_ptr:control rectangle
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUIPOPMENU_SetRect( IGUIPOPMENU_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
}

/*****************************************************************************/
//  Description:   get control rectangle
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		rect_ptr:control rectangle
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUIPOPMENU_GetRect( IGUIPOPMENU_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
}

/*****************************************************************************/
//  Description:   notify control paint again
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUIPOPMENU_Redraw( IGUIPOPMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->Redraw( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   set control state of active
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		is_true :TRUE,set control active state;FALSE,set control unactive state
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_SetActive( IGUIPOPMENU_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
}

/*****************************************************************************/
//  Description:   get control state of active
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:active;FALSE:unactive
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_IsActive( IGUIPOPMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->IsActive( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   handle control event
//  Author: Andrew.Zhang
//  Param
//      me_ptr :control object
//		msg_id:message id
//		param_ptr:message param
//  Return:
//       CAF_HANDLE_RESULT_E
//  Note: 
/*****************************************************************************/
static __inline CAF_HANDLE_RESULT_E IGUIPOPMENU_HandleEvent( IGUIPOPMENU_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   insert node
//  Author: Andrew.Zhang
//  Param
//      me_ptr :control object
//		nodex_index :node index,from 0
//		node_id :node id,from 1
//		parent_node_id :parent node id
//		node_item_ptr :node item
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_InsertNode( IGUIPOPMENU_T *me_ptr, uint16 nodex_index, uint32 node_id, uint32 parent_node_id, CAF_GUIMENU_DYNA_ITEM_T *node_item_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->InsertNode( me_ptr, nodex_index, node_id, parent_node_id, node_item_ptr );
}


/*****************************************************************************/
//  Description:   handle control event
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       current node id
//  Note: 
/*****************************************************************************/
static __inline uint32 IGUIPOPMENU_GetCurNodeId( IGUIPOPMENU_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->GetCurNodeId( me_ptr );
}

/*****************************************************************************/
//  Description:   set menu background color
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//       bg_color :background color
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIPOPMENU_SetBackgroundColor( IGUIPOPMENU_T *me_ptr, CAF_COLOR_T bg_color )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIPOPMENU_T )->SetBackgroundColor( me_ptr, bg_color );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIPOPMENU_H_
