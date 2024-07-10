/*****************************************************************************
** File Name:      hero_tts_api.c                                         *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      All Rights Reserved.                                      *
** Description:    tts                                               *
*****************************************************************************/

#include "mmi_app_tts_trc.h"
#ifdef HERO_ENGINE_TTS_SUPPORT
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
#include "hero_tts_api.h"
#include "hero_tts_audio.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifm_export.h"
#include "mmitts_export.h"
#include "setjmp.h" 
#include "priority_app.h"
#include "mmipb_id.h"

/////////
#include "mmi_id.h"
#include "mmicc_id.h"
#include "mmienvset_export.h"
#if defined BROWSER_SUPPORT
#include "mmibrowser_id.h"
#endif
#include "mmiset_id.h"
#include "mmialarm_id.h"
/////
#include "mn_type.h"
#include "mmisms_app.h"
#if defined(HERO_ENGINE_TTS_NUM_USE_MP3)	
#include "tts_num_res_mp3.h"
#else
#include "tts_num_res.h"
#endif
#include "heroEngine_mmi_text.h"
#include "heroEngineApi.h"

#include "hero_tts_ui.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmiacc_id.h"
#include "mmiim_id.h"

#include "IN_message.h"

#include "Mmieng_id.h"
//#include "smartdualsim_id.h"
#include "mmimp3_id.h"
#include "mmifmm_id.h"
#include "mmimp3_export.h"
#include "mmieng_id.h"
#include "synthtext.h"
#include "hero_tts_app.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define HERO_TTS_CONST_16 (16)
#define HERO_TTS_CONST_8 (8)
#define HERO_TTS_NORMAL_TEXT_LENGTH_128 (128)
#define HERO_TTS_NORMAL_TEXT_LENGTH_256 (256)
#define HERO_TTS_NORMAL_TEXT_LENGTH_512 (512)
#define HERO_TTS_NORMAL_TEXT_LENGTH_1024 (1024)

//extern uint8 s_hero_no_tts;

extern hero_tts_setting_struct g_herotts_setting;

static int g_app_ring_volume = 0;
static int g_current_ring_type = 0;

PUBLIC BOOLEAN hero_IsOpenEngMainMenu(void);

BOOLEAN hero_tts_master_switch_is_open(void);

MMI_RESULT_E handle_hero_tts_pp_msg(PWND app_ptr, uint16 msg_id, DPARAM param);

MMI_APPLICATION_T gHeroTtsApp;

void MMI_HeroTts_initApp()
{
	gHeroTtsApp.ProcessMsg = handle_hero_tts_pp_msg;
}

#if defined(PLATFORM_UIX8910)
#define HeroTts_TraceApp(a) SCI_TRACE_MID a
#else
#define HeroTts_TraceApp(a) SCI_TRACE_LOW a
#endif
void tiho_tts_trace(const char *format, ...)
{
	va_list params;
	static char sHeroTtsLogPrintfbuf[256];

	memset(sHeroTtsLogPrintfbuf, 0, sizeof(sHeroTtsLogPrintfbuf));

	va_start(params, format);
#ifndef WIN32
	vsnprintf(sHeroTtsLogPrintfbuf, sizeof(sHeroTtsLogPrintfbuf) - 2, format, params);
#else
	_vsnprintf(sHeroTtsLogPrintfbuf, sizeof(sHeroTtsLogPrintfbuf) - 2, format, params);
#endif
	va_end(params);

	HeroTts_TraceApp(("HERO_TTS:%s", (char*)sHeroTtsLogPrintfbuf));
}

void hero_tts_printf(const char *format,...)
{
	va_list params;
	static char sHeroTtsLogPrintfbuf[256];

	memset(sHeroTtsLogPrintfbuf, 0, sizeof(sHeroTtsLogPrintfbuf));

	va_start(params, format);
#ifndef WIN32
	vsnprintf(sHeroTtsLogPrintfbuf, sizeof(sHeroTtsLogPrintfbuf) - 2, format, params);
#else
	_vsnprintf(sHeroTtsLogPrintfbuf, sizeof(sHeroTtsLogPrintfbuf) - 2, format, params);
#endif
	va_end(params);

	HeroTts_TraceApp(("HERO_TTS:%s", (char*)sHeroTtsLogPrintfbuf));
}

void *hero_tts_malloc(int size)
{
	return (void*)SCI_ALLOC_APPZ(size);
}

void hero_tts_free(void *ptr)
{
	if(ptr != NULL)
	{
		SCI_FREE(ptr);
	}
}

int hero_tts_convert_moudle_id(int id)
{
	static const int gTttMoudleIdMap[HERO_TTS_MOUDLE_ID_SIGNAL_MAX] = {
		HERO_HERO_TTS_MOUDLE_SIGNAL_BEG,
		HERO_HERO_TTS_MOUDLE_START_PCM,
		HERO_HERO_TTS_MOUDLE_STOP_PCM,
		HERO_HERO_TTS_MOUDLE_CALLBACK,
		HERO_HERO_TTS_MOUDLE_SIGNAL_END,
	};
	return gTttMoudleIdMap[id];
}

LOCAL BOOLEAN hero_tts_switch_PreCheck(TTS_ALL_RING_TYPE_E ring_type)
{
	BOOLEAN result = FALSE;

	switch(ring_type) 
	{
	case TTS_RING_TYPE_CALL:
		if(g_herotts_setting.cur_incoming_read_status != INCOMING_BROADCAST_OFF)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_MENU:
		{
			// 通讯录
			//if(hero_PB_GetWinId())
			//{
			//	if(g_herotts_setting.cur_phb_read_status == 1)
			//	{
			//		result = TRUE;
			//	}
			//}
			//else
			{
				// 菜单
				if(g_herotts_setting.cur_menu_read_status == 1)
				{
					result = TRUE;
				}
			}
		}
		break;
	case TTS_RING_TYPE_SMS:
	case TTS_RING_TYPE_MMS:
		if(g_herotts_setting.cur_sms_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_AUTO_TIME:
		if(g_herotts_setting.cur_auto_time_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_AKEY_TIME:
		if(g_herotts_setting.cur_akey_time_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_DIAL:
		if(g_herotts_setting.cur_dial_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_NEW_SMS:
		if(g_herotts_setting.cur_new_sms_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_CHARGE:
		if(g_herotts_setting.cur_charge_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_LOW_POWER:
		if(g_herotts_setting.cur_low_power_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_LOCK_SCREEN:
		if(g_herotts_setting.cur_lock_screen_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_AKEY_POWER_LEVEL:
		if(g_herotts_setting.cur_akey_power_level_read_status == 1)
		{
			result = TRUE;
		}
		break;
	case TTS_RING_TYPE_APP:
		result = TRUE;
		break;
	default:
		break;
	}
	return result;
}

int hero_tts_need_broadcast(TTS_ALL_RING_TYPE_E type)
{	
	int active_mode = 0;
	int focusWinId = MMK_GetFocusWinId();
	MMISET_LANGUAGE_TYPE_E lang_type = 0;
#if 0//def MMI_IDLE_SHORTCUT_SUPPORT	
		//MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);
		GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
		GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
		GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
		GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_list_display_type);
		GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);
		GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);
#endif

	LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast focusWinId = %d, type = %d", focusWinId, type);
	MMIAPISET_GetLanguageType(&lang_type);
	if(lang_type == MMISET_LANGUAGE_ENGLISH)
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 014-0");
		return 0;
	}
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
    if(!hero_tts_master_switch_is_open())
		return 0;
#endif
#if 0
	//if(MMIAPIMP3_IsPlayerActive() || (GUIFORM_CHILD_DISP_HIDE != child_list_display_type )|| (GUIFORM_CHILD_DISP_HIDE != child_music_display_type )|| (GUIFORM_CHILD_DISP_HIDE != child_fm_display_type ))
	if((MMIAPIMP3_IsPlayerActive() 
		|| (GUIFORM_CHILD_DISP_HIDE != child_music_display_type )
		)
		&& (type != TTS_RING_TYPE_CALL)
	)
    {
       LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-11111");
	if(MMIMP3_GetAudioState() != MMIMP3_AUDIO_PAUSED)   
    	    return 0;
    }	
	
    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type )
    {
	    return 0;
    }
#endif	
	//if(MMIAPIMP3_IsPlayerActive()&& MMIAPIMP3_IsPlayerPlaying())
	//{
	//       hero_test_printf("[inkleak.zhao] hero_tts_need_broadcast 016-0");
	//	return 0;
	//}	

	//进入音乐播放器屏蔽语音王
	if(focusWinId>=MMIMP3_WIN_ID_START && focusWinId<MMIMP3_MAX_WIN_ID)
	{
	       LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-2222");
		return 0;
	}

	//if(focusWinId==MMISET_SET_SOUNDS_WIN_ID)
	//{
	//	LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-33333");
	//	return 0;
	//}

	if(focusWinId>MMIENG_WIN_ID_START && focusWinId<MMIENG_MAX_WIN_ID)
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-44444");
		return 0;
	}

	if (TRUE == hero_IsOpenEngMainMenu())//测试模式不播报
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-5555");
		return 0;
	}

	//if(s_hero_no_tts == 1)
	//{
	//	s_hero_no_tts = 0;
	//	return 0;
	//}
	
	if(type != TTS_RING_TYPE_CALL)
	{
		//通话状态冲突
		if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 013-0");
			return 0;
		}
	}

	//如果是静音模式和振动模式，则不播放
	active_mode = MMIAPIENVSET_GetActiveModeId();
	#if defined(ELT_NO_VIBRATOR)	
	if(active_mode == MMIENVSET_SILENT_MODE)
	#else
	if(active_mode == MMIENVSET_SILENT_MODE || active_mode == MMIENVSET_MEETING_MODE)
	#endif	
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 012-0");
		return 0;
	}

	// 相机冲突
	if(MMIAPIDC_IsOpened())
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 011-0");
		return 0;
	}
	
	// 录像机冲突
	#ifdef DCD_SUPPORT
	{
		extern BOOLEAN HERO_MMIDCD_IsOpened(void);
		if(HERO_MMIDCD_IsOpened())
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 010-0");
			return 0;
		}
	}
	#endif

