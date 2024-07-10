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
#define MMIALMSVC_INVLID_SVC_ID  0x0        // ��Ч�ķ���ID
#define MMIALMSVC_MAX_SVC_ID     0xFFFFFFFF // ����ID���ֵ
#define MMIALMSVC_INVLID_YEAR    0xFFFF     // ��Ч����ݣ���ʼ��ʹ��

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/ 
LOCAL MMI_LINK_NODE_T *s_alarm_service_list = PNULL; // ��¼���еķ����¼�

LOCAL uint32           s_svcid_count = 0;
LOCAL BOOLEAN          s_is_svcid_cyc = FALSE; // �ж�service id�Ƿ��Ѿ�������һ��ѭ�����Ѿ��ﵽ��һ��0xFFFFFFFF

//alarm����ģ���¼�ע����Ϣ
typedef struct
{
    BOOLEAN					   	is_on;      // �Ƿ���
    BOOLEAN					   	is_valid;   // �Ƿ���Ч����
    MMIALMSVC_ID_T              svc_id;     // �����¼�ID
    
    // ��ǰʱ��
    SCI_ALARM_T                 event_time;
    MMIALMSVC_EVENT_TYPE_E      event_type; // �¼�����
	MMIALMSVC_HANDLE_CALLBACK 	callback;   // �¼�������
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
// 	Description : �����¼�����ID
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
// 	Description : ����һ��������Ϣ���ָ��
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMIALMSVC_EVENT_INFO_T *CreateSvcInfo(
                                            MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                            BOOLEAN is_valid // �Ƿ�������
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
// 	Description : �������ӷ���
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
LOCAL void SetAlmService(void);

/*****************************************************************************/
// 	Description : ����ʱ�������б�
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: ���������ͷָ��
//	Note:    
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *GenerateHappendSvcList(
                                               MMI_LINK_NODE_T *head_node_ptr,
                                               SCI_ALARM_T event_time
                                               );

/*****************************************************************************/
// 	Description : ɾ���б�
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: ���������ͷָ��
//	Note:    
/*****************************************************************************/
LOCAL void DeleteSvcList(
                         MMI_LINK_NODE_T *head_node_ptr
                         );

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : �����¼�����ID
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GenerateEventSvcID(void)
{
    if (s_svcid_count == MMIALMSVC_MAX_SVC_ID || s_is_svcid_cyc)
    {
        s_svcid_count++;
        if (s_svcid_count == 0) // ȷ��com id����Ϊ0
        {
            s_svcid_count = 1;
        }
        // ����δʹ�õ�ID
        while (SearchSvcInListById(s_alarm_service_list, s_svcid_count))
        {
            s_svcid_count++;
        } 
    }
    else
    {
        s_svcid_count++;
        if(s_svcid_count == MMIALMSVC_MAX_SVC_ID) // �ﵽ��һ��ѭ��������״̬
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
// 	Description : ����һ��������Ϣ���ָ��
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMIALMSVC_EVENT_INFO_T *CreateSvcInfo(
                                            MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                            BOOLEAN is_valid // �Ƿ�������
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
            // ����ID���ң��ҵ�֮�󷵻�ָ��
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

    // �������û���ҵ������ʼ��PNULL
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
//             // ����ID���ң��ҵ�֮�󷵻�ָ��
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
//     // �������û���ҵ������ʼ��PNULL
//     if (!is_find)
//     {
//         tmp_ptr = PNULL;
//     }
// 
//     return tmp_ptr;    
// }

/*****************************************************************************/
// 	Description : ����ʱ�������б�
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: ���������ͷָ��
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
        // �õ��ڵ�����
        new_event_info_ptr = ((MMIALMSVC_EVENT_INFO_T *)tmp_head_node_ptr->data);
        // ��������
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
        // ָ����һ���ڵ�
        tmp_head_node_ptr = tmp_head_node_ptr->next_ptr;
        // �ж��Ƿ��ͷ�ڵ���ͬ
        if (head_node_ptr == tmp_head_node_ptr)
        {
            break;
        }
    }

    return result_node_ptr;
}

/*****************************************************************************/
// 	Description : ����ʱ�������б�
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN: ���������ͷָ��
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
    MMI_LINK_NODE_T *happen_svc_list_ptr = PNULL; // ��¼���е�ͬʱ�����ķ����¼�

    // ��ȡ��ǰϵͳʱ��
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_time.hour = time.hour;
    cur_time.min = time.min;
    cur_time.year = date.year;
    cur_time.mon = date.mon;
    cur_time.mday = date.mday;

    //SCI_TRACE_LOW:"StartAlmService:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_SERVICE_462_112_2_18_2_1_21_63,(uint8*)"ddddd",cur_time.year,cur_time.mon,cur_time.mday,cur_time.hour,cur_time.min);

    // ����ͬһʱ�̵�����
    happen_svc_list_ptr = GenerateHappendSvcList(s_alarm_service_list, cur_time);
    // �������ȼ�ִ���¼�.
    // 1��ִ��ϵͳALMSVC_EVENT_SYSTEM
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
            is_deal = TRUE; // ϵͳִֻ��һ��(���ӡ��ճ̡����ػ�)
            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
        }

        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }
    // 2��ִ��ALMSVC_EVENT_TV
    link_node_ptr = happen_svc_list_ptr;
    while (PNULL != link_node_ptr 
        && PNULL != ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data))
    {
        if (TRUE == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid
            && ALMSVC_EVENT_TV == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type)
        {
            // ��������֮������Ϊ��Ч
            ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid = FALSE;

            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
       }
        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }
    // 2��ִ��ALMSVC_EVENT_OTHER
    link_node_ptr = happen_svc_list_ptr;
    while (PNULL != link_node_ptr 
        && PNULL != ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data))
    {
        if (TRUE == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid
            && ALMSVC_EVENT_OTHER == ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type)
        {
            // ��������֮������Ϊ��Ч
            ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->is_valid = FALSE;

            is_success |= ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->callback(((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->svc_id, ((MMIALMSVC_EVENT_INFO_T*)link_node_ptr->data)->event_type);
        }

        link_node_ptr = link_node_ptr->next_ptr;
        if (link_node_ptr == happen_svc_list_ptr)
        {
            break;
        }
    }

    // ִ����֮��ɾ������
    if (PNULL != happen_svc_list_ptr)
    {
        DeleteSvcList(happen_svc_list_ptr);
    }
    
    // ������һ������
    SetAlmService();
    
    return is_success;
}

/*****************************************************************************/
// 	Description : �������ӷ���
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
        // ��ȡ��ǰϵͳʱ��
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
            // ����ID���ң��ҵ�֮�󷵻�ָ��
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
        // ��������
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
// 	Description : �жϷ����Ƿ���Ч?
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  
//	Note:��serviceΪ0����svc�б���û�и�id��Ӧ��svcʱ������FALSE
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
// 	Description : ���·�������
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

    // �����Ч���Ͳ��ò������ˣ���ʡʱ��
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
                // ������Ч����Ч֮����Ҫ������������
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
// 	Description : ���÷����Ƿ���Ч
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE����������ǰ��״̬
//	Note: ��������֮���Զ�����Ϊ��Ч״̬
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_SetServciceValid(
                                          MMIALMSVC_ID_T svc_id,
                                          BOOLEAN is_valid,
                                          BOOLEAN is_set_alarm // �Ƿ�������
                                          )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;

    // �����Ч���Ͳ��ò������ˣ���ʡʱ��
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            tmp_event_ptr =  ((MMIALMSVC_EVENT_INFO_T*)node_ptr->data);
            if (PNULL != tmp_event_ptr)
            {
                result = tmp_event_ptr->is_valid;
                // �ж��Ƿ���Ҫ�����������ӣ����״̬��ͬ������Ҫ��������
                if (tmp_event_ptr->is_valid != is_valid)
                {
                    if (is_set_alarm) // ���״̬��ͬ���������������ж��Ƿ�Ҫ�������ӡ�
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
// 	Description : ��ʼ���¼���������
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC MMIALMSVC_ID_T MMIALMSVC_RegService(
                                           MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                           BOOLEAN is_valid, // ע��ķ����Ƿ���Ч
                                           BOOLEAN is_set_alarm // �Ƿ�������
                                           )
{
    MMIALMSVC_ID_T svc_id = MMIALMSVC_INVLID_SVC_ID;
    MMIALMSVC_EVENT_INFO_T *new_event_info_ptr = PNULL;
    
    // ����һ��������Ϣ�ڵ�
    new_event_info_ptr = CreateSvcInfo(event_info_ptr, is_valid);
    // �������
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
        // ��������
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
// 	Description : �ͷ�alm
//	Global resource dependence : 
//  Author: liyan.zhu
//  RETRUN:  TRUE: init success!
//	Note:    CR:136024 COPY MMIALMSVC_UnRegService
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_FreeAlmRegService(
                                      MMIALMSVC_ID_T svc_id,
                                      BOOLEAN is_set_alarm // �Ƿ�������ӷ���
                                      )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
	//     MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;
    
    // �����Ч���Ͳ��ò������ˣ���ʡʱ��
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        // ��ѯ����
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            // ɾ������
            s_alarm_service_list = MMILINK_DestroyNode(node_ptr, s_alarm_service_list, DestroySvcInfo);
            // ������һ���¼�
           // SetAlmService();
        }
    }
	
    return result;
}
/*****************************************************************************/
// 	Description : ��ʼ���¼���������
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_UnRegService(
                                      MMIALMSVC_ID_T svc_id,
                                      BOOLEAN is_set_alarm // �Ƿ�������ӷ���
                                      )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
//     MMIALMSVC_EVENT_INFO_T *tmp_event_ptr = PNULL;
    
    // �����Ч���Ͳ��ò������ˣ���ʡʱ��
    if (MMIALMSVC_IsValidSvc(svc_id))
    {
        // ��ѯ����
        node_ptr = SearchSvcInListById(s_alarm_service_list, svc_id);
        if (PNULL != node_ptr)
        {
            // ɾ������
            s_alarm_service_list = MMILINK_DestroyNode(node_ptr, s_alarm_service_list, DestroySvcInfo);
            // ������һ���¼�
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
// 	Description : �������ӷ���
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
PUBLIC void MMIALMSVC_SetAlmService(void)
{
    SetAlmService();
}


