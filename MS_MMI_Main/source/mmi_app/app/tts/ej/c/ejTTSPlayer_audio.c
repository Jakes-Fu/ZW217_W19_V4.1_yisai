/*	ejTTSPlayer.c
 *	
 *	Author: Deng Jian, Huang Zhenglin
 *	Date: 2009-3-16
 *	
 *	Ver: 1.1
 *	Update: Ye Yan
 *	Date: 2009-3-29
 *
 *	Ver: 1.11
 *	Update: Deng Jian
 *	Date: 2009-5-18
 *
 *	Ver: 1.12
 *	Update: Ye Yan
 *	Date: 2009-5-28
 *
 *	Ver: 1.13
 *	Update: Ye Yan
 *	Date: 2009-6-3
 *
 *	Copyright (C) 1999-2008, SinoVoice Ltd.
 *
 *	Description: ejTTS前端ejTTSPlayer的实现
 */
 
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_tts_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "window_parse.h"
#include "os_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiset_export.h"
#include "mmi_signal.h"
#include "mmi_default.h"
#include "freq_cfg.h"
#include "ejTTSPlayer_api.h"
#include "ejTTSPlayer_audio.h"
#include "mmifm_export.h"
#include "mmipub.h"
#include "priority_app.h"
#include "mmitts_export.h"
#include "mmisrvaud_api.h"
#include "mmisrvaud.h"
#include "mmisrvmgr.h"
#include "wav_adp.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
		
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
// 是否支持英文
//#define EJ_ENG_SUPPORT

// 输出TTS Trace信息的宏
#define TTS_PLAYER_DEBUG

// 将一个原先SRAM上分配的空间改到堆上
//#define TTS_TEST_MALLOC


#define JTTS_COMMAND_STATUS_NONE		0			// 命令未使用
#define JTTS_COMMAND_STATUS_INIT		1			// 命令已初始化，等待处理
#define JTTS_COMMAND_STATUS_HANDLING	2			// 命令处理中
#define JTTS_COMMAND_STATUS_HANDLED		3			// 命令已处理


#define JTTS_COMMAND_INIT			0			// 初始化命令
#define JTTS_COMMAND_END			1			// 结束命令
#define JTTS_COMMAND_PAUSE			2			// 暂停命令
#define JTTS_COMMAND_RESUME			3			// 恢复命令
#define JTTS_COMMAND_STOP			4			// 停止命令
#define JTTS_COMMAND_PLAY			5			// 播放命令
#define JTTS_COMMAND_PLAYTOFILE		6			// 播放到文件命令
#define JTTS_COMMAND_GETPARAM		7			// 获取参数命令
#define JTTS_COMMAND_SETPARAM		8			// 设置参数命令

#define JTTS_COMMAND_SYNC			0			// 同步命令
#define JTTS_COMMAND_ASYNC			1			// 同步命令

// 播放线程状态
#define JTTS_STATUS_IDLE			0x00000000
#define JTTS_STATUS_PLAY			0x00000002
#define JTTS_STATUS_PAUSE			0x00000004
#define JTTS_STATUS_STOP			0x00000008
#define JTTS_STATUS_EXIT			0x00000010

#ifdef TTS_PLAYER_DEBUG
#define  JTTS_LOG SCI_TRACE_LOW						//打Log的宏
#else
//#pragma warning(disable: 2917)
#pragma warn -2917
#define  JTTS_LOG
#endif	// TTS_PLAYER_DEBUG

//临界区控制
#define ENTRY_MUTEX(m)  SCI_GetMutex(m, SCI_WAIT_FOREVER )

#define LEAVE_MUTEX(m)  SCI_PutMutex(m)

#define MMITTS_TASK_STACK_SIZE                    4096 //任务栈大小
#define MMITTS_TASK_QUEUE_NUM                     8   //任务消息队列大小
// 把TTS的优先级提高一级是为了避免java 与TTS 同级而造成的tts 无法运行
// 而把MMI挂起，被JAVA阻塞的情况， 具体见cr184217
#define MMITTS_TASK_PRIORITY                      PRI_MMITTS_TASK //29//(SCI_PRIORITY_IDLE - 1)    提高优先级不低于图片解码


#define MAX_SUPPORT_HANDLER_NUM			32              //Only support 32 handler running at the same clk.

#define	JTTSPCM_PLAY_BUF_SIZE	(8*1024)//(16*1024)				//AUDIO_Play() callback function buffer size


#define PARENT_EVENT	1				//PARENT_EVENT
#define CHILD_EVENT		2				//CHILD_EVENT


#ifndef MAX_PATH
#define MAX_PATH 260					//MAX_PATH
#endif

#define TTS_PLAYER_MAX_TEXTLEN	(1024 + 2)
#define TTS_AUDIO_DECODE_MIN_LEN  160

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
// JTTSPlayData用于存放播放线程内的数据
typedef struct tagJTTSPlayData
{
	uint32				hJTTS;				// JTTS句柄  2010-9-10 uint64 -> uint64
	char*				pHeap;				// 为引擎开辟的缓冲区
	SCI_MUTEX_PTR		pMutex;				// 数据操作临界区
}JTTSPlayData;

// JTTSCommandData定义命令数据，用于调用线程（主线程）和播放线程之间的同步
typedef struct tagJTTSCommandData
{
	uint32				nStatus;			// 命令处理状态
	uint32				nCommand;			// 命令类型
	uint32				nSync;				// 同步执行
	uint32				ErrCode;			// 命令执行结果状态
	union Parameter
	{
		struct Play
		{
			JTTSCALLBACKPROCEX	lpfnCallback;
			DWORD		dwUserData;
		}Play;
	}Parameter;
}JTTSCommandData;

// JTTSSyncData用于线程之间的同步，包括同步数据和要执行的命令
typedef struct tagJTTSSyncData
{
	SCI_MUTEX_PTR		pMutex;
	uint32				Status;				// 线程同步状态
	JTTSCommandData		Command;			// 操作命令
}JTTSSyncData;

typedef struct tagMyUserData							//用户自定义数据
{
	char		szTextIn[TTS_PLAYER_MAX_TEXTLEN];				//输入文本字符串
	uint16		uTextLen;
 	uint16		szOutputVoicePath[MAX_PATH];	//输出声音文件路径
}MyUserData;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MyUserData		myUserData;
LOCAL SFS_HANDLE		g_filehandle = INVALID_HANDLE;

//////////////////////////////////////////////////////////////////////////
LOCAL JTTSSyncData		g_SyncData = { 0 };					// 线程同步数据
LOCAL JTTSPlayData		g_PlayData = { 0 };					// 播放线程数据

LOCAL BLOCK_ID				g_jtts_thread_id = SCI_INVALID_BLOCK_ID;		// jtts thread id
LOCAL SCI_EVENT_GROUP_PTR	g_jtts_event = SCI_NULL;						// jtts event group
LOCAL MMISRV_HANDLE_T 		s_tts_audio_handle = INVALID_HANDLE;				// audio handle
						
LOCAL WAV_STREAM_BUFFER_T	g_head_info = { 0 };							// pcm stream head info

LOCAL uint8					*g_play_ptr = PNULL; //[JTTSPCM_PLAY_BUF_SIZE] = { 0 };			// play buffer
LOCAL uint32				g_playsize = 0;									// playing buffer size
LOCAL uint32				g_playpos = 0;									// play buffer g_temp size
LOCAL BOOLEAN				g_bplayed = FALSE;								// audio_play call back function 播放完成
LOCAL BOOLEAN 				g_bWrite = TRUE;
uint32 						g_uPrevTickCount = 0;
LOCAL uint8					*g_playbuffer_ptr = PNULL;//[JTTSPCM_PLAY_BUF_SIZE] = { 0 };
LOCAL BOOLEAN               s_is_stop_by_srv = FALSE;

// LOCAL BOOLEAN            s_is_audio_running = FALSE;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
//extern uint32 MMI_SetFreq(PROD_FREQ_INDEX_E freq);
//extern uint32 MMI_RestoreFreq(void);     


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 服务线程
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL void JTTS_Thread( uint32 argc, void * argv );

/*****************************************************************************/
//  Description : jtts 初始化
//  Global resource dependence : none
//  Author: yaye.jiang
//const char* szCNLib 中文音库
//const char* szENLib 英文音库
//const char* szDMLib DM音库
/*****************************************************************************/
LOCAL uint32 JTTS_Init(const char* szCNLib, const char* szENLib, const char* szDMLib);

/*****************************************************************************/
//  Description : jtts 播放
//  Global resource dependence : none
//  Author: yaye.jiang
//char * pcszText 播放文本指针
//JTTSCALLBACKPROCEX lpfnCallback 回调函数
//DWORD dwUserData 用户数据
/*****************************************************************************/
LOCAL uint32 JTTS_Play(char * pcszText, JTTSCALLBACKPROCEX lpfnCallback, DWORD dwUserData);

/*****************************************************************************/
//  Description : 将播放数据写入到文件
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_PlayToFile( char *pcszText, const char *pcszFileName );

/*****************************************************************************/
//  Description : 清除事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Clr(uint32 event_id);

/*****************************************************************************/
//  Description : 激活事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Set(uint32 event_id);

/*****************************************************************************/
//  Description : 挂起事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Wait(uint32 event_id);

/*****************************************************************************/
//  Description : notify handle function for TTS
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JTTS_AudioNotifyCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E JTTS_AudioCallBackFunc(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset)
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          );

/*****************************************************************************/
//  Description : 获得播放器当前音频格式
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 GetAudioFormat(DWORD dwHandle, uint32* pnSamplesPerSec, uint16* pnBitsPerSample);

/*****************************************************************************/
//  Description : voice callback type
//  Global resource dependence : none
//  Author: 
// pParameter			[in] user callback parameter
// iOutputFormat		[in] output data format
// pData				[in] output data buffer
// iSize				[in] output data size
/*****************************************************************************/
LOCAL ERRCODE OutputVoiceToFileProc(void* pParameter, long iOutputFormat, void* pData, long iSize);

