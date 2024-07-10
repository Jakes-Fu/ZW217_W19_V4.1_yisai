#include "port_cfg.h"
#include "lebao_app.h"

#include "std_header.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "mmk_timer.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmiset_export.h"


#ifdef WIFI_SUPPORT
  #if !defined(PLATFORM_UWS6121E)
  #define LEBAO_WIFI_SUPPORT_CRACK   1
  #else
  #define LEBAO_WIFI_SUPPORT_CRACK   0
  #endif
#else
  #define LEBAO_WIFI_SUPPORT_CRACK   0 
#endif  

#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
#include "mmiwifi_export.h"
#endif

#include "lebao_ctrl.h"
#include "lebao_misc.h"
#include "lebao_win_base.h"

#include "lebao_id.h"
#include "lebao_text.h"
#include "lebao_image.h"

// public variables
uint8 gLebaoEventTimerId = 0;

// private variables
LOCAL int _lebaoNetId = 0;

// private prototypes
LOCAL void lebao_event_timer_proc(uint8  timer_id, uint32 param);
LOCAL BOOLEAN lebao_pdp_active(void);
LOCAL void lebao_handle_pdp_msg(MMIPDP_CNF_INFO_T *msg_ptr);
LOCAL void lebao_print_current_ringset(void);

// public functions

void StartLebaoApp(void)
{
	// configure whether the LCD is a circular watch
	// port_cfg.h -> MAINLCD_CIRCULAR_WATCH
#ifdef MAINLCD_CIRCULAR_WATCH
	lebao_set_round_watch();
#endif

	lebao_set_max_volume(MMISET_VOL_MAX);

	// overwrite the last setting
	// reduce the default volume when the headset is connected
#ifdef BLUETOOTH_SUPPORT
	if (MMIAPIBT_GetActiveBTHeadset())
		lebao_set_volume(MMISET_VOL_FIVE); 
	else
#endif
	{
		if (MMISRVAUD_IsHeadSetPlugIn())
			lebao_set_volume(MMISET_VOL_FIVE);
		else
			lebao_set_volume(MMISET_VOL_MAX);
	}

	// maximum count of songs played automatically, 1 ~ 100
	lebao_player_set_next_max_count(10);

	// recording mode, 0 : file, 1 : streaming
  #if defined(PLATFORM_UMS9117) || defined(PLATFORM_ANTISW3) || defined(PLATFORM_UWS6121E)
	  lebao_ctrl_set_record_mode(0);
  #else
  	lebao_ctrl_set_record_mode(1);
  #endif
	
	// mode of playing tip sound, 0 : no, 1 : migu, 2 : lebao
	lebao_ctrl_set_can_play_tip_sound(0);

	// roll the text back and forth, first fix the ctrllabel.c (align)
	lebao_set_label_can_roll();
	
	// start app
	if (gLebaoEventTimerId != 0) {
		MMK_StopTimer(gLebaoEventTimerId);
		gLebaoEventTimerId = 0;
	}
	gLebaoEventTimerId = MMK_CreateTimerCallback(30, lebao_event_timer_proc, 0, TRUE);

	// external relations, continuous ID, non interruptible, for ctrl lib
	lebao_set_res_text_id(TEXT_LEBAO_WELCOME, TEXT_LEBAO_TIPS_THANK);
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	lebao_set_res_image_id(IMAGE_LEBAO_LOGO, IMAGE_LEBAO_VOLUME_5);
#else
	lebao_set_res_image_id(IMAGE_LEBAO_LOGO, IMAGE_LEBAO_LOADING);
#endif
	lebao_set_res_ctrl_id(MMI_LEBAO_CTRL_ID_MENU_OPTION, MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA);
	lebao_set_res_win_id(MMI_LEBAO_WIN_ID_SPLASH, MMI_LEBAO_WIN_ID_LOGIN);

// 儿童手表
	lebao_set_app_key("yisai8910watch", "c603dbc3");
// 成人手表
//	lebao_set_app_key(###, ###);

  // whether to use HTTPS mode, 0: no, 1: yes
  	lebao_set_https_mode(1);

	lebao_ctrl_init();

	// turn ringtone settings on or off, 0 : off, 1 : on
	lebao_player_set_open_ringtone(1);

	// delete MP3 when the disk space is less than 30kb
	if (lebao_ctrl_can_write_file(CFG_MP3_MIN_FILE_SIZE) != 0) {
//		lebao_create_msgbox_id(TEXT_LEBAO_MEMORY_FULL, 2000);
		lebao_ctrl_clear_mp3();
	}
	else {
		int maxSize = (CFG_MP3_MAX_FILE_SIZE < (300 * 1024)) ? (300 * 1024) : CFG_MP3_MAX_FILE_SIZE;

		if (lebao_ctrl_can_write_file(maxSize * 2) == 0) {
			lebao_get_config()->mp3MaxFileSize = maxSize;
		}
	}

	lebao_print_current_ringset();
}

