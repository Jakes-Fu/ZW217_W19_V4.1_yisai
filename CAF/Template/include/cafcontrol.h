/****************************************************************************
 ** File Name:      cafcontrol.h                                            *
 ** Author:         Xiaoqing.Lu                                             *
 ** Date:           04/15/2008                                              *
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

#ifndef  _CAFCONTROL_H_    
#define  _CAFCONTROL_H_  

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
#include "caf.h"
#include "cafshell.h"
#include "cafhelp.h"
#include "cafdc.h"
#ifdef MMI_VER_MAIN
#include "mmk_tree.h"
#endif
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct IGUICTRL_T   IGUICTRL_T;

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
//状态和属性是一样的，STATE暂时保留，为了和以前兼容，适当时可以考虑去掉
typedef uint32 GUICTRL_STATE_T;
#ifdef MMI_VER_STABLE
#define GUICTRL_STATE_ACTIVE             0x000000001    //active状态
#define GUICTRL_STATE_MENU               0x000000002    //menu控件
#endif
#define GUICTRL_STATE_SLIDE              0x000000004    //控件处于滑动状态
#ifdef MMI_VER_STABLE
#define GUICTRL_STATE_OWN_TITLE          0x000000008    //控件需要自绘标题
#endif
#define GUICTRL_STATE_DISABLE_ACTIVE     0x000000010    //控件禁止active
#define GUICTRL_STATE_DISABLE_KEY        0x000000020    //控件禁止接收按键消息
#define GUICTRL_STATE_DISABLE_TP         0x000000040    //控件禁止接收tp消息
#define GUICTRL_STATE_INVISIBLE          0x000000080    //控件隐藏
#define GUICTRL_STATE_KEYMODE_ACTIVE     0X000000100    //键盘模式下可以获取焦点，触屏模式下维持原来的获取焦点状态
// start: 0x000001000~0x00000f000为控件优先级用
#define GUICTRL_STATE_TOPMOST            0x000001000    //控件优先接收tp消息
#define GUICTRL_STATE_TOPMOST_1          0x000002000    //控件优先接收tp消息，比GUICTRL_STATE_TOPMOST优先级更高
#define GUICTRL_STATE_TOPMOST_MAX       GUICTRL_STATE_TOPMOST_1 // topmost最大级数(内部使用)
#define GUICTRL_STATE_TOPMOST_STEP      GUICTRL_STATE_TOPMOST   // topmost步长(内部使用)
// end
//控件属性类型
typedef uint32 CAF_GUICTRL_PROPERTIES_T;


//CTRL的通用属性
#define CAF_GUICTRL_COM_PROPERTIES_ACTIVE             0x000000001    //active属性
//#define GUICTRL_COM_PROPERTIES_MENU               0x000000002    //menu属性
//#define GUICTRL_COM_PROPERTIES_STATUSBAR          0x000000004    //状态栏控件属性
//#define GUICTRL_COM_PROPERTIES_OWN_TITLE          0x000000008    //控件需要自绘标题
//#define GUICTRL_COM_PROPERTIES_DISABLE_ACTIVE     0x000000010    //控件禁止active
//#define GUICTRL_COM_PROPERTIES_DISABLE_KEY        0x000000020    //控件禁止接收按键消息
#define CAF_GUICTRL_COM_PROPERTIES_DISABLE_TP         0x000000040    //控件禁止接收tp消息

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//border type
typedef enum
{
    GUI_BORDER_NONE,        //none
    GUI_BORDER_BOTTOM,
    GUI_BORDER_SOLID,       //solid line
    GUI_BORDER_ROUNDED,     //rounded line
    GUI_BORDER_SROUNDED,    //shadow rounded line
    GUI_BORDER_NOT_DISPLAY, //only border width,not display
    GUI_BORDER_MAX
} GUI_BORDER_TYPE_E;

//border
typedef struct
{
    uint16              width;      //border width
    CAF_COLOR_T         color;      //border color
    GUI_BORDER_TYPE_E   type;       //border type
} GUI_BORDER_T;

//bg type
typedef enum
{
    GUI_BG_NONE,    //none
    GUI_BG_IMG,     //bg image
    GUI_BG_COLOR,   //bg color
    GUI_BG_HALF_TRANS,//half transparent
    GUI_BG_MAX
} GUI_BG_TYPE_E;

//bg type
typedef enum
{
    GUI_SHAPE_RECT,         //rect
    GUI_SHAPE_ROUNDED_RECT, //rounded rect
    GUI_SHAPE_SROUNDED_RECT,//shadow rounded rect
    GUI_SHAPE_MAX
} GUI_SHAPE_E;

//bg
typedef struct
{
    GUI_BG_TYPE_E   bg_type;
    GUI_SHAPE_E     shape;          //only for color
    uint32          img_id;
    CAF_COLOR_T     color;
    BOOLEAN         is_screen_img;  //is all screen image,only for img
} GUI_BG_T;

//font
typedef struct
{
    CAF_FONT_TYPE_E     font;
    CAF_COLOR_T         color;
} GUI_FONT_ALL_T;

typedef struct
{
    int16       x;  /*!<coordinate x */
    int16 	    y;  /*!<coordinate y */
    int16       w;  /*!<coordinate w */
    int16       h;  /*!<coordinate height */
} GUI_SIZE_T;

