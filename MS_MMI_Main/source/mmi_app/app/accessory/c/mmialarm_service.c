/******************************************************************************
** File Name:      mmialarm_service.c                                         *
** Author:                                                                    *
** Date:           09/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe alarm service                *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME               DESCRIPTION                              *
** 09/2009        xiaoqing.lu        Create                                   *
** 09/2009        Xiaoqing.Lu        Modified                                 *
******************************************************************************/
//#include "sci_types.h"
#include "mmi_app_accessory_trc.h"
#include "mmialarm_export.h"
#include "mmialarm_service.h"
#include "mmi_link.h"
#include "mmicom_time.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/
#define MMIALMSVC_INVLID_SVC_ID  0x0        // 无效的服务ID
#define MMIALMSVC_MAX_SVC_ID     0xFFFFFFFF // 服务ID最大值
#define MMIALMSVC_INVLID_YEAR    0xFFFF     // 无效的年份，初始化使用

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/ 
LOCAL MMI_LINK_NODE_T *s_alarm_service_list = PNULL; // 记录所有的服务事件

LOCAL uint32           s_svcid_count = 0;
LOCAL BOOLEAN          s_is_svcid_cyc = FALSE; // 判断service id是否已经经过了一个循环，已经达到过一次0xFFFFFFFF

//alarm服务模块事件注册信息
typedef struct
{
    BOOLEAN					   	is_on;      // 是否开启
    BOOLEAN					   	is_valid;   // 是否有效服务
    MMIALMSVC_ID_T              svc_id;     // 服务事件ID
    
    // 当前时间
    SCI_ALARM_T                 event_time;
    MMIALMSVC_EVENT_TYPE_E      event_type; // 事件类型
	MMIALMSVC_HANDLE_CALLBACK 	callback;   // 事件处理函数
}MMIALMSVC_EVENT_INFO_T;

typedef enum
{
    MMIALMSVC_EQUAL =0,
    MMIALMSVC_FIRST_SMALL,
    MMIALMSVC_SECOND_SMALL
}MMIALMSVC_CMP_E;

/**--------------------------------------------------------------------------*
**                         LOCAL FUNC                                        *
**--------------------------------------------------------------------------*/ 
/*****************************************************************************/
// 	Description : 生成事件服务ID
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GenerateEventSvcID(void);

/*****************************************************************************/
// 	Description : CompareSvcDateTime
//         compare date and time to another group, find smaller(earlier)
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL MMIALMSVC_CMP_E CompareSvcDateTime(
                                         SCI_ALARM_T first, 
                                         SCI_ALARM_T second
                                         );
/*****************************************************************************/
// 	Description : 创建一个服务信息结点指针
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMIALMSVC_EVENT_INFO_T *CreateSvcInfo(
                                            MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                            BOOLEAN is_valid // 是否开启闹钟
                                            );

/*****************************************************************************/
// 	Description : Free the struct of MMIALMSVC_EVENT_INFO_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DestroySvcInfo(
                          MMI_LINK_NODE_T const * node_ptr
                          );

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchSvcInListById(
                                           MMI_LINK_NODE_T *head_node_ptr, 
                                           MMIALMSVC_ID_T svc_id
                                           );

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
// LOCAL MMI_LINK_NODE_T *SearchSvcInListByTime(
//                                              MMI_LINK_NODE_T *head_node_ptr, 
//                                              SCI_ALARM_T event_time
//                                              );

/*****************************************************************************/
// 	Description : start alarm when interrupt of alarm is coming
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE
//	Note:    
/*****************************************************************************/
LOCAL BOOLEAN StartAlmService(void);

/*****************************************************************************/
// 	Description : 设置闹钟服务
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
LOCAL void SetAlmService(void);

/*****************************************************************************/
// 	Description : 根据时间生成列表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: 返回链表的头指针
//	Note:    
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *GenerateHappendSvcList(
                                               MMI_LINK_NODE_T *head_node_ptr,
                                               SCI_ALARM_T event_time
                                               );