void StopLebaoApp(void)
{
	if (gLebaoEventTimerId != 0) {
		MMK_StopTimer(gLebaoEventTimerId);
		gLebaoEventTimerId = 0;
	}

	// free all resources, timer, thread, list, window...
	lebao_exit();
}

//-------------------------------audio-------------------------------

// for play file or buffer
// mmisrvaud_api.h
// the priority from high to low is BT -> EARPHONE -> SPEAKER
int lebao_get_audio_srv_route_type(void)
{
	return MMISRVAUD_ROUTE_NONE;
}

// for streaming
// audio_api.h, AUDIO_DEVICE_MODE_TYPE_E
int lebao_get_audio_dev_mode(void)
{
#ifdef BLUETOOTH_SUPPORT
	if (MMIAPIBT_GetActiveBTHeadset())
		return AUDIO_DEVICE_MODE_BLUEPHONE; // AUDIO_DEVICE_MODE_HANDFREE
	else
#endif
	{
		if (MMISRVAUD_IsHeadSetPlugIn())
			return AUDIO_DEVICE_MODE_EARPHONE;
		else
			return AUDIO_DEVICE_MODE_HANDFREE;
	}
}

// for recording
// audio_api.h, AUDIO_DEVICE_MODE_TYPE_E
int lebao_get_record_dev_mode(void)
{
	return AUDIO_DEVICE_MODE_HANDFREE;
}

HAUDIODEV lebao_get_audio_device(void)
{
#ifdef BT_A2DP_SUPPORT		
		return (lebao_get_audio_dev_mode() == AUDIO_DEVICE_MODE_BLUEPHONE) 
		  ? hA2DPDev
      #ifdef PLATFORM_UWS6121E
    		: hAUDDEV;
      #else
        : hARMVB;
      #endif
#else
  #ifdef PLATFORM_UWS6121E
		return hAUDDEV;
  #else
    return hARMVB;
  #endif
#endif
}

HAUDIODEV lebao_get_audio_record_device(void)
{
#ifdef PLATFORM_UWS6121E
  return hAUDDEVRECORD;
#else
  return hARMVBRECORD;
#endif
}

void lebao_open_bt_device(void)
{
#ifdef BLUETOOTH_SUPPORT
  if (MMIAPIBT_GetActiveBTHeadset())
	  MMIBT_OpenDevice(0); // MMISRVAUD_SAMPLE_RATE_DEFAULT
#endif    
}

//-----------------------------set the volume----------------------------------

static int _maxVolume = MMISET_VOL_MAX;
static int _currentVolume = MMISET_VOL_NINE;

// 1 <= vol <= 9
int lebao_set_volume(const int vol)
{
	int realVol = vol;

	if (realVol < MMISET_VOL_ONE) realVol = MMISET_VOL_ONE;
	else if (realVol > _maxVolume) realVol = _maxVolume;

	_currentVolume = realVol;

	if (lebao_get_audioHandle() != 0) {
		// TYPE_LEBAO_PLAY_STREAM == 0, TYPE_LEBAO_RECORD_FILE == 3
		if (lebao_get_playType() == 0 || lebao_get_playType() == 3)
			AUDIO_SetVolume(_currentVolume);
		else
			MMISRVAUD_SetVolume(lebao_get_audioHandle(), _currentVolume);
	}

	//MMISRVAUD_SetVolumeNoHandle
	return 0;
}

// real volume
int lebao_get_volume(void)
{
	return _currentVolume;
}

