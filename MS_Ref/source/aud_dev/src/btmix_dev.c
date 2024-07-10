/******************************************************************************
 ** File Name:      bt_mixer.c                                                                *
 ** Author:         Hui.Zhang                                             *
 ** DATE:           7/04/2011                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    BT recording.* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/04/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "os_api.h"
#include "dal_audio.h"
#include "aud_gen.h"
#include "adm.h"
#include "adm_dp.h"
#include "tasks_id.h"
#include "chip.h"
#include "sci_api.h"

#include "threadx_os.h"

#include "btmix_dev.h"

#define BT_TASK_MODE

// enable track will slow down in L7A
#ifdef SCI_TRACE_LOW
#undef SCI_TRACE_LOW
#endif
#define SCI_TRACE_LOW(...)

#define VOICE_DATA_SIZE 160
#define BLUE_LINK_NUM   20
#define BLUE_LINK_BUF_SIZE   (BLUE_LINK_NUM*VOICE_DATA_SIZE)
#define BLUE_DATA_CHECK_NUM     5

typedef enum
{
    BLUE_UPLINK_ID = 0,
    BLUE_DOWNLINK_ID,
    BLUE_LINK_MAX
}BLUE_LINK_E;

typedef enum
{
    BLUE_OPT_MIX,
    BLUE_OPT_IGNORE
}BLUE_OPT_E;

typedef enum
{
    BLUE_SIG_MIX_DATA,
    BLUE_SIG_MAX
}BLUE_SIG_E;

typedef enum
{
    BLUE_FLAG_STOP,
    BLUE_FLAG_PLAY,
    BLUE_FLAG_PAUSE
}BLUE_FLAG_E;

struct _tag_BLUE_LINK_DATA
{
    uint32 data_in;
    uint32 data_out;
    //volatile uint32 data_count;
    volatile uint32 remain_count;
    uint16 data_buf[BLUE_LINK_BUF_SIZE];
};

typedef BLUE_LINK_DATA_T    BLUE_MIX_DATA_T;
typedef BLUE_LINK_DATA_PTR  BLUE_MIX_DATA_PTR;
#define BLUE_GetMixDataLen  BLUE_GetLinkDataLen
#define BLUE_MIX_BUF_SIZE   BLUE_LINK_BUF_SIZE

#define BLUE_MIX_DATA_MIN   VOICE_DATA_SIZE
#define BLUE_MIX_DATA_MAX   VOICE_DATA_SIZE

#define BLUEMIX_STACK_SIZE  0x800
#define BLUEMIX_QUEUE_NUM   (BLUE_LINK_NUM*2)

typedef struct
{
    BLUE_LINK_DATA_T    data[BLUE_LINK_MAX];
    BLUE_MIX_DATA_T     record;
    ADM_RECCALLBACK     rec_cbf;
}BLUE_MIX_DM_T, *BLUE_MIX_DM_PTR;

typedef struct
{
    uint8  flag;
    uint32 task_id;
}BLUE_MIX_CTRL_T, *BLUE_MIX_CTRL_PTR;

typedef struct
{
    SIGNAL_VARS
}BLUE_MIX_MSG_T;


LOCAL BLUE_MIX_DM_PTR   s_blue_dm_ptr = NULL;
LOCAL BLUE_MIX_CTRL_T   s_blue_ctrl = {0};


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    initialize mix info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_InitMixInfo(void);

/*****************************************************************************/
//  Description:    free mix data info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_FreeMixInfo(void);

/*****************************************************************************/
//  Description:    get mix data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BLUE_MIX_DM_PTR BLUE_GetMixDataManage(void);

/*****************************************************************************/
//  Description:    get mix data ctrl info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BLUE_MIX_CTRL_PTR BLUE_GetMixCtrlInfo(void);

/*****************************************************************************/
//  Description:    put link data to data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PutLinkData(BLUE_LINK_DATA_PTR dst_ptr, uint16 *src_ptr, uint16 src_len);

/*****************************************************************************/
//  Description:    get link data length
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 BLUE_GetLinkDataLen(BLUE_LINK_DATA_PTR data_ptr);

