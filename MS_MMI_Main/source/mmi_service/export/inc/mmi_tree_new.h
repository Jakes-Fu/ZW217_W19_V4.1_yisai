/****************************************************************************
** File Name:      mmi_tree_new.h                                     *
** Author:         James.Zhang                                             *
** Date:           05/15/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 08/2012        Hongbing.Ju       Modified
****************************************************************************/

#ifndef  _MMI_TREE_NEW_H_    
#define  _MMI_TREE_NEW_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "os_api.h"
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
#define MMITREE_TRAVEL_MAX_LEN  32

    /*----------------------------------------------------------------------------*/
    /*                         Data Structures                                    */
    /*----------------------------------------------------------------------------*/
    typedef uint32 MMITREE_TRAVEL_STATE_T;
    /* item state */
#define MMITREE_TRAVEL_ORDER          0x000000001   //˳��, ����Ϊ����
#define MMITREE_TRAVEL_CHILD          0x000000002   //�����ӽڵ�, ����ֻ�������ڵ�

    typedef struct
    {
        uint32* data_ptr;
        uint32  total_num;
        uint32  max_num;

    }MMITREE_TRAVEL_INFO_T;

    typedef struct mmi_treenode_tag* MMI_TREENODE_HANDLE_T;

    typedef BOOLEAN (*TREENODE_CONSTRUCT)    ( MMI_TREENODE_HANDLE_T node_ptr, uint32 param1, uint32 param2 );

    typedef BOOLEAN (*TREENODE_DESTRUCT)     ( uint32 handle );
    typedef uint32  (*TREENODE_HANDLE_EVENT) ( uint32 handle, uint32 msg_id, void* param );
    typedef BOOLEAN (*TREENODE_COMPARE)      ( uint32 handle, uint32 condition1, uint32 condition2 );

    /*--------------------------------------------------------------------------*/
    /*                         FUNCTIONS                                        */
    /*--------------------------------------------------------------------------*/

    /*****************************************************************************/
    // 	Description : add tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ���ʧ��, ��Ҫ�û�����tree node
    /*****************************************************************************/
    PUBLIC MMI_TREENODE_HANDLE_T MMI_AddTreeNode(
        TREENODE_CONSTRUCT     func,
        uint32                  param1,
        uint32                  param2,
        MMI_TREENODE_HANDLE_T*  root_pptr,
        MMI_TREENODE_HANDLE_T   parent_node_ptr
        );

    /*****************************************************************************/
    // 	Description : get parent tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ȡ���ڵ�, ���ޣ�����PNULL
    /*****************************************************************************/
    PUBLIC MMI_TREENODE_HANDLE_T MMI_GetParentTreeNode(
        MMI_TREENODE_HANDLE_T node_ptr
        );

    /*****************************************************************************/
    // 	Description : get child tree node head
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ȡͷ�ӽڵ�, ���ޣ�����PNULL
    /*****************************************************************************/
    PUBLIC MMI_TREENODE_HANDLE_T MMI_GetChildTreeNode(
        MMI_TREENODE_HANDLE_T node_ptr
        );

    /*****************************************************************************/
    // 	Description : get sibling tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ȡ�ֵܽڵ�, ���ޣ�����PNULL
    /*****************************************************************************/
    PUBLIC MMI_TREENODE_HANDLE_T MMI_GetSiblingTreeNode(
        MMI_TREENODE_HANDLE_T node_ptr
        );

    /*****************************************************************************/
    // 	Description : get tree node data
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ȡͷ�ӽڵ�, ���ޣ�����PNULL
    /*****************************************************************************/
    PUBLIC uint32 MMI_GetTreeNodeData(
        MMI_TREENODE_HANDLE_T node_ptr
        );

    /*****************************************************************************/
    // 	Description : set tree node data
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: ���ýڵ�����
    /*****************************************************************************/
    PUBLIC void MMI_SetTreeNodeData(
        MMI_TREENODE_HANDLE_T   node_ptr,
        uint32                  data
        );

    /*****************************************************************************/
    // 	Description : find tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC uint32 MMI_FindAllTreeNode(
        MMI_TREENODE_HANDLE_T   root_ptr,
        TREENODE_COMPARE       func,
        uint32                  condition1, 
        uint32                  condition2,
        MMITREE_TRAVEL_STATE_T state
        );


    /*****************************************************************************/
    // 	Description : tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC void MMI_DispatchToAllTreeNode(
        MMI_TREENODE_HANDLE_T   root_ptr,
        TREENODE_HANDLE_EVENT  func,
        uint32                  msg_id,
        void*                   param,
        MMITREE_TRAVEL_STATE_T state
        );

    /*****************************************************************************/
    // 	Description : remove tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC void MMI_RemoveTreeNode(
        MMI_TREENODE_HANDLE_T*  root_pptr,
        TREENODE_DESTRUCT  func,
        MMI_TREENODE_HANDLE_T   tree_node_ptr
        );

    /*****************************************************************************/
    // 	Description : remove child tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC void MMI_RemoveAllTreeNode(
        MMI_TREENODE_HANDLE_T*  root_pptr,
        TREENODE_DESTRUCT      func
        );
#if 0
    /*****************************************************************************/
    // 	Description : remove tree nodes
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:���ٵ�ָ���ڵ�, ����return to idle��Ӧ��
    /*****************************************************************************/
    PUBLIC void MMI_RemoveToAppointTreeNode(
        MMI_TREENODE_HANDLE_T*  root_pptr,
        TREENODE_DESTRUCT      func,
        MMI_TREENODE_HANDLE_T   tree_node_ptr
        );
#endif
    /*****************************************************************************/
    // 	Description : get ancestor tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note: �ж������ڵ����ĸ������Ƚڵ�, ����޹�ϵ, ����PNULL
    /*****************************************************************************/
    PUBLIC MMI_TREENODE_HANDLE_T MMI_GetAncestorTreeNode(
        MMI_TREENODE_HANDLE_T   node1_ptr,
        MMI_TREENODE_HANDLE_T   node2_ptr
        );

    /*****************************************************************************/
    // 	Description : create travel info
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC MMITREE_TRAVEL_INFO_T* MMI_CreateTreeTravelInfo(
        void
        );

    /*****************************************************************************/
    // 	Description : travel all tree node
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:Ϊ��ֹ������ʱĳ���ڵ㱻����, �ȱ���������нڵ����Ϣ, �ٱ�������Ϣ
    /*****************************************************************************/
    PUBLIC void MMI_TravelTree(
        MMITREE_TRAVEL_INFO_T* travel_ptr,
        MMI_TREENODE_HANDLE_T   root_ptr,
        MMITREE_TRAVEL_STATE_T state
        );

    /*****************************************************************************/
    // 	Description : destroy travel info
    //	Global resource dependence : 
    //  Author:James.Zhang
    //	Note:
    /*****************************************************************************/
    PUBLIC void MMI_DestroyTreeTravelInfo(
        MMITREE_TRAVEL_INFO_T* travel_ptr
        );

    /*----------------------------------------------------------------------------*/
    /*                         Compiler Flag                                      */
    /*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
}
#endif

#endif /* END _MMI_TREE_NEW_H_  */