// 1 <= vol <= 9
void lebao_set_max_volume(const int vol)
{
	if (vol <= 0) _maxVolume = 1;
	else if (vol > MMISET_VOL_MAX) _maxVolume = MMISET_VOL_MAX;
	else _maxVolume = vol;
}

int lebao_get_max_volume(void)
{
	return (_maxVolume <= 0) ? 1 : _maxVolume;
}

int lebao_get_volume_step(void)
{
	return (_maxVolume <= 5) ? 1 : 2;
}

int lebao_get_volume_percent(const int vol)
{
	int per = ((vol + 1) * 100) / (lebao_get_max_volume() + 1);

	if (per > 100) per = 100;
	else if (per < 10) per = 10;

	return per;
}

//-------------------------------ringtone-------------------------------

extern MMISET_CALL_RING_T MMIAPIENVSET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys, uint8 mode_id);

// W307 / W217
#if defined(PLATFORM_ANTISW3) || defined(PLATFORM_UWS6121E)
extern BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
	MN_DUAL_SYS_E dual_sys,
	MMISET_CALL_MORE_RING_T ring_info,
	uint8 mode_id
);
#else
extern BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
	MN_DUAL_SYS_E dual_sys,
	MMISET_CALL_MORE_RING_T* ring_info,
	uint8 mode_id
);
#endif

LOCAL void lebao_print_current_ringset(void)
{
	MMISET_CALL_RING_T call_ring_info = { MMISET_CALL_RING_FIXED, 0, 0 };

	call_ring_info = MMIAPIENVSET_GetCallRingInfo(MMIAPISET_GetActiveSim(), MMIENVSET_GetCurModeId());

	if (MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type) {
		char path[256] = { 0 };
		GUI_WstrToUTF8(path, 255, call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len);

		if (!MMIAPIFMM_IsFileExist(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len)) {
			helper_debug_printf("not exist, filename = %s", path);
		}
		else {
			helper_debug_printf("ringtone, filename=%s", path);
		}
	}
	else {
		helper_debug_printf("ringtone, type=%d", call_ring_info.call_ring_type);
	}
}

// for set rintone
// int lebao_query_ringtone(MMISET_CALL_MORE_RING_T* filePath, MMISET_CALL_MORE_RING_T* songName);
// int lebao_set_default_ringtone(void);
int lebao_set_ringtone(const char* filename)
{
	BOOLEAN ret = FALSE;
	MMISET_CALL_MORE_RING_T ring_info = { 0 };
	int len = (filename == NULL) ? 0 : os_strlen(filename);
	if (len <= 0 || len > MMISET_CALL_RING_NAME_MAX_LEN)
		return -1;

	ring_info.name_wstr_len = GUI_GBToWstr(ring_info.name_wstr, (const uint8*)filename, len);
	if (ring_info.name_wstr_len <= 0) return -1;

	if (!MMIAPIFMM_IsFileExist(ring_info.name_wstr, ring_info.name_wstr_len)) {
		helper_debug_printf("not exist, filename = %s", filename);
		// return -1;
	}
	
#if defined(PLATFORM_ANTISW3) || defined(PLATFORM_UWS6121E)
	ret = MMIAPIENVSET_SetMoreCallRingInfo(MMIAPISET_GetActiveSim(), ring_info, MMIENVSET_GetCurModeId());
#else
	ret = MMIAPIENVSET_SetMoreCallRingInfo(MMIAPISET_GetActiveSim(), &ring_info, MMIENVSET_GetCurModeId());
#endif

	helper_debug_printf("set ringtone %s, filename = %s", (ret == TRUE) ? "ok" : "failed", filename);
	lebao_event_send(EVT_LEBAO_RING_RESULT, LEBAO_PAGE_PLAYER, ret, NULL);
	return 0; // ((ret == TRUE) ? 0 : -1)
}

