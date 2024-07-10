/****************************************************************************
 ** File Name:       cafctrliconlist.h                                      *
 ** Author:           Andrew.Zhang                                          *
 ** Date:             10/03/2009                                            *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is caf head of iconlist control				*
 **                   												        *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                       *
 ** 09/2009      Andrew.Zhang       Create                                  *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUIICONLIST_H_    
#define  _CAFGUIICONLIST_H_  

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
#include "cafctrlanim.h"


typedef struct  IGUIICONLIST_T   IGUIICONLIST_T;

#define CAF_ICONLIST_NAME_MAX_LEN    28  //icon list name max len

//icon info
typedef struct
{
    wchar      name_wstr[CAF_ICONLIST_NAME_MAX_LEN + 1];
    uint16     name_len;
    uint32     softkey_id[CAF_SOFTKEY_BUTTON_NUM];
} CAF_ICONLIST_ICON_DISP_T;

CAF_INTERFACE (IGUIICONLIST_T)
{
    INHERIT_IGUICTRL;// ¼Ì³Ð»ùÀà
    
	BOOLEAN (*SetTotalItem)( IGUIICONLIST_T * me_ptr, uint16 total_icon_num );
	BOOLEAN (*SetCurIndex)( IGUIICONLIST_T * me_ptr, uint16 selected_index );
	uint16 (*GetCurIndex)( IGUIICONLIST_T * me_pt );
	BOOLEAN (*AppendItem)(IGUIICONLIST_T * me_ptr, uint16 icon_index, CAF_ICONLIST_ICON_DISP_T *icon_ptr);
	BOOLEAN (*AppendIcon)(IGUIICONLIST_T * me_ptr, uint16 icon_index, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source);
};

struct IGUIICONLIST_T
{
    CAF_VTBL_T(IGUIICONLIST_T)        *pvt;
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
static __inline uint32 IGUIICONLIST_AddRef( IGUIICONLIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUIICONLIST_Release( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUIICONLIST_GetCtrlGuid( IGUIICONLIST_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUIICONLIST_GetCtrlHandle( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIICONLIST_SetProperties( IGUIICONLIST_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUIICONLIST_GetProperties ( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIICONLIST_TestProperties ( IGUIICONLIST_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUIICONLIST_SetRect( IGUIICONLIST_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIICONLIST_GetRect( IGUIICONLIST_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIICONLIST_Redraw( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIICONLIST_SetActive( IGUIICONLIST_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUIICONLIST_IsActive( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->IsActive( (IGUICTRL_T *)me_ptr );
}

/*****************************************************************************/
//  Description:   handle control event
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		msg_id:message id
//		param_ptr:message param
//  Return:
//       CAF_HANDLE_RESULT_E
//  Note: 
/*****************************************************************************/
static __inline CAF_HANDLE_RESULT_E IGUIICONLIST_HandleEvent( IGUIICONLIST_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set total item count
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		total_icon_num:total item count
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIICONLIST_SetTotalItem( IGUIICONLIST_T * me_ptr, uint16 total_icon_num )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->SetTotalItem( me_ptr, total_icon_num );
}

/*****************************************************************************/
//  Description:   set current index
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		selected_index:index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIICONLIST_SetCurIndex( IGUIICONLIST_T * me_ptr, uint16 selected_index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->SetCurIndex( me_ptr, selected_index );
}

/*****************************************************************************/
//  Description:   get current index
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
static __inline uint16 IGUIICONLIST_GetCurIndex( IGUIICONLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->GetCurIndex( me_ptr );
}

/*****************************************************************************/
//  Description:   add icon item display infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		icon_index:icon item index
//		icon_ptr:icon display infomation
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIICONLIST_AppendItem( IGUIICONLIST_T * me_ptr, uint16 icon_index, CAF_ICONLIST_ICON_DISP_T *icon_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->AppendItem( me_ptr, icon_index, icon_ptr );
}

/*****************************************************************************/
//  Description:   add icon item data infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		icon_index:icon item index
//		data_ptr:icon item data
//		resource_source:icon data source type
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIICONLIST_AppendIcon( IGUIICONLIST_T * me_ptr, uint16 icon_index, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIICONLIST_T )->AppendIcon( me_ptr, icon_index, data_ptr, resource_source );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIICONLIST_H_
