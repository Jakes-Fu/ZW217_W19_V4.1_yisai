/****************************************************************************
** File Name:      mmiasp_api.c                                             *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the ANSWER PHONE          *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_asp_trc.h"
#include "mmiasp_cortrol_layer.h"
#include "mmicc_export.h"
//#include "mmicc_image.h"

#ifdef MMI_RECORD_SUPPORT

#include "mmirecord_export.h"
#endif
#include "mmifm_export.h"
#include "mmicl_export.h"
#include "mmiasp_nv.h"
#include "mn_type.h"
#include "mmiasp_export.h"
#include "mmiasp_wintab.h"
#include "mmiudisk_export.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
LOCAL BOOLEAN s_asp_record_time_out_state = FALSE;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init asp mudule
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_InitModule(void)
{
    MMIASP_RegMenuGroup();
    MMIASP_RegNv();
    MMIASP_RegWinIdNameArr();
    MMIASP_RegAppletInfo();
}

/*****************************************************************************/
//  Description : post message to the win recording when plug out sd card
//  Global resource dependence : 
//  Author: koki.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SDCardPlugOut(void)
{
    MMK_PostMsg(MMIASP_RECORDING_WIN_ID, MSG_KEYUP_SDCARD_DETECT, PNULL,0);
}

/*****************************************************************************/
//  Description : create wait timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_CreateWaitTimer(void)
{
    /* if asp was opend, then creat the timer,auto start asp when time out 
       and also asp can start by yourself.
    */
    MMIASP_APPLET_T*    applet_ptr = PNULL;
    uint32   timeout    = MMIASP_GetWaitTime() * MMI_1SECONDS;
    if(MMIAPIASP_IsMachineTurnOn())
    {
        if(MMIASP_GetAnswerCondition())
        {
            applet_ptr = MMIASP_StartApplet(MMIASP_JUST_START_APPLET);
            if (PNULL != applet_ptr)
            {
                if (applet_ptr->wait_timer_id != 0)
                {
                    //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_CreatWaitTimer stop wait_timer_id = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_77_112_2_18_2_2_34_0,(uint8*)"d",applet_ptr->wait_timer_id);
                    MMK_StopTimer(applet_ptr->wait_timer_id);
                }
                
                applet_ptr->wait_timer_id = MMK_CreateTimer(timeout, FALSE);
                //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_CreatWaitTimer wait_timer_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_82_112_2_18_2_2_34_1,(uint8*)"d",applet_ptr->wait_timer_id);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_CreatWaitTimer applet_ptr == PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_86_112_2_18_2_2_34_2,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_CreatWaitTimer not suitable for asp !!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_91_112_2_18_2_2_34_3,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: machine not turn on"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_96_112_2_18_2_2_34_4,(uint8*)"");
    }

}
/*****************************************************************************/
//  Description : stop wait timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopWaitTimer(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );

    if(PNULL != applet_ptr)
    {
        if (applet_ptr->wait_timer_id != 0)
        {
            //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_StopWaitTimer wait_timer_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_112_112_2_18_2_2_34_5,(uint8*)"d",applet_ptr->wait_timer_id);
            MMK_StopTimer(applet_ptr->wait_timer_id);
            applet_ptr->wait_timer_id = 0;
        }
    }
    
}
/*****************************************************************************/
//  Description : �Ƿ��ѵ���¼�ȴ�ʱ��
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsWaitTimeout(DPARAM param)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if(PNULL != applet_ptr)
    {
        if (applet_ptr->wait_timer_id == *(uint8*)param)
        {
            return TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP MMIAPIASP_IsWaitTimeout APPLET has not started"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_136_112_2_18_2_2_34_6,(uint8*)"");
    }
    
    return FALSE;
}
/*****************************************************************************/
//  Description : ��¼���Ƿ��ڹ���״̬
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineWorking(void)
{
    BOOLEAN  is_working = FALSE;
    is_working =  MMIASP_IsMechineWorking();
    //SCI_TRACE_LOW:"MMIASP MMIAPIASP_IsMachineWorking work = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_149_112_2_18_2_2_34_7,(uint8*)"d", is_working);
    return is_working;
}
/*****************************************************************************/
//  Description :  whether asp is turn on
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineTurnOn(void)
{
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    MN_DUAL_SYS_E dual_sys = MMIAPICC_GetCallDualSys();
    
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    return cur_nv.asp_flag[dual_sys];
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterAsp(void)
{
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_ASP, (MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC)MMIAPIASP_EnterIntoAsp);
    return TRUE;
}
/*****************************************************************************/
//  Description : �����¼��
//  Global resource dependence : none
//  Author: jun.hu
//  Note: ��������뱣��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterIntoAsp(void)
{
    //SCI_TRACE_LOW:"MMIASP MMIAPIASP_EnterIntoAsp entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_183_112_2_18_2_2_34_8,(uint8*)"");
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    else
    {
        MMIASP_CloseAspApplet();
        MMIASP_StartApplet(MMIASP_START_DEFAULT);
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : ͨ��������������¼
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_WorkOneTime(void)
{ 
    MMIASP_APPLET_T*    applet_ptr = PNULL;
    
    applet_ptr = MMIASP_StartApplet(MMIASP_JUST_START_APPLET);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
    
    if(!MMIASP_GetAnswerCondition())
    {
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP MMIAPIASP_WorkOneTime not suitable for answer asp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_217_112_2_18_2_2_35_9,(uint8*)"");
    }
    
    return TRUE;
    
}

/*****************************************************************************/
//  Description : open ��¼�Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_OpenRecordingWin(void)
{
     MMIASP_OpenRecordingWin();
}
/*****************************************************************************/
//  Description : CLOSE ��¼�Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_CloseRecordingWin(void)
{
    //SCI_TraceLow:"MMIASP MMIAPIASP_CloseRecordingWin entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_238_112_2_18_2_2_35_10,(uint8*)"");
    MMIASP_CloseRecordingWin();
}
/*****************************************************************************/
//  Description : reset factory setting
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_ResetFactorySetting(void)
{
    MMIASP_NV_SETTING_T  set_nv = {0};
    
    SCI_MEMSET(&set_nv, 0, sizeof(MMIASP_NV_SETTING_T));
    
    MMIASP_ResetSetting(&set_nv);

    MMIAPIIDLE_UpdateAspState();
}
/*****************************************************************************/
//  Description : ѡ���¼��
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_SelectAnswerRing(PRINGSELECT_CALLBACKFUNC callback_fun_ptr)
{
    MMIASP_OpenRingSelectWin(callback_fun_ptr);
    
    return TRUE;
}
/*****************************************************************************/
//  Description :��ȡ�̶���������
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIASP_GetFixedRingNameByID(uint8 id)
{
    MMI_TEXT_ID_T ret_id = TXT_NULL;

    ret_id = MMIASP_GetFixedAnswerRingTextId(id);

    return ret_id;
}
/*****************************************************************************/
//  Description :��¼��ʾ�ﲥ����Ϻ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_HandlePlayAudioEnd(void)
{
    MMIASP_HandlePlayAudioEnd();
}
/*****************************************************************************/
//  Description :  stop call answer when call finished
//  Global resource dependence :none
//  Author:
//  Note: �ú��������ڴ�¼ʱ�û������˵绰ʱ���ã���MMIAPIASP_RealseCall���ǹҶ������绰ʱ����...
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopCallAnswer(void)
{
    //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_StopCallAnswer enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_291_112_2_18_2_2_35_11,(uint8*)"");
    
    MMIASP_StopAnswerCall();
}
/*****************************************************************************/
//  Description :  �˺���ר��RealseCallʱ����
//  Global resource dependence :none
//  Author:jun.hu
//  Note: ֻ�ڹҶ������绰ʱ����
/*****************************************************************************/
PUBLIC void MMIAPIASP_RealseCall(void)
{
    //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_RealseCall enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_301_112_2_18_2_2_35_12,(uint8*)"");
    
    MMIASP_RealseCall();
}
/*****************************************************************************/
//  Description :  �绰�Ƿ����˽���
//  Global resource dependence :none
//  Author:jun.hu
//  Note: �˺�����cc�Ҷϵ绰ʱ�ж��û��Ƿ�����绰����
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsCallSpeakedByOwner(void)
{
    BOOLEAN   is_accepted = TRUE;
    MMIASP_MACHINE_CALL_STATE_E call_state = MMIASP_MACHINE_CALL_NONE;
    call_state = MMIASP_GetCallState();
    //SCI_TRACE_LOW:"MMIASP MMIAPIASP_lsCallSpeakedByOwner call state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_314_112_2_18_2_2_35_13,(uint8*)"d", call_state);
    
    if((MMIASP_MACHINE_CALL_SPEAKING != call_state)&&(MMIASP_MACHINE_CALL_NONE != call_state))
    {
        is_accepted = FALSE;
    }
   
    return is_accepted;
}
/*****************************************************************************/
//  Description : ��ǰ�Ƿ��ڴ�¼�绰�����ڽ���״̬
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsAspCallSpeaking(void)
{
    MMIASP_MACHINE_CALL_STATE_E call_state = MMIASP_MACHINE_CALL_NONE;
    call_state = MMIASP_GetCallState();
    //SCI_TRACE_LOW:"MMIASP MMIAPIASP_lsAspCallSpeaking call state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_331_112_2_18_2_2_35_14,(uint8*)"d", call_state);
    
    if(MMIASP_MACHINE_CALL_SPEAKING == call_state)
    {
        return TRUE;
    }

    return  FALSE;
}
/*****************************************************************************/
//  Description :��¼����״̬����Ҫ��ʾ��ͼ��
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  MMI_IMAGE_ID_T MMIAPIASP_GetIdleImageID(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    uint32 i = MN_DUAL_SYS_1;
    // δ��ͼ��....
    if(0 < MMIASP_GetNewRecordNum())
    {
        return IMAGE_IDLE_TITLE_ICON_ASP_UNREAD;
    }

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(cur_nv.asp_flag[i])  
        {
            image_id = IMAGE_IDLE_TITLE_ICON_ASP;
            break;
        }
    }
    return image_id;
}
/*****************************************************************************/
//  Description :  �Ƿ����ڲ���¼����
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsPlayingMusic(void)
{
    return (MMIASP_STATE_PLAY == MMIASP_GetPlayStatus());
}
/*****************************************************************************/
//  Description : open ���Ž���-��˽�����Ļص�����
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   �˺���ר�����ⲿֱ�ӽ��뵽���Ž���(���Ǵ������б�������ģ�
/*****************************************************************************/
LOCAL void ASP_ValidatePrivacyAspOpenPlay(BOOLEAN is_permitted, DPARAM param_ptr)
{
    uint16 node_index = 0;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            node_index = *(uint16*)param_ptr;
        }
        MMIAPIASP_UnLockOpenPlayWinFromExternal(node_index);
    }
}

