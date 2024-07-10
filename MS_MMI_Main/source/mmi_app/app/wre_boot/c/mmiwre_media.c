#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
/******************************************************************************
 ** File Name:      adaptor_media.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           30/12/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps media-layer for mocor os			      *
 **																              *
 ******************************************************************************/
#include "mmiwre_media.h"
#include "math.h"
#include "mmiaudio.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaudio_ctrl.h"
#include "Mmk_type.h"
#include "mmk_app.h"
#include "wav_adp.h"
#include "mp3_adp.h"
#include "mmi_custom_define.h"
#include "mmiwre_export.h"
#include "mmibt_export.h"
#include "sig_code.h"
//added by leichi 20120423 begin
#include "Mn_type.h"
#include "Mmiwre_nv.h"
#include "mmisrvaud_util.h"	// changed by yifei
uint32 WRE_MEDIA_CURRENT_VOLUME = 5;
extern  BOOLEAN MMIWRE_IsWREVideoPlayerActive(void);//defined by leichi for sidekey function

//added by leichi 20120423 end

/////////////////////////////////////////////////////////////////////
//媒体数据方式
struct _MediaBuff
{
   union _header
   {
      MP3_STREAM_BUFFER_T  hdMP3; //mp3
      WAV_STREAM_BUFFER_T  hdWAV; //wav pcm
   }unHeader;
   uint8* pDataOrFn;//filepath or data buff,if stream ,point header addr
   uint32 dwDataLen; //媒体数据长度，如果控制类型是文件，那么该值将不做参考，否则将会作为数据长度

};
//媒体输入输出设备结构
typedef struct _HMEDIAINOUT
{
   //基本数据定义
   BOOLEAN             isMute; //静音使用类型，TRUE表示静音，FALSE表示非静音
   BOOLEAN             isPlay; //设备使用类型，TRUE表示用来播放，FALSE表示用来录制
   struct _MediaBuff   mdb;//媒体数据方式
   eMediaType          eType; //媒体控制类型
   uint32		        dwFormat; //媒体处理的格式
   void	            (*endMedia)(unsigned int wParam,unsigned int lParam,unsigned int unReserved); //媒体播放需要进行消息交互的callback，如果为空，将不进行交互
   eMMState            eState;
   int32               curTime;//record time for start
   uint32              curVol;
   uint32              unReserved;
   //流数据获取
   int	                (*doStreaming)(unsigned int unHWho,unsigned char** ppucDataAddr,unsigned int* puiDataLength); //媒体播放需要进行数据获取
   uint32              unReserved0;  //父句柄，音频合成用
   //文件句柄
   //uint32   handle;   //当前音频句柄//del by leichi 20120331
   MMISRV_HANDLE_T   handle; //added by leichi 20120331
   MMIFILE_HANDLE    hfile;

} HMEDIAINOUT,*LPHMEDIAINOUT;
/////////////////////////////////////////////////////////////////////////
#define SEMITONE_CONST      17.31234049066755       
#define WRE_FREQ(note)      (exp(note/SEMITONE_CONST)*8.176)
#define WREMEDIA_TIME_UNKNOWN			  0xFFFFFFFF
#define WRE_PLATFORM_VOLUME_MAX            MMISET_VOL_SIX
#define WRE_PLATFORM_VOLUME_DEFAULT        MMISET_VOL_THREE
/* WRE_DECODE_OUTPUT_BUF_SIZE should be the same as DECODE_OUTPUT_BUF_SIZE */
#define WRE_DECODE_OUTPUT_BUF_SIZE  (160 * 150)
LOCAL  int32     s_wre_platform_Volume = WRE_PLATFORM_VOLUME_DEFAULT;//(s_wre_platform_Volume is from 0-6)
const  uint8     s_actualVolumeMappingTable[WRE_PLATFORM_VOLUME_MAX][WRE_PLATFORM_VOLUME_MAX+1] = {
	/*x-coordinate is master volume 0 - 6*/
	/*y-coordinate is wre volume level 0 - 5*/
	/*map value is actualVolume  0 - 7, 7 is MMI_AUDIO_VOLUME_MAX*/
	{0, 0, 0, 0, 0, 0, 0},
	{0, 1, 2, 3, 4, 5, 6},
	{0, 2, 3, 4, 5, 6, 7},
	{0, 3, 4, 5, 6, 7, 7},
	{0, 4, 5, 6, 7, 7, 7},
	{0, 5, 6, 7, 7, 7, 7}
};
//通道号:1单声道/2双声道/2立体声
#define CHANNELNUM_MAX 4
const uint32 s_channelnum[CHANNELNUM_MAX]={2,2,2,1};
#define DEF_VOLUME          50
/////////////////////////////////////////////////////////////////////////
#define AUDIOHMMDNUM_MAX 10
LOCAL uint32 s_repeat_hmmd_ptr[AUDIOHMMDNUM_MAX] = {0};
LOCAL int32 s_repeat_hmmd_num = 0;
void wre_free_array_audioptr(void);
LOCAL BOOLEAN wre_set_array_audioptr(HMEDIAINOUT *hmmd);
LOCAL HMEDIAINOUT *wre_get_array_audioptr(MMISRV_HANDLE_T handle);
LOCAL BOOLEAN wre_del_array_audioptr(HMEDIAINOUT *hmmd);
//////////////////////////////////////////////////////////////////////////
LOCAL HMEDIAINOUT* s_cur_hmmd=NULL;
//LOCAL HMEDIAINOUT* s_pending_hmmd=NULL;
LOCAL BOOLEAN      s_hasExtPaused=FALSE; //外部暂停,冲突控制
LOCAL uint32 * s_puiTrackBuffer = NULL;   //声音合成buffer
LOCAL int32 CalculateActualVolume(int32 mastervolume ,int32 playervolume);
LOCAL HAUDIO MMIWRE_CreateAudioFileHandle(
                    BOOLEAN is_a2dp,
                    MMISRVAUD_RING_FMT_E ring_fmt,
                    const wchar *name_ptr,
                    uint32* puitrackbuffer,
                    uint32 uiTrackBufferSize
                    );
BOOLEAN SetDeviceMode(HMEDIAINOUT *hmmd);
/////////////////////////////////////////////////////////////////////////
//modified by chenjianyun begin
void wre_free_array_audioptr(void)
{	
	int16 i=0;
	for (i=0;i<AUDIOHMMDNUM_MAX;i++)
	{
		s_repeat_hmmd_ptr[i] = 0;
	} 
	s_repeat_hmmd_num = 0;
}
LOCAL BOOLEAN wre_set_array_audioptr(HMEDIAINOUT *hmmd)
{
	if (NULL == hmmd)
	{
		return FALSE;
	}
	if(s_repeat_hmmd_num < AUDIOHMMDNUM_MAX)
	{
		s_repeat_hmmd_ptr[s_repeat_hmmd_num] = (uint32)hmmd;
		s_repeat_hmmd_num ++;
	}
	else
	{
		s_repeat_hmmd_num = 0;
		s_repeat_hmmd_ptr[s_repeat_hmmd_num] = (uint32)hmmd;
	}
	return TRUE;
}
LOCAL HMEDIAINOUT *wre_get_array_audioptr(MMISRV_HANDLE_T handle)
{
	int16 i=0;
	if(INVALID_HANDLE == handle)
	{
		SCI_TRACE_LOW("wre_get_array_audioptr handle=NULL");
		return NULL;
	}
	for (i=0;i<AUDIOHMMDNUM_MAX;i++)
	{
		if (s_repeat_hmmd_ptr[i] == 0)
		{
			continue;
		}
		if (((HMEDIAINOUT *)(s_repeat_hmmd_ptr[i]))->handle == handle)
		{
			return (HMEDIAINOUT *)(s_repeat_hmmd_ptr[i]);
		}
	}
	SCI_TRACE_LOW("wre_get_array_audioptr null handle=[%d]",handle);
	return NULL;
}
LOCAL BOOLEAN wre_del_array_audioptr(HMEDIAINOUT *hmmd)
{
	int16 i=0;
	if (NULL == hmmd)
	{
		return FALSE;
	}
	for (i=0;i<AUDIOHMMDNUM_MAX;i++)
	{
		if (s_repeat_hmmd_ptr[i] == 0)
		{
			continue;
		}
		if (s_repeat_hmmd_ptr[i] == (uint32)hmmd)
		{
			s_repeat_hmmd_ptr[i] = 0;
			return TRUE;
		}
	}
	return FALSE;
}
//modified by chenjianyun end
/////////////////////////////////////////////////////////////////////////

//音量转换
LOCAL int32 CalculateActualVolume(int32 mastervolume ,int32 playervolume)
{
    int32 playervolume_level = 0;
    uint8 actual_volume = 0;
    	
    if( mastervolume>6 || mastervolume<0 || playervolume>127 || playervolume<0 )
    {
       return 0;
    }
    	
    if(playervolume ==126 || playervolume==127)
    {
       playervolume_level = 5;
    }
    else
    {
       if(playervolume%25 == 0)
          playervolume_level = playervolume/25;
       else
          playervolume_level = playervolume/25+1;
    }
    	
    actual_volume = s_actualVolumeMappingTable[playervolume_level][mastervolume];
    return actual_volume;
	
}
/*
 *	把字符转换成小写字符
 */
#ifndef WIN32
LOCAL int towlower( unsigned short c )
{
   return (c>=L'A')&&(c<=L'Z')?c+L'a'-L'A':c;
}
#endif
/*
 *	比较两个宽字符串
 */
 /*
 *	两个字符串进行小写比较
 */