#ifdef MMI_VER_STABLE
#define  CAF_GUICTRL_DATA_MEMBER\
    CAF_GUID_T          guid;\
    uint32              ref_num;\
    CAF_HANDLE_T        handle;\
    GUICTRL_STATE_T     state;\
    CAF_RECT_T          rect;\
    CAF_RECT_T          display_rect;\
    CAF_LCD_DEV_INFO    lcd_dev_info;\
    CAF_BOTH_RECT_T     both_rect;\
    GUI_BG_T            common_bg
#endif

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
// Inherit the base class of the IUNKNOWN
#if defined(MMI_VER_MAIN)
#define INHERIT_IGUICTRL\
    INHERIT_IUNKNOWN(IGUICTRL_T);\
    BOOLEAN                     (*Construct)                    ( IGUICTRL_T* me_ptr, void* init_data_ptr );\
    BOOLEAN                     (*Destruct)                     ( IGUICTRL_T* me_ptr );\
    CAF_HANDLE_RESULT_E         (*HandleEvent)                  ( IGUICTRL_T* me_ptr, CAF_MESSAGE_ID_E msg_id, void* param_ptr );\
    CAF_GUID_T                  (*GetCtrlGuid)                  ( IGUICTRL_T* me_ptr );\
    void                        (*SetCtrlHandle)                ( IGUICTRL_T* me_ptr, CAF_HANDLE_T ctrl_handle );\
    uint32                      (*GetCtrlHandle)                ( IGUICTRL_T* me_ptr );\
    void                        (*SetState)                     ( IGUICTRL_T* me_ptr, GUICTRL_STATE_T state, BOOLEAN is_true );\
    BOOLEAN                     (*GetState)                     ( IGUICTRL_T* me_ptr, GUICTRL_STATE_T state );\
    BOOLEAN                     (*SetRect)                      ( IGUICTRL_T* me_ptr, const CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*GetRect)                      ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    void                        (*SetLcdDevInfo)                ( IGUICTRL_T* me_ptr, const CAF_LCD_DEV_INFO* info_ptr );\
    BOOLEAN                     (*GetLcdDevInfo)                ( IGUICTRL_T* me_ptr, CAF_LCD_DEV_INFO* info_ptr );\
    BOOLEAN                     (*SetProperties)                ( IGUICTRL_T* me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true );\
    CAF_GUICTRL_PROPERTIES_T    (*GetProperties)                ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*TestProperties)               ( IGUICTRL_T* me_ptr, CAF_GUICTRL_PROPERTIES_T properties );\
    void                        (*Redraw)                       ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*SetActive)                    ( IGUICTRL_T* me_ptr, BOOLEAN is_true );\
    BOOLEAN                     (*IsActive)                     ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*SetBorder)                    ( IGUICTRL_T* me_ptr, GUI_BORDER_T* border_ptr );\
    BOOLEAN                     (*SetBg)                        ( IGUICTRL_T* me_ptr, GUI_BG_T* bg_ptr );\
    BOOLEAN                     (*SetFont)                      ( IGUICTRL_T* me_ptr, GUI_FONT_ALL_T* font_ptr);\
    BOOLEAN                     (*GetFont)                      ( IGUICTRL_T* me_ptr, GUI_FONT_ALL_T* font_ptr);\
    BOOLEAN                     (*SetProgress)                  ( IGUICTRL_T* me_ptr, BOOLEAN  is_display );\
    BOOLEAN                     (*SetCircularHandleUpDown)      ( IGUICTRL_T* me_ptr, BOOLEAN  is_circular );\
    BOOLEAN                     (*SetCircularHandleLeftRight)   ( IGUICTRL_T* me_ptr, BOOLEAN  is_circular );\
    BOOLEAN                     (*GetHeightByWidth)             ( IGUICTRL_T* me_ptr, uint16 width, uint16* height_ptr);\
    BOOLEAN                     (*GetDisplayRect)               ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*SetDisplayRect)               ( IGUICTRL_T* me_ptr, const CAF_RECT_T* rect_ptr, BOOLEAN is_update );\
    BOOLEAN                     (*GetSelectRect)                ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*GetSelectIndex)               ( IGUICTRL_T* me_ptr, BOOLEAN *is_first_ptr, BOOLEAN *is_last_ptr );\
    BOOLEAN                     (*SetSelectIndex)               ( IGUICTRL_T* me_ptr, BOOLEAN is_first, BOOLEAN *is_last );\
    BOOLEAN                     (*GetBothRect)                  ( IGUICTRL_T* me_ptr, CAF_BOTH_RECT_T* both_rect_ptr );\
    BOOLEAN                     (*SetBothRect)                  ( IGUICTRL_T* me_ptr, const CAF_BOTH_RECT_T* both_rect_ptr )
