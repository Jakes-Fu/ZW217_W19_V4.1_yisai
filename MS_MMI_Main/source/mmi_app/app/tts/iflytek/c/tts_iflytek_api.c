/*****************************************************************************
** File Name:      tts_iflytek_api.c                                         *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      All Rights Reserved.                                      *
** Description:    Iflytek tts                                               *
*****************************************************************************/

#include "mmi_app_tts_trc.h"
#ifdef TTS_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


#include "sci_types.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "block_mem.h"
#include "mmidc_export.h"
//#include  "mmiaudio_ctrl.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "ivTTS.h"
#include "mmiidle_export.h"
#include "freq_cfg.h"
#include "os_api.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmisd_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmisms_id.h"
#include "mmiudisk_export.h"
#include "tts_iflytek_api.h"
#include "tts_iflytek_audio.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifm_export.h"
#include "mmitts_export.h"
#include "setjmp.h" 
#include "priority_app.h"

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
                                     MACRO DEFINITION
 **--------------------------------------------------------------------------*/
/* constant for TTS heap size */
#define ivTTS_HEAP_SIZE     KiTTS_HEAP_BUFFER_SIZE //128000

#define PCM_STREAM_PLAY_MAX_LEN     (15*1024)

#define MMITTS_TASK_STACK_SIZE                    4096 //任务栈大小
#define MMITTS_TASK_QUEUE_NUM                     8   //任务消息队列大小
// 把TTS的优先级提高一级是为了避免java 与TTS 同级而造成的tts 无法运行
// 而把MMI挂起，被JAVA阻塞的情况， 具体见cr184217
#define MMITTS_TASK_PRIORITY                      PRI_MMITTS_TASK //29//(SCI_PRIORITY_IDLE - 1)    提高优先级不低于图片解码

/* constant for cache allocation */

LOCAL BOOLEAN s_is_prompt_when_err = TRUE;

#ifdef TTS_FILE_RES

/*the value must refer to "TTS_TTSRESCATHEBUFFER_SIZE"*/
#ifdef TTS_SUPPORT_LOW_MEMORY
#define ivTTS_CACHE_SIZE    128
#else
#define ivTTS_CACHE_SIZE    512
#endif
#define ivTTS_CACHE_COUNT   1024
#define ivTTS_CACHE_EXT     8
//#define SaveTcardDirStr "e:\\"
//#define TTS_RES_FILE_NAME "Resource.irf"
#define TTS_CALLBACK_CHECKPOINT     8//16//48

// jun.hu 把tts 资源文件放到系统文件下
LOCAL const wchar s_mmitts_reserved_dir[]  = { 'T', 't', 's' ,0 };
LOCAL const wchar s_mmitts_res_file_name[] = {'R', 'e', 's', 'o', 'u', 'r', 'c', 'e', '.', 'i','r','f', 0};

LOCAL const wchar s_mmitts_default_file_path[] = L"C:\\Tts\\Resource.irf";

#define TTS_RES_FILE_HEADER_STR       "IsResV4"
#define TTS_HIDEN_DIR_LEN             3
#define TTS_RES_FILE_NAME_LEN         12
#define TTS_RES_FILE_CHECK_BYTE_NUM   7   //IsResV4

#else
#define ivTTS_CACHE_SIZE    0
#define ivTTS_CACHE_COUNT   0
#define ivTTS_CACHE_EXT     0
#define     TTS_CALLBACK_CHECKPOINT     32
#endif

typedef struct tagTCBParam
{
    ivPointer   pParam1st;
    ivPointer   pParam2nd;
    ivPointer   pParam3rd;
}TCBParam, *PCBParam;

typedef struct TxtParam_
{
    uint8* str;
    uint32 len;
    BOOLEAN is_unicode;
    uint32 position;
}TxtParam;

typedef struct
{
    SIGNAL_VARS
    int32 data;
} MMITTS_SIG_T;
typedef enum
{
    MMITTS_MSG_MIN = 0x304,
    MMITTS_MSG_START,
    MMITTS_MSG_STOP,
    MMITTS_MSG_OVER,
    MMITTS_MSG_EXIT,
    MMITTS_MSG_MAX
} MMITTS_MSG_TYPE_E;

typedef enum
{
    MMITTS_PLAYING,
    MMITTS_STOPING,
    MMITTS_EXITING,
    MMITTS_OVERING,
    MMITTS_IDLE
}MMITTS_STATUS_TYPE_E;


/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN                               s_tts_is_running           = FALSE;      //tts运行状态标志
LOCAL MMITTS_STRING_T                       s_tts_text = {0};
LOCAL uint32                                s_tts_pcm_stream_count     = 0;

LOCAL uint8*                                s_tts_stream_buffer = PNULL;
//uint8                                     g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] = {0};
LOCAL char*                                 s_tts_voice_data           = PNULL;
LOCAL BOOLEAN                               MMITTS_user_stop           = FALSE;
LOCAL uint32                                s_kitts_callback_count     = 0;          //kitts回调次数

LOCAL BOOLEAN                               s_tts_SynthText_is_running = FALSE;      //tts运行状态标志
LOCAL uint8                                 s_tts_playover_timer_id    = 0;  
LOCAL BLOCK_ID                              s_tts_task_id              = 0;   
LOCAL MMITTS_STATUS_TYPE_E                  MMITTS_Task_status         = MMITTS_IDLE;

LOCAL char*                                 KiTTS_HeapBuffer           = PNULL;
LOCAL char*                                 CycleBuffer                = NULL;
LOCAL uint32                                CycleBufferLen             = 0;
LOCAL uint32                                putPosition                = 0;
LOCAL uint32                                getPosition                = 0;
LOCAL BOOLEAN                               TTS_ClkIsChange            = FALSE;

// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎把此变量置为TRUE ...(jun.hu)....
LOCAL BOOLEAN                               s_asynch_stop             = FALSE; //

jmp_buf                                     s_jumpbuf_tts;

#ifndef TTS_FILE_RES
#ifdef WIN32
LOCAL const unsigned char TTS_ResData = "";
#else
LOCAL const unsigned char TTS_ResData[]=
{
#ifdef TTS_DATA_RES_LIMIT
    #include "tts_res_array_600k.dat"
#else
    #include "tts_res_array.dat"
#endif  
};
#endif

#else
char * tts_resDataCathe;
#endif

/**--------------------------------------------------------------------------*
                                     FUNCTION DECLARE
 **--------------------------------------------------------------------------*/ 
LOCAL void TTS_SynthText(TxtParam* txtPar,char*HeapBuffer,uint32 HEAP_BUFFER_SIZE,void *ResData);
LOCAL void TTS_SendSignaltoTask(
                             MMITTS_MSG_TYPE_E type, 
                             BLOCK_ID task_id,
                             int32 data
                             );
LOCAL uint32 cycleBuffeGetUsedNum(void );
LOCAL BOOLEAN cycleBufferPut(char* Buffer,uint32 BufferLen);
LOCAL BOOLEAN cycleBufferGet(char* Buffer,uint32 *BufferLen_p);
LOCAL uint32 cycleBuffeGetEmptyNum(void );
LOCAL void cycleBufferInit( char* Buffer,uint32 BufferLen);
LOCAL void TTS_RestoreClk(void);
/*****************************************************************************/
//  Description : 申请的内存
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 把它改成了动态分配的方式....
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TTS_MemoryAlloc(void);
/*****************************************************************************/
//  Description : 当需要异步停止TTS时.....
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopTTSByASynch(void);
#ifdef TTS_FILE_RES
/*****************************************************************************/
//  Description : create tts dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: for 把tts的文件夹放到系统文件夹隐藏起来....
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSHidenDir(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSResouceFileHandle(
                                         MMIFILE_HANDLE* file_handle_ptr // [out]
                                         );
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AUDIO_PcmStreamPlayCallback(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset);
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          );
/*****************************************************************************/
//  Description : AllocTTS_HeapBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_HeapBuffer(void);
/*****************************************************************************/
//  Description : AllocTTS_VoiceData
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_VoiceData(void);
/*****************************************************************************/
//  Description : free tts HeapBuffer
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeKiTTS_HeapBuffer(void);
/*****************************************************************************/
//  Description : free tts VoiceData
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_VoiceData(void);
/*****************************************************************************/
//  Description : AllocTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_StreamBuffer(void);
/*****************************************************************************/
//  Description : FreeTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL void FreeTTS_StreamBuffer(void);
/*****************************************************************************/
//  Description : AllocTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_ResDataCathe(void);
/*****************************************************************************/
//  Description : FreeTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_ResDataCathe(void);

