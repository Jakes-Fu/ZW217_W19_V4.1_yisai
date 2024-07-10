/****************************************************************************
** File Name:      cafapplet_gen.h                                         *
** Author:         James.Zhang                                             *
** Date:           04/15/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_APPLET_GEN_H_    
#define  _CAF_APPLET_GEN_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafapplet.h"
#include "cafmodule_gen.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef CAF_HANDLE_RESULT_E  (*APPLET_HANDLEEVENT)   ( IAPPLET_T* applet_ptr, CAF_MESSAGE_ID_E msg_id, void* param );
typedef CAF_RESULT_E         (*APPLET_QUERYINTERFACE)( IAPPLET_T* applet_ptr, CAF_GUID_T guid, void** object_pptr );
typedef void                 (*APPLET_FREEDATA)      ( IAPPLET_T* applet_ptr );

typedef struct                                    
{
	CAF_VTBL_T(IAPPLET_T)  *vtbl_ptr;
    CAF_HANDLE_T           app_handle;

    CAF_GUID_T             guid;
    uint32                 ref_num;
    
    IMODULE_T*             module_ptr;

    APPLET_QUERYINTERFACE  QueryInterface;
    APPLET_FREEDATA        FreeData;
}CAF_APPLET_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : applet new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E CAFAPPLET_New( 
                                  uint32                size,
                                  CAF_GUID_T            guid,
                                  IMODULE_T*            imodule_ptr, 
                                  IAPPLET_T**           iapplet_pptr, 
                                  APPLET_HANDLEEVENT    app_handle_func,
                                  APPLET_QUERYINTERFACE app_query_func,
                                  APPLET_FREEDATA       app_free_func
                                  );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