/*****************************************************************************/
// 	Description : 删除列表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: 返回链表的头指针
//	Note:    
/*****************************************************************************/
LOCAL void DeleteSvcList(
                         MMI_LINK_NODE_T *head_node_ptr
                         );

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 生成事件服务ID
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GenerateEventSvcID(void)
{
    if (s_svcid_count == MMIALMSVC_MAX_SVC_ID || s_is_svcid_cyc)
    {
        s_svcid_count++;
        if (s_svcid_count == 0) // 确保com id不能为0
        {
            s_svcid_count = 1;
        }
        // 查找未使用的ID
        while (SearchSvcInListById(s_alarm_service_list, s_svcid_count))
        {
            s_svcid_count++;
        } 
    }
    else
    {
        s_svcid_count++;
        if(s_svcid_count == MMIALMSVC_MAX_SVC_ID) // 达到了一个循环，设置状态
        {
            s_is_svcid_cyc = TRUE;
        }
    }

    return s_svcid_count;
}

/*****************************************************************************/
// 	Description : CompareSvcDateTime
//         compare date and time to another group, find smaller(earlier)
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL MMIALMSVC_CMP_E CompareSvcDateTime(
                                         SCI_ALARM_T first, 
                                         SCI_ALARM_T second
                                         )
{
	if(first.year > second.year)
	{
		return MMIALMSVC_SECOND_SMALL;
	}
	else if(first.year < second.year)
	{
		return MMIALMSVC_FIRST_SMALL;
	}
	else
	{
		if(first.mon > second.mon)
		{
			return MMIALMSVC_SECOND_SMALL;
		}
		else if(first.mon < second.mon)
		{
			return MMIALMSVC_FIRST_SMALL;
		}
		else
		{
			if(first.mday > second.mday)
			{
				return MMIALMSVC_SECOND_SMALL;
			}
			else if(first.mday < second.mday)
			{
				return MMIALMSVC_FIRST_SMALL;
			}
			else
			{
				if(first.hour > second.hour)
				{
					return MMIALMSVC_SECOND_SMALL;
				}
				else if(first.hour < second.hour)
				{
					return MMIALMSVC_FIRST_SMALL;
				}
				else
				{
					if(first.min >second.min)
					{
						return MMIALMSVC_SECOND_SMALL;
					}
					else if(first.min <second.min)
					{
						return MMIALMSVC_FIRST_SMALL;
					}
					else
					{
						return MMIALMSVC_EQUAL;
					}
				}
			}
		}
	}
}

/*****************************************************************************/
// 	Description : 创建一个服务信息结点指针
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMIALMSVC_EVENT_INFO_T *CreateSvcInfo(
                                            MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                            BOOLEAN is_valid // 是否开启闹钟
                                            )
{
    MMIALMSVC_EVENT_INFO_T *new_event_info_ptr = PNULL;

    if (event_info_ptr != PNULL)
    {
        new_event_info_ptr = SCI_ALLOC_APP(sizeof(MMIALMSVC_EVENT_INFO_T));
        if (PNULL != new_event_info_ptr)
        {
            SCI_MEMSET(new_event_info_ptr, 0, sizeof(MMIALMSVC_EVENT_INFO_T));
            new_event_info_ptr->is_on = TRUE;
            new_event_info_ptr->is_valid = is_valid;
            new_event_info_ptr->svc_id = GenerateEventSvcID();
            new_event_info_ptr->event_time.year = event_info_ptr->event_time.year;
            new_event_info_ptr->event_time.mon = event_info_ptr->event_time.mon;
            new_event_info_ptr->event_time.mday = event_info_ptr->event_time.mday;
            new_event_info_ptr->event_time.hour = event_info_ptr->event_time.hour;
            new_event_info_ptr->event_time.min = event_info_ptr->event_time.min;
            new_event_info_ptr->event_time.sec = event_info_ptr->event_time.sec;
            new_event_info_ptr->event_type = event_info_ptr->event_type;
            new_event_info_ptr->callback = event_info_ptr->callback;
        }
    }

    return new_event_info_ptr;
}