#if 0//def VIDEO_PLAYER_SUPPORT
	// 视频播放冲突
	{
		extern BOOLEAN HERO_MMIVP_IsOpened(void);
		if(HERO_MMIVP_IsOpened() && type != TTS_RING_TYPE_CALL
		#ifdef MMI_KING_MOVIE_SUPPORT
		|| MMIAPIKM_IsKMmainWinOpen()
		#endif
		)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 009-0");
			return 0;
		}
	}
#endif
	
#ifndef HERO_ENGINE_TTS_INROM_SUPPORT	
	// 存储模式冲突
	if(MMIAPIUDISK_UdiskIsRun()) 
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 008-0");
		return 0;
	}
#endif
#if 1
	LOG_DEBUG("[matthew]:2-3:type=%d,(MMIAPIMP3_IsPlayerPlaying())=%d,(MMIAPIFM_IsFmActive())=%d,line=%d", type, (MMIAPIMP3_IsPlayerPlaying()), (MMIAPIFM_IsFmActive()),__LINE__);
	// MP3后台播放冲突
	//if(type == TTS_RING_TYPE_MENU) 
	if( (type == TTS_RING_TYPE_MENU) || (type == TTS_RING_TYPE_DIAL) || (type == TTS_RING_TYPE_LOCK_SCREEN)) 
	{
		if(MMIAPIMP3_IsPlayerPlaying()) //is_play_mp3bg = MMISRVAUD_IsAudPlaying(MMIAPIMP3_GetBGPlayerHandle());
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 007-0,line=%d",__LINE__);
			return 0;
		}
		#if 1
		{
			BOOLEAN is_play_mp3bg = FALSE;
			#ifdef MMI_AUDIO_PLAYER_SUPPORT
	    		is_play_mp3bg = MMISRVAUD_IsAudPlaying(MMIAPIMP3_GetBGPlayerHandle());
			if(is_play_mp3bg)
			{
				LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 007-1");
				return 0;
			}
			#endif
		}
		#endif
	}
#endif
	// FM后台播放冲突
	if(type == TTS_RING_TYPE_MENU) 
	{
		if(MMIAPIFM_IsFmActive())
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 006-0");
			return 0;
		}
	}
	
	//和来短信提示冲突
	if(type == TTS_RING_TYPE_MENU)
	{
		if(((focusWinId >= MMISET_RING_SELECT_CALL_WIN_ID)&& (focusWinId <= MMISET_RING_VOLTAGE_WARNING_WIN_ID))
			||((focusWinId >= MMIENVSET_RING_VOL_WIN_ID)&& (focusWinId <= MMIENVSET_KEY_RING_TYPE_WIN_ID))
			|| (focusWinId == MMIENVSET_ALL_RING_VOL_WIN_ID)
			|| (focusWinId == MMIALM_RINGLIST_WIN_ID)
			|| (focusWinId == MMIALM_FIXEDRINGLIST_WIN_ID)
			|| (focusWinId == MMIALM_EDIT_RINGTYPE_WIN_ID)
			|| (focusWinId == MMISCH_RINGLIST_WIN_ID)
			|| (focusWinId == MMISCH_FIXEDRINGLIST_WIN_ID)
			//|| (focusWinId == MMIMP3_SONG_PLAY_MAIN_VIEW_WIN_ID)
			//|| hero_GetSongPlayMainWinId()
			|| (focusWinId == MMIUDISK_USB_OPER_SELECT_WIN_ID) // 屏蔽USB选择界面菜单播报，和USB插拔提醒冲突
			)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 005-0");
			return 0;
		}
	}

	//和来短信提示冲突
	if(type == TTS_RING_TYPE_MENU)
	{
		if(focusWinId == MMISMS_NEWMSG_WIN_ID)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 004-0");
			return 0;
		}
#if defined BROWSER_SUPPORT
		if(focusWinId == MMIBROWSER_MAIN_WIN_ID)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 003-0");
			return 0;
		}
#endif
	}

	//处理拨号界面冲突
	if(0)//type == TTS_RING_TYPE_MENU)
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast focusWinId = %d", focusWinId);
		if(focusWinId == MMIIDLE_DIAL_WIN_ID || focusWinId == MAIN_IDLE_WIN_ID)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 001-0");
			return 0;
		}
	}

	if(type == TTS_RING_TYPE_DIAL)
	{	
		if(((focusWinId != MMIIDLE_DIAL_WIN_ID)  && (focusWinId != MAIN_IDLE_WIN_ID))
		)
		{
			LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast 002-0");
			return 0;
		}
		if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_AUTO_TIME))
		{
	    	TTSAPI_StopPlayTextByType(TTS_RING_TYPE_AUTO_TIME);
		}
	}
	LOG_DEBUG("[inkleak.zhao] hero_tts_need_broadcast return 1");
	return 1;
}

typedef struct {
	const unsigned char *ttsResour;
	unsigned int datalen;
}HERO_SIMPLE_RESOURE;
static const HERO_SIMPLE_RESOURE simplettsNumberRes[12] = 
{
	{&diar_0[0], sizeof(diar_0)},
	{&diar_1[0], sizeof(diar_1)},
	{&diar_2[0], sizeof(diar_2)},
	{&diar_3[0], sizeof(diar_3)},
	{&diar_4[0], sizeof(diar_4)},
	{&diar_5[0], sizeof(diar_5)},
	{&diar_6[0], sizeof(diar_6)},
	{&diar_7[0], sizeof(diar_7)},
	{&diar_8[0], sizeof(diar_8)},
	{&diar_9[0], sizeof(diar_9)},
	{&diar_x[0], sizeof(diar_x)},
	{&diar_j[0], sizeof(diar_j)}
};

static MMISRV_HANDLE_T s_hero_tts_audio_handle = 0;


LOCAL BOOLEAN s_is_prompt_when_err = TRUE;

static uint32 hero_tts_keycode_convert(uint32 ctrl_id)
{
	uint32 ret = -1;
	
	switch(ctrl_id)
	{
	       case MSG_KEYUP_0:
		case MSG_KEYDOWN_0:
		case KEY_0:
		case 0x0030:
			ret = 0;
			break;
		case MSG_KEYUP_1:
		case MSG_KEYDOWN_1:
		case KEY_1:
		case 0x0031:
			ret = 1;
			break;
		case MSG_KEYUP_2:
		case MSG_KEYDOWN_2:
		case KEY_2:
		case 0x0032:
			ret = 2;
			break;
		case MSG_KEYUP_3:
		case MSG_KEYDOWN_3:
		case KEY_3:
		case 0x0033:
			ret = 3;
			break;
		case MSG_KEYUP_4:
		case MSG_KEYDOWN_4:
		case KEY_4:	
		case 0x0034:
			ret = 4;
			break;
		case MSG_KEYUP_5:
		case MSG_KEYDOWN_5:	
		case KEY_5:	
		case 0x0035:
			ret = 5;
			break;
		case MSG_KEYUP_6:
		case MSG_KEYDOWN_6:	
		case KEY_6:	
		case 0x0036:
			ret = 6;
			break;
		case MSG_KEYUP_7:
		case MSG_KEYDOWN_7:	
		case KEY_7:
		case 0x0037:
			ret = 7;
			break;
		case MSG_KEYUP_8:
		case MSG_KEYDOWN_8:
		case KEY_8:
		case 0x0038:
			ret = 8;
			break;
		case MSG_KEYUP_9:
		case MSG_KEYDOWN_9:	
		case KEY_9:	
		case 0x0039:
			ret = 9;
			break;
		case MSG_KEYUP_STAR:
		case MSG_KEYDOWN_STAR:
		case KEY_STAR:	
		case 0x002A:
			ret = 10;
			break;
		case MSG_KEYUP_HASH:
		case MSG_KEYDOWN_HASH:
		case KEY_HASH:	
		case 0x0023:
			ret = 11;
			break;
		default:
			break;
	}
	return ret;
}