LOCAL int _wcsicmp (const unsigned short* cs, const unsigned short* ct)
{

    while (*cs != '\0' && *ct != '\0' && towlower(*cs) == towlower(*ct))
    {
       cs++;
       ct++;
    }
    return *cs - *ct;
}
//切换音频类型
LOCAL MMISRVAUD_RING_FMT_E switch_to_audiotype(const unsigned short* lpFormat)
{
    MMISRVAUD_RING_FMT_E  music_type = MMISRVAUD_RING_FMT_MAX;

    if(0 == _wcsicmp(MEDIA_FORMAT_MIDI,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_MIDI;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_TONE,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_MIDI;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_WAV,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_WAVE;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_MP3,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_MP3;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_AAC,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_AAC;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_M4A,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_AAC;
    }
    else if(0 == _wcsicmp(MEDIA_FORMAT_AMR,lpFormat))
    {
       music_type = MMISRVAUD_RING_FMT_AMR;
    }
    	
    return music_type;
}


//声音处理中,通常出现异常退出回调,姑且保护完成终止
#if 0
LOCAL void audio_end(MMIAUDIO_RESULT result, DPARAM param)
{
	HMEDIAINOUT *hmmd = s_cur_hmmd;
	if(hmmd->endMedia)
	{
		hmmd->endMedia(MEDIA_NOTIFY_FINISHED,(unsigned int)hmmd,hmmd->unReserved);
	}
}
#endif
//流媒体处理时的数据获取回调
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E audio_stream(		  HAUDIO hAudio, 
														  AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
														  void *pvOtherInfo)
{
    uint32 nRet=MEDIA_ERROR_UNKNOWNERROR;
    HMEDIAINOUT *hmmd = s_cur_hmmd;

    if(hmmd->doStreaming)
    {
       ptSrcDataInfo->tStreamCurSrcDataInfo.tOffsetTimeInfo.eOffsetType=AUDIO_SRCDATA_INFO_OFFSET_TIME;
       ptSrcDataInfo->tStreamCurSrcDataInfo.tOffsetTimeInfo.uOffsetTime.uiCurTime=0;
       nRet=(uint32)hmmd->doStreaming((unsigned int)hmmd, (uint8 **)&(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr), (unsigned int *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));
    }

    switch (nRet)
    {
       case MEDIA_ERROR_NOERROR:
            nRet= AUDIO_STREAM_GET_SRC_SUCCESS;
          break;
       case MEDIA_ERROR_GETSTREAM_WAIT:
            nRet= AUDIO_STREAM_GET_SRC_WAIT;
          break;
       case MEDIA_ERROR_GETSTREAM_ASY:
            nRet= AUDIO_STREAM_GET_SRC_ASY;
          break;
       case MEDIA_ERROR_UNKNOWNERROR:
       default:
            nRet= AUDIO_STREAM_GET_SRC_ERROR;
          break;
    }

    //SCI_TRACE_LOW:">>>audio_stream=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_229_112_2_18_3_10_45_67,(uint8*)"d",nRet);
    return nRet;
}


LOCAL BOOLEAN wre_player_notify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    MMISRVAUD_REPORT_RESULT_E result = 0;
	BOOLEAN bRet = FALSE;
    //HMEDIAINOUT *hmmd = s_cur_hmmd;
	HMEDIAINOUT *hmmd = wre_get_array_audioptr(handle); //modified by chenjianyun
    //SCI_TRACE_LOW:"wre_player_notify, handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_240_112_2_18_3_10_45_68,(uint8*)"d", handle);
    if(param != PNULL && handle > 0 && param->data != 0 && hmmd != NULL)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        result = (MMISRVAUD_REPORT_RESULT_E)report_ptr->data1;
		SCI_TRACE_LOW("wre_audio_20120810 1 hmmd->handle=[%d] handle=[%d]",hmmd->handle,handle);
        if(report_ptr != PNULL)
        {            
            switch(report_ptr->report)
            {
               case MMISRVAUD_REPORT_END:
                    if(hmmd->endMedia != NULL && (hmmd->handle == handle))//modified by chenjianyun
                   {
                      hmmd->endMedia(MEDIA_NOTIFY_FINISHED, (unsigned int)hmmd, hmmd->unReserved);
                   }
					bRet = wre_del_array_audioptr(hmmd);
					SCI_TRACE_LOW("wre_audio_20120810 del array audioptr ret = %d",bRet);
                   break;

               default:
                   break;
            }
        }
    }

    return TRUE;
}


MMISRV_HANDLE_T MediaRequest(HMEDIAINOUT *hmmd)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    req.is_auto_free = FALSE;
    req.notify = wre_player_notify;
    req.pri = MMISRVAUD_PRI_NORMAL;
    
    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    audio_srv.volume = CalculateActualVolume(s_wre_platform_Volume,hmmd->curVol);

    if(hmmd->isPlay)
    {
       //play
       if(hmmd->eType == MEDIA_TYPE_FILE)
       {
          audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
          audio_srv.info.ring_file.fmt = hmmd->dwFormat;
          audio_srv.info.ring_file.name = (wchar *)hmmd->mdb.pDataOrFn;
          audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen((wchar *)hmmd->mdb.pDataOrFn);
       }
       else if(hmmd->eType == MEDIA_TYPE_BUFFER)
       {
          audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
          audio_srv.info.ring_buf.fmt = hmmd->dwFormat; //MMISRVAUD_RING_FMT_MIDI;
          audio_srv.info.ring_buf.data= hmmd->mdb.pDataOrFn;
          audio_srv.info.ring_buf.data_len = hmmd->mdb.dwDataLen;
       }
       else if(hmmd->eType == MEDIA_TYPE_STREAM)
       {
          audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
          audio_srv.info.streaming.fmt = hmmd->dwFormat;
          audio_srv.info.streaming.data = hmmd->mdb.pDataOrFn;
          audio_srv.info.streaming.data_len = hmmd->mdb.dwDataLen;
          audio_srv.info.streaming.cb = audio_stream;	// changed by yifei
       }
       else
       {
          //SCI_TRACE_LOW:"MediaRequest, play, media type error "
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_310_112_2_18_3_10_45_69,(uint8*)"");
          return NULL;
       }
    }
    else
    {   
       //record
       req.notify = NULL;

       audio_srv.info.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
       audio_srv.info.record_file.fmt = hmmd->dwFormat;
       audio_srv.info.record_file.name = (wchar *)hmmd->mdb.pDataOrFn;
       audio_srv.info.record_file.name_len = MMIAPICOM_Wstrlen((wchar *)hmmd->mdb.pDataOrFn);
       audio_srv.info.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
       audio_srv.info.record_file.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
       if(MMIAPIFMM_IsFileExist((const wchar *)hmmd->mdb.pDataOrFn,hmmd->mdb.dwDataLen))
       {
          SFS_ERROR_E error;
          error = MMIAPIFMM_DeleteFile((const wchar *)hmmd->mdb.pDataOrFn, PNULL);
          //SCI_TRACE_LOW:"MediaRequest, record, error = %d"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_329_112_2_18_3_10_45_70,(uint8*)"d", error);
       }
       hmmd->hfile = MMIAPIFMM_CreateFile((const wchar *)hmmd->mdb.pDataOrFn, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL);
       audio_srv.info.record_file.file_handle = hmmd->hfile; 
       if(hmmd->hfile == 0)
       {
          //SCI_TRACE_LOW:"MediaRequest, record, create file error"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_335_112_2_18_3_10_45_71,(uint8*)"");
          return NULL;
       }
    }
   
    return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}


MMISRVAUD_RECORD_FMT_E switch_to_recordtype(const unsigned short* lpFormat)
{
   MMISRVAUD_RECORD_FMT_E music_type = MMISRVAUD_RECORD_FMT_MAX;

   if(0 == _wcsicmp(MEDIA_FORMAT_MP3,lpFormat))
   {
      music_type = MMISRVAUD_RECORD_FMT_MP3;
   }
   else if(0 == _wcsicmp(MEDIA_FORMAT_AMR,lpFormat))
   {
      music_type = MMISRVAUD_RECORD_FMT_AMR;
   }
   else
   {
      music_type = MMISRVAUD_RECORD_FMT_ADPCM;
   }
   return music_type;
}

/*********************************************************************
 *  s_mmPlay
 *
 * 关闭并析构一个媒体设备
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
LOCAL BOOL  s_mmPlay(HMEDIAINOUT *hmmd)
{
    BOOLEAN     result= FALSE;
	BOOLEAN	    bRet = FALSE;
    uint32      offset = 0;     
    MMISRVAUD_CONTENT_INFO_T format_info = {0};
    uint32 ret;

    //SCI_TRACE_LOW:"s_mmPlay, eState = %d, handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_382_112_2_18_3_10_45_72,(uint8*)"dd", hmmd->eState, hmmd->handle);
//added by leichi 20120401 begin
	if(INVALID_HANDLE == hmmd->handle)
	{
		//hmmd->handle = MediaRequest(hmmd);
		//SCI_TRACE_LOW("YYYYY s_mmPlay hmmd->handle = 0x%x", hmmd->handle);
		return FALSE;
	}//modified by chenjianyun
//added by leichi 20120401 end
	if (MEDIA_STATE_STOP != hmmd->eState && MEDIA_STATE_UNKNOWN != hmmd->eState)
	{
		MMISRVAUD_Stop(hmmd->handle);
	}
//modified by chenjianyun
    s_cur_hmmd = hmmd;
	bRet = wre_set_array_audioptr(hmmd);
	SCI_TRACE_LOW("playmedia set array audioptr ret = %d",bRet);
    if (hmmd->handle != INVALID_HANDLE)
    {    
    	 hmmd->curVol = 14*WRE_MEDIA_CURRENT_VOLUME;//modified by chejianyun
    	 MMISRVAUD_SetVolume(hmmd->handle, hmmd->curVol );//added by leichi 20120521

        if(hmmd->unReserved0 == 0)
        {
            if((hmmd->curTime > 0 ) && (TRUE== MMISRVAUD_GetContentInfo(hmmd->handle,&format_info)))
            {
                if (0 != format_info.total_time)
                {
                    offset = (uint32)(hmmd->curTime * format_info.total_data_length/ (format_info.total_time* 1000));
                }
            }

			ret = MMISRVAUD_Play(hmmd->handle, offset);


			if(ret != MMISRVAUD_RET_ERROR)
			{
				hmmd->eState = MEDIA_STATE_PLAYING;
				result = TRUE;
			}
			else
			{
				//MMISRVMGR_Free(hmmd->handle);
				//hmmd->handle = 0;
				hmmd->eState = MEDIA_STATE_UNKNOWN;
				result = FALSE;
				//modified by chenjianyun
			}            
		}
		else
		{
			SetDeviceMode(hmmd);
			ret = AUDIO_Play(hmmd->handle, offset);
			if(ret != AUDIO_NO_ERROR)
			{
				//SCI_TRACE_LOW:"s_mmPlay, AUDIO_Play, fail. ret = %d "
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_421_112_2_18_3_10_45_74,(uint8*)"d", ret);
				AUDIO_CloseHandle(hmmd->handle);
				hmmd->handle = 0;
				result = FALSE;
			}
			else
			{
				hmmd->eState = MEDIA_STATE_PLAYING;
				result = TRUE;
			}
		}
	}
	else
	{
		result = FALSE;
	}      

    return result;    
}
/*********************************************************************
 *  OpenMedia
 *
 * 构造并初始化一个媒体设备
 *
 * 参数
 * lpmmd
 *   [in] 指向一个MEDIAData 结构。在做为参数传递之前，必须按照合适的类型属性来填充这个结构。
 * 返回值
 *  如果成功，返回值是一个32位的HMEDIAHANDLE。
 *  如果失败，返回值是零。

 * 说明
 *   对于tone音,目前只有媒体类型有用。
 *
***********************************************************************/
HMEDIAHANDLE  OpenMedia(MEDIAData *lpmmd)
{
	return _OpenMediaEx(NULL,lpmmd);
}