int lebao_query_ringtone(MMISET_CALL_MORE_RING_T* filePath, MMISET_CALL_MORE_RING_T* songName)
{
	int i = 0;
	link_list_node_t* node = NULL;
	song_info_t* song = NULL;

	link_list_t * localSongs = lebao_get_playlist();
	if (localSongs == NULL || localSongs->len <= 0)
		return -1;

	for (i = 0; i < (int)localSongs->len; i++) {
		node = link_list_at(localSongs, i);
		song = node->val;
		if (song == NULL || song->contentId == NULL || song->ringListenUrl == NULL) 
			continue;

		if (helper_is_local_file(song->ringListenUrl) && helper_is_file_exist(song->ringListenUrl)) {
			if(filePath != NULL && sdslen(song->ringListenUrl) <= MMISET_CALL_RING_NAME_MAX_LEN) {
				filePath->name_wstr_len = GUI_GBToWstr(filePath->name_wstr, 
					(const uint8*)song->ringListenUrl, sdslen(song->ringListenUrl));
			}

			if(songName != NULL && song->songName != NULL && sdslen(song->songName) <= MMISET_CALL_RING_NAME_MAX_LEN) {
				songName->name_wstr_len = GUI_UTF8ToWstr(songName->name_wstr,
					MMISET_CALL_RING_NAME_MAX_LEN,
					(const uint8*)song->songName, sdslen(song->songName));				
			}
			return 0;
		}
	}

	return -1;
}

int lebao_set_default_ringtone(void)
{
	int i = 0;
	link_list_node_t* node = NULL;
	song_info_t* song = NULL;

	link_list_t * localSongs = lebao_get_playlist();
	if (localSongs == NULL || localSongs->len <= 0)
		return -1;

	for (i = 0; i < (int)localSongs->len; i++) {
		node = link_list_at(localSongs, i);
		song = node->val;
		if (song == NULL || song->contentId == NULL || song->ringListenUrl == NULL) 
			continue;

		if (helper_is_local_file(song->ringListenUrl) && helper_is_file_exist(song->ringListenUrl)) {
			return lebao_set_ringtone(song->ringListenUrl);
		}
	}

	return -1;
}

int lebao_del_default_ringtone(void)
{
	int i = 0;
	link_list_node_t* node = NULL;
	song_info_t* song = NULL;

	link_list_t * localSongs = lebao_get_playlist();
	if (localSongs == NULL || localSongs->len <= 0)
		return 0;

	for (i = 0; i < (int)localSongs->len; i++) {
		node = link_list_at(localSongs, i);
		song = node->val;
		if (song == NULL || song->contentId == NULL || song->ringListenUrl == NULL) 
			continue;

		if (helper_is_local_file(song->ringListenUrl) && helper_is_file_exist(song->ringListenUrl)) {
			helper_delete_file(song->ringListenUrl);
			sdsfree_val(song->ringListenUrl);
		}
	}

	lebao_save_playlist(1);
	return 0;
}

//-------------------------------network-------------------------------

#if defined(MULTI_SIM_SYS_SINGLE)
MN_DUAL_SYS_E lebao_get_active_sim(void)
{
	return MMIAPISET_GetActiveSim();
}
#else
MN_DUAL_SYS_E lebao_get_active_sim(void)
{
	MN_DUAL_SYS_E dualSysUse = MN_DUAL_SYS_1;

	if (!(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) && !(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
		return MMIAPISET_GetActiveSim();

	if (MMIAPIPHONE_GetSimExistedStatus(dualSysUse) && MMIAPIPHONE_GetDataServiceSIM(&dualSysUse))
		return dualSysUse;

	dualSysUse = MN_DUAL_SYS_2;
	if (MMIAPIPHONE_GetSimExistedStatus(dualSysUse) && MMIAPIPHONE_GetDataServiceSIM(&dualSysUse))
		return dualSysUse;

	return MMIAPISET_GetActiveSim();
}
#endif

int lebao_get_net_id(void)
{
	return _lebaoNetId;
}

#if 0
extern void WatchWIFI_MainWin_Enter(void);
#endif

BOOLEAN lebao_active_network(void)
{
	BOOLEAN result = FALSE;

	_lebaoNetId = 0;
	
#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
	// priority  wifi -> ps
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
		return TRUE;

	do {
#if defined(MULTI_SIM_SYS_SINGLE)
		if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
			break;
#else
		if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1) || MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2))
			break;
#endif
		else {
      lebao_network_enter_setting();
		}
	} while (0);
