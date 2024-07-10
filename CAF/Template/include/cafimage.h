/****************************************************************************
** File Name:      cafimage.h                                            *
** Author:         Andrew.Zhang                                             *
** Date:           26/03/2009                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This is cafimage head file.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             	DESCRIPTION                             *
** 03/2009       Andrew.Zhang     Create
** 
****************************************************************************/

#ifndef  _CAFIMAGE_H_    
#define  _CAFIMAGE_H_  

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
#include "sci_types.h" 
#include "caf.h"
#include "cafguid_def.h"


/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
typedef struct IIMAGE_T        IIMAGE_T;

//图像参数设置类型
typedef enum
{
    CAFIMAGE_PARM_SIZE, //设置图像显示的高和宽
    CAFIMAGE_PARM_IMAGE //设置图像数据相关信息
}CAFIMAGE_PARM_E;

//图像信息
typedef struct
{
   uint16 width;	//图象宽(像素)
   uint16 heigth;	//图象高(像素)
   uint16 colors;	//图像中的颜色数目
   uint16 reserve1;	//保留
   uint32 reserve2;	//保留
}CAF_IMAGE_INFO_T;


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/


// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IIMAGE_T)
{
//modified by andrew.zhang at 2009/03/02
#if 0
    INHERIT_IUNKNOWN(IIMAGE_T);

    BOOLEAN (*SetImageBits)(IIMAGE_T *me, uint8* bits_ptr);
    BOOLEAN (*SetImageType)(IIMAGE_T *me, uint32 type);
    BOOLEAN (*SetImageWidth)(IIMAGE_T *me, uint32 width);
    BOOLEAN (*SetImageHeight)(IIMAGE_T *me, uint32 height);
    BOOLEAN (*SetImageWbytes)(IIMAGE_T *me, uint32 width_bytes);
    BOOLEAN (*SetImageBpixel)(IIMAGE_T *me, uint32 bpixel);
    BOOLEAN (*SetImageBplanes)(IIMAGE_T *me, uint32 bplanes);


    BOOLEAN (*GetImageType)(IIMAGE_T *me, uint32 *type);
    BOOLEAN (*GetImageWidth)(IIMAGE_T *me, uint32 *width);
    BOOLEAN (*GetImageHeight)(IIMAGE_T *me, uint32 *height);
    BOOLEAN (*GetImageWbytes)(IIMAGE_T *me, uint32 *width_bytes);
    BOOLEAN (*GetImageBpixel)(IIMAGE_T *me, uint32 *bpixel);
    BOOLEAN (*GetImageBplanes)(IIMAGE_T *me, uint32 *bplanes);
    uint8* (*GetImageBits)(IIMAGE_T *me);
#else
    INHERIT_IUNKNOWN(IIMAGE_T);

    uint32 (*Draw)( IIMAGE_T *me_ptr, int32 x, int32 y ); 
    uint32 (*GetInfo)( IIMAGE_T *me_ptr, CAF_IMAGE_INFO_T * image_info_ptr ); 
    uint32 (*SetParm)( IIMAGE_T *me_ptr, int32 parm, int32 p1, int32 p2 ); 
#endif
};


// Define pointer to poit the virtual table of the control base class
struct IIMAGE_T
{
    CAF_VTBL_T(IIMAGE_T)  *vtbl_ptr;
};


static __inline CAF_RESULT_E IIMAGE_QueryInterface( IIMAGE_T *me_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, IIMAGE_T )->QueryInterface( me_ptr, guid, object_pptr );
}

static __inline uint32 IIMAGE_AddRef( IIMAGE_T *me_ptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, IIMAGE_T )->AddRef( me_ptr );
}

//closed by andrew.zhang at 2009/03/02
#if 0
static __inline uint32 IMODULE_Release( IIMAGE_T *me ) 
{
    return CAF_GET_VTBLPTR( me, IIMAGE_T )->Release( me );
}

static __inline MMI_RESULT_E IIMAGE_Release( IIMAGE_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    return CAF_GET_VTBLPTR( me, IIMAGE_T )->CreateInstance( me, guid, object_pptr );
}
#endif

static __inline uint32 IIMAGE_Release( IIMAGE_T *me_ptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr , IIMAGE_T )->Release( me_ptr );
}

/*****************************************************************************/
//  Description:  draw image on the screen
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       x:start point x
//       y:start point y
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline uint32 IIMAGE_Draw( IIMAGE_T *me_ptr, int32 x, int32 y ) 
{
    return CAF_GET_VTBLPTR( me_ptr, IIMAGE_T )->Draw( me_ptr, x, y );
}

/*****************************************************************************/
//  Description:  get image information
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       image_info_ptr:image information pointer
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline uint32 IIMAGE_GetInfo( IIMAGE_T *me_ptr, CAF_IMAGE_INFO_T * image_info_ptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, IIMAGE_T )->GetInfo( me_ptr, image_info_ptr );
}

/*****************************************************************************/
//  Description:  set image param
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       parm:param type
//       p1:value have been defined by parm
//       p2:value have been defined by parm
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline uint32 IIMAGE_SetParm( IIMAGE_T *me_ptr, int32 parm, int32 p1, int32 p2 ) 
{
    return CAF_GET_VTBLPTR( me_ptr, IIMAGE_T )->SetParm( me_ptr, parm, p1, p2 );
}

/*****************************************************************************/
//  Description:  set image width and height
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       p1:image display width
//       p2:image display height
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
static __inline uint32 IIMAGE_SetDrawSize( IIMAGE_T *me_ptr, int32 width, int32 heigth ) 
{
    return IIMAGE_SetParm( me_ptr, CAFIMAGE_PARM_SIZE, width, heigth ) ;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFIMAGE_H_