HMEDIAHANDLE  _OpenMediaEx(HMEDIAHANDLE hParent, MEDIAData *lpmmd)
{
    HMEDIAINOUT *hmmd=NULL;

    if(NULL == lpmmd)
    {
       //SCI_TRACE_LOW:"OpenMedia lpmmd is NULL "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_461_112_2_18_3_10_45_75,(uint8*)"");
       return 0;
    }
    
    //SCI_TRACE_LOW:"OpenMedia, isPlay = %d, eType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_465_112_2_18_3_10_45_76,(uint8*)"dd", lpmmd->isPlay, lpmmd->eType);
    hmmd=(LPHMEDIAINOUT)SCI_ALLOC_APP(sizeof(HMEDIAINOUT));
    if(NULL == hmmd)
    {
       //SCI_TRACE_LOW:"OpenMedia hmmd is NULL "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_469_112_2_18_3_10_45_77,(uint8*)"");
       return 0;
    }
    memset(hmmd,0x00,sizeof(HMEDIAINOUT));
    hmmd->unReserved0 = hParent;
    if(lpmmd->isPlay)
    {
       //play
       hmmd->dwFormat = switch_to_audiotype(lpmmd->lpFormat);
       if(hmmd->dwFormat >= MMISRVAUD_RING_FMT_MAX)
       {
          SCI_FREE(hmmd);
          //SCI_TRACE_LOW:"OpenMedia play Format error, lpFormat = %d "
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_481_112_2_18_3_10_45_78,(uint8*)"d", lpmmd->lpFormat);
          return 0;
       }
      
       if(hmmd->unReserved0 != 0)
       {
           /*声音合成只支持wav 和mid */
           if(!(hmmd->dwFormat == MMISRVAUD_RING_FMT_WAVE || hmmd->dwFormat == MMISRVAUD_RING_FMT_MIDI))
           {
              SCI_FREE(hmmd);
              //SCI_TRACE_LOW:"OpenMedia unReserved0 != 0, lpFormat = %d "
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_491_112_2_18_3_10_45_79,(uint8*)"d", lpmmd->lpFormat);
              return 0;
           }
       }
    }
    else
    {
       //record 
		if(MEDIA_TYPE_BUFFER == lpmmd->eType)
		{
			if(NULL == lpmmd->lpData || lpmmd->dwDataLen == 0)
			{
				//no data buffer
				SCI_FREE(hmmd);
				//SCI_TRACE_LOW:"OpenMedia play buffer, lpData = NULL or dwDataLen = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_564_112_2_18_3_10_45_85,(uint8*)"d", lpmmd->dwDataLen);
				return 0;
			}
			hmmd->mdb.pDataOrFn=(uint8*)lpmmd->lpData;
			hmmd->mdb.dwDataLen = lpmmd->dwDataLen;
			goto DIRECT_ENTER;
		}

       hmmd->dwFormat = switch_to_recordtype(lpmmd->lpFormat);
       hmmd->mdb.dwDataLen = MMIAPICOM_Wstrlen(lpmmd->lpData);
       if(0 == hmmd->mdb.dwDataLen || hmmd->dwFormat >= MMISRVAUD_RECORD_FMT_MAX|| MEDIA_TYPE_STREAM== lpmmd->eType)
       {
          /* 暂不支持流录制, 暂不支持buffer录制*/
          SCI_FREE(hmmd);  /* no file or not support type */    /*lint !e831*/
          //SCI_TRACE_LOW:"OpenMedia record, DataLen = %d, Formate = %d, eType = %d"
		  if(hmmd) /*lint !e774 !e831*/
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_506_112_2_18_3_10_45_80,(uint8*)"ddd", hmmd->mdb.dwDataLen, hmmd->dwFormat, lpmmd->eType);
          return 0;
       }
       hmmd->mdb.pDataOrFn = (uint8*)SCI_ALLOC_APP((hmmd->mdb.dwDataLen+1)*sizeof(wchar));
       if(!hmmd->mdb.pDataOrFn)
       {
          SCI_FREE(hmmd);
          //SCI_TRACE_LOW:"OpenMedia record pDataOrFn malloc error "
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_513_112_2_18_3_10_45_81,(uint8*)"");
          return 0;
       }
       memset(hmmd->mdb.pDataOrFn,0x00,(hmmd->mdb.dwDataLen+1)*sizeof(wchar));
       MMIAPICOM_Wstrncpy((wchar*)hmmd->mdb.pDataOrFn, lpmmd->lpData,hmmd->mdb.dwDataLen);

       goto DIRECT_ENTER;
    }
    
    //类型检验
    if(MEDIA_TYPE_TONE == lpmmd->eType)
    {
       goto DIRECT_ENTER;
    }
    
    //检验文件
    if(MEDIA_TYPE_FILE == lpmmd->eType)
    {
        hmmd->mdb.dwDataLen = MMIAPICOM_Wstrlen(lpmmd->lpData);
        if(0 == hmmd->mdb.dwDataLen)
        {
            SCI_FREE(hmmd);
            //SCI_TRACE_LOW:"OpenMedia play file dwDataLen = 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_535_112_2_18_3_10_45_82,(uint8*)"");
            return 0;
        }
        //播放文件检测
        if(lpmmd->isPlay)
        {
            if(!MMIAPIFMM_IsFileExist((const wchar *)lpmmd->lpData,hmmd->mdb.dwDataLen))
            {
                SCI_FREE(hmmd);
                //SCI_TRACE_LOW:"OpenMedia play file is not exist "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_544_112_2_18_3_10_45_83,(uint8*)"");
                return 0;
            }
        }
        hmmd->mdb.pDataOrFn = (uint8*)SCI_ALLOC_APP((hmmd->mdb.dwDataLen+1)*sizeof(wchar));
        if(!hmmd->mdb.pDataOrFn)
        {
            SCI_FREE(hmmd);
            //SCI_TRACE_LOW:"OpenMedia play file pDataOrFn malloc error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_552_112_2_18_3_10_45_84,(uint8*)"");
            return 0;
        }
        memset(hmmd->mdb.pDataOrFn,0x00,(hmmd->mdb.dwDataLen+1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy((wchar*)hmmd->mdb.pDataOrFn,lpmmd->lpData,hmmd->mdb.dwDataLen);
    }
    else if((MEDIA_TYPE_BUFFER == lpmmd->eType)&&(TRUE == (lpmmd->isPlay)))//added by leichi 20120509
    {
        if(NULL == lpmmd->lpData || lpmmd->dwDataLen == 0)
        {
            //no data buffer
            SCI_FREE(hmmd);
            //SCI_TRACE_LOW:"OpenMedia play buffer, lpData = NULL or dwDataLen = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_564_112_2_18_3_10_45_85,(uint8*)"d", lpmmd->dwDataLen);
            return 0;
        }
        hmmd->mdb.pDataOrFn=(uint8*)lpmmd->lpData;
        hmmd->mdb.dwDataLen = lpmmd->dwDataLen;
    }
    else if(MEDIA_TYPE_STREAM == lpmmd->eType)
    {
        switch (hmmd->dwFormat)
        {
            case MMISRVAUD_RING_FMT_MP3:
               hmmd->mdb.unHeader.hdMP3.bType=SCI_TRUE;
               hmmd->mdb.unHeader.hdMP3.unMp3StreamData.tMp3FormatInfo.uiSampleRate=lpmmd->uiSampleRate;
               hmmd->mdb.unHeader.hdMP3.unMp3StreamData.tMp3FormatInfo.uiChannelNum=s_channelnum[lpmmd->uiChannelNum];
               hmmd->mdb.unHeader.hdMP3.unMp3StreamData.tMp3FormatInfo.pvOtherInfo=(void*)lpmmd->lpData;
               hmmd->mdb.dwDataLen=sizeof(MP3_STREAM_BUFFER_T);
               hmmd->mdb.pDataOrFn=(uint8*)&hmmd->mdb.unHeader.hdMP3;
               break;
            
            case MMISRVAUD_RING_FMT_WAVE:
               hmmd->mdb.unHeader.hdWAV.bType=SCI_TRUE;
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.uiSampleRate=lpmmd->uiSampleRate;
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.uiChannelNum=s_channelnum[lpmmd->uiChannelNum];
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.pvOtherInfo=(void*)lpmmd->lpData;
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.uiBitPerSample=lpmmd->uiBitPerSample;
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.eSubtype=WAV_PCM;
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.uiBlockAlign=(lpmmd->uiBitPerSample<=8)?1:(lpmmd->uiBitPerSample>>3);
               hmmd->mdb.unHeader.hdWAV.unWavStreamData.tWavFormatInfo.uiDataSize=0xFFFFFFFF;//sizeof(WAV_STREAM_BUFFER_T);
               hmmd->mdb.dwDataLen=sizeof(WAV_STREAM_BUFFER_T);
               hmmd->mdb.pDataOrFn=(uint8*)&hmmd->mdb.unHeader.hdWAV;
               break;
  
            default:
               {
                   SCI_FREE(hmmd);
                   //SCI_TRACE_LOW:"OpenMedia play stream format error "
                   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_599_112_2_18_3_10_45_86,(uint8*)"");
                   return 0;
               }
            
        }
    
    }
    else
    {
        SCI_FREE(hmmd);
        //SCI_TRACE_LOW:"OpenMedia play error, eType = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_609_112_2_18_3_10_45_87,(uint8*)"d", lpmmd->eType);
        return 0;
    }

DIRECT_ENTER:
    hmmd->isMute = FALSE;
    hmmd->eType = lpmmd->eType;
    hmmd->endMedia = lpmmd->hOwner;
    hmmd->isPlay = lpmmd->isPlay;
    hmmd->curVol = DEF_VOLUME;
    hmmd->curTime = (int32)WREMEDIA_TIME_UNKNOWN;
    hmmd->eState = MEDIA_STATE_UNKNOWN;
    hmmd->unReserved = lpmmd->dwReserved;
    hmmd->doStreaming = lpmmd->GetStreamData;
    SCI_TRACE_LOW("wre_audio openmediaEX before being set hmmd->curVol = %d",hmmd->curVol);
    if(MN_RETURN_SUCCESS == MMI_ReadNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME))
    {
	 	hmmd->curVol = WRE_MEDIA_CURRENT_VOLUME*14;
	 	SCI_TRACE_LOW("wre_audio openmediaEX  hmmd->curVol = %d | %d",hmmd->curVol,WRE_MEDIA_CURRENT_VOLUME);
    }
  
	if((MEDIA_TYPE_BUFFER == lpmmd->eType)&&(FALSE == lpmmd->isPlay))
	{
		hmmd->handle = MMISRVAUD_ReqVirtualHandle("WreBufferRecord", MMISRVAUD_PRI_NORMAL);
	    return (HMEDIAHANDLE)hmmd;

	}
	 if(MEDIA_TYPE_TONE != lpmmd->eType)
	{
		if(hmmd->unReserved0 != 0)
		{
			if(s_puiTrackBuffer == NULL)
			{
				s_puiTrackBuffer = SCI_ALLOC_APP(WRE_DECODE_OUTPUT_BUF_SIZE * sizeof(uint32));
			}

			if(s_puiTrackBuffer != NULL)
			{
				hmmd->handle = MMIWRE_CreateAudioFileHandle(FALSE, hmmd->dwFormat, (const wchar *)hmmd->mdb.pDataOrFn,   s_puiTrackBuffer, WRE_DECODE_OUTPUT_BUF_SIZE);
			}
		}
		else
		{
			hmmd->handle = MediaRequest(hmmd);
		}

		if(hmmd->handle == INVALID_HANDLE)
		{
		/* audio request fail */
			if(MEDIA_TYPE_FILE == lpmmd->eType)
			{
				if(hmmd->mdb.pDataOrFn != NULL)
				{
					SCI_FREE(hmmd->mdb.pDataOrFn);
					hmmd->mdb.pDataOrFn = NULL;
				}
			}
			SCI_FREE(hmmd);
			//SCI_TRACE_LOW:"OpenMedia request handle fail "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_657_112_2_18_3_10_45_88,(uint8*)"");
			return 0;
		}
	}
    
    return (HMEDIAHANDLE)hmmd;
}
/*********************************************************************
 *  CloseMedia
 *
 * 关闭并析构一个媒体设备
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  CloseMedia(HMEDIAHANDLE  hwho)
{
    HRESULT hres=MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    if(NULL == hmmd)
    {
       //SCI_TRACE_LOW:"CloseMedia hmmd is NULL "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_683_112_2_18_3_10_45_89,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    //SCI_TRACE_LOW:"CloseMedia, hmmd = 0x%x, hmmd->handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_686_112_2_18_3_10_45_90,(uint8*)"dd", hmmd, hmmd->handle);

    //playing or pause
    hres = StopMedia(hwho);
    if(s_cur_hmmd == hmmd)
    {
       s_cur_hmmd = NULL;
    }
	if(hmmd->handle != 0)
	{
		if((MEDIA_TYPE_BUFFER == hmmd->eType)&&(FALSE == hmmd->isPlay))
		{
			BOOLEAN res;
			res = MMISRVAUD_FreeVirtualHandle("WreBufferRecord");
			SCI_TRACE_LOW("CloseMedia buffer record close res = %d",res);
			hmmd->handle = 0;
		}
		else
		{
			MMISRVMGR_Free(hmmd->handle);
			hmmd->handle = 0;

		}
	}
    
    if(hmmd->eType == MEDIA_TYPE_FILE)
    {
          SCI_FREE(hmmd->mdb.pDataOrFn);
          hmmd->mdb.pDataOrFn = NULL;
    }
   
    if(hmmd->unReserved0 != 0)
    {
        if(s_puiTrackBuffer != NULL)
        {
            SCI_FREE(s_puiTrackBuffer);
            s_puiTrackBuffer = NULL;
        }
    }
    SCI_FREE(hmmd);

    return hres;
}

/*********************************************************************
 *  mmPlay
 *
 * 启动媒体播放
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  PlayMedia(HMEDIAHANDLE  hwho)
{
    HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
	
    if(NULL == hmmd)
    {
       //SCI_TRACE_LOW:"PlayMedia error, hmmd = NULL"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_744_112_2_18_3_10_46_91,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    
    //SCI_TRACE_LOW:">>>PlayMedia=%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_748_112_2_18_3_10_46_92,(uint8*)"ddd",hmmd->isPlay,hmmd->eState,s_hasExtPaused);
    
    if(MEDIA_TYPE_TONE == hmmd->eType)
    {
       //no support
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }
    
    if(!hmmd->isPlay)
    {
       //no support
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }
    //冲突占用
    if(s_hasExtPaused)
    {
       hmmd->eState = MEDIA_STATE_PLAYING;
       return (HRESULT)MEDIA_ERROR_CONFLICT;
    }
    
    if(s_mmPlay(hmmd))
    {
       hmmd->eState = MEDIA_STATE_PLAYING;
    }
    else
    {
       hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
    }

    return hres;
}

/*********************************************************************
 *  mmPause
 *
 * 暂停媒体播放/录制
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  PauseMedia(HMEDIAHANDLE  hwho)
{
     HRESULT hres=MEDIA_ERROR_NOERROR;
     HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
     BOOLEAN ret = FALSE;
   
     if(NULL == hmmd)
     {
        //SCI_TRACE_LOW:"PauseMedia error, hmmd is NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_802_112_2_18_3_10_46_93,(uint8*)"");
        return (HRESULT)MEDIA_ERROR_INVALPARAM;
     }
     //SCI_TRACE_LOW:"PauseMedia, eState = %d, handle = %d, s_hasExtPaused = %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_805_112_2_18_3_10_46_94,(uint8*)"ddd", hmmd->eState, hmmd->handle, s_hasExtPaused);
     	
     if(hmmd->eState != MEDIA_STATE_PLAYING && hmmd->eState != MEDIA_STATE_RECORDING)
     {
        return hres;
     }
     
     if(s_hasExtPaused)
     {
        hmmd->eState = MEDIA_STATE_PAUSE;
        return hres;
     }
     
     if(MEDIA_TYPE_TONE == hmmd->eType)
     {
        //SCI_TRACE_LOW:"PauseMedia, eType == TONE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_820_112_2_18_3_10_46_95,(uint8*)"");
        return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
     }

     if(hmmd->handle == INVALID_HANDLE)
     {
         hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
     }
     else
     {
        if(hmmd->unReserved0 != 0)
        {
            if(AUDIO_NO_ERROR == AUDIO_Pause(hmmd->handle))
            {
                ret = TRUE;
            }
        }
        else
        {
            if(MMISRVAUD_Pause(hmmd->handle))
            {
                ret = TRUE;
            }
        }
        
        if(ret)
        {
            hmmd->eState = MEDIA_STATE_PAUSE;
        }
        else
        {
            //SCI_TRACE_LOW:"PauseMedia, Pause fail, unReserved0 = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_851_112_2_18_3_10_46_96,(uint8*)"d", hmmd->unReserved0);
            hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
        }
     }
    
     return hres;
}
/*********************************************************************
 *  mmResume
 *
 * 继续播放/录制暂停的媒体
 *
 * 参数
 * hwho
 * [in] 指向一个设备句柄。
 * 返回值
 * 如果成功，返回值是零。
 * 如果失败，返回值是 一个 HRESULT类型的值。
 *
 * 说明
 * 对于tone音,返回失败。
**********************************************************************/
HRESULT  ResumeMedia(HMEDIAHANDLE  hwho)
{
   	HRESULT hres=MEDIA_ERROR_NOERROR;
   	HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
   	BOOLEAN flag = FALSE;
   	
   	if(NULL == hmmd)
   	{
   	    //SCI_TRACE_LOW:"ResumeMedia error, hmmd is NULL "
   	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_880_112_2_18_3_10_46_97,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
   	}
   	//SCI_TRACE_LOW:"ResumeMedia, eState = %d, handle = %d, s_hasExtPaused = %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_883_112_2_18_3_10_46_98,(uint8*)"ddd", hmmd->eState, hmmd->handle, s_hasExtPaused);
 
   	//no supported for tone & stream
   	if(MEDIA_TYPE_TONE == hmmd->eType)
   	{
       //SCI_TRACE_LOW:"ResumeMedia, eType == TONE"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_888_112_2_18_3_10_46_99,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
   	}
   
   	//冲突占用
   	if(s_hasExtPaused)
   	{
       hmmd->eState= (hmmd->isPlay) ? MEDIA_STATE_PLAYING : MEDIA_STATE_RECORDING;
       return (HRESULT)MEDIA_ERROR_CONFLICT;
   	}

    if(hmmd->eState == MEDIA_STATE_PAUSE)
    {
       if(hmmd->handle != INVALID_HANDLE)
       {
           if(hmmd->unReserved0 != 0)
           {
               if(AUDIO_NO_ERROR == AUDIO_Resume(hmmd->handle))
               {
                   flag = TRUE;
               }
           }
           else
           {
               if(MMISRVAUD_Resume(hmmd->handle))
               {
                   flag = TRUE;
               }
           }
          
           if(flag)
           {
              if(hmmd->isPlay)
              {
                 hmmd->eState = MEDIA_STATE_PLAYING;
              }
              else
              {
                 hmmd->eState = MEDIA_STATE_RECORDING;
              }
           }
           else
           {
              //SCI_TRACE_LOW:"ResumeMedia, Resume fail, unReserved0 = %d "
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_931_112_2_18_3_10_46_100,(uint8*)"d", hmmd->unReserved0);
              hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
           }
         
       }
    }

    return hres;
}
/*********************************************************************
 *  mmStop
 *
 * 停止播放/录制的媒体
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 *说明
 *   对于tone音,返回失败。
*/
HRESULT  StopMedia(HMEDIAHANDLE  hwho)
{
    HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;	// changed by yifei
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;

    if(NULL == hmmd)
    {
        //SCI_TRACE_LOW:"CloseMedia error, hmmd is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_961_112_2_18_3_10_46_101,(uint8*)"");
        return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    //SCI_TRACE_LOW:"CloseMedia, hmmd = 0x%x, handle = %d, eState = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_964_112_2_18_3_10_46_102,(uint8*)"ddd", hmmd, hmmd->handle, hmmd->eState);

	if(hmmd->eState == MEDIA_STATE_STOP)
	{
		return hres;
	}
	//for buffer record //20120420
	if((hmmd->eType == MEDIA_TYPE_BUFFER)&&(hmmd->eState == MEDIA_STATE_RECORDING ))
	{
		
		if(!MMIWRE_RecordBufferStop())
		{
			hmmd->eState = MEDIA_STATE_STOP;
			hres = MEDIA_ERROR_NOERROR;
			SCI_TRACE_LOW("MMIWRE_RecordBufferStop success !");
		}
		else
		{
			hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;	// changed by yifei
			SCI_TRACE_LOW("MMIWRE_RecordBufferStop fail !");
		}
		return hres;
	}
	
	if(MEDIA_TYPE_TONE == hmmd->eType)
	{
		MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_CUSTOM);
		hmmd->eState = MEDIA_STATE_STOP;
		//SCI_TRACE_LOW:"StopMedia, stop tone"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_975_112_2_18_3_10_46_103,(uint8*)"");
	}
	else
	{
		if((hmmd->eState == MEDIA_STATE_PLAYING || hmmd->eState == MEDIA_STATE_PAUSE  || hmmd->eState == MEDIA_STATE_RECORDING ) && hmmd->handle != INVALID_HANDLE)
		{

				if(hmmd->unReserved0 != 0)
				{
					AUDIO_Stop(hmmd->handle);
				}
				else
				{
					MMISRVAUD_Stop(hmmd->handle);
					if(!hmmd->isPlay)
					{
						SFS_ERROR_E err = SFS_NO_ERROR;
						err = MMIAPIFMM_CloseFile(hmmd->hfile);
					}
				}
				hmmd->eState = MEDIA_STATE_STOP;
		   }
	}
	return hres;
}