/*****************************************************************************/
//  Description : output voice callback type
//  Global resource dependence : none
//  Author: 
// pParameter			[in] user callback parameter
// iOutputFormat		[in] output data format
// pData				[in] output data buffer
// iSize				[in] output data size
/*****************************************************************************/
LOCAL ERRCODE OutputVoiceToPlayProc(void* pParameter, int32 iOutputFormat, void* pData, int32 iSize);

/*****************************************************************************/
//  Description : jtts 暂停
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Pause(void);

/*****************************************************************************/
//  Description : jtts 恢复
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Resume(void);

/*****************************************************************************/
//  Description : jtts 停止
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Stop(void);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E JTTS_AudioCallBackFunc(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset)
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          )
{	
	uint32 uTickCount = 0;
	uTickCount = SCI_GetTickCount();
	//JTTS_LOG:"TTSPlayer|Entery JTTS_AudioCallBackFunc tick = %d g_bplayed = %d, g_SyncData.Status = %d, hAudio = %d, g_playsize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_321_112_2_18_3_3_40_29,(uint8*)"ddddd", uTickCount,g_bplayed,g_SyncData.Status,hAudio,g_playsize);

	SCI_TRACE_LOW("[TTS] JTTS_AudioCallBackFunc g_play_ptr = %x, g_playbuffer_ptr = %x",g_play_ptr,g_playbuffer_ptr);
	if (PNULL == g_play_ptr
		|| PNULL  == g_playbuffer_ptr)
	{
		SCI_TRACE_LOW("JTTS_AudioCallBackFunc g_play_ptr is null");
		return AUDIO_STREAM_GET_SRC_ERROR;
	}
	
	if (hAudio != MMISRVAUD_SearchHandleByHandle(TTS_GetAudiohandle()))
	{
		//JTTS_LOG:"TTSPlayer|JTTS_AudioCallBackFunc audio handle is not g_audio_handle"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_326_112_2_18_3_3_40_30,(uint8*)"" );
		ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
		return AUDIO_STREAM_GET_SRC_SUCCESS;
	}
	
	if (g_playsize == 0)
	{
		ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
		//JTTS_LOG:"TTSPlayer|AudioCallBack wait for g_playsize > 0"
		SCI_TRACE_LOW("TTSPlayer|AudioCallBack g_bplayed = %d",g_bplayed);
	//	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_339_112_2_18_3_3_40_32,(uint8*)"" );
		if (g_bplayed)
		{
			return AUDIO_STREAM_GET_SRC_ERROR;
		}
		else
		{
			return AUDIO_STREAM_GET_SRC_WAIT;
		}
	}	

		
	//SCI_TRACE_LOW:"TTSPlayer|JTTS_AudioCallBackFunc g_playsize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_344_112_2_18_3_3_40_33,(uint8*)"d", g_playsize );
	//将播放数据 copy 到 *ppucDataAddr 
	SCI_MEMSET(g_playbuffer_ptr, 0, JTTSPCM_PLAY_BUF_SIZE);
	SCI_MEMCPY(g_playbuffer_ptr, g_play_ptr, g_playsize);
	ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr = g_playbuffer_ptr;
	ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = g_playsize;

	//JTTS_LOG:"TTSPlayer|JTTS_Audio_CallBackFunc|Out %x %x"
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_351_112_2_18_3_3_40_34,(uint8*)"dd", g_playbuffer_ptr[g_playsize-2], g_playbuffer[g_playsize-1]);

	g_playsize = 0;
	SCI_MEMSET(g_play_ptr,0,JTTSPCM_PLAY_BUF_SIZE);
	g_bWrite = TRUE;
	
//	JTTS_LOG( "TTSPlayer|JTTS_Audio_CallBackFunc|Out %x %x", g_playbuffer[g_playsize-2], g_playbuffer[g_playsize-1]);

	return AUDIO_STREAM_GET_SRC_SUCCESS;
}

/*****************************************************************************/
//  Description : voice callback type
//  Global resource dependence : none
//  Author: 
// pParameter			[in] user callback parameter
// iOutputFormat		[in] output data format
// pData				[in] output data buffer
// iSize				[in] output data size
/*****************************************************************************/
LOCAL ERRCODE OutputVoiceToFileProc(void* pParameter, long iOutputFormat, void* pData, long iSize)
{
//	MyUserData *	pUserData;
	uint32 uwritten;
	
	//JTTS_LOG:"TTSPlayer|OuputVoiceToFile|In iSize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_374_112_2_18_3_3_40_35,(uint8*)"d", iSize);

	//写入pcm文件 
	if ( INVALID_HANDLE != g_filehandle )
	{
		MMIFILE_WriteFile(g_filehandle, pData, iSize, &uwritten, 0);
	}
	
	//JTTS_LOG("TTSPlayer|Leave OutputvoiceTofile");
	return jtTTS_ERR_NONE;
}