/*****************************************************************************/
//  Description : open ���Ž���Ľ���ǰ�ж��Ƿ������˽����
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   �˺���ר�����ⲿֱ�ӽ��뵽���Ž���(���Ǵ������б�������ģ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_OpenPlayWinFromExternal(uint16 node_index)
{
    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_ASP, ASP_ValidatePrivacyAspOpenPlay, &node_index, sizeof(node_index));
    return TRUE;
}
/*****************************************************************************/
//  Description : open ���Ž���Ľ���
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   �˺���ר�����ⲿֱ�ӽ��뵽���Ž���(���Ǵ������б�������ģ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_UnLockOpenPlayWinFromExternal(uint16 node_index)
{
    MMIASP_StartApplet(MMIASP_JUST_START_APPLET);
    return MMIASP_OpenPlayWinFromExternal(node_index);
}

/*****************************************************************************/
//  Description : ��ȡ�µ�������Ŀ��..
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC uint32 MMIAPIASP_GetNewRecordNum(void)
{
    return MMIASP_GetNewRecordNum();
}
/*****************************************************************************/
//  Description : ��ȡ���µ�������
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetLastestUnreadRecord(
                                                MMIASP_MSG_INFO_T* msg_info_ptr, // out
                                                uint16*            node_index_ptr// out
                                                )
{
   return MMIASP_GetLastestUnreadRecord(msg_info_ptr, node_index_ptr);
}
/*****************************************************************************/
//  Description : �Զ����Ŵ�¼�������Լ���Ƿ���������˽����.....
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_AutoPlayRecordMessages(void)
{
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_ASP, MMIAPIASP_UnLockAutoPlayRecordMessages);
}
/*****************************************************************************/
//  Description : �Զ����Ŵ�¼����
//  Global resource dependence : none
//  Author: kun.liu
//  Note: ��������뱣��
/*****************************************************************************/
PUBLIC void MMIAPIASP_UnLockAutoPlayRecordMessages(void)
{
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        MMIASP_CloseAspApplet();
        MMIASP_StartApplet(MMIASP_AUTO_PLAY_MESSEAGES);
    }
}