/*********************************************************************
 *  mmSeek
 *
 * 媒体播放定位
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * esType
 *   [in] 指定位搜查类型，详看2.1.2
 * lValue
 *   [in] 指定位的值，按字节还是毫秒，根据类型确定
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  SeekMedia(HMEDIAHANDLE  hwho,     // 设备句柄
				eMMSeekType  esType,
				long     lValue)
{
    HRESULT hres=MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    	
    if(NULL == hmmd)
    {
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    
    if(MEDIA_SEEK_TYPE_POS == esType || MEDIA_TYPE_TONE == hmmd->eType)
    {
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }
    
    if(MEDIA_STATE_PLAYING != hmmd->eState && MEDIA_STATE_RECORDING != hmmd->eState && lValue == 0)
    {
       hmmd->curTime = lValue;
       return hres;	
    }
   
    if(hmmd->isPlay)
    {
       if (FALSE == MMISRVAUD_SeekByTime(hmmd->handle, lValue / 1000))
       {
          return (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
       }
    }
    else
    {
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }
    hmmd->curTime = lValue;
 
    return hres;
}

/*********************************************************************
 *  mmSetVolume
 *
 * 设置设备音量
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * dwValue
 *   [in] 指音量值
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 ****************************************************************/
HRESULT  SetMediaVolume(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long     dwValue)
{
	HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;	// changed by yifei
	unsigned long     preVolumeValue;
	HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
	int32       nativeVolume = 0;

	if(NULL == hmmd)
	{
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1086_112_2_18_3_10_46_104,(uint8*)"");
		return (HRESULT)MEDIA_ERROR_INVALPARAM;
	} 
	
	if (dwValue <= 127)	// changed by yifei
	{
		nativeVolume = CalculateActualVolume(s_wre_platform_Volume, dwValue);
		if(MMISRVAUD_SetVolume(hmmd->handle,nativeVolume ))
		{
			 hmmd->curVol = dwValue;
			 WRE_MEDIA_CURRENT_VOLUME = nativeVolume;//added by leichi
		     MMI_WriteNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);//add by leichi
		 }
		 else
		 {
			hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;	// changed by yifei
		 }
	}
	else// in case of that wre application wants to set media volume by itself with side key,but fail to do it,then 
	{    //the volume effect is caused by wre boot .so ,the code following is important.
		GetMediaVolume(hwho, &preVolumeValue);
		nativeVolume = CalculateActualVolume(s_wre_platform_Volume, preVolumeValue);
		MMISRVAUD_SetVolume(hmmd->handle,nativeVolume );
		
		hres = (HRESULT)MEDIA_ERROR_OUTOFRANGE;
	}
	 SCI_TRACE_LOW("SetMediaVolume WRE_MEDIA_CURRENT_VOLUME=%d pre_volume= %d",WRE_MEDIA_CURRENT_VOLUME,nativeVolume);
	return hres;
}