#endif

	if (MMIAPIPDP_PsIsPdpLinkExist())
		return TRUE;

	if(result == FALSE)
		result = lebao_pdp_active();

	return result;
}

BOOLEAN lebao_deactive_network(void)
{
	_lebaoNetId = 0;
	return MMIAPIPDP_Deactive(MMI_MODULE_LEBAO);
}

BOOLEAN lebao_network_is_connected(void)
{
#ifndef _WIN32
  #if (LEBAO_WIFI_SUPPORT_CRACK != 0)
  	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
  		return TRUE;
	#endif

	return MMIAPIPDP_PsIsActivedPdpLinkExist();
#else
	return TRUE;
#endif
}

void lebao_network_enter_setting(void)
{
#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
		return; 
	else {
		#if !defined(PLATFORM_ANTISW3) && !defined(PLATFORM_UWS6121E)
			MMIPDP_ACTIVE_INFO_T active_info = { 0 };
			active_info.app_handler = MMI_MODULE_LEBAO;
			active_info.handle_msg_callback = lebao_handle_pdp_msg;
			active_info.ps_interface = MMIPDP_INTERFACE_WIFI;
			result = MMIAPIPDP_Active(&active_info);
		#else
			WatchWIFI_MainWin_Enter();
		#endif
	}
#endif
}

void lebao_enter_setting_after_start_failed(int timeoutSeconds)
{
  if (timeoutSeconds >= 5) {
    lebao_network_enter_setting();
  }
}

// private functions
LOCAL void lebao_event_timer_proc(uint8 timer_id, uint32 param)
{
	lebao_event_task_run(NULL);
}

LOCAL void lebao_handle_pdp_msg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	if (PNULL == msg_ptr) {
		return;
	}

	helper_debug_printf("nsapi = %d, msg_id = %d, app = %d", 
		msg_ptr->nsapi,
		msg_ptr->msg_id,
		msg_ptr->app_handler);

	switch (msg_ptr->msg_id)
	{
	case MMIPDP_ACTIVE_CNF:
		if (MMIPDP_RESULT_SUCC == msg_ptr->result) {
			_lebaoNetId = msg_ptr->nsapi;
		}
		else {
			lebao_deactive_network();
#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
			if (MMIPDP_INTERFACE_WIFI == msg_ptr->ps_interface) {
				// Todo : change to GPRS
			}
#endif
		}
		break;

	case MMIPDP_DEACTIVE_CNF:
		break;

	case MMIPDP_DEACTIVE_IND: {
		lebao_deactive_network();
		break;
	}
	
	default:
		break;
	}

	MMI_CheckAllocatedMemInfo();
}

LOCAL BOOLEAN lebao_pdp_active(void)
{
	MN_DUAL_SYS_E dual_sys = lebao_get_active_sim();
	MMIPDP_ACTIVE_INFO_T    active_info = { 0 };
	BOOLEAN                 result = FALSE;
	MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

	linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, 0);

	if(linksetting == NULL)
		return FALSE;

	active_info.app_handler = MMI_MODULE_LEBAO;
	active_info.dual_sys = dual_sys;
	active_info.apn_ptr =  (char*)linksetting->apn;
	active_info.user_name_ptr = (char*)linksetting->username;
	active_info.psw_ptr = (char*)linksetting->password;
	active_info.priority = 3;
	active_info.ps_service_rat = MN_UNSPECIFIED;
	active_info.ps_interface = MMIPDP_INTERFACE_GPRS;
	active_info.handle_msg_callback = lebao_handle_pdp_msg;
	active_info.ps_service_type = BROWSER_E;
	active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
	active_info.ip_type = MMICONNECTION_IP_V4;//linksetting->ip_type;
#endif
	if (MMIAPIPDP_Active(&active_info)) {
		result = TRUE;
	}

	helper_debug_printf("result = %d", result);

	return result;
}

#if defined(LEBAO_FEATURE_PHONE_STYLE)
BOOLEAN lebao_gprs_is_opened(void)
{
#ifdef MMI_GPRS_SUPPORT
	if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
		return FALSE;
#endif
	return TRUE;
}

//-------------------------------sms-------------------------------

#include "mmisms_send.h"

extern int lebao_order_need_filter_sms(void);
extern int lebao_order_set_captcha(const char* captcha);

