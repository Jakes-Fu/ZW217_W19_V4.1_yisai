/****************************************************************************
 ** File Name:      cafctrledit.h                                           *
 ** Author:         James.Zhang                                             *
 ** Date:           08/08/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the base class of the GUI *
 **                 control, all other controls will inherit this class.    *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 08/2008        James.Zhang       Create                                 *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUIEDIT_H_    
#define  _CAFGUIEDIT_H_  

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

typedef struct  IGUIEDIT_T   IGUIEDIT_T;

//edit style
typedef enum
{
    CAF_GUIEDIT_STYLE_NULL,
    CAF_GUIEDIT_STYLE_SINGLE,	//单行edit,没有滚动条
	CAF_GUIEDIT_STYLE_MULTI,	//多行edit,有滚动条
	CAF_GUIEDIT_STYLE_ADAPTIVE,	//多行edit,没有滚动条,内嵌在组合控件中中
    CAF_GUIEDIT_STYLE_MAX
} CAF_GUIEDIT_STYLE_E;

CAF_INTERFACE (IGUIEDIT_T)
{
    INHERIT_IGUICTRL;// 继承基类

    BOOLEAN (*SetText)     ( IGUIEDIT_T *me_ptr, const CAF_STRING_T* str_ptr );
    BOOLEAN (*GetText)     ( IGUIEDIT_T *me_ptr, CAF_STRING_T* str_ptr );

    BOOLEAN (*Empty) (IGUIEDIT_T * me_ptr);
    BOOLEAN (*SetStyle) (IGUIEDIT_T * me_ptr, CAF_GUIEDIT_STYLE_E style);
    BOOLEAN (*SetMaxLength) (IGUIEDIT_T * me_ptr, uint16 max_len);
};


struct IGUIEDIT_T
{
    CAF_VTBL_T(IGUIEDIT_T)        *pvt;
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
static __inline uint32 IGUIEDIT_AddRef( IGUIEDIT_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUIEDIT_Release( IGUIEDIT_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUIEDIT_GetCtrlGuid( IGUIEDIT_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUIEDIT_GetCtrlHandle( IGUIEDIT_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIEDIT_SetProperties( IGUIEDIT_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUIEDIT_GetProperties ( IGUIEDIT_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIEDIT_TestProperties ( IGUIEDIT_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUIEDIT_SetRect( IGUIEDIT_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIEDIT_GetRect( IGUIEDIT_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIEDIT_Redraw( IGUIEDIT_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIEDIT_SetActive( IGUIEDIT_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUIEDIT_IsActive( IGUIEDIT_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUIEDIT_HandleEvent( IGUIEDIT_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIEDIT_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		str_ptr:text content
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUIEDIT_SetText( IGUIEDIT_T *me_ptr, const CAF_STRING_T* str_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUIEDIT_T)->SetText( me_ptr, str_ptr );
}

/*****************************************************************************/
//  Description:   get text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		str_ptr:text content
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUIEDIT_GetText( IGUIEDIT_T *me_ptr, CAF_STRING_T* str_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUIEDIT_T)->GetText( me_ptr, str_ptr );
}


/*****************************************************************************/
//  Description:   clear text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUIEDIT_Empty(IGUIEDIT_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUIEDIT_T)->Empty( me_ptr );
}

/*****************************************************************************/
//  Description:   set edit style
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		style:font size
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUIEDIT_SetStyle(IGUIEDIT_T * me_ptr, CAF_GUIEDIT_STYLE_E style)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUIEDIT_T)->SetStyle( me_ptr, style );
}


/*****************************************************************************/
//  Description:   set edit content max len
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		max_len:edit content max len
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUIEDIT_SetMaxLength(IGUIEDIT_T * me_ptr, uint16 max_len)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUIEDIT_T)->SetMaxLength( me_ptr, max_len );
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIEDIT_H_