/*****************************************************************************/
//  Description:    get mix data from data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BLUE_GetMixData(uint16 *dst_ptr, BLUE_MIX_DATA_PTR src_ptr, uint16 dst_size);

#ifdef BT_TASK_MODE
/*****************************************************************************/
//  Description:    open mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskOpen(uint32 priority);

/*****************************************************************************/
//  Description:    close mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskClose(void);

/*****************************************************************************/
//  Description:    mix task entry function
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskEntry(uint32 argc, void *argv);

/*****************************************************************************/
//  Description:    send signal to mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_SendSignal(uint32 signalCode);

/*****************************************************************************/
//  Description:    get mix task queue avilable
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 BLUE_GetMixTask_Queue_Avilable(void);
#endif

/*****************************************************************************/
//  Description:    execute mix data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_ExecMixData(void);

/*****************************************************************************/
//  Description:    mix data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 BLUE_MixData(BLUE_MIX_DATA_PTR mix_ptr, BLUE_LINK_DATA_PTR uplink_ptr, BLUE_LINK_DATA_PTR downlink_ptr);

/*****************************************************************************/
//  Description:    get link data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
//LOCAL BOOLEAN BLUE_GetLinkData(BLUE_LINK_DATA_PTR src_ptr, uint16 *dst_ptr, uint16 dst_len);
LOCAL uint16 *BLUE_GetLinkDataOutPtr(BLUE_LINK_DATA_PTR src_ptr, uint16 dst_len);
LOCAL void BLUE_IncLinkDataOutPtr(BLUE_LINK_DATA_PTR src_ptr, uint16 dst_len);

/*****************************************************************************/
//  Description:    put data to data manager after mixed
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
//LOCAL BOOLEAN BLUE_PutMixData(BLUE_MIX_DATA_PTR dst_ptr, uint16 *src_ptr, uint16 src_len);
LOCAL uint16 *BLUE_GetMixDataInPtr(BLUE_MIX_DATA_PTR dst_ptr, uint16 src_len);
LOCAL void BLUE_IncMixDataInPtr(BLUE_MIX_DATA_PTR dst_ptr, uint16 src_len);

/*****************************************************************************/
//  Description:    play audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    stop audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Stop(DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    pause audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Pause(DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    resume audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Resume(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    register audio device callback function to get data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 BLUEMIX_RegMixCallback(ADM_RECCALLBACK pCallback);

/*****************************************************************************/
//  Description:    unregister audio device callback function
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUEMIX_UnRegMixCallback(uint32 uiRegVector);

/*****************************************************************************/
//  Description:    read mix data to buffer from data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUEMIX_ReadData(uint16* buf_ptr, uint32 buf_size, uint32 *p_real_size);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    put link data to data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void BLUE_PutLinkData(uint16 *src_ptr, uint16 src_len, BOOLEAN is_uplink)
{
    BLUE_MIX_CTRL_PTR   ctrl_ptr = BLUE_GetMixCtrlInfo();
    BLUE_MIX_DM_PTR     dm_ptr = BLUE_GetMixDataManage();
    uint8 id = 0;

    if (NULL == src_ptr || 0 == src_len || NULL == ctrl_ptr || NULL == dm_ptr)
    {
        return;
    }

    if (BLUE_FLAG_PLAY != ctrl_ptr->flag || NULL == dm_ptr->rec_cbf)
    {
        return;
    }

    SCI_TRACE_LOW("BLUE_PutLinkData: S=0x%08x, L=%d, isup=%d, C=0x%08x, D=0x%08x, flag=%d, rec_cbf=0x%08x",
        src_ptr, src_len, is_uplink, ctrl_ptr, dm_ptr, ctrl_ptr->flag, dm_ptr->rec_cbf);
    id = is_uplink ? BLUE_UPLINK_ID : BLUE_DOWNLINK_ID;
    if (PutLinkData(&dm_ptr->data[id], src_ptr, src_len))
    {
#ifdef BT_TASK_MODE
        if (BLUE_GetMixTask_Queue_Avilable() > 0)
        {
            BLUE_SendSignal(BLUE_SIG_MIX_DATA);
        }
#else
        BLUE_ExecMixData();
#endif
    }
}