/*****************************************************************************/
//  Description : handle the timer msg 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TTS_HandleTTSPlayOverTimerMsg(
                                          uint8 timer_id,
                                          uint32   param
                                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TTSPlayPreCheck(TTS_ALL_RING_TYPE_E ring_type);


/*****************************************************************************/
//  Description : GetDitigMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetDitigMode(MMITTS_DIGIT_MODE_TYPE_E digit_mode);

/*****************************************************************************/
//  Description : GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetPuncMode(MMITTS_PUNC_MODE_TYPE_E punc_mode);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AUDIO_PcmStreamPlayCallback(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset)
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          )
{
    uint32 usedDataLen;   

    //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_307_112_2_18_3_3_44_159,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
    if(PNULL == s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_310_112_2_18_3_3_44_160,(uint8*)"");
        if(!AllocTTS_StreamBuffer())
        {
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
    }
    ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =PCM_STREAM_PLAY_MAX_LEN;
    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr =s_tts_stream_buffer;

    while (cycleBuffeGetUsedNum()<=PCM_STREAM_PLAY_MAX_LEN) 
    {
        if (s_tts_is_running==FALSE||s_tts_SynthText_is_running==FALSE ||(MMITTS_user_stop==TRUE))
        {
            /** puiDataLength=0;*/
            break;
        }
        SCI_Sleep(50);
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_327_112_2_18_3_3_44_161,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
    }

    if (!s_tts_is_running || MMITTS_user_stop)
    {
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength=0;
    }
        
    if(0 < ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength)
    {
        usedDataLen=cycleBuffeGetUsedNum();
        usedDataLen-=usedDataLen%2;
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = MIN(PCM_STREAM_PLAY_MAX_LEN,usedDataLen);
        
        cycleBufferGet((char *)(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr), (uint32 *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));
        
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_343_112_2_18_3_3_44_162,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
        if(0 < ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength)
        {
            return AUDIO_STREAM_GET_SRC_SUCCESS;
        }
        else
        {
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_355_112_2_18_3_3_44_163,(uint8*)"");
        return AUDIO_STREAM_GET_SRC_ERROR;
    }

}

#ifndef WIN32

/* iTTS系统有PCM语音数据输出的时候会调用该回调函数输出生成的PCM数据 */ 
LOCAL ivTTSErrID ivCall KiTTS_PcmStreamOutputCallback(
     ivPointer  lpParameter,    /* [in] user callback parameter */
    ivUInt16        wCode,          /* [in] output data code */
    ivCPointer  lpcData,        /* [in] output data buffer */
    ivSize  dwSize )        /* [in] output data size */
{
//  uint32  tick1 = 0,tick2 = 0;
//  int counter=0;
    
    /* 从回调的用户参数获取文件句柄 */
    //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback()  enter time:%u counter =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_373_112_2_18_3_3_44_164,(uint8*)"ud",SCI_GetTickCount (),s_kitts_callback_count);

    while (cycleBuffeGetEmptyNum()<=dwSize )
    {
        //if (s_tts_is_running==FALSE ||!TTS_IsCreatePcmStreamHandle() ||MMITTS_user_stop==TRUE||!TTSAPI_GetAudioIsOpen())
        if (s_tts_is_running==FALSE ||MMITTS_user_stop==TRUE||!TTSAPI_GetAudioIsOpen())
            return ivTTS_ERR_FAILED;
        SCI_Sleep(50);
        //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback()  waiting time:%u empty =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_381_112_2_18_3_3_44_165,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetEmptyNum());
/*      counter++;
        if(counter>80)
        {
            uint32 param=1;
            VB_SoftReset();
            MMK_PostMsg(VIRTUAL_WIN_ID,MSG_TTS_STOP,(DPARAM)&param,sizeof(param));
            MMK_PostMsg(VIRTUAL_WIN_ID,MSG_TTS_PLAY,PNULL,0);
            MMI_TriggerMMITask();
            return 
    }
*/  
    }   
    //if (s_tts_is_running==FALSE ||!TTS_IsCreatePcmStreamHandle()|| MMITTS_user_stop==TRUE ||!TTSAPI_GetAudioIsOpen())
    if (s_tts_is_running==FALSE || MMITTS_user_stop==TRUE ||!TTSAPI_GetAudioIsOpen())
    {
        //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback()  error exit time:%u counter =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_396_112_2_18_3_3_44_166,(uint8*)"ud",SCI_GetTickCount (),s_kitts_callback_count);
        return ivTTS_ERR_FAILED;
    }
    
    s_kitts_callback_count++;   
    if( s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT )
    {
        //合成ing播放前的初始化
        //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_404_112_2_18_3_3_44_167,(uint8*)"");
        //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback() begin to play:%u used =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_405_112_2_18_3_3_44_168,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
        s_tts_pcm_stream_count = TTS_GetPcmStreamId();
        MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_AUDIO_PLAY, &s_tts_pcm_stream_count, sizeof (s_tts_pcm_stream_count));
        MMI_TriggerMMITask();//加快反应 
    }
    cycleBufferPut((char *)lpcData, dwSize);
    
    //SCI_TRACE_LOW:"KiTTS_PcmStreamOutputCallback()  enter time:%u used =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_412_112_2_18_3_3_44_169,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
    return ivTTS_ERR_OK;
}
#else
/* iTTS系统有PCM语音数据输出的时候会调用该回调函数输出生成的PCM数据 */ 
LOCAL ivTTSErrID ivCall KiTTS_PcmStreamOutputCallback(
     ivPointer  lpParameter,    /* [in] user callback parameter */
    ivUInt16        wCode,          /* [in] output data code */
    ivCPointer  lpcData,        /* [in] output data buffer */
    ivSize  dwSize )        /* [in] output data size */
{
    return ivTTS_ERR_OK;
}
#endif

/********************************Cycle buffer*********************************/


/*api*/
LOCAL uint32 cycleBuffeGetEmptyNum(void )
{
    if (putPosition>=getPosition)
        return CycleBufferLen-(putPosition-getPosition)-1;
    else
        return getPosition-putPosition-1;
        
}

LOCAL uint32 cycleBuffeGetUsedNum(void )
{
    uint32 len = 0;
    if (putPosition > getPosition)
    {
        len = putPosition - getPosition;
    }
    else if(getPosition > putPosition)
    {
        len = CycleBufferLen - (getPosition - putPosition);
    }
    return len;
}

LOCAL void cycleBufferInit( char* Buffer,uint32 BufferLen)
{
    CycleBuffer=Buffer;
    CycleBufferLen=BufferLen;
    putPosition=0;
    getPosition=0;
}

LOCAL BOOLEAN cycleBufferPut(char* Buffer,uint32 BufferLen)
{
    BOOLEAN ret = FALSE;
    if (cycleBuffeGetEmptyNum() > BufferLen)
    {
        uint32 i = CycleBufferLen - putPosition;
        if (BufferLen+putPosition > CycleBufferLen) 
        {
            SCI_MEMCPY(CycleBuffer+putPosition, Buffer, i);
            SCI_MEMCPY(CycleBuffer, Buffer+i, BufferLen-i);
        }
        else
        {
            SCI_MEMCPY(CycleBuffer+putPosition, Buffer, BufferLen);
        }
        
        putPosition = (BufferLen+putPosition) % CycleBufferLen;
        ret = TRUE;
    }   
    return ret;
}

LOCAL BOOLEAN cycleBufferGet(char* Buffer,uint32 *BufferLen_p)
{
    BOOLEAN ret      = FALSE;
    uint32 activeNum = cycleBuffeGetUsedNum();

    if (0 < activeNum)
    {
        uint32 i = CycleBufferLen - getPosition;
        uint32 outLen = MIN(*BufferLen_p, activeNum);
        if (outLen+getPosition > CycleBufferLen) 
        {
            SCI_MEMCPY(Buffer, CycleBuffer+getPosition, i);
            SCI_MEMCPY(Buffer+i, CycleBuffer, outLen-i);
        }
        else
        {
            SCI_MEMCPY(Buffer, CycleBuffer+getPosition, outLen);
        }
        
        *BufferLen_p = outLen;
        getPosition = (getPosition+outLen) % CycleBufferLen;
        ret = TRUE;
    }
    
    return ret;
    
}


