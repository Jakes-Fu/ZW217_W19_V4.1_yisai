/****************************************************************************
** File Name:      mmiasp_export.h                                          *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe fuction of ANSWER PHONE    *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/
#ifndef _MMIASP_API_H
#define _MMIASP_API_H
#include "mmiasp_cortrol_layer.h"


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIASP_SetRecordRingState   MMIAPIASP_SetRecordRingState
#define MMIASP_GetRecordRingState   MMIAPIASP_GetRecordRingState

#endif //MMI_FUNC_COMPATIBLE_SUPPORT


/*****************************************************************************/
//  Description :  init asp mudule
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_InitModule(void);

/*****************************************************************************/
//  Description : create wait timer
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_CreateWaitTimer(void);

/*****************************************************************************/
//  Description : stop wati timer
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopWaitTimer(void);

/*****************************************************************************/
//  Description :  stop call answer when call finished
//  Global resource dependence :none
//  Author:
//  Note: �ú��������ڴ�¼ʱ�û������˵绰ʱ���ã���MMIAPIASP_RealseCall���ǹҶ������绰ʱ����
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopCallAnswer(void);

/*****************************************************************************/
//  Description : �����¼��
//  Global resource dependence : none
//  Author: jun.hu
//  Note: ������뱣��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterAsp(void);

/*****************************************************************************/
//  Description : whether asp is turn on
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineTurnOn(void);

/*****************************************************************************/
//  Description : ��¼���Ƿ��ڹ���״̬
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineWorking(void);

/*****************************************************************************/
//  Description : reset factory setting
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : CLOSE ��¼�Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_CloseRecordingWin(void);

/*****************************************************************************/
//  Description : open ��¼�Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_OpenRecordingWin(void);

/*****************************************************************************/
//  Description : ��ȡ�̶���������
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIASP_GetFixedRingNameByID(uint8 id);

/*****************************************************************************/
//  Description : ��¼��ʾ�ﲥ����Ϻ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_HandlePlayAudioEnd(void);

/*****************************************************************************/
//  Description :  �˺���ר��RealseCallʱ����
//  Global resource dependence :none
//  Author:jun.hu
//  Note: ֻ�ڹҶ������绰ʱ����
/*****************************************************************************/
PUBLIC void MMIAPIASP_RealseCall(void);

/*****************************************************************************/
//  Description :  �绰�Ƿ����˽���
//  Global resource dependence :none
//  Author:jun.hu
//  Note: �˺�����cc�Ҷϵ绰ʱ�ж��û��Ƿ�����绰����
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsCallSpeakedByOwner(void);

/*****************************************************************************/
//  Description : ѡ���¼��
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_SelectAnswerRing(PRINGSELECT_CALLBACKFUNC callback_fun_ptr);

/*****************************************************************************/
//  Description : ��¼����״̬����Ҫ��ʾ��ͼ��
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  MMI_IMAGE_ID_T MMIAPIASP_GetIdleImageID(void);

/*****************************************************************************/
//  Description : �Ƿ����ڲ���¼����
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsPlayingMusic(void);

/*****************************************************************************/
//  Description : ��ǰ�Ƿ��ڴ�¼�绰�����ڽ���״̬
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsAspCallSpeaking(void);

/*****************************************************************************/
//  Description : open ���Ž���Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   �˺���ר�����ⲿֱ�ӽ��뵽���Ž���(���Ǵ������б�������ģ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_OpenPlayWinFromExternal(uint16 node_index);

/*****************************************************************************/
//  Description : ��ȡ�µ�������Ŀ��..
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC uint32 MMIAPIASP_GetNewRecordNum(void);

/*****************************************************************************/
//  Description : ��ȡ���µ�������
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetLastestUnreadRecord(
                                                MMIASP_MSG_INFO_T* msg_info_ptr, // out
                                                uint16*            node_index_ptr// out
                                                );
/*****************************************************************************/
//  Description : �Զ����Ŵ�¼������
//  Global resource dependence : none
//  Author: jun.hu
//  Note: ������뱣��
/*****************************************************************************/
PUBLIC void MMIAPIASP_AutoPlayRecordMessages(void);

/*****************************************************************************/
//  Description : post message to the win recording when plug out sd card
//  Global resource dependence : 
//  Author: koki.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SDCardPlugOut(void);

/*****************************************************************************/
//  Description : ���ô�¼��Ϊ¼�ƴ�¼״̬
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetRecordRingState(BOOLEAN  is_record_ring);

/*****************************************************************************/
//  Description :��ȡ ���ô�¼���Ƿ�Ϊ¼�ƴ�¼״̬
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIASP_GetRecordRingState(void);

/*****************************************************************************/
//  Description : ͨ��������������¼
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_WorkOneTime(void);

/*****************************************************************************/
//  Description : �Ƿ��ѵ���¼�ȴ�ʱ��
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsWaitTimeout(DPARAM param);

/*****************************************************************************/
//  Description : ��ʼ��¼
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_StartAnswerCall(void);

/*****************************************************************************/
//  Description : �����¼��
//  Global resource dependence : none
//  Author: kun.liu
//  Note: ��������뱣��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterIntoAsp(void);

/*****************************************************************************/
//  Description : �Զ����Ŵ�¼����
//  Global resource dependence : none
//  Author: kun.liu
//  Note: ��������뱣��
/*****************************************************************************/
PUBLIC void MMIAPIASP_UnLockAutoPlayRecordMessages(void);

/*****************************************************************************/
//  Description : open ���Ž���Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   �˺���ר�����ⲿֱ�ӽ��뵽���Ž���(���Ǵ������б�������ģ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_UnLockOpenPlayWinFromExternal(uint16 node_index);

/*****************************************************************************/
//  Description : ����ͨ����¼ʱ�䵽��״̬
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetAspRecordTimeOutState(BOOLEAN  asp_record_state);

/*****************************************************************************/
//  Description : ���ͨ����¼ʱ�䵽��״̬
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetAspRecordTimeOutState(void);

/*****************************************************************************/
//  Description :  ���ڶ����绰���ֲ�Ӧ��ʱ�ص���¼
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopAspRecording(void);

/*****************************************************************************/
//   Description : ͨ�������ȡ����
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetNameByNumber(
                                      uint8*          call_num_ptr,//in
                                      MMI_STRING_T*   name_ptr //out
                                      );

#endif

