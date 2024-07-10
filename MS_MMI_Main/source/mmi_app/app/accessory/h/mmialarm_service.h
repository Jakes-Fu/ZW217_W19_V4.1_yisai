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

//�����¼�����ö��,�����ȼ��Ⱥ�˳����
typedef enum
{
	ALMSVC_EVENT_SYSTEM,   // alarm, calendar, power on, power off
	ALMSVC_EVENT_TV,       // mobile tv
    ALMSVC_EVENT_OTHER,    // other modules
	ALMSVC_EVENT_MAX
}MMIALMSVC_EVENT_TYPE_E; 

typedef BOOLEAN (*MMIALMSVC_HANDLE_CALLBACK)(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type);//�����¼�callback������

//alarm����ģ���¼�ע����Ϣ
typedef struct
{
    // ��¼�¼���ʱ��
    SCI_ALARM_T                 event_time;
    // �¼������ͺʹ�����
    MMIALMSVC_EVENT_TYPE_E      event_type; // �¼�����
	MMIALMSVC_HANDLE_CALLBACK 	callback;   // �¼�������
}MMIALMSVC_EVENT_REG_INFO_T;


/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : �жϷ����Ƿ���Ч?
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  
//	Note:��serviceΪ0����svc�б���û�и�id��Ӧ��svcʱ������FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIALMSVC_IsValidSvc(
                                    MMIALMSVC_ID_T svc_id
                                    );

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
                                        );

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
                                          );

/*****************************************************************************/
// 	Description : ��ʼ���¼���������
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:  TRUE: init success!
//	Note:    
/*****************************************************************************/
PUBLIC MMIALMSVC_ID_T MMIALMSVC_RegService(
                                           MMIALMSVC_EVENT_REG_INFO_T *event_info_ptr,
                                           BOOLEAN is_valid,  // �����Ƿ���Ч
                                           BOOLEAN is_set_alarm // �Ƿ�������
                                           );
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
                                      );
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
// 	Description : �������ӷ���
//	Global resource dependence : 
//  Author: xiaoqing.lu
//  RETRUN:
//	Note:    
/*****************************************************************************/
PUBLIC void MMIALMSVC_SetAlmService(void);

#endif //_MMI_ALARM_SERVICE_H_
