/****************************************************************************
** File Name:      caf.h                                                   *
** Author:         Xiaoqing.Lu                                             *
** Date:           05/05/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/

#ifndef  _CAFLCD_H_    
#define  _CAFLCD_H_  

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

typedef struct IGUILCD_T        IGUILCD_T;

#define IGUILCD_QUERYINTERFACE( _this_ , _guid_, _pptr_ )\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->QueryInterface( _this_ , _guid_, _pptr_ )

#define IGUILCD_ADDREF( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->AddRef( _this_ )

#define IGUILCD_RELEASE( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->Release( _this_ )

#define IGUILCD_GetLcdWidth( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->GetLcdWidth( _this_ )

#define IGUILCD_GetLcdHight( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->GetLcdHight( _this_ )

#define IGUILCD_SetLcdWidth( _this_ , width)\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->SetLcdWidth( _this_ , width)

#define IGUILCD_SetLcdHight( _this_ , hight)\
    CAF_GET_VTBLPTR( _this_ ,IGUILCD_T)->SetLcdHight( _this_ , hight)

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/


// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IGUILCD_T)
{
   INHERIT_IUNKNOWN(IGUILCD_T);
   
   uint32 (*GetLcdWidth)(IGUILCD_T *iguilcd_ptr);
   uint32 (*GetLcdHight)(IGUILCD_T *iguilcd_ptr);
   BOOLEAN (*SetLcdWidth)(IGUILCD_T *iguilcd_ptr, uint32 width);
   BOOLEAN (*SetLcdHight)(IGUILCD_T *iguilcd_ptr, uint32 hight);
};

// Define pointer to poit the virtual table of the control base class
struct IGUILCD_T
{
    CAF_VTBL_T(IGUILCD_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFLCD_H_