/*****************************************************************************/
//  Description : ��ʼ��¼
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_StartAnswerCall(void)
{
    MMIAPIFM_StopRecord();
    MMIASP_StartAnswerCall();
}

/*****************************************************************************/
//  Description : ���ô�¼��Ϊ¼�ƴ�¼״̬
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetRecordRingState(BOOLEAN  is_record_ring)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    if(PNULL != applet_ptr)
    {
        applet_ptr->is_record_ring = is_record_ring;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP applet is not set answer ring as recording ring "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_478_112_2_18_2_2_35_15,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description :��ȡ ���ô�¼���Ƿ�Ϊ¼�ƴ�¼״̬
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIASP_GetRecordRingState()
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    if(PNULL != applet_ptr)
    {
        return applet_ptr->is_record_ring;
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : ����ͨ����¼ʱ�䵽��״̬
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetAspRecordTimeOutState(BOOLEAN  asp_record_state)
{
    s_asp_record_time_out_state = asp_record_state;
}
/*****************************************************************************/
//  Description : ���ͨ����¼ʱ�䵽��״̬
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetAspRecordTimeOutState(void)
{
    return s_asp_record_time_out_state;
}
/*****************************************************************************/
//  Description :  ���ڶ����绰���ֲ�Ӧ��ʱ�ص���¼
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopAspRecording(void)
{
    //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_StopAspRecording enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_API_517_112_2_18_2_2_35_16,(uint8*)"");
    
    MMIASP_CloseRecordingWin();
    MMIASP_CloseReleasingWin();
    MMIASP_StopAnswerCall();
    MMIASP_SetCallState(MMIASP_MACHINE_CALL_NONE);
}

