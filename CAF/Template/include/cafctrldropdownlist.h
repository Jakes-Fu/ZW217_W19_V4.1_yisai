/****************************************************************************
 ** File Name:       cafctrldropdownlist.h                                      *
 ** Author:           Andrew.Zhang                                          *
 ** Date:             10/03/2009                                            *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is caf head of dropdownlist control				*
 **                   												        *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                       *
 ** 09/2009      Andrew.Zhang       Create                                  *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUIDROPDOWNLIST_H_    
#define  _CAFGUIDROPDOWNLIST_H_  

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


typedef struct  IGUIDROPDOWNLIST_T   IGUIDROPDOWNLIST_T;

#define CAF_DROPDOWNLIST_STRING_MAX_NUM  56//28 //max string length of item 

//DropDownList的RECT风格
typedef enum
{ 
    CAF_DROPDOWNLIST_RECT_STYLE_NONE,
    CAF_DROPDOWNLIST_RECT_STYLE_SOLID, //3
    CAF_DROPDOWNLIST_RECT_STYLE_PLANE  //平面
}CAF_DROPDOWNLIST_RECT_STYLE_E;

//ITEM 结构
typedef struct
{
    wchar           wstr[CAF_DROPDOWNLIST_STRING_MAX_NUM];   // item string
    uint16          wstr_len;							// string length
}CAF_DROPDOWNLIST_ITEM_T;


CAF_INTERFACE (IGUIDROPDOWNLIST_T)
{
    INHERIT_IGUICTRL;// 继承基类
    
    BOOLEAN (*SetSelectedItem)( IGUIDROPDOWNLIST_T * me_ptr,  uint16 index );
    int16 (*GetSelectedItem)( IGUIDROPDOWNLIST_T * me_ptr );
    BOOLEAN (*LoadItemArray)( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_ITEM_T *item_array_ptr, uint16 total_num );
    BOOLEAN (*SetRectStyle)( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_RECT_STYLE_E style );
    BOOLEAN (*GetSelectedItemInfo)( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_ITEM_T *item_info_ptr );	
};

struct IGUIDROPDOWNLIST_T
{
    CAF_VTBL_T(IGUIDROPDOWNLIST_T)        *pvt;
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
static __inline uint32 IGUIDROPDOWNLIST_AddRef( IGUIDROPDOWNLIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUIDROPDOWNLIST_Release( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUIDROPDOWNLIST_GetCtrlGuid( IGUIDROPDOWNLIST_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUIDROPDOWNLIST_GetCtrlHandle( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIDROPDOWNLIST_SetProperties( IGUIDROPDOWNLIST_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUIDROPDOWNLIST_GetProperties ( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIDROPDOWNLIST_TestProperties ( IGUIDROPDOWNLIST_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUIDROPDOWNLIST_SetRect( IGUIDROPDOWNLIST_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIDROPDOWNLIST_GetRect( IGUIDROPDOWNLIST_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIDROPDOWNLIST_Redraw( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIDROPDOWNLIST_SetActive( IGUIDROPDOWNLIST_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUIDROPDOWNLIST_IsActive( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUIDROPDOWNLIST_HandleEvent( IGUIDROPDOWNLIST_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set selected item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:item index
//  Return:
//       TRUE:success:FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIDROPDOWNLIST_SetSelectedItem( IGUIDROPDOWNLIST_T * me_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->SetSelectedItem( me_ptr, index );
}

/*****************************************************************************/
//  Description:   get selected item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success:FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline int16 IGUIDROPDOWNLIST_GetSelectedItem( IGUIDROPDOWNLIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetSelectedItem( me_ptr );
}

/*****************************************************************************/
//  Description:   load item infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		item_array_ptr:item infomation array
//		total_num:array item count
//  Return:
//       TRUE:success:FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIDROPDOWNLIST_LoadItemArray( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_ITEM_T *item_array_ptr, uint16 total_num )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->LoadItemArray( me_ptr, item_array_ptr, total_num );
}

/*****************************************************************************/
//  Description:   set rect style
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		style:rect style
//  Return:
//       TRUE:success:FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIDROPDOWNLIST_SetRectStyle( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_RECT_STYLE_E style )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->SetRectStyle( me_ptr, style );
}

/*****************************************************************************/
//  Description:   get selected item infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		item_info_ptr:selected item infomation
//  Return:
//       TRUE:success:FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIDROPDOWNLIST_GetSelectedItemInfo( IGUIDROPDOWNLIST_T * me_ptr, CAF_DROPDOWNLIST_ITEM_T *item_info_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIDROPDOWNLIST_T )->GetSelectedItemInfo( me_ptr, item_info_ptr );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIDROPDOWNLIST_H_