#else
#define INHERIT_IGUICTRL\
    INHERIT_IUNKNOWN(IGUICTRL_T);\
    BOOLEAN                     (*Construct)                    ( IGUICTRL_T* me_ptr, void* init_data_ptr );\
    BOOLEAN                     (*Destruct)                     ( IGUICTRL_T* me_ptr );\
    CAF_HANDLE_RESULT_E         (*HandleEvent)                  ( IGUICTRL_T* me_ptr, CAF_MESSAGE_ID_E msg_id, void* param_ptr );\
    CAF_GUID_T                  (*GetCtrlGuid)                  ( IGUICTRL_T* me_ptr );\
    void                        (*SetCtrlHandle)                ( IGUICTRL_T* me_ptr, CAF_HANDLE_T ctrl_handle );\
    uint32                      (*GetCtrlHandle)                ( IGUICTRL_T* me_ptr );\
    void                        (*SetState)                     ( IGUICTRL_T* me_ptr, GUICTRL_STATE_T state, BOOLEAN is_true );\
    BOOLEAN                     (*GetState)                     ( IGUICTRL_T* me_ptr, GUICTRL_STATE_T state );\
    BOOLEAN                     (*SetRect)                      ( IGUICTRL_T* me_ptr, const CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*GetRect)                      ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    void                        (*SetLcdDevInfo)                ( IGUICTRL_T* me_ptr, const CAF_LCD_DEV_INFO* info_ptr );\
    BOOLEAN                     (*GetLcdDevInfo)                ( IGUICTRL_T* me_ptr, CAF_LCD_DEV_INFO* info_ptr );\
    BOOLEAN                     (*SetProperties)                ( IGUICTRL_T* me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true );\
    CAF_GUICTRL_PROPERTIES_T    (*GetProperties)                ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*TestProperties)               ( IGUICTRL_T* me_ptr, CAF_GUICTRL_PROPERTIES_T properties );\
    void                        (*Redraw)                       ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*SetActive)                    ( IGUICTRL_T* me_ptr, BOOLEAN is_true );\
    BOOLEAN                     (*IsActive)                     ( IGUICTRL_T* me_ptr );\
    BOOLEAN                     (*SetBorder)                    ( IGUICTRL_T* me_ptr, GUI_BORDER_T* border_ptr );\
    BOOLEAN                     (*SetBg)                        ( IGUICTRL_T* me_ptr, GUI_BG_T* bg_ptr );\
    BOOLEAN                     (*SetFont)                      ( IGUICTRL_T* me_ptr, GUI_FONT_ALL_T* font_ptr);\
    BOOLEAN                     (*GetFont)                      ( IGUICTRL_T* me_ptr, GUI_FONT_ALL_T* font_ptr);\
    BOOLEAN                     (*SetProgress)                  ( IGUICTRL_T* me_ptr, BOOLEAN  is_display );\
    BOOLEAN                     (*SetCircularHandleUpDown)      ( IGUICTRL_T* me_ptr, BOOLEAN  is_circular );\
    BOOLEAN                     (*SetCircularHandleLeftRight)   ( IGUICTRL_T* me_ptr, BOOLEAN  is_circular );\
    BOOLEAN                     (*GetHeightByWidth)             ( IGUICTRL_T* me_ptr, uint16 width, uint16* height_ptr);\
    BOOLEAN                     (*GetDisplayRect)               ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*SetDisplayRect)               ( IGUICTRL_T* me_ptr, const CAF_RECT_T* rect_ptr, BOOLEAN is_update );\
    BOOLEAN                     (*GetSelectRect)                ( IGUICTRL_T* me_ptr, CAF_RECT_T* rect_ptr );\
    BOOLEAN                     (*GetSelectIndex)               ( IGUICTRL_T* me_ptr, BOOLEAN *is_first_ptr, BOOLEAN *is_last_ptr );\
    BOOLEAN                     (*GetBothRect)                  ( IGUICTRL_T* me_ptr, CAF_BOTH_RECT_T* both_rect_ptr );\
    BOOLEAN                     (*SetBothRect)                  ( IGUICTRL_T* me_ptr, const CAF_BOTH_RECT_T* both_rect_ptr )