/*****************************************************************************/
//  Description : output voice callback type
//  Global resource dependence : none
//  Author: 
// pParameter			[in] user callback parameter
// iOutputFormat		[in] output data format
// pData				[in] output data buffer
// iSize				[in] output data size
/*****************************************************************************/
LOCAL ERRCODE OutputVoiceToPlayProc(void* pParameter, int32 iOutputFormat, void* pData, int32 iSize)
{
	uint32 uTickCount = 0;

	uTickCount = SCI_GetTickCount();
	//JTTS_LOG:"TTSPlayer|OutputVoiceToPlayProc tick count = %d iSize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_398_112_2_18_3_3_40_36,(uint8*)"dd", uTickCount,iSize);

	if ( g_uPrevTickCount != 0 )
	{
		uTickCount = uTickCount - g_uPrevTickCount;
		//JTTS_LOG:"TTSPlayer|synthe time used = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_403_112_2_18_3_3_40_37,(uint8*)"d", uTickCount );
	}
	// JTTS_LOG( "TTSPlayer|sync data mutex is %d", (uint32)g_SyncData.pMutex );

	// 上层终止
	ENTRY_MUTEX( g_SyncData.pMutex );	

	if( ( g_SyncData.Status == JTTS_STATUS_STOP ) 
		|| ( g_SyncData.Status == JTTS_STATUS_EXIT ))									// 上层终止
	{
		//JTTS_LOG:"TTSPlayer|set g_playpos = 0"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_413_112_2_18_3_3_40_38,(uint8*)"" );
		g_playsize = 0;
		g_bplayed = TRUE;
		LEAVE_MUTEX( g_SyncData.pMutex );
		return jtTTS_ERR_NONE;
	}
	LEAVE_MUTEX( g_SyncData.pMutex );
	
	while ( !g_bWrite )
	{
		//JTTS_LOG:"TTSPlayer|wait for write"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_423_112_2_18_3_3_40_39,(uint8*)"" );
		SCI_Sleep( 50 );
	}

	while (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
	{
		//JTTS_LOG:"TTSPlayer|pause sleep 100"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_429_112_2_18_3_3_40_40,(uint8*)"" );
		SCI_Sleep( 100 );
 	}
	SCI_TRACE_LOW("[TTS] OutputVoiceToPlayProc g_play_ptr = %x",g_play_ptr);
	if (PNULL == g_play_ptr)
	{
		return TTS_PLAYER_ERR_PARAM;
	}

	SCI_MEMSET( g_play_ptr, 0x00, JTTSPCM_PLAY_BUF_SIZE);	
	SCI_MEMCPY( g_play_ptr, pData, iSize );
	g_playsize = iSize;
	
	g_bWrite = FALSE;

	uTickCount = SCI_GetTickCount();
	//JTTS_LOG:"TTSPlayer|Leave OutputVoiceToPlayProc tickcount = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_441_112_2_18_3_3_40_41,(uint8*)"d", uTickCount );
	g_uPrevTickCount = uTickCount;

	return jtTTS_ERR_NONE;
}

/*****************************************************************************/
//  Description : ejTTSPlayer_Init 播放器初始化
//  Global resource dependence : none
//  Author: yaye.jiang
// params	const char *szCNLib	中文音库
//			const char *szEnLib 英文音库
//			const char *szDMLib 定制库 
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Init( const char *szCNLib, const char *szEnLib, const char *szDMLib )
{
	uint32 errCode;

	//JTTS_LOG:"** ejTTSPlayer_Init in"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_457_112_2_18_3_3_40_42,(uint8*)"" );

	//只能初始化一次 判断 thread id 是否为空
	if ( SCI_INVALID_BLOCK_ID != g_jtts_thread_id )
	{
		//JTTS_LOG:"** Error! TTS is already init"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_462_112_2_18_3_3_40_43,(uint8*)"" );
		return TTS_PLAYER_ERR_ALREADYINIT;
	}
    
	//同步数据 mutex
	g_SyncData.pMutex = SCI_CreateMutex( "JTTS_SYNC_MUTEX", SCI_INHERIT );
	if ( SCI_NULL == g_SyncData.pMutex )
	{
		//JTTS_LOG:"Create Mutex failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_470_112_2_18_3_3_40_44,(uint8*)"" );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	//JTTS_LOG:"TTSPlayer|Create Mutex success"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_473_112_2_18_3_3_40_45,(uint8*)"" );
	
	//播放数据的 mutex
	g_PlayData.pMutex = SCI_CreateMutex( "JTTS_PLAY_MUTEX", SCI_INHERIT );
	if ( SCI_NULL == g_PlayData.pMutex )
	{
		//JTTS_LOG:"TTSPlayer|Create Play Mutex failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_479_112_2_18_3_3_40_46,(uint8*)"" );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	//JTTS_LOG:"TTSPlayer|Create Play Mutex success"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_482_112_2_18_3_3_40_47,(uint8*)"" );

	//设置播放状态为 Idle
	g_SyncData.Status = JTTS_STATUS_IDLE;
	
	//创建事件组
	g_jtts_event = SCI_CreateEvent( "JTTS_GROUP_EVENT" );
	if ( SCI_NULL == g_jtts_event )
	{
		//JTTS_LOG:"Create JTTS_GROUP_EVENT failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_491_112_2_18_3_3_40_48,(uint8*)"" );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_NONE;

	//创建服务线程
	g_jtts_thread_id = SCI_CreateThread( "JTTS_Thread",
						"Q_JTTS",
						JTTS_Thread,
						0,
						0,
						MMITTS_TASK_STACK_SIZE,
						MMITTS_TASK_QUEUE_NUM,
						MMITTS_TASK_PRIORITY,
						SCI_PREEMPT,
						SCI_AUTO_START );

	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		//JTTS_LOG:"TTSPlayer|Create Thread failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_511_112_2_18_3_3_40_49,(uint8*)"" );
	//	SCI_DeleteEvent( g_jtts_event );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	//JTTS_LOG:"TTSPlayer|Create JTTS_Thread success"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_515_112_2_18_3_3_40_50,(uint8*)"" );

	errCode = JTTS_Init( szCNLib, szEnLib, szDMLib );
	if ( TTS_PLAYER_ERR_NONE != errCode )
	{
		//JTTS_LOG:"JTTS_Init Failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_520_112_2_18_3_3_40_51,(uint8*)"" );
		//SCI_DeleteEvent( g_jtts_event );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"ejTTSPlayer_Init success"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_525_112_2_18_3_3_40_52,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description : 退出 ejttsPlayer
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_End(void)
{	
	MMISRV_HANDLE_T         audio_handle = TTS_GetAudiohandle();

	//JTTS_LOG:"TTSPlayer|Entry ejTTSPlayer_End"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_536_112_2_18_3_3_40_53,(uint8*)"");
	if ( g_jtts_thread_id == SCI_INVALID_BLOCK_ID )
	{
		if (PNULL != g_jtts_event)
		{
			SCI_DeleteEvent( g_jtts_event );
			g_jtts_event = PNULL;
		}		
		if ( SCI_NULL != g_SyncData.pMutex )
		{
			SCI_DeleteMutex( g_SyncData.pMutex );
			g_SyncData.pMutex = PNULL;
		}
		if ( SCI_NULL != g_PlayData.pMutex )
		{
			SCI_DeleteMutex( g_PlayData.pMutex );
			g_PlayData.pMutex = PNULL;
		}
		if (PNULL != g_play_ptr)
		{
			SCI_FREE(g_play_ptr);
			g_play_ptr = PNULL;
		}
		if (PNULL != g_playbuffer_ptr)
		{
			SCI_FREE(g_playbuffer_ptr);
			g_playbuffer_ptr = PNULL;
		}
		return TTS_PLAYER_ERR_NOTINIT;
	}

	if (( g_SyncData.Status & JTTS_STATUS_PLAY ) == JTTS_STATUS_PLAY )
	{
		//JTTS_LOG:"TTSPlayer|End Playing"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_559_112_2_18_3_3_40_54,(uint8*)"" );
		return TTS_PLAYER_ERR_PLAYING;
	}

	ENTRY_MUTEX( g_SyncData.pMutex );
	g_SyncData.Status = JTTS_STATUS_EXIT;
	//JTTS_LOG:"TTSPlayer|set exit status"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_565_112_2_18_3_3_40_55,(uint8*)"" );
	LEAVE_MUTEX( g_SyncData.pMutex );

	JTTS_Event_Set( CHILD_EVENT );
	//JTTS_LOG:"TTSPlayer|Set child event"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_569_112_2_18_3_3_40_56,(uint8*)"" );

	JTTS_Event_Wait( PARENT_EVENT );
	//JTTS_LOG:"TTSPlayer|end wait for parent event"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_572_112_2_18_3_3_40_57,(uint8*)"" );

    // 要等thread完全退出再继续运行，否则可能会出现线程重入
	while (SCI_INVALID_BLOCK_ID != g_jtts_thread_id)
    {
        //JTTS_LOG:"TTSPlayer|end wait g_jtts_thread_id = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_577_112_2_18_3_3_40_58,(uint8*)"" );
        SCI_Sleep(2);
    }

	if ( 0 < audio_handle )
	{
		MMISRVMGR_Free(audio_handle);
		TTS_SetAudiohandle(0);
	}

	SCI_DeleteEvent( g_jtts_event );
	g_jtts_event = PNULL;
	if ( SCI_NULL != g_SyncData.pMutex )
	{
		SCI_DeleteMutex( g_SyncData.pMutex );
		g_SyncData.pMutex = PNULL;
	}
	if ( SCI_NULL != g_PlayData.pMutex )
	{
		SCI_DeleteMutex( g_PlayData.pMutex );
		g_PlayData.pMutex = PNULL;
	}
	
	if(g_PlayData.hJTTS != SCI_NULL)
	{
		jtTTS_End(g_PlayData.hJTTS);
		SCI_FREE(g_PlayData.pHeap);
		g_PlayData.pHeap = SCI_NULL;
		g_PlayData.hJTTS = SCI_NULL;
	}
	if (PNULL != g_play_ptr)
	{
		SCI_FREE(g_play_ptr);
		g_play_ptr = PNULL;
	}
	if (PNULL != g_playbuffer_ptr)
	{
		SCI_FREE(g_playbuffer_ptr);
		g_playbuffer_ptr = PNULL;
	}
	//JTTS_LOG:"TTSPlayer|Leave ejTTSPlayer_End"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_608_112_2_18_3_3_40_59,(uint8*)"");
	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description : ejttsplayer 播放
//  Global resource dependence : none
//  Author: yaye.jiang
//jtTTSPlayMode ePlayMode 播放方式
//params char * pcszText 合成文本指针
//JTTSCALLBACKPROCEX lpfnCallback 回调函数
//DWORD dwUserData  
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_Play(jtTTSPlayMode ePlayMode, 
								char * pcszText, 
								JTTSCALLBACKPROCEX lpfnCallback, 
								DWORD dwUserData )
{
	int32 uResult;
	//JTTS_LOG:"TTSPlayer|Entry ejTTSPlayer_Play"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_625_112_2_18_3_3_40_60,(uint8*)"" );

	if (TTS_PLAYER_PLAY_ASYNC != ePlayMode && 
		TTS_PLAYER_PLAY_SYNC != ePlayMode)
	{
		return TTS_PLAYER_ERR_PARAM;
	}
	if ( SCI_NULL == pcszText )
	{
		return TTS_PLAYER_ERR_PARAM;
	}

	if ( strlen( pcszText ) >= 1024 )
	{
		return TTS_PLAYER_ERR_TOO_MORE_TEXT;
	}

	if ( g_jtts_thread_id == SCI_INVALID_BLOCK_ID)
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}
	
	if ( g_SyncData.Status != JTTS_STATUS_IDLE )
	{
		return TTS_PLAYER_ERR_PLAYING;
	}

	//设置 user 数据
	SCI_MEMSET( &myUserData, 0, sizeof(MyUserData));
//	SCI_MEMCPY(myUserData.szTextIn, pcszText, strlen(pcszText));

	myUserData.uTextLen = (uint16)(sizeof(wchar) * MMIAPICOM_Wstrlen((const wchar*)pcszText));
	SCI_MEMCPY(myUserData.szTextIn, pcszText, myUserData.uTextLen);
	//JTTS_LOG:"** ejTTSPlayer_Play uTextLen = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_658_112_2_18_3_3_40_61,(uint8*)"d", myUserData.uTextLen);

	//设置命令状态为初始化
	ENTRY_MUTEX( g_SyncData.pMutex );
	if ( g_SyncData.Command.nStatus != JTTS_COMMAND_STATUS_NONE )
	{
		//JTTS_LOG:"** ejTTSPlayer_Play commamd status =%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_664_112_2_18_3_3_40_62,(uint8*)"d",g_SyncData.Command.nStatus);
		LEAVE_MUTEX( g_SyncData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_INIT;
	LEAVE_MUTEX( g_SyncData.pMutex );

	//设置参数为异步播放
	ENTRY_MUTEX( g_SyncData.pMutex );
	if (TTS_PLAYER_PLAY_ASYNC == ePlayMode)
	{
		g_SyncData.Command.nSync = JTTS_COMMAND_ASYNC;
	}
	else if (TTS_PLAYER_PLAY_SYNC == ePlayMode)
	{
		g_SyncData.Command.nSync = JTTS_COMMAND_SYNC;
	}

	g_SyncData.Command.nCommand = JTTS_COMMAND_PLAY;
	g_SyncData.Command.Parameter.Play.lpfnCallback	= lpfnCallback;
	g_SyncData.Command.Parameter.Play.dwUserData	= dwUserData;
	LEAVE_MUTEX( g_SyncData.pMutex );

	JTTS_Event_Set( CHILD_EVENT );

	//JTTS_LOG:"TTSPlayer|Leave ejTTSPlayer_Play"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_689_112_2_18_3_3_40_63,(uint8*)"" );

	if (JTTS_COMMAND_SYNC == g_SyncData.Command.nSync)
	{
		uResult = JTTS_Event_Wait( PARENT_EVENT );
		if ( uResult != SCI_SUCCESS )
		{
			//JTTS_LOG:"TTSPlayer|wait parent event failed"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_696_112_2_18_3_3_40_64,(uint8*)"" );
			return TTS_PLAYER_ERR_DONOTHING;
		}
	}

	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description : ejttsplayer 将合成数据保存到文件
//  Global resource dependence : none
//  Author: yaye.jiang
//params char * pcszText 合成文本指针
//char* pcszFileName 保存到文件名字
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_PlayToFile( char *pszText, const char* pszFileName)
{
	uint32		dwErrCode;
	
	//JTTS_LOG:"Entry ejTTSPlayer_PlayToFile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_713_112_2_18_3_3_41_65,(uint8*)"" );
	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		//JTTS_LOG:"TTSPlayer|Invalid thread id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_716_112_2_18_3_3_41_66,(uint8*)"" );
		return TTS_PLAYER_ERR_NOTINIT;
	}	
	
	if(pszText == NULL)
	{
		//JTTS_LOG:"TTSPlayer|Invalid input text"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_722_112_2_18_3_3_41_67,(uint8*)"" );
		return TTS_PLAYER_ERR_PARAM;
	}

	if ( strlen( pszText ) >= 1024 )
	{
		return TTS_PLAYER_ERR_PARAM;
	}
	
	if ( g_SyncData.Status != JTTS_STATUS_IDLE )
	{
		return TTS_PLAYER_ERR_PLAYING;
	}
	
	dwErrCode = JTTS_PlayToFile( pszText, pszFileName );
	
	//JTTS_LOG:"TTSPlayer|Leave ejTTSPlayer_PlayToFile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_738_112_2_18_3_3_41_68,(uint8*)"" );
	return dwErrCode;
}

/*****************************************************************************/
//  Description :ejttsplayer 停止
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Stop(jtTTSStopMode eStopMode)
{
	//MMISRV_HANDLE_T  audio_handle = TTS_GetAudiohandle();	

	//JTTS_LOG:"TTSPlayer|entry ejTTSPlayer_Stop g_jtts_thread_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_749_112_2_18_3_3_41_69,(uint8*)"d",g_jtts_thread_id );
	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}

	//设置状态为停止
	//JTTS_LOG:"TTSPlayer|Status is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_756_112_2_18_3_3_41_70,(uint8*)"d", g_SyncData.Status );
	ENTRY_MUTEX( g_SyncData.pMutex );
	if ( g_SyncData.Status != JTTS_STATUS_IDLE )
	{
		if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
		{
			ejTTSPlayer_Resume();
		}
		
		//JTTS_LOG:"TTSPlayer|Stop SynthText"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_765_112_2_18_3_3_41_71,(uint8*)"" );
		jtTTS_SynthStop(g_PlayData.hJTTS);

		LEAVE_MUTEX( g_SyncData.pMutex );

 		while ( eStopMode == TTS_PLAYER_STOP_BLOCK &&
				g_SyncData.Status != JTTS_STATUS_IDLE)
 		{
 			SCI_Sleep( 100 );
 			//JTTS_LOG:"TTSPlayer|Sleep 100 wait for play finish"
 			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_774_112_2_18_3_3_41_72,(uint8*)"" );
 		}
		
		g_playsize = 0;
		g_bplayed = TRUE;
		g_bWrite = TRUE; 	
		
	}
	else
	{
		LEAVE_MUTEX( g_SyncData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"TTSPlayer|leave ejTTSPlayer_Stop"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_788_112_2_18_3_3_41_73,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description :ejttsplayer 继续播放
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Resume(void)
{		
	MMISRV_HANDLE_T  audio_handle = TTS_GetAudiohandle();
	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}

	//如果现在没有暂停状态 那么可以继续播放
	if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) != JTTS_STATUS_PAUSE )
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//如果有播放状态，则可以去掉暂停状态
	//JTTS_LOG:"TTSPlayer|ejTTSPlayer_Resume status=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_810_112_2_18_3_3_41_74,(uint8*)"d", g_SyncData.Status );
	if (( g_SyncData.Status & JTTS_STATUS_PLAY ) == JTTS_STATUS_PLAY )
	{
		//JTTS_LOG:"TTSPlayer|Set ~Pause status"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_813_112_2_18_3_3_41_75,(uint8*)"" );
		g_SyncData.Status &= ~JTTS_STATUS_PAUSE;
		
		g_bWrite = TRUE;
		g_bplayed = FALSE;

 		if ( 0 < audio_handle )
 		{
 			MMISRVAUD_Resume(audio_handle); 			
 		}
	}

	//JTTS_LOG:"TTSPlayer|ejTTSPlayer_Resume status=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_825_112_2_18_3_3_41_76,(uint8*)"d", g_SyncData.Status );

	//JTTS_LOG:"TTSPlayer|Leave ejTTSPlayer_Resume"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_827_112_2_18_3_3_41_77,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description :ejttsplayer 暂停
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Pause(void)
{	

    MMISRV_HANDLE_T  audio_handle = TTS_GetAudiohandle();

	//JTTS_LOG:"TTSPlayer|Entry ejTTSPlayer_Pause"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_839_112_2_18_3_3_41_78,(uint8*)"" );
	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}

	if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//如果现在是播放状态，则添加暂停状态
	//JTTS_LOG:"TTSPlayer|begin status = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_851_112_2_18_3_3_41_79,(uint8*)"d", g_SyncData.Status );
	if (( g_SyncData.Status & JTTS_STATUS_PLAY ) == JTTS_STATUS_PLAY )
	{
		//JTTS_LOG:"TTSPlayer|Set pause status"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_854_112_2_18_3_3_41_80,(uint8*)"" );
		g_SyncData.Status |= JTTS_STATUS_PAUSE;
		
 		if ( 0 < audio_handle )
 		{			
 			MMISRVAUD_Pause(audio_handle);			
 		} 
 		
 		g_playsize = 0;
		//g_bplayed = TRUE;
		//g_bWrite = FALSE;
	}
	else
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"TTSPlayer|Leave ejTTSPlayer_Pause"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_871_112_2_18_3_3_41_81,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description :ejttsplayer 获取当前的状态
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 *piStatus 状态
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_GetStatus(int *piStatus)
{
	//SCI_TRACE_LOW:"ejTTSPlayer_GetStatus g_jtts_thread_id = %d, g_SyncData.Status = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_881_112_2_18_3_3_41_82,(uint8*)"dd",g_jtts_thread_id,g_SyncData.Status);
	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		*piStatus = TTS_PLAYER_NOT_INIT;
		return TTS_PLAYER_ERR_NONE;
	}

	//获取当前的状态是 播放 or 暂停 or 停止,默认为停止
	if ( g_SyncData.Status == JTTS_STATUS_PLAY )
	{
		*piStatus = TTS_PLAYER_PLAYING;
		return TTS_PLAYER_ERR_NONE;
	}

	if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
	{
		*piStatus = TTS_PLAYER_PAUSE;
		return TTS_PLAYER_ERR_NONE;
	}

	if ( g_SyncData.Status == JTTS_STATUS_STOP )
	{
		*piStatus = TTS_PLAYER_IDLE;
		return TTS_PLAYER_ERR_NONE;
	}

	*piStatus = TTS_PLAYER_IDLE;

	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description :ejttsPlayer获取参数
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam 参数类型
//unsigned long *pdwValue 参数值
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_GetParam( JTTSPARAM nParam, unsigned long *pdwValue)
{
	uint32		dwErrCode = 0;
	
	if ( g_jtts_thread_id == SCI_INVALID_BLOCK_ID )
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}	
	
	dwErrCode = jtTTS_GetParam( g_PlayData.hJTTS, nParam, (long *)pdwValue );
	
	return dwErrCode;
}

/*****************************************************************************/
//  Description :ejttsPlayer 设置参数
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam 参数类型
//DWORD dwValue 参数值
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_SetParam(JTTSPARAM nParam, unsigned long dwValue)
{
	uint32 dwErrCode = 0;

	if ( SCI_INVALID_BLOCK_ID == g_jtts_thread_id )
	{
		return TTS_PLAYER_ERR_NOTINIT;
	}

	if (jtTTS_PARAM_OUTPUT_SIZE == nParam || 
		jtTTS_PARAM_INPUTTXT_MODE == nParam ||
		jtTTS_PARAM_OUTPUT_CALLBACK == nParam ||
		jtTTS_PARAM_WAV_FORMAT == nParam ||
		jtTTS_PARAM_INPUT_CALLBACK == nParam ||
		jtTTS_PARAM_INPUTTXT_MODE == nParam ||
		jtTTS_PARAM_OUTPUT_CALLBACK == nParam ||
		jtTTS_PARAM_SLEEP_CALLBACK == nParam ||
		jtTTS_PARAM_PARAM_CALLBACK == nParam)
	{
		return jtTTS_ERR_INPUT_PARAM;
	}

	dwErrCode = jtTTS_SetParam(g_PlayData.hJTTS, nParam, dwValue);
	
	return dwErrCode;
}


/*****************************************************************************/
//  Description : 服务线程
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL void JTTS_Thread( uint32 argc, void * argv )
{
	uint32 dwErrCode;
	uint32 uResult;
	xSignalHeaderRec *psig = SCI_NULL;

	//JTTS_LOG:"TTSPlayer|Entry JTTS_Thread"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_973_112_2_18_3_3_41_83,(uint8*)"" );

	while ( g_SyncData.Status != JTTS_STATUS_EXIT )
	{
		//JTTS_LOG:"** JTTTS_Thread g_SyncData.Status = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_977_112_2_18_3_3_41_84,(uint8*)"d",g_SyncData.Status);
		// clear all the signals to avoid the queue full
		while ( TRUE ) /*lint !e716*/
		{
			MMI_PEEK_SIGNAL( psig, SCI_IdentifyThread());
			if ( SCI_NULL == psig )
			{
				//JTTS_LOG:"JTTS: SigCode is NULL"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_984_112_2_18_3_3_41_85,(uint8*)"" );
				break;
			}
			//JTTS_LOG:"JTTS: SigCode = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_987_112_2_18_3_3_41_86,(uint8*)"d", psig->SignalCode );
			MMI_FREE_SIGNAL(psig);
			psig = SCI_NULL;
		}

		// wait for child event
		uResult = JTTS_Event_Wait( CHILD_EVENT );
		if ( uResult != SCI_SUCCESS )
		{
			//JTTS_LOG:"TTSPlayer|wait child event failed"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_996_112_2_18_3_3_41_87,(uint8*)"" );
			break;
		}
		//JTTS_LOG:"TTSPlayer|Event Wait Child event success"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_999_112_2_18_3_3_41_88,(uint8*)"" );

		if ( g_SyncData.Status == JTTS_STATUS_EXIT )
		{
			//JTTS_LOG:"TTSPlayer|status exit"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1003_112_2_18_3_3_41_89,(uint8*)"" );
			break;
		}

		//设置状态
		ENTRY_MUTEX( g_SyncData.pMutex );
		if ( g_SyncData.Status == JTTS_STATUS_STOP )
		{
			g_SyncData.Status = JTTS_STATUS_IDLE;
			//JTTS_LOG:"TTSPlayer|stop status -> idle status %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1012_112_2_18_3_3_41_90,(uint8*)"d", g_SyncData.Status );
		}
		LEAVE_MUTEX( g_SyncData.pMutex );

		ENTRY_MUTEX( g_SyncData.pMutex );
		if ( g_SyncData.Command.nStatus != JTTS_COMMAND_STATUS_INIT )
		{
			g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_NONE;
			LEAVE_MUTEX( g_SyncData.pMutex );
			//JTTS_LOG:"TTSPlayer|command status is not init, continue"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1021_112_2_18_3_3_41_91,(uint8*)"" );
			continue;
		}
		g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_HANDLING;
		LEAVE_MUTEX( g_SyncData.pMutex );

		switch ( g_SyncData.Command.nCommand )
		{
		case JTTS_COMMAND_PLAY:
			// Play 
			//JTTS_LOG:"TTSPlayer|Case TTSPlayer|COMMAND PLAY "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1031_112_2_18_3_3_41_92,(uint8*)"" );
			dwErrCode = JTTS_Play( myUserData.szTextIn,
				g_SyncData.Command.Parameter.Play.lpfnCallback,
				g_SyncData.Command.Parameter.Play.dwUserData );
			//JTTS_LOG:"TTSPlayer|ERROR CODE = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1035_112_2_18_3_3_41_93,(uint8*)"d", dwErrCode );
			break;

		default:
			dwErrCode = TTS_PLAYER_ERR_PARAM;
			break;
		}

		ENTRY_MUTEX( g_SyncData.pMutex );
		//如果不在处理中
		if ( g_SyncData.Command.nStatus != JTTS_COMMAND_STATUS_HANDLING )
		{
			g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_NONE;			
			if (dwErrCode == TTS_PLAYER_ERR_NONE 
				&& NULL != g_SyncData.Command.Parameter.Play.lpfnCallback)
			{
				g_SyncData.Command.Parameter.Play.lpfnCallback(TTS_PLAYER_NOTIFY_END, 0L, PNULL);
			}
		
			LEAVE_MUTEX( g_SyncData.pMutex );
			continue;
		}
		g_SyncData.Command.ErrCode = dwErrCode;
		//如果是同步
		if ( g_SyncData.Command.nSync == JTTS_COMMAND_SYNC )
		{
			//g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_HANDLED;
			g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_NONE;
			JTTS_Event_Set( PARENT_EVENT );
		}
		else//如果是异步
		{
			g_SyncData.Command.nStatus = JTTS_COMMAND_STATUS_NONE;
		}

		if (dwErrCode == TTS_PLAYER_ERR_NONE 
			&& NULL != g_SyncData.Command.Parameter.Play.lpfnCallback)
		{
			g_SyncData.Command.Parameter.Play.lpfnCallback(TTS_PLAYER_NOTIFY_END, 0L, PNULL);
		}
		LEAVE_MUTEX( g_SyncData.pMutex );
	}


	JTTS_Event_Set( PARENT_EVENT );

	//JTTS_LOG:"TTSPlayer|Exit JTTS_Thread"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1069_112_2_18_3_3_41_94,(uint8*)"" );

	g_jtts_thread_id = SCI_INVALID_BLOCK_ID;
	SCI_ThreadExit();

	return;
}
/*****************************************************************************/
//  Description : jtts 初始化
//  Global resource dependence : none
//  Author: yaye.jiang
//const char* szCNLib 中文音库
//const char* szENLib 英文音库
//const char* szDMLib DM音库
/*****************************************************************************/
LOCAL uint32 JTTS_Init(const char* szCNLib, const char* szENLib, const char* szDMLib)
{
	uint32 dwErrCode;
	int32 lSize = 0;
	uint32 hTTS;			// handle to TTS engine 2010-9-10 uint64 -> uint64
	void* pGlobalData = PNULL;

	ENTRY_MUTEX( g_PlayData.pMutex );
	dwErrCode = jtTTS_GetExtBufSize(szCNLib,szENLib,szDMLib,&lSize );
	
	//JTTS_LOG:"TTSPlayer|lSize=%d, dwErrCode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1092_112_2_18_3_3_41_95,(uint8*)"dd", lSize, dwErrCode );

	if (jtTTS_ERR_NONE != dwErrCode
		|| 0 == lSize)
	{
		LEAVE_MUTEX( g_PlayData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	pGlobalData = SCI_ALLOCA( lSize );
	if ( SCI_NULL == pGlobalData )
	{
		//JTTS_LOG:"TTSPlayer|alloc memory failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1102_112_2_18_3_3_41_96,(uint8*)"" );
		LEAVE_MUTEX( g_PlayData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	SCI_TRACE_LOW("TTS JTTS_Init g_play_ptr = %x",g_play_ptr);
	g_play_ptr = SCI_ALLOCA( JTTSPCM_PLAY_BUF_SIZE );
	SCI_TRACE_LOW("TTS JTTS_Init g_play_ptr = %x",g_play_ptr);
	if ( SCI_NULL == g_play_ptr )
	{        
		SCI_FREE(pGlobalData);
		//JTTS_LOG:"TTSPlayer|alloc memory failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1102_112_2_18_3_3_41_96,(uint8*)"" );
		LEAVE_MUTEX( g_PlayData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	SCI_TRACE_LOW("TTS JTTS_Init g_playbuffer_ptr = %x",g_playbuffer_ptr);
	g_playbuffer_ptr = SCI_ALLOCA( JTTSPCM_PLAY_BUF_SIZE );
	SCI_TRACE_LOW("TTS JTTS_Init g_playbuffer_ptr = %x",g_playbuffer_ptr);
	if ( SCI_NULL == g_playbuffer_ptr )
	{        
		SCI_FREE(pGlobalData);
		SCI_FREE(g_play_ptr);
		g_play_ptr = PNULL;
		//JTTS_LOG:"TTSPlayer|alloc memory failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1102_112_2_18_3_3_41_96,(uint8*)"" );
		LEAVE_MUTEX( g_PlayData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"TTSPlayer|global data address is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1106_112_2_18_3_3_41_97,(uint8*)"d", (uint32)pGlobalData );
	
	//tts 初始化
	dwErrCode = jtTTS_Init( szCNLib, szENLib, szDMLib, &hTTS, pGlobalData );
	//JTTS_LOG:"TTSPlayer|Init errcode=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1110_112_2_18_3_3_41_98,(uint8*)"d", dwErrCode );

	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		SCI_FREE(g_play_ptr);
		g_play_ptr = PNULL;
		SCI_FREE(g_playbuffer_ptr);
		g_playbuffer_ptr = PNULL;
		SCI_FREE( pGlobalData );
		LEAVE_MUTEX( g_PlayData.pMutex );
		return (TTS_PLAYER_ERR_MORE)dwErrCode;
	}

	g_PlayData.hJTTS = hTTS;
	g_PlayData.pHeap = pGlobalData;

	LEAVE_MUTEX( g_PlayData.pMutex );


	return TTS_PLAYER_ERR_NONE;
}

/*****************************************************************************/
//  Description : jtts 停止
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Stop(void)
{
	MMISRV_HANDLE_T  audio_handle = TTS_GetAudiohandle();
	//JTTS_LOG:"TTSPlayer|entry JTTS_Stop audio_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1134_112_2_18_3_3_41_99,(uint8*)"d", audio_handle );
    
	//设置状态为停止
	//JTTS_LOG:"TTSPlayer|Status JTTS_Stop is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1137_112_2_18_3_3_41_100,(uint8*)"d", g_SyncData.Status );
	ENTRY_MUTEX( g_SyncData.pMutex );
	if ( g_SyncData.Status != JTTS_STATUS_IDLE )
	{
// 		if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
// 		{
// 			JTTS_Resume();
// 		}		
// 			
// 				
		//JTTS_LOG:"TTSPlayer|JTTS_Stop g_playsize 0 g_bplayed True g_bWrite false"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1147_112_2_18_3_3_41_101,(uint8*)"" );
		if (0 < audio_handle)
		{
			MMISRVAUD_Stop(audio_handle);	
			MMISRVMGR_Free(audio_handle);
			TTS_SetAudiohandle(0);
		}
		
		//JTTS_LOG:"TTSPlayer|Stop SynthText"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1155_112_2_18_3_3_41_102,(uint8*)"" );
		jtTTS_SynthStop(g_PlayData.hJTTS);
 		g_SyncData.Status = JTTS_STATUS_STOP;	
		LEAVE_MUTEX( g_SyncData.pMutex );
		
		
	}
	else
	{
		LEAVE_MUTEX( g_SyncData.pMutex );
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"TTSPlayer|leave JTTS_Stop;"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1168_112_2_18_3_3_41_103,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;

}

/*****************************************************************************/
//  Description : jtts 恢复
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Resume(void)
{
	MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
	
	//JTTS_LOG:"TTSPlayer|JTTS_Resume audio_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1180_112_2_18_3_3_41_104,(uint8*)"d",audio_handle);

	//如果现在没有暂停状态 那么可以继续播放
	if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) != JTTS_STATUS_PAUSE )
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//如果有播放状态，则可以去掉暂停状态
	//JTTS_LOG:"TTSPlayer|JTTS_Resume status=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1189_112_2_18_3_3_41_105,(uint8*)"d", g_SyncData.Status );
	
	//JTTS_LOG:"TTSPlayer|Set ~Pause status"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1191_112_2_18_3_3_41_106,(uint8*)"" );
	g_SyncData.Status = JTTS_STATUS_PLAY;	
	g_bplayed = FALSE;	
	
 	if ( 0 < audio_handle )
 	{
 		MMISRVAUD_Resume(audio_handle); 			
 	}
	

	//JTTS_LOG:"TTSPlayer|JTTS_Resume status=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1201_112_2_18_3_3_41_107,(uint8*)"d", g_SyncData.Status );

	//JTTS_LOG:"TTSPlayer|Leave JTTS_Resume"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1203_112_2_18_3_3_41_108,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;

}

/*****************************************************************************/
//  Description : jtts 暂停
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_Pause(void)
{
	MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();

	//JTTS_LOG:"TTSPlayer|Entry JTTS_Pause audio_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1215_112_2_18_3_3_42_109,(uint8*)"d" ,audio_handle );

	if (( g_SyncData.Status & JTTS_STATUS_PAUSE ) == JTTS_STATUS_PAUSE )
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//如果现在是播放状态，则添加暂停状态
	//JTTS_LOG:"TTSPlayer|begin JTTS_Pause status = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1223_112_2_18_3_3_42_110,(uint8*)"d", g_SyncData.Status );
	if (( g_SyncData.Status & JTTS_STATUS_PLAY ) == JTTS_STATUS_PLAY )
	{
		//JTTS_LOG:"TTSPlayer|JTTS_Pause Set pause status"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1226_112_2_18_3_3_42_111,(uint8*)"" );
		g_SyncData.Status = JTTS_STATUS_PAUSE;
		g_bplayed = FALSE;		
 		if ( 0 < audio_handle )
 		{			
 			MMISRVAUD_Pause(audio_handle);			
 		} 		
 		
	}
	else
	{
		return TTS_PLAYER_ERR_DONOTHING;
	}

	//JTTS_LOG:"TTSPlayer|Leave JTTS_Pause"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1240_112_2_18_3_3_42_112,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;

}

/*****************************************************************************/
//  Description : jtts 播放
//  Global resource dependence : none
//  Author: yaye.jiang
//char * pcszText 播放文本指针
//JTTSCALLBACKPROCEX lpfnCallback 回调函数
//DWORD dwUserData 用户数据
/*****************************************************************************/
LOCAL uint32 JTTS_Play(char * pcszText, JTTSCALLBACKPROCEX lpfnCallback, DWORD dwUserData)
{
	uint32			dwErrCode;
	uint32 uiSampleRate = 0;
	uint16 nBitsPerSample = 0;
	uint32 size = 0;
//	int32 lVolume = 0;	
	uint32 uTickCount = 0;
	MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
	MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T        audio_srv = {0};
//	uint32 drv_handle = 0;

	

	//JTTS_LOG:"TTSPlayer|Entry JTTS_Play"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1266_112_2_18_3_3_42_113,(uint8*)"" );

	if(pcszText == SCI_NULL)
	{
		//JTTS_LOG:"TTSPlayer|Invalid input text"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1270_112_2_18_3_3_42_114,(uint8*)"" );
		if (NULL != lpfnCallback)
		{
			lpfnCallback(TTS_PLAYER_NOTIFY_ERROR, TTS_PLAYER_ERR_PARAM, dwUserData);
		}
		return TTS_PLAYER_ERR_PARAM;
	}
	
	// 设置为播放状态
	g_SyncData.Status = JTTS_STATUS_PLAY;

	dwErrCode = GetAudioFormat(g_PlayData.hJTTS, &uiSampleRate, &nBitsPerSample);
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1284_112_2_18_3_3_42_115,(uint8*)"" );
	}
	//JTTS_LOG:"TTSPlayer|SampleRate=%d Bitpersample=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1286_112_2_18_3_3_42_116,(uint8*)"dd", uiSampleRate, nBitsPerSample );
	// pcm 格式头	
	g_head_info.bType = SCI_TRUE;
	memset(&g_head_info.pad,0,3); 
	g_head_info.unWavStreamData.tWavFormatInfo.eSubtype = WAV_PCM_BIGENDIAN;
	g_head_info.unWavStreamData.tWavFormatInfo.uiSampleRate = uiSampleRate;
	g_head_info.unWavStreamData.tWavFormatInfo.uiBitPerSample = nBitsPerSample;
	g_head_info.unWavStreamData.tWavFormatInfo.uiChannelNum = 1;
	g_head_info.unWavStreamData.tWavFormatInfo.uiDataSize = sizeof(WAV_STREAM_BUFFER_T);
	g_head_info.unWavStreamData.tWavFormatInfo.uiBlockAlign = 0;
	g_head_info.unWavStreamData.tWavFormatInfo.pvOtherInfo = 0;	
	
	req.notify   = JTTS_AudioNotifyCallback;
    
	switch(TTS_GetTTSRingType())
	{
	case TTS_RING_TYPE_CALL:
		req.pri      = MMISRVAUD_PRI_HIGH;
		break;
	case TTS_RING_TYPE_MENU:
		req.pri      = MMISRVAUD_PRI_LOW_P10;
		req.is_auto_free = TRUE;
		break;
	default:
		req.pri      = MMISRVAUD_PRI_NORMAL;
		break;
	}    
    
    audio_srv.volume  = TTS_GetTTSVolume();
	audio_srv.volume_type = (MMISRVAUD_VOLUME_TYPE_E)TTS_GetTTSVolumeType();
    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
    audio_srv.info.streaming.data = (uint8 *)(&g_head_info);
    audio_srv.info.streaming.data_len = sizeof(g_head_info);
    audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_WAVE;
    audio_srv.info.streaming.cb = (void *)JTTS_AudioCallBackFunc; /*lint !e611*/
    audio_srv.all_support_route = TTS_GetRoute();
    TTS_SetManualStop(FALSE);
	

	if (0 == audio_handle)
	{	
		audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

		//JTTS_LOG:"TTSPlayer|Entry JTTS_Play audio_handle = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1334_112_2_18_3_3_42_117,(uint8*)"d" ,audio_handle );
		if (0 == audio_handle)
		{
			if (NULL != lpfnCallback)
			{
				lpfnCallback(TTS_PLAYER_NOTIFY_ERROR, TTS_PLAYER_ERR_AUDIO_DEVICE, dwUserData);
			}						
			ENTRY_MUTEX( g_SyncData.pMutex );
			g_SyncData.Status = JTTS_STATUS_IDLE;
			LEAVE_MUTEX( g_SyncData.pMutex );
			return TTS_PLAYER_ERR_AUDIO_DEVICE;
		}
		else
		{
			TTS_SetAudiohandle(audio_handle);
		}	
	}
	
	if (NULL != lpfnCallback)
	{
		lpfnCallback(TTS_PLAYER_NOTIFY_BEGIN, 0L, 0);
	}
	//JTTS_LOG:"TTSPlayer|Entry JTTS_Play g_playsize = 0; g_bplayed = FALSE; g_bWrite = TRUE"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1348_112_2_18_3_3_42_118,(uint8*)"" );
	g_playsize = 0;
	g_bplayed = FALSE;
	g_bWrite = TRUE;

	//JTTS_LOG:"TTSPlayer|tts handle =%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1353_112_2_18_3_3_42_119,(uint8*)"d", g_PlayData.hJTTS);

    jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_UNICODE);
	// 设置输入方式
	jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
	// 设置音频输出回调
	jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (int32)OutputVoiceToPlayProc);
	// 设置用户数据
	jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_CALLBACK_USERDATA, (int32)&myUserData);	
	//设置数字读取
	jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_DIGIT_MODE, TTS_GetDigitMode());
	//设置符号读取
	jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_PUNC_MODE, TTS_GetPuncMode());
	//播放 启动播放回调函数
	// add by yeyan 2009-6-3
	// 调用播放接口失败则返回
	s_is_stop_by_srv = FALSE;
	if (MMISRVAUD_RET_ERROR == MMISRVAUD_Play(audio_handle, 0))
	{		
	    MMISRVMGR_Free(audio_handle);
	    TTS_SetAudiohandle(0);
		g_bplayed = TRUE;
		if (NULL != lpfnCallback)
		{
			lpfnCallback(TTS_PLAYER_NOTIFY_ERROR, TTS_PLAYER_ERR_AUDIO_DEVICE, dwUserData);
		}		
		ENTRY_MUTEX( g_SyncData.pMutex );
		g_SyncData.Status = JTTS_STATUS_IDLE;
		LEAVE_MUTEX( g_SyncData.pMutex );	
		return TTS_PLAYER_ERR_AUDIO_DEVICE;
	}
	// add by yeyan 2009-6-3 end
//     s_is_audio_running = TRUE;
    MMI_SetFreq(FREQ_INDEX_APP_HIGH);

	//合成数据
	uTickCount = SCI_GetTickCount();
	//JTTS_LOG:"TTSPlayer|Ready to Call jtTTS_SynthesizeText tick = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1385_112_2_18_3_3_42_120,(uint8*)"d", uTickCount);
	dwErrCode = jtTTS_SynthesizeText(g_PlayData.hJTTS, pcszText, MMIAPICOM_Wstrlen((const wchar*)pcszText)*sizeof(uint16));
	//JTTS_LOG:"TTSPlayer|jtTTS_SynthesizeText dwErrCode =%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1387_112_2_18_3_3_42_121,(uint8*)"d", dwErrCode );
	
	MMI_RestoreFreq();
    
	if (jtTTS_ERR_NONE != dwErrCode)
	{
		g_playsize = 0 ;
		TTS_SetManualStop(TRUE);
	}

	while (g_playsize != 0
		&& !TTS_GetManualStop()
		&& !s_is_stop_by_srv)
	{
		//JTTS_LOG:"TTSPlayer|play wait for play finish"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1394_112_2_18_3_3_42_122,(uint8*)"" );
		SCI_Sleep( 50 );
	}

	g_bplayed = TRUE;

	size = MMISRVAUD_GetDecoderDataLen(audio_handle);

	while (TTS_AUDIO_DECODE_MIN_LEN < size
		&& !TTS_GetManualStop()
		&& !s_is_stop_by_srv)
	{
		//JTTS_LOG:"TTSPlayer|play wait for play all buffer finish %d size %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1405_112_2_18_3_3_42_123,(uint8*)"dd", g_bplayed, size);
		SCI_Sleep( 50 );
		size = MMISRVAUD_GetDecoderDataLen(audio_handle);
	}
   // g_bplayed = TRUE;
	JTTS_LOG( "TTSPlayer|play wait for play all buffer finish %d size %d", g_bplayed, size);
	
	if ( 0 < audio_handle )
	{
		MMISRVAUD_Stop( audio_handle );		
 	}
///	SCI_Sleep( 30 );
	
	MMISRVMGR_Free( audio_handle );
	TTS_SetAudiohandle(0);	
		
	
	//JTTS_LOG:"TTSPlayer|Leave JTTS_Play"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1420_112_2_18_3_3_42_124,(uint8*)"" );

	TTS_SetManualStop(FALSE);
     s_is_stop_by_srv = FALSE; 
	ENTRY_MUTEX( g_SyncData.pMutex );
	g_SyncData.Status = JTTS_STATUS_IDLE;
	LEAVE_MUTEX( g_SyncData.pMutex );

	return TTS_PLAYER_ERR_NONE;

	
}

/*****************************************************************************/
//  Description : 将播放数据写入到文件
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
LOCAL uint32 JTTS_PlayToFile( char *pcszText, const char *pcszFileName )
{
	uint32			dwErrCode;
	// 音频播放设备设置信息
	uint32 nSamplesPerSec = 16000;
	uint16 nBitsPerSample = 16;
	int32  nFormat = jtTTS_FORMAT_PCM_16K16B;

	uint16 *pwszFileName = (uint16 *)pcszFileName;

	uint32 uLen = MMIAPICOM_Wstrlen(pwszFileName);
	//JTTS_LOG:"TTSPlayer|file name len is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1453_112_2_18_3_3_42_125,(uint8*)"d", uLen );
	if ( uLen >= MAX_PATH )
	{
		return TTS_PLAYER_ERR_PARAM;
	}

	//JTTS_LOG:"Entry JTTS_PlayToFile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1459_112_2_18_3_3_42_126,(uint8*)"" );

	if(g_PlayData.hJTTS == SCI_NULL)
	{
		//JTTS_LOG:"TTSPlayer|NOT INIT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1463_112_2_18_3_3_42_127,(uint8*)"" );
		return TTS_PLAYER_ERR_NOTINIT;
	}

	g_SyncData.Status = JTTS_STATUS_PLAY;

	dwErrCode = jtTTS_SetParam( g_PlayData.hJTTS, jtTTS_PARAM_WAV_FORMAT, nFormat );
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Set Wave format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1472_112_2_18_3_3_42_128,(uint8*)"" );
	}
	
	dwErrCode = GetAudioFormat(g_PlayData.hJTTS, &nSamplesPerSec, &nBitsPerSample);
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1478_112_2_18_3_3_42_129,(uint8*)"" );
	}
	//JTTS_LOG:"TTSPlayer|SampleRate=%d Bitpersample=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1480_112_2_18_3_3_42_130,(uint8*)"dd", nSamplesPerSec, nBitsPerSample );
			
	g_filehandle = MMIFILE_CreateFile( pwszFileName, 
		SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, 0, 0 );
	if ( INVALID_HANDLE == g_filehandle )
	{
		g_SyncData.Status = JTTS_STATUS_IDLE;
		//JTTS_LOG:"TTSPlayer|Can not open file"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1487_112_2_18_3_3_42_131,(uint8*)"" );
		return TTS_PLAYER_ERR_OPENFILE;
	}

	/* 设置直接文本输入 */
	dwErrCode = jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1495_112_2_18_3_3_42_132,(uint8*)"" );
	}

	/* 设置音频输出回调 */
	dwErrCode = jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long)OutputVoiceToFileProc);	
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1502_112_2_18_3_3_42_133,(uint8*)"" );
	}

	dwErrCode = jtTTS_SetParam(g_PlayData.hJTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)&myUserData);
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1508_112_2_18_3_3_42_134,(uint8*)"" );
	}