/*****************************************************************************/
//  Description:    get mix data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BLUE_MIX_DM_PTR BLUE_GetMixDataManage(void)
{
    return s_blue_dm_ptr;
}

/*****************************************************************************/
//  Description:    get mix data ctrl info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BLUE_MIX_CTRL_PTR BLUE_GetMixCtrlInfo(void)
{
    return &s_blue_ctrl;
}

/*****************************************************************************/
//  Description:    put link data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PutLinkData(BLUE_LINK_DATA_PTR dst_ptr, uint16 *src_ptr, uint16 src_len)
{
    if (NULL == src_ptr || NULL == dst_ptr)
    {
        return FALSE;
    }

    if(src_len > dst_ptr->remain_count)
    {
        return FALSE;
    }

    if(dst_ptr->data_in + src_len > BLUE_LINK_BUF_SIZE)
    {
        uint16 copy_len = BLUE_LINK_BUF_SIZE - dst_ptr->data_in;

        SCI_MEM16CPY(&dst_ptr->data_buf[dst_ptr->data_in], src_ptr, copy_len*sizeof(uint16));
        SCI_MEM16CPY(&dst_ptr->data_buf[0], src_ptr+copy_len, (src_len - copy_len)*sizeof(uint16));
    }
    else
    {
        SCI_MEM16CPY(&dst_ptr->data_buf[dst_ptr->data_in], src_ptr, src_len*sizeof(uint16));
    }

    dst_ptr->data_in = (dst_ptr->data_in + src_len) % BLUE_LINK_BUF_SIZE;
    SCI_DisableIRQ();
    dst_ptr->remain_count -= src_len;
    SCI_RestoreIRQ();

    return TRUE;
}

/*****************************************************************************/
//  Description:    register mix audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC HAUDIODEV BTMIX_RegAudioDevice(void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY((uint16*)tDeviceInfo.usDeviceName, ANSI2UINT16("BLUEMIX"), strlen("BLUEMIX") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 160;
    tDeviceInfo.tAudioDevOpe.pOpen = BLUEMIX_Play;
    tDeviceInfo.tAudioDevOpe.pClose = BLUEMIX_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = BLUEMIX_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = BLUEMIX_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = NULL;
    tDeviceInfo.tAudioDevOpe.pStartRecord = NULL;
    tDeviceInfo.tAudioDevOpe.pStopRecord = NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = BLUEMIX_RegMixCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = BLUEMIX_UnRegMixCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = BLUEMIX_ReadData;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice(&tDeviceInfo);
   return hDevice;
}

/*****************************************************************************/
//  Description:    play audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    SCI_TRACE_LOW("BLUEMIX_Play: %d %d %d %d %d %d %d %d",
        uiSampleRate,
        ptDevSetting->eDev_mode,
        ptDevSetting->uiVol,
        ptDevSetting->bDownlinkLocal_mute,
        ptDevSetting->bDownlinkNet_mute,
        ptDevSetting->bDownlink_mute,
        ptDevSetting->bUplink_mute,
        ptDevSetting->uiDigital_gain);
    BLUE_InitMixInfo();
    if (NULL != ctrl_ptr && BLUE_FLAG_STOP == ctrl_ptr->flag)
    {
#ifdef BT_TASK_MODE
        BLUE_TaskOpen(73);
#endif
        ctrl_ptr->flag = BLUE_FLAG_PLAY;
        return AUDIO_NO_ERROR;
    }
    else
    {
        return AUDIO_ERROR;
    }
}

