/****************************************************************************
 ** File Name:      cafctrllist.h                                           *
 ** Author:         Xiaoqing.Lu                                             *
 ** Date:           06/11/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the base class of the GUI *
 **                 control, all other controls will inherit this class.    *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 06/2008        Xiaoqing.Lu       Create                                 *
 **                                                                         *
 ****************************************************************************/

#ifndef  _CAFGUILIST_H_    
#define  _CAFGUILIST_H_  

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

typedef struct  IGUILIST_T   IGUILIST_T;

typedef enum
{
    CAF_GUILIST_TYPE_NONE,
    CAF_GUILIST_RADIOLIST_E, //单选
    CAF_GUILIST_CHECKLIST_E, //多选
    CAF_GUILIST_TEXTLIST_E,
    CAF_GUILIST_TYPE_MAX
}CAF_GUILIST_TYPE_E;

typedef struct
{
    uint32        image_id;
    uint32        text_id;
    CAF_STRING_T  text_buffer; //text id == 0时有效

    uint32        left_sk_id;
    uint32        middle_sk_id;
    uint32        right_sk_id;

    uint32        user_data;    //设置用户数据

    uint32        reserved;

}CAF_GUILIST_ITEM_T;

CAF_INTERFACE (IGUILIST_T)
{
    INHERIT_IGUICTRL;// 继承基类

    //item
    BOOLEAN (*AppendItem)     ( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T *item_ptr );
    BOOLEAN (*InsertItem)     ( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T* item_ptr, uint16 index );
    BOOLEAN (*ReplaceItem)    ( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T* item_ptr, uint16 index );
    BOOLEAN (*RemoveItem)     ( IGUILIST_T *me_ptr, uint16 index );
    BOOLEAN (*RemoveAllItems) ( IGUILIST_T *me_ptr );
    BOOLEAN (*GetItemData)    ( IGUILIST_T *me_ptr, uint16 index, uint32* user_data_ptr );
    
    //item state
    BOOLEAN (*SetItemValid)   ( IGUILIST_T *me_ptr, uint16 index, BOOLEAN is_valid );
    //modified by andrew.zhang at 2009/03/19
    BOOLEAN (*IsItemValid)   ( IGUILIST_T *me_ptr, uint16 index );
    BOOLEAN (*SetItemSel)     ( IGUILIST_T *me_ptr, uint16 index, BOOLEAN is_selected );
    //modified by andrew.zhang at 2009/03/19
    BOOLEAN (*IsItemSel)     ( IGUILIST_T *me_ptr, uint16 index );
    
    // item number/index
    BOOLEAN (*SetMaxItemNum)       ( IGUILIST_T *me_ptr, uint16 max_num );
    BOOLEAN (*SetMaxSelectedItem)  ( IGUILIST_T *me_ptr, uint16 max_num );
    BOOLEAN (*SetCurItemIndex)     ( IGUILIST_T *me_ptr, uint16 index );
    uint16  (*GetCurItemIndex)     ( IGUILIST_T *me_ptr );
    uint16  (*GetTotalItemNum)     ( IGUILIST_T *me_ptr );

    BOOLEAN  (*SetBackgroundColor)     ( IGUILIST_T *me_ptr, CAF_COLOR_T back_color );	
    BOOLEAN  (*GetItem)     ( IGUILIST_T *me_ptr, CAF_GUILIST_ITEM_T* item_ptr, uint16 index );			
};