//	dwErrCode = jtTTS_SynthesizeText(g_PlayData.hJTTS, myUserData.szTextIn, strlen(myUserData.szTextIn)*sizeof(char));
	dwErrCode = jtTTS_SynthesizeText(g_PlayData.hJTTS, pcszText, MMIAPICOM_Wstrlen((const wchar*)pcszText)*sizeof(uint16));
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1515_112_2_18_3_3_42_135,(uint8*)"" );
	}

	MMIFILE_CloseFile( g_filehandle );
	g_filehandle = INVALID_HANDLE;

	ENTRY_MUTEX( g_SyncData.pMutex );
	g_SyncData.Status = JTTS_STATUS_IDLE;
	LEAVE_MUTEX( g_SyncData.pMutex );

	//JTTS_LOG:"TTSPlayer|Leave JTTS_Playtofile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1525_112_2_18_3_3_42_136,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}


/*****************************************************************************/
//  Description : 激活事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Set(uint32 event_id)
{
	uint32 status_E = SCI_ERROR;
	
	if(event_id <= MAX_SUPPORT_HANDLER_NUM)
	{
		
		if (SCI_NULL == g_jtts_event)
		{
			return 0x07;
		}

		status_E = SCI_SetEvent(g_jtts_event, (1<<event_id), SCI_OR);			
		
	}
	
	SCI_TRACE_LOW("[TTS] JTTS_Event_Set status_E = %d", status_E);
	return status_E;
}