/*****************************************************************************/
// 	Description : Free the struct of MMIALMSVC_EVENT_INFO_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DestroySvcInfo(
                          MMI_LINK_NODE_T const * node_ptr
                          )
{
    MMIALMSVC_EVENT_INFO_T *event_info_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    event_info_ptr = (MMIALMSVC_EVENT_INFO_T*)node_ptr->data;

    if (event_info_ptr != PNULL)
    {
        SCI_FREE(event_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchSvcInListById(
                                           MMI_LINK_NODE_T *head_node_ptr, 
                                           MMIALMSVC_ID_T svc_id
                                           )
{
    BOOLEAN is_find = FALSE;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;

    tmp_ptr = head_node_ptr;

    if (PNULL != tmp_ptr)
    {
        do 
        {
            // 根据ID查找，找到之后返回指针
            if (PNULL != ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data) 
                && ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->svc_id == svc_id )
            {
                is_find = TRUE;
                break;
            }
            else
            {
                tmp_ptr = tmp_ptr->next_ptr;
            }
        } while(tmp_ptr != head_node_ptr);
    }

    // 如果根本没有找到，则初始化PNULL
    if (!is_find)
    {
        tmp_ptr = PNULL;
    }

    return tmp_ptr;    
}

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
// LOCAL MMI_LINK_NODE_T *SearchSvcInListByTime(
//                                              MMI_LINK_NODE_T *head_node_ptr, 
//                                              SCI_ALARM_T event_time
//                                              )
// {
//     BOOLEAN is_find = FALSE;
//     MMI_LINK_NODE_T *tmp_ptr = PNULL;
// 
//     tmp_ptr = head_node_ptr;
// 
//     if (PNULL != tmp_ptr)
//     {
//         do 
//         {
//             // 根据ID查找，找到之后返回指针
//             if (PNULL != ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data) 
//                 && MMIALMSVC_EQUAL == CompareSvcDateTime(event_time, (((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->event_time)))
//             {
//                 is_find = TRUE;
//                 break;
//             }
//             else
//             {
//                 tmp_ptr = tmp_ptr->next_ptr;
//             }
//         } while(tmp_ptr != head_node_ptr);
//     }
// 
//     // 如果根本没有找到，则初始化PNULL
//     if (!is_find)
//     {
//         tmp_ptr = PNULL;
//     }
// 
//     return tmp_ptr;    
// }

/*****************************************************************************/
// 	Description : 根据时间生成列表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: 返回链表的头指针
//	Note:    
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *GenerateHappendSvcList(
                                              MMI_LINK_NODE_T *head_node_ptr,
                                              SCI_ALARM_T event_time
                                              )
{
//     MMIALMSVC_ID_T svc_id = MMIALMSVC_INVLID_SVC_ID;
    MMI_LINK_NODE_T *tmp_head_node_ptr = PNULL;
    MMI_LINK_NODE_T *result_node_ptr = PNULL;
    MMIALMSVC_EVENT_INFO_T *new_event_info_ptr = PNULL;
    
    tmp_head_node_ptr = head_node_ptr;

    while(PNULL != tmp_head_node_ptr)                     
    {
        // 得到节点数据
        new_event_info_ptr = ((MMIALMSVC_EVENT_INFO_T *)tmp_head_node_ptr->data);
        // 创建链表
        if (new_event_info_ptr->is_valid
            && MMIALMSVC_EQUAL == CompareSvcDateTime(event_time, (new_event_info_ptr->event_time)))
        {
            if (PNULL == result_node_ptr)
            {
                result_node_ptr = MMILINK_CreateHead((uint32)new_event_info_ptr);
            }
            else
            {
                MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_event_info_ptr);
                MMILINK_AddNodeBeforeBaseNode(result_node_ptr, tmp_node_ptr);
            }
        }
        // 指向下一个节点
        tmp_head_node_ptr = tmp_head_node_ptr->next_ptr;
        // 判断是否和头节点相同
        if (head_node_ptr == tmp_head_node_ptr)
        {
            break;
        }
    }

    return result_node_ptr;
}

/*****************************************************************************/
// 	Description : 根据时间生成列表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: 返回链表的头指针
//	Note:    
/*****************************************************************************/
LOCAL void DeleteSvcList(
                         MMI_LINK_NODE_T *head_node_ptr
                         )
{
    MMILINK_DestroyLink(head_node_ptr, PNULL);
}

/*****************************************************************************/
// 	Description : start alarm when interrupt of alarm is coming
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN StartAlmService(void)
{
    BOOLEAN         is_success = FALSE;
    BOOLEAN         is_deal = FALSE;
    SCI_ALARM_T		cur_time = {0};
//     SCI_ALARM_T		event_time = {0};
    SCI_DATE_T      date        = {0};
    SCI_TIME_T      time        = {0};
    MMI_LINK_NODE_T *link_node_ptr = PNULL;
    MMI_LINK_NODE_T *happen_svc_list_ptr = PNULL; // 记录所有的同时发生的服务事件

    // 获取当前系统时间
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_time.hour = time.hour;
    cur_time.min = time.min;
    cur_time.year = date.year;
    cur_time.mon = date.mon;
    cur_time.mday = date.mday;

    //SCI_TRACE_LOW:"StartAlmService:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_462_112_2_18_2_1_21_63,(uint8*)"ddddd",cur_time.year,cur_time.mon,cur_time.mday,cur_time.hour,cur_time.min);

    // 生成同一时刻的链表
    happen_svc_list_ptr = GenerateHappendSvcList(s_alarm_service_list, cur_time);
    // 根据优先级执行事件.
    // 1、执行系统ALMSVC_EVENT_SYSTEM
    link_node_ptr = happen_svc_list_ptr;
    while (PNULL != link_node_ptr 
        && PNULL != ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data))
    {
        //SCI_TRACE_LOW:"StartAlmService:while"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_472_112_2_18_2_1_21_64,(uint8*)"");
        if (!is_deal && TRUE == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid
            && ALMSVC_EVENT_SYSTEM == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type)
        {
            //SCI_TRACE_LOW:"StartAlmService:alarm callback"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_476_112_2_18_2_1_21_65,(uint8*)"");
            is_deal = TRUE; // 系统只执行一次(闹钟、日程、开关机)
            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
        }

        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }
    // 2、执行ALMSVC_EVENT_TV
    link_node_ptr = happen_svc_list_ptr;
    while (PNULL != link_node_ptr 
        && PNULL != ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data))
    {
        if (TRUE == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid
            && ALMSVC_EVENT_TV == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type)
        {
            // 服务启动之后，设置为无效
            ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid = FALSE;

            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
       }
        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }
    // 2、执行ALMSVC_EVENT_OTHER
    link_node_ptr = happen_svc_list_ptr;
    while (PNULL != link_node_ptr 
        && PNULL != ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data))
    {
        if (TRUE == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid
            && ALMSVC_EVENT_OTHER == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type)
        {
            // 服务启动之后，设置为无效
            ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid = FALSE;

            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
        }

        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }

    // 执行完之后删除链表
    if (PNULL != happen_svc_list_ptr)
    {
        DeleteSvcList(happen_svc_list_ptr);
    }
    
    // 设置下一个闹钟
    SetAlmService();
    
    return is_success;
}