LOCAL uint32 TTS_GetQueueAvilable(BLOCK_ID id)
{
    uint32          status              = 0;
    uint32          count               = 0;
    uint32          enqueued            = 0;
    uint32          store               = 0;
    char            *queue_name_ptr     = SCI_NULL;
    TX_THREAD       * suspended_thread  = SCI_NULL; 
    TX_QUEUE        * next_queue        = SCI_NULL;
    SCI_THREAD_T    * thread_block  = SCI_NULL;
       
    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(id);
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
                               &queue_name_ptr,
                               (uint32 *)&enqueued,
                               (uint32 *)&store,
                               (TX_THREAD **)&suspended_thread,
                               (uint32 *)&count,
                               (TX_QUEUE **)&next_queue);
    
    //SCI_TRACE_LOW:"TTS_GetQueueAvilable: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_530_112_2_18_3_3_44_170,(uint8*)"d",store);
    
    return store;
}

LOCAL void TTS_SendSignaltoTask(
                             MMITTS_MSG_TYPE_E type, 
                             BLOCK_ID task_id,
                             int32 data
                             )
{
    MMITTS_SIG_T *sig_ptr = PNULL;
    
    //保证消息能够到达
    if (task_id == PNULL)
    {
        return;
    }
    sig_ptr = (MMITTS_SIG_T *)SCI_ALLOC(sizeof(MMITTS_SIG_T));
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_551_112_2_18_3_3_44_171,(uint8*)"");
        return;
    }
    
    sig_ptr->SignalCode = (uint16)type;
    sig_ptr->SignalSize = sizeof(xSignalHeaderRec);
    sig_ptr->Sender = PNULL;
    sig_ptr->data = data;
    
    if(SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, task_id))
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_562_112_2_18_3_3_44_172,(uint8*)"");
    }
}

PUBLIC void TTS_Task(
                       uint32 argc, 
                       void * argv
                       )
{
    MMITTS_SIG_T  *   sig_ptr = PNULL;
    BOOLEAN             is_task_exit = FALSE;
#ifdef TTS_FILE_RES
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
#endif
    //SCI_TRACE_LOW:"TTS_Task start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_576_112_2_18_3_3_44_173,(uint8*)"");
    
    for(;;)
    {
        sig_ptr = (MMITTS_SIG_T *)SCI_GetSignal(SCI_IdentifyThread());
        
        if(sig_ptr != PNULL)
        {
            switch (sig_ptr->SignalCode)
            {
            case MMITTS_MSG_EXIT:
                MMITTS_Task_status=MMITTS_EXITING;
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_EXIT"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_588_112_2_18_3_3_45_174,(uint8*)"");
                is_task_exit=TRUE;
                break;
                
            case MMITTS_MSG_START:
                {
                    TxtParam txtPar;
                    //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_START"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_595_112_2_18_3_3_45_175,(uint8*)"");
                    MMITTS_Task_status = MMITTS_PLAYING;
                    
                    if (NULL==s_tts_voice_data ||NULL==KiTTS_HeapBuffer)
                    {
                        break;
                    }
                    
                    if (!s_tts_is_running )
                    {
                        break;
                    }
                    
                    s_kitts_callback_count  = 0;    
                    cycleBufferInit(s_tts_voice_data, KiTTS_DATA_BUFFER_SIZE);  
                    s_tts_SynthText_is_running=TRUE;
                    
                    txtPar.is_unicode=s_tts_text.is_ucs2;
                    txtPar.len=s_tts_text.tts_str_len;
                    txtPar.str=s_tts_text.str_arr;
                    txtPar.position=0;
#ifdef TTS_FILE_RES
                    CreateTTSResouceFileHandle(&file_handle);
                    //SCI_TRACE_LOW:"TTS_Task :MMIAPIFMM_CreateFile: handle=%u"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_618_112_2_18_3_3_45_176,(uint8*)"u",file_handle);
                    
                    if (SFS_INVALID_HANDLE != file_handle)
                    {
                        TTS_SynthText(   &txtPar, KiTTS_HeapBuffer, ivTTS_HEAP_SIZE,( void *)file_handle);
                        StopTTSByASynch();
                    }
                    else
                    {
                        MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL,0 );
                        MMI_TriggerMMITask();//加快反应
                    }
                    
                    if (SFS_INVALID_HANDLE != file_handle)
                    {
                        MMIAPIFMM_CloseFile(file_handle);
                    }
#else
                    TTS_SynthText(   &txtPar, KiTTS_HeapBuffer, ivTTS_HEAP_SIZE,( void *)TTS_ResData);
                    StopTTSByASynch();
#endif
                    s_tts_SynthText_is_running=FALSE;
                }
                break;
                
            case MMITTS_MSG_STOP:
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_STOP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_644_112_2_18_3_3_45_177,(uint8*)"");
                MMITTS_Task_status=MMITTS_STOPING;
                TTS_MemoryRelease();
                break;
                
            case MMITTS_MSG_OVER:
                MMITTS_Task_status=MMITTS_OVERING;
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_OVER"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_651_112_2_18_3_3_45_178,(uint8*)"");
                {
                    //启动定时器来关闭资源
                    if( s_tts_playover_timer_id != 0 )
                    {
                        MMK_StopTimer(s_tts_playover_timer_id);
                        s_tts_playover_timer_id = 0;
                    }
                    if (s_tts_is_running &&(MMITTS_user_stop==FALSE))
                    {
                        s_tts_playover_timer_id = MMK_CreateTimerCallback(  2000, TTS_HandleTTSPlayOverTimerMsg, NULL, FALSE);
                    }
                }
                break;
                
            default:
                break;
            }
            
            SCI_FREE(sig_ptr);
        }
        
        if (is_task_exit)
        {
            break;
        }
        
            MMITTS_Task_status = MMITTS_IDLE;
    }
    
    s_tts_task_id = 0;
    
    //SCI_TRACE_LOW:"TTS_Task exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_683_112_2_18_3_3_45_179,(uint8*)"");
    MMITTS_Task_status=MMITTS_IDLE;
    SCI_ThreadExit(); 
}



LOCAL void TTS_ChangeClk(void)
{
    if (!TTS_ClkIsChange)
    {
        TTS_ClkIsChange=TRUE;
        MMI_SetFreq(FREQ_INDEX_APP_HIGH);
    }
}

LOCAL void TTS_RestoreClk(void)
{
    if (TTS_ClkIsChange)
    {
        TTS_ClkIsChange=FALSE;
        MMI_RestoreFreq(); 
    }
}

PUBLIC BOOLEAN  TTSAPI_SynthTextByTask(void )
{    

    //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_711_112_2_18_3_3_45_180,(uint8*)"");
        
#ifdef TTS_FILE_RES
    if (!TTSAPI_checkResFile())
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask no res file exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_716_112_2_18_3_3_45_181,(uint8*)"");
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask res file find"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_721_112_2_18_3_3_45_182,(uint8*)"");
    }