/*****************************************************************************/
//  Description : 清除事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Clr(uint32 event_id)
{
	uint32 status_E = SCI_ERROR;
	
	if(event_id <= MAX_SUPPORT_HANDLER_NUM)
	{
	
		if (SCI_NULL == g_jtts_event)
		{
			return 0x07;
		}
				
		status_E = SCI_SetEvent(g_jtts_event, (uint32)(~(1<<event_id)), SCI_AND);/*lint !e502*/		
	
	}
	SCI_TRACE_LOW("[TTS] JTTS_Event_Clr status_E = %d", status_E);
	return status_E;
}

/*****************************************************************************/
//  Description : 挂起事件
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 JTTS_Event_Wait(uint32 event_id)
{
	uint32 actual_event,status = SCI_ERROR;
	
	if(PNULL != g_jtts_event && event_id <= MAX_SUPPORT_HANDLER_NUM)
	{	
	
	
		status = SCI_GetEvent( g_jtts_event, 
          						 (1 << event_id), 
          						 SCI_AND_CLEAR,
          						 &actual_event, 
          						 SCI_WAIT_FOREVER);
	}
   return status;
}

/*****************************************************************************/
//  Description : 获得播放器当前音频格式
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 event_id 事件 id
/*****************************************************************************/
LOCAL uint32 GetAudioFormat(DWORD dwHandle, uint32* pnSamplesPerSec, uint16* pnBitsPerSample)
{
	uint32 err = TTS_PLAYER_ERR_NONE;
	int32	nFormat = 0;
	
	err = jtTTS_GetParam(dwHandle, jtTTS_PARAM_WAV_FORMAT, &nFormat);
	if(err != TTS_PLAYER_ERR_NONE)
		return TTS_PLAYER_ERR_PARAM;
	
	switch(nFormat)
	{
	case jtTTS_FORMAT_PCM_NORMAL:
	case jtTTS_FORMAT_PCM_16K16B:
		*pnSamplesPerSec = 16000;
		*pnBitsPerSample = 16;
		break;
	case jtTTS_FORMAT_VOX_6K:
		*pnSamplesPerSec = 6000;
		*pnBitsPerSample = 4;
		break;
	case jtTTS_FORMAT_VOX_8K:
		*pnSamplesPerSec = 8000;
		*pnBitsPerSample = 4;
		break;
	case jtTTS_FORMAT_ALAW_8K:
	case jtTTS_FORMAT_uLAW_8K:
	case jtTTS_FORMAT_PCM_8K8B:	
		*pnSamplesPerSec = 8000;
		*pnBitsPerSample = 8;
		break;
	case jtTTS_FORMAT_PCM_8K16B:
		*pnSamplesPerSec = 8000;
		*pnBitsPerSample = 16;
		break;
	case jtTTS_FORMAT_PCM_16K8B:
		*pnSamplesPerSec = 16000;
		*pnBitsPerSample = 8;
		break;
	case jtTTS_FORMAT_PCM_11K8B:
		*pnSamplesPerSec = 11025;
		*pnBitsPerSample = 8;
		break;
	case jtTTS_FORMAT_PCM_11K16B:
		*pnSamplesPerSec = 11025;
		*pnBitsPerSample = 16;
		break;
	case jtTTS_FORMAT_PCM_22K8B:
		*pnSamplesPerSec = 22050;
		*pnBitsPerSample = 8;
		break;
	case jtTTS_FORMAT_PCM_22K16B:
		*pnSamplesPerSec = 22050;
		*pnBitsPerSample = 16;
		break;
	case jtTTS_FORMAT_PCM_44K8B:
		*pnSamplesPerSec = 44100;
		*pnBitsPerSample = 8;
		break;
	case jtTTS_FORMAT_PCM_44K16B:
		*pnSamplesPerSec = 44100;
		*pnBitsPerSample = 16;
		break;
	default:
		return TTS_PLAYER_ERR_PARAM;		
	}
	
	return TTS_PLAYER_ERR_NONE;
}