static const unsigned char * hero_simpleTtsGetNumberResour(int index, unsigned int* datalen)
{
	if(index >= 0 && index < 12)
	{
		*datalen = simplettsNumberRes[index].datalen;
		return simplettsNumberRes[index].ttsResour;
	}
	return NULL;
}

static BOOLEAN hero_simpletts_mdi_cb(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{    
    switch(param->event)
    {
    case MMISRVMGR_NOTIFY_EXT:
    	if(s_hero_tts_audio_handle != 0)
		{
	        MMISRVAUD_Stop(s_hero_tts_audio_handle);
	        MMISRVMGR_Free(s_hero_tts_audio_handle);
	        s_hero_tts_audio_handle = 0;
		}
		break;
    default:
        break;
    }
    return TRUE;
}

static int hero_simplettsplay(const unsigned char *ttsResour, unsigned int datalen, HERO_INT32 cb)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRVAUD_RET_E	result = 0;

	if(s_hero_tts_audio_handle != 0)
	{
        MMISRVAUD_Stop(s_hero_tts_audio_handle);
        MMISRVMGR_Free(s_hero_tts_audio_handle);
        s_hero_tts_audio_handle = 0;
		SCI_SLEEP(50);
	}	
    req.notify = hero_simpletts_mdi_cb;
    req.pri = MMISRVAUD_PRI_HIGH;
    audio_srv.volume = TTS_GetTTSVolume();
	if(MMICC_IsExistIncommingCall())  // 插入耳机。来电话时，语音王从喇叭发声
	{
		audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER;
	}
    else
    {
	    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE | MMISRVAUD_ROUTE_EARFREE;
    }
    // is buf
    
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF; // MMISRVAUD_TYPE_RING_FILE 
    #if defined(HERO_ENGINE_TTS_NUM_USE_MP3)	
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
    #else	
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_WAVE;	
    #endif	
    audio_srv.info.ring_buf.data = ttsResour;
    audio_srv.info.ring_buf.data_len = datalen;
    audio_srv.play_times = 1;
	s_hero_tts_audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
	result = MMISRVAUD_Play(s_hero_tts_audio_handle, 0);
    if (MMISRVAUD_RET_ERROR== result)
    {
		MMISRVMGR_Free(s_hero_tts_audio_handle);
		s_hero_tts_audio_handle = 0;
		return 0;
	}
	else
	{	
		return 1;
	}	
}

BOOLEAN hero_tts_dial_broadcast_is_open(void)
{
	if(g_herotts_setting.cur_master_switch_status == 0)
	{
		return FALSE;
	}
	return hero_tts_switch_PreCheck(TTS_RING_TYPE_DIAL);
}

BOOLEAN hero_tts_dial_broadcast(uint32 num)
{
	int keycode = 0;
	const unsigned char *ttsResour = NULL;
	unsigned int datalen = 0;
	BOOLEAN ret = FALSE;

	if(g_herotts_setting.cur_dial_read_status == 0 || !hero_tts_need_broadcast(TTS_RING_TYPE_DIAL) || g_herotts_setting.cur_master_switch_status == 0)
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_dial_broadcast 1-0");
		return FALSE;
	}
	if(MMK_IsActiveCtrl(MMIIDLE_DIAL_LISTBOX_CTRL_ID))
		return FALSE;
	
	//20200819
	TTSAPI_StopPlayText();	

	keycode = hero_tts_keycode_convert(num);
	ttsResour = hero_simpleTtsGetNumberResour(keycode, &datalen);
	if(NULL == ttsResour)
	{
		LOG_DEBUG("[inkleak.zhao] hero_tts_dial_broadcast 2-0");
		ret = FALSE;
	}
	else
	{
		ret = (BOOLEAN)hero_simplettsplay(ttsResour, datalen, NULL); 
	}
	return ret;
}

PUBLIC void TTSAPI_StopPlayText()
{
	hero_tts_stop_play();
}

PUBLIC BOOLEAN TTSAPI_GetPlayStatus(void)
{
    return hero_tts_is_play_ring() ? TRUE : FALSE;
}

PUBLIC BOOLEAN TTSAPI_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type)
{
    return TTSAPI_GetPlayStatus();
}

PUBLIC void TTSAPI_StopSynthTextByTask(void)
{
	hero_tts_stop_play();
}

PUBLIC void TTSAPI_StopASynchTextByTask(void)
{
	hero_tts_stop_play();
}

PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type)
{
	return hero_tts_stop_play() ? TRUE : FALSE;
}

PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void)
{
	return hero_tts_is_play_ring() ? TRUE : FALSE;
}

PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id)
{

}

PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void)
{
    return  TTS_RING_TYPE_NONE;
}

PUBLIC BOOLEAN  TTSAPI_SynthTextByTask(void)
{
	return TRUE;
}

LOCAL BOOLEAN TTSPlayPreCheck(TTS_ALL_RING_TYPE_E ring_type)
{
	BOOLEAN result = TRUE;

	if(!hero_tts_switch_PreCheck(ring_type) || !hero_tts_need_broadcast(ring_type)) {
		result = FALSE;
	} else {
		TTSAPI_StopPlayText();
		result = TRUE;
	}
	LOG_DEBUG("[inkleak.zhao] TTSPlayPreCheck result = %d", result);
    return result;
}

static int heroTtsGetDitigMode(void)
{
	int focusWinId = MMK_GetFocusWinId();
	int tts_digit_mode = TIHO_TTS_READDIGIT_AUTO;	

	if((focusWinId == MMIIM_SWITCH_IM_WIN_ID)
		||(focusWinId == MMISMS_MSGBOX_MAIN_WIN_ID)
	#ifdef MMI_SMS_CHAT_SUPPORT
		||(focusWinId == MMISMS_CHATBOX_CHILD_WIN_ID)
		||(focusWinId == MMISMS_SHOW_CHAT_SMS_WIN_ID)
	#endif
		//||(focusWinId == MMISET_SET_BLACKLIST_LIST_WIN_ID)
		|| (focusWinId>MMI_CC_WIN_ID_START && focusWinId<MMICC_MAX_WIN_ID)
		//		|| (focusWinId>=MMISET_SET_SOS_WIN_ID && focusWinId<=MMISET_SET_SOS_ABOUT_WIN_ID)
		|| (focusWinId>MMIFMM_WIN_ID_START && focusWinId<MMIFMM_MAX_WIN_ID)
		|| (focusWinId>MMI_PB_WIN_ID_START && focusWinId<MMIPB_MAX_WIN_ID)
		//|| hero_PB_GetListWithSearchWinId()
		//|| hero_PB_GetOperateSelWinId()
		//|| hero_PB_GetWinId()
		//|| hero_PB_GetDetailWinId()
		//|| hero_PB_GetAddContactWinId()
	)
	{
		tts_digit_mode = TIHO_TTS_READDIGIT_AS_NUMBER;
	}
	return tts_digit_mode;
}

static void hero_tts_play_end_callback(int ret, void *userdata)
{
	int ring_type = (int)userdata;

	LOG_DEBUG("hero_tts_play_end_callback ring_type = %d", ring_type);
	if(ring_type == TTS_RING_TYPE_CALL)
	{
		if(MMICC_IsExistIncommingCall())
		{
			//extern void HeroPlayCustomRing(void);
			//HeroPlayCustomRing();
		}
	}
	else if(ring_type == TTS_RING_TYPE_APP)
	{
		hr_event(910, 0, 0);
	}
}

