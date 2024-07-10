/***************************************************************************
** File Name:      MySample_image.h                                        *
** Author:         hua.fang                                                *
** Date:           07/16/2011                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMI_LIST_H_
#define _MMI_LIST_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "os_api.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// ����ṹ
typedef struct mmi_list_tag
{
    struct mmi_list_tag  *prev;
    struct mmi_list_tag  *next;
} MMI_LIST_T;

typedef void (*MMILIST_DATADESTRUCTFUNC)(MMI_LIST_T *list_node_ptr);

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_LIST_T* MMILIST_CreateNode(
                                      uint16    node_size                       // �ڵ��С
                                      );

/*****************************************************************************/
//  Description : destroy a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_DestroyNode(
                                MMI_LIST_T                  *list_node_ptr,     // Ҫ���ͷŵĽڵ�
                                MMILIST_DATADESTRUCTFUNC    destroy_func        // ���ٽڵ�ǰ�����û��Ļص�
                                );

/*****************************************************************************/
//  Description : destroy all list nodes
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_DestroyAllNodes(
                                    MMI_LIST_T                  *list_header_ptr,   // ͷ�ڵ�
                                    BOOLEAN                     is_destroy_header,  // �Ƿ�Ҫ����ͷ�ڵ�
                                    MMILIST_DATADESTRUCTFUNC    destroy_func        // ���ٽڵ�ǰ�����û��Ļص�
                                    );

/*****************************************************************************/
//  Description : init list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_Init(
                         MMI_LIST_T     *list_node_ptr
                         );

/*****************************************************************************/
//  Description : is list empty
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMILIST_IsEmpty(
                               MMI_LIST_T   *list_header_ptr
                               );

/*****************************************************************************/
//  Description : detach a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_LIST_T* MMILIST_Detach(
                                  MMI_LIST_T    *list_node_ptr
                                  );

/*****************************************************************************/
//  Description : insert into the first
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertFirst(
                                MMI_LIST_T     *list_header_ptr,
                                MMI_LIST_T     *list_node_ptr
                                );

/*****************************************************************************/
//  Description : insert last
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertLast(
                               MMI_LIST_T     *list_header_ptr,
                               MMI_LIST_T     *list_node_ptr
                               );

/*****************************************************************************/
//  Description : insert before base node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertBefore(
                                 MMI_LIST_T     *base_node_ptr,
                                 MMI_LIST_T     *insert_node_ptr
                                 );

/*****************************************************************************/
//  Description : insert after base node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertAfter(
                                MMI_LIST_T     *base_node_ptr,
                                MMI_LIST_T     *insert_node_ptr
                                );

/*****************************************************************************/
//  Description : insert by index
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertByIndex(
                                  MMI_LIST_T      *list_header_ptr,
                                  MMI_LIST_T      *list_node_ptr,
                                  uint16          index
                                  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMI_LIST_H_