/*********************************************************************
 *  mmGetVolume
 *
 * 获取设备音量值
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwValue
 *    [out] 指音量值
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaVolume(
				HMEDIAHANDLE  hwho,    // 设备句柄
				unsigned long     *pdwValue)
{
    HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    	
    if(NULL == hmmd || NULL == pdwValue)
    {
       //SCI_TRACE_LOW:"GetMediaVolume, invalid param"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1137_112_2_18_3_10_46_107,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    hmmd->curVol = 14* WRE_MEDIA_CURRENT_VOLUME;//added by leichi 20120521;
    *pdwValue=hmmd->curVol;
    //SCI_TRACE_LOW:"GetMediaVolume, curVol = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1142_112_2_18_3_10_46_108,(uint8*)"d", hmmd->curVol);
   
    return hres;
}

/*********************************************************************
 *  mmGetState
 *
 * 获取当前媒体控制状态
 *
 * 参数
 * hwho  [in] 指向一个设备句柄。
 * pdwState [out] 指状态值，对于播放来说，是指：播放、暂停、停止、未知；
 *     对于录制来说：录制、暂停、停止、未知。 
 *
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *********************************************************************/
HRESULT  GetMediaState(
				HMEDIAHANDLE  hwho,    // 设备句柄
				unsigned long   *pdwState)
{
    HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    	
    if(NULL == hmmd || NULL == pdwState)
    {
        return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    	
    *pdwState=hmmd->eState;
    return hres;
}

/*********************************************************************
 *  mmGetDuration
 *
 * 获取媒体播放总时长
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwDuration
 *   [out] 指媒体播放总时长(毫秒)
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*********************************************************************/
HRESULT  GetMediaDuration(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long   *pdwDuration)
{
    HRESULT hres=(HRESULT)MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    MMISRVAUD_CONTENT_INFO_T      format_info = {0};
    MMISRVAUD_PLAY_INFO_T play_info = {0};

    if(NULL == hmmd || NULL == pdwDuration)
    {
       //param is NULL
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    
    //no support type
    if(MEDIA_TYPE_TONE == hmmd->eType)
    {
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }
    
    //record no support
    if(!hmmd->isPlay)
    {
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }

       
    if(hmmd->handle != PNULL)
    {
       MMISRVAUD_GetPlayingInfo(hmmd->handle, &play_info);
       *pdwDuration =  (uint32)((uint64)play_info.total_time*1000);	// changed by yifei
       return hres;
    }

    if(MEDIA_TYPE_FILE == hmmd->eType)
    {
       if(MMISRVAUD_GetFileContentInfo((wchar *)hmmd->mdb.pDataOrFn, MMIAPICOM_Wstrlen((wchar *)hmmd->mdb.pDataOrFn), &format_info))
       {
          *pdwDuration = format_info.total_time * 1000;
       }
       else
       {
          hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
       }
    }
    else
    {
       if(MMISRVAUD_GetBuffContentInfo(hmmd->mdb.pDataOrFn, hmmd->mdb.dwDataLen, hmmd->dwFormat, &format_info))
       {
          *pdwDuration = format_info.total_time * 1000;
       }
       else
       {
          hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
       }
    }		

    return hres;
}

/*********************************************************************
 *  mmGetTime
 *
 * 获取媒体播放当前时间
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwTime
 *   [out] 指媒体播放当前时间(毫秒)
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaTime(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long   *pdwTime)
{
    HRESULT hres=MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    MMISRVAUD_PLAY_INFO_T play_info = {0};
    MMISRVAUD_CONTENT_INFO_T      format_info = {0};

    if(NULL == hmmd || NULL == pdwTime)
    {
       //param is NULL
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }


    if(MEDIA_TYPE_TONE == hmmd->eType || !hmmd->isPlay)
    {
       //command no support
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }

    if(hmmd->handle == 0)
    {
       *pdwTime = 0;
       return hres;
    }

    if(MMISRVAUD_GetPlayingInfo(hmmd->handle, &play_info))
    {
        *pdwTime = play_info.cur_time* 1000;
    }
    else
    {
       hres = (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
    }
    return hres;
}

/*********************************************************************
 *  mmRecord
 *
 * 媒体录制
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 * 说明
 * 对于tone音,返回失败。
 *
*/
HRESULT  RecordMedia(HMEDIAHANDLE  hwho)
{
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;

    if(NULL == hmmd)
    {
       //SCI_TRACE_LOW:"RecordMedia error, hmmd is NULL"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1333_112_2_18_3_10_47_109,(uint8*)"");
       return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
	  //SCI_TRACE_LOW:"RecordMedia, eType = %d, eState = %d"
	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1336_112_2_18_3_10_47_110,(uint8*)"dd", hmmd->eType, hmmd->eState);

	if((MEDIA_TYPE_FILE != hmmd->eType)&&(MEDIA_TYPE_BUFFER != hmmd->eType))//leichi20120509
	{
		return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
	}

    if(hmmd->isPlay)
    {
       //command not supported
       return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }

    if (MEDIA_STATE_STOP != hmmd->eState && MEDIA_STATE_UNKNOWN != hmmd->eState)
    {
        MMISRVAUD_Stop(hmmd->handle);
        MMISRVMGR_Free(hmmd->handle);
        hmmd->handle = 0;
    }

	s_cur_hmmd = hmmd;
	if(hmmd->eType == MEDIA_TYPE_BUFFER)
	{
		BOOLEAN res = FALSE;
		res = MMIWRE_StartRecordBuffer((const uint8 *)hmmd->mdb.pDataOrFn, hmmd->mdb.dwDataLen);//modefied by leichi 20120331
		SCI_TRACE_LOW("RecordMedia 111111");
		if (res)
		{
			hmmd->eState = MEDIA_STATE_RECORDING;
			SCI_TRACE_LOW("RecordMedia MEDIA_STATE_RECORDING");			
		}
		else
		{
			MMISRVMGR_Free(hmmd->handle);
			hmmd->handle = 0;
			SCI_TRACE_LOW("RecordMedia 22222");
			return (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
		}
	}
	else
	{
		if(MMISRVAUD_Play(hmmd->handle, 0))
		{
			hmmd->eState = MEDIA_STATE_RECORDING;
		}
		else
		{
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1364_112_2_18_3_10_47_111,(uint8*)"d", hmmd->handle);
			MMISRVMGR_Free(hmmd->handle);
			hmmd->handle = 0;
			return (HRESULT)MEDIA_ERROR_UNKNOWNERROR;
		}
	}
	return MEDIA_ERROR_NOERROR;
}

/*********************************************************************
 *  mmSetMute
 *
 * 静音设置
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * isMute
 *   [in] 静音开关，TRUE 静音，FALSE 关闭静音
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
*/
HRESULT  SetMediaMute(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned char    isMute)
{
   HRESULT hres=MEDIA_ERROR_NOERROR;
   HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
	
   if(NULL == hmmd)
   {
      //param is NULL
      return (HRESULT)MEDIA_ERROR_INVALPARAM;
   }

   if(isMute)
   {
      hmmd ->isMute = TRUE;//added by leichi 20120510
      MMISRVAUD_SetMuteNohandle(TRUE);
   }
   else
   {
      hmmd ->isMute = FALSE;//added by leichi 20120510
      MMISRVAUD_SetMuteNohandle(FALSE);
   }
   hmmd->isMute = isMute;

   return hres;
}

/*********************************************************************
 *  mmPlayTone
 *
 * 播放tone音
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * dwDuration
 *  [in] 播放时长
 * dwFreq
 *  [in] 播放频率
 *
 *
 *返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
*/
HRESULT  PlayTone(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long      dwDuration,
				unsigned long   dwFreg)
{

   HRESULT hres=MEDIA_ERROR_NOERROR;
   HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
	
   if(NULL == hmmd)
   {
      //param is NULL
      return (HRESULT)MEDIA_ERROR_INVALPARAM;
   }
	
   if(MEDIA_TYPE_TONE != hmmd->eType || hmmd->eState == MEDIA_STATE_RECORDING)
   {
      //command not supported
      return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
   }

   if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_WRE))
   {	
      uint32 freq = WRE_FREQ(dwFreg);/*lint !e524*/
      MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_CUSTOM, 0, dwDuration, freq);
      hmmd->eState = MEDIA_STATE_PLAYING;
   }
   
   return hres;
}