/*****************************************************************************/
// 	Description : 设置闹钟服务
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
LOCAL void SetAlmService(void)
{
    SCI_DATE_T      date                = {0};
    SCI_TIME_T      time                = {0};
    SCI_ALARM_T		latest_event_time   = {0};
    SCI_ALARM_T		cur_time            = {0};
    MMI_LINK_NODE_T *tmp_ptr            = PNULL;

    tmp_ptr = s_alarm_service_list;

    if (PNULL != tmp_ptr)
    {
        // 获取当前系统时间
        TM_GetSysDate(&date);
        TM_GetSysTime(&time);
        cur_time.hour = time.hour;
        cur_time.min = time.min;
        cur_time.year = date.year;
        cur_time.mon = date.mon;
        cur_time.mday = date.mday;
        latest_event_time.year = MMIALMSVC_INVLID_YEAR;
        //SCI_TRACE_LOW:"SetAlmService:cur_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_566_112_2_18_2_1_21_66,(uint8*)"ddddd",cur_time.year,cur_time.mon,cur_time.mday,cur_time.hour,cur_time.min);
        
        while(tmp_ptr != PNULL) 
        {
            // 根据ID查找，找到之后返回指针
            if (PNULL != ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data) 
                && ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->is_valid
                && MMIALMSVC_FIRST_SMALL == CompareSvcDateTime(cur_time, (((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->event_time))
                && MMIALMSVC_SECOND_SMALL == CompareSvcDateTime(latest_event_time, (((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->event_time)))
            {
                latest_event_time = ((MMIALMSVC_EVENT_INFO_T*)tmp_ptr->data)->event_time;
            }
            tmp_ptr = tmp_ptr->next_ptr;
            if (tmp_ptr == s_alarm_service_list)
            {
                break;
            }
        }
        //SCI_TRACE_LOW:"SetAlmService:latest_event_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_585_112_2_18_2_1_21_67,(uint8*)"ddddd",latest_event_time.year,latest_event_time.mon,latest_event_time.mday,latest_event_time.hour,latest_event_time.min);
        // 设置闹钟
        if(MMIALMSVC_INVLID_YEAR == latest_event_time.year)    // not find valid event 
        {
            TM_DisableAlarm(); 
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIALARM SERVICE]SetAlmService enter!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_593_112_2_18_2_1_21_68,(uint8*)"");
            if (TM_SetAlarm(latest_event_time)!= ERR_TM_NONE)
            {
                //SCI_PASSERT(0, ("SetAlmService: ERR_TM_NONE!")); /*assert verified*/       // error should not be happened    
                //SCI_TRACE_LOW:"SetAlmService: ERR_TM_NONE!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_597_112_2_18_2_1_21_69,(uint8*)"");
            }
        }
    }
    else
    {
        TM_DisableAlarm();
    }

    return;    
}

/*****************************************************************************/
// 	Description : 判断服务是否有效?
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  
//	Note:当service为0或者svc列表中没有该id对应的svc时，返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_IsValidSvc(
                                    MMIALMSVC_ID_T svc_id
                                    )
{
    BOOLEAN result = FALSE;

    if (MMIALMSVC_INVLID_SVC_ID != svc_id)
    {
        if (PNULL != SearchSvcInListById(s_alarm_service_list, svc_id))
        {
            result = TRUE;
        }
    }
    return result;
}