#endif
// Define the virtual table of the control base class
CAF_INTERFACE(IGUICTRL_T)
{
   INHERIT_IGUICTRL;
};

// Define pointer to poit the virtual table of the control base class
struct IGUICTRL_T
{
    CAF_VTBL_T(IGUICTRL_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:   Query COM object
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//       guid:COM guid
//       object_pptr:query result 
//  Return:
//       CAF_RESULT_SUCCEEDED: Success 
//       Other: Fail 
//  Note: 
/*****************************************************************************/
static __inline CAF_RESULT_E IGUICTRL_QueryInterface( IGUICTRL_T *me_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->QueryInterface( me_ptr, guid, object_pptr );
}

/*****************************************************************************/
//  Description:   increase reference count of the control
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       uint32: the last reference count 
//  Note: 
/*****************************************************************************/
static __inline uint32 IGUICTRL_AddRef( IGUICTRL_T *me_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->AddRef( me_ptr );
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
static __inline uint32 IGUICTRL_Release( IGUICTRL_T *me_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->Release( me_ptr );
}

/*****************************************************************************/
//  Description:   construct the control object,called by the system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		init_data_ptr:initial data
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_Construct( IGUICTRL_T* me_ptr, void* init_data_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->Construct( me_ptr, init_data_ptr );
}

/*****************************************************************************/
//  Description:   destruct the control object,called by the system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_Destruct( IGUICTRL_T* me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->Destruct( me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUICTRL_HandleEvent( IGUICTRL_T *me_ptr, CAF_MESSAGE_ID_E msg_id, void* param_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->HandleEvent( me_ptr, msg_id, param_ptr );
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
static __inline CAF_GUID_T IGUICTRL_GetCtrlGuid( IGUICTRL_T* me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetCtrlGuid( me_ptr );
}

/*****************************************************************************/
//  Description:   set control handle,called by system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		ctrl_handle:control handle
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUICTRL_SetCtrlHandle( IGUICTRL_T *me_ptr, CAF_HANDLE_T ctrl_handle ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetCtrlHandle( me_ptr, ctrl_handle );
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
static __inline CAF_HANDLE_T IGUICTRL_GetCtrlHandle( IGUICTRL_T *me_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetCtrlHandle( me_ptr );
}

/*****************************************************************************/
//  Description:   set control state,called by system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		state:control state type
//		is_true:TRUE:set the state;FALSE:clear the state
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUICTRL_SetState( IGUICTRL_T *me_ptr, GUICTRL_STATE_T state, BOOLEAN is_true ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetState( me_ptr, state, is_true );
}

/*****************************************************************************/
//  Description:   test control states,called by system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		state:control states type
//  Return:
//       TRUE:states is seted;otherwise,not seted
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetState( IGUICTRL_T *me_ptr, GUICTRL_STATE_T state ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetState( me_ptr, state );
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
static __inline void IGUICTRL_SetRect( IGUICTRL_T *me_ptr, const CAF_RECT_T* rect_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetRect( me_ptr, rect_ptr );
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
static __inline void IGUICTRL_GetRect( IGUICTRL_T *me_ptr, CAF_RECT_T* rect_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetRect( me_ptr, rect_ptr );
}

/*****************************************************************************/
//  Description:   set LCD device infomation,called by system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		info_ptr:LCD device infomation
//  Return:
//       void
//  Note: 
/*****************************************************************************/
static __inline void IGUICTRL_SetLcdDevInfo( IGUICTRL_T *me_ptr, const CAF_LCD_DEV_INFO* info_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetLcdDevInfo( me_ptr, info_ptr );
}

/*****************************************************************************/
//  Description:   get LCD device infomation,called by system only
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		info_ptr:LCD device infomation
//  Return:
//       TRUE:success; FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetLcdDevInfo( IGUICTRL_T *me_ptr, CAF_LCD_DEV_INFO* info_ptr ) 
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetLcdDevInfo( me_ptr, info_ptr );
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
static __inline BOOLEAN IGUICTRL_SetProperties( IGUICTRL_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetProperties( me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUICTRL_GetProperties ( IGUICTRL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->GetProperties( me_ptr );
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
static __inline BOOLEAN IGUICTRL_TestProperties ( IGUICTRL_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->TestProperties( me_ptr, properties );
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
static __inline void IGUICTRL_Redraw( IGUICTRL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->Redraw( me_ptr );
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
static __inline BOOLEAN IGUICTRL_SetActive( IGUICTRL_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->SetActive( me_ptr, is_true );
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
static __inline BOOLEAN IGUICTRL_IsActive( IGUICTRL_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUICTRL_T )->IsActive( me_ptr );
}

/*****************************************************************************/
// 	Description : set control border info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetBorder(
                                           IGUICTRL_T   *me_ptr,
                                           GUI_BORDER_T *border_ptr
                                           ) 
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetBorder(me_ptr,border_ptr);
}

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetBg(
                                       IGUICTRL_T   *me_ptr,
                                       GUI_BG_T     *bg_ptr
                                       ) 
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetBg(me_ptr,bg_ptr);
}

/*****************************************************************************/
// 	Description : set control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetFont(
                                         IGUICTRL_T     *me_ptr,
                                         GUI_FONT_ALL_T *font_ptr
                                         ) 
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetFont(me_ptr,font_ptr);
}

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetFont(
                                         IGUICTRL_T     *me_ptr,
                                         GUI_FONT_ALL_T *font_ptr
                                         ) 
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetFont(me_ptr,font_ptr);
}

/*****************************************************************************/
// 	Description : set control is display progress
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetProgress(
                                             IGUICTRL_T     *me_ptr,
                                             BOOLEAN        is_display
                                             )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetProgress(me_ptr,is_display);
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetCircularHandleUpDown(
                                                         IGUICTRL_T *me_ptr,
                                                         BOOLEAN    is_circular
                                                         )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetCircularHandleUpDown(me_ptr,is_circular);
}

/*****************************************************************************/
// 	Description : set control is circular handle left/right key msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetCircularHandleLeftRight(
                                                            IGUICTRL_T  *me_ptr,
                                                            BOOLEAN     is_circular
                                                            )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetCircularHandleLeftRight(me_ptr,is_circular);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetHeightByWidth(
                                                  IGUICTRL_T    *me_ptr,    //in:
                                                  uint16        width,      //in:
                                                  uint16        *height_ptr //in/out:
                                                  )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetHeightByWidth(me_ptr,width,height_ptr);
}

/*****************************************************************************/
// 	Description : Get control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetDisplayRect(
                                                IGUICTRL_T      *me_ptr,    //in:
                                                CAF_RECT_T      *rect_ptr   //in/out:
                                                )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetDisplayRect(me_ptr,rect_ptr);
}

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetDisplayRect(
                                                IGUICTRL_T          *me_ptr,    //in:
                                                const CAF_RECT_T    *rect_ptr,  //in:
                                                BOOLEAN             is_update   //in:
                                                )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetDisplayRect(me_ptr,rect_ptr,is_update);
}

