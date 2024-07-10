/*****************************************************************************
** File Name:      watch_notifycenter.c                                      *
** Author:         longwei.qiao                                              *
** Date:           02/20/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe notify function             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2021      longwei.qiao          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 #include "mmk_type.h"
 #include "mmi_common.h"
 #include "mmicom_trace.h"
 #include "watch_notifycenter.h"
 #include "watch_notifycenter_id.h"
 #include "watch_notifycenter_shortcutwin.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/

LOCAL NOTIFY_HANDLE_T s_notify_handle = 0;
LOCAL NOTIFY_LIST_T s_notifycenter_list = {0};

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get notify handle by position from notify list
//  Parameter: [In] position: the position of node in list
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_HANDLE_T NotifyCenter_GetHandleByPos ( uint32 position )
{
    uint32 node_pos = 0;
    NOTIFY_NODE_T*  p_notify_node =PNULL;

    TRACE_APP_NOTIFYCENTER("position = %d!",position);

    p_notify_node = s_notifycenter_list.notify_head;

    if(position > s_notifycenter_list.total_num -1)
    {
        TRACE_APP_NOTIFYCENTER("error position = %d, total_num = %d",position,s_notifycenter_list.total_num);
        return 0;
    }
    while(PNULL != p_notify_node)
    {
        if(node_pos == position)
        {
            break;
        }
        else
        {
            p_notify_node = p_notify_node->next;
        }
        node_pos++;
    }

    return p_notify_node->notify_handle;
}

/*****************************************************************************/
//  Description : Get notify node by handle from notify list
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFY_NODE_T:notify node
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_NODE_T * NotifyCenter_GetNode( NOTIFY_HANDLE_T notify_handle )
{
    NOTIFY_NODE_T*  p_notify_node =PNULL;

    TRACE_APP_NOTIFYCENTER("notify_handle = %d!",notify_handle);
    p_notify_node = s_notifycenter_list.notify_head;
    while(PNULL != p_notify_node)
    {
        if(notify_handle == p_notify_node->notify_handle)
        {
            break;
        }
        else
        {
            p_notify_node= p_notify_node->next;
        }
    }

    return p_notify_node;
}

/*****************************************************************************/
//  Description : Add notify info into notify list
//  Parameter: [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC uint32 NotifyCenter_GetTotalNum( void )
{
    TRACE_APP_NOTIFYCENTER("total_num = %d!",s_notifycenter_list.total_num);

    return s_notifycenter_list.total_num;
}

/**--------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Add notify info into notify list
//  Parameter: [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_HANDLE_T NotifyCenter_Add(NOTIFYCENTER_NOTIFY_INFO_T *p_notify_info)
{
    //Create node
    NOTIFY_NODE_T* p_notify_node = PNULL;
    if(PNULL == p_notify_info)
    {
        TRACE_APP_NOTIFYCENTER("p_notify_info is NULL!");
        return NOTIFYCENTER_INPUT_ERROR;
    }

    p_notify_node = (NOTIFY_NODE_T*)SCI_ALLOC_APP(sizeof(NOTIFY_NODE_T));
    if(PNULL == p_notify_node)
    {
        TRACE_APP_NOTIFYCENTER("MALLOC ERROR!");
        return NOTIFYCENTER_MALLOC_ERROR;
    }
    SCI_MEMSET(p_notify_node, 0x00, sizeof(NOTIFY_NODE_T));
    SCI_MEMCPY(&(p_notify_node->notify_info), p_notify_info, sizeof(NOTIFYCENTER_NOTIFY_INFO_T));

    s_notify_handle++;

    p_notify_node->notify_handle = s_notify_handle;
    if(PNULL == s_notifycenter_list.notify_head)//NotifyList is empty
    {
        s_notifycenter_list.notify_tail = p_notify_node;
        s_notifycenter_list.notify_head = p_notify_node;
        s_notifycenter_list.notify_head->next = PNULL;
        s_notifycenter_list.notify_tail->next = PNULL;
        s_notifycenter_list.total_num = 1;
    }
    else
    {
        s_notifycenter_list.notify_tail->next = p_notify_node;
        s_notifycenter_list.notify_tail = s_notifycenter_list.notify_tail->next;
        s_notifycenter_list.notify_tail->next = PNULL;
        s_notifycenter_list.total_num++;
    }
    TRACE_APP_NOTIFYCENTER("s_notify_handle = %d!",s_notify_handle);
    return s_notify_handle;
}

/*****************************************************************************/
//  Description : Read notify by notify handle
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:1. This API will call app cb_NotifyRead when notify read
//       2. This API will delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Read(NOTIFY_HANDLE_T notify_handle)
{
    NOTIFY_NODE_T* cur_node_ptr = PNULL;
    NOTIFY_NODE_T* pre_node_ptr = PNULL;
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_INPUT_ERROR;
    if(s_notifycenter_list.total_num == 0)
    {
        TRACE_APP_NOTIFYCENTER("s_notifycenter_list.total_num = 0!");
        return NOTIFYCENTER_ERROR;
    }
    cur_node_ptr = s_notifycenter_list.notify_head;
    while(PNULL != cur_node_ptr)
    {
        if(cur_node_ptr->notify_handle == notify_handle)
        {
            //1.call read callback first
            if(PNULL != cur_node_ptr->notify_info.cb_NotifyRead)
            {
                TRACE_APP_NOTIFYCENTER("Call notify read callback!");
                cur_node_ptr->notify_info.cb_NotifyRead(notify_handle);
            }
            //2.delete notify node from list
            if(PNULL != pre_node_ptr)
            {
                if(cur_node_ptr->next == PNULL)
                {
                    s_notifycenter_list.notify_tail= pre_node_ptr;
                }
                pre_node_ptr->next = cur_node_ptr->next;
            }
            else
            {
                s_notifycenter_list.notify_head = cur_node_ptr->next;
            }
            SCI_FREE(cur_node_ptr);
            cur_node_ptr = PNULL;
            s_notifycenter_list.total_num--;
            ret = NOTIFYCENTER_SUCCESS;
            break;
        }
        pre_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next;
    }
    TRACE_APP_NOTIFYCENTER("can not find notify with notify_handle = %d",notify_handle);
    return ret;
}

