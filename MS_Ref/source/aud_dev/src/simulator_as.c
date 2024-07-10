/******************************************************************************
 ** File Name:      armvb_as.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Voiceband(ARM Controlled) audioservice adapter layer.* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
 /*lint -save -e661 -e718*/
#include "os_api.h"


#include "simulator_as.h"
#include "adm.h"
#include "aud_gen.h"


#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
#include "admm_II.h"
#endif


#include "windows.h"
#include <windowsx.h>
#include "mmsystem.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#undef __MODULE__ 
#define __MODULE__ "win32_dev.c"


AUDIO_DEVICE_HANDLE_T* g_win32_dev_b_device_ptr;
#define WIN32_DEV_FIFO_SIZE        32


LOCAL  AUDIO_OUT_OPEN          s_audio_out_open;
LOCAL AUDIO_OUT_PAUSE          s_audio_out_pause;
LOCAL AUDIO_OUT_RESUME        s_audio_out_resume;
LOCAL AUDIO_OUT_STOP         s_audio_out_stop;




#define MAX_BUFFER_LENGTH    (8*1024)

LOCAL     HWAVEOUT     m_hWaveOut;
LOCAL     int32 m_lpData[MAX_BUFFER_LENGTH];
LOCAL WAVEHDR   m_WaveHdr_1={0};
LOCAL LPWAVEHDR   m_lpWaveHdr=(LPWAVEHDR)&m_WaveHdr_1;
LOCAL     int32 m_lpData_2[MAX_BUFFER_LENGTH];
LOCAL WAVEHDR   m_WaveHdr_2={0};
LOCAL LPWAVEHDR   m_lpWaveHdr_2=(LPWAVEHDR)&m_WaveHdr_2;

LOCAL     HANDLE  h_wavout_thread;
 HANDLE h_wavout_event;
LOCAL HANDLE h_wavout_thread_killed_event;

LOCAL uint32 s_device_stop=FALSE;




extern void Init_Dsp(void );


void   CALLBACK   waveOutProc(HWAVEOUT   hWaveOut,UINT   uMsg,DWORD   dwInstance, 
                      DWORD   dwParam1,DWORD   dwParam2) 
{ 
        switch(uMsg)
            {
            case   MM_WOM_OPEN:
                    
                break;
            case   MM_WOM_DONE: 
                        if(h_wavout_event)
                        {
                            SetEvent( h_wavout_event );
                        }
                break   ;
            case   MM_WOM_CLOSE:
                break;
            default:
            break;
            }

        
        
} 




DWORD WINAPI  wavout_thread_func(LPVOID p)
{
        LPSTR m_lpData=NULL;
        LPSTR m_lpData2=NULL;
        int32 pcm_length=0;
        
        while(!s_device_stop)
        {
            int32 is_played=FALSE;
            
            if(h_wavout_event)
            {
                WaitForSingleObject( h_wavout_event, INFINITE );
                
                if(m_lpWaveHdr->dwFlags & WHDR_DONE)
                {
                m_lpData=m_lpWaveHdr->lpData;
                _tx_thread_context_save();
                pcm_length=AudioOut_GetPcmData(m_lpData,MAX_BUFFER_LENGTH/4);
                _tx_thread_context_restore();
                //SCI_TRACE_LOW("peter:pcm_length is %d",pcm_length);
                if(pcm_length)
                {
                        is_played=TRUE;
                    m_lpWaveHdr->dwBufferLength =pcm_length*4;
                    m_lpWaveHdr->dwFlags = 0L;
                    m_lpWaveHdr->dwLoops = 0L;
#if _MSC_VER < 1300
                    waveOutPrepareHeader(m_hWaveOut,(PWAVEHDR)m_lpWaveHdr,sizeof(WAVEHDR)); 
                    waveOutWrite(m_hWaveOut,(PWAVEHDR)m_lpWaveHdr,sizeof(WAVEHDR)); 
#endif
                }
                
            }

            if(m_lpWaveHdr_2->dwFlags & WHDR_DONE)
            {
                    m_lpData=m_lpWaveHdr_2->lpData;
                _tx_thread_context_save();
                pcm_length=AudioOut_GetPcmData(m_lpData,MAX_BUFFER_LENGTH/4);
                _tx_thread_context_restore();
                //SCI_TRACE_LOW("peter:pcm_length is %d",pcm_length);
                if(pcm_length)
                {
                       is_played=TRUE;
                    m_lpWaveHdr_2->dwBufferLength =pcm_length*4;
                    m_lpWaveHdr_2->dwFlags = 0L;
                    m_lpWaveHdr_2->dwLoops = 0L;
#if _MSC_VER < 1300        			
                    waveOutPrepareHeader(m_hWaveOut,(PWAVEHDR)m_lpWaveHdr_2,sizeof(WAVEHDR)); 
                    waveOutWrite(m_hWaveOut,(PWAVEHDR)m_lpWaveHdr_2,sizeof(WAVEHDR));                     
#endif
                }
            }
            
            }
        }


}