/*****************************************************************************/
//  Description:    initialize mix info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_InitMixInfo(void)
{
    if (NULL == s_blue_dm_ptr)
    {
        s_blue_dm_ptr = SCI_ALLOC_APP(sizeof(BLUE_MIX_DM_T));

        SCI_TRACE_LOW("BLUE_InitMixInfo: alloc dm=%08x", s_blue_dm_ptr);

        SCI_MEMSET(s_blue_dm_ptr, 0, sizeof(BLUE_MIX_DM_T));
        s_blue_dm_ptr->data[BLUE_UPLINK_ID].remain_count = BLUE_LINK_BUF_SIZE;
        s_blue_dm_ptr->data[BLUE_DOWNLINK_ID].remain_count = BLUE_LINK_BUF_SIZE;
        s_blue_dm_ptr->record.remain_count = BLUE_MIX_BUF_SIZE;

        SCI_MEMSET(&s_blue_ctrl, 0, sizeof(BLUE_MIX_CTRL_T));
        s_blue_ctrl.flag = BLUE_FLAG_STOP;
        s_blue_ctrl.task_id = SCI_INVALID_BLOCK_ID;
    }
}

#ifdef BT_TASK_MODE
/*****************************************************************************/
//  Description:    open mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskOpen(uint32 priority)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL == ctrl_ptr || SCI_INVALID_BLOCK_ID != ctrl_ptr->task_id)
    {
        return;
    }

    ctrl_ptr->task_id = SCI_CreateAppThread("T_BLUEMIX",
                                    "Q_BLUEMIX",
                                    BLUE_TaskEntry,
                                    0,
                                    0,
                                    BLUEMIX_STACK_SIZE,
                                    BLUEMIX_QUEUE_NUM,
                                    priority,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);

     SCI_ASSERT(SCI_INVALID_BLOCK_ID != ctrl_ptr->task_id);  /*assert verified*/
}

/*****************************************************************************/
//  Description:    close mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskClose(void)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();
    BLUE_MIX_MSG_T *sig_ptr = NULL;

    if (NULL == ctrl_ptr || SCI_INVALID_BLOCK_ID == ctrl_ptr->task_id)
    {
        return;
    }

    SCI_SuspendThread(ctrl_ptr->task_id);

    sig_ptr = (void*)SCI_PeekSignal(ctrl_ptr->task_id);
    while (NULL != sig_ptr)
    {
        SCI_FREE(sig_ptr);
        sig_ptr = (void*)SCI_PeekSignal(ctrl_ptr->task_id);
    }

    SCI_TerminateThread(ctrl_ptr->task_id);
    SCI_DeleteThread(ctrl_ptr->task_id);

    ctrl_ptr->task_id = SCI_INVALID_BLOCK_ID;
}

/*****************************************************************************/
//  Description:    mix task entry function
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_TaskEntry(uint32 argc, void *argv)
{
    uint32 task_id = SCI_IdentifyThread();
    BLUE_MIX_MSG_T signal;
    BLUE_MIX_MSG_T *sig_ptr;

    for(;;)
    {
        SCI_DisableIRQ();
        sig_ptr = (BLUE_MIX_MSG_T *)SCI_GetSignal(task_id);
        
        signal = *sig_ptr;
        SCI_FREE(sig_ptr);
        SCI_RestoreIRQ();

        switch (signal.SignalCode)
        {
        case BLUE_SIG_MIX_DATA:
            BLUE_ExecMixData();
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description:    send signal to mix task
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_SendSignal(uint32 signalCode)
{
    BLUE_MIX_MSG_T *sig_ptr = NULL;
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL == ctrl_ptr || SCI_INVALID_BLOCK_ID == ctrl_ptr->task_id)
    {
        return;
    }

    sig_ptr = (BLUE_MIX_MSG_T *)SCI_ALLOC_APP(sizeof(BLUE_MIX_MSG_T));
    
    SCI_MEMSET(sig_ptr, 0, sizeof(BLUE_MIX_MSG_T));
    sig_ptr->SignalCode = signalCode;
    sig_ptr->SignalSize = sizeof(BLUE_MIX_MSG_T);
    sig_ptr->Sender = SCI_IdentifyThread();

    if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, ctrl_ptr->task_id))
    {
        SCI_ASSERT(FALSE); /*assert verified*/
    }
}