PUBLIC BOOLEAN TTSAPI_PlayText(MMITTS_PLAY_PARAM_T *play_param_ptr)
{
	int stringbufsize = 0; 
	char *stringbuf = NULL;
	int i = 0; int textsize = 0;

	if (PNULL == play_param_ptr) {
		return FALSE;
	}

	if(!TTSPlayPreCheck(play_param_ptr->ring_type)) {
		return FALSE;
	}

	if(play_param_ptr->txt_addr == PNULL || play_param_ptr->txt_len_in_byte == 0) {
		return FALSE;
	}

	if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)) {
		return FALSE;
	}

	stringbuf = hero_tts_get_string_buf(&stringbufsize);
	memset(stringbuf, 0, stringbufsize);

	MMI_MEMCPY(stringbuf, stringbufsize, play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, play_param_ptr->txt_len_in_byte);
	textsize = play_param_ptr->txt_len_in_byte;

	LOG_DEBUG("TTSAPI_PlayText play_param_ptr->ring_type = %d", play_param_ptr->ring_type);
	if(TTS_RING_TYPE_CALL == play_param_ptr->ring_type)
	{
		if(g_herotts_setting.cur_incoming_read_status == INCOMING_BROADCAST_5 || // 五次
			g_herotts_setting.cur_incoming_read_status == INCOMING_BROADCAST_3 || g_herotts_setting.cur_incoming_read_status == INCOMING_BROADCAST_REPEAT)
		{
			int count = g_herotts_setting.cur_incoming_read_status == INCOMING_BROADCAST_3 ? 2 : 4;
			if(g_herotts_setting.cur_incoming_read_status == INCOMING_BROADCAST_REPEAT) {
				count += 20;
			}

			for(i = 0; i < count; i++) 
			{
				if(textsize + play_param_ptr->txt_len_in_byte > stringbufsize){
					break;
				}
				MMI_MEMCPY(stringbuf + textsize, stringbufsize, play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, play_param_ptr->txt_len_in_byte);
				textsize += play_param_ptr->txt_len_in_byte;
			}
		}
	}

	tiho_tts_set_read_digit_type(heroTtsGetDitigMode());
	if(TTS_RING_TYPE_NEW_SMS == play_param_ptr->ring_type || TTS_RING_TYPE_MENU == play_param_ptr->ring_type){
		tiho_tts_set_read_digit_type(TIHO_TTS_READDIGIT_AS_NUMBER);
	} else if(TTS_RING_TYPE_AKEY_TIME == play_param_ptr->ring_type || TTS_RING_TYPE_AUTO_TIME == play_param_ptr->ring_type){
		tiho_tts_set_read_digit_type(TIHO_TTS_READDIGIT_AS_VALUE);
	}

	g_app_ring_volume = play_param_ptr->tts_vol;
	g_current_ring_type = play_param_ptr->ring_type;
	return hero_tts_start_play(stringbuf, textsize, hero_tts_play_end_callback, (void*)play_param_ptr->ring_type, TTS_RING_TYPE_CALL == play_param_ptr->ring_type) ? TRUE : FALSE;
}

int tiho_get_tts_out_volume()
{
	return TIHO_TTS_VOLUME_NORMAL;
}

int tiho_get_tts_out_volume_increase()
{
	return TIHO_TTS_VOLUME_INCREASE_MIN;
}

void hero_tts_send_text(wchar* wstr_ptr, uint16 wstr_len, TTS_ALL_RING_TYPE_E ring_type)
{
	MMITTS_PLAY_PARAM_T play_param = {0};

	play_param.txt_addr = (uint8 *)wstr_ptr;
	play_param.txt_len_in_byte = wstr_len * 2;
	play_param.is_ucs2 = TRUE;
	play_param.play_times = 1;
	play_param.tts_vol = MMIAPISET_GetMultimVolume();
	play_param.ring_type = ring_type;
	play_param.win_handle = MMK_GetFocusWinId();
	play_param.tts_route = MMISRVAUD_ROUTE_AUTO;

	if(TTS_RING_TYPE_AKEY_TIME == play_param.ring_type || TTS_RING_TYPE_AUTO_TIME == play_param.ring_type) {
		play_param.digit_mode = TTS_DIGIT_VALUE;
	} else {
		play_param.digit_mode = TTS_DIGIT_AUTO;
	}
	play_param.punc_mode = TTS_PUNC_OFF;
	play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;

	TTSAPI_PlayText(&play_param);
}

int hero_tts_get_volume(void)
{
	return g_herotts_setting.cur_volume_status;
}

uint16* hero_tts_Wstrcpy(wchar* dst, const wchar* src)
{
	return MMIAPICOM_Wstrcpy(dst, src);
}

wchar* hero_tts_Wstrcat(wchar* dst, const wchar* src)
{
	return MMIAPICOM_Wstrcat(dst, src);
}

unsigned int hero_tts_Wstrlen( const wchar* str )
{
	return MMIAPICOM_Wstrlen(str);
}

wchar* hero_tts_Wstrncpy(wchar* dst, const wchar* src, size_t count)
{
	return MMIAPICOM_Wstrncpy(dst, src, count);
}

uint32 hero_tts_UTF8ToWstr(wchar *wstr_ptr, uint32 wstr_len, const uint8 *utf8_ptr, uint32 utf8_len)
{
	return GUI_UTF8ToWstr(wstr_ptr, wstr_len, utf8_ptr, utf8_len);
}

void hero_tts_SolarToLunar(uint16 solar_year, uint8 solar_month, uint8 solar_day, uint16 *lunar_year_ptr, uint8 *lunar_month_ptr, uint8 *lunar_day_ptr )
{
#ifdef HERO_ENGINE_TTS_TIME_BROADCAST_AND_LUNAR
	MMICALENDAR_SolarToLunar(solar_year, solar_month, solar_day, lunar_year_ptr, lunar_month_ptr, lunar_day_ptr);
#endif
}

#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
BOOLEAN hero_tts_master_switch_is_open(void)
{
    return g_herotts_setting.cur_master_switch_status;
}
#endif