#endif

    if (s_tts_is_running)
    {
        return FALSE;
    }

    if (!TTS_MemoryAlloc())
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask no memory exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_732_112_2_18_3_3_45_183,(uint8*)"");
        if (s_is_prompt_when_err)
        {
            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
        }
        return FALSE;
    }
         
    s_tts_is_running = TRUE; 

    if (TTSAPI_GetAudioIsOpen())
    {
        TTS_StopAndClosePcmStreamHandle();
    }
         
    if (0 == s_tts_task_id)
    {
        s_tts_task_id = SCI_CreateThread( 
                                        "T_TTS", 
                                        "Q_TTS", 
                                        TTS_Task, 
                                        0, 
                                        0, 
                                        MMITTS_TASK_STACK_SIZE, 
                                        MMITTS_TASK_QUEUE_NUM, 
                                        MMITTS_TASK_PRIORITY, 
                                        SCI_PREEMPT, 
                                        SCI_AUTO_START
                                        );   
    }
    else
    {
         /*等待TTS TASK 处理结束*/
        int i=4;
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task created"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_766_112_2_18_3_3_45_184,(uint8*)"");
        while(i)
        {
            i--;
            if (MMITTS_TASK_QUEUE_NUM==TTS_GetQueueAvilable(s_tts_task_id))
                break;
            SCI_Sleep(100);
            //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task while"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_773_112_2_18_3_3_45_185,(uint8*)"");
        }
        /*等待一段时间还不能结束，就退出*/
        if (0==i)
        {
            //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task quue exit"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_778_112_2_18_3_3_45_186,(uint8*)"");
            TTS_MemoryRelease();
            return FALSE;
        }
                    
    }
    
    if (TTS_CreatePcmStreamHandle(s_tts_text.ring_type, AUDIO_PcmStreamPlayCallback,s_tts_text.tts_route))/*lint !e64*/
    {
        TTS_SendSignaltoTask(MMITTS_MSG_START,s_tts_task_id,0);
        TTS_ChangeClk();
        SCI_Sleep(50);  /*run TTS task*/
    }
    //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_791_112_2_18_3_3_45_187,(uint8*)"");
    return TRUE;
}
/*****************************************************************************/
//  Description : AllocTTS_HeapBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_HeapBuffer(void)
{
    FreeKiTTS_HeapBuffer();
    // jun hu 2010/11/4 开始采用动态分配的方式.....
    /*KiTTS_HeapBuffer = (char*)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSHEAPBUFFER); */
    KiTTS_HeapBuffer = SCI_ALLOCA(KiTTS_HEAP_BUFFER_SIZE);
    return (PNULL != KiTTS_HeapBuffer);
    
}
/*****************************************************************************/
//  Description : AllocTTS_VoiceData
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_VoiceData(void)
{
    FreeTTS_VoiceData();
    // jun hu 2010/11/4 开始采用动态分配的方式.....
    /*s_tts_voice_data = (char*)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSVOICEDATA);*/
    s_tts_voice_data = SCI_ALLOCA(KiTTS_DATA_BUFFER_SIZE);
    return (PNULL != s_tts_voice_data);
    
}
/*****************************************************************************/
//  Description : AllocTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_ResDataCathe(void)
{
#ifdef TTS_FILE_RES
    FreeTTS_ResDataCathe();
    tts_resDataCathe = SCI_ALLOCA(KiTTS_RESCATHEBUFFER_SIZE);
    return (PNULL != tts_resDataCathe);
#else
    return TRUE;
#endif
}
/*****************************************************************************/
//  Description : FreeTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_ResDataCathe(void)
{
#ifdef TTS_FILE_RES
    if(PNULL != tts_resDataCathe)
    {
        //SCI_TRACE_LOW:" tts tts_resDataCathe  is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_843_112_2_18_3_3_45_188,(uint8*)"");
        SCI_FREE(tts_resDataCathe);
        tts_resDataCathe = PNULL;
    }
#endif
}
/*****************************************************************************/
//  Description : free tts HeapBuffer
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeKiTTS_HeapBuffer(void)
{
    if(PNULL != KiTTS_HeapBuffer)
    {
        //SCI_TRACE_LOW:" tts KiTTS_HeapBuffer   is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_857_112_2_18_3_3_45_189,(uint8*)"");
        SCI_FREE(KiTTS_HeapBuffer);
        KiTTS_HeapBuffer = PNULL;
    }
}
/*****************************************************************************/
//  Description : free tts VoiceData
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_VoiceData(void)
{
    if(PNULL != s_tts_voice_data)
    {
        //SCI_TRACE_LOW:"tts s_tts_voice_data   is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_870_112_2_18_3_3_45_190,(uint8*)"");
        SCI_FREE(s_tts_voice_data);
        s_tts_voice_data = PNULL;
    }
}

/*****************************************************************************/
//  Description : AllocTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_StreamBuffer(void)
{
    if(PNULL != s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_884_112_2_18_3_3_45_191,(uint8*)"");
        SCI_FREE(s_tts_stream_buffer);
        s_tts_stream_buffer = PNULL;
    }
    s_tts_stream_buffer = SCI_ALLOCA(PCM_STREAM_PLAY_MAX_LEN);
    return (PNULL != s_tts_stream_buffer);
}
/*****************************************************************************/
//  Description : FreeTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL void FreeTTS_StreamBuffer(void)
{
    if(PNULL != s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]:FreeTTS_StreamBuffer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_899_112_2_18_3_3_45_192,(uint8*)"");
        SCI_FREE(s_tts_stream_buffer);
        s_tts_stream_buffer = PNULL;
    }
}

/*****************************************************************************/
//  Description : 申请的内存
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 把它改成了动态分配的方式....
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TTS_MemoryAlloc(void)
{
    BOOLEAN result = TRUE;
    
    //SCI_TRACE_LOW:" [MMITTS]: TTS_MemoryAlloc entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_913_112_2_18_3_3_45_193,(uint8*)"");
    if(!AllocTTS_HeapBuffer())
    {
        //SCI_TRACE_LOW:" [MMITTS]: MemoryAlloc HeapBuffer failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_916_112_2_18_3_3_45_194,(uint8*)"");
        result  = FALSE;
    }

    if(result && !AllocTTS_VoiceData())
    {
        //SCI_TRACE_LOW:" [MMITTS]:  AllocTTS_VoiceData failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_922_112_2_18_3_3_45_195,(uint8*)"");
        result  = FALSE;
    }

    if(result && !AllocTTS_StreamBuffer())
    {
        //SCI_TRACE_LOW:" [MMITTS]:  AllocTTS_StreamBuffer failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_928_112_2_18_3_3_45_196,(uint8*)"");
        result  = FALSE;
    }
    
#ifdef TTS_FILE_RES
    if(result && !AllocTTS_ResDataCathe())
    {
        //SCI_TRACE_LOW:"[MMITTS]:  AllocTTS_ResDataCathe failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_935_112_2_18_3_3_45_197,(uint8*)"");
        result  = FALSE;
    }
#endif
    if(result)
    {
        //SCI_TRACE_LOW:"[MMITTS]:KiTTS_HeapBuffer address = %x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_941_112_2_18_3_3_45_198,(uint8*)"d", (uint32)KiTTS_HeapBuffer );
        //SCI_TRACE_LOW:"[MMITTS]:KiTTS_voice data address = %x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_942_112_2_18_3_3_45_199,(uint8*)"d", (uint32)s_tts_voice_data );
    }
    else
    {
        FreeKiTTS_HeapBuffer();
        FreeTTS_VoiceData();
        FreeTTS_StreamBuffer();
#ifdef TTS_FILE_RES
        FreeTTS_ResDataCathe();
#endif
    }
    return result;
}