/*********************************************************************
 //供MOCOR调用的接口
*********************************************************************/
void MMIWRE_PauseMM(void)
{
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)s_cur_hmmd;
	
    if(NULL == hmmd)
    {
       return;
    }
    //SCI_TRACE_LOW:">>>PauseWREMM=%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1475_112_2_18_3_10_47_112,(uint8*)"ddd",hmmd->isPlay,hmmd->eState,s_hasExtPaused);

    if(MEDIA_TYPE_TONE == hmmd->eType)
    {
       //no support
       return;
    }

    if(hmmd->eState == MEDIA_STATE_PAUSE || hmmd->eState == MEDIA_STATE_STOP)
    {
       s_hasExtPaused=TRUE;
       return;
    }

    if(s_hasExtPaused)
    {
       //冲突中
       return;
    }

    if(hmmd->handle != INVALID_HANDLE)
    {
       if(hmmd->unReserved0 != 0)
       {
           HMEDIAINOUT *hParent = (HMEDIAINOUT *)hmmd->unReserved0;
           
           AUDIO_Pause(hmmd->handle);
           hmmd->eState = MEDIA_STATE_PAUSE;
           if(hParent->handle != INVALID_HANDLE)
           {
               MMISRVAUD_Pause(hParent->handle);
               hParent->eState = MEDIA_STATE_PAUSE;
           }
       }
       else
       {
           MMISRVAUD_Pause(hmmd->handle);
           hmmd->eState = MEDIA_STATE_PAUSE;
       }
    }
    s_hasExtPaused = TRUE;
}


void MMIWRE_ResumeMM(void)
{
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)s_cur_hmmd;
    	
    if(NULL == hmmd)
    {
       //SCI_TRACE_LOW:"MMIWRE_ResumeMM, hmmd is NULL "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1525_112_2_18_3_10_47_113,(uint8*)"");
       return ;
    }
    //SCI_TRACE_LOW:">>>ResumeWREMM=%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1528_112_2_18_3_10_47_114,(uint8*)"ddd",hmmd->isPlay,hmmd->eState,s_hasExtPaused);
    
    //no supported for tone & stream
    if(MEDIA_TYPE_TONE == hmmd->eType)
    {
       return ;
    }
    //没有外部暂停
    if(!s_hasExtPaused)
    {
       return;
    }
    if(hmmd->eState == MEDIA_STATE_PAUSE || hmmd->eState == MEDIA_STATE_STOP)
    {
       s_hasExtPaused=FALSE;
       return;
    }

    if(hmmd->isPlay)
    {
       if(hmmd->eState == MEDIA_STATE_PAUSE || hmmd->eState == MEDIA_STATE_PLAYING)
       {
           if(hmmd->unReserved0 != 0)
           {
               HMEDIAINOUT *hParent = (HMEDIAINOUT *)hmmd->unReserved0;

               if(hmmd->handle != INVALID_HANDLE)
               {
                   AUDIO_Resume(hmmd->handle);
                   hmmd->eState = MEDIA_STATE_PLAYING;
               }
              
               if(hParent->eState == MEDIA_STATE_PAUSE || hParent->eState == MEDIA_STATE_PLAYING)
               {
                   MMISRVAUD_Resume(hParent->handle);
                   hParent->eState = MEDIA_STATE_PLAYING;
               }
           }
           else
           {
               MMISRVAUD_Resume(hmmd->handle);
               hmmd->eState = MEDIA_STATE_PLAYING;
           }
       }
    }
    s_hasExtPaused =FALSE;
}


void MMIWRE_StopMM(void)
{
   HMEDIAINOUT *hmmd=(HMEDIAINOUT*)s_cur_hmmd;
   uint32 ret;
   if(NULL == hmmd)
   {
      //SCI_TRACE_LOW:"MMIWRE_StopMM, hmmd is NULL "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1583_112_2_18_3_10_47_115,(uint8*)"");
      return;
   }

   //SCI_TRACE_LOW:"MMIWRE_StopMM=%d, %d, %d, %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1587_112_2_18_3_10_47_116,(uint8*)"dddd", hmmd->eType, hmmd->isPlay,hmmd->eState, hmmd->handle);
   if(MEDIA_TYPE_TONE == hmmd->eType)
   {
      //tone
      MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_CUSTOM);
      hmmd->eState = MEDIA_STATE_STOP;
   }
   else
   {
      //media
      if(hmmd->isPlay)
      {
         if(hmmd->eState == MEDIA_STATE_PLAYING || hmmd->eState == MEDIA_STATE_PAUSE)
         {
             if(hmmd->unReserved0 != 0)
             {
                 HMEDIAINOUT *hParent = (HMEDIAINOUT *) hmmd->unReserved0;

                 if(hmmd->handle != INVALID_HANDLE)
                 {
                     if(AUDIO_Stop(hmmd->handle) == AUDIO_NO_ERROR)
                     {
                         hmmd->eState = MEDIA_STATE_STOP;
                     }
                 }
                 
                 if(hParent->eState == MEDIA_STATE_PLAYING  || hParent->eState == MEDIA_STATE_PAUSE)
                 {
                     if(MMISRVAUD_Stop(hParent->handle))
                     {
                        hParent->eState = MEDIA_STATE_STOP;
                     }
                 }
             }
             else
             {
                if(MMISRVAUD_Stop(hmmd->handle))
                {
                   hmmd->eState = MEDIA_STATE_STOP;
                }
             }
         }
      }
      else
      {
         if(hmmd->eState == MEDIA_STATE_RECORDING || hmmd->eState == MEDIA_STATE_PAUSE)
         {
            MMISRVAUD_Stop(hmmd->handle);
            MMISRVMGR_Free(hmmd->handle);
            hmmd->handle = 0;
            MMIAPIFMM_CloseFile(hmmd->hfile);
            hmmd->eState = MEDIA_STATE_STOP;
         }
      }
   }
   s_hasExtPaused =FALSE;
}
////
BOOLEAN MMIWRE_IsMMPlaying(uint32 nType)
{
    if(MMISET_RING_TYPE_WRE == nType)
    {
        return FALSE;
    }

    return (s_cur_hmmd)?TRUE:FALSE;
}
extern  BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMISET_CALL_MORE_RING_T ring_info,
                                                uint8 mode_id
                                                );
extern BOOLEAN MMIAPIENVSET_SetMoreMsgRingInfo(
                                               MN_DUAL_SYS_E dual_sys,
                                               MMISET_CALL_MORE_RING_T ring_info,
                                               uint8 mode_id
                                               );
extern uint8 MMIENVSET_GetCurModeId(void);
#ifndef MMIFMM_FILE_FULL_PATH_MAX_LEN
#define MMIFMM_FILE_FULL_PATH_MAX_LEN 255
#endif
typedef struct _tagAPP_RING_SET_PARAM
{
    int ring_type;
    int dual_sys;
    const unsigned short  full_path_ptr[MMIFMM_FILE_FULL_PATH_MAX_LEN];
    unsigned short  full_path_len;
}APP_RING_SET_PARAM, *PAPP_RING_SET_PARAM;
BOOLEAN _SetRingInfo(void *param)
{
    MMISET_CALL_MORE_RING_T     select_ring_info = {0};
    int ring_type,dual_sys;
    const unsigned short  *full_path_ptr = PNULL;
    unsigned short  full_path_len;
    PAPP_RING_SET_PARAM p;

    if (param == NULL)
    {
        return FALSE;
    }
    p = (PAPP_RING_SET_PARAM)param;
    ring_type = p->ring_type;
    dual_sys = p->dual_sys;
    full_path_ptr = p->full_path_ptr;
    full_path_len = p->full_path_len;
    if (dual_sys >= MN_DUAL_SYS_MAX || dual_sys < MN_DUAL_SYS_1)
    {//select sim card error
        return FALSE;
    }
   
    if (ring_type == 1)
    {//call ring
        if (full_path_len > MMISET_CALL_RING_NAME_MAX_LEN)
        {
            return FALSE;
        }
        select_ring_info.name_wstr_len = full_path_len;
        MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, full_path_ptr, full_path_len );
        if(!MMIAPIENVSET_SetMoreCallRingInfo(dual_sys,select_ring_info,MMIENVSET_GetCurModeId()))
        {
            return FALSE;
        }
    }
    else if (ring_type == 2)
    {//msg ring
        if (full_path_len > MMISET_CALL_RING_NAME_MAX_LEN)
        {
            return FALSE;
        }
        select_ring_info.name_wstr_len = full_path_len;
        MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, full_path_ptr, full_path_len );
        if(!MMIAPIENVSET_SetMoreMsgRingInfo(dual_sys,select_ring_info,MMIENVSET_GetCurModeId()))
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}
/*********************************************************************
*  Enable_Tp_Key_Tone
*  Author:heng.xiao
* 用于控制TP或者按键声音,特别是在游戏中,频繁触屏发触屏音影响效率
*
* 参数
* tone_type[in]
*   0, key tone
*   1, tp tone
*   2, tp and key tone
* opt[in]
*   TRRE, enable
*   FALSE, disable
* 返回值
*  TRUE--- 成功
*  FALSE-- 失败
*********************************************************************/
BOOLEAN Enable_Tp_Key_Tone(int tone_type,BOOLEAN opt)
{
#if 0
    MMIAUDIO_KEY_TYPE_E type;
    if (tone_type == 0)
    {
        type = MMIAUDIO_KEY_RING;
    }
    else if(tone_type == 1)
    {
        type = MMIAUDIO_TP_RING; 
    }
    else if (tone_type == 2)
    {
        type = MMIAUDIO_KEY_ALL;
    }
    else
    {//ring type error
        return FALSE;
    }
    MMIAUDIO_EnableKeyRing(type, MMIBGPLAY_MODULE_WRE, opt);
#endif
    return TRUE;    
}


