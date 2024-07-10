/****************************************************************************
 ** File Name:       cafctrlanim.h                                           			  *
 ** Author:           Andrew.Zhang                                             		  *
 ** Date:             10/03/2009                                              			  *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is caf head of anim control					  *
 **                   												  *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                       *
 ** 09/2009      Andrew.Zhang       Create                                 *
 **                                                                                          *
 ****************************************************************************/

#ifndef  _CAFGUIANIM_H_    
#define  _CAFGUIANIM_H_  

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
#include "cafimage.h"


typedef struct  IGUIANIM_T   IGUIANIM_T;

//anim set param result
typedef enum
{
    CAF_ANIM_RESULT_SUCC,        //success
    CAF_ANIM_RESULT_FAIL,        //fail
    CAF_ANIM_RESULT_WAIT,        //waiting
    CAF_ANIM_RESULT_DEC_FAIL,    //decode fail
    CAF_ANIM_RESULT_READ_FAIL,   //read fail
    CAF_ANIM_RESULT_BUSY,        //busy
    CAF_ANIM_RESULT_NO_SUPPORT,  //not support
    CAF_ANIM_RESULT_EMPTY,       //file is null
    CAF_ANIM_RESULT_BIG,         //file is too big
    CAF_ANIM_RESULT_INTERRUPT,   //decode/encode task is destroy
    CAF_ANIM_RESULT_MAX
} CAF_ANIM_RESULT_E;


//anim display align
typedef enum
{
    CAF_ANIM_ALIGN_NONE,
    CAF_ANIM_ALIGN_LEFT_TOP,         //左上
    CAF_ANIM_ALIGN_LEFT_VMIDDLE,     //左中
    CAF_ANIM_ALIGN_LEFT_BOTTOM,      //左下
    CAF_ANIM_ALIGN_HMIDDLE_TOP,      //中上
    CAF_ANIM_ALIGN_HVMIDDLE,         //中
    CAF_ANIM_ALIGN_HMIDDLE_BOTTOM,   //中下
    CAF_ANIM_ALIGN_RIGHT_TOP,        //右上
    CAF_ANIM_ALIGN_RIGHT_VMIDDLE,    //右中
    CAF_ANIM_ALIGN_RIGHT_BOTTOM,     //右下
    CAF_ANIM_ALIGN_MAX
} CAF_ANIM_ALIGN_STYLE_E;

//anim type
typedef enum
{
    CAF_ANIM_TYPE_NONE,      // none, nothing display
    CAF_ANIM_TYPE_UNKNOW,    // unknow
    CAF_ANIM_TYPE_IMAGE_BMP, // bmp image
    CAF_ANIM_TYPE_IMAGE_WBMP,// wbmp image
    CAF_ANIM_TYPE_IMAGE_GIF, // gif image
    CAF_ANIM_TYPE_IMAGE_JPG, // jpg image
    CAF_ANIM_TYPE_IMAGE_PNG, // png image
    CAF_ANIM_TYPE_ANIM,      // anim
    CAF_ANIM_TYPE_MAX
} CAF_ANIM_TYPE_E;

//ID type
//typedef enum
//{
//    CAF_ID_TYPE_ANIM = 1,      // ANIM ID,指MOCOR平台自定义动画
//    CAF_ID_TYPE_IMAGE,    // IMAGE ID
//    CAF_ID_TYPE_MAX
//} CAF_ANIM_ID_TYPE_E;

//file info
typedef struct 
{
    BOOLEAN     is_read_syn;    //是否同步读取数据
    BOOLEAN     reserved;       //reserved
    wchar       *full_path_wstr_ptr; //file full path
    uint16      full_path_wstr_len;  //file full path len,字节长度
} CAF_ANIM_FILE_INFO_T;


//资源包图像数据信息
typedef struct 
{
    uint32     				id;	//ID值
    //CAF_ANIM_ID_TYPE_E     	type;	//ID类型
    CAF_ANIM_TYPE_E     	type;	//类型
} CAF_ANIM_ID_INFO_T;


//BUFFER类型图像数据信息
typedef struct 
{
    uint8     *data_ptr;	//data buffer
    uint32     data_size;	//data size
} CAF_ANIM_BUFFER_INFO_T;


//display info
typedef struct 
{
    BOOLEAN                 			is_need_resize;         //is need resize
    BOOLEAN                 			is_update;              //is need update immediately
    BOOLEAN                 			is_play_once;           //if play anim is once
    BOOLEAN                 			is_bg_img;              //是否需要为ctrl rect填上背景图
    BOOLEAN                 			is_bg_color;            //是否需要为ctrl rect填上背景色
    BOOLEAN                 			is_disp_one_frame;      //是否只显示一帧图片,比如九宫格预览gif,选项卡主菜单,只显示单张图片,不显示动画
    uint16                  			display_x;              //display x
    uint16                  			display_y;              //display y
    CAF_COLOR_T          		bg_color;               //control background color
    uint32          				bg_img_id;              //control background image id
    CAF_ANIM_ALIGN_STYLE_E   	align_style;            //align style,当为GUIANIM_ALIGN_NONE时,display x,y才起作用
} CAF_ANIM_DISPLAY_INFO_T;