/*****************************************************************************/
//  Description : TTS_MemoryAlloc
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 修改了此函数....
//  Note: 
/*****************************************************************************/
PUBLIC void TTS_MemoryRelease(void)
{
    //SCI_TRACE_LOW:" tts TTS_MemoryRelease entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_962_112_2_18_3_3_45_200,(uint8*)"");
    FreeKiTTS_HeapBuffer();
    FreeTTS_VoiceData();
    FreeTTS_StreamBuffer();
#ifdef TTS_FILE_RES
    FreeTTS_ResDataCathe();
#endif
}
/*****************************************************************************/
//  Description :  同步停tts
//  Global resource dependence : none
//  Author: jun hu 2010/10 修改了此函数....
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void TTSAPI_StopSynthTextByTask(void)
{
    // int i=30;
    
    /*NOTE: !!下面这个is_call_ring不能要，否则会造成多个assert问题, jun.hu*/
    //BOOLEAN is_call_ring = TTSAPI_IsPlayRing(MMISET_RING_TYPE_CALL);
    
    //SCI_TRACE_LOW:"MMITTS TTSAPI_StopSynthTextByTask entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_981_112_2_18_3_3_45_201,(uint8*)"u",SCI_GetTickCount ());
    // 增加对MMITTS_user_stop的判断是支持对同步和异步停TTS的两种方式的支持，以使两种互斥进行.....
    if(!MMITTS_user_stop)
    {
        MMITTS_user_stop = TRUE;
        
        if (TTSAPI_GetAudioIsOpen())
        {
            TTS_StopAndClosePcmStreamHandle();
        }
        
        //启动定时器来关闭资源
        if( s_tts_playover_timer_id != 0 )
        {
            MMK_StopTimer(s_tts_playover_timer_id);
            s_tts_playover_timer_id = 0;
        }
        
        
        /*等待TTS TASK 结束*/
        if (s_tts_task_id!=0   /*&&   //Code from ZhenhaoHuang's advice....
            !is_call_ring*/)/*这里不能要is_call_ring这个条件,否则会造成一些assert问题，jun.hu*/
        {
            while (MMITTS_Task_status!=MMITTS_IDLE 
                || (MMITTS_TASK_QUEUE_NUM!=TTS_GetQueueAvilable(s_tts_task_id)))  
            {
                SCI_Sleep(100);
#if 0
                /*防止死循环*/
                if (0==i--)
                    break;
#endif
            }
        }
        
        TTS_RestoreClk();
        TTS_MemoryRelease();
        s_tts_is_running = FALSE;
        MMITTS_user_stop = FALSE;
    }

    //SCI_TRACE_LOW:"TTSAPI_StopSynthTextByTask exit time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1022_112_2_18_3_3_45_202,(uint8*)"u",SCI_GetTickCount ());
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT， 故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void TTSAPI_StopASynchTextByTask(void)
{
    //SCI_TRACE_LOW:"MMITTS TTSAPI_StopASynchTextByTask entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1031_112_2_18_3_3_46_203,(uint8*)"u",SCI_GetTickCount ());
    
    if(!MMITTS_user_stop)
    {
        MMITTS_user_stop = TRUE;
        if (TTSAPI_GetAudioIsOpen())
        {
            TTS_StopAndClosePcmStreamHandle();
        }
        //启动定时器来关闭资源
        if( s_tts_playover_timer_id != 0 )
        {
            MMK_StopTimer(s_tts_playover_timer_id);
            s_tts_playover_timer_id = 0;
        }
        
        s_asynch_stop = TRUE;
    }
    
}
/*****************************************************************************/
//  Description : 当需要异步停止TTS时.....
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopTTSByASynch(void)
{
    if(s_asynch_stop)
    {
        TTS_RestoreClk();
        TTS_MemoryRelease();
        s_tts_is_running = FALSE;
        MMITTS_user_stop=FALSE;
        s_asynch_stop = FALSE;
    }
}
/*************************************************************************************/
                            /*TTS kernel*/
/*************************************************************************************/
#ifdef TTS_FILE_RES
/* read resource callback */
LOCAL void ivCall ReadResCB(
                      ivPointer     pParameter,     /* [in] user callback parameter */
                      ivPointer     pBuffer,        /* [out] read resource buffer */
                      ivResAddress  iPos,           /* [in] read start position */
                      ivResSize     nSize )         /* [in] read size */
{
    MMIFILE_HANDLE pFile = (MMIFILE_HANDLE)pParameter;
    uint32 bytes_read;
    
    MMIAPIFMM_SetFilePointer(pFile, iPos, MMIFILE_SEEK_BEGIN);

    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(
                                pFile,
                                pBuffer,
                                nSize,
                                &bytes_read,
                                NULL
                                ))
    {
        //SCI_TRACE_LOW:"[MMITTS]:ReadResCB FAIL!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1087_112_2_18_3_3_46_204,(uint8*)"");
        longjmp(s_jumpbuf_tts, 1);
    }
}
#else
/* read resource callback */
LOCAL void ivCall ReadResCB(
                      ivPointer     pParameter,     /* [in] user callback parameter */
                      ivPointer     pBuffer,        /* [out] read resource buffer */
                      ivResAddress  iPos,           /* [in] read start position */
                      ivResSize     nSize )         /* [in] read size */
{
    uint8 * pBuf = pParameter;
    memcpy(pBuffer,  pBuf+iPos, nSize);
}
#endif

/* input callback */
LOCAL ivTTSErrID ivCall InputCB(
                          ivPointer     pParameter,     /* [in] user callback parameter */
                          ivPointer     pText,          /* [out] input text buffer */
                          ivSize ivPtr  pnSize )        /* [out] input text size */
{
    PCBParam    pParam;
    TxtParam* txtPar;
    uint32 copyLen;

    if (MMITTS_user_stop==TRUE)
        return ivTTS_ERR_END_OF_INPUT;
    
    pParam = (PCBParam)pParameter;
    txtPar=(TxtParam*)pParam->pParam1st;
    copyLen=MIN(*pnSize,txtPar->len-txtPar->position);

    //SCI_TRACE_LOW:"MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1119_112_2_18_3_3_46_205,(uint8*)"uddud",pParameter,txtPar,copyLen,pParam,txtPar->position);
    
    if (copyLen!=0)
        memcpy(pText,txtPar->str+txtPar->position,copyLen);
    
    txtPar->position+=copyLen;
        
    *pnSize = copyLen;
        
    return (0 == *pnSize)?ivTTS_ERR_END_OF_INPUT:ivTTS_ERR_OK;
}

/*****************************************************************************/
//  Description : GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetPuncMode(MMITTS_PUNC_MODE_TYPE_E punc_mode)
{
	uint16 tts_punc_mode = 0;

	switch(punc_mode) 
	{
	case TTS_PUNC_OFF:
		tts_punc_mode = 0;
		break;

	case TTS_PUNC_ON:
		tts_punc_mode = 1;
		break;

	default:
		break;
	}
    SCI_TRACE_LOW("[TTS] GetDitigMode punc_mode = %d, tts_punc_mode = %d",punc_mode,tts_punc_mode);
	return tts_punc_mode;
}

/*****************************************************************************/
//  Description : GetDitigMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetDitigMode(MMITTS_DIGIT_MODE_TYPE_E digit_mode)
{
	uint16 tts_digit_mode = ivTTS_READDIGIT_AUTO;	
	switch(digit_mode) 
	{
	case TTS_DIGIT_AUTO:
		tts_digit_mode = ivTTS_READDIGIT_AUTO;
		break;			
    case TTS_DIGIT_NUMBER:
		tts_digit_mode = ivTTS_READDIGIT_AS_NUMBER;
		break;
    case TTS_DIGIT_VALUE:
		tts_digit_mode = ivTTS_READDIGIT_AS_VALUE;
		break;

	default:
		SCI_TRACE_LOW("[TTS] GetDitigMode digit_mode = %d, tts_digit_mode = %d",digit_mode,tts_digit_mode);
		break;
	}
	return tts_digit_mode;
}

