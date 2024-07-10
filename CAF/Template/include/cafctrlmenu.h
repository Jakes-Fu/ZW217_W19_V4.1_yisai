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

#ifndef  _CAFGUIMENU_H_    
#define  _CAFGUIMENU_H_  

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

typedef struct  IGUIMENU_T   IGUIMENU_T;

//menu item
typedef struct
{
    CAF_STRING_T        *item_text_ptr; //item string
    uint32				select_icon_id; //item selected icon id
} CAF_GUIMENU_DYNA_ITEM_T;


CAF_INTERFACE (IGUIMENU_T)
{
    INHERIT_IGUICTRL;// ¼Ì³Ð»ùÀà
 
    BOOLEAN (*InsertNode)( IGUIMENU_T *me_ptr, uint16 nodex_index, uint32 node_id, uint32 parent_node_id, CAF_GUIMENU_DYNA_ITEM_T *node_item_ptr );
    uint32 (*GetCurNodeId)( IGUIMENU_T *me_ptr );
    BOOLEAN (*SetBackgroundColor) ( IGUIMENU_T *me_ptr, CAF_COLOR_T bg_color );   
	
};

struct IGUIMENU_T
{
    CAF_VTBL_T(IGUIMENU_T)        *pvt;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
static __inline uint32 IGUIMENU_AddRef( IGUIMENU_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->AddRef( (IGUICTRL_T *)me_ptr );
}

static __inline uint32 IGUIMENU_Release( IGUIMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->Release( (IGUICTRL_T *)me_ptr );
}

static __inline CAF_GUID_T IGUIMENU_GetCtrlGuid( IGUIMENU_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
}

static __inline CAF_HANDLE_T IGUIMENU_GetCtrlHandle( IGUIMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
}

static __inline BOOLEAN IGUIMENU_SetProperties( IGUIMENU_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
}

static __inline CAF_GUICTRL_PROPERTIES_T IGUIMENU_GetProperties ( IGUIMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->GetProperties( (IGUICTRL_T *)me_ptr );
}

static __inline BOOLEAN IGUIMENU_TestProperties ( IGUIMENU_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
}

static __inline void IGUIMENU_SetRect( IGUIMENU_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
}

static __inline void IGUIMENU_GetRect( IGUIMENU_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
}

static __inline void IGUIMENU_Redraw( IGUIMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->Redraw( (IGUICTRL_T *)me_ptr );
}

static __inline BOOLEAN IGUIMENU_SetActive( IGUIMENU_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
}

static __inline BOOLEAN IGUIMENU_IsActive( IGUIMENU_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->IsActive( (IGUICTRL_T *)me_ptr );
}

static __inline CAF_HANDLE_RESULT_E IGUIMENU_HandleEvent( IGUIMENU_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
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
static __inline BOOLEAN IGUIMENU_InsertNode( IGUIMENU_T *me_ptr, uint16 nodex_index, uint32 node_id, uint32 parent_node_id, CAF_GUIMENU_DYNA_ITEM_T *node_item_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->InsertNode( me_ptr, nodex_index, node_id, parent_node_id, node_item_ptr );
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
static __inline uint32 IGUIMENU_GetCurNodeId( IGUIMENU_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->GetCurNodeId( me_ptr );
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
static __inline BOOLEAN IGUIMENU_SetBackgroundColor( IGUIMENU_T *me_ptr, CAF_COLOR_T bg_color )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIMENU_T )->SetBackgroundColor( me_ptr, bg_color );
}




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIMENU_H_
