/****************************************************************************
 ** File Name:      cafctrltextbox.h                                        *
 ** Author:         James.Zhang                                             *
 ** Date:           09/26/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the base class of the GUI *
 **                 control, all other controls will inherit this class.    *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 09/2008        James.Zhang       Create                                 *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUITEXTBOX_H_    
#define  _CAFGUITEXTBOX_H_  

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
#include "cafdc.h"

typedef struct  IGUITEXTBOX_T   IGUITEXTBOX_T;

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN         is_valid;
    uint16          start_pos;
    uint16          len;
    CAF_COLOR_T     font_color;
} CAF_TEXTBOX_MARK_T;

CAF_INTERFACE (IGUITEXTBOX_T)
{
    INHERIT_IGUICTRL;// ¼Ì³Ð»ùÀà

    BOOLEAN (*SetText)   ( IGUITEXTBOX_T *me_ptr, CAF_STRING_T *text_ptr, BOOLEAN is_update );
    BOOLEAN (*SetTextId) ( IGUITEXTBOX_T *me_ptr, uint32 text_id, BOOLEAN is_update );

    BOOLEAN (*SetMark) ( IGUITEXTBOX_T * me_ptr, CAF_TEXTBOX_MARK_T  mark );

    BOOLEAN (*SetBackground) ( IGUITEXTBOX_T * me_ptr, CAF_COLOR_T bg_color, uint32 image_id );

    BOOLEAN (*EnableScrollbar) ( IGUITEXTBOX_T * me_ptr, BOOLEAN is_need_scrollbar );
    BOOLEAN (*EnableAutoScroll) ( IGUITEXTBOX_T * me_ptr, uint32 scroll_period, BOOLEAN is_auto_scroll );
};

struct IGUITEXTBOX_T
{
    CAF_VTBL_T(IGUITEXTBOX_T)        *pvt;
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
static __inline uint32 IGUITEXTBOX_AddRef( IGUITEXTBOX_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUITEXTBOX_Release( IGUITEXTBOX_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUITEXTBOX_GetCtrlGuid( IGUITEXTBOX_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUITEXTBOX_GetCtrlHandle( IGUITEXTBOX_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUITEXTBOX_SetProperties( IGUITEXTBOX_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUITEXTBOX_GetProperties ( IGUITEXTBOX_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IIGUITEXTBOX_TestProperties ( IGUITEXTBOX_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUITEXTBOX_SetRect( IGUITEXTBOX_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUITEXTBOX_GetRect( IGUITEXTBOX_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUITEXTBOX_Redraw( IGUITEXTBOX_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUITEXTBOX_SetActive( IGUITEXTBOX_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUITEXTBOX_IsActive( IGUITEXTBOX_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUITEXTBOX_HandleEvent( IGUITEXTBOX_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUITEXTBOX_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_ptr:text content
//		is_update:TRUE,update at once;FALSE,not update at once
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_SetText( IGUITEXTBOX_T *me_ptr, CAF_STRING_T *text_ptr, BOOLEAN is_update )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->SetText( me_ptr, text_ptr, is_update );
}

/*****************************************************************************/
//  Description:   set text by text id
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_id:text id
//		is_update:TRUE,update at once;FALSE,not update at once
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_SetTextId( IGUITEXTBOX_T *me_ptr, uint32 text_id, BOOLEAN is_update )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->SetTextId( me_ptr, text_id, is_update );
}

/*****************************************************************************/
//  Description:   set mark
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		mark:mark infomation
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_SetMark( IGUITEXTBOX_T * me_ptr, CAF_TEXTBOX_MARK_T  mark ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->SetMark( me_ptr, mark );
}

/*****************************************************************************/
//  Description:   set textbox back ground
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		bg_color:back groud color
//		image_id:back groud image id
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_SetBackground( IGUITEXTBOX_T * me_ptr, CAF_COLOR_T bg_color, uint32 image_id ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->SetBackground( me_ptr, bg_color, image_id );
}

/*****************************************************************************/
//  Description:   set scroll bar
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		is_need_scrollbar:TRUE,need scroll bar;FALSE,not need scroll bar
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_EnableScrollbar( IGUITEXTBOX_T * me_ptr, BOOLEAN is_need_scrollbar ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->EnableScrollbar( me_ptr, is_need_scrollbar );
}

/*****************************************************************************/
//  Description:   set auto scroll
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//       scroll_period :auto scroll period
//		is_auto_scroll:TRUE,need auto scroll;FALSE,not need auto scroll
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUITEXTBOX_EnableAutoScroll( IGUITEXTBOX_T * me_ptr, uint32 scroll_period, BOOLEAN is_auto_scroll ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUITEXTBOX_T)->EnableAutoScroll( me_ptr, scroll_period, is_auto_scroll );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUITEXTBOX_H_