extern SCI_THREAD_PTR SCI_GetThreadBlockFromId(BLOCK_ID thread_id);
/*****************************************************************************/
//  Description:    get mix task queue avilable
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 BLUE_GetMixTask_Queue_Avilable(void)
{
    uint32          count    = 0;
    uint32          enqueued = 0;
    uint32          store    = 0;
    char            *queue_name_ptr    = NULL;
    TX_THREAD       * suspended_thread = NULL;
    TX_QUEUE        * next_queue       = NULL;
    SCI_THREAD_T    *thread_block = NULL;
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL == ctrl_ptr || SCI_INVALID_BLOCK_ID == ctrl_ptr->task_id)
    {
        return 0;
    }

    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(ctrl_ptr->task_id);
    SCI_ASSERT(NULL != thread_block);  /*assert to do*/

    tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
        &queue_name_ptr,
        (uint32 *)&enqueued,
        (uint32 *)&store,
        (TX_THREAD **)&suspended_thread,
        (uint32 *)&count,
        (TX_QUEUE **)&next_queue);

    return store;
}
#endif

/*****************************************************************************/
//  Description:    execute mix data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_ExecMixData(void)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();
    BLUE_MIX_DM_PTR dm_ptr = BLUE_GetMixDataManage();
    uint16 data_len = 0;

    if (NULL == ctrl_ptr || NULL == dm_ptr)
    {
        return;
    }

    if (BLUE_FLAG_PLAY == ctrl_ptr->flag)
    {
        data_len = BLUE_MixData(&dm_ptr->record, &dm_ptr->data[BLUE_UPLINK_ID], &dm_ptr->data[BLUE_DOWNLINK_ID]);
    }

    if (0 != data_len)
    {
        if  (NULL != dm_ptr->rec_cbf)
        {
            dm_ptr->rec_cbf(data_len);
        }
    }
    else
    {
        SCI_Sleep(20);
    }
}

/*****************************************************************************/
//  Description:    mix data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 BLUE_MixData(BLUE_MIX_DATA_PTR mix_ptr, BLUE_LINK_DATA_PTR uplink_ptr, BLUE_LINK_DATA_PTR downlink_ptr)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();
    uint16 uplink_len, downlink_len, mix_len;
    unsigned  i, j, count;
    uint16 *uplinkbuf, *downlinkbuf, *mixbuf;

    if (NULL == mix_ptr || NULL == uplink_ptr || NULL == downlink_ptr || NULL == ctrl_ptr)
    {
        return 0;
    }

    uplink_len = BLUE_GetLinkDataLen(uplink_ptr);
    downlink_len = BLUE_GetLinkDataLen(downlink_ptr);

    mix_len = MIN(uplink_len, downlink_len);
    if (mix_len < BLUE_MIX_DATA_MIN)
    {
        return 0;
    }

    mix_len = (mix_len / BLUE_MIX_DATA_MIN)*BLUE_MIX_DATA_MIN;
    mix_len = MIN(mix_len, BLUE_MIX_DATA_MAX);
    count = mix_len / VOICE_DATA_SIZE;
    SCI_TRACE_LOW("BLUE_MixData: count=%d, mix_len=%d, remain_count=%d",count, mix_len, mix_ptr->remain_count);

    if (mix_len > mix_ptr->remain_count)
    {
        return 0;
    }

    for (i=0; i < count; i++)
    {
        uplinkbuf = BLUE_GetLinkDataOutPtr(uplink_ptr, VOICE_DATA_SIZE);
        downlinkbuf = BLUE_GetLinkDataOutPtr(downlink_ptr, VOICE_DATA_SIZE);
        mixbuf = BLUE_GetMixDataInPtr(mix_ptr, VOICE_DATA_SIZE);
        SCI_ASSERT( uplinkbuf && downlinkbuf &&  mixbuf); /*assert to do*/

        for (j = 0; j < VOICE_DATA_SIZE/4; j++)
        {
            mixbuf[j*4+0] = (uint16)( ((int32)(int16)uplinkbuf[j*4+0] + (int32)(int16)downlinkbuf[j*4+0]) / 2 );
            mixbuf[j*4+1] = (uint16)( ((int32)(int16)uplinkbuf[j*4+1] + (int32)(int16)downlinkbuf[j*4+1]) / 2 );
            mixbuf[j*4+2] = (uint16)( ((int32)(int16)uplinkbuf[j*4+2] + (int32)(int16)downlinkbuf[j*4+2]) / 2 );
            mixbuf[j*4+3] = (uint16)( ((int32)(int16)uplinkbuf[j*4+3] + (int32)(int16)downlinkbuf[j*4+3]) / 2 );
        }
        BLUE_IncLinkDataOutPtr(uplink_ptr, VOICE_DATA_SIZE);
        BLUE_IncLinkDataOutPtr(downlink_ptr, VOICE_DATA_SIZE);
        BLUE_IncMixDataInPtr(mix_ptr, VOICE_DATA_SIZE);
    }

    if (BLUE_FLAG_PLAY != ctrl_ptr->flag)
    {
        mix_len = 0;
    }

    return mix_len;
}