/*********************************************************************
 *  GetMediaMicDb
 *
 * 获取当前设备Mic的灵敏度
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwMicDb
 *    [out] 指Mic灵敏度
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
 **********************************************************************/
HRESULT  _GetMediaMicDb(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long    *pdwMicDb)
{
    HRESULT hres=MEDIA_ERROR_NOERROR;
    HMEDIAINOUT *hmmd=(HMEDIAINOUT*)hwho;
    	
    if(NULL == hmmd || NULL == pdwMicDb)
    {
        //param is NULL
        return (HRESULT)MEDIA_ERROR_INVALPARAM;
    }
    //播放时, 不返回mic的灵敏度
    if(hmmd->isPlay)
    {
        return (HRESULT)MEDIA_ERROR_NOTSUPPORTED;
    }

    *pdwMicDb = MMISRVAUD_GetRecordDB(hmmd->handle);
    
    return hres;
}


PUBLIC void MMIWRE_HandleCallBackMsg(
                    void *msg_ptr
                    )
{
    MMISRVAUD_REPORT_T rpt_data = {0};
    HAUDIO handle = 0; 
    AUDIO_RESULT_E  audio_result = AUDIO_NO_ERROR;
    MmiAudioCallbackMsgS *callback_info_ptr = (MmiAudioCallbackMsgS *)msg_ptr;
    MMISRVMGR_NOTIFY_PARAM_T param = {0};

    if(callback_info_ptr != PNULL)
    {
        if(AUDIO_PLAYEND_IND == callback_info_ptr->notify_info)
        {
            rpt_data.report = MMISRVAUD_REPORT_END;
            handle = callback_info_ptr->hAudio;
            audio_result = (AUDIO_RESULT_E)callback_info_ptr->affix_info;
            if(handle != 0)
            {
                switch(audio_result)
                {
                case AUDIO_NO_ERROR:
                    rpt_data.data1 = (uint32)(MMISRVAUD_REPORT_RESULT_SUCESS);
                    break;
                default:
                    rpt_data.data1 = (uint32)(MMISRVAUD_REPORT_RESULT_ERROR);
                    break;
                }
                //MMISRVAUD_HandleAudioReport(handle, &rpt_data);
                param.event = MMISRVMGR_NOTIFY_EXT;
                param.data = (uint32) &rpt_data;
                wre_player_notify(handle, &param);
            }
        }
    }
}

LOCAL void AudioPlayCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    MmiAudioCallbackMsgS *sendSignal = PNULL;

    //SCI_TRACE_LOW:"[WRE] AudioPlayCallback, hAudio=0x%x, notify_info=%d,affix_info=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1856_112_2_18_3_10_48_117,(uint8*)"ddd", hAudio, notify_info, affix_info);

    if(AUDIO_PLAYEND_IND == notify_info)
    {
        MmiCreateSignal(APP_WRE_AUDIO_END_IND, sizeof(MmiAudioCallbackMsgS), (MmiSignalS**)&sendSignal);
        sendSignal->Sender = P_APP;
        sendSignal->hAudio      = hAudio;
        sendSignal->notify_info = notify_info;
        sendSignal->affix_info  = affix_info;
        MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
    }
}

LOCAL HAUDIOCODEC GetAudioCodec(MMISRVAUD_RING_FMT_E ring_type)
{
    HAUDIOCODEC hAudioCodec = 0;

    switch(ring_type)
    {
        case MMISRVAUD_RING_FMT_MIDI:
            hAudioCodec = hMIDICodec;
            break;

        case MMISRVAUD_RING_FMT_SMAF:
            hAudioCodec = hMIDICodec;
            break;

        case MMISRVAUD_RING_FMT_MP3:
			
#if defined(PLATFORM_SC6600L) || defined(AUDIO_SC6800H)
            hAudioCodec = hMP3DSPCodec;
#else
            hAudioCodec = hMP3Codec;
#endif
            break;

        case MMISRVAUD_RING_FMT_WMA:
#ifdef WMA_SUPPORT
            hAudioCodec = hWMACodec;
#endif
            break;

        case MMISRVAUD_RING_FMT_AMR:
            hAudioCodec = hAMRCodec;
            break;
            
        case MMISRVAUD_RING_FMT_WAVE:
            hAudioCodec = hWAVCodec;
            break;
            
        case MMISRVAUD_RING_FMT_AAC:
        case MMISRVAUD_RING_FMT_M4A:
            hAudioCodec = hAACLCCodec;
            break;
            
        default:
            hAudioCodec = hMIDICodec;
            break;
    }

    //SCI_TRACE_LOW:"[WRE] WreGetRingCodec, ring_type=%d, codec=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1916_112_2_18_3_10_48_118,(uint8*)"dd", ring_type, hAudioCodec);

    return hAudioCodec;
}

LOCAL HAUDIODEV GetAudioDev(BOOLEAN is_a2dp, MMISRVAUD_RING_FMT_E ring_fmt)
{
    HAUDIODEV hDevice = 0;

    switch(ring_fmt)
    {
        case MMISRVAUD_RING_FMT_MP3:
        case MMISRVAUD_RING_FMT_AAC:
        case MMISRVAUD_RING_FMT_M4A:
        case MMISRVAUD_RING_FMT_WMA:
        case MMISRVAUD_RING_FMT_MIDI:
        case MMISRVAUD_RING_FMT_WAVE:
#ifdef AMR_A2DP_SUPPORT
        case MMISRVAUD_RING_FMT_AMR:
#endif            
            #ifdef BLUETOOTH_SUPPORT
            if (is_a2dp)
            {
                if(MMIBT_HFG_HEADSET == MMIAPIBT_GetActiveHeadsetType())
                {
                    //HFG device play
                    hDevice = hPCMDev; 
                }   
                #ifdef BT_A2DP_SUPPORT
                else
                {
                    //A2DP Device play
                    hDevice = hA2DPDev; 
                }
                #endif
            }
            else
            #endif
            {
                hDevice = hARMVB;
            }
            break;
        default:
            hDevice = hLAYER1;
            break;
    }

    //SCI_TRACE_LOW:"[WRE]GetAudioDev, is_a2dp=%d, ring_fmt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_1963_112_2_18_3_10_48_119,(uint8*)"dd", is_a2dp, ring_fmt);

    return hDevice;
}

BOOLEAN SetDeviceMode(HMEDIAINOUT *hmmd)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = 0;
    AUDIO_DEVICE_MODE_TYPE_E set_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    BOOLEAN ret = FALSE;
    HAUDIODEV  hDevice = 0;

    if(hmmd == NULL)
    {
        return ret;
    }
    //if(IsTypeSupportBt(entity_ptr) && MMISRVAUD_IsBTHeadSetActive())
    if(MMISRVAUD_IsHeadSetPlugIn())
    {
        set_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
    }
    else
    {
        set_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    
    hDevice = GetAudioDev(FALSE, hmmd->dwFormat);
    result = AUDIO_SwitchDevice(hmmd->handle, hDevice, PNULL);
    if(AUDIO_GetDevMode(&cur_dev_mode) == AUDIO_NO_ERROR) 
    {
        if(set_dev_mode != cur_dev_mode)
        {
            result = AUDIO_SetDevMode(set_dev_mode);
            if(result == AUDIO_NO_ERROR)
            {
                ret = TRUE;
            }
        }
        else
        {
            ret = TRUE;
        }
    }
    return ret;
}

LOCAL HAUDIO MMIWRE_CreateAudioFileHandle(
                    BOOLEAN is_a2dp,
                    MMISRVAUD_RING_FMT_E ring_fmt,
                    const wchar *name_ptr,
                    uint32* puitrackbuffer,
                    uint32 uiTrackBufferSize
                    )
{
    HAUDIO audiohandle = INVALID_HANDLE;
    HAUDIOCODEC hAudioCodec = 0;
    uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    uint16* pusDevName = PNULL;
    HAUDIOCODEC real_audio_codec = 0;
    AUDIO_RESULT_E result = AUDIO_ERROR;

    
    hAudioCodec = GetAudioCodec(ring_fmt);
    hDevice = GetAudioDev(is_a2dp, ring_fmt);

    audiohandle = AUDIO_CreateFileHandle(
                        hAudioCodec,    // the codec handle which will be used.
                        pusCodecName,     // the codec name which will be used.
                        hDevice, 
                        pusDevName,
                        (const int16 *)name_ptr,
                        puitrackbuffer,     // decode data output buffer.
                        uiTrackBufferSize,    // the track buffer size.
                        AudioPlayCallback
                        );

    if(INVALID_HANDLE != audiohandle)
    {
        real_audio_codec = AUDIO_GetCodecHandle(audiohandle);
        if(real_audio_codec != hAudioCodec)
        {
            hDevice = GetAudioDev(is_a2dp, ring_fmt);
            result = AUDIO_SwitchDevice(audiohandle, hDevice, pusDevName);
        }
    }

    //SCI_TRACE_LOW:"[WRE]:audiohandle=0x%x,hAudioCodec=0x%x,real_audio_codec=0x%x,hDevice=0x%x,result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_MEDIA_2052_112_2_18_3_10_48_120,(uint8*)"ddddd",audiohandle, hAudioCodec, real_audio_codec, hDevice, result);

    return audiohandle;
}
#if 1 // These  functions fallowing were defined by leichi for volume adjusting in wre application using side key 20120417;