CAF_INTERFACE (IGUIANIM_T)
{
    INHERIT_IGUICTRL;// 继承基类
    
    CAF_ANIM_RESULT_E (*SetParam)( IGUIANIM_T * me_ptr, CAF_ANIM_DISPLAY_INFO_T * display_info_ptr, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source );
    BOOLEAN (*SetDisplayPos)( IGUIANIM_T * me_ptr, CAF_POINT_T* point_ptr );
    BOOLEAN (*Play)( IGUIANIM_T * me_ptr );
    BOOLEAN (*Pause)( IGUIANIM_T * me_ptr );
    BOOLEAN (*Resume)( IGUIANIM_T * me_ptr );
    BOOLEAN (*GetImageInfo)( IGUIANIM_T * me_ptr , CAF_IMAGE_INFO_T * anim_info_ptr );
};

struct IGUIANIM_T
{
    CAF_VTBL_T(IGUIANIM_T)        *pvt;
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
static __inline uint32 IGUIANIM_AddRef( IGUIANIM_T *me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->AddRef( (IGUICTRL_T *)me_ptr );
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
static __inline uint32 IGUIANIM_Release( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->Release( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_GUID_T IGUIANIM_GetCtrlGuid( IGUIANIM_T * me_ptr)
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->GetCtrlGuid( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_T IGUIANIM_GetCtrlHandle( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->GetCtrlHandle( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIANIM_SetProperties( IGUIANIM_T * me_ptr, CAF_GUICTRL_PROPERTIES_T properties, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->SetProperties( (IGUICTRL_T *)me_ptr, properties, is_true );
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
static __inline CAF_GUICTRL_PROPERTIES_T IGUIANIM_GetProperties ( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->GetProperties( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIANIM_TestProperties ( IGUIANIM_T *me_ptr, CAF_GUICTRL_PROPERTIES_T properties )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->TestProperties( (IGUICTRL_T *)me_ptr, properties );
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
static __inline void IGUIANIM_SetRect( IGUIANIM_T * me_ptr, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->SetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIANIM_GetRect( IGUIANIM_T * me_ptr, CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->GetRect( (IGUICTRL_T *)me_ptr, rect_ptr );
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
static __inline void IGUIANIM_Redraw( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->Redraw( (IGUICTRL_T *)me_ptr );
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
static __inline BOOLEAN IGUIANIM_SetActive( IGUIANIM_T * me_ptr, BOOLEAN is_true )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->SetActive( (IGUICTRL_T *)me_ptr, is_true );
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
static __inline BOOLEAN IGUIANIM_IsActive( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->IsActive( (IGUICTRL_T *)me_ptr );
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
static __inline CAF_HANDLE_RESULT_E IGUIANIM_HandleEvent( IGUIANIM_T * me_ptr, CAF_MESSAGE_ID_E msg_id, void * param_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->HandleEvent( (IGUICTRL_T *)me_ptr, msg_id, param_ptr );
}

/*****************************************************************************/
//  Description:   set animal param
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		display_info_ptr:animal display infomation
//		data_ptr:animal data infomation
//		resource_source:animal data type
//  Return:
//       CAF_ANIM_RESULT_SUCC:success
//  Note: 
/*****************************************************************************/
static __inline CAF_ANIM_RESULT_E IGUIANIM_SetParam( IGUIANIM_T * me_ptr, CAF_ANIM_DISPLAY_INFO_T * display_info_ptr, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->SetParam( me_ptr, display_info_ptr, data_ptr, resource_source );
}

/*****************************************************************************/
//  Description:   set display positon
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		point_ptr:display start point
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIANIM_SetDisplayPos ( IGUIANIM_T * me_ptr, CAF_POINT_T* point_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->SetDisplayPos( me_ptr, point_ptr );
}

/*****************************************************************************/
//  Description:   play animal
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIANIM_Play( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->Play( me_ptr );
}

/*****************************************************************************/
//  Description:   pause play
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIANIM_Pause( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->Pause( me_ptr );
}

/*****************************************************************************/
//  Description:   play animal again
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIANIM_Resume( IGUIANIM_T * me_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->Resume( me_ptr );
}

/*****************************************************************************/
//  Description:   get animal data infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//		anim_info_ptr:animal data infomation
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN IGUIANIM_GetImageInfo( IGUIANIM_T * me_ptr , CAF_IMAGE_INFO_T * anim_info_ptr )
{
    CAF_ASSERT( PNULL != me_ptr );
    return CAF_GET_VTBLPTR( me_ptr, IGUIANIM_T )->GetImageInfo( me_ptr, anim_info_ptr );
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
   
#endif // _CAFGUIANIM_H_