// sms received
static BOOLEAN _lebao_sms_captcha_filter(DPARAM param)
{
	APP_MN_SMS_IND_T *sigPtr = (APP_MN_SMS_IND_T *)param;
	APP_SMS_USER_DATA_T smsData = { 0 };
	MMIPB_BCD_NUMBER_T origNum = { 0 };

	BOOLEAN isTarget = FALSE;
	BOOLEAN findNameTag = FALSE;
	BOOLEAN findDataTag = FALSE;

	char captcha[7] = { 0 };
	uint8 tele[128] = { 0 };
	uint8 tele_len = 0;
	int i = 0, begin = 0;

	uint8  miguyinyue[8] = { 0x54, 0xaa, 0x54, 0x95, 0x97, 0xf3, 0x4e, 0x50 }; // 咪咕音乐
	uint8  yanzhengma[8] = { 0x9a, 0x8c, 0x8b, 0xc1, 0x78, 0x01, 0xff, 0x1a }; // 验证码: 

	origNum.number_len = sigPtr->sms_t.origin_addr_t.num_len;
	origNum.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sigPtr->sms_t.origin_addr_t.number_type, MN_NUM_PLAN_UNKNOW);
	SCI_MEMCPY(&origNum.number, &sigPtr->sms_t.origin_addr_t.party_num, origNum.number_len);
	tele_len = MMIAPIPB_BCDNumberToString(&origNum, 128, tele);

	MMISMS_DecodeUserHeadAndContent(
		sigPtr->dual_sys,
		sigPtr->sms_t.user_head_is_exist,
		&(sigPtr->sms_t.dcs),
		&(sigPtr->sms_t.user_data_t),
		&smsData);

	helper_debug_printf("caller=%s, length=%d", tele, smsData.user_valid_data_t.length);

	for (i = 0; i < (smsData.user_valid_data_t.length - 8); i++)
	{
		if (os_memcmp(&smsData.user_valid_data_t.user_valid_data_arr[i], miguyinyue, 8) == 0) {
			findNameTag = TRUE;
			break;
		}
	}

	for (i = 0; i < (smsData.user_valid_data_t.length - 8); i++) {
		if (os_memcmp(&smsData.user_valid_data_t.user_valid_data_arr[i], yanzhengma, 8) == 0) {
			begin = i + 7;
			findDataTag = TRUE;
			break;
		}
	}

	if (findNameTag == TRUE && findDataTag == TRUE) {
		for (i = 0; i < 6; i++) {
			captcha[i] = smsData.user_valid_data_t.user_valid_data_arr[begin + 2 + 2 * i];
		}
		captcha[6] = '\0';
		helper_debug_printf("find captcha=%s", captcha);
		isTarget = TRUE;
		lebao_order_set_captcha(captcha);
	}

	return isTarget;
}

BOOLEAN lebao_sms_captcha_filter(DPARAM param)
{
	if(lebao_order_need_filter_sms() == 0)
		return FALSE;

	return _lebao_sms_captcha_filter(param);
}

int lebao_send_sms(const MN_DUAL_SYS_E dualSysUse, const char* called, const char* content)
{
	MMI_STRING_T strContent = { 0 };
	uint8 sms[80] = { 0 }; // 140
	wchar w_sms[80] = { 0 };
	uint16 w_sms_len = 0;
	uint8 len = 0;
	BOOLEAN result = FALSE;
	MMISMS_SEND_DATA_T sendData = { 0 };

	if (called == NULL || content == NULL)
		return -1;

	helper_debug_printf("called=%s, content=%s", called, content);

	// content
	SCI_MEMSET(w_sms, 0, sizeof(w_sms));
	len = os_strlen(content);
	if (len <= 0) return -1;
	else if (len > 79) len = 79;

	os_memcpy(sms, content, len);

	MMIAPICOM_StrToWstr(sms, w_sms);
	w_sms_len = MMIAPICOM_Wstrlen(w_sms);
	if (w_sms_len <= 0)
		return -1;

	strContent.wstr_ptr = w_sms;
	strContent.wstr_len = w_sms_len;

	sendData.sms_content_ptr = &strContent;
	sendData.dst_port = 0;
	sendData.src_port = 0;
	sendData.dest_addr_ptr = (uint8 *)called;
	sendData.dest_addr_len = os_strlen(called);
	sendData.dual_sys = dualSysUse;
	sendData.is_need_packet = TRUE;
	sendData.send_callback = NULL;

	result = MMISMS_SendSmsForOther(&sendData);
	return (result == TRUE ? 0 : 1);
}