PUBLIC BOOLEAN MMIWRE_IsWREAudioPlayerActive(void)//defined by leichi for sidekey function
{
	SCI_TRACE_LOW("wre_audio MMIWRE_IsWREAudioPlayerActive s_cur_hmmd = 0x%d",s_cur_hmmd);
	if(NULL == s_cur_hmmd)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
PUBLIC void MMIWRE_InitSideKeyVolume(void)
{	
	MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;
	SCI_TRACE_LOW("wre_audio MMIWRE_InitSideKeyVolume 1111");
	ret_value = MMI_ReadNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);
	if(MN_RETURN_FAILURE == ret_value)
	{
		WRE_MEDIA_CURRENT_VOLUME = 5;
		MMI_WriteNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);
		SCI_TRACE_LOW("wre_audio MMIWRE_InitSideKeyVolume 2222");
	}
}

PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_UP(void)
{

	// int32       nativeVolume = 0;
	 BOOLEAN result = FALSE;
	 MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;

	if ((NULL == s_cur_hmmd)&&(!MMIWRE_IsWREVideoPlayerActive()))	// changed by yifei
	{
		return FALSE;
	}
	/*
	nativeVolume = (s_cur_hmmd->curVol ) / 14;
	if(nativeVolume < 9)
	{
		nativeVolume++;
		s_cur_hmmd->curVol = s_cur_hmmd->curVol + 14;
	}
	*/

	ret_value = MMI_ReadNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);
	if(MN_RETURN_FAILURE == ret_value)
	{
		WRE_MEDIA_CURRENT_VOLUME = 5;
	}
	if(WRE_MEDIA_CURRENT_VOLUME < 9)
	{
		WRE_MEDIA_CURRENT_VOLUME++;
	}
	MMI_WriteNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);

	result = MMISRVAUD_SetVolumeNoHandle(WRE_MEDIA_CURRENT_VOLUME );
	SCI_TRACE_LOW("MMIWRE_AdjustVolumeBySideKey_UP down ret_value = %d,WRE_MEDIA_CURRENT_VOLUME= %d",ret_value,WRE_MEDIA_CURRENT_VOLUME);

	return result;
}

PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_DOWN(void)
{
	 BOOLEAN result = FALSE;
	 MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;

	if ((NULL == s_cur_hmmd)&&(!MMIWRE_IsWREVideoPlayerActive()))	// changed by yifei
	{
		return FALSE;
	}
	/*
	nativeVolume = (s_cur_hmmd->curVol ) / 14;
	if(nativeVolume > 0)
	{
		nativeVolume--;
		s_cur_hmmd->curVol = s_cur_hmmd->curVol - 14;
	}
	*/
	ret_value = MMI_ReadNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);
	if(MN_RETURN_FAILURE == ret_value)
	{
		WRE_MEDIA_CURRENT_VOLUME = 5;
	}
	if(WRE_MEDIA_CURRENT_VOLUME > 0)
	{
		WRE_MEDIA_CURRENT_VOLUME--;
	}
	MMI_WriteNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);

	result = MMISRVAUD_SetVolumeNoHandle(WRE_MEDIA_CURRENT_VOLUME );
	SCI_TRACE_LOW("MMIWRE_AdjustVolumeBySideKey_DOWN down ret_value = %d,WRE_MEDIA_CURRENT_VOLUME= %d",ret_value,WRE_MEDIA_CURRENT_VOLUME);
	return result;

}

#endif
#if 1//added by leichi for buffer record 
#define ANSI2UINT16(quote) (L##quote) 

typedef struct __tagRecordBuffer
{
    BOOLEAN flag;
    uint8 * buffer;
    uint8 * record_ptr;
    uint32 buf_len;
    uint32 written_len;
}Record_In_Buffer_Struct;
Record_In_Buffer_Struct s_record_data;
LOCAL HAUDIO s_record_handle;
#include "dsp_codec_adp.h"
PUBLIC void record_to_buffer_init(uint8 *buffer, uint32 buf_len)
{
    s_record_data.buffer = buffer;
    s_record_data.buf_len = buf_len;
    s_record_data.written_len = 0;
    s_record_data.record_ptr = NULL;
    s_record_data.flag = TRUE;
}
LOCAL void record_in_buffer_finish(uint32 writeLen)//modified by leichi 20120331
{
    HMEDIAINOUT *hmmd = s_cur_hmmd;

    if(hmmd->endMedia)
    {
        hmmd->endMedia(MEDIA_NOTIFY_REPONSEDATA,(unsigned int)hmmd,writeLen);//modified by leichi 20120331
    }
}
PUBLIC uint32 mmiwre_record_read_data(uint32 buf_addr,uint32 buf_len,uint32 data_len)
{
	AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
	uint16 *p_buff_2B_align = PNULL;
	uint32 sample_len_getted = 0;
	HAUDIO record_handle = s_record_handle;//MMIWRE_GetRecordHandle();
	DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
	uint32 temp;

	SCI_TRACE_LOW("mmiwre_record_read_data 11111 ");
	if( INVALID_HANDLE == record_handle)
	{
		return 0;
	}

	if( (buf_addr & 0x1)> 0 )
	{
		SCI_TRACE_LOW("mmiwre_record_read_data 2222 ");
		p_buff_2B_align = (uint16 *)(buf_addr + 1);
	}
	else
	{
		SCI_TRACE_LOW(" mmiwre_record_read_data 3333 ");
		p_buff_2B_align = (uint16 *)buf_addr;
	}

	ptPara.source_buf_ptr = p_buff_2B_align;
	ptPara.data_length = (uint16)(data_len>>1);// 2bytes per sample.
	aud_result = AUDIO_ExeCodecExtOpe(record_handle, ANSI2UINT16("READ_VOICE_DATA"), DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, &ptPara,&temp); 

	sample_len_getted = (AUDIO_NO_ERROR != aud_result)? 0 : (data_len>>1);
	SCI_TRACE_LOW("mmiwre_record_read_data 44444 sample_len_getted = %d", sample_len_getted);

	if( sample_len_getted > 0)
	{
		SCI_TRACE_LOW("mmiwre_record_read_data 5555 record_ptr = 0x%x, written_len = %d", s_record_data.record_ptr, s_record_data.written_len);
		if(s_record_data.written_len == 0)
		{
			s_record_data.record_ptr = s_record_data.buffer;
		}
		if((s_record_data.written_len + (sample_len_getted<<1)) <=  s_record_data.buf_len)
		{
			temp = (sample_len_getted<<1);
			SCI_MEMCPY((uint8 *)s_record_data.record_ptr, p_buff_2B_align, temp);
			s_record_data.written_len += temp;
			s_record_data.record_ptr += temp;
		}
		else
		{
			SCI_TRACE_LOW("mmiwre_record_read_data 7777 ");
			{
				record_in_buffer_finish(s_record_data.written_len);
				s_record_data.written_len = 0;//added by leichi 20120331

			}
		}


	if( (buf_addr & 0x1)> 0 )
	{
		SCI_TRACE_LOW("  mmiwre_record_read_data 8888 ");
		SCI_MEMCPY((uint8 *)buf_addr,p_buff_2B_align,(sample_len_getted<<1));
	}
    }

    return (sample_len_getted<<1);
}

// RECORD_CALLBACK
static void dsp_codec_callback(uint16 sample_len)
{
    // 2bytes per sample.
    uint32 data_len  = ((uint32)sample_len)<<1;
    uint16 test_buf[160];

    SCI_TRACE_LOW("  dsp_codec_callback @@@ sample_len = %d", sample_len);
    mmiwre_record_read_data((uint32)test_buf,(160<<1),data_len );
    return;
}


PUBLIC int MMIWRE_RecordBufferStop(void)
{
    uint32 temp;
    AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
    int ret = SCI_SUCCESS;
    HAUDIO record_handle = s_record_handle;

    SCI_TRACE_LOW(" wre_record_buffer_stop 1111");
    if(INVALID_HANDLE == record_handle)
    {
        return ret;
    }

    aud_result = AUDIO_ExeCodecExtOpe(record_handle, 
                            ANSI2UINT16("EXT_STOP_RECORD"), 
                            1, 
                            (void *)record_handle,
                            &temp
                            ); 
    SCI_TRACE_LOW(" wre_record_buffer_stop 4444 aud_result = %d", aud_result);
    ret = (AUDIO_NO_ERROR != aud_result)? SCI_ERROR : ret;

    aud_result = AUDIO_CloseHandle(record_handle); 
    ret = (AUDIO_NO_ERROR != aud_result)? SCI_ERROR : ret;
    SCI_TRACE_LOW(" wre_record_buffer_stop 5555 aud_result = %d", aud_result);
    s_record_handle = INVALID_HANDLE;
    memset(&s_record_data, 0, sizeof(Record_In_Buffer_Struct));

    return ret;
}

static void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}
PUBLIC BOOLEAN MMIWRE_StartRecordBuffer(const uint8 * buffer, uint32 buf_len)
{
	AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
	uint32 temp;
	int ret = FALSE;
	DSP_CODEC_EXT_OPE_START_RECORD_T ptPara;
	HAUDIO record_handle ;//MMIWRE_GetRecordHandle();
	if(INVALID_HANDLE == s_cur_hmmd)
	{
		return FALSE;
	}
	
	if((buffer == NULL) || (buf_len == 0))
	{
		return FALSE;
	}
	SCI_TRACE_LOW("MMIWRE_StartRecordBuffer 1111 ");

	record_to_buffer_init(buffer, buf_len);
	record_handle = AUDIO_CreateNormalHandle(
	                        hDSPCodec, 
	                        PNULL, 
	                        hMP4RECORDVB, 
	                        PNULL,
	                        AudioDummyNotifyCallback);

	if( INVALID_HANDLE == record_handle)
	{
		SCI_TRACE_LOW(" MMIWRE_StartRecordBuffer 22222 s_record_handle opened fail");
		return FALSE ;
	}
	else
	{
		s_record_handle = record_handle;
		SCI_TRACE_LOW(" MMIWRE_StartRecordBuffer yyy s_record_handle opened successl");

	}

	ptPara.recorder_format = AUD_REC_ADPCM;
	ptPara.callback_fun = dsp_codec_callback;
	ptPara.hAudioHandle = record_handle;

	aud_result = AUDIO_ExeCodecExtOpe(record_handle, 
	                    ANSI2UINT16("EXT_START_RECORD"), 
	                    DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT, 
	                    &ptPara,
	                    &temp
	                    ); 
	ret = (AUDIO_NO_ERROR != aud_result)? FALSE : TRUE;
	SCI_TRACE_LOW(" MMIWRE_StartRecord 5555, ret = %d", ret);
	return ret;
}
#endif

#endif