int32 WavOut_Open(uint32 samplerate)
{
    int32 result=0;
    uint32 os_thread_id;
    WAVEFORMATEX wavFormatinfo;
    wavFormatinfo.wFormatTag=WAVE_FORMAT_PCM;
    wavFormatinfo.nChannels=2;
    wavFormatinfo.nSamplesPerSec=samplerate;
    wavFormatinfo.wBitsPerSample=16;
    wavFormatinfo.nBlockAlign=wavFormatinfo.wBitsPerSample*wavFormatinfo.nChannels/8;
    wavFormatinfo.nAvgBytesPerSec=samplerate*wavFormatinfo.nBlockAlign;
    wavFormatinfo.cbSize=0;


    s_device_stop=FALSE;
#if _MSC_VER < 1300
    if (result=waveOutOpen(&m_hWaveOut, WAVE_MAPPER,(WAVEFORMATEX *)&wavFormatinfo, 0, 0L,
               WAVE_FORMAT_QUERY))
#endif
       {
            // 波形音频输出设备不支持该格式
            return FALSE;
       }
#if _MSC_VER < 1300	
       if(waveOutOpen(&m_hWaveOut, WAVE_MAPPER,
              (WAVEFORMATEX *)&wavFormatinfo, (UINT)waveOutProc,0, CALLBACK_FUNCTION))
#endif
           {
            return FALSE;

           }

    // 设置WAVEHDR结构，并准备其数据
    m_lpWaveHdr->lpData = m_lpData;
    m_lpWaveHdr->dwBufferLength = 0;
    m_lpWaveHdr->dwFlags = WHDR_DONE;
    m_lpWaveHdr->dwLoops = 0L;

    // 设置WAVEHDR结构，并准备其数据
    m_lpWaveHdr_2->lpData = m_lpData_2;
    m_lpWaveHdr_2->dwBufferLength = 0;
    m_lpWaveHdr_2->dwFlags = WHDR_DONE;
    m_lpWaveHdr_2->dwLoops = 0L;

    if(!h_wavout_event)
    {
        h_wavout_event=CreateEvent( NULL, FALSE, FALSE, NULL );
    }

    if(h_wavout_event)
    {
        SetEvent( h_wavout_event);

    }
    else
    {
#if _MSC_VER < 1300
        waveOutClose(m_hWaveOut);
#endif
        return FALSE;     
    }

    

      if(!h_wavout_thread)
      {
            h_wavout_thread=CreateThread(NULL,
                                        10*1024*1024,
                                        wavout_thread_func,
                                        0,
                                        0,
                                        &os_thread_id);
    }
    if(!h_wavout_thread)
    {
#if _MSC_VER < 1300
        waveOutClose(m_hWaveOut);
#endif
         if(h_wavout_event)
        {
            CloseHandle( h_wavout_event);
        }
        return FALSE;     
    }
    else
    {
        SetThreadPriority(h_wavout_thread,THREAD_PRIORITY_HIGHEST);
    }
    
    return TRUE;
}


int32 WavOut_Close(void)
{    
    MMRESULT result=MMSYSERR_NOERROR;
    uint32 count=0;
    s_device_stop=TRUE;

    if(h_wavout_thread)
    {
        CloseHandle( h_wavout_thread);
        h_wavout_thread=NULL;
    }

    if(h_wavout_event)
    {
            CloseHandle( h_wavout_event);
            h_wavout_event=NULL;
    }
#if _MSC_VER < 1300	
    do
    {
        result=waveOutUnprepareHeader( m_hWaveOut, m_lpWaveHdr, 
                                sizeof(WAVEHDR));
        SCI_Sleep(20);
        count++;
    }while((result==WAVERR_STILLPLAYING)&&(count < 20));
#endif	
    count=0;
#if _MSC_VER < 1300
    do
    {
           count ++;
        result=waveOutUnprepareHeader( m_hWaveOut, m_lpWaveHdr_2, 
                                sizeof(WAVEHDR));
        SCI_Sleep(20);
    }while((result==WAVERR_STILLPLAYING)&&(count< 20));
#endif
       if(m_hWaveOut)
    {
#if _MSC_VER < 1300
        waveOutClose(m_hWaveOut);
#endif
    }


    return 0;
}



PUBLIC void ARMVB_EnableAmplifier(BOOLEAN bEnable)
{
    return;
}

void AudioOut_Set_Open(AUDIO_OUT_OPEN audio_out_open)
{
    s_audio_out_open=audio_out_open;
}

void AudioOut_Set_Pause(AUDIO_OUT_PAUSE audio_out_pause)
{
    s_audio_out_pause=audio_out_pause;
}

void AudioOut_Set_Resume(AUDIO_OUT_RESUME audio_out_resume)
{
    s_audio_out_resume=audio_out_resume;
}


void AudioOut_Set_Stop(AUDIO_OUT_STOP audio_out_stop)
{
    s_audio_out_stop=audio_out_stop;
}



