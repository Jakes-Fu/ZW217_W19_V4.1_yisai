/*****************************************************************************
** File Name:      mmisrvap_iup.c                                            *
** Author:                                                                   *
** Date:           11/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv inferface for up      *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMISRVAP_IUP_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicom_data.h"
#include "mmi_string.h"

//other module
#include "mmisrv_fsm.h"

//local module
#include "mmisrvap_idown.h"
#include "mmisrvap_fsm.h"
#include "mmisrvap_iup.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APDestroyFSM(
    MMISRVAP_HANDLE ap_handle //in
);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL   MMI_SLISTHANDLE_T s_mmisrvap_fsm_list_ptr = PNULL;  //srv state machine list; 扩展多状态机使用
LOCAL   SCI_MUTEX_PTR s_mmisrvap_fsm_list_mutex = 0; //mutex to protect s_mmisrvap_fsm_list;
LOCAL   MMISRVAP_INFO_T s_mmisrvap_info = {0};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建fsm list mutex
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN APCreateFsmListMutex(void)
{
    BOOLEAN ret = FALSE;

    if(PNULL == s_mmisrvap_fsm_list_mutex)
    {
        s_mmisrvap_fsm_list_mutex = SCI_CreateMutex("MMISRVAP FSM LIST Mutex", SCI_NO_INHERIT);

        MMISRVAP_TRACE_LOW("[MMISRVAP] APCreateFsmListMutex s_mmisrvap_fsm_list_mutex=0x%X", \
                           s_mmisrvap_fsm_list_mutex);

    }

    if (PNULL != s_mmisrvap_fsm_list_mutex)
    {
        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : 释放fsm list mutex
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 平台mutex总数量有限制，故在不需要时释放
/*****************************************************************************/
PUBLIC void APReleaseFsmListMutex(void)
{
    //若总服务器数量为0时，才释放mutex
    if(PNULL == s_mmisrvap_fsm_list_ptr)
    {
        if(PNULL != s_mmisrvap_fsm_list_mutex)
        {
            MMISRVAP_TRACE_LOW("[MMISRVAP] APReleaseFsmListMutex s_mmisrvap_fsm_list_mutex=0x%X", \
                               s_mmisrvap_fsm_list_mutex);

            SCI_DeleteMutex(s_mmisrvap_fsm_list_mutex);
            s_mmisrvap_fsm_list_mutex = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : 上锁
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_Lock(void)
{
    if(PNULL != s_mmisrvap_fsm_list_mutex)
    {
        SCI_GetMutex(s_mmisrvap_fsm_list_mutex, SCI_WAIT_FOREVER);
    }
}

/*****************************************************************************/
//  Description :解锁
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_UnLock(void)
{
    if(PNULL != s_mmisrvap_fsm_list_mutex)
    {
        SCI_PutMutex(s_mmisrvap_fsm_list_mutex);
    }
}

/*****************************************************************************/
//  Description : 创建ap handle列表
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APCreateHandleList(
    uint32 elementSize //in
)
{
    BOOLEAN ret = FALSE;


    if (PNULL == s_mmisrvap_fsm_list_ptr)
    {
        s_mmisrvap_fsm_list_ptr = MMI_SLIST_Create(elementSize);
    }

    if (PNULL != s_mmisrvap_fsm_list_ptr)
    {
        ret = TRUE;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_CreateHandleList s_mmisrvap_fsm_list_ptr=0x%X", \
                       s_mmisrvap_fsm_list_ptr);
    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_CreateHandleList ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 销毁handle 列表
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APDestroyHandleList(
)
{

    if (PNULL == s_mmisrvap_fsm_list_ptr)
    {
        return ;
    }


    if (MMI_SLIST_IsEmpty(s_mmisrvap_fsm_list_ptr))
    {
        MMI_SLIST_Destroy(&s_mmisrvap_fsm_list_ptr);

        MMISRVAP_TRACE_LOW("[MMISRVAP] APDestroyHandleList");
    }
}

/*****************************************************************************/
//  Description : 添加ap handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APAddHandleToList(
    MMISRVAP_HANDLE ap_handle, //in
    MMI_SLISTNODEHANDLE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMI_SLISTNODEHANDLE_T node = PNULL;


    if (PNULL != s_mmisrvap_fsm_list_ptr)
    {

        node = MMI_SLIST_Append(s_mmisrvap_fsm_list_ptr, &ap_handle);
        if (PNULL != node)
        {
            if (PNULL != node_out_ptr)
            {
                *node_out_ptr = node;
            }
            ret = TRUE;
        }
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_AddHandleToList node=0x%X", \
                       node);
    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_AddHandleToList ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 删除ap handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APDelHandleFromList(
    MMISRVAP_HANDLE ap_handle //in
)
{
    BOOLEAN ret = FALSE;
    MMI_SLISTNODEHANDLE_T node = PNULL;
    MMISRVAP_HANDLE *element_ptr = PNULL;

    if (PNULL == s_mmisrvap_fsm_list_ptr)
    {
        return ret;
    }

    if (ap_handle == s_mmisrvap_info.cur_handle)
    {
        node = s_mmisrvap_info.cur_node;
        ret = TRUE;
    }
    else
    {
        MMI_SLIST_FOREACH(s_mmisrvap_fsm_list_ptr, node)
        {
            element_ptr = MMI_SLIST_GetElement(node);
            if (PNULL != element_ptr)
            {
                if (*element_ptr == ap_handle)
                {
                    ret = TRUE;
                    break;
                }
            }
        }
    }

    if (ret)
    {
        MMI_SLIST_Delete(s_mmisrvap_fsm_list_ptr, &node);
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_DelHandleFromList node=0x%X", \
                       node);
    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_DelHandleFromList ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 查找ap handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APFindHandleFromList(
    MMISRVAP_HANDLE ap_handle //in
)
{
    BOOLEAN ret = FALSE;
    MMI_SLISTNODEHANDLE_T node = PNULL;
    MMISRVAP_HANDLE *element_ptr = PNULL;

    if (PNULL == s_mmisrvap_fsm_list_ptr)
    {
        return ret;
    }


    MMI_SLIST_FOREACH(s_mmisrvap_fsm_list_ptr, node)
    {
        element_ptr = MMI_SLIST_GetElement(node);
        if (PNULL != element_ptr)
        {
            if (*element_ptr == ap_handle)
            {
                ret = TRUE;
                break;
            }
        }
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_FindHandleFromList node=0x%X", \
                       node);
    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_FindHandleFromList ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 查找第1个handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  void APGetFirstHandle(
    MMISRVAP_HANDLE *ap_handle_ptr, //out
    MMI_SLISTNODEHANDLE_T *node_out_ptr //out
)
{
    MMISRVAP_HANDLE *element_ptr = PNULL;
    MMI_SLISTNODEHANDLE_T node = PNULL;



    if (PNULL != s_mmisrvap_fsm_list_ptr)
    {
        node = MMI_SLIST_GetFirst(s_mmisrvap_fsm_list_ptr);
        element_ptr = MMI_SLIST_GetElement(node);
    }

    if (PNULL != ap_handle_ptr
            && PNULL != element_ptr
       )
    {
        *ap_handle_ptr = *element_ptr;
    }
    if (PNULL != node_out_ptr)
    {
        *node_out_ptr = node;
    }

}


/*****************************************************************************/
//  Description : 更新当前srv info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  void APUpdateCurSrvInfo(
    MMISRVAP_HANDLE ap_handle, //out
    MMI_SLISTNODEHANDLE_T node //out
)
{
    s_mmisrvap_info.cur_handle = ap_handle;
    s_mmisrvap_info.cur_node = node;
}

/*****************************************************************************/
//  Description : 查找audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APFindAudioHandleFromList(
    MMISRV_HANDLE_T audio_handle, //in
    MMISRVAP_FSM_T **fsm_out_ptr_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMI_SLISTNODEHANDLE_T node = PNULL;
    MMISRVAP_HANDLE *element_ptr = PNULL;
    MMISRVAP_HANDLE ap_handle = PNULL;

    MMISRVAP_FSM_T *fsm_ptr = PNULL;


    if (PNULL == s_mmisrvap_fsm_list_ptr)
    {
        return ret;
    }


    MMI_SLIST_FOREACH(s_mmisrvap_fsm_list_ptr, node)
    {
        element_ptr = MMI_SLIST_GetElement(node);
        if (PNULL != element_ptr)
        {
            ap_handle = *element_ptr;

            if(PNULL != ap_handle)
            {
                fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);

                if(PNULL != fsm_ptr)
                {
                    if(audio_handle == fsm_ptr->audiosrv_handle)
                    {
                        if (PNULL != fsm_out_ptr_ptr
                                && PNULL != *fsm_out_ptr_ptr
                           )
                        {
                            *fsm_out_ptr_ptr = fsm_ptr;
                            ret = TRUE;
                        }
                        break;
                    }
                }
            }
        }
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APFindAudioHandleFromList audio_handle=0x%X", \
                       audio_handle);
    MMISRVAP_TRACE_LOW("[MMISRVAP] APFindAudioHandleFromList ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 判断handle是否有效
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_IsValidHandle(
    MMISRVAP_HANDLE ap_handle //in
)
{
    BOOLEAN result = FALSE;


    if (ap_handle == s_mmisrvap_info.cur_handle)
    {
        result = TRUE;
    }
    else
    {
        //在状态机列表中查找该状态机是否存在
        result = APFindHandleFromList(ap_handle);
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_IsValidHandle result=%d, handle=0x%X", \
                       result, ap_handle);

    return result;
}

/*****************************************************************************/
//  Description : 创建状态机,并添加到列表中
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_HANDLE AP_CreateFSMToList(
    MMISRVAP_OPEN_PARAM_T *open_param_ptr //in
)
{
    BOOLEAN ret = FALSE;

    FSMSRV_HANDLE ap_handle = PNULL;
    MMISRVAP_FSM_T ap_fsm = {0};
    MMI_SLISTNODEHANDLE_T node = PNULL;


    if(PNULL == open_param_ptr)
    {
        return ap_handle;
    }

    //创建mutex
    if (!APCreateFsmListMutex())
    {
        return ap_handle;
    }


    //上锁
    AP_Lock();

    do
    {
        //初始化状态机信息
        ap_fsm.caller_callback = open_param_ptr->callback_func;
        ap_fsm.play_info.vol = open_param_ptr->vol;
        ap_fsm.play_info.is_support_bt = open_param_ptr->is_support_bt;

        //为状态机分配空间
        ap_handle = FSMSRV_Construct((FSM_OBJECT)(&ap_fsm), sizeof(ap_fsm), AP_GetIdleStateFunc());

        if(PNULL == ap_handle)
        {
            break;
        }

        ret = APCreateHandleList(sizeof(ap_handle));
        if (!ret)
        {
            break;
        }

        //将当前状态机插入到状态机列表中
        ret = APAddHandleToList(ap_handle,&node);
        if (!ret)
        {
            break;
        }

        //更新当前状态机
        APUpdateCurSrvInfo(ap_handle, node);

        MMISRVAP_TRACE_LOW("[MMISRVAP] AP_CreateFSMToList");

        ret = TRUE;
    }
    while(0);


    //没有成功，释放分配的内存
    if(!ret)
    {
        APDestroyFSM(ap_handle);
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_CreateFSMToList ret=%d, ap_handle=0x%X", \
                       ret, ap_handle);

    //解锁
    AP_UnLock();

    return (MMISRVAP_HANDLE)ap_handle;
}

/*****************************************************************************/
//  Description : 释放audio file srv数据
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APReleaseFSM(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN ret = FALSE;


    if(PNULL == fsm_ptr)
    {
        return ret;
    }

    //释放分配的内存
    if(PNULL != fsm_ptr->play_info.full_path_ptr)
    {
        SCI_FREE(fsm_ptr->play_info.full_path_ptr);
        fsm_ptr->play_info.full_path_ptr = PNULL;
    }

    //释放audio handle
    if(PNULL != fsm_ptr->audiosrv_handle)
    {
        MMISRVMGR_Free(fsm_ptr->audiosrv_handle);
        fsm_ptr->audiosrv_handle = PNULL;
    }

    ret = TRUE;

    MMISRVAP_TRACE_LOW("[MMISRVAP] APReleaseFSM ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : MMISRVAP_ResetReviewInfo
//  Global resource dependence :  none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_ResetReviewInfo(
	MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_FSM_T *fsm_ptr = PNULL;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;
	fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
	if (PNULL == fsm_ptr)
	{
		return;
	}
	review_ptr = AP_GetReviewInfoPtr(fsm_ptr);
	if (PNULL == review_ptr)
	{
		return;
	}
    SCI_MEMSET(review_ptr, 0, sizeof(*review_ptr));
	MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_ResetReviewInfo");
}

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APDestroyFSM(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_FSM_T *fsm_ptr = PNULL;
    MMISRVAP_HANDLE new_handle = PNULL;
    MMI_SLISTNODEHANDLE_T new_node = PNULL;



    if (PNULL == ap_handle)
    {
        return;
    }

    //stop timer
    fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
    AP_StopPeriodTimer(fsm_ptr);


    // 1.从状态机列表中删除该状态机
    APDelHandleFromList(ap_handle);

    // 2. 更新当前状态机
    APGetFirstHandle(&new_handle, &new_node);
    APUpdateCurSrvInfo(new_handle, new_node);


    // 3.释放audio file srv数据
    if(PNULL != fsm_ptr)
    {
        APReleaseFSM(fsm_ptr);
    }

    // 4.销毁fsm srv handle
    FSMSRV_Destruct((FSMSRV_HANDLE*)&ap_handle);


    //5.销毁handle list
    APDestroyHandleList();


    MMISRVAP_TRACE_LOW("[MMISRVAP] APDestroyFSM ap_handle = 0x%X", \
                       ap_handle);
    MMISRVAP_TRACE_LOW("[MMISRVAP] APDestroyFSM new_handle = 0x%X", \
                       new_handle);

}

/*****************************************************************************/
//  Description : 开启audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_HANDLE MMISRVAP_Open(
    MMISRVAP_OPEN_PARAM_T *open_param_ptr //in
)
{
    MMISRVAP_HANDLE ap_handle = PNULL;


    ap_handle = AP_CreateFSMToList(open_param_ptr);


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Open ap_handle=0x%X", ap_handle);

    return ap_handle;
}

/*****************************************************************************/
//  Description : 关闭audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_Close(
    MMISRVAP_HANDLE ap_handle //in
)
{

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Close ap_handle=0x%X", \
                       ap_handle);

    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle))
    {
        AP_UnLock();
        return;
    }

    APDestroyFSM(ap_handle);

    AP_UnLock();


    APReleaseFsmListMutex();
}

/*****************************************************************************/
//  Description : 序列化播放参数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 为状态机消息分配连续空间，便于后续释放
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E AP_Serialize_Play(
    MMISRVAP_REQ_PLAY_PARAM_T *param_ptr, //in
    uint32 size_of_param, //in
    void **msg_body_ptr, //out
    uint32 *size_of_msg_body //out
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NOT_ENOUGH_MEMORY;

    void *serialize_ptr = PNULL;
    uint32 size_of_serialize = 0;

    MMISRVAP_REQ_PLAY_PARAM_T *param2_ptr = PNULL;


    //为状态机消息分配连续空间，便于后续释放

    do
    {

        if(PNULL == param_ptr
                || size_of_param != sizeof(MMISRVAP_REQ_PLAY_PARAM_T)
          )
        {
            break;
        }

        //获取序列化的长度
        size_of_serialize = size_of_param \
                            + ((param_ptr->full_path_len + 1) * sizeof(wchar)) \
                            + sizeof(void *);

        //序列化
        serialize_ptr = (void*)SCI_ALLOCA(size_of_serialize);

        if(PNULL == serialize_ptr)
        {
            break;
        }

        SCI_MEMSET(serialize_ptr, 0, size_of_serialize);

        //参数结构序列化
        SCI_MEMCPY(serialize_ptr, param_ptr, size_of_param);

        //文件名称序列化
        param2_ptr = serialize_ptr;
        param2_ptr->full_path_ptr = (wchar *)((uint32)serialize_ptr + sizeof(MMISRVAP_REQ_PLAY_PARAM_T));
        param2_ptr->full_path_ptr = (wchar *)((uint32)(param2_ptr->full_path_ptr) \
                                              + MMIAPICOM_GetAlign4Offset((uint32)(param2_ptr->full_path_ptr)));
        MMIAPICOM_Wstrncpy(param2_ptr->full_path_ptr, param_ptr->full_path_ptr, param_ptr->full_path_len);


        *msg_body_ptr = serialize_ptr;
        *size_of_msg_body = size_of_serialize;

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_Serialize_Play size_of_serialize=%d", \
                       size_of_serialize);

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_Serialize_Play serialize_ptr=0x%X", \
                       serialize_ptr);

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_Serialize_Play error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 派发请求数据
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APRequestDispatch(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_FSM_MSG_E msg_id, //in
    void *param_ptr, //in
    uint32 size_of_param //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NONE;
    BOOLEAN ret = FALSE;

    BOOLEAN is_free = FALSE;
    void *msg_body_ptr = PNULL;
    uint32 msg_body_size = 0;



    //参数检查
    if(PNULL == ap_handle)
    {
        error_code = MMISRVAP_ERROR_PARAM;
        return error_code;
    }

    // 1.序列化
    switch(msg_id)
    {
    case MMISRVAP_PLAY_REQ:
        is_free = TRUE;
        error_code = AP_Serialize_Play(param_ptr, size_of_param,
                                       &msg_body_ptr, &msg_body_size);
        break;

    default:
        error_code = MMISRVAP_ERROR_NONE;
        msg_body_ptr = param_ptr;
        msg_body_size = size_of_param;
        break;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APRequestDispatch error_code=%d",
                       error_code);

    // 2.dispatch
    if(MMISRVAP_ERROR_NONE == error_code)
    {
        ret = AP_FsmDispatch(ap_handle, \
                             msg_id, \
                             msg_body_ptr, \
                             msg_body_size
                            );

        if(ret)
        {
            error_code = AP_GetErrorCodeByHandle(ap_handle);
        }
        else
        {
            error_code = MMISRVAP_ERROR_UNKNOWN;
        }
    }

    // 3.释放序列化的内存
    if(is_free)
    {
        if(PNULL != msg_body_ptr)
        {
            SCI_FREE(msg_body_ptr);
        }
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APRequestDispatch return error_code=%d",
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 播放文件
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Play(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_PLAY_PARAM_T *play_param_ptr, //in
    uint32 size_of_param //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || PNULL == play_param_ptr
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_PLAY_REQ, \
                                   (void*)play_param_ptr, \
                                   size_of_param \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Play error_code=%d", \
                       error_code);
    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 暂停播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Pause(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_PAUSE_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Pause error_code=%d", \
                       error_code);
    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 恢复播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Resume(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_RESUME_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Resume error_code=%d", \
                       error_code);
    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 停止播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Stop(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_STOP_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_Stop error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 搜索
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SeekTo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_SEEK_PARAM_T *seek_param_ptr, //in
    uint32 size_of_param //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || PNULL == seek_param_ptr
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_SEEK_REQ, \
                                   (void*)seek_param_ptr, \
                                   size_of_param \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SeekTo error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 设置错误码
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:使用内部参数
/*****************************************************************************/
PUBLIC void AP_SetErrorCode(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_ERROR_E error_code //in
)
{

    if(PNULL != fsm_ptr)
    {
        fsm_ptr->error_code = error_code;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_SetErrorCode error_code=%d", \
                       error_code);
}

/*****************************************************************************/
//  Description : 通过ap handle设置错误码
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_SetErrorCodeByHandle(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_ERROR_E error_code //in
)
{
    MMISRVAP_FSM_T *fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);


    if(PNULL != fsm_ptr)
    {
        fsm_ptr->error_code = error_code;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_SetErrorCodeByHandle error_code=%d", \
                       error_code);
}

/*****************************************************************************/
//  Description : 获得错误码
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:使用内部参数
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E AP_GetErrorCode(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = 0;


    if(PNULL != fsm_ptr)
    {
        error_code = fsm_ptr->error_code;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_GetErrorCode error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 通过ap handle获得错误码
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E AP_GetErrorCodeByHandle(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = 0;
    MMISRVAP_FSM_T *fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);


    if(PNULL != fsm_ptr)
    {
        error_code = fsm_ptr->error_code;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_GetErrorCodeByHandle error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 获得播放中的信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_GetPlayingInfo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_PLAY_INFO_T *playing_info_ptr //out
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_FSM_T *fsm_ptr = PNULL;

    MMISRV_HANDLE_T audio_handle = PNULL;
    BOOLEAN ret = FALSE;



    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
            || (PNULL == playing_info_ptr)
      )
    {
        AP_UnLock();
        return error_code;
    }



    do
    {
        fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
        if (PNULL == fsm_ptr)
        {
            break;
        }
        audio_handle = fsm_ptr->audiosrv_handle;

        if(PNULL == audio_handle)
        {
            break;
        }

        ret = MMISRVAUD_GetPlayingInfo(audio_handle, playing_info_ptr);

        MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_GetPlayingInfo ret=%d", \
                           ret);

        if(!ret)
        {
            break;
        }

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_GetPlayingInfo error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;

}

/*****************************************************************************/
//  Description : 根据audio handle获得ap srv 的回调参数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_GetAPInfoByAudioHandle(
    MMISRV_HANDLE_T audio_handle, //in
    MMISRVAP_HANDLE *ap_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMISRVAP_FSM_T *fsm_ptr = PNULL;
    MMISRVAP_HANDLE ap_handle = PNULL;



    if(PNULL == audio_handle
            || PNULL == ap_out_ptr
      )
    {
        return ret;
    }


    //查找audio_handle对应的ap handle
    ap_handle = s_mmisrvap_info.cur_handle;

    do
    {
        if(PNULL != ap_handle)
        {
            fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);

            if(PNULL != fsm_ptr)
            {
                if(audio_handle == fsm_ptr->audiosrv_handle)
                {
                    ret = TRUE;
                    break;
                }
            }
        }

        //从状态机列表中，查找audio handle
        ret = APFindAudioHandleFromList(audio_handle,
                                        &fsm_ptr
                                       );
    }
    while(0);

    if(ret
            && PNULL != fsm_ptr
      )
    {
        *ap_out_ptr = fsm_ptr->ap_handle;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_GetAPInfoByAudioHandle ret=%d", \
                       ret);

    return ret;
}

/*****************************************************************************/
//  Description : 设置音量
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetVolume(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_VOLUME_T vol //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_FSM_T *fsm_ptr = PNULL;
    MMISRV_HANDLE_T audio_handle = PNULL;



    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    do
    {
        fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
        if (PNULL == fsm_ptr)
        {
            break;
        }

        audio_handle = fsm_ptr->audiosrv_handle;

        if(PNULL == audio_handle)
        {
            break;
        }

        MMISRVAUD_SetVolume(audio_handle, vol);

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SetVolume error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;
}

/*****************************************************************************/
//  Description : 设置复读位置
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetReviewPoint(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_REVIEW_PARAM_T *review_param_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_FSM_T *fsm_ptr = PNULL;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
            || PNULL == review_param_ptr
      )
    {
        AP_UnLock();
        return error_code;
    }

    do
    {
        fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
        if (PNULL == fsm_ptr)
        {
            break;
        }
        review_ptr = AP_GetReviewInfoPtr(fsm_ptr);
        if (PNULL == review_ptr)
        {
            break;
        }
        review_ptr->is_support_review = TRUE;
        review_ptr->start_point = review_param_ptr->start_point;
        review_ptr->end_point = review_param_ptr->end_point;
        if (0!= review_ptr->end_point)
        {
            review_ptr->is_end_file = FALSE;
        }
        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SetReviewStartPoint start_point=%d", \
                       review_param_ptr->start_point);
    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SetReviewStartPoint end_point=%d", \
                       review_param_ptr->end_point);

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SetReviewStartPoint error_code=%d", \
                       error_code);


    AP_UnLock();

    return error_code;
}

/*****************************************************************************/
//  Description : 启动复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StartReview(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;



    AP_Lock();

    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_REVIEW_START_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SeekTo error_code=%d", \
                       error_code);
    AP_UnLock();

    return error_code;

}

/*****************************************************************************/
//  Description : 停止复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StopReview(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_REVIEW_STOP_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_SeekTo error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;

}

/*****************************************************************************/
//  Description : 获得播放文件的信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_GetContentInfo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_CONTENT_INFO_T *content_info_ptr //out
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_FSM_T *fsm_ptr = PNULL;

    MMISRV_HANDLE_T audio_handle = PNULL;
    BOOLEAN ret = FALSE;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
            || (PNULL == content_info_ptr)
      )
    {
        AP_UnLock();
        return error_code;
    }


    do
    {

        fsm_ptr = FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);
        if (PNULL == fsm_ptr)
        {
            break;
        }

        audio_handle = fsm_ptr->audiosrv_handle;

        if(PNULL == audio_handle)
        {
            break;
        }

        ret = MMISRVAUD_GetContentInfo(audio_handle, content_info_ptr);

        MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_GetContentInfo ret=%d", \
                           ret);

        if(!ret)
        {
            break;
        }

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_GetContentInfo error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;

}

/*****************************************************************************/
//  Description : mgr resume
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 让其他handle处于非激活状态，
//       让本handle处于激活状态
//    1) 若是被audio srv暂停的,会有resume动作
//    2) 若未被audio srv暂停，保持原来状态
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_MGR_Resume(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_MGR_RESUME_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_MGR_Resume error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;

}

/*****************************************************************************/
//  Description : release audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_ReleaseAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_RELEASE_AUDIO_HANDLE_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );

    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_ReleaseAudioHandle error_code=%d", \
                       error_code);

    AP_UnLock();

    return error_code;

}

/*****************************************************************************/
//  Description : request audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_RequestAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    AP_Lock();
    if(PNULL == ap_handle
            || !AP_IsValidHandle(ap_handle)
      )
    {
        AP_UnLock();
        return error_code;
    }

    error_code = APRequestDispatch(ap_handle, \
                                   MMISRVAP_REQUEST_AUDIO_HANDLE_REQ, \
                                   (void*)PNULL, \
                                   0 \
                                  );


    MMISRVAP_TRACE_LOW("[MMISRVAP] MMISRVAP_RequestAudioHandle error_code=%d", \
                       error_code);

    AP_UnLock();
    return error_code;

}