PUBLIC BOOLEAN hero_IsOpenEngMainMenu(void)
{
	if (MMK_IsOpenWin(MMIENG_MAINMENU_WIN_ID) || MMK_IsOpenWin(MMIENG_UITEST_MENU_WIN_ID))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL hero_tts_is_open_usb_select_win(void)
{
	return MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
}

BOOLEAN hero_tts_low_power_broadcast_is_open(void)
{
	BOOLEAN ret = FALSE;
	if(hero_tts_need_broadcast(TTS_RING_TYPE_LOW_POWER) && hero_tts_switch_PreCheck(TTS_RING_TYPE_LOW_POWER))
	{
		ret = TRUE;
	}
	return ret;
}

BOOLEAN hero_tts_menu_broadcast_is_open(void)
{
	BOOLEAN ret = FALSE;
	if(hero_tts_need_broadcast(TTS_RING_TYPE_MENU) && hero_tts_switch_PreCheck(TTS_RING_TYPE_MENU))
	{
		ret = TRUE;
	}
	return ret;
}

BOOLEAN hero_tts_new_sms_broadcast_is_open(void)
{
	BOOLEAN ret = FALSE;
	if(hero_tts_need_broadcast(TTS_RING_TYPE_NEW_SMS) && hero_tts_switch_PreCheck(TTS_RING_TYPE_NEW_SMS)) {
		ret = TRUE;
	}
	return ret;
}

static const char *heroTtsNumberMap[] = {
	"\xE9\x9B\xB6\x00", //零
	"\xE4\xB8\x80\x00", //一
	"\xE4\xBA\x8C\x00", //二
	"\xE4\xB8\x89\x00", //三
	"\xE5\x9B\x9B\x00", //四
	"\xE4\xBA\x94\x00", //五
	"\xE5\x85\xAD\x00", //六
	"\xE4\xB8\x83\x00", //七
	"\xE5\x85\xAB\x00", //八
	"\xE4\xB9\x9D\x00", //九
};
	
static void hero_tts_date_to_12_hour_string(wchar* output, SCI_DATE_T*	pdate, SCI_TIME_T  * ptime, BOOLEAN isMin)
{
	uint8 temp_buff[32] = {0};
	uint8 temp_buff_w[64] = {0};
	uint8 myHour = 0;
	uint8                   lun_month = 0;
    uint8                   lun_day = 0;
    uint16                  lun_year = 0;
	uint8 isAppend = 0;
	uint8 yearMap[4] = {0};
	uint8 count = 0;
	uint16 year = pdate->year;

	hero_tts_Wstrcpy(output, (const wchar*)"\xb0\x73\x28\x57\x2f\x66\x2c\x00\x2c\x00\x00\x00");

	while(year > 0 && count < sizeof(yearMap))
	{
		yearMap[sizeof(yearMap) - count - 1] = year%10;
		year = year/10;
		count++;
	}
	memset(temp_buff, 0, sizeof(temp_buff));
	for(count = 0; count < sizeof(yearMap); count++)
	{
		if(yearMap[count] > 0 || isAppend)
		{
			isAppend = 1;
			strcat(temp_buff, heroTtsNumberMap[yearMap[count]]);
		}
	}

	hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
	hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
	hero_tts_Wstrcat(output, (const wchar*)"\x74\x5e\x00\x00");

	memset(temp_buff, 0, HERO_TTS_CONST_8);
	memset(temp_buff_w, 0, HERO_TTS_CONST_16);
	sprintf(temp_buff, "%d", pdate->mon);
	hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
	hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
	hero_tts_Wstrcat(output, (const wchar*)"\x08\x67\x00\x00");

	memset(temp_buff, 0, HERO_TTS_CONST_8);
	memset(temp_buff_w, 0, HERO_TTS_CONST_16);
	sprintf(temp_buff, "%d", pdate->mday);
	hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
	hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
	hero_tts_Wstrcat(output, (const wchar*)"\xe5\x65\x2c\x00\x2c\x00\x00\x00");

	// 农历
	hero_tts_SolarToLunar(pdate->year, pdate->mon, pdate->mday, &lun_year, &lun_month, &lun_day);
	if(lun_year != 0 && lun_month != 0 && lun_day != 0)
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x9c\x51\x86\x53\x00\x00");
		hero_tts_Wstrcat(output, (const wchar*)"\x2c\x00\x2c\x00\x00\x00");
		if(lun_year != pdate->year)
		{
			memset(temp_buff, 0, HERO_TTS_CONST_8);
			memset(temp_buff_w, 0, HERO_TTS_CONST_16);
			sprintf(temp_buff, "%d", lun_year);
			hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
			hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
			hero_tts_Wstrcat(output, (const wchar*)"\x74\x5e\x00\x00");
		}

		memset(temp_buff, 0, HERO_TTS_CONST_8);
		memset(temp_buff_w, 0, HERO_TTS_CONST_16);
		sprintf(temp_buff, "%d", lun_month);
		hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
		hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
		hero_tts_Wstrcat(output, (const wchar*)"\x08\x67\x00\x00");

		memset(temp_buff, 0, HERO_TTS_CONST_8);
		memset(temp_buff_w, 0, HERO_TTS_CONST_16);
		sprintf(temp_buff, "%d", lun_day);
		hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
		hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
		hero_tts_Wstrcat(output, (const wchar*)"\xe5\x65\x2c\x00\x2c\x00\x00\x00");
	}

	switch(pdate->wday)
	{
		case 0:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\xe5\x65\x00\x00");
			break;
		case 1:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\x00\x4e\x00\x00");
			break;
		case 2:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\x8c\x4e\x00\x00");
			break;
		case 3:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\x09\x4e\x00\x00");
			break;
		case 4:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\xdb\x56\x00\x00");
			break;
		case 5:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\x94\x4e\x00\x00");
			break;
		case 6:
			hero_tts_Wstrcat(output, (const wchar*)"\x1f\x66\x1f\x67\x6d\x51\x00\x00");
			break;
	}
	hero_tts_Wstrcat(output, (const wchar*)"\x2c\x00\x2c\x00\x00\x00");

	if(ptime->hour <= 5 && ptime->hour >= 0) // 凌晨：0-5
	{
		hero_tts_Wstrcat(output, (const wchar*)"\xcc\x51\x68\x66\x00\x00");
	}
	else if(ptime->hour <= 8 && ptime->hour > 5) // 早晨：5-8
	{
		hero_tts_Wstrcat(output, (const wchar*)"\xe9\x65\x68\x66\x00\x00");
	}
	else if(ptime->hour <= 11 && ptime->hour > 8) // 上午：8-11
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x0a\x4e\x48\x53\x00\x00");
	}
	else if(ptime->hour <= 13 && ptime->hour > 11) // 中午：11-13
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x2d\x4e\x48\x53\x00\x00");
	}
	else if(ptime->hour <= 16 && ptime->hour > 13) // 下午：13-16
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x0b\x4e\x48\x53\x00\x00");
	}
	else if(ptime->hour <= 19 && ptime->hour > 16) // 傍晚：16-19
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x8d\x50\x5a\x66\x00\x00");
	}
	else if(ptime->hour <= 24 && ptime->hour > 19) // 晚上：19-24
	{
		hero_tts_Wstrcat(output, (const wchar*)"\x5a\x66\x0a\x4e\x00\x00");
	}
	hero_tts_Wstrcat(output, (const wchar*)"\x2c\x00\x2c\x00\x00\x00");

	memset(temp_buff, 0, HERO_TTS_CONST_8);
	memset(temp_buff_w, 0, HERO_TTS_CONST_16);
	if(ptime->hour > 12)
	{
		myHour = ptime->hour - 12;
	}
	else
	{
		myHour = ptime->hour;
	}
	if(isMin == 0)
	{
		sprintf(temp_buff, "%02d:%02d", myHour, 0);
	}
	else
	{
		sprintf(temp_buff, "%02d:%02d", myHour, ptime->min);
	}
	hero_tts_UTF8ToWstr((wchar *)temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
	hero_tts_Wstrcat(output, (const wchar*)temp_buff_w);
}

void hero_tts_auto_time_broadcast(void)
{
	SCI_DATE_T  date = {0};
	SCI_TIME_T  time = {0};
	wchar text_buff_w[HERO_TTS_NORMAL_TEXT_LENGTH_512] = {0};
	
	if(TM_GetSysDate(&date) == ERR_TM_NONE
		&& TM_GetSysTime(&time) == ERR_TM_NONE)
	{
		do
		{
			if(g_herotts_setting.cur_auto_time_read_status != 1 || time.min != 0)
			{
				break;
			}
			if(g_herotts_setting.cur_auto_time_quantum_mode == 1)
			{//白天模式
				if(time.hour > 18 || time.hour < 8)
				{
					break;
				}
			}
			else if(g_herotts_setting.cur_auto_time_quantum_mode == 2)
			{//夜晚模式
				if(time.hour > 23 || time.hour < 18)
				{
					break;
				}
			}
			else if(g_herotts_setting.cur_auto_time_quantum_mode == 3)
			{//全天模式
				;
			}
			else if(g_herotts_setting.cur_auto_time_quantum_mode == 4)
			{//自定义模式
				if(g_herotts_setting.cur_auto_time_end_time >= g_herotts_setting.cur_auto_time_start_time)
				{
					if(time.hour > g_herotts_setting.cur_auto_time_end_time
					|| time.hour < g_herotts_setting.cur_auto_time_start_time)
					{
						break;
					}
				}
				else
				{
					if(!((time.hour >= g_herotts_setting.cur_auto_time_start_time && time.hour <= 23) || (time.hour >= 0 && time.hour <= g_herotts_setting.cur_auto_time_end_time)))
					{
						break;
					}
				}
			}
			hero_tts_date_to_12_hour_string(text_buff_w, &date, &time, 1);
			hero_tts_send_text(text_buff_w, hero_tts_Wstrlen(text_buff_w), TTS_RING_TYPE_AUTO_TIME);
		}while(0);
	}
}

void hero_tts_akey_time_broadcast(void)
{
	SCI_DATE_T	date = {0};
	SCI_TIME_T	time = {0};
	wchar text_buff_w[HERO_TTS_NORMAL_TEXT_LENGTH_512] = {0};

	if(TM_GetSysDate(&date) == ERR_TM_NONE && TM_GetSysTime(&time) == ERR_TM_NONE)
	{
		hero_tts_date_to_12_hour_string(text_buff_w, &date, &time, 1);
		hero_tts_send_text(text_buff_w, hero_tts_Wstrlen(text_buff_w), TTS_RING_TYPE_AKEY_TIME);
	}
}

static void hero_tts_new_sms_broadcast_in(wchar* wstr_ptr, TTS_ALL_RING_TYPE_E ring_type)
{
	uint32 text_len = 0;
	text_len = MMIAPICOM_Wstrlen(wstr_ptr);
	hero_tts_send_text(wstr_ptr, text_len, TTS_RING_TYPE_NEW_SMS);
}