/*****************************************************************************/
//  Description:    get link data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
#if 0
LOCAL BOOLEAN BLUE_GetLinkData(BLUE_LINK_DATA_PTR src_ptr, uint16 *dst_ptr, uint16 dst_len)
{
    if (NULL == src_ptr || NULL == dst_ptr)
    {
        return FALSE;
    }

    if (src_ptr->data_out + dst_len > BLUE_LINK_BUF_SIZE)
    {
        uint16 copy_len = BLUE_LINK_BUF_SIZE - src_ptr->data_out;

        SCI_MEM16CPY(dst_ptr, &src_ptr->data_buf[src_ptr->data_out], copy_len*sizeof(uint16));
        SCI_MEM16CPY(dst_ptr+copy_len, &src_ptr->data_buf[0], (dst_len - copy_len)*sizeof(uint16));
    }
    else
    {
        SCI_MEM16CPY(dst_ptr, &src_ptr->data_buf[src_ptr->data_out], dst_len*sizeof(uint16));
    }

    src_ptr->data_out = (src_ptr->data_out + dst_len) % BLUE_LINK_BUF_SIZE;
    SCI_DisableIRQ();
    src_ptr->remain_count += dst_len;
    SCI_RestoreIRQ();
    return TRUE;
}
#endif
LOCAL uint16 *BLUE_GetLinkDataOutPtr(BLUE_LINK_DATA_PTR src_ptr, uint16 dst_len)
{
    if (NULL == src_ptr)
    {
        return SCI_NULL;
    }

    if (src_ptr->data_out + dst_len > BLUE_LINK_BUF_SIZE)
    {
        uint16 copy_len = BLUE_LINK_BUF_SIZE - src_ptr->data_out;
        SCI_ASSERT(copy_len==0);    /*assert to do*/
        return &src_ptr->data_buf[0];
    }
    else
    {
        return &src_ptr->data_buf[src_ptr->data_out];
    }
}

LOCAL void BLUE_IncLinkDataOutPtr(BLUE_LINK_DATA_PTR src_ptr, uint16 dst_len)
{
    if (NULL == src_ptr)
    {
        return;
    }

    src_ptr->data_out = (src_ptr->data_out + dst_len) % BLUE_LINK_BUF_SIZE;
    SCI_DisableIRQ();
    src_ptr->remain_count += dst_len;
    SCI_RestoreIRQ();
}