/*****************************************************************************/
//  Description : notify handle function for TTS
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JTTS_AudioNotifyCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{	
	MMISRVAUD_REPORT_T *report_ptr = PNULL;
	//SCI_TRACE_LOW:"JTTS_AudioNotifyCallback enter"
	
	if (PNULL != param
		&& MMISRVMGR_NOTIFY_EXT == param->event)
	{
		report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(PNULL != report_ptr
			&& MMISRVAUD_REPORT_END == report_ptr->report)
         {                
				s_is_stop_by_srv = TRUE;
		}
	}
	
	
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1683_112_2_18_3_3_42_138,(uint8*)"");
	
	return TRUE;
}

/*****************************************************************************/
//  Description : 获得audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_GetAudiohandle(void)
{
	//JTTS_LOG:"TTS_GetAudiohandle s_tts_audio_handle= %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1694_112_2_18_3_3_42_139,(uint8*)"d",s_tts_audio_handle);
    return s_tts_audio_handle;
}

/*****************************************************************************/
//  Description : 设置audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_SetAudiohandle(MMISRV_HANDLE_T audio_handle)
{
    return s_tts_audio_handle = audio_handle;
}

/*****************************************************************************/
//  Description : TTS_IsCommandStatusNone
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsCommandStatusNone(void)
{
	//JTTS_LOG:"** TTS_IsCommandStatusNone nStatus=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1713_112_2_18_3_3_42_140,(uint8*)"d", g_SyncData.Command.nStatus);
	if (JTTS_COMMAND_STATUS_NONE == g_SyncData.Command.nStatus)
	{
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}

/*****************************************************************************/
//  Description : TTS_IsPlayStatus
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsPlayStatus(void)
{
	//JTTS_LOG:"** TTS_IsPlayStatus nStatus=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1731_112_2_18_3_3_43_141,(uint8*)"d", g_SyncData.Status);
	if (JTTS_STATUS_PLAY == g_SyncData.Status
		|| JTTS_STATUS_PAUSE == g_SyncData.Status
		)
	{
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}


#ifdef WIN32
/////////////////////////////////////////////////

// init tts engine
jtErrCode jtTTSAPI jtTTS_Init(
	const signed char* szCNLib,			// [in] chinese data library
	const signed char* szENLib,			// [in] english data library
	const signed char* szDMLib,			// [in] domain data library
	unsigned long*	pdwHandle,			// [out] handle to TTS engine
	void*			pExtBuf)			// [in] extend buffer size
{
    return jtTTS_ERR_NONE;
}

// exit tts engine
jtErrCode jtTTSAPI jtTTS_End(
	unsigned long	dwHandle)			// [in] handle to TTS engine
{
    return jtTTS_ERR_NONE;
}

// set parameter
jtErrCode jtTTSAPI jtTTS_SetParam(
	unsigned long	dwHandle,			// [in] handle to TTS engine
	jtTTSParam		nParam,				// [in] parameter index
	long			iValue) 			// [in] parameter value
{
    return jtTTS_ERR_NONE;
}

// get parameter
jtErrCode jtTTSAPI jtTTS_GetParam(
	unsigned long	dwHandle,			// [in] handle to TTS engine
	jtTTSParam		nParam,				// [in] parameter index
	long*			piValue)			// [out] buffer to receive the parameter value
{
    return jtTTS_ERR_NONE;
}

// exit running of an instance and leave current thread's control
jtErrCode jtTTSAPI jtTTS_SynthStop(
	unsigned long	dwHandle)			// [in] handle to TTS engine
{
    return jtTTS_ERR_NONE;
}

// begin to synthesize text, and output data through OutputVoiceProc callback
jtErrCode jtTTSAPI jtTTS_SynthesizeText(
	unsigned long	dwHandle,			// [in] handle to TTS
	const void*		pText,				// [in] input text buffer to be synthesized
	long			iSize)				// [in] size of input text buffer to be synthesized
{
    return jtTTS_ERR_NONE;
}

// get extend buffer size
jtErrCode jtTTSAPI jtTTS_GetExtBufSize(
	const signed char* szCNLib,			// [in] chinese data library
	const signed char* szENLib,			// [in] english data library
	const signed char* szDMLib,			// [in] domain data library
	long*			piSize)			// [out] size of extend buffer size
{
    return jtTTS_ERR_NONE;
}

#endif

//////////////////////////////////////////////////spreadtrum add start





PUBLIC uint32 ejTTSPlayer_TestFile(const char *szCNLib, char *pcszText, const char *pcszFileName)
{
	uint32 dwErrCode;
	int32 lSize = 0;
	uint32 hTTS;			// handle to TTS engine 2010-9-10 uint64 -> uint64
	void* pGlobalData = PNULL;
	// 音频播放设备设置信息
	uint32 nSamplesPerSec = 16000;
	uint16 nBitsPerSample = 16;
	int32  nFormat = jtTTS_FORMAT_PCM_16K16B;
	uint16 *pwszFileName = (uint16 *)pcszFileName;
	//SFS_HANDLE		    filehandle;

	//JTTS_LOG:"TTSPlayer|ejTTSPlayer_Init in"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1830_112_2_18_3_3_43_142,(uint8*)"" );

	dwErrCode = jtTTS_GetExtBufSize(szCNLib,PNULL,PNULL,&lSize );

	//JTTS_LOG:"TTSPlayer|lSize=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1834_112_2_18_3_3_43_143,(uint8*)"d", lSize );
	//JTTS_LOG:"TTSPlayer|GetExtBufSize|dwErrCode=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1835_112_2_18_3_3_43_144,(uint8*)"d", dwErrCode );

	pGlobalData = SCI_ALLOCA( lSize );
	if ( SCI_NULL == pGlobalData )
	{
		//JTTS_LOG:"TTSPlayer|alloc memory failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1840_112_2_18_3_3_43_145,(uint8*)"" );
		return TTS_PLAYER_ERR_DONOTHING;
	}
	//JTTS_LOG:"TTSPlayer|global data address is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1843_112_2_18_3_3_43_146,(uint8*)"d", (uint32)pGlobalData );

	//tts 初始化
	dwErrCode = jtTTS_Init( szCNLib, PNULL, PNULL, &hTTS, pGlobalData );
	//JTTS_LOG:"TTSPlayer|Init errcode=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1847_112_2_18_3_3_43_147,(uint8*)"d", dwErrCode );

	g_PlayData.pHeap = pGlobalData;

	//JTTS_LOG:"TTSPlayer|ejTTSPlayer_Init success"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1851_112_2_18_3_3_43_148,(uint8*)"" );

	//JTTS_LOG:"TTSPlayer|Entry JTTS_PlayToFile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1853_112_2_18_3_3_43_149,(uint8*)"" );

	if(hTTS == SCI_NULL)
	{
		//JTTS_LOG:"TTSPlayer|TTSPlayer|NOT INIT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1857_112_2_18_3_3_43_150,(uint8*)"" );
		return TTS_PLAYER_ERR_NOTINIT;
	}

	dwErrCode = jtTTS_SetParam( hTTS, jtTTS_PARAM_WAV_FORMAT, (long)(&nFormat) );
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Set Wave format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1864_112_2_18_3_3_43_151,(uint8*)"" );
	}

	dwErrCode = GetAudioFormat(hTTS, &nSamplesPerSec, &nBitsPerSample);
	if ( dwErrCode != TTS_PLAYER_ERR_NONE )
	{
		//JTTS_LOG:"TTSPlayer|Get audio format failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1870_112_2_18_3_3_43_152,(uint8*)"" );
	}
	//JTTS_LOG:"TTSPlayer|SampleRate=%d Bitpersample=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1872_112_2_18_3_3_43_153,(uint8*)"dd", nSamplesPerSec, nBitsPerSample );

	g_filehandle = MMIFILE_CreateFile( pwszFileName, 
		SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, 0, 0 );
	if ( INVALID_HANDLE == g_filehandle )
	{
		//JTTS_LOG:"TTSPlayer|Can not open file"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1878_112_2_18_3_3_43_154,(uint8*)"" );
		return TTS_PLAYER_ERR_OPENFILE;
	}

	/* 设置直接文本输入 */
	dwErrCode = jtTTS_SetParam(hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);

	//JTTS_LOG:"TTSPlayer|INPUTTXT_MODE=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1885_112_2_18_3_3_43_155,(uint8*)"d", dwErrCode );

	/* 设置音频输出回调 */
	dwErrCode = jtTTS_SetParam(hTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long)OutputVoiceToFileProc);	

	//JTTS_LOG:"TTSPlayer|OUTPUT_CALLBACK=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1890_112_2_18_3_3_43_156,(uint8*)"d", dwErrCode );

	jtTTS_SetParam(hTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)&myUserData);

	//JTTS_LOG:"TTSPlayer|CALLBACK_USERDATA=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1894_112_2_18_3_3_43_157,(uint8*)"d", dwErrCode );

	//	dwErrCode = jtTTS_SynthesizeText(g_PlayData.hJTTS, myUserData.szTextIn, strlen(myUserData.szTextIn)*sizeof(char));
	dwErrCode = jtTTS_SynthesizeText(hTTS, pcszText, MMIAPICOM_Wstrlen((const wchar* )pcszText)*sizeof(uint16));

	jtTTS_End(hTTS);

	MMIFILE_CloseFile( g_filehandle );
	if ( SCI_NULL != pGlobalData )/*lint !e774*/
	{
		SCI_FREE( pGlobalData );
	}

	g_filehandle = INVALID_HANDLE;

	//JTTS_LOG:"TTSPlayer|Leave JTTS_Playtofile"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_AUDIO_1909_112_2_18_3_3_43_158,(uint8*)"" );
	return TTS_PLAYER_ERR_NONE;
}


#ifdef   __cplusplus
    }
#endif