void hero_tts_new_sms_broadcast(void *data ,void *content)
{
	APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T*)data;
	APP_SMS_USER_DATA_T* sms_user_data_t = (APP_SMS_USER_DATA_T*)content;
	uint32 length = 0;
	int asc_num_len;
	uint32 warn_text_len = 0;
	unsigned short new_sms_remind[] = {0x60A8, 0x6709, 0x4E00, 0x6761, 0x65B0, 0x6D88, 0x606F, 0x0000};
	unsigned short tts_new_sms[] = {0xFF1A, 0x6765, 0x81EA, 0x0000};
	unsigned short sms_content[HERO_TTS_NORMAL_TEXT_LENGTH_256] = {0};
	unsigned char phone_number[32] = {0};
	ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
	BOOLEAN is_new_mms = FALSE;

#ifndef MMS_SUPPORT
	// 判断是否为mms
	err_code = MNSMS_IsSupportMMSEx(sig_ptr->dual_sys,
									sms_user_data_t->user_data_head_t.length,
									sms_user_data_t->user_data_head_t.user_header_arr,
									&is_new_mms
									);
	//如果不支持彩信，直接返回		
	if(is_new_mms)
	{
		return;
	}
#endif

	asc_num_len = MMIAPICOM_GenDispNumber(sig_ptr->sms_t.origin_addr_t.number_type,
											sig_ptr->sms_t.origin_addr_t.num_len,
											sig_ptr->sms_t.origin_addr_t.party_num,
											(uint8 *)phone_number,
											(uint16)(sizeof(phone_number)));	
	if(memcmp(phone_number, "+86", 3) == 0)
	{
		memmove(phone_number, &phone_number[3], asc_num_len - 3);
		asc_num_len -= 3;
	}
	if(memcmp(phone_number, "86", 2) == 0)
	{
		memmove(phone_number, &phone_number[2], asc_num_len - 2);
		asc_num_len -= 2;
	}
	phone_number[asc_num_len] = 0;

	memcpy(sms_content, new_sms_remind, sizeof(new_sms_remind) - sizeof(unsigned short));
	warn_text_len = MMIAPICOM_Wstrlen(sms_content);
	memcpy(&sms_content[warn_text_len], tts_new_sms, sizeof(tts_new_sms) - sizeof(unsigned short));
	warn_text_len = MMIAPICOM_Wstrlen(sms_content);
	GUI_UTF8ToWstr(&sms_content[warn_text_len], 
			HERO_TTS_NORMAL_TEXT_LENGTH_256 - warn_text_len,
			phone_number,
			asc_num_len);
	hero_tts_new_sms_broadcast_in(sms_content, TTS_RING_TYPE_NEW_SMS);
}

static void hero_tts_lock_screen_broadcast_in(wchar* wstr_ptr, uint16 wstr_len, TTS_ALL_RING_TYPE_E ring_type)
{
	uint32 text_len = 0;
	text_len = wstr_len;
	hero_tts_send_text(wstr_ptr, text_len, TTS_RING_TYPE_LOCK_SCREEN);
}

void hero_tts_lock_screen_broadcast(BOOLEAN isLock)
{
	MMI_STRING_T kstring  = {0};

	MMI_GetLabelTextByLang(isLock ? TXT_HERO_TTS_LOCK_SCREEN_LOCKED : TXT_HERO_TTS_LOCK_SCREEN_UNLOCKED, &kstring);
	hero_tts_lock_screen_broadcast_in(kstring.wstr_ptr, kstring.wstr_len, TTS_RING_TYPE_LOCK_SCREEN);
}

static MMI_TEXT_ID_T hero_tts_get_text_id(HERO_TTS_TEXT_ID_E text_id_type)
{
	MMI_TEXT_ID_T text_id = 0;
	switch(text_id_type)
	{
		case HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT_90:
			text_id = TXT_HERO_TTS_SET_AKEY_POWER_LEVEL_TEXT_90;
			break;
    	case HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT_20:
    		text_id = TXT_HERO_TTS_SET_AKEY_POWER_LEVEL_TEXT_20;
    		break;
	    case HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT:
	    	text_id = TXT_HERO_TTS_SET_AKEY_POWER_LEVEL_TEXT;
	    	break;
	#if defined(MMI_POP_ONLY_CHARGE_NOTICE)
		case HERO_TTS_TEXT_ID_CHARGE_USB_REMOVE:
			text_id = TXT_HERO_TTS_CHARGE_REMOVE;
			break;
	#else			
	    case HERO_TTS_TEXT_ID_CHARGE_USB_REMOVE:
	    	text_id = TXT_HERO_TTS_CHARGE_USB_REMOVE;
	    	break;
	#endif			
	    case HERO_TTS_TEXT_ID_CHARGE_REMOVE:
	    	text_id = TXT_HERO_TTS_CHARGE_REMOVE;
	    	break;
	#if defined(MMI_POP_ONLY_CHARGE_NOTICE)
		case HERO_TTS_TEXT_ID_CHARGE_USB_CONNECT:
			text_id = TXT_HERO_TTS_CHARGE_CONNECT;
			break;
	#else			
	    case HERO_TTS_TEXT_ID_CHARGE_USB_CONNECT:
	    	text_id = TXT_HERO_TTS_CHARGE_USB_CONNECT;
	    	break;
	#endif			
	    case HERO_TTS_TEXT_ID_CHARGE_CONNECT:
	    	text_id = TXT_HERO_TTS_CHARGE_CONNECT;
	    	break;
	    case HERO_TTS_TEXT_ID_CHARGE_CHARGE_WARNING:
	    	text_id = TXT_HERO_TTS_CHARGE_WARNING;
	    	break;
	    case HERO_TTS_TEXT_ID_CHARGE_CHARGE_END:
	    	text_id = TXT_HERO_TTS_CHARGE_END;
	    	break;
	    case HERO_TTS_TEXT_ID_CHARGE_CHARGE_OVER_VOLTAGE:
	    	text_id = TXT_HERO_TTS_CHARGE_OVER_VOLTAGE;
	    	break;
	    default:
	    	break;
	}
	return text_id;
}

void hero_tts_GetText(MMI_TEXT_ID_T label, MMI_STRING_T* str_ptr)
{
	MMI_GetLabelTextByLang(label, str_ptr);
	//MMIRES_GetText(lTextId, NULL, &kstring);
}

static void hero_tts_akey_power_level_broadcast_in(int bat_value)
{
	MMI_STRING_T kstring  = {0};
	uint8 temp_buff[HERO_TTS_CONST_8] = {0};
	wchar temp_buff_w[HERO_TTS_CONST_16] = {0};
	wchar text_buff_w[HERO_TTS_NORMAL_TEXT_LENGTH_128] = {0};
	MMI_TEXT_ID_T notifyTextId = 0;
	HERO_TTS_TEXT_ID_E text_id_type;

	if(bat_value >= 90)
	{
		text_id_type = HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT_90;
	}
	else if(bat_value <= 20)
	{
		text_id_type = HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT_20;
	}
	else
	{
		text_id_type = HERO_TTS_TEXT_ID_POWER_LEVEL_TEXT;
	}
	notifyTextId = hero_tts_get_text_id(text_id_type);
	hero_tts_GetText(notifyTextId, &kstring);
	hero_tts_Wstrncpy(text_buff_w, kstring.wstr_ptr, kstring.wstr_len);
	if(bat_value > 20 && bat_value < 90)
	{
		sprintf(temp_buff, "%d", bat_value);
		hero_tts_UTF8ToWstr(temp_buff_w, HERO_TTS_CONST_16, temp_buff, strlen(temp_buff));
		hero_tts_Wstrcat(text_buff_w, temp_buff_w);
	}
	hero_tts_send_text(text_buff_w, hero_tts_Wstrlen(text_buff_w), TTS_RING_TYPE_AKEY_POWER_LEVEL);
}

void hero_tts_akey_power_level_broadcast(void)
{
	uint8 bat_value = 0;
	CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

	bat_value = p_chgmng_info->bat_remain_cap%101;
	hero_tts_akey_power_level_broadcast_in(bat_value);
}