LOCAL void TTS_SynthText(TxtParam* txtPar,char*HeapBuffer,uint32 HEAP_BUFFER_SIZE,void *ResData)
{
    ivHTTS          hTTS;
    ivPByte         pHeap;
    ivTResPackDesc  tResPackDesc;
    TCBParam        tcbparm;
    ivTTSErrID id;
    BOOLEAN is_usc2=txtPar->is_unicode;
    int jump_value = 0;
	uint16    digit_mode = GetDitigMode(s_tts_text.digit_mode);
	uint16    punc_mode = GetPuncMode(s_tts_text.punc_mode);
    
    //SCI_TRACE_LOW:"MMITTS: TTS_SynthText txtPar= %u, HeapBuffer=%d ,HEAP_BUFFER_SIZE=%d,ResData=%u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1141_112_2_18_3_3_46_206,(uint8*)"uddu",txtPar,HeapBuffer,HEAP_BUFFER_SIZE,ResData);

    tcbparm.pParam1st =(ivPointer)txtPar;
    tcbparm.pParam2nd = NULL;
    tcbparm.pParam3rd = NULL;
        
    /* 分配堆 */
    pHeap=(ivPByte)HeapBuffer;
    memset(pHeap, 0, HEAP_BUFFER_SIZE);
        
    /* 初始化资源 */
    tResPackDesc.pCBParam =(ivPointer)ResData;
    tResPackDesc.pfnRead = ReadResCB;
    tResPackDesc.pfnMap = NULL;
    tResPackDesc.nSize = 0;
        
    if (tResPackDesc.pCBParam)
    {
#ifdef TTS_FILE_RES
        tResPackDesc.pCacheBlockIndex = SCI_ALLOC(ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT);
        tResPackDesc.pCacheBuffer = tts_resDataCathe;    // SCI_ALLOC((ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT)*(ivTTS_CACHE_SIZE));
        tResPackDesc.nCacheBlockSize = ivTTS_CACHE_SIZE;
        tResPackDesc.nCacheBlockCount = ivTTS_CACHE_COUNT;
        tResPackDesc.nCacheBlockExt = ivTTS_CACHE_EXT;
#else
        tResPackDesc.pCacheBlockIndex =NULL;// SCI_ALLOC(ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT);
        tResPackDesc.pCacheBuffer =NULL;// tts_resDataCathe;    // SCI_ALLOC((ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT)*(ivTTS_CACHE_SIZE));
        tResPackDesc.nCacheBlockSize = ivTTS_CACHE_SIZE;
        tResPackDesc.nCacheBlockCount = ivTTS_CACHE_COUNT;
        tResPackDesc.nCacheBlockExt = ivTTS_CACHE_EXT;
#endif
    }
        
    /* 创建 TTS 实例 */
    id=ivTTS_Create(&hTTS, (ivPointer)pHeap, HEAP_BUFFER_SIZE, (ivPointer)&tcbparm, (ivPResPackDesc)&tResPackDesc, (ivSize)1);
    // jun.hu 2011/2/16 for 增加实例创建成功与否的判断.....
    if(ivTTS_ERR_OK == id)
    {
        /* 设置流式输入回调 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_MODE, ivTTS_INPUT_CALLBACK);
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CALLBACK, (ivUInt32)InputCB);
        
        /* 设置音频输出回调 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)KiTTS_PcmStreamOutputCallback);
        
        /* 设置输入文本代码页 */
        if( is_usc2 )
        {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            id=ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_UTF16BE);
#else// jun.hu 2010.12.7 for 小端项目....
            id=ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_UTF16LE);
#endif
        }
        else
        {
            id=ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_ASCII);
        }
        
        /* 设置语种 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_LANGUAGE, ivTTS_LANGUAGE_CHINESE);      
        /* 设置文本标记参数 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_TEXT_MARK, ivTTS_TEXTMARK_SIMPLE_TAGS);
        /* 设置文本范围参数 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_TEXT_SCOPE, ivTTS_SCOPE_UNLIMITED);
        /* 设置发音风格 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_SPEAK_STYLE, ivTTS_STYLE_NORMAL);   
        /* 设置音量 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_VOLUME, ivTTS_VOLUME_NORMAL);           
        /* 设置音效 */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_VEMODE, ivTTS_VEMODE_NONE);
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_VOICE_SPEED, ivTTS_SPEED_NORMAL);
        //设置数字读法
		id=ivTTS_SetParam(hTTS, ivTTS_PARAM_READ_DIGIT, digit_mode);
		//设置符号读法
		id=ivTTS_SetParam(hTTS, ivTTS_PARAM_READ_PUNCTUATIONS, punc_mode);
        /************************************************************************
        开启流式合成
        ************************************************************************/
        /* 设置发音人为 YANPING */
        id=ivTTS_SetParam(hTTS, ivTTS_PARAM_ROLE, ivTTS_ROLE_YANPING);
        jump_value = setjmp(s_jumpbuf_tts);
        if(jump_value == 0)
        {
            //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SynthText ivTTS_SynthStart"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1222_112_2_18_3_3_46_207,(uint8*)"");
            id=ivTTS_SynthStart(hTTS);
        
            //if (s_kitts_callback_count<TTS_CALLBACK_CHECKPOINT && MMITTS_user_stop==FALSE && s_tts_is_running &&TTS_IsCreatePcmStreamHandle())
            if (s_kitts_callback_count<TTS_CALLBACK_CHECKPOINT && MMITTS_user_stop==FALSE && s_tts_is_running && TTSAPI_GetAudioIsOpen())
            {
                s_tts_pcm_stream_count = TTS_GetPcmStreamId();
                MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_AUDIO_PLAY, &s_tts_pcm_stream_count, sizeof (s_tts_pcm_stream_count));
                MMI_TriggerMMITask();//加快反应
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SynthText jump_value=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1235_112_2_18_3_3_46_208,(uint8*)"d",jump_value);
        }
    }
    // jun.hu 2011/2/16 增加实例创建失败，对TTS资源文件的判断....
    else if(ivTTS_ERR_RESOURCE == id)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_ERROR_RESOURCE, PNULL ,0);
        MMI_TriggerMMITask();//加快反应, PNULL ,0);
    }
    /* 逆初始化 */
    id=ivTTS_Destroy(hTTS);

    if (tResPackDesc.pCacheBlockIndex)
        SCI_FREE(tResPackDesc.pCacheBlockIndex);
}
/*****************************************************************************/
//  Description : handle the timer msg 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TTS_HandleTTSPlayOverTimerMsg(
                                          uint8 timer_id,
                                          uint32   param
                                          )
{    
    //SCI_TRACE_LOW:"MMITTS, HandleTTSPlayOverTimerMsg entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1259_112_2_18_3_3_46_209,(uint8*)"");
    if (s_tts_playover_timer_id == *(uint8*)param)
    {
        //SCI_TRACE_LOW:"TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1262_112_2_18_3_3_46_210,(uint8*)"");
        
        s_tts_playover_timer_id = 0;
        
        if (s_tts_is_running)
        {
            s_tts_is_running = FALSE;
            TTSAPI_NotifyTTSOver();
            if (TTSAPI_GetAudioIsOpen())
            {
                TTS_StopAndClosePcmStreamHandle();
            }
        }
        
        TTS_MemoryRelease();
        TTS_RestoreClk();
    }
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_NotifyTTSOver(void)
{
    if(0 != s_tts_text.win_handle)
    {
        MMK_PostMsg(s_tts_text.win_handle, MSG_TTS_OVER, PNULL,0 );
    }
}


PUBLIC BOOLEAN TTSAPI_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr)
{
    MMITTS_RESULT_E tts_result = MMITTS_PLAY_SUCCESS;

	if (PNULL == play_param_ptr)
	{
		return FALSE;
	}
	
    //SCI_TRACE_LOW:"TTSAPI_PlayText() txt_len_in_byte= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1299_112_2_18_3_3_46_211,(uint8*)"d", play_param_ptr->txt_len_in_byte);
    //SCI_TRACE_LOW:"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1300_112_2_18_3_3_46_212,(uint8*)"dd",s_tts_text.tts_start_pos,s_tts_text.tts_str_len);
    
    //启动定时器来关闭资源
    if( s_tts_playover_timer_id != 0 )
    {
        MMK_StopTimer(s_tts_playover_timer_id);
        s_tts_playover_timer_id = 0;
    }
    if(!TTSPlayPreCheck(play_param_ptr->ring_type))
    {
        return FALSE;
    }
    if(play_param_ptr->txt_addr==PNULL || play_param_ptr->txt_len_in_byte==0)   //如果数据无效,直接返回
    {
        tts_result = MMITTS_INVALID_PARAM;
        return FALSE;
    }
    else if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        tts_result = MMITTS_NO_MEMORY;
        if (s_is_prompt_when_err)
        {
            MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        }
        return FALSE;
            
    }
   // 这一段去掉......
    /*else if (
#ifdef TTS_FILE_RES
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSRESCATHEBUFFER) || //block memory被占用
#endif
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSHEAPBUFFER) ||    
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSVOICEDATA) )
    {
        tts_result = MMITTS_NO_MEMORY;
        MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
        return FALSE;
    }*/
#ifdef TTS_FILE_RES
    else if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        if (s_is_prompt_when_err)    
        {
            tts_result = MMITTS_NO_FILE_RES;
            MMIPUB_OpenAlertWarningWin(TXT_TTS_UDISK_USING);
        }
        return FALSE;
    }
    else if(!TTSAPI_checkResFile())
    {
        tts_result = MMITTS_NO_FILE_RES;
        if (s_is_prompt_when_err)
        {
            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
        }
        return FALSE;
    }
#endif
    /*if use 600 res file, only chinese can read*/
#ifdef TTS_DATA_RES_LIMIT
    {
        int i=0;
        if (play_param_ptr->is_ucs2)
        {
            for (i=0;i<play_param_ptr->txt_len_in_byte ;i+=2)
            {
                if (txt_addr[i]!=0 ||(play_param_ptr->txt_addr[i+1]<=0x39 && play_param_ptr->txt_addr[i+1]>=0x30 ))
                    break;
            }
            if (i>=play_param_ptr->txt_len_in_byte)
                return FALSE;
        }
        else
        {
            for (i=0;i<play_param_ptr->txt_len_in_byte;i++)
            {
                if (play_param_ptr->txt_addr[i]<=0x39 && play_param_ptr->txt_addr[i]>=0x30)
                    break;
            }
            if (i>=play_param_ptr->txt_len_in_byte)
                return FALSE;
        }
    }
