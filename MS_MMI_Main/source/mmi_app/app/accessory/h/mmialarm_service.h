/******************************************************************************
** File Name:      mmialarm_service.h                                         *
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
#ifndef _MMI_ALARM_SERVICE_H_
#define _MMI_ALARM_SERVICE_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_time.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE DEFINITION                                   */
/*---------------------------------------------------------------------------*/
typedef uint32 MMIALMSVC_ID_T;


 /**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                   *
 **--------------------------------------------------------------------------*/

//起闹事件类型枚举,按优先级先后顺序排
typedef enum
{
	ALMSVC_EVENT_SYSTEM,   // alarm, calendar, power on, power off
	ALMSVC_EVENT_TV,       // mobile tv
    ALMSVC_EVENT_OTHER,    // other modules
	ALMSVC_EVENT_MAX
}MMIALMSVC_EVENT_TYPE_E; 

typedef BOOLEAN (*MMIALMSVC_HANDLE_CALLBACK)(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type);//起闹事件callback处理函数

//alarm服务模块事件注册信息
typedef struct
{
    // 记录事件的时间
    SCI_ALARM_T                 event_time;
    // 事件的类型和处理函数
    MMIALMSVC_EVENT_TYPE_E      event_type; // 事件类型
	MMIALMSVC_HANDLE_CALLBACK 	callback;   // 事件处理函数
}MMIALMSVC_EVENT_REG_INFO_T;


/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 判断服务是否有效?
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  
//	Note:当service为0或者svc列表中没有该id对应的svc时，返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_IsValidSvc(
                                    MMIALMSVC_ID_T svc_id
                                    );

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
                                        );

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
                                          );

/*****************************************************************************/
// 	Description : 初始化事件服务链表
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC MMIALMSVC_ID_T MMIALMSVC_RegService(
                                           MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                           BOOLEAN is_valid,  // 闹钟是否有效
                                           BOOLEAN is_set_alarm // 是否开启闹钟
                                           );
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
                                      );
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
                                      );

/*****************************************************************************/
// 	Description : start alarm when interrupt of alarm is coming
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE/FALSE
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_StartService(void);

/*****************************************************************************/
// 	Description : 设置闹钟服务
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
PUBLIC void MMIALMSVC_SetAlmService(void);

#endif //_MMI_ALARM_SERVICE_H_