static BOOLEAN s_is_hero_tts_usb_charge = FALSE;
static void hero_tts_charge_opt_broadcast_in(MMI_TEXT_ID_T textId)
{
	MMI_STRING_T    kstring = {0};
	TTS_ALL_RING_TYPE_E myRingType = 0;
	MMI_TEXT_ID_T lTextId = 0;
	HERO_TTS_TEXT_ID_E text_id_type;

	switch(textId)
	{
		case HERO_TTS_CHARGE_OPT_PLUGOUT: // 充电器已拔除
			myRingType = TTS_RING_TYPE_CHARGE;
			if(s_is_hero_tts_usb_charge)
			{
				s_is_hero_tts_usb_charge = FALSE;
				text_id_type = HERO_TTS_TEXT_ID_CHARGE_USB_REMOVE;
			}
			else
			{
				text_id_type = HERO_TTS_TEXT_ID_CHARGE_REMOVE;
			}
			break;
		case HERO_TTS_CHARGE_OPT_START: // 开始充电
			myRingType = TTS_RING_TYPE_CHARGE;
			if(hero_tts_is_open_usb_select_win())
			{
				s_is_hero_tts_usb_charge = TRUE;
				text_id_type = HERO_TTS_TEXT_ID_CHARGE_USB_CONNECT;
			}
			else
			{
				s_is_hero_tts_usb_charge = FALSE;
				text_id_type = HERO_TTS_TEXT_ID_CHARGE_CONNECT;
			}
			break;
		case HERO_TTS_CHARGE_OPT_WARNING: // 电池电量低
			myRingType = TTS_RING_TYPE_LOW_POWER;
			text_id_type = HERO_TTS_TEXT_ID_CHARGE_CHARGE_WARNING;
			break;
		case HERO_TTS_CHARGE_OPT_END: // 充电结束
			myRingType = TTS_RING_TYPE_LOW_POWER;
			text_id_type = HERO_TTS_TEXT_ID_CHARGE_CHARGE_END;
			break;
		case HERO_TTS_CHARGE_OPT_OVER_VOLTAGE: // 充电电压过大
			myRingType = TTS_RING_TYPE_CHARGE;
			text_id_type = HERO_TTS_TEXT_ID_CHARGE_CHARGE_OVER_VOLTAGE;
			break;
	}
	lTextId = hero_tts_get_text_id(text_id_type);
	hero_tts_GetText(lTextId, &kstring);
	hero_tts_send_text(kstring.wstr_ptr, kstring.wstr_len, myRingType);
}

void hero_tts_charge_opt_broadcast(MMI_TEXT_ID_T textId)
{
	HERO_TTS_CHARGE_OPT_E charge_type;
	switch(textId)
	{
		case TXT_CHARGE_PLUGOUT: // 充电器已拔除
			charge_type = HERO_TTS_CHARGE_OPT_PLUGOUT;
		break;
		case TXT_CHARGE_START: // 开始充电
			charge_type = HERO_TTS_CHARGE_OPT_START;
		break;
		case TXT_CHARGE_WARNING: // 电池电量低
			charge_type = HERO_TTS_CHARGE_OPT_WARNING;
		break;
		case TXT_CHARGE_END: // 充电结束
			charge_type = HERO_TTS_CHARGE_OPT_END;
		break;
		case TXT_CHARGE_OVER_VOLTAGE: // 充电电压过大
			charge_type = HERO_TTS_CHARGE_OPT_OVER_VOLTAGE;
		break;
		default:
		return;
	}
	if (STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition()) //正常开机状态下才播报 //start modify by matthew 20181212
	{
		hero_tts_charge_opt_broadcast_in(charge_type);
	}
}

int32 hr_plat_tts_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int volume = 0;
	MMITTS_PLAY_PARAM_T play_param = {0};
	tihoTtsReqDataSt *reqData = (tihoTtsReqDataSt*)input;
	extern char* hr_str_convert_endian(char * str);

	if(input == NULL ||reqData->textPtr == NULL || input_len != sizeof(tihoTtsReqDataSt))
	{
		return HR_FAILED;
	}
	if(reqData->volume == 0)
	{
		volume = reqData->volume;
	}
	else
	{
		volume = reqData->volume * 2 - 1;
	}
    if(volume >= MMISET_VOL_MAX)
	{
		volume = MMISET_VOL_MAX;
	}
	hr_str_convert_endian(reqData->textPtr);

	play_param.txt_addr = (uint8 *)reqData->textPtr;
	play_param.txt_len_in_byte = hero_tts_Wstrlen(reqData->textPtr) * 2;
	play_param.is_ucs2 = TRUE;
	play_param.play_times = 1;
	play_param.tts_vol = volume;
	play_param.ring_type = TTS_RING_TYPE_APP;
	play_param.win_handle = MMK_GetFocusWinId();
	play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
	play_param.digit_mode = TTS_DIGIT_AUTO;
	play_param.punc_mode = TTS_PUNC_OFF;
	play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;

	TTSAPI_PlayText(&play_param);
	return HR_SUCCESS;
}

int32 hr_stop_tts_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	TTSAPI_StopPlayTextByType(TTS_RING_TYPE_APP);
	return HR_SUCCESS;
}

int32 hr_get_tts_type(int32 param)
{		
#ifdef HERO_ENGINE_TTS_INROM_SUPPORT
	return HR_PLAT_VALUE_BASE;
#else
	if(TTSAPI_checkResFile())
	{
		return HR_PLAT_VALUE_BASE;
	}
	return HR_PLAT_VALUE_BASE + 1;
#endif
}

PUBLIC void TTSAPI_SetSoftkeyTextId( 
                    MMI_WIN_ID_T    win_id,        // 窗口id
                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                    BOOLEAN         is_need_update // whether update)
                    )
{
	if(TTSAPI_GetPlayStatus())
	{
		GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, STXT_STOP, rightsoft_id, is_need_update);
	}
	else
	{
		GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, TXT_SET_TTS_PLAY, rightsoft_id, is_need_update);
	}
}

static const char *tiho_tts_wstrwstr(const char *buf, const char *sub)
{
	const char * bp;
	const char * sp;

	while (buf[0] || buf[1])
	{
		bp = buf;
		sp = sub;

		if (0 == sp[0] && 0 == sp[1])
			return NULL;
		
		while((bp[0] == sp[0]) && (bp[1] == sp[1]))
		{
			if (0 == sp[2] && 0 == sp[3])
				return buf;
			bp += 2;
			sp += 2;
		}
		
		buf += 2;
	}

	return NULL;
}

static char *tiho_tts_replace_str(char *inbuf, char *searchStr, char *replaceStrOver)
{
	char *outBuf = NULL;
	do
	{
		char *replactStartPtr = NULL;
		int inBufLen = 0;
		int replaceStrLen = 0;
		int copylen = 0;
		int bufPos = 0;
		//char *twentyStr = "\x8c\x4e\x41\x53\x00\x00"; // 20
		if(inbuf == NULL || searchStr == NULL)
		{
			break;
		}
		inBufLen = MMIAPICOM_Wstrlen((const char*)inbuf)<<1;
		if(inBufLen <= 0)
		{
			break;
		}
		outBuf = SCI_ALLOC(inBufLen + 66);
		if(outBuf == NULL)
		{
			break;
		}
		SCI_MEMSET(outBuf, 0, inBufLen + 66);

		replactStartPtr = (char*)tiho_tts_wstrwstr((const char*)inbuf, (const char*)searchStr);
		if(replactStartPtr == NULL)
		{
			SCI_FREE((void*)outBuf);
			outBuf = NULL;
			break;
		}
		replaceStrLen = MMIAPICOM_Wstrlen((const char*)searchStr)<<1;

		copylen = (int)((uint32)replactStartPtr - (uint32)inbuf);
		if(copylen > 0)
		{
			SCI_MEMCPY(outBuf, inbuf, copylen);
			bufPos += copylen;
		}

		SCI_MEMCPY(outBuf + bufPos, replaceStrOver, 2);
		bufPos += 2;

		copylen = inBufLen - copylen - replaceStrLen;
		if(copylen > 0)
		{
			SCI_MEMCPY(outBuf + bufPos, replactStartPtr + replaceStrLen, copylen);
		}
	}while(0);
	return outBuf;
}

typedef struct str_replace_table {
    wchar search_str[3];
    wchar replace_str[3];
}str_TABLE_T;

LOCAL const str_TABLE_T s_place_str_table[] =
{
    {{0x66f4,0x0000}, {0x8015,0x0000}},  /* 更 耕*/
	{{0x4e3a,0x0000}, {0x7ef4,0x0000}},  /* 为 维*/
	{{0x4eba,0x0000}, {0x4ec1,0x0000}},  /* 人 仁*/	
	{{0x003e,0x0000}, {0x0020,0x0000}},  
	{{0x003c,0x0000}, {0x0020,0x0000}},  
	{{0x007c,0x0000}, {0x0020,0x0000}},  /* |          */		
	{{0x5355,0x0000}, {0x4e39,0x0000}},  /* 单丹*/	
};