#endif
    
    play_param_ptr->txt_len_in_byte = MIN( play_param_ptr->txt_len_in_byte,MMITTS_STRING_SIZE );
    SCI_MEMSET(&s_tts_text, 0, sizeof(MMITTS_STRING_T));

    s_tts_text.str_len = play_param_ptr->txt_len_in_byte;
    s_tts_text.is_ucs2 = play_param_ptr->is_ucs2;    
    MMI_MEMCPY( s_tts_text.str_arr, MMITTS_STRING_SIZE, 
                play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, 
                play_param_ptr->txt_len_in_byte);

    if (0==play_param_ptr->play_times)
    {
        while (MMITTS_STRING_SIZE-s_tts_text.str_len>play_param_ptr->txt_len_in_byte)
            {
                MMI_MEMCPY( s_tts_text.str_arr+s_tts_text.str_len, MMITTS_STRING_SIZE, 
                            play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, 
                            play_param_ptr->txt_len_in_byte);
                s_tts_text.str_len += play_param_ptr->txt_len_in_byte;
                s_tts_text.is_ucs2 = play_param_ptr->is_ucs2;    
            }
    }

    play_param_ptr->play_times = play_param_ptr->play_times>20 ? 20:play_param_ptr->play_times;
    
    s_tts_text.play_times       = play_param_ptr->play_times;//是否反复播放
    s_tts_text.ring_type        = play_param_ptr->ring_type;
    s_tts_text.tts_vol          = play_param_ptr->tts_vol;

    s_tts_text.tts_start_pos    = 0;  /*no use*/
    s_tts_text.tts_str_len      =s_tts_text.str_len;  //txt_len_in_byte;
    s_tts_text.win_handle = play_param_ptr->win_handle;
	s_tts_text.tts_route = play_param_ptr->tts_route;
	s_tts_text.digit_mode = play_param_ptr->digit_mode;
	s_tts_text.punc_mode = play_param_ptr->punc_mode;
	s_tts_text.volume_type = play_param_ptr->volume_type;

     
    //SCI_TRACE_LOW:"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1417_112_2_18_3_3_46_213,(uint8*)"dd",s_tts_text.tts_start_pos,s_tts_text.tts_str_len);
#ifdef TTS_FILE_RES
    {
//      char tempStr[MAX_SAVE_TCARD_FILE_LEN/2]={0};
//       sprintf (tempStr,"%s%s",SaveTcardDirStr,TTS_RES_FILE_NAME);    
//      memset(tts_res_file_name,0,sizeof(tts_res_file_name));
//      MMIAPICOM_Char2wchar((wchar*)tts_res_file_name,(char const *)tempStr);
    }
#endif
    return TTSAPI_SynthTextByTask();
}

PUBLIC BOOLEAN TTS_IsRepeatePlayText(void)
{
    if (s_tts_text.play_times==0)
        return TRUE;
    else
        return FALSE;
}

/*****************************************************************************/
//  Description : stop tts by type
//  Global resource dependence :
//  Author: aoke.hu
//  Note: 返回值 true:tts未播放或调用了停止播放;  false:tts仍旧在播放
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = TRUE;
    
    //SCI_TRACE_LOW:"TTSAPI_StopPlayTextByType ring_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1445_112_2_18_3_3_46_214,(uint8*)"d",ring_type);
    if(s_tts_is_running         
        ||TTSAPI_GetAudioIsOpen())
    {
        if(s_tts_text.ring_type == ring_type)
        {
            TTSAPI_StopPlayText();
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    
    return result;
}

PUBLIC void TTSAPI_StopPlayText()
{
    //SCI_TRACE_LOW:"TTSAPI_StopPlayText entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1465_112_2_18_3_3_46_215,(uint8*)"u",SCI_GetTickCount ());

    MMK_SendMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL );

    s_tts_text.str_len = 0;
    s_tts_text.tts_str_len = 0;
    
    //SCI_TRACE_LOW:"TTSAPI_StopPlayText exit time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1472_112_2_18_3_3_46_216,(uint8*)"u",SCI_GetTickCount ());
}

PUBLIC BOOLEAN TTSAPI_GetPlayStatus(void)
{
    return s_tts_is_running||TTSAPI_GetAudioIsOpen();
}

/*****************************************************************************/
//  Description : 判断是否在播放
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type)
{
    if(s_tts_is_running         &&
       TTSAPI_GetAudioIsOpen()  &&
       s_tts_text.ring_type == ring_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TTSPlayPreCheck(TTS_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = TRUE;

    //SCI_TRACE_LOW:"TTSAPI_StopPlayTextByType ring_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1506_112_2_18_3_3_46_217,(uint8*)"d",ring_type);
    if(s_tts_is_running         
        ||TTSAPI_GetAudioIsOpen())
    {
        result = FALSE;
        switch(ring_type) 
        {
        case TTS_RING_TYPE_CALL:
            TTSAPI_StopPlayText();
            result = TRUE;
        	break;
        
        case TTS_RING_TYPE_MENU:
            if(s_tts_text.ring_type == ring_type)
            {
                TTSAPI_StopPlayText();
                result = TRUE;
            }
        	break;
        default:
            if(TTS_RING_TYPE_CALL < s_tts_text.ring_type)
            {
                TTSAPI_StopPlayText();
                result = TRUE;
            }
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    )
{
     // 如果TTS正在播放....
    if(TTSAPI_GetPlayStatus())
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, STXT_STOP, rightsoft_id, is_need_update);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, TXT_SET_TTS_PLAY, rightsoft_id, is_need_update);
    }
}

#ifdef TTS_FILE_RES
/*****************************************************************************/
//  Description : create tts dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: for 把tts的文件夹放到系统文件夹隐藏起来....
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSHidenDir(void)
{
    BOOLEAN return_val = FALSE;
    //  创建目录...
    if(MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(), s_mmitts_reserved_dir, TTS_HIDEN_DIR_LEN, PNULL))
    {
        return_val = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD create dcd main dir fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1575_112_2_18_3_3_47_218,(uint8*)"");
    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        )
{
    BOOLEAN  return_val    = FALSE;

    if((PNULL != full_path_name_ptr) && (PNULL != full_path_len_ptr))
    {
        return_val = MMIAPIFMM_CombineSysPath(
                                            dev_type,
                                            s_mmitts_reserved_dir,
                                            TTS_HIDEN_DIR_LEN,
                                            s_mmitts_res_file_name,
                                            TTS_RES_FILE_NAME_LEN,
                                            full_path_name_ptr,
                                            full_path_len_ptr
                                            );

    }
    else
    {
        //SCI_TRACE_LOW:"TTS GetTTSResouceFileFullName error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1608_112_2_18_3_3_47_219,(uint8*)"");
    }

    return return_val;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSResouceFileHandle(
                                         MMIFILE_HANDLE* file_handle_ptr // [out]
                                        )
{
    wchar    res_file_full_path_name[MMIFILE_FILE_NAME_MAX_LEN + 1]  = {0};
    MMIFILE_HANDLE file_handle  = SFS_INVALID_HANDLE;
    uint16   path_len      = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  return_val    = FALSE;
    BOOLEAN  find_resut    = FALSE;
    //MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E   i = MMI_DEVICE_NUM -1;
    wchar*   dev_path_ptr = PNULL;
    uint16   dev_len = 0;
    if(PNULL == file_handle_ptr)
    {
        return FALSE;
    }
    
    // 先在C: 盘下面找...
    file_handle = MMIAPIFMM_CreateFile(s_mmitts_default_file_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
    if(SFS_INVALID_HANDLE != file_handle)
    {
        find_resut = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"TTS c: disk not exist resouce file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1643_112_2_18_3_3_47_220,(uint8*)"");
    }

    if(!find_resut)
    {
        for (i = MMI_DEVICE_NUM -1 ;i >= MMI_DEVICE_UDISK;i--)
        {
            dev_len = MMIAPIFMM_GetDevicePathLen(i);
            dev_path_ptr =  MMIAPIFMM_GetDevicePath(i);

            if(!MMIAPIFMM_GetDeviceStatus(dev_path_ptr, dev_len))
            {
                continue;
            }
            else
            {
                return_val = GetTTSResouceFileFullName(i, res_file_full_path_name, &path_len); 
                if(return_val)
                {
                    file_handle = MMIAPIFMM_CreateFile(res_file_full_path_name, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
                    if(SFS_INVALID_HANDLE != file_handle)
                    {
                        find_resut = TRUE;
                        break;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"TTS %d disk not exist resouce file"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1670_112_2_18_3_3_47_221,(uint8*)"d",i);
                    }
                }
            }
        }
    }

    *file_handle_ptr = file_handle;

    return find_resut;
}
/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_checkResFile(void)
{
    uint8    header_info[TTS_RES_FILE_CHECK_BYTE_NUM + 1]   =  {0};
    MMIFILE_HANDLE file_handle  = SFS_INVALID_HANDLE;
    uint32   readed_size   = 0;
    
    BOOLEAN  return_val    = FALSE;
    
    if(CreateTTSResouceFileHandle(&file_handle))
    {
        // 定位到头, 读出头信息....
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, header_info, TTS_RES_FILE_CHECK_BYTE_NUM, &readed_size, NULL))
            {
                if(0 == MMIAPICOM_Stricmp((uint8 *)TTS_RES_FILE_HEADER_STR , header_info))
                {
                    return_val = TRUE;
                    //SCI_TRACE_LOW:"TTS TTSAPI_checkResFile has passed "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1703_112_2_18_3_3_47_222,(uint8*)"");
                }
            }
        }
        
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"TTS TTSAPI_checkResFile not find the resouce file  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1712_112_2_18_3_3_47_223,(uint8*)"");
    }
    
    return return_val;
}

