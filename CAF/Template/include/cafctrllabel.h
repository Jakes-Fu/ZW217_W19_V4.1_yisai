/****************************************************************************
 ** File Name:      cafcontrol.h                                            *
 ** Author:         Xiaoqing.Lu                                             *
 ** Date:           04/21/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the base class of the GUI *
 **                 control, all other controls will inherit this class.    *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 04/2008        Xiaoqing.Lu       Create                                 *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUILABEL_H_    
#define  _CAFGUILABEL_H_  

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


typedef struct  IGUILABEL_T   IGUILABEL_T;

//LABEL文本对齐方式
typedef enum
{
    CAF_GUILABEL_LEFT,
    CAF_GUILABEL_CENTER,
    CAF_GUILABEL_RIGHT,
    CAF_GUILABEL_ALIGNMENG_MAX
}CAF_GUILABEL_ALIGNMENG_TYPE_E;

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

CAF_INTERFACE (IGUILABEL_T)
{
    INHERIT_IGUICTRL;// 继承基类

    BOOLEAN (*SetText)   ( IGUILABEL_T *me_ptr, CAF_STRING_T *text_ptr, BOOLEAN is_update );
    BOOLEAN (*SetTextId) ( IGUILABEL_T *me_ptr, uint32 text_id, BOOLEAN is_update );
    BOOLEAN (*GetText) ( IGUILABEL_T *me_ptr, CAF_STRING_T *text_ptr );
    BOOLEAN (*SetIcon) ( IGUILABEL_T *me_ptr, uint32 icon_id );
    BOOLEAN (*SetBackgroundColor) ( IGUILABEL_T *me_ptr, CAF_COLOR_T back_color );
    BOOLEAN (*SetFontParam) ( IGUILABEL_T *me_ptr, CAF_FONT_TYPE_E font, CAF_COLOR_T color, uint8 font_space );
    BOOLEAN (*SetBackgroundImage) ( IGUILABEL_T *me_ptr, uint32 image_id );
    BOOLEAN (*SetLabelFontEffect) ( IGUILABEL_T *me_ptr, CAF_FONT_EFFECT_E font_effect );
};

struct IGUILABEL_T
{
    CAF_VTBL_T(IGUILABEL_T)        *pvt;
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
static __inline uint32 IGUILABEL_AddRef( IGUILABEL_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUILABEL_Release( IGUILABEL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUILABEL_GetCtrlGuid( IGUILABEL_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUILABEL_GetCtrlHandle( IGUILABEL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILABEL_SetProperties( IGUILABEL_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUILABEL_GetProperties ( IGUILABEL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILABEL_TestProperties ( IGUILABEL_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUILABEL_SetRect( IGUILABEL_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUILABEL_GetRect( IGUILABEL_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUILABEL_Redraw( IGUILABEL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILABEL_SetActive( IGUILABEL_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUILABEL_IsActive( IGUILABEL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUILABEL_HandleEvent( IGUILABEL_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILABEL_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_ptr:text content
//		is_update:TRUE,update at once;FALSE,not update at once
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetText( IGUILABEL_T *me_ptr, CAF_STRING_T *text_ptr, BOOLEAN is_update )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetText( me_ptr, text_ptr, is_update );
}

/*****************************************************************************/
//  Description:   set text by text id
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_id:text id
//		is_update:TRUE,update at once;FALSE,not update at once
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetTextId( IGUILABEL_T *me_ptr, uint32 text_id, BOOLEAN is_update )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetTextId( me_ptr, text_id, is_update );
}

/*****************************************************************************/
//  Description:   get text content
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_ptr:text content
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_GetText( IGUILABEL_T * me_ptr, CAF_STRING_T *text_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->GetText( me_ptr, text_ptr );
}

/*****************************************************************************/
//  Description:   set icon
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		icon_id:icon id
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetIcon( IGUILABEL_T * me_ptr, uint32 icon_id )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetIcon( me_ptr, icon_id );
}

/*****************************************************************************/
//  Description:   set background color
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		back_color:background color
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetBackgroundColor( IGUILABEL_T * me_ptr, CAF_COLOR_T back_color )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetBackgroundColor( me_ptr, back_color );
}

/*****************************************************************************/
//  Description:   set font infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		font:font
//		color:font color
//		font_space:font space
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetFontParam( IGUILABEL_T * me_ptr, CAF_FONT_TYPE_E font, CAF_COLOR_T color, uint8 font_space  )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetFontParam( me_ptr, font, color, font_space );
}

/*****************************************************************************/
//  Description:   set background image
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		image_id:image id
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetBackgroundImage( IGUILABEL_T * me_ptr, uint32 image_id )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetBackgroundImage( me_ptr, image_id );
}

/*****************************************************************************/
//  Description:   set font effect
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		font_effect:font effect
//  Return:
//       TRUE:success;FALSE,fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILABEL_SetLabelFontEffect( IGUILABEL_T * me_ptr, CAF_FONT_EFFECT_E font_effect)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILABEL_T)->SetLabelFontEffect( me_ptr, font_effect );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUILABEL_H_