int32 AudioOut_GetPcmData(void * dest_buffer,uint32 read_len)
{
    int32 i=0;
    uint32 count=0;
    uint32 pcm_count=0;
    int16 *left_data_ptr=NULL;
    int16 *right_data_ptr=NULL;
    int16 *dest_ptr=(int16 *)dest_buffer;
    ADM_TRACK_T* track_ptr;
    ADM_DecOutputBuf_T *cur_out_buffer=NULL;

    if(g_win32_dev_b_device_ptr->uiTotalTrackon == 1)
    {
        track_ptr = g_win32_dev_b_device_ptr->ptTrackList;
        cur_out_buffer=&(g_adm_dec_output_buf[track_ptr->uiTrackNum]);
    }
    else
    {
        return 0;
    }
    if(!cur_out_buffer)
    {
        return 0;
    }

    SCI_DisableIRQ();

    
    if(cur_out_buffer->data_count>=read_len)
    {
        pcm_count=read_len;
    }
    else
    {
        pcm_count=cur_out_buffer->data_count;    
    }
    
    count=cur_out_buffer->buffer_size-cur_out_buffer->data_out_pos;


    left_data_ptr=cur_out_buffer->data_buffer_ptr+cur_out_buffer->data_out_pos;
    right_data_ptr=cur_out_buffer->data_buffer_ptr_r+cur_out_buffer->data_out_pos;

    if(count<pcm_count)
    {
        for(i=0;i<count;i++)
        {
            *(dest_ptr+2*i)=*(left_data_ptr+i);
            *(dest_ptr+2*i+1)=*(right_data_ptr+i);
        }
//        memcpy((void *)dest_ptr,(void *)left_data_ptr,count*2);                    
        left_data_ptr=cur_out_buffer->data_buffer_ptr;
        right_data_ptr=cur_out_buffer->data_buffer_ptr_r;
        dest_ptr+=count*2;
        for(i=0;i<(pcm_count-count);i++)
        {
            *(dest_ptr+2*i)=*(left_data_ptr+i);
            *(dest_ptr+2*i+1)=*(right_data_ptr+i);
        }
//        memcpy((void *)(dest_ptr+count),(void *)left_data_ptr,(pcm_count-count)*2);    
        
    }
    else
    {
        for(i=0;i<pcm_count;i++)
        {
            *(dest_ptr+2*i)=*(left_data_ptr+i);
            *(dest_ptr+2*i+1)=*(right_data_ptr+i);
        }
//        memcpy((void *)dest_ptr,(void *)left_data_ptr,pcm_count*2);    
    }
    cur_out_buffer->data_count-=pcm_count;
    cur_out_buffer->data_out_pos=(cur_out_buffer->data_out_pos+pcm_count)%cur_out_buffer->buffer_size;
    SCI_RestoreIRQ();
    return pcm_count;

}



LOCAL AUDIO_RESULT_E WIN32_DEV_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting )
{
    if(s_audio_out_open)
    {
        s_audio_out_open(uiSampleRate);
    }
    else
    {
        WavOut_Open(uiSampleRate);
    }

    
    return AUDIO_NO_ERROR;
}


LOCAL AUDIO_RESULT_E WIN32_DEV_Stop(DEV_SETTING_T *ptDevSetting )
{

    if(s_audio_out_stop)
    {
        s_audio_out_stop();
    }
    else
    {
        WavOut_Close();
    }
    return AUDIO_NO_ERROR;
}

LOCAL AUDIO_RESULT_E WIN32_DEV_Pause(DEV_SETTING_T *ptDevSetting )
{

    if(s_audio_out_pause)
    {
        s_audio_out_pause();
    }
    else
    {
        WavOut_Close();
    }

    return AUDIO_NO_ERROR;

}

LOCAL AUDIO_RESULT_E WIN32_DEV_Resume(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting )
{

    if(s_audio_out_resume)
    {
        s_audio_out_resume(uiSampleRate);
    }
    else
    {
        WavOut_Open(uiSampleRate);
    }
    return AUDIO_NO_ERROR;
}

LOCAL void WIN32_DEV_SetVolume(uint32  uiVol)
{
    return;
}

LOCAL void WIN32_DEV_SetDevMode(AUDIO_DEVICE_MODE_TYPE_E eMode)
{


}

LOCAL int32  WIN32_DEV_GetAppropriateOutputSamplerate(int32 uSampleRate)
{
    return uSampleRate;
}

LOCAL void WIN32_DEV_SetDownlinkMute(
    BOOLEAN is_mute   
    )
    {
    return;

    }

PUBLIC HAUDIODEV WIN32_DEV_RegAudioDevice(void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
    
    SCI_MEM16CPY((uint16*)tDeviceInfo.usDeviceName, ANSI2UINT16("ARMVB"), strlen("ARMVB") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = WIN32_DEV_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = WIN32_DEV_Play;
    tDeviceInfo.tAudioDevOpe.pClose = WIN32_DEV_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = WIN32_DEV_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = WIN32_DEV_Resume;


    tDeviceInfo.tAudioDevOpe.pSetVolume = WIN32_DEV_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = WIN32_DEV_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;  
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = WIN32_DEV_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = WIN32_DEV_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice(&tDeviceInfo);
    g_win32_dev_b_device_ptr = AUDIO_DM_GetDeviceHandleStruct(hDevice);   // Record device structure

#if defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530) || defined (PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    Init_Dsp();
#endif

    return hDevice;
}




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of asm_hc.c