/*****************************************************************************/
//  Description:    put data to data manager after mixed
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
#if 0
LOCAL BOOLEAN BLUE_PutMixData(BLUE_MIX_DATA_PTR dst_ptr, uint16 *src_ptr, uint16 src_len)
{
    if (NULL == src_ptr || NULL == dst_ptr)
    {
        return FALSE;
    }

    if(src_len > dst_ptr->remain_count)
    {
        SCI_TRACE_LOW("BLUE_PutMixData: src_len, remain_count=%d", src_len, dst_ptr->remain_count);
        return FALSE;
    }

    if(dst_ptr->data_in + src_len > BLUE_MIX_BUF_SIZE)
    {
        uint16 copy_len = BLUE_MIX_BUF_SIZE - dst_ptr->data_in;

        SCI_MEM16CPY(&dst_ptr->data_buf[dst_ptr->data_in], src_ptr, copy_len*sizeof(uint16));
        SCI_MEM16CPY(&dst_ptr->data_buf[0], src_ptr+copy_len, (src_len - copy_len)*sizeof(uint16));
    }
    else
    {
        SCI_MEM16CPY(&dst_ptr->data_buf[dst_ptr->data_in], src_ptr, src_len*sizeof(uint16));
    }

    dst_ptr->data_in = (dst_ptr->data_in + src_len) % BLUE_MIX_BUF_SIZE;
    SCI_DisableIRQ();
    dst_ptr->remain_count -= src_len;
    SCI_RestoreIRQ();
    SCI_TRACE_LOW("BLUE_PutMixData: remain_count=%d", dst_ptr->remain_count);
    return TRUE;
}
#endif

LOCAL uint16 *BLUE_GetMixDataInPtr(BLUE_MIX_DATA_PTR dst_ptr, uint16 src_len)
{
    if (NULL == dst_ptr)
    {
        return SCI_NULL;
    }

    if(src_len > dst_ptr->remain_count)
    {
        SCI_TRACE_LOW("BLUE_GetMixDataInPtr: src_len, remain_count=%d", src_len, dst_ptr->remain_count);
        return SCI_NULL;
    }

    if(dst_ptr->data_in + src_len > BLUE_MIX_BUF_SIZE)
    {
        uint16 copy_len = BLUE_MIX_BUF_SIZE - dst_ptr->data_in;
        SCI_ASSERT(copy_len == 0);  /*assert to do*/
        return &dst_ptr->data_buf[0];
    }
    else
    {
        return &dst_ptr->data_buf[dst_ptr->data_in];
    }
}

LOCAL void BLUE_IncMixDataInPtr(BLUE_MIX_DATA_PTR dst_ptr, uint16 src_len)
{
    if (NULL == dst_ptr)
    {
        return;
    }

    if(src_len > dst_ptr->remain_count)
    {
        SCI_TRACE_LOW("BLUE_IncMixDataInPtr: src_len, remain_count=%d", src_len, dst_ptr->remain_count);
        return;
    }

    dst_ptr->data_in = (dst_ptr->data_in + src_len) % BLUE_MIX_BUF_SIZE;
    SCI_DisableIRQ();
    dst_ptr->remain_count -= src_len;
    SCI_RestoreIRQ();
    SCI_TRACE_LOW("BLUE_IncMixDataInPtr: remain_count=%d", dst_ptr->remain_count);
}