#endif

/*****************************************************************************/
//  Description : gui 调用此接口朗读菜单 
//  Global resource dependence : none
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_PlayTextForControl(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
									  MMISRVAUD_ROUTE_T tts_route
                                      )
{
#ifdef MMI_READMENU_ENABLE
    if(PNULL == str_ptr || 0 == num)
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1732_112_2_18_3_3_47_224,(uint8*)"");
        return;
    }
    if(!TTSPlayPreCheck(TTS_RING_TYPE_MENU))
    {
        return ;
    }
    
    // 0 为开关打开状态
    if (0 == MMIAPIENVSET_GetActiveModeOptValue(0, ENVSET_MENU_READ_SWITCH) &&
        !MMIAPIENG_GetIQModeStatus())
    {
        uint32 i = 0, len = 0;
        wchar* total_str_ptr = NULL;
        uint8 vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_READMENU);
		MMITTS_PLAY_PARAM_T  play_param = {0};
        
        // 根据需求，开关此功能
#if 1
        // 考虑到隐私问题，不自动读出收件箱，发件箱，草稿箱，已发信箱在list中部分内容
        MMI_WIN_ID_T focus_win_id = MMK_GetFocusWinId();
        if (MMISMS_MSGBOX_MAIN_WIN_ID == focus_win_id)
        {
            if (3 == num) num = 2;
        }
#endif
        
        // 不知道所有字符串的总长，需要累加得出
        for (; i < num; i ++)
        {
            len += str_ptr[i].wstr_len + 1;
        }
        total_str_ptr = (wchar*)SCI_ALLOC((len + 1) * 2);
        SCI_MEMSET(total_str_ptr, 0, (len + 1) * 2);
        
        // 将所有字符串复制到一个buffer中，便于朗读
        for (i = 0, len = 0; i < num; i ++)
        {
            if (str_ptr[i].wstr_len > 0)
            {
                // 复制
                SCI_MEMCPY(total_str_ptr + len, str_ptr[i].wstr_ptr, str_ptr[i].wstr_len * 2);
                len += str_ptr[i].wstr_len;
                // 在字符串与字符串之间加空格符，这样可以产生停顿效果
                *(total_str_ptr + len) = 0x20;
                len ++;
            }
        }
        
        // 此时不应该弹出出错提示（比如缺少TTS资源文件等），因为菜单朗读太频繁，提示会给用户带来困扰
        s_is_prompt_when_err = FALSE;
		play_param.txt_addr = total_str_ptr;
		play_param.txt_len_in_byte = len * 2;
		play_param.is_ucs2 = TRUE;
		play_param.play_times = 1;
		play_param.tts_vol = vol;
		play_param.ring_type = TTS_RING_TYPE_MENU;
		play_param.win_handle = 0;
		play_param.tts_route = tts_route;
		play_param.digit_mode = TTS_DIGIT_NUMBER;
		play_param.punc_mode = TTS_PUNC_OFF;
		play_param.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;
        TTSAPI_PlayText(&play_param);
        // 恢复弹出出错提示标志量
        s_is_prompt_when_err = TRUE;
        
        SCI_FREE(total_str_ptr);
    }

#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_Init(void)
{

#ifdef TTS_FILE_RES
    CreateTTSHidenDir();
#endif

}

/*****************************************************************************/
//  Description : 设置TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRunStatus(BOOLEAN tts_is_run)
{
    s_tts_is_running = tts_is_run;
}

/*****************************************************************************/
//  Description :  获取TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetTTSRunStatus(void)
{
    return  s_tts_is_running;   
}

/*****************************************************************************/
//  Description :  获取TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void)
{
    return  s_tts_text.ring_type;   
}

/*****************************************************************************/
//  Description :  获取TTS volume type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_VOLUME_TYPE_E TTS_GetTTSVolumeType(void)
{
    return s_tts_text.volume_type;   
}

/*****************************************************************************/
//  Description :  获取TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolume(void)
{
    return s_tts_text.tts_vol;   
}

/*****************************************************************************/
//  Description :  设置TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSVolume(uint16 set_vol)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
    uint8           cur_vol      = TTS_GetTTSVolume();
    
    if((audio_handle > 0) && (cur_vol != set_vol))
    {
        MMISRVAUD_SetVolume(audio_handle, set_vol);
        s_tts_text.tts_vol = set_vol;
    }
}


#ifdef WIN32
/* get SDK version */
ivTTSErrID ivCall ivTTS_GetVersion(
    ivPUInt8        piMajor,            /* [out] major version number */
    ivPUInt8        piMinor,            /* [out] minor version number */
    ivPUInt16       piRevision)         /* [out] revision number */
{
    return 0;
}

/* create an instance */
ivTTSErrID ivCall ivTTS_Create(
    ivHTTS ivPtr    phTTS,              /* [out] handle to an instance */
    ivPointer       pHeap,              /* [in] heap for instance */
    ivSize          nHeapSize,          /* [in] size of the heap */
    ivPointer       pCBParam,           /* [in] user callback parameter */
    ivPResPackDesc  pResPackDesc,       /* [in] resource pack description array */
    ivSize          nResPackCount)      /* [in] resource pack count */
{
    return 0;
}

/* get a parameter associated with an instance */
ivTTSErrID ivCall ivTTS_GetParam(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivUInt32        nParamID,           /* [in] parameter ID */
    ivPUInt32       pnParamValue )      /* [out] buffer to receive the parameter value */
{
    return 0;
}

/* set a parameter associated with an instance */
ivTTSErrID ivCall ivTTS_SetParam(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivUInt32        nParamID,           /* [in] parameter ID */
    ivUInt32        nParamValue )       /* [in] parameter value */
{
    return 0;
}

/* get pcm data */
ivTTSErrID ivCall ivTTS_GetData(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivPointer       pData,              /* [in] pointer of pcm data buffer */
    ivSize ivPtr    pSize )             /* [in/out] data size */
{
    return 0;
}

/* exit running of an instance and leave current thread's control */
ivTTSErrID ivCall ivTTS_Exit(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* synthesize a buffer of text on an instance */
ivTTSErrID ivCall ivTTS_SynthText(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivCPointer      pcData,             /* [in] pointer of text buffer data to be synthesized */
    ivSize          nSize )             /* [in] size of text buffer data to be synthesized */
{
    return 0;
}

/* destroy an instance */
ivTTSErrID ivCall ivTTS_Destroy(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* run an instance and hold current thread's control */
ivTTSErrID ivCall ivTTS_Run(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* begin to synthesize from callback on an instance */
ivTTSErrID ivCall ivTTS_SynthStart(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