/*****************************************************************************/
// 	Description : get control select rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetSelectRect(
                                               IGUICTRL_T   *me_ptr,    //in:
                                               CAF_RECT_T   *rect_ptr   //in/out:
                                               )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetSelectRect(me_ptr,rect_ptr);
}

/*****************************************************************************/
// 	Description : get control select index,is first or last line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetSelectIndex(
                                                IGUICTRL_T  *me_ptr,        //in:
                                                BOOLEAN     *is_first_ptr,  //in/out:
                                                BOOLEAN     *is_last_ptr    //in/out:
                                                )
{
    CAF_ASSERT(PNULL != me_ptr);

    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetSelectIndex(me_ptr,is_first_ptr,is_last_ptr);
}

/*****************************************************************************/
// 	Description : set control select index,is first or last line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetSelectIndex(
                                                IGUICTRL_T  *me_ptr,    //in:
                                                BOOLEAN     is_first,   //in:
                                                BOOLEAN     is_last     //in:
                                                )
{
    CAF_ASSERT(PNULL != me_ptr);
#if defined(MMI_VER_MAIN)
    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetSelectIndex(me_ptr,is_first,&is_last);
#endif
}


/*****************************************************************************/
// 	Description : Get control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_GetBothRect(
                                             IGUICTRL_T*      me_ptr,    //in:
                                             CAF_BOTH_RECT_T* both_rect_ptr
                                             )
{
    CAF_ASSERT(PNULL != me_ptr);
    
    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->GetBothRect(me_ptr, both_rect_ptr);
}

/*****************************************************************************/
// 	Description : set control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
static __inline BOOLEAN IGUICTRL_SetBothRect(
                                             IGUICTRL_T*            me_ptr,    //in:
                                             const CAF_BOTH_RECT_T* both_rect_ptr
                                             )
{
    CAF_ASSERT(PNULL != me_ptr);
    
    return CAF_GET_VTBLPTR(me_ptr,IGUICTRL_T)->SetBothRect(me_ptr, both_rect_ptr);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFCONTROL_H_