/*****************************************************************************/
//  Description : Modify notify info by notify handle
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:1. This API will call app cb_NotifyRead when notify read
//       2. This API will delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Modify(NOTIFY_HANDLE_T notify_handle, NOTIFYCENTER_NOTIFY_INFO_T *p_notify_info)
{
    NOTIFY_NODE_T*  p_notify_node =PNULL;
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_INPUT_ERROR;

    TRACE_APP_NOTIFYCENTER("notify_handle = %d!",notify_handle);
    if(PNULL == p_notify_info)
    {
        TRACE_APP_NOTIFYCENTER("p_notify_info is NULL!");
        return NOTIFYCENTER_INPUT_ERROR;
    }
    if(s_notifycenter_list.total_num == 0)
    {
        TRACE_APP_NOTIFYCENTER("s_notifycenter_list.total_num = 0!");
        return NOTIFYCENTER_ERROR;
    }

    p_notify_node = s_notifycenter_list.notify_head;
    while(PNULL != p_notify_node)
    {
        if(notify_handle == p_notify_node->notify_handle)
        {
            //modify notify info
            SCI_MEMSET(&(p_notify_node->notify_info), 0x00, sizeof(NOTIFYCENTER_NOTIFY_INFO_T));
            SCI_MEMCPY(&(p_notify_node->notify_info), p_notify_info, sizeof(NOTIFYCENTER_NOTIFY_INFO_T));
            ret = NOTIFYCENTER_SUCCESS;
            break;
        }
        else
        {
            p_notify_node= p_notify_node->next;
        }
    }
    TRACE_APP_NOTIFYCENTER("can not find notify with notify_handle = %d",notify_handle);
    return ret;
}

/*****************************************************************************/
//  Description : Delete notify by handle from notify list
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:This API just delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Delete(NOTIFY_HANDLE_T notify_handle)
{
    NOTIFY_NODE_T* cur_node_ptr = PNULL;
    NOTIFY_NODE_T* pre_node_ptr = PNULL;
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_INPUT_ERROR;
    if(s_notifycenter_list.total_num == 0)
    {
        TRACE_APP_NOTIFYCENTER("s_notifycenter_list.total_num = 0!");
        return NOTIFYCENTER_ERROR;
    }
    cur_node_ptr = s_notifycenter_list.notify_head;
    while(PNULL != cur_node_ptr)
    {
        if(cur_node_ptr->notify_handle == notify_handle)
        {
            if(PNULL != pre_node_ptr)
            {
                if(cur_node_ptr->next == PNULL)
                {
                    s_notifycenter_list.notify_tail= pre_node_ptr;
                }
                pre_node_ptr->next = cur_node_ptr->next;
            }
            else
            {
                s_notifycenter_list.notify_head = cur_node_ptr->next;
            }
            SCI_FREE(cur_node_ptr);
            cur_node_ptr = PNULL;
            s_notifycenter_list.total_num--;
            ret = NOTIFYCENTER_SUCCESS;
            break;
        }
        pre_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next;
    }
    TRACE_APP_NOTIFYCENTER("can not find notify with notify_handle = %d",notify_handle);
    return ret;
}

/*****************************************************************************/
//  Description : Delete all notify node from notify list
//  Parameter: [In]  None
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:This API just delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_DeleteAll( void )
{
    NOTIFY_NODE_T*  cur_node_ptr = PNULL;

    if(0 != s_notifycenter_list.total_num)
    {
        while(PNULL != s_notifycenter_list.notify_head)
        {
            cur_node_ptr = s_notifycenter_list.notify_head->next;
            SCI_FREE(s_notifycenter_list.notify_head);
            s_notifycenter_list.notify_head = PNULL;
            s_notifycenter_list.notify_head = cur_node_ptr;
        }
        s_notifycenter_list.total_num = 0;
        s_notifycenter_list.notify_tail = PNULL;
    }

    return NOTIFYCENTER_SUCCESS;
}

/*****************************************************************************/
//  Description : Check wheather there is notify
//  Parameter: [In]  None
//             [Out] None
//             [Return] BOOLEAN:TRUE,FALSE
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN NotifyCenter_IsExist( void )
{
    TRACE_APP_NOTIFYCENTER("s_notifycenter_list.total_num = %d!",s_notifycenter_list.total_num);
    if(s_notifycenter_list.total_num > 0)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Init notifycenter module
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_NotifyCenter_InitModule(void)
{
    WATCH_NotifyCenter_RegWinIdNameArr();
    NotifyCenter_RegisterBTService();
    NotifyCenter_RegisterWIFIService();
}