PUBLIC void TTSAPI_PlayTextForControl( MMI_STRING_T* str_ptr, uint32 num, MMISRVAUD_ROUTE_T tts_route )
{
#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
    if(PNULL == str_ptr || 0 == num)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_API_1732_112_2_18_3_3_47_224,(uint8*)"");
        return;
    }

    if(!TTSPlayPreCheck(TTS_RING_TYPE_MENU))
    {
        return;
    }
    
    // 0 为开关打开状态
    if (!MMIAPIENG_GetIQModeStatus())
    {
		uint32 i = 0, len = 0;
		wchar* total_str_ptr = NULL;
		uint8 vol = 1;
		MMITTS_PLAY_PARAM_T  play_param = {0};

        // 根据需求，开关此功能
	#if 1
		// 考虑到隐私问题，不自动读出收件箱，发件箱，草稿箱，已发信箱在list中部分内容
		MMI_WIN_ID_T focus_win_id = MMK_GetFocusWinId();
		if (MMISMS_MSGBOX_MAIN_WIN_ID == focus_win_id)
		{
			if (3 == num) num = 2;
		}

		//if(hero_PB_GetListWithSearchWinId() 
		//	|| MMISET_SET_CALL_WIN_ID == focus_win_id
		//	|| MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID == focus_win_id
		//	#ifdef MMI_SMS_CHAT_SUPPORT
		//    || (MMISMS_CHATBOX_CHILD_WIN_ID == focus_win_id)
		//	#endif
		//	)
		{
			num = 1;
		}
	#endif

		// 不知道所有字符串的总长，需要累加得出
		for (; i < num; i ++)
		{
			len += str_ptr[i].wstr_len + 2;
		}
		total_str_ptr = (wchar*)SCI_ALLOC((len + 2) * 2);
		SCI_MEMSET(total_str_ptr, 0, (len + 2) * 2);

		// 将所有字符串复制到一个buffer中，便于朗读
		for (i = 0, len = 0; i < num; i ++)
		{
			if (str_ptr[i].wstr_len > 0)
			{
				// 复制
				SCI_MEMCPY(total_str_ptr + len, str_ptr[i].wstr_ptr, str_ptr[i].wstr_len * 2);
				len += str_ptr[i].wstr_len;
				// 在字符串与字符串之间加空格符，这样可以产生停顿效果
				*(total_str_ptr + len) = 0x2e;
				len ++;
				*(total_str_ptr + len) = 0x2e;
				len ++;
			}
		}

		//特殊处理读音错误的字
		{
			uint32 j=0;
			for(j=0; j<sizeof(s_place_str_table)/sizeof(s_place_str_table[0]); j++)
			{
				char *outBuf = (wchar*)tiho_tts_replace_str((char*)total_str_ptr,(char*)s_place_str_table[j].search_str,(char*)s_place_str_table[j].replace_str);;
				if(outBuf) {
					if(total_str_ptr != NULL){
						SCI_FREE(total_str_ptr);
					}
					total_str_ptr = outBuf;
				}
			}
		}

		// 此时不应该弹出出错提示（比如缺少TTS资源文件等），因为菜单朗读太频繁，提示会给用户带来困扰
		play_param.txt_addr = total_str_ptr;
		play_param.txt_len_in_byte = len * 2;
		play_param.is_ucs2 = TRUE;
		play_param.play_times = 1;
		play_param.tts_vol = vol;
		play_param.ring_type = TTS_RING_TYPE_MENU;
		play_param.win_handle = 0;
		play_param.tts_route = tts_route;
		play_param.digit_mode = TTS_DIGIT_AUTO; //TTS_DIGIT_NUMBER;
		play_param.punc_mode = TTS_PUNC_OFF;
		play_param.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;
		TTSAPI_PlayText(&play_param);

		SCI_FREE(total_str_ptr);
    }
#endif
}

PUBLIC void TTSAPI_PlayTextForZDTSMS(wchar* wstr_ptr, uint16 wstr_len,uint16 repeat_times)
{
	MMISRVAUD_ROUTE_T tts_route = MMISRVAUD_ROUTE_SPEAKER;
	MMITTS_PLAY_PARAM_T play_param = {0};

	play_param.txt_addr = (uint8 *)wstr_ptr;
	play_param.txt_len_in_byte = wstr_len * 2;
	play_param.is_ucs2 = TRUE;
	play_param.play_times = repeat_times;
	play_param.tts_vol = 9;//MMIAPISET_GetMultimVolume();
	play_param.ring_type = TTS_RING_TYPE_SMS;
	play_param.win_handle = MMK_GetFocusWinId();
	play_param.tts_route = tts_route;//MMISRVAUD_ROUTE_AUTO;
	play_param.digit_mode = TTS_DIGIT_AUTO;
	play_param.punc_mode = TTS_PUNC_OFF;
	play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
	TTSAPI_PlayText(&play_param);
}

PUBLIC void TTSAPI_PlayTextForZDT(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
                                      uint16 repeat_times
                                      )
{
    MMISRVAUD_ROUTE_T tts_route = MMISRVAUD_ROUTE_SPEAKER;
#if defined(HERO_ENGINE_TTS_SUPPORT)
    if(PNULL == str_ptr || 0 == num)
    {
        return;
    }
    if(!TTSPlayPreCheck(TTS_RING_TYPE_MENU))
    {
        return ;
    }
    
    // 0 为开关打开状态
    //if (!MMIAPIENG_GetIQModeStatus())
    if(1)
    {
        uint32 i = 0, len = 0;
        wchar* total_str_ptr = NULL;
        uint8 vol = 9;
        MMITTS_PLAY_PARAM_T  play_param = {0};
        
#if defined(HERO_ENGINE_TTS_USE_MP3)
	if((num == 1)&&hero_tts_check_is_mispronunciation(str_ptr[0].wstr_ptr))
		return ;
#endif
	
        // 不知道所有字符串的总长，需要累加得出
        for (; i < num; i ++)
        {
            len += str_ptr[i].wstr_len + 2;
        }
        total_str_ptr = (wchar*)SCI_ALLOC_APP((len + 2) * 2);
        SCI_MEMSET(total_str_ptr, 0, (len + 2) * 2);
        
        // 将所有字符串复制到一个buffer中，便于朗读
        for (i = 0, len = 0; i < num; i ++)
        {
            if (str_ptr[i].wstr_len > 0)
            {
                // 复制
                SCI_MEMCPY(total_str_ptr + len, str_ptr[i].wstr_ptr, str_ptr[i].wstr_len * 2);
                len += str_ptr[i].wstr_len;
                // 在字符串与字符串之间加空格符，这样可以产生停顿效果
                *(total_str_ptr + len) = 0x2e;
                len ++;
                *(total_str_ptr + len) = 0x2e;
                len ++;
            }
        }
		
		//特殊处理读音错误的字
		{
					 uint32 j=0;
		
				for(j=0; j<sizeof(s_place_str_table)/sizeof(s_place_str_table[0]); j++)
					{
						char *outBuf =NULL;
						outBuf = (wchar* ) tiho_tts_replace_str((char*)total_str_ptr,(char*)s_place_str_table[j].search_str,(char*)s_place_str_table[j].replace_str);;
						if(outBuf)
						{				
								total_str_ptr  =outBuf;
						}
					}
		}

        // 此时不应该弹出出错提示（比如缺少TTS资源文件等），因为菜单朗读太频繁，提示会给用户带来困扰
        s_is_prompt_when_err = FALSE;
		play_param.txt_addr = total_str_ptr;
		play_param.txt_len_in_byte = len * 2;
		play_param.is_ucs2 = TRUE;
		play_param.play_times = repeat_times;
		play_param.tts_vol = vol;
		play_param.ring_type = TTS_RING_TYPE_SMS;
		play_param.win_handle = 0;
		play_param.tts_route = tts_route;
		play_param.digit_mode = TTS_DIGIT_AUTO; //TTS_DIGIT_NUMBER;
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
	extern int hero_tts_init();
	hero_tts_init();
	MMI_HeroTts_initApp();
#ifndef HERO_ENGINE_TTS_INROM_SUPPORT
    CreateTTSHidenDir();
#endif
}

PUBLIC MMISRVAUD_VOLUME_TYPE_E TTS_GetTTSVolumeType(void)
{
	return MMISRVAUD_VOLUME_TYPE_NONE;
}

PUBLIC uint16 TTS_GetTTSVolume(void)
{
	extern int hero_tts_get_volume(void);

	if(TTS_RING_TYPE_APP == g_current_ring_type){
		return g_app_ring_volume;
	}

	return hero_tts_get_volume();
}

PUBLIC void TTS_SetTTSVolume(uint16 set_vol)
{
	hero_tts_volume_set(TTS_GetTTSVolume());
}

PUBLIC BOOLEAN hero_is_in_tts_unusual_win(void)
{
	return MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID) || MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