/*****************************************************************************/
//  Description:    stop audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Stop(DEV_SETTING_T *ptDevSetting)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL != ctrl_ptr && BLUE_FLAG_STOP != ctrl_ptr->flag)
    {
        ctrl_ptr->flag = BLUE_FLAG_STOP;
        SCI_Sleep(20); // be sure no more signal is sent to
#ifdef BT_TASK_MODE
        BLUE_TaskClose();
#endif
        BLUE_FreeMixInfo();
    }
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    pause audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Pause(DEV_SETTING_T *ptDevSetting)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL != ctrl_ptr && BLUE_FLAG_PLAY == ctrl_ptr->flag)
    {
        ctrl_ptr->flag = BLUE_FLAG_PAUSE;
    }
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    resume audio device
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_RESULT_E BLUEMIX_Resume(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    BLUE_MIX_CTRL_PTR ctrl_ptr = BLUE_GetMixCtrlInfo();

    if (NULL != ctrl_ptr && BLUE_FLAG_PAUSE == ctrl_ptr->flag)
    {
        ctrl_ptr->flag = BLUE_FLAG_PLAY;
    }
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    register audio device callback function to get data
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 BLUEMIX_RegMixCallback(ADM_RECCALLBACK pCallback)
{
    BLUE_MIX_DM_PTR dm_ptr = BLUE_GetMixDataManage();
    ADM_RECCALLBACK rec_cbf = NULL;
    
    if (NULL != dm_ptr)
    {
        rec_cbf = pCallback;
        dm_ptr->rec_cbf = rec_cbf;
    }
    return (uint32)rec_cbf;
}

/*****************************************************************************/
//  Description:    unregister audio device callback function
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUEMIX_UnRegMixCallback(uint32 uiRegVector)
{
    BLUE_MIX_DM_PTR dm_ptr = BLUE_GetMixDataManage();

    if (NULL != dm_ptr)
    {
        dm_ptr->rec_cbf = NULL;
    }
}

/*****************************************************************************/
//  Description:    read mix data to buffer from data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUEMIX_ReadData(uint16* buf_ptr, uint32 buf_size, uint32 *p_real_size)
{
    uint32 data_len = 0;
    BLUE_MIX_DM_PTR dm_ptr = BLUE_GetMixDataManage();
    BLUE_MIX_DATA_PTR mix_ptr = NULL;

    if (NULL == dm_ptr || NULL == buf_ptr)
    {
        *p_real_size = 0;
        return;
    }

    mix_ptr = &dm_ptr->record;
    data_len = BLUE_GetMixDataLen(mix_ptr);
    SCI_TRACE_LOW("BLUEMIX_ReadData: data_len=%d, buf_size=%x", data_len, buf_size);

    data_len = MIN(data_len, buf_size);
    if (BLUE_GetMixData(buf_ptr, mix_ptr, data_len))
    {
        *p_real_size = data_len;
        return;
    }

    *p_real_size = 0;
}

/*****************************************************************************/
//  Description:    get link data length
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 BLUE_GetLinkDataLen(BLUE_LINK_DATA_PTR data_ptr)
{
    if (NULL == data_ptr)
    {
        return 0;
    }

    SCI_ASSERT(BLUE_LINK_BUF_SIZE >= data_ptr->remain_count);  /*assert to do*/
    return (BLUE_LINK_BUF_SIZE - data_ptr->remain_count);
}

/*****************************************************************************/
//  Description:    get mix data from data manager
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BLUE_GetMixData(uint16 *dst_ptr, BLUE_MIX_DATA_PTR src_ptr, uint16 dst_size)
{
    if (NULL == src_ptr || NULL == dst_ptr)
    {
        return FALSE;
    }

    if(dst_size > BLUE_MIX_BUF_SIZE - src_ptr->remain_count)
    {
        return FALSE;
    }

    if (src_ptr->data_out + dst_size > BLUE_MIX_BUF_SIZE)
    {
        uint16 copy_len = BLUE_MIX_BUF_SIZE - src_ptr->data_out;

        SCI_MEM16CPY(dst_ptr, &src_ptr->data_buf[src_ptr->data_out], copy_len*sizeof(uint16));
        SCI_MEM16CPY(dst_ptr+copy_len, &src_ptr->data_buf[0], (dst_size - copy_len)*sizeof(uint16));
    }
    else
    {
        SCI_MEM16CPY(dst_ptr, &src_ptr->data_buf[src_ptr->data_out], dst_size*sizeof(uint16));
    }

    src_ptr->data_out = (src_ptr->data_out + dst_size) % BLUE_MIX_BUF_SIZE;
    SCI_DisableIRQ();
    src_ptr->remain_count += dst_size;
    SCI_RestoreIRQ();
    return TRUE;
}

/*****************************************************************************/
//  Description:    free mix data info
//  Author:         Hui.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void BLUE_FreeMixInfo(void)
{
    if (NULL != s_blue_dm_ptr)
    {
        SCI_FREE(s_blue_dm_ptr);
        s_blue_dm_ptr = NULL;
        SCI_TRACE_LOW("BLUE_FreeMixInfo");

        SCI_MEMSET(&s_blue_ctrl, 0, sizeof(BLUE_MIX_CTRL_T));
        s_blue_ctrl.flag = BLUE_FLAG_STOP;
        s_blue_ctrl.task_id = SCI_INVALID_BLOCK_ID;
    }
}

