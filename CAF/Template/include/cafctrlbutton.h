/****************************************************************************
 ** File Name:       cafctrlbutton.h                                           			  *
 ** Author:           Andrew.Zhang                                             		  *
 ** Date:             10/03/2009                                              			  *
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

#ifndef  _CAFGUIBUTTON_H_    
#define  _CAFGUIBUTTON_H_  

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

typedef struct  IGUIBUTTON_T   IGUIBUTTON_T;


//BUTTON上icon或image的位置
typedef enum
{ 
    CAF_GUI_BUTTON_ICON_LEFT,//左上
    CAF_GUI_BUTTON_ICON_RIGHT,//右上
    CAF_GUI_BUTTON_ICON_CENTER,//水平垂直居中
    CAF_GUI_BUTTON_ICON_UP_CENTER,//水平靠上居中 
    CAF_GUI_BUTTON_ICON_DOWN_CENTER,//水平靠下居中
    CAF_GUI_BUTTON_ICON_LEFT_BOTTOM,//左下
    CAF_GUI_BUTTON_ICON_RIGHT_BOTTOM,//右下
    CAF_GUI_BUTTON_ICON_LEFT_VCENTER,//垂直靠左居中
    CAF_GUI_BUTTON_ICON_RIGHT_VCENTER,//垂直靠右居中
    CAF_GUI_BUTTON_ICON_ALIGN_MAX
}CAF_GUI_BUTTON_ICON_POS_E;

//BUTTON的背景
typedef enum
{
    CAF_GUI_BUTTON_BG_NULL, 
    CAF_GUI_BUTTON_BG_IMAGE_ONLY,  
    CAF_GUI_BUTTON_BG_COLOR_ONLY
}CAF_GUI_BUTTON_BG_COLOR_E;

// button background
typedef struct
{
	CAF_GUI_BUTTON_BG_COLOR_E   	bg_type;                				//背景类型
   uint32          										bg_pressed_image_id;    	//背景图片
	uint32          										bg_released_image_id;   	//背景图片
   CAF_COLOR_T             						bg_pressed_color;       		//背景颜色 
	CAF_COLOR_T             						bg_released_color;      		//背景颜色
}CAF_GUI_BUTTON_BG_T;

CAF_INTERFACE (IGUIBUTTON_T)
{
    INHERIT_IGUICTRL;// 继承基类
    
    BOOLEAN (*SetText)( IGUIBUTTON_T * me_ptr, CAF_STRING_T *text_ptr );
    BOOLEAN (*SetTextById)( IGUIBUTTON_T * me_ptr, uint32 text_id );
    BOOLEAN (*SetGrayed)( IGUIBUTTON_T * me_ptr, BOOLEAN is_grayed, BOOLEAN is_flash );
    BOOLEAN (*SetLongPressed)( IGUIBUTTON_T * me_ptr, BOOLEAN is_longpressed );
    BOOLEAN (*SetTransparentDisp)( IGUIBUTTON_T * me_ptr, BOOLEAN is_transparent, CAF_COLOR_T transparent_color );
    BOOLEAN (*SetBackGround)( IGUIBUTTON_T * me_ptr, CAF_GUI_BUTTON_BG_T button_bg );
    BOOLEAN (*GetText)( IGUIBUTTON_T * me_ptr, CAF_STRING_T *text_ptr );		
};

struct IGUIBUTTON_T
{
    CAF_VTBL_T(IGUIBUTTON_T)        *pvt;
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
static __inline uint32 IGUIBUTTON_AddRef( IGUIBUTTON_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUIBUTTON_Release( IGUIBUTTON_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUIBUTTON_GetCtrlGuid( IGUIBUTTON_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUIBUTTON_GetCtrlHandle( IGUIBUTTON_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIBUTTON_SetProperties( IGUIBUTTON_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUIBUTTON_GetProperties ( IGUIBUTTON_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIBUTTON_TestProperties ( IGUIBUTTON_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUIBUTTON_SetRect( IGUIBUTTON_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIBUTTON_GetRect( IGUIBUTTON_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIBUTTON_Redraw( IGUIBUTTON_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIBUTTON_SetActive( IGUIBUTTON_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUIBUTTON_IsActive( IGUIBUTTON_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUIBUTTON_HandleEvent( IGUIBUTTON_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set button text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_ptr:text string
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetText(IGUIBUTTON_T * me_ptr, CAF_STRING_T *text_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetText( me_ptr, text_ptr );
}

/*****************************************************************************/
//  Description:   set button text by text id
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		text_id:text id
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetTextById(IGUIBUTTON_T * me_ptr, uint32 text_id) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetTextById( me_ptr, text_id );
}

/*****************************************************************************/
//  Description:   set button gray
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		is_grayed:TRUE,set button gray;FALSE,not gray
//		is_flash:TRUE,paint at once;FALSE,not paint at once
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetGrayed(IGUIBUTTON_T * me_ptr, BOOLEAN is_grayed, BOOLEAN is_flash)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetGrayed( me_ptr, is_grayed, is_flash );
}

/*****************************************************************************/
//  Description:   set button long press
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		is_longpressed:TRUE,long press valid;FALSE,long press not valid
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetLongPressed(IGUIBUTTON_T * me_ptr, BOOLEAN is_longpressed )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetLongPressed( me_ptr, is_longpressed );
}

/*****************************************************************************/
//  Description:   set button transparent display
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		is_transparent:TRUE,transparent;FALSE,not transparent
//		transparent_color:transparent color
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetTransparentDisp(IGUIBUTTON_T * me_ptr, BOOLEAN is_transparent, CAF_COLOR_T transparent_color )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetTransparentDisp( me_ptr, is_transparent, transparent_color );
}

/*****************************************************************************/
//  Description:   set button background
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		button_bg:background param
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_SetBackGround(IGUIBUTTON_T * me_ptr, CAF_GUI_BUTTON_BG_T button_bg )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->SetBackGround( me_ptr, button_bg );
}

/*****************************************************************************/
//  Description:   get button text
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		 text_ptr:text
//  Return:
//       TRUE:set success;FALSE:set fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIBUTTON_GetText(IGUIBUTTON_T * me_ptr, CAF_STRING_T *text_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIBUTTON_T )->GetText( me_ptr, text_ptr );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIBUTTON_H_