struct IGUILIST_T
{
    CAF_VTBL_T(IGUILIST_T)        *pvt;
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
static __inline uint32 IGUILIST_AddRef( IGUILIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUILIST_Release( IGUILIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUILIST_GetCtrlGuid( IGUILIST_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUILIST_GetCtrlHandle( IGUILIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILIST_SetProperties( IGUILIST_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUILIST_GetProperties ( IGUILIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILIST_TestProperties ( IGUILIST_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUILIST_SetRect( IGUILIST_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUILIST_GetRect( IGUILIST_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUILIST_Redraw( IGUILIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUILIST_SetActive( IGUILIST_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUILIST_IsActive( IGUILIST_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUILIST_HandleEvent( IGUILIST_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUILIST_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   add a list item at the tail
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		item_ptr:item infomation
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline  BOOLEAN IGUILIST_AppendItem( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T* item_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->AppendItem( me_ptr, item_ptr );
}

/*****************************************************************************/
//  Description:   insert a list item at index position
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		item_ptr:item infomation
//		index:position index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_InsertItem( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T* item_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr);
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->InsertItem( me_ptr, item_ptr, index );
}

/*****************************************************************************/
//  Description:   replace a list item at index position
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		item_ptr:item infomation
//		index:position index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_ReplaceItem( IGUILIST_T *me_ptr, const CAF_GUILIST_ITEM_T* item_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->ReplaceItem( me_ptr, item_ptr, index );
}

/*****************************************************************************/
//  Description:   remove a list item at index position
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_RemoveItem( IGUILIST_T *me_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->RemoveItem( me_ptr, index );
}

/*****************************************************************************/
//  Description:   remove all list item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_RemoveAllItems( IGUILIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->RemoveAllItems( me_ptr );
}

/*****************************************************************************/
//  Description:   get list user data
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//		user_data_ptr:user data
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_GetItemData( IGUILIST_T *me_ptr, uint16 index, uint32* user_data_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->GetItemData( me_ptr, index, user_data_ptr );
}

/*****************************************************************************/
//  Description:   set list item valid state
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//		is_valid:TRUE:item is valid;FALSE:item is invalid
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetItemValid( IGUILIST_T *me_ptr, uint16 index, BOOLEAN is_valid )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->SetItemValid( me_ptr, index, is_valid );
}

/*****************************************************************************/
//  Description:   get list item valid state
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//  Return:
//       TRUE:valid;FALSE:invalid
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_IsItemValid( IGUILIST_T *me_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->IsItemValid( me_ptr, index );
}

/*****************************************************************************/
//  Description:   set list item state selected 
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//		is_selected:TRUE:selected;FALSE:not selected
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetItemSel( IGUILIST_T *me_ptr, uint16 index, BOOLEAN is_selected )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->SetItemSel( me_ptr, index, is_selected );
}

/*****************************************************************************/
//  Description:   get list item state selected 
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//  Return:
//       TRUE:selected;FALSE:not selected
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_IsItemSel( IGUILIST_T *me_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->IsItemSel( me_ptr, index );
}

/*****************************************************************************/
//  Description:   set list the max item count
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		max_num:item count
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetMaxItemNum( IGUILIST_T *me_ptr, uint16 max_num )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T )->SetMaxItemNum( me_ptr, max_num );
}

/*****************************************************************************/
//  Description:   set list the max item count selected
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		max_num:item count
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetMaxSelectedItem( IGUILIST_T *me_ptr, uint16 max_num )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T )->SetMaxSelectedItem( me_ptr, max_num );
}

/*****************************************************************************/
//  Description:   set current item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:item index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetCurItemIndex( IGUILIST_T *me_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->SetCurItemIndex( me_ptr, index );
}

/*****************************************************************************/
//  Description:   get current item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       current item index
//  Note: 
/*****************************************************************************/
static __inline uint16 IGUILIST_GetCurItemIndex( IGUILIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->GetCurItemIndex( me_ptr );
}

/*****************************************************************************/
//  Description:   get total item count
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       total item count
//  Note: 
/*****************************************************************************/
static __inline uint16 IGUILIST_GetTotalItemNum( IGUILIST_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->GetTotalItemNum( me_ptr );
}


/*****************************************************************************/
//  Description:   set background color
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		back_color:background color
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_SetBackgroundColor( IGUILIST_T *me_ptr, CAF_COLOR_T back_color )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->SetBackgroundColor( me_ptr, back_color );
}


/*****************************************************************************/
//  Description:   get item
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		index:position index
//		user_data_ptr:user data
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUILIST_GetItem( IGUILIST_T *me_ptr, CAF_GUILIST_ITEM_T* item_ptr, uint16 index )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR(me_ptr, IGUILIST_T)->GetItem( me_ptr, item_ptr, index );
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUILIST_H_