// Added by sunhongzhe for snoozed calendar events.
PUBLIC BOOLEAN MMIALMSVC_IsValidTimedSvc(
                                    MMIALMSVC_ID_T svc_id,
                                    SCI_ALARM_T cur_time
                                    )
{
    BOOLEAN result = FALSE;

    if (MMIALMSVC_INVLID_SVC_ID != svc_id)
    {
        MMI_LINK_NODE_T *p_node = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != p_node)
        {
            MMIALMSVC_EVENT_INFO_T *p_info = (MMIALMSVC_EVENT_INFO_T *)p_node->data;

            if (PNULL != p_info
                && p_info->is_valid
                && MMIALMSVC_FIRST_SMALL == CompareSvcDateTime(cur_time, p_info->event_time))
            {
                result = TRUE;
            }
        }
    }
    return result;
}
// End

/*****************************************************************************/
// 	Description : 更新服务内容
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_UpdateServcice(
                                        MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                        MMIALMSVC_ID_T svc_id,
                                        BOOLEAN is_valid,
                                        BOOLEAN is_set_alarm
                                        )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;

    // 如果无效，就不用查链表了，节省时间
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            tmp_event_ptr =  ((MMIALMSVC_EVENT_INFO_T*)node_ptr->data);
            if (PNULL != tmp_event_ptr)
            {
                tmp_event_ptr->callback = event_info_ptr->callback;
                tmp_event_ptr->event_type = event_info_ptr->event_type;
                tmp_event_ptr->event_time.year = event_info_ptr->event_time.year;
                tmp_event_ptr->event_time.mon = event_info_ptr->event_time.mon;
                tmp_event_ptr->event_time.mday = event_info_ptr->event_time.mday;
                tmp_event_ptr->event_time.hour = event_info_ptr->event_time.hour;
                tmp_event_ptr->event_time.min = event_info_ptr->event_time.min;
                tmp_event_ptr->event_time.sec = event_info_ptr->event_time.sec;
                tmp_event_ptr->is_valid = is_valid;
                // 设置有效或无效之后，需要重新设置闹钟
                if (is_set_alarm)
                {
                    SetAlmService();
                }
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 设置服务是否有效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE，返回设置前的状态
//	Note: 服务被启动之后将自动设置为无效状态
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_SetServciceValid(
                                          MMIALMSVC_ID_T svc_id,
                                          BOOLEAN is_valid,
                                          BOOLEAN is_set_alarm // 是否开启闹钟
                                          )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;

    // 如果无效，就不用查链表了，节省时间
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            tmp_event_ptr =  ((MMIALMSVC_EVENT_INFO_T*)node_ptr->data);
            if (PNULL != tmp_event_ptr)
            {
                result = tmp_event_ptr->is_valid;
                // 判断是否需要重新设置闹钟，如果状态相同，则不需要重新设置
                if (tmp_event_ptr->is_valid != is_valid)
                {
                    if (is_set_alarm) // 如果状态不同，则根据输入参数判断是否要设置闹钟。
                    {
                        SetAlmService();
                    }

                    tmp_event_ptr->is_valid = is_valid;
                }
            }
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 初始化事件服务链表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC MMIALMSVC_ID_T MMIALMSVC_RegService(
                                           MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                           BOOLEAN is_valid, // 注册的服务是否有效
                                           BOOLEAN is_set_alarm // 是否开启闹钟
                                           )
{
    MMIALMSVC_ID_T svc_id = MMIALMSVC_INVLID_SVC_ID;
    MMIALMSVC_EVENT_INFO_T *new_event_info_ptr = PNULL;
    
    // 创建一个服务信息节点
    new_event_info_ptr = CreateSvcInfo(event_info_ptr, is_valid);
    // 参数检查
    if (PNULL != event_info_ptr 
        && event_info_ptr->event_type < ALMSVC_EVENT_MAX
        && PNULL != new_event_info_ptr)
    {

        if (PNULL == s_alarm_service_list)
        {
            s_alarm_service_list = MMILINK_CreateHead((uint32)new_event_info_ptr);
        }
        else
        {
            MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_event_info_ptr);
            MMILINK_AddNodeBeforeBaseNode(s_alarm_service_list, tmp_node_ptr);
        }

        svc_id = new_event_info_ptr->svc_id;
        // 设置起闹
        if(is_set_alarm)
        {
            SetAlmService();
        }
    }
    else
    {
        if (PNULL != new_event_info_ptr)
        {
            SCI_FREE(new_event_info_ptr);
        }
    }

    return svc_id;
}
/*****************************************************************************/
// 	Description : 释放alm
//	Global resource dependence : 
//  Author: liyan.zhu
//  RETRUN:  TRUE: init success!
//	Note:    CR:136024 COPY MMIALMSVC_UnRegService
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_FreeAlmRegService(
                                      MMIALMSVC_ID_T svc_id,
                                      BOOLEAN is_set_alarm // 是否更新闹钟服务
                                      )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
	//     MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;
    
    // 如果无效，就不用查链表了，节省时间
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        // 查询服务
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            // 删除服务
            s_alarm_service_list = MMILINK_DestroyNode(node_ptr, s_alarm_service_list, DestroySvcInfo);
            // 设置下一个事件
           // SetAlmService();
        }
    }
	
    return result;
}
/*****************************************************************************/
// 	Description : 初始化事件服务链表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_UnRegService(
                                      MMIALMSVC_ID_T svc_id,
                                      BOOLEAN is_set_alarm // 是否更新闹钟服务
                                      )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
//     MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;
    
    // 如果无效，就不用查链表了，节省时间
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        // 查询服务
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            // 删除服务
            s_alarm_service_list = MMILINK_DestroyNode(node_ptr, s_alarm_service_list, DestroySvcInfo);
            // 设置下一个事件
            SetAlmService();
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : start alarm when interrupt of alarm is coming
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_StartService(void)
{
    return StartAlmService();
}

/*****************************************************************************/
// 	Description : 设置闹钟服务
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
PUBLIC void MMIALMSVC_SetAlmService(void)
{
    SetAlmService();
}