//-------------------------------call-------------------------------

int lebao_make_call(const MN_DUAL_SYS_E dualSysUse, const char* called)
{
	if (called == NULL) return -1;

	MMIAPIMENU_QuitLebao();
	MMK_ReturnIdleWin();

	return MMIAPICC_MakeCall(
		dualSysUse,
		called,
		os_strlen(called),
		PNULL,
		PNULL,
		MMIAPICC_GetCallSimType(dualSysUse),
		CC_CALL_NORMAL_CALL,
		PNULL);
}
#endif

/*
// test functions

#include "http_client.h"
static int test_http_base()
{
	int status = 0;
	do {
		http_t* request = http_client_get("http://www.baidu.com/");
		if (request == NULL) {
			return 1;
		}
		status = http_client_process(request, NULL, NULL);
		if (status != 0) {
			http_client_release(request);
			return 2;
		}

		if (sdslen(request->responseData) <= 0) {
			http_client_release(request);
			return 3;
		}

		http_client_release(request);
	} while (0);

	do {
		http_t* request = http_client_get("http://www.baidu.com/");
		if (request == NULL) {
			return 4;
		}
		status = http_client_download(request, "D:\\Others\\baidu.txt", NULL, NULL);
		if (status != 0) {
			http_client_release(request);
			return 5;
		}

		http_client_release(request);
	} while (0);


	return 0;
}

static void test_load_big_file() {
	sds buffer = lebao_load_file("D:\\Others\\record.wav");
	if (buffer == NULL || sdslen(buffer) <= 0) {
		lebao_create_msgbox_id(TEXT_LEBAO_ERR_SERVICE, 1000);
	}
	else {
		char temp[32];
		sprintf(temp, "len=%d", sdslen(buffer));
		lebao_msgbox_create(temp, 1000);
	}
	sdsfree(buffer);
}

*/

/*
#if 0
typedef struct
{
	MN_DUAL_SYS_E	dual_sys;
	uint16			dest_num_len;			// actual number length
	BOOLEAN			is_need_packet;
	uint8			dest_num[5][40 + 1];	// contact's number
	MMI_STRING_T*	sms_content_ptr;
	SMS_SEND_CALLBACK send_callback;
} lebao_mmisms_multi_send_data_t;

static int  lebao_send_sms(const char* called, const char* content)
{
	MMI_STRING_T str_content = { 0 };
	uint8 sms_content[80] = { 0 };
	wchar w_sms_content[80] = { 0 };
	uint16 wstr_len = 0;
	uint8 len = 0;
	BOOLEAN result = FALSE;
	lebao_mmisms_multi_send_data_t send_data = { 0 };
	uint8 contacts_index = 0;

	helper_debug_printf("called=%s, content=%s", called, content);

	// content
	SCI_MEMSET(w_sms_content, 0, sizeof(w_sms_content));
	len = os_strlen(content);
	if (len <= 0) return -1;
	else if (len > 79) len = 79;

	os_memcpy(sms_content, content, len);

	MMIAPICOM_StrToWstr(sms_content, w_sms_content);
	wstr_len = MMIAPICOM_Wstrlen(w_sms_content);
	if (wstr_len <= 0)
		return -1;

	str_content.wstr_len = wstr_len;
	str_content.wstr_ptr = w_sms_content;

	send_data.dual_sys = (MN_DUAL_SYS_E)_dual_sys_use;
	send_data.sms_content_ptr = &str_content;
	send_data.is_need_packet = TRUE;
	send_data.send_callback = PNULL;

	// called
	len = os_strlen(called);
	if (len > 0) {
		os_memcpy(send_data.dest_num[0], called, len);
		contacts_index++;
	}

	send_data.dest_num_len = contacts_index;
	result = MMISMS_MultiSendSms(&send_data);

	return (result == TRUE ? 0 : 1);
}
#endif
*/