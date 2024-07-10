/****************************************************************************
** File Name:      mmk_handle_internal.h                                   *
** Author:         James.Zhang                                             *
** Date:           05/07/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _MMK_HANDLE_INTERNAL_H_    
#define  _MMK_HANDLE_INTERNAL_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_app.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
        
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct
{
    MMI_HANDLE_TYPE_E handle_type;
    MMI_HANDLE_T      handle;
}MMI_BASE_NODE_T;

typedef BOOLEAN (*MMI_BASE_NODE_COMPARE)( MMI_BASE_NODE_T* node_ptr, uint32 condition1, uint32 condition2 );
/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : init handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_InitHandleList( void );

/*****************************************************************************/
// 	Description : create base node 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_CreateBaseNode( 
                                           MMI_HANDLE_TYPE_E type,
                                           uint32 size
                                           );

/*****************************************************************************/
// 	Description : destroy base node 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyBaseNode(
                                   MMI_BASE_NODE_T* base_node_ptr
                                   );

/*****************************************************************************/
// 	Description : find base node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_FindBaseNode(
                                         MMI_HANDLE_T handle
                                         );

/*****************************************************************************/
// 	Description : find base node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_FindInBaseNodeByCondition(
                                                      MMI_BASE_NODE_COMPARE func,  //�ȽϵĻص�����
                                                      uint32 condition1,           //�û����������1
                                                      uint32 condition2,           //�û����������2
                                                      MMI_HANDLE_T first_handle,   //���ҵ���ʼhandle, Ϊ0��Ĭ�ϴ�ͷ\β����
                                                      BOOLEAN is_order              //˳�������
                                                      );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_HANDLE_INTERNAL_H_  */
